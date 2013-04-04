/******************************************************************************

 *

 * NAME

 *    stdvq_util.c

 *    J. R. Goldschneider 5/93

 *

 * MODIFICATIONS

 *    7/93 min_sup and max_inf added for constrained searches. JRG

 *

 * SYNOPSIS

 *    splitcodewords(codebook,oldsize,newsize)

 *    perturb(oldcodeword,newcodeword)

 *    min_sup(first, last, tempnorm, norm)

 *    max_inf(first, last, tempnorm, norm)

 *    writecodebook(codebook,size)

 *

 * DESCRIPTION

 *    see below

 *    

 * RETURN VALUE

 *    see below

 *    

 * PARAMETERS

 *    see below

 *

 * CALLS

 *

 *****************************************************************************/



#include <stdio.h>

//#include <libc.h>

#include <string.h>

#include <math.h>

#include <stdlib.h>

#include "vq.h" 

#include "stdvq.def" 

#include "stdvq.h" 

#include "gdefs.h"

/******************************************************************************

 *

 *    splitcodewords takes a codebook and creates oldsize-newsize new codewords

 *    from the old codewords.  The old codewords are not modified.

 *  

 *****************************************************************************/



void splitcodewords(VQDATA **codebook,int oldsize,int newsize,int scale)

{

  int    i, j;

  void   perturb();



  /* create the new codewords */

  for (i = 0; i < newsize - oldsize; i++) {

	j = i; if (scale) j = scale;

    perturb(codebook[j], codebook[i], codebook[i+oldsize],scale); 

  }

}



/******************************************************************************

 *

 *    perturb takes oldcodeword and changes is slightly to form

 *    newcodeword.  oldcodeword is not altered in the process.

 *    scale is available to change so that a codeword can be split many times

 *    if necessary and still have each resulting new codeword be different.

 * 

 *****************************************************************************/



void perturb(oldcodeword,nextcodeword, newcodeword,scale)

     VQDATA *oldcodeword;

     VQDATA *nextcodeword;

     VQDATA *newcodeword;

     int   scale;

{

  int i;

  float  addoffset, muloffset;



  addoffset = offset_add/(pow(2.0, (VQDATA) scale));

  muloffset = offset_mul/(pow(2.0, (VQDATA) scale));



  for (i = 0; i < dimension; i++) {

    if (oldcodeword[i] == 0.0) {

      newcodeword[i] = addoffset*((VQDATA) random())/2147483647.0;

    }

    else if (fabs(oldcodeword[i]) < 0.9*addoffset) {

      newcodeword[i]=oldcodeword[i]+addoffset*

	(fabs(oldcodeword[i])/oldcodeword[i])*((VQDATA) random())/2147483647.0;

    }

    else {

      newcodeword[i]=oldcodeword[i]+muloffset*

	oldcodeword[i]*((VQDATA) random())/2147483647.0;

    }

  }

}



/******************************************************************************

 *

 *    min_sup returns the index of the smallest element of norm which is

 *    greater than tempnorm.  If no element is found, then the last index 

 *    is returned. 

 *

 *    first is the lower index of the array. 

 *    last is the upper index of the array.

 *    tempnorm is the comparison value.

 *    norm is the array to search.

 * 

 *****************************************************************************/



int  min_sup(first, last, tempnorm, norm)

     int   first;

     int   last;

     VQDATA tempnorm;

     VQDATA *norm;

{

  if (first == last) return(first);

  if (tempnorm > norm[(first + last)/2] )

    return(min_sup( (first + last)/2 + 1, last, tempnorm, norm));

  else

    return(min_sup( first, (first + last)/2, tempnorm, norm));

}



/******************************************************************************

 *

 *    max_inf returns the index of the largest element of norm which is

 *    less than tempnorm.  If no element is found, then the lowest index 

 *    is returned.

 * 

 *    first is the lower index of the array. 

 *    last is the upper index of the array.

 *    tempnorm is the comparison value.

 *    norm is the array to search.

 * 

 *****************************************************************************/



int  max_inf(first, last, tempnorm, norm)

     int   first;

     int   last;

     VQDATA tempnorm;

     VQDATA *norm;

{

  if (first == last) return(first);

  if (tempnorm > norm[(first + last + 1)/2] )

    return(max_inf( (first + last + 1)/2, last, tempnorm, norm));

  else

    return(max_inf( first, (first + last - 1)/2, tempnorm, norm));

}



