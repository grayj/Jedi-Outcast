/**

 **     $Header: /roq/libim/imcur.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER  "	$Header: /roq/libim/imcur.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imcur.c		-  Microsoft Windows CUR file I/O

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imcur.c contains routines to read and write Microsoft Windows CUR

 **	files for the image manipulation library.  Raster data read

 **	in is stored in a VFB.  Raster data written out is taken from a

 **	tag table.

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **	ImFileCurFormat		v  file format information

 **

 **  PRIVATE CONTENTS

 **	imCurHeader		t  file header information

 **	imCurHeaderFields	v  imCurHeader description for Bin pkg.

 **

 **	imCurDirEntry		t  cursor directory entry

 **	imCurDirEntryFields	v  imCurDirEntry description for Bin pkg.

 **

 **	imCurInfo		t  cursor information

 **	imCurInfoFields		v  imCurInfo description for Bin pkg.

 **

 **	imCurRead		f  read CUR file

 **	imCurImageRead		f  read the ith cursor image

 **	imCurImageReadVfb	m  read the ith cursor image

 **

 **	imCurWriteRaw1		f  write 1-bit uncompressed CUR file

 **

 **  HISTORY

 **     $Log: /roq/libim/imcur.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.12  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.11  1995/06/15  20:16:03  bduggan

 **	took out unused variables.  Took out commands at unreachable code.

 **	Removed magic number since it conflicted with tga's.

 **

 **	Revision 1.10  1995/05/16  22:11:11  bduggan

 **	Took \n out of iminfo's

 **	made HotSpot a static variable (since its address is used

 **	in the tag table)

 **

 **	Revision 1.9  1995/04/03  21:21:49  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.8  94/10/03  11:30:02  nadeau

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

 **	Revision 1.7  92/12/03  01:46:29  nadeau

 **	Corrected info messages.

 **	

 **	Revision 1.6  92/11/24  11:47:02  groening

 **	Removed use of IMINFOMSG.

 **	

 **	Revision 1.5  92/11/18  12:28:53  groening

 **	fixed error in offset of image

 **	

 **	Revision 1.4  92/11/04  11:46:19  groening

 **	added multiple read and write.

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.3  92/10/12  15:54:17  vle

 **	Made changes to make Cray happy.

 **	

 **	Revision 1.2  92/09/29  17:59:03  vle

 **	Added ImInfo messages.

 **	

 **	Revision 1.1  92/09/17  14:31:40  vle

 **	Initial revision

 **	

 **	

 **/



#include "iminternal.h"





/**

 **  FORMAT

 **	cur	-  Microsoft Windows cursor image

 **

 **  AKA

 **

 **  FORMAT REFERENCES

 **	Microsoft Windows 3.1 Programmer's Reference, volume 3, Microsoft

 **	Press, 1992.

 **

 **	Microsoft Windows 3.1 Programmer's Reference, volume 3, Microsoft

 **	Press, 1992.

 **

 **  CODE CREDITS

 **	Custom development, Vinh Le, San Diego Supercomputer Center, 1992.

 **	Extensions, Dave Nadeau, San Diego Supercomputer Center, 1993.

 ** 

 **  DESCRIPTION

 **	Microsoft's CUR (CURsor) format is used in Microsoft's Windows

 **	versions 3.0 and up for storage of one or more cursors.

 **

 **	The first item encountered in an CUR file is a Cursor Directory

 **	Header.  This tells that the file is indeed a file containing

 **	cursors and that there are n number of them.

 **

 **	Following the Cursor Directory Header is an Cursor Directory

 **	Entry for each cursor.  A Cursor Directory Entry contains

 **	information on the cursor's location within the file,

 **	its width, height, etc.  There are n Cursor Directory Entries.

 **

 **	Following all this is the actual Cursor Images themselves, which

 **	consist of an info header, a colortable, and XOR and AND masks.

 **	The XOR mask could be considered the "real" cursor.  The AND

 **	mask determines the XOR image's "transparency".  XOR masks

 **	are 1-bit/pixel.  1-bit/pixel images are packed 8 pixels/byte.

 **	AND masks are always monochrome images, thus they are

 **	packed as 1-bit/pixel images, or 8-bits/byte.

 **

 **

 **	Stucture Overview

 **	-----------------

 **

 **	CURSOR DIRECTORY

 **	---------------------------------------------------------

 **	|CURSOR	|CURSOR	|CURSOR	|  ...	|	|	|	|

 **	|ENTRY 1|ENTRY 2|ENTRY 3|	|	|	|	|

 **	---------------------------------------------------------

 **	    |

 **	    |		---------

 **	    +---------> |CURSOR	|

 **			|IMAGE  |

 **			|-------|

 **			|Header |

 **			|-------|

 **	    		|Color	|

 **			|Table	|

 **			|-------|

 **			|XOR	|

 **			|Mask	|

 **			|-------|

 **			|AND	|

 **			|Mask	|

 **			---------

 **

 **

 **	Cursor Directory Header (6 bytes+ 16 bytes/cursor entry)

 **	-----------------------

 **

 **	The Cursor Directory Header contains the resource type and

 **	number of cursors in the Cursor Directory.

 **

 **	Name		Type		Size		Comments

 **	----		----		----		--------

 **	cdReserved	word		2 bytes		reserved, must be 0

 **	cdType		word		2 bytes		resource type,

 **							  must be 2

 **	cdCount		word		2 bytes		number of cursors

 **	cdEntries[]	CurosrDirEntry	16 bytes	array of info for

 **					  per entry	  individual cursors

 **

 **	typedef struct CURSORDIR

 **	{

 **		word		cdReserved;

 **		word		cdType;

 **		word		cdCount;

 **		CURSORDIRENTRY	cdEntries[cdCount];

 **	} CURSORDIR;

 **

 **

 **	Cursor Directory Entry (16 bytes/cursor entry)

 **	----------------------

 **

 **	An Cursor Directory Entry stores information about the an cursor's

 **	dimensions and color attributes.

 **

 **	Name		Type	Size	Comments

 **	----		----	----	--------

 **	Width		byte	1 byte	width in pixels

 **	Height		byte	1 byte	height in pixels

 **	ColorCount	byte	1 byte	reserved, must be 0

 **	Reserved	byte	1 byte	reserved, must be 0

 **	XHotspot	word	2 byte	x coordinate of hotspot

 **	YHotspot	word	2 byte	y coordinate of hotspot

 **	BytesInRes	dword	4 byte	size of cursor in bytes

 **	ImageOffset	dword	4 byte	offset from beginning of file to

 **					  Cursor Image

 **

 **	typedef struct CURSORDIRENTRY

 **	{

 **		byte	Width;

 **		byte	Height;

 **		byte	ColorCount;

 **		byte	Reserved;

 **		word	XHotspot;

 **		word	YHotspot;

 **		dword	BytesInRes;

 **		dword	ImageOffset;

 **	} CURSORDIRENTRY;

 **

 **

 **	Cursor Image (40 bytes + 4 bytes/color entry + sizeof XOR mask +

 **	------------  sizeof AND mask)

 **

 **	Each Cursor Image stores 1 cursor.

 **

 **	Name		Type			Size		Comments

 **	----		----			----		--------

 **	crHeader	BITMAPFILEHEADER	40 bytes	only biSize

 **								  through

 **								  biBitCount

 **								  and

 **								  biSizeImage

 **								  are used

 **                                                             biHeight =

 **                                                               2*XOR Mask

 **                                                               height

 **                                                             biPlanes = 1

 **                                                             biBitCount = 1

 **                                                             all other

 **                                                               fields = 0

 **	crColors[]	RGBQUAD			4 bytes per	colortable used

 **						  color		  in XOR mask

 **	crXOR[]		byte			1 byte per	XOR image mask

 **						  element

 **	crAND[]		byte			1 byte per	AND image mask

 **						  element

 **

 **

 **     Cur-Info Header (40 bytes)

 **     ------------------

 ** 

 **     The Cur-Info Header contains information about the cursor's

 **     size and dimensions.

 ** 

 **     Name            Type    Size            Comments

 **     ----            ----    ----            --------

 **     biSize          dword   4 bytes         number of bytes in

 **                                             Cur-Info Header

 **     biWidth         long    4 bytes         width, in pixels

 **     biHeight        long    4 bytes         height, in pixels

 **     biPlanes        word    2 bytes         number of planes for the

 **                                             display device, must be 1

 **     biBitCount      word    2 bytes         bits per pixel, must be 1

 **     biCompression   dword   4 bytes         unused, must be 0

 **     biSizeImage     dword   4 bytes         image size in bytes

 **     biXPelsPerMeter long    4 bytes         unused, must be 0

 **     biYPelsPerMeter long    4 bytes         unused, must be 0

 **     biClrUsed       dword   4 bytes         unused, must be 0

 **     biClrImportant  dword   4 bytes         unused, must be 0

 ** 

 **     typedef struct tagBITMAPINFOHEADER

 **	{ 

 **             dword   biSize;

 **             long    biWidth;

 **             long    biHeight;

 **             word    biPlanes;

 **             word    biBitCount;

 **             dword   biCompression;

 **             dword   biSizeImage;

 **             long    biXPelsPerMeter;

 **             long    biYPelsPerMeter;

 **             dword   biClrUsed;

 **             dword   biClrImportant;

 **     } BITMAPINFOHEADER;

 **

 **

 **     ColorTable (4 bytes per color)

 **     ----------

 ** 

 **     ColorTable is an array of RGBQuad's.  Each RGBQuad contains color

 **     intensity values for Red, Green, and Blue.

 **

 **     RGBQuad

 **     -------

 ** 

 **     Name            Type    Size            Comment

 **     ----            ----    ----            -------

 **     rgbBlue         byte    1 byte          intensity of blue

 **     rgbGreen        byte    1 byte          intensity of green

 **     rgbRed          byte    1 byte          intensity of red

 **     rgbReserved     byte    1 byte          unused

 ** 

 **     typedef struct tagRGBQUAD

 **	{

 **        	byte    rgbBlue;

 **             byte    rgbGreen;

 **             byte    rgbRed;

 **             byte    rgbReserved;

 **     } RGBQUAD;

 ** 

 **/





/*

 *  FUNCTION DECLARATIONS

 */

#ifdef __STDC__

static int imCurRead( int, int, FILE *, TagTable *, TagTable * );

static int imCurWriteRaw1( ImFileFormatWriteMap *, int, int, FILE *,

	TagTable *, TagTable * );

#else

static int imCurRead( );

static int imCurWriteRaw1( );

#endif











/*

 *  FORMAT INFORMATION

 *	imCurNames		-  format's name and aliases

 *	imCurReadMap		-  read attributes

 *	imCurWriteMap		-  write attributes

 *	imCurMagicNumber	-  magic number

 *	imCurMagic		-  list of magic numbers

 *	ImFileCurFormat		-  master format description

 */

static char *imCurNames[ ] = { "cur", NULL };

static ImFileFormatReadMap imCurReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IN,1,1,       C,      IMVFBINDEX8,    C|A },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imCurWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBMONO,    C|A,    IN,1,1,         C,      imCurWriteRaw1 },

        { -1,           0,      -1,             0,      NULL },

};



/*

 * Magic number:

 *    The CUR magic number conflicts with

 * TGA (targa) rgb files.  So, I've left

 * out the magic number so tga's can be

 * read safely.

 *

 *    The effect of this is not drastic:

 * It just means that cur's can only be 

 * identified by their filename extension.

 *

 */



/* static unsigned char imCurMagicNumber[ ] = { 0x00, 0x00, 0x02, 0x00 }; */

static ImFileMagic imCurMagic[ ] =

{

	/* { 0, 4, imCurMagicNumber }, */

	{ 0, 0, NULL },

};



ImFileFormat ImFileCurFormat =

{

	imCurNames,					/* Names	*/

	"Windows cursor image file",			/* Description	*/

	"Microsoft",					/* Creator	*/

	"1-bit color index images with alpha channels.",/* Read support */

	"1-bit color index images with alpha channels.",/* Write support*/

	imCurMagic,					/* Magic #'s	*/

	IMMULTI, IMPIPE,				/* Read?	*/

	IMMULTI, IMPIPE,				/* Write?	*/

	imCurRead, imCurReadMap, imCurWriteMap		/* Maps		*/

};











/*

 *  TYPEDEF & STRUCTURE

 *	imCurHeader		-  CUR file header 

 *	imCurHeaderFields	-  CUR file header fields for binary pkg.

 *	imCurDirEntry		-  CUR cursor directory entry

 *	imCurDirEntryFields	-  CUR cursor directory entry for bin pkg.

 *	imCurInfo		-  CUR file info

 *	imCurInfoFields		-  CUR file info fields for bin pkg.

 *	imCurRGBQuad		-  CUR color information

 *	imCurRGBQuadFields	-  CUR color information fields for bin pkg.

 *

 *  DESCRIPTION

 *	The imCurRGBQuad is one entry in the colortable.

 */



typedef struct imCurHeader

{

	sdsc_uint16  cur_idReserved;	/* reserved, must be zero	*/

	sdsc_uint16  cur_idType;	/* resource type, must be 2	*/

	sdsc_uint16  cur_idCount;	/* number of entries in directory*/

} imCurHeader;



static BinField imCurHeaderFields[ ] =

{

	{ UINT16, 2, 1 },		/* cur_idReserved		*/

	{ UINT16, 2, 1 },		/* cur_idType			*/

	{ UINT16, 2, 1 },		/* cur_idCount			*/

	{ 0, 0, 0 }

};







typedef struct imCurDirEntry

{

	unsigned char	cur_Width;	/* width in pixels		*/

	unsigned char	cur_Height;	/* height in pixels		*/

	unsigned char	cur_ColorCount;	/* reserved, must be 0		*/

	unsigned char	cur_Reserved;	/* reserved, must be 0		*/

	sdsc_uint16	cur_XHotspot;	/* x coordinate of hotspot	*/

	sdsc_uint16	cur_YHotspot;	/* y coordinate of hotspot	*/

	sdsc_uint32	cur_BytesInRes;	/* size of cursor in bytes	*/

	sdsc_uint32	cur_ImageOffset;/* offset of file to cursor	*/

} imCurDirEntry;



static BinField imCurDirEntryFields[ ] =

{

	{ UCHAR, 1, 1 },		/* cur_Width			*/

	{ UCHAR, 1, 1 },		/* cur_Height			*/

	{ UCHAR, 1, 1 },		/* cur_ColorCount		*/

	{ UCHAR, 1, 1 },		/* cur_Reserved			*/

	{ UINT16, 2, 1 },		/* cur_XHotspot			*/

	{ UINT16, 2, 1 },		/* cur_YHotspot			*/

	{ UINT32, 4, 1 },		/* cur_ByteInRes		*/

	{ UINT32, 4, 1 },		/* cur_ImageOffset		*/

	{ 0, 0, 0 }

};







typedef struct imCurInfo

{

	sdsc_uint32	cur_bisize;	/* # bytes in cursor info header*/

	sdsc_uint32	cur_biwidth;	/* image width in pixels	*/

	sdsc_uint32	cur_biheight;	/* image height in pixels	*/

	sdsc_uint16	cur_biplanes;	/* # planes for device, always 1*/

	sdsc_uint16	cur_bibitcount;	/* bits/pixel, must be 1	*/

	sdsc_uint32	cur_bicompress;	/* unused, must be 0		*/

	sdsc_uint32	cur_bisizeimage;/* image size in bytes		*/

	sdsc_uint32	cur_bixpm;	/* unused, must be 0		*/

	sdsc_uint32	cur_biypm;	/* unused, must be 0		*/

	sdsc_uint32	cur_biclrused;	/* unused, must be 0		*/

	sdsc_uint32	cur_biclrim;	/* unused, must be 0		*/

} imCurInfo;



static BinField imCurInfoFields[ ] =

{

	{ UINT32, 4, 1 },		/* cur_bisize			*/

	{ UINT32, 4, 1 },		/* cur_biwidth			*/

	{ UINT32, 4, 1 },		/* cur_biheight			*/

	{ UINT16, 2, 1 },		/* cur_biplanes			*/

	{ UINT16, 2, 1 },		/* cur_bibitcount		*/

	{ UINT32, 4, 1 },		/* cur_bicompress		*/

	{ UINT32, 4, 1 },		/* cur_bisizeimage		*/

	{ UINT32, 4, 1 },		/* cur_bixpm			*/

	{ UINT32, 4, 1 },		/* cur_biypm			*/

	{ UINT32, 4, 1 },		/* cur_biclrused		*/

	{ UINT32, 4, 1 },		/* cur_biclrim			*/

	{ 0, 0, 0 }

};





typedef struct imCurRGBQuad

{

	unsigned char    blue;			/* blue intensity	*/

	unsigned char    green;			/* green intensity	*/

	unsigned char    red;			/* red intensity	*/

	unsigned char    reserved;		/* unused		*/

} imCurRGBQuad;



static BinField imCurRGBQuadFields[ ] =

{

	{ UCHAR, 1, 1 },		/* blue				*/

	{ UCHAR, 1, 1 },		/* green			*/

	{ UCHAR, 1, 1 },		/* red				*/

	{ UCHAR, 1, 1 },		/* reserved			*/

	{ 0, 0, 0 }

};











/*

 *  CONSTANTS

 *	CUR*	-  assorted useful CUR constants

 */



#define IMCURMAGIC		2

				/* magic resource number for cursors */

#define IMCURHEADERSIZE		6

				/* 6 bytes */

#define IMCURDIRENTRYSIZE	16

				/* 16 bytes */

#define IMCURINFOSIZE		40

				/* 40 bytes */

#define IMCURRGBQUADSIZE	4

				/* 4 bytes */

#define IMCURXOR		0

				/* XOR vfb flag, IMVFBMONO */

#define IMCURAND		1

				/* AND vfb flag, IMVFBMONO */











#ifdef __STDC__

static int imCurImageRead( int, int, FILE *, TagTable *, TagTable *,

	imCurDirEntry * );

#else

static int imCurImageRead( );

#endif











/*

 *  FUNCTION

 *	imCurRead	-  read a Microsoft Windows Cur file

 *

 *  DESCRIPTION

 *	The file header is read and the size of the image determined.

 *	Space is allocated for the VFB and the file is read into the

 *	VFB.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__

imCurRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imCurRead( ioType, fd, fp, flagsTable, tagTable )

	int         ioType;		/* I/O flags			*/

	int         fd;			/* Input file descriptor	*/

	FILE	   *fp;			/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable   *tagTable;		/* Tag table to add to		*/

#endif

{

	imCurHeader  header;		/* CUR file header		*/

	imCurDirEntry  *curdirentries;	/* CUR dir entries		*/

	imCurDirEntry  *curdirentryptr;	/* CUR dir entry pointer	*/

	unsigned int	i;		/* Counter			*/

	char	     message[100];	/* ImInfo message		*/





	/*

	 * Set Binary Package byte order and read in part of header

	 */

	BinByteOrder( BINLBF );

	if( ImBinReadStruct( ioType, fd, fp, &header,

		imCurHeaderFields ) == -1 )

	{

		ImReturnBinError( );

	}



	/*

	 * Use idType to determine byte ordering

	 */

	if( header.cur_idType != IMCURMAGIC )

	{

		/*

		 * Swap bytes

		 */

		header.cur_idType = (header.cur_idType&0xFF00)>>8 |

				    (header.cur_idType&0x00FF)<<8;

		if( header.cur_idType != IMCURMAGIC )

		{

			ImErrorFatal( ImQError( ), -1, IMEMAGIC );

		}

		else

		{

			header.cur_idCount = (header.cur_idCount&0xFF00)>>8 |

					     (header.cur_idCount&0x00FF)<<8;

			BinByteOrder( BINMBF );

			ImInfo( "Byte Order", "Most Significant Byte First" );

		}

	}

	else

	{

		ImInfo( "Byte Order", "Least Significant Byte First" );

	}



	/*

	 * Allocate space for cursor dir entries

	 */

	ImMalloc( curdirentries, imCurDirEntry*,

		header.cur_idCount * sizeof(imCurDirEntry) );



	/*

	 * Read in cursor dir entries

	 */

	curdirentryptr = curdirentries;

	for( i = 0; i < header.cur_idCount; ++i )

	{

		if( ImBinReadStruct( ioType, fd, fp, curdirentryptr,

			imCurDirEntryFields ) == -1 )

		{

			ImReturnBinError( );

		}

		++curdirentryptr;

	}



	/*

	 * Read in cursor images

	 */

	curdirentryptr = curdirentries;

	for( i = 0; i < header.cur_idCount; ++i )

	{

		/*

		 * Read in ith cursor

		 */

		sprintf( message, "%d of %d", (i+1), header.cur_idCount );

		ImInfo( "Image", message );



		imCurImageRead( ioType, fd, fp, flagsTable, tagTable,

			curdirentryptr );

		++curdirentryptr;

	}



	free( curdirentries );

	return header.cur_idCount;

}











#define imCurImageReadVfb( tmpvfb, vfbtype )				\
/*									\
 * The image is stored from the bottom up; that is,			\
 * the first scanline in the file is the last				\
 * scanline in the image.						\
 */									\
									\
/*									\
 * Go to the last scanline in the tmpvfb				\
 */									\
vfbptr = ImVfbQPtr( tmpvfb, 0, y-1 );					\
									\
switch( info.cur_bibitcount )						\
{									\
	case 1:								\
		/*							\
		 * Read 1-bit non-compressed image			\
		 *							\
		 * 1 bytes contains info for 8 pixels			\
		 *							\
		 * --------  <- 1 byte					\
		 * 12345678						\
		 *							\
		 * Set = first color in colortable			\
		 * Not Set = second color in colortable			\
		 *							\
		 */							\
									\
		/*							\
		 * Find scanline size to nearest 32-bit			\
		 * boundary and allocate read buffer			\
		 *							\
		 * Note: bit shifting is used to speed up		\
		 *	 calculations					\
		 */							\
		scanlinesize = x>>5;					\
		if( x%32 ) scanlinesize++;				\
		scanlinesize <<= 2;					\
									\
		ImMalloc( rbuf, unsigned char*, scanlinesize );			\
									\
		for( i = 0; i < y ; ++i )				\
		{							\
		    if( ImBinRead( ioType, fd, fp,			\
		   	 rbuf, UCHAR, 1, scanlinesize ) == -1)		\
		    {							\
			ImReturnBinError( );				\
		    }							\
		    rbufptr = rbuf;					\
		    							\
		    for( j = 0; j < (x/8); ++j )			\
		    {							\
			 mask = 128;					\
			 for( k = 0; k < 8; ++k )			\
			 {						\
			    if( ( *(rbufptr) & mask ) == mask )		\
			    {						\
				if( vfbtype == IMCURXOR )		\
				{					\
					ImVfbSIndex8( tmpvfb, vfbptr, 1 );\
				}					\
				else					\
				{					\
					ImVfbSAlpha( tmpvfb, vfbptr, 0 );\
				}					\
			    }						\
			    else					\
			    {						\
				if( vfbtype == IMCURXOR )		\
				{					\
					ImVfbSIndex8( tmpvfb, vfbptr, 0 );\
				}					\
				else					\
				{					\
					ImVfbSAlpha( tmpvfb, vfbptr, 255 );\
				}					\
			    }						\
			    vfbptr = ImVfbQRight( tmpvfb, vfbptr );	\
			    mask = mask >> 1;				\
			 }						\
		       	 ++rbufptr;					\
		    }							\
		    if( x%8 )						\
		    {							\
			 mask = 128;					\
			 for( k = 0; k < x%8; ++k )			\
			 {						\
			    if( ( *(rbufptr) & mask ) == mask )		\
			    {						\
				if( vfbtype == IMCURXOR )		\
				{					\
			 		ImVfbSIndex8( tmpvfb, vfbptr, 1 );\
				}					\
				else					\
				{					\
			 		ImVfbSAlpha( tmpvfb, vfbptr, 0 );\
				}					\
		    	    }						\
		    	    else					\
		    	    {						\
				if( vfbtype == IMCURXOR )		\
				{					\
			 		ImVfbSIndex8( tmpvfb, vfbptr, 0 );\
				}					\
				else					\
				{					\
			 		ImVfbSAlpha( tmpvfb, vfbptr, 255 );\
				}					\
			    }						\
			    vfbptr = ImVfbQRight( tmpvfb, vfbptr );	\
			    mask = mask >> 1;				\
		    	 }						\
		    }							\
									\
		       /*						\
			* Go up two scanline since we			\
			* wrapped-around with ImVfbQRight( )		\
			*/						\
			vfbptr = ImVfbQUp( tmpvfb, vfbptr );		\
			vfbptr = ImVfbQUp( tmpvfb, vfbptr );		\
		   }							\
									\
		   break;						\
									\
	default:							\
		ImErrorFatal( ImQError( ), -1, IMEPLANES );		\
									\
} /* end switch */							\
									\
free( (unsigned char*) rbuf );						\
									\











static int

#ifdef __STDC__

imCurImageRead ( int ioType, int fd, FILE *fp, TagTable *flagsTable,

	TagTable *tagTable, imCurDirEntry *curdirentryptr )

#else

imCurImageRead ( ioType, fd, fp, flagsTable, tagTable, curdirentryptr )

	int         ioType;		/* I/O flags			*/

	int         fd;			/* Input file descriptor	*/

	FILE	   *fp;			/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable   *tagTable;		/* Tag table to add to		*/

	imCurDirEntry *curdirentryptr;	/* Cursor dir entry pointer	*/

#endif

{

	imCurInfo      info;		/* Cursor image info		*/

	ImVfb         *vfb;		/* Virtual Frame Buffers	*/

	ImVfbPtr       vfbptr;		/* Pixel pointer		*/

	ImClt         *clt = IMCLTNULL;	/* Colortable			*/

	ImCltPtr       cltptr;		/* Colortable entry pointer	*/

	static ImHotSpot hotspot;	/* Cursor hotspot		*/

	ImHotSpotPtr   hotspotptr;	/* Cursor hotspot pointer	*/

	unsigned char  mask;		/* Mask for extracting pixels	*/

	unsigned char *rbuf = NULL;	/* Read buffer			*/

	unsigned char *rbufptr;		/* Read buffer data pointer	*/

	unsigned int   scanlinesize;	/* Length of scanline in bytes  */

	unsigned int   ncolors = 0;	/* Number of colors used	*/

	unsigned int   x, y;		/* Width and Height		*/

	unsigned int   i, j, k;		/* Counters			*/

	char	       message[100];	/* ImInfo message		*/





	/*

	 * Read in cursor image info.

	 */

	if( ImBinReadStruct( ioType, fd, fp, &info, imCurInfoFields ) == -1 )

	{

		ImReturnBinError( );

	}



	/*

	 * Store hotspot in TagTable

	 */

	hotspotptr = &hotspot;

	ImHotSpotSX( hotspotptr, curdirentryptr->cur_XHotspot );

	ImHotSpotSY( hotspotptr, curdirentryptr->cur_YHotspot );

	TagTableAppend( tagTable, TagEntryAlloc( "image hot spot",

		POINTER, &hotspotptr) );



	x = info.cur_biwidth;

	y = info.cur_biheight / 2;







	/*

	 *  Allocate a VFBs of the required size and type.

	 */

	switch( info.cur_bibitcount )

	{

	case 1:		/* 1-bit index image */

		break;



	default:	/* unsupported image depth */

		ImErrorFatal( ImQError( ), -1, IMEDEPTH );

	}

	vfb = ImVfbAlloc( x, y, IMVFBINDEX8 | IMVFBALPHA);

	if ( vfb == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	/*

	 * Allocate and read in colortable

	 */

	ncolors = (1 << info.cur_bibitcount);

	switch( ncolors )

	{

	case 2:		/* 1-bit, monochrome */

		break;

	default:

		ImErrorFatal( ImQError( ), -1, IMECLTLENGTH );

		/* output warning */

	}

	clt = ImCltAlloc( ncolors );



	/*

	 * Read in colortable and set RGB values

	 */

	cltptr = ImCltQFirst( clt );

	ImMalloc( rbuf, unsigned char*, ncolors*IMCURRGBQUADSIZE );

	rbufptr = rbuf;

	if( ImBinRead( ioType, fd, fp, rbuf, UCHAR, 1,

		ncolors*IMCURRGBQUADSIZE ) == -1 )

	{

		ImReturnBinError( );

	}

	for( i = 0; i < ncolors; ++i )

	{

		ImCltSBlue( cltptr, *rbufptr );

		++rbufptr;

		ImCltSGreen( cltptr, *rbufptr );

		++rbufptr;

		ImCltSRed( cltptr, *rbufptr );

		++rbufptr;

		/* skip reserved field */

		++rbufptr;



		cltptr = ImCltQNext( clt, cltptr );

	}

	free( rbuf );



	/*

	 * Attach colortable to vfb and append to tagTable

	 */

	ImVfbSClt( vfb, clt );

	TagTableAppend( tagTable, TagEntryAlloc( "image clt",

		POINTER, &clt) );



	/*

	 * Read in XOR mask image

	 */

	imCurImageReadVfb( vfb, IMCURXOR );



	/*

	 * Read in AND mask image

	 */

	info.cur_bibitcount = 1;	/* AND masks are mono images */

	imCurImageReadVfb( vfb, IMCURAND );



	/*

	 *  Add the VFB to the tagTable.

	 */

	TagTableAppend( tagTable,

		TagEntryAlloc( "image vfb", POINTER, &vfb ) );



	/*

	 * Output -verbose message

	 */

	sprintf( message, "%d x %d", x, y );

	ImInfo( "Resolution", message );



	ImInfo( "Type", "1-bit Color Indexed (XOR mask)" );



	sprintf( message, "%d Entries", ncolors );

	ImInfo( "Color Table", message );



	ImInfo( "Alpha Channel", "1-bit Monochrome (AND mask)" );



	sprintf( message, "%d, %d", hotspot.hot_x, hotspot.hot_y );

	ImInfo( "Hot Spot", message );



	return ( 2 );

}











/*

 *  FUNCTION

 *	imCurWriteRaw1	-  write an 1-bit non-compressed CUR file

 *

 *  DESCRIPTION

 *	That VFB is queried, and the CUR file header set up and written out.

 *	The VFB data is then converted and written out.

 *	Multiple image writes are supported.

 */



static int                              /* Returns # of tags used       */

#ifdef __STDC__

imCurWriteRaw1( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable )

#else

imCurWriteRaw1( pMap, ioType, fd, fp, flagsTable, tagTable )

        ImFileFormatWriteMap *pMap;     /* Write map entry to adhear to */

        int          ioType;            /* I/O flags                    */

        int          fd;                /* Input file descriptor        */

        FILE        *fp;                /* Input file pointer           */

        TagTable    *flagsTable;        /* Flags                        */

        TagTable    *tagTable;          /* Tag list to read from        */

#endif

{

        imCurHeader    header;		/* CUR file header              */

	imCurDirEntry  direntry;	/* CUR dir entry		*/

	imCurInfo      info;		/* CUR info			*/

        ImVfb         *vfb;		/* Read in image                */

        ImVfbPtr       vfbptr;		/* Vfb pixel pointer            */

	imCurRGBQuad   cltentry;	/* Colortable RGBQuad entry	*/

	ImClt	      *clt;		/* Colortable			*/

	ImCltPtr       cltptr;		/* Colortable entry pointer	*/

	ImHotSpotPtr   hotspotptr;	/* Cursor hotspot pointer	*/

	unsigned char  pvalue;		/* Pixel value			*/

	unsigned char *wbuf;		/* Pointer to write buffer	*/

	unsigned char *wbufptr;		/* Pointer to write buffer data	*/

	unsigned int   scanlinesize;	/* Length of scanline		*/

	unsigned char  bitfield;	/* Bitfield of 8 pixels		*/

        unsigned int   x, y;		/* Width, Height                */

        int            i, j, k, loop;	/* Counters                     */

	unsigned int   count;		/* Counter			*/

	int	       fields;		/* Vfb fields			*/

	char	       message[100];	/* ImInfo message		*/

	int	       imageNum;	/* number of images in a file	*/

        TagEntry      *tagEntry;	/* Tag table entry holder       */

	unsigned long  offsetTotal;	/* where you are in image	*/

 

 

        /*

         * Set byte ordering

         */

        BinByteOrder( BINLBF );

	ImInfo( "Byte Order", "Least Significant Byte First" );



	/*

	 * Get number of images

	 */

        imageNum   = TagTableQNEntry( tagTable, "image vfb" );



	/*

	 * Setup header and write it out

	 */

	header.cur_idReserved	= 0;

	header.cur_idType	= (sdsc_uint16) IMCURMAGIC;

	header.cur_idCount	= (sdsc_uint16) imageNum;



        if ( ImBinWriteStruct( ioType, fd, fp, &header, imCurHeaderFields )==-1)

	{

		ImReturnBinError( );

	}



#ifdef old

        offsetTotal = (sdsc_uint32)( sizeof(imCurHeader)

                + (imageNum * (sizeof(imCurDirEntry))));

#else

	/*

	 *  Compute the file offset to right after the header and individual

	 *  cursor directory entries.

	 */

	offsetTotal = (unsigned long) IMCURHEADERSIZE +

		(imageNum * (unsigned long) IMCURDIRENTRYSIZE);

#endif





	/*

	 *  Start by doing a dummy pass through all the images in the

	 *  tag table.  For each image, write out a directory entry and

	 *  compute where the image's bytes will eventually be in the file.

	 */

	for ( loop = 0; loop < imageNum; loop++ )

	{

		/*

		 * Get vfb and clt

		 */

		tagEntry = TagTableQDirect( tagTable, "image vfb", loop );

		TagEntryQValue( tagEntry, &vfb );

		clt = ImVfbQClt( vfb );

		x   = ImVfbQWidth( vfb );

		y   = ImVfbQHeight( vfb );





		/*

		 * Find scanline size to nearest 32-bit boundary.

		 */

		scanlinesize = x>>5;

		if ( x%32 ) scanlinesize++;

		scanlinesize <<= 2;





		/*

		 * Setup cursor directory and write it out

		 */

		direntry.cur_Width	= (unsigned char) x;

		direntry.cur_Height	= (unsigned char) y;

		direntry.cur_ColorCount = (unsigned char) 2;	/* Always 2 */

		direntry.cur_Reserved	= 0;

		if( TagEntryQValue( TagTableQDirect( tagTable,

			"image hot spot", loop ), &hotspotptr ) == -1 )

		{

			direntry.cur_XHotspot = 0;

			direntry.cur_YHotspot = 0;

		}

		else

		{

			direntry.cur_XHotspot =ImHotSpotQX( hotspotptr);

			direntry.cur_YHotspot =ImHotSpotQY( hotspotptr);

		}	

		/*

		 *  # of bytes = cursor info + colortable +

		 *		XOR mask + AND mask

		 */

#ifdef old

		direntry.cur_BytesInRes	= (sdsc_uint32) ( IMCURINFOSIZE +

			( 2 * IMCURRGBQUADSIZE ) + (scanlinesize * y) +

			(x*y/8) );

#else

		direntry.cur_BytesInRes	= (sdsc_uint32) ( IMCURINFOSIZE +

			( 2 * IMCURRGBQUADSIZE ) + (scanlinesize * y*2));

#endif

		direntry.cur_ImageOffset= (sdsc_uint32) (offsetTotal);



		if ( ImBinWriteStruct( ioType, fd, fp, &direntry,

			imCurDirEntryFields )==-1)

		{

			ImReturnBinError( );

		}



#ifdef old

		offsetTotal += sizeof(imCurInfo)

		       + (2 * sizeof(imCurRGBQuad))

		       + ( 2 * ((x/8) *y) );

#else

		offsetTotal += IMCURINFOSIZE + (2 * IMCURRGBQUADSIZE) +

			2 * scanlinesize * y;

#endif



	}





	/*

	 *  Now do it for real.  Loop through all the images and write them

	 *  out.

	 */

	for ( loop = 0; loop < imageNum; loop++ )

	{

		/*

		 * Get vfb and clt

		 */

		tagEntry = TagTableQDirect( tagTable, "image vfb", loop );

		TagEntryQValue( tagEntry, &vfb );

		clt = ImVfbQClt( vfb );

		x   = ImVfbQWidth( vfb );

		y   = ImVfbQHeight( vfb );



		/*

		 * Find scanline size to nearest 32-bit boundary.

		 */

		scanlinesize = x>>5;

		if( x%32 ) scanlinesize++;

		scanlinesize <<= 2;





		/*

		 * Setup cursor info and write it out

		 */

		info.cur_bisize		= (sdsc_uint32) IMCURINFOSIZE;

		info.cur_biwidth	= (sdsc_uint32) x;

		info.cur_biheight	= (sdsc_uint32) (2*y);

		info.cur_biplanes	= (sdsc_uint16) 1;

		info.cur_bibitcount	= (sdsc_uint16) 1;

		info.cur_bicompress	= 0;

		info.cur_bisizeimage	= (sdsc_uint32) (scanlinesize * y);

		info.cur_bixpm		= 0;

		info.cur_biypm		= 0;

		info.cur_biclrused	= 0;

		info.cur_biclrim	= 0;

 

		if ( ImBinWriteStruct( ioType, fd, fp, &info,

			imCurInfoFields )==-1)

		{

			ImReturnBinError( );

		}



		/*

		 * Output -verbose message

		 */

		sprintf( message, "%d of %d", loop, imageNum );

		ImInfo( "Image", message );



		sprintf( message, "%d x %d", x, y );

		ImInfo( "Resolution", message );



		ImInfo( "Type", "1-bit Color Indexed (XOR mask)" );

		ImInfo( "Color Table", "2 Entries" );

		ImInfo( "Alpha Channel", "1-bit Monochrome (AND mask)" );



		sprintf( message, "%d, %d", direntry.cur_XHotspot,

			direntry.cur_YHotspot );

		ImInfo( "Hot Spot", message );





		/*

		 * Write colortable

		 */

		cltentry.reserved = 0;

		cltentry.blue     = 0;

		cltentry.green    = 0;

		cltentry.red      = 0;



		if( clt != IMCLTNULL )

		{

			clt = ImVfbQClt( vfb );



			cltptr = ImCltQFirst( clt );

			cltentry.blue  = (unsigned char) ImCltQBlue( cltptr );

			cltentry.green = (unsigned char) ImCltQGreen( cltptr );

			cltentry.red   = (unsigned char) ImCltQRed( cltptr );

		}

		if ( ImBinWriteStruct( ioType, fd, fp, &cltentry,

			imCurRGBQuadFields ) == -1 )

		{

			ImReturnBinError( );

		}



		cltentry.blue  = 255;

		cltentry.green = 255;

		cltentry.red   = 255;



		if( ( clt != IMCLTNULL ) && (ImCltQNColors( clt ) > 1) )

		{

			cltptr = ImCltQNext( clt, cltptr );

			cltentry.blue  = (unsigned char) ImCltQBlue( cltptr );

			cltentry.green = (unsigned char) ImCltQGreen( cltptr );

			cltentry.red   = (unsigned char) ImCltQRed( cltptr );

		}



		if ( ImBinWriteStruct(ioType,fd,fp,&cltentry,imCurRGBQuadFields )==-1)

		{

			ImReturnBinError( );

		}



		/* 

		 * Allocate write buffer space

		 */

		ImCalloc( wbuf, unsigned char*, scanlinesize,

			sizeof( unsigned char ) );



		/* 

		 * Write XOR mask image

		 */

		for( i = 0; i < y; ++i )

		{

			vfbptr = ImVfbQPtr( vfb, 0, y-i-1 );

			wbufptr = wbuf;

			for( j = 0; j < (x/8); ++j )

			{

				/*

				 * Pack 8 pixels into a 1 byte bitfield

				 *

				 * --------  <-- 1 byte

				 * 12345678

				 *

				 */

				bitfield = 0;

				for( k = 0; k < 8; ++k )

				{

					pvalue = (unsigned char) ImVfbQMono( vfb, vfbptr );

					if( pvalue ) {

						bitfield = ( bitfield | (1 << (7-k) ) );

					}

					vfbptr = ImVfbQRight( vfb, vfbptr );

				}

				*( wbufptr ) = bitfield;

				++wbufptr;

			}



			if( x%8 )

			{

				/*

				 * Pack 8 pixels into a 1 byte bitfield

				 *

				 * --------  <-- 1 byte

				 * 12345678

				 *

				 */

				bitfield = 0;

				for( k = 0; k < x%8; ++k )

				{

					pvalue = (unsigned char) ImVfbQMono( vfb, vfbptr );

					if( pvalue ) {

						bitfield = ( bitfield | (1 << (7-k) ) );

					}

					vfbptr = ImVfbQRight( vfb, vfbptr );

				}

				*( wbufptr ) = bitfield;

			}



			if( ImBinWrite( ioType,fd,fp,wbuf,UCHAR,1,scanlinesize ) == -1 )

			{

				ImReturnBinError( );

			}



			/*

			 * Go up two scanline since we

			 * wrapped-around with ImVfbQRight( )

			 */

			vfbptr = ImVfbQUp( vfb, vfbptr );

			vfbptr = ImVfbQUp( vfb, vfbptr );

		}



		/*

		 * Check for an existing alpha channel

		 */

		fields = ImVfbQFields( vfb );



		/*

		 * If there's an alpha channel, use it as the AND mask

		 */

		if( fields & IMVFBALPHA )

		{

			/*

			 * Initialize

			 */

			count = 0;



			/*

			 * Traverse through alpha plane and write out AND mask

			 */

			for( i = y-1; i > -1; --i )

			{

				wbufptr = wbuf;

				for( j = 0; j < x; ++j )

				{

					vfbptr = ImVfbQPtr( vfb, j, i );

					pvalue = ImVfbQAlpha( vfb, vfbptr );

		

					if( pvalue < 128 )

					{

						bitfield = bitfield | 1;

					}



					++count;

					if( count == 8 )

					{

						*(wbufptr++) = bitfield;

						count = 0;

						bitfield = 0;

					}

					bitfield = bitfield << 1;

				}

				if( ImBinWrite( ioType,fd,fp,wbuf,UCHAR,1,

					scanlinesize )== -1 )

				{

					ImReturnBinError( );

				}

			}

		}

		else

		{

			/*

			 * Clear wbuf

			 */

			wbufptr = wbuf;

			for( i = 0; i < scanlinesize; ++i )

			{

				*(wbufptr++) = 0;

			}



			/*

			 * Write AND mask scanlines

			 */

			for( i = 0; i < y; ++i )

			{

				if( ImBinWrite( ioType,fd,fp,wbuf,UCHAR,1,

					scanlinesize )== -1 )

				{

					ImReturnBinError( );

				}

			}

		}

		free( (unsigned char*) wbuf );

	}



        return ( 1 );

}

