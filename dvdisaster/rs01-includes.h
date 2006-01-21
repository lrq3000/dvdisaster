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

#ifndef RS01INCLUDES_H
#define RS01INCLUDES_H

/* Data structs from rs01-window.c */

typedef struct
{
   /*** Widgets for RS01 encoding */

   GtkWidget *encHeadline;
   GtkWidget *encLabel1;
   GtkWidget *encPBar1;
   GtkWidget *encLabel2;
   GtkWidget *encPBar2;
   GtkWidget *encFootline;
   GtkWidget *encFootline2;
   GtkWidget *curveButton;

   /*** Widgets for RS01 fixing */

   GtkWidget *fixHeadline;
   GtkWidget *fixDrawingArea;
   GtkWidget *fixNotebook;
   GtkWidget *fixFootline;
   GtkWidget *fixFootlineBox;
   GtkWidget *fixCorrected;
   GtkWidget *fixProgress;
   GtkWidget *fixUncorrected;
   Curve  *fixCurve;

   /*** Widgets for RS01 compare action */

   GtkWidget *cmpHeadline;

   GtkWidget *cmpImageNotebook;
   GtkWidget *cmpImageSectors;
   GtkWidget *cmpChkSumErrors;
   GtkWidget *cmpMissingSectors;
   GtkWidget *cmpImageMd5Sum;
   GtkWidget *cmpImageResult;

   GtkWidget *cmpEccNotebook;
   GtkWidget *cmpEccEmptyMsg;
   GtkWidget *cmpEccCreatedBy;
   GtkWidget *cmpEccMethod;
   GtkWidget *cmpEccRequires;
   GtkWidget *cmpEccMediumSectors;
   GtkWidget *cmpEccImgMd5Sum;
   GtkWidget *cmpEccFingerprint;
   GtkWidget *cmpEccBlocks;
   GtkWidget *cmpEccMd5Sum;
   GtkWidget *cmpEccResult;

   GtkWidget *cmpDrawingArea;
   Spiral *cmpSpiral;
   PangoLayout *cmpLayout;

   /*** Widgets in the Preferences window */

   GtkWidget *radio1,*radio2,*radio3,*radio4;
   GtkWidget *radio4Label;
   GtkWidget *redundancyScale;
   GtkWidget *redundancySpin;

   /*** Some state vars used during fixing */

   gint64 corrected;
   gint64 uncorrected;
   gint64 nSectors;
   int    eccBytes;
   int    dataBytes;
   int    percent, lastPercent;
} RS01Widgets;

/* 
 * These are exported via the Method struct 
 */

void CreateRS01EWindow(Method*, GtkWidget*);
void CreateRS01FWindow(Method*, GtkWidget*);
void CreateRS01PrefsPage(Method*, GtkWidget*);
void ResetRS01EncodeWindow(Method*);
void ResetRS01FixWindow(Method*);

void RS01ShowCurveButton(Method*);

void ResetRS01CompareWindow(Method*);
void CreateRS01CompareWindow(Method*, GtkWidget*);

/*
 * These are exported (resp. only used) in ecc-rs01.c and rs01*.c
 * and should not be called from somewhere else as we can not
 * rely on the method plug-in being available.
 * If you need similar functions in your own codec,
 * please copy these functions over to the respective plug-in.
 */

/* rs01-common.c */

void RS01ReadSector(ImageInfo*, EccHeader*, unsigned char*, gint64);
void RS01ScanImage(Method*, ImageInfo*, EccInfo*, int);

/* rs01-compare.c */

#define COMPARE_IMAGE_SEGMENTS 1000

void RS01Compare(Method*);
void RS01AddCompareValues(Method*, int, gint64, gint64, gint64, gint64);

/* rs01-create.c */

void RS01Create(Method*);

/* rs01-fix.c */

void RS01Fix(Method*);

/* rs01-window.c */

void AddFixValues(RS01Widgets*, int, int);
void SetFixMaxValues(RS01Widgets*, int, int, gint64);
void UpdateFixResults(RS01Widgets*, gint64, gint64);

#endif
