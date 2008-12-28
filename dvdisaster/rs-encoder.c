/*  dvdisaster: Additional error correction for optical media.
 *  Copyright (C) 2004-2009 Carsten Gnoerlich.
 *  Project home page: http://www.dvdisaster.com
 *  Email: carsten@dvdisaster.com  -or-  cgnoerlich@fsfe.org
 *
 *  The Reed-Solomon error correction draws a lot of inspiration - and even code -
 *  from Phil Karn's excellent Reed-Solomon library: http://www.ka9q.net/code/fec/
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

/***
 *** Reed-Solomon encoding
 ***/

/*
 * Encode next layer of data into RS buffer
 */ 

void EncodeNextLayer(ReedSolomonTables *rt, unsigned char *data, unsigned char *parity, guint64 layer_size, int shift)
{  gint32 *gf_index_of  = rt->gfTables->indexOf;
   gint32 *enc_alpha_to = rt->gfTables->encAlphaTo;
   gint32 *rs_gpoly     = rt->gpoly;
   int nroots       = rt->nroots;
   int lut_size32   = nroots>>2;
   int byte_rest    = nroots & 3;
   int i;

   /* Use a clipmask if the number of roots is not a 4-byte multiple */

   for(i=0; i<layer_size; i++)
   {  register int feedback = gf_index_of[data[i] ^ parity[shift]];
      int offset   = nroots-shift-1;
      int byte_offset = offset&3;

      if(feedback != GF_ALPHA0) /* non-zero feedback term */
      {	 register guint32 *par_idx = (guint32*)parity;

	 /* Pick lookup table with right 32bit alignment. */

	 register guint32 *e_lut = ((guint32*)rt->eLut[byte_offset][feedback])+(offset>>2);

	 /* Process lut in 32 bit steps */

	 switch(lut_size32)
	 {  
	     case 44: *par_idx++ ^= *e_lut++;
	     case 43: *par_idx++ ^= *e_lut++;
	     case 42: *par_idx++ ^= *e_lut++;
	     case 41: *par_idx++ ^= *e_lut++;
	     case 40: *par_idx++ ^= *e_lut++;
	     case 39: *par_idx++ ^= *e_lut++;
	     case 38: *par_idx++ ^= *e_lut++;
	     case 37: *par_idx++ ^= *e_lut++;
	     case 36: *par_idx++ ^= *e_lut++;
	     case 35: *par_idx++ ^= *e_lut++;
	     case 34: *par_idx++ ^= *e_lut++;
	     case 33: *par_idx++ ^= *e_lut++;
	     case 32: *par_idx++ ^= *e_lut++;
	     case 31: *par_idx++ ^= *e_lut++;
	     case 30: *par_idx++ ^= *e_lut++;
	     case 29: *par_idx++ ^= *e_lut++;
	     case 28: *par_idx++ ^= *e_lut++;
	     case 27: *par_idx++ ^= *e_lut++;
	     case 26: *par_idx++ ^= *e_lut++;
	     case 25: *par_idx++ ^= *e_lut++;
	     case 24: *par_idx++ ^= *e_lut++;
	     case 23: *par_idx++ ^= *e_lut++;
	     case 22: *par_idx++ ^= *e_lut++;
	     case 21: *par_idx++ ^= *e_lut++;
	     case 20: *par_idx++ ^= *e_lut++;
	     case 19: *par_idx++ ^= *e_lut++;
	     case 18: *par_idx++ ^= *e_lut++;
	     case 17: *par_idx++ ^= *e_lut++;
	     case 16: *par_idx++ ^= *e_lut++;
	     case 15: *par_idx++ ^= *e_lut++;
	     case 14: *par_idx++ ^= *e_lut++;
	     case 13: *par_idx++ ^= *e_lut++;
	     case 12: *par_idx++ ^= *e_lut++;
	     case 11: *par_idx++ ^= *e_lut++;
	     case 10: *par_idx++ ^= *e_lut++;
	     case  9: *par_idx++ ^= *e_lut++;
	     case  8: *par_idx++ ^= *e_lut++;
	     case  7: *par_idx++ ^= *e_lut++;
	     case  6: *par_idx++ ^= *e_lut++;
	     case  5: *par_idx++ ^= *e_lut++;
	     case  4: *par_idx++ ^= *e_lut++;
	     case  3: *par_idx++ ^= *e_lut++;
	     case  2: *par_idx++ ^= *e_lut++;
	     case  1: *par_idx++ ^= *e_lut++;
	 }

	 /* Work the remaining bytes of the lookup table into the parity.
	    Note that we can not operate on word width here and mask off
	    the unneeded bytes as that would cause a race condition when
	    encoding with multiple threads back-to-front within the same
	    buffer (as it happens in RS03) */

	 if(byte_rest)
	 {  guint32 partial = *par_idx ^ *e_lut;
	    int i;
	    
	    for(i=0; i<byte_rest; i++)
	       ((guint8*)par_idx)[i] = ((guint8*)&partial)[i]; 

	    par_idx++; e_lut++;
	 }

	 parity[shift] = enc_alpha_to[feedback + rs_gpoly[0]];
       }
       else  /* zero feedback term */
	   parity[shift] = 0;

       parity += nroots;
   }
}

