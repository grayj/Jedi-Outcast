/**
 **     $Header: /roq/libim/imviff.c 1     11/02/99 4:38p Zaphod $
 **     Copyright (c) 1989-1995  San Diego Supercomputer Center (SDSC)
 **             a division of General Atomics, San Diego, California, USA
 **
 **     Users and possessors of this source code are hereby granted a
 **     nonexclusive, royalty-free copyright and design patent license to
 **     use this code in individual software.  License is not granted for
 **     commercial resale, in whole or in part, without prior written
 **     permission from SDSC.  This source is provided "AS IS" without express
 **     or implied warranty of any kind.
 **
 **     For further information contact:
 **             E-Mail:         info@sds.sdsc.edu
 **
 **             Surface Mail:   Information Center
 **                             San Diego Supercomputer Center
 **                             P.O. Box 85608
 **                             San Diego, CA  92138-5608
 **                             (619) 534-5000
 **/

#define HEADER  "    $Header: /roq/libim/imviff.c 1     11/02/99 4:38p Zaphod $"

/**
 **  FILE
 **     imviff.c         - Khoros image storage type
 **
 **  PROJECT
 **     libim   -  SDSC image manipulation library
 **
 **  DESCRIPTION
 **     imviff.c contains routines to read and write VIFF image files for
 **     the image manipulation library.  Raster data read in is stored
 **     in a VFB.  Raster data written out is taken from a tag table.
 **
 **  PUBLIC CONTENTS
 **                     d =defined constant
 **                     f =function
 **                     m =defined macro
 **                     t =typedef/struct/union
 **                     v =variable
 **                     ? =other
 **
 **	none
 **
 **  PRIVATE CONTENTS
 **     imViffRead       f  read a Targa file
 **     imViffWrite      f  write an uncompressed RGB file
 **
 **
 **  
 **  HISTORY

 **     $Log: /roq/libim/imviff.c $
 ** 
 ** 1     11/02/99 4:38p Zaphod
 **     Revision 1.12  1995/06/29  00:28:04  bduggan
 **     updated copyright year
 **
 **     Revision 1.11  1995/06/15  21:22:38  bduggan
 **     changed bzero to memset
 **
 **     Revision 1.10  1995/04/03  21:40:22  bduggan
 **     took out #ifdef NEWMAGIC
 **
 **     Revision 1.9  1995/01/10  23:50:04  bduggan
 **     made read/write routines static
 **
 **     Revision 1.8  94/10/03  11:31:09  nadeau
 **     Updated to ANSI C and C++ compatibility.
 **     Removed all use of register keyword.
 **     Minimized use of custom SDSC types (e.g., uchar vs. unsigned char)
 **     Changed all float arguments to double.
 **     Added forward declarations.
 **     Added misc. casts to passify SGI and DEC compilers.
 **     Changed all macros and defined constants to have names
 **     starting with IM.
 **     Rearranged magic number structures for format handlers.
 **     Made format handler routines static (i.e., local to file).
 **     Updated comments, adding format descriptions and references.
 **     Updated indenting on some code.
 **     Updated copyright message.
 **     
 **     Revision 1.7  93/08/26  17:11:05  secoskyj
 **     Modified the read routine error messages
 **     
 **     Revision 1.6  92/12/03  01:56:17  nadeau
 **     Corrected info messages.
 **     
 **     Revision 1.5  92/11/04  12:10:02  groening
 **     put ImFIleFormat info and magic number info
 **     from imfmt.c into this file.
 **     
 **     Revision 1.4  92/10/19  14:06:11  groening
 **     added ImInfo features
 **     
 **     Revision 1.3  92/08/31  17:43:23  vle
 **     Updated copyright notice.
 **     
 **     Revision 1.2  92/08/24  14:29:18  groening
 **     minor stuff
 **     
 **     Revision 1.1  92/06/02  09:34:49  groening
 **     Initial revision
 **     
 **/


/**
 **
 **  FORMAT
 **	VIFF - Khoros viff images
 **
 **  AKA
 **	xv
 **
 **  FORMAT REFERENCES
 **	Writing Programs/VIFF Format, Chapter 1, Khoros Programmer's
 **		Manual, Volume II, University of New Mexico
 **
 **  CODE CREDITS
 **     Custom development, Chris Groening, San Diego Supercomputer Center, 1992.
 **
 **  DESCRIPTION
 **
 **  	Viff images are very flexible: they can store images in a variety
 **  	of encoding schemes and dimensions.  More than 3 dimensions are
 **  	possible in order to store data in Viff files that is not necessarily
 **  	meant to be visualized.
 **
 ** 	A Viff file has the following sections:
 **
 **		- header		(1024 bytes)
 **		- map(s)	
 **		- location data
 ** 		- image data
 **
 **		The locations of the last three sections are specified
 **		in the last three fields of the header.  The (very verbose)
 **		header is described in detail below.
 **
 **
 **  	The Viff format can deal with many things, however not amny of these
 **  	are compatible with the way image tools is implemented now.  Right now
 **  	multiple images are not handled.
 **
 **		What can be translated is 1 or 3 band image stored in bit or byte.
 **		1 band image can also be stored in 2 byte format (short).
 **		All of these formats can have optional color lookup tables.
 **		The color lookup table can only be stored in byte format.
 **
 **
 **
 **
 **   	Here is the complete description of the viff format header file,
 **   	taken directly from Chapter 1 Volume 2 of the khoros manuals 
 **
 The header is 1024 bytes long.  Its fields can be grouped into five categories:
	- administration : file management information
 	- data storiage  : describes how the data is stored, but not how it
		it is interpreted.
	- location data : describes the spatial location of the data (optional)
	- data mapping  : describes how the data should be mapped or
		interpreted.
	- color space   : for images, indicates what coordinate space and
		model is being used.


______________________________________________________________________________
______________________________________________________________________________
char       	A one byte magic number that identifies a VIFF file
identifier;

DEFINES: XV_FILE_MAGIC_NUM

______________________________________________________________________________
char		A one byte code indicating Khoros file type
file_type;		 (currently only VIFF).

DEFINES: XV_FILE_TYPE_XVIFF

______________________________________________________________________________
char		A one byte code indicating the specific release of the
release;        viff.h file (currently 0); this does not have to agree
		with the Khoros system release number.

DEFINES: IM_XV_IMAGE_REL_NUM

______________________________________________________________________________
char		A one byte code indicating the specific version of the
version;	viff.h file (currently 3);  this does not have to agree
		with the Khoros system version number.  For example,
	        version 1 release 0 is referred to as viff 1.0.

DEFINES: IM_XV_IMAGE_VER_NUM

______________________________________________________________________________
char		A 512 byte space available for any use, but currently is
comment[512];	used in Khoros as a comment field to document the VIFF
		data file.

DEFINES: none

______________________________________________________________________________
char		A one byte code indicating how the particular architecture
machine_dep;	that the image was last processed on treats data.  Currently
		supported are DEC order, IEEE order and NS order.  Supported
		machines include the VAX, SUN, SONY News, Silicon Graphics,
		Motorola, Encore, Sequent, MIPS, DEC, IBM, Apollo, and NeXT.

DEFINES: VIFF_DEP_IEEEORDER
         VIFF_DEP_DECORDER
         VIFF_DEP_NSORDER
         VIFF_DEP_BIGENDIAN
         VIFF_DEP_LITENDIAN

______________________________________________________________________________
unsigned long	These two unsigned long fields indicate data size,
row_size,	specifically, the number of data items in a band.
col_size;	row_size indicates the length of a row (the number of columns,
		or the image width) in pixels.  col_size indicates the
		length of a column (the number of rows, or the image height)
		in pixels.  Images with row and column sizes of zero are
		sometimes valid, and indicate that only the map information
		is important.  The product of the two values is the total
		number of data items present.

DEFINES: none

______________________________________________________________________________
unsigned long	This unsigned long field specifies the length of any subrows
subrow_size	in the image.  This is useful when one wants pixel vectors
		to represent 2D objects (images). The size of each pixel
		"image" would be subrow_size (columns) by
		num_data_bands/subrow_size (rows).  This field may be
		ignored except by routines that need the 2D interpretation.

DEFINES: none

______________________________________________________________________________
unsigned long	These two long fields indicate the location of a
startx,		subimage in a parent image.  The image is a sub image
starty;		if startx and starty have values greater than zero.
		startx and starty locate the upper left hand corner
		of where the image was extracted.
		This applies to 2D data that has implicit locations.

DEFINES: IMVFF_NOTSUB

______________________________________________________________________________
float		These two floats specify the actual pixel size in meters
pixsizx,	at the time of digitization.  This information is needed
pixsizy;	to do true measurements and calculate true frequencies.
		The ratio of these fields will give you the aspect ratio
		of the digitized pixel.  Most CCD camera's are not one-to-one.
		These values may not have a meaning if the data is
		IMVFF_LOC_EXPLICIT.

DEFINES: none

______________________________________________________________________________
unsigned long	This unsigned long field indicates
location_type;	whether the image data has implicit or explicit locations.

		If the locations are implicit, the field location_dim must
		be set to zero (0), and the location data will be empty.

		If the locations are explicit, the field location_dim
		indicates the dimensionality of the space (1D, 2D and 3D
		will be most common).  The explicit location data is pointed
		to by location, and is stored as bands of coordinates. 
		For example, if (location_dim = 2), implying 2D locations,
		the location data would be stored; x1, x2, . . . , xn;
		y1, y2, . . . , yn.

DEFINES: IMVFF_LOC_IMPLICIT
	 IMVFF_LOC_EXPLICIT

______________________________________________________________________________
unsigned long	This unsigned long indicates the dimensionality of the
location_dim	location space of data.  Zero-, one-, and two-
		dimensionalities  will be most common.  If the location_type
		is implicit, this field must be set to zero (0). If the
		location_type is explicit, this field is set to the
		dimensionality correctly describing the data.  Remember that
		3-dimensional image data is represented with 
		location_dim = 2 (x & y); the third dimension (pixel intensity)
		is stored in the imagedata.

DEFINES: none

______________________________________________________________________________
unsigned long	This unsigned long indicates the
num_of_images	the number of images (not bands) pointed to by *imagedata.

DEFINES: none

______________________________________________________________________________
unsigned long	This unsigned long indicates the number of
num_data_bands  bands per image or the dimensionality of vectors.
		In some cases, it may be convenient to think of an
		image pixel as a vector (when there is more than one band).

DEFINES: none

______________________________________________________________________________
unsigned long	This unsigned long field indicates the data storage type
data_storage_type;   of the pixel or vector data.  Currently, bit, byte,
		short, integer, float, float complex, double, and double
		complex data types are supported.  The BIT storage type
		stores the bits in packed unsigned chars and pads to
		a byte;  the order is Least Significant Bit (LSB) first.

DEFINES: IMVFF_TYP_BIT
         IMVFF_TYP_1_BYTE
         IMVFF_TYP_2_BYTE
         IMVFF_TYP_4_BYTE
         IMVFF_TYP_FLOAT
         IMVFF_TYP_COMPLEX
         IMVFF_TYP_DOUBLE
         IMVFF_TYP_DCOMPLEX

______________________________________________________________________________
unsigned long	This unsigned long field contains the information that
data_encode_scheme  specifies the encoding or compression method used for
		storage of the data.  Only the raw and compress encoding
		schemes are supported in Khoros 1.0.

DEFINES: IMVFF_DES_RAW
         IMVFF_DES_COMPRESS
         IMVFF_DES_RLE
         IMVFF_DES_TRANSFORM
         IMVFF_DES_CCITT
         IMVFF_DES_ADPCM
         IMVFF_DES_GENERIC

______________________________________________________________________________
unsigned long	This unsigned long field specifies the type of mapping 
map_scheme	that should occur.  The map is an array, where the data
		pointed to by *imagedata is used as an index into rows
		of the array.

		ONEPERBAND indicates that each data band has a map.  CYCLE is
		for when a single band is displayed by cycling through maps.
		SHARED is for when there is only one map for all bands to
		share.  The GROUP mapping scheme is for future Khoros 
		development, and should not be used as of Khoros 1.0.

DEFINES: IMVFF_MS_NONE
         IMVFF_MS_ONEPERBAND
         IMVFF_MS_CYCLE
         IMVFF_MS_SHARED
         IMVFF_MS_GROUP

______________________________________________________________________________
unsigned long	This unsigned long field indicates the type
map_storage_type;  of data in the map.  Data in the map may be of type
		char, short, integer, float, or complex.  This is also
		the resulting data type after a mapping has been done.

DEFINES: IMVFF_MAPTYP_NONE
         IMVFF_MAPTYP_1_BYTE
         IMVFF_MAPTYP_2_BYTE
         IMVFF_MAPTYP_4_BYTE
         IMVFF_MAPTYP_FLOAT
         IMVFF_MAPTYP_COMPLEX

______________________________________________________________________________
unsigned long	These two unsigned long fields indicate the number of
map_row_size,	rows in the 2D map and the number of columns in the map.
map_col_size;	The maps are stored as a sequence of columns (stacked columns).

DEFINES: none

______________________________________________________________________________
unsigned long	This unsigned long field specifies the number of subrows
map_subrow_size; in a map.  This is useful when using the output vector from
		the map is a 2D image, rather than just a vector.
		The size of the 2D image would be: map_subrow_size (columns) by
		map_row_size/map_subrow_size (rows). This field may be ignored
		except by routines that need the 2D interpretation.

DEFINES: none

______________________________________________________________________________
unsigned long	This unsigned long field specifies if the image data will be
map_enable;	valid regardless of whether or not it has been sent through
		a map.  In some cases the mapping is optional
		(IMVFF_MAP_OPTIONAL),  while in others, the data must be mapped
		in order for it to have a valid meaning (IMVFF_MAP_FORCE).  
		See the previous section on FILE FORMAT TERMINOLOGY for
		a more detailed explanation of the map_enable field.

DEFINES: IMVFF_MAP_OPTIONAL
	 IMVFF_MAP_FORCE

______________________________________________________________________________
unsigned long	This unsigned long field specifies the number of
maps_per_cycle;	maps that would constitute one cycle when the cycled map
		scheme type is used. Of course, this field is only valid
		when map_scheme is set to IMVFF_MS_CYCLE.

DEFINES: none
______________________________________________________________________________
unsigned long	This unsigned long field indicates the color space or the
color_space_model;   coordinate system being used to interpret the image bands.

		The color space model defines use the following convention:

		NTSC: National Television Systems Committee
		CIE:  Commission Internationale de L'Eclairage
		UCS:  Universal Chromaticity Scale
		RGB:  Red Component, Green Component, Blue Component
		CMY:  Cyan Component, Magenta Component, Yellow Component
		YIQ:  Luminance, I and Q represent chrominance
		HSV:  Hue, Saturation & Value
		IHS:  Intensity, Hue & Saturation


		Most color map models only make sense when the map_scheme is
		set to IMVFF_MS_ONEPERBAND and the map_row_size = 3 and the
		num_data_bands = 1; alternatively, the map_scheme may be set
		to IMVFF_MS_NONE and the map_row_size = 0, and the
		num_data_bands = 3.  The exceptions to this are:

		IMVFF_CM_NONE indicates that no color space model has been
		assigned,
		IMVFF_CM_GENERIC indicates that a color space is valid, but is
		being defined by the user.
		IMVFF_CM_genericRGB would imply that IMVFF_MS_ONEPERBAND is set
		with map_row_size = 3 and num_data_bands = 1.  genericRGB is
		an RGB image but doesn't conform to a standard.

DEFINES: IMVFF_CM_NONE
         IMVFF_CM_ntscRGB
         IMVFF_CM_ntscCMY
         IMVFF_CM_ntscYIQ
         IMVFF_CM_HSV
         IMVFF_CM_HLS
         IMVFF_CM_IHS
         IMVFF_CM_cieRGB
         IMVFF_CM_cieXYZ
         IMVFF_CM_cieUVW
         IMVFF_CM_cieucsUVW
         IMVFF_CM_cieucsSOW
         IMVFF_CM_cieucsLab
         IMVFF_CM_cieucsLuv
         IMVFF_CM_GENERIC
         IMVFF_CM_genericRGB

______________________________________________________________________________
unsigned long	Spare fields available for user defined fields.  These
ispare1,	fields are not supported except for reading and writing
ispare2;	correctly with respect to machine dependencies.
float
fspare1,
fspare2;

DEFINES: none

______________________________________________________________________________
reserve		Reserve space is allocated so that the total length of the
		header is 1024 bytes and so that more fields can be added.

DEFINES: none

______________________________________________________________________________
char *maps;	This is a pointer to the beginning of the map data.
		When it is used, it must be cast to the proper data type.

DEFINES: none


______________________________________________________________________________
float *location	This is a pointer to the beginning of the location data.
		The dimensionality of the location data is given in the field
		location_dim. (location_dim specifies the number of
		bands of coordinates.) The number of coordinate values per
		band is row_size*col_size.

DEFINES: none

______________________________________________________________________________
char		This is a pointer to the beginning of the image data.  When it
*imagedata	is used in an image processing routine it must be cast to the
		proper data type.

DEFINES: none

______________________________________________________________________________
*/

#include "iminternal.h"

/*
 *  VIFF - Khoros Visualization Image File Format
 *      For information on these structures, how to use them, etc. please
 *      see imfmt.c.
 */
#ifdef __STDC__
static int imViffRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );
static int imViffWriteRGBCLT (ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, 
	TagTable *flagsTable, TagTable *tagTable);
static int imViffWriteRGBNOCLT( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, 
	TagTable *flagsTable, TagTable *tagTable);
static int imViffWriteINDEX8CLT(ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, 
	TagTable *flagsTable, TagTable *tagTable );
static int imViffWriteINDEX8NOCLT( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, 
	TagTable *flagsTable, TagTable *tagTable);
static int imViffWriteINDEX16CLT( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, 
	TagTable *flagsTable, TagTable *tagTable);
static int imViffWriteINDEX16NOCLT( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, 
	TagTable *flagsTable, TagTable *tagTable);
static int imViffWriteMONO( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, 
	TagTable *flagsTable, TagTable *tagTable);
#else
static int imViffRead( );
static int imViffWriteRGBCLT( );
static int imViffWriteRGBNOCLT( );
static int imViffWriteINDEX8CLT( );
static int imViffWriteINDEX8NOCLT( );
static int imViffWriteINDEX16CLT( );
static int imViffWriteINDEX16NOCLT( );
static int imViffWriteMONO( );
#endif
static char *imViffNames[ ]  = { "viff", "xv", NULL };
static ImFileFormatReadMap imViffReadMap[ ] =
{
        /* in                   out                                     */
        /* type,ch,dep, attr.   VFB type        attr.                   */
        { RGB,3,8,      0,      IMVFBRGB,       0 },
        { RGB,3,8,      C,      IMVFBRGB,       C },
        { IN,1,1,       0,      IMVFBMONO,      0 },
        { IN,1,8,       C,      IMVFBINDEX8,    C },
        { IN,1,8,       0,      IMVFBINDEX8,    0 },
        { IN,1,16,      C,      IMVFBINDEX16,   C },
        { IN,1,16,      0,      IMVFBINDEX16,   0 },
        { -1,           0,      -1,             0 },

};
static ImFileFormatWriteMap imViffWriteMap[ ] =
{
        /* in                   out                                     */
        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */
        { IMVFBRGB,     C,      RGB,3,8,        C,      imViffWriteRGBCLT },
        { IMVFBRGB,     0,      RGB,3,8,        0,      imViffWriteRGBNOCLT },
        { IMVFBINDEX8,  0,      IN,1,8,         0,      imViffWriteINDEX8NOCLT},        { IMVFBINDEX8,  C,      IN,1,8,         C,      imViffWriteINDEX8CLT},
        { IMVFBINDEX16, C,      IN,1,16,        C,      imViffWriteINDEX16CLT},
        { IMVFBINDEX16, 0,      IN,1,16,        0,     imViffWriteINDEX16NOCLT},        { IMVFBMONO,    0,      IN,1,1,         0,      imViffWriteMONO},
        { -1,           0,      -1,             0,      NULL },
};

static ImFileMagic imFileViffMagic []=
{
	{ 0, 0, NULL},
};

ImFileFormat ImFileViffFormat =
{
	imViffNames, "Khoros Visualization image file",
	"Khoros",
	"1-bit monochrome, 8- and 16-bit color index, and 24-bit RGB color\n\
mages, uncompressed.",
	"1-bit monochrome, 8- and 16-bit color index, and 24-bit RGB color\n\
mages, uncompressed.",
	imFileViffMagic,
	IMNOMULTI, IMNOPIPE,
	IMMULTI, IMNOPIPE,
	imViffRead, imViffReadMap, imViffWriteMap
};

#ifdef __STDC__
static int	imReadViff1Byte(int ioType, int fd, FILE *fp, ImVfb *vfb, int q, unsigned long h, unsigned long w, unsigned long c);
static int	imReadViff2Byte(int ioType, int fd, FILE *fp, ImVfb *vfb, int q, unsigned long h, unsigned long w, unsigned long c);
static int	imReadViffBit(int ioType, int fd, FILE *fp, ImVfb *vfb, int q, unsigned long h, unsigned long w, unsigned long c);
static int	imViffWriteHeader( int ioType, int fd, FILE *fp, TagTable *flagsTable,  TagTable *tagTable,unsigned long numDataBands,
		unsigned long dataStorageType, unsigned long dataEncodeScheme, unsigned long mapScheme,
		unsigned long colorSpaceModel, unsigned long mapRowSize );
static int	imViffWriteRGBData (int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);
static int	imViffWriteINDEX8Data (int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);
static int	imViffWriteINDEX16Data (int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);
#else
static int	imReadViff1Byte();
static int	imReadViff2Byte();
static int	imReadViffBit();
static int	imViffWriteHeader();
static int	imViffWriteRGBData();
static int	imViffWriteINDEX8Data();
static int	imViffWriteINDEX16Data();
#endif

/* definitions for version number,
   char release; */
#define IM_XV_IMAGE_VER_NUM        3       /* Version 3 (3.1) */


/* definitions for release number,
   char version; */
#define IM_XV_IMAGE_REL_NUM        1       /* Release 1   */

/* definitions for subimage information,
   long startx, starty; */
#define IMVFF_NOTSUB              ~0      /* a negative number indicates that
                                           the image is not a subimage  */

/* definitions for machine dependencies,
   char machine_dep; */
#define IMVFF_DEP_IEEEORDER       0x2     /* IEEE byte ordering */
#define IMVFF_DEP_DECORDER        0x4     /* DEC (VAX) byte ordering */
#define IMVFF_DEP_NSORDER         0x8     /* NS32000 byte ordering */
#define IMVFF_DEP_CRAYORDER       0xA     /* Cray byte size and ordering */

#define IMVFF_DEP_BIGENDIAN       IMVFF_DEP_IEEEORDER
#define IMVFF_DEP_LITENDIAN       IMVFF_DEP_NSORDER


/* definitions for data storage type,
   unsigned long data_storage_type; */
#define IMVFF_TYP_BIT             0       /* pixels are on or off (binary image)*/
                                        /* Note: This is an X11 XBitmap 
                                           with bits packed into a byte and
                                           padded to a byte */
#define IMVFF_TYP_1_BYTE          1       /* pixels are byte (unsigned char) */
#define IMVFF_TYP_2_BYTE          2       /* pixels are two byte (short int) */
#define IMVFF_TYP_4_BYTE          4       /* pixels are four byte (integer) */
#define IMVFF_TYP_FLOAT           5       /* pixels are float (single precision)*/
#define IMVFF_TYP_COMPLEX         6       /* pixels are complex float */
#define IMVFF_TYP_DOUBLE          9       /* pixels are float (double precision)*/

#define IMVFF_TYP_DCOMPLEX        10      /* double complex */

/* definitions for data encoding scheme on disk - i.e. it may be
   compressed using RLE, or uncompressed (RAW).
   unsigned long data_encode_scheme; */
#define IMVFF_DES_RAW             0       /* Raw - no compression */
#define IMVFF_DES_COMPRESS        1       /* Compressed using ALZ */
#define IMVFF_DES_RLE             2       /* Compressed using RLE */
#define IMVFF_DES_TRANSFORM       3       /* Transform based compression */
#define IMVFF_DES_CCITT           4       /* CCITT standard compression */
#define IMVFF_DES_ADPCM           5       /* ADPCM compression */
#define IMVFF_DES_GENERIC         6       /* User-specified compression */

/* definitions for map data or cells storage type,
   unsigned long map_storage_type; */
#define IMVFF_MAPTYP_NONE         0       /* No cell type is assigned  */
#define IMVFF_MAPTYP_1_BYTE       1       /* cells are byte (unsigned char)    */
#define IMVFF_MAPTYP_2_BYTE       2       /* cells are two byte (short int) */
#define IMVFF_MAPTYP_4_BYTE       4       /* cells are four byte (integer) */
#define IMVFF_MAPTYP_FLOAT        5       /* cells are float (single precision) */
#define IMVFF_MAPTYP_COMPLEX      6       /* cells are complex FLOAT */
#define IMVFF_MAPTYP_DOUBLE       7       /* cells are float (double precision) */

/* definitions for mapping schemes,
   unsigned long map_scheme; */
#define IMVFF_MS_NONE             0       /* No mapping is to be done, and no
                                           maps are to be stored. */
#define IMVFF_MS_ONEPERBAND       1       /* Each data band has its own map */
#define IMVFF_MS_CYCLE            2       /* An array of maps is selected in order
                                           by groups of maps_per_cycle, allowing
                                           "rotating the color map" */
#define IMVFF_MS_SHARED           3       /* All data band share the same map */
#define IMVFF_MS_GROUP            4       /* All data bands are "grouped" 
                                           together to point into one map */
/* definitions for enabling the map,
   unsigned long map_enable; */
#define IMVFF_MAP_OPTIONAL        1       /* The data is valid without being
                                           sent thru the color map. If a
                                           map is defined, the data may 
                                           optionally be sent thru it. */
#define IMVFF_MAP_FORCE           2       /* The data MUST be sent thru the map
                                           to be interpreted */
#define IMVFF_CM_NONE     0
#define IMVFF_CM_ntscRGB  1
#define IMVFF_CM_ntscCMY  2
#define IMVFF_CM_ntscYIQ  3
#define IMVFF_CM_HSV      4
#define IMVFF_CM_HLS      5
#define IMVFF_CM_IHS      6
#define IMVFF_CM_cieRGB   7
#define IMVFF_CM_cieXYZ   8
#define IMVFF_CM_cieUVW   9
#define IMVFF_CM_cieucsUVW        10
#define IMVFF_CM_cieucsSOW        11
#define IMVFF_CM_cieucsLab        12
#define IMVFF_CM_cieucsLuv        13
#define IMVFF_CM_GENERIC          14      /* the color space is user defined */
#define IMVFF_CM_genericRGB       15      /* an RGB image but not conforming
                                           to any standard */

/* definitions for location type,
   unsigned long location_type; */
#define IMVFF_LOC_IMPLICIT        1       /*  The location of image pixels
                                            or vector data is given by using
                                            the implied 2D array given by
                                            row_size and col_size.  */
#define IMVFF_LOC_EXPLICIT        2       /*  The location of the image pixels
                                            or the vectors is explicit */

#define IMVIFF_HEADERSIZE	1024

/*  TYPEDEF & STRUCTURE
 *  imViffHeaderInfo     - VIFF file header information
 *  imViffTgaHeaderFields   - VIFF file header fields for binary package
 */

typedef struct imViffHeaderInfo
{
	/*  Administrative or file management information */

        char    viff_identifier;     /* a magic number that tells
                                   the world that this is an
                                   Khoros file */

        char    viff_file_type;      /* tells if this file is a VIFF file */

        char    viff_release;        /* release number */

        char    viff_version;        /* version number */

        char    viff_machine_dep;    /* indicates peculiarities of */
                                /* machine architecture */
        
        char    viff_trash[3];       /* preserves word boundaries */
                                /* groups of 4 bytes */

        char    viff_comment[512];   /* text for image commentary */

/* Things that specify the spatial properties of the image, pixel
   organization, and data storage arrangement.  */

        unsigned long   viff_row_size;       /* length of row in pixels,
                                           i.e. number of columns */

        unsigned long   viff_col_size;       /* length of column in pixels,
                                           i.e. number or rows */
        unsigned long   viff_subrow_size;    /* Length of subrows.
					   This is useful
                                           when one wants pixel vectors to
                                           represent 2D objects (images).
                                           The size of each pixel "image"
                                           would be subrow_size (columns)
                                           by num_data_bands/subrow_size (rows).
                                           This field may be ignored except
                                           by routines that need the 2D
                                           interpretation. */

/* The product of row_size and col_size is used to indicate 
   the number of locations when the location type is explicit,
   the product also indicates the number of pixels in a band,
   or the number of vectors.  */

        long    viff_startx, viff_starty; /* subimage starting position (upper
                                   left hand corner), negative indicates
                                   that it is not a subimage */

        float   viff_pixsizx, viff_pixsizy;       /* Actual size of pixel 
				at time of digitization in meters */

        unsigned long   viff_location_type;  /* implied or explicit location
                                           data (implied locations are
                                           derived from row_size and
                                           col_size */

        unsigned long   viff_location_dim;   /* explicit locations can be of
                                           any dimension  */

        unsigned long   viff_num_of_images;     /* number of images
                                           pointed to by *imagedata,
                                           do not confuse with number of
                                           bands */

        unsigned long   viff_num_data_bands; /* Number of bands per data pixel,
                                           or number of bands per image, or
                                           dimension of vector data, or
                                           number of elements in a vector */

        unsigned long   viff_data_storage_type;  /*storage type for disk data */

        unsigned long   viff_data_encode_scheme; /*encoding scheme of diskdata*/

/* Things that determine how the mapping (if any) of data bands is 
   to be done to obtain the actual "image" or data.  */

        unsigned long   viff_map_scheme;  /* How mapping (if any) is to occur */

        unsigned long   viff_map_storage_type;	/* Storage type of 	
						cells in the maps */

        unsigned long   viff_map_row_size; /* number of columns in map array */

        unsigned long   viff_map_col_size; /* number of entries in map (rows) */

        unsigned long   viff_map_subrow_size;/*Length of subrows. This is useful
                                            when using the output vector from
                                            the map as a 2-D image, rather
                                            than just a vector. The size of
                                            the 2-D image would be:
                                            map_subrow_size (columns) by
                                            map_row_size/map_subrow_size
                                            (rows). This field may be ignored
                                            except by routines that need the 2D
                                            interpretation */

        unsigned long   viff_map_enable;     /* Tells if the disk data is valid
                                           with or without being sent thru the
                                           map. Some data MUST be mapped to be
                                           valid. */

        unsigned long   viff_maps_per_cycle; /* number of maps to constitue
					 a "cycle" for IMVFF_MS_CYCLE */

/* Specification of the particular color model in use when working with a
   color image. This just tells what the coordinate system and axis orientation
   of the color space is.  */

        unsigned long   viff_color_space_model;

/* Extra fields for use by the user as needed. These are NOT SUPPORTED
   in any way, except for being read and written correctly with respect
   to machine dependencies.  */

        unsigned long   viff_ispare1,viff_ispare2;        /* Spare long ints */

        float           viff_fspare1,viff_fspare2;        /* Spare floats */

/* Pointers to the actual data - these are valid only when in memory! */

        char    viff_reserve[IMVIFF_HEADERSIZE - (21*4)  
                             - (520*1) 
                             - (2*4) - (4*4)
                             - (4*1)];
                                /* maximum header information is 
                                   1024 bytes, what is not currently
                                   used is saved in reserve */

        char *viff_maps;             /* a pointer to the maps, must be cast into 
                                   the proper type */

        float *viff_location;        /* a pointer to the location data (for
                                   explicit locations, each location is
                                   paired with data pointed to by 
                                   *imagedata,  all locations are 
                                   in float  */

        char *viff_imagedata;        /* a pointer to the input data (straight
			 off of disk, must be cast into the proper type */
        } imViffHeaderInfo;

static BinField imViffHeaderFields[]=
{
	{ UCHAR, 1, 1	},	/* viff_identifier		*/
	{ UCHAR, 1, 1	},	/* viff_file_type		*/
	{ UCHAR, 1, 1   },	/* viff_release			*/
	{ UCHAR, 1, 1	},	/* viff_version			*/
	{ UCHAR, 1, 1	},	/* viff_machine_dep		*/
	{ UCHAR, 1, 3	},	/* viff_trash			*/
	{ UCHAR, 1, 512	},	/* viff_comment			*/
	{ ULONG, 4, 1	},	/* viff_row_size		*/
	{ ULONG, 4, 1	},	/* viff_col_size		*/
	{ ULONG, 4, 1	},	/* viff_subrow_size		*/
	{ LONG, 4, 1	},	/* viff_startx			*/
	{ LONG, 4, 1	},	/* viff_starty			*/
	{ FLOAT, 4, 1	},	/* viff_pixsizx			*/
	{ FLOAT, 4, 1	},	/* viff_pixsizy			*/
	{ ULONG, 4, 1	},	/* viff_location_type		*/
	{ ULONG, 4, 1	},	/* viff_location_dim		*/
	{ ULONG, 4, 1	},	/* viff_num_of_images		*/
	{ ULONG, 4, 1	},	/* viff_num_data_bands		*/
	{ ULONG, 4, 1	},	/* viff_data_storage_type	*/
	{ ULONG, 4, 1	},	/* viff_data_encode_scheme	*/
	{ ULONG, 4, 1	},	/* viff_map_scheme		*/
	{ ULONG, 4, 1	},	/* viff_map_storage_type	*/
	{ ULONG, 4, 1	},	/* viff_map_row_size		*/
	{ ULONG, 4, 1	},	/* viff_map_col_size		*/
	{ ULONG, 4, 1	},	/* viff_map_subrow_size		*/
	{ ULONG, 4, 1	},	/* viff_map_enable		*/
	{ ULONG, 4, 1	},	/* viff_maps_per_cycle		*/
	{ ULONG, 4, 1	},	/* viff_color_space_model	*/
	{ ULONG, 4, 1	},	/* viff_ispare1			*/
	{ ULONG, 4, 1	},	/* viff_ipsare2			*/
	{ FLOAT, 4, 1	},	/* viff_fpsare1			*/
	{ FLOAT, 4, 1	},	/* viff_fpsare2			*/
	{ CHAR,  1, (IMVIFF_HEADERSIZE - (21 * 4)
		 - (520*1)
		 - (2 *4) - (4*4)
		 - (4))},/*viff_reserve*/
	{ CHAR ,  4, 1	},	/* viff_maps			*/
	{ FLOAT ,  4, 1	},	/* viff_location		*/
	{ CHAR ,  4, 1	},	/* viff_imagedata		*/
	{ 0, 0, 0}
};


static int                              /* returns number of tags read in */
#ifdef __STDC__
imViffRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )
#else
imViffRead( ioType, fd, fp, flagsTable, tagTable )
        int         ioType;             /* I/O flags                    */
        int         fd;                 /* Input file descriptor        */
        FILE       *fp;                 /* Input file pointer           */
        TagTable   *flagsTable;         /* Flags                        */
        TagTable   *tagTable;           /* Tag table to add to          */
#endif
{
        unsigned char *pBuffer;        /* buffer pointer               */
        ImVfbPtr pPixel;       /* VFB pixel pointer            */
        ImVfb        *vfb;              /* Read in image                */
        ImVfbPtr     pptr;              /* Pixel pointer                */
        ImClt        *clt;              /* New clt                      */
        ImCltPtr     pColor;            /* CLT entry pointer            */
        imViffHeaderInfo  header;        /* Viff header info pointer    */
        unsigned char        *bufferuc;         /* Run buffer(type uchar)       */
        unsigned char        *buffer;           /* Run buffer(type uchar)       */
        long         *bufferl;          /* Run buffer (type long)       */
        int          i,x,y;             /* Counter                      */
        int          colortable;        /* color map   flag             */
        unsigned long c,n,m,h,w,z,q;       /* convenient short names       */
        char         message[100];      /* Tmp error message text       */
	char	     *viffName;		/* storgae foe comment	*/

	colortable=0;
	BinByteOrder (BINMBF);
	if (ImBinReadStruct (ioType, fd, fp, &header, imViffHeaderFields)==-1)
	{
		ImReturnBinError();
	}

        switch ( header.viff_machine_dep )
        {
        case IMVFF_DEP_NSORDER:
        case IMVFF_DEP_DECORDER:
                BinByteOrder (BINLBF);
                ImSeek (ioType,fd,fp,0,0);
                if (ImBinReadStruct (ioType, fd, fp, &header, imViffHeaderFields)==-1)
                {
                        ImReturnBinError();
                }
                break;
        case IMVFF_DEP_IEEEORDER:
        case IMVFF_DEP_CRAYORDER:
                break;
        default:
                ImErrorFatal( "Unknown header byte order\n", -1, IMESYNTAX );
        }



	
/* Below is error checking to make sure that the Viff file we are reading in
   is the type of Viff file that we wish to translate.   Right now there are
   a myriad of permutaions of a Viff file only a small subsection that we
   actually support.  More will be added as requested. */

	if (header.viff_file_type!=1)   {
	   sprintf (message, "Invalid Viff file type, may not be a Viff image.\n");
	   ImErrorFatal (message, -1, IMESYNTAX);
					}

	if (header.viff_machine_dep==IMVFF_DEP_NSORDER)	{
								}

	if ((header.viff_machine_dep!=IMVFF_DEP_IEEEORDER) &
	    (header.viff_machine_dep!=IMVFF_DEP_NSORDER))
	{
	   sprintf (message, "Cannot read non-IEEE ordering.\n");
	   ImErrorFatal (message, -1, IMESYNTAX);
								}

	if (header.viff_location_type!=IMVFF_LOC_IMPLICIT)	{
	   sprintf (message, "Can only read implicit images.\n");
	   ImErrorFatal (message, -1, IMESYNTAX);
						}
	   
	if ((header.viff_data_storage_type==IMVFF_TYP_DCOMPLEX)  ||
	    (header.viff_data_storage_type==IMVFF_TYP_4_BYTE)    ||
	    (header.viff_data_storage_type==IMVFF_TYP_DOUBLE)    ||
	    (header.viff_data_storage_type==IMVFF_TYP_COMPLEX)   ||
	    (header.viff_data_storage_type==IMVFF_TYP_FLOAT)) 	{
	   sprintf (message, "Cannot read data storage type %d images.\n",
		header.viff_data_storage_type);
	   ImErrorFatal (message, -1, IMESYNTAX);
								}
	   
	if ((header.viff_data_storage_type==IMVFF_TYP_2_BYTE)  &
	    (header.viff_num_data_bands==3))			{
	   sprintf (message, "Cannot read 2 bytes per pixel and a 3 band images.\n");
	   ImErrorFatal (message, -1, IMESYNTAX);
								}

	if ((header.viff_data_encode_scheme!=IMVFF_DES_RAW) &	
	    (header.viff_data_encode_scheme!=IMVFF_DES_RLE))	{
	   sprintf (message, "Cannot read encoding scheme %d images.\n",
			header.viff_data_encode_scheme);
	   ImErrorFatal (message, -1, IMESYNTAX);
								}
	   
	if ((header.viff_map_storage_type!=IMVFF_MAPTYP_NONE) &
	    (header.viff_map_storage_type!=IMVFF_MAPTYP_1_BYTE)) {
	   sprintf (message, "Cannot read map storage type %d images.\n",
			header.viff_map_storage_type);
	   ImErrorFatal (message, -1, IMESYNTAX);
								}

	if ((header.viff_color_space_model!=IMVFF_CM_genericRGB) &
	    (header.viff_color_space_model!=IMVFF_CM_NONE))  {
	   sprintf (message, "Cannot read color space scheme %d images.\n",
			header.viff_color_space_model);
	   ImErrorFatal (message, -1, IMESYNTAX);
								}

	if (header.viff_num_of_images!=1)	{
	   sprintf (message, "%d images in file.  Can only read single image file.\n",
			header.viff_num_of_images);
	   ImErrorFatal (message, -1, IMESYNTAX);
						}


	if ((header.viff_num_data_bands!=1) & (header.viff_num_data_bands!=3)){
	   sprintf (message, "There are %d data bands in image.  Can only read 1 or 3.  \n",header.viff_num_data_bands);
	   ImErrorFatal (message, -1, IMESYNTAX);
									      }


	ImInfo ("Image Name", header.viff_comment);
        sprintf (message, "%d",header.viff_release);
        ImInfo ("Release",message);

        sprintf (message, "%d",header.viff_version);
        ImInfo ("Version",message);

        if (header.viff_machine_dep==IMVFF_DEP_IEEEORDER)
        sprintf (message, "Most Significant Byte First");
        else sprintf (message, "Least Significant Byte First");
        ImInfo ("Byte Order",message);

        sprintf (message, "%d",header.viff_num_data_bands);
        ImInfo ("Number of Data Bands",message);

        sprintf (message, "%d x %d",header.viff_row_size, header.viff_col_size);        ImInfo ("Resolution",message);

	if (header.viff_data_storage_type==IMVFF_TYP_BIT)
                sprintf (message, "1-bit Monochrome");

        if (header.viff_data_storage_type==IMVFF_TYP_1_BYTE)
        {
           if (header.viff_num_data_bands==1)
             sprintf (message, "%d-bit Color Indexed",header.viff_num_data_bands*8);
           else sprintf (message, "%d-bit RGB",header.viff_num_data_bands*8);
        }

        if (header.viff_data_storage_type==IMVFF_TYP_2_BYTE)
        {
           if (header.viff_num_data_bands==1)
             sprintf (message, "%d-bit Color Indexed",header.viff_num_data_bands*16);
           else sprintf (message, "%d-bit RGB",header.viff_num_data_bands*16);
        }
        ImInfo ("Type",message);



	w = header.viff_row_size;	/* get the width of the image */
	h = header.viff_col_size;	/* get the height of the image */
	c = header.viff_data_encode_scheme; /* get the encode type	*/

	/* Allocate a VFB of the required size and type */
		   if (header.viff_num_data_bands==3) /* RGB */
		   {
			if ( (vfb=ImVfbAlloc (w,h,IMVFBRGB)) == IMVFBNULL)
				ImErrorFatal (ImQError(), -1, IMEMALLOC);	
		   }
		   else if (header.viff_data_storage_type==IMVFF_TYP_1_BYTE) 
		   {  /* index8  */
			if ( (vfb=ImVfbAlloc (w,h,IMVFBINDEX8)) == IMVFBNULL)
				ImErrorFatal (ImQError(), -1, IMEMALLOC);	
		   }
		   else if ( header.viff_data_storage_type == IMVFF_TYP_BIT)
		   { /* monochrome image */
		        if ( (vfb=ImVfbAlloc (w,h,IMVFBMONO)) == IMVFBNULL)
				ImErrorFatal (ImQError(), -1, IMEMALLOC);	
		   }
		   else if (header.viff_data_storage_type==IMVFF_TYP_2_BYTE) 
		   {  /* index16  */
			if ( (vfb=ImVfbAlloc (w,h,IMVFBINDEX16)) == IMVFBNULL)
				ImErrorFatal (ImQError(), -1, IMEMALLOC);	
		   }
		

	if (header.viff_map_scheme)
	{
	   colortable=1;
	   n= header.viff_map_col_size;

               sprintf (message, "%d Entries",header.viff_map_col_size);
                ImInfo ("Color Table",message);

	   if ( (m=header.viff_map_storage_type)!=IMVFF_MAPTYP_1_BYTE)
		{
		   sprintf (message, "Map storage type %f invalid",
			header.viff_map_storage_type);
		   ImErrorFatal (ImQError(), -1, IMEMALLOC);
		}

  	   if ((clt = ImCltAlloc (n)) == IMCLTNULL)
           ImErrorFatal (ImQError(), -1, IMEMALLOC);

           ImMalloc ( buffer, unsigned char *, sizeof(unsigned char)*n*3);
           pColor = ImCltQFirst (clt);

	   if (header.viff_map_row_size==3)
	   {
                if (ImBinRead (ioType,fd,fp,buffer, UCHAR, 1, n*3)== -1)
                {
                   free ( (char *)buffer);
                   ImReturnBinError();
                }
		for (i= 0; i<n; i++) 
		{
		ImCltSRed (pColor, buffer[i]);
		ImCltSInc(clt,pColor);
		}

       	        pColor = ImCltQFirst (clt);
		for (i= 0; i<n; i++) 
		{
		ImCltSGreen (pColor, buffer[i+n]);
		ImCltSInc(clt,pColor);
		}

       	        pColor = ImCltQFirst (clt);
		for (i= 0; i<n; i++) 
		{
		ImCltSBlue (pColor, buffer[i+(2*n)]);
		ImCltSInc(clt,pColor);
		}
	   }
	
/* I am pretty sure next lines are unneeded */
	   if (header.viff_map_row_size==1)
	   {
                if (ImBinRead (ioType,fd,fp,buffer, UCHAR, 1, n*1)== -1)
                {
                   free ( (char *)buffer);
                   ImReturnBinError();
                }
                for (i =0; i<n; i++)
                {
                     ImCltSRed (pColor, buffer[i]);
                     ImCltSGreen(pColor, buffer[i]);
                     ImCltSBlue (pColor, buffer[i]);
                     ImCltSInc (clt, pColor);
                }
	   }
           free ( (char *)buffer);
	}	

        /* Send things for printout to iminfo if verbose flag is set */


        if (header.viff_data_encode_scheme==IMVFF_DES_RAW)
                sprintf (message,"none");
        if (header.viff_data_encode_scheme==IMVFF_DES_COMPRESS)
                sprintf (message,"ALZ");
        if (header.viff_data_encode_scheme==IMVFF_DES_RLE)
                sprintf (message,"Run Lenght Encoded (RLE)");
        if (header.viff_data_encode_scheme==IMVFF_DES_TRANSFORM)
                sprintf (message,"Transform based");
        if (header.viff_data_encode_scheme==IMVFF_DES_CCITT)
                sprintf (message,"CCITT standard");
        if (header.viff_data_encode_scheme==IMVFF_DES_ADPCM)
                sprintf (message,"ADPCM");
        if (header.viff_data_encode_scheme==IMVFF_DES_GENERIC)
                sprintf (message,"User Specified (generic)");
        ImInfo ("Compression Type",message);


	q=header.viff_num_data_bands;
	/* now we read the image pixel data in from our data file */
	switch (header.viff_data_storage_type)
	{
	   case IMVFF_TYP_BIT:		/* one monochrome bit per pixel */
		imReadViffBit (ioType, fd, fp, vfb, q,h, w,c);
		break;
	   case IMVFF_TYP_1_BYTE:		/* one byte per pixel	*/
		imReadViff1Byte (ioType, fd, fp, vfb, q, h,w,c);
		break;
	   case IMVFF_TYP_2_BYTE:
		imReadViff2Byte (ioType, fd, fp, vfb, q, h,w,c);
		break;
	}

        /* Attach name field to vfb is there is one */
	if (strlen (header.viff_comment) >1)
        {
		ImMalloc (viffName, char *, strlen (header.viff_comment)+1);
		strcpy(viffName,header.viff_comment);
	TagTableAppend (tagTable,TagEntryAlloc("image name",POINTER,&viffName));        }

        /* Attach (set) the vfb's color lookup table if one */
        if (colortable) ImVfbSClt(vfb, clt);

        /* Attach vfb to tag table */
        TagTableAppend (tagTable, TagEntryAlloc ("image vfb", POINTER, &vfb));
        return(1);
}


/*
 *  FUNCTION
 *    imReadViff1Byte - read a file with 1 byte per index that could have
 *	1 or 3 bytes of info per pixel.
 *    1 byte per pixel info is index8
 *    3 bytes per pixel info is RGB 
 *
 */
static int     /* Returns 1 on successful read */
#ifdef __STDC__
imReadViff1Byte (int ioType, int fd, FILE *fp, ImVfb *vfb, int q, unsigned long h, unsigned long w, unsigned long c)
#else
imReadViff1Byte (ioType,fd,fp,vfb, q,h,w,c)
        int   ioType;           /* I/O flags                    */
        int   fd;               /* Input file descriptor        */
        FILE  *fp;              /* Input file pointer           */
        ImVfb *vfb;             /* Read in Image                */
        int   q;                /* How many bits per pixel      */
        unsigned long   h;                /* Height of image              */
        unsigned long   w;                /* width of image               */
	unsigned long	c;	/* compression type		*/
#endif
{
        long         *bufferl;          /* Run buffer (type long)       */
        long *pBufferl;        /* bufferl pointer              */
        ImVfbPtr pPixel;       /*VFB pixel pointer     */
        int   x,y,z;              /* Counters                     */
        unsigned char *buffer;          /* Run buffer (type uchar)      */
        unsigned char *pBuffer;        /* buffer pointer       */
        int   total;            /* Counter for number of bytes encode */
        int   count;            /* Number of bytes to decode    */
        unsigned char   rle;            /* wether something is rle or not       */
        int   field;            /* red,green, or blue color     */
        unsigned char   leadByte;       /* Read in the lead byte        */
        int     expred;                 /* The five red bits            */
        int     expgreen;               /* The five green bits          */
        int     expblue;                /* The five blue bits           */

        pPixel = ImVfbQFirst(vfb);
                if (q==1)   /* one byte per index */
               {
		if (c==IMVFF_DES_RAW)
		{
                   ImMalloc (buffer, unsigned char *, w * sizeof(unsigned char) );
                   pPixel = ImVfbQFirst (vfb);
                   for (y =0; y<h; y++)
                   {
                      if (ImBinRead (ioType,fd,fp,buffer, UCHAR, 1,w) == -1)
                      {
                        free ( (char *)buffer);
                        ImReturnBinError();
                      }
                      pBuffer = buffer;
                      for (x=0; x<w; x++)
                      {
                         ImVfbSIndex8(vfb, pPixel,  *pBuffer++ );
                         ImVfbSInc(vfb, pPixel);
                      }
                   }
                    free ( (char *) buffer);
                }
		else {
        	   total=(h*w*1);  /* a running count of bytes encode */
        	   field =0;               /* Start with the red pixel     */

           	   ImMalloc (buffer, unsigned char *, 129);
           	   do {
                	   if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,1) == -1)
                	   {
                   	   free ( (char *)buffer);
                   	   ImReturnBinError();
                	   }

                	   pBuffer = buffer;
                   	   leadByte = (*pBuffer++);
                   	   rle = (leadByte&0x80);       /* rle = first bit   */
                   	   count = (leadByte&0x7F)+1; /*count=last 7 bytes +1 */

                   	   if (rle)     /* run length encoded packet */
                   	   {
                            if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,1) == -1)
                        	   {
                           	   free ( (char *)buffer);
                           	   ImReturnBinError();
                        	   }
                        	   pBuffer = buffer;
                        	   for (y=0; y<count; y++)
                        	   {
                           	   ImVfbSIndex8(vfb,pPixel, *pBuffer);
                           	   ImVfbSInc(vfb,pPixel);
                        	   }
                   	   }
                   	   else         /* non rleencoded packet        */
                   	   {
                        if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,count) == -1)                        {
                        	free ( (char *)buffer);
                           	ImReturnBinError();
                        }
                       	    	pBuffer = buffer;
				for (y=0; y<count; y++)
                        	{
                           	ImVfbSIndex8(vfb,pPixel,*pBuffer++);
                           	ImVfbSInc(vfb,pPixel);
                        	}
                   	   }

                  
                	total-=count;
            	} while (total>0);
            	free ( (char *)buffer);
	  } 
	}


		else
		{
		if (c==IMVFF_DES_RAW)
		{

                   ImMalloc (buffer, unsigned char *, w * sizeof(unsigned char) );

                   pPixel = ImVfbQFirst (vfb);
                   for (y =0; y<h; y++)
                   {
                      if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,1*w) == -1)
                      {
                        free ( (char *)buffer);
                        ImReturnBinError();
                      }
                      pBuffer = buffer;
                      for (x=0; x<w; x++)
                      {
                         ImVfbSRed(vfb, pPixel,  *pBuffer++ );
                         ImVfbSInc(vfb, pPixel);
                      }
                    }

                   pPixel = ImVfbQFirst (vfb);
                   for (y =0; y<h; y++)
                   {
                      if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,1*w) == -1)
                      {
                        free ( (char *)buffer);
                        ImReturnBinError();
                      }
                      pBuffer = buffer;
                      for (x=0; x<w; x++)
                      {
                         ImVfbSGreen(vfb, pPixel,  *pBuffer++ );
                         ImVfbSInc(vfb, pPixel);
                      }
		    }

                   pPixel = ImVfbQFirst (vfb);
                   for (y =0; y<h; y++)
                   {
                      if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,1*w) == -1)
                      {
                        free ( (char *)buffer);
                        ImReturnBinError();
                      }
                      pBuffer = buffer;
                      for (x=0; x<w; x++)
                      {
                         ImVfbSBlue(vfb, pPixel,  *pBuffer++ );
                         ImVfbSInc(vfb, pPixel);
                      }
                    }
                    free ( (char *) buffer);
		}
		else {
           ImMalloc (buffer, unsigned char *, 390);
           total=h*w*1;         /* a running count of bytes decode */
           do {
                if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,1) == -1)
                {
                   free ( (char *)buffer);
                   ImReturnBinError();
                }

                pBuffer = buffer;

                   leadByte = (*pBuffer++);
                   rle = (leadByte&0x80);       /* rle = first bit   */
                   count = (leadByte&0x7F)+1;  /*count = last 7 bytes +1 */

                   if (rle)     /* run length encoded packet */
                   {
                        if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,3) == -1)
                        {
                           free ( (char *)buffer);
                           ImReturnBinError();
                        }
                        pBuffer = buffer;
                        for (y=0; y<count; y++)
                        {
                           ImVfbSRed (vfb,pPixel, *pBuffer);
                           ImVfbSGreen(vfb,pPixel, *(pBuffer+1));
                           ImVfbSBlue (vfb,pPixel, *(pBuffer+2) );
                           ImVfbSInc (vfb,pPixel);
                        }
                   }

                  else         /* non rleencoded packet        */
                   {
                        if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,count*3) == -1)
                        {
                           free ( (char *)buffer);
                           ImReturnBinError();
                        }
                        pBuffer = buffer;
                        for (y=0; y<count; y++)
                        {
                           ImVfbSRed (vfb,pPixel, *pBuffer++ );
                           ImVfbSGreen(vfb,pPixel, *pBuffer++ );
                           ImVfbSBlue (vfb,pPixel, *pBuffer++ );
                           ImVfbSInc (vfb,pPixel);
                        }
                   }

                  
                total-=count;
            } while (total>0);
            free ( (char *)buffer);
    
		}
	}
	return(1);
}
/*
 *  FUNCTION
 *    imReadViff2Byte - read a file with 4 byte per index that could have
 *	1 or 3 bytes of info per pixel.
 *    1 byte per pixel info is index8
 *    3 bytes per pixel info is RGB 
 *
 */
static int     /* Returns 1 on successful read */
#ifdef __STDC__
imReadViff2Byte (int ioType, int fd, FILE *fp, ImVfb *vfb, int q, unsigned long h, unsigned long w, unsigned long c)
#else
imReadViff2Byte (ioType,fd,fp,vfb, q,h,w,c)
        int   ioType;           /* I/O flags                    */
        int   fd;               /* Input file descriptor        */
        FILE  *fp;              /* Input file pointer           */
        ImVfb *vfb;             /* Read in Image                */
        int   q;                /* How many bits per pixel      */
        unsigned long   h;      /* Height of image              */
        unsigned long   w;      /* width of image               */
	unsigned long	c;	/* compression type		*/
#endif
{
        ImVfbPtr pPixel;       /*VFB pixel pointer     */
        int   x,y,z;              /* Counters                     */
        sdsc_uint16 *buffer;          /* Run buffer (type unsigned char)      */
        sdsc_uint16 *pBuffer;        /* buffer pointer       */

        BinByteOrder (BINLBF);
        pPixel = ImVfbQFirst(vfb);
                if (q==1)    /* one byte per index */
               {
		if (c==IMVFF_DES_RAW)
		{
                   ImMalloc (buffer, sdsc_uint16 *, w* sizeof(sdsc_uint16) );
                   for (y =0; y<h; y++)
                   {
                      if (ImBinRead (ioType,fd,fp,buffer, UINT16, 2,w*1) == -1)
                      {
                        free ( (sdsc_uint16 *)buffer);
                        ImReturnBinError();
                      }
                      pBuffer = buffer;
                      for (x=0; x<w; x++)
                      {
                         ImVfbSIndex16(vfb,pPixel,*pBuffer++ );
                         ImVfbSInc(vfb, pPixel);
                      }
                   }
                    free ( (sdsc_uint16 *) buffer);
                }
	  	} 
	return(1);
}




/*
 *  FUNCTION
 *    imReadViffBit - read a file with 1 bit per index - monochrome 
 *
 */
static int     /* Returns 1 on successful read */
#ifdef __STDC__
imReadViffBit (int ioType, int fd, FILE *fp, ImVfb *vfb, int q, unsigned long h, unsigned long w, unsigned long c)
#else
imReadViffBit (ioType,fd,fp,vfb, q,h,w,c)
        int   ioType;           /* I/O flags                    */
        int   fd;               /* Input file descriptor        */
        FILE  *fp;              /* Input file pointer           */
        ImVfb *vfb;             /* Read in Image                */
        int   q;                /* How many bits per pixel      */
        unsigned long   h;                /* Height of image              */
        unsigned long   w;                /* width of image               */
	unsigned long	c;	/* compression type		*/
#endif
{
        long         *bufferl;          /* Run buffer (type long)       */
        long *pBufferl;        /* bufferl pointer              */
        ImVfbPtr pPixel;       /*VFB pixel pointer     */
        int   x,y,z;              /* Counters                     */
        unsigned char *buffer;          /* Run buffer (type uchar)      */
        unsigned char *pBuffer;        /* buffer pointer       */
        int   total;            /* Counter for number of bytes encode */
        int   count;            /* Number of bytes to decode    */
        unsigned char   rle;            /* wether something is rle or not       */
        int   field;            /* red,green, or blue color     */
        unsigned char   leadByte;       /* Read in the lead byte        */
	unsigned char   pixel;
	int	regular;

	switch (c)
	{
		case (IMVFF_DES_RAW):
		   regular = ((w+7)/8);
		   ImMalloc (buffer, unsigned char *, regular);
		   pPixel = ImVfbQFirst(vfb);
		   for (y=0;y<h;y++)
		   {
			count=0;
                      if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,regular) == -1)
                      {
                        free ( (char *)buffer);
                        ImReturnBinError();
                      }
		    for (x=0;x<regular;x++)
		    {
		        pixel = buffer[x];
			for (z=0;z<8;z++)
			{
			   if (count<w){
			   ImVfbSMono (vfb, pPixel, (pixel&0x01) );
			   ImVfbSInc (vfb, pPixel);
			   pixel >>=1;
			   count++;
					}
			}
	            }
		   }
		   free ( (char *) buffer);
		break;
	}
	return(1);
}
/*  FUNCTION
 *	imViffWriteMONO	- write a Viff monochrome file 
 *
 *  DESCRIPTION
 *      
 */
static int
#ifdef __STDC__
imViffWriteMONO (ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)
#else
imViffWriteMONO (pMap, ioType, fd, fp, flagsTable, tagTable)
	ImFileFormatWriteMap *pMap;     /* Write map entry to adhear to */
        int          ioType;            /* I/O flags                    */
        int          fd;                /* Input file descriptor        */
        FILE        *fp;                /* Input file pointer           */
        TagTable   *flagsTable;         /* Flags                        */
        TagTable    *tagTable;          /* Tag table to read from       */
#endif
{
        ImVfb        *vfb;              /* Read in image                */
        ImVfbPtr     pptr;     /* Pixel pointer                */
        unsigned char      *buffer;             /* line buffer                  */
        unsigned char      *pBuffer;            /* current location in  buffer  */
        int          w,h,n,x,y,z,i,j;         /* Counter                      */
        TagEntry    *tagEntry;          /* Tmp table entry              */
	unsigned char	    temp;		/* byte to pack with bits	*/
	int	    count;		/* count of number of bits 	*/
        unsigned long numDataBands;       /* how many data bands it is 	*/
	unsigned long dataStorageType;	/* storage type of disk data	*/
	unsigned long dataEncodeScheme; /* encodingscheme of disk data  */
	unsigned long mapScheme;	/* how mapping is to happen	*/
	unsigned long colorSpaceModel;	/* how mapping is to happen	*/
	unsigned long mapRowSize;	/* wether a RGB or greyscale map */
	int	regular;
	unsigned char pixel;


	BinByteOrder (BINLBF);
	mapRowSize =0;
        numDataBands =1;
        dataStorageType = IMVFF_TYP_BIT;
        dataEncodeScheme = IMVFF_DES_RAW;
        mapScheme = IMVFF_MS_NONE;
        colorSpaceModel = IMVFF_CM_NONE;

        imViffWriteHeader (ioType, fd, fp, flagsTable, tagTable,
                numDataBands, dataStorageType, dataEncodeScheme, mapScheme,
                colorSpaceModel,mapRowSize);

        TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);
	w = ImVfbQWidth(vfb);
        h = ImVfbQHeight(vfb);

        /* allocate buffer space  for (width*length*3) scan lines */

	regular = ((w+7)/8);
	ImMalloc (buffer, unsigned char *, regular);
        pptr = ImVfbQFirst (vfb);
	for (y=0;y<h;y++)
	{
        pBuffer = buffer;
	    count=0;
	    for (x=0;x<regular;x++)
	    {
	  	  pixel=0;	
		for (z=0; z<8; z++)
		{
		if (count<w){	
		    temp=ImVfbQMono(vfb,pptr);
		    temp = (temp&0x01)<<z;
		    ImVfbSInc (vfb, pptr);
		    pixel |= temp;
		    count++;
			    }
		}
		*pBuffer++=pixel;
	    }
            if (ImBinWrite (ioType, fd, fp, buffer, UCHAR,1, regular) ==-1)
            {
                   free ( (char *)buffer);
                   ImReturnBinError( );
            }
	}

	free ( (char *) buffer);
        return(1);
}




/*  FUNCTION
 *	imViffWriteINDEX8CLT	- write a Viff RGB file that has a clt
 *
 *  DESCRIPTION
 *      
 */
static int
#ifdef __STDC__
imViffWriteINDEX8CLT (ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)
#else
imViffWriteINDEX8CLT (pMap, ioType, fd, fp, flagsTable, tagTable)
	ImFileFormatWriteMap *pMap;     /* Write map entry to adhear to */
        int          ioType;            /* I/O flags                    */
        int          fd;                /* Input file descriptor        */
        FILE        *fp;                /* Input file pointer           */
        TagTable   *flagsTable;         /* Flags                        */
        TagTable    *tagTable;          /* Tag table to read from       */
#endif
{
        unsigned long numDataBands;       /* how many data bands it is 	*/
	unsigned long dataStorageType;	/* storage type of disk data	*/
	unsigned long dataEncodeScheme; /* encodingscheme of disk data  */
	unsigned long mapScheme;	/* how mapping is to happen	*/
	unsigned long colorSpaceModel;	/* how mapping is to happen	*/
	unsigned long mapRowSize;	/* wether a RGB or greyscale map */

	mapRowSize =3;
        numDataBands =1;
        dataStorageType = IMVFF_TYP_1_BYTE;
        dataEncodeScheme = IMVFF_DES_RAW;
        mapScheme = IMVFF_MS_ONEPERBAND;
        colorSpaceModel = IMVFF_CM_genericRGB;

        imViffWriteHeader (ioType, fd, fp, flagsTable, tagTable,
                numDataBands, dataStorageType, dataEncodeScheme, mapScheme,
                colorSpaceModel,mapRowSize);

        imViffWriteINDEX8Data (ioType, fd, fp, flagsTable, tagTable);

        return(1);
}

/*  FUNCTION
 *	imViffWriteINDEX8NOCLT	- write a Viff RGB file that has a clt
 *
 *  DESCRIPTION
 *      
 */
static int
#ifdef __STDC__
imViffWriteINDEX8NOCLT (ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)
#else
imViffWriteINDEX8NOCLT (pMap, ioType, fd, fp, flagsTable, tagTable)
	ImFileFormatWriteMap *pMap;     /* Write map entry to adhear to */
        int          ioType;            /* I/O flags                    */
        int          fd;                /* Input file descriptor        */
        FILE        *fp;                /* Input file pointer           */
        TagTable   *flagsTable;         /* Flags                        */
        TagTable    *tagTable;          /* Tag table to read from       */
#endif
{
        unsigned long numDataBands;       /* how many data bands it is 	*/
	unsigned long dataStorageType;	/* storage type of disk data	*/
	unsigned long dataEncodeScheme; /* encodingscheme of disk data  */
	unsigned long mapScheme;	/* how mapping is to happen	*/
	unsigned long colorSpaceModel;	/* how mapping is to happen	*/
	unsigned long mapRowSize;	/* wether a RGB or greyscale map */

	mapRowSize =0;
        numDataBands =1;
        dataStorageType = IMVFF_TYP_1_BYTE;
        dataEncodeScheme = IMVFF_DES_RAW;
        mapScheme = IMVFF_MS_NONE;
        colorSpaceModel = IMVFF_CM_NONE;

        imViffWriteHeader (ioType, fd, fp, flagsTable, tagTable,
                numDataBands, dataStorageType, dataEncodeScheme, mapScheme,
                colorSpaceModel, mapRowSize);

        imViffWriteINDEX8Data (ioType, fd, fp, flagsTable, tagTable);

        return(1);
}

/*  FUNCTION
 *	imViffWriteINDEX8Data	- write out RGB data
 *
 *  DESCRIPTION
 */
static int
#ifdef __STDC__
imViffWriteINDEX8Data (int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)
#else
imViffWriteINDEX8Data (ioType, fd, fp, flagsTable, tagTable)
        int          ioType;            /* I/O flags                    */
        int          fd;                /* Input file descriptor        */
        FILE        *fp;                /* Input file pointer           */
        TagTable    *flagsTable;
        TagTable    *tagTable;
#endif
{
        ImVfb        *vfb;              /* Read in image                */
        ImVfbPtr     pptr;     /* Pixel pointer                */
        unsigned char      *buffer;             /* line buffer                  */
        unsigned char      *pBuffer;            /* current location in  buffer  */
        int          n,x,y,i,j;         /* Counter                      */
        TagEntry    *tagEntry;          /* Tmp table entry              */

        BinByteOrder (BINLBF);

        TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);
	x = ImVfbQWidth(vfb);
        y = ImVfbQHeight(vfb);

        /* allocate buffer space  for (width*length*3) scan lines */
        ImMalloc (buffer, unsigned char *,x);
        pptr = ImVfbQFirst (vfb);

        for (j=0; j<y; j++)
        {

                pBuffer = buffer;
                for (i=0; i<x; i++)
                {
                        *pBuffer++ = ImVfbQIndex8 (vfb, pptr);
                        ImVfbSInc (vfb, pptr);
		}
              if (ImBinWrite (ioType, fd, fp, buffer, UCHAR,1,pBuffer - buffer) ==-1)
                {
                        free ( (char *)buffer);
                        ImReturnBinError( );
                }
	      free ( (char *) buffer);

	}
	return(1);
}


/*  FUNCTION
 *	imViffWriteINDEX16CLT	- write a Viff INDEX16 file that has a clt
 *
 *  DESCRIPTION
 *      
 */
static int
#ifdef __STDC__
imViffWriteINDEX16CLT (ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)
#else
imViffWriteINDEX16CLT (pMap, ioType, fd, fp, flagsTable, tagTable)
	ImFileFormatWriteMap *pMap;     /* Write map entry to adhear to */
        int          ioType;            /* I/O flags                    */
        int          fd;                /* Input file descriptor        */
        FILE        *fp;                /* Input file pointer           */
        TagTable   *flagsTable;         /* Flags                        */
        TagTable    *tagTable;          /* Tag table to read from       */
#endif
{
        unsigned long numDataBands;       /* how many data bands it is 	*/
	unsigned long dataStorageType;	/* storage type of disk data	*/
	unsigned long dataEncodeScheme; /* encodingscheme of disk data  */
	unsigned long mapScheme;	/* how mapping is to happen	*/
	unsigned long colorSpaceModel;	/* how mapping is to happen	*/
	unsigned long mapRowSize;	/* wether a RGB or greyscale map */

	mapRowSize =3;
        numDataBands =1;
        dataStorageType = IMVFF_TYP_2_BYTE;
        dataEncodeScheme = IMVFF_DES_RAW;
        mapScheme = IMVFF_MS_ONEPERBAND;
        colorSpaceModel = IMVFF_CM_genericRGB;

        imViffWriteHeader (ioType, fd, fp, flagsTable, tagTable,
                numDataBands, dataStorageType, dataEncodeScheme, mapScheme,
                colorSpaceModel,mapRowSize);

        imViffWriteINDEX16Data (ioType, fd, fp, flagsTable, tagTable);

        return(1);
}

/*  FUNCTION
 *	imViffWriteINDEX8NOCLT	- write a Viff RGB file that has a clt
 *
 *  DESCRIPTION
 *      
 */
static int
#ifdef __STDC__
imViffWriteINDEX16NOCLT (ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)
#else
imViffWriteINDEX16NOCLT (pMap, ioType, fd, fp, flagsTable, tagTable)
	ImFileFormatWriteMap *pMap;     /* Write map entry to adhear to */
        int          ioType;            /* I/O flags                    */
        int          fd;                /* Input file descriptor        */
        FILE        *fp;                /* Input file pointer           */
        TagTable   *flagsTable;         /* Flags                        */
        TagTable    *tagTable;          /* Tag table to read from       */
#endif
{
        unsigned long numDataBands;       /* how many data bands it is 	*/
	unsigned long dataStorageType;	/* storage type of disk data	*/
	unsigned long dataEncodeScheme; /* encodingscheme of disk data  */
	unsigned long mapScheme;	/* how mapping is to happen	*/
	unsigned long colorSpaceModel;	/* how mapping is to happen	*/
	unsigned long mapRowSize;	/* wether a RGB or greyscale map */

	mapRowSize =0;
        numDataBands =1;
        dataStorageType = IMVFF_TYP_2_BYTE;
        dataEncodeScheme = IMVFF_DES_RAW;
        mapScheme = IMVFF_MS_NONE;
        colorSpaceModel = IMVFF_CM_NONE;

        imViffWriteHeader (ioType, fd, fp, flagsTable, tagTable,
                numDataBands, dataStorageType, dataEncodeScheme, mapScheme,
                colorSpaceModel, mapRowSize);

        imViffWriteINDEX16Data (ioType, fd, fp, flagsTable, tagTable);

        return(1);
}

/*  FUNCTION
 *	imViffWriteINDEX16Data	- write out RGB data
 *
 *  DESCRIPTION
 */
static int
#ifdef __STDC__
imViffWriteINDEX16Data (int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)
#else
imViffWriteINDEX16Data (ioType, fd, fp, flagsTable, tagTable)
        int          ioType;            /* I/O flags                    */
        int          fd;                /* Input file descriptor        */
        FILE        *fp;                /* Input file pointer           */
        TagTable    *flagsTable;
        TagTable    *tagTable;
#endif
{
        ImVfb        *vfb;              /* Read in image                */
        ImVfbPtr     pptr;     /* Pixel pointer                */
        sdsc_uint16      *buffer;             /* line buffer                  */
        sdsc_uint16      *pBuffer;            /* current location in  buffer  */
        int          n,x,y,i,j;         /* Counter                      */
        TagEntry    *tagEntry;          /* Tmp table entry              */

        BinByteOrder (BINLBF);

        TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);
	x = ImVfbQWidth(vfb);
        y = ImVfbQHeight(vfb);

        /* allocate buffer space  for (width*length*3) scan lines */
        ImMalloc (buffer, sdsc_uint16 *, sizeof(sdsc_uint16)*x);
        pptr = ImVfbQFirst (vfb);

        for (j=0; j<y; j++)
        {

                pBuffer = buffer;
                for (i=0; i<x; i++)
                {
                        *pBuffer++ = ImVfbQIndex16 (vfb, pptr);
                        ImVfbSInc (vfb, pptr);
		}
              if (ImBinWrite (ioType, fd, fp, buffer, UINT16,2,pBuffer - buffer) ==-1)
                {
                        free ( (sdsc_uint16 *)buffer);
                        ImReturnBinError( );
                }
	}

        free ( (sdsc_uint16 *) buffer);
	return(1);
}


/*  FUNCTION
 *	imViffWriteRGBNOCLT	- write a Viff RGB file that has a clt
 *
 *  DESCRIPTION
 *      
 */
static int
#ifdef __STDC__
imViffWriteRGBNOCLT (ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)
#else
imViffWriteRGBNOCLT (pMap, ioType, fd, fp, flagsTable, tagTable)
	ImFileFormatWriteMap *pMap;     /* Write map entry to adhear to */
        int          ioType;            /* I/O flags                    */
        int          fd;                /* Input file descriptor        */
        FILE        *fp;                /* Input file pointer           */
        TagTable   *flagsTable;         /* Flags                        */
        TagTable    *tagTable;          /* Tag table to read from       */
#endif
{
        unsigned long numDataBands;       /* how many data bands it is 	*/
	unsigned long dataStorageType;	/* storage type of disk data	*/
	unsigned long dataEncodeScheme; /* encodingscheme of disk data  */
	unsigned long mapScheme;	/* how mapping is to happen	*/
	unsigned long colorSpaceModel;	/* how mapping is to happen	*/
	unsigned long mapRowSize;	/* wether a RGB or greyscale map */
	
	mapRowSize = 0;
	numDataBands =3;
	dataStorageType = IMVFF_TYP_1_BYTE;
	dataEncodeScheme = IMVFF_DES_RAW;
	mapScheme = IMVFF_MS_NONE;
	colorSpaceModel = IMVFF_CM_genericRGB;
	
	imViffWriteHeader (ioType, fd, fp, flagsTable, tagTable, 
		numDataBands, dataStorageType, dataEncodeScheme, mapScheme,
		colorSpaceModel,mapRowSize);

	imViffWriteRGBData (ioType, fd, fp, flagsTable, tagTable);

	return(1);
}



/*  FUNCTION
 *	imViffWriteRGBCLT	- write a Viff RGB file that has a clt
 *
 *  DESCRIPTION
 *      
 */
static int
#ifdef __STDC__
imViffWriteRGBCLT (ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)
#else
imViffWriteRGBCLT (pMap, ioType, fd, fp, flagsTable, tagTable)
	ImFileFormatWriteMap *pMap;     /* Write map entry to adhear to */
        int          ioType;            /* I/O flags                    */
        int          fd;                /* Input file descriptor        */
        FILE        *fp;                /* Input file pointer           */
        TagTable   *flagsTable;         /* Flags                        */
        TagTable    *tagTable;          /* Tag table to read from       */
#endif
{
        unsigned long numDataBands;       /* how many data bands it is 	*/
	unsigned long dataStorageType;	/* storage type of disk data	*/
	unsigned long dataEncodeScheme; /* encodingscheme of disk data  */
	unsigned long mapScheme;	/* how mapping is to happen	*/
	unsigned long colorSpaceModel;	/* how mapping is to happen	*/
	unsigned long mapRowSize;	/* wether a RGB or greyscale map */
	
	numDataBands =3;
	mapRowSize =3;
	dataStorageType = IMVFF_TYP_1_BYTE;
	dataEncodeScheme = IMVFF_DES_RAW;
	mapScheme = IMVFF_MS_ONEPERBAND;
	colorSpaceModel = IMVFF_CM_genericRGB;
	
	imViffWriteHeader (ioType, fd, fp, flagsTable, tagTable, 
		numDataBands, dataStorageType, dataEncodeScheme, mapScheme,
		colorSpaceModel, mapRowSize);

	imViffWriteRGBData (ioType, fd, fp, flagsTable, tagTable);

	return(1);
}


/*  FUNCTION
 *	imViffWriteRGBData	- write out RGB data
 *
 *  DESCRIPTION
 */
static int
#ifdef __STDC__
imViffWriteRGBData (int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)
#else
imViffWriteRGBData (ioType, fd, fp, flagsTable, tagTable)
        int          ioType;            /* I/O flags                    */
        int          fd;                /* Input file descriptor        */
        FILE        *fp;                /* Input file pointer           */
        TagTable    *flagsTable;
        TagTable    *tagTable;
#endif
{
        ImVfb        *vfb;              /* Read in image                */
        ImVfbPtr     pptr;     /* Pixel pointer                */
        unsigned char      *buffer;             /* line buffer                  */
        unsigned char      *pBuffer;            /* current location in  buffer  */
        int          n,x,y,i,j;         /* Counter                      */
        TagEntry    *tagEntry;          /* Tmp table entry              */

        BinByteOrder (BINLBF);

        TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);
	x = ImVfbQWidth(vfb);
        y = ImVfbQHeight(vfb);

        /* allocate buffer space  for (width*length*3) scan lines */
        ImMalloc (buffer, unsigned char *, x);
        pptr = ImVfbQFirst (vfb);

        for (j=0; j<y; j++)
        {

                pBuffer = buffer;
                for (i=0; i<x; i++)
                {
                        *pBuffer++ = ImVfbQRed (vfb, pptr);
                        ImVfbSInc (vfb, pptr);
		}
              if (ImBinWrite (ioType, fd, fp, buffer, UCHAR,1,pBuffer - buffer) ==-1)
                {
                        free ( (char *)buffer);
                        ImReturnBinError( );
                }

	}
        pptr = ImVfbQFirst (vfb);

        for (j=0; j<y; j++)
        {

                pBuffer = buffer;
                for (i=0; i<x; i++)
                {
                        *pBuffer++ = ImVfbQGreen (vfb, pptr);
                        ImVfbSInc (vfb, pptr);
		}
              if (ImBinWrite (ioType, fd, fp, buffer, UCHAR,1,pBuffer - buffer) ==-1)
                {
                        free ( (char *)buffer);
                        ImReturnBinError( );
                }

	}
        pptr = ImVfbQFirst (vfb);

        for (j=0; j<y; j++)
        {

                pBuffer = buffer;
                for (i=0; i<x; i++)
                {
                        *pBuffer++ = ImVfbQBlue (vfb, pptr);
                        ImVfbSInc (vfb, pptr);
		}
              if (ImBinWrite (ioType, fd, fp, buffer, UCHAR,1,pBuffer - buffer) ==-1)
                {
                        free ( (char *)buffer);
                        ImReturnBinError( );
                }

	}

        free ( (char *)buffer);
        return (1);
}



/*
 *  FUNCTION
 *      imViffWriteHeader        -  write a Viff file header
 *
 *  DESCRIPTION
 *      Based on info passed to it a standard Viff file header written out.
 *      If there is a color lookup table then it too is written out.
 *      If there is a name field it too is written out.
 */
static int                             /* Returns # of tags used       */
#ifdef __STDC__
imViffWriteHeader( int ioType, int fd, FILE *fp, TagTable *flagsTable,  TagTable *tagTable,unsigned long numDataBands,
		unsigned long dataStorageType, unsigned long dataEncodeScheme, unsigned long mapScheme,
		unsigned long colorSpaceModel, unsigned long mapRowSize )
#else
imViffWriteHeader( ioType, fd, fp,flagsTable, tagTable,numDataBands,
		dataStorageType, dataEncodeScheme, mapScheme,
		colorSpaceModel, mapRowSize )
        int          ioType;            /* I/O flags                    */
        int          fd;                /* Input file descriptor        */
        FILE        *fp;                /* Input file pointer           */
        TagTable    *flagsTable;
        TagTable    *tagTable;
        unsigned long numDataBands;       /* how many data bands it is 	*/
	unsigned long dataStorageType;	/* storage type of disk data	*/
	unsigned long dataEncodeScheme; /* encodingscheme of disk data  */
	unsigned long mapScheme;	/* how mapping is to happen	*/
	unsigned long colorSpaceModel;	/* how mapping is to happen	*/
	unsigned long mapRowSize;	/* wether it is a RGB or greyscale map*/
#endif
{
        ImVfb        *vfb;              /* Read in image                */
        ImVfbPtr     pptr;     /* Pixel pointer                */
        ImClt           *clt;           /* CLT pointer                  */
        ImCltPtr        pColor;
        imViffHeaderInfo  header;        /* Tga file header              */
        unsigned char      *buffer;             /* line buffer                  */
        unsigned char      *pBuffer;            /* current location in  buffer  */
        int          n,x,y,i,j;         /* Counter                      */
        TagEntry    *tagEntry;          /* Tmp table entry              */
        char        *tmpString;         /* Tmp string holder            */
	char		message[1024];
        /* Set up and write out the header */
        BinByteOrder (BINLBF);

        TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);

        clt = ImVfbQClt(vfb);

	memset ( (void *) &header,0x00, sizeof(header));
	header.viff_identifier=171;
	header.viff_file_type=1;
	header.viff_release=IM_XV_IMAGE_REL_NUM;
	header.viff_version=IM_XV_IMAGE_VER_NUM;
	header.viff_machine_dep=IMVFF_DEP_NSORDER;
	strcpy(header.viff_trash,"not");
/* Get the name field if there is one  */
        tagEntry = TagTableQDirect (tagTable, "image name",0);
        if (tagEntry == TAGENTRYNULL) strcpy (header.viff_comment,"no name");
        else
        {
                TagEntryQValue(tagEntry, &tmpString);
		strncpy (header.viff_comment,tmpString,512);
                ImInfo ("Image Name",header.viff_comment);
        }
	header.viff_row_size=ImVfbQWidth(vfb);
	header.viff_col_size=ImVfbQHeight(vfb);
	header.viff_subrow_size=0;
	header.viff_startx=0;
	header.viff_starty=0;
	header.viff_pixsizx=0.0;
	header.viff_pixsizy=0.0;
	header.viff_location_type=IMVFF_LOC_IMPLICIT;
	header.viff_location_dim=0;
	header.viff_num_of_images=1;
	header.viff_num_data_bands=numDataBands;
	header.viff_data_storage_type=dataStorageType;
	header.viff_data_encode_scheme=dataEncodeScheme;
	header.viff_map_scheme=mapScheme;
	header.viff_map_storage_type=IMVFF_MAPTYP_1_BYTE;
	header.viff_map_row_size=mapRowSize;
	if ( (clt==IMCLTNULL) || (mapRowSize==0) )
				 header.viff_map_col_size =0;					else header.viff_map_col_size=ImCltQNColors(clt);
	header.viff_map_subrow_size=0;
	if ((numDataBands==3) & (colorSpaceModel==IMVFF_CM_genericRGB))
		header.viff_map_enable = IMVFF_MAP_OPTIONAL;
	else if (colorSpaceModel==IMVFF_MS_NONE)
			 header.viff_map_enable = IMVFF_MAP_OPTIONAL;
	else header.viff_map_enable = IMVFF_MAP_FORCE;
	header.viff_maps_per_cycle=0;
	header.viff_color_space_model=colorSpaceModel;
	header.viff_ispare1=0;
	header.viff_ispare2=0;
	header.viff_fspare1=0.0;
	header.viff_fspare2=0.0;
	strcpy( header.viff_reserve,"nothing");
	header.viff_maps=0;
	header.viff_location=0;
	header.viff_imagedata=0;

        if (ImBinWriteStruct (ioType, fd, fp, &header, imViffHeaderFields) ==-1)
                ImReturnBinError( );

        sprintf (message, "%d",header.viff_release);
        ImInfo ("Release",message);

        sprintf (message, "%d",header.viff_version);
        ImInfo ("Version",message);

        if (header.viff_machine_dep==IMVFF_DEP_IEEEORDER)
        sprintf (message, "Most Significant Byte First");
        else sprintf (message, "Least Significant Byte First");
        ImInfo ("Byte Order",message);

        sprintf (message, "%d",header.viff_num_data_bands);
        ImInfo ("Number of Data Bands",message);

        sprintf (message, "%d x %d",header.viff_row_size, header.viff_col_size);        ImInfo ("Resolution",message);

        if (header.viff_data_storage_type==IMVFF_TYP_BIT)
                sprintf (message, "1-bit Monochrome");

        if (header.viff_data_storage_type==IMVFF_TYP_1_BYTE)
        {
           if (header.viff_num_data_bands==1)
             sprintf (message, "%d-bit Color Indexed",header.viff_num_data_bands*8);
           else sprintf (message, "%d-bit RGB",header.viff_num_data_bands*8);
        }

        if (header.viff_data_storage_type==IMVFF_TYP_2_BYTE)
        {
           if (header.viff_num_data_bands==1)
             sprintf (message, "%d-bit Color Indexed",header.viff_num_data_bands*16);
           else sprintf (message, "%d-bit RGB",header.viff_num_data_bands*16);
        }
        ImInfo ("Type",message);

	if ( (mapScheme!=IMVFF_MS_NONE) & (clt!=IMCLTNULL))
        {
                ImMalloc (buffer, unsigned char *, mapRowSize*header.viff_map_col_size)
                pColor = ImCltQFirst(clt);
                pBuffer = buffer;

                for (j=0; j<header.viff_map_col_size; j++)
                {
                   *pBuffer++ = ImCltQRed( pColor);
                   ImCltSInc(clt, pColor);
		}

                pColor = ImCltQFirst(clt);
                for (j=0; j<header.viff_map_col_size; j++)
                {
                   *pBuffer++ = ImCltQGreen( pColor);
                   ImCltSInc(clt, pColor);
		}

                pColor = ImCltQFirst(clt);
                for (j=0; j<header.viff_map_col_size; j++)
                {
                   *pBuffer++ = ImCltQBlue( pColor);
                   ImCltSInc(clt, pColor);
		}
                if (ImBinWrite (ioType, fd, fp, buffer, UCHAR,1,pBuffer - buffer) ==-1)
                {
                 free ( (char *)buffer);
                 ImReturnBinError( );
                }

		free ( ( char *)buffer);
                sprintf (message, "%d Entries",header.viff_map_col_size);
                ImInfo ("Color Table",message);

        }
        /* Send things for printout to iminfo if verbose flag is set */


        if (header.viff_data_encode_scheme==IMVFF_DES_RAW)
                sprintf (message,"none");
        if (header.viff_data_encode_scheme==IMVFF_DES_COMPRESS)
                sprintf (message,"ALZ");
        if (header.viff_data_encode_scheme==IMVFF_DES_RLE)
                sprintf (message,"Run Lenght Encoded (RLE)");
        if (header.viff_data_encode_scheme==IMVFF_DES_TRANSFORM)
                sprintf (message,"Transform based");
        if (header.viff_data_encode_scheme==IMVFF_DES_CCITT)
                sprintf (message,"CCITT standard");
        if (header.viff_data_encode_scheme==IMVFF_DES_ADPCM)
                sprintf (message,"ADPCM");
        if (header.viff_data_encode_scheme==IMVFF_DES_GENERIC)
                sprintf (message,"User Specified (generic)");
        ImInfo ("Compression Type",message);

        return(1);
}
