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
 *** Collect the available methods
 ***/

/*
 * Invite all methods for registration 
 */

void CollectMethods(void)
{
  BindMethods();
}

/*
 * All methods register by calling this 
 */

void RegisterMethod(Method *method)
{
  g_ptr_array_add(Closure->methodList, method);
}

/*
 * List the available methods
 */

void ListMethods(void)
{  char name[5];
   unsigned int i;

   PrintCLI(_("\nList of available methods:\n\n"));
   name[4] = 0;
 
   for(i=0; i<Closure->methodList->len; i++)
   {  Method *method = g_ptr_array_index(Closure->methodList, i);

      strncpy(name, method->name, 4);
      PrintCLI("%s -- %s\n",name,method->description);
   }
}

/*
 * Call the method destructors
 */

void CallMethodDestructors(void)
{  unsigned int i;

   for(i=0; i<Closure->methodList->len; i++)  
   {  Method *method = g_ptr_array_index(Closure->methodList, i);
 
      method->destroy(method);
      if(method->lastEh)
	g_free(method->lastEh);
   }
}

/***
 *** Determine methods from name or ecc information
 ***/

/*
 * Find a method by name
 */

Method *FindMethod(char *name)
{  unsigned int i;

   for(i=0; i<Closure->methodList->len; i++) 
   {  Method *method = g_ptr_array_index(Closure->methodList, i);

      if(!strncmp(method->name, name, 4))
        return method;
   }

   return NULL;
}

/*
 * Search for ecc headers in RS02 style image files
 */

EccHeader* FindHeaderInImage(char *filename)
{  EccHeader *eh = NULL;
   LargeFile *file;
   unsigned char buf[4096];
   gint64 length,sectors,pos;
   gint64 header_modulo;

   if(!LargeStat(filename, &length))
     return NULL;

   /*** TODO: This needs to search all possible header locations */

   file = LargeOpen(filename, O_RDONLY, IMG_PERMS);
   if(!file) return NULL;

   header_modulo = 4096;
   sectors = length / 2048;
   pos = sectors & ~(header_modulo - 1);

   while(pos > 0)
   {  if(LargeSeek(file, 2048*pos))
      {  int n;

	 n = LargeRead(file, buf, sizeof(EccHeader));

	 if(n != sizeof(EccHeader))
	   goto check_next;

	 eh = (EccHeader*)buf;

	 /* Medium read error in ecc header? */

	 if(   !memcmp(buf, Closure->deadSector, 2048)
	       || !memcmp(buf+2048, Closure->deadSector, 2048))
	 {  printf("header at %lld: read error\n", pos);
	    goto check_next;
	 }

	 /* See if the magic cookie is there */

	 if(!strncmp((char*)eh->cookie, "*dvdisaster*", 12))
	 {  guint32 recorded_crc = eh->selfCRC;
 	    guint32 real_crc;

	    printf("header found at %lld\n",pos);

	    eh->selfCRC = 0x4c5047;
	    real_crc = Crc32((unsigned char*)eh, sizeof(EccHeader));

	    if(real_crc == recorded_crc)
	    {  eh = g_malloc(sizeof(EccHeader));
	       memcpy(eh, buf, sizeof(EccHeader));
	       return eh;
	    }

	    goto check_next;
	 }
      }

   check_next:
     pos -= header_modulo;
   }

   LargeClose(file);
   return NULL;
}

/*
 * Find method for a given ecc file (like in RS01)
 * or augmented image (like in the RS02 image format).
 * Since locating the header is expensive in the RS02 case,
 * it is cached in the corresponding Method struct.
 */

Method *EccFileMethod(int process_error)
{  LargeFile *ecc_file = NULL;
   Method *method;
   EccHeader *eh;
   char method_name[5];
   gint64 length;

   /* First see if an ecc file is available */

   method_name[0] = 0;

   if((ecc_file = LargeOpen(Closure->eccName, O_RDONLY, 0)))
   {  EccHeader eh;
      int n;

      n = LargeRead(ecc_file, &eh, sizeof(EccHeader));
      LargeClose(ecc_file);

      if(n != sizeof(EccHeader))
	goto no_ecc_file;

      if(strncmp((char*)eh.cookie, "*dvdisaster*", 12))
	goto no_ecc_file;

      memcpy(method_name, eh.method, 4); method_name[4] = 0;

      if((method = FindMethod(method_name)))
	return method;
   }

   /* No ecc file, see if the image contains hidden ecc information */

no_ecc_file:
   if(!LargeStat(Closure->imageName, &length))
   {  if(process_error)
      {  if(Closure->guiMode)
	      CreateMessage(_("Image file %s not present.\n"), GTK_MESSAGE_ERROR, Closure->imageName, strerror(errno));
         else Stop(_("Image file %s not present.\n"), Closure->imageName, strerror(errno));
      }
      return NULL;
   }

   eh = FindHeaderInImage(Closure->imageName);

   if(eh)
   {  memcpy(method_name, eh->method, 4); method_name[4] = 0;

      if((method = FindMethod(method_name)))
      {  if(method->lastEh) g_free(method->lastEh);
	 method->lastEh = eh;

	 return method;
      }
      g_free(eh);
   }

   /* No ecc augmented image */

   if(process_error)
   {  if(Closure->guiMode)
      {  if(method_name[0])
	      CreateMessage(_("\nUnknown method %s.\n"), GTK_MESSAGE_ERROR, method_name);
         else CreateMessage(_("\nNeither ecc file nor ecc data in image found.\n"), GTK_MESSAGE_ERROR);
      }
      else
      {  if(method_name[0])
	      Stop(_("\nUnknown method %s.\n"), method_name);
         else Stop(_("\nNeither ecc file nor ecc data in image found.\n"));
      }
   }

   return NULL;
}

