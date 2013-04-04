/**
 **	$Header: /roq/libim/imvfbresize.c 1     11/02/99 4:38p Zaphod $
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

#define HEADER	"    $Header: /roq/libim/imvfbresize.c 1     11/02/99 4:38p Zaphod $ "

/**
 **  FILE
 **	imvfbresize.c		-  Change a VFB to a new size
 **
 **  PROJECT
 **	libim		-  SDSC image manipulation library
 **
 **  DESCRIPTION
 **	imvfbresize.c contains code to change the resolution of an image.
 **
 **  PUBLIC CONTENTS
 **			d =defined constant
 **			f =function
 **			m =defined macro
 **			t =typedef/struct/union
 **			v =variable
 **			? =other
 **
 **	ImVfbResize	f  change the resolution of a vfb
 **
 **  PRIVATE CONTENTS
 **	none
 **
 **  HISTORY
 **	$Log: /roq/libim/imvfbresize.c $
 * 
 * 1     11/02/99 4:38p Zaphod
 **	Revision 1.11  1995/06/30  22:11:56  bduggan
 **	added some casts
 **
 **	Revision 1.10  1995/06/29  00:28:04  bduggan
 **	updated copyright year
 **
 **	Revision 1.9  1995/06/16  09:00:36  bduggan
 **	added some casts
 **
 **	Revision 1.8  94/10/03  11:29:55  nadeau
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
 **	Revision 1.7  92/08/31  17:42:42  vle
 **	Updated copyright notice.
 **	
 **	Revision 1.6  92/08/26  11:19:28  nadeau
 **	Added additional error checks.
 **	
 **	Revision 1.5  92/08/25  16:08:00  groening
 **	added pixel replication feature
 **	
 **	Revision 1.4  92/02/27  16:21:55  nadeau
 **	Fixed bug in resizing of FDATA field info.
 **	
 **	Revision 1.3  91/10/03  09:22:21  nadeau
 **	Added handling of a non-new destination VFB.  Added
 **	handling of IDATA, FDATA, MONO, Z, and WPROT fields.
 **	Fixed problem that stepped beyond the right and bottom
 **	edges of the VFB.  Fixed problem that caused jaggy
 **	edges on all images scaled up.
 **	
 **	Revision 1.2  91/03/08  14:37:31  nadeau
 **	Changed name from ImVfbNewRes to ImVfbReSize.  Added comments.
 **	Deleted comments.  Changed IMVFBINDEX32 to IMVFBINDEX16.
 **	
 **	Revision 1.1  91/02/05  13:46:55  mjb
 **	Initial revision
 **/

/**
 **  CODE CREDITS
 **     Custom development, Michael Bailey, San Diego Supercomputer Center, 1991.
 **/

#include <math.h>
#include "iminternal.h"





/*
 *  FUNCTION
 *	ImVfbResize	-  change the resolution of a vfb
 *
 *  DESCRIPTION
 *	Increase or decrease the size of the image using pixel replication
 *	or bilinear interpolation.  In either case a new
 *	destination VFB is created (unless one has been passed in) and
 *	the source image copied into it, changing its size as we go.
 */

ImVfb *				/* Returns resized VFB		*/
#ifdef __STDC__
ImVfbResize( ImVfb *srcVfb, int algorithm, ImVfb *dstVfb, int width, int height )
#else
ImVfbResize( srcVfb, algorithm, dstVfb, width, height )
	ImVfb   *srcVfb;	/* VFB to resize		*/
	int               algorithm;	/* Algorithm to use		*/
	ImVfb            *dstVfb;	/* Result VFB			*/
	int               width, height;/* Desired dimensions		*/
#endif
{
	ImVfbPtr pdst;		/* Destination VFB pointer	*/
	ImVfbPtr psrc;		/* Destination VFB pointer	*/
	ImVfbPtr p00, p10, p01, p11;	/* corner pointers	*/
	int      i,j,k,l;	/* generic integer value	*/
	int      fields;	/* vfb field description	*/
	ImVfb            *tmpVfb;	/* Temporary VFB		*/
	float             f;		/* Generic float value		*/
	int               sw, sh;	/* Source size			*/
	int               dw, dh;	/* Destination size		*/
	int               intdx, intdy;	/* Step through dst vfb		*/
	int               intsx, intsy;	/* Integer source locations	*/
	int		  intsx2, intsy2;/* Second integer source loc	*/
	float             sclw, sclh;	/* mapping scale factors	*/
	float             c00, c10, c01, c11;	/* corner coefficients	*/
	float             floatsx, floatsy;	/* fp source pos	*/
	float             fracsx, fracsy;	/* fractional source pos*/
	float             omfracsx, omfracsy;	/* 1.0 - fractional values*/
	int		  scaleFacOne, scaleFacTwo; /* scale factors	*/


	/*
	 *  Check what algorithm the user wants us to use.
	 */
	switch ( algorithm )
	{
	case IMVFBPIXELREP:
		/* 
		 *  Check that the new size is larger than the source size,
		 *  and that it is an even multiple.
		 */
		if ( width < ImVfbQWidth( srcVfb ) )
		{
			ImErrNo = IMEWIDTH;
			return ( IMVFBNULL );
		}
		if ( height < ImVfbQHeight( srcVfb ) )
		{
			ImErrNo = IMEHEIGHT;
			return ( IMVFBNULL );
		}
		scaleFacOne = width  % ImVfbQWidth(srcVfb);
		scaleFacTwo = height % ImVfbQHeight(srcVfb);
		if ( (scaleFacOne!=0) || (scaleFacTwo!=0) )
		{
			ImErrNo = IMEPIXELREP;
			return(IMVFBNULL);
		}
		break;

	case IMVFBBILINEAR:
		break;

	default:
		ImErrNo = IMEBADALGORITHM;
		return( IMVFBNULL );
	}


	/*
	 *  If the user hasn't given us a destination VFB, make one.
	 *  If they have given us a destination VFB, make sure it is usable.
	 */
	fields = ImVfbQFields( srcVfb );
	sw     = ImVfbQWidth( srcVfb );
	sh     = ImVfbQHeight( srcVfb );
	tmpVfb = IMVFBNULL;
	if ( dstVfb == IMVFBNEW )
	{
		dstVfb = ImVfbAlloc( width, height, fields );
		if( dstVfb == IMVFBNULL )
		{
			ImErrNo = IMEMALLOC;
			return( IMVFBNULL );
		}
	}
	else
	{
		if ( ImVfbQWidth( dstVfb ) != width )
		{
			ImErrNo = IMEWIDTH;
			return ( IMVFBNULL );
		}
		if ( ImVfbQHeight( dstVfb ) != height )
		{
			ImErrNo = IMEHEIGHT;
			return ( IMVFBNULL );
		}
		if ( ImVfbQFields( dstVfb ) != fields )
		{
			ImErrNo = IMECONFLICT;
			return ( IMVFBNULL );
		}
	}



	if (algorithm == IMVFBPIXELREP)
	{
	    /*
	     *  Perform pixel replication to increase the image's size.
	     */
	    scaleFacOne = width  / ImVfbQWidth(srcVfb);
	    scaleFacTwo = height / ImVfbQHeight(srcVfb);

	    psrc = ImVfbQFirst( srcVfb);
	    pdst = ImVfbQFirst( dstVfb);

	    for (i=0; i<ImVfbQHeight(srcVfb); i++)
	    {
	      for (k=0; k<scaleFacTwo; k++)
	      {
	      	for (j=0; j<ImVfbQWidth(srcVfb); j++)
	      	{
		   for (l=0; l<scaleFacOne;l++)
		   {
			if ( fields & IMVFBRGB )
			{
			   ImVfbSRed (dstVfb, pdst, ImVfbQRed (srcVfb, psrc) );
			   ImVfbSGreen (dstVfb, pdst,ImVfbQGreen(srcVfb, psrc));
			   ImVfbSBlue (dstVfb, pdst, ImVfbQBlue(srcVfb, psrc) );
			}

			if ( fields & IMVFBINDEX8 )
			{
			   ImVfbSIndex8(dstVfb,pdst,ImVfbQIndex8(srcVfb,psrc) );
			}

			if ( fields & IMVFBINDEX16 )
			{
			   ImVfbSIndex16(dstVfb,pdst,ImVfbQIndex16(srcVfb,psrc));
			}

			if ( fields & IMVFBALPHA )
			{
			   ImVfbSAlpha(dstVfb,pdst,ImVfbQAlpha(srcVfb,psrc) );
			}

			if ( fields & IMVFBWPROT )
			{
			   ImVfbSWProt(dstVfb,pdst,ImVfbQWProt(srcVfb,psrc) );
			}

			if ( fields & IMVFBIDATA )
			{
			   ImVfbSIData(dstVfb,pdst,ImVfbQIData(srcVfb,psrc) );
			}

			if ( fields & IMVFBFDATA )
			{
			   ImVfbSFData(dstVfb,pdst,ImVfbQFData(srcVfb,psrc) );
			}

			if ( fields & IMVFBMONO )
			{
			   ImVfbSMono(dstVfb,pdst,ImVfbQMono(srcVfb,psrc) );
			}

			if ( fields & IMVFBZ )
			{
			   ImVfbSZ(dstVfb,pdst,ImVfbQZ(srcVfb,psrc) );
			}

			ImVfbSInc (dstVfb, pdst);
		   }
		   ImVfbSInc(srcVfb, psrc);
	      	}
		ImVfbSUp(srcVfb, psrc);
	      }
	      ImVfbSDown (srcVfb, psrc);
	    }

	    return( dstVfb );
	}


	/*
	 *  Perform bilinear interpolation to increase or decrease
	 *  image's size.
	 */

	if ( srcVfb == dstVfb )
	{
		/*
		 *  This is a destructive algorithm.  In order to
		 *  avoid stepping on ourself we copy the source
		 *  VFB to a temporary VFB and work with that.
		 */
		tmpVfb = ImVfbCopy( srcVfb, 0, 0, sw, sh, fields,
			IMVFBNEW, 0, 0 );
		if( tmpVfb == IMVFBNULL )
		{
			ImErrNo = IMEMALLOC;
			return( IMVFBNULL );
		}
		srcVfb = tmpVfb;
	}


	/*
	 *  Figure scale factors.
	 */
	dw   = width;
	dh   = height;
	sclw = (float)(sw-1) / (float)(dw-1);
	sclh = (float)(sh-1) / (float)(dh-1);


	/*
	 *  Walk through the image, computing new X and Y floating point
	 *  positions along the way.
	 */
	for ( floatsy = 0.0, intdy = 0, pdst = ImVfbQFirst( dstVfb );
	     intdy < dh; floatsy += sclh, intdy++ )
	{
		/*
		 *  Compute our integer and fractional Y positions in
		 *  the source image.
		 */
		intsy    = (int)floatsy;
		intsy2   = intsy + 1;
		if ( intsy2 > sh-1 )
			intsy2 = sh-1;
		fracsy   = floatsy - (float) intsy;
		omfracsy = 1.0 - fracsy;

		for ( floatsx = 0.0, intdx = 0; intdx < dw;
		  floatsx += sclw, intdx++, ImVfbSInc( dstVfb, pdst ) )
		{
			/*
			 *  Compute our integer and fractional X positions
			 *  in the source image.
			 */
			intsx    = (int)floatsx;
			intsx2   = intsx + 1;
			if ( intsx2 > sw-1 )
				intsx2 = sw-1;
			fracsx   = floatsx - (float) intsx;
			omfracsx = 1.0 - fracsx;


			/*
			 *  Advance our position in the source image.
			 */
			p00 = ImVfbQPtr( srcVfb, intsx , intsy  );
			p10 = ImVfbQPtr( srcVfb, intsx2, intsy  );
			p01 = ImVfbQPtr( srcVfb, intsx , intsy2 );
			p11 = ImVfbQPtr( srcVfb, intsx2, intsy2 );


			/*
			 *  Compute corner coefficients.
			 */
			c00 = omfracsx * omfracsy;
			c10 =   fracsx * omfracsy;
			c01 = omfracsx *   fracsy;
			c11 =   fracsx *   fracsy;


			/*
			 *  Interpolate each of the VFB fields.
			 */
			if ( fields & IMVFBRGB )
			{
				i = (int)
				   (c00 * ImVfbQRed( srcVfb, p00 ) +
				    c10 * ImVfbQRed( srcVfb, p10 ) +
				    c01 * ImVfbQRed( srcVfb, p01 ) +
				    c11 * ImVfbQRed( srcVfb, p11 ));
				ImVfbSRed( dstVfb, pdst, i & 0xFF );

				i = (int)
				   (c00 * ImVfbQGreen( srcVfb, p00 ) +
				    c10 * ImVfbQGreen( srcVfb, p10 ) +
				    c01 * ImVfbQGreen( srcVfb, p01 ) +
				    c11 * ImVfbQGreen( srcVfb, p11 ));
				ImVfbSGreen( dstVfb, pdst, i & 0xFF );

				i = (int)
				    (c00 * ImVfbQBlue( srcVfb, p00 ) +
				    c10 * ImVfbQBlue( srcVfb, p10 ) +
				    c01 * ImVfbQBlue( srcVfb, p01 ) +
				    c11 * ImVfbQBlue( srcVfb, p11 ));
				ImVfbSBlue( dstVfb, pdst, i & 0xFF );
			}

			if ( fields & IMVFBINDEX8 )
			{
				i = (int)
				    (c00 * ImVfbQIndex8( srcVfb, p00 ) +
				    c10 * ImVfbQIndex8( srcVfb, p10 ) +
				    c01 * ImVfbQIndex8( srcVfb, p01 ) +
				    c11 * ImVfbQIndex8( srcVfb, p11 ));
				ImVfbSIndex8( dstVfb, pdst, i & 0xFF );
			}

			if ( fields & IMVFBINDEX16 )
			{
				i = (int)
				   (c00 * ImVfbQIndex16( srcVfb, p00 ) +
				    c10 * ImVfbQIndex16( srcVfb, p10 ) +
				    c01 * ImVfbQIndex16( srcVfb, p01 ) +
				    c11 * ImVfbQIndex16( srcVfb, p11 ));
				ImVfbSIndex16( dstVfb, pdst, i & 0xFFFF );
			}

			if ( fields & IMVFBMONO )
			{
				i = (int)
				   (c00 * ImVfbQMono( srcVfb, p00 ) +
				    c10 * ImVfbQMono( srcVfb, p10 ) +
				    c01 * ImVfbQMono( srcVfb, p01 ) +
				    c11 * ImVfbQMono( srcVfb, p11 ));
				ImVfbSMono( dstVfb, pdst, i & 0x1 );
			}

			if ( fields & IMVFBALPHA )
			{
				i = (int)
				   (c00 * ImVfbQAlpha( srcVfb, p00 ) +
				    c10 * ImVfbQAlpha( srcVfb, p10 ) +
				    c01 * ImVfbQAlpha( srcVfb, p01 ) +
				    c11 * ImVfbQAlpha( srcVfb, p11 ));
				ImVfbSAlpha( dstVfb, pdst, i & 0xFF );
			}

			if ( fields & IMVFBWPROT )
			{
				i = (int)
				   (c00 * ImVfbQWProt( srcVfb, p00 ) +
				    c10 * ImVfbQWProt( srcVfb, p10 ) +
				    c01 * ImVfbQWProt( srcVfb, p01 ) +
				    c11 * ImVfbQWProt( srcVfb, p11 ));
				ImVfbSWProt( dstVfb, pdst, i & 0xFF );
			}

			if ( fields & IMVFBZ )
			{
				i = (int)
				   (c00 * ImVfbQZ( srcVfb, p00 ) +
				    c10 * ImVfbQZ( srcVfb, p10 ) +
				    c01 * ImVfbQZ( srcVfb, p01 ) +
				    c11 * ImVfbQZ( srcVfb, p11 ));
				ImVfbSZ( dstVfb, pdst, i );
			}

			if ( fields & IMVFBIDATA )
			{
				i = (int)
				   (c00 * ImVfbQIData( srcVfb, p00 ) +
				    c10 * ImVfbQIData( srcVfb, p10 ) +
				    c01 * ImVfbQIData( srcVfb, p01 ) +
				    c11 * ImVfbQIData( srcVfb, p11 ));
				ImVfbSIData( dstVfb, pdst, i );
			}

			if ( fields & IMVFBFDATA )
			{
				f = (int)
				   (c00 * ImVfbQFData( srcVfb, p00 ) +
				    c10 * ImVfbQFData( srcVfb, p10 ) +
				    c01 * ImVfbQFData( srcVfb, p01 ) +
				    c11 * ImVfbQFData( srcVfb, p11 ));
				ImVfbSFData( dstVfb, pdst, f );
			}
		}
	}
	if ( srcVfb == tmpVfb )
		ImVfbFree( tmpVfb );

	return( dstVfb );
}
