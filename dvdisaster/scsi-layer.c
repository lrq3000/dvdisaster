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

/***
 *** Forward declarations
 ***/

static int query_type(DeviceHandle*, int);
static unsigned int query_size(DeviceHandle*);
static int query_copyright(DeviceHandle*);

static int read_dvd_sector(DeviceHandle*, unsigned char*, int, int);
static int read_cd_sector(DeviceHandle*, unsigned char*, int, int);
static int read_raw_cd_sector(DeviceHandle*, unsigned char*, int, int);

/***
 *** Create a buffer aligned at a 4096 byte boundary.
 *** Some SCSI drivers seem to need this.
 */

AlignedBuffer* CreateAlignedBuffer(int size)
{  AlignedBuffer *ab = g_malloc0(sizeof(AlignedBuffer));

   ab->base = g_malloc(size+4096);
   ab->buf  = ab->base + (4096 - ((unsigned long)ab->base & 4095));

   return ab;
}

void FreeAlignedBuffer(AlignedBuffer *ab)
{  g_free(ab->base);
   g_free(ab);
}

/***
 *** CD and DVD query routines.
 *** Everything below should be system independent.
 ***/

/*
 * Send INQUIRY to the device.
 */

int InquireDevice(DeviceHandle *dh, int probe_only)
{  AlignedBuffer *ab = CreateAlignedBuffer(2048);
   Sense sense;
   char *ibuf,*vbuf;
   unsigned char cmd[MAX_CDB_SIZE];
   unsigned char device_type;

   /*** Try to learn something about the device vendor */

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0x12;   /* INQUIRY */
   cmd[4] = 36;     /* allocation length */

   if(SendPacket(dh, cmd, 6, ab->buf, 36, &sense, DATA_READ)<0)
   {  FreeAlignedBuffer(ab);
      if(probe_only) return 0x1f;  /* don't care about failure, just return invalid device */

      strcpy(dh->devinfo, _("unknown"));

#ifdef SYS_LINUX
      PrintCLI("\n");
      Stop(_("Can open %s, but INQUIRY fails.\n"
	     "Chances are that you're using ide-scsi emulation for an ATAPI drive,\n"
	     "and try to access it via /dev/cdrom or /dev/hd?.\n"
	     "Either use /dev/scd? or /dev/sr? instead, or disable ide-scsi emulation.\n"),
	   dh->device);
      return 0;
#else
      PrintCLI("\n");
      Stop(_("INQUIRY failed. Something is wrong with drive %s.\n"),dh->device);
      return 0;
#endif
   }
   else
   {  int i,j,vidx=0; 

      ibuf = dh->devinfo;
      vbuf = dh->vendor;
      for(i=0,j=8; j<36; j++)   
      {  if(j==32) 
	 {  vidx = i;
	    if(i>0 && !isspace(ibuf[i-1])) /* separate the version string */
	      ibuf[i++] = ' ';
	 }
         if(   isprint(ab->buf[j])         /* eliminate multiple spaces and unprintables */
            && (!isspace(ab->buf[j]) || (i>0 && !isspace(ibuf[i-1]))))
	 {    vbuf[i] = ab->buf[j];
	      ibuf[i++] = ab->buf[j];
	 }
      }
      ibuf[i] = vbuf[i] = 0;

      if(vidx) vbuf[vidx--] = 0;
      while(vidx >= 0 && vbuf[vidx] == ' ')
	vbuf[vidx--] =0;

      if(ab->buf[0] != 0x05 && !probe_only)
      {  PrintCLI("\n");
	 if(ab->buf[0]) Stop(_("Device %s (%s) is not a CDROM drive."),dh->device,ibuf);
	 else           Stop(_("Device %s (%s) is a hard disk."),dh->device,ibuf);
      }
   }

   device_type = ab->buf[0] & 0x1f; 
   FreeAlignedBuffer(ab);

   return device_type;  /* return the SCSI peripheral device type */
}

/*
 * Find out what type of disc has been inserted.
 */

static int query_type(DeviceHandle *dh, int probe_only)
{  static char sbuf[32];
   AlignedBuffer *ab = CreateAlignedBuffer(2048);
   unsigned char *buf = ab->buf;
   Sense sense;
   unsigned char cmd[MAX_CDB_SIZE];
   unsigned int length;
   int control;
   unsigned int ua_start,ua_end,ua_end0;

   /*** If the medium is a DVD, the following query will succeed. */

   dh->mainType   = DVD;  /* default assumption */
   dh->read       = read_dvd_sector;
   dh->singleRate = 1352.54;
   dh->maxRate    = 17;
 
   /* Query length of returned data */

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0xad;     /* READ DVD STRUCTURE */
   cmd[6] = 0;        /* First layer */
   cmd[7] = 0;        /* We want PHYSICAL info */
   cmd[8] = 0;        /* Allocation length */
   cmd[9] = 2;

   /* Different drives react with different error codes on this request;
      especially CDROMs seem to react very indeterministic here 
      (okay, they obviously do not know about DVD cdbs).
      So we do not look for specific error and regard any failure as a sign
      that the medium is not a DVD. */

   if(SendPacket(dh, cmd, 12, buf, 2, &sense, DATA_READ)<0)
   {  
      if(Closure->debugMode)
	LogWarning(_("%s\nCould not query dvd structure length.\n"), 
	     GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));
   
      goto assume_cd;
   }

   length = buf[0]<<8 | buf[1];
   length += 2;

   if(length>4096) /* don't let the drive hack us using a buffer overflow ;-) */
   {  FreeAlignedBuffer(ab);
      Stop(_("Could not query dvd physical structure - implausible packet length %d\n"),length);
      return FALSE;
   }

   /* Do the real query */

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0xad;     /* READ DVD STRUCTURE */
   cmd[6] = 0;        /* First layer */
   cmd[7] = 0;        /* We want PHYSICAL info */
   cmd[8] = (length>>8) & 0xff;  /* Allocation length */
   cmd[9] = length & 0xff;

   if(SendPacket(dh, cmd, 12, buf, length, &sense, DATA_READ)<0)
   {  FreeAlignedBuffer(ab);
      Stop(_("%s\nCould not query physical dvd structure.\n"),
	   GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));
      return FALSE;
   }

   /* See what we've got */

#if 0
   {  int layer_type = buf[6] & 0x0f;
      printf("Layer type(s): ");
      if(layer_type & 0x01) printf("embossed ");
      if(layer_type & 0x02) printf("recordable ");
      if(layer_type & 0x04) printf("rewriteable ");
      printf("\n");
      fflush(stdout);
   }
#endif
   dh->subType = DVD;

   switch((buf[4]>>4) & 0x0f)   /* evaluate the book type */
   {  case  1: dh->typedescr = "DVD-RAM"; 
               dh->rewriteable = TRUE;
               break;
      case  2: dh->typedescr = "DVD-R"; 
	       break;
      case  3: dh->typedescr = "DVD-RW"; 
               dh->rewriteable = TRUE;
	       break;
      case  9: dh->typedescr = "DVD+RW"; 
               dh->rewriteable = TRUE;
	       break;
      case 10: dh->typedescr = "DVD+R"; 
	       break;
      case 14: dh->typedescr = "DVD+R9 DL"; 
	       break;

      case  0: 
      {  int layer_type = buf[6] & 0x0f;

	 if(layer_type & 0x06) 
	    dh->typedescr = "DVD-ROM (faked book type)";
	 else
	 {  dh->typedescr = "DVD-ROM";
	    dh->subType = UNSUPPORTED;
	 }
	 break;
      }

      default: 
	g_sprintf(sbuf,"DVD book type 0x%02x",(buf[4]>>4) & 0x0f); 
	dh->typedescr=sbuf; 
	break;
   }

   dh->layers = 1 + ((buf[6] & 0x60) >> 5);
   dh->sessions = 1;

   /* While we're at it, extract the user area size.
      For +RW media, this is better than the value provided by READ CAPACITY. */

   ua_start = /*buf[ 8]<<24 |*/ buf[ 9]<<16 | buf[10]<<8 | buf[11]; 
   ua_end   = /*buf[12]<<24 |*/ buf[13]<<16 | buf[14]<<8 | buf[15];
   ua_end0  = /*buf[16]<<24 |*/ buf[17]<<16 | buf[18]<<8 | buf[19];

   if(dh->layers == 1)
        dh->userAreaSize = (gint64)(ua_end-ua_start);
   else dh->userAreaSize = (gint64)(ua_end0-ua_start)*2;

   FreeAlignedBuffer(ab);
   return TRUE;

   /*** If we reach this, the medium is assumed to be one of the CD varities.*/

assume_cd:
   dh->mainType   = CD;
   dh->read       = read_cd_sector;
   dh->singleRate = 150.0;
   dh->maxRate    = 52;

   /*** First, do a READ TOC with format 0 to fetch the CONTROL field. */
   
   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0x43;  /* READ TOC/PMA/ATIP */
   cmd[2] = 0;     /* format; we want the TOC */
   cmd[6] = 1;     /* track/session number */
   cmd[7] = 0;     /* allocation length */
   cmd[8] = 2;

   if(SendPacket(dh, cmd, 10, buf, 2, &sense, DATA_READ)<0)
   {  if(!probe_only)
      {  FreeAlignedBuffer(ab);
         Stop(_("%s\nCould not query TOC length.\n"),
	      GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));
      }
      return FALSE;
   }

   length = buf[0]<<8 | buf[1];
   length += 2  ;  /* MMC3: "Disc information length excludes itself" */
   if(length>1024) /* don't let the drive hack us using a buffer overflow ;-) */
   {  FreeAlignedBuffer(ab);
      Stop(_("TOC info too long (%d), probably multisession.\n"),length);
      return FALSE;
   }

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0x43;  /* READ TOC/PMA/ATIP */
   cmd[2] = 0;     /* format; we want the TOC */
   cmd[6] = 1;     /* track/session number */
   cmd[7] = 0;     /* allocation length */
   cmd[8] = length;

   if(SendPacket(dh, cmd, 10, buf, length, &sense, DATA_READ)<0)
   {  FreeAlignedBuffer(ab);
      Stop(_("%s\nCould not read TOC.\n"),
	   GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));
      return FALSE;
   }

   control = buf[5];

   /*** Do the READ TOC again with format 2 to fetch the full toc
        as we want the disc type info also. 
	We do not use the CONTROL data included here as it turned
	out to be invalid on certain CD-ROMs. Bleah. */

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0x43;  /* READ TOC/PMA/ATIP */
   cmd[1] = 0x02;  /* TIME bit required for this format */
   cmd[2] = 2;     /* format; we want the full TOC */
   cmd[6] = 1;     /* track/session number */
   cmd[7] = 0;     /* allocation length */
   cmd[8] = 2;

   if(SendPacket(dh, cmd, 10, buf, 2, &sense, DATA_READ)<0)
   {  FreeAlignedBuffer(ab);
      Stop(_("%s\nCould not query full TOC length.\n"),
	   GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));
      return FALSE;
   }

   length = buf[0]<<8 | buf[1];
   length += 2;    /* MMC3: "Disc information length excludes itself" */
   if(length < 15)
   {  FreeAlignedBuffer(ab);
      Stop(_("TOC info too short, length %d.\n"),length);
      return FALSE;
   }
   if(length>1024) /* don't let the drive hack us using a buffer overflow ;-) */
   {  FreeAlignedBuffer(ab);
      Stop(_("TOC info too long (%d), probably multisession.\n"),length);
      return FALSE;
   }

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0x43;  /* READ TOC/PMA/ATIP */
   cmd[1] = 0x02;  /* TIME bit required for this format */
   cmd[2] = 2;     /* format; we want the full TOC */
   cmd[6] = 1;     /* track/session number */
   cmd[7] = 0;     /* allocation length */
   cmd[8] = length;

   if(SendPacket(dh, cmd, 10, buf, length, &sense, DATA_READ)<0)
   {  FreeAlignedBuffer(ab);
      Stop(_("%s\nCould not read full TOC.\n"),
	   GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));
      return FALSE;
   }

   if(buf[7] != 0xa0)
   {  int i;
      PrintLog(_("\nUnexpected TOC format (length %d):\n"),length);

      for(i=0; i<(int)length; i++) 
      {  PrintLog("%02x ",buf[i]);
	 if(i==3 || (i-3)%11 == 0) PrintLog("\n");
      }
      FreeAlignedBuffer(ab);
      Stop(_("Consider sending a bug report.\n"));
      return FALSE;
   }

   dh->sessions = buf[3];

   if(control & 4)
     switch(buf[13])
     {  case 0x00: dh->typedescr = _("Data CD mode 1"); dh->subType = DATA1; break;
        case 0x10: dh->typedescr = _("CD-I"); dh->subType = UNSUPPORTED; break;
        case 0x20: dh->typedescr = _("CD XA form 1"); dh->subType = XA21; break;
        default:   dh->typedescr = _("Unknown"); dh->subType = UNSUPPORTED; break;
     }
   else 
   {  dh->typedescr = _("CD Audio"); 
      dh->subType = UNSUPPORTED;
   }

   FreeAlignedBuffer(ab);
   return TRUE;
}

/*
 * Find out whether the drive can return raw sectors with
 * uncorrected read errors. Depending on override,
 * mode is or'ed with existing flags (override = FALSE),
 * or written over existings flags (override = TRUE). 
 */

//#define RAW_DEBUG

int SetRawMode(DeviceHandle *dh, int mode, int override)
{  AlignedBuffer *ab = CreateAlignedBuffer(2048);
   unsigned char *buf = ab->buf;
   Sense sense;
   unsigned char cdb[16];
   unsigned wanted_mode;
   int pll, ret;

   /*** Read mode page 1 */

   memset(cdb, 0, MAX_CDB_SIZE);
   cdb[0] = 0x5a;         /* MODE SENSE(10) */
   cdb[2] = 1;            /* Page code */
   cdb[8] = 255;          /* Allocation length */
   ret  = SendPacket(dh, cdb, 10, buf, 255, &sense, DATA_READ);

   if(ret<0) 
   {  FreeAlignedBuffer(ab);
#ifdef RAW_DEBUG
      Verbose("MODE SENSE: %s\n", 
	      GetSenseString(sense.sense_key, sense.asc, sense.ascq, 0));
#endif
      return FALSE;
   }

#ifdef RAW_DEBUG
   Verbose("MODE PAGE 01h:\n");
   Verbose("  mode data length = %d\n", buf[0]<<8 | buf[1]);
   Verbose("  block descriptor length = %d\n", buf[6]<<8 | buf[7]);
   Verbose("  page byte 0 = %2x\n", buf[8]);
   Verbose("  page byte 1 = %2x\n", buf[9]);
   Verbose("  page byte 2 = %2x\n", buf[10]);
   Verbose("  page byte 3 = %2x\n", buf[11]);
#endif

   pll = buf[1] + 2;  /* mode data length + 2 */

#ifdef RAW_DEBUG
   Verbose("  using mode data length %d\n", pll);
#endif

   /*** Set new raw reading mode */

   memset(cdb, 0, MAX_CDB_SIZE);
   cdb[0] = 0x55;         /* MODE SELECT(10) */
   cdb[1] = 0x10;         /* set page format (PF) bit */
   cdb[7] = 0;
   cdb[8] = pll;          /* parameter list length */

   dh->previousReadMode = buf[10];
   if(override) buf[10] = mode;       /* add new read mode */
   else         buf[10] |= mode;
   wanted_mode = buf[10];

   ret  = SendPacket(dh, cdb, 10, buf, pll, &sense, DATA_WRITE);

   if(ret<0) 
   {  FreeAlignedBuffer(ab);
#ifdef RAW_DEBUG
      Verbose("MODE SELECT: %s\n", 
	      GetSenseString(sense.sense_key, sense.asc, sense.ascq, 0));
#endif
      return FALSE;
   }

   /*** Read mode page 1 back again to make sure it worked */
       
   memset(cdb, 0, MAX_CDB_SIZE);
   memset(buf, 0, pll);
   cdb[0] = 0x5a;         /* MODE SENSE(10) */
   cdb[2] = 1;            /* Page code */
   cdb[8] = pll;          /* Allocation length */

   ret  = SendPacket(dh, cdb, 10, buf, 255, &sense, DATA_READ);

   if(ret<0) 
   {  FreeAlignedBuffer(ab);
#ifdef RAW_DEBUG
      Verbose("MODE SENSE: %s\n", 
	      GetSenseString(sense.sense_key, sense.asc, sense.ascq, 0));
#endif
      return FALSE;
   }

   if(buf[10] != wanted_mode)
   {  FreeAlignedBuffer(ab);
#ifdef RAW_DEBUG
      Verbose("Setting raw mode failed: %2x instead of %2x\n", buf[10], wanted_mode);
#endif
      return FALSE;
   }

   FreeAlignedBuffer(ab);
   return TRUE;
}

static int query_raw_mode(DeviceHandle *dh, int mode)
{  
   dh->canRawRead = SetRawMode(dh, mode, FALSE);

   if(dh->canRawRead)
   {  dh->rawBuffer = CreateRawBuffer();

      switch(dh->mainType)
      {  case CD:
	   dh->readRaw = read_raw_cd_sector;
	   break;
         default:
	   dh->readRaw = NULL;
	   break;
      }
   }

   return dh->canRawRead;
}

/*
 * Find out whether we are allowed to create an image from the DVD.
 */

static int query_copyright(DeviceHandle *dh)
{  Sense sense;
   AlignedBuffer *ab = CreateAlignedBuffer(2048);
   unsigned char *buf = ab->buf;
   unsigned char cmd[MAX_CDB_SIZE];
   unsigned char result;
   unsigned int length;

   /* Query length of returned data */

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0xad;     /* READ DVD STRUCTURE */
   cmd[6] = 0;        /* First layer */
   cmd[7] = 1;        /* We want copyright info */
   cmd[8] = 0;        /* Allocation length */
   cmd[9] = 2;

   if(SendPacket(dh, cmd, 12, buf, 2, &sense, DATA_READ)<0)
   {  FreeAlignedBuffer(ab);
      Stop(_("%s\nCould not query dvd structure length for format code 1.\n"),
	   GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));
      return TRUE;
   }

   length = buf[0]<<8 | buf[1];
   length += 2;

   if(length>4096) /* don't let the drive hack us using a buffer overflow ;-) */
   {  FreeAlignedBuffer(ab);
      Stop(_("Could not query dvd copyright info - implausible packet length %d\n"),length);
      return TRUE;
   }

   /* Do the real query */

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0xad;     /* READ DVD STRUCTURE */
   cmd[6] = 0;        /* First layer */
   cmd[7] = 1;        /* We want copyright info */
   cmd[8] = (length>>8) & 0xff;  /* Allocation length */
   cmd[9] = length & 0xff;

   if(SendPacket(dh, cmd, 12, buf, length, &sense, DATA_READ)<0)
   {  FreeAlignedBuffer(ab);
      Stop(_("%s\nCould not query copyright info.\n"),
	   GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));
      return TRUE;
   }

   result = buf[4] & 1;
   FreeAlignedBuffer(ab);
   
   return result;
}

/*
 * See whether a sector lies within the user area.
 */

static int check_sector(DeviceHandle *dh, GString *msg_out, guint64 sector, int n_sectors)
{  AlignedBuffer *scratch = CreateAlignedBuffer(32768);
   int status,result;
   char *msg;

   status = read_dvd_sector(dh, scratch->buf, sector, n_sectors);
   FreeAlignedBuffer(scratch);

   if(!status)   /* Sector was readable */ 
   {  msg = _("readable");
      result = 0;
   }
   else          /* Read error */
   {  msg = GetLastSenseString(FALSE);

      if(   dh->sense.sense_key == 0x05   /* illegal request */
	 || dh->sense.sense_key == 0x00)  /* or None; happens at least on Pioneer drives */
      {  if(   dh->sense.asc  == 0x63     
            && dh->sense.ascq == 0x00)
	      result = 1;                 /* end of user area */ 
         else result = 2;                 /* other illegal request, */
                                          /* usually 0x21 Logical block address out of range*/
      }
      else result = 3;                    /* other read error */ 
   }

   if(n_sectors == 1)
        g_string_append_printf(msg_out, _("Sector %lld: %s\n"), sector, msg);
   else g_string_append_printf(msg_out, _("Sectors %lld-%lld: %s\n"), 
			       sector, sector+n_sectors-1, msg);

   return result;
}

/*
 * Try to find out if the given sectors really lie
 * at the end of the user area.
 */

static void evaluate_results(int res0, int res1, int *result, char **msg)
{
  if(res0 == 3 || res1 == 3) 
  {  *result = FALSE;
     *msg    = _("is undecideable because of read error");
     return;
  }

  if(res0 == 0 && (res1 == 1 || res1 == 2))
  {  *result = TRUE;
     *msg    = _("looks GOOD");
     return;
  }

  if(res0 == 1 && res1 == 2)
  {  *result = FALSE;
     *msg    = _("gives unformatted size (UNUSABLE)");
     return;
  }

  *result = FALSE;
  *msg    = _("is UNUSABLE");
}

/*
 * Query the medium size.
 */

static unsigned int query_size(DeviceHandle *dh)
{  Sense sense;
   AlignedBuffer *ab = CreateAlignedBuffer(2048);
   unsigned char *buf = ab->buf; 
   unsigned char cmd[MAX_CDB_SIZE];
   gint64 read_capacity;

   /*** Query size by doing READ CAPACITY */

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0x25;  /* READ CAPACITY */

   if(SendPacket(dh, cmd, 10, buf, 8, &sense, DATA_READ)<0)
   {  FreeAlignedBuffer(ab);
      Stop(_("%s\nCould not query medium size.\n"),
	   GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));
      return 0;
   }

   read_capacity = (gint64)(buf[0]<<24 | buf[1]<<16 | buf[2]<<8 | buf[3]);
   FreeAlignedBuffer(ab);

   /*** If RS02 header search is enabled and we can find an appropriate header,
	use it as an authoritative source for the medium size. */

   if(Closure->querySize >= 2)
   {  if(dh->rs02Size <= 0)
	 dh->rs02Size = MediumLengthFromRS02(dh, MAX(read_capacity, dh->userAreaSize));
      else Verbose("Root header search succeeded!\n"); 
      if(dh->rs02Size > 0) 
      {  Verbose("Medium size obtained from ECC header: %lld sectors\n", dh->rs02Size);  
	 return dh->rs02Size;
      }
      else Verbose("Medium size could NOT be determined from ECC header.\n");
   } 
   else 
   {   if(dh->rs02Size > 0) 
      {  Verbose("Exhaustive ECC header search disabled, but root header search succeeded!\n"); 
	 Verbose("Medium size obtained from ECC header: %lld sectors\n", dh->rs02Size);  
	 return dh->rs02Size;
      }
      Verbose("Skipping medium size determination from ECC header.\n");
   }

   /*** If ISO/UDF filesystem parsing is enabled try this next. */

   if(Closure->querySize >= 1)
   {  if(dh->isoInfo)
      {  Verbose("Medium size obtained from ISO/UDF file system: %d sectors\n", 
		 dh->isoInfo->volumeSize);  
	 return dh->isoInfo->volumeSize;
      }
      else Verbose("Medium size could NOT be determined from ISO/UDF filesystem.\n");
   } else Verbose("Skipping medium size determination from ISO/UDF filesystem.\n");

   /*** If everything else fails, query the drive. */
   
   /* For CD media, thats all we have to do */

   if(dh->mainType == CD)
   {  Verbose("CD medium - using size from READ CAPACITY: %lld sectors\n", read_capacity+1);
      return read_capacity+1;  /* size is the number of the last sector, starting with 0 */
   }

   /* For DVD media, READ CAPACITY should give the real image size.
      READ DVD STRUCTURE may be same value or the unformatted size.
      But some drives appear to have both functions reversed,
      so we have to be careful here. */

   if(read_capacity == dh->userAreaSize)  /* If they are equal just return one */
   {  Verbose("READ CAPACITY and READ DVD STRUCTUE agree: %lld sectors\n", read_capacity+1);
      return read_capacity+1;  
   }
   else                                   /* Tricky case. Try some heuristics. */
   {  int last_valid, first_out;
      gint test_sector;
      int cap_result,strct_result,decision;
      char *cap_msg, *strct_msg, *decision_msg;
      GString *warning;

      /*** Try to find out the correct value empirically. */

      warning = g_string_sized_new(1024);
      g_string_printf(warning,
		      _("Different media sizes depending on query method:\n"
			"READ CAPACITY:      %lld sectors\n"
			"READ DVD STRUCTURE: %lld sectors\n\n"),
		      read_capacity+1, dh->userAreaSize+1);

      g_string_append_printf(warning, _("Evaluation of returned medium sizes:\n\n"));

      /*** Look at READ CAPACITY results */

      test_sector = read_capacity;
      last_valid = check_sector(dh, warning, test_sector,   1);
      first_out  = check_sector(dh, warning, test_sector+1, 1);

      /*** Some drives can read about 16 or 32 sectors past the image
           into the border/lead out. Drats. */

      if(last_valid == 0 && first_out == 0)  
      {  test_sector &= ~((long long)15);
       	 first_out  = check_sector(dh, warning, test_sector, 16);
       	 if(!first_out)
	   first_out = check_sector(dh, warning, test_sector+16, 16);
       	 if(!first_out)
	   first_out = check_sector(dh, warning, test_sector+32, 16);
       	 if(!first_out)
	   first_out = check_sector(dh, warning, test_sector+48, 16);
      }

      evaluate_results(last_valid, first_out, &cap_result, &cap_msg);
      g_string_append_printf(warning, "-> READ CAPACITY %s\n\n", cap_msg);

      /*** Look at READ DVD STRUCTURE results */

      test_sector = dh->userAreaSize;
      last_valid = check_sector(dh, warning, test_sector,   1);
      first_out  = check_sector(dh, warning, test_sector+1, 1);
      
      /*** Some drives can read about 16 or 32 sectors past the image
           into the border/lead out. Drats. */

      if(last_valid == 0 && first_out == 0)  
      {  test_sector &= ~((long long)15);
       	 first_out  = check_sector(dh, warning, test_sector, 16);
       	 if(!first_out)
	   first_out = check_sector(dh, warning, test_sector+16, 16);
       	 if(!first_out)
	   first_out = check_sector(dh, warning, test_sector+32, 16);
       	 if(!first_out)
	   first_out = check_sector(dh, warning, test_sector+48, 16);
      }

      evaluate_results(last_valid, first_out, &strct_result, &strct_msg);
      g_string_append_printf(warning, "-> READ DVD STRUCTURE %s\n\n", strct_msg);

      /*** Decide what to use */

      decision = 0;
      decision_msg = NULL;

      if(cap_result) 
      {  decision = 1; 
	 decision_msg = _("Using value from READ CAPACITY");
      }
      if(strct_result) 
      {  decision = 2; 
	 decision_msg = _("Using value from READ DVD STRUCTURE");
      }

      if(!decision)
      {  if(!read_capacity) decision = 2;
         if(!dh->userAreaSize) decision = 1;

	 if(read_capacity && dh->userAreaSize)
	   decision = read_capacity < dh->userAreaSize ? 1 : 2;

	 decision_msg = _("FAILED to determine image size.\n"
			  "Using smaller value as this is right on >90%% of all drives,\n"
			  "but CONTINUE AT YOUR OWN RISK (the image may be incomplete/unusable)");
      }

      g_string_append_printf(warning, _("Final decision: %s\n\n"), decision_msg);
      LogWarning(warning->str);

      g_string_free(warning, TRUE);

      switch(decision)
      {  case 1  : return read_capacity+1;
         case 2  : return dh->userAreaSize+1;
         default : Stop(_("Failed to determine image size.\n"
			  "Try using a different drive."));
	           return 0;
      }
   }

   return 0;
}

/*
 * Return size of current drive and medium (if any)
 */

gint64 CurrentImageSize()
{  DeviceHandle *dh = NULL;
   gint64 size;

#ifdef SYS_UNKNOWN
   return 0;
#endif

   dh = OpenDevice(Closure->device);
   if(!dh) return 0;
   if(InquireDevice(dh, 1) != 0x05) 
   {  CloseDevice(dh);
      return 0;
   }

   if(!query_type(dh, 1))
   {  CloseDevice(dh);
      return 0;
   }

   size = query_size(dh);
   CloseDevice(dh);

   return size;
}

/*
 * Return capacity of current medium (if any)
 */

gint64 CurrentImageCapacity()
{  DeviceHandle *dh = NULL;
   Sense sense;
   AlignedBuffer *ab = CreateAlignedBuffer(2048);
   unsigned char *buf = ab->buf; 
   unsigned char cmd[MAX_CDB_SIZE];
   gint64 cmc_size;
   int i,idx, n_lists;

#ifdef SYS_UNKNOWN
   return 0;
#endif

   dh = OpenDevice(Closure->device);
   if(!dh)
   {  FreeAlignedBuffer(ab);
      return 0;
   }

   if(InquireDevice(dh, 1) != 0x05) 
   {  CloseDevice(dh);
      FreeAlignedBuffer(ab);
      return 0;
   }

   /* Get size by doing READ CAPACITY */

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0x23;  /* READ FORMAT CAPACITIES */
   cmd[7] = 1;     /* 256 bytes of buffer (32*8 lists maximum) */

   if(SendPacket(dh, cmd, 10, buf, 256, &sense, DATA_READ)<0)
   {  FreeAlignedBuffer(ab);
      return 0;
   }

//   printf("Cap list length %d\n", buf[3]);

   CloseDevice(dh);

   /* Current/Maximum capacacity descriptor */

   cmc_size = (gint64)(buf[4]<<24 | buf[5]<<16 | buf[6]<<8 | buf[7]);

   n_lists = buf[3] / 8;
   n_lists--;

   if(!n_lists) 
   {  FreeAlignedBuffer(ab);
      return cmc_size;
   }

#if 0
   printf("CMC descriptor: %lld blocks\n", cmc_size);

   switch(buf[8] & 3)
   {  case 1: /* unformatted */
        printf("unformatted\n");
        break;
      case 2: /* formatted */
        printf("formatted\n");
        break;
      case 3: /* no medium */
        printf("%d no medium\n", buf[8]);
        break;
      default: /* b0rked */
        printf("b0rked\n");
        break;
   }
#endif

   /* Now go through all capacity lists */

   for(i=0, idx=12; i<n_lists; i++, idx+=8)
   {  gint64 result;
      //printf("Cap list %d - type %d\n", i, buf[idx+4]>>2);
      //size = (gint64)(buf[idx]<<24 | buf[idx+1]<<16 | buf[idx+2]<<8 | buf[idx+3]);
      //printf(".. size %lld\n", size);

      switch(buf[idx+4]>>2)  /* format type */
      {  case 0x00:
         case 0x10:  /* blank CD-RW capacity */
         case 0x26:  /* blank DVD+RW capacity */
	     result = (gint64)(buf[idx]<<24 | buf[idx+1]<<16 | buf[idx+2]<<8 | buf[idx+3]);
	     FreeAlignedBuffer(ab);
	     return result;
	     break;
      }
   }
   
   return 0;
}

/*
 * Spin up drive.
 * Most drive give a *beep* about sending the START STOP CDB,
 * so we simply nudge them with reading request until the spin up
 * time is over. Pathetic ;-)
 */

void SpinupDevice(DeviceHandle *dh)
{  AlignedBuffer *ab = CreateAlignedBuffer(32768);
   GTimer *timer;
   gint64 s;

   if(!Closure->spinupDelay)
     return;

   PrintCLI(_("Waiting %d seconds for drive to spin up...\n"), Closure->spinupDelay);
   
   timer = g_timer_new();
   g_timer_start(timer);

   for(s=0; ;s+=16)
   {  int status;
      double elapsed;
      gulong ignore;

      if(s>=dh->sectors) return;
 
      status = ReadSectors(dh, ab->buf, s, 16);
      if(status) return;

      elapsed = g_timer_elapsed(timer, &ignore);
      if(elapsed > Closure->spinupDelay)
	break;
   }

   FreeAlignedBuffer(ab);
   g_timer_destroy(timer);
}

/*
 * Set the reading speed. Does not work reliably yet. 
 */

#if 0
static void set_speed(DeviceHandle *dh)
{  Sense sense;
   char cmd[MAX_CDB_SIZE];
   char buf[4];
   int speed_kb;
   int rotcntl;

   if(driveSpeed < 0) 
   {  rotcntl = 1; 
      driveSpeed = -driveSpeed;
   }
   else rotcntl = 0;

   if(!driveSpeed)		/* use maximum performance */
        speed_kb = 0xffff;
   else speed_kb = dh->subType == DVD ? 1353*driveSpeed : 177*driveSpeed;

   if(speed_kb > 0xffff) speed_kb = 0xffff;

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0xbb;            /* SET CD SPEED */
   cmd[1] = rotcntl & 0x01;  /* CLV or CAV speed */
   cmd[2] = (speed_kb >> 8) & 0xff;
   cmd[3] = speed_kb & 0xff;
   cmd[4] = 0xff;            /* Set write speed always to max */
   cmd[5] = 0xff;

   if(SendPacket(dh, cmd, 12, buf, 4, &sense, DATA_READ)<0)
   {  LogWarning(_("Setting the drive speed failed:\n"));
      PrintSenseNow(sense.sense_key, sense.asc, sense.ascq);
   }
}
#endif

/*
 * Sector reading using the packet interface.
 * Using read() hangs some Linux systems on read errors,
 * so we can't go that route.
 */

static int read_dvd_sector(DeviceHandle *dh, unsigned char *buf, int lba, int nsectors)
{  Sense *sense = &dh->sense;
   unsigned char cmd[MAX_CDB_SIZE];
   int ret;

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0x28;  /* READ(10) */
   cmd[1] = 0;  /* no special flags */
   cmd[2] = (lba >> 24) & 0xff;
   cmd[3] = (lba >> 16) & 0xff;
   cmd[4] = (lba >>  8) & 0xff;
   cmd[5] = lba & 0xff;
   cmd[6] = 0;         /* reserved */
   cmd[7] = 0;         /* number of sectors */
   cmd[8] = nsectors;  /* read 1 sector */

   ret = SendPacket(dh, cmd, 10, buf, 2048*nsectors, sense, DATA_READ);

   if(ret<0) RememberSense(sense->sense_key, sense->asc, sense->ascq);

   return ret;
}

static int read_cd_sector(DeviceHandle *dh, unsigned char *buf, int lba, int nsectors)
{  Sense *sense = &dh->sense;
   unsigned char cmd[MAX_CDB_SIZE];
   int ret;

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0]  = 0xbe;         /* READ CD */
   switch(dh->subType)
   {  case DATA1: cmd[1] = 2<<2; break;  /* data mode 1 */
      case XA21:  cmd[1] = 4<<2; break;  /* xa mode 2 form 1 */
   }
   cmd[2]  = (lba >> 24) & 0xff;
   cmd[3]  = (lba >> 16) & 0xff;
   cmd[4]  = (lba >>  8) & 0xff;
   cmd[5]  = lba & 0xff;
   cmd[6]  = 0;        /* number of sectors to read (3 bytes) */
   cmd[7]  = 0;  
   cmd[8]  = nsectors; /* read nsectors */

   cmd[9]  = 0x10;  /* we want the user data only */
   cmd[10] = 0;    /* reserved stuff */
   cmd[11] = 0;    /* no special wishes for the control byte */

   ret = SendPacket(dh, cmd, 12, buf, 2048*nsectors, sense, DATA_READ);

   if(ret<0) RememberSense(sense->sense_key, sense->asc, sense->ascq);
	
   return ret;
}

static int read_raw_cd_sector(DeviceHandle *dh, unsigned char *outbuf, int lba, int nsectors)
{  Sense *sense = &dh->sense;
   unsigned char cdb[MAX_CDB_SIZE];
   AlignedBuffer *raw = CreateAlignedBuffer(4096);
   RawBuffer *rb;
   int i, ret = -1;

   if(!dh->rawBuffer)  /* sanity check */ 
     return 1;

   /* Return error for reading multiple sectors;
      caller will retry reading single sectors */

   if(nsectors != 1)
     return 1;

   /* reset raw reading buffer */

   rb = dh->rawBuffer;
   rb->lba          = lba;
   rb->samplesRead  = 0;
   rb->sampleLength = 0;

   /* try a number of raw reads */

   for(i=0; i<Closure->rawAttempts; i++)
   {  Verbose("Trying raw read #%d for sector %d\n", i, lba);

      memset(cdb, 0, MAX_CDB_SIZE);
      cdb[0]  = 0xbe;         /* READ CD */
      switch(dh->subType)     /* Expected sector type */
      {  case DATA1: cdb[1] = 2<<2; rb->sampleLength=2352; break;  /* data mode 1 */
         case XA21:  cdb[1] = 4<<2; rb->sampleLength=2328; break;  /* xa mode 2 form 1 */
      }
      cdb[2]  = (lba >> 24) & 0xff;
      cdb[3]  = (lba >> 16) & 0xff;
      cdb[4]  = (lba >>  8) & 0xff;
      cdb[5]  = lba & 0xff;
      cdb[6]  = 0;        /* number of sectors to read (3 bytes) */
      cdb[7]  = 0;  
      cdb[8]  = 1;        /* read nsectors */

      cdb[9]  = 0xb8;     /* we want Sync + Header + User data + EDC/ECC */
      cdb[10] = 0;        /* reserved stuff */
      cdb[11] = 0;        /* no special wishes for the control byte */

      memcpy(raw->buf, Closure->deadSector, 2048);
      ret = SendPacket(dh, cdb, 12, raw->buf, rb->sampleLength, sense, DATA_READ);

      if(ret<0)  /* Read failed, data incomplete or missing */
      {  RememberSense(sense->sense_key, sense->asc, sense->ascq);

	 /* See if drive returned some data at all. */
	
	 if(memcmp(raw->buf, Closure->deadSector, 2048))
	 {  memcpy(rb->rawBuf[rb->samplesRead], raw->buf, rb->sampleLength);
	    rb->rawState[rb->samplesRead] = RAW_READ_ERROR;
	    rb->samplesRead++;
	    Verbose("... raw data read\n");
	 }
	 else /* Nope, no data came back */
	 {  Verbose("... nothing returned\n");
	 }
      }
      else   /* Drive says data is okay. Remember this but trust  it */
      {  memcpy(rb->rawBuf[rb->samplesRead], raw->buf, rb->sampleLength);
	 rb->rawState[rb->samplesRead] = RAW_SUCCESS;
	 rb->samplesRead++;
	 Verbose("... reading SUCCEEDED\n");
      }

      Verbose("--> %d raw samples gathered\n", rb->samplesRead);
   }

   FreeAlignedBuffer(raw);

   if(RecoverRaw(outbuf, rb))
     return 0;

   return ret;
}

/*
 * Sector reading through the device handle.
 * dh->read dispatches to one the routines above.
 * Certain error conditions have been (empirically) found to be retryable
 * event if the SCSI specs say otherwise,
 * so we check for them and do up to 3 retries for the user's convenience.
 */

int ReadSectors(DeviceHandle *dh, unsigned char *buf, gint64 s, int nsectors)
{  int retry,status;

   /* See if we are in a simulated defective area */ 

   if(dh->defects)
   {  gint64 i,idx;

     for(idx=s,i=0; i<nsectors; idx++,i++)
       if(GetBit(dh->defects, idx))
       {  dh->sense.sense_key = 3;
	  dh->sense.asc       = 255;
	  dh->sense.ascq      = 255;
	  RememberSense(dh->sense.sense_key, dh->sense.asc, dh->sense.ascq);
	  return TRUE;
       }
   }

   /* Try normal read */

   for(retry=3; retry; retry--)
   {  status = dh->read(dh, buf, s, nsectors);

      if(status && retry>1 && dh->sense.sense_key == 4 && dh->sense.asc == 8 && dh->sense.ascq == 3)
      { 
	 PrintCLIorLabel(Closure->status,
			 _("Retrying Sector %lld because of: %s\n"),
			 s, GetLastSenseString(FALSE));
      }
      else break;
   }

   /* Try raw read if possible */

   if(status && dh->readRaw)
     status = dh->readRaw(dh, buf, s, nsectors);

   return status;
}

/*** 
 *** Open the device and query some of its properties.
 ***/

DeviceHandle* OpenAndQueryDevice(char *device)
{  DeviceHandle *dh = NULL;

   /* Open the device. This won't return if the device can't be opened. */

   dh = OpenDevice(device);
   InquireDevice(dh, 0);

#ifdef SYS_LINUX
   PrintLog(_("\nDevice: %s, %s\n"),device, dh->devinfo);
#endif

#ifdef SYS_MINGW
   PrintLog(_("\nDevice: %s (%s), %s\n"),
	    device, dh->aspiUsed ? "ASPI" : "SPTI", dh->devinfo);
#endif

   query_type(dh, 0);

   /* Try switching into raw mode if rawAttempts > 0 */

   if(Closure->rawAttempts > 0)
   {  if(query_raw_mode(dh, 0x20))
      {  PrintLog(_("... Device can read defective sectors in RAW mode. Good!\n")); 
      }
      else
      {  PrintLog(_("... Device can NOT read defective sectors in RAW mode.\n")); 
      }
   }

   if(Closure->querySize >= 1)  /* parseUDF or better requested */
     ExamineUDF(dh);

   dh->sectors = query_size(dh);

   switch(dh->subType)
   {  case DVD:
        if(!dh->isoInfo) // || dh->rs02Size > 0)
	  dh->mediumDescr = g_strdup_printf(_("Medium: %s, %lld sectors%s %d layer(s)"),
					    dh->typedescr, dh->sectors, 
					    dh->rs02Size ? ", Ecc," : ",",
					    dh->layers);
	else
	  dh->mediumDescr = g_strdup_printf(_("Medium \"%s\": %s, %lld sectors%s %d layer(s), created %s"),
					    dh->isoInfo->volumeLabel,
					    dh->typedescr, dh->sectors, 
					    dh->rs02Size ? ", Ecc," : ",",
					    dh->layers, dh->isoInfo->creationDate);

	PrintLog("%s\n\n", dh->mediumDescr);
	break;

      case DATA1:
      case XA21:
        if(!dh->isoInfo) // || dh->rs02Size > 0)
	  dh->mediumDescr = g_strdup_printf(_("Medium: %s, %lld sectors%s"),
					    dh->typedescr, dh->sectors,
					    dh->rs02Size ? ", Ecc" : " ");
	else
	  dh->mediumDescr = g_strdup_printf(_("Medium \"%s\": %s, %lld sectors%s created %s"),
					    dh->isoInfo->volumeLabel,
					    dh->typedescr, dh->sectors,
					    dh->rs02Size ? ", Ecc," : ",",
					    dh->isoInfo->creationDate);

	PrintLog("%s\n\n", dh->mediumDescr);
	break;

      default:
      {  char *td = alloca(strlen(dh->typedescr)+1);

 	 strcpy(td, dh->typedescr);
	 CloseDevice(dh);
       	 Stop(_("This software does not support \"%s\" type media."), td);
       	 return NULL;
      }
   }

   if(dh->mainType == DVD && query_copyright(dh))
   {  CloseDevice(dh);
      Stop(_("This software does not support encrypted media.\n"));
   }

   if(dh->sessions>1)
   {  int sessions = dh->sessions;

      CloseDevice(dh);
      Stop(_("This software does not support multisession (%d sessions) media."), sessions);
      return NULL;
   }

   /* Create the bitmap of simulated defects */

   if(Closure->simulateDefects)
     dh->defects = SimulateDefects(dh->sectors);

   return dh;
}

/***
 *** Debugging function for sending a cdb to the drive
 ***/

int SendReadCDB(char *device, unsigned char *buf, unsigned char *cdb, int cdb_len, int alloc_len)
{  DeviceHandle *dh = NULL;
   Sense sense;
   int i,status;

   dh = OpenDevice(device);
   InquireDevice(dh, 0);

   PrintLog("Sending cdb to device: %s, %s\n", device, dh->devinfo);

   PrintLog("cdb:");
   for(i=0; i<cdb_len; i++)
     PrintLog(" %02x",cdb[i]);
   PrintLog(", length %d, allocation length %d\n", cdb_len, alloc_len);

   status = SendPacket(dh, cdb, cdb_len, buf, alloc_len, &sense, DATA_READ);

   CloseDevice(dh);

   if(status < 0)
   {  PrintLog("\nOperation failed with status = %d\n", status);
      PrintLog("Sense key: %02x, asc/ascq: %02x/%02x\n", sense.sense_key, sense.asc, sense.ascq);
      PrintLog("%s\n", GetSenseString(sense.sense_key, sense.asc, sense.ascq, FALSE));
   }

   return status;
}
