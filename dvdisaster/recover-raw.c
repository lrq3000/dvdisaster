/*  dvdisaster: Additional error correction for optical media.
 *  Copyright (C) 2004-2006 Carsten Gnoerlich.
 *  Copyright (C) 2006 Andrei Grecu
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

   rb->dataOffset = 16;  /* default for mode1 data sectors */

   rb->workBuf   = CreateAlignedBuffer(2352*16);  /* max lba...lba+15 are read at once */
   rb->zeroSector= g_malloc0(2352);
   rb->rawBuf    = g_malloc(Closure->maxReadAttempts * sizeof(unsigned char*));
   rb->recovered = g_malloc(sample_length);
   rb->byteState = g_malloc(sample_length);

   for(i=0; i<Closure->maxReadAttempts; i++)
   {  rb->rawBuf[i] = g_malloc(sample_length);
   }

   for(i=0; i<N_P_VECTORS; i++)
   {  rb->pParity1[i] = g_malloc(Closure->maxReadAttempts);
      rb->pParity2[i] = g_malloc(Closure->maxReadAttempts);
   }

   for(i=0; i<N_Q_VECTORS; i++)
   {  rb->qParity1[i] = g_malloc(Closure->maxReadAttempts);
      rb->qParity2[i] = g_malloc(Closure->maxReadAttempts);
   }


   rb->pLoad = g_malloc0(Closure->maxReadAttempts * sizeof(int));
   rb->qLoad = g_malloc0(Closure->maxReadAttempts * sizeof(int));

   return rb;
}

void ResetRawBuffer(RawBuffer *rb)
{  int i;

   rb->samplesRead = 0;

   for(i=0; i<N_P_VECTORS; i++)
     rb->pParityN[i][0] = rb->pParityN[i][1] = 0;

   for(i=0; i<N_Q_VECTORS; i++)
     rb->qParityN[i][0] = rb->qParityN[i][1] = 0;
}

void FreeRawBuffer(RawBuffer *rb)
{  int i;

   FreeGaloisTables(rb->gt);
   FreeReedSolomonTables(rb->rt);

   for(i=0; i<Closure->maxReadAttempts; i++)
     g_free(rb->rawBuf[i]);

   for(i=0; i<N_P_VECTORS; i++)
   {  g_free(rb->pParity1[i]);
      g_free(rb->pParity2[i]);
   }

   for(i=0; i<N_Q_VECTORS; i++)
   {  g_free(rb->qParity1[i]);
      g_free(rb->qParity2[i]);
   }

   g_free(rb->pLoad);
   g_free(rb->qLoad);

   FreeAlignedBuffer(rb->workBuf);
   g_free(rb->zeroSector);
   g_free(rb->rawBuf);
   g_free(rb->recovered);
   g_free(rb->byteState);
   g_free(rb);
}

/***
 *** CD MSF address calculations
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

/*
 * Validate the MSF field contents. 
 * Returns TRUE if the given lba matches the MSF field.
 */

static int check_msf(unsigned char *frame, int lba)
{  unsigned char min,sec,frm;

   lba_to_msf(lba, &min, &sec, &frm);
   min = int_to_bcd(min);
   sec = int_to_bcd(sec);
   frm = int_to_bcd(frm);

   if(   frame[12] != min
      || frame[13] != sec	
      || frame[14] != frm)
     return FALSE;

   return TRUE;
}

/*
 * Initialize Sync, MSF and data mode bytes 
 */

static unsigned char sync_pattern[12] = 
{ 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0 };

static void initialize_cd_frame(unsigned char *cd_frame, int sector)
{  unsigned char minute, second, frame;

   memset(cd_frame, 0, 2352);  /* defensive programming */

   /* 12 sync bytes 0x00, 0xff, 0xff, ..., 0xff, 0xff, 0x00 */

   memcpy(cd_frame, sync_pattern, 12);

   /* MSF of sector address, BCD encoded */

   lba_to_msf(sector, &minute, &second, &frame);
   cd_frame[12] = int_to_bcd(minute);
   cd_frame[13] = int_to_bcd(second);
   cd_frame[14] = int_to_bcd(frame);

   /* Data mode */
   
   cd_frame[15] = 0x01;
}

/*
 * Returns TRUE is at least 75% of the frame sync bytes match the sync pattern.
 */

static int check_for_sync_pattern(unsigned char *new_frame)
{  int matches = 0;
   int i;

   for(i=0; i<12; i++)
     if(new_frame[i] == sync_pattern[i])
       matches++;

   return matches >= 8;
}

/***
 *** CD level CRC calculation
 ***/

/*
 * Test raw sector against its 32bit CRC.
 * Returns TRUE if frame is good.
 */

static int check_edc(unsigned char *cd_frame, int xa_mode)
{ unsigned int expected_crc, real_crc;

   /* Get CRC from CD frame (byte position 2064) */

   if(xa_mode)
   {  expected_crc =  cd_frame[2072] << 24
                    | cd_frame[2073] << 16
                    | cd_frame[2074] <<  8
                    | cd_frame[2075];
   }
   else
   {  expected_crc =  cd_frame[0x810] << 24
                    | cd_frame[0x811] << 16
                    | cd_frame[0x812] <<  8
                    | cd_frame[0x813];
   }

   expected_crc = SwapBytes32(expected_crc);  /* CRC on disc is big endian */

   if(xa_mode) real_crc = EDCCrc32(cd_frame+16, 2056);
   else        real_crc = EDCCrc32(cd_frame, 2064);

   return expected_crc == real_crc;
}

/***
 *** A very simple L-EC error correction.
 ***
 * Perform just one pass over the Q and P vectors to see if everything
 * is okay respectively correct minor errors. This is pretty much the
 * same stuff the drive is supposed to do in the final L-EC stage.
 */

static int simple_lec(RawBuffer *rb, unsigned char *frame)
{  unsigned char byte_state[rb->sampleLength];
   unsigned char p_vector[P_VECTOR_SIZE];
   unsigned char q_vector[Q_VECTOR_SIZE];
   unsigned char p_state[P_VECTOR_SIZE];
   int erasures[Q_VECTOR_SIZE], erasure_count;
   int p_failures, q_failures;
   int p_corrected, q_corrected;
   int p,q;

   /* Setup */

   memset(byte_state, 0, rb->sampleLength);

   p_failures = q_failures = 0;
   p_corrected = q_corrected = 0;

   /* Perform Q-Parity error correction */

   for(q=0; q<N_Q_VECTORS; q++)
   {  int err;

      /* We have no erasure information for Q vectors */

     GetQVector(frame, q_vector, q);
     err = DecodePQ(rb->rt, q_vector, Q_PADDING, erasures, 0);

     /* See what we've got */

     if(err < 0)  /* Uncorrectable. Mark bytes are erasure. */
     {  q_failures++;
        FillQVector(byte_state, 1, q);
     }
     else         /* Correctable */ 
     {  if(err == 1 || err == 2) /* Store back corrected vector */ 
	{  SetQVector(frame, q_vector, q);
	   q_corrected++;
	}
     }
   }

   /* Perform P-Parity error correction */

   for(p=0; p<N_P_VECTORS; p++)
   {  int err,i;

      /* Try error correction without erasure information */

      GetPVector(frame, p_vector, p);
      err = DecodePQ(rb->rt, p_vector, P_PADDING, erasures, 0);

      /* If unsuccessful, try again using erasures.
	 Erasure information is uncertain, so try this last. */

      if(err < 0 || err > 2)
      {  GetPVector(byte_state, p_state, p);
	 erasure_count = 0;

	 for(i=0; i<P_VECTOR_SIZE; i++)
	   if(p_state[i])
	     erasures[erasure_count++] = i;

	 if(erasure_count > 0 && erasure_count <= 2)
	 {  GetPVector(frame, p_vector, p);
	    err = DecodePQ(rb->rt, p_vector, P_PADDING, erasures, erasure_count);
	 }
      }

      /* See what we've got */

      if(err < 0)  /* Uncorrectable. */
      {  p_failures++;
      }
      else         /* Correctable. */ 
      {  if(err == 1 || err == 2) /* Store back corrected vector */ 
	 {  SetPVector(frame, p_vector, p);
	    p_corrected++;
	 }
      }
   }

   /* Sum up */

   if(q_failures || p_failures || q_corrected || p_corrected)
   {
     PrintCLIorLabel(Closure->status, 
		     "Sector %d  L-EC P/Q results: %d/%d failures, %d/%d corrected.\n",
		     rb->lba, p_failures, q_failures, p_corrected, q_corrected);
     return 1;
   }

   return 0;
}


/***
 *** Validate CD raw sector
 ***/

int ValidateRawSector(RawBuffer *rb, unsigned char *frame)
{  int lec_did_sth = FALSE;
   unsigned char saved_msf[4];

  /* Do simple L-EC.
     It seems that drives stop their internal L-EC as soon as the
     EDC is okay, so we may see uncorrected errors in the parity bytes.
     Since we are also interested in the user data only and doing the
     L-EC is expensive, we skip our L-EC as well when the EDC is fine. */

  if(rb->xaMode)
  {  memcpy(saved_msf, frame+12, 4);
     memset(frame+12, 0, 4);
  }

  if(!check_edc(frame, rb->xaMode))
    lec_did_sth = simple_lec(rb, frame);


  if(rb->xaMode)
    memcpy(frame+12, saved_msf, 4);

  /* Test internal sector checksum again */

  if(!check_edc(frame, rb->xaMode))
  {  RememberSense(3, 255, 1);  /* EDC failure in RAW sector */
       return FALSE;
  }

  /* Test internal sector address */

  if(!check_msf(frame, rb->lba))
  {  RememberSense(3, 255, 2);  /* Wrong MSF in RAW sector */
     return FALSE;
  }

  /* Tell user that L-EC succeeded */

  if(lec_did_sth)
    PrintCLIorLabel(Closure->status, 
		    "Sector %d: Recovered in raw reader by L-EC.\n",
		    rb->lba);

   return TRUE;
}

/***
 *** Try to recover a raw CD frame sample.
 ***/

/*
 * Customized RS decoding.
 *
 * Erasure information is uncertain,
 * so we try to correct with erasure information as well as without.
 * 
 * Returns the number of corrected errors or 3 if correction failed.
 */

static int p_decode(RawBuffer *rb, unsigned char *vector, unsigned char *state)
{ int erasures[P_VECTOR_SIZE];
  unsigned char working_vector[P_VECTOR_SIZE];
  int err, erasure_count;

  /* Try error correction without erasure information */

  memcpy(working_vector, vector, P_VECTOR_SIZE);
  err = DecodePQ(rb->rt, working_vector, P_PADDING, erasures, 0);

  /* If unsuccessful, try again using erasures. */

  if(err < 0 || err > 2)
  {  int i;

     erasure_count = 0;

     for(i=0; i<P_VECTOR_SIZE; i++)
       if(!(state[i]&2))
	 erasures[erasure_count++] = i;

     if(erasure_count > 0 && erasure_count <= 2)
     {  memcpy(working_vector, vector, P_VECTOR_SIZE);
        err = DecodePQ(rb->rt, working_vector, P_PADDING, erasures, erasure_count);
     }
  }

  if(err == 1 || err == 2)
    memcpy(vector, working_vector, P_VECTOR_SIZE);

  return err < 0 ? 3 : err;
}

static int q_decode(RawBuffer *rb, unsigned char *vector, unsigned char *state)
{ int erasures[Q_VECTOR_SIZE];
  unsigned char working_vector[Q_VECTOR_SIZE];
  int err, erasure_count;

  /* Try error correction without erasure information */

  memcpy(working_vector, vector, Q_VECTOR_SIZE);
  err = DecodePQ(rb->rt, working_vector, Q_PADDING, erasures, 0);

  /* If unsuccessful, try again using erasures. */

  if(err < 0 || err > 2)
  {  int i;

     erasure_count = 0;

     for(i=0; i<Q_VECTOR_SIZE-2; i++)
       if(!(state[i]&2))
	 erasures[erasure_count++] = i;

     if(erasure_count > 0 && erasure_count <= 2)
     {  memcpy(working_vector, vector, Q_VECTOR_SIZE);
        err = DecodePQ(rb->rt, working_vector, Q_PADDING, erasures, erasure_count);
     }
  }

  if(err == 1 || err == 2)
    memcpy(vector, working_vector, Q_VECTOR_SIZE);

  return err < 0 ? 3 : err;
}

/*
 * Try to correct remaining bytes in rb->recovered.
 * Iterates over P and Q vectors until no further improvements are made.
 */

//#define DEBUG_ITERATIVE

int iterative_lec(RawBuffer *rb)
{  unsigned char p_vector[P_VECTOR_SIZE];
   unsigned char q_vector[Q_VECTOR_SIZE];
   int p_failures, q_failures;
   int p_corrected, q_corrected;
   int p,q;
   int last_p_failures = N_P_VECTORS;
   int last_q_failures = N_Q_VECTORS;
   int iteration=1;

   for(; ;) /* iterate over P- and Q-Parity until failures converge */
   {	
      p_failures = q_failures = 0;
      p_corrected = q_corrected = 0;

      /* Perform Q-Parity error correction */

      for(q=0; q<N_Q_VECTORS; q++)
      {  int err;

	 /* Try error correction */

	 GetQVector(rb->recovered, q_vector, q);
	 err = q_decode(rb, q_vector, rb->byteState);

	 /* See what we've got */

	 if(err > 2)  /* Uncorrectable. Mark bytes are erasure. */
	 {  q_failures++;
	    AndQVector(rb->byteState, ~1, q);
	 }
	 else  /* Correctable. Mark bytes as good; store back results. */ 
	 {  if(err == 1 || err == 2) /* Store back corrected vector */ 
	    {  SetQVector(rb->recovered, q_vector, q);
	       q_corrected++;
	    }
	    OrQVector(rb->byteState, 1, q);
	 }
      }

      /* Perform P-Parity error correction */

      for(p=0; p<N_P_VECTORS; p++)
      {  int err;

	 /* Try error correction */

	 GetPVector(rb->recovered, p_vector, p);
	 err = p_decode(rb, p_vector, rb->byteState);

	 /* See what we've got */

	 if(err > 2)  /* Uncorrectable. */
	 {  p_failures++;
	    AndPVector(rb->byteState, ~2, p);
	 }
	 else  /* Correctable. Mark bytes as good; store back results. */ 
	 {  if(err == 1 || err == 2) /* Store back corrected vector */ 
	    {  SetPVector(rb->recovered, p_vector, p);
	       p_corrected++;
	    }
	    OrPVector(rb->byteState, 2, p);
	 }
      }

      /* See if there was an improvement */

#ifdef DEBUG_ITERATIVE
      printf("L-EC: iteration %d\n", iteration); 
      printf("      Q-failures/corrected: %2d/%2d\n", q_failures, q_corrected);
      printf("      P-failures/corrected: %2d/%2d\n", p_failures, p_corrected);
#endif

      if(check_edc(rb->recovered, rb->xaMode) || p_failures + q_failures == 0)
	break;

      if(   last_p_failures > p_failures
	 || last_q_failures > q_failures)
      {  last_p_failures = p_failures;
	 last_q_failures = q_failures;
	 iteration++;
      }
      else break;
   }

   return (p_failures + q_failures == 0);
}

/***
 *** Heuristic L-EC attempt
 *** Andrei Grecu, 2006
 */

static int eval_q_candidate(RawBuffer *rb, unsigned char *q_vector, int q, 
			    int *p_failures_out, int *p_errors_out)
{
   unsigned char     p_vector[P_VECTOR_SIZE];
   unsigned char old_q_vector[Q_VECTOR_SIZE];
   int erasures[Q_VECTOR_SIZE];
   int p, p_errors = 0;
   int p_failures = 0;
   int err;
   
   GetQVector(rb->recovered, old_q_vector, q);
   SetQVector(rb->recovered,     q_vector, q);
   
   /* Count P failures after setting our Q vector. */

   for(p = 0; p < N_P_VECTORS; p++)
   {
      GetPVector(rb->recovered, p_vector, p);
      err = DecodePQ(rb->rt, p_vector, P_PADDING, erasures, 0);
      if(err <  0) p_failures++;
      else if(err == 1) p_errors++;
   }            

   SetQVector(rb->recovered, old_q_vector, q);

   *p_failures_out = p_failures;
   *p_errors_out   = p_errors;

   return TRUE;
}

static void eval_p_candidate(RawBuffer *rb, unsigned char *p_vector, int p, 
			    int *q_failures_out, int *q_errors_out)
{
   unsigned char     q_vector[Q_VECTOR_SIZE];
   unsigned char old_p_vector[P_VECTOR_SIZE];
   int erasures[P_VECTOR_SIZE];
   int q, q_errors = 0;
   int q_failures = 0;
   int err;
   
   GetPVector(rb->recovered, old_p_vector, p);
   SetPVector(rb->recovered,     p_vector, p);
   
   /* Count Q failures after setting our P vector. */

   for(q = 0; q < N_Q_VECTORS; q++)
   {
      GetQVector(rb->recovered, q_vector, q);
      err = DecodePQ(rb->rt, q_vector, Q_PADDING, erasures, 0);
      if(err <  0) q_failures++;
      else if(err == 1) q_errors++;
   }            

   SetPVector(rb->recovered, old_p_vector, p);

   *q_failures_out = q_failures;
   *q_errors_out = q_errors;
}


/*
 * An enhanced L-EC loop
 */   

//#define DEBUG_HEURISTIC_LEC

static int heuristic_lec(unsigned char *cd_frame, RawBuffer *rb, unsigned char *out)
{
   unsigned char p_vector[P_VECTOR_SIZE];
   unsigned char q_vector[Q_VECTOR_SIZE];
   unsigned char p_state[P_VECTOR_SIZE];
   unsigned char q_state[Q_VECTOR_SIZE];
   int erasures[Q_VECTOR_SIZE], decimated_erasures[2], erasure_count;
   int p_failures, q_failures;
   int p_corrected, q_corrected;
   int i,p,q;
   int iteration=1;
   int p_err, q_err;
   int p_decimated, q_decimated;
   int last_p_err = N_P_VECTORS;
   int last_q_err = N_Q_VECTORS;
   int last_p_failures = N_P_VECTORS;
   int last_q_failures = N_Q_VECTORS;
   int max_p_failures = 0;
   int max_p_errors = 0;
   int max_q_failures = 0;
   int max_q_errors = 0;
   int err;

   memset(rb->byteState, FRAME_BYTE_UNKNOWN, rb->sampleLength);
   
   /* Count initial P failures */

   for(p = 0; p < N_P_VECTORS; p++)
   {
      GetPVector(rb->recovered, p_vector, p);
      err = DecodePQ(rb->rt, p_vector, P_PADDING, erasures, 0);
      if(err < 0) max_p_failures++;
      if(err == 1) max_p_errors++;
   }

   /* Count initial Q failures */

   for(q = 0; q < N_Q_VECTORS; q++)
   {
      GetQVector(rb->recovered, q_vector, q);
      err = DecodePQ(rb->rt, q_vector, Q_PADDING, erasures, 0);
      if(err < 0) max_q_failures++;
      if(err == 1) max_q_errors++;
   }   
   
#ifdef DEBUG_HEURISTIC_LEC
   Verbose("      P-failures/corrected/errors + decimated: %2d/%2d/ 0 + 0\n", max_p_failures, max_p_errors);
   Verbose("      Q-failures/corrected/errors + decimated: %2d/%2d/ 0 + 0\n", max_q_failures, max_q_errors);
#endif

   for(; ;) /* iterate over P- and Q-Parity until failures converge */
   {   
      p_failures  = q_failures = 0;
      p_corrected = q_corrected = 0;
      p_err = q_err = 0;
      p_decimated = q_decimated = 0;

      /* Perform P-Parity error correction */

      for(p=0; p<N_P_VECTORS; p++)
      {  
         /* Determine number of erasures */

         GetPVector(rb->byteState, p_state, p);
         erasure_count = 0;

         for(i=0; i<P_VECTOR_SIZE; i++)
            if(p_state[i] == FRAME_BYTE_ERROR) erasures[erasure_count++] = i;

         /* First try to see whether P is correctable without erasure markings. */

         GetPVector(rb->recovered, p_vector, p);
         err = DecodePQ(rb->rt, p_vector, P_PADDING, erasures, 0);
         
         if(err == 1) /* Store back corrected vector */ 
         {  
            SetPVector(rb->recovered, p_vector, p);
            FillPVector(rb->byteState, FRAME_BYTE_GOOD, p);
            p_corrected++;
            p_err += err;
         }
            
         /* Erasure markings are overly pessimistic as each failing vector will mark
	    a full row as an erasure. We assume that there are only 2 real erasures
	    and try all combinations.
	    The case for 2 erasure is also included here. */

         if(err < 0 && erasure_count > 1) 
	 {  unsigned char best_p[P_VECTOR_SIZE];
	    int best_q_failures = N_Q_VECTORS;
	    int best_q_errors = N_Q_VECTORS;
 	    int candidate = FALSE;
	    int a, b;

            /* Try error correction with decimated erasures */
            
            for(a = 0; a < erasure_count - 1; a++)
            {
               for(b = a + 1; b < erasure_count; b++)
               {
                  decimated_erasures[0] = erasures[a];
                  decimated_erasures[1] = erasures[b];
                  GetPVector(rb->recovered, p_vector, p);
                  err = DecodePQ(rb->rt, p_vector, P_PADDING, decimated_erasures, 2);
                  if(err == 2) 
		  {  int q_err, q_fail;

		     candidate = TRUE;
		     eval_p_candidate(rb, p_vector, p, &q_fail, &q_err);

		     if(q_fail <= best_q_failures && q_err <= best_q_errors)
		     {  best_q_failures = q_fail;
		        best_q_errors   = q_err;
			memcpy(best_p, p_vector, P_VECTOR_SIZE); 
		     }
		  }
               }
            }

            if(!candidate) err = -1; /* If P failed */
            else
            {  FillPVector(rb->byteState, FRAME_BYTE_GOOD, p);
               SetPVector(rb->recovered, best_p, p);
               
               err = 0;
               p_err += 2;
               p_corrected++;
               p_decimated++;
            }
         }

         if(err == 0) FillPVector(rb->byteState, FRAME_BYTE_GOOD, p);
         if(err < 0)  /* Uncorrectable. */
         {  
            p_failures++;
            FillPVector(rb->byteState, FRAME_BYTE_ERROR, p);
         }
      }

      if(check_edc(rb->recovered, rb->xaMode)) break;

      /* Perform Q-Parity error correction */

      for(q=0; q<N_Q_VECTORS; q++)
      {  
         /* Determine number of erasures */

         GetQVector(rb->byteState, q_state, q);
         erasure_count = 0;

         for(i=0; i<Q_VECTOR_SIZE-2; i++)
            if(q_state[i] == FRAME_BYTE_ERROR) erasures[erasure_count++] = i;

         /* First try to see whether Q is correctable without erasure markings. */

         GetQVector(rb->recovered, q_vector, q);
         err = DecodePQ(rb->rt, q_vector, Q_PADDING, erasures, 0);

         if(err == 1) /* Store back corrected vector */ 
         {  
            SetQVector(rb->recovered, q_vector, q);
            FillQVector(rb->byteState, FRAME_BYTE_GOOD, q);
            q_corrected++;
            q_err++;
         }
         
         /* If there are more than 2 erasures we have have to decimate them to 2. */
         /* The case for 2 erasure is also included here. */

         if(err < 0 && erasure_count > 1) 
	 {  unsigned char best_q[Q_VECTOR_SIZE];
	    int best_p_failures = N_P_VECTORS;
	    int best_p_errors = N_P_VECTORS;
	    int candidate = FALSE;
	    int a, b;      

            /* Try error correction with decimated erasures */
            
            for(a = 0; a < erasure_count - 1; a++)
            {
               for(b = a + 1; b < erasure_count; b++)
               {
                  decimated_erasures[0] = erasures[a];
                  decimated_erasures[1] = erasures[b];
                  GetQVector(rb->recovered, q_vector, q);
                  err = DecodePQ(rb->rt, q_vector, Q_PADDING, decimated_erasures, 2);
                  if(err == 2) 
		  {  int p_err, p_fail;

		     candidate = TRUE;
                     eval_q_candidate(rb, q_vector, q, &p_fail, &p_err);

		     if(p_fail <= best_p_failures && p_err <= best_p_errors)
		     {  best_p_failures = p_fail;
		        best_p_errors   = p_err;
			memcpy(best_q, q_vector, Q_VECTOR_SIZE); 
		     }
                  }
               }
            }

            if(!candidate) err = -1; /* If Q failed */
            else
            {  FillQVector(rb->byteState, FRAME_BYTE_GOOD, q);
               SetQVector(rb->recovered, best_q, q);
               
               err = 0;
               q_err += 2;
               q_corrected++;
               q_decimated++;
            }
         }
         
         if(err == 0) FillQVector(rb->byteState, FRAME_BYTE_GOOD, q);
         if(err < 0)  /* Uncorrectable. Mark bytes are erasure. */
         {  
            q_failures++;
            FillQVector(rb->byteState, FRAME_BYTE_ERROR, q);
         }
      }

      /* See if there was an improvement */

#ifdef DEBUG_HEURISTIC_LEC
      Verbose("L-EC: iteration %d\n", iteration); 
      Verbose("      P-failures/corrected/errors + decimated: %2d/%2d/%2d + %2d\n", p_failures, p_corrected, p_err, p_decimated);
      Verbose("      Q-failures/corrected/errors + decimated: %2d/%2d/%2d + %2d\n", q_failures, q_corrected, q_err, q_decimated);
#endif
      
      if(p_failures + p_err + q_failures + q_err == 0) break;
      if(last_p_err <= p_err && last_q_err <= q_err && last_p_failures <= p_failures && last_q_failures <= q_failures) break;
		
      if(iteration > N_P_VECTORS + N_Q_VECTORS) break;
      
      if(check_edc(rb->recovered, rb->xaMode)) break;

      last_p_err = p_err;
      last_q_err = q_err;
      last_p_failures = p_failures;
      last_q_failures = q_failures;
      iteration++;
   }
   
   return TRUE;
}

/***
 *** Heuristic search for best sector
 *** Andrei Grecu, 2006
 */

//#define DEBUG_SEARCHPLAUSIBLESECTOR

static int check_q_plausibility(RawBuffer *rb, unsigned char *target_q_vector, 
				int q, int pos_a, int pos_b)
{  unsigned char q_vector[Q_VECTOR_SIZE];
   int plausible = FALSE;
   int q_index;
   int i;
   
   /* If no position was given, then find it out. */

   if(pos_a == -1)
   {
     GetQVector(rb->recovered, q_vector, q);
     for(i = 0; i < Q_VECTOR_SIZE; i++)
       if(target_q_vector[i] != q_vector[i]) 
       {
	 pos_a = i;
	 break;
       }

     /* pos_a == -1 implies that only 1 byte was corrected
        -> don't care about pos_b */

     pos_b = -1;  
   }
	
   /* Check plausibility of pos_a. */

   q_index = QToByteIndex(q, pos_a);

   for(i = 0; i < rb->samplesRead; i++)
     if(rb->rawBuf[i][q_index] == target_q_vector[pos_a]) 
     { 
       plausible = TRUE; 
       break; 
     }
	
   if(!plausible) return FALSE;

   /* pos_b only != -1 when Q corrected in erasure mode */

   plausible = FALSE;
	
   if(pos_b != -1)
   {
     /* Check plausibility of pos_b. */

     q_index = QToByteIndex(q, pos_b);

     for(i = 0; i < rb->samplesRead; i++)
       if(rb->rawBuf[i][q_index] == target_q_vector[pos_b]) 
       { 
	 plausible = TRUE; 
	 break; 
       }
     
     if(!plausible) return FALSE;
   }

   return TRUE;
}

static int check_p_plausibility(RawBuffer *rb, unsigned char *target_p_vector, 
				int p, int pos_a, int pos_b)
{  unsigned char p_vector[P_VECTOR_SIZE];
   int plausible = FALSE;
   int p_index;
   int i;
   
   /* If no position was given, then find it out. */

   if(pos_a == -1)
   {
     GetPVector(rb->recovered, p_vector, p);
     for(i = 0; i < P_VECTOR_SIZE; i++)
     {
       if(target_p_vector[i] != p_vector[i]) 
       {
	 pos_a = i;
	 break;
       }
     }

     /* pos_a == -1 implies that only 1 byte was corrected
        -> don't care about pos_b */

     pos_b = -1;
   }
	
   /* Check plausibility of pos_a. */

   p_index = PToByteIndex(p, pos_a);

   for(i = 0; i < rb->samplesRead; i++)
     if(rb->rawBuf[i][p_index] == target_p_vector[pos_a]) 
     { 
       plausible = TRUE; 
       break; 
     }

   if(!plausible) return FALSE;
   
   /* pos_b only != -1 when P corrected in erasure mode */

   plausible = FALSE;

   if(pos_b != -1)
   {
     /* Check plausibility of pos_b. */

     p_index = PToByteIndex(p, pos_b);

     for(i = 0; i < rb->samplesRead; i++)
       if(rb->rawBuf[i][p_index] == target_p_vector[pos_b]) 
       { 
	 plausible = TRUE; 
	 break; 
       }
		
     if(!plausible) return FALSE;
   }
   
   return TRUE;
}

static int find_better_p(RawBuffer *rb, int p, int refError)
{  unsigned char p_vector[P_VECTOR_SIZE];
   int np1, np2;
   int dummy[2];
   int err;
  
   /* Try all possible Ps and see whether we get a better load. */
 
   for(np1 = 0; np1 < rb->pParityN[p][0]; np1++)
   {
     for(np2 = 0; np2 < rb->pParityN[p][1]; np2++)
     {
       GetPVector(rb->recovered, p_vector, p);
       p_vector[24] = rb->pParity1[p][np1];
       p_vector[25] = rb->pParity2[p][np2];
	  		
       err = DecodePQ(rb->rt, p_vector, P_PADDING, dummy, 0);
       if(err < refError && err >= 0)
       {
	 SetPVector(rb->recovered, p_vector, p);			
	 return TRUE;
       }
     }
   }
   
   return FALSE;
}

static int find_better_q(RawBuffer *rb, int q, int refError)
{  unsigned char q_vector[Q_VECTOR_SIZE];
   int nq1, nq2;
   int dummy[2];
   int err;
  
   /* Try all possible Qs and see whether we get a better load. */

   for(nq1 = 0; nq1 < rb->qParityN[q][0]; nq1++)
   {
     for(nq2 = 0; nq2 < rb->qParityN[q][1]; nq2++)
     {
       GetQVector(rb->recovered, q_vector, q);
       q_vector[43] = rb->qParity1[q][nq1];
       q_vector[44] = rb->qParity2[q][nq2];
	  		
       err = DecodePQ(rb->rt, q_vector, Q_PADDING, dummy, 0);
       if(err < refError && err >= 0)
       {
	 SetQVector(rb->recovered, q_vector, q);			
	 return TRUE;
       }
     }
   }
   
   return FALSE;
}

static void initialize_frame(RawBuffer *rb)
{  int best_sector = 0;
   int max_load = 2 * (N_P_VECTORS + N_Q_VECTORS);
   int i;
	
   /* Initialize sector header. */

   initialize_cd_frame(rb->recovered, rb->lba);

   /* Search for block with least P and Q load and load into the frame. */

   for(i = 0; i < rb->samplesRead; i++) 
   {
     if(rb->qLoad[i] + rb->pLoad[i] < max_load)
     {
       max_load = rb->pLoad[i] + rb->qLoad[i];
       best_sector = i;
     }
   }
   
   memcpy(&(rb->recovered[16]), &(rb->rawBuf[best_sector][16]), rb->sampleLength - 16);
}

//#define DEBUG_SEARCH_PLAUSIBLE

int search_plausible_sector(RawBuffer *rb)
{
   unsigned char p_vector[26];
   unsigned char q_vector[45];
   int decimated_erasures[2];
   int p_failures, q_failures;
   int p_corrected, q_corrected;
   int p,q;
   int iteration=1;
   int p_err, q_err;
   int p_decimated, q_decimated;
   int last_p_err = N_P_VECTORS;
   int last_q_err = N_Q_VECTORS;
   int last_p_failures = N_P_VECTORS;
   int last_q_failures = N_Q_VECTORS;
   int err;
   
   /* Initialize sector */     

   initialize_frame(rb);
	
   for(; ;) /* iterate over P- and Q-Parity until failures converge */
   {   
      p_failures = q_failures = 0;
      p_corrected = q_corrected = 0;
      p_err = q_err = 0;
      p_decimated = q_decimated = 0;

      /* Perform Q-Parity error correction */

      for(q=0; q<N_Q_VECTORS; q++)
      {  
      	/* Check whether Q is correct. */
      	GetQVector(rb->recovered, q_vector, q);
	err = DecodePQ(rb->rt, q_vector, Q_PADDING, decimated_erasures, 0);
         
	/* If it is not correct. */
	if(err == 1 || err < 0)
        {	        
	  /* If Q is correctable. */
	  if(err == 1)
	  {
	    /* Check correction for plausibility: if corrected byte was read in one of the read attempts. */
	    if(check_q_plausibility(rb, q_vector, q, -1, -1))
	    {
	      /* Store back corrected vector */ 
	      SetQVector(rb->recovered, q_vector, q);
	      q_corrected++;
	      q_err++;
	    }
	    else 
	    {
	      /* See whether we can find some Q parity bytes accepting the original Q vector. */
	      if(find_better_q(rb, q, 1)) 
	      { 
		q_corrected++; 
		q_err++; 
	      }
	      else err = -1;
	    }
	  }
	         
	  /* If correction is not plausible or possible then try 2 error-decimation. */
	  if(err < 0)
	  {
	    /* Try error correction with decimated erasures.
	       Note that no erasure information for the parity bytes is available */
	    int a, b; 
	    int solFound = FALSE;     
	    for(a = 0; a < Q_VECTOR_SIZE - 2; a++)
	    {
	      for(b = a + 1; b < Q_VECTOR_SIZE - 2; b++)
	      {
		decimated_erasures[0] = a;
		decimated_erasures[1] = b;
		GetQVector(rb->recovered, q_vector, q);
		err = DecodePQ(rb->rt, q_vector, Q_PADDING, decimated_erasures, 2);
		if(err == 2) 
		{ 
		  if(check_q_plausibility(rb, q_vector, q, a, b)) { solFound = TRUE; break;	}
		}
	      }
	      if(solFound) break;
	    }
	    if(solFound)
	    {
	      /* Store back corrected vector */ 
	      SetQVector(rb->recovered, q_vector, q);
	      q_err += 2;
	      q_corrected++;
	      q_decimated++;
	    }
	    else 
	    {
	      /* See whether we can find some Q parity bytes accepting the original Q vector. */
	      if(find_better_q(rb, q, 2)) 
	      { 
		q_corrected++; 
		q_err++; 
	      }
	      else q_failures++; /* If Q failed */
	    }
	  }
	}
      }

      /* Perform P-Parity error correction */

      for(p=0; p<N_P_VECTORS; p++)
      {  
      	/* Check whether P is correct. */
      	GetPVector(rb->recovered, p_vector, p);
	err = DecodePQ(rb->rt, p_vector, P_PADDING, decimated_erasures, 0);
         
	/* If it is not correct. */
	if(err == 1 || err < 0)
        {	        
	  /* If Q is correctable. */
	  if(err == 1)
	  {
	    /* Check correction for plausibility: if corrected byte was read in one of the read attempts. */
	    if(check_p_plausibility(rb, p_vector, p, -1, -1))
	    {
	      /* Store back corrected vector */ 
	      SetPVector(rb->recovered, p_vector, p);
	      p_corrected++;
	      p_err++;
	    }
	    else 
	    {
	      /* See whether we can find some P parity bytes accepting the original P vector. */
	      if(find_better_p(rb, p, 1)) 
	      { 
		p_corrected++; 
		p_err++; 
	      }
	      else err = -1;
	    }
	  }
	  
	  /* If correction is not plausible or possible then try 2 error-decimation. */
	  if(err < 0)
	  {
	    /* Try error correction with decimated erasures */
	    int a, b; 
	    int solFound = FALSE;     
	    for(a = 0; a < P_VECTOR_SIZE; a++)
	    {
	      for(b = a + 1; b < P_VECTOR_SIZE; b++)
	      {
		decimated_erasures[0] = a;
		decimated_erasures[1] = b;
		GetPVector(rb->recovered, p_vector, p);
		err = DecodePQ(rb->rt, p_vector, P_PADDING, decimated_erasures, 2);
		if(err == 2) 
		{ 
		  if(check_p_plausibility(rb, p_vector, p, a, b)) { solFound = TRUE; break; }
		}
	      }
	      if(solFound) break;
	    }
	    if(solFound)
	    {
	      /* Store back corrected vector */ 
	      SetPVector(rb->recovered, p_vector, p);
	      p_err += 2;
	      p_corrected++;
	      p_decimated++;
	    }
	    else 
	    {
	      /* See whether we can find some P parity bytes accepting the original P vector. */
	      if(find_better_p(rb, p, 2)) 
	      { 
		p_corrected++; 
		p_err++; 
	      }
	      else p_failures++; /* If P failed */
	    }
	  }
	}
      }

      /* See if there was an improvement */

#ifdef DEBUG_SEARCH_PLAUSIBLE
      Verbose("SPS L-EC: iteration %d\n", iteration); 
      Verbose("      Q-f/c/e + d: %2d/%2d/%2d + %2d\n", q_failures, q_corrected, q_err, q_decimated);
      Verbose("      P-f/c/e + d: %2d/%2d/%2d + %2d\n", p_failures, p_corrected, p_err, p_decimated);
#endif
      
      if(p_failures + p_err + q_failures + q_err == 0) break;
      if(last_p_err <= p_err && last_q_err <= q_err && last_p_failures <= p_failures && last_q_failures <= q_failures) break;
      
      if(iteration > N_P_VECTORS + N_Q_VECTORS) break;
      if(p_failures == 0)
      {
	if(check_edc(rb->recovered, rb->xaMode)) break;
      }
      
      last_p_err = p_err;
      last_q_err = q_err;
      last_p_failures = p_failures;
      last_q_failures = q_failures;
      iteration++;
   }	

   return TRUE;
}

/***
 *** Auxiliary functions for collecting some stuff incrementally
 ***/

/*
 * Determine work load of P/Q vectors in the given frame
 */

static void calculate_pq_load(RawBuffer *rb)
{  unsigned char q_vector[N_Q_VECTORS];
   unsigned char p_vector[N_P_VECTORS];
   int frame_idx = rb->samplesRead - 1;
   unsigned char *new_frame = rb->rawBuf[frame_idx];
   int ignore[2];
   int q, p;
   int err;
	
   for(q = 0; q < N_Q_VECTORS; q++)
   {
     GetQVector(new_frame, q_vector, q);
     err = DecodePQ(rb->rt, q_vector, Q_PADDING, ignore, 0);
     if(err <  0) rb->qLoad[frame_idx] += 2;
     if(err == 1) rb->qLoad[frame_idx]++; /* We assume without any erasures specified there can't be more than 1 errors corrected. */
   }      

   for(p = 0; p < N_P_VECTORS; p++)
   {
     GetPVector(new_frame, p_vector, p);
     err = DecodePQ(rb->rt, p_vector, P_PADDING, ignore, 0);
     if(err <  0) rb->pLoad[frame_idx] += 2;
     if(err == 1) rb->pLoad[frame_idx]++; /* We assume without any erasures specified there can't be more than 1 errors corrected. */
   }      
}   

/*
 * Collect a list of all seen P/Q parity bytes.
 */

static void update_pq_parity_list(RawBuffer *rb, unsigned char *new_frame)
{  unsigned char p_vector[P_VECTOR_SIZE];
   unsigned char q_vector[Q_VECTOR_SIZE];
   int i,p,q;

   /*** See if new frame has any Q parity bytes different from the existing ones. */

   for(q=0; q<N_Q_VECTORS; q++)
   {  int qfound = FALSE;

      GetQVector(new_frame, q_vector, q);

      for(i=0; i<rb->qParityN[q][0]; i++)
	if(rb->qParity1[q][i] == q_vector[43])
	{  qfound = TRUE;
	   break;
	}

      if(!qfound)
	rb->qParity1[q][rb->qParityN[q][0]++] = q_vector[43];

      qfound = FALSE;

      for(i=0; i<rb->qParityN[q][1]; i++)
	if(rb->qParity2[q][i] == q_vector[44])
	{  qfound = TRUE;
	   break;
	}

      if(!qfound)
	rb->qParity2[q][rb->qParityN[q][1]++] = q_vector[44];
   }

   /*** See if new frame has any P parity bytes different from the existing ones. */

   for(p=0; p<N_P_VECTORS; p++)
   {  int pfound = FALSE;

      GetPVector(new_frame, p_vector, p);

      for(i=0; i<rb->pParityN[p][0]; i++)
	if(rb->pParity1[p][i] == p_vector[24])
	{  pfound = TRUE;
	   break;
	}

      if(!pfound)
	rb->pParity1[p][rb->pParityN[p][0]++] = p_vector[24];

      pfound = FALSE;

      for(i=0; i<rb->pParityN[p][1]; i++)
	if(rb->pParity2[p][i] == p_vector[25])
	{  pfound = TRUE;
	   break;
	}

      if(!pfound)
	rb->pParity2[p][rb->pParityN[p][1]++] = p_vector[25];
   }
}

/*** 
 *** The grand wrapper:
 ***
 * Try several strategies to analyse and recover
 * a collection of RAW frame samples.
 */

int TryCDFrameRecovery(RawBuffer *rb, unsigned char *outbuf)
{  unsigned char *new_frame = rb->workBuf->buf;

   /*** Reject unplausible sectors */

   /* See if the buffer was returned unchanged. */

   if(!memcmp(new_frame, Closure->deadSector, 2048))
   {  RememberSense(3, 255, 0);  /* No data returned */
      return -1;
   }

   /* A fully zeroed out buffer is suspicious since at least the
      sync byte sequence and address fields should not be zero.  
      This is usually a sign that the atapi/scsi driver is broken; 
      e.g. that it does not pass through data when the drive 
      signalled an error. */

   if(!memcmp(new_frame, rb->zeroSector, 2352))
   {  RememberSense(3, 255, 5); /* Atapi/scsi drive possibly broken (zero sector) */
      return -1;
   }

   /* Some operating systems are even worse - random data is returned.
      If the sync sequence is missing, reject the sector. */

   if(!check_for_sync_pattern(new_frame))
   {  RememberSense(3,255, 8); /* Atapi/scsi drive possibly broken (random data) */
      return -1;
   }

   /* Compare lba against MSF field. Some drives return sectors
      from wrong places in RAW mode. */

   if(!check_msf(new_frame, rb->lba))
   {  RememberSense(3, 255, 2); /* Wrong MSF in raw sector */
      return -1;
   }

   /* Okay, accept sector as a valid sample for recovery. */

   if(rb->xaMode)
     memset(new_frame+12, 0, 4); 

   memcpy(rb->rawBuf[rb->samplesRead], new_frame, rb->sampleLength);
   rb->samplesRead++;

#ifdef DUMP_MODE
     dump_sector(rb);
#endif

   /*** Cheap shot: See if we can recover the sector itself
	(without using the more complex heuristics and other
	sectors).
        Note that we ignore the return value of iterative_recovery().
        If e.g. some parity bytes remain uncorrected we don't care
        as long as the EDC tells us that the user data part is okay. */

   memcpy(rb->recovered, new_frame, rb->sampleLength);
   memset(rb->byteState, 0, rb->sampleLength);

   iterative_lec(rb);

   if(check_edc(rb->recovered, rb->xaMode))
   {
       PrintCLIorLabel(Closure->status, 
		       "Sector %d: Recovered in raw reader by iterative L-EC.\n",
		       rb->lba);

       memcpy(outbuf, rb->recovered+rb->dataOffset, 2048);
       return 0;
   }

   /*** More sophisticated heuristics */

   /* Incremental update of our data */

   calculate_pq_load(rb);
   update_pq_parity_list(rb, new_frame);

   /* The actual heuristics */

   search_plausible_sector(rb);

   if(check_edc(rb->recovered, rb->xaMode))
   {  PrintCLIorLabel(Closure->status, 
		      "Sector %d: Recovered in raw reader by plausible sector search.\n",
		      rb->lba);
      memcpy(outbuf, rb->recovered+rb->dataOffset, 2048);
      return 0; 
   }

   heuristic_lec(rb->recovered, rb, outbuf);

   if(check_edc(rb->recovered, rb->xaMode))
   {  PrintCLIorLabel(Closure->status, 
		      "Sector %d: Recovered in raw reader by heuristic L-EC.\n",
		      rb->lba);
      memcpy(outbuf, rb->recovered+rb->dataOffset, 2048);
      return 0; 
   }

   /*** Recovery failed */

   RememberSense(3, 255, 6);  /* Sector accumulated for analysis */
   rb->recommendedAttempts = Closure->maxReadAttempts;
   return -1;
}
