/*  dvdisaster: Additional error correction for optical media.
 *  Copyright (C) 2004-2007 Carsten Gnoerlich.
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
   int phy_info4, phy_info6;
   int is_dvd_plus = FALSE;
   int is_dvd_dash = FALSE;
   int is_hd_dvd   = FALSE;
   int book_type   = 0;
   int i;

   Verbose("# *** query_type(%s, %d) ***\n", dh->devinfo, probe_only);
   Verbose("#BD: starting media probe\n");

   /*** If the medium is a BD, the following will succeed. */

   dh->mainType    = BD;
   dh->read        = read_dvd_sector;
   dh->singleRate  = 36000.0/8.0;  /* 1x = 36 kbit */
   dh->maxRate     = 8;
   dh->clusterSize = 32;

   /* Query length of returned data */

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0xad;     /* READ DVD STRUCTURE */
   cmd[1] = 0x01;     /* subcommand for BD */
   cmd[6] = 0;        /* First layer */
   cmd[7] = 0;        /* We want DI (disc information) */
   cmd[8] = 0;        /* Allocation length */
   cmd[9] = 2;

   /*** Only a BD should respond positively here */
   
   Verbose("#BD: trying READ DVD with BD subcommand for size\n");
   if(SendPacket(dh, cmd, 12, buf, 2, &sense, DATA_READ)<0)
   {  
	Verbose(_("%s\nCould not query BD disc structure length.\n"), 
	     GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));
   
      goto try_dvd;
   }

   length = buf[0]<<8 | buf[1];
   length += 2;
   Verbose("#BD: disc structure query succeeded, length %d bytes\n", length);

   /*** Some DVD drives ignore the media type 0x01 and return the dvd structure.
	Since the DVD structure is 2052 bytes while the BD DI is 4100 bytes,
	we can tell from the size whether we have been fooled. */

   if(length != 4100) /* not a BD */
      goto try_dvd;

   /* Do the real query */

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0xad;     /* READ DVD STRUCTURE */
   cmd[1] = 0x01;     /* subcommand for BD */
   cmd[6] = 0;        /* First layer */
   cmd[7] = 0;        /* We want DI (disc information) */
   cmd[8] = (length>>8) & 0xff;  /* Allocation length */
   cmd[9] = length & 0xff;

   Verbose("#BD: trying READ DVD with BD subcommand for real query\n");
   if(SendPacket(dh, cmd, 12, buf, length, &sense, DATA_READ)<0)
   {  FreeAlignedBuffer(ab);
      Stop(_("%s\nCould not query BD disc structure.\n"),
	   GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));
      return FALSE;
   }

   if(Closure->verbose)
      HexDump(buf, length, 16);

   dh->layers = 0;        /* n.a. ? */
   dh->sessions = 1;
   dh->userAreaSize = 0;  /* n.a. ? */

   for(i=0; i<6; i++)
      dh->manuID[i] = isprint(buf[4+100+i]) ? buf[4+100+i] : ' ';
   dh->manuID[6] = 0;

   if(!strncmp((char*)&buf[4+8], "BDO", 3))
   {  dh->typedescr = "BD-ROM";
      dh->subType = UNSUPPORTED;
   }

   if(!strncmp((char*)&buf[4+8], "BDW", 3))
   {  dh->typedescr = "BD-RE";
      dh->subType = BD;
   }

   if(!strncmp((char*)&buf[4+8], "BDR", 3))
   {  dh->typedescr = "BD-R";
      dh->subType = BD;
   }

   Verbose("#BD: BD medium detected, type %s\n", dh->typedescr);
   FreeAlignedBuffer(ab);
   return TRUE;

   /*** If the medium is a DVD, the following query will succeed. */

try_dvd:
   Verbose("#DVD: starting media probe\n");
   dh->mainType    = DVD;  /* default assumption */
   dh->read        = read_dvd_sector;
   dh->singleRate  = 1352.54;
   dh->maxRate     = 17;
   dh->clusterSize = 16;

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

   Verbose("#DVD: trying READ DVD for size of PHYSICAL info\n");
   if(SendPacket(dh, cmd, 12, buf, 2, &sense, DATA_READ)<0)
   {  
      if(Closure->verbose)
	Verbose(_("%s\nCould not query dvd structure length.\n"), 
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
   Verbose("#DVD: size returned is %d\n", length);

   /* Do the real query */

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0xad;     /* READ DVD STRUCTURE */
   cmd[6] = 0;        /* First layer */
   cmd[7] = 0;        /* We want PHYSICAL info */
   cmd[8] = (length>>8) & 0xff;  /* Allocation length */
   cmd[9] = length & 0xff;

   Verbose("#DVD: trying READ DVD for real PHYSICAL info\n");
   if(SendPacket(dh, cmd, 12, buf, length, &sense, DATA_READ)<0)
   {  FreeAlignedBuffer(ab);
      Stop(_("%s\nCould not query physical dvd structure.\n"),
	   GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));
      return FALSE;
   }
   if(Closure->verbose)
      HexDump(buf, length, 16);

   /* Determine whether we have DVD or HD DVD */

   book_type = (buf[4]>>4) & 0x0f;
   is_hd_dvd = (book_type >= 4 && book_type <= 6);
   Verbose("#DVD: book type %d; is HD DVD: %d\n", book_type, is_hd_dvd);

   if(is_hd_dvd)
   {  dh->mainType    = HDDVD;
      dh->singleRate  = 36550.0/8.0;  /* 1x = 36.5 kbit */
      dh->maxRate     = 8;
      dh->clusterSize = 32;
   }

   /* Determine number of layers */

   dh->layers = 1 + ((buf[6] & 0x60) >> 5);
   dh->sessions = 1;
   Verbose("#DVD: %d layers\n", dh->layers);

   /* While we're at it, extract the user area size.
      For +RW media, this is better than the value provided by READ CAPACITY. */

   ua_start = /*buf[ 8]<<24 |*/ buf[ 9]<<16 | buf[10]<<8 | buf[11]; 
   ua_end   = /*buf[12]<<24 |*/ buf[13]<<16 | buf[14]<<8 | buf[15];
   ua_end0  = /*buf[16]<<24 |*/ buf[17]<<16 | buf[18]<<8 | buf[19];
   Verbose("#DVD: ua_start/_end/_end0: %d %d %d\n", ua_start, ua_end, ua_end0);

   if(dh->layers == 1)
   {  gint64 max_size = is_hd_dvd ? MAX_HD_DVD_SL_SIZE : MAX_DVD_SL_SIZE;
      dh->userAreaSize = (gint64)(ua_end-ua_start);

      if(dh->userAreaSize < 0 || dh->userAreaSize > max_size)
      {  LogWarning(_("READ DVD STRUCTURE: implausible medium size, %lld-%lld=%lld sectors\n"),
		    (gint64)ua_end, (gint64)ua_start, (gint64)dh->userAreaSize);
	 dh->userAreaSize = 0;
      }
   }
   else 
   {  gint64 max_size = is_hd_dvd ? MAX_HD_DVD_DL_SIZE : MAX_DVD_DL_SIZE;
      dh->userAreaSize = (gint64)(ua_end0-ua_start)*2;

      if(dh->userAreaSize < 0 || dh->userAreaSize > max_size)
      {  LogWarning(_("READ DVD STRUCTURE: implausible medium size, %lld-%lld=%lld sectors\n"),
		    (gint64)ua_end0, (gint64)ua_start, (gint64)dh->userAreaSize);
	 dh->userAreaSize = 0;
      }
   }

   /*** Some drives report lead in information along with physical info. 
        This allows us to collect some info on DVD-ROM drives which 
        can not do READ DVD STRUCTURE with subtype 0x0e. */

   dh->manuID[0] = 0;

   /* Check if some nonzero bytes have been returned */

   for(i=0x200; i<length; i++)
      if(buf[i])
      {  int j;

	 Verbose("#DVD: physical info contains lead-in data\n");
	 for(j=0; j<6; j++)
	    dh->manuID[j] = isprint(buf[0x225+j]) ? buf[0x225+j] : ' ';
	 dh->manuID[6] = ' ';

	 for(j=0; j<6; j++)
	    dh->manuID[j+7] = isprint(buf[0x22d+j]) ? buf[0x22d+j] : ' ';
	 dh->manuID[13] = 0;

	 for(j=11; j>=0; j--)
	    if(dh->manuID[j] == ' ') dh->manuID[j] = 0;
	    else break;

	 Verbose("#DVD: manufacturer id %s\n", dh->manuID);
	 break;
      }

   /*** Find out medium type */

   phy_info4 = buf[4];
   phy_info6 = buf[6];
   Verbose("#DVD: phy_info4/6: 0x%x 0x%x\n", phy_info4, phy_info6);

   /* Try getting ADIP information. 
      This is more reliable than the physical info. */

   if(!is_hd_dvd)
   {  memset(cmd, 0, MAX_CDB_SIZE);
      cmd[0] = 0xad;     /* READ DVD STRUCTURE */
      cmd[6] = 0;        /* First layer */
      cmd[7] = 0x11;     /* We want the ADIP */
      cmd[8] = 0;        /* Allocation length */
      cmd[9] = 2;

      Verbose("#DVD: trying READ DVD for size of ADIP\n");
      if(SendPacket(dh, cmd, 12, buf, 2, &sense, DATA_READ) == 0)
      {  length = buf[0]<<8 | buf[1];
	 length += 2;

	 Verbose("#DVD: size returned is %d\n", length);

	 if(length < 4096)
	 {  memset(cmd, 0, MAX_CDB_SIZE);
	    cmd[0] = 0xad;     /* READ DVD STRUCTURE */
	    cmd[6] = 0;        /* First layer */
	    cmd[7] = 0x11;     /* We want the ADIP */
	    cmd[8] = (length>>8) & 0xff;  /* Allocation length */
	    cmd[9] = length & 0xff;
	    
	    Verbose("#DVD: trying READ DVD for real ADIP\n");
	    if(SendPacket(dh, cmd, 12, buf, length, &sense, DATA_READ) == 0)
	    {  int i;

	       if(Closure->verbose)
		  HexDump(buf, length, 16);

	       is_dvd_plus = TRUE;
	       phy_info4 = buf[4];
	       phy_info6 = buf[6];
	       Verbose("#DVD: assuming DVD plus; phy_info4/6 now 0x%x 0x%x\n", phy_info4, phy_info6);

	       for(i=0; i<11; i++)
		  dh->manuID[i] = isprint(buf[23+i]) ? buf[23+i] : ' ';
	       dh->manuID[11] = 0;

	       for(i=10; i>=0; i--)
		  if(dh->manuID[i] == ' ') dh->manuID[i] = 0;
		  else break;
	    }
	 }
      }
      else Verbose("#DVD: no ADIP\n");
   }

   /* Get pre-recorded info from lead-in (only on -R/-RW media).
      Only used for getting the manufacturer ID. */
   
   if(!is_hd_dvd)
   {  memset(cmd, 0, MAX_CDB_SIZE);
      cmd[0] = 0xad;     /* READ DVD STRUCTURE */
      cmd[6] = 0;        /* First layer */
      cmd[7] = 0x0E;     /* We want the lead-in info */
      cmd[8] = 0;        /* Allocation length */
      cmd[9] = 2;

      Verbose("#DVD: trying READ DVD for size of lead-in\n");
      if(SendPacket(dh, cmd, 12, buf, 2, &sense, DATA_READ) == 0)
      {  length = buf[0]<<8 | buf[1];
	 length += 2;

	 Verbose("#DVD: size returned is %d\n", length);
	 if(length < 4096)
	 {  memset(cmd, 0, MAX_CDB_SIZE);
	    cmd[0] = 0xad;     /* READ DVD STRUCTURE */
	    cmd[6] = 0;        /* First layer */
	    cmd[7] = 0x0E;     /* We want the lead-in info */
	    cmd[8] = (length>>8) & 0xff;  /* Allocation length */
	    cmd[9] = length & 0xff;

	    Verbose("#DVD: trying READ DVD for real lead-in\n");
	    if(SendPacket(dh, cmd, 12, buf, length, &sense, DATA_READ) == 0)
	    {  int i;

	       is_dvd_dash = TRUE;
	       Verbose("#DVD: assuming DVD dash\n");

	       for(i=0; i<6; i++)
		  dh->manuID[i] = isprint(buf[21+i]) ? buf[21+i] : ' ';
	       dh->manuID[6] = ' ';

	       for(i=0; i<6; i++)
		  dh->manuID[i+7] = isprint(buf[29+i]) ? buf[29+i] : ' ';
	       dh->manuID[13] = 0;

	       for(i=11; i>=0; i--)
		  if(dh->manuID[i] == ' ') dh->manuID[i] = 0;
		  else break;
	    }
	 }
      } else Verbose("#DVD: lead-in could not be queried\n");
   }

   /*** Layer type info (may be faked) */

   if(Closure->verbose)
   {  int layer_type = phy_info6 & 0x0f;
      Verbose("#DVD: Layer type(s): ");
      if(layer_type & 0x01) Verbose("embossed ");
      if(layer_type & 0x02) Verbose("recordable ");
      if(layer_type & 0x04) Verbose("rewriteable ");
      Verbose("\n");
   }

   /*** Evaluate book type */

   dh->subType = DVD;

   book_type = (phy_info4>>4) & 0x0f;
   Verbose("#DVD: book type (%d) evaluation...\n", book_type);
   switch(book_type)   /* evaluate the book type */
   {  case  1: dh->typedescr = "DVD-RAM"; 
               dh->rewriteable = TRUE;
               break;
      case  2: dh->typedescr = dh->layers == 1 ? "DVD-R" : "DVD-R DL"; 
	       break;
      case  3: dh->typedescr = "DVD-RW"; 
               dh->rewriteable = TRUE;
	       break;
      case  4: dh->typedescr = "HD DVD-ROM";
	       dh->subType = UNSUPPORTED;
	       break;
      case  5: dh->typedescr = "HD DVD-RAM";
	       dh->subType = HDDVD;
               dh->rewriteable = TRUE;
	       break;
      case  6: dh->typedescr = "HD DVD-R";
	       dh->subType = HDDVD;
	       break;
      case  9: dh->typedescr = "DVD+RW"; 
               dh->rewriteable = TRUE;
	       break;
      case 10: dh->typedescr = "DVD+R"; 
	       break;
      case 13: dh->typedescr = "DVD+RW DL"; 
               dh->rewriteable = TRUE;
	       break;
      case 14: dh->typedescr = "DVD+R DL"; 
	       break;

      case  0: /* tricky case: real or faked DVD-ROM? */
      {  int layer_type = phy_info6 & 0x0f;
	 
	 Verbose("#DVD: fake DVD-ROM detection (%d %d 0x%x)\n", 
		 is_dvd_dash, is_dvd_plus, layer_type); 
	 if(is_dvd_dash)
	 {  dh->typedescr = dh->layers == 1 ? "DVD-R/-RW" : "DVD-R DL"; 
	    break;
	 }

	 if(is_dvd_plus)
	 {  dh->typedescr = dh->layers == 1 ? "DVD+R/+RW" : "DVD+R DL"; 
	    break;
	 }

	 if(layer_type & 0x06) /* strange thing: (re-)writeable but neither plus nor dash */ 
	 {  dh->typedescr = "DVD-ROM (fake)";
	    break;
	 }

	 dh->typedescr = "DVD-ROM";
	 dh->subType = UNSUPPORTED;
	 break;
      }

      default: 
	g_sprintf(sbuf,"DVD book type 0x%02x",(phy_info4>>4) & 0x0f); 
	dh->typedescr=sbuf; 
	break;
   }

   FreeAlignedBuffer(ab);
   Verbose("#DVD: DVD medium detected, type %s\n", dh->typedescr);
   return TRUE;

   /*** If we reach this, the medium is assumed to be one of the CD varities.*/

assume_cd:
   Verbose("#CD: starting media probe\n");
   dh->mainType    = CD;
   dh->read        = read_cd_sector;
   dh->readRaw     = read_raw_cd_sector;
   dh->singleRate  = 150.0;
   dh->maxRate     = 52;
   dh->clusterSize = 16;  /* really 1, but this is faster */ 

   /*** First, do a READ TOC with format 0 to fetch the CONTROL field. */
   
   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0x43;  /* READ TOC/PMA/ATIP */
   cmd[2] = 0;     /* format; we want the TOC */
   cmd[6] = 1;     /* track/session number */
   cmd[7] = 0;     /* allocation length */
   cmd[8] = 2;

   Verbose("#CD: querying size of READ TOC/PMA/ATIP (for TOC)\n");
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
   Verbose("#CD: size returned is %d\n", length);

   if(length>1024) /* don't let the drive hack us using a buffer overflow ;-) */
   {  if(Closure->verbose)
	 HexDump(buf, 1024, 16);

      FreeAlignedBuffer(ab);
      Stop(_("TOC info too long (%d), probably multisession.\n"),length);
      return FALSE;
   }

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0x43;  /* READ TOC/PMA/ATIP */
   cmd[2] = 0;     /* format; we want the TOC */
   cmd[6] = 1;     /* track/session number */
   cmd[7] = (length>>8) & 0xff; /* allocation length */
   cmd[8] = length & 0xff;

   Verbose("#CD: querying real READ TOC/PMA/ATIP (for TOC)\n");
   if(SendPacket(dh, cmd, 10, buf, length, &sense, DATA_READ)<0)
   {  FreeAlignedBuffer(ab);
      Stop(_("%s\nCould not read TOC.\n"),
	   GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));
      return FALSE;
   }
   if(Closure->verbose)
      HexDump(buf, length, 16);

   control = buf[5];
   Verbose("#CD: control is 0x%x\n", control);
   
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

   Verbose("#CD: querying size of READ TOC/PMA/ATIP (for full TOC)\n");
   if(SendPacket(dh, cmd, 10, buf, 2, &sense, DATA_READ)<0)
   {  FreeAlignedBuffer(ab);
      Stop(_("%s\nCould not query full TOC length.\n"),
	   GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));
      return FALSE;
   }

   length = buf[0]<<8 | buf[1];
   length += 2;    /* MMC3: "Disc information length excludes itself" */
   Verbose("#CD: size returned is %d\n", length);

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
   cmd[7] = (length>>8) & 0xff; /* allocation length */
   cmd[8] = length & 0xff;

   Verbose("#CD: querying real READ TOC/PMA/ATIP (for full TOC)\n");
   if(SendPacket(dh, cmd, 10, buf, length, &sense, DATA_READ)<0)
   {  FreeAlignedBuffer(ab);
      Stop(_("%s\nCould not read full TOC.\n"),
	   GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));
      return FALSE;
   }
   if(Closure->verbose)
      HexDump(buf, length, 16);

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
   Verbose("#CD: %d sessions\n", dh->sessions);

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
   Verbose("#CD: CD medium detected, type: %s\n", dh->typedescr);
   return TRUE;
}

/*
 * Find out whether the drive can return raw sectors with
 * uncorrected read errors. Depending on override,
 * mode is or'ed with existing flags (override = FALSE),
 * or written over existing flags (override = TRUE). 
 */

int SetRawMode(DeviceHandle *dh, int operator)
{  AlignedBuffer *ab = CreateAlignedBuffer(2048);
   unsigned char *buf = ab->buf;
   Sense sense;
   unsigned char cdb[16];
   unsigned char wanted_mode;
   int pll, ret;

   /*** Read mode page 1 */

   memset(cdb, 0, MAX_CDB_SIZE);
   cdb[0] = 0x5a;         /* MODE SENSE(10) */
   cdb[2] = 1;            /* Page code */
   cdb[8] = 255;          /* Allocation length */
   ret  = SendPacket(dh, cdb, 10, buf, 255, &sense, DATA_READ);

   if(ret<0) 
   {  FreeAlignedBuffer(ab);
      Verbose("\nRead mode page 01h failed: %s\n", 
	      GetSenseString(sense.sense_key, sense.asc, sense.ascq, 0));
      return FALSE;
   }

   Verbose("\nMode page 01h:\n");
   Verbose("  mode data length = %d\n", buf[0]<<8 | buf[1]);
   Verbose("  block descriptor length = %d\n", buf[6]<<8 | buf[7]);
   Verbose("  page byte 0 = %2x\n", buf[8]);
   Verbose("  page byte 1 = %2x\n", buf[9]);
   Verbose("  page byte 2 = %2x\n", buf[10]);
   Verbose("  page byte 3 = %2x\n", buf[11]);

   pll = buf[1] + 2;  /* mode data length + 2 */

   Verbose("  using mode data length %d\n", pll);

   /*** Set new raw reading mode */

   memset(cdb, 0, MAX_CDB_SIZE);
   cdb[0] = 0x55;         /* MODE SELECT(10) */
   cdb[1] = 0x10;         /* set page format (PF) bit */
   cdb[7] = 0;
   cdb[8] = pll;          /* parameter list length */

   switch(operator)       /* set new read mode */
   {  case MODE_PAGE_SET:
	 dh->previousReadMode = buf[10];
	 dh->previousRetries  = buf[11];
	 buf[10] |= Closure->rawMode;
	 if(Closure->internalAttempts >= 0)
	    buf[11] = Closure->internalAttempts;
	 break;

      case MODE_PAGE_UNSET:
	 buf[10] = dh->previousReadMode;
	 buf[11] = dh->previousRetries;
	 break;
   }

   wanted_mode = buf[10];

   ret  = SendPacket(dh, cdb, 10, buf, pll, &sense, DATA_WRITE);

   if(ret<0) 
   {  FreeAlignedBuffer(ab);
      Verbose("Setting mode page 01h failed: %s\n", 
	      GetSenseString(sense.sense_key, sense.asc, sense.ascq, 0));
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
      Verbose("Reading back mode page 1 failed: %s\n", 
	      GetSenseString(sense.sense_key, sense.asc, sense.ascq, 0));
      return FALSE;
   }

   if(buf[10] != wanted_mode)
   {  FreeAlignedBuffer(ab);
      Verbose("Setting raw mode failed: %2x instead of %2x\n", buf[10], wanted_mode);
      return FALSE;
   }

   dh->currentReadMode = buf[10];
   FreeAlignedBuffer(ab);
   return TRUE;
}

/*
 * Find out whether the drive can do C2 scans.
 * Done by empirically trying a READ CD; maybe we should examine the drive
 * feature codes instead. 
 */

int TestC2Scan(DeviceHandle *dh)
{  Sense *sense = &dh->sense;
   unsigned char cdb[MAX_CDB_SIZE];
   RawBuffer *rb;
   int lba = 0;
   int ret;
   
   if(!(rb=dh->rawBuffer))  /* sanity check: No C2 scans without RAW reading */
      return FALSE;
   
   memset(cdb, 0, MAX_CDB_SIZE);
   cdb[0]  = 0xbe;         /* READ CD */

   switch(dh->subType)     /* Expected sector type */
   {  case DATA1:          /* data mode 1 */ 
        cdb[1] = 2<<2; 
	cdb[9] = 0xba;    /* we want Sync + Header + User data + EDC/ECC + C2 */
	break;  

      case XA21:           /* xa mode 2 form 1 */
	cdb[1] = 4<<2; 
	cdb[9] = 0xfc;
	break;  
   }

   cdb[2]  = (lba >> 24) & 0xff;
   cdb[3]  = (lba >> 16) & 0xff;
   cdb[4]  = (lba >>  8) & 0xff;
   cdb[5]  = lba & 0xff;
   cdb[6]  = 0;        /* number of sectors to read (3 bytes) */
   cdb[7]  = 0;  
   cdb[8]  = 1;        /* read 1 sector */

   cdb[10] = 0;        /* reserved stuff */
   cdb[11] = 0;        /* no special wishes for the control byte */

   ret = SendPacket(dh, cdb, 12, rb->workBuf->buf, CD_RAW_C2_SECTOR_SIZE, sense, DATA_READ);

   Verbose("C2 scan probing: %s\n", 
	   GetSenseString(sense->sense_key, sense->asc, sense->ascq, 0));

   if(!ret || sense->sense_key != 5)  /* good status or error != illegal request means C2 works */
   {  rb->sampleSize = CD_RAW_C2_SECTOR_SIZE;
      return TRUE;
   }
   else 
   {  rb->sampleSize = CD_RAW_SECTOR_SIZE; 
      return FALSE;
   }
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

   result = buf[4] & 0x11;
   FreeAlignedBuffer(ab);
   
   return result;
}

/*
 * See whether a sector lies within the user area.
 */

static int check_sector(DeviceHandle *dh, GString *msg_out, guint64 sector, int n_sectors)
{  AlignedBuffer *scratch = CreateAlignedBuffer(MAX_CLUSTER_SIZE);
   int status,result;
   char *msg;

   if(sector<2) return 4;

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
   int implausible = FALSE;

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

   /*** Validate capacity */

   if(dh->mainType == CD && read_capacity > MAX_CDR_SIZE)
      implausible = TRUE;

   if(dh->mainType == DVD && dh->layers == 1 && read_capacity > MAX_DVD_SL_SIZE)
      implausible = TRUE;

   if(dh->mainType == DVD && dh->layers == 2 && read_capacity > MAX_DVD_DL_SIZE)
      implausible = TRUE;

   if(implausible)
   {  LogWarning(_("READ CAPACITY: implausible medium size, %lld sectors\n"), 
		 (gint64)read_capacity);
      read_capacity = 0;
   }

   /*** If RS02 header search is enabled and we can find an appropriate header,
	use it as an authoritative source for the medium size. */

   if(Closure->querySize >= 2)
   {  if(dh->rs02Size <= 0)
      {  gint64 last_sector = MAX(read_capacity, dh->userAreaSize);
      
	 if(last_sector > 0)
	    dh->rs02Size = MediumLengthFromRS02(dh, last_sector);
      }
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

   /* BD drives have neither dh->userAreaSize nor will the following heuristics
      works as unformatted sectors can be always read. Stick with READ CAPACITY. */

   if(dh->mainType == BD)
   {  Verbose("BD medium - using size from READ CAPACITY: %lld sectors\n", read_capacity+1);
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

/***
 *** Optional drive settings
 ***/

/*
 * Spin up drive.
 * Most drive give a *beep* about sending the START STOP CDB,
 * so we simply nudge them with reading request until the spin up
 * time is over. Pathetic ;-)
 */

void SpinupDevice(DeviceHandle *dh)
{  AlignedBuffer *ab;
   GTimer *timer;
   gint64 s;

   if(!Closure->spinupDelay)
      return;

   PrintCLI(_("Waiting %d seconds for drive to spin up...\n"), Closure->spinupDelay);
   
   ab = CreateAlignedBuffer(MAX_CLUSTER_SIZE);

   timer = g_timer_new();
   g_timer_start(timer);

   for(s=0; ;s+=dh->clusterSize)
   {  int status;
      double elapsed;
      gulong ignore;

      if(s>=dh->sectors) return;
 
      status = ReadSectorsFast(dh, ab->buf, s, dh->clusterSize);
      if(status) return;

      elapsed = g_timer_elapsed(timer, &ignore);
      if(elapsed > Closure->spinupDelay)
	break;
   }

   FreeAlignedBuffer(ab);
   g_timer_destroy(timer);
}

/*
 * Load/eject the medium
 */

void LoadMedium(DeviceHandle *dh, int load)
{  Sense sense;
   unsigned char cmd[MAX_CDB_SIZE];

   /* Try to load or eject the medium */

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0x1b;                /* START STOP */
   cmd[4] = load ? 0x03 : 0x02;  /* LOEJ=1; START=load/eject */

   if(SendPacket(dh, cmd, 12, NULL, 0, &sense, DATA_READ)<0
      && (sense.asc != 0x53 || sense.ascq != 0x02))
   {
      PrintLog(_("%s\nCould not load/unload the medium.\n"), 
		 GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));
      return;
   }

   if(load) return;

   /* Some drives lock the tray when it was closed via START STOP.
      That sucks especially under newer Linux kernels as we need to
      be root to unlock it. Try anyways; maybe this changes in
      future kernels. */

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0x1e;                /* PREVENT ALLOW MEDIUM REMOVAL */

   if(SendPacket(dh, cmd, 12, NULL, 0, &sense, DATA_READ)<0)
      PrintLog(_("%s\nCould not unlock the medium.\n"), 
	       GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));

   /* Try ejecting again */

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0x1b;                /* START STOP */
   cmd[4] = 0x02;                /* LOEJ=1; START=eject */

   if(SendPacket(dh, cmd, 12, NULL, 0, &sense, DATA_READ)<0)
      PrintLog(_("%s\nCould not load/unload the medium.\n"), 
	       GetSenseString(sense.sense_key, sense.asc, sense.ascq, TRUE));
}

/*
 * Wait for the drive to become ready
 */

int TestUnitReady(DeviceHandle *dh)
{  unsigned char cmd[MAX_CDB_SIZE];
   int i;

   /*** Send TEST UNIT READY, return if drive says go */

   memset(cmd, 0, MAX_CDB_SIZE);
   cmd[0] = 0x00;     /* TEST UNIT READY */

   if(SendPacket(dh, cmd, 12, NULL, 0, &dh->sense, DATA_READ) != -1)
      return TRUE;

   /*** If no medium present, try closing the tray. */

   if(   dh->sense.sense_key == 2  /* Not Ready */
      && dh->sense.asc == 0x3a)    /* Medium not present */
      LoadMedium(dh, TRUE);

   /*** Wait 10 seconds for drives reporting that they are
        becoming ready */

   for(i=0; i<10; i++)
   {  memset(cmd, 0, MAX_CDB_SIZE);
      cmd[0] = 0x00;     /* TEST UNIT READY */

      if(SendPacket(dh, cmd, 12, NULL, 0, &dh->sense, DATA_READ) != -1)
      {  if(Closure->guiMode)
	    SetLabelText(Closure->status, "");
	 return TRUE;
      }

      if(   dh->sense.sense_key == 2   /* Not Ready */
	 && dh->sense.asc  == 0x04     /* but in process of becoming so */
	 && dh->sense.ascq == 0x01)
      {  PrintCLIorLabel(Closure->status,
			 _("Waiting 10 seconds for drive: %d\n"),9-i);

	 if(Closure->stopActions)
	    return FALSE;

	 g_usleep(G_USEC_PER_SEC);
	 continue;
      }

      break;  /* Something is wrong with the drive */
   }

   if(Closure->guiMode)
      SetLabelText(Closure->status, "");

   return FALSE;
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

/***
 *** Sector reading routines
 ***/

/*
 * Sector reading using the packet interface.
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
   unsigned char c2bit;
   RawBuffer *rb;
   int ret = -1;
   int i,s;
   int offset = 16;

   /* Sanity checks */ 

   if(!dh->rawBuffer)
   {  RememberSense(3, 255, 4);  /* RAW buffer not allocated */
      return -1;
   }

   if(nsectors > dh->clusterSize)
   {  RememberSense(3, 255, 3); /* RAW reading > cluster size not supported */
      return -1;
   }

   rb = dh->rawBuffer;
   rb->lba = lba;

   /*** Perform the raw read */

   c2bit = dh->canC2Scan ? 0x02 : 0;

   memset(cdb, 0, MAX_CDB_SIZE);
   cdb[0]  = 0xbe;         /* READ CD */
   switch(dh->subType)     /* Expected sector type */
   {  case DATA1:          /* data mode 1 */ 
        cdb[1] = 2<<2; 
	cdb[9] = 0xb8 | c2bit; /* we want Sync + Header + User data + EDC/ECC + C2 */
	offset=16;
	break;  

      case XA21:           /* xa mode 2 form 1 */
	cdb[1] = 4<<2; 
	cdb[9] = 0xf8 | c2bit;
	offset=24;
	break;  
   }

   cdb[2]  = (lba >> 24) & 0xff;
   cdb[3]  = (lba >> 16) & 0xff;
   cdb[4]  = (lba >>  8) & 0xff;
   cdb[5]  = lba & 0xff;
   cdb[6]  = 0;        /* number of sectors to read (3 bytes) */
   cdb[7]  = 0;  
   cdb[8]  = nsectors; /* read nsectors */

   cdb[10] = 0;        /* reserved stuff */
   cdb[11] = 0;        /* no special wishes for the control byte */

   /* initialize sectors with our marker */

   for(i=0, s=0; i<nsectors; i++, s+=rb->sampleSize)
     memcpy(rb->workBuf->buf+s, Closure->deadSector, 2048);

   ret = SendPacket(dh, cdb, 12, rb->workBuf->buf, nsectors*rb->sampleSize, sense, DATA_READ);
   RememberSense(sense->sense_key, sense->asc, sense->ascq);

#if 0
   if(lba==160)  /* fixme */
   rb->workBuf->buf[230]^=255;
#endif

   /* count c2 error bits */

   if(dh->canC2Scan)
   {  for(i=0, s=0; i<nsectors; i++, s+=rb->sampleSize)
	 dh->c2[i] = CountC2Errors(rb->workBuf->buf+s);
   }

   /* The drive screws up sometimes and returns a damaged sector as good. 
      When nsectors==1, this sector is still interesting for data recovery.
      We flag it as bad so that the next if() will pick it up correctly. */

   if(!ret && nsectors == 1 && !ValidateRawSector(rb, rb->workBuf->buf, "verify incoming"))
   {  ret = -1;
   }

   /*** See if defective sector reading is enabled and applicable. */

   if(ret<0 && dh->canReadDefective)
   {  
     /* Fail if multiple sectors were requested.
	Caller must retry reading single sectors. */

      if(nsectors != 1)
         return ret;

      return TryCDFrameRecovery(rb, outbuf);
   }

   /*** If we reach this, only the simple L-EC check is requested. */

   /* See if drive returned some data at all. */

   for(i=0, s=0; i<nsectors; i++, s+=rb->sampleSize)
     if(!memcmp(rb->workBuf->buf+s, Closure->deadSector, 2048))
     {  if(dh->canReadDefective)
 	  RememberSense(3, 255, 0); /* report that nothing came back */
        return -1;
     }

   /* Verify data and copy it back */

   ret = 0;
   rb->lba = lba;

   for(i=0, s=0; i<nsectors; i++, s+=rb->sampleSize)
   {  if(!ValidateRawSector(rb, rb->workBuf->buf+s, "verify outgoing"))
        return -1;

      memcpy(outbuf, rb->workBuf->buf+s+offset, 2048);
      outbuf += 2048;
      rb->lba++;
   }

   return ret;
}

/*
 * Sector reading through the device handle.
 * dh->read dispatches to one the routines above.
 */

int ReadSectors(DeviceHandle *dh, unsigned char *buf, gint64 s, int nsectors)
{  int retry,status = -1;
   int recommended_attempts = Closure->minReadAttempts;

   /* See if we are in a simulated defective area */ 

   if(dh->defects)
   {  gint64 i,idx;

     for(idx=s,i=0; i<nsectors; idx++,i++)
	//       if(GetBit(dh->defects, idx))
	if(GetBit(dh->defects, idx) && !((Random() & 15)))
       {  dh->sense.sense_key = 3;
	  dh->sense.asc       = 255;
	  dh->sense.ascq      = 255;
	  RememberSense(dh->sense.sense_key, dh->sense.asc, dh->sense.ascq);
	  return TRUE;
       }
   }

   /* Reset raw reading buffer (if there is one) */

   if(Closure->readRaw && dh->rawBuffer)
   {  ResetRawBuffer(dh->rawBuffer);
      dh->rawBuffer->recommendedAttempts = Closure->minReadAttempts;
   }

   /* Try normal read */

   for(retry=1; retry<=recommended_attempts; retry++)
   {  
      /* Dispatch between normal reader and raw reader */

      if(Closure->readRaw && dh->readRaw)
	   status = dh->readRaw(dh, buf, s, nsectors);
      else status = dh->read(dh, buf, s, nsectors);

#if 0
   if(s==160 || s>=144624)  /* fixme */
      buf[230]^=255;
#endif

      if(Closure->readRaw && dh->rawBuffer)
	recommended_attempts = dh->rawBuffer->recommendedAttempts;

      if(status)  /* current try was unsucessful */
      {  int last_key, last_asc, last_ascq;

	 if(Closure->stopActions)  /* user break */
	    return status;

	 /* Do not attempt multiple re-reads if nsectors > 1 and sectorSkip == 0
	    as these will be re-read with nsectors==1 anyways. */

	 if(dh->canReadDefective && nsectors > 1 && Closure->sectorSkip == 0)
	 {  PrintCLIorLabel(Closure->status,
			    _("Sectors %lld - %lld: %s\n"),
			    s, s+nsectors-1, GetLastSenseString(FALSE));
	    return status;
	 }

	 /* Print results of current attempt.
	    If the error was a wrong MSF in the sector, 
	    print info about the sector which was returned. */

	 GetLastSense(&last_key, &last_asc, &last_ascq);

	 if(last_key == 3 && last_asc == 255 && last_ascq == 2 && dh->rawBuffer)
	 {  unsigned char *frame = dh->rawBuffer->workBuf->buf;
	    PrintCLIorLabel(Closure->status,
			    _("Sector %lld, try %d: %s Sector returned: %d.\n"),
			    s, retry, GetLastSenseString(FALSE),
			    MSFtoLBA(frame[12], frame[13], frame[14]));
	 }
	 else
	    PrintCLIorLabel(Closure->status,
			    _("Sector %lld, try %d: %s\n"),
			    s, retry, GetLastSenseString(FALSE));

	 /* Last attempt; create failure notice */

	 if(dh->canReadDefective && retry >= recommended_attempts)
	    RememberSense(3, 255, 7);  /* Recovery failed */
      }
      else /* good return status */
      {  if(recommended_attempts > 1 && retry > 1)
	  PrintCLIorLabel(Closure->status,
			  _("Sector %lld, try %d: success\n"), s, retry);

         break;
      }
   }

   /* Update / use the defective sector cache */

   if(dh->canReadDefective && status && Closure->defectiveDump)
   {   
      if(SaveDefectiveSector(dh->rawBuffer,dh->canC2Scan))  /* any new sectors? */
      {  status = TryDefectiveSectorCache(dh->rawBuffer, buf);
	 if(status)
	    RememberSense(3, 255, 7);  /* Recovery failed */
      }
   }
   return status;
}

/*
 * Sector reading through the device handle.
 * dh->read dispatches to one the routines above.
 * No reading retries and/or raw reading are used.
 */

int ReadSectorsFast(DeviceHandle *dh, unsigned char *buf, gint64 s, int nsectors)
{  int status = -1;

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

   status = dh->read(dh, buf, s, nsectors);

   return status;
}

/*** 
 *** Open the device and query some of its properties.
 ***/

DeviceHandle* OpenAndQueryDevice(char *device)
{  DeviceHandle *dh = NULL;

   /* Open the device. */

   Verbose("# *** OpenAndQueryDevice(%s) ***\n", device);
   dh = OpenDevice(device);
   InquireDevice(dh, 0);
   Verbose("# InquireDevice returned: %s\n", dh->devinfo);

   if(!TestUnitReady(dh))
   {  if(   dh->sense.sense_key == 2  /* Not Ready */
	 && dh->sense.asc == 0x3a)    /* Medium not present */
	   Stop(_("Device %s: no medium present\n"), device);
      else Stop(_("Device %s does not become ready:\n%s\n\n"), device,
		GetSenseString(dh->sense.sense_key, dh->sense.asc, dh->sense.ascq, FALSE));
   }

#ifdef SYS_LINUX
   PrintLog(_("\nDevice: %s, %s\n"),device, dh->devinfo);
#endif

#ifdef SYS_MINGW
   PrintLog(_("\nDevice: %s (%s), %s\n"),
	    device, dh->aspiUsed ? "ASPI" : "SPTI", dh->devinfo);
#endif

   /* Query the type and fail immediately if incompatible medium is found
      so that the later tests are not derailed by the wrong medium type */

   query_type(dh, 0);
   Verbose("# query_type() returned.\n");
   
   if(dh->subType == UNSUPPORTED)
   {  char *td = alloca(strlen(dh->typedescr)+1);

      strcpy(td, dh->typedescr);
      CloseDevice(dh);
      Stop(_("This software does not support \"%s\" type media."), td);
      return NULL;
   }

   if(dh->sessions>1)
   {  int sessions = dh->sessions;

      CloseDevice(dh);
      Stop(_("This software does not support multisession (%d sessions) media."), sessions);
      return NULL;
   }

   /* Activate raw reading features if possible,
      output selected reading mode */

   Verbose("# deciding reading strategy...\n");
   switch(dh->mainType)
   {  case CD:
        PrintLog(_("Using READ CD"));
	if(Closure->readRaw)
	{  dh->rawBuffer = CreateRawBuffer(MAX_RAW_TRANSFER_SIZE);

	   dh->rawBuffer->validFP = GetMediumFingerprint(dh, dh->rawBuffer->mediumFP, FINGERPRINT_SECTOR);
	 
	   if(dh->subType == XA21)
	   {  dh->rawBuffer->dataOffset = 24;
	      dh->rawBuffer->xaMode = TRUE;
	   }
	   PrintLog(_(", RAW reading"));

	   dh->canReadDefective = SetRawMode(dh, MODE_PAGE_SET);

	   if(dh->canReadDefective)
	      PrintLog(_(", Mode page 1 ERP = %02xh"), dh->currentReadMode);

	   dh->canC2Scan = TestC2Scan(dh);
	   if(dh->canC2Scan)
	      PrintLog(_(", C2 scanning"));
	}
	PrintLog(".\n");
        break;

      default:
	PrintLog(_("Using READ(10).\n"));
	break;
   }

   /* Examine medium type */

   if(Closure->querySize >= 1)  /* parseUDF or better requested */
     ExamineUDF(dh);

   Verbose("# Calling query_size()\n");
   dh->sectors = query_size(dh);
   Verbose("# returned: %lld sectors\n", dh->sectors); 

   switch(dh->subType)
   {  case BD:
      case HDDVD:
      case DVD:
      case DATA1:
      case XA21:
      {  char *tmp;
         if(!dh->isoInfo) // || dh->rs02Size > 0)
	    tmp = g_strdup_printf(_("Medium: %s, %lld sectors%s"),
				  dh->typedescr, dh->sectors,
				  dh->rs02Size ? ", Ecc" : "");
	 else
	    tmp = g_strdup_printf(_("Medium \"%s\": %s, %lld sectors%s created %s"),
				  dh->isoInfo->volumeLabel,
				  dh->typedescr, dh->sectors,
				  dh->rs02Size ? ", Ecc," : ",",
				  dh->isoInfo->creationDate);

	 if(dh->manuID[0]) dh->mediumDescr = g_strdup_printf("%s, %s %s.", tmp, _("Manuf.-ID:"), dh->manuID);
	 else              dh->mediumDescr = g_strdup_printf("%s.", tmp);
	 g_free(tmp);
	 PrintLog("%s\n\n", dh->mediumDescr);
	 break;
      }

      default:
      {  char *td = alloca(strlen(dh->typedescr)+1);

 	 strcpy(td, dh->typedescr);
	 CloseDevice(dh);
       	 Stop(_("This software does not support \"%s\" type media."), td);
       	 return NULL;
      }
   }

   if(dh->mainType != CD && query_copyright(dh))
   {  CloseDevice(dh);
      Stop(_("This software does not support encrypted media.\n"));
   }

   /* Create the bitmap of simulated defects */

   if(Closure->simulateDefects)
     dh->defects = SimulateDefects(dh->sectors);

   return dh;
}

/*
 * Get the md5sum from the specified sector. Results are cached in the
 * DeviceHandle as multiple queries may occur.
 */

int GetMediumFingerprint(DeviceHandle *dh, guint8 *fp_out, gint64 sector)
{  AlignedBuffer *ab;
   int status;

   /* Sector already cached? */

   if(dh->fpSector == sector)
      switch(dh->fpState)
      {  case 0:    /* not read */
	    break;
	 case 1:    /* unreadable */
	    return FALSE;
	 case 2:    /* already cached */
	    memcpy(fp_out, dh->mediumFP, 16);
	    return TRUE;
      }

     ab = CreateAlignedBuffer(2048);
     status = ReadSectorsFast(dh, ab->buf, sector, 1);
   
     dh->fpSector = sector;
     if(status)  /* read error */
     {  dh->fpState = 1;
     }
     else
     {  struct MD5Context md5ctxt;
   
	dh->fpState = 2;

	MD5Init(&md5ctxt);
	MD5Update(&md5ctxt, ab->buf, 2048);
	MD5Final(dh->mediumFP, &md5ctxt);
	memcpy(fp_out, dh->mediumFP, 16);
     }

     FreeAlignedBuffer(ab);

     return dh->fpState == 2;
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
