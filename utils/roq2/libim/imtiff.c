/**

 **	$Header: /roq/libim/imtiff.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imtiff.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imtiff.c		-  TIFF Raster I/O

 **

 **  PROJECT

 **	libim		-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imtiff.c contains routines to read and write TIFF image files for

 **	the image manipulation library.  Raster data read in is stored

 **	in a VFB and optional CLT in a tag list.  Raster data written

 **	out is taken from a tag list.

 **

 **	The compiler flag -DUSE_TIFF_LIB will use the tiff library written

 **	by Sam Leffler instead of using the routines in this module.

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

 **

 **	imTiffRead		f  read a TIFF Rasterfile

 **	imTiffWrite		f  write a TIFF Rasterfile

 **

 **	imTiffHeaderInfo	t  Rasterfile header information

 **	imTiffHeaderFields	v  imTiffHeaderInfo description for Bin pkg

 **	imTiffHeader		v  Rasterfile header holder

 **	imTiffDataWidth		v  How many bytes for a byte, short, long, etc

 **	imTiffMask		v  Bits to mask when shifting variable data 

 **

 **	imTiffDirEntry		t  An Image File Directory entry

 **	imTiffDirectoryFields	t  imTiffDirEntry description for Bin pkg

 **	imTiffDirData		t  Program internal representation of dir data

 **	imTiffDirInfo		t  Program internal representation of directory

 **

 **	IMTIFFBIGENDIANMAGIC	d  file magic number

 **	IMTIFFLITTLEENDIANMAGIC	d  file magic number

 **	IMTIFF*			d  type enumeration

 **

 **	IMTIFFBITSPERBYTE	d  how big is a byte (8-bits of course)

 **	IMTIFFBYTESPERWORD	d  how many bytes in a word (4)

 **	IMTIFFSHORTSPERWORD	d  how many shorts in a word (2)

 **	NULL			d  an empty pointer

 **

 **	imTiffDecode		f  decode the data in the directory offset

 **	imTiffReadDirectory	f  read a tiff Image File Directory

 **	imTiffFetchData		f  get data pointed to be a directory entry

 **	imTiffDataItem		f  get data out of dirdata structure

 **	imPrintTiffDirInfo	f  print the contents of an internal tiffdir

 **	imPrintTiffDirLongEntry	f  print the contents of a data entry

 **

 **	imTiffRead1		f  read a 1 bit image

 **	imTiffRead4		f  read a 4 bit image

 **	imTiffRead8		f  read a 8 bit image

 **	imTiffRead24		f  read a 24 bit image

 **

 **	imTiffWrite1		f  write a 1 bit image

 **	imTiffWrite8		f  write a 8 bit image

 **	imTiffWrite24		f  write a 24 bit image

 **

 **

 **

 **  HISTORY

 **	$Log: /roq/libim/imtiff.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.41  1995/10/03  20:42:37  bduggan

 **	Put back non-tiff library reading capability,

 **	since it works on the cray.

 **

 **	Revision 1.39  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.38  1995/06/15  21:21:13  bduggan

 **	changed bcopy's to memcpy's.

 **

 **	Revision 1.37  1995/04/03  21:38:35  bduggan

 **	took out #ifdef NEWMAGIC

 **	Made write routines work on decAlpha (LSBF)

 **

 **	Revision 1.36  1995/02/16  21:42:19  bduggan

 **	Commented out index16 writing, since xv & amazon can't read 'em

 **

 **	Revision 1.35  1995/02/01  00:50:40  bduggan

 **	Fixed bug with ReadNonInterleavedTIles

 **

 **	Revision 1.34  1995/01/30  21:30:06  bduggan

 **	Took out some stderr's

 **

 **	Revision 1.33  1995/01/16  22:06:54  bduggan

 **	Added tiff library support!

 **

 **	Revision 1.32  1994/10/27  23:49:05  bduggan

 **	changed ImTiffRead and Write into static functions

 **

 **	Revision 1.31  94/10/03  11:31:04  nadeau

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

 **	Revision 1.30  92/12/03  01:53:02  nadeau

 **	Corrected info messages.

 **	

 **	Revision 1.29  92/11/23  18:43:14  nadeau

 **	Removed use of IMINFOMSG.

 **	

 **	Revision 1.28  92/11/04  12:09:13  groening

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	Also, added multiple magic numbers.

 **	

 **	Revision 1.27  92/10/19  14:08:28  groening

 **	removed some debuggin statements left by vle

 **	

 **	Revision 1.26  92/10/16  13:58:09  vle

 **	Tracked down color table error.

 **	.`

 **	

 **	Revision 1.25  92/08/31  17:36:46  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.24  92/04/08  12:13:47  nadeau

 **	Added extern declarations for private functions.

 **	

 **	Revision 1.23  91/10/03  09:19:48  nadeau

 **	Changed 'interlace' to 'interleave'.

 **	

 **	Revision 1.22  91/09/13  14:10:14  nadeau

 **	Fixed minor typos caused by Alliant ethernet controller that

 **	munged random bytes in files as they were read or written.

 **	

 **	Revision 1.21  91/09/11  13:44:33  todd

 **	update to tiff specification 5.0

 **	

 **	Revision 1.20  91/03/20  16:04:12  todd

 **	Cleaned up code that deals with stripbytecounts and stripoffsets

 **	so it is machine independent.

 **	Fixed ImTiffWrite to write alpha channels.

 **	

 **	Revision 1.19  91/03/20  10:53:22  todd

 **	Fix another bug.  Can't read an array of shorts into an int (4 bytes)

 **	on the cray, of course.

 **	

 **	Revision 1.18  91/03/18  15:45:13  todd

 **	*** empty log message ***

 **	

 **	Revision 1.17  91/03/18  15:44:16  todd

 **	Fixed a bug in writing array of shorts.

 **	Two shorts cannot fit in 4 bytes on the cray.

 **	

 **	Revision 1.16  91/03/15  14:10:16  todd

 **	Fixed two bugs.

 **	Changed INT to CHAR in ImBinWrite in ImTiffWrite

 **	packbits case.

 **	Changed code that reads and writes the offset field

 **	in the tagged directory entries to do correct

 **	type of binary I/O.

 **	Added ImBinFloat() initialization

 **	

 **	Revision 1.15  91/03/14  15:34:31  todd

 **	Added return(0) statemensts where needed.

 **	Fixed imTiffGetClt so it doesn't try to free

 **	a null pointer.

 **	

 **	Revision 1.14  91/03/12  16:17:20  todd

 **	no changes

 **	

 **	Revision 1.13  91/03/12  15:57:17  todd

 **	Fixed conflict between cltRequest and clt == NULL.

 **	An extra bogus header tag was being written out.

 **	

 **	Revision 1.12  91/03/12  11:02:42  nadeau

 **	Added BinFloatFormat select.

 **	

 **	Revision 1.11  91/02/13  14:11:53  nadeau

 **	Added error message handling for read and write return cases

 **	that just returned a -1 instead of doing a message.

 **	

 **	Revision 1.10  91/02/12  17:01:25  nadeau

 **	Removed tag table checking and temp file handling now

 **	handled by ImFileRead and ImFileWrite.  Changed GRAY

 **	to MONO VFB's.  Fixed minor byte order machine dependency

 **	in read and write code for 16-bit indexes.

 **	

 **	Revision 1.9  91/01/29  11:12:32  todd

 **	add monochrome to write routine

 **	

 **	Revision 1.8  91/01/09  15:52:51  todd

 **	Updated some types for better portability.

 ** 

 **	Revision 1.7  90/12/13  13:43:50  rama

 **	Fixed problems with pointers

 ** 

 **	Revision 1.6  90/12/13  12:52:52  rama

 **	fixed problems with bcopy and calloc

 ** 

 **	Revision 1.5  90/12/12  18:52:17  todd

 **	no changes.  Rama is going to fix Todd's non-portable bugs.

 ** 

 **	Revision 1.4  90/09/10  09:57:07  todd

 **	Works for 8 bit images with and without CLTs.  Works for 24 bit images.

 **	Reads 1 and 4 bit images.  Lempel-Ziv Welch encoding works for read

 **	and write.  32 bit images with CLTs should work but its untested.

 **	Alpha channels are supported.

 ** 

 **	Revision 1.3  90/09/06  14:24:22  todd

 **	Added lots of functionality.  Everything works except 

 **	writing out LZW compressed images.  Writing 1 bit images

 **	is not supported either.

 ** 

 **	Revision 1.2  90/07/25  15:17:59  todd

 **	Most read routines work.  Write routines written but not debugged yet.

 ** 

 **	Revision 1.1  90/06/25  13:25:07  todd

 **	Initial revision

 ** 

 **	

 **/



#include <stdio.h>

#include <sys/types.h>

//#include <sys/file.h>

#include <fcntl.h>

#include <errno.h>

#include "iminternal.h"



#ifdef USE_TIFF_LIB

#include "tiff.h"

#include "tiffio.h"

#endif







/*

 **

 **  FORMAT

 **	TIFF - Aldus Tagged Image File Format

 **

 **  AKA

 **	tif

 **

 **  FORMAT REFERENCES

 **	- Bit-Mapped Graphics, Steve Rimmer

 **	- Supercharged Bitmapped Graphics, Steve Rimmer

 **	- Graphics File Formats, David C. Kay, John R. Levine

 **	- TIFF Revision 6.0, Aldus

 **

 **

 **  CODE CREDITS

 **     Custom development, Todd Elvins, San Diego Supercomputer Center, 1992.

 **

 **  DESCRIPTION

 **	TIFF files are composed of a small file header, one or more

 ** 	image directories containing tagged information about an image

 **	in the file, and one or more images.  Images of any depth

 **	can be stored in a Tiff file.  We only deal with 

 **	reading 1, 4, 8, and 24 bit images, and writing 8 and 24 bit

 **	images.  The pixel data in Tiff files can be encoded in a number

 **	of different ways.  Currently we are only handling unencoded

 **	pixel data, and Lempil-Ziv Welsh encoding.  In the future

 **	we might want to add Thunderscan, Macintosh PackBits, or Pixar picio.

 **

 **

 **	The sections of a tiff file are as follows :

 **		

 **		- header		8 bytes		Header info.

 **		- image file directory	variable	points to image data

 **		   (IFD)  There may be

 **		  more than one IFD.

 **		- actual data

 **	The sections are not necessarily in that order; the header point 

 **	to the first IFD, which points to subsequent IFDs and image data.

 **

 **	Header

 **	------

 **	

 **		Bytes	Description

 **		0-1	Byte Order (MSB or LSB)

 **		2-3	Magic number (stored according to byte order)

 **		4-7	Location of the first IFD in the file

 **

 **	Image File Directories

 **	----------------------

 **	An IFD consists of the following information:

 **		- the number of entries in the directory	(2 bytes)

 **		- a series of entries 				(12 bytes each)

 **		- the location of the next IFD or 0 for the	(4 bytes)

 **		  last entry

 **	Each entry in the IFD contains:

 **		- a tag identifying the field			(bytes 0-1)

 **		- the field type				(bytes 2-3)

 **		- the size of the data: 'count' 	 	

 **		  (the actual size of the data is

 **		  sizeof(field_type) * count bytes)		(bytes 4-7)

 **		- the location of the data in the file (must 	

 **		  begin on a word boundary, so this is an even 

 **		  number)					(bytes 8-11)

 **			

 **/



/**

 **	The TIFF library

 **	----------------

 **

 **	This library claims to support many many many different 

 **	possible image types.   The specifications for TIFF allow for

 **	any number of channels in an image, and any depth for each channel.

 **	BUT, most software packages that use the tiff library do not

 **	support such a variety of formats.

 **

 **	Here is what the routines that follow support :

 **

 **	Read support:

 **	

 **		1. RGB Images with

 **		   1,2,4,8 or 16 bits per channel

 **		   alpha or no alpha channel

 **		   non-interleaved

 **		   grouped by scanlines

 **

 **		2. RGB Images with

 **		   8 bits per channel

 **		   alpha or no alpha channel

 **		   plane-interleaved

 **		   grouped by scanlines or tiles

 **

 **		3. Color Indexed or Greyscale Images with

 **		   1,2,4,8, or 16 bits per channel

 **		   plane-interleaved or non-interleaved

 **			(those are equivalent since there's one plane)

 **		   grouped by scanlines or tiles

 **

 **		Compression is handled by the tiff library.  So, whatever

 **		compression schemes the tiff library handles are supported

 **		by the image tools.

 **

 **	Write support:

 **

 **		1. RGB Images with

 **		   8 bits per channel

 **		   alpha or no alpha channel

 **		   non-interleaved or plane-interleaved

 **		   grouped by scanlines or tiles

 **	

 **		2. Color Indexed or Greyscale images with

 **		   1,2,4,8,16 bits per channel

 **		   plane or non-interleaved (equivalent)

 **		   grouped by scanlines

 **

 **		3. Color Indexed or Greyscale images with

 **		   8 bits per channel

 **		   plane or non-interleaved (equivalent)

 **		   grouped by tiles

 **

 **		The compression schemes supported are:

 **

 **			Macintosh PackBits encoding,

 **			Lempel-Ziv & Welch encoding,

 **			JPEG encoding (Discrete Cosine Transform),

 **			none

 **

 **	

 **	The reason for not having write capability for rgb images

 **	with other than 8 bits per channel is because such files

 ** 	make Amazon and xv dump core.  The capability IS in place

 **	to write such images, and this can be accomplished simply

 **	by adding these specifications to the write map below.

 **

 **	i.e. adding

 **

 **       { IMVFBRGB,     0,      RGB,3,1,        0,  imTiffWrite },

 **       { IMVFBRGB,     0,      RGB,3,2,        0,  imTiffWrite },

 **       { IMVFBRGB,     0,      RGB,3,4,        0,  imTiffWrite },

 **

 **	would allow for writing RGB images with no compression and

 **	1,2, or 4 bits per channel.

 **

 **/	



/*

 *  TIFF - Tagged Image File Format

 *      For information on these structures, how to use them, etc. please

 *      see imfmt.c.

 */

#ifdef __STDC__

static int imTiffRead( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable );

static int imTiffWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

#else

static int imTiffRead( );

static int imTiffWrite( );

#endif

static char *imTiffNames[ ]  = { "tiff", "tif", NULL };

static unsigned char imTiffMagicNumberA[ ] = {0x4d, 0x4d };

static unsigned char imTiffMagicNumberB[ ] = {0x49, 0x49 };

static ImFileFormatReadMap imTiffReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

#ifdef USE_TIFF_LIB

        { IN,1,1,       0,      IMVFBMONO,      0 },

        { IN,1,4,       C,      IMVFBINDEX8,    C },

        { IN,1,4,       0,      IMVFBINDEX8,    0 },

        { IN,1,4,       LZW|C,	IMVFBINDEX8, 	C },

        { IN,1,4,       LZW,    IMVFBINDEX8,    0 },

        { IN,1,4,       PB|C,	IMVFBINDEX8, 	C },

        { IN,1,4,       PB,     IMVFBINDEX8,    0 },

        { IN,1,8,       0,      IMVFBINDEX8,    0 },

        { IN,1,8,       0,      IMVFBINDEX8,    0 },

        { IN,1,8,       C,      IMVFBINDEX8,    C },

        { IN,1,32,      0,      IMVFBINDEX16,   0 }, 

        { IN,1,32,      C,      IMVFBINDEX16,   C },

        { RGB,3,8,      0,      IMVFBRGB,       0 },

        { RGB,3,8,      A,      IMVFBRGB,       A },



        { IN,1,1,       PB,     IMVFBMONO,      0 },

        { IN,1,4,       PB,     IMVFBINDEX8,    0 },

        { IN,1,8,       PB,     IMVFBINDEX8,    0 },

        { IN,1,8,       PB,     IMVFBINDEX8,    0 },

        { IN,1,8,       PB|C,   IMVFBINDEX8,    C },

        { IN,1,32,      PB,     IMVFBINDEX16,   0 }, 

        { IN,1,32,      PB|C,   IMVFBINDEX16,   C }, 

        { RGB,3,8,      PB,     IMVFBRGB,       0 },

        { RGB,3,8,      PB|A,   IMVFBRGB,       A },



        { IN,1,1,       LZW,    IMVFBMONO,      0 },

        { IN,1,4,       LZW,    IMVFBINDEX8,    0 },

        { IN,1,8,       LZW,    IMVFBINDEX8,    0 },

        { IN,1,8,       LZW,    IMVFBINDEX8,    0 },

        { IN,1,8,       LZW|C,  IMVFBINDEX8,    C },

        { IN,1,32,      LZW,    IMVFBINDEX16,   0 }, 

        { IN,1,32,      LZW|C,  IMVFBINDEX16,   C }, 

        { RGB,3,8,      LZW,    IMVFBRGB,       0 },

        { RGB,3,8,      LZW|A,  IMVFBRGB,       A },

#else

        { IN,1,1,       0,      IMVFBMONO,      0 },

        { IN,1,4,       0,      IMVFBINDEX8,    0 },

        { IN,1,8,       0,      IMVFBINDEX8,    0 },

        { IN,1,8,       0,      IMVFBINDEX8,    0 },

        { IN,1,8,       C,      IMVFBINDEX8,    C },

        { IN,1,32,      0,      IMVFBINDEX16,   0 },

        { IN,1,32,      C,      IMVFBINDEX16,   C },

        { RGB,3,8,      0,      IMVFBRGB,       0 },

        { RGB,3,8,      A,      IMVFBRGB,       A },



        { IN,1,1,       PB,     IMVFBMONO,      0 },

        { IN,1,4,       PB,     IMVFBINDEX8,    0 },

        { IN,1,8,       PB,     IMVFBINDEX8,    0 },

        { IN,1,8,       PB,     IMVFBINDEX8,    0 },

        { IN,1,8,       PB|C,   IMVFBINDEX8,    C },

        { IN,1,32,      PB,     IMVFBINDEX16,   0 },

        { IN,1,32,      PB|C,   IMVFBINDEX16,   C },

        { RGB,3,8,      PB,     IMVFBRGB,       0 },

        { RGB,3,8,      PB|A,   IMVFBRGB,       A },



        { IN,1,1,       LZW,    IMVFBMONO,      0 },

        { IN,1,4,       LZW,    IMVFBINDEX8,    0 },

        { IN,1,8,       LZW,    IMVFBINDEX8,    0 },

        { IN,1,8,       LZW,    IMVFBINDEX8,    0 },

        { IN,1,8,       LZW|C,  IMVFBINDEX8,    C },

        { IN,1,32,      LZW,    IMVFBINDEX16,   0 },

        { IN,1,32,      LZW|C,  IMVFBINDEX16,   C },

        { RGB,3,8,      LZW,    IMVFBRGB,       0 },

        { RGB,3,8,      LZW|A,  IMVFBRGB,       A },

#endif

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imTiffWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */



#ifdef USE_TIFF_LIB



        { IMVFBMONO,    0,      IN,1,1,         LZW,     imTiffWrite },

        { IMVFBMONO,    0,      IN,1,1,         PB,      imTiffWrite },

        { IMVFBMONO,    0,      IN,1,1,         0,       imTiffWrite },



        { IMVFBMONO,    C,      IN,1,1,         C|LZW,   imTiffWrite },

        { IMVFBMONO,    C,      IN,1,1,         C|PB,    imTiffWrite },

        { IMVFBMONO,    C,      IN,1,1,         C,       imTiffWrite },



        { IMVFBMONO,    0,      IN,1,1,         PI|LZW,  imTiffWrite },

        { IMVFBMONO,    0,      IN,1,1,         PI|PB,   imTiffWrite },

        { IMVFBMONO,    0,      IN,1,1,         PI,      imTiffWrite },

 

        { IMVFBMONO,    C,      IN,1,1,         PI|C|LZW,imTiffWrite },

        { IMVFBMONO,    C,      IN,1,1,         PI|C|PB, imTiffWrite },

        { IMVFBMONO,    C,      IN,1,1,         PI|C,    imTiffWrite },



        { IMVFBINDEX8,  0,      IN,1,8,         LZW,     imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,8,         PB,      imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,8,         0,       imTiffWrite },



        { IMVFBINDEX8,  C,      IN,1,8,         C|LZW,   imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,8,         C|PB,    imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,8,         C,       imTiffWrite },



        { IMVFBINDEX8,  0,      IN,1,8,         PI|LZW,  imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,8,         PI|PB,   imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,8,         PI,      imTiffWrite },



        { IMVFBINDEX8,  C,      IN,1,8,         PI|C|LZW,imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,8,         PI|C|PB, imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,8,         PI|C,    imTiffWrite },

 

        { IMVFBINDEX8,  0,      IN,1,4,         LZW,     imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,4,         PB,      imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,4,         0,       imTiffWrite },



        { IMVFBINDEX8,  C,      IN,1,4,         C|LZW,   imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,4,         C|PB,    imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,4,         C,       imTiffWrite },



        { IMVFBINDEX8,  0,      IN,1,4,         PI|LZW,  imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,4,         PI|PB,   imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,4,         PI,      imTiffWrite },



        { IMVFBINDEX8,  C,      IN,1,4,         PI|C|LZW,imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,4,         PI|C|PB, imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,4,         PI|C,    imTiffWrite },



        { IMVFBINDEX8,  0,      IN,1,2,         LZW,     imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,2,         PB,      imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,2,         0,       imTiffWrite },



        { IMVFBINDEX8,  C,      IN,1,2,         C|LZW,   imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,2,         C|PB,    imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,2,         C,       imTiffWrite },



        { IMVFBINDEX8,  0,      IN,1,2,         PI|LZW,  imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,2,         PI|PB,   imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,2,         PI,      imTiffWrite },



        { IMVFBINDEX8,  C,      IN,1,2,         PI|C|LZW,imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,2,         PI|C|PB, imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,2,         PI|C,    imTiffWrite },



#if 0   /* Nobody seems capable of reading index16 tiffs.   */

        { IMVFBINDEX16, 0,      IN,1,16,        LZW,     imTiffWrite },

        { IMVFBINDEX16, 0,      IN,1,16,        PB,      imTiffWrite },

        { IMVFBINDEX16, 0,      IN,1,16,        0,       imTiffWrite },



        { IMVFBINDEX16, C,      IN,1,16,        C|LZW,   imTiffWrite },

        { IMVFBINDEX16, C,      IN,1,16,        C|PB,    imTiffWrite },

        { IMVFBINDEX16, C,      IN,1,16,        C,       imTiffWrite },



        { IMVFBINDEX16, 0,      IN,1,16,        PI|LZW,  imTiffWrite },

        { IMVFBINDEX16, 0,      IN,1,16,        PI|PB,   imTiffWrite },

        { IMVFBINDEX16, 0,      IN,1,16,        PI,      imTiffWrite },



        { IMVFBINDEX16, C,      IN,1,16,        PI|C|LZW,imTiffWrite },

        { IMVFBINDEX16, C,      IN,1,16,        PI|C|PB, imTiffWrite },

        { IMVFBINDEX16, C,      IN,1,16,        PI|C,    imTiffWrite },

#endif





        { IMVFBRGB,     0,      RGB,3,8,        LZW,     imTiffWrite },

        { IMVFBRGB,     0,      RGB,3,8,        PB,      imTiffWrite },

        { IMVFBRGB,     0,      RGB,3,8,        0,       imTiffWrite },



        { IMVFBRGB,     A,      RGB,3,8,        A|LZW,   imTiffWrite },

        { IMVFBRGB,     A,      RGB,3,8,        A|PB,    imTiffWrite },

        { IMVFBRGB,     A,      RGB,3,8,        A,       imTiffWrite },



        { IMVFBRGB,     0,      RGB,3,8,        PI|LZW,  imTiffWrite },

        { IMVFBRGB,     0,      RGB,3,8,        PI|PB,   imTiffWrite },

        { IMVFBRGB,     0,      RGB,3,8,        PI,      imTiffWrite },



        { IMVFBRGB,     A,      RGB,3,8,        PI|A|LZW,imTiffWrite },

        { IMVFBRGB,     A,      RGB,3,8,        PI|A|PB, imTiffWrite },

        { IMVFBRGB,     A,      RGB,3,8,        PI|A,    imTiffWrite },



/* tiles */



        { IMVFBINDEX8,  0,      IN,1,8,         T|LZW,     imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,8,         T|PB,      imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,8,         T|0,       imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,8,         T|DCT,     imTiffWrite },



        { IMVFBINDEX8,  C,      IN,1,8,         T|C|LZW,   imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,8,         T|C|PB,    imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,8,         T|C,       imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,8,         T|C|DCT,   imTiffWrite },



        { IMVFBINDEX8,  0,      IN,1,8,         T|PI|LZW,  imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,8,         T|PI|PB,   imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,8,         T|PI,      imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,8,         T|PI|DCT,  imTiffWrite },



        { IMVFBINDEX8,  C,      IN,1,8,         T|PI|C|LZW,imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,8,         T|PI|C|PB, imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,8,         T|PI|C,    imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,8,         T|PI|C|DCT,imTiffWrite },





        { IMVFBRGB,     0,      RGB,3,8,        T|LZW,     imTiffWrite },

        { IMVFBRGB,     0,      RGB,3,8,        T|PB,      imTiffWrite },

        { IMVFBRGB,     0,      RGB,3,8,        T,         imTiffWrite },

        { IMVFBRGB,     0,      RGB,3,8,        T|DCT,     imTiffWrite },



        { IMVFBRGB,     A,      RGB,3,8,        T|A|LZW,   imTiffWrite },

        { IMVFBRGB,     A,      RGB,3,8,        T|A|PB,    imTiffWrite },

        { IMVFBRGB,     A,      RGB,3,8,        T|A,       imTiffWrite },

        { IMVFBRGB,     A,      RGB,3,8,        T|A|DCT,   imTiffWrite },



        { IMVFBRGB,     0,      RGB,3,8,        T|PI|LZW,  imTiffWrite },

        { IMVFBRGB,     0,      RGB,3,8,        T|PI|PB,   imTiffWrite },

        { IMVFBRGB,     0,      RGB,3,8,        T|PI,      imTiffWrite },

        { IMVFBRGB,     0,      RGB,3,8,        T|PI|DCT,  imTiffWrite },



        { IMVFBRGB,     A,      RGB,3,8,        T|PI|A|LZW,imTiffWrite },

        { IMVFBRGB,     A,      RGB,3,8,        T|PI|A|PB, imTiffWrite },

        { IMVFBRGB,     A,      RGB,3,8,        T|PI|A,    imTiffWrite },

        { IMVFBRGB,     A,      RGB,3,8,        T|PI|A|DCT,imTiffWrite },

#else



        { IMVFBMONO,    0,      IN,1,1,         LZW,     imTiffWrite },

        { IMVFBMONO,    0,      IN,1,1,         PB,      imTiffWrite },

        { IMVFBMONO,    0,      IN,1,1,         0,       imTiffWrite },



        { IMVFBINDEX8,  0,      IN,1,8,         LZW,     imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,8,         PB,      imTiffWrite },

        { IMVFBINDEX8,  0,      IN,1,8,         0,       imTiffWrite },



        { IMVFBINDEX8,  C,      IN,1,8,         LZW|C,   imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,8,         PB|C,    imTiffWrite },

        { IMVFBINDEX8,  C,      IN,1,8,         C,       imTiffWrite },



        { IMVFBRGB,     A,      RGB,3,8,        LZW|A,   imTiffWrite },

        { IMVFBRGB,     A,      RGB,3,8,        PB|A,    imTiffWrite },

        { IMVFBRGB,     A,      RGB,3,8,        A,       imTiffWrite },



        { IMVFBRGB,     0,      RGB,3,8,        LZW,     imTiffWrite },

        { IMVFBRGB,     0,      RGB,3,8,        PB,      imTiffWrite },

        { IMVFBRGB,     0,      RGB,3,8,        0,       imTiffWrite },

#endif



        { -1,           0,      -1,             0,      NULL },

};



static ImFileMagic imFileTiffMagic []=

{

	{ 0, 2, imTiffMagicNumberA},

	{ 0, 2, imTiffMagicNumberB},

	{ 0, 0, NULL},

};



ImFileFormat ImFileTiffFormat =

{

	imTiffNames, "Tagged image file",

	"Aldus, MicroSoft, and NeXT",

	"1-, 4-, 8-, and 32-bit color index images.  24-bit RGB and 32-bit\n\
RGB+alpha images.  Standard (uncompressed), Mac Packbits, and Lempel-\n\
Ziv & Welsh compression.",
	"1-, 8-, and 32-bit color index images.  24-bit RGB and 32-bit\n\
RGB+alpha images.  Standard (uncompressed), Mac Packbits, and Lempel-\n\
Ziv & Welsh compression.",

	imFileTiffMagic,

#ifdef USE_TIFF_LIB

	IMMULTI, IMNOPIPE,  /* Read  */

	IMMULTI, IMNOPIPE,  /* Write */

#else

	IMNOMULTI, IMNOPIPE,

	IMNOMULTI, IMNOPIPE,

#endif

	imTiffRead, imTiffReadMap, imTiffWriteMap

};



#ifdef USE_TIFF_LIB



/*

 *  MACRO 

 *	IM_QCHANNEL( value, channel)

 *

 * DESCRIPTION

 *	Set a value to be this attribute of this pixel

 *

 */



#define IM_QCHANNEL(value, channel)						\
switch (channel)								\
{										\
	case IMVFBGREY :  /* same as INDEX8 */					\
		value = (uint16) ImVfbQGrey (vfb, vfbptr);			\
		break;								\
	case IMVFBRED  :							\
		value = (uint16) ImVfbQRed  (vfb, vfbptr);			\
		break;								\
	case IMVFBGREEN :							\
		value = (uint16) ImVfbQGreen(vfb, vfbptr);			\
		break;								\
	case IMVFBBLUE  :							\
		value = (uint16) ImVfbQBlue (vfb, vfbptr);			\
		break;								\
	case IMVFBALPHA :							\
		value = (uint16) ImVfbQAlpha (vfb, vfbptr);			\
		break;								\
	case IMVFBINDEX16 :							\
		value = (uint16) ImVfbQIndex16 (vfb, vfbptr);			\
		break;								\
}



#define IMTIFFNULL	NULL

#define IM_MIN(x,y) 	( (x) < (y) ? (x) : (y) )



#ifdef __STDC__



static int imTiffReadNonInterleavedScanlines( TIFF* tiff, ImVfb* vfb);

static int imTiffReadPlaneInterleavedScanlines( TIFF *tif, ImVfb* vfb); 

static int imTiffReadNonInterleavedTiles (TIFF *tif, ImVfb *vfb);

static int imTiffReadPlaneInterleavedTiles ( TIFF *tif, ImVfb *vfb);

static int imTiffReadClt(TIFF* tif, ImClt **cltBuf);



static int imTiffReadRedScanlinePlane(TIFF* tif, ImVfb* vfb);

static int imTiffReadGreenScanlinePlane(TIFF* tif, ImVfb* vfb);

static int imTiffReadBlueScanlinePlane(TIFF* tif, ImVfb* vfb);

static int imTiffReadAlphaScanlinePlane(TIFF* tif, ImVfb* vfb);



static int imTiffReadRedTiledPlane( TIFF* tif, ImVfb* vfb, uint32 tileWidth, 

		uint32 tileLength, uint32 imageWidth, uint32 imageLength, tsize_t tileSize);

static int imTiffReadGreenTiledPlane( TIFF* tif, ImVfb* vfb, uint32 tileWidth, 

		uint32 tileLength, uint32 imageWidth, uint32 imageLength, tsize_t tileSize);

static int imTiffReadBlueTiledPlane( TIFF* tif, ImVfb* vfb, uint32 tileWidth, 

		uint32 tileLength, uint32 imageWidth, uint32 imageLength, tsize_t tileSize);

static int imTiffReadAlphaTiledPlane( TIFF* tif, ImVfb* vfb, uint32 tileWidth, 

		uint32 tileLength, uint32 imageWidth, uint32 imageLength, tsize_t tileSize);



static int imTiffWriteNonInterleavedScanlines( TIFF* tiff, ImVfb* vfb, ImFileFormatWriteMap* pMap);

static int imTiffWritePlaneInterleavedScanlines( TIFF *tif, ImVfb* vfb, ImFileFormatWriteMap* pMap); 

static int imTiffWriteNonInterleavedTiles (TIFF *tif, ImVfb *vfb, ImFileFormatWriteMap* pMap );

static int imTiffWritePlaneInterleavedTiles ( TIFF *tif, ImVfb *vfb, ImFileFormatWriteMap* pMap);



static int imTiffWriteClt(TIFF* tif, ImClt *cltBuf, long numEntries);

static int imTiffWriteRedScanlinePlane(ImVfb *vfb, TIFF *tif, uint16 chandepth);

static int imTiffWriteGreenScanlinePlane(ImVfb *vfb, TIFF *tif, uint16 chandepth);

static int imTiffWriteBlueScanlinePlane(ImVfb *vfb, TIFF *tif, uint16 chandepth);

static int imTiffWriteAlphaScanlinePlane(ImVfb *vfb, TIFF *tif, uint16 chandepth);

static int imTiffWriteIndex8ScanlinePlane(ImVfb *vfb, TIFF *tif, uint16 chandepth);

static int imTiffWriteIndex16ScanlinePlane(ImVfb *vfb, TIFF *tif, uint16 chandepth);



static int imTiffWriteRedTiledPlane(ImVfb* vfb, TIFF *tif, uint16 chandepth, uint32 tileWidth, uint32 tileHeight);

static int imTiffWriteGreenTiledPlane( ImVfb* vfb, TIFF* tif, uint16 chanDepth, uint32 tileWidth, uint32 tileHeight);

static int imTiffWriteBlueTiledPlane( ImVfb* vfb, TIFF* tif, uint16 chanDepth, uint32 tileWidth, uint32 tileHeight);

static int imTiffWriteAlphaTiledPlane( ImVfb* vfb, TIFF* tif, uint16 chanDepth, uint32 tileWidth, uint32 tileHeight);

static int imTiffWriteIndex8TiledPlane( ImVfb* vfb, TIFF* tif, uint16 chanDepth, uint32 tileWidth, uint32 tileHeight);



static void imTiffSwapBytes(unsigned char* linebuf, tsize_t linesize);

#else	/* __STDC__ is not defined */



static int imTiffReadNonInterleavedScanlines( );

static int imTiffReadPlaneInterleavedScanlines(); 

static int imTiffReadNonInterleavedTiles ( );

static int imTiffReadPlaneInterleavedTiles ( );

static int imTiffReadClt();



static int imTiffReadRedScanlinePlane();

static int imTiffReadGreenScanlinePlane();

static int imTiffReadBlueScanlinePlane();

static int imTiffReadAlphaScanlinePlane();



static int imTiffReadRedTiledPlane( );

static int imTiffReadGreenTiledPlane( );

static int imTiffReadBlueTiledPlane( );

static int imTiffReadAlphaTiledPlane( );



static int imTiffWriteNonInterleavedScanlines();

static int imTiffWritePlaneInterleavedScanlines( ); 

static int imTiffWriteNonInterleavedTiles ();

static int imTiffWritePlaneInterleavedTiles ( );

static int imTiffWriteClt();

static int imTiffWriteRedScanlinePlane();

static int imTiffWriteGreenScanlinePlane();

static int imTiffWriteBlueScanlinePlane();

static int imTiffWriteAlphaScanlinePlane();

static int imTiffWriteIndex8ScanlinePlane();

static int imTiffWriteIndex16ScanlinePlane();



static int imTiffWriteRedTiledPlane();

static int imTiffWriteGreenTiledPlane();

static int imTiffWriteBluedTiledPlane();

static int imTiffWriteAlphadTiledPlane();

static int imTiffWriteIndex8TiledPlane();



static void imTiffSwapBytes();

#endif		/* __STDC__ */



#endif		/* USE_TIFF_LIB  */



/*

 *

 * Code to read tiff files without using the tiff library starts here.

 *

 */



#ifndef USE_TIFF_LIB



/*

 * Prototypes

 *   These functions are in imlzw.c or macpack.c

 */



#ifdef __STDC__

int ImLzwPreDecode( unsigned char *buf, int size );

int ImLzwDecode(unsigned char *buf, char *op, int occ);

int ImLzwPostDecode();

int ImLzwPreEncode(int size);

int ImLzwEncode(int ioType, int fd, FILE *fp, unsigned char *bp, int cc);

int ImLzwPostEncode( int ioType, int fd, FILE *fp );

void ImLzwCleanup();

void UnpackBits( unsigned char *srcPtr, unsigned char *dstPtr, unsigned int *cnt );

void PackBits3( unsigned char *cIn, unsigned char *cOut, unsigned int * width );

void PackBits( unsigned char *cIn, unsigned char *cOut, unsigned int *width );





#else

int ImLzwPreDecode( );

int ImLzwDecode();

int ImLzwPostDecode();

int ImLzwPreEncode();

int ImLzwEncode();

int ImLzwPostEncode( );

void ImLzwCleanup();

void UnpackBits( );

void PackBits3( );

void PackBits( );

#endif



/*

 *  TYPEDEF & STRUCTURE

 *	imTiffHeaderInfo	-  Rasterfile header information

 *	imTiffHeaderFields	-  imTiffHeaderInfo description for Bin pkg

 *	imTiffDirEntry		-  An Image File Directory entry

 *	imTiffDirFields		-  imTiffDirEntry description for Bin pkg

 *	imTiffDataWidth		-  An array of values indicating data size

 *	imTiffMask		-  A bit mask corresponding to data size

 *	imTiffDirData		-  Program internal representation of dir data

 *	imTiffDirInfo		-  Program internal representation of directory

 *	imTmpFile 		-  A filename format for a temporary file.

 *

 *  DESCRIPTION

 *	A TIFF file header contains a magic number, the version number,

 *	and an offset to the first directory entry.  Directories are

 *	variable length.  The number of dir entries is in the short preceeding

 *	the first directory entry.  The offset of the next directory in

 *	the file is in the 4 bytes after the last directory entry.  Most

 *	directory entries contain a tag, type, size, and a single value.

 *	Other entries contain a tag, type, size, and offset to an array of

 *	values (or a single double that would not fit in the value field).

 *

 *  NOTE:

 *	TIFF's tiff.h is **NOT** directly included here!

 *	Therefore we recreate the essentials ourselves.

 */



typedef struct imTiffHeaderInfo

{

	unsigned short tiff_magic;		/* Magic number (byte order)	*/

	unsigned short tiff_version;		/* TIFF version number		*/

	unsigned int   tiff_diroff;		/* byte offset to first dir 	*/

} imTiffHeaderInfo;





static BinField imTiffHeaderFields[ ] =

{

	{ USHORT, 2, 1 },		/* tiff_magic			*/

	{ USHORT, 2, 1 },		/* tiff_version			*/

	{ UINT, 4, 1 },			/* tiff_diroff			*/

	{ 0, 0, 0 },

};





typedef struct imTiffDirEntry

{

	unsigned short tdir_tag;		/* Tagged file format type	*/

	unsigned short tdir_type;		/* Data type			*/

	unsigned int tdir_count;		/* Number of items; length 	*/

	unsigned int tdir_offset;		/* byte offset to field data 	*/

					/* Last field is read separately*/

} imTiffDirEntry;



/*

 * Read the tdir_offset separately depending on whether the

 * type indicates it contains, bytes, shorts, or a long.

 */

static BinField imTiffDirFields[ ] =

{

	{ USHORT, 2, 1 },		/* tdir_tag			*/

	{ USHORT, 2, 1 },		/* tdir_type			*/

	{ UINT, 4, 1 },			/* tdir_count			*/

	{ 0, 0, 0 },

};



 

/*

 * These are the sizes of various datatypes as they exist in tiff files.

 * Always.  We have to remember that in memory they can be wildly different.

 */

static int imTiffDataWidth[] = {

    1,  				/* Nothing 			*/

    1,  				/* Byte 			*/

    1,  				/* ASCII 			*/

    2,  				/* Short 			*/

    4,  				/* Long 			*/

    8,  				/* Rational 			*/

};



static int imTiffMask[]=

{

	0xff, 				/* Nothing			*/

	0xff, 				/* Byte				*/

	0xff, 				/* ASCII			*/

	0xffff,				/* Short			*/

	0xffffffff,			/* Long				*/

};





typedef struct imTiffDirData

{	

	int	type;			/* Byte, short, or long		*/

	int	cnt;			/* How many of them		*/

	unsigned char  *data;			/* The actual array of values 	*/

} imTiffDirData;





typedef struct imTiffDirInfo

{

	unsigned int 	 t_width;		/* Image width			*/

	unsigned int 	 t_height;		/* Image height			*/

	unsigned short 	 t_depth;		/* Image depth in bits		*/

	unsigned int 	 t_compression;		/* Image compression scheme	*/

	unsigned int 	 t_photometric;		/* Photometric interpretation	*/

	unsigned int 	 t_orientation;		/* Image orientation		*/

	unsigned int 	 t_rowsperstrip;	/* Image rows/strip of data	*/

	unsigned int 	 t_samplesperpixel;	/* Image samples/pixel		*/

	unsigned int	 t_planarconfig;	/* Storage type			*/

	imTiffDirData   t_xposition;	/* Image x position		*/

	imTiffDirData   t_yposition;	/* Image y position		*/

	imTiffDirData 	t_bitspersample;/* Image depth in bits		*/

	imTiffDirData 	t_stripoffsets;	/* Image bytes counts for strips*/

	imTiffDirData 	t_stripbytecounts; /* Image bytes counts for strips*/

	imTiffDirData 	t_colormap;	/* RGB map for pallette image	*/

	unsigned char 	*t_imagedescr;		/* Image description		*/

	unsigned char 	*t_software;		/* Software used to create	*/

	unsigned char 	*t_datetime;		/* Date/time created		*/

	unsigned char 	*t_artist;		/* Creator			*/

	unsigned char 	*t_host;		/* Host computer used		*/

	unsigned char 	*t_whitepoint;		/* Image white point		*/

	unsigned char 	*t_primarychrom;	/* Primary chromaticities	*/

	unsigned int 	 t_predictor;		/* Predict pixel differences 	*/ 

	unsigned int 	 t_matteing;		/* Alpha channel is present  ?  */ 

} imTiffDirInfo;





static imTiffHeaderInfo imTiffHeader;	/* TIFF file header		*/







/*

 *  CONSTANTS

 *	IMTIFFBIGENDIANMAGIC	-  file magic number

 *	IMTIFFLITTLEENDIANMAGIC	-  escape code for run-length encoding

 *	IMTIFFVERSION		-  TIFF version number

 *	IMTIFFBYTESPERWORD	-  Number of bytes in a word

 *	IMTIFFSHORTSPERWORD	-  Number of shorts in a word

 *	IMTIFFBITSPERBYTE	-  Number of bits in a byte

 *	IMTIFFNIBBLESPERBYTE	-  Number of nibbles in a byte

 *	IMTIFFNIBBLESMULT	-  Multiply by 16 to fill out to 8 bits

 *	IMTIFFBITMULT		-  Multiply by 255 to fill out to 8 bits

 *	IMTIFF(type)		-  enumerated types

 *

 *  DESCRIPTION

 *	IMTIFFBIGENDIANMAGIC and IMTIFFLITTLEENDIANMAGIC are 

 *	the 32-bit magic numbers at the top of all tiff files.

 *	The former indicates MBF byte ordering and the latter indicates

 *	LBF byte ordering.

 *

 *	IMTIFFVERSION is always 42.

 *

 *	The enumerated types are found in the directory type field to

 *	describe what is to be found in the offset field or whereever the

 *	offset points to.

 *	

 */



#define IMTIFFBIGENDIANMAGIC	0x4d4d

#define IMTIFFLITTLEENDIANMAGIC	0x4949

#define IMTIFFVERSION		(42)

#define IMTIFFBITSPERBYTE	(8)

#define IMTIFFBITSPERLONG	(32)

#define IMTIFFBYTESPERWORD	(4)

#define IMTIFFSHORTSPERWORD	(2)

#define IMTIFFNIBBLESPERBYTE	(2)

#define IMTIFFNIBBLEMULT	(16)



#define IMTIFFBYTE	(1)

#define IMTIFFASCII	(2)

#define IMTIFFSHORT	(3)

#define IMTIFFLONG	(4)

#define IMTIFFRATIONAL	(5)



/*

#define DEBUG

*/











/*

 *  CONSTANTS

 *	NULL		-  an empty pointer

 *

 *  DESCRIPTION

 *	NULL might not be defined by the include files, but we need it.

 */



#ifndef NULL

#define NULL		(0)

#endif





/*

 *  CONSTANTS

 * 	TIFF Tag Definitions.

 *

 *  DESCRIPTIONS

 *	These are straight out of tiff.h (with an IM_added )

 * 	Those marked with a + are obsoleted by revision 5.0

 */



#define IM_TIFFTAG_SUBFILETYPE             254     /* subfile data descriptor */

#define     FILETYPE_REDUCEDIMAGE       0x1     /* reduced resolution version */

#define     FILETYPE_PAGE               0x2     /* one page of many */

#define     FILETYPE_MASK               0x4     /* transparency mask */

#define IM_TIFFTAG_OSUBFILETYPE            255     /* +kind of data in subfile */

#define     OFILETYPE_IMAGE             1       /* full resolution image data */

#define     OFILETYPE_REDUCEDIMAGE      2       /* reduced size image data */

#define     OFILETYPE_PAGE              3       /* one page of many */

#define IM_TIFFTAG_IMAGEWIDTH              256     /* image width in pixels */

#define IM_TIFFTAG_IMAGELENGTH             257     /* image height in pixels */

#define IM_TIFFTAG_BITSPERSAMPLE           258     /* bits per channel (sample) */

#define IM_TIFFTAG_COMPRESSION             259     /* data compression technique */

#define     IM_COMPRESSION_NONE            1       /* dump mode */

#define     IM_COMPRESSION_CCITTRLE        2       /* CCITT modified Huffman RLE */

#define     IM_COMPRESSION_CCITTFAX3       3       /* CCITT Group 3 fax encoding */

#define     IM_COMPRESSION_CCITTFAX4       4       /* CCITT Group 4 fax encoding */

#define     IM_COMPRESSION_LZW             5       /* Lempel-Ziv  & Welch */

#define     IM_COMPRESSION_NEXT            32766   /* NeXT 2-bit RLE */

#define     IM_COMPRESSION_CCITTRLEW       32771   /* #1 w/ word alignment */

#define     IM_COMPRESSION_PACKBITS        32773   /* Macintosh RLE */

#define     IM_COMPRESSION_THUNDERSCAN     32809   /* ThunderScan RLE */

#define     IM_COMPRESSION_PICIO           32900   /* old Pixar picio RLE */

#define     IM_COMPRESSION_SGIRLE          32901   /* Silicon Graphics RLE */

#define IM_TIFFTAG_PHOTOMETRIC             262     /* photometric interpretation */

#define     IM_PHOTOMETRIC_MINISWHITE      0       /* min value is white */

#define     IM_PHOTOMETRIC_MINISBLACK      1       /* min value is black */

#define     IM_PHOTOMETRIC_RGB             2       /* RGB color model */

#define     IM_PHOTOMETRIC_PALETTE         3       /* color map indexed */

#define     IM_PHOTOMETRIC_MASK            4       /* holdout mask */

#define     IM_PHOTOMETRIC_DEPTH           32768   /* z-depth data */

#define IM_TIFFTAG_THRESHHOLDING           263     /* +thresholding used on data */

#define     IM_THRESHHOLD_BILEVEL          1       /* b&w art scan */

#define     IM_THRESHHOLD_HALFTONE         2       /* or dithered scan */

#define     IM_THRESHHOLD_ERRORDIFFUSE     3       /* usually floyd-steinberg */

#define IM_TIFFTAG_CELLWIDTH               264     /* +dithering matrix width */

#define IM_TIFFTAG_CELLLENGTH              265     /* +dithering matrix height */

#define IM_TIFFTAG_FILLORDER               266     /* +data order within a byte */

#define     IM_FILLORDER_MSB2LSB           1       /* most significant -> least */

#define     IM_FILLORDER_LSB2MSB           2       /* least significant -> most */

#define IM_TIFFTAG_DOCUMENTNAME            269     /* name of doc. image is from */

#define IM_TIFFTAG_IMAGEDESCRIPTION        270     /* info about image */

#define IM_TIFFTAG_MAKE                    271     /* scanner manufacturer name */

#define IM_TIFFTAG_MODEL                   272     /* scanner model name/number */

#define IM_TIFFTAG_STRIPOFFSETS            273     /* offsets to data strips */

#define IM_TIFFTAG_ORIENTATION             274     /* +image orientation */

#define     IM_ORIENTATION_TOPLEFT         1       /* row 0 top, col 0 lhs */

#define     IM_ORIENTATION_TOPRIGHT        2       /* row 0 top, col 0 rhs */

#define     IM_ORIENTATION_BOTRIGHT        3       /* row 0 bottom, col 0 rhs */

#define     IM_ORIENTATION_BOTLEFT         4       /* row 0 bottom, col 0 lhs */

#define     IM_ORIENTATION_LEFTTOP         5       /* row 0 lhs, col 0 top */

#define     IM_ORIENTATION_RIGHTTOP        6       /* row 0 rhs, col 0 top */

#define     IM_ORIENTATION_RIGHTBOT        7       /* row 0 rhs, col 0 bottom */

#define     IM_ORIENTATION_LEFTBOT         8       /* row 0 lhs, col 0 bottom */

#define IM_TIFFTAG_SAMPLESPERPIXEL         277     /* samples per pixel */

#define IM_TIFFTAG_ROWSPERSTRIP            278     /* rows per strip of data */

#define IM_TIFFTAG_STRIPBYTECOUNTS         279     /* bytes counts for strips */

#define IM_TIFFTAG_MINSAMPLEVALUE          280     /* +minimum sample value */

#define IM_TIFFTAG_MAXSAMPLEVALUE          281     /* maximum sample value */

#define IM_TIFFTAG_XRESOLUTION             282     /* pixels/resolution in x */

#define IM_TIFFTAG_YRESOLUTION             283     /* pixels/resolution in y */

#define IM_TIFFTAG_PLANARCONFIG            284     /* storage organization */

#define     IM_PLANARCONFIG_CONTIG         1       /* single image plane */

#define     IM_PLANARCONFIG_SEPARATE       2       /* separate planes of data */

#define IM_TIFFTAG_PAGENAME                285     /* page name image is from */

#define IM_TIFFTAG_XPOSITION               286     /* x page offset of image lhs */

#define IM_TIFFTAG_YPOSITION               287     /* y page offset of image lhs */

#define IM_TIFFTAG_FREEOFFSETS             288     /* +byte offset to free block */

#define IM_TIFFTAG_FREEBYTECOUNTS          289     /* +sizes of free blocks */

#define IM_TIFFTAG_GRAYRESPONSEUNIT        290     /* gray scale curve accuracy */

#define     IM_GRAYRESPONSEUNIT_10S        1       /* tenths of a unit */

#define     IM_GRAYRESPONSEUNIT_100S       2       /* hundredths of a unit */

#define     IM_GRAYRESPONSEUNIT_1000S      3       /* thousandths of a unit */

#define     IM_GRAYRESPONSEUNIT_10000S     4       /* ten-thousandths of a unit */

#define     IM_GRAYRESPONSEUNIT_100000S    5       /* hundred-thousandths */

#define IM_TIFFTAG_GRAYRESPONSECURVE       291     /* gray scale response curve */

#define IM_TIFFTAG_GROUP3OPTIONS           292     /* 32 flag bits */

#define     IM_GROUP3OPT_2DENCODING        0x1     /* 2-dimensional coding */

#define     IM_GROUP3OPT_UNCOMPRESSED      0x2     /* data not compressed */

#define     IM_GROUP3OPT_FILLBITS          0x4     /* fill to byte boundary */

#define IM_TIFFTAG_GROUP4OPTIONS           293     /* 32 flag bits */

#define     IM_GROUP4OPT_UNCOMPRESSED      0x2     /* data not compressed */

#define IM_TIFFTAG_RESOLUTIONUNIT          296     /* units of resolutions */

#define     IM_RESUNIT_NONE                1       /* no meaningful units */

#define     IM_RESUNIT_INCH                2       /* english */

#define     IM_RESUNIT_CENTIMETER          3       /* metric */

#define IM_TIFFTAG_PAGENUMBER              297     /* page numbers of multi-page */

#define IM_TIFFTAG_COLORRESPONSEUNIT       300     /* color scale curve accuracy */

#define     IM_COLORRESPONSEUNIT_10S       1       /* tenths of a unit */

#define     IM_COLORRESPONSEUNIT_100S      2       /* hundredths of a unit */

#define     IM_COLORRESPONSEUNIT_1000S     3       /* thousandths of a unit */

#define     IM_COLORRESPONSEUNIT_10000S    4       /* ten-thousandths of a unit */

#define     IM_COLORRESPONSEUNIT_100000S   5       /* hundred-thousandths */

#define IM_TIFFTAG_COLORRESPONSECURVE      301     /* RGB response curve */

#define IM_TIFFTAG_SOFTWARE                305     /* name & release */

#define IM_TIFFTAG_DATETIME                306     /* creation date and time */

#define IM_TIFFTAG_ARTIST                  315     /* creator of image */

#define IM_TIFFTAG_HOSTCOMPUTER            316     /* machine where created */

#define IM_TIFFTAG_PREDICTOR               317     /* prediction scheme w/ LZW */

#define IM_TIFFTAG_WHITEPOINT              318     /* image white point */

#define IM_TIFFTAG_PRIMARYCHROMATICITIES   319     /* primary chromaticities */

#define IM_TIFFTAG_COLORMAP                320     /* RGB map for pallette image */

#define IM_TIFFTAG_BADFAXLINES             326     /* lines w/ wrong pixel count */

#define IM_TIFFTAG_CLEANFAXDATA            327     /* regenerated line info */

#define      IM_CLEANFAXDATA_CLEAN         0       /* no errors detected */

#define      IM_CLEANFAXDATA_REGENERATED   1       /* receiver regenerated lines */

#define      IM_CLEANFAXDATA_UNCLEAN       2       /* uncorrected errors exist */

#define IM_TIFFTAG_CONSECUTIVEBADFAXLINES  328     /* max consecutive bad lines */

/* tags 32995-32999 are private tags registered to SGI */

#define IM_TIFFTAG_MATTEING                32995   /* alpha channel is present */

#define IM_TIFFTAG_SGICOLORMAP             32996   /* SGI colormap indicator */





/*

 *  STRUCTURE

 *	tagnames - an array of the names of the tages

 *

 *  DESCRIPTION

 *	An array of tagnames so that we can print and debug easily.

 */



static   

struct ImTiffTagNames {

        int     tag;

        char    *name;

} ImTiffTagNames[] = {

    { IM_TIFFTAG_SUBFILETYPE,      "SubFileType" },

    { IM_TIFFTAG_OSUBFILETYPE,     "OldSubFileType" },

    { IM_TIFFTAG_IMAGEWIDTH,       "ImageWidth" },

    { IM_TIFFTAG_IMAGELENGTH,      "ImageLength" },

    { IM_TIFFTAG_BITSPERSAMPLE,    "BitsPerSample" },

    { IM_TIFFTAG_COMPRESSION,      "Compression" },

    { IM_TIFFTAG_PHOTOMETRIC,      "Photometric" },

    { IM_TIFFTAG_THRESHHOLDING,    "Threshholding" },

    { IM_TIFFTAG_CELLWIDTH,        "CellWidth" },

    { IM_TIFFTAG_CELLLENGTH,       "CellLength" },

    { IM_TIFFTAG_FILLORDER,        "FillOrder" },

    { IM_TIFFTAG_DOCUMENTNAME,     "DocumentName" },

    { IM_TIFFTAG_IMAGEDESCRIPTION, "ImageDescription" },

    { IM_TIFFTAG_MAKE,             "Make" },

    { IM_TIFFTAG_MODEL,            "Model" },

    { IM_TIFFTAG_STRIPOFFSETS,     "StripOffsets" },

    { IM_TIFFTAG_ORIENTATION,      "Orientation" },

    { IM_TIFFTAG_SAMPLESPERPIXEL,  "SamplesPerPixel" },

    { IM_TIFFTAG_ROWSPERSTRIP,     "RowsPerStrip" },

    { IM_TIFFTAG_STRIPBYTECOUNTS,  "StripByteCounts" },

    { IM_TIFFTAG_MINSAMPLEVALUE,   "MinSampleValue" },

    { IM_TIFFTAG_MAXSAMPLEVALUE,   "MaxSampleValue" },

    { IM_TIFFTAG_XRESOLUTION,      "XResolution" },

    { IM_TIFFTAG_YRESOLUTION,      "YResolution" },

    { IM_TIFFTAG_PLANARCONFIG,     "PlanarConfig" },

    { IM_TIFFTAG_PAGENAME,         "PageName" },

    { IM_TIFFTAG_XPOSITION,        "XPosition" },

    { IM_TIFFTAG_YPOSITION,        "YPosition" },

    { IM_TIFFTAG_FREEOFFSETS,      "FreeOffsets" },

    { IM_TIFFTAG_FREEBYTECOUNTS,   "FreeByteCounts" },

    { IM_TIFFTAG_GRAYRESPONSEUNIT, "GrayResponseUnit" },

    { IM_TIFFTAG_GRAYRESPONSECURVE,"GrayResponseCurve" },

    { IM_TIFFTAG_GROUP3OPTIONS,    "Group3Options" },

    { IM_TIFFTAG_GROUP4OPTIONS,    "Group4Options" },

    { IM_TIFFTAG_RESOLUTIONUNIT,   "ResolutionUnit" },

    { IM_TIFFTAG_PAGENUMBER,       "PageNumber" },

    { IM_TIFFTAG_COLORRESPONSEUNIT,"ColorResponseUnit" },

    { IM_TIFFTAG_COLORRESPONSECURVE,"ColorResponseCurve" },

    { IM_TIFFTAG_SOFTWARE,         "Software" },

    { IM_TIFFTAG_DATETIME,         "DateTime" },

    { IM_TIFFTAG_ARTIST,           "Artist" },

    { IM_TIFFTAG_HOSTCOMPUTER,     "HostComputer" },

    { IM_TIFFTAG_PREDICTOR,        "Predictor" },

    { IM_TIFFTAG_WHITEPOINT,       "Whitepoint" },

    { IM_TIFFTAG_PRIMARYCHROMATICITIES,"PrimaryChromaticities" },

    { IM_TIFFTAG_COLORMAP,         "Colormap" },

    { IM_TIFFTAG_BADFAXLINES,      "Bad FAX Lines" },

    { IM_TIFFTAG_CLEANFAXDATA,     "Clean Fax Data" },

    { IM_TIFFTAG_CONSECUTIVEBADFAXLINES,     "Consecutive bad fax lines" },

    { IM_TIFFTAG_SGICOLORMAP,      "SGIColormap" },

    { 32768,                    "OLD BOGUS Matteing tag" },

    { IM_TIFFTAG_MATTEING,         "Matteing" },

};



#define IM_NTAGS   (sizeof (ImTiffTagNames) / sizeof (ImTiffTagNames[0]))



#define IMTIFF_IMAGEWIDTH_INDEX		0

#define IMTIFF_IMAGELENGTH_INDEX	1

#define IMTIFF_BITSPERSAMPLE_INDEX	2

#define IMTIFF_COMPRESSION_INDEX	3

#define IMTIFF_PHOTOMETRIC_INDEX	4

#define IMTIFF_STRIPOFFSETS_INDEX	5

#define IMTIFF_ORIENTATION_INDEX	6

#define IMTIFF_SAMPLESPERPIXEL_INDEX	7

#define IMTIFF_ROWSPERSTRIP_INDEX	8

#define IMTIFF_STRIPBYTECOUNTS_INDEX	9

#define IMTIFF_PLANARCONFIG_INDEX	10

#define IMTIFF_XPOSITION_INDEX		11 

#define IMTIFF_YPOSITION_INDEX		12

#define IMTIFF_MATTEING_INDEX		13

#define IMTIFF_COLORMAP_INDEX		14	/* Alway last ! */



/*

 * By default we write out a 1 bit deep file with 1 sample per pixel.

 * No compression and origin in the topleft corner.

 *

 * Note: These must be ordered by tag.

 */

static imTiffDirEntry imTiffWriteDir[] =

{

	{ IM_TIFFTAG_IMAGEWIDTH, 		IMTIFFSHORT, 1, 0 },

	{ IM_TIFFTAG_IMAGELENGTH, 		IMTIFFSHORT, 1, 0 },

	{ IM_TIFFTAG_BITSPERSAMPLE, 	IMTIFFSHORT, 1, (8 << 16) },

	{ IM_TIFFTAG_COMPRESSION, 	IMTIFFSHORT, 1, (IM_COMPRESSION_NONE << 16) },

	{ IM_TIFFTAG_PHOTOMETRIC, 	IMTIFFSHORT, 1, (IM_PHOTOMETRIC_PALETTE << 16) },

	{ IM_TIFFTAG_STRIPOFFSETS, IMTIFFLONG, 1, 0 },

	{ IM_TIFFTAG_ORIENTATION,	IMTIFFSHORT, 1, (IM_ORIENTATION_TOPLEFT << 16) },

	{ IM_TIFFTAG_SAMPLESPERPIXEL, 	IMTIFFSHORT, 1, 1 },

	{ IM_TIFFTAG_ROWSPERSTRIP, 	IMTIFFLONG, 1, 1 },

	{ IM_TIFFTAG_STRIPBYTECOUNTS, 	IMTIFFLONG, 1, 0 },

	{ IM_TIFFTAG_PLANARCONFIG,	IMTIFFSHORT,1, (IM_PLANARCONFIG_CONTIG << 16) },

	{ IM_TIFFTAG_XPOSITION,	IMTIFFRATIONAL, 1, 0 },

	{ IM_TIFFTAG_YPOSITION,	IMTIFFRATIONAL, 1, 0 },

	{ IM_TIFFTAG_MATTEING,	IMTIFFSHORT, 1, 0 },

	{ IM_TIFFTAG_COLORMAP,	IMTIFFSHORT, 768, 0 },

};



#define IMTIFFWRITEDIRENTRIES	sizeof(imTiffWriteDir)/sizeof(imTiffDirEntry)



#ifdef __STDC__

static int imTiffReadDirectory( int ioType, int fd, FILE *fp, unsigned int *dirOffset, imTiffDirInfo *imTDI );

static int imTiffRead16( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imTiffDirInfo *imTDI, 

	ImVfb **pVfb );

static int imTiffRead24Cont( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imTiffDirInfo *imTDI, 

	ImVfb **pVfb );

static int imTiffRead24Sep( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imTiffDirInfo *imTDI, 

	ImVfb **pVfb );

static int imTiffRead8( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imTiffDirInfo *imTDI, ImVfb **pVfb );

static int imTiffRead4( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imTiffDirInfo *imTDI, ImVfb **pVfb );

static int imTiffRead1( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imTiffDirInfo *imTDI, ImVfb **pVfb );

static int imTiffGetClt( imTiffDirInfo *imTDI, ImClt **pClt );

static int imTiffVfbWrite( int ioType, int fd, FILE *fp, TagTable *flagsTable, ImVfb *vfb );

#else

static int imTiffReadDirectory( );

static int imTiffRead16( );

static int imTiffRead24Cont( );

static int imTiffRead24Sep( );

static int imTiffRead8( );

static int imTiffRead4( );

static int imTiffRead1( );

static int imTiffGetClt( );

static int imTiffVfbWrite( );

#endif



/*

 *  FUNCTION

 *	imPrintTiffTag	-  print the name of tag

 *

 *  DESCRIPTION

 *	Given a tag value, seach through the table until the entry with

 *	the matching tag is found.  Print the string in the second field.

 */



static void			/* Doesn't return a value 	*/

#ifdef __STDC__

imPrintTiffTag(unsigned short tag)

#else

imPrintTiffTag(tag)

        unsigned short tag;		/* The tag number 		*/

#endif  /* __STDC__ */

{

        struct ImTiffTagNames *tp;

	char message[1024];



        for (tp = ImTiffTagNames; tp < &ImTiffTagNames[IM_NTAGS]; tp++)

	{

                if (tp->tag == tag) 

		{

                        sprintf(message, "%s (%d) \n", tp->name, tag);

			ImErrorWarning(message,IM_NOTHING,IMEUNSUPPORTED);

                        return;

                }

	}

        sprintf(message, "%d (0x%x) Tag Not Found \n", tag, tag);

	ImErrorWarning(message,IM_NOTHING,IMEUNSUPPORTED);

}







#ifdef DEBUG



/*

 *  FUNCTION

 *	imPrintTiffDirLongEntry	-  Print the fields in a data entry

 *	imPrintTiffDirInfo	-  Print the fields in a Tiff directory

 *

 *  DESCRIPTION

 *	Print the fields from the internal tiff directory structure in

 *	human readable form.

 *	This is strictly for debugging.

 *

 *	WARNING: This routine assumes that a double is 8 bytes.

 */



static void

#ifdef __STDC__

imPrintTiffDirLongEntry( imTiffDirData *imTD, char *str )

#else

imPrintTiffDirLongEntry( imTD, str )

	imTiffDirData  	*imTD;		/* The internal directory info */

	char		*str;

#endif

{

	int    i; 

	int    val;



	fprintf(stderr,"%s: type %d   count %d   data %d \n <",

		str, imTD->type, imTD->cnt, imTD->data );



	if ( imTD->data == NULL )

	{

		fprintf(stderr," > \n");

		return;

	}



	if ( imTD->type == IMTIFFASCII )

	{

		fprintf(stderr,"%s > \n",imTD->data );

		return;

	}



	for( i=0; i<imTD->cnt; i++)

	{

		val = imTiffDataItem( imTD, i );

		fprintf(stderr,"%d ",val);

	}

	fprintf(stderr,"> \n ");

}





static void

#ifdef __STDC__

imPrintTiffDirInfo( imTiffDirInfo * imTI )

#else

imPrintTiffDirInfo( imTI )

	imTiffDirInfo* 	imTI;		/* The internal directory info */

#endif

{

	fprintf(stderr,"Tiff Directory Info \n");

	fprintf(stderr,"------------------- \n");

	fprintf(stderr,"width : %d \n",	imTI->t_width);

	fprintf(stderr,"heigt : %d \n",	imTI->t_height);

	fprintf(stderr,"depth : %d \n",	imTI->t_depth);

	fprintf(stderr,"compr : %d \n",	imTI->t_compression);

	fprintf(stderr,"photo : %d \n",	imTI->t_photometric);

	fprintf(stderr,"planr : %d \n",	imTI->t_planarconfig);

	fprintf(stderr,"orien : %d \n",	imTI->t_orientation);

	fprintf(stderr,"sampl : %d \n", imTI->t_samplesperpixel);	

	fprintf(stderr,"rowsp : %d \n", imTI->t_rowsperstrip);

	fprintf(stderr,"descrip  : %s \n",	imTI->t_imagedescr);

	fprintf(stderr,"software : %s \n", 	imTI->t_software);

	fprintf(stderr,"datetime : %s \n", 	imTI->t_datetime);

	fprintf(stderr,"user     : %s \n", 	imTI->t_artist);

	fprintf(stderr,"host     : %s \n", 	imTI->t_host);	

	fprintf(stderr,"whitepoint : %s \n", 	imTI->t_whitepoint);

	fprintf(stderr,"primarychr : %s \n", 	imTI->t_primarychrom);

	fprintf(stderr,"predictor  : %d \n", 	imTI->t_predictor);

	fprintf(stderr,"matting    : %d \n", 	imTI->t_matteing);



	imPrintTiffDirLongEntry( &imTI->t_xposition, "xposition" );

	imPrintTiffDirLongEntry( &imTI->t_yposition, "yposition" );

	imPrintTiffDirLongEntry( &imTI->t_bitspersample, "bitspersample" );

	imPrintTiffDirLongEntry( &imTI->t_colormap, "colormap" );

	imPrintTiffDirLongEntry( &imTI->t_stripbytecounts, "stripbytecounts" );

	imPrintTiffDirLongEntry( &imTI->t_stripoffsets, "stripoffsets" );



	fprintf(stderr,"------------------- \n");

}





static void

#ifdef __STDC__

imPrintTiffWriteDir( char *s, unsigned int *sc, unsigned int *so )

#else

imPrintTiffWriteDir( s, sc, so )

char *s;

unsigned int *sc, *so;

#endif

{

	int i, y;



	fprintf(stderr,"%s \n",s);



	for(i=0;i<IMTIFFWRITEDIRENTRIES;i++)

	{

		imPrintTiffTag( imTiffWriteDir[i].tdir_tag );

		fprintf(stderr,"cnt %d  type %d  offset %d 0x%x \n",

			imTiffWriteDir[i].tdir_count,

			imTiffWriteDir[i].tdir_type,

			imTiffWriteDir[i].tdir_offset,

			imTiffWriteDir[i].tdir_offset );

	}



	fprintf(stderr,"\n <bytecount offset> \n");



	y = imTiffWriteDir[IMTIFF_IMAGELENGTH_INDEX].tdir_offset >> 16;	



	for( i=0; i<y; i++ )

		fprintf(stderr,"<%d %d> ", *sc++, *so++ );

}



#endif       /* DEBUG  */





/*

 *  FUNCTION

 *	imTiffRead	-  read a TIFF image file

 *

 *  DESCRIPTION

 *	The file header is read and the magic number checked.  If there is

 *	a CLT in the file, it is read in and converted into an ImClt.

 *	Separate routines are then called to handle different depth and

 *	storage format variations of the image data.

 */



static int				/* Returns status		*/

#ifdef __STDC__

imTiffRead( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable )

#else

imTiffRead( ioType, fd, fp, flags, tagTable )

	int              ioType;	/* I/O flags			*/

	int	         fd;		/* Input file descriptor	*/

	FILE	        *fp;		/* Input file pointer		*/

	TagTable        *flags;		/* Format flags			*/

	TagTable        *tagTable;	/* Tag table to add to		*/

#endif

{

	unsigned int		 nextDir;	/* Offset to next directory	*/

	unsigned int		 imageCount;	/* Number of images in file	*/

	ImVfb	 	*vfb;		/* vfb holder			*/

	ImClt	 	*clt;		/* clt holder			*/

	unsigned char		 fileBuf;	/* Read in the whole file	*/

	int		 status;	/* Status returned from readdir */

	imTiffDirInfo	 imTiffInfo;	/* Structure with useful info	*/

	int		 depth;		/* The bit depth of the image	*/

	char		 message[100];	/* ImInfo message		*/



	imageCount = 0;



	/*

	 * Initialize the byte ordering to something so that we can read the

	 * header

	 */

	BinByteOrder( BINMBF );

	BinFloatFormat( BINIEEE );



	/*

	 * Read the magic number, check it, initialize byte order, then

	 * read in the version number and directory offset to the first

	 * directory.

	 */



	if ( ImBinRead( ioType, fd, fp, &imTiffHeader.tiff_magic, USHORT, 2, 1 ) == -1)

	{

		ImReturnBinError( );

	}





	if ( imTiffHeader.tiff_magic == IMTIFFBIGENDIANMAGIC ) 

		BinByteOrder( BINMBF );



	else if ( imTiffHeader.tiff_magic == IMTIFFLITTLEENDIANMAGIC ) 

		BinByteOrder( BINLBF );



	else

	{

 		ImErrNo = IMEMAGIC;

                ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	if ( ImBinRead( ioType, fd, fp, &imTiffHeader.tiff_version,

		USHORT, 2, 1 ) == -1)

	{

		ImReturnBinError( );

	}



	if ( imTiffHeader.tiff_version != IMTIFFVERSION ) 

	{

		ImErrNo = IMEVERSION;

                ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	/*

	 * Output -verbose message

	 */

	sprintf( message, "%d", IMTIFFVERSION );

	ImInfo( "Version", message );

	if ( imTiffHeader.tiff_magic == IMTIFFBIGENDIANMAGIC ) 

	{

		ImInfo( "Byte Order", "Most Significant Byte First" );

	}

	else

		ImInfo( "Byte Order", "Least Significant Byte First" );





	if ( ImBinRead( ioType, fd, fp, &imTiffHeader.tiff_diroff,

		UINT, 4, 1 ) == -1)

	{

		ImReturnBinError( );

	}



	/*

	 * Loop for every Image File Directory (IFD) in the file

	 */



	nextDir = imTiffHeader.tiff_diroff;



	while ( nextDir )

	{

		/*

		 * Read in the directory info.  It is variable length.

		 * Put the important info in the imTiffDirInfo struct.

		 */



		memset( &imTiffInfo, 0x00, sizeof(imTiffDirInfo) );



		status = imTiffReadDirectory( ioType, fd, fp, &nextDir, 

								&imTiffInfo );



		if ( status == -1 )

			return( -1 );  		/* ImErrNo already set */



#ifdef DEBUG

		imPrintTiffDirInfo( &imTiffInfo );

#endif



		/*

		 * Output -verbose message

		 */

		sprintf( message, "%d", (imageCount+1) );

		ImInfo( "Image", message );



		if( imTiffInfo.t_imagedescr != NULL )

		{

			ImInfo( "Description", imTiffInfo.t_imagedescr );

		}

		else

			ImInfo( "Description", "none" );



		sprintf( message, "%d x %d", imTiffInfo.t_width,

			 imTiffInfo.t_height );

		ImInfo( "Resolution", message );



		depth = imTiffInfo.t_depth * imTiffInfo.t_samplesperpixel;

		if( depth < 24 )

			sprintf( message, "%d-bit Color Indexed", depth );

		else

			sprintf( message, "%d-bit RGB", depth );

		ImInfo( "Type", message );



		if( imTiffInfo.t_planarconfig == IM_PLANARCONFIG_CONTIG )

			sprintf( message, "Chunky (Contiguous)" );

		else

			sprintf( message, "Planar (Separate)" );

		ImInfo( "Plane Configuration", message );



		/*

		 * Call the appropriate routine for the depth of the image.

		 * These routines are not called anywhere else but at least

 		 * it breaks up the code for debugging and modularity.

		 * The routines called here have to make their own decisions

		 * about what to do with encoded pixel data.

		 */



		depth = imTiffInfo.t_depth * imTiffInfo.t_samplesperpixel;



		/*

		 * 32 images refer to index32.  24 includes the alpha channel

		 */

		if ( imTiffInfo.t_matteing )

			depth = 24;



		switch( depth )

		{

		case 16:

			status = imTiffRead16( ioType, fd, fp, flags, 

					tagTable, &imTiffInfo, &vfb );

			break;



		case 24:

			if ( imTiffInfo.t_planarconfig == IM_PLANARCONFIG_CONTIG )

				status = imTiffRead24Cont(ioType,fd, fp, flags, 

					tagTable, &imTiffInfo, &vfb );

			else 

			if ( imTiffInfo.t_planarconfig == IM_PLANARCONFIG_SEPARATE)

				status = imTiffRead24Sep(ioType,fd, fp, flags, 

					tagTable, &imTiffInfo, &vfb );

			else

			{

				ImErrNo = IMEPLANES;

				status = -1;

			}

			break;



		case 8:

			status = imTiffRead8( ioType, fd, fp, flags, 

					tagTable, &imTiffInfo, &vfb );

			break;



		case 4:

			status = imTiffRead4( ioType, fd, fp, flags, 

					tagTable, &imTiffInfo, &vfb );

			break;



		case 1:

			status = imTiffRead1( ioType, fd, fp, flags, 

					tagTable, &imTiffInfo, &vfb );

			break;





		case 32:

		default:

			ImErrNo = IMEDEPTH;	/* We only understand 24,8,1 */ 

                	ImErrorFatal( ImQError( ), -1, ImErrNo );

		}

		

		if ( status == -1 )

			return( -1 );		/* ImErrNo already set */



		/*

		 * Check the image orientation

		 */

		if ( imTiffInfo.t_orientation == IM_ORIENTATION_BOTLEFT  ||

		     imTiffInfo.t_orientation == IM_ORIENTATION_LEFTBOT )

			if ( ImVfbFlip( vfb, IMVFBYFLIP, vfb ) == IMVFBNULL )

			{

				ImErrorFatal( ImQError( ), -1, ImErrNo );

			}



		/*

		 * Get an associated CLT if there is one

		 */

		if ( imTiffGetClt( &imTiffInfo, &clt ) == -1 )

			return( -1 );		/* ImErrNo already set */



		if ( clt != IMCLTNULL )

		{

			ImVfbSClt( vfb, clt );

			TagTableAppend( tagTable, 

				TagEntryAlloc( "image clt", POINTER, &clt ));

		}

		TagTableAppend(tagTable, TagEntryAlloc( "image vfb",

			POINTER, &vfb ));



		/*

		 * Output -verbose message

		 */

		switch( imTiffInfo.t_compression )

		{

			case IM_COMPRESSION_NONE:

				sprintf( message, "none (Dump)" );

				break;

			case IM_COMPRESSION_CCITTRLE:

				sprintf( message, "CCITT Run Length Encoded \
 (Huffman RLE)" );

				break;

			case IM_COMPRESSION_CCITTFAX3:

				sprintf( message, "CCITT Group 3 FAX" );

				break;

			case IM_COMPRESSION_CCITTFAX4:

				sprintf( message, "CCITT Group 4 FAX" );

				break;

			case IM_COMPRESSION_LZW:

				sprintf(message,"Lempel-Ziv and Welch (LZW)" );

				break;

			case IM_COMPRESSION_NEXT:

				sprintf( message, "Run Length Encoded \
(NeXT 2-bit RLE)" );

				break;

			case IM_COMPRESSION_CCITTRLEW:

				sprintf( message, "Run Length Encoded W \
(RLE with word alignment)" );

				break;

			case IM_COMPRESSION_PACKBITS:

				sprintf( message, "Apple Macintosh Packbits");

				break;

			case IM_COMPRESSION_THUNDERSCAN:

				sprintf( message, "Run Length Encoded \
(Thunderscan RLE)" );

				break;

			case IM_COMPRESSION_PICIO:

				sprintf( message, "Run Length Encoded \
(Pixar RLE)" );

				break;

			case IM_COMPRESSION_SGIRLE:

				sprintf( message, "Run Length Encoded \
(Silicon Graphics RLE)" );

				break;

		}

		ImInfo( "Compression Type", message );



		if( imTiffInfo.t_matteing )

		{

			ImInfo( "Alpha Channel", "8-bit\n" );

		}

		else

			ImInfo( "Alpha Channel", "none\n" );





		imageCount++;

	}



	if ( imTiffInfo.t_compression == IM_COMPRESSION_LZW )

		ImLzwCleanup( );



	/*

	 * Return the number of vfb's in the tag table

	 */

	return( imageCount );

}







/*

 *  FUNCTION

 *	imTiffDecode	-  Decode the type, length, and offset fields

 *

 *  DESCRIPTION

 *	This is the case where the actual data item is stored in the offset

 *	field.  The type indicates how many bytes we want (i think).  The

 *	bytes are left justified so we have to shift to get them out.

 */



static int

#ifdef __STDC__

imTiffDecode( imTiffDirEntry *tifDir, unsigned char *p ) 

#else

imTiffDecode( tifDir, p ) 

	imTiffDirEntry 	*tifDir;

	unsigned char 	*p;

#endif

{

	int 	shift;

	int	da;



	switch( tifDir->tdir_type )

	{

	case IMTIFFBYTE:

	case IMTIFFASCII:

		shift = (IMTIFFBYTESPERWORD - tifDir->tdir_count) * IMTIFFBITSPERBYTE;

		da = tifDir->tdir_offset >> shift;

		memcpy( p, &da, sizeof(int) );

		break;



	case IMTIFFSHORT:

		shift = (IMTIFFSHORTSPERWORD - tifDir->tdir_count) * IMTIFFBITSPERBYTE;

		da = tifDir->tdir_offset >> (shift * IMTIFFSHORTSPERWORD);

		memcpy( p, &da, sizeof(int) );

		break;



	case IMTIFFLONG:

		memcpy( p, &tifDir->tdir_offset, sizeof(int) );

		break;

	

	case IMTIFFRATIONAL:	/* It is an error to have 8 byte data here*/

	default:

		ImErrNo = IMEOUTOFRANGE;  

                ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	return( 0 );

}





/*

 *  FUNCTION

 *	imTiffDecOrFetch	-  Decode or fetch directory data

 *

 *  DESCRIPTION

 *	Seek to the proper place in the file, allocate memory and then

 *	read in the pertinent data.  This is used to read in arrays of

 *	data including strings, and colormaps.

 *	If the data is actually just stuffed in the offset field then

 *	copy that to the malloced space.

 *	Return a pointer to the dataspace.

 */



static int				/* Returns pointer or -1 for error */

#ifdef __STDC__

imTiffDecOrFetch( int ioType, int fd, FILE *fp, imTiffDirEntry *d, unsigned char **p )

#else

imTiffDecOrFetch( ioType, fd, fp, d, p )

	int           	 ioType;	/* I/O flags			*/

	int	    	 fd;		/* Input file descriptor	*/

	FILE	       	*fp;		/* Input file pointer		*/

	imTiffDirEntry	*d; 		/* The directory entry to fill 	*/

	unsigned char	**p;		/* Return a pointer result	*/

#endif

{

	int 	 	size;		/* Size of the dir data		*/

	int 	 	status;		/* Returned from Bin Read	*/

	int		*ip=NULL;

	unsigned short	*sp=NULL;

	double		*dp=NULL;



	/*

	 * Size of the data in the tiff file, NOT size in memory

	 */

        size = d->tdir_count * imTiffDataWidth[d->tdir_type];



	if ( size <= 4 )

	{

		if (imTiffDecode( d, (unsigned char*)p ) == -1) 

			return( -1 );		/* ImErrNo already set */

	}

	else

	{

		ImSeek( ioType, fd, fp, d->tdir_offset, 0 );



		switch( d->tdir_type )

		{

		case IMTIFFRATIONAL:

			ImMalloc( dp, double *, d->tdir_count * sizeof(double));

			status = ImBinRead( ioType, fd, fp, dp, DOUBLE, 8,

				 d->tdir_count );

			*p = (unsigned char *)dp;

			break;



		case IMTIFFLONG:

			ImMalloc( ip, int *, d->tdir_count * sizeof(int));

			status = ImBinRead( ioType, fd, fp, ip, INT, 4,

				 d->tdir_count );

			*p = (unsigned char *)ip;

			break;



		case IMTIFFSHORT:

			ImMalloc( sp, unsigned short *, d->tdir_count * sizeof(ushort));

			status = ImBinRead( ioType, fd, fp, sp, USHORT, 2,

				 d->tdir_count );

			*p = (unsigned char *)sp;

			break;



		case IMTIFFBYTE:

		case IMTIFFASCII:

			ImMalloc( *p, unsigned char *, d->tdir_count * sizeof(unsigned char));

			status = ImBinRead( ioType, fd, fp, *p, UCHAR, 1,

				 d->tdir_count );

			break;

		}



		if ( status == -1 )

		{

			ImReturnBinError( );

		}

	}



	return( 0 );

}







/*

 *  FUNCTION

 *	imTiffDataItem	-  Get a data value out of the data field

 *

 *  DESCRIPTION

 *	Data may either be resident in the data field or else may be

 *	out in an data array.  Given an index and a XXX structure, find

 *	and return the right size data element.

 */



static int				/* Returns pointer or -1 for error */

#ifdef __STDC__

imTiffDataItem( imTiffDirData *iTD, int index )

#else

imTiffDataItem( iTD, index )

	imTiffDirData	*iTD;		/* The data item structure	*/

	int		 index;		/* What number item		*/

#endif

{

	int 	 	size;		/* Size of one item of dir data	*/

	int 	 	shift;		/* Number of bits to shift	*/

	unsigned char		*cd;		/* Array of chars		*/

	short		*sd;		/* Array of shorts		*/

	int		*id;		/* Array of ints		*/

	double		*dd;		/* Array of doubles		*/



	/*

	 * Size of the data in the tiff file, Not size in memory

	 */

	size = imTiffDataWidth[iTD->type];



	if ( iTD->cnt * size <= 4 )	/* Data is resident in data field */

	{

		shift = index * size * IMTIFFBITSPERBYTE; 

		return(((int)iTD->data >> shift) & imTiffMask[iTD->type]);

	}

	else 

	{

        	/* 

         	 *  Use the correct pointer type, and then indices work !

         	 */

		switch (iTD->type) 

                {

                case IMTIFFBYTE:

                case IMTIFFASCII:  

			cd = (unsigned char *)iTD->data;

                        return ((int)cd[index]);

                        break;



                case IMTIFFSHORT: 

			sd = (short *)iTD->data;

                        return ((int)sd[index]);

                        break;



                case IMTIFFLONG: 

			id = (int *)iTD->data;

                        return ((int)id[index]);

                        break;



                case IMTIFFRATIONAL: 

			dd = (double *)iTD->data;

                        return ((int)dd[index]);

                        break;

                }



	}

}





/*

 *      imTiffReadDirectory -  read a TIFF Rasterfile Directory

 *

 *  DESCRIPTION

 *      Read a Tiff directory.  Parse the directory and associated pixel data.

 *      Store the resulting vfb and possibly clt in the tagtable.

 *      Return the offset to the next tiff directory in dirOffset.

 */



static int		

#ifdef __STDC__

imTiffReadDirectory( int ioType, int fd, FILE *fp, unsigned int *dirOffset, imTiffDirInfo *imTDI )

#else

imTiffReadDirectory( ioType, fd, fp, dirOffset, imTDI )

	int              ioType;	/* I/O flags			*/

	int	         fd;		/* Input file descriptor	*/

	FILE	        *fp;		/* Input file pointer		*/

	unsigned int 	*dirOffset;	/* The offset of the tiff dir 	*/

	imTiffDirInfo	*imTDI;		/* Unencoded directory info	*/

#endif

{

	imTiffDirEntry	*tifDir;	/* Store an array of entries	*/

	unsigned char		*bits;		/* Point to an array of data	*/

	unsigned short		 numEntries;	/* Number of directory entries	*/

	unsigned short		 so[2];		/* Temp storage of two shorts	*/

	int		 i;		/* Loop counter			*/

	int 	 	 c,t;		/* count and type of data data	*/

	int 	 	 depth;		/* Bit depth of image		*/

	int		 status=0;	/* Hold returned status		*/

	int		 size;		/* Number of bytes of offset	*/





	/*

	 * Seek to the directory at offset dirOffset in the file.

	 */

	ImSeek( ioType, fd, fp, *dirOffset, 0 );



	/* 

	 * Read in the number of entries in this directory, allocate space

	 * and then read in the directory entries one by one.

	 */

	if ( ImBinRead( ioType, fd, fp, &numEntries, USHORT, 2, 1) == -1 )

	{

		ImReturnBinError( );

	}



	ImCalloc( tifDir, imTiffDirEntry *, sizeof(imTiffDirEntry), numEntries);



	/*

	 * Read all of the directory entries into an array of structs

	 */

	for( i=0; i<numEntries; i++ )

	{

		/*

		 * Read the type, count, and tag fields since they are

		 * of fixed type.  

		 */

		if ( ImBinReadStruct( ioType, fd, fp, &(tifDir[i]), 

						imTiffDirFields) == -1 )

		{

			ImReturnBinError( );

		}



		/*

		 * Read the offset field separately since it can contain one

		 * of three different types of variables.

		 * Check to see if the offset field contains an offset.

		 * If it does, then read it as an int and continue

		 */

        	size = tifDir[i].tdir_count *	

				imTiffDataWidth[tifDir[i].tdir_type];



		if ( size > 4 )

		{

			if ( ImBinRead( ioType, fd, fp, 

				&(tifDir[i].tdir_offset), INT, 4, 1) == -1 )

				ImReturnBinError( );

			continue;

		}



		/*

		 * If it is not an offset, then there is data in the

		 * offset field.  We have to read it as the correct type

		 */

		switch( tifDir[i].tdir_type )

		{

		case IMTIFFBYTE:

		case IMTIFFASCII:

			status = ImBinRead( ioType, fd, fp, 

					&(tifDir[i].tdir_offset), UCHAR, 1, 4);

			break;



		case IMTIFFSHORT:

			status = ImBinRead( ioType, fd, fp, so, USHORT, 2, 2);

			tifDir[i].tdir_offset = (so[0] << 16) | so[1];

			break;



		case IMTIFFLONG:

			status = ImBinRead( ioType, fd, fp, 

					&(tifDir[i].tdir_offset), INT, 4, 1);

			break;

		}



		if ( status == -1 )

			ImReturnBinError( );

	}



	/*

	 * Read the offset of the next directory.  It will probably be

	 * zero indicating that this file only has one directory.

	 */

	if ( ImBinRead( ioType, fd, fp, dirOffset, UINT, 4, 1) == -1 )

	{

		ImReturnBinError( );

	}



	/*

	 * Parse the directory entries and do something sensible with each

	 * value we are interested in.

	 */

	for( i=0; i<numEntries; i++ )

	{

		switch( tifDir[i].tdir_tag )

		{

		case IM_TIFFTAG_IMAGEWIDTH:

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]), 

						(unsigned char**)&imTDI->t_width );

			break;



		case IM_TIFFTAG_IMAGELENGTH:	/* This means height */

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]), 

						(unsigned char**)&imTDI->t_height );

			break;



		case IM_TIFFTAG_COMPRESSION:

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]), 

						(unsigned char**)&imTDI->t_compression );

			break;

	

		case IM_TIFFTAG_PHOTOMETRIC:

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]), 

						(unsigned char**)&imTDI->t_photometric );

			break;



		case IM_TIFFTAG_IMAGEDESCRIPTION:

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]), 

						&imTDI->t_imagedescr );

			break;



		case IM_TIFFTAG_ORIENTATION:

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]),

						(unsigned char**)&imTDI->t_orientation );

			if ( imTDI->t_orientation != IM_ORIENTATION_TOPLEFT &&

			     imTDI->t_orientation != IM_ORIENTATION_BOTLEFT &&

			     imTDI->t_orientation != IM_ORIENTATION_LEFTTOP &&

			     imTDI->t_orientation != IM_ORIENTATION_LEFTBOT ) 

			{

				ImErrNo = IMEORIENTATION;

                		ImErrorFatal( ImQError( ), -1, ImErrNo );

			}

			break;



		case IM_TIFFTAG_SAMPLESPERPIXEL:

			status= imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]), 

						(unsigned char**)&imTDI->t_samplesperpixel );

			break;



		case IM_TIFFTAG_ROWSPERSTRIP:

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]), 

						(unsigned char**)&imTDI->t_rowsperstrip );

			break;



		case IM_TIFFTAG_BITSPERSAMPLE:

			imTDI->t_bitspersample.type = tifDir[i].tdir_type;

			imTDI->t_bitspersample.cnt = tifDir[i].tdir_count;

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]),

					&imTDI->t_bitspersample.data );



			/*

			 * The depth may be (8 8 8) for eight bits each

			 * channel.  Just save one of the counts in the 

			 * in the t_depth field for easier access.

			 */	



			imTDI->t_depth=imTiffDataItem(&imTDI->t_bitspersample,0);

			break;

		

		case IM_TIFFTAG_STRIPBYTECOUNTS:

			imTDI->t_stripbytecounts.type = tifDir[i].tdir_type;

			imTDI->t_stripbytecounts.cnt = tifDir[i].tdir_count;

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]),

					&imTDI->t_stripbytecounts.data );

			break;



		case IM_TIFFTAG_STRIPOFFSETS:

			imTDI->t_stripoffsets.type = tifDir[i].tdir_type;

			imTDI->t_stripoffsets.cnt = tifDir[i].tdir_count;

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]),

					&imTDI->t_stripoffsets.data );

			break;



		case IM_TIFFTAG_COLORMAP:

			imTDI->t_colormap.type = tifDir[i].tdir_type;

			imTDI->t_colormap.cnt = tifDir[i].tdir_count;

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]),

					&imTDI->t_colormap.data );

			break;



		case IM_TIFFTAG_PLANARCONFIG:

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]), 

						(unsigned char**)&imTDI->t_planarconfig );

			break;



		case IM_TIFFTAG_XPOSITION:

			imTDI->t_xposition.type = tifDir[i].tdir_type;

			imTDI->t_xposition.cnt = tifDir[i].tdir_count;

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]),

						&imTDI->t_xposition.data );

			break;



		case IM_TIFFTAG_YPOSITION:

			imTDI->t_yposition.type = tifDir[i].tdir_type;

			imTDI->t_yposition.cnt = tifDir[i].tdir_count;

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]),

						&imTDI->t_yposition.data );

			break;



		case IM_TIFFTAG_SOFTWARE:

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]), 

						&imTDI->t_software );

			break;



		case IM_TIFFTAG_DATETIME:

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]), 

						&imTDI->t_datetime );

			break;



		case IM_TIFFTAG_ARTIST:

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]), 

						&imTDI->t_artist );

			break;



		case IM_TIFFTAG_HOSTCOMPUTER:

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]), 

						&imTDI->t_host );

			break;



		case IM_TIFFTAG_WHITEPOINT:

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]), 

						&imTDI->t_whitepoint );

			break;



		case IM_TIFFTAG_PRIMARYCHROMATICITIES:

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]), 

						&imTDI->t_primarychrom );

			break;



		case IM_TIFFTAG_PREDICTOR:

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]), 

						(unsigned char**)&imTDI->t_predictor );

			break;



		case IM_TIFFTAG_MATTEING:

			status = imTiffDecOrFetch( ioType, fd, fp, &(tifDir[i]), 

						(unsigned char**)&imTDI->t_matteing );

			break;



		default:

			ImErrNo = IMEFORMAT;

                	ImErrorWarning("Ignoring tiff dir tag", -1, ImErrNo );

                	imPrintTiffTag( tifDir[i].tdir_tag );

			break;

		}



		if ( status == -1 )

			return( -1 );   	/* ImErrNo already set */



	} /* End of parsing directory */



	/* 

	 * Check to be sure that values are reasonable 

	 */



	if ( imTDI->t_predictor > 1 )

	{

		ImErrNo = IMEUNSUPPORTED;

                ImErrorFatal( "LZW+Prediction not supported", -1, ImErrNo );

	}

	if ( imTDI->t_width < 1 )

	{

		ImErrNo = IMEWIDTH;

                ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	if ( imTDI->t_height < 1 )

	{

		ImErrNo = IMEHEIGHT;

                ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	if ( imTDI->t_rowsperstrip == 0 )

		imTDI->t_rowsperstrip = imTDI->t_height;



	if ( imTDI->t_depth == 0 )

		imTDI->t_depth = 1;



	if ( imTDI->t_samplesperpixel == 0 )

		imTDI->t_samplesperpixel = 1;



	if ( imTDI->t_compression == 0 )

		imTDI->t_compression = IM_COMPRESSION_NONE;



	if ( imTDI->t_orientation == 0 )

		imTDI->t_orientation = IM_ORIENTATION_TOPLEFT;



	if ( imTDI->t_compression != IM_COMPRESSION_NONE &&

	     imTDI->t_compression != IM_COMPRESSION_PACKBITS &&

	     imTDI->t_compression != IM_COMPRESSION_LZW ) 

	{

		ImErrNo = IMEUNSUPPORTED;

                ImErrorFatal("TIFF compression scheme not supported", -1, ImErrNo );

	}



	/*

	 * If the bytecounts field is missing and there is more than one

	 * strip offset given, then give up, it is too hard to figure out.

 	 * The bytecounts field is mandatory but someprograms don't write it

	 * anyway.

	 */



	if ( imTDI->t_stripbytecounts.data == NULL )

	{

		if ( imTDI->t_compression != IM_COMPRESSION_NONE )

		{

			ImErrNo = IMECONFLICT;

                	ImErrorFatal( 

			"TIFF required tag 'stripbytecounts' not present", 

				-1, ImErrNo );

		}



		if ( imTDI->t_stripoffsets.cnt > 1 )

		{

			ImErrNo = IMECONFLICT;

                	ImErrorFatal( ImQError( ), -1, ImErrNo );

		}



                ImErrorWarning( 

		"TIFF required tag 'stripbytecounts' tag not present, trying anyway",

				-1, IMESYNTAX );



		imTDI->t_stripbytecounts.cnt = 1;

		imTDI->t_stripbytecounts.type = IMTIFFLONG;

		imTDI->t_stripbytecounts.data = (unsigned char*) ((imTDI->t_width * 

			imTDI->t_height * imTDI->t_depth) / IMTIFFBITSPERBYTE);

	}



	free( tifDir );

	return ( 0 );

}





#define	imTiffFree()			\
	if(strip!=NULL)			\
		free(strip);		\
	if(ucstrip!=NULL)		\
		free(ucstrip);		



/*

 *  FUNCTION

 *	imTiffRead1	-  read 1-bit TIFF format

 *	imTiffRead4	-  read 4-bit TIFF format

 *	imTiffRead8	-  read 8-bit TIFF format

 *	imTiffRead16	-  read 16-bit TIFF format (CLT longer than 256)

 *	imTiffRead24Sep	-  read 24-bit TIFF format separate planes

 *	imTiffRead24Cont	-  read 24-bit TIFF format contiguous planes

 *

 *  DESCRIPTION

 *	Each of these routines deal with TIFF files.

 *	The input stream is a file or a pipe.  We don't care.

 *

 *	A new VFB is allocated.  The image is read in

 *	and converted into the VFB.

 *

 *	The 1 and 4 bit reads should be changed when we implement 1 and 4

 *	bit vfbs.

 */



static int				/* Returns status		*/

#ifdef __STDC__

imTiffRead1( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imTiffDirInfo *imTDI, ImVfb **pVfb )

#else

imTiffRead1( ioType, fd, fp, flags, tagTable, imTDI, pVfb )

	int             ioType;		/* I/O flags			*/

	int	        fd;		/* Input file descriptor	*/

	FILE	       *fp;		/* Input file pointer		*/

	TagTable       *flags;		/* Format flags			*/

	TagTable       *tagTable;	/* Tag table to add to		*/

	imTiffDirInfo  *imTDI;		/* Directory information	*/

	ImVfb	      **pVfb;		/* VFB to fill and return	*/

#endif

{

	ImVfbPtr 	 pptr;		/* Pixel pointer		*/

	ImVfb	 	*vfb;		/* A more convenient pointer	*/

	unsigned char		*strip;		/* Storage for pixel indices	*/

	unsigned char		*ucstrip=NULL;	/* If encoded, use this buffer	*/

	unsigned char		*stripptr;	/* Walk thru the pixels		*/

	int		 size;		/* The number of bytes of index */

	int		 i,j;		/* Loop counters		*/

	int		 where;		/* Where to start reading	*/

	int		 howMany;	/* How many to read		*/

	int		 decodedsize;	/* How many bytes when decoded	*/

	int		 k;		/* Counter			*/

	int		 rows;		/* Number of scanlines this time*/

	int		 rowsread=0;	/* Number of scanlines readsofar*/

	int		 shift;		/* Number of bits to shift	*/

	int		 val;		/* Pixel value, either 0 or 255 */



	/*

	 * Allocate a VFB of the required size.

	 */



	if ( (*pVfb = ImVfbAlloc( imTDI->t_width, imTDI->t_height, 

		IMVFBMONO )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	vfb = *pVfb;

        pptr = ImVfbQFirst( vfb );



	/*

	 * Allocate space for the pixel values

	 * Sometimes things are bigger when compresses, so we make it 2x 

	 */



	size = (imTDI->t_width * imTDI->t_rowsperstrip) / IMTIFFBITSPERBYTE;

	ImMalloc( strip, unsigned char *, size*2 );



	/*

	 * If compressed, we need a extra buffer

	 */



	if ( imTDI->t_compression != IM_COMPRESSION_NONE )

		ImMalloc( ucstrip, unsigned char *, size );

	

	rows = imTDI->t_rowsperstrip;



	/*

	 * Read in the strips and put them in the vfb one at a time

	 */ 



	for( i=0; i < imTDI->t_stripoffsets.cnt; i++ )

	{

		/*

		 * We have to know how many pixels are left to read, so we

		 * keep track of the number of rows read so far.  We keep

 		 * these numbers in uncompressed bytes even if reading a

 		 * compressed file.

		 */



		if ( rows > imTDI->t_height - rowsread )  

			rows = imTDI->t_height % imTDI->t_rowsperstrip;



		decodedsize = rows * ((imTDI->t_samplesperpixel *

			imTDI->t_width + (IMTIFFBITSPERBYTE - 1)) /

			IMTIFFBITSPERBYTE);



		where = imTiffDataItem( &imTDI->t_stripoffsets, i );

		howMany = imTiffDataItem( &imTDI->t_stripbytecounts, i );



		ImSeek( ioType, fd, fp, where, 0 );



		if ( ImBinRead( ioType, fd, fp, strip, UCHAR, 1, howMany )== -1)

		{

			ImReturnBinError( );

		}



		rowsread += rows;

		stripptr = strip;



		/*

		 * If the rows are compressed, uncompress them

		 */



		if ( imTDI->t_compression == IM_COMPRESSION_LZW )

		{

			if (ImLzwPreDecode( strip, howMany ) == -1)

			{

				imTiffFree();

				ImVfbFree(vfb);

				return( -1 );

			}

			if (ImLzwDecode( strip, ucstrip, decodedsize ) == -1)

			{

				imTiffFree();

				ImVfbFree(vfb);

				return( -1 );

			}

			ImLzwPostDecode();



			stripptr = ucstrip;

		}



		if ( imTDI->t_compression == IM_COMPRESSION_PACKBITS )

		{

			UnpackBits( strip, ucstrip, (unsigned int*)&howMany );

			decodedsize = howMany;

			stripptr = ucstrip;

		}



		for ( k = 0; k < rows; k++ )

		{

			/*

			 *  Added to VFB.  Watch for the last byte of each

			 *  row.  If the image width is not a multiple of 8,

			 *  the last byte will not be filled and we'll need

			 *  to skip the extra bits.

			 */

			for ( j = 0; j < imTDI->t_width; j++ )

			{

				shift=IMTIFFBITSPERBYTE-(j % IMTIFFBITSPERBYTE)-1;

				val= (*stripptr >> shift) & 0x01;	

				if (shift == 0)

					stripptr++;

				ImVfbSMono( vfb, pptr, val );

				ImVfbSInc( vfb, pptr );

			}

			if ( shift != 0 )

				stripptr++;

		}

	}



	imTiffFree();

	return(1);

}







static int				/* Returns status		*/

#ifdef __STDC__

imTiffRead4( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imTiffDirInfo *imTDI, ImVfb **pVfb )

#else

imTiffRead4( ioType, fd, fp, flags, tagTable, imTDI, pVfb )

	int             ioType;		/* I/O flags			*/

	int	        fd;		/* Input file descriptor	*/

	FILE	       *fp;		/* Input file pointer		*/

	TagTable       *flags;		/* Format flags			*/

	TagTable       *tagTable;	/* Tag table to add to		*/

	imTiffDirInfo  *imTDI;		/* Directory information	*/

	ImVfb	      **pVfb;		/* VFB to fill and return	*/

#endif

{

	ImVfbPtr 	 pptr;		/* Pixel pointer		*/

	ImVfb	 	*vfb;		/* A more convenient pointer	*/

	unsigned char		*strip;		/* Storage for pixel indices	*/

	unsigned char		*ucstrip=NULL;	/* If encoded, use this buffer	*/

	unsigned char		*stripptr;	/* Walk thru the pixels		*/

	int		 size;		/* The number of bytes of index */

	int		 i,j;		/* Loop counters		*/

	int		 where;		/* Where to start reading	*/

	int		 howMany;	/* How many to read		*/

	int		 decodedsize;	/* How many bytes when decoded	*/

	int		 rows;		/* Number of scanlines this time*/

	int		 rowsread=0;	/* Number of scanlines readsofar*/

	int		 shift;		/* Number of bits to shift	*/

	int		 val;		/* Pixel value, either 0 or 255 */



	/*

	 * Allocate a VFB of the required size.

	 */

	if ( (*pVfb = ImVfbAlloc( imTDI->t_width, imTDI->t_height, 

		IMVFBINDEX8 )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	vfb = *pVfb;

        pptr = ImVfbQFirst( vfb );



	/*

	 * Allocate space for the pixel values

	 * Sometimes things are bigger when compresses, so we make it 2x 

	 */



	size = (imTDI->t_width * imTDI->t_rowsperstrip) / IMTIFFNIBBLESPERBYTE;

	ImMalloc( strip, unsigned char *, size*2 );



	/*

	 * If compressed, we need a extra buffer

	 */



	if ( imTDI->t_compression != IM_COMPRESSION_NONE )

		ImMalloc( ucstrip, unsigned char *, size );

	

	rows = imTDI->t_rowsperstrip;



	/*

	 * Read in the strips and put them in the vfb one at a time

	 */ 



	for( i=0; i < imTDI->t_stripoffsets.cnt; i++ )

	{

		/*

		 * We have to know how many pixels are left to read, so we

		 * keep track of the number of rows read so far.  We keep

 		 * these numbers in uncompressed bytes even if reading a

 		 * compressed file.

		 */



		if ( rows > imTDI->t_height - rowsread )  

			rows = imTDI->t_height % imTDI->t_rowsperstrip;



		decodedsize = (rows * imTDI->t_samplesperpixel * imTDI->t_width 

				* imTDI->t_depth) / IMTIFFBITSPERBYTE;



		where = imTiffDataItem( &imTDI->t_stripoffsets, i );

		howMany = imTiffDataItem( &imTDI->t_stripbytecounts, i );



		ImSeek( ioType, fd, fp, where, 0 );



		if ( ImBinRead( ioType, fd, fp, strip, UCHAR, 1, howMany )== -1)

		{

			ImReturnBinError( );

		}



		rowsread += rows;

		stripptr = strip;



		/*

		 * If the rows are compressed, uncompress them

		 */



		if ( imTDI->t_compression == IM_COMPRESSION_LZW )

		{

			if (ImLzwPreDecode( strip, howMany ) == -1)

			{

				imTiffFree();

				ImVfbFree(vfb);

				return( -1 );

			}

			if (ImLzwDecode( strip, ucstrip, decodedsize ) == -1)

			{

				imTiffFree();

				ImVfbFree(vfb);

				return( -1 );

			}

			ImLzwPostDecode();



			stripptr = ucstrip;

		}



		if ( imTDI->t_compression == IM_COMPRESSION_PACKBITS )

		{

			UnpackBits( strip, ucstrip, (unsigned int*)&howMany );

			decodedsize = howMany;

			stripptr = ucstrip;

		}





		/*

		 * If there is no colormap, then put them in a gray vfb

		 */

		for( j=0; j < decodedsize*IMTIFFNIBBLESPERBYTE; j++ )

		{

			if ( j % IMTIFFNIBBLESPERBYTE == 0 )

				val=((*stripptr >> 4) & 0x0f);	

			else

			{

				val=(*stripptr & 0x0f);	

				stripptr++;

			}

			ImVfbSIndex8( vfb, pptr, val * IMTIFFNIBBLEMULT );

			ImVfbSInc( vfb, pptr );

		}

	}



	imTiffFree();

	return(1);

}







static int				/* Returns status		*/

#ifdef __STDC__

imTiffRead8( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imTiffDirInfo *imTDI, ImVfb **pVfb )

#else

imTiffRead8( ioType, fd, fp, flags, tagTable, imTDI, pVfb )

	int             ioType;		/* I/O flags			*/

	int	        fd;		/* Input file descriptor	*/

	FILE	       *fp;		/* Input file pointer		*/

	TagTable       *flags;		/* Format flags			*/

	TagTable       *tagTable;	/* Tag table to add to		*/

	imTiffDirInfo  *imTDI;		/* Directory information	*/

	ImVfb	      **pVfb;		/* VFB to fill and return	*/

#endif

{

	ImVfbPtr 	 pptr;		/* Pixel pointer		*/

	ImVfb	 	*vfb;		/* A more convenient pointer	*/

	unsigned char		*strip;		/* Storage for pixel indices	*/

	unsigned char		*ucstrip=NULL;	/* If encoded, use this buffer	*/

	unsigned char		*stripptr;	/* Walk thru the pixels		*/

	int		 size;		/* The number of bytes of index */

	int		 i,j;		/* Loop counters		*/

	int		 where;		/* Where to start reading	*/

	int		 howMany;	/* How many to read		*/

	int		 decodedsize;	/* How many bytes when decoded	*/

	int		 rows;		/* Number of scanlines this time*/

	int		 rowsread=0;	/* Number of scanlines readsofar*/



	/*

	 * Allocate a VFB of the required size.

	 */

	if ( (*pVfb = ImVfbAlloc( imTDI->t_width, imTDI->t_height, 

		IMVFBINDEX8 )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	vfb = *pVfb;

        pptr = ImVfbQFirst( vfb );



	/*

	 * Allocate space for the pixel indices

	 * Sometimes things are bigger when compresses, so we make it 2x 

	 */



	size = imTDI->t_width * imTDI->t_rowsperstrip;

	ImMalloc( strip, unsigned char *, size*2 );



	/*

	 * If compressed, we need a extra buffer

	 */



	if ( imTDI->t_compression != IM_COMPRESSION_NONE )

		ImMalloc( ucstrip, unsigned char *, size );

	

	rows = imTDI->t_rowsperstrip;





	/*

	 * Read in the strips and put them in the vfb one at a time

	 */ 



	for( i=0; i < imTDI->t_stripoffsets.cnt; i++ )

	{

		/*

		 * We have to know how many pixels are left to read, so we

		 * keep track of the number of rows read so far.  We keep

 		 * these numbers in uncompressed bytes even if reading a

 		 * compressed file.

		 */



		if ( rows > imTDI->t_height - rowsread )  

			rows = imTDI->t_height % imTDI->t_rowsperstrip;



		decodedsize = (rows * imTDI->t_samplesperpixel * imTDI->t_width 

				* imTDI->t_depth) / IMTIFFBITSPERBYTE;



		where = imTiffDataItem( &imTDI->t_stripoffsets, i );

		howMany = imTiffDataItem( &imTDI->t_stripbytecounts, i );



		ImSeek( ioType, fd, fp, where, 0 );



		if ( ImBinRead( ioType, fd, fp, strip, UCHAR, 1, howMany )== -1)

		{

			ImReturnBinError( );

		}



		rowsread += rows;

		stripptr = strip;



		/*

		 * If the rows are compressed, uncompress them

		 */



		if ( imTDI->t_compression == IM_COMPRESSION_LZW )

		{

			if (ImLzwPreDecode( strip, howMany ) == -1)

			{

				imTiffFree();

				ImVfbFree(vfb);

				return( -1 );

			}

			if (ImLzwDecode( strip, ucstrip, decodedsize ) == -1)

			{

				imTiffFree();

				ImVfbFree(vfb);

				return( -1 );

			}

			ImLzwPostDecode();



			stripptr = ucstrip;

		}



		if ( imTDI->t_compression == IM_COMPRESSION_PACKBITS )

		{

			UnpackBits( strip, ucstrip, (unsigned int*)&howMany );

			decodedsize = howMany;

			stripptr = ucstrip;

		}





		for( j=0; j < decodedsize; j++ )

		{

			ImVfbSIndex8( vfb, pptr, *stripptr++ );

			ImVfbSInc( vfb, pptr );

		}

	}



	imTiffFree();

	return(1);

}

	



static int				/* Returns status		*/

#ifdef __STDC__

imTiffRead16( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imTiffDirInfo *imTDI, ImVfb **pVfb )

#else

imTiffRead16( ioType, fd, fp, flags, tagTable, imTDI, pVfb )

	int             ioType;		/* I/O flags			*/

	int	        fd;		/* Input file descriptor	*/

	FILE	       *fp;		/* Input file pointer		*/

	TagTable       *flags;		/* Format flags			*/

	TagTable       *tagTable;	/* Tag table to add to		*/

	imTiffDirInfo  *imTDI;		/* Directory information	*/

	ImVfb	      **pVfb;		/* VFB to fill and return	*/

#endif

{

	ImVfbPtr 	 pptr;		/* Pixel pointer		*/

	ImVfb	 	*vfb;		/* A more convenient pointer	*/

	unsigned char		*strip;		/* Storage for pixel indices	*/

	unsigned char		*ucstrip=NULL;	/* If encoded, use this buffer	*/

	unsigned char		*stripptr;	/* Walk thru the pixels		*/

	unsigned char		*rbp;		/* Walk thru an array of uchars	*/

	int		 size;		/* The number of bytes of index */

	int		 i,j;		/* Loop counters		*/

	unsigned int		 tmp;		/* Temp integer holder		*/

	int		 where;		/* Where to start reading	*/

	int		 howMany;	/* How many to read		*/

	int		 decodedsize;	/* How many bytes when decoded	*/

	int		 rows;		/* Number of scanlines this time*/

	int		 rowsread=0;	/* Number of scanlines readsofar*/



	/*

	 * Allocate a VFB of the required size.

	 */



	if ( (*pVfb = ImVfbAlloc( imTDI->t_width, imTDI->t_height, 

		IMVFBINDEX16 )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	vfb = *pVfb;

        pptr = ImVfbQFirst( vfb );



	/*

	 * Allocate space for the pixel indices

	 * Sometimes things are bigger when compresses, so we make it 2x 

	 */



	size = imTDI->t_width * imTDI->t_rowsperstrip * imTDI->t_depth/8;

	ImMalloc( strip, unsigned char *, size*2 );



	/*

	 * If compressed, we need a extra buffer

	 */



	if ( imTDI->t_compression != IM_COMPRESSION_NONE )

		ImMalloc( ucstrip, unsigned char *, size );

	

	rows = imTDI->t_rowsperstrip;



	/*

	 * Read in the strips and put them in the vfb one at a time

	 */ 



	for( i=0; i < imTDI->t_stripoffsets.cnt; i++ )

	{

		/*

		 * We have to know how many pixels are left to read, so we

		 * keep track of the number of rows read so far.  We keep

 		 * these numbers in uncompressed bytes even if reading a

 		 * compressed file.

		 */



		if ( rows > imTDI->t_height - rowsread )  

			rows = imTDI->t_height % imTDI->t_rowsperstrip;



		decodedsize = (rows * imTDI->t_samplesperpixel * imTDI->t_width 

				* imTDI->t_depth) / IMTIFFBITSPERBYTE;



		where = imTiffDataItem( &imTDI->t_stripoffsets, i );

		howMany = imTiffDataItem( &imTDI->t_stripbytecounts, i );



		ImSeek( ioType, fd, fp, where, 0 );



		if ( ImBinRead( ioType, fd, fp, strip, UCHAR, 1, howMany )== -1)

		{

			ImReturnBinError( );

		}



		rowsread += rows;

		stripptr = strip;



		/*

		 * If the rows are compressed, uncompress them

		 */



		if ( imTDI->t_compression == IM_COMPRESSION_LZW )

		{

			if (ImLzwPreDecode( strip, howMany ) == -1)

			{

				imTiffFree();

				ImVfbFree(vfb);

				return( -1 );

			}

			if (ImLzwDecode( strip, ucstrip, decodedsize ) == -1)

			{

				imTiffFree();

				ImVfbFree(vfb);

				return( -1 );

			}

			ImLzwPostDecode();



			stripptr = ucstrip;

		}



		if ( imTDI->t_compression == IM_COMPRESSION_PACKBITS )

		{

			UnpackBits( strip, ucstrip, (unsigned int*)&howMany );

			decodedsize = howMany;

			stripptr = ucstrip;

		}



		/*

		 * Put the values into a vfb.

		 */

		if ( imTiffHeader.tiff_magic == IMTIFFBIGENDIANMAGIC ) 

		{

			/* MBF byte order.				*/

			rbp = stripptr;

			for( j=0; j < decodedsize / 2; j++ )

			{

				tmp = ((*rbp) << 8) | (*(rbp+1));

				rbp += 2;

				ImVfbSIndex16( vfb, pptr, tmp );

				ImVfbSInc( vfb, pptr );

			}

		}

		else

		{

			/* LBF byte order.				*/

			rbp = stripptr;

			for( j=0; j < decodedsize / 2; j++ )

			{

				tmp = ((*(rbp+1)) << 8) | (*rbp);

				rbp += 2;

				ImVfbSIndex16( vfb, pptr, tmp );

				ImVfbSInc( vfb, pptr );

			}

		}

	}



	imTiffFree();

	return(1);

}







static int				/* Returns status		*/

#ifdef __STDC__

imTiffRead24Cont( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imTiffDirInfo *imTDI, ImVfb **pVfb )

#else

imTiffRead24Cont( ioType, fd, fp, flags, tagTable, imTDI, pVfb )

	int             ioType;		/* I/O flags			*/

	int	        fd;		/* Input file descriptor	*/

	FILE	       *fp;		/* Input file pointer		*/

	TagTable       *flags;		/* Format flags			*/

	TagTable       *tagTable;	/* Tag table to add to		*/

	imTiffDirInfo  *imTDI;		/* Directory information	*/

	ImVfb	      **pVfb;		/* VFB to fill and return	*/

#endif

{

	ImVfbPtr 	 pptr;		/* Pixel pointer		*/

	ImVfb	 	*vfb;		/* A more convenient pointer	*/

	unsigned char		*strip;		/* Storage for pixel indices	*/

	unsigned char		*ucstrip=NULL;	/* If encoded, use this buffer	*/

	unsigned char		*stripptr;	/* Walk thru the pixels		*/

	int		 size;		/* The number of bytes of index */

	int		 i,j;		/* Loop counters		*/

	int		 where;		/* Where to start reading	*/

	int		 howMany;	/* How many to read		*/

	int		 decodedsize;	/* How many bytes when decoded	*/

	int		 rows;		/* Number of scanlines this time*/

	int		 alpha;		/* Is there an alpha channel	*/

	int		 rowsread=0;	/* Number of scanlines readsofar*/



	/*

	 * Allocate a VFB of the required size.

 	 * If there is an alpha channel make room for it.

	 * The location of the alpha channel is undocumented so we are assuming

	 * that it is mixed in with the color channels.

	 */



	alpha = imTDI->t_matteing;



	if ( alpha )

	{

		if ( (*pVfb = ImVfbAlloc( imTDI->t_width, imTDI->t_height, 

			IMVFBRGB|IMVFBALPHA )) == IMVFBNULL )

		{

			ImErrorFatal( ImQError( ), -1, ImErrNo );

		}

		size = imTDI->t_width * imTDI->t_rowsperstrip * 4;

	}

	else

	{

		if ( (*pVfb = ImVfbAlloc( imTDI->t_width, imTDI->t_height, 

			IMVFBRGB )) == IMVFBNULL )

		{

			ImErrorFatal( ImQError( ), -1, ImErrNo );

		}

		size = imTDI->t_width * imTDI->t_rowsperstrip * 3;

	}



	vfb = *pVfb;

        pptr = ImVfbQFirst( vfb );



	/*

	 * Allocate space for the pixel values

	 * Sometimes things are bigger when compresses, so we make it 2x 

	 */



	ImMalloc( strip, unsigned char *, size*2 );



	/*

	 * If compressed, we need a extra buffer

	 */



	if ( imTDI->t_compression != IM_COMPRESSION_NONE )

		ImMalloc( ucstrip, unsigned char *, size );

	

	rows = imTDI->t_rowsperstrip;



	/*

	 * Read in the strips and put them in the vfb one at a time

	 */ 



	for( i=0; i < imTDI->t_stripoffsets.cnt; i++ )

	{

		/*

		 * We have to know how many pixels are left to read, so we

		 * keep track of the number of rows read so far.  We keep

 		 * these numbers in uncompressed bytes even if reading a

 		 * compressed file.

		 */



		if ( rows > imTDI->t_height - rowsread )  

			rows = imTDI->t_height % imTDI->t_rowsperstrip;



		decodedsize = (rows * imTDI->t_samplesperpixel * imTDI->t_width 

				* imTDI->t_depth) / IMTIFFBITSPERBYTE;



		where = imTiffDataItem( &imTDI->t_stripoffsets, i );

		howMany = imTiffDataItem( &imTDI->t_stripbytecounts, i );



		ImSeek( ioType, fd, fp, where, 0 );



		if ( ImBinRead( ioType, fd, fp, strip, UCHAR, 1, howMany )== -1)

		{

			ImReturnBinError( );

		}



		rowsread += rows;

		stripptr = strip;



		/*

		 * If the rows are compressed, uncompress them

		 */



		if ( imTDI->t_compression == IM_COMPRESSION_LZW )

		{

			if (ImLzwPreDecode( strip, howMany ) == -1)

			{

				imTiffFree();

				ImVfbFree(vfb);

				return( -1 );

			}



			if (ImLzwDecode( strip, ucstrip, decodedsize ) == -1)

			{

				imTiffFree();

				ImVfbFree(vfb);

				return( -1 );

			}

			ImLzwPostDecode();



			stripptr = ucstrip;

		}



		if ( imTDI->t_compression == IM_COMPRESSION_PACKBITS )

		{

			UnpackBits( strip, ucstrip, (unsigned int*)&howMany );

			decodedsize = howMany;

			stripptr = ucstrip;

		}





		/*

		 * If there is no colormap, then put them in a gray vfb

		 */

		if ( alpha )

			for( j=0; j < decodedsize/4; j++ )

			{

				ImVfbSRed( vfb, pptr, *stripptr++ );

				ImVfbSGreen( vfb, pptr, *stripptr++ );

				ImVfbSBlue( vfb, pptr, *stripptr++ );

				ImVfbSAlpha( vfb, pptr, *stripptr++ );

				ImVfbSInc( vfb, pptr );

			}

		else

			for( j=0; j < decodedsize/3; j++ )

			{

				ImVfbSRed( vfb, pptr, *stripptr++ );

				ImVfbSGreen( vfb, pptr, *stripptr++ );

				ImVfbSBlue( vfb, pptr, *stripptr++ );

				ImVfbSInc( vfb, pptr );

			}

	}



	imTiffFree();

	return(1);

}





static int				/* Returns status		*/

#ifdef __STDC__

imTiffRead24Sep( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imTiffDirInfo *imTDI, ImVfb **pVfb )

#else

imTiffRead24Sep( ioType, fd, fp, flags, tagTable, imTDI, pVfb )

	int             ioType;		/* I/O flags			*/

	int	        fd;		/* Input file descriptor	*/

	FILE	       *fp;		/* Input file pointer		*/

	TagTable       *flags;		/* Format flags			*/

	TagTable       *tagTable;	/* Tag table to add to		*/

	imTiffDirInfo  *imTDI;		/* Directory information	*/

	ImVfb	      **pVfb;		/* VFB to fill and return	*/

#endif

{

	ImVfbPtr 	 pptr;		/* Pixel pointer		*/

	ImVfb	 	*vfb;		/* A more convenient pointer	*/

	unsigned char		*strip;		/* Storage for pixels		*/

	unsigned char		*ucstrip=NULL;	/* If encoded, use this buffer	*/

	unsigned char		*stripptr;	/* Walk thru the pixels	red	*/

	int		 where;		/* Where to start reading	*/

	int		 howMany;	/* How many reds to read	*/

	int		 i,j;		/* Loop counters		*/

	int		 decodedsize;	/* How many bytes when decoded	*/

	int		 alpha=0;	/* Is there an alpha channel	*/

	int		 samples;	/* Samples per pixel		*/

	int		 rows;		/* Number of scanlines this time*/

	int		 size;		/* The number of bytes of index */

	int		 rowsread;	/* Number of scanlines readsofar*/

	int		 color;		/* Loop counter for colors	*/

	int		 stripspercolor;/* Number of strips/component	*/



	/*

	 * Tiff files with separate image planes store all of the red rows

	 * followed by all of the green rows and then all of the blue and

	 * alpha rows (I think).

	 */



	alpha   = imTDI->t_matteing;

	samples = imTDI->t_samplesperpixel;

	rows    = imTDI->t_rowsperstrip;

	size    = imTDI->t_width * rows;

	stripspercolor = imTDI->t_stripoffsets.cnt/samples;



	/*

	 * Allocate a VFB of the required size.

 	 * If there is an alpha channel make room for it.

	 * The location of the alpha channel is undocumented so we are assume

	 * that it follows the blue plane

	 */



	if ( alpha )

	{

		if ( (*pVfb = ImVfbAlloc( imTDI->t_width, imTDI->t_height, 

			IMVFBRGB|IMVFBALPHA )) == IMVFBNULL )

		{

			ImErrorFatal( ImQError( ), -1, ImErrNo );

		}

	}

	else

	{

		if ( (*pVfb = ImVfbAlloc( imTDI->t_width, imTDI->t_height, 

			IMVFBRGB )) == IMVFBNULL )

		{

			ImErrorFatal( ImQError( ), -1, ImErrNo );

		}

	}



	vfb = *pVfb;



	/*

	 * Allocate space for one component of one row of pixels

	 * Sometimes things are bigger when compresses, so we make it 2x 

	 */



	ImMalloc( strip, unsigned char *, size*2 );



	/*

	 * If compressed, we need a extra buffer

	 */



	if ( imTDI->t_compression != IM_COMPRESSION_NONE )

		ImMalloc( ucstrip, unsigned char *, size );

	

	/*

	 * Cycle through red, green, blue, alpha(?) and for each one

	 * read in all of the scanlines (rows), decode them (?), and 

	 * put them into a vfb.

	 */ 



	for( color=0; color<samples; color++ )

	{

        	pptr = ImVfbQFirst( vfb );

		rowsread = 0;



		for( i=0; i < stripspercolor; i++ )

		{

			/*

		 	 * We have to know how many pixels are left to read, 

			 * so we keep track of the number of rows read so far.

			 * We keep these numbers in uncompressed bytes even 

			 * if reading a compressed file.

		 	 */



			if ( rows > imTDI->t_height - rowsread )  

				decodedsize = imTDI->t_width *

				     (imTDI->t_height % imTDI->t_rowsperstrip);

			else

				decodedsize = imTDI->t_width * rows;

	

			/*

		 	 * Get the stripoffset and stripbytecount from the file

		 	 */

			where = imTiffDataItem( &imTDI->t_stripoffsets,

						(stripspercolor * color) + i );

			howMany = imTiffDataItem( &imTDI->t_stripbytecounts, 

						(stripspercolor * color) + i );

	

			ImSeek( ioType, fd, fp, where, 0 );



			if ( ImBinRead(ioType, fd, fp, strip, UCHAR, 1, 

				howMany) == -1)

			{

				ImReturnBinError( );

			}

	

			/*

		 	 * Keep track of the number of rows because the last 

			 * row may not contain as many scanlines as the others

		 	 */

			rowsread += (decodedsize/imTDI->t_width);

			stripptr = strip;

	

			/*

		 	* If the rows are compressed, uncompress them

		 	*/

	

			if ( imTDI->t_compression == IM_COMPRESSION_LZW )

			{

				if (ImLzwPreDecode( strip, howMany ) == -1)

				{

					imTiffFree();

					return( -1 );

				}

				if (ImLzwDecode(strip,ucstrip,decodedsize)== -1)

				{

					imTiffFree();

					return( -1 );

				}

				ImLzwPostDecode();

				stripptr = ucstrip;

			}

	

			if ( imTDI->t_compression == IM_COMPRESSION_PACKBITS )

			{

				UnpackBits( strip, ucstrip, (unsigned int*)&howMany );

				decodedsize = howMany;

				stripptr = ucstrip;

			}

	

			/*

		 	 * Put one component of one row in a vfb

		 	 */

			switch ( color )

			{

			case 0:

				for( j=0; j < decodedsize; j++ )

				{	

					ImVfbSRed( vfb, pptr, *stripptr++ );

					ImVfbSInc( vfb, pptr );

				}

				break;

			case 1:

				for( j=0; j < decodedsize; j++ )

				{

					ImVfbSGreen( vfb, pptr, *stripptr++ );

					ImVfbSInc( vfb, pptr );

				}

				break;

			case 2:

				for( j=0; j < decodedsize; j++ )

				{

					ImVfbSBlue( vfb, pptr, *stripptr++ );

					ImVfbSInc( vfb, pptr );

				}

				break;

			case 3:

				for( j=0; j < decodedsize; j++ )

				{

					ImVfbSAlpha( vfb, pptr, *stripptr++ );

					ImVfbSInc( vfb, pptr );

				}

				break;

			}



		}	/* End foreach row   */

	

	}		/* End foreach color */



	imTiffFree();

	return(1);

}

	



/*

 *  FUNCTION

 *	imTiffGetClt	-  Transfer a colormap into a CLT

 *

 *  DESCRIPTION

 *	Elements in a TIFF colormap may be bytes or shorts or whatever.

 *	So we use memcpys to get them out of the colormap and into the CLT.

 */

static int

#ifdef __STDC__

imTiffGetClt( imTiffDirInfo *imTDI, ImClt **pClt )

#else

imTiffGetClt( imTDI, pClt )

	imTiffDirInfo  *imTDI;		/* Directory information	*/

	ImClt	      **pClt;		/* VFB to fill and return	*/

#endif

{

	ImClt		*clt;		/* Color pointer		*/

	ImCltPtr	 cptr;		/* Color pointer		*/

	int		 i;		/* Loop counter			*/

	unsigned int             redp;		/* Red CLT range pointer	*/

	unsigned int             grnp;		/* Green CLT range pointer	*/

	unsigned int             blup;		/* Blue CLT range pointer	*/

	unsigned int             n;		/* Number of colors		*/

	unsigned char		 red;		/* Temporary storage of red val */

	unsigned char		 grn;		/* Temporary storage of grn val */

	unsigned char		 blu;		/* Temporary storage of blu val */

	unsigned char		*cd;		/* Treat array as chars		*/

	unsigned short		*sd;		/* Treat array as shorts	*/

	char		 message[100];	/* ImInfo message		*/





	/*

	 * If there is a colormap in the file, put it in a CLT

	 */



	*pClt = IMCLTNULL;



	if ( imTDI->t_colormap.data != NULL )

	{

		clt = *pClt = ImCltAlloc( imTDI->t_colormap.cnt/3 );

		cptr = ImCltQFirst( clt );



		n = imTDI->t_colormap.cnt / 3;

		redp = 0;

		grnp = n;

		blup = n+n;



		/*

		 * Output -verbose message

		 */

		sprintf( message, "%d Entries", n );

		ImInfo( "Color Table", message );



		switch ( imTDI->t_colormap.type )

		{

		case IMTIFFBYTE:

		case IMTIFFASCII:

                        cd = (unsigned char *)imTDI->t_colormap.data;

			for ( i = 0; i < n; i++ )

			{

                            	red = cd[redp++];

                            	grn = cd[grnp++];

                            	blu = cd[blup++];

                        	ImCltSRed (cptr, red);

                        	ImCltSGreen (cptr, grn);

                        	ImCltSBlue (cptr, blu);

				ImCltSInc( clt, cptr );

			}

			break;

		case IMTIFFSHORT:

                        sd = (unsigned short *)imTDI->t_colormap.data;

			for ( i = 0; i < n; i++ )

			{

				/*

				 * Since we can only store 8-bit R, G and

				 * B values, we have to use some method

				 * of removing 8-bits of data from the 16-bit

				 * short.  This usually involves a

				 * scaling function to reduce the

				 * range of colors.  For TIFF colortable

				 * entries, all we need to do is use

				 * the upper 8-bits and ignore the lower

				 * 8-bits.

				 */

                            	red = (unsigned char) (sd[redp++] );

                            	grn = (unsigned char) (sd[grnp++] );

                            	blu = (unsigned char) (sd[blup++] );

				

                        	ImCltSRed (cptr, red);

                        	ImCltSGreen (cptr, grn);

                        	ImCltSBlue (cptr, blu);

				ImCltSInc( clt, cptr );

			}

			break;

		}

	}

	else

		ImInfo( "Color Table", "none" );



	if ( imTDI->t_colormap.data != NULL )

		free( imTDI->t_colormap.data );



	return( 0 );

}





/*

 *  FUNCTION

 *	imTiffWrite	-  Write a TIFF format file or stream

 *

 *  DESCRIPTION

 *	Each of these routines deal with TIFF files.

 */



static int                             /* Returns # of tags written    */

#ifdef __STDC__

imTiffWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imTiffWrite( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

        int             ioType;         /* I/O flags                    */

        int             fd;             /* Output file descriptor       */

        FILE           *fp;             /* Output file pointer          */

        TagTable       *flagsTable;     /* Format flags                 */

        TagTable       *tagTable;       /* Tag list to add to           */

#endif

{

        ImClt          *clt;            /* CLT pointer                  */

        ImVfb          *vfb;            /* VFB pointer                  */

        char           *s;              /* Tag name string              */



        int             nTag = 0;       /* # of tags written            */

        int             n;              /* # of tag table entries       */

        int             i;              /* Counter                      */

	int		flags;		/* Write flags			*/

        TagEntry       *tagEntry;       /* Tag table entry holder       */

	char		 message[100];	/* ImInfo message		*/





        /*

         *  All administrative stuff (tags, reference counts, file offsets)

         *  are in MBF byte order.

         */

        BinByteOrder( BINMBF );

	BinFloatFormat( BINIEEE );



	ImInfo( "Version", "42" );

	ImInfo( "Byte Order", "Most Significant Byte First" );



	/*

	 * Write out the file header, initialize the byte order and check

	 * the magic number, check the version number.

	 */

	memset( (void *)&imTiffHeader, 0x00, sizeof( imTiffHeader ) );

	imTiffHeader.tiff_magic = IMTIFFBIGENDIANMAGIC; 

	imTiffHeader.tiff_version = IMTIFFVERSION; 

	imTiffHeader.tiff_diroff = 0;			/* Fill this in later */



	if ( ImBinWriteStruct( ioType, fd, fp, &imTiffHeader, 

		imTiffHeaderFields ) == -1 )

	{

		ImReturnBinError( );

	}



	n = TagTableQNEntry( tagTable, "image vfb" );



	for ( i = 0; i < n; i++ )

	{

		sprintf( message, "%d of %d\n", (i+1), n );

		ImInfo( "Image", message );

		ImInfo( "Description", "none" );



		tagEntry = TagTableQDirect( tagTable, "image vfb", i );

		s = TagEntryQTag( tagEntry );

		TagEntryQValue( tagEntry, &vfb );

		if ( imTiffVfbWrite( ioType, fd, fp, flagsTable, vfb )== -1)

			return ( -1 );  /* Error already posted */

		nTag++;

	}



	/* 

	 * Write out a zero into the next-dir-offset location indicating

	 * that there are no more directories

	 */



	n = 0;

	if (ImBinWrite( ioType, fd, fp, &n, UINT, 4, 1 ) == -1)

	{

		ImReturnBinError( );

	}

	return( 0 );

}





/*

 *  FUNCTION

 *	imTiffVfbWrite	-  write VFB to file

 *

 *  DESCRIPTION

 *	Write out Clt, Vfb, and then stripbyte counts, and

 *	stripoffsets.  Then write out the directory afterwards.

 *	Seek back and fill in the directory offset.

 *	This routine may be called multiple times for

 *	multiple vfb/clt sets.

 */



static int				/* Returns # of tags written	*/

#ifdef __STDC__

imTiffVfbWrite( int ioType, int fd, FILE *fp, TagTable *flagsTable, ImVfb *vfb )

#else

imTiffVfbWrite( ioType, fd, fp, flagsTable, vfb )

	int             ioType;		/* I/O flags			*/

	int             fd;		/* Output file descriptor	*/

	FILE	       *fp;		/* Output file pointer		*/

        TagTable       *flagsTable;     /* Format flags                 */

	ImVfb          *vfb;		/* VFB to write out		*/

#endif

{

	ImClt  		*clt;  		/* VFB's CLT                    */

	ImCltPtr	 pColor;	/* VFB's CLT                    */

	ImVfbPtr	 pptr;		/* VFB's pixel ptr		*/

	unsigned short		 sh;		/* For writing out a temp const */

	unsigned short		 so[2];		/* Temp storage for writing	*/

	int		 i,j;		/* Loop counters		*/

	int		 n;		/* Number of colors in colormap */

	int		 junk;		/* A dummy value to write out 	*/

	int		 fields;	/* VFB header fields		*/

	int		 sx,x,y,d;	/* Convenient dimension access  */

	int		 pixcnt;	/* The number of bytes written  */

	int		 compress;	/* Do lzw compression ?		*/

	int		 cnt;		/* Byte count after compression	*/

	int		 diroffsetposition;  /* Offset in file		*/

	unsigned int		*stripbytecountsptr; /* Walk thru byte counts	*/

	unsigned int		*stripbytecounts;    /* Byte count array	*/

	unsigned int		*stripoffsetsptr;    /* Byte count after compression */

	unsigned int		*stripoffsets;  /* Byte offsets to pixel info	*/

	int		 newdiroffset; 	/* The offset to the new direc 	*/

	int		 offset; 	/* Used to calc word boundaries */

	int		 shift; 	/* Number of bits to shift      */

	int		 size; 		/* Number of bytes in offset fld*/

	int		 status;	/* Returned from binwrite	*/

	unsigned char		*buffer=NULL;	/* Pixel buffer		 	*/

	unsigned char		*pbuffer=NULL;	/* Pack bits pixel buffer	*/

	unsigned short		*cbuffer=NULL;	/* Colormap buffer		*/

	unsigned char		*rbp;		/* Walk thru the buffer	as bytes*/

	unsigned char		 bw;		/* A value, either 255 or 0	*/

	unsigned int		*rip;		/* Walk thru the buffer	as ints	*/

	char		 errstr[256];	/* For printing out error stings*/

/*

	int		 xoffset;

	int		 yoffset;

*/

	double		 xoffset;

	double		 yoffset;



	unsigned int		tmp;		/* Integer holder		*/

	int		interRequest;	/* Interleave request		*/

	int		compRequest;	/* Compression request		*/

	int		cltRequest;	/* CLT request			*/

	int		alphaRequest;	/* Alpha plane request		*/

	TagEntry       *tagEntry;	/* Flags table entry holder	*/

	char		 message[100];	/* ImInfo message		*/





	/*

	 *  Get the requests from the flags table.

	 */

	interRequest = IMINTERPLANE;

	compRequest  = IMCOMPRLE;

	cltRequest   = IMCLTYES;

	alphaRequest = IMALPHAYES;

	if ( flagsTable != TAGTABLENULL )

	{

		tagEntry = TagTableQDirect( flagsTable, "image interleave request", 0 );

		if ( tagEntry != TAGENTRYNULL )

			TagEntryQValue( tagEntry, &interRequest );

		/* interRequest is guaranteed to be one we can support.	*/



		tagEntry = TagTableQDirect( flagsTable, "image compression request", 0 );

		if ( tagEntry != TAGENTRYNULL )

			TagEntryQValue( tagEntry, &compRequest );

		/* compRequest is guaranteed to be one we can support.	*/



		tagEntry = TagTableQDirect( flagsTable, "image clt request", 0 );

		if ( tagEntry != TAGENTRYNULL )

			TagEntryQValue( tagEntry, &cltRequest );

		/* cltRequest is guaranteed to be one we can support.	*/



		tagEntry = TagTableQDirect( flagsTable, "image alpha request", 0 );

		if ( tagEntry != TAGENTRYNULL )

			TagEntryQValue( tagEntry, &alphaRequest );

		/* alphaRequest is guaranteed to be one we can support.	*/

	}





	/* 

	 * Eventually we need to get the x & y image offsets out of the

	 * the tag table

	 */

	xoffset = 0;

	yoffset = 0;



	/*

	 * The current write position is 4 bytes past 

	 * where the "next-dir-offset" is going

	 * to go, when we know what that is.

	 */

	diroffsetposition = ImTell( ioType, fd, fp ) - 4;





	if ( compRequest == IMCOMPLZW )

	{

		imTiffWriteDir[IMTIFF_COMPRESSION_INDEX].tdir_offset = 

							IM_COMPRESSION_LZW << 16;

		compress = IM_COMPRESSION_LZW;

	}

	else if ( compRequest == IMCOMPPB )

	{

		imTiffWriteDir[IMTIFF_COMPRESSION_INDEX].tdir_offset = 

							IM_COMPRESSION_PACKBITS << 16;

		compress = IM_COMPRESSION_PACKBITS;

	}

	else

	{

		imTiffWriteDir[IMTIFF_COMPRESSION_INDEX].tdir_offset = 

							IM_COMPRESSION_NONE << 16;

		compress = IM_COMPRESSION_NONE;

	}

		



	/*

	 * Write out the clt if there is one and we are supposed to write it

	 * out.  Remember where it is and such.

	 */

	clt = ImVfbQClt( vfb );



	if ( clt != IMCLTNULL && cltRequest == IMCLTYES )

	{

		imTiffWriteDir[IMTIFF_COLORMAP_INDEX].tdir_count = n 

					 	= 3 * ImCltQNColors(clt);



		imTiffWriteDir[IMTIFF_COLORMAP_INDEX].tdir_offset = ImTell( ioType, fd, fp );



		ImMalloc( cbuffer, unsigned short*, sizeof(ushort) * n );



        	pColor = ImCltQFirst( clt );



        	for ( i = 0; i < n/3; i++ )

        	{

                	cbuffer[i]     = (unsigned short) ImCltQRed( pColor );

                	cbuffer[i+n/3]   = (unsigned short) ImCltQGreen( pColor );

                	cbuffer[i+n/3+n/3] = (unsigned short) ImCltQBlue( pColor );

                	ImCltSInc( clt, pColor );

        	}

		if (ImBinWrite( ioType, fd, fp, cbuffer, USHORT, 2, n ) == -1)

		{

			ImReturnBinError( );

		}



		free( cbuffer );

	}

	else

	{

		imTiffWriteDir[IMTIFF_COLORMAP_INDEX].tdir_count= 0;

		imTiffWriteDir[IMTIFF_COLORMAP_INDEX].tdir_offset = 0;

	}



	/*

	 * Set width and height in the tiff directory

	 */

	pptr = ImVfbQFirst( vfb );

	x = ImVfbQWidth( vfb );

	y = ImVfbQHeight( vfb );

	imTiffWriteDir[IMTIFF_IMAGEWIDTH_INDEX].tdir_offset =  x << 16; 

	imTiffWriteDir[IMTIFF_IMAGELENGTH_INDEX].tdir_offset = y << 16;



	sprintf( message, "%d x %d", x, y );

	ImInfo( "Resolution", message );



	/*

	 * Set depth and bitspersample in the tiff directory

	 */



        fields = ImVfbQFields( vfb );



	if ( fields & IMVFBMONO )

	{

		d = 1;

		imTiffWriteDir[IMTIFF_BITSPERSAMPLE_INDEX].tdir_offset = 1<<16;

		imTiffWriteDir[IMTIFF_SAMPLESPERPIXEL_INDEX].tdir_offset= 1<<16;

	        imTiffWriteDir[IMTIFF_PHOTOMETRIC_INDEX].tdir_offset =

			IM_PHOTOMETRIC_MINISBLACK << 16;

	}

	else if ( fields & IMVFBINDEX8 )

	{

		d = 8;

		imTiffWriteDir[IMTIFF_BITSPERSAMPLE_INDEX].tdir_offset = 8<<16;

		imTiffWriteDir[IMTIFF_SAMPLESPERPIXEL_INDEX].tdir_offset= 1<<16;

	        imTiffWriteDir[IMTIFF_PHOTOMETRIC_INDEX].tdir_offset =

			IM_PHOTOMETRIC_PALETTE << 16;

	}

	else if ( fields & IMVFBINDEX16 )

	{

		d = 16;

		imTiffWriteDir[IMTIFF_BITSPERSAMPLE_INDEX].tdir_offset = 8<<16;

		imTiffWriteDir[IMTIFF_SAMPLESPERPIXEL_INDEX].tdir_offset= 4<<16;

	        imTiffWriteDir[IMTIFF_PHOTOMETRIC_INDEX].tdir_offset =

			IM_PHOTOMETRIC_PALETTE << 16;

	}

	else if ( (fields & IMVFBRGB) && (fields & IMVFBALPHA) && alphaRequest == IMALPHAYES )

	{

		d = 32;

		imTiffWriteDir[IMTIFF_BITSPERSAMPLE_INDEX].tdir_offset = 8<<16;

		imTiffWriteDir[IMTIFF_SAMPLESPERPIXEL_INDEX].tdir_offset= 4<<16;

		imTiffWriteDir[IMTIFF_MATTEING_INDEX].tdir_offset = 1<<16;

	        imTiffWriteDir[IMTIFF_PHOTOMETRIC_INDEX].tdir_offset =

			IM_PHOTOMETRIC_RGB << 16;

	}

	else if ( fields & IMVFBRGB )

	{	

		d = 24;

		imTiffWriteDir[IMTIFF_BITSPERSAMPLE_INDEX].tdir_offset = 8<<16;

		imTiffWriteDir[IMTIFF_SAMPLESPERPIXEL_INDEX].tdir_offset= 3<<16;

	        imTiffWriteDir[IMTIFF_PHOTOMETRIC_INDEX].tdir_offset =

			IM_PHOTOMETRIC_RGB << 16;

	}

	else 

	{

		ImErrNo = IMEDEPTH;

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	if( d < 24 )

		sprintf( message, "%d-bit Color Indexed", d );

	else

		sprintf( message, "%d-bit RGB", d );

	ImInfo( "Type", message );

	ImInfo( "Plane Configuration", "Chunky (Contiguous)" );



	if( clt != NULL )

		sprintf( message, "%d Entries", ImCltQNColors( clt ) );

	else

		sprintf( message, "none" );

	ImInfo( "Color Table", message );



	/*

	 * Write out place holders for the stripbyte counts and

	 * the stripbyteoffsets.  Remember where they are and such.

	 */

	ImCalloc( stripbytecounts,  unsigned int *, sizeof(uint), y );  





	stripbytecountsptr = stripbytecounts;

	imTiffWriteDir[IMTIFF_STRIPBYTECOUNTS_INDEX].tdir_count = y;

	imTiffWriteDir[IMTIFF_STRIPBYTECOUNTS_INDEX].tdir_offset = ImTell( ioType, fd, fp );

	if (ImBinWrite( ioType, fd, fp, stripbytecounts, UINT, 4, y ) == -1)

	{

		ImReturnBinError( );

	}





	ImCalloc( stripoffsets,  unsigned int *,  sizeof(uint), y );

	stripoffsetsptr = stripoffsets;

	imTiffWriteDir[IMTIFF_STRIPOFFSETS_INDEX].tdir_count = y;

	imTiffWriteDir[IMTIFF_STRIPOFFSETS_INDEX].tdir_offset = ImTell( ioType, fd, fp );

	if (ImBinWrite( ioType, fd, fp, stripoffsets, UINT, 4, y ) == -1)

	{

		ImReturnBinError( );

	}



	/*

	 * Write out x and y image offset.  Always zero for now

	 */



	imTiffWriteDir[IMTIFF_XPOSITION_INDEX].tdir_offset = 

						ImTell( ioType, fd, fp );

/*

 * We need to fix the binary IO package to do doubles

 * In the mean time just write out two ints.  x &y offset are

 * both zero until we figure out how to put them in the tab table

 *

 */

	if (ImBinWrite( ioType, fd, fp, &xoffset, DOUBLE, 8, 1 ) == -1)

	{

		ImReturnBinError( );

	}



/*

	if (ImBinWrite( ioType, fd, fp, &xoffset, INT, 4, 1 ) == -1)

	{

		ImReturnBinError( );

	}

	if (ImBinWrite( ioType, fd, fp, &xoffset, INT, 4, 1 ) == -1)

	{

		ImReturnBinError( );

	}

*/



	imTiffWriteDir[IMTIFF_YPOSITION_INDEX].tdir_offset = 

						ImTell( ioType, fd, fp );



/*

	if (ImBinWrite( ioType, fd, fp, &yoffset, INT, 4, 1 ) == -1)

	{

		ImReturnBinError( );

	}

	if (ImBinWrite( ioType, fd, fp, &yoffset, INT, 4, 1 ) == -1)

	{

		ImReturnBinError( );

	}

*/

	if (ImBinWrite( ioType, fd, fp, &yoffset, DOUBLE, 8, 1 ) == -1)

	{

		ImReturnBinError( );

	}



	/*

	 * Get ready to write out some pixel values

	 */

	if ( d == 1 )

		cnt = sx = (x + (IMTIFFBITSPERBYTE-1)) / IMTIFFBITSPERBYTE;

	else

		cnt = sx = x * d / IMTIFFBITSPERBYTE;   



	ImMalloc( buffer, unsigned char *, sx );



	if ( compress != IM_COMPRESSION_NONE )

		ImMalloc( pbuffer, unsigned char *, sx+1 );



	switch( compress )

	{

		case IM_COMPRESSION_LZW:

			ImInfo("Compression Type","Lempel-Ziv and Welch (LZW)");

			break;

		case IM_COMPRESSION_PACKBITS:

			ImInfo("Compression Type","Apple Macintosh Packbits" );

			break;

		case IM_COMPRESSION_NONE:

			ImInfo( "Compression Type", "none" );

			break;

	}

	if ( (fields & IMVFBRGB) && (fields & IMVFBALPHA) && alphaRequest == IMALPHAYES )

	{

		ImInfo( "Alpha Channel", "8-bit\n" );

	}

	else

		ImInfo( "Alpha Channel", "none\n" );





	/*

	 * For every scanline

	 */

	for( j=0; j<y; j++ )

	{



		/*

	 	 * Walk through each of the scanlines

		 * Get pixels out of vfb and into buffer

	  	 */



		switch (d)

		{

		case 1:

			memset( buffer,0x00, sx );

			rbp = buffer;

			for( i=0; i<x; i++ )

			{

                       		bw = ImVfbQMono( vfb, pptr ); 

				shift=IMTIFFBITSPERBYTE-(i%IMTIFFBITSPERBYTE)-1;

                       		*rbp = *rbp | ((bw&0x1) << shift); 

				if ( shift == 0 )

					rbp++;

                        	ImVfbSInc( vfb, pptr );          

			}

			break;



		case 8:

			rbp = buffer;

			for( i=0; i<x; i++ )

			{

                       		*rbp++ = ImVfbQIndex8( vfb, pptr ); 

                        	ImVfbSInc( vfb, pptr );          

			}

			break;



		case 24:

			rbp = buffer;

			for( i=0; i<x; i++ )

			{

                       		*rbp++ = ImVfbQRed( vfb, pptr ); 

                       		*rbp++ = ImVfbQGreen( vfb, pptr ); 

                       		*rbp++ = ImVfbQBlue( vfb, pptr ); 

                        	ImVfbSInc( vfb, pptr );          

			}

			break;



		case 32:

			rbp = buffer;

			for( i=0; i<x; i++ )

			{

                       		*rbp++ = ImVfbQRed( vfb, pptr ); 

                       		*rbp++ = ImVfbQGreen( vfb, pptr ); 

                       		*rbp++ = ImVfbQBlue( vfb, pptr ); 

                       		*rbp++ = ImVfbQAlpha( vfb, pptr ); 

                       		ImVfbSInc( vfb, pptr );          

			}

			break;



		case 16:

			rbp = buffer;

			for( i=0; i<x; i++ )

			{

                       		tmp = ImVfbQIndex16( vfb, pptr ); 

                       		ImVfbSInc( vfb, pptr );          



				/* Guarantee MBF byte order.		*/

				*rbp++ = (tmp>>8) & 0xFF;

				*rbp++ = tmp & 0xFF;

			}

			break;



		default:

			ImErrNo = IMEDEPTH;

			sprintf( errstr,"Can't write %d bit image files",d);

			ImErrorFatal( errstr, -1, ImErrNo );

		}



		/*

		 * Only write scanlines starting on word boundaries

		 */

		offset = ImTell( ioType, fd, fp );

		offset += 4 - (offset % 4);

		ImSeek( ioType, fd, fp, offset, 0 );



		if ( compress == IM_COMPRESSION_LZW )

		{

			ImLzwPreEncode( sx );



			if ( ImLzwEncode( ioType, fd, fp, buffer, sx ) == -1 )

				return( -1 );  	/* ImErrNo already set	*/



			if ( ( cnt = ImLzwPostEncode( ioType, fd, fp )) == -1 )

				return( -1 );  	/* ImErrNo already set	*/



		}

		else if ( compress == IM_COMPRESSION_PACKBITS )

		{

			cnt = sx;

			PackBits( buffer, pbuffer, (unsigned int*)&cnt );



			if (ImBinWrite( ioType, fd, fp, pbuffer, 

					UCHAR, 1, cnt ) == -1)

			{

				ImReturnBinError( );

			}

		}

		else

		{

			if (ImBinWrite(ioType,fd,fp,buffer, UCHAR, 1, cnt)== -1)

			{

				ImReturnBinError( );

			}

		}



		*stripbytecountsptr++ = cnt;

		*stripoffsetsptr++ = offset;

	}



#ifdef DEBUG

	imPrintTiffWriteDir( "After", stripbytecounts, stripoffsets );

#endif



	if ( compress == IM_COMPRESSION_LZW )

		ImLzwCleanup( );



	/*

	 * Seek back and write out the offset of the new direntry

	 */

	newdiroffset  = ImTell( ioType, fd, fp );

	newdiroffset += 4 - (newdiroffset % 4);



	ImSeek( ioType, fd, fp, diroffsetposition, 0 );

	if (ImBinWrite( ioType, fd, fp, &newdiroffset, UINT, 4, 1 ) == -1)

	{

		ImReturnBinError( );

	}





	/*

	 * Seek to the stripoffset location and write out some real values

	 */

	ImSeek( ioType, fd, fp, 

		imTiffWriteDir[IMTIFF_STRIPOFFSETS_INDEX].tdir_offset, 0 );

	if (ImBinWrite( ioType, fd, fp, stripoffsets, UINT, 4, y ) == -1)

	{

		ImReturnBinError( );

	}





	/*

	 * Seek to the stripbytecounts location and write out some real values

	 */

	ImSeek( ioType, fd, fp, 

		imTiffWriteDir[IMTIFF_STRIPBYTECOUNTS_INDEX].tdir_offset, 0 );

	if (ImBinWrite( ioType, fd, fp, stripbytecounts, UINT, 4, y ) == -1)

	{

		ImReturnBinError( );

	}





	/*

	 * Seek to the new dir location and write out the dir entries

	 */

	ImSeek( ioType, fd, fp, newdiroffset, 0 );



	if ( cltRequest == IMCLTNO || clt == IMCLTNULL )

		sh = IMTIFFWRITEDIRENTRIES - 1;

	else

		sh = IMTIFFWRITEDIRENTRIES;



	if (ImBinWrite( ioType, fd, fp, &sh, USHORT, 2, 1 ) == -1)

	{

		ImReturnBinError( );

	}



	/*

	 * Write out all of the directory entries

	 */

	for( i=0; i<sh; i++ )

	{

		if ( ImBinWriteStruct( ioType, fd, fp, &(imTiffWriteDir[i]), 

			imTiffDirFields) == -1 )

		{

			ImReturnBinError( );

		}



		/*

		 * Write the offset field separately since it can contain one

		 * of three different types of variables.

		 * Check to see if the offset field contains an offset.

		 * If it does, then write it as an int and continue

		 */

        	size = imTiffWriteDir[i].tdir_count * 

				imTiffDataWidth[imTiffWriteDir[i].tdir_type];



		if ( size > 4 )

		{

			if ( ImBinWrite( ioType, fd, fp, 

			   &(imTiffWriteDir[i].tdir_offset), INT, 4, 1) == -1 )

				ImReturnBinError();

			continue;

		}



		/*

		 * If it is not an offset, then there is data in the

		 * offset field.  We have to read it as the correct type

		 */

		switch( imTiffWriteDir[i].tdir_type )

		{

		case IMTIFFBYTE:

		case IMTIFFASCII:

			status = ImBinWrite( ioType, fd, fp, 

				&(imTiffWriteDir[i].tdir_offset), UCHAR, 1, 4);

			break;



		case IMTIFFSHORT:

			so[0]=(imTiffWriteDir[i].tdir_offset & 0xffff0000) >>16;

			so[1]=imTiffWriteDir[i].tdir_offset & 0xffff;

			status = ImBinWrite( ioType, fd, fp, so, SHORT, 2, 2);

			break;



		case IMTIFFLONG:

			status = ImBinWrite( ioType, fd, fp, 

				&(imTiffWriteDir[i].tdir_offset), INT, 4, 1);

			break;

		}



		if ( status == -1 )

			ImReturnBinError( );



	}



	/*

	 * The file pointer is currently positioned at the place where the

	 * "offset-to-next-directory" value goes.  The next call to this

	 * routine (imTiffVfbWrite()) calls ImTell() to get this offset.

	 */

	free( buffer );

	if ( pbuffer )

		free( pbuffer );

	return( 0 );

}





#else /* from #ifndef USE_TIFF_LIB */



/*

 *

 * Code to use the tiff library routines starts here.

 *

 */





/*

 * FUNCTION

 *	imTiffRead

 *

 * DESCRIPTION

 *	Read in a tiff file using the routines in the tiff library.

 *

 */



static int				/* Returns status		*/

#ifdef __STDC__

imTiffRead( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable )

#else

imTiffRead( ioType, fd, fp, flags, tagTable )

	int              ioType;	/* I/O flags			*/

	int	         fd;		/* Input file descriptor	*/

	FILE	        *fp;		/* Input file pointer		*/

	TagTable        *flags;		/* Format flags			*/

	TagTable        *tagTable;	/* Tag table to add to		*/

#endif

{ 

 TIFF* tif;			/* Structure for manipulating TIFF image.   */

 ImVfb* vfb;			/* Vfb we will be creating.                 */

 uint32 xSize, ySize;		/* Image Size (in bytes) for allocating vfb */

 int fieldMask;			/* Fields in the Vfb.                       */

 uint16 numchans;	        /* number of channels                       */

 uint16 chandepth;    	  	/* number of bits per channel               */

 uint16 interleave_method; 	/* method of interleaving.                  */

 uint16 photometric;            /* indicates color index / rgb / b&w        */

 ImClt* cltBuf;                 /* Color lookup table                       */

 uint16 byteOrder;              /* byte order in file (just for ImInfo)     */ 

 uint16 compression;            /* compression method (just for ImInfo)     */ 

 char* filename;                /* filename                                 */

 char message[500]; 		/* message buffer                           */

 int curImage;                  /* Index of current image in the file       */

 int moreImages;                /* Flag for the while loop                  */

 TagEntry* entry;               /* Entry to get file name                   */



 /*

  * We need a file descriptor.  So, if we have a 

  * file pointer, get a file descriptor

  * for it.

  */



 if (ioType & IMFILEIOFP)

 {

	fd = fileno(fp);

	fflush(fp);

	rewind(fp);

 }





 /* 

  * If we can't discern the filename, use 'libtiff'.  This way, error messages

  * will say things like 'libtiff: Can't open file'.

  */



 if (!flags || TagTableQNEntry (flags, "file name") == 0)

 {

	filename = NULL;

 }

 else

 {

	entry = TagTableQDirect ( flags, "file name", 0 );

	if (TagEntryQValue( entry, &filename) == -1)

		filename = NULL;

 }





 tif = TIFFFdOpen( fd, filename ? filename : "libtiff", "rb"); 

 if (tif == IMTIFFNULL)

 {

	ImErrorFatal( "Error opening file.", -1, IMENOFILE);

 }





 moreImages = 1;

 curImage = 0;



 /* Loop through the images in the file */



 while (moreImages)

 {

	curImage++;

	sprintf( message, "%d", curImage );

	ImInfo( "Image", message );



	 /*

	  * Figure out how big the image is and what fields it contains.

	  * Then allocate our vfb accordingly.

	  */



	 /* Get the image size */



	 TIFFGetField( tif, TIFFTAG_IMAGEWIDTH, &xSize); 

	 TIFFGetField( tif, TIFFTAG_IMAGELENGTH, &ySize); 

	 sprintf(message,"%d x %d",xSize,ySize);

	 ImInfo( "Resolution", message);



	 /* Get the image type */



	 TIFFGetField( tif, TIFFTAG_PHOTOMETRIC, &photometric);

	 TIFFGetFieldDefaulted(tif, TIFFTAG_BITSPERSAMPLE,   &chandepth);

	 TIFFGetFieldDefaulted(tif, TIFFTAG_SAMPLESPERPIXEL, &numchans);



	 /* 

	  * Translate tiff's idea of an image type into the image tools'

	  * idea of an image type

	  */



	 fieldMask = 0;

	 switch (numchans)

	 {

		case 1:	/* Greyscale or color-indexed */

			if (chandepth<=8)

			{

				sprintf(message,"%d-bit %s", chandepth,

					photometric==PHOTOMETRIC_PALETTE ? "Color Indexed" : "Greyscale" );

				fieldMask = IMVFBINDEX8;

				ImInfo ( "Type", message);

				if (photometric==PHOTOMETRIC_MINISWHITE)

					ImInfo("Greyscale Mode", "Minimum is white");

				if (photometric==PHOTOMETRIC_MINISBLACK)

					ImInfo("Greyscale Mode", "Minimum is black");

			}

			else

			if (chandepth==16)

			{

				fieldMask = IMVFBINDEX16;

				ImInfo ("Type", "16-bit Color indexed");

			}

			break;



		case 3: /* RGB with no alpha */

			fieldMask = IMVFBRGB;

			sprintf(message,"%d-bit RGB", chandepth * numchans);

			ImInfo ("Type", message);

			break;



		case 4: /* RGB with alpha    */

			fieldMask = IMVFBRGB | IMVFBALPHA;

			break;

		default: break;

	 } /* End of switch */



	 if (fieldMask==0)

	 {

		sprintf(message,"Cannot handle images with %d channels, and depth %d",

			numchans, chandepth);

		ImErrorFatal( message, -1, IMEUNSUPPORTED);

	 }



	 /*

	  * No support for the following.

	  */



	 if (photometric==PHOTOMETRIC_YCBCR)

	 {

		 ImErrorFatal( "No support for YCBCR photometric interepretation",-1,

			IMEUNSUPPORTED);

	 }



	 /* Allocate our vfb */



	 vfb = ImVfbAlloc( xSize, ySize, fieldMask );

	 if ( (vfb = ImVfbAlloc( xSize, ySize, fieldMask )) == IMVFBNULL)

	 {

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	 }



	 /* Discern the byte order for ImInfo message */



	 TIFFGetFieldDefaulted(tif, TIFFTAG_FILLORDER, &byteOrder);



	 if (byteOrder==FILLORDER_MSB2LSB)

		ImInfo( "Byte Order", "Most Significant Byte First")

	 else

		ImInfo( "Byte Order", "Least Significant Byte First")



	 /* Discern the compression scheme */



	 TIFFGetFieldDefaulted(tif, TIFFTAG_COMPRESSION, &compression);



	 switch (compression)

	 {

		case COMPRESSION_NONE:

			ImInfo ("Compression Type", "none");

			break;

		case COMPRESSION_CCITTRLE:

			ImInfo ("Compression Type", "CCITT modified Huffman encoding");

			break;

		case COMPRESSION_CCITTFAX3:

			ImInfo ("Compression Type", "CCITT Group 3 facsimile encoding");

			break;

		case COMPRESSION_CCITTFAX4:

			ImInfo ("Compression Type", "CCITT Group 4 facsimile encoding");

			break;

		case COMPRESSION_CCITTRLEW:

			ImInfo ("Compression Type", "CCITT modified Huffman encoding w/ word alignment");

			break;

		case COMPRESSION_PACKBITS:

			ImInfo ("Compression Type", "Macintosh PackBits encoding");

			break;

		case COMPRESSION_THUNDERSCAN:

			ImInfo ("Compression Type", "ThunderScan 4-bit encoding");

			break;

		case COMPRESSION_LZW:

			ImInfo ("Compression Type", "Lempel-Ziv & Welch encoding");

			break;

		case COMPRESSION_NEXT:

			ImInfo ("Compression Type", "NeXT 2-bit encoding");

			break;

		case COMPRESSION_JPEG:

			ImInfo ("Compression Type", "JPEG encoding");

			break;

		default:

			ImInfo ("Compression Type", "Unknown");

			break;

	 }



	 /*

	  *

	  *  Determine the method of interleaving, and grouping.

	  *  Then call the appropriate subroutine to decode the pixel

	  *  data.

	  *

	  */



	 if (TIFFIsTiled(tif))

		ImInfo( "Grouping Method", "Tiles" ) 

	 else

		ImInfo( "Grouping Method", "Scanlines" )



	 TIFFGetFieldDefaulted(tif, TIFFTAG_PLANARCONFIG, &interleave_method);



	 switch (interleave_method)

	 {

		case PLANARCONFIG_CONTIG : /* contiguous (noninterleaved) */



			ImInfo( "Interleave Method", "Non-interleaved" );

			if (TIFFIsTiled(tif))

				imTiffReadNonInterleavedTiles (tif, vfb);

			else

				imTiffReadNonInterleavedScanlines( tif, vfb);

			break;	



		case PLANARCONFIG_SEPARATE : /* plane interleaved */



			ImInfo( "Interleave Method", "Plane Interleaved" );

			if (TIFFIsTiled (tif))

				imTiffReadPlaneInterleavedTiles ( tif, vfb);

			else

				imTiffReadPlaneInterleavedScanlines ( tif, vfb); 

			break;



		default :

		 	ImErrorFatal( "Unknown interleave method.",-1, IMEUNSUPPORTED);

	 }



	 /*

	  *  If there is a clt, read it in.

	  */



	 if (photometric == PHOTOMETRIC_PALETTE)

	 {

		sprintf(message,"%d-bit", chandepth);

		ImInfo ("Color Table", message );

		if (imTiffReadClt(tif, &cltBuf) != 0) 

		{

			ImVfbSClt (vfb, cltBuf);

			TagTableAppend( tagTable,

				TagEntryAlloc( "image clt", POINTER, &cltBuf ) );

		} 

		/* else error is reported by imTiffReadClt */

	 }

	 else

		ImInfo ("Color Table", "none");



	 /*

	  * Append the vfb to the tagtable

	  */



	 TagTableAppend( tagTable,

		TagEntryAlloc( "image vfb", POINTER, &vfb ) );



	moreImages = TIFFReadDirectory(tif);

 }   /* End of while moreimages */



 TIFFClose( tif );



 return curImage;

}





/*

 * Macro to scale value to be between 0 and 255 

 */



#define IM_SCALE_0_255(currentValue,chandepth)  ( ((currentValue) * 255) / ( (1L << (chandepth)) - 1))







/*

 *

 *  FUNCTION

 *    imTiffReadNonInterleavedScanlines

 *

 *  DESCRIPTION

 *    Read data into a vfb, from a TIFF structure. 

 *    The data is stored in the form RGBRGBRGB....

 *    The tiff library calls this "contiguous".  We

 *    call it "non-interleaved".

 *

 *  RETURN VALUE

 *    1 if everything went smoothly.

 *    0 otherwise.

 *

 */



static int

#ifdef __STDC__

imTiffReadNonInterleavedScanlines( TIFF* tif, ImVfb* vfb) 

#else

imTiffReadNonInterleavedScanlines( tif, vfb) 

TIFF* tif;   	/* incoming data */

ImVfb* vfb;     /* outgoing vfb  */

#endif

{

 unsigned char* linebuf;	/* buffer for one scanline                    */

 uint16* linebuf16;             /* the line as an array of 16-bit values      */

 tsize_t scanlineSize;  	/* size of scanlines 			      */

 ImVfbPtr vfbptr;       	/* ptr into a vfb                             */

 uint32 row, col;		/* Loop indices                               */

 uint32 ySize;			/* Image size (y)	                      */

 uint16 numchans;               /* number of channels                         */

 uint16 chandepth; 	 	/* number of bits per channel                 */

 uint16 mask;                   /* mask to read pixels from the array         */

 uint16 startmask;              /* start with this mask                       */

 uint16 currentValue;           /* value of current channel of current pixel  */

 int   sampleNum;               /* Index of the current value in the scanline */

 int samplesPerScanline;        /* number of samples per scanline.            */

 int chanIndex;                 /* which channel is being set                 */

				/* (index in the chanList array)              */

 int index16;			/* index within 16-bit array                  */

 int chanList[5];               /* List of channels to cycle through          */

 uint16 photometric;            /* Tells us the type of image this is.        */

 int currentShift;              /* How many bits to shift this sample         */

 char message[200];             /* Buffer for error messages                  */

 int isGreyscale;               /* flag indicating greyscale image (vs clt)   */

 uint16 compression;		/* Compression type                           */

 int byteOrder;  		/* indicates byte order                       */



 /* Get some important fields from the tiff structure. */



 TIFFGetFieldDefaulted(tif, TIFFTAG_BITSPERSAMPLE,   &chandepth);

 TIFFGetFieldDefaulted(tif, TIFFTAG_SAMPLESPERPIXEL, &numchans);

 TIFFGetField( tif, TIFFTAG_IMAGELENGTH, &ySize); 

 TIFFGetField( tif, TIFFTAG_PHOTOMETRIC, &photometric);

 TIFFGetFieldDefaulted(tif, TIFFTAG_COMPRESSION, &compression);

 scanlineSize       = TIFFScanlineSize(tif);

 samplesPerScanline = scanlineSize * 8 / chandepth;

 byteOrder = BinQMachine()->bin_byteOrder;



 /* 

  * According to page 17 of the TIFF 6.0 specification, grayscale images that

  * are compressed with certain compression schemes should be reversed.

  */



 if ( (compression==COMPRESSION_CCITTRLE) ||

	(compression==COMPRESSION_CCITTFAX3) ||

	(compression==COMPRESSION_CCITTRLEW) ||

	(compression==COMPRESSION_CCITTFAX4) )

	{

	/* Switch the photometric if it is a greyscale image */

		if (photometric==PHOTOMETRIC_MINISBLACK)

			photometric = PHOTOMETRIC_MINISWHITE;

		else if (photometric==PHOTOMETRIC_MINISWHITE)

			photometric = PHOTOMETRIC_MINISBLACK;

	}



 /*

  * Here is what's going on here:

  *

  * The scanlines are read one at a time.

  *

  * For each scanline...

  * 

  * We need to extract values from the scanline that are not necessarily 

  * 8-bits (in particular, that are 1,2,4,8 or 16 bits).

  *

  * To do this, we cast the array of values into an array of 16-bit values,

  * and then use a bit-mask, in conjunction with an array index, to traverse

  * the array.

  *

  * i.e.

  *    array:   +----+----+----+----+ ...

  *             |    |    |    |    |

  *             +----+----+----+----+ ...

  *    mask:     1100 

  *    index16:   0

  * 

  * when index16 is 0, and the mask masks the upper 4 bits of a uint16, then

  * we are looking at the value in the first 4 bits of the first array element.

  *

  * When index16 is 1, we are looking at the first 4 bits of the second array element.

  *  et cetera

  *

  * This allows us to extract values from the scanline.  Then, for each value, we 

  * determine which channel we are on by using an array containing the channels through

  * which we are looping.

  *

  * This will allow us to read a scanline of the form

  *    RGBRGBRGBRGB

  *

  *  where each R is only 2 bits long.  

  *  (and each G is only 2 bits long and each B is only 2 bits long)

  */



  /* Set up the original mask (Mask the leftmost chandepth bits) */



 mask = (uint16) 0;

 switch (chandepth)

 {

 	case 16 : mask = mask | (1<<0);  

		  mask = mask | (1<<1);

		  mask = mask | (1<<2);

		  mask = mask | (1<<3);

		  mask = mask | (1<<4);

		  mask = mask | (1<<5);

		  mask = mask | (1<<6);

		  mask = mask | (1<<7);   /* fall through */

	case  8 : mask = mask | (1<<8);

		  mask = mask | (1<<9);

		  mask = mask | (1<<10);

		  mask = mask | (1<<11);   /* fall through */

	case 4  : mask = mask | (1<<12);

		  mask = mask | (1<<13);   /* fall through */

	case 2  : mask = mask | (1<<14);   /* fall through */

	case 1  : mask = mask | (1<<15);

		  break;

	default :

		sprintf(message,"Unsupported channel depth.  No support for %d bits per sample",chandepth);

		ImErrorWarning(message,0,IMEUNSUPPORTED);

		return 0;

 }

 startmask = mask;



 /*

  *   Set up the list of channels through which we are going to cycle.

  */



 switch (numchans)

 {

 	case 1 : 

		/* 

		 * Determine whether this is a greyscale image or a color

		 * indexed image.  (So that we know whether or not to scale

		 * the incoming value to be between 0 and 255.

		 */



		 if (photometric!=PHOTOMETRIC_PALETTE)

			isGreyscale = 1;

		 else

			isGreyscale = 0;

		 if (chandepth==16)

			 chanList[0] = IMVFBINDEX16;

		 else

			 chanList[0] = IMVFBINDEX8;

		 break;

	case 3 : 

		 chanList[0] = IMVFBRED;

		 chanList[1] = IMVFBGREEN;

		 chanList[2] = IMVFBBLUE;

		 break;

	case 4 : 

		 chanList[0] = IMVFBRED;

		 chanList[1] = IMVFBGREEN;

		 chanList[2] = IMVFBBLUE;

		 chanList[3] = IMVFBALPHA;

		 break;

	default : 

		sprintf(message,"Unsupported channel depth (bits per sample) : %d",chandepth);

		ImErrorWarning(message,0,IMEUNSUPPORTED);

 }



 /*

  * currentShift tells us how many bits to shift the value

  * to the right, after we get it from the array.

  * i.e. after using mask to collect the upper 2 bits of 

  * a uint16, we need to shift the value 14 bits to the right

  * to get our final value.

  */

 currentShift = 16 - chandepth;



 /* Allocate space and cast our array into a 16-bit one. */



 ImMalloc(linebuf,unsigned char *,scanlineSize);

 linebuf16 = (uint16 *) linebuf;



 for (row=0; row< ySize ; row++)

 {

	/* Read a scanline */



	if (TIFFReadScanline(tif, linebuf, row, 0) < 0)

		break;

	if (byteOrder==BINLBF)

		imTiffSwapBytes(linebuf, samplesPerScanline);





	/* 

	 * mask and index16 start at the upper chandepth 

	 * bits of the first array element 

	 */



	index16      = 0;

	mask         = startmask;



	/*

	 * start with the first channel (i.e. red for RGB, 

	 * INDEX8 for indexed images ... 

	 */



	chanIndex    = 0;



	/* Keep track of how many samples we've traversed (so far 0) */



	sampleNum    = 0;



	/* currentShift's function is explained above */



	currentShift = 16 - chandepth;



	/* move to the beginning of the row */



	vfbptr       = ImVfbQPtr(vfb, 0, row);



	while (sampleNum < samplesPerScanline)

	{

		/*

		 * index16 is the index relative to the 16-bit array. 

		 * mask is the mask within the current array element

		 *  (i.e. the index relative to the current 16-bit value)

		 * sampleNum is the index of the sample in the scanline.

		 *

		 */



		currentValue = (mask & *(linebuf16 + index16));	

		currentValue = currentValue >> currentShift;

		currentShift -= chandepth;

		if (currentShift < 0)

			currentShift = 16 - chandepth;



		/* Is 0 black or white? */

		if (photometric == PHOTOMETRIC_MINISWHITE)

		{

			currentValue = ~currentValue;

			/* Knock off upper (16 - chandepth) bits */

			currentValue = currentValue << (16 - chandepth);

			currentValue = currentValue >> (16 - chandepth);

		}



		

		switch (chanList[chanIndex])

		{

			case IMVFBINDEX8 : /* same as IMVFBGREY */

				if (isGreyscale)

					ImVfbSGrey(vfb, vfbptr, IM_SCALE_0_255(currentValue,chandepth));

				else

					ImVfbSIndex8 (vfb, vfbptr, currentValue);

				break;

			case IMVFBRED  :

				ImVfbSRed  (vfb, vfbptr, IM_SCALE_0_255(currentValue,chandepth) );

				break;

			case IMVFBGREEN :

				ImVfbSGreen(vfb, vfbptr, IM_SCALE_0_255(currentValue,chandepth) );

				break;

			case IMVFBBLUE  :

				ImVfbSBlue (vfb, vfbptr, IM_SCALE_0_255(currentValue,chandepth) );

				break;

			case IMVFBALPHA :

				ImVfbSAlpha (vfb, vfbptr, IM_SCALE_0_255(currentValue,chandepth) );

				break;

			case IMVFBINDEX16 :

				ImVfbSIndex16 (vfb, vfbptr, currentValue );

				break;

		}



		/*

		 * Increment: the channel index, the sample number,

		 * the mask, possibly the vfb pointer, and possibly 

		 * the index in the scanline array.

		 */



		chanIndex++;

		if (chanIndex > (numchans - 1))

		{

			ImVfbSNext (vfb, vfbptr);

			chanIndex = 0;

		}

		sampleNum++;



		/* Shift mask and possibly index16 */

		mask = mask>>chandepth;

		if (mask == 0)

		{

			mask = startmask;

			index16++;

		}

	}

 }

 free ((unsigned char *) linebuf);



 return 1;

}











/*

 *

 *  FUNCTION

 *    imTiffReadPlaneInterleavedScanlines

 *

 *  DESCRIPTION

 *    Read data into a vfb, from a TIFF structure. 

 *    The data is stored in the form RRR...GGG...BBB....B

 *    The tiff library calls this "seperate".  We

 *    call it "plane-interleaved".

 *

 */



static int	/* returns 1 for success, 0 for failure */

#ifdef __STDC__

imTiffReadPlaneInterleavedScanlines( TIFF* tif, ImVfb* vfb) 

#else

imTiffReadPlaneInterleavedScanlines( tif, vfb) 

TIFF* tif;   	/* incoming data */

ImVfb* vfb;     /* outgoing vfb  */

#endif

{

 uint16 numchans;               /* number of channels    */

 uint16 chandepth;              /* Depth of each channel */

 char message[200];		/* Buffer for messages   */



 /*

  * Get the number of channels and channel depth from tiff

  */



 TIFFGetFieldDefaulted(tif, TIFFTAG_SAMPLESPERPIXEL, &numchans);

 TIFFGetFieldDefaulted(tif, TIFFTAG_BITSPERSAMPLE, &chandepth);



 /*

  * Go through the vfb once for each channel, and

  * read the appropriate plane from the TIFF structure.

  */



 switch (numchans)

 {

	case 1:  

		/* 

		 * Being plane interleaved and having only one 

		 * plane is silly.  It's the same as being non-interleaved.

		 * Nevertheless, tiff's flags can indicate such a thing

		 * occurring.

		 * So, let's just call the non-interleaved function.

		 * (which handles varying channel depths.)

		 */



		imTiffReadNonInterleavedScanlines(tif, vfb); 

		break;

	case 3:

		if (chandepth!=8)

		{

			sprintf(message,"Can't read %d bits per channel plane-interleaved images\n",chandepth);

			ImErrorFatal(message,0,IMEUNSUPPORTED);

		}

		imTiffReadRedScanlinePlane(tif, vfb);

		imTiffReadGreenScanlinePlane(tif, vfb);

		imTiffReadBlueScanlinePlane(tif, vfb);

		break;

	case 4:

		if (chandepth!=8)

		{

			sprintf(message,"Can't read %d bits per channel plane-interleaved images\n",chandepth);

			ImErrorFatal(message,0,IMEUNSUPPORTED);

		}

		imTiffReadRedScanlinePlane(tif, vfb);

		imTiffReadGreenScanlinePlane(tif, vfb);

		imTiffReadBlueScanlinePlane(tif, vfb);

		imTiffReadAlphaScanlinePlane(tif, vfb);

		break;

	default : 

		sprintf(message,"Can't read %d channels plane-interleaved\n",numchans);

		ImErrorFatal(message,0,IMEUNSUPPORTED);

 }



 return 1;

}



/*

 *

 * FUNCTION

 *   imTiffReadClt

 *

 * DESCRIPTION

 *   Allocate space for, and then read a clt from a tiff file.

 *

 */



static int 	/* returns 1 for success, 0 for failure */

#ifdef __STDC__

imTiffReadClt(TIFF* tif, ImClt **cltBuf)

#else

imTiffReadClt(tif, cltBuf)

TIFF* tif;		/* TIFF structure     */

ImClt **cltBuf;         /* buffer for new clt */

#endif

{

	int nClt;         /* Number of entries in the clt       */

        uint16 *redmap,   /* Red color table entry components   */

	       *greenmap, /* Green color table entry components */

	       *bluemap;  /* Blue color table entry components  */

        uint16 chandepth; /* Depth of each channel              */

	ImCltPtr cltptr;  /* Pointer into a clt                 */

	int i;            /* loop index                         */



	/*

	 * Read in the table

	 */



	if (!TIFFGetField(tif, TIFFTAG_COLORMAP, &redmap, &greenmap, &bluemap))

	{	

		ImErrorFatal("No color table for image",0,IMENOCLT);

	}



	/* Compute number of entries in the clt. */

	

	TIFFGetFieldDefaulted(tif, TIFFTAG_BITSPERSAMPLE, &chandepth);

	nClt = 1<<chandepth;



 	/* Allocate a new clt */

	if ( (*cltBuf = ImCltAlloc(nClt)) == IMCLTNULL)

	{

		ImErrorFatal ( ImQError(), 0, ImErrNo );

	}



	cltptr = ImCltQFirst (*cltBuf);



	/* Copy stuff into the clt */

	for (i=0;i < nClt; i++)

	{

		ImCltSRed   (cltptr, (redmap[i])   );

		ImCltSGreen (cltptr, (greenmap[i]) );

		ImCltSBlue  (cltptr, (bluemap[i])  );

		ImCltSInc   (*clrBuf, cltptr);

	}

	return 1;

}





 /*

  * FUNCTION

  *

  *   imTiffReadNonInterleavedTiles

  *

  * DESCRIPTION

  *

  *   Read the data in a tif file into a vfb, where the storage

  * method is non-interleaved tiles.  (i.e. RGBRGBRGB.... organized into

  * tiles.)

  *

  */



static int 	/* returns 1 for success, 0 for failure */

#ifdef __STDC__

imTiffReadNonInterleavedTiles (TIFF *tif, ImVfb *vfb)

#else

imTiffReadNonInterleavedTiles(tif, vfb)

TIFF* tif;

ImVfb* vfb;

#endif

{

 unsigned char* tilebuf;	/* buffer for one scanline                    */

 uint16* tilebuf16;             /* the line as an array of 16-bit values      */

 tsize_t tileSize;  		/* size of tiles 			      */

 ImVfbPtr vfbptr;       	/* ptr into a vfb                             */

 int x, y, index16;          	/* Loop indices                               */

 uint32 imageWidth,

	imageLength;		/* Image size (y)	                      */

 uint16 numchans;               /* number of channels                         */

 uint16 chandepth; 	 	/* number of bits per channel                 */

 uint16 mask;                   /* mask to read pixels from the array         */

 uint16 startmask;              /* start with this mask                       */

 uint16 currentValue;           /* value of current channel of current pixel  */

 int   sampleNum;               /* Index of the current value in the tile     */

 int samplesPerTile;            /* number of samples per scanline.            */

 int chanIndex;                 /* which channel is being set                 */

				/* (index in the chanList array)              */

 int chanList[5];               /* List of channels to cycle through          */

 uint16 photometric;            /* Tells us the type of image this is.        */

 int currentShift;              /* How many bits to shift this sample         */

 uint32 tileWidth,              /* Width of each tile (must be multiple of 16)*/

	tileLength;		/* Length of each tile (  "         "   )     */

 int pixelNum;                  /* x-coordinate of pixel within this tile     */

 int pixelsInThisTile;   	/* Number of pixels in this tile              */

 char message[200];             /* Buffer for error messages                  */

 int isGreyscale;               /* flag indicating greyscale image (vs clt)   */

 uint16 compression;		/* Compression type                           */

 int vfbX, vfbY;   		/* index relative to the entire vfb           */

 int byteOrder;                 /* machine's byte order                       */





 /* Get some important fields from the tiff structure. */



 TIFFGetFieldDefaulted(tif, TIFFTAG_BITSPERSAMPLE,   &chandepth);

 TIFFGetFieldDefaulted(tif, TIFFTAG_SAMPLESPERPIXEL, &numchans);

 TIFFGetFieldDefaulted(tif, TIFFTAG_COMPRESSION, &compression);

 TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &imageWidth);

 TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imageLength);

 TIFFGetField( tif, TIFFTAG_PHOTOMETRIC, &photometric);

 TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tileWidth);

 TIFFGetField(tif, TIFFTAG_TILELENGTH, &tileLength);

 tileSize       = TIFFTileSize(tif);

 samplesPerTile = tileSize * 8 / chandepth;

 byteOrder      = BinQMachine()->bin_byteOrder;



 /* 

  * According to page 17 of the TIFF 6.0 specification, grayscale images that

  * are compressed with certain compression schemes should be reversed.

  *

  */

 if ( (compression==COMPRESSION_CCITTRLE) ||

	(compression==COMPRESSION_CCITTFAX3) ||

	(compression==COMPRESSION_CCITTRLEW) ||

	(compression==COMPRESSION_CCITTFAX4) )

	{

	/* Switch the photometric if it is a greyscale image */

		if (photometric==PHOTOMETRIC_MINISBLACK)

			photometric = PHOTOMETRIC_MINISWHITE;

		else if (photometric==PHOTOMETRIC_MINISWHITE)

			photometric = PHOTOMETRIC_MINISBLACK;

	}



 /*

  * Here is what's going on here:

  *

  * The tiles are read one at a time.

  *

  * For each tile...

  * 

  * We need to extract values from the tile that are not necessarily 

  * 8-bits (in particular, that are 1,2,4,8 or 16 bits).

  *

  * To do this, we cast the array of values into an array of 16-bit values,

  * and then use a bit-mask, in conjunction with an array index, to traverse

  * the array.

  *

  * i.e.

  *    array:   +----+----+----+----+ ...

  *             |    |    |    |    |

  *             +----+----+----+----+ ...

  *    mask:     1100 

  *    index16:   0

  * 

  * when index16 is 0, and the mask masks the upper 4 bits of a uint16, then

  * we are looking at the value in the first 4 bits of the first array element.

  *

  * When index16 is 1, we are looking at the first 4 bits of the second array element.

  *  et cetera

  *

  * This allows us to extract values from the tile.  Then, for each value, we 

  * determine which channel we are on by using an array containing the channels through

  * which we are looping.

  *

  * This will allow us to read a tile of the form

  *    RGBRGBRGBRGB

  *

  *  where each R is only 2 bits long.  

  *  (and each G is only 2 bits long and each B is only 2 bits long)

  *

  * After reading a value, the correct channel for the vfbptr is set, the vfbptr

  * from from left to right, and top to bottom along each tile.

  */



  /*

   *   Note also that if we read a tile that is x by y, then there

   *   will always be x*y*number-of-channes pixels in the buffer.

   *   This means that if a tile goes beyond the boundaries of a 

   *   vfb, then it will be padded with junk.

   */



  /* Set up the original mask (Mask the leftmost chandepth bits) */



 mask = (uint16) 0;

 switch (chandepth)

 {

 	case 16 : mask = mask | (1<<0);  

		  mask = mask | (1<<1);

		  mask = mask | (1<<2);

		  mask = mask | (1<<3);

		  mask = mask | (1<<4);

		  mask = mask | (1<<5);

		  mask = mask | (1<<6);

		  mask = mask | (1<<7);   /* fall through */

	case  8 : mask = mask | (1<<8);

		  mask = mask | (1<<9);

		  mask = mask | (1<<10);

		  mask = mask | (1<<11);   /* fall through */

	case 4  : mask = mask | (1<<12);

		  mask = mask | (1<<13);   /* fall through */

	case 2  : mask = mask | (1<<14);   /* fall through */

	case 1  : mask = mask | (1<<15);

		  break;

	default :

		sprintf(message,"Unsupported channel depth (bits per sample) : %d",chandepth);

		ImErrorWarning(message,0,IMEUNSUPPORTED);

 }

 startmask = mask;



 /*

  *   Set up the list of channels through which we are going to cycle.

  */



 switch (numchans)

 {

 	 

	case 1 : 

		/* 

		 * Determine whether this is a greyscale image or a color

		 * indexed image.  (So that we know whether or not to scale

		 * the incoming value to be between 0 and 255.

		 */



		 if (photometric!=PHOTOMETRIC_PALETTE)

			isGreyscale = 1;

		 else

			isGreyscale = 0;



		 if (chandepth==16)

			 chanList[0] = IMVFBINDEX16;

		 else

			 chanList[0] = IMVFBINDEX8;

		 break;

	case 3 : 

		 chanList[0] = IMVFBRED;

		 chanList[1] = IMVFBGREEN;

		 chanList[2] = IMVFBBLUE;

		 break;

	case 4 : 

		 chanList[0] = IMVFBRED;

		 chanList[1] = IMVFBGREEN;

		 chanList[2] = IMVFBBLUE;

		 chanList[3] = IMVFBALPHA;

		 break;

	default : 

		sprintf(message,"No support for %d channels in an image.  Skipping image",numchans);

		ImErrorWarning(message,0,IMEUNSUPPORTED);

 }

 chanIndex    = 0;



 /*

  * currentShift tells us how many bits to shift the value

  * to the right, after we get it from the array.

  * i.e. after using mask to collect the upper 2 bits of 

  * a uint16, we need to shift the value 14 bits to the right

  * to get our final value.

  */

 currentShift = 16 - chandepth;



 /*

  * Allocate space for our tile buffer, and cast

  * it into an array of 16-bit values 

  */

 ImMalloc(tilebuf,unsigned char *,tileSize+1);

 tilebuf16 = (uint16 *) tilebuf;



 for (y = 0; y < imageLength ; y+=tileLength)

 {



	for (x = 0; x < imageWidth; x += tileWidth)

	{

		/* Keep track of where we are precisely with vfbX and vfbY */

		vfbY = y;

		vfbX = x;

			

		/* Read the tile! */

		if (TIFFReadTile(tif, tilebuf, x, y, 0, 0) < 0)

			break;

		if (byteOrder==BINLBF)

			imTiffSwapBytes(tilebuf, tileSize);



		/* 

		 * Set vfbptr to the upper left hand corner of where this

		 * tile goes in the image.

		 */



		vfbptr = ImVfbQPtr(vfb, x , y);



		/* Start with the first sample of the first pixel in the first array element*/

		pixelNum  = 0;

		index16   = 0;

		sampleNum = 0;



		while (vfbY < y + tileLength )

		{

			/*

			 * index16 is the index relative to the 16-bit array. 

			 * mask is the mask within the current array element

			 *  (i.e. the index relative to the current 16-bit value)

			 * sampleNum is the index of the sample in the tile.

			 */



			currentValue = (mask & tilebuf16[index16]);	

			currentValue = currentValue >> currentShift;

			currentShift -= chandepth;

			if (currentShift < 0)

				currentShift = 16 - chandepth;



			/* Is 0 black or white? */



			if (photometric == PHOTOMETRIC_MINISWHITE)

			{

				currentValue = ~currentValue;

				/* Knock off upper (16 - chandepth) bits */

				currentValue = currentValue << (16 - chandepth);

				currentValue = currentValue >> (16 - chandepth);

			}





			/*

			 *  Only set the value if we are within the vfb

			 */

			if (vfbX < imageWidth && vfbY < imageLength)

			{

				switch (chanList[chanIndex])

				{

					case IMVFBGREY :  /* same as INDEX8 */

						if (isGreyscale)

							ImVfbSGrey(vfb, vfbptr, IM_SCALE_0_255(currentValue,chandepth));

						else

							ImVfbSIndex8 (vfb, vfbptr, currentValue);

						break;

					case IMVFBRED  :

						ImVfbSRed  (vfb, vfbptr, IM_SCALE_0_255(currentValue,chandepth) );

						break;

					case IMVFBGREEN :

						ImVfbSGreen(vfb, vfbptr, IM_SCALE_0_255(currentValue,chandepth) );

						break;

					case IMVFBBLUE  :

						ImVfbSBlue (vfb, vfbptr, IM_SCALE_0_255(currentValue,chandepth) );

						break;

					case IMVFBALPHA :

						ImVfbSAlpha (vfb, vfbptr, IM_SCALE_0_255(currentValue,chandepth) );

						break;

					case IMVFBINDEX16 :

						ImVfbSIndex16 (vfb, vfbptr, currentValue );

						break;

				}

			}



			/*

			 * Increment: the channel index, the sample number,

			 * the mask, possibly the vfb pointer, and possibly 

			 * the index in the scanline array.

			 */

			

			chanIndex++;

			if (chanIndex > (numchans - 1))

			{

				/*

				 * Move right one pixel in the vfb.

				 * If we are at a boundary of the tile, then

				 * move down one line and left to y

				 */



				vfbX++;

				pixelNum++;

				if (pixelNum == tileWidth)

				{  	/* Reached the right edge of the tile */

					pixelNum = 0;

					vfbY += 1;

					vfbX = x;

					if (vfbX < imageWidth && vfbY < imageLength)

						vfbptr = ImVfbQPtr(vfb, x, vfbY);

				}

				else

				{

					/* If we're still in the vfb */

					if (vfbX < imageWidth && vfbY < imageLength)

						ImVfbSRight (vfb, vfbptr);

				}

				chanIndex = 0;

			}

			sampleNum++;



			/* Shift mask and possibly index16 */

			mask = mask>>chandepth;



			if (mask == 0)

			{

				mask = startmask;

				index16++;

			}

		}

	}

 }

 free ((unsigned char *) tilebuf);



 return 1;

}



 /*

  * FUNCTION

  *

  *   imTiffReadPlaneInterleavedTiles

  *

  * DESCRIPTION

  *

  *   Read the data in a tif file into a vfb, where the storage

  * method is non-interleaved tiles.

  *

  */



static int 	/* returns 1 for success, 0 for failure */

#ifdef __STDC__

imTiffReadPlaneInterleavedTiles ( TIFF *tif, ImVfb *vfb)

#else

imTiffReadPlaneInterleavedTiles ( tif, vfb)

TIFF* tif;

ImVfb* vfb;

#endif

{

 uint16 numchans;               /* number of channels    */

 uint16 chandepth;              /* Depth of each channel */

 char message[200];		/* Buffer for messages   */

 uint32 tileWidth, tileLength;  /* size of tiles         */

 uint32 imageWidth, imageLength;/* size of image         */

 tsize_t tileSize;              /* # of bytes per tile   */



 /*

  * Go through the vfb once for each channel.

  * Call the appropriate read function for each channel.

  */



 /*

  * Get the number of channels and channel depth from tiff

  */



 TIFFGetFieldDefaulted(tif, TIFFTAG_SAMPLESPERPIXEL, &numchans);

 TIFFGetFieldDefaulted(tif, TIFFTAG_BITSPERSAMPLE, &chandepth);

 tileSize = TIFFTileSize(tif);

 TIFFGetField(tif, TIFFTAG_IMAGEWIDTH,  &imageWidth );

 TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imageLength);

 TIFFGetField(tif, TIFFTAG_TILEWIDTH,   &tileWidth  );

 TIFFGetField(tif, TIFFTAG_TILELENGTH,  &tileLength );



 switch (numchans)

 {

	case 1: 

		/*

		 * It is silly to have one plane, and call it plane

		 * interleaved.  This is non-interleaved.  Hence, just

		 * call the read-non-intereleaved-tiles function.

		 * (which allows for varying channel depths)

		 */

		imTiffReadNonInterleavedTiles(tif, vfb);

		break;

	case 3:

		 if (chandepth!=8)

		 {

			sprintf(message,"Can't read %d bits per channel plane-interleaved, tiled images\n",chandepth);

			ImErrorFatal(message,0,IMEUNSUPPORTED);

		 }

		imTiffReadRedTiledPlane  (tif, vfb, tileWidth, tileLength, imageWidth, imageLength, tileSize);

		imTiffReadGreenTiledPlane(tif, vfb, tileWidth, tileLength, imageWidth, imageLength, tileSize);

		imTiffReadBlueTiledPlane (tif, vfb, tileWidth, tileLength, imageWidth, imageLength, tileSize);

		break;

	case 4:

		 if (chandepth!=8)

		 {

			sprintf(message,"Can't read %d bits per channel plane-interleaved, tiled images\n",chandepth);

			ImErrorFatal(message,0,IMEUNSUPPORTED);

		 }

		imTiffReadRedTiledPlane  (tif, vfb, tileWidth, tileLength, imageWidth, imageLength, tileSize);

		imTiffReadGreenTiledPlane(tif, vfb, tileWidth, tileLength, imageWidth, imageLength, tileSize);

		imTiffReadBlueTiledPlane (tif, vfb, tileWidth, tileLength, imageWidth, imageLength, tileSize);

		imTiffReadAlphaTiledPlane(tif, vfb, tileWidth, tileLength, imageWidth, imageLength, tileSize);

		break;

	default : 

		sprintf(message,"Can't read %d channels plane-interleaved\n",numchans);

		ImErrorFatal(message,0,IMEUNSUPPORTED);

 }



 return 1;

}





static int                             /* Returns # of tags written    */

#ifdef __STDC__

imTiffWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imTiffWrite( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

        int             ioType;         /* I/O flags                    */

        int             fd;             /* Output file descriptor       */

        FILE           *fp;             /* Output file pointer          */

        TagTable       *flagsTable;     /* Format flags                 */

        TagTable       *tagTable;       /* Tag list to add to           */

#endif

{



 TIFF* tif;			/* TIFF structure 		   */

 char message[200];		/* buffer for messages.            */

 ImVfb* vfb;  			/* Holds a vfb                     */

 ImClt* clt; 			/* Holds a clt                     */

 char *filename;                /* Holds file name                 */

 int curImage;                  /* index for the current image     */

 int numImages;                 /* number of images in the vfb     */

 TagEntry* entry;		/* entry in table with file name   */



 /*

  * Open our tif structure for writing.

  */

 

 if (ioType & IMFILEIOFP)

 {

	fd = fileno(fp);

 }



 /* Search flags table for filename */



 if (!flagsTable || TagTableQNEntry (flagsTable, "file name") == 0)

 {

	filename = NULL;

 }

 else

 {

	entry = TagTableQDirect ( flagsTable, "file name", 0 );

	if (TagEntryQValue( entry, &filename) == -1)

		filename = NULL;

 }



 tif = TIFFFdOpen(fd, filename ? filename : "libtiff", "wb"); 

 if (!tif)

 {

	ImErrorFatal("Could not write to output file", -1, IMENOTPOSSIBLE);

 }





 /* 

  * Loop through images in vfb

  */



 numImages = TagTableQNEntry( tagTable, "image vfb");



 for (curImage = 0; curImage < numImages; curImage++)

 {

	 /* Retreieve this vfb from the table */



	 TagEntryQValue (TagTableQDirect (tagTable, "image vfb", curImage), &vfb);  



	 if (numImages > 1)

	 {

		sprintf (message,"%d of %d", curImage + 1, numImages);

		ImInfo ( "Image", message );

	 }



	 /*

	  * Set resolution

	  */



	 sprintf(message,"%d x %d",ImVfbQWidth(vfb),ImVfbQHeight(vfb));

	 ImInfo( "Resolution", message );

	 TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, ImVfbQWidth(vfb) );

	 TIFFSetField(tif, TIFFTAG_IMAGELENGTH, ImVfbQHeight(vfb) );



	 /*

	  * Set orientation

	  */



	  TIFFSetField (tif,TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);



	 /*

	  * Set min, max values.

	  */



	  TIFFSetField (tif, TIFFTAG_MINSAMPLEVALUE, 0 );

	  TIFFSetField (tif, TIFFTAG_MAXSAMPLEVALUE, ((1L)<<(pMap->map_outChannelDepth)) - 1L);





	 /*

	  * Set byte order

	  */

 	  if ( BinQMachine()->bin_byteOrder == BINLBF)

	  {

		  TIFFSetField(tif, TIFFTAG_FILLORDER,FILLORDER_LSB2MSB);   

		  ImInfo ("Byte Order", "Least Significant Byte First");

	  }

	  else

	  {

		  TIFFSetField(tif, TIFFTAG_FILLORDER,FILLORDER_MSB2LSB);   

		  ImInfo ("Byte Order", "Most Significant Byte First");

	  }



	 /*

	  * Set image type 

	  */



	 if (pMap->map_outAttributes & IMALPHAYES || (ImVfbQFields(vfb) & IMVFBALPHA))

		sprintf(message,"%d-bit ",pMap->map_outChannelDepth * ( pMap->map_outNChannels + 1)) ;

	 else

		sprintf(message,"%d-bit ",pMap->map_outChannelDepth * pMap->map_outNChannels) ;



	 if (pMap->map_outType == IMTYPERGB || (ImVfbQFields(vfb) & IMVFBRGB) )

	 {

		strcat(message,"RGB");

		TIFFSetField(tif,TIFFTAG_PHOTOMETRIC,PHOTOMETRIC_RGB);

	 }

	 else

	 {

		strcat(message,"Color Indexed");



		/*

		 * Is this a color-indexed image or a mono image or a greyscale image?

		 */



		if (ImVfbQClt(vfb) != IMCLTNULL)

			TIFFSetField(tif,TIFFTAG_PHOTOMETRIC,PHOTOMETRIC_PALETTE);

		else  /* mono or grayscale.  Treat them the same. */

			TIFFSetField(tif,TIFFTAG_PHOTOMETRIC,PHOTOMETRIC_MINISBLACK);

	 }



	 ImInfo ("Type",message);

	 TIFFSetField (tif, TIFFTAG_BITSPERSAMPLE ,pMap->map_outChannelDepth);





	 /*

	  * Set alpha channel if one exists

	  */





	 if (pMap->map_outAttributes & IMALPHAYES || (ImVfbQFields(vfb) & IMVFBALPHA))

	 {

		uint16 fields[2];



		fields[0] = EXTRASAMPLE_ASSOCALPHA;



		sprintf(message,"%d-bit",pMap->map_outChannelDepth);

		ImInfo ("Alpha Channel", message);

		TIFFSetField (tif, TIFFTAG_SAMPLESPERPIXEL ,pMap->map_outNChannels + 1);

		TIFFSetField (tif, TIFFTAG_EXTRASAMPLES, 1, fields );

	 }

	 else

	 {

		ImInfo ("Alpha Channel", "none");

		TIFFSetField (tif, TIFFTAG_SAMPLESPERPIXEL ,pMap->map_outNChannels);

	 }







	 /* 

	  * Set Interleave method

	  */



	 if (pMap->map_outAttributes & IMINTERPLANE)

	 {

		ImInfo ("Interleave method","Plane Interleaved");

		TIFFSetField (tif,TIFFTAG_PLANARCONFIG, PLANARCONFIG_SEPARATE );

	 }

	 else

	 {

		ImInfo ("Interleave Method","Non-interleaved");

		TIFFSetField (tif,TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG );

	 }



	 /*

	  * Set Color Table

	  */



	 if (pMap->map_outAttributes & IMCLTYES || (ImVfbQClt(vfb) != IMCLTNULL) )

	 {

		sprintf(message,"%d entries",(1L)<<pMap->map_outChannelDepth);

		clt = ImVfbQClt(vfb);

		if (clt==NULL)

		{

			ImErrorFatal("No color table for image",0,IMENOCLT);

		}

		imTiffWriteClt (tif, clt , (1L)<<pMap->map_outChannelDepth);

	 }

	 else

	 {

		sprintf(message,"none");

	 }



	 ImInfo ("Color Table", message);



	 /*

	  * Set Compression scheme

	  */



	 switch (pMap->map_outAttributes & IMCOMPMASK)

	 {

	  case IMCOMPPACKBITS :

		ImInfo ("Compression Type","Macintosh PackBits encoding");

		TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_PACKBITS);

		break;

	  case IMCOMPLZW :

		ImInfo ("Compression Type", "Lempel-Ziv & Welch encoding");

		TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW);

		break;

	  case IMCOMPDCT :

		ImInfo ("Compression Type", "JPEG encoding (DCT)");

		TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_JPEG);

		break;

	  case IMCOMPNO :

		ImInfo ("Compression Type", "none");

		TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);

		break; 

	 }



	 /*

	  * Set Grouping method

	  */



	 if ( (pMap->map_outAttributes & IMGROUPMASK) == IMGROUPTILES)

	 {

		 ImInfo ("Grouping Method", "Tiles");



		 /* Determine interleaving method and call the correct routine. */

		 if ( pMap->map_outAttributes  & IMINTERPLANE )

		 {

			imTiffWritePlaneInterleavedTiles(tif,vfb, pMap);

		 }

		 else

		 {

			imTiffWriteNonInterleavedTiles(tif, vfb, pMap);

		 }

	 }

	 else /* store as scanlines */

	 {

		 ImInfo ("Grouping Method", "Scanlines");



		 /* Determine interleaving method and call the correct routine. */

		 if ( pMap->map_outAttributes  & IMINTERPLANE )

		 {

			imTiffWritePlaneInterleavedScanlines(tif,vfb, pMap);

		 }

		 else

		 {

			imTiffWriteNonInterleavedScanlines(tif, vfb, pMap);

		 }

	 }





	/* 

	 * Write the directory to prepare for the next vfb 

	 */



	TIFFWriteDirectory(tif);



 } /* End of for loop */

 TIFFClose (tif);



 return numImages;

}





/*

 *

 *  FUNCTION

 *

 *	imTiffWriteNonInterleavedScanlines

 *

 *  DESCRIPTION

 *

 *	Write a tiff file with the pixels noninterleaved, 

 *  and grouping by scanlines.

 *

 */



static int 	/* Returns 1 for okay, 0 for failure */

#ifdef __STDC__

imTiffWriteNonInterleavedScanlines( TIFF* tif, ImVfb* vfb, ImFileFormatWriteMap* pMap)

#else

imTiffWriteNonInterleavedScanlines( tif, vfb, pMap)

TIFF* tif;

ImVfb* vfb; 

ImFileFormatWriteMap *pMap;

#endif

{

 unsigned char* linebuf;	/* buffer for one scanline                    */

 uint16*  ptr16;                /* pointer to the scanline as a 16-bit array  */

 tsize_t  scanlineSize;  	/* size of scanlines 			      */

 ImVfbPtr vfbptr;       	/* ptr into a vfb                             */

 uint32   x,y;			/* Loop indices                               */

 uint32   ySize;		/* Image size (y)	                      */

 uint32   xSize;		/* Image size (x)	                      */

 uint16   numchans;             /* number of channels                         */

 uint16   chandepth; 	 	/* number of bits per channel                 */

 uint16   currentValue;         /* value of current channel of current pixel  */

 int      chanIndex;            /* which channel is being set                 */

 int      chanList[5];          /* List of channels to cycle through          */

 uint16   photometric;          /* Tells us the type of image this is.        */

 int      currentShift;         /* How many bits to shift this sample         */

 char     message[200];         /* Buffer for error messages                  */

 uint16   lastBits;             /* Contains the chandepth bits of data        */

 

 /* 

  * Determine some important things about the vfb

  */



 chandepth = pMap->map_outChannelDepth;

 numchans  = pMap->map_outNChannels;

 xSize     = ImVfbQWidth (vfb);

 ySize     = ImVfbQHeight(vfb);



 if (ImVfbQFields(vfb) & IMVFBALPHA)

	numchans++;

 /*

  * Set Rows per strip to "infinite"

  */

  TIFFSetField (tif, TIFFTAG_ROWSPERSTRIP, (uint32) -1);



 /* Allocate space for the scanline buffer */

 scanlineSize = xSize * numchans * chandepth / 8 + 2;



 ImMalloc(linebuf, unsigned char *, scanlineSize );

 

 /*

  *   Set up the list of channels through which we are going to cycle.

  */



 switch (numchans)

 {

 	case 1 : 

		 if (chandepth==16)

			 chanList[0] = IMVFBINDEX16;

		 else

			 chanList[0] = IMVFBINDEX8;

		 break;

	case 3 : 

		 chanList[0] = IMVFBRED;

		 chanList[1] = IMVFBGREEN;

		 chanList[2] = IMVFBBLUE;

		 break;

	case 4 : 

		 chanList[0] = IMVFBRED;

		 chanList[1] = IMVFBGREEN;

		 chanList[2] = IMVFBBLUE;

		 chanList[3] = IMVFBALPHA;

		 break;

	default : 

		sprintf(message,"No support for %d channels in an image.  Skipping image",numchans);

		ImErrorWarning(message,0,IMEUNSUPPORTED);

		break;

 } /* end of switch */



 /*

  *  Algorithm for writing non-interleaved scanlines:

  *

  *  Loop through the pixels in the file, and for 

  *  each channel of each pixel, query the value, and 

  *  store it in a scanline buffer.

  *

  *  Note that if we are writing less than 8-bits per channel,

  *  then we need to do some bit shifting and |'ing to place

  *  the data from the vfb in the correct place.

  *

  * Admittedly, having a switch statement within the loop that

  * writes pixels will slow down processing time, but this enables

  * us to handle a wider variety of formats.

  *

  */



  for (y=0; y< ySize ; y++)

  {	  



	 /* Write Scanline #y */



	 ptr16        = (uint16 *) linebuf;

	 x            = 0;

	 vfbptr       = ImVfbQPtr( vfb, 0, y);

	 chanIndex    = 0;

	 currentShift = 0;



	 /* Clear the first entry in the array */



	 *ptr16 = (uint16) 0;



	 /* Fill up linebuf with information from the vfb */



	 while (x < xSize)

	 {



		/*

		 * Get current data from VFB

		 */



		IM_QCHANNEL (currentValue, chanList[chanIndex])



		/* 

		 * Store the last chandepth bits of currentValue  

		 * in the first chandepth bits of lastBits.

		 */



		lastBits = (currentValue << (16 - chandepth)) ;

		/*

		 * Shift the value by currentShift (to the right) and bit-or it 

		 * with *ptr16 (ptr16 is traversing the scanline in 16-bit increments)   

		 */



		*ptr16 = (*ptr16) | (lastBits >> currentShift); 



		 /* 

		  * Increment the channel index. This signifies which aspect

		  * of the pixel we will query next.

		  */



		 chanIndex++;

		 if (chanIndex == numchans)

		 {  /* We're done with this pixel */

			ImVfbSNext(vfb, vfbptr);

			chanIndex = 0;

			x++;

		 }



		/* Increment currentShift.  If we get a number bigger than 16,

		 * increment ptr16. 

		 */



		 currentShift += chandepth;

		 if (currentShift >= 16)

		 {

			ptr16++;

			currentShift = 0;

			/* Clear the next entry in the array */

			*ptr16 = (uint16) 0;

		 }



	 }	/* end of while x < xSize    */



 	if ( BinQMachine()->bin_byteOrder == BINLBF)

		imTiffSwapBytes(linebuf, scanlineSize);



	 /* Okay, now write the scanline to the tiff file. */

	if (TIFFWriteScanline( tif, linebuf, y, 0)==-1)

	{

		ImErrorFatal("Error writing scanline",0,IMESYS);

	}



 } 		/* end of for y = 0 to ySize */



 return 1;



}









/*

 *

 *  FUNCTION

 *

 *	imTiffWritePlaneInterleavedScanlines

 *

 *  DESCRIPTION

 *

 *	Write a tiff file, plane-interleaving the pixels,

 *   and using scanlines.

 *

 */



static int 	/* Returns 1 for okay, 0 for failure */

#ifdef __STDC__

imTiffWritePlaneInterleavedScanlines( TIFF *tif, ImVfb* vfb, ImFileFormatWriteMap* pMap) 

#else

imTiffWritePlaneInterleavedScanlines( tif,  vfb, pMap)

TIFF* tif;

ImVfb* vfb;

ImFileFormatWriteMap* pMap;

#endif

{



 char message[300];		/* Buffer for error message 		      */

 uint16   numchans;             /* number of channels                         */

 uint16   chandepth; 	 	/* number of bits per channel                 */



 /*

  * Algorithm :

  *

  *   This is basically just like writing non-interleaved scanlines,

  * once for each channel.  So this routine does just that. 

  * It calls the functions : 

  * 	imTiffWriteRedScanlinePlane, imTiffWriteGreenScanlinePlane

  *     imTiffWriteBlueScanlinePlane, imTiffWriteAlphaScanlinePlane

  *

  *  Each of those functions simply acts like

  *	imTiffWriteNonInterleavedScanlines

  *  except that it only writes one channel.

  *

  */





 /* 

  * Determine depth and number of channels 

  */



 chandepth = pMap->map_outChannelDepth;

 numchans  = pMap->map_outNChannels;

 if (ImVfbQFields(vfb) & IMVFBALPHA)

	numchans++;



 /*

  * Set Rows per strip to "infinite"

  */



 TIFFSetField (tif, TIFFTAG_ROWSPERSTRIP, (uint32) -1);



 switch (numchans)

 {

	case 1:

		if (ImVfbQFields(vfb) & IMVFBINDEX16)

			imTiffWriteIndex16ScanlinePlane(vfb, tif, chandepth);

		else

			imTiffWriteIndex8ScanlinePlane(vfb, tif, chandepth);

		break;

	case 3:

		imTiffWriteRedScanlinePlane  (vfb, tif, chandepth);

		imTiffWriteGreenScanlinePlane(vfb, tif, chandepth);

		imTiffWriteBlueScanlinePlane (vfb, tif, chandepth);

		break;

	case 4:

		imTiffWriteRedScanlinePlane  (vfb, tif, chandepth);

		imTiffWriteGreenScanlinePlane(vfb, tif, chandepth);

		imTiffWriteBlueScanlinePlane (vfb, tif, chandepth);

		imTiffWriteAlphaScanlinePlane(vfb, tif, chandepth);

		break;

	default:

		sprintf(message,"Can't handle %d channels plane-interleaved scanlines\n", numchans);

		ImErrorFatal(message,0,IMEUNSUPPORTED);

 }



 return 1;

}





/*

 *

 *  FUNCTION

 *

 *  	imTiffWriteNonInterleavedTiles 

 *

 *  DESCRIPTION

 *

 *	Write a tiff file, using tiles, and not interleaving

 * the pixels.

 *

 */



static int	/* Returns 1 for okay, 0 for failure */

#ifdef __STDC__

imTiffWriteNonInterleavedTiles (TIFF *tif, ImVfb *vfb, ImFileFormatWriteMap* pMap)

#else

imTiffWriteNonInterleavedTiles (tif, vfb, pMap)

TIFF* tif;

ImVfb* vfb;

ImFileFormatWriteMap* pMap;

#endif

{

 uint32 vfbX, vfbY;		/* loop indexes    		*/

 unsigned char* tilebuf;	/* buffer for tiles		*/			

 ImVfbPtr vfbptr;		/* pointer into a vfb           */			

 int vfbwidth, vfbheight;	/* dimensions of the vfb        */	

 int tileindex;                 /* index into tilebuf           */	

 int x,y;			/* indexes within a tile        */	

 uint16 numchans;		/* number of channels           */

 uint16 chandepth;		/* channel depth                */

 uint32 tileWidth, tileHeight;  /* size of tiles         	*/

 char message[120];		/* buffer for error messages    */

								

 /*

  * Discern the number of channels and channel depth 

  */



 numchans  = pMap->map_outNChannels;

 if (ImVfbQFields(vfb) & IMVFBALPHA)

	numchans++;

 chandepth = pMap->map_outChannelDepth;



 if (chandepth!=8)

 {

	sprintf(message,"No support non-interleaved tiles with depth %d",chandepth);

	ImErrorWarning(message,0,IMEUNSUPPORTED);

 }



  /* Let's just make the tiles 16 by 16 (the dimensions must be multiples of 16) */



  tileWidth = (uint32) 16;

  tileHeight = (uint32) 16;

  TIFFSetField (tif, TIFFTAG_TILEWIDTH, tileWidth);

  TIFFSetField (tif, TIFFTAG_TILELENGTH, tileHeight);



 /* 

  * if only one plane then this is equivalent to writing plane

  * interleaved tiles

  */



  if (numchans==1)

  {

	return imTiffWriteIndex8TiledPlane(vfb, tif, chandepth, tileWidth, tileHeight);

  }



 /* Determine vfb dimensions */



 vfbwidth  = ImVfbQWidth(vfb);								

 vfbheight = ImVfbQHeight(vfb);								

											

 /*											

  * Allocate space for tilebuf								

  */											



 ImMalloc(tilebuf, unsigned char *, (int) (tileWidth * tileHeight * numchans) + 1 );			

											

 /*											

  * Loop through tile positions	and create either an RGB or an RGBA image

  */											

											

 for (vfbX=0; vfbX<vfbwidth; vfbX+=tileWidth)						

 {											

	for (vfbY=0; vfbY<vfbheight; vfbY+=tileHeight)					

	{										

		/*									

		 * Create a tile at vfbX, vfbY

		 */									



		vfbptr = ImVfbQPtr(vfb, vfbX, vfbY);					

		tileindex = 0;								



		/*

		 * start x,y at the upper left hand corner

		 * of the tile

		 */



		x = 0;									

		y = 0;									



		while ( y < (int) tileHeight && vfbY + y < vfbheight )			

		{									

			/*								

			 * If we're past the right edge of the image, 			

			 * put in a dummy value (i.e. pad the image)			

			 */								

											

			if (vfbX + x >= vfbwidth)					

			{								

				tilebuf[tileindex++] = (unsigned char) 0;

				tilebuf[tileindex++] = (unsigned char) 0;

				tilebuf[tileindex++] = (unsigned char) 0;

				if (numchans==4)

					tilebuf[tileindex++] = (unsigned char) 0;

			}								

			else  /* put in the real values */				

			{								

				tilebuf[tileindex++] = ImVfbQRed(vfb,vfbptr);	

				tilebuf[tileindex++] = ImVfbQGreen(vfb,vfbptr);	

				tilebuf[tileindex++] = ImVfbQBlue(vfb,vfbptr);	

				if (numchans==4)

					tilebuf[tileindex++] = ImVfbQAlpha(vfb,vfbptr);

			}								

											

			/* Increment index within tile */				

			x++;								



			/*

			 * if we're at the  end of the tile	

			 * move down one line				

			 * and back to the start of the			

			 * tile.						

			 */							



			if ( x >= (int) tileWidth ) 				

			{						

				y++;				

				if (vfbY + y < vfbheight)			

					vfbptr = ImVfbQPtr(vfb, vfbX, vfbY + y);

				x = 0;							

			}							

			else  /* just move to the right */		

			{

				if (vfbY + y < vfbheight)			

					vfbptr = ImVfbQRight(vfb,vfbptr);			

			}

		}									

		/* Send out tile to tiff */						

		if ( BinQMachine()->bin_byteOrder == BINLBF)				

			imTiffSwapBytes(tilebuf, tileWidth * tileHeight * numchans);	

		TIFFWriteTile(tif, tilebuf, (uint32) vfbX, (uint32) vfbY, 0, 0);	

	}										

 }											

 return 1;										



}







/*

 *

 *  FUNCTION

 *

 *	imTiffWritePlaneInterleavedTiles 

 *

 *  DESCRIPTION

 *

 *	Write a tiff file, using plane-interleaving and

 *  tiles.

 *

 */



static int 	/* Returns 1 for okay, 0 for failure */

#ifdef __STDC__

imTiffWritePlaneInterleavedTiles ( TIFF *tif, ImVfb *vfb, ImFileFormatWriteMap* pMap)

#else

imTiffWritePlaneInterleavedTiles ( tif, vfb, pMap)

TIFF* tif;

ImVfb* vfb;

ImFileFormatWriteMap* pMap;

#endif

{

 

 uint16   numchans;     /* number of channels          */

 uint16   chandepth; 	/* number of bits per channel  */

 uint32   tileWidth;	/* width of tiles              */

 uint32   tileHeight;   /* height of tiles             */

 char message[100];	/* buffer for messages         */

 int numTiles;          /* Number of tiles in the image*/



 /*

  * For each plane in the image, call a function to write

  *  the plane to the tiff structure

  */



 chandepth = pMap->map_outChannelDepth;

 numchans  = pMap->map_outNChannels;

 if (ImVfbQFields(vfb) & IMVFBALPHA)

	numchans++;

 

 /*

  * No support for depths other than 8 at this point in time

  */



  if (chandepth!=8)

  {

	sprintf(message,"Can't write %d depth tiled images",chandepth);

	ImErrorFatal(message,0,IMEUNSUPPORTED);

  }

 

 /* 

  * Let's make the tiles 16 x 16.

  * If anyone ever wants to change this, just be sure

  * that the dimensions are multiples of 16 (as per

  * tiff's standards).  Or maybe multiples of 8.  TIFF

  * has some contradictions in their standards.  The man

  * pages for TIFFSetField say 8.  The README file says 16.

  *

  */



  tileWidth = (uint32) 16;

  tileHeight = (uint32) 16;

  TIFFSetField (tif, TIFFTAG_TILEWIDTH, tileWidth);

  TIFFSetField (tif, TIFFTAG_TILELENGTH, tileHeight);



 switch (numchans)

 {

	case 1:

		imTiffWriteIndex8TiledPlane(vfb, tif, chandepth, tileWidth, tileHeight);

		break;

	case 3:

		imTiffWriteRedTiledPlane  (vfb, tif, chandepth, tileWidth, tileHeight);

		imTiffWriteGreenTiledPlane(vfb, tif, chandepth, tileWidth, tileHeight);

		imTiffWriteBlueTiledPlane (vfb, tif, chandepth, tileWidth, tileHeight);

		break;

	case 4:

		imTiffWriteRedTiledPlane  (vfb, tif, chandepth, tileWidth, tileHeight);

		imTiffWriteGreenTiledPlane(vfb, tif, chandepth, tileWidth, tileHeight);

		imTiffWriteBlueTiledPlane (vfb, tif, chandepth, tileWidth, tileHeight);

		imTiffWriteAlphaTiledPlane(vfb, tif, chandepth, tileWidth, tileHeight);

		break;

	default:

		sprintf(message,"Can't handle %d channels plane-interleaved tiles\n", numchans);

		ImErrorFatal(message,0,IMEUNSUPPORTED);

 }

 return 1;



}





/*

 *

 *  FUNCTION

 *

 *	imTiffWriteClt

 *

 *  DESCRIPTION

 *

 *	Write the clt for an image to a file.

 *

 */



static 

int /* Returns 1 for okay, 0 for failure */

#ifdef __STDC__

imTiffWriteClt(TIFF* tif, ImClt *clt, long numEntries)

#else

imTiffWriteClt(tif, clt, numEntries)

TIFF* tif;

ImClt* clt;

long numEntries;

#endif

{

  uint16* redmap;		/* Stores red components for color map entries   */

  uint16* bluemap;		/* Stores blue components for color map entries  */

  uint16* greenmap;		/* Stores green components for color map entries */

  ImCltPtr cltptr;		/* pointer into the clt                          */

  long i;			/* loop index 					 */

  long realNumEntries; 		/* minimum of desired and actual no. of entries  */



 /* Allocate space for arrays which are passed to tif structure */



 ImMalloc(redmap  , uint16* , numEntries * sizeof(uint16) );

 ImMalloc(bluemap , uint16* , numEntries * sizeof(uint16));

 ImMalloc(greenmap, uint16* , numEntries * sizeof(uint16));



 /* Do we really have numEntries entries in the clt?  Or are there fewer? */



 realNumEntries = numEntries; 

 if (ImCltQNColors(clt)<realNumEntries)

	realNumEntries = ImCltQNColors(clt);



 /* Read entries from clt and put them into tif. */



 cltptr = ImCltQFirst(clt);

 i = (long) 0;

 while (i < realNumEntries)

 {

	redmap[i]   = (uint16) ImCltQRed(cltptr);

	bluemap[i]  = (uint16) ImCltQBlue(cltptr);

	greenmap[i] = (uint16) ImCltQGreen(cltptr);

	i++;

	cltptr = ImCltQNext(clt, cltptr);

 }



 /*

  * Scale colormap to TIFF-required 16-bit values.

  */

#define	IM_CLTSCALE(x)	(((x)*((1L<<16)-1))/255)

 for (i = (long) 0; i < realNumEntries; i++) 

 {

	redmap[i]   = IM_CLTSCALE(redmap[i]);

	greenmap[i] = IM_CLTSCALE(greenmap[i]);

	bluemap[i]  = IM_CLTSCALE(bluemap[i]);

 }

 

 /* Write the map to the tiff structure */

 TIFFSetField(tif, TIFFTAG_COLORMAP, redmap, greenmap, bluemap);



 free(redmap);

 free(greenmap);

 free(bluemap);



 return 1;

}





 /*

  * FUNCTIONS

  * 	imTiffWriteRedScanlinePlane

  * 	imTiffWriteBlueScanlinePlane

  * 	imTiffWriteGreenScanlinePlane

  * 	imTiffWriteIndex8ScanlinePlane

  * 	imTiffWriteAlphaScanlinePlane

  * 	imTiffWriteIndex16ScanlinePlane

  *

  * DESCRIPTION

  * 	Go through the entire vfb, and write a series of 

  * scanlines to the tiff structure.

  *

  */



#ifdef __STDC__

#define FunctionHeaderSPW(functionName)					\

static int functionName(ImVfb *vfb, TIFF *tif, uint16 chandepth)



#else



#define FunctionHeaderSPW(functionName)					\

static int functionName(vfb, tif, chandepth)				\

ImVfb* vfb;								\

TIFF* tif;								\

uint16 chandepth;

#endif





#define IM_MAKE_SCANLINE_PLANE_WRITE_FUNCTION(functionName, ImVfbQSomething, imagePlane)	\

										\

FunctionHeaderSPW(functionName)							\

{										\

 unsigned char* linebuf;	/* buffer for one scanline                   */	\

 tsize_t  scanlineSize;		/* size of a scanline                        */ \

 uint16*  ptr16;                /* pointer to a 16-bit value                 */	\

 ImVfbPtr vfbptr;       	/* ptr into a vfb                            */	\

 uint32   x,y;			/* Loop indices                              */	\

 uint32   ySize;		/* Image size (y)	                     */	\

 uint32   xSize;		/* Image size (x)	                     */	\

 uint16   currentValue;         /* value of current channel of current pixel */	\

 int      currentShift;         /* How many bits to shift this sample        */	\

 uint16   lastBits;             /* Contains chandepth bits of data           */	\

										\

 /*										\

  * Determine some important things about the vfb				\

  */										\

										\

 xSize     = ImVfbQWidth (vfb);							\

 ySize     = ImVfbQHeight(vfb);							\

										\

 /* Allocate space for the scanline buffer */					\

										\

 scanlineSize = xSize * chandepth / 8 + 2;					\

 ImMalloc(linebuf,unsigned char *, scanlineSize);				\

 										\

 /*										\

  *  Note that if we are writing less than 8-bits per channel,			\

  *  then we need to do some bit shifting and |'ing to place			\

  *  the data from the vfb in the correct place.				\

  *										\

  * I won't explain it all here.  						\

  * See imTiffWriteNonInterleavedScanlines for more comments.			\

  *										\

  */										\

										\

  for (y=0; y< ySize ; y++)							\

  {	  									\

										\

	 /* Write Scanline #y */						\

										\

	 ptr16        = (uint16 *) linebuf;					\

	 vfbptr       = ImVfbQPtr( vfb, 0, y);					\

	 currentShift = 0;							\

										\

	 /* Clear the first entry in the array */				\

										\

	 *ptr16 = (uint16) 0;							\

										\

	 /* Fill up linebuf with information from the vfb */			\

										\

	for (x=0;x < xSize; x++)						\

	{									\

		currentValue = (uint16) ImVfbQSomething(vfb,vfbptr);  		\

		lastBits = (currentValue << (16 - chandepth)) ;			\

		*ptr16 = (*ptr16) | (lastBits >> currentShift); 		\

		currentShift += chandepth;					\

		if (currentShift >= 16)						\

		{								\

			ptr16++;						\

			currentShift = 0;					\

			/* Clear the next entry in the array */			\

			*ptr16 = (uint16) 0;					\

		}								\

										\

		ImVfbSNext(vfb, vfbptr);					\

	 }									\

										\

 	if ( BinQMachine()->bin_byteOrder == BINLBF)				\

		imTiffSwapBytes(linebuf, scanlineSize);				\

										\

	 /* Okay, now write the scanline to the tiff file. */			\

	if (TIFFWriteScanline( tif, linebuf, y, imagePlane)==-1)		\

	{									\

		ImErrorFatal("Error writing scanline",0,IMESYS);		\

	}									\

										\

 } 		/* end of for y = 0 to ySize */					\

										\

 return 1;									\

										\

}



IM_MAKE_SCANLINE_PLANE_WRITE_FUNCTION(imTiffWriteRedScanlinePlane,ImVfbQRed, 0)

IM_MAKE_SCANLINE_PLANE_WRITE_FUNCTION(imTiffWriteGreenScanlinePlane,ImVfbQGreen, 1)

IM_MAKE_SCANLINE_PLANE_WRITE_FUNCTION(imTiffWriteBlueScanlinePlane,ImVfbQBlue, 2)

IM_MAKE_SCANLINE_PLANE_WRITE_FUNCTION(imTiffWriteAlphaScanlinePlane,ImVfbQAlpha, 3)

IM_MAKE_SCANLINE_PLANE_WRITE_FUNCTION(imTiffWriteIndex8ScanlinePlane,ImVfbQIndex8, 0)

IM_MAKE_SCANLINE_PLANE_WRITE_FUNCTION(imTiffWriteIndex16ScanlinePlane,ImVfbQIndex16, 0)



/*

 * FUNCTION

 * 	imTiffReadRedScanlinePlane

 * 	imTiffReadGreenScanlinePlane

 * 	imTiffReadBlueScanlinePlane

 *

 * DESCRIPTION

 * 	Read the plane into the vfb.

 *      This routine only works for 8-bit images.

 *

 */



#ifdef __STDC__

#define FunctionHeaderSPR(functionName)			\

static int						\

functionName(TIFF* tif, ImVfb* vfb)

#else



#define FunctionHeaderSPR(functionName)			\

static int						\

functionName(tif,vfb)					\

TIFF* tif;						\

ImVfb* vfb;

#endif



#define IM_MAKE_SCANLINE_PLANE_READ_FUNCTION(functionName,ImVfbSSomething,imagePlane)	\

FunctionHeaderSPR(functionName)							\

{										\

 unsigned char* linebuf;        /* buffer for one scanline                */	\

 tsize_t scanlineSize;          /* size of scanlines                      */	\

 ImVfbPtr vfbptr;               /* ptr into a vfb                         */	\

 uint32 row, col;        	/* Loop indices                           */	\

 uint32 ySize;           	/* Image size (y)                         */	\

										\

 TIFFGetField( tif, TIFFTAG_IMAGELENGTH, &ySize);				\

 scanlineSize = TIFFScanlineSize(tif);						\

 ImMalloc(linebuf,unsigned char *,scanlineSize);				\

 vfbptr = ImVfbQPtr(vfb, 0, 0);							\

 for (row=0; row<ySize; row++)							\

 {										\

 	if (TIFFReadScanline(tif, linebuf, row, imagePlane) < 0)		\

		break;								\

	for (col=0;col<scanlineSize;col++)					\

	{									\

		ImVfbSSomething(vfb, vfbptr, linebuf[col]);			\

		ImVfbSNext(vfb, vfbptr);					\

	}									\

 }										\

 free(linebuf);									\

 return 1;									\

}



IM_MAKE_SCANLINE_PLANE_READ_FUNCTION(imTiffReadRedScanlinePlane,   ImVfbSRed, 0)

IM_MAKE_SCANLINE_PLANE_READ_FUNCTION(imTiffReadGreenScanlinePlane, ImVfbSGreen, 1)

IM_MAKE_SCANLINE_PLANE_READ_FUNCTION(imTiffReadBlueScanlinePlane,  ImVfbSBlue, 2)

IM_MAKE_SCANLINE_PLANE_READ_FUNCTION(imTiffReadAlphaScanlinePlane, ImVfbSAlpha, 3)



/*

 *

 * FUNCTION

 *	imTiffWriteRedTiledPlane

 *	imTiffWriteGreenTiledPlane

 *	imTiffWriteBlueTiledPlane

 *	imTiffWriteAlphaTiledPlane

 *	imTiffWriteIndex8TiledPlane

 *

 * DESCRIPTION

 *	Write the red,green, blue or alpha planes to the tiff

 *	structure, storing them in tiles.

 *	These functions only support 8 bits per channel.

 */



#ifdef __STDC__

#define functionHeaderTPWF(functionName)						\

static int  /* Returns 1 for success, 0 for failure */					\

functionName(  ImVfb* vfb, TIFF* tif, uint16 chanDepth, uint32 tileWidth, uint32 tileHeight)



#else



#define functionHeaderTPWF(functionName)						\

static int  /* Returns 1 for success, 0 for failure */					\

functionName(   vfb, tif, chanDepth, tileWidth, tileHeight) 				\

ImVfb* vfb;										\

TIFF* tif;										\

uint16 chanDepth;									\

uint32 tileWidth;									\

uint32 tileHeight;

#endif



#define IM_MAKE_TIFF_TILE_PLANE_WRITE_FUNCTION(functionName,ImVfbQSomething,imagePlane)	\

functionHeaderTPWF(functionName)							\

{											\

 uint32 vfbX, vfbY;		/* loop indexes    		*/			\

 unsigned char* tilebuf;	/* buffer for tiles		*/			\

 ImVfbPtr vfbptr;		/* pointer into a vfb           */			\

 int vfbwidth, vfbheight;	/* dimensions of the vfb        */			\

 int tileindex;                 /* index into tilebuf           */			\

 int x,y;			/* indexes within a tile        */			\

											\

 vfbwidth = ImVfbQWidth(vfb);								\

 vfbheight = ImVfbQHeight(vfb);								\

											\

 /*											\

  * Allocate space for tilebuf								\

  */											\

 ImMalloc(tilebuf, unsigned char *, (int) (tileWidth * tileHeight) + 1);		\

											\

 /*											\

  * Loop through tile positions								\

  */											\

											\

 for (vfbX=0; vfbX<vfbwidth; vfbX+=tileWidth)						\

 {											\

	for (vfbY=0; vfbY<vfbheight; vfbY+=tileHeight)					\

	{										\

		/*									\

		 * Create a tile!							\

		 */									\

		vfbptr = ImVfbQPtr(vfb, vfbX, vfbY);					\

		tileindex = 0;								\

		x = 0;									\

		y = 0;									\

		while ( y < (int) tileHeight && vfbY + y < vfbheight )			\

		{									\

			/*								\

			 * If we're past the edge of the image, 			\

			 * put in a dummy value (i.e. pad the image)			\

			 */								\

											\

			if (vfbX + x >= vfbwidth)					\

			{								\

				tilebuf[tileindex] = (unsigned char) 0;			\

			}								\

			else  /* put in the real value */				\

			{								\

				tilebuf[tileindex] = ImVfbQSomething(vfb,vfbptr);	\

			}								\

											\

			/* Increment index within tile */				\

			x++;								\

											\

			/*								\

			 * if we're at the  end of the tile				\

			 * move down one line						\

			 * and back to the start of the					\

			 * tile.							\

			 */								\

			if ( x >= (int) tileWidth ) 					\

			{								\

				y++;							\

				if (vfbY + y < vfbheight)				\

					vfbptr = ImVfbQPtr(vfb, vfbX, vfbY + y);	\

				x = 0;							\

			}								\

			else  /* just move to the right */				\

			{								\

				if (vfbY + y < vfbheight)				\

					vfbptr = ImVfbQRight(vfb,vfbptr);		\

			}								\

			tileindex++;							\

		}									\

		/* Send out tile to tiff */						\

		if ( BinQMachine()->bin_byteOrder == BINLBF)				\

			imTiffSwapBytes(tilebuf, tileWidth * tileHeight);		\

		TIFFWriteTile(tif, tilebuf, (uint32) vfbX, (uint32) vfbY, 0, imagePlane);	\

	}										\

 }											\

return 1;										\

}



IM_MAKE_TIFF_TILE_PLANE_WRITE_FUNCTION(imTiffWriteIndex8TiledPlane,ImVfbQIndex8,0)

IM_MAKE_TIFF_TILE_PLANE_WRITE_FUNCTION(imTiffWriteRedTiledPlane,ImVfbQRed,0)

IM_MAKE_TIFF_TILE_PLANE_WRITE_FUNCTION(imTiffWriteGreenTiledPlane,ImVfbQGreen,1)

IM_MAKE_TIFF_TILE_PLANE_WRITE_FUNCTION(imTiffWriteBlueTiledPlane,ImVfbQBlue,2)

IM_MAKE_TIFF_TILE_PLANE_WRITE_FUNCTION(imTiffWriteAlphaTiledPlane,ImVfbQAlpha,3)



/*

 * FUNCTIONS

 * 	

 *   imTiffReadIndex8TiledPlane

 *   imTiffReadRedTiledPlane

 *   imTiffReadGreenTiledPlane

 *   imTiffReadBlueTiledPlane

 *   imTiffReadAlphaTiledPlane

 *

 * DESCRIPTION

 *   Read this channel into the entire vfb, when the

 * data is grouped in tiles.

 *

 */



#ifdef __STDC__

#define functionHeaderRTPF(functionName)						\

static int /* returns 1 for success, 0 for failure */					\

functionName( TIFF* tif, ImVfb* vfb, uint32 tileWidth, 					\

	uint32 tileLength, uint32 imageWidth, uint32 imageLength, tsize_t tileSize)

#else

#define functionHeaderRTPF(functionName)						\

static int /* returns 1 for success, 0 for failure */					\

functionName( tif, vfb, tileWidth,tileLength, imageWidth, imageLength, tileSize)	\

TIFF* tif;										\

ImVfb* vfb;										\

uint32 tileWidth;									\

uint32 tileLength;									\

uint32 imageWidth;									\

uint32 imageLength;									\

tsize_t tileSize;

#endif



#define IM_TIFF_MAKE_READ_TILED_PLANE_FUNCTION(functionName, ImVfbSSomething, imagePlane)  \

functionHeaderRTPF(functionName)							\

{											\

 ImVfbPtr vfbptr;	/* Pointer into the vfb			*/			\

 uint32 tileX, tileY;	/* relative to the tile 		*/			\

 uint32 vfbX, vfbY;	/* start of tile relative to vfb 	*/			\

 unsigned char* tilebuf;/* buffer for tile data                 */			\

 int tileIndex;         /* index within tilebuf                 */			\

											\

 /* Allocate space for tilebuf */							\

											\

 ImMalloc( tilebuf, unsigned char *, tileSize);						\

											\

for (vfbY = 0; vfbY < imageLength; vfbY += tileLength)					\

{											\

	 for (vfbX = 0; vfbX < imageWidth; vfbX += tileWidth)				\

	 {										\

		TIFFReadTile( tif, tilebuf, vfbX, vfbY, 0, imagePlane);			\

		tileX = 0;								\

		tileY = 0;								\

											\

		/* Move to upper left hand corner of this tile */			\

											\

		vfbptr = ImVfbQPtr( vfb, vfbX, vfbY);					\

		tileIndex = 0;								\

											\

		/* Read this tile! */							\

											\

		while (tileIndex < tileSize)						\

		{									\

			/* Only write to the vfb if we're within our boundaries */	\

											\

			if (vfbX + tileX < imageWidth && vfbY + tileY < imageLength)	\

			{								\

				ImVfbSSomething (vfb, vfbptr, tilebuf[tileIndex]);	\

				ImVfbSRight (vfb, vfbptr);				\

			}								\

			tileX++;							\

			if (tileX >= tileWidth)						\

			{								\

				/* move down a line and back to the left edge of the tile */	\

				tileX = 0;						\

				tileY++;						\

				if (vfbY + tileY < imageLength)				\

				{							\

					vfbptr = ImVfbQPtr (vfb, vfbX + 0, vfbY + tileY);	\

				}							\

			}								\

			tileIndex++;							\

		}									\

	}										\

 }											\

 return 1;										\

}				



IM_TIFF_MAKE_READ_TILED_PLANE_FUNCTION(imTiffReadRedTiledPlane, ImVfbSRed, 0)

IM_TIFF_MAKE_READ_TILED_PLANE_FUNCTION(imTiffReadGreenTiledPlane, ImVfbSGreen, 1)

IM_TIFF_MAKE_READ_TILED_PLANE_FUNCTION(imTiffReadBlueTiledPlane, ImVfbSBlue, 2)

IM_TIFF_MAKE_READ_TILED_PLANE_FUNCTION(imTiffReadAlphaTiledPlane, ImVfbSAlpha, 0)





/*

 * FUNCTION

 *    imTiffSwapBytes

 *

 * DESCRIPTION

 *    Traverse a line and switch every pair of bytes

 *    An extra byte should be allocated when passing a buffer with

 *    an odd size to this routine.  

 *

 */



static void /* Returns nothing */

#ifdef __STDC__

imTiffSwapBytes(unsigned char* linebuf, tsize_t linesize)

#else

imTiffSwapBytes(linebuf, linesize)

unsigned char* linebuf;

tsize_t linesize;

#endif

{

 int i; 		/* loop index         */

 unsigned char temp;    /* temporary variable */



 for (i=0;i<linesize;i+=2)

 {

	 /* Swap linebuf[i] and linebuf[i+1] */

	 temp = linebuf[i];

	 linebuf[i] = linebuf[i+1];

	 linebuf[i+1] = temp;

 }

}





#endif  /* USE_TIFF_LIB */



