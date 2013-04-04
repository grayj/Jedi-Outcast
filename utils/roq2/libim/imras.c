/**	$Header: /roq/libim/imras.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imras.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imras.c		-  Sun Rasterfile I/O

 **

 **  PROJECT

 **	libim		-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imras.c contains routines to read and write Sun Rasterfiles for

 **	the image manipulation library.  Raster data read in is stored

 **	in a VFB and optional CLT in a tag list.  Raster data written

 **	out is taken from a tag list.

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

 **

 **	imRasRead		f  read a Sun Rasterfile

 **	imRasWrite1		f  write 1-bit standard RAS format

 **	imRasWrite8		f  write 8-bit standard RAS format

 **	imRasWriteRGB		f  write 24-bit RGB standard RAS format

 **	imRasWriteRGBA		f  write 32-bit RGBA standard RAS format

 **	imRasWriteRLE1		f  write 1-bit encoded RAS format

 **	imRasWriteRLE8		f  write 8-bit encoded RAS format

 **	imRasWriteRLERGB	f  write 24-bit RGB encoded RAS format

 **	imRasWriteRLERGBA	f  write 32-bit RGBA encoded RAS format

 **

 **	imRasHeaderInfo		t  Rasterfile header information

 **	imRasHeaderFields	v  imRasHeaderInfo description for Bin pkg

 **	imRasHeader		v  Rasterfile header holder

 **

 **	IMRASMAGIC		d  file magic number

 **	IMRASESC		d  escape code for run-length encoding

 **	IMRT*			d  raster encoding types

 **	IMRMT*			d  raster colormap encoding types

 **

 **	IMRAS_RED,IMRAS_GREEN,

 **	IMRAS_BLUE,IMRAS_ALPHA	d  RGBA color cycle counter values

 **

 **	imRasRead1		f  read 1-bit RAS format

 **	imRasRead8		f  read 8-bit standard RAS format

 **	imRasRead24RGB		f  read 24-bit RGB standard RAS format

 **	imRasRead32RGBA		f  read 32-bit RGBA standard RAS format

 **	imRasReadRLE1		f  read 1-bit encoded RAS format

 **	imRasReadRLE8		f  read 8-bit encoded RAS format

 **	imRasReadRLE24RGB	f  read 24-bit RGB encoded RAS format

 **	imRasReadRLE32RGBA	f  read 32-bit RGBA encoded RAS format

 **

 **	imRasWriteHeaderClt	f  write header and CLT

 **	imRasCltToBuffer	f  convert CLT to file format buffer

 **

 **	IMAddRun		m  add another pixel to RLE buffer

 **	IMDumpRun		m  dump an RLE run into the run buffer

 **

 **  HISTORY

 **	$Log: /roq/libim/imras.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.22  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.21  1995/04/03  21:35:15  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.20  1995/01/10  23:41:09  bduggan

 **	put in IMMULTI, IMPIPE instead of TRUE/FALSE

 **	made read/write routines static

 **

 **	Revision 1.19  94/10/03  11:30:45  nadeau

 **	Fixed bug in RGB+Alpha handling that did incorrect scanling padding,

 **	causing corrupted images for some image widths.

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

 **	Revision 1.18  92/12/03  01:51:39  nadeau

 **	Corrected info messages.

 **	

 **	Revision 1.17  92/11/23  18:43:05  nadeau

 **	Removed use of IMINFOMSG.

 **	

 **	Revision 1.16  92/11/04  12:06:10  groening

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.15  92/09/29  17:57:38  vle

 **	Added ImInfo messages.

 **	

 **	Revision 1.14  92/08/31  17:33:39  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.13  92/04/09  09:35:21  groening

 **	To make the compiler happy added extern statements.

 **	

 **	Revision 1.12  91/10/03  09:16:52  nadeau

 **	Reversed order from R-G-B to B-G-R on 24-bit image files.

 **	Added 32-bit image file read and write support.

 **	

 **	Revision 1.11  91/03/13  17:37:10  nadeau

 **	Fixed RGB ordering bug for RGB uncompressed read.

 **	

 **	Revision 1.10  91/02/12  10:44:47  nadeau

 **	Changed read and write routine names.  Removed the top level

 **	write call and updated the VFB-specific write calls to be

 **	entry points from the generic ImFileWrite code.  Encapsulated

 **	the run-length encoding into a few macros to reduce the

 **	apparent complexity of the RLE write routines.  Removed the

 **	tag table error checking now handled by ImFileWrite.  Removed

 **	temp file creation, now handled by ImFileWrite.

 **	

 **	Revision 1.9  91/01/30  18:10:04  nadeau

 **	Added monochrome support for read and write.

 **	

 **	Revision 1.8  90/12/03  12:51:14  nadeau

 **	Added checks to skip the extra pad byte on odd-length scanlines

 **	when reading RLE images.

 **	

 **	Revision 1.7  90/07/25  16:20:28  nadeau

 **	Documented the RAS format more fully.  Changed image write to use

 **	a scratch file when writing to pipes.  Fixed a byte padding problem

 **	on encoded images (and added description of problem to RAS comments).

 **	Removed stubs for 1-bit deep image read/write.

 **	

 **	Revision 1.6  90/07/23  13:49:04  nadeau

 **	Reversed order of channels on reading RGB images from BGR to RGB.

 **	

 **	Revision 1.5  90/07/02  13:21:43  nadeau

 **	Updated to the new error handling mechanism.

 **	

 **	Revision 1.4  90/06/25  14:39:01  nadeau

 **	Changed ImTag* to Tag* (new names).

 **	

 **	Revision 1.3  90/05/16  07:47:46  todd

 **	Add #include "ininternal.h" to top of file

 **	

 **	Revision 1.2  90/05/11  09:56:11  nadeau

 **	Totally restructured everything.  Broke format variants into separate

 **	routines.  For write, broke pipe vs file I/O into separate routines.

 **	Updated call interface and code to handle I/O on file descriptors nad

 **	file pointers.  Finished 24-bit code.

 **	

 **	Revision 1.1  90/03/06  17:29:09  nadeau

 **	Initial revision

 **	

 **/



#include "iminternal.h"



/**

 **  FORMAT

 **	RAS	-  Sun Rasterfile

 **

 **  AKA

 ** 	sun, sr, scr

 **

 **  FORMAT REFERENCES

 **	Graphics File Formats, David C. Kay, John R. Levine

 **

 **  CODE CREDITS

 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1990.

 **

 **  DESCRIPTION

 **	Sun rasterfiles start with a fixed size header:

 **

 **	magic		The file's magic number.

 **

 **	width		The width of the image, in pixels.

 **

 **	height		The height of the image, in pixels.

 **

 **	depth		The depth of the image, in bits.

 **

 **	length		The total image storage size, in bytes.

 **

 **	type		The type of image encoding used.  One of:

 **

 **			Standard	Unencoded (see below).

 **			ByteEncoded	Run-length encoded (see below).

 **

 **	mapType		The type of CLT (if any).  One of:

 **

 **			None		No CLT included.

 **			EqualRGB	All red, then green, then blue

 **			Raw		Unkown!!!

 **

 **	mapLength	The length of the CLT (if any), in bytes.

 **

 **	Following the header is the CLT, stored as a string of red values,

 **	then green, then blue, for consecutive entries in the CLT.

 **

 **  STANDARD FORMAT (unencoded)

 **	For standard (unencoded) images, consecutive bytes in the file

 **	correspond to consecutive pixels (or components of the pixel for

 **	RGB images).

 **

 **  BYTE-ENCODING FORMAT (run-length encoded)

 **	The Sun rasterfile encodes the image byte stream by counting up

 **	adjacent identical bytes and flagging a count-index pair with an

 **	"escape" byte:

 **		<escape> <count> <index>

 **

 **	The count is always biased by -1 (ie, count=1 means do a 2-byte run,

 **	count=2 means do a 3-byte run, and so on).  The maximum run length

 **	is therefore 256.  Runs always end on scanline boundaries.

 **

 **	To include the "escape" byte in the image, give a count of 0,

 **	followed by no index:

 **		<escape> <0>

 **

 **	To avoid expanding the storage size of images without any runs (like

 **	the ubiquitous Mandrill), just give the index by itself:

 **		<index>

 **

 **	The worst case scenario is an image filled with "escape" bytes.

 **	Each "escape" byte will be expanded into an <escape> <0> pair,

 **	thus doubling the size of the image file.

 **

 **	The "escape" byte is an 0x80 (0200) (128).

 **

 **  EXAMPLE #1

 **	Unencoded 8-bit Index Stream:

 **		0xAA 0xBB 0xBB 0xCC 0x80 0xDD 0xDD 0xDD

 **

 **	Encoded Stream:

 **		0xAA 0x80 0x01 0xBB 0xCC 0x80 0x00 0x80 0x02 0xDD

 **

 **	There are two runs (2 * 0xBB, and 3 * 0xDD) and one occurrence of

 **	the escape byte.  The encoded stream turns out to be 2 bytes larger

 **	than the unencoded one.

 **

 **

 **  EXAMPLE #2

 **	Unencoded RGB Stream:

 **		0x00 0x00 0x00  0x45 0x52 0x12  0x12 0x12 0x43

 **	     (blue) (green) (red) ...

 **

 **	Encoded Stream:

 **		0x80 0x02 0x00 0x45 0x52 0x80 0x02 0x12 0x43

 **

 **	There are two runs (3 * 0x0, and 3 * 0x12).  The encoded stream comes

 **	out the same size as the unencoded one.  Note that the 0x12 of the

 **	second RGB triplet, and the 0x12's of the third RGB triplet form a

 **	run of three 0x12's.

 **

 **  ODDITIES

 **	Scanlines with an odd number of pixels must be padded with one dummy

 **	pixel.  Note that the addition of a pad byte is based upon the scanline

 **	length...

 **

 **		For 8-bit and 24-bit "standard" images, the pad byte makes

 **		each scanline's stream of bytes end on a 16-bit boundary.

 **		This is clearly the original intent of the padding.

 **

 **		For 8-bit and 24-bit "encoded" images, the length of the

 **		scanline's encoded stream could be odd or even, depending upon

 **		the way runs are created from the data, not dependent upon the

 **		evenness or oddness of the original scanline length.

 **		Nevertheless, if the original scanline had an odd number of

 **		pixels in it, then a pad byte must be added, even if this

 **		makes the encoded stream have an odd number of bytes.

 **

 **		For 32-bit images (with an alpha channel), there is no padding.

 **/



#ifdef __STDC__

static int imRasRead1(int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb );

static int imRasRead8( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb);

static int imRasRead24RGB( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb);

static int imRasRead32RGBA( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb);

static int imRasReadRLE1( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb);

static int imRasReadRLE8( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb);

static int imRasReadRLE24RGB( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb);

static int imRasReadRLE32RGBA( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb);

static int imRasCltToBuffer( ImVfb *vfb, unsigned char **cltBuffer, int *nClt );

#else

static int imRasRead1( );

static int imRasRead8( );

static int imRasRead24RGB( );

static int imRasRead32RGBA( );

static int imRasReadRLE1( );

static int imRasReadRLE8( );

static int imRasReadRLE24RGB( );

static int imRasReadRLE32RGBA( );

static int imRasCltToBuffer( );

#endif



/*

 *  RAS - Sun Rasterfile

 *      For information on these structures, how to use them, etc. please

 *      see imfmt.c.

 */

#ifdef __STDC__

static int imRasRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imRasWrite1( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 

	TagTable *tagTable );

static int imRasWrite8(ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 

	TagTable *tagTable  );

static int imRasWriteRGB( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 

	TagTable *tagTable );  

static int imRasWriteRGBA( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 

	TagTable *tagTable );

static int imRasWriteRLE1( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 

	TagTable *tagTable );

static int imRasWriteRLE8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 

	TagTable *tagTable );

static int imRasWriteRLERGB( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 

	TagTable *tagTable );

static int imRasWriteRLERGBA( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 

	TagTable *tagTable );

#else

static int imRasRead( );

static int imRasWrite1( ),      imRasWrite8( );

static int imRasWriteRGB( ),    imRasWriteRGBA( );

static int imRasWriteRLE1( ),   imRasWriteRLE8( );

static int imRasWriteRLERGB( ), imRasWriteRLERGBA( );

#endif

static char *imRasNames[ ]  = { "ras", "sun", "sr", "scr", NULL };

static unsigned char imRasMagicNumber[ ] = { 0x59, 0xA6, 0x6A, 0x95 };



static ImFileFormatReadMap imRasReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IN,1,1,       0,      IMVFBMONO,      0 },

        { IN,1,1,       C,      IMVFBMONO,      C },

        { IN,1,8,       0,      IMVFBINDEX8,    0 },

        { IN,1,8,       C,      IMVFBINDEX8,    C },

        { RGB,3,8,      0,      IMVFBRGB,       0 },

        { RGB,3,8,      C,      IMVFBRGB,       C },

        { RGB,3,8,      A,      IMVFBRGB,       A },

        { RGB,3,8,      A | C,  IMVFBRGB,       A | C },



        { IN,1,1,       RLE,    IMVFBMONO,      0 },

        { IN,1,1,       RLE | C,IMVFBMONO,      C },

        { IN,1,8,       RLE,    IMVFBINDEX8,    0 },

        { IN,1,8,       RLE | C,IMVFBINDEX8,    C },

        { RGB,3,8,      RLE,    IMVFBRGB,       0 },

        { RGB,3,8,      RLE | C,IMVFBRGB,       C },

        { RGB,3,8,      RLE|A,  IMVFBRGB,       A },

        { RGB,3,8,      RLE|A|C,IMVFBRGB,       A | C },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imRasWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBMONO,    0,      IN,1,1,         RLE,    imRasWriteRLE1 },

        { IMVFBMONO,    C,      IN,1,1,         RLE | C,imRasWriteRLE1 },

        { IMVFBMONO,    0,      IN,1,1,         0,      imRasWrite1 },

        { IMVFBMONO,    C,      IN,1,1,         C,      imRasWrite1 },



        { IMVFBINDEX8,  0,      IN,1,8,         RLE,    imRasWriteRLE8 },

        { IMVFBINDEX8,  C,      IN,1,8,         RLE | C,imRasWriteRLE8 },

        { IMVFBINDEX8,  0,      IN,1,8,         0,      imRasWrite8 },

        { IMVFBINDEX8,  C,      IN,1,8,         C,      imRasWrite8 },



        { IMVFBRGB,     0,      RGB,3,8,        RLE,    imRasWriteRLERGB},

        { IMVFBRGB,     C,      RGB,3,8,        RLE|C,  imRasWriteRLERGB},

        { IMVFBRGB,     0,      RGB,3,8,        0,      imRasWriteRGB},

        { IMVFBRGB,     C,      RGB,3,8,        C,      imRasWriteRGB},



        { IMVFBRGB,     A,      RGB,3,8,        RLE|A,  imRasWriteRLERGBA},

        { IMVFBRGB,     A | C,  RGB,3,8,        RLE|A|C,imRasWriteRLERGBA},

        { IMVFBRGB,     A,      RGB,3,8,        A,      imRasWriteRGBA},

        { IMVFBRGB,     A | C,  RGB,3,8,        A|C,    imRasWriteRGBA},

        { -1,           0,      -1,             0,      NULL },

};





static ImFileMagic imFileRasMagic []=

{

	{ 0, 4, imRasMagicNumber},

	{ 0, 0, NULL },

};



ImFileFormat ImFileRasFormat =

{

	imRasNames, "Sun Rasterfile",

	"Sun Microsystems, Inc.",

	"1- and 8-bit color index image files, with or without CLT's.\n\
24-bit RGB and 32-bit RGB+alpha image files, with or without CLT's.\n\
Standard (uncompressed) and byte-encoded (RLE compressed).",
	"1- and 8-bit color index image files, with or without CLT's.\n\
24-bit RGB and 32-bit RGB+alpha image files, with or without CLT's.\n\
Standard (uncompressed) and byte-encoded (RLE compressed).",
	imFileRasMagic,

	IMNOMULTI, IMNOPIPE,

	IMNOMULTI, IMNOPIPE,

	imRasRead, imRasReadMap, imRasWriteMap

};











/*

 *  TYPEDEF & STRUCTURE

 *	imRasHeaderInfo		-  Rasterfile header information

 *	imRasHeaderFields	-  imRasHeaderInfo description for Bin pkg

 *	imRasHeader		-  Rasterfile header holder

 *

 *  DESCRIPTION

 *	A rasterfile's header contains a magic number, the image's width,

 *	height, and depth, the length of the file, the type of run length

 *	encoding in use, and the CLT, if any.

 */



typedef struct imRasHeaderInfo

{

	unsigned int ras_magic;			/* Magic number			*/

	unsigned int ras_width;			/* Image width			*/

	unsigned int ras_height;		/* Image height			*/

	unsigned int ras_depth;			/* Image depth			*/

	unsigned int ras_length;		/* Image length (in bytes)	*/

	unsigned int ras_type;			/* Type of encoding		*/

	unsigned int ras_maptype;		/* Type of CLT (color map)	*/

	unsigned int ras_maplength;		/* Length of CLT		*/

} imRasHeaderInfo;



static BinField imRasHeaderFields[ ] =

{

	{ UINT, 4, 1 },			/* ras_magic			*/

	{ UINT, 4, 1 },			/* ras_width			*/

	{ UINT, 4, 1 },			/* ras_height			*/

	{ UINT, 4, 1 },			/* ras_depth			*/

	{ UINT, 4, 1 },			/* ras_length			*/

	{ UINT, 4, 1 },			/* ras_type			*/

	{ UINT, 4, 1 },			/* ras_maptype			*/

	{ UINT, 4, 1 },			/* ras_maplength		*/

	{ 0, 0, 0 },

};



static imRasHeaderInfo	 imRasHeader;	/* RAS file header		*/











/*

 *  CONSTANTS

 *	IMRASMAGIC	-  file magic number

 *	IMRASESC	-  escape code for run-length encoding

 *	IMRT*		-  raster encoding types

 *	IMRMT*		-  raster colormap encoding types

 *

 *  DESCRIPTION

 *	IMRASMAGIC (Sun's RAS_MAGIC) is the 32-bit magic number at the

 *	top of all rasterfiles.

 *

 *	IMRASESC indicates the beginning of a run in a run length

 *	encoded raster file.

 *

 *	IMRT* (Sun's RT_*) select the type of run-length encoding

 *	used to encode the image data in the rasterfile.

 *

 *	IMRMT* (Sun's RMT_*) select the type of colormap (CLT)

 *	encoding used to encode the CLT data in the rasterfile, if any.

 */

#define IMRASMAGIC	0x59A66A95

#define IMRASESC	0x80



#define IMRTOLD		0		/* Raw pixrect image in 68K order*/

#define IMRTSTANDARD	1		/* Raw pixrect image in 68K order*/

#define IMRTBYTEENCODED	2		/* Run-length encoded		*/

#define IMRTEXPERIMENTAL 0xFFFF		/* Reserved by Sun for testing	*/



#define IMRMTNONE	0		/* No CLT.  ras_maplength == 0	*/

#define IMRMTEQUALRGB	1		/* All red, then green, then blue*/

#define IMRMTRAW	2		/* Raw CLT (?)			*/











/*

 *  CONSTANTS

 *	IMRAS_RED, IMRAS_GREEN, IMRAS_BLUE, IMRAS_ALPHA -  RGBA color cycle counter values

 *

 *  DESCRIPTION

 *	IMRAS_RED, IMRAS_GREEN, IMRAS_BLUE, and IMRAS_ALPHA are used as cycle values for a counter

 *	when encoding and decoding run-length stuff.

 */



#define IMRAS_BLUE		0

#define IMRAS_GREEN		1

#define IMRAS_RED		2

#define IMRAS_ALPHA		3









#ifndef L_SET

#define L_SET	0		/* Absolute offset			*/

#define L_INCR	1		/* Relative to current offset		*/

#define L_XTND	2		/* Relative to end of file		*/

#endif



#ifndef F_SET

#define F_SET	0		/* Absolute offset			*/

#define F_INCR	1		/* Relative to current offset		*/

#define F_XTND	2		/* Relative to end of file		*/

#endif











/*

 *  FUNCTION

 *	imRasRead	-  read a Sun Rasterfile

 *

 *  DESCRIPTION

 *	The file header is read and the magic number checked.  If there is

 *	a CLT in the file, it is read in and converted into an ImClt.

 *	Separate routines are then called to handle different depth and

 *	storage format variations of the image data.

 *

 *	Read supports no format flags.

 */



static int				/* Returns status		*/

#ifdef __STDC__

imRasRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRasRead( ioType, fd, fp, flagsTable, tagTable )

	int              ioType;	/* I/O flags			*/

	int	         fd;		/* Input file descriptor	*/

	FILE	        *fp;		/* Input file pointer		*/

	TagTable        *flagsTable;	/* Flags			*/

	TagTable        *tagTable;	/* Tag table to add to		*/

#endif

{

	ImVfb		 *vfb;		/* Read in image		*/

	int		 nClt;		/* Number of CLT entries	*/

	ImClt		 *clt;		/* Read in colormap		*/

	ImCltPtr	 cptr;		/* Color pointer		*/

	unsigned char		*cltBuffer;	/* Color lookup table buffer	*/

	unsigned char           *redp;		/* Red CLT range pointer	*/

	unsigned char           *greenp;	/* Green CLT range pointer	*/

	unsigned char           *bluep;		/* Blue CLT range pointer	*/

	int		 i;		/* Counter			*/

	int		 status;	/* Return status holder		*/

	char		 message[100];	/* ImInfo message		*/





	/*

	 *  Set the Binary I/O package's byte order, read in the file's

	 *  header, and check the magic number.

	 */

	BinByteOrder( BINMBF );

	if ( ImBinReadStruct( ioType, fd, fp, &imRasHeader,

		imRasHeaderFields )== -1)

	{

		ImReturnBinError( );

	}

	if ( imRasHeader.ras_magic != IMRASMAGIC )

	{

		ImErrNo = IMEMAGIC;

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	ImInfo( "Byte Order", "Most Significant Byte First" );



	sprintf( message, "%d x %d", imRasHeader.ras_width,

		 imRasHeader.ras_height );

	ImInfo( "Resolution", message );



	if( imRasHeader.ras_depth >= 24 )

		sprintf( message, "%d-bit RGB", imRasHeader.ras_depth );

	else

		sprintf( message, "%d-bit Color Indexed", imRasHeader.ras_depth );

	ImInfo( "Type", message );



	/*

	 *  Read in the CLT, if any.

	 */

	switch ( imRasHeader.ras_maptype )

	{

	case IMRMTNONE:		/* No CLT.				*/

		if ( imRasHeader.ras_maplength != 0 )

		{

			/* No CLT, yet the length says there is one!?	*/

			ImErrNo = IMECLTLENGTH;

			ImErrorFatal( ImQError( ), -1, ImErrNo );

		}

		clt = IMCLTNULL;



		ImInfo( "Color Table", "none" );

		break;



	case IMRMTEQUALRGB:	/* RGB CLT.				*/

		if ( imRasHeader.ras_maplength == 0 )

		{

			/* Is a CLT, yet the length says not!?		*/

			ImErrNo = IMECLTLENGTH;

			ImErrorFatal( ImQError( ), -1, ImErrNo );

		}

		nClt = imRasHeader.ras_maplength / 3;

		if ( (clt = ImCltAlloc( nClt )) ==IMCLTNULL)

		{

			ImErrorFatal( ImQError( ), -1, ImErrNo );

		}

		ImMalloc( cltBuffer, unsigned char *, imRasHeader.ras_maplength );



		/* Read in Red, Green, and Blue.			*/

		if ( ImBinRead( ioType, fd, fp, cltBuffer, UCHAR, 1,

			imRasHeader.ras_maplength ) == -1 )

		{

			free( (char *)cltBuffer );

			ImReturnBinError( );

		}

		cptr   = ImCltQFirst( clt );

		redp   = cltBuffer;

		greenp = cltBuffer + nClt;

		bluep  = cltBuffer + nClt + nClt;

		for ( i = 0; i < nClt; i++ )

		{

			ImCltSRed(   cptr, *redp++ );

			ImCltSGreen( cptr, *greenp++ );

			ImCltSBlue(  cptr, *bluep++ );

			ImCltSInc(   clt,   cptr );

		}



		free( (char *)cltBuffer );



		sprintf( message, "%d Entries", nClt );

		ImInfo( "Color Table", message );

		break;



	case IMRMTRAW:		/* Raw CLT ?				*/



		/* Fall through to error.				*/



	default:

		ImErrNo = IMESYNTAX;

		ImErrorFatal( "Unkown CLT type in file header.", -1, ImErrNo );

	}





	/*

	 *  Based on the image depth and encoding scheme, call a function

	 *  to do the rest of the work.  These functions are never called

	 *  again anywhere else.  It just breaks up this routine into

	 *  smaller bite-sized chunks for easier debugging.

	 */

	switch ( imRasHeader.ras_type )

	{

	case IMRTOLD:

		/*

		 *  Old format rasterfiles have a 0 in the ras_length field.

		 *  Fortunately, old format rasterfiles don't do run-length

		 *  encoding, so the size of the image data is just the height

		 *  times the width times the depth, in bytes.

		 */

		imRasHeader.ras_length = imRasHeader.ras_width *

			imRasHeader.ras_height * imRasHeader.ras_depth;



		/*  Fall through to standard.				*/



	case IMRTSTANDARD:

		/*  Un-encoded raster image.				*/



		ImInfo( "Compression Type", "none (Standard)" );



		switch ( imRasHeader.ras_depth )

		{

		case 1:	/* Monochrome image.				*/

			status = imRasRead1( ioType, fd, fp, flagsTable,

				tagTable, &vfb );

			ImInfo( "Alpha Channel", "none" );

			break;

		case 8:	/* CLT image.					*/

			status = imRasRead8( ioType, fd, fp, flagsTable,

				tagTable, &vfb );

			ImInfo( "Alpha Channel", "none" );

			break;

		case 24:/* RGB image.					*/

			status = imRasRead24RGB( ioType, fd, fp, flagsTable,

				tagTable, &vfb );

			ImInfo( "Alpha Channel", "none" );

			break;

		case 32:/* RGBA image.					*/

			status = imRasRead32RGBA( ioType, fd, fp, flagsTable,

				tagTable, &vfb );

			ImInfo( "Alpha Channel", "8-bit" );

			break;

		default:

			ImErrNo = IMEDEPTH;

			ImErrorFatal( "Unknown standard-format image depth", -1, ImErrNo );

		}

		break;



	case IMRTBYTEENCODED:

		/*  Run-length encoded raster image.			*/



		ImInfo("Compression Type","Run Length Encoded (Byte Encoded)");



		switch ( imRasHeader.ras_depth )

		{

		case 1:	/* Monochrome image.				*/

			status = imRasReadRLE1( ioType, fd, fp, flagsTable,

				tagTable, &vfb );

			ImInfo( "Alpha Channel", "none" );

			break;

		case 8:	/* CLT image.					*/

			status = imRasReadRLE8( ioType, fd, fp, flagsTable,

				tagTable, &vfb );

			ImInfo( "Alpha Channel", "none" );

			break;

		case 24:/* RGB image.					*/

			status = imRasReadRLE24RGB( ioType, fd, fp, flagsTable,

				tagTable, &vfb );

			ImInfo( "Alpha Channel", "none" );

			break;

		case 32:/* RGBA image.					*/

			status = imRasReadRLE32RGBA( ioType, fd, fp, flagsTable,

				tagTable, &vfb );

			ImInfo( "Alpha Channel", "8-bit" );

			break;

		default:

			ImErrNo = IMEDEPTH;

			ImErrorFatal( "Unkown byte-encoded-format image depth", -1, ImErrNo );

		}

		break;



	case IMRTEXPERIMENTAL:

		/*  Sun private experimental format.  Unsupported by us.*/



		/* Fall through to error.				*/



	default:

		ImErrNo = IMEIMAGETYPE;

		ImErrorFatal( "Unknown image type in file header", -1, ImErrNo );

	}





	if ( status == -1 )

		return ( -1 );



	if ( clt != IMCLTNULL )

	{

		ImVfbSClt( vfb, clt );

		TagTableAppend( tagTable,

			TagEntryAlloc( "image clt", POINTER, &clt ));

	}

	TagTableAppend( tagTable, TagEntryAlloc( "image vfb", POINTER, &vfb ));



	return ( 0 );

}











/*

 *  FUNCTION

 *	imRasRead1	-  read 1-bit RAS format

 *	imRasRead8	-  read 8-bit RAS format

 *	imRasRead24RGB	-  read 24-bit RGB RAS format

 *	imRasRead32RGBA	-  read 32-bit RGBA RAS format

 *

 *  DESCRIPTION

 *	Each of these routines deal with "standard" format RAS files.

 *	The input stream is a file or a pipe.  We don't care.

 *

 *	A new VFB is allocated.  The image is read in, one scanline at

 *	a time, and converted into the VFB.

 */



static int				/* Returns status		*/

#ifdef __STDC__

imRasRead1( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb )

#else

imRasRead1( ioType, fd, fp, flagsTable, tagTable, pVfb )

	int               ioType;	/* I/O flags			*/

	int	          fd;		/* Input file descriptor	*/

	FILE	         *fp;		/* Input file pointer		*/

	TagTable         *flagsTable;	/* Flags			*/

	TagTable         *tagTable;	/* Tag table to add to		*/

	ImVfb	        **pVfb;		/* VFB to fill and return	*/

#endif

{

	unsigned char   *rbp;		/* Run buffer pointer		*/

	int	  	i, j;		/* Counters			*/

	ImVfb	 	*vfb;		/* New vfb			*/

	ImVfbPtr 	pptr;		/* Pixel pointer		*/

	int             n;		/* # of bytes in a scanline	*/

	int		byte;		/* Byte of 8 mono pixels	*/

	unsigned char	*runBuffer;	/* Run buffer			*/





	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (*pVfb = ImVfbAlloc( imRasHeader.ras_width, imRasHeader.ras_height,

		IMVFBMONO )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	vfb = *pVfb;





	/*

	 *  Allocate a buffer that is big enough for one scanline

	 *  (rounded to 2-bytes).  Read in and process the image one

	 *  scanline at a time.

	 */

	pptr = ImVfbQFirst( vfb );

	n    = (imRasHeader.ras_width + 7) / 8;

	if ( (n&1) != 0 )

		n++;		/* Round out to multiple of 2		*/



	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * n );



	for ( i = 0; i < imRasHeader.ras_height; i++ )

	{

		if ( ImBinRead( ioType, fd, fp, runBuffer, UCHAR, 1, n ) == -1 )

		{

			free( (char *)runBuffer );

			ImVfbFree( vfb );

			ImReturnBinError( );

		}

		rbp = runBuffer;

		for ( j = 0; j < imRasHeader.ras_width; j++ ) 

		{

			if ( (j&7) == 0 )

				byte = *rbp++;

			ImVfbSMono( vfb, pptr, (byte & 0x80) ? 0 : 1 );

			ImVfbSInc( vfb, pptr );

			byte <<= 1;

		}

	}



	free( (char *)runBuffer );

	return ( 0 );

}



static int				/* Returns status		*/

#ifdef __STDC__

imRasRead8( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb )

#else

imRasRead8( ioType, fd, fp, flagsTable, tagTable, pVfb )

	int               ioType;	/* I/O flags			*/

	int	          fd;		/* Input file descriptor	*/

	FILE	         *fp;		/* Input file pointer		*/

	TagTable         *flagsTable;	/* Flags			*/

	TagTable         *tagTable;	/* Tag table to add to		*/

	ImVfb	        **pVfb;		/* VFB to fill and return	*/

#endif

{

	unsigned char   *rbp;		/* Run buffer pointer		*/

	int	  i, j;		/* Counters			*/

	ImVfb	 *vfb;		/* New vfb			*/

	ImVfbPtr pptr;		/* Pixel pointer		*/

	int               n;		/* # of bytes in a scanline	*/

	unsigned char	         *runBuffer;	/* Run buffer			*/





	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (*pVfb = ImVfbAlloc( imRasHeader.ras_width, imRasHeader.ras_height,

		IMVFBINDEX8 )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	vfb = *pVfb;





	/*

	 *  Allocate a buffer that is big enough for one scanline

	 *  (rounded to 2-bytes).  Read in and process the image one

	 *  scanline at a time.

	 */

	pptr = ImVfbQFirst( vfb );

	n    = imRasHeader.ras_width;

	if ( (n&1) != 0 )

		n++;		/* Round out to multiple of 2		*/



	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * n );



	for ( i = 0; i < imRasHeader.ras_height; i++ )

	{

		if ( ImBinRead( ioType, fd, fp, runBuffer, UCHAR, 1, n ) == -1 )

		{

			free( (char *)runBuffer );

			ImVfbFree( vfb );

			ImReturnBinError( );

		}

		rbp = runBuffer;

		for ( j = 0; j < imRasHeader.ras_width; j++ ) 

		{

			ImVfbSIndex8( vfb, pptr, *rbp++ );

			ImVfbSInc( vfb, pptr );

		}

	}



	free( (char *)runBuffer );

	return ( 0 );

}



static int				/* Returns status		*/

#ifdef __STDC__

imRasRead24RGB( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb )

#else

imRasRead24RGB( ioType, fd, fp, flagsTable, tagTable, pVfb )

	int             ioType;		/* I/O flags			*/

	int	        fd;		/* Input file descriptor	*/

	FILE	       *fp;		/* Input file pointer		*/

	TagTable       *flagsTable;	/* Flags			*/

	TagTable       *tagTable;	/* Tag table to add to		*/

	ImVfb	      **pVfb;		/* VFB to fill and return	*/

#endif

{

	unsigned char *rbp;		/* Run buffer pointer		*/

	int	i, j;		/* Counters			*/

	ImVfb	*vfb;		/* New vfb			*/

	ImVfbPtr pptr;		/* Pixel pointer		*/

	int             n;		/* # of bytes in a scanline	*/

	unsigned char	       *runBuffer;	/* Run buffer			*/





	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (*pVfb = ImVfbAlloc( imRasHeader.ras_width, imRasHeader.ras_height,

		IMVFBRGB )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	vfb = *pVfb;





	/*

	 *  Allocate a buffer that is big enough for one scanline

	 *  (rounded to 2-bytes).  Read in and process the image one

	 *  scanline at a time.

	 */

	pptr = ImVfbQFirst( vfb );

	n    = imRasHeader.ras_width * 3;	/* # of bytes		*/

	if ( (n&1) != 0 )

		n++;		/* Round out to multiple of 2		*/



	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * n );



	for ( i = 0; i < imRasHeader.ras_height; i++ )

	{

		if ( ImBinRead( ioType, fd, fp, runBuffer, UCHAR, 1, n ) == -1 )

		{

			free( (char *)runBuffer );

			ImVfbFree( vfb );

			ImReturnBinError( );

		}

		rbp = runBuffer;



		for ( j = 0; j < imRasHeader.ras_width; j++ ) 

		{

			ImVfbSBlue(  vfb, pptr, *rbp++ );

			ImVfbSGreen( vfb, pptr, *rbp++ );

			ImVfbSRed(   vfb, pptr, *rbp++ );

			ImVfbSInc(   vfb, pptr );

		}

	}



	free( (char *)runBuffer );

	return ( 0 );

}



static int				/* Returns status		*/

#ifdef __STDC__

imRasRead32RGBA( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb )

#else

imRasRead32RGBA( ioType, fd, fp, flagsTable, tagTable, pVfb )

	int             ioType;		/* I/O flags			*/

	int	        fd;		/* Input file descriptor	*/

	FILE	       *fp;		/* Input file pointer		*/

	TagTable       *flagsTable;	/* Flags			*/

	TagTable       *tagTable;	/* Tag table to add to		*/

	ImVfb	      **pVfb;		/* VFB to fill and return	*/

#endif

{

	unsigned char *rbp;		/* Run buffer pointer		*/

	int	i, j;		/* Counters			*/

	ImVfb	*vfb;		/* New vfb			*/

	ImVfbPtr pptr;		/* Pixel pointer		*/

	int             n;		/* # of bytes in a scanline	*/

	unsigned char	       *runBuffer;	/* Run buffer			*/





	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (*pVfb = ImVfbAlloc( imRasHeader.ras_width, imRasHeader.ras_height,

		IMVFBRGB | IMVFBALPHA )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	vfb = *pVfb;





	/*

	 *  Allocate a buffer that is big enough for one scanline

	 *  (rounded to 2-bytes).  Read in and process the image one

	 *  scanline at a time.

	 */

	pptr = ImVfbQFirst( vfb );

	n    = imRasHeader.ras_width * 4;	/* # of bytes		*/

	if ( (n&1) != 0 )

		n++;		/* Round out to multiple of 2		*/



	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * n );



	for ( i = 0; i < imRasHeader.ras_height; i++ )

	{

		if ( ImBinRead( ioType, fd, fp, runBuffer, UCHAR, 1, n ) == -1 )

		{

			free( (char *)runBuffer );

			ImVfbFree( vfb );

			ImReturnBinError( );

		}

		rbp = runBuffer;



		for ( j = 0; j < imRasHeader.ras_width; j++ ) 

		{

			ImVfbSAlpha( vfb, pptr, *rbp++ );

			ImVfbSBlue(  vfb, pptr, *rbp++ );

			ImVfbSGreen( vfb, pptr, *rbp++ );

			ImVfbSRed(   vfb, pptr, *rbp++ );

			ImVfbSInc(   vfb, pptr );

		}

	}



	free( (char *)runBuffer );

	return ( 0 );

}











/*

 *  FUNCTION

 *	imRasReadRLE1	-  read 1-bit encoded RAS format

 *	imRasReadRLE8	-  read 8-bit encoded RAS format

 *	imRasReadRLE24RGB	-  read 24-bit RGB encoded RAS format

 *	imRasReadRLE32RGBA	-  read 32-bit RGBA encoded RAS format

 *

 *  DESCRIPTION

 *	Each of these routines deal with "encoded" format RAS files.

 *	The output stream is a file or a pipe.  We don't care.

 *

 *	A new VFB is allocated.  The image is read into an image-sized

 *	buffer, then converted into the VFB.

 *

 *	We allocate a buffer the size of the file's run's because it is

 *	awkward to decode runs in chunks.  This should be done in the future

 *	in order to reduce the memory use of these routines.

 */



static int				/* Returns status		*/

#ifdef __STDC__

imRasReadRLE1( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb )

#else

imRasReadRLE1( ioType, fd, fp, flagsTable, tagTable, pVfb )

	int               ioType;	/* I/O flags			*/

	int	          fd;		/* Input file descriptor	*/

	FILE	         *fp;		/* Input file pointer		*/

	TagTable         *flagsTable;	/* Flags			*/

	TagTable         *tagTable;	/* Tag table to add to		*/

	ImVfb	        **pVfb;		/* VFB to fill and return	*/

#endif

{

	unsigned char   *rbp;		/* Run buffer pointer		*/

	ImVfb	  *vfb;		/* New vfb			*/

	ImVfbPtr pptr;		/* Pixel pointer		*/

	unsigned char	  color;	/* Color			*/

	int	  count;	/* Run-length encodeing count	*/

	int      	  n;		/* # of bytes in image		*/

	unsigned char	         *runBuffer;	/* Run buffer			*/

	unsigned char            *endrbp;	/* Ending rbp pointer value	*/

	int		  nPixels;	/* # of pixels so far in scanline*/

	int		  byte;		/* Byte of pixels		*/

	int		  pad;		/* Pad scanline?		*/

	long		  fileOffset;	/* Location in file		*/





	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (*pVfb = ImVfbAlloc( imRasHeader.ras_width, imRasHeader.ras_height,

		IMVFBMONO )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	vfb = *pVfb;





	/*

	 *  The imRasHeader.ras_length field of the file header is supposed

	 *  to be set to the number of bytes of image data in the file.

	 *  Unfortunately, this has been interpreted to mean several different

	 *  things by different people:

	 *

	 *	-  height * width, even when compressed.

	 *	-  the actual number of compressed bytes.

	 *	-  zero.

	 *	-  a number of variants on these.

	 *

	 *  This makes the ras_length field an undependable value.

	 *

	 *  So, we'll ignore it.  We seek to the end of the file and see

	 *  how many bytes there are from here to there, then read them all in.

	 *  This is a potential big waste of memory, but there's little we

	 *  can safely do about it.

	 */

	fileOffset = ImTell( ioType, fd, fp );

	ImSeek( ioType, fd, fp, 0, L_XTND );

	n = ImTell( ioType, fd, fp ) - fileOffset;

	ImSeek( ioType, fd, fp, fileOffset, L_SET );





	/*

	 *  Allocate a buffer big enough for all those bytes.

	 */

	pptr = ImVfbQFirst( vfb );

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * n );

	rbp = runBuffer;





	/*

	 *  And read them in.

	 */

	if ( (n = ImBinRead( ioType, fd, fp, runBuffer, UCHAR, 1, n )) == -1 )

	{

		free( (char *)runBuffer );

		ImVfbFree( vfb );

		ImReturnBinError( );

	}



	endrbp  = &runBuffer[n-1];

	nPixels = 0;

	pad     = ((int)((imRasHeader.ras_width + 7)/ 8) & 0x01) ? TRUE : FALSE;

	while ( rbp <= endrbp )

	{

		color = *rbp++;

		count = 1;

		if ( color == IMRASESC && (count += *rbp++) != 1 )

			color = *rbp++;



		while ( count-- != 0 )

		{

			if ( nPixels >= imRasHeader.ras_width )

			{

				/* End of scan line.			*/

				nPixels = 0;

				if ( pad )

					continue;	/* Skip pixel	*/

			}



			/*

			 *  Expand byte into up to 8 pixels.

			 */

			if ( (nPixels + 8) <= imRasHeader.ras_width )

			{

				/* Expand full byte.  Unwrap loop for speed.*/

				ImVfbSMono( vfb, pptr, (color&0x80) ? 0 : 1 );

				ImVfbSInc( vfb, pptr );

				ImVfbSMono( vfb, pptr, (color&0x40) ? 0 : 1 );

				ImVfbSInc( vfb, pptr );

				ImVfbSMono( vfb, pptr, (color&0x20) ? 0 : 1 );

				ImVfbSInc( vfb, pptr );

				ImVfbSMono( vfb, pptr, (color&0x10) ? 0 : 1 );

				ImVfbSInc( vfb, pptr );

				ImVfbSMono( vfb, pptr, (color&0x08) ? 0 : 1 );

				ImVfbSInc( vfb, pptr );

				ImVfbSMono( vfb, pptr, (color&0x04) ? 0 : 1 );

				ImVfbSInc( vfb, pptr );

				ImVfbSMono( vfb, pptr, (color&0x02) ? 0 : 1 );

				ImVfbSInc( vfb, pptr );

				ImVfbSMono( vfb, pptr, (color&0x01) ? 0 : 1 );

				ImVfbSInc( vfb, pptr );

				nPixels += 8;

				continue;

			}



			/* Expand part of byte.  Unwrap loop for speed.	*/

			byte = color;

			switch ( imRasHeader.ras_width - nPixels )

			{

			case 7:	ImVfbSMono( vfb, pptr, (byte&0x80) ? 0 : 1 );

				ImVfbSInc( vfb, pptr );

				byte <<= 1;

			case 6:	ImVfbSMono( vfb, pptr, (byte&0x80) ? 0 : 1 );

				ImVfbSInc( vfb, pptr );

				byte <<= 1;

			case 5:	ImVfbSMono( vfb, pptr, (byte&0x80) ? 0 : 1 );

				ImVfbSInc( vfb, pptr );

				byte <<= 1;

			case 4:	ImVfbSMono( vfb, pptr, (byte&0x80) ? 0 : 1 );

				ImVfbSInc( vfb, pptr );

				byte <<= 1;

			case 3:	ImVfbSMono( vfb, pptr, (byte&0x80) ? 0 : 1 );

				ImVfbSInc( vfb, pptr );

				byte <<= 1;

			case 2:	ImVfbSMono( vfb, pptr, (byte&0x80) ? 0 : 1 );

				ImVfbSInc( vfb, pptr );

				byte <<= 1;

			case 1:	ImVfbSMono( vfb, pptr, (byte&0x80) ? 0 : 1 );

				ImVfbSInc( vfb, pptr );

				byte <<= 1;

			}

			nPixels = imRasHeader.ras_width;

		}

	}



	free( (char *)runBuffer );

	return ( 0 );

}



static int				/* Returns status		*/

#ifdef __STDC__

imRasReadRLE8( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb )

#else

imRasReadRLE8( ioType, fd, fp, flagsTable, tagTable, pVfb )

	int               ioType;	/* I/O flags			*/

	int	          fd;		/* Input file descriptor	*/

	FILE	         *fp;		/* Input file pointer		*/

	TagTable         *flagsTable;	/* Flags			*/

	TagTable         *tagTable;	/* Tag table to add to		*/

	ImVfb	        **pVfb;		/* VFB to fill and return	*/

#endif

{

	unsigned char   *rbp;		/* Run buffer pointer		*/

	ImVfb	  *vfb;		/* New vfb			*/

	ImVfbPtr pptr;		/* Pixel pointer		*/

	int      n;		/* # of bytes in image		*/

	unsigned char	         *runBuffer;	/* Run buffer			*/

	unsigned char		  color;	/* Color			*/

	int		  count;	/* Run-length encodeing count	*/

	unsigned char            *endrbp;	/* Ending rbp pointer value	*/

	int		  nPixels;	/* # of pixels in scanline	*/

	long		  fileOffset;	/* Location in file		*/





	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (*pVfb = ImVfbAlloc( imRasHeader.ras_width, imRasHeader.ras_height,

		IMVFBINDEX8 )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	vfb = *pVfb;





	/*

	 *  The imRasHeader.ras_length field of the file header is supposed

	 *  to be set to the number of bytes of image data in the file.

	 *  Unfortunately, this has been interpreted to mean several different

	 *  things by different people:

	 *

	 *	-  height * width, even when compressed.

	 *	-  the actual number of compressed bytes.

	 *	-  zero.

	 *	-  a number of variants on these.

	 *

	 *  This makes the ras_length field an undependable value.

	 *

	 *  So, we'll ignore it.  We seek to the end of the file and see

	 *  how many bytes there are from here to there, then read them all in.

	 *  This is a potential big waste of memory, but there's little we

	 *  can safely do about it.

	 */

	fileOffset = ImTell( ioType, fd, fp );

	ImSeek( ioType, fd, fp, 0, L_XTND );

	n = ImTell( ioType, fd, fp ) - fileOffset;

	ImSeek( ioType, fd, fp, fileOffset, L_SET );





	/*

	 *  Allocate a buffer big enough for all those bytes.

	 */

	pptr = ImVfbQFirst( vfb );

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * n );

	rbp = runBuffer;



	/*

	 *  And read them in.

	 */

	if ( (n = ImBinRead( ioType, fd, fp, runBuffer, UCHAR, 1, n )) == -1 )

	{

		free( (char *)runBuffer );

		ImVfbFree( vfb );

		ImReturnBinError( );

	}



	nPixels = 0;

	endrbp  = &runBuffer[n-1];

	while ( rbp <= endrbp )

	{

		color = *rbp++;

		count = 1;

		if ( color == IMRASESC && (count += *rbp++) != 1 )

			color = *rbp++;



		if ( (imRasHeader.ras_width & 0x01) == 0 )

		{

			/* Even width.  No pad bytes.			*/

			while ( count-- != 0 )

			{

				ImVfbSIndex8(  vfb, pptr, color );

				ImVfbSInc( vfb, pptr );

			}

			continue;

		}



		/* Odd width.  Skip pad byte after every 'width' pixels.*/

		while ( count-- != 0 )

		{

			if ( nPixels >= imRasHeader.ras_width )

			{

				nPixels = 0;

				continue;	/* Skip this pixel.	*/

			}

			ImVfbSIndex8( vfb, pptr, color );

			ImVfbSInc( vfb, pptr );

			nPixels++;

		}

	}



	free( (char *)runBuffer );

	return ( 0 );

}



static int				/* Returns status		*/

#ifdef __STDC__

imRasReadRLE24RGB( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb )

#else

imRasReadRLE24RGB( ioType, fd, fp, flagsTable, tagTable, pVfb )

	int             ioType;		/* I/O flags			*/

	int	        fd;		/* Input file descriptor	*/

	FILE	       *fp;		/* Input file pointer		*/

	TagTable       *flagsTable;	/* Flags			*/

	TagTable       *tagTable;	/* Tag table to add to		*/

	ImVfb	      **pVfb;		/* VFB to fill and return	*/

#endif

{

	unsigned char 	*rbp;		/* Run buffer pointer		*/

	ImVfb		*vfb;		/* New vfb			*/

	ImVfbPtr 	pptr;		/* Pixel pointer		*/

	int             n;		/* # of bytes in image		*/

	unsigned char	*runBuffer;	/* Run buffer			*/

	unsigned char	color;		/* Color			*/

	int		count;		/* Run-length encodeing count	*/

	unsigned char   *endrbp;	/* Ending rbp pointer value	*/

	int             onRGB;		/* RGB color cycle counter	*/

	int		nPixels;	/* # of pixels in scanline	*/

	long		fileOffset;	/* Location in file		*/





	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (*pVfb = ImVfbAlloc( imRasHeader.ras_width, imRasHeader.ras_height,

		IMVFBRGB )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	vfb = *pVfb;





	/*

	 *  The imRasHeader.ras_length field of the file header is supposed

	 *  to be set to the number of bytes of image data in the file.

	 *  Unfortunately, this has been interpreted to mean several different

	 *  things by different people:

	 *

	 *	-  height * width, even when compressed.

	 *	-  the actual number of compressed bytes.

	 *	-  zero.

	 *	-  a number of variants on these.

	 *

	 *  This makes the ras_length field an undependable value.

	 *

	 *  So, we'll ignore it.  We seek to the end of the file and see

	 *  how many bytes there are from here to there, then read them all in.

	 *  This is a potential big waste of memory, but there's little we

	 *  can safely do about it.

	 */

	fileOffset = ImTell( ioType, fd, fp );

	ImSeek( ioType, fd, fp, 0, L_XTND );

	n = ImTell( ioType, fd, fp ) - fileOffset;

	ImSeek( ioType, fd, fp, fileOffset, L_SET );





	/*

	 *  Allocate a buffer big enough for all those bytes.

	 */

	pptr = ImVfbQFirst( vfb );

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * n );

	rbp = runBuffer;



	/*

	 *  And read them in.

	 */

	if ( (n = ImBinRead( ioType, fd, fp, runBuffer, UCHAR, 1, n )) == -1 )

	{

		free( (char *)runBuffer );

		ImVfbFree( vfb );

		ImReturnBinError( );

	}



	nPixels = 0;

	endrbp  = &runBuffer[n-1];

	onRGB   = IMRAS_BLUE;

	while ( rbp <= endrbp )

	{

		color = *rbp++;

		count = 1;

		if ( color == IMRASESC && (count += *rbp++) != 1 )

			color = *rbp++;



		if ( (imRasHeader.ras_width & 0x01) == 0 )

		{

			/* Even image width.  No padding needed.	*/

			while ( count-- != 0 )

			{

				switch ( onRGB )

				{

				case IMRAS_BLUE:	ImVfbSBlue( vfb, pptr, color );

						onRGB = IMRAS_GREEN;

						continue;

				case IMRAS_GREEN:	ImVfbSGreen( vfb, pptr, color );

						onRGB = IMRAS_RED;

						continue;

				case IMRAS_RED:	ImVfbSRed( vfb, pptr, color );

						ImVfbSInc( vfb, pptr );

						nPixels++;

						onRGB = IMRAS_BLUE;

						continue;

				}

			}

			continue;

		}



		/* Odd image width.  Pad 1 byte per scanline.		*/

		while ( count-- != 0 )

		{

			if ( nPixels >= imRasHeader.ras_width )

			{

				nPixels = 0;

				continue;	/* Skip this byte.	*/

			}

			switch ( onRGB )

			{

			case IMRAS_BLUE:	ImVfbSBlue( vfb, pptr, color );

					onRGB = IMRAS_GREEN;

					continue;

			case IMRAS_GREEN:	ImVfbSGreen( vfb, pptr, color );

					onRGB = IMRAS_RED;

					continue;

			case IMRAS_RED:	ImVfbSRed( vfb, pptr, color );

					ImVfbSInc( vfb, pptr );

					nPixels++;

					onRGB = IMRAS_BLUE;

					continue;

			}

		}

	}



	free( (char *)runBuffer );

	return ( 0 );

}



static int				/* Returns status		*/

#ifdef __STDC__

imRasReadRLE32RGBA( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable, ImVfb **pVfb )

#else

imRasReadRLE32RGBA( ioType, fd, fp, flagsTable, tagTable, pVfb )

	int             ioType;		/* I/O flags			*/

	int	        fd;		/* Input file descriptor	*/

	FILE	       *fp;		/* Input file pointer		*/

	TagTable       *flagsTable;	/* Flags			*/

	TagTable       *tagTable;	/* Tag table to add to		*/

	ImVfb	      **pVfb;		/* VFB to fill and return	*/

#endif

{

	unsigned char *rbp;		/* Run buffer pointer		*/

	ImVfb	*vfb;		/* New vfb			*/

	ImVfbPtr pptr;		/* Pixel pointer		*/

	int             n;		/* # of bytes in image		*/

	unsigned char		*runBuffer;	/* Run buffer			*/

	unsigned char		color;		/* Color			*/

	int		count;		/* Run-length encodeing count	*/

	unsigned char          *endrbp;		/* Ending rbp pointer value	*/

	int             onRGB;		/* RGB color cycle counter	*/

	int		nPixels;	/* # of pixels in scanline	*/

	long		  fileOffset;	/* Location in file		*/





	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (*pVfb = ImVfbAlloc( imRasHeader.ras_width, imRasHeader.ras_height,

		IMVFBRGB | IMVFBALPHA )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	vfb = *pVfb;





	/*

	 *  The imRasHeader.ras_length field of the file header is supposed

	 *  to be set to the number of bytes of image data in the file.

	 *  Unfortunately, this has been interpreted to mean several different

	 *  things by different people:

	 *

	 *	-  height * width, even when compressed.

	 *	-  the actual number of compressed bytes.

	 *	-  zero.

	 *	-  a number of variants on these.

	 *

	 *  This makes the ras_length field an undependable value.

	 *

	 *  So, we'll ignore it.  We seek to the end of the file and see

	 *  how many bytes there are from here to there, then read them all in.

	 *  This is a potential big waste of memory, but there's little we

	 *  can safely do about it.

	 */

	fileOffset = ImTell( ioType, fd, fp );

	ImSeek( ioType, fd, fp, 0, L_XTND );

	n = ImTell( ioType, fd, fp ) - fileOffset;

	ImSeek( ioType, fd, fp, fileOffset, L_SET );





	/*

	 *  Allocate a buffer big enough for all those bytes.

	 */

	pptr = ImVfbQFirst( vfb );

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * n );

	rbp = runBuffer;



	/*

	 *  And read them in.

	 */

	if ( (n = ImBinRead( ioType, fd, fp, runBuffer, UCHAR, 1, n )) == -1 )

	{

		free( (char *)runBuffer );

		ImVfbFree( vfb );

		ImReturnBinError( );

	}



	nPixels = 0;

	endrbp  = &runBuffer[n-1];

	onRGB   = IMRAS_ALPHA;

	while ( rbp <= endrbp )

	{

		color = *rbp++;

		count = 1;

		if ( color == IMRASESC && (count += *rbp++) != 1 )

			color = *rbp++;



		while ( count-- != 0 )

		{

			switch ( onRGB )

			{

			case IMRAS_ALPHA:	ImVfbSAlpha( vfb, pptr, color );

						onRGB = IMRAS_BLUE;

						continue;

			case IMRAS_BLUE:	ImVfbSBlue( vfb, pptr, color );

						onRGB = IMRAS_GREEN;

						continue;

			case IMRAS_GREEN:	ImVfbSGreen( vfb, pptr, color );

						onRGB = IMRAS_RED;

						continue;

			case IMRAS_RED:		ImVfbSRed( vfb, pptr, color );

						ImVfbSInc( vfb, pptr );

						nPixels++;

						onRGB = IMRAS_ALPHA;

						continue;

			}

		}

	}



	free( (char *)runBuffer );

	return ( 0 );

}











/*

 *  FUNCTION

 *	imRasWriteHeaderClt	-  write header and CLT

 *

 *  DESCRIPTION

 *	A RAS file header is intialized and written out.  If a CLT is to

 *	be written out, it is converted to a CLT buffer, then written

 *	following the header.

 */



static int				/* Returns status		*/

#ifdef __STDC__

imRasWriteHeaderClt( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, ImVfb *vfb, int depth, int type )

#else

imRasWriteHeaderClt( pMap, ioType, fd, fp, vfb, depth, type )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int               ioType;	/* I/O flags			*/

	int	          fd;		/* Output file descriptor	*/

	FILE	         *fp;		/* Output file pointer		*/

	ImVfb		 *vfb;		/* VFB to write out		*/

	int		  depth;	/* Image depth			*/

	int		  type;		/* Image type			*/

#endif

{

	unsigned char		 *cltBuffer;	/* Converted CLT		*/

	int               nClt;		/* # of CLT entries		*/

	char		 message[100];	/* ImInfo message		*/





	/*

	 *  Set up the basic header.

	 */

	imRasHeader.ras_magic  = IMRASMAGIC;

	imRasHeader.ras_width  = ImVfbQWidth(  vfb );

	imRasHeader.ras_height = ImVfbQHeight( vfb );

	imRasHeader.ras_depth  = depth;

	imRasHeader.ras_type   = type;

	imRasHeader.ras_length = imRasHeader.ras_width *

		imRasHeader.ras_height * depth / 8;

	if ( (imRasHeader.ras_width * depth / 8) % 2 != 0 )

	{

		/* Pad each scanline to 2-byte boundary.		*/

		imRasHeader.ras_length += imRasHeader.ras_height * depth / 8;

	}





	/*

	 *  If we've been requested to write the CLT, convert it to a tmp

	 *  buffer first.

	 */

	nClt = 0;

	if ( (pMap->map_outAttributes & IMCLTYES) &&

		imRasCltToBuffer( vfb, &cltBuffer, &nClt ) == -1)

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	/*

	 * Output -verbose message

	 */

	ImInfo( "Byte Order", "Most Significant Byte First" );



	sprintf( message, "%d x %d", ImVfbQWidth( vfb ), ImVfbQHeight( vfb ) );

	ImInfo( "Resolution", message );



	if( depth >= 24 )

		sprintf( message, "%d-bit RGB", depth );

	else

		sprintf( message, "%d-bit Color Indexed", depth );

	ImInfo( "Type", message );



	if( nClt == 0 )

		sprintf( message, "none" );

	else

		sprintf( message, "%d Entries", nClt );

	ImInfo( "Color Table", message );



	if( type == IMRTBYTEENCODED )

	{

		ImInfo("Compression Type","Run Length Encoded (Byte Encoded)");

	}

	else

		ImInfo( "Compression Type", "none (Standard)" );



	if( depth > 24 )	

	{

		ImInfo( "Alpha Channel", "8-bit" );

	}

	else

		ImInfo( "Alpha Channel", "none" );



	/*

	 *  If there's no CLT to write out, just write the header.  Otherwise

	 *  write the header followed by the CLT tmp buffer.

	 */

	if ( nClt == 0 )

	{

		imRasHeader.ras_maplength = 0;

		imRasHeader.ras_maptype = IMRMTNONE;

		if ( ImBinWriteStruct( ioType, fd, fp, &imRasHeader,

			imRasHeaderFields ) == -1 )

		{

			ImReturnBinError( );

		}

		return ( 0 );

	}



	imRasHeader.ras_maplength = nClt * 3;

	imRasHeader.ras_maptype = IMRMTEQUALRGB;

	if ( ImBinWriteStruct( ioType, fd, fp, &imRasHeader,

		imRasHeaderFields ) == -1 )

	{

		ImReturnBinError( );

	}



	if ( ImBinWrite( ioType, fd, fp, cltBuffer, UCHAR, 1, 

		imRasHeader.ras_maplength ) == -1 )

	{

		free( (char *)cltBuffer );

		ImReturnBinError( );

	}



	free( (char *)cltBuffer );

	return ( 0 );

}











/*

 *  FUNCTION

 *	imRasCltToBuffer	-  convert CLT to file format buffer

 *

 *  DESCRIPTION

 *	The CLT for the given VFB is converted into a CLT buffer for later

 *	writing out to a RAS image file.

 *

 *	Since RAS CLT's are not a fixed size, we just have to convert

 *	the actual entries in our CLT and not worry about padding the

 *	convert buffer with dummy CLT entries.

 */



static int				/* Returns status		*/

#ifdef __STDC__

imRasCltToBuffer( ImVfb *vfb, unsigned char **cltBuffer, int *nClt )

#else

imRasCltToBuffer( vfb, cltBuffer, nClt )

	ImVfb            *vfb;		/* VFB to check			*/

	unsigned char           **cltBuffer;	/* Returned buffer		*/

	int              *nClt;		/* Returned # of CLT entries	*/

#endif

{

	ImCltPtr cptr;		/* CLT pointer			*/

	unsigned char   *redp;		/* Red CLT range pointer	*/

	unsigned char   *greenp;	/* Green CLT range pointer	*/

	unsigned char   *bluep;	/* Blue CLT range pointer	*/

	ImClt	          *clt;		/* Colormap			*/

	int               i;		/* Counter			*/

	int               n;		/* Number of CLT entries	*/





	/* Get the VFB's CLT (if any).					*/

	clt = ImVfbQClt( vfb );

	if ( clt == IMCLTNULL )

	{

		/* Isn't one.  Never mind.				*/

		*nClt      = 0;

		*cltBuffer = NULL;

		return ( 0 );		/* No CLT, but that's OK.	*/

	}



	*nClt  = n = ImCltQNColors( clt );

	ImMalloc( *cltBuffer, unsigned char *, sizeof( unsigned char ) * n * 3 );



	redp   = *cltBuffer;

	greenp = *cltBuffer + n;

	bluep  = *cltBuffer + n + n;

	cptr   = ImCltQFirst( clt );

	for ( i = 0; i < n; i++ )

	{

		*redp++   = ImCltQRed(   cptr );

		*greenp++ = ImCltQGreen( cptr );

		*bluep++  = ImCltQBlue(  cptr );

		ImCltSInc( clt, cptr );

	}



	return ( 0 );			/* CLT came from VFB		*/

}











/*

 *  FUNCTION

 *	imRasWrite1	-  write 1-bit standard RAS format

 *	imRasWrite8	-  write 8-bit standard RAS format

 *	imRasWriteRGB	-  write 24-bit RGB standard RAS format

 *	imRasWriteRGBA	-  write 32-bit RGBA standard RAS format

 *

 *  DESCRIPTION

 *	Each of these routines deal with "standard" format RAS files

 *	(ie., no run-length encoding).  The output stream may be a pipe

 *	or a file.  We don't care.

 *

 *	The header and CLT (if any) are written out.  The VFB is then

 *	converted and written out one scanline at a time.  Scanlines

 *	are always padded to 2-byte boundaries.

 */



static int				/* Returns status		*/

#ifdef __STDC__

imRasWrite1( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRasWrite1( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int               ioType;	/* I/O flags			*/

	int	          fd;		/* Output file descriptor	*/

	FILE	         *fp;		/* Output file pointer		*/

	TagTable         *flagsTable;	/* Flags			*/

	TagTable         *tagTable;	/* Tag table to read from	*/

#endif

{

	int      i, j;		/* Counters			*/

	ImVfbPtr pptr;		/* Pixel pointer		*/

	unsigned char   *rbp;		/* Run buffer pointer		*/

	ImVfb            *vfb;		/* VFB to write out		*/

	unsigned char            *runBuffer;	/* Run buffer			*/

	int               n;		/* # of bytes to write		*/

	unsigned char		  byte;		/* New byte of pixels		*/





	/*

	 *  Write out the header and possible CLT.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	BinByteOrder( BINMBF );

	if ( imRasWriteHeaderClt( pMap, ioType, fd, fp, vfb, 1,

		IMRTSTANDARD ) == -1 )

		return ( -1 );		/* Error already handled	*/





	/*

	 *  Convert and write out the image.

	 */

	n = (imRasHeader.ras_width + 7) / 8;	/* # bytes		*/

	if ( (n&1) != 0 )

		n++;				/* Pad to 2-byte boundary*/

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * n );

	runBuffer[n-1] = 0;			/* Set the pad byte	*/

	pptr           = ImVfbQFirst( vfb );



	for ( i = 0; i < imRasHeader.ras_height; i++ )

	{

		rbp  = runBuffer;

		byte = 0;

		for ( j = 0; j < imRasHeader.ras_width; j++ )

		{

			byte <<= 1;

			if ( ImVfbQMono( vfb, pptr ) == 0 )

				byte |= 1;

			ImVfbSInc( vfb, pptr );

			if ( (j&0x7) == 0x7 )

			{

				*rbp++ = byte;

				byte   = 0;

			}

		}

		if ( (j&0x7) != 0 )

			*rbp++ = byte;



		if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, n ) == -1)

		{

			free( (unsigned char *)runBuffer );

			ImReturnBinError( );

		}

	}

	free( (unsigned char *)runBuffer );



	return ( 1 );

}



static int				/* Returns status		*/

#ifdef __STDC__

imRasWrite8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRasWrite8( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int               ioType;	/* I/O flags			*/

	int	          fd;		/* Output file descriptor	*/

	FILE	         *fp;		/* Output file pointer		*/

	TagTable         *flagsTable;	/* Flags			*/

	TagTable         *tagTable;	/* Tag table to read from	*/

#endif

{

	int      i, j;		/* Counters			*/

	ImVfbPtr pptr;		/* Pixel pointer		*/

	unsigned char   *rbp;		/* Run buffer pointer		*/

	ImVfb            *vfb;		/* VFB to write out		*/

	unsigned char            *runBuffer;	/* Run buffer			*/

	int               n;		/* # of bytes to write		*/





	/*

	 *  Write out the header and possible CLT.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	BinByteOrder( BINMBF );

	if ( imRasWriteHeaderClt( pMap, ioType, fd, fp, vfb, 8,

		IMRTSTANDARD ) == -1 )

		return ( -1 );		/* Error already handled	*/





	/*

	 *  Convert and write out the image.

	 */

	n = imRasHeader.ras_width * imRasHeader.ras_depth / 8;/* # bytes*/

	if ( (n&1) != 0 )

		n++;				/* Pad to 2-byte boundary*/

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * n );

	runBuffer[n-1] = 0;			/* Set the pad byte	*/

	pptr           = ImVfbQFirst( vfb );



	for ( i = 0; i < imRasHeader.ras_height; i++ )

	{

		rbp = runBuffer;

		for ( j = 0; j < imRasHeader.ras_width; j++ )

		{

			*rbp++ = ImVfbQIndex8( vfb, pptr );

			ImVfbSInc( vfb, pptr );

		}



		if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, n ) == -1)

		{

			free( (unsigned char *)runBuffer );

			ImReturnBinError( );

		}

	}

	free( (unsigned char *)runBuffer );



	return ( 1 );

}



static int				/* Returns status		*/

#ifdef __STDC__

imRasWriteRGB( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRasWriteRGB( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int               ioType;	/* I/O flags			*/

	int	          fd;		/* Output file descriptor	*/

	FILE	         *fp;		/* Output file pointer		*/

	TagTable         *flagsTable;	/* Flags			*/

	TagTable         *tagTable;	/* Tag table to read from	*/

#endif

{

	int      i, j;		/* Counters			*/

	ImVfbPtr pptr;		/* Pixel pointer		*/

	unsigned char   *rbp;		/* Run buffer pointer		*/

	ImVfb            *vfb;		/* VFB to write out		*/

	unsigned char            *runBuffer;	/* Run buffer			*/

	int               n;		/* # of bytes to write		*/





	/*

	 *  Write out the header and possible CLT.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	BinByteOrder( BINMBF );

	if ( imRasWriteHeaderClt( pMap, ioType, fd, fp, vfb, 24,

		IMRTSTANDARD ) == -1 )

		return ( -1 );		/* Error already handled	*/





	/*

	 *  Convert and write out the image.

	 */

	n = imRasHeader.ras_width * imRasHeader.ras_depth / 8;

	if ( (n&1) != 0 )

		n++;				/* Pad to 2-byte boundary*/

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * n );

	runBuffer[n-1] = 0;			/* Set the pad byte	*/

	pptr           = ImVfbQFirst( vfb );



	for ( i = 0; i < imRasHeader.ras_height; i++ )

	{

		rbp = runBuffer;

		for ( j = 0; j < imRasHeader.ras_width; j++ )

		{

			*rbp++ = ImVfbQBlue( vfb, pptr );

			*rbp++ = ImVfbQGreen( vfb, pptr );

			*rbp++ = ImVfbQRed( vfb, pptr );

			ImVfbSInc( vfb, pptr );

		}



		if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, n ) == -1)

		{

			free( (unsigned char *)runBuffer );

			ImReturnBinError( );

		}

	}

	free( (unsigned char *)runBuffer );



	return ( 1 );

}



static int				/* Returns status		*/

#ifdef __STDC__

imRasWriteRGBA( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRasWriteRGBA( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int               ioType;	/* I/O flags			*/

	int	          fd;		/* Output file descriptor	*/

	FILE	         *fp;		/* Output file pointer		*/

	TagTable         *flagsTable;	/* Flags			*/

	TagTable         *tagTable;	/* Tag table to read from	*/

#endif

{

	int      i, j;		/* Counters			*/

	ImVfbPtr pptr;		/* Pixel pointer		*/

	unsigned char   *rbp;		/* Run buffer pointer		*/

	ImVfb            *vfb;		/* VFB to write out		*/

	unsigned char            *runBuffer;	/* Run buffer			*/

	int               n;		/* # of bytes to write		*/





	/*

	 *  Write out the header and possible CLT.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	BinByteOrder( BINMBF );

	if ( imRasWriteHeaderClt( pMap, ioType, fd, fp, vfb, 32,

		IMRTSTANDARD ) == -1 )

		return ( -1 );		/* Error already handled	*/





	/*

	 *  Convert and write out the image.

	 */

	n = imRasHeader.ras_width * imRasHeader.ras_depth / 8;

	if ( (n&1) != 0 )

		n++;				/* Pad to 2-byte boundary*/

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * n );

	runBuffer[n-1] = 0;			/* Set the pad byte	*/

	pptr           = ImVfbQFirst( vfb );



	for ( i = 0; i < imRasHeader.ras_height; i++ )

	{

		rbp = runBuffer;

		for ( j = 0; j < imRasHeader.ras_width; j++ )

		{

			*rbp++ = ImVfbQAlpha( vfb, pptr );

			*rbp++ = ImVfbQBlue(  vfb, pptr );

			*rbp++ = ImVfbQGreen( vfb, pptr );

			*rbp++ = ImVfbQRed(   vfb, pptr );

			ImVfbSInc( vfb, pptr );

		}



		if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, n ) == -1)

		{

			free( (unsigned char *)runBuffer );

			ImReturnBinError( );

		}

	}

	free( (unsigned char *)runBuffer );



	return ( 1 );

}











/*

 *  MACROS

 *	IMAddRun		-  add another pixel to RLE buffer

 *	IMDumpRun		-  dump an RLE run into the run buffer

 *

 *  DESCRIPTION

 *	The 'index' pixel is added to the caller's run-length encoding

 *	buffer.  This involves checking if it differs from 'oldindex'

 *	(the previous index value).

 *

 *	If not, the run count is incremented.  However, if it reaches 256,

 *	we have to stop the run, copy it into the outgoing buffer, and

 *	start a new run.

 *

 *	If 'index' isn't the same as the current run index, then the run

 *	is over.  If it was a run of ESC values, the run is dumped as a

 *	list of ESC,0 pairs.  If the run was a count of 1, then the pixel

 *	is dumped into the run buffer raw.  Otherwise a run is dumped to

 *	the run buffer.

 */



#define IMAddRun(count,oldindex,index,rbp,nBytes)			\
{									\
	if ( (index) == (oldindex) )					\
	{								\
		if ( ++(count) != 256 )					\
			continue;					\
		/* Overflow.  Dump run.					*/\
		IMDumpRun( count, oldindex, index, rbp, nBytes );	\
		(count) = 0;						\
	}								\
	else								\
	{								\
		/* End of run.  Dump run.				*/\
		IMDumpRun( count, oldindex, index, rbp, nBytes );	\
		(oldindex) = (index);					\
		(count)    = 1;						\
	}								\
}



#define IMDumpRun( count, oldindex, index, rbp, nBytes )		\
	if ( (count) != 0 )						\
	{								\
		if ( (oldindex) == IMRASESC )				\
		{							\
			while ( (count)-- )				\
			{						\
				*(rbp)++ = IMRASESC;			\
				*(rbp)++ = 0;				\
				(nBytes) += 2;				\
			}						\
		}							\
		else if ( count == 1 )					\
		{							\
			/* Use the raw format.				*/\
			*(rbp)++   = (oldindex);			\
			(nBytes)++;					\
		}							\
		else							\
		{							\
			/* Use count format.				*/\
			*(rbp)++ = IMRASESC;				\
			*(rbp)++ = (count) - 1;				\
			*(rbp)++ = (oldindex);				\
			(nBytes)+= 3;					\
		}							\
	}











/*

 *  FUNCTION

 *	imRasWriteRLE1		-  write 1-bit encoded RAS format

 *	imRasWriteRLE8		-  write 8-bit encoded RAS format

 *	imRasWriteRLERGB	-  write 24-bit RGB encoded RAS format

 *	imRasWriteRLERGBA	-  write 32-bit RGBA encoded RAS format

 *

 *  DESCRIPTION

 *	Each of these routines deal with "encoded" format RAS files.

 *	The output stream is a file.

 *

 *	The header and CLT (if any) are written out.  The VFB is then

 *	converted and written out in chunks.  When we are all done and

 *	know how big the encoded image was, we seek back and fill in

 *	the length field of the header.

 */



static int				/* Returns status		*/

#ifdef __STDC__

imRasWriteRLE1( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRasWriteRLE1( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int               ioType;	/* I/O flags			*/

	int	          fd;		/* Output file descriptor	*/

	FILE	         *fp;		/* Output file pointer		*/

	TagTable         *flagsTable;	/* Flags			*/

	TagTable         *tagTable;	/* Tag table to read from	*/

#endif

{

	int      i, j, k;	/* Counters			*/

	ImVfbPtr pptr;		/* Pixel pointer		*/

	unsigned char   *rbp;		/* Run buffer pointer		*/

	ImVfb            *vfb;		/* VFB to write out		*/

	unsigned char            *runBuffer;	/* Run buffer			*/

	int               n;		/* # of bytes to write		*/

	int		  nBytes;	/* # of bytes to write		*/

	int		  count;	/* Run length count		*/

	int		  index;	/* Run index			*/

	int		  oldindex;	/* Prevous run index		*/

	int		  width;	/* Width in bytes		*/





	/*

	 *  Write out the header and possible CLT.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	BinByteOrder( BINMBF );

	if ( imRasWriteHeaderClt( pMap, ioType, fd, fp, vfb, 1,

		IMRTBYTEENCODED ) == -1 )

		return ( -1 );		/* Error already handled	*/





	/*

	 *  Convert and write out the image.  Maximum run buffer is twice

	 *  size of one scanline.

	 */

	width = (imRasHeader.ras_width + 7) / 8;

	n     = 2 * width;

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * n );

	pptr = ImVfbQFirst( vfb );

	imRasHeader.ras_length = 0;





	for ( i = 0; i < imRasHeader.ras_height; i++ )

	{

		rbp    = runBuffer;

		nBytes = 0;

		count  = 1;



		if ( imRasHeader.ras_width < 8 )

		{

			for ( oldindex = 0, k = 0; k < imRasHeader.ras_width; k++ )

			{

				oldindex <<= 1;

				if ( ImVfbQMono( vfb, pptr ) == 0 )

					oldindex |= 1;

				ImVfbSInc( vfb, pptr );

			}

			oldindex <<= (8-k);

		}

		else

		{

			for ( oldindex = 0, k = 0; k < 8; k++ )

			{

				oldindex <<= 1;

				if ( ImVfbQMono( vfb, pptr ) == 0 )

					oldindex |= 1;

				ImVfbSInc( vfb, pptr );

			}

		}



		for ( j = k; j < imRasHeader.ras_width; )

		{

			if ( imRasHeader.ras_width < (j+8) )

			{

				for ( index = 0, k = 0; j < imRasHeader.ras_width; j++, k++ )

				{

					index <<= 1;

					if ( ImVfbQMono( vfb, pptr ) == 0 )

						index |= 1;

					ImVfbSInc( vfb, pptr );

				}

				index <<= (8-k);

			}

			else

			{

				for ( index = 0, k = 0; k < 8; k++ )

				{

					index <<= 1;

					if ( ImVfbQMono( vfb, pptr ) == 0 )

						index |= 1;

					ImVfbSInc( vfb, pptr );

				}

				j += 8;

			}

			IMAddRun( count, oldindex, index, rbp, nBytes );

		}



		/* Dump last run.					*/

		IMDumpRun( count, oldindex, index, rbp, nBytes );

		if ( width & 0x1 )

		{

			nBytes++;		/* Strange 2-byte padding*/

			*rbp++ = 0;

		}

		if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, nBytes ) == -1 )

		{

			free( (unsigned char *)runBuffer );

			ImReturnBinError( );

		}

		imRasHeader.ras_length += nBytes;

	}

	free( (unsigned char *)runBuffer );





	/*

	 *  Seek back to the head of the file and rewrite the header.

	 */

	ImSeek( ioType, fd, fp, 0, 0 );

	if ( ImBinWriteStruct( ioType, fd, fp, &imRasHeader,

		imRasHeaderFields ) == -1 )

	{

		ImReturnBinError( );

	}



	return ( 1 );

}



static int				/* Returns status		*/

#ifdef __STDC__

imRasWriteRLE8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRasWriteRLE8( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int               ioType;	/* I/O flags			*/

	int	          fd;		/* Output file descriptor	*/

	FILE	         *fp;		/* Output file pointer		*/

	TagTable         *flagsTable;	/* Flags			*/

	TagTable         *tagTable;	/* Tag table to read from	*/

#endif

{

	int      i, j;		/* Counters			*/

	ImVfbPtr pptr;		/* Pixel pointer		*/

	unsigned char   *rbp;		/* Run buffer pointer		*/

	ImVfb            *vfb;		/* VFB to write out		*/

	unsigned char            *runBuffer;	/* Run buffer			*/

	int               n;		/* # of bytes to write		*/

	int		  nBytes;	/* # of bytes to write		*/

	int		  count;	/* Run length count		*/

	int		  index;	/* Run index			*/

	int		  oldindex;	/* Prevous run index		*/





	/*

	 *  Write out the header and possible CLT.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	BinByteOrder( BINMBF );

	if ( imRasWriteHeaderClt( pMap, ioType, fd, fp, vfb, 8,

		IMRTBYTEENCODED ) == -1 )

		return ( -1 );		/* Error already handled	*/





	/*

	 *  Convert and write out the image.  Maximum run buffer is twice

	 *  size of one scanline.

	 */

	n = 2 * imRasHeader.ras_width * imRasHeader.ras_depth / 8;

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * n );

	pptr = ImVfbQFirst( vfb );

	imRasHeader.ras_length = 0;



	for ( i = 0; i < imRasHeader.ras_height; i++ )

	{

		rbp    = runBuffer;

		nBytes = 0;

		count  = 1;



		oldindex = ImVfbQIndex8( vfb, pptr );

		ImVfbSInc( vfb, pptr );



		for ( j = 1; j < imRasHeader.ras_width; j++ )

		{

			index = ImVfbQIndex8( vfb, pptr );

			ImVfbSInc( vfb, pptr );

			IMAddRun( count, oldindex, index, rbp, nBytes );

		}



		/* Dump last run.					*/

		IMDumpRun( count, oldindex, index, rbp, nBytes );

		if ( imRasHeader.ras_width & 0x1 )

		{

			nBytes++;		/* Strange 2-byte padding*/

			*rbp++ = 0;

		}

		if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, nBytes ) == -1 )

		{

			free( (unsigned char *)runBuffer );

			ImReturnBinError( );

		}

		imRasHeader.ras_length += nBytes;

	}

	free( (unsigned char *)runBuffer );





	/*

	 *  Seek back to the head of the file and rewrite the header.

	 */

	ImSeek( ioType, fd, fp, 0, 0 );

	if ( ImBinWriteStruct( ioType, fd, fp, &imRasHeader,

		imRasHeaderFields ) == -1 )

	{

		ImReturnBinError( );

	}



	return ( 1 );

}



static int				/* Returns status		*/

#ifdef __STDC__

imRasWriteRLERGB( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRasWriteRLERGB( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int               ioType;	/* I/O flags			*/

	int	          fd;		/* Output file descriptor	*/

	FILE	         *fp;		/* Output file pointer		*/

	TagTable         *flagsTable;	/* Flags			*/

	TagTable         *tagTable;	/* Tag table to read from	*/

#endif

{

	int      i, j;		/* Counters			*/

	ImVfbPtr pptr;		/* Pixel pointer		*/

	unsigned char   *rbp;		/* Run buffer pointer		*/

	ImVfb            *vfb;		/* VFB to write out		*/

	unsigned char            *runBuffer;	/* Run buffer			*/

	int		  nBytes;	/* # of bytes to write		*/

	int		  count;	/* Run length count		*/

	int		  index;	/* Run index			*/

	int		  oldindex;	/* Prevous run index		*/

	int		  onRGB;	/* Which channel from VFB next?	*/

	int               n;		/* # of bytes to write		*/





	/*

	 *  Write out the header and possible CLT.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	BinByteOrder( BINMBF );

	if ( imRasWriteHeaderClt( pMap, ioType, fd, fp, vfb, 24,

		IMRTBYTEENCODED ) == -1 )

		return ( -1 );		/* Error already handled	*/



	/*

	 *  Convert and write out the image.  Maximum run buffer is twice

	 *  size of one scanline.

	 */

	n = 2 * imRasHeader.ras_width * imRasHeader.ras_depth / 8;

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * n );

	pptr = ImVfbQFirst( vfb );

	imRasHeader.ras_length = 0;



	for ( i = 0; i < imRasHeader.ras_height; i++ )

	{

		rbp   = runBuffer;

		nBytes = 0;

		count = 1;



		oldindex = ImVfbQBlue( vfb, pptr );

		onRGB    = IMRAS_GREEN;



		for ( j = 1; j < imRasHeader.ras_width*3; j++ )

		{

			switch ( onRGB )

			{

			case IMRAS_BLUE:	/* Get IMRAS_BLUE next.		*/

				index = ImVfbQBlue( vfb, pptr );

				onRGB = IMRAS_GREEN;

				break;

			case IMRAS_GREEN:	/* Get IMRAS_GREEN next.		*/

				index = ImVfbQGreen( vfb, pptr );

				onRGB = IMRAS_RED;

				break;

			case IMRAS_RED:	/* Get IMRAS_RED next.		*/

				index = ImVfbQRed( vfb, pptr );

				ImVfbSInc( vfb, pptr );

				onRGB = IMRAS_BLUE;

				break;

			}



			IMAddRun( count, oldindex, index, rbp, nBytes );

		}



		/* Dump last run.					*/

		IMDumpRun( count, oldindex, index, rbp, nBytes );

		if ( imRasHeader.ras_width & 0x1 )

		{

			nBytes++;		/* Strange 2-byte padding*/

			*rbp++ = 0;

		}



		if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, nBytes ) == -1 )

		{

			free( (unsigned char *)runBuffer );

			ImReturnBinError( );

		}

		imRasHeader.ras_length += nBytes;

	}

	free( (unsigned char *)runBuffer );





	/*

	 *  Seek back to the head of the file and rewrite the header.

	 */

	ImSeek( ioType, fd, fp, 0, 0 );

	if ( ImBinWriteStruct( ioType, fd, fp, &imRasHeader,

		imRasHeaderFields ) == -1 )

	{

		ImReturnBinError( );

	}



	return ( 1 );

}



static int				/* Returns status		*/

#ifdef __STDC__

imRasWriteRLERGBA( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRasWriteRLERGBA( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int               ioType;	/* I/O flags			*/

	int	          fd;		/* Output file descriptor	*/

	FILE	         *fp;		/* Output file pointer		*/

	TagTable         *flagsTable;	/* Flags			*/

	TagTable         *tagTable;	/* Tag table to read from	*/

#endif

{

	int      i, j;		/* Counters			*/

	ImVfbPtr pptr;		/* Pixel pointer		*/

	unsigned char   *rbp;		/* Run buffer pointer		*/

	ImVfb            *vfb;		/* VFB to write out		*/

	unsigned char            *runBuffer;	/* Run buffer			*/

	int		  nBytes;	/* # of bytes to write		*/

	int		  count;	/* Run length count		*/

	int		  index;	/* Run index			*/

	int		  oldindex;	/* Prevous run index		*/

	int		  onRGB;	/* Which channel from VFB next?	*/

	int               n;		/* # of bytes to write		*/





	/*

	 *  Write out the header and possible CLT.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	BinByteOrder( BINMBF );

	if ( imRasWriteHeaderClt( pMap, ioType, fd, fp, vfb, 32,

		IMRTBYTEENCODED ) == -1 )

		return ( -1 );		/* Error already handled	*/



	/*

	 *  Convert and write out the image.  Maximum run buffer is twice

	 *  size of one scanline.

	 */

	n = 2 * imRasHeader.ras_width * imRasHeader.ras_depth / 8;

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * n );

	pptr = ImVfbQFirst( vfb );

	imRasHeader.ras_length = 0;



	for ( i = 0; i < imRasHeader.ras_height; i++ )

	{

		rbp   = runBuffer;

		nBytes = 0;

		count = 1;



		oldindex = ImVfbQAlpha( vfb, pptr );

		onRGB    = IMRAS_BLUE;



		for ( j = 1; j < imRasHeader.ras_width*4; j++ )

		{

			switch ( onRGB )

			{

			case IMRAS_ALPHA:	/* Get IMRAS_ALPHA next.	*/

				index = ImVfbQAlpha( vfb, pptr );

				onRGB = IMRAS_BLUE;

				break;

			case IMRAS_BLUE:	/* Get IMRAS_BLUE next.		*/

				index = ImVfbQBlue( vfb, pptr );

				onRGB = IMRAS_GREEN;

				break;

			case IMRAS_GREEN:	/* Get IMRAS_GREEN next.	*/

				index = ImVfbQGreen( vfb, pptr );

				onRGB = IMRAS_RED;

				break;

			case IMRAS_RED:		/* Get IMRAS_RED next.		*/

				index = ImVfbQRed( vfb, pptr );

				ImVfbSInc( vfb, pptr );

				onRGB = IMRAS_ALPHA;

				break;

			}



			IMAddRun( count, oldindex, index, rbp, nBytes );

		}



		/* Dump last run.					*/

		IMDumpRun( count, oldindex, index, rbp, nBytes );



		if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, nBytes ) == -1 )

		{

			free( (unsigned char *)runBuffer );

			ImReturnBinError( );

		}

		imRasHeader.ras_length += nBytes;

	}

	free( (unsigned char *)runBuffer );





	/*

	 *  Seek back to the head of the file and rewrite the header.

	 */

	ImSeek( ioType, fd, fp, 0, 0 );

	if ( ImBinWriteStruct( ioType, fd, fp, &imRasHeader,

		imRasHeaderFields ) == -1 )

	{

		ImReturnBinError( );

	}



	return ( 1 );

}

