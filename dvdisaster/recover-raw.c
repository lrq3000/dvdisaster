/*  dvdisaster: Additional error correction for optical media.
 *  Copyright (C) 2004-2006 Carsten Gnoerlich.
 *  Project home page: http://www.dvdisaster.com
 *  Email: carsten@dvdisaster.com  -or-  cgnoerlich@fsfe.org
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA,
 *  or direct your browser at http://www.gnu.org.
 */

#include "dvdisaster.h"

/*
 * Debugging function
 */

//#define DUMP_MODE

#ifdef DUMP_MODE
static void dump_sector(RawBuffer *rb)
{  FILE *file = fopen("test-cases/dump.h", "w");
   int i;
   
   fprintf(file, 
	   "#define SAMPLES_READ %d\n"
	   "#define SAMPLE_LENGTH %d\n"
	   "#define LBA %d\n"
	   "unsigned char cd_frame[][%d] = {\n",
	   rb->samplesRead, rb->sampleLength, rb->lba, rb->sampleLength);

   for(i=0; i<rb->samplesRead; i++)
   {  int j;

      fprintf(file, "{\n");
      for(j=0; j<rb->sampleLength; j++)
      {  fprintf(file, "%3d, ",rb->rawBuf[i][j]); 
	 if(j%16 == 15) fprintf(file, "\n");
      }
      fprintf(file, "},\n");
   }
   
   fprintf(file, "};\n");

   fclose(file);
}
#endif

/***
 *** Create our local working context
 ***/

RawBuffer *CreateRawBuffer(int sample_length)
{  RawBuffer *rb;
   int i;

   rb = g_malloc0(sizeof(RawBuffer));

   rb->gt = CreateGaloisTables(0x11d);
   rb->rt = CreateReedSolomonTables(rb->gt, 0, 1, 10);

   rb->rawBuf    = g_malloc(Closure->rawAttempts * sizeof(unsigned char*));
   rb->rawState  = g_malloc(Closure->rawAttempts * sizeof(int));
   rb->valid     = g_malloc(Closure->rawAttempts * sizeof(int));
   rb->recovered = g_malloc(sample_length);
   rb->byteState = g_malloc(sample_length);

   for(i=0; i<Closure->rawAttempts; i++)
   {  rb->rawBuf[i] = g_malloc(sample_length);
   }

   for(i=0; i<N_P_VECTORS; i++)
     rb->pList[i] = g_malloc(Closure->rawAttempts * sizeof(int));

   for(i=0; i<N_Q_VECTORS; i++)
     rb->qList[i] = g_malloc(Closure->rawAttempts * sizeof(int));

   return rb;
}

void FreeRawBuffer(RawBuffer *rb)
{  int i;

   FreeGaloisTables(rb->gt);
   FreeReedSolomonTables(rb->rt);

   for(i=0; i<Closure->rawAttempts; i++)
     g_free(rb->rawBuf[i]);

   for(i=0; i<N_P_VECTORS; i++)
     g_free(rb->pList[i]);

   for(i=0; i<N_Q_VECTORS; i++)
     g_free(rb->qList[i]);

   g_free(rb->rawBuf);
   g_free(rb->rawState);
   g_free(rb->valid);
   g_free(rb->recovered);
   g_free(rb->byteState);
   g_free(rb);
}

/***
 *** CD level CRC calculation
 ***/

/* Convert LBA sector number into MSF format */

static void lba_to_msf(int lba, unsigned char *minute, unsigned char *second, unsigned char
*frame)
{
  *frame = lba % 75;
  lba /= 75;
  lba += 2;             /* address + 150 frames */
  *second = lba % 60;
  *minute = lba / 60;
}

/* Convert byte into BCD notation */

static int int_to_bcd(int value)
{
  return ((value / 10) << 4) | (value % 10);
}

/* Initialize Sync, MSF and data mode bytes */

void InitializeCDFrame(unsigned char *cd_frame, int sector)
{  unsigned char minute, second, frame;
   int i;

   memset(cd_frame, 0, 2352);  /* defensive programming */

   /* 12 sync bytes 0x00, 0xff, 0xff, ..., 0xff, 0xff, 0x00 */

   cd_frame[0] = 0x00;
   for(i=1; i<=10; i++)
     cd_frame[i] = 0xff;
   cd_frame[11] = 0x00;

   /* MSF of sector address, BCD encoded */

   lba_to_msf(sector, &minute, &second, &frame);
   cd_frame[12] = int_to_bcd(minute);
   cd_frame[13] = int_to_bcd(second);
   cd_frame[14] = int_to_bcd(frame);

   /* Data mode */
   
   cd_frame[15] = 0x01;
}

/*
 * Test raw sector against its 32bit CRC.
 * Returns TRUE if frame is good.
 */

int CheckEDC(unsigned char *cd_frame)
{ unsigned int expected_crc, real_crc;

  /* Get CRC from CD frame (byte position 2064) */

   expected_crc =  cd_frame[0x810] << 24
                 | cd_frame[0x811] << 16
                 | cd_frame[0x812] <<  8
                 | cd_frame[0x813];

   expected_crc = SwapBytes32(expected_crc);  /* CRC on disc is big endian */

   real_crc = EDCCrc32(cd_frame, 2064);

   return expected_crc == real_crc;
}

/***
 *** The majority decision algorithm (currently unused)
 ***/   

//#define DEBUG_MAJ

#if 0
static int majority_decision(unsigned char *cd_frame, RawBuffer *rb)
{  int byte_votes[256];
   int byte_nominations[256];
   int buffer_score[rb->samplesRead];
   unsigned char tie[rb->sampleLength];
   int smallest = 256;
   int ties_encountered = FALSE;
   int i,j;

#ifdef DEBUG_MAJ
   Verbose("Doing majority decision for %d samples\n", rb->samplesRead);
#endif

   /*** We need at last three samples for a majority */

   if(rb->samplesRead < 3)
     return FALSE;

   /*** If a read buffer contributed a byte to a winning votes,
	its score get upped by one. Read buffers which have been
	flagged as valid by the drive start with a score of
	rb->sampleLength (1 point for each byte contained). */

   for(i=0; i<rb->samplesRead; i++)
     if(rb->rawState == RAW_SUCCESS)  
           buffer_score[i] = rb->sampleLength;
     else  buffer_score[i] = 0;

   /*** Reset the number of ties */

   memset(tie, 0, rb->sampleLength);

   /*** Count votes for each byte */

   for(i=0; i<rb->sampleLength; i++)
   {  unsigned char chosen_byte = 0;
      int max_vote = -1;

      memset(byte_votes, 0, 256*sizeof(int));

      /* See how often each byte appeared */

      for(j=0; j<rb->samplesRead; j++)
	 byte_votes[rb->rawBuf[j][i]]++;

      /* Determine which byte was found most */

      for(j=0; j<256; j++)
	if(byte_votes[j] > max_vote) 
	{  max_vote = byte_votes[j];
	   chosen_byte = j;
	   tie[i] = FALSE;
	}
        else 
	  if(byte_votes[j] == max_vote)
	    tie[i] = TRUE;

      /* If no tie was detected, use the chosen byte.
	 Otherwise put off decision for later. */

      if(tie[i] == 0)
      {  cd_frame[i] = chosen_byte;

	 for(j=0; j<rb->samplesRead; j++)  /* Raise score for winning buffers */
	   if(rb->rawBuf[j][i] == chosen_byte)
	     buffer_score[j]++;

	 if(smallest > byte_votes[chosen_byte]) /* debugging stats */
	   smallest = byte_votes[chosen_byte];
      }
      else
      {  ties_encountered = TRUE;
      }
   }

#ifdef DEBUG_MAJ
   Verbose("smallest majority was %d.\n", smallest);
#endif

   /*** Resolve ties.
	If two or more bytes got the same number of votes,
	vote again giving each buffer buffer_score[j] votes 
	instead of one. */

   if(ties_encountered)
   {  
#ifdef DEBUG_MAJ
      Verbose("resolving ties...\n");
#endif

      for(i=0; i<rb->sampleLength; i++)
      {  if(tie[i])
	 {  int max_votes = -1;
	    int max_nominations = -1;
	    int chosen_byte = 0;

#ifdef DEBUG_MAJ
	    Verbose("Tie for byte %4x:\n", i);
#endif
	    /* Vote again by counting the number of nominations
	       of each bytes (= buffers containing it) and the
	       score of its voting buffers. The byte winning in
	       both categories is selected. */

	    memset(byte_votes, 0, 256*sizeof(int));
	    memset(byte_nominations, 0, 256*sizeof(int));

	    for(j=0; j<rb->samplesRead; j++)
	    {  byte_votes[rb->rawBuf[j][i]] += buffer_score[j];
	       byte_nominations[rb->rawBuf[j][i]]++;
#ifdef DEBUG_MAJ
	       Verbose(" %2x [%d]\n", rb->rawBuf[j][i], buffer_score[j]);
#endif
	    }

	    /* Pick byte with highest score,
	       if a tie happens again, ignore. */

	    for(j=0; j<256; j++)
	    {  if(byte_nominations[j] > max_nominations)
	       {  max_nominations = byte_nominations[j];
		  max_votes = byte_votes[j];
		  chosen_byte = j;
	       }
	       else if(   byte_nominations[j] == max_nominations 
		       && byte_votes[j] > max_votes) 
	       {  max_votes = byte_votes[j];
		  chosen_byte = j;
	       }
	    }

	    cd_frame[i] = chosen_byte;
#ifdef DEBUG_MAJ
	    Verbose("picked %2x with %d nominations and %d votes\n", 
		    chosen_byte, max_nominations, max_votes);
#endif
	 }
      }
   }
   
   return TRUE;
}
#endif

/***
 *** Detection of unplausible sectors.
 ***/

//#define DEBUG_VALIDATION 1

/* 
 * Throw out those sectors which are obviously wrong
 * by examining their MSF.
 */

static void validate_by_msf(RawBuffer *rb)
{  unsigned char minute,second,frame;
   int last = rb->samplesRead - 1;

   if(!rb->samplesRead)
     return;

   lba_to_msf(rb->lba, &minute, &second, &frame);
   minute = int_to_bcd(minute);
   second = int_to_bcd(second);
   frame = int_to_bcd(frame);

   if(   rb->rawBuf[last][12] != minute
      || rb->rawBuf[last][13] != second	
      || rb->rawBuf[last][14] != frame)
   {   
#ifdef DEBUG_VALIDATION
       printf("MSF mismatch for sector %d: %2d%2d%2d / %2d%2d%2d\n",
	      last, minute, second, frame,
	      rb->rawBuf[last][12], rb->rawBuf[last][13], rb->rawBuf[last][14]);
#endif
       rb->samplesRead--; 
   }
}

/* The drive might have lost track and returned the wrong sector.
 * We build a difference matrix containing the pairwise byte 
 * differences of all sectors gathered so far, then mark those 
 * sectors as bad whose average difference exceeds the median 
 * plus some safety margin.
 * Note that the bad sectors are just marked but not deleted as
 * we might be unlucky to start off with a batch of bad sectors.
 * If the good sectors come in later, we might have to revise our
 * decision.  
 */

static int int_compare(const void *a, const void *b)
{  return (*(int*)a) - (*(int*)b);
}

static void validate_by_difference(RawBuffer *rb)
{  int matrix_size = rb->samplesRead*rb->samplesRead;
   int score[matrix_size];
   int col_avg[rb->samplesRead];
   int col_avg_sorted[matrix_size];
   int i,j,k;
   int margin,median;

   if(rb->samplesRead < 2)  /* Nothing to do yet */
     return;

   for(i=0; i<rb->samplesRead; i++)
     rb->valid[i] = TRUE;  /* for initialization */ 

   /* Examine sectors based on their byte differences */

   memset(score, 0, sizeof(score));

   for(i=0; i<rb->samplesRead; i++)
   { col_avg[i] = 0;
     for(j=i; j<rb->samplesRead; j++)
     {  score[i*rb->samplesRead+j] = 0;
        for(k=0; k< rb->sampleLength; k++)
	  if(rb->rawBuf[i][k] != rb->rawBuf[j][k])
	    score[i*rb->samplesRead+j]++;
	score[j*rb->samplesRead+i] = score[i*rb->samplesRead+j];
     }

     for(j=0; j<rb->samplesRead; j++)
       col_avg[i]+=score[i*rb->samplesRead+j];

     col_avg[i] /= (rb->samplesRead-1);

#ifdef DEBUG_VALIDATION
     for(j=0; j<rb->samplesRead; j++)
	printf("%5d",score[i*rb->samplesRead+j]);
     printf("\n");
#endif
   }

   /* Calculate median of differences;
      skip rb->samplesRead elements for the zero diagonal */

   memcpy(col_avg_sorted, col_avg, sizeof(col_avg));
   qsort(col_avg_sorted, rb->samplesRead, sizeof(int), int_compare);
   if(rb->samplesRead & 1) 
        median = col_avg_sorted[rb->samplesRead/2];
   else median = (col_avg_sorted[rb->samplesRead/2] + col_avg_sorted[rb->samplesRead/2-1]) / 2;

   margin = median + median/2;  /* margin = 1.5*median */

#ifdef DEBUG_VALIDATION
   printf("----\n");
   for(i=0; i<rb->samplesRead; i++)
     printf("%5d", col_avg_sorted[i]);

   printf("\n");

   for(i=0; i<rb->samplesRead; i++)
     printf("%5d",col_avg[i]);

   printf(" - median %d, margin %d\n", median, margin);
#endif

   /* Sectors whose difference exceeds the median for floor(rb->samplesRead) times
      are marked as invalid; the drive might have delivered a wrong sector here. */

   for(i=0; i<rb->samplesRead; i++)
     if(col_avg[i] > margin)
     {  rb->valid[i] = FALSE; 
#ifdef DEBUG_VALIDATION
	printf("rejecting sector %d\n", i);
#endif
     }
}

/***
 *** Analyse P and Q vectors for each sector.
 ***
 * After this stage, we have:
 *
 * - marked the frame bytes in rb->byteState are as following;
 *  3 = Byte lies at intersection of good P and Q vector 
 *  2 = Byte lies at good P vector from above
 *  1 = Byte lies at good Q vector from above
 *  0 = Byte is uncorrected 
 *
 * - recorded bytes with a score better than 0 in rb->recovered;
 *
 * - collected all P and Q vectors which pretend to be correct
 *   or correctable in rb->pList and rb->qList.
 */

//#define DEBUG_ANALYSE

static int analyse_pq(RawBuffer *rb)
{  int score[4];
   int p_miss,q_miss,p_clash,q_clash;
   int i,f;

   /* Initially all bytes are considered as bad
      and none are recovered */

   memset(rb->byteState, 0, rb->sampleLength);
   InitializeCDFrame(rb->recovered, rb->lba);

   memset(rb->pIndex, 0, sizeof(rb->pIndex));
   memset(rb->qIndex, 0, sizeof(rb->qIndex));

   /* Iterate over all non-rejected sectors */

   for(f=0; f<rb->samplesRead; f++)
   {  unsigned char frame_state[rb->sampleLength];
      unsigned char frame_byte[rb->sampleLength];
      unsigned char vector[Q_VECTOR_SIZE];
      unsigned char compare[Q_VECTOR_SIZE];
      int erasures[2];
      int i,err,v;

      if(!rb->valid[f])
	 continue;

      memset(frame_state, 0, sizeof(frame_state));

      /*** Search for undamaged Q vectors in the frame */

      for(v=0; v<N_Q_VECTORS; v++)
      {  GetQVector(rb->rawBuf[f], vector, v);

	 err = DecodePQ(rb->rt, vector, Q_PADDING, erasures, 0);
	 if(err >= 0 && err <= 2)    /* Good vector */
	 {  
	    /* Record all different q vectors which claim to
	       be correct(able). */

	    if(rb->qIndex[v])
	    {  GetQVector(frame_byte, compare, v);
	       if(memcmp(compare, vector, Q_VECTOR_SIZE))
		 rb->qList[v][rb->qIndex[v]++] = f;
	    }
	    else  /* record first q vector found */ 
	    {  rb->qList[v][rb->qIndex[v]++] = f;

	       SetQVector(frame_byte, vector, v);
	    }

	    OrQVector(frame_state, 1, v);
	 }
      }

      /*** Search for undamaged P vectors in the frame */

      for(v=0; v<N_P_VECTORS; v++)
      {  GetPVector(rb->rawBuf[f], vector, v);

	 err = DecodePQ(rb->rt, vector, P_PADDING, erasures, 0);
	 if(err >= 0 && err <= 2) /* Good vector */
	 {  

	    /* Record all different p vectors which claim to
	       be correct(able). */

	    if(rb->pIndex[v])
	    {  GetPVector(frame_byte, compare, v);
	       if(memcmp(compare, vector, P_VECTOR_SIZE))
		 rb->pList[v][rb->pIndex[v]++] = f;
	    }
	    else /* record first p vector found */ 
	    {  rb->pList[v][rb->pIndex[v]++] = f;

	       SetPVector(frame_byte, vector, v);
	    }

	    OrPVector(frame_state, 2, v);
	 }
      }

      /*** Now the frame bytes are marked as following:
	   3 = Byte lies at intersection of good P and Q vector 
	   2 = Byte lies at P vector from above
	   1 = Byte lies at Q vector from above
	   0 = Byte is uncorrected */
      
      score[0] = score[1] = score[2] = score[3] = 0;

      for(i=12; i<rb->sampleLength; i++)
      {  int s = frame_state[i];

	 if(s > rb->byteState[i]) 
	 {  rb->recovered[i] = frame_byte[i];
	    rb->byteState[i] = s;
	 }
	 score[s]++;
      }
      
      printf("Frame %d: [3/2/1/0]=[%d/%d/%d/%d]\n", f, score[3], score[2], score[1], score[0]);
   }

   /* Count total byte states */

   score[0] = score[1] = score[2] = score[3] = 0;

   for(i=12; i<rb->sampleLength; i++)
   {  int s = rb->byteState[i];

      score[s]++;
   }

   printf("Recovered: [3/2/1/0]=[%d/%d/%d/%d]\n", score[3], score[2], score[1], score[0]);

   /* Evaluate p/q vector state */
   
   p_miss = q_miss = p_clash = q_clash = 0;

   for(i=0; i<N_P_VECTORS; i++)
   {  if(rb->pIndex[i] == 0) p_miss++;
      if(rb->pIndex[i] > 1) p_clash++;
   }

   for(i=0; i<N_Q_VECTORS; i++)
   {  if(rb->qIndex[i] == 0) q_miss++;
      if(rb->qIndex[i] > 1) q_clash++;
   }

   printf("P/Q vectors: %d/%d missing, %d/%d ambiguous\n", p_miss, q_miss, p_clash, q_clash);

#ifdef DEBUG_ANALYSE
   for(i=0; i<N_P_VECTORS; i++)
   {  unsigned char vector[P_VECTOR_SIZE];
      GetPVector(rb->recovered, vector, i);
      PrintVector(vector, P_VECTOR_SIZE, i);
      GetPVector(rb->byteState, vector, i);
      PrintVector(vector, P_VECTOR_SIZE, i);
   }
   for(i=0; i<N_Q_VECTORS; i++)
   {  unsigned char vector[Q_VECTOR_SIZE];
      GetQVector(rb->recovered, vector, i);
      PrintVector(vector, Q_VECTOR_SIZE, i);
      GetQVector(rb->byteState, vector, i);
      PrintVector(vector, Q_VECTOR_SIZE, i);
   }
#endif

   return score[0] == 0;
}

/***
 *** Try to cut down P/Q vector alternatives
 ***
 * If there is more than one alternative for a specific P/Q vector,
 * try to find the right one.
 */

//#define DEBUG_DISAMBIGUATE

static void disambiguate_vectors(RawBuffer *rb)
{  unsigned char vector[Q_VECTOR_SIZE];
   unsigned char hint[Q_VECTOR_SIZE];
   int i,j,k,err;
   int erasures[2];
   int misses;
   int min_misses, best_vector;

   for(i=0; i<N_P_VECTORS; i++)
   {   
       if(rb->pIndex[i] < 2)
       continue;

#ifdef DEBUG_DISAMBIGUATE
       g_printf("%d alternatives for P-vector %d\n", rb->pIndex[i], i);
#endif

       best_vector = 0;
       min_misses = P_VECTOR_SIZE;

       for(j=0; j<rb->pIndex[i]; j++)
       {  GetPVector(rb->rawBuf[rb->pList[i][j]], vector, i);
#ifdef DEBUG_DISAMBIGUATE
 	  PrintVector(vector, P_VECTOR_SIZE, i);
#endif
	  misses = 0;

	  memset(hint, 0, sizeof(hint));
	  SetPVector(rb->recovered, vector, i);
	  for(k=0; k<P_VECTOR_SIZE; k++)
	  {  int byte_index = PToByteIndex(i,k);
	     int crossing_q,crossing_q_index;
	     
	     ByteIndexToQ(byte_index, &crossing_q, &crossing_q_index);
	     GetQVector(rb->recovered, vector, crossing_q);
	     err = DecodePQ(rb->rt, vector, Q_PADDING, erasures, 0);
	     if(err < 0 || err > 1)
	       misses++;
	     else hint[k]++;
	  }
#ifdef DEBUG_DISAMBIGUATE
	  PrintVector(hint, P_VECTOR_SIZE, i); 
	  err = DecodePQ(rb->rt, vector, P_PADDING, erasures, 0);
	  g_printf("index %d; misses = %d; %d errors\n", j, misses, err);
#endif
	  if(misses < min_misses)
	  {  min_misses = misses;
	     best_vector = j;
	  }
       }
       GetPVector(rb->rawBuf[rb->pList[i][best_vector]], vector, i);
       SetPVector(rb->recovered, vector, i); 

#ifdef DEBUG_DISAMBIGUATE
       g_printf("Best vector: %d\n", best_vector);
#if 1
       GetPVector(rb->recovered, vector, i);
       PrintVector(vector, P_VECTOR_SIZE, i);
#endif
       GetPVector(rb->byteState, vector, i);
       PrintVector(vector, P_VECTOR_SIZE, i);
#endif
   }

   /* TODO: this :-) */

#if 0
   for(i=0; i<N_Q_VECTORS; i++)
   {  if(rb->qIndex[i] < 2)
       continue;
     
       g_printf("%d alternatives for Q-vector %d\n", rb->qIndex[i], i);
       for(j=0; j<rb->qIndex[i]; j++)
       {  GetQVector(rb->rawBuf[rb->qList[i][j]], vector, i);
 	  PrintVector(vector, Q_VECTOR_SIZE, i);
	  err = DecodePQ(rb->rt, vector, Q_PADDING, erasures, 0);
	  printf("%d errors\n", err);
       }
       GetQVector(rb->recovered, vector, i);
       PrintVector(vector, Q_VECTOR_SIZE, i);
       GetQVector(rb->byteState, vector, i);
       PrintVector(vector, Q_VECTOR_SIZE, i);
   }
#endif
}

/***
 *** Try to correct remaining bytes in rb->recovered.
 ***
 * Iterate over P and Q recovery until no further improvements are made.
 */

int iterative_recovery(RawBuffer *rb)
{  unsigned char p_vector[P_VECTOR_SIZE];
   unsigned char q_vector[Q_VECTOR_SIZE];
   unsigned char p_state[P_VECTOR_SIZE];
   unsigned char q_state[Q_VECTOR_SIZE];
   int erasures[Q_VECTOR_SIZE], erasure_count;
   int p_failures, q_failures;
   int p_corrected, q_corrected;
   int p,q;
   int last_p_failures = N_P_VECTORS;
   int last_q_failures = N_Q_VECTORS;
   int iteration=1;
   int score[4];

   /* Maybe the sector can be corrected by L-EC after the majority decision? */


   for(; ;) /* iterate over P- and Q-Parity until failures converge */
   {	
      p_failures = q_failures = 0;
      p_corrected = q_corrected = 0;

      /* Perform Q-Parity error correction */

      erasure_count = 0;

      for(q=0; q<N_Q_VECTORS; q++)
      {  int i,err;

	 /* Determine number of erasures */

	 GetQVector(rb->byteState, q_state, q);
	 erasure_count = 0;

	 for(i=0; i<Q_VECTOR_SIZE; i++)
	   if(!q_state[i])
	     erasures[erasure_count++] = i;

	 /* Erasure values >2 are not helpful,
	    but it is not certain that these bytes are wrong at all.
	    Lets give it a try anyways. */

	 if(erasure_count > 2)
	   erasure_count = 0;

	 /* Try error correction */

	 GetQVector(rb->recovered, q_vector, q);
	 err = DecodePQ(rb->rt, q_vector, Q_PADDING, erasures, erasure_count);

	 if(err < 0)  /* Uncorrectable. Mark bytes are erasure. */
	 {  q_failures++;
//	    FillQVector(rb->byteState, FRAME_BYTE_ERROR, q);
	 }
	 else 
	 {  if(err == 1 || err == 2) /* Store back corrected vector */ 
	    {  SetQVector(rb->recovered, q_vector, q);
	       RaiseQVector(rb->byteState, 1, q);
	       q_corrected++;
	    }
	 }
      }

      /* Perform P-Parity error correction */

      for(p=0; p<N_P_VECTORS; p++)
      {  int err,i;

	/* Determine number of erasures */

	GetPVector(rb->byteState, p_state, p);
	erasure_count = 0;

	for(i=0; i<P_VECTOR_SIZE; i++)
	  if(!p_state[i])
	    erasures[erasure_count++] = i;

	/* Erasure values >2 are not helpful,
	   but it is not certain that these bytes are wrong at all.
	   Lets give it a try anyways. */

	if(erasure_count > 2)
	  erasure_count = 0;
	
	/* Try error correction */

	GetPVector(rb->recovered, p_vector, p);
	err = DecodePQ(rb->rt, p_vector, P_PADDING, erasures, erasure_count);

	if(err < 0)  /* Uncorrectable. */
	{  p_failures++;
//	   FillPVector(rb->byteState, FRAME_BYTE_ERROR, p);
	}
	else 
	  {  if(err == 1 || err == 2) /* Store back corrected vector */ 
	     {  SetPVector(rb->recovered, p_vector, p);
	        RaisePVector(rb->byteState, 1, p);
		p_corrected++;
	     }
	  }
      }

      /* See if there was an improvement */

      printf("L-EC: iteration %d\n", iteration); 
      printf("      Q-failures/corrected: %2d/%2d\n", q_failures, q_corrected);
      printf("      P-failures/corrected: %2d/%2d\n", p_failures, p_corrected);

      if(p_failures + q_failures == 0)
	break;

      if(   last_p_failures > p_failures
	 || last_q_failures > q_failures)
      {  last_p_failures = p_failures;
	 last_q_failures = q_failures;
	 iteration++;
      }
      else break;
   }

   score[0] = score[1] = score[2] = score[3] = 0;

   for(p=12; p<rb->sampleLength; p++)
   {  int s = rb->byteState[p];

      score[s]++;
   }

   printf("Recovered: [3/2/1/0]=[%d/%d/%d/%d]\n", score[3], score[2], score[1], score[0]);


   return (p_failures + q_failures == 0);
}

/***
 *** Perform analysis and recovery of raw read data.
 ***
 * RawBuffer contains the following:
 * 
 * rb->rawBuf[]       the samples from the raw read attempts, unscrambled
 * rb->samplesRead    the number of samples in the array above
 * rb->sampleLength   the length of each sample
 * rb->lba            sector we are currently working on
 *
 * e.g. rb->rawBuf contains samplesRead samples of sampleLength bytes,
 * and the max array dimensions are rb->rawBuf[samplesRead][sampleLength]
 *
 * rb->rawState       the drives comments about the respective reading attempt:
 *                    RAW_SUCCESS    - drive considered read to be good
 *                    RAW_READ_ERROR - drive could not correct the sector,
 *                                     but something was read
 * rb->valid          outcome of validate_sectors()
 *
 * rb->recovered      working buffer for recovering the CD frame, unscrambled
 * rb->byteState      state of bytes in rb->recovered as a guidance to error correction
 * rb->pList          list of p vectors pretending to be good
 * rb->qList          list of q vectors pretending to be good
 * rb->pindex         length of above
 * rb->qindex         length of above
 *
 * Return TRUE if data was recovered successfully, else return FALSE.
 */

int RecoverRaw(unsigned char *out, RawBuffer *rb)
{  int success;

   /* Reject unplausible sectors */

   if(rb->samplesRead < 1) return FALSE;

   validate_by_msf(rb);

   printf("RecoverRaw() for %d samples.\n", rb->samplesRead);

#ifdef DUMP_MODE
   if(rb->attempt == Closure->rawAttempts - 1)
     dump_sector(rb);
#endif

   /* We need at least 3 successful reads to do anything */

   if(rb->samplesRead < 3) return FALSE;

#if 0   /* maybe not needed at all */
   validate_by_difference(rb);
#else
   int i;
   for(i=0; i<rb->samplesRead; i++)
     rb->valid[i] = TRUE;
#endif

   /* Collect P and Q statistics for each sector */

   success = analyse_pq(rb);

   if(success && CheckEDC(rb->recovered))
   {  Verbose("Sector %d recovered after L-EC analysis (stage 1/5).\n", rb->lba);

      memcpy(out, rb->recovered+16, 2048);
      return TRUE;
   }

   disambiguate_vectors(rb);

   /* Try iterative L-EC */

   success = iterative_recovery(rb);

   if(success && CheckEDC(rb->recovered))
   {  Verbose("Sector %d recovered after iterative L-EC (stage 2/5).\n", rb->lba);

      memcpy(out, rb->recovered+16, 2048);
#ifdef DUMP_MODE
      return FALSE;
#else
      return TRUE;
#endif
   }
#ifdef DUMP_MODE
   return FALSE;
#endif
   /* I have another idea for a heuristic search here ... (stage 3/5) */

   /* Try iterative L-EC */
#if 1 
   Level1_L_EC(rb);

   if(CheckEDC(rb->recovered))
   {  Verbose("Sector %d recovered after iterative L-EC (stage 4/11).\n", rb->lba);

      memcpy(out, rb->recovered+16, 2048);
      return TRUE;
   }   

   Level2_L_EC(rb->recovered, rb, out);

   if(CheckEDC(rb->recovered))
   {  Verbose("Sector %d recovered after heuristic L-EC (stage 5/11).\n", rb->lba);

      memcpy(out, rb->recovered+16, 2048);
      return TRUE;
   }   
#endif

   /* Heuristically search for the most probable sector */
   SearchBestSector(rb);
   
   if(CheckEDC(rb->recovered)) 
   {  Verbose("Sector %d recovered after best sector search (stage 6/11).\n", rb->lba);
      memcpy(out, rb->recovered+16, 2048);
      return TRUE; 
   }

   /* Try simple L-EC */
#if 1
	Level1_L_EC(rb);

   if(CheckEDC(rb->recovered))
   {  Verbose("Sector %d recovered after best sector search and iterative L-EC (stage 7/11).\n", rb->lba);
      memcpy(out, rb->recovered+16, 2048);
      return TRUE;
   }

   /* Try heuristic L-EC */

   Level2_L_EC(rb->recovered, rb, out);

   if(CheckEDC(rb->recovered))
   {  Verbose("Sector %d recovered after best sector search and heuristic L-EC (stage 8/11).\n", rb->lba);
      memcpy(out, rb->recovered+16, 2048);
      return TRUE;
   }
#endif

   /* Heuristically search for the most probable sector */
   SearchPlausibleSector(rb);
   
   if(CheckEDC(rb->recovered)) 
   {  Verbose("Sector %d recovered after plausible sector search (stage 9/11).\n", rb->lba);
      memcpy(out, rb->recovered+16, 2048);
      return TRUE; 
   }

   /* Try simple L-EC */
#if 1
	Level1_L_EC(rb);

   if(CheckEDC(rb->recovered))
   {  Verbose("Sector %d recovered after plausible sector search and iterative L-EC (stage 10/11).\n", rb->lba);
      memcpy(out, rb->recovered+16, 2048);
      return TRUE;
   }

   /* Try heuristic L-EC */

   Level2_L_EC(rb->recovered, rb, out);

   if(CheckEDC(rb->recovered))
   {  Verbose("Sector %d recovered after plausible sector search and heuristic L-EC (stage 11/11).\n", rb->lba);
      memcpy(out, rb->recovered+16, 2048);
      return TRUE;
   }
#endif

   return FALSE;
}
