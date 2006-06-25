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

#ifndef DVDISASTER_H
#define DVDISASTER_H

/* "Dare to be gorgeous and unique. 
 *  But don't ever be cryptic or otherwise unfathomable.
 *  Make it unforgettably great."
 *
 *  From "A Final Note on Style", 
 *  Amiga Intuition Reference Manual, 1986, p. 231
 */

/***
 *** I'm too lazy to mess with #include dependencies.
 *** Everything #includeable is rolled up herein...
 */

#define _GNU_SOURCE

//#include <glib.h>

#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <glib/gstrfuncs.h>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#ifdef WITH_NLS_YES
 #include <libintl.h>
 #include <locale.h>
#endif
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef SYS_MINGW
#include <io.h>
#endif

#include "md5.h"

#ifndef G_THREADS_ENABLED
 #error "need multithreading glib2"
#endif

/* Phrase extraction for gettext() 
   Note that these functions are even required when
   WITH_NLS_NO is set! */

#define _(string) sgettext(string)
#define _utf(string) sgettext_utf8(string)

/* File permissions for images */

#ifdef SYS_MINGW
#define IMG_PERMS (S_IRUSR | S_IWUSR)
#else
#define IMG_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)
#endif

/* Using round() is preferred over rint() on systems which have it */

#ifndef HAVE_ROUND
 #define round(x) rint(x)
#endif

/* Some standard media sizes */

#define CDR_SIZE         (351*1024)
#define DVD_SL_SIZE      2295104  /* DVD+R/RW size used at least common denominator */
#define DVD_DL_SIZE 	 4171712  /* also seen: 4148992 4173824  */

/***
 *** Our global closure (encapsulation of global variables)
 ***/

typedef struct _GlobalClosure
{  int version;         /* Integer number representing current program version */
   char *cookedVersion; /* version string formatted for GUI use */
   gint8 releaseFlags;  /* flags marking release status */
   char *device;        /* currently selected device to read from */
   GPtrArray *deviceNames;  /* List of drive names */
   GPtrArray *deviceNodes;  /* List of device nodes (C: or /dev/foo) */
   char *imageName;     /* complete path of current image file */
   char *eccName;       /* complete path of current ecc file */
   GPtrArray *methodList; /* List of available methods */
   char *methodName;    /* Name of currently selected codec */
   gint64 readStart;    /* Range to read */
   gint64 readEnd;
   gint64 cdSize;       /* Maximum cd size (for RS02 type images) */
   gint64 dvdSize1;     /* Maximum 1-layer dvd size (for RS02 type images) */
   gint64 dvdSize2;     /* Maximum 2-layer dvd size (for RS02 type images) */
   gint64 savedCDSize;  /* Undo values for above */
   gint64 savedDVDSize1;
   gint64 savedDVDSize2;
   gint64 mediumSize;   /* Maximum medium size (for RS02 type images) */
   int cacheMB;         /* Cache setting for the parity codec, in megabytes */
   int sectorSkip;      /* Number of sectors to skip after read error occurs */
   char *redundancy;    /* Error correction code redundancy */
   int adaptiveRead;    /* Use optimized strategy for reading defective images */
   int speedWarning;    /* Print warning if speed changes by more than given percentage */
   int fillUnreadable;  /* Byte value for filling unreadable sectors or -1 */
   int spinupDelay;     /* Seconds to wait for drive to spin up */
   int truncate;        /* confirms truncation of large images */
   int noTruncate;      /* do not truncate image at the end */
   int unlinkImage;     /* delete image after ecc file creation */
   int driveSpeed;      /* currently unused */
   int debugMode;       /* may activate additional features */
   int verbose;         /* may activate additional messages */
   int splitFiles;      /* limit image files to 2GB */
   int autoSuffix;      /* automatically extend files with suffices .iso/.ecc */
   int querySize;       /* what sources are used for image size queries */
   int readAndCreate;   /* automatically create .ecc file after reading an image */
   int enableCurveSwitch; /* TRUE in readAndCreateMode after reading is complete */
   int welcomeMessage;  /* just print dvdisaster logo if FALSE */
   int dotFileVersion;  /* version of dotfile */
   int simulateDefects; /* if >0, this is the percentage of simulated media defects */
  
   char *deadSector;    /* Copy of our "dead sector" marker. */
   char *dotFile;       /* path to .dvdisaster file */
   char *logFile;       /* path to logfile */
   char *binDir;        /* place where the binary resides */
   char *docDir;        /* place where our documentation resides */
   char *browser;       /* Name of preferred WEB browser */

   char bs[256];        /* A string of 255 backspace characters */

   GThread *mainThread; /* Thread of the main() routine */
   void (*cleanupProc)(gpointer);  /* Procedure to cleanup running threads after an error condition */
   gpointer cleanupData;
   char *errorTitle;    /* Title to show in error dialogs */
   gint32 randomSeed;   /* for the random number generator */

#ifdef SYS_MINGW
   /*** Hooks into the ASPI library */

   void *aspiLib;
   unsigned long (*GetASPI32SupportInfo)(void);
   unsigned long (*SendASPI32Command)(void*);
#endif

   unsigned char *scratchBufBase;  /* scratch buffer for use with SCSI commands; */
   unsigned char *scratchBuf;      /* 32768 bytes aligned at a 4096 byte boundary */

   guint32 *crcCache;              /* sectorwise CRC32 for last image read */
   char    *crcImageName;          /* file name of cached image */
   unsigned char md5Cache[16];     /* md5sum of last image read */
   

   /*** GUI-related things */

   int guiMode;         /* TRUE if GUI is active */
   int keepStyle;       /* TRUE = don't tinker with style */
   int stopActions;     /* crude method to stop ongoing action(s) */

   GtkWidget *logWidget;     /* Dialog for the log display */
   GtkScrolledWindow *logScroll; /* and its scrolled window */
   GtkTextBuffer *logBuffer; /* Text buffer for the log output */
   GString *logString;       /* holds logging output for current action */

   /*** Widgets of the main window */

   GtkWindow *window;        /* main window */
   GtkTooltips *tooltips;    /* our global tooltips structure */
   GdkPixbuf *windowIcon;    /* main window icon */

   GtkWidget *imageFileSel;  /* image file selector */
   GtkWidget *imageEntry;    /* image name entry field */
   GtkWidget *eccFileSel;    /* ecc file selector */
   GtkWidget *eccEntry;      /* ecc name entry field */
   
   GtkWidget *notebook;      /* The notebook behind our central output area */
   GtkLabel  *status;        /* The status label */

   GtkWidget *prefsButton;
   GtkWidget *helpButton;

   GtkWidget *readButton;
   GtkWidget *createButton;
   GtkWidget *scanButton;
   GtkWidget *fixButton;
   GtkWidget *testButton;

   /*** The preferences window */

   GtkWindow *prefsWindow;
   void *prefsContext;       /* local data for the preferences window */

   /*** Common stuff for drawing curves and spirals */

   GdkGC     *drawGC;
   GdkColor  *background,*grid,*black,*white;
   GdkColor  *red,*yellow,*green,*darkgreen,*blue;
   gint      lastPercent;
   gint      lastSegment;
   gint      lastPlotted;
   gint      lastPlottedY;
   gint      percent; 

   /*** Widgets for the linear reading/scanning action */

   GtkWidget *readLinearHeadline;
   GtkWidget *readLinearDrawingArea;
   struct _Curve  *readLinearCurve;
   struct _Spiral *readLinearSpiral;
   GtkWidget *readLinearNotebook;
   GtkWidget *readLinearSpeed;
   GtkWidget *readLinearErrors;
   GtkWidget *readLinearFootline;
   GtkWidget *readLinearFootlineBox;
   int    checkCrc;               /* these are passed between threads and */
   gint64 crcErrors, readErrors;  /* must therefore be global */

   /*** Widgets for the adaptive reading action */

   GtkWidget *readAdaptiveHeadline;
   GtkWidget *readAdaptiveDrawingArea;
   struct _Spiral *readAdaptiveSpiral;
   char *readAdaptiveSubtitle;
   char *readAdaptiveErrorMsg;
   int additionalSpiralColor;

} GlobalClosure;

extern GlobalClosure *Closure;  /* these should be the only global variables! */
extern int exitCode;            /* value to use on exit() */

/***
 *** 
 ***/

#define MAX_FILE_SEGMENTS 100

typedef struct _LargeFile
{  int fileSegment[MAX_FILE_SEGMENTS];
   int flags;
   mode_t mode;
   int segment;
   gint64 offset;
   char *basename;
   char *suffix;
   int namelen;
   gint64 size;
} LargeFile;

/***
 *** An info package about a medium image 
 *** (NOT part or a header of the image file!)
 */

typedef struct _ImageInfo
{  LargeFile *file;                  /* file handle for image */
   gint64 size;                      /* number of medium bytes */
   gint64 sectors;                   /* number of medium sectors */
   gint64 sectorsMissing;            /* number of missing medium sectors */
   gint64 crcErrors;                 /* sectors with CRC32 errors */
   int inLast;                       /* bytes in last sector */
   unsigned char mediumFP[16];       /* md5sum of first sector */
   unsigned char mediumSum[16];      /* complete md5sum of whole medium */
} ImageInfo;

/***
 *** An info package about a error correction file 
 *** (NOT part or a header of the ecc file!)
 */

typedef struct _EccInfo
{  LargeFile *file;             /* file handle for ecc file */
   struct _EccHeader *eh;       /* the header struct below */
   gint64 sectors;              /* gint64 version of eh->sectors */ 
   struct MD5Context md5Ctxt;   /* md5 context of crc portion of file */
} EccInfo;

/***
 *** The .ecc file header
 ***/

#define FINGERPRINT_SECTOR 16 /* Sector currently used to calculate the fingerprint. */
                              /* This is the ISO filesystem root sector which contains */
                              /* the volume label and creation time stamps. */
                              /* Versions upto 0.64 used sector 257, */
                              /* but that was not a wise choice for CD media.*/

#define MFLAG_DEVEL (1<<0)    /* for methodFlags[3] */
#define MFLAG_RC    (1<<1)                      

typedef struct _EccHeader
{  gint8 cookie[12];           /* "*dvdisaster*" */
   gint8 method[4];            /* e.g. "RS01" */
   gint8 methodFlags[4];       /* 0-2 for free use by the respective methods; 3 see above */
   guint8 mediumFP[16];        /* fingerprint of FINGERPRINT SECTOR */ 
   guint8 mediumSum[16];       /* complete md5sum of whole medium */
   guint8 eccSum[16];          /* md5sum of ecc code section of .ecc file */
   guint8 sectors[8];          /* number of sectors medium is supposed to have */
   gint32 dataBytes;           /* data bytes per ecc block */
   gint32 eccBytes;            /* ecc bytes per ecc block */
   gint32 creatorVersion;      /* which dvdisaster version created this */
   gint32 neededVersion;       /* oldest version which can decode this file */
   gint32 fpSector;            /* sector used to calculate mediumFP */
   guint32 selfCRC;            /* CRC32 of EccHeader (currently RS02 only) -- since V0.66 --*/
   guint8 crcSum[16];          /* md5sum of crc code section of RS02 .iso file  */
   gint32 inLast;              /* bytes contained in last sector */
   gint8 padding[3976];        /* pad to 4096 bytes: room for future expansion */

  /* Note: Bytes 2048 and up are currently used by the RS02 codec
           for a copy of the first ecc blocks CRC sums. */
} EccHeader;

/***
 *** dvdisaster.c
 ***/

void PrepareDeadSector(void);

void CreateEcc(void);
void FixEcc(void);
void Verify(void);

/***
 *** bitmap.c
 ***/

typedef struct _Bitmap
{  guint32 *bitmap;
   gint32 size;
   gint32 words;
} Bitmap;

Bitmap* CreateBitmap0(int);
#define GetBit(bm,bit) (bm->bitmap[(bit)>>5] & (1<<((bit)&31))) 
#define SetBit(bm,bit) bm->bitmap[(bit)>>5] |= (1<<((bit)&31)) 
#define ClearBit(bm,bit) bm->bitmap[(bit)>>5] &= ~(1<<((bit)&31)) 
int CountBits(Bitmap*);
void FreeBitmap(Bitmap*);

/***
 *** closure.c
 ***/

void InitClosure(void);
void ClearCrcCache(void);
void FreeClosure(void);
void ReadDotfile(void);

/***
 *** crc32.c
 ***/

guint32 Crc32(unsigned char*, int);

/***
 *** curve.c
 ***/

enum 
{  CURVE_PERCENT,
   CURVE_MEGABYTES
}; 

typedef struct _Curve
{  GtkWidget *widget;   /* drawing area which is hosting us */
   PangoLayout *layout;
   gdouble *fvalue;     /* floating point curve values */
   gint    *ivalue;     /* integer curve values */
   gint lastValueIdx;   /* end of value array */
   gint leftX,rightX;   /* Position of left and right y axis */
   gint topY,bottomY;   /* Top and bottom end of y axes */
   char *leftLabel;     /* Label of left coordinate axis */
   char *leftFormat;    /* Format of left coordinate axis numbering */
   int bottomFormat;    /* what kind of data are we displaying? */
   int margin;
   int maxX,maxY;       /* current max value at y axis */
} Curve;

#define REDRAW_ICURVE (1<<0)
#define REDRAW_FCURVE (1<<1)

Curve* CreateCurve(GtkWidget*, char*, char*, int, int);
void ZeroCurve(Curve*);
void FreeCurve(Curve*);

void UpdateCurveGeometry(Curve*, char*, int);

int  CurveX(Curve*, gdouble);
int  CurveY(Curve*, gdouble);
void RedrawAxes(Curve*);
void RedrawCurve(Curve*, int, int);

/***
 *** debug.c
 ***/

void Byteset(char*);
void Erase(char*);
void RandomError(char*, char*);
void RandomImage(char*, char*);
void SendCDB(char*);
void ShowSector(char*);
Bitmap* SimulateDefects(gint64);
void TruncateImage(char*);
void ZeroUnreadable(void);

/***
 *** endian.c
 ***/

guint32 SwapBytes32(guint32);
guint64 SwapBytes64(guint64);
void    SwapEccHeaderBytes(EccHeader*);

/***
 *** file.c
 ***/

#define READABLE_IMAGE    0
#define READABLE_ECC      0
#define WRITEABLE_IMAGE   (1<<0)
#define WRITEABLE_ECC     (1<<1)
#define PRINT_MODE        (1<<4)
#define CREATE_CRC        ((1<<1) | (1<<5))

ImageInfo* OpenImageFile(EccHeader*, int);
EccInfo* OpenEccFile(int);
EccInfo* OpenEccFileOld(int);
void FreeImageInfo(ImageInfo*);
void FreeEccInfo(EccInfo*);

char *ApplyAutoSuffix(char*, char*);
int VerifyVersion(EccHeader*, int fatal);
void UnlinkImage(GtkWidget*);

/***
 *** fix-window.c
 ***/

void CreateFixWindow(GtkWidget*);

/***
 *** galois.c
 ***
 * This is currently the hardcoded GF(2**8).
 * gint32 gives abundant space for the GF.
 * Squeezing it down to guint8 won't probably gain much,
 * so we implement this defensively here.
 *
 * Note that some performance critical stuff needs to
 * be #included from galois-inlines.h
 */  

#define SYMBOLSIZE 8
#define FIELDSIZE (1<<SYMBOLSIZE)
#define FIELDMAX (FIELDSIZE-1)
#define ALPHA0 FIELDMAX

#define GENERATOR_POLY 0x187    /* 1 + X + X**2 + X**7 + X**8 */
#define FIRST_ROOT 112          /* same choices as in CCSDS */                 
#define PRIM_ELEM 11                  
#define PRIMTH_ROOT 116         /* prim-th root of 1 */           

typedef struct _GaloisTables
{  gint32 nroots;        /* degree of generator polynomial */
   gint32 ndata;         /* data bytes per ecc block */

   gint32 *index_of;     /* log */
   gint32 *alpha_to;     /* inverse log */
   gint32 *enc_alpha_to; /* inverse log optimized for encoder */
   gint32 *gpoly;        /* generator polynomial */
} GaloisTables;

GaloisTables* CreateGaloisTables(int);
void FreeGaloisTables(GaloisTables*);

/***
 *** help-dialogs.c
 ***/

GtkWidget* ShowTextfile(char*, char*, char*, GtkScrolledWindow**, GtkTextBuffer**);
void ShowGPL();
void ShowLog();
void UpdateLog();
void AboutDialog();

void AboutText(GtkWidget*, char*, ...);
void AboutLink(GtkWidget*, char*, char*);
void AboutTextWithLink(GtkWidget*, char*, char*);

/***
 *** icon-factory.c
 ***/

void CreateIconFactory();

/***
 *** large-io.c
 ***/

LargeFile *LargeOpen(char*, int, mode_t);
int LargeSeek(LargeFile*, gint64);
int LargeEOF(LargeFile*);
int LargeRead(LargeFile*, void*, size_t);
int LargeWrite(LargeFile*, void*, size_t);
int LargeClose(LargeFile*);
int LargeTruncate(LargeFile*, gint64);
int LargeStat(char*, gint64*);
int LargeUnlink(char*);

/***
 *** main-window.c
 ***/

#define FIRST_CREATE_WINDOW 3

typedef enum 
{  ACTION_WELCOME,   /* Tab 0; not really an action   */
   ACTION_STOP,      /* ----   does not have a window */
   ACTION_READ,      /* Tab 1 (linear); Tab 2 (adaptive)*/
   ACTION_SCAN,      /* Tab 1 (linear); Tab 2 (adaptive)*/
   ACTION_VERIFY,    /* VERIFY, CREATE and FIX have separate windows assigned */
   ACTION_CREATE,    /* for each method. */
   ACTION_CREATE_CONT,
   ACTION_FIX        
} MajorActions;

void CreateMainWindow(int*, char***);
void ContinueWithAction(int);

/***
 *** memtrack.c
 ***/

/*
 * Macro replacements for the glib functions.
 */

#ifdef WITH_MEMDEBUG_YES
#define g_malloc(size) malloc_ext(size,__FILE__,__LINE__)
#define g_malloc0(size) malloc_ext(size,__FILE__,__LINE__)
#define g_realloc(ptr,size) realloc_ext(ptr,size,__FILE__,__LINE__)
#define g_strdup(str) strdup_ext(str,__FILE__,__LINE__)

#define g_try_malloc(size) try_malloc_ext(size,__FILE__,__LINE__)

#define g_strdup_printf(format,args...) \
        strdup_printf_ext(format,__FILE__,__LINE__, ## args)
#define g_strdup_vprintf(format,argp) \
        strdup_vprintf_ext(format,argp,__FILE__,__LINE__)
#define g_locale_to_utf8(str,size,in,out,gerr) \
        g_locale_to_utf8_ext(str,size,in,out,gerr,__FILE__,__LINE__)
#define g_free(size) free_ext(size,__FILE__,__LINE__)

#define REMEMBER(ptr) remember(ptr, 0, __FILE__, __LINE__)
#define FORGET(ptr) forget(ptr)
#else
#define REMEMBER(ptr)
#define FORGET(ptr)
#endif

/* 
 * Protos for the replacement functions.
 */

void*	malloc_ext(int,char*,int);
void*	realloc_ext(void*, int, char*, int);
void*	try_malloc_ext(int,char*,int);
char*	strdup_ext(const char*,char*,int);
char*	strdup_printf_ext(char*, char*, int, ...);
char*	strdup_vprintf_ext(char*, va_list, char*, int);
gchar*  g_locale_to_utf8_ext(const gchar*, gssize, gsize*, gsize*, GError**, char*, int);
void	free_ext(void*,char*,int);

void    remember(void*, int, char*, int);
int     forget(void*);

void    check_memleaks(void);

/***
 *** menubar.c
 ***/

void AttachTooltip(GtkWidget*, char*, char*);
GtkWidget* CreateMenuBar(GtkWidget*);
GtkWidget* CreateToolBar(GtkWidget*);

/***
 *** method.c / method-link.c
 ***
 * method-link.c is automatically created by the configure script.
 */

typedef struct _Method
{  char name[4];                     /* Method name tag */
   char *description;                /* Fulltext description */
   char *menuEntry;                  /* Text for use in preferences menu */
   void (*create)(struct _Method*);  /* Creates an error correction file */
   void (*fix)(struct _Method*);     /* Fixes a damaged image */
   void (*verify)(struct _Method*);  /* Verifies image with ecc data */
   void (*createVerifyWindow)(struct _Method*, GtkWidget*);
   void (*createCreateWindow)(struct _Method*, GtkWidget*);
   void (*createFixWindow)(struct _Method*, GtkWidget*);
   void (*createPrefsPage)(struct _Method*, GtkWidget*);
   void (*resetVerifyWindow)(struct _Method*);
   void (*resetCreateWindow)(struct _Method*);
   void (*resetFixWindow)(struct _Method*);
   void (*resetPrefsPage)(struct _Method*);
   void (*readPreferences)(struct _Method*);
   void (*destroy)(struct _Method*);
   int  tabWindowIndex;              /* our position in the (invisible) notebook */
   void *widgetList;                 /* linkage to window system */
   EccHeader *lastEh;                /* copy of EccHeader from last EccFileMethod() call */
} Method;

void BindMethods(void);        /* created by configure in method-link.c */

void CollectMethods(void);
void RegisterMethod(Method*);
void ListMethods(void);
Method* FindMethod(char*);
EccHeader* FindHeaderInImage(char*);
Method *EccFileMethod(int);
void CallMethodDestructors(void);

/***
 *** misc.c 
 ***/

char* sgettext(char*);
char* sgettext_utf8(char*);

gint64 uchar_to_gint64(unsigned char*);
void gint64_to_uchar(unsigned char*, gint64);

void CalcSectors(gint64, gint64*, int*);

void PrintCLI(char*, ...);
void PrintLog(char*, ...);
void PrintLogFile(char*, ...);
void Verbose(char*, ...);
void PrintTimeToLog(GTimer*, char*, ...);
void PrintProgress(char*, ...);
void PrintCLIorLabel(GtkLabel*, char*, ...);

void LogWarning(char*, ...);
void Stop(char*, ...);
void RegisterCleanup(char*, void (*)(gpointer), gpointer);

GThread* CreateGThread(GThreadFunc, gpointer);

void ShowWidget(GtkWidget*);
void AllowActions(gboolean);

void ShowMessage(GtkWindow*, char*, GtkMessageType);
GtkWidget* CreateMessage(char*, GtkMessageType, ...);
void SetLabelText(GtkLabel*, char*, ...);
void SetProgress(GtkWidget*, int, int);

int ModalDialog(GtkMessageType, GtkButtonsType, void (*)(GtkDialog*), char*, ...);
int ModalWarning(GtkMessageType, GtkButtonsType, void (*)(GtkDialog*), char*, ...);

void SetText(PangoLayout*, char*, int*, int*);
void SwitchAndSetFootline(GtkWidget*, int, GtkWidget*, char*, ...);

void ReverseCancelOK(GtkDialog*);
void TimedInsensitive(GtkWidget*, int);

/***
 *** preferences.c
 ***/

void CreatePreferencesWindow(void);
void UpdateMethodPreferences(void);
void HidePreferences(void);
void FreePreferences(void*);

/***
 *** print_sense.c
 ***/

void RememberSense(int, int, int);
char *GetSenseString(int, int, int, int);
char* GetLastSenseString(int);

/*** 
 *** read-linear.c
 ***/

void ReadMediumLinear(gpointer);

/***
 *** read-linear-window.c
 ***/

void ResetLinearReadWindow();
void CreateLinearReadWindow(GtkWidget*);

void InitializeCurve(int, gint64, gint64, gint64);
void AddCurveValues(int, double, int);

/***
 *** random.c
 ***/

#define	MY_RAND_MAX	2147483647

gint32  Random(void);
void    SRandom(gint32);
guint32 Random32(void);

/*** 
 *** read-adaptive.c
 ***/

void ReadMediumAdaptive(gpointer);

/***
 *** read-adaptive-window.c
 ***/

#define ADAPTIVE_READ_SPIRAL_SIZE 4800

void ResetAdaptiveReadWindow();
void CreateAdaptiveReadWindow(GtkWidget*);

void ClipReadAdaptiveSpiral(int);
void SetAdaptiveReadSubtitle(char*);
void SetAdaptiveReadFootline(char*, GdkColor*);
void UpdateAdaptiveResults(gint64, gint64, gint64, int);
void ChangeSegmentColor(GdkColor*, int);
void ChangeSpiralCursor(int);
void RemoveFillMarkers();

/*** 
 *** scsi-layer.c
 ***
 * Note that there is also a scsi-layer.h with more
 * scsi wrapper dependent declarations.
 */

void OpenAspi(void);
void CloseAspi(void);
void ListAspiDrives(void);

char* DefaultDevice(void);
gint64 CurrentImageSize(void);
gint64 CurrentImageCapacity(void);

int SendReadCDB(char*, unsigned char*, int, int);

/***
 *** show-manual.c
 ***/

void ShowHTML(char*);

/***
 *** spiral.c
 ***/

typedef struct _Spiral
{  GdkDrawable *drawable;
   int mx, my;
   int startRadius;
   int segmentSize;
   int segmentCount;
   double *segmentPos;
   GdkColor **segmentColor;
   GdkColor *outline;
   int diameter;
   int segmentClipping;
   int cursorPos;
   GdkColor *colorUnderCursor;
} Spiral;

Spiral* CreateSpiral(GdkColor*, GdkColor*, int, int, int);
void SetSpiralWidget(Spiral*, GtkWidget*);
void FillSpiral(Spiral*, GdkColor*);
void FreeSpiral(Spiral*);
void DrawSpiral(Spiral*);
void DrawSpiralSegment(Spiral*, GdkColor*, int);
void DrawSpiralLabel(Spiral*, PangoLayout*, char*, GdkColor*, int, int);
void MoveSpiralCursor(Spiral*, int);

/***
 *** style.c
 ***/

void AdjustStyle();

/***
 *** welcome-window.c
 ***/

void CreateWelcomePage(GtkNotebook*);


#endif				/* DVDISASTER_H */
