/**

 **	$Header: /roq/libim/immpnt.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER  "    $Header: /roq/libim/immpnt.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	immpnt.c	-  Apple Macintosh MacPaint image file I/O

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	immpnt.c contains routines to read and write Apple Macintosh

 **	MacPaint files for the image manipulation library.  Raster data

 **	read in is stored in a VFB.  Raster data written out is taken

 **	from a tag table.

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **	none

 **

 **  PRIVATE CONTENTS

 **     imMpntRead      f  read  an Apple Macintosh MacPaint file

 **     imMpntWrite     f  write an Apple Macintosh MacPaint file

 **

 **	IMtstBit	m  Test an unsigned bit value

 **	IMsetBit	m  Set an unsigned bit value

 **

 **  HISTORY

 **	$Log: /roq/libim/immpnt.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.17  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.16  1995/06/15  20:32:31  bduggan

 **	Removed anything printing to stderr.  (i.e. debug messages)

 **

 **	Revision 1.15  1995/06/14  18:58:32  bduggan

 **	Added some casts, took out some useless variables.

 **

 **	Revision 1.14  1995/04/03  21:30:18  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.13  1995/01/10  23:33:45  bduggan

 **	put in IMMULTI, IMPIPE instead of TRUE/FALSE

 **	uncapitlized i's in local functions

 **	made read/write routines static

 **

 **	Revision 1.12  94/10/03  11:30:18  nadeau

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

 **	Revision 1.11  92/12/03  01:50:02  nadeau

 **	Corrected info messages.

 **	

 **	Revision 1.10  92/11/23  18:42:39  nadeau

 **	Removed use of IMINFOMSG.

 **	

 **	Revision 1.9  92/11/04  12:04:02  groening

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.8  92/09/29  17:57:58  vle

 **	Added ImInfo messages.

 **	

 **	Revision 1.7  92/08/31  17:28:24  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.6  91/10/03  09:14:30  nadeau

 **	Fixed #includes.

 **	

 **	Revision 1.5  91/03/12  17:56:23  nadeau

 **	Fixed bug with writing incomming VFBs that are smaller than

 **	mpnt standard size.  The right side of such images were comming

 **	out black, when they should have been white.

 **	

 **	Revision 1.4  91/02/18  16:24:34  moreland

 **	fixed !

 **	

 **	Revision 1.3  91/02/12  11:34:38  nadeau

 **	Removed the tag table checking and VFB conversion now

 **	handled by ImFileRead and ImFileWrite.  Removed the

 **	IMtstBit and IMsetBit functions and turned them into macros.

 **	Issued warning messages in write when the incomming VFB

 **	is bigger than MacPaint will allow an image to be.

 **	Optimized write.  Changed GRAY VFB references to MONO.

 **	

 **	Revision 1.2  91/01/28  10:49:43  moreland

 **	Worked on Write routine.

 **

 **	Revision 1.1  91/01/10  16:44:45  soraya

 **	Initial revision

 **/



#include "iminternal.h"







/**

 **  FORMAT

 **	MacPaint  -  Apple Macintosh MacPaint image file

 **

 **  AKA

 ** 	mpnt, macp, pntg

 **

 **  FORMAT REFERENCES

 **	- Bit-Mapped Graphics, Steve Rimmer

 **	- Graphics File Formats, David C. Kay, John R. Levine	

 **

 **  CODE CREDITS

 **     Custom development, Soraya Gonzalez, San Diego Supercomputer Center, 1991.

 **

 **  DESCRIPTION

 **	Apple Macintosh MacPaint image files start with a 512 byte header

 **	which is typically all 0 (zero) values, and is usually ignored.

 **	Following the 512 byte header is 720 packed scan lines.

 **	macpack.c has information about how the lines are packed.

 **/



/*

 *  MPNT - Apple Macintosh MacPaint

 *      For information on these structures, how to use them, etc. please

 *      see imfmt.c.

 */

#ifdef __STDC__

static int imMpntRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imMpntWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

#else

static int imMpntRead( );

static int imMpntWrite( );

#endif

static char *imMpntNames[ ]  = { "mpnt", "macp", "pntg", NULL };

static ImFileFormatReadMap imMpntReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IN,1,1,       PB,     IMVFBMONO,      0 },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imMpntWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBMONO,    0,      IN,1,1,         PB,     imMpntWrite },

        { -1,           0,      -1,             0,      NULL },

};



static ImFileMagic imFileMpntMagic []=

{

	  { 0, 0, NULL },

};



ImFileFormat ImFileMpntFormat =

{

	imMpntNames, "Apple Macintosh MacPaint file",

	"Apple Computer, Inc.",

	"1-bit Packbits-compressed MacPaint images.",

	"1-bit Packbits-compressed MacPaint images.",

	imFileMpntMagic,

	IMNOMULTI, IMPIPE,

	IMNOMULTI, IMPIPE,

	imMpntRead, imMpntReadMap, imMpntWriteMap

};



/*

 *  Macpaint defines

 */

#define IMMAC_HEADER_SIZE	512

#define IMMAC_MAX_UNPACKED_SIZE	51840

#define IMMAC_IMAGE_HEIGHT	720

#define IMMAC_IMAGE_WIDTH	576

#define IMMAC_ROW_BYTES		( IMMAC_IMAGE_WIDTH/8)

#define IMMAC_BLACK		0

#define IMMAC_WHITE		1





/*

 *  MACROS

 *	IMtstBit	-  Test a bit value

 *	IMsetBit	-  Set a bit value

 *

 *  DESCRIPTION

 *	IMtstBit:		return the on/off setting of the selected bit.

 *	IMsetBit:		if val == 0, turn the bit off, otherwise on.

 */



#define IMtstBit(byte,bit)	((1<<(bit))&(byte))

#define IMsetBit(pByte,bit)	(*(pByte) |= (1<<(bit)))

#define IMunsetBit(pByte,bit)	(*(pByte) &= ~(1<<(bit)))





/*

 *  FUNCTION

 *	imMpntRead  -  read an Apple Macintosh MacPaint file

 *

 *  DESCRIPTION

 *	A Macpaint header and image are read in.  The image is unpacked

 *	(Mac packbits compression) and copied into a monochrome VFB.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__

imMpntRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imMpntRead( ioType, fd, fp, flagsTable, tagTable )

	int		ioType;		/* I/O flags			*/

	int		fd;		/* Input file descriptor	*/

	FILE		* fp;		/* Input file pointer		*/

	TagTable	* flagsTable;	/* Flags			*/

	TagTable	* tagTable;	/* Tag table to add to		*/

#endif

{

	ImVfb		* vfb;		/* Read in image		*/

	ImVfbPtr	pPixel;		/* Pixel pointer		*/

	unsigned char		packed[  IMMAC_MAX_UNPACKED_SIZE ];/* Packed bits buffer*/

	unsigned char		unpacked[  IMMAC_IMAGE_WIDTH ];/* Unpacked bits buffer	*/

	unsigned char		*pPtr, *uPtr;	/* Bits buffer pointers		*/

	int		sLine;		/* Current scanline		*/

	int		col;		/* Current column		*/

	int		byte;		/* Byte run counter		*/

	unsigned int		bit;		/* Which bit in a byte		*/

	unsigned long		byteCount;	/* # of bytes read		*/

	unsigned int		count, oldcount;/* Run counters			*/

	char		 message[100];	/* ImInfo message		*/





	/*

	 *  Read (and ignore) the 512 byte header

	 */

	BinByteOrder( BINMBF );

	if ( ImBinRead( ioType, fd, fp, packed, UCHAR, 1, IMMAC_HEADER_SIZE ) == -1 )

	{

		ImReturnBinError();

	}





	/*

	 *  Allocate a Monochrome VFB of the required size.

	 */

	if ( (vfb = ImVfbAlloc(  IMMAC_IMAGE_WIDTH,  IMMAC_IMAGE_HEIGHT, IMVFBMONO )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError(), -1, ImErrNo );

	}



	/*

	 *  Read the whole compressed image into a buffer.

	 */

	if ( (byteCount = ImBinRead( ioType, fd, fp, packed, UCHAR, 1,  IMMAC_MAX_UNPACKED_SIZE )) == (unsigned long)-1 )

	{

		ImReturnBinError();

	}



	/*

	 *  Copy to the VFB, one scanline at a time.

	 */

	uPtr  = unpacked;

	pPtr  = packed;

	sLine = 0;

	col   = 0;

	pPixel = ImVfbQFirst( vfb );

	while ( sLine< IMMAC_IMAGE_HEIGHT )

	{

		uPtr = unpacked;

		if ( pPtr[0] > 127 )

			/* Unpack next 2 bytes of compressed data	*/

			count = 2;

		else

			/* Unpack next ? bytes of non-compressed data	*/

			count = pPtr[0] + 2;

		oldcount = count;		/* remember BYTE count	*/

		UnpackBits( pPtr, uPtr, &count );

		pPtr += oldcount;		/* Inc packedPtr by BYTE count*/



		for ( byte=0; byte<count; byte++ )

		{

			for ( bit=0; bit<8; bit++ )

			{

				ImVfbSMono( vfb, pPixel,

					IMtstBit( unpacked[byte], 7-bit ) ?

					 IMMAC_BLACK :  IMMAC_WHITE );

				ImVfbSInc( vfb, pPixel );

				if ( ++col >=  IMMAC_IMAGE_WIDTH )

				{

					sLine++;

					col = 0;

				}

			}

		}



	}



	TagTableAppend( tagTable, TagEntryAlloc( "image vfb", POINTER, &vfb ) );



	/*

	 * Output -verbose message

	 */

	ImInfo( "Byte Order", "Most Significant Byte First" );



	sprintf( message, "%d x %d",  IMMAC_IMAGE_WIDTH,  IMMAC_IMAGE_HEIGHT );

	ImInfo( "Resolution", message );



	ImInfo( "Type", "1-bit Monochrome" );

	ImInfo( "Compression Type", "Apple Macintosh Packbits" );



	return ( 1 );

}











/*

 *  FUNCTION

 *	imMpntWrite	-  write an Apple Macintosh MacPaint file

 *

 *  DESCRIPTION

 *	Write a monochrome MacPaint image.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imMpntWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imMpntWrite( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int		ioType;		/* I/O flags			*/

	int		fd;		/* Input file descriptor	*/

	FILE		* fp;		/* Input file pointer		*/

	TagTable	* flagsTable;	/* Flags			*/

	TagTable	* tagTable;	/* Tag table to read from	*/

#endif

{

	ImVfb		*vfb;		/* Image to output		*/

	ImVfbPtr	pPixel;		/* Pixel pointer		*/

	char		* buf[ IMMAC_HEADER_SIZE ];	/* MacPaint file header */

	unsigned char		packed[  IMMAC_MAX_UNPACKED_SIZE ];

	unsigned char		unpacked[  IMMAC_IMAGE_WIDTH ];

	unsigned char		*pPtr;

	unsigned short		sLine, bit, bitVal;

	unsigned long		byteCount;

	unsigned int		count, oldcount;

	unsigned short		col;

	int		n;

	int		vfbHeight, vfbWidth;

	int		mono_threshold;

	char		 message[100];	/* ImInfo message		*/





	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	vfbHeight = ImVfbQHeight( vfb );

	vfbWidth  = ImVfbQWidth( vfb );



	BinByteOrder( BINMBF );



	/*

	 *  Write out the 512 byte (blank) header.

	 */

	memset( buf, 0x00, IMMAC_HEADER_SIZE );

	if ( ImBinWrite( ioType, fd, fp, buf, UCHAR, 1, IMMAC_HEADER_SIZE ) == -1 )

	{

		ImReturnBinError();

	}



	/*

	 * Output -verbose message

	 */

	ImInfo( "Byte Order", "Most Significant Byte First" );



	sprintf( message, "%d x %d",  IMMAC_IMAGE_WIDTH,  IMMAC_IMAGE_HEIGHT );

	ImInfo( "Resolution", message );



	ImInfo( "Type", "1-bit Monochrome" );

	ImInfo( "Compression Type", "Apple Macintosh Packbits" );



	/*

	 *  Write the VFB out one scanline at a time.  If the image is taller

	 *  than a MacPaint file allows, clip off the bottom.  If smaller,

	 *  pad with zeroes.

	 */

	if ( vfbWidth >  IMMAC_IMAGE_WIDTH )

	{

		ImErrorWarning( "Image too wide for MacPaint format.  Image has been clipped.", -1, IMEWIDTH );

		vfbWidth =  IMMAC_IMAGE_WIDTH;

	}

	if ( vfbHeight >  IMMAC_IMAGE_HEIGHT )

	{

		ImErrorWarning( "Image too tall for MacPaint format.  Image has been clipped.", -1, IMEHEIGHT );

		vfbHeight =  IMMAC_IMAGE_HEIGHT;

	}

	for ( sLine=0; sLine<vfbHeight; sLine++ )

	{

		/*

		 *  Copy the VFB, left justified.  If the image is wider

		 *  than  IMMAC_IMAGE_WIDTH, it is truncated.  If it is smaller

		 *  than  IMMAC_IMAGE_WIDTH, it is zero-padded.

		 */

		pPixel = ImVfbQPtr( vfb, 0, sLine );

		for ( col=0; col<vfbWidth; col++ )

		{

			if ( ImVfbQMono( vfb, pPixel ) ==  IMMAC_BLACK )

				IMsetBit( &unpacked[ col / 8 ], 7 - (col % 8) );

			else

				IMunsetBit( &unpacked[ col / 8 ], 7 - (col % 8) );

			ImVfbSInc( vfb, pPixel );

		}



		/*

		 *  Zero pad anything extra.

		 */

		for ( ; col< IMMAC_IMAGE_WIDTH; col++ )

			IMunsetBit( &unpacked[ col / 8 ], 7 - (col % 8) );



		count =  IMMAC_ROW_BYTES;

		PackBits( unpacked, packed, &count );

		if ( ImBinWrite( ioType, fd, fp, packed, UCHAR, 1, count )== -1)

		{

			ImReturnBinError();

		}

	}



	/* IF THE IMAGE WAS SHORTER THAN A MACPAINT IMAGE, */

	/* THEN FILL THE SCANLINES WITH IMMAC_WHITE */

	memset( unpacked,  0x00, IMMAC_ROW_BYTES );

	count =  IMMAC_ROW_BYTES;

	PackBits( unpacked, packed, &count );

	for ( ; sLine< IMMAC_IMAGE_HEIGHT; sLine++ )

	{

		if ( ImBinWrite( ioType, fd, fp, packed, UCHAR, 1, count )== -1)

		{

			ImReturnBinError();

		}

	}



	return ( 1 );

}





