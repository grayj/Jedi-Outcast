/**
 **	$Header: /roq/libim/imvfbto.c 1     11/02/99 4:38p Zaphod $
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

#define HEADER	"    $Header: /roq/libim/imvfbto.c 1     11/02/99 4:38p Zaphod $"

/**
 **  FILE
 **	imvfbto.c	-  Image Library VFB depth conversions
 **
 **  PROJECT
 **	libim	-  SDSC image manipulation library
 **
 **  DESCRIPTION
 **	These functions convert images from their current depth to
 **	images of a specific depth:
 **		to 24-bit true color
 **		to 8-bit color index + CLT
 **		to 16-bit color index + CLT
 **		to 8-bit grey index
 **		to 1-bit mono
 **
 **  PUBLIC CONTENTS
 **			d =defined constant
 **			f =function
 **			m =defined macro
 **			t =typedef/struct/union
 **			v =variable
 **			? =other
 **
 **	ImVfbToRgb		f  convert a vfb to 24-bit color
 **	ImVfbToMono		f  convert to 8-bit monochrome
 **	ImVfbFSDitherToMono	f  dither a Vfb to monochrome using 
 **				   Floyd-Steinberg dithering
 **	ImVfbToGray		f  convert to 8-bit grayscale
 **	ImVfbToIndex		f  convert to n-bit color
 **	ImVfbToIndex8		f  convert to 8-bit color
 **	ImVfbToIndex16		f  convert to 8-bit color
 **
 **  PRIVATE CONTENTS
 **	none
 **
 **  HISTORY
 **	$Log: /roq/libim/imvfbto.c $
* 
* 1     11/02/99 4:38p Zaphod
 **	Revision 1.18  1995/06/29  00:28:04  bduggan
 **	updated copyright year
 **
 **	Revision 1.17  1995/06/16  09:01:34  bduggan
 **	changed bzero to memset.  removed some unused vars
 **
 **	Revision 1.16  1995/03/31  05:50:22  bduggan
 **	Grab the lower 8-bits when converting to index8 from index16
 **	instead of the upper 8-bits.
 **
 **	Revision 1.15  1995/02/16  21:43:47  bduggan
 **	Removed threshold variable from dither function
 **
 **	Revision 1.14  1995/01/18  21:49:37  bduggan
 **	Put in ImCallocRetOnError macro so that -1
 **	is not returned from a function that's supposed to return
 **	a pointer
 **
 **	Revision 1.13  1995/01/12  00:36:02  bduggan
 **	Added dithering support
 **
 **	Revision 1.12  94/10/03  11:29:58  nadeau
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
 **	Revision 1.11  92/09/18  12:39:07  vle
 **	Fixed bug in ImVfbToMono() that caused RGB images to be monochromed
 **	incorrectly.
 **	
 **	Revision 1.10  92/09/17  14:58:08  vle
 **	Added ImVfbToIndex() which converts any VFB type to an N color
 **	indexed VFB.  Modified TryRGB() and TryYUV() to accommadate
 **	the changes.  Fixed bug in ImVfbToIndex16() that caused image
 **	data to be shifted when it shouldn't be.  Updated copyright
 **	notice.
 **	
 **	Revision 1.9  91/10/03  09:23:49  nadeau
 **	Fixed problem that mapped mono white to (1,1,1) instead
 **	of (255,255,255) on RGB conversion.  Added another decimal
 **	place to RED/GREEN/BLUE percentages for grayscale conversion.
 **	Fixed #includes.
 **	
 **	Revision 1.8  91/03/13  12:55:43  nadeau
 **	Typo fixed.
 **	
 **	Revision 1.7  91/03/13  12:49:15  nadeau
 **	Fixed minor CLT grayscale filling bug that always counted to
 **	256, whether the CLT was that big or not.
 **	
 **	Revision 1.6  91/03/08  14:38:32  nadeau
 **	Added lots of comments.  Added more robust support for the
 **	various VFB types.  Added IMVFBINDEX16 and IMVFBMONO support.
 **	
 **	Revision 1.5  91/01/30  18:14:22  nadeau
 **	Added mono conversion and check and fixed bug in gray conversion.
 **	
 **	Revision 1.4  90/10/22  16:40:53  nadeau
 **	Moved imvfbto8color.c into the file.
 **	
 **	Revision 1.3  90/07/23  12:57:12  todd
 **	Add grayscale (no CLT) to ImVfbTo24()
 **	
 **	Revision 1.2  90/07/02  13:23:07  nadeau
 **	Updated comment and added inclusion of iminternal.h.
 **	
 **	Revision 1.1  90/05/11  14:29:07  nadeau
 **	Initial revision
 **	
 **/

/**
 **  CODE CREDITS
 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1990.
 **/

#include <math.h>
#include "iminternal.h"

#ifdef __STDC__
ImVfb *ImVfbToGray( ImVfb *srcVfb, ImVfb *dstVfb );
#else
ImVfb *ImVfbToGray();
#endif


/*
 *  FUNCTION
 *	ImVfbToRgb	-  convert a vfb to RGB
 *
 *  DESCRIPTION
 *	Promotion:
 *		Mono VFB:
 *		Index8 VFB:
 *		Index 16 VFB:
 *			Copied to RGB VFB.  CLT used, if any.  Otherwise
 *			grayscale assumed.
 *
 *	No change:
 *		RGB VFB:
 *			Copied to RGB VFB.
 *
 *  CLT HANDLING
 *	When a CLT is not present, the RGB values are generated assuming
 *	the incoming image is grayscale.  Each grayscale index is copied
 *	to all three channels (red, green, blue) of the RGB VFB.  For 16-bit
 *	index VFB's, the indexes are (unavoidably) truncated to the lower
 *	8-bits.
 *
 *	When a CLT is present, the RGB values are generated by looking up
 *	each VFB index in the CLT and copying the corresponding red, green,
 *	and blue values to the RGB VFB.
 */

ImVfb *			/* Returns changed VFB			*/
#ifdef __STDC__
ImVfbToRgb( ImVfb *srcVfb, ImVfb *dstVfb )
#else
ImVfbToRgb( srcVfb, dstVfb )
	ImVfb    *srcVfb;	/* input vfb				*/
	ImVfb    *dstVfb;	/* possibly the output vfb		*/
#endif
{
	ImVfb    *rgbVfb;	/* the 24-bit vfb			*/
	ImVfbPtr  in;		/* input pointer into srcVfb		*/
	ImVfbPtr  out;		/* output pointer into rgbVfb		*/
	ImClt    *clt;		/* the clt of the source vfb		*/
	ImCltPtr  cp;		/* pointer into clt			*/
	int       fields;	/* Input VFB's field mask		*/
	int       type;		/* Type of input VFB			*/
	ImVfbPtr  last;		/* Last pointer for input VFB		*/
	int       index;	/* Color index				*/


	/*
	 *  Create the new VFB, if needed.
	 */
	if ( dstVfb == IMVFBNEW )
	{
		if ( (rgbVfb = ImVfbAlloc( ImVfbQWidth( srcVfb ),
			ImVfbQHeight( srcVfb ), IMVFBRGB )) == IMVFBNULL )
		{
			ImErrNo = IMEMALLOC;
			return( IMVFBNULL );
		}
	}
	else
	{
		/*
		 *  Confirm the given VFB has an RGB field and that it has
		 *  the same image dimensions.
		 */
		if ( !(ImVfbQFields( dstVfb ) & IMVFBRGB) )
		{
			ImErrNo = IMENOTRGB;
			return ( IMVFBNULL );
		}
		if ( ImVfbQWidth( srcVfb ) != ImVfbQWidth( dstVfb ) )
		{
			ImErrNo = IMEWIDTH;
			return ( IMVFBNULL );
		}
		if ( ImVfbQHeight( srcVfb ) != ImVfbQHeight( dstVfb ) )
		{
			ImErrNo = IMEHEIGHT;
			return ( IMVFBNULL );
		}
		rgbVfb = dstVfb;
	}


	/*
	 *  Figure out where to get input colors to be put into the RGB VFB.
	 */
	clt  = ImVfbQClt( srcVfb );		/* Might be IMCLTNULL	*/
	fields = ImVfbQFields( srcVfb );
	if ( fields & IMVFBINDEX16 )
		type = IMVFBINDEX16;		/* 16-bit color index	*/
	else if ( fields & IMVFBINDEX8 )
		type = IMVFBINDEX8;		/* 8-bit color index	*/
	else if ( fields & IMVFBMONO )
		type = IMVFBMONO;		/* 1-bit mono		*/
	else if ( fields & IMVFBRGB )
	{
		if ( srcVfb == dstVfb )
			return ( srcVfb );	/* Already RGB		*/

		last = ImVfbQLast( srcVfb );
		for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(rgbVfb);
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(rgbVfb,out) )
		{
			ImVfbSRed(   rgbVfb, out, ImVfbQRed(   srcVfb, in ) );
			ImVfbSGreen( rgbVfb, out, ImVfbQGreen( srcVfb, in ) );
			ImVfbSBlue(  rgbVfb, out, ImVfbQBlue(  srcVfb, in ) );
		}
		return ( rgbVfb );
	}
	else
	{
		/*
		 *  No image to convert?
		 */
		if ( dstVfb != rgbVfb )
			ImVfbFree( rgbVfb );
		ImErrNo = IMENOTINFO;
		return ( IMVFBNULL );
	}


	if ( clt == IMCLTNULL )
	{
		/*
		 *  No color table.  Assume grayscale.
		 */
		switch ( type )
		{
		case IMVFBMONO:		/* Monochrome			*/
			last = ImVfbQLast( srcVfb );
			for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(rgbVfb);
				in <= last;
				ImVfbSInc(srcVfb,in), ImVfbSInc(rgbVfb,out) )
			{
				index = (ImVfbQMono( srcVfb, in ) & 0x1);
				ImVfbSRed(   rgbVfb, out, index * 255 );
				ImVfbSGreen( rgbVfb, out, index * 255 );
				ImVfbSBlue(  rgbVfb, out, index * 255 );
			}
			return ( rgbVfb );

		case IMVFBINDEX8:	/* 8-bit color			*/
			last = ImVfbQLast( srcVfb );
			for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(rgbVfb);
				in <= last;
				ImVfbSInc(srcVfb,in), ImVfbSInc(rgbVfb,out) )
			{
				index = ImVfbQIndex8( srcVfb, in );
				ImVfbSRed(   rgbVfb, out, index );
				ImVfbSGreen( rgbVfb, out, index );
				ImVfbSBlue(  rgbVfb, out, index );
			}
			return ( rgbVfb );

		case IMVFBINDEX16:	/* 16-bit color			*/
			/*
			 *  Grab the lower 8-bits of each index and treat it
			 *  as a grayscale value.
			 */
			last = ImVfbQLast( srcVfb );
			for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(rgbVfb);
				in <= last;
				ImVfbSInc(srcVfb,in), ImVfbSInc(rgbVfb,out) )
			{
				index = ((ImVfbQIndex16( srcVfb, in )) & 0x00FF);
				ImVfbSRed(   rgbVfb, out, index );
				ImVfbSGreen( rgbVfb, out, index );
				ImVfbSBlue(  rgbVfb, out, index );
			}
			return ( rgbVfb );
		}
	}


	/*
	 *  Has color table.
	 */
	switch ( type )
	{
	case IMVFBMONO:		/* Monochrome				*/
		last = ImVfbQLast( srcVfb );
		for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(rgbVfb);
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(rgbVfb,out) )
		{
			cp = ImCltQPtr( clt, ImVfbQMono( srcVfb, in ) ) ;
			ImVfbSRed(   rgbVfb, out, ImCltQRed( cp ) );
			ImVfbSGreen( rgbVfb, out, ImCltQGreen( cp ) );
			ImVfbSBlue(  rgbVfb, out, ImCltQBlue( cp ) );
		}
		break;

	case IMVFBINDEX8:	/* 8-bit color				*/
		last = ImVfbQLast( srcVfb );
		for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(rgbVfb);
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(rgbVfb,out) )
		{
			cp = ImCltQPtr( clt, ImVfbQIndex8( srcVfb, in ) ) ;
			ImVfbSRed(   rgbVfb, out, ImCltQRed( cp ) );
			ImVfbSGreen( rgbVfb, out, ImCltQGreen( cp ) );
			ImVfbSBlue(  rgbVfb, out, ImCltQBlue( cp ) );
		}
		break;

	case IMVFBINDEX16:	/* 16-bit color				*/
		last = ImVfbQLast( srcVfb );
		for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(rgbVfb);
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(rgbVfb,out) )
		{
			cp = ImCltQPtr( clt, ImVfbQIndex16( srcVfb, in ) ) ;
			ImVfbSRed(   rgbVfb, out, ImCltQRed( cp ) );
			ImVfbSGreen( rgbVfb, out, ImCltQGreen( cp ) );
			ImVfbSBlue(  rgbVfb, out, ImCltQBlue( cp ) );
		}
		break;
	}
	return ( rgbVfb );
}





/*
 *  CONSTANTS & MACRO
 *	*_PERCENT	-  percent contribution to computation of YIQ gray
 *	YIQ		-  compute YIQ grayscale from RGB
 *
 *  DESCRIPTION
 *	The red, green, and blue components of a color are combined
 *	to get a gray value via the formula:
 *
 *		gray = 0.30 * red + 0.59 * green + 0.11 * blue + 0.5
 *
 *	This is based upon the NTSC YIQ Y equation that computes the intensity
 *	value to be used in displaying a color image on a black-n-white TV.
 */

#define IM_RED_PERCENT		0.299
#define IM_GREEN_PERCENT	0.587
#define IM_BLUE_PERCENT		0.114

#define IM_NTSCY( red, green, blue )					\
((int)( IM_RED_PERCENT   * (float) (red) +			\
	IM_GREEN_PERCENT * (float) (green) +			\
	IM_BLUE_PERCENT  * (float) (blue) + 0.5 ))





/*
 *  FUNCTION
 *	ImVfbToMono	-  convert to 1-bit monochrome
 *
 *  DESCRIPTION
 *	No change:
 *		Mono VFB:
 *			Copied to 1-bit mono VFB.
 *
 *	Demotion:
 *		Index8 VFB:
 *		Index 16 VFB:
 *			If no CLT, assumed already grayscale and just
 *			thresholded and copied.  Otherwise pixels converted
 *			to NTSC Y space to generate grayscale.  Those are
 *			thresholded and copied.
 *		RGB VFB:
 *			Pixels converted to NTSC Y space to generate gray.
 *			Those are thresholded and copied.
 *
 *  NTSC Y SPACE CONVERSION
 *	The red, green, and blue components of each pixel color are combined
 *	to get a gray value via the formula:
 *
 *		gray = 0.30 * red + 0.59 * green + 0.11 * blue + 0.5
 *
 *	This is based upon the NTSC YIQ Y equation that computes the intensity
 *	value to be used in displaying a color image on a black-n-white TV.
 *
 *  MONO THRESHOLD
 *	Grayscale values less than or equal to the monoThreshold argument
 *	are considered 'black' and given a mono value of 0.  Those above
 *	the threshold are considered 'white' and given a mono value of 1.
 */

ImVfb *				/* Returns mono VFB		*/
#ifdef __STDC__
ImVfbToMono( ImVfb *srcVfb, int monoThreshold, ImVfb *dstVfb )
#else
ImVfbToMono( srcVfb, monoThreshold, dstVfb )
	ImVfb *srcVfb;			/* the vfb to change to mono	*/
	int    monoThreshold;		/* Threshold index		*/
	ImVfb *dstVfb;			/* the destination vfb 		*/
#endif
{
	int i;			/* color index counter		*/
	int g;			/* computed gray		*/
	int          fields;		/* Source fields		*/
	ImVfb       *monoVfb;		/* the new vfb to create	*/
	ImVfbPtr     in;		/* input pointer		*/
	ImVfbPtr     out;		/* output pointer		*/
	ImClt       *clt;		/* color lookup table		*/
	ImCltPtr     cp;		/* lookup table pointer		*/
	int          glt[256];		/* gray lookup table		*/
	ImVfbPtr     last;		/* Last pointer for input VFB	*/
	int          type;		/* Type of VFB			*/
	int          index;		/* Color index holder		*/


	/*
	 *  Create the new VFB, if needed.
	 */
	if ( dstVfb == IMVFBNEW )
	{
		if ( (monoVfb = ImVfbAlloc( ImVfbQWidth( srcVfb ),
			ImVfbQHeight( srcVfb ), IMVFBMONO )) == IMVFBNULL )
		{
			ImErrNo = IMEMALLOC;
			return( IMVFBNULL );
		}
	}
	else
	{
		/*
		 *  Confirm the given VFB has a mono field and that it has
		 *  the same image dimensions.
		 */
		if ( !(ImVfbQFields( dstVfb ) & IMVFBMONO) )
		{
			ImErrNo = IMENOTMONO;
			return ( IMVFBNULL );
		}
		if ( ImVfbQWidth( srcVfb ) != ImVfbQWidth( dstVfb ) )
		{
			ImErrNo = IMEWIDTH;
			return ( IMVFBNULL );
		}
		if ( ImVfbQHeight( srcVfb ) != ImVfbQHeight( dstVfb ) )
		{
			ImErrNo = IMEHEIGHT;
			return ( IMVFBNULL );
		}
		monoVfb = dstVfb;
	}


	/*
	 *  Figure out where to get input colors to be put into the mono VFB.
	 */
	clt  = ImVfbQClt( srcVfb );		/* Might be IMCLTNULL	*/
	fields = ImVfbQFields( srcVfb );
	if ( fields & IMVFBINDEX16 )
		type = IMVFBINDEX16;		/* 16-bit color index	*/
	else if ( fields & IMVFBINDEX8 )
		type = IMVFBINDEX8;		/* 8-bit color index	*/
	else if ( fields & IMVFBRGB )
		type = IMVFBRGB;		/* 24-bit RGB color	*/
	else if ( fields & IMVFBMONO )
	{
		if ( srcVfb == dstVfb )
			return ( srcVfb );	/* Already mono		*/

		last = ImVfbQLast( srcVfb );
		for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(monoVfb);
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(monoVfb,out) )
		{
			ImVfbSMono( monoVfb, out, ImVfbQMono( srcVfb, in ) );
		}
		return ( monoVfb );
	}
	else
	{
		/*
		 *  No image to convert?
		 */
		if ( dstVfb != monoVfb )
			ImVfbFree( monoVfb );
		ImErrNo = IMENOTINFO;
		return ( IMVFBNULL );
	}



	if ( clt == IMCLTNULL )
	{
		/*
		 *  No color table.  Assume grayscale.
		 */
		switch ( type )
		{
		case IMVFBINDEX8:	/* 8-bit color			*/
			last = ImVfbQLast( srcVfb );
			for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(monoVfb);
				in <= last;
				ImVfbSInc(srcVfb,in), ImVfbSInc(monoVfb,out) )
			{
				index = ImVfbQIndex8( srcVfb, in );
				if ( index > monoThreshold )
					index = 1;
				else
					index = 0;
				ImVfbSMono( monoVfb, out, index );
			}
			return ( monoVfb );

		case IMVFBINDEX16:	/* 16-bit color			*/
			last = ImVfbQLast( srcVfb );
			for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(monoVfb);
				in <= last;
				ImVfbSInc(srcVfb,in), ImVfbSInc(monoVfb,out) )
			{
				index = ImVfbQIndex16( srcVfb, in );
				if ( index > monoThreshold )
					index = 1;
				else
					index = 0;
				ImVfbSMono( monoVfb, out, index );
			}
			return ( monoVfb );

		case IMVFBRGB:		/* RGB true-color		*/
			last = ImVfbQLast( srcVfb );
			for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst( monoVfb );
				in <= last;
				ImVfbSInc(srcVfb,in), ImVfbSInc(monoVfb,out) )
			{
				g = IM_NTSCY( ImVfbQRed( srcVfb, in ),
					   ImVfbQGreen( srcVfb, in ),
					   ImVfbQBlue( srcVfb, in ) );
				if ( g > monoThreshold )
					g = 1;
				else
					g = 0;
				ImVfbSMono( monoVfb, out, g );
			}
			return ( monoVfb );
		}
	}


	/*
	 *  Has color table.  Use NTSC Y conversion.
	 */
	switch ( type )
	{
	case IMVFBINDEX8:	/* 8-bit color			*/
		/*
		 *  Scan the CLT and create a temporary gray
		 *  lookup table.  Then scan the image, looking up each
		 *  8-bit index in the gray (mono) CLT to get a mono value.
		 */
		for( i=0, cp=ImCltQFirst(clt); i<ImCltQNColors(clt); i++, ImCltSInc(clt,cp) )
		{
			g = IM_NTSCY( ImCltQRed( cp ), ImCltQGreen( cp ),
				   ImCltQBlue( cp ) );
			if ( g > monoThreshold )
				glt[i] = 1;
			else
				glt[i] = 0;
		}
		for ( ; i < 256; i++ )
			glt[i] = 0;

		last = ImVfbQLast( srcVfb );
		for( in = ImVfbQFirst(srcVfb), out = ImVfbQFirst( monoVfb );
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(monoVfb,out) )
		{
			g = glt[ ImVfbQIndex8( srcVfb, in ) ];
			ImVfbSMono( monoVfb, out, g );
		}
		return ( monoVfb );

	case IMVFBINDEX16:	/* 16-bit color				*/
		/*
		 *  16-bit (or less) index.  Creating a temporary CLT
		 *  capable of holding 2^16 = 65535 entries is a bit much.
		 *  So, the image is scanned and each pixel looked up in
		 *  the VFB's CLT, then converted a gray-scale value.
		 *  Slower, but more practical.
		 */
		last = ImVfbQLast( srcVfb );
		for( in = ImVfbQFirst(srcVfb), out = ImVfbQFirst( monoVfb );
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(monoVfb,out) )
		{
			cp = ImCltQPtr( clt, ImVfbQIndex16( srcVfb, in ) );
			g = IM_NTSCY( ImCltQRed( cp ), ImCltQGreen( cp ),
				   ImCltQBlue( cp ) );
			if ( g > monoThreshold )
				g = 1;
			else
				g = 0;
			ImVfbSMono( monoVfb, out, g );
		}
		return ( monoVfb );

	case IMVFBRGB:		/* RGB true-color			*/
		/*
		 *  Don't know what it means to have an RGB VFB with a CLT?
		 *  Ignore the CLT.
		 */
		last = ImVfbQLast( srcVfb );
		for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst( monoVfb );
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(monoVfb,out) )
		{
			g = IM_NTSCY( ImVfbQRed( srcVfb, in ),
				   ImVfbQGreen( srcVfb, in ),
				   ImVfbQBlue( srcVfb, in ) );
			if ( g > monoThreshold )
				g = 1;
			else
				g = 0;
			ImVfbSMono( monoVfb, out, g );
		}
		return ( monoVfb );
	}
	return IMVFBNULL;
}



/*
 *  FUNCTION
 *	ImVfbFSDitherToMono
 *
 *  DESCRIPTION
 *	Dither a color image to black and white, using the Floyd Steinberg
 *	algorithm.
 *      
 *      FLOYD-STEINBERG ALGORITHM
 *	   First, we convert the image to greyscale.  Then we traverse the
 *	   image from top to bottom, and left to right.  If a given pixel
 *	   has a value greater than or equal to 255, then this pixel is
 *	   set to white in the outgoing mono vfb, and the error (i.e. how much
 *	   more than 255 this pixel is) is propogated to the surrounding pixels.
 *
 *         Seven-sixteenths of the error propogates to the right neighbor, 
 *	   three-sixteenths to the bottom-left, five-sixteenths to the bottom, 
 *	   and one-sixteenths to the bottom-right.  
 *
 *                     ----   xx  7/16
 *                     3/16  5/16 1/16
 *          
 *	  The purpose of the algorithm is to enable one to dither an image in
 * 	  a single top-bottom, left-right pass.
 *
 *  RETURN VALUE
 *	The destination VFB, or IMVFBNULL if an error has occurred.
 *      
 */

ImVfb *			/* Returns mono VFB */
#ifdef __STDC__
ImVfbFSDitherToMono(ImVfb *sourceVfb, ImVfb *dstVfb)
#else
ImVfbFSDitherToMono(sourceVfb, dstVfb)
ImVfb *sourceVfb;
ImVfb *dstVfb;
#endif
{
	ImVfb *grayVfb;			/* VFB to hold grayscale image       */
	ImVfbPtr  vfbptr;		/* Ptr into a vfb                    */
	int  *dithered;			/* Dithered values                   */
	int row, column;		/* Loop indices                      */
	int width, height;		/* Width, height of image            */
	int error;			/* Error value to propogate          */

	/*
	 *  Create a new vfb if necessary
	 */

	if ( dstVfb == IMVFBNEW )
	{
		if ( (dstVfb = ImVfbAlloc( ImVfbQWidth( sourceVfb ),
			ImVfbQHeight( sourceVfb ), IMVFBMONO )) == IMVFBNULL )
		{
			ImErrNo = IMEMALLOC;
			return( IMVFBNULL );
		}
	}
	else
	{
		/*
		 *  Confirm that the given VFB has a mono field and that it 
		 *  has the same image dimensions.
		 */

		if ( !(ImVfbQFields( dstVfb ) & IMVFBMONO) )
		{
			ImErrNo = IMENOTMONO;
			return ( IMVFBNULL );
		}
		if ( ImVfbQWidth( sourceVfb ) != ImVfbQWidth( dstVfb ) )
		{
			ImErrNo = IMEWIDTH;
			return ( IMVFBNULL );
		}
		if ( ImVfbQHeight( sourceVfb ) != ImVfbQHeight( dstVfb ) )
		{
			ImErrNo = IMEHEIGHT;
			return ( IMVFBNULL );
		}
	}

	/*
	 * First, create a Grayscale version of the image with the same
	 * height and width
	 */

	width  = ImVfbQWidth (sourceVfb);
	height = ImVfbQHeight (sourceVfb);

	grayVfb = ImVfbAlloc (width, height, IMVFBGRAY);
	if (grayVfb == IMVFBNULL) 
	{
		ImErrNo = IMEMALLOC;
		return ( IMVFBNULL );
	}

	/*
	 * Next create space for the dithered image
	 */

	ImCallocRetOnError( dithered, int *, (width * height), (sizeof(int)), NULL); 

	if (dithered == NULL) 
	{
		ImErrNo = IMEMALLOC;
		return (IMVFBNULL);
	}

	/*
	 * Convert the color image to grayscale and then loop through
	 * the entire image to pull out the individual grayscale values.
	 *
	 * The grayscale values are between 0 and 255, with 0 being black
	 * and 255 being white.
	 */

	if ((ImVfbToGray (sourceVfb, grayVfb)) == IMVFBNULL) 
	{
		/* ImErrNo will have been set by ImVfbToGray */
		return ( IMVFBNULL );
	}

	  
	vfbptr = ImVfbQFirst (grayVfb);
	for (row = 0; row < height; row++) 
	{
		for (column = 0; column < width; column++) 
		{
			dithered[row*width+column] = ImVfbQGray (grayVfb, vfbptr);
			ImVfbSInc (grayVfb, vfbptr);
		}
	}

	ImVfbFree (grayVfb);		/* Free the grayscale vfb */


	/*
	 * Loop through the image and set the monochrome vfb to black or
	 * white depending on the current image value.  Then propogate
	 * the error to the neighboring pixels.
	 */

	vfbptr = ImVfbQFirst (dstVfb);
	for (row = 0; row < height; row++) 
	{
		for (column = 0; column < width; column++) 
		{

			/*
			 * If the value is over the threshold, then set the pixel
			 * white, otherwise set it to black
			 */

			if (dithered[row*width+column] > 255) 
			{
				ImVfbSMono (dstVfb, vfbptr, 1);
				error = (dithered[row*width+column]) - 255; 
			}
			else 
			{
				ImVfbSMono (dstVfb, vfbptr, 0);
				error = dithered[row*width+column];
			}

			/*
			 * Propogate the error to the right neighbor
			 */

			if (column < width - 1)
				dithered[row*width+column+1] += ((7 * error) / 16);

			 /*
			  * Propogate the error to the next row
			  */

			 if (row < height - 1) 
			 {
				dithered[(row+1)*width+column] += ((5 * error) / 16);
				if (column > 0)
					dithered[(row+1)*width+column-1] += ((3 * error) / 16);
				if (column < width -1) 
					dithered[(row+1)*width+column+1] += (error / 16);
			 }
			 ImVfbSInc (dstVfb, vfbptr);
		}
	}

	/*
	 * Free the dithered image
	 */

	free (dithered);
	return (dstVfb);

}



/*
 *  FUNCTION
 *	ImVfbToGray	-  convert to 8-bit grayscale
 *
 *  DESCRIPTION
 *	Promotion:
 *		Mono VFB:
 *			Copied to 8-bit gray VFB.  CLT copied, if any.
 *			Otherwise grayscale assumed.
 *
 *	Demotion:
 *		Index8 VFB:
 *			If no CLT, assumed already grayscale and just copied.
 *			Otherwise pixels converted to NTSC Y space to generate
 *			grayscale.
 *		Index 16 VFB:
 *			If no CLT, assumed already grayscale and just copied
 *			(truncating indexes!).  Otherwise pixels converted
 *			to NTSC Y space to generate grayscale.
 *		RGB VFB:
 *			Pixels converted to NTSC Y space to generate gray.
 *
 *  NTSC Y SPACE CONVERSION
 *	The red, green, and blue components of each pixel color are combined
 *	to get a gray value via the formula:
 *
 *		gray = 0.30 * red + 0.59 * green + 0.11 * blue + 0.5
 *
 *	This is based upon the NTSC YIQ Y equation that computes the intensity
 *	value to be used in displaying a color image on a black-n-white TV.
 */

ImVfb *				/* Returns gray VFB		*/
#ifdef __STDC__
ImVfbToGray( ImVfb *srcVfb, ImVfb *dstVfb )
#else
ImVfbToGray( srcVfb, dstVfb )
	ImVfb *srcVfb;			/* the vfb to change to gray	*/
	ImVfb *dstVfb;			/* the destination vfb 		*/
#endif
{
	int i;			/* color index counter		*/
	int g;			/* computed gray		*/
	int          fields;		/* Source fields		*/
	ImVfb       *grayVfb;		/* the new vfb to create	*/
	ImVfbPtr     in;		/* input pointer		*/
	ImVfbPtr     out;		/* output pointer		*/
	ImClt       *clt;		/* color lookup table		*/
	ImCltPtr     cp;		/* lookup table pointer		*/
	int          glt[256];		/* gray lookup table		*/
	ImVfbPtr     last;		/* Last pointer for input VFB	*/
	int          type;		/* Type of VFB			*/
	int          index;		/* Color index holder		*/


	/*
	 *  Create the new VFB, if needed.
	 */
	if ( dstVfb == IMVFBNEW )
	{
		if ( (grayVfb = ImVfbAlloc( ImVfbQWidth( srcVfb ),
			ImVfbQHeight( srcVfb ), IMVFBINDEX8 )) == IMVFBNULL )
		{
			ImErrNo = IMEMALLOC;
			return( IMVFBNULL );
		}
	}
	else
	{
		/*
		 *  Confirm the given VFB has a gray field and that it has
		 *  the same image dimensions.
		 */
		if ( !(ImVfbQFields( dstVfb ) & IMVFBINDEX8) )
		{
			ImErrNo = IMENOTINDEX8;
			return ( IMVFBNULL );
		}
		if ( ImVfbQWidth( srcVfb ) != ImVfbQWidth( dstVfb ) )
		{
			ImErrNo = IMEWIDTH;
			return ( IMVFBNULL );
		}
		if ( ImVfbQHeight( srcVfb ) != ImVfbQHeight( dstVfb ) )
		{
			ImErrNo = IMEHEIGHT;
			return ( IMVFBNULL );
		}
		grayVfb = dstVfb;
	}


	/*
	 *  Figure out where to get input colors to be put into the gray VFB.
	 */
	clt  = ImVfbQClt( srcVfb );		/* Might be IMCLTNULL	*/
	fields = ImVfbQFields( srcVfb );
	if ( fields & IMVFBINDEX16 )
		type = IMVFBINDEX16;		/* 16-bit color index	*/
	else if ( fields & IMVFBINDEX8 )
		type = IMVFBINDEX8;		/* 8-bit color index	*/
	else if ( fields & IMVFBRGB )
		type = IMVFBRGB;		/* 24-bit RGB color	*/
	else if ( fields & IMVFBMONO )
		type = IMVFBMONO;		/* 1-bit mono		*/
	else
	{
		/*
		 *  No image to convert?
		 */
		if ( dstVfb != grayVfb )
			ImVfbFree( grayVfb );
		ImErrNo = IMENOTINFO;
		return ( IMVFBNULL );
	}



	if ( clt == IMCLTNULL )
	{
		/*
		 *  No color table.  Assume grayscale.
		 */
		switch ( type )
		{
		case IMVFBMONO:		/* Monochrome			*/
			last = ImVfbQLast( srcVfb );
			for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(grayVfb);
				in <= last;
				ImVfbSInc(srcVfb,in), ImVfbSInc(grayVfb,out) )
			{
				index = ImVfbQMono( srcVfb, in );
				if ( index == 0 )
					ImVfbSIndex8( grayVfb, out, 0 );
				else
					ImVfbSIndex8( grayVfb, out, 255 );
			}
			return ( grayVfb );

		case IMVFBINDEX8:	/* 8-bit color			*/
			last = ImVfbQLast( srcVfb );
			for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(grayVfb);
				in <= last;
				ImVfbSInc(srcVfb,in), ImVfbSInc(grayVfb,out) )
			{
				ImVfbSIndex8( grayVfb, out, ImVfbQIndex8( srcVfb, in ) );
			}
			return ( grayVfb );

		case IMVFBINDEX16:	/* 16-bit color			*/
			/*
			 *  Grab the upper 8-bits of each index and treat it
			 *  as a grayscale value.
			 */
			last = ImVfbQLast( srcVfb );
			for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(grayVfb);
				in <= last;
				ImVfbSInc(srcVfb,in), ImVfbSInc(grayVfb,out) )
			{
				index = ((ImVfbQIndex16( srcVfb, in )) & 0xFF00)>>8;
				ImVfbSIndex8( grayVfb, out, index );
			}
			return ( grayVfb );

		case IMVFBRGB:		/* RGB true-color		*/
			last = ImVfbQLast( srcVfb );
			for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst( grayVfb );
				in <= last;
				ImVfbSInc(srcVfb,in), ImVfbSInc(grayVfb,out) )
			{
				g = IM_NTSCY( ImVfbQRed( srcVfb, in ),
					   ImVfbQGreen( srcVfb, in ),
					   ImVfbQBlue( srcVfb, in ) );
				ImVfbSIndex8( grayVfb, out, g );
			}
			return ( grayVfb );
		}
	}


	/*
	 *  Has color table.  Use NTSC Y conversion.
	 */
	switch ( type )
	{
	case IMVFBMONO:		/* Monochrome				*/
		/*
		 *  Create a 2-entry temporary gray lookup table.  Then scan
		 *  the image, looking up each 1-bit index in the gray CLT
		 *  to get a gray scale value.
		 */
		cp = ImCltQFirst( clt );
		glt[0] = IM_NTSCY( ImCltQRed( cp ), ImCltQGreen( cp ),
			ImCltQBlue( cp ) );
		ImCltSInc( clt, cp );
		glt[1] = IM_NTSCY( ImCltQRed( cp ), ImCltQGreen( cp ),
			ImCltQBlue( cp ) );

		last = ImVfbQLast( srcVfb );
		for( in = ImVfbQFirst(srcVfb), out = ImVfbQFirst( grayVfb );
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(grayVfb,out) )
		{
			g = glt[ ((ImVfbQMono( srcVfb, in )) & 0x1) ];
			ImVfbSIndex8( grayVfb, out, g );
		}
		return ( grayVfb );

	case IMVFBINDEX8:	/* 8-bit color				*/
		/*
		 *  Scan the CLT and create a temporary gray lookup table.
		 *  Then scan the image, looking up each 8-bit index in the
		 *  gray CLT to get a gray scale value.
		 */
		for( i=0, cp=ImCltQFirst(clt); i<ImCltQNColors(clt); i++, ImCltSInc(clt,cp) )
		{
			glt[i] = IM_NTSCY( ImCltQRed( cp ), ImCltQGreen( cp ),
				ImCltQBlue( cp ) );
		}
		for ( ; i < 256; i++ )
			glt[i] = 0;

		last = ImVfbQLast( srcVfb );
		for( in = ImVfbQFirst(srcVfb), out = ImVfbQFirst( grayVfb );
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(grayVfb,out) )
		{
			g = glt[ ImVfbQIndex8( srcVfb, in ) ];
			ImVfbSIndex8( grayVfb, out, g );
		}
		return ( grayVfb );

	case IMVFBINDEX16:	/* 16-bit color				*/
		/*
		 *  16-bit (or less) index.  Creating a temporary CLT
		 *  capable of holding 2^16 = 65536 entries is a
		 *  bit much.  So, the image is scanned and each
		 *  pixel looked up in the VFB's CLT, then converted a
		 *  gray-scale value.  Slower, but more practical.
		 */
		last = ImVfbQLast( srcVfb );
		for( in = ImVfbQFirst(srcVfb), out = ImVfbQFirst( grayVfb );
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(grayVfb,out) )
		{
			cp = ImCltQPtr( clt, ImVfbQIndex16( srcVfb, in ) );
			g  = IM_NTSCY( ImCltQRed( cp ), ImCltQGreen( cp ),
				ImCltQBlue( cp ) );
			ImVfbSIndex8( grayVfb, out, g );
		}
		return ( grayVfb );

	case IMVFBRGB:		/* RGB true-color			*/
		/*
		 *  Don't know what it means to have an RGB VFB with a CLT?
		 *  Ignore the CLT.
		 */
		last = ImVfbQLast( srcVfb );
		for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst( grayVfb );
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(grayVfb,out) )
		{
			g = IM_NTSCY( ImVfbQRed( srcVfb, in ),
				   ImVfbQGreen( srcVfb, in ),
				   ImVfbQBlue( srcVfb, in ) );
			ImVfbSIndex8( grayVfb, out, g );
		}
		return ( grayVfb );
	}
	return IMVFBNULL;
}





/* max # bits to mask off when trying RGB:				*/
#define IM_MAX_BITS_TO_MASK	1

/* range of integer values:						*/
#define IMVT_MAXR		255
#define IMVT_MAXG		255
#define IMVT_MAXB		255

#define IMVT_MAXY		255
#define IMVT_MAXU		 63
#define IMVT_MAXV		 63

/* array indices:							*/
#define IMVT_Y	0
#define IMVT_U	1
#define IMVT_V	2

#define IMVT_R	0
#define IMVT_G	1
#define IMVT_B	2

/**
 ** helpful information:
 **
 **	Y = 0.30*R + 0.59*G + 0.11*B
 **	U = B - Y
 **	V = R - Y
 **
 **/

/* macro to determine a Freq array index from integer YUV:		*/
#define IM_INDEX(iy,iu,iv)	( (iy)*(IMVT_MAXU+1)*(IMVT_MAXV+1) + (iu)*(IMVT_MAXV+1) + (iv) )

/* macros to define YUV as functions of RGB:				*/
#define IM_YFRGB(r,g,b)	(  0.30*(r)/(float)IMVT_MAXR + 0.59*(g)/(float)IMVT_MAXG + 0.11*(b)/(float)IMVT_MAXB )
#define IM_UFRGB(r,g,b)	( -0.30*(r)/(float)IMVT_MAXR - 0.59*(g)/(float)IMVT_MAXG + 0.89*(b)/(float)IMVT_MAXB )
#define IM_VFRGB(r,g,b)	(  0.70*(r)/(float)IMVT_MAXR - 0.59*(g)/(float)IMVT_MAXG - 0.11*(b)/(float)IMVT_MAXB )

/* macros to define RGB as functions of YUV:				*/
#define IM_RFYUV(y,u,v)	(int) (  (float)IMVT_MAXR * ( (y) + (v) )   )
#define IM_GFYUV(y,u,v)	(int) (  (float)IMVT_MAXG * ( (0.70-0.11)*(y) + (-0.11)*(u) + (0.70-1.00)*(v) )  /  0.59   )
#define IM_BFYUV(y,u,v)	(int) (  (float)IMVT_MAXB * ( (y) + (u) )   )

/* macros to define the integer mappings of YUV:			*/
#define IM_IYFY(y)		( (int)( (y) * (float)IMVT_MAXY ) )
#define IM_IUFU(u)		( (int)( ( (u)+0.89 ) * ( (float)IMVT_MAXU / (2.*0.89) ) ) )
#define IM_IVFV(v)		( (int)( ( (v)+0.70 ) * ( (float)IMVT_MAXV / (2.*0.70) ) ) )

/* macros to define the floating mappings of YUV:			*/
#define IM_YFIY(iy)	( (float)(iy) / (float)IMVT_MAXY )
#define IM_UFIU(iu)	( (float)(iu) * ( 2.*0.89/(float)IMVT_MAXU ) - 0.89 )
#define IM_VFIV(iv)	( (float)(iv) * ( 2.*0.70/(float)IMVT_MAXV ) - 0.70 )


/*
 *  STRUCT
 *	rgb	-  unique RGB color holder
 *	box	-  range of colors
 */

struct rgb
{
	unsigned char r_used;			/* != 0: this color is active	*/
	unsigned char r_r, r_g, r_b;		/* the color components		*/
	int r_index;			/* clt location #		*/
};


struct box
{
	int b_min[3];			/* minimum values		*/
	int b_max[3];			/* maximum values		*/
	int b_maxrange;			/* maximum( max - min )		*/
	int b_mean[3];			/* centroid			*/
	struct box *b_next;		/* linked list			*/
};




/*
 *  GLOBALS
 */

struct box	*Bfree;		/* start of box free list		*/
struct box	*Blist;		/* start of box list			*/
struct box	*Boxes;		/* will become Boxes[n]			*/
int		*Freq;		/* will become Freq[y][u][v]		*/





/**
 ** function declarations:
 **/

#ifdef __STDC__
static void	CutBox();
static void	FixUp( struct box *bp );
static int	Hash8( unsigned char r, unsigned char g, unsigned char b );
static int	Hash12( unsigned char r, unsigned char g, unsigned char b );
static void	Insert( struct box *b );
static int	NColors( struct rgb *rgb, int n, int mask, 
	int (*hash)( unsigned char r, unsigned char g, unsigned char b), ImVfb *vfb );
static int	TryYUV( ImVfb *srcVfb, ImVfb *dstVfb, int maxcolors );
static int	TryRGB( ImVfb *srcVfb, ImVfb *dstVfb, int maxcolors );
#else
static void	CutBox();
static void	FixUp();
static int	Hash8();
static int	Hash12();
static void	Insert();
static int	NColors();
static int	TryYUV();
static int	TryRGB();
#endif





/*
 *  FUNCTION
 *	ImVfbToIndex	-  change a vfb to have n colors
 *
 *  DESCRIPTION
 */

ImVfb *				/* Returns converted VFB	*/
#ifdef __STDC__
ImVfbToIndex( ImVfb *srcVfb, int nColors, ImVfb *dstVfb )
#else
ImVfbToIndex( srcVfb, nColors, dstVfb )
	ImVfb *srcVfb;			/* Source VFB			*/
	int   nColors;			/* Number of colors for
					   Destination VFB		*/
	ImVfb *dstVfb;			/* Destination VFB		*/
#endif
{
	int fields;		/* vfb field description	*/
	ImClt       *dstClt;		/* destination CLT		*/
	ImClt       *indexClt;		/* destination CLT		*/
	ImClt	    *clt;		/* Source CLT			*/
	ImVfbPtr     in;		/* input pointer		*/
	ImVfbPtr     out;		/* output pointer		*/
	int          type;		/* Type of VFB			*/
	int          index;		/* Color index holder		*/
	ImVfbPtr     last;		/* Last pointer for input VFB	*/
	ImVfb       *indexVfb;		/* New 8-bit or 16-bit color
					   index VFB			*/
	int	     bitsNeeded;	/* number of bits needed for
					   Ncolors			*/


	/*
	 *  Compute number of bits necessary to display nColors
	 */
	bitsNeeded = 0;
	while( (1 << bitsNeeded) < nColors )
	{
		++bitsNeeded;
	}
	if( bitsNeeded > 16 )
	{
		/* nColors too big */
	}

	/*
	 *  Create the new VFB, if needed.
	 */
	if ( dstVfb == IMVFBNEW )
	{
		if( bitsNeeded < 9 )
		{
			if ( (indexVfb = ImVfbAlloc( ImVfbQWidth( srcVfb ),
				ImVfbQHeight( srcVfb ), IMVFBINDEX8 )) ==
				IMVFBNULL )
				{
					ImErrNo = IMEMALLOC;
					return( IMVFBNULL );
				}
		}
		else
		{
			if ( (indexVfb = ImVfbAlloc( ImVfbQWidth( srcVfb ),
				ImVfbQHeight( srcVfb ), IMVFBINDEX16 )) ==
				IMVFBNULL )
				{
					ImErrNo = IMEMALLOC;
					return( IMVFBNULL );
				}
		}
	}
	else
	{
		/*
		 *  Confirm the given VFB has enough bits for the desired
		 *  number of colors and the same image dimensions.
		 */
		if( bitsNeeded < 9 )
		{
			if( ( !(ImVfbQFields( dstVfb ) & IMVFBINDEX8) ) &&
			    ( !(ImVfbQFields( dstVfb ) & IMVFBINDEX16) ) )
			{
				ImErrNo = IMENOTINDEX8;
				return ( IMVFBNULL );
			}
		}
		else
		{
			if( !(ImVfbQFields( dstVfb ) & IMVFBINDEX16) )
			{
				ImErrNo = IMENOTINDEX16;
				return ( IMVFBNULL );
			}
		}
		if ( ImVfbQWidth( srcVfb ) != ImVfbQWidth( dstVfb ) )
		{
			ImErrNo = IMEWIDTH;
			return ( IMVFBNULL );
		}
		if ( ImVfbQHeight( srcVfb ) != ImVfbQHeight( dstVfb ) )
		{
			ImErrNo = IMEHEIGHT;
			return ( IMVFBNULL );
		}
		indexVfb = dstVfb;
	}


	/*
	 *  Figure out where to get input colors to be put into the Index8 VFB.
	 */
	clt  = ImVfbQClt( srcVfb );		/* Might be IMCLTNULL	*/
	fields = ImVfbQFields( srcVfb );
	if ( fields & IMVFBMONO )
	{
		/* Copy the mono VFB to the index 8 VFB.		*/
		last = ImVfbQLast( srcVfb );
		for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(indexVfb);
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(indexVfb,out) )
		{
			index = ImVfbQMono( srcVfb, in );
			if ( index == 0 )
				ImVfbSIndex( indexVfb, out, 0 );
			else
				ImVfbSIndex( indexVfb, out, nColors-1 );
		}

		if ( clt != IMCLTNULL )
			dstClt = ImCltDup( clt );
		else
			dstClt = ImCltGrayRamp( IMCLTNULL, 0, nColors-1,
				 0, nColors-1, IMCLTNEW );
		if ( dstClt == IMCLTNULL )
		{
			if ( dstVfb != indexVfb )
				ImVfbFree( indexVfb );
			return ( IMVFBNULL );	/* ImErrNo already set	*/
		}
		ImVfbSClt( indexVfb, dstClt );
		return ( indexVfb );
	}
	else if( fields & IMVFBINDEX8 )
		type = IMVFBINDEX8;		/* 8-bit color index	*/
	else if ( fields & IMVFBINDEX16 )
		type = IMVFBINDEX16;		/* 16-bit color index	*/
	else if ( fields & IMVFBRGB )
		type = IMVFBRGB;		/* 24-bit RGB color	*/
	else
	{
		/*
		 *  No image to convert?
		 */
		if ( dstVfb != indexVfb )
			ImVfbFree( indexVfb );
		ImErrNo = IMENOTINFO;
		return ( IMVFBNULL );
	}


	/*
	 *  At this point we have either an Index8, Index16, or RGB VFB
	 *  to convert.
	 */


	/*
	 *  If the destination doesn't already have a CLT, add one.
	 *  Remember, the source and destination could be the same VFB.
	 */
	dstClt = ImVfbQClt( indexVfb );
	if ( dstClt == IMCLTNULL )
	{
		indexClt = ImCltAlloc( nColors-1 );
		if ( indexClt == IMCLTNULL )
		{
			if ( dstVfb != indexVfb )
				ImVfbFree( indexVfb );
			ImErrNo = IMEMALLOC;
			return( IMVFBNULL );
		}
		ImVfbSClt( indexVfb, indexClt );
	}
	else
		indexClt = dstClt;


	/* Try using masked RGB values.					*/
	if (  TryRGB( srcVfb, indexVfb, nColors-1 )  >= 0  )
		return( indexVfb );

	/* If that didn't work, cut according to YUV values.		*/
	if (  TryYUV( srcVfb, indexVfb, nColors-1 )  >=  0  )
		return( indexVfb );

	/* If didn't work (can't think why it wouldn't), return error.	*/
	if ( dstClt != indexClt )
		ImCltFree( indexClt );
	if ( dstVfb != indexVfb )
		ImVfbFree( indexVfb );
	ImErrNo = IMENOTINFO;
	return( IMVFBNULL );
}




/*
 *  FUNCTION
 *	ImVfbToIndex8	-  change a vfb to have 8-bit color
 *
 *  DESCRIPTION
 *	Promotion:
 *		Mono VFB:
 *			Copied to 8-bit VFB.  CLT copied, if any.  Otherwise
 *			2-entry grayscale VFB generated.
 *	No change:
 *		Index 8 VFB:
 *			Copied to 8-bit VFB.  CLT copied, if any.
 *
 *	Demotion:
 *		Index 16 VFB:
 *		RGB VFB:
 *			Scanned for number of unique colors in RGB and YUV
 *			space.  If too many, colors are masked to reduce
 *			their precision and the new number of unique colors
 *			counted.  Continues until we get a reasonable number
 *			of colors.
 */

ImVfb *				/* Returns converted VFB	*/
#ifdef __STDC__
ImVfbToIndex8( ImVfb *srcVfb, ImVfb *dstVfb )
#else
ImVfbToIndex8( srcVfb, dstVfb )
	ImVfb *srcVfb;			/* Source VFB			*/
	ImVfb *dstVfb;			/* Destination VFB		*/
#endif
{
	int fields;		/* vfb field description	*/
	ImClt       *dstClt;		/* destination CLT		*/
	ImClt       *indexClt;		/* destination CLT		*/
	ImClt	    *clt;		/* Source CLT			*/
	ImVfbPtr     in;		/* input pointer		*/
	ImVfbPtr     out;		/* output pointer		*/
	int          type;		/* Type of VFB			*/
	int          index;		/* Color index holder		*/
	ImVfbPtr     last;		/* Last pointer for input VFB	*/
	ImVfb       *indexVfb;		/* New 8-bit color index VFB	*/


	/*
	 *  Create the new VFB, if needed.
	 */
	if ( dstVfb == IMVFBNEW )
	{
		if ( (indexVfb = ImVfbAlloc( ImVfbQWidth( srcVfb ),
			ImVfbQHeight( srcVfb ), IMVFBINDEX8 )) == IMVFBNULL )
		{
			ImErrNo = IMEMALLOC;
			return( IMVFBNULL );
		}
	}
	else
	{
		/*
		 *  Confirm the given VFB has an Index8 field and that it has
		 *  the same image dimensions.
		 */
		if ( !(ImVfbQFields( dstVfb ) & IMVFBINDEX8) )
		{
			ImErrNo = IMENOTINDEX8;
			return ( IMVFBNULL );
		}
		if ( ImVfbQWidth( srcVfb ) != ImVfbQWidth( dstVfb ) )
		{
			ImErrNo = IMEWIDTH;
			return ( IMVFBNULL );
		}
		if ( ImVfbQHeight( srcVfb ) != ImVfbQHeight( dstVfb ) )
		{
			ImErrNo = IMEHEIGHT;
			return ( IMVFBNULL );
		}
		indexVfb = dstVfb;
	}


	/*
	 *  Figure out where to get input colors to be put into the Index8 VFB.
	 */
	clt  = ImVfbQClt( srcVfb );		/* Might be IMCLTNULL	*/
	fields = ImVfbQFields( srcVfb );
	if ( fields & IMVFBINDEX16 )
		type = IMVFBINDEX16;		/* 16-bit color index	*/
	else if ( fields & IMVFBRGB )
		type = IMVFBRGB;		/* 24-bit RGB color	*/
	else if ( fields & IMVFBMONO )
	{
		/* Copy the mono VFB to the index 8 VFB.		*/
		last = ImVfbQLast( srcVfb );
		for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(indexVfb);
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(indexVfb,out) )
		{
			index = ImVfbQMono( srcVfb, in );
			if ( index == 0 )
				ImVfbSIndex8( indexVfb, out, 0 );
			else
				ImVfbSIndex8( indexVfb, out, 255 );
		}

		if ( clt != IMCLTNULL )
			dstClt = ImCltDup( clt );
		else
			dstClt = ImCltGrayRamp( IMCLTNULL, 0, 255, 0, 255,
				 IMCLTNEW );
		if ( dstClt == IMCLTNULL )
		{
			if ( dstVfb != indexVfb )
				ImVfbFree( indexVfb );
			return ( IMVFBNULL );	/* ImErrNo already set	*/
		}
		ImVfbSClt( indexVfb, dstClt );
		return ( indexVfb );
	}
	else if ( fields & IMVFBINDEX8 )
	{
		if ( srcVfb == dstVfb )
			return ( srcVfb );	/* Already Index8	*/

		last = ImVfbQLast( srcVfb );
		for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(indexVfb);
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(indexVfb,out) )
		{
			ImVfbSIndex8( indexVfb, out, ImVfbQIndex8( srcVfb, in));
		}

		if ( clt != IMCLTNULL )
			dstClt = ImCltDup( clt );
		else
			dstClt = ImCltGrayRamp( IMCLTNULL, 0, 255, 0, 255,
				 IMCLTNEW );
		if ( dstClt == IMCLTNULL )
		{
			if ( dstVfb != indexVfb )
				ImVfbFree( indexVfb );
			return ( IMVFBNULL );	/* ImErrNo already set	*/
		}
		ImVfbSClt( indexVfb, dstClt );
		return ( indexVfb );
	}
	else
	{
		/*
		 *  No image to convert?
		 */
		if ( dstVfb != indexVfb )
			ImVfbFree( indexVfb );
		ImErrNo = IMENOTINFO;
		return ( IMVFBNULL );
	}


	/*
	 *  At this point we have either an RGB or an Index16 VFB to convert.
	 */


	/*
	 *  If the destination doesn't already have a CLT, add one.
	 *  Remember, the source and destination could be the same VFB.
	 */
	dstClt = ImVfbQClt( indexVfb );
	if ( dstClt == IMCLTNULL )
	{
		indexClt = ImCltAlloc( 256 );
		if ( indexClt == IMCLTNULL )
		{
			if ( dstVfb != indexVfb )
				ImVfbFree( indexVfb );
			ImErrNo = IMEMALLOC;
			return( IMVFBNULL );
		}
		ImVfbSClt( indexVfb, indexClt );
	}
	else
	{
		indexClt = dstClt;
	}


	/* Try using masked RGB values.					*/
	if (  TryRGB( srcVfb, indexVfb, 256 )  >= 0  )
		return( indexVfb );

	/* If that didn't work, cut according to YUV values.		*/
	if (  TryYUV( srcVfb, indexVfb, 256 )  >=  0  )
		return( indexVfb );

	/* If didn't work (can't think why it wouldn't), return error.	*/
	if ( dstClt != indexClt )
		ImCltFree( indexClt );
	if ( dstVfb != indexVfb )
		ImVfbFree( indexVfb );
	ImErrNo = IMENOTINFO;
	return( IMVFBNULL );
}





/*
 *  FUNCTION
 *	ImVfbToIndex16	-  change a vfb to have 16-bit color
 *
 *  DESCRIPTION
 *	Promotion:
 *		Mono VFB:
 *		Index8 VFB:
 *			Copied to 16-bit VFB.  CLT copied, if any.  Otherwise
 *			256-entry grayscale VFB generated.
 *	No change:
 *		Index 16 VFB:
 *			Copied to 16-bit VFB.  CLT copied, if any.
 *
 *	Demotion:
 *		RGB VFB:
 *			Scanned for number of unique colors in RGB and YUV
 *			space.  If too many, colors are masked to reduce
 *			their precision and the new number of unique colors
 *			counted.  Continues until we get a reasonable number
 *			of colors.
 */

ImVfb *				/* Returns converted VFB	*/
#ifdef __STDC__
ImVfbToIndex16( ImVfb *srcVfb, ImVfb *dstVfb )
#else
ImVfbToIndex16( srcVfb, dstVfb )
	ImVfb *srcVfb;			/* Source VFB			*/
	ImVfb *dstVfb;			/* Destination VFB		*/
#endif
{
	int fields;		/* vfb field description	*/
	ImClt       *dstClt;		/* destination CLT		*/
	ImClt       *indexClt;		/* destination CLT		*/
	ImClt	    *clt;		/* Source CLT			*/
	ImVfbPtr     in;		/* input pointer		*/
	ImVfbPtr     out;		/* output pointer		*/
	int          index;		/* Color index holder		*/
	ImVfbPtr     last;		/* Last pointer for input VFB	*/
	ImVfb       *indexVfb;		/* New 8-bit color index VFB	*/


	/*
	 *  Create the new VFB, if needed.
	 */
	if ( dstVfb == IMVFBNEW )
	{
		if ( (indexVfb = ImVfbAlloc( ImVfbQWidth( srcVfb ),
			ImVfbQHeight( srcVfb ), IMVFBINDEX16 )) == IMVFBNULL )
		{
			ImErrNo = IMEMALLOC;
			return( IMVFBNULL );
		}
	}
	else
	{
		/*
		 *  Confirm the given VFB has an Index16 field and that it has
		 *  the same image dimensions.
		 */
		if ( !(ImVfbQFields( dstVfb ) & IMVFBINDEX16) )
		{
			ImErrNo = IMENOTINDEX16;
			return ( IMVFBNULL );
		}
		if ( ImVfbQWidth( srcVfb ) != ImVfbQWidth( dstVfb ) )
		{
			ImErrNo = IMEWIDTH;
			return ( IMVFBNULL );
		}
		if ( ImVfbQHeight( srcVfb ) != ImVfbQHeight( dstVfb ) )
		{
			ImErrNo = IMEHEIGHT;
			return ( IMVFBNULL );
		}
		indexVfb = dstVfb;
	}


	/*
	 *  Figure out where to get input colors to be put into the Index16 VFB.
	 */
	clt  = ImVfbQClt( srcVfb );		/* Might be IMCLTNULL	*/
	fields = ImVfbQFields( srcVfb );
	if ( fields & IMVFBINDEX16 )
	{
		if ( srcVfb == dstVfb )
			return ( srcVfb );	/* Already Index16	*/

		last = ImVfbQLast( srcVfb );
		for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(indexVfb);
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(indexVfb,out) )
		{
			ImVfbSIndex16( indexVfb, out, ImVfbQIndex16(srcVfb,in));
		}

		if ( clt != IMCLTNULL )
			dstClt = ImCltDup( clt );
		else
			dstClt = ImCltGrayRamp( IMCLTNULL, 0, 65535, 0, 65535,
				 IMCLTNEW );
		if ( dstClt == IMCLTNULL )
		{
			if ( dstVfb != indexVfb )
				ImVfbFree( indexVfb );
			return ( IMVFBNULL );	/* ImErrNo already set	*/
		}
		ImVfbSClt( indexVfb, dstClt );
		return ( indexVfb );
	}
	else if ( fields & IMVFBINDEX8 )
	{
		/* Copy the index 8 VFB to the index 16 VFB.		*/
		last = ImVfbQLast( srcVfb );
		for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(indexVfb);
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(indexVfb,out) )
		{
			ImVfbSIndex16( indexVfb, out,
				(ImVfbQIndex8( srcVfb,in) ) );
		}

		if ( clt != IMCLTNULL )
			dstClt = ImCltDup( clt );
		else
			dstClt = ImCltGrayRamp( IMCLTNULL, 0, 65535, 0, 65535,
				 IMCLTNEW );
		if ( dstClt == IMCLTNULL )
		{
			if ( dstVfb != indexVfb )
				ImVfbFree( indexVfb );
			return ( IMVFBNULL );	/* ImErrNo already set	*/
		}
		ImVfbSClt( indexVfb, dstClt );
		return ( indexVfb );
	}
	else if ( fields & IMVFBMONO )
	{
		/* Copy the mono VFB to the index 16 VFB.		*/
		last = ImVfbQLast( srcVfb );
		for( in=ImVfbQFirst(srcVfb), out=ImVfbQFirst(indexVfb);
			in <= last;
			ImVfbSInc(srcVfb,in), ImVfbSInc(indexVfb,out) )
		{
			index = ImVfbQMono( srcVfb, in );
			if ( index == 0 )
				ImVfbSIndex16( indexVfb, out, 0 );
			else
				ImVfbSIndex16( indexVfb, out, 65535 );
		}

		if ( clt != IMCLTNULL )
			dstClt = ImCltDup( clt );
		else
			dstClt = ImCltGrayRamp( IMCLTNULL, 0, 65535, 0, 65535,
				 IMCLTNEW );
		if ( dstClt == IMCLTNULL )
		{
			if ( dstVfb != indexVfb )
				ImVfbFree( indexVfb );
			return ( IMVFBNULL );	/* ImErrNo already set	*/
		}
		ImVfbSClt( indexVfb, dstClt );
		return ( indexVfb );
	}
	else if ( !(fields & IMVFBRGB) )
	{
		/*
		 *  No image to convert?
		 */
		if ( dstVfb != indexVfb )
			ImVfbFree( indexVfb );
		ImErrNo = IMENOTINFO;
		return ( IMVFBNULL );
	}


	/*
	 *  At this point we have an RGB VFB to convert.
	 */


	/*
	 *  If the destination doesn't already have a CLT, add one.
	 *  Remember, the source and destination could be the same VFB.
	 */
	dstClt = ImVfbQClt( indexVfb );
	if ( dstClt == IMCLTNULL )
	{
		indexClt = ImCltAlloc( 65536 );
		if ( indexClt == IMCLTNULL )
		{
			if ( dstVfb != indexVfb )
				ImVfbFree( indexVfb );
			ImErrNo = IMEMALLOC;
			return( IMVFBNULL );
		}
		ImVfbSClt( indexVfb, indexClt );
	}
	else
		indexClt = dstClt;


	/* Try using masked RGB values.					*/
	if (  TryRGB( srcVfb, indexVfb, 65536 )  >= 0  )
		return( indexVfb );

	/* If that didn't work, cut according to YUV values.		*/
	if (  TryYUV( srcVfb, indexVfb, 65536 )  >=  0  )
		return( indexVfb );

	/* If didn't work (can't think why it wouldn't), return error.	*/
	if ( dstClt != indexClt )
		ImCltFree( indexClt );
	if ( dstVfb != indexVfb )
		ImVfbFree( indexVfb );
	ImErrNo = IMENOTINFO;
	return( IMVFBNULL );
}





/*
 *  FUNCTION
 *	CutBox		-  cut the largest box into 2 boxes
 *
 *  DESCRIPTION
 *	The largest box is at the top of the Blist
 *	Cut it in the direction of the largest range
 *	Cut it on its median
 *	Put the resulting boxes back in the Blist according to their new
 *	  ranges
 *
 */

static void				/* Returns nothing		*/
CutBox( )
{
	struct box *bp;	/* largest box			*/
	struct box *bpnew;	/* new box to be created	*/
	int i, j, k;			/* yuv indices			*/
	int i1, i2, i3;	/* indices in range order	*/
	int *iy, *iu, *iv;		/* where are y,u,v in i,j,k	*/
	int mid;		/* mid point (median) of yuv	*/
	int num;		/* counter to get to mid	*/
	int in;		/* Freq index			*/
	int lastnum;		/* shadow of 'num'		*/
	int split;		/* where to split the box	*/


	/* pointer to current box and new box:				*/

	bp = Blist;		Blist = Blist->b_next;
	bpnew = Bfree;		Bfree = Bfree->b_next;

	*bpnew = *bp;		/* identical copy, for now		*/


	/* determine which dimension needs to be cut:			*/

	if ( bp->b_maxrange  ==  bp->b_max[IMVT_Y] - bp->b_min[IMVT_Y] + 1 )
	{
		i1 = IMVT_Y;		i2 = IMVT_U;		i3 = IMVT_V;
		iy = &i;	iu = &j;	iv = &k;
#ifdef DEBUG
		fprintf( stderr, "Cut along Y: " );
#endif
	}
	else
	{
		if ( bp->b_maxrange  ==  bp->b_max[IMVT_U] - bp->b_min[IMVT_U] + 1 )
		{
			i1 = IMVT_U;		i2 = IMVT_Y;		i3 = IMVT_V;
			iu = &i;	iy = &j;	iv = &k;
#ifdef DEBUG
			fprintf( stderr, "Cut along U: " );
#endif
		}
		else
		{
			i1 = IMVT_V;		i2 = IMVT_Y;		i3 = IMVT_U;
			iv = &i;	iy = &j;	iu = &k;
#ifdef DEBUG
			fprintf( stderr, "Cut along V: " );
#endif
		}
	}


	/* determine the median location:				*/

	mid = 0;

	for( i = bp->b_min[i1]; i <= bp->b_max[i1]; i++ )
	{
		for( j = bp->b_min[i2]; j <= bp->b_max[i2]; j++ )
		{
			for( k = bp->b_min[i3]; k <= bp->b_max[i3]; k++ )
			{
				in = IM_INDEX( *iy, *iu, *iv );
				mid += Freq[in];
			}
		}
	}

	mid = mid / 2;


	/* find where that median occurred:				*/

	num = 0;
	lastnum = -1;

	for( i = bp->b_min[i1]; i <= bp->b_max[i1]; i++, lastnum = num )
	{
		for( j = bp->b_min[i2]; j <= bp->b_max[i2]; j++ )
		{
			for( k = bp->b_min[i3]; k <= bp->b_max[i3]; k++ )
			{
				in = IM_INDEX( *iy, *iu, *iv );
				num += Freq[in];
			}
		}
		if ( num >= mid )
			break;
	}


	/* determine where to put the split:				*/
	/* divide between 'split' and 'split+1'				*/

	if ( i <= bp->b_min[i1] )
	{
		split = i;
	}
	else
	{
		if ( i >= bp->b_max[i1] )
		{
			split = i - 1;
		}
		else
		{
			if ( num == mid )
				split = i;
			else
				split = i - 1;
		}
	}


#ifdef DEBUG
	fprintf( stderr, "%3d - %3d    +    %3d - %3d\n",
		bp->b_min[i1], split, split+1, bp->b_max[i1] );
#endif


	/* make the proper assignments into the 2 box structures:	*/

	bp->b_max[i1] = split;
	bpnew->b_min[i1] = split+1;


	/* clean-up the 2 box structures:				*/

	FixUp( bp );
	FixUp( bpnew );



	/* insert the 2 box structures where they belong:		*/

	Insert( bp );
	Insert( bpnew );


	/* done:							*/

}





/*
 *  FUNCTION
 *	FixUp		-  cleanup new or changed box structures
 *
 *  DESCRIPTION
 *	Reset b_min and b_max
 *	Re-compute b_maxrange
 *
 */

static void				/* Returns nothing		*/
#ifdef __STDC__
FixUp( struct box *bp )
#else
FixUp( bp )
	struct box *bp;	/* the box to fixup		*/
#endif
{
	int iy, iu, iv;	/* yuv values			*/
	int in;		/* Freq index			*/
	int iymin, iymax;	/* y range			*/
	int iumin, iumax;	/* u range			*/
	int ivmin, ivmax;	/* v range			*/
	int ry, ru, rv;			/* yuv ranges			*/


	/* possibly the mins and maxes:					*/
	/* these are most likely more spread out than they need		*/
	/*	to be - we will fix that below				*/

	iymin = bp->b_max[IMVT_Y];		iymax = bp->b_min[IMVT_Y];
	iumin = bp->b_max[IMVT_U];		iumax = bp->b_min[IMVT_U];
	ivmin = bp->b_max[IMVT_V];		ivmax = bp->b_min[IMVT_V];


	/* loop through the ranges setting correct mins and maxes:	*/

	for( iy = bp->b_min[IMVT_Y]; iy <= bp->b_max[IMVT_Y]; iy++ )
	{
		for( iu = bp->b_min[IMVT_U]; iu <= bp->b_max[IMVT_U]; iu++ )
		{
			for( iv = bp->b_min[IMVT_V]; iv <= bp->b_max[IMVT_V]; iv++ )
			{
				in = IM_INDEX( iy, iu, iv );
				if ( Freq[in] != 0 )
				{
					if ( iy < iymin )	iymin = iy;
					if ( iy > iymax )	iymax = iy;
					if ( iu < iumin )	iumin = iu;
					if ( iu > iumax )	iumax = iu;
					if ( iv < ivmin )	ivmin = iv;
					if ( iv > ivmax )	ivmax = iv;
				}
			}
		}
	}


	/* set the new min and max values:				*/

	bp->b_min[IMVT_Y] = iymin;		bp->b_max[IMVT_Y] = iymax;
	bp->b_min[IMVT_U] = iumin;		bp->b_max[IMVT_U] = iumax;
	bp->b_min[IMVT_V] = ivmin;		bp->b_max[IMVT_V] = ivmax;


	/* determine the maximum range for this box:			*/

	ry = bp->b_max[IMVT_Y] - bp->b_min[IMVT_Y] + 1;
	ru = bp->b_max[IMVT_U] - bp->b_min[IMVT_U] + 1;
	rv = bp->b_max[IMVT_V] - bp->b_min[IMVT_V] + 1;

	bp->b_maxrange = ry;

	if ( ru >= ry  &&  ru >= rv )
		bp->b_maxrange = ru;

	if ( rv >= ry  &&  rv >= ru )
		bp->b_maxrange = rv;


	/* done:							*/

}





/*
 *  FUNCTION
 *	Hash8		-  hash code for an 8-bit color
 *	Hash12		-  hash code for a 12-bit color
 *	Hash16		-  hash code for a 16-bit color
 *
 */

static int				/* Returns hash code		*/
#ifdef __STDC__
Hash8( unsigned char r, unsigned char g, unsigned char b )
#else
Hash8( r, g, b )
	unsigned char r, g, b;
#endif
{
	return(  ( r ^ g ^ b )  &  0xff );
}

static int				/* Returns hash code		*/
#ifdef __STDC__
Hash12( unsigned char r, unsigned char g, unsigned char b )
#else
Hash12( r, g, b )
	unsigned char r, g, b;
#endif
{
	int i1, i2;		/* temporary integers		*/

	i1 = (  ( r << 4 ) & 0x0ff0  )  |  ( g >> 4 );
	i2 = (  ( g << 8 ) & 0x0f00  )  |  b;
	return(  ( i1 ^ i2 ) & 0x0fff );
}

static int				/* Returns hash code		*/
#ifdef __STDC__
Hash16( unsigned char r, unsigned char g, unsigned char b )
#else
Hash16( r, g, b )
	unsigned char r, g, b;
#endif
{
	int i1, i2;		/* temporary integers		*/

	i1 = (  ( r << 4 ) & 0x0ff0  )  |  ( g >> 4 );
	i2 = (  ( g << 8 ) & 0x0f00  )  |  b;
	return(  ( i1 ^ i2 ) & 0x0fff );
}





/*
 *  FUNCTION
 *	Insert		-  insert a box into the box list
 *
 */

static void				/* Returns nothing		*/
#ifdef __STDC__
Insert( struct box *b )
#else
Insert( b )
	struct box *b;
#endif
{
	struct box *bp;	/* box pointer			*/
	struct box *last;	/* shadow box pointer		*/


	/* if list is empty, create it with this element:		*/

	if ( Blist == NULL )
	{
		Blist = b;
		b->b_next = NULL;
		return;
	}



	/* otherwise, do an insertion sort:				*/

	for( bp = Blist, last = NULL; bp != NULL; last = bp, bp = bp->b_next )
	{
		/* loop until bp->b_maxrange gets small enough:		*/

		if ( b->b_maxrange  <  bp->b_maxrange )
			continue;

		/* if at first point, make it head of list:		*/

		if ( last == NULL )
		{
			b->b_next = Blist;
			Blist = b;
		}
		else		/* otherwise, link it in		*/
		{
			b->b_next = bp;
			last->b_next = b;
		}

		return;

	}



	/* if got here then add to end of list:				*/

	b->b_next = NULL;
	last->b_next = b;

}





/*
 *  FUNCTION
 *	NColors		-  count the # colors that will result if
 *			   the given mask is applied to all RGB values
 *
 *  DESCRIPTION
 *	This routine looks through all RGB values in the given vfb
 *	  (applying a bitmask to each) and keeps a table of unique
 *	  combinations.  If that table overflows, then this mask
 *	  cannot be used to quantize the colors.
 *
 *	A return of -1 indicates that the table overflowed
 *	A return of >= 0 indicates the number of unique combinations
 */

static int			/* Returns # of colors			*/
#ifdef __STDC__
NColors( struct rgb *rgb, int n, int mask, 
	int (*hash)(unsigned char r, unsigned char g, unsigned char b), ImVfb *vfb )
#else
NColors( rgb, n, mask, hash, vfb )
	struct rgb *rgb;	/* rgb array (already alloc'd)	*/
	int n;		/* max # colors allowed			*/
	int mask;	/* bitmask to apply to all rgb's	*/
	int (*hash)();		/* hash function to index into rgb[]	*/
	ImVfb *vfb;		/* the vfb to browse			*/
#endif
{
	int i;		/* counter				*/
	int r, g, b;	/* rgb values				*/
	int h;		/* hash index				*/
	int type;		/* type of vfb				*/
	int nunique;		/* # unique combinations		*/
	struct rgb *rgbp;	/* pointer into rgb[]		*/
	ImVfbPtr p;	/* pointer into the vfb			*/
	ImClt *clt;		/* clt attached to vfb			*/
	ImCltPtr cp;		/* pointer into clt			*/



	/* determine vfb type:						*/

	if ( ImVfbQFields(vfb) & IMVFBRGB )
		type = IMVFBRGB;
	else
	{
		if ( ImVfbQFields(vfb) & IMVFBINDEX8 )
			type = IMVFBINDEX8;
		else
			type = IMVFBINDEX16;

		clt = ImVfbQClt( vfb );
	}


	/* set all rgb[].r_used to unused:				*/

	for( i = 0, rgbp = rgb; i < n; i++, rgbp++ )
		rgbp->r_used = 0;


	/* count the unique values:					*/
	/* if table overflows, return an error				*/

	nunique = 0;

	for( p = ImVfbQFirst( vfb ); p <= ImVfbQLast( vfb ); ImVfbSInc( vfb, p ) )
	{
		if ( type == IMVFBRGB )
		{
			r = ImVfbQRed( vfb, p ) & mask;
			g = ImVfbQGreen( vfb, p ) & mask;
			b = ImVfbQBlue( vfb, p ) & mask;
		}
		else
		{
			cp = ImCltQPtr( clt, ImVfbQIndex( vfb, p ) );
			r = ImCltQRed( cp ) & mask;
			g = ImCltQGreen( cp ) & mask;
			b = ImCltQBlue( cp ) & mask;
		}

		h = (*hash)( r, g, b );		/* hash index		*/


		/* find this rgb combo in rgb[]:			*/
		/* if cannot find it, try to create it			*/
		/* if cannot create it, then table is full		*/

		for( i = 0, rgbp = &rgb[h]; i < n; i++, rgbp++ )
		{
			if ( rgbp >= &rgb[n] )
				rgbp = rgb;


			/* if find an unused slot, create this rgb:	*/

			if ( rgbp->r_used == 0 )
			{
				nunique++;
				rgbp->r_used = 1;
				rgbp->r_r = r;
				rgbp->r_g = g;
				rgbp->r_b = b;
				break;
			}


			/* if slot is used, see if it matches r, g, and b:*/

			if ( rgbp->r_r == r  &&  rgbp->r_g == g  &&  rgbp->r_b == b )
				break;


			/* if got to here, then haven't found an exact	*/
			/*   match, and haven't found an empty slot to	*/
			/*   create a new entry in - keep looking	*/

		}



		/* if got to here because no more array locations to	*/
		/*   look at, table is full and colors cannot be	*/
		/*   quantized with this bitmask:			*/

		if ( i >= n )
			return( -1 );


		/* keep browsing through the vfb:			*/

	}


	/* if get here, then colors can be quantized with this bitmask:	*/

	return( nunique );
}







/*
 *  FUNCTION
 *	TryRGB		-  see if an RGB model will work with this
 *			   number of colors
 *
 *  DESCRIPTION
 *	This routine will see if there are <= N unique RGB combinations
 *	If not, it will then start to mask off bits from the end of
 *	 the RGB components and see if there are <= N unique combinations
 *	 of those
 *
 *	A return of -1 indicates that the destination vfb is screwy
 *	A return of -2 indicates that a malloc failed
 *	A return of -3 indicates that there were too many unique
 *	 combinations and that these colors will need to be quantized further
 */

static int			/* REturns status			*/
#ifdef __STDC__
TryRGB( ImVfb *srcVfb, ImVfb *dstVfb, int maxcolors )
#else
TryRGB( srcVfb, dstVfb, maxcolors )
	ImVfb *srcVfb;		/* source vfb				*/
	ImVfb *dstVfb;		/* destination vfb			*/
	int maxcolors;		/* max # colors allowed			*/
#endif
{
	int mask;	/* bit mask				*/
	int comp;	/* complement to the bitmask		*/
	struct rgb *rgb;	/* array of rgb values		*/
	struct rgb *rgbp;	/* pointer into rgb array	*/
	int i;		/* counter				*/
	int r, g, b;	/* rgb values				*/
	int h;		/* hash index				*/
	int index;	/* index into clt			*/
	int nc;			/* return from NColors()		*/
	int type;		/* type of frame buffer			*/
	ImVfbPtr psrc, pdst;	/* pointers into the vfbs		*/
	ImClt *clt;		/* color lookup table			*/
	ImCltPtr cp;		/* clt pointer				*/
#ifdef __STDC__
	int (*hash)(unsigned char r, unsigned char g, unsigned char b);	/* hash function */
#else
	int (*hash)();		/* hash function			*/
#endif


	if( ( maxcolors < 0 ) || ( maxcolors > 65536 ) )
	{
		return( -1 );
	}
	else if( maxcolors > 4096 )
	{
		hash = Hash16;
	}
	else if( maxcolors > 256 )
	{
		hash = Hash12;
	}
	else
	{
		hash = Hash8;
	}

	/* allocate rgb array:						*/

	rgb = (struct rgb *) malloc( maxcolors * sizeof( struct rgb ) );
	if ( rgb == NULL )
	{
		return( -2 );
	}



	/* try to fit all the colors into maxcolors slots:		*/

	for( i=0, comp=0; i <= IM_MAX_BITS_TO_MASK; i++, comp = ( comp << 1 ) | 1 )
	{
		mask = ~comp;
		nc = NColors( rgb, maxcolors, mask, hash, srcVfb );

#ifdef DEBUG
		fprintf( stderr, "NColors( rgb, %d, 0x%0x, hash, srcVfb ) = %d\n",
			maxcolors, mask, nc );
#endif
		if ( nc >= 0 )		/* successful !			*/
			break;
	}


	/* if all calls to NColors() failed, return an error:		*/

	if ( nc < 0 )
	{
		free( (char *) rgb );
		return( -3 );
	}



	/* if a call to NColors() succeeded, fill dstVfb:		*/


	/* first, pack colors in top of clt (point to top anyway):	*/

	for( i = 0, rgbp = &rgb[0], index = 0; i < maxcolors; i++, rgbp++ )
	{
		if ( rgbp->r_used == 0 )
			continue;

		rgbp->r_index = index;
		index++;
	}


	/* determine type of vfb the input is:				*/

	if ( ImVfbQFields(srcVfb) & IMVFBRGB )
		type = IMVFBRGB;
	else
	{
		if ( ImVfbQFields(srcVfb) & IMVFBINDEX8 )
			type = IMVFBINDEX8;
		else
			type = IMVFBINDEX16;

		clt = ImVfbQClt( srcVfb );
	}


	/* fill the output vfb with correct color indices:		*/

	for( psrc = ImVfbQFirst(srcVfb), pdst = ImVfbQFirst(dstVfb);
		psrc <= ImVfbQLast(srcVfb);
		ImVfbSInc(srcVfb,psrc), ImVfbSInc(dstVfb,pdst) )
	{
		if ( type == IMVFBRGB )
		{
			r = ImVfbQRed( srcVfb, psrc ) & mask;
			g = ImVfbQGreen( srcVfb, psrc ) & mask;
			b = ImVfbQBlue( srcVfb, psrc ) & mask;
		}
		else
		{
			cp = ImCltQPtr( clt, ImVfbQIndex( srcVfb, psrc ) );
			r = ImCltQRed( cp ) & mask;
			g = ImCltQGreen( cp ) & mask;
			b = ImCltQBlue( cp ) & mask;
		}


		/* locate that RGB combination in the hash table:	*/

		h = (*hash)( r, g, b );
		
		for( i = 0, rgbp = &rgb[h]; i < maxcolors; i++, rgbp++ )
		{
			if ( rgbp >= &rgb[maxcolors] )
				rgbp = rgb;

			if ( rgbp->r_used == 0 )
				continue;

			if ( rgbp->r_r == r  &&  rgbp->r_g == g
					    &&  rgbp->r_b == b )
				break;
		}

		/* we know that this will succeed sooner or later...	*/

		index = rgbp - rgb;
		ImVfbSIndex( dstVfb, pdst, rgb[index].r_index );

	}


	/* fill the new clt:						*/


	clt = ImVfbQClt( dstVfb );

	if ( nc != maxcolors )
	{
		ImCltFree( clt );		/* get rid of full clt	*/
		clt = ImCltAlloc( nc );
		ImVfbSClt( dstVfb, clt );
	}

	for( i = 0, cp = ImCltQFirst(clt), rgbp = rgb;
		i < maxcolors;
		i++, rgbp++ )
	{
		if ( rgbp->r_used )
		{
			ImCltSRed( cp, rgbp->r_r );
			ImCltSGreen( cp, rgbp->r_g );
			ImCltSBlue( cp, rgbp->r_b );
			ImCltSInc( clt, cp );
		}
	}


	/* end game:							*/

	free( (char *) rgb );

	return( nc );
}





/*
 *  FUNCTION
 *	TryYUV		-  quantize the colors down with a YUV model
 *
 *  DESCRIPTION
 *	This routine will see if there are <= N unique YUV combinations
 *	If not, it will then start to mask off bits from the end of
 *	 the YUV components and see if there are <= N unique combinations
 *	 of those
 *
 *	A return of -1 indicates that the destination vfb is screwy
 *	A return of -2 indicates that a malloc failed
 */

static int			/* Returns status			*/
#ifdef __STDC__
TryYUV( ImVfb *srcVfb, ImVfb *dstVfb, int maxcolors )
#else
TryYUV( srcVfb, dstVfb, maxcolors )
	ImVfb *srcVfb;		/* source vfb				*/
	ImVfb *dstVfb;		/* destination vfb			*/
	int maxcolors;		/* # colors				*/
#endif
{
	int i;		/* counter				*/
	int in;	/* Freq index				*/
	int iy, iu, iv;	/* yuv indices			*/
	int num;	/* total # colors			*/
	int r, g, b;		/* rgb values				*/
	int iymin, iymax, iumin, iumax, ivmin, ivmax;	/* yuv ranges	*/
	int type;		/* type of vfb				*/
	int ymean, umean, vmean;	/* accumulate avg yuv values	*/
	int denom;		/* denominator for the avg yuv values	*/
	struct box *bp;	/* box pointer			*/
	ImVfbPtr psrc, pdst;	/* vfb pointers				*/
	ImClt *clt;		/* clt attached to srcVfb / dstVfb	*/
	ImCltPtr cp;		/* clt pointer				*/



	/* # colors to quantize to:					*/
	if ( ( maxcolors < 0 ) || ( maxcolors > 65536 ) )
	{
		return( -1 );
	}


	/* allocate the Frequency array:				*/

	Freq = (int *) malloc( (IMVT_MAXY+1)*(IMVT_MAXU+1)*(IMVT_MAXV+1)*sizeof(int) );
	if ( Freq == NULL )
		return( -2 );


	/* allocate Boxes structure:					*/

	Boxes = (struct box *) malloc( maxcolors * sizeof( struct box ) );
	if ( Boxes == NULL )
	{
		free( (char *) Freq );
		return( -2 );
	}


	/* setup used and free lists:					*/

	Blist = NULL;
	Bfree = Boxes;
	for( i=0; i<maxcolors-1; i++ )
		Boxes[i].b_next = &Boxes[i+1];
	Boxes[maxcolors-1].b_next = NULL;



	/* zero out Frequency array:					*/

	memset( (void *) Freq, 0x00, (IMVT_MAXY+1)*(IMVT_MAXU+1)*(IMVT_MAXV+1)*sizeof(int) );



	/* determine vfb type:						*/

	if ( ImVfbQFields(srcVfb) & IMVFBRGB )
		type = IMVFBRGB;
	else
	{
		if( ImVfbQFields( srcVfb ) & IMVFBINDEX8 )
			type = IMVFBINDEX8;
		else
			type = IMVFBINDEX16;
		clt = ImVfbQClt( srcVfb );
	}



	/* fill the frequency array and determine range:		*/

	iymin = iumin = ivmin =  100000;
	iymax = iumax = ivmax = -100000;
	num = 0;

	for( psrc = ImVfbQFirst(srcVfb); psrc <= ImVfbQLast(srcVfb); ImVfbSInc(srcVfb,psrc) )
	{
		if ( type == IMVFBRGB )
		{
			r = ImVfbQRed( srcVfb, psrc );
			g = ImVfbQGreen( srcVfb, psrc );
			b = ImVfbQBlue( srcVfb, psrc );
		}
		else
		{
			cp = ImCltQPtr( clt, ImVfbQIndex( srcVfb, psrc ) );
			r = ImCltQRed( cp );
			g = ImCltQGreen( cp );
			b = ImCltQBlue( cp );
		}

		iy = IM_IYFY( IM_YFRGB( r, g, b ) );
		iu = IM_IUFU( IM_UFRGB( r, g, b ) );
		iv = IM_IVFV( IM_VFRGB( r, g, b ) );

		in = IM_INDEX( iy, iu, iv );
		if ( Freq[in] == 0 )
			num++;
		Freq[in]++;


		if ( iy < iymin )	iymin = iy;
		if ( iy > iymax )	iymax = iy;
		if ( iu < iumin )	iumin = iu;
		if ( iu > iumax )	iumax = iu;
		if ( iv < ivmin )	ivmin = iv;
		if ( iv > ivmax )	ivmax = iv;
	}

#ifdef DEBUG
	fprintf( stderr, "\nYUV bounds on original data:\n" );
	fprintf( stderr, "\tY :\t%4d  -  %4d\n", iymin, iymax );
	fprintf( stderr, "\tU :\t%4d  -  %4d\n", iumin, iumax );
	fprintf( stderr, "\tV :\t%4d  -  %4d\n", ivmin, ivmax );
	fprintf( stderr, "%d unique YUV combinations\n", num );
#endif


	/* setup the first box:						*/

	Blist = bp = Bfree;
	Bfree = Bfree->b_next;
	bp->b_next = NULL;

	bp->b_min[IMVT_Y] = iymin;
	bp->b_max[IMVT_Y] = iymax;
	bp->b_min[IMVT_U] = iumin;
	bp->b_max[IMVT_U] = iumax;
	bp->b_min[IMVT_V] = ivmin;
	bp->b_max[IMVT_V] = ivmax;

	bp->b_maxrange = iymax - iymin;
	if ( iumax - iumin  >  bp->b_maxrange )
		bp->b_maxrange = iumax - iumin;
	if ( ivmax - ivmin  >  bp->b_maxrange )
		bp->b_maxrange = ivmax - ivmin;

	bp->b_maxrange++;	/* range is actually 1 larger		*/



	/* cut the box (n-1) times:					*/

	for( i = 1; i < maxcolors; i++ )
		CutBox();



	/* re-use the Frequency array to hold index into the box #:	*/
	/* at the same time, compute the RGB mean in each box:		*/

	for( i = 0, bp = Blist; i < maxcolors; i++, bp = bp->b_next )
	{
		ymean = umean = vmean = 0;
		denom = 0;

		for( iy = bp->b_min[IMVT_Y]; iy <= bp->b_max[IMVT_Y]; iy++ )
		{
			for( iu = bp->b_min[IMVT_U]; iu <= bp->b_max[IMVT_U]; iu++ )
			{
				for( iv = bp->b_min[IMVT_V]; iv <= bp->b_max[IMVT_V]; iv++ )
				{
					in = IM_INDEX( iy, iu, iv );

					if ( Freq[in] > 0 )
					{
						ymean += iy * Freq[in];
						umean += iu * Freq[in];
						vmean += iv * Freq[in];
						denom += Freq[in];
					}

					Freq[in] = i;
				}
			}
		}

		if ( denom > 0 )
		{
			ymean /= denom;
			umean /= denom;
			vmean /= denom;
		}

		bp->b_mean[IMVT_R] = IM_RFYUV( IM_YFIY(ymean), IM_UFIU(umean), IM_VFIV(vmean) );
		bp->b_mean[IMVT_G] = IM_GFYUV( IM_YFIY(ymean), IM_UFIU(umean), IM_VFIV(vmean) );
		bp->b_mean[IMVT_B] = IM_BFYUV( IM_YFIY(ymean), IM_UFIU(umean), IM_VFIV(vmean) );

		if ( bp->b_mean[IMVT_R] > 255 )	bp->b_mean[IMVT_R] = 255;
		if ( bp->b_mean[IMVT_G] > 255 )	bp->b_mean[IMVT_G] = 255;
		if ( bp->b_mean[IMVT_B] > 255 )	bp->b_mean[IMVT_B] = 255;

		if ( bp->b_mean[IMVT_R] <   0 )	bp->b_mean[IMVT_R] =   0;
		if ( bp->b_mean[IMVT_G] <   0 )	bp->b_mean[IMVT_G] =   0;
		if ( bp->b_mean[IMVT_B] <   0 )	bp->b_mean[IMVT_B] =   0;

#ifdef DEBUG
		fprintf( stderr, "Color %3d: YUV = %4d , %4d , %4d\t\t", i,
			ymean, umean, vmean );
		fprintf( stderr, "RGB = %3d , %3d , %3d\n",
			bp->b_mean[R], bp->b_mean[G], bp->b_mean[B] );
#endif


	}



	/* insert the indices into the destination vfb:			*/

	for( psrc = ImVfbQFirst(srcVfb), pdst = ImVfbQFirst(dstVfb);
		psrc <= ImVfbQLast(srcVfb);
		ImVfbSInc(srcVfb,psrc), ImVfbSInc(dstVfb,pdst) )
	{
		if ( type == IMVFBRGB )
		{
			r = ImVfbQRed( srcVfb, psrc );
			g = ImVfbQGreen( srcVfb, psrc );
			b = ImVfbQBlue( srcVfb, psrc );
		}
		else
		{
			cp = ImCltQPtr( clt, ImVfbQIndex( srcVfb, psrc ) );
			r = ImCltQRed( cp );
			g = ImCltQGreen( cp );
			b = ImCltQBlue( cp );
		}


		/* locate that RGB combination in the Freq array:	*/

		iy = IM_IYFY(  IM_YFRGB( r, g, b )  );
		iu = IM_IUFU(  IM_UFRGB( r, g, b )  );
		iv = IM_IVFV(  IM_VFRGB( r, g, b )  );
		in = IM_INDEX( iy, iu, iv );
		
		ImVfbSIndex( dstVfb, pdst, Freq[in] );

	}


	/* fill the new clt:						*/


	clt = ImVfbQClt( dstVfb );

	for( i = 0, cp = ImCltQFirst(clt), bp = Blist;
		i < maxcolors;
		i++, ImCltSInc(clt,cp), bp = bp->b_next )
	{
		ImCltSRed( cp, bp->b_mean[IMVT_R] );
		ImCltSGreen( cp, bp->b_mean[IMVT_G] );
		ImCltSBlue( cp, bp->b_mean[IMVT_B] );
	}


	/* end game:							*/

	free( (char *) Boxes );
	free( (char *) Freq );

	return( maxcolors );
}
