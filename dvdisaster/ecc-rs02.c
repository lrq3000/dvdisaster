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
 *** The RS02 codec is work in progress.
 ***
 * Do not use for production work; 
 * do not assume created .ecc data and images to be compatible with
 * the final release!
 */

/***
 *** Method registration
 ***/

static void destroy(Method*);


void register_rs02(void)
{  Method *method = g_malloc0(sizeof(Method));

   /*** Standard infomation and methods */ 

   strncpy(method->name, "RS02", 4);
   method->description = _("Reed-Solomon method with improved tolerance for defective ecc data"); 
   method->create  = RS02Create;
   method->fix     = RS02Fix;
   method->compare = RS02Compare;

   /*** Linkage to rs02-window.c */

   method->createCreateWindow = CreateRS02EncWindow;
   method->createFixWindow    = CreateRS02FixWindow;

   method->resetCreateWindow = ResetRS02EncWindow;
   method->resetFixWindow    = ResetRS02FixWindow;

   method->createPrefsPage   = NULL;

   /*** Linkage to rs01-compare.c */

   method->createCompareWindow = CreateRS02CompareWindow;
   method->resetCompareWindow  = ResetRS02CompareWindow;

   /*** Register ourself */

   method->destroy = destroy;

   RegisterMethod(method);
}

static void destroy(Method *method)
{  RS02Widgets *wl = (RS02Widgets*)method->widgetList;

   if(wl)
   {  
      g_free(method->widgetList);
   }
}

