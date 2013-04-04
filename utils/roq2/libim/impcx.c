/**

 **	$Header: /roq/libim/impcx.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER        "    $Header: /roq/libim/impcx.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	impcx.c		-  ZSoft IBM PC Paintbrush image file I/O

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	impcx.c contains routines to read and write ZSoft PCX files for

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

 **	imPcxRead	f  read an PCX file

 **	imPcxWrite1	f  write a 1-bit PCX file

 **	imPcxWrite8	f  write an 8-bit PCX file

 **

 **  PRIVATE CONTENTS

 **	imPcxHeaderInfo		t  header for PCX files

 **	imPcxHeaderFields	v  header description for binary I/O package

 **

 **  HISTORY

 **	$Log: /roq/libim/impcx.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.12  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.11  1995/06/15  20:48:58  bduggan

 **	Removed useless ++.  Changed bzero to memset.

 **

 **	Revision 1.10  1995/04/03  21:32:34  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.9  1995/01/10  23:36:33  bduggan

 **	put in IMMULTI, IMPIPE instead of TRUE/FALSE

 **	uncapitlized i's in local functions

 **	made read/write routines static

 **

 **	Revision 1.8  94/10/03  11:30:21  nadeau

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

 **	Revision 1.7  92/12/03  01:50:23  nadeau

 **	Corrected info messages.

 **	

 **	Revision 1.6  92/11/23  18:42:43  nadeau

 **	Removed use of IMINFOMSG.

 **	

 **	Revision 1.5  92/11/04  12:12:37  groening

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.4  92/09/29  18:00:29  vle

 **	Added ImInfo messages.

 **	

 **	Revision 1.3  92/08/31  17:30:11  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.2  91/10/03  09:15:18  nadeau

 **	Added undocumented PC PaintBrush CLT type.

 **	

 **	Revision 1.1  91/09/17  20:16:28  nadeau

 **	Initial revision

 **	

 **/



#include "iminternal.h"





/**

 **	$Header: /roq/libim/impcx.c 1     11/02/99 4:38p Zaphod $

 **	Copyright (c) 1989-1994  San Diego Supercomputer Center (SDSC)

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



#define HEADER        "    $Header: /roq/libim/impcx.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	impcx.c		-  ZSoft IBM PC Paintbrush image file I/O

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	impcx.c contains routines to read and write ZSoft PCX files for

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

 **	done

 **

 **  PRIVATE CONTENTS

 **	imPcxRead	f  read an PCX file

 **	imPcxWrite1	f  write a 1-bit PCX file

 **	imPcxWrite8	f  write an 8-bit PCX file

 **	imPcxHeaderInfo		t  header for PCX files

 **	imPcxHeaderFields	v  header description for binary I/O package

 **

 **  HISTORY

 **	$Log: /roq/libim/impcx.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.12  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.11  1995/06/15  20:48:58  bduggan

 **	Removed useless ++.  Changed bzero to memset.

 **

 **	Revision 1.10  1995/04/03  21:32:34  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.9  1995/01/10  23:36:33  bduggan

 **	put in IMMULTI, IMPIPE instead of TRUE/FALSE

 **	uncapitlized i's in local functions

 **	made read/write routines static

 **

 **	Revision 1.8  94/10/03  11:30:21  nadeau

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

 **	Revision 1.7  92/12/03  01:50:23  nadeau

 **	Corrected info messages.

 **	

 **	Revision 1.6  92/11/23  18:42:43  nadeau

 **	Removed use of IMINFOMSG.

 **	

 **	Revision 1.5  92/11/04  12:12:37  groening

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.4  92/09/29  18:00:29  vle

 **	Added ImInfo messages.

 **	

 **	Revision 1.3  92/08/31  17:30:11  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.2  91/10/03  09:15:18  nadeau

 **	Added undocumented PC PaintBrush CLT type.

 **	

 **	Revision 1.1  91/09/17  20:16:28  nadeau

 **	Initial revision

 **	

 **/



#include "iminternal.h"









/**

 **  FORMAT

 **	PCX	-  ZSoft IBM PC Paintbrush image

 **

 **  AKA

 **	pcc

 **

 **  FORMAT REFERENCES

 **	- Technical Reference Manual for PC Paintbrush et al, ZSoft Corp.

 **	- Bit-Mapped Graphics, Steve Rimmer

 ** 	- Supercharged Bitmapped Graphics, Steve Rimmer

 **	- Graphics File Formats, David C. Kay, John R. Levine

 ** 	- HP LaserJet Programming, Andrew Binstock, David Babcock, Marv Luse

 **

 **  CODE CREDITS

 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1991.

 **

 **  DESCRIPTION

 **	ZSoft's PCX format is used by a variety of IBM PC software, including

 **	ZSoft's own Publisher's Paintbrush, PC Paintbrush Plus, and PC

 **	Paintbrush.

 **

 **	This code supports the format as described in:

 **

 **		Technical Reference Manual

 **		ZSoft Corporation

 **		450 Franklin Rd. Suite 100

 **		Marietta, GA  30067

 **		(404) 428-0008

 **

 **	This report is dated 1988.

 **

 **	Additional format information was obtained from the book "Bit-Mapped

 **	Graphics" by Steve Rimmer and published by WINDCREST/McGraw-Hill.

 **

 **	PCX files start with a fixed-length 128-byte header consisting of

 **	the following:

 **

 **	Byte	Item		Size	Description/Comments

 **

 **	0	Manufacturer	1	10 = ZSoft .PCX

 **

 **	1	Version		1	Version #

 **						0 = Version 2.5

 **						2 = Version 2.8 w/palette info

 **						3 = Version 2.8 wo/palette info

 **						5 = Version 3.0

 **

 **	2	Encoding	1	1 = .PCX run length encoding

 **

 **	3	Bits per pixel	1	# of bits/pixel per plane

 **						1 = monochrome (CGA)

 **						4 = 16-color   (EGA)

 **						8 = 256-color  (VGA)

 **

 **	4	Window		4*2	Picture dimensions

 **						xmin

 **						ymin

 **						xmax

 **						ymax

 **

 **	12	Horizontal Res	2	Horizontal resolution of dev, in pixels

 **

 **	14	Vertical Res	2	Vertical resolution of dev, in pixels

 **

 **	16	Colormap	16*3	Basic color table

 **

 **	64	Reserved	1

 **

 **	65	No. Planes	1	# of color planes

 **

 **	66	Bytes per line	2	# of bytes per scanline per color

 **					color plane (always even)

 **

 **	68	Colormap type	2	Colormap type

 **						1 = color/BW

 **						2 = grayscale

 **

 **	70	Filler		58	Blanks

 **

 **	All 2-byte integers are given in Intel byte order (LBF).

 **

 **	Image data immediately follows the header.  In version 6.0 PCX files,

 **	an optional 256-entry color palette may follow the image data (but

 **	see below).

 **

 **  HEADER DETAILS

 **	The 'Manufacturer' field is always '10' for PCX files.  ZSoft has never

 **	introduced further 'Manufacturer' codes.  The SDSC Image Tools use

 **	the 'Manufacturer' field as the file's magic number.

 **

 **	The 'Encoding' field is always '1'.  ZSoft has never introduced other

 **	encoding schemes (which it should have, since this one is particularly

 **	stupid).  ZSoft does not support unencoded images in PCX files.

 **

 **	The 'Window' field is used to compute the image dimensions using:

 **

 **		width  = xmax - xmin + 1;

 **		height = ymax - ymin + 1;

 **

 **	Note the '+1' needed on both of these.  Just a quirk of PCX files.

 **	On write, the SDSC Image Tools set the 'xmin' and 'ymin' fields to 0,

 **	and the 'xmax' and 'ymax' fields to the width and height, minus 1.

 **

 **	The 'Horizontal Res' and 'Vertical Res' fields are irrelevant.

 **	SDSC Image Tools code ignores these on read, and sets them to 0's

 **	on write.

 **

 **		NOTE:  The popular UNIX PBM+ tools set the horizontal and

 **		vertical device resolution to the image size on write, but

 **		ignore them on read.  This is incorrect.  The resolution fields

 **		are defined by the spec to be HARDWARE resolution, not image

 **		resolution.  We feel that setting them to zeroes is less

 **		incorrect than setting them to some value.

 **

 **	The 'Reserved' and 'Filler' fields are always ignored by us on

 **	read, and always set to 0's on write.

 **

 **	The remaining fields have specific combinations of values depending

 **	upon the type of image being stored.

 **

 **  MONOCHROME IMAGES

 **	Monochrome PCX files were intended for the original CGA card as well

 **	as single-plane use of the EGA card.

 **

 **	Header values set to the following flag monochrome images:

 **

 **		Version		= 0, 2, or 3

 **		Bits per pixel	= 1

 **		Colormap	= ignored

 **		No. Planes	= 1

 **		Bytes per line	= (width + 7) / 8 rounded up to even byte count

 **		Colormap type	= ignored

 **

 **	Image data following the header is an encoded buffer with 8 pixels

 **	to the byte.  The left-most bit in each byte corresponds to the

 **	left-most pixel of each group of 8.  Scanline buffers are rounded up

 **	to an even number of bytes, then encoded (see below).

 **

 **	"Extra" image data may follow the last scanline in order to bring the

 **	image data up to a multiple of 8 or 16 scanlines.  On read, we ignore

 **	such extra data.  On write we don't write extra data.

 **

 **  4-BIT COLOR IMAGES

 **	4-bit color PCX files were intended for the four-plane mode of the

 **	EGA card.

 **

 **	Header values set to the following flag 4-bit color images:

 **

 **		Version		= 2 or 3

 **		Bits per pixel	= 1

 **		Colormap	= used (see below)

 **		No. Planes	= 4

 **		Bytes per line	= (width + 7)/8 rounded up to even byte count

 **		Colormap type	= ignored

 **

 **	If the 'Version' field of the header is a '2', the header contains

 **	a color palette in the 'Colormap' portion of the header.  Palette

 **	values are given from low to high and consist of 16 3-byte RGB triplets.

 **

 **	If the 'Version' field of the header is a '3', the header does not

 **	contain a palette for the image.  For our purposes, this makes the

 **	image grayscale.

 **

 **	Image data following the header is grouped as four consequetive

 **	1-bit scanlines of data.  Each 1-bit scanline is packed into bytes

 **	at 8 pixels to the byte, with the left most bit in each byte

 **	corresponding to the left-most pixel of each group of 8.  The entire

 **	set of 4 1-bit scanlines is considered one buffer, and as a whole

 **	is padded to the next higher even byte boundary, then encoded.

 **

 **	For encoding purposes, the group of 4 1-bit scanlines is considered

 **	a single buffer.  Encoding always breaks at the end of such a buffer,

 **	but might not break going from one 1-bit scanline to the next within

 **	such a group.

 **

 **	"Extra" image data may follow the last scanline in order to bring the

 **	image data up to a multiple of 8 or 16 scanlines.  We neither read it

 **	or write it.

 **

 **  8-BIT COLOR IMAGES

 **	8-bit color PCX files were intended for the VGA card.

 **

 **	Header values set to the following flag 8-bit color images:

 **

 **		Version		= 5

 **		Bits per plane	= 8

 **		Colormap	= unused (see below)

 **		No. Planes	= 1

 **		Bytes per line	= ((width + 1)/2) * 2

 **		Colormap type	= 1 or 2

 **

 **	The 'Version' field will always be 5 for 8-bit color images.

 **

 **	The color palette for 8-bit color images is too big for the 'Colormap'

 **	field of the header.  Instead, the palette, if any, is tacked onto

 **	the end of the image data.  The "official" procedure (from the spec)

 **	is to seek to 769 bytes before the end of the file and read a byte.

 **	If it is a 0x0C (12 decimal), then the next 768 bytes are 256 3-byte

 **	RGB triplets.

 **

 **	The 'Colormap type' field is meant to toggle the VGA card between

 **	grayscale and color.  In our case, it is meaningless.  The existance

 **	of a palette makes the image color.  Nonexistance means the image is

 **	grayscale.  On read, we ignore this field.  On write, we always set

 **	it to 1 (color).

 **

 **	Image data following the header is encoded from a stream of bytes,

 **	at one byte per pixel.  Consequetive bits in each byte correspond to

 **	consequetive color planes (as with normal graphics hardware).

 **	As with monochrome and 4-bit images, each scanline buffer is rounded

 **	up to an even number of bytes.  Encoding breaks at the end of each

 **	scanline.

 **

 **	"Extra" image data may follow the last scanline in order to bring the

 **	image data up to a multiple of 8 or 16 scanlines.  We neither read or

 **	write such extra scanlines.

 **

 **  ENCODING

 **	Image data buffers are encoded with a type of run-length encoding.

 **	A data byte with its upper two bits set is a run count for the number

 **	of times to repeat the next byte in the file.  Data bytes without

 **	their upper two bits set are literal values.  For example:

 **

 **	Decode:

 **		pEncoded = encodedBuffer;

 **		pDecoded = decodedBuffer;

 **		pEnd     = decodedBuffer + BytesPerLine;

 **		while ( pDecoded < pEnd )

 **		{

 **			if ( (*pEncoded & 0xC0) == 0xC0 )

 **			{

 **				count = (*pEncoded++) & 0x3F;

 **				value = *pEncoded++;

 **				for ( j = 0; j < count; j++ )

 **					*pDecoded++ = value;

 **			}

 **			else

 **				*pDecoded++ = *pEncoded++;

 **		}

 **

 **	When encoding one must watch for data values with their upper two

 **	bits set.  Such bytes are encoded as a run byte of 1 followed by

 **	the data byte.

 **

 **	The worst case image only contains data bytes with their upper two

 **	bits set.  This will cause an encoded image to take DOUBLE the space

 **	of the unencoded image.  Brain dead.

 **

 **  COLOR TABLE CAVEATS

 **    #1:  PBM+ Problems.

 **	According to the spec, the existance and style of CLT is determined

 **	by the header's version number:

 **

 **		0	2.5	no CLT

 **		2	2.8W	CLT in header

 **		3	2.8WO	no CLT

 **		5	3.0	CLT at end of file (possibly)

 **

 **	Unfortunately, the popular PBM+ package for UNIX systems always

 **	generates PCX files for verison 3.0, and assumes other packages

 **	will check the header plane and bits per pixel fields to decide

 **	where a CLT is.

 **

 **	In order to be compatible with this software, we ignore the version

 **	number header and similarly depend upon the plane and bits per pixel

 **	counts to decide what type of CLT, if any, is present.  The only

 **	version number we check is 2.8WO which explicitly says there is no

 **	CLT for the image and that the header CLT should be ignored.

 **

 **	Fortunately, this change of parsing still maintains compatibility

 **	with PCX usage.  For CLT writing, we choose the right version number

 **	the way we're supposed to.

 **

 **    #2:  Spec Problems.

 **	The PCX spec recommends finding end-of-file CLTs by the following:

 **

 **		lseek( fd, -769, 2 );	-- seek to end of file, -769 bytes

 **		read( fd, buffer, 1 );	-- read 1 byte

 **		if ( *buffer == 0x0C )

 **			there is a CLT

 **

 **	This is not a robust way of checking for an EOF CLT.  Imagine the

 **	case where there is no CLT, and seeking back 769 bytes gets you

 **	into the middle of image data.  0x0C is not a byte that is impossible

 **	to encounter in image data.  If you happened to encounter it, you

 **	would mistakenly conclude there was a CLT.

 **

 **	The popular UNIX PBM+ package simply reads the image data in, one

 **	byte at a time.  When it has finished reading in the data, it looks

 **	at the next byte to see if it is 0x0C.  If so, it reads in a CLT.

 **

 **	This is neither speedy (lots of single byte reads) nor robust.

 **	The spec says:

 **		"There may be extra scan lines at the bottom of the image,

 **		to round to 8 or 16 scan lines."

 **

 **	If they were there, PBM+ would not find the CLT.

 **

 **	Our approach is more robust and much faster.

 **

 **	The CLT is read in first by the seek approach.  Doing it first is

 **	merely convenient algorithmically.  It could be done last as well.

 **

 **	The image data is then read in by making a worst-case guess at its

 **	size in bytes (twice the size of a decoded image).  This will

 **	very likely read too much data.  This is OK since we watch the

 **	decode count while decoding so that we don't decode off the end of

 **	the image.  The advantage is that we can read it all in with one

 **	read system call.  Much much much faster.

 **

 **	After decoding, a count is returned of the number of encoded bytes

 **	used.  To determine if the CLT we read in earlier is truely a CLT,

 **	and not image data improperly interpreted, we compare the file position

 **	of the 769-bytes-from-the-end CLT with the position we'd be at if we

 **	had only read in the image data (header + # of encoded bytes used).

 **	If the CLT file position is in advance of the end-of-image data

 **	position, then it is considered a valid CLT.

 **

 **	It is still possible that there are lots and lots of "extra" scanlines

 **	after the image data and that they contain 0x0C's.  A seek into the

 **	midst of these would still falsly conclude there was a CLT there.

 **	This seems very unlikely.  Software that pads with extra scanlines

 **	usually sets them to zeroes.

 **

 **  IMAGE DATA CAVEATS

 **    #1:  Even-byte-count buffers

 **	According to the spec, all scanlines, prior to encoding, are rounded

 **	up to an even number of bytes.  Unfortunately, the popular PBM+

 **	package for UNIX systems fails to do this.

 **

 **	In order to be compatible with this software, we check the header's

 **	"Bytes per line" field and skip the difference between it and the

 **	size of a decoded buffer.  If the PCX generator made the byte count

 **	even, this will skip 1 byte for odd-length lines.  If the PCX generator

 **	did not make this even, such as PBM+, then this will skip no bytes

 **	in such cases.

 **

 **	Fortunately, this change of skipping still maintains compatibility

 **	with PCX usage.  For image writing, we round up the way we're

 **	supposed to.

 **

 **    #2:  2-bit and 3-bit color images

 **	According to the documentation, 2-bit and 3-bit images aren't really

 **	allowed.  However, the poplular UNIX PBM+ package freely generates

 **	them.

 **

 **	In order to be compatible with such code, we also accept 2-bit and

 **	3-bit images as variations of the 4-bit case.

 **

 **	Header values set to the following flag 4-bit color images:

 **

 **		Version		= 2 or 3

 **		Bits per pixel	= 1

 **		Colormap	= used (see below)

 **		No. Planes	= 2 or 3

 **		Bytes per line	= (width + 7)/8 rounded up to even byte count

 **		Colormap type	= ignored

 **

 **	2-bit and 3-bit cases are stored the same as for the 4-bit case.

 */





/*

 *  PCX - ZSoft IBM PC Paintbrush

 *      For information on these structures, how to use them, etc. please

 *      see imfmt.c.

 */

#ifdef __STDC__

static int imPcxRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imPcxWrite1( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imPcxWrite8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

#else

static int imPcxRead( );

static int imPcxWrite1( ), imPcxWrite8( );

#endif

static char *imPcxNames[ ]  = { "pcx", "pcc", NULL };

static unsigned char imPcxMagicNumber[ ] = { 0x0a };

static ImFileFormatReadMap imPcxReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IN,1,1,       RLE,    IMVFBMONO,      0 },

        { IN,1,2,       RLE,    IMVFBINDEX8,    0 },

        { IN,1,2,       RLE|C,  IMVFBINDEX8,    C },

        { IN,1,3,       RLE,    IMVFBINDEX8,    0 },

        { IN,1,3,       RLE|C,  IMVFBINDEX8,    C },

        { IN,1,4,       RLE,    IMVFBINDEX8,    0 },

        { IN,1,4,       RLE|C,  IMVFBINDEX8,    C },

        { IN,1,8,       RLE,    IMVFBINDEX8,    0 },

        { IN,1,8,       RLE|C,  IMVFBINDEX8,    C },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imPcxWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBMONO,    0,      IN,1,1,         RLE,    imPcxWrite1 },

        { IMVFBINDEX8,  0,      IN,1,8,         RLE,    imPcxWrite8 },

        { IMVFBINDEX8,  C,      IN,1,8,         RLE| C, imPcxWrite8 },

        { -1,           0,      -1,             0,      NULL },

};



static ImFileMagic imFilePcxMagic[]=

{

	{ 0, 1, imPcxMagicNumber },

	{ 0, 0, NULL },

};



ImFileFormat ImFilePcxFormat =

{

	imPcxNames, "ZSoft IBM PC Paintbrush file",

	"ZSoft Corp.",

	"1-bit monochrome (CGA), 2-bit, 3-bit, and 4-bit color (EGA) files\n\
with or without a color table, and 8-bit color (VGA) files with or\n\
without a color table.  All are RLE-encoded.",
	"1-bit monochrome (CGA), and 8-bit color (VGA) files with or\n\
without a color table.  All are RLE-encoded.",
	imFilePcxMagic,

	IMNOMULTI, IMNOPIPE,

	IMNOMULTI, IMPIPE,

	imPcxRead, imPcxReadMap, imPcxWriteMap

};







/*

 *  TYPEDEF & STRUCT

 *	imPcxHeaderInfo		-  header for PCX files

 *	imPcxHeaderFields	-  header description for binary I/O package

 *

 *  DESCRIPTION

 *	PCX files start with a header that gives various details of the

 *	image contained in the file.

 */



typedef struct imPcxHeaderInfo		/* PCX				*/

{

	unsigned char	pcx_manufacturer;

	unsigned char	pcx_version;

	unsigned char	pcx_encoding;

	unsigned char	pcx_bitsPerPixel;

	unsigned short	pcx_window[4];

	unsigned short	pcx_horizontalRes;

	unsigned short	pcx_verticalRes;

	unsigned char	pcx_clt[16][3];

	unsigned char	pcx_reserved;

	unsigned char	pcx_nPlanes;

	unsigned short	pcx_bytesPerLine;

	unsigned short	pcx_cltType;

	unsigned char	pcx_filler[58];

} imPcxHeaderInfo;



static BinField imPcxHeaderFields[ ] =

{

	/* pcx_manufacturer */		UCHAR, 1, 1,

	/* pcx_version */		UCHAR, 1, 1,

	/* pcx_encoding */		UCHAR, 1, 1,

	/* pcx_bitsPerPixel */		UCHAR, 1, 1,

	/* pcx_window */		USHORT, 2, 4,

	/* pcx_horizontalRes */		USHORT, 2, 1,

	/* pcx_verticalRes */		USHORT, 2, 1,

	/* pcx_clt */			UCHAR, 1, 48,

	/* pcx_reserved */		UCHAR, 1, 1,

	/* pcx_nPlanes */		UCHAR, 1, 1,

	/* pcx_bytesPerLine */		USHORT, 2, 1,

	/* pcx_cltType */		USHORT, 2, 1,

	/* pcx_filler */		UCHAR, 1, 58,

					    0, 0, 0,

};



#define IMPCXHEADERSIZE	(128)





/*

 *  Value for fields:

 */

#define IMPCXMANUFACTURER	0x0A	/* ZSoft .PCX			*/



#define IMPCXV_2_5		0	/* Version 2.5			*/

#define IMPCXV_2_8W		2	/* Version 2.8 w/clt		*/

#define IMPCXV_2_8WO		3	/* Version 2.8 wo/clt		*/

#define IMPCXV_3_0		5	/* Version 3.0			*/



#define IMPCXENCODING		1	/* Run-length encoded		*/



#define IMPCXCOLOR		1	/* Color or BW CLT		*/

#define IMPCXGRAY		2	/* Grayscale CLT		*/

#define IMPCXPAINTBRUSH		2992	/* Strange PC Paintbrush type	*/











/*

 *  FUNCTION

 *	imPcxDecode	-  decode a run buffer

 *

 *  DESCRIPTION

 *	An incomming encoded data buffer is decoded into a second buffer.

 */



static int				/* Returns # of bytes decoded	*/

#ifdef __STDC__

imPcxDecode( unsigned char *src, int nDst, unsigned char *dst )

#else

imPcxDecode( src, nDst, dst )

	unsigned char *src;		/* Source buffer (encoded)	*/

		 int    nDst;		/* # of bytes in destination buffer*/

	unsigned char *dst;		/* Destination buffer (decoded)	*/

#endif

{

	int    i;		/* Counter			*/

	unsigned char  value;		/* Run value			*/

	unsigned char *dstLast;	/* Last byte +1 of dst buffer	*/

		 unsigned char *srcSave;	/* Saved src pointer		*/



	srcSave = src;

	dstLast = dst + nDst;

	while ( dst < dstLast )

	{

		if ( (*src & 0xC0) != 0xC0 )

			*dst++ = *src++;

		else

		{

			i = *src++ & 0x3F;

			for ( value = *src++; i; i-- )

				*dst++ = value;

		}

	}



	return ( src - srcSave );

}









/*

 *  FUNCTION

 *	imPcxEncode	-  encode a run buffer

 *

 *  DESCRIPTION

 *	An incomming data buffer is encoded into a second buffer.  Bytes

 *	with their upper two bits set are flagged and stored as runs of 1.

 *

 *	Note:  becuase the upper two bits of a byte flag a run, we get two

 *	special cases to watch for:

 *		1.  The maximum run length is 63 (lower 6 bits set).

 *		2.  Data bytes with the upper two bits set have to be

 *		    treated as runs of 1.

 */



static int				/* Returns # of bytes encoded	*/

#ifdef __STDC__

imPcxEncode( unsigned char *src, int nSrc, unsigned char *dst )

#else

imPcxEncode( src, nSrc, dst )

	unsigned char *src;		/* Source buffer (encoded)	*/

		 int    nSrc;		/* # of bytes in source buffer	*/

	unsigned char *dst;		/* Destination buffer (decoded)	*/

#endif

{

	int    run;		/* Run count			*/

	unsigned char *srcLast;	/* Last byte +1 of source buffer*/

		 unsigned char *dstSave;	/* Saved original dst value	*/



	srcLast = src + nSrc - 1;

	dstSave = dst;

	while ( src <= srcLast )

	{

		run = 0;

		while ( *src == *(src+1) && src < srcLast && run < 62 )

		{

			run++;

			src++;

		}

		if ( run > 0 )

			*dst++ = (run + 1) | 0xC0;/* Store the run count*/

		else if ( (*src & 0xC0) == 0xC0 )

			*dst++ = 1 | 0xC0;	/* Make it a run of 1	*/

		*dst++ = *src++;

	}

	return ( dst - dstSave );

}











/*

 *  FUNCTION

 *	imPcxRead	-  read a PCX file

 *

 *  DESCRIPTION

 *	The file header is read and checked.  The color table, if any, is

 *	read in.  The image data is read in and decoded, then dispersed into

 *	a VFB of the appropriate depth.  The VFB and CLT are added to the

 *	tag table.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__

imPcxRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imPcxRead( ioType, fd, fp, flagsTable, tagTable )

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable    *flagsTable;	/* Flags			*/

	TagTable    *tagTable;		/* Tag table to add to		*/

#endif

{

	ImVfb       *vfb;		/* Read in image		*/

	ImVfbPtr     pPixel;		/* Pcxel pointer		*/



	ImClt       *clt;		/* Image CLT			*/

	ImCltPtr     pColor;		/* Color pointer		*/



	unsigned char	     cltBuffer[769];	/* VGA card palette		*/

	unsigned char       *pCltBuffer;	/* Buffer pointer		*/



	unsigned char	    *encoded;		/* Encoded scanline buffer	*/

	unsigned char	    *decoded;		/* Decoded scanline buffer	*/

	unsigned char       *pDecoded;		/* Decoded buffer pointer	*/

	unsigned char	     value;		/* Pixel value			*/

	unsigned char	     value2;		/* Old pixel value		*/



	imPcxHeaderInfo header;		/* PCX file header		*/



	int	     height, width;	/* Image dimensions		*/

	int	     width8;		/* Width / 8			*/

	int	     widthextra;	/* Width % 8			*/



	int	     n;			/* Byte count			*/

	int          i, j;		/* Counter			*/

	int	     x, y;		/* Current scanline		*/



	int	     cltInHeader;	/* Get the CLT from the header	*/

	int	     cltAtEnd;		/* Get the CLT from the EOF	*/



	long	     cltFilePosition;	/* File position of CLT		*/

	long	     imageFilePosition;	/* File position of Image	*/

	char		 message[100];	/* ImInfo message		*/





	/*

	 *  Read in the header in LBF (Intel) byte order.  Check that it is

	 *  valid.

	 */

	BinByteOrder( BINLBF );

	if ( ImBinReadStruct( ioType, fd, fp, &header, imPcxHeaderFields )== -1)

		ImReturnBinError( );



	if ( header.pcx_manufacturer != IMPCXMANUFACTURER )

		ImErrorFatal( "Bad manufacturer code in PCX file header", -1, IMESYNTAX );



	switch ( header.pcx_version )

	{

	case IMPCXV_2_5:

		ImInfo( "Version", "2.5" );

		break;

	case IMPCXV_2_8W:

		ImInfo( "Version", "2.8 with Palette" );

		break;

	case IMPCXV_2_8WO:

		ImInfo( "Version", "2.8 without Palette" );

		break;

	case IMPCXV_3_0:

		ImInfo( "Version", "3.0" );

		break;

	default:

		ImErrorFatal( "Unknown file version in PCX file header", -1, IMESYNTAX );

	}



	if ( header.pcx_encoding != IMPCXENCODING )

		ImErrorFatal( "Unknown encoding method in PCX file header", -1, IMESYNTAX );



	switch ( header.pcx_bitsPerPixel )

	{

	case 1:	/* 1-bit monochrome (CGA)				*/

		/* 2-bit color (EGA)					*/

		/* 3-bit color (EGA)					*/

		/* 4-bit color (EGA)					*/

		if ( header.pcx_nPlanes > 4 )

			ImErrorFatal( "Bad color plane count in PCX file header", -1, IMESYNTAX );

		if ( header.pcx_version == IMPCXV_2_8WO )

			cltInHeader = FALSE;

		else

			cltInHeader = TRUE;

		cltAtEnd = FALSE;

		break;



	case 8:	/* 8-bit color (VGA)					*/

		if ( header.pcx_nPlanes != 1 )

			ImErrorFatal( "Bad color plane count in PCX file header", -1, IMESYNTAX );

		cltInHeader = FALSE;

		cltAtEnd = TRUE;

		break;

	default:

		ImErrorFatal( "Unknown image depth.  Must be 1, 4, or 8.", -1, IMESYNTAX );

	}



	width  = header.pcx_window[2] - header.pcx_window[0] + 1;

	height = header.pcx_window[3] - header.pcx_window[1] + 1;





	switch ( header.pcx_cltType )

	{

	case 0:			/* For backwards compatibility		*/

	case IMPCXCOLOR:

	case IMPCXGRAY:

	case IMPCXPAINTBRUSH:

		break;



	default:

		ImErrorFatal( "Unknown palette type in PCX file header", -1, IMESYNTAX );

	}



	ImInfo( "Byte Order", "Least Significant Byte First" );



	sprintf( message, "%d x %d", width, height );

	ImInfo( "Resolution", message );



	sprintf( message, "%d-bit Color Indexed", header.pcx_bitsPerPixel);

	ImInfo( "Type", message );



	/*

	 *  Make a CLT if we need one.

	 */

	clt = IMCLTNULL;

	if ( cltInHeader )

	{

		/* 16-color CLT included in header.			*/

		if ( (clt = ImCltAlloc( 16 )) == IMCLTNULL )

			ImErrorFatal( ImQError( ), -1, ImErrNo );

		pColor = ImCltQFirst( clt );

		for ( i = 0; i < 16; i++ )

		{

			ImCltSRed(   pColor, header.pcx_clt[i][0] );

			ImCltSGreen( pColor, header.pcx_clt[i][1] );

			ImCltSBlue(  pColor, header.pcx_clt[i][2] );

			ImCltSInc( clt, pColor );

		}



		ImInfo( "Color Table", "16 Entries" );

	}

	else if ( cltAtEnd )

	{

		/*

		 *  Seek to 769 bytes from the bottom and see if there

		 *  is a 0x0C there.  If so, we've got a 256 entry palette.

		 *

		 *  HOWEVER, there is the chance that seeking to 769 bytes

		 *  from the end will put us within the image data if there

		 *  really isn't a CLT.  If that next image data byte just

		 *  happened to be a 0x0C, then we'd falsely conclude there

		 *  was a CLT.  This is a problem in the PCX spec.

		 *

		 *  To handle this case, we remember where things are and

		 *  later compare where we got the alleged CLT versus the

		 *  end of the image data.  If it is too early in the file,

		 *  we were fooled and we get rid of the bogus CLT.

		 */

		imageFilePosition = ImTell( ioType, fd, fp );

		ImSeek( ioType, fd, fp, -769, 2 );

		cltFilePosition   = ImTell( ioType, fd, fp );

		n = ImBinRead( ioType, fd, fp, cltBuffer, UCHAR, 1, 769 );

		if ( n != -1 && *cltBuffer == 0x0C )

		{

			if ( (clt = ImCltAlloc( 256 )) == IMCLTNULL )

				ImErrorFatal( ImQError( ), -1, ImErrNo );

			pColor     = ImCltQFirst( clt );

			pCltBuffer = cltBuffer + 1;

			for ( i = 0; i < 256; i++ )

			{

				ImCltSRed(   pColor, *pCltBuffer++ );

				ImCltSGreen( pColor, *pCltBuffer++ );

				ImCltSBlue(  pColor, *pCltBuffer++ );

				ImCltSInc( clt, pColor );

			}



			ImInfo( "Color Table", "256 Entries" );

		}

		else

		{

			ImInfo( "Color Table", "none" );

		}

		ImSeek( ioType,fd, fp, IMPCXHEADERSIZE, 0 );

	}





	ImInfo( "Compression Type", "Run Length Encoded (RLE)" );



	/*

	 *  Read in the image data.  Unfortunately, because it is encoded,

	 *  we don't know how much data there is.  We do know our worst

	 *  case encoding could produce a file twice the size of the image.

	 *  So, we allocate a buffer that big and try to read that much

	 *  data.  It is very likely that BinRead will return less than

	 *  that.  Fine.  We then decode that buffer into a new buffer

	 *  that is the size of the image (in bytes).

	 */

	n = header.pcx_bytesPerLine * header.pcx_nPlanes * height;

	ImMalloc( encoded, unsigned char *, 2 * n );

	ImMalloc( decoded, unsigned char *, n );



	if ( ImBinRead( ioType, fd, fp, encoded, UCHAR, 1, 2 * n ) == -1 )

	{

		free( (char *)encoded );

		free( (char *)decoded );

		if ( clt != IMCLTNULL )

			ImCltFree( clt );

		ImReturnBinError( );

	}



	n = imPcxDecode( encoded, n, decoded );





	/*

	 *  If the location at which we found the CLT is within the image

	 *  data we just read in, then there wasn't really a CLT there at

	 *  all.  It just happened that seeking to 769 bytes from the end

	 *  fell upon a 0x0C within the image data, and we falsly concluded

	 *  there was a CLT.  We fix our mistake now.

	 */

	if ( cltAtEnd && imageFilePosition + n > cltFilePosition )

	{

		ImCltFree( clt );

		clt = IMCLTNULL;

	}





	/*

	 *  Walk the decoded buffer and expand it into a new VFB.

	 */

	if ( header.pcx_bitsPerPixel == 1 && header.pcx_nPlanes == 1 )

	{

		/*

		 *  Monochrome.  The decoded buffer contains packed bytes,

		 *  each holding 8 1-bit pixel values.  Watch for widths that

		 *  are not a multiple of 8, and remember to toss an extra

		 *  byte at the end of each scanline if the width (in bytes)

		 *  is not even.

		 */

		if ( (vfb=ImVfbAlloc( width, height, IMVFBMONO )) == IMVFBNULL )

		{

			free( (char *)encoded );

			free( (char *)decoded );

			if ( clt != IMCLTNULL )

				ImCltFree( clt );

			ImErrorFatal( ImQError( ), -1, ImErrNo );

		}



		pPixel   = ImVfbQFirst( vfb );

		pDecoded = decoded;

		width8   = width / 8;			/* Width in bytes*/

		widthextra = width & 0xF;		/* width % 8	*/

		for ( y = 0; y < height; y++ )

		{

			for ( x = 0; x < width8; x++ )

			{

				value = *pDecoded++;

				for ( i = 7; i >= 0; i-- )

				{

					ImVfbSMono( vfb, pPixel,

						(value>>i) & 0x1 );

					ImVfbSInc( vfb, pPixel );

				}

			}

			if ( widthextra )

			{

				value = *pDecoded++;

				for ( i = 7; i >= (8-widthextra); i-- )

				{

					ImVfbSMono( vfb, pPixel,

						(value>>i) & 0x1 );

					ImVfbSInc( vfb, pPixel );

				}

			}



			/*

			 *  According to the spec, we only have to skip a

			 *  byte if the width is odd.  However, some packages

			 *  don't adhear to the even-byte-count requirement

			 *  (such as PBM+).  So, we only skip if the header

			 *  says we should, and then as much as it says we

			 *  should.

			 */

			for ( i = width8 + (widthextra?1:0);

				i < header.pcx_bytesPerLine; i++ )

				pDecoded++;		/* Skip		*/

		}

	}

	else if ( header.pcx_bitsPerPixel == 1 )

	{

		/*

		 *  n-bit color (usually 4-bit).  The decoded buffer contains

		 *  packed bytes, each holding 8 1-bit pixel values... yes,

		 *  1-bit pixel values!  Each scanline is stored as nPlanes

		 *  of 1-bit planes.  We need to remember to toss an extra

		 *  byte per set if each scanline plane set's byte count is

		 *  not even.

		 */

		if ( (vfb=ImVfbAlloc( width, height, IMVFBINDEX8 ))==IMVFBNULL )

		{

			free( (char *)encoded );

			free( (char *)decoded );

			if ( clt != IMCLTNULL )

				ImCltFree( clt );

			ImErrorFatal( ImQError( ), -1, ImErrNo );

		}



		pDecoded = decoded;

		width8   = width / 8;			/* Width in bytes*/

		widthextra = width & 0xF;		/* width % 8	*/

		for ( y = 0; y < height; y++ )

		{

			/* First plane.					*/

			pPixel = ImVfbQPtr( vfb, 0, y );

			for ( x = 0; x < width8; x++ )

			{

				value = *pDecoded++;

				for ( i = 7; i >= 0; i-- )

				{

					ImVfbSIndex8( vfb, pPixel,

						((value>>i) & 0x1) );

					ImVfbSInc( vfb, pPixel );

				}

			}

			if ( widthextra )

			{

				value = *pDecoded++;

				for ( i = 7; i >= (8-widthextra); i-- )

				{

					ImVfbSIndex8( vfb, pPixel,

						((value>>i) & 0x1) );

					ImVfbSInc( vfb, pPixel );

				}

			}



			/* Additional planes.				*/

			for ( j = 1; j < header.pcx_nPlanes; j++ )

			{

				pPixel = ImVfbQPtr( vfb, 0, y );

				for ( x = 0; x < width8; x++ )

				{

					value = *pDecoded++;

					for ( i = 7; i >= 0; i-- )

					{

						value2 = ImVfbQIndex8( vfb,

							pPixel );

						ImVfbSIndex8( vfb, pPixel,

							value2 |

							((value>>i) & 0x1)<<j );

						ImVfbSInc( vfb, pPixel );

					}

				}

				if ( widthextra )

				{

					value = *pDecoded++;

					for ( i = 7; i >= (8-widthextra); i-- )

					{

						value2 = ImVfbQIndex8( vfb,

							pPixel );

						ImVfbSIndex8( vfb, pPixel,

							value2 |

							((value>>i) & 0x1)<<j );

						ImVfbSInc( vfb, pPixel );

					}

				}

			}



			/*

			 *  According to the spec, we only have to skip a

			 *  byte if the width is odd (note:  width includes

			 *  bytes for all the planes for one scanline).

			 *  However, some packages don't adhear to the

			 *  even-byte-count requirement (such as PBM+).  So

			 *  we only skip if the header says we should, and

			 *  then as much as it says we should.

			 */

			for ( i = (width8+(widthextra?1:0))*header.pcx_nPlanes;

				i < header.pcx_bytesPerLine; i++ )

				pDecoded++;		/* Skip		*/

		}

	}

	else

	{

		/*

		 *  8-bit color.  The decoded buffer contains one byte per

		 *  pixel.  We just copy it out and into the VFB.  We need

		 *  to toss a byte for each scanline if the width is not

		 *  even.

		 */

		if ( (vfb=ImVfbAlloc( width, height, IMVFBINDEX8 ))==IMVFBNULL )

		{

			free( (char *)encoded );

			free( (char *)decoded );

			if ( clt != IMCLTNULL )

				ImCltFree( clt );

			ImErrorFatal( ImQError( ), -1, ImErrNo );

		}



		pPixel   = ImVfbQFirst( vfb );

		pDecoded = decoded;

		for ( y = 0; y < height; y++ )

		{

			for ( x = 0; x < width; x++ )

			{

				ImVfbSIndex8( vfb, pPixel, *pDecoded++ );

				ImVfbSInc( vfb, pPixel );

			}



			/*

			 *  According to the spec, we only have to skip a

			 *  byte if the width is odd.  However, some packages

			 *  don't adhear to the even-byte-count requirement

			 *  (such as PBM+).  So, we only skip if the header

			 *  says we should, and then as much as it says we

			 *  should.

			 */

			for ( i = width; i < header.pcx_bytesPerLine; i++ )

				*pDecoded++;		/* Skip		*/

		}

	}





	/*

	 *  Clean up.

	 */

	free( (char *)encoded );

	free( (char *)decoded );





	/*

	 *  Add the VFB to the tagTable.

	 */

	if ( clt != IMCLTNULL )

	{

		ImVfbSClt( vfb, clt );

		TagTableAppend( tagTable,

			TagEntryAlloc( "image clt", POINTER, &clt ) );

		TagTableAppend( tagTable,

			TagEntryAlloc( "image vfb", POINTER, &vfb ) );

		return ( 2 );

	}

	TagTableAppend( tagTable,

		TagEntryAlloc( "image vfb", POINTER, &vfb ) );

	return ( 1 );

}











/*

 *  FUNCTION

 *	imPcxWrite1	-  write a 1-bit PCX file

 *

 *  DESCRIPTION

 *	The PCX file header set up and written out followed by the encoded

 *	image data.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imPcxWrite1( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imPcxWrite1( pMap, ioType, fd, fp, flagsTable, tagTable )

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



	imPcxHeaderInfo header;		/* PCX file header		*/



	unsigned char       *encoded;		/* Encoded data buffer		*/

	unsigned char	    *decoded;		/* Decoded data buffer		*/

	unsigned char	    *pDecoded;		/* Encoded buffer pointer	*/



	int          n;			/* Byte count			*/

	int          x, y;		/* Pixel counters		*/

	int	     i;			/* Counter			*/

	int	     value;		/* Pixel value			*/

	int	     height, width;	/* Image size			*/

	int	     width8;		/* Width in bytes		*/

	int	     widthextra;	/* Extra bits in last byte	*/

	char		 message[100];	/* ImInfo message		*/





	/*

	 *  Set up the header and write it out.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	width  = ImVfbQWidth( vfb );

	height = ImVfbQHeight( vfb );



	BinByteOrder( BINLBF );



	memset( (void *)&header, 0x00, sizeof( header ) );

	header.pcx_manufacturer	= IMPCXMANUFACTURER;

	header.pcx_version	= IMPCXV_2_5;	/* No color		*/

	header.pcx_encoding	= IMPCXENCODING;

	header.pcx_bitsPerPixel	= 1;		/* Monochrome		*/

	header.pcx_window[0]	= 0;		/* xmin			*/

	header.pcx_window[1]	= 0;		/* ymin			*/

	header.pcx_window[2]	= width - 1;	/* xmax (minus 1)	*/

	header.pcx_window[3]	= height - 1;	/* ymax (minus 1)	*/

	header.pcx_nPlanes	= 1;		/* Monochrome		*/

	header.pcx_bytesPerLine	= (width + 7)/8;

	if ( header.pcx_bytesPerLine & 0x1 )

		++header.pcx_bytesPerLine;	/* Make it even		*/

	header.pcx_cltType	= 0;		/* Not relevant		*/



	header.pcx_clt[0][0]    = 0;		/* Black		*/

	header.pcx_clt[0][1]    = 0;

	header.pcx_clt[0][2]    = 0;

	header.pcx_clt[1][0]    = 255;		/* White		*/

	header.pcx_clt[1][1]    = 255;

	header.pcx_clt[1][2]    = 255;



	/* Remaining fields left zero.					*/



	if ( ImBinWriteStruct( ioType, fd, fp, &header, imPcxHeaderFields )==-1)

	{

		ImReturnBinError( );

	}



	/*

	 * Output -verbose message

	 */

	ImInfo( "Version", "2.5" );

	ImInfo( "Byte Order", "Least Significant Byte First" );



	sprintf( message, "%d x %d", width, height );

	ImInfo( "Resolution", message);



	ImInfo( "Type", "1-bit Color Indexed" );

	ImInfo( "Color Table", "16 Entries" );

	ImInfo( "Compression Type", "Run Length Encoded (RLE)" );



	/*

	 *  Buffer up, encode, and write out the image data.  Remember that

	 *  an encoded buffer could take up twice as much space as the

	 *  decoded one, in the pathelogical case.

	 */

	ImMalloc( decoded, unsigned char *, header.pcx_bytesPerLine );

	ImMalloc( encoded, unsigned char *, 2 * header.pcx_bytesPerLine );

	memset( (void *)decoded, 0x00, header.pcx_bytesPerLine );



	width8 = width / 8;			/* # of bytes		*/

	widthextra = width & 0xF;		/* width % 8		*/



	pPixel = ImVfbQFirst( vfb );

	for ( y = 0; y < height; y++ )

	{

		pDecoded = decoded;

		for ( x = 0; x < width8; x++ )

		{

			value = 0;

			for ( i = 7; i >= 0; i-- )

			{

				value |= (ImVfbQMono( vfb, pPixel ) & 0x1) << i;

				ImVfbSInc( vfb, pPixel );

			}

			*pDecoded++ = value;

		}

		if ( widthextra )

		{

			value = 0;

			for ( i = 7; i >= (8-widthextra); i-- )

			{

				value |= (ImVfbQMono( vfb, pPixel ) & 0x1) << i;

				ImVfbSInc( vfb, pPixel );

			}

			*pDecoded++ = value;

		}





		/*

		 *  Encoded it and write it out.

		 */

		n = imPcxEncode( decoded, header.pcx_bytesPerLine, encoded );

		if ( ImBinWrite( ioType, fd, fp, encoded, UCHAR, 1, n ) == -1 )

		{

			free( (char *)encoded );

			free( (char *)decoded );

			ImReturnBinError( );

		}

	}



	free( (char *)encoded );

	free( (char *)decoded );

	return ( 1 );

}











/*

 *  FUNCTION

 *	imPcxWrite8	-  write an 8-bit PCX file

 *

 *  DESCRIPTION

 *	The PCX file header set up and written out followed by the encoded

 *	image data.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imPcxWrite8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imPcxWrite8( pMap, ioType, fd, fp, flagsTable, tagTable )

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



	ImClt	    *clt;		/* Color lookup table		*/

	ImCltPtr     pColor;		/* Color pointer		*/



	imPcxHeaderInfo header;		/* PCX file header		*/



	unsigned char       *encoded;		/* Encoded data buffer		*/

	unsigned char	    *decoded;		/* Decoded data buffer		*/

	unsigned char	    *pDecoded;		/* Encoded buffer pointer	*/



	unsigned char	    *colorBuf;		/* Color table output buffer	*/

	unsigned char	    *pColorBuf;		/* Color table buffer pointer	*/



	int          n;			/* Byte count			*/

	int	     i, j;		/* Color counters		*/

	int          x, y;		/* Pixel counters		*/

	int	     height, width;	/* Image size			*/

	char		 message[100];	/* ImInfo message		*/





	/*

	 *  Set up the header and write it out.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	clt    = ImVfbQClt( vfb );

	width  = ImVfbQWidth( vfb );

	height = ImVfbQHeight( vfb );



	BinByteOrder( BINLBF );



	memset( (void *)&header, 0x00, sizeof( header ) );

	header.pcx_manufacturer	= IMPCXMANUFACTURER;

	header.pcx_version	= IMPCXV_3_0;	/* VGA color		*/

	header.pcx_encoding	= IMPCXENCODING;

	header.pcx_bitsPerPixel	= 8;		/* 8-bit color		*/

	header.pcx_window[0]	= 0;		/* xmin			*/

	header.pcx_window[1]	= 0;		/* ymin			*/

	header.pcx_window[2]	= width - 1;	/* xmax (minus 1)	*/

	header.pcx_window[3]	= height - 1;	/* ymax (minus 1)	*/

	header.pcx_nPlanes	= 1;		/* 1 color plane	*/

	header.pcx_bytesPerLine	= width;

	if ( header.pcx_bytesPerLine & 0x1 )

		++header.pcx_bytesPerLine;	/* Make it even		*/

	header.pcx_cltType	= IMPCXCOLOR;	/* Yup, its color	*/



	/* Remaining fields left zero.					*/



	if ( ImBinWriteStruct( ioType, fd, fp, &header, imPcxHeaderFields )==-1)

	{

		ImReturnBinError( );

	}



	/*

	 * Output -verbose message

	 */

	ImInfo( "Version", "3.0" );

	ImInfo( "Byte Order", "Least Significant Byte First" );



	sprintf( message, "%d x %d", width, height );

	ImInfo( "Resolution", message);



	ImInfo( "Type", "8-bit Color Indexed" );

	ImInfo( "Color Table", "256 Entries" );

	ImInfo( "Compression Type", "Run Length Encoded (RLE)" );



	/*

	 *  Buffer up, encode, and write out the image data.  Remember that

	 *  an encoded buffer could take up twice as much space as the

	 *  decoded one, in the pathelogical case.

	 */

	ImMalloc( decoded, unsigned char *, header.pcx_bytesPerLine );

	ImMalloc( encoded, unsigned char *, 2 * header.pcx_bytesPerLine );

	memset( (void *)decoded, 0x00, header.pcx_bytesPerLine );



	pPixel = ImVfbQFirst( vfb );

	for ( y = 0; y < height; y++ )

	{

		pDecoded = decoded;

		for ( x = 0; x < width; x++ )

		{

			*pDecoded++ = ImVfbQIndex8( vfb, pPixel ) & 0xFF;

			ImVfbSInc( vfb, pPixel );

		}





		/*

		 *  Encoded it and write it out.

		 */

		n = imPcxEncode( decoded, header.pcx_bytesPerLine, encoded );

		if ( ImBinWrite( ioType, fd, fp, encoded, UCHAR, 1, n ) == -1 )

		{

			free( (char *)encoded );

			free( (char *)decoded );

			ImReturnBinError( );

		}

	}

	free( (char *)encoded );

	free( (char *)decoded );





	/*

	 *  Write out the color table (if any).  Remember to start off the

	 *  color table with the special "yes it's here" byte:  0x0C.

	 */

	if ( clt == IMCLTNULL )

		return ( 1 );			/* No color table	*/



	n = 256 * 3 + 1;			/* Always 256 entries	*/

	ImMalloc( colorBuf, unsigned char *, n );

	memset( (void *)colorBuf, 0x00, n );



	pColorBuf = colorBuf;

	*pColorBuf++ = 0x0C;			/* Yes, it's here	*/



	/* Fill in with CLT entries from the VFB.			*/

	i = ImCltQNColors( clt );

	pColor = ImCltQFirst( clt );

	for ( j = 0; j < i; j++ )

	{

		*pColorBuf++ = ImCltQRed( pColor ) & 0xFF;

		*pColorBuf++ = ImCltQGreen( pColor ) & 0xFF;

		*pColorBuf++ = ImCltQBlue( pColor ) & 0xFF;

		ImCltSInc( clt, pColor );

	}



	/* Pad to 256 entries.						*/

	for ( ; j < 256; j++ )

	{

		*pColorBuf++ = 0;

		*pColorBuf++ = 0;

		*pColorBuf++ = 0;

	}



	/* Write it out.						*/

	if ( ImBinWrite( ioType, fd, fp, colorBuf, UCHAR, 1, n ) == -1 )

	{

		free( (char *)colorBuf );

		ImReturnBinError( );

	}



	free( (char *)colorBuf );

	return ( 2 );				/* VFB + CLT		*/

}



