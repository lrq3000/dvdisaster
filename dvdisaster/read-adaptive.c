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

//#define VERBOSE 1
//#define CHECK_VISITED 1        /* This gives only reasonable output            */
                                 /* when the .ecc file is present while reading! */ 

/***
 *** Local data package used during reading
 ***/

typedef struct
{  LargeFile *image;
   EccInfo *ei;
   DeviceHandle *dh;
   gint64 *intervals;
   gint64 max_intervals;
   gint64 n_intervals;
   unsigned char *bufbase;
   guint32 *crcbuf;
   int earlyTermination;
   gint64 readable, unreadable, correctable;
   gint64 interval_start, interval_size;
   guint32 *eccStripe[256];
   int eccStripeLen;
   char progressMsg[256];
   char progressBs[256];
   char progressSp[256];
   int progressMsgLen;
   char *subtitle;
   int sectorsPerSegment;
   int *segmentState;
   int lastPercent;
   gint64 lastUnreadable, lastCorrectable;
} read_closure;

static void cleanup(gpointer data)
{  read_closure *rc = (read_closure*)data;
   int i;

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

   for(i=0; i<255; i++)
     if(rc->eccStripe[i])
       g_free(rc->eccStripe[i]);

   if(rc->intervals) g_free(rc->intervals);
   if(rc->bufbase) g_free(rc->bufbase);
   if(rc->crcbuf) g_free(rc->crcbuf);

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

  if(rc->n_intervals == 0)
  {  rc->intervals[0] = start;
     rc->intervals[1] = size;
     rc->n_intervals++;
     return;
  }

  /* Find insertion place in list */

  for(i=0,si=0; i<rc->n_intervals; i++,si+=2)
    if(size > rc->intervals[si+1])
      break;

  /* Make sure we have enough space in the array */

  rc->n_intervals++;
  if(rc->n_intervals > rc->max_intervals)
  {  rc->max_intervals *= 2;
     
     rc->intervals = g_realloc(rc->intervals, rc->max_intervals*2*sizeof(gint64));
  }


  /* Shift unless we insert at the list tail */

  if(i<rc->n_intervals-1)
    memmove(rc->intervals+si+2, rc->intervals+si, 2*sizeof(gint64)*(int)(rc->n_intervals-i-1));

  /* Add new pair into the list */
  
  rc->intervals[si]   = start;
  rc->intervals[si+1] = size;
}

/*
 * Remove first element from the queue
 */

static void pop_interval(read_closure *rc)
{  
  if(rc->n_intervals > 0)
  {  rc->n_intervals--;
    memmove(rc->intervals, rc->intervals+2, 2*sizeof(gint64)*(int)rc->n_intervals);
  }
}

/*
 * Print the queue (for debugging purposes only)
 */

void print_intervals(read_closure *rc)
{  int i;

   printf("%lld Intervals:\n", (long long int)rc->n_intervals);
   for(i=0; i<rc->n_intervals; i++)
     printf("%7lld [%7lld..%7lld]\n",
	    (long long int)rc->intervals[2*i+1], 
	    (long long int)rc->intervals[2*i], 
	    (long long int)rc->intervals[2*i]+rc->intervals[2*i+1]-1);
}

/***
 *** Convenience function for printing the progress message
 ***/

static void print_progress(read_closure *rc, int immediate)
{  int n;
   int total = rc->readable+rc->correctable;
   int percent = (int)((1000LL*(long long)total)/rc->dh->sectors);

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
		    rc->interval_start, rc->interval_start+rc->interval_size-1, rc->interval_size);
   else
     n = g_snprintf(rc->progressMsg, 256,
		    _("Repairable: %2d.%1d%% (missing: %lld; now reading [%lld..%lld], size %lld)"),
		    percent/10, percent%10, rc->dh->sectors-rc->readable, 
		    rc->interval_start, rc->interval_start+rc->interval_size-1, rc->interval_size);

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
   //   if(segment<0 || segment>4799) printf("falsches Segment %d, Sektor %lld\n",segment,sector);

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
			   rc->dh->sectors-rc->readable-rc->correctable,
			   (int)((1000LL*(rc->readable+rc->correctable))/rc->dh->sectors));
#if 0
   if(!color || ChangeSegmentColor(color, (sector*ADAPTIVE_READ_SPIRAL_SIZE)/rc->dh->sectors))
     UpdateAdaptiveResults(rc->readable, rc->correctable, 
			   rc->dh->sectors-rc->readable-rc->correctable,
			   (int)((1000LL*(rc->readable+rc->correctable))/rc->dh->sectors));
#endif
}

/***
 *** Try reading the medium and create the image and map.
 ***/

#define CRCBUFSIZE (1024*256)

static void insert_buttons(GtkDialog *dialog)
{  
  gtk_dialog_add_buttons(dialog, 
			 _("Ignore once"), 1,
			 _("Ignore always"), 2,
			 _("Abort"), 0, NULL);
} 

void ReadMediumAdaptive(gpointer data)
{  read_closure *rc;
   EccHeader *eh = NULL;
   int crcidx = 0;
   gint64 start,size,end,s;
   gint64 first_sector, last_sector, last_sector_max;
   gint64 image_file_size,image_file_sectors;
   gint64 max_sector = 0;
   unsigned char *buf = NULL;
   guint32 *crcbuf;
   int status,i,n;
#ifdef CHECK_VISITED
   char *count;
#endif

   /*** Initialize the read closure.
	Align the read buffer at a 4096 boundary.
	Might be needed by some SCSI drivers. */

   rc = g_malloc0(sizeof(read_closure));

   rc->bufbase = g_malloc(32768 + 4096);
   buf = rc->bufbase + (4096 - ((unsigned long)rc->bufbase & 4095));

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

   /*** Open Device and query medium properties */

   rc->dh = OpenAndQueryDevice(Closure->device);
   last_sector_max = rc->dh->sectors-1;

   /*** Open the ecc file. */

   rc->ei = OpenEccFile(READABLE_ECC | PRINT_MODE);

   if(!rc->ei) 
   {  rc->subtitle = g_strdup_printf(_("Stopping when unreadable intervals < %d."), 
				     Closure->sectorSkip);
      PrintLog(_("Adaptive reading: %s\n"), rc->subtitle); 
   }
   else
   {  rc->subtitle = g_strdup(_("Trying to collect enough data for error correction."));
      PrintLog(_("Adaptive reading: %s\n"), rc->subtitle);  
      eh=rc->ei->eh;

      if(rc->ei->sectors < rc->dh->sectors)
      {  int answer;

	 answer = ModalWarning(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK_CANCEL, NULL,
			       _("Medium contains %lld sectors more as recorded in the .ecc file\n"
				 "(Medium: %lld sectors; expected from .ecc file: %lld sectors).\n"
				 "Only the first %lld medium sectors will be processed.\n"),
			       rc->dh->sectors-rc->ei->sectors, rc->dh->sectors, rc->ei->sectors,
			       rc->ei->sectors);

	 rc->dh->sectors = rc->ei->sectors;

	 if(!answer)
	 {  SetAdaptiveReadFootline(_("Aborted by user request!"), Closure->red);
 	    rc->earlyTermination = FALSE;
	    goto terminate;
	 }
      }

      if(rc->ei->sectors > rc->dh->sectors)
      {  int answer;

	 answer =  ModalWarning(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK_CANCEL, NULL,
				_("Medium contains %lld sectors less as recorded in the .ecc file\n"
				  "(Medium: %lld sectors; expected from .ecc file: %lld sectors).\n"),
				rc->ei->sectors-rc->dh->sectors, rc->dh->sectors, rc->ei->sectors,
				rc->ei->sectors);

	 last_sector = rc->dh->sectors-1;
	 rc->dh->sectors = rc->ei->sectors;

	 if(!answer)
	 {  SetAdaptiveReadFootline(_("Aborted by user request!"), Closure->red);
 	    rc->earlyTermination = FALSE;
	    goto terminate;
	 }
      }
   }

   /*** See if user wants to limit the read range. */

   if(Closure->readStart || Closure->readEnd)
   {  if(!Closure->guiMode)
      {  first_sector = Closure->readStart;
         last_sector  = Closure->readEnd < 0 ? rc->dh->sectors-1 : Closure->readEnd;
      }
      else  /* be more permissive in GUI mode */
      {  first_sector = 0;
 	 last_sector  = rc->dh->sectors-1;

	 if(Closure->readStart <= Closure->readEnd)
	 {  first_sector = Closure->readStart < rc->dh->sectors ? Closure->readStart : rc->dh->sectors-1;
	    last_sector  = Closure->readEnd   < rc->dh->sectors ? Closure->readEnd   : rc->dh->sectors-1;
	 }
      }

      if(first_sector>last_sector || first_sector < 0 || last_sector >= rc->dh->sectors)
	Stop(_("Sectors must be in range [0..%lld].\n"),rc->dh->sectors-1);

      PrintLog(_("Limiting sector range to [%lld,%lld].\n"),first_sector,last_sector);
   }
   else 
   {  first_sector = 0; last_sector = rc->dh->sectors-1;
   }

   if(last_sector > last_sector_max) /* pathological case when medium has less sectors */
     last_sector = last_sector_max;  /* than recorded in the error correction file */

   start = first_sector;
   end   = last_sector;

   /*** Compare image and ecc fingerprints
        (only if the .ecc file is available) */

   if(rc->ei)
   {  //status = ReadSectors(rc->dh, buf+2048, FOOTPRINT_SECTOR, 1);
      status = ReadSectors(rc->dh, buf+2048, rc->ei->eh->fpSector, 1);

      if(status) 
      {  int answer;

	 answer = ModalWarning(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK_CANCEL, NULL,
			       _("Sector %d is missing. Can not compare image and ecc fingerprints.\n"
				 "Double check that image and ecc file belong together.\n"),
			       eh->fpSector);

	 if(!answer)
	 {  SetAdaptiveReadFootline(_("Aborted by user request!"), Closure->red);
 	    rc->earlyTermination = FALSE;
	    goto terminate;
	 }
      }
      else 
      {  struct MD5Context image_md5;

	 MD5Init(&image_md5);
	 MD5Update(&image_md5, (unsigned char*)buf+2048, 2048);
	 MD5Final(buf, &image_md5);

	 if(memcmp(buf, eh->mediumFP, 16))
	   Stop(_("Footprints of image and ecc file do not match.\n"
		  "Image and ecc file do not belong together.\n"));
      }
   }

   /*** Initialize ecc stripe bitmaps
        (only if the .ecc file is available) */

   if(rc->ei)
   {  rc->eccStripeLen = (rc->ei->sectors+eh->dataBytes-1)/eh->dataBytes;
      for(i=0; i<eh->dataBytes; i++)
	rc->eccStripe[i] = g_malloc0(4+rc->eccStripeLen/8);

      for(i=rc->ei->sectors%rc->eccStripeLen; i<rc->eccStripeLen; i++)
      {  int idx  = i/32;
	 guint32 mask = 1<<(i&31);
	 rc->eccStripe[eh->dataBytes-1][idx] |= mask;  /* mark the zero padding sectors */
      }
   }
   else rc->eccStripeLen = 1;  /* avoids division by zero */

#ifdef CHECK_VISITED
   if(!rc->ei)
   {  count = g_malloc((int)rc->dh->sectors+160);
      memset(count, 0, (int)rc->dh->sectors+160);
   }
   else
   {  count = g_malloc((int)rc->ei->sectors+160);
      memset(count, 0, (int)rc->ei->sectors+160);
   }
#endif

   /*** Initialize segment state counters (only in GUI mode) */

   if(Closure->guiMode)
   {  rc->sectorsPerSegment = 1 + (rc->dh->sectors / ADAPTIVE_READ_SPIRAL_SIZE);
      rc->segmentState = g_malloc0(ADAPTIVE_READ_SPIRAL_SIZE * sizeof(int));
      ClipReadAdaptiveSpiral(rc->dh->sectors/rc->sectorsPerSegment);
   }

   /*** Initialize the interval list */

   rc->intervals = g_malloc(8*sizeof(gint64));
   rc->max_intervals = 4; 
   rc->n_intervals = 0; 

   /*** See if we already have an image file. */

reopen_image:
   if(!LargeStat(Closure->imageName, &image_file_size))
   {  char *t = _("Reading new medium image.");

      /* Start with a fresh file. */

      if(!(rc->image = LargeOpen(Closure->imageName, O_RDWR | O_CREAT, IMG_PERMS)))
	Stop(_("Can't open %s:\n%s"),Closure->imageName,strerror(errno));

      PrintLog(_("Creating new %s image.\n"),Closure->imageName);
      if(Closure->guiMode)
	SetLabelText(GTK_LABEL(Closure->readAdaptiveHeadline),
		     "<big>%s</big>\n<i>%s</i>",t,rc->dh->mediumDescr);
   }
   else  /* examine the existing image file */
   {  char *t = _("Completing existing medium image.");
      int unknown_footprint = FALSE;
      int last_percent = 0;
      int tail_included = FALSE;
      gint64 first_missing, last_missing, current_missing;
      guint32 crc;

      /* Open the existing file. */

      if(!(rc->image = LargeOpen(Closure->imageName, O_RDWR, IMG_PERMS)))
	Stop(_("Can't open %s:\n%s"),Closure->imageName,strerror(errno));

      /* See if the media and image footprints match. */

      if(!LargeSeek(rc->image, (gint64)(2048*FOOTPRINT_SECTOR)))
	unknown_footprint = TRUE;
      else
      {  n = LargeRead(rc->image, buf, 2048);
	 status = ReadSectors(rc->dh, buf+2048, FOOTPRINT_SECTOR, 1);

	 if(n != 2048 || status || !memcmp(buf, Closure->deadSector, 2048))
	   unknown_footprint = TRUE;
      }

      if(!unknown_footprint && memcmp(buf, buf+2048, 2048))
      {  	  
	 if(!Closure->guiMode)
	   Stop(_("Image file does not match the CD/DVD."));
	 else
	 {  int answer = ModalDialog(GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK_CANCEL, NULL,
				     _("Image file already exists and does not match the CD/DVD.\n"
				       "The existing image file will be deleted."));
	   
	    if(!answer)
	    {  rc->earlyTermination = FALSE;
	       SetAdaptiveReadFootline(_("Reading aborted. Please select a different image file."), Closure->red);
	       goto terminate;
	    }
	    else
	    {  LargeClose(rc->image);
	       LargeUnlink(Closure->imageName);
	       goto reopen_image;
	    } 
	 }
      }

      if(Closure->guiMode)
	SetLabelText(GTK_LABEL(Closure->readAdaptiveHeadline),
		     "<big>%s</big>\n<i>%s</i>",t,rc->dh->mediumDescr);

      /*** Compare length of image and medium.
	   Still to do: image with byte sizes being not a multiple of 2048 */

      image_file_sectors = image_file_size / 2048;

      if(image_file_sectors > rc->dh->sectors)
      {  int answer;
	 
	 answer = ModalWarning(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK_CANCEL, NULL,
			       _("Image file is %lld sectors longer than inserted medium\n"
				 "(Image file: %lld sectors; medium: %lld sectors).\n"),
			       image_file_sectors-rc->dh->sectors, image_file_sectors, rc->dh->sectors);
	 max_sector = rc->dh->sectors;

	 if(!answer)
	 {  SetAdaptiveReadFootline(_("Aborted by user request!"), Closure->red);
 	    rc->earlyTermination = FALSE;
	    goto terminate;
	 }
      }
      else max_sector = image_file_sectors;

      /*** Build the interval list */

      /* Position ecc file behind the header,
	 initialize CRC buffer pointers if the .ecc file is available. */

      if(rc->ei)
      {  if(!LargeSeek(rc->ei->file, (gint64)sizeof(EccHeader)))
	    Stop(_("Failed skipping the ecc header: %s"),strerror(errno));

	 crcidx = CRCBUFSIZE;
      }

      LargeSeek(rc->image, 0);            /* rewind image file */   
      
      first_missing = last_missing = -1;

      /* Go through all sectors in the image file.
	 Check them for "dead sector markers" and checksum failures. */
   
      if(Closure->guiMode)
	SetAdaptiveReadSubtitle(_("Analysing existing image file"));

      for(s=0; s<max_sector; s++)
      {  int ecc_stripe = s / rc->eccStripeLen;
	 int stripe_idx = s % rc->eccStripeLen;
	 int idx = stripe_idx / 32;
	 guint32 mask = 1<<(stripe_idx & 31);
         int n,percent;

	 /* Check for user interruption. */

	 if(Closure->stopActions)   
	 {  SetAdaptiveReadFootline(_("Aborted by user request!"), Closure->red);
 	    rc->earlyTermination = FALSE;
	    goto terminate;
	 }

	 /* Read the next sector */

	 n = LargeRead(rc->image, buf, 2048);
	 if(n != 2048) /* && (s != rc->dh->sectors - 1 || n != ii->inLast)) */
	   Stop(_("premature end in image (only %d bytes): %s\n"),n,strerror(errno));

	 /* Look for the dead sector marker */

	 current_missing = !memcmp(buf, Closure->deadSector, n);

	 if(current_missing)
	   mark_sector(rc, s, Closure->red);

	 /* If the CRC buf is exhausted, refill it. Then compare checksums.
	    (only if the .ecc file is available) */

	 if(rc->ei)
	 {  if(crcidx >= CRCBUFSIZE)
	    {  size_t remain = max_sector-s;
	       size_t size;

	       if(remain < CRCBUFSIZE)
	            size = remain*sizeof(guint32);
	       else size = CRCBUFSIZE*sizeof(guint32);

	       if(LargeRead(rc->ei->file, crcbuf, size) != size)
		 Stop(_("Error reading CRC information: %s"),strerror(errno));

	       crcidx = 0;
	    }

	   crc = Crc32(buf, 2048); 
	   if(crc != crcbuf[crcidx++] && !current_missing)
	   {  current_missing = 1;
	      mark_sector(rc, s, Closure->yellow);
	   }
	 }

	 if(current_missing)  /* Remember defect sector in current interval */
	 {  if(first_missing < 0) first_missing = s;
	    last_missing = s;
	 }
	 else                 /* Remember good sector in eccStripe bitmap */
	 {  
	    rc->readable++;
	    if(rc->ei)
	      rc->eccStripe[ecc_stripe][idx] |= mask;

	    mark_sector(rc, s, Closure->green);

#ifdef CHECK_VISITED
	    count[s]++;
#endif
	 }

	 /* Determine end of current interval and write it out */

	 if((!current_missing || s>=max_sector-1) && first_missing>=0)
	 {  if(s>=max_sector-1)  /* special case: interval end = image end */
	    {  last_missing = last_sector;  /* may happen when image is truncated */
	       tail_included = TRUE;
	    }

	    if(first_missing < first_sector)  /* user may limit the read range */
	      first_missing = first_sector;
	    if(last_missing > last_sector)
	      last_missing = last_sector;

	    if(first_missing <= last_missing)
	      add_interval(rc, first_missing, last_missing-first_missing+1);

	    first_missing = -1;
	 }

	 /* Visualize the progress */

	 percent = (100*s)/max_sector;
	 if(last_percent != percent) 
	 {  if(!Closure->guiMode)
	      PrintProgress(_("Analysing existing image file: %2d%%"),percent);
#if 0
	   else UpdateAdaptiveResults(rc->readable, rc->correctable, 
				      rc->dh->sectors-rc->readable,
				      (int)((1000LL*(rc->readable+rc->correctable))/rc->dh->sectors));
#endif

	   last_percent = percent;
	 }
      }

      /* If the image is shorter than the medium and the missing part
	 was not already included in the last interval,
	 insert another interval for the missing portion. */

      if(s<last_sector && !tail_included)  /* truncated image? */
	add_interval(rc, s, last_sector-s+1);

      /*** Now that all readable sectors are known,
	   determine those which can already be corrected.
           (only if the .ecc file is available) */

      if(Closure->guiMode)
	SetAdaptiveReadSubtitle(_("Determining correctable sectors"));

      if(rc->ei)
      {  for(s=0; s<rc->dh->sectors; s++)
	 {  int stripe_idx = s % rc->eccStripeLen;
	    int idx = stripe_idx / 32;
	    guint32 mask = 1<<(stripe_idx & 31);
	    int j,present=0;

	    for(j=0; j<eh->dataBytes; j++)            /* Count available sectors */
	      if(rc->eccStripe[j][idx] & mask)
		present++;

	    if(eh->dataBytes-present <= eh->eccBytes) /* remaining sectors are correctable */
	      for(j=0; j<eh->dataBytes; j++)          /* mark them as visited */
		if(!(rc->eccStripe[j][idx] & mask))
		  {  rc->eccStripe[j][idx] |= mask;
		    rc->correctable++;
#ifdef CHECK_VISITED
		    count[rc->eccStripeLen*j+stripe_idx]++;
#endif
		  }
	 }
      }

      /*** Tell user results of image file analysis */

      if(rc->ei)
	   PrintLog(_("Analysing existing image file: %lld readable, %lld correctable, %lld still missing.\n"),
		     rc->readable, rc->correctable, rc->dh->sectors-rc->readable-rc->correctable);
      else PrintLog(_("Analysing existing image file: %lld readable, %lld still missing.\n"),
		     rc->readable, rc->dh->sectors-rc->readable-rc->correctable);

      if(Closure->guiMode)
	UpdateAdaptiveResults(rc->readable, rc->correctable, 
			      rc->dh->sectors-rc->readable-rc->correctable,
			      (int)((1000LL*(rc->readable+rc->correctable))/rc->dh->sectors));

      /*** Already enough information available? */
	 
      if(rc->readable + rc->correctable >= rc->dh->sectors)
      {  char *t = _("\nSufficient data for reconstructing the image is available.\n");
	 PrintLog(t);
	 if(Closure->guiMode)
	   SetAdaptiveReadFootline(t, Closure->darkgreen);
	 goto finished;
      }

      /*** Nope, begin with first interval */

      if(!rc->n_intervals)  /* may happen when reading range is restricted too much */
	goto finished;

      start = rc->intervals[0];
      size  = rc->intervals[1];
      end = start+size-1;
      pop_interval(rc);
   }

   rc->interval_size  = end-start+1;
   rc->interval_start = start;

   /*** Read the medium image. */

   if(Closure->guiMode)
     SetAdaptiveReadSubtitle(rc->subtitle);

   for(;;)
   {  
      /*** If we jumped beyond the max_sector, fill the gap with dead sector markers. */

      if(start > max_sector+1)
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

	 if(first_sector==0 || max_sector > 0)
	   max_sector++;

         t = g_strdup_printf(_("Filling image area [%lld..%lld]"), max_sector, start-1);
	 clear_progress(rc);
	 if(Closure->guiMode)
	 {  SetAdaptiveReadSubtitle(t);
	    ChangeSpiralCursor(-1); 
	 }
         PrintCLI(t);
	 g_free(t);

	 if(!LargeSeek(rc->image, (gint64)(2048*max_sector)))
	   Stop(_("Failed seeking to sector %lld in image [%s]: %s"),
		max_sector, "fill", strerror(errno));

	 for(i=max_sector, j=0; i<start; i++)
	 {  n = LargeWrite(rc->image, Closure->deadSector, 2048);
	    if(n != 2048)
	      Stop(_("Failed writing to sector %lld in image [%s]: %s"),
		   max_sector, "fill", strerror(errno));
	     
	    if(Closure->stopActions)   /* somebody hit the Stop button */
	    {  if(Closure->guiMode)
		 SetAdaptiveReadFootline(_("Aborted by user request!"), Closure->red);

	       rc->earlyTermination = FALSE;  /* suppress respective error message */
	       goto terminate;
	    }

	    if(j++ % 2000)
	    {  int seq = (j/2000)%10;

	       PrintCLI(anim[seq]);
	    }
	    
	    if(Closure->guiMode)
	    {  int segment = i / rc->sectorsPerSegment;

 	       if(Closure->readAdaptiveSpiral->segmentColor[segment] == Closure->background)
		 ChangeSegmentColor(Closure->white, segment);
	    }
	 }

	 PrintCLI("               \n");
	 max_sector = start-1;

	 if(Closure->guiMode)
	 {  RemoveFillMarkers();
	    SetAdaptiveReadSubtitle(rc->subtitle);
	 }
      }

      /*** Try reading the next interval */

      print_progress(rc, TRUE);
      crcidx = CRCBUFSIZE;

      for(s=start; s<=end; )
      {  int nsectors,cnt;
	 int ignore_fatal = 0;
 
	 if(Closure->stopActions)   /* somebody hit the Stop button */
	 {  if(Closure->guiMode)
	       SetAdaptiveReadFootline(_("Aborted by user request!"), Closure->red);

	    rc->earlyTermination = FALSE;  /* suppress respective error message */
	    goto terminate;
	 }

	 if(Closure->guiMode)
	   ChangeSpiralCursor(s / rc->sectorsPerSegment);
	    
	 /*** Try reading the next <nsectors> sector(s). */

	 if(s & 15)
               nsectors = 1;
	 else  nsectors = 16;

	 if(s+nsectors > end) nsectors = end-s+1;

	 if(rc->ei)   /* skip sectors which can be calculated from .ecc file */
	 {  for(i=cnt=0; i<nsectors; i++)  /* sectors already present? */
	    {  int ecc_stripe = (s+i) / rc->eccStripeLen;
	       int stripe_idx = (s+i) % rc->eccStripeLen; 
	       int idx = stripe_idx / 32;
	       guint32 mask = 1<<(stripe_idx & 31);

	       if(rc->eccStripe[ecc_stripe][idx] & mask)
		 cnt++;
	    }

	    if(cnt) /* switch down to 1 sector per read to avoid double reads */ 
	    {  int ecc_stripe = s / rc->eccStripeLen;
	       int stripe_idx = s % rc->eccStripeLen; 
	       int idx = stripe_idx / 32;
	       guint32 mask = 1<<(stripe_idx & 31);

	       nsectors = 1;

	       if(rc->eccStripe[ecc_stripe][idx] & mask)
	       {  s++;
		  if(rc->ei) crcidx++;
		  continue;
	       }
	    }
	 }

	 status = ReadSectors(rc->dh, buf, s, nsectors);

	 /* Medium Error (3) and Illegal Request (5) may result from 
	    a medium read problem, but other errors are regarded as fatal. */

	 if(status && !ignore_fatal
	    && rc->dh->sense.sense_key && rc->dh->sense.sense_key != 3 && rc->dh->sense.sense_key != 5)
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

	 /*** Reading was successful */

	 if(!status)   
	 {  gint64 b;

	    /* Store sector(s) in the image file */

	    if(!LargeSeek(rc->image, (gint64)(2048*s)))
	      Stop(_("Failed seeking to sector %lld in image [%s]: %s"),
		   s,"store",strerror(errno));

	    for(i=0, b=s; i<nsectors; i++,b++)
	    {  int ecc_stripe = b / rc->eccStripeLen;
	       int stripe_idx = b % rc->eccStripeLen;
	       int idx = stripe_idx / 32;
	       guint32 mask = 1<<(stripe_idx & 31);
	       guint32 crc;

	       if(rc->ei)  /* we have crc information available */
	       { 
		 /* If the CRC buf is exhausted, refill it. */

		 if(crcidx >= CRCBUFSIZE)  
		 {  size_t remain = rc->ei->sectors-b;
		    size_t size;

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

		 crc = Crc32(buf+i*2048, 2048); 

		 if(crc != crcbuf[crcidx++])
		 {  PrintCLI("\n");
		    PrintCLI(_("CRC error in sector %lld\n"),b);
		    print_progress(rc, TRUE);

		    n = LargeWrite(rc->image, Closure->deadSector, 2048);
		    if(n != 2048)
		      Stop(_("Failed writing to sector %lld in image [%s]: %s"),
			   s, "unv", strerror(errno));

		    mark_sector(rc, b, Closure->yellow);
		 }
		 else 
		 {  n = LargeWrite(rc->image, buf+i*2048, 2048);
		    if(n != 2048)
		      Stop(_("Failed writing to sector %lld in image [%s]: %s"),
			   s, "store", strerror(errno));

		    rc->eccStripe[ecc_stripe][idx] |= mask;
		    rc->readable++;

		    mark_sector(rc, b, Closure->green);
		 }
	       }

	       else /* no crc information available */
	       {  n = LargeWrite(rc->image, buf+i*2048, 2048);
		  if(n != 2048)
		    Stop(_("Failed writing to sector %lld in image [%s]: %s"),
			 s, "store", strerror(errno));

		  rc->readable++;

		  mark_sector(rc, b, Closure->green);
	       }
	    }

	    /* See if additional sectors become correctable */
	    
	    if(rc->ei)
	    {  for(i=0; i<nsectors; i++) 
	       {  //int ecc_stripe = (s+i) / rc->eccStripeLen;
		  int stripe_idx = (s+i) % rc->eccStripeLen;
		  int idx = stripe_idx / 32;
		  guint32 mask = 1<<(stripe_idx & 31);

		  int j,present=0;

#ifdef CHECK_VISITED
		  count[s+i]++;
#endif

		  for(j=0; j<eh->dataBytes; j++) /* Count available sectors */
		    if(rc->eccStripe[j][idx] & mask)
		      present++;

		  if(eh->dataBytes-present <= eh->eccBytes) /* remaining sectors are correctable */
		    for(j=0; j<eh->dataBytes; j++)          /* mark them as visited */
		      if(!(rc->eccStripe[j][idx] & mask))
		      {  rc->eccStripe[j][idx] |= mask;
			 rc->correctable++;
			 
			 mark_sector(rc, rc->eccStripeLen*j+stripe_idx, Closure->green);
#ifdef CHECK_VISITED
			 count[rc->eccStripeLen*j+stripe_idx]++;
#endif
		      }
	       }
	    }

	    s+=nsectors;
	    if(s>max_sector) max_sector=s-1;

	    if(rc->readable + rc->correctable >= rc->dh->sectors)
	    {  char *t = _("\nSufficient data for reconstructing the image is available.\n");

	       print_progress(rc, TRUE);
	       PrintLog(t);
	       if(Closure->guiMode && rc->ei)
		 SetAdaptiveReadFootline(t, Closure->black);
	       goto finished;
	    }
	 }  /* end of if(!status) */

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

	    if(max_sector < s+nsectors)
	      max_sector = s+nsectors-1;

	    /* find out how to continue */

	    if(s+nsectors-1 >= end)  /* Interval used up */
	    {  
#ifdef VERBOSE
	       printf("... Interval [%lld..%lld] used up\n", start, end);
#endif
	    }
	    else                   /* Insert rest of interval into queue */
	    {  
	       start = s+nsectors;
	       size  = end-start+1;

#ifdef VERBOSE
	       printf("... Interval %lld [%lld..%lld] added\n", size, start, start+size-1);
#endif
	       add_interval(rc, start, size);

#ifdef VERBOSE
	       print_intervals(rc);
#endif
	    }
	    break;
	 }

	 print_progress(rc, FALSE);
      }

      if(s>=end)
      {  print_progress(rc, TRUE);
	 PrintCLI("\n");
      }

      /* Pop and prepare next interval */

      if(rc->n_intervals <= 0)
	goto finished;

      start = rc->intervals[0];
      size  = rc->intervals[1];
      pop_interval(rc);

      if(size>1)
      {  
#ifdef VERBOSE
         printf("*** Splitting [%lld..%lld]\n",start,start+size-1);
#endif
	 add_interval(rc, start, size/2);
	 end = start+size-1;
	 start = start+size/2;
      }
      else 
      {  
	 end = start;
#ifdef VERBOSE
         printf("*** Popped [%lld]\n",start);
#endif
      }

#ifdef VERBOSE
      print_intervals(rc);
#endif

      rc->interval_size  = end-start+1;
      rc->interval_start = start;
      if(!rc->ei && rc->interval_size < Closure->sectorSkip)
	 goto finished;
   }

finished:

#ifdef CHECK_VISITED
   {  int i,cnt=0;
      for(i=0; i<(int)rc->dh->sectors; i++)
      {  cnt+=count[i];
         if(count[i] != 1)
           printf("Sector %d: %d\n",i,count[i]);
      }

      printf("\nTotal visited %d (%d)\n",cnt,i);

      for(i=(int)rc->dh->sectors; i<(int)rc->dh->sectors+160; i++)
        if(count[i] != 0)
          printf("SECTOR %d: %d\n",i,count[i]);

      g_free(count);
   }
#endif

   if(Closure->guiMode)
   {  ChangeSpiralCursor(-1);
      mark_sector(rc, 0, NULL);  /* force output of final results */
   }

   if(rc->ei && (rc->readable + rc->correctable < rc->dh->sectors))
   {  int total = rc->readable+rc->correctable;
      int percent = (int)((1000LL*(long long)total)/rc->dh->sectors);
      char *t = g_strdup_printf(_("Only %2d.%1d%% of the image are readable or correctable"),
				  percent/10, percent%10); 

      PrintLog(_("\n%s\n"
		  "(%lld readable,  %lld correctable,  %lld still missing).\n"),
		t, rc->readable, rc->correctable, rc->dh->sectors-total);
      if(Closure->guiMode)
	 SetAdaptiveReadFootline(t, Closure->black);

      g_free(t);
      exitCode = EXIT_FAILURE;
   }

   if(!rc->ei)
   {  if(rc->readable == rc->dh->sectors)
      {  char *t = _("\nGood! All sectors have been read.\n"); 
	 PrintLog(t);
	 if(Closure->guiMode)
	   SetAdaptiveReadFootline(t, Closure->black);
      }
      else
      {  int percent = (int)((1000LL*rc->readable)/rc->dh->sectors);
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

   /*** Close and clean up */

   rc->earlyTermination = FALSE;

terminate:
   cleanup((gpointer)rc);
}

