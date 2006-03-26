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

#ifdef SYS_LINUX
#include <sys/wait.h>
#endif

#ifdef SYS_MINGW
#include "windows.h"
#include "shellapi.h"
#endif

/***
 *** Ask user to specify his browser
 ***/

#ifdef SYS_LINUX
#define SEARCH_BUTTON 1

typedef struct
{  GtkWidget *dialog;
   GtkWidget *entry;
   GtkWidget *search;
   GtkWidget *filesel;
   GtkWidget *fileok;
   GtkWidget *filecancel;
   char *url;
} browser_dialog_info;

static void response_cb(GtkWidget *widget, int response, gpointer data)
{  browser_dialog_info *bdi = (browser_dialog_info*)data; 

   switch(response)
   {  case GTK_RESPONSE_ACCEPT:
	if(Closure->browser) g_free(Closure->browser);
	Closure->browser = g_strdup(gtk_entry_get_text(GTK_ENTRY(bdi->entry)));
	ShowHTML(bdi->url);
	break;

      case GTK_RESPONSE_REJECT:
	if(bdi->url) g_free(bdi->url);
        break;
   }
   gtk_widget_destroy(widget);
   if(bdi->filesel)
     gtk_widget_destroy(bdi->filesel);
   g_free(bdi);
}

static void search_cb(GtkWidget *widget, gpointer data)
{  browser_dialog_info *bdi = (browser_dialog_info*)data; 

   if(widget == bdi->search) 
   {  bdi->filesel = gtk_file_selection_new(_utf("windowtitle|Choose a browser"));
      bdi->fileok = GTK_FILE_SELECTION(bdi->filesel)->ok_button;
      bdi->filecancel = GTK_FILE_SELECTION(bdi->filesel)->cancel_button;
      ReverseCancelOK(GTK_DIALOG(bdi->filesel));
      gtk_file_selection_hide_fileop_buttons(GTK_FILE_SELECTION(bdi->filesel));
      g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(bdi->filesel)->ok_button), "clicked", 
		       G_CALLBACK(search_cb), bdi);
    
      g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(bdi->filesel)->cancel_button), "clicked", 
		       G_CALLBACK(search_cb), bdi);
      
      gtk_widget_show(bdi->filesel);
   }

   if(widget == bdi->fileok)
   {
      if(Closure->browser) g_free(Closure->browser);
      Closure->browser = g_strdup(gtk_file_selection_get_filename(GTK_FILE_SELECTION(bdi->filesel)));
      ShowHTML(bdi->url);
      gtk_widget_destroy(bdi->filesel);
      gtk_widget_destroy(bdi->dialog);
      g_free(bdi);
      return;
   }

   if(widget == bdi->filecancel)
   {  gtk_widget_destroy(bdi->filesel);
      bdi->filesel = NULL;
   }
}

static void browser_dialog(char *url)
{  GtkWidget *dialog, *vbox, *hbox, *label, *entry, *button;
   browser_dialog_info *bdi = g_malloc0(sizeof(browser_dialog_info));

   /* Create the dialog */

   dialog = gtk_dialog_new_with_buttons(_utf("windowtitle|Browser required"), 
				       Closure->window, GTK_DIALOG_DESTROY_WITH_PARENT,
				       GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, 
				       GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, NULL);
   bdi->dialog = dialog;
   if(url)
   {  bdi->url = g_strdup(url);
   }

   vbox = gtk_vbox_new(FALSE, 0);
   gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), vbox, FALSE, FALSE, 0);
   gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);

   /* Insert the contents */

   label = gtk_label_new(NULL);
   gtk_label_set_markup(GTK_LABEL(label), _utf("<b>Could not find a suitable browser.</b>\n\n"
                                               "Which browser would you like to use\n"
                                               "for reading the online documentation?\n\n"
			                       "Please enter its name (e.g. mozilla) or\n"
			                       "use the \"Search\" button for a file dialog.\n")),
			      gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 10);

   hbox = gtk_hbox_new(FALSE, 0);
   gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 10);

   bdi->entry = entry = gtk_entry_new();
   gtk_box_pack_start(GTK_BOX(hbox), entry, FALSE, FALSE, 10);

   bdi->search = button = gtk_button_new_with_label(_utf("Search"));
   g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(search_cb), bdi);
   gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 10);

   /* Show it */

   g_signal_connect(dialog, "response", G_CALLBACK(response_cb), bdi);

   gtk_widget_show_all(dialog);
}
#endif /* SYS_LINUX */

/***
 *** Show the manual in an external browser
 ***/

/*
 * Check the child processes exit status
 * to find whether the browser could be invoked.
 */

typedef struct
{  pid_t pid;
   char *url;
   GtkWidget *msg;
   int seconds;
} browser_info;


static void msg_destroy_cb(GtkWidget *widget, gpointer data)
{  browser_info *bi = (browser_info*)data;

   bi->msg = NULL; 
}

#ifdef SYS_LINUX

/* 
 * The following list of browsers and html wrappers
 * will be tried one at a time until one entry succeeds by:
 * - returning zero
 * - not returning within 60 seconds
 */

static int browser_index;
static void try_browser(browser_info*);

static char *browsers[] = 
{  "user-selection",
   "gnome-open",
   "htmlview",
   "firefox",
   "mozilla",
   "konqueror",
   "netscape",
   "opera",
   NULL
};

static gboolean browser_timeout_func(gpointer data)
{  browser_info *bi = (browser_info*)data;
   int status;

   waitpid(bi->pid, &status, WNOHANG);

   if(WIFEXITED(status))
   {  switch(WEXITSTATUS(status))
      {  case 0:  /* browser was successful */
	   if(bi->msg) 
	     gtk_widget_destroy(bi->msg);
	   if(bi->url) 
	     g_free(bi->url);
	   g_free(bi);
	   return FALSE;

         case 110: /* browser did not execute */
         default:  /* browser executed, but failed afterwards */
	   browser_index++;
	   if(!browsers[browser_index]) /* all browsers from the list failed */
	   {  browser_dialog(bi->url);

	      if(bi->msg) 
		gtk_widget_destroy(bi->msg);
	      if(bi->url) 
		g_free(bi->url);
	      g_free(bi);
	   }
	   else                        /* try next browser from list */
	   {  bi->seconds = 0;  
	      try_browser(bi);
	   }
	   return FALSE;
	   break;
      }
   }

   bi->seconds++;
   if(bi->seconds == 10 && bi->msg)
   {  gtk_widget_destroy(bi->msg);
      bi->msg = NULL;
   }

   return bi->seconds > 60 ? FALSE : TRUE;
}
#endif /* SYS_LINUX */

#ifdef SYS_MINGW
static gboolean browser_timeout_func(gpointer data)
{  browser_info *bi = (browser_info*)data;
   
   bi->seconds++;
   if(bi->seconds >= 10)
   {  if(bi->msg)
      {  gtk_widget_destroy(bi->msg);
         bi->msg = NULL;
      }
      if(bi->url) g_free(bi->url);
      g_free(bi);
      return FALSE;
   }

   return TRUE;
}
#endif /* SYS_MINGW */

/*
 * Invoke the browser
 */

#ifdef SYS_LINUX
static void try_browser(browser_info *bi)
{  pid_t pid;

   bi->pid = pid = fork();

   if(pid == -1)
   {  printf("fork failed\n");
      return;
   }

   /* make the parent remember and wait() for the browser */

   if(pid > 0)  
   {  g_timeout_add(1000, browser_timeout_func, (gpointer)bi);

      if(browser_index)
      {  g_free(Closure->browser);
	 Closure->browser = g_strdup(browsers[browser_index]);
      }
   }

   /* try calling the browser */

   if(pid == 0)
   {  char *argv[10];
      int argc = 0;

      argv[argc++] = browser_index ? browsers[browser_index] : Closure->browser;
      argv[argc++] = bi->url;
      argv[argc++] = NULL;
      execvp(argv[0], argv);

      _exit(110); /* couldn't execute */
   }
}
#endif /* SYS_LINUX */


void ShowHTML(char *target)
{  browser_info *bi = g_malloc0(sizeof(browser_info));
   struct stat mystat;
   const char *lang;

   /* If no target is given, select between translations of the manual. */

   if(!target)
   { 
      if(!Closure->docDir)
      {  CreateMessage(_("Documentation not installed."), GTK_MESSAGE_ERROR);
         g_free(bi);
         return;
      }

      lang = g_getenv("LANG");
      
      if(!strncmp(lang, "cs", 2)) 
	   target = g_strdup_printf("%s/cs/index.html",Closure->docDir); 
      else if(!strncmp(lang, "de", 2)) 
	   target = g_strdup_printf("%s/de/index.html",Closure->docDir); 
      else target = g_strdup_printf("%s/en/index.html",Closure->docDir); 

#ifdef SYS_MINGW      
      if(stat(target, &mystat) == -1)
      {  g_free(target);  /* the local dir is Windows specific */
         target = g_strdup_printf("%s/local/index.html",Closure->docDir);
      }
#endif
      
      bi->url = target;
   }
   else bi->url = target;

   if(stat(target, &mystat) == -1)
   {  CreateMessage(_("Documentation file\n%s\nnot found.\n"), GTK_MESSAGE_ERROR, target);
      g_free(bi);
      g_free(target);
      return;
   }

   /* Lock the help button and show a message for 10 seconds. */

   TimedInsensitive(Closure->helpButton, 10000);
   bi->msg = CreateMessage(_("Please hang on until the browser comes up!"), GTK_MESSAGE_INFO);
   g_signal_connect(G_OBJECT(bi->msg), "destroy", G_CALLBACK(msg_destroy_cb), bi);

#ifdef SYS_MINGW
   /* Okay, Billy wins big time here ;-) */

   ShellExecute(NULL, "open", target, NULL, NULL, SW_SHOWNORMAL);
   g_timeout_add(1000, browser_timeout_func, (gpointer)bi);
#endif

#ifdef SYS_LINUX
   /* Try the first browser */

   browser_index = 0;
   try_browser(bi);
#endif /* SYS_LINUX */
}
