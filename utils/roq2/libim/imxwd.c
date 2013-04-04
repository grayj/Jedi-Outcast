/**
 **	$Header: /roq/libim/imxwd.c 1     11/02/99 4:38p Zaphod $
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

#define HEADER        "    $Header: /roq/libim/imxwd.c 1     11/02/99 4:38p Zaphod $"

/**
 **  FILE
 **	imxwd.c		-  X Window System window dump file I/O
 **
 **  PROJECT
 **	libim	-  SDSC image manipulation library
 **
 **  DESCRIPTION
 **	imxwd.c contains routines to read and write X XWD files for
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
 **
 **	none
 **
 **  PRIVATE CONTENTS
 **	imXwdRead	f  read an X XWD file
 **	imXwdWrite8	f  write an 8-bit X XWD file
 **	imXwdWriteRGB	f  write a 24-bit X XWD file
 **
 **	imXwdHeaderInfo		t  header for X window dump files
 **	imXwdHeaderFields	v  header description for binary I/O package
 **	imXwdColor		t  CLT entry
 **
 **	imXwd8Read8		f  read 8-bit  image in 8-bit bitmapUnit
 **	imXwd8Read16		f  read 8-bit  image in 16-bit bitmapUnit
 **	imXwd8Read32		f  read 8-bit  image in 32-bit bitmapUnit
 **	imXwd24Read32		f  read 24-bit image in 32-bit bitmapUnit
 **
 **	imXwdWriteHeader	f  write an XWD file header
 **
 **  HISTORY
 **	$Log: /roq/libim/imxwd.c $
* 
* 1     11/02/99 4:38p Zaphod
 **	Revision 1.12  1995/06/29  00:28:04  bduggan
 **	updated copyright year
 **
 **	Revision 1.11  1995/04/03  21:42:00  bduggan
 **	took out #ifdef NEWMAGIC
 **
 **	Revision 1.10  1995/01/10  23:51:41  bduggan
 **	made read/write routines static
 **
 **	Revision 1.9  94/10/03  11:31:15  nadeau
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
 **	Revision 1.8  92/12/03  01:56:59  nadeau
 **	Corrected info messages.
 **	
 **	Revision 1.7  92/11/04  12:11:11  groening
 **	put ImFIleFormat info and magic number info
 **	from imfmt.c into this file.
 **	
 **	Revision 1.6  92/09/25  17:08:11  vle
 **	Modified to use byte ordering info in header to read in data.  This
 **	fixes the Decstation bug.  Added code to read in window name in
 **	header, instead of just ignoring it.  Updated copyright notice.
 **	Added ImInfo() messages.
 **	
 **	Revision 1.5  92/05/05  16:51:42  vle
 **	Fixed 24-bit write bug that caused imconv to segmentation fault.
 **	
 **	Revision 1.4  92/03/26  11:21:07  vle
 **	Fixed 24-bit read bug that caused imconv to segmentation fault.
 **	Added code to more closely match xwud.c's interpretation of
 **	the "flags" field.
 **	
 **	Revision 1.3  91/10/03  09:25:42  nadeau
 **	Added 24-bit image read and write support.
 **	
 **	Revision 1.2  91/02/12  10:49:11  nadeau
 **	Removed the tag table checking and VFB conversion now
 **	handled by ImFileRead and ImFileWrite.
 **	
 **	Revision 1.1  90/07/25  16:21:46  nadeau
 **	Initial revision
 **/

#include "iminternal.h"





/**
 **  FORMAT
 **	XWD	-  X Window System window dump
 **
 **  AKA
 **	x11
 **
 **  FORMAT REFERENCES
 **	 Xlib - C Language X Interface, MIT X Window System,  Version 11.
 **
 **  CODE CREDITS
 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1990.
 **
 **  DESCRIPTION
 **	The X Window System's XWD (X Window Dump) format is that produced
 **	by the 'xwd' utility in the X tool set.  XWD files may be redisplayed
 **	using 'xwud' (un-dump).
 **
 **	Version 11 of X upgraded the XWD format from its larval stages in
 **	version 10.  This code only supports the X11 version.
 **
 **	XWD files start with a fixed size header describing the file's contents:
 **
 **	headerSize	The size of the "entire" header, in bytes.  The "entire"
 **			header includes the fixed size header, and a window
 **			name string immediately following the header.
 **
 **	version		The format's version number.  For X10, this is a 6.
 **			For X11, this is a 7.
 **
 **	pixmapFormat	The storage format for the image.  One of:
 **
 **			XY bitmap	A 1-bit deep image.  Consecutive
 **					bits are for consecutive pixels.
 **
 **			XY pixmap	A multi-bit deep image.  Consecutive
 **					bits are for consecutive pixels, one
 **					plane at a time.  (ie., all of bit
 **					plane 1, then all of bit plane 2, ...).
 **
 **			Z pixmap	A multi-bit deep image.  Consecutive
 **					bits make up a complete multi-bit pixel,
 **					followed by the next pixel, ...
 **
 **	pixmapDepth	The number of bits that make up one pixel.
 **
 **	pixmapWidth	The width of the image, in pixels.
 **
 **	pixmapHeight	The height of the image, in pixels.
 **
 **	xOffset		An offset for the image in the X direction.  Ignored
 **			by this and most software.
 **
 **	byteOrder	The byte order for multi-byte data.  Only relevant
 **			when the bitmapUnit (see below) is greater than one
 **			byte.
 **
 **	bitmapUnit	The unit of storage for pixel data.  Pixels of size
 **			bitsPerPixel (see blow) are packed into chunks of
 **			size bitmapUnit bits.  For example, 8-bit index
 **			images might have bitsPerPixel = 8, and bitmapUnit
 **			= 32.
 **
 **	bitmapBitOrder	The order of bits within a byte of data.
 **
 **	bitmapPad	The unit of storage to which scanlines are padded.
 **			Usually the same as bitmapUnit.
 **
 **	bitsPerPixel	The number of bits that make up one pixel, as stored
 **			in the file.  For a 24-bit image, the pixmapDepth
 **			would be 24, but the bitsPerPixel could be 32.  This
 **			indicates that each 32-bit file quantity contains
 **			24 bits of pixel data and 8 bits that are to be ignored.
 **
 **	bytesPerLine	The number of bytes of data per scanline, including
 **			padding (see bitmapPad).
 **
 **	visualClass	How colormaps are handled.  One of
 **
 **			StaticGrey	Fixed colormap greyscale
 **			GreyScale	Variable colormap greyscale
 **
 **			StaticColor	Fixed colormap color
 **			PseudoColor	Variable colormap color
 **
 **			TrueColor	Fixed RGB
 **			DirectColor	Variable RGB
 **
 **			8-bit index displays are StaticColor if the CLT is
 **			not changeable (owned by the server?), and PseudoColor
 **			if the CLT can be changed to that of the image.  A
 **			StaticColor image displayed by 'xwud' uses the server's
 **			default CLT.  A PseudoColor image displayed by 'xwud'
 **			uses the CLT given with the image.  When the cursor
 **			is outside the image's window, the normal CLT's are
 **			used.  When the cursor is inside the window, the
 **			image's CLT is used.
 **
 **			RGB displays are almost always TrueColor.  DirectColor
 **			displays use a CLT at the "backend" of the display
 **			hardware and are usually only there for fancy gamma
 **			correction.
 **
 **	redMask		A mask to get the red-component bits from an RGB pixel.
 **
 **	greenMask	A mask to get the green-component bits from an RGB pixel
 **
 **	blueMask	A mask to get the blue-component bits from an RGB pixel.
 **
 **	bitsPerRGB	The number of bits for one component of an RGB image.
 **			Typically 8.
 **
 **	colormapEntries	The maximum number of entries allowed in a CLT.  This
 **			is 2 to the power bitsPerPixel.  For 8-bit images,
 **			this would be 256.
 **
 **	nColors		The number of CLT entries actually given in the image
 **			file.  This may be less than or equal to
 **			colormapEntries.
 **
 **	windowWidth	The width of the window to display the image.
 **
 **	windowHeight	The height of the window to display the image.
 **
 **	windowX		The X location of the window to display the image.
 **
 **	windowY		The Y location of the window to display the image.
 **
 **	windowBorderWith The width of the border around the window to display
 **			the image.
 **
 **	headerEnd	32-bits of padding to make the header an even number
 **			of 32-bit words.  However, it appears that even the
 **			Cray does not use this field, so we drop it.
 **
 **	Immediately following the fixed size header is a variable number of
 **	bytes giving the name of the window, NULL terminated.  The number of
 **	bytes is given by (headerSize - sizeof( header )), where sizeof(
 **	header ) is, of course, given in a non-machine-specific way.
 **
 **	If the window has no name, 'xwd' uses the name 'xwdump'.
 **
 **	Following the header (headerSize bytes into the file) are zero or more
 ** 	CLT entries (see nColors).  Each CLT entry gives:
 **
 **	pixel		The CLT index at which to store the color.
 **
 **	red,green,blue	The RGB components of the color.  Color components are
 **			each 16-bits wide.  Color values are scaled down to
 **			fit into standard 8-bit per component CLTs:
 **
 **				CLT[pixel].red   = red   * 256 / 65536
 **				CLT[pixel].green = green * 256 / 65536
 **				CLT[pixel].blue  = blue  * 256 / 65536
 **
 **	flags		Flags indicating which of the components should be
 **			stored into the CLT entry.  Flags is the bitwise OR of:
 **
 **			DoRed	store the red component
 **			DoGreen	store the green component
 **			DoBlue	store the blue component
 **				
 **			Note: X11R5 xwud incorrectly ignores this field,
 **			so images that xwud displays uses the whole CLT.
 **			This does not follow the X Window specs!
 **
 **	pad		Unused space to pad structure to a multiple of 4 bytes
 **			in length.
 **
 **	After all that is the image data.
 **
 **  IMAGE DATA READ ALGORITHM
 **	With all of the bitsPerThis and bytesPerThat fields in the header,
 **	a very wide range of pixel storage arrangements are possible.  A
 **	generic read algorithm might be structured as follows:
 **
 **		-- read in the raw data
 **		nBytes = bytesPerLine * pixmap_Height
 **		read nBytes of data into buffer
 **
 **		-- byte swap based on byteOrder and bitmapUnit
 **		if byteOrder is MBF,
 **			for each group of (bitmapUnit/8) bytes,
 **				reverse their order
 **
 **		-- bit swap based on bitmapBitOrder
 **		if bitmapBitOrder is MBF,
 **			for each byte,
 **				reverse the bit order
 **
 **		-- copy them to the VFB
 **		for each scanline,
 **			for each pixel in the scanline,
 **				copy the next pixmapDepth bits into VFB
 **				advance to next VFB pixel
 **				advance in buffer by bitsPerPixel bits
 **			skip to the next bitmapPad bit boundary
 **
 **	This algorithm is generic and handles all of the quirky features of
 **	the header.  It is also painfully slow.
 **
 **	The reality of image storage is that there are only a couple common
 **	configurations:
 **
 **		pixmapDepth	8 or 24
 **		byteOrder	MBF or LBF (but usually MBF)
 **		bitmapUnit	8, 16, or 32 (but usually 32)
 **		bitmapBitOrder	LBF
 **		bitmapPad	8, 16, or 32 (but usually 32)
 **		bitsPerPixel	8 or 32
 **
 **	Rather than use a generic, but very slow algorithm for image data
 **	read, this code uses four depth-specific read routines that handle
 **	each of the common image depth cases:
 **
 **		pixmapDepth	8
 **		bitmapUnit	8
 **
 **		pixmapDepth	8
 **		bitmapUnit	16
 **
 **		pixmapDepth	8
 **		bitmapUnit	32
 **
 **		pixmapDepth	24
 **		bitmapUnit	32
 **
 **	All other cases are considered unreadable.
 **
 **  RESTRICTIONS
 **	In addition to the read restrictions discussed above, this code
 **	handles XWD file header information as follows on read operations:
 **
 **		version		Must be 7.
 **		pixmapFormat	Must be a Z pixmap.
 **		xOffset		Ignored.
 **		visualClass	Ignored.
 **		window*		Ignored.
 **
 **	Images written by this code set header values to the following:
 **
 **		version		Always 7.
 **		pixmapFormat	Always a Z pixmap.
 **		pixmapDepth	Always 8 or 24.
 **		xOffset		Always 0.
 **		byteOrder	Always MBF.
 **		bitmapUnit	Always 8 or 32 bits.
 **		bitmapBitOrder	Always MBF.
 **		bitmapPad	Always the same as bitmapUnit.
 **		bitsPerPixel	Always 8 or 32.
 **		visualClass	Always PseudoColor (8-bit) or TrueColor (RGB).
 **		windowWidth	Always the same as pixmapWidth.
 **		windowHeight	Always the same as pixmapHeight.
 **		windowX		Always 0.
 **		windowY		Always 0.
 **		windowBorderWidth Always 0.
 **/


/*
 *  Flag the removal of the optional 'headerEnd' field of the header.  This
 *  field is flagged in the standard X XWDFile.h include file as only being
 *  included on 64-bit systems.  However, the Cray doesn't even include it.
 *  So, we don't either.
 */
#define no_headerEnd


/*
 *  XWD - MIT X11 Window System Window Dump
 *      For information on these structures, how to use them, etc. please
 *      see imfmt.c.
 */
#ifdef __STDC__
static int imXwdRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );
static int imXwdWrite8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, 
	TagTable *flagsTable, TagTable *tagTable );
static int imXwdWriteRGB( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, 
	TagTable *flagsTable, TagTable *tagTable );
#else
static int imXwdRead( );
static int imXwdWrite8( ), imXwdWriteRGB( );
#endif

#ifdef notdef
static int imXwdWrite1( );
#endif
static char *imXwdNames[ ]  = { "xwd", "x11", NULL };
static ImFileFormatReadMap imXwdReadMap[ ] =
{
        /* in                   out                                     */
        /* type,ch,dep, attr.   VFB type        attr.                   */
#ifdef notdef
        { IN,1,1,       0,      IMVFBMONO,      0 },
        { IN,1,1,       C,      IMVFBMONO,      C },
#endif
        { IN,1,8,       0,      IMVFBINDEX8,    0 },
        { IN,1,8,       C,      IMVFBINDEX8,    C },
        { RGB,3,8,      0,      IMVFBRGB,       0 },
        { RGB,3,8,      C,      IMVFBRGB,       0 },
        { -1,           0,      -1,             0 },
};

static ImFileFormatWriteMap imXwdWriteMap[ ] =
{
        /* in                   out                                     */
        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */
#ifdef notdef
        { IMVFBMONO,    0,      IN,1,1,         0,      imXwdWrite1 },
        { IMVFBMONO,    C,      IN,1,1,         C,      imXwdWrite1 },
        { IMVFBINDEX8,  0,      IN,1,8,         0,      imXwdWrite8 },
#endif
        { IMVFBINDEX8,  C,      IN,1,8,         C,      imXwdWrite8 },
        { IMVFBRGB,     0,      RGB,3,8,        0,      imXwdWriteRGB },
        { -1,           0,      -1,             0,      NULL },
};


static ImFileMagic imFileXwdMagic []=
{
	{ 0, 0, NULL },
};

ImFileFormat ImFileXwdFormat =
{
	imXwdNames, "X Window System window dump image file",
	"X Consortium / MIT",
#ifdef notdef
	"1-bit XY format bitmaps.  8-bit and 24-bit RGB Z format pixmaps.\n\
8-bit and 24-bit RGB Z format pixmaps, with or without a CLT.",
	"1-bit XY format bitmaps.  8-bit and 24-bit RGB Z format pixmaps.\n\
8-bit and 24-bit RGB Z format pixmaps, with or without a CLT.",
#else
	"8-bit and 24-bit RGB Z format pixmaps, with or without a CLT.",
	"8-bit with CLT and 24-bit RGB Z format pixmaps without CLT.",
#endif
	imFileXwdMagic,
	IMNOMULTI, IMPIPE,
	IMNOMULTI, IMPIPE,
	imXwdRead, imXwdReadMap, imXwdWriteMap
};







/*
 *  TYPEDEF & STRUCT
 *	imXwdHeaderInfo		-  header for X window dump files
 *	imXwdHeaderFields	-  header description for binary I/O package
 *
 *  DESCRIPTION
 *	XWD files start with a header that gives various details of the
 *	image contained in the file.  Different formats of image storage
 *	will use different numbers of fields in the header, the other fields
 *	being ignored.
 *
 *	Immediately following the header is the name of the window, if any.
 */

typedef struct imXwdHeaderInfo		/* X11 window dump, version 7	*/
{
	unsigned long	xwd_pixmapFormat;	/* Pixmap format		*/

	unsigned long	xwd_pixmapDepth;	/* Pixmap depth			*/
	unsigned long	xwd_pixmapWidth;	/* Pixmap width			*/
	unsigned long	xwd_pixmapHeight;	/* Pixmap height		*/

	unsigned long	xwd_xOffset;		/* Bitmap x offset		*/

	unsigned long	xwd_byteOrder;		/* MSBFirst, LSBFirst		*/

	unsigned long	xwd_bitmapUnit;		/* Bitmap unit			*/
	unsigned long	xwd_bitmapBitOrder;	/* MSBFirst, LSBFirst		*/
	unsigned long	xwd_bitmapPad;		/* Bitmap scanline pad		*/

	unsigned long	xwd_bitsPerPixel;	/* Bits per pixel		*/
	unsigned long	xwd_bytesPerLine;	/* Bytes per scanline		*/

	unsigned long	xwd_visual;		/* Class of colormap		*/

	unsigned long	xwd_redMask;		/* Red mask			*/
	unsigned long	xwd_greenMask;		/* Green mask			*/
	unsigned long	xwd_blueMask;		/* Blue mask			*/

	unsigned long	xwd_bitsPerRGB;		/* Log base 2 of distinct color	*/

	unsigned long	xwd_colormapEntries;	/* Number of entries in colormap*/
	unsigned long	xwd_nColors;		/* Number of Color structures	*/

	unsigned long	xwd_windowWidth;	/* Window width			*/
	unsigned long	xwd_windowHeight;	/* Window height		*/
	long	xwd_windowX;		/* Window upper left X coordinate*/
	long	xwd_windowY;		/* Window upper left Y coordinate*/
	unsigned long	xwd_windowBorderWidth;	/* Window border width		*/

#ifndef no_headerEnd
	/* Removed since even the Cray doesn't include it in the header.*/
	unsigned long	xwd_headerEnd;		/* Header padding		*/
#endif
} imXwdHeaderInfo;

static BinField imXwdHeaderFields[ ] =
{
	/* xwd_pixmapFormat */		ULONG, 4, 1,
	/* xwd_pixmapDepth */		ULONG, 4, 1,
	/* xwd_pixmapWidth */		ULONG, 4, 1,
	/* xwd_pixmapHeight */		ULONG, 4, 1,
	/* xwd_xOffset */		ULONG, 4, 1,
	/* xwd_byteOrder */		ULONG, 4, 1,
	/* xwd_bitmapUnit */		ULONG, 4, 1,
	/* xwd_bitmapBitOrder */	ULONG, 4, 1,
	/* xwd_bitmapPad */		ULONG, 4, 1,
	/* xwd_bitsPerPixel */		ULONG, 4, 1,
	/* xwd_bytesPerLine */		ULONG, 4, 1,
	/* xwd_visual */		ULONG, 4, 1,
	/* xwd_redMask */		ULONG, 4, 1,
	/* xwd_greenMask */		ULONG, 4, 1,
	/* xwd_blueMask */		ULONG, 4, 1,
	/* xwd_bitsPerRGB */		ULONG, 4, 1,
	/* xwd_colormapEntries */	ULONG, 4, 1,
	/* xwd_nColors */		ULONG, 4, 1,
	/* xwd_windowWidth */		ULONG, 4, 1,
	/* xwd_windowHeight */		ULONG, 4, 1,
	/* xwd_windowX */		 LONG, 4, 1,
	/* xwd_windowY */		 LONG, 4, 1,
	/* xwd_windowBorderWidth */	ULONG, 4, 1,
#ifndef no_headerEnd
	/* Removed since even the Cray doesn't include it in the header.*/
	/* xwd_headerEnd */		ULONG, 4, 1,
#endif
					    0, 0, 0,
};


unsigned char	*xwd_windowName = NULL;		/* XWD window name in file	*/


#ifndef no_headerEnd
#define IMXWDHEADERSIZE	(26 * 4)	/* 26 4-byte integers		*/
					/* includes header size & version*/
					/* fields not in above struct	*/
#else
#define IMXWDHEADERSIZE	(25 * 4)	/* 25 4-byte integers		*/
					/* includes header size & version*/
					/* fields not in above struct	*/
#endif


/*
 *  Value for xwd_version:
 */
#define IMXWDVERSION		7	/* 1st X11 version		*/

/*
 *  Values for xwd_byteOrder and xwd_bitmapBitOrder:
 */
#define IMXWDLBF		0	/* Least significatn first	*/
#define IMXWDMBF		1	/* Most significatn first	*/

/*
 *  Values for xwd_pixmapFormat:
 */
#define IMXWDXYBITMAP		0	/* 1-bit plane bitmap		*/
#define IMXWDXYPIXMAP		1	/* plane interleaved pixmap	*/
#define IMXWDZPIXMAP		2	/* uninterleaved pixmap		*/

/*
 *  Values for xwd_visual:
 */
#define IMXWDSTATICGREY		0	/* Fixed greyscale		*/
#define IMXWDGREYSCALE		1	/* Adjustable greyscale		*/
#define IMXWDSTATICCOLOR	2	/* Fixed colormap		*/
#define IMXWDPSEUDOCOLOR	3	/* Adjustable colormap		*/
#define IMXWDTRUECOLOR		4	/* Fixed RGB			*/
#define IMXWDDIRECTCOLOR	5	/* Adjustable RGB		*/





/*
 *  TYPEDEF & STRUCT
 *	imXwdColor	-  CLT entry
 *
 *  DESCRIPTION
 *	Rather than dump the entire CLT to the file, XWD files may contain
 *	only those CLT entries actually used by the image.  Each Color
 *	structure gives the CLT index to fill and the RGB value to place
 *	there.
 */

typedef struct imXwdColor
{
	unsigned long	xwd_pixel;		/* CLT index			*/
	unsigned short	xwd_red, xwd_green, xwd_blue;/* RGB value for that location*/
	char	xwd_flags;		/* DoRed, DoGreen, DoBlue	*/
	char	xwd_pad;		/* Unused space			*/
} imXwdColor;

static BinField imXwdColorFields[ ] =
{
	/* xwd_pixel */		ULONG,  4, 1,
	/* xwd_red */		USHORT, 2, 1,
	/* xwd_green */		USHORT, 2, 1,
	/* xwd_blue */		USHORT, 2, 1,
	/* xwd_flags */		CHAR,   1, 1,
	/* xwd_pad */		CHAR,   1, 1,
				0,      0, 0,
};

/*
 *  Values for xwd_flags:
 */

#define IMXWDDORED	(1<<0)
#define IMXWDDOGREEN	(1<<1)
#define IMXWDDOBLUE	(1<<2)


#define IMXWDNONAME	"xwdump"


#ifdef __STDC__
static int imXwd8Read8(int ioType, int fd, FILE *fp, imXwdHeaderInfo *header, ImVfb **vfb);
static int imXwd8Read16(int ioType, int fd, FILE *fp, imXwdHeaderInfo *header, ImVfb **vfb);
static int imXwd8Read32(int ioType, int fd, FILE *fp, imXwdHeaderInfo *header, ImVfb **vfb);
static int imXwd24Read32( int ioType, int fd, FILE *fp, imXwdHeaderInfo *header, ImVfb **vfb, ImClt *clt );
#else
static int imXwd8Read8();
static int imXwd8Read16();
static int imXwd8Read32();
static int imXwd24Read32();
#endif




/*
 *  FUNCTION
 *	imXwdRead	-  read an X XWD file
 *	imXwd8Read16	-  read 16-bit bitmapUnit image
 *	imXwd8Read32	-  read 32-bit bitmapUnit image
 *
 *  DESCRIPTION
 *	The file header is read, followed by the CLT (if any).  Based upon
 *	the bitmapUnit one of three routines are called to read the image
 *	data into a new VFB.  The VFB (and CLT?) are added to the tag table.
 */

static int				/* Returns # tags read in	*/
#ifdef __STDC__
imXwdRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )
#else
imXwdRead( ioType, fd, fp, flagsTable, tagTable )
	int          ioType;		/* I/O flags			*/
	int          fd;		/* Input file descriptor	*/
	FILE	    *fp;		/* Input file pointer		*/
	TagTable    *flagsTable;	/* Flags			*/
	TagTable    *tagTable;		/* Tag table to add to		*/
#endif
{
	ImVfb       *vfb;		/* Read in image		*/
	ImVfbPtr     pPixel;		/* Xwdel pointer		*/

	ImClt       *clt;		/* Image CLT			*/
	ImCltPtr     pColor;		/* Color pointer		*/
	imXwdColor   color;		/* XWD color			*/
	unsigned long	     mask;		/* Pixel index mask		*/

	unsigned long        headerSize;	/* Size, in bytes, of header	*/
	unsigned long	     version;		/* Format version		*/
	imXwdHeaderInfo header;		/* XWD file header		*/

	unsigned char        swapBuffer[sizeof(unsigned long)];	/* Byte swapping buffer	*/
	int          i;			/* Counter			*/
	char         message[100];	/* Error message buffer		*/


	/*
	 *  Read in the header size and version number (each 4 bytes long).
	 *
	 *  We don't know what byte order to use, so we assume MBF and
	 *  read in these two values.  If the version number is not one
	 *  we recognize, we try swapping bytes.  If we recognize it then,
	 *  then we have an LBF file instead.  If we still don't recognize
	 *  it, then probably the file is in a version we don't understand
	 *  and we give up.
	 */
	BinByteOrder( BINMBF );
	if ( ImBinRead( ioType, fd, fp, &headerSize, ULONG, 4, 1 ) == -1 )
		ImReturnBinError( );
	if ( ImBinRead( ioType, fd, fp, &version, ULONG, 4, 1 ) == -1 )
		ImReturnBinError( );

	if ( version != IMXWDVERSION )
	{
		/*
		 *  Swap bytes of version value.  Don't depend upon a unsigned long
		 *  being 4 bytes long.
		 */
		for ( i = 0; i < sizeof( unsigned long ); i++ )
			swapBuffer[i] = (version>>(i*8)) & 0xFF;
		version = 0;
		for ( i = 0; i < sizeof( unsigned long ); i++ )
			version = (version << 8) | swapBuffer[i];
		if ( version != IMXWDVERSION )
		{
			sprintf( message, "Unsupported XWD format version. Only support version %d.", IMXWDVERSION );
			ImErrorFatal( message, -1, IMEUNSUPPORTED );
		}
		for ( i = 0; i < sizeof( unsigned long ); i++ )
			swapBuffer[i] = (headerSize>>(i*8)) & 0xFF;
		headerSize = 0;
		for ( i = 0; i < sizeof( unsigned long ); i++ )
			headerSize = (headerSize << 8) | swapBuffer[i];
		BinByteOrder( BINLBF );
	}

	/*
	 *  Read in the rest of the header.  Skip the window name.
	 */
	if ( ImBinReadStruct( ioType, fd, fp, &header, imXwdHeaderFields )== -1)
		ImReturnBinError( );
	i = headerSize - IMXWDHEADERSIZE;
	if ( i < 0 )
	{
		/* Whoa!  Stated header size is bogus.  Give up!	*/
		ImErrorFatal( "Bogus XWD file header size!", -1, IMESYNTAX );
	}
	if ( i > 0 )	/* read window name */
	{
		ImMalloc( xwd_windowName, unsigned char*, i+1 );
		if( ImBinRead( ioType, fd, fp, xwd_windowName, UCHAR,
			1, i ) == -1 )
		{
			ImReturnBinError( );
		}
		TagTableAppend( tagTable, TagEntryAlloc( "image name",
			POINTER, &xwd_windowName ) );
	}


	/*
	 *  Check for unsupported variations.
	 */
	if ( header.xwd_pixmapFormat != IMXWDZPIXMAP )
	{
		ImErrorFatal( "Only XWD file Z format pixmaps are supported",
			-1, IMEUNSUPPORTED );
	}
#ifdef bogus
	if ( header.xwd_pixmapDepth != header.xwd_bitsPerPixel )
	{
		ImErrorFatal( "XWD file image depth not the same as bits per pixel?",
			-1, IMEUNSUPPORTED );
	}
#endif
	if ( header.xwd_pixmapDepth != 8 && header.xwd_pixmapDepth != 24 )
	{
		ImErrorFatal( "Only 8-bit and 24-bit XWD image depths supported",
			-1, IMEUNSUPPORTED );
	}
	if ( header.xwd_bitmapUnit != 8 && header.xwd_bitmapUnit != 16 &&
		header.xwd_bitmapUnit != 32 )
	{
		ImErrorFatal( "XWD bitmap units must be 8-, 16-, or 32-bits wide",
			-1, IMEUNSUPPORTED );
	}
	if ( (header.xwd_bitmapPad % 8) != 0 )
	{
		ImErrorFatal( "XWD bitmap pad must be a multiple of 8 bits",
			-1, IMEUNSUPPORTED );
	}
	if ( header.xwd_pixmapDepth == 24 &&
		(header.xwd_bitmapPad != 32 || header.xwd_bitmapUnit != 32 ||
		header.xwd_bitsPerPixel != 32 ) )
	{
		ImErrorFatal( "XWD 24-bit images must be stored in 32-bit units",
			-1, IMEUNSUPPORTED );
	}

	/*
	 * Output -verbose file info
	 */
	if( xwd_windowName != NULL )
		ImInfo( "Window Name", xwd_windowName );

	sprintf( message, "%d", version );
	ImInfo( "Version", message );

	if( header.xwd_byteOrder == IMXWDMBF )
	{
		ImInfo( "Byte Order", "Most Significant Byte First" );
	}
	else
	{
		ImInfo( "Byte Order", "Least Significant Byte First" );
	}

	sprintf( message, "%d x %d", header.xwd_pixmapWidth,
		header.xwd_pixmapHeight );
	ImInfo( "Resolution", message );

	switch( header.xwd_visual )
	{
		case IMXWDSTATICGREY:
			sprintf( message, "%d-bit Grayscale (Static Grey)",
				header.xwd_pixmapDepth );
			break;
		case IMXWDGREYSCALE:
			sprintf( message, "%d-bit Grayscale (Grey Scale)",
				header.xwd_pixmapDepth );
			break;
		case IMXWDSTATICCOLOR:
			sprintf( message, "%d-bit Color Indexed (Static Color)",
				header.xwd_pixmapDepth );
			break;
		case IMXWDPSEUDOCOLOR:
			sprintf( message, "%d-bit Color Indexed (Pseudo Color)",
				header.xwd_pixmapDepth );
			break;
		case IMXWDTRUECOLOR:
			sprintf( message, "%d-bit RGB (True Color)",
				header.xwd_pixmapDepth );
			break;
		case IMXWDDIRECTCOLOR:
			sprintf( message, "%d-bit RGB (Direct Color)",
				header.xwd_pixmapDepth );
			break;
		default:
			strcpy( message, "unknown" );
			break;
	}
	ImInfo( "Type", message );

	/*
	 * Read in the CLT, if any.
	 */
	clt = IMCLTNULL;
	if ( header.xwd_colormapEntries != 0 && header.xwd_nColors != 0 )
	{
		if ( (clt = ImCltAlloc( header.xwd_colormapEntries )) ==
			IMCLTNULL )
		{
			ImErrorFatal( ImQError( ), -1, ImErrNo );
		}

		/*
		 * Output -verbose info
		 */
		sprintf( message, "%d Entries",
			header.xwd_colormapEntries );
		ImInfo( "Color Table", message );

		/* Initialize the CLT to black entries.			*/
		pColor = ImCltQFirst( clt );
		for ( i = 0; i < header.xwd_colormapEntries; i++ )
		{
			ImCltSRed(   pColor, 0 );
			ImCltSGreen( pColor, 0 );
			ImCltSBlue(  pColor, 0 );
			ImCltSInc(   clt, pColor );
		}

		if ( header.xwd_visual == IMXWDDIRECTCOLOR )
			mask = ~((~0) << header.xwd_bitsPerRGB);
		else
			mask = ~0;	/* Any pixel index	*/

		/*
		 *  Read in the CLT entries.  Scale color components down
		 *  to 8-bit values.
		 */
		for ( i = 0; i < header.xwd_nColors; i++ )
		{
			if ( ImBinReadStruct( ioType, fd, fp, &color,
				imXwdColorFields ) == -1 )
			{
				ImReturnBinError( );
			}

			/*
			 * Check to see if xwd_flags is zero.  If
			 * yes, assume that all the primary color
			 * components, RGB, should be used.  This
			 * is to compensate for the xwud code in X11R5
			 * which incorrectly ignores the flags field.
			 *
			 * Remove this conditional if xwud.c is fixed
			 * to correctly use the flags field.
			 *
			 * (see xwud.c, Do_Pseudo(), in X11R5 distribution)
			 */
			if( color.xwd_flags == 0 )
			{
				color.xwd_flags = IMXWDDORED|IMXWDDOGREEN|
						  IMXWDDOBLUE;
			}

			/*
			 * if color index given in file is out of range
			 * of the colortable, just make it the ith
			 * one.
			 */
			if( color.xwd_pixel > header.xwd_nColors )
			{
				pColor = ImCltQPtr( clt, i & mask);
			}
			else
			{
				pColor = ImCltQPtr( clt,
					 (int) color.xwd_pixel & mask );
			}
			if ( color.xwd_flags & IMXWDDORED )
				ImCltSRed(   pColor,
					(color.xwd_red   *255/65535));
			if ( color.xwd_flags & IMXWDDOGREEN )
				ImCltSGreen( pColor,
					(color.xwd_green *255/65535));
			if ( color.xwd_flags & IMXWDDOBLUE )
				ImCltSBlue(  pColor,
					(color.xwd_blue  *255/65535));
		}
	}
	else	/* no colortable */
	{
		/*
		 * Output -verbose info
		 */
		ImInfo( "Color Table", "none" );
	}

	/*
	 * Use byteOrder field in header to set Bin package byte
	 * order for image data.
	 */
	if ( header.xwd_byteOrder == IMXWDMBF )
	{
		BinByteOrder( BINMBF );
	}
	else if ( header.xwd_byteOrder == IMXWDLBF )
	{
		BinByteOrder( BINLBF );
	}
	else
	{
		sprintf( message, "Unknown XWD file byte order:  '%d'",
			header.xwd_byteOrder );
		ImErrorFatal( message, -1, IMEUNSUPPORTED );
	}

	/*
	 *  Read in the image.
	 */
	if ( header.xwd_pixmapDepth == 8 )
	{
		/*
		 *  8-bit index image.
		 *
		 *  Image may be stored in units of 8, 16 and 32 bit quantities,
		 *  with padding.
		 */
		if ( (vfb = ImVfbAlloc( header.xwd_pixmapWidth,
			header.xwd_pixmapHeight, IMVFBINDEX8 )) == IMVFBNULL )
			ImErrorFatal( ImQError( ), -1, ImErrNo );

		ImVfbSClt( vfb, clt );	/* CLT could be NULL		*/

		switch ( header.xwd_bitmapUnit )
		{
		case 8:
			if ( imXwd8Read8( ioType, fd, fp, &header, &vfb)== -1)
			{
				ImVfbFree( vfb );
				return ( -1 );	/* Error already handled */
			}
			break;

		case 16:
			if ( imXwd8Read16( ioType, fd, fp, &header,&vfb)== -1)
			{
				ImVfbFree( vfb );
				return ( -1 );	/* Error already handled */
			}
			break;

		case 32:
			if ( imXwd8Read32( ioType, fd, fp, &header,&vfb)== -1)
			{
				ImVfbFree( vfb );
				return ( -1 );	/* Error already handled */
			}
			break;
		}

		TagTableAppend( tagTable,
			TagEntryAlloc( "image clt", POINTER, &clt ) );
		TagTableAppend( tagTable,
			TagEntryAlloc( "image vfb", POINTER, &vfb ) );

		if ( ImVfbQClt( vfb ) != IMCLTNULL )
			return ( 2 );
		return ( 1 );
	}


	/*
	 *  RGB image.
	 *
	 *  Image must be stored in 32-bit quantities, with padding.
	 */
	if ( (vfb = ImVfbAlloc( header.xwd_pixmapWidth,
		header.xwd_pixmapHeight, IMVFBRGB )) == IMVFBNULL )
		ImErrorFatal( ImQError( ), -1, ImErrNo );

	if ( imXwd24Read32( ioType, fd, fp, &header, &vfb, clt ) == -1 )
	{
		if ( clt != IMCLTNULL )
			ImCltFree( clt );
		ImVfbFree( vfb );
		return ( -1 );		/* Error already handled	*/
	}

	/*
	 *  Add the VFB to the tagTable.
	 */
	TagTableAppend( tagTable, TagEntryAlloc( "image vfb", POINTER, &vfb ) );

#ifdef notdef
	if ( clt != IMCLTNULL )
		ImCltFree( clt );
#endif
	return ( 1 );
}





/*
 *  FUNCTION
 *	imXwd8Read8	-  read 8-bit image in 8-bit bitmapUnit
 *	imXwd8Read16	-  read 8-bit image in 16-bit bitmapUnit
 *	imXwd8Read32	-  read 8-bit image in 32-bit bitmapUnit
 *	imXwd24Read32	-  read 24-bit image in 32-bit bitmapUnit
 *	
 *  DESCRIPTION
 *	8-bit pixel data is read in where:
 *
 *		bitmapUnit	= 8 bits
 *		byteOrder	= irrelevant
 *		bitmapBitOrder	= MBF (does any machine use LBF?)
 *		bitmapPad	= 8, 16, or 32
 *		bitsPerPixel	= 8
 *		bytesPerLine	= whatever
 */

static int				/* Returns status		*/
#ifdef __STDC__
imXwd8Read8( int ioType, int fd, FILE *fp, imXwdHeaderInfo *header, ImVfb **vfb )
#else
imXwd8Read8( ioType, fd, fp, header, vfb )
	int               ioType;	/* I/O flags			*/
	int               fd;		/* Input file descriptor	*/
	FILE	         *fp;		/* Input file pointer		*/
	imXwdHeaderInfo  *header;	/* XWD file header		*/
	ImVfb  **vfb;		/* Read in image		*/
#endif
{
	ImVfbPtr pPixel;	/* Pixel pointer		*/
	int	          y;		/* Scanline counter		*/
	int               pad;		/* # of bytes into next padding	*/
	int               nBytes;	/* # of bytes per scanline	*/
	unsigned char            *buffer;	/* Input buffer			*/
	unsigned char   *pBufferEnd;	/* End of buffer		*/
	unsigned char   *pBuffer;	/* Buffer pointer		*/


	/* Get space enough for one scanline.				*/
	nBytes = header->xwd_pixmapWidth;
	if ( header->xwd_bytesPerLine == 0 )
	{
		pad = nBytes % (header->xwd_bitmapPad/8);
		if ( pad != 0 )
			header->xwd_bytesPerLine = nBytes +
				header->xwd_bitmapPad / 8 - pad;
		else
			header->xwd_bytesPerLine = nBytes;
	}
	ImMalloc( buffer, unsigned char *, header->xwd_bytesPerLine );


	/* Read in each scanline and copy it into the VFB.		*/
	pPixel     = ImVfbQFirst( *vfb );
	pBufferEnd = buffer + nBytes;

	for ( y = 0; y < header->xwd_pixmapHeight; y++ )
	{
		pBuffer = buffer;
		if ( ImBinRead( ioType, fd, fp, buffer, UCHAR, 1, header->xwd_bytesPerLine ) == -1 )
		{
			free( (char *)buffer );
			ImReturnBinError( );
		}

		while ( pBuffer != pBufferEnd )
		{
			ImVfbSIndex8( *vfb, pPixel, *pBuffer++ );
			ImVfbSInc( *vfb, pPixel );
		}
	}
	free( (char *)buffer );

	return ( 0 );
}

static int				/* Returns status		*/
#ifdef __STDC__
imXwd8Read16( int ioType, int fd, FILE *fp, imXwdHeaderInfo *header, ImVfb **vfb )
#else
imXwd8Read16( ioType, fd, fp, header, vfb )
	int                ioType;	/* I/O flags			*/
	int                fd;		/* Input file descriptor	*/
	FILE	          *fp;		/* Input file pointer		*/
	imXwdHeaderInfo   *header;	/* XWD file header		*/
	ImVfb  **vfb;		/* Read in image		*/
#endif
{
	ImVfbPtr pPixel;	/* Pixel pointer		*/
	int	          y;		/* Scanline counter		*/
	int               pad;		/* # of bytes into next padding	*/
	int               nBytes;	/* # of bytes per scanline	*/
	sdsc_uint16           *buffer;	/* Input buffer			*/
	sdsc_uint16  *pBufferEnd;	/* End of buffer		*/
	sdsc_uint16  *pBuffer;	/* Buffer pointer		*/


	/*
	 *  Read 8-bit image stored in 16-bit units.
	 */
	nBytes = header->xwd_pixmapWidth;
	pad    = nBytes % (header->xwd_bitmapPad/8);
	if ( pad != 0 )
		nBytes += header->xwd_bitmapPad / 8 - pad;
	ImMalloc( buffer, sdsc_uint16 *, nBytes );
	nBytes /= 2;

	pPixel = ImVfbQFirst( *vfb );

	/* Compute end.  Don't include last partially filled unit.	*/
	pBufferEnd = buffer + (header->xwd_pixmapWidth/2);


	/*
	 *  Read in each scanline and copy it into VFB pixels.  For speed,
	 *  this is split into two slightly different loops, one for LBF
	 *  and one for MBF data.  In both cases each 16-bit quantity is read
	 *  into a host short (which may or may not be 16 bits long).  The
	 *  16-bit value is then split into two pixels by shifting and masking.
	 *  The difference between these two loops is only in the order of
	 *  the pixels in the 16-bit value and the shifting and masking needed.
	 */
	if ( header->xwd_bitmapBitOrder == IMXWDLBF )
	{
		/* 1st pixel is at the bottom of the sdsc_uint16.		*/
		for ( y = 0; y < header->xwd_pixmapHeight; y++ )
		{
			pBuffer = buffer;
			if ( ImBinRead( ioType, fd, fp, buffer, UINT16, 2,
				nBytes ) == -1 )
			{
				free( (char *)buffer );
				ImReturnBinError( );
			}

			while ( pBuffer != pBufferEnd )
			{
				ImVfbSIndex8( *vfb, pPixel, *pBuffer&0xFF );
				ImVfbSInc( *vfb, pPixel );

				ImVfbSIndex8( *vfb, pPixel, (*pBuffer++>>8)&0xFF );
				ImVfbSInc( *vfb, pPixel );
			}

			if ( pad != 0 )
			{
				ImVfbSIndex8( *vfb, pPixel, *pBuffer&0xFF );
				ImVfbSInc( *vfb, pPixel );
			}
		}
		free( (char *)buffer );
		return ( 0 );
	}

	/* 1st pixel is at the top of the sdsc_uint16.			*/
	for ( y = 0; y < header->xwd_pixmapHeight; y++ )
	{
		pBuffer = buffer;
		if ( ImBinRead( ioType, fd, fp, buffer, UINT16, 2,
			nBytes ) == -1 )
		{
			free( (char *)buffer );
			ImReturnBinError( );
		}

		while ( pBuffer != pBufferEnd )
		{
			ImVfbSIndex8( *vfb, pPixel, (*pBuffer>>8)&0xFF );
			ImVfbSInc( *vfb, pPixel );

			ImVfbSIndex8( *vfb, pPixel, *pBuffer++&0xFF );
			ImVfbSInc( *vfb, pPixel );
		}

		if ( pad != 0 )
		{
			ImVfbSIndex8( *vfb, pPixel, (*pBuffer>>8)&0xFF );
			ImVfbSInc( *vfb, pPixel );
		}
	}
	free( (char *)buffer );
	return ( 0 );
}

static int				/* Returns status		*/
#ifdef __STDC__
imXwd8Read32( int ioType, int fd, FILE *fp, imXwdHeaderInfo *header, ImVfb **vfb )
#else
imXwd8Read32( ioType, fd, fp, header, vfb )
	int                ioType;	/* I/O flags			*/
	int                fd;		/* Input file descriptor	*/
	FILE	          *fp;		/* Input file pointer		*/
	imXwdHeaderInfo   *header;	/* XWD file header		*/
	ImVfb   **vfb;		/* Read in image		*/
#endif
{
	ImVfbPtr pPixel;	/* Pixel pointer		*/
	int	          y;		/* Scanline counter		*/
	int               pad;		/* # of bytes into next padding	*/
	int               nBytes;	/* # of bytes per scanline	*/
	sdsc_uint32           *buffer;	/* Input buffer			*/
	sdsc_uint32  *pBufferEnd;	/* End of buffer		*/
	sdsc_uint32  *pBuffer;	/* Buffer pointer		*/
	int               i;		/* Counter			*/


	/*
	 *  Read 8-bit image stored in 32-bit units.
	 */
	nBytes = header->xwd_pixmapWidth;
	pad    = nBytes % (header->xwd_bitmapPad/8);
	if ( pad != 0 )
		nBytes += header->xwd_bitmapPad / 8 - pad;
	ImMalloc( buffer, sdsc_uint32 *, nBytes );
	nBytes /= 4;

	pPixel = ImVfbQFirst( *vfb );

	/* Compute end.  Don't include last partially filled unit.	*/
	pBufferEnd = buffer + (header->xwd_pixmapWidth/4);

	/*
	 *  Read in each scanline and copy it into VFB pixels.  For speed,
	 *  this is split into two slightly different loops, one for LBF
	 *  and one for MBF data.  In both cases each 32-bit quantity is read
	 *  into a host int (which may or may not be 32 bits long).  The
	 *  32-bit value is then split into four pixels by shifting and masking.
	 *  The difference between these two loops is only in the order of
	 *  the pixels in the 32-bit value and the shifting and masking needed.
	 */
	if ( header->xwd_bitmapBitOrder == IMXWDLBF )
	{
		/* 1st pixel is at bottom of sdsc_uint32.			*/
		for ( y = 0; y < header->xwd_pixmapHeight; y++ )
		{
			pBuffer = buffer;
			if ( ImBinRead( ioType, fd, fp, buffer, UINT32, 4,
				nBytes ) == -1 )
			{
				free( (char *)buffer );
				ImReturnBinError( );
			}

			while ( pBuffer != pBufferEnd )
			{
				ImVfbSIndex8( *vfb, pPixel, *pBuffer&0xFF );
				ImVfbSInc( *vfb, pPixel );

				ImVfbSIndex8( *vfb, pPixel, (*pBuffer>>8)&0xFF );
				ImVfbSInc( *vfb, pPixel );

				ImVfbSIndex8( *vfb, pPixel, (*pBuffer>>16)&0xFF );
				ImVfbSInc( *vfb, pPixel );

				ImVfbSIndex8( *vfb, pPixel, (*pBuffer>>24)&0xFF );
				ImVfbSInc( *vfb, pPixel );

				pBuffer++;
			}

			for ( i = 0; i < pad; i++ )
			{
				ImVfbSIndex8( *vfb, pPixel, (*pBuffer>>(i*8))&0xFF );
				ImVfbSInc( *vfb, pPixel );
			}
		}
		free( (char *)buffer );
		return ( 0 );
	}

	/* 1st pixel is at top of sdsc_uint32.				*/
	for ( y = 0; y < header->xwd_pixmapHeight; y++ )
	{
		pBuffer = buffer;
		if ( ImBinRead( ioType, fd, fp, buffer, UINT32, 4,
			nBytes ) == -1 )
		{
			free( (char *)buffer );
			ImReturnBinError( );
		}

		while ( pBuffer != pBufferEnd )
		{
			ImVfbSIndex8( *vfb, pPixel, (*pBuffer>>24)&0xFF );
			ImVfbSInc( *vfb, pPixel );

			ImVfbSIndex8( *vfb, pPixel, (*pBuffer>>16)&0xFF );
			ImVfbSInc( *vfb, pPixel );

			ImVfbSIndex8( *vfb, pPixel, (*pBuffer>>8)&0xFF );
			ImVfbSInc( *vfb, pPixel );

			ImVfbSIndex8( *vfb, pPixel, *pBuffer&0xFF );
			ImVfbSInc( *vfb, pPixel );

			pBuffer++;
		}

		for ( i = 0; i < pad; i++ )
		{
			ImVfbSIndex8( *vfb, pPixel, (*pBuffer>>(24-i*8))&0xFF );
			ImVfbSInc( *vfb, pPixel );
		}
	}
	free( (char *)buffer );
	return ( 0 );
}


static int				/* Returns status		*/
#ifdef __STDC__
imXwd24Read32( int ioType, int fd, FILE *fp, imXwdHeaderInfo *header, ImVfb **vfb, ImClt *clt )
#else
imXwd24Read32( ioType, fd, fp, header, vfb, clt )
	int                ioType;	/* I/O flags			*/
	int                fd;		/* Input file descriptor	*/
	FILE	          *fp;		/* Input file pointer		*/
	imXwdHeaderInfo   *header;	/* XWD file header		*/
	ImVfb   **vfb;		/* Read in image		*/
	ImClt		  *clt;		/* CLT to apply			*/
#endif
{
	ImVfbPtr pPixel;	/* Pixel pointer		*/
	int	          y;		/* Scanline counter		*/
	int               nPix;		/* # of pixels per scanline	*/
	sdsc_uint32           *buffer;	/* Input buffer			*/
	sdsc_uint32  *pBufferEnd;	/* End of buffer		*/
	sdsc_uint32  *pBuffer;	/* Buffer pointer		*/
	sdsc_uint32	  	  redMask;	/* Mask to get red value	*/
	sdsc_uint32	  	  greenMask;	/* Mask to get green value	*/
	sdsc_uint32	  	  blueMask;	/* Mask to get blue value	*/
	int		  redShift;	/* Shift to put red into 8-bits	*/
	int		  greenShift;	/* Shift to put green into 8-bits*/
	int		  blueShift;	/* Shift to put blue into 8-bits*/
	sdsc_uint32		  tmp;		/* Temp value holder		*/
	ImCltPtr	  pColor;	/* CLT entry pointer		*/
	int		  index;	/* CLT index			*/


	/*
	 *  Read 24-bit image stored in 32-bit units.
	 */
	nPix = header->xwd_pixmapWidth;
	ImMalloc( buffer, sdsc_uint32 *, nPix * sizeof( sdsc_uint32 ) );
	pPixel = ImVfbQFirst( *vfb );
	pBufferEnd = buffer + header->xwd_pixmapWidth;


	/*
	 *  Compute shift values.
	 */
	tmp = redMask = header->xwd_redMask;
	for ( redShift = 0; (tmp&0x1) == 0; redShift++, tmp >>= 1 )
		;
	tmp = greenMask = header->xwd_greenMask;
	for ( greenShift = 0; (tmp&0x1) == 0; greenShift++, tmp >>= 1 )
		;
	tmp = blueMask = header->xwd_blueMask;
	for ( blueShift = 0; (tmp&0x1) == 0; blueShift++, tmp >>= 1 )
		;


	/*
	 *  Read in each scanline.  For each 32-bit quantity, pull out the
	 *  red, green, and blue components as indicated by the header's
	 *  masks.
	 */
	for ( y = 0; y < header->xwd_pixmapHeight; y++ )
	{
		pBuffer = buffer;
		if ( ImBinRead( ioType, fd, fp, buffer, UINT32, 4, nPix)== -1)
		{
			free( (char *)buffer );
			ImReturnBinError( );
		}

		while ( pBuffer != pBufferEnd )
		{
			tmp = *pBuffer++;
			if ( clt == IMCLTNULL )
			{
				ImVfbSRed(   *vfb, pPixel,
					(tmp&redMask)>>redShift);
				ImVfbSGreen( *vfb, pPixel,
					(tmp&greenMask)>>greenShift);
				ImVfbSBlue(  *vfb, pPixel,
					(tmp&blueMask)>>blueShift);
			}
			else
			{
				index  = (tmp&redMask)>>redShift;
				pColor = ImCltQPtr( clt, index );
				ImVfbSRed(   *vfb, pPixel, ImCltQRed( pColor ));

				index  = (tmp&greenMask)>>greenShift;
				pColor = ImCltQPtr( clt, index );
				ImVfbSGreen( *vfb, pPixel, ImCltQGreen(pColor));

				index  = (tmp&blueMask)>>blueShift;
				pColor = ImCltQPtr( clt, index );
				ImVfbSBlue(  *vfb, pPixel, ImCltQBlue( pColor));
			}
			ImVfbSInc( *vfb, pPixel );
		}
	}
	free( (char *)buffer );
	return ( 0 );
}





/*
 *  FUNCTION
 *	imXwdWriteHeader	-  write an XWD file header
 *
 *  DESCRIPTION
 *	The XWD file header is written out for 8-bit or 24-bit images.
 *	The header includes the image's name, if any.
 */

static int				/* Returns status		*/
#ifdef __STDC__
imXwdWriteHeader( int ioType, int fd, FILE *fp, ImVfb *vfb, char *name, int writeClt )
#else
imXwdWriteHeader( ioType, fd, fp, vfb, name, writeClt )
	int          ioType;		/* I/O flags			*/
	int          fd;		/* Input file descriptor	*/
	FILE	    *fp;		/* Input file pointer		*/
	ImVfb       *vfb;		/* Read in image		*/
	char	    *name;		/* Window name			*/
	int      writeClt;		/* Include CLT in XWD file?	*/
#endif
{
	ImClt       *clt;		/* VFB's CLT			*/
	ImCltPtr     pColor;		/* CLT color pointer		*/
	imXwdHeaderInfo header;		/* XWD file header		*/
	imXwdColor   color;		/* XWD color holder		*/
	unsigned long        headerSize;	/* Size of header, in bytes	*/
	unsigned long        version;		/* Header version number	*/
	int          n;			/* Counter			*/
	char	     message[100];	/* ImInfo message		*/


	/*
	 *  Set up the header and write it out.  Include a default window
	 *  name after the header.
	 */
	headerSize = IMXWDHEADERSIZE + strlen( name ) + 1;
	version    = IMXWDVERSION;

	BinByteOrder( BINMBF );
	if ( ImBinWrite( ioType, fd, fp, &headerSize, ULONG, 4, 1 ) == -1 )
	{
		ImReturnBinError( );
	}
	if ( ImBinWrite( ioType, fd, fp, &version, ULONG, 4, 1 ) == -1 )
	{
		ImReturnBinError( );
	}

	header.xwd_pixmapFormat   = IMXWDZPIXMAP;
	if ( ImVfbQFields( vfb ) & IMVFBINDEX8 )
	{
		/* 8-bit image.						*/
		header.xwd_pixmapDepth    = 8;
		header.xwd_bitmapUnit     = 8;
		header.xwd_redMask        = 0;
		header.xwd_greenMask      = 0;
		header.xwd_blueMask       = 0;
		header.xwd_visual         = IMXWDPSEUDOCOLOR;
	}
	else
	{
		/* RGB image.						*/
		header.xwd_pixmapDepth    = 24;
		header.xwd_bitmapUnit     = 32;
		header.xwd_redMask        = 0xFF0000;
		header.xwd_greenMask      = 0x00FF00;
		header.xwd_blueMask       = 0x0000FF;
		header.xwd_visual         = IMXWDTRUECOLOR;
	}
	header.xwd_pixmapWidth    = ImVfbQWidth( vfb );
	header.xwd_pixmapHeight   = ImVfbQHeight( vfb );
	header.xwd_xOffset        = 0;
	header.xwd_byteOrder      = IMXWDMBF;	/* Always MBF from us	*/
	header.xwd_bitmapBitOrder = IMXWDMBF;
	header.xwd_bitmapPad      = header.xwd_bitmapUnit;
	header.xwd_bitsPerPixel   = header.xwd_bitmapUnit;
	header.xwd_bytesPerLine   = header.xwd_pixmapWidth * header.xwd_bitsPerPixel / 8;
	header.xwd_bitsPerRGB     = 8;		/* Always 8		*/
	if ( writeClt )
	{
		clt = ImVfbQClt( vfb );
		header.xwd_colormapEntries = 256;
		header.xwd_nColors         = ImCltQNColors( clt );
	}
	else
	{
		header.xwd_colormapEntries = 0;
		header.xwd_nColors         = 0;
	}
	header.xwd_windowWidth    = header.xwd_pixmapWidth;
	header.xwd_windowHeight   = header.xwd_pixmapHeight;
	header.xwd_windowX        = 0;
	header.xwd_windowY        = 0;
	header.xwd_windowBorderWidth = 0;
#ifndef no_headerEnd
	header.xwd_headerEnd      = 0;
#endif

	if ( ImBinWriteStruct( ioType, fd, fp, &header, imXwdHeaderFields )==-1)
	{
		ImReturnBinError( );
	}
	if ( ImBinWrite( ioType, fd, fp, name, UCHAR, 1,
		strlen( name ) + 1 ) == -1 )
	{
		ImReturnBinError( );
	}


	/*
	 *  Write out the CLT, if any.
	 */
	if ( writeClt )
	{
		pColor = ImCltQFirst( clt );
		color.xwd_flags = IMXWDDORED | IMXWDDOGREEN | IMXWDDOBLUE;
		color.xwd_pad   = 0;
		for ( n = 0; n < ImCltQNColors( clt ); n++ )
		{
			color.xwd_pixel = n;
			color.xwd_red   = ImCltQRed(   pColor ) *65535/255;
			color.xwd_green = ImCltQGreen( pColor ) *65535/255;
			color.xwd_blue  = ImCltQBlue(  pColor ) *65535/255;
			ImCltSInc( clt, pColor );
			if ( ImBinWriteStruct( ioType, fd, fp, &color,
				imXwdColorFields ) == -1 )
			{
				ImReturnBinError( );
			}
		}
	}

	/*
	 * Output -verbose file info
	 */
	if( name != NULL )
		ImInfo( "Window Name", name );

	sprintf( message, "%d", version );
	ImInfo( "Version", message );

	if( header.xwd_byteOrder == IMXWDMBF )
	{
		ImInfo( "Byte Order", "Most Significant Byte First" );
	}
	else
	{
		ImInfo( "Byte Order", "Least Significant Byte First" );
	}

	sprintf( message, "%d x %d", header.xwd_pixmapWidth,
		 header.xwd_pixmapHeight );
	ImInfo( "Resolution", message );

	switch( header.xwd_visual )
	{
		case IMXWDSTATICGREY:
			sprintf( message, "%d-bit Grayscale (Static Grey)",
				header.xwd_pixmapDepth );
			break;
		case IMXWDGREYSCALE:
			sprintf( message, "%d-bit Grayscale (Grey Scale)",
				header.xwd_pixmapDepth );
			break;
		case IMXWDSTATICCOLOR:
			sprintf( message, "%d-bit Color Indexed (Static Color)",
				header.xwd_pixmapDepth );
			break;
		case IMXWDPSEUDOCOLOR:
			sprintf( message, "%d-bit Color Indexed (Pseudo Color)",
				header.xwd_pixmapDepth );
			break;
		case IMXWDTRUECOLOR:
			sprintf( message, "%d-bit RGB (True Color)",
				header.xwd_pixmapDepth );
			break;
		case IMXWDDIRECTCOLOR:
			sprintf( message, "%d-bit RGB (Direct Color)",
				header.xwd_pixmapDepth );
			break;
		default:
			strcpy( message, "unknown" );
			break;
	}
	ImInfo( "Type", message );

	if( header.xwd_colormapEntries )
	{
		sprintf( message, "%d Entries",
			header.xwd_colormapEntries );
		ImInfo( "Color Table", message );
	}
	else
		ImInfo( "Color Table", "none" );

	return ( 0 );
}





/*
 *  FUNCTION
 *	imXwdWrite8	-  write an 8-bit X XWD file
 *
 *  DESCRIPTION
 *	The XWD file header set up and written out, followed by the CLT
 *	(if any).  The VFB is then written into the file.
 */

static int				/* Returns # of tags used	*/
#ifdef __STDC__
imXwdWrite8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )
#else
imXwdWrite8( pMap, ioType, fd, fp, flagsTable, tagTable )
	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/
	int          ioType;		/* I/O flags			*/
	int          fd;		/* Input file descriptor	*/
	FILE	    *fp;		/* Input file pointer		*/
	TagTable    *flagsTable;	/* Flags			*/
	TagTable    *tagTable;		/* Tag table to read from	*/
#endif
{
	ImVfb       *vfb;		/* Read in image		*/
	ImVfbPtr     pPixel;		/* Pixel pointer		*/
	unsigned char       *buffer;		/* Output buffer		*/
	unsigned char       *pBuffer;		/* Buffer pointer		*/
	char	    *name;		/* Window name			*/
	int          x, y;		/* Pixel counters		*/
	int	     width, height;	/* Image size			*/


	/*
	 *  Get the image, and its name, and write out the XWD file header.
	 */
	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );
	if ( TagTableQNEntry( tagTable, "image name" ) != 0 )
		TagEntryQValue( TagTableQDirect( tagTable, "image name", 0 ),
			&name );
	else
		name = IMXWDNONAME;

	if ( pMap->map_outAttributes & IMCLTYES )
	{
		if ( imXwdWriteHeader( ioType, fd, fp, vfb, name, TRUE ) == -1 )
			return ( -1 );		/* Error already set	*/
	}
	else
	{
		if ( imXwdWriteHeader( ioType, fd, fp, vfb, name, FALSE )== -1 )
			return ( -1 );		/* Error already set	*/
	}


	/*
	 *  Copy the VFB to the file, one scanline at a time.
	 */
	pPixel = ImVfbQFirst( vfb );
	width  = ImVfbQWidth( vfb );
	height = ImVfbQHeight( vfb );
	ImMalloc( buffer, unsigned char *, width );
	for ( y = 0; y < height; y++ )
	{
		pBuffer = buffer;
		for ( x = 0; x < width; x++ )
		{
			*pBuffer++ = ImVfbQIndex8( vfb, pPixel );
			ImVfbSInc( vfb, pPixel );
		}

		/*
		 *  Write out the buffer.
		 */
		if ( ImBinWrite( ioType, fd, fp, buffer, UCHAR, 1, width)== -1 )
		{
			free( (char *)buffer );
			ImReturnBinError( );
		}
	}

	free( (char *)buffer );
	return ( 1 );
}





/*
 *  FUNCTION
 *	imXwdWriteRGB	-  write a 24-bit X XWD file
 *
 *  DESCRIPTION
 *	The XWD file header set up and written out, followed by the CLT
 *	(if any).  The VFB is then written into the file.
 */

static int				/* Returns # of tags used	*/
#ifdef __STDC__
imXwdWriteRGB( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, 
	TagTable *flagsTable, TagTable *tagTable )
#else
imXwdWriteRGB( pMap, ioType, fd, fp, flagsTable, tagTable )
	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/
	int          ioType;		/* I/O flags			*/
	int          fd;		/* Input file descriptor	*/
	FILE	    *fp;		/* Input file pointer		*/
	TagTable    *flagsTable;	/* Flags			*/
	TagTable    *tagTable;		/* Tag table to read from	*/
#endif
{
	ImVfb       *vfb;		/* Read in image		*/
	ImVfbPtr     pPixel;		/* Pixel pointer		*/
	sdsc_uint32      *buffer;		/* Output buffer		*/
	sdsc_uint32      *pBuffer;		/* Buffer pointer		*/
	char	    *name;		/* Window name			*/
	int          x, y;		/* Pixel counters		*/
	int	     width, height;	/* Image size			*/


	/*
	 *  Get the image, and its name, and write out the XWD file header.
	 */
	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );
	if ( TagTableQNEntry( tagTable, "image name" ) != 0 )
		TagEntryQValue( TagTableQDirect( tagTable, "image name", 0 ),
			&name );
	else
		name = IMXWDNONAME;

	if ( pMap->map_outAttributes & IMCLTYES )
	{
		if ( imXwdWriteHeader( ioType, fd, fp, vfb, name, TRUE ) == -1 )
			return ( -1 );		/* Error already set	*/
	}
	else
	{
		if ( imXwdWriteHeader( ioType, fd, fp, vfb, name, FALSE )== -1 )
			return ( -1 );		/* Error already set	*/
	}


	/*
	 *  Copy the VFB to the file, one scanline at a time.
	 */
	pPixel = ImVfbQFirst( vfb );
	width  = ImVfbQWidth( vfb );
	height = ImVfbQHeight( vfb );
	ImMalloc( buffer, sdsc_uint32 *, sizeof( sdsc_uint32 ) * width );
	for ( y = 0; y < height; y++ )
	{
		pBuffer = buffer;
		for ( x = 0; x < width; x++ )
		{
			*pBuffer++ = ((sdsc_uint32) ImVfbQRed( vfb, pPixel ) << 16) |
				((sdsc_uint32) ImVfbQGreen( vfb, pPixel ) << 8) |
				(sdsc_uint32) ImVfbQBlue( vfb, pPixel );
			ImVfbSInc( vfb, pPixel );
		}

		/*
		 *  Write out the buffer.
		 */
		if ( ImBinWrite( ioType, fd, fp, buffer, UINT32, 4, width)== -1)
		{
			free( (char *)buffer );
			ImReturnBinError( );
		}
	}

	free( (char *)buffer );
	return ( 1 );
}
