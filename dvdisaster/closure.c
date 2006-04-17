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

#ifdef SYS_MINGW
 #include <windows.h>
 #include <tlhelp32.h>
 #include <psapi.h>
#endif

/***
 *** Locate the binary and documentation directory
 ***/

/*
 * Find the place of our executable
 * (Windows only)
 */

#ifdef SYS_MINGW
static char* get_exe_path()
{  DWORD pid = GetCurrentProcessId();
   HANDLE handle;
   PROCESSENTRY32 pe32;
   HINSTANCE lib;
   DWORD (*GetModuleFileNameExA)(HANDLE,HMODULE,LPSTR,DWORD);

   /* The psapi functions are only available under Win2000 and later. */

   if((lib = LoadLibrary("PSAPI.DLL")))
   {  if((GetModuleFileNameExA = (DWORD(*)(HANDLE,HMODULE,LPSTR,DWORD))GetProcAddress(lib, "GetModuleFileNameExA")))
      {  handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, pid);
         if(handle)
         {  char path[MAX_PATH];
            int n = GetModuleFileNameExA(handle, 0, path, MAX_PATH);
	    
            CloseHandle(handle);
	    FreeLibrary(lib);

	    if(n>0)
            {  char *backslash = strrchr(path, '\\');

               if(backslash) *backslash=0;
	       return g_strdup(path);
            } 
	    else return NULL;
         }
       }
   }

   /* The following only works for Windows 98 */
       
   handle = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
   if(handle)
   {  pe32.dwSize = sizeof(PROCESSENTRY32);
      if(Process32First(handle, &pe32))
      {  do
         {  if(pe32.th32ProcessID == pid)	
	    {  char *backslash,path[MAX_PATH];
	        
               strcpy(path, pe32.szExeFile);
               backslash = strrchr(path, '\\');
               if(backslash) *backslash=0;

	       return strdup(path);
            }
	 } while(Process32Next(handle, &pe32));
      }
      CloseHandle(handle);
   }

   return NULL;
}
#endif

static void get_base_dirs()
{  struct stat mystat;

   /*** The source directory is supposed to hold the most recent files,
	so try this first. */

   if(!stat(SRCDIR, &mystat))
   {  Closure->binDir = g_strdup(SRCDIR);
      Closure->docDir = g_strdup_printf("%s/documentation",SRCDIR);
      goto find_dotfile;
   } 

   /*** Otherwise try the installation directory. 
	On Linux this is a hardcoded directory.
	Windows has binary distributions with no prior known installation place,
	but luckily it provides a way for figuring out that location. */

#if defined(SYS_LINUX) || defined(SYS_FREEBSD) || defined(SYS_UNKNOWN)
   if(!stat(BINDIR, &mystat))
     Closure->binDir = g_strdup(BINDIR);

   if(!stat(DOCDIR, &mystat))
     Closure->docDir = g_strdup(DOCDIR);
#endif

#ifdef SYS_MINGW
   Closure->binDir = get_exe_path();
   if(Closure->binDir)
      Closure->docDir = g_strdup_printf("%s/documentation", Closure->binDir);
#endif

find_dotfile:

   /*** The location of the dotfile depends on the operating system. */

#if defined(SYS_LINUX) || defined(SYS_FREEBSD) || defined(SYS_UNKNOWN)
   Closure->dotFile = g_strdup_printf("%s/.dvdisaster", g_getenv("HOME"));
#endif
#ifdef SYS_MINGW
   Closure->dotFile = g_strdup_printf("%s/.dvdisaster", Closure->binDir);
#endif

#if 0
   g_printf("* File locations:\n*\n"
	    "* Bin dir: %s\n"
	    "* Doc dir: %s\n"
	    "* dotfile: %s\n\n",
	    Closure->binDir,
	    Closure->docDir,
	    Closure->dotFile);   
#endif
}

/***
 *** Save and restore user settings to/from the .dvdisaster file
 ***/

#define MAX_LINE_LEN 512

void ReadDotfile()
{  FILE *dotfile;
   char line[MAX_LINE_LEN];

   dotfile = fopen(Closure->dotFile, "rb");
   if(!dotfile)
      return;

   while(TRUE)
   {  int n;
      char symbol[41];
      char *value;

      /* Get first MAX_LINE_LEN bytes of line, discard the rest */
     
      line[MAX_LINE_LEN-1] = 1;
      fgets(line, MAX_LINE_LEN, dotfile);
      if(!line[MAX_LINE_LEN-1])  /* line longer than buffer */
	while(!feof(dotfile) && fgetc(dotfile) != '\n')
	  ;

      /* Trivially reject the line */

      if(feof(dotfile)) break;
      if(*line == '#') continue;
      if(!sscanf(line, "%40[0-9a-zA-Z-]%n", symbol, &n)) continue;
      if(line[n] != ':') continue;

      /* Separate line contents into symbol: value pair */

      value = line+n+1;
      while(*value && *value == ' ')
	value++;
      if(!*value) continue;
      n = strlen(value);
      if(value[n-1] == '\n')
	value[n-1] = 0;

      /* Parse the symbols which are recognized in this version */

      if(!strcmp(symbol, "last-device"))     { if(Closure->device) g_free(Closure->device);
	                                       Closure->device      = g_strdup(value); continue; }
      if(!strcmp(symbol, "last-image"))      { g_free(Closure->imageName);
					       Closure->imageName   = g_strdup(value); continue; }
      if(!strcmp(symbol, "last-ecc"))        { g_free(Closure->eccName);
                                               Closure->eccName     = g_strdup(value); continue; }
      if(!strcmp(symbol, "browser"))         { g_free(Closure->browser);
                                               Closure->browser     = g_strdup(value); continue; }

      if(!strcmp(symbol, "adaptive-read"))   { Closure->adaptiveRead   = atoi(value); continue; }
      if(!strcmp(symbol, "auto-suffix"))     { Closure->autoSuffix  = atoi(value); continue; }
      if(!strcmp(symbol, "cache-size"))      { Closure->cacheMB     = atoi(value); continue; }
      if(!strcmp(symbol, "cd-size"))         { Closure->cdSize = Closure->savedCDSize = atoll(value); continue; }
      if(!strcmp(symbol, "dao"))             { Closure->noTruncate  = atoi(value); continue; }
      if(!strcmp(symbol, "dotfile-version")) { Closure->dotFileVersion = atoi(value); continue; }
      if(!strcmp(symbol, "dvd-size1"))       { Closure->dvdSize1 = Closure->savedDVDSize1 = atoll(value); continue; }
      if(!strcmp(symbol, "dvd-size2"))       { Closure->dvdSize2 = Closure->savedDVDSize2 = atoll(value); continue; }
      if(!strcmp(symbol, "fill-unreadable")) { Closure->fillUnreadable = atoi(value); 
	                                       PrepareDeadSector();
	                                       continue; 
                                             }
      if(!strcmp(symbol, "jump"))            { Closure->sectorSkip  = atoi(value); continue; }
      if(!strcmp(symbol, "medium-size"))     { Closure->mediumSize  = atoll(value); continue; }
      if(!strcmp(symbol, "method-name"))     { if(Closure->methodName) g_free(Closure->methodName);
	                                       Closure->methodName = g_strdup(value); continue; }
      if(!strcmp(symbol, "parse-ecc"))       { Closure->parseEcc  = atoi(value); continue; }
      if(!strcmp(symbol, "parse-udf"))       { Closure->parseUDF  = atoi(value); continue; }
      if(!strcmp(symbol, "read-and-create")) { Closure->readAndCreate = atoi(value); continue; }
      if(!strcmp(symbol, "redundancy"))      { if(Closure->redundancy) g_free(Closure->redundancy);
                                               Closure->redundancy  = g_strdup(value); continue; }
      if(!strcmp(symbol, "spinup-delay"))    { Closure->spinupDelay = atoi(value); continue; }
      if(!strcmp(symbol, "split-files"))     { Closure->splitFiles  = atoi(value); continue; }
      if(!strcmp(symbol, "unlink"))          { Closure->unlinkImage = atoi(value); continue; }
      if(!strcmp(symbol, "welcome-msg"))     { Closure->welcomeMessage = atoi(value); continue; }
   }

   if(fclose(dotfile))
     g_fprintf(stderr, _("Error closing configuration file %s: %s\n"), 
	       Closure->dotFile, strerror(errno));
}

static void update_dotfile()
{  const char *no_dot_files;
   FILE *dotfile;

   /*** If the environment $NO_DOT_FILES is set,
        do not alter the dotfile. */

   no_dot_files = g_getenv("NO_DOT_FILES");

   if(no_dot_files && atoi(no_dot_files))
      return;

   /*** Otherwise, save our session */

   dotfile = fopen(Closure->dotFile, "wb");
   if(!dotfile)
   {  g_fprintf(stderr, _("Could not open configuration file %s: %s\n"), 
		Closure->dotFile, strerror(errno));
      return;
   }

   g_fprintf(dotfile, 
	     _("# dvdisaster-%s configuration file\n"
	       "# This is an automatically generated file\n"
	       "# which will be overwritten each time dvdisaster is run.\n\n"),
	     VERSION);

   g_fprintf(dotfile, "last-device:     %s\n", Closure->device);
   g_fprintf(dotfile, "last-image:      %s\n", Closure->imageName);
   g_fprintf(dotfile, "last-ecc:        %s\n", Closure->eccName);
   g_fprintf(dotfile, "browser:         %s\n\n", Closure->browser);

   g_fprintf(dotfile, "adaptive-read:   %d\n", Closure->adaptiveRead);
   g_fprintf(dotfile, "auto-suffix:     %d\n", Closure->autoSuffix);
   g_fprintf(dotfile, "cache-size:      %d\n", Closure->cacheMB);
   g_fprintf(dotfile, "cd-size:         %lld\n", Closure->cdSize);
   g_fprintf(dotfile, "dao:             %d\n", Closure->noTruncate);
   g_fprintf(dotfile, "dotfile-version: %d\n", Closure->dotFileVersion);
   g_fprintf(dotfile, "dvd-size1:       %lld\n", Closure->dvdSize1);
   g_fprintf(dotfile, "dvd-size2:       %lld\n", Closure->dvdSize2);
   g_fprintf(dotfile, "fill-unreadable: %d\n", Closure->fillUnreadable);
   g_fprintf(dotfile, "jump:            %d\n", Closure->sectorSkip);
   g_fprintf(dotfile, "medium-size:     %lld\n", Closure->mediumSize);
   g_fprintf(dotfile, "method-name:     %s\n", Closure->methodName);
   g_fprintf(dotfile, "parse-ecc:       %d\n", Closure->parseEcc);
   g_fprintf(dotfile, "parse-udf:       %d\n", Closure->parseUDF);
   g_fprintf(dotfile, "read-and-create: %d\n", Closure->readAndCreate);
   if(Closure->redundancy)
     g_fprintf(dotfile, "redundancy:    %s\n", Closure->redundancy);
   g_fprintf(dotfile, "spinup-delay:    %d\n", Closure->spinupDelay);
   g_fprintf(dotfile, "split-files:     %d\n", Closure->splitFiles);
   g_fprintf(dotfile, "unlink:          %d\n", Closure->unlinkImage);
   g_fprintf(dotfile, "welcome-msg:     %d\n", Closure->welcomeMessage);

   if(fclose(dotfile))
     g_fprintf(stderr, _("Error closing configuration file %s: %s\n"), 
	       Closure->dotFile, strerror(errno));

}

/***
 *** Allocate and initialize our global variables
 ***/

GlobalClosure *Closure;
int exitCode = EXIT_SUCCESS;

void InitClosure()
{  int v1,v2,v3,dots=0;
   char *v,version[strlen(VERSION)+1];

   Closure = g_malloc0(sizeof(GlobalClosure));

   /* Give versions with patch levels a nicer formatting */

   if(!strcmp(RELEASE_STATUS, "patch"))
     Closure->cookedVersion = g_strdup_printf("%s (pl%s)",VERSION,RELEASE_MICRO);
   else if(!strcmp(RELEASE_STATUS, "devel"))
   { Closure->releaseFlags = MFLAG_DEVEL;
     Closure->cookedVersion = g_strdup_printf("%s (devel-%s)",VERSION,RELEASE_MICRO);
   }
   else if(!strcmp(RELEASE_STATUS, "rc"))
   { Closure->releaseFlags = MFLAG_RC;
     Closure->cookedVersion = g_strdup_printf("%s (rc-%s)",VERSION,RELEASE_MICRO);
   }
   else Closure->cookedVersion = g_strdup(VERSION);

   /* Replace the dot with a locale-resistant separator */

   strcpy(version,VERSION);
   for(v=version; *v; v++)
     if(*v=='.') 
     {  *v='x';
        dots++;
     }

   if(dots == 1) 
   {  v1 = v2 = v3 = 0;
      sscanf(version,"%dx%d",&v1,&v2);
   }
   else 
   {  g_printf("Error: malformed version number %s\n",VERSION);
      exit(EXIT_FAILURE);
   }

   v3 = atoi(RELEASE_MICRO);
   Closure->version = 10000*v1 + 100*v2 + v3;

#if 0
   printf("Version %s; %d; Flags %d\n", Closure->cookedVersion, Closure->version, Closure->releaseFlags);
#endif

   /* Fill in other closure defaults */

   Closure->deviceNames = g_ptr_array_new();
   Closure->deviceNodes = g_ptr_array_new();
   Closure->imageName   = g_strdup("medium.iso");
   Closure->eccName     = g_strdup("medium.ecc");
   Closure->browser     = g_strdup("gnome-open");
   Closure->methodList  = g_ptr_array_new();
   Closure->methodName  = g_strdup("RS01");
   Closure->cacheMB     = 32;
   Closure->sectorSkip  = 16;
   Closure->spinupDelay = 5;
   Closure->parseEcc    = 1;
   Closure->fillUnreadable = -1;
   Closure->welcomeMessage = 1;

   /* default sizes for typical CD and DVD media */

   Closure->cdSize   = Closure->savedCDSize   = CDR_SIZE;
   Closure->dvdSize1 = Closure->savedDVDSize1 = DVD_SL_SIZE;
   Closure->dvdSize2 = Closure->savedDVDSize2 = DVD_DL_SIZE;

   /*** Align the buffer at a 4096 boundary.
	Might be needed by some SCSI drivers. */

   Closure->scratchBufBase = g_malloc(32768+4096);
   Closure->scratchBuf = Closure->scratchBufBase 
                         + (4096 - ((unsigned long)Closure->scratchBufBase & 4095));

   Closure->logString = g_string_sized_new(1024);

   Closure->background = g_malloc0(sizeof(GdkColor));
   Closure->black = g_malloc0(sizeof(GdkColor));
   Closure->white = g_malloc0(sizeof(GdkColor));
   Closure->red   = g_malloc0(sizeof(GdkColor));
   Closure->yellow= g_malloc0(sizeof(GdkColor));
   Closure->green = g_malloc0(sizeof(GdkColor));
   Closure->darkgreen = g_malloc0(sizeof(GdkColor));
   Closure->blue  = g_malloc0(sizeof(GdkColor));
   Closure->grid  = g_malloc0(sizeof(GdkColor));

   memset(Closure->bs, '\b', 255);

   get_base_dirs();

#ifdef SYS_MINGW
   OpenAspi();
#endif
}

/*
 * Clear the CRC cache
 */

void ClearCrcCache(void)
{  if(Closure->crcCache)
      g_free(Closure->crcCache);
   if(Closure->crcImageName)
      g_free(Closure->crcImageName);

   Closure->crcCache = NULL;
   Closure->crcImageName = NULL;
   memset(Closure->md5Cache, 0, 16);
}

/*
 * Clean up properly 
 */

#define cond_free(x) if(x) g_free(x)

/* Doing a simple g_ptr_array_free(a, TRUE)
   would confuse our memory leak checker */

void cond_free_ptr_array(GPtrArray *a) 
{  unsigned int i;

   if(!a) return;

   for(i=0; i<a->len; i++)
     g_free(g_ptr_array_index(a,i));

   g_ptr_array_free(a, FALSE);
}
    
void FreeClosure()
{
   if(Closure->guiMode)
     update_dotfile();

   ClearCrcCache();

   cond_free(Closure->cookedVersion);
   cond_free(Closure->device);
   cond_free_ptr_array(Closure->deviceNames);
   cond_free_ptr_array(Closure->deviceNodes);
   cond_free(Closure->imageName);
   cond_free(Closure->eccName);
   cond_free(Closure->redundancy);

   CallMethodDestructors();
   cond_free_ptr_array(Closure->methodList);

   cond_free(Closure->methodName);
   cond_free(Closure->dotFile);
   cond_free(Closure->binDir);
   cond_free(Closure->docDir);
   cond_free(Closure->browser);
   cond_free(Closure->deadSector);
   cond_free(Closure->errorTitle);
   cond_free(Closure->scratchBufBase);

   if(Closure->prefsContext)
     FreePreferences(Closure->prefsContext);

   if(Closure->logString)
     g_string_free(Closure->logString, TRUE);

   if(Closure->drawGC)
     g_object_unref(Closure->drawGC);
   cond_free(Closure->red);
   cond_free(Closure->yellow);
   cond_free(Closure->green);
   cond_free(Closure->darkgreen);
   cond_free(Closure->blue);
   cond_free(Closure->black);
   cond_free(Closure->white);
   cond_free(Closure->grid);
   cond_free(Closure->background);

   if(Closure->readLinearCurve)
     FreeCurve(Closure->readLinearCurve);

   if(Closure->readLinearSpiral)
     FreeSpiral(Closure->readLinearSpiral);

   if(Closure->readAdaptiveSpiral)
     FreeSpiral(Closure->readAdaptiveSpiral);

   if(Closure->readAdaptiveSubtitle)
     g_free(Closure->readAdaptiveSubtitle);

   if(Closure->readAdaptiveErrorMsg)
     g_free(Closure->readAdaptiveErrorMsg);

   g_free(Closure);

#ifdef SYS_MINGW
   CloseAspi();
#endif
}
