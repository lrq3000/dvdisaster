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

/***
 *** Constructors and destructors
 ***/

/*
 * Initialize the curve
 */

Curve* CreateCurve(GtkWidget *widget, char *left_label, char *left_format, int n_values, int bottom_format)
{  Curve *curve = g_malloc0(sizeof(Curve));

   curve->widget     = widget;
   curve->layout     = gtk_widget_create_pango_layout(widget, NULL);
   curve->leftLabel  = g_strdup(left_label); 
   curve->leftFormat = g_strdup(left_format);
   curve->bottomFormat = bottom_format;   

   curve->fvalue       = g_malloc0(sizeof(gdouble)*(n_values+1));
   curve->ivalue       = g_malloc0(sizeof(gint)*(n_values+1));
   curve->lastValueIdx = n_values;

   curve->maxX      = 1;
   curve->maxY      = 1;

   if(bottom_format != CURVE_PERCENT)
     curve->margin = 2;

   return curve;
} 

/*
 * Get rid of it
 */

void FreeCurve(Curve *curve)
{
   g_object_unref(curve->layout);
   g_free(curve->leftLabel);
   g_free(curve->leftFormat);
   g_free(curve->fvalue);
   g_free(curve->ivalue);
   g_free(curve);
}

/*
 * Reset the values
 */

void ZeroCurve(Curve *curve)
{  int i;

   if(curve)
     for(i=0; i<=curve->lastValueIdx; i++)
     {  curve->fvalue[i] = 0.0;
        curve->ivalue[i] = 0;
     }
}

/***
 *** Auxiliary functions
 ***/

/*
 * Calculate pixel coords from curve values
 */

int CurveX(Curve *curve, gdouble x)
{  gdouble width = (curve->rightX - curve->leftX - curve->margin);

   return 1 + curve->leftX + ((gdouble)x * width) / 1000.0;
}

int CurveLX(Curve *curve, gdouble x)
{  gdouble width = (curve->rightX - curve->leftX - curve->margin);

   return 1 + curve->leftX + (x * width) / (gdouble)curve->maxX;
}

int CurveY(Curve *curve, gdouble y)
{  gdouble hfact;

   hfact =   (gdouble)(curve->bottomY - curve->topY) 
           / (gdouble)curve->maxY;

   return curve->bottomY - y * hfact;
}


/***
 *** Calculate the curve geometry
 ***/

void UpdateCurveGeometry(Curve *curve, char *largest_left_label, int right_padding)
{  GtkAllocation *a = &curve->widget->allocation;
   int w,h; 

   /* Top and bottom margins */

   SetText(curve->layout, curve->leftLabel, &w, &h);
   curve->topY = h + 10;

   SetText(curve->layout, "0123456789", &w, &h);
   curve->bottomY = a->height - h - 10;

   /* Left and right margins */

   SetText(curve->layout, largest_left_label, &w, &h);
   curve->leftX   = 5 + 6 + 3 + w;
   curve->rightX  = a->width - right_padding;
}

/***
 *** Redraw the coordinate axes
 ***/

void RedrawAxes(Curve *curve)
{  GdkDrawable *d = curve->widget->window;
   int i,w,h,x,y; 
   int yg=0;
   int step;

   /* Draw and label the left coordinate axis */

   gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->foreground);

   gdk_draw_line(d, Closure->drawGC,
		 curve->leftX, curve->topY, curve->leftX, curve->bottomY);

   gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->curveColor);
   SetText(curve->layout, curve->leftLabel, &w, &h);
   x = curve->leftX - w/2;
   if(x < 5) x = 5;
   gdk_draw_layout(d, Closure->drawGC, 
		   x, curve->topY - h - 5, curve->layout);

   if(curve->maxY < 20) step = 4;
   else step = 10;

   for(i=0; i<=curve->maxY; i+=step)
   {  char buf[4];
   
      g_snprintf(buf, 4, curve->leftFormat, i);
      SetText(curve->layout, buf, &w, &h);

      y = yg = CurveY(curve, i);
      gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->curveColor);
      gdk_draw_layout(d, Closure->drawGC, curve->leftX-9-w, y-h/2, curve->layout);
      gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->foreground);
      gdk_draw_line(d, Closure->drawGC, curve->leftX-6, y, curve->leftX, y);

      gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->grid);
      gdk_draw_line(d, Closure->drawGC, curve->leftX, y, curve->rightX, y);

      gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->foreground);
      y = CurveY(curve, i+step/2);
      if(y >= curve->topY)
        gdk_draw_line(d, Closure->drawGC, curve->leftX-3, y, curve->leftX, y);
   }


   /* Draw the right coordinate axis */

   gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->foreground);

   gdk_draw_line(d, Closure->drawGC,
		 curve->rightX, curve->topY, curve->rightX, curve->bottomY);


   /* Draw and label the bottom coordinate axis */

   gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->foreground);

   gdk_draw_line(d, Closure->drawGC,
		 curve->leftX, curve->bottomY, curve->rightX, curve->bottomY);

   if(curve->maxX <= 100) step = 20;
   else if(curve->maxX < 1000) step = 100;
   else step = 1024;

   for(i=0; i<=curve->maxX; i+=step)
   {  char buf[10];
   
      switch(curve->bottomFormat)
      {  case CURVE_PERCENT:
 	   g_snprintf(buf, 10, "%d%%", i);
	   break;

         case CURVE_MEGABYTES:
	   if(step <= 100)
	        g_snprintf(buf, 10, "%dM",i);
	   else g_snprintf(buf, 10, "%3.1fG",(gdouble)i/1024.0);
	   break;
      }
      SetText(curve->layout, buf, &w, &h);

      x = CurveLX(curve,i)-1;
      gdk_draw_line(d, Closure->drawGC, x, curve->bottomY+6, x, curve->bottomY);
      gdk_draw_layout(d, Closure->drawGC, x-w/2, curve->bottomY+8, curve->layout);

      if(i && x < curve->rightX)
      {  gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->grid);
         gdk_draw_line(d, Closure->drawGC, x, curve->bottomY-1, x, yg);
      }

      gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->foreground);
      x = CurveLX(curve,i+step/2)-1;
      if(x < curve->rightX)
        gdk_draw_line(d, Closure->drawGC, x, curve->bottomY+3, x, curve->bottomY);
   }
}

/*
 * Redraw the curve
 */

void RedrawCurve(Curve *curve, int last, int which)
{  int i,x0,x1,fy0,fy1,iy;

   x0  = CurveX(curve, 0);
   fy0 = CurveY(curve, curve->fvalue[0]);

   gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->curveColor);

   /* Draw the curve */

   for(i=1; i<=last; i++)
   {  x1  = CurveX(curve, i);
      fy1 = CurveY(curve, curve->fvalue[i]);
      iy  = CurveY(curve, curve->ivalue[i]);

      if(which & REDRAW_FCURVE && curve->fvalue[i] >= 0)
      {  if(x0 < x1)
	 {  gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->curveColor);
	    gdk_draw_line(curve->widget->window, Closure->drawGC, x0, fy0, x1, fy1);
	    fy0 = fy1;
	 }
         x0  =  x1;
      }

      if(which & REDRAW_ICURVE)
      {  if(curve->ivalue[i] > 0)
	 {  gdk_gc_set_rgb_fg_color(Closure->drawGC, Closure->barColor);
	    gdk_draw_rectangle(curve->widget->window,
			       Closure->drawGC, TRUE,
			       x0, iy, x0==x1 ? 1 : x1-x0, curve->bottomY-iy);
	 }
	 x0 = x1;
      }
   }
}
