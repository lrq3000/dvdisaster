/*  dvdisaster: Additional error correction for optical media.
 *  Copyright (C) 2004-2010 Carsten Gnoerlich.
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
#include "scsi-layer.h"

/***
 *** Read and buffer CRC information from RS02 file 
 ***/

CrcBuf *RS02GetCrcBuf(Image *image)
{  RS02CksumClosure *csc;
   AlignedBuffer *ab = CreateAlignedBuffer(2048);
   RS02Layout *lay;
   CrcBuf *cb;
   gint64 block_idx[256];
   guint32 *buf;
   gint64 image_sectors,crc_sector;
   gint64 s,i;
   int crc_idx, crc_valid = FALSE;

   csc = (RS02CksumClosure*)image->eccMethod->ckSumClosure;
   if(csc->lay) g_free(csc->lay);
   lay = csc->lay = CalcRS02Layout(uchar_to_gint64(image->eccHeader->sectors), image->eccHeader->eccBytes);

   image_sectors = lay->eccSectors+lay->dataSectors;
   cb = CreateCrcBuf(image_sectors);
   buf = cb->crcbuf;

   /* Initialize ecc block index pointers.
      The first CRC set (of lay->ndata checksums) relates to
      ecc block lay->firstCrcLayerIndex + 1. */

   for(s=0, i=0; i<lay->ndata; s+=lay->sectorsPerLayer, i++)
     block_idx[i] = s + lay->firstCrcLayerIndex + 1;

   crc_idx = 512;                   /* force crc buffer reload */
   crc_sector = lay->dataSectors+2; /* first crc data sector on medium */

   /* Cycle through the ecc blocks and descramble CRC sums in
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
      {  gint64 bidx = block_idx[i];

	 if(bidx < lay->dataSectors)  /* only data sectors have CRCs */
	 {  
	    /* Refill crc cache if needed */
	    
	    if(crc_idx >= 512)
	    {   crc_valid = ImageReadSectors(image, ab->buf, crc_sector++, 1);
		crc_idx = 0;
	    }

	    /* Sort crc into appropriate place */

	    if(crc_valid)
	    {  cb->crcbuf[bidx] = ((guint32*)ab->buf)[crc_idx];
	       SetBit(cb->valid, bidx);
	    }
	    crc_idx++;
	    block_idx[i]++;
	 }
      }
   }

   FreeAlignedBuffer(ab);

   return cb;
}

/***
 *** Internal checksum handling.
 ***/

void RS02ResetCksums(Image *image)
{  RS02CksumClosure *csc = (RS02CksumClosure*)image->eccMethod->ckSumClosure;

   MD5Init(&csc->md5ctxt);
   MD5Init(&csc->dataCtxt);
   MD5Init(&csc->crcCtxt);
   MD5Init(&csc->eccCtxt);
   MD5Init(&csc->metaCtxt);
}

//#define BORK 35071  //FIXME
void RS02UpdateCksums(Image *image, gint64 sector, unsigned char *buf)
{  RS02CksumClosure *csc = (RS02CksumClosure*)image->eccMethod->ckSumClosure;

   /* md5sum over full image */
   //if(sector == BORK) buf[42]++; //FIXME

   MD5Update(&csc->md5ctxt, buf, 2048);

   /* md5sum the data portion */

   if(sector < csc->lay->dataSectors)
   {  if(sector < csc->lay->dataSectors - 1)
	   MD5Update(&csc->dataCtxt, buf, 2048);
      else MD5Update(&csc->dataCtxt, buf, image->eccHeader->inLast);
   }

   /* md5sum the crc portion */
   if(sector >= csc->lay->dataSectors+2 && sector < csc->lay->protectedSectors)
      MD5Update(&csc->crcCtxt, buf, 2048);

   /* md5sum the ecc layers */
   if(sector >= csc->lay->protectedSectors)
   {  gint64 layer, n;

      RS02SliceIndex(csc->lay, sector, &layer, &n);
      if(layer != -1)  /* not an ecc header? */
      {  if(n < csc->lay->sectorsPerLayer-1)  /* not at layer end? */
	    MD5Update(&csc->eccCtxt, buf, 2048);
	 else  /* layer end; update meta md5 and skip to next layer */
	 {  guint8 sum[16];
	    MD5Update(&csc->eccCtxt, buf, 2048);
	    MD5Final(sum, &csc->eccCtxt);
	    MD5Update(&csc->metaCtxt, sum, 16);
	    MD5Init(&csc->eccCtxt);
	 }
      }
      /* maybe add ...else { check ecc header } */ 
   }
   //if(sector == BORK) buf[42]--;  //FIXME
}

char* RS02FinalizeCksums(Image *image)
{  RS02CksumClosure *csc = (RS02CksumClosure*)image->eccMethod->ckSumClosure;
   guint8 image_fp[16];
   guint8 data_md5[16],crc_md5[16],meta_md5[16];

   MD5Final(image_fp, &csc->md5ctxt);
   MD5Final(data_md5, &csc->dataCtxt);
   MD5Final(crc_md5,  &csc->crcCtxt);
   MD5Final(meta_md5, &csc->metaCtxt);

   /* Data (payload) checksum */
   
   if(memcmp(data_md5, image->eccHeader->mediumSum, 16))
   {  Verbose("BAD Data md5sum\n");
      return g_strdup(_("All sectors successfully read, but wrong data md5sum."));
   }
   else Verbose("GOOD Data md5sum\n");

   /* Crc layer checksum */

   if(memcmp(crc_md5, image->eccHeader->crcSum, 16))
   {  Verbose("BAD CRC md5sum\n");
      return g_strdup(_("All sectors successfully read, but wrong crc md5sum."));
   }
   else Verbose("GOOD CRC md5sum\n");

   /* Ecc meta checksum */

   if(memcmp(meta_md5, image->eccHeader->eccSum, 16))
   {  Verbose("BAD ECC md5sum\n");
      return g_strdup(_("All sectors successfully read, but wrong ecc md5sum."));
      
   }
   else Verbose("GOOD ECC md5sum\n");

   /* All checksums match */

   return NULL;
}

/***
 *** Read an image sector from the .iso file.
 ****
 * Reading sectors beyond lay->protectedSectors always returns a zero padding sector.
 */

void RS02ReadSector(Image *image, RS02Layout *lay, unsigned char *buf, gint64 s)
{  int n;

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

  /* Reading beyond the image returns dead sectors */

  if(s >= image->sectorSize)
  {  CreateMissingSector(buf, s, NULL, 0, NULL);
     return;
  }

  /* Read a real sector */

  if(!LargeSeek(image->file, (gint64)(2048*s)))
    Stop(_("Failed seeking to sector %lld in image: %s"),
	 s, strerror(errno));

  n = LargeRead(image->file, buf, 2048);
  if(n != 2048)
    Stop(_("Failed reading sector %lld in image: %s"),s,strerror(errno));
}

/***
 *** Calculate position of n-th Ecc sector of the given slice in the image.
 ***
 * Deprecated; use RS02SectorIndex() instead.
 */

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
 *** Calculate position of n-th sector of the given slice in the image.
 ***/

gint64 RS02SectorIndex(RS02Layout *lay, gint64 slice, gint64 n)
{  gint64 ecc_idx;
   gint64 fr,base;
   gint64 s,nh;

   /* Easy case: Sector is a data or crc sector */

   if(slice < lay->ndata)
   {  gint64 sector = slice*lay->sectorsPerLayer + n;

      if(sector < lay->protectedSectors)
	   return sector;
      else return -1;      /* padding sector (invalid)! */
   }

   /* else calculate position of ecc sector */

   slice -= lay->ndata;

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
 *** Calculate position of given sector within its Ecc slice.
 ***
 * E.g. if s = RS02SectorIndex(lay, slice, n)
 * then    RS02SliceIndex(lay, s, &slice, &n)
 * returns the slice and n values for s.
 */

void RS02SliceIndex(RS02Layout *lay, gint64 sector, gint64 *slice, gint64 *n)
{  gint64 remainder;
   gint64 first_repeat;
   gint64 base;

   /* Sector comes from data or crc section */ 

   if(sector < lay->protectedSectors)
   {  *slice = sector / lay->sectorsPerLayer; 
      *n     = sector % lay->sectorsPerLayer;
      return;
   }

   /* Position of first ecc header repeat */

   first_repeat = (lay->protectedSectors + lay->headerModulo - 1) / lay->headerModulo;
   first_repeat *= lay->headerModulo;

   /* Querying a header returns -1 for the slice 
      and the header repeat number in n */

   remainder = sector % lay->headerModulo;
   if(remainder < 2)
   {  
     *slice = -1;
      *n = (sector-first_repeat) / lay->headerModulo;
      return;
   }

   /* Sector is an ecc sector and lies before first interleaved Ecc header */

   if(sector < first_repeat)
   {  *slice = lay->ndata + (sector-lay->protectedSectors)/lay->sectorsPerLayer;
      *n = (sector - lay->protectedSectors) % lay->sectorsPerLayer;
      return;
   }

   /* Sector is an ecc sector and lies behind the first interleaved Ecc header */

   base = first_repeat - lay->protectedSectors; /* ecc sectors before first repeat */

   sector -= first_repeat;
   sector -= 2;                                 /* subtract first repeated header */
   sector -= 2*(sector/(lay->headerModulo-0));  /* and other crossed repeats      */
   sector += base;                              /* add sectors before first repeat */

   *slice = lay->ndata + sector / lay->sectorsPerLayer;
   *n     = sector % lay->sectorsPerLayer;
}

/***
 *** Calculation of the image layout
 ***/

RS02Layout *CalcRS02Layout(gint64 data_sectors, int requested_roots)
{  RS02Layout *lay = g_malloc0(sizeof(RS02Layout));
   guint64 ecc_area;

   /* If no medium size is given by the user,
      pick the smallest possible among CDR, single layer DVD and two layer DVD. */

   if(Closure->mediumSize)
     lay->mediumCapacity = Closure->mediumSize;
   else
   {  if(data_sectors < Closure->cdSize)
            lay->mediumCapacity = Closure->cdSize;   /* CDR */
      else if(data_sectors < Closure->dvdSize1)
            lay->mediumCapacity = Closure->dvdSize1; /* Single layered DVD */
      else if(data_sectors < Closure->dvdSize2)
	    lay->mediumCapacity = Closure->dvdSize2; /* Double layered DVD */
      else if(data_sectors < Closure->bdSize1)
	    lay->mediumCapacity = Closure->bdSize1;  /* Single layered BD */
      else  lay->mediumCapacity = Closure->bdSize2;  /* Double layered BD */
   }

   lay->dataSectors      = data_sectors;
   lay->firstEccHeader   = lay->dataSectors;
   lay->crcSectors       = (sizeof(guint32)*lay->dataSectors+2047)/2048;
   lay->protectedSectors = lay->dataSectors + 2 + lay->crcSectors; /* two sectors for header */

   /* See if user wants to pick a certain redundancy */

   if(!Closure->guiMode && !requested_roots && Closure->redundancy)
   {  int len = strlen(Closure->redundancy);

      switch(Closure->redundancy[len-1])
      {  case 'r':   /* pick number of roots */
	 {  char buf[len];
 
            strncpy(buf, Closure->redundancy, len-1);
	    requested_roots = atoi(buf);
	    break;
	 }
	 case '%':  /* pick redundancy directly */
	 {  char buf[len];
	    int percent;
 
            strncpy(buf, Closure->redundancy, len-1);
	    percent = atoi(buf);

	    for(requested_roots = 7; requested_roots < 171; requested_roots++)
	    {  double redundancy = ((double)requested_roots*100.0)/((double)(GF_FIELDMAX-requested_roots));
	       if(redundancy >= percent)
		  break;
	    }
	    if(requested_roots >170)
	       requested_roots = 0;
	       
	    break;
	 }
      }
   }

   /* Calculate starting value for the redundancy */

   if(requested_roots > 0)
      lay->nroots = requested_roots;
   else
   {  lay->rsSectors        = lay->mediumCapacity - lay->protectedSectors;     /* just to start */
      lay->nroots           = (GF_FIELDMAX*lay->rsSectors) / lay->mediumCapacity; /* iteration below */
   }

   if(lay->nroots > 170)   /* Cap redundancy to 200% */
     lay->nroots = 170;

   /* Calculate the header repeat value so that 
      we get about 20 to 40 copies of the header in the ecc section. */

   lay->headerModulo = 32;
   lay->ndata        = GF_FIELDMAX-lay->nroots;
   ecc_area = lay->nroots * ((lay->protectedSectors + lay->ndata - 1) / lay->ndata); 

   while(ecc_area / lay->headerModulo > 40)
     lay->headerModulo <<= 1;

   /* Now assemble everything together and make sure it fits on the medium */

   while(lay->nroots > 7)
   {  gint64 first_repeat;  /* first header which is interleaved with ecc sectors */
      gint64 interleaved;   /* number of ecc sectors after first header */

      lay->ndata             = GF_FIELDMAX-lay->nroots;
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

      if(lay->eccSectors + lay->dataSectors <= lay->mediumCapacity)
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
   else Verbose("medium capacity   = %lld\n", lay->mediumCapacity);

   Verbose("\nInterleaving layout:\n");
   Verbose("%lld sectors per ecc layer\n",lay->sectorsPerLayer);
   Verbose("first layer sector with CRC data %lld (sector# %lld)\n",
	   lay->firstCrcLayerIndex, lay->dataSectors+2);
   Verbose("\n");

   return lay;
}

/*
 * Determine expected size of image.
 */

guint64 RS02ExpectedImageSize(EccHeader *eh)
{  RS02Layout *lay;
   guint64 size;

   if(!eh) return 0;

   lay  = CalcRS02Layout(uchar_to_gint64(eh->sectors), eh->eccBytes);
   size = lay->eccSectors+lay->dataSectors;

   g_free(lay);

   return size;
}

/***
 *** Write the RS02 headers into the image.
 ***/

void WriteRS02Headers(LargeFile *file, RS02Layout *lay, EccHeader *eh)
{  guint64 hpos;
  //   guint64 end = lay->eccSectors+lay->dataSectors-2;
   guint64 end = lay->eccSectors+lay->dataSectors;
   int n;

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
