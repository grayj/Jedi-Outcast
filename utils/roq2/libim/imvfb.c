/**
 **	$Header: /roq/libim/imvfb.c 1     11/02/99 4:38p Zaphod $
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

#define HEADER	"    $Header: /roq/libim/imvfb.c 1     11/02/99 4:38p Zaphod $"

/**
 **  FILE
 **	imvfb.c		-  general VFB functions
 **
 **  PROJECT
 **	libim	-  SDSC image manipulation library
 **
 **  DESCRIPTION
 **	imvfb.c contains the VFB and CLT allocation and deallocation functions
 **	for the IM package.
 **
 **  PUBLIC CONTENTS
 **			d =defined constant
 **			f =function
 **			m =defined macro
 **			t =typedef/struct/union
 **			v =variable
 **			? =other
 **
 **	ImVfbAlloc	f  alocate a virtual frame buffer
 **	ImVfbFree	f  deallocate a VFB
 **
 **	ImVfbDup	f  duplicate a VFB
 **	ImVfbCopy	f  copy a subarea within a virtual frame buffer.
 **	ImVfbClear	f  clear a virtual frame buffer.
 **	ImVfbMix	f  mix together two virtual frame buffers
 **	ImVfbFill	f  fill fields of a virtual frame buffer
 **     ImVfbRoll	f  rolls (wraps) a vfb.
 **     ImVfbPrint	f  print the pixels of a vfb
 **
 **  PRIVATE CONTENTS
 **	IM_SIZEOF*	d  sizes of various things
 **	IM_ALLIGN*	m  alignment adjustments
 **	IM_SETPIXEL	m  set a pixel to a value
 **
 **	imVfbFillRect	f  fill a rectangular region of a VFB
 **
 **  HISTORY
 **	$Log: /roq/libim/imvfb.c $
 * 
 * 1     11/02/99 4:38p Zaphod
 **	Revision 1.20  1995/06/30  22:11:11  bduggan
 **	fixed non-ansi declaration
 **
 **	Revision 1.19  1995/06/29  00:28:04  bduggan
 **	updated copyright year
 **
 **	Revision 1.18  1995/06/16  08:49:52  bduggan
 **	changed bzero to memset and bcopy to memcpy
 **
 **	Revision 1.17  1995/05/17  23:48:17  bduggan
 **	Added ImVfbPrint
 **	Added ImGetTransparency
 **
 **	Revision 1.16  1995/03/01  22:25:09  bduggan
 **	Fixed faulty ramp algorithm
 **
 **	Revision 1.15  1994/10/03  11:29:42  nadeau
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
 **	Revision 1.14  92/12/03  01:53:20  nadeau
 **	Changed names of ImRgbHsi to ImRgbToHsi and ImHsiRgb to
 **	ImHsiToRgb.  Moved ImVfbStat code to imvfbhist.c.
 **	
 **	Revision 1.13  92/09/02  13:15:52  vle
 **	Updated copyright notice.
 **	
 **	Revision 1.12  92/09/02  13:01:52  nadeau
 **	Rewrote ImVfbRoll to include additional error checking and be
 **	more efficient when copying between VFB's of identical type.
 **	
 **	Revision 1.11  92/09/01  12:26:18  nadeau
 **	Rearranged and partially rewrote ImVfbFill to add additional
 **	error checks and support filling of monochrome, write protect,
 **	Z-buffer, integer data, and float data fields.  Changed call
 **	arguments to ImVfbFill to add a srcVfb.
 **	
 **	Revision 1.10  92/08/26  12:48:04  groening
 **	added imvfbroll to roll vfb's
 **	
 **	Revision 1.9  92/08/07  11:56:52  groening
 **	added imvfbdata and imvfbroll
 **	
 **	Revision 1.8  92/07/14  09:29:34  groening
 **	Added ImVfbMix, ImVfbFill and corresponding subroutines
 **	like imSetPixelField, imSetPixelFieldHSI, ImHsiToRgb, ImRgbToHsi
 **	
 **	Revision 1.7  92/06/02  16:26:24  nadeau
 **	Added initial versions of ImVfbMix and ImVfbFade.
 **	
 **	Revision 1.6  91/10/03  09:20:42  nadeau
 **	Cosmetic changes to alloc.  Fixed bug in copy.
 **	
 **	Revision 1.5  91/03/08  14:35:35  nadeau
 **	Added comments.  Added IMVFBMONO.  Dropped IMVFBGRAY.  Changed
 **	IMVFBINDEX32 to IMVFBINDEX16.  Moved CLT alloc/free functions
 **	to a separate file (imclt.c).  Moved other general VFB functions
 **	from separate files into this one.
 **	
 **	Revision 1.4  90/07/09  07:15:45  mjb
 **	*** empty log message ***
 **	
 **	Revision 1.3  90/07/02  13:22:28  nadeau
 **	Updated comment and added inclusion of iminternal.h
 **	
 **	Revision 1.2  90/06/25  14:29:30  nadeau
 **	ImVfb -> ImVfb*, ImClt -> ImClt* type changes plus minor cleanup.
 **	
 **	Revision 1.1  90/03/06  17:33:19  mjb
 **	Initial revision
 **	
 **/

/**
 **  CODE CREDITS
 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1991.
 **     Custom development, Michael Bailey, San Diego Supercomputer Center, 1990.
 **/
#include "iminternal.h"

#ifdef __STDC__
static void 	imVfbFillRect( ImVfb *dstVfb, int sxl, int sxr, int syt,
	int syb, double xInc, double yInc, double fillStart, int fillField );
#else
static void	imVfbFillRect( );
#endif



/* sizes of various variable types:					*/
#define IM_SIZEOFBYTE	sizeof( unsigned char )
#define IM_SIZEOFSHORT	sizeof( unsigned short )
#define IM_SIZEOFINT	sizeof( unsigned int )
#define IM_SIZEOFFLOAT	sizeof( float )


/* ways of aligning to various types:					*/
#define IM_ALIGNBYTE(n)
#define IM_ALIGNINT(n)	( n += ( (m=(n%IM_SIZEOFINT) ) == 0 ? 0 : IM_SIZEOFINT-m ) )
#define IM_ALIGNFLOAT(n)	( n += ( (m=(n%IM_SIZEOFFLOAT)) == 0 ? 0 : IM_SIZEOFFLOAT-m ))





/*
 *  FUNCTION
 *	ImVfbAlloc	-  alocate a virtual frame buffer
 *
 *  DESCRIPTION
 *	The size of the VFB is computed based upon the fields mask.
 *	Space is allocated and byte offsets set up in the VFB structure.
 */

ImVfb *			/* Returns VFB				*/
#ifdef __STDC__
ImVfbAlloc( int width, int height, int fields )
#else
ImVfbAlloc( width, height, fields )
	int width, height;	/* size of framebuffer			*/
	int fields;		/* mask of what information to store	*/
#endif
{
	ImVfb *v;	/* ImVfb allocated			*/
	int nbytes;	/* # bytes per pixel			*/
	ImVfbPtr p;	/* pointer to the frame buffer		*/
	int m;			/* mod( nbytes, sizeof(int) )		*/
	int allbytes;		/* TRUE if only allocating byte data	*/


	/* check if have been asked to allocate anything:		*/

	if ( fields == 0 )
	{
		ImErrNo = IMEFIELD;
		return ( IMVFBNULL );
	}


	/* initialize:							*/

	allbytes = TRUE;
	nbytes = 0;

	v = (ImVfb *) malloc( sizeof(ImVfb) );
	if ( v == NULL )
	{
		ImErrNo = IMEMALLOC;
		return ( IMVFBNULL );
	}


        if ( fields & IMVFBRED )
                fields = (fields & ~IMVFBRED)   | IMVFBRGB;
        if ( fields & IMVFBGREEN )
                fields = (fields & ~IMVFBGREEN) | IMVFBRGB;
        if ( fields & IMVFBBLUE )
                fields = (fields & ~IMVFBBLUE)  | IMVFBRGB;

	v->vfb_width  = width;
	v->vfb_height = height;
	v->vfb_fields = fields;

	v->vfb_clt      = IMCLTNULL;

	v->vfb_roff     = -1;
	v->vfb_goff	= -1;
	v->vfb_boff	= -1;
	v->vfb_i8off    = -1;
	v->vfb_i16off	= -1;
	v->vfb_aoff     = -1;
	v->vfb_wpoff	= -1;
	v->vfb_zoff	= -1;
	v->vfb_moff	= -1;
	v->vfb_fpoff    = -1;
	v->vfb_ioff	= -1;


	/* check each fields possibility:				*/

	if  ( fields & IMVFBRGB ) 
	{
		IM_ALIGNBYTE( nbytes );
		v->vfb_roff = nbytes;
		nbytes += IM_SIZEOFBYTE;

		IM_ALIGNBYTE( nbytes );
		v->vfb_goff = nbytes;
		nbytes += IM_SIZEOFBYTE;

		IM_ALIGNBYTE( nbytes );
		v->vfb_boff = nbytes;
		nbytes += IM_SIZEOFBYTE;
	}

	if ( fields & IMVFBALPHA )
	{
		IM_ALIGNBYTE( nbytes );
		v->vfb_aoff = nbytes;
		nbytes += IM_SIZEOFBYTE;
	}

	if ( fields & IMVFBINDEX8 )
	{
		IM_ALIGNBYTE( nbytes );
		v->vfb_i8off = nbytes;
		nbytes += IM_SIZEOFBYTE;
	}

	if ( fields & IMVFBMONO )
	{
		/* Use 1 byte for each mono value.			*/
		IM_ALIGNBYTE( nbytes );
		v->vfb_moff = nbytes;
		nbytes += IM_SIZEOFBYTE;
	}

	if ( fields & IMVFBWPROT )
	{
		IM_ALIGNBYTE( nbytes );
		v->vfb_wpoff = nbytes;
		nbytes += IM_SIZEOFBYTE;
	}

	if ( fields & IMVFBINDEX16 )
	{
		IM_ALIGNINT( nbytes );
		v->vfb_i16off = nbytes;
		nbytes += IM_SIZEOFSHORT;
		allbytes = FALSE;
	}

	if ( fields & IMVFBZ )
	{
		IM_ALIGNINT( nbytes );
		v->vfb_zoff = nbytes;
		nbytes += IM_SIZEOFINT;
		allbytes = FALSE;
	}

	if ( fields & IMVFBFDATA )
	{
		IM_ALIGNFLOAT( nbytes );
		v->vfb_fpoff = nbytes;
		nbytes += IM_SIZEOFFLOAT;
		allbytes = FALSE;
	}

	if ( fields & IMVFBIDATA )
	{
		IM_ALIGNINT( nbytes );
		v->vfb_ioff = nbytes;
		nbytes += IM_SIZEOFINT;
		allbytes = FALSE;
	}



	/* pad so produces an even integer size, if necessary:		*/

	if ( ! allbytes )
		IM_ALIGNINT( nbytes );

	v->vfb_nbytes = nbytes;



	/* allocate the necessary frame buffer:				*/

	p = (ImVfbPtr) malloc( width*height*nbytes );
	if ( p == (ImVfbPtr)NULL )
	{
		free( (char *) v );
		ImErrNo = IMEMALLOC;
		return ( IMVFBNULL );
	}

	v->vfb_pfirst = p;
	v->vfb_plast  = p + ( nbytes * width * height )  -  nbytes;



#ifdef DEBUG
	fprintf( stderr, "vfb_pfirst   = %0x\n", v->vfb_pfirst );
	fprintf( stderr, "vfb_plast    = %0x\n", v->vfb_plast );
	fprintf( stderr, "vfb_width    = %3d\n", v->vfb_width );
	fprintf( stderr, "vfb_height   = %3d\n", v->vfb_height );
	fprintf( stderr, "vfb_fields   = %3d\n", v->vfb_fields );
	fprintf( stderr, "vfb_nbytes   = %3d\n", v->vfb_nbytes );
	fprintf( stderr, "vfb_roff     = %3d\n", v->vfb_roff );
	fprintf( stderr, "vfb_goff     = %3d\n", v->vfb_goff );
	fprintf( stderr, "vfb_boff     = %3d\n", v->vfb_boff );
	fprintf( stderr, "vfb_aoff     = %3d\n", v->vfb_aoff );
	fprintf( stderr, "vfb_i8off    = %3d\n", v->vfb_i8off );
	fprintf( stderr, "vfb_moff     = %3d\n", v->vfb_moff );
	fprintf( stderr, "vfb_wpoff    = %3d\n", v->vfb_wpoff );
	fprintf( stderr, "vfb_i16off   = %3d\n", v->vfb_i16off );
	fprintf( stderr, "vfb_zoff     = %3d\n", v->vfb_zoff );
	fprintf( stderr, "vfb_fpoff    = %3d\n", v->vfb_fpoff );
	fprintf( stderr, "vfb_ioff     = %3d\n", v->vfb_ioff );
#endif



	/* done: return the pointer to the ImVfb structure:		*/

	return( v );
}





/*
 *  FUNCTION
 *	ImVfbFree	-  deallocate a VFB
 *
 *  DESCRIPTION
 *	The VFB's data planes are freed, and then the VFB struct itself.
 */

void			/* Returns nothing			*/
#ifdef __STDC__
ImVfbFree( ImVfb *v )
#else
ImVfbFree( v )
	ImVfb *v;		/* VFB to free				*/
#endif
{
	if ( v != IMVFBNULL )
	{
		free( (char *) ImVfbQFirst(v) );
		free( (char *) v );
	}
}





/*
 *  FUNCTION
 *	ImVfbDup	-  duplicate a VFB
 *
 *  DESCRIPTION
 *	A new VFB is allocated that has the same attributes as the source
 *	VFB.  The source VFB's data is copied to the new VFB.
 *
 *	If the source VFB has a CLT, it is copied and added to the new VFB.
 */

ImVfb *				/* Returns new VFB		*/
#ifdef __STDC__
ImVfbDup( ImVfb* srcVfb )
#else
ImVfbDup( srcVfb )
	ImVfb *srcVfb;			/* VFB to duplicate		*/
#endif
{
	ImVfb *newVfb;		/* New VFB			*/
	ImClt *srcClt;			/* Original CLT			*/
	ImClt *newClt;			/* New CLT			*/

	newVfb = ImVfbAlloc( ImVfbQWidth( srcVfb ), ImVfbQHeight( srcVfb ),
		ImVfbQFields( srcVfb ) );
	if( newVfb == 0 )
	{
		ImErrNo = IMEMALLOC;
		return( IMVFBNULL );
	}

	memcpy( (void *)ImVfbQFirst( newVfb ), (void *)ImVfbQFirst( srcVfb ),
		ImVfbQNBytes( srcVfb ) * ImVfbQHeight( srcVfb ) *
		ImVfbQWidth( srcVfb )  );

	srcClt = ImVfbQClt( srcVfb );
	if ( srcClt != IMCLTNULL )
	{
		newClt = ImCltDup( srcClt );
		if ( newClt == IMCLTNULL )
		{
			ImVfbFree( newVfb );
			return ( IMVFBNULL );	/* ImErrNo already set	*/
		}
	}
	ImVfbSClt( newVfb, srcClt );

	return ( newVfb );
}





/*
 *  FUNCTION
 *	ImVfbCopy	-  copy  a subarea within a virtual frame buffer.
 *
 *  DESCRIPTION
 *	Copy a portion of a virtual frame buffer to a new location.
 *
 *	When the incomming and outgoing VFB's have the same set of fields,
 *	we just do a memcopy().  Otherwise we have to copy each field
 *	explicitly.
 */

ImVfb *			/* Returnes copy of source VFB		*/
#ifdef __STDC__
ImVfbCopy( ImVfb *srcVfb, int srcXLeft, int srcYTop, int srcDX, int srcDY, int fieldMask,
	ImVfb* dstVfb, int dstXLeft, int dstYTop )
#else
ImVfbCopy( srcVfb, srcXLeft, srcYTop, srcDX, srcDY, fieldMask,
	dstVfb, dstXLeft, dstYTop )
	ImVfb *srcVfb;	/* the vfb whose size will change	*/
	int srcXLeft, srcYTop;	/* upper left corner of copy area	*/
	int srcDX, srcDY;	/* size of copy area			*/
	int fieldMask;		/* Fields to copy			*/
	ImVfb *dstVfb;		/* the vfb to put the result in		*/
	int dstXLeft, dstYTop;	/* upper left corner of the dest. area	*/
#endif
{
	ImVfbPtr psrc;		/* pointer into source vfb		*/
	ImVfbPtr pdst;		/* pointer into destination vfb		*/
	int i, j;		/* counter and temp storage		*/
	int nbytes;		/* # bytes of vfb storage per scanline	*/
	int f;			/* vfb field description		*/
	int sxl, sxr, syb, syt;	/* copy boundaries in source vfb	*/
	int dw, dh;		/* copy size				*/


	/* Truncate desired size, if necessary.				*/
	sxl = srcXLeft;
	if ( sxl < 0 ) 
		sxl = 0;
	if ( sxl >= ImVfbQWidth( srcVfb ) )
		sxl = ImVfbQWidth( srcVfb )  -  1;

	sxr = srcXLeft + srcDX - 1;
	if ( sxr < 0 )
		sxr = 0;
	if ( sxr >= ImVfbQWidth( srcVfb ) )
		sxr = ImVfbQWidth( srcVfb )  -  1;

	syt = srcYTop;
	if ( syt < 0 )
		syt = 0;
	if ( syt >= ImVfbQHeight( srcVfb ) )
		syt = ImVfbQHeight( srcVfb )  -  1;

	syb = srcYTop + srcDY - 1;
	if ( syb < 0 )
		syb = 0;
	if ( syb >= ImVfbQHeight( srcVfb ) )
		syb = ImVfbQHeight( srcVfb )  -  1;


	/* Swap left and right, top and bottom if necessary.		*/
	if ( sxl > sxr )
	{
		i = sxl;	sxl = sxr;	sxr = i;
	}
	if ( syt > syb )
	{
		i = syb;	syb = syt;	syt = i;
	}


	/* determine widths and see if they are legal:			*/
	dw = sxr - sxl + 1;
	if ( dw <= 0 )
	{
		ImErrNo = IMEWIDTH;
		return( IMVFBNULL );
	}

	dh = syb - syt + 1;
	if ( dh <= 0 )
	{
		ImErrNo = IMEHEIGHT;
		return( IMVFBNULL );
	}



	/* determine the fields on this vfb:				*/
	f = ImVfbQFields( srcVfb );
	if ( fieldMask == IMVFBALL )
		fieldMask = f;
	else
		fieldMask = f & fieldMask;

	if ( fieldMask == 0 )
	{
		ImErrNo = IMEFIELD;
		return ( IMVFBNULL );
	}


	if ( dstVfb == IMVFBNEW )
	{
		/* Allocate the new vfb.				*/
		dstVfb = ImVfbAlloc( dw, dh, fieldMask );
		if ( dstVfb == IMVFBNULL )
		{
			ImErrNo = IMEMALLOC;
			return( IMVFBNULL );
	  	}
	}
	else
	{
		fieldMask &= ImVfbQFields( dstVfb );
	}


	/* Loop through destination framebuffer, filling it as we go.	*/
	if ( f == fieldMask && f == ImVfbQFields( dstVfb ) )
	{
		/* # bytes per scanline:				*/
		nbytes = ImVfbQNBytes( srcVfb ) * dw;

		psrc = ImVfbQPtr( srcVfb, sxl, syt );
		pdst = ImVfbQPtr(dstVfb, dstXLeft, dstYTop);
		for( i = syt; i <= syb; i++ )
		{
			memcpy( (void *) pdst, (void *) psrc, nbytes );
			ImVfbSDown( srcVfb, psrc );
			ImVfbSDown( dstVfb, pdst );
		}
		return( dstVfb );
	}
	for ( i = syt; i <= syb; i++, dstYTop++ )
	{
		psrc = ImVfbQPtr( srcVfb, sxl, i );
		pdst = ImVfbQPtr( dstVfb, dstXLeft, dstYTop );
		for ( j = sxl; j <= sxr; j++ )
		{
			if ( fieldMask & IMVFBMONO )
			{
				ImVfbSMono( dstVfb, pdst, ImVfbQMono( srcVfb, psrc ) );
			}
			if ( fieldMask & IMVFBINDEX8 )
			{
				ImVfbSIndex8( dstVfb, pdst, ImVfbQIndex8( srcVfb, psrc ) );
			}
			if ( fieldMask & IMVFBINDEX16 )
			{
				ImVfbSIndex16( dstVfb, pdst, ImVfbQIndex16( srcVfb, psrc ) );
			}
			if ( fieldMask & IMVFBRGB )
			{
				ImVfbSRed( dstVfb, pdst, ImVfbQRed( srcVfb, psrc ) );
				ImVfbSGreen( dstVfb, pdst, ImVfbQGreen( srcVfb, psrc ) );
				ImVfbSBlue( dstVfb, pdst, ImVfbQBlue( srcVfb, psrc ) );
			}
			if ( fieldMask & IMVFBALPHA )
			{
				ImVfbSAlpha( dstVfb, pdst, ImVfbQAlpha( srcVfb, psrc ) );
			}
			if ( fieldMask & IMVFBIDATA )
			{
				ImVfbSIData( dstVfb, pdst, ImVfbQIData( srcVfb, psrc ) );
			}
			if ( fieldMask & IMVFBFDATA )
			{
				ImVfbSFData( dstVfb, pdst, ImVfbQFData( srcVfb, psrc ) );
			}
			if ( fieldMask & IMVFBZ )
			{
				ImVfbSZ( dstVfb, pdst, ImVfbQZ( srcVfb, psrc ) );
			}
			if ( fieldMask & IMVFBWPROT )
			{
				ImVfbSWProt( dstVfb, pdst, ImVfbQWProt( srcVfb, psrc ) );
			}
			ImVfbSInc( srcVfb, psrc );
			ImVfbSInc( dstVfb, pdst );
		}
	}
	return( dstVfb );
}





/*
 *  FUNCTION
 *	IM_SETPIXEL	-  set a pixel to a value.
 *	ImVfbClear	-  clear a virtual frame buffer.
 *
 *  DESCRIPTION
 *	Erases a portion of a VFB.
 */

#define IM_SETPIXEL(dstVfb,pdst,fieldMask,value)			\
{									\
	if ( fieldMask & IMVFBRGB )					\
	{								\
		ImVfbSRed( dstVfb, pdst, (value) );			\
		ImVfbSGreen( dstVfb, pdst, (value) );			\
		ImVfbSBlue( dstVfb, pdst, (value) );			\
	}								\
	if ( fieldMask & IMVFBMONO )					\
		ImVfbSMono( dstVfb, pdst, (value) );			\
	if ( fieldMask & IMVFBINDEX8 )					\
		ImVfbSIndex8( dstVfb, pdst, (value) );			\
	if ( fieldMask & IMVFBINDEX16 )					\
		ImVfbSIndex16( dstVfb, pdst, (value) );			\
	if ( fieldMask & IMVFBALPHA )					\
		ImVfbSAlpha( dstVfb, pdst, (value) );			\
	if ( fieldMask & IMVFBIDATA )					\
		ImVfbSIData( dstVfb, pdst, (value) );			\
	if ( fieldMask & IMVFBFDATA )					\
		ImVfbSFData( dstVfb, pdst, (value) );			\
	if ( fieldMask & IMVFBZ )					\
		ImVfbSZ( dstVfb, pdst, (value) );			\
	if ( fieldMask & IMVFBWPROT )					\
		ImVfbSWProt( dstVfb, pdst, (value) );			\
}

ImVfb *			/* Returns cleared VFB			*/
#ifdef __STDC__
ImVfbClear( int fieldMask, int value, ImVfb* dstVfb )
#else
ImVfbClear( fieldMask, value, dstVfb )
	int fieldMask;		/* Fields to copy			*/
	int value;		/* Value to set fields to		*/
	ImVfb *dstVfb;		/* the vfb to put the result in		*/
#endif
{
	ImVfbPtr pdst;		/* pointer into destination vfb		*/
	int i, j;		/* counter and temp storage		*/
	int nbytes;		/* # bytes of vfb storage per scanline	*/
	int f;			/* vfb field description		*/
	int width, height;	/* Image width and height		*/


	width  = ImVfbQWidth(  dstVfb );
	height = ImVfbQHeight( dstVfb );
	f      = ImVfbQFields( dstVfb );
	fieldMask = f & fieldMask;


	/* Loop through destination framebuffer, filling it as we go.	*/
	if ( f == fieldMask && f == ImVfbQFields( dstVfb ) && value == 0 )
	{
		/* # bytes per scanline:				*/
		nbytes = ImVfbQNBytes( dstVfb ) * width;

		pdst = ImVfbQFirst( dstVfb );
		for( i = 0; i < height; i++ )
		{
			memset( (void *) pdst, 0x00, nbytes );
			ImVfbSDown( dstVfb, pdst );
		}
		return( dstVfb );
	}

	pdst = ImVfbQFirst( dstVfb );
	for ( i = 0; i < height; i++ )
	{
		for ( j = 0; j < width; j++ )
		{
			IM_SETPIXEL( dstVfb, pdst, fieldMask, value );
			ImVfbSInc( dstVfb, pdst );
		}
	}

	return ( dstVfb );
}




/*
 *  FUNCTION
 *	ImVfbMix	-  mix together two virtual frame buffers
 *
 *  DESCRIPTION
 *	Pixels in the two source images are mixed together based upon the
 *	given weighting factors:
 *
 *		w1 = src1Weight / (src1Weight + src2Weight)
 *		w2 = src2Weight / (src1Weight + src2Weight)
 *
 *		dst = src1 * w1 + src2 * w2
 */

ImVfb *			/* Returns new VFB			*/
#ifdef __STDC__
ImVfbMix(ImVfb* src1Vfb, double src1Weight, ImVfb* src2Vfb, double src2Weight, int fieldMask, ImVfb *dstVfb )
#else
ImVfbMix( src1Vfb, src1Weight, src2Vfb, src2Weight, fieldMask, dstVfb )
	ImVfb *src1Vfb;		/* First source VFB			*/
	double  src1Weight;	/* First source's weighting factor	*/
	ImVfb *src2Vfb;		/* Second source VFB			*/
	double  src2Weight;	/* Second source's weighting factor	*/
	int    fieldMask;	/* Fields to combine			*/
	ImVfb *dstVfb;		/* Resulting VFB			*/
#endif
{
	ImVfbPtr psrc1, psrc2;	/* Source VFB pixel pointers		*/
	ImVfbPtr pdst;		/* Destination VFB pixel pointer	*/
	int pixel1, pixel2;	/* Source pixel values			*/
	int i, j;		/* Counters and temp storage		*/
	int f;			/* vfb field description		*/
	int dw, dh;		/* copy size				*/
	float divisor;		/* Weighting factor divisor		*/
	int   tempMask;

	/*
	 *  Normalize weighting factors.
	 */
	divisor = 1.0 / (src1Weight + src2Weight);
	src1Weight *= divisor;
	src2Weight *= divisor;


	/*
	 *  Determine the fields to be affected.
	 */
	tempMask = fieldMask;
	f = ImVfbQFields( src1Vfb ) & ImVfbQFields( src2Vfb );
	if ( fieldMask == IMVFBALL )
		fieldMask = f;
	else
		fieldMask = f & fieldMask;
	if ( (tempMask&IMVFBRED) && (f&IMVFBRGB)) fieldMask |= IMVFBRED ;
	if ( (tempMask&IMVFBGREEN) && (f&IMVFBRGB)) fieldMask |= IMVFBGREEN ;
	if ( (tempMask&IMVFBBLUE) && (f&IMVFBRGB)) fieldMask |= IMVFBBLUE ;
	if ( fieldMask == 0 )
	{
		ImErrNo = IMEFIELD;
		return ( IMVFBNULL );
	}

	/*
	 *  Determine XY range of pixels to be dissolved.
	 */
	dw = ImVfbQWidth( src1Vfb ) < ImVfbQWidth( src2Vfb ) ?
		ImVfbQWidth( src1Vfb ) : ImVfbQWidth( src2Vfb );
	dh = ImVfbQHeight( src1Vfb ) < ImVfbQHeight( src2Vfb ) ?
		ImVfbQHeight( src1Vfb ) : ImVfbQHeight( src2Vfb );

	if ( dstVfb == IMVFBNEW )
	{
		/* Create a new VFB at the size of the smallest source.	*/
		dstVfb = ImVfbAlloc( dw, dh, fieldMask );
		if ( dstVfb == IMVFBNULL )
		{
			ImErrNo = IMEMALLOC;
			return( IMVFBNULL );
	  	}
	}
	else
	{
		if ( dw > ImVfbQWidth( dstVfb ) )
			dw = ImVfbQWidth( dstVfb );
		if ( dh > ImVfbQHeight( dstVfb ) )
			dh = ImVfbQHeight( dstVfb );
	}

	if ( dw <= 0 )
	{
		ImErrNo = IMEWIDTH;
		return( IMVFBNULL );
	}
	if ( dh <= 0 )
	{
		ImErrNo = IMEHEIGHT;
		return( IMVFBNULL );
	}


	/*
	 *  Loop through the VFB's, copying pixels to the destination VFB
	 *  using the formula:
	 *
	 *	dst = src1 * src1Wieght + src2 * src2Weight
	 */
	for ( i = 0; i < dh; i++ )
	{
		psrc1 = ImVfbQPtr( src1Vfb, 0, i );
		psrc2 = ImVfbQPtr( src2Vfb, 0, i );
		pdst  = ImVfbQPtr( dstVfb,  0, i );

		for ( j = 0; j < dw; j++ )
		{
			if ( fieldMask & IMVFBMONO )
			{
				pixel1 = ImVfbQMono( src1Vfb, psrc1 );
				pixel2 = ImVfbQMono( src2Vfb, psrc2 );
				ImVfbSMono( dstVfb, pdst,
					(int)( pixel1 * src1Weight +
						pixel2 * src2Weight ) );
			}
			if ( fieldMask & IMVFBINDEX8 )
			{
				pixel1 = ImVfbQIndex8( src1Vfb, psrc1 );
				pixel2 = ImVfbQIndex8( src2Vfb, psrc2 );
				ImVfbSIndex8( dstVfb, pdst,
					(int)( pixel1 * src1Weight +
						pixel2 * src2Weight ) );
			}
			if ( fieldMask & IMVFBINDEX16 )
			{
				pixel1 = ImVfbQIndex16( src1Vfb, psrc1 );
				pixel2 = ImVfbQIndex16( src2Vfb, psrc2 );
				ImVfbSIndex16( dstVfb, pdst,
					(int)( pixel1 * src1Weight +
						pixel2 * src2Weight ) );
			}
			if ( fieldMask & IMVFBRED )
			{
				pixel1 = ImVfbQRed( src1Vfb, psrc1 );
				pixel2 = ImVfbQRed( src2Vfb, psrc2 );
				ImVfbSRed( dstVfb, pdst,
					(int)( pixel1 * src1Weight +
						pixel2 * src2Weight ) );
			}
			if ( fieldMask & IMVFBGREEN )
			{
				pixel1 = ImVfbQGreen( src1Vfb, psrc1 );
				pixel2 = ImVfbQGreen( src2Vfb, psrc2 );
				ImVfbSGreen( dstVfb, pdst,
					(int)( pixel1 * src1Weight +
						pixel2 * src2Weight ) );
			}
			if ( fieldMask & IMVFBBLUE )
			{
				pixel1 = ImVfbQBlue( src1Vfb, psrc1 );
				pixel2 = ImVfbQBlue( src2Vfb, psrc2 );
				ImVfbSBlue( dstVfb, pdst,
					(int)( pixel1 * src1Weight +
						pixel2 * src2Weight ) );
			}
			if ( fieldMask & IMVFBRGB )
			{
				pixel1 = ImVfbQRed( src1Vfb, psrc1 );
				pixel2 = ImVfbQRed( src2Vfb, psrc2 );
				ImVfbSRed( dstVfb, pdst,
					(int)( pixel1 * src1Weight +
						pixel2 * src2Weight ) );
				pixel1 = ImVfbQGreen( src1Vfb, psrc1 );
				pixel2 = ImVfbQGreen( src2Vfb, psrc2 );
				ImVfbSGreen( dstVfb, pdst,
					(int)( pixel1 * src1Weight +
						pixel2 * src2Weight ) );
				pixel1 = ImVfbQBlue( src1Vfb, psrc1 );
				pixel2 = ImVfbQBlue( src2Vfb, psrc2 );
				ImVfbSBlue( dstVfb, pdst,
					(int)( pixel1 * src1Weight +
						pixel2 * src2Weight ) );
			}
			if ( fieldMask & IMVFBALPHA )
			{
				pixel1 = ImVfbQAlpha( src1Vfb, psrc1 );
				pixel2 = ImVfbQAlpha( src2Vfb, psrc2 );
				ImVfbSAlpha( dstVfb, pdst,
					(int)( pixel1 * src1Weight +
						pixel2 * src2Weight ) );
			}
			if ( fieldMask & IMVFBIDATA )
			{
				pixel1 = ImVfbQIData( src1Vfb, psrc1 );
				pixel2 = ImVfbQIData( src2Vfb, psrc2 );
				ImVfbSIData( dstVfb, pdst,
					(int)( pixel1 * src1Weight +
						pixel2 * src2Weight ) );
			}
			if ( fieldMask & IMVFBFDATA )
			{
				ImVfbSFData( dstVfb, pdst,
					( ImVfbQFData( src1Vfb, psrc1 ) *
					src1Weight +
					ImVfbQFData( src2Vfb, psrc2 ) *
					src2Weight ) );
			}
			if ( fieldMask & IMVFBZ )
			{
				pixel1 = ImVfbQZ( src1Vfb, psrc1 );
				pixel2 = ImVfbQZ( src2Vfb, psrc2 );
				ImVfbSZ( dstVfb, pdst,
					(int)( pixel1 * src1Weight +
						pixel2 * src2Weight ) );
			}
			if ( fieldMask & IMVFBWPROT )
			{
				pixel1 = ImVfbQWProt( src1Vfb, psrc1 );
				pixel2 = ImVfbQWProt( src2Vfb, psrc2 );
				ImVfbSWProt( dstVfb, pdst,
					(int)( pixel1 * src1Weight +
						pixel2 * src2Weight ) );
			}
			ImVfbSInc( src1Vfb, psrc1 );
			ImVfbSInc( src2Vfb, psrc2 );
			ImVfbSInc( dstVfb,  pdst );
		}
	}
	return( dstVfb );
}





/*
 *  FUNCTION
 *	ImVfbFade	-  fade a virtual frame buffer to 0's
 *
 *  DESCRIPTION
 *	Pixels are faded towards zero in all fields using the given weighting
 *	factor:
 *
 *		dst = src * weight
 */

ImVfb *			/* Returns new VFB			*/
#ifdef __STDC__
ImVfbFade( ImVfb* srcVfb, double srcWeight, int fieldMask, ImVfb* dstVfb )
#else
ImVfbFade( srcVfb, srcWeight, fieldMask, dstVfb )
	ImVfb *srcVfb;		/* First source VFB			*/
	double  srcWeight;	/* First source's weighting factor	*/
	int    fieldMask;	/* Fields to combine			*/
	ImVfb *dstVfb;		/* Resulting VFB			*/
#endif
{
	ImVfbPtr psrc;		/* Source VFB pixel pointers		*/
	ImVfbPtr pdst;		/* Destination VFB pixel pointer	*/
	int pixel;		/* Source pixel values			*/
	int i, j;		/* Counters and temp storage		*/
	int f;			/* vfb field description		*/
	int dw, dh;		/* copy size				*/


	/*
	 *  Determine the fields to be affected.
	 */
	f = ImVfbQFields( srcVfb );
	if ( fieldMask == IMVFBALL )
		fieldMask = f;
	else
		fieldMask = f & fieldMask;
	if ( fieldMask == 0 )
	{
		ImErrNo = IMEFIELD;
		return ( IMVFBNULL );
	}


	/*
	 *  Determine XY range of pixels to be faded.
	 */
	dw = ImVfbQWidth( srcVfb );
	dh = ImVfbQHeight( srcVfb );
	if ( dstVfb == IMVFBNEW )
	{
		/* Create a new VFB at the size of the smallest source.	*/
		dstVfb = ImVfbAlloc( dw, dh, fieldMask );
		if ( dstVfb == IMVFBNULL )
		{
			ImErrNo = IMEMALLOC;
			return( IMVFBNULL );
	  	}
	}
	else
	{
		fieldMask &= ImVfbQFields( dstVfb );
		if ( dw > ImVfbQWidth( dstVfb ) )
			dw = ImVfbQWidth( dstVfb );
		if ( dh > ImVfbQHeight( dstVfb ) )
			dh = ImVfbQHeight( dstVfb );
	}
	if ( dw <= 0 )
	{
		ImErrNo = IMEWIDTH;
		return( IMVFBNULL );
	}
	if ( dh <= 0 )
	{
		ImErrNo = IMEHEIGHT;
		return( IMVFBNULL );
	}


	/*
	 *  Loop through the VFB's, copying pixels to the destination VFB
	 *  using the formula:
	 *
	 *	dst = src * srcWieght
	 */
	for ( i = 0; i < dh; i++ )
	{
		psrc = ImVfbQPtr( srcVfb, 0, i );
		pdst = ImVfbQPtr( dstVfb,  0, i );

		for ( j = 0; j < dw; j++ )
		{
			if ( fieldMask & IMVFBMONO )
			{
				pixel = ImVfbQMono( srcVfb, psrc );
				ImVfbSMono( dstVfb, pdst,
					(int)( pixel * srcWeight ) & 0x1 );
			}
			if ( fieldMask & IMVFBINDEX8 )
			{
				pixel = ImVfbQIndex8( srcVfb, psrc );
				ImVfbSIndex8( dstVfb, pdst,
					(int)( pixel * srcWeight ) & 0xFF );
			}
			if ( fieldMask & IMVFBINDEX16 )
			{
				pixel = ImVfbQIndex16( srcVfb, psrc );
				ImVfbSIndex16( dstVfb, pdst,
					(int)( pixel * srcWeight ) & 0xFFFF );
			}
			if ( fieldMask & IMVFBRGB )
			{
				pixel = ImVfbQRed( srcVfb, psrc );
				ImVfbSRed( dstVfb, pdst,
					(int)( pixel * srcWeight ) & 0xFF );
				pixel = ImVfbQGreen( srcVfb, psrc );
				ImVfbSGreen( dstVfb, pdst,
					(int)( pixel * srcWeight ) & 0xFF );
				pixel = ImVfbQBlue( srcVfb, psrc );
				ImVfbSBlue( dstVfb, pdst,
					(int)( pixel * srcWeight ) & 0xFF );
			}
			if ( fieldMask & IMVFBALPHA )
			{
				pixel = ImVfbQAlpha( srcVfb, psrc );
				ImVfbSAlpha( dstVfb, pdst,
					(int)( pixel * srcWeight ) & 0xFF );
			}
			if ( fieldMask & IMVFBIDATA )
			{
				pixel = ImVfbQIData( srcVfb, psrc );
				ImVfbSIData( dstVfb, pdst,
					(int)( pixel * srcWeight ) );
			}
			if ( fieldMask & IMVFBFDATA )
			{
				ImVfbSFData( dstVfb, pdst,
					( ImVfbQFData( srcVfb, psrc ) *
					srcWeight ) );
			}
			if ( fieldMask & IMVFBZ )
			{
				pixel = ImVfbQZ( srcVfb, psrc );
				ImVfbSZ( dstVfb, pdst,
					(int)( pixel * srcWeight ) );
			}
			if ( fieldMask & IMVFBWPROT )
			{
				pixel = ImVfbQWProt( srcVfb, psrc );
				ImVfbSWProt( dstVfb, pdst,
					(int)( pixel * srcWeight ) );
			}
			ImVfbSInc( srcVfb, psrc );
			ImVfbSInc( dstVfb, pdst );
		}
	}
	return( dstVfb );
}




/*
 *  FUNCTION
 *	ImHsiToRgb( hsi, rgb )
 *
 *  DESCRIPTION
 *	convert a hue-saturation-intensity into a red-green-blue value
 */

#define IM_Round(x)	( (int)( (x) + 0.5 ) )

void				/* Returns nothing		*/
#ifdef __STDC__
ImHsiToRgb( float hsi[3], int rgb[3] )
#else
ImHsiToRgb( hsi, rgb )
	float hsi[3];			/* HSI to convert		*/
	int rgb[3];			/* Returned RGB			*/
#endif
{
	float h, s, i;			/* hue, sat, intens		*/
	float delta;			/* change in color intens	*/
	float r, g, b;			/* red, green, blue		*/
	int ih;				/* integer hue			*/

	/* guarantee valid input:					*/
	h = hsi[0] /360.;

	ih = (int) h;
	h = h - (float)ih;

	s = hsi[1];
	if( s < 0. )
		s = 0.;
	if( s > 1. )
		s = 1.;

	i = hsi[2];
	if( i < 0. )
		i = 0.;
	if( i > 1. )
		i = 1.;


	/* get an rgb from the hue itself:				*/
	if( h <=  (1./6.) )
	{
	    r = 1.0;
	    g = 6. * h;
	    b = 0.;
	}
	else if( h <= (2./6.) )
	{
	    r = 2. - 6. * h;
	    g = 1.;
	    b = 0.;
	}
	else if(  h <= (3./6.) )
	{
	    r = 0.;
	    g = 1.;
	    b = 6. * h - 2.;
	}
	else if(  h <= (4./6.) )
	{
	    r = 0.;
	    g = 4. - 6. * h;
	    b = 1.;
	}
	else if(  h <= (5./6.) )
	{
	    r = 6. * h - 4.;
	    g = 0.;
	    b = 1.;
	}
	else
	{
	    r = 1.;
	    g = 0.;
	    b = 6. - 6. * h;
	}


	/* add in the saturation and intensity effects:			*/
	/* red:								*/
	delta = s * ( r - 0.5 );
	if( i <= 0.5 )
		r = 2.*i*( 0.5 + delta );
	else
		r = 0.5 + delta + (2.*i-1.)*( 0.5 - delta );

	/* green:							*/
	delta = s * ( g - 0.5 );
	if( i <= 0.5 )
		g = 2.*i*( 0.5 + delta );
	else
		g = 0.5 + delta + (2.*i-1.)*( 0.5 - delta );

	/* blue:							*/
	delta = s * ( b - 0.5 );
	if( i <= 0.5 )
		b = 2.*i*( 0.5 + delta );
	else
		b = 0.5 + delta + (2.*i-1.)*( 0.5 - delta );


	/* convert to integer in the range 0-255:			*/
	rgb[0] = IM_Round( 255. * r );
	rgb[1] = IM_Round( 255. * g );
	rgb[2] = IM_Round( 255. * b );
}





/*
 *  FUNCTION
 *	RgbHsi
 *
 *  DESCRIPTION
 *	convert a red-green-blue value into hue-saturation-intensity
 */

void				/* Returns nothing		*/
#ifdef __STDC__
ImRgbToHsi( int rgb[3], float hsi[3] )
#else
ImRgbToHsi( rgb, hsi )
	int rgb[3];			/* RGB to convert		*/
	float hsi[3];			/* HSI to return		*/
#endif
{
	int r, g, b;		/* red, green, blue		*/
	int min, max;		/* min and max rgb values	*/
	float fmin, fmax, diff;		/* min, max, and range of rgb vals */
	float hue, sat, intens;		/* h s i			*/
	float cr, cg, cb;		/* coefficients for computing hue */


	/* guarantee valid input:					*/
	r = rgb[0] & 255;
	g = rgb[1] & 255;
	b = rgb[2] & 255;


	/* determine min and max color primary values:			*/
	min = r;
	max = r;
	if( g < min ) min = g;
	if( g > max ) max = g;
	if( b < min ) min = b;
	if( b > max ) max = b;

	fmin = (float)min;
	fmax = (float)max;
	diff = fmax - fmin;


	/* special case r==g==b==0 */
	if( max == 0 )
	{
		hsi[0] = 0.;
		hsi[1] = 0.;
		hsi[2] = 0.;
		return;
	}


	/* special case r==g==b==255 */
	if( min == 255 )
	{
		hsi[0] = 0.;
		hsi[1] = 1.;
		hsi[2] = 1.;
		return;
	}


	/* compute saturation:						*/
	intens =  ( fmin + fmax ) / (255.+255.);

	if( intens <= 0.5 )
		sat = diff / (fmax + fmin);
	else
		sat = diff / ( 255.+255. - fmax - fmin );


	/* compute hue:							*/
	if( max == min )
		hue = 0.0;
	else
	{
		cr = ( fmax-(float)r ) / diff;
		cg = ( fmax-(float)g ) / diff;
		cb = ( fmax-(float)b ) / diff;

		if( max == r ) 
			hue =      cb - cg;
		else if( max == g ) 
			hue = 2. + cr - cb;
		else if( max == b ) 
			hue = 4. + cg - cr;
	}


	hue *= 60.0;
	if( hue < 0.0 )
		hue += 360.0;
	if( hue > 360.0 )
		hue -= 360.0;


	/* store output values:						*/
	hsi[0] = hue;
	hsi[1] = sat;
	hsi[2] = intens;
}



/*
 *  FUNCTION
 *	ImVfbRoll	-  roll a VFB
 *
 *  DESCRIPTION
 *	If no destination VFB is given, a new destination VFB is created.
 *
 *	The source VFB is then copied into the destination VFB, rolling
 *	its pixels horizontally, vertically, or both.
 */

ImVfb *			/* Returns rolled VFB			*/
#ifdef __STDC__
ImVfbRoll( ImVfb *sourceVfb, int xPixels, int yPixels, ImVfb* dstVfb)
#else
ImVfbRoll( sourceVfb, xPixels, yPixels, dstVfb)
	ImVfb   *sourceVfb;	/* VFB to roll				*/
	int      xPixels;	/* amount of horizontal pixels to roll	*/
	int      yPixels;	/* amount of vertical pixels to roll	*/
	ImVfb   *dstVfb;	/* VFB to overwrite with rolled source	*/
#endif
{
	ImVfb   *sVfb;		/* VFB to overwrite with rolled source	*/
	int      dw, dh;	/* column, row indices			*/
	int	 i,j;		/* counters				*/
	int      x, y;		/* New pixel location			*/
	ImVfbPtr psrc, psrc2;	/* Source VFB pixel pointers		*/
	ImVfbPtr pdst, pdst2;	/* Destination VFB pixel pointers	*/
	int	 fieldMask;	/* Fields to roll			*/
	int	 n1, n2;	/* # of bytes to copy			*/


	/*
	 *  Check that we've been given acceptable source and destination
	 *  VFBs and valid pixel roll amounts.
	 */
	if ( sourceVfb == IMVFBNULL )
	{
		ImErrNo = IMENOVFB;
		return ( IMVFBNULL );
	}
	dw        = ImVfbQWidth( sourceVfb );
	dh        = ImVfbQHeight( sourceVfb );
	fieldMask = ImVfbQFields( sourceVfb );
	if ( dstVfb != IMVFBNEW )
	{
		if ( dw != ImVfbQWidth( dstVfb ) )
		{
			ImErrNo = IMEWIDTH;
			return ( IMVFBNULL );
		}
		if ( dh != ImVfbQHeight( dstVfb ) )
		{
			ImErrNo = IMEHEIGHT;
			return ( IMVFBNULL );
		}
		if ( fieldMask != ImVfbQFields( dstVfb ) )
		{
			ImErrNo = IMEFIELD;
			return ( IMVFBNULL );
		}
	}

	xPixels %= dw;
	yPixels %= dh;
	if ( xPixels < 0 )
		xPixels += dw;
	if ( yPixels < 0 )
		yPixels += dh;


	/*
	 *  Create a new destination VFB if necessary.
	 *  If the source and destination are the same, create a temporary
	 *  copy of the source before doing the roll.
	 */
	sVfb = sourceVfb;
	if ( dstVfb == IMVFBNEW )
	{
		if ( (dstVfb = ImVfbAlloc( dw, dh, fieldMask )) == IMVFBNULL )
		{
			ImErrNo = IMEMALLOC;
			return( IMVFBNULL );
		}
	}
	else if ( sourceVfb == dstVfb )
	{
		if ( (sVfb = ImVfbDup( sourceVfb )) == IMVFBNULL )
			return ( IMVFBNULL );	/* ImErrNo already set	*/
	}
 

	/*
	 *  Roll the image by copying pixels from the source to the
	 *  destination.  If the source and destination VFB's have the
	 *  same fields, we can speed up the copy by using memcopy().
	 */
	if ( fieldMask == ImVfbQFields( dstVfb ) )
	{
		psrc  = ImVfbQFirst( sVfb );
		psrc2 = ImVfbQPtr( sVfb, (dw - xPixels), 0 );
		pdst  = ImVfbQPtr( dstVfb, xPixels, yPixels );
		pdst2 = ImVfbQPtr( dstVfb, 0, yPixels );
		n1    = ImVfbQNBytes( sVfb ) * (dw - xPixels);
		n2    = ImVfbQNBytes( sVfb ) * xPixels;

		for( i = 0, y = yPixels; i < dh; i++, y++ )
		{
			if ( y == dh )
			{
				y = 0;
				pdst  = ImVfbQPtr( dstVfb, xPixels, 0 );
				pdst2 = ImVfbQPtr( dstVfb, 0, 0 );
			}
			if ( n1 != 0 )
				memcpy( (void *)pdst,  (void *)psrc,  n1 );
			if ( n2 != 0 )
				memcpy( (void *)pdst2, (void *)psrc2, n2 );
			ImVfbSDown( sVfb, psrc );
			ImVfbSDown( sVfb, psrc2 );
			ImVfbSDown( dstVfb, pdst );
			ImVfbSDown( dstVfb, pdst2 );
		}

		if ( sourceVfb != sVfb )
			ImVfbFree( sVfb );
		return( dstVfb );
	}


	psrc = ImVfbQFirst( sVfb );
	for ( i = 0, y = yPixels; i < dh; i++, y++ )
	{
		pdst = ImVfbQPtr( dstVfb, xPixels, y );
		if ( y == dh )
		{
			y = 0;
			pdst = ImVfbQPtr( dstVfb, xPixels, 0 );
		}
		for ( j = 0, x = xPixels; j < dw; j++, x++ )
		{
			if ( x == dw )
			{
				x = 0;
				pdst = ImVfbQPtr( dstVfb, 0, y );
			}
			if ( fieldMask & IMVFBMONO )
				ImVfbSMono( dstVfb, pdst, ImVfbQMono( sVfb, psrc ) );
			if ( fieldMask & IMVFBINDEX8 )
				ImVfbSIndex8( dstVfb, pdst, ImVfbQIndex8( sVfb, psrc ) );
			if ( fieldMask & IMVFBINDEX16 )
				ImVfbSIndex16( dstVfb, pdst, ImVfbQIndex16( sVfb, psrc ) );
			if ( fieldMask & IMVFBRGB )
			{
				ImVfbSRed(   dstVfb, pdst, ImVfbQRed( sVfb, psrc ) );
				ImVfbSGreen( dstVfb, pdst, ImVfbQGreen( sVfb, psrc ) );
				ImVfbSBlue(  dstVfb, pdst, ImVfbQBlue( sVfb, psrc ) );
			}
			if ( fieldMask & IMVFBALPHA )
				ImVfbSAlpha( dstVfb, pdst, ImVfbQAlpha( sVfb, psrc ) );
			if ( fieldMask & IMVFBIDATA )
				ImVfbSIData( dstVfb, pdst, ImVfbQIData( sVfb, psrc ) );
			if ( fieldMask & IMVFBFDATA )
				ImVfbSFData( dstVfb, pdst, ImVfbQFData( sVfb, psrc ) );
			if ( fieldMask & IMVFBZ )
				ImVfbSZ( dstVfb, pdst, ImVfbQZ( sVfb, psrc ) );
			if ( fieldMask & IMVFBWPROT )
				ImVfbSWProt( dstVfb, pdst, ImVfbQWProt( sVfb, psrc ) );
			ImVfbSInc( sVfb, psrc );
			ImVfbSInc( dstVfb, pdst );
		}
	}

	if ( sourceVfb != sVfb )
		ImVfbFree( sVfb );
	return ( dstVfb );
}





/*
 * FUNCTION
 *	ImVfbFill	- fill a subarea within a virtual frame buffer.
 *
 * DESCRIPTION
 *	The source is copied to the destination.  The rectangular region
 *	in the image is scanned and each pixel's fillField filled with a
 *	new pixel value calculated as a ramp in the 'graduate' direction
 *	between the fill start and end values.
 *
 *	The majority of the code here is just to check that all the incomming
 *	arguments make sense!
 */
ImVfb *			/* Returns filled destination vfb 	*/
#ifdef __STDC__
ImVfbFill(ImVfb* srcVfb, int srcXLeft, int srcYTop, int srcDx, int srcDy,
	int fillField, double fillStart, double fillEnd, int inOut, int graduate, ImVfb* dstVfb )
#else
ImVfbFill( srcVfb, srcXLeft, srcYTop, srcDx, srcDy,
	fillField, fillStart, fillEnd, inOut, graduate, dstVfb )

	ImVfb   *srcVfb;	/* Source vfb				*/
	int	 srcXLeft;	/* upper lefthand corner of area	*/
	int	 srcYTop;
	int	 srcDx;		/* Width of area			*/
	int	 srcDy;		/* Height of area			*/
	int	 fillField;	/* Mask for the incoming data		*/
	double	 fillStart;	/* Value to start replacing in fieldmask*/
	double    fillEnd;	/* Value to end replacing in fieldmask  */
	int	 inOut;		/* whether to fill inside or outside area*/
	int	 graduate;	/* which direction to graduate in	*/
	ImVfb	*dstVfb;	/* Destination vfb			*/
#endif
{
	int	 i;		/* Temp value holder			*/
	int	 fields;	/* Image fields				*/
	int	 sxl, sxr, syb, syt;	/* Fill region			*/
	int	 x, y;		/* fill area width and height		*/
	int	 w, h;		/* Image width and height		*/
	float	 xInc, yInc;	/* X and Y direction grad increments	*/


	/*
	 *  Clamp location and size values to the boundaries of the image.
	 */
	if ( srcVfb == IMVFBNULL )
	{
		ImErrNo = IMENOVFB;
		return ( IMVFBNULL );
	}
	w   = ImVfbQWidth( srcVfb );
	sxl = srcXLeft;
	if ( sxl < 0 )			sxl = 0;
	if ( sxl >= w )			sxl = w - 1;

	sxr = srcXLeft + srcDx - 1;
	if ( sxr < 0 )			sxr = 0;
	if ( sxr >= w )			sxr = w - 1;

	h   = ImVfbQHeight( srcVfb );
	syt = srcYTop;
	if ( syt < 0 )			syt = 0;
	if ( syt >= h )			syt = h - 1;

	syb = srcYTop + srcDy - 1;
	if ( syb < 0 )			syb = 0;
	if ( syb >= h )			syb = h - 1;


	/*
	 *  Swap left & right, top & bottom, if necessary.
	 */
	if ( sxl > sxr )
	{
		i = sxl;	sxl = sxr;	sxr = i;
	}
	if ( syt > syb )
	{
		i = syb;	syb = syt;	syt = i;
	}


	/*
	 *  Confirm widths are legal.
	 */
	x = sxr - sxl + 1;
	if ( x <= 0 )
	{
		ImErrNo = IMEWIDTH;
		return( IMVFBNULL );
	}
	y = syb - syt + 1;
	if ( y <= 0 )
	{
		ImErrNo = IMEHEIGHT;
		return( IMVFBNULL );
	}


	/*
	 *  Figure out what fields we require be in the source & destination
	 *  and make sure they are there.
	 */
	fields = 0;
	if ( fillField & (IMRED | IMGREEN | IMBLUE | IMHUE | IMSATURATION | IMINTENSITY) )
		fields |= IMVFBRGB;
	if ( fillField & IMMONO )
		fields |= IMVFBMONO;
	if ( fillField & IMINDEX8 )
		fields |= IMVFBINDEX8;
	if ( fillField & IMINDEX16 )
		fields |= IMVFBINDEX16;
	if ( fillField & IMALPHA )
		fields |= IMVFBALPHA;
	if ( fillField & IMWPROT )
		fields |= IMVFBWPROT;
	if ( fillField & IMZ )
		fields |= IMVFBZ;
	if ( fillField & IMIDATA )
		fields |= IMVFBIDATA;
	if ( fillField & IMFDATA )
		fields |= IMVFBFDATA;
	if ( (fields & ImVfbQFields( srcVfb )) != fields )
	{
		ImErrNo = IMEFIELD;
		return ( IMVFBNULL );
	}


	/*
	 *  Make sure the fill range is good.
	 */
	switch ( fillField )
	{
	case IMMONO:
		if (      fillStart < 0.0 )	fillStart = 0.0;
		else if ( fillStart > 1.0 )	fillStart = 1.0;
		if (      fillEnd   < 0.0 )	fillEnd   = 0.0;
		else if ( fillEnd   > 1.0 )	fillEnd   = 1.0;
		break;

	case IMRED:
	case IMGREEN:
	case IMBLUE:
	case IMALPHA:
	case IMINDEX8:
	case IMWPROT:
		if (      fillStart < 0.0 )	fillStart = 0.0;
		else if ( fillStart > 255.0 )	fillStart = 255.0;
		if (      fillEnd   < 0.0 )	fillEnd   = 0.0;
		else if ( fillEnd   > 255.0 )	fillEnd   = 255.0;
		break;

	case IMINDEX16:
		if (      fillStart < 0.0 )	fillStart = 0.0;
		else if ( fillStart > 65535.0 )	fillStart = 65535.0;
		if (      fillEnd   < 0.0 )	fillEnd   = 0.0;
		else if ( fillEnd   > 65535.0 )	fillEnd   = 65535.0;
		break;

	case IMHUE:
		if (      fillStart < 0.0 )	fillStart = 0.0;
		else if ( fillStart > 360.0 )	fillStart = 360.0;
		if (      fillEnd   < 0.0 )	fillEnd   = 0.0;
		else if ( fillEnd   > 360.0 )	fillEnd   = 360.0;
		break;

	case IMSATURATION:
	case IMINTENSITY:
		if (      fillStart < 0.0 )	fillStart = 0.0;
		else if ( fillStart > 1.0 )	fillStart = 1.0;
		if (      fillEnd   < 0.0 )	fillEnd   = 0.0;
		else if ( fillEnd   > 1.0 )	fillEnd   = 1.0;
		break;

	case IMZ:
		if ( fillStart < 0.0 )		fillStart = 0.0;
		if ( fillEnd   < 0.0 )		fillEnd   = 0.0;
		break;

	case IMIDATA:
	case IMFDATA:
		/* All values allowed.					*/
		break;

	default:
		ImErrNo = IMEFIELD;
		return ( IMVFBNULL );
	}


	/*
	 *  Determine the pixel-to-pixel value increment.  Could be negative.
	 * 
	 *  The increment is computed as follows:
	 *  If the first pixel is color s, and the last is color e,
	 *  and there are n pixels, and the increment is i, then
	 *  we have s + (n-1)i = e.  Or i = (e-s)/(n-1).
	 */
	switch ( graduate )
	{
	case IMGRADHORIZ:	/* Horizontal graduation.		*/
		yInc = 0.0;
		xInc = fillEnd - fillStart;
		switch ( inOut )
		{
		case IMVFBINSIDE:	if (x!=1) xInc /= (x-1);	break;
		case IMVFBOUTSIDE:	if (w!=1) xInc /= (w-1);	break;
		default:
			ImErrNo = IMEBADINOUT;
			return ( IMVFBNULL );
		}
		break;

	case IMGRADVERT:	/* Vertical graduation.			*/
		xInc = 0.0;
		yInc = fillEnd - fillStart;
		switch ( inOut )
		{
		case IMVFBINSIDE:	if (y!=1) yInc /= (y-1);	break;
		case IMVFBOUTSIDE:	if (h!=1) yInc /= (h-1);	break;
		default:
			ImErrNo = IMEBADINOUT;
			return ( IMVFBNULL );
		}
		break;

	case IMGRADNONE:	/* No graduation.			*/
		xInc = yInc = 0.0;
		switch ( inOut )
		{
		case IMVFBINSIDE:
		case IMVFBOUTSIDE:
			break;
		default:
			ImErrNo = IMEBADINOUT;
			return ( IMVFBNULL );
		}
		break;

	default:
		ImErrNo = IMEGRADUATION;
		return( IMVFBNULL );
	}


	/*
	 *  Make a destination VFB, if needed.
	 */
	if ( srcVfb != dstVfb )
	{
		dstVfb = ImVfbCopy( srcVfb, 0, 0, w, h, IMVFBALL, dstVfb,0,0);
		if ( dstVfb == IMVFBNULL )
			return ( IMVFBNULL );	/* ImErrNo already set	*/
	}


	/*
	 *  Fill the image.
	 */
	if ( inOut == IMVFBINSIDE )
	{
		imVfbFillRect( dstVfb, sxl, sxr, syt, syb,
			xInc, yInc, fillStart, fillField );
	}
	else
	{
		/*
		 *  Fill 4 rectangles on the 4 sides of the area to be left
		 *  unfilled.
		 */
		switch ( graduate )
		{
		case IMGRADHORIZ:
			imVfbFillRect( dstVfb, 0, w-1, 0, syt,
				xInc, yInc, fillStart, fillField );
			imVfbFillRect( dstVfb, 0, sxl, syt, syb,
				xInc, yInc, fillStart, fillField );
			imVfbFillRect( dstVfb, 0, w-1, syb, h-1,
				xInc, yInc, fillStart, fillField );
			imVfbFillRect( dstVfb, sxr, w-1, syt, syb,
				xInc, yInc, fillStart + sxr*xInc, fillField );
			break;
		case IMGRADVERT:
			imVfbFillRect( dstVfb, 0, sxl, 0, h-1,
				xInc, yInc, fillStart, fillField );
			imVfbFillRect( dstVfb, sxl, sxr, 0, syt,
				xInc, yInc, fillStart, fillField );
			imVfbFillRect( dstVfb, sxr, w-1, 0, h-1,
				xInc, yInc, fillStart, fillField );
			imVfbFillRect( dstVfb, sxl, sxr, syb, h-1,
				xInc, yInc, fillStart + syb*yInc, fillField );
			break;
		case IMGRADNONE:
			imVfbFillRect( dstVfb, 0, w-1, 0, syt,
				xInc, yInc, fillStart, fillField );
			imVfbFillRect( dstVfb, 0, sxl, syt, syb,
				xInc, yInc, fillStart, fillField );
			imVfbFillRect( dstVfb, 0, w-1, syb, h-1,
				xInc, yInc, fillStart, fillField );
			imVfbFillRect( dstVfb, sxr, w-1, syt, syb,
				xInc, yInc, fillStart, fillField );
			break;
		}
	}

	return ( dstVfb );
}





/*
 *  FUNCTION
 *	imVfbFillRect	-  fill a rectangle in a source image
 *
 *  DESCRIPTION
 *	The rectangular region is looped through and each pixel's fillField
 *	changed to a fill value calculated based on a starting value and
 *	X and Y direction increments.
 */
static void				/* Returns nothing		*/
#ifdef __STDC__
imVfbFillRect( ImVfb* dstVfb, int sxl, int sxr, int syt, int syb, double xInc, double yInc, double fillStart, int fillField )
#else
imVfbFillRect( dstVfb, sxl, sxr, syt, syb, xInc, yInc, fillStart, fillField )
	ImVfb	*dstVfb;		/* Destination VFB		*/
	int      sxl, sxr, syt, syb;	/* Rectangle to fill		*/
	double	 xInc, yInc;		/* Directional pixel increments	*/
	double	 fillStart;		/* Starting fill value		*/
	int	 fillField;		/* Field of VFB to fill		*/
#endif
{
	ImVfbPtr pDst;			/* VFB pixel pointer		*/
	int	 i,j;			/* counters			*/
	float	 pixel;			/* New pixel value		*/
	float    hsi[3];		/* HSI pixel value		*/
	int      rgb[3];		/* RGB pixel value		*/


	for ( i = syt; i <= syb; i++ )
	{
		pDst  = ImVfbQPtr( dstVfb, sxl, i );
		pixel = fillStart + (i-syt) * yInc;
		switch ( fillField )
		{
		case IMRED:
			for ( j = sxl; j <= sxr; j++ )
			{
				ImVfbSRed( dstVfb, pDst, (int)pixel );
				ImVfbSInc( dstVfb, pDst );
				pixel += xInc;
			}
			break;
		case IMGREEN:
			for ( j = sxl; j <= sxr; j++ )
			{
				ImVfbSGreen( dstVfb, pDst, (int)pixel );
				ImVfbSInc( dstVfb, pDst );
				pixel += xInc;
			}
			break;
		case IMBLUE:
			for ( j = sxl; j <= sxr; j++ )
			{
				ImVfbSBlue( dstVfb, pDst, (int)pixel );
				ImVfbSInc( dstVfb, pDst );
				pixel += xInc;
			}
			break;
		case IMINDEX8:
			for ( j = sxl; j <= sxr; j++ )
			{
				ImVfbSIndex8( dstVfb, pDst, (int)pixel );
				ImVfbSInc( dstVfb, pDst );
				pixel += xInc;
			}
			break;
		case IMINDEX16:
			for ( j = sxl; j <= sxr; j++ )
			{
				ImVfbSIndex16( dstVfb, pDst, (int)pixel );
				ImVfbSInc( dstVfb, pDst );
				pixel += xInc;
			}
			break;
		case IMMONO:
			for ( j = sxl; j <= sxr; j++ )
			{
				ImVfbSMono( dstVfb, pDst, (int)pixel );
				ImVfbSInc( dstVfb, pDst );
				pixel += xInc;
			}
			break;
		case IMHUE:
		case IMSATURATION:
		case IMINTENSITY:
			for ( j = sxl; j <= sxr; j++ )
			{

				rgb[0] = ImVfbQRed( dstVfb, pDst );
				rgb[1] = ImVfbQGreen( dstVfb, pDst );
				rgb[2] = ImVfbQBlue( dstVfb, pDst );
				ImRgbToHsi( rgb, hsi );
				switch ( fillField )
				{
				case IMHUE:		hsi[0] = pixel; break;
				case IMSATURATION:	hsi[1] = pixel; break;
				case IMINTENSITY:	hsi[2] = pixel; break;
				}
				ImHsiToRgb( hsi, rgb );
				ImVfbSRed( dstVfb, pDst,   rgb[0] );
				ImVfbSGreen( dstVfb, pDst, rgb[1] );
				ImVfbSBlue( dstVfb, pDst,  rgb[2] );
				ImVfbSInc( dstVfb, pDst );
				pixel += xInc;
			}
			break;
		case IMALPHA:
			for ( j = sxl; j <= sxr; j++ )
			{
				ImVfbSAlpha( dstVfb, pDst, (int)pixel );
				ImVfbSInc( dstVfb, pDst );
				pixel += xInc;
			}
			break;
		case IMWPROT:
			for ( j = sxl; j <= sxr; j++ )
			{
				ImVfbSWProt( dstVfb, pDst, (int)pixel );
				ImVfbSInc( dstVfb, pDst );
				pixel += xInc;
			}
			break;
		case IMZ:
			for ( j = sxl; j <= sxr; j++ )
			{
				ImVfbSZ( dstVfb, pDst, (int)pixel );
				ImVfbSInc( dstVfb, pDst );
				pixel += xInc;
			}
			break;
		case IMFDATA:
			for ( j = sxl; j <= sxr; j++ )
			{
				ImVfbSFData( dstVfb, pDst, pixel );
				ImVfbSInc( dstVfb, pDst );
				pixel += xInc;
			}
			break;
		case IMIDATA:
			for ( j = sxl; j <= sxr; j++ )
			{
				ImVfbSIData( dstVfb, pDst, (int)pixel );
				ImVfbSInc( dstVfb, pDst );
				pixel += xInc;
			}
			break;
		}
	}
}


/*
 * FUNCTION
 *   ImVfbPrint
 *
 * DESCRIPTION
 *   Print the pixels of a vfb, as floats or integers, as
 *  specified by the 'format' parameter.  (This should be
 *  IMVFBPRINTFLOAT or IMVFBPRINTINT)
 *
 */

void /* returns nothing */
#ifdef __STDC__
ImVfbPrint(FILE* fp, int format, ImVfb* vfb)
#else
ImVfbPrint(fp, format, vfb)
FILE* fp;	/* where to direct the output */
int format;	/* the format of the numbers  */
ImVfb* vfb;	/* the vfb                    */
#endif
{
 int fields; 	/* The fields in the vfb. */
 int i;         /* loop index             */
 ImVfbPtr vfbptr; /* Points into the vfb  */

 /* Determine the fields in the vfb. */
 fields = 0;
 fields = ImVfbQFields(vfb);
 
 vfbptr = ImVfbQPtr(vfb, 0, 0);
 for (i=0; i < ImVfbQWidth(vfb) * ImVfbQHeight(vfb); i++)
 {
	  /* Print this pixel */

	 if (format & IMVFBPRINTFLOAT)
	 {
	 	if (fields & IMVFBRGB)
	 	{
			fprintf( fp, "%f %f %f ",
			(float)(ImVfbQRed  ( vfb, vfbptr )) / 255.0,
			(float)(ImVfbQGreen( vfb, vfbptr )) / 255.0,
			(float)(ImVfbQBlue ( vfb, vfbptr )) / 255.0 );
	 	}
		if (fields & IMVFBALPHA)
		{
			fprintf( fp, "%f ",
			(float)(ImVfbQAlpha (vfb, vfbptr )) / 255.0 );
		}
		if (fields & IMVFBINDEX8)
		{
			fprintf( fp, "%f ",
			(float)(ImVfbQIndex( vfb, vfbptr )) / 255.0 );
		}
		if (fields & IMVFBINDEX16)
		{
			fprintf( fp, "%f ",
			(float)(ImVfbQIndex16( vfb, vfbptr )) / 65535.0 );
		}
		fprintf( fp, "        ");
	 }
	 if (format & IMVFBPRINTINT)
	 {
	 	if (fields & IMVFBRGB)
	 	{
			fprintf( fp, "%d %d %d ",
			ImVfbQRed  ( vfb, vfbptr ),
			ImVfbQGreen( vfb, vfbptr ),
			ImVfbQBlue ( vfb, vfbptr ));
	 	}
		if (fields & IMVFBALPHA)
		{
			fprintf( fp, "%d ",
			ImVfbQAlpha (vfb, vfbptr ) );
		}
		if (fields & IMVFBINDEX8)
		{
			fprintf( fp, "%d ",
			ImVfbQIndex( vfb, vfbptr ) );
		}
		if (fields & IMVFBINDEX16)
		{
			fprintf( fp, "%ld ",
			ImVfbQIndex16( vfb, vfbptr ) );
		}
	 }
	 fprintf(fp,"\n");
	 vfbptr = ImVfbQNext(vfb, vfbptr);
 }   /* End of pixel loop */
}


/*
 *  FUNCTION
 *	ImGetTransparency
 *
 *  DESCRIPTION
 *	The transparency value of an image is an index in the color
 * lookup table.  Pixels with this index are deemed transparent.
 * While RGB images use alpha channels for transparency, indexed 
 * images sometimes use transparency values for transparency.
 *
 * This routine looks in the flags table for a request,
 * then looks in the tagTable for a value, and returns the
 * transparency value of the image based on these two checks.
 *
 * The vfb passed to this routine must be of type INDEX8
 * If there is a transparency value in the tagTable, or a
 * request in the flagsTable, this routine figures out that
 * value, and returns it.
 *
 *	An "image transparency request" in the flags table may have one
 *	of the following values:
 *
 *	"most common"	<- means we choose the most common value in the image
 *	"index=nn"	<- means pixels with nn should be transparent
 *	"rgb=rr gg bb"  <- means pixels with rr gg bb should be tranparent
 *
 * If there is no transparency request in either table, we return -1.
 *
 */
int /* returns an index value or -1 */
#ifdef __STDC__
ImGetTransparency(TagTable* tagTable, TagTable* flagsTable, ImVfb* vfb)
#else
ImGetTransparency(tagTable, flagsTable, vfb)
TagTable* tagTable;
TagTable* flagsTable;
ImVfb* vfb;
#endif
{
	TagEntry* tagEntry;	/* entry 			*/
	char* request;		/* the request 			*/
	int transInt;		/* transparency value as an integer */
	int redInt=-1, 
	greenInt=0, blueInt=0; 	/* rgb requested values 	*/
	ImClt* clt;		/* image clt 			*/
	ImCltPtr cltPtr;	/* points into the clt 		*/
	ImHistTable* 	histogram=NULL;	/* a histogram 		*/
	long int largestWeight; /* biggest weight        	*/
	long int thisWeight;    /* one weight            	*/
	int 	field;		/* mono or index8 		*/
	char    message[200];   /* message buffer        	*/
	int transColor=-1;	/* The value to return   	*/
	int numPixels;		/* # of pix w/ this index value */
	int i;


	/*
	 * First look for a request in the flags table
	 */
	if (flagsTable!=TAGTABLENULL)
		tagEntry = TagTableQDirect(flagsTable, "image transparency request", 0);
	else
		tagEntry=TAGENTRYNULL;

	/*
	 * then check the tagTable
	 */
	if (tagEntry==TAGENTRYNULL)
	{
		tagEntry = TagTableQDirect(tagTable, "transparency value", 0);
	}

	if (tagEntry==TAGENTRYNULL)
	{	/* No transparency! */
		return -1;
	}


	/*
	 * There is transparency.
	 * What should be do with it?
	 */

	TagEntryQValue( tagEntry, &request);

	/*
	 * The request can be one of the following:
	 *	"most common"	<- means we choose the most common value in the image
	 *	"index=nn"	<- means pixels with nn should be transparent
	 *	"rgb=rr gg bb"  <- means pixels with rr gg bb should be tranparent
	 */

	if (strncmp(request,"index",5)==0)
	{
		sscanf(request,"index=%d",&transInt);
		transColor = (unsigned char)transInt;
	}
	else if (strncmp(request,"rgb",3)==0)
	{
		/* Find the first entry with this rgb value */
		sscanf(request,"rgb=%d %d %d",&redInt, &greenInt, &blueInt);
		clt = ImVfbQClt(vfb);
		transInt = -1;
		cltPtr = ImCltQFirst(clt);
		for (i=0; i< ImCltQNColors(clt); i++)
		{
			if ( (redInt == ImCltQRed(cltPtr)) &&
			     (greenInt==ImCltQGreen(cltPtr)) &&
			     (blueInt ==ImCltQBlue (cltPtr)))
			     {
				transInt = i;
				i = ImCltQNColors(clt)+1;
			     }
			cltPtr = ImCltQNext (clt, cltPtr );
		}

		if (transInt==-1)
		{
			sprintf(message,"Couldn't find any %d, %d, %d pixels in the image!",
				redInt, greenInt, blueInt);
			ImErrorWarning(message, IMEVALOUTOFRANGE, -1);
			return 1;
		}
		transColor = (unsigned char) transInt;
	}
	else if (strcmp(request,"most common")==0)
	{
		if (ImVfbQFields(vfb) & IMVFBINDEX8)
			field = IMINDEX8; 
		else
			field = IMMONO;
		histogram = ImVfbHist( vfb, field, 0 );

		/*
		 * Step through the histogram.  Get the most frequent color.
		 */
		largestWeight = (long) 0;
		transColor    = 1;
		for (i=0; i< histogram->imhist_nEntries; i++)
		{
			numPixels = histogram->imhist_nOccur[i];
			thisWeight = numPixels;
			if (thisWeight > largestWeight)
			{
				largestWeight = thisWeight;
				if (field==IMINDEX8)
					transColor = histogram->imhist_index8[i];
				else /* mono */
					transColor = histogram->imhist_mono[i];
			}
		}
		ImVfbHistTableFree(histogram);
	}
	else
	{
		sprintf(message,"Unknown transparency request: '%s'",
			request);
		ImErrorWarning(message, IMEVALOUTOFRANGE, -1);
		return -1;
	}

	return transColor;
}



