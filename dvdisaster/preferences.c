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
   struct _prefs_context *pc;
} non_linear_info;

/***
 *** Preferences window housekeeping
 ***/

typedef struct _prefs_context
{  //char **jumpFormat;
   char *formatLinear;
   char *formatAdaptive;

   GPtrArray *helpPages;   /* The online help frame structures */

  /* Widgets for changing preferences settings. The are two copies (A and B) 
     of each; one for the standard dialog and one embedded in the online help. */

   GtkWidget *radioDrive, *radioISO, *radioECC;
   GtkWidget *radioLinearA, *radioLinearB;
   GtkWidget *radioAdaptiveA, *radioAdaptiveB;
   GtkWidget *rawScaleA, *rawScaleB;
   GtkWidget *rangeToggleA, *rangeToggleB;
   GtkWidget *rangeSpin1A, *rangeSpin1B;
   GtkWidget *rangeSpin2A, *rangeSpin2B;
   GtkWidget *jumpScaleA, *jumpScaleB;
   GtkWidget *byteEntry, *byteCheck;
   GtkWidget *readAndCreateButton;
   GtkWidget *mainNotebook;
   GtkWidget *methodChooser;
   GtkWidget *methodNotebook;

   non_linear_info *jumpScaleInfo;
   non_linear_info *rawScaleInfo;
} prefs_context;

void FreePreferences(void *context)
{  prefs_context *pc = (prefs_context*)context;
   int i;

   for(i=0; i<pc->helpPages->len; i++)
   {  FrameWithOnlineHelp *fwoh = g_ptr_array_index(pc->helpPages,i);

      ClearFrameWithOnlineHelp(fwoh);
   }

   if(pc->formatLinear) g_free(pc->formatLinear);
   if(pc->formatAdaptive) g_free(pc->formatAdaptive);

   if(pc->jumpScaleInfo) g_free(pc->jumpScaleInfo);
   if(pc->rawScaleInfo) 
   {  g_free(pc->rawScaleInfo->format);
      g_free(pc->rawScaleInfo);
   }
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
   Method *method;
   int method_index;
   int i;

   /* Get reading range values */

   if(   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pc->rangeToggleA))   
      || gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pc->rangeToggleB)))   
   {     
      Closure->readStart = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(pc->rangeSpin1A));
      Closure->readEnd   = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(pc->rangeSpin2A));
   }
   else Closure->readStart = Closure->readEnd = 0;

   /* Get fill byte and recalculate the dead sector marker */

   if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pc->byteCheck)))   
   {  const char *value = gtk_entry_get_text(GTK_ENTRY(pc->byteEntry));

      Closure->fillUnreadable = strtol(value, NULL, 0);

      if(Closure->fillUnreadable < 0)
	Closure->fillUnreadable = 1;

      if(Closure->fillUnreadable > 255)
	Closure->fillUnreadable = 255;
   }
   PrepareDeadSector();

   /* Ask currently selected method to update its settings
      from the preferences */

   method_index = gtk_notebook_get_current_page(GTK_NOTEBOOK(pc->methodNotebook));
   method = g_ptr_array_index(Closure->methodList, method_index);
   if(method->readPreferences)
     method->readPreferences(method);

   /* hide preferences and finish */

   gtk_widget_hide(GTK_WIDGET(Closure->prefsWindow));

   for(i=0; i<pc->helpPages->len; i++)
   {  FrameWithOnlineHelp *fwoh = g_ptr_array_index(pc->helpPages,i);
      gtk_widget_hide(fwoh->helpWindow);
   }
}

static void close_cb(GtkWidget *widget, gpointer data)
{  
   HidePreferences();
}

/*
 * Actions used in the preferences
 */

enum 
{  TOGGLE_READ_CREATE,
   TOGGLE_UNLINK,
   TOGGLE_SUFFIX,
   TOGGLE_DAO,
   TOGGLE_2GB,
   TOGGLE_RANGE,

   SPIN_DELAY,
   
   SLIDER_JUMP,
   SLIDER_RAW_ATTEMPTS
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
   {  case TOGGLE_READ_CREATE:
	Closure->readAndCreate = state;
	if(state && Closure->adaptiveRead)  /* set reading strategy to linear */
	{  prefs_context *pc = Closure->prefsContext;

	   Closure->adaptiveRead = FALSE;
	   pc->jumpScaleInfo->format = pc->formatLinear;
	   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioLinearA), TRUE);
	   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioLinearB), TRUE);

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

	 gtk_widget_set_sensitive(pc->rangeSpin1A, state);
	 gtk_widget_set_sensitive(pc->rangeSpin1B, state);
	 gtk_widget_set_sensitive(pc->rangeSpin2A, state);
	 gtk_widget_set_sensitive(pc->rangeSpin2B, state);

	 gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin1A), 0.0);
	 gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin1B), 0.0);
	 
	 if(state) 
	 {  gtk_spin_button_set_range(GTK_SPIN_BUTTON(pc->rangeSpin2A), 0.0, image_size);
	    gtk_spin_button_set_range(GTK_SPIN_BUTTON(pc->rangeSpin2B), 0.0, image_size);
	    gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin2A), image_size);
	    gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin2B), image_size);
	 }
	 else
	 {  gtk_spin_button_set_range(GTK_SPIN_BUTTON(pc->rangeSpin2A), 0.0, 1.0);
	    gtk_spin_button_set_range(GTK_SPIN_BUTTON(pc->rangeSpin2B), 0.0, 1.0);
	    gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin2A), 0.1);
	    gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin2B), 0.1);
	 }

	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->rangeToggleA), state);
	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->rangeToggleB), state);
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

   if(pc->rangeSpin1A == widget || pc->rangeSpin2A == widget)
   {  int from = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(pc->rangeSpin1A));
      int to = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(pc->rangeSpin2A));

      if(from > to)
      {  if(widget == pc->rangeSpin1A)
	 {    gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin1A), to);
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin1B), to);
	 }
	 else 
	 {    gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin2A), from);
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin2B), from);
	 }
      }
      else 
      {    gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin1B), from);
	   gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin2B), to);
      }
   }
   else
   {  int from = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(pc->rangeSpin1B));
      int to = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(pc->rangeSpin2B));

      if(from > to)
      {  if(widget == pc->rangeSpin1B)
	 {    gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin1A), to);
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin1B), to);
	 }
	 else 
	 {    gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin2A), from);
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin2B), from);
	 }
      }
      else 
      {    gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin1A), from);
	   gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->rangeSpin2A), to);
      }
   }
}

/***
 *** Non-linear scales
 ***/

static int jump_values[] = { 0, 16, 32, 64, 128, 256, 384, 512, 768, 1024, 2048, 4096, 10240,
 20480 };
#define JUMP_VALUE_LENGTH 14

static int raw_values[] = { 0, 5, 7, 9, 15, 20, 25, 30}; 
#define RAW_VALUE_LENGTH 8

static void non_linear_cb(GtkWidget *widget, gpointer data)
{  non_linear_info *nli = (non_linear_info*)data;
   int index  = gtk_range_get_value(GTK_RANGE(widget));

   switch(nli->action)
   {  case SLIDER_JUMP:
        Closure->sectorSkip = nli->values[index];
	gtk_range_set_value(GTK_RANGE(nli->pc->jumpScaleA), index);
	gtk_range_set_value(GTK_RANGE(nli->pc->jumpScaleB), index);
	break;

      case SLIDER_RAW_ATTEMPTS:
        Closure->rawAttempts = nli->values[index];
	gtk_range_set_value(GTK_RANGE(nli->pc->rawScaleA), index);
	gtk_range_set_value(GTK_RANGE(nli->pc->rawScaleB), index);
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

static GtkWidget* non_linear_scale(prefs_context *pc, non_linear_info **nli_ptr, 
				   int action, int *values, int n, int value)
{  GtkWidget *scale;
   non_linear_info *nli;
   int index;

   for(index = 0; index < n; index++)
     if(values[index] > value)
       break;

   if(!*nli_ptr)
   {  nli = g_malloc0(sizeof(non_linear_info));
      nli->action = action;
      nli->values = values;
      nli->pc     = pc;
      *nli_ptr = nli;
   }
   else nli = *nli_ptr;

   scale = gtk_hscale_new_with_range(0,n-1,1);
   gtk_scale_set_value_pos(GTK_SCALE(scale), GTK_POS_RIGHT);
   gtk_range_set_increments(GTK_RANGE(scale), 1, 1);
   gtk_range_set_value(GTK_RANGE(scale), index > 0 ? index-1 : index);
   g_signal_connect(scale, "format-value", G_CALLBACK(non_linear_format_cb), nli);
   g_signal_connect(scale, "value-changed", G_CALLBACK(non_linear_cb), nli);

   return scale;
}

/*
 * Image size query method selection 
 */

static void imgsize_cb(GtkWidget *widget, gpointer data)
{  int state  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
   prefs_context *pc = (prefs_context*)data;

   if(state == TRUE)
   {  if(pc->radioDrive == widget) Closure->querySize = 0;
      if(pc->radioISO   == widget) Closure->querySize = 1;
      if(pc->radioECC   == widget) Closure->querySize = 2;
   }
}

/*
 * Read strategy selection 
 */

static void strategy_cb(GtkWidget *widget, gpointer data)
{  int state  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
   prefs_context *pc = (prefs_context*)data;

   if(pc->jumpScaleA && pc->jumpScaleB && state == TRUE)
   {  double tmp;

      if(pc->radioLinearA == widget || pc->radioLinearB == widget)
      {  Closure->adaptiveRead = FALSE;
         pc->jumpScaleInfo->format = pc->formatLinear;
	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioLinearA), TRUE);
	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioLinearB), TRUE);
      }

      if(pc->radioAdaptiveA == widget || pc->radioAdaptiveB == widget)
      {  Closure->adaptiveRead = TRUE;
         pc->jumpScaleInfo->format = pc->formatAdaptive;
	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioAdaptiveA), TRUE);
	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioAdaptiveB), TRUE);

	 if(Closure->readAndCreate)
	 {  Closure->readAndCreate = FALSE;

	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->readAndCreateButton), FALSE);
	    ShowMessage(Closure->prefsWindow,
			_("Disabled automatic error correction file generation."), 
			GTK_MESSAGE_INFO);
	 }
      }

      /* poor hack to make the range widgets redraw */

      tmp = gtk_range_get_value(GTK_RANGE(pc->jumpScaleA));
      gtk_range_set_value(GTK_RANGE(pc->jumpScaleA), tmp+1.0);
      gtk_range_set_value(GTK_RANGE(pc->jumpScaleA), tmp);
      gtk_range_set_value(GTK_RANGE(pc->jumpScaleB), tmp+1.0);
      gtk_range_set_value(GTK_RANGE(pc->jumpScaleB), tmp);
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
 *** Error correction method selection
 ***/

static void method_select_cb(GtkWidget *widget, gpointer data)
{  Method *method;
   prefs_context *pc = (prefs_context*)data;
   int n;

#if GTK_MINOR_VERSION >= 4
   n = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
#else
   n = gtk_option_menu_get_history(GTK_OPTION_MENU(widget));
#endif

   if(n<0 || !pc->methodNotebook)
     return;

   method = g_ptr_array_index(Closure->methodList, n);

   /* Switch methods if selection changed */

   if(strncmp(Closure->methodName, method->name, 4))
   {  strncpy(Closure->methodName, method->name, 4);
      gtk_notebook_set_current_page(GTK_NOTEBOOK(pc->methodNotebook), n);
   }
}

/*
 * Setting the notebook page does not work at creation time.
 */

static gboolean notebook_idle_func(gpointer data)
{  prefs_context *pc = (prefs_context*)data;
   int n;

#if GTK_MINOR_VERSION >= 4
   n = gtk_combo_box_get_active(GTK_COMBO_BOX(pc->methodChooser));
#else
   n = gtk_option_menu_get_history(GTK_OPTION_MENU(pc->methodChooser));
#endif

   if(n>=0)
     gtk_notebook_set_current_page(GTK_NOTEBOOK(pc->methodNotebook), n);

   gtk_notebook_set_current_page(GTK_NOTEBOOK(pc->mainNotebook), 0);

   return FALSE;
}

/***
 *** Assemble and open the preferences window.
 ***/

void UpdateMethodPreferences(void)
{  int i;
   
   for(i=0; i<Closure->methodList->len; i++)
   {  Method *method = g_ptr_array_index(Closure->methodList, i);

      method->resetPrefsPage(method);
   }
}

void CreatePreferencesWindow(void)
{  
   if(!Closure->prefsWindow)  /* No window to reuse? */
   {  GtkWidget *window, *outer_box, *hbox, *vbox, *vbox2, *notebook, *space, *button, *frame;
      GtkWidget *lab, *spin, *radio1, *radio2, *radio3, *check, *entry;
#if GTK_MINOR_VERSION < 4
      GtkWidget *option_menu_strip;
#endif
      FrameWithOnlineHelp *fwoh;
      prefs_context *pc = g_malloc0(sizeof(prefs_context));
      int i, method_idx = 0;
 
      pc->helpPages = g_ptr_array_new();
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

      notebook = pc->mainNotebook = gtk_notebook_new();
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

      vbox2 = gtk_vbox_new(FALSE, 15);
      gtk_container_set_border_width(GTK_CONTAINER(vbox2), 10);
      gtk_container_add(GTK_CONTAINER(frame), vbox2);

      hbox = gtk_hbox_new(FALSE, 4);

      lab = gtk_label_new(_utf("Get Image size from: ")); 
      gtk_box_pack_start(GTK_BOX(hbox), lab, FALSE, FALSE, 0);

      radio1 = pc->radioDrive = gtk_radio_button_new(NULL);
      g_signal_connect(G_OBJECT(radio1), "toggled", G_CALLBACK(imgsize_cb), pc);
      gtk_box_pack_start(GTK_BOX(hbox), radio1, FALSE, FALSE, 0);
      lab = gtk_label_new(_utf("Drive"));
      gtk_container_add(GTK_CONTAINER(radio1), lab);

      radio2 = pc->radioISO = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(radio1));
      g_signal_connect(G_OBJECT(radio2), "toggled", G_CALLBACK(imgsize_cb), pc);
      gtk_box_pack_start(GTK_BOX(hbox), radio2, FALSE, FALSE, 0);
      lab = gtk_label_new(_utf("ISO/UDF"));
      gtk_container_add(GTK_CONTAINER(radio2), lab);

      radio3 = pc->radioECC = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(radio2));
      g_signal_connect(G_OBJECT(radio3), "toggled", G_CALLBACK(imgsize_cb), pc);
      gtk_box_pack_start(GTK_BOX(hbox), radio3, FALSE, FALSE, 0);
      lab = gtk_label_new(_utf("ECC/RS02"));
      gtk_container_add(GTK_CONTAINER(radio3), lab);

      switch(Closure->querySize)
      {  case 0: gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio1), TRUE); break;
         case 1: gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio2), TRUE); break;
         case 2: gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio3), TRUE); break;
      }

      gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);

      /* file extension */

      frame = gtk_frame_new(_utf("Local files (on hard disc)"));
      gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

      vbox2 = gtk_vbox_new(FALSE, 15);
      gtk_container_set_border_width(GTK_CONTAINER(vbox2), 10);
      gtk_container_add(GTK_CONTAINER(frame), vbox2);

      button = gtk_check_button_new_with_label(_utf("Automatically add .iso and .ecc file suffixes"));
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
      
      fwoh = CreateFrameWithOnlineHelp(_("Reading preferences"));
      g_ptr_array_add(pc->helpPages, fwoh);
      frame = fwoh->clientFrame;
      gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

      vbox2 = gtk_vbox_new(FALSE, 20);
      gtk_container_set_border_width(GTK_CONTAINER(vbox2), 10);
      gtk_container_add(GTK_CONTAINER(frame), vbox2);

      /* Reading strategy */

      AddHelpParagraph(fwoh, 
		       _("<b>Reading strategy</b>\n"
			 "dvdisaster provides two basic strategies for reading CD/DVD media:"));

      for(i=0; i<2; i++)
      {  GtkWidget *hbox = gtk_hbox_new(FALSE, 4);
	 GtkWidget *lab, *radio1, *radio2;

         lab = gtk_label_new(_utf("Reading strategy: ")); 
	 gtk_box_pack_start(GTK_BOX(hbox), lab, FALSE, FALSE, 0);

	 radio1 = gtk_radio_button_new(NULL);
	 if(!i) pc->radioLinearA = radio1;
	 else   pc->radioLinearB = radio1;
	 g_signal_connect(G_OBJECT(radio1), "toggled", G_CALLBACK(strategy_cb), pc);
	 gtk_box_pack_start(GTK_BOX(hbox), radio1, FALSE, FALSE, 0);
	 lab = gtk_label_new(_utf("Linear"));
	 gtk_container_add(GTK_CONTAINER(radio1), lab);

	 radio2 = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(radio1));
	 if(!i) pc->radioAdaptiveA = radio2;
	 else   pc->radioAdaptiveB = radio2;
 	 g_signal_connect(G_OBJECT(radio2), "toggled", G_CALLBACK(strategy_cb), pc);
	 gtk_box_pack_start(GTK_BOX(hbox), radio2, FALSE, FALSE, 0);
	 lab = gtk_label_new(_utf("Adaptive (for defective media)"));
	 gtk_container_add(GTK_CONTAINER(radio2), lab);

	 if(Closure->adaptiveRead)
	      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio2), TRUE);
         else gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio1), TRUE);

	 if(!i) gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
	 else   AddHelpWidget(fwoh, hbox);
      }

      AddHelpParagraph(fwoh, 
		       _("The linear strategy is suited for undamaged media while the adaptive strategy\n"
			 "is better for media already containing read errors.\n"
			 "Both reading strategies can be further customized:\n"));

      /* Reading attempts */

      AddHelpParagraph(fwoh, 
		       _("<b>Raw reading</b>\n"
			 "Most drives return no data at all for defective sectors, but some have a special\n"
			 "raw reading mode which returns even partly readable sectors. In that case it\n"
			 "can be possible to recombine sector contents from several raw reading attempts."));

      pc->rawScaleInfo = NULL;

      for(i=0; i<2; i++)
      {  GtkWidget *scale;
 
	 scale = non_linear_scale(pc, &pc->rawScaleInfo,
				  SLIDER_RAW_ATTEMPTS, raw_values, RAW_VALUE_LENGTH,
				  Closure->rawAttempts);

	 if(!i) pc->rawScaleInfo->format = g_strdup(_("Try %d raw reads after read error (if supported)"));
	 if(!i) pc->rawScaleA = scale;
	 else   pc->rawScaleB = scale;

	 if(!i) gtk_box_pack_start(GTK_BOX(vbox2), scale, FALSE, FALSE, 0);
	 else   AddHelpWidget(fwoh, scale);
      }

      /* Reading range */

      AddHelpParagraph(fwoh, 
		       _("<b>Reading range</b>\n"
			 "Reading can be limited to a part of the medium (in sectors holding 2KB each).\n"
			 "The values include the borders: 0-100 will read 101 sectors."));

      for(i=0; i<2; i++)
      {  GtkWidget *hbox = gtk_hbox_new(FALSE, 4);
	 GtkWidget *toggle,*spin1, *spin2;

	 toggle = gtk_check_button_new_with_label(_utf("Read/Scan from sector"));
	 if(!i) pc->rangeToggleA = toggle;
	 else   pc->rangeToggleB = toggle;
	 g_signal_connect(G_OBJECT(toggle), "toggled", G_CALLBACK(toggle_cb), GINT_TO_POINTER(TOGGLE_RANGE));
	 gtk_box_pack_start(GTK_BOX(hbox), toggle, FALSE, FALSE, 0);

	 spin1 = gtk_spin_button_new_with_range(0, 10000000, 1000);
	 if(!i) pc->rangeSpin1A = spin1;	 
	 else   pc->rangeSpin1B = spin1;
	 gtk_entry_set_width_chars(GTK_ENTRY(spin1), 9);
	 gtk_widget_set_sensitive(spin1, FALSE);
	 g_signal_connect(spin1, "value-changed", G_CALLBACK(read_range_cb), pc);
	 gtk_box_pack_start(GTK_BOX(hbox), spin1, FALSE, FALSE, 0);

	 lab = gtk_label_new(_utf("to sector"));
	 gtk_box_pack_start(GTK_BOX(hbox), lab, FALSE, FALSE, 0);

	 spin2 = gtk_spin_button_new_with_range(0, 1, 10000);
	 if(!i) pc->rangeSpin2A = spin2;	 
	 else   pc->rangeSpin2B = spin2;
	 gtk_entry_set_width_chars(GTK_ENTRY(spin2), 9);
	 gtk_widget_set_sensitive(spin2, FALSE);
	 g_signal_connect(spin2, "value-changed", G_CALLBACK(read_range_cb), pc);
	 gtk_box_pack_start(GTK_BOX(hbox), spin2, FALSE, FALSE, 0);

	 if(!i) gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
	 else   AddHelpWidget(fwoh, hbox);

	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle), FALSE);
      }

      AddHelpParagraph(fwoh, 
		       _("Limiting the reading range is not recommended for <i>adaptive reading</i> since it might\n"
			 "prevent sectors from being read which are required for a succesful error correction.\n"
			 "These settings are only effective for the current session and will not be saved.\n"));

      /* Jump selector */

      AddHelpParagraph(fwoh, 
		       _("<b>Treatment of unreadable areas</b>\n"
			 "Defective media usually contain numerous read errors in a contigous region.\n"
			 "Therefore skipping sectors after a read error reduces the amount of failed read\n"
			 "attempts. <i>Larger values</i> reduce the processing time and the mechanical wear\n"
			 "on the drive, but leave <i>larger gaps</i> in the image when reading defective areas."));

      pc->formatLinear = g_strdup(_utf("Skip %d sectors after read error"));
      pc->formatAdaptive  = g_strdup(_utf("Stop reading when unreadable intervals < %d"));
      pc->jumpScaleInfo = NULL;

      for(i=0; i<2; i++)
      {  GtkWidget *scale;
 
	 scale = non_linear_scale(pc, &pc->jumpScaleInfo,
				  SLIDER_JUMP, jump_values, JUMP_VALUE_LENGTH,
				  Closure->sectorSkip);

	 pc->jumpScaleInfo->format = Closure->adaptiveRead ? pc->formatAdaptive : pc->formatLinear;
	 if(!i) pc->jumpScaleA = scale;
	 else   pc->jumpScaleB = scale;

	 if(!i) gtk_box_pack_start(GTK_BOX(vbox2), scale, FALSE, FALSE, 0);
	 else   AddHelpWidget(fwoh, scale);
      }

      AddHelpParagraph(fwoh, 
		       _("When using the <i>linear reading strategy</i>, the given number of sectors will be\n"
			 "skipped after the occurance of a read error. This has the following consequences:\n\n" 
			 "- Skipping a large number of sectors (e.g. 1024) gives a quick overview of\n"
			 "- damaged areas, but will usually not collect enough data for repairing the image.\n"
			 "- Smaller values like 16, 32 or 64 are a good trade-off: The processing time will be\n"
			 "- considerably shortened, but still enough data for repairing the image is collected.\n\n"

			 "The <i>adaptive reading strategy</i> uses this setting only if no error correction data\n"
			 "is available. In that case the reading process will stop when no unread areas\n"
			 "larger than the selected size remain. Values smaller than 128 <i>are not recommended</i>\n"
			 "as they cause the drive to carry out lots of laser head repositioning during the\n"
			 "final phase of the reading process. If adaptive reading with a setting of 128 is not\n"
			 "sufficient, try reading the remaining sectors with an additional linear reading pass.\n\n"

			 "On DVD media read errors do usually extend over at least 16 sectors for technical\n"
			 "reasons. Therefore selecting a value less than 16 is not recommended for DVD."
			 ));

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

      /* Method chooser menu */

      vbox = create_page(notebook, _utf("Error correction"));

      hbox = gtk_hbox_new(FALSE, 4);
      lab = gtk_label_new(_utf("Storage method:")); 
      gtk_box_pack_start(GTK_BOX(hbox), lab, FALSE, FALSE, 0);
      gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

#if GTK_MINOR_VERSION >= 4
      pc->methodChooser = gtk_combo_box_new_text();

      g_signal_connect(G_OBJECT(pc->methodChooser), "changed", G_CALLBACK(method_select_cb), pc);

      for(i=0; i<Closure->methodList->len; i++)
      {  Method *method = g_ptr_array_index(Closure->methodList, i);

	 gtk_combo_box_append_text(GTK_COMBO_BOX(pc->methodChooser), method->menuEntry); 

	 if(!strncmp(Closure->methodName, method->name, 4))
	   method_idx = i;
      }


      gtk_combo_box_set_active(GTK_COMBO_BOX(pc->methodChooser), method_idx);
      gtk_box_pack_start(GTK_BOX(hbox), pc->methodChooser, FALSE, FALSE, 0);
#else
      pc->methodChooser = gtk_option_menu_new();

      g_signal_connect(G_OBJECT(pc->methodChooser), "changed", G_CALLBACK(method_select_cb), pc);
      option_menu_strip = gtk_menu_new(); 

      for(i=0; i<Closure->methodList->len; i++)
      {  Method *method = g_ptr_array_index(Closure->methodList, i);
	 GtkWidget *item;

	 item = gtk_menu_item_new_with_label(method->menuEntry);
	 gtk_menu_shell_append(GTK_MENU_SHELL(option_menu_strip), item);

	 if(!strncmp(Closure->methodName, method->name, 4))
	   method_idx = i;
      }

      gtk_option_menu_set_menu(GTK_OPTION_MENU(pc->methodChooser), option_menu_strip);
      gtk_option_menu_set_history(GTK_OPTION_MENU(pc->methodChooser), method_idx);
      gtk_box_pack_start(GTK_BOX(hbox), pc->methodChooser, FALSE, FALSE, 0);
#endif

      /* sub pages for individual method configuration */
      
      pc->methodNotebook = gtk_notebook_new();
      gtk_notebook_set_show_tabs(GTK_NOTEBOOK(pc->methodNotebook), FALSE);
      gtk_notebook_set_show_border(GTK_NOTEBOOK(pc->methodNotebook), FALSE);
      gtk_box_pack_start(GTK_BOX(vbox), pc->methodNotebook, TRUE, TRUE, 0);

      for(i=0; i<Closure->methodList->len; i++)
      {  Method *method = g_ptr_array_index(Closure->methodList, i);
	 GtkWidget *vbox2 = gtk_vbox_new(FALSE, 0);
	 GtkWidget *ignore = gtk_label_new("method_tab");

	 if(method->createPrefsPage)
	    method->createPrefsPage(method, vbox2);
	 else
	 {  GtkWidget *lab;

	    lab = gtk_label_new("This method has no configuration options.");
	    gtk_box_pack_start(GTK_BOX(vbox2), lab, TRUE, TRUE, 0);
	 }

	 gtk_notebook_append_page(GTK_NOTEBOOK(pc->methodNotebook), vbox2, ignore);
	 if(i==method_idx)
      gtk_notebook_set_current_page(GTK_NOTEBOOK(pc->methodNotebook), method_idx);
      }

      g_idle_add(notebook_idle_func, pc);
   }

   /* Show the created / reused window */

   gtk_widget_show_all(GTK_WIDGET(Closure->prefsWindow));
}
