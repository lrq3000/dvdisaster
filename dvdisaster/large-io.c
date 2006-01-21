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

/***
 *** Wrappers around the standard low level file system interface.
 ***
 * This is pointless for Linux, but gives us the possibility to 
 * hide differences in Linux/Windows semantics and to 
 * do split image files on VFAT in a transparent way.
 *
 * Note the different return value semantics from standard functions:
 * - LargeOpen() returns a LargeFile pointer on success and NULL otherwise;
 * - LargeRead() and LargeWrite() return the number of bytes read/written;
 * - the remaining functions return True on success or False on failure.
 *
 * Also, individual behaviour may deviate from standard functions especially
 * when in split file mode. 
 */

//#define MAX_FILE_SIZE (128*1024*1024)
//#define MAX_FILE_SIZE (8*1024*1024)
#define MAX_FILE_SIZE (2048LL*1024LL*1024LL)

#ifdef SYS_MINGW

#include <windows.h>

#define stat _stati64
#define lseek _lseeki64

/* The original windows ftruncate has off_size (32bit) */

int large_ftruncate(int fd, gint64 size)
{  gint32 handle;

   if((handle=_get_osfhandle(fd)) == -1)
     return -1;

   if(_lseeki64(fd, size, SEEK_SET) == -1)
     return -1;

   if(SetEndOfFile((HANDLE)handle) == 0)
     return -1;

   return 0;
}

#else
  #define large_ftruncate ftruncate
#endif /* SYS_MINGW */

/*
 * local aux function: opens the given segment of a large file.
 */

static int open_segment(LargeFile *lf, int n)
{  char name[lf->namelen];

   if(!lf->suffix) g_sprintf(name, "%s%02d", lf->basename, n); 
   else            g_sprintf(name, "%s%02d.%s", lf->basename, n, lf->suffix); 

   lf->fileSegment[n] = open(name, lf->flags, lf->mode);
    
   if(lf->fileSegment[n] == -1)
   {  //PrintLog("open_segment(\"%s*\", %d) failed\n", lf->basename, n);
      return FALSE;
   }

   return TRUE;
}

/*
 * Large stat replacement (queries only file size)
 */

int LargeStat(char *path, gint64 *length_return)
{  struct stat mystat;
   char name[strlen(path)+3];
   char prefix[strlen(path)+1];
   char *suffix = NULL, *c;
   int i;

   /* Unsplit file case */
   
   if(!Closure->splitFiles)
   {  if(stat(path, &mystat) == -1)
	 return FALSE;

      *length_return = mystat.st_size;
      return TRUE;
   }

   /* stat() all segments and add up their sizes */

   *length_return = 0;
   strcpy(prefix, path);
   c = strrchr(prefix, '.');
   if(c)
   {  suffix = c+1;
      *c = 0;
   }

   for(i=0; i<MAX_FILE_SEGMENTS; i++)
   {  if(!suffix) g_sprintf(name, "%s%02d", prefix, i); 
      else        g_sprintf(name, "%s%02d.%s", prefix,i,suffix);

      if(stat(name, &mystat) == -1)
            return i != 0;
      else  *length_return += mystat.st_size;
   }

   return TRUE;
}

/*
 * Open a file
 */

LargeFile* LargeOpen(char *name, int flags, mode_t mode)
{  LargeFile *lf = g_malloc0(sizeof(LargeFile));
   char *c; 

#ifdef HAVE_O_LARGEFILE
   flags |= O_LARGEFILE;
#endif
#ifdef SYS_MINGW
   flags |= O_BINARY;
#endif

   /* Unsplit file case */

   if(!Closure->splitFiles)
   {
      lf->fileSegment[0] = open(name, flags, mode);

      if(lf->fileSegment[0] == -1)
      {  g_free(lf); return NULL;
      }

      LargeStat(name, &lf->size);

      return lf;
   }

   /* Prepare for using split files.
    * Note that we're only trying to open the first segment,
    * so a failure condition of LargeOpen() is weaker for segmented
    * files than for the single file case. 
    */

   lf->flags = flags;
   if(lf->flags & O_RDWR)    /* O_RDWR must imply O_CREAT here to create */
     lf->flags |= O_CREAT;   /* the additional file segments */
   lf->mode  = mode;
   lf->namelen = strlen(name+3);
   lf->basename = g_strdup(name);
   c = strrchr(lf->basename, '.');
   if(c) 
   {  lf->suffix = c+1;
      *c = 0;
   }

   if(!open_segment(lf, 0))
   {  g_free(lf); return NULL;
   }

   LargeStat(name, &lf->size);

   return lf;
}

/*
 * Seeking in large files.
 * Note: Seeking beyond the end of a split file is undefined.
 */

int LargeSeek(LargeFile *lf, gint64 pos)
{  
   /* Unsplit file case */  

   if(!Closure->splitFiles)
   {  lf->offset = pos;
      if(lseek(lf->fileSegment[0], pos, SEEK_SET) != pos)
        return FALSE;
   }

   /* Split file case */

   else
   {  gint64 seg    = pos / MAX_FILE_SIZE;
      gint64 segpos = pos - seg * MAX_FILE_SIZE; 

      if(seg >= MAX_FILE_SEGMENTS) /* Hit the maximum segment limit? */
      {  PrintLog("LargeSeek(\"%s*\", %lld [%d:%d]) out of file descriptors\n", 
		  lf->basename, pos, seg, segpos);
	   return FALSE;
      }

      /* Open the respective segment */

      if(!lf->fileSegment[seg]) 
	if(!open_segment(lf, seg))
	{  PrintLog("LargeSeek(\"%s*\", %lld [%d:%d]) failed opening segment\n", 
		    lf->basename, pos, seg, segpos);
           return FALSE;
	}

      /* lseek() within the segment */

      if(lseek(lf->fileSegment[seg], segpos, SEEK_SET) != segpos)
      {  PrintLog("LargeSeek(\"%s*\", %lld [%d:%d]) failed seeking in segment\n", 
		  lf->basename, pos, seg, segpos);
         return FALSE;
      }

      /* remember segment and offset within */

      lf->segment = seg;
      lf->offset  = segpos;
   }

   return TRUE;
}

/*
 * EOF predicate for large files.
 *
 * Note: Works only correctly for read only files!
 */

int LargeEOF(LargeFile *lf)
{  int filepos;

   if(!Closure->splitFiles)
        filepos = lf->offset;
   else filepos = MAX_FILE_SIZE * lf->segment + lf->offset; 

   return filepos >= lf->size;
}

/*
 * Reading in segmented files
 */

int LargeRead(LargeFile *lf, void *buf, size_t count)
{  int n;

   /* Simple unsegmented case */  

   if(!Closure->splitFiles)
   {  n = read(lf->fileSegment[0], buf, count);
      lf->offset += n;
      return n;
   }

   /* Segmented file case; open first segment if necessary */

   if(!lf->fileSegment[lf->segment])
     if(!open_segment(lf, lf->segment)) 
       return -1;

   /* If buffer does not cross a segment boundary,
      simply read from the current segment and return */

   if(lf->offset + count <= MAX_FILE_SIZE)
   {  n = read(lf->fileSegment[lf->segment], buf, count);
      lf->offset += n;

      /* If the segment boundary was touched,
	 wrap to next segment */

      if(lf->offset >= MAX_FILE_SIZE)
      {  lf->offset = 0;
         lf->segment++;
         
	 if(lf->fileSegment[lf->segment] && lseek(lf->fileSegment[lf->segment], 0, SEEK_SET) != 0)
         {  PrintLog("LargeRead(\"%s*\", ...) failed wrapping to next segment\n", 
		     lf->segment);
	    return -1;
	 }
      }

      return n;
   }

   /* Read is spread over two or more segments */

   else
   {  /* Handle portion coming from current segment */

      size_t first = MAX_FILE_SIZE - lf->offset;
      size_t chunk = 0;
      size_t read_in = 0;

      n = read(lf->fileSegment[lf->segment], buf, first);
      lf->offset += n;
      if(n != first) return n;

      count -= n;

      /* Handle remainder which comes from the next segments */

      while(count > 0)
      { 
	/* Open next segment */

	lf->segment++;

	if(!lf->fileSegment[lf->segment])
	{  if(!open_segment(lf, lf->segment)) 
	     return -1;
	}
	else
	{  if(lseek(lf->fileSegment[lf->segment], 0, SEEK_SET) != 0)
	   {  PrintLog("LargeRead(\"%s*\", ...) failed switching to next segment\n", 
		       lf->segment);      
	      return n;
	   }
	}

        chunk = count > MAX_FILE_SIZE ? MAX_FILE_SIZE : count; 

        read_in = read(lf->fileSegment[lf->segment], buf+n, chunk);
	n += read_in;
	count -= read_in;
	if(read_in != chunk) return n;
      }

      /* If the segment boundary was touched, wrap to next segment */

      lf->offset = read_in;

      if(lf->offset >= MAX_FILE_SIZE)
      {  lf->offset = 0;
         lf->segment++;
         
	 if(lf->fileSegment[lf->segment] && lseek(lf->fileSegment[lf->segment], 0, SEEK_SET) != 0)
         {  PrintLog("LargeRead(\"%s*\", ...) failed wrapping to next segment\n", 
		     lf->segment);
	    return -1;
	 }
      }

      return n;
   }
}

/*
 * Writing in segmented files
 */

int LargeWrite(LargeFile *lf, void *buf, size_t count)
{  int n;

   /* Simple unsegmented case */  

   if(!Closure->splitFiles)
   {  n = write(lf->fileSegment[0], buf, count);
      lf->offset += n;
      return n;
   }

   /* Segmented file case; open first segment if necessary */

   if(!lf->fileSegment[lf->segment])
     if(!open_segment(lf, lf->segment)) 
       return -1;

   /* If buffer does not cross a segment boundary,
      simply write it to the current segment and return */

   if(lf->offset + count <= MAX_FILE_SIZE)
   {  n = write(lf->fileSegment[lf->segment], buf, count);
      lf->offset += n;

      /* If the segment boundary was touched,
	 wrap to next segment */

      if(lf->offset >= MAX_FILE_SIZE)
      {  lf->offset = 0;
         lf->segment++;

	 if(lf->fileSegment[lf->segment] && lseek(lf->fileSegment[lf->segment], 0, SEEK_SET) != 0)
         {  PrintLog("LargeWrite(\"%s*\", ...) failed wrapping to next segment\n", 
		     lf->segment);
	    return -1;
	 }
      }

      return n;
   }

   /* Write is spread over two or more segments */

   else
   {  /* Handle portion going to current segment */

      size_t first = MAX_FILE_SIZE - lf->offset;
      size_t chunk = 0;
      size_t written = 0;
	
      n = write(lf->fileSegment[lf->segment], buf, first);
      lf->offset += n;
      if(n != first) return n;

      count -= n;

      /* Handle remainder which goes into the next segments */

      while(count > 0)
      { 
	/* Open next segment */

	lf->segment++;

	if(!lf->fileSegment[lf->segment])
        {  if(!open_segment(lf, lf->segment)) 
	     return -1;
	}
	else
	{  if(lseek(lf->fileSegment[lf->segment], 0, SEEK_SET) != 0)
	   {  PrintLog("LargeWrite(\"%s*\", ...) failed switching to next segment\n", 
		       lf->segment);      
	      return n;
	   }
	}

        chunk = count > MAX_FILE_SIZE ? MAX_FILE_SIZE : count; 

	written = write(lf->fileSegment[lf->segment], buf+n, chunk);
	n += written;
	count -= written;
	if(written != chunk) return n;
      }

      /* If the segment boundary was touched, wrap to next segment */

      lf->offset = written;

      if(lf->offset >= MAX_FILE_SIZE)
      {  lf->offset = 0;
         lf->segment++;
         
	 if(lf->fileSegment[lf->segment] && lseek(lf->fileSegment[lf->segment], 0, SEEK_SET) != 0)
         {  PrintLog("LargeWrite(\"%s*\", ...) failed wrapping to next segment\n", 
		     lf->segment);
	    return -1;
	 }
      }

      return n;
   }
}

/*
 * Large file closing
 */

int LargeClose(LargeFile *lf)
{  int result = TRUE;

   /* Simple unsegmented case */  

   if(!Closure->splitFiles)
     result = (close(lf->fileSegment[0]) == 0);

   /* Segmented case */
   else
   {  int i;

      for(i=0; i<MAX_FILE_SEGMENTS; i++)
      {  if(lf->fileSegment[i] && close(lf->fileSegment[i]) != 0)
	 {  result = FALSE;
	    PrintLog("LargeClose(\"%s*\") failed closing segment %d\n", lf->basename, i); 
	 }
      }
   }

   /* Free the LargeFile struct and return results */

   if(lf->basename) g_free(lf->basename);
   g_free(lf);

   return result;
}

/*
 * Large file truncation
 */

int LargeTruncate(LargeFile *lf, gint64 length)
{
   /* Simple unsegmented case */  

   if(!Closure->splitFiles)
     return large_ftruncate(lf->fileSegment[0], length) == 0;

   /* Segmented case; first truncate the last segment */

   else
   {  gint64 seg    = length / MAX_FILE_SIZE;
      gint64 seglen = length - seg * MAX_FILE_SIZE; 
      int i;

      if(!lf->fileSegment[seg])
      {  if(!open_segment(lf, seg)) 
	    return FALSE;
      }

      if(large_ftruncate(lf->fileSegment[seg], seglen) != 0)
	return FALSE;

      /* In case of large truncation, close and delete excess segments */

      for(i=seg+1; i<MAX_FILE_SEGMENTS; i++)
      {  char name[lf->namelen];

	 close(lf->fileSegment[i]);   /* no need for error testing */
	 if(!lf->suffix) g_sprintf(name, "%s%02d", lf->basename, i); 
	 else            g_sprintf(name, "%s%02d.%s", lf->basename, i, lf->suffix); 
	 unlink(name);
      }
   }

   return TRUE;
}

/*
 * Large file unlinking
 */

int LargeUnlink(char *path)
{  char name[strlen(path)+3];
   char prefix[strlen(path)+1];
   char *suffix = NULL, *c;
   int i;

   /* Simple unsegmented case */  

   if(!Closure->splitFiles)
     return unlink(path) == 0;

   /* Segmented case. This will unlink name00..name99 */

   strcpy(prefix, path);
   c = strrchr(prefix, '.');
   if(c)
   {  suffix = c+1;
      *c = 0;
   }

   for(i=0; i<MAX_FILE_SEGMENTS; i++)
   {  if(!suffix) g_sprintf(name, "%s%02d", prefix, i); 
      else        g_sprintf(name, "%s%02d.%s", prefix, i, suffix);

      if(unlink(name) == -1)
	return i != 0;
   }

   return TRUE;
}
