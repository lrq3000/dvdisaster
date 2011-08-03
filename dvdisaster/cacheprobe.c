/*  dvdisaster: Additional error correction for optical media.
 *  Copyright (C) 2004-2011 Carsten Gnoerlich.
 *
 *  Email: carsten@dvdisaster.org  -or-  cgnoerlich@fsfe.org
 *  Project homepage: http://www.dvdisaster.org
 *
 *  This file is part of dvdisaster.
 *
 *  dvdisaster is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  dvdisaster is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with dvdisaster. If not, see <http://www.gnu.org/licenses/>.
 */

#include "dvdisaster.h"

#ifdef SYS_LINUX
int ProbeCacheLineSize()
{  int cl_size = 0;

   cl_size = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);

   if(!cl_size)
     cl_size = sysconf(_SC_LEVEL2_CACHE_LINESIZE);

   if(cl_size < 16)
     cl_size = 64;

   return cl_size;
}
#endif

#ifdef SYS_NETBSD
#include <param.h>

int ProbeCacheLineSize()
{  int cl_size = CACHE_LINE_SIZE;

  /* NetBSD seems to have no obvious way to determine this info;
     use appropriate value for Intel as a default. */

   if(cl_size < 16)
     cl_size = 64;

printf("Cache line size: %d\n", cl_size);
   return cl_size;
}
#endif



#ifdef SYS_DARWIN

#include <sys/sysctl.h>

int ProbeCacheLineSize()
{  int mib[2], cl_size;
   size_t len;

   mib[0] = CTL_HW;
   mib[1] = HW_CACHELINE;
   len = sizeof(cl_size);

   if (sysctl(mib, 2, &cl_size, &len, NULL, 0) || (cl_size < 16))
     cl_size = 64;

printf("Cache line size: %d\n", cl_size);
   return cl_size;
}
#endif


#ifdef SYS_MINGW
int ProbeCacheLineSize()
{
  return 64;
}
#endif

#ifdef SYS_UNKNOWN
int ProbeCacheLineSize()
{
  return 64;
}
#endif


