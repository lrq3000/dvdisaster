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

#include "udf.h"
#include "rs02-includes.h"

/***
 *** Look for ecc headers in RS02 style media
 ***/

static int read_fingerprint(DeviceHandle *dh, unsigned char *fingerprint, gint64 sector)
{  struct MD5Context md5ctxt;
   int status;

   status = ReadSectors(dh, Closure->scratchBuf, sector, 1);

   if(status) return FALSE;

   MD5Init(&md5ctxt);
   MD5Update(&md5ctxt, Closure->scratchBuf, 2048);
   MD5Final(fingerprint, &md5ctxt);

   return TRUE;
}

enum { HEADER_FOUND, TRY_NEXT_HEADER, TRY_NEXT_MODULO};

static int try_sector(DeviceHandle *dh, gint64 pos, EccHeader **ehptr)
{  EccHeader *eh;
   unsigned char fingerprint[16];
   guint32 recorded_crc;
   guint32 real_crc;
   gint64 last_fp = -1;

   /* Try reading the sector */

   Verbose("udf/try_sector: trying sector %lld\n", pos);

   if(ReadSectors(dh, Closure->scratchBuf, pos, 2))
   {  Verbose("udf/try_sector: read error, trying next header\n");
      return TRY_NEXT_HEADER;
   }

   eh = (EccHeader*)Closure->scratchBuf;

   /* See if the magic cookie is there. If not, searching within
      this modulo makes no sense for write-once media.
      However if the medium is rewriteable, there might be trash
      data behind the image. So finding an invalid sector
      does not imply there is not RS02 data present. */

   if(strncmp((char*)eh->cookie, "*dvdisaster*", 12))
   {  if(dh->rewriteable)
      {   Verbose("udf/try_sector: no cookie but rewriteable medium: skipping header\n");
	  return TRY_NEXT_HEADER;
      }
      else
      {   Verbose("udf/try_sector: no cookie, skipping current modulo\n");
	  return TRY_NEXT_MODULO;
      }
   }
   else Verbose("udf/try_sector: header at %lld: magic cookie found\n", (long long int)pos);

   /* Calculate CRC */

   recorded_crc = eh->selfCRC;

#ifdef HAVE_BIG_ENDIAN
   eh->selfCRC = 0x47504c00;
#else
   eh->selfCRC = 0x4c5047;
#endif
   real_crc = Crc32((unsigned char*)eh, sizeof(EccHeader));

   if(real_crc != recorded_crc)
   {  Verbose("udf/try_sector: CRC failed, skipping header\n");
      return TRY_NEXT_HEADER;
   }

   eh = g_malloc(sizeof(EccHeader));
   memcpy(eh, Closure->scratchBuf, sizeof(EccHeader));
#ifdef HAVE_BIG_ENDIAN
   SwapEccHeaderBytes(eh);
#endif
   eh->selfCRC = recorded_crc;

   Verbose("udf/try_sector: CRC okay\n");

   /* Compare medium fingerprint with that recorded in Ecc header */

   if(last_fp != eh->fpSector)  /* fingerprint in different sector as before? */
   {  int status;

      /* read new fingerprint */

      status = read_fingerprint(dh, fingerprint, eh->fpSector);
      last_fp = eh->fpSector;
		  
      if(!status)  /* be optimistic if fingerprint sector is unreadable */
      {  *ehptr = eh;
	 Verbose("udf/try_sector: read error in fingerprint sector\n");
	 return HEADER_FOUND;
      }

      if(!memcmp(fingerprint, eh->mediumFP, 16))  /* good fingerprint */
      {  *ehptr = eh;
	 Verbose("udf/try_sector: fingerprint okay, header good\n");
	 return HEADER_FOUND;
      }

      /* This might be a header from a larger previous session.
	 Discard it and continue */

      Verbose("udf/try_sector: fingerprint mismatch, skipping sector\n");
      g_free(eh);
   }
   
   return TRY_NEXT_HEADER;
}

EccHeader* FindHeaderInMedium(DeviceHandle *dh, gint64 max_sectors)
{  EccHeader *eh = NULL;
   Bitmap *try_next_header, *try_next_modulo;
   gint64 pos;
   gint64 header_modulo;

   /*** Quick search at fixed offsets relative to ISO filesystem */

   if(!max_sectors && dh->isoInfo)
   {  gint64 iso_size = dh->isoInfo->volumeSize; 

      /* Iso size is correct; look for root sector at +2 */

      if(try_sector(dh, iso_size, &eh) == HEADER_FOUND)
      {  Verbose("Root sector search at +0 successful\n");
	 return eh;
      }

      /* Strange stuff. Sometimes the iso size is increased by 150
	 sectors by the burning software. */

      if(try_sector(dh, iso_size-150, &eh) == HEADER_FOUND)
      {  Verbose("Root sector search at -150 successful\n");
	 return eh;
      }

      return NULL;
   }

   /*** Normal exhaustive search */

   header_modulo = (gint64)1<<62;

   try_next_header = CreateBitmap0(max_sectors);
   try_next_modulo = CreateBitmap0(max_sectors);

   Verbose("Medium rewriteable: %s\n", dh->rewriteable ? "TRUE" : "FALSE");

   /*** Search for the headers */

   while(header_modulo >= 32)
   {  pos = max_sectors & ~(header_modulo - 1);

      Verbose("FindHeaderInMedium: Trying modulo %lld\n", header_modulo);

      while(pos > 0)
      {  int result;
	
	 if(GetBit(try_next_header, pos))
	 {  Verbose("Sector %lld cached; skipping\n", pos);
	    goto check_next_header;
	 }

	 if(GetBit(try_next_modulo, pos))
	 {  Verbose("Sector %lld cached; skipping modulo\n", pos);
	     goto check_next_modulo;
	 }

	 result = try_sector(dh, pos, &eh);

	 switch(result)
	 {  case TRY_NEXT_HEADER:
	       SetBit(try_next_header, pos);
	       goto check_next_header;
	    case TRY_NEXT_MODULO:
	       SetBit(try_next_modulo, pos);
	       goto check_next_modulo;
	    case HEADER_FOUND:
	       FreeBitmap(try_next_header);
	       FreeBitmap(try_next_modulo);
	       return eh;
	 }

      check_next_header:
	pos -= header_modulo;
      }

   check_next_modulo:
      header_modulo >>= 1;
   }

   FreeBitmap(try_next_header);
   FreeBitmap(try_next_modulo);
   return NULL;
}

gint64 MediumLengthFromRS02(DeviceHandle *dh, gint64 max_size)
{  EccHeader *eh;
   RS02Layout *lay;
   gint64 real_size;

   eh = FindHeaderInMedium(dh, max_size);
   if(eh) 
   {  dh->rs02Header = eh;

      lay = CalcRS02Layout(uchar_to_gint64(eh->sectors), eh->eccBytes); 
      real_size = lay->eccSectors+lay->dataSectors;

      g_free(lay);

      return real_size;
   }

   return 0;
}


/***
 *** Rudimentary UDF and ISO filesystem parsing.
 ***
 * Information about UDF and ISO was gathered from ECMA-119,
 * ECMA-167, ECMA-167 and the UDF 2.6 standard from OSTA.org.
 * However, no claims are made to be actually conformant to any
 * of those standards.
 */

/*
 * ECMA sector indices start with 1;
 * this definition make it easier to use the standard for reading strings
 * from the buffer.
 * begin and end are the bp postions of the string start and end position.
 */

#define bp_byte(buf, idx) (buf[(idx)-1])

#define bp_short_bbo(buf, idx) (*((guint16*)(buf+idx-1)))

#define bp_long_lsb(buf, idx) (*((guint32*)(buf+idx-1)))
#define bp_long_msb(buf, idx) (((guint32)buf[(idx)-1])<<24|((guint32)buf[idx])<<16|((guint32)buf[(idx)+1])<<8|((guint32)buf[(idx)+2]))
#define bp_long_bbo(buf, idx) (*((guint32*)(buf+idx-1)))

void bp_copy(unsigned char *dest, unsigned char *src, int begin, int end)
{  int length = end-begin+1;  

  strncpy((char*)dest, (char*)src+begin-1, length);
}

void bp_string(unsigned char *dest, unsigned char *src, int begin, int end)
{  int length = end-begin+1;  

  strncpy((char*)dest, (char*)src+begin-1, length);
   dest[length] = 0;
}

void get_date(unsigned char *date, unsigned char *buf, int idx)
{  
   idx--;
   sprintf((char*)date, "dd-mm-yyyy hh:mm:ss.ss");
   bp_copy(date+ 6, buf, idx+ 1, idx+ 4);
   bp_copy(date+ 3, buf, idx+ 5, idx+ 6);
   bp_copy(date+ 0, buf, idx+ 7, idx+ 8);
   bp_copy(date+11, buf, idx+ 9, idx+10);
   bp_copy(date+14, buf, idx+11, idx+12);
   bp_copy(date+17, buf, idx+13, idx+14);
   bp_copy(date+20, buf, idx+15, idx+16);
}

static void beautify_dchar(char *dchar)
{  int idx = strlen(dchar)-1;

  while(idx>=0)
  {  if(dchar[idx] != ' ') break;
     dchar[idx--] = 0;
  }

  while(idx>0)
  {  if(dchar[idx-1] != ' ')
       dchar[idx] = tolower(dchar[idx]);
     idx--;
  }

  if(!*dchar) strcpy(dchar, _("Unnamed"));
}

/***
 *** Extract some useful information from the ISO file system
 ***/

void FreeIsoInfo(IsoInfo *ii)
{  
   g_free(ii);
}

static IsoInfo* examine_primary_vd(unsigned char *buf)
{  IsoInfo *ii = g_malloc(sizeof(IsoInfo));
   unsigned char vlabel[33];
   unsigned char str[80];
   guint32 x,vss;
   unsigned char date[32];
   
   bp_string(str, buf, 9, 40);
   Verbose("    System identifier         : |%s|\n", str);

   bp_string(vlabel, buf, 41, 72);
   Verbose("    Volume identifier         : |%s|\n", vlabel);

   vss = bp_long_bbo(buf, 81);
   Verbose("    Volume space size         : %d sectors\n", vss);

   x = bp_short_bbo(buf, 121);
   Verbose("    Volume set size           : %d\n", x);

   x = bp_short_bbo(buf, 125);
   Verbose("    Volume sequence size      : %d\n", x);

   x = bp_short_bbo(buf, 129);
   Verbose("    Logical block size        : %d\n", x);

   x = bp_long_bbo(buf, 133);
   Verbose("    Path table size           : %d bytes\n", x);

   x = bp_long_lsb(buf, 141);
   Verbose("    L-Path table location     : %d\n", x);

   x = bp_long_lsb(buf, 145);
   Verbose("    Opt L-Path table location : %d\n", x);

   x = bp_long_msb(buf, 149);
   Verbose("    M-Path table location     : %d\n", x);

   x = bp_long_msb(buf, 153);
   Verbose("    Opt M-Path table location : %d\n", x);

   /* 157 .. 190 directory record */

   /* 191 .. 318 Volume set identifier */

   /* 319 .. 446 Publisher Identifier */

   /* 447 .. 574 Data Preparer Identifier */

   /* 575 .. 702 Application Identfier */

   /* 703 .. 739 Copyright File Identifier */

   /* 740 .. 776 Abstract File Identifier */
   
   /* 777 .. 813 Bibliographic File Identifier */

   get_date(date, buf, 814);
   Verbose("    Volume creation date/time : %s\n", date);
   
   get_date(str, buf, 831);
   Verbose("    Volume modification d/t   : %s\n", str);
   
   get_date(str, buf, 848);
   Verbose("    Volume expiration d/t     : %s\n", str);

   get_date(str, buf, 865);
   Verbose("    Volume effective d/t      : %s\n", str);

   x = bp_byte(buf,882);
   Verbose("    File structure version    : %d\n", x);

   /* Extract information for IsoInfo */

   ii->volumeSize = vss;

   strcpy(ii->volumeLabel, (char*)vlabel);
   beautify_dchar(ii->volumeLabel);
   strcpy(ii->creationDate, (char*)date);
   ii->creationDate[11] = 0;
   return ii;
}

static IsoInfo* examine_iso(DeviceHandle *dh)
{  IsoInfo *ii = NULL;
   int sector,status;
   unsigned char *buf = Closure->scratchBuf;
   int vdt,vdt_ver;
   unsigned char sid[6];
      
   Verbose(" Examining the ISO file system...\n");

   /*** Iterate over the volume decriptors */

   for(sector=16; sector<32; sector++)
   {  status = ReadSectors(dh, buf, sector, 1);

      if(status)
      {  Verbose("  Sector %2d: unreadable\n", sector);
	 continue;
      }

      vdt = bp_byte(buf, 1);      /* Volume descriptor type */
      bp_string(sid, buf, 2, 6);  /* Standard identifier */
      vdt_ver = bp_byte(buf,7);   /* Volume descriptor version */

      Verbose("  Sector %2d:\n"
		 "   Volume descriptor type    = %d\n"
		 "   Volume descriptor version = %d\n"
		 "   Standard identifier       = %s\n",
		 sector, vdt, vdt_ver, sid);

      if(strncmp((char*)sid,"CD001",5))
      {  Verbose("  * Wrong or missing standard identifier.\n");
	 continue;
      }

      switch(vdt)
      {  case 0: Verbose("   -> boot record: *skipped*\n");
	         break;
	 case 1: Verbose("   -> primary volume descriptor:\n");
	         ii = examine_primary_vd(buf);
		 break;
	 case 2: Verbose("   -> supplementary volume descriptor: *skipped*\n");
	         break;		   
	 case 255: Verbose("   -> volume descriptor set terminator;\n"
			      "      end of ISO file system parsing.\n");
	           goto finished;
	           break;		   
	 default : Verbose("   -> unknown volume descriptor: *skipped*\n");
	           break;
      }
   }

finished:
   return ii;
}

/***
 *** The main wrapper for visiting the ISO and UDF file system structures
 ***/


int ExamineUDF(DeviceHandle *dh)
{
   Verbose("\nExamineUDF(%s)\n",dh->devinfo);

   dh->isoInfo = examine_iso(dh);

   Verbose(" Examining the UDF file system...\n");
   Verbose("  not yet implemented.\n\n");

   /* Try to find the root header at a fixed offset to the ISO filesystem end. */

   dh->rs02Size = MediumLengthFromRS02(dh, 0);

   return TRUE;
}
