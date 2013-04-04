/**

 **	$Header: /roq/libim/imrgb.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imrgb.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imrgb.c		-  Iris RGB file I/O

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imrgb.c contains routines to read and write Iris RGB files for

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

 **	imRgbRead		f  read an Iris RGB file

 **	imRgbWriteRLE8		f  write an Iris grayscale file run

 **				   length encoded

 **	imRgbWriteRLERGB	f  write an Iris RGB file run length encoded

 **	imRgbWriteRaw8		f  write an Iris grayscale file no

 **				   compression

 **	imRgbWriteRawRGB	f  write an Iris RGB file no compression

 **

 **	imRgbHeaderInfo		t  RGB file header information

 **	imRgbHeaderFields	v  imPixHeaderInfo description for Bin pkg

 **

 **	imExpandRun		f  expand run length encoding

 **	imRgbEncode		f  compute run length encoding

 **	imRgbFrBuf		m  free internal buffers

 **

 **  HISTORY

 **	$Log: /roq/libim/imrgb.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.22  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.21  1995/06/15  21:10:58  bduggan

 **	changed bzero to memset

 **

 **	Revision 1.20  1995/04/14  23:06:47  bduggan

 **	made rle the default

 **

 **	Revision 1.19  1995/04/03  21:35:40  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.18  1995/01/10  23:41:55  bduggan

 **	made read/write routines static

 **	put in IMMULTI, IMPIPE instead of TRUE/FALSE

 **

 **	Revision 1.17  94/10/03  11:30:49  nadeau

 **	Added read and write support for RGB+Alpha images, compressed

 **	or verbatim.

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

 **	Revision 1.16  92/12/09  16:52:26  nadeau

 **	Fixed bug in reading of RGB RLE images that incorrectly computed

 **	the maximum size needed for a runbuffer.  This caused memory to

 **	be overwritten and a core dump the next time malloc referenced

 **	its free list for allocating a new memory block.

 **	

 **	Revision 1.15  92/12/03  01:51:56  nadeau

 **	Corrected info messages.

 **	

 **	Revision 1.14  92/11/23  18:43:11  nadeau

 **	Removed use of IMINFOMSG.

 **	

 **	Revision 1.13  92/11/04  12:06:37  groening

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.12  92/10/19  14:12:29  groening

 **	fixed bug in dealing with the name field in the header

 **	

 **	Revision 1.11  92/09/29  17:57:23  vle

 **	Added ImInfo messages.

 **	

 **	Revision 1.10  92/08/31  17:34:08  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.9  92/06/12  12:58:01  vle

 **	Updated image format description.

 **	

 **	Revision 1.8  92/04/03  18:11:40  vle

 **	Added 8-bit image read and write support.  Cleaned up code

 **	a little.

 **	

 **	Revision 1.7  92/01/28  15:45:51  u27101

 **	Cleaned up old comments, added documentation for format, rewrote

 **	imRgbRead to process the image on disk instead of reading in the

 **	whole file first and then processing. - Vinh

 **	

 **	Revision 1.6  92/01/16  15:24:36  nadeau

 **	With Vinh's help, corrected file read code that failed to

 **	use the rowstart table to determine the runbuffer start of

 **	each encoded scanline of pixels.

 **	

 **	Revision 1.5  91/10/03  09:17:22  nadeau

 **	Fixed #includes.

 **	

 **	Revision 1.4  91/03/18  15:46:10  todd

 **	added a call to bzero() to fix a bug on the cray

 **	

 **	Revision 1.3  91/03/08  16:17:08  todd

 **	Rewrote imRgbWriteRLE from scratch.

 **	Optimized the rest.

 **	Removed MinPixel and MaxPixel code.  histo is the only rgb program

 **	that used this information.

 **	

 **	Revision 1.2  91/03/02  18:34:04  nadeau

 **	Removed the tag table checking and VFB conversion now

 **	handled by ImFileRead and ImFileWrite.

 **	

 **	Revision 1.1  91/01/31  08:34:53  soraya

 **	Initial revision

 **	

 **	Revision 1.0  90/05/04  09:47:00 jesus ferrer 

 **	Initial revision

 **	

 **/



#include "iminternal.h"







/**

 **  FORMAT

 **	RGB	- Silicon Graphics IRIS image

 **

 **  AKA

 ** 	sgi

 **

 **  FORMAT REFERENCES

 **	 Silicon Graphics RGB Specification, Silicon Graphics Inc.

 **

 **  CODE CREDITS

 **     Custom development, Soraya Gonzalez, San Diego Supercomputer Center, 1991.

 **

 **  DESCRIPTION

 **	Silicon Graphics' RGB format, also know as SGI format, is used

 **	on the Silicon Graphics IRIS family of computers.  Images of this

 **	format are generated by iristools.  This format was created by

 **	Paul Haeberli, 1984.

 **

 **	Additional format information can be derived from:

 **		4Dgifts/iristools code

 **

 **

 **	The IRIS RGB format consists of two segments, a 512 byte header

 **	segment and a data segment, which length varies depending on

 **	the image size and if a compression scheme is used.

 **

 **	Header Segment Format (header size 512 bytes)

 **	---------------------

 **

 **	Name		Type	 Size		Comments

 **	----		----	 ----		--------

 **	imagic		unsigned short	 2 bytes	imagic = 0x01da,

 **					      /4Dgifts/iristools/libimage/open.c

 **						  uses this to determine the

 **						  byte ordering and revision

 **						  number (see dorev)

 **	type		unsigned short	 2 bytes	storage type:

 **						  type = RLE (0x0100) 

 **							 RLE compression

 **						  type = VERBATIM (0x0000)

 **							 no compression

 **						type or'ed with BBP

 **							(Bytes Per Pixel)

 **						  BPP = 0x01, normally used

 **						  BPP = 0x02 

 **	dim		unsigned short	 2 bytes	image dimensions

 **						  dim = 0x01, xsize only

 **						  dim = 0x02, xsize and ysize

 **						  dim = 0x03, xsize, ysize

 **						        and depth

 **	xsize		unsigned short	 2 bytes	width

 **	ysize		unsigned short	 2 bytes	height

 **	zsize		unsigned short	 2 bytes	depth

 **                                               zsize = 1 for 8-bit or

 **                                                     16-bit image,

 **                                                     depending on BPP

 **							(grayscale image)

 **                                               zsize = 2 for 16-bit or

 **                                                     32-bit image,

 **                                                     depending on BPP

 **							(never encountered yet)

 **                                               zsize = 3 for 24-bit or

 **                                                     48-bit image,

 **                                                     depending on BPP

 **                                                     (usually this type)

 **						  zsize = 4 for 32 bit or

 **							64 bit image,

 **							depending on BPP

 **							(for alpha channels)

 **	min		int	 4 bytes	minimum pixel value

 **	max		int	 4 bytes	maximum pixel value

 **	wastebytes	int	 4 bytes	number of bytes wasted by image

 **	name		char	80 bytes	name of image, null terminated

 **						  string

 **	colormap	int	 4 bytes	color map type

 **						 colormap = CM_NORMAL (0x0000)

 **						 colormap = CM_DITHERED (0x0001)

 **						 colormap = CM_SCREEN (0x0002)

 **						 colormap = CM_COLORMAP (0x0003)

 **	file		long	 4 bytes	*not used

 **	flags		unsigned short	 2 bytes	*not used

 **	dorev		short	 2 bytes	revision number

 **						  dorev = 0, most significant

 **							     byte first

 **						  dorev = 1, least significant

 **							     byte first

 **	x		short	 2 bytes	*not used

 **	y		short	 2 bytes	*not used

 **	z		short	 2 bytes	*not used

 **	cnt		short	 2 bytes	*not used

 **	*ptr		unsigned short	 4 bytes	*not used

 **	*base		unsigned short	 4 bytes	*not used

 **	*tmpbuf		unsigned short	 4 bytes	*not used

 **	offset		unsigned long	 4 bytes	*not used

 **	rleend		unsigned long	 4 bytes	file location of end of RLE

 **	*rowstart	unsigned long	 4 bytes	file location of row start table

 **	*rowsize	long	 4 bytes	file location of row size table

 **	pad		char   360 bytes	end of header padding

 **

 **

 **	*not used - these fields are used only in the 4Dgifts code, so do not

 **		    provide any useful information for the image 

 **

 **

 **

 **	Data Segment Format

 **	-------------------

 **

 **	Currently, there are two formats for the data segment, VERBATIM data

 **	and RLE compressed data.

 **

 **

 **	The VERBATIM data format:

 ** 

 **	For depth = 1,

 **		grayscale plane

 ** 

 **	For depth = 3,

 **		R plane

 **		G plane

 **		B plane

 **	This is the normal plane interleaved convention.

 **

 **	For depth = 4,

 **		R plane

 **		G plane

 **		B plane

 **		alpha plane

 **	

 **	

 **	The RLE compressed data format:

 **	

 **	For the RLE compressed data format, there are two tables that are used.

 **	These two tables are pointed to by two pointers (file offsets from the

 **	beginning of the file) in the header, *rowstart and *rowsize.

 **

 **	unsigned long startTable [ height * depth ]	This table contains file offsets

 **						from the beginning of the file

 **						to locations in the file where

 **						an RLE chunk for one scanline

 **						is stored.

 **

 **	unsigned long sizeTable [ height * depth ]	This table contains the size of

 **						each RLE scanline chunk.  One

 **						RLE scanline chunk decompresses

 **						into one scanline.

 **

 **	For depth = 1, the tables are not divided:

 **		[0..height-1]			for grayscale

 **

 **	For depth = 3, the tables are divided into three parts:

 **		[0..height-1] 			for R

 **		[height..2*height-1]		for G

 **		[2*height..3*height-1]		for B

 **	(in other words, the tables are plane interleaved)

 ** 

 **	For depth = 4, the tables are divided into three parts:

 **		[0..height-1] 			for R

 **		[height..2*height-1]		for G

 **		[2*height..3*height-1]		for B

 **		[3*height..4*height-1]		for alpha

 **

 **

 **	To decompress one complete grayscale scanline,

 **

 **	1) use the startTable to find the file offset for one RLE compressed

 **		grayscale scanline chunk

 **	2) use the sizeTable to get the RLE compressed grayscale scanline

 **		chunk size

 **	3) read in and decompress the RLE compressed grayscale scanline

 **		chunk to get the real grayscale scanline

 **

 **	To decompress one complete RGB scanline,

 **

 **	1) use the startTable to find the file offset for one RLE compressed

 **		R scanline chunk

 **	2) use the sizeTable to get the RLE compressed R scanline chunk size

 **	3) read in and decompress the RLE compressed R scanline chunk to

 **		get the real R scanline

 **	4) use the startTable to find the file offset for one RLE compressed

 **		G scanline chunk

 **	5) use the sizeTable to get the RLE compressed G scanline chunk size

 **	6) read in and decompress the RLE compressed G scanline chunk to

 **		get the real G scanline

 **	7) use the startTable to find the file offset for one RLE compressed

 **		B scanline chunk

 **	8) use the sizeTable to get the RLE compressed B scanline chunk size

 **	9) read in and decompress the RLE compressed B scanline chunk to

 **		get the real B scanline

 **	10) combine the real R, G, and B scanlines to get the full RGB scanline

 **

 **

 **	RLE decompression: (for byte type IRIS RGB images only)

 **

 **	There are two types of RLE data segment bytes: control data bytes

 **	and real data bytes.

 **

 **	A control data byte consists of a 1 bit compression flag and 7 bit

 **	count value.

 **		________  ________ ________ ...

 **		^^     ^  ^

 **		||_____|  |____ next bytes...

 **		|   |

 **		|   |______ count value (range 1 - 127, 0 means end of scanline)

 **		|______ compression flag (most significant bit)

 **

 **	compression flag = 0, copy the next byte [count value] times,

 **				i.e. run compression

 **	compression flag = 1, copy the next [count value] bytes literally,

 **				i.e. no run compression

 **

 **	A real data byte is just an 8-bit R, G, or B value to be copied.

 **	

 **	To decompress,

 **		1) read in a control byte

 **		2) if [count value] == 0, end of scanline, else 3)

 **		3) based on compression flag, expand run segment accordingly

 **		4) repeat from 1)

 **

 ** IMPLEMENTATION SPECIFIC NOTES

 **

 **	1) Only 1 Byte Per Plane type images are supported, although 2

 **	   Bytes Per Plane type images are possible.  All images

 **	   encountered so far are 1 Byte Per Plane.

 **	2) Byte ordering swapped images are handled.

 **

 **/



/*

 *  RGB - Silicon Graphics Iris RGB file

 *      For information on these structures, how to use them, etc. please

 *      see imfmt.c.

 */

#ifdef __STDC__

static int imRgbRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imRgbWriteRaw8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 

	TagTable *tagTable);

static int imRgbWriteRLE8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 

	TagTable *tagTable);

static int imRgbWriteRawRGB( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 

	TagTable *tagTable);

static int imRgbWriteRLERGB( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 

	TagTable *tagTable);

static int imRgbWriteRawRGBA( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 

	TagTable *tagTable);

static int imRgbWriteRLERGBA( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 

	TagTable *tagTable);

#else

static int imRgbRead( );

static int imRgbWriteRaw8( ), imRgbWriteRLE8( );

static int imRgbWriteRawRGB( ), imRgbWriteRLERGB( );

static int imRgbWriteRawRGBA( ), imRgbWriteRLERGBA( );

#endif

static char *imRgbNames[ ]  = { "rgb", "iris", "sgi", NULL };

static unsigned char imRgbMagicNumber[ ] = { 0x01, 0xDA };

static ImFileFormatReadMap imRgbReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IN,1,8,       LI,     IMVFBINDEX8,    0 },

        { IN,1,8,       RLE,    IMVFBINDEX8,    0 },

        { RGB,3,8,      LI,     IMVFBRGB,       0 },

        { RGB,3,8,      RLE,    IMVFBRGB,       0 },

	{ RGB,4,8,	LI|A,	IMVFBRGB,	A },

	{ RGB,4,8,	RLE|A,	IMVFBRGB,	A },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imRgbWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBINDEX8,  0,      IN,1,8,         RLE,    imRgbWriteRLE8 },

        { IMVFBINDEX8,  0,      IN,1,8,         LI,     imRgbWriteRaw8 },

        { IMVFBRGB,     0,      RGB,3,8,        RLE,    imRgbWriteRLERGB },

        { IMVFBRGB,     0,      RGB,3,8,        LI,     imRgbWriteRawRGB },

        { IMVFBRGB,     A,      RGB,4,8,        RLE|A,  imRgbWriteRLERGBA },

        { IMVFBRGB,     A,      RGB,4,8,        LI|A,   imRgbWriteRawRGBA },

        { -1,           0,      -1,             0,      NULL },

};



static ImFileMagic imFileRgbMagic []=

{

	{ 0, 2, imRgbMagicNumber},

	{ 0, 0, NULL },

};



ImFileFormat ImFileRgbFormat =

{

	imRgbNames, "SGI RGB image file",

	"Silicon Graphics, Inc.",

	"8-bit color index without CLT and 24-bit RGB color images with optional alpha channels,\n\
uncompressed (verbatim) and RLE-compressed.",
	"8-bit color index without CLT and 24-bit RGB color images with optional alpha channels,\n\
uncompressed (verbatim) and RLE-compressed.",

	imFileRgbMagic,

	IMNOMULTI, IMPIPE,

	IMNOMULTI, IMNOPIPE,

	imRgbRead, imRgbReadMap, imRgbWriteMap

};



#ifndef L_SET

#define L_SET   0               /* Absolute offset                      */

#define L_INCR  1               /* Relative to current offset           */

#define L_XTND  2               /* Relative to end of file              */

#endif



#ifndef F_SET

#define F_SET   0               /* Absolute offset                      */

#define F_INCR  1               /* Relative to current offset           */

#define F_XTND  2               /* Relative to end of file              */

#endif





/*

 *  TYPEDEF & STRUCTURE

 *	imRgbHeaderInfo		-  RGB file header information

 *	imRgbHeaderFields	-  RGB file header fields for binary pkg.

 *

 *  DESCRIPTION

 *	A RGB file's header contains the image's width, height, and depth,

 *	and two unused dummy fields.

 */



typedef struct imRgbHeaderInfo

{

	unsigned short rgb_imagic;	/* Magic number (0x01da)	*/

	unsigned short rgb_stype;	/* Storage type			*/

	unsigned short rgb_dim;		/* Number of dimension of image	*/

	unsigned short rgb_width;	/* Image width			*/

	unsigned short rgb_height;	/* Image height			*/

	unsigned short rgb_zsize;	/* # channels is 3 or 4 in RGB images*/

	int    rgb_min;		/* Minimun pixel value			*/

	int    rgb_max;		/* Maximun pixel value			*/

	int    rgb_wastebytes;	/* Number of bytes wasted by image	*/

	char   rgb_name[80];	/* Image name				*/

	int    rgb_colormap;	/* Color map type			*/



	char   rgb_dummy[32];	/* Unused dummy field			*/



	unsigned int   rgb_rleend;	/* File location of end of rle data	*/

	unsigned int   rgb_rowstart;	/* File location of row start table	*/

	unsigned int   rgb_rowsize;	/* File location of row size table	*/

	char   rgb_pad[360];	/* Unused dummy field 			*/

} imRgbHeaderInfo;



static BinField imRgbHeaderFields[ ] =

{

	{ USHORT, 2, 1 }, 		/* rgb_imagic			*/

	{ USHORT, 2, 1 },		/* rgb_type			*/

	{ USHORT, 2, 1 },		/* rgb_dim			*/

	{ USHORT, 2, 1 },		/* rgb_width			*/

	{ USHORT, 2, 1 },		/* rgb_height			*/

	{ USHORT, 2, 1 },		/* rgb_zsize			*/

	{ INT, 4, 1 },			/* rgb_min			*/

	{ INT, 4, 1 },			/* rgb_max			*/

	{ INT, 4, 1 },			/* rgb_wastedbytes		*/

	{ CHAR, 1, 80 },		/* rgb_name			*/

	{ INT, 4, 1 },			/* rgb_colormap			*/



	{ CHAR, 1, 32 },		/* rgb_dummy			*/



	{ UINT, 4, 1 },			/* rgb_rleend			*/

	{ UINT, 4, 1 },			/* rgb_rowstart			*/

	{ UINT, 4, 1 },			/* rgb_rowsize			*/

	{ CHAR, 1, 360 },		/* rgb_pad			*/

	{ 0, 0, 0 }

};



/*

 *  MACRO

 *      imRgbFrBuf -  Free six chunks of memory

 *

 *  DESCRIPTION

 *      Call free six times to free the six chunks of memory pointed to by

 *      a-f.

 */



#define imRgbFrBuf(a,b,c,d,e,f) free((a));free((b));free((c));free((d));free((e));free((f))

#define imRgbFrBufAlp(a,b,c,d,e,f,g) free((a));free((b));free((c));free((d));free((e));free((f));free((g))





/*

 *  	Various useful RGB specific constants and macros

 */



#define IMRGB_ITYPE_RLE		0x0100

#define IMRGB_ITYPE_VERBATIM	0x0000

#define IMRGB_ITYPE_BPP1	0x0001

#define IMRGB_ISRLE(type)	(((type) & 0xff00) == IMRGB_ITYPE_RLE)

#define IMRGB_ISVERBATIM(type) 	(((type) & 0xff00) == IMRGB_ITYPE_VERBATIM)

#define IMRGB_ISBPP1(type) 	(((type) & 0x00ff) == IMRGB_ITYPE_BPP1)

#define	IMRGBHEADERSIZE		(512L)

#define IMRGB_BYTES_PER_PLANE	0x01

#define IMRGB_IMAGIC_MBF	0x01da

#define IMRGB_IMAGIC_LBF	0xda01









/*

 *  FUNCTION

 *	imExpandRun	- Expand run length encoded pixels 

 *

 *  DESCRIPTION

 *	Expand the runs to create a raw RGB image in the VFB.

 *

 *	RLE coding scheme for 1 byte type images (IMRGB_BYTES_PER_PLANE = 0x01):

 *

 *	Each control byte consists of 1 bit compression flag (F) , and

 *		7 bit count value (C).

 *

 *		F = 0,	write next byte C times, example...

 *

 *			  RLE:	0x85 0x9d

 *			image:	0x9d 0x9d 0x9d 0x9d 0x9d

 *

 *		F = 1, write next C bytes literally, example...

 *

 *			  RLE:	0x05 0xff 0x12 0x32 0xaf 0x1d

 *			image:	0xff 0x12 0x32 0xaf 0x1d

 *

 *	Data bytes use all 8 bits and follow a control byte or other

 *		data bytes.

 */

static void

#ifdef __STDC__

imExpandRun(ImVfb *vfb,ImVfbPtr vfbptr,unsigned char *rbp,int channel)

#else

imExpandRun(vfb,vfbptr,rbp,channel)

	ImVfb        *vfb;	/* Read in image			*/

	ImVfbPtr     vfbptr;	/* Pointer to vfb			*/

	unsigned char *rbp;	/* Pointer to output buffer		*/

	int	     channel;	/* Channel to be processed

				   (Grayscale,Red,Green,Blue,Alpha)	 	*/

#endif

{	

	ImVfbPtr vfbpfirst;

	unsigned short	count;

	unsigned short	c;      /* color component			*/



	vfbpfirst = vfbptr;

	switch ( channel )

	{

	case 0 :		/* Color index				*/

		while ( count = (*rbp & 0x7f) )

		{ 

		   if ( *rbp & 0x80 )

			for ( ; (count > 0); count-- ) 

			{

			   (rbp)++;

			   c = *rbp;

			   ImVfbSIndex8( vfb, vfbptr,  c );

			   ImVfbSInc( vfb , vfbptr );

			}

		   else 

		   {

			(rbp)++;

			c = *rbp;

			for ( ; ( count > 0); count-- )

			{

			   ImVfbSIndex8( vfb, vfbptr,  c );

			   ImVfbSInc( vfb, vfbptr );

			}

		   }

		   (rbp)++;

		}

		(rbp)++;

		break;



	case 1 :		/* Red channel				*/

		while ( count = (*rbp & 0x7f) )

		{

		   if ( *rbp & 0x80 )

			for ( ; (count > 0); count-- ) 

			{

			   (rbp)++;

			   c = *rbp;

			   ImVfbSRed( vfb, vfbptr,  c );

			   ImVfbSInc( vfb , vfbptr );

			}

		   else 

		   {

			(rbp)++;

			c = *rbp;

			for ( ; ( count > 0); count-- )

			{

			   ImVfbSRed( vfb, vfbptr, c );

			   ImVfbSInc( vfb, vfbptr );

			}

		   }

		   (rbp)++;

		}

		(rbp)++;

		break;



	 case 2 :		/* Green channel			*/

		while ( count = (*rbp & 0x7f) )

		{

		   if ( *rbp & 0x80 )

			for ( ; (count > 0); count-- ) 

			{

			   (rbp)++;

			   c = *rbp;

			   ImVfbSGreen( vfb, vfbptr, c );

			   ImVfbSInc( vfb , vfbptr );

			}

		   else 

		   {

			(rbp)++;

			c = *rbp;

			for ( ; ( count > 0); count-- )

			{

			   ImVfbSGreen( vfb, vfbptr, c );

			   ImVfbSInc( vfb, vfbptr );

			}

		   }

		   (rbp)++;

		}

		(rbp)++;

		break;



	  case 3 :		/* Blue channel				*/

		while ( count = (*rbp & 0x7f) )

		{ 

		   if ( *rbp & 0x80 )

			for ( ; (count > 0); count-- ) 

			{

			   (rbp)++;

			   c = *rbp;

			   ImVfbSBlue( vfb, vfbptr,  c );

			   ImVfbSInc( vfb , vfbptr );

			}

		   else 

		   {

			(rbp)++;

			c = *rbp;

			for ( ; ( count > 0); count-- )

			{

			   ImVfbSBlue( vfb, vfbptr,  c );

			   ImVfbSInc( vfb, vfbptr );

			}

		   }

		   (rbp)++;

		}

		(rbp)++;

		break;

	  case 4 :		/* Alpha channel				*/

		while ( count = (*rbp & 0x7f) )

		{ 

		   if ( *rbp & 0x80 )

			for ( ; (count > 0); count-- ) 

			{

			   (rbp)++;

			   c = *rbp;

			   ImVfbSAlpha( vfb, vfbptr,  c ); 

			   ImVfbSInc( vfb , vfbptr );

			}

		   else 

		   {

			(rbp)++;

			c = *rbp;

			for ( ; ( count > 0); count-- )

			{

			   ImVfbSAlpha( vfb, vfbptr,  c ); 

			   ImVfbSInc( vfb, vfbptr );

			}

		   }

		   (rbp)++;

		}

		(rbp)++;

		break;

	}  /* End of switch */

}









/*

 *  FUNCTION

 *	imRgbRead	-  read an Iris RGB file

 *

 *  DESCRIPTION

 *	The file header is read and the size of the image determined.

 *	Space is allocated for the VFB and the run codes read in to

 *	a run buffer.  The run buffer is then expanded into straight

 *	RGB values in the VFB and the completed VFB added to the tag list.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__

imRgbRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRgbRead( ioType, fd, fp, flagsTable, tagTable )

	int         ioType;		/* I/O flags			*/

	int         fd;			/* Input file descriptor	*/

	FILE	   *fp;			/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable   *tagTable;		/* Tag table to add to		*/

#endif

{

	ImVfb        	*vfb;		/* Read in image		*/

	ImVfbPtr     	pptr;		/* Pixel pointer		*/

	imRgbHeaderInfo header;		/* RGB file header		*/

	char	    	*rgbName;	/* Name of the rgb image in file*/

	unsigned char   *runBuffer;	/* Run buffer			*/

	unsigned char   *rbp;		/* Run buffer pointer		*/

	int	     	x,y,z;		/* Convenient short names	*/

	unsigned long   *starttable;	/* Rowstart table		*/	

	long  	     	*sizetable;	/* Rowsize table		*/

	int     xty, xtyt2, xtyt3;	/* Loop invariants		*/

	int     i,j,k;			/* Counters			*/

	int	     	status;		/* Status returned from seek	*/

	unsigned long	     maxrun;	/* Maximum run size		*/

	char		 message[100];	/* ImInfo message		*/



	/*

	 *  Read in magic number in header, first 2 bytes.

	 */

	BinByteOrder( BINMBF );

	if ( ImBinRead( ioType, fd, fp, &(header.rgb_imagic),USHORT,2,1) == -1 )

	{

		ImReturnBinError( );

	}

	

	/*

	 *  Use magic number to determine byte ordering of rest

	 *  of file and to make sure we can handle this particular

	 *  image type.

	 */

	switch ( header.rgb_imagic )

	{

		case IMRGB_IMAGIC_MBF:

			BinByteOrder( BINMBF );

			break;

		case IMRGB_IMAGIC_LBF:	

			BinByteOrder( BINLBF );

			break;

		default:

			/*

			 * Bad magic number.

			 */

			ImErrorFatal( ImQError( ), -1, IMEMAGIC );

	}



	/*

	 *  Reset fd or fp to beginning of file.  Read in complete header.

	 */

	status = ImSeek( ioType, fd, fp, 0, L_SET );

	if ( status == -1 )

	{

		ImErrorFatal( ImQError( ), -1, IMESYS );

	}

	if ( ImBinReadStruct( ioType, fd, fp, &header, imRgbHeaderFields )== -1)

	{

		ImReturnBinError( );

	}



	/*

	 *  Make sure we support the number of Bytes Per Plane.

	 *  Currently, only 1 Byte Per Plane is supported.

	 */

	if( !IMRGB_ISBPP1( header.rgb_stype ) )

	{

		ImErrorFatal( ImQError( ), -1, IMEPLANES );

	}

		

	x = header.rgb_width;

	y = header.rgb_height;

	z = header.rgb_zsize;



	/*

	 * Output -verbose message

	 */

	ImInfo( "Image Name", header.rgb_name );

	switch ( header.rgb_imagic )

	{

		case IMRGB_IMAGIC_MBF:

			ImInfo( "Byte Order", "Most Significant Byte First" );

			break;

		case IMRGB_IMAGIC_LBF:	

			ImInfo( "Byte Order", "Least Significant Byte First" );

			break;

	}

	sprintf( message, "%d x %d", x, y );

	ImInfo( "Resolution", message );



	switch( z )

	{

		case 1:

			ImInfo( "Type", "8-bit Grayscale" );

			ImInfo( "Alpha Channel", "none" );

			break;

		case 3:

			ImInfo( "Type", "24-bit RGB" );

			ImInfo( "Alpha Channel", "none" );

			break;

		case 4:

			ImInfo( "Type", "32-bit RGB" );

			ImInfo( "Alpha Channel", "8-bit" );

			break;

	}



	if( IMRGB_ISRLE( header.rgb_stype ) )

	{

		ImInfo( "Compression Type", "Run Length Encoded (RLE)" );

	}

	else

		ImInfo( "Compression Type", "none (Verbatim)" );



	/*

	 *  Allocate a VFB of the required size and type.

	 */

	switch( z )

	{

		case 1:		/* 8-bit grayscale image */

		    if( (vfb = ImVfbAlloc( x, y, IMVFBINDEX8 )) == IMVFBNULL )

		    {

				ImErrorFatal( ImQError( ), -1, ImErrNo );

		    }

		    break;



		case 3:		/* 24-bit RGB image */

		    if( (vfb = ImVfbAlloc( x, y, IMVFBRGB )) == IMVFBNULL )

		    {

				ImErrorFatal( ImQError( ), -1, ImErrNo );

		    }

		    break;

		case 4:		/* 32-bit RGB image + alpha */

		    if( (vfb = ImVfbAlloc( x, y, IMVFBRGB|IMVFBALPHA )) == IMVFBNULL )

		    {

				ImErrorFatal( ImQError( ), -1, ImErrNo );

		    }

		    break;



		default:	/* unsupported image depth */

		    ImErrorFatal( ImQError( ), -1, IMEDEPTH );

	}



	/*

	 * Use image type to determine whether to do RLE expansion into

	 * vfb or do verbatim copy into vfb.

	 */ 

	pptr = ImVfbQFirst( vfb );

	if( IMRGB_ISRLE( header.rgb_stype ) )

	{

		/*

		 *  Allocate row start and row size tables.

		 */

		ImMalloc( starttable, unsigned long *, y * z * sizeof( unsigned long ) );

		ImMalloc( sizetable,  long *, y * z * sizeof( long ) );



		/*

		 *  Read rowstart table & rowsize tables.

		 */

		if( ImBinRead( ioType, fd,fp,starttable,ULONG, 4, y * z) == -1 )

		{

			free( (char *)starttable );

			free( (char *)sizetable );

			ImReturnBinError( );

		}

		if( ImBinRead( ioType, fd, fp, sizetable,LONG, 4, y * z) == -1 )

		{

			free( (char *)starttable );

			free( (char *)sizetable );

			ImReturnBinError( );

		}

		/*

		 * Allocate worst case runBuffer, largest size in sizetable

		 */

		maxrun = sizetable[0];

		for( j = 1; j < y*z; j++ )

		{

			if( sizetable[j] > maxrun )

			{

				maxrun = sizetable[j];

			}

		}

		ImMalloc( runBuffer, unsigned char *, maxrun+1 );

		rbp = runBuffer;



		/*

		 * Check image depth, z, and expand accordingly.

		 */

		switch( z )

		{

		  case 1:	/* 8-bit grayscale image */

		     for( j = 0; j < y; j++ )

		     {

			/*

			 * Read and expand a Grayscale scanline

			 */

			status = ImSeek( ioType,fd,fp,starttable[j],L_SET );

			if ( status == -1 )

			{

				ImErrorFatal( ImQError( ), -1, IMESYS );

			}

			if ( ImBinRead ( ioType, fd, fp, runBuffer,

				UCHAR, 1, sizetable[j]) == -1 )

			{

				free( (char *)runBuffer );

				free( (char *)starttable );

				free( (char *)sizetable );

				ImReturnBinError( );

			}

			imExpandRun( vfb, pptr, rbp, 0 );



			/*

			 * Move to next scanline in vfb

			 */

			pptr = ImVfbQDown( vfb, pptr );

		     }

		     break;



		  case 3:	/* 24-bit RGB image */

		     /*

		      *  Read in each of the R, G, and B scanlines and alpha in

		      *  turn and expand the runs into the VFB.

		      */

		     for ( j = 0; j < y; j++ )

		     {

			/*

			 * Read and expand a Red scanline

			 */

			status = ImSeek( ioType,fd,fp,starttable[0*y+j],L_SET );

			if ( status == -1 )

			{

				ImErrorFatal( ImQError( ), -1, IMESYS );

			}

			if ( ImBinRead ( ioType, fd, fp, runBuffer,

				UCHAR, 1, sizetable[0*y+j]) == -1 )

			{

				free( (char *)runBuffer );

				free( (char *)starttable );

				free( (char *)sizetable );

				ImReturnBinError( );

			}

			imExpandRun( vfb, pptr, rbp, 1 );



			/*

			 * Read and expand a Blue scanline

			 */

			status = ImSeek( ioType,fd,fp,starttable[1*y+j],L_SET );

			if ( status == -1 )

			{

				ImErrorFatal( ImQError( ), -1, IMESYS );

			}

			if ( ImBinRead ( ioType, fd, fp, runBuffer,

				UCHAR, 1, sizetable[1*y+j]) == -1 )

			{

				free( (char *)runBuffer );

				free( (char *)starttable );

				free( (char *)sizetable );

				ImReturnBinError( );

			}

			imExpandRun( vfb, pptr, rbp, 2 );



			/*

			 * Read and expand a Green scanline

			 */

			status = ImSeek( ioType,fd,fp,starttable[2*y+j],L_SET );

			if ( status == -1 )

			{

				ImErrorFatal( ImQError( ), -1, IMESYS );

			}

			if ( ImBinRead ( ioType, fd, fp, runBuffer,

				UCHAR, 1, sizetable[2*y+j]) == -1 )

			{

				free( (char *)runBuffer );

				free( (char *)starttable );

				free( (char *)sizetable );

				ImReturnBinError( );

			}

			imExpandRun( vfb, pptr, rbp, 3 );



			/*

			 * Move to next scanline in vfb

			 */

			pptr = ImVfbQDown( vfb, pptr );

		    }  /* End of For */

		    break;



		  case 4:	/* 32-bit RGB image + alpha channel */

		     /*

		      *  Read in each of the R, G, and B scanlines and possibly alpha in

		      *  turn and expand the runs into the VFB.

		      */

		     for ( j = 0; j < y; j++ )

		     {

			/*

			 * Read and expand a Red scanline

			 */

			status = ImSeek( ioType,fd,fp,starttable[0*y+j],L_SET );

			if ( status == -1 )

			{

				ImErrorFatal( ImQError( ), -1, IMESYS );

			}

			if ( ImBinRead ( ioType, fd, fp, runBuffer,

				UCHAR, 1, sizetable[0*y+j]) == -1 )

			{

				free( (char *)runBuffer );

				free( (char *)starttable );

				free( (char *)sizetable );

				ImReturnBinError( );

			}

			imExpandRun( vfb, pptr, rbp, 1 );



			/*

			 * Read and expand a Blue scanline

			 */

			status = ImSeek( ioType,fd,fp,starttable[1*y+j],L_SET );

			if ( status == -1 )

			{

				ImErrorFatal( ImQError( ), -1, IMESYS );

			}

			if ( ImBinRead ( ioType, fd, fp, runBuffer,

				UCHAR, 1, sizetable[1*y+j]) == -1 )

			{

				free( (char *)runBuffer );

				free( (char *)starttable );

				free( (char *)sizetable );

				ImReturnBinError( );

			}

			imExpandRun( vfb, pptr, rbp, 2 );



			/*

			 * Read and expand a Green scanline

			 */

			status = ImSeek( ioType,fd,fp,starttable[2*y+j],L_SET );

			if ( status == -1 )

			{

				ImErrorFatal( ImQError( ), -1, IMESYS );

			}

			if ( ImBinRead ( ioType, fd, fp, runBuffer,

				UCHAR, 1, sizetable[2*y+j]) == -1 )

			{

				free( (char *)runBuffer );

				free( (char *)starttable );

				free( (char *)sizetable );

				ImReturnBinError( );

			}

			imExpandRun( vfb, pptr, rbp, 3 );



			/*

			 * Read and expand an alpha scanline

			 */

			status = ImSeek( ioType,fd,fp,starttable[3*y+j],L_SET );

			if ( status == -1 )

			{

				ImErrorFatal( ImQError( ), -1, IMESYS );

			}

			if ( ImBinRead ( ioType, fd, fp, runBuffer,

				UCHAR, 1, sizetable[3*y+j]) == -1 )

			{

				free( (char *)runBuffer );

				free( (char *)starttable );

				free( (char *)sizetable );

				ImReturnBinError( );

			}

			imExpandRun( vfb, pptr, rbp, 4 );

			/*

			 * Move to next scanline in vfb

			 */

			pptr = ImVfbQDown( vfb, pptr );



			} /* End of for loop */

			break;



		  default:	/* unsupported image depth */

		    ImErrorFatal( ImQError( ), -1, IMEDEPTH );

		} /* end switch */



		/*

		 * free starttable and sizetable

		 */



		free( (char *)sizetable );

		free( (char *)starttable );

	}

	else /* VERBATIM format */

	{

		/*

		 *  Allocate a run buffer large enough for the image.

		 *  Read it in.

		 */

		ImMalloc( runBuffer, unsigned char *, 4 * x * y );

		if ( ImBinRead( ioType, fd, fp, runBuffer, UCHAR, 1, 4*x*y)==-1)

		{

			free( (char *)runBuffer );

			ImReturnBinError( );

		}



		/*

		 *  Copy it to the VFB.

		 */

		rbp    = runBuffer;

		xty = x * y;

		xtyt2 = x * y * 2;

		xtyt3 = x * y * 3;



		switch( z ) 

		{

		  case 1:	/* 8-bit grayscale image */

		    for ( j = 1; j <= y; j++ )

		    {

			for ( i = 1; i <= x; i++, rbp++ )

			{ 

				ImVfbSIndex8( vfb, pptr, *rbp );

				ImVfbSInc( vfb, pptr );

			}

		    }

		    break;



		  case 3:	/* 24-bit RGB image */

		    for ( j = 1; j <= y; j++ )

		    {

			for ( i = 1; i <= x; i++, rbp++ )

			{ 

				ImVfbSRed( vfb, pptr, *rbp );

				ImVfbSGreen( vfb, pptr, *(rbp+xty) );

				ImVfbSBlue( vfb, pptr, *(rbp+xtyt2) );

				ImVfbSInc( vfb, pptr );

			}

		    }

		    break;



		  case 4:	/* 32-bit RGB image + alpha channel */

		    for ( j = 1; j <= y; j++ )

		    {

			for ( i = 1; i <= x; i++, rbp++ )

			{ 

				ImVfbSRed( vfb, pptr, *rbp );

				ImVfbSGreen( vfb, pptr, *(rbp+xty) );

				ImVfbSBlue( vfb, pptr, *(rbp+xtyt2) );

				ImVfbSAlpha( vfb, pptr, *(rbp+xtyt3) ); 

				ImVfbSInc( vfb, pptr );

			}

		    }

		    break;



		  default:	/* unsupported image depth */

		    ImErrorFatal( ImQError( ), -1, IMEDEPTH );



		} /* end switch */

	}

	ImVfbFlip( vfb, IMVFBYFLIP, vfb );

	free( runBuffer );





	/*

	 * Save the image name in a more permanent location

	 * so we can put it in the tag table

	 */



	if ( strlen( header.rgb_name ) > 0 )

	{

		ImMalloc( rgbName, char *, strlen(header.rgb_name)+1 ); 

		strcpy( rgbName, header.rgb_name );



		TagTableAppend( tagTable,

			TagEntryAlloc( "image name", POINTER, &rgbName ) );

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

 *	imRgbWriteRaw8	-  write an 8-bit uncompressed Iris RGB file (verbatim)

 *

 *  DESCRIPTION

 *	That VFB is queried, and the RGB file header set up and written out.

 *	The VFB data is then converted and written out.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imRgbWriteRaw8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRgbWriteRaw8( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable    *flagsTable;	/* Flags			*/

	TagTable    *tagTable;		/* Tag list to read from	*/

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointers		*/

	imRgbHeaderInfo header;		/* RGB file header		*/

	unsigned char   *runBuffer;	/* Run buffer 			*/

	unsigned char   *rbp;		/* Run buffer point.		*/

	int          n;			/* Counter			*/

	int          x, y;		/* Pixel counters		*/

	TagEntry     *tagEntry;		/* Tmp table entry		*/

	char         *tmpString;	/* Tmp string holder		*/

	int	     i, j;		/* Counters			*/

	char		 message[100];	/* ImInfo message		*/





	/*

	 *  Set up and write out the header.

	 */

	BinByteOrder( BINMBF );



	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );



	header.rgb_stype      = IMRGB_ITYPE_VERBATIM | IMRGB_BYTES_PER_PLANE;

	header.rgb_imagic     = 0x01da;

	header.rgb_dim        = 2;

	header.rgb_width      = ImVfbQWidth( vfb );

	header.rgb_height     = ImVfbQHeight( vfb );

	header.rgb_zsize      = 1;

	header.rgb_min        = 0;

	header.rgb_max        = 255;

	header.rgb_wastebytes = 3;

	header.rgb_colormap   = 0;

	header.rgb_rleend     = 0;

	header.rgb_rowstart   = 0;

	header.rgb_rowsize    = 0;



	tagEntry = TagTableQDirect( tagTable, "image name", 0 );

	if ( tagEntry == TAGENTRYNULL )

		strcpy( header.rgb_name, "untitled" );

	else

	{

		TagEntryQValue( tagEntry, &tmpString );

		strncpy( header.rgb_name, tmpString, 80 );

	}



	if ( ImBinWriteStruct( ioType, fd, fp, &header, imRgbHeaderFields )==-1)

		ImReturnBinError( );



	/*

	 * Output -verbose message

	 */

	ImInfo( "Image Name", header.rgb_name );

	ImInfo( "Byte Order", "Most Significant Byte First" );



	sprintf( message, "%d x %d", header.rgb_width, header.rgb_height );

	ImInfo( "Resolution", message );



	ImInfo( "Type", "8-bit Grayscale" );

	ImInfo( "Compression Type", "none (Verbatim)" );

	ImInfo( "Alpha Channel", "none" );



      	/*   

	 *  Allocate space for a run buffer large enough for :

	 *  (width * height * 1)  unencoded scanlines. 

	 */

	x = header.rgb_width;

	y = header.rgb_height;



	ImMalloc( runBuffer, unsigned char *, y * x );



	rbp  = runBuffer;



	/*

	 *  Output grayscale.

	 */

	pptr = ImVfbQLast( vfb );

	for (i=1; i < x; i++)

		ImVfbSDec( vfb, pptr );

	for (j = 1; (j <= y); j++)

	{

		for (i = 1; (i <= x); i++)

		{

		   *rbp = ImVfbQIndex8( vfb, pptr );

		   ImVfbSInc( vfb, pptr );

		   rbp++;

		}

		ImVfbSDec( vfb, pptr );

		pptr  = ImVfbQUp( vfb, pptr );

		for (i=1; i < x; i++)

			ImVfbSDec( vfb, pptr );

	}



	/*

	 *  Write out the buffer.

	 */

	if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, x * y ) == -1)

	{

		free( (char *)runBuffer );

		ImReturnBinError( );

	}





	free( (char *)runBuffer );

	return ( 1 );

}









/*

 *  FUNCTION

 *	imRgbWriteRawRGB -  write an uncompressed Iris RGB file (verbatim)

 *

 *  DESCRIPTION

 *	That VFB is queried, and the RGB file header set up and written out.

 *	The VFB data is then converted and written out.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imRgbWriteRawRGB( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRgbWriteRawRGB( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable    *flagsTable;	/* Flags			*/

	TagTable    *tagTable;		/* Tag list to read from	*/

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointers		*/

	imRgbHeaderInfo header;		/* RGB file header		*/

	unsigned char  *runBuffer;	/* Run buffer 			*/

	unsigned char  *rbp;		/* Run buffer point.		*/

	int          n;			/* Counter			*/

	int          x, y;		/* Pixel counters		*/

	TagEntry     *tagEntry;		/* Tmp table entry		*/

	char         *tmpString;	/* Tmp string holder		*/

	int	     i, j;		/* Counters			*/

	char		 message[100];	/* ImInfo message		*/





	/*

	 *  Set up and write out the header.

	 */

	BinByteOrder( BINMBF );



	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );



	header.rgb_stype      = IMRGB_ITYPE_VERBATIM | IMRGB_BYTES_PER_PLANE;

	header.rgb_imagic     = 0x01da;

	header.rgb_dim        = 3;

	header.rgb_width      = ImVfbQWidth( vfb );

	header.rgb_height     = ImVfbQHeight( vfb );

	header.rgb_zsize      = 3;

	header.rgb_min        = 0;

	header.rgb_max        = 255;

	header.rgb_wastebytes = 3;

	header.rgb_colormap   = 0;

	header.rgb_rleend     = 0;

	header.rgb_rowstart   = 0;

	header.rgb_rowsize    = 0;



	tagEntry = TagTableQDirect( tagTable, "image name", 0 );

	if ( tagEntry == TAGENTRYNULL )

		strcpy( header.rgb_name, "untitled" );

	else

	{

		TagEntryQValue( tagEntry, &tmpString );

		strncpy( header.rgb_name, tmpString, 80 );

	}



	if ( ImBinWriteStruct( ioType, fd, fp, &header, imRgbHeaderFields )==-1)

		ImReturnBinError( );



	/*

	 * Output -verbose message

	 */

	ImInfo( "Image Name", header.rgb_name );

	ImInfo( "Byte Order", "Most Significant Byte First" );



	sprintf( message, "%d x %d", header.rgb_width, header.rgb_height );

	ImInfo( "Resolution", message );



	ImInfo( "Type", "24-bit RGB" );

	ImInfo( "Compression Type", "none (Verbatim)" );

	ImInfo( "Alpha Channel", "none" );



      	/*   

	 *  Allocate space for a run buffer large enough for :

	 *  (width * height * 3)  unencoded scanlines. 

	 */

	x = header.rgb_width;

	y = header.rgb_height;



	ImMalloc( runBuffer, unsigned char *, 3 * y * x );



	rbp  = runBuffer;

	pptr = ImVfbQLast( vfb );

	for (i=1; i < x; i++)

		ImVfbSDec( vfb, pptr );





	/*

	 *  Red, Green, then Blue channel.

	 */

	for (j = 1; (j <= y); j++)

	{

		for (i = 1; (i <= x); i++)

		{

		   *rbp = ImVfbQRed( vfb, pptr );

		   ImVfbSInc( vfb, pptr );

		   rbp++;

		}

		ImVfbSDec( vfb, pptr );

		pptr  = ImVfbQUp( vfb, pptr );

		for (i=1; i < x; i++)

			ImVfbSDec( vfb, pptr );

	}



	pptr  = ImVfbQLast( vfb );

	for(i=1; i < x; i++)  ImVfbSDec( vfb, pptr );

	for (j = 1; (j <= y); j++)

	{

		for (i = 1; (i <= x); i++)

		{

		   *rbp = ImVfbQGreen( vfb, pptr );

		   ImVfbSInc( vfb, pptr );

		   rbp++;

		}

		ImVfbSDec( vfb, pptr );

		pptr  = ImVfbQUp( vfb, pptr );

		for (i=1; i < x; i++)

			ImVfbSDec( vfb, pptr );

	}



	pptr  = ImVfbQLast( vfb );

	for(i=1; i < x; i++)  ImVfbSDec( vfb, pptr );

	for (j = 1; (j <= y); j++)

	{

		for (i = 1; (i <= x); i++)

		{

		   *rbp = ImVfbQBlue( vfb, pptr );

		   ImVfbSInc( vfb, pptr );

		   rbp++;

		}

		ImVfbSDec( vfb, pptr );

		pptr  = ImVfbQUp( vfb, pptr );

		for (i=1; i < x; i++)

			ImVfbSDec( vfb, pptr );

	}





	/*

	 *  Write out the buffer.

	 */

	if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, x * y * 3 ) == -1)

	{

		free( (char *)runBuffer );

		ImReturnBinError( );

	}





	free( (char *)runBuffer );

	return ( 1 );

}





/*

 *  FUNCTION

 *	imRgbWriteRawRGBA -  write an uncompressed Iris RGB file (verbatim)

 *

 *  DESCRIPTION

 *	That VFB is queried, and the RGB file header set up and written out.

 *	The VFB data is then converted and written out.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imRgbWriteRawRGBA( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRgbWriteRawRGBA( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable    *flagsTable;	/* Flags			*/

	TagTable    *tagTable;		/* Tag list to read from	*/

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointers		*/

	imRgbHeaderInfo header;		/* RGB file header		*/

	unsigned char  *runBuffer;	/* Run buffer 			*/

	unsigned char  *rbp;		/* Run buffer point.		*/

	int          n;			/* Counter			*/

	int          x, y;		/* Pixel counters		*/

	TagEntry     *tagEntry;		/* Tmp table entry		*/

	char         *tmpString;	/* Tmp string holder		*/

	int	     i, j;		/* Counters			*/

	char		 message[100];	/* ImInfo message		*/





	/*

	 *  Set up and write out the header.

	 */

	BinByteOrder( BINMBF );



	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );



	header.rgb_stype      = IMRGB_ITYPE_VERBATIM | IMRGB_BYTES_PER_PLANE;

	header.rgb_imagic     = 0x01da;

	header.rgb_dim        = 3;

	header.rgb_width      = ImVfbQWidth( vfb );

	header.rgb_height     = ImVfbQHeight( vfb );

	header.rgb_zsize      = 4;

	header.rgb_min        = 0;

	header.rgb_max        = 255;

	header.rgb_wastebytes = 3;

	header.rgb_colormap   = 0;

	header.rgb_rleend     = 0;

	header.rgb_rowstart   = 0;

	header.rgb_rowsize    = 0;



	tagEntry = TagTableQDirect( tagTable, "image name", 0 );

	if ( tagEntry == TAGENTRYNULL )

		strcpy( header.rgb_name, "untitled" );

	else

	{

		TagEntryQValue( tagEntry, &tmpString );

		strncpy( header.rgb_name, tmpString, 80 );

	}



	if ( ImBinWriteStruct( ioType, fd, fp, &header, imRgbHeaderFields )==-1)

		ImReturnBinError( );



	/*

	 * Output -verbose message

	 */

	ImInfo( "Image Name", header.rgb_name );

	ImInfo( "Byte Order", "Most Significant Byte First" );



	sprintf( message, "%d x %d", header.rgb_width, header.rgb_height );

	ImInfo( "Resolution", message );



	ImInfo( "Type", "24-bit RGB" );

	ImInfo( "Compression Type", "none (Verbatim)" );



	ImInfo( "Alpha Channel", "8-bit" );



      	/*   

	 *  Allocate space for a run buffer large enough for :

	 *  (width * height * 3)  unencoded scanlines. 

	 */

	x = header.rgb_width;

	y = header.rgb_height;



	ImMalloc( runBuffer, unsigned char *, 4 * y * x );



	rbp  = runBuffer;

	pptr = ImVfbQLast( vfb );

	for (i=1; i < x; i++)

		ImVfbSDec( vfb, pptr );





	/*

	 *  Red, Green, Blue, then Alpha channel.

	 */

	for (j = 1; (j <= y); j++)

	{

		for (i = 1; (i <= x); i++)

		{

		   *rbp = ImVfbQRed( vfb, pptr );

		   ImVfbSInc( vfb, pptr );

		   rbp++;

		}

		ImVfbSDec( vfb, pptr );

		pptr  = ImVfbQUp( vfb, pptr );

		for (i=1; i < x; i++)

			ImVfbSDec( vfb, pptr );

	}



	pptr  = ImVfbQLast( vfb );

	for(i=1; i < x; i++)  ImVfbSDec( vfb, pptr );

	for (j = 1; (j <= y); j++)

	{

		for (i = 1; (i <= x); i++)

		{

		   *rbp = ImVfbQGreen( vfb, pptr );

		   ImVfbSInc( vfb, pptr );

		   rbp++;

		}

		ImVfbSDec( vfb, pptr );

		pptr  = ImVfbQUp( vfb, pptr );

		for (i=1; i < x; i++)

			ImVfbSDec( vfb, pptr );

	}



	pptr  = ImVfbQLast( vfb );

	for(i=1; i < x; i++)  ImVfbSDec( vfb, pptr );

	for (j = 1; (j <= y); j++)

	{

		for (i = 1; (i <= x); i++)

		{

		   *rbp = ImVfbQBlue( vfb, pptr );

		   ImVfbSInc( vfb, pptr );

		   rbp++;

		}

		ImVfbSDec( vfb, pptr );

		pptr  = ImVfbQUp( vfb, pptr );

		for (i=1; i < x; i++)

			ImVfbSDec( vfb, pptr );

	}



	pptr  = ImVfbQLast( vfb );

	for(i=1; i < x; i++)  ImVfbSDec( vfb, pptr );

	for (j = 1; (j <= y); j++)

	{

		for (i = 1; (i <= x); i++)

		{

		   *rbp = ImVfbQAlpha( vfb, pptr );

		   ImVfbSInc( vfb, pptr );

		   rbp++;

		}

		ImVfbSDec( vfb, pptr );

		pptr  = ImVfbQUp( vfb, pptr );

		for (i=1; i < x; i++)

			ImVfbSDec( vfb, pptr );

	}



	/*

	 *  Write out the buffer.

	 */

	if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, x * y * 4 ) == -1)

	{

		free( (char *)runBuffer );

		ImReturnBinError( );

	}





	free( (char *)runBuffer );

	return ( 1 );

}







/*

 *  FUNCTION

 *	imRgbEncode	-  encode one channel of one scanline

 *

 *  DESCRIPTION

 *	One unencoded channel of one scanline (cIn) is encoded into

 * 	Iris RGB format and returned in cOut.  

 *	Width is the length of unencoded data.

 */



static int

#ifdef __STDC__

imRgbEncode( unsigned char *cOut, unsigned char *cIn, int width )

#else

imRgbEncode( cOut, cIn, width )

	unsigned char *cOut;

	unsigned char *cIn;

	int width;

#endif

{

	int cnt;

	int len;



	len = 0;



	/*

	 * Loop thru the unencoded bytes

	 */

	while ( width > 0 )

	{

		if (( width > 1 ) && ( cIn[0] == cIn[1] ))

		{

			/* Encode case */

			for( cnt=2; cnt < width; cnt++ )

			{

				if ( cIn[cnt] != cIn[cnt-1] )

					break;

				if ( cnt >= 127 )

					break;

			}



			/* Write out count */

			*cOut++ = cnt;

			len++;



			/* Write out value */

			*cOut++ = *cIn;

			len++;

			width -= cnt;

			cIn += cnt;

		}

		else	/* Don't encode */

		{

			for( cnt=1; cnt <  width; cnt++ )

			{

				if ((width-cnt > 1) && (cIn[cnt]==cIn[cnt+1]))

					break;

				if ( cnt >= 127 )

					break;

			}



			/* Write out count  (*cOut++ = 256-cnt)  */

			*cOut++ = 0x80 | cnt;

			len++;

		

			/* Copy string of pixels */

			for ( ; cnt-- > 0; len++, width-- )

				*cOut++ = *cIn++;

		}

	}



	/*

	 * Mark the end of a scanline with a zero count

	 */

	*cOut++ = 0;

	len++;

	return( len );

}











/*

 *  FUNCTION

 *	imRgbWriteRLE8	-  write an 8-bit Iris RGB file

 *

 *  DESCRIPTION

 *	The VFB is queried, and the RGB file header set up and written out.

 *	The VFB data is then read out and converted to run-codes, and those

 *	codes written out.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imRgbWriteRLE8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRgbWriteRLE8( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

        int          ioType;            /* I/O flags                    */

        int          fd;                /* Input file descriptor        */

        FILE        *fp;                /* Input file pointer           */

	TagTable    *flagsTable;	/* Flags			*/

        TagTable    *tagTable;          /* Tag list to read from        */

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	ImVfbPtr     leftpptr;		/* Points to start of scanlines */

	TagEntry    *tagEntry;		/* Tmp table entry		*/

	unsigned char *grayBuf;		/* Run buffer			*/

	unsigned char *buf;		/* Run buffer			*/

	char	    *tmpString;		/* For the image name		*/

	int x,y,z;			/* Convenient short names	*/

	int i,j;			/* Loop counters		*/

	int	     t;			/* Table index			*/

	int	    *offsetTable;	/* Array of scanline offsets	*/

	int	    *sizeTable;		/* Array of scanline sizes	*/

	int	     tablesize;		/* size of the scanline tables  */

        int          writeCount;        /* Keep count of file offset    */

	int	     status;		/* Status returned from seek	*/

	int	     len;		/* Number of bytes unencoded	*/

	imRgbHeaderInfo  header;	/* RGB file header		*/

	char		 message[100];	/* ImInfo message		*/







	/*

	 *  RGB files are usually generated on Irises, which have an MBF

	 *  byte order.  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINMBF );



	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );



	tagEntry = TagTableQDirect( tagTable, "image name", 0 );

	if ( tagEntry == TAGENTRYNULL )

		strcpy( header.rgb_name, "untitled" );

	else

	{

		TagEntryQValue( tagEntry, &tmpString );

		strncpy( header.rgb_name, tmpString, 80 );

	}



	/*

	 * Set header fields to reasonable values 

	 */

	header.rgb_stype      = IMRGB_ITYPE_RLE | IMRGB_BYTES_PER_PLANE;

	header.rgb_imagic     = 0x01da;

	header.rgb_dim        = 2;

	header.rgb_width      = ImVfbQWidth( vfb );

	header.rgb_height     = ImVfbQHeight( vfb );

	header.rgb_zsize      = 1;

	header.rgb_min        = 0;

	header.rgb_max        = 255;

	header.rgb_wastebytes = 3;

	header.rgb_colormap   = 0;



	/* Shorthand */

	x = header.rgb_width;

	y = header.rgb_height;

	z = header.rgb_zsize;



	pptr = leftpptr = ImVfbQPtr( vfb, 0, y-1 );



	tablesize = y * z * 4;  	/* 4 is sizeof int in file */

	header.rgb_rowstart = IMRGBHEADERSIZE;	

	header.rgb_rowsize = IMRGBHEADERSIZE + tablesize;

	header.rgb_rleend = IMRGBHEADERSIZE + (2 * tablesize);



	/*

	 * Write out the RGB image file header

	 */



	if ( ImBinWriteStruct( ioType, fd, fp, &header, imRgbHeaderFields)==-1)

		ImReturnBinError( );



	/*

	 * Output -verbose message

	 */

	ImInfo( "Image Name", header.rgb_name );

	ImInfo( "Byte Order", "Most Significant Byte First" );



	sprintf( message, "%d x %d", header.rgb_width, header.rgb_height );

	ImInfo( "Resolution", message );



	ImInfo( "Type", "8-bit Grayscale" );

	ImInfo( "Compression Type", "Run Length Encoded (RLE)" );

	ImInfo( "Alpha Channel", "none" );



	/*

	 * Allocate offset and size tables and write out as place holders

	 */

	ImMalloc( offsetTable, int *, y * z * sizeof(int) );

	ImMalloc( sizeTable, int *, y * z * sizeof(int) );



	memset( offsetTable, 0x00, y * z * sizeof(int) );

	memset( sizeTable, 0x00, y * z * sizeof(int) );



	if ( ImBinWrite( ioType, fd, fp, offsetTable, INT, 4, y*z ) == -1 )

	{

		free( (char *)offsetTable );

		ImReturnBinError( );

	}

	if ( ImBinWrite( ioType, fd, fp, sizeTable, INT, 4, y*z ) == -1 )

	{

		free( (char *)offsetTable );

		free( (char *)sizeTable );

		ImReturnBinError( );

	}



	/*

	 * Keep track of how many bytes written so far

	 */

	writeCount = IMRGBHEADERSIZE + (2 * tablesize);



	/*

	 * Allocate run buffers large enough for one scanline per color

	 */ 

	ImCalloc( grayBuf, unsigned char *, x, 1 );

	ImCalloc( buf,    unsigned char *, x*2, 1 );



	/*

	 * Loop through the scan lines.  Fill the buffers with channel values,

	 * encode them, and then write them out.

	 */

	t = 0;



	for( i=0; i<y; i++ )

	{

		/*

		 * Copy the vfb pixel values into scanline buffers.

		 */

		for ( j = 0; j < x; j++ )

		{

			grayBuf[j] = ImVfbQIndex8(   vfb, pptr );

			ImVfbSInc(   vfb, pptr );

		}



		/*

		 * Do one grayscale scanline

		 */



		sizeTable[t]  = len = imRgbEncode( buf, grayBuf, x );

		offsetTable[t] = writeCount;

		writeCount += len;



		if (ImBinWrite(ioType, fd, fp, buf, UCHAR, 1, len ) == -1)

		{

			free( (char*) grayBuf );

			free( (char*) buf );

			free( (char*) offsetTable );

			free( (char*) sizeTable );

			ImReturnBinError( );

		}

		

		/*

		 * Position leftpptr at the beginning of the scanline above

		 */

		ImVfbSUp( vfb, leftpptr );

		pptr = leftpptr;

		t++;

	}



	/*

	 * Seek backwards and write out the correct offsetTable and sizetable.

	 */

	if ( ioType & IMFILEIOFD )

		status = lseek( fd, IMRGBHEADERSIZE, L_SET );

	else

		status = fseek( fp, IMRGBHEADERSIZE, F_SET );



	if ( status == -1 )

	{

		free( (char*) grayBuf );

		free( (char*) buf );

		free( (char*) offsetTable );

		free( (char*) sizeTable );

		ImErrNo = IMESYS;

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	if ( ImBinWrite( ioType, fd, fp, offsetTable, INT, 4, y*z ) == -1 )

	{

		free( (char*) grayBuf );

		free( (char*) buf );

		free( (char*) offsetTable );

		free( (char*) sizeTable );

		ImReturnBinError( );

	}

	if ( ImBinWrite( ioType, fd, fp, sizeTable, INT, 4, y*z ) == -1 )

	{

		free( (char*) grayBuf );

		free( (char*) buf );

		free( (char*) offsetTable );

		free( (char*) sizeTable );

		ImReturnBinError( );

	}



	free( (char*) grayBuf );

	free( (char*) buf );

	free( (char*) offsetTable );

	free( (char*) sizeTable );



	return ( 1 );

}









/*

 *  FUNCTION

 *	imRgbWriteRLERGB -  write an Iris RGB file

 *

 *  DESCRIPTION

 *	The VFB is queried, and the RGB file header set up and written out.

 *	The VFB data is then read out and converted to run-codes, and those

 *	codes written out.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imRgbWriteRLERGB( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRgbWriteRLERGB( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

        int          ioType;            /* I/O flags                    */

        int          fd;                /* Input file descriptor        */

        FILE        *fp;                /* Input file pointer           */

	TagTable    *flagsTable;	/* Flags			*/

        TagTable    *tagTable;          /* Tag list to read from        */

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	ImVfbPtr     leftpptr;		/* Points to start of scanlines */

	TagEntry    *tagEntry;		/* Tmp table entry		*/

	unsigned char  *redBuf;		/* Run buffer			*/

	unsigned char  *grnBuf;		/* Run buffer			*/

	unsigned char  *bluBuf;		/* Run buffer			*/

	unsigned char  *buf;		/* Run buffer			*/

	char	    *tmpString;		/* For the image name		*/

	int x,y,z;			/* Convenient short names	*/

	int i,j;			/* Loop counters		*/

	int	     t;			/* Table index			*/

	int	    *offsetTable;	/* Array of scanline offsets	*/

	int	    *sizeTable;		/* Array of scanline sizes	*/

	int	     tablesize;		/* size of the scanline tables  */

        int          writeCount;        /* Keep count of file offset    */

	int	     status;		/* Status returned from seek	*/

	int	     len;		/* Number of bytes unencoded	*/

	imRgbHeaderInfo  header;	/* RGB file header		*/

	char		 message[100];	/* ImInfo message		*/







	/*

	 *  RGB files are usually generated on Irises, which have an MBF

	 *  byte order.  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINMBF );



	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );



	tagEntry = TagTableQDirect( tagTable, "image name", 0 );

	if ( tagEntry == TAGENTRYNULL )

		strcpy( header.rgb_name, "untitled" );

	else

	{

		TagEntryQValue( tagEntry, &tmpString );

		strncpy( header.rgb_name, tmpString, 80 );

	}



	/*

	 * Set header fields to reasonable values 

	 */

	header.rgb_stype      = IMRGB_ITYPE_RLE | IMRGB_BYTES_PER_PLANE;

	header.rgb_imagic     = 0x01da;

	header.rgb_dim        = 3;

	header.rgb_width      = ImVfbQWidth( vfb );

	header.rgb_height     = ImVfbQHeight( vfb );

	header.rgb_zsize      = 3;

	header.rgb_min        = 0;

	header.rgb_max        = 255;

	header.rgb_wastebytes = 3;

	header.rgb_colormap   = 0;



	/* Shorthand */

	x = header.rgb_width;

	y = header.rgb_height;

	z = header.rgb_zsize;



	pptr = leftpptr = ImVfbQPtr( vfb, 0, y-1 );



	tablesize = y * z * 4;  	/* 4 is sizeof int in file */

	header.rgb_rowstart = IMRGBHEADERSIZE;	

	header.rgb_rowsize = IMRGBHEADERSIZE + tablesize;

	header.rgb_rleend = IMRGBHEADERSIZE + (2 * tablesize);



	/*

	 * Write out the RGB image file header

	 */



	if ( ImBinWriteStruct( ioType, fd, fp, &header, imRgbHeaderFields)==-1)

		ImReturnBinError( );



	/*

	 * Output -verbose message

	 */

	ImInfo( "Image Name", header.rgb_name );

	ImInfo( "Byte Order", "Most Significant Byte First" );



	sprintf( message, "%d x %d", header.rgb_width, header.rgb_height );

	ImInfo( "Resolution", message );



	ImInfo( "Type", "24-bit RGB" );

	ImInfo( "Compression Type", "Run Length Encoded (RLE)" );

	ImInfo( "Alpha Channel", "none");



	/*

	 * Allocate offset and size tables and write out as place holders

	 */

	ImMalloc( offsetTable, int *, y * z * sizeof(int) );

	ImMalloc( sizeTable, int *, y * z * sizeof(int) );



	memset( offsetTable, 0x00, y * z * sizeof(int) );

	memset( sizeTable, 0x00, y * z * sizeof(int) );



	if ( ImBinWrite( ioType, fd, fp, offsetTable, INT, 4, y*z ) == -1 )

	{

		free( (char *)offsetTable );

		ImReturnBinError( );

	}

	if ( ImBinWrite( ioType, fd, fp, sizeTable, INT, 4, y*z ) == -1 )

	{

		free( (char *)offsetTable );

		free( (char *)sizeTable );

		ImReturnBinError( );

	}



	/*

	 * Keep track of how many bytes written so far

	 */

	writeCount = IMRGBHEADERSIZE + (2 * tablesize);



	/*

	 * Allocate run buffers large enough for one scanline per color

	 */ 

	ImCalloc( redBuf, unsigned char *, x, 1 );

	ImCalloc( grnBuf, unsigned char *, x, 1 );

	ImCalloc( bluBuf, unsigned char *, x, 1 );

	ImCalloc( buf,    unsigned char *, x*2, 1 );



	/*

	 * Loop through the scan lines.  Fill the buffers with channel values,

	 * encode them, and then write them out.

	 */

	t = 0;



	for( i=0; i<y; i++ )

	{

		/*

		 * Copy the vfb pixel values into scanline buffers.

		 */

		for ( j = 0; j < x; j++ )

		{

			redBuf[j] = ImVfbQRed(   vfb, pptr );

			grnBuf[j] = ImVfbQGreen( vfb, pptr );

			bluBuf[j] = ImVfbQBlue(  vfb, pptr );

			ImVfbSInc(   vfb, pptr );

		}



		/*

		 * Red channel

		 */



		sizeTable[t]  = len = imRgbEncode( buf, redBuf, x );

		offsetTable[t] = writeCount;

		writeCount += len;



		if (ImBinWrite(ioType, fd, fp, buf, UCHAR, 1, len ) == -1)

		{

			imRgbFrBuf(redBuf,grnBuf,bluBuf,buf,offsetTable,sizeTable);

			ImReturnBinError( );

		}

		

		/*

		 * Green channel

		 */



		sizeTable[t+y]  = len = imRgbEncode( buf, grnBuf, x );

		offsetTable[t+y] = writeCount;

		writeCount += len;



		if (ImBinWrite(ioType, fd, fp, buf, UCHAR, 1, len ) == -1)

		{

			imRgbFrBuf(redBuf,grnBuf,bluBuf,buf,offsetTable,sizeTable);

			ImReturnBinError( );

		}



		/*

		 * Blue channel

		 */



		sizeTable[t+y+y]  = len = imRgbEncode( buf, bluBuf, x );

		offsetTable[t+y+y] = writeCount;

		writeCount += len;



		if (ImBinWrite(ioType, fd, fp, buf, UCHAR, 1, len ) == -1)

		{

			imRgbFrBuf(redBuf,grnBuf,bluBuf,buf,offsetTable,sizeTable);

			ImReturnBinError( );

		}

		

		/*

		 * Position leftpptr at the beginning of the scanline above

		 */

		ImVfbSUp( vfb, leftpptr );

		pptr = leftpptr;

		t++;

	}



	/*

	 * Seek backwards and write out the correct offsetTable and sizetable.

	 */

	if ( ioType & IMFILEIOFD )

		status = lseek( fd, IMRGBHEADERSIZE, L_SET );

	else

		status = fseek( fp, IMRGBHEADERSIZE, F_SET );



	if ( status == -1 )

	{

		imRgbFrBuf(redBuf,grnBuf,bluBuf,buf,offsetTable,sizeTable);

		ImErrNo = IMESYS;

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	if ( ImBinWrite( ioType, fd, fp, offsetTable, INT, 4, y*z ) == -1 )

	{

		imRgbFrBuf(redBuf,grnBuf,bluBuf,buf,offsetTable,sizeTable);

		ImReturnBinError( );

	}

	if ( ImBinWrite( ioType, fd, fp, sizeTable, INT, 4, y*z ) == -1 )

	{

		imRgbFrBuf(redBuf,grnBuf,bluBuf,buf,offsetTable,sizeTable);

		ImReturnBinError( );

	}



	imRgbFrBuf(redBuf,grnBuf,bluBuf,buf,offsetTable,sizeTable);



	return ( 1 );

}



/*

 *  FUNCTION

 *	imRgbWriteRLERGBA -  write an Iris RGB file

 *

 *  DESCRIPTION

 *	The VFB is queried, and the RGB file header set up and written out.

 *	The VFB data is then read out and converted to run-codes, and those

 *	codes written out.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imRgbWriteRLERGBA( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRgbWriteRLERGBA( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

        int          ioType;            /* I/O flags                    */

        int          fd;                /* Input file descriptor        */

        FILE        *fp;                /* Input file pointer           */

	TagTable    *flagsTable;	/* Flags			*/

        TagTable    *tagTable;          /* Tag list to read from        */

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	ImVfbPtr     leftpptr;		/* Points to start of scanlines */

	TagEntry    *tagEntry;		/* Tmp table entry		*/

	unsigned char  *redBuf;		/* Run buffer			*/

	unsigned char  *grnBuf;		/* Run buffer			*/

	unsigned char  *bluBuf;		/* Run buffer			*/

	unsigned char  *alpBuf;		/* Run buffer			*/

	unsigned char  *buf;		/* Run buffer			*/

	char	    *tmpString;		/* For the image name		*/

	int x,y,z;			/* Convenient short names	*/

	int i,j;			/* Loop counters		*/

	int	     t;			/* Table index			*/

	int	    *offsetTable;	/* Array of scanline offsets	*/

	int	    *sizeTable;		/* Array of scanline sizes	*/

	int	     tablesize;		/* size of the scanline tables  */

        int          writeCount;        /* Keep count of file offset    */

	int	     status;		/* Status returned from seek	*/

	int	     len;		/* Number of bytes unencoded	*/

	imRgbHeaderInfo  header;	/* RGB file header		*/

	char		 message[100];	/* ImInfo message		*/





	/*

	 *  RGB files are usually generated on Irises, which have an MBF

	 *  byte order.  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINMBF );



	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );



	tagEntry = TagTableQDirect( tagTable, "image name", 0 );

	if ( tagEntry == TAGENTRYNULL )

		strcpy( header.rgb_name, "untitled" );

	else

	{

		TagEntryQValue( tagEntry, &tmpString );

		strncpy( header.rgb_name, tmpString, 80 );

	}



	/*

	 * Set header fields to reasonable values 

	 */

	header.rgb_stype      = IMRGB_ITYPE_RLE | IMRGB_BYTES_PER_PLANE;

	header.rgb_imagic     = 0x01da;

	header.rgb_dim        = 3;

	header.rgb_width      = ImVfbQWidth( vfb );

	header.rgb_height     = ImVfbQHeight( vfb );

	header.rgb_zsize      = 4;

	header.rgb_min        = 0;

	header.rgb_max        = 255;

	header.rgb_wastebytes = 3;

	header.rgb_colormap   = 0;



	/* Shorthand */

	x = header.rgb_width;

	y = header.rgb_height;

	z = header.rgb_zsize;



	pptr = leftpptr = ImVfbQPtr( vfb, 0, y-1 );



	tablesize = y * z * 4;  	/* 4 is sizeof int in file */

	header.rgb_rowstart = IMRGBHEADERSIZE;	

	header.rgb_rowsize = IMRGBHEADERSIZE + tablesize;

	header.rgb_rleend = IMRGBHEADERSIZE + (2 * tablesize);



	/*

	 * Write out the RGB image file header

	 */



	if ( ImBinWriteStruct( ioType, fd, fp, &header, imRgbHeaderFields)==-1)

		ImReturnBinError( );



	/*

	 * Output -verbose message

	 */

	ImInfo( "Image Name", header.rgb_name );

	ImInfo( "Byte Order", "Most Significant Byte First" );



	sprintf( message, "%d x %d", header.rgb_width, header.rgb_height );

	ImInfo( "Resolution", message );



	ImInfo( "Type", "24-bit RGB" );

	ImInfo( "Compression Type", "Run Length Encoded (RLE)" );



	ImInfo( "Alpha Channel", "8-bit");





	/*

	 * Allocate offset and size tables and write out as place holders

	 */

	ImMalloc( offsetTable, int *, y * z * sizeof(int) );

	ImMalloc( sizeTable, int *, y * z * sizeof(int) );



	memset( offsetTable, 0x00, y * z * sizeof(int) );

	memset( sizeTable, 0x00, y * z * sizeof(int) );



	if ( ImBinWrite( ioType, fd, fp, offsetTable, INT, 4, y*z ) == -1 )

	{

		free( (char *)offsetTable );

		ImReturnBinError( );

	}

	if ( ImBinWrite( ioType, fd, fp, sizeTable, INT, 4, y*z ) == -1 )

	{

		free( (char *)offsetTable );

		free( (char *)sizeTable );

		ImReturnBinError( );

	}



	/*

	 * Keep track of how many bytes written so far

	 */

	writeCount = IMRGBHEADERSIZE + (2 * tablesize);



	/*

	 * Allocate run buffers large enough for one scanline per color

	 */ 

	ImCalloc( redBuf, unsigned char *, x, 1 );

	ImCalloc( grnBuf, unsigned char *, x, 1 );

	ImCalloc( bluBuf, unsigned char *, x, 1 );

	ImCalloc( alpBuf, unsigned char *, x, 1 );

	ImCalloc( buf,    unsigned char *, x*2, 1 );



	/*

	 * Loop through the scan lines.  Fill the buffers with channel values,

	 * encode them, and then write them out.

	 */

	t = 0;



	for( i=0; i<y; i++ )

	{

		/*

		 * Copy the vfb pixel values into scanline buffers.

		 */

		for ( j = 0; j < x; j++ )

		{

			redBuf[j] = ImVfbQRed(   vfb, pptr );

			grnBuf[j] = ImVfbQGreen( vfb, pptr );

			bluBuf[j] = ImVfbQBlue(  vfb, pptr );

			alpBuf[j] = ImVfbQAlpha(  vfb, pptr );

			ImVfbSInc(   vfb, pptr );

		}



		/*

		 * Red channel

		 */



		sizeTable[t]  = len = imRgbEncode( buf, redBuf, x );

		offsetTable[t] = writeCount;

		writeCount += len;



		if (ImBinWrite(ioType, fd, fp, buf, UCHAR, 1, len ) == -1)

		{

			imRgbFrBufAlp(redBuf,grnBuf,bluBuf,alpBuf,buf,offsetTable,sizeTable);

			ImReturnBinError( );

		}

		

		/*

		 * Green channel

		 */



		sizeTable[t+y]  = len = imRgbEncode( buf, grnBuf, x );

		offsetTable[t+y] = writeCount;

		writeCount += len;



		if (ImBinWrite(ioType, fd, fp, buf, UCHAR, 1, len ) == -1)

		{

			imRgbFrBufAlp(redBuf,grnBuf,bluBuf,alpBuf,buf,offsetTable,sizeTable);

			ImReturnBinError( );

		}



		/*

		 * Blue channel

		 */



		sizeTable[t+y+y]  = len = imRgbEncode( buf, bluBuf, x );

		offsetTable[t+y+y] = writeCount;

		writeCount += len;



		if (ImBinWrite(ioType, fd, fp, buf, UCHAR, 1, len ) == -1)

		{

			imRgbFrBufAlp(redBuf,grnBuf,bluBuf,alpBuf,buf,offsetTable,sizeTable);

			ImReturnBinError( );

		}



		/*

		 * Alpha channel

		 */



		sizeTable[t+y+y+y]  = len = imRgbEncode( buf, alpBuf, x );

		offsetTable[t+y+y+y] = writeCount;

		writeCount += len;



		if (ImBinWrite(ioType, fd, fp, buf, UCHAR, 1, len ) == -1)

		{

			imRgbFrBufAlp(redBuf,grnBuf,bluBuf,alpBuf,buf,offsetTable,sizeTable);

			ImReturnBinError( );

		}

	

		

		/*

		 * Position leftpptr at the beginning of the scanline above

		 */

		ImVfbSUp( vfb, leftpptr );

		pptr = leftpptr;

		t++;

	}



	/*

	 * Seek backwards and write out the correct offsetTable and sizetable.

	 */

	if ( ioType & IMFILEIOFD )

		status = lseek( fd, IMRGBHEADERSIZE, L_SET );

	else

		status = fseek( fp, IMRGBHEADERSIZE, F_SET );



	if ( status == -1 )

	{

		imRgbFrBufAlp(redBuf,grnBuf,bluBuf,alpBuf,buf,offsetTable,sizeTable);

		ImErrNo = IMESYS;

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	if ( ImBinWrite( ioType, fd, fp, offsetTable, INT, 4, y*z ) == -1 )

	{

		imRgbFrBufAlp(redBuf,grnBuf,bluBuf,alpBuf,buf,offsetTable,sizeTable);

		ImReturnBinError( );

	}

	if ( ImBinWrite( ioType, fd, fp, sizeTable, INT, 4, y*z ) == -1 )

	{

		imRgbFrBufAlp(redBuf,grnBuf,bluBuf,alpBuf,buf,offsetTable,sizeTable);

		ImReturnBinError( );

	}



	imRgbFrBufAlp(redBuf,grnBuf,bluBuf,alpBuf,buf,offsetTable,sizeTable);



	return ( 1 );

}

