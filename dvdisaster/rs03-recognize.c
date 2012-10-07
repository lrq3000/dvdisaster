/*  dvdisaster: Additional error correction for optical media.
 *  Copyright (C) 2004-2012 Carsten Gnoerlich.
 *
 *  Email: carsten@dvdisaster.org  -or-  cgnoerlich@fsfe.org
 *  Project homepage: http://www.dvdisaster.org
 *
 *  This file is part of dvdisaster.
 *
 *  dvdisaster is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  dvdisaster is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with dvdisaster. If not, see <http://www.gnu.org/licenses/>.
 */

#include "dvdisaster.h"
#include "udf.h"

#include "rs03-includes.h"

/***
 *** Recognize a RS03 error correction file
 ***/

int RS03RecognizeFile(LargeFile *ecc_file, EccHeader **eh)
{  int n;

   *eh = g_malloc(sizeof(EccHeader));

   LargeSeek(ecc_file, 0);
   n = LargeRead(ecc_file, *eh, sizeof(EccHeader));

   if(n != sizeof(EccHeader))
   {  g_free(*eh);
      return ECCFILE_INVALID;
   }

   if(strncmp((char*)(*eh)->cookie, "*dvdisaster*", 12))
     return ECCFILE_DEFECTIVE_HEADER;

   if(!strncmp((char*)(*eh)->method, "RS03", 4))
   {
#ifdef HAVE_BIG_ENDIAN
      SwapEccHeaderBytes(*eh);
#endif
      return ECCFILE_PRESENT;
   }

   g_free(*eh);
   return ECCFILE_WRONG_CODEC;
}

/***
 *** Recognize RS03 error correction data in the image
 ***/

#if 0
static int read_fingerprint(LargeFile *file, unsigned char *fingerprint, gint64 sector)
{  struct MD5Context md5ctxt;
   unsigned char buf[2048];
   int n;

   if(!LargeSeek(file, 2048LL*sector))
     return FALSE;

   n = LargeRead(file, buf, 2048);

   if(n != 2048) return FALSE;

   if(CheckForMissingSector(buf, sector, NULL, 0) != SECTOR_PRESENT)
     return FALSE;

   MD5Init(&md5ctxt);
   MD5Update(&md5ctxt, buf, 2048);
   MD5Final(fingerprint, &md5ctxt);

   return TRUE;
}
#endif

EccHeader* ValidHeader(unsigned char *buf, gint64 hdr_pos)
{  EccHeader *eh = (EccHeader*)buf;
   guint32 recorded_crc, real_crc;
   //   unsigned char fingerprint[16];

   /* Medium read error in ecc header? */

   if(   (CheckForMissingSector(buf, hdr_pos, NULL, 0) != SECTOR_PRESENT)
      || (CheckForMissingSector(buf+2048, hdr_pos+1, NULL, 0) != SECTOR_PRESENT))
     return NULL;

   /* See if the magic cookie is there */

   if(   strncmp((char*)eh->cookie, "*dvdisaster*", 12)
	 || strncmp((char*)eh->method, "RS03", 4))   // FIXME
     return NULL;

   /* Examine the checksum */

   recorded_crc = eh->selfCRC;

#ifdef HAVE_BIG_ENDIAN
   eh->selfCRC = 0x47504c00;
#else
   eh->selfCRC = 0x4c5047;
#endif
   real_crc = Crc32((unsigned char*)eh, 4096);

   if(real_crc != recorded_crc)
     return NULL;

   /* Check the fingerprint */

   eh = g_malloc(sizeof(EccHeader));
   memcpy(eh, buf, sizeof(EccHeader));
#ifdef HAVE_BIG_ENDIAN
   SwapEccHeaderBytes(eh);
#endif
   eh->selfCRC = recorded_crc;

#if 0
   status = read_fingerprint(file, fingerprint, eh->fpSector);

   if(!status)  /* be optimistic if fingerprint sector is unreadable */
     return eh;

   if(!memcmp(fingerprint, eh->mediumFP, 16))  /* good fingerprint */
     {  printf("RS03 header found\n");
      return eh;
     }
   g_free(eh);
#endif

   return eh;
}

EccHeader* FindRS03HeaderInImage(Image *image)
{  EccHeader *eh = NULL;
   gint64 hdr_pos;
   IsoInfo *ii; 
   unsigned char buf[4096];

   switch(image->type)
   { case IMAGE_FILE:
       Verbose("FindRS03HeaderInImage: file %s\n", image->file->path);
       break;

     case IMAGE_MEDIUM:
       Verbose("FindRS03HeaderInImage: medium %s\n", image->dh->device);
       break;

     default:
       Verbose("FindRS03HeaderInImage: unknown type %d\n", image->type);
       break;
   }

   /*** Try to find the header behind the ISO image */

   ii = image->isoInfo;
   if(!ii) Verbose(" . NO ISO structures found!\n");

   if(ii)
   {  hdr_pos = ii->volumeSize;

      if(ImageReadSectors(image, buf, hdr_pos, 2) == 2)
      {  eh = ValidHeader(buf, hdr_pos);
	 if(eh) 
	 { Verbose("FindRS03HeaderInImage(): Header found at pos +0\n"); 
	   return eh;
	 }
      }

      hdr_pos = ii->volumeSize - 150;
      if(ImageReadSectors(image, buf, hdr_pos, 2) == 2)
      {  eh = ValidHeader(buf, hdr_pos);
	 if(eh) 
	 { Verbose("FindRS03HeaderInImage(): Header found at pos -150\n"); 
	   return eh;
	 }
      }
   }
      
   return NULL;
}

typedef struct 
{  gint64 bidx[256];
   char *layer[256];
} recognize_context;

static void free_recognize_context(recognize_context *rc)
{  int i;

   for(i=0; i<255; i++)
      if(rc->layer[i])
	 g_free(rc->layer[i]);

   g_free(rc);
}
   
int RS03RecognizeImage(Image *image)
{  recognize_context *rc = g_malloc0(sizeof(recognize_context));
   LargeFile *ecc_file = image->file;
   guint64 file_size;
   guint64 layer_size;
   int ecc_block,ndata,nroots;
   int i;

   switch(image->type)
   { case IMAGE_FILE:
       Verbose("RS03Recognize: file %s\n", image->file->path);
       break;

     case IMAGE_MEDIUM:
       Verbose("RS03Recognize: medium %s\n", image->dh->device);
       break;

     default:
       Verbose("RS03Recognize: unknown type %d\n", image->type);
       break;
   }

   /* Easy shot: Locate the ecc header in the image */

   image->eccHeader = FindRS03HeaderInImage(image); 
  
   if(image->eccHeader) 
   {  free_recognize_context(rc);
      return TRUE;
   }

   /* No exhaustive search unless explicitly okayed by user */

   if(!Closure->examineRS03)
   {  free_recognize_context(rc);
      return FALSE;
   }

   /* Ugly case. Experimentally try the RS-Code. */

   Verbose("RS03RecognizeImage(): No EH\n");

   if(!LargeStat(Closure->imageName, &file_size))
   {  free_recognize_context(rc);
      return FALSE;
   }

   file_size /= 2048;

   if(Closure->debugMode && Closure->mediumSize)
      layer_size = Closure->mediumSize/GF_FIELDMAX;
   else
   {  if(file_size < CDR_SIZE)         layer_size = CDR_SIZE/GF_FIELDMAX;
      else if(file_size < DVD_SL_SIZE) layer_size = DVD_SL_SIZE/GF_FIELDMAX; 
      else if(file_size < DVD_DL_SIZE) layer_size = DVD_DL_SIZE/GF_FIELDMAX; 
      else if(file_size < BD_SL_SIZE)  layer_size = BD_SL_SIZE/GF_FIELDMAX; 
      else                             layer_size = BD_DL_SIZE/GF_FIELDMAX; 
   }

   Verbose(".. trying layer size %lld\n", layer_size);

   for(i=0; i<255; i++)
   {  rc->bidx[i] = i*layer_size;
      rc->layer[i] = malloc(2048);
   }

   /* Now try all ecc blocks */

   for(ecc_block=0; ecc_block<layer_size; ecc_block++)
   {  Verbose("Assembling ecc block %d\n", ecc_block); 

      /* Assemble the ecc block */

      for(i=0; i<255; i++)  
      {  gint64 sector = rc->bidx[i]++;
	 int n;

	 if(!LargeSeek(ecc_file, (gint64)(2048*sector)))
	    Stop(_("Failed seeking to sector %lld in image: %s"),
		 sector, strerror(errno));

	 n = LargeRead(ecc_file, rc->layer[i], 2048);
	 if(n != 2048)
	    Stop(_("Failed reading sector %lld in image: %s"),sector,strerror(errno));
      }	 

      /* Experimentally apply the RS code */

      for(ndata=255-8; ndata >=85; ndata--)
      {  CrcBlock *cb = (CrcBlock*)rc->layer[ndata];

	 /* Do the real decode here */


	 /* See if we have decoded a CRC block */

	 if(  !memcmp(cb->cookie, "*dvdisaster*", 12)
	    ||!memcmp(cb->method, "RS03", 4))
	 {  
	    nroots = 255-ndata-1;
	    Verbose(".. Success: rediscovered format with %d roots\n", nroots); 

	    image->eccHeader = g_malloc(sizeof(EccHeader));
	    ReconstructRS03Header(image->eccHeader, cb);
	    //FIXME: endianess okay?
	    free_recognize_context(rc);
	    return TRUE;
	 }
      }
   }

   free_recognize_context(rc);
   return FALSE;
}

