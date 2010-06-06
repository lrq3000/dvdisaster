/*  dvdisaster: Additional error correction for optical media.
 *  Copyright (C) 2004-2010 Carsten Gnoerlich.
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
int ProbeCacheLineSize()
{  int cl_size = 0;

   cl_size = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);

   if(!cl_size)
     cl_size = sysconf(_SC_LEVEL2_CACHE_LINESIZE);

   if(cl_size < 16)
     cl_size = 64;

printf("Cache line size: %d\n", cl_size);
   return cl_size;
}
#endif

#ifdef SYS_UNKNOWN
int ProbeCacheLineSize()
{
  return 64;
}
#endif


