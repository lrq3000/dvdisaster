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
 *** Forward declarations
 ***/

static void redraw_curve(RS02Widgets*);
static void update_geometry(RS02Widgets*);

/***
 *** Encoding window
 ***/

void ResetRS02EncWindow(Method *method)
{  RS02Widgets *wl = (RS02Widgets*)method->widgetList;

   SetProgress(wl->encPBar1, 0, 100);
   SetProgress(wl->encPBar2, 0, 100);

   gtk_widget_hide(wl->encLabel2);
   gtk_widget_hide(wl->encPBar2);

   gtk_label_set_text(GTK_LABEL(wl->encFootline), "");
   gtk_label_set_text(GTK_LABEL(wl->encFootline2), "");
}

void CreateRS02EncWindow(Method *method, GtkWidget *parent)
{  GtkWidget *wid,*table,*pbar,*sep;
   RS02Widgets *wl;

   if(!method->widgetList)
   {  wl = g_malloc0(sizeof(RS02Widgets));
      method->widgetList = wl;
   }
   else wl = method->widgetList;

   wl->encHeadline = gtk_label_new(NULL);
   gtk_misc_set_alignment(GTK_MISC(wl->encHeadline), 0.0, 0.0); 
   gtk_misc_set_padding(GTK_MISC(wl->encHeadline), 5, 0);
   gtk_box_pack_start(GTK_BOX(parent), wl->encHeadline, FALSE, FALSE, 3);

   sep = gtk_hseparator_new();
   gtk_box_pack_start(GTK_BOX(parent), sep, FALSE, FALSE, 0);

   sep = gtk_hseparator_new();
   gtk_box_pack_start(GTK_BOX(parent), sep, FALSE, FALSE, 0);

   table = gtk_table_new(2, 2, FALSE);
   gtk_box_pack_start(GTK_BOX(parent), table, FALSE, FALSE, 30);

   wl->encLabel1 = wid = gtk_label_new(NULL);
   gtk_label_set_markup(GTK_LABEL(wid),
			_utf("<b>1. Preparing image:</b>"));
   gtk_misc_set_alignment(GTK_MISC(wid), 0.0, 0.0);
   gtk_table_attach(GTK_TABLE(table), wid, 0, 1, 0, 1, GTK_SHRINK | GTK_FILL, GTK_SHRINK, 20, 20);

   pbar = wl->encPBar1 = gtk_progress_bar_new();
   gtk_table_attach(GTK_TABLE(table), pbar, 1, 2, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND, 20, 20);

   wl->encLabel2 = wid = gtk_label_new(NULL);
   gtk_label_set_markup(GTK_LABEL(wid),
			_utf("<b>2. Creating error correction data:</b>"));
   gtk_misc_set_alignment(GTK_MISC(wid), 0.0, 0.0);
   gtk_table_attach(GTK_TABLE(table), wid, 0, 1, 1, 2, GTK_SHRINK | GTK_FILL, GTK_SHRINK, 20, 20);

   pbar = wl->encPBar2 = gtk_progress_bar_new();
   gtk_table_attach(GTK_TABLE(table), pbar, 1, 2, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND, 20, 20);

   wl->encFootline = gtk_label_new(NULL);
   gtk_misc_set_alignment(GTK_MISC(wl->encFootline), 0.0, 0.5); 
   gtk_misc_set_padding(GTK_MISC(wl->encFootline), 20, 0);
   gtk_box_pack_start(GTK_BOX(parent), wl->encFootline, FALSE, FALSE, 3);

   wl->encFootline2 = gtk_label_new(NULL);
   gtk_misc_set_alignment(GTK_MISC(wl->encFootline2), 0.0, 0.5); 
   gtk_misc_set_padding(GTK_MISC(wl->encFootline2), 20, 0);
   gtk_box_pack_start(GTK_BOX(parent), wl->encFootline2, FALSE, FALSE, 3);
}

/***
 *** Fix window
 ***/

/*
 * Set the media size and ecc capacity
 */

static gboolean set_max_idle_func(gpointer data)
{  RS02Widgets *wl = (RS02Widgets*)data;

   redraw_curve(wl);

   return FALSE;
}

void RS02SetFixMaxValues(RS02Widgets *wl, int data_bytes, int ecc_bytes, gint64 sectors)
{
   wl->dataBytes = data_bytes;
   wl->eccBytes  = ecc_bytes;
   wl->nSectors  = sectors;
   wl->fixCurve->maxX = 100;
   wl->fixCurve->maxY = ecc_bytes - (ecc_bytes % 5) + 5;

   g_idle_add(set_max_idle_func, wl);
}

/*
 * Update the corrected / uncorrected numbers
 */

static gboolean results_idle_func(gpointer data)
{  RS02Widgets *wl = (RS02Widgets*)data;

   SetLabelText(GTK_LABEL(wl->fixCorrected), _("Repaired: %lld"), wl->corrected); 
   SetLabelText(GTK_LABEL(wl->fixUncorrected), _("Unrepairable: <span color=\"red\">%lld</span>"), wl->uncorrected); 
   SetLabelText(GTK_LABEL(wl->fixProgress), _("Progress: %3d.%1d%%"), wl->percent/10, wl->percent%10);

   return FALSE;
}

void RS02UpdateFixResults(RS02Widgets *wl, gint64 corrected, gint64 uncorrected)
{
   wl->corrected = corrected;
   wl->uncorrected = uncorrected;

   g_idle_add(results_idle_func, wl);
}

/*
 * Update the error curve 
 */

static gboolean curve_idle_func(gpointer data)
{  RS02Widgets *wl = (RS02Widgets*)data;
   gint x0 = CurveX(wl->fixCurve, (double)wl->lastPercent);
   gint x1 = CurveX(wl->fixCurve, (double)wl->percent);
   gint y = CurveY(wl->fixCurve, wl->fixCurve->ivalue[wl->percent]);
   gint i;

   /*** Mark unused ecc values */

   for(i=wl->lastPercent+1; i<wl->percent; i++)
      wl->fixCurve->ivalue[i] = wl->fixCurve->ivalue[wl->percent];

   /*** Resize the Y axes if error values exceeds current maximum */

   if(wl->fixCurve->ivalue[wl->percent] > wl->fixCurve->maxY)
   {  wl->fixCurve->maxY = wl->fixCurve->ivalue[wl->percent];
      wl->fixCurve->maxY = wl->fixCurve->maxY - (wl->fixCurve->maxY % 5) + 5;

      update_geometry(wl);
      gdk_window_clear(wl->fixCurve->widget->window);
      redraw_curve(wl);
      wl->lastPercent = wl->percent;

      return FALSE;
   }

   /*** Draw the error value */

   if(wl->fixCurve->ivalue[wl->percent] > 0)
   {  gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->red);
      gdk_draw_rectangle(wl->fixCurve->widget->window,
			 Closure->drawGC, TRUE,
			 x0, y, x0==x1 ? 1 : x1-x0, wl->fixCurve->bottomY-y);
   }
   wl->lastPercent = wl->percent;

   /* Redraw the ecc capacity threshold line */

   y = CurveY(wl->fixCurve, wl->eccBytes);  
   gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->green);
   gdk_draw_line(wl->fixCurve->widget->window,
		 Closure->drawGC,
		 wl->fixCurve->leftX-6, y, wl->fixCurve->rightX+6, y);
   return FALSE;
}

/* 
 * Add one new data point 
 */

void RS02AddFixValues(RS02Widgets *wl, int percent, int ecc_max)
{
   if(percent < 0 || percent > 1000)
     return;

   wl->fixCurve->ivalue[percent] = ecc_max;
   wl->percent = percent;
   g_idle_add(curve_idle_func, wl);
}
  
/*
 * Redraw the whole curve
 */

/* Calculate the geometry of the curve and spiral */

static void update_geometry(RS02Widgets *wl)
{  
   /* Curve geometry */ 

   UpdateCurveGeometry(wl->fixCurve, "999", 20);

   /* Label positions in the foot line */

   gtk_box_set_child_packing(GTK_BOX(wl->fixFootlineBox), wl->fixCorrected,
			     TRUE, TRUE, wl->fixCurve->leftX, GTK_PACK_START);
   gtk_box_set_child_packing(GTK_BOX(wl->fixFootlineBox), wl->fixUncorrected, 
			     TRUE, TRUE, wl->fixCurve->leftX, GTK_PACK_START);
}

static void redraw_curve(RS02Widgets *wl)
{  int y;

   /* Redraw the curve */

   RedrawAxes(wl->fixCurve);
   RedrawCurve(wl->fixCurve, wl->percent, REDRAW_ICURVE);

   /* Ecc capacity threshold line */

   y = CurveY(wl->fixCurve, wl->eccBytes);  
   gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->green);
   gdk_draw_line(wl->fixCurve->widget->window,
		 Closure->drawGC,
		 wl->fixCurve->leftX-6, y, wl->fixCurve->rightX+6, y);
}

/*
 * Expose callback
 */

static gboolean expose_cb(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{  RS02Widgets *wl = (RS02Widgets*)data; 

   if(event->count) /* Exposure compression */
     return TRUE;

   update_geometry(wl);
   redraw_curve(wl);

   return TRUE;
}

void ResetRS02FixWindow(Method *method)
{  RS02Widgets *wl = (RS02Widgets*)method->widgetList;

   gtk_notebook_set_current_page(GTK_NOTEBOOK(wl->fixNotebook), 0);

   ZeroCurve(wl->fixCurve);
   RS02UpdateFixResults(wl, 0, 0);

   if(wl->fixCurve && wl->fixCurve->widget)
   {  gdk_window_clear(wl->fixCurve->widget->window);
      redraw_curve(wl);
   }

   wl->percent = 0;
   wl->lastPercent = 0;
}

/*
 * Create the Fix window contents
 */


void CreateRS02FixWindow(Method *method, GtkWidget *parent)
{  RS02Widgets *wl;
   GtkWidget *sep,*ignore,*d_area,*notebook,*hbox;

   if(!method->widgetList)
   {  wl = g_malloc0(sizeof(RS02Widgets));
      method->widgetList = wl;
   }
   else wl = method->widgetList;

   wl->fixHeadline = gtk_label_new(NULL);
   gtk_misc_set_alignment(GTK_MISC(wl->fixHeadline), 0.0, 0.0); 
   gtk_misc_set_padding(GTK_MISC(wl->fixHeadline), 5, 0);
   gtk_box_pack_start(GTK_BOX(parent), wl->fixHeadline, FALSE, FALSE, 3);

   sep = gtk_hseparator_new();
   gtk_box_pack_start(GTK_BOX(parent), sep, FALSE, FALSE, 0);

   sep = gtk_hseparator_new();
   gtk_box_pack_start(GTK_BOX(parent), sep, FALSE, FALSE, 0);

   d_area = wl->fixDrawingArea = gtk_drawing_area_new();
   gtk_box_pack_start(GTK_BOX(parent), d_area, TRUE, TRUE, 0);
   g_signal_connect(G_OBJECT (d_area), "expose_event", G_CALLBACK(expose_cb), (gpointer)wl);
   
   notebook = wl->fixNotebook = gtk_notebook_new();
   gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), FALSE);
   gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), FALSE);
   gtk_box_pack_end(GTK_BOX(parent), notebook, FALSE, FALSE, 0);

   hbox = wl->fixFootlineBox = gtk_hbox_new(TRUE, 0);

   wl->fixCorrected = gtk_label_new(NULL);
   gtk_misc_set_alignment(GTK_MISC(wl->fixCorrected), 0.0, 0.0); 
   gtk_box_pack_start(GTK_BOX(hbox), wl->fixCorrected, TRUE, TRUE, 0);

   wl->fixProgress = gtk_label_new(NULL);
   gtk_misc_set_alignment(GTK_MISC(wl->fixProgress), 0.5, 0.0); 
   gtk_box_pack_start(GTK_BOX(hbox), wl->fixProgress, TRUE, TRUE, 0);

   wl->fixUncorrected = gtk_label_new(NULL);
   gtk_misc_set_alignment(GTK_MISC(wl->fixUncorrected), 1.0, 0.0); 
   gtk_box_pack_start(GTK_BOX(hbox), wl->fixUncorrected, TRUE, TRUE, 0);

   ignore = gtk_label_new("progress_tab");
   gtk_notebook_append_page(GTK_NOTEBOOK(notebook), hbox, ignore);

   wl->fixFootline = gtk_label_new("Footline");
   gtk_misc_set_alignment(GTK_MISC(wl->fixFootline), 0.0, 0.5); 
   gtk_misc_set_padding(GTK_MISC(wl->fixFootline), 5, 0);
   ignore = gtk_label_new("footer_tab");
   gtk_notebook_append_page(GTK_NOTEBOOK(notebook), wl->fixFootline, ignore);

   wl->fixCurve  = CreateCurve(d_area, _("Errors/Ecc block"), "%d", 1000, CURVE_PERCENT);
}

/***
 *** Create the preferences page for setting redundancy etc.
 ***/

enum 
{  PREF_NROOTS = 0,
   PREF_CACHE = 1,
   PREF_ECC_SIZE = 2
};

static int cache_size[] = { 1, 2, 4, 8, 16, 32, 64, 96, 128, 192, 256, 384, 512, 768, 1024, 1536, 2048 };

static gchar* format_cb(GtkScale *scale, gdouble value, gpointer data)
{  int nroots = value;
   int ndata  = GF_FIELDMAX - nroots;
   char *label;

   if(GPOINTER_TO_INT(data) == PREF_CACHE)
     label = g_strdup_printf(_utf("%d MB of system memory"), cache_size[nroots]);
   else
     label = g_strdup_printf(_utf("%4.1f%% redundancy (%d roots)"),
			    ((double)nroots*100.0)/(double)ndata,
			    nroots);

   FORGET(label);  /* will be g_free()ed by the scale */
   return label;
}

static void scale_cb(GtkWidget *widget, gpointer data)
{  int value = gtk_range_get_value(GTK_RANGE(widget));

   Closure->cacheMB = cache_size[value];
   UpdateMethodPreferences();
}

static void toggle_cb(GtkWidget *widget, gpointer data)
{  Method *method = (Method*)data;
   RS02Widgets *wl = (RS02Widgets*)method->widgetList;
   int state  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

   if(state == TRUE)
   {  
      if(widget == wl->radio1) /* automatic */
      {  
	 gtk_widget_set_sensitive(wl->cdEntry, TRUE);
	 gtk_widget_set_sensitive(wl->dvdEntry1, TRUE);
	 gtk_widget_set_sensitive(wl->dvdEntry2, TRUE);
	 gtk_widget_set_sensitive(wl->cdButton, TRUE);
	 gtk_widget_set_sensitive(wl->dvdButton1, TRUE);
	 gtk_widget_set_sensitive(wl->dvdButton2, TRUE);
	 gtk_widget_set_sensitive(wl->cdUndoButton, TRUE);
	 gtk_widget_set_sensitive(wl->dvdUndoButton1, TRUE);
	 gtk_widget_set_sensitive(wl->dvdUndoButton2, TRUE);

	 gtk_widget_set_sensitive(wl->otherEntry, FALSE);

         Closure->mediumSize = 0;
      }

      if(widget == wl->radio2) /* user specified value */
      {  const char *value = gtk_entry_get_text(GTK_ENTRY(wl->otherEntry));

	 gtk_widget_set_sensitive(wl->cdEntry, FALSE);
	 gtk_widget_set_sensitive(wl->dvdEntry1, FALSE);
	 gtk_widget_set_sensitive(wl->dvdEntry2, FALSE);
	 gtk_widget_set_sensitive(wl->cdButton, FALSE);
	 gtk_widget_set_sensitive(wl->dvdButton1, FALSE);
	 gtk_widget_set_sensitive(wl->dvdButton2, FALSE);
	 gtk_widget_set_sensitive(wl->cdUndoButton, FALSE);
	 gtk_widget_set_sensitive(wl->dvdUndoButton1, FALSE);
	 gtk_widget_set_sensitive(wl->dvdUndoButton2, FALSE);

	 gtk_widget_set_sensitive(wl->otherEntry, TRUE);

	 Closure->mediumSize = atoll(value);
      }
   }
}

/*
 * Query size from current medium
 */

static void query_cb(GtkWidget *widget, gpointer data)
{  RS02Widgets *wl = (RS02Widgets*)data;
   char value[40];
   gint64 size;
 
   size = CurrentImageCapacity();
   g_snprintf(value, 40, "%lld", (long long int)size);

   if(widget == wl->cdButton)
     gtk_entry_set_text(GTK_ENTRY(wl->cdEntry), value);

   if(widget == wl->cdUndoButton)
   {  g_snprintf(value, 40, "%lld", (long long int)Closure->savedCDSize);
      gtk_entry_set_text(GTK_ENTRY(wl->cdEntry), value);
   }

   if(widget == wl->dvdButton1)
     gtk_entry_set_text(GTK_ENTRY(wl->dvdEntry1), value);

   if(widget == wl->dvdUndoButton1)
   {  g_snprintf(value, 40, "%lld", (long long int)Closure->savedDVDSize1);
      gtk_entry_set_text(GTK_ENTRY(wl->dvdEntry1), value);
   }

   if(widget == wl->dvdButton2)
     gtk_entry_set_text(GTK_ENTRY(wl->dvdEntry2), value);

   if(widget == wl->dvdUndoButton2)
   {  g_snprintf(value, 40, "%lld", (long long int)Closure->savedDVDSize2);
      gtk_entry_set_text(GTK_ENTRY(wl->dvdEntry2), value);
   }
}

/* 
 * Some values are shared with RS01.
 * If they changed there, update our preferences page.
 */

void ResetRS02PrefsPage(Method *method)
{  RS02Widgets *wl = (RS02Widgets*)method->widgetList;
   int index;

   for(index = 0; index < sizeof(cache_size)/sizeof(int); index++)
     if(cache_size[index] > Closure->cacheMB)
       break;

   gtk_range_set_value(GTK_RANGE(wl->cacheScale), index > 0 ? index-1 : index);
}

/*
 * Read values from our preferences page
 * to make sure that all changed values from text entries
 * are recognized.
 */

void ReadRS02Preferences(Method *method)
{  RS02Widgets *wl = (RS02Widgets*)method->widgetList;
   gint64 value;

   value = atoll(gtk_entry_get_text(GTK_ENTRY(wl->cdEntry)));
   Closure->cdSize = value > 0 ? value : 0; 
   value = atoll(gtk_entry_get_text(GTK_ENTRY(wl->dvdEntry1)));
   Closure->dvdSize1 = value > 0 ? value : 0; 
   value = atoll(gtk_entry_get_text(GTK_ENTRY(wl->dvdEntry2)));
   Closure->dvdSize2 = value > 0 ? value : 0; 

   if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wl->radio1)))   
   {  Closure->mediumSize = 0;
   }

   if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wl->radio2)))   
   {  value = atoll(gtk_entry_get_text(GTK_ENTRY(wl->otherEntry)));
   
      Closure->mediumSize = value > 0 ? value : 0; 
   }
}

/*
 * Create our preferences page
 */

void CreateRS02PrefsPage(Method *method, GtkWidget *parent)
{  RS02Widgets *wl = (RS02Widgets*)method->widgetList;
   GtkWidget *frame, *hbox, *vbox, *vbox2, *lab, *scale, *table;
   GtkWidget *radio1, *radio2, *icon; 
   unsigned int index;
   char value[40];

   /*** Redundancy selection */

   frame = gtk_frame_new(_utf("Maximum image size"));
   gtk_box_pack_start(GTK_BOX(parent), frame, FALSE, FALSE, 0);

   vbox = gtk_vbox_new(FALSE, 10);
   gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
   gtk_container_add(GTK_CONTAINER(frame), vbox);

   hbox = gtk_hbox_new(FALSE, 0);
   gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

   vbox2 = gtk_vbox_new(FALSE, 0);
   gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 0);
   wl->radio1 = radio1 = gtk_radio_button_new(NULL);
   g_signal_connect(G_OBJECT(radio1), "toggled", G_CALLBACK(toggle_cb), method);
   gtk_box_pack_start(GTK_BOX(vbox2), radio1, FALSE, FALSE, 0);

   table = gtk_table_new(5, 4, FALSE);
   gtk_box_pack_start(GTK_BOX(hbox), table, FALSE, FALSE, 0);

   lab = gtk_label_new(_utf("Use smallest possible size from following table (in sectors):"));
   gtk_misc_set_alignment(GTK_MISC(lab), 0.0, 0.0); 
   gtk_table_attach(GTK_TABLE(table), lab, 0, 5, 0, 1, GTK_SHRINK | GTK_FILL, GTK_SHRINK, 5, 5);

   lab = gtk_label_new(_("CD-R / CD-RW:"));
   gtk_misc_set_alignment(GTK_MISC(lab), 0.0, 0.0); 
   gtk_table_attach(GTK_TABLE(table), lab, 0, 1, 1, 2, GTK_SHRINK | GTK_FILL, GTK_SHRINK, 5, 5);

   lab = gtk_label_new(_("DVD 1 layer:"));
   gtk_misc_set_alignment(GTK_MISC(lab), 0.0, 0.0); 
   gtk_table_attach(GTK_TABLE(table), lab, 0, 1, 2, 3, GTK_SHRINK | GTK_FILL, GTK_SHRINK, 5, 5);

   lab = gtk_label_new(_("DVD 2 layers:"));
   gtk_misc_set_alignment(GTK_MISC(lab), 0.0, 0.0); 
   gtk_table_attach(GTK_TABLE(table), lab, 0, 1, 3, 4, GTK_SHRINK | GTK_FILL, GTK_SHRINK, 5, 5);

   wl->cdEntry = gtk_entry_new();
   gtk_entry_set_width_chars(GTK_ENTRY(wl->cdEntry), 9);
   gtk_table_attach(GTK_TABLE(table), wl->cdEntry, 1, 2, 1, 2, GTK_SHRINK | GTK_FILL, GTK_SHRINK, 5, 5);

   wl->dvdEntry1 = gtk_entry_new();
   gtk_entry_set_width_chars(GTK_ENTRY(wl->dvdEntry1), 9);
   gtk_table_attach(GTK_TABLE(table), wl->dvdEntry1, 1, 2, 2, 3, GTK_SHRINK | GTK_FILL, GTK_SHRINK, 5, 5);

   wl->dvdEntry2 = gtk_entry_new();
   gtk_entry_set_width_chars(GTK_ENTRY(wl->dvdEntry2), 9);
   gtk_table_attach(GTK_TABLE(table), wl->dvdEntry2, 1, 2, 3, 4, GTK_SHRINK | GTK_FILL, GTK_SHRINK, 5, 5);

   wl->cdButton = gtk_button_new_with_label(_utf("query medium"));
   g_signal_connect(G_OBJECT(wl->cdButton), "clicked", G_CALLBACK(query_cb), wl);
   gtk_table_attach(GTK_TABLE(table), wl->cdButton, 2, 3, 1, 2, GTK_SHRINK | GTK_FILL, GTK_SHRINK, 5, 5);

   wl->dvdButton1 = gtk_button_new_with_label(_utf("query medium"));
   g_signal_connect(G_OBJECT(wl->dvdButton1), "clicked", G_CALLBACK(query_cb), wl);
   gtk_table_attach(GTK_TABLE(table), wl->dvdButton1, 2, 3, 2, 3, GTK_SHRINK | GTK_FILL, GTK_SHRINK, 5, 5);

   wl->dvdButton2 = gtk_button_new_with_label(_utf("query medium"));
   g_signal_connect(G_OBJECT(wl->dvdButton2), "clicked", G_CALLBACK(query_cb), wl);
   gtk_table_attach(GTK_TABLE(table), wl->dvdButton2, 2, 3, 3, 4, GTK_SHRINK | GTK_FILL, GTK_SHRINK, 5, 5);

   icon = gtk_image_new_from_stock(GTK_STOCK_UNDO, GTK_ICON_SIZE_SMALL_TOOLBAR);
   wl->cdUndoButton = gtk_button_new();
   gtk_container_add(GTK_CONTAINER(wl->cdUndoButton), icon);
   g_signal_connect(G_OBJECT(wl->cdUndoButton), "clicked", G_CALLBACK(query_cb), wl);
   gtk_table_attach(GTK_TABLE(table), wl->cdUndoButton, 3, 4, 1, 2, GTK_SHRINK | GTK_FILL, GTK_SHRINK, 5, 5);

   icon = gtk_image_new_from_stock(GTK_STOCK_UNDO, GTK_ICON_SIZE_SMALL_TOOLBAR);
   wl->dvdUndoButton1 = gtk_button_new();
   gtk_container_add(GTK_CONTAINER(wl->dvdUndoButton1), icon);
   g_signal_connect(G_OBJECT(wl->dvdUndoButton1), "clicked", G_CALLBACK(query_cb), wl);
   gtk_table_attach(GTK_TABLE(table), wl->dvdUndoButton1, 3, 4, 2, 3, GTK_SHRINK | GTK_FILL, GTK_SHRINK, 5, 5);

   icon = gtk_image_new_from_stock(GTK_STOCK_UNDO, GTK_ICON_SIZE_SMALL_TOOLBAR);
   wl->dvdUndoButton2 = gtk_button_new();
   gtk_container_add(GTK_CONTAINER(wl->dvdUndoButton2), icon);
   g_signal_connect(G_OBJECT(wl->dvdUndoButton2), "clicked", G_CALLBACK(query_cb), wl);
   gtk_table_attach(GTK_TABLE(table), wl->dvdUndoButton2, 3, 4, 3, 4, GTK_SHRINK | GTK_FILL, GTK_SHRINK, 5, 5);

   g_snprintf(value, 40, "%lld", (long long int)Closure->cdSize);
   gtk_entry_set_text(GTK_ENTRY(wl->cdEntry), value);
   g_snprintf(value, 40, "%lld", (long long int)Closure->dvdSize1);
   gtk_entry_set_text(GTK_ENTRY(wl->dvdEntry1), value);
   g_snprintf(value, 40, "%lld", (long long int)Closure->dvdSize2);
   gtk_entry_set_text(GTK_ENTRY(wl->dvdEntry2), value);

   /* custom value selection */

   hbox = gtk_hbox_new(FALSE, 4);
   gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

   wl->radio2 = radio2 = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(radio1));
   g_signal_connect(G_OBJECT(radio2), "toggled", G_CALLBACK(toggle_cb), method);
   gtk_box_pack_start(GTK_BOX(hbox), radio2, FALSE, FALSE, 0);
   lab = gtk_label_new(_utf("Use at most"));
   gtk_container_add(GTK_CONTAINER(radio2), lab);

   wl->otherEntry = gtk_entry_new();
   gtk_entry_set_width_chars(GTK_ENTRY(wl->otherEntry), 9);
   gtk_box_pack_start(GTK_BOX(hbox), wl->otherEntry, FALSE, FALSE, 0);

   lab = gtk_label_new(_utf("sectors."));
   gtk_box_pack_start(GTK_BOX(hbox), lab, FALSE, FALSE, 0);

   g_snprintf(value, 40, "%lld", (long long int)Closure->mediumSize);
   gtk_entry_set_text(GTK_ENTRY(wl->otherEntry), value);

   /* Initialize radio buttons */

   if(Closure->mediumSize)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio2), TRUE);
   else 
   {    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio1), TRUE);
        gtk_widget_set_sensitive(wl->otherEntry, FALSE);
   }

   /* Memory utilization */
   
   frame = gtk_frame_new(_utf("Memory utilization"));
   gtk_box_pack_start(GTK_BOX(parent), frame, FALSE, FALSE, 0);

   hbox = gtk_hbox_new(FALSE, 4);
   gtk_container_set_border_width(GTK_CONTAINER(hbox), 10);
   gtk_container_add(GTK_CONTAINER(frame), hbox);

   lab = gtk_label_new(_utf("Use"));
   gtk_box_pack_start(GTK_BOX(hbox), lab, FALSE, FALSE, 0);

   for(index = 0; index < sizeof(cache_size)/sizeof(int); index++)
     if(cache_size[index] > Closure->cacheMB)
       break;

   scale = wl->cacheScale = gtk_hscale_new_with_range(0,16,1);
   gtk_scale_set_value_pos(GTK_SCALE(scale), GTK_POS_RIGHT);
   gtk_range_set_increments(GTK_RANGE(scale), 1, 1);
   gtk_range_set_value(GTK_RANGE(scale), index > 0 ? index-1 : index);
   g_signal_connect(scale, "format-value", G_CALLBACK(format_cb), (gpointer)PREF_CACHE);
   g_signal_connect(scale, "value-changed", G_CALLBACK(scale_cb), (gpointer)PREF_CACHE);
   gtk_box_pack_start(GTK_BOX(hbox), scale, TRUE, TRUE, 0);

}
