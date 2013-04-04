/**

 **	$Header: /roq/libim/imvfbgamma.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imvfbgamma.c 1     11/02/99 4:38p Zaphod $ "



/**

 **  FILE

 **	imvfbgamma.c	-  Change a VFB's colors' gamma correction

 **

 **  PROJECT

 **	libim		-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **

 **

 **  HISTORY

 **	$Log: /roq/libim/imvfbgamma.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.3  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.2  1994/10/03  11:29:50  nadeau

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

 **	Revision 1.1  92/12/03  01:57:58  nadeau

 **	Initial revision

 **	

 **	

 **	

 **/



#include <math.h>

#include "iminternal.h"







/* range of integer values:						*/



#define IM_MAXR		255

#define IM_MAXG		255

#define IM_MAXB		255





/* types of framebuffers:						*/



#define IM_FBINDEX		0

#define IM_FBRGB		1

#define IM_FBGRAY		2









/*

 *  FUNCTION

 *	ImVfbGamma	-  change a vfb's colors to be brighter or darker

 *

 *  DESCRIPTION

 */



ImVfb *			/* Returns gamma-corrected VFB		*/

ImVfbGamma( srcVfb, gamma, dstVfb )

	ImVfb *srcVfb;		/* source vfb				*/

	double gamma;		/* gamma correction			*/

	ImVfb *dstVfb;		/* destination vfb			*/

{

	int r, g, b;	/* red, green, blue values		*/

	int i;		/* counter				*/

	int f;		/* vfb field description		*/

	int type;		/* framebuffer type			*/

	ImClt *srcClt;		/* source CLT				*/

	ImClt *dstClt;		/* destination CLT			*/

	float fr, fg, fb;	/* floating point r, g, b		*/

	ImCltPtr cp;		/* internal clt pointer			*/

	ImVfbPtr psrc, pdst;	/* internal vfb pointers		*/

	int ncolors;		/* # colors in clt			*/





	/* determine the fields on this vfb:				*/



	f = ImVfbQFields( srcVfb );





	/* determine vfb type:						*/



	if( f & IMVFBRGB )

		type = IM_FBRGB;

	else

	{

		if( (f&IMVFBINDEX8)  ||  (f&IMVFBINDEX16) )

			type = IM_FBINDEX;

		else

			type = IM_FBGRAY;

	}





	/* allocate the new vfb and clt:				*/



	if( dstVfb == IMVFBNEW )

	{

		dstVfb = ImVfbAlloc( ImVfbQWidth(srcVfb), ImVfbQHeight(srcVfb), f );

		if( dstVfb == IMVFBNULL )

		{

			ImErrNo = IMEMALLOC;

			return( IMVFBNULL );

		}





		/* allocate the new clt, if necessary:			*/



		if( type == IM_FBINDEX )

		{

			srcClt = ImVfbQClt( srcVfb );

			ncolors = ImCltQNColors( srcClt );

			dstClt = ImCltAlloc( ncolors );

			if( dstClt == IMCLTNULL )

			{

				ImVfbFree( dstVfb );

				ImErrNo = IMEMALLOC;

				return( IMVFBNULL );

			}



			ImVfbSClt( dstVfb, dstClt );

		}



	}







	/* if a clt situation, walk through the clt changing the colors: */



	if( type == IM_FBINDEX )

	{



		for( i = 0; i < ncolors; i++ )

		{

			cp = ImCltQPtr( srcClt, i );

			r = ImCltQRed( cp );

			g = ImCltQGreen( cp );

			b = ImCltQBlue( cp );



			if( r > 0 )

			{

				fr = (float) r / (float) IM_MAXR;

				fr = pow( fr, gamma );

				r = (int) ( fr * (float) IM_MAXR + 0.5 );

			}



			if( g > 0 )

			{

				fg = (float) g / (float) IM_MAXG;

				fg = pow( fg, gamma );

				g = (int) ( fg * (float) IM_MAXG + 0.5 );

			}



			if( b > 0 )

			{

				fb = (float) b / (float) IM_MAXB;

				fb = pow( fb, gamma );

				b = (int) ( fb * (float) IM_MAXB + 0.5 );

			}



			if( r < 0 )		r = 0;

			if( r > IM_MAXR )	r = IM_MAXR;

			if( g < 0 )		g = 0;

			if( g > IM_MAXG )	g = IM_MAXG;

			if( b < 0 )		b = 0;

			if( b > IM_MAXB )	b = IM_MAXB;



			cp = ImCltQPtr( dstClt, i );

			ImCltSRed( cp, r );

			ImCltSGreen( cp, g );

			ImCltSBlue( cp, b );

		}





		return( dstVfb );

	}





	/* walk through the vfb, changing the gamma of the colors:	*/



	for( psrc=ImVfbQFirst(srcVfb), pdst=ImVfbQFirst(dstVfb);

		psrc <= ImVfbQLast(srcVfb);

		ImVfbSInc(srcVfb,psrc), ImVfbSInc(dstVfb,pdst) )

	{

		switch( type )

		{

			case IM_FBRGB:

				r = ImVfbQRed( srcVfb, psrc );

				g = ImVfbQGreen( srcVfb, psrc );

				b = ImVfbQBlue( srcVfb, psrc );



				if( r > 0 )

				{

					fr = (float) r / (float) IM_MAXR;

					fr = pow( fr, gamma );

					r = (int) ( fr * (float) IM_MAXR + 0.5 );

				}



				if( g > 0 )

				{

					fg = (float) g / (float) IM_MAXG;

					fg = pow( fg, gamma );

					g = (int) ( fg * (float) IM_MAXG + 0.5 );

				}



				if( b > 0 )

				{

					fb = (float) b / (float) IM_MAXB;

					fb = pow( fb, gamma );

					b = (int) ( fb * (float) IM_MAXB + 0.5 );

				}



				if( r < 0 )		r = 0;

				if( r > IM_MAXR )	r = IM_MAXR;

				if( g < 0 )		g = 0;

				if( g > IM_MAXG )	g = IM_MAXG;

				if( b < 0 )		b = 0;

				if( b > IM_MAXB )	b = IM_MAXB;



				ImVfbSRed( dstVfb, pdst, r );

				ImVfbSGreen( dstVfb, pdst, g );

				ImVfbSBlue( dstVfb, pdst, b );



				break;





			case IM_FBGRAY:

				g = ImVfbQGray( srcVfb, psrc );



				if( g > 0 )

				{

					fg = (float) g / (float) IM_MAXG;

					fg = pow( fg, gamma );

					g = (int) ( fg * (float) IM_MAXG + 0.5 );

				}



				if( g < 0 )		g = 0;

				if( g > IM_MAXG )	g = IM_MAXG;



				ImVfbSGray( dstVfb, pdst, g );



				break;

		}



	}



	return( dstVfb );

}

