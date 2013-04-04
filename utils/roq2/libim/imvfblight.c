/**

 **	$Header: /roq/libim/imvfblight.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imvfblight.c 1     11/02/99 4:38p Zaphod $ "



/**

 **  FILE

 **	imvfblight.c	-  Change a VFB's colors to be brighter/darker

 **

 **  PROJECT

 **	libim		-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **

 **	imvfblight.c contains routines to modify the lightness/darkness of a VFB

 **

 **  HISTORY

 **	$Log: /roq/libim/imvfblight.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.9  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.8  1995/06/16  08:55:44  bduggan

 **	added a cast

 **	/.

 **

 **	Revision 1.7  94/10/03  11:29:54  nadeau

 **	Added support for grayscale image lightening.

 **	Fixed code that missed obtaining a pointer to the CLT in some

 **	cases, causing a core dump.

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

 **	Revision 1.6  92/11/04  12:12:02  groening

 **	changed macro names

 **	

 **	Revision 1.5  92/08/31  17:42:02  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.4  91/10/03  09:21:11  nadeau

 **	Fixed #includes.

 **	

 **	Revision 1.3  91/05/14  07:46:19  mjb

 **	Bug fix

 **	

 **	Revision 1.2  91/03/08  14:37:01  nadeau

 **	Changed IMVFBINDEX32 to IMVFBINDEX16.

 **	

 **	Revision 1.1  91/02/05  13:46:58  nadeau

 **	Initial revision

 **	

 **	

 **/



/**

 **  CODE CREDITS

 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1991.

 **/



#include <math.h>

#include "iminternal.h"







/* range of integer values:						*/



#define IMVFBL_MAXR		255

#define IMVFBL_MAXG		255

#define IMVFBL_MAXB		255





/**

 ** helpful information:

 **

 **	Y = 0.30*R + 0.59*G + 0.11*B

 **	U = B - Y

 **	V = R - Y

 **

 **/









/* macros to define YUV as functions of RGB:				*/



#define IM_YFRGB(r,g,b)	(  0.30*(r)/(float)IMVFBL_MAXR + 0.59*(g)/(float)IMVFBL_MAXG + 0.11*(b)/(float)IMVFBL_MAXB )

#define IM_UFRGB(r,g,b)	( -0.30*(r)/(float)IMVFBL_MAXR - 0.59*(g)/(float)IMVFBL_MAXG + 0.89*(b)/(float)IMVFBL_MAXB )

#define IM_VFRGB(r,g,b)	(  0.70*(r)/(float)IMVFBL_MAXR - 0.59*(g)/(float)IMVFBL_MAXG - 0.11*(b)/(float)IMVFBL_MAXB )





/* macros to define RGB as functions of YUV:				*/



#define IM_RFYUV(y,u,v)	(int) (  (float)IMVFBL_MAXR * ( (y) + (v) )   )

#define IM_GFYUV(y,u,v)	(int) (  (float)IMVFBL_MAXG * ( (0.70-0.11)*(y) + (-0.11)*(u) + (0.70-1.00)*(v) )  /  0.59   )

#define IM_BFYUV(y,u,v)	(int) (  (float)IMVFBL_MAXB * ( (y) + (u) )   )











/*

 *  FUNCTION

 *	ImVfbLightness	-  change a vfb's colors to be brighter or darker

 *

 *  DESCRIPTION

 *	The given scale factor is applied to each pixel in the image by

 *	extracting the pixel's color, converting it to YUV space,

 *	multiplying the Y value (intensity) by the factor, then converting

 *	back to the original color space.

 */



ImVfb *				/* Returns lighter VFB			*/

#ifdef __STDC__

ImVfbLightness( ImVfb *srcVfb, double factor, ImVfb *dstVfb )

#else

ImVfbLightness( srcVfb, factor, dstVfb )

	ImVfb *srcVfb;		/* source vfb				*/

	double factor;		/* scale factor for lightness		*/

	ImVfb *dstVfb;		/* destination vfb			*/

#endif

{

	int r, g, b;		/* red, green, blue values		*/

	int i;			/* counter				*/

	int type;		/* framebuffer type			*/

	ImClt *srcClt;		/* source CLT				*/

	ImClt *dstClt;		/* destination CLT			*/

	float y, u, v;		/* y, u, v values			*/

	ImCltPtr cp;		/* internal clt pointer			*/

	ImVfbPtr psrc, pdst;	/* internal vfb pointers		*/

	int ncolors;		/* # colors in clt			*/





	/*

	 *  Get the image type and make sure we can handle it.

	 */

	type = ImVfbQFields( srcVfb ) & IMVFBIMAGEMASK;

	if ( type == 0 || type == IMVFBMONO )

	{

		/* Can't handle it.					*/

		ImErrNo = IMEFIELD;

		return ( IMVFBNULL );

	}





	/*

	 *  If there's no destination VFB, make one.  Give it a CLT if

	 *  required.

	 */

	srcClt = ImVfbQClt( srcVfb );

	dstClt = IMCLTNULL;

	if ( dstVfb == IMVFBNEW )

	{

		dstVfb = ImVfbAlloc( ImVfbQWidth( srcVfb ),

			ImVfbQHeight( srcVfb ), type );

		if( dstVfb == IMVFBNULL )

		{

			ImErrNo = IMEMALLOC;

			return( IMVFBNULL );

		}





		/* Allocate the new clt, if necessary			*/

		if ( srcClt != IMCLTNULL )

		{

			ncolors = ImCltQNColors( srcClt );

			dstClt  = ImCltAlloc( ncolors );

			if( dstClt == IMCLTNULL )

			{

				ImVfbFree( dstVfb );

				ImErrNo = IMEMALLOC;

				return( IMVFBNULL );

			}

			ImVfbSClt( dstVfb, dstClt );

		}

	}

	else if ( srcClt != IMCLTNULL )

	{

		dstClt  = ImVfbQClt( dstVfb );

		if ( dstClt == IMCLTNULL )

		{

			ImErrNo = IMEFIELD;

			return ( IMVFBNULL );

		}

		ncolors = ImCltQNColors( srcClt );

		if ( ncolors > ImCltQNColors( dstClt ) )

			ncolors = ImCltQNColors( dstClt );

	}





	/*

	 *  Color indexed image.  Walk through it's CLT and change each

	 *  color.

	 */

	if ( srcClt != IMCLTNULL &&

		(type == IMVFBINDEX8 || type == IMVFBINDEX16) )

	{

		for ( i = 0; i < ncolors; i++ )

		{

			cp = ImCltQPtr( srcClt, i );

			r = ImCltQRed( cp );

			g = ImCltQGreen( cp );

			b = ImCltQBlue( cp );



			y = IM_YFRGB( r, g, b );

			u = IM_UFRGB( r, g, b );

			v = IM_VFRGB( r, g, b );



			y *= factor;



			r = IM_RFYUV( y, u, v );

			g = IM_GFYUV( y, u, v );

			b = IM_BFYUV( y, u, v );



			if ( r < 0 )		r = 0;

			if ( r > IMVFBL_MAXR )	r = IMVFBL_MAXR;

			if ( g < 0 )		g = 0;

			if ( g > IMVFBL_MAXG )	g = IMVFBL_MAXG;

			if ( b < 0 )		b = 0;

			if ( b > IMVFBL_MAXB )	b = IMVFBL_MAXB;



			cp = ImCltQPtr( dstClt, i );

			ImCltSRed( cp, r );

			ImCltSGreen( cp, g );

			ImCltSBlue( cp, b );

		}

		return( dstVfb );

	}





	/*

	 *  RGB image.  Walk through it's pixels and change each one.

	 */

	if ( type == IMVFBRGB )

	{

		for ( psrc=ImVfbQFirst(srcVfb), pdst=ImVfbQFirst(dstVfb);

			psrc <= ImVfbQLast(srcVfb);

			ImVfbSInc(srcVfb,psrc), ImVfbSInc(dstVfb,pdst) )

		{

			r = ImVfbQRed( srcVfb, psrc );

			g = ImVfbQGreen( srcVfb, psrc );

			b = ImVfbQBlue( srcVfb, psrc );



			y = IM_YFRGB( r, g, b );

			u = IM_UFRGB( r, g, b );

			v = IM_VFRGB( r, g, b );



			y *= factor;



			r = IM_RFYUV( y, u, v );

			g = IM_GFYUV( y, u, v );

			b = IM_BFYUV( y, u, v );



			if ( r < 0 )		r = 0;

			if ( r > IMVFBL_MAXR )	r = IMVFBL_MAXR;

			if ( g < 0 )		g = 0;

			if ( g > IMVFBL_MAXG )	g = IMVFBL_MAXG;

			if ( b < 0 )		b = 0;

			if ( b > IMVFBL_MAXB )	b = IMVFBL_MAXB;



			ImVfbSRed( dstVfb, pdst, r );

			ImVfbSGreen( dstVfb, pdst, g );

			ImVfbSBlue( dstVfb, pdst, b );

		}

		return ( dstVfb );

	}





	/*

	 *  Grayscale image.  Pixels are treated as single integer gray

	 *  values rather than indexes into a color lookup table.  To

	 *  lighten them, apply the lighten factor to each index.

	 */

	for ( psrc=ImVfbQFirst(srcVfb), pdst=ImVfbQFirst(dstVfb);

		psrc <= ImVfbQLast(srcVfb);

		ImVfbSInc(srcVfb,psrc), ImVfbSInc(dstVfb,pdst) )

	{

		g = ImVfbQGray( srcVfb, psrc );



		g = (int) (g * factor);



		if ( g < 0 )		g = 0;

		if ( g > IMVFBL_MAXG )	g = IMVFBL_MAXG;



		ImVfbSGray( dstVfb, pdst, g );

	}

	return ( dstVfb );

}

