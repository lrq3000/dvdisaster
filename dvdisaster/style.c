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

/*
 * Override some elements of the default style.
 * Use the "-keep-style" option to suppress.
 */

void AdjustStyle()
{
   if(Closure->keepStyle) return;

   gtk_rc_parse_string("style \"dvdisaster-style\"\n"
		       "{  GtkButton::child_displacement_x = 0\n"
		       "   GtkButton::child_displacement_y = 1\n"
		       "   GtkButton::default_border = { 0, 0, 0, 0 }\n"
		       "   GtkButton::default_outside_border = { 0, 0, 0, 0 }\n"
                       "   GtkMenuBar::shadow_type = none\n"
                       "}\n"
#if 1
      		       "class \"GtkButton\" style \"dvdisaster-style\"\n"
      		       "class \"GtkMenuBar\" style \"dvdisaster-style\"\n"
      		       "class \"GtkWidget\" style \"dvdisaster-style\"");
#endif
#if 0
		       "widget \"*.GtkButton\" style : highest \"dvdisaster-style\" "
                       "widget \"*.GtkWidget\" style : highest \"dvdisaster-style\" "
                       "widget \"*.GtkOptionMenu\" style : highest \"dvdisaster-style\" "
		       "widget \"*.GtkMenuBar\" style : highest \"dvdisaster-style\" ");

#endif
}
