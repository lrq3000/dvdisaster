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

#include "dvdisaster.h"

#include "rs02-includes.h"

/***
 *** Local data package used during encoding
 ***/

typedef struct
{  Method *self;
   RS02Widgets *wl;
   RS02Layout *lay;
   GaloisTables *gt;
   ImageInfo *ii;
   EccHeader *eh;
   unsigned char *data;
   unsigned char *parity;
   unsigned char *slice[256];
   struct MD5Context md5Ctxt[256];
   guint8 md5Sum[16*256];
   guint8 eccSum[16];
   char *msg;
   int earlyTermination;
   GTimer *timer;
} ecc_closure;

static void ecc_cleanup(gpointer data)
{  ecc_closure *ec = (ecc_closure*)data;
   int i;

   Closure->cleanupProc = NULL;

   if(Closure->guiMode)
   {  if(ec->earlyTermination)
        SetLabelText(GTK_LABEL(ec->wl->encFootline),
		     _("<span color=\"red\">Aborted by unrecoverable error.</span>")); 
      AllowActions(TRUE);
   }

   /** Clean up */

   if(ec->gt) FreeGaloisTables(ec->gt);
   if(ec->ii) FreeImageInfo(ec->ii);
   if(ec->eh) g_free(ec->eh);
   if(ec->lay) g_free(ec->lay);
   if(ec->data) g_free(ec->data);
   if(ec->parity) g_free(ec->parity);
   if(ec->msg) g_free(ec->msg);
   if(ec->timer) g_timer_destroy(ec->timer);

   for(i=0; i<256; i++)
     if(ec->slice[i])
       g_free(ec->slice[i]);

   if(Closure->enableCurveSwitch)
   {  Closure->enableCurveSwitch = FALSE;
     //      RS01ShowCurveButton(ec->self);
   }

   g_free(ec);

   g_thread_exit(0);
}

/***
 *** Some sub tasks to be done during encoding
 ***/


/*
 * Remove already existing RS02 ecc data from the image.
 */

static void remove_old_ecc(ecc_closure *ec)
{  EccHeader *old_eh;

   old_eh = FindHeaderInImage(Closure->imageName);

   if(old_eh)
   {  gint64 data_sectors = uchar_to_gint64(old_eh->sectors);
      LargeFile *tmp;

      PrintLog(_("* Image \"%s\" already contains error correction information.\n"
		 "* Truncating image to data part (%lld sectors).\n"),
	       Closure->imageName, data_sectors);

      tmp = LargeOpen(Closure->imageName, O_RDWR, IMG_PERMS);
      if(!tmp || !LargeTruncate(tmp, (gint64)(2048*data_sectors)))
	Stop(_("Could not truncate %s: %s\n"),Closure->imageName,strerror(errno));
      LargeClose(tmp);
   }
}

/*
 * Check the image for completeness and calculate the CRC sums
 * if the respective data has not already been supplied by ReadLinear() 
*/

static void check_image(ecc_closure *ec)
{  struct MD5Context image_md5;
   RS02Layout *lay = ec->lay;
   ImageInfo *ii = ec->ii;
   gint64 sectors;
   int last_percent, percent;

   if(!Closure->crcCache)
   {  guint32 *crcptr;

      last_percent = 0;
      MD5Init(&image_md5);
   
      Closure->crcCache = crcptr = g_malloc(sizeof(guint32) * lay->dataSectors);

      if(!LargeSeek(ii->file, 0))
	Stop(_("Failed seeking to start of image: %s\n"), strerror(errno));

      for(sectors = 0; sectors < lay->dataSectors; sectors++)
      {  unsigned char buf[2048];
	 int n;

	 n = LargeRead(ii->file, buf, 2048);
	 if(n != 2048)
	   Stop(_("Failed reading sector %lld in image: %s"),sectors,strerror(errno));

	 if(!memcmp(buf, Closure->deadSector, n))
	   Stop(_("Image contains unread(able) sectors.\n"
		  "Error correction information can only be\n"
	       "appended to complete (undamaged) images.\n"));

	 *crcptr++ = Crc32(buf, 2048);
	 MD5Update(&image_md5, buf, n);

	 percent = (100*sectors)/lay->dataSectors;

	 if(last_percent != percent) 
	 {  PrintProgress(_("Calculating sector checksums: %3d%%") ,percent);
	   
	    last_percent = percent;
	 }
      }

      MD5Final(ii->mediumSum, &image_md5);

      PrintProgress(_("Calculating sector checksums: %3d%%"), 100);
      PrintProgress("\n");
   }
   else
     Stop("fill in stuff from Closure->md5Cache etc.\n");
}


/*
 * Expand the image by lay->eccSectors.
 * This avoids horrible file fragmentation under some file systems. 
 */

static void expand_image(ecc_closure *ec)
{  RS02Layout *lay = ec->lay;
   ImageInfo *ii = ec->ii;
   int last_percent, percent;
   gint64 sectors;

   if(!LargeSeek(ii->file, 2048*lay->dataSectors))
     Stop(_("Failed seeking to end of image: %s\n"), strerror(errno));

   last_percent = 0;
   for(sectors = 0; sectors < lay->eccSectors; sectors++)
   {  int n;

      n = LargeWrite(ii->file, Closure->deadSector, 2048);
      if(n != 2048)
	Stop(_("Failed expanding the image: %s\n"), strerror(errno));

      percent = (100*sectors) / lay->eccSectors;
      if(last_percent != percent)
      {  if(Closure->guiMode)
	  ;
	 else PrintProgress(_("Expanding image: %3d%%"), percent);
	 last_percent = percent; 
      }
   }

   if(Closure->guiMode)
     ;
   else 
   {  PrintProgress(_("Expanding image: %3d%%"), 100);
      PrintProgress("\n");
   }
}

/*
 * Write the RS02 CRC32 sums into the image file 
 */

static void write_crc(ecc_closure *ec)
{  RS02Layout *lay = ec->lay;
   ImageInfo *ii = ec->ii;
   EccHeader *eh = ec->eh;
   gint64 crc_sector;
   gint64 layer_sector;
   gint64 layer_offset;
   guint32 crc_buf[512], *crc_boot_ptr;
   struct MD5Context md5ctxt;
   int crc_idx,i;
   int writepos=0;
   layer_offset = lay->firstCrcLayerIndex + 1;
   crc_sector   = lay->dataSectors + 2;
   crc_idx = 0;

   /*** A copy of the CRCs for the lay->firstCrcLayerIndex ecc block
	is copied into the EccHeader starting with byte position 2048. */

   crc_boot_ptr = (guint32*)((char*)eh + 2048);
   MD5Init(&md5ctxt);

   /*** Calculate the CRCs */

   if(!LargeSeek(ii->file, 2048*crc_sector))
     Stop(_("Failed seeking to sector %lld in image: %s"), crc_sector, strerror(errno));

   for(layer_sector=0; layer_sector<lay->sectorsPerLayer; layer_sector++)
   {  gint64 layer_index = (layer_sector + layer_offset) % lay->sectorsPerLayer;

      /* Write CRC sums for layer_index'th slice.
         Some ecc blocks contain padding sectors >= lay->dataSectors. 
         CRCs for padding sectors are not written out,
         so we have to keep in mind that there might be <= ndata CRC sums
         per ecc blocks. */

      for(i=0; i<lay->ndata; i++)
      {  
	 if(layer_index < lay->dataSectors)
	 {  crc_buf[crc_idx++] = Closure->crcCache[layer_index];
 
	    if(layer_sector == lay->sectorsPerLayer - 1)
	      *crc_boot_ptr++ = Closure->crcCache[layer_index];
#if 0
     printf("layer sector %lld, layer %3d: sector %lld, byte %8x, writepos %d\n",
	    layer_index - i*lay->sectorsPerLayer, i,
	    layer_index, crc_buf[crc_idx-1], writepos);
#endif

            if(crc_idx >= 512)
	    {  int n = LargeWrite(ii->file, crc_buf, 2048);

	       if(n != 2048)
		 Stop(_("Failed writing to sector %lld in image: %s"), crc_sector, strerror(errno));
	       MD5Update(&md5ctxt, (unsigned char*)crc_buf, n);

	       crc_sector++;
	       crc_idx = 0;
	    }
	    writepos++;
	    layer_index += lay->sectorsPerLayer;
	 }
      }
   }
   //   printf("%d CRC sums written.\n",writepos);

   /* flush last CRC sector */

   if(crc_idx)
   {  int n; 

      for(n=crc_idx; n<512; n++) /* pad unused portion of CRC buffer */
	crc_buf[n] = 0x4c5047;

      n = LargeWrite(ii->file, crc_buf, 2048);

      if(n != 2048)
	Stop(_("Failed writing to sector %lld in image: %s"), crc_sector, strerror(errno));

      MD5Update(&md5ctxt, (unsigned char*)crc_buf, n);
   }

   /* finish and store the md5sum */

   MD5Final(eh->crcSum, &md5ctxt);
}

/*
 * Fill in the necessary values for the EccHeader.
 * Note that a copy of the CRC sums for ecc block lay->firstCrcLayerIndex + 1
 * has been put at byte pos 2048 into the Eccheader by the previous function.
 */

static void prepare_header(ecc_closure *ec)
{  ImageInfo *ii = ec->ii;
   EccHeader *eh = ec->eh;
   RS02Layout *lay = ec->lay;

   memcpy(eh->cookie, "*dvdisaster*", 12);
   memcpy(eh->method, "RS02", 4);
   eh->methodFlags[0]  = 0;
   eh->methodFlags[3]  = Closure->releaseFlags;
   memcpy(eh->mediumFP, ii->mediumFP, 16);
   memcpy(eh->mediumSum, ii->mediumSum, 16);
   memcpy(eh->eccSum, ec->eccSum, 16);
   gint64_to_uchar(eh->sectors, ii->sectors);
   eh->dataBytes       = lay->ndata;
   eh->eccBytes        = lay->nroots;

   eh->creatorVersion  = Closure->version;
   eh->neededVersion   = 6600;
   eh->fpSector        = FOOTPRINT_SECTOR;

   eh->selfCRC = 0x4c5047;
   eh->selfCRC = Crc32((unsigned char*)eh, sizeof(EccHeader));
}

/*
 * Calculate the Reed-Solomon error correction code
 */

static void create_reed_solomon(ecc_closure *ec)
{  RS02Layout *lay = ec->lay;
   LargeFile *file = ec->ii->file;
   int nroots = lay->nroots;
   int ndata  = lay->ndata;
   gint64 b_idx, block_idx[256]; 
   int n_parity_blocks,n_layer_sectors;
   int n_parity_bytes,n_layer_bytes;
   int last_percent, percent, max_percent, progress;
   int layer,chunk,i,j,k;
   unsigned char *par_ptr;
static gint32 *gf_index_of;    /* These need to be static globals */
static gint32 *gf_gpoly;       /* for optimization reasons. */
static gint32 *enc_alpha_to;

#if 0
 int j,secs[100000];
 gint64 cnt=0,x,hpos;
 for(i=0; i<100000; i++) secs[i] = 0;

 for(j=0; j<lay->sectorsPerLayer; j++)
   for(i=0; i<lay->nroots; i++)  /* layers */
   {  x = RS02EccSectorIndex(lay, i, j);
      secs[x]++;
      cnt++;
   }

 for(i=lay->protectedSectors; i<lay->eccSectors+lay->dataSectors; i++)
   if(secs[i] != 1)
     printf("Sector %d: %d\n", i, secs[i]);
 printf("%lld secs total written\n", cnt);

   hpos = (lay->protectedSectors + lay->headerModulo - 1) / lay->headerModulo;
   hpos *= lay->headerModulo;
   while(hpos < lay->dataSectors+lay->eccSectors)
     {  printf("Ecc header: %lld\n", hpos);
       hpos += lay->headerModulo;
     }
#endif


   /*** Create table for Galois field math */

   ec->gt = CreateGaloisTables(nroots);

   gf_index_of  = ec->gt->index_of;
   enc_alpha_to = ec->gt->enc_alpha_to;
   gf_gpoly     = ec->gt->gpoly;

   /*** Allocate buffers for the parity calculation and image data caching. 

        The algorithm builds the parity file consecutively in chunks of n_parity_blocks.
        We use all the amount of memory allowed by cacheMB for caching the parity blocks. */

   n_parity_blocks = (Closure->cacheMB<<20) / nroots;  /* 1 MB = 2^20 */
   n_parity_blocks >>= 1;                              /* two buffer sets for scrambling */
   n_parity_blocks &= ~0x7ff;                          /* round down to multiple of 2048 */
   n_parity_bytes  = nroots * n_parity_blocks;

   /* Each chunk of parity blocks is built iteratively by processing the data in layers
      (first all bytes at pos 0, then pos 1, until ndata layers have been processed).
      So we need to buffer n_layer_bytes = n_parity_blocks of input data.
      For practical reasons we require that the layer size is a multiple of the
      medium sector size of 2048 bytes. */

   n_layer_bytes   = n_parity_blocks;
   n_layer_sectors = n_parity_blocks/2048;

   if(n_layer_sectors*2048 != n_parity_blocks)
     Stop(_("Internal error: parity blocks are not a multiple of sector size.\n"));

   ec->parity = g_malloc(n_parity_bytes);
   ec->data   = g_malloc(n_layer_bytes);

   /*** Setup the block counters for mapping medium sectors to ecc blocks 
        The image is divided into ndata layers;
        with each layer spanning s lay->sectorsPerLayer sectors. */

   for(b_idx=0, i=0; i<ndata; b_idx+=lay->sectorsPerLayer, i++)
     block_idx[i] = b_idx;

   /*** Create buffers for dividing the ecc information into nroots slices */

   for(i=0; i<nroots; i++)
     ec->slice[i] = g_malloc(n_layer_bytes);

   /*** Initialize md5 contexts for checksumming the nroots slices */

   for(i=0; i<nroots; i++)
      MD5Init(&ec->md5Ctxt[i]);

   /*** Create ecc information for the protected sectors portion of the image. */ 

   max_percent = ndata * ((lay->sectorsPerLayer / n_layer_sectors) + 1);
   progress = percent = 0;
   last_percent = -1;
   g_timer_start(ec->timer);

   /* Process the image.
      From each layer a chunk of n_layer_sectors is read in at once.
      So after (lay->sectorsPerLayer/n_layer_sectors)+1 iterations 
      the whole image has been processed. */

   for(chunk=0; chunk<lay->sectorsPerLayer; chunk+=n_layer_sectors) 
   {  int actual_layer_bytes,actual_layer_sectors;
      int sp;

      /* Prepare the parity data for the next chunk. */

      memset(ec->parity, 0, n_parity_bytes);

      /* The last chunk may contain fewer sectors. */

      if(chunk+n_layer_sectors < lay->sectorsPerLayer)
           actual_layer_sectors = n_layer_sectors;
      else actual_layer_sectors = lay->sectorsPerLayer-chunk;

      actual_layer_bytes   = 2048*actual_layer_sectors;

      /* Work each of the ndata data layers 
	 into the parity data of the current chunk. */

      sp = nroots - ndata % nroots;  /* => (ndata + sp) mod nroots = 0 so that parity */
	                             /* is aligned at sp=0 after ndata iterations */
      if(sp==nroots) sp=0;

      for(layer=0; layer<ndata; layer++)
      {  int offset = 0;
         unsigned char *par_idx = ec->parity;

#if 0
	    if(Closure->stopActions) /* User hit the Stop button */
	    {  SetLabelText(GTK_LABEL(wl->encFootline), 
			    _("<span color=\"red\">Aborted by user request!</span>")); 
	       ec->earlyTermination = FALSE;   /* suppress respective error message */
	       LargeClose(ei->file);
	       ei->file = NULL;
	       LargeUnlink(Closure->eccName);  /* Do not leave partial .ecc file behind */
	       goto terminate;
	    }
#endif
         /* Read the next data sectors of this layer. */

   	 for(i=0; i<actual_layer_sectors; i++)
	 {  RS02ReadSector(ec->ii, lay, ec->data+offset, block_idx[layer]);
	    block_idx[layer]++;
	    offset += 2048;
	 }

	 /* Now process the data bytes of the current layer. */

	 for(i=0; i<actual_layer_bytes; i++)
	 {  register int feedback;

	    feedback = gf_index_of[ec->data[i] ^ par_idx[sp]];

	    if(feedback != ALPHA0) /* non-zero feedback term */
	    {  register int spk = sp+1;
	       register int *gpoly = gf_gpoly + nroots;

	       switch(nroots-spk)  /* unrolled loop part1 */
	       {  
	          case 170: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 169: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 168: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 167: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 166: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 165: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 164: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 163: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 162: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 161: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 160: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 159: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 158: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 157: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 156: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 155: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 154: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 153: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 152: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 151: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 150: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 149: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 148: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 147: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 146: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 145: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 144: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 143: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 142: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 141: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 140: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 139: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 138: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 137: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 136: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 135: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 134: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 133: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 132: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 131: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 130: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 129: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 128: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 127: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 126: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 125: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 124: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 123: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 122: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 121: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 120: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 119: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 118: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 117: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 116: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 115: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 114: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 113: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 112: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 111: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 110: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 109: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 108: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 107: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 106: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 105: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 104: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 103: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 102: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 101: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 100: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 99: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 98: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 97: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 96: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 95: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 94: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 93: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 92: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 91: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 90: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 89: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 88: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 87: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 86: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 85: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 84: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 83: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 82: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 81: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 80: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 79: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 78: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 77: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 76: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 75: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 74: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 73: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 72: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 71: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 70: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 69: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 68: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 67: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 66: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 65: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 64: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 63: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 62: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 61: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 60: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 59: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 58: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 57: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 56: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 55: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 54: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 53: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 52: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 51: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 50: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 49: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 48: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 47: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 46: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 45: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 44: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 43: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 42: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 41: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 40: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 39: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 38: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 37: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 36: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 35: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 34: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 33: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 32: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 31: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 30: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 29: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 28: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 27: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 26: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 25: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 24: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 23: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 22: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 21: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 20: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 19: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 18: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 17: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 16: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 15: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 14: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 13: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 12: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 11: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 10: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  9: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  8: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  7: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  6: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  5: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  4: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  3: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  2: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  1: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	       }

	       spk = 0;
		  
	       switch(sp)  /* unrolled loop part2 */
	       {
	          case 170: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 169: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 168: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 167: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 166: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 165: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 164: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 163: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 162: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 161: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 160: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 159: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 158: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 157: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 156: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 155: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 154: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 153: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 152: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 151: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 150: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 149: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 148: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 147: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 146: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 145: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 144: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 143: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 142: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 141: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 140: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 139: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 138: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 137: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 136: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 135: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 134: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 133: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 132: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 131: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 130: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 129: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 128: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 127: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 126: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 125: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 124: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 123: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 122: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 121: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 120: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 119: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	          case 118: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 117: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 116: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 115: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 114: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 113: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 112: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 111: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
                  case 110: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 109: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 108: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 107: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 106: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 105: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 104: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 103: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 102: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 101: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		  case 100: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 99: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 98: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 97: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 96: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 95: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 94: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 93: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 92: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 91: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 90: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 89: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 88: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 87: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 86: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 85: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 84: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 83: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 82: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 81: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 80: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 79: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 78: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 77: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 76: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 75: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 74: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 73: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 72: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 71: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 70: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 69: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 68: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 67: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 66: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 65: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 64: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 63: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 62: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 61: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 60: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 59: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 58: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 57: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 56: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 55: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 54: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 53: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 52: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 51: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 50: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 49: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 48: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 47: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 46: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 45: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 44: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 43: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 42: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 41: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 40: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 39: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 38: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 37: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 36: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 35: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 34: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 33: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 32: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 31: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 30: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 29: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 28: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 27: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 26: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 25: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 24: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 23: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 22: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 21: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 20: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 19: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 18: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 17: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 16: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 15: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 14: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 13: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 12: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 11: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case 10: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  9: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  8: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  7: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  6: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  5: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  4: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  3: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  2: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
		   case  1: par_idx[spk++] ^= enc_alpha_to[feedback + *--gpoly];
	       }

	       par_idx[sp] = enc_alpha_to[feedback + gf_gpoly[0]];
	    }
	    else                   /* zero feedback term */
	      par_idx[sp] = 0;

	    par_idx += nroots;
	 }

	 if(++sp>=nroots) sp=0;   /* shift */

	 /* Report progress */

	 progress++;
	 percent = (1000*progress)/max_percent;
	 if(last_percent != percent) 
	 {
#if 0
	     if(Closure->guiMode)
	       SetProgress(wl->encPBar2, percent, 1000);
	     else
#endif
	       PrintProgress(_("Ecc generation: %3d.%1d%%"), percent/10, percent%10);
	    last_percent = percent;
	 }
      }

      /* The parity bytes have been prepared as sequences of nroots bytes for each 
	 ecc block. Now we split them up into nroots slices and write them out. */

      par_ptr = ec->parity;

      for(i=0; i<actual_layer_sectors; i++)
      {  int idx = 2048*i;

	 for(j=0; j<2048; j++, idx++)
	 {  for(k=0; k<nroots; k++)
	      ec->slice[k][idx] = *par_ptr++;
	 }
      }

      for(k=0; k<nroots; k++)
      {  int idx=0;

	for(i=0; i<actual_layer_sectors; i++, idx+=2048)
	 {  gint64 s = RS02EccSectorIndex(lay, k, chunk + i);

	    if(!LargeSeek(file, 2048*s))
	      Stop(_("Failed seeking to sector %lld in image: %s"), s, strerror(errno));

	    if(LargeWrite(file, ec->slice[k]+idx, 2048) != 2048)
	      Stop(_("Failed writing to sector %lld in image: %s"), s, strerror(errno));

	    MD5Update(&ec->md5Ctxt[k], ec->slice[k]+idx, 2048);
	}
      }

#if 0
      for(i=0; i<actual_layer_sectors; i++)
      {  for(j=0; j<2048; j++)
	 {  for(k=0; k<nroots; k++)
	      ec->slice[k][j] = *par_ptr++;
	 }

 	 for(k=0; k<nroots; k++)
	 {  gint64 s = RS02EccSectorIndex(lay, k, chunk + i);

	    if(!LargeSeek(file, 2048*s))
	      Stop(_("Failed seeking to sector %lld in image: %s"), s, strerror(errno));

	    if(LargeWrite(file, ec->slice[k], 2048) != 2048)
	      Stop(_("Failed writing to sector %lld in image: %s"), s, strerror(errno));

	    MD5Update(&ec->md5Ctxt[k], ec->slice[k], 2048);
	}
      }
#endif
   }

   /*** We can store only one md5sum in the header,
	so lets produce a meta-checksum from all nroots md5sums */

   for(i=0; i<nroots; i++)
     MD5Final(&ec->md5Sum[i*16], &ec->md5Ctxt[i]);

   MD5Init(&ec->md5Ctxt[0]);
   MD5Update(&ec->md5Ctxt[0], ec->md5Sum, 16*nroots);
   MD5Final(ec->eccSum, &ec->md5Ctxt[0]);
}

/***
 *** Append the parity information to the image
 ***/

void RS02Create(Method *method)
{  RS02Widgets *wl = (RS02Widgets*)method->widgetList;
   RS02Layout *lay;
   ecc_closure *ec = g_malloc0(sizeof(ecc_closure));
   ImageInfo *ii;

   /*** Register the cleanup procedure for GUI mode */

   ec->self = method;
   ec->wl = wl;
   ec->eh = g_malloc0(sizeof(EccHeader));
   ec->earlyTermination = TRUE;
   ec->timer   = g_timer_new();

   RegisterCleanup(_("Error correction file creation aborted"), ecc_cleanup, ec);

   /*** If the image already contains error correction information, remove it. */

   remove_old_ecc(ec);

   /*** Open image file and calculate a suitable redundancy .*/

   ii = ec->ii = OpenImageFile(NULL, WRITEABLE_IMAGE);

   lay = ec->lay = CalcRS02Layout(ii->sectors, 0);

   if(lay->nroots < 8)
     Stop(_("Not enough space on medium left for error correction data.\n"
	    "If reducing the image size or using a larger medium is\n"
	    "not an option, please create a separate error correction\n"
	    "file using the RS01 method.\n"));

   /*** Announce what we are going to do */

   if(!Closure->guiMode)
   {  PrintLog("* Experimental codec: Do not use for production releases.\n"
	       "* Generated error correction files and images may not work.\n"
	       "* Files generated by this codec may not be compatible\n"
	       "* with the final release.\n\n");
   }

   ec->msg = g_strdup_printf(_("Appending image with Method RS02: %lldM data, %lldM ecc (%d roots; %4.1f%% redundancy)"),
			     lay->dataSectors/512, lay->eccSectors/512, lay->nroots, lay->redundancy);
   PrintLog("%s\n",ec->msg);
   if(lay->redundancy < 20)
     PrintLog(_("* Warning: Using redundancies below 20%% may not give\n"
		"*          the expected data loss protection.\n"));

   /*** Check image for completeness and fetch its CRC sums */

   check_image(ec);

   /*** Expand the image by lay->eccSectors. */

   expand_image(ec);

   /*** Distribute and write the CRC sums */

   write_crc(ec);

   /*** Create the Reed-Solomon parts of the ecc section */

   create_reed_solomon(ec);

   /*** Prepare the Ecc header 
	and write all copies of the header out */

   prepare_header(ec);
   WriteRS02Headers(ec->ii->file, ec->lay, ec->eh);

   PrintTimeToLog(ec->timer, "for ECC generation.\n");

   PrintProgress(_("Ecc generation: 100.0%%\n"));
   PrintLog(_("Image has been appended with error correction data.\n"
	      "New image size is %lld sectors.\n"),
	      lay->eccSectors+lay->dataSectors);
   
#if 0
   if(Closure->guiMode)
   {  SetProgress(wl->encPBar2, 100, 100);

      SetLabelText(GTK_LABEL(wl->encFootline), 
		   _("The error correction file has been successfully created.\n"
		     "Make sure to keep this file on a reliable medium."),
		   Closure->eccName); 
   }
#endif

   /*** Clean up */

   ec->earlyTermination = FALSE;
   ecc_cleanup((gpointer)ec);
}

