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

//#define DEBUG 1

/***
 *** Local data package used during reading 
 ***/

typedef struct
{  LargeFile *image;
   struct _DeviceHandle *dh;
   EccInfo *ei;
   char *msg;
   unsigned char *bufbase;
   GTimer *speedTimer,*readTimer;
   int unreportedError;
   int earlyTermination;
   int scanMode;
   gint64 readOK;
} read_closure;

static void cleanup(gpointer data)
{  read_closure *rc = (read_closure*)data;
   int full_read = FALSE;
   int aborted   = rc->earlyTermination;
   int scan_mode = rc->scanMode;

   if(rc->dh)
     full_read = (rc->readOK == rc->dh->sectors && !Closure->crcErrors);

   Closure->cleanupProc = NULL;

   if(Closure->guiMode)
   {  if(rc->unreportedError)
         SwitchAndSetFootline(Closure->readLinearNotebook, 1, Closure->readLinearFootline, 
			      _("<span color=\"red\">Aborted by unrecoverable error.</span> %lld sectors read, %lld sectors unreadable/skipped so far."),rc->readOK,Closure->readErrors); 
   }

   if(rc->image)   
     if(!LargeClose(rc->image))
       Stop(_("Error closing image file:\n%s"), strerror(errno));
   if(rc->dh)      CloseDevice(rc->dh);
   if(rc->ei)      FreeEccInfo(rc->ei);
   if(rc->msg)     g_free(rc->msg);
   if(rc->bufbase) g_free(rc->bufbase);
   if(rc->speedTimer) g_timer_destroy(rc->speedTimer);
   if(rc->readTimer)  g_timer_destroy(rc->readTimer);
   g_free(rc);

   if(Closure->readAndCreate && Closure->guiMode && !scan_mode && !aborted)
   {  if(!full_read)
      {  ModalDialog(GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, NULL,
		     _("Automatic error correction file creation\n"
		       "is only possible after a full reading pass.\n"));
	 AllowActions(TRUE);
      }
      else ContinueWithAction(ACTION_CREATE_CONT); 
   }
   else 
     if(Closure->guiMode)
       AllowActions(TRUE);

   if(!full_read && Closure->crcCache)
     ClearCrcCache();

   if(scan_mode)   /* we haven't created an image, so throw away the crc sums */
     ClearCrcCache();

   g_thread_exit(0);
}

/***
 *** Try reading the medium and create the image and map.
 ***/

static void insert_buttons(GtkDialog *dialog)
{  
  gtk_dialog_add_buttons(dialog, 
			 _("Ignore once"), 1,
			 _("Ignore always"), 2,
			 _("Abort"), 0, NULL);
} 

void ReadMediumLinear(gpointer data)
{  read_closure *rc = g_malloc0(sizeof(read_closure));
   int scan_mode = GPOINTER_TO_INT(data);
   int nsectors; 
   gint64 start,end,s,read_marker;
   gint64 image_size;
   int percent, last_percent;
   double last_speed = -1.0;
   int first_speed_value = TRUE;
   int rereading = 0;
   gint previous_read_errors = 0;
   gint previous_crc_errors = 0;
   unsigned char *buf = NULL;
   char *t;
   int status,n;
   gint64 sectors,last_read_ok,last_errors_printed;
   double speed = 0.0;
   int tao_tail = 0;
   int ignore_fatal = FALSE;
   struct MD5Context md5ctxt;

   Closure->additionalSpiralColor = -1;

   /*** Register the cleanup procedure so that Stop() can abort us properly. */

   rc->unreportedError  = TRUE;
   rc->earlyTermination = TRUE;
   rc->scanMode = scan_mode;

   if(scan_mode)  /* Output messages differ in read and scan mode */
   {  RegisterCleanup(_("Scanning aborted"), cleanup, rc);
      if(Closure->guiMode)
	SetLabelText(GTK_LABEL(Closure->readLinearHeadline), "<big>%s</big>\n<i>%s</i>",
		     _("Scanning medium for read errors."),
		     _("Medium: not yet determined"));
   }
   else
   {  RegisterCleanup(_("Reading aborted"), cleanup, rc);
      if(Closure->guiMode)
       SetLabelText(GTK_LABEL(Closure->readLinearHeadline), "<big>%s</big>\n<i>%s</i>",
		    _("Preparing for reading the medium image."),
		    _("Medium: not yet determined"));
   }

   if(Closure->readAndCreate && !scan_mode)
   {  gint64 ignore;
      if(LargeStat(Closure->eccName, &ignore))
      { if(Closure->guiMode)
	{  int answer = ModalDialog(GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK_CANCEL, NULL,
				    _("Automatic error correction file creation is enabled,\n"
				      "and \"%s\" already exists.\n"
				      "Overwrite it?\n"),
				    Closure->eccName);

	  if(!answer)
	  {  SwitchAndSetFootline(Closure->readLinearNotebook, 1, Closure->readLinearFootline, 
				_("<span color=\"red\">Aborted by user request!</span>")); 
	     rc->unreportedError = FALSE;
	     goto terminate;
	  }
	}
      }
   }

   /*** Timer setup */

   rc->speedTimer = g_timer_new();
   rc->readTimer  = g_timer_new();

   /*** Align the buffer at a 4096 boundary.
	Might be needed by some SCSI drivers. */

   rc->bufbase = g_malloc(32768 + 4096);
   buf = rc->bufbase + (4096 - ((unsigned long)rc->bufbase & 4095));


   /*** Open Device and query medium properties */

   rc->dh = OpenAndQueryDevice(Closure->device);
   sectors = rc->dh->sectors;
   Closure->readErrors = Closure->crcErrors = rc->readOK = 0;

   /*** See if the current ecc file belongs to the medium */

   rc->ei = OpenEccFile(READABLE_ECC | PRINT_MODE);
   if(rc->ei) /* Compare the footprint sectors */
   {  struct MD5Context md5ctxt;
      guint8 footprint[16];

      status = ReadSectors(rc->dh, buf, rc->ei->eh->fpSector, 1);

      MD5Init(&md5ctxt);
      MD5Update(&md5ctxt, buf, 2048);
      MD5Final(footprint, &md5ctxt);

      if(!status && !memcmp(footprint, rc->ei->eh->mediumFP, 16))
	Closure->checkCrc = TRUE;
   }

   /*** See if user wants to limit the read range. */

   if(Closure->readStart || Closure->readEnd)
   {  if(!Closure->guiMode)
      {  start = Closure->readStart;
         end   = Closure->readEnd < 0 ? sectors-1 : Closure->readEnd;
      }
      else  /* be more permissive in GUI mode */
      {  start = 0;
 	 end   = sectors-1;

	 if(Closure->readStart <= Closure->readEnd)
	 {  start = Closure->readStart < sectors ? Closure->readStart : sectors-1;
	    end   = Closure->readEnd   < sectors ? Closure->readEnd   : sectors-1;
	 }
      }

      if(start>end || start < 0 || end >= sectors)
	Stop(_("Sectors must be in range [0..%lld].\n"),sectors-1);

      PrintLog(_("Limiting sector range to [%lld,%lld].\n"),start,end);
   }
   else 
   {  start = 0; end = sectors-1;
   }

   /*** See if we already have an image file. */

   if(scan_mode)  /* don't care if in scan mode */
   {  
      rc->msg = g_strdup(_("Scanning medium for read errors."));

      PrintLog("%s\n", rc->msg);
      if(Closure->guiMode)
      {  if(Closure->checkCrc)
	   SetLabelText(GTK_LABEL(Closure->readLinearHeadline), 
			"<big>%s</big>\n<i>- %s -</i>", rc->msg,
			_("Reading CRC information from ecc file"));

         else
	   SetLabelText(GTK_LABEL(Closure->readLinearHeadline), 
			"<big>%s</big>\n<i>%s</i>", rc->msg, rc->dh->mediumDescr);
      }
      read_marker = 0;
   } 
   else          
   { 
reopen_image:
     if(!LargeStat(Closure->imageName, &image_size))
     {  
        rc->msg = g_strdup(_("Reading new medium image."));

        /* Start with a fresh file. */

	if(!(rc->image = LargeOpen(Closure->imageName, O_RDWR | O_CREAT, IMG_PERMS)))
	  Stop(_("Can't open %s:\n%s"),Closure->imageName,strerror(errno));

        PrintLog(_("Creating new %s image.\n"),Closure->imageName);
	if(Closure->guiMode)
	{  if(Closure->checkCrc)
	     SetLabelText(GTK_LABEL(Closure->readLinearHeadline),
			  "<big>%s</big>\n<i>%s</i>", rc->msg,
			  _("Reading CRC information from ecc file"));
	   else
	     SetLabelText(GTK_LABEL(Closure->readLinearHeadline),
			  "<big>%s</big>\n<i>%s</i>", rc->msg, rc->dh->mediumDescr);
	}
        rereading = 0;
	read_marker = 0;
     }
     else
     {  char *t = _("Completing existing medium image.");
        int unknown_footprint = FALSE;

        /* Use the existing file as a starting point.
	   Set the read_marker at the end of the file
	   so that the reader looks for "dead_sector" markers
	   and skips already read blocks. */

        if(!(rc->image = LargeOpen(Closure->imageName, O_RDWR, IMG_PERMS)))
  	  Stop(_("Can't open %s:\n%s"),Closure->imageName,strerror(errno));

        rereading = 1;
	read_marker = image_size / 2048;

	/* See if the media and image footprints match */

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
	      {  rc->unreportedError = FALSE;
		 SwitchAndSetFootline(Closure->readLinearNotebook, 1, Closure->readLinearFootline, 
				      _("<span color=\"red\">Reading aborted.</span> Please select a different image file.")); 
		 goto terminate;
	      }
	      else
	      {  LargeClose(rc->image);
		 LargeUnlink(Closure->imageName);
		 goto reopen_image;
	      } 
	   }
	}

	/* If the image is not complete yet, first aim to read the
	   unvisited sectors before trying to re-read the missing ones. */

	Closure->checkCrc = 0; /* makes only sense if image is completely read */

	if(!Closure->readStart && !Closure->readEnd && read_marker < sectors-1)
	{  PrintLog(_("Completing image %s. Continuing with sector %lld.\n"),
	            Closure->imageName, read_marker);
	   start = read_marker;
	   Closure->additionalSpiralColor = 0;  /* blue */
	}
	else 
	{  PrintLog(_("Completing image %s. Only missing sectors will be read.\n"), Closure->imageName);
	   Closure->additionalSpiralColor = 3;  /* dark green*/
	}
	   
	if(Closure->guiMode)
	  SetLabelText(GTK_LABEL(Closure->readLinearHeadline),
		       "<big>%s</big>\n<i>%s</i>",t,rc->dh->mediumDescr);
     }
   }

   if(Closure->guiMode)
     InitializeCurve(rc->dh->maxRate, start, end, sectors);

   /*** If start > read_marker, fill the gap with dead sector markers. */

   if(!scan_mode && start > read_marker)
   {  s = read_marker;

      if(!LargeSeek(rc->image, (gint64)(2048*s)))
	Stop(_("Failed seeking to sector %lld in image [%s]: %s"),
	     s, "fill", strerror(errno));

      while(s++ < start)
      {  n = LargeWrite(rc->image, Closure->deadSector, 2048);
	 if(n != 2048)
	   Stop(_("Failed writing to sector %lld in image [%s]: %s"),
		s, "fill", strerror(errno));
      }
   }

   /*** Memory for the CRC32 sums is needed in two cases: */

   /* a) a full image read is attempted, and the image CRC32 
         and md5sum are calculated on the fly. */

   if(   !scan_mode && !rereading && start == 0 && end == sectors-1)
   {  Closure->crcCache = g_try_malloc(sizeof(guint32) * sectors);

      if(Closure->crcCache)
	Closure->crcImageName = g_strdup(Closure->imageName);

      MD5Init(&md5ctxt);
   }

   /* b) we have a suitable ecc file and compare CRC32sum against
         it while reading */

   if(Closure->checkCrc)
   {  gint64 crc_sectors = rc->ei->sectors;

      if(sectors > crc_sectors)  /* Allows completion of crc info */
	crc_sectors = sectors;   /* when image contains additional sectors */

      Closure->crcCache = g_try_malloc(sizeof(guint32) * crc_sectors);

      if(!Closure->crcCache)
      {  Closure->checkCrc = FALSE;
      }
      else
      {  guint32 *cache = Closure->crcCache;
         int i=0;

	 if(!LargeSeek(rc->ei->file, (gint64)sizeof(EccHeader)))
	   Stop(_("Failed skipping the ecc header: %s"),strerror(errno));

	 PrintCLI("%s ...",_("Reading CRC information from ecc file"));

	 while(i<crc_sectors)
	 {  int n = i+512<crc_sectors ? 512 : crc_sectors - i;

	    if(LargeRead(rc->ei->file, cache, 4*n) != 4*n)
	      Stop(_("Error reading CRC information: %s"), strerror(errno));

	    cache += n;
	    i+=n;
	 }

         PrintCLI(_("done.\n"));

	 if(Closure->guiMode)
	   SetLabelText(GTK_LABEL(Closure->readLinearHeadline),
			"<big>%s</big>\n<i>%s</i>", rc->msg, rc->dh->mediumDescr);

	 MD5Init(&md5ctxt);
      }
   }

   /*** Prepare the speed timing */

   if(Closure->guiMode && Closure->spinupDelay)
     SwitchAndSetFootline(Closure->readLinearNotebook, 1, Closure->readLinearFootline,
			  _("Waiting %d seconds for drive to spin up...\n"), Closure->spinupDelay);

   SpinupDevice(rc->dh);

   if(Closure->guiMode && Closure->spinupDelay)
     SwitchAndSetFootline(Closure->readLinearNotebook, 0, Closure->readLinearFootline, "ignore");

   ReadSectors(rc->dh, buf, start, 1); /* eliminate initial seek time from timing */
   g_timer_start(rc->speedTimer);
   g_timer_start(rc->readTimer);

   /*** Read the medium image. */

   s = start;
   last_percent = (1000*s)/sectors;
   last_read_ok = last_errors_printed = 0;

   while(s<=end)
   {  if(Closure->stopActions)   /* somebody hit the Stop button */
      {
	SwitchAndSetFootline(Closure->readLinearNotebook, 1, Closure->readLinearFootline, 
			     _("<span color=\"red\">Aborted by user request!</span> %lld sectors read, %lld sectors unreadable/skipped so far."),
			     rc->readOK,Closure->readErrors); 
	rc->unreportedError = FALSE;  /* suppress respective error message */
        goto terminate;
      }

      /*** Decide between reading in fast mode (16 sectors at once)
	   or reading one sector at a time.
	   Fast mode gains some reading speed due to transfering fewer
	   but larger data blocks from the device.
	   Switching to fast mode is done only on 32K boundaries
	   -- this matches the internal DVD structure better. 
           In order to treat the 2 read errors at the end of TAO discs correctly,
           we switch back to per sector reading at the end of the medium. */

      if(s & 15 || s >= ((sectors - 2) & ~15) ) /* - 16 removed */ 
            nsectors = 1;
      else  nsectors = 16;

      if(s+nsectors > end)    /* don't read past the (CD) media end */
	nsectors = end-s+1;

      /*** If s is lower than the read marker,
	   check if the sector has already been read
	   in a previous session. */

reread:
      if(!scan_mode && s < read_marker)
      {  int i,ok = 0;
	 int num_compare = nsectors;

	 if(!LargeSeek(rc->image, (gint64)(2048*s)))
	   Stop(_("Failed seeking to sector %lld in image [%s]: %s"),
		s, "reread", strerror(errno));

	 if(s+nsectors > read_marker)
	   num_compare = read_marker-s;

	 for(i=0; i<num_compare; i++)
	 {  n = LargeRead(rc->image, buf, 2048);

	    if(n != 2048)
	      Stop(_("unexpected read error in image for sector %lld"),s);

	    if(memcmp(buf, Closure->deadSector, n))
	      ok++;
	 }

	 if(ok == nsectors)
	 {
#ifdef DEBUG
printf("Sector %lld: %d sectors already present.\n", s, nsectors);
#endif
	   goto step_counter;
	 }
	 else
	 {
#ifdef DEBUG
printf("Sector %lld: %d sectors still missing.\n", s, nsectors-ok);
#endif
           if(nsectors > 1 && ok > 0)
	   {  nsectors = 1;
	      goto reread;
	   }
	 }
      }

#ifdef DEBUG
printf("Sector %lld: reading %2d sectors\n",s,nsectors);
#endif
      /*** Try reading the next <nsectors> sector(s).
      	   Medium Error (3) and Illegal Request (5) may result from 
	   a medium read problem, but other errors are regarded as fatal. */

      status = ReadSectors(rc->dh, buf, s, nsectors);

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
	 {  SwitchAndSetFootline(Closure->readLinearNotebook, 1, Closure->readLinearFootline, 
				_("<span color=\"red\">Aborted by user request!</span> %lld sectors read, %lld sectors unreadable/skipped so far."),
				 rc->readOK,Closure->readErrors); 
	    rc->unreportedError = FALSE;  /* suppress respective error message */
	    goto terminate;
	 }
      }

      /*** Process the read error. */

      if(status)
      {  int nfill;

	 /* Disable on the fly checksum calculation */

	 if(Closure->crcCache && !Closure->checkCrc)
	   ClearCrcCache();

	 /* Determine number of sectors to skip forward. 
	    Make sure not to skip past the media end
	    and to land at a multiple of 16. */

	 if(nsectors>=Closure->sectorSkip) nfill = nsectors;
	 else
	 {  if(s+Closure->sectorSkip > end) nfill = end-s+1;
	    else nfill = Closure->sectorSkip - ((s + Closure->sectorSkip) & 15);
	 }

	 /* If we are reading past the read marker we must take care 
            to fill up any holes with dead sector markers before skipping forward. */

	 if(!scan_mode && s+nsectors >= read_marker)
	 {  int i;

	    /* Write nfill of "dead sector" markers so that
	       they are tried again in the following iterations / sessions. */
#ifdef DEBUG
printf("Sector %lld: filling %d sectors\n",s,nfill);
#endif
	    if(!LargeSeek(rc->image, (gint64)(2048*s)))
	      Stop(_("Failed seeking to sector %lld in image [%s]: %s"),
		   s, "nfill", strerror(errno));

	    for(i=0; i<nfill; i++)
	    {  n = LargeWrite(rc->image, Closure->deadSector, 2048);
	       if(n != 2048)
		 Stop(_("Failed writing to sector %lld in image [%s]: %s"),
		      s, "nfill", strerror(errno));
	    }
	 }
	 
	 /* If sectorSkip is set, perform the skip.
	    nfill has been calculated so that the skip lands
	    at a multiple of 16. Therefore nsectors can remain
	    at its former value as skipping forward will not 
	    destroy 16 sector aligned reads. 
	    The nsectors>1 case can only happen when processing the tao_tail. */

	 if(Closure->sectorSkip && nsectors > 1)
	 {  PrintCLIorLabel(Closure->status,
			    _("Sector %lld: %s Skipping %d sectors.\n"),
			    s, GetLastSenseString(FALSE), nfill-1);  
	    Closure->readErrors+=nfill; 
	    s+=nfill-nsectors;   /* nsectors will be added again after the goto */
	    goto step_counter;
	 }

	 /* However, if no sector skipping is requested
	    and we are currently in fast read mode (nsectors > 1),
	    slow down to reading 1 sectors at once
	    and try to re-read the first sector. */

	 else 
	 {
	    if(nsectors > 1)
	    {  nsectors = 1;
	       goto reread;
	    }
	    else 
	    {  PrintCLIorLabel(Closure->status,
			       _("Sector %lld: %s\n"),
			       s, GetLastSenseString(FALSE));  
	       if(s >= sectors - 2) tao_tail++;
	       Closure->readErrors++;
	    }
	 }
      }
      else
      { int i;
	/*** Store sector(s) in the image file */

	rc->readOK += nsectors;

	if(!scan_mode)
	{  if(!LargeSeek(rc->image, (gint64)(2048*s)))
	     Stop(_("Failed seeking to sector %lld in image [%s]: %s"),
		  s, "store", strerror(errno));

#ifdef DEBUG
printf("Sector %lld: writing %2d sectors\n",s,nsectors);
#endif
           n = LargeWrite(rc->image, buf, 2048*nsectors);
	   if(n != 2048*nsectors)
	     Stop(_("Failed writing to sector %lld in image [%s]: %s"),
		  s, "store", strerror(errno));

	   /* On-the-fly CRC/MD5 calculation */

	   if(!Closure->checkCrc && Closure->crcCache)  
	   {  for(i=0; i<nsectors; i++)
		Closure->crcCache[s+i] = Crc32(buf+2048*i, 2048);

	      MD5Update(&md5ctxt, buf, 2048*nsectors);
	   }
	}

	/* On-the-fly CRC testing */         

	if(Closure->checkCrc)
	  for(i=0; i<nsectors; i++)
	  {  guint32 crc = Crc32(buf+2048*i, 2048);

	     if(s+i < rc->ei->sectors)
	     {  if(Closure->crcCache[s+i] != crc)
	        {  PrintCLI(_("* CRC error, sector: %lld\n"), (long long int)s+i);
		   Closure->crcErrors++;
		}
	     }
	     else Closure->crcCache[s+i] = crc; /* add CRCsums for additional sectors */

	     MD5Update(&md5ctxt, buf+2048*i, 2048);
	  }
      }

      /*** Step the progress counter */

step_counter:
      if(Closure->guiMode && last_errors_printed != Closure->readErrors)
      {  SetLabelText(GTK_LABEL(Closure->readLinearErrors), 
		      _("Unreadable / skipped sectors: %lld"), Closure->readErrors);
	 last_errors_printed = Closure->readErrors;
      }

      s += nsectors;
      if(s>read_marker) read_marker=s;
      percent = (1000*s)/sectors;
      if(last_percent != percent) 
      {  gulong ignore;
	 int color;

	 if(rc->readOK <= last_read_ok)  /* anything read since last sample? */
	 {  speed = 0.0;                 /* nothing read */
	    if(Closure->readErrors-previous_read_errors > 0)
	         color = 2;
	    else if(Closure->crcErrors-previous_crc_errors > 0)
	         color = 4;
	    else color = Closure->additionalSpiralColor;

	    if(Closure->guiMode)
	       AddCurveValues(percent, speed, color);
	    last_percent    = percent;
	    last_speed      = speed;
	    previous_read_errors = Closure->readErrors;
	    previous_crc_errors  = Closure->crcErrors;
	    last_read_ok    = rc->readOK;
	 }
	 else
	 {  double kb_read = (rc->readOK - last_read_ok) * 2.0;
	    double elapsed = g_timer_elapsed(rc->speedTimer, &ignore);
	    double kb_sec  = kb_read / elapsed;

	    if(Closure->readErrors-previous_read_errors > 0)
	         color = 2;
	    else if(Closure->crcErrors-previous_crc_errors > 0)
	         color = 4;
	    else color = 1;

	    if(first_speed_value)
	    {   speed = kb_sec / rc->dh->singleRate;

	        if(Closure->guiMode)
		{  AddCurveValues(last_percent, speed, color);
		   AddCurveValues(percent, speed, color);
		}

		first_speed_value = FALSE;
		last_percent      = percent;
		last_speed        = speed;
		previous_read_errors = Closure->readErrors;
		previous_crc_errors  = Closure->crcErrors;
		last_read_ok      = rc->readOK;
	    }
	    else
	    {  speed = (speed + kb_sec / rc->dh->singleRate) / 2.0;
	       if(speed>99.9) speed=99.9;

	       if(Closure->guiMode)
		 AddCurveValues(percent, speed, color);
	       if(Closure->speedWarning && last_speed > 0.5)
	       {  double delta = speed - last_speed;
		  double abs_delta = fabs(delta);
		  double sp = (100.0*abs_delta) / last_speed; 

		  if(sp >= Closure->speedWarning)
		  {  if(delta > 0.0)
		       PrintCLI(_("Sector %lld: Speed increased to %4.1fx\n"), s, fabs(speed));
		     else
		       PrintCLI(_("Sector %lld: Speed dropped to %4.1fx\n"), s, fabs(speed));
		  }
	       }

	       PrintProgress(_("Read position: %3d.%1d%% (%4.1fx)"),
			     percent/10,percent%10,speed);

	       last_percent    = percent;
	       last_speed      = speed;
	       previous_read_errors = Closure->readErrors;
	       previous_crc_errors = Closure->crcErrors;
	       last_read_ok    = rc->readOK;
	       g_timer_start(rc->speedTimer);
	    }
	 }
      }
   }

   if(Closure->crcCache)
     MD5Final(Closure->md5Cache, &md5ctxt);

   if(rereading)
   {  if(!Closure->readErrors) t = g_strdup_printf(_("%lld sectors read.     "),rc->readOK);
      else                t = g_strdup_printf(_("%lld sectors read; %lld unreadable sectors."),rc->readOK,Closure->readErrors);
   }
   else
   {  if(!Closure->readErrors && !Closure->crcErrors) 
      {  
	 if(Closure->checkCrc && rc->ei)
	 {  
	    if(rc->dh->sectors != rc->ei->sectors)
	      t = g_strdup_printf(_("All sectors successfully read, but wrong image length (%lld sectors difference)"), rc->dh->sectors - rc->ei->sectors);
	    else if(memcmp(rc->ei->eh->mediumSum, Closure->md5Cache, 16))
	      t = g_strdup_printf(_("All sectors successfully read, but wrong image checksum."));
	    else t = g_strdup_printf(_("All sectors successfully read. Checksums match."));
	 }
	 else t = g_strdup_printf(_("All sectors successfully read."));
      }
      else 
      {  if(Closure->readErrors && !Closure->crcErrors)
	      t = g_strdup_printf(_("%lld unreadable sectors."),Closure->readErrors);
         else if(!Closure->readErrors && Closure->crcErrors)
	      t = g_strdup_printf(_("%lld CRC errors."),Closure->crcErrors);
	 else t = g_strdup_printf(_("%lld CRC errors, %lld unreadable sectors."),Closure->crcErrors, Closure->readErrors);
      }
   }
   PrintLog("\n%s\n",t);
   if(Closure->guiMode)
   {  if(scan_mode) SwitchAndSetFootline(Closure->readLinearNotebook, 1, Closure->readLinearFootline, 
					 "%s%s",_("Scanning finished: "),t);
      else          SwitchAndSetFootline(Closure->readLinearNotebook, 1, Closure->readLinearFootline, 
					 "%s%s",_("Reading finished: "),t);
   }
   g_free(t);

   PrintTimeToLog(rc->readTimer, "for reading/scanning.\n");

   if(rc->dh->subType != DVD && tao_tail && tao_tail == Closure->readErrors && !Closure->noTruncate)
   {  int answer;
   
      if(Closure->guiMode)
        answer = ModalWarning(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK_CANCEL, NULL,
			      _("%d sectors missing at the end of the disc.\n"
				"This is okay if the CD was written in TAO (track at once) mode.\n"
				"The Image will be trimmed accordingly. See the manual for details.\n"),
			      tao_tail);
      else 
        answer = ModalWarning(GTK_MESSAGE_WARNING, GTK_BUTTONS_OK_CANCEL, NULL,
			      _("%d sectors missing at the end of the disc.\n"
				"This is okay if the CD was written in TAO (track at once) mode.\n"
				"The Image will be trimmed accordingly. See the manual for details.\n"
				"Use the --dao option to disable image trimming.\n"),
			      tao_tail);
     
      sectors -= tao_tail;

      if(!scan_mode && answer)
        if(!LargeTruncate(rc->image, (gint64)(2048*sectors)))
	  Stop(_("Could not truncate %s: %s\n"),Closure->imageName,strerror(errno));
   }
   else if(Closure->readErrors) exitCode = EXIT_FAILURE;

   /*** Close and clean up */

   rc->unreportedError = FALSE;
   rc->earlyTermination = FALSE;

terminate:
   cleanup((gpointer)rc);
}

