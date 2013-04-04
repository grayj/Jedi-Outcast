/**

 **     $Header: /roq/libim/imico.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER  "	$Header: /roq/libim/imico.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imico.c		-  Microsoft Windows ICO file I/O

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imico.c contains routines to read and write Microsoft Windows ICO

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

 **	none

 **

 **  PRIVATE CONTENTS

 **	imIcoRead		f  read a Microsoft Windows ICO file

 **	imIcoWrite		f  control for what to write

 **

 **	imIcoHeader		t  file header information

 **	imIcoHeaderFields	v  imIcoHeader description for Bin pkg.

 **	imIcoDirEntry		t  icon directory entry

 **	imIcoDirEntryFields	v  imIcoDirEntry description for Bin pkg.

 **	imIcoInfo		t  icon information

 **	imIcoInfoFields		v  imIcoInfo description for Bin pkg.

 **

 **	imIcoImageRead		f  read the ith icon image

 **	imIcoReadImage		f  read the icon image

 **

 **	imIcoWriteImage		f  write the icon image

 **	imIcoWriteRaw1		f  write a Microsoft Windows ICO 1-bit file

 **	imIcoWriteRaw4		f  write a Microsoft Windows ICO 4-bit file

 **

 **  HISTORY

 **     $Log: /roq/libim/imico.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.12  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.11  1995/06/15  20:26:23  bduggan

 **	took out some unused vars.

 **	turned bzero into memset

 **

 **	Revision 1.10  1995/04/03  21:26:04  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.9  1995/01/16  22:08:46  bduggan

 **	Fixed a typo that was hindering reading of

 **	alpha channels (changed 0x100 to 0x80)

 **

 **	Revision 1.8  1995/01/10  23:27:34  bduggan

 **	uncapitlized i's in local functions, made read/write routines static,

 **	put in IMMULTI, IMPIPE instead of TRUE/FALSE

 **

 **	Revision 1.7  1994/08/31  17:34:50  nadeau

 **	Updated to ANSI C comaptibility.  Updated comments.

 **	Rearranged code a bit for better readability.  Reformatted

 **	code for consistent indentation style.  Misc cleanup.

 **

 **	Revision 1.7  1994/08/31  17:34:50  nadeau

 **	Updated to ANSI C comaptibility.  Updated comments.

 **	Rearranged code a bit for better readability.  Reformatted

 **	code for consistent indentation style.  Misc cleanup.

 **

 **	Revision 1.6  92/12/03  01:49:00  nadeau

 **	Corrected info messages.  Got rid of IMINFOMSG references.

 **	

 **	Revision 1.5  92/11/04  11:47:52  groening

 **	added multiple read and write.

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.4  92/10/12  15:54:33  vle

 **	Made changes to make Cray happy.

 **	

 **	Revision 1.3  92/09/29  17:58:34  vle

 **	Added ImInfo messages.

 **	

 **	Revision 1.2  92/09/17  14:23:09  vle

 **	Fixed "scanlinesize" math bug that caused images to be cropped.

 **	

 **	Revision 1.1  92/08/18  12:21:58  vle

 **	Initial revision

 **	

 **/

 

#include "iminternal.h"





/**

 **  FORMAT

 **	ICO		Microsoft Windows icon image

 **

 **  AKA

 **	None

 **

 **  FORMAT REFERENCES

 **	Microsoft Windows 3.1 Programmer's Reference, volume 3, Microsoft

 **	Press, 1992.

 **

 **	Microsoft Windows 3.1 Programmer's Reference, volume 3, Microsoft

 **	Press, 1992.

 **

 **  CODE CREDITS

 ** 	Custom development, Vinh Le, San Diego Supercomputer Center, 1992.

 **

 **  DESCRIPTION

 **	Microsoft's ICO (ICOn) format is used in Microsoft's Windows

 **	versions 3.0 and up for storage of one or more icons.

 **

 **	The first item encountered in an ICO file is an Icon Directory

 **	Header.  This tells that the file is indeed a file containing

 **	icons and that there are n number of them.

 **

 **	Following the Icon Directory Header is an Icon Directory

 **	Entry for each icon.  An Icon Directory Entry contains

 **	information on the icon's location within the file,

 **	its width, height, etc.  There are n Icon Directory Entries.

 **

 **	Following all this is the actual Icon Images themselves, which

 **	consist of an info header, a colortable, and XOR and AND masks.

 **	The XOR mask could be considered the "real" icon.  The AND

 **	mask determines the XOR image's "tranparency".  XOR masks

 **	can either be 1-, 3-, or 4-bits/pixel.  4-bits/pixel images

 **	are packed 2 pixels/byte.  3-bits/pixel images are currently

 **	unsupported.  1-bit/pixel images are packed 8 pixels/byte.

 **	AND masks are always monochrome images, thus they are

 **	packed as 1-bit/pixel images, or 8-bits/byte.

 **

 **

 **	Stucture Overview

 **	-----------------

 **

 **	ICON DIRECTORY

 **	---------------------------------------------------------

 **	|ICON	|ICON	|ICON	|  ...	|	|	|	|

 **	|ENTRY 1|ENTRY 2|ENTRY 3|	|	|	|	|

 **	---------------------------------------------------------

 **	    |

 **	    |		---------

 **	    +---------> |ICON	|

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

 **	Icon Directory Header (6 bytes + 16 bytes/icon entry)

 **	---------------------

 **

 **	The Icon Directory Header contains the resource type and

 **	number of icons in the Icon Directory.

 **

 **	Name		Type		Size		Comments

 **	----		----		----		--------

 **	idReserved	word		2 bytes		reserved, must be 0

 **	idType		word		2 bytes		resource type,

 **							  must be 1

 **	idCount		word		2 bytes		number of icons

 **	idEntries[]	IconDirEntry	16 bytes	array of info for

 **					  per entry	  individual icons

 **

 **	typedef struct ICONDIR

 **	{

 **		word		idReserved;

 **		word		idType;

 **		word		idCount;

 **		ICONDIRENTRY	idEntries[idCount];

 **	} ICONDIR;

 **

 **	Icon Directory Entry (16 bytes/icon entry)

 **	--------------------

 **

 **	An Icon Directory Entry stores information about the an icon's

 **	dimensions and color attributes.

 **

 **	Name		Type	Size	Comments

 **	----		----	----	--------

 **	Width		byte	1 byte	width in pixels, must be 16, 32, or 64

 **	Height		byte	1 byte	height in pixels, must be 16, 32, or 64

 **	ColorCount	byte	1 byte	number of colors, must be 2, 8 or 16

 **	Reserved	byte	1 byte	reserved, must be 0

 **	Planes		word	2 byte	number of color planes

 **	BitCount	word	2 byte	number of bits

 **	BytesInRes	dword	4 byte	size of icon in bytes

 **	ImageOffset	dword	4 byte	offset from beginning of file to

 **					  Icon Image

 **

 **	typedef struct ICONDIRENTRY

 **	{

 **		byte	Width;

 **		byte	Height;

 **		byte	ColorCount;

 **		byte	Reserved;

 **		word	Planes;

 **		word	BitCount;

 **		dword	BytesInRes;

 **		dword	ImageOffset;

 **	} ICONDIRENTRY;

 **

 **	Icon Image (40 bytes + 4 bytes/color entry + sizeof XOR mask +

 **	----------  sizeof AND mask)

 **

 **	Each Icon Image stores 1 icon.

 **

 **	Name		Type			Size		Comments

 **	----		----			----		--------

 **	icHeader	BITMAPFILEHEADER	40 bytes	only biSize

 **								  through

 **								  biBitCount

 **								  and

 **								  biSizeImage

 **								  are used

 **	icColors[]	RGBQUAD			4 bytes per	colortable used

 **						  color		  in XOR mask

 **	icXOR[]		byte			1 byte per	XOR image mask

 **						  element

 **	icAND[]		byte			1 byte per	AND image mask

 **						  element

 **

 **     Ico-Info Header (40 bytes)

 **     ------------------

 ** 

 **     The Ico-Info Header contains information about the icon's

 **     size and dimensions.

 ** 

 **     Name            Type    Size            Comments

 **     ----            ----    ----            --------

 **     biSize          dword   4 bytes         number of bytes in

 **                                             Ico-Info Header

 **     biWidth         long    4 bytes         width, in pixels

 **     biHeight        long    4 bytes         height, in pixels

 **     biPlanes        word    2 bytes         number of planes for the

 **                                             display device, must be 1

 **     biBitCount      word    2 bytes         bits per pixel, must be 1,

 **                                             3, 4

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

 **  IMPLEMENTATION SPECIFIC NOTES

 **	3-bits/pixel (8 color) icons are not supported due to the

 **     lack of documentation on the packing method for such icons.

 **

 **/











/*

 *  ICO - Microsoft Windows icon

 *      For information on these structures, how to use them, etc. please

 *      see imfmt.c.

 */

#ifdef __STDC__

static int imIcoRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imIcoWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, 

	TagTable *flagsTable, TagTable *tagTable );

#else

static int imIcoRead( ), imIcoWrite( );

#endif

static char *imIcoNames[ ] = { "ico", NULL };

static unsigned char imIcoMagicNumber[ ] = { 0x00, 0x00, 0x01, 0x00 };

static ImFileMagic imFileIcoMagic []=

{

	{ 0, 4, imIcoMagicNumber },

	{ 0, 0, NULL },

};



static ImFileFormatReadMap imIcoReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IN,1,1,       C|A,    IMVFBINDEX8,    C|A },

        { IN,1,4,       C|A,    IMVFBINDEX8,    C|A },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imIcoWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBMONO,    0,      IN,1,1,         C|A,    imIcoWrite },

        { IMVFBMONO,    C,      IN,1,1,         C|A,    imIcoWrite },

        { IMVFBMONO,    A,      IN,1,1,         C|A,    imIcoWrite },

        { IMVFBMONO,    C|A,    IN,1,1,         C|A,    imIcoWrite },

        { IMVFBINDEX8,  0,      IN,1,4,         C|A,    imIcoWrite },

        { IMVFBINDEX8,  C,      IN,1,4,         C|A,    imIcoWrite },

        { IMVFBINDEX8,  A,      IN,1,4,         C|A,    imIcoWrite },

        { IMVFBINDEX8,  C|A,    IN,1,4,         C|A,    imIcoWrite },

        { -1,           0,      -1,             0,      NULL },

};



ImFileFormat ImFileIcoFormat =

{

	imIcoNames, "Windows icon image file",

	"Microsoft Corp.",

	"1- and 4-bit color index images with alpha channels.",

	"1- and 4-bit color index images with alpha channels.",

	imFileIcoMagic,

	IMMULTI, IMPIPE,	/* Read */

	IMMULTI, IMPIPE,	/* Write */

	imIcoRead, imIcoReadMap, imIcoWriteMap

};











/*

 *  TYPEDEF & STRUCTURE

 *	imIcoHeader		-  ICO file header 

 *	imIcoHeaderFields	-  ICO file header fields for binary pkg.

 *	imIcoDirEntry		-  ICO icon directory entry

 *	imIcoDirEntryFields	-  ICO icon directory entry for bin pkg.

 *	imIcoInfo		-  ICO file info

 *	imIcoInfoFields		-  ICO file info fields for bin pkg.

 *	imIcoRGBQuad		-  ICO color information

 *	imIcoRGBQuadFields	-  ICO color information fields for bin pkg.

 *

 *  DESCRIPTION

 *	The imIcoRGBQuad is one entry in the colortable.

 */



typedef struct imIcoHeader

{

	sdsc_uint16  ico_idReserved;	/* reserved, must be zero		*/

	sdsc_uint16  ico_idType;	/* resource type, must be 1		*/

	sdsc_uint16  ico_idCount;	/* number of entries in directory	*/

} imIcoHeader;



static BinField imIcoHeaderFields[ ] =

{

	{ UINT16, 2, 1 },		/* ico_idReserved		*/

	{ UINT16, 2, 1 },		/* ico_idType			*/

	{ UINT16, 2, 1 },		/* ico_idCount			*/

	{ 0, 0, 0 }

};



#define IMICOHEADERSIZE		6	/* bytes			*/







typedef struct imIcoDirEntry

{

	unsigned char	ico_Width;	/* width in pixels, must be 16,32,or 64	*/

	unsigned char	ico_Height;	/* height in pixels, must be 16,32,or 64*/

	unsigned char	ico_ColorCount;	/* number of colors, must be 2,8 or 16	*/

	unsigned char	ico_Reserved;	/* reserved, must be 0			*/

	sdsc_uint16	ico_Planes;	/* number of color planes		*/

	sdsc_uint16	ico_BitCount;	/* number of bits			*/

	sdsc_uint32	ico_BytesInRes;	/* size of icon in bytes		*/

	sdsc_uint32	ico_ImageOffset;/* offset from beginning of file to

				  Icon Image				*/

} imIcoDirEntry;



static BinField imIcoDirEntryFields[ ] =

{

	{ UCHAR, 1, 1 },		/* ico_Width			*/

	{ UCHAR, 1, 1 },		/* ico_Height			*/

	{ UCHAR, 1, 1 },		/* ico_ColorCount		*/

	{ UCHAR, 1, 1 },		/* ico_Reserved			*/

	{ UINT16, 2, 1 },		/* ico_Planes			*/

	{ UINT16, 2, 1 },		/* ico_BitCount			*/

	{ UINT32, 4, 1 },		/* ico_ByteInRes		*/

	{ UINT32, 4, 1 },		/* ico_ImageOffset		*/

	{ 0, 0, 0 }

};



#define IMICODIRENTRYSIZE		16	/* bytes			*/







typedef struct imIcoInfo

{

	sdsc_uint32	ico_bisize;	/* number of bytes in icon info header	*/

	sdsc_uint32	ico_biwidth;	/* image width in pixels		*/

	sdsc_uint32	ico_biheight;	/* image height in pixels		*/

	sdsc_uint16	ico_biplanes;	/* number of planes for device, always 1*/

	sdsc_uint16	ico_bibitcount;	/* bits/pixel (1, 3, or 4)		*/

	sdsc_uint32	ico_bicompress;	/* unused, must be 0 			*/

	sdsc_uint32	ico_bisizeimage;/* image size in bytes			*/

	sdsc_uint32	ico_bixpm;	/* unused, must be 0			*/

	sdsc_uint32	ico_biypm;	/* unused, must be 0			*/

	sdsc_uint32	ico_biclrused;	/* unused, must be 0			*/

	sdsc_uint32	ico_biclrim;	/* unused, must be 0			*/

} imIcoInfo;



static BinField imIcoInfoFields[ ] =

{

	{ UINT32, 4, 1 },		/* ico_bisize			*/

	{ UINT32, 4, 1 },		/* ico_biwidth			*/

	{ UINT32, 4, 1 },		/* ico_biheight			*/

	{ UINT16, 2, 1 },		/* ico_biplanes			*/

	{ UINT16, 2, 1 },		/* ico_bibitcount		*/

	{ UINT32, 4, 1 },		/* ico_bicompress		*/

	{ UINT32, 4, 1 },		/* ico_bisizeimage		*/

	{ UINT32, 4, 1 },		/* ico_bixpm			*/

	{ UINT32, 4, 1 },		/* ico_biypm			*/

	{ UINT32, 4, 1 },		/* ico_biclrused		*/

	{ UINT32, 4, 1 },		/* ico_biclrim			*/

	{ 0, 0, 0 }

};



#define IMICOINFOSIZE		40	/* bytes			*/







typedef struct imIcoRGBQuad

{

	unsigned char    blue;			/* blue intensity		*/

	unsigned char    green;			/* green intensity		*/

	unsigned char    red;			/* red intensity		*/

	unsigned char    reserved;		/* unused			*/

} imIcoRGBQuad;



static BinField imIcoRGBQuadFields[ ] =

{

	{ UCHAR, 1, 1 },		/* blue				*/

	{ UCHAR, 1, 1 },		/* green			*/

	{ UCHAR, 1, 1 },		/* red				*/

	{ UCHAR, 1, 1 },		/* reserved			*/

	{ 0, 0, 0 }

};



#define IMICORGBQUADSIZE		4	/* bytes			*/







/*

 *  	Various useful ICO specific constants

 */



#define IMICOMAGIC		1

				/* magic resource number for icons */

#define IMICOXOR		0

				/* XOR vfb flag, IMVFBINDEX8 */

#define IMICOAND		1

				/* AND vfb flag, IMVFBMONO */







#ifdef __STDC__

static int imIcoImageRead ( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, 

	imIcoDirEntry *icodirentryptr);

static int imIcoReadImage( int ioType, int fd, FILE *fp, ImVfb *vfb, int what );

static int imIcoWriteRaw1(ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, ImVfb *vfb, ImClt *clt, 

	imIcoDirEntry *direntry );

static int imIcoWriteRaw4( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, ImVfb *vfb, ImClt *clt, 

	imIcoDirEntry *direntry);

static int imIcoWriteImage( int ioType, int fd, FILE *fp, ImVfb *vfb, int what );

#else

static int imIcoImageRead( );

static int imIcoReadImage( );

static int imIcoWriteRaw1( );

static int imIcoWriteRaw4( );

static int imIcoWriteImage( );

#endif











/*

 *  FUNCTION

 *	imIcoRead	-  read a Microsoft Windows Ico file

 *

 *  DESCRIPTION

 *	The file header is read and the size of the image determined.

 *	Space is allocated for the VFB and the file is read into the

 *	VFB.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__

imIcoRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imIcoRead( ioType, fd, fp, flagsTable, tagTable )

	int         ioType;		/* I/O flags			*/

	int         fd;			/* Input file descriptor	*/

	FILE	   *fp;			/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable   *tagTable;		/* Tag table to add to		*/

#endif

{

	imIcoHeader  header;		/* ICO file header		*/

	imIcoDirEntry  *icodirentries;	/* ICO dir entries		*/

	imIcoDirEntry  *icodirentryptr;	/* ICO dir entry pointer	*/

	unsigned int	i;		/* Counter			*/

	char		message[100];	/* ImInfo message		*/





	/*

	 * Set Binary Package byte order and read in part of header

	 */

	BinByteOrder( BINLBF );

	if( ImBinReadStruct( ioType, fd, fp, &header,

		imIcoHeaderFields ) == -1 )

	{

		ImReturnBinError( );

	}



	/*

	 * Use idType to determine byte ordering

	 */

	if( header.ico_idType != IMICOMAGIC )

	{

		/*

		 * Swap bytes

		 */

		header.ico_idType = (header.ico_idType&0xFF00)>>8 |

				    (header.ico_idType&0x00FF)<<8;

		if( header.ico_idType != IMICOMAGIC )

		{

			ImErrorFatal( ImQError( ), -1, IMEMAGIC );

		}

		else

		{

			header.ico_idCount = (header.ico_idCount&0xFF00)>>8 |

					     (header.ico_idCount&0x00FF)<<8;

			BinByteOrder( BINMBF );

			ImInfo( "Byte Order", "Most Significant Byte First");

		}

	}

	else

	{

		ImInfo( "Byte Order", "Least Significant Byte First" );

	}



	/*

	 * Allocate space for icon dir entries

	 */

	ImMalloc( icodirentries, imIcoDirEntry*,

		header.ico_idCount * sizeof(imIcoDirEntry) );



	/*

	 * Read in icon dir entries

	 */

	icodirentryptr = icodirentries;

	for( i = 0; i < header.ico_idCount; ++i )

	{

		if( ImBinReadStruct( ioType, fd, fp, icodirentryptr,

			imIcoDirEntryFields ) == -1 )

		{

			ImReturnBinError( );

		}

		++icodirentryptr;

	}



	/*

	 * Read in icon images

	 */

	icodirentryptr = icodirentries;

	for( i = 0; i < header.ico_idCount; ++i )

	{

		/*

		 * Read in ith icon

		 */

		sprintf( message, "%d of %d", (i+1), header.ico_idCount );

		ImInfo( "Image", message );



		imIcoImageRead( ioType, fd, fp, flagsTable, tagTable,

			icodirentryptr);

		++icodirentryptr;

	}



	free( icodirentries );

	return header.ico_idCount;

}











/*

 *  FUNCTION

 *	imIcoReadImage	-  read in an image's pixels

 *

 *  DESCRIPTION

 *	A temporary buffer is allocated and the image's pixels read in.

 *	Pixels are unpacked from the buffer and dispersed into the VFB.

 */



#define IMREADMONO	0

#define IMREADINDEX8	1

#define IMREADALPHA	2



static int				/* Returns status		*/

#ifdef __STDC__

imIcoReadImage( int ioType, int fd, FILE *fp, ImVfb *vfb, int what )

#else

imIcoReadImage( ioType, fd, fp, vfb, what )

	int         ioType;		/* I/O flags			*/

	int         fd;			/* Input file descriptor	*/

	FILE	   *fp;			/* Input file pointer		*/

	ImVfb      *vfb;		/* Image to read into		*/

	int	    what;		/* What kind of thing to read	*/

#endif

{

	ImVfbPtr     vfbptr;		/* Pixel pointer		*/

	unsigned char	     mask;		/* Mask for extracting pixels	*/

	unsigned char	     *rbuf = NULL;	/* Read buffer			*/

	unsigned char	     *rbufptr;		/* Read buffer data pointer	*/

	int	     scanlinesize;	/* Length of scanline in bytes  */

	int	     x, y;		/* Width and Height		*/

	int	     i, j, k;		/* Counters			*/

	int	     xgroup;		/* # of groups of pixels	*/





	/* 

	 *  Allocate read buffer space

	 */

	y = ImVfbQHeight( vfb );

	x = ImVfbQWidth( vfb );

	switch ( what )

	{

	case IMREADMONO:

	case IMREADALPHA:

		scanlinesize = (x * 1) / 8;

		xgroup = x/8;	/* 8 1-bit quantities per byte		*/

		break;

	case IMREADINDEX8:

		scanlinesize = (x * 4) / 8;

		xgroup = x/2;	/* 2 4-bit quantities per byte		*/

		break;

	}

	if ( scanlinesize & 0x3 )

		scanlinesize = (scanlinesize & ~0x3) + 4;

	ImCalloc( rbuf, unsigned char*, scanlinesize, sizeof( unsigned char) );





	/*

	 *  Read in pixels and put them into the selected VFB field.

	 */

	switch ( what )

	{

	case IMREADMONO:	/* 8 pixels to a byte.				*/

		/*

		 *  An OFF bit uses the 1st CLT entry, while an ON bit

		 *  uses the 2nd CLT entry.

		 */

		for ( i = y-1; i >= 0 ; --i )

		{

			vfbptr = ImVfbQPtr( vfb, 0, i );

			if ( ImBinRead( ioType, fd, fp, rbuf, UCHAR, 1,

				scanlinesize ) == -1 )

			{

				ImReturnBinError( );

			}

			rbufptr = rbuf;



			for ( j = 0; j < xgroup; ++j )

			{

				mask = 0x80;

				for ( k = 0; k < 8; k++ )

				{

					if ( (*rbufptr & mask) == mask )

						ImVfbSIndex8( vfb, vfbptr, 1 );

					else

						ImVfbSIndex8( vfb, vfbptr, 0 );

					ImVfbSInc( vfb, vfbptr );

					mask >>= 1;

				}

				++rbufptr;

			}

		}

		break;



	case IMREADALPHA:	/* 8 pixels to a byte.				*/

		/*

		 *  An ON bit maps to 255 (opaque) and an OFF bit to

		 *  0 (transparent).

		 */

		for ( i = y-1; i >= 0 ; --i )

		{

			vfbptr = ImVfbQPtr( vfb, 0, i );

			if ( ImBinRead( ioType, fd, fp, rbuf, UCHAR, 1,

				scanlinesize ) == -1 )

			{

				ImReturnBinError( );

			}

			rbufptr = rbuf;



			for ( j = 0; j < xgroup; ++j )

			{

				mask = 0x80;		

				for ( k = 0; k < 8; k++ )

				{

					if ( (*rbufptr & mask) == mask )

						ImVfbSAlpha( vfb, vfbptr, 255 );

					else

						ImVfbSAlpha( vfb, vfbptr, 0 );

					ImVfbSInc( vfb, vfbptr );

					mask >>= 1;

				}

				++rbufptr;

			}

		}

		break;



	case IMREADINDEX8:/* 2 pixels to a byte.				*/

		for ( i = y-1; i >= 0 ; --i )

		{

			vfbptr = ImVfbQPtr( vfb, 0, i );

			if ( ImBinRead( ioType, fd, fp, rbuf, UCHAR, 1,

				scanlinesize ) == -1 )

			{

				ImReturnBinError( );

			}

			rbufptr = rbuf;



			for ( j = 0; j < xgroup; ++j )

			{

				ImVfbSIndex8( vfb, vfbptr, (*rbufptr>>4)&0xF );

				ImVfbSInc( vfb, vfbptr );

				ImVfbSIndex8( vfb, vfbptr, *rbufptr&0x0F );

				ImVfbSInc( vfb, vfbptr );

				++rbufptr;

			}

		}

		break;

	}



	free( (char *)rbuf );



	return ( 0 );

}











/*

 *  FUNCTION

 *	imIcoImageRead	-  read in an image entry

 *

 *  DESCRIPTION

 *	The next image's header, CLT, and pixels are read in and added

 *	to the tag table.

 */

static int

#ifdef __STDC__

imIcoImageRead ( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, imIcoDirEntry *icodirentryptr)

#else

imIcoImageRead ( ioType, fd, fp, flagsTable, tagTable, icodirentryptr)

	int         ioType;		/* I/O flags			*/

	int         fd;			/* Input file descriptor	*/

	FILE	   *fp;			/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable   *tagTable;		/* Tag table to add to		*/

	imIcoDirEntry *icodirentryptr;	/* Icon dir entry pointer	*/

#endif

{

	imIcoInfo    info;		/* Icon image info		*/

	ImVfb        *vfb;		/* Virtual Frame Buffers	*/

	ImClt        *clt = IMCLTNULL;	/* Colortable			*/

	ImCltPtr     cltptr;		/* Colortable entry pointer	*/

	unsigned char	     *rbuf = NULL;	/* Read buffer			*/

	unsigned char	     *rbufptr;		/* Read buffer data pointer	*/

	unsigned char	     *sbuf = NULL;	/* Skip buffer			*/

	unsigned int	     skipsize = 0;	/* Number of bytes to skip	*/

	unsigned int	     ncolors = 0;	/* Number of colors used	*/

	unsigned int	     x, y;		/* Width and Height		*/

	int	     i;			/* Counters			*/

	char	     message[100];	/* ImInfo message		*/





	/*

	 * Read in icon image info.

	 */

	if( ImBinReadStruct( ioType, fd, fp, &info, imIcoInfoFields ) == -1 )

	{

		ImReturnBinError( );

	}





	/*

	 *  Allocate a VFBs of the required size and type.

	 */

	x = info.ico_biwidth;

	y = info.ico_biheight;

	switch( info.ico_bibitcount )

	{

	case 1:		/* 1-bit index image */

	case 4:		/* 4-bit index image */

	    if((vfb = ImVfbAlloc( x, y, IMVFBINDEX8 | IMVFBALPHA)) == IMVFBNULL)

	    {

			ImErrorFatal( ImQError( ), -1, ImErrNo );

	    }

	    break;



	default:	/* unsupported image depth */

	    ImErrorFatal( ImQError( ), -1, IMEDEPTH );

	}





	/*

	 *  Allocate and read in colortable

	 */

	ncolors = (1 << info.ico_bibitcount);

	switch( ncolors )

	{

	case 2:		/* 1-bit, monochrome */

	case 16:	/* 4-bit, indexed color */

		break;

	default:

		ImErrorFatal( ImQError( ), -1, IMECLTLENGTH );

		/* output warning */

	}



	clt = ImCltAlloc( ncolors );

	if ( clt == IMCLTNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	cltptr = ImCltQFirst( clt );

	ImMalloc( rbuf, unsigned char*, ncolors*IMICORGBQUADSIZE );

	rbufptr = rbuf;

	if( ImBinRead( ioType, fd, fp, rbuf, UCHAR, 1,

		ncolors*IMICORGBQUADSIZE ) == -1 )

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

	switch( info.ico_bibitcount )

	{

	case 1:		/* 1-bit index image */

		if ( imIcoReadImage( ioType, fd, fp, vfb, IMREADMONO ) == -1 )

			return ( -1 );	/* ImErrNo already set	*/

		break;



	case 4:		/* 4-bit index image */

		if ( imIcoReadImage( ioType, fd, fp, vfb, IMREADINDEX8 ) == -1 )

			return ( -1 );	/* ImErrNo already set	*/

		break;



	}





	/*

	 * Read in AND mask image

	 */

	if ( imIcoReadImage( ioType, fd, fp, vfb, IMREADALPHA ) == -1 )

		return ( -1 );	/* ImErrNo already set	*/





	/*

	 *  Add the VFB to the tagTable.

	 */

	TagTableAppend( tagTable,

		TagEntryAlloc( "image vfb", POINTER, &(vfb) ) );





        /*

         * Output -verbose message

         */

        sprintf( message, "%d x %d", x, y );

        ImInfo( "Resolution", message );

	if ( info.ico_bibitcount == 1 )

	{

		ImInfo( "Type", "1-bit Color Indexed (XOR mask)" );

	}

	else

	{

		ImInfo( "Type", "4-bit Color Indexed (XOR mask)" );

	}

        sprintf( message, "%d Entries", ncolors );

        ImInfo( "Color Table", message );

        ImInfo( "Alpha Channel", "1-bit Monochrome (AND mask)" );



	return ( 2 );

}











/*

 *  FUNCTION

 *	imIcoWrite	-  top level control for writing Ico files

 *

 *  DESCRIPTION

 *	A file header is written out, followed by a directory entry for

 *	each image in the VFB.  The VFB's are then processed, one by one,

 *	to write them out with their individual headers, color tables, and

 *	pixels.

 */



static int                              /* Returns # of tags used       */

#ifdef __STDC__

imIcoWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imIcoWrite( pMap, ioType, fd, fp, flagsTable, tagTable )

        ImFileFormatWriteMap *pMap;     /* Write map entry to adhear to */

        int		ioType;            /* I/O flags                    */

        int		fd;                /* Input file descriptor        */

        FILE		*fp;                /* Input file pointer           */

        TagTable	*flagsTable;        /* Flags                        */

        TagTable	*tagTable;          /* Tag list to read from        */

#endif

{

        imIcoHeader	header;		/* ICO file header              */

	imIcoDirEntry	*direntries = NULL;/* List of ICO dir entries	*/

	int		ndirentries = 0;/* Number of ICO dir entries	*/

	imIcoInfo	info;		/* ICO info			*/

        ImVfb		*vfb;		/* Read in image                */

	ImClt		*clt;		/* Colortable			*/

	unsigned int		scanlinesize;	/* Length of scanline		*/

        unsigned int		x, y;		/* Width, Height                */

        int		loop;		/* Loop counter			*/

	unsigned int		iconx, icony;	/* Width and Height of icon	*/

	char		message[100];	/* ImInfo message		*/

        TagEntry       *tagEntry;       /* Tag table entry holder       */

	int		fileOffset;	/* Running file offset		*/





        /*

         *  Set byte ordering

         */

        BinByteOrder( BINLBF );





        /*

         *  Get number of images

         */

        ndirentries = TagTableQNEntry( tagTable, "image vfb" );

	ImMalloc( direntries, imIcoDirEntry *,

		(sizeof( imIcoDirEntry ) * ndirentries) );





	/*

	 *  Setup the file header and write it out

	 */

	header.ico_idReserved	= 0;

	header.ico_idType	= IMICOMAGIC;

	header.ico_idCount	= ndirentries;



        if ( ImBinWriteStruct( ioType, fd, fp, &header,

		imIcoHeaderFields )==-1)

	{

		ImReturnBinError( );

	}





	/*

	 *  Create and write out a directory entry for each VFB in the tag

	 *  table.

	 *

	 *  Note:  the directory entry for an icon includes a file offset to

	 *  that icon's data.  Fortunately, we can compute this ahead of time

	 *  by knowing the sizes and depths of the images being written out,

	 *  knowing their order in the file (the same as that in the tag

	 *  table), and knowing that they aren't being compressed (yeah!).

	 */

	fileOffset = IMICOHEADERSIZE +		/* File header		*/

		ndirentries * IMICODIRENTRYSIZE;	/* All the dir entries	*/

	for (loop = 0; loop < ndirentries; loop++)

	{

		/*

		 *  Get vfb and clt

		 */

		tagEntry = TagTableQDirect( tagTable, "image vfb", loop );

		TagEntryQValue( tagEntry, &vfb );

		clt = ImVfbQClt( vfb );		/* Might be IMCLTNULL	*/





		/*

		 *  ICO icons are constrained to be 16, 32, or 64 pixels wide

		 *  and high.  If the VFB is larger than these, we'll have to

		 *  to clip it.

		 */

		x = ImVfbQWidth( vfb );

		y = ImVfbQHeight( vfb );

		if ( x >= 64 )		iconx = x = 64;

		else if ( x >= 32 )	iconx = x = 32;

		else if ( x >= 16 )	iconx = x = 16;

		else			iconx     = 16;



		if ( y >= 64 )		icony = y = 64;

		else if ( y >= 32 )	icony = y = 32;

		else if ( y >= 16 )	icony = y = 16;

		else			icony     = 16;





		/*

		 *  Setup icon directory and write it out.

		 *

		 *  Note that only 2 color and 16 color icons are supported.

		 *  8 color icons are insufficiently described in the MicroSoft

		 *  documentation.

		 */

		direntries[loop].ico_Width	= iconx;

		direntries[loop].ico_Height	= icony;



		if ( ImVfbQFields( vfb ) & IMVFBMONO )

			/* Monochrome or only uses 2 colors.		*/

			direntries[loop].ico_ColorCount = 2;

		else

			/* Uses more than 2 colors, so use 16 color version.*/

			direntries[loop].ico_ColorCount = 16;



		direntries[loop].ico_Reserved = 0;

		direntries[loop].ico_Planes	= 1;

		if ( direntries[loop].ico_ColorCount == 2 )

			/* Monochrome					*/

			direntries[loop].ico_BitCount = 1;

		else

			/* Color					*/

			direntries[loop].ico_BitCount = 4;



		/*

		 *  Resource size is the size of the icon header (IMICOINFOSIZE),

		 *  plus the size of the color table (IMICORGBQUADSIZE for each

		 *  entry), plus the icon byte data (icony scanlines), plus the

		 *  size of the AND mask (iconx * icony bits).

		 *

		 *  The scanline size is computed by calculating the number of

		 *  bits on a scanline, then packing them into 8-bit bytes, then

		 *  rounding up to the next higher 32-bit boundary.

		 */

		scanlinesize = (iconx * direntries[loop].ico_BitCount) / 8;

		if ( scanlinesize & 0x3 )

			scanlinesize = (scanlinesize & ~0x3) + 4;

		direntries[loop].ico_BytesInRes = IMICOINFOSIZE +

		direntries[loop].ico_ColorCount * IMICORGBQUADSIZE +

			(scanlinesize * icony) + (iconx * icony / 8);



		direntries[loop].ico_ImageOffset = fileOffset;

		fileOffset += direntries[loop].ico_BytesInRes;





		/*

		 *  Write out the directory entry.

		 */

		if ( ImBinWriteStruct( ioType, fd, fp, &direntries[loop],

			imIcoDirEntryFields )==-1)

		{

			ImReturnBinError( );

		}

	}





	/*

	 *  Loop through the images and write them out.

	 */

	for (loop = 0; loop < ndirentries; loop++)

	{

		/*

		 *  Get vfb and clt

		 */

		tagEntry = TagTableQDirect( tagTable, "image vfb", loop );

		clt = ImVfbQClt( vfb );		/* Might be IMCLTNULL	*/





		/*

		 *  ICO icons are constrained to be 16, 32, or 64 pixels wide

		 *  and high.  If the VFB is larger than these, we'll have to

		 *  to clip it.

		 */

		x = ImVfbQWidth( vfb );

		y = ImVfbQHeight( vfb );

		if ( x >= 64 )		iconx = x = 64;

		else if ( x >= 32 )	iconx = x = 32;

		else if ( x >= 16 )	iconx = x = 16;

		else			iconx     = 16;



		if ( y >= 64 )		icony = y = 64;

		else if ( y >= 32 )	icony = y = 32;

		else if ( y >= 16 )	icony = y = 16;

		else			icony     = 16;





		/*

		 *  The scanline size is computed by calculating the number of

		 *  bits on a scanline, then packing them into 8-bit bytes, then

		 *  rounding up to the next higher 32-bit boundary.

		 */

		scanlinesize = (iconx * direntries[loop].ico_BitCount) / 8;

		if ( scanlinesize & 0x3 )

			scanlinesize = (scanlinesize & ~0x3) + 4;





		/*

		 *  Setup icon info and write it out

		 */

		info.ico_bisize		= IMICOINFOSIZE;

		info.ico_biwidth	= iconx;

		info.ico_biheight	= icony;

		info.ico_biplanes	= 1;

		info.ico_bibitcount	= direntries[loop].ico_BitCount;

		info.ico_bicompress	= 0;

		info.ico_bisizeimage	= scanlinesize * icony;

		info.ico_bixpm		= 0;

		info.ico_biypm		= 0;

		info.ico_biclrused	= 0;

		info.ico_biclrim	= 0;

		if ( ImBinWriteStruct( ioType, fd, fp, &info,

			imIcoInfoFields ) == -1 )

		{

			ImReturnBinError( );

		}





		/*

		 *  Output -verbose messages

		 */

		ImInfo( "Byte Order", "Least Significant Byte First" );

		sprintf( message, "%d of %d", loop+1, ndirentries );

		ImInfo( "Image", message );

		sprintf( message, "%d x %d", iconx, icony );

		ImInfo( "Resolution", message );

		if ( info.ico_bibitcount == 1 )

		{

			ImInfo( "Type", "1-bit Color Indexed (XOR mask)" );

			ImInfo( "Color Table", "2 Entries" );

		}

		else

		{

			ImInfo( "Type", "4-bit Color Indexed (XOR mask)" );

			ImInfo( "Color Table", "16 Entries" );

		}

		ImInfo( "Alpha Channel", "1-bit Monochrome (AND mask)" );





		/*

		 *  Write out the icon.

		 */

		if ( info.ico_bibitcount == 1 )

			imIcoWriteRaw1( pMap, ioType, fd, fp, vfb, clt,

				&direntries[loop] );

		else

			imIcoWriteRaw4( pMap, ioType, fd, fp, vfb, clt,

				&direntries[loop] );

	}



	free( (char *)direntries );

	return ndirentries;

}











/*

 *  FUNCTION

 *	imIcoWriteImage	-  write an icon image

 *

 *  DESCRIPTION

 *	An icon image is written out, packing multiple pixels to a byte

 *	as appropriate.

 */



#define IMWRITEMONO	0

#define IMWRITEINDEX8	1

#define IMWRITEALPHA	2



static int                              /* Returns status		*/

#ifdef __STDC__

imIcoWriteImage( int ioType, int fd, FILE *fp, ImVfb *vfb, int what )

#else

imIcoWriteImage( ioType, fd, fp, vfb, what )

        int          ioType;            /* I/O flags                    */

        int          fd;                /* Input file descriptor        */

        FILE        *fp;                /* Input file pointer           */

        ImVfb       *vfb;               /* Read in image                */

	int	     what;		/* What to write out		*/

#endif

{

        ImVfbPtr     vfbptr;            /* Vfb pixel pointer            */

        unsigned char        pvalue;            /* Pixel value                  */

        unsigned char        *wbuf;             /* Pointer to write buffer      */

        unsigned char        *wbufptr;          /* Pointer to write buffer data */

        unsigned int         scanlinesize;      /* Length of scanline           */

        unsigned char        bitfield;          /* Bitfield of 8 pixels         */

        unsigned int         x, y;              /* Width, Height                */

	int	     iconx, icony;	/* Written icon size		*/

        int	     i, j, k;		/* Counters                     */

	int	     xgroup;		/* Number of pixel groups	*/

	int	     xleft;		/* Left over for last group	*/





	/*

	 *  Compute the written icon size (rounded up to 16, 32, or 64

	 *  pixels).

	 */

	x = ImVfbQWidth( vfb );

	y = ImVfbQHeight( vfb );

	if ( x >= 64 )		iconx = x = 64;

	else if ( x >= 32 )	iconx = x = 32;

	else if ( x >= 16 )	iconx = x = 16;

	else			iconx     = 16;



	if ( y >= 64 )		icony = y = 64;

	else if ( y >= 32 )	icony = y = 32;

	else if ( y >= 16 )	icony = y = 16;

	else			icony     = 16;





	/* 

	 *  Allocate write buffer space

	 */

	switch ( what )

	{

	case IMWRITEMONO:

	case IMWRITEALPHA:

		scanlinesize = (iconx * 1) / 8;

		xgroup = x/8;	/* 8 1-bit quantities per byte		*/

		xleft  = x%8;

		break;

	case IMWRITEINDEX8:

		scanlinesize = (iconx * 4) / 8;

		xgroup = x/2;	/* 2 4-bit quantities per byte		*/

		xleft  = x%2;

		break;

	}

	if ( scanlinesize & 0x3 )

		scanlinesize = (scanlinesize & ~0x3) + 4;

	ImCalloc( wbuf, unsigned char*, scanlinesize, sizeof( unsigned char) );





	/* 

	 *  Write image

	 */

	if ( what == IMWRITEALPHA && !(ImVfbQFields( vfb ) & IMVFBALPHA) )

	{

		/* No alpha on VFB.  Write a zero alpha.		*/

		memset( wbuf, 0x00, scanlinesize );

		for ( i = 0; i < icony; ++i )

		{

			if( ImBinWrite( ioType, fd, fp, wbuf, UCHAR, 1,

				scanlinesize )== -1 )

			{

				ImReturnBinError( );

			}

		}

		free( (char *)wbuf );

		return ( 0 );

	}



	if ( y < icony )

	{

		/*

		 *  Image is smaller than icon being written.  Dump some

		 *  empty lines first.

		 */

		memset( wbuf, 0x00, scanlinesize );

		for ( i = icony - y; i > 0; i-- )

		{

			if( ImBinWrite( ioType,fd,fp,wbuf,UCHAR,

				1,scanlinesize )== -1 )

			{

				ImReturnBinError( );

			}

		}

	}





	for ( i = y-1; i >= 0; --i )

	{

		vfbptr = ImVfbQPtr( vfb, 0, i );

		memset( wbuf, 0x00, scanlinesize );

		wbufptr = wbuf;

		for( j = 0; j < xgroup; ++j )

		{

			switch ( what )

			{

			case IMWRITEMONO:	/* Pack 8 pixels to a byte	*/

				for ( bitfield = 0, k = 0; k < 8; ++k )

				{

					pvalue = ImVfbQMono( vfb, vfbptr );

					if ( pvalue )

						bitfield |= (1 << (7-k));

					ImVfbSInc( vfb, vfbptr );

				}

				break;



			case IMWRITEALPHA:/* Pack 8 pixels to a byte	*/

				for ( bitfield = 0, k = 0; k < 8; ++k )

				{

					pvalue = ImVfbQAlpha( vfb, vfbptr );

					if ( pvalue < 128 )

						bitfield |= (1 << (7-k));

					ImVfbSInc( vfb, vfbptr );

				}

				break;



			case IMWRITEINDEX8:/* Pack 2 pixels to a byte	*/

				pvalue    = ImVfbQIndex8( vfb, vfbptr ) & 0xF;

				bitfield  = pvalue << 4;

				ImVfbSInc( vfb, vfbptr );

				pvalue    = ImVfbQIndex8( vfb, vfbptr ) & 0xF;

				bitfield |= pvalue;

				ImVfbSInc( vfb, vfbptr );

				break;

			}

			*wbufptr++ = bitfield;

		}



		if ( xleft )

		{

			switch ( what )

			{

			case IMWRITEMONO:	/* Pack <8 pixels to a byte	*/

				for ( bitfield = 0, k = 0; k < xleft; ++k )

				{

					pvalue = ImVfbQMono( vfb, vfbptr );

					if ( pvalue )

						bitfield |= (1 << (7-k));

					ImVfbSInc( vfb, vfbptr );

				}

				break;



			case IMWRITEALPHA:/* Pack <8 pixels to a byte	*/

				for ( bitfield = 0, k = 0; k < xleft; ++k )

				{

					pvalue = ImVfbQAlpha( vfb, vfbptr );

					if ( pvalue < 128 )

						bitfield |= (1 << (7-k));

					ImVfbSInc( vfb, vfbptr );

				}

				break;



			case IMWRITEINDEX8:/* Pack <2 pixels to a byte	*/

				pvalue   = ImVfbQIndex8( vfb, vfbptr ) & 0xF;

				bitfield = pvalue << 4;

				ImVfbSInc( vfb, vfbptr );

				break;

			}

			*wbufptr++ = bitfield;

		}



		if( ImBinWrite( ioType,fd,fp,wbuf,UCHAR,1,scanlinesize ) == -1 )

		{

			ImReturnBinError( );

		}

	}





	free( (unsigned char*) wbuf );



	return ( 0 );

}











/*

 *  FUNCTION

 *      imIcoWriteRaw1  -  write a 1-bit non-compressed ICO file

 *

 *  DESCRIPTION

 *	An icon header and 2-color color table is written out, followed by

 *	the monochrome icon image.

 */



static int                              /* Returns # of tags used       */

#ifdef __STDC__

imIcoWriteRaw1( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, ImVfb *vfb, ImClt *clt, imIcoDirEntry *direntry )

#else

imIcoWriteRaw1( pMap, ioType, fd, fp, vfb, clt, direntry )

        ImFileFormatWriteMap *pMap;     /* Write map entry to adhear to */

        int          ioType;            /* I/O flags                    */

        int          fd;                /* Input file descriptor        */

        FILE        *fp;                /* Input file pointer           */

        ImVfb        *vfb;              /* Read in image                */

        ImClt        *clt;              /* Colortable                   */

	imIcoDirEntry *direntry;	/* ICO dir entry		*/

#endif

{

        ImVfbPtr     vfbptr;            /* Vfb pixel pointer            */

        imIcoRGBQuad cltentry;          /* Colortable RGBQuad entry     */

        ImCltPtr     cltptr;            /* Colortable entry pointer     */

        unsigned char        bitfield;          /* Bitfield of 8 pixels         */





	/*

	 *  Write colortable

	 */

	if ( clt == IMCLTNULL )

	{

		/* Define the first entry to be black, by default.	*/

		cltentry.reserved = 0;

		cltentry.blue     = 0;

		cltentry.green    = 0;

		cltentry.red      = 0;

		if ( ImBinWriteStruct( ioType, fd, fp, &cltentry,

			imIcoRGBQuadFields ) == -1 )

		{

			ImReturnBinError( );

		}



		/* And the second to be white.				*/

		cltentry.blue     = 255;

		cltentry.green    = 255;

		cltentry.red      = 255;

		if ( ImBinWriteStruct( ioType, fd, fp, &cltentry,

			imIcoRGBQuadFields ) == -1 )

		{

			ImReturnBinError( );

		}

	}

	else

	{

		/* Use the first and second entries.			*/

		cltptr = ImCltQFirst( clt );

		cltentry.reserved = 0;

		cltentry.blue     = ImCltQBlue( cltptr );

		cltentry.green    = ImCltQGreen( cltptr );

		cltentry.red      = ImCltQRed( cltptr );

		if ( ImBinWriteStruct( ioType, fd, fp, &cltentry,

			imIcoRGBQuadFields ) == -1 )

		{

			ImReturnBinError( );

		}



		if ( ImCltQNColors( clt ) > 1 )

		{

			cltptr         = ImCltQNext( clt, cltptr );

			cltentry.blue  = ImCltQBlue( cltptr );

			cltentry.green = ImCltQGreen( cltptr );

			cltentry.red   = ImCltQRed( cltptr );

		}

		else

		{

			cltentry.blue     = 255;

			cltentry.green    = 255;

			cltentry.red      = 255;

		}

		if ( ImBinWriteStruct( ioType, fd, fp, &cltentry,

			imIcoRGBQuadFields ) == -1 )

		{

			ImReturnBinError( );

		}

	}





	/*

	 *  Write XOR mask and AND mask.

	 */

	if ( imIcoWriteImage( ioType, fd, fp, vfb, IMWRITEMONO ) != 0 )

		return ( -1 );			/* ImErrNo already set	*/

	if ( imIcoWriteImage( ioType, fd, fp, vfb, IMWRITEALPHA ) != 0 )

		return ( -1 );			/* ImErrNo already set	*/



        return ( 1 );

}

 









/*

 *  FUNCTION

 *	imIcoWriteRaw4	-  write a 4-bit non-compressed ICO file

 *

 *  DESCRIPTION

 *	An icon header and 16-color color table is written out, followed by

 *	the color icon image.

 */



static int                              /* Returns # of tags used       */

#ifdef __STDC__

imIcoWriteRaw4( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, ImVfb *vfb, ImClt *clt, imIcoDirEntry *direntry )

#else

imIcoWriteRaw4( pMap, ioType, fd, fp, vfb, clt, direntry )

        ImFileFormatWriteMap *pMap;     /* Write map entry to adhear to */

        int          ioType;            /* I/O flags                    */

        int          fd;                /* Input file descriptor        */

        FILE        *fp;                /* Input file pointer           */

        ImVfb        *vfb;              /* Read in image                */

	ImClt	     *clt;		/* Colortable			*/

	imIcoDirEntry *direntry;	/* ICO dir entry		*/

#endif

{

        ImVfb        *newvfb;           /* Icon image			*/

	ImCltPtr     cltptr;		/* Colortable entry pointer	*/

	unsigned char	     *wbuf;		/* Pointer to write buffer	*/

	unsigned char	     *wbufptr;		/* Pointer to write buffer data	*/

	unsigned int	     ncolors = 0;	/* Number of colors used	*/

        unsigned int         x, y;		/* Width, Height                */

        int i;					/* Counter                      */

	unsigned int	     iconx, icony;	/* Width, Height of result icon	*/

	char	     message[100];	/* ImInfo message		*/





	x = ImVfbQWidth( vfb );

	y = ImVfbQHeight( vfb );

	if ( x >= 64 )		iconx = x = 64;

	else if ( x >= 32 )	iconx = x = 32;

	else if ( x >= 16 )	iconx = x = 16;

	else			iconx     = 16;

	if ( y >= 64 )		icony = y = 64;

	else if ( y >= 32 )	icony = y = 32;

	else if ( y >= 16 )	icony = y = 16;

	else			icony     = 16;

 

 

	/*

	 *  We may have to reduce the number of colors in the image in order

	 *  to make it fit into a 16 color table.  To generally simplify the

	 *  whole process, we'll copy the upper-left quadrant of the image

	 *  into a new temporary VFB (up to 64 x 64) and only convert that

	 *  smaller image.

	 */

	newvfb = ImVfbAlloc( iconx, icony, IMVFBINDEX8 | IMVFBALPHA );

	ImVfbClear( IMVFBINDEX8, 0,   newvfb );

	ImVfbClear( IMVFBALPHA,  255, newvfb );

	ImVfbSClt(  newvfb, ImVfbQClt( vfb ) );

	ImVfbCopy(  vfb, 0, 0, x, y, IMVFBINDEX8 | IMVFBALPHA, newvfb, 0, 0 );

	vfb = newvfb;





	/*

	 * Reduce the number of colors (if necessary) and get clt

	 */

	ImVfbToIndex( vfb, 16, vfb );

	clt = ImVfbQClt( vfb );

	ncolors = ImCltQNColors( clt );

	if( ncolors > 16 )

		ncolors = 16;







	/*

	 *  Write colortable

	 */

	cltptr = ImCltQFirst( clt );

	ImCalloc( wbuf, unsigned char*, 16, IMICORGBQUADSIZE );

	memset( wbuf, 0x00, 16 * IMICORGBQUADSIZE );

	wbufptr = wbuf;

	for( i = 0; i < ncolors; ++i )

	{

		*wbufptr++ = ImCltQBlue( cltptr );

		*wbufptr++ = ImCltQGreen( cltptr );

		*wbufptr++ = ImCltQRed( cltptr );

		wbufptr++;	/* skip reserved field			*/

		cltptr = ImCltQNext( clt, cltptr );

	}

	if ( ImBinWrite( ioType, fd, fp, wbuf, UCHAR, 1,

		16*IMICORGBQUADSIZE ) == -1 )

	{

		ImReturnBinError( );

	}

	free( wbuf );





	/*

	 *  Write XOR mask and AND mask.

	 */

	if ( imIcoWriteImage( ioType, fd, fp, vfb, IMWRITEINDEX8 ) != 0 )

		return ( -1 );			/* ImErrNo already set	*/

	if ( imIcoWriteImage( ioType, fd, fp, vfb, IMWRITEALPHA ) != 0 )

		return ( -1 );			/* ImErrNo already set	*/





	ImVfbFree( newvfb );



        return ( 1 );

}

