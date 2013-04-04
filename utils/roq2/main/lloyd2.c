/******************************************************************************

 *

 * NAME

 *    lloyd2.c

 *    J. R. Goldschneider 11/93

 *

 * MODIFICATIONS

 *

 * SYNOPSIS

 *    lloyd2(codebook,size)

 *

 * DESCRIPTION

 *    lloyd2 performs the generalized lloyd algorithm for the given 

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

 *    using the annulus and sphere constrained method of Huang, Bi, Stiles, 

 *    and Harris (algorithm III) of the IEEE Transactions on Image 

 *    Processing July 1992.  

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



VQDATA lloyd2(codebook,size)

     VQDATA **codebook;

     int   size;



{

  VQDATA  distortion;     /* distortion between training set and codebook */

  VQDATA  olddistortion;  /* distortion from the previous pass of GLA */

  VQDATA  bestdistortion; /* distortion between 1 vector and best codeword */

  VQDATA  tempnorm;       /* temporary variable */

  VQDATA  tempfloat;     /* temporary variable */

  VQDATA  temp;           /* temporary variable */

  int    numbervectors;  /* number of vectors used */

  int    i,j,n,imin,imax;/* counters and indices */

  int    pass;           /* the number of attempts to split empty cells */

  int    bestcodeword;   /* index of the best codeword for a given vector */

  int    bestnorm;       /* used as an index for sorting norm */

  int    emptycells;     /* number of empty cells */

  int    *count;         /* array of number of vectors per region */

  VQDATA  **centroid;     /* centroids of each voronoi region */

  VQDATA  **dist_matrix;  /* MSE distances between all codewords */

  VQDATA  *norm;          /* array of norms of each codeword */

  VQDATA  *tempcodeword;  /* temporary pointer to a codeword */

  DATA    *tempvector;    /* the training vector */



  /* allocate memory for the centroids, temporary vector, and count vector */

  if (!(centroid = (VQDATA **) calloc(size,sizeof(VQDATA *))) ||

      !(tempvector = (DATA *) calloc(dimension, sizeof(DATA))) ||

      !(norm  = (VQDATA *) calloc(size,sizeof(VQDATA))) ||

      !(tempcodeword = (VQDATA *) calloc(dimension,sizeof(VQDATA))) ||      

      !(count = (int *) calloc(size, sizeof(VQDATA))) ||

      !(dist_matrix  = (VQDATA **) calloc(size,sizeof(VQDATA *)))) {

    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);

    return(-1.0);

  }



  /* allocate memory for the dimension of each centroid, and initialize the

   * the centroids and countnumber */ 

  for(i=0; i < size; i++) {

    if (!(centroid[i] = (VQDATA *) calloc(dimension,sizeof(VQDATA))) ||

	!(dist_matrix[i] = (VQDATA *) calloc(size,sizeof(VQDATA)))) {

      fprintf(stderr,"%s: %s\n",programname,NOMEMORY);

      return(-1.0);

    }

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



    /* use the sphere and annulus method to constrain the search space */

    /* start by finding the norm of each codeword and storing it in

       an array */

    for (i = 0; i < size; i++) {

      norm[i] = 0.0;

      for (j = 0; j < dimension; j++) {

	norm[i] += codebook[i][j]*codebook[i][j];

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

      

      tempfloat = norm[i];

      norm[i] = norm[bestnorm];

      norm[bestnorm] = tempfloat;

    }



    /* find the table of distortions between all of the codewords */

    for (i = 0; i < size; i++) {

      for (j = i; j < size; j++) {

	tempfloat = 0.0;

	for (n = 0; n < dimension; n++) {

	  temp = ( (VQDATA) codebook[i][n]) - codebook[j][n];

	  tempfloat += temp*temp;

	}    

	tempfloat = sqrt(tempfloat);

	dist_matrix[i][j] = tempfloat;

	dist_matrix[j][i] = tempfloat;

      }

    }



    /* compute distortion */

    distortion = 0.0;



	vectorreset();



    /* read in vector and find the closest codeword */

    while ( (tempnorm = getnextvector(tempvector))>=0 ) {





//      /* compute the norm of the input vector */

//      tempnorm = 0.0;

//      for (j = 0; j < dimension; j++) {

//	tempnorm += (VQDATA)tempvector[j]*tempvector[j];

 //     }

 //     tempnorm = sqrt(tempnorm);

      

      /* find the codework with a norm closest to the input vector */

      bestcodeword = min_sup(0,size-1,tempnorm,norm);

      

      /* compute the distance between the codeword and the input vector that

	 has the closest norm */

      tempfloat = 0.0;

      for (j = 0; j < dimension; j++) {

	temp = ( (VQDATA) tempvector[j]) - codebook[bestcodeword][j];

	tempfloat += temp*temp;

      }    

      tempfloat = sqrt(tempfloat);



      /* identify the subset of the codewords to search such that 

	 norm(input)-distance <= norm(codeword) <= norm(input)+distance */

      imin = min_sup(0, size-1, tempnorm-tempfloat, norm);

      imax = max_inf(imin, size-1, tempnorm+tempfloat, norm);



      /* find the bestcodeword using partial distortion method */

      bestdistortion = HUGE; /* keep convention that ties go to lower index */

      for (i = imin; i <= imax; i++){ /* find the best codeword */

	if (dist_matrix[i][bestcodeword] <= 2*sqrt(bestdistortion)) {

	  tempfloat = 0.0;

	  for (j = 0; j < dimension; j++) {

	    temp = ( (VQDATA) tempvector[j]) - codebook[i][j];

	    tempfloat += temp*temp;

	    if (tempfloat > bestdistortion) j = dimension; /* abort loop */

	  }

	  

	  if (tempfloat < bestdistortion) {

	    bestdistortion = tempfloat;

	    bestcodeword = i;

	  }

	  

	  /* if bestidistortion is 0 then the best codeword has been found */

	  if (bestdistortion == 0.0) i=size;

	}

      }

      count[bestcodeword]++;

      for (j = 0; j < dimension; j++){

	centroid[bestcodeword][j] += (VQDATA) tempvector[j];

      }

      distortion += bestdistortion;

    }    /* all training vectors have been encoded */



    /* normalize the distortion */

    numbervectors = 0;

    for (i = 0; i < size; i++) {

      numbervectors += count[i];

    }

    if(numbervectors == 0) {

      fprintf(stderr,"%s: %s: %s\n",programname,trainingname,NOTFOUND);

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

	if (emptycells > 1) {

	  fprintf(stderr,"%s %d %s %d\n",NOFILL,emptycells,EMPTYCELLS,size);

	}

	else {

	  fprintf(stderr,"%s %d %s %d\n",NOFILL,emptycells,EMPTYCELL,size);

	}

	fprintf(stderr,"%s\n",ABORT_STDVQ);

	return(-1.0);

      }



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

      if (emptycells > 1) {

	fprintf(stderr,"%s %d %s %d\n",TRYFILL,emptycells,EMPTYCELLS,size);

      }

      else {

	fprintf(stderr,"%s %d %s %d\n",TRYFILL,emptycells,EMPTYCELL,size);

      }

      fflush(stderr);

      

      /* split the required number of codewords */

      splitcodewords(codebook, size - emptycells, size, pass);

      olddistortion = distortion;

      pass++;

    }



  } while (TRUE);

}

