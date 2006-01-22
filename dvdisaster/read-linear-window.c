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
 *** Forward declarations
 ***/

static void redraw_curve(GtkWidget*);
static void update_geometry(GtkWidget*);

/***
 *** Routines for updating the GUI from the action thread.
 ***/

/*
 * Set the (predicted) maximum reading speed
 */

static gboolean max_speed_idle_func(gpointer data)
{  
   gdk_window_clear(Closure->readLinearDrawingArea->window);
   redraw_curve(Closure->readLinearDrawingArea);

   return FALSE;
}

void InitializeCurve(int max_speed, gint64 start, gint64 end, gint64 medium_size)
{  int i;

   Closure->readLinearCurve->maxY = max_speed;
   Closure->readLinearCurve->maxX = medium_size/512;
   Closure->lastPercent = (1000*start)/medium_size;
   Closure->lastPercentPlotted = Closure->lastPercent;

   if(Closure->readLinearSpiral)
     for(i=Closure->lastPercent-1; i>=0; i--)
       Closure->readLinearSpiral->segmentColor[i] = Closure->blue;

   g_idle_add(max_speed_idle_func, NULL);
}

/*
 * Drawing the reading speed curve 
 */

static gboolean curve_idle_func(gpointer data)
{  gint x0 = CurveX(Closure->readLinearCurve, Closure->lastPercentPlotted);
   gint x1 = CurveX(Closure->readLinearCurve, Closure->percent);
   gint y0 = CurveY(Closure->readLinearCurve, Closure->readLinearCurve->fvalue[Closure->lastPercentPlotted]);
   gint y1 = CurveY(Closure->readLinearCurve, Closure->readLinearCurve->fvalue[Closure->percent]);
   char *utf,buf[80];
   gint i;

   /*** Update the textual output */

   g_snprintf(buf, 80, _("Current Speed: %d.%dx"), 
	      (int)Closure->readLinearCurve->fvalue[Closure->lastPercent],
	      (int)(fmod(10*Closure->readLinearCurve->fvalue[Closure->lastPercent],10)));
   utf = g_locale_to_utf8(buf, -1, NULL, NULL, NULL);
   gtk_label_set_text(GTK_LABEL(Closure->readLinearSpeed), utf);
   g_free(utf);
   
   g_snprintf(buf, 80, _("Unreadable / skipped sectors: %lld"), *(gint64*)data);
   utf = g_locale_to_utf8(buf, -1, NULL, NULL, NULL);
   gtk_label_set_text(GTK_LABEL(Closure->readLinearErrors), utf);
   g_free(utf);

   /*** Mark unused speed values */

   for(i=Closure->lastPercent+1; i<Closure->percent; i++)
      Closure->readLinearCurve->fvalue[i] = Closure->readLinearCurve->fvalue[Closure->percent] > 0.0 ? -1.0 : 0.0;

   /*** Mark the spiral */

   for(i=Closure->lastPercent+1; i<=Closure->percent; i++)
     switch(Closure->readLinearCurve->ivalue[Closure->percent])
     {  case 0: DrawSpiralSegment(Closure->readLinearSpiral, Closure->blue, i-1); break;
        case 1: DrawSpiralSegment(Closure->readLinearSpiral, Closure->green, i-1); break;
        case 2: DrawSpiralSegment(Closure->readLinearSpiral, Closure->red, i-1); break;
        case 3: DrawSpiralSegment(Closure->readLinearSpiral, Closure->darkgreen, i-1); break;
     }

   Closure->lastPercent = Closure->percent;

   /*** If we have not moved on by at least one pixel, skip the rest */

   if(x0 >= x1)
      return FALSE;

   /*** Resize the Y axes if speed value exceeds current maximum */

   if(Closure->readLinearCurve->fvalue[Closure->percent] > Closure->readLinearCurve->maxY)
   {  Closure->readLinearCurve->maxY = Closure->readLinearCurve->fvalue[Closure->percent] + 1;

      update_geometry(Closure->readLinearDrawingArea);
      gdk_window_clear(Closure->readLinearDrawingArea->window);
      redraw_curve(Closure->readLinearDrawingArea);
      //      Closure->lastPercent = Closure->percent;
      Closure->lastPercentPlotted = Closure->percent;

      return FALSE;
   }

   /*** Draw the speed curve */

   gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->blue);
   gdk_draw_line(Closure->readLinearDrawingArea->window,
		 Closure->drawGC,
		x0, y0, x1, y1);

   //   Closure->lastPercent = Closure->percent;
   Closure->lastPercentPlotted = Closure->percent;

   return FALSE;
}

/*
 * Add one new data point
 */

void AddCurveValues(int percent, double speed, gint64 total_errors, int color)
{  static gint64 t_errors;

   if(percent < 0 || percent > 1000)
     return;

   Closure->readLinearCurve->fvalue[percent] = speed;
   Closure->readLinearCurve->ivalue[percent] = color;
   Closure->percent = percent;
   t_errors = total_errors;

   g_idle_remove_by_data(&t_errors);        /* do not queue up redraws */
   g_idle_add(curve_idle_func, &t_errors);
}

/*
 * Redraw the whole curve
 */

/* Calculate the geometry of the curve and spiral */

static void update_geometry(GtkWidget *widget)
{  GtkAllocation *a = &widget->allocation;

   /* Curve geometry */ 

   UpdateCurveGeometry(Closure->readLinearCurve, "99x", Closure->readLinearSpiral->diameter + 30);

   /* Spiral center */

   Closure->readLinearSpiral->mx = a->width - 15 - Closure->readLinearSpiral->diameter / 2;
   Closure->readLinearSpiral->my = a->height / 2;

   /* Label positions in the foot line */

   gtk_box_set_child_packing(GTK_BOX(Closure->readLinearFootlineBox), Closure->readLinearSpeed, 
			     TRUE, TRUE, Closure->readLinearCurve->leftX, GTK_PACK_START);
   gtk_box_set_child_packing(GTK_BOX(Closure->readLinearFootlineBox), Closure->readLinearErrors, 
			     TRUE, TRUE, Closure->readLinearCurve->leftX, GTK_PACK_START);

}

static void redraw_curve(GtkWidget *widget)
{  GdkDrawable *d = Closure->readLinearDrawingArea->window;
   int x,w,h;

   /* Draw and label the spiral */

   x = Closure->readLinearCurve->rightX + 20;
   gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->blue);
   SetText(Closure->readLinearCurve->layout, _("Media state"), &w, &h);
   gdk_draw_layout(d, Closure->drawGC, 
		   x,
		   Closure->readLinearCurve->topY - h - 5, 
		   Closure->readLinearCurve->layout);

   if(Closure->additionalSpiralColor == 0)
     DrawSpiralLabel(Closure->readLinearSpiral, Closure->readLinearCurve->layout,
		     _("Not touched this time"), Closure->blue, x, -1);

   if(Closure->additionalSpiralColor == 3)
     DrawSpiralLabel(Closure->readLinearSpiral, Closure->readLinearCurve->layout,
		     _("Already present"), Closure->darkgreen, x, -1);

   DrawSpiralLabel(Closure->readLinearSpiral, Closure->readLinearCurve->layout,
		   _("Successfully read"), Closure->green, x, 1);

   DrawSpiralLabel(Closure->readLinearSpiral, Closure->readLinearCurve->layout,
		   _("Unreadable / skipped"), Closure->red, x, 2);

   DrawSpiral(Closure->readLinearSpiral);

   /* Redraw the curve */

   RedrawAxes(Closure->readLinearCurve);
   RedrawCurve(Closure->readLinearCurve, Closure->percent, REDRAW_FCURVE);

}

static gboolean expose_cb(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{  
   SetSpiralWidget(Closure->readLinearSpiral, widget);
  
   if(event->count) /* Exposure compression */
     return TRUE;

   update_geometry(widget);
   redraw_curve(widget);

   return TRUE;
}

/***
 *** Reset the notebook contents for new scan/read action
 ***/

void ResetLinearReadWindow()
{  
   gtk_notebook_set_current_page(GTK_NOTEBOOK(Closure->readLinearNotebook), 0);

   ZeroCurve(Closure->readLinearCurve);
   FillSpiral(Closure->readLinearSpiral, Closure->background);
   DrawSpiral(Closure->readLinearSpiral);

   Closure->percent = 0;
   Closure->lastPercent = 0;
   Closure->lastPercentPlotted = 0;
}

/***
 *** Create the notebook contents for the reading and scanning action
 ***/

void CreateLinearReadWindow(GtkWidget *parent)
{  GtkWidget *sep,*ignore,*d_area,*notebook,*hbox;

   Closure->readLinearHeadline = gtk_label_new(NULL);
   gtk_misc_set_alignment(GTK_MISC(Closure->readLinearHeadline), 0.0, 0.0); 
   gtk_misc_set_padding(GTK_MISC(Closure->readLinearHeadline), 5, 0);
   gtk_box_pack_start(GTK_BOX(parent), Closure->readLinearHeadline, FALSE, FALSE, 3);

   sep = gtk_hseparator_new();
   gtk_box_pack_start(GTK_BOX(parent), sep, FALSE, FALSE, 0);

   sep = gtk_hseparator_new();
   gtk_box_pack_start(GTK_BOX(parent), sep, FALSE, FALSE, 0);

   d_area = Closure->readLinearDrawingArea = gtk_drawing_area_new();
   gtk_box_pack_start(GTK_BOX(parent), d_area, TRUE, TRUE, 0);
   g_signal_connect(G_OBJECT(d_area), "expose_event", G_CALLBACK(expose_cb), NULL);

   notebook = Closure->readLinearNotebook = gtk_notebook_new();
   gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), FALSE);
   gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), FALSE);
   gtk_box_pack_end(GTK_BOX(parent), notebook, FALSE, FALSE, 0);

   hbox = Closure->readLinearFootlineBox = gtk_hbox_new(TRUE, 0);
   Closure->readLinearSpeed = gtk_label_new(NULL);
   gtk_misc_set_alignment(GTK_MISC(Closure->readLinearSpeed), 0.0, 0.0); 
   gtk_box_pack_start(GTK_BOX(hbox), Closure->readLinearSpeed, TRUE, TRUE, 0);

   Closure->readLinearErrors = gtk_label_new(NULL);
   gtk_misc_set_alignment(GTK_MISC(Closure->readLinearErrors), 1.0, 0.0); 
   gtk_box_pack_start(GTK_BOX(hbox), Closure->readLinearErrors, TRUE, TRUE, 0);

   ignore = gtk_label_new("progress_tab");
   gtk_notebook_append_page(GTK_NOTEBOOK(notebook), hbox, ignore);

   Closure->readLinearFootline = gtk_label_new(NULL);
   gtk_misc_set_alignment(GTK_MISC(Closure->readLinearFootline), 0.0, 0.5); 
   gtk_misc_set_padding(GTK_MISC(Closure->readLinearFootline), 5, 0);
   ignore = gtk_label_new("footer_tab");
   gtk_notebook_append_page(GTK_NOTEBOOK(notebook), Closure->readLinearFootline, ignore);

   Closure->readLinearCurve  = CreateCurve(d_area, _("Speed"), "%dx", 1000, CURVE_MEGABYTES);
   Closure->readLinearSpiral = CreateSpiral(Closure->grid, Closure->background, 10, 5, 999);
}