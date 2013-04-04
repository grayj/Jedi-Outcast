/**
 **	$Header: /roq/libim/imclt.c 1     11/02/99 4:38p Zaphod $
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

#define HEADER	"    $Header: /roq/libim/imclt.c 1     11/02/99 4:38p Zaphod $"

/**
 **  FILE
 **	imclt.c		-  general CLT functions
 **
 **  PROJECT
 **	libim	-  SDSC image manipulation library
 **
 **  DESCRIPTION
 **	imclt.c contains functions for generating and manipulating CLT's.
 **
 **  PUBLIC CONTENTS
 **			d =defined constant
 **			f =function
 **			m =defined macro
 **			t =typedef/struct/union
 **			v =variable
 **			? =other
 **
 **	ImCltAlloc	f  allocate a CLT
 **	ImCltFree	f  deallocate a CLT
 **
 **	ImCltDup	f  duplicate a CLT
 **	ImCltGrayRamp	f  fill a CLT with a grayscale ramp
 **	ImCltRoll	f  roll a cltby x amount
 **	ImCltAdjust	f  adjust the elements in a clt
 **
 **  PRIVATE CONTENTS
 **	none
 **
 **  HISTORY
 **	$Log: /roq/libim/imclt.c $
 * 
 * 1     11/02/99 4:38p Zaphod
 **	Revision 1.8  1995/06/29  00:28:04  bduggan
 **	updated copyright year
 **
 **	Revision 1.7  1995/06/16  08:36:29  bduggan
 **	changed bcopy to memcpy.
 **	Allowed for index16 vfb's in ImCltGrayRamp
 **
 **	Revision 1.6  94/10/03  11:29:32  nadeau
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
 **	Revision 1.5  92/12/03  01:47:22  nadeau
 **	Cleaned up ImCltRoll.
 **	
 **	Revision 1.4  92/09/01  12:42:12  vle
 **	Updated ImCltGrayRamp to include a srcClt and handle
 **	lots of variations of input arguments.
 **	
 **	Revision 1.3  92/08/07  11:56:33  groening
 **	added imcltroll
 **	
 **	Revision 1.2  91/10/03  08:48:59  nadeau
 **	Minor cosmetic changes.  No bug fixing.
 **	
 **	Revision 1.1  91/03/08  14:26:27  nadeau
 **	Initial revision
 **	
 **/

#include "iminternal.h"

/**
 **  CODE CREDITS
 **	Custom development, Mike Bailey, San Diego Supercomputer Center, 1991.
 **	Custom development, Dave Nadeau, San Diego Supercomputer Center, 1991.
 **/





/*
 *  FUNCTION
 *	ImCltAlloc	-  allocate a CLT
 *
 *  DESCRIPTION
 *	Space for the desired length of CLT is allocated and a new CLT
 *	structure set up and returned.
 */

ImClt *				/* Returns a CLT			*/
#ifdef __STDC__
ImCltAlloc( int ncolors )
#else
ImCltAlloc( ncolors )
	int ncolors;		/* # entries in this ImClt		*/
#endif
{
	ImClt   *c;		/* the ImClt being allocated		*/
	ImCltPtr ct;		/* the actual color table		*/


	c = (ImClt *) malloc( sizeof( ImClt ) );
	if ( c == NULL )
	{
		ImErrNo = IMEMALLOC;
		return( IMCLTNULL );
	}
	c->clt_ncolors = ncolors;


	ct = (ImCltPtr) malloc( ncolors*3 );
	if ( ct == NULL )
	{
		free( (char *)c );
		ImErrNo = IMEMALLOC;
		return( IMCLTNULL );
	}

	c->clt_clt = ct;

	return( c );
}





/*
 *  FUNCTION
 *	ImCltFree	-  deallocate a CLT
 *
 *  DESCRIPTION
 *	The CLT's space, and then the CLT structure itself are freed.
 */

void				/* Returns nothing			*/
#ifdef __STDC__
ImCltFree( ImClt *c )
#else
ImCltFree( c )
	ImClt *c;		/* the CLT being freed			*/
#endif
{
	if ( c != IMCLTNULL )
	{
		free( (char *) c->clt_clt );
		free( (char *) c );
	}
}





/*
 *  FUNCTION
 *	ImCltDup	-  duplicate a CLT
 *
 *  DESCRIPTION
 *	A new CLT is allocated that has the same length as the source
 *	CLT.  The source CLT is copied to the new CLT.
 */

ImClt *					/* Returns new CLT		*/
#ifdef __STDC__
ImCltDup( ImClt *srcClt )
#else
ImCltDup( srcClt )
	ImClt *srcClt;			/* CLT to be duplicated		*/
#endif
{
	ImClt *newClt;			/* New CLT			*/

	newClt = ImCltAlloc( ImCltQNColors( srcClt ) );
	if( newClt == 0 )
	{
		ImErrNo = IMEMALLOC;
		return( IMCLTNULL );
	}

	memcpy( (void *)ImCltQFirst( newClt ), (void *)ImCltQFirst( srcClt ),
		3*ImCltQNColors( srcClt )  );

	return ( newClt );
}





/*
 *  FUNCTION
 *	ImCltGrayRamp	-  fill a CLT with a grayscale ramp
 *
 *  DESCRIPTION
 *	If the destination CLT is IMCLTNEW, a new CLT is allocated.
 *	In any case, the CLT is walked for the selected number of entries,
 *	starting at the selected location, and grayscale values generated
 *	from the given low to the given max.  The gray CLT is returned.
 */

ImClt *					/* Returns grayscale CLT	*/
#ifdef __STDC__
ImCltGrayRamp( ImClt *srcClt, int startIndex, int endIndex, int startGray,
	int endGray, ImClt *dstClt )
#else
ImCltGrayRamp( srcClt, startIndex, endIndex, startGray, endGray, dstClt )
	ImClt *srcClt;			/* CLT to fill			*/
	int    startIndex, endIndex;	/* Range of CLT to fill		*/
	int    startGray, endGray;	/* Range of gray to fill with	*/
	ImClt *dstClt;			/* CLT to return		*/
#endif
{
	int i;				/* Counter			*/
	float delta;			/* Increment value for grays	*/
	float gray;			/* Current gray value		*/
	ImCltPtr pClt;			/* Pointer to CLT entry		*/
	ImCltPtr dptr;			/* Pointer to dst CLT entry	*/
	ImCltPtr sptr;			/* Pointer to src CLT entry	*/
	int maxindex;			/* Maximum index in CLT		*/
	int numcopy;			/* Number of CLT entries to copy*/

	if( startIndex < 0 || endIndex < 0 )
	{
		/* startIndex or endIndex too small */
		ImErrNo = IMEBADINOUT;
		return( IMCLTNULL );
	}

	if( startGray < 0 || endGray < 0 || startGray > 65535 || endGray > 65535 )
	{
		/* startGray or endGray out of valid range */
		ImErrNo = IMEBADINOUT;
		return( IMCLTNULL );
	}

	if( srcClt == IMCLTNULL )
	{
		if( dstClt == IMCLTNEW )
		{
			if ( startIndex > endIndex )
				dstClt = ImCltAlloc( startIndex + 1 );
			else
				dstClt = ImCltAlloc( endIndex + 1 );
			if ( dstClt == IMCLTNULL )
			{
				ImErrNo = IMEMALLOC;
				return ( IMCLTNULL );
			}
		}
		else if( dstClt == IMCLTNULL )
		{
			ImErrNo = IMENOCLT;
			return( IMCLTNULL );
		}
		else	/* dstClt is a valid pointer */
		{
			maxindex = ImCltQNColors( dstClt ) - 1;
			if( ( maxindex < startIndex ) ||
			    ( maxindex < endIndex ) )
			{
				/* Index out of range */
				ImErrNo = IMEBADINOUT;
				return( IMCLTNULL );
			}
		}
	}
	else if( srcClt == IMCLTNEW )
	{
		if( dstClt == IMCLTNEW )
		{
			if ( startIndex > endIndex )
				dstClt = ImCltAlloc( startIndex + 1 );
			else
				dstClt = ImCltAlloc( endIndex + 1 );
			if ( dstClt == IMCLTNULL )
			{
				ImErrNo = IMEMALLOC;
				return ( IMCLTNULL );
			}
		}
		else if( dstClt == IMCLTNULL )
		{
			/* Error */
			ImErrNo = IMENOCLT;
			return( IMCLTNULL );
		}
		else	/* dstClt is a valid pointer */
		{
			maxindex = ImCltQNColors( dstClt ) - 1;
			if( ( maxindex < startIndex ) ||
			    ( maxindex < endIndex ) )
			{
				/* Index out of range */
				ImErrNo = IMEBADINOUT;
				return( IMCLTNULL );
			}
		}
	}
	else	/* srcClt is a valid pointer */
	{
		if( dstClt == IMCLTNEW )
		{
			maxindex = ImCltQNColors( srcClt ) - 1;
			if( ( maxindex < startIndex ) ||
			    ( maxindex < endIndex ) )
			{
				/* Index out of range */
				ImErrNo = IMEBADINOUT;
				return( IMCLTNULL );
			}

			dstClt = ImCltDup( srcClt );
			if ( dstClt == IMCLTNULL )
			{
				ImErrNo = IMEMALLOC;
				return ( IMCLTNULL );
			}
		}
		else if( dstClt == IMCLTNULL )
		{
			/* Error */
			ImErrNo = IMEBADINOUT;
			return( IMCLTNULL );
		}
		else	/* dstClt is a valid pointer */
		{
			if( srcClt != dstClt )
			{
				maxindex = ImCltQNColors( dstClt ) - 1;
				if( ( maxindex < startIndex ) ||
				    ( maxindex < endIndex ) )
				{
					/* Index out of range */
					ImErrNo = IMEBADINOUT;
					return( IMCLTNULL );
				}

				/*
				 * Determine how many color entries to
				 * copy
				 */
				if( ImCltQNColors( srcClt ) > 
				    ImCltQNColors( dstClt ) )
				{
					numcopy = ImCltQNColors( dstClt );
				}
				else
				{
					numcopy = ImCltQNColors( srcClt );
				}

				/*
				 * Copy the color entries
				 */
				sptr = ImCltQFirst( srcClt );
				dptr = ImCltQFirst( dstClt );

				for( i = 0; i < numcopy; ++i )
				{
					ImCltSRed( dptr, ImCltQRed( sptr ) );
					ImCltSGreen( dptr, ImCltQGreen(sptr) );
					ImCltSBlue( dptr, ImCltQBlue( sptr ) );

					sptr = ImCltQNext( srcClt, sptr );
					dptr = ImCltQNext( dstClt, dptr );
				}
			}
		}
	}

	if ( endIndex > startIndex )
	{
		delta  = endGray - startGray;
		delta /= (float)(endIndex - startIndex);

		gray = startGray;
		pClt = ImCltQPtr( dstClt, startIndex );
		for ( i = startIndex; i <= endIndex; i++, gray += delta )
		{
			ImCltSRed(   pClt, (int)gray );
			ImCltSGreen( pClt, (int)gray );
			ImCltSBlue(  pClt, (int)gray );
			ImCltSInc( dstClt, pClt );
		}
		return ( dstClt );
	}

	delta  = endGray - startGray;
	delta /= (float)(startIndex - endIndex);

	gray = startGray;
	pClt = ImCltQPtr( dstClt, startIndex );
	for ( i = startIndex; i >= endIndex; i--, gray += delta )
	{
		ImCltSRed(   pClt, (int)gray );
		ImCltSGreen( pClt, (int)gray );
		ImCltSBlue(  pClt, (int)gray );
		ImCltSInc( dstClt, pClt );
	}
	return ( dstClt );
}





/*
 *  FUNCTION
 *	ImCltRoll	-  roll a CLT
 *
 *  DESCRIPTION
 *	If no destination CLT is given, a new destination CLT is created.
 *
 *	The source CLT is then copied into the destination CLT, rolling
 *	its entries.
 */

ImClt *				/* Returns rolled CLT			*/
#ifdef __STDC__
ImCltRoll( ImClt *sourceClt, int nEntries, ImClt *dstClt )
#else
ImCltRoll( sourceClt, nEntries, dstClt )
	ImClt   *sourceClt;	/* CLT to roll				*/
	int      nEntries;	/* # of entries to roll			*/
	ImClt   *dstClt;	/* CLT to overwrite with rolled source	*/
#endif
{
	ImClt   *sClt;		/* CLT to overwrite with rolled source	*/
	int	 i, j;		/* counters				*/
	ImCltPtr psrc, pdst;	/* src, dst pixel pointers		*/
	int	 nColors;	/* number of colors in clt		*/


	/*
	 *  Get information on the source CLT and check for errors.
	 */
	if ( sourceClt == IMCLTNULL )
	{
		ImErrNo = IMENOCLT;
		return ( IMCLTNULL );
	}
	nColors = ImCltQNColors(sourceClt);

	nEntries %= nColors;
	if ( nEntries < 0 )
		nEntries += nColors;


	/*
	 *  Allocate a new VFB if necessary.  If the source and destination
	 *  CLT's are the same, we'll need a copy of the source.
	 */
	sClt = sourceClt;
	if (dstClt==IMCLTNEW)
	{
		dstClt = ImCltAlloc( nColors );
		if ( dstClt == IMCLTNULL )
		{
			ImErrNo = IMEMALLOC;
			return( IMCLTNULL );
		}
	}
	else if ( sourceClt == dstClt )
	{
		if ( (sClt = ImCltDup(sourceClt)) == IMCLTNULL )
			return ( IMCLTNULL );	/* ImErrNo already set	*/
	}
	else if ( nColors != ImCltQNColors( dstClt ) )
	{
		ImErrNo = IMECLTLENGTH;
		return ( IMCLTNULL );
	}


	/*
	 *  Roll it.
	 */
        psrc = ImCltQFirst( sClt );
	pdst = ImCltQPtr( dstClt, nEntries );

	for ( j = 0, i = nEntries; j < nColors; j++, i++ )
	{
		if ( i == nColors )
		{
			i = 0;
			pdst = ImCltQFirst( dstClt );
		}

		ImCltSRed( pdst, ImCltQRed( psrc ) );
		ImCltSGreen( pdst, ImCltQGreen( psrc ) );
		ImCltSBlue( pdst, ImCltQBlue( psrc ) );

		ImCltSInc( sClt, psrc );
		ImCltSInc( dstClt, pdst );
	}
	if ( sourceClt != sClt )
		ImCltFree( sClt );
	return ( dstClt );
}
