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

   rb->workBuf   = CreateAlignedBuffer(2352*16);
   rb->zeroSector= g_malloc0(2352);
   rb->rawBuf    = g_malloc(Closure->readAttempts * sizeof(unsigned char*));
   rb->recovered = g_malloc(sample_length);
   rb->byteState = g_malloc(sample_length);

   for(i=0; i<Closure->readAttempts; i++)
   {  rb->rawBuf[i] = g_malloc(sample_length);
   }

   for(i=0; i<N_P_VECTORS; i++)
     rb->pList[i] = g_malloc(Closure->readAttempts * sizeof(int));

   for(i=0; i<N_Q_VECTORS; i++)
     rb->qList[i] = g_malloc(Closure->readAttempts * sizeof(int));

   return rb;
}

void FreeRawBuffer(RawBuffer *rb)
{  int i;

   FreeGaloisTables(rb->gt);
   FreeReedSolomonTables(rb->rt);

   for(i=0; i<Closure->readAttempts; i++)
     g_free(rb->rawBuf[i]);

   for(i=0; i<N_P_VECTORS; i++)
     g_free(rb->pList[i]);

   for(i=0; i<N_Q_VECTORS; i++)
     g_free(rb->qList[i]);

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

/***
 *** CD level CRC calculation
 ***/

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
		     "Sector %d  L-EC P/Q results: %d/%d failures, %d/%d corrected\n",
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

  /* Do simple L-EC.
     It seems that drives stop their internal L-EC as soon as the
     EDC is okay, so we may see uncorrected errors in the parity bytes.
     Since we are also interested in the user data only and doing the
     L-EC is expensive, we skip our L-EC as well when the EDC is fine. */

  if(!CheckEDC(frame))
    lec_did_sth = simple_lec(rb, frame);

  /* Test internal sector checksum again */

  if(!CheckEDC(frame))
  {  RememberSense(16, 255, 1);
       return FALSE;
  }

  /* Test internal sector address */

  if(!check_msf(frame, rb->lba))
  {  RememberSense(16, 255, 2);
     return FALSE;
  }

  /* Tell user that L-EC succeeded */

  if(lec_did_sth)
    PrintCLIorLabel(Closure->status, 
		    "Sector %d: Recovered in raw reader by L-EC\n",
		    rb->lba);

   return TRUE;
}

/***
 *** Try to analyse and recover a series of raw CD frame samples.
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

int iterative_recovery(RawBuffer *rb)
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

      if(CheckEDC(rb->recovered) || p_failures + q_failures == 0)
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


int TryCDFrameRecovery(RawBuffer *rb, unsigned char *outbuf)
{  unsigned char *new_frame = rb->workBuf->buf;

  /* Collect sectors for debugging purposes */

   /*** Reject unplausible sectors */

   /* See if the buffer was returned unchanged. */

   if(!memcmp(new_frame, Closure->deadSector, 2048))
   {  RememberSense(16, 255, 0);
      return -1;
   }

   /* A fully zeroed out buffer is suspicious since at least the
      sync byte sequence and address fields should not be zero.  
      This is usually a sign that the atapi/scsi driver is broken; 
      e.g. that it does not pass through data when the drive 
      signalled an error. */

   if(!memcmp(new_frame, rb->zeroSector, 2352))
   {  RememberSense(16, 255, 5);
      return -1;
   }

   /* Compare lba against MSF field. Some drive return sectors
      from wrong places in RAW mode. */

   if(!check_msf(new_frame, rb->lba))
   {  RememberSense(16, 255, 2);
      return -1;
   }

   /* Okay, accept sector as a valid sample for recovery. */

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

   iterative_recovery(rb);

   if(   CheckEDC(rb->recovered)
      && check_msf(rb->recovered, rb->lba))
   {
       PrintCLIorLabel(Closure->status, 
		       "Sector %d: Recovered in raw reader by iterative L-EC\n",
		       rb->lba);

       memcpy(outbuf, rb->recovered+16, 2048);
       return 0;
   }

   RememberSense(16, 255, 6);
   return -1;
}
