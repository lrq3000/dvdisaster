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

#include "rs02-includes.h"

/***
 *** Reset the compare output window
 ***/

void ResetRS02CompareWindow(Method *self)
{  
}

/***
 *** Create the notebook contents for the compare output
 ***/

void CreateRS02CompareWindow(Method *self, GtkWidget *parent)
{  
}

/***
 *** Check the consistency of the augmented image
 ***/

/* 
 * housekeeping
 */

typedef struct
{  LargeFile *file;
   RS02Layout *lay;
   guint32 *crcBuf;
   unsigned char crcSum[16];
} compare_closure;

static void cleanup(gpointer data)
{  compare_closure *cc = (compare_closure*)data;

   Closure->cleanupProc = NULL;

   if(Closure->guiMode)
      AllowActions(TRUE);

   if(cc->file) LargeClose(cc->file);
   if(cc->lay) g_free(cc->lay);
   if(cc->crcBuf) g_free(cc->crcBuf);
   
   g_free(cc);

   g_thread_exit(0);
}

/***
 *** Read the crc portion and descramble it from ecc block order
 *** into ascending sector order. 
 */

static void read_crc(compare_closure *cc, RS02Layout *lay)
{  struct MD5Context crc_md5;
   gint64 block_idx[256];
   guint32 crc_buf[512];
   gint64 s;
   int i,crc_idx;
   
   /* Allocate buffer for ascending sector order CRCs */

   cc->crcBuf = g_malloc(2048 * lay->crcSectors);
   MD5Init(&crc_md5);

   /* First sector containing crc data */

   if(!LargeSeek(cc->file, 2048*(lay->dataSectors+2)))
     Stop(_("Failed seeking to sector %lld in image: %s"), 
	  lay->dataSectors+2, strerror(errno));

   /* Initialize ecc block index pointers.
      The first CRC set (of lay->ndata checksums) relates to
      ecc block lay->firstCrcLayerIndex + 1. */

   for(s=0, i=0; i<lay->ndata; s+=lay->sectorsPerLayer, i++)
     block_idx[i] = s + lay->firstCrcLayerIndex + 1;

   crc_idx = 512;  /* force crc buffer reload */

   /* Cycle to the ecc blocks and sort CRC sums in
      ascending sector numbers. */

   for(s=0; s<lay->sectorsPerLayer; s++)
   {  gint64 si = (s + lay->firstCrcLayerIndex + 1) % lay->sectorsPerLayer;

      /* Wrap the block_idx[] ptrs at si == 0 */

      if(!si)
      {  gint64 bs;

         for(bs=0, i=0; i<lay->ndata; bs+=lay->sectorsPerLayer, i++)
	   block_idx[i] = bs;
      }

      /* Go through all data sectors of current ecc block */

      for(i=0; i<lay->ndata; i++)
      {
	 if(block_idx[i] < lay->dataSectors)  /* only data sectors have CRCs */
	 {  
	    /* Refill crc cache if needed */
	    
	    if(crc_idx >= 512)
	    {   if(LargeRead(cc->file, crc_buf, 2048) != 2048)
		  Stop(_("problem reading crc data: %s"), strerror(errno));
		
	        MD5Update(&crc_md5, (unsigned char*)crc_buf, 2048);
		crc_idx = 0;
	    }

	    /* Sort crc into appropriate place */

	    cc->crcBuf[block_idx[i]] = crc_buf[crc_idx];
	    crc_idx++;
	    block_idx[i]++;
	 }
      }
   }

   MD5Final(cc->crcSum, &crc_md5);
}

/*
 * The compare action
 */

void RS02Compare(Method *self)
{  compare_closure *cc = g_malloc0(sizeof(compare_closure));
   LargeFile *image;
   EccHeader *eh;
   RS02Layout *lay;
   struct MD5Context image_md5;
   struct MD5Context ecc_md5;
   struct MD5Context meta_md5;
   unsigned char ecc_sum[16];
   unsigned char medium_sum[16];
   char digest[33];
   gint64 s, image_sectors, layer_offset, crc_idx;
   int last_percent = 0;
   unsigned char buf[2048];
   gint64 first_missing, last_missing;
   gint64 total_missing,data_missing,crc_missing,ecc_missing,hdr_missing;
   gint64 data_crc_errors,hdr_crc_errors;
   gint64 hdr_ok,hdr_pos;
   gint64 ecc_sector;
   int ecc_slice;
   int major,minor,pl;
   char method[5];

   /*** Prepare for early termination */

   RegisterCleanup(_("Comparison aborted"), cleanup, cc);

   /*** Open the .iso file */

   LargeStat(Closure->imageName, &image_sectors);
   image_sectors /= 2048;
   image = cc->file = LargeOpen(Closure->imageName, O_RDONLY, IMG_PERMS);

   if(!image)  /* Failing here is unlikely since caller could open it */
     Stop("Could not open %s: %s",Closure->imageName, strerror(errno));

   PrintLog("\n%s: ",Closure->imageName);
   PrintLog(_("present, contains %lld medium sectors.\n"),image_sectors);

   eh  = self->lastEh;  /* will always be present */
   lay = CalcRS02Layout(uchar_to_gint64(eh->sectors), eh->eccBytes); 

   /*** Read the CRC portion */ 

   read_crc(cc, lay);

   /*** Check the data portion of the image file for the
	"dead sector marker" and CRC errors */
   
   if(!LargeSeek(image, 0))
     Stop(_("Failed seeking to start of image: %s\n"), strerror(errno));

   MD5Init(&image_md5);
   MD5Init(&ecc_md5);
   MD5Init(&meta_md5);

   first_missing = last_missing = -1;
   total_missing = data_missing = crc_missing = ecc_missing = 0;
   data_crc_errors = 0;
   crc_idx = 0;

   ecc_sector = 0;
   ecc_slice  = 0;

   layer_offset = lay->sectorsPerLayer - lay->firstCrcLayerIndex - 1;

   for(s=0; s<image_sectors; s++)
   {  int n,percent,current_missing;

      /* Check for user interruption */

      if(Closure->stopActions)   
	goto terminate;

      /* Read the next sector */

      n = LargeRead(image, buf, 2048);
      if(n != 2048)
	 Stop(_("premature end in image (only %d bytes): %s\n"),n,strerror(errno));

      if(s < lay->dataSectors)
	MD5Update(&image_md5, buf, n);

      /* Look for the dead sector marker */

      current_missing = !memcmp(buf, Closure->deadSector, n);
      if(current_missing)
      {  if(first_missing < 0) first_missing = s;
         last_missing = s;
	 total_missing++;
	 if(s < lay->dataSectors) data_missing++;
	 else if(s < lay->protectedSectors) crc_missing++;
	 else ecc_missing++;
      }

      /* Report dead sectors. Combine subsequent missing sectors into one report. */

      if(!current_missing || s==image_sectors-1)
      {  if(first_missing>=0)
	 {   if(first_missing == last_missing)
	           PrintCLI(_("* missing sector   : %lld\n"), first_missing);
	     else PrintCLI(_("* missing sectors  : %lld - %lld\n"), first_missing, last_missing);
	     first_missing = -1;
	 }
      }

      /* If the image sector is from the data portion and it was readable, 
	 test its CRC sum */

      if(s < lay->dataSectors && !current_missing)
      {  guint32 crc = Crc32(buf, 2048);

	 if(crc != cc->crcBuf[crc_idx])
	 {  PrintCLI(_("* CRC error, sector: %lld\n"), s);
	    data_crc_errors++;
	 }
      }
      crc_idx++;

      /* Calculate the ecc checksum */

      if(s == RS02EccSectorIndex(lay, ecc_slice, ecc_sector))
      {  MD5Update(&ecc_md5, buf, 2048);
	 ecc_sector++;
	 if(ecc_sector >= lay->sectorsPerLayer)
	 {  MD5Final(ecc_sum, &ecc_md5); 
	    MD5Init(&ecc_md5);
	    MD5Update(&meta_md5, ecc_sum, 16);

	    ecc_sector = 0;
	    ecc_slice++;
	 }
      }

      if(Closure->guiMode) 
	    percent = (COMPARE_IMAGE_SEGMENTS*s)/image_sectors;
      else  percent = (100*s)/image_sectors;

      if(last_percent != percent) 
      {  PrintProgress(_("- testing sectors  : %3d%%") ,percent);

#if 0
         if(pbar)
	   SetProgress(pbar, percent, 100);

	 if(Closure->guiMode && mode & PRINT_MODE)
	 {  AddCompareValues(percent, ii->sectorsMissing, ii->crcErrors,
			     ii->sectorsMissing - prev_missing,
			     ii->crcErrors - prev_crc_errors);
	    prev_missing = ii->sectorsMissing;
	    prev_crc_errors = ii->crcErrors;
	 }
#endif
	 last_percent = percent;
      }
   }

   /* The image md5sum is only useful if all blocks have been successfully read. */

   MD5Final(medium_sum, &image_md5);
   AsciiDigest(digest, medium_sum);

   MD5Final(ecc_sum, &meta_md5); 
	    

   PrintProgress(_("- testing sectors  : %3d%%"), 100);

   /* Check integrity of the ecc headers */

   hdr_ok = hdr_missing = hdr_crc_errors = 0;
   hdr_pos = lay->firstEccHeader;

   while(hdr_pos < image_sectors - 2)
   {  EccHeader eh;
      int n;

      if(!LargeSeek(image, 2048*hdr_pos))
	Stop(_("Failed seeking to ecc header at %lld: %s\n"), hdr_pos, strerror(errno));

      n = LargeRead(image, &eh, sizeof(EccHeader));
      if(n != sizeof(EccHeader))
	Stop(_("Failed reading ecc header at %lld: %s\n"), hdr_pos, strerror(errno));

      if(!strncmp((char*)eh.cookie, "*dvdisaster*", 12))
      {  guint32 recorded_crc = eh.selfCRC;
 	 guint32 real_crc;

 	 eh.selfCRC = 0x4c5047;
         real_crc = Crc32((unsigned char*)&eh, sizeof(EccHeader));

	 if(real_crc == recorded_crc)
	    hdr_ok++;
	 else
	 {  hdr_crc_errors++; hdr_missing++;
	 }
      }
      else hdr_missing++;

      if(hdr_pos == lay->firstEccHeader)
	   hdr_pos = (lay->protectedSectors + lay->headerModulo - 1) & ~(lay->headerModulo-1);
      else hdr_pos += lay->headerModulo;
   }

   /* Do a resume of our findings */ 

   if(!total_missing && !hdr_missing)
      PrintLog(_("- good image       : all sectors present\n"
		 "- data md5sum      : %s\n"),digest);
   else
   {  gint64 total_crc_errors = data_crc_errors + hdr_crc_errors;
      if(!total_crc_errors)
	PrintLog(_("* BAD image        : %lld sectors missing\n"), total_missing);
      if(!total_missing)
	PrintLog(_("* suspicious image : all sectors present, but %lld CRC errors\n"), total_crc_errors);
      if(total_missing && total_crc_errors)
	PrintLog(_("* BAD image        : %lld sectors missing, %lld CRC errors\n"), 
		 total_missing, total_crc_errors);
      PrintLog(_("  ... ecc headers    : %lld ok, %lld CRC errrors, %lld missing\n"),
		 hdr_ok, hdr_crc_errors, hdr_missing);
      PrintLog(_("  ... data section   : %lld sectors missing; %lld CRC errors\n"), 
	       data_missing, data_crc_errors);
      if(!data_missing)
	PrintLog(_("  ... data md5sum    : %s\n"), digest); 
      PrintLog(_("  ... crc section    : %lld sectors missing\n"), crc_missing);
      PrintLog(_("  ... ecc section    : %lld sectors missing\n"), ecc_missing);
   }

   /*** Print some information on the ecc portion */

   PrintLog(_("\nError correction data: "));

   major = eh->creatorVersion/10000; 
   minor = (eh->creatorVersion%10000)/100;
   pl    = eh->creatorVersion%100;

   if(eh->creatorVersion%100)        
   {  char *format, *color_format = NULL;

      if(eh->methodFlags[3] & MFLAG_DEVEL) 
      {  format = "%s-%d.%d (devel-%d)";
	 color_format = "%s-%d.%d <span color=\"red\">(devel-%d)</span>";
      }
      else if(eh->methodFlags[3] & MFLAG_RC) 
      {  format = "%s-%d.%d (rc-%d)";
	 color_format = "%s-%d.%d <span color=\"red\">(rc-%d)</span>";
      }
      else format = "%s-%d.%d (pl%d)";

      PrintLog(format, _("created by dvdisaster"), major, minor, pl);
      PrintLog("\n");

      if(!color_format) color_format = format;
#if 0
      if(Closure->guiMode)
	SwitchAndSetFootline(wl->cmpEccNotebook, 1,
			     wl->cmpEccCreatedBy, 
			     color_format, "dvdisaster",
			     major, minor, pl);
#endif
   }
   else
   {  PrintLog(_("created by dvdisaster-%d.%d\n"), 
	       major, minor);

#if 0
      if(Closure->guiMode)
	SwitchAndSetFootline(wl->cmpEccNotebook, 1,
			     wl->cmpEccCreatedBy, "dvdisaster-%d.%d",
			     major, minor);
#endif
   }

   /* Error correction method */

   memcpy(method, eh->method, 4); method[4] = 0;

   PrintLog(_("- method           : %4s, %d roots, %4.1f%% redundancy.\n"),
	    method, eh->eccBytes, 
	    ((double)eh->eccBytes*100.0)/(double)eh->dataBytes);

#if 0
   if(Closure->guiMode)
     SetLabelText(GTK_LABEL(Closure->cmpEccMethod), _("%4s, %d roots, %4.1f%% redundancy"),
		  method, eh->eccBytes, 
		  ((double)eh->eccBytes*100.0)/(double)eh->dataBytes);
#endif

   /* required dvdisaster version */

   if(!VerifyVersion(eh, 0))
   {  PrintLog(_("- requires         : dvdisaster-%d.%d (good)\n"),
	       eh->neededVersion/10000,
	       (eh->neededVersion%10000)/100);

#if 0
      if(Closure->guiMode)
	SetLabelText(GTK_LABEL(Closure->cmpEccRequires), "dvdisaster-%d.%d",
		     eh->neededVersion/10000,
		     (eh->neededVersion%10000)/100);
#endif
   }
   else 
   {  PrintLog(_("* requires         : dvdisaster-%d.%d (BAD)\n"
		 "* Warning          : The following output might be incorrect.\n"
		 "*                  : Please visit http://www.dvdisaster.com for an upgrade.\n"),
	       eh->neededVersion/10000,
	       (eh->neededVersion%10000)/100);

#if 0
     if(Closure->guiMode)
     {  SetLabelText(GTK_LABEL(Closure->cmpEccRequires), 
		     "<span color=\"red\">dvdisaster-%d.%d</span>",
		     eh->neededVersion/10000,
		     (eh->neededVersion%10000)/100);
        if(!ecc_advice) 
	  ecc_advice = g_strdup(_("<span color=\"red\">Please upgrade your version of dvdisaster!</span>"));
     }
#endif
   }

   /* image md5sum as stored in the ecc header */

   if(!data_missing)
   {  int n = !memcmp(eh->mediumSum, medium_sum, 16);

      if(n) PrintLog(_("- data md5sum      : %s (good)\n"),digest);
      else  PrintLog(_("* data md5sum      : %s (BAD)\n"),digest);
#if 0
      if(Closure->guiMode)
      {  if(n) SetLabelText(GTK_LABEL(Closure->cmpEccImgMd5Sum), "%s", edigest);
	 else  
	 {  SetLabelText(GTK_LABEL(Closure->cmpEccImgMd5Sum), "<span color=\"red\">%s</span>", edigest);
	    SetLabelText(GTK_LABEL(Closure->cmpImageMd5Sum), "<span color=\"red\">%s</span>", idigest);
	 }
      }
#endif
   }
   else 
   {  PrintLog(_("- data md5sum      : %s\n"),digest);
#if 0
      if(Closure->guiMode)
	SetLabelText(GTK_LABEL(Closure->cmpEccImgMd5Sum), "%s", edigest);
#endif
   }

   /*** md5sum of the crc portion */

   AsciiDigest(digest, cc->crcSum);
   if(!memcmp(eh->crcSum, cc->crcSum, 16))
        PrintLog(_("- crc md5sum       : %s (good)\n"),digest);
   else PrintLog(_("* crc md5sum       : %s (BAD)\n"),digest);


   /*** meta md5sum of the ecc slices */

   AsciiDigest(digest, ecc_sum);
   if(!memcmp(eh->eccSum, ecc_sum, 16))
        PrintLog(_("- ecc md5sum       : %s (good)\n"),digest);
   else PrintLog(_("* ecc md5sum       : %s (BAD)\n"),digest);

   /*** Close and clean up */

terminate:
   cleanup((gpointer)cc);
}
