/**

 **	$Header: /roq/libim/imvfbflip.c 1     11/02/99 4:38p Zaphod $

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

 **				San Diego, CA  92138-5608

 **				(619) 534-5000

 **/



#define HEADER	"    $Header: /roq/libim/imvfbflip.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imvfbflip.c	-  Function to Flip a Virtual Frame Buffer

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imvfbflip.c contains the routine to flip a vfb for

 **	the IM package.

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **	ImVfbFlip	f  flip a VFB

 **

 **  PRIVATE CONTENTS

 **	none

 **

 **  HISTORY

 **	$Log: /roq/libim/imvfbflip.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.9  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.8  1995/06/16  08:52:59  bduggan

 **	changed bcopy to memcpy

 **

 **	Revision 1.7  94/10/03  11:29:49  nadeau

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

 **	Revision 1.6  92/08/31  17:38:04  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.5  91/09/18  17:26:52  mcleodj

 **	Modified routine to correctly handle destination`

 **	vfb's that are not equivalent to the source vfb.

 **	

 **	Revision 1.4  91/09/13  14:50:07  nadeau

 **	Removed unnecessary inclusion of <stdio.h>

 **	

 **	Revision 1.3  91/07/12  10:31:21  nadeau

 **	Removed extra include statement.

 **	

 **	Revision 1.2  90/07/23  13:55:39  nadeau

 **	Totally rewrote.  The original version wouldn't compile.  If it had

 **	compiled, it wouldn't have linked.  If it had linked, it wouldn't

 **	have properly flipped images.  If it had properly flipped images,

 **	it would have done it in the slowest possible way.  The shear

 **	stupidity of the original version is mind-boggeling.

 **	

 **	Revision 1.1  90/03/06  17:33:56  mercurio

 **	Initial revision

 **	

 **/



/**

 **  CODE CREDITS

 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1990.

 **/



#include "iminternal.h"











/*

 *  FUNCTION

 *	ImVfbFlip	-  flip a VFB

 *

 *  DESCRIPTION

 *	If no destination VFB is given, a new destination VFB is created.

 *

 *	The source VFB is then copied into the destination VFB, flipping

 *	its pixels horizontally, vertically, or both.

 */



ImVfb *			/* Returns flipped VFB			*/

#ifdef __STDC__

ImVfbFlip( ImVfb   *vfbSrc, int flipDirection, ImVfb   *vfbDst )

#else

ImVfbFlip( vfbSrc, flipDirection, vfbDst )

	ImVfb   *vfbSrc;	/* VFB to flip				*/

	int      flipDirection;	/* Direction to flip			*/

	ImVfb   *vfbDst;	/* VFB to overwrite with flipped source	*/

#endif

{

	int      x, y;		/* column, row indices			*/

	int      hw;		/* Half image width			*/

	int      hh;		/* Half image height			*/

	int      wm1;		/* Width of image, minus 1		*/

	int      hm1;		/* Height of image, minus 1		*/

	int      nBytes;	/* # bytes per pixel			*/

	unsigned char   *pixel;	/* Temporary pixel holder		*/

	ImVfbPtr pSrc, pDst;	/* src, dst pixel pointers		*/





	/* Check the flip direction					*/

	if ( flipDirection != IMVFBXFLIP && flipDirection != IMVFBYFLIP &&

	     flipDirection != IMVFBXYFLIP )

	{

		ImErrNo = IMEBADFLIP;

		return( IMVFBNULL );

	}





	/*  Allocate a new VFB, if necessary				*/

	if ( vfbDst == IMVFBNEW )

	{

		vfbDst = ImVfbAlloc( ImVfbQWidth( vfbSrc ),

			ImVfbQHeight( vfbSrc ), ImVfbQFields( vfbSrc ) );

		if ( vfbDst == IMVFBNULL )

		{

			ImErrNo = IMEMALLOC;

			return( IMVFBNULL );

		}



		/* 

		 * For a new vfb, All the infomation (including any colortables)

		 * in the source vfb must be re-written to the destination 

		 * in a flipped order therfore the range of pixel information

		 * to parse (ie: hh and hw) is the entire source vfb 

		 * range (ie: h x w)

		 */

		ImVfbSClt( vfbDst, ImVfbQClt( vfbSrc ) );

		hh = ImVfbQHeight( vfbDst );

		hw = ImVfbQWidth( vfbDst );

	}

	else if ( vfbDst == vfbSrc )

	{

		/*

		 * If the request is to replace the source Vfb with the flipped

		 * pixel information thereby overwriting the original pixel 

		 * data, a short cut can be performed.  Only half of the pixels

		 * need to be parsed through to achieve the flipped effect

		 * NOTE: the color table ( if any ) automatically remains 

		 * attached to the source vfb.

		 */

		hh = ImVfbQHeight( vfbDst )/2;

		hw = ImVfbQWidth( vfbDst )/2;



	}

	else if ( ImVfbQWidth( vfbSrc )  != ImVfbQWidth( vfbDst )  ||

		  ImVfbQHeight( vfbSrc ) != ImVfbQHeight( vfbDst ) ||

		  ImVfbQFields( vfbSrc ) != ImVfbQFields( vfbDst )    )

	{

		/* If different VFB's, must have the same size.		*/

		ImErrNo = IMEDIFFSIZE;

		return ( IMVFBNULL );

	}

	else

	{

		/* 

		 * If the request is to overwrite a destination Vfb that

		 * has already been allocated before the request and

		 * is the same size as the source, then the full parsing

		 * of pixel data must occur.

		 */

		hh = ImVfbQHeight( vfbDst );

		hw = ImVfbQWidth( vfbDst );

	}



	/* Allocate temporary space for one pixel.			*/

	nBytes = ImVfbQNBytes( vfbSrc );

	if ( (pixel = (unsigned char *) malloc( nBytes )) == NULL )

	{

		ImErrNo = IMEMALLOC;

		return( IMVFBNULL );

	}



	/* Flip the source into the destination.			*/

	if ( flipDirection == IMVFBXFLIP || flipDirection == IMVFBXYFLIP )

	{

		/* Flip horizontally.					*/

		wm1 = ImVfbQWidth( vfbDst ) - 1;



		for ( y = 0; y < ImVfbQHeight( vfbSrc ); y++ )

		{

			pSrc = ImVfbQPtr( vfbSrc, 0,   y );

			pDst = ImVfbQPtr( vfbDst, wm1, y );

			for ( x = 0; x < hw; x++, ImVfbSRight( vfbSrc, pSrc ),

				ImVfbSLeft( vfbDst, pDst ) )

			{

				memcpy( pixel, (void*) pSrc, nBytes );

				memcpy( (void *) pSrc, pDst,  nBytes );

				memcpy( (void *) pDst, pixel, nBytes );

			}

		}



		if ( flipDirection == IMVFBXFLIP )

		{

			free( (char *)pixel );

			return ( vfbDst );

		}



		/* 

		 * If the execution reaches this point, then a flip of the

		 * pixel data in the y direction must be performed.

		 * Execution time can be saved by making the previously

		 * x-flipped destination vfb the source vfb, in which

		 * case the amount of pixel data of the vfb's height to parse

		 * must be reduced in half.

		 * NOTE: this is only necessary for requests to flip a source

		 * vfb into a new destination vfb .

		 */	

		vfbSrc = vfbDst;

		hh = ImVfbQHeight( vfbDst )/2;

	}





	/* Flip vertically.						*/

	hm1 = ImVfbQHeight( vfbDst ) - 1;

	for ( x = 0; x < ImVfbQWidth( vfbSrc ); x++ )

	{

		pSrc = ImVfbQPtr( vfbSrc, x, 0 );

		pDst = ImVfbQPtr( vfbDst, x, hm1 );

		for ( y = 0; y < hh; y++, ImVfbSDown( vfbSrc, pSrc ),

			ImVfbSUp( vfbDst, pDst ) )

		{

			memcpy( pixel, pSrc, nBytes );

			memcpy( (void *)pSrc, pDst, nBytes );

			memcpy( (void *)pDst, pixel, nBytes );

		}

	}

	free( (char *)pixel );

	return ( vfbDst );

}

