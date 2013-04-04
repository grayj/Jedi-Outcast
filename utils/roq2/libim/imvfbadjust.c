/**

 **	$Header: /roq/libim/imvfbadjust.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imvfbadjust.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imvfbadjust.c - adjust a VFB

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imvfbadjust.c contains routines for adjusting components of pixels in a VFB

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **	ImVfbAdjust	f  adjust the field values of a virtual frame buffer

 **

 **  PRIVATE CONTENTS

 **	imAdjustPixel	m  adjust a pixel component

 **	imCheckField	m  check that a field value is within legal range

 **	IM_ERROR	m  return an error

 **

 **  HISTORY

 **     $Log: /roq/libim/imvfbadjust.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.7  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.6  1994/10/03  11:29:45  nadeau

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

 **	Revision 1.5  92/12/03  01:53:59  nadeau

 **	Changed names of ImRgbHsi and ImHsiRgb to new names.

 **	

 **	Revision 1.4  92/09/01  20:11:06  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.3  92/09/01  12:24:45  nadeau

 **	Rearranged and rewrote much of the code to add in additional

 **	error checks, support keying off of non-RGB images when given

 **	RGB or HSI key fields, support monochrome, write protect,

 **	Z-buffer, integer data, and float data fields.

 **	

 **	Revision 1.2  92/08/26  12:45:45  groening

 **	minor error corrections

 **    

 **	Revision 1.1  92/08/10  08:52:09  groening

 **	Initial revision

 **    

 **	

 **/



/**

 **  CODE CREDITS

 **     Custom development, Chris Groening, San Diego Supercomputer Center, 1992.

 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1992.

 **/

#include "iminternal.h"











/*

 *  MACRO

 *	imAdjustPixel	-  adjust a pixel component

 *

 *  DESCRIPTION

 *	Used by each of the adjustment operators of ImVfbAdjust(),

 *	this macro queries a pixel's component value, adjusts it as

 *	requested, checks that it is a legal value, then saves it

 *	back to the VFB.

 *

 *	The macro is provide to encapsulate an algorithm that is

 *	nearly identical for each adjustment operator.  Note that

 *	the 'op' argument is a C operator, like "+=" or "*=" that

 *	is substituted into the code.  It is not a variable and is

 *	not a character string.  This macro should be invoked with

 *	a statement like:

 *

 *		imAdjustPixel( += )

 *

 *	in order to implement an ADD operation.  Pretty tricky, huh?

 */

#define imAdjustPixel(op)						\
{									\
	float   hsi[3];							\
	int     rgb[3];							\
	switch ( adjustField )						\
	{								\
	case IMRED:							\
		pixel = ImVfbQRed( dstVfb, pDst );			\
		pixel op value;						\
		if ( pixel > 255 ) pixel = 255;				\
		if ( pixel < 0   ) pixel = 0;				\
		ImVfbSRed( dstVfb, pDst, (int)pixel );			\
		break;							\
	case IMGREEN:							\
		pixel = ImVfbQGreen( dstVfb, pDst );			\
		pixel op value;						\
		if ( pixel > 255 ) pixel = 255;				\
		if ( pixel < 0   ) pixel = 0;				\
		ImVfbSGreen( dstVfb, pDst, (int)pixel );		\
		break;							\
	case IMBLUE:							\
		pixel = ImVfbQBlue( dstVfb, pDst );			\
		pixel op value;						\
		if ( pixel > 255 ) pixel = 255;				\
		if ( pixel < 0   ) pixel = 0;				\
		ImVfbSBlue( dstVfb, pDst, (int)pixel );			\
		break;							\
	case IMINDEX8:							\
		pixel = ImVfbQIndex8( dstVfb, pDst );			\
		pixel op value;						\
		if ( pixel > 255 ) pixel = 255;				\
		if ( pixel < 0   ) pixel = 0;				\
		ImVfbSIndex8( dstVfb, pDst, (int)pixel );		\
		break;							\
	case IMINDEX16:							\
		pixel = ImVfbQIndex16( dstVfb, pDst );			\
		pixel op value;						\
		if ( pixel > 65535 ) pixel = 65535;			\
		if ( pixel < 0     ) pixel = 0;				\
		ImVfbSIndex16( dstVfb, pDst, (int)pixel );		\
		break;							\
	case IMMONO:							\
		pixel = ImVfbQMono( dstVfb, pDst );			\
		pixel op value;						\
		if ( pixel > 1 ) pixel = 1;				\
		if ( pixel < 0 ) pixel = 0;				\
		ImVfbSMono( dstVfb, pDst, (int)pixel );			\
		break;							\
	case IMHUE:							\
	case IMSATURATION:						\
	case IMINTENSITY:						\
		rgb[0] = ImVfbQRed( dstVfb, pDst );			\
		rgb[1] = ImVfbQGreen( dstVfb, pDst );			\
		rgb[2] = ImVfbQBlue( dstVfb, pDst );			\
		ImRgbToHsi( rgb, hsi );					\
		if ( adjustField == IMHUE )		hsi[0] op value;\
		else if ( adjustField == IMSATURATION) hsi[1] op value;\
		else 					hsi[2] op value;\
		ImHsiToRgb( hsi, rgb );					\
		ImVfbSRed( dstVfb, pDst,   rgb[0] );			\
		ImVfbSGreen( dstVfb, pDst, rgb[1] );			\
		ImVfbSBlue( dstVfb, pDst,  rgb[2] );			\
		break;							\
	case IMALPHA:							\
		pixel = ImVfbQAlpha( dstVfb, pDst );			\
		pixel op value;						\
		if ( pixel > 255 ) pixel = 255;				\
		if ( pixel < 0 )   pixel = 0;				\
		ImVfbSAlpha( dstVfb, pDst, (int)pixel );		\
		break;							\
	case IMWPROT:							\
		pixel = ImVfbQWProt( dstVfb, pDst );			\
		pixel op value;						\
		if ( pixel > 255 ) pixel = 255;				\
		if ( pixel < 0 )   pixel = 0;				\
		ImVfbSWProt( dstVfb, pDst, (int)pixel );		\
		break;							\
	case IMZ:							\
		pixel = ImVfbQZ( dstVfb, pDst );			\
		pixel op value;						\
		ImVfbSZ( dstVfb, pDst, (int)pixel );			\
		break;							\
	case IMFDATA:							\
		pixel = ImVfbQFData( dstVfb, pDst );			\
		pixel op value;						\
		ImVfbSFData( dstVfb, pDst, pixel );			\
		break;							\
	case IMIDATA:							\
		pixel = ImVfbQIData( dstVfb, pDst );			\
		pixel op value;						\
		ImVfbSIData( dstVfb, pDst, (int)pixel );		\
		break;							\
	}								\
}









/*

 *  MACRO

 *	IM_ERROR	-  return an error code

 *

 *  DESCRIPTION

 *	This macro encapsulates setting ImErrNo and returning and is

 *	used just to make the code shorter and easier to read.

 */

#define IM_ERROR(errnum)						\
{									\
	ImErrNo = (errnum);						\
	return( IMVFBNULL );						\
}











/*

 *  MACRO

 *	imCheckField	-  check that a value is within a legal range

 *

 *  DESCRIPTION

 *	This macro is used to check each of the key and adjust values

 *	to make sure they are within acceptable ranges.

 */

#define imCheckField(extent,error)					\
{									\
	switch ( operation )						\
	{								\
	case IMADD:							\
	case IMSUBTRACT:						\
		if ( adjustLow < -(extent) || adjustHigh > (extent) )	\
			IM_ERROR( error );				\
		break;							\
	case IMMULTIPLY:						\
		if ( adjustLow < 0 )					\
			IM_ERROR( error );				\
		break;							\
	case IMDIVIDE:							\
		if ( adjustLow <= 0 )					\
			IM_ERROR( error );				\
		break;							\
	case IMSET:							\
		if ( adjustLow < 0 || adjustHigh > (extent) )		\
			IM_ERROR( error );				\
		break;							\
	}								\
}











/*

 * FUNCTION

 *	ImVfbAdjust - adjust the  fields in a virtual frame buffer.

 *

 * DESCRIPTION

 *	The source image's pixels are adjusted and written to the destination

 *	image.  Adjustements are performed on pixels selected by the 'key'

 *	arguments and modify those pixel's 'adjust' fields using the given

 *	operation.

 *

 *	The majority of the code here is just to check that all the incomming

 *	arguments make sense.  Whew!

 */



#ifdef __STDC__

ImVfb *			/* Returns filled destination vfb 	*/

ImVfbAdjust( ImVfb *srcVfb, int keyField, double keyStart, double keyEnd, int operation, 

	int adjustField, double adjustStart, double adjustEnd, ImVfb *dstVfb )

#else

ImVfb *			/* Returns filled destination vfb 	*/

ImVfbAdjust( srcVfb, keyField, keyStart, keyEnd, operation, 

		adjustField, adjustStart, adjustEnd, dstVfb)

	ImVfb	*srcVfb;	/* incoming vfb				*/

	int	keyField;	/* what field to change			*/

	double	keyStart;	/* Start range of field to change	*/

	double	keyEnd;		/* End range of field to change		*/

	int	operation;	/* What operation to perform on field   */

	int	adjustField;	/* what field to adjust			*/

	double	adjustStart;	/* Start range of field to adjust	*/

	double	adjustEnd;	/* End range of field to adjust		*/

	ImVfb	*dstVfb;	/* Destination vfb			*/

#endif

{

	int     x,y;		/* Width and height of image		*/

	int     i,j;		/* Counters				*/

	float	factor;		/* Replacement value factor		*/

	int     fields;		/* Fields of source VFB			*/

	int     pixelRgb[3];	/* RGB pixel value			*/

	float   pixelHsi[3];	/* HSI pixel value			*/

	float   pixel;		/* Pixel value				*/

	float   value;		/* Value with which to modify pixel	*/

	ImClt  *clt;		/* Image's color lookup table		*/

	ImVfbPtr pDst;		/* Pointer to destination VFB		*/

	float   keyLow;		/* Low key value			*/

	float   keyHigh;	/* High key value			*/

	float   adjustLow;	/* Low adjust value			*/

	float   adjustHigh;	/* High adjust value			*/





	/*

	 *  Check for legal key and adjust values.  This is made a bit

	 *  tricky because of the following:

	 *

	 *	1.  Different field types have different legal key value

	 *	    ranges.

	 *

	 *	2.  Replace value ranges depend on the operation being

	 *	    performed, as well as upon the adjust field's type.

	 *

	 *   Key fields are restricted as follows:

	 *		mono:					0 to 1

	 *		red, green, blue, alpha, index8, wprot:	0 to 255

	 *		index16:				0 to 65535

	 *		z:					0 to maxint

	 *		idata:					any int

	 *		fdata:					any float

	 *		hue:					0 to 360.0

	 *		saturation, intensity:			0 to 1.0

	 *

	 *  Adjust fields are restricted as follows:

	 *

	 *	Add and subtract:

	 *	    It doesn't make much sense to add more, or subtract more

	 *	    than the max or min field value.  Doing so puts the result

	 *	    beyond the max or min, and will just get clamped back to

	 *	    the max or min anyway.

	 *		mono:					-1 to 1

	 *		red, green, blue, alpha, index8, wprot:	-255 to 255

	 *		index16:				-65535 to 65535

	 *		z, idata, fdata:			anything

	 *		hue:					-360.0 to 360.0

	 *		saturation, intensity:			-1.0 to 1.0

	 *

	 *	Multiply:

	 *	    Multiplication factors can be anything except negative.

	 *	    Negative factors would produce negative results, which

	 *	    would just get clamped back to 0 anyway.

	 *		mono:					0 to 1

	 *		red, green, blue, alpha, index8, wprot:	0 to anything

	 *		index16:				0 to anything

	 *		z:					0 to anything

	 *		idata, fdata:				anything

	 *		hue:					0 to anything

	 *		saturation, intensity:			0 to anything

	 *

	 *	Divide:

	 *	    Division factors can be anything except zero or negative.

	 *	    Zero causes horrible things.  Negative values would produce

	 *	    negative results, which would just get clamped back to

	 *	    0 anyway.

	 *		mono:					>0 to 1

	 *		red, green, blue, alpha, index8, wprot:	>0 to anything

	 *		index16:				>0 to anything

	 *		z:					>0 to anything

	 *		idata, fdata:				!=0, anything

	 *		hue:					>0 to anything

	 *		saturation, intensity:			>0 to anything

	 *

	 *	Set:

	 *	    Set can be anything legal for the field.

	 *		mono:					0 to 1

	 *		red, green, blue, alpha, index8, wprot:	0 to 255

	 *		index16:				0 to 65535

	 *		z:					0 to anything

	 *		idata, fdata:				anything

	 *		hue:					0 to 360.0

	 *		saturation, intensity:			0 to 1.0

	 */



	/*

	 *  Get the low and high key and adjust values to make testing

	 *  extent faster.

	 */

	if ( keyStart < keyEnd )

	{

		keyLow  = keyStart;

		keyHigh = keyEnd;

	}

	else

	{

		keyHigh = keyStart;

		keyLow  = keyEnd;

	}

	if ( adjustStart < adjustEnd )

	{

		adjustLow  = adjustStart;

		adjustHigh = adjustEnd;

	}

	else

	{

		adjustHigh = adjustStart;

		adjustLow  = adjustEnd;

	}





	/*

	 *  Make sure the key field and key range are good.

	 */

	switch ( keyField )

	{

	case IMMONO:

		if ( keyLow < 0.0 || keyHigh > 1.0 )

			IM_ERROR( IMEKEY );

		break;



	case IMRED:

	case IMGREEN:

	case IMBLUE:

	case IMALPHA:

	case IMINDEX8:

	case IMWPROT:

		if ( keyLow < 0.0 || keyHigh > 255.0 )

			IM_ERROR( IMEKEY );

		break;



	case IMINDEX16:

		if ( keyLow < 0.0 || keyHigh > 65535.0 )

			IM_ERROR( IMEKEY );

		break;



	case IMHUE:

		if ( keyLow < 0.0 || keyHigh > 360.0 )

			IM_ERROR( IMEKEY );

		break;



	case IMSATURATION:

	case IMINTENSITY:

		if ( keyLow < 0.0 || keyHigh > 1.0 )

			IM_ERROR( IMEKEY );

		break;



	case IMZ:

		if ( keyLow < 0.0 )

			IM_ERROR( IMEKEY );

		break;



	case IMIDATA:

	case IMFDATA:

		/* All values allowed.					*/

		break;



	default:

		IM_ERROR( IMEKEYFIELD );

	}





	/*

	 *  Make sure the operation is good.

	 */

	switch ( operation )

	{

	case IMADD:

	case IMSUBTRACT:

	case IMMULTIPLY:

	case IMDIVIDE:

	case IMSET:

		break;

	default:

		IM_ERROR( IMEOPERATION );

	}





	/*

	 *  Make sure the adjust field and adjust range are good.

	 */

	switch ( adjustField )

	{

	case IMMONO:

		imCheckField( 1.0, IMEADJUST );

		break;



	case IMRED:

	case IMGREEN:

	case IMBLUE:

	case IMALPHA:

	case IMINDEX8:

	case IMWPROT:

		imCheckField( 255.0, IMEADJUST );

		break;



	case IMINDEX16:

		imCheckField( 65535.0, IMEADJUST );

		break;



	case IMZ:

		switch ( operation )

		{

		case IMADD:

		case IMSUBTRACT:

			break;

		case IMMULTIPLY:

		case IMSET:

			if ( adjustLow < 0.0 )

				IM_ERROR( IMEADJUST );

			break;

		case IMDIVIDE:

			if ( adjustLow == 0.0 || adjustHigh == 0.0 )

				IM_ERROR( IMEADJUST );

			break;

		}

		break;



	case IMIDATA:

	case IMFDATA:

		if ( operation == IMDIVIDE &&

			( adjustLow == 0.0 || adjustHigh == 0.0 ) )

			IM_ERROR( IMEADJUST );

		break;



	case IMHUE:

		imCheckField( 360.0, IMEADJUST );

		break;



	case IMSATURATION:

	case IMINTENSITY:

		imCheckField( 1.0, IMEADJUST );

		break;

	}





        /*

	 *  Check that the source VFB has the fields we need to do the

	 *  adjustment.

	 *

	 *  Key fields:

	 *	red, green, blue:	source may have any color field type

	 *	hue, saturation, inten:	source may have any color field type

	 *	mono:			source must have MONO

	 *	index8:			source must have INDEX8

	 *	index16:		source must have INDEX16

	 *	alpha:			source must have ALPHA

	 *	wprot:			source must have WPROT

	 *	z:			source must have Z

	 *	idata:			source must have IDATA

	 *	fdata:			source must have FDATA

	 *

	 *  Replace fields:

	 *	red, green, blue:	destination must have RGB.

	 *	hue, saturation, inten:	destination must have RGB.

	 *	index8:			destination must have INDEX8.

	 *	index16:		destination must have INDEX16.

	 *	alpha:			destination must have ALPHA.

	 *	wprot:			destination must have WPROT

	 *	z:			destination must have Z

	 *	idata:			destination must have IDATA

	 *	fdata:			destination must have FDATA

         */

	fields = ImVfbQFields( srcVfb );

	switch ( keyField )

	{

	case IMMONO:

		if ( !(fields & IMVFBMONO) )

			IM_ERROR( IMEKEYFIELD );

		break;



	case IMRED:

	case IMGREEN:

	case IMBLUE:

	case IMHUE:

	case IMSATURATION:

	case IMINTENSITY:

		if ( !(fields&(IMVFBRGB|IMVFBINDEX8|IMVFBINDEX16|IMVFBMONO)))

			IM_ERROR( IMEKEYFIELD );

		break;



	case IMINDEX8:

		if ( !(fields & IMVFBINDEX8) )

			IM_ERROR( IMEKEYFIELD );

		break;



	case IMINDEX16:

		if ( !(fields & IMVFBINDEX16) )

			IM_ERROR( IMEKEYFIELD );

		break;



	case IMALPHA:

		if ( !(fields & IMVFBALPHA) )

			IM_ERROR( IMEKEYFIELD );

		break;



	case IMWPROT:

		if ( !(fields & IMVFBWPROT) )

			IM_ERROR( IMEKEYFIELD );

		break;



	case IMZ:

		if ( !(fields & IMVFBZ) )

			IM_ERROR( IMEKEYFIELD );

		break;



	case IMIDATA:

		if ( !(fields & IMVFBIDATA) )

			IM_ERROR( IMEKEYFIELD );

		break;



	case IMFDATA:

		if ( !(fields & IMVFBFDATA) )

			IM_ERROR( IMEKEYFIELD );

		break;

	}

	switch ( adjustField )

	{

	case IMMONO:

		if ( !(fields & IMVFBMONO) )

			IM_ERROR( IMEADJUSTFIELD );

		break;



	case IMRED:

	case IMGREEN:

	case IMBLUE:

	case IMHUE:

	case IMSATURATION:

	case IMINTENSITY:

		if ( !(fields & IMVFBRGB) )

			IM_ERROR( IMEADJUSTFIELD );

		break;



	case IMINDEX8:

		if ( !(fields & IMVFBINDEX8) )

			IM_ERROR( IMEADJUSTFIELD );

		break;



	case IMINDEX16:

		if ( !(fields & IMVFBINDEX16) )

			IM_ERROR( IMEADJUSTFIELD );

		break;



	case IMALPHA:

		if ( !(fields & IMVFBALPHA) )

			IM_ERROR( IMEADJUSTFIELD );

		break;



	case IMWPROT:

		if ( !(fields & IMVFBWPROT) )

			IM_ERROR( IMEADJUSTFIELD );

		break;



	case IMZ:

		if ( !(fields & IMVFBZ) )

			IM_ERROR( IMEADJUSTFIELD );

		break;



	case IMIDATA:

		if ( !(fields & IMVFBIDATA) )

			IM_ERROR( IMEADJUSTFIELD );

		break;



	case IMFDATA:

		if ( !(fields & IMVFBFDATA) )

			IM_ERROR( IMEADJUSTFIELD );

		break;

	}







	/*

	 *  Copy the source into the destination, creating a new destination

	 *  if needed.  ImVfbCopy() will catch problems where the given

	 *  destination VFB (if not IMVFBNEW) doesn't have the same fields as

	 *  the source, or isn't at least as large as the source.

	 */

	x = ImVfbQWidth( srcVfb ); 

	y = ImVfbQHeight( srcVfb );

	if ( srcVfb == IMVFBNULL )

		IM_ERROR( IMENOVFB );

	if ( srcVfb != dstVfb )

	{

		dstVfb = ImVfbCopy( srcVfb, 0, 0, x, y, fields, dstVfb, 0,0);

		if ( dstVfb == IMVFBNULL )

			return ( IMVFBNULL );	/* ImErrNo already set	*/

	}





	/*

	 *  Prepare to adjust the image.

	 */

	if ( keyEnd != keyStart )

		factor = (adjustEnd - adjustStart) / (keyEnd - keyStart);

	else

		factor = 0;



	clt    = ImVfbQClt( dstVfb );	/* Might be IMCLTNULL		*/

	pDst   = ImVfbQFirst( dstVfb );

	fields = ImVfbQFields( dstVfb );





	/*

	 *  Simplify the field mask to the single key field we care about.

	 */

	switch ( keyField )

	{

	case IMRED:

	case IMGREEN:

	case IMBLUE:

	case IMHUE:

	case IMSATURATION:

	case IMINTENSITY:

		fields &= IMVFBIMAGEMASK;

		break;

	default:

		fields = keyField;

	}







	/*

	 *  Adjust the image.

	 *

	 *  Note that there are a lot of checks in the innermost pixel scan

	 *  loop that are loop-invariant.  It would certainly speed things

	 *  up if they were moved out of the loop.  Unfortunately, this

	 *  considerably increases the amount of the code since it requires

	 *  duplicating the loop for each combination of:

	 *

	 *	keyField

	 *		red		4 combinations of dst VFB fields

	 *		green		4 combinations of dst VFB fields

	 *		blue		4 combinations of dst VFB fields

	 *		hue		4 combinations of dst VFB fields

	 *		saturation	4 combinations of dst VFB fields

	 *		intensity	4 combinations of dst VFB fields

	 *		index8		2 combinations of dst CLT existance

	 *		index16		2 combinations of dst CLT existance

	 *		mono		1 combination

	 *		alpha		1 combination

	 *		wprot		1 combination

	 *		z		1 combination

	 *		idata		1 combination

	 *		fdata		1 combination

	 *				--

	 *		subtotal	34

	 *

	 *	operator		5 possible values

	 *	adjustField		10 possible values

	 *				---

	 *	TOTAL		 	34 * 5 * 10 = 1700 variants

	 *

	 *  Things clearly get out of hand quick.  Additionally, the basic

	 *  algorithm gets spread out all over everywhere, even with liberal

	 *  use of macros.

	 *

	 *  So, we'll pay the speed penalty in order to get less code and

	 *  easier maintenance.

	 */

	for ( i = 0; i < y; i++ )

	{

	    for ( j = 0; j < x; j++ )

	    {

		/* Get the pixel value depending upon keyfield.		*/

		switch ( keyField )

		{

		case IMRED:

		    /* Get pixel value from VFB depending upon fields.	*/

		    switch ( fields )

		    {

		    case IMVFBRGB:

			pixel = ImVfbQRed( dstVfb, pDst );

			break;

		    case IMVFBINDEX8:

			pixel = ImVfbQIndex8( dstVfb, pDst );

			if ( clt != IMCLTNULL )

				pixel = ImCltQRed( ImCltQPtr( clt, (int)pixel));

			break;

		    case IMVFBINDEX16:

			pixel = ImVfbQIndex16( dstVfb, pDst );

			if ( clt != IMCLTNULL )

				pixel = ImCltQRed( ImCltQPtr( clt, (int)pixel));

			else

				pixel /= 255;

			break;

		    case IMVFBMONO:

			pixel = ImVfbQMono( dstVfb, pDst ) * 255;

			break;

		    }

		    break;

		case IMGREEN:

		    /* Get pixel value from VFB depending upon fields.	*/

		    switch ( fields )

		    {

		    case IMVFBRGB:

			pixel = ImVfbQGreen( dstVfb, pDst );

			break;

		    case IMVFBINDEX8:

			pixel = ImVfbQIndex8( dstVfb, pDst );

			if ( clt != IMCLTNULL )

				pixel = ImCltQGreen( ImCltQPtr( clt, (int)pixel));

			break;

		    case IMVFBINDEX16:

			pixel = ImVfbQIndex16( dstVfb, pDst );

			if ( clt != IMCLTNULL )

				pixel = ImCltQGreen( ImCltQPtr( clt, (int)pixel));

			else

				pixel /= 255;

			break;

		    case IMVFBMONO:

			pixel = ImVfbQMono( dstVfb, pDst ) * 255;

			break;

		    }

		    break;

		case IMBLUE:

		    /* Get pixel value from VFB depending upon fields.	*/

		    switch ( fields )

		    {

		    case IMVFBRGB:

			pixel = ImVfbQBlue( dstVfb, pDst );

			break;

		    case IMVFBINDEX8:

			pixel = ImVfbQIndex8( dstVfb, pDst );

			if ( clt != IMCLTNULL )

				pixel = ImCltQBlue( ImCltQPtr( clt, (int)pixel));

			break;

		    case IMVFBINDEX16:

			pixel = ImVfbQIndex16( dstVfb, pDst );

			if ( clt != IMCLTNULL )

				pixel = ImCltQBlue( ImCltQPtr( clt, (int)pixel));

			else

				pixel /= 255;

			break;

		    case IMVFBMONO:

			pixel = ImVfbQMono( dstVfb, pDst ) * 255;

			break;

		    }

		    break;

		case IMINDEX8:

		    pixel = ImVfbQIndex8( dstVfb, pDst );

		    break;

		case IMINDEX16:

		    pixel = ImVfbQIndex16( dstVfb, pDst );

		    break;

		case IMMONO:

		    pixel = ImVfbQMono( dstVfb, pDst );

		    break;

		case IMALPHA:

		    pixel = ImVfbQAlpha( dstVfb, pDst );

		    break;

		case IMWPROT:

		    pixel = ImVfbQWProt( dstVfb, pDst );

		    break;

		case IMZ:

		    pixel = ImVfbQZ( dstVfb, pDst );

		    break;

		case IMIDATA:

		    pixel = ImVfbQIData( dstVfb, pDst );

		    break;

		case IMFDATA:

		    pixel = ImVfbQFData( dstVfb, pDst );

		    break;

		case IMHUE:

		case IMSATURATION:

		case IMINTENSITY:

		    /* Get pixel value from VFB depending upon fields.	*/

		    switch ( fields )

		    {

		    case IMVFBRGB:

			pixelRgb[0] = ImVfbQRed( dstVfb, pDst );

			pixelRgb[1] = ImVfbQGreen( dstVfb, pDst );

			pixelRgb[2] = ImVfbQBlue( dstVfb, pDst );

			break;

		    case IMVFBINDEX8:

			pixel = ImVfbQIndex8( dstVfb, pDst );

			if ( clt != IMCLTNULL )

			{

				pixelRgb[0] = ImCltQRed( ImCltQPtr( clt, (int)pixel));

				pixelRgb[1] = ImCltQGreen( ImCltQPtr( clt, (int)pixel));

				pixelRgb[2] = ImCltQBlue( ImCltQPtr( clt, (int)pixel));

			}

			else

			{

				pixelRgb[0] = (int)pixel;

				pixelRgb[1] = (int)pixel;

				pixelRgb[2] = (int)pixel;

			}

			break;

		    case IMVFBINDEX16:

			pixel = ImVfbQIndex16( dstVfb, pDst );

			if ( clt != IMCLTNULL )

			{

				pixelRgb[0] = ImCltQRed( ImCltQPtr( clt, (int)pixel));

				pixelRgb[1] = ImCltQGreen( ImCltQPtr( clt, (int)pixel));

				pixelRgb[2] = ImCltQBlue( ImCltQPtr( clt, (int)pixel));

			}

			else

			{

				pixelRgb[0] = (int)pixel / 255;

				pixelRgb[1] = (int)pixel / 255;

				pixelRgb[2] = (int)pixel / 255;

			}

			break;

		    case IMVFBMONO:

			pixel = ImVfbQMono( dstVfb, pDst ) * 255;

			pixelRgb[0] = (int)pixel;

			pixelRgb[1] = (int)pixel;

			pixelRgb[2] = (int)pixel;

			break;

		    }

		    ImRgbToHsi( pixelRgb, pixelHsi );

		    switch ( keyField )

		    {

		    case IMHUE:		pixel = pixelHsi[0]; break;

		    case IMSATURATION:	pixel = pixelHsi[1]; break;

		    case IMINTENSITY:	pixel = pixelHsi[2]; break;

		    }

		    break;

	        }



		/*  Check if the pixel is within the key range.		*/

		if ( !(pixel >= keyStart && pixel <= keyEnd) )

		{

			/* Nope!					*/

			ImVfbSInc( dstVfb, pDst );

			continue;

		}



		/* Compute the value to add, subtract, etc.		*/

		value = adjustStart + factor * (pixel - keyStart);



		/*  Update the pixel depending upon the operation.	*/

		switch ( operation )

		{

		case IMADD:

			imAdjustPixel( += );

			break;

		case IMSUBTRACT:

			imAdjustPixel( -= );

			break;

		case IMMULTIPLY:

			imAdjustPixel( *= );

			break;

		case IMDIVIDE:

			imAdjustPixel( /= );

			break;

		case IMSET:

			imAdjustPixel( = );

			break;

		}

		ImVfbSInc( dstVfb, pDst );

	    }

	}



	return( dstVfb);	

}

