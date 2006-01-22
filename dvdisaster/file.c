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
 *** Open the image file and fill in the respective Info struct.
 ***/

ImageInfo* OpenImageFile(EccHeader *eh, int mode)
{  ImageInfo *ii = NULL;
   gint64 img_size;
   int n,file_flags;
   gint64 fp_sector;
   unsigned char buf[2048];
   struct MD5Context md5ctxt;

   ii = g_malloc0(sizeof(ImageInfo));

   if(!(mode & PRINT_MODE))
     PrintLog(_("\nOpening %s"), Closure->imageName);

   if(!LargeStat(Closure->imageName, &img_size))
   {  if(mode & PRINT_MODE)  /* no error in print mode */ 
      {  FreeImageInfo(ii);
	 return NULL;
      }

      PrintLog(_(": not present.\n"));
      g_free(ii);
      Stop(_("Image file %s not present."),Closure->imageName);
      return NULL;
   }

   file_flags = mode & WRITEABLE_IMAGE ? O_RDWR : O_RDONLY;

   if(!(ii->file = LargeOpen(Closure->imageName, file_flags, IMG_PERMS)))
   {  if(Closure->guiMode)
      {  g_free(ii);
	 PrintLog(": %s", strerror(errno));
	 Stop(_("Can't open %s:\n%s"),Closure->imageName,strerror(errno));
      }
      else Stop(": %s", strerror(errno));
      return NULL;
   }

   ii->size = img_size;
   CalcSectors(img_size, &ii->sectors, &ii->inLast);

   if(!(mode & PRINT_MODE))
     PrintLog(_(": %lld medium sectors.\n"), ii->sectors);


   /*** Calculate md5sum of the footprint sector.
	Use sector specified by .ecc file if possible, 
	else use built-in default  */

   if(!eh)
        fp_sector = FOOTPRINT_SECTOR;
   else fp_sector = eh->fpSector;

   LargeSeek(ii->file, fp_sector*2048);

   MD5Init(&md5ctxt);
   n = LargeRead(ii->file, buf, 2048);

   if(n != 2048)
     Stop(_("could not read image sector %lld (only %d bytes):\n%s"),
	  fp_sector,n,strerror(errno));
   MD5Update(&md5ctxt, buf, n);
   MD5Final(ii->mediumFP, &md5ctxt);
   if(!memcmp(buf,Closure->deadSector,2048))  /* No sector, no md5sum */
     memset(ii->mediumFP, 0 ,16);
   LargeSeek(ii->file, 0);                     /* rewind */

   return ii;
}

void FreeImageInfo(ImageInfo *ii)
{
   if(ii->file) 
     if(!LargeClose(ii->file))
       Stop(_("Error closing image file:\n%s"), strerror(errno));

   g_free(ii);
}


/***
 *** Open the ecc file and fill in the info stuff
 ***/

EccInfo* OpenEccFile(int mode)
{  EccInfo *ei = NULL; 
   int file_flags;

   /*** Sanity check for ecc file reads */

   if(!(mode & WRITEABLE_ECC))
   {  gint64 ecc_size;

      if(!LargeStat(Closure->eccName, &ecc_size))
      {  if(!(mode & PRINT_MODE))
	   Stop(_("Can't open %s:\n%s"),Closure->eccName,strerror(errno));
	 return NULL;
      }

      if(ecc_size < 4096)
	Stop(_("Invalid or damaged ecc file"));
   }
   
   /*** Open the ecc file  */

   ei = g_malloc0(sizeof(EccInfo));
   ei->eh = g_malloc0(sizeof(EccHeader));

   file_flags = mode & WRITEABLE_ECC ? O_RDWR | O_CREAT : O_RDONLY;

   if(!(ei->file = LargeOpen(Closure->eccName, file_flags, IMG_PERMS)))
   {  FreeEccInfo(ei);
      ei = NULL;

      if(!(mode & PRINT_MODE))  /* missing ecc file no problem in print mode */
	Stop(_("Can't open %s:\n%s"),Closure->eccName,strerror(errno));
      return NULL;
   }

   if(!(mode & WRITEABLE_ECC))
   {   int n = LargeRead(ei->file, ei->eh, sizeof(EccHeader));

       if(n != sizeof(EccHeader))
       {  FreeEccInfo(ei);
	  Stop(_("Can't read ecc header:\n%s"),strerror(errno));
       }

       /*** Endian annoyance */

#ifdef HAVE_BIG_ENDIAN
       SwapEccHeaderBytes(ei->eh);
#endif

       /*** See if we can use the ecc file */

       if(strncmp((char*)ei->eh->cookie, "*dvdisaster*", 12))
       {  FreeEccInfo(ei);
	  Stop(_("Invalid or damaged ecc file"));
       }

       if(   ei->eh->methodFlags[0] != 1 || ei->eh->methodFlags[1] 
          || ei->eh->methodFlags[2] || ei->eh->methodFlags[3])
       {  FreeEccInfo(ei);
	  Stop(_("\nEcc file has been created with version 0.40.7.\n"
		 "Please use version 0.41.1 or higher to create the .ecc files.\n"
		 "I pledge not to change the file format again until version 2.0 :-)\n"));
       }

       VerifyVersion(ei->eh,1);

       ei->sectors = uchar_to_gint64(ei->eh->sectors);
       LargeSeek(ei->file, 0);
   }

   return ei;
}

void FreeEccInfo(EccInfo *ei)
{
#if 0
   if(ei->oldfile) 
     if(fclose(ei->oldfile))
       Stop(_("Error closing error correction file:\n%s"), strerror(errno));
#endif

   if(ei->file)
     if(!LargeClose(ei->file))
       Stop(_("Error closing error correction file:\n%s"), strerror(errno));

   if(ei->eh) g_free(ei->eh);
   g_free(ei);
}

/***
 *** Auxiliary functions
 ***/

/*
 * Append the given file suffix if none is already present
 */

char *ApplyAutoSuffix(char *filename, char *suffix)
{  char *out;

   if(!filename || !*filename || strrchr(filename, '.')) 
     return filename;

   out = g_strdup_printf("%s.%s",filename,suffix);
   g_free(filename);
   
   return out;
}

/*
 * See if we can handle the specified .ecc file
 */

int VerifyVersion(EccHeader *eh, int fatal)
{  
   if(Closure->version < eh->neededVersion)
   {  if(fatal)
       Stop(_("\n* This .ecc file requires dvdisaster V%d.%d.%d or higher.\n"
	      "* Please visit http://www.dvdisaster.com for an upgrade."),
	    eh->neededVersion/10000,
	    (eh->neededVersion%10000)/100,
	    eh->neededVersion%100);

      return 1;
   }

   return 0;
}

/* Remove the image file */

void UnlinkImage(GtkWidget *label)
{
   if(LargeUnlink(Closure->imageName))
   {    PrintLog(_("\nImage file %s deleted.\n"),Closure->imageName);

        if(Closure->guiMode)
	  SetLabelText(GTK_LABEL(label),
		       _("\nImage file %s deleted.\n"), Closure->imageName);
   }
   else 
   {  if(!Closure->guiMode)
       PrintLog("\n");

       ModalWarning(GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, NULL,
		    _("Image file %s not deleted: %s\n"),
		    Closure->imageName, strerror(errno));
   }
}
