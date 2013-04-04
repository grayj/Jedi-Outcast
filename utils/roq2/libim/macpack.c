

/**

 **	$Header: /roq/libim/macpack.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/macpack.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	macpack.c	-  Apple Macintosh BitMap/PixMap packing/unpacking routines

 **

 **  PROJECT

 **	libimage	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	macpack.c contains routines to compress/uncompress (pack/unpack) Apple

 **	Macintosh BitMap/PixMap raster graphics for the image manipulation library.

 **

 **  PUBLIC CONTENTS

 **

 **	PackBits			Pack BitMap/PixMap raster.

 **	UnpackBits		Unpack BitMap/PixMap raster.

 **

 **	PackBits3		Pack BitMap/PixMap raster (16 bit).

 **	UnpackBits3		Unpack BitMap/PixMap raster (16 bit).

 **						(3 refers to the PixMap "packType" field value)

 **

 **  PRIVATE CONTENTS

 **	NULL			d  an empty pointer

 **

 **	HISTORY

 **	$Log: /roq/libim/macpack.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.10  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.9  1994/10/03  11:31:18  nadeau

 **	Updated to ANSI C and C++ compatibility.

 **	Removed all use of register keyword.

 **	Minimized use of custom SDSC types (e.g., uchar vs. unsigned char)

 **	Changed all float arguments to double.

 **	Added forward declarations.

 **	Added misc. casts to passify SGI and DEC compilers.

 **	Changed all macros and defined constants to have names

 **	starting with IM.

 **	Rearranged magic number structures for format handlers.

 **	Made format handler routines static (i.e., local to file).

 **	Updated comments, adding format descriptions and references.

 **	Updated indenting on some code.

 **	Updated copyright message.

 **

 **	Revision 1.8  93/10/21  15:17:53  moreland

 **	Added 16-bit word PackBits3 and UnpackBits3 in order to support

 **	the 16-bit direct color pixel spec of the PICT/Quickdraw formats.

 **	

 **	Revision 1.7  92/08/31  17:45:32  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.6  91/03/21  08:51:24  nadeau

 **	Fixed typo.

 **	

 **	Revision 1.5  91/03/15  13:58:34  nadeau

 **	Attempted optimization of UnpackBits.  Changed things a bit

 **	to make it through the loop unrolling problems of the

 **	Alliant FX2800 compiler.

 **	

 **	Revision 1.4  91/03/14  18:16:36  todd

 **	Tried to fool the fx2800 compiler into not

 **	unrolling two loops in UnpackBits.

 **	

 **	Revision 1.3  91/02/18  16:23:43  moreland

 **	fixed !

 **	

 **	Revision 1.2  91/01/29  11:13:24  todd

 **	add comments

 **

 **	Revision 1.1  90/10/10  09:20:41  moreland

 **	Initial revision

 **

 **

 **/



#include "iminternal.h"





/*

 *  FUNCTION

 *	PackBits - Use macintosh byte packing algorithm

 *

 *  DESCRIPTION

 *		PackBits compresses "width" bytes of data starting at "cIn" and

 *		stores the compressed data at "cOut". Bytes are compressed when there

 *		are three or more consecutive equal bytes.  After the data is

 *		compressed, the value of "width" is the number of compressed data

 *		bytes stored at "cOut".  In the worst case, the compressed data can

 *		be one byte longer than the original data.

 */



void

#ifdef __STDC__

PackBits( unsigned char *cIn, unsigned char *cOut, unsigned int *width )

#else

PackBits( cIn, cOut, width )

	unsigned char *cIn;

	unsigned char *cOut;

	unsigned int * width;

#endif

{

	int cnt;

	int len;

	unsigned int wid;



	wid = *width;

	len = 0;



	/* Loop thru the unencoded bytes */

	while ( wid > 0 ) 

	{

		if ( wid == 1 ) 

		{

			cOut[0] = 0;

			cOut[1] = cIn[0];

			len += 2;

			wid = 0;

		} 

		else if ( wid == 2 ) 

		{

			cOut[0] = 1;

			cOut[1] = cIn[0];

			cOut[2] = cIn[1];

			len += 3;

			wid = 0;

		} 

		else if ( wid == 3 ) 

		{

			if ( (cIn[0] == cIn[1]) && (cIn[1] == cIn[2]) ) 

			{

				cOut[0] = (256 - 3) + 1;

				cOut[1] = cIn[0];

				len += 2;

				wid = 0;

			} 

			else 

			{

				cOut[0] = 2;

				cOut[1] = cIn[0];

				cOut[2] = cIn[1];

				cOut[3] = cIn[2];

				len += 4;

				wid = 0;

			}

		} 

		else if ( (cIn[0]==cIn[1]) &&  (cIn[1]==cIn[2]) ) 

		{

			/* PACKED RUN */

			/* We have >= 3 equal bytes */

			cnt=3;

			while ( (cnt<wid) && (cnt<127) && (cIn[0]==cIn[cnt]) ) 

			{

				cnt++;

			}

			cOut[0] = (256 - cnt) + 1;        /* count byte: Two's comp */

			cOut[1] = cIn[0];                      /* The repeated byte */

			cOut += 2;                 /* We have "output" 2 more bytes */

			cIn += cnt;        /* Advance unpacked ptr: countbyte & cnt */

			len += 2;       /* We have added 2 more bytes to packed ptr */

			wid -= cnt;                /* We looked at # unpacked bytes */

		} 

		else 

		{

			/* UNPACKED/LITERAL RUN */

			/* We have >= 3 unequal bytes */

			cnt=0;

			while ( (cnt<wid) && (cnt<127)

				&& ! ( (cIn[cnt+0]==cIn[cnt+1])

				    && (cIn[cnt+1]==cIn[cnt+2]) )    ) 

			{

				cOut[cnt+1] = cIn[cnt];

				cnt++;

			}

			cOut[0] = cnt - 1;                            /* count byte */

			cOut += (cnt + 1);  /* Advance packed ptr: countbyte & byte */

			cIn += cnt;                    /* Advance unpacked ptr: cnt */

			len += (cnt + 1);      /* We added more bytes to packed ptr */

			wid -= cnt;    /* We have looked at cnt more unpacked bytes */

		}



	}   /* End of while wid > 0*/



	*width = len;

}







/*

 *  FUNCTION

 *	UnpackBits - Use macintosh byte unpacking algorithm

 *

 *  DESCRIPTION

 *  	Given a pointer in srcPtr to data that was compressed by PackBits,

 *  	UnpackBits expands the data and stores the result at dstPtr.  cnt is

 *  	passed IN as the number of compressed bytes to unpack.  cnt, when

 *  	passed back OUT, is the number of uncompressed bytes.

 */





#ifndef new

void

#ifdef __STDC__

UnpackBits( unsigned char *srcPtr, unsigned char *dstPtr, unsigned int *cnt )

#else

UnpackBits( srcPtr, dstPtr, cnt )

	unsigned char * srcPtr;

	unsigned char * dstPtr;

	unsigned int * cnt;

#endif /* STDC */

{

	int i=0, p=0, u=0, run=0;

	unsigned int packed = *cnt;

#ifndef older

	unsigned char *pDst, *pSrc;

#endif



	while ( p < packed ) 

	{

		if ( srcPtr[p] >= 128 ) 

		{	/* PACKED RUN */

			run = 256 - srcPtr[p] + 1;

#ifdef older

			for ( i=0; i<run; i++ )  dstPtr[u+i] = srcPtr[p+1];

#else

			pSrc = &srcPtr[p+1];

			pDst = &dstPtr[u];

			for ( i = 0; i < run; i++ )

				*pDst++ = *pSrc;

#endif

			p += 2;

			u += run;

		} 

		else 

		{				/* NONPACKED RUN */

			run = srcPtr[p] + 1;

#ifdef older

			for ( i=0; i<run; i++ )  dstPtr[u+i] = srcPtr[p+i+1];

#else

			pSrc = &srcPtr[p+1];

			pDst = &dstPtr[u];

			for ( i = 0; i < run; i++ )

				*pDst++ = *pSrc++;

#endif

			p += run+1;

			u += run;

		}

	}



	*cnt = u;

}



#else

void

#ifdef __STDC__

UnpackBits( unsigned char *srcPtr, unsigned char *dstPtr, unsigned int *cnt )

#else

UnpackBits( srcPtr, dstPtr, cnt )

	unsigned char *srcPtr;

	unsigned char *dstPtr;

	unsigned int           *cnt;

#endif

{

	unsigned int   run;

	int    packed   = *cnt;

	unsigned int   unpacked = 0;



	while ( packed > 0 )

	{

		run = *srcPtr++;

		if ( run >= 128 )

		{

			/* PACKED RUN					*/

			unpacked += (run = 257 - run);

			packed   -= 2;

			while ( run-- )

				*dstPtr++ = *srcPtr;

			srcPtr++;

		}

		else

		{

			/* NONPACKED RUN				*/

			run++;

			unpacked += run;

			packed   -= run + 1;

			while ( run-- )

				*dstPtr++ = *srcPtr++;

		}

	}



	*cnt = unpacked;

}

#endif





/*

 *  FUNCTION

 *		PackBits3 - Use macintosh byte packing algorithm (16 bit data)

 *

 *  DESCRIPTION

 *		PackBits compresses "width" bytes of data (as 16 bit words) starting

 *		at "cIn" and stores the compressed data at "cOut".  The 16 bits words

 *		are compressed when there are three or more consecutive equal words.

 *		After the data is compressed, the value of "width" is the number of

 *		compressed bytes stored at "cOut".  In the worst case, the compressed

 *		data can be one 16 bit word (2 bytes) longer than the original data.

 */



void

#ifdef __STDC__

PackBits3( unsigned char *cIn, unsigned char *cOut, unsigned int * width )

#else

PackBits3( cIn, cOut, width )

	unsigned char *cIn;

	unsigned char *cOut;

	unsigned int * width;

#endif

{

	int cnt;

	int len;

	unsigned int wid;



	wid = *width;

	len = 0;



	/* Loop thru the unencoded 16 bit words */

	while ( wid > 0 ) 

	{



		if ( wid == 1 ) 

		{    /* WE HAVE 1 BYTE LEFT */

			cOut[0] = 0;      /* THIS CASE SHOULD NEVER HAPPEN: ODD BYTE COUNT */

			cOut[1] = 0;		/* BUT, PAD THE ODD BYTE WITH A ZERO BYTE */

			cOut[2] = cIn[0];

			len += 3;

			wid = 0;

		} 

		else if ( (wid == 2) || (wid == 3) ) 

		{   /* WE HAVE 2 OR 3 BYTES LEFT */

			cOut[0] = 0;

			cOut[1] = cIn[0];

			cOut[2] = cIn[1];

			len += 3;

			wid -= 2;

		} 

		else if ( (wid == 4) || (wid == 5) ) 

		{   /* WE HAVE 4 OR 5 BYTES LEFT */

			cOut[0] = 1;

			cOut[1] = cIn[0];

			cOut[2] = cIn[1];

			cOut[3] = cIn[2];

			cOut[4] = cIn[3];

			len += 5;

			wid -= 4;

		} 

		else if ( wid == 6 ) 

		{

			if ( (cIn[0] == cIn[2])

			  && (cIn[1] == cIn[3])

			  && (cIn[2] == cIn[4])

			  && (cIn[3] == cIn[5]) ) 

			  {

				cOut[0] = (256 - 3) + 1;

				cOut[1] = cIn[0];

				cOut[2] = cIn[1];

				len += 3;

				wid = 0;

			} else 

			{

				cOut[0] = 2;

				cOut[1] = cIn[0];

				cOut[2] = cIn[1];

				cOut[3] = cIn[2];

				cOut[4] = cIn[3];

				cOut[5] = cIn[4];

				cOut[6] = cIn[5];

				len += 7;

				wid = 0;

			}

		} 

		else if ( (cIn[0] == cIn[2])

			  && (cIn[1] == cIn[3])

			  && (cIn[2] == cIn[4])

			  && (cIn[3] == cIn[5]) ) 

		{

			/* PACKED RUN */

			/* We have >= 3 equal words */

			cnt=3;

			while ( ( cnt<wid )

				&& ( cnt<127 )

				&& ( cIn[0]==cIn[cnt*2] )

				&& ( cIn[1]==cIn[cnt*2+1] ) ) 

				{

					cnt++;

				}

			cOut[0] = (256 - cnt) + 1;        /* count byte: Two's comp */

			cOut[1] = cIn[0];                      /* The repeated word */

			cOut[2] = cIn[1];

			cOut += 3;                    /* We have "output" 3 more bytes */

			cIn += (cnt*2 + 1);   /* Advance unpacked ptr: countbyte & cnt */

			len += 3;          /* We have added 3 more bytes to packed ptr */

			wid -= cnt*2;                 /* We looked at # unpacked bytes */

		} 

		else 

		{

			/* UNPACKED/LITERAL RUN */

			/* We have >= 3 unequal words */

			cnt=0;

			while ( ( cnt<wid )

				&& ( cnt<127 )

				&& ! ( (cIn[cnt*2+0]==cIn[cnt*2+2])

				    && (cIn[cnt*2+1]==cIn[cnt*2+3]) )    ) 

		  	{

				cOut[cnt*2+1] = cIn[cnt*2+0];

				cOut[cnt*2+2] = cIn[cnt*2+1];

				cnt++;

			}

			cOut[0] = cnt - 1;                               /* count byte */

			cOut += (cnt*2 + 1);   /* Advance packed ptr: countbyte & byte */

			cIn += cnt*2;                     /* Advance unpacked ptr: cnt */

			len += (cnt*2 + 1);       /* We added more bytes to packed ptr */

			wid -= cnt*2;     /* We have looked at cnt more unpacked bytes */

		}



	}



	*width = len;

}





/*

 *  FUNCTION

 *		UnpackBits - Use macintosh byte unpacking algorithm (16 bit data)

 *

 *  DESCRIPTION

 *  	Given a pointer in srcPtr to data that was compressed by PackBits,

 *  	UnpackBits expands the data and stores the result at dstPtr.  cnt is

 *  	passed IN as the number of compressed bytes to unpack.  cnt, when

 *  	passed back OUT, is the number of uncompressed bytes.

 */



void

#ifdef __STDC__

UnpackBits3( unsigned char *srcPtr, unsigned char *dstPtr, unsigned int *cnt )

#else

UnpackBits3( srcPtr, dstPtr, cnt )

	unsigned char *srcPtr;

	unsigned char *dstPtr;

	unsigned int           *cnt;

#endif

{

	unsigned int   run;

	int    packed   = *cnt;

	unsigned int   unpacked = 0;



	while ( packed > 0 )

	{

		run = *srcPtr++;

		if ( run >= 128 )

		{

			/* PACKED RUN					*/

			run = 256 - run + 1;

			unpacked += (run*2);

			packed   -= 3;

			while ( run-- ) 

			{

				*dstPtr++ = srcPtr[0];

				*dstPtr++ = srcPtr[1];

			}

			srcPtr++;

			srcPtr++;

		}

		else

		{

			/* NONPACKED RUN				*/

			run++;

			unpacked += (run*2);

			packed   -= (run*2 + 1);

			while ( run-- ) {

				*dstPtr++ = *srcPtr++;

				*dstPtr++ = *srcPtr++;

			}

		}

	}



	*cnt = unpacked;

}



