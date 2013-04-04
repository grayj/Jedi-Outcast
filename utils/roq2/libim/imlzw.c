/**

 **     $Header: /roq/libim/imlzw.c 1     11/02/99 4:38p Zaphod $

 **     Copyright (c) 1989-1995  San Diego Supercomputer Center (SDSC)

 **             a division of General Atomics, San Diego, California, USA

 **

 **     Users and possessors of this source code are hereby granted a

 **     nonexclusive, royalty-free copyright and design patent license to

 **     use this code in individual software.  License is not granted for

 **     commercial resale, in whole or in part, without prior written

 **     permission from SDSC.  This source is provided "AS IS" without express

 **     or implied warranty of any kind.

 **

 **     For further information contact:

 **             E-Mail:         info@sds.sdsc.edu

 **

 **             Surface Mail:   Information Center

 **                             San Diego Supercomputer Center

 **                             P.O. Box 85608

 **                             San Diego, CA  92138-5608

 **                             (619) 534-5000

 **/



#define HEADER  "    $Header: /roq/libim/imlzw.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **     imlzw.c         -  Do Lempel-Ziv & Welch uncompress/compress

 **

 **  PROJECT

 **     libim   -  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imlzw.c contains rountines for decoding and encoding byte buffers

 **	using Lempel-Ziv & Welch uncompress/compress algorithm.

 **

 **  PUBLIC CONTENTS

 **                     d =defined constant

 **                     f =function

 **                     m =defined macro

 **                     t =typedef/struct/union

 **                     v =variable

 **                     ? =other

 **

 **     ImLzwPreDecode          f  Set up for lzw uncompress

 **     ImLzwDecode             f  Do uncompression

 **     ImLzwPostDecode         f  Clean up after lzw uncompress

 **

 **     ImLzwPreEncode          f  Set up for lzw compress

 **     ImLzwEncode             f  Do compression

 **     ImLzwPostEncode         f  Clean up after lzw compress

 **

 **

 **

 **  PRIVATE CONTENTS

 **     NULL                    d  an empty pointer

 **

 **     imLzwGetNextCode       	f  Get the next compression code from buf

 **     imLzwPutNextCode      	f  Put the next compression code to output

 **     imLzwWriteData      	f  Write compressed pixels to output

 **     imLzwClearBlock        	f  Check the compression ration, poss reset

 **     imLzwClearHash         	f  Reset the compression hash table

 **     imLzwCleanUp          	f  Clean up storage at the end

 **

 **  HISTORY

 **     $Log: /roq/libim/imlzw.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.8  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.7  1995/06/29  00:19:56  bduggan

 **	added some casts

 **

 **	Revision 1.6  1994/10/03  11:29:40  nadeau

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

 **	Revision 1.6  1994/10/03  11:29:40  nadeau

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

 **	Revision 1.5  92/08/31  17:27:42  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.4  92/04/09  09:33:46  groening

 **	To make the compiler happy added extern statements.

 **	

 **	Revision 1.3  91/10/03  09:14:06  todd

 **	Updated to match TIFF 5.0 spec.

 **	

 **	Revision 1.2  91/03/15  11:23:47  todd

 **	added return( 0 ) statements

 **	

 **	Revision 1.1  91/01/29  11:09:24  todd

 **	Initial revision

 **	

 **

 **

 **/



/*

 * Copyright (c) 1988 by Sam Leffler.

 * All rights reserved.

 *

 * This file is provided for unrestricted use provided that this

 * legend is included on all tape media and as a part of the

 * software program in whole or part.  Users may copy, modify or

 * distribute this file at will.

 */



/*

 * Rev 5.0 Lempel-Ziv & Welch Compression Support

 *

 * This code is derived from the compress program whose code is

 * derived from software contributed to Berkeley by James A. Woods,

 * derived from original work by Spencer Thomas and Joseph Orost.

 *

 * The original Berkeley copyright notice appears below in its entirety.

 */



/*

 * Copyright (c) 1985, 1986 The Regents of the University of California.

 * All rights reserved.

 *

 * This code is derived from software contributed to Berkeley by

 * James A. Woods, derived from original work by Spencer Thomas

 * and Joseph Orost.

 *

 * Redistribution and use in source and binary forms are permitted

 * provided that the above copyright notice and this paragraph are

 * duplicated in all such forms and that any documentation,

 * advertising materials, and other materials related to such

 * distribution and use acknowledge that the software was developed

 * by the University of California, Berkeley.  The name of the

 * University may not be used to endorse or promote products derived

 * from this software without imLzwSpecific prior written permission.

 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR

 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED

 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.

 */



#include <stdio.h>

#include "iminternal.h"



#ifdef __STDC__

static int imLzwGetNextCode(unsigned char *buf);

static int imLzwPutNextCode( int ioType, int fd, FILE *fp, int c);

static int imLzwClearBlock( int ioType, int fd, FILE *fp);

static int imLzwWriteData( int ioType, int fd, FILE *fp );

#else

static int imLzwGetNextCode( );

static int imLzwPutNextCode( );

static int imLzwClearBlock( );

static int imLzwWriteData( );

#endif



/*

 *  MACROS

 *      

 *

 *  DESCRIPTION

 *	Decode one scanline of pixels.

 * 	The TIFF imLzwSpec imLzwSpecifies that encoded bit strings range

 * 	from 9 to 12 bits.  This is somewhat unfortunate in that

 * 	experience indicates full color RGB pictures often need

 * 	~14 bits for reasonable compression.

 */

#define IMMAXCODE(n)	((1 << (n)) - 1)

#define	IMBITS_MIN	9		/* start with 9 bits */

#define	IMBITS_MAX	12		/* max of 12 bit strings */



/* 

 * predefined codes 

 */



#define	IMCODE_CLEAR	256		/* code to clear string table */

#define	IMCODE_EOI	257		/* end-of-information code */

#define IMCODE_FIRST	258		/* first free code entry */

#define	IMCODE_MAX	IMMAXCODE(IMBITS_MAX)



#ifdef notdef

#define	IMHSIZE		9001		/* 91% occupancy */

#define	IMHSHIFT	(8-(16-13))

#else

#define	IMHSIZE		5003		/* 80% occupancy */

#define	IMHSHIFT		(8-(16-12))

#endif





/*

 * NB: The 5.0 spec describes a different algorithm than Aldus

 *     implements.  Specifically, Aldus does code length transitions

 *     one code earlier than should be done (for real LZW).

 *     Earlier versions of this library implemented the correct

 *     LZW algorithm, but emitted codes in a bit order opposite

 *     to the TIFF spec.  Thus, to maintain compatibility w/ Aldus

 *     we interpret MSB-LSB ordered codes to be images written w/

 *     old versions of this library, but otherwise adhere to the

 *     Aldus "off by one" algorithm.

 *

 * Future revisions to the TIFF spec are expected to "clarify this issue".

 */

#define IMSETMAXCODE(sp, v) {                  \
        (sp)->lzwMaxcode = (v)-1;              \
        if ((sp)->lzwFlags & IMLZW_COMPAT)     \
                (sp)->lzwMaxcode++;            \
}







/*

 *  TYPEDEF & STRUCTURE

 *      

 *

 *  DESCRIPTION

 * 	Decoding and encoding imLzwSpecific state.

 */

struct decode {

	short	prefixtab[IMHSIZE];	/* prefix(code) */

	unsigned char	suffixtab[IMCODE_MAX+1];	/* suffix(code) */

	unsigned char	stack[IMHSIZE-(IMCODE_MAX+1)];

	unsigned char	*stackp;		/* stack pointer */

	int	firstchar;		/* of string associated w/ last code */

};



/*

 * Encoding-imLzwSpecific state.

 */

struct encode {

	int	checkpoint;		/* point at which to clear table */

#define IMCHECK_GAP	10000		/* enc_ratio check interval */

	long	ratio;			/* current compression ratio */

	int	incount;		/* (input) data bytes encoded */

	int	outcount;		/* encoded (output) bytes */

	int	htab[IMHSIZE];		/* hash table */

	short	codetab[IMHSIZE];		/* code table */

};



/*

 * State block for each open TIFF

 * file using Lzw compression.

 */

typedef	struct LzwState

{

	int	lzwOldcode;		/* last code encountered 	*/

/*new*/	unsigned char	lzwHorDif;		/* Undefined in 2.2	*/

#define IMLZW_HORDIFF4    0x01            /* hor. diff w/ 4-bit samples */

#define IMLZW_HORDIFF8    0x02            /* hor. diff w/ 8-bit samples */

#define IMLZW_HORDIFF16   0x04            /* hor. diff w/ 16-bit samples */

#define IMLZW_HORDIFF32   0x08            /* hor. diff w/ 32-bit samples */

	unsigned short	lzwFlags;		/* flags			*/

#define IMLZW_RESTART     0x01            /* restart interrupted decode */

#define IMLZW_COMPAT      0x02            /* read old bit-reversed codes */

	unsigned short	lzwNbits;		/* number of bits/code 		*/

/*new*/	unsigned short	lzwStride;		/* number of bits/code 		*/

	int	lzwMaxcode;		/* maximum code for lzwNbits 	*/

	int	lzwBitoff;		/* bit offset into data 	*/

	int	lzwBitsize;		/* size of strip in bits 	*/

	int	lzwFree_ent;		/* next free entry in hash tabl	*/

	int     lzwRawcc;               /* byte count of unread/written */

	int	lzwBytesWritten;        /* byte count of unread/written */

	char   *lzwRawdata;		/* raw data buffer		*/

	union {

		struct	decode dec;

		struct	encode enc;

	} u;

} LzwState;



/*

 *  TYPEDEF & STRUCTURE

 *	dec & enc 	- shorthand structure field names      

 *

 *  DESCRIPTION

 * 	Decoding and encoding imLzwSpecific state.

 */

#define	IM_DEC_PREFIX	u.dec.prefixtab

#define	IM_DEC_SUFFIX	u.dec.suffixtab

#define	IM_DEC_STACK	u.dec.stack

#define	IM_DEC_STACKP	u.dec.stackp

#define	IM_DEC_FIRSTCHAR	u.dec.firstchar



#define	IM_ENC_CHECKPOINT	u.enc.checkpoint

#define	IM_ENC_RATIO	u.enc.ratio

#define	IM_ENC_INCOUNT	u.enc.incount

#define	IM_ENC_OUTCOUNT	u.enc.outcount

#define	IM_ENC_HTAB	u.enc.htab

#define	IM_ENC_CODETAB	u.enc.codetab



/*

 *  TYPEDEF & STRUCTURE

 *      

 *

 *  DESCRIPTION

 * 	Decoding and encoding imLzwSpecific state.

 */

/* 

 * masks for extracting/inserting variable length bit codes 

 */

static	unsigned char rmask[9] =

    { 0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff };

static  unsigned char lmask[9] =

    { 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff };



/*  

 * Old lmask

static	unsigned char lmask[9] =

    { 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00 };

*/







#ifdef __STDC__

static void imLzwClearHash(void);

#else

static void imLzwClearHash();

#endif





/*

 *  STRUCTURE

 *  	imLzwSp	- pointer to the global State info structure    

 *

 *  DESCRIPTION

 */

LzwState 	*imLzwSp=NULL;







/*

 * Lzw Decoder.

 */



/*

 *  FUNCTION

 *      ImLzwPreDecode     -  Inititialize structures for encode/decode

 *

 *  DESCRIPTION

 *	Set up the encode/decode structure

 */

int

#ifdef __STDC__

ImLzwPreDecode( unsigned char *buf, int size )

#else

ImLzwPreDecode( buf, size )

	unsigned char *buf;	/* Encoded bytes so we can check first 2 bytes	*/

	int size;	/* Max number of bytes to be decoded		*/

#endif

{

	int code;



	if (imLzwSp == NULL) 

	{

		ImCalloc( imLzwSp, LzwState *, sizeof(LzwState), 1 );

		imLzwSp->lzwFlags = 0;

		imLzwSp->lzwHorDif = 0;

		imLzwSp->lzwStride = 0;

	}

	else

		imLzwSp->lzwFlags &= ~IMLZW_RESTART;



	imLzwSp->lzwNbits = IMBITS_MIN;



	/*

	 * Pre-load the table.

	 */

	for (code = 255; code >= 0; code--)

		imLzwSp->IM_DEC_SUFFIX[code] = (unsigned char)code;



	imLzwSp->lzwFree_ent = IMCODE_FIRST;

	imLzwSp->lzwBitoff = 0;



	/* 

	 * Calculate data size in bits 

	 */

	imLzwSp->lzwBitsize = size;

	imLzwSp->lzwBitsize = (imLzwSp->lzwBitsize << 3) - (IMBITS_MAX-1);

	imLzwSp->IM_DEC_STACKP = imLzwSp->IM_DEC_STACK;

	imLzwSp->lzwOldcode = -1;

	imLzwSp->IM_DEC_FIRSTCHAR = -1;

	imLzwSp->lzwRawcc = size;

	imLzwSp->lzwBytesWritten = 0;

	imLzwSp->lzwRawdata = NULL;



	/*

 	* Check for old bit-reversed codes.  All the flag

 	* manipulations are to insure only one warning is

 	* given for a file.

 	*/

	if ((buf[0] == 0) && (buf[1] & 0x1)) 

	{

       		if ((imLzwSp->lzwFlags & IMLZW_COMPAT) == 0)

			ImErrorWarning("Old LZW codes-converting",-1,IMESYNTAX);



        	imLzwSp->lzwFlags |= IMLZW_COMPAT;

	} else

        	imLzwSp->lzwFlags &= ~IMLZW_COMPAT;





	IMSETMAXCODE( imLzwSp, IMMAXCODE(IMBITS_MIN));

	return (0);

}





/*

 *  FUNCTION

 *      ImLzwDecode     -  Decode one scanline of pixels.

 *

 *  DESCRIPTION

 *	Decode one scanline of pixels.

 */

int

#ifdef __STDC__

ImLzwDecode(unsigned char *buf, char *op, int occ)

#else

ImLzwDecode(buf, op, occ)

	unsigned char *buf;		/* Pixels to be decoded		*/

	char *op;		/* Decoded pixels returned 	*/

	int occ;

#endif

{

	int code;		/* A compression code		*/

	int retCnt=0;		/* The number of bytes uncompre */

	unsigned char *stackp;		/* Stack pointer		*/

	int firstchar, oldcode, incode; 





	stackp = imLzwSp->IM_DEC_STACKP;



	/*

	 * Restart interrupted unstacking operations.

	 */

	if (imLzwSp->lzwFlags & IMLZW_RESTART) 

	{

		do 

		{

			if (--occ < 0) 		/* end of scanline */

			{	

				imLzwSp->IM_DEC_STACKP = stackp;

				return (1);

			}

			*op++ = *--stackp;

			retCnt++;

		} while (stackp > imLzwSp->IM_DEC_STACK);

		imLzwSp->lzwFlags &= ~IMLZW_RESTART;

	}



	oldcode = imLzwSp->lzwOldcode;

	firstchar = imLzwSp->IM_DEC_FIRSTCHAR;



	while (occ > 0 && (code = imLzwGetNextCode(buf)) != IMCODE_EOI) 

	{

		if (code == IMCODE_CLEAR) 

		{

			memset(imLzwSp->IM_DEC_PREFIX, 0x00, sizeof (imLzwSp->IM_DEC_PREFIX));

			imLzwSp->lzwFree_ent = IMCODE_FIRST;

			imLzwSp->lzwNbits = IMBITS_MIN;

			IMSETMAXCODE( imLzwSp, IMMAXCODE(IMBITS_MIN));

			if ((code = imLzwGetNextCode(buf)) == IMCODE_EOI)

				break;

			*op++ = code, occ--;

			retCnt++;

			oldcode = firstchar = code;

			continue;

		}



		incode = code;



		/*

		 * When a code is not in the table we use (as shown):

		 *    StringFromCode(oldcode) +

		 *        FirstChar(StringFromCode(oldcode))

		 */

		if (code >= imLzwSp->lzwFree_ent) 	/* code not in table */

		{	

			*stackp++ = firstchar;

			code = oldcode;

		}



		/*

		 * Generate output string (first in reverse).

		 */

		for (; code >= 256; code = imLzwSp->IM_DEC_PREFIX[code])

			*stackp++ = imLzwSp->IM_DEC_SUFFIX[code];



		*stackp++ = firstchar = imLzwSp->IM_DEC_SUFFIX[code];



		do 

		{

			if (--occ < 0) 		/* end of scanline */

			{	

				imLzwSp->lzwFlags |= IMLZW_RESTART;

				break;

			}

			*op++ = *--stackp;

			retCnt++;



		} while (stackp > imLzwSp->IM_DEC_STACK);



		/*

		 * Add the new entry to the code table.

		 */

		if ((code = imLzwSp->lzwFree_ent) < IMCODE_MAX) 

		{

			imLzwSp->IM_DEC_PREFIX[code] = (unsigned short)oldcode;

			imLzwSp->IM_DEC_SUFFIX[code] = firstchar;

			imLzwSp->lzwFree_ent++;

                        /*

                         * If the next entry is too big for the

                         * current code size, then increase the

                         * size up to the maximum possible.

                         */

                        if (imLzwSp->lzwFree_ent > imLzwSp->lzwMaxcode) {

                                imLzwSp->lzwNbits++;

                                if (imLzwSp->lzwNbits > IMBITS_MAX)

                                        imLzwSp->lzwNbits = IMBITS_MAX;

                                IMSETMAXCODE(imLzwSp, IMMAXCODE(imLzwSp->lzwNbits));

                        }

		} 

		oldcode = incode;

	}

	imLzwSp->IM_DEC_STACKP = stackp;

	imLzwSp->lzwOldcode = oldcode;

	imLzwSp->IM_DEC_FIRSTCHAR = firstchar;



	/*

	 * If we were doing prediction we would have to put some

	 * horizontal differencing code here

	 */



	if (occ > 0) 

		ImErrorFatal("Not enough data for scanline", -1, IMENOREAD );



	return ( retCnt );

}





/*

 *  FUNCTION

 *      ImLzwPostDecode     - Clean up after uncompression

 *

 *  DESCRIPTION

 *

 */

int

ImLzwPostDecode()

{

	return ( 1 );

}





/*

 *  FUNCTION

 *      imLzwGetNextCode     - Get the next code out of the buffer

 *

 *  DESCRIPTION

 * 	Get the next code from the raw data buffer.

 */

static int

#ifdef __STDC__

imLzwGetNextCode(unsigned char *buf)

#else

imLzwGetNextCode(buf)

	unsigned char 	*buf;		/* Undecoded data	*/

#endif

{

	int code, r_off, bits;

	unsigned char *bp;



	/*

	 * This check shouldn't be necessary because each

	 * strip is suppose to be terminated with IMCODE_EOI.

	 * At worst it's a substitute for the IMCODE_EOI that's

	 * supposed to be there (see calculation of lzwBitsize

	 * in LzwPreDecode()).

	 */

	if (imLzwSp->lzwBitoff > imLzwSp->lzwBitsize)

		return (IMCODE_EOI);



	r_off = imLzwSp->lzwBitoff;

	bits = imLzwSp->lzwNbits;



	/*

	 * Get to the first byte.

	 */

	bp = (unsigned char *)buf + (r_off >> 3);

	r_off &= 7;



	if (imLzwSp->lzwFlags & IMLZW_COMPAT)

	{

                /* Get first part (low order bits) */

                code = (*bp++ >> r_off);

                r_off = 8 - r_off;              /* now, offset into code word */

                bits -= r_off;

                /* Get any 8 bit parts in the middle (<=1 for up to 16 bits). */

                if (bits >= 8) {

                        code |= *bp++ << r_off;

                        r_off += 8;

                        bits -= 8;

                }

                /* high order bits. */

                code |= (*bp & rmask[bits]) << r_off;

        }

	else 

	{

                r_off = 8 - r_off;              /* convert offset to count */

                code = *bp++ & rmask[r_off];    /* high order bits */

                bits -= r_off;

                if (bits >= 8) {

                        code = (code<<8) | *bp++;

                        bits -= 8;

                }

                /* low order bits */

                code = (code << bits) | ((*bp & lmask[bits]) >> (8 - bits));

        }



	imLzwSp->lzwBitoff += imLzwSp->lzwNbits;

	return (code);

}







/*

 * Lzw Encoding.

 */



/*

 *  FUNCTION

 *      ImLzwPreEncode     - initialize data structure

 *

 *  DESCRIPTION

 * 	Allocate memory and initialize values as necessary.

 * 	Reset encoding state at the start of a strip.

 */

int

#ifdef __STDC__

ImLzwPreEncode(int size)

#else

ImLzwPreEncode(size)

	int 	size;		/* The number of bytes to be decoded 	*/

#endif

{

	if (imLzwSp == NULL) 

	{

		ImCalloc( imLzwSp, LzwState *, sizeof(LzwState), 1 );

                imLzwSp->lzwFlags = 0;

                imLzwSp->lzwHorDif = 0;

                imLzwSp->lzwStride = 0;

        }



	imLzwSp->IM_ENC_RATIO = 0;

	imLzwSp->IM_ENC_CHECKPOINT = IMCHECK_GAP;

	IMSETMAXCODE(imLzwSp, IMMAXCODE(imLzwSp->lzwNbits = IMBITS_MIN)+1);

	imLzwSp->lzwFree_ent = IMCODE_FIRST;

	imLzwSp->lzwBitoff = 0;

	imLzwSp->lzwBitsize = (size << 3) - (IMBITS_MAX-1);

	imLzwClearHash();		/* clear hash table 		     */

	imLzwSp->lzwOldcode = -1;	/* generates IMCODE_CLEAR in LzwEncode */

	imLzwSp->lzwRawcc = size;

	imLzwSp->lzwBytesWritten = 0;



	/*

	 * This is stinky.  The lzw code uses a fixed size buffer to store

	 * the compressed data.  Compressed data should be smaller so we

	 * allocate a buffer twice as big just in case.

	 */

	if (imLzwSp->lzwRawdata == NULL)

		ImCalloc( imLzwSp->lzwRawdata, char *, 1, size*2 );



	return( 0 );

}



/*

 *  FUNCTION

 *      imLzwEncode     - Encode a scanline of pixels

 *

 *  DESCRIPTION

 * 	Uses an open addressing double hashing (no chaining) on the 

 * 	prefix code/next character combination.  We do a variant of

 * 	Knuth's algorithm D (vol. 3, sec. 6.4) along with G. Knott's

 * 	relatively-prime secondary probe.  Here, the modular division

 * 	first probe is gives way to a faster exclusive-or manipulation. 

 * 	Also do block compression with an adaptive reset, whereby the

 * 	code table is cleared when the compression ratio decreases,

 * 	but after the table fills.  The variable-length output codes

 * 	are re-sized at this point, and a IMCODE_CLEAR is generated

 * 	for the decoder. 

 */

int

#ifdef __STDC__

ImLzwEncode(int ioType, int fd, FILE *fp, unsigned char *bp, int cc)

#else

ImLzwEncode(ioType, fd, fp, bp, cc)

        int         ioType;             /* I/O flags                    */

        int         fd;                 /* Input file descriptor        */

        FILE       *fp;                 /* Input file pointer           */

	unsigned char 	   *bp;			/* Bytes to be encoded		*/

	int         cc;			/* Count of bytes to be encoded	*/

#endif

{

	long fcode;

	int h, c, ent, disp;



	if ( imLzwSp == NULL )

	{

		ImErrorFatal("Null pointer in LZW encode", -1, IMEENCODING );

	}



	/*

	 * If we were doing prediction we would have to put some

 	 * horizontal differencing code here

	 */



	ent = imLzwSp->lzwOldcode;



	if (ent == -1 && cc > 0) 

	{

		if (imLzwPutNextCode(ioType, fd, fp, IMCODE_CLEAR) == -1)

			return( -1 );		/* ImErrNo already set */



		ent = *bp++; cc--; imLzwSp->IM_ENC_INCOUNT++;

	}



	while (cc > 0) 

	{

		c = *bp++; cc--; imLzwSp->IM_ENC_INCOUNT++;



		fcode = ((long)c << IMBITS_MAX) + ent;

		h = (c << IMHSHIFT) ^ ent;	/* xor hashing */



		if (imLzwSp->IM_ENC_HTAB[h] == fcode) 

		{

			ent = imLzwSp->IM_ENC_CODETAB[h];

			continue;

		}



		if (imLzwSp->IM_ENC_HTAB[h] >= 0) 

		{

			/*

			 * Primary hash failed, check secondary hash.

			 */

			disp = IMHSIZE - h;

			if (h == 0)

				disp = 1;

			do 

			{

				if ((h -= disp) < 0)

					h += IMHSIZE;

				if (imLzwSp->IM_ENC_HTAB[h] == fcode) 

				{

					ent = imLzwSp->IM_ENC_CODETAB[h];

					goto hit;

				}

			} while (imLzwSp->IM_ENC_HTAB[h] >= 0);

		}



		/*

		 * New entry, add to table.

		 */

		if (imLzwPutNextCode(ioType, fd, fp, ent) == -1)

			return( -1 );		/* ImErrNo already set */

		ent = c;



                imLzwSp->IM_ENC_CODETAB[h] = imLzwSp->lzwFree_ent++;

                imLzwSp->IM_ENC_HTAB[h] = (int) fcode;



		if (imLzwSp->lzwFree_ent == IMCODE_MAX-1)  /* Table is full */

		{

			imLzwSp->IM_ENC_RATIO = 0;

			imLzwClearHash();         /* clear hash table */

			imLzwSp->lzwFree_ent = IMCODE_FIRST; 

			if (imLzwPutNextCode(ioType, fd, fp, IMCODE_CLEAR) == -1)

				return( -1 );	/* ImErrNo already set */

                        IMSETMAXCODE(imLzwSp, 

				IMMAXCODE(imLzwSp->lzwNbits = IMBITS_MIN)+1);

		}

		else	/* Table is not full */

		{

			if (imLzwSp->IM_ENC_INCOUNT >= imLzwSp->IM_ENC_CHECKPOINT)

			{

				if ( imLzwClearBlock( ioType, fd, fp ) == -1 )

					return( -1 );  /* ImErrNo already set */

			}

			if (imLzwSp->lzwFree_ent > imLzwSp->lzwMaxcode)

			{

         			imLzwSp->lzwNbits++;

                                if ( ! (imLzwSp->lzwNbits <= IMBITS_MAX))

				{	

					ImErrorFatal("Nbits <= MaxBits", -1, 

								IMEENCODING );

				}

                                IMSETMAXCODE(imLzwSp, 

					IMMAXCODE(imLzwSp->lzwNbits)+1);

			}

		}

	hit:

		;

	}



	imLzwSp->lzwOldcode = ent;



	return ( 0 );

}





/*

 *  FUNCTION

 *      ImLzwPostEncode     - Clean up after compression

 *

 *  DESCRIPTION

 * 	Finish off an encoded strip by flushing the last

 * 	string and tacking on an End Of Information code.

 *	Returns the total number of bytes written out.

 */

int

#ifdef __STDC__

ImLzwPostEncode( int ioType, int fd, FILE *fp )

#else

ImLzwPostEncode( ioType, fd, fp )

        int         ioType;             /* I/O flags                    */

        int         fd;                 /* Input file descriptor        */

        FILE       *fp;                 /* Input file pointer           */

#endif

{

	if (imLzwSp->lzwOldcode != -1)

		if (imLzwPutNextCode(ioType, fd, fp, imLzwSp->lzwOldcode) == -1)

			return( -1 );		/* ImErrNo already set	*/



	if (imLzwPutNextCode( ioType, fd, fp, IMCODE_EOI) == -1)

		return( -1 );		/* ImErrNo already set 		*/



	if (imLzwWriteData( ioType, fd, fp ) == -1)

		return( -1 );	/* ImErrNo already set	*/



	return ( imLzwSp->lzwBytesWritten );

}





/*

 *  FUNCTION

 *      imLzwPutNextCode     - Put the next code to output

 *

 *  DESCRIPTION

 * 	Put the next code to output

 */

static int

#ifdef __STDC__

imLzwPutNextCode( int ioType, int fd, FILE *fp, int c)

#else

imLzwPutNextCode( ioType, fd, fp, c)

        int         ioType;             /* I/O flags                    */

        int         fd;                 /* Input file descriptor        */

        FILE       *fp;                 /* Input file pointer           */

	int 	    c;			/* Code				*/

#endif

{

	int r_off, bits, code = c;

	char *bp;



	r_off = imLzwSp->lzwBitoff;

	bits = imLzwSp->lzwNbits;



	/*

	 * Flush buffer if code doesn't fit.

	 */

	if (r_off + bits > imLzwSp->lzwBitsize) 

	{

		/*

		 * Calculate the number of full bytes that can be

		 * written and save anything else for the next write.

		 */

		if (r_off & 7) 

		{

			imLzwSp->lzwRawcc = r_off >> 3;

			bp = imLzwSp->lzwRawdata + imLzwSp->lzwRawcc;



			if (imLzwWriteData( ioType, fd, fp ) == -1)

				return( -1 );	/* ImErrNo already set	*/



			imLzwSp->lzwRawdata[0] = *bp;

		} 

		else 

		{

			/*

			 * Otherwise, on a byte boundary (in

			 * which lzwRawcc is already correct).

			 */

			if (imLzwWriteData( ioType, fd, fp ) == -1)

				return( -1 );	/* ImErrNo already set	*/

		}

		bp = imLzwSp->lzwRawdata;

		imLzwSp->lzwBitoff = (r_off &= 7);

	} 

	else 

	{

		/*

		 * Get to the first byte.

		 */

		bp = imLzwSp->lzwRawdata + (r_off >> 3);

		r_off &= 7;

	}



        /*

         * Note that lzw_bitoff is maintained as the bit offset

         * into the buffer w/ a right-to-left orientation (i.e.

         * lsb-to-msb).  The bits, however, go in the file in

         * an msb-to-lsb order.

         */

        bits -= (8 - r_off);

        *bp = (*bp & lmask[r_off]) | (code >> bits);

        bp++;

        if (bits >= 8) {

                bits -= 8;

                *bp++ = code >> bits;

        }

        if (bits)

                *bp = (code & rmask[bits]) << (8 - bits);



	/*

	 * IM_ENC_OUTCOUNT is used by the compression analysis machinery

	 * which resets the compression tables when the compression

	 * ratio goes up.  lzwBitoff is used here (in imLzwPutNextCode) for

	 * inserting codes into the output buffer.  imLzwSp_rawcc must

	 * be updated for the mainline write code in TIFFWriteScanline()

	 * so that data is flushed when the end of a strip is reached.

	 * Note that the latter is rounded up to ensure that a non-zero

	 * byte count is present. 

	 */

	imLzwSp->IM_ENC_OUTCOUNT += imLzwSp->lzwNbits;

	imLzwSp->lzwBitoff += imLzwSp->lzwNbits;

	imLzwSp->lzwRawcc = (imLzwSp->lzwBitoff + 7) >> 3;



	return( 0 );

}





/*

 *  FUNCTION

 *      imLzwWriteData     - Write compressed data to output

 *

 *  DESCRIPTION

 *	Write compressed data to output

 */

static int

#ifdef __STDC__

imLzwWriteData( int ioType, int fd, FILE *fp )

#else

imLzwWriteData( ioType, fd, fp )

        int         ioType;             /* I/O flags                    */

        int         fd;                 /* Input file descriptor        */

        FILE       *fp;                 /* Input file pointer           */

#endif

{

	if ( ImBinWrite( ioType, fd, fp, 

		imLzwSp->lzwRawdata, UCHAR, 1, imLzwSp->lzwRawcc ) == -1 )

                ImReturnBinError( );



	imLzwSp->lzwBytesWritten += imLzwSp->lzwRawcc;



	imLzwSp->lzwRawcc = 0;



	return( 0 );

}





/*

 *  FUNCTION

 *      imLzwClearBlock     - Check compression ratio

 *

 *  DESCRIPTION

 * 	Check compression ratio and, if things seem to

 * 	be slipping, clear the hash table and reset state.

 */

static int

#ifdef __STDC__

imLzwClearBlock( int ioType, int fd, FILE *fp)

#else

imLzwClearBlock( ioType, fd, fp)

        int         ioType;             /* I/O flags                    */

        int         fd;                 /* Input file descriptor        */

        FILE       *fp;                 /* Input file pointer           */

#endif

{

	long rat;



	imLzwSp->IM_ENC_CHECKPOINT = imLzwSp->IM_ENC_INCOUNT + IMCHECK_GAP;



	if (imLzwSp->IM_ENC_INCOUNT > 0x007fffff) 	/* shift will overflow 	*/

	{	

		rat = imLzwSp->IM_ENC_OUTCOUNT >> 8;

		rat = (rat == 0 ? 0x7fffffff : imLzwSp->IM_ENC_INCOUNT / rat);

	} 

	else					/* 8 fract bits 	*/

		rat = (imLzwSp->IM_ENC_INCOUNT << 8) / imLzwSp->IM_ENC_OUTCOUNT; 



	if (rat <= imLzwSp->IM_ENC_RATIO) 

	{

		imLzwSp->IM_ENC_RATIO = 0;

		imLzwClearHash();

		imLzwSp->lzwFree_ent = IMCODE_FIRST;

		if (imLzwPutNextCode( ioType, fd, fp, IMCODE_CLEAR) == -1)

			return( -1 );		/* ImErrNo already set */

		IMSETMAXCODE( imLzwSp, IMMAXCODE(imLzwSp->lzwNbits = IMBITS_MIN)+1);

	} 

	else

		imLzwSp->IM_ENC_RATIO = rat;



	return( 0 );

}





/*

 *  FUNCTION

 *      imClearHash     - Get the next code out of the buffer

 *

 *  DESCRIPTION

 * 	Get the next code from the raw data buffer.

 */

/*

 * Reset code table.

 */

static void

imLzwClearHash()

{

	int *htab_p = imLzwSp->IM_ENC_HTAB+IMHSIZE;

	int i, m1 = -1;



	i = IMHSIZE - 16;



 	do {

		*(htab_p-16) = m1;

		*(htab_p-15) = m1;

		*(htab_p-14) = m1;

		*(htab_p-13) = m1;

		*(htab_p-12) = m1;

		*(htab_p-11) = m1;

		*(htab_p-10) = m1;

		*(htab_p-9) = m1;

		*(htab_p-8) = m1;

		*(htab_p-7) = m1;

		*(htab_p-6) = m1;

		*(htab_p-5) = m1;

		*(htab_p-4) = m1;

		*(htab_p-3) = m1;

		*(htab_p-2) = m1;

		*(htab_p-1) = m1;

		htab_p -= 16;

	} while ((i -= 16) >= 0);



    	for (i += 16; i > 0; i--)

		*--htab_p = m1;

}



/*

 *  FUNCTION

 *      ImLzwCleanup     - free storage

 *

 *  DESCRIPTION

 * 	Release the structure memory used by lzw compression 

 */

void

ImLzwCleanup()

{

	if (imLzwSp->lzwRawdata) 

		free(imLzwSp->lzwRawdata);

	if (imLzwSp) 

		free(imLzwSp);



	imLzwSp = NULL;

}



