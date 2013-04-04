/**

 **	$Header: /roq/libim/imgif.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imgif.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imgif.c		-  Compuserve Graphics Interchange Format

 **

 **  PROJECT

 **	libim		-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imgif.c contains routines to read and write Compuserve GIF image

 **	format for the image manipulation library. 

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 ** 	none

 **

 **  PRIVATE CONTENTS

 **	imGifHeaderInfo		t  GIF header information

 **	imGifHeaderFields	v  imGifHeaderInfo description for Bin pkg

 **	imGifHeader             v  Gif header holder

 **     imGifImDescInfo         t  Gif image descriptor information

 **     imGifImDescFields       v  imGifImDescInfo description for Bin pkg

 **     imGifImDesc             v  Gif image descriptor holder

 **

 **	IMGIFMAGIC              d  file magic number

 **     IMGIFBITPIXEL           d  mask to  read #bits/pixel in the image

 **     IMGIFGLOBALCM           d  mask to read if a global colormap is present

 **     IMGIFLOCALCM            d  mask to read if a local colormap is present

 **     IMGIFINTERLACED         d  mask to read the pixel display sequence used

 **     IMGIFTERMINATOR         d  termination character of a Gif image file

 **     IMGIFEXTENSION          d  Gif extension block introducer character

 **     IMGIFSEPARATOR          d  image separator character

 **     IMGIFENDBLOCK           d  terminates extension blocks

 **     IMGIFNCOLORRES          d  # bits of color resolution

 **     IMGIFNBITSPIXEL         d  # bits per pixel

 **     IMGIFBACKGROUND         d  index of background color

 **     IMGIFCODESIZE           d  initial code size used for lzw algorithm

 **     IMGIFNOCOLORMAP         d  no color lookup table present

 **

 **	imGifVfbRead1           f  Read a mono Gif image

 **     imGifCltRead            f  Read a color lookup table

 **     imGifCltWrite           f  Write a color lookup table

 **     imGifVfbWrite8          f  Write a 8-index Vfb

 **     imGifVfbWrite1          f  Write a mono Vfb

 **	imGifRead		f  read a Compuserve GIF image

 **	imGifWrite		f  write a Compuserve GIF image 

 **

 **  HISTORY

 **	$Log: /roq/libim/imgif.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.22  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.21  1995/06/15  20:19:28  bduggan

 **	removed use of 'new'

 **	Added some casts

 **

 **	Revision 1.20  1995/05/30  16:57:24  bduggan

 **	Changed comment.

 **

 **	Revision 1.19  1995/05/17  23:44:54  bduggan

 **	Added transparency support

 **

 **	Revision 1.18  1995/04/03  21:24:38  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.17  1995/01/10  23:23:37  bduggan

 **	Made read/write functions static

 **

 **	Revision 1.17  1995/01/10  23:23:37  bduggan

 **	Made read/write functions static

 **

 **	Revision 1.16  94/10/03  11:30:06  nadeau

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

 **	Revision 1.15  92/12/09  18:33:05  nadeau

 **	Corrected misinterpretation of CLT write flags that wrote

 **	grayscale INDEX8 images incorrectly.  Also corrected and

 **	updated various info messages.

 **	

 **	Revision 1.14  92/12/03  01:48:12  nadeau

 **	Corrected info messages.

 **	

 **	Revision 1.13  92/11/23  18:42:14  nadeau

 **	Removed use of IMINFOMSG.

 **	

 **	Revision 1.12  92/11/04  11:49:03  groening

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.11  92/10/12  15:57:13  vle

 **	Fixed typo: Limpel -> Lempel

 **	

 **	Revision 1.10  92/09/29  17:58:50  vle

 **	Added ImInfo messages.

 **	

 **	Revision 1.9  92/09/17  14:42:03  vle

 **	Updated to 89a specification.  Fixed bug in 2-bit image read.

 **	Updated copyright notice.

 **	

 **	Revision 1.8  92/04/03  17:43:14  nadeau

 **	Added extern declarations of functions so that the SGI

 **	compiler and linker don't get confused.

 **	

 **	Revision 1.7  91/10/03  09:03:01  nadeau

 **	Fixed #includes.  Turned off DEBUG.

 **	

 **	Revision 1.6  91/03/13  17:09:44  soraya

 **	fixing bugs

 **	

 **	Revision 1.5  91/03/11  09:16:40  soraya

 **	Optimization, finish of write, and removal of debug

 **	print statements.

 **	

 **	Revision 1.4  91/02/12  11:45:52  nadeau

 **	Removed the tag table checking and VFB conversion now

 **	handled by ImFileRead and ImFileWrite.  Made the error

 **	checking more robust.

 **	

 **	Revision 1.3  91/01/31  08:34:20  soraya

 **	*** empty log message ***

 **	

 **	Revision 1.2  90/12/26  18:50:20  soraya

 **	*** empty log message ***

 **

 **	Revision 1.1  90/12/26  13:24:43  soraya

 **	Initial revision

 **	

 **/



#include "iminternal.h"





/**

 **  FORMAT

 **	gif	-  Compuserve Graphics Interchange Format

 **

 **  AKA

 **	giff

 **

 **  FORMAT REFERENCES

 **	Graphics Interchange Format, version 89a, CompuServe, 1990.

 **

 **	Bit-Mapped Graphics, Steve Rimmer, McGraw-Hill, 1990.

 **

 **	Graphics File Formats, David C. Kay, John R. Levine, McGraw-Hill,

 **	1992.

 **

 **	Supercharged Bitmapped Graphics, Steve Rimmer, McGraw-Hill, 1992.

 **

 **  CODE CREDITS

 **	Custom development, Soraya Gonzales, Intevep S.A., Venzuela, 1990.

 **	Extensions, Vinh Le, San Diego Supercomputer Center, 1992.

 **	Extensions, Dave Nadeau, San Diego Supercomputer Center, 1993.

 **

 **  DESCRIPTION              

 **	CompuServe's GIF (Graphics Interchange Format) is used on on-line

 **	image libraries throughout the world for the storage of medium and

 **	low resolution images of 8-bit depths or less.

 **

 **   Header Block

 **	GIF data streams begin with a header block of the form:

 **

 **		Field		Size

 **		-----		----

 **		Signature	3 bytes

 **		Version		3 bytes

 **

 **	The "Signature" field is always "GIF".  The "Version" field is

 **	one of "87a" or "89a", corresponding to the two outstanding versions

 **	of the GIF format specification.

 **

 **		This GIF translator reads 87a and 89a spec GIF files, but

 **		per recommendations in the GIF spec, always writes the

 **		lowest-common-denominator GIF files at 87a spec levels.

 **

 **   Logical Screen Block

 **	Immediately following the GIF header is a logical screen block of

 **	the form:

 **

 **		Field			Size

 **		-----			----

 **		Logical Screen Width	2 bytes, unsigned integer

 **		Logical Screen Height	2 bytes, unsigned integer

 **		Flags			1 byte

 **		Background Color Index	1 byte

 **		Pixel Aspect Ratio	1 byte

 **

 **	The screen width and height refer to the size of the logical screen

 **	onto which the GIF image is to be displayed on the display device.

 **

 **		This GIF translator ignores the screen width and height.

 **

 **	The "Flags" field is a bitmask with the following bit meanings:

 **

 **		Field			Bits

 **		-----			----

 **		Global Color Table	7

 **		Color Resolution	6,5,4

 **		Sort			3

 **		Size of Global Color Tb	2,1,0

 **

 **	The "Global Color Table" flag indicates if the file has a global

 **	color table.  If so, it'll immediately follow the Logical Screen

 **	Block in the file.  Bit 7 = 0 = no global color table.  Bit 7 = 1 =

 **	there is a global color table.

 **

 **		This GIF translator can read global color tables, but the

 **		writer does not generate them.  The writer uses local color

 **		tables instead.

 **

 **	The "Color Resolution" flag gives the number of bits per primary color,

 **	minus 1.  A value of 7 means 8-bits per primary and is typical.

 **

 **		This GIF translator can read color table resolutions up to

 **		8-bits per channel and converts them internally to ImClt's

 **		with 8-bits per channel.  GIF files written by this

 **		translator are always 8-bits per channel.

 **

 **	The "Sort" flag indicates if the global color table (if there is one)

 **	is sorted from most to least important color.  Bit 3 = 0 = unsorted.

 **	Bit 3 = 1 = sorted.

 **

 **		This GIF translator ignores color table sorting on reading.

 **		On writing, the color table is always unsorted.

 **

 **	The "Global Color Table Size" field is used to calculate the length of

 **	the global color table.  Take 2 and raise it to a power equal to this

 **	field plus 1.  So, a value of 7 for the field gives 2^(7+1) entries,

 **	or 2^8 = 256 entries.  Even when there is no global color table, this

 **	field should be set to the size of a typical color table to help

 **	future display routines.

 **

 **		This GIF translator uses this field on reading to determine

 **		the global color table size, and sets this field on writing

 **		to an appropriate value.

 **

 **	The "Background Color Index" field is an index into the global

 **	color table (if the table is present) and selects the color to use

 **	for background pixels not covered by image data from the GIF file.

 **	If there is no global color table, this field should be 0.

 **

 **		This GIF translator doesn't use the background color on

 **		reading, and sets it to 0 on writing.

 **

 **	The "Pixel Aspect Ratio" field gives the aspect ratio of pixels in

 **	the image and is meant to account for non-square pixels on crude

 **	graphics systems.  A value of 0 means no aspect ratio information is

 **	provided in the field.

 **

 **		This GIF translator ignores the pixel aspect ratio on reading,

 **		and sets it to 0 on writing.

 **

 **   Global Color Table Block

 **	The global color table is a default color table for images in the file

 **	that don't have a local color table associated with them.  This block

 **	is only present if the global color table flag is set in the Logical

 **	Screen block above.

 **

 **	If present, this block will contain a number of bytes equal to:

 **

 **		3 * 2^(table size + 1)

 **

 **	where the "table size" is given in the Logical Screen Block above.

 **

 **	Color table values are in the order RGB, RGB, RGB, one byte per channel,

 **	from color table entry 0 to the maximum.  The maximum number of entries

 **	allowed by the spec is 256.

 **

 **		This GIF translator handles global color tables on reading

 **		image files, but never generates them on writing.  All

 **		written images are provided with local color tables.

 **

 **   Image Descriptor

 **	The image descriptor describes a raster image in the GIF file.  There

 **	will be one image descriptor for each such image, with no upper limit

 **	on the number of images contained within a single GIF file.

 **

 **	Image descriptions begin with an image descriptor block of the form:

 **

 **		Field			Size

 **		-----			----

 **		Separator		1 byte

 **		Left Position		2 bytes, unsigned integer

 **		Top Position		2 bytes, unsigned integer

 **		Width			2 bytes, unsigned integer

 **		Height			2 bytes, unsigned integer

 **		Flags			1 byte

 **

 **	The image separator marks the beginning of the descriptor and helps

 **	to separate images from each other in a multi-image file.  The

 **	separator always contains the hex value "0x2C".

 **

 **	The image left and top position values are relative to the logical

 **	screen, as defined in the logical screen block.  (0,0) is at the

 **	top left of the logical screen.

 **

 **		This GIF translator does not pay attention to logical screens.

 **		So, image left-top positions within such are ignored.  On

 **		writing images, the left and top position values are always

 **		set to 0.

 **

 **	The width and height fields give the image size in pixels.

 **

 **	The "Flags" field is a bitmask with the following bit meanings:

 **

 **		Field			Bits

 **		-----			----

 **		Local Color Table	7

 **		Interlace		6

 **		Sort			5

 **		Reserved		4,3

 **		Size of Local CLT	2,1,0

 **

 **	The "Local Color Table" flag indicates if there is a local CLT

 **	immediately following the image descriptor.  Bit 7 = 1 = there is

 **	a local CLT.  Bit 7 = 0 = there isn't.

 **

 **		This GIF translator handles local color tables with read

 **		images.  Written images always have a local CLT if the

 **		VFB to be written had one.  Grayscale VFB's without CLTs

 **		are written without local CLTs.

 **

 **	The "Interlace" flag indicates if the image is interlaced using a

 **	4-pass interlace pattern.  Interlacing is discussed later on in this

 **	comment block.  Bit 6 = 1 = the image is interlaced.  Bit 6 = 0 =

 **	it isn't interlaced.

 **

 **		This GIF translator handles interlaced incoming images,

 **		but only writes non-interlaced images.

 **

 **	The "Sort" flag indicates if the local color table (if there is one)

 **	is sorted from most to least important color.  Bit 5 = 0 = unsorted.

 **	Bit 5 = 1 = sorted.

 **

 **		This GIF translator ignores color table sorting on reading.

 **		On writing, the color table is always unsorted.

 **

 **	The "Size of Local Color Table" field is used to calculate the length of

 **	the local color table.  Take 2 and raise it to a power equal to this

 **	field plus 1.  So, a value of 7 for the field gives 2^(7+1) entries,

 **	or 2^8 = 256 entries.  If there is no local CLT, this field should be

 **	set to 0.

 **

 **		This GIF translator uses this field on reading to determine

 **		the local color table size, and sets this field on writing

 **		to an appropriate value.

 **

 **   Local Color Table Block

 **	If an image has a local color table (see the "Flags" field of the

 **	image descriptor block), then a description of that table immediate

 **	follows the image descriptor for the image.  This local CLT overrides

 **	any global CLT for this image only.

 **

 **	If present, this block will contain a number of bytes equal to:

 **

 **		3 * 2^(table size + 1)

 **

 **	where the "table size" is given in the Image Descriptor Block above.

 **

 **	Color table values are in the order RGB, RGB, RGB, one byte per channel,

 **	from color table entry 0 to the maximum.  The maximum number of entries

 **	allowed by the spec is 256.

 **

 **		This GIF translator reads local CLTs as needed.  On writing

 **		images, local CLTs are generated for VFBs with CLTs.

 **		Grayscale VFBs without CLTs are written without a local CLT.

 **

 **   Image Data Block

 **	The image data for an image follows immediately after the local CLT

 **	(if any) following the image descriptor for the image.  Image data is

 **	always compressed using a Limpel-Ziv & Welch (LZW) compression scheme

 **	described below.

 **

 **   GIF Trailer

 **	The GIF data stream is terminated with a GIF trailer block of the

 **	form:

 **

 **		Field			Size

 **		-----			----

 **		GIF Trailer		1 byte

 **

 **	The "GIF Trailer" byte always has the hex value "0x3B".

 **

 **   Extensions

 **	Various extensions are defined by the GIF spec.  These extensions

 **	allow the embedding of various display and application-specific

 **	controls.  All extensions begin with the following three fields:

 **

 **		Field			Size

 **		-----			----

 **		Extension Introducer	1 byte

 **		Extension Label		1 byte

 **		Extension Block Size	1 byte

 **

 **	The "Extension Introducer" is a flag indicating the start of an

 **	extension block and always has the hex value "0x21".

 **

 **	The "Extension Label" indicates the type of extension.  The 1989 spec

 **	defines the following extension labels:

 **

 **		Extension Label		Meaning

 **		---------------		-------

 **		0xF9			Graphic Control Extension

 **		0xFE			Comment Extension

 **		0x01			Plain Text Extension

 **		0xFF			Application Extension

 **

 **	The "Extension Block Size" field gives the size, in bytes, of the

 **	extension block.

 **

 **		This GIF translator ignores all extensions on reading GIF

 **		files, and generates no extensions when writing them.

 **

 **   Image Data Interlacing

 **	The "Interlace" flag in the "Flags" field of an Image Descriptor

 **	Block indicates whether the following image data is interlaced or

 **	not.  The basic idea is to order the scanlines in the image data

 **	so that widely separated scanlines come first, and then we fill in

 **	the ones we missed.  This scheme was designed to allow quick previewing

 **	of an incomming GIF file by seeing some of the scanlines, all over

 **	the image, first.  Then, if you didn't want to continue reading the

 **	GIF file you could cancel the read and save time by not bother with

 **	reading the rest in.  Although this idea was meant for slower technology,

 **	it has proved useful once more, since transmission of data from one 

 **	corner of the internet to the other is not instanataneous, and programs

 **	such as netScape display the image as it is received.

 **

 **	So, we generate interlaced gif's.

 **

 **	When interlacing is enabled, the image data is grouped into 4 passes:

 **

 **		Pass	Contains

 **		----	--------

 **		1	Every 8th row, starting with row 0

 **		2	Every 8th row, starting with row 4

 **		3	Every 4th row, starting with row 2

 **		4	Every 2nd row, starting with row 1

 **

 **	When interlacing is turned off, the image data instead contains

 **	image rows in sequential order from row 0 (top) to the last row.

 **/







/*

 *  FUNCTION DECLARATIONS

 */

#ifdef __STDC__

static int imGifVfbRead1(int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb, 

	unsigned char interlaced,unsigned char codesize );

static int imGifVfbRead8(int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb, 

	unsigned char interlaced, unsigned char codesize );

static int imGifNBitsPixel (int ncolors,int *);

static int imGifRead( int ioType, int fd, FILE *fp,TagTable *flagsTable, TagTable *tagTable);

static int imGifWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imGifWriteTransparency( int ioType, int fd, FILE* fp, int transValue);

#else

static int imGifVfbRead1( );

static int imGifVfbRead8( );

static int imGifNBitsPixel( );



static int imGifRead( );

static int imGifWrite( );

static int imGifWriteTransparency( );

#endif









/*

 *  FORMAT INFORMATION

 *	imGifNames		-  format's name and aliases

 *	imGifReadMap		-  read attributes

 *	imGifWriteMap		-  write attributes

 *	imGifMagicNumber	-  magic number

 *	imGifMagic		-  list of magic numbers

 *	ImFileGifFormat		-  master format description

 */

static char *imGifNames[ ] = { "gif", "giff", NULL };

static ImFileFormatReadMap imGifReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IN,1,1,       LZW,    IMVFBMONO,      0 },

        { IN,1,2,       LZW,    IMVFBINDEX8,    0 },

        { IN,1,3,       LZW,    IMVFBINDEX8,    0 },

        { IN,1,4,       LZW,    IMVFBINDEX8,    0 },

        { IN,1,5,       LZW,    IMVFBINDEX8,    0 },

        { IN,1,6,       LZW,    IMVFBINDEX8,    0 },

        { IN,1,7,       LZW,    IMVFBINDEX8,    0 },

        { IN,1,8,       LZW,    IMVFBINDEX8,    0 },



        { IN,1,1,       LZW|C,  IMVFBMONO,      C },

        { IN,1,2,       LZW|C,  IMVFBINDEX8,    C },

        { IN,1,3,       LZW|C,  IMVFBINDEX8,    C },

        { IN,1,4,       LZW|C,  IMVFBINDEX8,    C },

        { IN,1,5,       LZW|C,  IMVFBINDEX8,    C },

        { IN,1,6,       LZW|C,  IMVFBINDEX8,    C },

        { IN,1,7,       LZW|C,  IMVFBINDEX8,    C },

        { IN,1,8,       LZW|C,  IMVFBINDEX8,    C },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imGifWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBMONO,    C|A,    IN,1,1,         LZW|C|A,imGifWrite },

        { IMVFBMONO,    C,      IN,1,1,         LZW|C,  imGifWrite },

        { IMVFBINDEX8,  C|A,    IN,1,8,         LZW|C|A,imGifWrite },

        { IMVFBINDEX8,  C,      IN,1,8,         LZW|C,  imGifWrite },

        { -1,           0,      -1,             0,      NULL },

};



static unsigned char imGifMagicNumber87[ ] = { 'G', 'I', 'F', '8', '7', 'a' };

static unsigned char imGifMagicNumber89[ ] = { 'G', 'I', 'F', '8', '9', 'a' };

static ImFileMagic imFileGifMagic[ ] =

{

	{ 0, 6, imGifMagicNumber87 },

	{ 0, 6, imGifMagicNumber89 },

	{ 0, 0, NULL },

};



ImFileFormat ImFileGifFormat =

{

	imGifNames,					/* Names	*/

	"Graphics Image File",				/* Description	*/

	"Compuserve",					/* Creator	*/

	"1- thru 8-bit color index Lempel-Ziv & Welch-compressed single- or\n\
multi-image files.",					/* Read support	*/
	"1 and 8-bit color index Lempel-Ziv & Welch-compressed single- or\n\
multi-image files.",					/* Write support*/

	imFileGifMagic,					/* Magic #'s	*/

	IMMULTI, IMPIPE,				/* Read?	*/

	IMMULTI, IMPIPE,				/* Write?	*/

	imGifRead, imGifReadMap, imGifWriteMap,		/* Maps		*/

};











/*

 *  TYPEDEF & STRUCTURE

 *	imGifHeaderInfo		-  Gif header information

 *	imGifHeaderFields	-  Gif Info description for Bin pkg

 *	imGifHeader		-  Gif header holder

 *

 *  DESCRIPTION

 *	The imGifHeaderInfo includes the GIF Header block and Logical Screen

 *	block fields.

 */



typedef struct imGifHeaderInfo

{

	unsigned char gif_magic[6];	/* GIF signature + version	*/

	sdsc_uint16   gif_width;	/* Screen width			*/

	sdsc_uint16   gif_height;	/* Screen height		*/

	unsigned char gif_flags;	/* Assorted flags		*/

	unsigned char gif_background;	/* Color index of screen background*/

	unsigned char gif_aspect;	/* Pixel aspect ratio		*/

} imGifHeaderInfo;



static imGifHeaderInfo		imGifHeader;	/* GIF file header	*/



static BinField imGifHeaderFields[ ] =

{

	{ UCHAR,  1, 6 },		/* gif_magic			*/

	{ UINT16, 2, 1 },		/* gif_width			*/

	{ UINT16, 2, 1 },		/* gif_height			*/

	{ UCHAR,  1, 1 },		/* gif_flags			*/

	{ UCHAR,  1, 1 },		/* gif_background		*/ 

	{ UCHAR,  1, 1 },		/* gif_aspect			*/

	{ 0, 0, 0 }

};











/*

 *   TYPEDEF & STRUCTURE

 *	imGifImDescInfo		- Gif image descriptor information

 *	imGifImDescFields	- imGifImDescInfo description for Bin pkg

 *	imGifImDesc		- Gif image descriptor holder

 *

 *   DESCRIPTION

 *	The image descriptor defines the actual placement and extents of the

 *	following image within the space defined by screen width and height.

 *	Also defined are flags to indicate the presence of a local color

 *	lookup map, the pixel display sequence and the #bits per pixel

 *	defined for this image.

 *	

 */



typedef struct imGifImDescInfo   

{

	sdsc_uint16	im_left;       	/* Start of image from left side*/

	sdsc_uint16	im_top;		/* Start of image from top      */

	sdsc_uint16	im_width;	/* Width of the image in pixels	*/

	sdsc_uint16	im_height;	/* Height of the image in pixels*/

	unsigned char	im_flags;	/* Assorted flags		*/

	

}imGifImDescInfo;



static imGifImDescInfo	imGifImDesc;	/* GIF image descriptor 	*/



static BinField imGifImDescFields[] =

{

	{ UINT16, 2, 1 },		/* im_left        		*/

	{ UINT16, 2, 1 },		/* im_top         		*/

	{ UINT16, 2, 1 },		/* im_width       		*/

	{ UINT16, 2, 1 },		/* im_height      		*/

	{ UCHAR,  1, 1 },		/* im_flags	  		*/ 

	{ 0, 0, 0 }

};







/*

 *   TYPEDEF 

 *	imGifControlBlock	-  Gif Control Block structure

 *

 *   DESCRIPTION

 *	This is the structure that is contained in an graphics control

 *   block.  We just use it to set the transparency field.

 */

typedef struct imGifControlBlock

{

	unsigned char blocksize; 	/* always 4	                   */

	unsigned char flags;            /* what's in the block             */

	sdsc_uint16 delay;              /* how long to wait for a keypress */

	unsigned char transparent_color; /* The color that is transparent  */

	unsigned char terminator;       /* always 0                        */

} imGifControlBlock;



static BinField imGifControlBlockFields[] = 

{

	{ UCHAR, 1, 1},			/* blocksize                       */

	{ UCHAR, 1, 1},                 /* flags                           */

	{ UINT16, 2, 1},                /* delay                           */

	{ UCHAR, 1, 1},                 /* transparent_color               */

	{ UCHAR, 1, 1},                 /* terminator                      */

	{ 0, 0, 0 }

};





/*

 *  CONSTANTS

 *      IMGIFMAGIC87a   -  file magic number, version 87a

 *      IMGIFMAGIC89a   -  file magic number, version 89a

 *      IMGIFBITPIXEL   -  mask to read #bits/pixel in the image

 *      IMGIFGLOBALCM   -  mask to read if a global colormap is present

 *      IMGIFLOCALCM    -  mask to read if a local colormap is present

 *      IMGIFINTERLACED -  mask to read the pixel display sequence used

 *      IMGIFTERMINATOR -  termination character of a Gif image file

 *      IMGIFEXTENSION  -  Gif extension block introducer character

 *      IMGIFSEPARATOR  -  image separator character

 *      IMGIFENDBLOCK   -  terminates extension blocks

 *      IMGIFNCOLORRES  -  # bits of color resolution

 *      IMGIFNBITSPIXEL -  # bits per pixel

 *      IMGIFBACKGROUND -  index of background color

 *      IMGIFCODESIZE   -  initial code size used for lzw algorithm

 *      IMGIFNOCOLORMAP -  no color lookup table present

 *

 */



#define IMGIFMAGIC87a   "GIF87a"

#define IMGIFMAGIC89a   "GIF89a"

#define IMGIFBITPIXEL   0x07

#define IMGIFGLOBALCM   0x80

#define IMGIFLOCALCM    0x80

#define IMGIFINTERLACED 0x40

#define IMGIFTERMINATOR ';'

#define IMGIFEXTENSION  '!'

#define IMGIFSEPARATOR  ','

#define IMGIFENDBLOCK   0

#define IMGIFNCOLORRES  8

#define IMGIFNBITSPIXEL 8

#define IMGIFBACKGROUND 0

#define IMGIFCODESIZE   IMGIFNBITSPIXEL

#define IMGIFNOCOLORMAP 0

#define IMGIFGRAPHICCONTROL	0xF9

#define IMGIFCOMMENT		0xFE

#define IMGIFPLAINTEXT		0x01

#define IMGIFAPPLICATION	0xFF

#define IMGIFTRANSPARENCY	0x01	/* For graphic control blocks */











/*

 *  FUNCTION

 *	imGifCltRead	- read a color lookup table

 *

 *  DESCRIPTION

 *	Used for reading either global or local color tables, this routine

 *	reads in a a table of nClt colors and stores it into a new ImClt

 *	which is then returned.

 */



static int				/* Returns status		*/

#ifdef __STDC__

imGifCltRead( int ioType, int fd, FILE *fp, int nClt, ImClt **clt ) 

#else

imGifCltRead( ioType, fd, fp, nClt, clt ) 

	int	ioType;			/* Type of I/O to do		*/

	int	fd;			/* Descriptor to read from	*/

	FILE	*fp;			/* Pointer to read from		*/

	int	nClt;			/* # of colors to read		*/

	ImClt **clt;			/* Where to put them		*/

#endif

{

	ImCltPtr cptr;			/* Color pointer		*/

	unsigned char *cltBuffer;	/* Color lookup table buffer	*/

	unsigned char *colorp;		/* Color buffer pointer		*/

	int i;				/* Counter			*/





	/*

	 *  Allocate a new ImClt and a temporary buffer to store the

	 *  the raw incomming data.

	 */

	if ( (*clt = ImCltAlloc(nClt)) == IMCLTNULL)

	{

		ImErrorFatal ( ImQError(), -1, ImErrNo );	

	}

	ImMalloc( cltBuffer, unsigned char *, sizeof( UCHAR ) * nClt * 3 );





	/* Read in Red, Green, and Blues.                        	*/

        if ( ImBinRead( ioType, fd, fp, cltBuffer, UCHAR, 1, nClt * 3 ) == -1 )

        {

                free( (char *)cltBuffer );

                ImReturnBinError( );

        }





	/* And copy them to the ImClt.					*/

	cptr   = ImCltQFirst( *clt );

	colorp = cltBuffer;

	for ( i = 0; i < nClt; i++ )

	{

		ImCltSRed(   cptr, *(colorp++) ); 

		ImCltSGreen( cptr, *(colorp++) );

		ImCltSBlue(  cptr, *(colorp++) );

		ImCltSInc(   *clt, cptr );

	}

	free( (char *) cltBuffer );

	return ( 0 );

}











/*

 *  FUNCTION

 *	imGifExtRead	    - Read the gif extension blocks 

 *	

 *  DESCRIPTION

 *	The only extension we pay attention to is image transparency.

 *	If we see this, we add it to the data table.

 *

 *	Note that we intentionally don't use seek() calls to skip the

 *	extension data.  This allows us to read from pipes.

 */



static int				/* Returns status		*/

#ifdef __STDC__

imGifExtRead( int ioType, int fd, FILE *fp, TagTable* tagTable )

#else

imGifExtRead( ioType, fd, fp, tagTable )

	int	ioType;			/* Type of I/O to do		*/

	int	fd;			/* Descriptor to read from	*/

	FILE	*fp;			/* Pointer to read from		*/

	TagTable *tagTable;		/* tag table                    */

#endif

{

	unsigned char c, buf[256];	/* Temp buffer			 */

	int transparency=0;             /* Is this a transparency block? */

	imGifControlBlock block;        /* structure with transparency   */

	char* tmp;			/* temporary string for tagtable */

	char message[100];		/* buffer for message            */



	/*

	 *	Read in extension label

	 */

	if ( ImBinRead( ioType, fd, fp, &c, UCHAR, 1, 1 ) == -1 )

	{

		ImReturnBinError();

	}



	/*

	 *	Make sure extension blocks are valid ones

	 */

	switch( c )

	{

		case IMGIFGRAPHICCONTROL:

			transparency = 1;

			break;

		case IMGIFCOMMENT:

		case IMGIFPLAINTEXT:

		case IMGIFAPPLICATION:

			break;

		default:

			ImErrNo = IMESYNTAX;

			ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	if (transparency==1)

	{

		if ( ImBinReadStruct( ioType, fd, fp, &block,

			imGifControlBlockFields ) == -1 )

		{

			ImReturnBinError();

		}

		/* Put block.transparent_color in tagTable */

		if (block.flags & IMGIFTRANSPARENCY)

		{

			ImMalloc(tmp, char * , 15 );

			sprintf(tmp,"index=%d",(int)block.transparent_color);

		        TagTableAppend( tagTable,

				TagEntryAlloc( "transparency value", POINTER, &tmp));

		}

		sprintf(message,"Pixels with index %d.",(int)block.transparent_color);

		ImInfo ("Transparency",message);

	}

	else

	{

		/*

		 *	Skip over extension data

		 */

		for ( ; ; )

		{

			/*

			 *	Read in block size or block terminator

			 */

			if ( ImBinRead( ioType, fd, fp, &c, UCHAR, 1, 1 ) == -1 )

			{

				ImReturnBinError();

			}

			/*

			 *	If block terminator, quit loop

			 */

			if ( c == IMGIFENDBLOCK )

				break;



			/*

			 *	Skip over block

			 */

			if ( ImBinRead( ioType, fd, fp, buf, UCHAR, 1, c  ) == -1 )

			{

				ImReturnBinError();

			}

		}

	}



	return( 0 );

}











/*

 *  FUNCTION

 *	imGifRead	-  read a gif image file

 *

 *  DESCRIPTION

 *	The file header is read and the magic number checked. 

 *	Separate routines are then called to handle different

 *	sizes (1 and anything from 2 to 8)

 */



static int				/* Returns status		*/

#ifdef __STDC__

imGifRead( int ioType, int fd, FILE *fp,TagTable *flagsTable, TagTable *tagTable)

#else

imGifRead( ioType, fd, fp, flagsTable, tagTable )

	int              ioType;	/* I/O flags			*/

	int	         fd;		/* Input file descriptor	*/

	FILE	        *fp;		/* Input file pointer		*/

	TagTable        *flagsTable;	/* Flags			*/

	TagTable        *tagTable;	/* Tag table to add to		*/

#endif

{



	ImVfb		 *vfb;		/* Read in image		*/

	int		 nGClt,nLClt;	/* Number of Global and Local   */

			                /* CLT entries			*/

	ImClt		 *gClt, *lClt;	/* Read in colormaps		*/

	unsigned char	 c;		/* aux var			*/

	int		 end;		/* end to reading images	*/

	int		 status;	/* Return status		*/

	int		 imageCount = 1;/* Number of images processed	*/

	char		 message[100];	/* ImInfo message		*/

	int		 nbits;		/* # of bits per pixel		*/





	/*

	 *	Set the Binary I/O package's byte order

	 */

	BinByteOrder( BINLBF );



	/*

	 *	Read GIF header

	 */

	if ( ImBinReadStruct( ioType, fd, fp, &imGifHeader,

		imGifHeaderFields )== -1)

	{

		ImReturnBinError( );

	}



	/*

	 *	Check for valid Signature and version (treated as magic

	 *	numbers).  Always 6 bytes long.

	 */

	if( strncmp( (char*)imGifHeader.gif_magic, IMGIFMAGIC87a, 6 ) == 0 )

	{

		ImInfo( "Version", "GIF87a" );

	}

	else if ( strncmp( (char*)imGifHeader.gif_magic, IMGIFMAGIC89a, 6 ) == 0 )

	{

		ImInfo( "Version", "GIF89a" );

	}

	else

	{

		ImErrNo = IMEMAGIC;

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	ImInfo( "Byte Order", "Least Significant Byte First" );





	/*

	 *	Read the Global Color Map if it exists

	 */

	gClt = IMCLTNULL;

	lClt = IMCLTNULL;

	if ( (imGifHeader.gif_flags & IMGIFGLOBALCM) == IMGIFGLOBALCM )

	{

		/*

		 *  Compute the size of the GCLT and read it in.

		 */

		nGClt = 2 << ( imGifHeader.gif_flags & IMGIFBITPIXEL );

		if ( imGifCltRead( ioType, fd, fp, nGClt, &gClt ) == -1 )

			return ( -1 );		/* Error already handled */

		sprintf( message, "%d Entries", nGClt );

		ImInfo( "Global Color Table", message );

	}

	else

	{

		ImInfo( "Global Color Table", "none" );

	}



	/*

	 *	Starts to read the images following

	 */

	end = 0;

	while(!end)

	{

		if ( ImBinRead(ioType,fd,fp,&c,UCHAR,1,1 ) == -1 )

		{

			ImReturnBinError();

		}

		switch(c)

		{

		case IMGIFSEPARATOR:	/* Image separator character 	  */



		   /*

		    *	Read the gif image descriptor

		    */



		   if ( ImBinReadStruct( ioType, fd, fp, &imGifImDesc,

		        imGifImDescFields ) == -1 )

		   {

		   	ImReturnBinError();

		   }



		   /*

		    *   Output -verbose messages

		    */

		   sprintf( message, "%d of ?", imageCount++ );

		   ImInfo( "Image", message );



		   sprintf( message, "%d x %d", imGifImDesc.im_width,

			    imGifImDesc.im_height );

		   ImInfo( "Resolution", message );



		   if ( (imGifImDesc.im_flags & IMGIFLOCALCM) == IMGIFLOCALCM)	

		   {

			nbits = (imGifImDesc.im_flags & IMGIFBITPIXEL) + 1;

		   	sprintf( message, "%d-bit Color Indexed", nbits );

		  	ImInfo( "Type", message );

		   }

		   else

		   {

			/* No local color table.			*/

			nbits = (imGifHeader.gif_flags&IMGIFBITPIXEL) + 1;

			if ( gClt == NULL && nbits == 1 )

			{

				ImInfo( "Type", "1-bit Monochrome" );

			}

			else if ( gClt == NULL )

			{

				sprintf( message, "%d-bit Grayscale", nbits );

				ImInfo( "Type", message );

			}

			else

			{

				sprintf( message, "%d-bit Color Indexed",nbits);

				ImInfo( "Type", message );

			}

		   }



		   /*

		    *	Read the local color map if it exists

		    */



		   if ( (imGifImDesc.im_flags & IMGIFLOCALCM) == IMGIFLOCALCM)	

		   {

		      nLClt = 2 << ( imGifImDesc.im_flags & IMGIFBITPIXEL );

		      if ( imGifCltRead(ioType,fd,fp,nLClt,&lClt) == -1 )

				return ( -1 );

		      sprintf( message, "%d Entries", nLClt );

		      ImInfo( "Local Color Table", message );

		   }

		   else

		   {

		      ImInfo( "Local Color Table", "none" );

		   }



		   ImInfo( "Compression Type", "Lempel-Ziv and Welch (LZW)");



		   /*

		    *	Read the initial code size

		    */



	           if(ImBinRead(ioType,fd,fp,&c, UCHAR, 1, 1 ) == -1 )

        	   {

                	ImReturnBinError( );

        	   }



		   /*

 		    *	Read the raster data into the vfb

		    */



		   /*

       		    * check the #bits/pixel

         	    */



		   if ( nbits == 1 )

                         status = imGifVfbRead1(ioType, fd, fp, flagsTable,

				tagTable, &vfb,

				(imGifImDesc.im_flags & IMGIFINTERLACED),c);

	  	   else 

		         status = imGifVfbRead8( ioType, fd, fp, flagsTable,

				tagTable, &vfb,

				(imGifImDesc.im_flags & IMGIFINTERLACED),c);

		   if ( status == -1 )

			return ( -1 );		/* Error already handled*/





		   /*

		    *	Set the color lookup table

		    */

		  



		   /*

		    *	if a local color map is present then it is assigned

		    *	to the vfb. if not, then the global color table

		    *   is used.

		    */



		   if (((imGifImDesc.im_flags & IMGIFLOCALCM) == IMGIFLOCALCM )

			   && ( lClt != IMCLTNULL) )

		   {

		   	ImVfbSClt( vfb, lClt );

		        TagTableAppend( tagTable,

				TagEntryAlloc( "image clt", POINTER, &lClt));

		   }

		   else if (gClt != IMCLTNULL)

		   {

			ImVfbSClt( vfb, gClt );

		        TagTableAppend( tagTable,

				TagEntryAlloc( "image clt", POINTER, &gClt));

		   }

		   TagTableAppend( tagTable,

			TagEntryAlloc( "image vfb", POINTER, &vfb));

		   break;

	

		case IMGIFEXTENSION:	/* Gif extension block introducer */	

		   if ( imGifExtRead(ioType,fd,fp, tagTable) == -1 )

			return ( -1 );		/* Error already handled*/

		   break;



		case IMGIFTERMINATOR:	/* GIf terminator 		*/

		   end = 1;

		   break;

		}

	}

	return ( 1 );

}











/*

 *  FUNCTION

 *	imGifVfbRead8	-  read 2-8 bit Gif image

 *

 *  DESCRIPTION

 *	A new VFB is allocated.  The image is read in, pixel by pixel,

 *	using the LZW algorithm, into the VFB. The order of the

 *	scanlines is given by the value of interlaced. If it is

 *	equal to IMGIFINTERLACED means that the scanlines are

 *	organized in an interlaced way. If not, they are written

 *	sequentially.

 */



static int				/* Returns status		*/

#ifdef __STDC__

imGifVfbRead8(int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb, 

	unsigned char interlaced, unsigned char codesize )

#else

imGifVfbRead8(ioType, fd, fp, flagsTable, tagTable, pVfb, interlaced, codesize )

	int               ioType;	/* I/O flags			*/

	int	          fd;		/* Input file descriptor	*/

	FILE	         *fp;		/* Input file pointer		*/

	TagTable         *flagsTable;	/* Flags			*/

	TagTable         *tagTable;	/* Tag table to add to		*/

	ImVfb	        **pVfb;		/* VFB to fill and return	*/

	unsigned char		  interlaced;	/* tells if the image is inter  */

	unsigned char             codesize;     /* Inital code size used by LWZ */

					/* laced			*/	

#endif

{

	unsigned char    	index; 		/* color index 			*/

	ImVfb	 *vfb;		/* New vfb			*/

	ImVfbPtr pptr;		/* Pixel pointer		*/

	int		  column;	/* Column pixel into the image  */

	int		  row;		/* Row pixel into the image	*/

	int		  pass;		/* used for interlaced image	*/

	int		  value;	/* pixel value returned by LWZ  */	

	int		  size;		/* size of image in pixels	*/

	int		  cont;		/* counter			*/







	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (*pVfb = ImVfbAlloc( imGifImDesc.im_width, imGifImDesc.im_height,

		 IMVFBINDEX8 )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	vfb = *pVfb;



	/* initializing lzw structures	*/

	if ( imLzwReadByte(ioType, fd, fp, TRUE, codesize ) < 0 )

		return( 0 );



	size = imGifImDesc.im_width * imGifImDesc.im_height;

	pptr = ImVfbQFirst( vfb );

	cont = 0;

	if (interlaced != IMGIFINTERLACED)

	{

	   while ((cont < size) &&

	      ((value = imLzwReadByte(ioType, fd, fp, FALSE, codesize)) >= 0) )	

	   {

		 index = value;

		 ImVfbSIndex8( vfb, pptr, index);

		 ImVfbSInc( vfb, pptr );

		 cont++;

	   }

	}	

	else	

	{

	   column = row = pass = 0;

	   while ((cont < size) &&

	      ((value = imLzwReadByte(ioType, fd, fp, FALSE, codesize)) >= 0 ))

	   {

		pptr = ImVfbQPtr( vfb, column,row );

		index = value;

		ImVfbSIndex8( vfb, pptr, index);

		cont++;

		if (++column == imGifImDesc.im_width)

		{ 

			  column = 0;

			  switch (pass) {

				  case 0:	

				  case 1:

					  row+= 8;

					  break;

				  case 2:

					  row+= 4;

					  break;

				  case 3: 

					  row+= 2;

					  break;

			  }

			  if (row >= imGifImDesc.im_height )

				  switch(++pass)

				  {

					  case 1:

						  row = 4;

						  break;

					  case 2:

						  row = 2;

						  break;

					  case 3:

						  row = 1;

						  break;

				  }

		}

	     }

	}

	return 1;

}











/*

 *  FUNCTION

 *	imGifVfbRead1	-  read 1-bit  Gif image

 *

 *  DESCRIPTION

 *	A new VFB is allocated.  The image is read in, one scanline at

 *	a time, and converted into the VFB.

 *	

 */



static int				/* Returns status		*/

#ifdef __STDC__

imGifVfbRead1(int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb, 

	unsigned char interlaced,unsigned char codesize )

#else

imGifVfbRead1(ioType, fd, fp, flagsTable, tagTable, pVfb, interlaced,codesize )

	int               ioType;	/* I/O flags			*/

	int	          fd;		/* Input file descriptor	*/

	FILE	         *fp;		/* Input file pointer		*/

	TagTable         *flagsTable;	/* Flags			*/

	TagTable         *tagTable;	/* Tag table to add to		*/

	ImVfb	        **pVfb;		/* VFB to fill and return	*/

	unsigned char		  interlaced;	/* tells if the image is inter  */

	unsigned char		  codesize;	/* Initial code size used by LWZ*/

					/* laced			*/	

#endif

{

	ImVfb	 *vfb;		/* New vfb			*/

	ImVfbPtr pptr;		/* Pixel pointer		*/

	int		  column;	/* Column pixel into the image  */

	int		  row;		/* Row pixel into the image	*/

	int		  pass;		/* used for interlaced image	*/

	int		  value;	/* pixel value returned by LWZ  */	

	int		  size;		/* size of image in pixels	*/

	int		  cont;







	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (*pVfb = ImVfbAlloc( imGifImDesc.im_width, imGifImDesc.im_height,

		 IMVFBMONO )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	vfb = *pVfb;



	/* initializing lzw structures	*/

	if ( imLzwReadByte(ioType, fd, fp, TRUE, (int) codesize ) < 0 )

		return( 0 );



	size = imGifImDesc.im_width * imGifImDesc.im_height;

	pptr = ImVfbQFirst( vfb );

	cont = 0;

	if (interlaced != IMGIFINTERLACED)

	{

	   while ((cont < size) &&

	      ((value = imLzwReadByte(ioType, fd, fp, FALSE, codesize)) >= 0) )	

	   {

		 ImVfbSMono( vfb, pptr, (value) ? 1 : 0);

		 ImVfbSInc( vfb, pptr );

		 cont++;

	   }

	}	

	else	

	{

	   column = row = pass = 0;

	   while ((cont < size) &&

	       ((value = imLzwReadByte(ioType, fd, fp, FALSE, codesize)) >= 0))

	   {

		pptr = ImVfbQPtr( vfb, column,row );

		ImVfbSMono ( vfb, pptr, (value) ? 1 : 0);

		cont++;

		if (++column == imGifImDesc.im_width)

		{ 

			  column = 0;

			  switch (pass) {

				  case 0:	

				  case 1:

					  row+= 8;

					  break;

				  case 2:

					  row+= 4;

					  break;

				  case 3: 

					  row+= 2;

					  break;

			  }

			  if (row >= imGifImDesc.im_height )

				  switch(++pass)

				  {

					  case 1:

						  row = 4;

						  break;

					  case 2:

						  row = 2;

						  break;

					  case 3:

						  row = 1;

						  break;

				  }

		}

	     }

	}

	return 1;

}











/*

 *  FUNCTION

 *      imGifCltWrite    - Write a color lookup table

 *

 *  DESCRIPTION

 *	Write the color lockup table clt into the file

 */

 

static int				/* Returns status		*/

#ifdef __STDC__

imGifCltWrite(int ioType, int fd, FILE *fp, int nClt,int newClt, ImClt *clt)

#else

imGifCltWrite(ioType, fd, fp, nClt,newClt, clt)

	int     ioType;

	int     fd;

	FILE    *fp;

	int     nClt;

	int 	newClt;

	ImClt	*clt;

#endif

{

	ImCltPtr cptr;          /* Color pointer                */

        unsigned char   *cltBuffer;     /* Color lookup table buffer    */

        unsigned char   *colorp;        /* Color buffer pointer         */

        int i;



	ImMalloc( cltBuffer, unsigned char *, sizeof( unsigned char ) * newClt * 3 );



        colorp   = cltBuffer;

        cptr     = ImCltQFirst( clt );



	for (i=0; i< nClt; i++)

        {

                *(colorp++) = ImCltQRed  ( cptr );

		*(colorp++) = ImCltQGreen( cptr );

		*(colorp++) = ImCltQBlue ( cptr );

                ImCltSInc(  clt, cptr );

        }





	if ( ImBinWrite( ioType, fd, fp, cltBuffer, UCHAR, 1, newClt * 3 ) == -1)

        {

                free( (unsigned char *)cltBuffer );

                ImReturnBinError( );

        }

	return 1;

}











/*

 *  FUNCTION

 *      imGifVfbWrite8    - 	Write an 8-bit vfb into a gif file 

 *

 *  DESCRIPTION

 */



static int                              /* Returns status               */

#ifdef __STDC__

imGifVfbWrite8( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb *vfb, 

	unsigned char interlaced )

#else

imGifVfbWrite8( ioType, fd, fp, flagsTable, tagTable, vfb, interlaced )

        int               ioType;       /* I/O flags                    */

        int               fd;           /* Input file descriptor        */

        FILE             *fp;           /* Input file pointer           */

        TagTable         *flagsTable;   /* Flags                        */

        TagTable         *tagTable;     /* Tag table to add to          */

        ImVfb            *vfb;          /* VFB to write	                */

        unsigned char    interlaced;    /* tells if the image is inter  */

                                        /* laced                        */

#endif

{

	unsigned char	 *rasterimage;	/* image pixel values		*/

	int		  i;

	int		  size;		/* size of the image		*/

	int		  pass;

	int		  row,column;

	ImVfbPtr	  ptr;

	unsigned char		  c;







	/*

	 *	Write an Image separator

	 */



	c = IMGIFSEPARATOR;

	if ( ImBinWrite( ioType, fd, fp, &c, UCHAR, 1, 1 ) == -1 )

	{

                ImReturnBinError( );  

	}



	/*

	 *	Setup the Image Descriptor and write it out

	 */



	imGifImDesc.im_left   = imGifImDesc.im_top = 0;

	imGifImDesc.im_width  = ImVfbQWidth ( vfb );

	imGifImDesc.im_height = ImVfbQHeight ( vfb ); 

	imGifImDesc.im_flags = interlaced;



	if ( ImBinWriteStruct( ioType, fd, fp, &imGifImDesc,

                imGifImDescFields ) == -1 )

	{

                ImReturnBinError( );

	}





	/*

	 *	Write out the initial code size before 

	 * 	start to compress the raster data

	 */



	c = IMGIFCODESIZE;

	if ( ImBinWrite( ioType, fd, fp, &c, UCHAR, 1, 1 ) == -1 )

	{

                ImReturnBinError( );

	}



	/*

	 *	Allocate a buffer to store the image pixel values

	 *	to be compressed                        

	 */



	size =  imGifImDesc.im_width * imGifImDesc.im_height;

	ImMalloc( rasterimage, unsigned char *, size ); 

        if ( rasterimage == NULL )

        {

                ImErrNo = IMEMALLOC;

                ImErrorFatal( ImQError( ), -1, ImErrNo );

        }



	/*

 	 *	Fill the buffer with the pixel values

	 */



	ptr = ImVfbQFirst(vfb);

	if (interlaced != IMGIFINTERLACED)

	   for (i=0; i< size; i++)

	   {

	      rasterimage[i] = ImVfbQIndex8(vfb,ptr);

	      ImVfbSInc(vfb,ptr);

	   }

	else

	{

	   column = row = pass = 0;

	   for (i=0; i< size; i++)

	   {

	      ptr = ImVfbQPtr( vfb, column,row );

	      rasterimage[i] = ImVfbQIndex8(vfb,ptr);

	      if (++column == imGifImDesc.im_width)

              {

                          column = 0;

                          switch (pass) {

                                  case 0:

                                  case 1:

                                          row+= 8;

                                          break;

                                  case 2:

                                          row+= 4;

                                          break;

                                  case 3:

                                          row+= 2;

                                          break;

                          }

                          if (row >= imGifImDesc.im_height )

                                  switch(++pass)

                                  {

                                          case 1:

                                                  row = 4;

                                                  break;



                                          case 2:

                                                  row = 2;

                                                  break;

                                          case 3:

                                                  row = 1;

                                                  break;

                                  }

                }

	      }

	   }



	/*

	 *	Compress the raster data

	 */





	imLzwCompGif( ioType, fd, fp, IMGIFCODESIZE + 1, rasterimage, size);



	free ((unsigned char *) rasterimage);



	/*

	 *	Write out a Zero-length packet

	 */



	

	c = 0;

	if ( ImBinWrite( ioType, fd, fp, &c, UCHAR, 1, 1 ) == -1 )

	{

                ImReturnBinError( );

	}



	/*

	 *	Write the Gif terminator

	 */



	c = IMGIFTERMINATOR;

	if ( ImBinWrite( ioType, fd, fp, &c, UCHAR, 1, 1 ) == -1 )

	{

                ImReturnBinError( );

	}



	return(1);

}



/*

 *  FUNCTION

 *      imGifVfbWrite1    -     Write an 1-bit vfb into a gif file

 *

 */

 

static  int                             /* Returns status               */

#ifdef __STDC__

imGifVfbWrite1( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb *vfb, 

	unsigned char interlaced )

#else

imGifVfbWrite1( ioType, fd, fp, flagsTable, tagTable, vfb, interlaced )

        int               ioType;       /* I/O flags                    */

        int               fd;           /* Input file descriptor        */

        FILE             *fp;           /* Input file pointer           */

        TagTable         *flagsTable;   /* Flags                        */

        TagTable         *tagTable;     /* Tag table to add to          */

        ImVfb            *vfb;          /* VFB to write                 */

        unsigned char             interlaced;   /* tells if the image is inter  */

                                        /* laced                        */

#endif

{

        unsigned char            *rasterimage;  /* image pixel values           */

        int               i;            /* counter                      */

        int               size;         /* size of the image            */

        int               pass;         /* pass number                  */

        int               row,column;   /* pixel row and pixel column   */

        ImVfbPtr          ptr;          /* pixel pointer                */

        unsigned char             c;            /* auxiliar var                 */

 

 

 

 

        /*

         *      Write an Image separator

         */

 

        c = IMGIFSEPARATOR;

        if ( ImBinWrite( ioType, fd, fp, &c, UCHAR, 1, 1 ) == -1 )

        {

                ImReturnBinError( );

        }

 

        /*

         *      Setup the Image Descriptor and write it out

         */

 

        imGifImDesc.im_left   = imGifImDesc.im_top = 0;

        imGifImDesc.im_width  = ImVfbQWidth ( vfb );

        imGifImDesc.im_height = ImVfbQHeight ( vfb );

        imGifImDesc.im_flags = interlaced;

 

        if ( ImBinWriteStruct( ioType, fd, fp, &imGifImDesc,

                imGifImDescFields ) == -1 )

        {

                ImReturnBinError( );

        }

 

 

        /*

         *      Write out the initial code size before

         *      start to compress the raster data. When

         *      the number of bits per pixel is 1 the

         *      initial code size used for LWZ algorithm is

         *      equal to 2.

         */

 

        c = 2;

 

        if ( ImBinWrite( ioType, fd, fp, &c, UCHAR, 1, 1 ) == -1 )

        {

                ImReturnBinError( );

        }

 

        /*

         *      Allocate a buffer to store the image pixel values

         *      to be compressed

         */

 

        size =  imGifImDesc.im_width * imGifImDesc.im_height;

        ImMalloc( rasterimage, unsigned char *, size );

        if ( rasterimage == NULL )

        {

                ImErrNo = IMEMALLOC;

                ImErrorFatal( ImQError( ), -1, ImErrNo );

        }

 

        /*

         *      Fill the buffer with the pixel values

         */

 



        ptr = ImVfbQFirst(vfb);

        if (interlaced != IMGIFINTERLACED)

           for (i=0; i< size; i++)

           {

              rasterimage[i] = (ImVfbQMono(vfb,ptr)? 1 : 0);

              ImVfbSInc(vfb,ptr);

           }

        else

        {

           column = row = pass = 0;

           for (i=0; i< size; i++)

           {

              ptr = ImVfbQPtr( vfb, column,row );

              rasterimage[i] = (ImVfbQMono(vfb,ptr)? 1 : 0);

              if (++column == imGifImDesc.im_width)

              {

                          column = 0;

                          switch (pass) {

                                  case 0:

                                  case 1:

                                          row+= 8;

                                          break;

                                  case 2:

                                          row+= 4;

                                          break;

                                  case 3:

                                          row+= 2;

                                          break;

                          }

                          if (row >= imGifImDesc.im_height )

                                  switch(++pass)

                                  {

                                          case 1:

                                                  row = 4;

                                                  break;

 

                                          case 2:

                                                  row = 2;

                                                  break;

                                          case 3:

                                                  row = 1;

                                                  break;

                                  }

                }

              }

           }

        /*

         *      Compress the raster data

         */

 

 

        /*

         *      3 = initial code size + 1

         *      the initial code size is always 2 for

         *      one-bit images

         */

 

        imLzwCompGif( ioType, fd, fp, 3, rasterimage, size);

 

        free ((unsigned char *) rasterimage);

 

        /*

         *      Write out a Zero-length packet

         */

 

       

        c = 0;

        if ( ImBinWrite( ioType, fd, fp, &c, UCHAR, 1, 1 ) == -1 )

        {

                ImReturnBinError( );

        }



	/*

         *      Write the Gif terminator

         */

 

        c = IMGIFTERMINATOR;

        if ( ImBinWrite( ioType, fd, fp, &c, UCHAR, 1, 1 ) == -1 )

        {

                ImReturnBinError( );

        }

 

 

        return(1);

}











/*

 *  FUNCTION

 *      imGifWrite       -  write a vfb image into gif image format

 *

 *  DESCRIPTION

 */



static int        	              /* Returns # of tags written    */

#ifdef __STDC__

imGifWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imGifWrite( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int              ioType;        /* I/O flags                    */

	int              fd;            /* Output file descriptor       */

	FILE            *fp;            /* Output file pointer          */

	TagTable        *flagsTable;    /* Format Flags                 */

	TagTable        *tagTable;      /* Tag list to add to           */

#endif

{	

	ImVfb            *vfb;		/* Read in image		*/

	int              nClt;          /* Number of CLT entries        */

        ImClt            *clt;          /* Read in colormap             */

	int		 newClt;	/* Number of CLT entries rounded to a power of two */

	int		 nbitspixel;	/* number of bits per pixel	*/

	char		 message[100];	/* ImInfo message		*/

	int 		transparency;	/* transparency value		*/







	ImInfo( "Version", "GIF89a" );

	ImInfo( "Byte Order", "Least Significant Byte First" );



	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );



	/*

         *  Set up the header and write it out

         */

        strncpy((char*)imGifHeader.gif_magic,IMGIFMAGIC89a,6);

        imGifHeader.gif_width  = ImVfbQWidth ( vfb );

        imGifHeader.gif_height = ImVfbQHeight( vfb );



	/*

	 *  Decide how to write out the GIF file.

	 */

	if (pMap->map_outAttributes & IMCLTYES)

	{

		/*

		 *  A CLT should be written out.  Higher level code will

		 *  have insured that it has a CLT.  Figure out how many

		 *  bits per pixel based upon the CLT size.

		 */

		clt = ImVfbQClt( vfb );

                nClt= ImCltQNColors( clt );

		nbitspixel = imGifNBitsPixel(nClt,&newClt);

		imGifHeader.gif_flags =  IMGIFGLOBALCM;

                imGifHeader.gif_flags|= (IMGIFNCOLORRES - 1) << 4;

	}

	else

	{

		/*

		 *  No CLT to write out.  Could be grayscale or monochrome.

		 */

		if ( ImVfbQFields( vfb ) & IMVFBINDEX8 )

			nbitspixel = 8;

		else

			nbitspixel = 1;

		imGifHeader.gif_flags =  IMGIFNOCOLORMAP;

	}

	imGifHeader.gif_flags |=  (nbitspixel-1);

        imGifHeader.gif_background = IMGIFBACKGROUND;

        imGifHeader.gif_aspect =

		( ImVfbQWidth( vfb ) / ImVfbQHeight( vfb ) + 15 ) / 64;

 

        BinByteOrder( BINLBF );



        if ( ImBinWriteStruct( ioType, fd, fp, &imGifHeader,

                imGifHeaderFields ) == -1 )

        {

                ImReturnBinError( );

        }



 



	/*

 	 *	Write the CLT if exists

	 */

	if (pMap->map_outAttributes & IMCLTYES)

	{

		if ( imGifCltWrite(ioType, fd, fp, nClt, newClt, clt) == -1 )

			return ( -1 );

		sprintf( message, "%d Entries", nClt );

		ImInfo( "Global Color Table", message );

	}

	else

		ImInfo( "Global Color Table", "none" );



	ImInfo( "Image", "1 of 1" );



	sprintf( message, "%d x %d", imGifHeader.gif_width,

		 imGifHeader.gif_height );

	ImInfo( "Resolution", message );

	

	/*

	 * If there is a request for transparency or if

	 * there's transparency in the tagTable, add it to

	 * the image.

	 */



	if ((transparency=ImGetTransparency(tagTable, flagsTable, vfb))!=-1)

	{

		if (imGifWriteTransparency( ioType, fd, fp, transparency)==-1)

		{

			return -1;

		}

	}



	/* query the vfb and call the appropiate routine */

	if ((ImVfbQFields( vfb ) & IMVFBMONO ))

	{

	  ImInfo( "Type", "1-bit Monochrome" );

	  ImInfo( "Local Color Table", "none" );

	  ImInfo( "Compression Type", "Lempel-Ziv and Welch (LZW)" );



          return(imGifVfbWrite1(ioType,fd,fp,flagsTable,tagTable,

		vfb,IMGIFINTERLACED));

	}

	else

	{

	  if ( pMap->map_outAttributes & IMCLTYES )

	    sprintf( message, "%d-bit Color Indexed", nbitspixel );

	  else

	    sprintf( message, "%d-bit Grayscale", nbitspixel );

	  ImInfo( "Type", message );

	  ImInfo( "Local Color Table", "none" );

	  ImInfo( "Compression Type", "Lempel-Ziv and Welch (LZW)" );



	  return(imGifVfbWrite8(ioType,fd,fp,flagsTable,tagTable,

		vfb,IMGIFINTERLACED));

	}

}











/*

 *  FUNCTION

 *	imGifNBitsPixel	-  compute # of bits to represent a color

 *

 *  DESCRIPTION

 *	Rounds the number of colors to the closest power

 *	of two and return the necesary number of bits to

 *	represent this number of colors.

 */



static int				/* Returns # of bits		*/

#ifdef __STDC__

imGifNBitsPixel (int ncolors,int *newNumber)

#else

imGifNBitsPixel (ncolors,newNumber)

	int ncolors;			/* number of colors 		*/

	int *newNumber;			/* new number of colors 	*/

#endif

{

	int nbits;



	*newNumber = 2; 

	nbits = 1; 

	if ( ncolors > 256)

	{    

		nbits = 8; 

		*newNumber = 256;  

	}    

	else 

	{

		while (*newNumber < ncolors)

		{    

			(*newNumber) <<= 1;

			nbits++;   

		} 

	}

	return(nbits); 

}





/*

 *  FUNCTION

 *	imGifWriteTransparency

 *

 *  DESCRIPTION

 *	Write out the color which is transparent in the image

 *  as specified by the flags table.  (As a default, write out

 *  the most popular color in the image.)

 *

 *  If the tagEntry parameter specifies an index value, use that.

 *

 *  If the tagEntry parameter specifes an rgb value, use the most

 *  common index value with that rgb value.

 *

 *  If the tagEntry parameter speciefies "most common", use the

 *  most common index value in the image.

 *

 *

 */

static int /* returns status */

#ifdef __STDC__

imGifWriteTransparency( int ioType, int fd, FILE* fp, int transparency)

#else

imGifWriteTransparency( ioType, fd, fp, transparency)

int ioType;

int fd;

FILE* fp;

int transparency;

#endif

{

	imGifControlBlock	controlBlock;    /* data */

	unsigned char		blockId;	 /* id for block */

	char message[1000];	/*message buffer */



	/* Specify that this is an extension */

	blockId = IMGIFEXTENSION;

        if ( ImBinWrite( ioType, fd, fp, &blockId, UCHAR, 1, 1 ) == -1 )

        {

                ImReturnBinError( );

        }



	/* Specify what type of extension this is */

	blockId = IMGIFGRAPHICCONTROL;

        if ( ImBinWrite( ioType, fd, fp, &blockId, UCHAR, 1, 1 ) == -1 )

        {

                ImReturnBinError( );

        }



	sprintf(message,"Pixels with index %d.",transparency);

	ImInfo("Transparency",message);



	controlBlock.blocksize = 0x04;

	controlBlock.flags     = 0x01;

	controlBlock.delay     = 0x00;

	controlBlock.transparent_color = (unsigned char)transparency;

	controlBlock.terminator = 0x00;



        if ( ImBinWriteStruct( ioType, fd, fp, &controlBlock,

                imGifControlBlockFields ) == -1 )

        {

                ImReturnBinError( );

        }





	return 1;

}





