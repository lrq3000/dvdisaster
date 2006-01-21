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

#include "rs01-includes.h"

/***
 *** Forward declarations
 ***/

static void redraw_curve(RS01Widgets*);
static void update_geometry(RS01Widgets*);
      

/***
 *** Encoding window
 ***/

/*
 * Reset the notebook contents for new encoding action
 */

void ResetRS01EncodeWindow(Method *method)
{  RS01Widgets *wl = (RS01Widgets*)method->widgetList;

   SetProgress(wl->encPBar1, 0, 100);
   SetProgress(wl->encPBar2, 0, 100);

   gtk_widget_hide(wl->encLabel2);
   gtk_widget_hide(wl->encPBar2);
   gtk_widget_hide(wl->curveButton);

   gtk_label_set_text(GTK_LABEL(wl->encFootline), "");
   gtk_label_set_text(GTK_LABEL(wl->encFootline2), "");

}

/* 
 * Show the button for switching to the reading curve
 */

static gboolean show_button_idle_func(gpointer data)
{  Method *method = (Method*)data;
   RS01Widgets *wl = (RS01Widgets*)method->widgetList;

   gtk_widget_show(wl->curveButton);

   return FALSE;
}

void RS01ShowCurveButton(Method *method)
{  
   g_idle_add(show_button_idle_func, method);
   
}

/* 
 * Switch back to the reading curve (read and create mode only)
 */

static gboolean curve_button_cb(GtkWidget *wid, gpointer action)
{  gtk_notebook_set_current_page(GTK_NOTEBOOK(Closure->notebook), 1);
  
   return FALSE;
}

/*
 * Create the notebook contents for creating an error correction file
 */

void CreateRS01EWindow(Method *method, GtkWidget *parent)
{  RS01Widgets *wl;
   GtkWidget *sep,*wid,*pbar,*table,*hbox;

   if(!method->widgetList)
   {  wl = g_malloc0(sizeof(RS01Widgets));
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
			_utf("<b>1. Calculating image sector checksums:</b>"));
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

   hbox = gtk_hbox_new(FALSE, 0);
   gtk_box_pack_start(GTK_BOX(parent), hbox, FALSE, FALSE, 0);

   wid = gtk_label_new(NULL);
   gtk_misc_set_padding(GTK_MISC(wid), 10, 0);
   gtk_box_pack_start(GTK_BOX(hbox), wid, FALSE, FALSE, 0);

   wl->curveButton = gtk_button_new_with_label(_utf("Show reading speed curve"));
   g_signal_connect(G_OBJECT(wl->curveButton), "clicked", G_CALLBACK(curve_button_cb), NULL);
   gtk_box_pack_start(GTK_BOX(hbox), wl->curveButton, FALSE, FALSE, 0);
}

/***
 *** "Fix" window
 ***/

/*
 * Set the media size and ecc capacity
 */

static gboolean set_max_idle_func(gpointer data)
{  RS01Widgets *wl = (RS01Widgets*)data;

   redraw_curve(wl);

   return FALSE;
}

void SetFixMaxValues(RS01Widgets *wl, int data_bytes, int ecc_bytes, gint64 sectors)
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
{  RS01Widgets *wl = (RS01Widgets*)data;

   SetLabelText(GTK_LABEL(wl->fixCorrected), _("Repaired: %lld"), wl->corrected); 
   SetLabelText(GTK_LABEL(wl->fixUncorrected), _("Unrepairable: <span color=\"red\">%lld</span>"), wl->uncorrected); 
   SetLabelText(GTK_LABEL(wl->fixProgress), _("Progress: %3d.%1d%%"), wl->percent/10, wl->percent%10);

   return FALSE;
}

void UpdateFixResults(RS01Widgets *wl, gint64 corrected, gint64 uncorrected)
{
   wl->corrected = corrected;
   wl->uncorrected = uncorrected;

   g_idle_add(results_idle_func, wl);
}

/*
 * Update the error curve 
 */

static gboolean curve_idle_func(gpointer data)
{  RS01Widgets *wl = (RS01Widgets*)data;
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

void AddFixValues(RS01Widgets *wl, int percent, int ecc_max)
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

static void update_geometry(RS01Widgets *wl)
{  
   /* Curve geometry */ 

   UpdateCurveGeometry(wl->fixCurve, "999", 20);

   /* Label positions in the foot line */

   gtk_box_set_child_packing(GTK_BOX(wl->fixFootlineBox), wl->fixCorrected,
			     TRUE, TRUE, wl->fixCurve->leftX, GTK_PACK_START);
   gtk_box_set_child_packing(GTK_BOX(wl->fixFootlineBox), wl->fixUncorrected, 
			     TRUE, TRUE, wl->fixCurve->leftX, GTK_PACK_START);
}

static void redraw_curve(RS01Widgets *wl)
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
{  RS01Widgets *wl = (RS01Widgets*)data; 

   if(event->count) /* Exposure compression */
     return TRUE;

   update_geometry(wl);
   redraw_curve(wl);

   return TRUE;
}

/*
 * Reset the notebook contents for new fixing action
 */

void ResetRS01FixWindow(Method *method)
{  RS01Widgets *wl = (RS01Widgets*)method->widgetList;

   gtk_notebook_set_current_page(GTK_NOTEBOOK(wl->fixNotebook), 0);

   ZeroCurve(wl->fixCurve);
   UpdateFixResults(wl, 0, 0);

   if(wl->fixCurve && wl->fixCurve->widget)
   {  gdk_window_clear(wl->fixCurve->widget->window);
      redraw_curve(wl);
   }

   wl->percent = 0;
   wl->lastPercent = 0;
}

/*
 * Create the notebook contents for fixing an image
 */

void CreateRS01FWindow(Method *method, GtkWidget *parent)
{  RS01Widgets *wl;
   GtkWidget *sep,*ignore,*d_area,*notebook,*hbox;

   if(!method->widgetList)
   {  wl = g_malloc0(sizeof(RS01Widgets));
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

#define SYMBOLSIZE 8
#define FIELDSIZE (1<<SYMBOLSIZE)
#define FIELDMAX (FIELDSIZE-1)

enum 
{  PREF_NROOTS = 0,
   PREF_CACHE = 1,
   PREF_ECC_SIZE = 2
};

static int cache_size[] = { 1, 2, 4, 8, 16, 32, 64, 96, 128, 192, 256, 384, 512, 768, 1024, 1536, 2048 };

static gchar* format_cb(GtkScale *scale, gdouble value, gpointer data)
{  int nroots = value;
   int ndata  = FIELDMAX - nroots;
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
{  int which = GPOINTER_TO_INT(data);
   int value;

   switch(which)
   {  case PREF_CACHE:
	value = gtk_range_get_value(GTK_RANGE(widget));
        Closure->cacheMB = cache_size[value];
	break;
      case PREF_NROOTS:
	value = gtk_range_get_value(GTK_RANGE(widget));
	if(Closure->redundancy) g_free(Closure->redundancy);
	Closure->redundancy = g_strdup_printf("%d", value);
	break;
      case PREF_ECC_SIZE:
	value = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widget));
	if(Closure->redundancy) g_free(Closure->redundancy);
	Closure->redundancy = g_strdup_printf("%dm", value);
	break;
   }
}

static void toggle_cb(GtkWidget *widget, gpointer data)
{  Method *method = (Method*)data;
   RS01Widgets *wl = (RS01Widgets*)method->widgetList;
   int state  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

   if(state == TRUE)
   {  gtk_widget_set_sensitive(wl->redundancyScale, wl->radio3 == widget ? TRUE : FALSE);
      gtk_widget_set_sensitive(wl->redundancySpin, wl->radio4 == widget ? TRUE : FALSE);
      gtk_widget_set_sensitive(wl->radio4Label, wl->radio4 == widget ? TRUE : FALSE);

      

      if(widget == wl->radio1) /* Normal */
      {  
         gtk_range_set_value(GTK_RANGE(wl->redundancyScale), 32);

	 if(Closure->redundancy) g_free(Closure->redundancy);
         Closure->redundancy = g_strdup("normal");
      }

      if(widget == wl->radio2) /* High */
      {  
         gtk_range_set_value(GTK_RANGE(wl->redundancyScale), 64);

	 if(Closure->redundancy) g_free(Closure->redundancy);
	 Closure->redundancy = g_strdup("high");
      }

      if(widget == wl->radio3) /* number of roots */
      {  int nroots = gtk_range_get_value(GTK_RANGE(wl->redundancyScale));

	 if(Closure->redundancy) g_free(Closure->redundancy);
	 Closure->redundancy = g_strdup_printf("%d", nroots);
      }

      if(widget == wl->radio4) /* relative to space usage */
      {  int space = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(wl->redundancySpin));

	 if(Closure->redundancy) g_free(Closure->redundancy);
	 Closure->redundancy = g_strdup_printf("%dm", space);
      }
   }
}


void CreateRS01PrefsPage(Method *method, GtkWidget *parent)
{  RS01Widgets *wl = (RS01Widgets*)method->widgetList;
   GtkWidget *frame, *hbox, *vbox, *lab, *scale, *spin;
   GtkWidget *radio1, *radio2, *radio3, *radio4; 
   unsigned int index;

   /*** Redundancy selection */

   frame = gtk_frame_new(_utf("Redundancy for new error correction files"));
   gtk_box_pack_start(GTK_BOX(parent), frame, FALSE, FALSE, 0);

   vbox = gtk_vbox_new(FALSE, 20);
   gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
   gtk_container_add(GTK_CONTAINER(frame), vbox);

   wl->radio1 = radio1 = gtk_radio_button_new(NULL);
   g_signal_connect(G_OBJECT(radio1), "toggled", G_CALLBACK(toggle_cb), method);
   gtk_box_pack_start(GTK_BOX(vbox), radio1, FALSE, FALSE, 0);
   lab = gtk_label_new(_utf("Normal"));
   gtk_container_add(GTK_CONTAINER(radio1), lab);

   wl->radio2 = radio2 = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(radio1));
   g_signal_connect(G_OBJECT(radio2), "toggled", G_CALLBACK(toggle_cb), method);
   gtk_box_pack_start(GTK_BOX(vbox), radio2, FALSE, FALSE, 0);
   lab = gtk_label_new(_utf("High"));
   gtk_container_add(GTK_CONTAINER(radio2), lab);


   hbox = gtk_hbox_new(FALSE, 4);
   gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

   wl->radio3 = radio3 = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(radio1));
   g_signal_connect(G_OBJECT(radio3), "toggled", G_CALLBACK(toggle_cb), method);
   gtk_box_pack_start(GTK_BOX(hbox), radio3, FALSE, FALSE, 0);
   lab = gtk_label_new(_utf("Other"));
   gtk_container_add(GTK_CONTAINER(radio3), lab);

   wl->redundancyScale = scale = gtk_hscale_new_with_range(8,100,1);
   gtk_scale_set_value_pos(GTK_SCALE(scale), GTK_POS_RIGHT);
   gtk_range_set_increments(GTK_RANGE(scale), 1, 1);
   gtk_range_set_value(GTK_RANGE(scale), 32);
   gtk_widget_set_sensitive(scale, FALSE);
   g_signal_connect(scale, "format-value", G_CALLBACK(format_cb), (gpointer)PREF_NROOTS);
   g_signal_connect(scale, "value-changed", G_CALLBACK(scale_cb), (gpointer)PREF_NROOTS);
   gtk_container_add(GTK_CONTAINER(hbox), scale);


   hbox = gtk_hbox_new(FALSE, 4);
   gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

   wl->radio4 = radio4 = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(radio3));
   g_signal_connect(G_OBJECT(radio4), "toggled", G_CALLBACK(toggle_cb), method);
   gtk_box_pack_start(GTK_BOX(hbox), radio4, FALSE, FALSE, 0);
   lab = gtk_label_new(_utf("Use at most"));
   gtk_container_add(GTK_CONTAINER(radio4), lab);

   wl->redundancySpin = spin = gtk_spin_button_new_with_range(0, 100000, 100);
   g_signal_connect(spin, "value-changed", G_CALLBACK(scale_cb), (gpointer)PREF_ECC_SIZE);
   gtk_entry_set_width_chars(GTK_ENTRY(spin), 8);
   gtk_box_pack_start(GTK_BOX(hbox), spin, FALSE, FALSE, 0);

   wl->radio4Label = lab = gtk_label_new(_utf("MB for error correction data"));
   gtk_box_pack_start(GTK_BOX(hbox), lab, FALSE, FALSE, 0);
   gtk_widget_set_sensitive(spin, FALSE);
   gtk_widget_set_sensitive(lab, FALSE);

   if(Closure->redundancy)
   {  if(!strcmp(Closure->redundancy, "normal"))
         gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wl->radio1), TRUE);
      else if(!strcmp(Closure->redundancy, "high"))
         gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wl->radio2), TRUE);
      else
      {  int last = strlen(Closure->redundancy)-1;

         if(Closure->redundancy[last] == 'm')
	 {  Closure->redundancy[last] = 0;
	    gtk_spin_button_set_value(GTK_SPIN_BUTTON(wl->redundancySpin), atoi(Closure->redundancy));
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wl->radio4), TRUE);
	 }
	 else
	 {  gtk_range_set_value(GTK_RANGE(wl->redundancyScale), atoi(Closure->redundancy));
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wl->radio3), TRUE);
	 }
      }
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

   scale = gtk_hscale_new_with_range(0,16,1);
   gtk_scale_set_value_pos(GTK_SCALE(scale), GTK_POS_RIGHT);
   gtk_range_set_increments(GTK_RANGE(scale), 1, 1);
   gtk_range_set_value(GTK_RANGE(scale), index > 0 ? index-1 : index);
   g_signal_connect(scale, "format-value", G_CALLBACK(format_cb), (gpointer)PREF_CACHE);
   g_signal_connect(scale, "value-changed", G_CALLBACK(scale_cb), (gpointer)PREF_CACHE);
   gtk_box_pack_start(GTK_BOX(hbox), scale, TRUE, TRUE, 0);

}
