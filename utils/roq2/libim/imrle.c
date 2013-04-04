/**

 **	$Header: /roq/libim/imrle.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imrle.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imrle.c		-  Utah Raster Toolkit RLE file output

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imrle.c contains read and write routines to handle RLE files for

 **	the image manipulation library.

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

 **	imRleRead	f  read a RLE file

 **	imRleWrite	f  write a RLE file

 **

 **	IMDumpRun	m  dump a run of identical pixels

 **	IMDumpLitRun	m  dump a run of literal pixels

 **

 **  HISTORY

 **	$Log: /roq/libim/imrle.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.14  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.13  1995/06/15  21:13:09  bduggan

 **	ByteOrder to Byte Order

 **

 **	Revision 1.12  1995/04/03  21:36:46  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.11  1995/01/10  23:43:21  bduggan

 **	made read/write routines static

 **	put in IMMULTI, IMPIPE instead of TRUE/FALSE

 **

 **	Revision 1.10  94/10/03  11:30:55  nadeau

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

 **	Revision 1.9  92/12/03  01:52:20  nadeau

 **	Corrected info messages.

 **	

 **	Revision 1.8  92/11/04  12:07:24  groening

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.7  92/10/19  14:10:48  groening

 **	added ImInfo statements

 **	

 **	Revision 1.6  92/08/31  17:35:07  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.5  91/10/03  09:20:12  nadeau

 **	Fixed #includes.

 **	

 **	Revision 1.4  91/03/14  13:44:40  nadeau

 **	Fixed bug in reading comments from RLE files.

 **	

 **	Revision 1.3  91/03/08  14:31:52  nadeau

 **	Complete rewrite.  There might be a comment or two left from

 **	the original.  Restructured.  Reorganized.  Rewrote.

 **	Optimized some (though it is still very slow).

 **

 **	Revision 1.2  91/01/21  15:40:16  doeringd

 **	Changes in imRleWrite to use ImMalloc for cmaptr to work-around

 **	Y/MP ANSI compiler (scc) problem.

 **

 **/



#include <string.h>

#include <sys/types.h>

#include "unistd.h"

#include "iminternal.h"











/*

 **  FORMAT

 **	RLE	-  Utah Raster Toolkit Run-Length Encoded image file

 **

 **  AKA

 **

 **  FORMAT REFERENCES

 **	Design of the Utah RLE Format, Spencer W. Thomas, University

 **		of Utah

 **

 **  CODE CREDITS

 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1991.

 **

 **  DESCRIPTION

 **	RLE files start with a fixed size header giving:

 **

 **		magic number

 **		image location (offset from (0,0) of bottom left corner)

 **		image size

 **		flags

 **		number of channels per image pixel

 **		number of bits in a channel

 **		number of channels per color map entry

 **		number of color map entries

 **

 **	See the imRleHeaderInfo comments below for more details.

 **

 **	Following the fixed size header is an optional background color

 **	for flooding the image prior to loading in pixel data.

 **

 **	Next is a variable length color map.

 **

 **	Next are zero or more comments.

 **

 **	Finally comes the image data.  Image data contains a variety of

 **	opcodes telling how to uncompress the data.  Opcodes and their

 **	operands come in one of two flavors:  short and long.

 **

 **	short form:	byte 0:   opcode

 **			byte 1:   operand

 **

 **	long form:	byte 0:   opcode

 **			byte 1:   unused

 **			byte 2-3: LBF operand

 **

 **	The long form is used if the operand is a value too large to fit

 **	in one byte.  The long form is flagged by the setting of the IMLONGOP

 **	bit in the opcode's byte.

 **

 **	The image opcodes control three state variables:

 **		x       = current X position on scanline

 **		y       = current Y position in image

 **		channel = current channel

 **

 **	x varies from 0 (left) to the image size - 1 (right).

 **

 **	y varies from 0 (bottom) to the image size - 1 (top).  Note that

 **	this is the opposite of VFB's where line 0 is the top line, not

 **	the bottom.

 **

 **	IMOPSetColor

 **		Select which channel of the image all following data is to

 **		be put into.  operand is the channel number.  By convention:

 **			0   = color index, or red

 **			1   = green

 **			2   = blue

 **			255 = alpha

 **		This also sets the current 'x' position in the image to 0.

 **

 **	IMOPSkipLines

 **		The current 'y' position is incremented by the operand.

 **		This also sets the current 'x' position in the image to 0.

 **

 **	IMOPSkipPixels

 **		The current 'x' position is incremented by the operand.

 **

 **	IMOPPixelData

 **		The operand is one less than the count of literal 1-byte

 **		pixel values that follow.  If that count (operand + 1) is

 **		odd, a single pad byte is added to bring the literal pixel

 **		run to a 16-bit boundary.  After processing the run of literal

 **		pixel values, the current 'x' position will be (operand+1)

 **		pixels further to the right.

 **

 **	IMOPRunData

 **		The operand is one less than the count of image pixels that

 **		should be filled with the single 1-byte pixel value that

 **		follows.  A single pad byte is added to bring things back to

 **		a 16-bit boundary.  After processing the run of identical pixel

 **		values, the current 'x' position will be (operand+1) pixels

 **		further to the right.

 **

 **	IMOPEOF

 **		The end of the file.  An I/O EOF also ends things.

 **

 **  USE OF BACKGROUND COLOR

 **	The background color scheme of RLE reasons that most images have a

 **	background color that occurs quite a bit.  Rather than represent

 **	that color as a run, disk space can be saved by using the IMOPSkipPixels

 **	opcode to skip over those background pixels.

 **

 **	If an incomming image has a background color, the IMRLE_CLEARFIRST bit

 **	will be set in the header's flags word, and the IMRLE_NOBACKGROUND bit

 **	not set.  The background color to use then follows the header.

 **

 **	One of two approaches may be used in handling background colors:

 **

 **		Initialize the image to the background color then treat

 **		IMOPSkipPixels as a skip.

 **

 **		Don't initialize the image but treat an IMOPSkipPixels as a

 **		fill with the background color.

 **

 **	We have chosen to do the first approach.

 **

 **  COLOR MAP STORAGE

 **	The color map is stored as a series of color channel's (red, green,

 **	blue).  With each of these color channels, the entry value is stored

 **	starting with entry 0.

 **

 **	Each color map entry value is 16-bits wide.  RLE documentation says

 **	the upper 8-bits should be used on systems that only support 8-bit

 **	color channels (virtually all current graphics hardware).  The lower

 **	8-bits should be ignored.

 **

 **	So, a color map is a series of nested structures that can be

 **	read out with looping like:

 **

 **		for each color channel (header's ncmap field),

 **			for each entry (header's cmaplen field sorta),

 **				entry value = short << 8

 **

 **	Note that the header gives the color map length by specifying the

 **	power of 2 to use.  So, for a 256 entry color map, cmaplen would be 8.

 **

 **  RESTRICTIONS

 **	The RLE format can support image configurations that don't really

 **	make sense, such as a 5-channel pixel image where each channel has

 **	a 42-channel color map.  Huh?

 **

 **	In the interests of simplicity and to reduce the RLE format to the

 **	common use case, we only support the following combinations:

 **

 **		1 channel per pixel

 **			with or without a 3-channel (RGB) color map

 **			with or without an alpha plane

 **			mapped to an IMVFBINDEX8 ( | IMVFBALPHA)

 **

 **		3 channels per pixel

 **			with or without a color map

 **			with or without an alpha plane

 **			mapped to an IMVFBRGB ( | IMVFBALPHA)

 **

 **	Any other combination is complained about and rejected.

 **

 **	When writing images, we do not set the IMRLE_CLEARFIRST bit in the header

 **	and do not worry about a background color.  To do so would require

 **	us to scan the VFB and make some sort of guess about what would be

 **	a good color to use as the background.  This is timeconsuming and

 **	really not all that worthwhile.  Consider:

 **

 **		An IMOPSkipPixels opcode/operand takes 2-4 bytes.

 **

 **		An IMOPRunData opcode/operand and pixel value takes 4-6 bytes.

 **

 **	We are talking about a savings of (hold your hats now), 2 bytes per

 **	use.  Sorry, not worth the effort or time of scanning the image first.

 **

 **	When writing color maps, VFB CLT's can be any length, but RLE CLT's

 **	are constrained to be a power of 2 in length.  So, when we have to

 **	write a color map bigger than the VFB's CLT, we fill in with black.

 **/



/*

 *  RLE - Utah Raster Toolkit run-length encoded image file

 *      For information on these structures, how to use them, etc. please

 *      see imfmt.c.

 */

#ifdef __STDC__

static int imRleRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imRleWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

#else

static int imRleRead( );

static int imRleWrite( );

#endif

static char *imRleNames[ ]  = { "rle", NULL };

static unsigned char imRleMagicNumber[ ] = { 0xCC, 0x52 };

static ImFileFormatReadMap imRleReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IN,1,8,       RLE,    IMVFBINDEX8,    0 },

        { IN,1,8,       RLE|C,  IMVFBINDEX8,    C },

        { IN,1,8,       RLE|A,  IMVFBINDEX8,    A },

        { IN,1,8,       RLE|C|A,IMVFBINDEX8,    C|A },

        { RGB,3,8,      RLE,    IMVFBRGB,       0 },

        { RGB,3,8,      RLE|A,  IMVFBRGB,       A },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imRleWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBINDEX8,  C,      IN,1,8,         RLE|C,  imRleWrite },

        { IMVFBINDEX8,  C|A,    IN,1,8,         RLE|C|A,imRleWrite },

        { IMVFBINDEX8,  A,      IN,1,8,         RLE|A,  imRleWrite },

        { IMVFBINDEX8,  0,      IN,1,8,         RLE,    imRleWrite },

        { IMVFBRGB,     0,      RGB,3,8,        RLE,    imRleWrite },

        { IMVFBRGB,     A,      RGB,3,8,        RLE|A,  imRleWrite },

        { -1,           0,      -1,             0,      NULL },

};



static ImFileMagic imFileRleMagic[]=

{

	{ 0, 2, imRleMagicNumber },

	{ 0, 0, NULL},

};



ImFileFormat ImFileRleFormat =

{

	imRleNames, "Utah Run length encoded image file",

	"Utah Raster Toolkit, University of Utah",

	"8-bit RLE-compressed color index image files, with or without CLT,\n\
with or without 8-bit Alpha plane.  24-bit RGB RLE-compressed image\n\
files, with or without 8-bit Alpha plane.",
	"8-bit RLE-compressed color index image files, with or without CLT,\n\
with or without 8-bit Alpha plane.  24-bit RGB RLE-compressed image\n\
files, with or without 8-bit Alpha plane.",
	imFileRleMagic,
	IMNOMULTI, IMPIPE,

	IMNOMULTI, IMPIPE,

	imRleRead, imRleReadMap, imRleWriteMap

};









/*

 *  TYPEDEF & STRUCT

 *	imRleHeaderInfo	-  RLE file header

 *

 *  DESCRIPTION

 *	RLE files start with a fixed size file header.

 *

 *	rle_magic is the RLE magic number, and is always 0xCC52.

 *

 *	rle_xpos, _ypos, _xsize, and _ysize give the position and size of

 *	the image.  We ignore the position, and use the size to determine

 *	how big a VFB to allocate.

 *

 *	rle_flags indicates how the background of the VFB should be treated.

 *

 *	rle_ncolors is the number of color channels saved, between 0 and 254.

 *	A color-index image would be 1, while an RGB image would be 3.

 *

 *	rle_pixelbits is the number of bits per pixel per color channel.

 *	This must be 8 (original Utah RLE code makes the same restriction).

 *

 *	rle_ncmap is the number of channels in the color map.  In most cases

 *	this will be 3, for R, G, and B.

 *

 *	rle_cmaplen is the log base 2 of the length of the color map for

 *	each channel.  For instance, for a 256 entry CLT, rle_cmaplen would

 *	be 8.

 *

 *  EXAMPLES

 *	An 8-bit color index image:

 *		rle_ncolors   = 1	-- just a CLT index

 *		rle_pixelbits = 8	-- 8-bits wide CLT index

 *		rle_ncmap     = 3	-- RGB for each CLT entry

 *		rle_cmaplen   = 8	-- 256 entries long

 *

 *	A 24-bit RGB image:

 *		rle_ncolors   = 3	-- R, G, and B per pixel

 *		rle_pixelbits = 8	-- 8-bits for R, for G, and for B

 *		rle_ncmap     = 0	-- No CLT

 *		rle_cmaplen   = 0	-- No CLT

 *

 *	A 24-bit RGB image with a CLT:

 *		rle_ncolors   = 3	-- R, G, and B per pixel

 *		rle_pixelbits = 8	-- 8-bits for R, for G, and for B

 *		rle_ncmap     = 3	-- RGB for each cLT entry

 *		rle_cmaplen   = 8	-- 256 entries long

 */



typedef struct imRleHeaderInfo

{

	unsigned short	rle_magic;		/* Magic number			*/

	unsigned short	rle_xpos;		/* X position of image		*/

	unsigned short	rle_ypos;		/* Y position of image		*/

	unsigned short	rle_xsize;		/* Width of image		*/

	unsigned short	rle_ysize;		/* Height of image		*/

	unsigned char	rle_flags;		/* Flags			*/

	unsigned char	rle_ncolors;		/* # of color channels saved	*/

	unsigned char	rle_pixelbits;		/* # of bits in a pixel		*/

	unsigned char	rle_ncmap;		/* # of color channels in CLT	*/

	unsigned char	rle_cmaplen;		/* Length of CLT		*/

} imRleHeaderInfo;



static BinField imRleHeaderFields[] =

{

	USHORT, 2, 1,			/* rle_magic			*/

	USHORT, 2, 1,			/* rle_xpos			*/

	USHORT, 2, 1,			/* rle_ypos			*/

	USHORT, 2, 1,			/* rle_xsize			*/

	USHORT, 2, 1,			/* rle_ysize			*/

	UCHAR, 1, 1,			/* rle_flags			*/

	UCHAR, 1, 1,			/* rle_ncolors			*/

	UCHAR, 1, 1,			/* rle_pixelbits		*/

	UCHAR, 1, 1,			/* rle_ncmap			*/

	UCHAR, 1, 1,			/* rle_cmaplen			*/

	0, 0, 0

};





/*

 *  imRleHeaderInfo field values and flags:

 */

#define	IMRLEMAGIC	(0xCC52)	/* RLE magic number		*/



#define IMRLE_CLEARFIRST      ( 0x1 )		/* If set, clear framebuffer	*/

#define IMRLE_NOBACKGROUND	( 0x2 )		/* If set, no bg color supplied	*/

#define	IMRLE_ALPHA		( 0x4 )		/* If set, alpha channel present*/

#define	IMRLE_COMMENT		( 0x8 )		/* If set, comments present	*/











/*

 *  Opcodes

 */

#define IMOPSkipLines	1

#define IMOPSetColor	2

#define IMOPSkipPixels	3

#define IMOPPixelData	5

#define IMOPRunData	6

#define IMOPEOF		7



#define IMLONGOP	0x40





/*

 *  Which channel we are reading into.

 */

#define IMONINDEX8	0

#define IMONRED		1

#define IMONGREEN	2

#define IMONBLUE	3

#define IMONALPHA	4











/*

 *  FUNCTION

 *	imRleRead	-  read an RLE file

 *

 *  DESCRIPTION

 *	The fixed size header is read in and the magic number checked.

 *	The header is also checked for reasonable and supported variations

 *	on number of image and color map channels.

 *

 *	If present, a background color is read in.

 *

 *	If present, a color map is read in.

 *

 *	If present, comments are read in and tossed.

 *

 *	If present, an image is read in and the various opcodes processed.

 */



static int			       /* Returns status		*/

#ifdef __STDC__

imRleRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRleRead( ioType, fd, fp, flagsTable, tagTable )

	int     ioType;			/* I/O flags			*/

	int     fd;			/* Input file descriptor	*/

	FILE   *fp;			/* Input file buffer pointer	*/

	TagTable *flagsTable;		/* Input parameter/flags	*/

	TagTable *tagTable;		/* Tag table to add to		*/

#endif

{

	imRleHeaderInfo header;		/* File header			*/

	char		msg[80];	/* Error message		*/

	unsigned char		bgColor[3];	/* Background color (if any)	*/

	int		fields;		/* Fields for VFB		*/

	unsigned int	        len;		/* Length of comments		*/

	int		i, j;		/* Counters			*/

	int		x, y;		/* Row and column counters	*/



	int		cmaplen;	/* Length of color map		*/

	unsigned short	       *cmap;		/* Color map buffer		*/

	unsigned short	       *pRed;		/* Pointer into color map buffer*/

	unsigned short	       *pGreen;		/* Pointer into color map buffer*/

	unsigned short	       *pBlue;		/* Pointer into color map buffer*/



	ImVfb	       *vfb;		/* New image			*/

	ImVfbPtr        pPixel;		/* Pointer to pixel in VFB	*/

	ImClt	       *clt;		/* New color table		*/

	ImCltPtr        pColor;		/* Pointer to color in CLT	*/



	unsigned char	       *buffer;		/* Generic buffer		*/

	unsigned char	       *pBuffer;	/* Buffer pointer		*/

	unsigned int		operand;	/* Operand of opcode		*/

	int		chan;		/* Current VFB channel		*/

	char		message[256];	/* buffer for use with ImInfo	*/





	/*

	 *  Read in the fixed size header and check for a good magic number

	 *  and reasonable header values.

	 */

	BinByteOrder( BINLBF );

	if ( ImBinReadStruct( ioType, fd, fp, &header, imRleHeaderFields) == -1)

	{

		ImReturnBinError();

	}

	if ( header.rle_magic != IMRLEMAGIC )

	{

		ImErrNo = IMEMAGIC;

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	if ( header.rle_pixelbits != 8 )

	{

		ImErrorFatal( "RLE images with other than 8-bit pixel channels are not supported", -1, IMESYNTAX );

	}



	/* following lines are printouts for Iminfo if imfile

		or inconv have the -verbose flags attached */

	ImInfo ("Byte Order", "Least Significant Byte First");

	sprintf (message, "%d x %d",header.rle_xsize,header.rle_ysize);

	ImInfo ("Resolution",message);



	if (header.rle_ncolors==1)

		sprintf (message, "%d-bit Color Indexed",header.rle_pixelbits);

	else if (header.rle_ncolors==3)

		sprintf (message, "%d-bit RGB",3*header.rle_pixelbits);

	ImInfo ("Type",message);



	if (header.rle_ncmap!=0)

	{

		sprintf (message, "%d Entries", header.rle_cmaplen);

		ImInfo ("Color Table", message);

	}



	ImInfo ("Compression Type","Run Length Encoded (RLE)");



	if (header.rle_flags & IMRLE_ALPHA)

		sprintf (message, "8-bit");

	else sprintf (message, "none");

	ImInfo ("Alpha Channel",message);



	switch ( header.rle_ncolors )

	{

	case 0:

		/*

		 *  No channels per pixel.  This means there is no image in

		 *  the file, but there might be a color map.  If so, we

		 *  read that in and we're done.

		 */

		fields = 0;

		switch ( header.rle_ncmap )

		{

		case 0:		/* No color map.  Nothing in file!	*/

			return ( 0 );

		case 3:		/* 3 color channels.  RGB CLT.		*/

			break;

		default:

			sprintf( msg, "RLE color maps with %d color channels are not supported", header.rle_ncmap );

			ImErrorFatal( msg, -1, IMESYNTAX );

		}

		break;



	case 1:

		/*

		 *  1 channel per pixel.  It is assumed this is a color index

		 *  image.  The color map can be non-existant, or have 3

		 *  color channels (RGB).  Anything else is weird.

		 */

		fields = IMVFBINDEX8;

		switch ( header.rle_ncmap )

		{

		case 0:		/* No color map.  Grayscale image.	*/

			break;

		case 3:		/* 3 color channels.  RGB CLT.		*/

			break;

		default:

			sprintf( msg, "RLE color maps with %d color channels are not supported", header.rle_ncmap );

			ImErrorFatal( msg, -1, IMESYNTAX );

		}

		break;



	case 3:

		/*

		 *  3 channels per pixel.  It is assumed this is an RGB image.

		 *  The color map must be non-existant.  Other combinations

		 *  are weird.

		 */

		fields = IMVFBRGB;

		switch ( header.rle_ncmap )

		{

		case 0:		/* No color map.  Normal RGB image.	*/

			break;

		case 3:		/* 3 color channels.  RGB CLT.		*/

			break;

		default:

			sprintf( msg, "RLE color maps with %d color channels are not supported", header.rle_ncmap );

			ImErrorFatal( msg, -1, IMESYNTAX );

		}

		break;



	default:

		/*

		 *  Strange number of channels.  Cannot support.

		 */

		sprintf( msg, "RLE images with %d channels are not supported",

			header.rle_ncolors );

		ImErrorFatal( msg, -1, IMESYNTAX );

	}





	/*

	 *  At this point we've narrowed things down to:

	 *	8-bit color index or 24-bit RGB image

	 *	Optional RGB CLT

	 */





	/*

	 *  Allocate a VFB if there is an image or alpha plane in the file.

	 */

	if ( header.rle_flags & IMRLE_ALPHA )

		fields |= IMVFBALPHA;

	vfb = IMVFBNULL;

	if ( fields != 0 )

	{

		vfb = ImVfbAlloc( header.rle_xsize, header.rle_ysize, fields);

		if ( vfb == IMVFBNULL )

		{

			ImErrorFatal( ImQError( ), -1, ImErrNo );

		}

	}







	/*

	 *  Read in the variable length background color following the

	 *  header.  There will be one 8-bit value for each channel in the

	 *  image (rle_ncolors).  We've already restricted our RLE handling to:

	 *

	 *	rle_ncolors = 0		no image, so no background color

	 *	rle_ncolors = 1		index image, 1 8-bit value

	 *	rle_ncolors = 3		RGB image, 3 8-bit values

	 *

	 *  For the rle_ncolors = 0 case, a single pad byte is present.

	 *

	 *  Clear the VFB to the given background color, if the IMRLE_CLEARFIRST

	 *  flag is set in the header.

	 *

	 *  The background color for the alpha plane, if present, is always 0.

	 */

	switch ( header.rle_ncolors )

	{

	case 0:

		if ( ImBinRead( ioType, fd, fp, bgColor, UCHAR, 1, 1 ) == -1 )

		{

			ImReturnBinError( );

		}

		break;			/* bgColor is pad byte		*/



	case 1:

		if ( ImBinRead( ioType, fd, fp, bgColor, UCHAR, 1, 1 ) == -1 )

		{

			ImVfbFree( vfb );

			ImReturnBinError( );

		}

		if ( header.rle_flags & IMRLE_NOBACKGROUND )

			break;		/* bgColor is pad byte		*/

		if ( !(header.rle_flags & IMRLE_CLEARFIRST) )

			break;		/* Nothing to do		*/

		if ( fields & IMVFBALPHA )

		{

			if ( bgColor[0] == 0 )

				ImVfbClear( IMVFBALL, bgColor[0], vfb );

			else

			{

				ImVfbClear( IMVFBINDEX8, bgColor[0], vfb );

				ImVfbClear( IMVFBALPHA, 0, vfb );

			}

		}

		else

			ImVfbClear( IMVFBALL, bgColor[0], vfb );

		break;



	case 3:

		if ( header.rle_flags & IMRLE_NOBACKGROUND )

		{

			if ( ImBinRead( ioType, fd, fp, bgColor, UCHAR, 1, 1 ) == -1 )

			{

				ImVfbFree( vfb );

				ImReturnBinError( );

			}

			break;		/* bgColor is pad byte		*/

		}

		if ( ImBinRead( ioType, fd, fp, bgColor, UCHAR, 1, 3 ) == -1 )

		{

			ImVfbFree( vfb );

			ImReturnBinError( );

		}

		if ( !(header.rle_flags & IMRLE_CLEARFIRST) )

			break;		/* Nothing to do		*/

		if ( bgColor[0] == bgColor[1] && bgColor[0] == bgColor[2] )

		{

			if ( fields & IMVFBALPHA )

			{

				if ( bgColor[0] == 0 )

					ImVfbClear( IMVFBALL, bgColor[0], vfb );

				else

				{

					ImVfbClear( IMVFBRGB, bgColor[0], vfb );

					ImVfbClear( IMVFBALPHA, 0, vfb );

				}

			}

			else

				ImVfbClear( IMVFBALL, bgColor[0], vfb );

			break;

		}

		if ( fields & IMVFBALPHA )

			ImVfbClear( IMVFBALPHA, 0, vfb );

		pPixel = ImVfbQFirst( vfb );

		for ( y = 0; y < header.rle_ysize; y++ )

		{

			for ( x = 0; x < header.rle_xsize; x++ )

			{

				ImVfbSRed(   vfb, pPixel, bgColor[0] );

				ImVfbSGreen( vfb, pPixel, bgColor[1] );

				ImVfbSBlue(  vfb, pPixel, bgColor[2] );

				ImVfbSInc(   vfb, pPixel );

			}

		}

		break;

	}





	/*

	 *  Read in the variable length color map.  We constrain things to:

	 *

	 *	rle_ncmap = 0	no color map

	 *	rle_ncmap = 3	RGB color map

	 */

	clt = IMCLTNULL;

	if ( header.rle_ncmap == 3 )

	{

		/*

		 *  Color map entries are 16-bit values.  When mapping to

		 *  display hardware (or a VFB's CLT), the upper bits are

		 *  the most useful.  Since we limit CLT color channel

		 *  values to 8-bits each, we shift each color map entry

		 *  to the right by 8 bits, and use what's left.

		 *

		 *  Color map values are stored in the order:

		 *	RRR...GGG...BBB...

		 */

		cmaplen = 1 << header.rle_cmaplen;

		ImMalloc( cmap, unsigned short *, sizeof( ushort ) * cmaplen * 3 );

		if ( ImBinRead( ioType, fd, fp, cmap, USHORT, 2, cmaplen * 3 )== -1 )

		{

			free( (char *)cmap );

			if ( vfb != IMVFBNULL )

				ImVfbFree( vfb );

			ImReturnBinError( );

		}



		if ( (clt = ImCltAlloc( cmaplen )) == IMCLTNULL )

		{

			free( (char *)cmap );

			if ( vfb != IMVFBNULL )

				ImVfbFree( vfb );

			ImErrorFatal( ImQError( ), -1, ImErrNo );

		}

		pColor = ImCltQFirst( clt );

		pRed   = cmap;

		pGreen = pRed   + cmaplen;

		pBlue  = pGreen + cmaplen;



		for ( i = 0; i < cmaplen; i++ )

		{

			ImCltSRed(   pColor, ((*pRed++   >> 8)) & 0xFF );

			ImCltSGreen( pColor, ((*pGreen++ >> 8)) & 0xFF );

			ImCltSBlue(  pColor, ((*pBlue++  >> 8)) & 0xFF );

			ImCltSInc(   clt, pColor );

		}

		free( (char *) cmap );



		ImVfbSClt( vfb, clt );

		TagTableAppend( tagTable,

			TagEntryAlloc( "image clt", POINTER, &clt ) );

	}





	/*

	 *  Read in and ignore comments, if present.

	 */

	if ( header.rle_flags & IMRLE_COMMENT )

	{

		/*

		 *  Comments are preceded by a 2-byte value giving the

		 *  length, in bytes, of the comments.  If this length

		 *  is odd, an extra pad byte is added.

		 *

		 *  We don't handle these comments, so we just toss them.

		 */



		if ( ImBinRead( ioType, fd, fp, &len, UINT, 2, 1 )== -1 )

		{

			if ( vfb != IMVFBNULL )

				ImVfbFree( vfb );

			ImReturnBinError( );

		}

		len = (len + 1) & ~0x1;	/* Round to 16-bits		*/



		/*

		 *  If reading from a file, just seek past the comments.

		 *  Otherwise we have to read them in and toss them.

		 */

		if ( ioType & IMFILEIOFILE )

			ImSeek( ioType, fd, fp, len, 1 );

		else

		{

			ImMalloc( buffer, unsigned char *, len );

			if ( ImBinRead( ioType, fd, fp, buffer, UCHAR, 1, len )== -1 )

			{

				free( (char *)buffer );

				if ( vfb != IMVFBNULL )

					ImVfbFree( vfb );

				ImReturnBinError( );

			}

			free( (char *)buffer );

		}

	}





	/*

	 *  If there is no image, stop.

	 */

	if ( header.rle_ncolors == 0 && !(header.rle_flags & IMRLE_ALPHA) )

		return ( (clt == IMCLTNULL) ? 0 : 1 );





	/*

	 *  Read in the image.  Each two byte opcode/operand pair is read

	 *  in and checked to see if the opcode has its "long form" bit set.

	 *  If so, the byte paired with the opcode is ignored and another

	 *  two bytes read in and treated as a 16-bit operand.

	 *

	 *  The opcode is then processed.  IMOPSkipLines and IMOPSkipPixels both

	 *  skip over pixels, setting nothing.  IMOPSetColor switches to a

	 *  different channel in the image.  IMOPPixelData has literal pixel

	 *  values for the current channel, while IMOPRunData has a run of a

	 *  single pixel value for the current channel.

	 *

	 *  Encountering file EOF, or the IMOPEOF opcode stops the image read.

	 *

	 *  NOTE:  VFB's have (0,0) in the upper-left.  RLE images have it

	 *  in the lower left.  So, we walk from the bottom of the image, up,

	 *  as we read it in.

	 */

	i = (header.rle_xsize + 1) & ~0x1;

	ImMalloc( buffer, unsigned char *, i * sizeof( unsigned char ) );



	x = 0;

	y = header.rle_ysize - 1;

	switch ( header.rle_ncolors )

	{

	case 0:	chan = IMONALPHA; break;

	case 1:	chan = IMONINDEX8; break;

	case 3:	chan = IMONRED; break;

	}



	for ( ; ; )

	{

		switch ( (j = ImBinRead( ioType, fd, fp, buffer, UCHAR, 1, 2 )))

		{

		case -1:	/* Error				*/

			free( (char *)buffer );

			ImVfbFree( vfb );

			ImReturnBinError( );



		case 0:		/* EOF					*/

			free( (char *)buffer );

			TagTableAppend( tagTable,

				TagEntryAlloc( "image vfb", POINTER, &vfb ) );

			return ( 1 );

		}

		operand = buffer[1];

		if ( buffer[0] & IMLONGOP )

		{

			if ( ImBinRead( ioType, fd, fp, &operand, UINT, 2, 1 ) == -1 )

			{

				free( (char *)buffer );

				ImVfbFree( vfb );

				ImReturnBinError( );

			}

		}



		switch ( buffer[0] )

		{

		case IMOPSkipLines | IMLONGOP:

		case IMOPSkipLines:

			/* Skip to the start of the next line.		*/

			y -= operand;

			x = 0;

			continue;



		case IMOPSetColor:

			/* Start on a different channel.		*/

			x = 0;

			switch ( operand )

			{

			case 0:	chan = (header.rle_ncolors==1)?IMONINDEX8:IMONRED;

				break;

			case 1:	chan = IMONGREEN; break;

			case 2:	chan = IMONBLUE; break;

			case 255:chan = IMONALPHA; break;

			default:

				free( (char *)buffer );

				ImVfbFree( vfb );

				sprintf( msg, "RLE image data selects channel %d, which doesn't exist!", operand );

				ImErrorFatal( msg, -1, IMESYNTAX );

			}

			continue;



		case IMOPSkipPixels | IMLONGOP:

		case IMOPSkipPixels:

			/* Skip forward on the scanline.		*/

			x += operand;

			continue;



		case IMOPPixelData | IMLONGOP:

		case IMOPPixelData:

			/* Read in literal pixel values.		*/

			operand++;

			i = (operand + 1) & ~0x1;

			if ( ImBinRead( ioType, fd, fp, buffer, UCHAR, 1, i ) == -1 )

			{

				free( (char *)buffer );

				ImVfbFree( vfb );

				ImReturnBinError( );

			}

			pBuffer = buffer;

			pPixel  = ImVfbQPtr( vfb, x, y );

			x      += operand;

			switch ( chan )

			{

			case IMONINDEX8:

				for ( i = 0; i < operand; i++ )

				{

					ImVfbSIndex8( vfb, pPixel, *pBuffer++ );

					ImVfbSInc( vfb, pPixel );

				}

				break;

			case IMONRED:

				for ( i = 0; i < operand; i++ )

				{

					ImVfbSRed( vfb, pPixel, *pBuffer++ );

					ImVfbSInc( vfb, pPixel );

				}

				break;

			case IMONGREEN:

				for ( i = 0; i < operand; i++ )

				{

					ImVfbSGreen( vfb, pPixel, *pBuffer++ );

					ImVfbSInc( vfb, pPixel );

				}

				break;

			case IMONBLUE:

				for ( i = 0; i < operand; i++ )

				{

					ImVfbSBlue( vfb, pPixel, *pBuffer++ );

					ImVfbSInc( vfb, pPixel );

				}

				break;

			case IMONALPHA:

				for ( i = 0; i < operand; i++ )

				{

					ImVfbSAlpha( vfb, pPixel, *pBuffer++ );

					ImVfbSInc( vfb, pPixel );

				}

				break;

			}

			continue;



		case IMOPRunData | IMLONGOP:

		case IMOPRunData:

			/* Read in and process a run of the same pixel value*/

			if ( ImBinRead( ioType, fd, fp, buffer, UCHAR, 1, 2 ) == -1 )

			{

				free( (char *)buffer );

				ImVfbFree( vfb );

				ImReturnBinError( );

			}

			operand++;

			pPixel  = ImVfbQPtr( vfb, x, y );

			x      += operand;

			switch ( chan )

			{

			case IMONINDEX8:

				for ( i = 0; i < operand; i++ )

				{

					ImVfbSIndex8( vfb, pPixel, *buffer );

					ImVfbSInc( vfb, pPixel );

				}

				break;

			case IMONRED:

				for ( i = 0; i < operand; i++ )

				{

					ImVfbSRed( vfb, pPixel, *buffer );

					ImVfbSInc( vfb, pPixel );

				}

				break;

			case IMONGREEN:

				for ( i = 0; i < operand; i++ )

				{

					ImVfbSGreen( vfb, pPixel, *buffer );

					ImVfbSInc( vfb, pPixel );

				}

				break;

			case IMONBLUE:

				for ( i = 0; i < operand; i++ )

				{

					ImVfbSBlue( vfb, pPixel, *buffer );

					ImVfbSInc( vfb, pPixel );

				}

				break;

			case IMONALPHA:

				for ( i = 0; i < operand; i++ )

				{

					ImVfbSAlpha( vfb, pPixel, *buffer );

					ImVfbSInc( vfb, pPixel );

				}

				break;

			}

			continue;



		case IMOPEOF:

			free( (char *)buffer );

			TagTableAppend( tagTable,

				TagEntryAlloc( "image vfb", POINTER, &vfb ) );

			return ( 1 );



		default:

			free( (char *)buffer );

				ImVfbFree( vfb );

			sprintf( msg, "Unknown RLE image opcode:  0x%02x",

				buffer[0] );

			ImErrorFatal( msg, -1, IMESYNTAX );

		}

	}

}











/*

 *  MACROS

 *	IMDumpRun	-  dump a run of identical pixels

 *	IMDumpLitRun	-  dump a run of literal pixels

 *

 *  DESCRIPTIONS

 *	Both of these macros assume a variety of local variables.  These

 *	algorithms are encapsulated in macros in order to simplify the

 *	write routines below.  They are *not* made into subroutines to

 *	avoid the extra expense of subroutine calls, argument passing, and

 *	global variable referencing that that would entail.

 *

 *	IMDumpRun outputs a RunData opcode in either short or long form,

 *	followed by a single byte of pixel value for the pixel value to run

 *	across several image pixels.  A pad byte is added to bring the

 *	opcode and run operand to a 16-bit boundary.

 *

 *	IMDumpLitRun outputs a PixelData opcode in either short or long form,

 *	followed by a string of literal pixel values to write into adjacent

 *	image pixels.  If the number of pixel values is odd, a pad byte is

 *	added to bring the opcode and literal run to a 16-bit boundary.

 */



#define IMDumpRun()							\
{									\
	/*								\
	 *  Write opcode (possibly in long form).			\
	 *  Operand is runtCount - 1.					\
	 */								\
	if ( --runCount <= 255 )					\
	{								\
		command[0] = IMOPRunData;				\
		command[1] = runCount;					\
		if ( ImBinWrite( ioType, fd, fp, command,		\
			UCHAR, 1, 2 ) == -1 )				\
		{							\
			free( (char *)buffer );				\
			ImReturnBinError();				\
		}							\
	}								\
	else								\
	{								\
		command[0] = IMOPRunData | IMLONGOP;			\
		command[1] = 0;						\
		if ( ImBinWrite( ioType, fd, fp, command,		\
			UCHAR, 1, 2 ) == -1 )				\
		{							\
			free( (char *)buffer );				\
			ImReturnBinError();				\
		}							\
		if ( ImBinWrite( ioType, fd, fp, &runCount,		\
			UINT, 2, 1 ) == -1 )				\
		{							\
			free( (char *)buffer );				\
			ImReturnBinError();				\
		}							\
	}								\
									\
	/*								\
	 *  Output single run byte, plus pad byte to round to		\
	 *  next highest 16-bit boundary.				\
	 */								\
	buffer[1] = 0;					/* Pad	*/	\
	if ( ImBinWrite( ioType, fd, fp, buffer, UCHAR, 1, 2 ) == -1 )	\
	{								\
		free( (char *)buffer );					\
		ImReturnBinError();					\
	}								\
}



#define IMDumpLitRun()							\
{									\
	/*								\
	 *  Write opcode (possibly in long form).			\
	 *  Operand is litCount - 1.					\
	 */								\
	if ( --litCount <= 255 )					\
	{								\
		command[0] = IMOPPixelData;				\
		command[1] = litCount;					\
		if ( ImBinWrite( ioType, fd, fp,			\
			command, UCHAR, 1, 2 ) == -1 )			\
		{							\
			free( (char *)buffer );				\
			ImReturnBinError();				\
		}							\
	}								\
	else								\
	{								\
		command[0] = IMOPPixelData | IMLONGOP;			\
		command[1] = 0;						\
		if ( ImBinWrite( ioType, fd, fp,			\
			command, UCHAR, 1, 2 ) == -1 )			\
		{							\
			free( (char *)buffer );				\
			ImReturnBinError();				\
		}							\
		if ( ImBinWrite( ioType, fd, fp, &litCount,		\
			UINT, 2, 1 ) == -1 )				\
		{							\
			free( (char *)buffer );				\
			ImReturnBinError();				\
		}							\
	}								\
									\
	/*								\
	 *  Output buffer built up so far.  Pad to			\
	 *  next highest 16-bit boundary.				\
	 */								\
	if ( ImBinWrite( ioType, fd, fp, buffer,			\
		UCHAR, 1, (litCount + 2) & ~0x1 ) == -1)		\
	{								\
		free( (char *)buffer );					\
		ImReturnBinError();					\
	}								\
}











/*

 *  FUNCTION

 *	imRleWrite	-  write an RLE file

 *

 *  DESCRIPTION

 *	This same routine handles INDEX8 and RGB VFB's, with or without

 *	alpha channels, and with or without CLT's.

 *

 *	The file header is set up and written out.

 *

 *	No background color.

 *

 *	A color map is constructed and written out, if necessary.

 *

 *	No comments.

 *

 *	The image is processed one scanline at a time, run-length encoding

 *	each channel and writing it out.

 */



static int

#ifdef __STDC__

imRleWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRleWrite( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int     ioType;			/* I/O flags			*/

	int     fd;			/* Output file descriptor	*/

	FILE   *fp;			/* Output file buffer pointer	*/

	TagTable *flagsTable;		/* Output parameter/flags	*/

	TagTable *tagTable;		/* Tag table to read from	*/

#endif

{

	imRleHeaderInfo header;		/* File header			*/

	int	  i, j, k;		/* Counters			*/

	int       x, y;			/* Row and column counters	*/



	ImVfb    *vfb;			/* Image to output		*/

	ImVfbPtr  pPixel;		/* VFB pixel pointer		*/

	int       fields;		/* VFB fields			*/

	unsigned char    *buffer;		/* Pixel buffer			*/

	unsigned char    *pBuffer;		/* Buffer pointer		*/

	unsigned char     command[4];		/* Command buffer		*/



	ImClt    *clt;			/* CLT to output		*/

	ImCltPtr  pColor;		/* CLT color pointer		*/

	int	  cltLen;		/* Length of CLT		*/

	int	  cltPadLen;		/* Length of Pad entries for CLT*/

	unsigned short   *cmap;			/* Color map buffer		*/

	unsigned short   *pRed;			/* Pointer into color map buffer*/

	unsigned short   *pGreen;		/* Pointer into color map buffer*/

	unsigned short   *pBlue;		/* Pointer into color map buffer*/



	int       index;		/* Current color index		*/

	int       oldindex;		/* Old color index		*/

	unsigned int	  runCount;		/* Run count			*/

	ImVfbPtr  pLitPixel;		/* Literal pixel pointer	*/

	unsigned int	  litCount;		/* Literal pixel count		*/

	char	  message[256];		/* buffer to be printed in ImInfo */

	



	/*

	 *  Set up and write out the file header.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	fields = ImVfbQFields( vfb );

	clt    = ImVfbQClt( vfb );

	header.rle_magic = IMRLEMAGIC;

	header.rle_xpos  = 0;

	header.rle_ypos  = 0;

	header.rle_xsize = ImVfbQWidth( vfb );

	header.rle_ysize = ImVfbQHeight( vfb );

	header.rle_flags = IMRLE_NOBACKGROUND;

	if ( (fields & IMVFBALPHA) && (pMap->map_outAttributes & IMALPHAYES) )

		header.rle_flags |= IMRLE_ALPHA;

	header.rle_pixelbits = 8;

	if ( fields & IMVFBINDEX8 )

	{

		header.rle_ncolors   = 1;

		if ( (clt != IMCLTNULL) && (pMap->map_outAttributes & IMCLTYES))

		{

			/*

			 *  Compute the next higher power of 2 to use for

			 *  the number of CLT entries.

			 */

			cltLen = ImCltQNColors( clt );

			for ( j = 0, i = cltLen; i > 1; j++ )

				i >>= 1;

			if ( (1 << j) < cltLen )

				j++;

			cltPadLen = (1 << j) - cltLen;

			header.rle_cmaplen = j;

			header.rle_ncmap   = 3;

		}

		else

		{

			/*

			 *  Don't output a color map.

			 */

			header.rle_cmaplen = 0;

			header.rle_ncmap   = 0;

		}

	}

	else

	{

		header.rle_ncolors = 3;

		header.rle_cmaplen = 0;

		header.rle_ncmap   = 0;

	}

	BinByteOrder( BINLBF );

	if ( ImBinWriteStruct( ioType, fd, fp, &header, imRleHeaderFields)== -1)

	{

		ImReturnBinError();

	}



        /* following lines are printouts for Iminfo if imfile

                or inconv have the -verbose flags attached */

        ImInfo ("Byte Order", "Least Significant Byte First");

        sprintf (message, "%d x %d",header.rle_xsize,header.rle_ysize);

        ImInfo ("Resolution",message);



        if (header.rle_ncolors==1)

                sprintf (message, "%d-bit Color Indexed",header.rle_pixelbits);

        else if (header.rle_ncolors==3)

                sprintf (message, "%d-bit RGB",3*header.rle_pixelbits);

        ImInfo ("Type",message);



        if (header.rle_ncmap!=0)

        {

                sprintf (message, "%d Entries", header.rle_cmaplen);

                ImInfo ("Color Table", message);

        }



        ImInfo ("Compression Type","Run Length Encoded (RLE)");



        if (header.rle_flags & IMRLE_ALPHA)

                sprintf (message, "8-bit");

        else sprintf (message, "none");

        ImInfo ("Alpha Channel",message);







	/*

	 *  No need for a background color because we set the IMRLE_NOBACKGROUND

	 *  bit in the header flags word.  Just output a pad byte to bring

	 *  the header to a 16-bit boundary.

	 */

	i = 0;

	if ( ImBinWrite( ioType, fd, fp, &i, INT, 1, 1 ) == -1 )

	{

		ImReturnBinError();

	}





	/*

	 *  Write out the CLT.  The red, then green, then blue channels, in

	 *  their entirity, are written out one at a time.  Each channel

	 *  value is shifted into the high byte of a 16-bit word.  The lower

	 *  byte is set to zeroes.

	 *

	 *  Since the RLE header requires that CLT's be a power of 2 in length,

	 *  but VFB's don't, we might have a CLT that is shorter than a

	 *  power of 2.  In such cases a batch of "pad" black CLT entries are

	 *  output.

	 */

	if ( header.rle_ncmap != 0 )

	{

		k = cltLen + cltPadLen;

		ImMalloc( cmap, unsigned short *, sizeof( ushort ) * 3 * k );



		pColor = ImCltQFirst( clt );

		pRed   = cmap;

		pGreen = pRed   + k;

		pBlue  = pGreen + k;



		for ( i = 0; i < cltLen; i++ )

		{

			*pRed++   = (ImCltQRed(   pColor ) << 8);

			*pGreen++ = (ImCltQGreen( pColor ) << 8);

			*pBlue++  = (ImCltQBlue(  pColor ) << 8);

			ImCltSInc( clt, pColor );

		}



		for ( i = 0; i < cltPadLen; i++ )

		{

			*pRed++   = 0;

			*pGreen++ = 0;

			*pBlue++  = 0;

		}



		if ( ImBinWrite( ioType, fd, fp, cmap, USHORT, 2, 3 * k ) == -1)

		{

			free( (char *)cmap );

			ImReturnBinError();

		}

		free( (char *)cmap );

	}





	/*

	 *  We left the IMRLE_COMMENT flag off in the header, so there are no

	 *  comments in the file.

	 */



	/*

	 *  Output the image.  Process pixels from the bottom of the image, up.

	 */

	ImMalloc( buffer, unsigned char *, (header.rle_xsize + 1) & ~1 );



	/* Process each scanline.					*/

	for ( y = header.rle_ysize - 1; y >= 0; y-- )

	{

		if ( fields & IMVFBRGB )

			k = 3;

		else

			k = 1;

		if ( header.rle_flags & IMRLE_ALPHA )

			k++;

		for ( j = 0; j < k; j++ )

		{

			if ( j == 1 && (fields & IMVFBINDEX8) )

				j = 3;



			/* Select channel.				*/

			command[0] = IMOPSetColor;

			if ( j == 3 )

				command[1] = 255;	/* Alpha	*/

			else

				command[1] = j;

			if ( ImBinWrite( ioType, fd, fp, command,

				UCHAR, 1, 2 ) == -1 )

			{

				ImReturnBinError();

			}



			pPixel   = ImVfbQPtr( vfb, 0, y );

			runCount = 0;

			litCount = 1;

			switch ( j )

			{

			case 0:	if ( fields & IMVFBINDEX8 )

					oldindex = ImVfbQIndex8( vfb, pPixel );

				else

					oldindex = ImVfbQRed( vfb, pPixel );

				break;

			case 1:	oldindex = ImVfbQGreen( vfb, pPixel ); break;

			case 2:	oldindex = ImVfbQBlue(  vfb, pPixel ); break;

			case 3:	oldindex = ImVfbQAlpha( vfb, pPixel ); break;

			}

			pBuffer    = buffer;

			*pBuffer++ = oldindex;

			ImVfbSInc( vfb, pPixel );



			for ( x = 1; x < header.rle_xsize; x++ )

			{

				switch ( j )

				{

				case 0:	if ( fields & IMVFBINDEX8 )

						index=ImVfbQIndex8( vfb,pPixel);

					else

						index=ImVfbQRed( vfb, pPixel );

					break;

				case 1:	index=ImVfbQGreen( vfb, pPixel ); break;

				case 2:	index=ImVfbQBlue(  vfb, pPixel ); break;

				case 3:	index=ImVfbQAlpha( vfb, pPixel ); break;

				}

				ImVfbSInc( vfb, pPixel );



				if ( index == oldindex )

				{

					if ( ++runCount != 1 )

						continue;	/* Cont run*/



					/*

					 *  Start of run.  Dump previous batch

					 *  of literal pixels, if any.

					 */

					runCount++;

					if ( --litCount == 0 )

						continue;	/* No lit*/

					IMDumpLitRun( );

					pBuffer    = buffer;

					*pBuffer++ = index;

					litCount   = 0;

					continue;

				}

				oldindex = index;





				if ( ++litCount != 1 )

				{

					*pBuffer++ = index;

					continue;	/* Cont literal run*/

				}



				/*

				 *  Start of literal run.  Dump previous batch

				 *  of run pixels.

				 */

				IMDumpRun( );

				pBuffer    = buffer;

				*pBuffer++ = index;

				runCount   = 0;

			}

			if ( litCount != 0 )

			{

				IMDumpLitRun( );

			}

			else if ( runCount != 0 )

				IMDumpRun( );

		}

		command[0] = IMOPSkipLines;

		command[1] = 1;

		if ( ImBinWrite( ioType, fd, fp, command, UCHAR, 1, 2 ) == -1 )

		{

			ImReturnBinError();

		}

	}



	free( (char *)buffer );

	command[0] = IMOPEOF;

	command[1] = 0;

	if ( ImBinWrite( ioType, fd, fp, command, UCHAR, 1, 2 ) == -1 )

	{

		ImReturnBinError();

	}

	return ( 1 );

}

