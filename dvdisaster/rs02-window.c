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
 *** The RS02 codec is not yet integrated into the graphical
 *** user interface.
 */

void ResetRS02EncWindow(Method *method)
{
  g_printf("RS02 codec -- ResetRS02EncWindow() called\n");
}

void ResetRS02FixWindow(Method *method)
{
  g_printf("RS02 codec -- ResetRS02FixWindow() called\n");
}

void CreateRS02EncWindow(Method *method, GtkWidget *parent)
{  GtkWidget *lab,*sep;

   lab = gtk_label_new(NULL);
   SetLabelText(GTK_LABEL(lab), "<big>Encoding with RS02 method</big>\n<i>nothing useful will happen</i>");
   gtk_misc_set_alignment(GTK_MISC(lab), 0.0, 0.0); 
   gtk_misc_set_padding(GTK_MISC(lab), 5, 0);
   gtk_box_pack_start(GTK_BOX(parent), lab, FALSE, FALSE, 3);

   sep = gtk_hseparator_new();
   gtk_box_pack_start(GTK_BOX(parent), sep, FALSE, FALSE, 0);

   sep = gtk_hseparator_new();
   gtk_box_pack_start(GTK_BOX(parent), sep, FALSE, FALSE, 0);
}

void CreateRS02FixWindow(Method *method, GtkWidget *parent)
{  GtkWidget *lab,*sep;

   lab = gtk_label_new(NULL);
   SetLabelText(GTK_LABEL(lab), "<big>Fixing with RS02 method</big>\n<i>nothing useful will happen</i>");
   gtk_misc_set_alignment(GTK_MISC(lab), 0.0, 0.0); 
   gtk_misc_set_padding(GTK_MISC(lab), 5, 0);
   gtk_box_pack_start(GTK_BOX(parent), lab, FALSE, FALSE, 3);

   sep = gtk_hseparator_new();
   gtk_box_pack_start(GTK_BOX(parent), sep, FALSE, FALSE, 0);

   sep = gtk_hseparator_new();
   gtk_box_pack_start(GTK_BOX(parent), sep, FALSE, FALSE, 0);
}

