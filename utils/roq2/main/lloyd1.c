/******************************************************************************

 *

 * NAME

 *    lloyd1.c

 *    J. R. Goldschneider 7/93

 *

 * MODIFICATIONS

 *    11/93 This was changed slightly to ensure that if distortion is zero,

 *    codebooksize is set to size so that the program will exit gracefully. JRG

 *

 * SYNOPSIS

 *    lloyd1(codebook,size)

 *

 * DESCRIPTION

 *    lloyd1 performs the generalized lloyd algorithm for the given 

 *    codebook with size codewords.  lloyd exits with a codebook if the 

 *    percent change in distortion from one pass to the next is less than 

 *    threshold. If there are any empty cells, lloyd will try to split the most

 *    populous cells.  lloyd will attempt to split cells up to size

 *    times unless the distortion is zero.  If the distortion is zero, then

 *    those codewords are returned and the global variable codebooksize 

 *    is modified since large codebooks can no inter be made.

 *    This ensures that the zero distortion codebook is returned to the

 *    user, but allows that the program stdvq terminate normally. If

 *    the write_all_codebooks option is not selected that the program

 *    will terminate.  The search for the closest codeword is constrained

 *    using the annulus constrained method of Huang, Bi, Stiles, and Harris 

 *    (algorithm II) of the IEEE Transactions on Image Processing July 1992.  

 *    

 * RETURN VALUE

 *    lloyd returns a positive number, i.e. the distortion, if a codebook

 *    is found.  A negative number is returned if there is not enough

 *    memory for temporary storage and computations or if a codebook

 *    cannot be found.  There are two possibilities for the latter case:

 *    there are empty cells which could not be filled after size attempts,

 *    or there are permanently empty cells, zero distortion, and

 *    the write_all_codebooks option was not selected.

 *

 * PARAMETERS

 *    codebook contains the codewords.

 *    size is the current number of words in the codebook.

 *

 * CALLS

 *    splitcodewords()

 *

 *****************************************************************************/

#include <stdio.h>

#include <string.h>

#include <math.h>

#include <stdlib.h>

#include "vq.h"

#include "stdvq.def"

#include "stdvq.h"



long    count[256];         /* array of number of vectors per region */

VQDATA  centroid[256][256];     /* centroids of each voronoi region */

VQDATA  norm[256];          /* array of norms of each codeword */

VQDATA  tempcodeword1[256];  /* temporary pointer to a codeword */

VQDATA  *tempcodeword = tempcodeword1;

DATA    tempvector[256];    /* the training vector */



VQDATA lloyd1(codebook,size)

     VQDATA **codebook;

     int   size;



{

  VQDATA  distortion;     /* distortion between training set and codebook */

  VQDATA  olddistortion;  /* distortion from the previous pass of GLA */

  VQDATA  bestdistortion; /* distortion between 1 vector and best codeword */

  VQDATA  tempnorm;       /* temporary variable */

  VQDATA  tempVQDATA;     /* temporary variable */

  VQDATA  temp;           /* temporary variable */

  long    numbervectors;  /* number of vectors used */

  long    i,j,n,imin,imax;/* counters and indices */

  long    pass;           /* the number of attempts to split empty cells */

  long    bestcodeword;   /* index of the best codeword for a given vector */

  long    bestnorm;       /* used as an index for sorting norm */

  long    emptycells;     /* number of empty cells */



  /* allocate memory for the centroids, temporary vector, and count vector */

//  if (!(centroid = (VQDATA **) calloc(size,sizeof(VQDATA *))) ||

//      !(tempvector = (DATA *) calloc(dimension, sizeof(DATA))) ||

//      !(norm  = (VQDATA *) calloc(size,sizeof(VQDATA))) ||

//      !(tempcodeword = (VQDATA *) calloc(dimension,sizeof(VQDATA))) ||      

//      !(count = (long *) calloc(size, sizeof(long)))) {

//    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);

//    return(-1.0);

//  }



  /* allocate memory for the dimension of each centroid, and initialize the

   * the centroids and countnumber */ 

  for(i=0; i < size; i++) {

//    if (!(centroid[i] = (VQDATA *) calloc(dimension,sizeof(VQDATA)))) {

//      fprintf(stderr,"%s: %s\n",programname,NOMEMORY);

//      return(-1.0);

//    }

    for (j = 0; j < dimension; j++) {

      centroid[i][j] = 0.0;

    }

    count[i] = 0;

  }





  /* do the lloyd iteration.  Use the nearest neighbor condition to

     find the cells.  Then find the centroid of each cell. */



  olddistortion = HUGE; /* first pass requires very large distortion */

  emptycells = 1; /* ensures that loop is done at least one time */

  pass = 0; /* no empty cells have been found yet */



  do {



    /* use the annulus method to constrain the search space */

    /* start by finding the norm of each codeword and storing it in

       an array */

    for (i = 0; i < size; i++) {

      norm[i] = 0.0;

      for (j = 0; j < dimension; j+=4) {

		norm[i] += codebook[i][j+0]*codebook[i][j+0];

		norm[i] += codebook[i][j+1]*codebook[i][j+1];

		norm[i] += codebook[i][j+2]*codebook[i][j+2];

		norm[i] += codebook[i][j+3]*codebook[i][j+3];

      }

      norm[i] = sqrt(norm[i]);

    }

    

    /* reorder the codebook by ascending norm */

    for (i = 0; i < size; i++) {

      bestnorm = i;

      /* find the lowest norm */

      for ( j = i; j < size; j++) {

		if (norm[j] < norm[bestnorm]) bestnorm = j;

      }

      /* exchange values */

      tempcodeword = codebook[i];

      codebook[i] = codebook[bestnorm];

      codebook[bestnorm] = tempcodeword;

      

      tempVQDATA = norm[i];

      norm[i] = norm[bestnorm];

      norm[bestnorm] = tempVQDATA;

    }



    /* compute distortion */

    distortion = 0.0;



	vectorreset();



    /* read in vector and find the closest codeword */

    while ( (tempnorm = getnextvector(tempvector))>=0 ) {



      /* find the codework with a norm closest to the input vector */

      bestcodeword = min_sup(0,size-1,tempnorm,norm);

      

      /* compute the distance between the codeword and the input vector that

	 has the closest norm */

      tempVQDATA = 0.0;

      for (j = 0; j < dimension; j+=4) {

		temp = tempvector[j+0] - codebook[bestcodeword][j+0];

		tempVQDATA += temp*temp;

		temp = tempvector[j+1] - codebook[bestcodeword][j+1];

		tempVQDATA += temp*temp;

		temp = tempvector[j+2] - codebook[bestcodeword][j+2];

		tempVQDATA += temp*temp;

		temp = tempvector[j+3] - codebook[bestcodeword][j+3];

		tempVQDATA += temp*temp;

      }    

      tempVQDATA = sqrt(tempVQDATA);



      /* identify the subset of the codewords to search such that 

	 norm(input)-distance <= norm(codeword) <= norm(input)+distance */

      imin = min_sup(0, size-1, tempnorm-tempVQDATA, norm);

      imax = max_inf(imin, size-1, tempnorm+tempVQDATA, norm);



      /* find the bestcodeword using partial distortion method */

      bestdistortion = HUGE; /* keep convention that ties go to lower index */

      for (i = imin; i <= imax; i++){ /* find the best codeword */

		tempVQDATA = 0.0;

		for (j = 0; j < dimension; j++) {

	  		temp = tempvector[j] - codebook[i][j];

	  		tempVQDATA += temp*temp;

	  		if (tempVQDATA > bestdistortion) j = dimension; /* abort loop */

		}



		if (tempVQDATA < bestdistortion) {

	  		bestdistortion = tempVQDATA;

	  		bestcodeword = i;

		}



	/* if the bestdistortion is 0.0, the best codeword is found */

		if (bestdistortion == 0.0) i=size;

      }



      count[bestcodeword]++;



      for (j = 0; j < dimension; j+=4){

		centroid[bestcodeword][j+0] += tempvector[j+0];

		centroid[bestcodeword][j+1] += tempvector[j+1];

		centroid[bestcodeword][j+2] += tempvector[j+2];

		centroid[bestcodeword][j+3] += tempvector[j+3];

      }

      distortion += bestdistortion;

    }    /* all training vectors have been encoded */



    /* normalize the distortion */

    numbervectors = 0;

    for (i = 0; i < size; i++) {

      numbervectors += count[i];

    }



    if(numbervectors == 0) {

      fprintf(stderr,"Sorry %s: %s: %s\n",programname,trainingname,NOTFOUND);

      return(-1.0);

    }

    distortion /= (VQDATA) numbervectors;

    if(distortion < 0) {

      fprintf(stderr,"%s: %s: %s\n",programname,OVERFLOWED,ABORT_STDVQ);

      return(-1.0);

    }



    /* if distortion = 0.0 or if change in distortion < threshold AND

       if there aren't any empty cells, exit */

    if ( (emptycells == 0) && 

	((distortion == 0.0) || 

	 ( (olddistortion - distortion)/distortion < threshold)) ) {



      /* if distortion is 0, let the program exit gracefully */

      if(distortion == 0 && size < codebooksize) {

		fprintf(stderr,"%s %d\n",STOP,size);

		size = codebooksize;

      }

      return(distortion);

    }



    /* Find the number of empty cells */

    emptycells = 0;

    for (i = 0; i < size; i++) {

      if (count[i] == 0) ++emptycells;

    }



    /* no empty cells, find new centroids and reinitialize for next pass */

    if (emptycells == 0) {

      for (i = 0; i < size; i++) {

	for (j = 0; j < dimension; j++ ) {

	  codebook[i][j] = centroid [i][j] / (VQDATA) count[i];

	  centroid[i][j] = 0.0;

	}

	count[i] = 0;

      }

      olddistortion = distortion;

    }

    

    /* there are empty cells, split the most populous codewords. try again */

    else { 



      /* if the distortion is 0, can't split cells, exit program gracefully */

      if (distortion == 0.0) { 

		for(n = 0; n < size - emptycells; n++) {

		j = 0;

		bestcodeword = 0;

		for (i = 0; i < size; i++) {

	  		if (count[i] > j) {

	    		j = count[i];

	   			bestcodeword = i;

	  		}

		}

	

		for (j = 0; j < dimension; j++ ) { /* find centroid */

	  		codebook[n][j] = centroid[bestcodeword][j] / (VQDATA) count[bestcodeword];

	  		centroid[bestcodeword][j] = 0.0;

		}

		count[bestcodeword] = 0;      

      }

	if (emptycells > 1) {

	  fprintf(stderr,"%s %d %s %d\n",

		  NOFILL,emptycells,EMPTYCELLS,size);

	}

	else {

	  fprintf(stderr,"%s %d %s %d\n",

		  NOFILL,emptycells,EMPTYCELL,size);

	}

	codebooksize = size - emptycells;

	fprintf(stderr,"%s %d\n",STOP,codebooksize);

	return(distortion);

      }



     /* If there have been too many attempts to fill cells, exit program */

     if (pass == 16) {

		return distortion;

	} else {

      /* consolidate the nonempty codewords at the beginning of the

	 array with the most populous cells first. */

      for(n = 0; n < size - emptycells; n++) {

	j = 0;

	bestcodeword = 0;

	for (i = 0; i < size; i++) {

	  if (count[i] > j) {

	    j = count[i];

	    bestcodeword = i;

	  }

	}

	

	for (j = 0; j < dimension; j++ ) { /* find centroid */

	  codebook[n][j] = centroid[bestcodeword][j] / 

	    (VQDATA) count[bestcodeword];

	  centroid[bestcodeword][j] = 0.0;

	}

	count[bestcodeword] = 0;      

      }

      

      /* try getting new codewords */

//      if (emptycells > 1) {

//	fprintf(stderr,"%s %d %s %d\n",TRYFILL,emptycells,EMPTYCELLS,size);

 //    }

//      else {

//	fprintf(stderr,"%s %d %s %d\n",TRYFILL,emptycells,EMPTYCELL,size);

 //    }

//      fflush(stderr);

      

      /* split the required number of codewords */

      splitcodewords(codebook, size - emptycells, size, pass);

      olddistortion = distortion;

      pass++;

    }

	}

  } while (TRUE);



	return 1;

}

