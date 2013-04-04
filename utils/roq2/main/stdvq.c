/******************************************************************************

 *

 * NAME

 *    stdvq.c

 *    J. R. Goldschneider 5/93

 *

 * MODIFICATIONS

 *    11/93 modified to display defaults values. The -s option was added so

 *    that three programs were no inter maintained.  Also a change made

 *    so that any time the distortion is found to be zero, the program

 *    exits gracefully. JRG

 *

 * SYNOPSIS

 *    stdvq -i f1 -c f2 -d dimension -f codebooksize -h threshold 

 *           -a addoffset -m muloffset -s speedup -W

 *

 * DESCRIPTION

 *    stdvq forms codebooks using the generalized lloyd algorithm.  It obtains

 *    user input, verifies the values, checks to see if the training file

 *    exists, and runs the GLA for codebooks of size 2^n n = 0,1,2,... 

 *    until the final size is reached. The final size can be any integer value.

 *    Each increase in the size of the codebook is done by splitting codewords 

 *    of the next smallest codebook, (perturbed versions of the old codewords).

 *    The GLA continues to run until the (percent) change in distortion

 *    is less than threshold. The GLA will abort if there are cells which 

 *    cannot be filled.  If there are empty cells, the lloyd iteration tries

 *    to split the most populous cells only, (individual cell distortion

 *    is not considered). There are three options to speed up the lloyd 

 *    algorithm. The first uses partial distortion for speedup.  The second two

 *    use constraints imposed by ordering the codewords.  See those programs

 *    for more detail.  All three versions use the mean square error as

 *    the metric.

 *

 * OPTIONS

 *    -i  training sequence file name (input)

 *    -c  codebook file name (output)

 *    -d  data vector dimension

 *    -f  final codebook size

 *    -h  convergence threshold

 *    -a  codeword split additive offset

 *    -m  codework split multiplicative offset

 *    -s  option to select type of constrained search for lloyd algorithm.

 *

 * FLAGS

 *    -W  write all codebooks of size power of two and final codebook size

 *

 *    See vq.h for definitions of default values

 *

 * CALLS

 *    lloyd(), splitcodewords(), writecodebook()

 *

 *****************************************************************************/



#include <stdio.h>

#include <string.h>

#include <math.h>

#include <stdlib.h>



#include "vq.h"

#include "stdvq.def"

#include "stdvq.h"



extern VQDATA lloyd0();

extern VQDATA lloyd1();

extern VQDATA lloyd2();



unsigned char *dimage;

int *dcoords;

int dtrainsize;

int onvector = 0;

int inquadrant = 0;

int pixelsWide;



void vectorreset() {

	onvector = 0;

	inquadrant = 0;

}



VQDATA getnextvector( DATA *vector ) {

int x,y,s,xx,yy,ov,temp = 0;

VQDATA dtemp;



	if (onvector>=dtrainsize) return -1;



	x = dcoords[onvector] & 0xffff;

	y = dcoords[onvector] >> 16;

	s = 4; if (dimension == 12 || dimension == 16) s = 2;

	ov = 0;



	if (s == 4) {

		for(yy=y;yy<(y+s);yy++) {

		for(xx=x;xx<(x+s);xx++) {

			temp = yy*pixelsWide+xx*4;

			vector[ov++] = (DATA)dimage[temp++];

			vector[ov++] = (DATA)dimage[temp++];

			vector[ov++] = (DATA)dimage[temp++];

			if (dimension == 64) vector[ov++] = (DATA)dimage[temp];

		}

		}

		onvector++;

	} else {

		if ((inquadrant == 2) || (inquadrant == 3) ) y += 2;

		if ((inquadrant == 1) || (inquadrant == 3) ) x += 2;

		for(yy=y;yy<(y+s);yy++) {

		for(xx=x;xx<(x+s);xx++) {

			temp = yy*pixelsWide+xx*4;

			vector[ov++] = (DATA)dimage[temp++];

			vector[ov++] = (DATA)dimage[temp++];

			vector[ov++] = (DATA)dimage[temp++];

			if (dimension == 16) vector[ov++] = (DATA)dimage[temp];

		}

		}

		inquadrant++;

		if (inquadrant == 4) { onvector++; inquadrant = 0; }

	}

	dtemp = 0.0;

	for(x=0;x<dimension;x+=4) {

		dtemp += vector[x+0]*vector[x+0];

		dtemp += vector[x+1]*vector[x+1];

		dtemp += vector[x+2]*vector[x+2];

		dtemp += vector[x+3]*vector[x+3];

	}

	dtemp = sqrt( dtemp );

	return dtemp;

}



int stdvq( int idimension, int icodebooksize, unsigned char *image, int ipixelsWide, int *coords, int trainsize, VQDATA **icodebook, int gotcodes, float *flist )

{

  VQDATA **codebook;  /* codebook array */

  VQDATA (*method)(); /* pointer to functions lloyd0, lloyd1, and lloyd2 */

  VQDATA distortion;  /* ave. distortion between codebook and training set */

  int   i,j;         /* counters */

  int    speedup;     /* indicates which type of speedup to use */





	dimage = image;

	dcoords = coords;

	dtrainsize = trainsize;

	codebook = icodebook;

	pixelsWide = ipixelsWide;



  /* set default values */



  sprintf(trainingname,DEF_INNAME);

  sprintf(codebookname,DEF_OUTNAME);

  dimension = idimension;		//DEF_DIMENSION;

  codebooksize = icodebooksize;	//DEF_CODEBOOKSIZE;

  distortion = -1;

  threshold = DEF_THRESHOLD;

  offset_add = DEF_OFFSET_ADD;

  offset_mul = DEF_OFFSET_MUL;

  write_all_codebooks = FALSE;

  sprintf(programname,"roq");

  speedup = DEF_SPEEDUP;



  

  /* find speedup option */

  switch (speedup)

    {

    case 0:

      method = lloyd0;

      break;

    case 1:

      method = lloyd1;

      break;

    case 2:

      method = lloyd2;

      break;

    default:

      fprintf(stderr,"%s %s %s\n",USAGE,programname,HOWTOUSE_STDVQ);

      exit(7);

    }

    

//  /* display all of the user's input values */

//  fprintf(stdout,"\n%s: -i  %s -c %s -d %d -f %d -h %g -a %g -m %g -s %d ",

//	  programname,trainingname,codebookname,dimension,codebooksize,

//	  threshold,offset_add,offset_mul,speedup);

//  fprintf(stdout,"\n\n");



  if (!gotcodes) {

	/* perform generalize lloyd algorithm on all codebook sizes */

  for(i = 1; i < codebooksize;) {



	onvector = 0;

    /* run the GLA for codebook of size i */

    if ( (distortion = method(codebook,i)) < 0) {

      exit(13);

    }



    /* if distortion is zero, no need to continue. 

       note that lloyd can and will change codebooksize in such a case */

    if (distortion == 0) break;



//    /* display the distortion of the training set to the codebook of size i */

//

    printf("stdvq: %s %-7d:  %f\n",DISTORTION,i,distortion);

//    fflush(stdout);





    /* find the number of new codewords that need to be made (j-i) */

    if ((j = 2*i) > codebooksize) j = codebooksize;



    /* split the codewords */

    splitcodewords(codebook,i,j,0);



    /* increment the codebook size */

    i = j;

  }

	onvector = 0;



  /* it may be that distortion is 0, so we can exit early */

  if (distortion == 0) {

			printf("going to do it one last time\n");

			codebooksize = 256;

//  		printf("stdvq: %s %-7d:  %f\n",DISTORTION,codebooksize,distortion);

//		return 0;	

  }

	}

  /* do the final codebook */

  if ( (distortion = method(codebook,codebooksize)) < 0) exit(16);

  printf("stdvq: %s %-7d:  %f\n",DISTORTION,codebooksize,distortion);



	return 0;



}

