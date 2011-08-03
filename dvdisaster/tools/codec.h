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

#ifndef CODEC_H
#define CODEC_H

#define FILEFORMAT 1
#define MAX_TILE_SIZE 32

void InitTileDatabase();
void FreeTileDatabase();
void RenderImage(Image*);
void EncodeImage(Image*);
void LoadPPK(char*, Image***, int*);
void SavePPK(char*);


#endif /* CODEC_H */
