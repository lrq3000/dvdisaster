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

#include "scsi-layer.h"
#include "udf.h"

#include "rs02-includes.h"

//#define CHECK_VISITED 1        /* This gives only reasonable output            */
                                 /* when the .ecc file is present while reading! */ 

/***
 *** Local data package used during reading
 ***/

enum { IMAGE_ONLY, ECC_IN_FILE, ECC_IN_IMAGE };

typedef struct
{  DeviceHandle *dh;            /* device we are reading from */
   gint64 sectors;              /* sectors in medium (maybe cooked value from file system) */
   LargeFile *image;            /* image file */
   int readMode;                /* see above enum for choices */
   EccInfo *ei;                 /* ecc info and header struct */
   EccHeader *eh;               /* if ecc information is present */
   int rs01LayerSectors;        /* length of each RS01 ecc layer in sectors */
   RS02Layout *lay;             /* layout of RS02 type ecc data */

   Bitmap *map;                 /* bitmap for keeping track of read sectors */
   guint32 *crcbuf;             /* crc sum buffer space */

   gint64 readable;             /* current outcome of reading process */
   gint64 unreadable;
   gint64 correctable;

   gint64 firstSector;          /* user limited reading range */
   gint64 lastSector;

   gint64 *intervals;           /* queue for keeping track of unread intervals */
   gint64 maxIntervals;
   gint64 nIntervals;

   gint64 intervalStart;        /* information about currently processed interval */
   gint64 intervalEnd;
   gint64 intervalSize;
   gint64 maxImageSector;       /* current size of image file */ 
 
   char progressMsg[256];       /* message output related */
   char progressBs[256];
   char progressSp[256];
   int  progressMsgLen;
   int  lastPercent;            /* used to determine next progress update */
   gint64 lastUnreadable;       /* used to find out whether something changed */
   gint64 lastCorrectable;      /* since last progress output */
   char *subtitle;              /* description of reading mode */
 
   int sectorsPerSegment;       /* number of sectors per spiral segment */
   int *segmentState;           /* tracks whether all sectors within segment are processed */

   int earlyTermination;        /* information about termination cause */

#ifdef CHECK_VISITED
   char *count;
#endif

} read_closure;

static void cleanup(gpointer data)
{  read_closure *rc = (read_closure*)data;

   Closure->cleanupProc = NULL;

   if(Closure->guiMode)
   {  if(rc->earlyTermination)
        SetAdaptiveReadFootline(_("Aborted by unrecoverable error."), Closure->red);

      AllowActions(TRUE);
   }

   if(rc->image)   
     if(!LargeClose(rc->image))
       Stop(_("Error closing image file:\n%s"), strerror(errno));
   if(rc->dh)      CloseDevice(rc->dh);
 
   if(rc->ei) FreeEccInfo(rc->ei);

   if(rc->subtitle) g_free(rc->subtitle);
   if(rc->segmentState) g_free(rc->segmentState);

   if(rc->intervals) g_free(rc->intervals);
   if(rc->crcbuf) g_free(rc->crcbuf);

   if(rc->map)
     FreeBitmap(rc->map);

   g_free(rc);

   g_thread_exit(0);
}

/***
 *** Sorted queue of unread intervals
 ***/

/*
 * Sort new interval into the queue
 */

static void add_interval(read_closure *rc, gint64 start, gint64 size)
{  int i,si;
  
  /* Trivial case: empty interval list */

  if(rc->nIntervals == 0)
  {  rc->intervals[0] = start;
     rc->intervals[1] = size;
     rc->nIntervals++;
     return;
  }

  /* Find insertion place in list */

  for(i=0,si=0; i<rc->nIntervals; i++,si+=2)
    if(size > rc->intervals[si+1])
      break;

  /* Make sure we have enough space in the array */

  rc->nIntervals++;
  if(rc->nIntervals > rc->maxIntervals)
  {  rc->maxIntervals *= 2;
     
     rc->intervals = g_realloc(rc->intervals, rc->maxIntervals*2*sizeof(gint64));
  }


  /* Shift unless we insert at the list tail */

  if(i<rc->nIntervals-1)
    memmove(rc->intervals+si+2, rc->intervals+si, 2*sizeof(gint64)*(int)(rc->nIntervals-i-1));

  /* Add new pair into the list */
  
  rc->intervals[si]   = start;
  rc->intervals[si+1] = size;
}

/*
 * Remove first element from the queue
 */

static void pop_interval(read_closure *rc)
{  
  if(rc->nIntervals > 0)
  {  rc->nIntervals--;
     memmove(rc->intervals, rc->intervals+2, 2*sizeof(gint64)*(int)rc->nIntervals);
  }
}

/*
 * Print the queue (for debugging purposes only)
 */

void print_intervals(read_closure *rc)
{  int i;

   printf("%lld Intervals:\n", (long long int)rc->nIntervals);
   for(i=0; i<rc->nIntervals; i++)
     printf("%7lld [%7lld..%7lld]\n",
	    (long long int)rc->intervals[2*i+1], 
	    (long long int)rc->intervals[2*i], 
	    (long long int)rc->intervals[2*i]+rc->intervals[2*i+1]-1);
}

/***
 *** Convenience functions for printing the progress message
 ***/

static void print_progress(read_closure *rc, int immediate)
{  int n;
   int total = rc->readable+rc->correctable;
   int percent = (int)((1000LL*(long long)total)/rc->sectors);

   if(Closure->guiMode)
      return;

   if(   rc->lastPercent >= percent 
      && rc->lastCorrectable == rc->correctable
      && rc->lastUnreadable  == rc->unreadable
      && !immediate)
     return;

   rc->lastPercent = percent;
   rc->lastCorrectable = rc->correctable;
   rc->lastUnreadable  = rc->unreadable;

   if(rc->ei)
     n = g_snprintf(rc->progressMsg, 256,
		    _("Repairable: %2d.%1d%% (correctable: %lld; now reading [%lld..%lld], size %lld)"),
		    percent/10, percent%10, rc->correctable, 
		    rc->intervalStart, rc->intervalStart+rc->intervalSize-1, rc->intervalSize);
   else
     n = g_snprintf(rc->progressMsg, 256,
		    _("Repairable: %2d.%1d%% (missing: %lld; now reading [%lld..%lld], size %lld)"),
		    percent/10, percent%10, rc->sectors-rc->readable, 
		    rc->intervalStart, rc->intervalStart+rc->intervalSize-1, rc->intervalSize);

   if(n>255) n = 255;

   /* If the new message is shorter, overwrite old message with spaces */

   if(rc->progressMsgLen > n)
   {  rc->progressSp[rc->progressMsgLen] = 0;
      rc->progressBs[rc->progressMsgLen] = 0;
      PrintCLI("%s%s", rc->progressSp, rc->progressBs);
      rc->progressSp[rc->progressMsgLen] = ' ';
      rc->progressBs[rc->progressMsgLen] = '\b';
   }

   /* Write new message */

   rc->progressBs[n] = 0;
   PrintCLI("%s%s", rc->progressMsg, rc->progressBs);
   rc->progressBs[n] = '\b';



   rc->progressMsgLen = n;
}

static void clear_progress(read_closure *rc)
{
   if(!rc->progressMsgLen || Closure->guiMode)
     return;

   rc->progressSp[rc->progressMsgLen] = 0;
   PrintCLI("%s", rc->progressSp);
   rc->progressSp[rc->progressMsgLen] = ' ';

   rc->progressBs[rc->progressMsgLen] = 0;
   PrintCLI("%s", rc->progressBs);
   rc->progressBs[rc->progressMsgLen] = '\b';

   rc->progressMsgLen = 0;
}

/*
 * Sector markup in the spiral
 */

static void mark_sector(read_closure *rc, gint64 sector, GdkColor *color)
{  int segment;
   int changed = FALSE;

   if(!Closure->guiMode) return;

   segment = sector / rc->sectorsPerSegment;

   if(color)
   {  GdkColor *old = Closure->readAdaptiveSpiral->segmentColor[segment];
      GdkColor *new = old;

      if(color == Closure->red && old != Closure->red)
	new = color;

      if(color == Closure->yellow && old != Closure->red && old != Closure->yellow)
	new = color;

      if(color == Closure->green)
      {  rc->segmentState[segment]++;

	 if(rc->segmentState[segment] >= rc->sectorsPerSegment)
	   new = color;
      }

      if(new != old)
      {  ChangeSegmentColor(color, segment);
	 changed = TRUE;
      }
   }
   else changed = TRUE;

   if(changed)
     UpdateAdaptiveResults(rc->readable, rc->correctable, 
			   rc->sectors-rc->readable-rc->correctable,
			   (int)((1000LL*(rc->readable+rc->correctable))/rc->sectors));
}

/***
 *** Basic device and image handling and sanity checks.
 ***/

/*
 * Open CD/DVD device and .ecc file.
 * Determine reading mode.
 */

static void open_and_determine_mode(read_closure *rc)
{
   rc->dh = OpenAndQueryDevice(Closure->device);
   rc->readMode = IMAGE_ONLY;

   /* See if the medium contains RS02 type ecc information */

   rc->eh = FindHeaderInMedium(rc->dh, rc->dh->sectors-1);
   if(rc->eh)
   {  rc->readMode = ECC_IN_IMAGE;
      rc->lay = CalcRS02Layout(uchar_to_gint64(rc->eh->sectors), rc->eh->eccBytes);
 
#if 1 /* remove me ! */
      { gint64 s,sinv,slice,idx;
	for(s=0; s<rc->dh->sectors-1; s++)
	{  RS02SliceIndex(rc->lay, s, &slice, &idx);
	   sinv = RS02SectorIndex(rc->lay, slice, idx);
	   /*
	   if(s<rc->lay->protectedSectors) sinv = slice*rc->lay->sectorsPerLayer + idx;
	   else  sinv = RS02EccSectorIndex(rc->lay, slice-rc->lay->ndata, idx);
	   */

	   if(slice == -1)
	     printf("Header %lld found at sector %lld\n", idx, s);
           else
	   if(s != sinv) Stop("Failed for sector %lld / %lld:\n"
			      "slice %lld, idx %lld\n",
			      s, sinv, slice, idx);
	}
	printf("RS02SliceIndex() is okay\n");
      }
#endif
   }

   /* else check the current ecc file */
   else
   {  rc->ei = OpenEccFile(READABLE_ECC | PRINT_MODE);
      if(rc->ei)
      {  rc->readMode = ECC_IN_FILE;
	 rc->eh = rc->ei->eh;

	 rc->rs01LayerSectors = (rc->ei->sectors+rc->eh->dataBytes-1)/rc->eh->dataBytes;
      }
   }

   /* Pick suitable message */

   switch(rc->readMode)
   {  case IMAGE_ONLY:
        rc->subtitle = g_strdup_printf(_("Stopping when unreadable intervals < %d."), 
				       Closure->sectorSkip);
	PrintLog(_("Adaptive reading: %s\n"), rc->subtitle); 
	break;

      case ECC_IN_FILE:
      case ECC_IN_IMAGE:
	rc->subtitle = g_strdup(_("Trying to collect enough data for error correction."));
	PrintLog(_("Adaptive reading: %s\n"), rc->subtitle);  
	break;
   }
}

/*
 * Validate image size against length noted in the ecc header
 */

static void check_size(read_closure *rc)
{  
   /* Number of sectors depends on ecc data */
 
   switch(rc->readMode)
   {  case IMAGE_ONLY:
        rc->sectors = rc->dh->sectors;
	return;

      case ECC_IN_FILE:
	rc->sectors = rc->ei->sectors;
	break;

      case ECC_IN_IMAGE:
	rc->sectors = rc->lay->eccSectors + rc->lay->dataSectors;
	break;
   }

   /* Compare size with answer from drive */

   if(rc->sectors < rc->dh->sectors)
   {  int answer;

      answer = ModalWarning(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK_CANCEL, NULL,
			    _("Medium contains %lld sectors more as recorded in the .ecc file\n"
			      "(Medium: %lld sectors; expected from .ecc file: %lld sectors).\n"
			      "Only the first %lld medium sectors will be processed.\n"),
			    rc->dh->sectors-rc->sectors, rc->dh->sectors, rc->sectors,
			    rc->sectors);

      if(!answer)
      {  SetAdaptiveReadFootline(_("Aborted by user request!"), Closure->red);
	 rc->earlyTermination = FALSE;
	 cleanup((gpointer)rc);
      }
   }

   if(rc->sectors > rc->dh->sectors)
   {  int answer;

      answer =  ModalWarning(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK_CANCEL, NULL,
			     _("Medium contains %lld sectors less as recorded in the .ecc file\n"
			       "(Medium: %lld sectors; expected from .ecc file: %lld sectors).\n"),
			     rc->sectors-rc->dh->sectors, rc->dh->sectors, rc->sectors,
			     rc->sectors);

      if(!answer)
      {  SetAdaptiveReadFootline(_("Aborted by user request!"), Closure->red);
	 rc->earlyTermination = FALSE;
	 cleanup((gpointer)rc);
      }

      rc->sectors = rc->dh->sectors;
   }
}

/*
 * Limit reading to user selected range
 */

static void get_reading_range(read_closure *rc)
{
   if(Closure->readStart || Closure->readEnd)
   {  if(!Closure->guiMode) /* more range checks are made below */ 
      {  rc->firstSector = Closure->readStart;
         rc->lastSector  = Closure->readEnd < 0 ? rc->sectors-1 : Closure->readEnd;
      }
      else  /* be more permissive in GUI mode */
      {  rc->firstSector = 0;
 	 rc->lastSector  = rc->sectors-1;

	 if(Closure->readStart <= Closure->readEnd)
	 {  rc->firstSector = Closure->readStart < rc->sectors ? Closure->readStart : rc->sectors-1;
	    rc->lastSector  = Closure->readEnd   < rc->sectors ? Closure->readEnd   : rc->sectors-1;
	 }
      }

      if(rc->firstSector > rc->lastSector || rc->firstSector < 0 || rc->lastSector >= rc->sectors)
	Stop(_("Sectors must be in range [0..%lld].\n"),rc->sectors-1);

      PrintLog(_("Limiting sector range to [%lld,%lld].\n"),rc->firstSector,rc->lastSector);
   }
   else 
   {  rc->firstSector = 0; rc->lastSector = rc->sectors-1;
   }
}

/*
 * Compare image fingerprint against fingerprint stored in error correction file 
 */

static void check_fingerprint(read_closure *rc)
{  unsigned char digest[16];
   int status;
   
   status = ReadSectors(rc->dh, Closure->scratchBuf, rc->eh->fpSector, 1);

   if(status) /* Not readable. Bad luck. */
   {  int answer;

      answer = ModalWarning(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK_CANCEL, NULL,
			    _("Sector %d is missing. Can not compare image and ecc fingerprints.\n"
			      "Double check that image and ecc file belong together.\n"),
			    rc->eh->fpSector);

      if(!answer)
      {  SetAdaptiveReadFootline(_("Aborted by user request!"), Closure->red);
	 rc->earlyTermination = FALSE;
	 cleanup((gpointer)rc);
      }
   }
   else 
   {  struct MD5Context image_md5;

      MD5Init(&image_md5);
      MD5Update(&image_md5, (unsigned char*)Closure->scratchBuf, 2048);
      MD5Final(digest, &image_md5);

      if(memcmp(digest, rc->eh->mediumFP, 16))
	Stop(_("Fingerprints of image and ecc file do not match.\n"
	       "Image and ecc file do not belong together.\n"));
   }
}

/*
 * Compare image fingerprint with medium.
 */

int check_image_fingerprint(read_closure *rc)
{  gint32 fingerprint_sector;
   int status,n;
  
   /* Determine fingerprint sector */

   if(rc->eh)  /* If ecc information is present get fp sector number from there */
        fingerprint_sector = rc->eh->fpSector;
   else fingerprint_sector = FINGERPRINT_SECTOR;

   /* Try to read fingerprint sectors from medium and image */

   if(!LargeSeek(rc->image, (gint64)(2048*fingerprint_sector)))
     return 0; /* can't tell, assume okay */

   n = LargeRead(rc->image, Closure->scratchBuf, 2048);
   status = ReadSectors(rc->dh, Closure->scratchBuf+2048, fingerprint_sector, 1);

   if(n != 2048 || status || !memcmp(Closure->scratchBuf, Closure->deadSector, 2048))
     return 0; /* can't tell, assume okay */

   /* If both could be read, compare them */

   if(memcmp(Closure->scratchBuf, Closure->scratchBuf+2048, 2048))
   {  	  
     if(!Closure->guiMode)
       Stop(_("Image file does not match the CD/DVD."));
     else
     {  int answer = ModalDialog(GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK_CANCEL, NULL,
				 _("Image file already exists and does not match the CD/DVD.\n"
				   "The existing image file will be deleted."));
	   
        if(!answer)
	{  rc->earlyTermination = FALSE;
	   SetAdaptiveReadFootline(_("Reading aborted. Please select a different image file."), 
				   Closure->red);
	   cleanup((gpointer)rc);
	}
	else
	{  LargeClose(rc->image);
	   LargeUnlink(Closure->imageName);
	   return TRUE; /* causes reopen of image in caller */
	} 
     }
   }

   return 0;  /* okay */
}

/*
 * Compare image size with medium.
 * TODO: image with byte sizes being not a multiple of 2048.
 */

void check_image_size(read_closure *rc, gint64 image_file_sectors)
{  
   if(image_file_sectors > rc->sectors)
   {  int answer;
	 
      answer = ModalWarning(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK_CANCEL, NULL,
			    _("Image file is %lld sectors longer than inserted medium\n"
			      "(Image file: %lld sectors; medium: %lld sectors).\n"),
			    image_file_sectors-rc->sectors, image_file_sectors, rc->sectors);

      if(!answer)
      {  SetAdaptiveReadFootline(_("Aborted by user request!"), Closure->red);
	 rc->earlyTermination = FALSE;
	 cleanup((gpointer)rc);
      }

      rc->maxImageSector = rc->sectors;
   }
   else rc->maxImageSector = image_file_sectors;
}

/***
 *** Examine existing image file.
 ***
 * Build an initial interval list from it.
 */

#define CRCBUFSIZE (1024*256)

void build_interval_from_image(read_closure *rc)
{  gint64 s;
   gint64 first_missing, last_missing, current_missing;
   int tail_included = FALSE;
   guint32 crc;
   int crcidx = CRCBUFSIZE;
   int last_percent = 0;

   /*** If we have a separate ecc file we can compare against the
        CRC sum it contains. Position ecc file behind the ecc header. */
  
   if(rc->readMode == ECC_IN_FILE)
   {
     if(!LargeSeek(rc->ei->file, (gint64)sizeof(EccHeader)))
       Stop(_("Failed skipping the ecc header: %s"),strerror(errno));
   }
  
   /*** Rewind image file */

   LargeSeek(rc->image, 0);
   first_missing = last_missing = -1;

   /*** Go through all sectors in the image file.
	Check them for "dead sector markers" 
	and for checksum failures if ecc data is present. */
   
   if(Closure->guiMode)
     SetAdaptiveReadSubtitle(_("Analysing existing image file"));

   for(s=0; s<rc->maxImageSector; s++)
   {  int n,percent;

      /* Check for user interruption. */

      if(Closure->stopActions)   
      {  SetAdaptiveReadFootline(_("Aborted by user request!"), Closure->red);
	 rc->earlyTermination = FALSE;
	 cleanup((gpointer)rc);
      }

      /* Read the next sector */

      n = LargeRead(rc->image, Closure->scratchBuf, 2048);
      if(n != 2048) /* && (s != rc->sectors - 1 || n != ii->inLast)) */
	Stop(_("premature end in image (only %d bytes): %s\n"),n,strerror(errno));

      /* Look for the dead sector marker */

      current_missing = !memcmp(Closure->scratchBuf, Closure->deadSector, n);

      if(current_missing)
	mark_sector(rc, s, Closure->red);

      /* Compare checksums if available */

      if(rc->readMode == ECC_IN_FILE)
      {
	/* If the CRC buf is exhausted, refill it. */

	 if(crcidx >= CRCBUFSIZE)
	 {  int remain = rc->maxImageSector-s;
	    int size;

	    if(remain < CRCBUFSIZE)
	         size = remain*sizeof(guint32);
	    else size = CRCBUFSIZE*sizeof(guint32);

	    if(LargeRead(rc->ei->file, rc->crcbuf, size) != size)
	      Stop(_("Error reading CRC information: %s"),strerror(errno));

	    crcidx = 0;
	 }

	 /* compare the checksums */

	 crc = Crc32(Closure->scratchBuf, 2048); 
	 if(crc != rc->crcbuf[crcidx++] && !current_missing)
	 {  current_missing = 1;
	    mark_sector(rc, s, Closure->yellow);
	 }
      }

      /* Remember sector state */

      if(current_missing)  /* Remember defect sector in current interval */
      {  if(first_missing < 0) first_missing = s;
	    last_missing = s;
      }
      else                 /* Remember good sector in eccStripe bitmap */
      {  rc->readable++;
	 if(rc->map)
	   SetBit(rc->map, s);

	 mark_sector(rc, s, Closure->green);

#ifdef CHECK_VISITED
	 rc->count[s]++;
#endif
      }

      /* Determine end of current interval and write it out */

      if((!current_missing || s>=rc->maxImageSector-1) && first_missing>=0)
      {  if(s>=rc->maxImageSector-1)       /* special case: interval end = image end */
	 {  last_missing = rc->lastSector; /* may happen when image is truncated */
	    tail_included = TRUE;
	 }

	 /* Clip interval by user selected read range */

	 if(first_missing < rc->firstSector)  
	   first_missing = rc->firstSector;
	 if(last_missing > rc->lastSector)
	   last_missing = rc->lastSector;

	 /* add interval to list */

	 if(first_missing <= last_missing)
	   add_interval(rc, first_missing, last_missing-first_missing+1);

	 first_missing = -1;
      }

      /* Visualize the progress */

      percent = (100*s)/rc->maxImageSector;
      if(last_percent != percent) 
      {  if(!Closure->guiMode)
	    PrintProgress(_("Analysing existing image file: %2d%%"),percent);

	 last_percent = percent;
      }
   }

   /*** If the image is shorter than the medium and the missing part
	was not already included in the last interval,
	insert another interval for the missing portion. */

   if(s<rc->lastSector && !tail_included)  /* truncated image? */
     add_interval(rc, s, rc->lastSector-s+1);

   /*** Now that all readable sectors are known,
	determine those which can already be corrected. */

   if(Closure->guiMode)
     SetAdaptiveReadSubtitle(_("Determining correctable sectors"));

   /* RS01 type error correction. */

   if(rc->readMode == ECC_IN_FILE)
   {  for(s=0; s<rc->rs01LayerSectors; s++)
      {  gint64 layer_idx;
	 int j,present=0;

	 /* Count available sectors in each layer */
	 
	 layer_idx = s;
	 for(j=0; j<rc->eh->dataBytes; j++)
	 {  if(   layer_idx >= rc->ei->sectors  /* padding sector */
	       || GetBit(rc->map, layer_idx))
	       present++;

	    layer_idx += rc->rs01LayerSectors;
	 }

	 /* See if remaining sectors are correctable */

	 if(rc->eh->dataBytes-present <= rc->eh->eccBytes)
	 {  layer_idx = s;
	    for(j=0; j<rc->eh->dataBytes; j++)   /* mark them as visited */
	    {  if(   layer_idx < rc->ei->sectors  /* skip padding sectors */
	          && !GetBit(rc->map, layer_idx))
	       {  SetBit(rc->map, layer_idx);
		  rc->correctable++;
#ifdef CHECK_VISITED
		  count[layer_idx]++;
#endif
	       }

	       layer_idx += rc->rs01LayerSectors;
	    }
	 }
      }
   }

   /*** Tell user results of image file analysis */

   if(rc->readMode == ECC_IN_FILE || rc->readMode == ECC_IN_IMAGE)
        PrintLog(_("Analysing existing image file: %lld readable, %lld correctable, %lld still missing.\n"),
		 rc->readable, rc->correctable, rc->sectors-rc->readable-rc->correctable);
   else PrintLog(_("Analysing existing image file: %lld readable, %lld still missing.\n"),
		 rc->readable, rc->sectors-rc->readable-rc->correctable);

   if(Closure->guiMode)
     UpdateAdaptiveResults(rc->readable, rc->correctable, 
			   rc->sectors-rc->readable-rc->correctable,
			   (int)((1000LL*(rc->readable+rc->correctable))/rc->sectors));
}

/***
 *** Main routine for adaptive reading
 ***/

static void insert_buttons(GtkDialog *dialog)
{  
  gtk_dialog_add_buttons(dialog, 
			 _("Ignore once"), 1,
			 _("Ignore always"), 2,
			 _("Abort"), 0, NULL);
} 

/*
 * Fill the gap between rc->intervalStart and rc->maxImageSectors
 * with dead sector markers. These are needed in case the user aborts the operation; 
 * otherwise the gap will just be zero-filled and we don't know its contents
 * are unprocessed if we try to re-read the image later.
 * Also, this prevents fragmentation under most filesystems.
 */

void fill_gap(read_closure *rc)
{  char *anim[] = { ": |*.........|\b\b\b\b\b\b\b\b\b\b\b\b\b\b",
		    ": |.*........|\b\b\b\b\b\b\b\b\b\b\b\b\b\b",
		    ": |..*.......|\b\b\b\b\b\b\b\b\b\b\b\b\b\b",
		    ": |...*......|\b\b\b\b\b\b\b\b\b\b\b\b\b\b",
		    ": |....*.....|\b\b\b\b\b\b\b\b\b\b\b\b\b\b",
		    ": |.....*....|\b\b\b\b\b\b\b\b\b\b\b\b\b\b",
		    ": |......*...|\b\b\b\b\b\b\b\b\b\b\b\b\b\b",
		    ": |.......*..|\b\b\b\b\b\b\b\b\b\b\b\b\b\b",
		    ": |........*.|\b\b\b\b\b\b\b\b\b\b\b\b\b\b",
		    ": |.........*|\b\b\b\b\b\b\b\b\b\b\b\b\b\b"};
  char *t;
  gint64 i,j;

  /*** TODO: this is fishy. */

  if(rc->firstSector==0 || rc->maxImageSector > 0)
    rc->maxImageSector++;

  /*** Tell user what's going on */

  t = g_strdup_printf(_("Filling image area [%lld..%lld]"), 
		      rc->maxImageSector, rc->intervalStart-1);
  clear_progress(rc);
  if(Closure->guiMode)
  {  SetAdaptiveReadSubtitle(t);
     ChangeSpiralCursor(-1); 
  }
  PrintCLI(t);
  g_free(t);

  /*** Seek to end of image */

  if(!LargeSeek(rc->image, (gint64)(2048*rc->maxImageSector)))
    Stop(_("Failed seeking to sector %lld in image [%s]: %s"),
	 rc->maxImageSector, "fill", strerror(errno));

  /*** Fill image with dead sector markers until rc->intervalStart */

  for(i=rc->maxImageSector, j=0; i<rc->intervalStart; i++)
  {  int n;

     /* Write next sector */ 
    
     n = LargeWrite(rc->image, Closure->deadSector, 2048);
     if(n != 2048)
       Stop(_("Failed writing to sector %lld in image [%s]: %s"),
	    rc->maxImageSector, "fill", strerror(errno));

     /* Check whether user hit the Stop button */
	     
     if(Closure->stopActions)
     {  if(Closure->guiMode)
	 SetAdaptiveReadFootline(_("Aborted by user request!"), Closure->red);

        rc->earlyTermination = FALSE;  /* suppress respective error message */
	cleanup((gpointer)rc);
     }

     /* Cycle the progress animation */

     if(j++ % 2000)
     {  int seq = (j/2000)%10;
       
        PrintCLI(anim[seq]);
     }
	
     /* Show progress in the spiral */
    
     if(Closure->guiMode)
     {  int segment = i / rc->sectorsPerSegment;
       
        if(Closure->readAdaptiveSpiral->segmentColor[segment] == Closure->background)
	  ChangeSegmentColor(Closure->white, segment);
     }
  }

  PrintCLI("               \n");
  rc->maxImageSector = rc->intervalStart-1;

  if(Closure->guiMode)  /* remove temporary fill markers */
  {  RemoveFillMarkers();
     SetAdaptiveReadSubtitle(rc->subtitle);
  }
}

/*
 * The adaptive read strategy 
 */

void ReadMediumAdaptive(gpointer data)
{  read_closure *rc;
   int crcidx = 0;
   gint64 s;
   gint64 image_file_size;
   guint32 *crcbuf;
   int status,i,n;

   /*** Initialize the read closure. */

   rc = g_malloc0(sizeof(read_closure));

   crcbuf = rc->crcbuf = g_malloc(sizeof(guint32)*CRCBUFSIZE);

   memset(rc->progressBs, '\b', 256);
   memset(rc->progressSp, ' ', 256);

   /*** Register the cleanup procedure so that Stop() can abort us properly. */

   rc->earlyTermination = TRUE;

   RegisterCleanup(_("Reading aborted"), cleanup, rc);
   if(Closure->guiMode)
     SetLabelText(GTK_LABEL(Closure->readAdaptiveHeadline), "<big>%s</big>\n<i>%s</i>",
		  _("Preparing for reading the medium image."),
		  _("Medium: not yet determined"));

   /*** Open Device and .ecc file. Determine read mode. */

   open_and_determine_mode(rc);

   /*** Validate image size against ecc data */
   
   check_size(rc);

   /*** Limit the read range from users choice */

   get_reading_range(rc);
   rc->intervalStart = rc->firstSector;
   rc->intervalEnd   = rc->lastSector;
   rc->intervalSize  = rc->intervalEnd - rc->intervalStart + 1;

   /*** Compare image and ecc fingerprints (only if RS01 type .ecc is available) */

   if(rc->readMode == ECC_IN_FILE)
     check_fingerprint(rc);

   /*** Initialize the sector bitmap if ecc information is present.
	This is used to stop when sufficient data for error correction
	becomes available. */

   if(   rc->readMode == ECC_IN_FILE
      || rc->readMode == ECC_IN_IMAGE)
      rc->map = CreateBitmap(rc->sectors);

#ifdef CHECK_VISITED
   rc->count = g_malloc0((int)rc->sectors+160);
#endif

   /*** Initialize segment state counters (only in GUI mode) */

   if(Closure->guiMode)
   {  rc->sectorsPerSegment = 1 + (rc->sectors / ADAPTIVE_READ_SPIRAL_SIZE);
      rc->segmentState = g_malloc0(ADAPTIVE_READ_SPIRAL_SIZE * sizeof(int));
      ClipReadAdaptiveSpiral(rc->sectors/rc->sectorsPerSegment);
   }

   /*** Mark RS02 header sectors as correctable.
        These are not part of any ecc block and have no influence on
	the decision when enough data has been gathered for error correction.
	Since they are needed for recognizing the image we will rewrite all
	them from the copy we got in rc->eh, but this can only be done when
	the image file has been fully created. */

   if(rc->readMode == ECC_IN_IMAGE)
   {  gint64 hpos, end;

      hpos = (rc->lay->protectedSectors + rc->lay->headerModulo - 1) / rc->lay->headerModulo;
      hpos *= rc->lay->headerModulo;
      end  = rc->lay->eccSectors + rc->lay->dataSectors - 2;

      while(hpos < end)
      {  SetBit(rc->map, hpos);
	 SetBit(rc->map, hpos+1);
	 mark_sector(rc, hpos, Closure->green);
	 mark_sector(rc, hpos+1, Closure->green);
	 hpos += rc->lay->headerModulo;
	 rc->correctable+=2;
      }
   }

   /*** Initialize the interval list */

   rc->intervals = g_malloc(8*sizeof(gint64));
   rc->maxIntervals = 4; 
   rc->nIntervals = 0; 

   /*** Start with a fresh image file if none is already present. */

reopen_image:
   if(!LargeStat(Closure->imageName, &image_file_size))
   {  if(!(rc->image = LargeOpen(Closure->imageName, O_RDWR | O_CREAT, IMG_PERMS)))
	Stop(_("Can't open %s:\n%s"),Closure->imageName,strerror(errno));

      PrintLog(_("Creating new %s image.\n"),Closure->imageName);
      if(Closure->guiMode)
	SetLabelText(GTK_LABEL(Closure->readAdaptiveHeadline),
		     "<big>%s</big>\n<i>%s</i>",
		     _("Reading new medium image."),
		     rc->dh->mediumDescr);
   }

   /*** else examine the existing image file ***/

   else 
   {  int reopen;

      if(Closure->guiMode)
	SetLabelText(GTK_LABEL(Closure->readAdaptiveHeadline),
		     "<big>%s</big>\n<i>%s</i>",
		     _("Completing existing medium image."),
		     rc->dh->mediumDescr);

      /* Open the existing image file. */

      if(!(rc->image = LargeOpen(Closure->imageName, O_RDWR, IMG_PERMS)))
	Stop(_("Can't open %s:\n%s"),Closure->imageName,strerror(errno));

      /* See if the image fingerprint matches the medium */

      reopen = check_image_fingerprint(rc);
      if(reopen) 
	goto reopen_image;  /* no match, user wants to erase old image */

      /* Compare length of image and medium. */

      check_image_size(rc, image_file_size / 2048);

      /* Build the interval list */

      build_interval_from_image(rc);

      /* Already enough information available? */
	 
      if(rc->readable + rc->correctable >= rc->sectors)
      {  char *t = _("\nSufficient data for reconstructing the image is available.\n");
	 PrintLog(t);
	 if(Closure->guiMode)
	   SetAdaptiveReadFootline(t, Closure->darkgreen);
	 goto finished;
      }

      /* Nope, begin with first interval */

      if(!rc->nIntervals)  /* may happen when reading range is restricted too much */
	goto finished;

      rc->intervalStart = rc->intervals[0];
      rc->intervalSize  = rc->intervals[1];
      rc->intervalEnd   = rc->intervalStart + rc->intervalSize - 1;
      pop_interval(rc);
   }

   /*** Read the medium image. */

   if(Closure->guiMode)
     SetAdaptiveReadSubtitle(rc->subtitle);

   for(;;)
   {  
      /* If we jumped beyond the max_sector, fill the gap with dead sector markers. */

      if(rc->intervalStart > rc->maxImageSector+1)
	fill_gap(rc);

      /*** Try reading the next interval */

      print_progress(rc, TRUE);
      crcidx = CRCBUFSIZE;

      for(s=rc->intervalStart; s<=rc->intervalEnd; ) /* s is incremented elsewhere */
      {  int nsectors,cnt;
	 int ignore_fatal = 0;
 
	 if(Closure->stopActions)          /* somebody hit the Stop button */
	 {  if(Closure->guiMode)
	       SetAdaptiveReadFootline(_("Aborted by user request!"), Closure->red);

	    rc->earlyTermination = FALSE;  /* suppress respective error message */
	    goto terminate;
	 }

	 if(Closure->guiMode)
	   ChangeSpiralCursor(s / rc->sectorsPerSegment);
	    
	 /* Determine number of sectors to read. Read the next 16 sectors
	    unless we're at the end of the interval or at a position which is
	    not divideable by 16. */

	 if(s & 15)
               nsectors = 1;
	 else  nsectors = 16;

	 if(s+nsectors > rc->intervalEnd) nsectors = rc->intervalEnd-s+1;

	 /* Skip sectors which have been marked as correctable by
	    ecc information. */

	 if(rc->map)
	 {  for(i=cnt=0; i<nsectors; i++)  /* sectors already present? */
	    {  int idx = s+i;

	       if(GetBit(rc->map, idx))
		 cnt++;
	    }

	    /* Shift the outer loop down to 1 sector per read.
	       Short circuit the outer loop if the sector is already present. */

	    if(cnt) 
	    {  nsectors = 1;

	       if(GetBit(rc->map, s))
	       {  s++;
		  crcidx++;
		  continue;  /* restart reading loop with next sector */
	       }
	    }
	 }

	 /* Try to actually read the next sector(s) */

	 status = ReadSectors(rc->dh, Closure->scratchBuf, s, nsectors);

	 /* Medium Error (3) and Illegal Request (5) may result from 
	    a medium read problem, but other errors are regarded as fatal. */

	 if(status && !ignore_fatal
	    && rc->dh->sense.sense_key 
	    && rc->dh->sense.sense_key != 3 && rc->dh->sense.sense_key != 5)
	 {  int answer;

	    if(!Closure->guiMode)
	      Stop(_("Sector %lld: %s\nCan not recover from above error."),
		   s, GetLastSenseString(FALSE));

	    answer = ModalDialog(GTK_MESSAGE_QUESTION, GTK_BUTTONS_NONE, insert_buttons,
				 _("Sector %lld: %s\n\n"
				   "It may not be possible to recover from this error.\n"
				   "Should the reading continue and ignore this error?"),
				 s, GetLastSenseString(FALSE));

	    if(answer == 2)
	      ignore_fatal = TRUE;

	    if(!answer)
	    {  SetAdaptiveReadFootline(_("Aborted by unrecoverable error."), Closure->red);

	       rc->earlyTermination = FALSE;  /* suppress respective error message */
	       goto terminate;
	    }
	 }

	 /* Reading was successful. */

	 if(!status)   
	 {  gint64 b;

	    if(!LargeSeek(rc->image, (gint64)(2048*s)))
	      Stop(_("Failed seeking to sector %lld in image [%s]: %s"),
		   s,"store",strerror(errno));

	    /* Store sector(s) in the image file if they pass the CRC test,
	       otherwise treat them as unprocessed. */

	    for(i=0, b=s; i<nsectors; i++,b++)
	    {  guint32 crc;

	       if(rc->readMode == ECC_IN_FILE) /* we have crc information */
	       { 
		 /* If the CRC buf is exhausted, refill it. */

		 if(crcidx >= CRCBUFSIZE)  
		 {  int remain = rc->sectors-b;
		    int size;

		    if(remain < CRCBUFSIZE)
		         size = remain*sizeof(guint32);
	            else size = CRCBUFSIZE*sizeof(guint32);

		    if(!LargeSeek(rc->ei->file, (gint64)sizeof(EccHeader)+b*4))
		      Stop(_("Failed seeking in crc area: %s"),strerror(errno));

		    if(LargeRead(rc->ei->file, crcbuf, size) != size)
		      Stop(_("Error reading CRC information: %s"),strerror(errno));

		    crcidx = 0;
		 }

		 /* Calculate and compare CRC sums.
		    Sectors with bad CRC sums are marked unvisited,
		    but do not terminate the current interval. */

		 crc = Crc32(Closure->scratchBuf+i*2048, 2048); 

		 if(crc != crcbuf[crcidx++])  /* failed CRC test */
		 {  PrintCLI("\n");
		    PrintCLI(_("CRC error in sector %lld\n"),b);
		    print_progress(rc, TRUE);

		    n = LargeWrite(rc->image, Closure->deadSector, 2048);
		    if(n != 2048)
		      Stop(_("Failed writing to sector %lld in image [%s]: %s"),
			   b, "unv", strerror(errno));

		    mark_sector(rc, b, Closure->yellow);
		 }
		 else /* good sector */
		 {  n = LargeWrite(rc->image, Closure->scratchBuf+i*2048, 2048);
		    if(n != 2048)
		      Stop(_("Failed writing to sector %lld in image [%s]: %s"),
			   b, "store", strerror(errno));

		    SetBit(rc->map, b);
		    rc->readable++;

		    mark_sector(rc, b, Closure->green);
		 }
	       }

	       else /* no crc information available */
	       {  n = LargeWrite(rc->image, Closure->scratchBuf+i*2048, 2048);
		  if(n != 2048)
		    Stop(_("Failed writing to sector %lld in image [%s]: %s"),
			 b, "store", strerror(errno));

		  if(rc->map)
		    SetBit(rc->map, b);
		  rc->readable++;

		  mark_sector(rc, b, Closure->green);
	       }
	    }

	    /* See if additional sectors become correctable. */
	    
	    if(rc->readMode == ECC_IN_FILE)  /* RS01 type ecc data */
	    {  for(i=0, b=s; i<nsectors; i++,b++) 
	       {  int j,present=0;
		  gint64 layer_idx;

#ifdef CHECK_VISITED
		  count[b]++;
#endif
		  /* Count available sectors. */

		  layer_idx = b % rc->rs01LayerSectors;
		  for(j=0; j<rc->eh->dataBytes; j++) 
		  {  if(   layer_idx >= rc->ei->sectors /* padding sector */
			|| GetBit(rc->map, layer_idx))
			present++;
		     layer_idx += rc->rs01LayerSectors;
		  }

		  /* If the remaining sectors are correctable,
		     mark them as visited. */

		  if(rc->eh->dataBytes-present <= rc->eh->eccBytes)
		  {  layer_idx = b % rc->rs01LayerSectors;

		     for(j=0; j<rc->eh->dataBytes; j++)
		     {  if(   layer_idx < rc->ei->sectors /* skip padding sector */
			   && !GetBit(rc->map, layer_idx))
			{  SetBit(rc->map, layer_idx);
			   rc->correctable++;
			   mark_sector(rc, layer_idx, Closure->green);
#ifdef CHECK_VISITED
			   count[layer_idx]++;
#endif
			}
		        layer_idx += rc->rs01LayerSectors;
		     }
		  }
	       }
	    }

	    if(rc->readMode == ECC_IN_IMAGE)  /* RS02 type ecc data */
	    {  for(i=0, b=s; i<nsectors; i++,b++) 
	       {  int j,present=0;
		  gint64 slice_idx, ignore, sector;

		  /* Count available sectors. */

		  RS02SliceIndex(rc->lay, b, &ignore, &slice_idx);
		  for(j=0; j<255; j++) 
		  {  sector = RS02SectorIndex(rc->lay, j, slice_idx);
		     if(GetBit(rc->map, sector))
			present++;
		  }

		  /* If the remaining sectors are correctable,
		     mark them as visited. */

		  if(255-present <= rc->eh->eccBytes)
		  {  for(j=0; j<255; j++)
		     {  sector = RS02SectorIndex(rc->lay, j, slice_idx);
		        if(!GetBit(rc->map, sector))
			{  SetBit(rc->map, sector);
			   rc->correctable++;
			   mark_sector(rc, sector, Closure->green);
			}
		     }
		  }
	       }
	    }

	    /* Increment sector counters. Adjust max image sector
	       if we added sectors beyond the image size. */

	    s+=nsectors;
	    if(s>rc->maxImageSector) rc->maxImageSector=s-1;

	    /* Stop reading if enough data for error correction
	       has benn gathered */

	    if(rc->readable + rc->correctable >= rc->sectors)
	    {  char *t = _("\nSufficient data for reconstructing the image is available.\n");

	       print_progress(rc, TRUE);
	       PrintLog(t);
	       if(Closure->guiMode && rc->ei)
		 SetAdaptiveReadFootline(t, Closure->black);
	       goto finished;
	    }
	 }  /* end of if(!status) (successful reading of sector(s)) */

	 else  /* Process the read error. */
	 {  PrintCLI("\n");
	    if(nsectors>1) PrintCLIorLabel(Closure->status,
					   _("Sectors %lld-%lld: %s\n"),
					   s, s+nsectors-1, GetLastSenseString(FALSE));  
	    else	   PrintCLIorLabel(Closure->status,
					   _("Sector %lld: %s\n"),
					   s, GetLastSenseString(FALSE));  

	    rc->unreadable += nsectors;

	    /* Write nsectors of "dead sector" markers */

	    if(!LargeSeek(rc->image, (gint64)(2048*s)))
	      Stop(_("Failed seeking to sector %lld in image [%s]: %s"),
		   s, "nds", strerror(errno));

	    for(i=0; i<nsectors; i++)
	    {  n = LargeWrite(rc->image, Closure->deadSector, 2048);
	       if(n != 2048)
		 Stop(_("Failed writing to sector %lld in image [%s]: %s"),
		      s, "nds", strerror(errno));

	       mark_sector(rc, s+i, Closure->red);
	    }

	    if(rc->maxImageSector < s+nsectors)
	      rc->maxImageSector = s+nsectors-1;

	    /* Reading of the interval ends at the first read error.
	       Store the remainder of the current interval in the queue. */

	    if(s+nsectors-1 >= rc->intervalEnd)  /* This was the last sector; interval used up */
	    {  Verbose("... Interval [%lld..%lld] used up\n", rc->intervalStart, rc->intervalEnd);
	    }
	    else  /* Insert remainder of interval into queue */
	    {  rc->intervalStart = s+nsectors;
	       rc->intervalSize  = rc->intervalEnd-rc->intervalStart+1;

	       Verbose("... Interval %lld [%lld..%lld] added\n", 
		       rc->intervalSize, rc->intervalStart, rc->intervalStart+rc->intervalSize-1);

	       add_interval(rc, rc->intervalStart, rc->intervalSize);
	       if(Closure->verbose) print_intervals(rc);
	    }
	    break; /* fall out of reading loop */
	 }

	 print_progress(rc, FALSE);
      }

      /* If we reach this, the current interval has either been read completely
	 or the loop was terminated early by a read error. 
         In both cases, the current interval has already been remove from the queue
         and the queue contains only the still unprocessed intervals. */

      if(s>=rc->intervalEnd) /* we fell out of the reading loop with interval completed */
      {  print_progress(rc, TRUE);
	 PrintCLI("\n");
      }

      /* Pop the next interval from the queue,
         prepare one half from it for processing
         and push the other half back on the queue. */

      if(rc->nIntervals <= 0)
	goto finished;

      rc->intervalStart = rc->intervals[0];
      rc->intervalSize  = rc->intervals[1];
      pop_interval(rc);

      /* Split the new interval */

      if(rc->intervalSize>1)
      {  Verbose("*** Splitting [%lld..%lld]\n",
		 rc->intervalStart,rc->intervalStart+rc->intervalSize-1);

	 add_interval(rc, rc->intervalStart, rc->intervalSize/2);
	 rc->intervalEnd = rc->intervalStart+rc->intervalSize-1;
	 rc->intervalStart = rc->intervalStart+rc->intervalSize/2;
      }
      else /* 1 sector intervals can't be split further */
      {  
	 rc->intervalEnd = rc->intervalStart;
	 Verbose("*** Popped [%lld]\n",rc->intervalStart);
      }

      if(Closure->verbose) print_intervals(rc);

      rc->intervalSize  = rc->intervalEnd-rc->intervalStart+1;

      /* Apply interval size termination criterion */

      if(   rc->readMode == IMAGE_ONLY
	 && rc->intervalSize < Closure->sectorSkip)
	 goto finished;
   }

finished:

#ifdef CHECK_VISITED
   {  int i,cnt=0;
      for(i=0; i<(int)rc->sectors; i++)
      {  cnt+=count[i];
         if(count[i] != 1)
           printf("Sector %d: %d\n",i,count[i]);
      }

      printf("\nTotal visited %d (%d)\n",cnt,i);

      for(i=(int)rc->sectors; i<(int)rc->sectors+160; i++)
        if(count[i] != 0)
          printf("SECTOR %d: %d\n",i,count[i]);

      g_free(count);
   }
#endif

   /* Force output of final results */

   if(Closure->guiMode)
   {  ChangeSpiralCursor(-1);
      mark_sector(rc, 0, NULL);
   }

   /*** Summarize results. */

   /* We were in ECC_IN_FILE or ECC_IN_IMAGE mode,
      but did not recover sufficient data. */

   if(rc->map && (rc->readable + rc->correctable < rc->sectors))
   {  int total = rc->readable+rc->correctable;
      int percent = (int)((1000LL*(long long)total)/rc->sectors);
      char *t = g_strdup_printf(_("Only %2d.%1d%% of the image are readable or correctable"),
				  percent/10, percent%10); 

      PrintLog(_("\n%s\n"
		  "(%lld readable,  %lld correctable,  %lld still missing).\n"),
		t, rc->readable, rc->correctable, rc->sectors-total);
      if(Closure->guiMode)
	 SetAdaptiveReadFootline(t, Closure->black);

      g_free(t);
      exitCode = EXIT_FAILURE;
   }

   /* Results for reading in IMAGE_ONLY mode */

   if(rc->readMode == IMAGE_ONLY)
   {  if(rc->readable == rc->sectors)
      {  char *t = _("\nGood! All sectors have been read.\n"); 
	 PrintLog(t);
	 if(Closure->guiMode)
	   SetAdaptiveReadFootline(t, Closure->black);
      }
      else
      {  int percent = (int)((1000LL*rc->readable)/rc->sectors);
	 char *t = g_strdup_printf(_("No unreadable intervals with >= %d sectors left."),
				   Closure->sectorSkip);

	 PrintLog(_("\n%s\n" 
		     "%2d.%1d%% of the image have been read (%lld sectors).\n"),
		   t, percent/10, percent%10, rc->readable);

	 if(Closure->guiMode)
	   SetAdaptiveReadFootline(t, Closure->black);
	 g_free(t);
	 exitCode = EXIT_FAILURE;
      }
   }

#if 0
   for(i=0; i<rc->sectors; i++)
     if(!GetBit(rc->map, i))
       printf("Missing: %d\n", i);
#endif

   /*** Close and clean up */

   if(rc->readMode == ECC_IN_IMAGE)   /* rewrite the header sectors */
     WriteRS02Headers(rc->image, rc->lay, rc->eh);

   rc->earlyTermination = FALSE;

terminate:
   cleanup((gpointer)rc);
}

