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
 *** The log viewer
 ***/

static void log_destroy_cb(GtkWidget *widget, gpointer data)
{
   Closure->logWidget = NULL;
   Closure->logBuffer = NULL;
}

static gboolean log_idle_func(gpointer data)
{  GtkAdjustment *a;
   GtkTextIter end;

   gtk_text_buffer_set_text(Closure->logBuffer, Closure->logString->str, Closure->logString->len);
   gtk_text_buffer_get_end_iter(Closure->logBuffer, &end);
   gtk_text_buffer_place_cursor(Closure->logBuffer, &end);

   a = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(Closure->logScroll));
   gtk_adjustment_set_value(a, a->upper);
   gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(Closure->logScroll), a);

   return FALSE;
}


void UpdateLog()
{  static int unique_addr;

   if(Closure->logWidget)
   {  g_idle_remove_by_data(&unique_addr);
      g_idle_add(log_idle_func, &unique_addr);
   }
}

void ShowLog()
{ GtkWidget *w;

  if(Closure->logWidget) 
  {  gtk_widget_show(Closure->logWidget);
     return;
  }

  w = ShowTextfile(_("windowtitle|Log data"),
		   _("<big>Log data</big>\n"
		     "<i>Protocol of the current or previous action</i>"),
		   "*LOG*", &Closure->logScroll, &Closure->logBuffer);

  g_signal_connect(G_OBJECT(w), "destroy", G_CALLBACK(log_destroy_cb), NULL);

  Closure->logWidget = w;
}


/***
 *** Help dialogs 
 ***/

void ShowGPL()
{
  ShowTextfile(_("windowtitle|GNU General Public License"), 
	       _("<big>GNU General Public License</big>\n"
		 "<i>The license terms of dvdisaster.</i>"),
	       "COPYING", NULL, NULL);
}

/*
 * Dialog for displaying text files
 */

char *find_file(char *file, size_t *size, char *lang)
{  char *path;
   char lang_suffix[3];
   struct stat mystat;

   lang_suffix[0] = lang_suffix[2] = 0;

   if(lang)
   {  
      lang_suffix[0] = lang[0];
      lang_suffix[1] = lang[1];
   }

   /* Try file in bin dir */

   if(Closure->binDir) 
   {  if(lang) 
           path = g_strdup_printf("%s/%s.%s",Closure->binDir, file, lang_suffix);
      else path = g_strdup_printf("%s/%s",Closure->binDir, file);

      if(!stat(path, &mystat))
      {	 *size = mystat.st_size;
	 return path;
      }

      g_free(path);
   }

   /* Try file in doc dir */

   if(Closure->docDir)
   {  if(lang)
           path = g_strdup_printf("%s/%s.%s",Closure->docDir, file, lang_suffix);
      else path = g_strdup_printf("%s/%s",Closure->docDir, file);

      if(!stat(path, &mystat))
      {	 *size = mystat.st_size;
	 return path;
      }

      g_free(path);
   }

   return NULL;
}

GtkWidget* ShowTextfile(char *title, char *explanation, char *file, 
			GtkScrolledWindow **scroll_out, GtkTextBuffer **buffer_out)
{  GtkWidget *dialog, *scroll_win, *vbox, *lab, *sep, *view;
   GtkTextBuffer *buffer; 
   GtkTextIter start;
   char *path;
   char *utf,*buf;
   size_t size = 0;

   /*** Read the text file */

   if(*file != '*')
   {  
       if(    !(path = find_file(file, &size, NULL))
	   && !(path = find_file(file, &size, (char*)g_getenv("LANG")))
	   && !(path = find_file(file, &size, "en"))
         )
      {  char *trans = _utf("File\n%s\nnot present");
      
	 buf = g_strdup_printf(trans, file);
	 size = strlen(buf);
      }
      else
      {  FILE *file = fopen(path, "rb");

	 buf = g_malloc(size);
	 fread(buf, size, 1, file);
	 fclose(file);
	 g_free(path);
      }
   }
   else 
   {  buf  = Closure->logString->str;
      size = Closure->logString->len;
   }

   /*** Create the dialog */

   utf = g_locale_to_utf8(title, -1, NULL, NULL, NULL);
   dialog = gtk_dialog_new_with_buttons(utf, Closure->window, GTK_DIALOG_DESTROY_WITH_PARENT,
				       GTK_STOCK_CLOSE, GTK_RESPONSE_ACCEPT, NULL);
   g_free(utf);
   gtk_window_set_default_size(GTK_WINDOW(dialog), 500, 600);
   g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

   vbox = gtk_vbox_new(FALSE, 0);
   gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), vbox, TRUE, TRUE, 0);
   gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);

   lab = gtk_label_new(NULL);
   utf = g_locale_to_utf8(explanation, -1, NULL, NULL, NULL);
   gtk_label_set_markup(GTK_LABEL(lab), utf);
   g_free(utf);
   gtk_misc_set_alignment(GTK_MISC(lab), 0.0, 0.0); 
   gtk_box_pack_start(GTK_BOX(vbox), lab, FALSE, FALSE, 0);

   sep = gtk_hseparator_new();
   gtk_box_pack_start(GTK_BOX(vbox), sep, FALSE, FALSE, 0);

   scroll_win = gtk_scrolled_window_new(NULL, NULL);
   gtk_box_pack_start(GTK_BOX(vbox), scroll_win, TRUE, TRUE, 5);
   if(scroll_out) *scroll_out = GTK_SCROLLED_WINDOW(scroll_win);

   view   = gtk_text_view_new();
   buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
   if(buffer_out) *buffer_out = buffer;

   gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
   gtk_text_buffer_set_text(buffer, buf, size);
   gtk_text_buffer_get_start_iter(buffer, &start);
   gtk_text_buffer_place_cursor(buffer, &start);

   gtk_container_add(GTK_CONTAINER(scroll_win), view);
   //   gtk_box_pack_start(GTK_BOX(vbox), view, FALSE, FALSE, 0);

   /* Show it */

#if 0
   { GtkRcStyle *rc_style = gtk_rc_style_new();
   rc_style->color_flags[GTK_STATE_NORMAL] |= GTK_RC_BG;
   rc_style->bg[GTK_STATE_NORMAL].red = 0xffff;
   rc_style->bg[GTK_STATE_NORMAL].green = 0xffff;
   rc_style->bg[GTK_STATE_NORMAL].blue = 0xffff;
   gtk_widget_modify_style(dialog, rc_style);
   }
#endif

   gtk_widget_show_all(dialog);

   if(*file != '*')
     g_free(buf);

   return dialog;
}

/*
 * About dialog
 */

static void show_modifying(void)
{  ShowTextfile(_("windowtitle|Modifying dvdisaster"), 
	       _("<big>Modifying dvdisaster</big>\n"
		 "<i>Your changes are not ours.</i>"),
	       "README.MODIFYING", NULL, NULL);
}

static gint about_cb(GtkWidget *widget, GdkEvent *event, gpointer data)
{  GtkWidget *lab = GTK_BIN(widget)->child;
   char *label = (char*)data;
   char text[strlen(label)+80];
   char *utf;
   static int inside;

   switch(event->type)
   {  case GDK_BUTTON_PRESS: 
        if(!inside) return FALSE; /* Bug in Gtk for Windows? */
        if(!strcmp(label,"GPL")) ShowGPL(); 
        else if(!strcmp(label,"MODIFYING")) show_modifying(); 
        else ShowHTML(g_strdup(label));
	break; 
      case GDK_ENTER_NOTIFY: 
	g_sprintf(text, "<span underline=\"single\" color=\"blue\">%s</span>", label);
	utf = g_locale_to_utf8(text, -1, NULL, NULL, NULL);
	gtk_label_set_markup(GTK_LABEL(lab), utf);
	g_free(utf);
	inside = TRUE;
	break;
      case GDK_LEAVE_NOTIFY: 
	g_sprintf(text, "<span color=\"blue\">%s</span>", label);
	utf = g_locale_to_utf8(text, -1, NULL, NULL, NULL);
	gtk_label_set_markup(GTK_LABEL(lab), utf); 
	g_free(utf);
	inside = FALSE;
	break;
      default: break;
   }

   return FALSE;
}

void AboutText(GtkWidget *parent, char *format, ...)
{  GtkWidget *lab;
   char *tmp, *utf_text;
   va_list argp;

   va_start(argp, format);

   lab = gtk_label_new(NULL);
   tmp = g_strdup_vprintf(format, argp);
   utf_text = g_locale_to_utf8(tmp, -1, NULL, NULL, NULL);
   gtk_label_set_markup(GTK_LABEL(lab), utf_text);
   gtk_misc_set_alignment(GTK_MISC(lab), 0.0, 0.0); 
   gtk_box_pack_start(GTK_BOX(parent), lab, FALSE, FALSE, 0);

   g_free(tmp);
   g_free(utf_text);

   va_end(argp);
}

void AboutLink(GtkWidget *parent, char *label, char *action)
{  GtkWidget *ebox,*lab;
   char text[strlen(label)+80];
   char *label_copy = strdup(label);
   char *utf;

   ebox = gtk_event_box_new();
   gtk_widget_set_events(ebox, GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_BUTTON_PRESS_MASK);
   g_signal_connect(G_OBJECT(ebox), "button_press_event", G_CALLBACK(about_cb), (gpointer)action);
   g_signal_connect(G_OBJECT(ebox), "enter_notify_event", G_CALLBACK(about_cb), (gpointer)label_copy);
   g_signal_connect(G_OBJECT(ebox), "leave_notify_event", G_CALLBACK(about_cb), (gpointer)label_copy);

   gtk_box_pack_start(GTK_BOX(parent), ebox, FALSE, FALSE, 0);

   lab  = gtk_label_new(NULL);
   g_sprintf(text, "<span color=\"blue\">%s</span>", label);
   utf = g_locale_to_utf8(text, -1, NULL, NULL, NULL);
   gtk_label_set_markup(GTK_LABEL(lab), utf);
   gtk_container_add(GTK_CONTAINER(ebox), lab);
   g_free(utf);
}

void AboutTextWithLink(GtkWidget *parent, char *text, char *action)
{  char *copy,*head,*end_of_line;
   char *link_start,*link_end; 
   char *utf;

   head = copy = g_strdup(text);

   while(*head)
   {  end_of_line = strchr(head, '\n');
      if(end_of_line && *end_of_line == '\n')
        *end_of_line = 0;

      link_start = strchr(head, '[');
      link_end = strchr(head, ']');

      if(link_start && link_end)
      {  GtkWidget *hbox = gtk_hbox_new(FALSE, 0);

         gtk_box_pack_start(GTK_BOX(parent), hbox, FALSE, FALSE, 0);
	 *link_start++ = *link_end++ = 0;

         if(*head) 
         {  GtkWidget *lab = gtk_label_new(NULL);

	    utf = g_locale_to_utf8(head, -1, NULL, NULL, NULL);
	    gtk_label_set_markup(GTK_LABEL(lab), utf);
	    gtk_box_pack_start(GTK_BOX(hbox), lab, FALSE, FALSE, 0);
	    g_free(utf);
	 }

         AboutLink(hbox, link_start, action);

         if(*link_end) 
         {  GtkWidget *lab = gtk_label_new(NULL);

	    utf = g_locale_to_utf8(link_end, -1, NULL, NULL, NULL);
	    gtk_label_set_markup(GTK_LABEL(lab), utf);
	    gtk_box_pack_start(GTK_BOX(hbox), lab, FALSE, FALSE, 0);
	    g_free(utf);
	 }
      }
      else AboutText(parent, head);

      if(end_of_line) head = end_of_line+1;
      else break;
   }

   g_free(copy);
}

void AboutTextWithLink2(GtkWidget *parent, char *text, char *actions)
{  char *copy,*head,*end_of_line;
   char *link_start,*link_end; 
   char *utf;
   char *action_separator;
   char *action,*action_copy;

   head = copy = g_strdup(text);
   action = action_copy = g_strdup(actions);

   while(*head)
   {  end_of_line = strchr(head, '\n');
      if(end_of_line && *end_of_line == '\n')
        *end_of_line = 0;

      link_start = strchr(head, '[');
      link_end = strchr(head, ']');

      if(!link_start || !link_end)
	   AboutText(parent, head);
      else
      {  GtkWidget *hbox = gtk_hbox_new(FALSE, 0);

         gtk_box_pack_start(GTK_BOX(parent), hbox, FALSE, FALSE, 0);

	 while(1)
	 {  *link_start++ = *link_end++ = 0;

	    if(*head) 
	    {  GtkWidget *lab = gtk_label_new(NULL);

	       utf = g_locale_to_utf8(head, -1, NULL, NULL, NULL);
	       gtk_label_set_markup(GTK_LABEL(lab), utf);
	       gtk_box_pack_start(GTK_BOX(hbox), lab, FALSE, FALSE, 0);
	       g_free(utf);
	    }

	    action_separator = strchr(action, ',');
	    if(action_separator) *action_separator = 0;
	    AboutLink(hbox, link_start, g_strdup(action));
	    if(action_separator) action = action_separator+1;

	    head = link_end;
	    if(head >= end_of_line) break;

	    link_start = strchr(head, '[');   /* more links in this line? */
	    link_end = strchr(head, ']');
	    if(link_start && link_end)
	      continue;
	    
	    if(*head)  /* no more links, but perhaps trailing text in this line */
	    {  GtkWidget *lab = gtk_label_new(NULL);

	       utf = g_locale_to_utf8(head, -1, NULL, NULL, NULL);
	       gtk_label_set_markup(GTK_LABEL(lab), utf);
	       gtk_box_pack_start(GTK_BOX(hbox), lab, FALSE, FALSE, 0);
	       g_free(utf);
	       break;
	    }
	 } 
      }

      if(end_of_line) head = end_of_line+1;
      else break;
   }

   g_free(copy);
   g_free(action_copy);
}

void AboutDialog()
{  GtkWidget *about, *vbox, *sep;
   char *text; 
#ifndef MODIFIED_SOURCE
   const char *lang;
#endif
   /* Create the dialog */

   about = gtk_dialog_new_with_buttons(_utf("windowtitle|About dvdisaster"), 
				       Closure->window, GTK_DIALOG_DESTROY_WITH_PARENT,
				       GTK_STOCK_CLOSE, GTK_RESPONSE_ACCEPT, NULL);

   g_signal_connect_swapped(about, "response", G_CALLBACK(gtk_widget_destroy), about);

   vbox = gtk_vbox_new(FALSE, 0);
   gtk_box_pack_start(GTK_BOX(GTK_DIALOG(about)->vbox), vbox, FALSE, FALSE, 0);
   gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);

   /* Insert the labels */

   text = g_strdup_printf("<span weight=\"bold\" size=\"xx-large\">dvdisaster</span><i> "
			  "Version %s</i>",
			  Closure->cookedVersion);
   AboutText(vbox, text);
   g_free(text);

#ifdef MODIFIED_SOURCE
   AboutTextWithLink(vbox, 
		     _("Modified version Copyright 2006 (please fill in - [directions])\n"
		       "Copyright 2004-2006 Carsten Gnoerlich"),
		     "MODIFYING");
#else
   AboutText(vbox, _("Copyright 2004-2006 Carsten Gnoerlich"));
#endif

   sep = gtk_hseparator_new();
   gtk_box_pack_start(GTK_BOX(vbox), sep, FALSE, FALSE, 10);


   AboutText(vbox, _("dvdisaster provides a margin of safety against data loss\n"
		      "on CD and DVD media caused by aging or scratches.\n"
		      "It creates error correction data which is used to recover\n"
		      "unreadable sectors if the disc becomes damaged later on.\n"));

   AboutTextWithLink(vbox, _("This software comes with  <b>absolutely no warranty</b>.\n"
				"This is free software and you are welcome to redistribute it\n"
				"under the conditions of the [GNU General Public License].\n"), 
			"GPL");

#ifdef MODIFIED_SOURCE
   AboutTextWithLink(vbox, _("\nThis program is <i>not the original</i>. It is based on the\n"
			     "source code of dvdisaster, but contains third-party changes.\n\n"
			     "Please do not bother the original authors of dvdisaster\n"
			     "([www.dvdisaster.org]) about issues with this version.\n"),
		             "http://www.dvdisaster.org");

#else
   lang = g_getenv("LANG");
   if(lang && !strncmp(lang, "de", 2))
   {    AboutTextWithLink(vbox, "\n[http://www.dvdisaster.de]", "http://www.dvdisaster.de");
   }
   else 
   {    AboutTextWithLink(vbox, "\n[http://www.dvdisaster.com]", "http://www.dvdisaster.com");
   }

   AboutText(vbox, _("\ne-mail: carsten@dvdisaster.com   -or-   cgnoerlich@fsfe.org")); 

#ifdef SYS_DARWIN
   AboutText(vbox, _("\nDarwin port (Mac OS X): Julian Einwag &lt;julian@einwag.de&gt;")); 
#endif
#endif
   /* Show it */

   gtk_widget_show_all(about);
}

