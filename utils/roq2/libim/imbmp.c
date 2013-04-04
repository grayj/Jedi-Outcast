/**
 **     $Header: /roq/libim/imbmp.c 1     11/02/99 4:38p Zaphod $
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

#define HEADER  "    $Header: /roq/libim/imbmp.c 1     11/02/99 4:38p Zaphod $"

/**
 **  FILE
 **	imbmp.c		-  Microsoft Windows BMP file I/O
 **
 **  PROJECT
 **	libim	-  SDSC image manipulation library
 **
 **  DESCRIPTION
 **	imbmp.c contains routines to read and write Microsoft Windows BMP
 **	(DIB) files for the image manipulation library.  Raster data read
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
 **	ImFileBmpFormat		v  file format information
 **
 **  PRIVATE CONTENTS
 **	imBmpFileHeader		t  file header information for read
 **	imBmpHeaderFields	v  imBmpHeaderInfoRead description for Bin pkg
 **
 **	imBmpFileHeaderWrite	t  file header information for write
 **	imBmpHeaderFieldsWrite	v  imBmpHeaderInfoWrite description for Bin pkg
 **
 **	imBmpRGBQuad		t  BMP colortable entry
 **	imBmpRGBQuadFields	v  imBmpRGBQuad description for Bin pkg
 **
 **	IMBMPCHECKBUFIN		m  test for full read buffer and do read
 **	imBmpWrite2Bytes	m  write two RLE bytes
 **
 **	imBmpRead		f  read BMP file
 **	imBmpReadRLE4		f  read 4-bit RLE BMP file
 **	imBmpReadRLE8		f  read 8-bit RLE BMP file
 **
 **	imBmpWriteRLE4		f  write 4-bit RLE BMP file
 **	imBmpWriteRLE8		f  write 8-bit RLE BMP file
 **	imBmpWriteRaw1		f  write 1-bit uncompressed BMP file
 **	imBmpWriteRaw4		f  write 4-bit uncompressed BMP file
 **	imBmpWriteRaw8		f  write 8-bit uncompressed BMP file
 **	imBmpWriteRaw24		f  write 24-bit uncompressed BMP file
 **
 **  HISTORY

 **	$Log: /roq/libim/imbmp.c $
 ** 
 ** 1     11/02/99 4:38p Zaphod
 **	Revision 1.14  1995/06/29  00:28:04  bduggan
 **	updated copyright year
 **
 **	Revision 1.13  1995/06/15  20:14:44  bduggan
 **	Took out 'break's at unreachable points.  Took out some
 **	unused variables.
 **
 **	Revision 1.12  1995/04/03  21:19:36  bduggan
 **	took out -DNEWMAGIC
 **
 **	Revision 1.11  1994/10/04  20:18:37  bduggan
 **	added IM to all defines
 **
 **	Revision 1.10  1994/07/20  11:42:23  nadeau
 **	Changed SDSC custom types to match v3.0 sdsc.h, added ANSI
 **	C function prototypes, updated read and write maps, updated
 **	to the new magic number scheme.
 **
 **	Revision 1.9  92/12/03  01:46:13  nadeau
 **	Corrected info messages.
 **	
 **	Revision 1.8  92/11/23  17:49:31  nadeau
 **	Removed use of IMINFOMSGWIDTH2
 **	
 **	Revision 1.7  92/11/04  11:48:24  groening
 **	put ImFIleFormat info and magic number info
 **	 from imfmt.c into this file.
 **	
 **	Revision 1.6  92/10/12  15:53:40  vle
 **	Made changes to make Cray happy.
 **	
 **	Revision 1.5  92/09/29  17:59:17  vle
 **	Added ImInfo messages.
 **	
 **	Revision 1.4  92/09/17  14:06:48  vle
 **	Fixed "scanlinesize" math bug that caused images to be cropped.
 **	
 **	Revision 1.3  92/09/03  18:19:05  vle
 **	Removed one function parameter from imBmpReadRLE4() and
 **	imBmpReadRLE8().
 **	
 **	Revision 1.2  92/08/31  17:20:57  vle
 **	Updated copyright notice.
 **	
 **	Revision 1.1  92/08/12  16:46:02  vle
 **	Initial revision
 **	
 **	
 **/

//#include <unistd.h>
#include "iminternal.h"


/**
 **  FORMAT
 **	bmp	-  Microsoft Windows bitmap image
 **
 **  AKA
 **	dib (Device Independent Bitmap)
 **
 **  FORMAT REFERENCES
 **	Microsoft Windows 3.1 Programmer's Reference, volume 3, Microsoft
 **	Press, 1992.
 **
 **	Microsoft Windows 3.1 Programmer's Reference, volume 3, Microsoft
 **	Press, 1992.
 **
 **	Graphics File Formats, David C. Kay, John R. Levine, McGraw-Hill,
 **	1992.
 **
 **	Supercharged Bitmapped Graphics, Steve Rimmer, McGraw-Hill, 1992.
 **
 **  CODE CREDITS
 **	Custom development, Vinh Le, San Diego Supercomputer Center, 1992.
 ** 
 **  DESCRIPTION
 ** 	Microsoft's BMP (BitMaP) format, also known as DIB (Device
 ** 	Independent Bitmap) format, is used in Microsoft's Windows
 ** 	versions 3.0 and up for bitmap image storage.
 ** 
 ** 	The BMP format consists of four segments:
 ** 
 ** 		Bitmap-File Header 
 ** 		Bitmap-Info Header
 ** 		ColorTable
 ** 		BitmapBits
 ** 
 **
 ** 	Bitmap-File Header (14 bytes)
 ** 	------------------
 ** 
 ** 	The Bitmap-File Header contains information about the type,
 ** 	size, and layout of a device-independent bitmap file.
 ** 
 ** 	Name		Type	Size		Comments
 ** 	----		----	----		--------
 ** 	bfType		uint	2 bytes		type of file, must be BM
 ** 	bfSize		dword	4 bytes		size of the file in bytes
 ** 	bfReserved1	uint	2 bytes		reserved, must be zero
 ** 	bfReserved2	uint	2 bytes		reserved, must be zero
 ** 	bfOffBits	dword	4 bytes		offset in bytes from the
 ** 						beginning of the file to
 ** 						the actual bitmap data in
 ** 						the file
 ** 
 **	( Note:  types above and below are as reported in the Microsoft
 **	specification and refer to IBM PC-sized values.  Usage of the
 **	SDSC Binary I/O library by this code insures that an appropriate
 **	portable mapping is made from PC types and sizes to this host's. )
 **
 ** 	typedef struct tagBITMAPFILEHEADER
 **	{ 
 ** 		uint	bfType;
 ** 		dword	bfSize;
 ** 		uint	bfReserved1;
 ** 		uint	bfReserved2;
 ** 		dword	bfOffBits;
 ** 	} BITMAPFILEHEADER;
 ** 
 **
 ** 	Bitmap-Info Header (40 bytes)
 ** 	------------------
 ** 
 ** 	The Bitmap-Info Header contains information about the bitmap's
 ** 	dimensions, compression type, and color format.
 ** 
 ** 	Name		Type	Size		Comments
 ** 	----		----	----		--------
 ** 	biSize		dword	4 bytes		number of bytes in
 ** 						Bitmap-Info Header
 ** 	biWidth		long	4 bytes		width, in pixels
 ** 	biHeight	long	4 bytes		height, in pixels
 ** 	biPlanes	word	2 bytes		number of planes for the
 ** 						display device, must be 1
 ** 	biBitCount	word	2 bytes		bits per pixel, must be 1,
 ** 						4, 8 or 24
 ** 	biCompression	dword	4 bytes		BI_RGB(0), no compression
 ** 						BI_RLE8(1), RLE compression of
 ** 							8-bits/pixel image
 ** 						BI_RLE4(2), RLE compression of
 ** 							4-bits/pixel image
 ** 	biSizeImage	dword	4 bytes		image size in bytes
 ** 	biXPelsPerMeter	long	4 bytes		horizontal resolution in
 ** 						pixels per meter, used for
 ** 						aspect scaling
 ** 	biYPelsPerMeter	long	4 bytes		vertical resolution in
 ** 						pixels per meter, used for
 ** 						aspect scaling
 ** 	biClrUsed	dword	4 bytes		number of color indexes
 ** 						actually used by bitmap
 ** 	biClrImportant	dword	4 bytes		number of color absolutely
 ** 						necessary to display the
 ** 						image; if 0, all colors are
 ** 						needed
 ** 
 ** 	typedef struct tagBITMAPINFOHEADER
 **	{
 ** 		dword	biSize;
 ** 		long	biWidth;
 ** 		long	biHeight;
 ** 		word	biPlanes;
 ** 		word	biBitCount;
 ** 		dword	biCompression;
 ** 		dword	biSizeImage;
 ** 		long	biXPelsPerMeter;
 ** 		long	biYPelsPerMeter;
 ** 		dword	biClrUsed;
 ** 		dword	biClrImportant;
 ** 	} BITMAPINFOHEADER;
 ** 		
 **
 ** 	ColorTable (4 bytes per color)
 ** 	----------
 ** 
 ** 	ColorTable is an array of RGBQuad's.  Each RGBQuad contains color
 ** 	intensity values for Red, Green, and Blue.
 ** 
 ** 	RGBQuad
 ** 	-------
 ** 
 ** 	Name		Type	Size		Comment
 ** 	----		----	----		-------
 ** 	rgbBlue		byte	1 byte		intensity of blue
 ** 	rgbGreen	byte	1 byte		intensity of green
 ** 	rgbRed		byte	1 byte		intensity of red
 ** 	rgbReserved	byte	1 byte		unused
 ** 
 ** 	typedef struct tagRGBQUAD
 **	{
 ** 		byte	rgbBlue;
 ** 		byte	rgbGreen;
 ** 		byte	rgbRed;
 ** 		byte	rgbReserved;
 ** 	} RGBQUAD;
 **  
 **
 ** 	BitmapBits
 ** 	----------
 ** 
 ** 	BitmapBits is the actual bitmap image data.  It is an array
 ** 	of bytes representing consecutive scanlines.  The number of
 ** 	bytes varies with the resolution and number of colors.  Each
 ** 	scanline must be zero-padded to a 32-bit boundary.  Scanlines
 ** 	are stored from the bottom up; that is, the first-byte is in
 ** 	the lower-left corner of the image.
 ** 
 ** 	The biBitCount field of BITMAPINFOHEADER gives the bits per
 ** 	pixel information for the bitmap.  Valid values are 1
 ** 	(monochrome image, 2 color image), 4 (16 color image),
 ** 	8 (256 color image), or 24 (true color).
 ** 
 ** 	Monochrome images have 2 color colortables.  They are non-
 ** 	compressed, but are pixel packed.  8 pixels are packed into
 ** 	each stored byte.  A clear bit means use the first color.
 ** 	A set bit means use the second color.
 ** 
 ** 	4-bits/pixel images can be non-compressed or compressed.
 ** 	Non-compressed images are pixel packed.  Each byte stores
 ** 	two 4-bit pixels.  Compressed images are compressed with
 ** 	a 4-bit RLE (RLE4) variant described below.
 ** 
 ** 	8-bits/pixel images can be non-compressed or compressed.
 ** 	For non-compressed images, each byte stores one 8-bit pixel.
 ** 	Compressed images are compressed with an 8-bit RLE (RLE8)
 ** 	variant described below.
 ** 
 ** 	24-bits/pixel images are stored non-compressed.  Each pixel
 ** 	is represented by 3 bytes.  Each byte represents Blue,
 ** 	Green, and Red, in this order.
 ** 
 ** 	RLE4 - 4-bit RLE Encoding Scheme
 **     --------------------------------
 **  
 **     The 4-bit RLE encoding scheme consists of two modes, Encoded
 **     and Absolute.
 **     
 **     The Encoded Mode consists of two byte pairs.
 **     1st byte        length of run
 **     2nd byte        This byte contains 2 color indicies, one in
 **                       the high 4 bits and one in the low 4 bits.
 **                       The run is filled with alternating sequences
 **                       of these indicies.
 **
 **     if the 1st byte is zero (0x00), then the 2nd byte is interpreted
 **     as follows:
 **             0x00    End of scanline
 **             0x01    End of bitmap
 **             0x02    "Delta".  The next two bytes indicate right
 **                       and down movements.
 **             0x03-   Copy the next 3 to 255 4-bit byte halves
 **             0xFF      literally.  This is the Absolute Mode.
 **
 **     The Absolute Mode is described as above.  It always has zero
 **     (0x00) as the 1st byte.  The 2nd byte can range between 0x03
 **     to 0xFF as described.
 **
 **     ex.     Compressed              Expanded
 **             ----------              --------
 **             03 04                   0 4 0
 **             05 06                   0 6 0 6 0
 **             00 06 45 56 67 00       4 5 5 6 6 7
 **             00 02 05 01             Move 5 right and 1 down
 **             04 78                   7 8 7 8
 **             00 00                   End of scanline
 **             09 1E                   1 E 1 E 1 E 1 E 1
 **             00 01                   End of RLE bitmap
 **
 **             Note: Runs must be word aligned!
 **
 **	RLE8 - 8-bit RLE Encoding Scheme
 **     --------------------------------
 **
 **     The 8-bit RLE encoding scheme consists of two modes, Encoded
 **     and Absolute.
 **
 **     The Encoded Mode consists of two byte pairs.
 **     1st byte        length of run
 **     2nd byte        byte to repeat in run (color index for pixel)
 **
 **     if the 1st byte is zero (0x00), then the 2nd byte is interpreted
 **     as follows:
 **             0x00    End of scanline
 **             0x01    End of bitmap
 **             0x02    "Delta".  The next two bytes indicate right
 **                       and down movements.
 **             0x03-   Copy the next 3 - 255 bytes literally.
 **             0xFF      This is the Absolute Mode.
 **
 **     The Absolute Mode is described as above.  It always has zero
 **     (0x00) as the 1st byte.  The 2nd byte can range between 0x03
 **     to 0xFF as described.
 **         
 **     ex.     Compressed              Expanded
 **             ----------              --------
 **             03 04                   04 04 04
 **             05 06                   06 06 06 06 06
 **             00 03 45 56 67 00       45 56 67
 **             00 02 05 01             Move 5 right and 1 down
 **             02 78                   78 78
 **             00 00                   End of scanline
 **             09 1E                   1E 1E 1E 1E 1E 1E 1E 1E 1E
 **             00 01                   End of RLE bitmap
 **
 **             Note: Runs must be word aligned!
 **
 **/


/*
 *  FUNCTION DECLARATIONS
 */
#ifdef __STDC__
static int imBmpRead( int, int, FILE *, TagTable *, TagTable * );
static int imBmpReadRLE4( int, int, FILE *, ImVfb *, unsigned int );
static int imBmpReadRLE8( int, int, FILE *, ImVfb *, unsigned int );

static int imBmpWriteRaw1( ImFileFormatWriteMap *, int, int, FILE *,
	TagTable *, TagTable * );
static int imBmpWriteRaw4( ImFileFormatWriteMap *, int, int, FILE *,
	TagTable *, TagTable * );
static int imBmpWriteRaw8( ImFileFormatWriteMap *, int, int, FILE *,
	TagTable *, TagTable * );
static int imBmpWriteRaw24( ImFileFormatWriteMap *, int, int, FILE *,
	TagTable *, TagTable * );
static int imBmpWriteRLE4( ImFileFormatWriteMap *, int, int, FILE *,
	TagTable *, TagTable * );
static int imBmpWriteRLE8( ImFileFormatWriteMap *, int, int, FILE *,
	TagTable *, TagTable * );
#else
static int imBmpRead( ), imBmpReadRLE4( ), imBmpReadRLE8( );
static int imBmpWriteRaw1( ), imBmpWriteRaw4( ), imBmpWriteRaw8( ) ;
static int imBmpWriteRaw24( ), imBmpWriteRLE4( ), imBmpWriteRLE8( ) ;
#endif





/*
 *  FORMAT INFORMATION
 *	imBmpNames		-  format's name and aliases
 *	imBmpReadMap		-  read attributes
 *	imBmpWriteMap		-  write attributes
 *	imBmpMagicNumber	-  magic number
 *	imBmpMagic		-  list of magic numbers
 *	ImFileBmpFormat		-  master format description
 */
static char *imBmpNames[ ] = { "bmp", "dib", NULL };
static ImFileFormatReadMap imBmpReadMap[ ] =
{
        /* in                   out                                     */
        /* type,ch,dep, attr.   VFB type        attr.                   */
        { RGB,3,8,      0,      IMVFBRGB,       0 },
        { IN,1,8,       C,      IMVFBINDEX8,    C },
        { IN,1,4,       C,      IMVFBINDEX8,    C },
        { IN,1,8,       RLE|C,  IMVFBINDEX8,    C },
        { IN,1,4,       RLE|C,  IMVFBINDEX8,    C },
        { IN,1,1,       C,      IMVFBINDEX8,    C },
        { -1,           0,      -1,             0 },
};
static ImFileFormatWriteMap imBmpWriteMap[ ] =
{
        /* in                   out                                     */
        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */
        { IMVFBMONO,    0,      IN,1,1,         C,      imBmpWriteRaw1 },
        { IMVFBINDEX8,  C,      IN,1,8,         C,      imBmpWriteRaw8 },
        { IMVFBINDEX8,  C,      IN,1,4,         C,      imBmpWriteRaw4 },
        { IMVFBINDEX8,  C,      IN,1,8,         RLE|C,  imBmpWriteRLE8 },
        { IMVFBINDEX8,  C,      IN,1,4,         RLE|C,  imBmpWriteRLE4 },
        { IMVFBRGB,     0,      RGB,3,8,        0,      imBmpWriteRaw24 },
        { -1,           0,      -1,             0,      NULL }
};

static unsigned char imBmpMagicNumber[ ] = { 'B', 'M' };
static ImFileMagic imBmpMagic[ ] =
{
	{ 0, 2, imBmpMagicNumber },
	{ 0, 0, NULL },
};

ImFileFormat ImFileBmpFormat =
{
	imBmpNames,					/* Names	*/
	"Windows bitmap image file",			/* Description	*/
	"Microsoft",					/* Creator	*/
	"1-, 4-, and 8-bit color index, and 24-bit RGB color, and\n\
- and 8-bit RLE compressed images.",			/* Read support	*/
"1-, 4-, and 8-bit color index, and 24-bit RGB color, and\n\
- and 8-bit RLE compressed images.",			/* Write support*/
	imBmpMagic,					/* Magic #'s	*/
	IMNOMULTI, IMPIPE,				/* Read?	*/
	IMNOMULTI, IMNOPIPE,				/* Write?	*/
	imBmpRead, imBmpReadMap, imBmpWriteMap		/* Maps		*/
};





/*
 *  TYPEDEF & STRUCTURE
 *	imBmpHeaderInfoRead	-  BMP file header information for read
 *	imBmpHeaderFieldsRead	-  BMP file header fields for binary pkg.
 *	imBmpHeaderInfoWrite	-  BMP file header information for write
 *	imBmpHeaderFieldsWrite	-  BMP file header fields for binary pkg.
 *	imBmpRGBQuad		-  BMP color information
 *	imBmpRGBQuadFields	-  BMP color information fields for binary
 *				   pkg.
 *
 *  DESCRIPTION
 *	imBmpHeaderInfo contains both the Bitmap-File Header and
 *	Bitmap-Info Header as described in the format specifications.
 *
 *	The imBmpRGBQuad is one entry in the colortable.
 */

typedef struct imBmpHeaderInfoRead
{
	sdsc_uint32 bmp_size;		/* size of file in bytes	*/
	sdsc_uint16 bmp_reserved1;	/* reserved1, must be zero	*/
	sdsc_uint16 bmp_reserved2;	/* reserved2, must be zero	*/
	sdsc_uint32 bmp_offsetbits;	/* offset to actual bitmap data	*/
	sdsc_uint32 bmp_bisize;		/* # bytes in bitmap info header*/
	sdsc_uint32 bmp_biwidth;	/* image width in pixels	*/
	sdsc_uint32 bmp_biheight;	/* image height in pixels	*/
	sdsc_uint16 bmp_biplanes;	/* # planes for device, always 1*/
	sdsc_uint16 bmp_bibitcount;	/* bits/pixel (1, 4, 8, or 24)	*/
	sdsc_uint32 bmp_bicompress;	/* compression method		*/
	sdsc_uint32 bmp_bisizeimage;	/* image size in bytes		*/
	sdsc_uint32 bmp_bixpm;		/* x pixels per meter		*/
	sdsc_uint32 bmp_biypm;		/* y pixels per meter		*/
	sdsc_uint32 bmp_biclrused;	/* # colors actually used in CLT*/
	sdsc_uint32 bmp_biclrim;	/* # required colors to display */
} imBmpHeaderInfoRead;


static BinField imBmpHeaderFieldsRead[ ] =
{
	{ UINT32, 4, 1 },		/* bmp_size			*/
	{ UINT16, 2, 1 },		/* bmp_reserved1		*/
	{ UINT16, 2, 1 },		/* bmp_reserved2		*/
	{ UINT32, 4, 1 },		/* bmp_offsetbits		*/
	{ UINT32, 4, 1 },		/* bmp_bisize			*/
	{ UINT32, 4, 1 },		/* bmp_biwidth			*/
	{ UINT32, 4, 1 },		/* bmp_biheight			*/
	{ UINT16, 2, 1 },		/* bmp_biplanes			*/
	{ UINT16, 2, 1 },		/* bmp_bibitcount		*/
	{ UINT32, 4, 1 },		/* bmp_bicompress		*/
	{ UINT32, 4, 1 },		/* bmp_bisizeimage		*/
	{ UINT32, 4, 1 },		/* bmp_bixpm			*/
	{ UINT32, 4, 1 },		/* bmp_biypm			*/
	{ UINT32, 4, 1 },		/* bmp_biclrused		*/
	{ UINT32, 4, 1 },		/* bmp_biclrim			*/
	{ 0, 0, 0 }
};


typedef struct imBmpHeaderInfoWrite
{
	sdsc_uint16 bmp_type;		/* type of image, must be 'BM'	*/
	sdsc_uint32 bmp_size;		/* size of file in bytes	*/
	sdsc_uint16 bmp_reserved1;	/* reserved1, must be zero	*/
	sdsc_uint16 bmp_reserved2;	/* reserved2, must be zero	*/
	sdsc_uint32 bmp_offsetbits;	/* offset to actual bitmap data	*/
	sdsc_uint32 bmp_bisize;		/* # bytes in bitmap info header*/
	sdsc_uint32 bmp_biwidth;	/* image width in pixels	*/
	sdsc_uint32 bmp_biheight;	/* image height in pixels	*/
	sdsc_uint16 bmp_biplanes;	/* # planes for device, always 1*/
	sdsc_uint16 bmp_bibitcount;	/* bits/pixel (1, 4, 8, or 24)	*/
	sdsc_uint32 bmp_bicompress;	/* compression method		*/
	sdsc_uint32 bmp_bisizeimage;	/* image size in bytes		*/
	sdsc_uint32 bmp_bixpm;		/* x pixels per meter		*/
	sdsc_uint32 bmp_biypm;		/* y pixels per meter		*/
	sdsc_uint32 bmp_biclrused;	/* # colors actually used from CLT*/
	sdsc_uint32 bmp_biclrim;	/* # required colors to display	*/
} imBmpHeaderInfoWrite;


static BinField imBmpHeaderFieldsWrite[ ] =
{
	{ UINT16, 2, 1 },		/* bmp_type			*/
	{ UINT32, 4, 1 },		/* bmp_size			*/
	{ UINT16, 2, 1 },		/* bmp_reserved1		*/
	{ UINT16, 2, 1 },		/* bmp_reserved2		*/
	{ UINT32, 4, 1 },		/* bmp_offsetbits		*/
	{ UINT32, 4, 1 },		/* bmp_bisize			*/
	{ UINT32, 4, 1 },		/* bmp_biwidth			*/
	{ UINT32, 4, 1 },		/* bmp_biheight			*/
	{ UINT16, 2, 1 },		/* bmp_biplanes			*/
	{ UINT16, 2, 1 },		/* bmp_bibitcount		*/
	{ UINT32, 4, 1 },		/* bmp_bicompress		*/
	{ UINT32, 4, 1 },		/* bmp_bisizeimage		*/
	{ UINT32, 4, 1 },		/* bmp_bixpm			*/
	{ UINT32, 4, 1 },		/* bmp_biypm			*/
	{ UINT32, 4, 1 },		/* bmp_biclrused		*/
	{ UINT32, 4, 1 },		/* bmp_biclrim			*/
	{ 0, 0, 0 }
};


typedef struct imBmpRGBQuad
{
	unsigned char    blue;		/* blue intensity		*/
	unsigned char    green;		/* green intensity		*/
	unsigned char    red;		/* red intensity		*/
	unsigned char    reserved;	/* unused			*/
} imBmpRGBQuad;

static BinField imBmpRGBQuadFields[ ] =
{
	{ UCHAR, 1, 1 },		/* blue				*/
	{ UCHAR, 1, 1 },		/* green			*/
	{ UCHAR, 1, 1 },		/* red				*/
	{ UCHAR, 1, 1 },		/* reserved			*/
	{ 0, 0, 0 }
};





/*
 *  CONSTANTS
 *	BMP*	-  assorted useful BMP constants
 */

#define IMBMPMAGIC_MBF		0x424d
				/* magic value, 'BM' */
#define IMBMPMAGIC_LBF		0x4d42
				/* magic value, 'BM' in reverse byte order */
#define IMBMPFILEHEADERSIZE	14
				/* 14 bytes */
#define IMBMPINFOHEADERSIZE	40
				/* 40 bytes */
#define IMBMPHEADERSIZE		(IMBMPFILEHEADERSIZE + IMBMPINFOHEADERSIZE)
				/* size of complete header */
#define IMBMPRGBQUADSIZE	4
				/* 4 bytes */
#define IMBMPRGB		0
				/* no compression */
#define IMBMPRLE8		1
				/* 8-bit RLE compression */
#define IMBMPRLE4		2
				/* 4-bit RLE compression */
#define IMBLKSIZE		(BUFSIZ)
				/* number of bytes to read */

#define IMBMPCHECKBUFIN						\
if( bufin > (IMBLKSIZE-1) )						\
{									\
	if( ImBinRead( ioType,fd,fp,buf,UCHAR,1,IMBLKSIZE )==-1)		\
	{								\
		ImReturnBinError( );					\
	}								\
	bufin = 0;							\
}									\







/*

 *  FUNCTION

 *	imBmpReadRLE8	- Read a BMP 8-bit RLE compressed image

 *

 *  DESCRIPTION

 *	This function reads in and decompresses a BMP 8-bit RLE image

 *	into a vfb.  See encoding scheme in FORMAT section.

 */
static int			/* Returns status			*/
#ifdef __STDC__
imBmpReadRLE8( int ioType, int fd, FILE *fp, ImVfb *vfb, unsigned int y )
#else
imBmpReadRLE8( ioType, fd, fp, vfb, y )
	int         ioType;	/* I/O flags				*/
	int         fd;		/* Input file descriptor		*/
	FILE	   *fp;		/* Input file pointer			*/
	ImVfb      *vfb;	/* Pointer to image			*/
	unsigned int y;		/* height of image			*/
#endif
{	
	ImVfbPtr     vfbptr;	/* Pointer into vfb image		*/
	unsigned char pair[ 2 ];/* RLE code pair			*/
	unsigned char ampair[ 2 ];/* RLE Absolute Mode pair		*/
	int ycount;		/* Current scanline			*/
	unsigned char buf[ IMBLKSIZE ];	/* File input buffer		*/
	int bufin;		/* File input buffer index		*/
	int i;			/* Counters				*/

	/*
	 * The image is stored from the bottom up; that is,
	 * the first scanline in the file is the last
	 * scanline in the image.
	 */

	/*
	 * Go to the last scanline in the vfb
	 */
	vfbptr = ImVfbQPtr( vfb, 0, y-1 );

	/*
	 * Initializations
	 */
	ycount = 0;
	bufin = IMBLKSIZE;

	/*
	 * Read in and decompress image bitmap
	 */
	while( ycount < y)
	{
		/*
		 * Read in file data into buf
		 */
		IMBMPCHECKBUFIN;
		pair[0] = buf[ bufin++ ];
		IMBMPCHECKBUFIN;
		pair[1] = buf[ bufin++ ];

		if ( pair[0] != 0x00 )
		{
			/* Encoded Mode */
			for( i = 0; i < pair[0]; ++i )
			{
				ImVfbSIndex8( vfb, vfbptr, pair[1] );
				vfbptr = ImVfbQRight( vfb, vfbptr );
			}
			continue;
		}

		switch( pair[1] )
		{
		case 0x00:	/* end of scanline */
			vfbptr = ImVfbQUp( vfb, vfbptr );
			vfbptr = ImVfbQUp( vfb, vfbptr );
			++ycount;
			break;

		case 0x01:	/* end of bitmap */
			if( ycount != y-1 )
			{
				/* issue warning to user */
				ImErrorWarning( "Unexpected end of bitmap.  Input file may be corrupted!", -1, IMEDECODING );
			}
			return( 1 );

		case 0x02:	/* "delta", right and down movement */
			/*
			 * Read in file data into buf
			 */
			IMBMPCHECKBUFIN;
			pair[0] = buf[ bufin++ ];
			IMBMPCHECKBUFIN;
			pair[1] = buf[ bufin++ ];

			for( i = 0; i < pair[0]; ++i )
			{
				vfbptr = ImVfbQRight( vfb, vfbptr );
			}
			for( i = 0; i < pair[1]; ++i )
			{
				vfbptr = ImVfbQDown( vfb, vfbptr );
			}
			break;

		default:	/* Absolute Mode */
			for( i = 0; i < pair[1]/2; ++i )
			{
				/*
				 * Read in file data into buf
				 */
				IMBMPCHECKBUFIN;
				ampair[0] = buf[ bufin++ ];
				IMBMPCHECKBUFIN;
				ampair[1] = buf[ bufin++ ];

				ImVfbSIndex8( vfb, vfbptr, ampair[0] );
				vfbptr = ImVfbQRight( vfb, vfbptr );

				ImVfbSIndex8( vfb, vfbptr, ampair[1] );
				vfbptr = ImVfbQRight( vfb, vfbptr );
			}
			if( (pair[1]%2) != 0 )
			{
				/*
				 * Read in file data into buf
				 */
				IMBMPCHECKBUFIN;
				ampair[0] = buf[ bufin++ ];
				IMBMPCHECKBUFIN;
				ampair[1] = buf[ bufin++ ];

				ImVfbSIndex8( vfb, vfbptr, ampair[0] );
				vfbptr = ImVfbQRight( vfb, vfbptr );
			}
			break;
		} /* end switch */
    	}
	return( 1 );
}





/*
 *  FUNCTION
 *	imBmpReadRLE4	- Read a BMP 4-bit RLE compressed image
 *
 *  DESCRIPTION
 *	This function reads in and decompresses a BMP 4-bit RLE image
 *	into a vfb.  See encoding scheme in FORMAT section.
 */
static int			/* Returns status			*/
#ifdef __STDC__
imBmpReadRLE4( int ioType, int fd, FILE *fp, ImVfb *vfb, unsigned int y )
#else
imBmpReadRLE4( ioType, fd, fp, vfb, y )
	int         ioType;	/* I/O flags				*/
	int         fd;		/* Input file descriptor		*/
	FILE	   *fp;		/* Input file pointer			*/
	ImVfb      *vfb;	/* Pointer to image			*/
	unsigned int y;		/* height of image			*/
#endif
{	
	ImVfbPtr     vfbptr;	/* Pointer into vfb image		*/
	unsigned char pair[ 2 ];/* RLE code pair			*/
	unsigned char ampair[ 2 ];/* RLE Absolute Mode pair		*/
	int ycount;		/* Current scanline			*/
	unsigned char buf[ IMBLKSIZE ];	/* File input buffer		*/
	int bufin;		/* File input buffer pointer		*/
	int i;			/* Counters				*/

	/*
	 * The image is stored from the bottom up; that is,
	 * the first scanline in the file is the last
	 * scanline in the image.
	 */

	/*
	 * Go to the last scanline in the vfb
	 */
	vfbptr = ImVfbQPtr( vfb, 0, y-1 );

	/*
	 * Initializations
	 */
	ycount = 0;
	bufin = IMBLKSIZE;

	/*
	 * Read in and decompress image bitmap
	 */
	while( ycount < y )
	{
		/*
		 * Read in file data into buf
		 */
		IMBMPCHECKBUFIN;
		pair[0] = buf[ bufin++ ];
		IMBMPCHECKBUFIN;
		pair[1] = buf[ bufin++ ];

		if ( pair[0] != 0x00 )
		{
			/* Encoded Mode */
			for( i = 0; i < pair[0]/2; ++i )
			{
				ImVfbSIndex8( vfb, vfbptr, (pair[1] >> 4) );
				vfbptr = ImVfbQRight( vfb, vfbptr );

				ImVfbSIndex8( vfb, vfbptr, pair[1] & 0x0F );
				vfbptr = ImVfbQRight( vfb, vfbptr );
			}
			if( pair[0]%2 != 0 )
			{
				ImVfbSIndex8( vfb, vfbptr, (pair[1] >> 4) );
				vfbptr = ImVfbQRight( vfb, vfbptr );
			}
			continue;
		}

		switch( pair[1] )
		{
		case 0x00:	/* end of scanline */
			vfbptr = ImVfbQUp( vfb, vfbptr );
			vfbptr = ImVfbQUp( vfb, vfbptr );
			++ycount;
			break;

		case 0x01:	/* end of bitmap */
			if( ycount != y-1 )
			{
				/* issue warning to user */
				ImErrorWarning( "Unexpected end of bitmap.  Input file may be corrupted!", -1, IMEDECODING );
			}
			return( 1 );

		case 0x02:	/* "delta", right and down movement */
			/*
			 * Read in file data into buf
			 */
			IMBMPCHECKBUFIN;
			pair[0] = buf[ bufin++ ];
			IMBMPCHECKBUFIN;
			pair[1] = buf[ bufin++ ];

			for( i = 0; i < pair[0]; ++i )
			{
				vfbptr = ImVfbQRight( vfb, vfbptr );
			}
			for( i = 0; i < pair[1]; ++i )
			{
				vfbptr = ImVfbQDown( vfb, vfbptr );
			}
			break;

		default:	/* Absolute Mode */
			for( i = 0; i < pair[1]/4; ++i )
			{
				/*
				 * Read in file data into buf
				 */
				IMBMPCHECKBUFIN;
				ampair[0] = buf[ bufin++ ];
				IMBMPCHECKBUFIN;
				ampair[1] = buf[ bufin++ ];

				ImVfbSIndex8( vfb, vfbptr,
					(ampair[0] >> 4) );
				vfbptr = ImVfbQRight( vfb, vfbptr );

				ImVfbSIndex8( vfb, vfbptr,
					ampair[0] & 0x0F );
				vfbptr = ImVfbQRight( vfb, vfbptr );

				ImVfbSIndex8( vfb, vfbptr,
					(ampair[1] >> 4) );
				vfbptr = ImVfbQRight( vfb, vfbptr );

				ImVfbSIndex8( vfb, vfbptr,
					ampair[1] & 0x0F );
				vfbptr = ImVfbQRight( vfb, vfbptr );
			}
			if( (pair[1]%4) != 0 )
			{
				/*
				 * Read in file data into buf
				 */
				IMBMPCHECKBUFIN;
				ampair[0] = buf[ bufin++ ];
				IMBMPCHECKBUFIN;
				ampair[1] = buf[ bufin++ ];

				ImVfbSIndex8( vfb, vfbptr,
					(ampair[0] >> 4) );
				vfbptr = ImVfbQRight( vfb, vfbptr );

				if( pair[1]%4 > 1 )
				{
					ImVfbSIndex8( vfb, vfbptr,
						ampair[0] & 0x0F );
					vfbptr = ImVfbQRight( vfb,
						vfbptr );
				}
				if( pair[1]%4 > 2 )
				{
					ImVfbSIndex8( vfb, vfbptr,
						ampair[1] >> 4 );
					vfbptr = ImVfbQRight( vfb,
						vfbptr );
				}
			}
			break;
		} /* end switch */
    	}
	return( 1 );
}





/*
 *  FUNCTION
 *	imBmpRead	-  read a Microsoft Windows Bmp file
 *
 *  DESCRIPTION
 *	The file header is read and the size of the image determined.
 *	Space is allocated for the VFB and the file is read into the
 *	VFB.
 */

static int				/* Returns # tags read in	*/
#ifdef __STDC__
imBmpRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )
#else
imBmpRead( ioType, fd, fp, flagsTable, tagTable )
	int         ioType;		/* I/O flags			*/
	int         fd;			/* Input file descriptor	*/
	FILE	   *fp;			/* Input file pointer		*/
	TagTable   *flagsTable;		/* Flags			*/
	TagTable   *tagTable;		/* Tag table to add to		*/
#endif
{
	imBmpHeaderInfoRead  header;	/* BMP file header		*/
	ImVfb         *vfb;		/* Virtual Frame Buffer		*/
	ImVfbPtr       vfbptr;		/* Pixel pointer		*/
	ImClt         *clt = IMCLTNULL;	/* Colortable			*/
	ImCltPtr       cltptr;		/* Colortable entry pointer	*/
	int	       x,y;		/* Convenient short names	*/
	unsigned char  mask;		/* Mask for extracting pixels	*/
	sdsc_uint16    bmp_type;	/* BMP type (magic number)	*/
	unsigned char *rbuf = NULL;	/* read buffer			*/
	unsigned char *rbufptr;		/* read buffer data pointer	*/
	unsigned char *sbuf = NULL;	/* skip buffer			*/
	unsigned int   skipsize = 0;	/* number of bytes to skip	*/
	unsigned int   scanlinesize;	/* length of scanline in bytes  */
	unsigned int   ncolors = 0;	/* number of colors used	*/
	unsigned int   i, j, k;		/* Counters			*/
	char	       message[100];	/* ImInfo message		*/


	/*
	 * Set Binary Package byte order and read in BMP type
	 */
	BinByteOrder( BINLBF );
	if( ImBinRead( ioType, fd, fp, &bmp_type, UINT16, 2, 1) == -1 )
	{
		ImReturnBinError( );
	}

	/*
	 * Use BMP type (magic number) to determine byte ordering of rest
	 * of file 
	 */

	switch( bmp_type )
	{
	case IMBMPMAGIC_LBF:
		/* no need to change byte ordering */
		ImInfo( "Byte Order", "Least Significant Byte First" );
		break;
	case IMBMPMAGIC_MBF:
		ImInfo( "Byte Order", "Most Significant Byte First" );
		BinByteOrder( BINMBF );
		break;
	default:
		/*
		 * Bad BMP type (magic number)
		 */
		ImErrorFatal( ImQError( ), -1, IMEMAGIC );
	}

	/*
	 * Read in rest of Bmp file header
	 */
	if( ImBinReadStruct( ioType, fd, fp, &header,
		imBmpHeaderFieldsRead )== -1)
	{
		ImReturnBinError( );
	}

	x = header.bmp_biwidth;
	y = header.bmp_biheight;

	sprintf( message, "%d x %d", x, y );
	ImInfo( "Resolution", message );

	/*
	 *  Allocate a VFB of the required size and type.
	 */
	switch( header.bmp_bibitcount )
	{
	case 1:		/* 1-bit index image */
	case 4:		/* 4-bit index image */
	case 8:		/* 8-bit index image */
		if( (vfb = ImVfbAlloc( x, y, IMVFBINDEX8 )) == IMVFBNULL )
		{
			ImErrorFatal( ImQError( ), -1, ImErrNo );
		}
		ImVfbClear( IMVFBINDEX8, 0, vfb );

		sprintf( message, "%d-bit Color Indexed", header.bmp_bibitcount );
		ImInfo( "Type", message );
		break;

	case 24:	/* 24-bit RGB image */
		if( (vfb = ImVfbAlloc( x, y, IMVFBRGB )) == IMVFBNULL )
		{
			ImErrorFatal( ImQError( ), -1, ImErrNo );
		}
		ImVfbClear( IMVFBRGB, 0, vfb );
		sprintf( message, "24-bit RGB" );
		ImInfo( "Type", message );
		break;

	default:	/* unsupported image depth */
		ImErrorFatal( ImQError( ), -1, IMEDEPTH );
	}

	/*
	 * Allocate and read in colortable, if necessary
	 */
	if( header.bmp_bibitcount != 24)
	{
		/*
		 * Find number of colors used and allocate space
		 * for colortable
		 */
		ncolors = header.bmp_biclrused;
		if( ncolors == 0 )
		{
			ncolors = 1 << header.bmp_bibitcount;
		}
		clt = ImCltAlloc( ncolors );

		/*
		 * Output -verbose messages
		 */
		sprintf( message, "%d Entries", ncolors );
		ImInfo( "Color Table", message );

		/*
		 * Read in colortable and set RGB values
		 */
		cltptr = ImCltQFirst( clt );
		ImMalloc( rbuf, unsigned char*, ncolors*IMBMPRGBQUADSIZE );
		rbufptr = rbuf;
		if( ImBinRead( ioType, fd, fp, rbuf, UCHAR, 1,
			ncolors*IMBMPRGBQUADSIZE ) == -1 )
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
			POINTER, &clt));
	}

	/*
	 * Skip to beginning of bitmap data
	 */
	skipsize = ( (header.bmp_offsetbits - IMBMPHEADERSIZE) -
			(ncolors * IMBMPRGBQUADSIZE ) );
	if( skipsize > 0 )
	{
		if( ioType & IMFILEIOFD )
		{
			ImSeek( ioType, fd, fp, header.bmp_offsetbits, L_SET );
		}
		else
		{
			ImMalloc( sbuf, unsigned char*, skipsize );
			if( ImBinRead( ioType, fd, fp, sbuf, UCHAR, 1,
				skipsize ) == -1 )
			{
				free( (char*) sbuf );
				ImReturnBinError( );
			}
			free( (unsigned char*) sbuf );
		}
	}

	/*
	 * Use image compression type info to determine whether to do
	 * verbatim copy into the vfb or RLE expansion into the vfb
	 */ 
	if( header.bmp_bicompress == IMBMPRGB )	/* no compression */
	{
		/*
		 * The image is stored from the bottom up; that is,
		 * the first scanline in the file is the last
		 * scanline in the image.
		 */

		/*
		 * Go to the last scanline in the vfb
		 */
		vfbptr = ImVfbQPtr( vfb, 0, y-1 );

		/*
		 * Read in non-compressed image
		 */
		switch( header.bmp_bibitcount )
		{
		case 1:
			ImInfo( "Compression Type", "none" );

			/*
			 * Read 1-bit non-compressed image
			 *
			 * 1 bytes contains info for 8 pixels
			 *
			 * --------  <- 1 byte
			 * 12345678
			 *
			 * Set = first color in colortable
			 * Not Set = second color in colortable
			 *
			 */

			/*
			 * Find scanline size to nearest 32-bit
			 * boundary and allocate read buffer
			 *
			 * Note: bit shifting is used to speed up
			 *	 calculations
			 */
			scanlinesize = x>>5;
			if( x%32 ) scanlinesize++;
			scanlinesize <<= 2;

			ImMalloc( rbuf, unsigned char*, scanlinesize );

			for( i = 0; i < y ; ++i )
			{
			    if( ImBinRead( ioType, fd, fp,
				 rbuf, UCHAR, 1, scanlinesize ) == -1)
			    {
				ImReturnBinError( );
			    }
			    rbufptr = rbuf;
			    
			    for( j = 0; j < (x/8); ++j )
			    {
				 mask = 128;
				 for( k = 0; k < 8; ++k )
				 {
				    if( ( *(rbufptr) & mask ) == mask )
				    {
					ImVfbSIndex8( vfb, vfbptr, 1 ); 
				    }
				    else
				    {
					ImVfbSIndex8( vfb, vfbptr, 0 ); 
				    }
				    vfbptr = ImVfbQRight( vfb, vfbptr );
				    mask = mask >> 1;
				 }
				 ++rbufptr;
			    }
			    if( x%8 )
			    {
				 mask = 128;
				 for( k = 0; k < x%8; ++k )
				 {
				    if( ( *(rbufptr) & mask ) == mask )
				    {
					ImVfbSIndex8( vfb, vfbptr, 1 ); 
				    }
				    else
				    {
					ImVfbSIndex8( vfb, vfbptr, 0 ); 
				    }
				    vfbptr = ImVfbQRight( vfb, vfbptr );
				    mask = mask >> 1;
				 }
			    }

			    /*
			     * Go up two scanline since we
			     * wrapped-around with ImVfbQRight( )
			     */
			    vfbptr = ImVfbQUp( vfb, vfbptr );
			    vfbptr = ImVfbQUp( vfb, vfbptr );
			}

			break;
		case 4:
			ImInfo( "Compression Type", "none" );

			/*
			 * Read 4-bit non-compressed image
			 *
			 * 1 byte contains info for 2 pixels
			 *
			 * ---- ----  <-- 1 byte
			 * 1111 2222
			 *
			 */

			/*
			 * Find scanline size to nearest 32-bit
			 * boundary and allocate read buffer
			 *
			 * Note: bit shifting is used to speed up
			 *	 calculations
			 */
			scanlinesize = x>>3;
			if( x%8 ) scanlinesize++;
			scanlinesize <<= 2;

			ImMalloc( rbuf, unsigned char*, scanlinesize );

			/*
			 * Read in image and store in vfb
			 */
			for( i = 0; i < y; ++i )
			{
			    if( ImBinRead( ioType, fd, fp,
				 rbuf, UCHAR, 1, scanlinesize ) == -1)
			    {
				ImReturnBinError( );
			    }
			    rbufptr = rbuf;
			    
			    for( j = 0; j < (x/2); ++j )
			    {
				mask = 0xF0;
				ImVfbSIndex8( vfb, vfbptr, 
					((*(rbufptr) & mask) >> 4) ); 
				vfbptr = ImVfbQRight( vfb, vfbptr );

				mask = 0x0F;
				ImVfbSIndex8( vfb, vfbptr, 
					*(rbufptr) & mask ); 
				vfbptr = ImVfbQRight( vfb, vfbptr );
			
				++rbufptr;
			    }
			    if( x%2 )
			    {
				mask = 0xF0;
				ImVfbSIndex8( vfb, vfbptr, 
					((*(rbufptr) & mask) >> 4) ); 
				vfbptr = ImVfbQRight( vfb, vfbptr );
			    }

			    /*
			     * Go up two scanline since we
			     * wrapped-around with ImVfbQRight( )
			     */
			    vfbptr = ImVfbQUp( vfb, vfbptr );
			    vfbptr = ImVfbQUp( vfb, vfbptr );
			}

			break;
		case 8:
			ImInfo( "Compression Type", "none" );

			/*
			 * Read 8-bit non-compressed image
			 *
			 * 1 byte contains info for 1 pixel
			 *
			 * --------  <-- 1 byte
			 * 11111111
			 *
			 */

			/*
			 * Find scanline size to nearest 32-bit
			 * boundary and allocate read buffer
			 *
			 * Note: bit shifting is used to speed up
			 *	 calculations
			 */
			scanlinesize = x>>2;
			if( x%4 ) scanlinesize++;
			scanlinesize <<= 2;

			ImMalloc( rbuf, unsigned char*, scanlinesize );

			/*
			 * Read in image and store in vfb
			 */
			for( i = 0; i < y; ++i )
			{
			    if( ImBinRead( ioType, fd, fp,
				 rbuf, UCHAR, 1, scanlinesize ) == -1)
			    {
				ImReturnBinError( );
			    }
			    rbufptr = rbuf;
			    
			    for( j = 0; j < x; ++j )
			    {
				ImVfbSIndex8( vfb, vfbptr, 
					*(rbufptr) ); 
				vfbptr = ImVfbQRight( vfb, vfbptr );

				++rbufptr;
			    }

			    /*
			     * Go up two scanline since we
			     * wrapped-around with ImVfbQRight( )
			     */
			    vfbptr = ImVfbQUp( vfb, vfbptr );
			    vfbptr = ImVfbQUp( vfb, vfbptr );
			}

			break;
		case 24:
			ImInfo( "Compression Type", "none" );

			/*
			 * Read 24-bit non-compressed image
			 *
			 * 3 bytes contain info for 1 pixel
			 *
			 * -------- -------- --------  <-- 3 bytes
			 * BBBBBBBB GGGGGGGG RRRRRRRR
			 *
			 */

			/*
			 * Find scanline size to nearest 32-bit
			 * boundary and allocate read buffer
			 *
			 * Note: bit shifting is used to speed up
			 *	 calculations
			 */
			scanlinesize = (x*3)>>2;
			if( x%4 ) scanlinesize++;
			scanlinesize <<= 2;

			ImMalloc( rbuf, unsigned char*, scanlinesize );

			/*
			 * Read in image and store in vfb
			 */
			for( i = 0; i < y; ++i )
			{
			    if( ImBinRead( ioType, fd, fp,
				 rbuf, UCHAR, 1, scanlinesize ) == -1)
			    {
				ImReturnBinError( );
			    }
			    rbufptr = rbuf;
			    
			    for( j = 0; j < x; ++j )
			    {
				ImVfbSBlue( vfb, vfbptr, *(rbufptr) ); 
				++rbufptr;

				ImVfbSGreen( vfb, vfbptr, *(rbufptr) ); 
				++rbufptr;

				ImVfbSRed( vfb, vfbptr, *(rbufptr) ); 
				++rbufptr;

				vfbptr = ImVfbQRight( vfb, vfbptr );

			    }

			    /*
			     * Go up two scanline since we
			     * wrapped-around with ImVfbQRight( )
			     */
			    vfbptr = ImVfbQUp( vfb, vfbptr );
			    vfbptr = ImVfbQUp( vfb, vfbptr );
			}

			break;
		} /* end switch */

		free( (unsigned char*) rbuf );
	}
	else 		/* RLE decompression for 4-bit or 8-bit images */
	{
		switch( header.bmp_bicompress )
		{
		case IMBMPRLE4:
			if( imBmpReadRLE4( ioType, fd, fp, vfb, y ) == -1 )
			{
				return( -1 );
			}

			ImInfo( "Compression Type",
				"Run Length Encoded (RLE4)" );
			break;
		case IMBMPRLE8:
			if( imBmpReadRLE8( ioType, fd, fp, vfb, y ) == -1 )
			{
				return( -1 );
			}
			ImInfo( "Compression Type", "Run Length Encoded (RLE8)" );
			break;
		default:
			/* unknown decompression type */
			ImErrorFatal( ImQError( ), -1, IMEIMAGETYPE );
		} /* end switch */
	}

	/*
	 *  Add the VFB to the tagTable.
	 */
	TagTableAppend( tagTable,
		TagEntryAlloc( "image vfb", POINTER, &vfb ) );

	return ( 2 );
}




/*
 *  FUNCTION
 *	imBmpWriteRaw1	-  write an 1-bit non-compressed BMP file
 *
 *  DESCRIPTION
 *	That VFB is queried, and the BMP file header set up and written out.
 *	The VFB data is then converted and written out.
 */

static int				/* Returns # of tags used       */
#ifdef __STDC__
imBmpWriteRaw1( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,
	TagTable *flagsTable, TagTable *tagTable )
#else
imBmpWriteRaw1( pMap, ioType, fd, fp, flagsTable, tagTable )
        ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to */
        int          ioType;		/* I/O flags                    */
        int          fd;		/* Input file descriptor        */
        FILE        *fp;		/* Input file pointer           */
        TagTable    *flagsTable;	/* Flags                        */
        TagTable    *tagTable;		/* Tag list to read from        */
#endif
{
        imBmpHeaderInfoWrite header;	/* BMP file header              */
        ImVfb         *vfb;		/* Read in image                */
        ImVfbPtr       vfbptr;		/* Vfb pixel pointer            */
	imBmpRGBQuad   cltentry;	/* Colortable RGBQuad entry	*/
	ImClt	      *clt;		/* Colortable			*/
	ImCltPtr       cltptr;		/* Colortable entry pointer	*/
	unsigned char  pvalue;		/* Pixel value			*/
	unsigned char *wbuf;		/* Pointer to write buffer	*/
	unsigned char *wbufptr;		/* Pointer to write buffer data	*/
	unsigned int   scanlinesize;	/* Length of scanline		*/
	unsigned int   ncolors = 0;	/* Number of colors used	*/
	unsigned char  bitfield;	/* Bitfield of 8 pixels		*/
        unsigned int   x, y;		/* Width, Height                */
        int	       i, j, k;		/* Counters                     */
	char	       message[100];	/* ImInfo message		*/
 
 
        /*
         * Set byte ordering
         */
        BinByteOrder( BINLBF );

	/*
	 * Get vfb and clt
	 */
        TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );
	clt = ImVfbQClt( vfb );

        x = ImVfbQWidth( vfb );
        y = ImVfbQHeight( vfb ); 

	/*
	 * Find scanline size to nearest 32-bit
	 * boundary and allocate write buffer
	 *
	 * Note: bit shifting is used to speed up
	 *	 calculations
	 */
	scanlinesize = x>>5;
	if( x%32 ) scanlinesize++;
	scanlinesize <<= 2;

	if( clt != IMCLTNULL )
	{
		ncolors = ImCltQNColors( clt );
		if( ncolors > 2 )
		{
			ncolors = 2;
		}
	}

	/*
	 * Setup header and write it out
	 */
        header.bmp_type		= IMBMPMAGIC_LBF;
        header.bmp_size		= (unsigned int) ( IMBMPHEADERSIZE +
				  ( 2 * IMBMPRGBQUADSIZE ) +
				  ( scanlinesize * y ) );
				/*
				 * file size = complete header size +
				 *	colortable size + image data size
				 */
        header.bmp_reserved1	= 0;
        header.bmp_reserved2	= 0;
        header.bmp_offsetbits	= (unsigned int) IMBMPHEADERSIZE +
				  ( 2 * IMBMPRGBQUADSIZE );
				/*
				 * offset to image data =
				 *	BMP header +
				 *	colortable size
				 */
        header.bmp_bisize	= IMBMPINFOHEADERSIZE;
        header.bmp_biwidth	= x;
        header.bmp_biheight	= y;
        header.bmp_biplanes	= 1;
        header.bmp_bibitcount	= 1;
        header.bmp_bicompress	= IMBMPRGB;
        header.bmp_bisizeimage	= scanlinesize * y;
        header.bmp_bixpm	= 0;
        header.bmp_biypm	= 0;
        header.bmp_biclrused	= 0;
        header.bmp_biclrim	= 0;
 
        if ( ImBinWriteStruct( ioType, fd, fp, &header,
		imBmpHeaderFieldsWrite )==-1)
	{
		ImReturnBinError( );
	}

	/*
	 * Output -verbose messages
	 */
	ImInfo( "Byte Order", "Least Significant Byte First" );

	sprintf( message, "%d x %d", x, y );
	ImInfo( "Resolution", message );

	ImInfo( "Type", "1-bit Color Indexed" );
	ImInfo( "Color Table", "2 Entries" );
	ImInfo( "Compression Type", "none" ); 

	/*
	 * Output Color Table
	 */
	cltentry.reserved = 0;

	cltentry.blue = 0;
	cltentry.green = 0;
	cltentry.red = 0;

	if( clt != IMCLTNULL )
	{
		clt = ImVfbQClt( vfb );

		cltptr = ImCltQFirst( clt );
		cltentry.blue = (unsigned char) ImCltQBlue( cltptr );
		cltentry.green = (unsigned char) ImCltQGreen( cltptr );
		cltentry.red = (unsigned char) ImCltQRed( cltptr );
	}
        if ( ImBinWriteStruct(ioType,fd,fp,&cltentry,imBmpRGBQuadFields )==-1)
	{
		ImReturnBinError( );
	}

	cltentry.blue = 255;
	cltentry.green = 255;
	cltentry.red = 255;

	if( clt != IMCLTNULL )
	{
		cltptr = ImCltQNext( clt, cltptr );
		cltentry.blue = (unsigned char) ImCltQBlue( cltptr );
		cltentry.green = (unsigned char) ImCltQGreen( cltptr );
		cltentry.red = (unsigned char) ImCltQRed( cltptr );
	}

        if ( ImBinWriteStruct(ioType,fd,fp,&cltentry,imBmpRGBQuadFields )==-1)
	{
		ImReturnBinError( );
	}

	/* 
	 * Allocate write buffer space
	 */
	ImCalloc( wbuf, unsigned char*, scanlinesize, sizeof( unsigned char) );

	/* 
	 * Output image bitmap
	 */

	/*
	 * Go to last vfb scanline
	 */
	vfbptr = ImVfbQPtr( vfb, 0, y-1 );

	for( i = 0; i < y; ++i )
	{
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

	free( (unsigned char*) wbuf );

        return ( 1 );
}
 




/*
 *  FUNCTION
 *	imBmpWriteRaw4	-  write a 4-bit non-compressed BMP file
 *
 *  DESCRIPTION
 *	That VFB is queried, and the BMP file header set up and written out.
 *	The VFB data is then converted and written out.
 */

static int				/* Returns # of tags used       */
#ifdef __STDC__
imBmpWriteRaw4( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,
	TagTable *flagsTable, TagTable *tagTable )
#else
imBmpWriteRaw4( pMap, ioType, fd, fp, flagsTable, tagTable )
        ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to */
        int          ioType;		/* I/O flags                    */
        int          fd;		/* Input file descriptor        */
        FILE        *fp;		/* Input file pointer           */
        TagTable    *flagsTable;	/* Flags                        */
        TagTable    *tagTable;		/* Tag list to read from        */
#endif
{
        imBmpHeaderInfoWrite header;	/* BMP file header              */
        ImVfb         *vfb;		/* Read in image                */
        ImVfbPtr       vfbptr;		/* Vfb pixel pointer            */
	imBmpRGBQuad   cltentry;	/* Colortable RGBQuad entry	*/
	ImClt	      *clt;		/* Colortable			*/
	ImCltPtr       cltptr;		/* Colortable entry pointer	*/
	unsigned char  pvalue1;		/* Pixel value			*/
	unsigned char  pvalue2;		/* Pixel value			*/
	unsigned char *wbuf;		/* Pointer to write buffer	*/
	unsigned char *wbufptr;		/* Pointer to write buffer data	*/
	unsigned int   scanlinesize;	/* Length of scanline		*/
	unsigned int   ncolors = 0;	/* Number of colors used	*/
        unsigned int   x, y;		/* Width, Height                */
        int	       i, j;		/* Counters                     */
	char	       message[100];	/* ImInfo message		*/
 
 
        /*
         * Set byte ordering
         */
        BinByteOrder( BINLBF );

	/*
	 * Get vfb and clt
	 */
        TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );
	ImVfbToIndex( vfb, 16, vfb );
	clt = ImVfbQClt( vfb );

        x = ImVfbQWidth( vfb );
        y = ImVfbQHeight( vfb ); 

	/*
	 * Find scanline size to nearest 32-bit
	 * boundary and allocate write buffer
	 *
	 * Note: bit shifting is used to speed up
	 *	 calculations
	 */
	scanlinesize = x>>3;
	if( x%8 ) scanlinesize++;
	scanlinesize <<= 2;

	ncolors = ImCltQNColors( clt );
	if( ncolors > 16 )
	{
		ncolors = 16;
	}

	/*
	 * Setup header and write it out
	 */
        header.bmp_type		= IMBMPMAGIC_LBF;
        header.bmp_size		= (unsigned int) ( IMBMPHEADERSIZE +
				  ( ncolors * IMBMPRGBQUADSIZE ) +
				  ( scanlinesize * y ) );
				/*
				 * file size = complete header size +
				 *	colortable size + image data size
				 */
        header.bmp_reserved1	= 0;
        header.bmp_reserved2	= 0;
        header.bmp_offsetbits	= (unsigned int) IMBMPHEADERSIZE +
				  ( ncolors * IMBMPRGBQUADSIZE );
				/*
				 * offset to image data =
				 *	BMP header +
				 *	colortable size
				 */
        header.bmp_bisize	= IMBMPINFOHEADERSIZE;
        header.bmp_biwidth	= x;
        header.bmp_biheight	= y;
        header.bmp_biplanes	= 1;
        header.bmp_bibitcount	= 4;
        header.bmp_bicompress	= IMBMPRGB;
        header.bmp_bisizeimage	= scanlinesize * y;
        header.bmp_bixpm	= 0;
        header.bmp_biypm	= 0;
        header.bmp_biclrused	= (unsigned int) ncolors;
	if( ncolors == 16 ) 
	{
        	header.bmp_biclrused	= 0;
	}
        header.bmp_biclrim	= (unsigned int) ncolors;
 
        if ( ImBinWriteStruct( ioType, fd, fp, &header,
		imBmpHeaderFieldsWrite )==-1)
	{
		ImReturnBinError( );
	}
 
	/*
	 * Output -verbose messages
	 */
	ImInfo( "Byte Order", "Least Significant Byte First" );

	sprintf( message, "%d x %d", x, y );
	ImInfo( "Resolution", message );

	ImInfo( "Type", "4-bit Color Indexed" );

	sprintf( message, "%d Entries", ncolors );
	ImInfo( "Color Table", message );

	ImInfo( "Compression Type", "none" ); 

	/*
	 * Write colortable
	 */
	cltptr = ImCltQFirst( clt );

	ImMalloc( wbuf, unsigned char*, ncolors*IMBMPRGBQUADSIZE );
	wbufptr = wbuf;
	for( i = 0; i < ncolors; ++i )
	{
		*wbufptr = (unsigned char) ImCltQBlue( cltptr );
		++wbufptr;
		*wbufptr = (unsigned char) ImCltQGreen( cltptr );
		++wbufptr;
		*wbufptr = (unsigned char) ImCltQRed( cltptr );
		++wbufptr;
		*wbufptr = 0;
		++wbufptr;

		cltptr = ImCltQNext( clt, cltptr );
	}
	if ( ImBinWrite( ioType, fd, fp, wbuf, UCHAR, 1,
		ncolors*IMBMPRGBQUADSIZE ) == -1 )
	{
		ImReturnBinError( );
	}
	free( wbuf );

	/* 
	 * Allocate write buffer space
	 */
	ImCalloc( wbuf, unsigned char*, scanlinesize, sizeof( unsigned char) );

	/* 
	 * Output image bitmap
	 */

	/*
	 * Go to last vfb scanline
	 */
	vfbptr = ImVfbQPtr( vfb, 0, y-1 );

	for( i = 0; i < y; ++i )
	{
		wbufptr = wbuf;
		for( j = 0; j < (x/2); ++j )
		{
			/*
			 * Pack 2 pixels into 1 byte
			 *
			 * ---- ----  <-- 1 byte
			 * 1111 2222
			 *
			 */
			pvalue1 = (unsigned char) ImVfbQIndex8( vfb, vfbptr );

			/* check for out of range pvalue */
			if( pvalue1 > 15 ) pvalue1 = 15;

			vfbptr = ImVfbQRight( vfb, vfbptr );

			pvalue2 = (unsigned char) ImVfbQIndex8( vfb, vfbptr );

			/* check for out of range pvalue */
			if( pvalue2 > 15 ) pvalue2 = 15;

			vfbptr = ImVfbQRight( vfb, vfbptr );

			*( wbufptr ) = ( ( pvalue1 << 4 ) | pvalue2 );
			++wbufptr;
		}

		if( x%2 )
		{
			pvalue1 = (unsigned char) ImVfbQIndex8( vfb, vfbptr );

			/* check for out of range pvalue */
			if( pvalue1 > 15 ) pvalue1 = 15;

			vfbptr = ImVfbQRight( vfb, vfbptr );

			*( wbufptr ) = ( pvalue1 << 4 );
		}

		if( ImBinWrite( ioType,fd,fp,wbuf,UCHAR,1,scanlinesize )== -1 )
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

	free( (unsigned char*) wbuf );

        return ( 1 );
}





/*
 *  FUNCTION
 *	imBmpWriteRaw8	-  write an 8-bit non-compressed BMP file
 *
 *  DESCRIPTION
 *	That VFB is queried, and the BMP file header set up and written out.
 *	The VFB data is then converted and written out.
 */
static int				/* Returns # of tags used       */
#ifdef __STDC__
imBmpWriteRaw8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,
	TagTable *flagsTable, TagTable *tagTable )
#else
imBmpWriteRaw8( pMap, ioType, fd, fp, flagsTable, tagTable )
	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to */
	int          ioType;		/* I/O flags                    */
	int          fd;		/* Input file descriptor        */
	FILE        *fp;		/* Input file pointer           */
	TagTable    *flagsTable;	/* Flags                        */
	TagTable    *tagTable;		/* Tag list to read from        */
#endif
{
	imBmpHeaderInfoWrite header;	/* BMP file header              */
	ImVfb         *vfb;		/* Read in image                */
	ImVfbPtr       vfbptr;		/* Vfb pixel pointer            */
	ImClt	      *clt;		/* Colortable			*/
	ImCltPtr       cltptr;		/* Colortable entry pointer	*/
	unsigned char *wbuf;		/* Pointer to write buffer	*/
	unsigned char *wbufptr;		/* Pointer to write buffer data	*/
	unsigned int   scanlinesize;	/* Length of scanline		*/
	unsigned int   ncolors = 0;	/* Number of colors used	*/
        unsigned int   x, y;		/* Width, Height                */
        int	       i, j;		/* Counters                     */
	char	       message[100];	/* ImInfo message		*/
 
 
        /*
         * Set byte ordering
         */
        BinByteOrder( BINLBF );

	/*
	 * Get vfb and clt
	 */
        TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );
	clt = ImVfbQClt( vfb );

        x = ImVfbQWidth( vfb );
        y = ImVfbQHeight( vfb ); 

	/*
	 * Find scanline size to nearest 32-bit
	 * boundary and allocate write buffer
	 *
	 * Note: bit shifting is used to speed up
	 *	 calculations
	 */
	scanlinesize = x>>2;
	if( x%4 ) scanlinesize++;
	scanlinesize <<= 2;

	ncolors = ImCltQNColors( clt );
	if( ncolors > 256 )
	{
		ncolors = 256;
	}

	/*
	 * Setup header and write it out
	 */
        header.bmp_type		= IMBMPMAGIC_LBF;
        header.bmp_size		= (unsigned int) ( IMBMPHEADERSIZE +
				  ( ncolors * IMBMPRGBQUADSIZE ) +
				  ( scanlinesize * y ) );
				/*
				 * file size = complete header size +
				 *	colortable size + image data size
				 */
        header.bmp_reserved1	= 0;
        header.bmp_reserved2	= 0;
        header.bmp_offsetbits	= (unsigned int) IMBMPHEADERSIZE +
				  ( ncolors * IMBMPRGBQUADSIZE );
				/*
				 * offset to image data =
				 *	BMP header +
				 *	colortable size
				 */
        header.bmp_bisize	= IMBMPINFOHEADERSIZE;
        header.bmp_biwidth	= x;
        header.bmp_biheight	= y;
        header.bmp_biplanes	= 1;
        header.bmp_bibitcount	= 8;
        header.bmp_bicompress	= IMBMPRGB;
        header.bmp_bisizeimage	= scanlinesize * y;
        header.bmp_bixpm	= 0;
        header.bmp_biypm	= 0;
        header.bmp_biclrused	= (unsigned int) ncolors;
	if( ncolors == 256 ) 
	{
        	header.bmp_biclrused	= 0;
	}
        header.bmp_biclrim	= (unsigned int) ncolors;
 
        if( ImBinWriteStruct( ioType, fd, fp, &header,
		imBmpHeaderFieldsWrite ) == -1)
	{
		ImReturnBinError( );
	}
 
	/*
	 * Output -verbose messages
	 */
	ImInfo( "Byte Order", "Least Significant Byte First" );

	sprintf( message, "%d x %d", x, y );
	ImInfo( "Resolution", message );

	ImInfo( "Type", "8-bit Color Indexed" );

	sprintf( message, "%d Entries", ncolors );
	ImInfo( "Color Table", message );

	ImInfo( "Compression Type", "none" ); 

	/*
	 * Write colortable
	 */
	cltptr = ImCltQFirst( clt );

	ImMalloc( wbuf, unsigned char*, ncolors*IMBMPRGBQUADSIZE );
	wbufptr = wbuf;
	for( i = 0; i < ncolors; ++i )
	{
		*wbufptr = (unsigned char) ImCltQBlue( cltptr );
		++wbufptr;
		*wbufptr = (unsigned char) ImCltQGreen( cltptr );
		++wbufptr;
		*wbufptr = (unsigned char) ImCltQRed( cltptr );
		++wbufptr;
		*wbufptr = 0;
		++wbufptr;

		cltptr = ImCltQNext( clt, cltptr );
	}
	if ( ImBinWrite( ioType, fd, fp, wbuf, UCHAR, 1,
		ncolors*IMBMPRGBQUADSIZE ) == -1 )
	{
		ImReturnBinError( );
	}
	free( wbuf );

	/* 
	 * Allocate write buffer space
	 */
	ImCalloc( wbuf, unsigned char*, scanlinesize, sizeof( unsigned char) );

	/* 
	 * Output image bitmap
	 */

	/*
	 * Go to last vfb scanline
	 */
	vfbptr = ImVfbQPtr( vfb, 0, y-1 );

	for( i = 0; i < y; ++i )
	{
		wbufptr = wbuf;
		for( j = 0; j < x; ++j )
		{
			*( wbufptr ) = (unsigned char) ImVfbQIndex8( vfb, vfbptr );
			vfbptr = ImVfbQRight( vfb, vfbptr );
			++wbufptr;
		}
		if( ImBinWrite( ioType,fd,fp,wbuf,UCHAR,1,scanlinesize )== -1 )
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

	free( (unsigned char*) wbuf );

	return( 1 );
}





/*
 *  FUNCTION
 *	imBmpWriteRaw24	-  write a 24-bit non-compressed BMP file
 *
 *  DESCRIPTION
 *	That VFB is queried, and the BMP file header set up and written out.
 *	The VFB data is then converted and written out.
 */
static int				/* Returns # of tags used       */
#ifdef __STDC__
imBmpWriteRaw24( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,
	TagTable *flagsTable, TagTable *tagTable )
#else
imBmpWriteRaw24( pMap, ioType, fd, fp, flagsTable, tagTable )
        ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to */
        int          ioType;		/* I/O flags                    */
        int          fd;		/* Input file descriptor        */
        FILE        *fp;		/* Input file pointer           */
        TagTable    *flagsTable;	/* Flags                        */
        TagTable    *tagTable;		/* Tag list to read from        */
#endif
{
        imBmpHeaderInfoWrite header;	/* BMP file header              */
        ImVfb         *vfb;		/* Read in image                */
        ImVfbPtr       vfbptr;		/* Vfb pixel pointer            */
	unsigned char *wbuf;		/* Pointer to write buffer	*/
	unsigned char *wbufptr;		/* Pointer to write buffer data	*/
	unsigned int   scanlinesize;	/* Length of scanline		*/
        unsigned int   x, y;		/* Width, height                */
        int	       i, j;		/* Counters                     */
	char	       message[100];	/* ImInfo message		*/
 
 
        /*
         * Set byte ordering
         */
        BinByteOrder( BINLBF );

	/*
	 * Get vfb
	 */
        TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

        x = ImVfbQWidth( vfb );
        y = ImVfbQHeight( vfb ); 

	/*
	 * Find scanline size to nearest 32-bit
	 * boundary and allocate write buffer
	 *
	 * Note: bit shifting is used to speed up
	 *	 calculations
	 */
	scanlinesize = (x*3)>>2;
	if( x%4 ) scanlinesize++;
	scanlinesize <<= 2;

	/*
	 * Setup header and write it out
	 */
        header.bmp_type		= IMBMPMAGIC_LBF;
        header.bmp_size		= (unsigned int) ( IMBMPHEADERSIZE +
				  ( scanlinesize * y ) );
				/*
				 * file size = complete header size +
				 *	image data size
				 */
        header.bmp_reserved1	= 0;
        header.bmp_reserved2	= 0;
        header.bmp_offsetbits	= IMBMPHEADERSIZE;
				/*
				 * offset to image data = BMP header
				 */
        header.bmp_bisize	= IMBMPINFOHEADERSIZE;
        header.bmp_biwidth	= x;
        header.bmp_biheight	= y;
        header.bmp_biplanes	= 1;
        header.bmp_bibitcount	= 24;
        header.bmp_bicompress	= IMBMPRGB;
        header.bmp_bisizeimage	= scanlinesize * y;
        header.bmp_bixpm	= 0;
        header.bmp_biypm	= 0;
        header.bmp_biclrused	= 0;
        header.bmp_biclrim	= 0;
 
        if( ImBinWriteStruct( ioType, fd, fp, &header,
		imBmpHeaderFieldsWrite )==-1)
	{
		ImReturnBinError( );
	}
 
	/*
	 * Output -verbose messages
	 */
	ImInfo( "Byte Order", "Least Significant Byte First" );

	sprintf( message, "%d x %d", x, y );
	ImInfo( "Resolution", message );

	ImInfo( "Type", "24-bit RGB" );

	ImInfo( "Compression Type", "none" ); 

	/* 
	 * Allocate write buffer space
	 */
	ImCalloc( wbuf, unsigned char*, scanlinesize, sizeof( unsigned char) );

	/* 
	 * Output image bitmap
	 */

	/*
	 * Go to last vfb scanline
	 */
	vfbptr = ImVfbQPtr( vfb, 0, y-1 );

	/*
	 * Output pixels
	 */
	for( i = 0; i < y; ++i )
	{
		wbufptr = wbuf;
		for( j = 0; j < x; ++j )
		{
			*( wbufptr ) = (unsigned char) ImVfbQBlue( vfb, vfbptr );
			++wbufptr;

			*( wbufptr ) = (unsigned char) ImVfbQGreen( vfb, vfbptr );
			++wbufptr;

			*( wbufptr ) = (unsigned char) ImVfbQRed( vfb, vfbptr );
			++wbufptr;

			vfbptr = ImVfbQRight( vfb, vfbptr );
		}
		if( ImBinWrite( ioType,fd,fp,wbuf,UCHAR,1,scanlinesize )== -1 )
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

	free( (unsigned char*) wbuf );

	return( 1 );
}


#define imBmpWrite2Bytes( arg1, arg2 )					\
									\
buf[ bufin ] = arg1;						\
++bufin;							\
buf[ bufin ] = arg2;						\
++bufin;							\
if( bufin > (IMBLKSIZE-1) )					\
{								\
	if( ImBinWrite( ioType,fd,fp,buf,UCHAR,1,IMBLKSIZE )==-1)	\
	{							\
		ImReturnBinError( );				\
	}							\
	bufin = 0;						\
}								\






/*
 *  FUNCTION
 *	imBmpWriteRLE4	-  write a 4-bit RLE compressed BMP file
 *
 *  DESCRIPTION
 *	That VFB is queried, and the BMP file header set up and written out.
 *	The VFB data is then converted and written out.  See encoding
 *	scheme in format section.
 */
static int				/* Returns # of tags used       */
#ifdef __STDC__
imBmpWriteRLE4( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,
	TagTable *flagsTable, TagTable *tagTable )
#else
imBmpWriteRLE4( pMap, ioType, fd, fp, flagsTable, tagTable )
	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to */
	int          ioType;		/* I/O flags                    */
	int          fd;		/* Input file descriptor        */
	FILE        *fp;		/* Input file pointer           */
	TagTable    *flagsTable;	/* Flags                        */
	TagTable    *tagTable;		/* Tag list to read from        */
#endif
{
	imBmpHeaderInfoWrite header;	/* BMP file header              */
	ImVfb         *vfb;		/* Read in image                */
	ImVfbPtr       vfbptr;		/* Vfb pixel pointer            */
	ImClt	      *clt;		/* Colortable			*/
	ImCltPtr       cltptr;		/* Colortable entry pointer	*/
	unsigned int   ncolors = 0;	/* Number of colors used	*/
        unsigned int   x, y;		/* Width, Height                */
	unsigned int   run;		/* Number of consecutive bytes	*/
	unsigned char  pixel;		/* Temp pixel value		*/
	unsigned int   bytecount = 0;	/* Bytes in compressed image	*/
	unsigned int   nowrite;		/* Write status			*/
	unsigned char  pixbuf[ 256 ];	/* Pixel buffer			*/
	unsigned char *cbuf;		/* Buffer for colortable write	*/
	unsigned char *cbufptr;		/* Buffer pointer		*/
	unsigned char  buf[ IMBLKSIZE ];/* File write output buffer	*/
	int            bufin;		/* Output buffer index		*/
	int            xcount, ycount;	/* Counters			*/
	int            i;		/* Counters                     */
	int            count;		/* Counter			*/
	char	       message[100];	/* ImInfo message		*/
 
 
        /*
         * Set byte ordering
         */
        BinByteOrder( BINLBF );

	/*
	 * Get vfb and clt
	 */
        TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );
	ImVfbToIndex( vfb, 16, vfb );
	clt = ImVfbQClt( vfb );

        x = ImVfbQWidth( vfb );
        y = ImVfbQHeight( vfb ); 

	ncolors = ImCltQNColors( clt );
	if( ncolors > 16 )
	{
		ncolors = 16;
	}

	/*
	 * Setup header and write it out
	 */
        header.bmp_type		= IMBMPMAGIC_LBF;
        header.bmp_reserved1	= 0;
        header.bmp_reserved2	= 0;
        header.bmp_offsetbits	= (unsigned int) IMBMPHEADERSIZE +
				  ( ncolors * IMBMPRGBQUADSIZE );
				/*
				 * offset to image data =
				 *	BMP header +
				 *	colortable size
				 */
        header.bmp_bisize	= IMBMPINFOHEADERSIZE;
        header.bmp_biwidth	= x;
        header.bmp_biheight	= y;
        header.bmp_biplanes	= 1;
        header.bmp_bibitcount	= 4;
        header.bmp_bicompress	= IMBMPRLE4;
        header.bmp_bixpm	= 0;
        header.bmp_biypm	= 0;
        header.bmp_biclrused	= (unsigned int) ncolors;
	if( ncolors == 16 ) 
	 {
        	header.bmp_biclrused	= 0;
	}
        header.bmp_biclrim	= (unsigned int) ncolors;
 
        if( ImBinWriteStruct( ioType, fd, fp, &header,
		imBmpHeaderFieldsWrite ) == -1 )
	{
		ImReturnBinError( );
	}
 
	/*
	 * Output -verbose messages
	 */
	ImInfo( "Byte Order", "Least Significant Byte First" );

	sprintf( message, "%d x %d", x, y );
	ImInfo( "Resolution", message );

	ImInfo( "Type", "4-bit Color Indexed" );

	sprintf( message, "%d Entries", ncolors );
	ImInfo( "Color Table", message );

	ImInfo( "Compression Type", "Run Length Encoded (RLE4)" ); 

	/*
	 * Write colortable
	 */
	cltptr = ImCltQFirst( clt );

	ImMalloc( cbuf, unsigned char*, ncolors*IMBMPRGBQUADSIZE );
	cbufptr = cbuf;
	for( i = 0; i < ncolors; ++i )
	{
		*cbufptr = (unsigned char) ImCltQBlue( cltptr );
		++cbufptr;
		*cbufptr = (unsigned char) ImCltQGreen( cltptr );
		++cbufptr;
		*cbufptr = (unsigned char) ImCltQRed( cltptr );
		++cbufptr;
		*cbufptr = 0;
		++cbufptr;

		cltptr = ImCltQNext( clt, cltptr );
	}
	if ( ImBinWrite( ioType, fd, fp, cbuf, UCHAR, 1,
		ncolors*IMBMPRGBQUADSIZE ) == -1 )
	{
		ImReturnBinError( );
	}
	free( cbuf );

	/* 
	 * Output image bitmap
	 */

	/*
	 * Go to last vfb scanline
	 */
	vfbptr = ImVfbQPtr( vfb, 0, y-1 );

	/*
	 * RLE encode the image
	 */
	bytecount = 0;
	xcount = 0;
	ycount = 0;
	bufin = 0;
	pixel = ImVfbQIndex8( vfb, vfbptr );

	/* check for out of range pixel */
	if( pixel > 15 ) pixel = 15;

	vfbptr = ImVfbQRight( vfb, vfbptr );
	while( ycount < y )
	{
		/*
		 * Boundary condition: 1 pixel at the end
		 *	of a scanline.
		 */
                if( xcount == (x-1) )
                {
			/*
			 * Encoded pixel as run of 1
			 */
			imBmpWrite2Bytes( 1, pixel << 4 );

                        /*
			 * Write end of scanline code
			 */
			imBmpWrite2Bytes( 0, 0 );
 
			/*
			 * Adjust counters and vfb pointers
			 */
                        bytecount += 4;
                        xcount = 0;
                        ++ycount;
                        vfbptr = ImVfbQUp( vfb, vfbptr );
                        vfbptr = ImVfbQUp( vfb, vfbptr );
                        pixel = ImVfbQIndex8( vfb, vfbptr );

			/* check for out of range pixel */
			if( pixel > 15 ) pixel = 15;

                        vfbptr = ImVfbQRight( vfb, vfbptr );
                }

		/*
		 * Boundary condition: 2 pixels at the end
		 *	of a scanline.
		 */
                if( xcount == (x-2) )
                {
			/*
			 * Write the last two pixels as a run of two
			 */
			imBmpWrite2Bytes( 2, ( pixel << 4 )
				| ImVfbQIndex8(vfb,vfbptr) );

                        /*
			 * Write end of scanline code
			 */
			imBmpWrite2Bytes( 0, 0 );
 
			/*
			 * Adjust counters and vfb pointers
			 */
                        bytecount += 4;
                        xcount = 0;
                        ++ycount;
                        vfbptr = ImVfbQRight( vfb, vfbptr );
                        vfbptr = ImVfbQUp( vfb, vfbptr );
                        vfbptr = ImVfbQUp( vfb, vfbptr );
                        pixel = ImVfbQIndex8( vfb, vfbptr );

			/* check for out of range pixel */
			if( pixel > 15 ) pixel = 15;

                        vfbptr = ImVfbQRight( vfb, vfbptr );
                }

		/*
		 * Encoded Mode - encoding of multiple consecutive
		 *	pixels of the same value into a run and
		 *	pixel value
		 */
		if( pixel == ImVfbQIndex8( vfb, vfbptr ) )
		{
			run = 0;
			nowrite = TRUE;

			/*
			 * While two adjacent pixels are the same,
			 * count them up.  If we reach the
			 * end of a scanline, write out the
			 * current run and pixel.  If we
			 * have a run of 255 pixels, write
			 * out the run and pixel.
			 */
			while( pixel == ImVfbQIndex8( vfb, vfbptr ) )
			{
				++run;
				++xcount;
				pixel = ImVfbQIndex8( vfb, vfbptr );

				/* check for out of range pixel */
				if( pixel > 15 ) pixel = 15;

				vfbptr = ImVfbQRight( vfb, vfbptr );

				/*
				 * End of a scanline
				 */
				if( xcount == x-1 )
				{
					/*
					 * Write run and pixel value
					 */
					imBmpWrite2Bytes( run+1, (pixel<<4)
						| pixel );

					/*
					 * Write end of scanline code
					 */
					imBmpWrite2Bytes( 0, 0 );

					/*
					 * Adjust counters and vfb pointers
					 */
					bytecount += 4;
					xcount = 0;
					++ycount;
					vfbptr = ImVfbQUp( vfb, vfbptr );
					vfbptr = ImVfbQUp( vfb, vfbptr );
					pixel = ImVfbQIndex8( vfb, vfbptr );

					/* check for out of range pixel */
					if( pixel > 15 ) pixel = 15;

					vfbptr = ImVfbQRight( vfb, vfbptr );
					nowrite = FALSE;
					break;
				}

				/*
				 * End of 255 pixels
				 */
				if( run == 254 )
				{
					/*
					 * Write run and pixel value
					 */
					imBmpWrite2Bytes( 255, (pixel<<4)
						| pixel );

					/*
					 * Adjust counters and vfb pointers
					 */
					bytecount += 2;
					run = 0;
					++xcount;
					pixel = ImVfbQIndex8( vfb, vfbptr );

					/* check for out of range pixel */
					if( pixel > 15 ) pixel = 15;

					vfbptr = ImVfbQRight( vfb, vfbptr );
					nowrite = FALSE;
					break;
				}
			}

			/*
			 * If we haven't written out the run and pixel,
			 *	do it now.
			 */
			if( nowrite == TRUE )
			{
				/*
				 * Write run and pixel
				 */
				imBmpWrite2Bytes( run+1, (pixel<<4) | pixel );

				/*
				 * Adjust counters and vfb pointers
				 */
				bytecount += 2;
				++xcount;
				pixel = ImVfbQIndex8( vfb, vfbptr );

				/* check for out of range pixel */
				if( pixel > 15 ) pixel = 15;

				vfbptr = ImVfbQRight( vfb, vfbptr );
			}
		}
 
		/*
		 * Absolute Mode - copying of consecutive different pixels
		 *	literally
		 */
		if( (pixel != ImVfbQIndex8( vfb, vfbptr ))
			&& (xcount < (x-2) )
			&& (ycount < y) )
		{
			run = 0;
			nowrite = TRUE;

			pixbuf[ 0 ] = pixel;
			pixel = ImVfbQIndex8( vfb, vfbptr );

			/* check for out of range pixel */
			if( pixel > 15 ) pixel = 15;

			vfbptr = ImVfbQRight( vfb, vfbptr );
			++xcount;

			/*
			 * Since Absolute Mode requires that there
			 * be at least 3 different adjacent
			 * pixels, we check for this condition
			 * first.  If there are only 1 or 2
			 * different adjacent bytes, then
			 * we write them out as runs of 1
			 * and pixel.
			 */
			if( pixel == ImVfbQIndex8( vfb, vfbptr ) )
			{
				/*
				 * There is only 1 different pixel
				 */ 

				/*
				 * Write out run and pixel
				 */
				imBmpWrite2Bytes( 1, pixbuf[ 0 ] << 4 );

				bytecount += 2;
				nowrite = FALSE;
			}
			else
			{
				pixbuf[ 1 ] = pixel;
				pixel = ImVfbQIndex8( vfb, vfbptr );

				/* check for out of range pixel */
				if( pixel > 15 ) pixel = 15;

				vfbptr = ImVfbQRight( vfb, vfbptr );
				++xcount;
				if( pixel == ImVfbQIndex8( vfb, vfbptr ) )
				{
					/*
					 * There are only 2 different
					 * adjacent pixels
					 */

					/*
					 * Write run and pixel
					 */
					imBmpWrite2Bytes( 2, (pixbuf[ 0 ]<<4)
						| pixbuf[ 1 ]);

					bytecount += 2;
					nowrite = FALSE;
				}
				else
				{
					/*
					 * There are at least 3 different
					 * adjacent pixels, so we reset
					 * the vfb pointers
					 */
					xcount -= 2;
					vfbptr = ImVfbQLeft( vfb, vfbptr );
					vfbptr = ImVfbQLeft( vfb, vfbptr );
					vfbptr = ImVfbQLeft( vfb, vfbptr );
					pixel = ImVfbQIndex8( vfb, vfbptr );

					/* check for out of range pixel */
					if( pixel > 15 ) pixel = 15;

					vfbptr = ImVfbQRight( vfb, vfbptr );
				}
			}
			
			/*
			 * While adjacent pixels are different, count them
			 * up and store them in pixbuf.
			 */
			while( pixel != ImVfbQIndex8( vfb, vfbptr ) )
			{
				pixbuf[ run ] = pixel;
				pixel = ImVfbQIndex8( vfb, vfbptr );

				/* check for out of range pixel */
				if( pixel > 15 ) pixel = 15;

				vfbptr = ImVfbQRight( vfb, vfbptr );
				++run;
				++xcount;

				/*
				 * End of scanline
				 */
				if( xcount == x-1 )
				{
					pixbuf[ run ] = pixel;
					pixbuf[ run+1 ] = 0;

					/*
					 * Write escape(0) and run
					 */
					imBmpWrite2Bytes( 0, run+1 );

					/*
					 * Write pixels in pixbuf
					 */
					count = 0;
					for( i = 0; i < run+1; i += 2)
					{
						buf[ bufin ] = (pixbuf[ i ]<<4)
							| pixbuf[ i+1 ];
						++bufin;
						if( bufin > (IMBLKSIZE-1) )
						{
							if( ImBinWrite( ioType,
								fd,fp,buf,
								UCHAR,1,
								IMBLKSIZE )==-1)
							{
							    ImReturnBinError( );
							}
							bufin = 0;
						}
						++count;
					}
					bytecount += count;

					/*
					 * Pad to word boundary if necessary
					 */
					if( (count%2) != 0 )
					{
						buf[ bufin ] = 0;
						++bufin;
						if( bufin > (IMBLKSIZE-1) )
						{
							if( ImBinWrite( ioType,
								fd,fp,buf,
								UCHAR,1,
								IMBLKSIZE )==-1)
							{
							    ImReturnBinError( );
							}
							bufin = 0;
						}
						++bytecount;
					}

					/*
					 * Write end of scanline code
					 */
					imBmpWrite2Bytes( 0, 0 );

					/*
					 * Adjust counters and vfb pointers
					 */
					bytecount += 4;
					xcount = 0;
					++ycount;
					vfbptr = ImVfbQUp( vfb, vfbptr );
					vfbptr = ImVfbQUp( vfb, vfbptr );
					pixel = ImVfbQIndex8( vfb, vfbptr );

					/* check for out of range pixel */
					if( pixel > 15 ) pixel = 15;

					vfbptr = ImVfbQRight( vfb, vfbptr );
					nowrite = FALSE;
					break;
				}

				/*
				 * End of 255 consecutive different pixels
				 */
				if( run == 254 )
				{
					pixbuf[ run ] = pixel;
					pixbuf[ run+1 ] = 0;

					/*
					 * Write escape code(0) and run
					 */
					imBmpWrite2Bytes( 0, 255 );

					/*
					 * Write pixel literals
					 */
					for( i = 0; i < 255; i += 2 )
					{
						buf[ bufin ] = (pixbuf[ i ]<<4)
							| pixbuf[ i+1 ];
						++bufin;
						if( bufin > (IMBLKSIZE-1) )
						{
							if( ImBinWrite( ioType,
								fd,fp,buf,
								UCHAR,1,
								IMBLKSIZE )==-1)
							{
							    ImReturnBinError( );
							}
							bufin = 0;
						}
					}

					/*
					 * Adjust counters and vfb pointers
					 */
					bytecount += 130;
					run = 0;
					++xcount;
					pixel = ImVfbQIndex8( vfb, vfbptr );

					/* check for out of range pixel */
					if( pixel > 15 ) pixel = 15;

					vfbptr = ImVfbQRight( vfb, vfbptr );
					nowrite = FALSE;
					break;
				}
			}

			/*
			 * If we haven't written out the pixels, then
			 * write it now
			 */
			if( nowrite == TRUE )
			{
				pixbuf[ run ] = 0;

				/*
				 * Write escape code(0) and run
				 */
				imBmpWrite2Bytes( 0, run );

				/*
				 * Write pixel literals
				 */
				count = 0;
				for( i = 0; i < run; i += 2 )
				{
					buf[ bufin ] = (pixbuf[ i ]<<4)
						| pixbuf[ i+1 ];
					++bufin;
					if( bufin > (IMBLKSIZE-1) )
					{
						if( ImBinWrite( ioType,fd,fp
							,buf,UCHAR,1
							,IMBLKSIZE )==-1)
						{
							ImReturnBinError( );
						}
						bufin = 0;
					}
					++count;
				}
				bytecount += count;

				/*
				 * Pad to word boundary if necessary
				 */
				if( count%2 != 0 )
				{
					buf[ bufin ] = 0;
					++bufin;
					if( bufin > (IMBLKSIZE-1) )
					{
						if( ImBinWrite( ioType,
							fd,fp,buf,
							UCHAR,1,
							IMBLKSIZE )==-1)
						{
						    ImReturnBinError( );
						}
						bufin = 0;
					}
					++bytecount;
				}

				/*
				 * Adjust bytecount
				 */
				bytecount += 2;
			}
		}
	}

	/*
	 * Write end of bitmap code
	 */
	buf[ bufin ] = 0;
	++bufin;
	buf[ bufin ] = 1;
	++bufin;
	if( ImBinWrite( ioType,fd,fp,buf,UCHAR,1,bufin )==-1)
	{
		ImReturnBinError( );
	}

	/*
	 * Adjust bytecount
	 */
	bytecount += 2;

	/*
	 * Seek to header and write file size
	 */
	if( ImSeek( ioType, fd, fp, 2, L_SET ) == -1 )
	{
		ImReturnBinError( );
	}
	header.bmp_size = IMBMPHEADERSIZE + ( ncolors * IMBMPRGBQUADSIZE )
		+ bytecount;
	if( ImBinWrite( ioType, fd, fp, &header.bmp_size, UINT32, 4, 1) == -1 )
	{
		ImReturnBinError( );
	}

	/*
	 * Seek to header and write bitmap size
	 */
	if( ImSeek( ioType, fd, fp, 34, L_SET ) == -1 )
	{
		ImReturnBinError( );
	}
	header.bmp_bisizeimage =  bytecount;
	if( ImBinWrite( ioType, fd, fp, &header.bmp_bisizeimage,
		UINT32, 4, 1) == -1 )
	{
		ImReturnBinError( );
	}

	return( 1 );
}





/*
 *  FUNCTION
 *	imBmpWriteRLE8	-  write an 8-bit RLE compressed BMP file
 *
 *  DESCRIPTION
 *	That VFB is queried, and the BMP file header set up and written out.
 *	The VFB data is then converted and written out.  See encoding
 *	scheme in FORMAT section.
 */
static int				/* Returns # of tags used       */
#ifdef __STDC__
imBmpWriteRLE8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,
	TagTable *flagsTable, TagTable *tagTable )
#else
imBmpWriteRLE8( pMap, ioType, fd, fp, flagsTable, tagTable )
	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to */
	int          ioType;		/* I/O flags                    */
	int          fd;		/* Input file descriptor        */
	FILE        *fp;		/* Input file pointer           */
	TagTable    *flagsTable;	/* Flags                        */
	TagTable    *tagTable;		/* Tag list to read from        */
#endif
{
	imBmpHeaderInfoWrite header;	/* BMP file header              */
	ImVfb         *vfb;		/* Read in image                */
	ImVfbPtr       vfbptr;		/* Vfb pixel pointer            */
	ImClt	      *clt;		/* Colortable			*/
	ImCltPtr       cltptr;		/* Colortable entry pointer	*/
	unsigned int   ncolors = 0;	/* Number of colors used	*/
	unsigned int   x, y;		/* Width, Height                */
	unsigned int   run;		/* Number of consecutive bytes	*/
	unsigned char  pixel;		/* Temp pixel value		*/
	unsigned int   bytecount = 0;	/* Bytes in compressed image	*/
	unsigned int   nowrite;		/* Write status			*/
	unsigned char  pixbuf[ 255 ];	/* Pixel buffer			*/
	unsigned char *cbuf;		/* Buffer for colortable write	*/
	unsigned char *cbufptr;		/* Buffer pointer		*/
	unsigned char  buf[ IMBLKSIZE ];/* File write output buffer	*/
	int            bufin;		/* Output buffer index		*/
	int            xcount, ycount;	/* Counters			*/
	int            i;		/* Counters                     */
	char	       message[100];	/* ImInfo message		*/
 
 
        /*
         * Set byte ordering
         */
        BinByteOrder( BINLBF );

	/*
	 * Get vfb and clt
	 */
        TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );
	clt = ImVfbQClt( vfb );

        x = ImVfbQWidth( vfb );
        y = ImVfbQHeight( vfb ); 

	ncolors = ImCltQNColors( clt );
	if( ncolors > 256 )
	{
		ncolors = 256;
	}

	/*
	 * Setup header and write it out
	 */
        header.bmp_type		= IMBMPMAGIC_LBF;
        header.bmp_reserved1	= 0;
        header.bmp_reserved2	= 0;
        header.bmp_offsetbits	= (unsigned int) IMBMPHEADERSIZE +
				  ( ncolors * IMBMPRGBQUADSIZE );
				/*
				 * offset to image data =
				 *	BMP header +
				 *	colortable size
				 */
        header.bmp_bisize	= IMBMPINFOHEADERSIZE;
        header.bmp_biwidth	= x;
        header.bmp_biheight	= y;
        header.bmp_biplanes	= 1;
        header.bmp_bibitcount	= 8;
        header.bmp_bicompress	= IMBMPRLE8;
        header.bmp_bixpm	= 0;
        header.bmp_biypm	= 0;
        header.bmp_biclrused	= (unsigned int) ncolors;
	if( ncolors == 256 ) 
	 {
        	header.bmp_biclrused	= 0;
	}
        header.bmp_biclrim	= (unsigned int) ncolors;;
 
        if( ImBinWriteStruct( ioType, fd, fp, &header,
		imBmpHeaderFieldsWrite ) == -1 )
	{
		ImReturnBinError( );
	}
 
	/*
	 * Output -verbose messages
	 */
	ImInfo( "Byte Order", "Least Significant Byte First" );

	sprintf( message, "%d x %d", x, y );
	ImInfo( "Resolution", message );

	ImInfo( "Type", "8-bit Color Indexed" );

	sprintf( message, "%d Entries", ncolors );
	ImInfo( "Color Table", message );

	ImInfo( "Compression Type", "Run Length Encoded (RLE8)" ); 

	/*
	 * Write colortable
	 */
	cltptr = ImCltQFirst( clt );

	ImMalloc( cbuf, unsigned char*, ncolors*IMBMPRGBQUADSIZE );
	cbufptr = cbuf;
	for( i = 0; i < ncolors; ++i )
	{
		*cbufptr = (unsigned char) ImCltQBlue( cltptr );
		++cbufptr;
		*cbufptr = (unsigned char) ImCltQGreen( cltptr );
		++cbufptr;
		*cbufptr = (unsigned char) ImCltQRed( cltptr );
		++cbufptr;
		*cbufptr = 0;
		++cbufptr;

		cltptr = ImCltQNext( clt, cltptr );
	}
	if ( ImBinWrite( ioType, fd, fp, cbuf, UCHAR, 1,
		ncolors*IMBMPRGBQUADSIZE ) == -1 )
	{
		ImReturnBinError( );
	}
	free( cbuf );

	/* 
	 * Output image bitmap
	 */

	/*
	 * Go to last vfb scanline
	 */
	vfbptr = ImVfbQPtr( vfb, 0, y-1 );

	/*
	 * RLE encode the image
	 */
	bytecount = 0;
	xcount = 0;
	ycount = 0;
	bufin = 0;
	pixel = ImVfbQIndex8( vfb, vfbptr );
	vfbptr = ImVfbQRight( vfb, vfbptr );
	while( ycount < y )
	{
		/*
		 * Boundary condition: 1 pixel at the end
		 *	of a scanline.
		 */
                if( xcount == (x-1) )
                {
			/*
			 * Encoded pixel as run of 1
			 */
			imBmpWrite2Bytes( 1, pixel );

                        /*
			 * Write end of scanline code
			 */
			imBmpWrite2Bytes( 0, 0 );
 
			/*
			 * Adjust counters and vfb pointers
			 */
                        bytecount += 4;
                        xcount = 0;
                        ++ycount;
                        vfbptr = ImVfbQUp( vfb, vfbptr );
                        vfbptr = ImVfbQUp( vfb, vfbptr );
                        pixel = ImVfbQIndex8( vfb, vfbptr );
                        vfbptr = ImVfbQRight( vfb, vfbptr );
                }

		/*
		 * Boundary condition: 2 pixels at the end
		 *	of a scanline.
		 */
                if( xcount == (x-2) )
                {
			/*
			 * If the last two pixels are the same
			 * then write them as a run of 2 with
			 * pixel value.  Else, write each
			 * pixel value as runs of 1.
			 */
			if( pixel == ImVfbQIndex8( vfb, vfbptr ) )
			{
				imBmpWrite2Bytes( 2, pixel );
                        	bytecount += 4;
			}
			else
			{
				imBmpWrite2Bytes( 1, pixel );
				imBmpWrite2Bytes( 1, ImVfbQIndex8(vfb,vfbptr));
                        	bytecount += 6;
			}
 
                        /*
			 * Write end of scanline code
			 */
			imBmpWrite2Bytes( 0, 0 );
 
			/*
			 * Adjust counters and vfb pointers
			 */
                        xcount = 0;
                        ++ycount;
                        vfbptr = ImVfbQRight( vfb, vfbptr );
                        vfbptr = ImVfbQUp( vfb, vfbptr );
                        vfbptr = ImVfbQUp( vfb, vfbptr );
                        pixel = ImVfbQIndex8( vfb, vfbptr );
                        vfbptr = ImVfbQRight( vfb, vfbptr );
                }

		/*
		 * Encoded Mode - encoding of multiple consecutive
		 *	pixels of the same value into a run and
		 *	pixel value
		 */
		if( pixel == ImVfbQIndex8( vfb, vfbptr ) )
		{
			run = 0;
			nowrite = TRUE;

			/*
			 * While two adjacent pixels are the same,
			 * count them up.  If we reach the
			 * end of a scanline, write out the
			 * current run and pixel.  If we
			 * have a run of 255 pixels, write
			 * out the run and pixel.
			 */
			while( pixel == ImVfbQIndex8( vfb, vfbptr ) )
			{
				++run;
				++xcount;
				pixel = ImVfbQIndex8( vfb, vfbptr );
				vfbptr = ImVfbQRight( vfb, vfbptr );

				/*
				 * End of a scanline
				 */
				if( xcount == x-1 )
				{
					/*
					 * Write run and pixel value
					 */
					imBmpWrite2Bytes( run+1, pixel );

					/*
					 * Write end of scanline code
					 */
					imBmpWrite2Bytes( 0, 0 );

					/*
					 * Adjust counters and vfb pointers
					 */
					bytecount += 4;
					xcount = 0;
					++ycount;
					vfbptr = ImVfbQUp( vfb, vfbptr );
					vfbptr = ImVfbQUp( vfb, vfbptr );
					pixel = ImVfbQIndex8( vfb, vfbptr );
					vfbptr = ImVfbQRight( vfb, vfbptr );
					nowrite = FALSE;
					break;
				}

				/*
				 * End of 255 pixels
				 */
				if( run == 254 )
				{
					/*
					 * Write run and pixel value
					 */
					imBmpWrite2Bytes( 255, pixel );

					/*
					 * Adjust counters and vfb pointers
					 */
					bytecount += 2;
					run = 0;
					++xcount;
					pixel = ImVfbQIndex8( vfb, vfbptr );
					vfbptr = ImVfbQRight( vfb, vfbptr );
					nowrite = FALSE;
					break;
				}
			}

			/*
			 * If we haven't written out the run and pixel,
			 *	do it now.
			 */
			if( nowrite == TRUE )
			{
				/*
				 * Write run and pixel
				 */
				imBmpWrite2Bytes( run+1, pixel );

				/*
				 * Adjust counters and vfb pointers
				 */
				bytecount += 2;
				++xcount;
				pixel = ImVfbQIndex8( vfb, vfbptr );
				vfbptr = ImVfbQRight( vfb, vfbptr );
			}
		}
 
		/*
		 * Absolute Mode - copying of consecutive different pixels
		 *	literally
		 */
		if( (pixel != ImVfbQIndex8( vfb, vfbptr ))
			&& (xcount < (x-2) )
			&& (ycount < y) )
		{
			run = 0;
			nowrite = TRUE;

			pixbuf[ 0 ] = pixel;
			pixel = ImVfbQIndex8( vfb, vfbptr );
			vfbptr = ImVfbQRight( vfb, vfbptr );
			++xcount;

			/*
			 * Since Absolute Mode requires that there
			 * be at least 3 different adjacent
			 * pixels, we check for this condition
			 * first.  If there are only 1 or 2
			 * different adjacent bytes, then
			 * we write them out as runs of 1
			 * and pixel.
			 */
			if( pixel == ImVfbQIndex8( vfb, vfbptr ) )
			{
				/*
				 * There is only 1 different pixel
				 */ 

				/*
				 * Write out run and pixel
				 */
				imBmpWrite2Bytes( 1, pixbuf[ 0 ] );

				bytecount += 2;
				nowrite = FALSE;
			}
			else
			{
				pixbuf[ 1 ] = pixel;
				pixel = ImVfbQIndex8( vfb, vfbptr );
				vfbptr = ImVfbQRight( vfb, vfbptr );
				++xcount;
				if( pixel == ImVfbQIndex8( vfb, vfbptr ) )
				{
					/*
					 * There are only 2 different
					 * adjacent pixels
					 */

					/*
					 * Write run and pixel
					 */
					imBmpWrite2Bytes( 1, pixbuf[ 0 ] );

					/*
					 * Write run and pixel
					 */
					imBmpWrite2Bytes( 1, pixbuf[ 1 ] );

					bytecount += 4;
					nowrite = FALSE;
				}
				else
				{
					/*
					 * There are at least 3 different
					 * adjacent pixels, so we reset
					 * the vfb pointers
					 */
					xcount -= 2;
					vfbptr = ImVfbQLeft( vfb, vfbptr );
					vfbptr = ImVfbQLeft( vfb, vfbptr );
					vfbptr = ImVfbQLeft( vfb, vfbptr );
					pixel = ImVfbQIndex8( vfb, vfbptr );
					vfbptr = ImVfbQRight( vfb, vfbptr );
				}
			}
			
			/*
			 * While adjacent pixels are different, count them
			 * up and store them in pixbuf.
			 */
			while( pixel != ImVfbQIndex8( vfb, vfbptr ) )
			{
				pixbuf[ run ] = pixel;
				pixel = ImVfbQIndex8( vfb, vfbptr );
				vfbptr = ImVfbQRight( vfb, vfbptr );
				++run;
				++xcount;

				/*
				 * End of scanline
				 */
				if( xcount == x-1 )
				{
					pixbuf[ run ] = pixel;

					/*
					 * Write escape(0) and run
					 */
					imBmpWrite2Bytes( 0, run+1 );

					/*
					 * Write pixels in pixbuf
					 */
					for( i = 0; i < run+1; ++i )
					{
						buf[ bufin ] = pixbuf[ i ];
						++bufin;
						if( bufin > (IMBLKSIZE-1) )
						{
							if( ImBinWrite( ioType,
								fd,fp,buf,
								UCHAR,1,
								IMBLKSIZE )==-1)
							{
							    ImReturnBinError( );
							}
							bufin = 0;
						}
					}

					/*
					 * Pad to word boundary if necessary
					 */
					if( ((run+1)%2) != 0 )
					{
						buf[ bufin ] = 0;
						++bufin;
						if( bufin > (IMBLKSIZE-1) )
						{
							if( ImBinWrite( ioType,
								fd,fp,buf,
								UCHAR,1,
								IMBLKSIZE )==-1)
							{
							    ImReturnBinError( );
							}
							bufin = 0;
						}
						++bytecount;
					}

					/*
					 * Write end of scanline code
					 */
					imBmpWrite2Bytes( 0, 0 );

					/*
					 * Adjust counters and vfb pointers
					 */
					bytecount += (5+run);
					xcount = 0;
					++ycount;
					vfbptr = ImVfbQUp( vfb, vfbptr );
					vfbptr = ImVfbQUp( vfb, vfbptr );
					pixel = ImVfbQIndex8( vfb, vfbptr );
					vfbptr = ImVfbQRight( vfb, vfbptr );
					nowrite = FALSE;
					break;
				}

				/*
				 * End of 255 consecutive different pixels
				 */
				if( run == 254 )
				{
					pixbuf[ run ] = pixel;

					/*
					 * Write escape code(0) and run
					 */
					imBmpWrite2Bytes( 0, 255 );

					/*
					 * Write pixel literals
					 */
					for( i = 0; i < 255; ++i )
					{
						buf[ bufin ] = pixbuf[ i ];
						++bufin;
						if( bufin > (IMBLKSIZE-1) )
						{
							if( ImBinWrite( ioType,
								fd,fp,buf,
								UCHAR,1,
								IMBLKSIZE )==-1)
							{
							    ImReturnBinError( );
							}
							bufin = 0;
						}
					}

					/*
					 * Write word boundary padding
					 */ 
					buf[ bufin ] = 0;
					++bufin;
					if( bufin > (IMBLKSIZE-1) )
					{
						if( ImBinWrite( ioType,
							fd,fp,buf,
							UCHAR,1,
							IMBLKSIZE )==-1)
						{
						    ImReturnBinError( );
						}
						bufin = 0;
					}

					/*
					 * Adjust counters and vfb pointers
					 */
					bytecount += 258;
					run = 0;
					++xcount;
					pixel = ImVfbQIndex8( vfb, vfbptr );
					vfbptr = ImVfbQRight( vfb, vfbptr );
					nowrite = FALSE;
					break;
				}
			}

			/*
			 * If we haven't written out the pixels, then
			 * write it now
			 */
			if( nowrite == TRUE )
			{
				/*
				 * Write escape code(0) and run
				 */
				imBmpWrite2Bytes( 0, run );

				/*
				 * Write pixel literals
				 */
				for( i = 0; i < run; ++i )
				{
					buf[ bufin ] = pixbuf[ i ];
					++bufin;
					if( bufin > (IMBLKSIZE-1) )
					{
						if( ImBinWrite( ioType,fd,fp
							,buf,UCHAR,1
							,IMBLKSIZE )==-1)
						{
							ImReturnBinError( );
						}
						bufin = 0;
					}
				}

				/*
				 * Write padding to word boundary if
				 * necessary
				 */
				if( run%2 != 0 )
				{
					buf[ bufin ] = 0;
					++bufin;
					if( bufin > (IMBLKSIZE-1) )
					{
						if( ImBinWrite( ioType,fd,fp
							,buf,UCHAR,1
							,IMBLKSIZE )==-1)
						{
							ImReturnBinError( );
						}
						bufin = 0;
					}
					++bytecount;
				}

				/*
				 * Adjust bytecount
				 */
				bytecount += (2+run);
			}
		}
	}

	/*
	 * Write end of bitmap code
	 */
	buf[ bufin ] = 0;
	++bufin;
	buf[ bufin ] = 1;
	++bufin;
	if( ImBinWrite( ioType,fd,fp,buf,UCHAR,1,bufin )==-1)
	{
		ImReturnBinError( );
	}

	/*
	 * Adjust bytecount
	 */
	bytecount += 2;

	/*
	 * Seek to header and write file size
	 */
	if( ImSeek( ioType, fd, fp, 2, L_SET ) == -1 )
	{
		ImReturnBinError( );
	}
	header.bmp_size = IMBMPHEADERSIZE + ( ncolors * IMBMPRGBQUADSIZE )
		+ bytecount;
	if( ImBinWrite( ioType, fd, fp, &header.bmp_size, UINT32, 4, 1) == -1 )
	{
		ImReturnBinError( );
	}

	/*
	 * Seek to header and write bitmap size
	 */
	if( ImSeek( ioType, fd, fp, 34, L_SET ) == -1 )
	{
		ImReturnBinError( );
	}
	header.bmp_bisizeimage =  bytecount;
	if( ImBinWrite( ioType, fd, fp, &header.bmp_bisizeimage,
		UINT32, 4, 1) == -1 )
	{
		ImReturnBinError( );
	}

	return( 1 );
}
