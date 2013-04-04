/**
 **	$Header: /sdsc/dev/vis/image/imtools/v3.0/libim/src/include/RCS/im.h,v 1.48 1995/06/30 22:13:07 bduggan Exp $
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

/**
 **  FILE
 **	im.h		-  Image library include file
 **
 **  PROJECT
 **	libim		-  SDSC image manipulation library
 **
 **  DESCRIPTION
 **	im.h contains the structs, typedefs, externs, defines, and macros
 **	needed to use the VFB, CLT, and File subpackages of the image
 **	library.
 **
 **  PUBLIC CONTENTS
 **			d =defined constant
 **			f =function
 **			m =defined macro
 **			t =typedef/struct/union
 **			v =variable
 **			? =other
 **
 **	__IMH__		d  file inclusion flag
 **
 **	ImErrNo		v  error number
 **	ImNErr		v  number of error messages
 **	ImErrList	v  error messages
 **	IME...		d  error codes
 **
 **	IMVFBNULL	d  NULL VFB
 **	IMCLTNULL	d  NULL CLT
 **	IMVFBNEW	d  Indicate a new VFB is to be allocated
 **	IMCLTNEW	d  Indicate a new CLT is to be allocated
 **
 **	ImCltPtr	t  pointer to CLT entry
 **	ImClt		t  color lookup table
 **	ImVfbPtr	t  pointer to VFB pixel
 **	ImVfb		t  virtual frame buffer
 **	ImHotSpot	t  image hotspot
 **
 **	ImVfb...	m  set and query VFB attributes
 **	ImClt...	m Set and query CLT attributes
 **	ImHotSpot...	m  set and query HotSpot attributes
 **
 **	IMVFB...	d  fields mask values
 **	IMVFB...	d  raster operations and inout values
 **	IMVFB...	d  flip directions
 **	IMVFB...	d  resolution change algorithms
 **
 **	IMTYPEINDEX	d  Indexed image type
 **	IMTYPERGB	d  RGB image type
 **	IMTYPE2D	d  2D primitives
 **	IMCOMP...	d  Compression options
 **	IMINTER...	d  Interlace options
 **	IMCLT...	d  CLT options
 **	IMALPHA...	d  Alpha plane options
 **
 **	ImFileFormat	t  file format info
 **
 **	ImFileFormatReadMap	t  map of how a format handles reads
 **	ImFileFormatWriteMap	t  map of how a format handles writes
 **
 **	Im...		f  Generic function declarations
 **	ImVfb...	f  VFB function declarations
 **	ImClt...	f  CLT function declarations
 **	ImFile...	f  File I/O function declarations
 **	ImGetFileFormats	f  Get the list of formats
 **
 **	IMFILE...	d  Image file format-specific options
 **
 **	IMERRORFATAL	d  fatal error
 **	IMERRORWARNING	d  warning error
 **	IMERRORINFO	d  information error
 **
 **	IMMAXNAME	d  maximum name size
 **	IMDEFMONOTHRESH	d  default monochrome threshold
 **
 **	IMMAXIMUM	d  computed maximum field value
 **	IMMINIMUM	d  computed minimum field value
 **	IMUNIQUEVAL	d  computed number of unique field values
 **	IMMAXNUMSTATS	d  number of statistics returned by ImVfbStat
 **
 **	ImHistTable	t  histogram information table
 **	IMHIST...	d  histogram table array index names
 **	imhist_...	d  histogram table union field names
 **
 **  PRIVATE CONTENTS
 **	none
 **
 **  HISTORY
 **	$Log: im.h,v $
 **	Revision 1.48  1995/06/30  22:13:07  bduggan
 **	added ImVfbFSDitherToMono non-ansi prototype
 **
 **	Revision 1.47  1995/06/29  00:30:40  bduggan
 **	Added IMVFBPRINTINTFLAOT
 **	oops, i mean FLOAT
 **
 **	Revision 1.46  1995/06/16  07:16:09  bduggan
 **	removed IMFILECOMPZ
 **
 **	Revision 1.45  1995/06/16  06:58:45  bduggan
 **	added imvfbcopychannel
 **
 **	Revision 1.44  1995/06/16  06:47:00  bduggan
 **	Added compression support.
 **	Removed global ImFileFormats variable.
 **	Added ANSI declarations for pointers to functions.
 **
 **	Revision 1.43  1995/05/18  00:09:51  bduggan
 **	Added IMCOMPASCII, ImVfbPrint, ImGetTransparency,
 **	IMFILECOMPZ,
 **	ImVfbSameRGB, ImVfbSameRGBA, (etc.),
 **	IMVFBPRINTFLOAT, IMVFBPRINTINT
 **
 **	Revision 1.42  1995/04/03  22:11:26  bduggan
 **	added support for channel mapping requests,
 **	image quality requests (for jpeg compression),
 **	and took out #ifdef NEWMAGIC
 **
 **	Revision 1.41  1995/01/18  21:54:21  bduggan
 **	Added GROUP macros and FSDitherToMono prototype
 **
 **	Revision 1.40  1994/10/03  16:01:28  nadeau
 **	Updated to ANSI C and C++ compatibility by adding function prototypes.
 **	Minimized use of custom SDSC types (e.g., uchar vs. unsigned char)
 **	Changed all float arguments to double.
 **	Rearranged magic number structures for format handlers.
 **	Added definitions for IMMULTI, IMNOMULTI, IMPIPE, & IMNOPIPE.
 **	Updated copyright message.
 **
 **	Revision 1.39  93/03/18  12:29:28  todd
 **	added  extern ImVfb *ImVfbIndex() to quiet the compiler
 **	
 **	Revision 1.38  93/03/09  10:44:58  nadeau
 **	Corrected non-ANSI-ism of an #endif comment.
 **	
 **	Revision 1.37  92/12/03  02:19:54  nadeau
 **	Updated histogram information and function declarations.
 **	
 **	Revision 1.36  92/11/06  17:00:35  groening
 **	change ImFIleFormats struct.
 **	
 **	Revision 1.35  92/10/06  13:44:42  nadeau
 **	Added function declarations.
 **	
 **	Revision 1.34  92/09/04  18:35:45  groening
 **	Added IMEIMPSHEAR error.
 **	
 **	Revision 1.33  92/08/31  14:49:26  nadeau
 **	Added additional function declarations.  Moved things around
 **	a bit.  Added comments to hotspot declarations.  Added additiona
 **	error codes.  Changed names on some error codes.
 **	
 **	Revision 1.32  92/08/28  15:29:14  vle
 **	Added hotspot support.
 **	
 **	Revision 1.31  92/08/18  13:08:21  groening
 **	added error measges and stuff for imhist.c
 **	plus #defines for operations, etc.
 **	
 **	Revision 1.30  92/06/06  13:36:52  vle
 **	Fixed type problem with macros.
 **	
 **	Revision 1.29  92/04/15  12:23:56  nadeau
 **	Added more extern declarations.
 **	
 **	Revision 1.28  91/10/03  12:56:07  nadeau
 **	Removed out-of-date PBM and FPS flags.
 **	
 **	Revision 1.27  91/10/03  12:54:09  nadeau
 **	Cosmetic changes.  Added read and write map support.
 **	
 **	Revision 1.26  91/01/23  12:50:34  doeringd
 **	Added prefix "IMFPS" to FPS enum list items.
 **	
 **	Revision 1.25  91/01/23  12:04:19  doeringd
 **	Removed unneeded enum name "aperture" to prevent conflict with
 **	int aperture.
 **	
 **	Revision 1.24  91/01/23  11:06:08  nadeau
 **	Removed redundant aperture declaration.
 **	
 **	Revision 1.23  91/01/23  10:44:00  doeringd
 **	Add fps enum definitions for apertures.
 **	
 **	Revision 1.22  91/01/23  10:27:08  nadeau
 **	Added multiVFB flag to ImFileFormats struct.
 **	
 **	Revision 1.21  91/01/20  07:43:55  doeringd
 **	Included imfps.h aperture enum values.
 **	
 **	Revision 1.20  91/01/16  10:30:04  doeringd
 **	Add PBM defines
 **	
 **	Revision 1.19  91/01/16  10:13:27  nadeau
 **	Added declarations for ImVfbIsMono() and ImVfbTo8Mono().
 **	
 **	Revision 1.18  90/09/04  14:03:44  ferrerj
 **	added ImVfbCopyArea to function list
 **	
 **	Revision 1.17  90/09/04  13:57:10  ferrerj
 **	added ImVfbCopyArea() to function list
 **	
 **	Revision 1.17  90/09/04  02:00:00  ferrerj
 **	added ImVfbCopyArea()  to function list
 **	
 **	Revision 1.16  90/07/26  11:45:09  mcleodj
 **	Added Pixar PIC format flag values
 **	
 **	Revision 1.15  90/07/25  16:25:14  nadeau
 **	Changed RAS file format flag values and their names.
 **	
 **	Revision 1.14  90/07/25  13:34:05  todd
 **	added several new IME* errors.
 **	added IMTIFF flag values
 **	
 **	Revision 1.13  90/07/23  13:51:14  nadeau
 **	Fixed Index16 macros to use vfb_i32off instead of vfb_ioff.  Added HDF
 **	write flags.  Removed PIX write flag.
 **	
 **	Revision 1.12  90/07/15  09:30:28  mjb
 **	added ImVfbCut() and ImvfbLightness() to function list
 **	
 **	Revision 1.11  90/07/02  15:48:25  mercurio
 **	Error messages added
 **	
 **	Revision 1.10  90/06/28  20:14:29  mercurio
 **	Added IMENOIMAGE and IMEUNSUPPORTED error codes
 **	
 **	Revision 1.9  90/06/28  15:16:00  nadeau
 **	Removed tag table types and function declarations.  Added #defines to
 **	map from the old tag table function names to the new ones.  Added
 **	declarations and #defines for the error handling code.
 **	
 **	Revision 1.8  90/05/15  13:57:56  todd
 **	Move IMFILEFD & FP PIPE and FILE macros to new iminternal.h file
 **	
 **	Revision 1.7  90/05/11  14:26:34  nadeau
 **	Removed old IMFILE format defines.
 **	
 **	Revision 1.6  90/05/11  10:02:47  nadeau
 **	Added ImFileFormat struct for file format information.  Left old way
 **	in still.
 **	
 **	Revision 1.5  90/05/02  11:13:25  mjb
 **	More of the above
 **	
 **	Revision 1.4  90/05/02  07:56:57  mjb
 **	moreof the above
 **	
 **	Revision 1.3  90/05/02  07:51:53  mjb
 **	Changed ImVfb and ImClt typedefs to be structures, not pointers
 **	This will require declarations of the form:
 **
 **		ImClt *srcClt;
 **	
 **	Revision 1.2  90/04/30  10:28:54  nadeau
 **	Changed tag list stuff to tag table stuff.  Added error codes and
 **	renumbered the ones already there.  Updated the function declarations
 **	and fixed various comments.  Added file format flag defines.
 **	
 **	Revision 1.1  90/03/28  11:17:57  nadeau
 **	Initial revision
 **	
 **/


#ifndef __IMH__
#define __IMH__

#ifndef __SDSCH__
//#include "sdsc.h"
#endif /* __SDSCH__ */



/*
 *  GLOBAL VARIABLE
 *	ImErrNo		-  error number
 *	ImNErr		-  number of error messages
 *	ImErrList	-  error messages
 *
 *  DESCRIPTION
 *	On an error, the image library routines return -1, or NULL, and set
 *	ImErrNo to an error code.  The programmer may call ImPError
 *	to print the associated error message to stderr, or may do the
 *	message lookup in ImErrList directly.
 */

extern int   ImErrNo;			/* Current error number		*/
extern int   ImNErr;			/* Number of error messages	*/
extern char *ImErrList[];		/* List of error messages	*/





/*
 *  CONSTANTS
 *	IME...		-  error codes
 *
 *  DESCRIPTION
 *	ImErrNo may be set to these error codes as a result of an error in
 *	calling one of the image library routines.
 */

#define IMESYS		0		/* System error			*/
#define IMEMALLOC	1		/* Cannot allocate		*/

#define IMENOCONTENTS	2		/* Bad fields mask		*/
#define IMENOFPDATA	3		/* Bad float data		*/
#define IMEBADINOUT	4		/* Bad inout arg		*/
#define IMEBADFBTYPE	5		/* Bad frame buffer type	*/
#define IMEFORMAT	6		/* Bad format			*/
#define IMENOFILE	7		/* No file?			*/
#define IMENOTINFO	8		/* Not enough info in VFB	*/
#define IMEBADFLIP	9		/* Bad flip selection		*/
#define IMEBADALGORITHM	10		/* Bad res change algorithm	*/

#define IMENOREAD	11		/* Read not supported on format	*/
#define IMENOWRITE	12		/* Write not supported on format*/
#define IMENOVFB	13		/* No VFB given for image write	*/
#define IMEMANYVFB	14		/* Too many VFB's for image write*/
#define IMENOTRGB	15		/* VFB isn't an RGB image	*/
#define IMEMAGIC	16		/* Bad magic number on image file*/
#define IMEIMAGETYPE	17		/* Unknown image type		*/
#define IMECLTTYPE	18		/* Unknown CLT type		*/
#define IMEDEPTH	19		/* Unknown image depth		*/
#define IMECLTLENGTH	20		/* Bad CLT length		*/
#define IMENOCLT	21		/* No CLT given for image	*/
#define IMEMANYCLT	22		/* Too many CLT's for image	*/
#define IMEUNKNOWN	23		/* VFB isn't a GRAY image	*/

#define IMENULLTAGTABLE	24		/* NULL tag table given		*/
#define IMESYNTAX	25		/* Syntax error			*/
#define IMENOIMAGE	26		/* No images in input file	*/
#define IMEUNSUPPORTED	27		/* Unsupported VFB type		*/
#define IMEDIFFSIZE	28		/* VFB's have different sizes	*/
#define IMEVERSION	29		/* Bad version number in header */
#define IMEPLANES	30		/* Unknown image plane config	*/
#define IMEOUTOFRANGE	31		/* Header value out of legal range */
#define IMEORIENTATION	32		/* Unsupported image orientation*/
#define IMEWIDTH	33		/* Zero or negative image width */
#define IMEHEIGHT	34		/* Zero or negative image height*/
#define IMECONFLICT	35		/* Conflicting info in im header*/
#define IMENOTINDEX8	36		/* Not an Index 8 image		*/
#define IMENOTINDEX16	37		/* Not an Index 16 image	*/
#define IMENOTMONO	38		/* Not a monochrome image	*/
#define IMEFIELD	39		/* Bad field mask		*/
#define IMENOTPOSSIBLE	40		/* Conversion/Output not possible*/
#define IMEDECODING	41		/* Error while decoding image	*/
#define IMEENCODING	42		/* Error while encoding image	*/
#define IMEOPERATION    43		/* Unknown operation		*/
#define IMEGRADUATION   44		/* Unknown graduation		*/
#define IMEHISTMAX	45		/* Histogram  maximum exceeded	*/
#define IMEBADRANGE	46		/* Value range inappropriate	*/
#define IMEKEY		47		/* invalid key parameter passed */
#define IMEADJUST	48		/* invalid adjust parameter passed */
#define IMENOALPHA	49		/* no alpha present when needed	*/
#define IMEPIXELREP	50		/* not exact multiples for pixel rep */
#define IMEKEYFIELD	51		/* Bad keyField on adjust	*/
#define IMEADJUSTFIELD	52		/* Bad adjustField on adjust	*/
#define IMEIMPSHEAR	53		/* shear of 90 degrees encountered */
#define IMEBADCHANNEL	54		/* Bad channel map request      */
#define IMEBADCHANNELS	55		/* Invalid channel map requests */
#define IMEVALOUTOFRANGE   56           /* Value out of range           */
#define IMEFORMATUNKNOWN   57           /* Couldn't figure out format   */
		/*	58-2^32-1	   Reserved			*/





/*
 *  CONSTANTS
 *	IMVFBNULL	-  NULL VFB
 *	IMCLTNULL	-  NULL CLT
 *	IMVFBNEW	-  Indicate a new VFB is to be allocated
 *	IMCLTNEW	-  Indicate a new CLT is to be allocated
 *
 *  DESCRIPTION
 *	These values are each used when a NULL (empty) VFB, or CLT is to be
 *	indicated.  These values may be passed as the destination arguments
 *	to VFB, and CLT, and may be returned as error condition values from
 *	functions that normally return VFB's, or CLT's.
 */

#define IMVFBNULL	((ImVfb *)0)
#define IMVFBNEW	IMVFBNULL
#define IMCLTNULL	((ImClt *)0)
#define IMCLTNEW	IMCLTNULL





/*
 *  TYPEDEF & STRUCTURE
 *	ImCltPtr	-  pointer to CLT entry
 *	ImClt		-  color lookup table
 *
 *  DESCRIPTION
 *	The ImClt struct contains a color lookup table.  Such a CLT may,
 *	or may not, be referenced by a VFB.
 *
 *	clt_ncolors is the number of entries in the CLT.  Each entry
 *	consists of 3 consecutive 8-bit bytes for red, green, and blue.
 *
 *	clt_clt is a pointer to the first byte of the first entry in the
 *	CLT.
 */

typedef unsigned char *ImCltPtr;	/* Color pointer		*/

typedef struct ImClt
{
	int    clt_ncolors;	/* # colors in CLT storage		*/
	ImCltPtr clt_clt;	/* points to actual CLT storage		*/
} ImClt;





/*
 *  TYPEDEF & STRUCTURE
 *	ImVfbPtr	-  pointer to VFB pixel
 *	ImVfb		-  virtual frame buffer
 *
 *  DESCRIPTION
 *	The ImVfb struct contains a virtual frame buffer (an image and its
 *	associated per-pixel data).
 *
 *	vfb_width and vfb_height give the width and height of the VFB.
 *
 *	vfb_fields is a mask that indicates what types of values are
 *	associated with each pixel in the VFB.
 *
 *	vfb_nbytes is the number of bytes per pixel.
 *
 *	vfb_clt is an optional CLT associated with the VFB.  vfb_clt will
 *	be IMCLTNULL if there is no CLT.
 *
 *	vfb_roff, vfb_goff, and vfb_boff are byte offsets to reach the
 *	red, green, and blue planes (if any) of the VFB.
 *
 *	vfb_aoff is a byte offset to the alpha plane (if any) of the VFB.
 *
 *	vfb_i8off, and vfb_i16off are byte offsets to the 8-bit or 16-bit
 *	color index planes (if any) of the VFB.
 *
 *	vfb_wpoff is a byte offset to the write protect plane (if any) of the
 *	VFB.
 *
 *	vfb_zoff is a byte offset to the Z-buffer plane (if any) of the VFB.
 *
 *	vfb_fpoff and vfb_ioff are a byte offsets to the floating point and
 *	integer data planes (if any) of the VFB.
 *
 *	vfb_pfirst and vfb_plast point to the first and last pixels in the
 *	VFB.
 */

typedef unsigned char *ImVfbPtr;	/* Pixel pointer			*/

typedef struct ImVfb
{
	int    vfb_width;	/* # cols				*/
	int    vfb_height;	/* # rows				*/
	int    vfb_fields;	/* mask specifying what is in each pixel*/
	int    vfb_nbytes;	/* # bytes per pixel 			*/
	ImClt  *vfb_clt;	/* points to attached CLT, if any	*/
	int    vfb_roff;	/* offset (in bytes) to reach red 	*/
	int    vfb_goff;	/* green				*/
	int    vfb_boff;	/* blue					*/
	int    vfb_aoff;	/* alpha-value				*/
	int    vfb_i8off;	/* color index				*/
	int    vfb_wpoff;	/* write protect offset			*/
	int    vfb_i16off;	/* color index				*/
	int    vfb_zoff;	/* z-value				*/
	int    vfb_moff;	/* mono					*/
	int    vfb_fpoff;	/* floating point			*/
	int    vfb_ioff;	/* integer				*/
	ImVfbPtr vfb_pfirst;	/* points to first pixel 		*/
	ImVfbPtr vfb_plast;	/* points to last pixel 		*/
} ImVfb;






/*
 *  CONSTANTS
 *	IMVFB...	-  fields mask values
 *
 *  DESCRIPTION
 *	The fields mask of a VFB is the bitwise OR of one or more of
 *	these mask values.  Each value indicates that data of that type
 *	is to be stored in the VFB at each pixel location.
 */

#define IMVFBALL		( ~0 )		/* Everything		*/

#define IMVFBIMAGEMASK		( 0xFF )	/* Image items		*/
#define IMVFBMONO		( 1 <<  0 )
#define IMVFBINDEX8		( 1 <<  1 )
#define IMVFBGRAY		IMVFBINDEX8
#define IMVFBGREY		IMVFBINDEX8
#define IMVFBINDEX16		( 1 <<  2 )
#define IMVFBRGB		( 1 <<  3 )
			/*		4-7	   Reserved		*/

#define IMVFBOTHERMASK		( ~0xFF )	/* Non-image items	*/
#define IMVFBALPHA		( 1 <<  9 )
#define IMVFBWPROT		( 1 <<  10 )
#define IMVFBZ			( 1 <<  11 )
#define IMVFBFDATA		( 1 <<  12 )
#define IMVFBIDATA		( 1 <<  13 )
#define IMVFBRED		( 1 <<  14 )
#define IMVFBGREEN		( 1 <<  15 )
#define IMVFBBLUE		( 1 <<  16 )
			/*		17-31	   Reserved		*/





/*
 *  CONSTANTS
 *       IM...		- field specifiers
 *
 *  DESCRIPTION
 *	These are used to pass fields around for adjustment, fill, and other
 *	tools capable of working in color spaces beyond those actually
 *	stored in the VFB (such as hue, saturation, and intensity).
 *
 *	NOTE!  These are not valid for passing to routines, like ImVfbAlloc,
 *	to specify actual fields in a VFB!  Use the IMVFB* variants instead.
 */
#define IMMONO		IMVFBMONO
#define IMGRAY		IMVFBGRAY
#define IMGREY		IMVFBGREY
#define IMINDEX8	IMVFBINDEX8
#define IMINDEX16	IMVFBINDEX16
#define IMRGB		IMVFBRGB
#define	IMALPHA		IMVFBALPHA
#define IMWPROT		IMVFBWPROT
#define IMZ		IMVFBZ
#define IMFDATA		IMVFBFDATA
#define IMIDATA		IMVFBIDATA

#define IMRED		IMVFBRED
#define IMGREEN		IMVFBGREEN
#define IMBLUE		IMVFBBLUE
#define IMSATURATION	( 1 << 28 )
#define IMINTENSITY	( 1 << 29 )
#define IMHUE		( 1 << 30 )
			/* 1-16 should be equal to equivalents to IMVFB*
			  17-27 are reserved				*/

/*
 *  CONSTANTS
 *       IM...		- operations
 *
 *  DESCRIPTION
 *	These are used to pass operations around
 */
#define IMADD		( 1 << 0 )
#define IMSUBTRACT	( 1 << 1 )
#define IMMULTIPLY	( 1 << 2 )
#define IMDIVIDE        ( 1 << 3 )
#define IMSET		( 1 << 4 )
			/*		5-8 	Reserved		*/

#define IMCOMPOVER	( 1 << 9 )
#define IMCOMPINSIDE	( 1 << 10 )
#define IMCOMPOUTSIDE	( 1 << 11 )
#define IMCOMPATOP	( 1 << 12 )
			/*		13-32 	Reserved		*/





/*
 *  MACROS
 *	ImVfb...	-  set and query VFB attributes
 *
 *  DESCRIPTION
 *	These macros, masquerading as subroutines, take a VFB pointer and
 *	set or query information from it.
 */

/* CLT and Characteristics.						*/
#define ImVfbQWidth( v )	((v)->vfb_width)
#define ImVfbQHeight( v )	((v)->vfb_height)
#define ImVfbQFields( v )	((v)->vfb_fields)
#define ImVfbQNBytes( v )	((v)->vfb_nbytes)
#define ImVfbQClt( v )		((v)->vfb_clt)

#define ImVfbSClt( v, c )	((v)->vfb_clt = (c))


/* Pixel values.							*/
#define ImVfbQMono( v, p )	( (int) ( *( (p)+(v)->vfb_moff ) ) )
#define ImVfbQIndex8( v, p )	( (int) ( *( (p)+(v)->vfb_i8off ) ) )
#define ImVfbQGrey( v, p )	ImVfbQIndex8(v,p)
#define ImVfbQGray( v, p )	ImVfbQIndex8(v,p)
#define ImVfbQIndex16( v, p )	( *((sdsc_uint16 *)((p)+(v)->vfb_i16off ) ) )
#define ImVfbQIndex( v, p )	\
	( ( ImVfbQFields(v)&IMVFBINDEX8 ) ? ImVfbQIndex8(v,p) :\
	 				    ImVfbQIndex16(v,p) )
#define ImVfbQRed( v, p )	( (int) ( *( (p)+(v)->vfb_roff ) ) )
#define ImVfbQGreen( v, p )	( (int) ( *( (p)+(v)->vfb_goff ) ) )
#define ImVfbQBlue( v, p )	( (int) ( *( (p)+(v)->vfb_boff ) ) )

#define ImVfbQAlpha( v, p )	( (int) ( *( (p)+(v)->vfb_aoff ) ) )
#define ImVfbQWProt( v, p )	( (int) ( *( (p)+(v)->vfb_wpoff) ) )
#define ImVfbQZ( v, p )		( *((int *)((p)+(v)->vfb_zoff ) ) )
#define ImVfbQFData( v, p )	( *((float *)( (p)+(v)->vfb_fpoff ) ) )
#define ImVfbQIData( v, p )	( *((int *)( (p)+(v)->vfb_ioff ) ) )


#define ImVfbSMono( v, p, m )	( *( (p)+(v)->vfb_moff ) = (unsigned char)(m) )
#define ImVfbSGray( v, p, g )	ImVfbSIndex8(v,p,g)
#define ImVfbSGrey( v, p, g )	ImVfbSIndex8(v,p,g)
#define ImVfbSIndex8( v, p, i )	( *( (p)+(v)->vfb_i8off ) = (unsigned char)(i) )
#define ImVfbSIndex16( v, p, i ) \
	( *((sdsc_uint16 *)((p)+(v)->vfb_i16off) ) = (unsigned short)(i) )
#define ImVfbSIndex( v, p, i )	\
	( ( ImVfbQFields(v)&IMVFBINDEX8 ) ? ImVfbSIndex8(v,p,i) :\
	 				    ImVfbSIndex16(v,p,i) )
#define ImVfbSRed( v, p, r )	( *( (p)+(v)->vfb_roff ) = (unsigned char)(r) )
#define ImVfbSGreen( v, p, g )	( *( (p)+(v)->vfb_goff ) = (unsigned char)(g) )
#define ImVfbSBlue( v, p, b )	( *( (p)+(v)->vfb_boff ) = (unsigned char)(b) )

#define ImVfbSAlpha( v, p, a )	( *( (p)+(v)->vfb_aoff ) = (unsigned char)(a) )
#define ImVfbSWProt( v, p, wp )	( *( (p)+(v)->vfb_wpoff) = (unsigned char)(wp))
#define ImVfbSZ( v, p, z )	( *((int *)((p)+(v)->vfb_zoff) ) = (int)(z) )
#define ImVfbSFData( v, p, fp )	( *((float *)((p)+(v)->vfb_fpoff) )=(float)(fp))
#define ImVfbSIData( v, p, i )	( *((int *)((p)+(v)->vfb_ioff) ) = (int)(i))


/* Pixel addressing.							*/
#define ImVfbQFirst( v )	( (v)->vfb_pfirst )
#define ImVfbQLast( v )		( (v)->vfb_plast )
#define ImVfbQPtr( v, x, y )	( ImVfbQFirst(v) + \
				  ImVfbQNBytes(v)*( (y)*ImVfbQWidth(v) + \
				  (x) ) )
#define ImVfbQNext( v, p )	( (p) + ImVfbQNBytes(v) )
#define ImVfbQPrev( v, p )	( (p) - ImVfbQNBytes(v) )
#define ImVfbQLeft( v, p )	ImVfbQPrev(v,p)
#define ImVfbQRight( v, p )	ImVfbQNext(v,p)
#define ImVfbQUp( v, p )	( (p) - ImVfbQWidth(v)*ImVfbQNBytes(v) )
#define ImVfbQDown( v, p )	( (p) + ImVfbQWidth(v)*ImVfbQNBytes(v) )

#define ImVfbSInc( v, p )	( (p) += ImVfbQNBytes(v) )
#define ImVfbSDec( v, p )	( (p) -= ImVfbQNBytes(v) )
#define ImVfbSLeft( v, p )	ImVfbSDec( v, p )
#define ImVfbSRight( v, p )	ImVfbSInc( v, p )
#define ImVfbSPrev( v, p )	ImVfbSDec( v, p )
#define ImVfbSNext( v, p )	ImVfbSInc( v, p )
#define ImVfbSUp( v, p )	( (p) -= ImVfbQWidth(v)*ImVfbQNBytes(v) )
#define ImVfbSDown( v, p )	( (p) += ImVfbQWidth(v)*ImVfbQNBytes(v) )

#define ImVfbSameRGB(vfb, one, two)				\
	((ImVfbQRed(vfb, one)==ImVfbQRed(vfb,two)) &&		\
	(ImVfbQGreen(vfb, one)==ImVfbQGreen(vfb, two)) &&	\
	(ImVfbQBlue(vfb, one)==ImVfbQBlue(vfb, two)) )

#define ImVfbSameRGBA(vfb, one, two)				\
	((ImVfbQRed(vfb, one)==ImVfbQRed(vfb,two)) &&		\
	(ImVfbQGreen(vfb, one)==ImVfbQGreen(vfb, two)) &&	\
	(ImVfbQAlpha(vfb, one)==ImVfbQAlpha(vfb, two)) &&	\
	(ImVfbQBlue(vfb, one)==ImVfbQBlue(vfb, two)) )		

#define ImVfbSameIndex8(vfb, one, two)				\
	(ImVfbQIndex8(vfb, one)==ImVfbQIndex8(vfb,two)) 

#define ImVfbSameIndex16(vfb, one, two)				\
	(ImVfbQIndex16(vfb, one)==ImVfbQIndex16(vfb,two)) 





/*
 *  MACROS
 *	ImClt...	-  Set and query CLT attributes
 *
 *  DESCRIPTION
 *	These macros, masquerading as subroutines, take a CLT pointer and
 *	set or retrieve information from it.
 */

/* Characteristics.							*/
#define ImCltQNColors(c)	( (c)->clt_ncolors )


/* Entry values.							*/
#define ImCltQRed( p )		( (int) ( *((p)+0) ) )
#define ImCltQGreen( p )	( (int) ( *((p)+1) ) )
#define ImCltQBlue( p )		( (int) ( *((p)+2) ) )

#define ImCltSRed( p, r )	( *((p)+0) = (unsigned char)(r) )
#define ImCltSGreen( p, g )	( *((p)+1) = (unsigned char)(g) )
#define ImCltSBlue( p, b )	( *((p)+2) = (unsigned char)(b) )


/* Entry addressing.							*/
#define ImCltQFirst( c )	( (c)->clt_clt )
#define ImCltQPtr( c, i )	( ImCltQFirst(c) + (3 * (i)) )
#define ImCltQLast( c )		( ImCltQFirst(c) + \
				( 3*( ImCltQNColors(c) - 1 ) ) )
#define ImCltQNext( c, p )	( (p) + 3 )
#define ImCltQPrev( c, p )	( (p) - 3 )

#define ImCltSInc( c, p )	( (p) += 3 )
#define ImCltSDec( c, p )	( (p) -= 3 )






/*
 *  TYPEDEF & STRUCT
 *	ImFileFormatReadMap	-  map of how a format handles reads
 *	ImFileFormatWriteMap	-  map of how a format handles writes
 *
 *  DESCRIPTION
 *	These two tables are essentially mirror images of each other.  The
 *	read map tells what depths of imagery can be read from a format and
 *	how it maps to VFB's.  The write map tells how different VFB's map
 *	to file depths.
 *
 *	map_inType and map_outType select whether the image is represented
 *	as a CLT index per pixel (pseudo-color) or with the full RGB color
 *	per pixel (true-color).
 *
 *	map_inNChannels and map_outNChannels give the number of channels per
 *	pixel (usually 1 or 3).
 *
 *	map_inChannelDepth and map_outChannelDepth give the size of the
 *	channels, in bits.  Usually 1-bit (mono) through 8-bit.
 *
 *	map_inAttributes and map_outAttributes select the attributes of
 *	an incomming or outgoing file image, such as whether it has a CLT,
 *	alpha planes, or is compressed somehow.  For read, the _outAttributes
 *	may not contain any compression indications.  Likewise for write, the
 *	_inAttributes may not contain compression stuff.
 *
 *	map_inField and map_outField select the type of VFB (as a VFB
 *	field mask) that is created by the read or taken by the write.  Only
 *	image depth type field values should be included (ie., don't add
 *	IMVFBALPHA, etc).
 *
 *	map_write is a function pointer to the write function to handle
 *	this specific output mapping.  There is no equivalent read for the
 *	read map.
 */

typedef struct ImFileFormatReadMap
{
	int    map_inType;		/* In image type (index | rgb)	*/
	int    map_inNChannels;		/* In # of channels		*/
	int    map_inChannelDepth;	/* In channel size (in bits)	*/
	int    map_inAttributes;	/* In image attributes		*/

	int    map_outField;		/* Out VFB field		*/
	int    map_outAttributes;	/* Out VFB attributes		*/
} ImFileFormatReadMap;

typedef struct ImFileFormatWriteMap ImFileFormatWriteMap;

struct ImFileFormatWriteMap
{
	int    map_inField;		/* In VFB field			*/
	int    map_inAttributes;	/* In VFB attributes		*/

	int    map_outType;		/* Out image type (index | rgb)	*/
	int    map_outNChannels;	/* Out # of channels		*/
	int    map_outChannelDepth;	/* Out channel size (in bits)	*/
	int    map_outAttributes;	/* Out image attributes		*/

#ifdef __STDC__
	int  (*map_write)(
		ImFileFormatWriteMap * ,
		int , int , FILE* ,
		TagTable* , TagTable* );/* Out write function		*/
#else
	int  (*map_write)( );		/* Out write function		*/
#endif
};





/*
 *  CONSTANTS
 *	IMTYPEINDEX	-  Indexed image type
 *	IMTYPERGB	-  RGB image type
 *	IMTYPE2D	-  2D primitives
 *	IMCOMP*		-  Compression options
 *	IMINTER*	-  Interlace options
 *	IMCLT*		-  CLT options
 *	IMALPHA*	-  Alpha plane options
 *	IMGROUP*	-  Grouping options
 * 	IMFILECOMP*	-  File compression option(s)
 *
 *  DESCRIPTION
 *	All of these are values used in initializing fields of the read
 *	and write mapping structures for the various formats.  They may also
 *	be used as user request values for flagsTable flags:
 *
 *	"image type request",		int type      = IMTYPEINDEX or IMTYPERGB
 *	"image channel number request",	int nchannels = 1 or 3 usually
 *	"image channel depth request",	int depth     = 1-32
 *	"image compression request",	int comp      = IMCOMP*
 *	"image interleave request",	int inter     = IMINTER*
 *	"image clt request",		int clt       = IMCLT*
 *	"image alpha request",		int alpha     = IMALPHA*
 *	"image group request",		int group     = IMGROUP*
 *	"image quality request"		int quality   = 1..100 usually
 *	"file compression"              int type      = 1
 *
 */

#define IMTYPEINDEX	(0)		/* CLT index per pixel		*/
#define IMTYPERGB	(1)		/* RGB color per pixel		*/
#define IMTYPE2D	(2)		/* 2D primitives		*/

#define IMCOMPMASK	0x0F
#define IMCOMPNO	(0<<0)		/* No compression		*/
#define IMCOMPNONE	(0<<0)		/* No compression		*/
#define IMCOMPRLE	(1<<0)		/* Run-Length Encoded compression*/
#define IMCOMPLZW	(2<<0)		/* Limpel-Ziv Welsh compression	*/
#define IMCOMPPACKBITS	(3<<0)		/* Packbits compression		*/
#define IMCOMPPB	(3<<0)		/* Packbits compression		*/
#define IMCOMPMAC	(3<<0)		/* Packbits compression		*/
#define IMCOMPDCT	(4<<0)		/* Descrete Cosine Transform    */
#define IMCOMPASCII	(5<<0)		/* Descrete Cosine Transform    */
		/*	(6<<0) - (15<<0)   Reserved			*/

#define IMGROUPMASK	0xF0
#define IMGROUPSCANLINES (0<<4)		/* Group into scanlines(default)*/
#define IMGROUPTILES	(1<<4)		/* Group into tiles             */
		/*	(2<<4) - (15<<4)   Reserved                     */

#define IMINTERMASK	0xF00
#define IMINTERNO	(0<<8)		/* Non-interleave		*/
#define IMINTERNONE	(0<<8)		/* Non-interleave		*/
#define IMINTERLINE	(1<<8)		/* Scanline interleave		*/
#define IMINTERPLANE	(2<<8)		/* Plane interleave		*/
		/*	(3<<8) - (15<<8) Reserved			*/

#define IMCLTMASK	0xF000
#define IMCLTNO		(0<<12)		/* No CLT included		*/
#define IMCLTNONE	(0<<12)		/* No CLT included		*/
#define IMCLTNODUMP	(0<<12)		/* No CLT included		*/
#define IMCLTYES	(1<<12)		/* CLT included			*/
#define IMCLTDUMP	(2<<12)		/* CLT included			*/
		/*	(3<<12) - (15<<12) Reserved			*/

#define IMALPHAMASK	0xF0000
#define IMALPHANO	(0<<16)		/* No Alpha planes included	*/
#define IMALPHANONE	(0<<16)		/* No Alpha planes included	*/
#define IMALPHANODUMP	(0<<16)		/* No Alpha planes included	*/
#define IMALPHAYES	(1<<16)		/* Alpha planes included	*/
#define IMALPHADUMP	(1<<16)		/* Alpha planes included	*/
		/*	(2<<16) - (15<<16) Reserved			*/

#define IMQUALITYMASK	0xF00000
#define IMQUALITYNO	(0<<20)         /* No quality control allowed   */
#define IMQUALITYYES    (1<<20)         /* Optional quality control     */
		/*  	(2<<20) - (15<<20) Reserved                     */

		/*	(1<<24) - (1<<31)  Reserved			*/




/*
 *  TYPEDEF & STRUCT
 *	ImFileMagic	-  magic number info
 *	ImFileFormat	-  file format info
 *
 *  DESCRIPTION
 *	The imFileFormat struct describes everything we know generically
 *	about the set of file formats supported by the image library.
 *
 *	format_names is a list of the names we know the format by.  The
 *	first name in the list is its "primary" name (the name we like most).
 *	The rest are "equivalent" names.  Names are used as command-line
 *	options in image tools, and as possible file extensions.  The list
 *	is terminated with a NULL.  Since all name checking is case-insensitive,
 *	only list things in lower case.
 *
 *	format_help is a help string that explains what the format is, in
 *	40 characters or less.  For instance, "ras" would have "Sun rasterfile"
 *	as its help string.  "pix" would have "Alias picture file" as its
 *	help string.  Image library tools will use this as the help string
 *	for the option keyword in their argument parsing code.
 *
 *	format_creator is a string indicating who created and/or supports
 *	this format.  "rgb" would read "Silicon Graphics".  "ras" would read
 *	"Sun Microsystems, Inc.".
 *
 *	format_readSupport and format_writeSupport are one-or-more line strings
 *	describing the variants of the format that we support.  The real
 *	detail specifics are left to the read and write maps (later), but these
 *	strings say the same thing in more human terms.  The strings are used
 *	by image tools to list what can be supported.
 *
 *      The struct ImFileMagic contains:
 *	format_magicLocation, _magicLength, and _magicNumber give information
 *	about the format's magic number, if any.  The location is the # of
 *	bytes from the start of the file at which the magic number is found.
 *	This is almost always 0.  The length is the # of bytes that constitute
 *	the magic number, and is usually 4.  If this value is 0, there is no
 *	magic number for this format.  The magic number itself is a string of
 *	bytes that must match the number in the file.  Image library tools
 *	will typicaly check this against the file in both MBF and LBF order.
 *
 *	format_readMultiVfb and format_writeMultiVfb are booleans indicating
 *	if the format can read in, and write out multiple images for the same
 *	file.
 *
 *	format_readPipe and format_writePipe are booleans indicating if
 *	the format can handle a pipe.  If they can't, the generic code will
 *	make a temp file for the format code.
 *
 *	format_read is a function pointer to the top level read routine for
 *	the format.  All format read requests enter here.
 *
 *	format_readMap describes in detail the different file variants that
 *	can be handled and what kind of VFB they map to.
 *
 *	format_writeMap describes in detail the different file variants that
 *	can be written and what kind of VFB they accept.  The map also has
 *	function pointers to specific format write routines to handle those
 *	variants.
 */

typedef struct ImFileMagic
{
	int    format_magicLocation;	/* Location of magic #		*/
	int    format_magicLength;	/* # bytes in magic #		*/
	unsigned char *format_magicNumber;	/* Magic #		*/
} ImFileMagic;
	
typedef struct ImFileFormat 
{
	char  **format_names;		/* Format name string		*/
	char  *format_help;		/* Long form of name		*/
	char  *format_creator;		/* Creator/source of format	*/
	char  *format_readSupport;	/* Message on read handling	*/
	char  *format_writeSupport;	/* Message on write handling	*/

	ImFileMagic *format_magicMap;	/* Magic numbers		*/

	int    format_readMultiVfb;	/* Can read generate multiple VFBs?*/
	int    format_readPipe;		/* Can read handle a pipe?	*/
	int    format_writeMultiVfb;	/* Can write take multiple VFBs?*/
	int    format_writePipe;	/* Can write handle a pipe?	*/

#ifdef __STDC__
	int  (*format_read)(int,int,FILE*,
		TagTable*, TagTable*);	/* Read format			*/
#else
	int  (*format_read)( );		/* Read format			*/
#endif
	ImFileFormatReadMap *format_readMap;/* Read support mapping	*/
	ImFileFormatWriteMap *format_writeMap;/* Write support mapping	*/
} ImFileFormat;


/*
 *  Values for format_readMultiVfb and format_writeMultiVfb:
 */
#define IMMULTI		(1)		/* Supports multi-image files	*/
#define IMNOMULTI	(0)		/* Doesn't			*/

#define IMPIPE		(1)		/* Supports pipe input		*/
#define IMNOPIPE	(0)		/* Doesn't			*/


/*
 * TYPEDEF and STRUCTURE
 *	ImCompressScheme
 *
 * DESCRIPTION
 *	This is the list of file compression/encoding schemes.
 *
 *	The elements of the structure are as follows:
 *
 *	compress_suffixes
 *		A list of suffixes used by the scheme.  For instance,
 *		compress/uncompress use the "Z" suffix.
 *	
 *	compress_name
 *		The name of the scheme.  "Z" compression is called
 *		"Lempel-Ziv Encoding"
 *
 *	compress_magic_numbers
 *		The possible magic numbers for a file of this type.
 *		This magic number structure is the same as the one
 *		in the ImFileFormat structure above.
 *
 *	compress_decode
 *		This is a subroutine which uncompresses a file, and destroys
 *		the compressed one.
 *
 *	compress_encode
 *		This is a subroutine which encodes a file, and destroys the
 *		unencoded one.
 *
 *	For more information about how to add your own compression schemes,
 *	refer to imschemes.c.
 *
 */


typedef struct ImCompressScheme
{
	char **compress_suffixes;          	/* List of suffixes   	*/
	char *compress_name;                  	/* Name                 */
	ImFileMagic* compress_magic_numbers;  	/* Magic number(s)      */
#ifdef __STDC__
	int  (*compress_decode)(char*,char*,TagTable*);  /* Decode (read) routine*/
	int  (*compress_encode)(char*,char*,TagTable*);  /* Encode (write) routine*/
#else
	int  (*compress_decode)();   		/* Read routine		*/
	int  (*compress_encode)();  		/* Write routine	*/
#endif

} ImCompressScheme;



/*
 *  CONSTANTS
 *	IMVFB...	-  raster operations and inout values
 *
 *  DESCRIPTION
 *	The first set of constants define various different raster operations
 *	used by ImVfbFill() et al.
 *
 *	IMVFBINSIDE and IMVFBOUTSIDE indicate how filling is to take place	
 *	in ImVfbFill().
 */

#define IMVFBCLEAR	0
#define IMVFBSET	15

#define IMVFBSOURCE	3
#define IMVFBPATTERN	5
#define IMVFBNSOURCE	12
#define IMVFBNPATTERN	10

#define IMVFBAND	1
#define IMVFBNAND	14

#define IMVFBOR		7
#define IMVFBXOR	6
#define IMVFBNOR	8
#define IMVFBNXOR	9

#define IMVFBINSIDE	0
#define IMVFBOUTSIDE	1

#define IMGRADNONE	0
#define IMGRADHORIZ	1
#define IMGRADVERT	2

#define IMVFBPRINTFLOAT	(1<<0)
#define IMVFBPRINTINT   (1<<1)
#define IMVFBPRINTINTFLOAT (IMVFBPRINTFLOAT|IMVFBPRINTINT)






/*
 *  CONSTANTS
 *	IMVFB...	-  flip directions
 *
 *  DESCRIPTION
 *	These constants are used to select flip directions in ImVfbFlip().
 */

#define IMVFBXFLIP	0x1		/* X direction			*/
#define IMVFBYFLIP	0x2		/* Y direction			*/
#define IMVFBXYFLIP	( IMVFBXFLIP | IMVFBYFLIP )	/* Both		*/
#define IMVFBLEFTRIGHT	IMVFBXFLIP
#define IMVFBUPDOWN	IMVFBYFLIP
#define IMVFBLEFTRIGHTUPDOWN	IMVFBXYFLIP
		/*	0x4-0xFFFFFFFF	Reserved			*/





/*
 *  CONSTANTS
 *	IMVFB...	-  resolution change algorithms
 *
 *  DESCRIPTION
 *	These constants select the algorithm to use when changing the
 *	resolution of an image using ImVfbResize().
 */

#define IMVFBPIXELREP	0	/* Pixel replecation			*/
#define IMVFBBILINEAR	1	/* Bilinear interpolation		*/




/*
 *  MACROS
 *	Im...		-  function pseudonims
 *
 *  DESCRIPTION
 *	These macros are defiend to support various names for the same
 *	function, such as 'gray' vs. 'grey'.
 */

#define ImVfbToGrey	ImVfbToGray




/*
 *  CONSTANTS
 *	IMERRORFATAL	-  fatal error
 *	IMERRORWARNING	-  warning error
 *	IMERRORINFO	-  information error
 *
 *  DESCRIPTION
 *	These three values flag the type of error causing the invocation of
 *	an error handler (nominated via the flagsTable).
 */

#define IMERRORFATAL	0
#define IMERRORWARNING	1
#define IMERRORINFO	2
		/*	3-2^32		   Reserved			*/





/*
 *  CONSTANT
 *	IMMAXNAME	-  maximum format name size
 *	IMDEFMONOTHRESH	-  default monochrome threshold
 */

#define IMMAXNAME	100
#define IMDEFMONOTHRESH	127





/*
 *  STRUCT & TYPEDEF
 *	ImHistTable	-  histogram information table
 *
 *  DESCRIPTION
 *	ImVfbHist( ) computes a field-correlated histogram on an image
 *	and returns occurrence counts for each unique field value or
 *	combination of field values.  That list of occurrence counts and
 *	unique values is returned in an ImHistTable structure.
 *
 *	For example, a histogram is computed on the RED and GREEN fields
 *	of an image.  The returned ImHistTable will have:
 *
 *	imhist_nEntries
 *		The number of unique RED-GREEN color combinations found.
 *	imhist_fieldMask
 *		A bitmask mask of the fields checked:  IMRED | IMGREEN
 *	imhist_nFields
 *		The number of fields checked... in this case 2:  RED & GREEN.
 *	imhist_values[IMHISTRED]
 *		A list of imhist_nEntries red values as part of the RED-GREEN
 *		combo.
 *	imhist_values[IMHISTGREEN]
 *		A list of imhist_nEntries green values as part of the RED-GREEN
 *		combo.
 *	imhist_values[all others]
 *		NULL pointers because these fields were not part of the
 *		histogram calcuation requested.
 *	imhist_nOccur
 *		A list of the occurrence counts for each of the RED-GREEN
 *		pairs listed in imhist_values[IMHISTRED] and
 *		imhist_values[IMHISTGREEN].  This list will be sorted from
 *		high to low, if requested on the call to ImVfbHist( ).
 *
 *	This information could be plotted by the user on a graph where the
 *	vertical axis is an occurrence count, and the horizontal axis is
 *	each RED-GREEN color combo listed one at a time.  The imhist_nOccur
 *	value for each RED-GREEN pair would be the height of a data point
 *	for the horizontal position of the RED-GREEN pair on the graph.
 *
 *	Such graphs are more typically used on single-field histograms,
 *	such as just the RED field, or just the HUE field, or whatever.
 *	In that case, occurrences would be the vertical axis, and the field
 *	value the horizontal axis.  For example, a histogram of RED in an
 *	image would produce a graph like the following:
 *
 *	100 -|                 * *                                            
 *	     |                  *                                             
 *	80  -|                                                             ** 
 *	     |                *                                         * *   
 *	60  -|                    *                                   *  *     
 *	     |              *                                       *         
 *	40  -|*                                                    *          
 *	     |   *        *             *                    *    *           
 *	20  -|  *  *                                      *      *            
 *	     | *  * ****** * *     ***** ***************** ** ***             
 *	 0  -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
 *	     0       32      64      96      128     160     192     224     256
 *
 *	This kind of graph of histogram values shows us that this example
 *	image has a fair amount of bright red (right end of graph) quite a bit
 *	of low-midrange red (middle of graph) and a bit of dark red (left
 *	edge of graph).  The blank spots in the graph are red values that do
 *	not occur in the image (the *'s along the bottom of the graph show
 *	occurrence values of 0).
 */

#define IMMAXFIELDS   20
typedef struct ImHistTable
{
        int     imhist_nEntries;	/* Number of unique colors in table*/
        int     imhist_fieldMask;	/* Fields for histogram		*/
        int     imhist_nFields;		/* Number of fields for histogram*/
        union
	{
                unsigned short *imhist_val16;	/* list for index16 numbers */
                unsigned char  *imhist_val8;	/* list for rgba & index8 #s*/
                float  *imhist_valFloat;/* list for hue, sat, intensity nums*/
	} imhist_values[IMMAXFIELDS];
        unsigned int  *imhist_nOccur;	/* list of occurrence counts	*/
} ImHistTable;





/*
 *  CONSTANTS
 *	IMHIST...	-  histogram table array index names
 *	imhist_...	-  histogram table union field names
 *
 *  DESCRIPTION
 *	Values in an ImHistTable returned by ImVfbHist( ) are indexed through
 *	the imhist_values field.  Indexes are named for fields on which the
 *	histogram may have been done.
 *
 *	To provide a more intuitive look to code using a ImHistTable, the
 *	fields in the union are given names for quick reference.
 */
#define IMHISTRED       	0
#define IMHISTGREEN     	1
#define IMHISTBLUE      	2
#define IMHISTALPHA     	3
#define IMHISTHUE       	4
#define IMHISTSATURATION        5
#define IMHISTINTENSITY 	6
#define IMHISTINDEX8    	7
#define IMHISTGRAY		7
#define IMHISTGREY		7
#define IMHISTINDEX16   	8
#define IMHISTMONO		9

#define imhist_red      	imhist_values[IMHISTRED].imhist_val8
#define imhist_blue     	imhist_values[IMHISTBLUE].imhist_val8
#define imhist_green    	imhist_values[IMHISTGREEN].imhist_val8
#define imhist_alpha    	imhist_values[IMHISTALPHA].imhist_val8
#define imhist_hue      	imhist_values[IMHISTHUE].imhist_valFloat
#define imhist_saturation       imhist_values[IMHISTSATURATION].imhist_valFloat
#define imhist_intensity        imhist_values[IMHISTINTENSITY].imhist_valFloat
#define imhist_gray	   	imhist_values[IMHISTINDEX8].imhist_val8
#define imhist_grey	   	imhist_values[IMHISTINDEX8].imhist_val8
#define imhist_index8   	imhist_values[IMHISTINDEX8].imhist_val8
#define imhist_index16  	imhist_values[IMHISTINDEX16].imhist_val16
#define imhist_mono	   	imhist_values[IMHISTMONO].imhist_val8





/*
 *  CONSTANTS
 *	IMMAXIMUM	-  computed maximum field value
 *	IMMINIMUM	-  computed minimum field value
 *	IMUNIQUEVAL	-  computed number of unique field values
 *	IMMAXNUMSTATS	-  number of statistics returned by ImVfbStat
 *
 *  DESCRIPTION
 *	ImVfbStat( ) computes simple statistics on a single pixel field in
 *	an image.  Statistics are returned in a floating point array of
 *	IMMAXNUMSTATS entries.  Individual statistics are obtained by
 *	indexing into this array using IMMAXIMUM, etc.
 */
#define	IMMAXIMUM	0
#define	IMMINIMUM	1
#define	IMUNIQUEVAL	2
#define IMMAXNUMSTATS	20





/*
 *  TYPEDEF & STRUCTURE
 *	ImHotSpot	-  image hotspot
 *
 *  DESCRIPTION
 *	ImHotSpot contains a cursor hotspot:  the point on a cursor image
 *	that corresponds to the cursor's location for picking purposes.
 *	For instance, this would be the point of an arrow cursor, the tip
 *	of a pencil cursor, or the end of the stinger on a bumble-bee cursor!
 *
 *	ImHotSpot contains the cursor's hotspot location, as measured from
 *	the upper-left corner (0,0) of the image, in pixels.
 */

typedef struct ImHotSpot
{
	int	hot_x;			/* x coordinate of hotspot	*/
	int	hot_y;			/* y coordinate of hotspot	*/
} ImHotSpot;

typedef ImHotSpot *ImHotSpotPtr;





/*
 *  MACROS
 *	ImHotSpot...	-  set and query HotSpot attributes
 *
 *  DESCRIPTION
 *	These macros, masquerading as subroutines, take a HotSpot pointer
 *	and set or query information from it.
 */

#define ImHotSpotQX( hs )		( (hs)->hot_x )
#define ImHotSpotQY( hs )		( (hs)->hot_y )
#define ImHotSpotSX( hs, xcoord )	( (hs)->hot_x = (int) xcoord )
#define ImHotSpotSY( hs, ycoord )	( (hs)->hot_y = (int) ycoord )




/*
 *  FUNCTIONS
 *	Im...		-  Generic functions
 *	ImClt...	-  CLT functions
 *	ImVfb...	-  VFB functions
 *	ImFile...	-  File I/O functions
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __STDC__
extern void		 ImPError( char * );
extern char		*ImQError( void );

extern void		 ImRgbToHsi( int[3], float[3] );
extern void		 ImHsiToRgb( float[3], int[3] );

extern ImClt		*ImCltAlloc( int );
extern void		 ImCltFree( ImClt * );
extern ImClt		*ImCltDup( ImClt * );
extern ImClt		*ImCltGrayRamp( ImClt *, int, int, int, int, ImClt * );
extern ImClt		*ImCltRoll( ImClt *, int, ImClt * );

extern ImVfb		*ImVfbAdjust( ImVfb *, int, double, double, int, int, double, double, ImVfb * );
extern ImVfb		*ImVfbAlloc( int, int, int );
extern ImVfb		*ImVfbClear( int, int, ImVfb * );
extern ImVfb		*ImVfbComp( ImVfb *, int, int, int, int, int, int, ImVfb *, int, int );
extern ImVfb		*ImVfbCopy( ImVfb *, int, int, int, int, int, ImVfb *, int, int );
extern ImVfb		*ImVfbDup( ImVfb * );
extern ImVfb		*ImVfbFade( ImVfb *, double, int, ImVfb * );
extern ImVfb		*ImVfbFill( ImVfb *, int, int, int, int, int, double, double, int, int, ImVfb * );
extern ImVfb		*ImVfbFlip( ImVfb *, int, ImVfb * );
extern void		 ImVfbFree( ImVfb * );
extern ImHistTable	*ImVfbHist( ImVfb *, int, int );
extern void		 ImVfbHistTableFree( ImHistTable *  );
extern ImVfb		*ImVfbLightness( ImVfb *, double, ImVfb * );
extern ImVfb		*ImVfbMix( ImVfb *, double, ImVfb *, double, int, ImVfb * );
extern ImVfb		*ImVfbResize( ImVfb *, int, ImVfb *, int, int );
extern ImVfb		*ImVfbRoll( ImVfb *, int, int, ImVfb * );
extern ImVfb		*ImVfbRotate( ImVfb *, double, ImVfb * );
extern ImVfb		*ImVfb90Rotate( ImVfb *, ImVfb * );
extern float		*ImVfbStat( ImVfb *, int, float * );
extern ImVfb		*ImVfbToIndex( ImVfb *, int, ImVfb * );
extern ImVfb		*ImVfbToIndex8( ImVfb *, ImVfb * );
extern ImVfb		*ImVfbToIndex16( ImVfb *, ImVfb * );
extern ImVfb		*ImVfbToMono( ImVfb *, int, ImVfb * );
extern ImVfb		*ImVfbFSDitherToMono( ImVfb *, ImVfb * );
extern ImVfb		*ImVfbToRgb( ImVfb *, ImVfb * );
extern ImVfb		*ImVfbToGray( ImVfb *, ImVfb * );
extern ImVfb		*ImVfbXShear( ImVfb *, double, ImVfb * );
extern ImVfb		*ImVfbYShear( ImVfb *, double, ImVfb * );
extern void		 ImVfbPrint( FILE*, int, ImVfb* );
extern ImVfb 		*ImVfbCopyChannel( ImVfb* inVfb, int inFieldMask, ImVfb* outVfb, int outFieldMask);


extern int		 ImFileRead( int, char *, TagTable *, TagTable * );
extern int		 ImFileFRead( FILE *, char *, TagTable *, TagTable * );
extern int		 ImFileWrite( int, char *, TagTable *, TagTable * );
extern int		 ImFileFWrite( FILE *, char *, TagTable *, TagTable * );
extern char		*ImFileQFormat( int, char * );
extern char		*ImFileQFFormat( FILE *, char * );
extern int		 ImFileQNFormat( void );
extern int		 ImFileFormatOptions( int, ArgOption *, ArgOption ** );
extern int		 ImFileFormatEquivs( int, ArgEquiv *, ArgEquiv ** );
extern ImFileFormat    **ImFileFindFormat( char * );
extern char		*ImFileQCompression(int , TagTable* );
extern char		*ImFileQFCompression(FILE* , TagTable* );
ImCompressScheme 	*ImFileFindCompressionScheme( char* );

extern ImFileFormat    **ImGetFileFormats( void );
extern ImCompressScheme**ImGetCompressSchemes( void );	

#else

extern void		 ImPError( );
extern char		*ImQError( );

extern void		 ImRgbToHsi( );
extern void		 ImHsiToRgb( );

extern ImClt		*ImCltAlloc( );
extern void		 ImCltFree( );
extern ImClt		*ImCltDup( );
extern ImClt		*ImCltGrayRamp( );
extern ImClt		*ImCltRoll( );

extern ImVfb		*ImVfbAdjust( );
extern ImVfb		*ImVfbAlloc( );
extern ImVfb		*ImVfbClear( );
extern ImVfb		*ImVfbComp( );
extern ImVfb		*ImVfbCopy( );
extern ImVfb		*ImVfbDup( );
extern ImVfb		*ImVfbFade( );
extern ImVfb		*ImVfbFill( );
extern ImVfb		*ImVfbFlip( );
extern void		 ImVfbFree( );
extern ImVfb		*ImVfbGamma( );
extern ImHistTable	*ImVfbHist( );
extern void		 ImVfbHistTableFree( );
extern ImVfb		*ImVfbLightness( );
extern ImVfb		*ImVfbMix( );
extern ImVfb		*ImVfbResize( );
extern ImVfb		*ImVfbRoll( );
extern ImVfb		*ImVfbRotate( );
extern ImVfb		*ImVfb90Rotate( );
extern float		*ImVfbStat( );
extern ImVfb		*ImVfbToIndex( );
extern ImVfb		*ImVfbToIndex8( );
extern ImVfb		*ImVfbToIndex16( );
extern ImVfb		*ImVfbFSDitherToMono( );
extern ImVfb		*ImVfbToMono( );
extern ImVfb		*ImVfbToRgb( );
extern ImVfb		*ImVfbToGray( );
extern ImVfb		*ImVfbXShear( );
extern ImVfb		*ImVfbYShear( );
extern void		 ImVfbPrint( );
extern ImVfb 		*ImVfbCopyChannel( );

extern int		 ImFileRead( );
extern int		 ImFileFRead( );
extern int		 ImFileWrite( );
extern int		 ImFileFWrite( );
extern char		*ImFileQFormat( );
extern char		*ImFileQFFormat( );
extern int		 ImFileQNFormat( );
extern int		 ImFileFormatOptions( );
extern int		 ImFileFormatEquivs( );
extern ImFileFormat    **ImFileFindFormat( );
extern char		*ImFileQCompression( );
extern ImCompressScheme *ImFileFindCompressionScheme( );

extern ImFileFormat    **ImGetFileFormats( );
extern ImCompressScheme**ImGetCompressSchemes( );	

#endif

#ifdef __cplusplus
}
#endif

#endif /* __IMH__ */

