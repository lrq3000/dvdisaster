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
 *** The RS02 codec is not yet integrated into the graphical
 *** user interface.
 */

void ResetRS02EncWindow(Method *method)
{  RS02Widgets *wl = (RS02Widgets*)method->widgetList;

   SetProgress(wl->encPBar1, 0, 100);
   SetProgress(wl->encPBar2, 0, 100);

   gtk_widget_hide(wl->encLabel2);
   gtk_widget_hide(wl->encPBar2);

   gtk_label_set_text(GTK_LABEL(wl->encFootline), "");
   gtk_label_set_text(GTK_LABEL(wl->encFootline2), "");
}

void ResetRS02FixWindow(Method *method)
{
  g_printf("RS02 codec -- ResetRS02FixWindow() called\n");
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

void CreateRS02FixWindow(Method *method, GtkWidget *parent)
{  GtkWidget *lab,*sep;
   RS02Widgets *wl;

   if(!method->widgetList)
   {  wl = g_malloc0(sizeof(RS02Widgets));
      method->widgetList = wl;
   }
   else wl = method->widgetList;

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
   g_snprintf(value, 40, "%lld", size);

   if(widget == wl->cdButton)
     gtk_entry_set_text(GTK_ENTRY(wl->cdEntry), value);

   if(widget == wl->cdUndoButton)
   {  g_snprintf(value, 40, "%lld", Closure->savedCDSize);
      gtk_entry_set_text(GTK_ENTRY(wl->cdEntry), value);
   }

   if(widget == wl->dvdButton1)
     gtk_entry_set_text(GTK_ENTRY(wl->dvdEntry1), value);

   if(widget == wl->dvdUndoButton1)
   {  g_snprintf(value, 40, "%lld", Closure->savedDVDSize1);
      gtk_entry_set_text(GTK_ENTRY(wl->dvdEntry1), value);
   }

   if(widget == wl->dvdButton2)
     gtk_entry_set_text(GTK_ENTRY(wl->dvdEntry2), value);

   if(widget == wl->dvdUndoButton2)
   {  g_snprintf(value, 40, "%lld", Closure->savedDVDSize2);
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

   wl->cdButton = gtk_button_new_with_label(_("query medium"));
   g_signal_connect(G_OBJECT(wl->cdButton), "clicked", G_CALLBACK(query_cb), wl);
   gtk_table_attach(GTK_TABLE(table), wl->cdButton, 2, 3, 1, 2, GTK_SHRINK | GTK_FILL, GTK_SHRINK, 5, 5);

   wl->dvdButton1 = gtk_button_new_with_label(_("query medium"));
   g_signal_connect(G_OBJECT(wl->dvdButton1), "clicked", G_CALLBACK(query_cb), wl);
   gtk_table_attach(GTK_TABLE(table), wl->dvdButton1, 2, 3, 2, 3, GTK_SHRINK | GTK_FILL, GTK_SHRINK, 5, 5);

   wl->dvdButton2 = gtk_button_new_with_label(_("query medium"));
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

   g_snprintf(value, 40, "%lld", Closure->cdSize);
   gtk_entry_set_text(GTK_ENTRY(wl->cdEntry), value);
   g_snprintf(value, 40, "%lld", Closure->dvdSize1);
   gtk_entry_set_text(GTK_ENTRY(wl->dvdEntry1), value);
   g_snprintf(value, 40, "%lld", Closure->dvdSize2);
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

   g_snprintf(value, 40, "%lld", Closure->mediumSize);
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
