/*  dvdisaster: Additional error correction for optical media.
 *  Copyright (C) 2004-2008 Carsten Gnoerlich.
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

#include "scsi-layer.h"
#include "udf.h"

/***
 *** The Linux SCSI wrapper. Uses the uniform CDROM driver. Kernel 2.6.x recommended. 
 ***/

#ifdef SYS_LINUX
#include <linux/param.h>

char* DefaultDevice()
{  DeviceHandle *dh;
   GDir *dir;
   const char* dev;
   int dev_type;

   dir = g_dir_open("/dev", 0, NULL);

   if(!dir)
   {  PrintLog(_("Can not access /dev for devices\n"
		  "No drives will be pre-selected.\n"));

      return g_strdup("/dev/cdrom");
   }

   dh = g_malloc(sizeof(DeviceHandle));

   while((dev = g_dir_read_name(dir)))
   {  
     if(   (strlen(dev) == 3 && (!strncmp(dev,"hd",2) || !strncmp(dev,"sd",2) || !strncmp(dev,"sr", 2)))
	|| (strlen(dev) == 4 && !strncmp(dev,"scd",3)) )
     { char buf[80];

       sprintf(buf,"/dev/%s", dev); 

       memset(dh, 0, sizeof(DeviceHandle));
       dh->fd = open(buf, O_RDONLY | O_NONBLOCK);
       dh->device = buf;

       if(dh->fd < 0)   /* device not even present */
	 continue;

       dev_type = InquireDevice(dh, 1);
       close(dh->fd);

       if(dev_type != 5)  /* not a CD/DVD ROM */
	 continue;

       g_ptr_array_add(Closure->deviceNodes, g_strdup(buf));

       sprintf(buf, "%s (/dev/%s)", dh->devinfo, dev);
       g_ptr_array_add(Closure->deviceNames, g_strdup(buf));
     }
   }

   g_dir_close(dir);
   g_free(dh);

   if(Closure->deviceNodes->len)
     return g_strdup(g_ptr_array_index(Closure->deviceNodes, 0));
   else
   {  PrintLog(_("No CD/DVD drives found in /dev.\n"
		  "No drives will be pre-selected.\n"));

      return g_strdup("/dev/cdrom");
   }
}

DeviceHandle* OpenDevice(char *device)
{  DeviceHandle *dh; 

   dh = g_malloc0(sizeof(DeviceHandle));
   dh->fd = open(device, O_RDONLY | O_NONBLOCK);

   if(dh->fd < 0)
   {  g_free(dh);
      Stop(_("Could not open %s: %s"),device, strerror(errno));
      return NULL;
   }

   dh->device = g_strdup(device);

   return dh;
}

void CloseDevice(DeviceHandle *dh)
{ 
  if(dh->canReadDefective)
    SetRawMode(dh, MODE_PAGE_UNSET);

  if(dh->rawBuffer)
     FreeRawBuffer(dh->rawBuffer);

  if(dh->fd)
    close(dh->fd);
  if(dh->device)
    g_free(dh->device);
  if(dh->rs02Header)
    g_free(dh->rs02Header);
  if(dh->typeDescr) 
    g_free(dh->typeDescr);
  if(dh->mediumDescr) 
    g_free(dh->mediumDescr);
  if(dh->isoInfo)
    FreeIsoInfo(dh->isoInfo);
  if(dh->defects)
    FreeBitmap(dh->defects);
  g_free(dh);
}

int SendPacket(DeviceHandle *dh, unsigned char *cmd, int cdb_size, unsigned char *buf, int size, Sense *sense, int data_mode)
{  struct cdrom_generic_command cgc;

   memset(&cgc, 0, sizeof(cgc));

   memcpy(cgc.cmd, cmd, MAX_CDB_SIZE);  /* Linux ignores the CDB size */
   cgc.buffer = buf;
   cgc.buflen = size;
   cgc.sense  = (struct request_sense*)sense;
   cgc.timeout = 10*60*HZ;   /* 10 minutes; a timeout hangs newer kernels  */

   switch(data_mode)
   {  case DATA_READ:
        cgc.data_direction = CGC_DATA_READ; 
	break;
      case DATA_WRITE:
        cgc.data_direction = CGC_DATA_WRITE; 
	break;
      default:
	Stop("illegal data_mode: %d", data_mode);
   }

   return ioctl(dh->fd, CDROM_SEND_PACKET, &cgc);
}
#endif /* SYS_LINUX */
