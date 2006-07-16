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
#include "udf.h"

#include <time.h>

/***
 *** Debugging functions.
 ***/

/*
 * Debugging function to seed the image with random correctable errors
 */

/* RS01-style files */

static void random_error1(char *prefix, char *arg)
{  ImageInfo *ii;
   gint64 block_idx[255];
   gint64 s,si;
   int block_sel[255];
   int i,percent,last_percent = 0;
   int n_data,n_eras,n_errors;
   double eras_scale, blk_scale;
   char *cpos = NULL;

   SRandom(Closure->randomSeed);

   cpos = strchr(arg,',');
   if(!cpos) Stop(_("2nd argument is missing"));
   *cpos = 0;

   n_eras   = atoi(arg);
   n_errors = atoi(cpos+1);

   if(n_eras < 8 || n_eras > 100 || n_errors < 1|| n_errors > n_eras)
     Stop(_("Number of roots must be 8..100;\n"
	    "the number of erasures must be > 0 and less than the number of roots.\n"));

   n_data = 255-n_eras;
   eras_scale = (n_errors+1)/((double)MY_RAND_MAX+1.0);
   blk_scale = (double)n_data/((double)MY_RAND_MAX+1.0);


   /*** Open the image file */

   ii = OpenImageFile(NULL, WRITEABLE_IMAGE);

   /*** Setup block pointers */

   s = (ii->sectors+n_data-1)/n_data;

   for(si=0, i=0; i<n_data; si+=s, i++)
     block_idx[i] = si;

   PrintLog(_("\nGenerating random correctable erasures (for %d roots, max erasures = %d).\n"),n_eras, n_errors);

   /*** Randomly delete the blocks */

   for(si=0; si<s; si++)
   {  int n_erasures = (int)(eras_scale*(double)Random());

      /* Reset the block selector */

      for(i=0; i<n_data; i++)  
	block_sel[i] = 0;

      /* Randomly pick n blocks */

      for(i=0; i<n_erasures; i++)
      {  int idx;
      
         do
	 {  idx = (int)(blk_scale*(double)Random());
	 } while(block_sel[idx]);

	 block_sel[idx] = 1;
      }

      /* Delete the randomly picked blocks */

      for(i=0; i<n_data; i++)
      {  if(block_sel[i] && block_idx[i]<ii->sectors)
	  {  if(!LargeSeek(ii->file, (gint64)(2048*block_idx[i])))
	      Stop(_("Failed seeking to sector %lld in image: %s"),block_idx[i],strerror(errno));
	     if(LargeWrite(ii->file, Closure->deadSector, 2048) != 2048)
	       Stop(_("Failed writing to sector %lld in image: %s"),block_idx[i],strerror(errno));
	  }

          block_idx[i]++;
      }

      percent = (100*si)/s;
      if(last_percent != percent) 
      {  PrintProgress(_("Progress: %3d%%"),percent);
	 last_percent = percent;
      }
   }

   PrintProgress(_("Progress: 100%%\n"
	"Recover the image using the --fix option before doing another --random-errors run.\n"
	"Otherwise you'll accumulate >= %d erasures/ECC block and the image will be lost.\n"), 
	n_errors);

   FreeImageInfo(ii);
}

/* RS02-style ecc images */

static void random_error2(EccHeader *eh, char *prefix, char *arg)
{  RS02Layout *lay;
   ImageInfo *ii;
   gint64 si;
   guint64 hpos;
   guint64 end;
   guint64 header[42];
   int block_sel[255];
   int i,percent,last_percent = 0;
   int hidx,n_errors,erase_max = 0;
   double eras_scale, blk_scale, hdr_scale;

   SRandom(Closure->randomSeed);
   lay = CalcRS02Layout(uchar_to_gint64(eh->sectors), eh->eccBytes); 

   n_errors = atoi(arg);

   if(n_errors < 0)
   {  erase_max = 1;
      n_errors = -n_errors;
   }

   if(n_errors <= 0 || n_errors > eh->eccBytes)
     Stop(_("Number of erasures must be > 0 and <= %d\n"), eh->eccBytes);

   eras_scale = (n_errors+1)/((double)MY_RAND_MAX+1.0);
   blk_scale  = (double)255.0/((double)MY_RAND_MAX+1.0);

   /*** Open the image file */

   ii = OpenImageFile(NULL, WRITEABLE_IMAGE);

   PrintLog(_("\nGenerating random correctable erasures (for %d roots, max erasures = %d).\n"), eh->eccBytes, n_errors);

   /*** Randomly delete some ecc headers */

   header[0] = lay->firstEccHeader;
   hidx = 1;

   hpos = (lay->protectedSectors + lay->headerModulo - 1) / lay->headerModulo;
   hpos *= lay->headerModulo;

   end = lay->eccSectors+lay->dataSectors-2;

   while(hpos < end)  /* Calculate positions of all headers */
   { 
      header[hidx++] = hpos;
      hpos += lay->headerModulo;
   }

   /* Pick one header to remain intact.
      Currently this must be one of the repeated headers */

   hdr_scale = (double)(hidx-1)/((double)MY_RAND_MAX+1.0);
   header[(int)(hdr_scale*(double)Random())+1] = 0;
#if 1
   for(i=0; i<hidx; i++)
   {  gint64 s = header[i];
      if(s>0)
      {  if(!LargeSeek(ii->file, (gint64)(2048*s)))
	   Stop(_("Failed seeking to sector %lld in image: %s"), s, strerror(errno));
         if(LargeWrite(ii->file, Closure->deadSector, 2048) != 2048)
	   Stop(_("Failed writing to sector %lld in image: %s"), s, strerror(errno));
      }
   }
#endif
   /*** Randomly delete the blocks */

   for(si=0; si<lay->sectorsPerLayer; si++)
   {  int n_erasures = (int)(eras_scale*(double)Random());
      if(erase_max)
	n_erasures = n_errors;

      /* Reset the block selector */

      for(i=0; i<255; i++)  
	block_sel[i] = 0;

      /* Randomly pick n blocks */

      for(i=0; i<n_erasures; i++)
      {  int idx;
      
         do
	 {  idx = (int)(blk_scale*(double)Random());
	 } while(block_sel[idx]);

	 block_sel[idx] = 1;
      }

      /* Delete the randomly picked blocks */

      for(i=0; i<255; i++)
      {  if(block_sel[i])
	 {  gint64 s;
	 
	    if(i<eh->dataBytes)
	    {     s = si + i * lay->sectorsPerLayer;
	          if(s >= lay->protectedSectors)  /* exclude the padding area */ 
		    continue;                     /* respective sectors do not exist */
	    }
	    else  s = RS02EccSectorIndex(lay, i-eh->dataBytes, si);

            if(!LargeSeek(ii->file, (gint64)(2048*s)))
	       Stop(_("Failed seeking to sector %lld in image: %s"), s, strerror(errno));
	    if(LargeWrite(ii->file, Closure->deadSector, 2048) != 2048)
	       Stop(_("Failed writing to sector %lld in image: %s"), s, strerror(errno));
	  }
      }

      percent = (100*si)/lay->sectorsPerLayer;
      if(last_percent != percent) 
      {  PrintProgress(_("Progress: %3d%%"),percent);
	 last_percent = percent;
      }
   }

   PrintProgress(_("Progress: 100%%\n"
	"Recover the image using the --fix option before doing another --random-errors run.\n"
	"Otherwise you'll accumulate >= %d erasures/ECC block and the image will be lost.\n"), 
	n_errors);

   FreeImageInfo(ii);
   g_free(lay);
}

void RandomError(char *prefix, char *arg)
{  Method *method = EccFileMethod(TRUE);

   if(!strncmp(method->name, "RS01", 4))
     random_error1(prefix, arg);


   if(!strncmp(method->name, "RS02", 4))
     random_error2(method->lastEh, prefix, arg);
}

/*
 * Debugging function to simulate images with single
 * byte errors (except for faulty cabling and/or controllers,
 * this should never happen)
 */

void Byteset(char *arg)
{  ImageInfo *ii;
   gint64 s;
   int i,byte;
   char *cpos = NULL;

   /*** Open the image file */

   ii = OpenImageFile(NULL, WRITEABLE_IMAGE);

   /*** See which byte to set */

   cpos = strchr(arg,',');
   if(!cpos) Stop(_("2nd argument is missing"));
   *cpos = 0;

   s = atoi(arg);
   arg = cpos+1;

   cpos = strchr(arg,',');
   if(!cpos) Stop(_("3rd argument is missing"));
   *cpos = 0;

   i = atoi(arg);
   byte = atoi(cpos+1);

   if(s<0 || s>ii->sectors-1)
     Stop(_("Sector must be in range [0..%lld]\n"),ii->sectors-1);

   if(i<0 || i>=2048)
     Stop(_("Byte position must be in range [0..2047]"));

   if(byte<0 || byte>=256)
     Stop(_("Byte value must be in range [0..255]"));

   PrintLog(_("Setting byte %d in sector %lld to value %d.\n"), i, s, byte); 

   /*** Set the byte */
   
   s = 2048*s + i;
   
   if(!LargeSeek(ii->file, (gint64)s))
     Stop(_("Failed seeking to start of image: %s\n"),strerror(errno));

   if(LargeWrite(ii->file, &byte, 1) != 1)
     Stop(_("Could not write the new byte value"));

   FreeImageInfo(ii);
}

/*
 * Debugging function to simulate medium with unreadable sectors
 */

void Erase(char *arg)
{  ImageInfo *ii;
   gint64 start,end,s;
   char *dashpos = NULL;

   /*** Open the image file */

   ii = OpenImageFile(NULL, WRITEABLE_IMAGE);

   /*** See which sectors to erase */

   dashpos = strchr(arg,'-');
   if(dashpos)
   {  *dashpos = 0;
      start = atoi(arg);
      end  = atoi(dashpos+1);
   }
   else start = end = atoi(arg);

   if(start>end || start < 0 || end >= ii->sectors)
     Stop(_("Sectors must be in range [0..%lld].\n"),ii->sectors-1);

   PrintLog(_("Erasing sectors [%lld,%lld]\n"),start,end);

   /*** Erase them. */

   if(!LargeSeek(ii->file, (gint64)(2048*start)))
     Stop(_("Failed seeking to start of image: %s\n"),strerror(errno));

   for(s=start; s<=end; s++)
   {  int m = (end == ii->sectors-1) ? ii->inLast : 2048;
      int n = LargeWrite(ii->file, Closure->deadSector, m);

      if(n != m)
	Stop(_("Failed writing to sector %lld in image: %s"),s,strerror(errno));
   }

   /*** Clean up */

   FreeImageInfo(ii);
}

/*
 * Debugging function for truncating images
 */

void TruncateImage(char *arg)
{  ImageInfo *ii;
   gint64 end;

   /*** Open the image file */

   ii = OpenImageFile(NULL, WRITEABLE_IMAGE);

   /*** Determine last sector */

   end = atoi(arg);

   if(end >= ii->sectors)
     Stop(_("New length must be in range [0..%lld].\n"),ii->sectors-1);

   PrintLog(_("Truncating image to %lld sectors.\n"),end);

   /*** Truncate it. */

   if(!LargeTruncate(ii->file, (gint64)(2048*end)))
     Stop(_("Could not truncate %s: %s\n"),Closure->imageName,strerror(errno));

   /*** Clean up */

   FreeImageInfo(ii);
}

/*
 * Debugging function to create an ISO image filled with random numbers
 */

void RandomImage(char *image_name, char *n_sectors, int mark)
{  LargeFile *image;
   IsoHeader *ih;
   gint64 sectors,s = 0;
   int percent, last_percent = 0;
   guint32 invert;

   sectors = atoi(n_sectors);
   if(sectors < 64) sectors = 64;

   /*** Open the image file */

   LargeUnlink(image_name);

   if(!(image = LargeOpen(image_name, O_RDWR | O_CREAT, IMG_PERMS)))
     Stop(_("Can't open %s:\n%s"),image_name,strerror(errno));

   /*** Print banner */

   PrintLog(_("\nCreating random image with %lld sectors.\n\n"
	      "There is no need for permanently storing this image;\n" 
              "you can always reproduce it by calling\n"
	      "dvdisaster --debug %s %lld --random-seed %d\n\n"),
	      sectors, 
	      mark ? "--marked-image" : "--random-image",
	      sectors, Closure->randomSeed);

   if(Closure->randomSeed >= 0)
   {  SRandom(Closure->randomSeed);
      invert = 0;
   }
   else
   {  SRandom(-Closure->randomSeed);
      invert = 0xffffffff;
   }

   /*** Create and write the ISO file system.
	Otherwise some writing software will not recognize the image. */

   ih = InitIsoHeader();
   AddFile(ih, "random.data", 2048*(sectors-25));
   s  = WriteIsoHeader(ih, image);
   FreeIsoHeader(ih);

   /*** Create it */

   while(s<sectors)
   {  guint32 buf[512];
      int i=511;
      int n;

#ifdef HAVE_LITTLE_ENDIAN
      do buf[i--] = (Random32() ^ invert); while(i>=0);
#else
      do buf[i--] = SwapBytes32(Random32() ^ invert); while(i>=0);
#endif
      
      if(mark)  /* Mark the sector with its number. */
      {  int i;

	 for(i=0; i<2048; i+=128)
	   sprintf(((char*)buf)+i, "Sector  %8lld", s);
      }

      n = LargeWrite(image, buf, 2048);
      s++;

      if(n != 2048)
	Stop(_("Failed writing to sector %lld in image: %s"),s,strerror(errno));

      percent = (100*s)/sectors;
      if(last_percent != percent) 
      {  PrintProgress(_("Progress: %3d%%"),percent);
	 last_percent = percent;
      }
   }

   /*** Clean up */

   if(!LargeClose(image))
     Stop(_("Error closing image file:\n%s"), strerror(errno));
}

/*
 * Replaces the "unreadable sector" marker with zeros.
 */

void ZeroUnreadable(void)
{  ImageInfo *ii;
   unsigned char buf[2048],zeros[2048];
   gint64 s,cnt=0;
   int percent, last_percent = 0;

   ii = OpenImageFile(NULL, WRITEABLE_IMAGE);
   PrintLog(_("Replacing the \"unreadable sector\" markers with zeros.\n"));
   memset(zeros, 0, 2048);   

   for(s=0; s<ii->sectors; s++)
   {  int n = LargeRead(ii->file, buf, 2048);

      if(n != 2048)
	Stop(_("Could not read image sector %lld:\n%s\n"),s,strerror(errno));

      /* Replace the dead sector marker */

      if(!memcmp(buf,Closure->deadSector,2048)) 
      {
	if(!LargeSeek(ii->file, (gint64)(2048*s)))
	  Stop(_("Failed seeking to sector %lld in image: %s"),s,strerror(errno));

	n = LargeWrite(ii->file, zeros, 2048);

	if(n != 2048)
	  Stop(_("Failed writing to sector %lld in image: %s"),s,strerror(errno));

	cnt++;
      }

      percent = (100*s)/ii->sectors;
      if(last_percent != percent) 
      {  PrintProgress(_("Progress: %3d%%"),percent);
	 last_percent = percent;
      }
   }

   PrintProgress(_("%lld \"unreadable sector\" markers replaced.\n"), cnt);

   FreeImageInfo(ii);
}

/*
 * Debugging function to show contents of a given sector
 */

static void hexdump(unsigned char *buf, int len, int step)
{  int i,j;

   for(i=0; i<len; i+=step)
   {  g_printf("%04x: ",i);
      for(j=0; j<step; j++)
	if(i+j >= len) g_printf((j&0x07) == 0x07 ? "    " : "   ");
	else           g_printf("%02x%s", buf[i+j], (j&0x07) == 0x07 ? "  " : " ");

      for(j=0; j<step; j++)
      { if(i+j >= len) break;
	if((j&0x07) == 0x07)
	      g_printf("%c ", isprint(buf[i+j]) ? buf[i+j] : '.');
	else  g_printf("%c", isprint(buf[i+j]) ? buf[i+j] : '.');
      }
    
      g_printf("\n");
   }
}

void ShowSector(char *arg)
{  ImageInfo *ii;
   gint64 sector;
   int n;
   unsigned char buf[2048];

   /*** Open the image file */

   ii = OpenImageFile(NULL, READABLE_IMAGE);

   /*** Determine sector to show */

   sector =  atoi(arg);

   if(sector < 0 || sector >= ii->sectors)
     Stop(_("Sector must be in range [0..%lld]\n"),ii->sectors-1);

   PrintLog(_("Contents of sector %lld:\n\n"),sector);

   /*** Show it. */

   if(!LargeSeek(ii->file, (gint64)(2048*sector)))
     Stop(_("Failed seeking to sector %lld in image: %s"),sector,strerror(errno));

   n = LargeRead(ii->file, buf, 2048);
   if(n != 2048)
     Stop(_("Failed writing to sector %lld in image: %s"),sector,strerror(errno));

   hexdump(buf, 2048, 32);

   g_printf("CRC32 = %04x\n", Crc32(buf, 2048));

   /*** Clean up */

   FreeImageInfo(ii);
}

/***
 *** Send a CDB to the drive and report what happens
 ***
 * Sending ill-formed cdbs may kill your system
 * and/or damage yout drive permanently.
 *
 * Example command line call for sending an inquiry:
 *
 * ./dvdisaster --debug --send-cdb 12,00,00,00,24,00:24
 *
 * The first six bytes make up the cdb; cdbs with upto 12 bytes are possible.
 * The :24 arg is the allocation length. 
 * Note that the allocation length must match those specified in the cdb;
 * differing values may crash the system.
 */

enum {  SHIFT0, SHIFT4, ALLOC };

void SendCDB(char *cdb_raw)
{  int cdb_len = 0;
   int alloc_len = 0;
   unsigned char cdb[16];
   int mode = SHIFT4;
   int nibble=0;
   int status;
   char *c = cdb_raw;

   while(*c && cdb_len<16)
   {  if(*c == ',' || *c== ':')
      {  if(*c == ':')
	   mode = ALLOC;
	 c++; continue; 
      }

      if(*c >= '0' && *c <= '9')
	nibble = *c - '0';
      else if(*c >= 'a' && *c <= 'f')
	nibble = *c - 'a' + 10;
      else if(*c >= 'A' && *c <= 'F')
	nibble = *c - 'A' + 10;
      else Stop("illegal char '%c' in cdb \"%s\"\n",*c,cdb_raw);

      switch(mode)
      {  case SHIFT0:
	   cdb[cdb_len] |= nibble;
	   mode = SHIFT4;
	   cdb_len++;
	   break;

         case SHIFT4:
	   cdb[cdb_len] = nibble << 4;
	   mode = SHIFT0;
	   break;

         case ALLOC:
	   alloc_len = (alloc_len << 4) | nibble;
	   break;
      }

      c++;
   }

   PrintLog("\n");
   status = SendReadCDB(Closure->device, cdb, cdb_len, alloc_len);

   if(!status)
   {  g_printf("\nDrive returned:\n\n");
      hexdump(Closure->scratchBuf, alloc_len, 16);
   }
}

/***
 *** Create a bitmap of simulated defects
 ***/

Bitmap* SimulateDefects(gint64 size)
{  Bitmap *bm = CreateBitmap0(size);
   int defects = (size*Closure->simulateDefects)/100;
   
   SRandom(Closure->randomSeed);

   /* Create sequences of n sectors until the number of defects is reached. */ 

   while(defects)
   {  double scale, size_scale;
      int n, bit;

      scale = (double)defects/((double)MY_RAND_MAX+1.0);
      if(defects > 32)
	    n = (int)(scale*(double)Random());
      else  n = defects;

      size_scale = (double)(size-n)/((double)MY_RAND_MAX+1.0);
      bit = (int)(size_scale*(double)Random());

      while(n--)
      {	if(!GetBit(bm, bit))
	{  SetBit(bm, bit);
	   defects--;
	}
	bit++;
      }
   }

   return bm;
}
