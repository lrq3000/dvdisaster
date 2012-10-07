/*  dvdisaster: Additional error correction for optical media.
 *  Copyright (C) 2004-2012 Carsten Gnoerlich.
 *
 *  Email: carsten@dvdisaster.org  -or-  cgnoerlich@fsfe.org
 *  Project homepage: http://www.dvdisaster.org
 *
 *  This file is part of dvdisaster.
 *
 *  dvdisaster is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  dvdisaster is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with dvdisaster. If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef WITH_MUDFLAP_YES

#define _GNU_SOURCE

#if !defined(SYS_FREEBSD) /* FreeBSD declares malloc() in stdlib.h */
 #include <malloc.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>
#include <gtk/gtk.h>
#include <mf-runtime.h>

#ifdef WITH_NLS_YES
 #include <libintl.h>
 #include <locale.h>
#endif

/*
 * We're not pulling in dvdisaster.h on purpose...
 */

/***
 *** Wrapper functions for external memory (de)allocators
 ***
 * Building all libraries with mudflap support is not an option,
 * so we will wrap all needed functions to make their allocations
 * known to mudflap (otherwise we would get hundreds of false
 * positives)
 */

/*
 * gettext
 *
 * Dealing with gettext() is tricky as it returns pointers
 * to strings from static buffers whose size is not known
 * in advance. We are registering each string with a fake
 * size of 2KB, copy it into a local buffer and unregister
 * immediately. We can not keep the gettext() string ptr
 * registered as it might subsequently overlap with a fake
 * registered one; therefore the mutex and double buffering. 
 */

#ifdef WITH_NLS_YES
char *gettext_ext(const char *msgid)
{  static GStaticMutex mutex = G_STATIC_MUTEX_INIT;
   static char last[32][2048];
   static int idx=0;
   char *result;

   g_static_mutex_lock(&mutex);

   /* Get string from gettext. We can not do even a strlen()
      on it before registering, so do a fake registration
      with a 2KB size. */

   result = gettext(msgid);
   __mf_register(result, 2048, __MF_TYPE_GUESS, "gettext");

   /* Copy it over to a local static buffer. */

   idx = (idx+1)&31;
   strncpy(last[idx], result, 2048);

   /* Unregister the gettext string */

   __mf_unregister(result, 2048, __MF_TYPE_GUESS);

   g_static_mutex_unlock(&mutex);
   return last[idx];
}
#endif

/*
 * GPtrArray related
 */

GPtrArray* g_ptr_array_new_ext()
{  GPtrArray *a = g_ptr_array_new();

   __mf_register(a, sizeof(GPtrArray), __MF_TYPE_HEAP, "g_ptr_array_new");

   return a;
}

/*
 * GString functions
 */

GString* g_string_sized_new_ext(gsize size)
{  GString *gstr = g_string_sized_new(size);

   __mf_register(gstr, sizeof(GString), __MF_TYPE_GUESS, "g_string_sized_new");

   return gstr;
}

GString* g_string_truncate_ext(GString *gstr, gsize size)
{
   if(gstr->len)
      __mf_unregister(gstr->str, gstr->len+1, __MF_TYPE_GUESS);

   if(size)
      __mf_register(gstr->str, size, __MF_TYPE_GUESS, "g_string_truncate");

   return g_string_truncate(gstr, size);
}

void g_string_printf(GString *gstr, const gchar *format, ...)
{  va_list argp;

   if(gstr->len)
      __mf_unregister(gstr->str, gstr->len+1, __MF_TYPE_GUESS);

   va_start(argp, format);
   g_string_vprintf(gstr, format, argp);
   va_end(argp);
   
   __mf_register(gstr->str, gstr->len+1, __MF_TYPE_GUESS, "g_string_printf");
}

gchar* g_string_free_ext(GString *gstr, gboolean free_segment)
{
   if(free_segment && gstr->len)
      __mf_unregister(gstr->str, gstr->len+1, __MF_TYPE_GUESS);

   __mf_unregister(gstr, sizeof(GString), __MF_TYPE_GUESS);

   return g_string_free(gstr, free_segment);
}

void g_string_append_ext(GString *gstr, const char *new)
{  
   if(gstr->len)
      __mf_unregister(gstr->str, gstr->len+1, __MF_TYPE_GUESS);
      
   g_string_append(gstr, new);

   __mf_register(gstr->str, gstr->len+1, __MF_TYPE_GUESS, "g_string_append");
}


#endif /* WITH_MUDFLAP_YES */
