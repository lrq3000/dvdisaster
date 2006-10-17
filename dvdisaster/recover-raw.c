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

   rb->rawBuf    = g_malloc(Closure->readAttempts * sizeof(unsigned char*));
   rb->rawState  = g_malloc(Closure->readAttempts * sizeof(int));
   rb->valid     = g_malloc(Closure->readAttempts * sizeof(int));
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
 *** Validate CD raw sector(s)
 ***/

int ValidateRawSectors(RawBuffer *rb, unsigned char *frames_in, int nframes)
{  unsigned char min,sec,frm;
   unsigned char *frame = frames_in;
   int lba,f;

   lba = rb->lba;

   for(f=0; f<nframes; f++)
   {

     /* Do simple L-EC */

     /* ... */

     /* Test internal sector checksum */

     if(!CheckEDC(frame))
     {  RememberSense(16, 255, 1);
        return FALSE;
     }

     /* Test internal sector address */

     lba_to_msf(lba, &min, &sec, &frm);
     min = int_to_bcd(min);
     sec = int_to_bcd(sec);
     frm = int_to_bcd(frm);

     if(   frame[12] != min
        || frame[13] != sec	
        || frame[14] != frm)
     {  RememberSense(16, 255, 2);
        return FALSE;
     }

     /* Goto next sector */

     frame += rb->sampleLength;
     lba++;
   }

   return TRUE;
}

