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
 *** Local data structs
 ***/

/* non linear scale housekeeping */

typedef struct
{  int action;
   int *values;
   char *format;
} non_linear_info;


/***
 *** Preferences window housekeeping
 ***/

typedef struct
{  //char **jumpFormat;
   char *formatLinear;
   char *formatAdaptive;

   GtkWidget *radioLinear, *radioAdaptive;
   GtkWidget *rangeToggle, *rangeLab;
   GtkWidget *rangeSpin1, *rangeSpin2;
   GtkWidget *jumpScale;
   GtkWidget *byteEntry, *byteCheck;
   GtkWidget *readAndCreateButton;

   non_linear_info *jumpScaleInfo;
} prefs_context;

void FreePreferences(void *context)
{  prefs_context *pc = (prefs_context*)context;

   if(pc->formatLinear) g_free(pc->formatLinear);
   if(pc->formatAdaptive) g_free(pc->formatAdaptive);

   if(pc->jumpScaleInfo) g_free(pc->jumpScaleInfo);

   g_free(pc);
}

static gboolean delete_cb(GtkWidget *widget, GdkEvent *event, gpointer data)
{
   FreePreferences(Closure->prefsContext);

   Closure->prefsWindow = NULL;
   Closure->prefsContext = NULL;
   return FALSE;
}

void HidePreferences(void)
{  prefs_context *pc = (prefs_context*)Closure->prefsContext;

   if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pc->rangeToggle)))   
   {     
      Closure->readStart = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(pc->rangeSpin1));
      Closure->readEnd   = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(pc->rangeSpin2));
   }
   else Closure->readStart = Closure->readEnd = 0;

   if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pc->byteCheck)))   
   {  const char *value = gtk_entry_get_text(GTK_ENTRY(pc->byteEntry));

      Closure->fillUnreadable = strtol(value, NULL, 0);

      if(Closure->fillUnreadable < 0)
	Closure->fillUnreadable = 1;

      if(Closure->fillUnreadable > 255)
	Closure->fillUnreadable = 255;
   }
   PrepareDeadSector();

   gtk_widget_hide(GTK_WIDGET(Closure->prefsWindow));
}

static void close_cb(GtkWidget *widget, gpointer data)
{  
  HidePreferences();
}

/*
 * Actions used in the preferences
 */

enum 
{  TOGGLE_UDF,
   TOGGLE_READ_CREATE,
   TOGGLE_UNLINK,
   TOGGLE_SUFFIX,
   TOGGLE_DAO,
   TOGGLE_2GB,
   TOGGLE_RANGE,

   SPIN_DELAY,
   
   SLIDER_JUMP
};

/*
 * Create a new notebook page
 */

static GtkWidget *create_page(GtkWidget *notebook, char *label)
{  GtkWidget *vbox,*tab_label;

   tab_label = gtk_label_new(label);
   vbox = gtk_vbox_new(FALSE, 5);
   gtk_container_set_border_width(GTK_CONTAINER(vbox), 12);
   
   gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, tab_label);

   return vbox;
}

/***
 *** Toggle button actions
 ***/

static void toggle_cb(GtkWidget *widget, gpointer data)
{  int state  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
   int action = GPOINTER_TO_INT(data);
   prefs_context *pc = (prefs_context*)Closure->prefsContext;

   switch(action)
   {  case TOGGLE_UDF:
	Closure->parseUDF = state;
	break;

      case TOGGLE_READ_CREATE:
	Closure->readAndCreate = state;
	if(state && Closure->adaptiveRead)  /* set reading strategy to linear */
	{  prefs_context *pc = Closure->prefsContext;

	   Closure->adaptiveRead = FALSE;
	   pc->jumpScaleInfo->format = pc->formatLinear;
	   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioLinear), TRUE);

	   ShowMessage(Closure->prefsWindow,
		       _("Switched to the linear reading strategy."), 
		       GTK_MESSAGE_INFO);
	}
	   
	break;

      case TOGGLE_UNLINK:
	Closure->unlinkImage = state;
	break;

      case TOGGLE_SUFFIX:
	Closure->autoSuffix = state;
	break;

      case TOGGLE_DAO:
	Closure->noTruncate = state;
	break;

      case TOGGLE_2GB:
	Closure->splitFiles = state;
	break;

      case TOGGLE_RANGE:
      {  int image_size = CurrentImageSize() - 1;

	 gtk_widget_set_sensitive(pc->rangeSpin1, state);
	 gtk_widget_set_sensitive(pc->rangeSpin2, state);

	 gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin1), 0.0);
	 
	 if(state) 
	 {  gtk_spin_button_set_range(GTK_SPIN_BUTTON(pc->rangeSpin2), 0.0, image_size);
	    gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin2), image_size);
	    
	 }
	 else
	 {  gtk_spin_button_set_range(GTK_SPIN_BUTTON(pc->rangeSpin2), 0.0, 1.0);
	    gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin2), 0.1);
	 }
      }
	break;
   }
}

/***
 *** Spin button actions
 ***/

static void spin_cb(GtkWidget *widget, gpointer data)
{  int which = GPOINTER_TO_INT(data);
   int value = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widget));

   switch(which)
   {  case SPIN_DELAY:
	Closure->spinupDelay = value;
	break;
   }
}

/*
 * Make sure the reading range is a valid interval
 */

static void read_range_cb(GtkWidget *widget, gpointer data)
{  prefs_context *pc = (prefs_context*)data;
   int from = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(pc->rangeSpin1));
   int to = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(pc->rangeSpin2));

   if(from > to)
   {  if(widget == pc->rangeSpin1)
           gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin1), to);
      else gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin2), from);
   }
}

/***
 *** Non-linear scales
 ***/

static int jump_values[] = { 0, 16, 32, 64, 128, 256, 384, 512, 768, 1024, 2048, 4096, 10240,
 20480 };
#define JUMP_VALUE_LENGTH 14

static void non_linear_cb(GtkWidget *widget, gpointer data)
{  non_linear_info *nli = (non_linear_info*)data;
   int index  = gtk_range_get_value(GTK_RANGE(widget));

   switch(nli->action)
   {  case SLIDER_JUMP:
        Closure->sectorSkip = nli->values[index];
	break;
   }
}

static gchar* non_linear_format_cb(GtkScale *scale, gdouble value, gpointer data)
{  non_linear_info *nli = (non_linear_info*)data;
   int index = value;
   char *text;

   text = g_strdup_printf(nli->format, nli->values[index]);
   FORGET(text);   /* The scale will free the old string by itself. Weird. */

   return text;
}

static GtkWidget* non_linear_scale(non_linear_info **nli_ptr, int action, int *values, int n, int value)
{  GtkWidget *scale;
   non_linear_info *nli = g_malloc0(sizeof(non_linear_info));
   int index;

   for(index = 0; index < n; index++)
     if(values[index] > value)
       break;

   nli->action = action;
   nli->values = values;
   *nli_ptr = nli;

   scale = gtk_hscale_new_with_range(0,n-1,1);
   gtk_scale_set_value_pos(GTK_SCALE(scale), GTK_POS_RIGHT);
   gtk_range_set_increments(GTK_RANGE(scale), 1, 1);
   gtk_range_set_value(GTK_RANGE(scale), index > 0 ? index-1 : index);
   g_signal_connect(scale, "format-value", G_CALLBACK(non_linear_format_cb), nli);
   g_signal_connect(scale, "value-changed", G_CALLBACK(non_linear_cb), nli);

   return scale;
}

/*
 * Read strategy selection 
 */

static void strategy_cb(GtkWidget *widget, gpointer data)
{  int state  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
   prefs_context *pc = (prefs_context*)data;

   if(pc->jumpScale && state == TRUE)
   {  double tmp;

      if(pc->radioLinear == widget)
      {  Closure->adaptiveRead = FALSE;
         pc->jumpScaleInfo->format = pc->formatLinear;
      }

      if(pc->radioAdaptive == widget)
      {  Closure->adaptiveRead = TRUE;
         pc->jumpScaleInfo->format = pc->formatAdaptive;

	 if(Closure->readAndCreate)
	 {  Closure->readAndCreate = FALSE;

	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->readAndCreateButton), FALSE);
	    ShowMessage(Closure->prefsWindow,
			_("Disabled automatic error correction file generation."), 
			GTK_MESSAGE_INFO);
	 }
      }

      /* poor hack to make the range widget redraw */

      tmp = gtk_range_get_value(GTK_RANGE(pc->jumpScale));
      gtk_range_set_value(GTK_RANGE(pc->jumpScale), tmp+1.0);
      gtk_range_set_value(GTK_RANGE(pc->jumpScale), tmp);
   }
}

/*
 * Select the value for filling unreadable sectors
 */

static void bytefill_cb(GtkWidget *widget, gpointer data)
{  prefs_context *pc = (prefs_context*)data;
   const char *value = gtk_entry_get_text(GTK_ENTRY(pc->byteEntry));

   Closure->fillUnreadable = strtol(value, NULL, 0);

   if(Closure->fillUnreadable < 0)
   {  Closure->fillUnreadable = 1;
      gtk_entry_set_text(GTK_ENTRY(pc->byteEntry), "1");
   }

   if(Closure->fillUnreadable > 255)
   {  Closure->fillUnreadable = 255;
   }

   printf("byte %x\n",Closure->fillUnreadable);

}


static void bytefill_check_cb(GtkWidget *widget, gpointer data)
{  prefs_context *pc = (prefs_context*)data;
   int state  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

   if(state)
   {  const char *current;
      Closure->fillUnreadable = 0xb0;
      gtk_widget_set_sensitive(pc->byteEntry, TRUE);

      current = gtk_entry_get_text(GTK_ENTRY(pc->byteEntry));
      if(!*current || !strlen(current))
	gtk_entry_set_text(GTK_ENTRY(pc->byteEntry), "0xb0");
   }
   else
   {  Closure->fillUnreadable = -1;
      gtk_widget_set_sensitive(pc->byteEntry, FALSE);
   }
}

/***
 *** Assemble and open the preferences window.
 ***/

void CreatePreferencesWindow(void)
{  
   if(!Closure->prefsWindow)  /* No window to reuse? */
   {  GtkWidget *window, *outer_box, *hbox, *vbox, *vbox2, *notebook, *space, *button, *scale, *frame;
      GtkWidget *lab, *spin, *radio1, *radio2, *check, *entry;
      prefs_context *pc = g_malloc0(sizeof(prefs_context));
      Method *method;
 
      Closure->prefsContext = pc;
 
      window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      Closure->prefsWindow = GTK_WINDOW(window);
      gtk_window_set_title(GTK_WINDOW(window), _utf("Preferences"));
      gtk_window_set_default_size(GTK_WINDOW(window), -1, 150);
      gtk_window_set_icon(GTK_WINDOW(window), Closure->windowIcon);
      gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
      gtk_container_set_border_width(GTK_CONTAINER(window), 12);

      /* Connect with the close button from the window manager */

      g_signal_connect(window, "delete_event", G_CALLBACK(delete_cb), NULL);

      /* Create the main layout of the window */

      outer_box = gtk_vbox_new(FALSE, 0);
      gtk_container_add(GTK_CONTAINER(window), outer_box);

      notebook = gtk_notebook_new();
      gtk_box_pack_start(GTK_BOX(outer_box), notebook, TRUE, TRUE, 0);

      space = gtk_image_new();
      gtk_box_pack_start(GTK_BOX(outer_box), space, FALSE, FALSE, 4);

      hbox = gtk_hbox_new(FALSE, 0);
      gtk_box_pack_start(GTK_BOX(outer_box), hbox, FALSE, FALSE, 0);

      button = gtk_button_new_from_stock(GTK_STOCK_CLOSE);
      gtk_box_pack_end(GTK_BOX(hbox), button, FALSE, FALSE, 0);
      g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(close_cb), NULL);

      /*** "General" page */

      vbox = create_page(notebook, _utf("General"));

      /* medium/image file system */

      frame = gtk_frame_new(_utf("Medium and image filesystem"));
      gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

      button = gtk_check_button_new_with_label(_utf("Use information from ISO/UDF filesystem (experimental)"));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), Closure->parseUDF);
      g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(toggle_cb), GINT_TO_POINTER(TOGGLE_UDF));
      gtk_container_add(GTK_CONTAINER(frame), button);
      gtk_container_set_border_width(GTK_CONTAINER(button), 10);

      /* file extension */

      frame = gtk_frame_new(_utf("Local files (on hard disc)"));
      gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

      vbox2 = gtk_vbox_new(FALSE, 15);
      gtk_container_set_border_width(GTK_CONTAINER(vbox2), 10);
      gtk_container_add(GTK_CONTAINER(frame), vbox2);

      button = gtk_check_button_new_with_label(_utf("Automatically add .img and .ecc file suffixes"));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), Closure->autoSuffix);
      g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(toggle_cb), GINT_TO_POINTER(TOGGLE_SUFFIX));
      gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, FALSE, 0);

      /* 2GB button */

      button = gtk_check_button_new_with_label(_utf("Split files into segments <= 2GB"));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), Closure->splitFiles);
      g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(toggle_cb), GINT_TO_POINTER(TOGGLE_2GB));
      gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, FALSE, 0);

      /*** Automatic file creation and deletion */

      frame = gtk_frame_new(_utf("Automatic file creation and deletion"));
      gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

      vbox2 = gtk_vbox_new(FALSE, 15);
      gtk_container_set_border_width(GTK_CONTAINER(vbox2), 10);
      gtk_container_add(GTK_CONTAINER(frame), vbox2);

      /* automatic creation */

      button = gtk_check_button_new_with_label(_utf("Create error correction file after reading image"));
      pc->readAndCreateButton = button;
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), Closure->readAndCreate);
      g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(toggle_cb), GINT_TO_POINTER(TOGGLE_READ_CREATE));
      gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, FALSE, 0);

      /* automatic deletion */

      button = gtk_check_button_new_with_label(_utf("Remove image after error correction file creation"));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), Closure->unlinkImage);
      g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(toggle_cb), GINT_TO_POINTER(TOGGLE_UNLINK));
      gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, FALSE, 0);

      /*** Read & Scan page */

      vbox = create_page(notebook, _utf("Read & Scan"));

      /** Reading preferences */
      
      frame = gtk_frame_new(_utf("Reading preferences"));
      gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

      vbox2 = gtk_vbox_new(FALSE, 20);
      gtk_container_set_border_width(GTK_CONTAINER(vbox2), 10);
      gtk_container_add(GTK_CONTAINER(frame), vbox2);

      /* Reading strategy */

      hbox = gtk_hbox_new(FALSE, 4);

      lab = gtk_label_new(_utf("Reading strategy: ")); 
      gtk_box_pack_start(GTK_BOX(hbox), lab, FALSE, FALSE, 0);

      radio1 = pc->radioLinear = gtk_radio_button_new(NULL);
      g_signal_connect(G_OBJECT(radio1), "toggled", G_CALLBACK(strategy_cb), pc);
      gtk_box_pack_start(GTK_BOX(hbox), radio1, FALSE, FALSE, 0);
      lab = gtk_label_new(_utf("Linear"));
      gtk_container_add(GTK_CONTAINER(radio1), lab);

      radio2 = pc->radioAdaptive = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(radio1));
      g_signal_connect(G_OBJECT(radio2), "toggled", G_CALLBACK(strategy_cb), pc);
      gtk_box_pack_start(GTK_BOX(hbox), radio2, FALSE, FALSE, 0);
      lab = gtk_label_new(_utf("Adaptive (for defective media)"));
      gtk_container_add(GTK_CONTAINER(radio2), lab);

      gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
      if(Closure->adaptiveRead)
	   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio2), TRUE);
      else gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio1), TRUE);

      /* Reading range */

      hbox = gtk_hbox_new(FALSE, 4);

      pc->rangeToggle  = gtk_check_button_new_with_label(_utf("Read/Scan from sector"));
      g_signal_connect(G_OBJECT(pc->rangeToggle), "toggled", G_CALLBACK(toggle_cb), GINT_TO_POINTER(TOGGLE_RANGE));
      gtk_box_pack_start(GTK_BOX(hbox), pc->rangeToggle, FALSE, FALSE, 0);
      pc->rangeSpin1 = gtk_spin_button_new_with_range(0, 10000000, 1000);
      gtk_entry_set_width_chars(GTK_ENTRY(pc->rangeSpin1), 9);
      gtk_widget_set_sensitive(pc->rangeSpin1, FALSE);
      g_signal_connect(pc->rangeSpin1, "value-changed", G_CALLBACK(read_range_cb), pc);
      gtk_box_pack_start(GTK_BOX(hbox), pc->rangeSpin1, FALSE, FALSE, 0);

      pc->rangeLab = gtk_label_new(_utf("to sector"));
      gtk_box_pack_start(GTK_BOX(hbox), pc->rangeLab, FALSE, FALSE, 0);
      pc->rangeSpin2 = gtk_spin_button_new_with_range(0, 1, 10000);
      gtk_entry_set_width_chars(GTK_ENTRY(pc->rangeSpin2), 9);
      gtk_widget_set_sensitive(pc->rangeSpin2, FALSE);
      g_signal_connect(pc->rangeSpin2, "value-changed", G_CALLBACK(read_range_cb), pc);
      gtk_box_pack_start(GTK_BOX(hbox), pc->rangeSpin2, FALSE, FALSE, 0);

      gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->rangeToggle), FALSE);
      

      /* Jump selector */

      pc->formatLinear = g_strdup(_utf("Skip %d sectors after read error"));
      pc->formatAdaptive  = g_strdup(_utf("Stop reading when unreadable intervals < %d"));

      scale = non_linear_scale(&pc->jumpScaleInfo,
			       SLIDER_JUMP, jump_values, JUMP_VALUE_LENGTH,
			       Closure->sectorSkip);
      pc->jumpScaleInfo->format = Closure->adaptiveRead ? pc->formatAdaptive : pc->formatLinear;
      pc->jumpScale = scale;

      gtk_box_pack_start(GTK_BOX(vbox2), scale, FALSE, FALSE, 0);

      /** Image properties */

      frame = gtk_frame_new(_utf("Image properties"));
      gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

      vbox2 = gtk_vbox_new(FALSE, 15);
      gtk_container_set_border_width(GTK_CONTAINER(vbox2), 10);
      gtk_container_add(GTK_CONTAINER(frame), vbox2);

      /* DAO button */

      button = gtk_check_button_new_with_label(_utf("Assume image to be written in DAO mode (don't truncate)"));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), Closure->noTruncate);
      g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(toggle_cb), GINT_TO_POINTER(TOGGLE_DAO));
      gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, FALSE, 0);

      /* byte filling */
      
      hbox = gtk_hbox_new(FALSE, 4);

      pc->byteCheck = check = gtk_check_button_new();
      g_signal_connect(check, "toggled", G_CALLBACK(bytefill_check_cb), pc);
      gtk_box_pack_start(GTK_BOX(hbox), check, FALSE, FALSE, 0);
      lab = gtk_label_new(_utf("Fill unreadable sectors with byte:"));
      gtk_container_add(GTK_CONTAINER(check), lab);

      pc->byteEntry = entry = gtk_entry_new();
      g_signal_connect(entry, "activate", G_CALLBACK(bytefill_cb), pc);
      gtk_entry_set_width_chars(GTK_ENTRY(entry), 5);
      gtk_box_pack_start(GTK_BOX(hbox), entry, FALSE, FALSE, 0);

      gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);

      if(Closure->fillUnreadable >= 0)
      {  char value[11];
	
	 g_snprintf(value, 10, "0x%x", Closure->fillUnreadable);
	 gtk_entry_set_text(GTK_ENTRY(pc->byteEntry), value);
	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->byteCheck), TRUE);
      }
      else gtk_widget_set_sensitive(pc->byteEntry, FALSE);


      /** Drive initialisation */

      frame = gtk_frame_new(_utf("Drive initialisation"));
      gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

      hbox = gtk_hbox_new(FALSE, 4);
      gtk_container_add(GTK_CONTAINER(frame), hbox);
      gtk_container_set_border_width(GTK_CONTAINER(hbox), 10);

      lab = gtk_label_new(_utf("Wait"));
      gtk_box_pack_start(GTK_BOX(hbox), lab, FALSE, FALSE, 0);

      spin = gtk_spin_button_new_with_range(0, 30, 1);
      gtk_entry_set_width_chars(GTK_ENTRY(spin), 3);
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), Closure->spinupDelay);
      g_signal_connect(spin, "value-changed", G_CALLBACK(spin_cb), (gpointer)SPIN_DELAY);
      gtk_box_pack_start(GTK_BOX(hbox), spin, FALSE, FALSE, 0);

      lab = gtk_label_new(_utf("seconds for drive to spin up"));
      gtk_box_pack_start(GTK_BOX(hbox), lab, FALSE, FALSE, 0);

      /*** "Error correction" page */

      vbox = create_page(notebook, _utf("Error correction"));

      method = FindMethod(Closure->methodName); 
      if(method) method->createPrefsPage(method, vbox);

      gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 1);
   }

   /* Show the created / reused window */

   gtk_widget_show_all(GTK_WIDGET(Closure->prefsWindow));
}
