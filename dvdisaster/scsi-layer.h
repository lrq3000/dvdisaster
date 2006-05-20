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

#ifndef SCSI_LAYER_H
#define SCSI_LAYER_H

#ifdef SYS_LINUX
#include <sys/ioctl.h>
#include <linux/cdrom.h>
#endif

#ifdef SYS_MINGW
#include <windows.h>
#include <winioctl.h>
#endif

#ifdef SYS_FREEBSD
#include <camlib.h>
#endif

#ifdef SYS_DARWIN
#define REAL_VERSION VERSION
#undef VERSION
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/scsi-commands/SCSITaskLib.h>
#include <IOKit/storage/IODVDTypes.h>
#include <mach/mach.h>
#include <string.h>
#include <stdlib.h>
#define VERSION REAL_VERSION
#endif

/***
 *** Define the Sense data structure.
 ***/

/* 
 * Linux already has one 
 */

#ifdef SYS_LINUX
#define MAX_CDB_SIZE CDROM_PACKET_SIZE

typedef struct request_sense Sense;
#endif

#ifdef SYS_FREEBSD
#define MAX_CDB_SIZE SCSI_MAX_CDBLEN

//typedef struct scsi_sense_data Sense;

/* This is actually the little endian version of the
 * Linux Sense struct. FreeBSD does not have the asb entry.
 */

typedef struct request_sense {
	guint8 error_code		: 7;
	guint8 valid			: 1;
	guint8 segment_number;
	guint8 sense_key		: 4;
	guint8 reserved2		: 1;
	guint8 ili			: 1;
	guint8 reserved1		: 2;
	guint8 information[4];
	guint8 add_sense_len;
	guint8 command_info[4];
	guint8 asc;
	guint8 ascq;
	guint8 fruc;
	guint8 sks[3];
	guint8 asb[46];
} Sense;
#endif

#ifdef SYS_MINGW
#define MAX_CDB_SIZE 16   /* longest possible SCSI command */

/* This is actually the little endian version of the
 * Linux Sense struct. Windows uses only the first 24 bytes.
 */

typedef struct request_sense {
	guint8 error_code		: 7;
	guint8 valid			: 1;
	guint8 segment_number;
	guint8 sense_key		: 4;
	guint8 reserved2		: 1;
	guint8 ili			: 1;
	guint8 reserved1		: 2;
	guint8 information[4];
	guint8 add_sense_len;
	guint8 command_info[4];
	guint8 asc;
	guint8 ascq;
	guint8 fruc;
	guint8 sks[3];
	guint8 asb[46];
} Sense;
#endif

#ifdef SYS_UNKNOWN
#define MAX_CDB_SIZE 16   /* longest possible SCSI command */

/* This is actually the little endian version of the
 * Linux Sense struct. 
 */

typedef struct request_sense {
	guint8 error_code		: 7;
	guint8 valid			: 1;
	guint8 segment_number;
	guint8 sense_key		: 4;
	guint8 reserved2		: 1;
	guint8 ili			: 1;
	guint8 reserved1		: 2;
	guint8 information[4];
	guint8 add_sense_len;
	guint8 command_info[4];
	guint8 asc;
	guint8 ascq;
	guint8 fruc;
	guint8 sks[3];
	guint8 asb[46];
} Sense;
#endif

#ifdef SYS_DARWIN
#define MAX_CDB_SIZE 16   /* longest possible SCSI command */

/* This is actually the little endian version of the
 * Linux Sense struct. 
 */

typedef struct request_sense {
	guint8 error_code		: 7;
	guint8 valid			: 1;
	guint8 segment_number;
	guint8 sense_key		: 4;
	guint8 reserved2		: 1;
	guint8 ili			: 1;
	guint8 reserved1		: 2;
	guint8 information[4];
	guint8 add_sense_len;
	guint8 command_info[4];
	guint8 asc;
	guint8 ascq;
	guint8 fruc;
	guint8 sks[3];
	guint8 asb[46];
} Sense;
#endif

/***
 ***  The DeviceHandle is pretty much our device abstraction layer. 
 ***
 * It contains info about the opened device and the inserted medium
 */

typedef struct _DeviceHandle
{
#ifdef SYS_LINUX
   int fd;                    /* device file descriptor */
#endif
#ifdef SYS_FREEBSD
   struct cam_device *camdev; /* camlib device handle */
   union ccb *ccb;
#endif
#ifdef SYS_MINGW
   HANDLE fd;                 /* Windows file handle for the device (SPTI case) */
   int aspiUsed;	      /* TRUE is device is accessed via ASPI */
   int ha,target,lun;         /* ASPI way of describing drives */ 
#endif
#ifdef SYS_DARWIN
   IOCFPlugInInterface **plugInInterface;
   MMCDeviceInterface **mmcDeviceInterface;
   SCSITaskDeviceInterface **scsiTaskDeviceInterface;
   SCSITaskInterface **taskInterface;
   IOVirtualRange *range;
#endif
   char *device;
   char devinfo[34];          /* whole device info string */
   char vendor[34];           /* vendor and product info only */
   int category;
   int (*read)(struct _DeviceHandle*, unsigned char*, int, int);
   Sense sense;               /* sense data from last operation */
   gint64 userAreaSize;       /* size of user area according to DVD Info struct */
   gint64 rs02Size;           /* size reported in RS02 header */
   int sessions;
   int layers;
   int mainType;
   int subType;
   char *typedescr;
   gint64 sectors;            /* Number of sectors */
   double singleRate;         /* supposed KB/sec @ single speed */
   int maxRate;               /* guessed maximum transfer rate */
   char *mediumDescr;         /* textual description of medium */
   struct _IsoInfo *isoInfo;  /* Information gathered from ISO filesystem */
} DeviceHandle;

/* These seem not to be standardized anywhere,
 * so we make up our own enum here.
 */

enum { DATA1, XA21, CD, DVD, UNSUPPORTED };

#define DATA_WRITE 0
#define DATA_READ  1
#define DATA_NONE  2

/***
 *** Exported functions
 ***/

/*
 * OS-dependent wrappers from scsi-<os>.c
 */

DeviceHandle* OpenDevice(char*);

#ifdef SYS_MINGW
DeviceHandle* open_aspi_device(char*, int);
DeviceHandle* open_spti_device(char*);
#endif

int SendPacket(DeviceHandle*, unsigned char*, int, unsigned char*, int, Sense*, int);

/*** 
 *** scsi-layer.c
 ***
 * The really user-visible stuff
 */

DeviceHandle* OpenAndQueryDevice(char*);
int InquireDevice(DeviceHandle*, int); 
void SpinupDevice(DeviceHandle*);
int ReadSectors(DeviceHandle*, unsigned char*, gint64, int);
void CloseDevice(DeviceHandle*);

#endif /* SCSI_LAYER_H */
