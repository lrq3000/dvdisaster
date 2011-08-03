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

#ifndef PNGIO_H
#define PNGIO_H

#include <zlib.h>
#include <png.h>

typedef struct
{  char *name;
   unsigned int width, height;
   unsigned int *image;
   unsigned int bytesize;
   unsigned int tile_background; 
   unsigned int png_background; 
   unsigned int channels;
   unsigned int first_opcode, last_opcode;
   unsigned char checksum[16];

  /* The rest is only used for png reading/writing */

   FILE *file; 
   png_byte   **row_pointers;
   png_struct *png_read, *png_write;
   png_info   *png_info;

} Image;

Image *LoadPNG(char*);
void SavePNG(Image*, char*);

void FreeImage(Image*);

#endif /* PNGIO_H */
