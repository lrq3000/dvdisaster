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
 *** Read an image sector from the .iso file.
 ****
 * Reading sectors beyond lay->protectedSectors always returns a zero padding sector.
 * TODO: Graceful handling of truncated images
 */

void RS02ReadSector(ImageInfo *ii, RS02Layout *lay, unsigned char *buf, gint64 s)
{  int n,expected;

  /* Padding sector for ecc calculation */  

  if(s >= lay->protectedSectors)
  {  memset(buf, 0, 2048);
     return;
  }

  /* There is a circular dependence between the first EccHeader
     and the error correction because of the eccSum.
     Simply return a null sector instead. */

  if(   s == lay->firstEccHeader
     || s == lay->firstEccHeader + 1)
  {  memset(buf, 0, 2048);
     return;
  }

  /* Read a real sector */

  if(!LargeSeek(ii->file, (gint64)(2048*s)))
    Stop(_("Failed seeking to sector %lld in image: %s"),
	 s, strerror(errno));

  /* Prepare for short reads at the last image sector.
     Doesn't happen for CD and DVD media, but perhaps for future media? */

  if(s < ii->sectors-1) expected = 2048;
  else  
  {  memset(buf, 0, 2048);
     expected = ii->inLast;
  }

  /* Finally, read the sector */

  n = LargeRead(ii->file, buf, expected);
  if(n != expected)
    Stop(_("Failed reading sector %lld in image: %s"),s,strerror(errno));
}

/***
 *** Calculate position of n-th Ecc sector of the given slice in the image.
 ***/

gint64 RS02EccSectorIndex(RS02Layout *lay, gint64 slice, gint64 n)
{  gint64 ecc_idx;
   gint64 fr,base;
   gint64 s,nh;

   /* Index of ecc sectors if numbering were continuos and starting from 0 */

   ecc_idx = slice*lay->sectorsPerLayer + n;

   /* Index of first Ecc header which is interleaved with ecc sectors */

   fr = (lay->protectedSectors + lay->headerModulo - 1) / lay->headerModulo;
   fr *= lay->headerModulo;

   /* Number of ecc sectors before first interleaved Ecc header */

   base = fr - lay->protectedSectors;

   if(ecc_idx < base)
     return lay->protectedSectors + ecc_idx;

   /* Calculate index of ecc sectors with interleaved headers taken into account */

   s = fr+2;             /* first available ecc sector behind first interleaved header */
   ecc_idx -= base;      /* number of ecc sectors behind first interleaved header */
   nh = ecc_idx/(lay->headerModulo-2); /* number of interleaved headers crossed */
   s += ecc_idx;         /* add ecc sector index */
   s += 2*nh;            /* add two for each interleaved header crossed */ 
   return s;
}

/***
 *** Calculation of the image layout
 ***/

#define CDR_SIZE (351*1024)
#define DVD_SL 	 2295104  /* DVD+R/RW size used at least common denominator */
#define DVD_DASH 2294922  /* DVD-R/RW, 2,298,496 seems to be more common */
#define DVD_DL 	 4171712  /* also seen: 4148992 4173824  */

RS02Layout *CalcRS02Layout(gint64 data_sectors, int requested_roots)
{  RS02Layout *lay = g_malloc0(sizeof(RS02Layout));
   guint64 medium_capacity;
   guint64 ecc_area;

   /* If no medium size is given by the user,
      pick the smallest possible among CDR, single layer DVD and two layer DVD. */

   if(Closure->mediumSize)
     medium_capacity = Closure->mediumSize;
   else
   {  if(data_sectors < CDR_SIZE)
            medium_capacity = CDR_SIZE; /* 80min CDR */
      else if(data_sectors < DVD_SL)
            medium_capacity = DVD_SL;   /* Single layered DVD */
      else  medium_capacity = DVD_DL;   /* Double layered DVD */
   }

   lay->dataSectors      = data_sectors;
   lay->firstEccHeader   = lay->dataSectors;
   lay->crcSectors       = (sizeof(guint32)*lay->dataSectors+2047)/2048;
   lay->protectedSectors = lay->dataSectors + 2 + lay->crcSectors; /* two sectors for header */

   /* Calculate starting value for the redundancy */

   if(requested_roots > 0)
      lay->nroots = requested_roots;
   else
   {  lay->rsSectors        = medium_capacity - lay->protectedSectors;     /* just to start */
      lay->nroots           = (FIELDMAX*lay->rsSectors) / medium_capacity; /* iteration below */
   }

   if(lay->nroots > 170)   /* Cap redundancy to 200% */
     lay->nroots = 170;

   /* Calculate the header repeat value so that 
      we get about 20 to 40 copies of the header in the ecc section. */

   lay->headerModulo = 32;
   lay->ndata        = FIELDMAX-lay->nroots;
   ecc_area = lay->nroots * ((lay->protectedSectors + lay->ndata - 1) / lay->ndata); 

   while(ecc_area / lay->headerModulo > 40)
     lay->headerModulo <<= 1;

   /* Now assemble everything together and make sure it fits on the medium */

   while(lay->nroots > 7)
   {  gint64 first_repeat;  /* first header which is interleaved with ecc sectors */
      gint64 interleaved;   /* number of ecc sectors after first header */

      lay->ndata             = FIELDMAX-lay->nroots;
      lay->rsSectors         = lay->nroots * ((lay->protectedSectors + lay->ndata - 1) / lay->ndata); 
      first_repeat  = (lay->protectedSectors + lay->headerModulo - 1) / lay->headerModulo;
      first_repeat *= lay->headerModulo;

      interleaved  = lay->rsSectors + lay->protectedSectors - first_repeat;
      lay->headers = interleaved / (lay->headerModulo-2) + 1;

      //lay->headers  = 1 + (lay->rsSectors + lay->protectedSectors - first_repeat) / lay->headerModulo;  

      lay->eccSectors         = 2 + lay->crcSectors + lay->rsSectors + 2*lay->headers;
      lay->sectorsPerLayer    = (lay->protectedSectors + lay->ndata - 1) / lay->ndata;
      lay->firstCrcLayerIndex = (2 + lay->dataSectors) % lay->sectorsPerLayer;

      if(requested_roots > 0)
	break;

      if(lay->eccSectors + lay->dataSectors <= medium_capacity)
	break;

      lay->nroots--;
   }

   lay->redundancy = ((double)lay->nroots*100.0)/(double)lay->ndata;


   Verbose("Calculated layout for RS02 image:\n");

   Verbose("data sectors      = %lld\n", lay->dataSectors);
   Verbose("crc sectors       = %lld\n", lay->crcSectors);
   Verbose("protected sectors = %lld (incl. 2 hdr sectors)\n", lay->protectedSectors);
   Verbose("reed solomon secs = %lld (%d roots, %d data)\n", lay->rsSectors,lay->nroots,lay->ndata);
   Verbose("header repeats    = %lld (using modulo %lld)\n", lay->headers, lay->headerModulo);
   Verbose("added sectors     = %lld\n", lay->eccSectors);
   Verbose("total image size  = %lld\n", lay->eccSectors+lay->dataSectors);
   if(requested_roots > 0)
        Verbose("medium capacity   = n.a.\n");
   else Verbose("medium capacity   = %lld\n", medium_capacity);

   Verbose("\nInterleaving layout:\n");
   Verbose("%lld sectors per ecc layer\n",lay->sectorsPerLayer);
   Verbose("first layer sector with CRC data %lld (sector# %lld)\n",
	   lay->firstCrcLayerIndex, lay->dataSectors+2);
   Verbose("\n");

   return lay;
}

/***
 *** Write the RS02 headers into the image.
 ***/

void WriteRS02Headers(LargeFile *file, RS02Layout *lay, EccHeader *eh)
{  guint64 hpos;
   guint64 end = lay->eccSectors+lay->dataSectors-2;
   int n;

#ifdef HAVE_BIG_ENDIAN
   SwapEccHeaderBytes(eh);
#endif

   if(!LargeSeek(file, 2048*lay->firstEccHeader))
     Stop(_("Failed seeking to ecc header at %lld: %s\n"), lay->firstEccHeader, strerror(errno));
   
   n = LargeWrite(file, eh, sizeof(EccHeader));
   if(n != sizeof(EccHeader))
     Stop(_("Failed writing ecc header at %lld: %s\n"), lay->firstEccHeader, strerror(errno));

   hpos = (lay->protectedSectors + lay->headerModulo - 1) / lay->headerModulo;
   hpos *= lay->headerModulo;

   while(hpos < end)
   { 
      if(!LargeSeek(file, 2048*hpos))
	Stop(_("Failed seeking to ecc header at %lld: %s\n"), hpos, strerror(errno));

      n = LargeWrite(file, eh, sizeof(EccHeader));
      if(n != sizeof(EccHeader))
	Stop(_("Failed writing ecc header at %lld: %s\n"), hpos, strerror(errno));

      hpos += lay->headerModulo;
   }
}
