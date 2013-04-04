/**

 **	$Header: /sdsc/dev/vis/image/imtools/v3.0/libim/src/include/RCS/imhdfinternal.h,v 1.5 1995/06/29 00:32:03 bduggan Exp $

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

 **	imhdfinternal.h	-  HDF image file I/O include

 **

 **  PROJECT

 **	libim		-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imhdfinternal.h contains macros and structure definitions used

 **	by the HDF read and write code of the image library.

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **	IMHDFT*		d  tag numbers

 **	IMHDFNT*	d  MT and NT values

 **	IMHDFC*		d  color formats

 **

 **	RED, GREEN, BLUE d  RGB flags

 **

 **	IMHDFTMPFILE	d  tmp file for holding stream data

 **

 **	imHdfDD		t  Data Descriptor entry from an HDF file

 **

 **	imHdfDDList	v  list of DDs

 **	imHdfDDListEnd	v  pointer to end of DD list

 **	imHdfDDCount	v  # of entries in DD list

 **

 **	imHdfDDQNEntry	m  query number of entries in DD list

 **	imHdfDDQTag	m  query tag for a DD entry

 **	imHdfDDQRef	m  query reference number for a DD entry

 **	imHdfDDQDataOffset	m  query data offset for a DD entry

 **	imHdfDDQDataLength	m  query data length for a DD entry

 **	imHdfDDQNext	m  query next in DD list

 **	imHdfDDQFirst	m  query first in DD list

 **

 **	imHdfClt	t  CLT that has been written out

 **

 **	imHdfCltList	v  list of CLT's being written out

 **	imHdfCltListEnd	v  end of the CLT list

 **

 **	imHdfCltQRefLUT	m  query LUT reference number for an entry

 **	imHdfCltQRefLD	m  query LD reference number for an entry

 **	imHdfCltQClt	m  query CLT pointer for an entry

 **

 **	ReadStruct	m  read a structure

 **	WriteStruct	m  write a structure

 **	Read		m  read an item

 **	Write		m  write an item

 **	Seek		m  seek to a file location

 **	Tell		m  tell current file locaiton

 **

 **	imHdfByteOrder	v  data byte order

 **	imHdfFloatFormat v  data float format

 **	imHdfRef	v  current reference number

 **

 **	imHdfDim	t  dimmension information

 **	imHdfRIG	t  RIG information collection

 **

 **  PRIVATE CONTENTS

 **	none

 **

 **  HISTORY

 **	$Log: imhdfinternal.h,v $

 **	Revision 1.5  1995/06/29  00:32:03  bduggan

 **	updated copyright

 **

 **	Revision 1.4  1994/10/03  16:03:03  nadeau

 **	Updated to ANSI C and C++ compatibility by adding function prototypes.

 **	Minimized use of custom SDSC types (e.g., uchar vs. unsigned char)

 **	Changed all macros and defined constants to have names starting with IM.

 **	Updated copyright message.

 **

 **	Revision 1.3  92/09/02  13:18:52  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.2  91/10/03  13:04:37  nadeau

 **	Changed 'interlace' to 'interleave'.

 **	

 **	Revision 1.1  91/10/03  12:56:45  nadeau

 **	Initial revision

 **	

 **/



#ifndef __IMHDFINTERNALH__











/*

 *  CONSTANTS

 *	IMHDFT*	-  tag numbers

 *

 *  DESCRIPTION

 *	Each tag in an HDF file is represented as an unsigned 16-bit number.

 *	The following #defines are derived from "df.h", an include file

 *	from the NCSA HDF library source.  We can't include "df.h" itself

 *	here, or we'd have to distribute "df.h" without our source.

 */



#define IMHDFTNULL  ((sdsc_uint16)1)	/* no data				*/



/* Utility set								*/

#define IMHDFTRLE   ((sdsc_uint16)11)  /* run length encoding		*/

#define IMHDFTIMC   ((sdsc_uint16)12)  /* IMCOMP compression		*/

#define IMHDFTFID   ((sdsc_uint16)100) /* File identifier			*/

#define IMHDFTFD    ((sdsc_uint16)101) /* File description		*/

#define IMHDFTTID   ((sdsc_uint16)102) /* Tag identifier			*/

#define IMHDFTTD    ((sdsc_uint16)103) /* Tag descriptor			*/

#define IMHDFTDIL   ((sdsc_uint16)104) /* data identifier label		*/

#define IMHDFTDIA   ((sdsc_uint16)105) /* data identifier annotation	*/

#define IMHDFTNT    ((sdsc_uint16)106) /* number type			*/

#define IMHDFTMT    ((sdsc_uint16)107) /* machine type			*/



/* Raster-8 set								*/

#define IMHDFTID8   ((sdsc_uint16)200) /* 8-bit Image dimension		*/

#define IMHDFTIP8   ((sdsc_uint16)201) /* 8-bit Image palette		*/

#define IMHDFTRI8   ((sdsc_uint16)202) /* Raster-8 image			*/

#define IMHDFTCI8   ((sdsc_uint16)203) /* RLE compressed 8-bit image	*/

#define IMHDFTII8   ((sdsc_uint16)204) /* IMCOMP compressed 8-bit image	*/



/* Raster Image set							*/

#define IMHDFTID    ((sdsc_uint16)300) /* Image DimRec			*/

#define IMHDFTLUT   ((sdsc_uint16)301) /* Image Palette			*/

#define IMHDFTRI    ((sdsc_uint16)302) /* Raster Image			*/

#define IMHDFTCI    ((sdsc_uint16)303) /* Compressed Image		*/



#define IMHDFTRIG   ((sdsc_uint16)306) /* Raster Image Group		*/

#define IMHDFTLD    ((sdsc_uint16)307) /* Palette DimRec			*/

#define IMHDFTMD    ((sdsc_uint16)308) /* Matte DimRec			*/

#define IMHDFTMA    ((sdsc_uint16)309) /* Matte Data			*/

#define IMHDFTCCN   ((sdsc_uint16)310) /* color correction		*/

#define IMHDFTCFM   ((sdsc_uint16)311) /* color format			*/

#define IMHDFTAR    ((sdsc_uint16)312) /* aspect ratio			*/



/* Composition set							*/

#define IMHDFTDRAW  ((sdsc_uint16)400) /* Draw these images in sequence	*/

#define IMHDFTRUN   ((sdsc_uint16)401) /* run this as a program/script	*/



#define IMHDFTXYP   ((sdsc_uint16)500) /* x-y position			*/

#define IMHDFTMTO   ((sdsc_uint16)501) /* machine-type override		*/



/* Tektronix								*/

#define IMHDFTT14   ((sdsc_uint16)602) /* TEK 4014 data			*/

#define IMHDFTT105  ((sdsc_uint16)603) /* TEK 4105 data			*/



/* Scientific Data set							*/

#define IMHDFTSDG   ((sdsc_uint16)700) /* Scientific Data Group		*/

#define IMHDFTSDD   ((sdsc_uint16)701) /* Scientific Data DimRec		*/

#define IMHDFTSD    ((sdsc_uint16)702) /* Scientific Data			*/

#define IMHDFTSDS   ((sdsc_uint16)703) /* Scales				*/

#define IMHDFTSDL   ((sdsc_uint16)704) /* Labels				*/

#define IMHDFTSDU   ((sdsc_uint16)705) /* Units				*/

#define IMHDFTSDF   ((sdsc_uint16)706) /* Formats				*/

#define IMHDFTSDM   ((sdsc_uint16)707) /* Max/Min				*/

#define IMHDFTSDC   ((sdsc_uint16)708) /* Coord sys			*/

#define IMHDFTSDT   ((sdsc_uint16)709) /* Transpose			*/











/*

 *  CONSTANTS

 *	IMHDFNT*	-  MT and NT values

 *

 *  DESCRIPTION

 *	The MT (machine type) tag uses the 16-bit reference number field as

 *	4 4-bit values broken down in the order (high to low):

 *

 *		unsigned char

 *		unsigned int

 *		float

 *		double

 *

 *	The unsigned char 4-bit field has as a value one of the char class

 *	codes.  Likewise for the unsigned int, float, and double.  Class

 *	code constants have names starting with IMHDFNT* and are the same as

 *	used by the NT tag.

 *

 *	The NT tag has 4 8-bit values assiciated with it, one of which

 *	specifies the type being specified (unsigned int, int, etc), and another

 *	the class code representing the description (its a VAX float, etc).

 */



#define	IMHDFNTVERSION    1	/* current version of NT info		*/



/* Type codes								*/

#define	IMHDFNTUINT       1

#define	IMHDFNTINT        2

#define	IMHDFNTUCHAR      3

#define	IMHDFNTCHAR       4

#define	IMHDFNTFLOAT      5

#define	IMHDFNTDOUBLE     6



/* Class codes for unsigned int and int.					*/

#define	IMHDFINTMBO       1	/* Motorola byte order 2's compl	*/

#define	IMHDFINTVBO       2	/* Vax byte order 2's compl		*/

#define	IMHDFINTIBO       4	/* Intel byte order 2's compl		*/



/* Class codes for float and double.					*/

#define	IMHDFFLOATIEEE      1	/* IEEE format				*/

#define	IMHDFFLOATVAX       2	/* Vax format				*/

#define	IMHDFFLOATCRAY      3	/* Cray format				*/

#define	IMHDFFLOATPC        4	/* PC floats - flipped IEEE		*/



/* Class codes for uchar and char.					*/

#define	IMHDFCHARBYTE      0	/* bitwise/numeric field		*/

#define	IMHDFCHARASCII     1	/* ASCII				*/

#define	IMHDFCHAREBCDIC    5	/* EBCDIC				*/











/*

 *  CONSTANTS

 *	IMHDFC*	-  color formats

 *

 *  DESCRIPTION

 *	Color formats are described by character strings in the data for

 *	the CFM tag.  Internally we treat them as the following integer

 *	constants for quicker comparisons.

 */



#define IMHDFCVALUE	0		/* Pseudo-Color			*/

#define IMHDFCRGB		1		/* RGB				*/

#define IMHDFCXYZ		2		/* CIE XYZ			*/

#define IMHDFCHSV		3		/* Hue-Saturation-Value		*/

#define IMHDFCHSI		4		/* Hue-Saturation-Intensity	*/

#define IMHDFCSPECTRAL	5		/* Spectral sampling		*/











/*

 *  CONSTANTS

 *	RED, GREEN, BLUE	-  RGB flags

 *

 *  DESCRIPTION

 *	These values are used during run-length encoding and decoding of

 *	RGB images in order to keep track of the channel value currently

 *	being handled.

 */



#define BLUE		0

#define GREEN		1

#define RED		2











/*

 *  DD List Management

 */



/*

 *  TYPEDEF & STRUCT

 *	imHdfDD		-  Data Descriptor entry from an HDF file

 *

 *  DESCRIPTION

 *	An imHdfDD describes one piece of data in an HDF file.  When an HDF

 *	table of contents is read in, a list of all of the data items in

 *	the file is made.

 */



typedef struct imHdfDD

{

	unsigned int	dd_tag;		/* Tag number			*/

	unsigned int	dd_ref;		/* Reference count		*/

	long	        dd_dataOffset;	/* File offset to data		*/

	unsigned int	dd_dataLength;	/* Length (in bytes) of data	*/



	struct imHdfDD *dd_next;	/* Next in linked list		*/

} imHdfDD;











/*

 *  GLOBALS

 *	imHdfDDList	-  list of DDs

 *	imHdfDDListEnd	-  pointer to end of DD list

 *	imHdfDDCount	-  # of entries in DD list

 */



extern imHdfDD *imHdfDDList;		/* List of DD's			*/

extern imHdfDD *imHdfDDListEnd;		/* Pointer to end of DD list	*/

extern int      imHdfDDCount;		/* # of DD's in list		*/











/*

 *  MACROS

 *	imHdfDDQNEntry	-  query number of entries in DD list

 *	imHdfDDQTag	-  query tag for a DD entry

 *	imHdfDDQRef	-  query reference number for a DD entry

 *	imHdfDDQDataOffset -  query data offset for a DD entry

 *	imHdfDDQDataLength -  query data length for a DD entry

 *	imHdfDDQNext	-  query next in DD list

 *	imHdfDDQFirst	-  query first in DD list

 *

 *  DESCRIPTION

 *	Query stuff from a DD list.

 */



#define imHdfDDQNEntry()			(imHdfDDCount)

#define imHdfDDQTag(pDD)			(pDD->dd_tag)

#define imHdfDDQRef(pDD)			(pDD->dd_ref)

#define imHdfDDQDataOffset(pDD)			(pDD->dd_dataOffset)

#define imHdfDDQDataLength(pDD)			(pDD->dd_dataLength)

#define imHdfDDQNext(pDD)			(pDD->dd_next)

#define imHdfDDQFirst()				(imHdfDDList)











/*

 *  CLT List Management

 */



/*

 *  TYPEDEF & STRUCT

 *	imHdfClt	-  CLT that has been written out

 *

 *  DESCRIPTION

 *	As CLT's are written out, their pointer, and LD and LUT tag/ref's

 *	are saved to possibly be included in a future RIG.

 */



typedef struct imHdfClt

{

	ImClt	*clt_clt;		/* CLT pointer			*/

	unsigned int	 clt_refLUT;	/* LUT's ref			*/

	unsigned int	 clt_refLD;	/* LD's ref			*/



	struct imHdfClt *clt_next;	/* Next in list			*/

} imHdfClt;











/*

 *  GLOBALS

 *	imHdfCltList	-  list of CLT's being written out

 *	imHdfCltListEnd	-  end of the CLT list

 */



extern imHdfClt *imHdfCltList;

extern imHdfClt *imHdfCltListEnd;











/*

 *  MACROS

 *	imHdfCltQRefLUT	-  query LUT reference number for an entry

 *	imHdfCltQRefLD	-  query LD reference number for an entry

 *	imHdfCltQClt	-  query CLT pointer for an entry

 *

 *  DESCRIPTION

 *	Query stuff from an HDF CLT.

 */



#define imHdfCltQRefLUT(pClt)		(pClt->clt_refLUT)

#define imHdfCltQRefLD(pClt)		(pClt->clt_refLD)

#define imHdfCltQClt(pClt)		(pClt->clt_clt)











/*

 *  MACROS

 *	ReadStruct	-  read a structure

 *	WriteStruct	-  write a structure

 *	Read		-  read an item

 *	Write		-  write an item

 *	Seek		-  seek to a file location

 *	Tell		-  tell current file locaiton

 *

 *  DESCRIPTION

 *	These macros just cover up the standard I/O calls and return

 *	a fatal error code.  They assume the local variables 'ioType', 'fd',

 *	and 'fp' exist.

 */



#define ReadStruct(ptr,bin)						\

	if ( ImBinReadStruct( ioType, fd, fp, (ptr), (bin) ) == -1 )	\

		ImReturnBinError( );

#define WriteStruct(ptr,bin)						\

	if ( ImBinWriteStruct( ioType, fd, fp, (ptr), (bin) ) == -1 )	\

		ImReturnBinError( );

#define Read(ptr,type,nbytes,num)					\

	if ( ImBinRead( ioType, fd, fp, (ptr), (type), (nbytes), (num) )== -1)\

		ImReturnBinError( );

#define Write(ptr,type,nbytes,num)					\

	if ( ImBinWrite( ioType, fd, fp, (ptr), (type), (nbytes), (num) )== -1)\

		ImReturnBinError( );

#define Seek(offset)							\

	ImSeek( ioType, fd, fp, (offset), 0 );

#define Tell()								\

	ImTell( ioType, fd, fp );











/*

 *  GLOBALS

 *	imHdfByteOrder		-  data byte order

 *	imHdfFloatFormat	-  data float format

 *	imHdfRef		-  current reference number

 */

extern int imHdfByteOrder;		/* Default byte order		*/

extern int imHdfFloatFormat;		/* Default float format		*/

extern sdsc_uint16 imHdfRef;		/* Current reference number	*/











/*

 *  TYPEDEF & STRUCT

 *	imHdfDim		-  dimmension information

 *

 *  DESCRIPTION

 *	The ID, LD, and MD tags each use the same structure in an HDF file.

 *	The each given a width and height.  They point to an NT tag to

 *	indicate how big a channel (red, green, etc) for a pixel is.  They

 *	tell how many channels it takes to make a pixel (3 for RGB) and

 *	what storage interleave scheme is used for the image.  They also point

 *	to a compression tag to describe the compression scheme.

 *

 *	imHdfDim holds all this information after being read in, or before

 *	being written out.

 */



typedef struct imHdfDim

{

	int	dim_width;		/* Image width			*/

	int	dim_height;		/* Image height			*/

	int     dim_channelType;	/* Type used for channel data	*/

	int	dim_channelSize;	/* # of bytes per channel	*/

	int     dim_channelByteOrder;	/* Channel byte order		*/

	int     dim_channelFloatFormat;	/* Channel float format		*/

	int	dim_pixelSize;		/* # of channels per pixel	*/

	int	dim_interleave;		/* Interleave scheme		*/

	int	dim_compression;	/* Compression scheme		*/

} imHdfDim;











/*

 *  TYPEDEF & STRUCT

 *	imHdfRIG		-  RIG information collection

 *

 *  DESCRIPTION

 *	A RIG (raster image group) references several other tags.  Much of

 *	the information these tags point to must be read in and available

 *	before the image, CLT, and matte image themselves can be read in

 *	and uncompressed.  imHdfRIG is is a repository for this collection of

 *	information prior to the image being read in.

 */



typedef struct imHdfRIG

{

	imHdfDD *rig_dd;		/* RIG data descriptor		*/



	/* Image Dimension (ID) info					*/

	imHdfDim  *rig_imageDim;	/* Image dimension info		*/



	/* Raster Image (RI) info					*/

	imHdfDD *rig_imageDD;		/* DD pointing to image		*/



	/* Lookup table Dimension (LD) info				*/

	imHdfDim  *rig_cltDim;		/* CLT dimension info		*/



	/* LookUp Table (LUT) info					*/

	imHdfDD *rig_cltDD;		/* DD pointing to CLT		*/



	/* Matte channel Dimension (MD) info				*/

	imHdfDim  *rig_matteDim;	/* Matte dimension info		*/



	/* MAtte channel (MA) info					*/

	imHdfDD *rig_matteDD;		/* DD pointing to matte		*/



	/* Color CorrectionN (CCN) info					*/

	int     rig_ccnGiven;		/* Was an CCN tag supplied?	*/

	float	rig_gamma;		/* Gamma correction factor	*/

	float	rig_red[3];		/* XYZ for red			*/

	float	rig_green[3];		/* XYZ for green		*/

	float	rig_blue[3];		/* XYZ for blue			*/

	float	rig_white[3];		/* XYZ for white		*/



	/* Color ForMat (CFM) info					*/

	int	rig_colorFormat;	/* Color format for image	*/



	/* Aspect Ratio (AR) info					*/

	float	rig_aspectRatio;	/* Image aspect ratio		*/



	/* Machine Type Override (MTO) info				*/





	struct imHdfRIG *rig_next;	/* Next in RIG list		*/

} imHdfRIG;











/*

 *  FUNCTIONS

 *	imHdf*	-  private functions to imhdfread.c and imhdfwrite.c

 */



#ifdef __STDC__

extern void imHdfDDEmpty(void );

extern imHdfDD *imHdfDDAppend(unsigned int tag, unsigned int ref, long dataOffset, long dataLength );

extern imHdfDD *imHdfDDFind(unsigned int tag, unsigned int ref  );



extern void imHdfCltEmpty( void );

extern imHdfClt *imHdfCltAppend( ImClt* clt, unsigned int refLUT, unsigned int refLD);

extern imHdfClt *imHdfCltFind( ImClt * clt );

extern imHdfClt *imHdfCltFindRef( unsigned int ref);

#else

extern void imHdfDDEmpty( );

extern imHdfDD *imHdfDDAppend( );

extern imHdfDD *imHdfDDFind( );



extern void imHdfCltEmpty( );

extern imHdfClt *imHdfCltAppend( );

extern imHdfClt *imHdfCltFind( );

extern imHdfClt *imHdfCltFindRef( );

#endif





#endif /* __IMHDFINTERNALH__ */

