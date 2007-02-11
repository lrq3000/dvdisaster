/*  dvdisaster: Additional error correction for optical media.
 *  Copyright (C) 2004-2007 Carsten Gnoerlich.
 *  Copyright (C) 2006 Andrei Grecu
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

/***
 *** Auxiliary functions for collecting some stuff incrementally
 ***/

/*
 * Determine work load of P/Q vectors in the given frame
 */

void CalculatePQLoad(RawBuffer *rb)
{  unsigned char p_vector[P_VECTOR_SIZE];
   unsigned char q_vector[Q_VECTOR_SIZE];
   int frame_idx = rb->samplesRead - 1;
   unsigned char *new_frame = rb->rawBuf[frame_idx];
   int ignore[2];
   int q, p;
   int err;
	
   for(q = 0; q < N_Q_VECTORS; q++)
   {
     GetQVector(new_frame, q_vector, q);
     err = DecodePQ(rb->rt, q_vector, Q_PADDING, ignore, 0);
     if(err <  0) rb->qLoad[frame_idx] += 2;
     if(err == 1) rb->qLoad[frame_idx]++; /* We assume without any erasures specified there can't be more than 1 errors corrected. */
   }      

   for(p = 0; p < N_P_VECTORS; p++)
   {
     GetPVector(new_frame, p_vector, p);
     err = DecodePQ(rb->rt, p_vector, P_PADDING, ignore, 0);
     if(err <  0) rb->pLoad[frame_idx] += 2;
     if(err == 1) rb->pLoad[frame_idx]++; /* We assume without any erasures specified there can't be more than 1 errors corrected. */
   }      
}   

/*
 * Collect a list of all seen P/Q parity bytes.
 */

void UpdatePQParityList(RawBuffer *rb, unsigned char *new_frame)
{  unsigned char p_vector[P_VECTOR_SIZE];
   unsigned char q_vector[Q_VECTOR_SIZE];
   int i,p,q;

   /*** See if new frame has any Q parity bytes different from the existing ones. */

   for(q=0; q<N_Q_VECTORS; q++)
   {  int qfound = FALSE;

      GetQVector(new_frame, q_vector, q);

      for(i=0; i<rb->qParityN[q][0]; i++)
	if(rb->qParity1[q][i] == q_vector[43])
	{  qfound = TRUE;
	   break;
	}

      if(!qfound)
	rb->qParity1[q][rb->qParityN[q][0]++] = q_vector[43];

      qfound = FALSE;

      for(i=0; i<rb->qParityN[q][1]; i++)
	if(rb->qParity2[q][i] == q_vector[44])
	{  qfound = TRUE;
	   break;
	}

      if(!qfound)
	rb->qParity2[q][rb->qParityN[q][1]++] = q_vector[44];
   }

   /*** See if new frame has any P parity bytes different from the existing ones. */

   for(p=0; p<N_P_VECTORS; p++)
   {  int pfound = FALSE;

      GetPVector(new_frame, p_vector, p);

      for(i=0; i<rb->pParityN[p][0]; i++)
	if(rb->pParity1[p][i] == p_vector[24])
	{  pfound = TRUE;
	   break;
	}

      if(!pfound)
	rb->pParity1[p][rb->pParityN[p][0]++] = p_vector[24];

      pfound = FALSE;

      for(i=0; i<rb->pParityN[p][1]; i++)
	if(rb->pParity2[p][i] == p_vector[25])
	{  pfound = TRUE;
	   break;
	}

      if(!pfound)
	rb->pParity2[p][rb->pParityN[p][1]++] = p_vector[25];
   }
}

/***
 *** Heuristic L-EC attempt
 *** Andrei Grecu, 2006
 */

static int eval_q_candidate(RawBuffer *rb, unsigned char *q_vector, int q, 
			    int *p_failures_out, int *p_errors_out)
{
   unsigned char     p_vector[P_VECTOR_SIZE];
   unsigned char old_q_vector[Q_VECTOR_SIZE];
   int erasures[Q_VECTOR_SIZE];
   int p, p_errors = 0;
   int p_failures = 0;
   int err;
   
   GetQVector(rb->recovered, old_q_vector, q);
   SetQVector(rb->recovered,     q_vector, q);
   
   /* Count P failures after setting our Q vector. */

   for(p = 0; p < N_P_VECTORS; p++)
   {
      GetPVector(rb->recovered, p_vector, p);
      err = DecodePQ(rb->rt, p_vector, P_PADDING, erasures, 0);
      if(err <  0) p_failures++;
      else if(err == 1) p_errors++;
   }            

   SetQVector(rb->recovered, old_q_vector, q);

   *p_failures_out = p_failures;
   *p_errors_out   = p_errors;

   return TRUE;
}

static void eval_p_candidate(RawBuffer *rb, unsigned char *p_vector, int p, 
			    int *q_failures_out, int *q_errors_out)
{
   unsigned char     q_vector[Q_VECTOR_SIZE];
   unsigned char old_p_vector[P_VECTOR_SIZE];
   int erasures[P_VECTOR_SIZE];
   int q, q_errors = 0;
   int q_failures = 0;
   int err;
   
   GetPVector(rb->recovered, old_p_vector, p);
   SetPVector(rb->recovered,     p_vector, p);
   
   /* Count Q failures after setting our P vector. */

   for(q = 0; q < N_Q_VECTORS; q++)
   {
      GetQVector(rb->recovered, q_vector, q);
      err = DecodePQ(rb->rt, q_vector, Q_PADDING, erasures, 0);
      if(err <  0) q_failures++;
      else if(err == 1) q_errors++;
   }            

   SetPVector(rb->recovered, old_p_vector, p);

   *q_failures_out = q_failures;
   *q_errors_out = q_errors;
}


/*
 * An enhanced L-EC loop
 */   

//#define DEBUG_HEURISTIC_LEC

int HeuristicLEC(unsigned char *cd_frame, RawBuffer *rb, unsigned char *out)
{
   unsigned char p_vector[P_VECTOR_SIZE];
   unsigned char q_vector[Q_VECTOR_SIZE];
   unsigned char p_state[P_VECTOR_SIZE];
   unsigned char q_state[Q_VECTOR_SIZE];
   int erasures[Q_VECTOR_SIZE], decimated_erasures[2], erasure_count;
   int p_failures, q_failures;
   int p_corrected, q_corrected;
   int i,p,q;
   int iteration=1;
   int p_err, q_err;
   int p_decimated, q_decimated;
   int last_p_err = N_P_VECTORS;
   int last_q_err = N_Q_VECTORS;
   int last_p_failures = N_P_VECTORS;
   int last_q_failures = N_Q_VECTORS;
   int max_p_failures = 0;
   int max_p_errors = 0;
   int max_q_failures = 0;
   int max_q_errors = 0;
   int err;

   memset(rb->byteState, FRAME_BYTE_UNKNOWN, rb->sampleLength);
   
   /* Count initial P failures */

   for(p = 0; p < N_P_VECTORS; p++)
   {
      GetPVector(rb->recovered, p_vector, p);
      err = DecodePQ(rb->rt, p_vector, P_PADDING, erasures, 0);
      if(err < 0) max_p_failures++;
      if(err == 1) max_p_errors++;
   }

   /* Count initial Q failures */

   for(q = 0; q < N_Q_VECTORS; q++)
   {
      GetQVector(rb->recovered, q_vector, q);
      err = DecodePQ(rb->rt, q_vector, Q_PADDING, erasures, 0);
      if(err < 0) max_q_failures++;
      if(err == 1) max_q_errors++;
   }   
   
#ifdef DEBUG_HEURISTIC_LEC
   Verbose("      P-failures/corrected/errors + decimated: %2d/%2d/ 0 + 0\n", max_p_failures, max_p_errors);
   Verbose("      Q-failures/corrected/errors + decimated: %2d/%2d/ 0 + 0\n", max_q_failures, max_q_errors);
#endif

   for(; ;) /* iterate over P- and Q-Parity until failures converge */
   {   
      p_failures  = q_failures = 0;
      p_corrected = q_corrected = 0;
      p_err = q_err = 0;
      p_decimated = q_decimated = 0;

      /* Perform P-Parity error correction */

      for(p=0; p<N_P_VECTORS; p++)
      {  
         /* Determine number of erasures */

         GetPVector(rb->byteState, p_state, p);
         erasure_count = 0;

         for(i=0; i<P_VECTOR_SIZE; i++)
            if(p_state[i] == FRAME_BYTE_ERROR) erasures[erasure_count++] = i;

         /* First try to see whether P is correctable without erasure markings. */

         GetPVector(rb->recovered, p_vector, p);
         err = DecodePQ(rb->rt, p_vector, P_PADDING, erasures, 0);
         
         if(err == 1) /* Store back corrected vector */ 
         {  
            SetPVector(rb->recovered, p_vector, p);
            FillPVector(rb->byteState, FRAME_BYTE_GOOD, p);
            p_corrected++;
            p_err += err;
         }
            
         /* Erasure markings are overly pessimistic as each failing vector will mark
	    a full row as an erasure. We assume that there are only 2 real erasures
	    and try all combinations.
	    The case for 2 erasure is also included here. */

         if(err < 0 && erasure_count > 1) 
	 {  unsigned char best_p[P_VECTOR_SIZE];
	    int best_q_failures = N_Q_VECTORS;
	    int best_q_errors = N_Q_VECTORS;
 	    int candidate = FALSE;
	    int a, b;

            /* Try error correction with decimated erasures */
            
            for(a = 0; a < erasure_count - 1; a++)
            {
               for(b = a + 1; b < erasure_count; b++)
               {
                  decimated_erasures[0] = erasures[a];
                  decimated_erasures[1] = erasures[b];
                  GetPVector(rb->recovered, p_vector, p);
                  err = DecodePQ(rb->rt, p_vector, P_PADDING, decimated_erasures, 2);
                  if(err == 2) 
		  {  int q_err, q_fail;

		     candidate = TRUE;
		     eval_p_candidate(rb, p_vector, p, &q_fail, &q_err);

		     if(q_fail <= best_q_failures && q_err <= best_q_errors)
		     {  best_q_failures = q_fail;
		        best_q_errors   = q_err;
			memcpy(best_p, p_vector, P_VECTOR_SIZE); 
		     }
		  }
               }
            }

            if(!candidate) err = -1; /* If P failed */
            else
            {  FillPVector(rb->byteState, FRAME_BYTE_GOOD, p);
               SetPVector(rb->recovered, best_p, p);
               
               err = 0;
               p_err += 2;
               p_corrected++;
               p_decimated++;
            }
         }

         if(err == 0) FillPVector(rb->byteState, FRAME_BYTE_GOOD, p);
         if(err < 0)  /* Uncorrectable. */
         {  
            p_failures++;
            FillPVector(rb->byteState, FRAME_BYTE_ERROR, p);
         }
      }

      if(CheckEDC(rb->recovered, rb->xaMode)) break;

      /* Perform Q-Parity error correction */

      for(q=0; q<N_Q_VECTORS; q++)
      {  
         /* Determine number of erasures */

         GetQVector(rb->byteState, q_state, q);
         erasure_count = 0;

         for(i=0; i<Q_VECTOR_SIZE-2; i++)
            if(q_state[i] == FRAME_BYTE_ERROR) erasures[erasure_count++] = i;

         /* First try to see whether Q is correctable without erasure markings. */

         GetQVector(rb->recovered, q_vector, q);
         err = DecodePQ(rb->rt, q_vector, Q_PADDING, erasures, 0);

         if(err == 1) /* Store back corrected vector */ 
         {  
            SetQVector(rb->recovered, q_vector, q);
            FillQVector(rb->byteState, FRAME_BYTE_GOOD, q);
            q_corrected++;
            q_err++;
         }
         
         /* If there are more than 2 erasures we have have to decimate them to 2. */
         /* The case for 2 erasure is also included here. */

         if(err < 0 && erasure_count > 1) 
	 {  unsigned char best_q[Q_VECTOR_SIZE];
	    int best_p_failures = N_P_VECTORS;
	    int best_p_errors = N_P_VECTORS;
	    int candidate = FALSE;
	    int a, b;      

            /* Try error correction with decimated erasures */
            
            for(a = 0; a < erasure_count - 1; a++)
            {
               for(b = a + 1; b < erasure_count; b++)
               {
                  decimated_erasures[0] = erasures[a];
                  decimated_erasures[1] = erasures[b];
                  GetQVector(rb->recovered, q_vector, q);
                  err = DecodePQ(rb->rt, q_vector, Q_PADDING, decimated_erasures, 2);
                  if(err == 2) 
		  {  int p_err, p_fail;

		     candidate = TRUE;
                     eval_q_candidate(rb, q_vector, q, &p_fail, &p_err);

		     if(p_fail <= best_p_failures && p_err <= best_p_errors)
		     {  best_p_failures = p_fail;
		        best_p_errors   = p_err;
			memcpy(best_q, q_vector, Q_VECTOR_SIZE); 
		     }
                  }
               }
            }

            if(!candidate) err = -1; /* If Q failed */
            else
            {  FillQVector(rb->byteState, FRAME_BYTE_GOOD, q);
               SetQVector(rb->recovered, best_q, q);
               
               err = 0;
               q_err += 2;
               q_corrected++;
               q_decimated++;
            }
         }
         
         if(err == 0) FillQVector(rb->byteState, FRAME_BYTE_GOOD, q);
         if(err < 0)  /* Uncorrectable. Mark bytes are erasure. */
         {  
            q_failures++;
            FillQVector(rb->byteState, FRAME_BYTE_ERROR, q);
         }
      }

      /* See if there was an improvement */

#ifdef DEBUG_HEURISTIC_LEC
      Verbose("L-EC: iteration %d\n", iteration); 
      Verbose("      P-failures/corrected/errors + decimated: %2d/%2d/%2d + %2d\n", p_failures, p_corrected, p_err, p_decimated);
      Verbose("      Q-failures/corrected/errors + decimated: %2d/%2d/%2d + %2d\n", q_failures, q_corrected, q_err, q_decimated);
#endif
      
      if(p_failures + p_err + q_failures + q_err == 0) break;
      if(last_p_err <= p_err && last_q_err <= q_err && last_p_failures <= p_failures && last_q_failures <= q_failures) break;
		
      if(iteration > N_P_VECTORS + N_Q_VECTORS) break;
      
      if(CheckEDC(rb->recovered, rb->xaMode)) break;

      last_p_err = p_err;
      last_q_err = q_err;
      last_p_failures = p_failures;
      last_q_failures = q_failures;
      iteration++;
   }
   
   return TRUE;
}

/***
 *** Heuristic search for best sector
 *** Andrei Grecu, 2006
 */

//#define DEBUG_SEARCHPLAUSIBLESECTOR

static int check_q_plausibility(RawBuffer *rb, unsigned char *target_q_vector, 
				int q, int pos_a, int pos_b)
{  unsigned char q_vector[Q_VECTOR_SIZE];
   int plausible = FALSE;
   int q_index;
   int i;
   
   /* If no position was given, then find it out. */

   if(pos_a == -1)
   {
     GetQVector(rb->recovered, q_vector, q);
     for(i = 0; i < Q_VECTOR_SIZE; i++)
       if(target_q_vector[i] != q_vector[i]) 
       {
	 pos_a = i;
	 break;
       }

     /* pos_a == -1 implies that only 1 byte was corrected
        -> don't care about pos_b */

     pos_b = -1;  
   }
	
   /* Check plausibility of pos_a. */

   q_index = QToByteIndex(q, pos_a);

   for(i = 0; i < rb->samplesRead; i++)
     if(rb->rawBuf[i][q_index] == target_q_vector[pos_a]) 
     { 
       plausible = TRUE; 
       break; 
     }
	
   if(!plausible) return FALSE;

   /* pos_b only != -1 when Q corrected in erasure mode */

   plausible = FALSE;
	
   if(pos_b != -1)
   {
     /* Check plausibility of pos_b. */

     q_index = QToByteIndex(q, pos_b);

     for(i = 0; i < rb->samplesRead; i++)
       if(rb->rawBuf[i][q_index] == target_q_vector[pos_b]) 
       { 
	 plausible = TRUE; 
	 break; 
       }
     
     if(!plausible) return FALSE;
   }

   return TRUE;
}

static int check_p_plausibility(RawBuffer *rb, unsigned char *target_p_vector, 
				int p, int pos_a, int pos_b)
{  unsigned char p_vector[P_VECTOR_SIZE];
   int plausible = FALSE;
   int p_index;
   int i;
   
   /* If no position was given, then find it out. */

   if(pos_a == -1)
   {
     GetPVector(rb->recovered, p_vector, p);
     for(i = 0; i < P_VECTOR_SIZE; i++)
     {
       if(target_p_vector[i] != p_vector[i]) 
       {
	 pos_a = i;
	 break;
       }
     }

     /* pos_a == -1 implies that only 1 byte was corrected
        -> don't care about pos_b */

     pos_b = -1;
   }
	
   /* Check plausibility of pos_a. */

   p_index = PToByteIndex(p, pos_a);

   for(i = 0; i < rb->samplesRead; i++)
     if(rb->rawBuf[i][p_index] == target_p_vector[pos_a]) 
     { 
       plausible = TRUE; 
       break; 
     }

   if(!plausible) return FALSE;
   
   /* pos_b only != -1 when P corrected in erasure mode */

   plausible = FALSE;

   if(pos_b != -1)
   {
     /* Check plausibility of pos_b. */

     p_index = PToByteIndex(p, pos_b);

     for(i = 0; i < rb->samplesRead; i++)
       if(rb->rawBuf[i][p_index] == target_p_vector[pos_b]) 
       { 
	 plausible = TRUE; 
	 break; 
       }
		
     if(!plausible) return FALSE;
   }
   
   return TRUE;
}

static int find_better_p(RawBuffer *rb, int p, int refError)
{  unsigned char p_vector[P_VECTOR_SIZE];
   int np1, np2;
   int dummy[2];
   int err;
  
   /* Try all possible Ps and see whether we get a better load. */
 
   for(np1 = 0; np1 < rb->pParityN[p][0]; np1++)
   {
     for(np2 = 0; np2 < rb->pParityN[p][1]; np2++)
     {
       GetPVector(rb->recovered, p_vector, p);
       p_vector[24] = rb->pParity1[p][np1];
       p_vector[25] = rb->pParity2[p][np2];
	  		
       err = DecodePQ(rb->rt, p_vector, P_PADDING, dummy, 0);
       if(err < refError && err >= 0)
       {
	 SetPVector(rb->recovered, p_vector, p);			
	 return TRUE;
       }
     }
   }
   
   return FALSE;
}

static int find_better_q(RawBuffer *rb, int q, int refError)
{  unsigned char q_vector[Q_VECTOR_SIZE];
   int nq1, nq2;
   int dummy[2];
   int err;
  
   /* Try all possible Qs and see whether we get a better load. */

   for(nq1 = 0; nq1 < rb->qParityN[q][0]; nq1++)
   {
     for(nq2 = 0; nq2 < rb->qParityN[q][1]; nq2++)
     {
       GetQVector(rb->recovered, q_vector, q);
       q_vector[43] = rb->qParity1[q][nq1];
       q_vector[44] = rb->qParity2[q][nq2];
	  		
       err = DecodePQ(rb->rt, q_vector, Q_PADDING, dummy, 0);
       if(err < refError && err >= 0)
       {
	 SetQVector(rb->recovered, q_vector, q);			
	 return TRUE;
       }
     }
   }
   
   return FALSE;
}

static void initialize_frame(RawBuffer *rb)
{  int best_sector = 0;
   int max_load = 2 * (N_P_VECTORS + N_Q_VECTORS);
   int i;
	
   /* Initialize sector header. */

   InitializeCDFrame(rb->recovered, rb->lba);

   /* Search for block with least P and Q load and load into the frame. */

   for(i = 0; i < rb->samplesRead; i++) 
   {
     if(rb->qLoad[i] + rb->pLoad[i] < max_load)
     {
       max_load = rb->pLoad[i] + rb->qLoad[i];
       best_sector = i;
     }
   }
   
   memcpy(&(rb->recovered[16]), &(rb->rawBuf[best_sector][16]), rb->sampleLength - 16);
}

//#define DEBUG_SEARCH_PLAUSIBLE

int SearchPlausibleSector(RawBuffer *rb)
{
   unsigned char p_vector[26];
   unsigned char q_vector[45];
   int decimated_erasures[2];
   int p_failures, q_failures;
   int p_corrected, q_corrected;
   int p,q;
   int iteration=1;
   int p_err, q_err;
   int p_decimated, q_decimated;
   int last_p_err = N_P_VECTORS;
   int last_q_err = N_Q_VECTORS;
   int last_p_failures = N_P_VECTORS;
   int last_q_failures = N_Q_VECTORS;
   int err;
   
   /* Initialize sector */     

   initialize_frame(rb);
	
   for(; ;) /* iterate over P- and Q-Parity until failures converge */
   {   
      p_failures = q_failures = 0;
      p_corrected = q_corrected = 0;
      p_err = q_err = 0;
      p_decimated = q_decimated = 0;

      /* Perform Q-Parity error correction */

      for(q=0; q<N_Q_VECTORS; q++)
      {  
      	/* Check whether Q is correct. */
      	GetQVector(rb->recovered, q_vector, q);
	err = DecodePQ(rb->rt, q_vector, Q_PADDING, decimated_erasures, 0);
         
	/* If it is not correct. */
	if(err == 1 || err < 0)
        {	        
	  /* If Q is correctable. */
	  if(err == 1)
	  {
	    /* Check correction for plausibility: if corrected byte was read in one of the read attempts. */
	    if(check_q_plausibility(rb, q_vector, q, -1, -1))
	    {
	      /* Store back corrected vector */ 
	      SetQVector(rb->recovered, q_vector, q);
	      q_corrected++;
	      q_err++;
	    }
	    else 
	    {
	      /* See whether we can find some Q parity bytes accepting the original Q vector. */
	      if(find_better_q(rb, q, 1)) 
	      { 
		q_corrected++; 
		q_err++; 
	      }
	      else err = -1;
	    }
	  }
	         
	  /* If correction is not plausible or possible then try 2 error-decimation. */
	  if(err < 0)
	  {
	    /* Try error correction with decimated erasures.
	       Note that no erasure information for the parity bytes is available */
	    int a, b; 
	    int solFound = FALSE;     
	    for(a = 0; a < Q_VECTOR_SIZE - 2; a++)
	    {
	      for(b = a + 1; b < Q_VECTOR_SIZE - 2; b++)
	      {
		decimated_erasures[0] = a;
		decimated_erasures[1] = b;
		GetQVector(rb->recovered, q_vector, q);
		err = DecodePQ(rb->rt, q_vector, Q_PADDING, decimated_erasures, 2);
		if(err == 2) 
		{ 
		  if(check_q_plausibility(rb, q_vector, q, a, b)) { solFound = TRUE; break;	}
		}
	      }
	      if(solFound) break;
	    }
	    if(solFound)
	    {
	      /* Store back corrected vector */ 
	      SetQVector(rb->recovered, q_vector, q);
	      q_err += 2;
	      q_corrected++;
	      q_decimated++;
	    }
	    else 
	    {
	      /* See whether we can find some Q parity bytes accepting the original Q vector. */
	      if(find_better_q(rb, q, 2)) 
	      { 
		q_corrected++; 
		q_err++; 
	      }
	      else q_failures++; /* If Q failed */
	    }
	  }
	}
      }

      /* Perform P-Parity error correction */

      for(p=0; p<N_P_VECTORS; p++)
      {  
      	/* Check whether P is correct. */
      	GetPVector(rb->recovered, p_vector, p);
	err = DecodePQ(rb->rt, p_vector, P_PADDING, decimated_erasures, 0);
         
	/* If it is not correct. */
	if(err == 1 || err < 0)
        {	        
	  /* If Q is correctable. */
	  if(err == 1)
	  {
	    /* Check correction for plausibility: if corrected byte was read in one of the read attempts. */
	    if(check_p_plausibility(rb, p_vector, p, -1, -1))
	    {
	      /* Store back corrected vector */ 
	      SetPVector(rb->recovered, p_vector, p);
	      p_corrected++;
	      p_err++;
	    }
	    else 
	    {
	      /* See whether we can find some P parity bytes accepting the original P vector. */
	      if(find_better_p(rb, p, 1)) 
	      { 
		p_corrected++; 
		p_err++; 
	      }
	      else err = -1;
	    }
	  }
	  
	  /* If correction is not plausible or possible then try 2 error-decimation. */
	  if(err < 0)
	  {
	    /* Try error correction with decimated erasures */
	    int a, b; 
	    int solFound = FALSE;     
	    for(a = 0; a < P_VECTOR_SIZE; a++)
	    {
	      for(b = a + 1; b < P_VECTOR_SIZE; b++)
	      {
		decimated_erasures[0] = a;
		decimated_erasures[1] = b;
		GetPVector(rb->recovered, p_vector, p);
		err = DecodePQ(rb->rt, p_vector, P_PADDING, decimated_erasures, 2);
		if(err == 2) 
		{ 
		  if(check_p_plausibility(rb, p_vector, p, a, b)) { solFound = TRUE; break; }
		}
	      }
	      if(solFound) break;
	    }
	    if(solFound)
	    {
	      /* Store back corrected vector */ 
	      SetPVector(rb->recovered, p_vector, p);
	      p_err += 2;
	      p_corrected++;
	      p_decimated++;
	    }
	    else 
	    {
	      /* See whether we can find some P parity bytes accepting the original P vector. */
	      if(find_better_p(rb, p, 2)) 
	      { 
		p_corrected++; 
		p_err++; 
	      }
	      else p_failures++; /* If P failed */
	    }
	  }
	}
      }

      /* See if there was an improvement */

#ifdef DEBUG_SEARCH_PLAUSIBLE
      Verbose("SPS L-EC: iteration %d\n", iteration); 
      Verbose("      Q-f/c/e + d: %2d/%2d/%2d + %2d\n", q_failures, q_corrected, q_err, q_decimated);
      Verbose("      P-f/c/e + d: %2d/%2d/%2d + %2d\n", p_failures, p_corrected, p_err, p_decimated);
#endif
      
      if(p_failures + p_err + q_failures + q_err == 0) break;
      if(last_p_err <= p_err && last_q_err <= q_err && last_p_failures <= p_failures && last_q_failures <= q_failures) break;
      
      if(iteration > N_P_VECTORS + N_Q_VECTORS) break;
      if(p_failures == 0)
      {
	if(CheckEDC(rb->recovered, rb->xaMode)) break;
      }
      
      last_p_err = p_err;
      last_q_err = q_err;
      last_p_failures = p_failures;
      last_q_failures = q_failures;
      iteration++;
   }	

   return TRUE;
}
