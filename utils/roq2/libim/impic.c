/**

 **	$Header: /roq/libim/impic.c 1     11/02/99 4:38p Zaphod $ 

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



#define HEADER	"   $Header: /roq/libim/impic.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	impic.c		-  PIXAR picture file I/O

 **

 **  PROJECT

 **	libimage	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	impic.c contains routines to read and write PIXAR picture files for

 **	the image manipulation library.  Raster data read in is stored

 **	in a VFB.   Raster data written	out is taken from a tag list.

 **

 **  PUBLIC CONTENTS

 **                     d =defined constant

 **                     f =function

 **                     m =defined macro

 **                     t =typedef/struct/union

 **                     v =variable

 **                     ? =other

 **

 **	done

 **

 **  PRIVATE CONTENTS

 **

 **     imPicRead               f  read a PIXAR picture file

 **	imPicWriteRGBA		f  write a four-channel PIC file, dump

 **	imPicWriteRGBARLE	f  write a four-channel PIC file, encoded

 **	imPicWriteRGB		f  write a three-channel PIC file, dump

 **	imPicWriteRGBRLE	f  write a three-channel PIC file, encoded

 **	imPicWrite8		f  write a one-channel PIC file, dump

 **	imPicWrite8RLE		f  write a one-channel PIC file, encoded

 **	IMPICENCODE8		d  8-bit encode signifier

 **	IMPICENCODE12		d  12-bit encode signifier

 **	IMPICDUMP8		d  8-bit dump signifier

 **	IMPICDUMP12		d  12-bit dump signifier

 **	IMFULLRGBA		d  4 channel bit representation

 **	IMRGBBACK		d  3 channel bit representation

 **	IMSINGLECHAN		d  1 channel bit representation

 **	IMISALPHA		d  PIC alpha mode signifier

 **	IMNOALPHA		d  PIC non alpha mode signifier

 **	IMPICLABEL		d  static array dimension for label

 **	IMPICGAP		d  static array dimension for gap in header 

 **	IMPICSPACE		d  static array dimension for space in header 

 **	IMPICUNUSED		d  static array dimension for unused section 

 **	IMPICHEADERSIZE		d  static size for header & first tile info 

 **	IMPICNUMBER		d  int representation of magic number

 **	IMPICBLOCK		d  static size for optimum block transfers

 **	IMPICWRITESTART		d  int byte offset for writing PIC info

 **

 **     imPicHeaderInfo         t  PIC header

 **     imPicHeaderFields       t  PIC header description for Bin pkg

 **	imPicFr2Buf		m  free 2 storage buffers of type unsigned char

 **	imPicFrTwoBuf		m  free 2 storage buffers: types sdsc_uint16 & unsigned char

 **	imPicPrHeader		f  Debug routine for printing header

 **

 **	imPicReadEncode8	f  convert 8 bit encoded PIC pixels to vfb

 **	imPicReadEncode12	f  convert 12 bit encoded PIC pixels to vfb

 **	imPicReadDump8		f  transfer 8 bit dumped  PIC pixels to vfb

 **	imPicReadDump12		f  transfer 12 bit dumped  PIC pixels to vfb

 **

 **	imPicWriteHeader	f  write header for a Pic file

 **

 **  HISTORY

 **	$Log: /roq/libim/impic.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.17  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.16  1995/06/15  20:51:04  bduggan

 **	changed bzero to memset.  cleaned up indenting.  added casts.

 **

 **	Revision 1.15  1995/04/03  21:33:06  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.14  1995/01/10  23:37:24  bduggan

 **	put in IMMULTI, IMPIPE instead of TRUE/FALSE

 **	uncapitlized i's in local functions

 **	made read/write routines static

 **

 **	Revision 1.13  94/10/03  11:30:23  nadeau

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

 **	Revision 1.12  92/12/03  01:50:39  nadeau

 **	Corrected info messages.

 **	

 **	Revision 1.11  92/11/23  18:42:46  nadeau

 **	Removed use of IMINFOMSG.

 **	

 **	Revision 1.10  92/11/04  12:04:26  groening

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.9  92/10/19  14:16:35  groening

 **	added ImInfo statements

 **	

 **	Revision 1.8  92/08/31  17:31:03  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.7  92/04/09  09:35:57  groening

 **	To make the compiler happy added extern statements.

 **	

 **	Revision 1.6  91/10/03  12:50:36  mcleodj

 **	modified Write routines to determine total

 **	size of pixel info and change location of

 **	where the pixel info starts being written

 **	

 **	Revision 1.5  91/07/10  16:25:56  nadeau

 **	Removed extra include.

 **	

 **	Revision 1.4  91/03/12  11:09:20  nadeau

 **	Changed zeroing loops to bzero() calls.  Optimized mallocs.

 **	

 **	Revision 1.3  91/02/12  11:31:19  nadeau

 **	Removed the tag table checking and VFB conversion now

 **	handled by ImFileRead and ImFileWRite.  Did some major

 **	reorganization of write code to separate different VFB

 **	handling into different routines for direct calling

 **	by ImFileWrite.

 **	

 **	Revision 1.2  91/01/31  12:29:09  mcleodj

 **	compression of procedures and modifications of PIC writing

 **	

 **	Revision 1.1  90/07/24  13:00:32  mcleodj

 **	Initial revision

 **	

 **	

 **/





#include <stdio.h>

#include <errno.h>

#include "sdsc.h"

#include "im.h"

#include "iminternal.h"





/**

 **  FORMAT 

 **	PIXAR picture file 

 **

 **  AKA

 ** 	pic, picio, pixar

 **

 **  FORMAT REFERENCES

 **	- PIXAR Image Computer Programmer's Manual, PIXAR.

 **	- PIXAR Image Computer ChapLibraries User's Guide, PIXAR.

 **	- The RenderMan Companion, Steve Upstill, PIXAR.

 **

 **

 **  CODE CREDITS

 **	Custom development, Jim McLeod, San Diego Supercomputer Center, 1992.

 **

 **  DESCRIPTION

 **

 **  	The Pixar picture format accomodates several formats:

 **	multiple channels; different number of bits; 

 **	encoded and dumped format; and arbitrary picture size.

 **

 **  	Large pictures can be handled by breaking the picture into 

 **  	smaller uniform rectangular pieces called " TILES ."

 **

 **  	Multiple byte data is stored with the LEAST SIGNIFICANT 8 bits

 **	in the first byte ( ie: the first 4 bytes of each file are 

 **					0x80, 0xe8, 0x00, 0x00	   )

 **

 **

 **

 **  PIXAR PICTURE IMAGE STORAGE

 **	Header:

 **		byte number	#bytes		name

 **		  000		  4	  magic number = 0x00, 0x00, 0xe8, 0x80

 **		  004		  2	  version number = 0

 **		  006		 246	  label

 **		  252		  4	  labelptr - for continuation

 **					  gap between data

 **		  416		  2	  picture height (+ num)

 **		  418		  2	  picture width (+ num)

 **		  420		  2	  tile height (+ num, <= pic h)

 **		  422		  2	  tile width (+ num, <= pic w)

 **		  424		  2	  picture format

 **		  426		  2	  picture storage

 **		  428		  2	  blocking factor

 **		  430		  2	  aplha mode (mat-to-black=0,unassoc.=1)

 **		  432		  2	  x offset

 **		  434		  2	  y offset

 **					  space between data

 **		  448		  4	  unused

 **		  452		 28	  unused

 **		  512		 8*n	  tile pointer table (n = # of tiles)

 **

 **	Label:	Ascii description - labels can be arbitrarily long as the 

 **		label pointer in the header points to any continuation  

 **		( allocated in chunks the size of the blocking factor, with 

 **		last four bytes of this	chunk reserved for further blocks )

 **

 **	Pic Format:	Any subset of RGBA channels. Single channel "R pictures"

 **		are recovered as grey scale pictures. RGBA channels correspond

 **		to bits 3210; so that, as an example: 

 **				RGB selection has 1110 binary.

 **

 **	Pic Storage:	Four modes:	(0)	8-bit "encoded"

 **					(1)	12-bit "encoded"

 **					(2)	8-bit dumped

 **					(3)	12-bit dumped

 **			(note: 12 bits stored as two bytes)

 **

 **	Encoded Tiles:	Pixel information is broken into "packets."

 **		NO packet may span multiple scanlines.

 **		NO packet spans multiple disk blocks.

 **		HOWEVER, each scanline may have ANY COMBINATION of the

 **			four types of packets.

 **

 **		flag	count	    RGBA	   RGB	 	  R

 **		 1	 c	RGBARGBARGBA.	RGBRGBRGB..	RRRR..

 **		 2	 c	lRGBAlRGBA...	lRGBlRGB... 	lRlR..

 **		 3	 c	ARGBRGBRGB... 	   n/a		 n/a

 **		 4	 c	AlRGBlRGB....	   n/a		 n/a

 **

 **		 0				signifies end of block

 **

 **		flag and count are packed into 16 bits as follows:

 **			1st byte:	count<0:7>

 **			2nd byte:	flag<0:3>	count<8:11>

 **

 **		if flag = 1 or 3: c = 1 less than the number of dummped pixels

 **		if flag = 2 or 4: c = 1 less than the number of run lengths

 **				    and l = number of repetitions 

 **				   (ie: l=0 indicates 1 instance 0 repetitions)

 **		Zeros fill out a block.

 **

 **	Dummped Tiles:	No excess bytes are used.

 **			(ex: RGB format: RGBRGBRGBRGB.....)

 **

 **	Blocking Factor:  Optimum disk transfer chunk ( normally 8192 bytes )

 **

 **	Alpha mode: Has something to do with background of image but does not

 **			influence the current implementation of this im utility.

 **

 **	Picture Offsets: For proper restoration of image on Pixar buffer window.

 **

 **	Tiles:  Each tile has a four byte pointer and four byte length.

 **		Tiles are numbered across from 

 **			0 to (num_x_tiles*num_y_tiles -1 )

 **			where num_x_tiles is 1+(pic_width-1)/tile_width

 **			and   num_y_tiles is 1+(pic_height-1)/tile_height.

 **

 **		Tile 0 is in the UPPER LEFT CORNER:  pic_pixel(0,0)=tile_0(0,0).

 **		Tiles can extend down or to the right beyond the picture 

 **		boundaries, but the pixels outside the picture and inside the

 **		tile are undefined with regard to the picture.

 **			 ( note: they are still properly encoded.)

 **		A tile pointer of 0 indicates null tile, a positive 

 **		pointer and a count of -1 indicates an incomplete tile.

 **

 **		( IMPLEMENTATION NOTE:  Tiles have NOT been handled by the

 **		  initial revision of the READ routine.  It is assumed that

 **		  all PIC files will only have ONE tile which is of the same

 **		  dimension of the picture.

 **		  For the WRITE routine, the PIC file created will only have

 **		  one tile which is the entire image.  )

 **		  

 **/



/**

 **  PIC - Pixar Picture file

 **      For information on these structures, how to use them, etc. please

 **      see imfmt.c.

 **/

#ifdef __STDC__

static int imPicRead( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable );

static int imPicWrite8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

static int imPicWriteRGB( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

static int imPicWriteRGBA( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

static int imPicWrite8RLE( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

static int imPicWriteRGBRLE( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable); 

static int imPicWriteRGBARLE( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

#else

static int imPicRead( );

static int imPicWrite8( ),    imPicWriteRGB( ),    imPicWriteRGBA( );

static int imPicWrite8RLE( ), imPicWriteRGBRLE( ), imPicWriteRGBARLE( );

#endif



static char *imPicNames[ ]  = { "pic", "picio", "pixar", NULL };

static unsigned char imPicMagicNumber[ ] = { 0x80, 0xE8, 0x00, 0x00 };

static ImFileFormatReadMap imPicReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IN,1,8,       RLE,    IMVFBINDEX8,    0 },

        { IN,1,8,       0,      IMVFBINDEX8,    0 },

        { IN,1,12,      RLE,    IMVFBINDEX16,   0 },

        { IN,1,12,      0,      IMVFBINDEX16,   0 },

        { RGB,3,8,      RLE,    IMVFBRGB,       0 },

        { RGB,3,8,      0,      IMVFBRGB,       0 },

        { RGB,3,12,     RLE,    IMVFBRGB,       0 },

        { RGB,3,12,     0,      IMVFBRGB,       0 },

        { RGB,3,8,      RLE|A,  IMVFBRGB,       A },

        { RGB,3,8,      A,      IMVFBRGB,       A },

        { RGB,3,12,     RLE|A,  IMVFBRGB,       A },

        { RGB,3,12,     A,      IMVFBRGB,       A },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imPicWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBINDEX8,  0,      IN,1,8,         RLE,    imPicWrite8RLE },

        { IMVFBINDEX8,  0,      IN,1,8,         0,      imPicWrite8 },



        { IMVFBRGB,     0,      RGB,3,8,        RLE,    imPicWriteRGBRLE },

        { IMVFBRGB,     A,      RGB,3,8,        RLE|A,  imPicWriteRGBARLE},

        { IMVFBRGB,     0,      RGB,3,8,        0,      imPicWriteRGB },

        { IMVFBRGB,     A,      RGB,3,8,        A,      imPicWriteRGBA },

        { -1,           0,      -1,             0,      NULL },

};





static ImFileMagic imFilePicMagic []=

{

	{ 0, 4, imPicMagicNumber },

	{ 0, 0, NULL },

};





ImFileFormat ImFilePicFormat =

{

	imPicNames, "PIXAR picture file",

	"PIXAR",

	"8- and 12-bit greyscale image files, (no CLT).  24-bit\n\
(8-bit/channel) and 36-bit (12-bit/channel) RGB image files.\n\
Uncompressed (dump) and RLE-compressed (packet encoding) files.",
	"8-bit greyscale image files (no CLT).  24-bit (8-bit/channel)\n\
RGB image files.  Uncompressed (dump) and RLE-compressed (packet encoding)\n\
files.",

	imFilePicMagic,

	IMNOMULTI, IMPIPE,

	IMNOMULTI, IMNOPIPE,

	imPicRead, imPicReadMap, imPicWriteMap

};













#ifndef NULL

#define NULL	(0)

#endif



FILE *fperr;



/*

 *  TYPEDEF & STRUCTURE

 *	imPicHeaderInfo		-  PIC file header information

 *	imPicHeaderFields 	-  Description for Binio package

 *

 *  DESCRIPTION

 *	A PIC file's header contains the image's width, height, and 

 *	picture format and storage type	and many other fields.

 */



#define IMPICENCODE8		(0)

#define IMPICENCODE12		(1)

#define IMPICDUMP8		(2)

#define IMPICDUMP12		(3)

#define IMFULLRGBA		(15)

#define IMRGBBACK		(14)

#define IMSINGLECHAN		(8)

#define IMISALPHA		(1)

#define IMNOALPHA		(0)

#define IMPICLABEL		(246)

#define IMPICGAP		(160)

#define IMPICSPACE		(12)

#define IMPICUNUSED		(60)

#define IMPICHEADERSIZE		(520)

#define IMPICNUMBER		(59520)

#define IMPICBLOCK		(8192)

#define IMPICWRITESTART		(8192)



	/* NOTE: IMPICWRITESTART is assumed to be bigger than IMPICHEADERSIZE

	 *	 If this is changed, don't make it less than.

	 * BIGNOTE:  Pixar Machines ( from which pic images originated )

	 *	seem to REQUIRE that the byte information for the pixel

	 *	info start at least one full block after the header ( ie: 

	 *	at a byte count of 8192 ).  Experiments to begin writing the 

	 *	data before byte count 8192 resulted in the failure of the

	 *	PIXAR machines to correctly display an image.

	 *	Therefore, if IMPICWRITESTART must be changed, only increase

	 *	this constant.

	 *

	 *	ALSO, the read algorithm assumes ( since it was not clearly

	 *	stated in the PIXAR specification sheets for the pic format )

	 *	that the pixel information is stored in block size pieces

	 *	starting from the location indicated by the tile_pointer.

	 *	(ie: if the tile pointer does not begin at the beginning of

	 *		an integer number of the block size, ( in this case

	 *		8192 or 16384 ... ) a full block of information

	 *		will still be read starting from that tile position 

	 *		as well as when writing the pixel information:

	 *		instead of starting the number of bytes written

	 *		initially to IMPICWRITESTART % IMPICBLOCKSIZE, this 

	 *		numWritten count begins at 0 )

	 */



typedef struct imPicHeaderInfo

{

	unsigned int	magic_number;		/* 0x00,0x00,0xe8,0x80(MSBshown)*/

	unsigned short	version_number;		/* Zero for current release 	*/

	unsigned char	label[IMPICLABEL];	/* Ascii description of image	*/

	long	labelptr;		/* Ptr to label continuation	*/

	unsigned char	gap[IMPICGAP];		/* Unused space in header info	*/

	unsigned short	pic_height;		/* Pixel height of full image	*/

	unsigned short	pic_width;		/* Pixel width of full image	*/

	unsigned short	tile_height;		/* Pixel height of each tile	*/

	unsigned short	tile_width;		/* Pixel width of each tile	*/

	unsigned short	pic_format;		/* four bits designating RGBA	*/

	unsigned short	pic_storage;		/* encoding and number of bits  */

	unsigned short	block;			/* optimum disk transfer chunk	*/

	unsigned short 	alpha;			/* matted-to-black:0 unassoc.:1	*/

	unsigned short	x_offset;		/* horizontal offset for picture*/

	unsigned short	y_offset;		/* vertical offest for picture	*/

	unsigned char	space[IMPICSPACE];	/* mysterious void in header	*/

	unsigned int	intspace;		/* Unused int space		*/

	unsigned char	unused[IMPICUNUSED];	/* Unused expansion space	*/

	long	tile_ptr;		/* Location of tile		*/

	unsigned int	tile_len;		/* Length of tile		*/

} imPicHeaderInfo;





BinField imPicHeaderFields[ ] =

{

	{ UINT, 4, 1 },			/* Magic number			*/

	{ USHORT, 2, 1 },		/* Zero for current release	*/

	{ UCHAR, 1, IMPICLABEL },	/* Ascii description of image	*/

	{ LONG, 4, 1 },			/* Ptr to label continuation	*/

	{ UCHAR, 1, IMPICGAP },		/* Unused space in header info	*/

	{ USHORT, 2, 1 },		/* Pixel height of full image	*/

	{ USHORT, 2, 1 },		/* Pixel width of full image	*/

	{ USHORT, 2, 1 },		/* Pixel height of each tile	*/

	{ USHORT, 2, 1 },		/* Pixel width of each tile	*/

	{ USHORT, 2, 1 },		/* four bits designating RGBA	*/

	{ USHORT, 2, 1 },		/* encoding and number of bits	*/

	{ USHORT, 2, 1 },		/* optimum disk transfer chunk	*/

	{ USHORT, 2, 1 },		/* matted-to-black:0 unassoc.:1	*/

	{ USHORT, 2, 1 },		/* horizontal offset for picture*/

	{ USHORT, 2, 1 },		/* vertical offset for picture	*/

	{ UCHAR, 1, IMPICSPACE },	/* mysterious void in header	*/

	{ UINT, 4, 1 },			/* Unused int space		*/

	{ UCHAR, 1, IMPICUNUSED },	/* Unused expansion space 	*/

	{ LONG, 4, 1 },			/* Location of tile		*/

	{ UINT, 4, 1 },			/* Length of tile		*/

	{ 0, 0, 0 }

};



/*

 *  MACRO

 *	imPicFr2Buf -  Free two chunks of memory

 *

 *  DESCRIPTION

 *	Call free two times to free the two chunks of memory pointed to by

 *	a-b. 

 */



#define imPicFr2Buf(a,b) 	free((char*)(a)); free((char*)(b))



/*

 *  MACRO

 *	imPicFrTwoBuf -  Free two chunks of memory of different types

 *

 *  DESCRIPTION

 *	Call free two times to free the two chunks of memory pointed to by

 *	a-b. 

 */



#define imPicFrTwoBuf(a,b) 	free((sdsc_uint16*)(a)); free((char*)(b))





#ifdef __STDC__ 

static int imPicReadEncode8( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imPicHeaderInfo *h, int fields );

static int imPicReadDump8( int ioType, int fd, FILE* fp, TagTable *flags, TagTable *tagTable, imPicHeaderInfo *h, int fields, int channels );

static int imPicReadEncode12( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imPicHeaderInfo *h, int fields );

static int imPicReadDump12( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imPicHeaderInfo *h, int fields, int channels );

#else

static int imPicReadEncode8( );

static int imPicReadDump8( );

static int imPicReadEncode12( );

static int imPicReadDump12( );

#endif





/*

 *  FUNCTION

 *	imPicRead	-  read a PIXAR picture file

 *

 *  DESCRIPTION

 *	The file header is read and the size of the image is detemined.

 *	The type of PIXAR storage format and the type of vfb are determined.  

 *	The appropriate private function is called to allocate the vfb and

 *	complete the read of the PIXAR file.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__ 

imPicRead( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable )

#else

imPicRead( ioType, fd, fp, flags, tagTable )

        int         ioType;             /* I/O flags                    */

        int         fd;                 /* Input file descriptor        */

        FILE       *fp;                 /* Input file pointer           */

        TagTable   *flags;              /* Format flags                 */

        TagTable   *tagTable;           /* Tag list to add to           */

#endif

{

	imPicHeaderInfo	header;		/* PIC file header		*/

	int		fields;		/* VFB alocation information	*/

	int		channels;	/* Num of bytes for each pixel	*/

	char		 message[100];	/* ImInfo message		*/



	/*

	 *  PIC files are usually generated on PIXARs, which stores 

	 *  MULTIPLE-byte data with the least significant 8 in the first 

	 *  byte. This is an LBF byte order. 

	 *  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINLBF );



	/*

	 *  Read in the header.

	 */

	if (ImBinReadStruct(ioType, fd, fp, &header, imPicHeaderFields ) == -1)

	{

		ImReturnBinError();

	}



	/*

	 * These lines are used if a flag is set to printout file info.

	 */

	sprintf (message,"%d",header.version_number);

	ImInfo ("Version",message);

	ImInfo ("Byte Order","Least Significant Byte First");

	sprintf (message, "%d x %d",header.pic_width, header.pic_height);

	ImInfo ("Resolution",message); 

	if ( (header.pic_storage == IMPICENCODE12) ||

	     (header.pic_storage == IMPICDUMP12)	)

	{

	  switch ( header.pic_format )

	  {

	    case IMFULLRGBA:

		sprintf (message, "48-bit RGB and Alpha");



	    case IMRGBBACK:

		sprintf (message, "36-bit RGB");



	    case IMSINGLECHAN:

		sprintf (message, "12-bit Grayscale");

	  }

	}

	else

	{

	  switch ( header.pic_format )

	  {

	    case IMFULLRGBA:

		sprintf (message, "32-bit RGB and Alpha");



	    case IMRGBBACK:

		sprintf (message, "24-bit RGB");



	    case IMSINGLECHAN:

		sprintf (message, "8-bit Grayscale");

	  }

	}

	ImInfo( "Type", message );

	if ( (header.pic_storage == IMPICENCODE12) ||

	     (header.pic_storage == IMPICENCODE8) )

	ImInfo ("Compression Type","Run Length Encoded (RLE)")

	else

	ImInfo ("Compression Type","none (dump)");

	if (header.pic_format==IMFULLRGBA) ImInfo ("Alpha Channel","8-bit")

	else ImInfo ("Alpha Channel","none");





	/*

	 * Check the important header parameters for picture format

	 *	(1) Were we given a pixar PIC picture file ?

  	 */

	if ( header.magic_number != IMPICNUMBER )

	{

		ImErrNo = IMEMAGIC;

		ImErrorFatal ( ImQError(), -1, ImErrNo );

	}



	/*

	 *	(2) Do we have just ONE tile ?

 	 * 	(NOTE: This is a temporary consideration for multiple tiles )

	 */

	if ( 	( header.pic_width != header.tile_width ) || 

		( header.pic_height != header.tile_height ) )

	{

		ImErrorFatal( "PIXAR pic files with multiple tiles not yet implemented", 

			-1, IMESYNTAX );

	}



	/*

	 *	(3) What pixar picture format have we been given?

	 *	Set the appropriate vfb fields for future construction.

	 */

	switch ( header.pic_format )

	{

	    case IMFULLRGBA:



		fields = IMVFBRGB | IMVFBALPHA;

		channels = 4;

		break;



	    case IMRGBBACK:



		fields = IMVFBRGB;

		channels = 3;

		break;



	    case IMSINGLECHAN:



		if ( (header.pic_storage == IMPICENCODE12) ||

		     (header.pic_storage == IMPICDUMP12)	)

			fields = IMVFBINDEX16;



		else 

			fields = IMVFBINDEX8;



		channels = 1;

		break;



	    default:

		

		ImErrNo = IMESYNTAX;

		ImErrorFatal( "Unknown Pic picture format type", -1, ImErrNo );

	}



	/*

	 *	(4) How is the pixar picture file stored?

	 *	Call specific routine for each case.

	 */

	switch ( header.pic_storage )

	{

	    case IMPICENCODE8:



		return ( imPicReadEncode8 ( ioType, fd, fp, flags, tagTable, 

					&header, fields ) ); 



	    case IMPICDUMP8:



		return ( imPicReadDump8 ( ioType, fd, fp, flags, tagTable, 

					&header, fields, channels ) ); 



	    case IMPICENCODE12:



		return ( imPicReadEncode12 ( ioType, fd, fp, flags, tagTable, 

					&header, fields ) ); 



	    case IMPICDUMP12:



		return ( imPicReadDump12 ( ioType, fd, fp, flags, tagTable, 

					&header, fields, channels ) ); 



	    default:



		ImErrNo = IMESYNTAX;

		ImErrorFatal( "Uknown Pic storage mode", -1, ImErrNo );

	}

}











/*

 *  FUNCTION

 *	imPicReadEncode8	-  read a PIXAR 8-bit encoded picture file 

 *

 *  DESCRIPTION

 *	Space is allocated for the VFB and the PIXAR pixel packets are 

 *	decoded and expanded if necessary into run buffers of straight

 *	RGB values. These values are transfered to the VFB and the 

 *	completed VFB added to the tag list.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__ 

imPicReadEncode8( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imPicHeaderInfo *h, int fields )

#else

imPicReadEncode8( ioType, fd, fp, flags, tagTable, h, fields )

        int         ioType;             /* I/O flags                    */

        int         fd;                 /* Input file descriptor        */

        FILE       *fp;                 /* Input file pointer           */

        TagTable   *flags;              /* Format flags                 */

        TagTable   *tagTable;           /* Tag list to add to           */

	imPicHeaderInfo *h;		/* The Pixar picture header	*/

	int	    fields;		/* The flags for the vfb	*/

#endif

{

	ImVfb       *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	unsigned char	    *info;	/* Buffer for PIXAR pic data	*/

	unsigned char	    *infoptr;	/* Pointer to info data		*/

	int x,y;			/* Convenient short names	*/

	int i;				/* Loop counters		*/

	int	     lcnt;		/* Number of pixels in line	*/

	int 	    byte1; 		/* 1st byte of 16-bit id	*/	

	int 	    byte2;		/* 2nd byte of 16-bit id	*/

	int 	    flag; 		/* Signifies the type of packet */

	int	    bytecount;		/* Tracks bytes for block read  */

	int count;			/* Number of pixel/runs in pack */

	int lambda;			/* Number of pixels in run	*/

	unsigned char 	    alpha;	/* Holds constant alpha values  */



	x = h->pic_width;

	y = h->pic_height;

	bytecount = 0;



	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (vfb = ImVfbAlloc( x, y, fields )) == IMVFBNULL )

	{

		ImErrNo = IMEMALLOC;

		ImErrorFatal( ImQError(), -1, ImErrNo );

	}



	/*

	 * Position the pointer at the beginning of the first scanline

	 */

	pptr = ImVfbQFirst( vfb );



	/*

	 * Allocate the data input buffer for the info to be read

	 */ 

	ImMalloc( info, unsigned char *, h->block);

	infoptr = info;



	/*

	 * First read in unused portion up to the first tile

	 */

	if ( (h->tile_ptr - IMPICHEADERSIZE) > 0 )

	{

		if ( ioType & IMFILEIOFILE )

		{

			ImSeek(ioType,fd,fp,h->tile_ptr-IMPICHEADERSIZE, 1 );

		}

		else

		{

			unsigned char *empty;	/* Local unused section of pic data */



			ImMalloc(empty, unsigned char*, h->tile_ptr-IMPICHEADERSIZE );

			if (ImBinRead(ioType, fd, fp, empty, UCHAR, 

				1, h->tile_ptr - IMPICHEADERSIZE) == -1)

			{

				free( (char *)empty );

				free( (char *)info );

				ImReturnBinError();

			}



			free( (char *)empty );

		}

	}



	/*

	 * Then read first block of pixel info

	 * (note: if pixel info does not start at logical beginning of a

	 *	  block, a full block of info will still be read in ) 

	 */

	if (ImBinRead(ioType, fd, fp, info, UCHAR, 1, h->block)==-1)

	{

		free( (char *)info );

		ImReturnBinError();

	}



	/* 

	 * Determine what PIC file format we have. 

	 * In each case:

	 * Loop through the scan lines.  Decode packets of info for each line.

	 * Placing each decoded packet directly into the vfb.

	 *

	 * (NOTE: No default needed in switch as pic_format checked before 

	 *	  this procedure)

	 */

	switch ( h->pic_format )

	{

	    case IMFULLRGBA:



		for ( i=0; i<y; i++ )

		{

		    lcnt = 0;

		    while ( lcnt < x )

		    {

			/*

			 * Decode packets of the 4 channel info based on the 

			 * type of packet that is read from the picture file.

			 * Directly place the byte info in the vfb

			 */





			byte1 = *infoptr++;

			byte2 = *infoptr++;



			bytecount += 2;



			flag = byte2 >> 4;

			count = ( (byte2 & 0x0f) << 8 ) | byte1;





			switch ( flag )

			{

			    case 1:

				lcnt = lcnt + count +1;

				while ( count-- >= 0 )

				{

				    ImVfbSRed(   vfb, pptr, (*infoptr++) );

				    ImVfbSGreen( vfb, pptr, (*infoptr++) );

				    ImVfbSBlue(  vfb, pptr, (*infoptr++) );

				    ImVfbSAlpha( vfb, pptr, (*infoptr++) );





				    ImVfbSInc(   vfb, pptr );

				    bytecount += 4;

				}

				break;



			    case 2:

				while ( count-- >= 0 )

				{

				    lambda = *infoptr++;

				    bytecount++;

					



				    lcnt = lcnt + lambda +1;

				    while ( lambda-- >= 0 )

				    {

					ImVfbSRed(   vfb, pptr, (*(infoptr))  );

					ImVfbSGreen( vfb, pptr, (*(infoptr+1)));

					ImVfbSBlue(  vfb, pptr, (*(infoptr+2)));

					ImVfbSAlpha( vfb, pptr, (*(infoptr+3)));

				        ImVfbSInc(   vfb, pptr );

				    }





				    infoptr += 4;

				    bytecount += 4;

				}

				break;



			    case 3:

				alpha = *infoptr++;

				bytecount++;



				lcnt = lcnt + count +1;

				while ( count-- >= 0 )

				{

				    ImVfbSRed(   vfb, pptr, (*infoptr++) );

				    ImVfbSGreen( vfb, pptr, (*infoptr++) );

				    ImVfbSBlue(  vfb, pptr, (*infoptr++) );

				    ImVfbSAlpha( vfb, pptr, alpha );





				    ImVfbSInc(   vfb, pptr );

				    bytecount += 3;

				}

				break;



			    case 4:

				alpha = *infoptr++;

				bytecount++;



				while ( count-- >= 0 )

				{

				    lambda = *infoptr++;

				    bytecount++;





				    lcnt = lcnt + lambda +1;

				    while ( lambda-- >= 0 )

				    {

					ImVfbSRed(   vfb, pptr, (*(infoptr))  );

					ImVfbSGreen( vfb, pptr, (*(infoptr+1)));

					ImVfbSBlue(  vfb, pptr, (*(infoptr+2)));

					ImVfbSAlpha( vfb, pptr, alpha );

				        ImVfbSInc(   vfb, pptr );

				    }



				    infoptr += 3;

				    bytecount += 3;

				}

				break;

	

			    case 0:		

				/* 

			 	 * This signifies end of block 

			 	 */

				bytecount = h->block;

				break;



	    		    default:		

				ImErrNo = IMESYNTAX;

				ImErrorFatal( ImQError(), -1, ImErrNo );

			}





			/* If the end of a block has been reached, 

			 * read next block of picture info.

			 */

			if (  bytecount >= h->block-2  )

		    	{





			    if (ImBinRead(ioType, fd, fp, info, UCHAR,

					  1, h->block) == -1)

			    {

				free( (char *)info );

				ImReturnBinError();

			    }



			    infoptr = info;

			    bytecount = 0;





			}

		    }

		}



		break;



	    case IMRGBBACK:	



		for ( i=0; i<y; i++ )

		{

		    lcnt = 0;

		    while ( lcnt < x )

		    {

			/*

			 * Decode packets of the 3 channel info based on the 

			 * type of packet that is read from the picture file.

			 * Directly place the byte info in the vfb

			 */



			byte1 = *infoptr++;

			byte2 = *infoptr++;



			bytecount += 2;



			flag = byte2 >> 4;

			count = ( (byte2 & 0x0f) << 8 ) | byte1;





			switch ( flag )

			{

			    case 1:

				lcnt = lcnt + count +1;

				while ( count-- >= 0 )

				{

				    ImVfbSRed(   vfb, pptr, (*infoptr++) );

				    ImVfbSGreen( vfb, pptr, (*infoptr++) );

				    ImVfbSBlue(  vfb, pptr, (*infoptr++) );



				    ImVfbSInc(   vfb, pptr );

				    bytecount += 3;

				}

				break;



			    case 2:

				while ( count-- >= 0 )

				{

				    lambda = *infoptr++;

				    bytecount++;

					



				    lcnt = lcnt + lambda +1;

				    while ( lambda-- >= 0 )

				    {

					ImVfbSRed(   vfb, pptr, (*(infoptr))  );

					ImVfbSGreen( vfb, pptr, (*(infoptr+1)));

					ImVfbSBlue(  vfb, pptr, (*(infoptr+2)));

				        ImVfbSInc(   vfb, pptr );

				    }





				    infoptr += 3;

				    bytecount += 3;

				}

				break;



			    case 0:		

				/* 

			 	 * This signifies end of block 

			 	 */

				bytecount = h->block;

				break;



	    		    default:		

				ImErrNo = IMESYNTAX;

				ImErrorFatal( ImQError(), -1, ImErrNo );

			}





			/* 

			 * If the end of a block has been reached, 

			 * read next block of picture info.

			 */

			if (  bytecount >= h->block-2  )

		    	{





			    if (ImBinRead(ioType, fd, fp, info, UCHAR,

					  1, h->block) == -1)

			    {

				free( (char *)info );

				ImReturnBinError();

			    }



			    infoptr = info;

			    bytecount = 0;





			}

		    }

		}



		break;



	    case IMSINGLECHAN:	



		for ( i=0; i<y; i++ )

		{

		    lcnt = 0;

		    while ( lcnt < x )

		    {

			/*

			 * Decode packets of the 1 channel info based on the 

			 * type of packet that is read from the picture file.

			 * Directly place the byte info in the vfb

			 */



			byte1 = *infoptr++;

			byte2 = *infoptr++;



			bytecount += 2;



			flag = byte2 >> 4;

			count = ( (byte2 & 0x0f) << 8 ) | byte1;



			switch ( flag )

			{

			    case 1:

				lcnt = lcnt + count +1;

				while ( count-- >= 0 )

				{

				    ImVfbSIndex8( vfb, pptr, (*infoptr++) );

				    ImVfbSInc(    vfb, pptr );

				    bytecount += 1;

				}

				break;



			    case 2:

				while ( count-- >= 0 )

				{

				    lambda = *infoptr++;

				    bytecount++;

					

				    lcnt = lcnt + lambda +1;

				    while ( lambda-- >= 0 )

				    {

					ImVfbSIndex8( vfb, pptr, (*infoptr) );

				        ImVfbSInc(    vfb, pptr );

				    }



				    infoptr++;

				    bytecount++;

				}

				break;



			    case 0:		

				/* 

			 	 * This signifies end of block 

			 	 */

				bytecount = h->block;

				break;



	    		    default:		

				ImErrNo = IMESYNTAX;

				ImErrorFatal( ImQError(), -1, ImErrNo );

			}



			/* 

			 * If the end of a block has been reached, 

			 * read next block of picture info.

			 */

			if (  bytecount >= h->block-2  )

		    	{



			    if (ImBinRead(ioType, fd, fp, info, UCHAR,

					  1, h->block) == -1)

			    {

				free( (char *)info );

				ImReturnBinError();

			    }



			    infoptr = info;

			    bytecount = 0;



			}

		    }

		}

	}



	/*

	 * Free the pixel storage arrays 

	 */

	free( (char *)info );



	/*

	 *  Add the VFB to the tagTable.

	 */

	TagTableAppend( tagTable, TagEntryAlloc( "image vfb", POINTER, &vfb ) );





	return ( 1 );

}











/*

 *  FUNCTION

 *	imPicReadDump8	-  read a PIXAR 8-bit dumped picture file 

 *

 *  DESCRIPTION

 *	Space is allocated for the VFB and the PIXAR dumped pixels are 

 *	are directly transfered to the VFB based on the number of channels 

 *	and the completed VFB is added to the tag list.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__ 

imPicReadDump8( int ioType, int fd, FILE* fp, TagTable *flags, TagTable *tagTable, imPicHeaderInfo *h, int fields, int channels )

#else

imPicReadDump8( ioType, fd, fp, flags, tagTable, h, fields, channels )

        int         ioType;             /* I/O flags                    */

        int         fd;                 /* Input file descriptor        */

        FILE       *fp;                 /* Input file pointer           */

        TagTable   *flags;              /* Format flags                 */

        TagTable   *tagTable;           /* Tag list to add to           */

	imPicHeaderInfo *h;		/* The Pixar picture header	*/

	int	    fields;		/* The flags for the vfb	*/

	int	    channels;		/* The Pixar channel format	*/

#endif

{

	ImVfb       *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	unsigned char	    *info;		/* Buffer for PIXAR pic data	*/

	unsigned char	    *infoHead;		/* The start of the info buffer */

	unsigned char	    *empty;		/* Buffer for unused area	*/

	int x,y;		/* Convenient short names	*/

	int i,j;		/* Loop counters		*/



	x = h->pic_width;

	y = h->pic_height;



	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (vfb = ImVfbAlloc( x, y, fields )) == IMVFBNULL )

	{

		ImErrNo = IMEMALLOC;

		ImErrorFatal( ImQError(), -1, ImErrNo );

	}



	/*

	 * Position the pointer at the beginning of the first scanline

	 */

	pptr = ImVfbQFirst( vfb );



	/*

	 * Allocate buffer large enough for one scanline of pixel info 

	 * and for the unused portion between the header and dumped pixels.

	 * Store start of info buffer for future reads.

	 */ 

	ImMalloc( info, unsigned char *, (channels*x));

	infoHead = info;



	/*

	 * First read in unused portion up to the first tile

	 */

	if ( (h->tile_ptr - IMPICHEADERSIZE) > 0 )

	{

		if ( ioType & IMFILEIOFILE )

		{

			ImSeek(ioType,fd,fp,h->tile_ptr-IMPICHEADERSIZE, 1 );

		}

		else

		{

			ImMalloc(empty, unsigned char*, h->tile_ptr - IMPICHEADERSIZE );

			if (ImBinRead(ioType, fd, fp, empty, UCHAR, 

				1, h->tile_ptr - IMPICHEADERSIZE) == -1)

			{

				free( (char *)empty );

				free( (char *)info );

				ImReturnBinError();

			}

		}

	}



	/* 

	 * Determine what PIC Image format we have and 

	 * Loop through the scan lines.  Dump pixels from info for each line

	 * into the vfb based on # of channels.

	 *

	 * (NOTE: No default needed as the error checking for form done before)

	 */

	switch ( h->pic_format )

	{

	    case IMFULLRGBA:



		for ( i=0; i<y; i++ )

		{

			/*

	 		 * read a scanline of pixel info to be dumped

	 		 */

			if (ImBinRead(ioType,fd,fp,info,UCHAR,1,4*x) == -1)

			{

				free( (char *)info );

				ImReturnBinError();

			}



			for ( j=0; j < x; j++ )

			{

				ImVfbSRed(   vfb, pptr, *info++ );

				ImVfbSGreen( vfb, pptr, *info++ );

				ImVfbSBlue(  vfb, pptr, *info++ );

				ImVfbSAlpha( vfb, pptr, *info++ );



				ImVfbSInc(   vfb, pptr );

			}

	

			info = infoHead;

		}

		break;



	    case IMRGBBACK:	



		for ( i=0; i<y; i++ )

		{

			/*

	 		 * read a scanline of pixel info to be dumped

	 		 */

			if (ImBinRead(ioType,fd,fp,info,UCHAR,1,3*x) == -1)

			{

				free( (char *)info );

				ImReturnBinError();

			}



			for ( j=0; j < x; j++ )

			{

				ImVfbSRed(   vfb, pptr, *info++ );

				ImVfbSGreen( vfb, pptr, *info++ );

				ImVfbSBlue(  vfb, pptr, *info++ );

				ImVfbSInc(   vfb, pptr );

			}

	

			info = infoHead;

		}

		break;



	    case IMSINGLECHAN:	



		for ( i=0; i<y; i++ )

		{

			/*

	 		 * read a scanline of pixel info to be dumped

	 		 */

			if (ImBinRead(ioType,fd,fp,info,UCHAR,1,x) == -1)

			{

				free( (char *)info );

				ImReturnBinError();

			}



			for ( j=0; j < x; j++ )

			{

				ImVfbSIndex8(  vfb, pptr, *info++ );

				ImVfbSInc(   vfb, pptr );

			}

	

			info = infoHead;

		}

	}



	/*

	 *  Add the VFB to the tagTable.

	 */

	TagTableAppend( tagTable, TagEntryAlloc( "image vfb", POINTER, &vfb ) );



	/*

	 * Free the pixel storage arrays

	 */

	free( (char *)info );



	return ( 1 );

}











/*

 *  FUNCTION

 *	imPicReadEncode12	-  read a PIXAR 12-bit encoded picture file 

 *

 *  DESCRIPTION

 *	Space is allocated for the VFB and the PIXAR pixel packets are 

 *	decoded and expanded if necessary into run buffers of straight

 *	RGB values. These values are transfered to the VFB and the 

 *	completed VFB added to the tag list.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__ 

imPicReadEncode12( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imPicHeaderInfo *h, int fields )

#else

imPicReadEncode12( ioType, fd, fp, flags, tagTable, h, fields )

        int         ioType;             /* I/O flags                    */

        int         fd;                 /* Input file descriptor        */

        FILE       *fp;                 /* Input file pointer           */

        TagTable   *flags;              /* Format flags                 */

        TagTable   *tagTable;           /* Tag list to add to           */

	imPicHeaderInfo *h;		/* The Pixar picture header	*/

	int	    fields;		/* The flags for the vfb	*/

#endif

{

	ImVfb       *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	sdsc_uint16	    *info;		/* Buffer for PIXAR pic data	*/

	sdsc_uint16	    *infoptr;		/* Pointer for info data    	*/

	unsigned char	    *empty;		/* Unused section of PIC file	*/

	int x,y;		/* Convenient short names	*/

	int i,j;		/* Loop counters		*/

	int	     lcnt;		/* Number of pixels in line	*/

	int         twobyte;		/* Holds the 16-bit id		*/

	int 	    flag; 		/* Signifies the type of packet */

	int	    bytecount;		/* Tracks bytes for block read  */

	int count;		/* Number of pixel/runs in pack */

	int lambda;		/* Number of pixels in run	*/

	unsigned char 	    alpha;		/* Holds constant alpha values  */



	x = h->pic_width;

	y = h->pic_height;

	bytecount = 0;



	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (vfb = ImVfbAlloc( x, y, fields )) == IMVFBNULL )

	{

		ImErrNo = IMEMALLOC;

		ImErrorFatal( ImQError(), -1, ImErrNo );

	}



	/*

	 * Position the pointer at the beginning of the first scanline

	 */

	pptr = ImVfbQFirst( vfb );



	/*

	 * Allocate the data input buffer for the info to be read

	 */ 

	ImMalloc( info, sdsc_uint16 *, h->block);

	infoptr = info;



	/*

	 * First read in unused portion up to the first tile

	 */

	if ( (h->tile_ptr - IMPICHEADERSIZE) > 0 )

	{

		if ( ioType & IMFILEIOFILE )

		{

			ImSeek(ioType,fd,fp,h->tile_ptr-IMPICHEADERSIZE, 1);

		}

		else

		{

			ImMalloc(empty, unsigned char*, h->tile_ptr - IMPICHEADERSIZE );

			if (ImBinRead(ioType, fd, fp, empty, UCHAR, 

				1, h->tile_ptr - IMPICHEADERSIZE) == -1)

			{

				free( (char *)empty );

				free( (char *)info );

				ImReturnBinError();

			}

		}

	}



	/*

	 * Then read first block of pixel info

	 */

	if (ImBinRead(ioType, fd, fp, info, UINT16, 2, h->block/2) == -1)

	{

		free( (char *)info );

		ImReturnBinError();

	}



	/* 

	 * Determine what PIC file format we have.

	 * In each case:

	 * Loop through the scan lines.  Decode packets of info for each line.

	 * Placing each decoded packet directly into the vfb.

	 *

	 * (NOTE: No default needed as pic_format checked before this procedure)

	 */

	switch ( h->pic_format )

	{

	    case IMFULLRGBA:



		/*

		 * Since Vfb can only hold 8-bit RGB values, the 12-bit values

		 * must be truncated.  Need to warn the user that this occurs

		 */

		ImErrorWarning( "12-bit data being reduced to 8-bit data", 

				-1, IMESYNTAX );



		for ( i=0; i<y; i++ )

		{

		    lcnt = 0;

		    while ( lcnt < x )

		    {

			/*

			 * Decode packets of the 4 channel info based on the 

			 * type of packet that is read from the picture file.

			 * Directly place the byte info in the vfb

			 */



			twobyte = *infoptr++;

			bytecount += 2;



        		/* 

			 * Since the packed 16 bits was read in as a single 

			 * sdsc_int16 value with LSB byte order the placement of the

			 * bits is as follows:

        		 *      byte:  flag<0:3>  count<8:11> count<0:7>

        		 * (Note: this is flipped from the byte listing 

			 * 	  in descriptions above)

        		 */

			flag = twobyte >> 12;

			count = twobyte & 0x0fff;



			switch ( flag )

			{

			    case 1:

				lcnt = lcnt + count +1;

				while ( count-- >= 0 )

				{

				    ImVfbSRed(   vfb, pptr, ((*infoptr++)>>4) );

				    ImVfbSGreen( vfb, pptr, ((*infoptr++)>>4) );

				    ImVfbSBlue(  vfb, pptr, ((*infoptr++)>>4) );

				    ImVfbSAlpha( vfb, pptr, ((*infoptr++)>>4) );

				    ImVfbSInc(   vfb, pptr );

				    bytecount += 8;

				}

				break;



			    case 2:

				while ( count-- >= 0 )

				{

				    lambda = *infoptr++;

				    bytecount += 2;



				    lcnt = lcnt + lambda +1;

				    while ( lambda-- >= 0 )

				    {

				      ImVfbSRed(  vfb,pptr,((*(infoptr))>>4)  );

				      ImVfbSGreen(vfb,pptr,((*(infoptr+1))>>4));

				      ImVfbSBlue( vfb,pptr,((*(infoptr+2))>>4));

				      ImVfbSAlpha(vfb,pptr,((*(infoptr+3))>>4));

				      ImVfbSInc(  vfb,pptr );

				    }



				    infoptr += 4;

				    bytecount += 8;

				}

				break;



			    case 3:

				alpha = *infoptr++;

				bytecount += 2;



				lcnt = lcnt + count +1;

				while ( count-- >= 0 )

				{

				    ImVfbSRed(   vfb, pptr, ((*infoptr++)>>4) );

				    ImVfbSGreen( vfb, pptr, ((*infoptr++)>>4) );

				    ImVfbSBlue(  vfb, pptr, ((*infoptr++)>>4) );

				    ImVfbSAlpha( vfb, pptr, alpha );

				    ImVfbSInc(   vfb, pptr );

				    bytecount += 6;

				}

				break;



			    case 4:

				alpha = *infoptr++;

				bytecount += 2;



				while ( count-- >= 0 )

				{

				    lambda = *infoptr++;

				    bytecount += 2;



				    lcnt = lcnt + lambda +1;

				    while ( lambda-- >= 0 )

				    {

				      ImVfbSRed(  vfb,pptr,((*(infoptr))>>4)  );

				      ImVfbSGreen(vfb,pptr,((*(infoptr+1))>>4));

				      ImVfbSBlue( vfb,pptr,((*(infoptr+2))>>4));

				      ImVfbSAlpha( vfb, pptr, alpha );

				      ImVfbSInc(   vfb, pptr );

				    }



				    infoptr += 3;

				    bytecount += 6;

				}

				break;

	

			    case 0:		

				/* 

			 	 * This signifies end of block 

			 	 */

				bytecount = h->block;

				break;



	    		    default:		

				ImErrNo = IMESYNTAX;

				ImErrorFatal( ImQError(), -1, ImErrNo );

			}



			/* If the end of a block has been reached, 

			 * read next block of picture info.

			 */

			if (  bytecount >= h->block-2  )

		    	{

			    if (ImBinRead(ioType, fd, fp, info, UINT16,

					  2, h->block/2 ) == -1)

			    {

				free( (char *)info );

				ImReturnBinError();

			    }



			    infoptr = info;

			    bytecount = 0;

			}

		    }

		}



		break;



	    case IMRGBBACK:	



		/*

		 * Since Vfb can only hold 8-bit RGB values, the 12-bit values

		 * must be truncated.  Need to warn the user that this occurs

		 */

		ImErrorWarning( "12-bit data being reduced to 8-bit data", 

				-1, IMESYNTAX );



		for ( i=0; i<y; i++ )

		{

		    lcnt = 0;

		    while ( lcnt < x )

		    {

			/*

			 * Decode packets of the 3 channel info based on the 

			 * type of packet that is read from the picture file.

			 * Directly place the byte info in the vfb

			 */



			twobyte = *infoptr++;

			bytecount += 2;



        		/* 

			 * Since the packed 16 bits was read in as a single 

			 * sdsc_int16 value with LSB byte order the placement of the

			 * bits is as follows:

        		 *      byte:  flag<0:3>  count<8:11> count<0:7>

        		 * (Note: this is flipped from the byte listing 

			 * 	  in descriptions above)

        		 */

			flag = twobyte >> 12;

			count = twobyte & 0x0fff;



			switch ( flag )

			{

			    case 1:

				lcnt = lcnt + count +1;

				while ( count-- >= 0 )

				{

				    ImVfbSRed(   vfb, pptr, ((*infoptr++)>>4) );

				    ImVfbSGreen( vfb, pptr, ((*infoptr++)>>4) );

				    ImVfbSBlue(  vfb, pptr, ((*infoptr++)>>4) );

				    ImVfbSInc(   vfb, pptr );

				    bytecount += 6;

				}

				break;



			    case 2:

				while ( count-- >= 0 )

				{

				    lambda = *infoptr++;

				    bytecount += 2;



				    lcnt = lcnt + lambda +1;

				    while ( lambda-- >= 0 )

				    {

				      ImVfbSRed(  vfb,pptr,((*(infoptr))>>4)  );

				      ImVfbSGreen(vfb,pptr,((*(infoptr+1))>>4));

				      ImVfbSBlue( vfb,pptr,((*(infoptr+2))>>4));

				      ImVfbSInc(  vfb,pptr );

				    }



				    infoptr += 3;

				    bytecount += 6;

				}

				break;



			    case 0:		

				/* 

			 	 * This signifies end of block 

			 	 */

				bytecount = h->block;

				break;



	    		    default:		

				ImErrNo = IMESYNTAX;

				ImErrorFatal( ImQError(), -1, ImErrNo );

			}



			/* If the end of a block has been reached, 

			 * read next block of picture info.

			 */

			if (  bytecount >= h->block-2  )

		    	{

			    if (ImBinRead(ioType, fd, fp, info, UINT16,

					  2, h->block/2 ) == -1)

			    {

				free( (char *)info );

				ImReturnBinError();

			    }



			    infoptr = info;

			    bytecount = 0;

			}

		    }

		}



	    case IMSINGLECHAN:	



		for ( i=0; i<y; i++ )

		{

		    lcnt = 0;

		    while ( lcnt < x )

		    {

			/*

			 * Decode packets of the 1 channel info based on the 

			 * type of packet that is read from the picture file.

			 * Directly place the byte info in the vfb

			 */



			twobyte = *infoptr++;

			bytecount += 2;



        		/* 

			 * Since the packed 16 bits was read in as a single 

			 * sdsc_int16 value with LSB byte order the placement of the

			 * bits is as follows:

        		 *      byte:  flag<0:3>  count<8:11> count<0:7>

        		 * (Note: this is flipped from the byte listing 

			 * 	  in descriptions above)

        		 */

			flag = twobyte >> 12;

			count = twobyte & 0x0fff;



			switch ( flag )

			{

			    case 1:

				lcnt = lcnt + count +1;

				while ( count-- >= 0 )

				{

				    ImVfbSIndex16( vfb, pptr, (*infoptr++) );

				    ImVfbSInc(    vfb, pptr );

				    bytecount += 2;

				}

				break;



			    case 2:

				while ( count-- >= 0 )

				{

				    lambda = *infoptr++;

				    bytecount++;

					

				    lcnt = lcnt + lambda +1;

				    while ( lambda-- >= 0 )

				    {

					ImVfbSIndex16( vfb, pptr, (*infoptr) );

				        ImVfbSInc(    vfb, pptr );

				    }



				    infoptr++;

				    bytecount +=2;

				}

				break;



			    case 0:		

				/* 

			 	 * This signifies end of block 

			 	 */

				bytecount = h->block;

				break;



	    		    default:		

				ImErrNo = IMESYNTAX;

				ImErrorFatal( ImQError(), -1, ImErrNo );

			}



			/* 

			 * If the end of a block has been reached, 

			 * read next block of picture info.

			 */

			if (  bytecount >= h->block-2  )

		    	{



			    if (ImBinRead(ioType, fd, fp, info, UINT16,

					  2, h->block/2 ) == -1)

			    {

				free( (char *)info );

				ImReturnBinError();

			    }



			    infoptr = info;

			    bytecount = 0;



			}

		    }

		}

	}



	/*

	 * Free the pixel storage arrays picture data 

	 */

	free( (char *)info );



	/*

	 *  Add the VFB to the tagTable.

	 */

	TagTableAppend( tagTable, TagEntryAlloc( "image vfb", POINTER, &vfb ) );



	return ( 1 );

}











/*

 *  FUNCTION

 *	imPicReadDump12	-  read a PIXAR 12-bit dumped picture file 

 *

 *  DESCRIPTION

 *	Space is allocated for the VFB and the PIXAR dumped pixels are 

 *	directly transfered to the VFB based on the number of channels 

 *	after reducing info to an 8 bit representation, and then 

 *	the completed VFB is added to the tag list.

 *

 *	(Note: that the 12-bit  pixel values in info are first reduced to

 *		an 8-bit representation before being transfered to the

 *		the appropriate vfb color indicators. )

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__ 

imPicReadDump12( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable, imPicHeaderInfo *h, int fields, int channels )

#else

imPicReadDump12( ioType, fd, fp, flags, tagTable, h, fields, channels )

        int         ioType;             /* I/O flags                    */

        int         fd;                 /* Input file descriptor        */

        FILE       *fp;                 /* Input file pointer           */

        TagTable   *flags;              /* Format flags                 */

        TagTable   *tagTable;           /* Tag list to add to           */

	imPicHeaderInfo *h;		/* The Pixar picture header	*/

	int	    fields;		/* The flags for the vfb	*/

	int	    channels;		/* The Pixar picture format	*/

#endif

{

	ImVfb       *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	sdsc_uint16	    *info;		/* Buffer for PIXAR pic data	*/

	sdsc_uint16	    *infoHead;		/* The start of the info buffer */

	unsigned char	    *empty;		/* Buffer for unused area	*/

	int x,y;		/* Convenient short names	*/

	int i,j;		/* Loop counters		*/



	x = h->pic_width;

	y = h->pic_height;



	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (vfb = ImVfbAlloc( x, y, fields )) == IMVFBNULL )

	{

		ImErrNo = IMEMALLOC;

		ImErrorFatal( ImQError(), -1, ImErrNo );

	}



	/*

	 * Position the pointer at the beginning of the first scanline

	 */

	pptr = ImVfbQFirst( vfb );



	/*

	 * Allocate buffer large enough for one scanline of pixel info 

	 * (NOTE: 12 bit data stored in TWO bytes ) 

	 * and for the unused portion between the header and dumped pixels.

	 * Save start point of info buffer.

	 */ 

	ImMalloc( info, sdsc_uint16 *, (2*channels*x) );

	infoHead = info;



	/*

	 * First read in unused portion up to the first tile

	 */

	if ( (h->tile_ptr - IMPICHEADERSIZE) > 0 )

	{

		if ( ioType & IMFILEIOFILE )

		{

			ImSeek(ioType,fd,fp,h->tile_ptr-IMPICHEADERSIZE, 1 );

		}

		else

		{

			ImMalloc( empty, unsigned char *, h->block-IMPICHEADERSIZE);

			if (ImBinRead(ioType, fd, fp, empty, UCHAR, 

				1, h->tile_ptr - IMPICHEADERSIZE) == -1)

			{

				free( (char *)empty );

				free( (char *)info );

				ImReturnBinError();

			}

		}

	}



	/* 

	 * Determine what PIC Image format we have and 

	 * Loop through the scan lines.  Dump pixels from info for each line

	 * into the vfb based on # of channels.

	 *

	 * (NOTE: No default needed as error checking for format done before)

	 */

	switch ( h->pic_format )

	{

	    case IMFULLRGBA:



		/*

		 * Since Vfb can only hold 8-bit RGB values, the 12-bit values

		 * must be truncated.  Need to warn the user that this occurs

		 */

		ImErrorWarning( "12-bit data being reduced to 8-bit data", 

				-1, IMESYNTAX );



		for ( i=0; i<y; i++ )

		{

			/*

	 		 * read a scanline of pixel info to be dumped

	 		 */

			if (ImBinRead(ioType,fd,fp,info,UINT16,2,4*x) == -1)

			{

				free( (char *)info );

				ImReturnBinError();

			}



			for ( j=0; j < x; j++ )

			{

				ImVfbSRed(   vfb, pptr, ((*info++) >> 4) );

				ImVfbSGreen( vfb, pptr, ((*info++) >> 4) );

				ImVfbSBlue(  vfb, pptr, ((*info++) >> 4) );

				ImVfbSAlpha( vfb, pptr, ((*info++) >> 4) );

				ImVfbSInc(   vfb, pptr );

			}

	

			info = infoHead;

		}

		break;



	    case IMRGBBACK:	



		/*

		 * Since Vfb can only hold 8-bit RGB values, the 12-bit values

		 * must be truncated.  Need to warn the user that this occurs

		 */

		ImErrorWarning( "12-bit data being reduced to 8-bit data", 

				-1, IMESYNTAX );



		for ( i=0; i<y; i++ )

		{

			/*

	 		 * read a scanline of pixel info to be dumped

	 		 */

			if (ImBinRead(ioType,fd,fp,info,UINT16,2,3*x) == -1)

			{

				free( (char *)info );

				ImReturnBinError();

			}



			for ( j=0; j < x; j++ )

			{

				ImVfbSRed(   vfb, pptr, ((*info++) >> 4) );

				ImVfbSGreen( vfb, pptr, ((*info++) >> 4) );

				ImVfbSBlue(  vfb, pptr, ((*info++) >> 4) );

				ImVfbSInc(   vfb, pptr );

			}

	

			info = infoHead;

		}

		break;



	    case IMSINGLECHAN:	



		for ( i=0; i<y; i++ )

		{

			/*

	 		 * read a scanline of pixel info to be dumped

	 		 */

			if (ImBinRead(ioType,fd,fp,info,UINT16,2,x) == -1)

			{

				free( (char *)info );

				ImReturnBinError();

			}



			for ( j=0; j < x; j++ )

			{

				ImVfbSIndex16(  vfb, pptr, *info++ );

				ImVfbSInc(   vfb, pptr );

			}



			info = infoHead;

		}

	}



	/*

	 *  Add the VFB to the tagTable.

	 */

	TagTableAppend( tagTable, TagEntryAlloc( "image vfb", POINTER, &vfb ) );



	/*

	 * Free the pixel storage arrays

	 */

	free( (char *)info );



	return ( 1 );

}











/*

 *  FUNCTION

 *	imPicWriteHeader	-  write header for a Pic file

 *

 *  DESCRIPTION

 *	Initialize and write out the header for a PIC file.

 */



static int				/* Returns status		*/

#ifdef __STDC__

imPicWriteHeader( int ioType, int fd, FILE* fp, ImVfb* vfb, int format, int storage, int alpha, int length )

#else

imPicWriteHeader( ioType, fd, fp, vfb, format, storage, alpha, length )

        int          ioType;            /* I/O flags                    */

        int          fd;                /* Input file descriptor        */

        FILE        *fp;                /* Input file pointer           */

	ImVfb       *vfb;		/* Read in image		*/

	int	     format;		/* Image format (depth)		*/

	int	     storage;		/* Image storage style		*/

	int	     alpha;		/* Image alpha mode		*/

	int	     length;		/* Image length 		*/

#endif

{

	imPicHeaderInfo  header;	/* PIC file header		*/

	int	     numWritten;	/* Number of bytes written	*/

	char		 message[100];	/* ImInfo message		*/





	/*

	 *  Check to see if the user wants the PIC image to be dump format

	 *  ( default: PIC image will be encoded 8-bit )

	 *  And set format in image header.

	 */

	memset( (void *)&header, 0x00, sizeof( header ) );

	header.pic_storage    = storage;

	header.magic_number   = IMPICNUMBER;

	header.version_number = 2;

	header.labelptr       = 0;

	header.pic_height     = ImVfbQHeight( vfb );

	header.pic_width      = ImVfbQWidth( vfb );

	header.tile_height    = header.pic_height;

	header.tile_width     = header.pic_width;

	header.block          = IMPICBLOCK;

	header.alpha          = alpha;

	header.x_offset       = 0;

	header.y_offset       = 0;

	header.intspace       = 0;

	header.tile_ptr       = IMPICWRITESTART;

	header.tile_len       = length;

	header.pic_format     = format;

	if (( numWritten = ImBinWriteStruct( ioType, fd, fp, &header, 

		imPicHeaderFields )) == -1 )

	{

		ImReturnBinError();

	}



	if ( numWritten != IMPICHEADERSIZE )

	{

		ImErrNo = IMESYS;

		ImErrorFatal ( ImQError(), -1, ImErrNo );

	}



	/*

	 * To allow for variable space between the header and the data

	 * this section has been included.

	 */



	if ( numWritten < IMPICWRITESTART )

	{

		int numToWrite;

		unsigned char	    *buf;	/* Pointer for temp buffer 	*/ 



		numToWrite = IMPICWRITESTART - numWritten;

		ImMalloc( buf, unsigned char*, numToWrite );

		memset( (void *)buf, 0x00, numToWrite );



		if(ImBinWrite(ioType,fd,fp,buf,UCHAR,1,numToWrite)== -1 )

		{

			free( buf );

			ImReturnBinError();

		}



		free( buf );

	}



	/*

	 * These lines are used if a flag is set to printout file info.

	 */

	sprintf (message,"%d",header.version_number);

	ImInfo ("Version",message);

	ImInfo ("Byte Order","Least Significant Byte First");

	sprintf (message, "%d x %d",header.pic_width, header.pic_height);

	ImInfo ("Resolution",message); 

	if ( (header.pic_storage == IMPICENCODE12) ||

	     (header.pic_storage == IMPICDUMP12)	)

	{

	  switch ( header.pic_format )

	  {

	    case IMFULLRGBA:

		sprintf (message, "48-bit RGB and Alpha");



	    case IMRGBBACK:

		sprintf (message, "36-bit RGB");



	    case IMSINGLECHAN:

		sprintf (message, "12-bit Grayscale");

	  }

	}

	else

	{

	  switch ( header.pic_format )

	  {

	    case IMFULLRGBA:

		sprintf (message, "32-bit RGB and Alpha");



	    case IMRGBBACK:

		sprintf (message, "24-bit RGB");



	    case IMSINGLECHAN:

		sprintf (message, "8-bit Grayscale");

	  }

	}

	ImInfo( "Type", message );

	if ( (header.pic_storage == IMPICENCODE12) ||

	     (header.pic_storage == IMPICENCODE8) )

	ImInfo ("Compression Type","Run Length Encoded (RLE)")

	else

	ImInfo ("Compression Type","none (dump)");

	if (header.pic_format==IMFULLRGBA) ImInfo ("Alpha Channel","8-bit")

	else ImInfo ("Alpha Channel","none");



	

	return ( 0 );

}











/*

 *  FUNCTION

 *	imPicWriteRGBA		-  write a four-channel PIC file, dump

 *	imPicWriteRGBARLE	-  write a four-channel PIC file, encoded

 *

 *  DESCRIPTION

 *	Vfb pixel info is read into buffers and is either run length encoded

 *	by counting the number of repitiions on each scanline, or the info

 *	is dumped directly into the PIXAR Picture file.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imPicWriteRGBA(ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imPicWriteRGBA( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

        int          ioType;            /* I/O flags                    */

        int          fd;                /* Input file descriptor        */

        FILE        *fp;                /* Input file pointer           */

        TagTable    *flagsTable;        /* Format flags                 */

        TagTable    *tagTable;          /* Tag list to read from        */

#endif

{

	ImVfb	    *vfb;		/* Image to write		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	unsigned char       *buf;	/* Run buffer			*/

	unsigned char	    *bufStart;	/* Pointer to first loc in buf	*/ 

	int i,j;			/* Loop counters		*/

	int	     x,y;		/* Loop bounds			*/



	/*

	 *  PIC files are usually generated on PIXARs, which stores 

	 *  MULTIPLE-byte data with the least significant 8 in the first 

	 *  byte. This is an LBF byte order. 

	 *  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINLBF );



	/*

	 *  Initialize the things we need

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	x    = ImVfbQWidth( vfb );

	y    = ImVfbQHeight( vfb );

	pptr = ImVfbQFirst( vfb );



	/*

	 *  Write the PIXAR picture file header.

	 *  NOTE: we can pre-compute the length of the pixel data since

	 *	we know the each pixel written will have 4 bytes associated

	 *	with it one for R,G,B,and A.  Therefore, Length = 4*x*y

	 */

	if ( imPicWriteHeader( ioType, fd, fp, vfb, IMFULLRGBA, IMPICDUMP8, IMISALPHA, (4*x*y) ) == -1 )

		return ( -1 );		/* Error already handled	*/



	/* 

	 * Allocate a buffer big enough for one scanline, read pixel info

	 * from vfb, and write it directly to pixar picture file.

	 */

	ImMalloc( bufStart, unsigned char*, 4*x );



	for( i=0; i<y; i++ )

	{

		buf = bufStart;

		for ( j = 0; j < x; j++ )

		{

			*buf++ = ImVfbQRed(   vfb, pptr );

			*buf++ = ImVfbQGreen( vfb, pptr );

			*buf++ = ImVfbQBlue(  vfb, pptr );

			*buf++ = ImVfbQAlpha( vfb, pptr );

			ImVfbSInc(   vfb, pptr );

		}



		if( ImBinWrite(ioType,fd,fp,bufStart,UCHAR,1,4*x) == -1 )

		{

			free( bufStart );

			ImReturnBinError();

		}

	}

	free( bufStart );





	return ( 1 );

}





static int				/* Returns # of tags used	*/

#ifdef __STDC__

imPicWriteRGBARLE(ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imPicWriteRGBARLE( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

        int          ioType;            /* I/O flags                    */

        int          fd;                /* Input file descriptor        */

        FILE        *fp;                /* Input file pointer           */

        TagTable    *flagsTable;        /* Format flags                 */

        TagTable    *tagTable;          /* Tag list to read from        */

#endif

{

	ImVfb	    *vfb;		/* Image to write		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	unsigned char        redBuf[2];		/* R value for two pixels	*/

	unsigned char        grnBuf[2];		/* G value for two pixels	*/

	unsigned char        bluBuf[2];		/* B value for two pixels	*/

	unsigned char        alpBuf[2];		/* A value for two pixels	*/

	unsigned char       *buf;		/* Run buffer			*/

	unsigned char	    *bufStart;		/* Pointer to first loc in buf	*/ 

	int i,j,k;		/* Loop counters		*/

	int	     x,y;		/* Loop bounds			*/

	unsigned char	     reps;		/* Number of repititions	*/

	unsigned int	     count, flag;	/* Pixar encoding parameters	*/ 

	int	     numToWrite;	/* Num of bytes to write in buf */	

	int	     numWritten;	/* bytes written in curr. block	*/

	int	     totWritten;	/* Total number of bytes written*/

	int	     fillBuf;		/* Flag to signify to fill buf	*/



	/*

	 *  PIC files are usually generated on PIXARs, which stores 

	 *  MULTIPLE-byte data with the least significant 8 in the first 

	 *  byte. This is an LBF byte order. 

	 *  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINLBF );



	/*

	 *  Although the PIXAR picture file header resides first in the file

	 *  the header contains a size field which indicates the length of the

	 *  image, which can only be determined AFTER the pixel values have

	 *  be written,

	 *

	 *  Therefore, skip over the header position first, write the data

	 *  next, then go back to the start of the file and write the header.

	 */



	/*

	 *  Seek to data start position

	 */	

	ImSeek( ioType, fd, fp, IMPICWRITESTART, 1 );

	

	/*

	 *  Initialize the things we need

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	x    = ImVfbQWidth( vfb );

	y    = ImVfbQHeight( vfb );

	pptr = ImVfbQFirst( vfb );



	/*

	 * Allocate run buffer large enough for one scanline

	 * with maximum storage for full channels ( one packet ) 

	 * and including utilization of space for exceeding block size.

	 * Retain the start of the buffer space.

	 */ 





	ImMalloc( bufStart, unsigned char *, (x*5+4) );

	buf = bufStart +2;



	/*

	 * Preset bytes written count for case of exceeding block size.

	 * Runlength encoding flag = 2 for Pixar format.

	 */

	numWritten = 0; 

	totWritten = 0;

	fillBuf = 0;

	flag = 2; 



	/*

	 * Loop through the scan lines. Fill the arrays with channel values,

	 * encode them, and then write them out to a temporary buffer

	 * in order to keep track of number of runlengths.

	 * Then write out to file at end of counting process filling out 

	 * block with zeros when maximum block size has been reached.

	 * (note:we do not have to worry about the "count" overflowing since

	 *  the blocksize has been fixed to IMPICBLOCK (=8192) and for

	 *  "count" to overflow there would be 5*4095 bytes "to be written".

	 *  The block size would be exceeded first )

	 */

	for( i=0; i<y; i++ )

	{

		reps = 0; count = (unsigned int) -1; 



		/*

		 * For each scanline this count starts at two (bytes) for

		 * minimum write of the 16 bit packed, flag and count 

		 */

		numToWrite = 2;



		redBuf[0] = ImVfbQRed(   vfb, pptr );

		grnBuf[0] = ImVfbQGreen( vfb, pptr );

		bluBuf[0] = ImVfbQBlue(  vfb, pptr );

		alpBuf[0] = ImVfbQAlpha( vfb, pptr );

		ImVfbSInc( vfb, pptr );



		/*

		 * Copy the vfb pixel values into arrays

		 * and check for repetitions throughout each scanline.

		 */

		for ( j = 1; j < x; j++ )

		{

			redBuf[1] = ImVfbQRed(   vfb, pptr );

			grnBuf[1] = ImVfbQGreen( vfb, pptr );

			bluBuf[1] = ImVfbQBlue(  vfb, pptr );

			alpBuf[1] = ImVfbQAlpha( vfb, pptr );

			ImVfbSInc( vfb, pptr );



			if ( (redBuf[1] == redBuf[0]) &&

				(grnBuf[1] == grnBuf[0]) &&

				(bluBuf[1] == bluBuf[0]) &&

				(alpBuf[1] == alpBuf[0])    )

			{

				if ( reps == 255 )

					fillBuf = 1;

				else

					reps ++;

			}

			else

			{

				fillBuf = 1; 

			}





			if ( fillBuf ) 

			{

				/*

				 * There must always be at least 2 bytes

				 * that can be filled with zero's to signify

				 * the end of the block.(packed as 16 bits)

				 */

				if ( (numWritten+numToWrite+5)>=(IMPICBLOCK-4) )

				{

					/*

					 * The blocksize will be exceeded if we

					 * fill the buffer with next packet, and

					 * since packets cannot span successive

					 * blocks, the current buffer needs to

					 * be written and the remainder of

					 * the block filled with 0's.

					 */



					/*

					 * A rare case occurs if the end of a 

					 * scanline was previously written and 

					 * the next packet to fill would cause

					 * block to overflow; thus, signify 

					 * end of block

					 */

					if ( numToWrite == 2 )

					{

						count = 0;

						flag = 0;

					}



					/* 

					 * store count and flag bits and 

					 * update count for subsequent check

					 */

					*bufStart = (unsigned char) (count & 0xff);

					*(bufStart+1) = (unsigned char) ((flag << 4) + (count >> 8) );

					numWritten += numToWrite;



					/*

					 * Fill out remainder of block

					 * with zero's which signifies end.

					 */

					if ( numWritten < IMPICBLOCK )

					{

						k = IMPICBLOCK - numWritten;

						memset( (void *)buf, 0x00, k );

						numToWrite += k;

						buf += k;

					}



					/*

					 * Write stored buffer runlength info

					 */	

					if(ImBinWrite(ioType,fd,fp,bufStart,

						UCHAR,1,numToWrite) == -1 )

					{

						free(bufStart);	

						ImReturnBinError();

					}



					totWritten += numToWrite;



					/*

					 * Reset variables for next block 

					 */

					buf = bufStart+2;

					count = (unsigned int)-1; 

					numToWrite = 2;

					numWritten = 0;

					if ( flag == 0 ) flag = 2;



				}



				/*

				 * Fill buffer with next packet info

				 */	

				count ++;

				*buf++ = reps;

				*buf++ = redBuf[0];

				*buf++ = grnBuf[0];

				*buf++ = bluBuf[0];

				*buf++ = alpBuf[0];

				numToWrite += 5;



				reps = 0;



				/*

				 * Reset flag and set current pixel as the

				 * pixel to be compared to for future runs.

				 */

				fillBuf = 0;

				redBuf[0] = redBuf[1];

				grnBuf[0] = grnBuf[1];

				bluBuf[0] = bluBuf[1];

				alpBuf[0] = alpBuf[1];

			}

		}



		/* 

		 * Completed scanline.  Write out packed 16 bits (flag & count)

		 * and info of packets contained in buffer. 

		 */



		/*

		 * Fill buffer with next packet info

		 */	

		count ++;

		*buf++ = reps;

		*buf++ = redBuf[0];

		*buf++ = grnBuf[0];

		*buf++ = bluBuf[0];

		*buf++ = alpBuf[0];





		numToWrite += 5;



		*bufStart = (unsigned char) (count & 0xff);

		*(bufStart+1) = (unsigned char) ((flag << 4) + (count >> 8));





		if(ImBinWrite(ioType,fd,fp,bufStart,UCHAR,1,numToWrite)== -1 )

		{

			free( bufStart );

			ImReturnBinError();

		}



		buf = bufStart+2;

		numWritten += numToWrite;

		totWritten += numToWrite;

	}





	/*

	 * The rest of the last block needs to be completed so that any block 

	 * reads that do not check for the end of the file can read blindly

	 * NOTE: Some tests were performed on the technique to just close the 

	 * last block with 4 zero's ( which signify the end of the block ),

	 * but just in case some applications may get confused if the

	 * block is not totally complete, the remainder of the block will

	 * be filled with zeros.

	 */

	if ( numWritten < IMPICBLOCK )

	{

		unsigned char	    *buf;



		numToWrite = IMPICBLOCK - numWritten;

		ImMalloc( buf, unsigned char*, numToWrite );

		memset( (void *)buf, 0x00, numToWrite );



		if(ImBinWrite(ioType,fd,fp,buf,UCHAR,1,numToWrite)== -1 )

		{

			free( buf );

			ImReturnBinError();

		}



		totWritten += numToWrite;

		free( buf );

	}



	/*

	 *  Now that all the image data has been written, the header must

	 *  be written; thus, seek back to the start of the file and write it

	 */

	ImSeek( ioType, fd, fp, 0, 0 );



	/*

	 *  Write the PIXAR picture file header.

	 */

	if ( imPicWriteHeader( ioType, fd, fp, vfb, IMFULLRGBA, IMPICENCODE8, IMISALPHA, totWritten ) == -1 )

	{

		return ( -1 );		/* Error already handled	*/

	}



	free( bufStart );







	return ( 1 );

}











/*

 *  FUNCTION

 *	imPicWriteRGB		-  write a three-channel PIC file, dump

 *	imPicWriteRGBRLE	-  write a three-channel PIC file, encoded

 *

 *  DESCRIPTION

 *	Vfb pixel info is read into buffers and is either run length encoded

 *	by counting the number of repitiions on each scanline, or the info

 *	is dumped directly into the PIXAR Picture file.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imPicWriteRGB(ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imPicWriteRGB( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

        int          ioType;            /* I/O flags                    */

        int          fd;                /* Input file descriptor        */

        FILE        *fp;                /* Input file pointer           */

        TagTable    *flagsTable;        /* Format flags                 */

        TagTable    *tagTable;          /* Tag list to read from        */

#endif

{

	ImVfb	    *vfb;		/* Image to write		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	unsigned char       *buf;	/* Run buffer			*/

	unsigned char	    *bufStart;	/* Pointer to first loc in buf	*/ 

	int i,j;			/* Loop counters		*/

	int	     x,y;		/* Loop bounds			*/



	/*

	 *  PIC files are usually generated on PIXARs, which stores 

	 *  MULTIPLE-byte data with the least significant 8 in the first 

	 *  byte. This is an LBF byte order. 

	 *  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINLBF );



	/*

	 *  Initialize the things we need

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	x    = ImVfbQWidth( vfb );

	y    = ImVfbQHeight( vfb );

	pptr = ImVfbQFirst( vfb );



	/*

	 *  Write the PIXAR picture file header.

	 *  NOTE: we can pre-compute the length of the pixel data since

	 *	we know the each pixel written will have 3 bytes associated

	 *	with it one for R,G,and B.  Therefore, Length = 3*x*y

	 */

	if ( imPicWriteHeader( ioType, fd, fp, vfb, IMRGBBACK, IMPICDUMP8, IMNOALPHA, (3*x*y) ) == -1 )

		return ( -1 );		/* Error already handled	*/



	/* Allocate a buffer big enough for one scanline, read pixel info

	 * from vfb, and write it directly to pixar picture file.

	 */

	ImMalloc( bufStart, unsigned char*, 3*x );



	for( i=0; i<y; i++ )

	{



		buf = bufStart;

		for ( j = 0; j < x; j++ )

		{

			*buf++ = ImVfbQRed(   vfb, pptr );

			*buf++ = ImVfbQGreen( vfb, pptr );

			*buf++ = ImVfbQBlue(  vfb, pptr );

			ImVfbSInc(   vfb, pptr );

		}

		if(ImBinWrite(ioType,fd,fp,bufStart,UCHAR,1,3*x) == -1 )

		{

			free( bufStart );

			ImReturnBinError();

		}

	}

	free( bufStart );





	return ( 1 );

}



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imPicWriteRGBRLE(ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imPicWriteRGBRLE( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

        int          ioType;            /* I/O flags                    */

        int          fd;                /* Input file descriptor        */

        FILE        *fp;                /* Input file pointer           */

        TagTable    *flagsTable;        /* Format flags                 */

        TagTable    *tagTable;          /* Tag list to read from        */

#endif

{

	ImVfb	    *vfb;		/* Image to write		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	unsigned char        redBuf[2];		/* R value for two pixels	*/

	unsigned char        grnBuf[2];		/* G value for two pixels	*/

	unsigned char        bluBuf[2];		/* B value for two pixels	*/

	unsigned char       *buf;		/* Run buffer			*/

	unsigned char	    *bufStart;		/* Pointer to first loc in buf	*/ 

	int i,j,k;				/* Loop counters		*/

	int	     x,y;			/* Loop bounds			*/

	unsigned char	     reps;		/* Number of repititions	*/

	unsigned int	     count, flag;	/* Pixar encoding parameters	*/ 

	int	     numToWrite;	/* Num of bytes to write in buf */	

	int	     numWritten;	/* Number of bytes written	*/

	int	     totWritten;	/* Total number of bytes written*/

	int	     fillBuf;		/* Flag to signify to fill buf	*/



	/*

	 *  PIC files are usually generated on PIXARs, which stores 

	 *  MULTIPLE-byte data with the least significant 8 in the first 

	 *  byte. This is an LBF byte order. 

	 *  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINLBF );



        /*

         *  Although the PIXAR picture file header resides first in the file

         *  the header contains a size field which indicates the length of the

         *  image, which can only be determined AFTER the pixel values have

         *  be written,

         *

         *  Therefore, skip over the header position first, write the data

         *  next, then go back to the start of the file and write the header.

         */



        /*

         *  Seek to data start position

         */

        ImSeek( ioType, fd, fp, IMPICWRITESTART, 1 );



	/*

	 *  Initialize the things we need

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	x    = ImVfbQWidth( vfb );

	y    = ImVfbQHeight( vfb );

	pptr = ImVfbQFirst( vfb );



	/*

	 * Allocate run buffer large enough for one scanline

	 * with maximum storage for full channels ( one packet ) 

	 * and including utilization of space for exceeding block size.

	 * Retain the start of the buffer space.

	 */ 





	ImMalloc( bufStart, unsigned char *, (x*4+4) );

	buf = bufStart +2;



	/*

	 * Preset bytes written count for case of exceeding block size.

	 * Runlength encoding flag = 2 for Pixar format.

	 */

	numWritten = 0; 

	totWritten = 0;

	fillBuf = 0;

	flag = 2; 



	/*

	 * Loop through the scan lines. Fill the arrays with channel values,

	 * encode them, and then write them out to a temporary buffer

	 * in order to keep track of number of runlengths.

	 * Then write out to file at end of counting process filling out 

	 * block with zeros when maximum block size has been reached.

	 * (note:we do not have to worry about the "count" overflowing since

	 *  the blocksize has been fixed to IMPICBLOCK (=8192) and for

	 *  "count" to overflow there would be 4*4095 bytes "to be written".

	 *  The block size would be exceeded first )

	 */

	for( i=0; i<y; i++ )

	{

		reps = 0; count = (unsigned int)-1; 



		/*

		 * For each scanline this count starts at two (bytes) for

		 * minimum write of the 16 bit packed flags and count 

		 */

		numToWrite = 2;



		redBuf[0] = ImVfbQRed(   vfb, pptr );

		grnBuf[0] = ImVfbQGreen( vfb, pptr );

		bluBuf[0] = ImVfbQBlue(  vfb, pptr );

		ImVfbSInc(   vfb, pptr );



		/*

		 * Copy the vfb pixel values into arrays

		 * and check for repetitions throughout each scanline.

		 */

		for ( j = 1; j < x; j++ )

		{

			redBuf[1] = ImVfbQRed(   vfb, pptr );

			grnBuf[1] = ImVfbQGreen( vfb, pptr );

			bluBuf[1] = ImVfbQBlue(  vfb, pptr );

			ImVfbSInc(   vfb, pptr );



			if ( (redBuf[1] == redBuf[0]) &&

				(grnBuf[1] == grnBuf[0]) &&

				(bluBuf[1] == bluBuf[0])	) 

			{

				if ( reps == 255 )

					fillBuf = 1;

				else

					reps ++;

			}

			else

			{

				fillBuf = 1; 

			}





			if ( fillBuf ) 

			{

				/*

				 * There must always be at least 2 bytes

				 * that can be filled with zero's to signify

				 * the end of the block.(packed as 16 bits)

				 */

				if ( (numWritten+numToWrite+4)>=(IMPICBLOCK-4) )

				{

					/*

					 * The blocksize will be exceeded if we

					 * fill the buffer with next packet, and

					 * since packets cannot span successive

					 * blocks, the current buffer needs to

					 * be written and the remainder of

					 * the block filled with 0's.

					 */



					/*

					 * A rare case occurs if the end of a 

					 * scanline was previously written and 

					 * the next packet to fill would cause

					 * block to overflow -- 

					 * 	signify end of block

					 */

					if ( numToWrite == 2 )

					{

						count = 0;

						flag = 0;

					}



                                        /*

                                         * store count and flag bits and

                                         * update count for subsequent check

                                         */

					*bufStart = (unsigned char) (count & 0xff);

					*(bufStart+1) = (unsigned char) ((flag << 4) + 

						(count >> 8) );





					numWritten += numToWrite;



					/*

					 * Fill out remainder of block

					 * with zero's to signify end.

					 */

					if ( numWritten < IMPICBLOCK )

					{

						k = IMPICBLOCK - numWritten;

						memset( (void *)buf, 0x00, k );

						numToWrite += k;

						buf += k;

					}



					/*

					 * Write stored buffer runlength info

					 */	

					if(ImBinWrite(ioType,fd,fp,bufStart,

						   UCHAR,1,numToWrite) == -1 )

					{

						free(bufStart);	

						ImReturnBinError();

					}



					totWritten += numToWrite;



					/*

					 * Reset variables for next block

					 */

					buf = bufStart+2;

					count = (unsigned int)-1; 

					numToWrite = 2;

					numWritten = 0;

					if ( flag == 0 ) flag = 2;



				}



				/*

				 * Fill buffer with next packet info

				 */	

				count ++;

				*buf++ = reps;

				*buf++ = redBuf[0];

				*buf++ = grnBuf[0];

				*buf++ = bluBuf[0];

				numToWrite += 4;





				reps = 0;



				/*

				 * Reset flag and set current pixel as the

				 * pixel to be compared to for future runs.

				 */

				fillBuf = 0;

				redBuf[0] = redBuf[1];

				grnBuf[0] = grnBuf[1];

				bluBuf[0] = bluBuf[1];

			}

		}



		/* 

		 * Completed scanline.  Write out packed 16 bits (flag & count)

		 * and info of packets contained in buffer. 

		 */



		/*

		 * Fill buffer with next packet info

		 */	

		count ++;

		*buf++ = reps;

		*buf++ = redBuf[0];

		*buf++ = grnBuf[0];

		*buf++ = bluBuf[0];





		numToWrite += 4;



		*bufStart = (unsigned char) (count & 0xff);

		*(bufStart+1) = (unsigned char) ((flag << 4) + (count >> 8));







		if(ImBinWrite(ioType,fd,fp,bufStart,UCHAR,1,numToWrite)== -1 )

		{

			free( bufStart );

			ImReturnBinError();

		}



		buf = bufStart+2;

		numWritten += numToWrite;

		totWritten += numToWrite;

	}





        /*

         * The rest of the last block needs to be completed so that any block

         * reads that do not check for the end of the file can read blindly

	 * NOTE: Some tests were performed on the technique to just close the 

	 * last block with 4 zero's ( which signify the end of the block ),

	 * but just in case some applications may get confused if the

	 * block is not totally complete, the remainder of the block will

	 * be filled with zeros.

         */

        if ( numWritten < IMPICBLOCK )

        {

                unsigned char       *buf;



                numToWrite = IMPICBLOCK - numWritten;

                ImMalloc( buf, unsigned char*, numToWrite );

                memset( (void *)buf, 0x00, numToWrite );



                if(ImBinWrite(ioType,fd,fp,buf,UCHAR,1,numToWrite)== -1 )

                {

                        free( buf );

                        ImReturnBinError();

                }



                totWritten += numToWrite;

                free( buf );

        }



	/* 

	 *  Now that all the image data has been written, the header must

         *  be written; thus, seek back to the start of the file and write it

         */

        ImSeek( ioType, fd, fp, 0, 0 );



	/*

	 *  Write the PIXAR picture file header.

	 */

	if ( imPicWriteHeader( ioType, fd, fp, vfb, IMRGBBACK, IMPICENCODE8, IMNOALPHA, totWritten ) == -1 )

		return ( -1 );		/* Error already handled	*/



	free( bufStart );





	return ( 1 );

}











/*

 *  FUNCTION

 *	imPicWrite8	-  write a one-channel PIC file, dump

 *	imPicWrite8RLE	-  write a one-channel PIC file, encoded

 *

 *  DESCRIPTION

 *	Vfb pixel info is read into buffers and is either run length encoded

 *	by counting the number of repitiions on each scanline, or the info

 *	is dumped directly into the PIXAR Picture file.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imPicWrite8(ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imPicWrite8( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

        int          ioType;            /* I/O flags                    */

        int          fd;                /* Input file descriptor        */

        FILE        *fp;                /* Input file pointer           */

        TagTable    *flagsTable;        /* Format flags                 */

        TagTable    *tagTable;          /* Tag list to read from        */

#endif

{

	ImVfb	    *vfb;		/* Image to write		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	unsigned char       *buf;	/* Run buffer			*/

	unsigned char	    *bufStart;	/* Pointer to first loc in buf	*/ 

	int i,j;			/* Loop counters		*/

	int	     x,y;		/* Loop bounds			*/



	/*

	 *  PIC files are usually generated on PIXARs, which stores 

	 *  MULTIPLE-byte data with the least significant 8 in the first 

	 *  byte. This is an LBF byte order. 

	 *  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINLBF );



	/*

	 *  Initialize the things we need

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	x    = ImVfbQWidth( vfb );

	y    = ImVfbQHeight( vfb );

	pptr = ImVfbQFirst( vfb );



	/*

	 *  Write the PIXAR picture file header.

	 *  NOTE: we can pre-compute the length of the pixel data since

	 *	we know the each pixel written will have 1 byte associated

	 *	with it one for R.  Therefore, Length = x*y

	 */

	if ( imPicWriteHeader( ioType, fd, fp, vfb, IMSINGLECHAN, IMPICDUMP8, IMNOALPHA, (x*y) ) == -1 )

		return ( -1 );		/* Error already handled	*/



	/* Allocate a buffer big enough for one scanline, read pixel info

	 * from vfb, and write it directly to pixar picture file.

	 */

	ImMalloc( bufStart, unsigned char*, x );



	for( i=0; i<y; i++ )

	{

		buf = bufStart;

		for ( j = 0; j < x; j++ )

		{

			*buf++ = ImVfbQIndex8( vfb, pptr );

			ImVfbSInc( vfb, pptr );

		}

		if(ImBinWrite(ioType,fd,fp,bufStart,UCHAR,1,x) == -1 )

		{

			free( bufStart );

			ImReturnBinError();

		}

	}



	free( bufStart );



	return ( 1 );

}

 

static int				/* Returns # of tags used	*/

#ifdef __STDC__

imPicWrite8RLE(ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imPicWrite8RLE( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

        int          ioType;            /* I/O flags                    */

        int          fd;                /* Input file descriptor        */

        FILE        *fp;                /* Input file pointer           */

        TagTable    *flagsTable;        /* Format flags                 */

        TagTable    *tagTable;          /* Tag list to read from        */

#endif

{

	ImVfb	    *vfb;		/* Image to write		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	unsigned char        redBuf[2];		/* R value for two pixels	*/

	unsigned char       *buf;		/* Run buffer			*/

	unsigned char	    *bufStart;		/* Pointer to first loc in buf	*/ 

	int i,j,k;		/* Loop counters		*/

	int	     x,y;		/* Loop bounds			*/

	unsigned char	     reps;		/* Number of repititions	*/

	unsigned int	     count, flag;	/* Pixar encoding parameters	*/ 

	int	     numToWrite;	/* Num of bytes to write in buf */	

	int	     numWritten;	/* Number of bytes written	*/

	int	     totWritten;	/* Total number of bytes written*/

	int	     fillBuf;		/* Flag to signify to fill buf	*/



	/*

	 *  PIC files are usually generated on PIXARs, which stores 

	 *  MULTIPLE-byte data with the least significant 8 in the first 

	 *  byte. This is an LBF byte order. 

	 *  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINLBF );



        /*

         *  Although the PIXAR picture file header resides first in the file

         *  the header contains a size field which indicates the length of the

         *  image, which can only be determined AFTER the pixel values have

         *  be written,

         *

         *  Therefore, skip over the header position first, write the data

         *  next, then go back to the start of the file and write the header.

         */



        /*

         *  Seek to data start position

         */

        ImSeek( ioType, fd, fp, IMPICWRITESTART, 1 );



	/*

	 *  Initialize the things we need

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	x    = ImVfbQWidth( vfb );

	y    = ImVfbQHeight( vfb );

	pptr = ImVfbQFirst( vfb );



	/*

	 * Allocate run buffer large enough for one scanline

	 * with maximum storage for full channels ( one packet ) 

	 * and including utilization of space for exceeding block size.

	 * Retain the start of the buffer space.

	 */ 

	ImMalloc( bufStart, unsigned char *, (x*2+4) );

	buf = bufStart +2;



	/*

	 * Preset bytes written count for case of exceeding block size.

	 * Runlength encoding flag = 2 for Pixar format.

	 */

	numWritten = 0; 

	totWritten = 0;

	flag = 2; 

	fillBuf = 0;



	/*

	 * Loop through the scan lines. Fill the arrays with channel values,

	 * encode them, and then write them out to a temporary buffer

	 * in order to keep track of number of runlengths.

	 * Then write out to file at end of counting process filling out 

	 * block with zeros when maximum block size has been reached.

	 * (note:we do not have to worry about the "count" overflowing since

	 *  the blocksize has been fixed to IMPICBLOCK (=8192) and for

	 *  "count" to overflow there would be 2*4095 bytes "to be written".

	 *  The block size would be exceeded first )

	 */

	for( i=0; i<y; i++ )

	{

		reps = 0; count = (unsigned int)-1; 



		/*

		 * For each scanline this count starts at two (bytes) for

		 * minimum write of the 16 bit packed flags and count 

		 */

		numToWrite = 2;



		redBuf[0] = ImVfbQIndex8( vfb, pptr );

		ImVfbSInc( vfb, pptr );



		/*

		 * Copy the vfb pixel values into arrays

		 * and check for repetitions throughout each scanline.

		 */

		for ( j = 1; j < x; j++ )

		{

			redBuf[1] = ImVfbQIndex8( vfb, pptr );

			ImVfbSInc( vfb, pptr );



			if ( redBuf[1] == redBuf[0] ) 

			{

				if ( reps == 255 )

					fillBuf = 1;

				else

					reps ++;

			}

			else

			{

				fillBuf = 1; 

			}





			if ( fillBuf ) 

			{

				/*

				 * There must always be at least 2 bytes

				 * that can be filled with zero's to signify

				 * the end of the block.(packed as 16 bits)

				 */

				if ( (numWritten+numToWrite+2)>=(IMPICBLOCK-4) )

				{

					/*

					 * The blocksize will be exceeded if we

					 * fill the buffer with next packet, and

					 * since packets cannot span successive

					 * blocks, the current buffer needs to

					 * be written and the remainder of

					 * the block filled with 0's.

					 */



					/*

					 * A rare case occurs if the end of a 

					 * scanline was previously written and 

					 * the next packet to fill would cause

					 * block to overflow -- 

					 * 	signify end of block

					 */

					if ( numToWrite == 2 )

					{

						count = 0;

						flag = 0;

					}



					/*

					 * store count and flag bits and

					 * update count for subsequent check

					 */

					*bufStart = (unsigned char) (count & 0xff);

					*(bufStart+1) = (unsigned char) ((flag << 4) + 

						(count >> 8) );



					numWritten += numToWrite;



					/*

					 * Fill out remainder of block

					 * with zero's to signify end.

					 */

					if ( numWritten < IMPICBLOCK )

					{

						k = IMPICBLOCK - numWritten;

						memset( (void *)buf, 0x00, k );

						numToWrite += k;

						buf += k;

					}



					/*

					 * Write stored buffer runlength info

					 */	

					if(ImBinWrite(ioType,fd,fp,bufStart,

						UCHAR,1,numToWrite) == -1 )

					{

						free(bufStart);	

						ImReturnBinError();

					}



					totWritten += numToWrite;



					/*

					 * Reset variables for next block

					 */

					buf = bufStart+2;

					count = (unsigned int)-1; 

					numToWrite = 2;

					numWritten = 0;

					if ( flag == 0 ) flag = 2;



				}



				/*

				 * Fill buffer with next packet info

				 */	

				count ++;

				*buf++ = reps;

				*buf++ = redBuf[0];

				numToWrite += 2;

				reps = 0;



				/*

				 * Reset flag and set current pixel as the

				 * pixel to be compared to for future runs.

				 */

				fillBuf = 0;

				redBuf[0] = redBuf[1];

			}

		}



		/* 

		 * Completed scanline.  Write out packed 16 bits (flag & count)

		 * and info of packets contained in buffer. 

		 */



		/*

		 * Fill buffer with next packet info

		 */	

		count ++;

		*buf++ = reps;

		*buf++ = redBuf[0];

		numToWrite += 2;



		*bufStart = (unsigned char) (count & 0xff);

		*(bufStart+1) = (unsigned char) ((flag << 4) + (count >> 8));



		if(ImBinWrite(ioType,fd,fp,bufStart,UCHAR,1,numToWrite)== -1 )

		{

			free( bufStart );

			ImReturnBinError();

		}



		buf = bufStart+2;

		numWritten += numToWrite;

		totWritten += numToWrite;

	}



        /*

         * The rest of the last block needs to be completed so that any block

         * reads that do not check for the end of the file can read blindly

         */

        if ( numWritten < IMPICBLOCK )

        {

                unsigned char       *buf;



                numToWrite = IMPICBLOCK - numWritten;

                ImMalloc( buf, unsigned char*, numToWrite );

                memset( (void *)buf, 0x00, numToWrite );



                if(ImBinWrite(ioType,fd,fp,buf,UCHAR,1,numToWrite)== -1 )

                {

                        free( buf );

                        ImReturnBinError();

                }



                totWritten += numToWrite;

                free( buf );

        }



        /*

         *  Now that all the image data has been written, the header must

         *  be written; thus, seek back to the start of the file and write it

         */

        ImSeek( ioType, fd, fp, 0, 0 );



	/*

	 *  Write the PIXAR picture file header.

	 */

	if ( imPicWriteHeader( ioType, fd, fp, vfb, IMSINGLECHAN, IMPICENCODE8, IMNOALPHA, totWritten ) == -1 )

		return ( -1 );		/* Error already handled	*/



	free( bufStart );



	return ( 1 );

}



