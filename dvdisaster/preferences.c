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
{  GtkWidget *label;  /* for help system linkage */
   LabelWithOnlineHelp *lwoh;   
   int action;
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

   GtkWidget *radioDriveA, *radioDriveB;
   GtkWidget *radioISOA, *radioISOB;
   GtkWidget *radioECCA, *radioECCB;
   GtkWidget *suffixA, *suffixB;
   GtkWidget *splitA, *splitB;
   GtkWidget *radioLinearA, *radioLinearB;
   GtkWidget *radioAdaptiveA, *radioAdaptiveB;
   GtkWidget *minAttemptsScaleA, *minAttemptsScaleB;
   GtkWidget *maxAttemptsScaleA, *maxAttemptsScaleB;
   GtkWidget *rangeToggleA, *rangeToggleB;
   GtkWidget *rangeSpin1A, *rangeSpin1B;
   GtkWidget *rangeSpin2A, *rangeSpin2B;
   GtkWidget *rawButtonA, *rawButtonB;
   GtkWidget *jumpScaleA, *jumpScaleB;
   GtkWidget *daoButtonA, *daoButtonB;
   GtkWidget *byteEntryA, *byteEntryB;
   GtkWidget *byteCheckA, *byteCheckB;
   GtkWidget *spinUpA, *spinUpB;
   GtkWidget *readAndCreateButtonA, *readAndCreateButtonB;
   GtkWidget *unlinkImageButtonA, *unlinkImageButtonB;
   GtkWidget *mainNotebook;
   GtkWidget *methodChooserA,*methodChooserB;
   GtkWidget *methodNotebook;

   non_linear_info *jumpScaleInfoA, *jumpScaleInfoB;
   LabelWithOnlineHelp *jumpScaleLwoh;

   non_linear_info *minAttemptsScaleInfoA, *minAttemptsScaleInfoB;
   non_linear_info *maxAttemptsScaleInfoA, *maxAttemptsScaleInfoB;
   LabelWithOnlineHelp *minAttemptsScaleLwoh, *maxAttemptsScaleLwoh;
} prefs_context;

void FreePreferences(void *context)
{  prefs_context *pc = (prefs_context*)context;
   int i;

   for(i=0; i<pc->helpPages->len; i++)
   {  LabelWithOnlineHelp *lwoh = g_ptr_array_index(pc->helpPages,i);

      FreeLabelWithOnlineHelp(lwoh);
   }

   if(pc->formatLinear) g_free(pc->formatLinear);
   if(pc->formatAdaptive) g_free(pc->formatAdaptive);

   if(pc->jumpScaleInfoA) g_free(pc->jumpScaleInfoA);
   if(pc->jumpScaleInfoB) g_free(pc->jumpScaleInfoB);
   if(pc->minAttemptsScaleInfoA->format) g_free(pc->minAttemptsScaleInfoA->format);
   if(pc->minAttemptsScaleInfoB->format) g_free(pc->minAttemptsScaleInfoB->format);
   if(pc->minAttemptsScaleInfoA) g_free(pc->minAttemptsScaleInfoA);
   if(pc->minAttemptsScaleInfoB) g_free(pc->minAttemptsScaleInfoB);
   if(pc->maxAttemptsScaleInfoA->format) g_free(pc->maxAttemptsScaleInfoA->format);
   if(pc->maxAttemptsScaleInfoB->format) g_free(pc->maxAttemptsScaleInfoB->format);
   if(pc->maxAttemptsScaleInfoA) g_free(pc->maxAttemptsScaleInfoA);
   if(pc->maxAttemptsScaleInfoB) g_free(pc->maxAttemptsScaleInfoB);

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

   if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pc->byteCheckA)))   
   {  const char *value1 = gtk_entry_get_text(GTK_ENTRY(pc->byteEntryA));
      const char *value2 = gtk_entry_get_text(GTK_ENTRY(pc->byteEntryB));
      int v1 = strtol(value1, NULL, 0);
      int v2 = strtol(value2, NULL, 0);

      /* both field may contain different values */

      if(Closure->fillUnreadable != v2)
      {  Closure->fillUnreadable = v2;
 	 gtk_entry_set_text(GTK_ENTRY(pc->byteEntryA), value2);
      }
      else
	if(Closure->fillUnreadable != v1)
	{  Closure->fillUnreadable = v1;
	   gtk_entry_set_text(GTK_ENTRY(pc->byteEntryB), value1);
	}

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
   {  LabelWithOnlineHelp *lwoh = g_ptr_array_index(pc->helpPages,i);
      gtk_widget_hide(lwoh->helpWindow);
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
   TOGGLE_RAW,

   SPIN_DELAY,
   
   SLIDER_JUMP,
   SLIDER_MIN_READ_ATTEMPTS,
   SLIDER_MAX_READ_ATTEMPTS
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
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->readAndCreateButtonA), state);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->readAndCreateButtonB), state);
	if(state && Closure->adaptiveRead)  /* set reading strategy to linear */
	{  prefs_context *pc = Closure->prefsContext;

	   Closure->adaptiveRead = FALSE;
	   pc->jumpScaleInfoA->format = pc->formatLinear;
	   pc->jumpScaleInfoB->format = pc->formatLinear;
	   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioLinearA), TRUE);
	   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioLinearB), TRUE);

	   ShowMessage(Closure->prefsWindow,
		       _("Switched to the linear reading strategy."), 
		       GTK_MESSAGE_INFO);
	}
	   
	break;

      case TOGGLE_UNLINK:
	Closure->unlinkImage = state;
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->unlinkImageButtonA), state);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->unlinkImageButtonB), state);
	break;

      case TOGGLE_SUFFIX:
	Closure->autoSuffix = state;
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->suffixA), state);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->suffixB), state);
	break;

      case TOGGLE_DAO:
	Closure->noTruncate = state;
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->daoButtonA), state);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->daoButtonB), state);
	break;

      case TOGGLE_2GB:
	Closure->splitFiles = state;
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->splitA), state);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->splitB), state);
	break;

      case TOGGLE_RAW:
	Closure->readRaw = state;
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->rawButtonA), state);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->rawButtonB), state);
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
   prefs_context *pc = (prefs_context*)Closure->prefsContext;

   switch(which)
   {  case SPIN_DELAY:
	Closure->spinupDelay = value;
	if(widget == pc->spinUpA)
	{  if(pc->spinUpB)
	     gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->spinUpB), value);
	}
	if(widget == pc->spinUpB)
	{  if(pc->spinUpA)
	     gtk_spin_button_set_value(GTK_SPIN_BUTTON(pc->spinUpA), value);
	}
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

static int min_attempts_values[] = { 1, 2, 3, 4, 5, 7, 9, 11, 15, 20, 25, 30, 40, 50}; 
static int max_attempts_values[] = { 1, 2, 3, 4, 5, 7, 9, 11, 15, 20, 25, 30, 40, 50, 60, 70, 80, 90, 100}; 
#define MIN_ATTEMPTS_VALUE_LENGTH 14
#define MAX_ATTEMPTS_VALUE_LENGTH 19

static void non_linear_cb(GtkWidget *widget, gpointer data)
{  non_linear_info *nli = (non_linear_info*)data;
   int index  = gtk_range_get_value(GTK_RANGE(widget));
   char *text,*utf;

   text = g_strdup_printf(nli->format, nli->values[index]);
   utf = g_locale_to_utf8(text, -1, NULL, NULL, NULL);

   switch(nli->action)
   {  case SLIDER_JUMP:
        Closure->sectorSkip = nli->values[index];

	gtk_range_set_value(GTK_RANGE(nli->pc->jumpScaleB), index);
	gtk_label_set_markup(GTK_LABEL(nli->pc->jumpScaleInfoB->label), utf);

	gtk_range_set_value(GTK_RANGE(nli->pc->jumpScaleA), index);
	gtk_label_set_markup(GTK_LABEL(nli->pc->jumpScaleInfoA->label), utf);
	SetOnlineHelpLinkText(nli->pc->jumpScaleLwoh, text);
	break;

      case SLIDER_MIN_READ_ATTEMPTS:
      { int max  = gtk_range_get_value(GTK_RANGE(nli->pc->maxAttemptsScaleA));

        Closure->minReadAttempts = nli->values[index];

	gtk_range_set_value(GTK_RANGE(nli->pc->minAttemptsScaleB), index);
	gtk_label_set_markup(GTK_LABEL(nli->pc->minAttemptsScaleInfoB->label), utf);

	gtk_range_set_value(GTK_RANGE(nli->pc->minAttemptsScaleA), index);
	gtk_label_set_markup(GTK_LABEL(nli->pc->minAttemptsScaleInfoA->label), utf);
	SetOnlineHelpLinkText(nli->pc->minAttemptsScaleLwoh, text);

	if(index > max)
	{
	   gtk_range_set_value(GTK_RANGE(nli->pc->maxAttemptsScaleA), index);
	   gtk_range_set_value(GTK_RANGE(nli->pc->maxAttemptsScaleB), index);
	}
      }
      break;

      case SLIDER_MAX_READ_ATTEMPTS:
      { int min  = gtk_range_get_value(GTK_RANGE(nli->pc->minAttemptsScaleA));

        Closure->maxReadAttempts = nli->values[index];

	gtk_range_set_value(GTK_RANGE(nli->pc->maxAttemptsScaleB), index);
	gtk_label_set_markup(GTK_LABEL(nli->pc->maxAttemptsScaleInfoB->label), utf);

	gtk_range_set_value(GTK_RANGE(nli->pc->maxAttemptsScaleA), index);
	gtk_label_set_markup(GTK_LABEL(nli->pc->maxAttemptsScaleInfoA->label), utf);
	SetOnlineHelpLinkText(nli->pc->maxAttemptsScaleLwoh, text);

	if(index < min)
	{
	   gtk_range_set_value(GTK_RANGE(nli->pc->minAttemptsScaleA), index);
	   gtk_range_set_value(GTK_RANGE(nli->pc->minAttemptsScaleB), index);
	}
      }
      break;
   }

   g_free(utf);
   g_free(text);
}

static gchar* non_linear_format_cb(GtkScale *scale, gdouble value, gpointer data)
{  char *text;

   text = g_strdup(" ");
   FORGET(text);   /* The scale will free the old string by itself. Weird. */

   return text;
}

static GtkWidget* non_linear_scale(GtkWidget **hbox_out, non_linear_info *nli, 
				   GtkWidget *label, prefs_context *pc, 
				   int action, int *values, int n, int value)
{  GtkWidget *scale;
   GtkWidget *hbox;
   char *text,*utf;
   int index;

   for(index = 0; index < n; index++)
     if(values[index] > value)
       break;

   nli->action = action;
   nli->values = values;
   nli->pc     = pc;

   hbox = gtk_hbox_new(FALSE, 0);
   scale = gtk_hscale_new_with_range(0,n-1,1);
   gtk_scale_set_value_pos(GTK_SCALE(scale), GTK_POS_RIGHT);
   gtk_range_set_increments(GTK_RANGE(scale), 1, 1);
   gtk_range_set_value(GTK_RANGE(scale), index > 0 ? index-1 : index);
   g_signal_connect(scale, "format-value", G_CALLBACK(non_linear_format_cb), nli);
   g_signal_connect(scale, "value-changed", G_CALLBACK(non_linear_cb), nli);

   gtk_box_pack_start(GTK_BOX(hbox), scale, TRUE, TRUE, 0);

   text = g_strdup_printf(nli->format, nli->values[index > 0 ? index-1 : index]);
   utf = g_locale_to_utf8(text, -1, NULL, NULL, NULL);
   nli->label = label;
   gtk_label_set_markup(GTK_LABEL(label), utf);

   if(nli->lwoh)
   {    SetOnlineHelpLinkText(nli->lwoh, text);
        gtk_box_pack_start(GTK_BOX(hbox), nli->lwoh->linkBox, FALSE, FALSE, 0);
   }
   else gtk_box_pack_start(GTK_BOX(hbox), nli->label, FALSE, FALSE, 0);
   g_free(utf);
   g_free(text);

   *hbox_out = hbox;
   return scale;
}

/*
 * Image size query method selection 
 */

static void imgsize_cb(GtkWidget *widget, gpointer data)
{  int state  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
   int selection = GPOINTER_TO_INT(data);
   prefs_context *pc = (prefs_context*)Closure->prefsContext;

   if(!state)  /* only track changes to activate state */
     return;

   Closure->querySize = selection;

   switch(selection)
   {  case 0:
        if(pc->radioDriveA)
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioDriveA), TRUE); 
        if(pc->radioDriveB)
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioDriveB), TRUE); 
	break;

      case 1:
        if(pc->radioISOA)
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioISOA), TRUE); 
        if(pc->radioISOB)
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioISOB), TRUE); 
        break;

      case 2:
        if(pc->radioECCA)
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioECCA), TRUE); 
        if(pc->radioECCB)
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioECCB), TRUE); 
        break;
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
         pc->jumpScaleInfoA->format = pc->formatLinear;
         pc->jumpScaleInfoB->format = pc->formatLinear;
	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioLinearA), TRUE);
	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioLinearB), TRUE);
      }

      if(pc->radioAdaptiveA == widget || pc->radioAdaptiveB == widget)
      {  Closure->adaptiveRead = TRUE;
         pc->jumpScaleInfoA->format = pc->formatAdaptive;
         pc->jumpScaleInfoB->format = pc->formatAdaptive;
	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioAdaptiveA), TRUE);
	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->radioAdaptiveB), TRUE);

	 if(Closure->readAndCreate)
	 {  Closure->readAndCreate = FALSE;

	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->readAndCreateButtonA), FALSE);
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->readAndCreateButtonB), FALSE);
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
   const char *value = gtk_entry_get_text(GTK_ENTRY(widget));
   char byte[11];

   Closure->fillUnreadable = strtol(value, NULL, 0);

   if(Closure->fillUnreadable < 0)
   {  Closure->fillUnreadable = 1;
      if(pc->byteEntryA) gtk_entry_set_text(GTK_ENTRY(pc->byteEntryA), "1");
      if(pc->byteEntryB) gtk_entry_set_text(GTK_ENTRY(pc->byteEntryB), "1");
      return;
   }

   if(Closure->fillUnreadable > 255)
   {  Closure->fillUnreadable = 255;
   }

   g_snprintf(byte, 10, "0x%02x", Closure->fillUnreadable);

   if(pc->byteEntryA) gtk_entry_set_text(GTK_ENTRY(pc->byteEntryA), byte);
   if(pc->byteEntryB) gtk_entry_set_text(GTK_ENTRY(pc->byteEntryB), byte);
}


static void bytefill_check_cb(GtkWidget *widget, gpointer data)
{  prefs_context *pc = (prefs_context*)data;
   int state  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

   if(pc->byteCheckA)
     gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->byteCheckA), state);
   if(pc->byteCheckB)
     gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pc->byteCheckB), state);

   if(state)
   {  char byte[11];

      if(pc->byteEntryA) gtk_widget_set_sensitive(pc->byteEntryA, TRUE);
      if(pc->byteEntryB) gtk_widget_set_sensitive(pc->byteEntryB, TRUE);

      if(Closure->fillUnreadable < 0 || Closure->fillUnreadable > 255)
	 Closure->fillUnreadable = 0xb0;

      g_snprintf(byte, 10, "0x%02x", Closure->fillUnreadable);

      if(pc->byteEntryA) gtk_entry_set_text(GTK_ENTRY(pc->byteEntryA), byte);
      if(pc->byteEntryB) gtk_entry_set_text(GTK_ENTRY(pc->byteEntryB), byte);
   }
   else
   {  Closure->fillUnreadable = -1;
      if(pc->byteEntryA) gtk_widget_set_sensitive(pc->byteEntryA, FALSE);
      if(pc->byteEntryB) gtk_widget_set_sensitive(pc->byteEntryB, FALSE);
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
   {  GtkWidget *other;

      strncpy(Closure->methodName, method->name, 4);
      gtk_notebook_set_current_page(GTK_NOTEBOOK(pc->methodNotebook), n);

      if(pc->methodChooserA == widget)
	   other = pc->methodChooserB;
      else other = pc->methodChooserA;

#if GTK_MINOR_VERSION >= 4
      gtk_combo_box_set_active(GTK_COMBO_BOX(other), n);
#else
      gtk_option_menu_set_history(GTK_OPTION_MENU(other), n);
#endif

   }
}

/*
 * Setting the notebook page does not work at creation time.
 */

static gboolean notebook_idle_func(gpointer data)
{  prefs_context *pc = (prefs_context*)data;
   int n;

#if GTK_MINOR_VERSION >= 4
   n = gtk_combo_box_get_active(GTK_COMBO_BOX(pc->methodChooserA));
#else
   n = gtk_option_menu_get_history(GTK_OPTION_MENU(pc->methodChooserA));
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
      GtkWidget *lab;
#if GTK_MINOR_VERSION < 4
      GtkWidget *option_menu_strip;
#endif
      LabelWithOnlineHelp *lwoh,*lwoh_clone;
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

      lwoh = CreateLabelWithOnlineHelp(_("Image size determination"), _("Get Image size from: "));
      g_ptr_array_add(pc->helpPages, lwoh);

      for(i=0; i<2; i++)
      {  GtkWidget *hbox = gtk_hbox_new(FALSE, 4);
	 GtkWidget *radio1, *radio2, *radio3;

	 gtk_box_pack_start(GTK_BOX(hbox), i ? lwoh->normalLabel : lwoh->linkBox, FALSE, FALSE, 0);

	 radio1 = gtk_radio_button_new(NULL);
	 g_signal_connect(G_OBJECT(radio1), "toggled", G_CALLBACK(imgsize_cb), (gpointer)0);
	 gtk_box_pack_start(GTK_BOX(hbox), radio1, FALSE, FALSE, 0);
	 lab = gtk_label_new(_utf("Drive"));
	 gtk_container_add(GTK_CONTAINER(radio1), lab);

	 radio2 = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(radio1));
	 g_signal_connect(G_OBJECT(radio2), "toggled", G_CALLBACK(imgsize_cb), (gpointer)1);
	 gtk_box_pack_start(GTK_BOX(hbox), radio2, FALSE, FALSE, 0);
	 lab = gtk_label_new(_utf("ISO/UDF"));
	 gtk_container_add(GTK_CONTAINER(radio2), lab);

	 radio3 = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(radio2));
	 g_signal_connect(G_OBJECT(radio3), "toggled", G_CALLBACK(imgsize_cb), (gpointer)2);
	 gtk_box_pack_start(GTK_BOX(hbox), radio3, FALSE, FALSE, 0);
	 lab = gtk_label_new(_utf("ECC/RS02"));
	 gtk_container_add(GTK_CONTAINER(radio3), lab);

	 switch(Closure->querySize)
	 {  case 0: gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio1), TRUE); break;
            case 1: gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio2), TRUE); break;
            case 2: gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio3), TRUE); break;
	 }

	 if(!i)
	 {  pc->radioDriveA = radio1;
	    pc->radioISOA   = radio2;
	    pc->radioECCA   = radio3;
	    gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
	 }
	 else  
	 {  pc->radioDriveB = radio1;
	    pc->radioISOB   = radio2;
	    pc->radioECCB   = radio3;
	    AddHelpWidget(lwoh, hbox);
	 }
      }

      AddHelpParagraph(lwoh, 
		       _("<b>Image size determination</b>\n\n"
			 "Use <i>ECC/RS02</i> for reading images augmented with error correction data; "
			 "else pick <i>ISO/UDF</i>.\n\n"

			 "<b>ECC/RS02:</b> The Image size is determined from the error correction data. "
			 "Reading RS02 augmented images requires this option; otherwise the images "
			 "may be incomplete. However if the medium does not contain error correction "
			 "data, the start of the reading operation may be delayed significantly.\n\n"

			 "<b>ISO/UDF:</b> The image size is determined from the ISO/UDF file system.\n"
			 "Caution: This is only suitable for working with error correction files. "
			 "Images containing RS02 error correction information may be truncated.\n\n"

			 "<b>Drive:</b> The image size reported by the drive will be used. "
			 "As this information is typically wrong for DVD-RW/+RW/-RAM media this option "
			 "is only present for backwards compatibility with older dvdisaster versions."));

      /* file extension */

      frame = gtk_frame_new(_utf("Local files (on hard disc)"));
      gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

      vbox2 = gtk_vbox_new(FALSE, 15);
      gtk_container_set_border_width(GTK_CONTAINER(vbox2), 10);
      gtk_container_add(GTK_CONTAINER(frame), vbox2);

      lwoh = CreateLabelWithOnlineHelp(_("Automatic file suffixes"), _("Automatically add .iso and .ecc file suffixes"));
      g_ptr_array_add(pc->helpPages, lwoh);

      for(i=0; i<2; i++)
      {  GtkWidget *hbox = gtk_hbox_new(FALSE, 0);
	 GtkWidget *button = gtk_check_button_new();

	 gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
	 gtk_box_pack_start(GTK_BOX(hbox), i ? lwoh->normalLabel : lwoh->linkBox, FALSE, FALSE, 0);

	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), Closure->autoSuffix);
	 g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(toggle_cb), GINT_TO_POINTER(TOGGLE_SUFFIX));

	 if(!i)
	 {  pc->suffixA = button;
	    gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
	 }
	 else
	 {  pc->suffixB = button;
	    AddHelpWidget(lwoh, hbox);
	 }
      }

      AddHelpParagraph(lwoh, 
		       _("<b>Automatically add file suffixes</b>\n\n"
			 "When this switch is set, files will be automatically appended with \".iso\" "
			 "or \".ecc\" suffixes if no other file name extension is already present."));

      /* 2GB button */

      lwoh = CreateLabelWithOnlineHelp(_("File splitting"), _("Split files into segments &lt;= 2GB"));
      g_ptr_array_add(pc->helpPages, lwoh);

      for(i=0; i<2; i++)
      {  GtkWidget *hbox = gtk_hbox_new(FALSE, 0);
	 GtkWidget *button = gtk_check_button_new();

	 gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
	 gtk_box_pack_start(GTK_BOX(hbox), i ? lwoh->normalLabel : lwoh->linkBox, FALSE, FALSE, 0);
	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), Closure->splitFiles);
	 g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(toggle_cb), GINT_TO_POINTER(TOGGLE_2GB));

	 if(!i)
	 {  pc->splitA = button;
	    gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
	 }
	 else
	 {  pc->splitB = button;
	    AddHelpWidget(lwoh, hbox);
	 }
      }

      AddHelpParagraph(lwoh, 
		       _("<b>File splitting</b>\n\n"
			 "Allows working with file systems which are limited to 2GB per file, e.g. "
			 "FAT from Windows. Created files are spread over upto 100 segments "
			 "called \"medium00.iso\", \"medium01.iso\" etc. at the cost of a small "
			 "performance hit."));


      /*** Automatic file creation and deletion */

      frame = gtk_frame_new(_utf("Automatic file creation and deletion"));
      gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

      vbox2 = gtk_vbox_new(FALSE, 15);
      gtk_container_set_border_width(GTK_CONTAINER(vbox2), 10);
      gtk_container_add(GTK_CONTAINER(frame), vbox2);

      /* automatic creation */

      lwoh = CreateLabelWithOnlineHelp(_("Automatic .ecc file creation"), _("Create error correction file after reading image"));
      g_ptr_array_add(pc->helpPages, lwoh);

      for(i=0; i<2; i++)
      {  GtkWidget *hbox = gtk_hbox_new(FALSE, 0);
	 GtkWidget *button = gtk_check_button_new();

	 gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
      	 gtk_box_pack_start(GTK_BOX(hbox), i ? lwoh->normalLabel : lwoh->linkBox, FALSE, FALSE, 0);
	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), Closure->readAndCreate);
	 g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(toggle_cb), GINT_TO_POINTER(TOGGLE_READ_CREATE));

	 if(!i)
	 {  pc->readAndCreateButtonA = button;
	    gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
	 }
	 else
	 {  pc->readAndCreateButtonB = button;
	    AddHelpWidget(lwoh, hbox);
	 }
      }

      AddHelpParagraph(lwoh, 
		       _("<b>Automatic error correction file creation</b>\n\n"
			 "Automatically creates an error correction file after reading an image. "
			 "Together with the \"Remove image\" option this will speed up error correction "
			 "file generation for a series of different media."));

      /* automatic deletion */

      lwoh = CreateLabelWithOnlineHelp(_("Automatic image file removal"), _("Remove image after error correction file creation"));
      g_ptr_array_add(pc->helpPages, lwoh);

      for(i=0; i<2; i++)
      {  GtkWidget *hbox = gtk_hbox_new(FALSE, 0);
	 GtkWidget *button = gtk_check_button_new();

	 gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
      	 gtk_box_pack_start(GTK_BOX(hbox), i ? lwoh->normalLabel : lwoh->linkBox, FALSE, FALSE, 0);
	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), Closure->unlinkImage);
	 g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(toggle_cb), GINT_TO_POINTER(TOGGLE_UNLINK));

	 if(!i)
	 {  pc->unlinkImageButtonA = button;
	    gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
	 }
	 else
	 {  pc->unlinkImageButtonB = button;
	    AddHelpWidget(lwoh, hbox);
	 }
      }

      AddHelpParagraph(lwoh, 
		       _("<b>Automatic image file removal</b>\n\n"
			 "If this switch is set the image file will be deleted following the successful "
			 "generation of the respective error correction file."));

      /*** Read & Scan page */

      vbox = create_page(notebook, _utf("Read & Scan"));

      /** Reading preferences */
      
      frame = gtk_frame_new(_utf("Reading preferences"));
      gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

      vbox2 = gtk_vbox_new(FALSE, 20);
      gtk_container_set_border_width(GTK_CONTAINER(vbox2), 10);
      gtk_container_add(GTK_CONTAINER(frame), vbox2);

      /* Reading strategy */

      lwoh = CreateLabelWithOnlineHelp(_("Reading strategy"), _("Reading strategy: "));
      g_ptr_array_add(pc->helpPages, lwoh);

      for(i=0; i<2; i++)
      {  GtkWidget *hbox = gtk_hbox_new(FALSE, 4);
	 GtkWidget *lab, *radio1, *radio2;

	 gtk_box_pack_start(GTK_BOX(hbox), i ? lwoh->normalLabel : lwoh->linkBox, FALSE, FALSE, 0);

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
	 else   AddHelpWidget(lwoh, hbox);
      }

      AddHelpParagraph(lwoh, 
		       _("<b>Reading strategy</b>\n"
		         "The linear strategy is optimized for undamaged media "
			 "while the adaptive strategy is better suited "
			 "for media already containing read errors."));

      /* Reading range */

      lwoh = CreateLabelWithOnlineHelp(_("Reading range"), _("Read/Scan from sector"));
      g_ptr_array_add(pc->helpPages, lwoh);

      for(i=0; i<2; i++)
      {  GtkWidget *hbox = gtk_hbox_new(FALSE, 4);
	 GtkWidget *toggle,*spin1, *spin2;

	 //	 toggle = gtk_check_button_new_with_label(_utf("Read/Scan from sector"));
	 toggle = gtk_check_button_new();
	 if(!i) pc->rangeToggleA = toggle;
	 else   pc->rangeToggleB = toggle;
	 g_signal_connect(G_OBJECT(toggle), "toggled", G_CALLBACK(toggle_cb), GINT_TO_POINTER(TOGGLE_RANGE));
	 gtk_box_pack_start(GTK_BOX(hbox), toggle, FALSE, FALSE, 0);

	 gtk_box_pack_start(GTK_BOX(hbox), i ? lwoh->normalLabel : lwoh->linkBox, 
			    FALSE, FALSE, 0);

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
	 else   AddHelpWidget(lwoh, hbox);

	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle), FALSE);
      }

      AddHelpParagraph(lwoh, 
		       _("<b>Reading range</b>\n\n"
			 "Reading can be limited to a part of the medium (in sectors holding 2KB each). "
			 "The values include the borders: 0-100 will read 101 sectors.\n\n"

			 "<b>Note:</b> Limiting the reading range is not recommended for <i>adaptive reading</i> since it might "
			 "prevent sectors from being read which are required for a succesful error correction.\n\n"
			 "These settings are only effective for the current session and will not be saved."));

      /** Reading preferences */
      
      frame = gtk_frame_new(_utf("Read error treatment"));
      gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

      vbox2 = gtk_vbox_new(FALSE, 20);
      gtk_container_set_border_width(GTK_CONTAINER(vbox2), 10);
      gtk_container_add(GTK_CONTAINER(frame), vbox2);

      /* Raw verify */

      lwoh = CreateLabelWithOnlineHelp(_("Raw reading"), _("Read and analyze raw sectors"));
      g_ptr_array_add(pc->helpPages, lwoh);

      for(i=0; i<2; i++)
      {  GtkWidget *hbox = gtk_hbox_new(FALSE, 0);

	 button = gtk_check_button_new();
	 gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
       	 gtk_box_pack_start(GTK_BOX(hbox), i ? lwoh->normalLabel : lwoh->linkBox, FALSE, FALSE, 0);

 	 if(!i) pc->rawButtonA = button;
	 else   pc->rawButtonB = button;

         gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), Closure->readRaw);
         g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(toggle_cb), GINT_TO_POINTER(TOGGLE_RAW));

	 if(!i) gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
	 else   AddHelpWidget(lwoh, hbox);
      }

      AddHelpParagraph(lwoh, 
		       _("<b>Raw reading</b>\n\n"
			 "Some CD/DVD drives may deliver unreliable results when their "
			 "internal error correction approaches its maximum capacity.\n\n"
			 "Activating this option makes dvdisaster read sectors in raw mode. "
			 "The L-EC P/Q vectors, EDC checksum and MSF address contained in the "
			 "raw data are checked to make sure that the sector was correctly read."
			 ));

      /* Minimum reading attempts */

      lwoh = CreateLabelWithOnlineHelp(_("Minimum number of reading attempts"), "ignore");
      g_ptr_array_add(pc->helpPages, lwoh);

      pc->minAttemptsScaleLwoh = lwoh;
      pc->minAttemptsScaleInfoA = g_malloc0(sizeof(non_linear_info));
      pc->minAttemptsScaleInfoB = g_malloc0(sizeof(non_linear_info));
      pc->minAttemptsScaleInfoA->format = g_strdup(_utf("Min. %d reading attempts per sector"));
      pc->minAttemptsScaleInfoB->format = g_strdup(_utf("Min. %d reading attempts per sector"));

      pc->minAttemptsScaleInfoA->lwoh = lwoh;

      for(i=0; i<2; i++)
	{  GtkWidget *scale,*scale_box;

	 scale = non_linear_scale(&scale_box, 
				  i ? pc->minAttemptsScaleInfoB : pc->minAttemptsScaleInfoA,
				  i ? lwoh->normalLabel : lwoh->linkLabel, 
				  pc, SLIDER_MIN_READ_ATTEMPTS, min_attempts_values, MIN_ATTEMPTS_VALUE_LENGTH,
				  Closure->minReadAttempts);

	 if(!i) pc->minAttemptsScaleA = scale;
	 else   pc->minAttemptsScaleB = scale;

	 if(!i) gtk_box_pack_start(GTK_BOX(vbox2), scale_box, FALSE, FALSE, 0);
	 else   AddHelpWidget(lwoh, scale_box);
      }

      AddHelpParagraph(lwoh, 
		       _("<b>Minimum number of reading attempts</b>\n\n"
			 "If an unreadable sector is encountered, "
			 "dvdisaster tries to re-read it the given number of times.\n\n" 
			 "Increasing the number of reading attempts may improve data recovery "
			 "on marginal media, but will also increase processing time and "
			 "mechanical wear on the drive."));

      /* Maximum reading attempts */

      lwoh = CreateLabelWithOnlineHelp(_("Maximum number of reading attempts"), "ignore");
      g_ptr_array_add(pc->helpPages, lwoh);

      pc->maxAttemptsScaleLwoh = lwoh;
      pc->maxAttemptsScaleInfoA = g_malloc0(sizeof(non_linear_info));
      pc->maxAttemptsScaleInfoB = g_malloc0(sizeof(non_linear_info));
      pc->maxAttemptsScaleInfoA->format = g_strdup(_utf("Max. %d reading attempts per sector"));
      pc->maxAttemptsScaleInfoB->format = g_strdup(_utf("Max. %d reading attempts per sector"));

      pc->maxAttemptsScaleInfoA->lwoh = lwoh;

      for(i=0; i<2; i++)
	{  GtkWidget *scale,*scale_box;

	 scale = non_linear_scale(&scale_box, 
				  i ? pc->maxAttemptsScaleInfoB : pc->maxAttemptsScaleInfoA,
				  i ? lwoh->normalLabel : lwoh->linkLabel, 
				  pc, SLIDER_MAX_READ_ATTEMPTS, max_attempts_values, MAX_ATTEMPTS_VALUE_LENGTH,
				  Closure->maxReadAttempts);

	 if(!i) pc->maxAttemptsScaleA = scale;
	 else   pc->maxAttemptsScaleB = scale;

	 if(!i) gtk_box_pack_start(GTK_BOX(vbox2), scale_box, FALSE, FALSE, 0);
	 else   AddHelpWidget(lwoh, scale_box);
      }

      AddHelpParagraph(lwoh, 
		       _("<b>Maximum number of reading attempts</b>\n\n"
			 "When the minimum number of reading attempts is reached "
			 "without success, dvdisaster might choose to perform additional "
			 "reading attempts upto this number.\n\n"

			 "The decision to do more attempts depends on the quality of "
			 "data gathered so far, which in turn is influenced by the "
			 "capabilities of your CD/DVD drive and the operating system. "
			 "So depending on your configuration, you may or "
			 "may not see dvdisaster using the maximum value."
			 ));

      /* Jump selector */

      lwoh = CreateLabelWithOnlineHelp(_("Treatment of unreadable areas"), "ignore");
      g_ptr_array_add(pc->helpPages, lwoh);

      pc->jumpScaleLwoh = lwoh;
      pc->formatLinear = g_strdup(_utf("Skip %d sectors after read error"));
      pc->formatAdaptive  = g_strdup(_utf("Stop reading when unreadable intervals &lt; %d"));

      pc->jumpScaleInfoA = g_malloc0(sizeof(non_linear_info));
      pc->jumpScaleInfoB = g_malloc0(sizeof(non_linear_info));
      pc->jumpScaleInfoA->format = Closure->adaptiveRead ? pc->formatAdaptive : pc->formatLinear;
      pc->jumpScaleInfoB->format = Closure->adaptiveRead ? pc->formatAdaptive : pc->formatLinear;

      pc->jumpScaleInfoA->lwoh = lwoh;

      for(i=0; i<2; i++)
      {  GtkWidget *scale, *scale_box;
 
	 scale = non_linear_scale(&scale_box, 
				  i ? pc->jumpScaleInfoB : pc->jumpScaleInfoA, 
				  i ? lwoh->normalLabel : lwoh->linkLabel, 
				  pc, SLIDER_JUMP, jump_values, JUMP_VALUE_LENGTH,
				  Closure->sectorSkip);

	 if(!i) pc->jumpScaleA = scale;
	 else   pc->jumpScaleB = scale;

	 if(!i) gtk_box_pack_start(GTK_BOX(vbox2), scale_box, FALSE, FALSE, 0);
	 else   AddHelpWidget(lwoh, scale_box);
      }

      AddHelpParagraph(lwoh, 
		       _("<b>Treatment of unreadable areas</b>\n\n"
			 "Defective media usually contain numerous read errors in a contigous region. "
			 "Skipping sectors after a read error reduces the processing time and the "
			 "mechanical wear on the drive, but leaves larger gaps in the image file.\n\n"
			 "Effects on the <b>linear reading strategy</b>:"));

      AddHelpListItem(lwoh, 
		       _("Skipping a large number of sectors (e.g. 1024) gives a quick overview of "
			 "damaged areas, but will usually not collect enough data for repairing the image."));
      
      AddHelpListItem(lwoh, 
		      _("Smaller values like 16, 32 or 64 are a good trade-off: The processing time will be"
			 "considerably shortened, but still enough data for repairing the image is collected.\n"));

      AddHelpParagraph(lwoh, 
		       _("The <b>adaptive reading strategy</b> uses this setting only if no error correction data "
			 "is available. In that case the reading process will stop when no unread areas "
			 "larger than the selected size remain. Values smaller than 128 <i>are not recommended</i> "
			 "as they cause the drive to carry out lots of laser head repositioning during the "
			 "final phase of the reading process. If adaptive reading with a setting of 128 is not "
			 "sufficient, try reading the remaining sectors with an additional linear reading pass.\n\n"

			 "On DVD media read errors do usually extend over at least 16 sectors for technical "
			 "reasons. Therefore selecting a value less than 16 is not recommended for DVD."
			 ));

      /** Image properties */

      frame = gtk_frame_new(_utf("Image properties"));
      gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

      vbox2 = gtk_vbox_new(FALSE, 15);
      gtk_container_set_border_width(GTK_CONTAINER(vbox2), 10);
      gtk_container_add(GTK_CONTAINER(frame), vbox2);

      /* DAO button */

      lwoh = CreateLabelWithOnlineHelp(_("DAO mode"), _("Assume image to be written in DAO mode (don't truncate)"));
      g_ptr_array_add(pc->helpPages, lwoh);

      for(i=0; i<2; i++)
      {  GtkWidget *hbox = gtk_hbox_new(FALSE, 0);
	 GtkWidget *button = gtk_check_button_new();

	 gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
	 gtk_box_pack_start(GTK_BOX(hbox), i ? lwoh->normalLabel : lwoh->linkBox, FALSE, FALSE, 0);

	 if(!i) pc->daoButtonA = button;
	 else   pc->daoButtonB = button;

	 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), Closure->noTruncate);
	 g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(toggle_cb), GINT_TO_POINTER(TOGGLE_DAO));
	 if(!i) gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
	 else   AddHelpWidget(lwoh, hbox);
      }

      AddHelpParagraph(lwoh, 
		       _("<b>Assume DAO mode</b>\n\n"
			 "Media written in \"TAO\" (\"track at once\") mode may contain two sectors "
			 "with pseudo read errors at the end. By default these two sectors are ignored.\n\n"

			 "If you are extremely unlucky to have a \"DAO\" (\"disc at once\") medium "
			 "with exactly one or two real read errors at the end, dvdisaster may treat "
			 "this as a \"TAO\" disc and truncate the image by two sectors. In that case "
			 "activate this option to have the last two read errors handled correctly.\n\n"

			 "<b>Tip:</b> To avoid these problems, consider using the \"DAO / Disc at once\" "
			 "(sometimes also called \"SAO / Session at once\") mode for writing single "
			 "session media."));

      /* byte filling */

      lwoh = CreateLabelWithOnlineHelp(_("Filling of unreadable sectors"), 
				       _("Fill unreadable sectors with byte:"));
      g_ptr_array_add(pc->helpPages, lwoh);

      for(i=0; i<2; i++)
      {  GtkWidget *hbox = gtk_hbox_new(FALSE, 4);
	 GtkWidget *check, *entry;

	 check = gtk_check_button_new();
	 g_signal_connect(check, "toggled", G_CALLBACK(bytefill_check_cb), pc);
	 gtk_box_pack_start(GTK_BOX(hbox), check, FALSE, FALSE, 0);
	 gtk_box_pack_start(GTK_BOX(hbox), i ? lwoh->normalLabel : lwoh->linkBox, FALSE, FALSE, 0);

	 entry = gtk_entry_new();
	 g_signal_connect(entry, "activate", G_CALLBACK(bytefill_cb), pc);
	 gtk_entry_set_width_chars(GTK_ENTRY(entry), 5);
	 gtk_box_pack_start(GTK_BOX(hbox), entry, FALSE, FALSE, 0);

	 if(!i)
	 {  pc->byteCheckA = check;
	    pc->byteEntryA = entry;
	    gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
	 }
	 else
	 {  pc->byteCheckB = check;
	    pc->byteEntryB = entry;
	    AddHelpWidget(lwoh, hbox);
	 }

	 if(Closure->fillUnreadable >= 0)
	 {  char value[11];
	
	    g_snprintf(value, 10, "0x%02x", Closure->fillUnreadable);
	    gtk_entry_set_text(GTK_ENTRY(entry), value);
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check), TRUE);
	 }
	 else gtk_widget_set_sensitive(entry, FALSE);
      }

      AddHelpParagraph(lwoh, 
		       _("<b>Filling of unreadable sectors</b>\n\n"

			 "dvdisaster marks unreadable sectors with a special filling pattern which "
			 "is very unlikely to occur in undamaged media.\n"
			 "In other data recovery software it is common to fill unreadable sectors "
			 "with a certain byte value. To allow interoperability with such programs, "
			 "you can specify the byte value they are using:\n"));

      AddHelpListItem(lwoh,
		      _("0xb0 (176 decimal): for compatibility with h2cdimage published by \"c't\", "
			"a German periodical.\n"));

      AddHelpParagraph(lwoh,
		       _("<b>Note:</b> Using zero filling (0x00, decimal 0) is highly discouraged. "
			 "Most media contain regular zero filled sectors which can not be told apart "
			 "from unreadable sectors if zero filling is used."));

      /** Drive initialisation */

      frame = gtk_frame_new(_utf("Drive initialisation"));
      gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

      lwoh = CreateLabelWithOnlineHelp(_("Drive initialisation"), 
				       _("Wait"));
      g_ptr_array_add(pc->helpPages, lwoh);

      lwoh_clone = CloneLabelWithOnlineHelp(lwoh, _("seconds for drive to spin up"));
      g_ptr_array_add(pc->helpPages, lwoh_clone);


      for(i=0; i<2; i++)
      {  GtkWidget *hbox = gtk_hbox_new(FALSE, 4);
	 GtkWidget *spin;

	 gtk_box_pack_start(GTK_BOX(hbox), i ? lwoh->normalLabel : lwoh->linkBox, FALSE, FALSE, 0);

	 spin = gtk_spin_button_new_with_range(0, 30, 1);
	 gtk_entry_set_width_chars(GTK_ENTRY(spin), 3);
	 gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), Closure->spinupDelay);
	 g_signal_connect(spin, "value-changed", G_CALLBACK(spin_cb), (gpointer)SPIN_DELAY);
	 gtk_box_pack_start(GTK_BOX(hbox), spin, FALSE, FALSE, 0);

	 gtk_box_pack_start(GTK_BOX(hbox), i ? lwoh_clone->normalLabel : lwoh_clone->linkBox, FALSE, FALSE, 0);
	 gtk_container_set_border_width(GTK_CONTAINER(hbox), 10);

	 if(!i)
	 {  pc->spinUpA = spin;
	    gtk_container_add(GTK_CONTAINER(frame), hbox);
	 }
	 else
	 {  pc->spinUpB = spin;
	    AddHelpWidget(lwoh, hbox);
	 }
      }

      AddHelpParagraph(lwoh, 
		       _("<b>Drive initialisation</b>\n\n"
			 "Waits the specified amount of seconds for letting the drive spin up. "
			 "This avoids speed jumps at the beginning of the reading curve."));

      /*** "Error correction" page */

      /* Method chooser menu */

      vbox = create_page(notebook, _utf("Error correction"));

      lwoh = CreateLabelWithOnlineHelp(_("Error correction method"), 
				       _("Storage method:"));
      g_ptr_array_add(pc->helpPages, lwoh);

      for(i=0; i<2; i++)
      {  GtkWidget *hbox = gtk_hbox_new(FALSE, 4);
	 GtkWidget *chooser;
	 int j;

	 gtk_box_pack_start(GTK_BOX(hbox), i ? lwoh->normalLabel : lwoh->linkBox, FALSE, FALSE, 0);

#if GTK_MINOR_VERSION >= 4

	 chooser = gtk_combo_box_new_text();

       	 g_signal_connect(G_OBJECT(chooser), "changed", G_CALLBACK(method_select_cb), pc);

	 for(j=0; j<Closure->methodList->len; j++)
	 {  Method *method = g_ptr_array_index(Closure->methodList, j);

	    gtk_combo_box_append_text(GTK_COMBO_BOX(chooser), method->menuEntry); 

	    if(!strncmp(Closure->methodName, method->name, 4))
	      method_idx = j;
	 }

	 gtk_combo_box_set_active(GTK_COMBO_BOX(chooser), method_idx);
	 gtk_box_pack_start(GTK_BOX(hbox), chooser, FALSE, FALSE, 0);
#else
	 chooser = gtk_option_menu_new();

	 g_signal_connect(G_OBJECT(chooser), "changed", G_CALLBACK(method_select_cb), pc);
	 option_menu_strip = gtk_menu_new(); 

	 for(j=0; j<Closure->methodList->len; j++)
	 {  Method *method = g_ptr_array_index(Closure->methodList, j);
	    GtkWidget *item;

	    item = gtk_menu_item_new_with_label(method->menuEntry);
	    gtk_menu_shell_append(GTK_MENU_SHELL(option_menu_strip), item);

	    if(!strncmp(Closure->methodName, method->name, 4))
	      method_idx = j;
	 }

	 gtk_option_menu_set_menu(GTK_OPTION_MENU(chooser), option_menu_strip);
	 gtk_option_menu_set_history(GTK_OPTION_MENU(chooser), method_idx);
	 gtk_box_pack_start(GTK_BOX(hbox), chooser, FALSE, FALSE, 0);
#endif
	 
	 if(!i)
	 {  pc->methodChooserA = chooser;
	    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
	 }
	 else
	 {  pc->methodChooserB = chooser;
	    AddHelpWidget(lwoh, hbox);
	 }
      }

      AddHelpParagraph(lwoh, _("<b>Error correction method</b>\n\n"
			       "dvdisaster creates error correction data which is used to recover "
			       "unreadable sectors if the disc becomes damaged later on.\n"
			       "Currently two Reed-Solomon based error correction methods are available. "
			       "provide different ways for storing the error correction information:\n"));

      AddHelpListItem(lwoh, _("RS01 creates error correction files which are stored separately "
			      "from the image they belong to. Since data protection at the file level "
			      "is difficult, error correction files must be stored on media which are "
			      "protected against data loss by dvdisaster, too.\n"));

      AddHelpListItem(lwoh, _("RS02 places the error correction data directly on the medium which is "
			      "to be protected. This requires creating an image on hard disk using "
			      "a CD/DVD writing software, as the image must be augmented with error "
			      "correction data prior to writing it to the medium.\n"
			      "Therefore the data to be protected and the error correction information "
			      "are located at the same medium. Damaged sectors in the error correction "
			      "information reduce the data recovery capacity, but do not make recovery "
			      "impossible - a second medium for keeping or protecting the error correction "
			      "information is not required."));

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
