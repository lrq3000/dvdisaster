/*  dvdisaster: Additional error correction for optical media.
 *  Copyright (C) 2004-2006 Carsten Gnoerlich.
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

#include "galois-inlines.h"

/***
 *** Galois field arithmetic.
 *** 
 * Calculations are done over the extension field GF(2**n).
 * Be careful not to overgeneralize these arithmetics;
 * they only work for the case of GF(p**n) with p being prime.
 */

/* Initialize the Galois field tables */


GaloisTables* CreateGaloisTables(int nroots_in)
{  GaloisTables *gt = g_malloc0(sizeof(GaloisTables));
   gint32 i,j,b,log,root;

   gt->nroots = nroots_in;
   gt->ndata  = FIELDMAX - gt->nroots;

   /* Allocate the tables.
      The encoder uses a special version of alpha_to which has the mod_fieldmax()
      folded into the table. */

   gt->index_of     = g_malloc(FIELDSIZE * sizeof(gint32));
   gt->alpha_to     = g_malloc(FIELDSIZE * sizeof(gint32));
   gt->enc_alpha_to = g_malloc(2*FIELDSIZE * sizeof(gint32));
   gt->gpoly        = g_malloc((gt->nroots+1) * sizeof(gint32));
   
   /* create the log/ilog values */

   for(b=1, log=0; log<FIELDMAX; log++)
   {  gt->index_of[b]   = log;
      gt->alpha_to[log] = b;
      b = b << 1;
      if(b & FIELDSIZE)
	b = b ^ GENERATOR_POLY;
   }

   if(b!=1) Stop(_("Failed to create the log tables!\n"));

   /* we're even closed using infinity (makes things easier) */

   gt->index_of[0] = ALPHA0;    /* log(0) = inf */
   gt->alpha_to[ALPHA0] = 0;   /* and the other way around */

   for(b=0; b<2*FIELDSIZE; b++)
     gt->enc_alpha_to[b] = gt->alpha_to[mod_fieldmax(b)];

   /* Create the RS code generator polynomial */

   gt->gpoly[0] = 1;

   for(i=0, root=FIRST_ROOT*PRIM_ELEM; i<gt->nroots; i++, root+=PRIM_ELEM)
   {  gt->gpoly[i+1] = 1;

     /* Multiply gpoly  by  alpha**(root+x) */

     for(j=i; j>0; j--)
     {
       if(gt->gpoly[j] != 0)
         gt->gpoly[j] = gt->gpoly[j-1] ^ gt->alpha_to[mod_fieldmax(gt->index_of[gt->gpoly[j]] + root)];
       else
	 gt->gpoly[j] = gt->gpoly[j-1];
     }

     gt->gpoly[0] = gt->alpha_to[mod_fieldmax(gt->index_of[gt->gpoly[0]] + root)];
   }

   /* Store the polynomials index for faster encoding */ 

   for(i=0; i<=gt->nroots; i++)
     gt->gpoly[i] = gt->index_of[gt->gpoly[i]];

#if 0
   /* for the precalculated unrolled loops only */

   for(i=gt->nroots-1; i>0; i--)
     PrintCLI(
	    "                  par_idx[((++spk)&%d)] ^= enc_alpha_to[feedback + %3d];\n",
	    nroots-1,gt->gpoly[i]);

   PrintCLI("                  par_idx[sp] = enc_alpha_to[feedback + %3d];\n",
	  gt->gpoly[0]);
#endif

   return gt;
}

void FreeGaloisTables(GaloisTables *gt)
{
  if(gt->index_of)     g_free(gt->index_of);
  if(gt->alpha_to)     g_free(gt->alpha_to);
  if(gt->enc_alpha_to) g_free(gt->enc_alpha_to);
  if(gt->gpoly)        g_free(gt->gpoly);

  g_free(gt);
}

