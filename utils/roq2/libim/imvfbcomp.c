/**

 **	$Header: /roq/libim/imvfbcomp.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imvfbcomp.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imvfbcomp.c		-  image compositing

 **

 **  PROJECT

 **	libim		-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imvfbcomp.c functions to do digital compositing on images.

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **	ImVfbComp	f  composite fields of a virtual frame buffer

 **

 **  PRIVATE CONTENTS

 **	imVfbCompOver	f  composites one image over another

 **	imVfbCompInside	f  composites one image inside another

 **	imVfbCompOutsidef  composites one image outside another

 **	imVfbCompAtop	f  composites one image atop another

 **

 **  HISTORY

 ** 	$Log: /roq/libim/imvfbcomp.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.16  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.15  1995/06/16  08:52:27  bduggan

 **	removed use of 'operator'

 **

 **	Revision 1.14  1995/01/10  23:46:32  bduggan

 **	added a comment

 **

 **	Revision 1.13  1994/10/03  11:29:46  nadeau

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

 **	Revision 1.12  93/05/19  19:45:51  shane

 **	Fixed two more bugs.

 **	

 **	Revision 1.11  93/05/19  18:52:39  shane

 **	Fixed two bugs.

 **	

 **	Revision 1.10  93/04/09  14:41:21  shane

 **	Standardized style. Optimized.

 **	

 **	Revision 1.9  93/03/30  11:59:01  shane

 **	Added ability to offset foreground into background... calling interface

 **	is all different.

 **	

 **	Revision 1.8  93/03/25  16:39:57  shane

 **	Re added line I accidentally deleted: "public ImVfb *" line right above

 **	ImVfbComp declaration

 **	

 **	

 **	Revision 1.7  93/03/25  16:29:09  shane

 **	Fixed same bug I just fixed in IM_OVER but in IM_ATOP, IM_INSIDE, and IM_OUTSIDE

 **	

 **	Revision 1.6  93/03/25  12:22:50  shane

 **	Fixed 

 **	vertical stripe error, due to improper vfb pointer incrementation

 **	

 **	Revision 1.5  92/12/03  01:54:33  nadeau

 **	Changed names of ImRgbHsi and ImHsiRgb to new names.

 **	

 **	Revision 1.4  92/11/18  17:20:03  nadeau

 **	Updated and rearranged code for greater efficiencey.

 **	Added more error checks.

 **	

 **	Revision 1.3  92/09/04  14:06:49  vle

 **	Fixed minor initialization bug.

 **	

 **	Revision 1.2  92/08/31  17:37:33  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.1  92/08/26  12:45:31  groening

 **	Initial revision

 **	

 **/



/**

 **  CODE CREDITS

 **     Custom development,  Chris Groening, San Diego Supercomputer Center, 1990.

 **/

#include "iminternal.h"

#ifdef __STDC__

static	void imVfbCompOver( ImVfb *fgVfb, int fgX, int fgY, int fgXSize, int fgYSize, int fieldMask,

	int bgXInto, int bgYInto,ImVfb *bgVfb, int bgXSize, int bgYSize );

static  void imVfbCompInside(ImVfb *fgVfb, int fgX, int fgY, int fgXSize, int fgYSize, int fieldMask,

        int bgXInto, int bgYInto,ImVfb *bgVfb, int bgXSize, int bgYSize);

static  void imVfbCompOutside(ImVfb *fgVfb, int fgX, int fgY, int fgXSize, int fgYSize, int fieldMask,

        int bgXInto, int bgYInto,ImVfb *bgVfb, int bgXSize, int bgYSize);

static  void imVfbCompAtop(ImVfb *fgVfb, int fgX, int fgY, int fgXSize, int fgYSize, int fieldMask,

        int bgXInto, int bgYInto,ImVfb *bgVfb, int bgXSize, int bgYSize);

#else

extern ImVfb *ImVfbComp();



static	void imVfbCompOver();

static  void imVfbCompInside();

static  void imVfbCompOutside();

static  void imVfbCompAtop();

#endif











/*

 * FUNCTION

 *	ImVfbComp - composites the  fields in a virtual frame buffer.

 *		The result is stored in the background VFB, whose address is

 *		also returned.

 *

 * DESCRIPTION

 *	The two source images are composited together for each field in

 *	fieldMask based upon the chosen operator.

 */



ImVfb *                  /* Returns destination vfb              */

#ifdef __STDC__

ImVfbComp( ImVfb *fgVfb, int fgX, int fgY, int fgXSize, int fgYSize, int fieldMask, int zOperator, ImVfb *bgVfb, 

	int bgXInto, int bgYInto )

#else

ImVfbComp( fgVfb, fgX, fgY, fgXSize, fgYSize, fieldMask, zOperator, bgVfb, 

	bgXInto, bgYInto )

        ImVfb   *fgVfb;         /* incoming forground vfb 		*/

        int     fgX;            /* right hand edge of fgVfb 		*/

        int     fgY;            /* top edge of fgVfb 			*/

        int     fgXSize;        /* width of fgVfb 			*/

        int     fgYSize;        /* height of fgVfb 			*/

        int     fieldMask;      /* what field to replace 		*/

        int     zOperator;      /* what operator to to perform on field */

        ImVfb   *bgVfb;         /* incoming bg vfb which is final dest vfb */

        int     bgXInto;        /* x coordinate of fgVfb inside of bgVfb*/

        int     bgYInto;        /* y coordinate of fgVfb inside of bgVfb*/

#endif

{



	int     fgField;	/* foreground fields			*/

	int	bgField;	/* background fields			*/

	int	neededFields;	/* Fields needed in VFB's		*/





	/*

	 *  Make sure the foreground  and background images are valid.

	 */

	if ( fgVfb == IMVFBNULL )

	{

		ImErrNo = IMENOVFB;

		return ( IMVFBNULL );

	}

	if ( bgVfb == IMVFBNULL )

	{

		ImErrNo = IMENOVFB;

		return ( IMVFBNULL );

	}

	

	/*

	 * Clamp values that don't make sense. It's ok to have negative 

	 * bgXInto and bgYInto, however.

	 */



	if ( fgXSize < 0 ) 

		fgXSize = 0;

	else if ( fgXSize > ImVfbQWidth(  fgVfb ) ) 

		fgXSize = ImVfbQWidth(  fgVfb );



	if ( fgYSize < 0 ) 

		fgYSize = 0;

	else if ( fgYSize > ImVfbQHeight( fgVfb ) ) 

		fgYSize = ImVfbQHeight( fgVfb );



	if ( fgX < 0 ) 

		fgX = 0;

	else if ( fgX > ImVfbQWidth( fgVfb ) ) 

		fgX = ImVfbQWidth( fgVfb );



	if ( fgY < 0 ) 

		fgY = 0;

	else if ( fgY > ImVfbQHeight( fgVfb ) ) 

		fgY = ImVfbQHeight( fgVfb );



	if ( bgXInto > ImVfbQWidth(  bgVfb ) ) 

		bgXInto = ImVfbQWidth(  bgVfb );



	if ( bgYInto > ImVfbQHeight( bgVfb ) ) 

		bgYInto = ImVfbQHeight( bgVfb );





	/*

	 * When the user decides to use to xstart and/or ystart options,

 	 * we must adjust the width so that we don't scan past the actual

	 * width of the foreground. 

	 */



	fgXSize = ImVfbQWidth( fgVfb ) - fgX;

	fgYSize = ImVfbQHeight( fgVfb ) - fgY;





	/*

	 *  Make sure the sources each have the fields listed in the

	 *  field mask.

	 */

	if ( fieldMask == 0 )

	{

		ImErrNo = IMEFIELD;

		return ( IMVFBNULL );

	}

	if ( fieldMask & IMVFBRGB )

		fieldMask = (fieldMask & ~IMVFBRGB) | IMRED | IMGREEN | IMBLUE;

	if ( fieldMask & (IMHUE | IMSATURATION | IMINTENSITY) )

		neededFields = IMVFBRGB | (fieldMask & ~(IMHUE | IMSATURATION | IMINTENSITY));

	else if ( fieldMask & (IMRED | IMGREEN | IMBLUE) )

		neededFields = IMVFBRGB | (fieldMask & ~(IMRED | IMGREEN | IMBLUE));

	else

		neededFields = fieldMask;

	fgField = ImVfbQFields( fgVfb );

	bgField = ImVfbQFields( bgVfb );

	if ( (fgField & neededFields) != neededFields )

	{

		ImErrNo = IMEFIELD;

		return ( IMVFBNULL );

	}

	if ( (bgField & neededFields) != neededFields )

	{

		ImErrNo = IMEFIELD;

		return ( IMVFBNULL );

	}





	/*

	 *  Make sure the zOperator is valid and that the right set of

	 *  alpha channels are available.

	 */

	switch ( zOperator )

	{

	case IMCOMPOVER:

		if ( !(fgField & IMALPHA) )

		{

			ImErrNo = IMENOALPHA;

			return ( IMVFBNULL );

		}

		break;



	case IMCOMPATOP:

		if ( !(fgField & IMALPHA) )

		{

			ImErrNo = IMENOALPHA;

			return ( IMVFBNULL );

		}

		if ( !(bgField & IMALPHA) )

		{

			ImErrNo = IMENOALPHA;

			return ( IMVFBNULL );

		}

		break;



	case IMCOMPINSIDE:

	case IMCOMPOUTSIDE:

		if ( !(bgField & IMALPHA) )

		{

			ImErrNo = IMENOALPHA;

			return ( IMVFBNULL );

		}

		break;



	default:

		ImErrNo = IMEOPERATION;

		break;

	}





	/*

	 * If the needed fields don't match up, then is is an error.

	 */

	if ( (ImVfbQFields( bgVfb ) & neededFields) != neededFields )

	{

		ImErrNo = IMEFIELD;

		return ( IMVFBNULL );

	}





	/*

	 *  Composite them!

	 */

	switch ( zOperator )

	{

	case IMCOMPOVER:

		imVfbCompOver( fgVfb, fgX, fgY, fgXSize, fgYSize, fieldMask, 

			bgXInto, bgYInto, bgVfb, ImVfbQWidth( bgVfb), 

			ImVfbQHeight( bgVfb ));

		break;



	case IMCOMPINSIDE:

		imVfbCompInside( fgVfb, fgX, fgY, fgXSize, fgYSize, fieldMask, 

			bgXInto, bgYInto, bgVfb, ImVfbQWidth( bgVfb), 

			ImVfbQHeight( bgVfb ));

		break;



	case IMCOMPOUTSIDE:

                imVfbCompOutside( fgVfb, fgX, fgY, fgXSize, fgYSize, fieldMask,

			bgXInto, bgYInto, bgVfb, ImVfbQWidth( bgVfb), 

			ImVfbQHeight( bgVfb ));

		break;



	case IMCOMPATOP:

                imVfbCompAtop( fgVfb, fgX, fgY, fgXSize, fgYSize, fieldMask, 

			bgXInto, bgYInto, bgVfb, ImVfbQWidth( bgVfb), 

			ImVfbQHeight( bgVfb ));

		break;

	}





	return( bgVfb);	

			

}











/*

 *  FUNCTION

 *	imVfbCompOver	-  composite using the IM_OVER operator

 *	imVfbCompAtop	-  composite using the IM_ATOP operator

 *	imVfbCompInside	-  composite using the IM_INSIDE operator

 *	imVfbCompOutside-  composite using the IM_OUTSIDE operator

 *

 *  DESCRIPTION

 *	The two source images are composited into the destination image based

 *	upon the operator:

 *	

 *	over:

 *		OutChannel = In1Channel * In1Alpha +

 *			     In2Channel * (1.0 - In1Alpha)

 *

 *	atop:

 *		OutChannel = In1Channel * In2Alpha +

 *			     In2Channel * (1.0 - In1Alpha)

 *

 *	inside:

 *		OutChannel = In1Channel * In2Alpha

 *

 *	outside:

 *		OutChannel = In1Channel * (1.0 - In2Alpha)

 *

 *	Each of the following four routines are essentially identical

 *	except for the application of the operator equations above.

 */



#define IM_OVER( query, set )						\
{									\
	set( bgVfb, pBg,						\
		(float)(query( fgVfb, pFg )) * alpha1 +			\
		(float)(query( bgVfb, pBg )) * (1.0 - alpha1) );	\
}


#define IM_ATOP( query, set )						\
{									\
	set( bgVfb, pBg,						\
		(float)(query( fgVfb, pFg )) * alpha2 +			\
		(float)(query( bgVfb, pBg )) * (1.0 - alpha1) );	\
}


#define IM_INSIDE( query, set )						\
{									\
	set( bgVfb, pBg,						\
		(float)(query( fgVfb, pFg )) * alpha2 );		\
}


#define IM_OUTSIDE( query, set )						\
{									\
	set( bgVfb, pBg,						\
		(float)(query( fgVfb, pFg )) * (1.0 - alpha2) );	\
}


#define IM_GETHSI( hsi, rgb, srcVfb, pSrc )				\
	rgb[0] = ImVfbQRed(   srcVfb, pSrc );				\
	rgb[1] = ImVfbQGreen( srcVfb, pSrc );				\
	rgb[2] = ImVfbQBlue(  srcVfb, pSrc );				\
	ImRgbToHsi( rgb, hsi );


#define IM_PUTRGB( hsi, rgb, srcVfb, pSrc )				\
	ImHsiToRgb( hsi, rgb );						\
	ImVfbSRed(   srcVfb, pSrc, rgb[0] );				\
	ImVfbSGreen( srcVfb, pSrc, rgb[1] );				\
	ImVfbSBlue(  srcVfb, pSrc, rgb[2] );


static void			/* Returns nothin			*/

#ifdef __STDC__

imVfbCompOver( ImVfb *fgVfb, int fgX, int fgY, int fgXSize, int fgYSize, int fieldMask,

	int bgXInto, int bgYInto,ImVfb *bgVfb, int bgXSize, int bgYSize )

#else

imVfbCompOver( fgVfb, fgX, fgY, fgXSize, fgYSize, fieldMask, bgXInto, bgYInto, 

	bgVfb, bgXSize, bgYSize )

	ImVfb	*fgVfb;		/* incoming vfb	foreground		*/

	int	fgX;		/* X offset into foreground 		*/

	int 	fgY;		/* Y offset into background 		*/

	int 	fgXSize;	/* width of foreground to copy 		*/

	int 	fgYSize;	/* height of foregroiund to copy	*/

	int	fieldMask;	/* what field to replace		*/

	int	bgXInto;	/* X offset of foreground into background */

	int	bgYInto;	/* Y offset of foreground into background */

	ImVfb	*bgVfb;		/* incoming vfb	background	 	*/

	int	bgXSize;	/* width of background			*/

	int	bgYSize;	/* height of background			*/

#endif

{

	int	   i,j;		/* counters				*/

	ImVfbPtr   pFg;		/* VFB pointer into fgVfb		*/

	ImVfbPtr   pBg;		/* VFB pointer into bgVfb		*/

	float      hsi1[3], hsi2[3];/* HSI values			*/

	int        rgb1[3], rgb2[3];/* RGB values			*/

	float	   alpha1;	/* image 1 alpha			*/

	int	   minXSize;	/* minimum width between fg and bg	*/

	int	   minYSize;	/* minimum height between fg and bg	*/





	/*

	 * Handle the case when the user wishes to offset the foreground by a 

	 * negative amount. Set their offset to zero and modify offset into 

	 * foreground accordingly.

	 */ 

	if (bgXInto < 0) 

	{

		fgX -= bgXInto;

		fgXSize += bgXInto;

		bgXInto = 0;

	}

	if (bgYInto < 0) 

	{

		fgY -= bgYInto;

		fgYSize += bgYInto;

		bgYInto = 0;

	}





	/*

         */

	if ( fgX > ImVfbQWidth( fgVfb ) ) return;

	if ( fgY > ImVfbQHeight( fgVfb ) ) return;





	/*

	 * Find the minimum width and height between the fg and bg VFBs.

	 */



	if ( bgYSize < fgYSize ) minYSize = bgYSize;

	else minYSize = fgYSize;



	if ( bgXSize < fgXSize ) minXSize = bgXSize;

	else minXSize = fgXSize;



	

	/*

	 * Do the actual composite point by point. Whenever the forground 

	 * overlaps of the left and lower end of the background, it is cut off.

	 */



        for ( i = 0; i < minYSize; i++ )

        if ( (bgYInto + i) < bgYSize )

	{

	    pFg = ImVfbQPtr( fgVfb, fgX, i+fgY );

	    pBg = ImVfbQPtr( bgVfb, bgXInto, i+bgYInto );



            for ( j = 0; j < minXSize; j++ )

            if ( (bgXInto + j) < bgXSize )

	    {

		alpha1 = ImVfbQAlpha( fgVfb, pFg )/255.0;

		if ( fieldMask & IMRED )	IM_OVER(ImVfbQRed,   ImVfbSRed)

		if ( fieldMask & IMGREEN )	IM_OVER(ImVfbQGreen, ImVfbSGreen)

		if ( fieldMask & IMBLUE )	IM_OVER(ImVfbQBlue,  ImVfbSBlue)

		if ( fieldMask & IMMONO )	IM_OVER(ImVfbQMono,  ImVfbSMono)

		if ( fieldMask & IMINDEX8 )	IM_OVER(ImVfbQIndex8,ImVfbSIndex8)

		if ( fieldMask & IMINDEX16)    IM_OVER(ImVfbQIndex16,ImVfbSIndex16)

		if ( fieldMask & IMWPROT )	IM_OVER(ImVfbQWProt, ImVfbSWProt)

		if ( fieldMask & IMIDATA )	IM_OVER(ImVfbQIData, ImVfbSIData)

		if ( fieldMask & IMFDATA )	IM_OVER(ImVfbQFData, ImVfbSFData)

		if ( fieldMask & IMZ )		IM_OVER(ImVfbQZ,     ImVfbSZ)

		if ( fieldMask & (IMHUE | IMSATURATION | IMINTENSITY) )

		{

			IM_GETHSI( hsi1, rgb1, fgVfb, pFg );

			IM_GETHSI( hsi2, rgb2, bgVfb, pBg );



			if ( fieldMask & IMHUE )

				hsi2[0] = hsi1[0] * alpha1 + hsi2[0] * (1.0 - alpha1);

			if ( fieldMask & IMSATURATION )

				hsi2[1] = hsi1[1] * alpha1 + hsi2[1] * (1.0 - alpha1);

			if ( fieldMask & IMINTENSITY )

				hsi2[2] = hsi1[2] * alpha1 + hsi2[2] * (1.0 - alpha1);



			IM_PUTRGB( hsi2, rgb2, bgVfb, pBg );

		}

		if ( fieldMask & IMALPHA )	IM_OVER( ImVfbQAlpha, ImVfbSAlpha )

		ImVfbSInc( fgVfb, pFg );

		ImVfbSInc( bgVfb, pBg );

	    }

	}

}





static void                    /* Returns nothin                       */

#ifdef __STDC__

imVfbCompAtop( ImVfb *fgVfb, int fgX, int fgY, int fgXSize, int fgYSize, int fieldMask,

	int bgXInto, int bgYInto,ImVfb *bgVfb, int bgXSize, int bgYSize )

#else

imVfbCompAtop( fgVfb, fgX, fgY, fgXSize, fgYSize, fieldMask, bgXInto, bgYInto, 

	bgVfb, bgXSize, bgYSize )

        ImVfb   *fgVfb;         /* incoming vfb foreground              */

        int     fgX;            /* X offset into foreground             */

        int     fgY;            /* Y offset into background             */

        int     fgXSize;        /* width of foreground to copy          */

        int     fgYSize;        /* height of foregroiund to copy        */

        int     fieldMask;      /* what field to replace                */

        int     bgXInto;        /* X offset of foreground into background */

        int     bgYInto;        /* Y offset of foreground into background */

        ImVfb   *bgVfb;         /* incoming vfb background              */

        int     bgXSize;        /* width of background                  */

        int     bgYSize;        /* height of background                 */

#endif

{

	int	   i,j;		/* counters				*/

	ImVfbPtr   pFg;		/* VFB pointer into fgVfb		*/

	ImVfbPtr   pBg;		/* VFB pointer into bgVfb		*/

	float      hsi1[3], hsi2[3];/* HSI values			*/

	int        rgb1[3], rgb2[3];/* RGB values			*/

	float	   alpha1;	/* image 1 alpha			*/

	float	   alpha2;	/* image 2 alpha			*/

	int	   minXSize;	/* minimum width between fg and bg	*/

	int	   minYSize;	/* minimum height between fg and bg	*/

	



        /*

         * Handle the case when the user wishes to offset the foreground by a

         * negative amount. Set their offset to zero and modify offset into 

	 * foreground accordingly.

         */

        if (bgXInto < 0)

	{

                fgX -= bgXInto;

                fgXSize += bgXInto;

		bgXInto = 0;

	}

        if (bgYInto < 0)

	{

		fgY -= bgYInto;

		fgYSize += bgYInto;

		bgYInto = 0;

	}





	/*

	 * Find the minimum width and height between the fg and bg VFBs.

	 */



	if ( bgYSize < fgYSize ) minYSize = bgYSize;

	else minYSize = fgYSize;



	if ( bgXSize < fgXSize ) minXSize = bgXSize;

	else minXSize = fgXSize;





        /*

         * Do the actual composite point by point. Whenever the forground 

	 * overlaps of the left and lower end of the background, it is cut off.

         */



        for ( i = 0; i < minYSize; i++ )

        if ( (bgYInto + i) < bgYSize )

        {

            pFg = ImVfbQPtr( fgVfb, fgX, i+fgY );

            pBg = ImVfbQPtr( bgVfb, bgXInto, i+bgYInto );



            for ( j = 0; j < minXSize; j++ )

            if ( (bgXInto + j) < bgXSize )

            {

		alpha1 = ImVfbQAlpha( fgVfb, pFg )/255.0;

		alpha2 = ImVfbQAlpha( bgVfb, pBg )/255.0;

		if ( fieldMask & IMRED )	IM_ATOP(ImVfbQRed,   ImVfbSRed)

		if ( fieldMask & IMGREEN )	IM_ATOP(ImVfbQGreen, ImVfbSGreen)

		if ( fieldMask & IMBLUE )	IM_ATOP(ImVfbQBlue,  ImVfbSBlue)

		if ( fieldMask & IMMONO )	IM_ATOP(ImVfbQMono,  ImVfbSMono)

		if ( fieldMask & IMINDEX8 )	IM_ATOP(ImVfbQIndex8,ImVfbSIndex8)

		if ( fieldMask & IMINDEX16)    IM_ATOP(ImVfbQIndex16,ImVfbSIndex16)

		if ( fieldMask & IMWPROT )	IM_ATOP(ImVfbQWProt, ImVfbSWProt)

		if ( fieldMask & IMIDATA )	IM_ATOP(ImVfbQIData, ImVfbSIData)

		if ( fieldMask & IMFDATA )	IM_ATOP(ImVfbQFData, ImVfbSFData)

		if ( fieldMask & IMZ )		IM_ATOP(ImVfbQZ,     ImVfbSZ)

		if ( fieldMask & (IMHUE | IMSATURATION | IMINTENSITY) )

		{

			IM_GETHSI( hsi1, rgb1, fgVfb, pFg );

			IM_GETHSI( hsi2, rgb2, bgVfb, pBg );



			if ( fieldMask & IMHUE )

				hsi2[0] = hsi1[0] * alpha2 + hsi2[0] * (1.0 - alpha1);

			if ( fieldMask & IMSATURATION )

				hsi2[1] = hsi1[1] * alpha2 + hsi2[1] * (1.0 - alpha1);

			if ( fieldMask & IMINTENSITY )

				hsi2[2] = hsi1[2] * alpha2 + hsi2[2] * (1.0 - alpha1);



			IM_PUTRGB( hsi2, rgb2, bgVfb, pBg );

		}

		if ( fieldMask & IMALPHA )	IM_ATOP( ImVfbQAlpha, ImVfbSAlpha )

		ImVfbSInc( fgVfb, pFg );

		ImVfbSInc( bgVfb, pBg );

	    }

	}

}





static void                    /* Returns nothin                       */

#ifdef __STDC__

imVfbCompInside( ImVfb *fgVfb, int fgX, int fgY, int fgXSize, int fgYSize, int fieldMask,

	int bgXInto, int bgYInto,ImVfb *bgVfb, int bgXSize, int bgYSize )

#else

imVfbCompInside( fgVfb, fgX, fgY, fgXSize, fgYSize, fieldMask, bgXInto, bgYInto,

	bgVfb, bgXSize, bgYSize )

        ImVfb   *fgVfb;         /* incoming vfb foreground              */

        int     fgX;            /* X offset into foreground             */

        int     fgY;            /* Y offset into background             */

        int     fgXSize;        /* width of foreground to copy          */

        int     fgYSize;        /* height of foregroiund to copy        */

        int     fieldMask;      /* what field to replace                */

        int     bgXInto;        /* X offset of foreground into background */

        int     bgYInto;        /* Y offset of foreground into background */

        ImVfb   *bgVfb;         /* incoming vfb background              */

        int     bgXSize;        /* width of background                  */

        int     bgYSize;        /* height of background                 */

#endif

{

	int	   i,j;		/* counters				*/

	ImVfbPtr   pFg;		/* VFB pointer into fgVfb		*/

	ImVfbPtr   pBg;		/* VFB pointer into bgVfb		*/

	float      hsi1[3], hsi2[3];/* HSI values			*/

	int        rgb1[3], rgb2[3];/* RGB values			*/

	float	   alpha2;	/* image 2 alpha			*/

	int	   minXSize;	/* minimum width between fg and bg	*/

	int	   minYSize;	/* minimum height between fg and bg	*/

	



        /*

         * Handle the case when the user wishes to offset the foreground by a

         * negative amount. Set their offset to zero and modify offset into 

	 * foreground accordingly.

         */

        if (bgXInto < 0)

        {

                fgX -= bgXInto;

                fgXSize += bgXInto;

                bgXInto = 0;

        }

        if (bgYInto < 0)

        {

                fgY -= bgYInto;

                fgYSize += bgYInto;

                bgYInto = 0;

        }





	/*

	 * Find the minimum width and height between the fg and bg VFBs.

	 */



	if ( bgYSize < fgYSize ) minYSize = bgYSize;

	else minYSize = fgYSize;



	if ( bgXSize < fgXSize ) minXSize = bgXSize;

	else minXSize = fgXSize;





        /*

         * Do the actual composite point by point. Whenever the forground 

	 * overlaps of the left and lower end of the background, it is cut off.

         */



        for ( i = 0; i < minYSize; i++ )

        if ( (bgYInto + i) < bgYSize )

	{

            pFg = ImVfbQPtr( fgVfb, fgX, i+fgY );

            pBg = ImVfbQPtr( bgVfb, bgXInto, i+bgYInto );



            for ( j = 0; j < minXSize; j++ )

            if ( (bgXInto + j) < bgXSize )

            {

		alpha2 = ImVfbQAlpha( bgVfb, pBg )/255.0;

		if ( fieldMask & IMRED )	IM_INSIDE(ImVfbQRed,   ImVfbSRed)

		if ( fieldMask & IMGREEN )	IM_INSIDE(ImVfbQGreen, ImVfbSGreen)

		if ( fieldMask & IMBLUE )	IM_INSIDE(ImVfbQBlue,  ImVfbSBlue)

		if ( fieldMask & IMMONO )	IM_INSIDE(ImVfbQMono,  ImVfbSMono)

		if ( fieldMask & IMINDEX8 )	IM_INSIDE(ImVfbQIndex8,ImVfbSIndex8)

		if ( fieldMask & IMINDEX16)    IM_INSIDE(ImVfbQIndex16,ImVfbSIndex16)

		if ( fieldMask & IMWPROT )	IM_INSIDE(ImVfbQWProt, ImVfbSWProt)

		if ( fieldMask & IMIDATA )	IM_INSIDE(ImVfbQIData, ImVfbSIData)

		if ( fieldMask & IMFDATA )	IM_INSIDE(ImVfbQFData, ImVfbSFData)

		if ( fieldMask & IMZ )		IM_INSIDE(ImVfbQZ,     ImVfbSZ)

		if ( fieldMask & (IMHUE | IMSATURATION | IMINTENSITY) )

		{

			IM_GETHSI( hsi1, rgb1, fgVfb, pFg );

			IM_GETHSI( hsi2, rgb2, bgVfb, pBg );



			if ( fieldMask & IMHUE )

				hsi2[0] = hsi1[0] * alpha2;

			if ( fieldMask & IMSATURATION )

				hsi2[1] = hsi1[1] * alpha2;

			if ( fieldMask & IMINTENSITY )

				hsi2[2] = hsi1[2] * alpha2;



			IM_PUTRGB( hsi2, rgb2, bgVfb, pBg );

		}

		if ( fieldMask & IMALPHA )	IM_INSIDE(ImVfbQAlpha, ImVfbSAlpha)

		ImVfbSInc( fgVfb, pFg );

		ImVfbSInc( bgVfb, pBg );

	    }

	}

}





static void                    /* Returns nothin                       */

#ifdef __STDC__

imVfbCompOutside( ImVfb *fgVfb, int fgX, int fgY, int fgXSize, int fgYSize, int fieldMask,

	int bgXInto, int bgYInto,ImVfb *bgVfb, int bgXSize, int bgYSize )

#else

imVfbCompOutside( fgVfb, fgX, fgY, fgXSize, fgYSize, fieldMask, bgXInto, 

	bgYInto, bgVfb, bgXSize, bgYSize )

        ImVfb   *fgVfb;         /* incoming vfb foreground              */

        int     fgX;            /* X offset into foreground             */

        int     fgY;            /* Y offset into background             */

        int     fgXSize;        /* width of foreground to copy          */

        int     fgYSize;        /* height of foregroiund to copy        */

        int     fieldMask;      /* what field to replace                */

        int     bgXInto;        /* X offset of foreground into background */

        int     bgYInto;        /* Y offset of foreground into background */

        ImVfb   *bgVfb;         /* incoming vfb background              */

        int     bgXSize;        /* width of background                  */

        int     bgYSize;        /* height of background                 */

#endif

{

	int	   i,j;		/* counters				*/

	ImVfbPtr   pFg;		/* VFB pointer into fgVfb		*/

	ImVfbPtr   pBg;		/* VFB pointer into bgVfb		*/

	float      hsi1[3], hsi2[3];/* HSI values			*/

	int        rgb1[3], rgb2[3];/* RGB values			*/

	float	   alpha2;	/* image 2 alpha			*/

	int	   minXSize;	/* minimum width between fg and bg	*/

	int	   minYSize;	/* minimum height between fg and bg	*/

	



        /*

         * Handle the case when the user wishes to offset the foreground by a 

	 * negative amount. Set their offset to zero and modify offset into 

	 * foreground accordingly.

         */

        if (bgXInto < 0)

        {

                fgX -= bgXInto;

                fgXSize += bgXInto;

                bgXInto = 0;

        }

        if (bgYInto < 0)

        {

                fgY -= bgYInto;

                fgYSize += bgYInto;

                bgYInto = 0;

        }





	/*

	 * Find the minimum width and height between the fg and bg VFBs.

	 */



	if ( bgYSize < fgYSize ) minYSize = bgYSize;

	else minYSize = fgYSize;



	if ( bgXSize < fgXSize ) minXSize = bgXSize;

	else minXSize = fgXSize;





        /*

         * Do the actual composite point by point. Whenever the forground 

	 * overlaps of the left and lower end of the background, it is cut off.

         */



        for ( i = 0; i < minYSize; i++ )

        if ( (bgYInto + i) < bgYSize )

        {

            pFg = ImVfbQPtr( fgVfb, fgX, i+fgY );

            pBg = ImVfbQPtr( bgVfb, bgXInto, i+bgYInto );



            for ( j = 0; j < minXSize; j++ )

            if ( (bgXInto + j) < bgXSize )

            {

		alpha2 = ImVfbQAlpha( bgVfb, pBg )/255.0;

		if ( fieldMask & IMRED )	IM_OUTSIDE(ImVfbQRed,   ImVfbSRed)

		if ( fieldMask & IMGREEN )	IM_OUTSIDE(ImVfbQGreen, ImVfbSGreen)

		if ( fieldMask & IMBLUE )	IM_OUTSIDE(ImVfbQBlue,  ImVfbSBlue)

		if ( fieldMask & IMMONO )	IM_OUTSIDE(ImVfbQMono,  ImVfbSMono)

		if ( fieldMask & IMINDEX8 )	IM_OUTSIDE(ImVfbQIndex8,ImVfbSIndex8)

		if ( fieldMask & IMINDEX16)     IM_OUTSIDE(ImVfbQIndex16,ImVfbSIndex16)

		if ( fieldMask & IMWPROT )	IM_OUTSIDE(ImVfbQWProt, ImVfbSWProt)

		if ( fieldMask & IMIDATA )	IM_OUTSIDE(ImVfbQIData, ImVfbSIData)

		if ( fieldMask & IMFDATA )	IM_OUTSIDE(ImVfbQFData, ImVfbSFData)

		if ( fieldMask & IMZ )		IM_OUTSIDE(ImVfbQZ,     ImVfbSZ)

		if ( fieldMask & (IMHUE | IMSATURATION | IMINTENSITY) )

		{

			IM_GETHSI( hsi1, rgb1, fgVfb, pFg );

			IM_GETHSI( hsi2, rgb2, bgVfb, pBg );



			if ( fieldMask & IMHUE )

				hsi2[0] = hsi1[0] * (1.0 - alpha2);

			if ( fieldMask & IMSATURATION )

				hsi2[1] = hsi1[1] * (1.0 - alpha2);

			if ( fieldMask & IMINTENSITY )

				hsi2[2] = hsi1[2] * (1.0 - alpha2);



			IM_PUTRGB( hsi2, rgb2, bgVfb, pBg );

		}

		if ( fieldMask & IMALPHA )	IM_OUTSIDE(ImVfbQAlpha,ImVfbSAlpha)

		ImVfbSInc( fgVfb, pFg );

		ImVfbSInc( bgVfb, pBg );

	    }

	}

}

