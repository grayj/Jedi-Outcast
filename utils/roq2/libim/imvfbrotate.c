/**

 **	$Header: /roq/libim/imvfbrotate.c 1     11/02/99 4:38p Zaphod $

 **	Copyright (c) 1989-1995  San Diego Supercomputer Center (SDSC)

 **		a division of General Atomics, San Diego, California, USA

 **

 **	Users and possessors of this source code are hereby granted a

 **	nonexclusive, royalty-free copyright and design patent license to

 **	use this code in individual software.  License is not granted for

 **	commercial resale, in whole or in part, without prior written

 **	permission from SDSC.  This source is provided "AS IS" without express

 **	or implied warranty of any kind.

 **

 **	For further information contact:

 **		E-Mail:		info@sds.sdsc.edu

 **

 **		Surface Mail:	Information Center

 **				San Diego Supercomputer Center

 **				P.O. Box 85608

 **				San Diego, CA  92186-9784

 **				(619) 534-5000

 **/



#define HEADER	"    $Header: /roq/libim/imvfbrotate.c 1     11/02/99 4:38p Zaphod $ "



/**

 **  FILE

 **	imvfbrotate.c		-  Rotate a VFB by a certain angle 

 **

 **  PROJECT

 **	libim		-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imvfbrotate.c contains code to rotate an image.

 **     	It also has the code to shear an image in the x or y direction

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **	ImVfbRotate	f  rotate the vfb

 **	ImVfbXShear	f  shear the vfb in the x direction

 **	ImVfbYShear	f  shear the vfb in the y direction

 **

 **  PRIVATE CONTENTS

 **	none

 **

 **  HISTORY

 **	$Log: /roq/libim/imvfbrotate.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.10  1995/06/30  22:12:11  bduggan

 **	added some casts

 **

 **	Revision 1.9  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.8  1995/06/16  09:01:05  bduggan

 **	added some casts

 **

 **	Revision 1.7  94/10/03  11:29:57  nadeau

 **	Updated to ANSI C and C++ compatibility.

 **	Removed all use of register keyword.

 **	Minimized use of custom SDSC types (e.g., uchar vs. unsigned char)

 **	Changed all float arguments to double.

 **	Added forward declarations.

 **	Added misc. casts to passify SGI and DEC compilers.

 **	Changed all macros and defined constants to have names

 **	starting with IM.

 **	Updated comments.

 **	Updated indenting on some code.

 **	Updated copyright message.

 **	

 **	Revision 1.6  92/12/03  01:56:03  nadeau

 **	Total rewrite.

 **	

 **	Revision 1.5  92/10/19  14:07:23  groening

 **	*** empty log message ***

 **  

 **	Revision 1.4  92/09/17  14:50:14  vle

 **	Added optional include for M_PI declaration to make some

 **	compilers happy.

 **  

 **	Revision 1.3  92/09/03  16:41:14  groening

 **	Added more error checks.

 **  

 **	Revision 1.2  92/09/02  11:17:02  vle

 **	Updated copyright notice.

 **  

 **	Revision 1.1  92/09/02  11:13:57  groening

 **	Initial revision

 **  

 **/



/**

 **  CODE CREDITS

 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1992.

 **/



#include <math.h>



#ifndef M_PI

//#include <values.h>

#define M_PI 3.1415926

#endif



#include "iminternal.h"





/*

 *  FUNCTION

 *	ImVfbXShear	-  shear a vfb in the X-direction

 *

 *  DESCRIPTION

 *	implements this formula x1 = x - tan(degree/2)*y

 *

 */



ImVfb *				/* Returns resized VFB		*/

#ifdef __STDC__

ImVfbXShear( ImVfb *srcVfb, double degree, ImVfb *dstVfb)

#else

ImVfbXShear( srcVfb, degree, dstVfb)

	ImVfb   *srcVfb;	/* VFB to resize		*/

	double             degree;	/* Amount to rotate		*/

	ImVfb            *dstVfb;	/* Result VFB			*/

#endif

{

	ImVfbPtr pdst;			/* Destination VFB pointer	*/

	ImVfbPtr psrc;			/* Destination VFB pointer	*/

	int      i,j;			/* generic integer value	*/

	int      fields;		/* vfb field description	*/

	int	dw, dh;			/* width an height		*/

	int	newX;			/* new x position		*/

	int	xDif;			/* how much wider new vfb is	*/

	double	tandegree;		/* tangent of degree/2		*/





	/*

	 * make sure there is a source vfb then make sure dst vfb

	 * has the same fields as the source.

	 */

        if ( srcVfb == IMVFBNULL )

	{

                ImErrNo = IMENOVFB ;

		return (IMVFBNULL);

	}





	/*

	 *  Floating point modulo 'degree' by 360.0.

	 */

	degree *= 2.0;

	degree -= ((int)(degree / 360.0)) * 360.0;





	/*

	 *  Make sure that a valid shear degree was requested.

	 */

	if ( (degree <= -180.0) || (degree >= 180.0) )

	{

		ImErrNo = IMEIMPSHEAR;

		return (IMVFBNULL);

	}



        if ( degree < 0.0 )

		degree += 360.0;

	degree *= -(M_PI/180);





	/*

	 * figure out the necessary size for the resulting vfb

	 */

	if ( degree > -M_PI )

	{

		dw     = (int)( 0.5 + ImVfbQWidth(srcVfb) -

			tan(degree/2)*ImVfbQHeight( srcVfb )); 

		xDif   = 0;

		dh     = ImVfbQHeight( srcVfb );

	}

	else

	{

		dw     = (int)( 0.5 + ImVfbQWidth(srcVfb) +

			tan(degree/2)*ImVfbQHeight( srcVfb )); 

		xDif   = dw-ImVfbQWidth(srcVfb);

		dh     = ImVfbQHeight( srcVfb );

	}



	/*

	 *  If the user hasn't given us a destination VFB, make one.

	 *  If they have given us a destination VFB, make sure it is usable.

	 */

	fields = ImVfbQFields( srcVfb );



	if ( dstVfb == IMVFBNEW )

	{

		dstVfb = ImVfbAlloc( dw, dh, fields );

		if( dstVfb == IMVFBNULL )

		{

			ImErrNo = IMEMALLOC;

			return( IMVFBNULL );

		}

		ImVfbClear( fields, 0, dstVfb );

	}

	else

	{

		/* make sure that the passed vfb is the correct size	*/

		if ( ImVfbQWidth( dstVfb ) < dw )

		{

			ImErrNo = IMEWIDTH;

			return ( IMVFBNULL );

		}

		if ( ImVfbQHeight( dstVfb ) < dh )

		{

			ImErrNo = IMEHEIGHT;

			return ( IMVFBNULL );

		}

		if (( ImVfbQFields(srcVfb) & ImVfbQFields(dstVfb)) !=

			 ImVfbQFields(srcVfb))	

		{

			ImErrNo = IMEFIELD;

			return (IMVFBNULL);

		}

	}



	/*

	 *  Now shear the vfb

	 */

	tandegree = tan( degree / 2 );

	psrc = ImVfbQFirst(srcVfb);



	for (i=0; i<ImVfbQHeight(srcVfb); i++)

	{

	      for (j=0; j<ImVfbQWidth(srcVfb); j++)

	      {

		newX = (int)(j - tandegree*i + xDif + 0.5);



                pdst = ImVfbQPtr (dstVfb,newX,i);

		if (fields & IMVFBRGB)

		{

                     ImVfbSRed (dstVfb, pdst, ImVfbQRed (srcVfb, psrc) );

                     ImVfbSGreen (dstVfb, pdst,ImVfbQGreen(srcVfb, psrc));

                     ImVfbSBlue (dstVfb, pdst, ImVfbQBlue(srcVfb, psrc) );

                }



		if (fields & IMVFBFDATA)

		{

                     ImVfbSFData (dstVfb, pdst, ImVfbQFData (srcVfb, psrc) );

                }



		if (fields & IMVFBIDATA)

		{

                     ImVfbSIData (dstVfb, pdst, ImVfbQIData (srcVfb, psrc) );

                }



		if (fields & IMVFBWPROT)

		{

                     ImVfbSWProt (dstVfb, pdst, ImVfbQWProt (srcVfb, psrc) );

                }



		if (fields & IMVFBZ)

		{

                     ImVfbSZ (dstVfb, pdst, ImVfbQZ (srcVfb, psrc) );

                }



		if (fields & IMVFBMONO)

		{

                     ImVfbSMono (dstVfb, pdst, ImVfbQMono (srcVfb, psrc) );

                }



		if (fields & IMVFBINDEX8)

		{

                     ImVfbSIndex8 (dstVfb, pdst, ImVfbQIndex8 (srcVfb, psrc) );

                }



		if (fields & IMVFBINDEX16)

		{

                     ImVfbSIndex16 (dstVfb,pdst, ImVfbQIndex16 (srcVfb, psrc) );

                }



		if (fields & IMVFBALPHA)

		{

                     ImVfbSAlpha (dstVfb, pdst, ImVfbQAlpha (srcVfb, psrc) );

                }



		ImVfbSInc(srcVfb,psrc);

	       }

	}



	return( dstVfb );

}





/*

 *  FUNCTION

 *	ImVfbYShear	-  shear a vfb in the x-direction

 *

 *  DESCRIPTION

 *	implements this formula x1 = x - tan(degree/2)*y

 *

 */



ImVfb *				/* Returns resized VFB		*/

#ifdef __STDC__

ImVfbYShear( ImVfb *srcVfb, double degree, ImVfb *dstVfb)

#else

ImVfbYShear( srcVfb, degree, dstVfb)

	ImVfb   *srcVfb;	/* VFB to resize		*/

	double               degree;	/* Amount to rotate		*/

	ImVfb            *dstVfb;	/* Result VFB			*/

#endif

{

	ImVfbPtr pdst;		/* Destination VFB pointer		*/

	ImVfbPtr psrc;		/* Destination VFB pointer		*/

	int      i,j;		/* generic integer value	*/

	int      fields;	/* vfb field description	*/

	int	dw, dh;		/* width an height			*/

	int	newY;		/* new x position			*/

	int	yDif;		/* amount to adjust shear by to get pos. value*/

	double  sindegree;	/* Sine of angle			*/



	/*

	 * make sure there is a source vfb then make sure dst vfb

	 * has the same fields as the source.

	 */

        if ( srcVfb == IMVFBNULL )

	{

                ImErrNo = IMENOVFB ;

		return (IMVFBNULL);

	}





	/*

	 *  Floating point modulo 'degree' by 360.0.

	 */

	degree *= 2.0;

	degree -= ((int)(degree / 360.0)) * 360.0;





	/*

	 *  Make sure that a valid shear degree was requested.

	 */

	if ( (degree <= -180.0) || (degree >= 180.0) )

	{

		ImErrNo = IMEIMPSHEAR;

		return (IMVFBNULL);

	}



        if ( degree < 0.0 )

		degree += 360.0;

	degree *= -(M_PI/180);





	/*

	 * figure out the necessary size for the resulting vfb

	 */

	if ( degree>-M_PI )

	{

		dh     = (int)(( 0.5 + ImVfbQHeight(srcVfb) - 

			sin(degree)*ImVfbQWidth( srcVfb ))); 

		dw     = ImVfbQWidth( srcVfb );

		yDif   = dh-ImVfbQHeight(srcVfb);

	}



	else

	{

		dh     = (int)(( 0.5 + ImVfbQHeight(srcVfb) + 

			sin(degree)*ImVfbQWidth( srcVfb ))); 

		dw     = ImVfbQWidth( srcVfb );

		yDif   = 0;

	}



	/*

	 *  If the user hasn't given us a destination VFB, make one.

	 *  If they have given us a destination VFB, make sure it is usable.

	 */

	fields = ImVfbQFields( srcVfb );

	

	if ( dstVfb == IMVFBNEW )

	{

		dstVfb = ImVfbAlloc( dw, dh, fields );

		if( dstVfb == IMVFBNULL )

		{

			ImErrNo = IMEMALLOC;

			return( IMVFBNULL );

		}

		ImVfbClear( fields, 0, dstVfb );

	}

	else

	{

		/* make sure that passed vfb is the correct size	*/

		if ( ImVfbQWidth( dstVfb ) < dw )

		{

			ImErrNo = IMEWIDTH;

			return ( IMVFBNULL );

		}

		if ( ImVfbQHeight( dstVfb ) < dh )

		{

			ImErrNo = IMEHEIGHT;

			return ( IMVFBNULL );

		}

		if (( ImVfbQFields(srcVfb) & ImVfbQFields(dstVfb)) !=

			 ImVfbQFields(srcVfb))	

		{

			ImErrNo = IMEFIELD;

			return (IMVFBNULL);

		}

	}





	/*

	 *  Now shear the vfb

	 */

	psrc = ImVfbQFirst(srcVfb);

	sindegree = sin( degree );

	for (i=0; i<ImVfbQHeight(srcVfb); i++)

	{

	      for (j=0; j<ImVfbQWidth(srcVfb); j++)

	      {

		newY = (int)(i + sindegree * j + yDif + 0.5);

                pdst = ImVfbQPtr (dstVfb,j,newY);

		if (fields & IMVFBRGB)

		{

                     ImVfbSRed (dstVfb, pdst, ImVfbQRed (srcVfb, psrc) );

                     ImVfbSGreen (dstVfb, pdst,ImVfbQGreen(srcVfb, psrc));

                     ImVfbSBlue (dstVfb, pdst, ImVfbQBlue(srcVfb, psrc) );

                }

		if (fields & IMVFBFDATA)

		{

                     ImVfbSFData (dstVfb, pdst, ImVfbQFData (srcVfb, psrc) );

                }



		if (fields & IMVFBIDATA)

		{

                     ImVfbSIData (dstVfb, pdst, ImVfbQIData (srcVfb, psrc) );

                }



		if (fields & IMVFBZ)

		{

                     ImVfbSZ (dstVfb, pdst, ImVfbQZ (srcVfb, psrc) );

                }



		if (fields & IMVFBALPHA)

		{

                     ImVfbSAlpha (dstVfb, pdst, ImVfbQAlpha (srcVfb, psrc) );

                }



		if (fields & IMVFBWPROT)

		{

                     ImVfbSWProt (dstVfb, pdst, ImVfbQWProt (srcVfb, psrc) );

                }



		if (fields & IMVFBMONO)

		{

                     ImVfbSMono (dstVfb, pdst, ImVfbQMono (srcVfb, psrc) );

                }



		if (fields & IMVFBINDEX8)

		{

                     ImVfbSIndex8 (dstVfb, pdst, ImVfbQIndex8 (srcVfb, psrc) );

                }



		if (fields & IMVFBINDEX16)

		{

                     ImVfbSIndex16 (dstVfb,pdst, ImVfbQIndex16 (srcVfb, psrc) );

                }



		ImVfbSInc(srcVfb,psrc);

	      }

	}



	return( dstVfb );

}









/*

 *  FUNCTION

 *	ImVfbRotate	-  rotate an image

 *

 *  DESCRIPTION

 *	Rotation consists of three shears in X, Y then X again.  We

 *	accomplish this using the ImVfbXShear and ImVfbYShear routines.

 *

 *	The job is complicated by a need to be able to put the rotated

 *	result back into an already existing destination VFB.  That VFB

 *	may already have image data in it:

 *

 *	Source VFB			   +

 *	+---+				 / S \

 *	|SSS| rotated by 45 degrees =   + SSS +

 *	+---+				 \ S /

 *					   +

 *

 *	Desination VFB

 *	+-------+				       +---+---+

 *	|DDDDDDD|				       |D/ S \D|

 *	|DDDDDDD| with the rotated source pasted in =  |+ SSS +|

 *	|DDDDDDD|				       |D\ S /D|

 *	+-------+				       +---+---+

 *

 *	Note that the corners of the final image still have the original

 *	destination image's contents.  Only the central portion has been

 *	overwritten by the rotated source.

 *

 *	In order to paste the rotated source onto the destination, we must

 *	know what pixels of the rotated source came from the original source,

 *	and what pixels are "empty" background pixels.

 *

 *	We could compute these edges and do a simple bresenham line walk to

 *	"fill" a polygon in the destination from the rotated source.

 *	Unfortunately, when we tried that, the bresenham walk computed a

 *	slightly different edge than actually occured from the chain of

 *	shears.  This was not acceptable.

 *

 *	Instead, we now create a dummy monochrome VFB that we initially fill

 *	with 1's.  Then, each time we shear the source image, we also shear

 *	the dummy VFB.  On each shear, the dummy's corners are filled in

 *	with 0's by the shear algorithm.  When we're done we have a rotated

 *	dummy image with a rotated rectangle of 1's amidst a field of 0's.

 *

 *	Finally, this dummy VFB is used as a key to tell us which pixels in

 *	the true rotated source are from the original image.  Those pixels

 *	are copied into the destination and we throw out the dummy.

 *

 *	Rotation by successive shearing works as follows:

 *

 *		1.  Shear in X.

 *		2.  Shear in Y

 *		3.  Trim excess from VFB to minimize VFB size.

 *		4.  Shear in X again.

 *		5.  Trim excess from VFB to minimize VFB size.

 *		6.  Copy across into destination.

 */



ImVfb *				/* Returns rotated VFB		*/

#ifdef __STDC__

ImVfbRotate( ImVfb* sorceVfb, double rotation, ImVfb* dstVfb)

#else

ImVfbRotate( sorceVfb, rotation, dstVfb)

	ImVfb   *sorceVfb;	/* VFB to resize			   */

	double    rotation;	/* Amount to rotate in degrees NOT radians */

	ImVfb   *dstVfb;	/* VFB to resize			   */

#endif

{

	ImVfb	   *tmpVfb;		/* Temporary VFB holder		*/

	ImVfb	   *srcVfb;		/* Temporary VFB copy		*/

	ImVfb	   *dummyVfb;		/* Bogus VFB 			*/



	ImVfbPtr   pSrc, pDst, pDummy;	/* VFB pixel pointers		*/



        int        H,W,Wone,Wtwo,Hone;  /* Heights and widths of vfb's  */

        int        srcXLeft, srcYTop,srcDX, srcDY;

        int        fieldMask;		/* Field mask of incomin vfb	*/

	float	   rad;			/* Radian equivalent of rotation */

	int	   i,j;			/* counters			*/





	/*

	 *  Make sure we have a source image.

	 */

        if ( sorceVfb == IMVFBNULL )

	{

                ImErrNo = IMENOVFB ;

		return (IMVFBNULL);

	}



	fieldMask = ImVfbQFields( sorceVfb );

	W = ImVfbQWidth(sorceVfb);

	H = ImVfbQHeight(sorceVfb);





	/*

	 *  Modulo the rotation angle by 360.0, make it positive,

	 *  and convert to radians.

	 */

	rotation -= ((int)(rotation / 360.0)) * 360.0;

	if ( rotation < 0 )

		rotation += 360.0;

	rad       = rotation*(M_PI/180);





	/*

	 *  Make a destination VFB if one was not given.  Otherwise make

	 *  sure the one given is big enough.

	 */

	srcDX = (int) (0.5 + fabs( sin( rad )*H ) + fabs(cos( rad )*W ));

	srcDY = (int) (0.5 + fabs( sin( rad )*W ) + fabs(cos( rad )*H ));

	if ( dstVfb == IMVFBNEW )

	{

		/*

		 *  Make dstVfb.

		 */

		if ( (dstVfb = ImVfbAlloc( srcDX, srcDY, fieldMask))==IMVFBNULL)

		{

			ImErrNo = IMEMALLOC;

			return (IMVFBNULL);

		}

		ImVfbClear( fieldMask, 0, dstVfb );

	}

	else

	{

		if ( ImVfbQWidth( dstVfb ) != srcDX )

		{

			ImErrNo = IMEWIDTH;

			return (IMVFBNULL);

		}

		if ( ImVfbQHeight( dstVfb ) != srcDY )

		{

			ImErrNo = IMEHEIGHT;

			return (IMVFBNULL);

		}

		if ( (fieldMask & ImVfbQFields(dstVfb)) != fieldMask )

		{

			ImErrNo = IMEFIELD;

			return (IMVFBNULL);

		}

	}





	/*

	 *  Rotations greater than 90 degrees introduce severe aliasing

	 *  effects.  Do 90 degree rotates until the rotation angle is

	 *  less than 90.  Then we'll do the fancy <90 method to do the rest.

	 */

	srcVfb = sorceVfb;

	while ( rotation >= 90.0 )

	{

		if ( (tmpVfb = ImVfb90Rotate( srcVfb, IMVFBNEW )) == IMVFBNULL )

		{

			ImErrNo = IMEMALLOC;

			return (IMVFBNULL);

		}

		rad      -= (M_PI/2.0);

		rotation -= 90.0;

		if ( srcVfb != sorceVfb )

			ImVfbFree( srcVfb );

		srcVfb = tmpVfb;

	}

	W = ImVfbQWidth(  srcVfb );

	H = ImVfbQHeight( srcVfb );



	if ( rotation == 0.0 )

	{

		/*

		 *  Nothing more to do.  Easy!  Copy the rotated image into

		 *  the destination VFB and return.

		 */

		if ( ImVfbCopy( srcVfb , 0, 0,  W, H, fieldMask, 

			dstVfb, 0, 0 ) == IMVFBNULL)

		{

			/* ImErrNo already set.				*/

			if ( srcVfb != sorceVfb )

				ImVfbFree( srcVfb );

			return ( IMVFBNULL );

		}

		if ( srcVfb != sorceVfb )

			ImVfbFree( srcVfb );

		return ( dstVfb );

	}





	/* 

	 *  Compute ultimate image size.

	 */

	Wone = (int) (W + 0.5 + H * fabs(tan(rad/2)));

	Hone = (int) (H + 0.5 + fabs(sin(rad)) * Wone);

	Wtwo = (int) (0.5 + Wone + Hone*fabs(tan(rad/2)));



	srcYTop = (int)(0.5 + fabs(tan(rad/2))*(fabs(sin(rad))*H));

	srcXLeft = (int) (0.5 + fabs(sin(rad)) * ( (W)*fabs(tan(rad/2))));



	srcDX  = (int) (0.5 + fabs(sin(rad))*H + fabs (cos(rad)*W));

	srcDY  = (int) (0.5 + fabs(sin(rad))*W + fabs (cos(rad)*H));

	srcDY +=2;

	if (srcDY>Hone)

		srcDY=Hone;

	if (srcYTop>1.0)

		srcYTop--;

	rotation /= 2.0;





	/*

	 *  Create a dummy monochrome VFB filled with 1's.

	 */

	if ( (dummyVfb = ImVfbAlloc( W, H, IMVFBMONO )) == IMVFBNULL)

	{

		/* ImErrNo already set.					*/

		if ( srcVfb != sorceVfb )

			ImVfbFree( srcVfb );

		return (IMVFBNULL);

	}

	if ( ImVfbFill( dummyVfb, 0, 0, W, H, IMMONO, 1.0, 1.0, 

		IMVFBINSIDE, IMGRADNONE, dummyVfb ) == IMVFBNULL )

	{

		/* ImErrNo already set.					*/

		if ( srcVfb != sorceVfb )

			ImVfbFree( srcVfb );

		ImVfbFree( dummyVfb );

		return (IMVFBNULL);

	}





	/*

	 *  Shear in X.

	 */

	if ( (tmpVfb = ImVfbXShear( srcVfb, rotation, IMVFBNEW ) ) == IMVFBNULL)

	{

		/* ImErrNo already set.					*/

		if ( srcVfb != sorceVfb )

			ImVfbFree( srcVfb );

		ImVfbFree( dummyVfb );

		return (IMVFBNULL);

	}

	if ( srcVfb != sorceVfb )

		ImVfbFree( srcVfb );

	srcVfb = tmpVfb;

	if ( (tmpVfb = ImVfbXShear( dummyVfb, rotation, IMVFBNEW)) == IMVFBNULL)

	{

		/* ImErrNo already set.					*/

		ImVfbFree( srcVfb );

		ImVfbFree( dummyVfb );

		return (IMVFBNULL);

	}

	ImVfbFree( dummyVfb );

	dummyVfb = tmpVfb;





	/*

	 *  Shear in Y.

	 */

	if ( (tmpVfb = ImVfbYShear( srcVfb, rotation, IMVFBNEW ) ) == IMVFBNULL)

	{

		/* ImErrNo already set.					*/

		ImVfbFree( srcVfb );

		ImVfbFree( dummyVfb );

		return (IMVFBNULL);

	}

	ImVfbFree( srcVfb );

	srcVfb = tmpVfb;

	if ( (tmpVfb = ImVfbYShear( dummyVfb, rotation, IMVFBNEW)) == IMVFBNULL)

	{

		/* ImErrNo already set.					*/

		ImVfbFree( srcVfb );

		ImVfbFree( dummyVfb );

		return (IMVFBNULL);

	}

	ImVfbFree( dummyVfb );

	dummyVfb = tmpVfb;





	/*

	 *  Trim off the excess that comes about by increasing the size to

	 *  shear in X, then increasing the size to shear in Y.  We end up

	 *  with a lot of extra "empty" space around the image.  By trimming

	 *  it off here, we save time later by having fewer pixels to move

	 *  about.

	 */

	if ( (tmpVfb = ImVfbCopy( srcVfb, 0, srcYTop, Wone, srcDY, 

		fieldMask, IMVFBNEW, 0, 0)) == IMVFBNULL)

	{

		/* ImErrNo already set.					*/

		ImVfbFree( srcVfb );

		ImVfbFree( dummyVfb );

		return (IMVFBNULL);

	}

	ImVfbFree( srcVfb );

	srcVfb = tmpVfb;

	if ( (tmpVfb = ImVfbCopy( dummyVfb, 0, srcYTop, Wone, srcDY, 

		IMVFBMONO, IMVFBNEW, 0, 0)) == IMVFBNULL)

	{

		/* ImErrNo already set.					*/

		ImVfbFree( srcVfb );

		ImVfbFree( dummyVfb );

		return (IMVFBNULL);

	}

	ImVfbFree( dummyVfb );

	dummyVfb = tmpVfb;





	/*

	 *  Shear in X again.

	 */

	if ( (tmpVfb = ImVfbXShear( srcVfb, rotation, IMVFBNEW ) ) == IMVFBNULL)

	{

		/* ImErrNo already set.					*/

		ImVfbFree( srcVfb );

		ImVfbFree( dummyVfb );

		return (IMVFBNULL);

	}

	ImVfbFree( srcVfb );

	srcVfb = tmpVfb;

	if ( (tmpVfb = ImVfbXShear( dummyVfb, rotation, IMVFBNEW)) == IMVFBNULL)

	{

		/* ImErrNo already set.					*/

		ImVfbFree( srcVfb );

		ImVfbFree( dummyVfb );

		return (IMVFBNULL);

	}

	ImVfbFree( dummyVfb );

	dummyVfb = tmpVfb;





	/*

	 *  Trim off the excess again.

	 */

	if ( (tmpVfb = ImVfbCopy( srcVfb, srcXLeft, 0, srcDX, 

		srcDY, fieldMask, IMVFBNEW, 0, 0 )) == IMVFBNULL )

	{

		/* ImErrNo already set.					*/

		ImVfbFree( srcVfb );

		ImVfbFree( dummyVfb );

		return (IMVFBNULL);

	}

	ImVfbFree( srcVfb );

	srcVfb = tmpVfb;

	if ( (tmpVfb = ImVfbCopy( dummyVfb, srcXLeft, 0, srcDX, 

		srcDY, IMVFBMONO, IMVFBNEW, 0, 0 )) == IMVFBNULL )

	{

		/* ImErrNo already set.					*/

		ImVfbFree( srcVfb );

		ImVfbFree( dummyVfb );

		return (IMVFBNULL);

	}

	ImVfbFree( dummyVfb );

	dummyVfb = tmpVfb;





	/*

	 *  Use the dummyVfb has a mask for the rotated srcVfb to determine

	 *  which pixels of the srcVfb to copy to the dstVfb.  1's in the

	 *  dummyVfb mean copy.  0's mean don't.

	 */

	pSrc   = ImVfbQFirst( srcVfb );

	pDummy = ImVfbQFirst( dummyVfb );

	pDst   = ImVfbQFirst( dstVfb );



	for ( i = 0; i < ImVfbQHeight( dummyVfb ); i++ )

	{

		for ( j = 0; j < ImVfbQWidth( dummyVfb) ; j++ )

		{

			if ( !ImVfbQMono( dummyVfb, pDummy ) )

			{

				/* Don't copy.				*/

				ImVfbSInc( dummyVfb, pDummy );

				ImVfbSInc( srcVfb, pSrc );

				ImVfbSInc( dstVfb, pDst );

				continue;

			}



			/* Do copy.					*/

			if (fieldMask&IMVFBRGB)

			{

				ImVfbSRed( dstVfb, pDst,

					ImVfbQRed( srcVfb, pSrc));

				ImVfbSGreen( dstVfb, pDst,

					ImVfbQGreen( srcVfb, pSrc));

				ImVfbSBlue( dstVfb, pDst,

					ImVfbQBlue( srcVfb, pSrc));

			}



			if (fieldMask&IMVFBZ)

				ImVfbSZ( dstVfb, pDst,

					ImVfbQZ( srcVfb, pSrc ) );

			if (fieldMask&IMVFBWPROT)

				ImVfbSWProt( dstVfb, pDst,

					ImVfbQWProt( srcVfb, pSrc ) );

			if (fieldMask&IMVFBIDATA)

				ImVfbSIData( dstVfb, pDst,

					ImVfbQIData( srcVfb, pSrc ) );

			if (fieldMask&IMVFBFDATA)

				ImVfbSFData( dstVfb, pDst,

					ImVfbQFData( srcVfb, pSrc ) );

			if (fieldMask&IMVFBMONO)

				ImVfbSMono( dstVfb, pDst,

					ImVfbQMono( srcVfb, pSrc ) );

			if (fieldMask&IMVFBALPHA)

				ImVfbSAlpha( dstVfb, pDst,

					ImVfbQAlpha( srcVfb, pSrc ) );

			if (fieldMask&IMVFBINDEX8)

				ImVfbSIndex8( dstVfb, pDst,

					ImVfbQIndex8( srcVfb, pSrc ) );

			if (fieldMask&IMVFBINDEX16)

				ImVfbSIndex16( dstVfb, pDst,

					ImVfbQIndex16( srcVfb, pSrc ) );

			ImVfbSInc( dummyVfb, pDummy );

			ImVfbSInc( srcVfb, pSrc );

			ImVfbSInc( dstVfb, pDst );

		}

	}



	ImVfbFree( srcVfb );

	ImVfbFree( dummyVfb );



	return( dstVfb );

}











/*

 *  FUNCTION

 *	ImVfb90Rotate	-  rotate by 90 degrees

 *

 *  DESCRIPTION

 *	When rotating by exactly 90 degrees counter-clockwise (right-hand

 *	rule), we can do the rotation quicker than by using 3 shears.

 */



ImVfb *				/* Returns rotated VFB		*/

#ifdef __STDC__

ImVfb90Rotate( ImVfb* sourceVfb, ImVfb* dstVfb )

#else

ImVfb90Rotate( sourceVfb, dstVfb )

	ImVfb     *sourceVfb;	/* VFB to rotated			*/

	ImVfb     *dstVfb;	/* VFB to return rotated		*/

#endif

{

	ImVfbPtr   psrc;	/* pointer into source vfb		*/

	ImVfbPtr   pdst;	/* pointer into destination vfb		*/

	int	   i,j;		/* Counters				*/

	int	   wDst, hDst;	/* Destination width and height		*/

	int	   fieldMask;	/* Fields to deal with			*/





	/*

	 *  Get destination width and height... reversed from source's

	 *  `cause we're rotating by 90.0.

	 */

	hDst      = ImVfbQWidth(  sourceVfb );

	wDst      = ImVfbQHeight( sourceVfb );

	fieldMask = ImVfbQFields( sourceVfb );





	/*

	 *  If a destination VFB wasn't given, allocate one at the same

	 *  size and depth as the source.  Otherwise make sure the desination

	 *  is the same size as the source and has at least the same fields

	 *  as the source.

	 */

	if (dstVfb == IMVFBNEW)

	{

		if ( (dstVfb = ImVfbAlloc( wDst, hDst, fieldMask )) ==IMVFBNULL)

		{

			ImErrNo = IMEMALLOC;

			return (IMVFBNULL);

		}

	}

	else

	{

		if ( ImVfbQWidth( dstVfb ) != wDst )

		{

			ImErrNo = IMEWIDTH;

			return (IMVFBNULL);

		}

		if ( ImVfbQHeight( dstVfb ) != hDst )

		{

			ImErrNo = IMEHEIGHT;

			return (IMVFBNULL);

		}

		if ( (fieldMask & ImVfbQFields(dstVfb)) != fieldMask )

		{

			ImErrNo = IMEFIELD;

			return (IMVFBNULL);

		}

	}





	/*

	 *  Walk the image and rotate it by 90 into the destination VFB.

	 */

	psrc = ImVfbQFirst( sourceVfb );

	for ( i = 0; i < wDst; i++ )

	{

	   for ( j = hDst-1; j >= 0; j-- )

	   {

		pdst = ImVfbQPtr( dstVfb, i, j );



		if (fieldMask&IMVFBRGB)

		{

		   ImVfbSRed (dstVfb, pdst, ImVfbQRed(sourceVfb, psrc));

		   ImVfbSGreen (dstVfb, pdst, ImVfbQGreen(sourceVfb, psrc));

		   ImVfbSBlue (dstVfb, pdst, ImVfbQBlue(sourceVfb, psrc));

		}



		if (fieldMask&IMVFBZ)

		{

		   ImVfbSZ (dstVfb, pdst, ImVfbQZ(sourceVfb, psrc));

		}



		if (fieldMask&IMVFBWPROT)

		{

		   ImVfbSWProt (dstVfb, pdst, ImVfbQWProt(sourceVfb, psrc));

		}



		if (fieldMask&IMVFBIDATA)

		{

		   ImVfbSIData (dstVfb, pdst, ImVfbQIData(sourceVfb, psrc));

		}



		if (fieldMask&IMVFBFDATA)

		{

		   ImVfbSFData (dstVfb, pdst, ImVfbQFData(sourceVfb, psrc));

		}



		if (fieldMask&IMVFBMONO)

		{

		   ImVfbSMono (dstVfb, pdst, ImVfbQMono(sourceVfb, psrc));

		}



		if (fieldMask&IMVFBALPHA)

		{

		   ImVfbSAlpha (dstVfb, pdst, ImVfbQAlpha(sourceVfb, psrc));

		}



		if (fieldMask&IMVFBINDEX8)

		{

		   ImVfbSIndex8 (dstVfb, pdst, ImVfbQIndex8(sourceVfb, psrc));

		}



		if (fieldMask&IMVFBINDEX16)

		{

		   ImVfbSIndex16 (dstVfb, pdst, ImVfbQIndex16(sourceVfb, psrc));

		}



		ImVfbSInc( sourceVfb, psrc );

	   }

	}



	return (dstVfb);

}

