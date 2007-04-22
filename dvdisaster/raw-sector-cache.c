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

/*
 * Write the raw dump header
 */

static void init_defective_sector_file(char *path, RawBuffer *rb, LargeFile **file, DefectiveSectorHeader *dsh)
{   int n;

    *file = LargeOpen(path, O_RDWR | O_CREAT, IMG_PERMS);

    if(!*file)
      Stop(_("Could not open %s: %s"), path, strerror(errno));

    memset(dsh, 0, sizeof(DefectiveSectorHeader));
    dsh->lba        = rb->lba;
    dsh->sectorSize = rb->sampleSize;

    if(rb->validFP)
    {  memcpy(dsh->mediumFP, rb->mediumFP, 16);
       dsh->properties |= DSH_HAS_FINGERPRINT;
    }

    n = LargeWrite(*file, dsh, sizeof(DefectiveSectorHeader));

    if(n != sizeof(DefectiveSectorHeader))
       Stop(_("Failed writing to defective sector file: %s"), strerror(errno));
}

/*
 * Open raw dump, read the header
 */

static void open_defective_sector_file(RawBuffer *rb, char *path, LargeFile **file, 
				       DefectiveSectorHeader *dsh)
{   gint64 length;
    int n;

    *file = LargeOpen(path, O_RDWR, IMG_PERMS);

    if(!*file) return;

    LargeStat(path, &length);

    n = LargeRead(*file, dsh, sizeof(DefectiveSectorHeader));
    if(n != sizeof(DefectiveSectorHeader))
       Stop(_("Failed reading from defective sector file: %s"), strerror(errno));

    dsh->nSectors = (length-sizeof(DefectiveSectorHeader))/dsh->sectorSize;
    if(dsh->nSectors*dsh->sectorSize+sizeof(DefectiveSectorHeader) != length)
       Stop(_("Defective sector file is truncated"));

    /* If the cache file has no fingerprint, add it now */

    if(!(dsh->properties & DSH_HAS_FINGERPRINT) && rb->validFP)
    {  memcpy(dsh->mediumFP, rb->mediumFP, 16);
       dsh->properties |= DSH_HAS_FINGERPRINT;

       if(!LargeSeek(*file, 0))
	  Stop(_("Failed seeking in defective sector file: %s"), strerror(errno));

       n = LargeWrite(*file, dsh, sizeof(DefectiveSectorHeader));
       
       if(n != sizeof(DefectiveSectorHeader))
	  Stop(_("Failed writing to defective sector file: %s"), strerror(errno));
    }

    /* Verify cache and medium fingerprint */

    if((dsh->properties & DSH_HAS_FINGERPRINT) && rb->validFP)
    {  if(memcmp(dsh->mediumFP, rb->mediumFP, 16))
	  Stop(_("Fingerprints of medium and defective sector cache do not match!"));
    }
}

/*
 * Append RawBuffer contents to defective sector dump
 */

int SaveDefectiveSector(RawBuffer *rb)
{  LargeFile *file;
   DefectiveSectorHeader *dsh = alloca(sizeof(DefectiveSectorHeader));
   unsigned char *cache_sectors = NULL;
   char *filename;
   gint64 length,offset;
   int count=0;
   int i,j,idx;

   if(!rb->samplesRead) 
     return 0;  /* Nothing to be done */

   /* Open cache file */

   filename = g_strdup_printf("%s/%s%lld.raw", 
			      Closure->dDumpDir, Closure->dDumpPrefix, 
			      (long long)rb->lba);

   if(!LargeStat(filename, &length))
   {  PrintCLIorLabel(Closure->status,_(" [Creating new cache file %s]\n"), filename);
      init_defective_sector_file(filename, rb, &file, dsh);
   }
   else 
   {  open_defective_sector_file(rb, filename, &file, dsh);
      if(!file)
	 Stop(_("Could not open %s: %s"), filename, strerror(errno));
   }

   /* Read already cached sectors */

   if(dsh->nSectors > 0)
   {  if(!LargeSeek(file, sizeof(DefectiveSectorHeader)))
	 Stop(_("Failed seeking in defective sector file: %s"), strerror(errno));

      cache_sectors = g_malloc(dsh->sectorSize*dsh->nSectors);
      for(i=0, idx=0; i<dsh->nSectors; i++, idx+=dsh->sectorSize)
      {  int n=LargeRead(file, cache_sectors+idx, dsh->sectorSize);
	 
	 if(n != dsh->sectorSize)
	    Stop(_("Failed reading from defective sector file: %s"), strerror(errno));
      }
   }

   /* Store sectors which are not already cached */
   
   offset = sizeof(DefectiveSectorHeader) + rb->sampleSize*dsh->nSectors;
   if(!LargeSeek(file, offset))
      Stop(_("Failed seeking in defective sector file: %s"), strerror(errno));

   for(i=0; i<rb->samplesRead; i++)
   {  int new_sector = TRUE;

      if(cache_sectors)  /* Sector already in cache? */
      {  
	 for(j=0, idx=0; j<dsh->nSectors; j++, idx+=dsh->sectorSize) 
	 {  if(!memcmp(rb->rawBuf[i], cache_sectors+idx, rb->sampleSize))
	    {  new_sector = FALSE;
	       break;
	    }
	 }
      }

      for(j=0; j<i; j++) /* Some drives return cached data after first read */
      {  if(!memcmp(rb->rawBuf[i], rb->rawBuf[j], rb->sampleSize))
	 {  new_sector = FALSE;
	    break;
	 }
      }

      if(new_sector)   /* same sector already in cache */
      {  int n=LargeWrite(file, rb->rawBuf[i], rb->sampleSize);
	 
	 if(n != rb->sampleSize)
	    Stop(_("Failed writing to defective sector file: %s"), strerror(errno));
	 count++;
      }
   }

   LargeClose(file);

   PrintCLIorLabel(Closure->status,
		   _(" [Appended %d/%d sectors to cache file %s; LBA=%lld, ssize=%d, %d sectors]\n"), 
		   count, rb->samplesRead, filename, dsh->lba, dsh->sectorSize, dsh->nSectors);

   g_free(filename);
   if(cache_sectors)
      g_free(cache_sectors);

   return count;
}

/*
 * Read sectors from the defective sector dump,
 * feed them into the raw buffer one by one
 * and retry recovery.
 */

int TryDefectiveSectorCache(RawBuffer *rb, unsigned char *outbuf)
{  DefectiveSectorHeader dsh;
   LargeFile *file;
   char *path;
   int status;
   int last_sector;
   int i;

   path = g_strdup_printf("%s/%s%lld.raw", 
			  Closure->dDumpDir, Closure->dDumpPrefix, 
			  (long long)rb->lba);
   open_defective_sector_file(rb, path, &file, &dsh);
   g_free(path);

   if(!file)   /* No cache file */
      return -1;

   /* skip sectors added in current pass */

   last_sector = dsh.nSectors - rb->samplesRead; 

   ReallocRawBuffer(rb, dsh.nSectors);

   for(i=0; i<last_sector; i++)
   {  int n;
  
      n = LargeRead(file, rb->workBuf->buf, rb->sampleSize);
      if(n != rb->sampleSize)
	 Stop(_("Failed reading from defective sector file: %s"), strerror(errno));

      status = TryCDFrameRecovery(rb, outbuf);
      if(!status) 
      {  PrintCLIorLabel(Closure->status,
			 " [Success after processing cached sector %d]\n", i+1);
	 return status; 
      }
   }

   LargeClose(file);
   
   return -1;
}

/*
 * Read sectors from the defective sector dump
 */

void ReadDefectiveSectorFile(RawBuffer *rb, char *path)
{  DefectiveSectorHeader dsh;
   LargeFile *file;
   int i;

   open_defective_sector_file(rb, path, &file, &dsh);
   if(!file)
      Stop(_("Could not open %s: %s"), path, strerror(errno));

   if(rb->lba < 0)   /* can only happen in external test cases */
      rb->lba = dsh.lba;

   ReallocRawBuffer(rb, dsh.nSectors);

   for(i=0; i<dsh.nSectors; i++)
   {  int n=LargeRead(file, rb->rawBuf[rb->samplesRead], rb->sampleSize);

      if(n != rb->sampleSize)
	 Stop(_("Failed reading from defective sector file: %s"), strerror(errno));

      rb->samplesRead++;
   }

   LargeClose(file);
}
