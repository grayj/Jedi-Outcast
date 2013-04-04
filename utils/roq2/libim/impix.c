/**

 **	$Header: /roq/libim/impix.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER        "    $Header: /roq/libim/impix.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	impix.c		-  Alias PIX file I/O

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	impix.c contains routines to read and write Alias PIX files for

 **	the image manipulation library.  Raster data read in is stored

 **	in a VFB.  Raster data written out is taken from a tag table.

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **	none

 **

 **  PRIVATE CONTENTS

 **	imPixRead	f  read an Alias PIX file

 **	imPixWrite	f  write an Alias PIX file

 **

 **	imPixHeaderInfo		t  PIX file header information

 **	imPixHeaderFields	v  imPixHeaderInfo description for Bin pkg

 **

 **  HISTORY

 **	$Log: /roq/libim/impix.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.26  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.25  1995/04/03  21:34:19  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.24  1995/02/16  21:41:24  bduggan

 **	Added support for Matte (index8) files

 **

 **	Revision 1.23  1995/01/16  22:53:39  bduggan

 **	fixed spacing in code

 **

 **	Revision 1.22  1995/01/10  23:39:57  bduggan

 **	put in IMMULTI, IMPIPE instead of TRUE/FALSE

 **	made read/write routines static

 **

 **	Revision 1.21  94/10/03  11:30:43  nadeau

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

 **	Revision 1.20  93/09/22  11:41:23  nadeau

 **	Corrected spelling error in message.

 **	

 **	Revision 1.19  92/12/03  01:51:14  nadeau

 **	Corrected info messages.

 **	.,

 **	

 **	Revision 1.18  92/11/23  18:43:02  nadeau

 **	Removed use of IMINFOMSG.

 **	

 **	Revision 1.17  92/11/04  12:05:30  groening

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.16  92/10/16  11:41:07  groening

 **	added ImInfo statements

 **	

 **	Revision 1.15  92/08/31  17:32:34  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.14  91/10/04  10:52:01  nadeau

 **	Fixed VAX bit shift problem.

 **	

 **	Revision 1.13  91/10/03  09:15:50  nadeau

 **	Fixed #includes.

 **	

 **	Revision 1.12  91/03/08  14:31:04  nadeau

 **	Ooops!  Forget to get the VFB before trying to write it out!

 **	Major core dump time.

 **	

 **	Revision 1.11  91/02/12  10:52:58  nadeau

 **	Removed the tag table checking and VFB conversion now

 **	handled by ImFileRead and ImFileWrite.

 **	

 **	Revision 1.10  90/12/03  12:47:39  nadeau

 **	Fixed read bug that would cause memory to be overwritten if

 **	the number of bytes in the file was not a multiple of 4 (ie,

 **	there were extra bytes at the end).

 **	

 **	Revision 1.9  90/07/02  13:21:02  nadeau

 **	Updated to the new error handling mechanism.

 **	

 **	Revision 1.8  90/06/25  14:40:57  nadeau

 **	Changed ImTag* to Tag* (new names).

 **	

 **	Revision 1.7  90/06/25  13:20:54  todd

 **	Fixed a few little bugs.

 **	

 **	Revision 1.6  90/05/16  07:46:32  todd

 **	Move some macros into new include file iminternal.h for shared use

 **	

 **	Revision 1.5  90/05/11  09:55:19  nadeau

 **	Updated call interface to handle file descriptors and file pointers.

 **	Updated code to watch for both of these and make the correct bin pkg

 **	call.

 **	

 **	Revision 1.4  90/04/05  17:07:21  todd

 **	Take out code that checks if input is a pipe.  BinRead as much as

 **	possible.  numbytes_read/4 is the number of runs.

 **	Change pixel arrays from ints to arrays of unsigned chars for portability.

 **	

 **	Revision 1.3  90/03/29  08:29:28  todd

 **	Changed some ints to unsigned ints.  PIX Read and Write routines seem

 **	to work fine.  

 **	

 **	Revision 1.2  90/03/28  11:13:53  nadeau

 **	Lots of fiddling and bug fixing.  Still doesn't work, but Todd

 **	has graciously volunteered to work on it now!

 **	

 **	Revision 1.1  90/03/06  17:32:24  nadeau

 **	Initial revision

 **	

 **/





#include "iminternal.h"



/*

 **

 **

 **  FORMAT

 **

 ** 	PIX - Alias Pixel file

 **

 **  AKA

 **	.alias

 **

 **  FORMAT REFERENCES

 **	 Alias Reference Manual, Alias Research, Inc.

 **

 **  CODE CREDITS

 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1990.

 **     Custom development, Todd Elvins, San Diego Supercomputer Center, 1990.

 **

 **  DESCRIPTION

 **	Alias' pix file has a 10-byte header followed by run-length-encoded

 **	image data.

 **

 **	Header

 **	======

 **

 **	The header for a pix file has the following information:

 **

 **	bytes:      		description:

 **	------			-------------

 **	0,1			width (x resolution in pixels)

 **	2,3			height (y resolution in pixels)

 **     4,5			xoffset (unused)

 **     6,7 			yoffset (unused)

 **     8,9		    	bits per pixel:

 **				0x18 for pix files

 **				0x08 for matte files (greyscale)

 **

 **	Encoding of Pixel Data

 **     ======================

 **

 ** 	The pixels are encoded in 4-byte packages, starting with the top

 **	scanline.  Runs do not extend beyond the end of a scanline.  The

 **	packages have the following format:

 **

 **	byte:		data range:	description

 **	-----		-----------	-----------

 **	1		1-255		runlength (number of pixels in

 **					succession with given RGB)

 **	2		0-255		blue (value of blue component)

 **	3		0-255		green (green component)

 **	4		0-255		red (red component)

 **

 **	Example

 **	=======

 **	Here is the hex of a file that is 8 pixels wide and 6 pixels high,

 **	representing a ramp that goes from black at the bottom of the image to

 **	blue at the top:

 **

 **	0008 0006 0000 0005 0018 08ff 0000 08cc

 **	0000 0899 0000 0866 0000 0833 0000 0000

 **	0000	

 **	

 **	The header is 0008 0006 0000 0005 0018 

 ** 	The first line is a run of eight pixels of B=0xff (255), G=0, R=0,

 **	The second line is a run of eight pixels of B=0xcc (204), G=0, R=0

 **	etc... 

 ** 

 **

 **

 **/



#ifdef __STDC__

static int imPixRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imPixWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imPixReadIndex8( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, int x, int y );

static int imPixWriteIndex8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable

);

#else

static int imPixRead( );

static int imPixWrite( );

static int imPixReadIndex8( );

static int imPixWriteIndex8( );

#endif



static char *imPixNames[ ]  = { "pix", "alias", NULL };

static ImFileFormatReadMap imPixReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IMVFBRGB,3,8,      RLE,    IMVFBRGB,       0 },

        { IMVFBINDEX8,1,8,   RLE,    IMVFBRGB,       0 },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imPixWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBRGB,     0,      RGB,3,8,        RLE,    imPixWrite },

	{ IMVFBINDEX8,  0,      IN,1,8,         RLE,    imPixWriteIndex8 },

        { -1,           0,      -1,             0,      NULL },

};



static ImFileMagic imFilePixMagic []=

{

	{ 0, 0, NULL},

};



ImFileFormat ImFilePixFormat =

{

	imPixNames, "Alias image file",

	"Alias Research, Inc.",

	"24-bit RGB RLE-compressed images, 8-bit greyscale RLE_compressed images",

	"24-bit RGB RLE-compressed images, 8-bit greyscale RLE_compressed images",

	imFilePixMagic,

	IMNOMULTI, IMPIPE,

	IMNOMULTI, IMPIPE,

	imPixRead, imPixReadMap, imPixWriteMap

};







/*

 *  TYPEDEF & STRUCTURE

 *	imPixHeaderInfo		-  PIX file header information

 *	imPixHeaderFields	-  PIX file header fields for binary pkg.

 *

 *  DESCRIPTION

 *	A PIX file's header contains the image's width, height, and depth,

 *	and two unused dummy fields.

 */



typedef struct imPixHeaderInfo

{

	short pix_width;		/* Image width			*/

	short pix_height;		/* Image height			*/

	short pix_dummy1;		/* Unused dummy field		*/

	short pix_dummy2;		/* Unused dummy field		*/

	short pix_depth;		/* Image depth			*/

} imPixHeaderInfo;



static BinField imPixHeaderFields[ ] =

{

	{ SHORT, 2, 1 },		/* pix_width			*/

	{ SHORT, 2, 1 },		/* pix_height			*/

	{ SHORT, 2, 1 },		/* pix_dummy1			*/

	{ SHORT, 2, 1 },		/* pix_dummy1			*/

	{ SHORT, 2, 1 },		/* pix_depth			*/

	{ 0, 0, 0 }

};











/*

 *  FUNCTION

 *	imPixRead	-  read an Alias PIX file

 *

 *  DESCRIPTION

 *	The file header is read and the size of the image determined.

 *	Space is allocated for the VFB and the run codes read in to

 *	a run buffer.  The run buffer is then expanded into straight

 *	RGB values in the VFB and the completed VFB added to the tag list.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__

imPixRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imPixRead( ioType, fd, fp, flagsTable, tagTable )

	int         ioType;		/* I/O flags			*/

	int         fd;			/* Input file descriptor	*/

	FILE	   *fp;			/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable   *tagTable;		/* Tag table to add to		*/

#endif

{

	ImVfb            *vfb;		/* Read in image		*/

	ImVfbPtr         pptr;		/* Pixel pointer		*/

	imPixHeaderInfo  header;	/* PIX file header		*/

	unsigned char    *runBuffer;	/* Run buffer			*/

	unsigned char    *rbp;		/* Run buffer pointer		*/

	unsigned char    r, g, b;	/* Red, green, and blue		*/

	long             seekPtr;	/* File seek pointer		*/

	int              nRead;		/* Number of pixel runs		*/

	int	         x,y;		/* Convenient short names	*/

	unsigned int 	 count;		/* Run length			*/

	char		 message[100];	/* ImInfo message		*/





	/*

	 *  PIX files are usually generated on Irises, which have an MBF

	 *  byte order.  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINMBF );



	ImInfo ("Byte Order","Most Significant Byte First");

	/*

	 *  Read in the header.

	 */

	if ( ImBinReadStruct( ioType, fd, fp, &header, imPixHeaderFields )== -1)

	{

		ImReturnBinError( );

	}

	x = header.pix_width;

	y = header.pix_height;



	if (header.pix_depth==0x08)

	{ 	/* Greyscale image */

		return imPixReadIndex8(ioType, fd, fp, flagsTable, tagTable, x, y);

	}



	sprintf (message, "%d x %d", x,y);

	ImInfo ("Resolution", message);

	ImInfo ( "Type", "24-bit RGB");

	ImInfo ( "Compression Type", "Run Length Encoded (RLE)");





	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (vfb = ImVfbAlloc( x, y, IMVFBRGB )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}





	/*

	 * Allocate a run buffer large enough for the worst case run

	 * length encoding.

	 */ 

	ImMalloc( runBuffer, unsigned char *, 4 * x * y );





	/*

	 * Read the entire image into the run buffer.  The number of bytes

	 * read divided by 4 is the number of runs.

	 */

	if ( (nRead = ImBinRead( ioType, fd, fp, runBuffer, UCHAR, 1, 4*x*y )) == -1 )

	{

		free( (char *)runBuffer );

		ImReturnBinError( );

	}



	/*

	 *  Expand the runs to create a raw RGB image in the VFB.

	 */

	pptr = ImVfbQFirst( vfb );

	for ( rbp = runBuffer; nRead > 3; nRead-=4, rbp+=4 )

	{

		r = *(rbp+3);

		g = *(rbp+2);

		b = *(rbp+1);

		for ( count = *rbp; count > 0; count-- )

		{

			ImVfbSRed(   vfb, pptr, r );

			ImVfbSGreen( vfb, pptr, g );

			ImVfbSBlue(  vfb, pptr, b );

			ImVfbSInc(   vfb, pptr );

		}

	}

	free( (char *)runBuffer );





	/*

	 *  Add the VFB to the tagTable.

	 */

	TagTableAppend( tagTable, TagEntryAlloc( "image vfb", POINTER, &vfb ) );



	return ( 1 );

}











/*

 *  FUNCTION

 *	imPixWrite	-  write an Alias PIX file

 *

 *  DESCRIPTION

 *	The PIX file header set up and written out.  The VFB data is then

 *	read, converted to run-codes, and written out.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imPixWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imPixWrite( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	ImVfb           *vfb;		/* Read in image		*/

	ImVfbPtr        pptr;		/* Pixel pointer		*/

	imPixHeaderInfo header;		/* PIX file header		*/

	unsigned char   *runBuffer;	/* Run buffer			*/

	unsigned char   *rbp;		/* Run buffer pointer		*/

	unsigned int    count;		/* Run length			*/

	int             nRuns;		/* Number of pixel runs		*/

	int             n;		/* Counter			*/

	int             x, y;		/* Pixel counters		*/

	int             rgb;		/* New rgb value		*/

	int             oldrgb;		/* Old rgb value		*/

	char		message[100];	/* ImInfo message		*/





	/*

	 *  PIX files are usually generated on Irises, which have an MBF

	 *  byte order.  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINMBF );





	/*

	 *  Set up the header and write it out.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	header.pix_width  = ImVfbQWidth( vfb );

	header.pix_height = ImVfbQHeight( vfb );

	header.pix_dummy1 = 0;			

	header.pix_dummy2 = header.pix_height - 1;

	header.pix_depth  = 24;



	if ( ImBinWriteStruct( ioType, fd, fp, &header, imPixHeaderFields )==-1)

	{

		ImReturnBinError( );

	}



	sprintf (message, "%d x %d", header.pix_width, header.pix_height);

	ImInfo ("Resolution", message);

	ImInfo ( "Type", "24-bit RGB");

	ImInfo ( "Compression Type", "Run Length Encoded (RLE)");





	/*

	 *  Allocate space for a run buffer large enough for 1 unencoded

	 *  scanline.  An encoded scanline will be smaller.

	 */

	if ( (runBuffer = (unsigned char *)malloc( 4 * header.pix_width )) == NULL )

	{

		ImErrNo = IMEMALLOC;

		ImErrorFatal( ImQError( ), -1, IMEMALLOC );

	}





	/*

	 *  Run-length encode the VFB.  In PIX files, run's always stop

	 *  at the end of a scanline, even if the start of the next scanline

	 *  is the same color.

	 */

	pptr  = ImVfbQFirst( vfb );

	for ( y = 0; y < header.pix_height; y++ )

	{

		rbp    = runBuffer;

		nRuns  = 0;

		oldrgb = (((unsigned int)ImVfbQBlue(  vfb, pptr )) << 16) |

			 (((unsigned int)ImVfbQGreen( vfb, pptr )) << 8) |

			   (unsigned int)ImVfbQRed(   vfb, pptr );

		ImVfbSInc( vfb, pptr );

		count  = 1;

		for ( x = 1; x < header.pix_width; x++ )

		{

			rgb = (((unsigned int)ImVfbQBlue(  vfb, pptr )) << 16) |

			      (((unsigned int)ImVfbQGreen( vfb, pptr )) << 8) |

			        (unsigned int)ImVfbQRed(   vfb, pptr );

			ImVfbSInc( vfb, pptr );



			if ( rgb == oldrgb )

			{

				/* Add 1 to the run.			*/

				if ( ++count == 255 )

				{

					/* Overflow 1 byte.  Dump run.	*/

					*rbp++ = 255;

					*rbp++ = (oldrgb >> 16) & 0xFF;

					*rbp++ = (oldrgb >>  8) & 0xFF;

					*rbp++ = (oldrgb) & 0xFF;

					nRuns++;

					count = 0;

				}

			}

			else

			{

				/* Different color.  Dump run.		*/

				if ( count != 0 )

				{

					*rbp++ = count; 

					*rbp++ = (oldrgb >> 16) & 0xFF;

					*rbp++ = (oldrgb >>  8) & 0xFF;

					*rbp++ = (oldrgb) & 0xFF;

					nRuns++;

				}

				count  = 1;

				oldrgb = rgb;

			}

		}

		if ( count != 0 )

		{

			*rbp++ = count;

			*rbp++ = (oldrgb >> 16) & 0xFF;

			*rbp++ = (oldrgb >>  8) & 0xFF;

			*rbp++ = (oldrgb) & 0xFF;

			nRuns++;

		}



		/*

		 *  Write out the run buffer.

		 */

		if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, nRuns * 4 ) == -1 )

		{

			free( (char *)runBuffer );

			ImReturnBinError( );

		}

	}





	/*

	 *  Add a terminating run with a count of 0 and write it out.

	 */

	*runBuffer = 0;			/* Terminate the runs		*/

	if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, 4 ) == -1 )

	{

		free( (char *)runBuffer );

		ImReturnBinError( );

	}



	free( (char *)runBuffer );



	return ( 1 );

}







/*

 *  FUNCTION

 *	imPixReadIndex8	-  read an Alias PIX file

 *

 *  DESCRIPTION

 *	The file header has been read and the size of the image has

 *      been determined by imPixRead.

 *	Here, space is allocated for the VFB and the run codes read in to

 *	a run buffer.  The run buffer is then expanded into straight

 *	greyscale values in the VFB and the completed VFB added to the tag list.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__

imPixReadIndex8( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, int x, int y )

#else

imPixReadIndex8( ioType, fd, fp, flagsTable, tagTable, x, y )

	int         ioType;		/* I/O flags			*/

	int         fd;			/* Input file descriptor	*/

	FILE	   *fp;			/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable   *tagTable;		/* Tag table to add to		*/

	int x;				/* Dimensions of the image      */

	int y;

#endif

{

	ImVfb              *vfb;	/* Read in image		*/

	ImVfbPtr           pptr;	/* Pixel pointer		*/

	unsigned char      *runBuffer;	/* Run buffer			*/

	unsigned char      *rbp;	/* Run buffer pointer		*/

	unsigned char      g;		/* Gray value                   */

	long               seekPtr;	/* File seek pointer		*/

	int                nRead;	/* Number of bytes read         */

	unsigned int 	   count;	/* Run length			*/

	char		   message[100];/* ImInfo message		*/





	/* The byte order has been set already by imReadPix */



	sprintf (message, "%d x %d", x,y);

	ImInfo ("Resolution", message);

	ImInfo ( "Type", "8-bit Grayscale");

	ImInfo ( "Compression Type", "Run Length Encoded (RLE)");





	/*

	 *  Allocate a VFB of the required size.

	 */



	if ( (vfb = ImVfbAlloc( x, y, IMVFBINDEX8 )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}





	/*

	 * Allocate a run buffer large enough for the worst case run

	 * length encoding.

	 */ 



	ImMalloc( runBuffer, unsigned char *, 2 * x * y );





	/*

	 * Read the entire image into the run buffer.  The number of bytes

	 * read divided by 2 is the number of runs.

	 */



	if ( (nRead = ImBinRead( ioType, fd, fp, runBuffer, UCHAR, 1, 2*x*y )) == -1 )

	{

		free( (char *)runBuffer );

		ImReturnBinError( );

	}



	/*

	 *  Expand the runs to create a grayscale image in the VFB.

	 */



	pptr = ImVfbQFirst( vfb );

	for ( rbp = runBuffer; nRead > 1; nRead-=2, rbp+=2 )

	{

		g = *(rbp+1);

		for ( count = *rbp; count > 0; count-- )

		{

			ImVfbSIndex8(vfb, pptr, g );

			ImVfbSInc(   vfb, pptr );

		}

	}

	free( (char *)runBuffer );





	/*

	 *  Add the VFB to the tagTable.

	 */

	TagTableAppend( tagTable, TagEntryAlloc( "image vfb", POINTER, &vfb ) );



	return ( 1 );

}









/*

 *  FUNCTION

 *	imPixWriteIndex8	-  write an Alias PIX file

 *

 *  DESCRIPTION

 *	The Matte file header set up and written out.  The VFB data is then

 *	read, converted to run-codes, and written out.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imPixWriteIndex8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imPixWriteIndex8( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	ImVfb             *vfb;		/* Read in image		*/

	ImVfbPtr          pptr;		/* Pixel pointer		*/

	imPixHeaderInfo header;		/* PIX file header		*/

	unsigned char     *runBuffer;	/* Run buffer			*/

	unsigned char     *rbp;		/* Run buffer pointer		*/

	unsigned int      count;	/* Run length			*/

	int               nRuns;	/* Number of pixel runs		*/

	int               n;		/* Counter			*/

	int               x, y;		/* Pixel counters		*/

	int               grey;		/* New greyscale value		*/

	unsigned char     oldgrey;	/* Old greyscale value          */

	char		  message[100];	/* ImInfo message		*/





	/*

	 *  PIX files are usually generated on Irises, which have an MBF

	 *  byte order.  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINMBF );





	/*

	 *  Set up the header and write it out.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	header.pix_width  = ImVfbQWidth( vfb );

	header.pix_height = ImVfbQHeight( vfb );

	header.pix_dummy1 = 0;			

	header.pix_dummy2 = header.pix_height - 1;

	header.pix_depth  = 8;



	if ( ImBinWriteStruct( ioType, fd, fp, &header, imPixHeaderFields )==-1)

	{

		ImReturnBinError( );

	}



	sprintf (message, "%d x %d", header.pix_width, header.pix_height);

	ImInfo ("Resolution", message);

	ImInfo ( "Type", "8-bit Grayscale");

	ImInfo ( "Compression Type", "Run Length Encoded (RLE)");





	/*

	 *  Allocate space for a run buffer large enough for 1 unencoded

	 *  scanline.  An encoded scanline will be smaller.

	 */

	ImMalloc( runBuffer, unsigned char *, 2 * header.pix_width);





	/*

	 *  Run-length encode the VFB.  In Matte files, runs always stop

	 *  at the end of a scanline, even if the start of the next scanline

	 *  is the same color.

	 */

	pptr  = ImVfbQFirst( vfb );

	for ( y = 0; y < header.pix_height; y++ )

	{

		rbp     = runBuffer;

		nRuns   = 0;

		oldgrey = ImVfbQIndex8( vfb, pptr);

		ImVfbSInc( vfb, pptr );

		count  = 1;

		for ( x = 1; x < header.pix_width; x++ )

		{

			grey = ImVfbQIndex8( vfb, pptr );

			ImVfbSInc( vfb, pptr );



			if ( grey == oldgrey )

			{

				/* Add 1 to the run.			*/

				if ( ++count == 255 )

				{

					/* Overflow 1 byte.  Dump run.	*/

					*rbp++ = 255;

					*rbp++ = oldgrey;

					nRuns++;

					count = 0;

				}

			}

			else

			{

				/* Different color.  Dump run.		*/

				if ( count != 0 )

				{

					*rbp++ = count; 

					*rbp++ = oldgrey;

					nRuns++;

				}

				count  = 1;

				oldgrey = grey;

			}

		}

		if ( count != 0 )

		{

			*rbp++ = count;

			*rbp++ = oldgrey;

			nRuns++;

		}



		/*

		 *  Write out the run buffer.

		 */

		if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, nRuns * 2 ) == -1 )

		{

			free( (char *)runBuffer );

			ImReturnBinError( );

		}

	}





	/*

	 *  Add a terminating run with a count of 0 and write it out.

	 */

	*runBuffer = 0;			/* Terminate the runs		*/

	if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, 2 ) == -1 )

	{

		free( (char *)runBuffer );

		ImReturnBinError( );

	}



	free( (char *)runBuffer );



	return ( 1 );

}

