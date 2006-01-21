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
 *** Creates the welcome window shown after startup.
 ***/

/*
 * The welcome window is shown first,
 * so it is convenient to initialize our GC when it is exposed.
 */

static gboolean expose_cb(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{  
   if(!Closure->drawGC)
   {  GdkColor *bg = &widget->style->bg[0];

      GdkColormap *cmap = gdk_colormap_get_system();
      Closure->drawGC = gdk_gc_new(widget->window);

      memcpy(Closure->background, bg, sizeof(GdkColor));

      gdk_colormap_alloc_color(cmap, Closure->black, FALSE, TRUE);

      Closure->white->red = Closure->white->green = Closure->white->blue = 0xffff;
      gdk_colormap_alloc_color(cmap, Closure->white, FALSE, TRUE);

      Closure->red->red   = 0xffff;
      gdk_colormap_alloc_color(cmap, Closure->red, FALSE, TRUE);

      Closure->yellow->red   = 0xffff;
      Closure->yellow->green = 0xc000;
      gdk_colormap_alloc_color(cmap, Closure->yellow, FALSE, TRUE);

      Closure->green->green = (bg->red + bg->green + bg->blue) / 3;
      gdk_colormap_alloc_color(cmap, Closure->green, FALSE, TRUE);

      Closure->darkgreen->green = 0x8000,
      gdk_colormap_alloc_color(cmap, Closure->darkgreen, FALSE, TRUE);

      Closure->blue->blue = 0xffff;
      gdk_colormap_alloc_color(cmap, Closure->blue, FALSE, TRUE);

      Closure->grid->red = bg->red-bg->red/8;
      Closure->grid->green = bg->green-bg->green/8;
      Closure->grid->blue = bg->blue-bg->blue/8;
      gdk_colormap_alloc_color(cmap, Closure->grid, FALSE, TRUE);
   }

   return TRUE;
}

/*
 * Create the window
 */

void CreateWelcomePage(GtkNotebook *notebook)
{  GtkWidget *box,*lab,*ignore;

   box = gtk_vbox_new(FALSE, 0);
   ignore = gtk_label_new("welcome_tab");
   g_signal_connect(G_OBJECT(box), "expose_event", G_CALLBACK(expose_cb), NULL);
   gtk_notebook_append_page(notebook, box, ignore);

   lab = gtk_label_new("welcome");
   gtk_box_pack_start(GTK_BOX(box), lab, FALSE, FALSE, 0);

}

