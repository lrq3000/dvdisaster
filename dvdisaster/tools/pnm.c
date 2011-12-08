/*  pngpack: lossless image compression for a series of screen shots
 *  Copyright (C) 2005-2011 Carsten Gnoerlich.
 *
 *  Email: carsten@dvdisaster.org  -or-  cgnoerlich@fsfe.org
 *  Project homepage: http://www.dvdisaster.org
 *
 *  This file is part of pngpack.
 *
 *  pngpack is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  pngpack is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with pngpack. If not, see <http://www.gnu.org/licenses/>.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "pngio.h"
#include "md5.h"
#include "memory.h"

#include "codec.h"

/***
 *** for debugging purposes
 ***/

/*
	    if(!strcmp(pi->name, "write-iso2.png"))
	       SavePNM(pi, "dump.pnm");
*/

void SavePNM(Image *image, char *name)
{  FILE *file;
   unsigned int i, *p;

   if(!(file = fopen(name, "w")))
      Stop("Can not open pnm file %s: %s\n", name, strerror(errno));

   fprintf(file,"P6\n"
	        "# CREATOR: pngpack\n"
	        "%d %d\n"
	        "255\n",
	   image->width, image->height);

   i = image->width * image->height;
   p = image->image;

   while(i--)
   {  fputc((*p>>16)&0xff, file);
      fputc((*p>> 8)&0xff, file);
      fputc((*p    )&0xff, file);
      p++;
   }
   fclose(file);
}


/***
 *** Save a tile
 ***/
   
typedef struct
{  unsigned int image[MAX_TILE_SIZE][MAX_TILE_SIZE];
   unsigned int width, height;
   unsigned int background;
} tile;

void SaveTile(tile *tile, char *name)
{  FILE *file;
   unsigned int i, j;

   if(!(file = fopen(name, "w")))
      Stop("Can not open pnm file %s: %s\n", name, strerror(errno));

   fprintf(file,"P6\n"
	        "# CREATOR: pngpack\n"
	        "%d %d\n"
	        "255\n",
	   tile->width, tile->height);


   for(j=0; j<tile->height; j++)
      for(i=0; i<tile->width; i++)
      {  fputc((tile->image[i][j]>>16)&0xff, file);
	 fputc((tile->image[i][j]>> 8)&0xff, file);
	 fputc((tile->image[i][j]    )&0xff, file);
      }
   fclose(file);
}
