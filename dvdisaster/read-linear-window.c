/*  dvdisaster: Additional error correction for optical media.
 *  Copyright (C) 2004-2007 Carsten Gnoerlich.
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

#include "read-linear.h"

/***
 *** Forward declarations
 ***/

static void redraw_curve(void);
static void update_geometry(void);

/***
 *** Routines for updating the GUI from the action thread.
 ***/

/*
 * Set the (predicted) maximum reading speed
 */

static gboolean max_speed_idle_func(gpointer data)
{  
   gdk_window_clear(Closure->readLinearDrawingArea->window);
   update_geometry();
   redraw_curve();

   return FALSE;
}

void InitializeCurve(void *rc_ptr, int max_rate)
{  read_closure *rc = (read_closure*)rc_ptr;
   int i;

   Closure->readLinearCurve->maxY = max_rate;
   Closure->readLinearCurve->maxX = rc->sectors/512;

   rc->lastCopied = (1000*rc->firstSector)/rc->sectors;
   rc->lastPlotted = rc->lastSegment = rc->lastCopied;
   rc->lastPlottedY = 0;

   if(Closure->readLinearSpiral)
     for(i=rc->lastCopied-1; i>=0; i--)
       Closure->readLinearSpiral->segmentColor[i] = Closure->blueSector;

   g_idle_add(max_speed_idle_func, NULL);
}

/*
 * Drawing the reading speed curve 
 */

typedef struct
{  read_closure *rc;
   int percent;
} curve_info;

static gboolean curve_idle_func(gpointer data)
{  curve_info *ci = (curve_info*)data;
   read_closure *rc=ci->rc;
   gint x0,y0;
   char *utf,buf[80];
   gint i;
   gint resize_curve = FALSE;

   /*** Update the textual output */

   g_snprintf(buf, 80, _("Current Speed: %d.%dx"), 
	      (int)Closure->readLinearCurve->fvalue[ci->percent],
	      (int)(fmod(10*Closure->readLinearCurve->fvalue[ci->percent],10)));
   utf = g_locale_to_utf8(buf, -1, NULL, NULL, NULL);
   gtk_label_set_text(GTK_LABEL(Closure->readLinearSpeed), utf);
   g_free(utf);

   g_snprintf(buf, 80, _("Unreadable / skipped sectors: %lld"), Closure->readErrors);

   utf = g_locale_to_utf8(buf, -1, NULL, NULL, NULL);
   gtk_label_set_text(GTK_LABEL(Closure->readLinearErrors), utf);
   g_free(utf);

   /*** Draw the changed spiral segments */

   for(i=rc->lastSegment; i<ci->percent; i++)
     switch(Closure->readLinearCurve->ivalue[i])
     {  case 0: DrawSpiralSegment(Closure->readLinearSpiral, Closure->blueSector, i); break;
        case 1: DrawSpiralSegment(Closure->readLinearSpiral, Closure->greenSector, i); break;
        case 2: DrawSpiralSegment(Closure->readLinearSpiral, Closure->redSector, i); break;
        case 3: DrawSpiralSegment(Closure->readLinearSpiral, Closure->darkSector, i); break;
        case 4: DrawSpiralSegment(Closure->readLinearSpiral, Closure->yellowSector, i); break;
     }

   rc->lastSegment = ci->percent;

   if(rc->pass)      /* 2nd or higher reading pass, don't touch the curve */
   {  g_free(ci);
      return FALSE;
   }

   /*** Resize the Y axes if speed value exceeds current maximum */

   for(i=rc->lastPlotted+1; i<=ci->percent; i++)
     if(Closure->readLinearCurve->fvalue[i] > Closure->readLinearCurve->maxY)
       resize_curve = TRUE;

   if(resize_curve)
   {  Closure->readLinearCurve->maxY = Closure->readLinearCurve->fvalue[ci->percent] + 1;

      update_geometry();
      gdk_window_clear(Closure->readLinearDrawingArea->window);
      redraw_curve();
      rc->lastPlotted = ci->percent;
      rc->lastPlottedY = CurveY(Closure->readLinearCurve, Closure->readLinearCurve->fvalue[ci->percent]); 
      g_free(ci);
      return FALSE;
   }

   /*** Draw the changed curve part */
   
   x0 = CurveX(Closure->readLinearCurve, rc->lastPlotted);
   y0 = CurveY(Closure->readLinearCurve, Closure->readLinearCurve->fvalue[rc->lastPlotted]);
   if(rc->lastPlottedY) y0 = rc->lastPlottedY;

   gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->curveColor);

   for(i=rc->lastPlotted+1; i<=ci->percent; i++)
   {  gint x1 = CurveX(Closure->readLinearCurve, i);
      gint y1 = CurveY(Closure->readLinearCurve, Closure->readLinearCurve->fvalue[i]);

      if(x0<x1)
      { gdk_draw_line(Closure->readLinearDrawingArea->window,
		      Closure->drawGC,
		      x0, y0, x1, y1);

	rc->lastPlotted = ci->percent;
	x0 = x1;
	rc->lastPlottedY = y0 = y1;
      }
   }

   g_free(ci);
   return FALSE;
}

/*
 * Add one new data point
 */

void AddCurveValues(void *rc_ptr, int percent, int color)
{  read_closure *rc = (read_closure*)rc_ptr;
   curve_info *ci;
   int i;

   if(percent < 0 || percent > 1000)
     return;

   ci = g_malloc(sizeof(curve_info));
   ci->rc = rc;
   ci->percent = percent;
   
   /*** Mark unused speed values between lastCopied and Percent */

   if(!rc->pass)
   {  Closure->readLinearCurve->fvalue[percent] = rc->speed;

      for(i=rc->lastCopied+1; i<percent; i++)
	 Closure->readLinearCurve->fvalue[i] = rc->speed > 0.0 ? -1.0 : 0.0;
   }

   /*** Mark the spiral segments between lastCopied and Percent*/

   /* lastCopied+1 ? */

   for(i=rc->lastCopied; i<=percent; i++)
     Closure->readLinearCurve->ivalue[i] = color;

   rc->lastCopied = percent;

   g_idle_add(curve_idle_func, ci);
}

/*
 * Mark existing sectors with the dark green color.
 */

static gboolean curve_mark_idle_func(gpointer data)
{
   DrawSpiral(Closure->readLinearSpiral);

   return FALSE;
}

void MarkExistingSectors(void)
{  int i;
   int x = Closure->readLinearCurve->rightX + 20;

   Closure->additionalSpiralColor = 3;
   DrawSpiralLabel(Closure->readLinearSpiral, Closure->readLinearCurve->layout,
		   _("Already present"), Closure->darkSector, x, -1);

   for(i=0; i<1000; i++)
      if(Closure->readLinearSpiral->segmentColor[i] == Closure->greenSector)
      {  Closure->readLinearSpiral->segmentColor[i] = Closure->darkSector;
	 Closure->readLinearCurve->ivalue[i] = 3;
      }

   g_idle_add(curve_mark_idle_func, NULL);
}

/*
 * Redraw the whole curve
 */

/* Calculate the geometry of the curve and spiral */

static void update_geometry(void)
{  GtkWidget *widget = Closure->readLinearDrawingArea;
   GtkAllocation *a = &widget->allocation;

   /* Curve geometry */ 

   UpdateCurveGeometry(Closure->readLinearCurve, "99x", Closure->readLinearSpiral->diameter + 30);

   /* Spiral center */

   Closure->readLinearSpiral->mx = a->width - 15 - Closure->readLinearSpiral->diameter / 2;
   Closure->readLinearSpiral->my = a->height / 2;

   if(Closure->checkCrc || Closure->crcErrors)
   {  int w,h;

      SetText(Closure->readLinearCurve->layout, _("Sectors with CRC errors"), &w, &h);

      Closure->readLinearSpiral->my -= h;
   }

   /* Label positions in the foot line */

   gtk_box_set_child_packing(GTK_BOX(Closure->readLinearFootlineBox), Closure->readLinearSpeed, 
			     TRUE, TRUE, Closure->readLinearCurve->leftX, GTK_PACK_START);
   gtk_box_set_child_packing(GTK_BOX(Closure->readLinearFootlineBox), Closure->readLinearErrors, 
			     TRUE, TRUE, Closure->readLinearCurve->leftX, GTK_PACK_START);

}

static void redraw_curve(void)
{  GdkDrawable *d = Closure->readLinearDrawingArea->window;
   int x,w,h;
   int pos = 1;

   /* Draw and label the spiral */

   x = Closure->readLinearCurve->rightX + 20;
   gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->curveColor);
   SetText(Closure->readLinearCurve->layout, _("Media state"), &w, &h);
   gdk_draw_layout(d, Closure->drawGC, 
		   x,
		   Closure->readLinearCurve->topY - h - 5, 
		   Closure->readLinearCurve->layout);

   if(Closure->additionalSpiralColor == 0)
     DrawSpiralLabel(Closure->readLinearSpiral, Closure->readLinearCurve->layout,
		     _("Not touched this time"), Closure->curveColor, x, -1);

   if(Closure->additionalSpiralColor == 3)
     DrawSpiralLabel(Closure->readLinearSpiral, Closure->readLinearCurve->layout,
		     _("Already present"), Closure->darkSector, x, -1);

   DrawSpiralLabel(Closure->readLinearSpiral, Closure->readLinearCurve->layout,
		   _("Successfully read"), Closure->greenSector, x, pos++);

   if(Closure->checkCrc || Closure->crcErrors)
     DrawSpiralLabel(Closure->readLinearSpiral, Closure->readLinearCurve->layout,
		     _("Sectors with CRC errors"), Closure->yellowSector, x, pos++);
   
   DrawSpiralLabel(Closure->readLinearSpiral, Closure->readLinearCurve->layout,
		   _("Unreadable / skipped"), Closure->redSector, x, pos++);

   DrawSpiral(Closure->readLinearSpiral);

   /* Redraw the curve */

   RedrawAxes(Closure->readLinearCurve);
   RedrawCurve(Closure->readLinearCurve, 1000, REDRAW_FCURVE);
}

static gboolean expose_cb(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{  
   SetSpiralWidget(Closure->readLinearSpiral, widget);
  
   if(event->count) /* Exposure compression */
     return TRUE;

   update_geometry();
   redraw_curve();

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

   hbox = Closure->readLinearFootlineBox = gtk_hbox_new(FALSE, 0);
   Closure->readLinearSpeed = gtk_label_new(NULL);
   gtk_misc_set_alignment(GTK_MISC(Closure->readLinearSpeed), 0.0, 0.0); 
   gtk_box_pack_start(GTK_BOX(hbox), Closure->readLinearSpeed, FALSE, FALSE, 0);

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
   Closure->readLinearSpiral = CreateSpiral(Closure->grid, Closure->background, 10, 5, 1000);
}
