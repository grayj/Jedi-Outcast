/**
 **	$Header: /roq/libim/imhdfread.c 1     11/02/99 4:38p Zaphod $
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

#define HEADER	"    $Header: /roq/libim/imhdfread.c 1     11/02/99 4:38p Zaphod $"

/**
 **  FILE
 **	imhdfread.c	-  HDF image file read
 **
 **  PROJECT
 **	libim		-  SDSC image manipulation library
 **
 **  DESCRIPTION
 **	imhdf.c contains routines to read HDF image files for
 **	the image manipulation library.  Raster data read in is stored
 **	in a VFB and optional CLT in a tag table.
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
 **	ImHdfRead	f  read an HDF file
 **
 **	imHdfDDList	v  list of DDs
 **	imHdfDDListEnd	v  pointer to end of DD list
 **	imHdfDDCount	v  # of entries in DD list
 **
 **	imHdfDDEmpty	f  empty the DD list
 **	imHdfDDAppend	f  append to the DD list
 **	imHdfDDFind	f  search the DD list
 **
 **	imHdfCltList	v  list of CLT's being written out
 **	imHdfCltListEnd	v  end of the CLT list
 **
 **	imHdfCltEmpty	f  empty the Clt list
 **	imHdfCltAppend	f  append to the Clt list
 **	imHdfCltFind	f  find entry based on its CLT pointer
 **	imHdfCltFindRef	f  find entry based on its LUT reference number
 **
 **	imHdfByteOrder	v  data byte order
 **	imHdfFloatFormat v  data float format
 **	imHdfRef	v  current reference number
 **
 **	imHdfDimRead	f  read dimensions
 **	imHdfCltRead	f  read in a CLT
 **	imHdfVfbRead	f  read in a VFB
 **
 **	imHdfVfbRead8		f  read 8-bit uncomp. VFB
 **	imHdfVfbReadRLE8	f  read 8-bit RLE comp. VFB
 **	imHdfVfbRead32		f  read 32-bit uncomp. VFB
 **	imHdfVfbReadRGB		f  read 24-bit uncomp. uninterleaved VFB
 **	imHdfVfbReadRGBLine	f  read 24-bit uncomp. line interleaved VFB
 **	imHdfVfbReadRGBPlane 	f  read 24-bit uncomp. plane interleaved VFB
 **	imHdfVfbReadRLERGB	f  read 24-bit RLE comp. uninterleaved VFB
 **	imHdfVfbReadRLERGBLine	f  read 24-bit RLE comp. line interleaved VFB
 **	imHdfVfbReadRLERGBPlane f  read 24-bit RLE comp. plane interleaved VFB
 **
 **  HISTORY
 **	$Log: /roq/libim/imhdfread.c $
* 
* 1     11/02/99 4:38p Zaphod
 **	Revision 1.20  1995/06/30  22:00:40  bduggan
 **	changed comparison, since we got stuck in a
 **	loop with c++ on decalpha's
 **
 **	Revision 1.19  1995/06/29  00:28:04  bduggan
 **	updated copyright year
 **
 **	Revision 1.18  1995/06/15  20:23:28  bduggan
 **	Added an include, a prototype, and a return.
 **	removed an unused variable or two.
 **
 **	Revision 1.17  1995/04/03  21:25:23  bduggan
 **	took out #ifdef NEWMAGIC
 **
 **	Revision 1.16  1995/01/10  23:26:36  bduggan
 **	Put in IMMULTI/IMPIPE instead of TRUE/FALSE,
 **	Made read/write routines static
 **
 **	Revision 1.15  94/10/03  11:30:09  nadeau
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
 **	Revision 1.14  92/12/03  01:48:26  nadeau
 **	Corrected info messages.
 **	
 **	Revision 1.13  92/12/01  17:28:48  nadeau
 **	Updated write map and corrected info messages.
 **	
 **	Revision 1.12  92/11/23  18:42:26  nadeau
 **	Removed use of IMINFOMSG.
 **	
 **	Revision 1.11  92/11/04  11:49:36  groening
 **	put ImFIleFormat info and magic number info
 **	from imfmt.c into this file.
 **	
 **	Revision 1.10  92/10/19  14:14:29  groening
 **	added ImInfo statements
 **	
 **	Revision 1.9  92/08/31  17:24:52  vle
 **	Updated copyright notice.
 **	
 **	Revision 1.8  92/04/09  09:34:23  groening
 **	To make the compiler happy added extern statements.
 **	
 **	Revision 1.7  91/10/03  09:04:17  nadeau
 **	Changed 'interlace' to 'interleave'.  Moved write
 **	support to imhdfwrite.c so that the file was smaller
 **	and would compile easier on small systems.  Moved
 **	opcode and flag #defines to imhdfinternal.h.  Various
 **	cosmetic changes.
 **	
 **	Revision 1.6  91/02/12  10:51:18  nadeau
 **	Removed the tag table checking, temp file creation, and
 **	VFB conversion now handled by ImFileRead and ImFileWrite.
 **	
 **	Revision 1.5  91/01/30  18:06:29  nadeau
 **	Changed a bit of obscure C trickery to something more
 **	acceptable to the Alliant FX2800 C compiler.
 **	
 **	Revision 1.4  91/01/09  13:38:32  nadeau
 **	Fixed malloc sizes for RLE buffers.  They didn't handle the worst
 **	case expansion factor on the HDF RLE scheme.
 **	
 **	Revision 1.3  90/12/12  20:09:21  rama
 **	Added function declarations for imHdfVfbWrite functions
 **	since they return a long value rather than an int.
 **	
 **	Revision 1.2  90/07/25  16:23:57  nadeau
 **	Updated comments.
 **	
 **	Revision 1.1  90/07/23  13:47:31  nadeau
 **	Initial revision
 **	
 **/

//#include <unistd.h>
#include "iminternal.h"
#include "imhdfinternal.h"




/*
 **  FORMAT
 **	HDF	-  Hierarchical Data Format
 **
 **  AKA
 **	df, ncsa
 **
 **  FORMAT REFERENCES
 **	NCSA HDF Specifications, NCSA
 **
 **  CODE CREDITS
 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1990.
 **
 **  DESCRIPTION
 **	An HDF (Hierarchical Data Format) file is a "tagged" data format.
 **	This means that each piece of information in the file is labeled
 **	in a standard way.  These labels are called "Tags".
 **
 **	An HDF file starts with a magic number and a "DD Block".  A DD block
 **	is a block of space filled with DD's.  A DD is a "Data Descriptor"
 **	and is just a group of tags (and related information).  A DD block
 **	starts with a DDH (DD Header) that tells how many DD's there are in
 **	the block, and a file byte offset to the next DD block in the file.
 **	If there are no more DD blocks, this offset is a 0.
 **
 **	A DD contains the tag (a 16-bit unsigned integer), a reference
 **	number (just a 16-bit unsigned occurrence number for that type of
 **	tag), a file byte offset to the tag's data, and the number of bytes
 **	of data at that location.
 **
 **	There are lots of different tags.  Tags are informally grouped by
 **	functionalilty into "sets".  HDF currently defines the "utility set"
 **	of miscellaneous tags, the "raster-8 set" of 8-bit raster image
 **	tags (now out of date, but still supported), the "raster image set"
 **	for generic raster images, the "composite set" for describing the
 **	compositing of images, the "vector set" for Tektronix command streams,
 **	and the "scientific data set" for scientific data (huge arrays of
 **	floating point numbers, usually).
 **
 **	This code supports some of the utility tags and all of the raster
 **	image set tags.  The raster-8 set tags are considered redundant and
 **	are ignored in preference to the generic raster image set tags.
 **	Vector, composite, and scientific data set tags are ignored.
 **
 **	The raster image set defines a special tag, called the "raster image
 **	group" that just lists other tags (and their reference numbers) that
 **	together describe an image.  A typical raster image group would list
 **	a tag describing the image's dimensions (the ID tag), the raster
 **	image (the RI tag), and possibly a CLT dimension tag (the LD tag)
 **	and the CLT (the LUT tag).
 **
 **  FORMAT HISTORY
 **	HDF was developed by NCSA (National Center for Supercomputing
 **	Applications), one of four national supercomputer centers funded
 **	by the National Science Foundation.  SDSC (San Diego Supercomputer
 **	Center) is another of the four centers.
 **
 **	NCSA supplies a C and FORTRAN callable routine library for read and
 **	writing HDF files.
 **
 **	We do not use the NCSA HDF library for a variety of reasons:
 **
 **		1.  NCSA's HDF library is not structured to be easily portable.
 **		    A great many of the byte order and floating point format
 **		    oddities in the world are exposed to the HDF library user.
 **
 **		    We have opted to use the portable binary I/O library
 **		    developed at SDSC to portably cover up this stuff as it
 **		    is being read in.
 **
 **		2.  NCSA's HDF library does not handle pipes.  It does the
 **		    opening itself and thus requires a file name.  The image
 **		    library has been structured more generically.  Input might
 **		    not be a file.  In any case, the file's name is not always
 **		    available at this level.
 **
 **		3.  NCSA's HDF library only supports one open HDF file at a
 **		    time (as of version 2.37).  The application may be using
 **		    the HDF library to manage a scientific data set file when
 **		    it calls the image library.  If we usurp the HDF library
 **		    for reading in the image, we have messed up the application.
 **
 **		4.  If we use the NCSA HDF library, then all applications that
 **		    link with the image library must also linked with an HDF
 **		    library, whether they make HDF calls themselves or not.
 **		    This is awkward.  Inclusion of the HDF code within the
 **		    image library is equally awkward and slightly immoral.
 **
 **		    Additionally, because the rest of the image library is
 **		    more portable than NCSA's code, there will be cases
 **		    where the image library can function on a new machine,
 **		    but NCSA hasn't ported the HDF library to it yet.  We
 **		    will be stuck waiting on NCSA.
 **
 **	So, we read in the HDF files directly, without use of NCSA's library.
 **	We use the portable binary I/O library and none of NCSA's source,
 **	and none of their include files.
 **
 **  FORMAT PROBLEMS
 **	HDF excels as a flexible file format.... at the cost of increased
 **	complexity.  That complexity makes it difficult to make a generic
 **	access library that covers up byte order and floating point formats.
 **
 **	HDF tags come in three flavors:
 **
 **		1.  Self-contained tags.  These tags give the names of things,
 **		    attributes of a type (int, float, etc), or attributes of
 **		    something else (image aspect ratio).
 **
 **		2.  Tags that reference other tags.  These tags include
 **		    within their data the tag number and reference number of
 **		    one or more other tags.  The raster image group is just
 **		    a list of such tag/ref pairs.  The image dimension tag (ID)
 **		    indicates a tag/ref describing a pixel channel (red, green,
 **		    whatever), and another tag/ref giving the compression
 **		    scheme used.
 **
 **		3.  Tags that require other tags in order to be read in.
 **		    A raster image requires info in an image dimension tag
 **		    in order to be read in.  Similarly with CLT's.
 **
 **	Because of this high degree of interconnectivity, a generic package
 **	design is tough.  There is no obvious place at which to begin to
 **	modularize... except at the "read it all in and give me a list of
 **	images" level represented by this source.
 **
 **	NCSA's HDF library modularizes by having calls to read in a tag and
 **	its byte stream of data.  It is then up to the caller to figure out
 **	byte order and floating point format oddities.
 **
 **	The upshot of all this is that we are using a monolithic coding style
 **	to read in the HDF file.  Modularization is purely for the convenience
 **	of this code and does not constitute any kind of generic HDF file
 **	read/write package.  It's not clear a good design for such a package
 **	is even possible.
 **
 **	Nevertheless, SDSC does support and applaud HDF.  It is an excellent,
 **	generic, and portable file format that completely describes its own
 **	contents.  Good work NCSA!
 **
 **  FORMAT VARIATIONS SUPPORTED
 **	The following image forms are read and written:
 **
 **	Chan.	# of	Inter-	Comp-
 **	Size	Chan.	lace	ress.	Meaning
 **	------- ------- ------- ------- ---------------------------------------
 **	1 byte	1	none	none	8-bit index
 **				RLE	8-bit index, compressed
 **
 **	2 byte	1	none	none	16-bit index
 **
 **	3 byte	1	none	none	24-bit index
 **
 **	4 byte	1	none	none	32-bit index
 **
 **	1 byte	3	none	none	24-bit RGB
 **				RLE	24-bit RGB, compressed
 **			line	none	24-bit RGB, scanline interleave
 **				RLE	24-bit RGB, scanline interleave, comp.
 **			plane	none	24-bit RGB, plane interleave
 **				RLE	24-bit RGB, plane interleave, compressed
 **
 **	RLE compression is not supported on 2, 3, and 4 byte indexes because
 **	of the poor choice of RLE definition used by the HDF spec.  RLE
 **	is defined to be byte-wise.  On multiple-byte-per-channel data, this
 **	would try to find runs between bytes, rather than between full channel
 **	integers.  A run would only occur when consequitive bytes of the same
 **	channel's integer happened to have the same bit pattern.  Very rare and
 **	very stupid.  It also prevents the hiding of byte-order nonsense
 **	below the level of format knowledge (such as in the binary I/O package
 **	used by this code).  So, RLE is not supported for multi-byte-per-channel
 **	data.
 **
 **	NCSA's IMCOMP compression scheme is not supported.
 **
 **	The following CLT forms are read and written:
 **
 **	Chan.	# of	Inter-	Comp-
 **	Size	Chan.	lace	ress.	Meaning
 **	------- ------- ------- ------- ---------------------------------------
 **	1 byte	3	none	none	8-bit per primary
 **			line		8-bit per primary, line interleaved
 **
 **	Plane interleave has no meaning on CLT's.
 **
 **	RLE and IMCOMP compressions are not supported on CLT's.
 **
 **	Only 1-byte-per-primary and 3-primary CLT's are handled, ie. RGB CLT's.
 **	The Image library doesn't currently support CLT's with larger
 **	primaries or more than 3 primaries.
 **
 **	CLT's may be of any length, including longer than is indexable by
 **	a pixel in the CLT.
 **
 **	The following tags are read and written:
 **
 **	Tag	Set		Meaning
 **	------- ---------------	------------------------------------------------
 **	NT	Utility		Number type
 **	MT	Utility		Machine type
 **	RLE	Utility		RLE compression
 **
 **	ID8	Raster-8	8-bit image dimensions
 **	IP8	Raster-8	8-bit CLT
 **	RI8	Raster-8	8-bit image
 **	CI8	Raster-8	8-bit compressed image
 **
 **	RIG	Raster		Raster image group
 **	ID	Raster		Image dimensions
 **	LD	Raster		CLT dimensions
 **	RI	Raster		Raster image
 **	CI	Raster		Compressed image
 **	LUT	Raster		CLT
 **	CFM	Raster		Color format
 **
 **	The CCN (Color correction) and AR (aspect ratio) tags of the general
 **	raster set are not supported.
 **
 **	Matte channels are not yet supported.
 **
 **	Composition, vector, and scientific data set tags are not supported.
 **/



#ifdef __STDC__
static int imHdfDimRead( int ioType, int fd, FILE *fp, imHdfDim **pDim );
static int imHdfCltRead( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImClt **pClt );
static int imHdfVfbRead(int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb );
static int imHdfVfbRead8( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb);
static int imHdfVfbReadRLE8( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb);
static int imHdfVfbRead32( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb);
static int imHdfVfbReadRGB( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb);
static int imHdfVfbReadRGBLine( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb);
static int imHdfVfbReadRGBPlane( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb);
static int imHdfVfbReadRLERGB( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb);
static int imHdfVfbReadRLERGBLine( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb);
static int imHdfVfbReadRLERGBPlane( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb);
static int ImHdfRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );
extern int ImHdfWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 
	TagTable *tagTable );
#else
static int imHdfDimRead();
static int imHdfCltRead();
static int imHdfVfbRead();
static int imHdfVfbRead8();
static int imHdfVfbReadRLE8();
static int imHdfVfbRead32( );
static int imHdfVfbReadRGB();
static int imHdfVfbReadRGBLine();
static int imHdfVfbReadRGBPlane();
static int imHdfVfbReadRLERGB() ;
static int imHdfVfbReadRLERGBLine();
static int ImHdfRead( );
extern int ImHdfWrite( );
#endif

/*
 *  HDF -  Hierarchical Data Format
 *      For information on these structures, how to use them, etc. please
 *      see imfmt.c.
 */

static char *imHdfNames[ ]  = { "hdf", "df", "ncsa", NULL };
static unsigned char imHdfMagicNumber[ ] = { 0x0E, 0x03, 0x13, 0x01 };
static ImFileMagic imFileHdfMagic []=
{
	{ 0, 4, imHdfMagicNumber},
	{ 0, 0, NULL },
	NULL
};

static ImFileFormatReadMap imHdfReadMap[ ] =
{
        /* in                   out                                     */
        /* type,ch,dep, attr.   VFB type        attr.                   */
        { IN,1,8,       0,      IMVFBINDEX8,    0 },
        { IN,1,8,       C,      IMVFBINDEX8,    C },

        { IN,1,16,      0,      IMVFBINDEX16,   0 },
        { IN,1,16,      C,      IMVFBINDEX16,   C },

        { IN,1,24,      0,      IMVFBINDEX16,   0 },
        { IN,1,24,      C,      IMVFBINDEX16,   C },

        { IN,1,32,      0,      IMVFBINDEX16,   0 },
        { IN,1,32,      C,      IMVFBINDEX16,   C },

        { RGB,3,8,      0,      IMVFBRGB,       0 },
        { RGB,3,8,      LI,     IMVFBRGB,       0 },
        { RGB,3,8,      PI,     IMVFBRGB,       0 },

        { IN,1,8,       RLE,    IMVFBINDEX8,    0 },
        { IN,1,8,       RLE | C,IMVFBINDEX8,    C },

        { RGB,3,8,      RLE,    IMVFBRGB,       0 },
        { RGB,3,8,      RLE |LI,IMVFBRGB,       0 },
        { RGB,3,8,      RLE |PI,IMVFBRGB,       0 },
        { -1,           0,      -1,             0 },
};
static ImFileFormatWriteMap imHdfWriteMap[ ] =
{
        /*
         *  For HDF, all image types vector thru to the same ImHdfWrite()
         *  routine.  This is necessary because a tagTable may be passed in
         *  that contains multiple VFB's, each with different depth, CLT
         *  and alpha attributes.  The write map primarily serves as a
         *  filter to make sure all such incomming VFB's are in one of the
         *  supported formats.
         */

        /* in                   out                                     */
        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */
        { IMVFBINDEX8,  C,      IN,1,8,         C,      ImHdfWrite },
        { IMVFBINDEX8,  0,      IN,1,8,         0,      ImHdfWrite },
        { IMVFBINDEX8,  C,      IN,1,8,         RLE|C,  ImHdfWrite },
        { IMVFBINDEX8,  0,      IN,1,8,         RLE,    ImHdfWrite },

        /* No RLE index 16 because HDF's RLE definition is braindamaged */
        { IMVFBINDEX16, C,      IN,1,16,        C,      ImHdfWrite },
        { IMVFBINDEX16, 0,      IN,1,16,        0,      ImHdfWrite },

#ifdef hdf_24_compressed
        /*
         *  NCSA's HDF tools don't fully support 24-bit images:
         *      NCSA Image 2.0 tries to read in 24-bit images, but bombs.
         *      NCSA Image 3.0 rejects 24-bit images with a cryptic message.
         *      SypGlass View 1.0 rejects 24-bit images with a message.
         *
         *      hdf24hdf8 accepts uncompressed 24-bit images, rejects
         *      compressed interleaved 24-bit images, and incorrectly grabs
         *      only the 1st third of the data for compressed uninterleaved
         *      24-bit images.
         *
         *  Until NCSA's library fully supports compressed 24-bit images,
         *  our support will be disabled.  Note that we will continue to
         *  read compressed 24-bit images.
         */
        { IMVFBRGB,     0,      RGB,3,8,        RLE|PI, ImHdfWrite },
        { IMVFBRGB,     0,      RGB,3,8,        RLE|LI, ImHdfWrite },
        { IMVFBRGB,     0,      RGB,3,8,        RLE,    ImHdfWrite },
#endif
        { IMVFBRGB,     0,      RGB,3,8,        0,      ImHdfWrite },
        { IMVFBRGB,     0,      RGB,3,8,        LI,     ImHdfWrite },
        { IMVFBRGB,     0,      RGB,3,8,        PI,     ImHdfWrite },
        { -1,           0,      -1,             0,      NULL },
};


ImFileFormat ImFileHdfFormat =
{
	imHdfNames, "Hierarchical Data File",
	"NCSA",
	"8-bit color index images, un- and RLE-compressed.  16-, 24-, and\n\
2-bit color index images, un-compressed.  24-bit RGB, un- and RLE-\n\
ompressed, un-, scanline- and plane-interleaved.  Raster image\n\
roups as well as older Raster-8 files.",
"8-bit color index images, un- and RLE-compressed.  16-bit color\n\
ndex images, un-compressed.  24-bit RGB, uncompressed, un-,\n\
canline- and plane-interleaved.  Raster image group and older\n\
aster-8 tags included.",
imFileHdfMagic,
	IMMULTI, IMNOPIPE, /* Read */
	IMMULTI, IMNOPIPE, /* Write */
	ImHdfRead, imHdfReadMap, imHdfWriteMap
};

/*
 *  DD List Management
 */

/*
 *  GLOBALS
 *	imHdfDDList	-  list of DDs
 *	imHdfDDListEnd	-  pointer to end of DD list
 *	imHdfDDCount	-  # of entries in DD list
 */

imHdfDD *imHdfDDList = NULL;	/* List of DD's			*/
imHdfDD *imHdfDDListEnd = NULL;/* Pointer to end of DD list	*/
int      imHdfDDCount = 0;	/* # of DD's in list		*/





/*
 *  FUNCTION
 *	imHdfDDEmpty	-  empty the DD list
 *	imHdfDDAppend	-  append to the DD list
 *	imHdfDDFind	-  search the DD list
 *
 *  DESCRIPTION
 *	A DD list of tag/ref values is created and maintained.
 */

void				/* Returns nothing		*/
imHdfDDEmpty( )
{
	imHdfDD *pDD;			/* DD list pointer		*/
	imHdfDD *pDD2;			/* 2nd DD list pointer		*/

	for ( pDD = imHdfDDList; pDD; )
	{
		pDD2 = pDD;
		pDD = pDD->dd_next;
		free( (char *)pDD2 );
	}
	imHdfDDList = imHdfDDListEnd = NULL;
	imHdfDDCount = 0;
}

imHdfDD *			/* Returns new DD pointer	*/
#ifdef __STDC__
imHdfDDAppend( unsigned int tag, unsigned int ref, long dataOffset, long dataLength )
#else
imHdfDDAppend( tag, ref, dataOffset, dataLength )
	unsigned int tag;			/* Tag number			*/
	unsigned int ref;			/* Reference number		*/
	long dataOffset;		/* File offset			*/
	long dataLength;		/* Number of bytes		*/
#endif
{
	imHdfDD *pDD;			/* New DD			*/

	if ( (pDD = (imHdfDD *)malloc( sizeof( imHdfDD ) )) == NULL )
	{
		ImErrNo = IMEMALLOC;
		ImErrorFatal( ImQError(), NULL, ImErrNo );
	}
	pDD->dd_tag        = tag;
	pDD->dd_ref        = ref;
	pDD->dd_dataOffset = dataOffset;
	pDD->dd_dataLength = dataLength;

	if ( imHdfDDList == NULL )
		imHdfDDList = pDD;
	else
		imHdfDDListEnd->dd_next = pDD;
	pDD->dd_next = NULL;
	imHdfDDListEnd = pDD;
	imHdfDDCount++;
	return ( pDD );
}

imHdfDD *			/* Returns found DD pointer	*/
#ifdef __STDC__
imHdfDDFind( unsigned int tag, unsigned int ref )
#else
imHdfDDFind( tag, ref )
	unsigned int tag;			/* Tag number			*/
	unsigned int ref;			/* Reference number		*/
#endif
{
	imHdfDD *pDD;			/* DD list pointer		*/

	for ( pDD = imHdfDDList; pDD; pDD = pDD->dd_next )
		if ( pDD->dd_tag == tag && pDD->dd_ref == ref )
			break;
	return ( pDD );			/* Could be NULL		*/
}






/*
 *  CLT List Management
 */

/*
 *  GLOBALS
 *	imHdfCltList	-  list of CLT's being written out
 *	imHdfCltListEnd	-  end of the CLT list
 */

imHdfClt *imHdfCltList = NULL;
imHdfClt *imHdfCltListEnd = NULL;





/*
 *  FUNCTION
 *	imHdfCltEmpty	-  empty the Clt list
 *	imHdfCltAppend	-  append to the Clt list
 *	imHdfCltFind	-  find entry based on its CLT pointer
 *	imHdfCltFindRef	-  find entry based on its LUT reference number
 *
 *  DESCRIPTION
 *	A Clt list of tag/ref values is created and maintained.
 */

void				/* Returns nothing		*/
imHdfCltEmpty( )
{
	imHdfClt *pClt;			/* Clt list pointer		*/
	imHdfClt *pClt2;		/* 2nd Clt list pointer		*/

	for ( pClt = imHdfCltList; pClt; )
	{
		pClt2 = pClt;
		pClt  = pClt->clt_next;
		free( (char *)pClt2 );
	}
	imHdfCltList = imHdfCltListEnd = NULL;
}

imHdfClt *			/* Returns new Clt pointer	*/
#ifdef __STDC__
imHdfCltAppend( ImClt* clt, unsigned int refLUT, unsigned int refLD )
#else
imHdfCltAppend( clt, refLUT, refLD )
	ImClt *clt;			/* CLT to add to list		*/
	unsigned int   refLUT;			/* It's LUT tag reference #	*/
	unsigned int   refLD;			/* It's LD tag reference #	*/
#endif
{
	imHdfClt *pClt;			/* New Clt list entry		*/

	if ( (pClt = (imHdfClt *)malloc( sizeof( imHdfClt ) )) == NULL )
	{
		ImErrNo = IMEMALLOC;
		ImErrorFatal( ImQError(), NULL, ImErrNo );
	}
	pClt->clt_clt    = clt;
	pClt->clt_refLUT = refLUT;
	pClt->clt_refLD  = refLD;

	if ( imHdfCltList == NULL )
		imHdfCltList = pClt;
	else
		imHdfCltListEnd->clt_next = pClt;
	pClt->clt_next = NULL;
	imHdfCltListEnd   = pClt;
	return ( pClt );
}

imHdfClt *			/* Returns found Clt pointer	*/
#ifdef __STDC__
imHdfCltFind(ImClt * clt )
#else
imHdfCltFind( clt )
	ImClt *clt;			/* CLT to look for		*/
#endif
{
	imHdfClt *pClt;			/* Clt list pointer		*/

	for ( pClt = imHdfCltList; pClt; pClt = pClt->clt_next )
		if ( pClt->clt_clt == clt )
			break;
	return ( pClt );		/* Could be NULL		*/
}

imHdfClt *			/* Returns found Clt pointer	*/
#ifdef __STDC__
imHdfCltFindRef( unsigned int ref )
#else
imHdfCltFindRef( ref )
	unsigned int ref;			/* Reference number to look for	*/
#endif
{
	imHdfClt *pClt;			/* Clt list pointer		*/

	for ( pClt = imHdfCltList; pClt; pClt = pClt->clt_next )
		if ( pClt->clt_refLUT == ref )
			break;
	return ( pClt );		/* Could be NULL		*/
}





/*
 *  GLOBALS
 *	imHdfByteOrder	-  data byte order
 *	imHdfFloatFormat	-  data float format
 *	imHdfRef		-  current reference number
 */
int imHdfByteOrder;		/* Default byte order		*/
int imHdfFloatFormat;		/* Default float format		*/
sdsc_uint16 imHdfRef = 100;		/* Current reference number	*/





/*
 *  FUNCTION
 *	ImHdfRead	-  read an HDF file
 *
 *  DESCRIPTION
 *	ImHdfRead() reads in an HDF file and adds all images and CLTs to
 *	the tag table.
 *
 *	HDF files are structured such that we have to do multiple passes on
 *	the file, and then on the data we read in in order to get things
 *	done...
 *
 *	1.  Read in all of the Data Descriptors (DDs), each one describing
 *	    one tag in the file.  A DD includes the file byte offset to get
 *	    to the actual data for the tag.
 *
 *	    We can't immediately go out and get the data when we read in a
 *	    DD because we don't know what to do with it yet.  All raster
 *	    image set tags are used by one or more raster image groups (RIGs)
 *	    and we don't know what to do with, say, an ID (image dimension)
 *	    tag until we've read in the RIG(s) that reference it.
 *
 *	    While reading in DD's, we toss ones for tags we don't handle.
 *	    We also build up a linked list of RIG tags that we've found.  The
 *	    RIG tags are sorted by reference count, which may be (but usually
 *	    isn't) different from the order found in the file.
 *
 *	2.  Read in all the RIGs.  Each RIG is a list of tag/ref values
 *	    that refer to tags that in turn refer to data relevant for this
 *	    image.  The order of tags listed in a RIG is undefined.  We'd
 *	    like to just read in the data for each of the RIG's tags,
 *	    but we can't always.  For instance, to read in an image,
 *	    we have to first know how big it is.  That's told by an ID tag.
 *	    But we might not have read in the ID tag for the RIG yet.  Sigh.
 *
 *	3.  Read in each RIG's CLT, image, and matte data based upon the
 *	    ID, etc, tag information we read in in (2).  Now we can process
 *	    the file's CLT into an ImCLT, and uncompress the file's image
 *	    into an ImVfb.  As we finish each RIG, we add the ImClt and ImVfb
 *	    to the tag table.
 */

static int				/* Returns status		*/
#ifdef __STDC__
ImHdfRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )
#else
ImHdfRead( ioType, fd, fp, flagsTable, tagTable )
	int         ioType;		/* I/O flags			*/
	int         fd;			/* Input file descriptor	*/
	FILE	   *fp;			/* Input file pointer		*/
	TagTable   *flagsTable;		/* Format flags			*/
	TagTable   *tagTable;		/* Tag list to add to		*/
#endif
{
	int  i;		/* Counter			*/
	ImVfb        *vfb;		/* virtual frame buffer		*/
	ImClt        *clt;		/* color lookup table		*/

	unsigned char         magic[4];		/* Magic number			*/
	char          message[1024];	/* Error message holder		*/

	unsigned int          nDD;		/* # of DD's in block		*/
	long          offset;		/* Offset to next DD block	*/

	imHdfDD      *pDD;		/* DD pointer			*/
	imHdfDD      *pDD2;		/* 2nd DD pointer		*/
	imHdfDD      *pPrevDD;		/* Previous DD pointer		*/

	imHdfClt     *pClt;		/* Clt information		*/

	imHdfRIG     *rigList;		/* List of RIG's		*/
	imHdfRIG     *rigListEnd;	/* Pointer to end of RIG list	*/
	imHdfRIG     *pRIG;		/* RIG pointer			*/
	imHdfRIG     *pRIG2;		/* 2nd RIG pointer		*/
	imHdfRIG     *pPrevRIG;		/* Previous RIG pointer		*/

	int	      nTag;		/* # of tags in a RIG		*/
	unsigned int	      tagList[50];	/* List of tags in a RIG	*/
	unsigned int	      refList[50];	/* and their ref numbers	*/

	unsigned int	      tag;		/* DD tag			*/
	unsigned int	      ref;		/* DD reference number		*/
	long	      dataOffset;	/* Offset to data		*/
	long	      dataLength;	/* Length of data		*/
	int	      imageNum;		/* number of images in a rig list */
	int	      imageNumII;	/* number of images in a rig list */

	/*
	 *  All administrative stuff (tags, reference counts, file offsets)
	 *  are in MBF byte order.
	 */
	BinByteOrder( BINMBF );

	ImInfo ("Byte Order (header)",
		"Most Significant Byte First");


	/*
	 *  Read in the magic number.
	 */
	Read( magic, UCHAR, 1, 4 );
	if ( magic[0] != 0x0E || magic[1] != 0x03 || magic[2] != 0x13 ||
		magic[3] != 0x01 )
	{
		ImErrNo = IMEMAGIC;
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	}


	/*
	 *  Read in the Data Descriptor Header (DDH) and each of the
	 *  Data Descriptors (DDs) in the block.  Create a linked list of
	 *  the DDs, then advance to the next DDH.
	 */
	imHdfByteOrder   = BINMBF;
	imHdfFloatFormat = BINIEEE;
	imHdfDDEmpty( );
	rigList = rigListEnd = NULL;
	for ( ; ; )
	{
		Read( &nDD,    UINT, 2, 1 );
		Read( &offset, LONG, 4, 1 );
		for ( i = 0; i < nDD; i++ )
		{
			Read( &tag,        UINT, 2, 1 );
			Read( &ref,        UINT, 2, 1 );
			Read( &dataOffset, LONG, 4, 1 );
			Read( &dataLength, LONG, 4, 1 );

			/*
			 *  Toss tags we don't care about and process those
			 *  we do.
			 */
			switch ( tag )
			{
			/* Keep number type tags			*/
			case IMHDFTNT:	/* Number type			*/
				if ( imHdfDDAppend( tag, ref, dataOffset,
					dataLength ) == NULL )
					return( -1 );	/* Error handled*/
				break;

			/* Keep raster image set tags			*/
			case IMHDFTCFM:	/* Color format			*/
			case IMHDFTID:	/* Image dimension		*/
			case IMHDFTLD:	/* Lookup table dimension	*/
			case IMHDFTMD:	/* Matte dimension		*/
			case IMHDFTMA:	/* Matte			*/
			case IMHDFTCCN:	/* Color correction		*/
			case IMHDFTAR:	/* Aspect ratio			*/
			case IMHDFTRI:	/* Raster image			*/
			case IMHDFTCI:	/* Compressed raster image	*/
			case IMHDFTLUT:	/* Lookup table			*/
			case IMHDFTMTO:	/* Machine-Type Override	*/
				if ( imHdfDDAppend( tag, ref, dataOffset,
					dataLength ) == NULL )
					return( -1 );	/* Error handled*/
				break;

			/* Add RIG's to a list, in ref number order	*/
			case IMHDFTRIG:	/* Raster image group		*/
				if ( (pDD = imHdfDDAppend( tag, ref, dataOffset,
					dataLength ) ) == NULL )
					return( -1 );	/* Error handled*/

				ImMalloc( pRIG, imHdfRIG *, sizeof( imHdfRIG ) );
				pRIG->rig_dd = pDD;

				/* Set defaults for RIGs.		*/
				pRIG->rig_imageDim = NULL;
				pRIG->rig_imageDD  = NULL;
				pRIG->rig_cltDim   = NULL;
				pRIG->rig_cltDD    = NULL;
				pRIG->rig_matteDim = NULL;
				pRIG->rig_matteDD  = NULL;
				pRIG->rig_ccnGiven = FALSE;
				pRIG->rig_colorFormat = IMHDFCVALUE;
				pRIG->rig_aspectRatio = 1.0;

				/* If list is empty, make it start of list*/
				if ( rigList == NULL )
				{
					rigList = rigListEnd = pRIG;
					pRIG->rig_next = NULL;
					break;
				}

				/* If ref > last in list, append to list*/
				if ( imHdfDDQRef( pDD ) > imHdfDDQRef( rigListEnd->rig_dd ) )
				{
					rigListEnd->rig_next = pRIG;
					pRIG->rig_next = NULL;
					rigListEnd = pRIG;
					break;
				}

				/* If ref < first in list, start list	*/
				if ( imHdfDDQRef( pDD ) < imHdfDDQRef( rigList->rig_dd ) )
				{
					pRIG->rig_next = rigList;
					rigList = pRIG;
					break;
				}

				/* Search list for proper position	*/
				pPrevRIG = rigList;
				pRIG2    = rigList->rig_next;
				while ( pRIG2 )
				{
					if ( imHdfDDQRef( pDD ) > imHdfDDQRef( pRIG2->rig_dd ) )
					{
						pPrevRIG = pRIG2;
						pRIG2 = pRIG2->rig_next;
						continue;
					}
					pRIG->rig_next = pRIG2;
					pPrevRIG->rig_next = pRIG;
					break;
				}
				break;

			/* Immediately process machine type tags	*/
			case IMHDFTMT:	/* Machine type			*/
				switch ( (ref>>12)&0xF )
				{
				case IMHDFINTMBO:	/* Motorola byte order (MBF)*/
				    ImInfo ("Byte Order (RIG)",
					    "Most Significant Byte First");
					imHdfByteOrder = BINMBF;
					break;
				case IMHDFINTVBO:	/* Vax byte order (LBF)	*/
				case IMHDFINTIBO:	/* Intel byte order (LBF)*/
				    ImInfo ("Byte Order (RIG)",
					    "Least Significant Byte First");
					imHdfByteOrder = BINLBF;
					break;
				}

				switch ( (ref>>8)&0xF )
				{
				case IMHDFFLOATIEEE:/* IEEE		*/
					imHdfFloatFormat = BINIEEE;
					break;
				case IMHDFFLOATVAX:/* VAX			*/
					imHdfFloatFormat = BINVAX;
					break;
				case IMHDFFLOATCRAY:/* Cray		*/
					imHdfFloatFormat = BINCRAYMP;
					break;
				case IMHDFFLOATPC:/* PC?			*/
					imHdfFloatFormat = BINIEEE;/* unknown!*/
					break;
				}
				continue;

			/* Skip null tags				*/
			case IMHDFTNULL:	/* No data			*/
				continue;

			/* Skip generic compression description tags	*/
			case IMHDFTRLE:	/* Run length encoded data	*/
			case IMHDFTIMC:	/* IMCOMP compressed data	*/
				continue;

			/* Skip file, tag, and data identifiers		*/
			case IMHDFTFID:	/* File identifier		*/
			case IMHDFTFD:	/* File descriptor		*/
			case IMHDFTTID:	/* Tag identifier		*/
			case IMHDFTTD:	/* Tag descriptor		*/
			case IMHDFTDIL:	/* Data identifier label	*/
			case IMHDFTDIA:	/* Data identifier annotation	*/
				continue;

			/* Skip redundant raster-8 tags			*/
			case IMHDFTID8:	/* Image dimension-8		*/
			case IMHDFTIP8:	/* Image palette-8		*/
			case IMHDFTRI8:	/* Raster image-8		*/
			case IMHDFTCI8:	/* Compressed image-8		*/
			case IMHDFTII8:	/* IMCOMP image-8		*/
				continue;

			/* Skip composition tags			*/
			case IMHDFTDRAW:	/* Draw				*/
			case IMHDFTRUN:	/* Run				*/
			case IMHDFTXYP:	/* XY position			*/
				continue;

			/* Skip vector set tags				*/
			case IMHDFTT14:	/* Tektronix 4014		*/
			case IMHDFTT105:	/* Tektronix 4105		*/
				continue;

			/* Skip scientific data set tags		*/
			case IMHDFTSDG:	/* Scientific data group	*/
			case IMHDFTSDD:	/* Scientific data dimension record*/
			case IMHDFTSD:	/* Scientific data		*/
			case IMHDFTSDS:	/* Scientific data scales	*/
			case IMHDFTSDL:	/* Scientific data labels	*/
			case IMHDFTSDU:	/* Scientific data units	*/
			case IMHDFTSDF:	/* Scientific data format	*/
			case IMHDFTSDC:	/* Scientific data coordinates	*/
			case IMHDFTSDM:	/* Scientific data max/min	*/
			case IMHDFTSDT:	/* Scientific data transpose	*/
				continue;

			/* Skip all unknown tags			*/
			default:
				continue;
			}
		}
		if ( offset <= 0 )
			break;
		Seek( offset );
	}


	/*
	 *  Walk the RIG list.  For each RIG, read in its list of tags and
	 *  reference numbers, find each one, read it in, and add data to
	 *  the RIG structure.
	 *
	 *  We have to postpone reading in the actual CLT, image, and matte
	 *  data until the rest of the RIG tags have been read in and processed.
	 *  For instance, to read in an image we have to have the ID tag's
	 *  info at hand.  But that tag may be listed after the tag pointing
	 *  to the image data.  Sigh.
	 */
	BinByteOrder( imHdfByteOrder );		/* Data byte order	*/
	BinFloatFormat( imHdfFloatFormat );	/* Data float format	*/
	for ( pRIG = rigList; pRIG; pRIG = pRIG->rig_next )
	{
		pDD = pRIG->rig_dd;
		Seek( imHdfDDQDataOffset( pDD ) );
		nTag = imHdfDDQDataLength( pDD ) / 4;/* 4 bytes per tag/ref*/

		for ( i = 0; i < nTag; i++ )
		{
			Read( &tagList[i], UINT, 2, 1 );
			Read( &refList[i], UINT, 2, 1 );
		}


		/*
		 *  For each tag/ref in the RIG, search the DD list, then
		 *  read in its data (if possible).
		 */
		for ( i = 0; i < nTag; i++ )
		{
			if ( (pDD = imHdfDDFind( tagList[i], refList[i] )) == NULL)
			{
				sprintf( message, "RIG %d references non-existant tag %d/%d",
					imHdfDDQRef( pRIG->rig_dd ),
					tagList[i], refList[i] );
				ImErrorFatal( message, -1, IMESYNTAX );
			}

			switch ( tagList[i] )
			{
			case IMHDFTRI:	/* Raster image			*/
			case IMHDFTCI:	/* Compressed raster image	*/
				pRIG->rig_imageDD = pDD;
				break;

			case IMHDFTLUT:	/* CLT				*/
				pRIG->rig_cltDD = pDD;
				break;

			case IMHDFTMA:	/* Matte channel		*/
				pRIG->rig_matteDD = pDD;
				break;

			case IMHDFTAR:	/* Aspect ratio			*/
				Seek( imHdfDDQDataOffset( pDD ) );
				Read( &pRIG->rig_aspectRatio, FLOAT, 4, 1 );
				break;

			case IMHDFTCCN:	/* Color correction		*/
				pRIG->rig_ccnGiven = TRUE;
				Seek( imHdfDDQDataOffset( pDD ) );
				Read( &pRIG->rig_gamma, FLOAT, 4, 1 );
				Read( pRIG->rig_red,    FLOAT, 4, 3 );
				Read( pRIG->rig_green,  FLOAT, 4, 3 );
				Read( pRIG->rig_blue,   FLOAT, 4, 3 );
				Read( pRIG->rig_white,  FLOAT, 4, 3 );
				break;

			case IMHDFTCFM:	/* Color format			*/
			{
				char *buffer;

				ImMalloc( buffer, char *, sizeof( char ) *
					(imHdfDDQDataLength( pDD ) + 1) );
				Seek( imHdfDDQDataOffset( pDD ) );
				Read( buffer, CHAR, 1, imHdfDDQDataLength( pDD ) );
				buffer[imHdfDDQDataLength( pDD )] = '\0';

				if ( strcmp( "VALUE", buffer ) == 0 )
					pRIG->rig_colorFormat = IMHDFCVALUE;
				else if ( strcmp( "RGB", buffer ) == 0 )
					pRIG->rig_colorFormat = IMHDFCRGB;
				else if ( strcmp( "XYZ", buffer ) == 0 )
					pRIG->rig_colorFormat = IMHDFCXYZ;
				else if ( strcmp( "HSV", buffer ) == 0 )
					pRIG->rig_colorFormat = IMHDFCHSV;
				else if ( strcmp( "HSI", buffer ) == 0 )
					pRIG->rig_colorFormat = IMHDFCHSI;
				else if ( strcmp( "SPECTRAL", buffer ) == 0 )
					pRIG->rig_colorFormat = IMHDFCSPECTRAL;
				else
				{
					sprintf( message, "Unknown color format '%s'", buffer );
					ImErrorFatal( message, -1, IMESYNTAX );
				}

				free( (char *)buffer );
				break;
			}

			case IMHDFTID:	/* Image dimension		*/
				Seek( imHdfDDQDataOffset( pDD ) );
				if ( imHdfDimRead( ioType, fd, fp,
					&pRIG->rig_imageDim ) == -1 )
					return ( -1 );	/* Error stuff done*/
				break;

			case IMHDFTLD:	/* Lookup table dimension	*/
				Seek( imHdfDDQDataOffset( pDD ) );
				if ( imHdfDimRead( ioType, fd, fp,
					&pRIG->rig_cltDim ) == -1 )
					return ( -1 );	/* Error stuff done*/
				break;

			case IMHDFTMD:	/* Matte dimension		*/
				Seek( imHdfDDQDataOffset( pDD ) );
				if ( imHdfDimRead( ioType, fd, fp,
					&pRIG->rig_matteDim ) == -1 )
					return ( -1 );	/* Error stuff done*/
				break;
			}
		}


		/*
		 *  There may or may not be an LD tag giving the dimensions of
		 *  the lookup table.  SDSC tools always add the LD tag.
		 *  NCSA tools apparently do not.
		 *
		 *  Without an explicit LD, we create one:
		 *	width                = 256
		 *	height               = 1
		 *	channel type         = UCHAR
		 *	channel size         = 1
		 *	channel byte order   = BINMBF  (unneeded)
		 *	channel float format = BINIEEE (unneeded)
		 *	pixel size           = 3
		 *	interleave            = 0 (RGB, RGB, RGB, ...)
		 *	compression          = 0 (none)
		 *
		 *  These attributes match those of the IP8 pallette most
		 *  commonly used in cases where the LD tag is "forgotten".
		 */
		if ( pRIG->rig_cltDD != NULL && pRIG->rig_cltDim == NULL )
		{
			ImMalloc( pRIG->rig_cltDim, imHdfDim *, sizeof( imHdfDim ) );
			pRIG->rig_cltDim->dim_width              = 256;
			pRIG->rig_cltDim->dim_height             = 1;
			pRIG->rig_cltDim->dim_channelType        = UCHAR;
			pRIG->rig_cltDim->dim_channelSize        = 1;
			pRIG->rig_cltDim->dim_channelByteOrder   = BINMBF;
			pRIG->rig_cltDim->dim_channelFloatFormat = BINIEEE;
			pRIG->rig_cltDim->dim_pixelSize          = 3;
			pRIG->rig_cltDim->dim_interleave          = 0;
			pRIG->rig_cltDim->dim_compression        = 0;
		}


		/*
		 *  Confirm that we got all that we needed.
		 */
		if ( pRIG->rig_imageDD != NULL && pRIG->rig_imageDim == NULL )
			ImErrorFatal( "Missing image dimension tag for raster image",
				-1, IMESYNTAX );
		if ( pRIG->rig_matteDD != NULL && pRIG->rig_matteDim == NULL )
			ImErrorFatal( "Missing matte dimension tag for matte",
				-1, IMESYNTAX );
	}


	/*
	 *  Walk the RIG list and read in each RIG's CLT (if any) and image
	 *  (if any).
	 *
	 *  We don't keep track of multiple RIG's pointing to the same VFB.
	 *  We do keep track of multiple RIG's using the same CLT.
	 */
	nTag = 0;
	imHdfCltEmpty( );


	/* Count the number of images	*/
	imageNum=0;
	imageNumII=0;
	for ( pRIG = rigList; pRIG; pRIG = pRIG->rig_next )
		if ( pRIG->rig_imageDD ) imageNum++;

	for ( pRIG = rigList; pRIG; pRIG = pRIG->rig_next )
	{
		if ( pRIG->rig_imageDD )
		{
			imageNumII++;
			sprintf (message, "%d of %d",imageNumII, imageNum);
			ImInfo ("Image",message);
		}
		/*
		 *  Read in the CLT.
		 */
		clt = IMCLTNULL;
		if ( pRIG->rig_cltDD )
		{
			/*
			 *  Check if CLT has already been read in for another
			 *  image.
			 */
			if ( (pClt = imHdfCltFindRef( imHdfDDQRef( pRIG->rig_cltDD ) )) == NULL )
			{
				/* Nope.  Read it in and add it to our list.*/
				if ( imHdfCltRead( ioType, fd, fp, pRIG, &clt ) == -1 )
					return ( -1 );	/* Error stuff done already*/
				imHdfCltAppend( clt, imHdfDDQRef( pRIG->rig_cltDD ),
					0 );
				TagTableAppend( tagTable,
					TagEntryAlloc( "image clt", POINTER, &clt ) );
			}
			else
				clt = imHdfCltQClt( pClt );
			nTag++;
		}


		/*
		 *  Read in the image.
		 */
		vfb = IMVFBNULL;
		if ( pRIG->rig_imageDD )
		{
			if ( imHdfVfbRead( ioType, fd, fp, pRIG, &vfb ) == -1 )
				return ( -1 );	/* Error stuff done already*/
			ImVfbSClt( vfb, clt );	/* Could be NULL clt	*/
			TagTableAppend( tagTable,
				TagEntryAlloc( "image vfb", POINTER, &vfb ) );
			nTag++;
		}
	}

	imHdfDDEmpty( );
	imHdfCltEmpty( );

	return ( nTag );
}





/*
 *  FUNCTION
 *	imHdfDimRead	-  read dimensions
 *
 *  DESCRIPTION
 *	The ID, LD, and MD tags all share the same structuring of their
 *	data.  imHdfDimRead() reads in this set of dimension information and
 *	returns it in a new imHdfDim structure.
 *
 *	A dimension tag also references an NT tag (for the size of a pixel
 *	channel), and an optional compression tag.  These tags are searched
 *	for in the DD list and their information incorporated into the imHdfDim
 *	structure.
 */

static int				/* Returns status		*/
#ifdef __STDC__
imHdfDimRead( int ioType, int fd, FILE *fp, imHdfDim **pDim )
#else
imHdfDimRead( ioType, fd, fp, pDim )
	int         ioType;		/* I/O flags			*/
	int         fd;			/* Input file descriptor	*/
	FILE	   *fp;			/* Input file pointer		*/
	imHdfDim     **pDim;		/* Returned dimensions		*/
#endif
{
	unsigned int        tagNT, refNT;	/* NT tag and reference number	*/
	unsigned int        tagComp, refComp;	/* Compression tag and ref number*/
	unsigned char       nt[4];		/* NT information		*/
	imHdfDD    *pDD2;		/* 2nd DD pointer		*/
	imHdfDim      *dim;		/* New dimension information	*/
	char        message[1024];	/* Tmp error message text	*/


	/*
	 *  Create a new dimension description and read in the basic info.
	 */
	ImMalloc( dim, imHdfDim *, sizeof( imHdfDim ) );
	Read( &dim->dim_width,     INT,  4, 1 );
	Read( &dim->dim_height,    INT,  4, 1 );
	Read( &tagNT,              UINT, 2, 1 );
	Read( &refNT,              UINT, 2, 1 );
	Read( &dim->dim_pixelSize, INT,  2, 1 );
	Read( &dim->dim_interleave, INT,  2, 1 );
	Read( &tagComp,            UINT, 2, 1 );
	Read( &refComp,            UINT, 2, 1 );


	/*
	 *  Search the DD list for the NT tag.
	 */
	if ( (pDD2 = imHdfDDFind( tagNT, refNT )) == NULL )
		ImErrorFatal( "Dimension tag references nonexistant NT tag",
			-1, IMESYNTAX );


	/*
	 *  Read in the NT tag and decode it.
	 *
	 *  The NT tag's data is 4 1-byte quantities:
	 *
	 *	0	version number of NT tag (version 1 supported here)
	 *	1	type code
	 *	2	width of type, in bits
	 *	3	class code (byte order, float format, etc)
	 */
	Seek( imHdfDDQDataOffset( pDD2 ) );
	Read( nt, UCHAR, 1, 4 );

	dim->dim_channelSize        = ((nt[2] + 7) / 8);
	dim->dim_channelByteOrder   = imHdfByteOrder;	/* Default	*/
	dim->dim_channelFloatFormat = imHdfFloatFormat;	/* Default	*/

	switch ( nt[1] )
	{
	case IMHDFNTUINT:	/* unsigned int						*/
	case IMHDFNTINT:	/* int						*/
		dim->dim_channelType = (nt[1] == IMHDFNTUINT) ? UINT : INT ;
		switch ( nt[3] )
		{
		default:
		case IMHDFINTMBO:
			dim->dim_channelByteOrder = BINMBF;
			ImInfo ("Byte Order (data)",
			    "Most Significant Byte First");
			break;
		case IMHDFINTVBO:
		case IMHDFINTIBO:
			ImInfo ("Byte Order (data)",
			    "Least Significant Byte First");
			dim->dim_channelByteOrder = BINLBF;
			break;
		}
		break;

	case IMHDFNTUCHAR:/* unsigned char					*/
		/* Ignore nt[3] = ASCII or bitwise-numeric.		*/
		dim->dim_channelType = UCHAR;
		break;

	case IMHDFNTCHAR:	/* char						*/
		/* Ignore nt[3] = ASCII or bitwise-numeric.		*/
		dim->dim_channelType = CHAR;
		break;

	case IMHDFNTFLOAT:/* float					*/
		dim->dim_channelType = FLOAT;
	case IMHDFNTDOUBLE:/* double					*/
		dim->dim_channelType = (nt[1] == IMHDFNTFLOAT) ? FLOAT : DOUBLE ;
		switch ( nt[3] )
		{
		default:
		case IMHDFFLOATPC:
		case IMHDFFLOATIEEE:
			dim->dim_channelFloatFormat = BINIEEE;
			break;
		case IMHDFFLOATVAX:
			dim->dim_channelFloatFormat = BINVAX;
			break;
		case IMHDFFLOATCRAY:
			dim->dim_channelFloatFormat = BINCRAYMP;
			break;
		}
		break;

	default:
		sprintf( message, "Unknown NT type code '%d'", nt[1] );
		ImErrorFatal( message, -1, IMESYNTAX );
	}


	/*
	 *  Check the compression tag type.
	 *
	 *  As of this version, 3 types of compression are defined:
	 *
	 *	0	none
	 *	RLE	run-length encode
	 *	IMCOMP	special compression
	 *
	 *  The later two have tags, but those tags don't actually point
	 *  to any data, so we don't bother reading them in.  If, someday,
	 *  other compression types exist, those types may have data and
	 *  we'd have to seek and read them in.
	 *
	 *  Note:  in practice, NCSA tools appear to simply skip adding the
	 *  compression tags, since they have no data.  Mentioning their
	 *  tag number is enough.  SDSC tools more closely follow the HDF
	 *  spec and do add the no-data compression tags.
	 */
	switch ( tagComp )
	{
	case 0:		/* No compression used				*/
	case IMHDFTRLE:	/* RLE used					*/
	case IMHDFTIMC:	/* IMCOMP used					*/
		dim->dim_compression = tagComp;
		break;
	default:
		sprintf( message, "Unknown image compression tag '%d'",
			tagComp );
		ImErrorFatal( message, -1, IMESYNTAX );
	}

	*pDim = dim;
	return ( 0 );
}





/*
 *  FUNCTION
 *	imHdfCltRead	-  read in a CLT
 *
 *  DESCRIPTION
 *	HDF files support lots of variations on the traditional CLT.
 *	Fortunately, virtually all HDF files use the same small subset of
 *	the possibilities.  We restrict ourselves to this same subset by
 *	ruling out the following possibilities:
 *
 *	2D CLT's
 *		CLT's are expected to have a width that is the number of
 *		colors in the CLT, and a height of 1.  Height values other
 *		than 1 are rejected.
 *
 *	Non-RGB CLT's (CLT's that aren't 3 8-bit chars)
 *		A channel (red, green, whatever) has a type, a size, a
 *		byte order and/or a floating point format.  A number of
 *		these channels together form a pixel value.
 *
 *		HDF allows one to describe obscure CLT's that have, say,
 *		10 channels, each one a 3 byte IEEE (no such thing) floating
 *		point number.  Woa!  This is too much.  We restrict ourselves
 *		to the traditional 3 channel, 1 char each CLT.
 *
 *	Compressed CLT's
 *		The LD tag can give a compression mode.  However, there are
 *		no compression modes currently defined for CLT's.  Its
 *		unclear why one would bother.
 */

static int				/* Returns status		*/
#ifdef __STDC__
imHdfCltRead( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImClt **pClt )
#else
imHdfCltRead( ioType, fd, fp, pRIG, pClt )
	int      ioType;		/* I/O flags			*/
	int      fd;			/* Input file descriptor	*/
	FILE    *fp;			/* Input file pointer		*/
	imHdfRIG *pRIG;			/* Image group description	*/
	ImClt  **pClt;			/* Returned CLT			*/
#endif
{
	int n;			/* Number of CLT entries	*/
	int i;			/* Counter			*/
	ImClt   *clt;			/* New clt			*/
	ImCltPtr pColor;		/* CLT entry "pointer"		*/
	char     message[1024];		/* Error message buffer		*/
	imHdfDim *pDim;			/* CLT dimension information	*/
	unsigned char   *buffer;		/* Temp CLT buffer		*/


	/*
	 *  Check for strange CLT specifications.
	 */
	pDim = pRIG->rig_cltDim;
	if ( pDim->dim_height != 1 )
	{
		sprintf( message, "Unsupported CLT height '%d' (should be 1)",
			pDim->dim_height );
		ImErrorFatal( message, -1, IMESYNTAX );
	}

	if ( pDim->dim_channelType != UCHAR || pDim->dim_channelSize != 1 ||
	     pDim->dim_pixelSize   != 3 )
		ImErrorFatal( "Non-RGB (24-bit) CLT's not supported",
			-1, IMESYNTAX );
	if ( pDim->dim_compression != 0 )
	{
		sprintf( message, "Unsupported CLT compression scheme '%d' (should be 0)",
			pDim->dim_compression );
		ImErrorFatal( message, -1, IMESYNTAX );
	}

	/*
	 *  Allocate a new CLT.
	 */
	n = pDim->dim_width;
	if ( (clt = ImCltAlloc( n )) == IMCLTNULL )
		ImErrorFatal( ImQError( ), -1, ImErrNo );

	sprintf (message, "%d Entries",pDim->dim_width);
	ImInfo ("Color Table",message);

	/*
	 *  Read it in, based on the interleave mode.
	 */
	Seek( imHdfDDQDataOffset( pRIG->rig_cltDD ) );
	switch ( pDim->dim_interleave )
	{
	case 0:	/* RGB, RGB, RGB, ...				*/
		ImMalloc( buffer, unsigned char *, sizeof( unsigned char ) * n * 3 );
		pColor = ImCltQFirst( clt );
		Read( buffer, UCHAR, 1, n * 3 );
		n *= 3;
		for ( i = 0; i < n; i+=3 )
		{
			ImCltSRed(   pColor, buffer[i] );
			ImCltSGreen( pColor, buffer[i+1] );
			ImCltSBlue(  pColor, buffer[i+2] );
			ImCltSInc( clt, pColor );
		}
		free( (char *)buffer );
		break;

	case 1:	/* RRRR..., GGGG..., BBBB...			*/
		ImMalloc( buffer, unsigned char *, sizeof( unsigned char ) * n );
		pColor = ImCltQFirst( clt );
		Read( buffer, UCHAR, 1, n );
		for ( i = 0; i < n; i++ )
		{
			ImCltSRed( pColor, buffer[i] );
			ImCltSInc( clt, pColor );
		}

		pColor = ImCltQFirst( clt );
		Read( buffer, UCHAR, 1, n );
		for ( i = 0; i < n; i++ )
		{
			ImCltSGreen( pColor, buffer[i] );
			ImCltSInc( clt, pColor );
		}

		pColor = ImCltQFirst( clt );
		Read( buffer, UCHAR, 1, n );
		for ( i = 0; i < n; i++ )
		{
			ImCltSBlue( pColor, buffer[i] );
			ImCltSInc( clt, pColor );
		}
		free( (char *)buffer );
		break;

	default:
		sprintf( message, "Unsupported CLT interleave scheme '%d'",
			pDim->dim_interleave );
		ImErrorFatal( message, -1, IMESYNTAX );
	}
	*pClt = clt;
	return ( 0 );
}





/*
 *  FUNCTION
 *	imHdfVfbRead	-  read in a VFB
 *
 *  DESCRIPTION
 *	HDF files represent images using one of three "interleave" methods:
 *
 *		#0:	RGB per pixel per scanline.
 *		#1:	Red scanline, then Green, then Blue.
 *		#2:	Red plane, then Green, then Blue.
 *
 *	Each of these three methods may be optionally compressed:
 *
 *		None:	Values are not compressed.
 *
 *		RLE:	Bytewise run-length encoded:
 *
 *				<count> <value> ...
 *
 *			if <count> < 0,  repeate <value> -<count> times
 *			else,            include next <count> values as is
 *
 *			Note:  worst case encoding:  ABBABBABB... where A and
 *			B are different <value>'s.  Encoded result is:
 *			1A-2BB1A-2BB1A-2BB..., giving a 33% expansion.
 *
 *		IMCOMP:	Color sampling.
 *
 *	HDF allows one to describe bizarre image storage configurations that
 *	have, for instance, 7 channels per pixel, each represented by a
 *	13-byte VAX float (no such thing) in the CIE XYZ color space (but
 *	with 7 channels?).  Yeah, right.  We restrict ourselves to the more
 *	traditional configurations:
 *
 *		1 8-bit channel:	8-bit color index + CLT
 *		1 16-bit channel:	16-bit color index + CLT
 *		1 24-bit channel:	24-bit color index + CLT
 *		1 32-bit channel:	32-bit color index + CLT
 *
 *		3 8-bit channels:	24-bit RGB color
 *
 *	All other variations cause errors.
 *
 *	Note that for 16-, 24-, and 32-bit index configurations we do not
 *	support encoding.  HDF defines their RLE encoding as being byte-wise,
 *	even on multi-byte integer values.  This is nonsense.  Encoding would
 *	only do anything if each byte in the integer had the same bit-pattern!
 *	Very dumb.  It also makes it impossible to hide byte order stuff
 *	beneath the encoding scheme (and in the Binary I/O package).
 *
 *	HDF supports a variety of color formats for pixel storage, including
 *	RGB, CIE, HSI, HSV, and spectral samples.  We only support RGB and
 *	pseudo-color (VALUE).
 */

static int				/* Returns status		*/
#ifdef __STDC__
imHdfVfbRead( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb )
#else
imHdfVfbRead( ioType, fd, fp, pRIG, pVfb )
	int      ioType;		/* I/O flags			*/
	int      fd;			/* Input file descriptor	*/
	FILE    *fp;			/* Input file pointer		*/
	imHdfRIG *pRIG;			/* Image group description	*/
	ImVfb  **pVfb;			/* Returned VFB			*/
#endif
{
	imHdfDim *pDim;			/* Image dimension information	*/
	char     message[1024];		/* Error message buffer		*/


	/*
	 *  Check for unsupported image specifications and vector off to
	 *  the appropriate read routine.
	 */
	pDim = pRIG->rig_imageDim;
	if ( pRIG->rig_ccnGiven )
		ImErrorInfo( "Color correction specifications ignored",
			-1, IMESYNTAX );

	/*
	 *  NCSA tools fail to add the color format tag, so we have to assume
	 *  it is either a value (for 8- and 16-bit images) or an RGB color (for
	 *  24-bit images).  SDSC tools always add the color format tag.
	 */
	if ( pRIG->rig_colorFormat != IMHDFCRGB &&
		pRIG->rig_colorFormat != IMHDFCVALUE )
		ImErrorFatal( "Non-RGB color formats not supported",
			-1, IMESYNTAX );

	/*
	 *  Check for illegal combinations.
	 */
	if ( pDim->dim_pixelSize == 1 )
	{
		/* 1 channel.  We support 1, 2, 3 and 4 byte channels.	*/
		if ( pDim->dim_channelSize < 1 || pDim->dim_channelSize > 4 )
		{
			sprintf( message, "Unsupported channel size:  '%d' bytes\n",
				pDim->dim_channelSize );
			ImErrorFatal( message, -1, IMESYNTAX );
		}
		if ( pDim->dim_channelSize != 1 && pDim->dim_compression != 0 )
			ImErrorFatal( "RLE image compression not supported on channels larger than 1 byte",
				-1, IMESYNTAX );
		if ( pDim->dim_interleave != 0 )
		{
			sprintf( message, "Interleave '%d' meaningless on single-channel images",
				pDim->dim_interleave );
			ImErrorInfo( message, -1, IMESYNTAX );
		}
	}
	else if ( pDim->dim_pixelSize != 3 )
	{
		sprintf( message, "Unsupported number of channels per pixel:  '%d'",
			pDim->dim_pixelSize );
		ImErrorFatal( message, -1, IMESYNTAX );
	}

	if ( pDim->dim_channelType == FLOAT || pDim->dim_channelType == DOUBLE )
		ImErrorFatal( "Floating point channels not supported",
			-1, IMESYNTAX );

	if ( pDim->dim_compression != 0 && pDim->dim_compression != IMHDFTRLE )
	{
		sprintf( message, "Unsupported image compression scheme '%d'",
			pDim->dim_compression );
		ImErrorFatal( message, -1, IMESYNTAX );
	}

	if ( pDim->dim_interleave < 0 || pDim->dim_interleave > 3 )
	{
		sprintf( message, "Unknown interleave scheme '%d'",
			pDim->dim_interleave );
		ImErrorFatal( message, -1, IMESYNTAX );
	}


	sprintf (message, "%d x %d",pDim->dim_width, pDim->dim_height);
	ImInfo("Resolution",message);

	/*
	 *  Call the appropriate VFB read routine.
	 */
	if ( pDim->dim_pixelSize == 1 )
	{
		switch ( pDim->dim_channelSize )
		{
		case 1:	/* 8-bit, single channel per pixel image.	*/
			ImInfo ("Type","8-bit Color Indexed");
			switch ( pDim->dim_compression )
			{
			case 0:	/* Uncompressed				*/
				ImInfo ("Compression Type","None");
				return ( imHdfVfbRead8( ioType, fd, fp,
					pRIG, pVfb ) );
			case IMHDFTRLE:/* Run-length encoded		*/
				ImInfo ("Compression Type","Run Length Encoded (RLE)");
				return ( imHdfVfbReadRLE8( ioType, fd, fp,
					pRIG, pVfb ) );
			}
		case 2:	/* 16-bit, single channel per pixel image.	*/
		case 3:	/* 24-bit, single channel per pixel image.	*/
		case 4:	/* 32-bit, single channel per pixel image.	*/
			/* Uncompressed					*/
			ImInfo ("Type","32-bit Color Indexed");
			ImInfo ("Compression Type","None");
			return ( imHdfVfbRead32( ioType, fd, fp,
				pRIG, pVfb ) );

		}
	}

	/* RGB image							*/
	ImInfo ("Type","24-bit RGB");
	switch ( pDim->dim_compression )
	{
	case 0:	/* Uncompressed						*/
		ImInfo ("Compression Type","None");
		switch ( pDim->dim_interleave )
		{
		case 0:	/* Uninterleaved				*/
			ImInfo ("Interleave Type","None (Non-interleaved)");
			return ( imHdfVfbReadRGB( ioType, fd, fp,
				pRIG, pVfb ) );
		case 1:	/* Scanline interleaved				*/
			ImInfo ("Interleave Type","Scanline");
			return ( imHdfVfbReadRGBLine( ioType, fd, fp,
				pRIG, pVfb ));
		case 2:	/* Plane interleaved				*/
			ImInfo ("Interleave Type","Plane");
			return ( imHdfVfbReadRGBPlane( ioType, fd, fp,
				pRIG, pVfb));
		}

	case IMHDFTRLE:/* Run-length encoded				*/
		ImInfo ("Compression Type","Run Length Encoded (RLE)");
		switch ( pDim->dim_interleave )
		{
		case 0:	/* Uninterleaved				*/
			ImInfo ("Interleave Type","None (Non-inteleaved)");
			return ( imHdfVfbReadRLERGB( ioType, fd, fp,
				pRIG, pVfb ) );
		case 1:	/* Scanline interleaved				*/
			ImInfo ("Interleave Type","Scanline");
			return ( imHdfVfbReadRLERGBLine( ioType, fd, fp,
				pRIG, pVfb ));
		case 2:	/* Plane interleaved				*/
			ImInfo ("Interleave Type","Plane");
			return ( imHdfVfbReadRLERGBPlane( ioType, fd, fp,
				pRIG, pVfb));
		}
	}
	/*NOT REACHED*/
	return 0;
}





/*
 *  FUNCTION
 *	imHdfVfbRead8		-  read 8-bit uncomp. VFB
 *	imHdfVfbReadRLE8	-  read 8-bit RLE comp. VFB
 *
 *	imHdfVfbRead32		-  read 32-bit uncomp. VFB
 *
 *	imHdfVfbReadRGB		-  read 24-bit uncomp. uninterleaved VFB
 *	imHdfVfbReadRGBLine	-  read 24-bit uncomp. line interleaved VFB
 *	imHdfVfbReadRGBPlane	-  read 24-bit uncomp. plane interleaved VFB
 *
 *	imHdfVfbReadRLERGB	-  read 24-bit RLE comp. uninterleaved VFB
 *	imHdfVfbReadRLERGBLine	-  read 24-bit RLE comp. line interleaved VFB
 *	imHdfVfbReadRLERGBPlane	-  read 24-bit RLE comp. plane interleaved VFB
 *
 *  DESCRIPTION
 *	Each of these routines allocate a new VFB, then read in the image
 *	data from the file.  If RLE compression is in use, runs are decoded.
 */

static int				/* Returns status		*/
#ifdef __STDC__
imHdfVfbRead8( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb )
#else
imHdfVfbRead8( ioType, fd, fp, pRIG, pVfb )
	int      ioType;		/* I/O flags			*/
	int      fd;			/* Input file descriptor	*/
	FILE    *fp;			/* Input file pointer		*/
	imHdfRIG *pRIG;			/* Image group description	*/
	ImVfb  **pVfb;			/* Returned VFB			*/
#endif
{
	imHdfDim *pDim;			/* Image dimension information	*/
	ImVfb   *vfb;			/* New VFB			*/
	ImVfbPtr pPixel;		/* Pixel pointer		*/
	unsigned char   *buffer;		/* Input buffer			*/
	unsigned char   *pBuffer;		/* Pointer into input buffer	*/
	int      i, j;			/* Counters			*/

	/*
	 *  Allocate a new 8-bit index VFB.
	 */
	pDim = pRIG->rig_imageDim;
	if ( (vfb = ImVfbAlloc( pDim->dim_width, pDim->dim_height, IMVFBINDEX8))
		== IMVFBNULL )
		ImErrorFatal( ImQError( ), -1, ImErrNo );


	/*
	 *  Allocate a buffer that is big enough for one scanline.  Read in
	 *  the image one scanline at a time.
	 */
	ImMalloc( buffer, unsigned char *, sizeof( unsigned char ) * pDim->dim_width );
	pPixel = ImVfbQFirst( vfb );
	Seek( imHdfDDQDataOffset( pRIG->rig_imageDD ) );

	for ( i = 0; i < pDim->dim_height; i++ )
	{
		Read( buffer, UCHAR, 1, pDim->dim_width );
		pBuffer = buffer;
		for ( j = 0; j < pDim->dim_width; j++ )
		{
			ImVfbSIndex8( vfb, pPixel, *pBuffer++ );
			ImVfbSInc( vfb, pPixel );
		}
	}
	free( (char *)buffer );
	*pVfb = vfb;
	return ( 0 );
}

static int				/* Returns status		*/
#ifdef __STDC__
imHdfVfbReadRLE8( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb )
#else
imHdfVfbReadRLE8( ioType, fd, fp, pRIG, pVfb )
	int      ioType;		/* I/O flags			*/
	int      fd;			/* Input file descriptor	*/
	FILE    *fp;			/* Input file pointer		*/
	imHdfRIG *pRIG;			/* Image group description	*/
	ImVfb  **pVfb;			/* Returned VFB			*/
#endif
{
	imHdfDim *pDim;			/* Image dimension information	*/
	ImVfb   *vfb;			/* New VFB			*/
	ImVfbPtr pPixel;		/* Pixel pointer		*/
	unsigned char   *buffer;		/* Input buffer			*/
	unsigned char   *pBuffer;		/* Pointer into input buffer	*/
	unsigned char   *pBufferEnd;		/* Pointer to end of buffer	*/
	unsigned char    count;			/* Run length count		*/


	/*
	 *  Allocate a new 8-bit index VFB.
	 */
	pDim = pRIG->rig_imageDim;
	if ( (vfb = ImVfbAlloc( pDim->dim_width, pDim->dim_height, IMVFBINDEX8))
		== IMVFBNULL )
		ImErrorFatal( ImQError( ), -1, ImErrNo );


	/*
	 *  Allocate a buffer big enough for the whole image, read it in,
	 *  then process the whole thing.  This takes more memory, but we
	 *  have no othe rway of alloating space sufficient for just one
	 *  scanline, then reading in only that scanline.
	 */
	Seek( imHdfDDQDataOffset( pRIG->rig_imageDD ) );
	pPixel = ImVfbQFirst( vfb );
	ImMalloc( buffer, unsigned char *, sizeof( unsigned char ) * imHdfDDQDataLength( pRIG->rig_imageDD ) );
	pBuffer = buffer;
	pBufferEnd = buffer + imHdfDDQDataLength( pRIG->rig_imageDD );
	Read( buffer, UCHAR, 1, imHdfDDQDataLength( pRIG->rig_imageDD ) );

	while ( pBuffer < pBufferEnd )
	{
		count = *pBuffer++;
		if ( !(count & 0x80) )
		{
			/* Take next 'count' bytes as literal.		*/
			while ( count-- )
			{
				ImVfbSIndex8( vfb, pPixel, *pBuffer++ );
				ImVfbSInc( vfb, pPixel );
			}
			continue;
		}

		/* Repeat next byte 'count' times.			*/
		count &= 0x7F;
		while ( count-- )
		{
			ImVfbSIndex8( vfb, pPixel, *pBuffer );
			ImVfbSInc( vfb, pPixel );
		}
		pBuffer++;
	}
	free( (char *)buffer );
	*pVfb = vfb;
	return ( 0 );
}

static int				/* Returns status		*/
#ifdef __STDC__
imHdfVfbRead32( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb )
#else
imHdfVfbRead32( ioType, fd, fp, pRIG, pVfb )
	int      ioType;		/* I/O flags			*/
	int      fd;			/* Input file descriptor	*/
	FILE    *fp;			/* Input file pointer		*/
	imHdfRIG *pRIG;			/* Image group description	*/
	ImVfb  **pVfb;			/* Returned VFB			*/
#endif
{
	imHdfDim *pDim;			/* Image dimension information	*/
	ImVfb   *vfb;			/* New VFB			*/
	ImVfbPtr pPixel;		/* Pixel pointer		*/
	sdsc_uint32  *buffer;		/* Input buffer			*/
	sdsc_uint32  *pBuffer;		/* Pointer into input buffer	*/
	int      i, j;			/* Counters			*/


	/*
	 *  Allocate a new 16-bit index VFB.
	 */
	pDim = pRIG->rig_imageDim;
	if ( (vfb = ImVfbAlloc( pDim->dim_width, pDim->dim_height,IMVFBINDEX16))
		== IMVFBNULL )
		ImErrorFatal( ImQError( ), -1, ImErrNo );


	/*
	 *  Allocate a buffer that is big enough for one scanline.  Read in
	 *  the image one scanline at a time.
	 */
	ImMalloc( buffer, sdsc_uint32 *, sizeof( sdsc_uint32 ) * pDim->dim_width );
	pPixel = ImVfbQFirst( vfb );
	Seek( imHdfDDQDataOffset( pRIG->rig_imageDD ) );

	for ( i = 0; i < pDim->dim_height; i++ )
	{
		Read( buffer, UINT32, pDim->dim_channelSize, pDim->dim_width );
		pBuffer = buffer;
		for ( j = 0; j < pDim->dim_width; j++ )
		{
			/* Truncate upper bits.				*/
			ImVfbSIndex16( vfb, pPixel, *pBuffer++ );
			ImVfbSInc( vfb, pPixel );
		}
	}
	free( (char *)buffer );
	*pVfb = vfb;
	return ( 0 );
}

static int				/* Returns status		*/
#ifdef __STDC__
imHdfVfbReadRGB( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb )
#else
imHdfVfbReadRGB( ioType, fd, fp, pRIG, pVfb )
	int      ioType;		/* I/O flags			*/
	int      fd;			/* Input file descriptor	*/
	FILE    *fp;			/* Input file pointer		*/
	imHdfRIG *pRIG;			/* Image group description	*/
	ImVfb  **pVfb;			/* Returned VFB			*/
#endif
{
	imHdfDim *pDim;			/* Image dimension information	*/
	ImVfb   *vfb;			/* New VFB			*/
	ImVfbPtr pPixel;		/* Pixel pointer		*/
	unsigned char   *buffer;		/* Input buffer			*/
	unsigned char   *pBuffer;		/* Pointer into input buffer	*/
	int      i, j;			/* Counters			*/


	/*
	 *  Allocate a new 24-bit index VFB.
	 */
	pDim = pRIG->rig_imageDim;
	if ( (vfb = ImVfbAlloc( pDim->dim_width, pDim->dim_height, IMVFBRGB ))
		== IMVFBNULL )
		ImErrorFatal( ImQError( ), -1, ImErrNo );


	/*
	 *  Allocate a buffer that is big enough for one scanline.  Read in
	 *  the image one scanline at a time.
	 */
	pPixel = ImVfbQFirst( vfb );
	Seek( imHdfDDQDataOffset( pRIG->rig_imageDD ) );
	ImMalloc( buffer, unsigned char *, sizeof( unsigned char )* pDim->dim_width*3 );

	for ( i = 0; i < pDim->dim_height; i++ )
	{
		Read( buffer, UCHAR, 1, pDim->dim_width * 3 );
		pBuffer = buffer;
		for ( j = 0; j < pDim->dim_width; j++ )
		{
			ImVfbSRed(   vfb, pPixel, *pBuffer++ );
			ImVfbSGreen( vfb, pPixel, *pBuffer++ );
			ImVfbSBlue(  vfb, pPixel, *pBuffer++ );
			ImVfbSInc(   vfb, pPixel );
		}
	}
	free( (char *)buffer );
	*pVfb = vfb;
	return ( 0 );
}

static int				/* Returns status		*/
#ifdef __STDC__
imHdfVfbReadRGBLine( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb )
#else
imHdfVfbReadRGBLine( ioType, fd, fp, pRIG, pVfb )
	int      ioType;		/* I/O flags			*/
	int      fd;			/* Input file descriptor	*/
	FILE    *fp;			/* Input file pointer		*/
	imHdfRIG *pRIG;			/* Image group description	*/
	ImVfb  **pVfb;			/* Returned VFB			*/
#endif
{
	imHdfDim *pDim;			/* Image dimension information	*/
	ImVfb   *vfb;			/* New VFB			*/
	ImVfbPtr pPixel;		/* Pixel pointer		*/
	unsigned char   *buffer;		/* Input buffer			*/
	unsigned char   *pBuffer1;		/* Pointer into input buffer	*/
	unsigned char   *pBuffer2;		/* Pointer into input buffer	*/
	unsigned char   *pBuffer3;		/* Pointer into input buffer	*/
	int      i, j;			/* Counters			*/


	/*
	 *  Allocate a new 24-bit index VFB.
	 */
	pDim = pRIG->rig_imageDim;
	if ( (vfb = ImVfbAlloc( pDim->dim_width, pDim->dim_height, IMVFBRGB ))
		== IMVFBNULL )
		ImErrorFatal( ImQError( ), -1, ImErrNo );


	/*
	 *  Allocate a buffer that is big enough for one scanline.  Read in
	 *  the image one scanline at a time.
	 */
	pPixel = ImVfbQFirst( vfb );
	Seek( imHdfDDQDataOffset( pRIG->rig_imageDD ) );
	ImMalloc( buffer, unsigned char *, sizeof( unsigned char )* pDim->dim_width*3 );

	for ( i = 0; i < pDim->dim_height; i++ )
	{
		Read( buffer, UCHAR, 1, pDim->dim_width * 3 );
		pBuffer1 = buffer;
		pBuffer2 = buffer + pDim->dim_width;
		pBuffer3 = pBuffer2 + pDim->dim_width;
		for ( j = 0; j < pDim->dim_width; j++ )
		{
			ImVfbSRed(   vfb, pPixel, *pBuffer1++ );
			ImVfbSGreen( vfb, pPixel, *pBuffer2++ );
			ImVfbSBlue(  vfb, pPixel, *pBuffer3++ );
			ImVfbSInc(   vfb, pPixel );
		}
	}
	free( (char *)buffer );
	*pVfb = vfb;
	return ( 0 );
}

static int				/* Returns status		*/
#ifdef __STDC__
imHdfVfbReadRGBPlane( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb )
#else
imHdfVfbReadRGBPlane( ioType, fd, fp, pRIG, pVfb )
	int      ioType;		/* I/O flags			*/
	int      fd;			/* Input file descriptor	*/
	FILE    *fp;			/* Input file pointer		*/
	imHdfRIG *pRIG;			/* Image group description	*/
	ImVfb  **pVfb;			/* Returned VFB			*/
#endif
{
	imHdfDim *pDim;			/* Image dimension information	*/
	ImVfb   *vfb;			/* New VFB			*/
	ImVfbPtr pPixel;		/* Pixel pointer		*/
	unsigned char   *buffer;		/* Input buffer			*/
	unsigned char   *pBuffer;		/* Pointer into input buffer	*/
	int      i, j;			/* Counters			*/


	/*
	 *  Allocate a new 24-bit index VFB.
	 */
	pDim = pRIG->rig_imageDim;
	if ( (vfb = ImVfbAlloc( pDim->dim_width, pDim->dim_height, IMVFBRGB ))
		== IMVFBNULL )
		ImErrorFatal( ImQError( ), -1, ImErrNo );


	/*
	 *  Allocate a buffer that is big enough for one scanline.  Read in
	 *  the image one scanline at a time.
	 */
	Seek( imHdfDDQDataOffset( pRIG->rig_imageDD ) );
	ImMalloc( buffer, unsigned char *, sizeof( unsigned char ) * pDim->dim_width );

	pPixel = ImVfbQFirst( vfb );
	for ( i = 0; i < pDim->dim_height; i++ )
	{
		Read( buffer, UCHAR, 1, pDim->dim_width );
		pBuffer = buffer;
		for ( j = 0; j < pDim->dim_width; j++ )
		{
			ImVfbSRed( vfb, pPixel, *pBuffer++ );
			ImVfbSInc( vfb, pPixel );
		}
	}
	pPixel = ImVfbQFirst( vfb );
	for ( i = 0; i < pDim->dim_height; i++ )
	{
		Read( buffer, UCHAR, 1, pDim->dim_width );
		pBuffer = buffer;
		for ( j = 0; j < pDim->dim_width; j++ )
		{
			ImVfbSGreen( vfb, pPixel, *pBuffer++ );
			ImVfbSInc( vfb, pPixel );
		}
	}
	pPixel = ImVfbQFirst( vfb );
	for ( i = 0; i < pDim->dim_height; i++ )
	{
		Read( buffer, UCHAR, 1, pDim->dim_width );
		pBuffer = buffer;
		for ( j = 0; j < pDim->dim_width; j++ )
		{
			ImVfbSBlue( vfb, pPixel, *pBuffer++ );
			ImVfbSInc( vfb, pPixel );
		}
	}
	free( (char *)buffer );
	*pVfb = vfb;
	return ( 0 );
}

static int				/* Returns status		*/
#ifdef __STDC__
imHdfVfbReadRLERGB( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb )
#else
imHdfVfbReadRLERGB( ioType, fd, fp, pRIG, pVfb )
	int      ioType;		/* I/O flags			*/
	int      fd;			/* Input file descriptor	*/
	FILE    *fp;			/* Input file pointer		*/
	imHdfRIG *pRIG;			/* Image group description	*/
	ImVfb  **pVfb;			/* Returned VFB			*/
#endif
{
	imHdfDim *pDim;			/* Image dimension information	*/
	ImVfb   *vfb;			/* New VFB			*/
	ImVfbPtr pPixel;		/* Pixel pointer		*/
	unsigned char   *buffer;		/* Input buffer			*/
	unsigned char   *pBufferEnd;		/* Pointer to end of buffer	*/
	unsigned char   *pBuffer;		/* Pointer into input buffer	*/
	unsigned char    count;			/* Run length count		*/
	int      onRGB;			/* Which channel are we on?	*/


	/*
	 *  Allocate a new 24-bit index VFB.
	 */
	pDim = pRIG->rig_imageDim;
	if ( (vfb = ImVfbAlloc( pDim->dim_width, pDim->dim_height, IMVFBRGB ))
		== IMVFBNULL )
		ImErrorFatal( ImQError( ), -1, ImErrNo );


	/*
	 *  Allocate a buffer big enough for the whole image, read it in,
	 *  then process the whole thing.  This takes more memory, but we
	 *  have no othe rway of alloating space sufficient for just one
	 *  scanline, then reading in only that scanline.
	 */
	Seek( imHdfDDQDataOffset( pRIG->rig_imageDD ) );
	pPixel = ImVfbQFirst( vfb );
	ImMalloc( buffer, unsigned char *, sizeof( unsigned char ) *
		imHdfDDQDataLength( pRIG->rig_imageDD ) );
	pBuffer = buffer;
	pBufferEnd = buffer + imHdfDDQDataLength( pRIG->rig_imageDD );
	Read( buffer, UCHAR, 1, imHdfDDQDataLength( pRIG->rig_imageDD ) );

	onRGB = RED;
	while ( pBuffer < pBufferEnd )
	{
		count = *pBuffer++;
		if ( !(count & 0x80) )
		{
			/* Take next 'count' bytes as literal.		*/
			while ( count-- )
			{
				switch ( onRGB )
				{
				case RED:
					ImVfbSRed( vfb, pPixel, *pBuffer++ );
					onRGB = GREEN;
					continue;
				case GREEN:
					ImVfbSGreen( vfb, pPixel, *pBuffer++ );
					onRGB = BLUE;
					continue;
				case BLUE:
					ImVfbSBlue( vfb, pPixel, *pBuffer++ );
					ImVfbSInc( vfb, pPixel );
					onRGB = RED;
					continue;
				}
			}
			continue;
		}

		/* Repeat next byte 'count' times.			*/
		count &= 0x7F;
		while ( count-- )
		{
			switch ( onRGB )
			{
			case RED:
				ImVfbSRed( vfb, pPixel, *pBuffer );
				onRGB = GREEN;
				continue;
			case GREEN:
				ImVfbSGreen( vfb, pPixel, *pBuffer );
				onRGB = BLUE;
				continue;
			case BLUE:
				ImVfbSBlue( vfb, pPixel, *pBuffer );
				ImVfbSInc( vfb, pPixel );
				onRGB = RED;
				continue;
			}
		}
		pBuffer++;
	}
	free( (char *)buffer );
	*pVfb = vfb;
	return ( 0 );
}

static int				/* Returns status		*/
#ifdef __STDC__
imHdfVfbReadRLERGBLine( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb )
#else
imHdfVfbReadRLERGBLine( ioType, fd, fp, pRIG, pVfb )
	int      ioType;		/* I/O flags			*/
	int      fd;			/* Input file descriptor	*/
	FILE    *fp;			/* Input file pointer		*/
	imHdfRIG *pRIG;			/* Image group description	*/
	ImVfb  **pVfb;			/* Returned VFB			*/
#endif
{
	imHdfDim *pDim;			/* Image dimension information	*/
	ImVfb   *vfb;			/* New VFB			*/
	ImVfbPtr pPixel;		/* Pixel pointer		*/
	ImVfbPtr pPixelStart;		/* Start of scanline		*/
	unsigned char   *buffer;		/* Input buffer			*/
	unsigned char   *pBufferEnd;		/* Pointer to end of buffer	*/
	unsigned char   *pBuffer;		/* Pointer into input buffer	*/
	unsigned char    count;			/* Run length count		*/
	int      onRGB;			/* Which channel are we on?	*/
	int      i, j;			/* Counters			*/


	/*
	 *  Allocate a new 24-bit index VFB.
	 */
	pDim = pRIG->rig_imageDim;
	if ( (vfb = ImVfbAlloc( pDim->dim_width, pDim->dim_height, IMVFBRGB ))
		== IMVFBNULL )
		ImErrorFatal( ImQError( ), -1, ImErrNo );


	/*
	 *  Allocate a buffer big enough for the whole image, read it in,
	 *  then process the whole thing.  This takes more memory, but we
	 *  have no othe rway of alloating space sufficient for just one
	 *  scanline, then reading in only that scanline.
	 */
	Seek( imHdfDDQDataOffset( pRIG->rig_imageDD ) );
	pPixel = ImVfbQFirst( vfb );
	ImMalloc( buffer, unsigned char *, sizeof( unsigned char ) *
		imHdfDDQDataLength( pRIG->rig_imageDD ) );
	pBuffer = buffer;
	pBufferEnd = buffer + imHdfDDQDataLength( pRIG->rig_imageDD );
	Read( buffer, UCHAR, 1, imHdfDDQDataLength( pRIG->rig_imageDD ) );

	/*
	 *  This algorithm is made slower by the possibility that a run
	 *  from one scanline extends into the next one (ie, a run at the
	 *  end of the RED channel flows into the GREEN channel).  SDSC
	 *  tools end scanline interleaved runs at the end of a channel's
	 *  scanline.  The HDF spec, however, makes no such guarantee.
	 */
	onRGB = RED;
	i     = 0;
	pPixelStart = pPixel;
	while ( pBuffer < pBufferEnd )
	{
		count = *pBuffer++;
		if ( !(count & 0x80) )
		{
			/* Take next 'count' bytes as literal.		*/
			while ( count-- )
			{
				switch ( onRGB )
				{
				case RED:
					ImVfbSRed( vfb, pPixel, *pBuffer++ );
					ImVfbSInc( vfb, pPixel );
					if ( ++i >= pDim->dim_width )
					{
						onRGB = GREEN;
						i = 0;
						pPixel = pPixelStart;
					}
					continue;
				case GREEN:
					ImVfbSGreen( vfb, pPixel, *pBuffer++ );
					ImVfbSInc( vfb, pPixel );
					if ( ++i >= pDim->dim_width )
					{
						onRGB = BLUE;
						i = 0;
						pPixel = pPixelStart;
					}
					continue;
				case BLUE:
					ImVfbSBlue( vfb, pPixel, *pBuffer++ );
					ImVfbSInc( vfb, pPixel );
					if ( ++i >= pDim->dim_width )
					{
						onRGB = RED;
						i = 0;
						pPixelStart = pPixel;
					}
					continue;
				}
			}
			continue;
		}

		/* Repeat next byte 'count' times.			*/
		count &= 0x7F;
		while ( count-- )
		{
			switch ( onRGB )
			{
			case RED:
				ImVfbSRed( vfb, pPixel, *pBuffer );
				ImVfbSInc( vfb, pPixel );
				if ( ++i >= pDim->dim_width )
				{
					onRGB = GREEN;
					i = 0;
					pPixel = pPixelStart;
				}
				continue;
			case GREEN:
				ImVfbSGreen( vfb, pPixel, *pBuffer );
				ImVfbSInc( vfb, pPixel );
				if ( ++i >= pDim->dim_width )
				{
					onRGB = BLUE;
					i = 0;
					pPixel = pPixelStart;
				}
				continue;
			case BLUE:
				ImVfbSBlue( vfb, pPixel, *pBuffer );
				ImVfbSInc( vfb, pPixel );
				if ( ++i >= pDim->dim_width )
				{
					onRGB = RED;
					i = 0;
					pPixelStart = pPixel;
				}
				continue;
			}
		}
		pBuffer++;
	}
	free( (char *)buffer );
	*pVfb = vfb;
	return ( 0 );
}

static int				/* Returns status		*/
#ifdef __STDC__
imHdfVfbReadRLERGBPlane( int ioType, int fd, FILE *fp, imHdfRIG *pRIG, ImVfb **pVfb )
#else
imHdfVfbReadRLERGBPlane( ioType, fd, fp, pRIG, pVfb )
	int      ioType;		/* I/O flags			*/
	int      fd;			/* Input file descriptor	*/
	FILE    *fp;			/* Input file pointer		*/
	imHdfRIG *pRIG;			/* Image group description	*/
	ImVfb  **pVfb;			/* Returned VFB			*/
#endif
{
	imHdfDim *pDim;			/* Image dimension information	*/
	ImVfb   *vfb;			/* New VFB			*/
	ImVfbPtr pPixel;		/* Pixel pointer		*/
	unsigned char   *buffer;		/* Input buffer			*/
	unsigned char   *pBufferEnd;		/* Pointer to end of buffer	*/
	unsigned char   *pBuffer;		/* Pointer into input buffer	*/
	unsigned char    count;			/* Run length count		*/
	int      i, j;			/* Counters			*/


	/*
	 *  Allocate a new 24-bit index VFB.
	 */
	pDim = pRIG->rig_imageDim;
	if ( (vfb = ImVfbAlloc( pDim->dim_width, pDim->dim_height, IMVFBRGB ))
		== IMVFBNULL )
		ImErrorFatal( ImQError( ), -1, ImErrNo );


	/*
	 *  Allocate a buffer big enough for the whole image, read it in,
	 *  then process the whole thing.  This takes more memory, but we
	 *  have no othe rway of alloating space sufficient for just one
	 *  scanline, then reading in only that scanline.
	 */
	Seek( imHdfDDQDataOffset( pRIG->rig_imageDD ) );
	ImMalloc( buffer, unsigned char *, sizeof( unsigned char ) *
		imHdfDDQDataLength( pRIG->rig_imageDD ) );
	pBuffer = buffer;
	pBufferEnd = buffer + imHdfDDQDataLength( pRIG->rig_imageDD );
	Read( buffer, UCHAR, 1, imHdfDDQDataLength( pRIG->rig_imageDD ) );

	j = pDim->dim_width * pDim->dim_height;
	i = 0;
	pPixel = ImVfbQFirst( vfb );
	while ( pBuffer < pBufferEnd && i < j )
	{
		count = *pBuffer++;
		if ( !(count & 0x80) )
		{
			/* Take next 'count' bytes as literal.		*/
			i += count;
			while ( count-- )
			{
				ImVfbSRed( vfb, pPixel, *pBuffer++ );
				ImVfbSInc( vfb, pPixel );
			}
			continue;
		}

		/* Repeat next byte 'count' times.			*/
		count &= 0x7F;
		i += count;
		while ( count-- )
		{
			ImVfbSRed( vfb, pPixel, *pBuffer );
			ImVfbSInc( vfb, pPixel );
		}
		pBuffer++;
	}
	i = 0;
	pPixel = ImVfbQFirst( vfb );
	while ( pBuffer < pBufferEnd && i < j )
	{
		count = *pBuffer++;
		if ( !(count & 0x80) )
		{
			/* Take next 'count' bytes as literal.		*/
			i += count;
			while ( count-- )
			{
				ImVfbSGreen( vfb, pPixel, *pBuffer++ );
				ImVfbSInc( vfb, pPixel );
			}
			continue;
		}

		/* Repeat next byte 'count' times.			*/
		count &= 0x7F;
		i += count;
		while ( count-- )
		{
			ImVfbSGreen( vfb, pPixel, *pBuffer );
			ImVfbSInc( vfb, pPixel );
		}
		pBuffer++;
	}
	i = 0;
	pPixel = ImVfbQFirst( vfb );
	while ( pBuffer < pBufferEnd && i < j )
	{
		count = *pBuffer++;
		if ( !(count & 0x80) )
		{
			/* Take next 'count' bytes as literal.		*/
			i += count;
			while ( count-- )
			{
				ImVfbSBlue( vfb, pPixel, *pBuffer++ );
				ImVfbSInc( vfb, pPixel );
			}
			continue;
		}

		/* Repeat next byte 'count' times.			*/
		count &= 0x7F;
		i += count;
		while ( count-- )
		{
			ImVfbSBlue( vfb, pPixel, *pBuffer );
			ImVfbSInc( vfb, pPixel );
		}
		pBuffer++;
	}
	free( (char *)buffer );
	*pVfb = vfb;
	return ( 0 );
}
