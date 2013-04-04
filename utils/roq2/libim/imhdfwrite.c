/**

 **	$Header: /roq/libim/imhdfwrite.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imhdfwrite.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imhdfwrite.c		-  HDF image file write

 **

 **  PROJECT

 **	libim		-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imhdf.c contains routines to write HDF image files for

 **	the image manipulation library.  Raster data written

 **	out is taken from a tag table.

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **	ImHdfWrite	f  write an HDF file

 **

 **  PRIVATE CONTENTS

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

 **	imHdfWrite	f  handle the writing of an HDF file

 **	imHdfDimWrite	f  write dimension tag to file

 **	imHdfCltWrite	f  write CLT to file

 **	imHdfVfbWrite	f  write VFB to file

 **

 **	imHdfVfbWrite8		f  write 8-bit uncomp. VFB

 **	imHdfVfbWriteRLE8	f  write 8-bit RLE comp. VFB

 **	imHdfVfbWrite16		f  write 16-bit uncomp. VFB

 **	imHdfVfbWriteRGB	f  write 24-bit uncomp. uninterleaved VFB

 **	imHdfVfbWriteRGBLine	f  write 24-bit uncomp. line interleaved VFB

 **	imHdfVfbWriteRGBPlan	f  write 24-bit uncomp. plane interleaved VFB

 **	imHdfVfbWriteRLERGB	f  write 24-bit RLE comp. uninterleaved VFB

 **	imHdfVfbWriteRLERGBLine f  write 24-bit RLE comp. line interleaved VFB

 **	imHdfVfbWriteRLERGBPlane f  write 24-bit RLE comp. plane interleaved VFB

 **

 **  HISTORY

 **	$Log: /roq/libim/imhdfwrite.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.10  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.9  1994/10/03  11:30:12  nadeau

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

 **	Revision 1.8  93/02/18  18:10:25  secoskyj

 **	Fixed carriage return being output before Image: output

 **	message.

 **	

 **	Revision 1.7  92/12/03  01:48:40  nadeau

 **	Corrected info messages.

 **	

 **	Revision 1.6  92/11/23  18:42:31  nadeau

 **	Removed use of IMINFOMSG.

 **	

 **	Revision 1.5  92/11/04  11:58:08  groening

 **	made minor changes in iminfo

 **	

 **	Revision 1.4  92/10/19  14:14:47  groening

 **	added ImInfo statements

 **	

 **	Revision 1.3  92/09/02  13:15:05  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.2  92/04/09  09:34:58  groening

 **	To make the compiler happy added extern statements.

 **	

 **	Revision 1.1  91/10/03  09:05:31  nadeau

 **	Initial revision

 **	

 **	

 **/



#include "iminternal.h"

#include "imhdfinternal.h"



/**

 **  CODE CREDITS

 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1992.

 **/







/*

 * Function declarations, since they return longs, rather than

 * integers.  This caused a problem on a Cray.

 */



#ifdef __STDC__

static long imHdfVfbWrite8( int ioType, int fd, FILE *fp, ImVfb *vfb );

static long imHdfVfbWriteRLE8(int ioType, int fd, FILE *fp, ImVfb *vfb);

static long imHdfVfbWrite16(int ioType, int fd, FILE *fp, ImVfb *vfb);

static long imHdfVfbWriteRGB(int ioType, int fd, FILE *fp, ImVfb *vfb);

static long imHdfVfbWriteRGBLine(int ioType, int fd, FILE *fp, ImVfb *vfb);

static long imHdfVfbWriteRGBPlane(int ioType, int fd, FILE *fp, ImVfb *vfb);

static long imHdfVfbWriteRLERGB(int ioType, int fd, FILE *fp, ImVfb *vfb);

static long imHdfVfbWriteRLERGBLine(int ioType, int fd, FILE *fp, ImVfb *vfb);

static long imHdfVfbWriteRLERGBPlane(int ioType, int fd, FILE *fp, ImVfb *vfb);

static int  imHdfCltWrite( int ioType, int fd, FILE *fp, ImClt *clt );

static int  imHdfVfbWrite( int ioType, int fd, FILE *fp, int interRequest, int compRequest, int cltRequest, 

	int alphaRequest, ImVfb *vfb );

#else

static long imHdfVfbWrite8();

static long imHdfVfbWriteRLE8();

static long imHdfVfbWrite16();

static long imHdfVfbWriteRGB();

static long imHdfVfbWriteRGBLine();

static long imHdfVfbWriteRGBPlane();

static long imHdfVfbWriteRLERGB();

static long imHdfVfbWriteRLERGBLine();

static long imHdfVfbWriteRLERGBPlane();

static int  imHdfCltWrite();

static int  imHdfVfbWrite();

#endif











/*

 *  FORMAT

 *	HDF	-  Hierarchical Data Format

 *

 *  DESCRIPTION

 *	See imhdfread.c.

 */











/*

 *  FUNCTION

 *	ImHdfWrite	-  write an HDF file

 *

 *  DESCRIPTION

 *	ImHdfWrite() walks the tag table and writes out CLT's and VFB's it

 *	finds there.  Because more than one VFB can use the same CLT, we'd

 *	like to avoid writing out the same CLT more than once.  CLT's are

 *	added to a CLT list.  As each new VFB is encountered, the CLT list

 *	is checked to see if we've already written out its CLT.  If so,

 *	we just point to it.  Otherwise that VFB's CLT is written out and

 *	added to the list.

 *

 *	After a VFB has been written out, a raster image group (RIG) is

 *	created for all of the tags related to the image.

 *

 *	As each new tag is written to the file, it is also added to a DD list.

 *	When we're done, the DD list is written out as a DD block and we go

 *	back and patch up the DD header at the top of the file to make it

 *	point to the DD block at the end of the file.

 *

 *	The tags written to the file are as follows (but not in this order):

 *

 *		NCSA	For

 *		tools	Image

 *	Tag	output?	Depths?		Meaning

 *

 *	MT	yes	all		Machine characteristics

 *

 *	RIG	yes	all		Raster image group tag list

 *

 *	RI	yes	all		Raster image (uncompressed)

 *	CI	yes	all		Compressed image

 *	ID	yes	all		Image dimensions

 *	NT	yes	all		Channel size/type

 *	RLE	no	all		Flag RLE compression scheme

 *	CFM	no	all		Color format

 *

 *	LUT	yes	8-bit		CLT

 *	LD	no	8-bit		CLT dimensions

 *

 *	RI8	yes	8-bit		Raster image (uncompressed)

 *	CI8	yes	8-bit		Compressed image

 *	ID8	yes	8-bit		Image dimensions

 *	IP8	yes	8-bit		CLT

 *

 *	Note:  some of the tags we output are standard tags that the HDF

 *	spec states should be written, yet the NCSA tools and NCSA HDF

 *	library doesn't normally write them.

 */





int				/* Returns # of tags written	*/

#ifdef __STDC__

ImHdfWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

ImHdfWrite( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int         ioType;		/* I/O flags			*/

	int         fd;			/* Output file descriptor	*/

	FILE	   *fp;			/* Output file pointer		*/

	TagTable   *flagsTable;		/* Format flags			*/

	TagTable   *tagTable;		/* Tag list to add to		*/

#endif

{

	sdsc_uint16          nDD;		/* # of DD's in block		*/

	imHdfDD        *pDD;		/* DD list pointer		*/

	unsigned char           buffer[1024];	/* Temporary write buffer	*/



	unsigned int	        tag;		/* DD tag			*/

	unsigned int	        ref;		/* DD reference number		*/

	long	        dataOffset;	/* Offset to data		*/

	long	        dataLength;	/* Length of data		*/



	BinMachineInfo *machine;	/* Machine information		*/

	ImClt          *clt;		/* CLT pointer			*/

	ImVfb          *vfb;		/* VFB pointer			*/

	char           *s;		/* Tag name string		*/

	long            offset;		/* File offset to next DD header*/



	int             nTag = 0;	/* # of tags written		*/

	int             n;		/* # of tag table entries	*/

	int             i;		/* Counter			*/

	TagEntry       *tagEntry;	/* Tag table entry holder	*/



	int		interRequest;	/* Interleave request		*/

	int		compRequest;	/* Compression request		*/

	int		cltRequest;	/* CLT request			*/

	int		alphaRequest;	/* Alpha plane request		*/

	int		imageNum;	/* number of vfb's in tagtable	*/

	int		imageNumII;	/* number of vfb's in tagtable	*/

	char		message[100];	/* ImInfo message		*/





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

	 *  All administrative stuff (tags, reference counts, file offsets)

	 *  are in MBF byte order.

	 */

	BinByteOrder( BINMBF );





	/*

	 *  Write the magic number.

	 */

	buffer[0] = 0x0E;			/* CTRL-N		*/

	buffer[1] = 0x03;			/* CTRL-C		*/

	buffer[2] = 0x13;			/* CTRL-S		*/

	buffer[3] = 0x01;			/* CTRL-A		*/

	Write( buffer, UCHAR, 1, 4 );





	/*

	 *  Write out an initial data block with one DD (data descriptor)

	 *  describing the machine type (MT tag).  The rest of the data

	 *  for the file will be listed in another DD placed at the end of

	 *  the file.  When we're done, we'll come back to this block and

	 *  update its offset to point to that final DD.

	 */

	nDD = 1;				/* 1 DD in this block	*/

	Write( &nDD,    UINT16, 2, 1 );

	offset = 0;				/* Filled in later	*/

	Write( &offset, LONG,   4, 1 );



	tag        = IMHDFTMT;			/* MT = machine type	*/

	dataOffset = 0;				/* No data		*/

	dataLength = 0;				/* No data		*/



	machine = BinQMachine( );

	ref = 0;

	imHdfByteOrder = machine->bin_byteOrder;

	switch ( machine->bin_byteOrder )

	{

	case BINLBF:	/* Least-significant byte first			*/

		ImInfo ("Byte Order","Least Significant Byte First");

		ref = (IMHDFINTVBO<<12);		/* unsigned ints are Vax bo	*/

		break;

	case BINMBF:	/* Most-significant byte first			*/

		ImInfo ("Byte Order","Most Significant Byte First");

		ref = (IMHDFINTMBO<<12);		/* unsigned ints are Motorola bo*/

		break;

	}



	imHdfFloatFormat = machine->bin_floatFormat;

	switch ( machine->bin_floatFormat )

	{

	case BINIEEE:	/* IEEE floats and doubles			*/

		if ( machine->bin_byteOrder == BINMBF )

			ref |= (IMHDFFLOATIEEE<<8) | (IMHDFFLOATIEEE<<4);

		else

			ref |= (IMHDFFLOATPC<<8) | (IMHDFFLOATPC<<4);

		break;

	case BINVAX:	/* Vax floats and doubles			*/

		ref |= (IMHDFFLOATVAX<<8) | (IMHDFFLOATVAX<<4);

		break;

	case BINCRAYMP:	/* Cray floats and doubles			*/

		ref |= (IMHDFFLOATCRAY<<8) | (IMHDFFLOATCRAY<<4);

		break;

	}



	ref |= IMHDFCHARASCII;			/* unsigned chars are in ASCII	*/



	Write( &tag,        UINT, 2, 1 );

	Write( &ref,        UINT, 2, 1 );

	Write( &dataOffset, LONG, 4, 1 );

	Write( &dataLength, LONG, 4, 1 );







	/*

	 *  Walk the tag table, watching for CLT's and VFB's.

	 */

	BinByteOrder( imHdfByteOrder );

	BinFloatFormat( imHdfFloatFormat );

	n = TagTableQNEntry( tagTable, NULL );

	imHdfDDEmpty( );

	imHdfRef = 100;



	/* count number of vfb's */

	imageNum=0;

	imageNumII=1;

	for ( i = 0; i < n; i++, imHdfRef += 5 )

	{

		tagEntry = TagTableQLinear( tagTable, i );

		s = TagEntryQTag( tagEntry );

		if ( strcmp( s, "image vfb" ) == 0 ) imageNum++;

	}



	for ( i = 0; i < n; i++, imHdfRef += 5 )

	{

		tagEntry = TagTableQLinear( tagTable, i );

		s = TagEntryQTag( tagEntry );



		if ( (cltRequest == IMCLTYES) && strcmp( s, "image clt" ) == 0 )

		{

			TagEntryQValue( tagEntry, &clt );

			if ( (nTag += imHdfCltWrite( ioType, fd, fp, clt ))== -1)

				return ( -1 );	/* Error already posted	*/

		}



		if ( strcmp( s, "image vfb" ) == 0 )

		{

			sprintf (message, "%d of %d",imageNumII++,imageNum);

			ImInfo ("Image",message);

			TagEntryQValue( tagEntry, &vfb );

			if ( (nTag += imHdfVfbWrite( ioType, fd, fp,

				interRequest, compRequest, cltRequest,

				alphaRequest, vfb ))== -1)

				return ( -1 );	/* Error already posted	*/

		}



		/*

		 *  Ignore all other tags.

		 */

	}





	/*

	 *  Write out the DD list, preceded by its header.

	 */

	BinByteOrder( BINMBF );

	offset = Tell( );

	nDD    = imHdfDDQNEntry( );

	dataOffset = 0;				/* End of header list	*/

	Write( &nDD,        UINT16, 2, 1 );

	Write( &dataOffset, LONG,   4, 1 );



	for ( pDD = imHdfDDQFirst( ); pDD; pDD = imHdfDDQNext( pDD ) )

	{

		tag        = imHdfDDQTag( pDD );

		ref        = imHdfDDQRef( pDD );

		dataOffset = imHdfDDQDataOffset( pDD );

		dataLength = imHdfDDQDataLength( pDD );



		Write( &tag,        UINT, 2, 1 );

		Write( &ref,        UINT, 2, 1 );

		Write( &dataOffset, LONG, 4, 1 );

		Write( &dataLength, LONG, 4, 1 );

	}





	/*

	 *  Seek back to the first DD header nad change its offset pointer

	 *  to point to the new DD header we just added at the end of the file.

	 */

	Seek( 4 + 2 );				/* Seek to 6th byte	*/

	Write( &offset, LONG, 4, 1 );



	return ( nTag );

}











/*

 *  FUNCTION

 *	imHdfDimWrite	-  write dimension tag to file

 *

 *  DESCRIPTION

 *	Tags describing a dimension are written out:

 *

 * 		NT	numeric type

 *		RLE	compression scheme

 *

 *	The dimension tag's data itself is also written out.

 */



static long				/* Returns file offset		*/

#ifdef __STDC__

imHdfDimWrite( int ioType, int fd, FILE *fp, int tag, imHdfDim *pDim )

#else

imHdfDimWrite( ioType, fd, fp, tag, pDim )

	int             ioType;		/* I/O flags			*/

	int             fd;		/* Output file descriptor	*/

	FILE	       *fp;		/* Output file pointer		*/

	int             tag;		/* Tag type			*/

	imHdfDim       *pDim;		/* Dimension information	*/

#endif

{

	long            offset;		/* File offset			*/

	unsigned char           nt[4];		/* NT buffer			*/

	sdsc_uint16          tmpTag;		/* Temp tag holder		*/

	sdsc_uint16          tmpRef;		/* Temp ref holder		*/





	/*

	 *  Write out an NT tag's data describing the type for a channel.

	 *

	 *  The NT tag's data is 4 1-byte quantities:

	 *

	 *	0	version number of NT tag (version 1 supported here)

	 *	1	type code

	 *	2	width of type, in bits

	 *	3	class code (byte order, float format, etc)

	 */

	offset = Tell( );

	nt[0] = 1;			/* Version 1			*/

	if ( pDim->dim_channelType == UINT )

	{

		nt[1] = IMHDFNTUINT;

		if ( pDim->dim_channelByteOrder == BINLBF )

			nt[3] = IMHDFINTVBO;

		else

			nt[3] = IMHDFINTMBO;

	}

	else

	{

		nt[1] = IMHDFNTUCHAR;

		nt[3] = IMHDFCHARBYTE;	/* bitwise numeric		*/

	}

	nt[2] = pDim->dim_channelSize * 8;

	Write( nt, UCHAR, 1, 4 );

	if ( tag == IMHDFTID )

		tmpRef = imHdfRef;

	else

		tmpRef = imHdfRef + 1;

	imHdfDDAppend( IMHDFTNT, tmpRef, offset, 4 );





	/*

	 *  Add a no-data RLE tag if we are doing compression.

	 */

	if ( pDim->dim_compression == IMHDFTRLE )

		imHdfDDAppend( IMHDFTRLE, imHdfRef, 0, 0 );





	/*

	 *  Write out the dimension tag.

	 */

	offset = Tell( );

	tmpTag = IMHDFTNT;

	Write( &pDim->dim_width,       INT,    4, 1 );

	Write( &pDim->dim_height,      INT,    4, 1 );

	Write( &tmpTag,                UINT16, 2, 1 );

	Write( &tmpRef,                UINT16, 2, 1 );

	Write( &pDim->dim_pixelSize,   INT,    2, 1 );

	Write( &pDim->dim_interleave,   INT,    2, 1 );

	Write( &pDim->dim_compression, UINT,   2, 1 );

	if ( pDim->dim_compression == 0 )

	{

		tmpRef = 0;

		Write( &tmpRef,        UINT16, 2, 1 );

	}

	else

		Write( &imHdfRef,         UINT16, 2, 1 );

	imHdfDDAppend( tag, imHdfRef, offset, 20 );



	return ( 0 );

}











/*

 *  FUNCTION

 *	imHdfCltWrite	-  write CLT to file

 *

 *  DESCRIPTION

 *	Tags describing a CLT are written out:

 *

 *		LD	lookup table dimensions

 *		LUT	lookup table

 *		IP8	8-bit lookup table (if 256 or fewer CLT entries)

 *

 *	The CLT is written out uncompressed and uninterleaved.

 */



static int				/* Returns # of tags written	*/

#ifdef __STDC__

imHdfCltWrite( int ioType, int fd, FILE *fp, ImClt *clt )

#else

imHdfCltWrite( ioType, fd, fp, clt )

	int             ioType;		/* I/O flags			*/

	int             fd;		/* Output file descriptor	*/

	FILE	       *fp;		/* Output file pointer		*/

	ImClt          *clt;		/* CLT to write out		*/

#endif

{

	imHdfDim        dim;		/* Dimensions of CLT		*/

	long            offset;		/* tag data offset		*/

	int             i;		/* Counter			*/

	ImCltPtr        pColor;		/* CLT color pointer		*/

	unsigned char          *buffer;		/* RGB buffer			*/

	int             n;		/* # of colors			*/





	/*

	 *  Write out an LD dimensions tag.

	 *	Width is # of colors

	 *	Height is always 1

	 *	Each channel is a unsigned char (byte order & float format unused)

	 *	3 channels per pixel (RGB)

	 *	CLT not interleaved or compressed

	 */



	dim.dim_width              = n = ImCltQNColors( clt );

	dim.dim_height             = 1;

	dim.dim_channelType        = UCHAR;

	dim.dim_channelSize        = 1;

	dim.dim_channelByteOrder   = imHdfByteOrder;

	dim.dim_channelFloatFormat = imHdfFloatFormat;

	dim.dim_pixelSize          = 3;

	dim.dim_interleave          = 0;

	dim.dim_compression        = 0;



	if ( imHdfDimWrite( ioType, fd, fp, IMHDFTLD, &dim ) == -1 )

		return ( -1 );		/* Error stuff already handled	*/





	/*

	 *  Write out the CLT, not interleaved or compressed.

	 */

	offset = Tell( );

	n *= 3;

	if ( n < 256*3 )

	{

		ImMalloc( buffer, unsigned char *, sizeof( unsigned char ) * 256 * 3 );

	}

	else

	{

		ImMalloc( buffer, unsigned char *, sizeof( unsigned char ) * n );

	}

	pColor = ImCltQFirst( clt );

	for ( i = 0; i < n; )

	{

		buffer[i++] = ImCltQRed( pColor );

		buffer[i++] = ImCltQGreen( pColor );

		buffer[i++] = ImCltQBlue( pColor );

		ImCltSInc( clt, pColor );

	}

	if ( dim.dim_width <= 256 )

	{

		/*

		 *  With CLT's of 256 entries or less, we can support the

		 *  IP8 tag.  However, IP8 palettes must be 256 entries

		 *  long.  So, we pad the CLT with enough zero entries to

		 *  make it the right size.

		 *

		 *  The LD tag, which allows arbitrary CLT lengths, will only

		 *  report the number of entries filled with real data, not

		 *  our pad entries.

		 *

		 *  The IP8 tag, which assumes 256 CLT entries, will have

		 *  a data length that includes the pad entries.

		 *

		 *  The LUT tag, which allows any CLT length, will have a

		 *  data length that does not include the pad entries.

		 */

		while ( i < 256*3 )

		{

			buffer[i++] = 0;

			buffer[i++] = 0;

			buffer[i++] = 0;

		}

		imHdfDDAppend( IMHDFTIP8, imHdfRef, offset, 256 * 3 );

		Write( buffer, UCHAR, 1, 256 * 3 );

	}

	else

		Write( buffer, UCHAR, 1, n );

	free( (char *)buffer );

	imHdfDDAppend( IMHDFTLUT, imHdfRef, offset, n );



	imHdfCltAppend( clt, imHdfRef, imHdfRef );



	return ( 1 );			/* Wrote CLT from tag table	*/

}











/*

 *  FUNCTION

 *	imHdfVfbWrite	-  write VFB to file

 *

 *  DESCRIPTION

 *	The tags associated with a VFB are written out:

 *

 *		ID	image dimensions

 *		RI	raster image

 *		CI	compressed raster image

 *		RLE	run-length encoded

 *		CFM	color format

 *		RIG	raster image group

 *

 *	The image is written out as either a compressed or uncompressed

 *	image, uninterleaved, scanline interleaved, or plane interleaved,

 *	all as selected by the interRequest from the flagsTable.

 */



static int				/* Returns # of tags written	*/

#ifdef __STDC__

imHdfVfbWrite( int ioType, int fd, FILE *fp, int interRequest, int compRequest, int cltRequest, int alphaRequest, ImVfb *vfb )

#else

imHdfVfbWrite( ioType, fd, fp, interRequest, compRequest, cltRequest, alphaRequest, vfb )

	int             ioType;		/* I/O flags			*/

	int             fd;		/* Output file descriptor	*/

	FILE	       *fp;		/* Output file pointer		*/

	int             interRequest;	/* Interleave request		*/

	int		compRequest;	/* Compression request		*/

	int		cltRequest;	/* CLT channel request		*/

	int		alphaRequest;	/* Alpha channel request	*/

	ImVfb          *vfb;		/* VFB to write out		*/

#endif

{

	int             i;		/* Counter			*/

	imHdfDim        dim;		/* Dimensions of VFB		*/

	ImVfbPtr        pPixel;		/* VFB pixel pointer		*/

	ImClt          *clt;		/* VFB's CLT			*/

	imHdfClt       *hdfClt;		/* CLT list entry		*/

	int             n;		/* # of colors			*/

	long            offset;		/* tag data offset		*/

	unsigned int            fields;		/* Field mask			*/

	int             nTag = 0;	/* # of tags written		*/

	unsigned char          *buffer;		/* RGB buffer			*/

	int             nRIG = 0;	/* # of items in the RIG	*/

	unsigned int            tagList[50];	/* RIG tags			*/

	unsigned int            refList[50];	/* RIG refs			*/

	TagEntry       *tagEntry;	/* Tag table entry holder	*/

	char		message[100];	/* ImInfo message		*/





	/*

	 *  If the VFB has a CLT, look it up in the CLT list to see if it

	 *  has already been written out.

	 */

	if ( (cltRequest == IMCLTYES) && (clt = ImVfbQClt( vfb )) != IMCLTNULL )

	{

		if ( (hdfClt = imHdfCltFind( clt )) == NULL )

		{



			sprintf (message, "%d Entries", ImCltQNColors (clt));

			ImInfo ("Color Table",message);

			/*

			 *  Not already written.  Write it and add it to the

			 *  written CLT list.

			 */

			if ( (nTag = imHdfCltWrite( ioType, fd, fp, clt ))== -1)

				return ( -1 );	/* Error already posted	*/

			hdfClt = imHdfCltFind( clt );

		}

		tagList[nRIG]   = IMHDFTLD;

		refList[nRIG++] = imHdfCltQRefLD( hdfClt );

		tagList[nRIG]   = IMHDFTLUT;

		refList[nRIG++] = imHdfCltQRefLUT( hdfClt );

	}





	/*

	 *  Write out an ID dimensions tag.

	 *	Width and height of image

	 *	Each channel is a unsigned char (RGB & INDEX8) or a unsigned int (INDEX16)

	 *	Machine's byte order and float format

	 *	3 channels per pixel (RGB) or 1 per pixel (INDEX8 & INDEX16)

	 *	Uninterleaved, scanline interleaved, or plane interleaved

	 *	Compressed or uncompressed

	 */



	dim.dim_width              = ImVfbQWidth( vfb );

	dim.dim_height             = ImVfbQHeight( vfb );

	dim.dim_channelByteOrder   = imHdfByteOrder;

	dim.dim_channelFloatFormat = imHdfFloatFormat;

	fields = ImVfbQFields( vfb );

	if ( fields & IMVFBINDEX8 )

	{

		dim.dim_channelType= UCHAR;

		dim.dim_pixelSize  = 1;

		dim.dim_channelSize= 1;

	}

	else if ( fields & IMVFBINDEX16 )

	{

		dim.dim_channelType= UINT;

		dim.dim_pixelSize  = 1;

		clt = ImVfbQClt( vfb );

		if ( clt != IMCLTNULL )

		{

			n = ImCltQNColors( clt );

			if ( n < 256 )			/* 8-bit	*/

				dim.dim_channelSize = 1;

			else if ( n < 65536 )		/* 16-bit	*/

				dim.dim_channelSize = 2;

			else if ( n < 16777216 )	/* 24-bit	*/

				dim.dim_channelSize = 3;

			else				/* 32-bit	*/

				dim.dim_channelSize = 4;

		}

		else

			dim.dim_channelSize = 4;

	}

	else

	{

		/* RGB							*/

		dim.dim_channelType = UCHAR;

		dim.dim_pixelSize  = 3;

		dim.dim_channelSize= 1;

	}



	if ( dim.dim_pixelSize == 1 )

		dim.dim_interleave = 0;		/* Can't interleave indexes*/

	else if ( interRequest == IMINTERLINE )

		dim.dim_interleave  = 1;		/* Scanline interleaved	*/

	else if ( interRequest == IMINTERPLANE )

		dim.dim_interleave  = 2;		/* Plane interleaved	*/

	else

		dim.dim_interleave  = 0;		/* Uninterleaved		*/



	if ( (dim.dim_channelType != UINT) && (compRequest == IMCOMPRLE) )

		dim.dim_compression= IMHDFTRLE;	/* RLE compression	*/

	else

		dim.dim_compression= 0;		/* No compression	*/



	if ( imHdfDimWrite( ioType, fd, fp, IMHDFTID, &dim ) == -1 )

		return ( -1 );		/* Error stuff already handled	*/

	tagList[nRIG]   = IMHDFTID;

	refList[nRIG++] = imHdfRef;





	/*

	 *  If the image is an 8-bit image, write out an ID8 tag.

	 */

	if ( fields & IMVFBINDEX8 )

	{

		offset = Tell( );

		Write( &dim.dim_width,  INT, 2, 1 );

		Write( &dim.dim_height, INT, 2, 1 );

		imHdfDDAppend( IMHDFTID8, imHdfRef, offset, 4 );

	}





	/*

	 *  Write out the color format used

	 */

	if ( dim.dim_pixelSize == 1 )		/* 8-bit and 16-bit indexes*/

	{

		offset = Tell( );

		Write( "VALUE", CHAR, 1, 7 );

		imHdfDDAppend( IMHDFTCFM, imHdfRef, offset, 7 );

		tagList[nRIG]   = IMHDFTCFM;

		refList[nRIG++] = imHdfRef;

	}

	else

	{

		offset = Tell( );

		Write( "RGB", CHAR, 1, 4 );

		imHdfDDAppend( IMHDFTCFM, imHdfRef, offset, 4 );

		tagList[nRIG]   = IMHDFTCFM;

		refList[nRIG++] = imHdfRef;

	}



	sprintf (message, "%d x %d",dim.dim_width, dim.dim_height);

	ImInfo ("Resolution", message);



	/*

	 *  Write out the VFB.

	 */

	offset = Tell( );

	if ( fields & IMVFBINDEX8 )

	{

		switch ( dim.dim_compression )

		{

		case 0:	/* No compression.				*/

			ImInfo ("Type","8-bit Color Indexed");

			ImInfo ("Compression Type","none");

			if ( (n = imHdfVfbWrite8( ioType, fd, fp, vfb )) == -1 )

				return ( -1 );	/* Errors already handled*/

			imHdfDDAppend( IMHDFTRI8, imHdfRef, offset, n );

			imHdfDDAppend( IMHDFTRI, imHdfRef, offset, n );

			tagList[nRIG]   = IMHDFTRI;

			refList[nRIG++] = imHdfRef;

			break;



		case IMHDFTRLE:	/* Run-length encoding			*/

			ImInfo ("Type","8-bit Color Indexed");

			ImInfo ("Compression Type","Run Length Encoded (RLE)");

			if ( (n = imHdfVfbWriteRLE8( ioType, fd, fp, vfb )) == -1 )

				return ( -1 );	/* Errors already handled*/

			imHdfDDAppend( IMHDFTCI8, imHdfRef, offset, n );

			imHdfDDAppend( IMHDFTCI, imHdfRef, offset, n );

			tagList[nRIG]   = IMHDFTCI;

			refList[nRIG++] = imHdfRef;

			break;

		}

	}

	else if ( fields & IMVFBINDEX16 )

	{

		ImInfo ("Type","16-bit Color Indexed");

		ImInfo ("Compression Type","none");

		if ( (n = imHdfVfbWrite16( ioType, fd, fp, vfb )) == -1 )

			return ( -1 );	/* Errors already handled*/

		imHdfDDAppend( IMHDFTRI, imHdfRef, offset, n );

		tagList[nRIG]   = IMHDFTRI;

		refList[nRIG++] = imHdfRef;

	}

	else	/* RGB */

	{

		switch ( dim.dim_compression )

		{

		case 0:	/* No compression.				*/

			switch ( dim.dim_interleave )

			{

			case 0:	/* Uninterleaved			*/

				ImInfo ("Type","24-bit RGB");

				ImInfo ("Compression Type","none");

				ImInfo ("Interleave Type","none (Non-interleaved)");

				if ( (n = imHdfVfbWriteRGB( ioType, fd, fp,

					vfb )) == -1 )

					return ( -1 );	/* Errors done*/

				break;

			case 1:	/* Scanline interleaved			*/

				ImInfo ("Type","24-bit RGB");

				ImInfo ("Compression Type","none");

				ImInfo ("Interleave Type","Scanline");

				if ( (n = imHdfVfbWriteRGBLine( ioType, fd, fp,

					vfb )) == -1 )

					return ( -1 );	/* Errors done*/

				break;

			case 2:	/* Plane interleaved			*/

				ImInfo ("Type","24-bit RGB");

				ImInfo ("Compression Type","none");

				ImInfo ("Interleave Type","Plane");

				if ( (n = imHdfVfbWriteRGBPlane( ioType, fd, fp,

					vfb )) == -1 )

					return ( -1 );	/* Errors done*/

				break;

			}

			imHdfDDAppend( IMHDFTRI, imHdfRef, offset, n );

			tagList[nRIG]   = IMHDFTRI;

			refList[nRIG++] = imHdfRef;

			break;



		case IMHDFTRLE:	/* Run-length encoding			*/

			switch ( dim.dim_interleave )

			{

			case 0:	/* Uninterleaved			*/

				ImInfo ("Type","24-bit RGB");

				ImInfo ("Compression Type","Run Length Encoded (RLE)");

				ImInfo ("Interleave Type","none (Non-interleaved)");

				if ( (n = imHdfVfbWriteRLERGB( ioType, fd, fp,

					vfb )) == -1 )

					return ( -1 );	/* Errors done*/

				break;

			case 1:	/* Scanline interleaved			*/

				ImInfo ("Type","24-bit RGB");

				ImInfo ("Compression Type","Run Length Encoded (RLE)");

				ImInfo ("Interleave Type","Scanline");

				if ( (n = imHdfVfbWriteRLERGBLine( ioType,fd,fp,

					vfb )) == -1 )

					return ( -1 );	/* Errors done*/

				break;

			case 2:	/* Plane interleaved			*/

				ImInfo ("Type","24-bit RGB");

				ImInfo ("Compression Type","Run Length Encoded (RLE)");

				ImInfo ("Interleave Type","Plane");

				if ( (n = imHdfVfbWriteRLERGBPlane( ioType, fd, fp,

					vfb )) == -1 )

					return ( -1 );	/* Errors done*/

				break;

			}

			imHdfDDAppend( IMHDFTCI, imHdfRef, offset, n );

			tagList[nRIG]   = IMHDFTCI;

			refList[nRIG++] = imHdfRef;

			break;

		}

	}





	/*

	 *  Write out the RIG

	 */

	offset = Tell( );

	for ( i = 0; i < nRIG; i++ )

	{

		Write( &tagList[i], UINT, 2, 1 );

		Write( &refList[i], UINT, 2, 1 );

	}

	imHdfDDAppend( IMHDFTRIG, imHdfRef, offset, nRIG * 4  );



	return ( nTag + 1 );		/* VFB and optional CLT		*/

}











/*

 *  MACRO

 *	IMADDTOBUFFER	-  add index to run buffer

 *

 *  DESCRIPTION

 *	Each of the RLE routines which follow use the same basic scheme

 *	for adding a new value to the run buffer.  Their only differences

 *	are in how the get the value to add (red, green, blue, index8, or

 *	index16).  To make the code easier (and shorter), this macro

 *	implements the add-value-to-buffer scheme.  It assumes all of

 *	these variables have been declared by the caller and that the

 *	value to add is in 'index'.

 */



#define IMADDTOBUFFER()							\
{									\
	if ( index != oldIndex )					\
	{								\
		/* Another unique pixel.		*/		\
		if ( count < 0 )					\
		{							\
			/* Fill in run count first.	*/		\
			*pCount = ((unsigned char)(-count)) | 0x80;		\
			count   = 0;					\
			pCount  = rbp++;				\
		}							\
		else if ( count >= 127 )				\
		{							\
			/* Fill in run count first.	*/		\
			*pCount = (unsigned char)count;				\
			count   = 0;					\
			pCount  = rbp++;				\
		}							\
									\
		/* Increase the unique pixel count	*/		\
		oldIndex = index;					\
		++count;						\
		*rbp++ = index;						\
		continue;						\
	}								\
									\
	/* Another non-unique pixel for the run.	*/		\
	if ( count <= -127 )						\
	{								\
		*pCount  = ((unsigned char)(-count)) | 0x80;			\
		count    = 1;						\
		pCount   = rbp++;					\
		*rbp++   = index;					\
		oldIndex = index;					\
		continue;						\
	}								\
									\
	if ( count < 0 )						\
	{								\
		/* Continue the run			*/		\
		--count;						\
		continue;						\
	}								\
									\
	if ( count == 1 )						\
	{								\
		/* Change unique pixel to a run of 2	*/		\
		count = -2;						\
		continue;						\
	}								\
									\
	*pCount  = (unsigned char)(count - 1);					\
	oldIndex = index;						\
	count    = -2;							\
	pCount   = rbp - 1;						\
	*rbp++   = index;						\
}











/*

 *  MACROS

 *	IMCOPYPLANE	-  copy a plane from a VFB to the output file

 *	IMENCODEPLANE	-  encode a plane from a VFB to the output file

 *

 *  DESCRIPTION

 *	These macros represent commonly used code to take information from

 *	a VFB and write it out to the output file.  This code is placed

 *	into macros to make it easier to maintain the same algorithm used

 *	by multiple functions, without the additional overhead of another

 *	subroutine call.

 *

 *	IMCOPYPLANE() uses the given query functions (such as ImVfbQIndex8()

 *	or ImVfbQRed()), calls it to get the next pixel value, and copies

 *	it into the run buffer.  After each scanline, it writes out the

 *	run buffer.

 *

 *	IMENCODEPLANE() uses the query function, calls it to get the next

 *	pixel value, and worries about run-length encoding (using

 *	IMADDTOBUFFER()).  After each scanline, it writes out the run buffer.

 *

 *	These macros assume that all the appropriate variables have been

 *	declared by the caller.

 */



#define IMCOPYPLANE(query,type,size)					\
{									\
	for ( y = 0; y < height; y++ )					\
	{								\
		rbp = runBuffer;					\
		for ( x = 0; x < width; x++ )				\
		{							\
			*rbp++ = query( vfb, pPixel );			\
			ImVfbSInc( vfb, pPixel );			\
		}							\
		Write( runBuffer, type, size, width );			\
	}								\
}



#define IMENCODEPLANE(query,type,size)					\
{									\
	for ( y = 0; y < height; y++ )					\
	{								\
		rbp      = runBuffer;					\
		oldIndex = query( vfb, pPixel );			\
		count    = 1;						\
		ImVfbSInc( vfb, pPixel );				\
		pCount   = rbp++;					\
		*rbp++   = oldIndex;					\
		for ( x = 1; x < width; x++ )				\
		{							\
			index = query( vfb, pPixel );			\
			ImVfbSInc( vfb, pPixel );			\
			IMADDTOBUFFER( );				\
		}							\
		if ( count < 0 )					\
			*pCount = ((unsigned char)(-count)) | 0x80;		\
		else							\
			*pCount  = (unsigned char)count;			\
		Write( runBuffer, type, size, (rbp - runBuffer) );	\
		nBytes += (rbp - runBuffer);				\
	}								\
}









/*

 *  FUNCTION

 *	imHdfVfbWrite8		-  write 8-bit uncomp. VFB

 *	imHdfVfbWriteRLE8	-  write 8-bit RLE comp. VFB

 *

 *	imHdfVfbWrite16		-  write 16-bit uncomp. VFB

 *

 *	imHdfVfbWriteRGB	-  write 24-bit uncomp. uninterleaved VFB

 *	imHdfVfbWriteRGBLine	-  write 24-bit uncomp. line interleaved VFB

 *	imHdfVfbWriteRGBPlane	-  write 24-bit uncomp. plane interleaved VFB

 *

 *	imHdfVfbWriteRLERGB	-  write 24-bit RLE comp. uninterleaved VFB

 *	imHdfVfbWriteRLERGBLine	-  write 24-bit RLE comp. line interleaved VFB

 *	imHdfVfbWriteRLERGBPlane-  write 24-bit RLE comp. plane interleaved VFB

 *

 *  DESCRIPTION

 *	The given VFB is written out as an 8-bit, 16-bit, or 24-bit,

 *	uncompressed or RLE compressed, uninterleaved, scanline interleaved,

 *	or plane interleaved image.

 */



static long				/* Returns number of bytes	*/

#ifdef __STDC__

imHdfVfbWrite8( int ioType, int fd, FILE *fp, ImVfb *vfb )

#else

imHdfVfbWrite8( ioType, fd, fp, vfb )

	int      ioType;		/* I/O flags			*/

	int      fd;			/* Output file descriptor	*/

	FILE    *fp;			/* Output file pointer		*/

	ImVfb   *vfb;			/* VFB to write out		*/

#endif

{

	int      width, height;		/* Image size			*/

	int      x, y;			/* X and Y image coordinates	*/

	unsigned char   *runBuffer;		/* Buffered up runs		*/

	unsigned char   *rbp;			/* Run buffer pointer		*/

	ImVfbPtr pPixel;		/* VFB pixel pointer		*/





	/*

	 *  Write out the uncompressed 8-bit index VFB.

	 */

	pPixel = ImVfbQFirst( vfb );

	width  = ImVfbQWidth( vfb );

	height = ImVfbQHeight( vfb );

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * width );



	IMCOPYPLANE( ImVfbQIndex8, UCHAR, 1 );



	free( (char *)runBuffer );



	return ( width * height );

}



static long				/* Returns number of bytes	*/

#ifdef __STDC__

imHdfVfbWriteRLE8( int ioType, int fd, FILE *fp, ImVfb *vfb )

#else

imHdfVfbWriteRLE8( ioType, fd, fp, vfb )

	int      ioType;		/* I/O flags			*/

	int      fd;			/* Output file descriptor	*/

	FILE    *fp;			/* Output file pointer		*/

	ImVfb   *vfb;			/* VFB to write out		*/

#endif

{

	int      width, height;		/* Image size			*/

	int      x, y;			/* X and Y image coordinates	*/

	unsigned char   *runBuffer;		/* Buffered up runs		*/

	unsigned char   *rbp;			/* Run buffer pointer		*/

	unsigned char   *pCount;		/* Last run's count byte	*/



	int      index;			/* New pixel index		*/

	int      oldIndex;		/* Old pixel index		*/

	int      count;			/* Run length			*/

	ImVfbPtr pPixel;		/* VFB pixel pointer		*/



	long     nBytes = 0;		/* Total number of bytes written*/





	/*

	 *  Run-length encode the 8-bit index VFB.

	 *	Worst case compression:

	 *		input:   ABB

	 *		output:  1A2BB

	 *	Worst case takes 5/3 * width bytes.  Run buffer is

	 *	allocated to be big enough for the worst case.

	 */

	pPixel = ImVfbQFirst( vfb );

	width  = ImVfbQWidth( vfb );

	height = ImVfbQHeight( vfb );

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * width * 2 );



	IMENCODEPLANE( ImVfbQIndex8, UCHAR, 1 );



	free( (char *)runBuffer );



	return ( nBytes );

}



static long				/* Returns number of bytes	*/

#ifdef __STDC__

imHdfVfbWrite16( int ioType, int fd, FILE *fp, ImVfb *vfb )

#else

imHdfVfbWrite16( ioType, fd, fp, vfb )

	int      ioType;		/* I/O flags			*/

	int      fd;			/* Output file descriptor	*/

	FILE    *fp;			/* Output file pointer		*/

	ImVfb   *vfb;			/* VFB to write out		*/

#endif

{

	int      width, height;		/* Image size			*/

	int      x, y;			/* X and Y image coordinates	*/

	sdsc_uint32  *runBuffer;		/* Buffered up runs		*/

	sdsc_uint32  *rbp;			/* Run buffer pointer		*/

	ImVfbPtr pPixel;		/* VFB pixel pointer		*/

	ImClt   *clt;			/* VFB's clt			*/

	int      n;			/* Number of colors		*/

	int      b;			/* Number of bytes when in file	*/





	/*

	 *  Write out the uncompressed 16-bit index VFB.

	 *	Worst case compression:

	 *		input:   ABB

	 *		output:  1A2BB

	 *	Worst case takes 5/3 * width bytes.  Run buffer is

	 *	allocated to be big enough for the worst case.

	 */

	pPixel = ImVfbQFirst( vfb );

	width  = ImVfbQWidth( vfb );

	height = ImVfbQHeight( vfb );

	ImMalloc( runBuffer, sdsc_uint32 *, sizeof( sdsc_uint32 ) * width * 2 );



	clt = ImVfbQClt( vfb );

	if ( clt != IMCLTNULL )

	{

		n = ImCltQNColors( clt );

		if ( n <= 256 )			/* 8-bit		*/

			b = 1;

		else				/* 16-bit		*/

			b = 2;

	}

	else

		b = 2;



	IMCOPYPLANE( ImVfbQIndex16, UINT32, b );



	free( (char *)runBuffer );



	return ( width * height );

}



static long				/* Returns number of bytes	*/

#ifdef __STDC__

imHdfVfbWriteRGB( int ioType, int fd, FILE *fp, ImVfb *vfb )

#else

imHdfVfbWriteRGB( ioType, fd, fp, vfb )

	int      ioType;		/* I/O flags			*/

	int      fd;			/* Output file descriptor	*/

	FILE    *fp;			/* Output file pointer		*/

	ImVfb   *vfb;			/* VFB to write out		*/

#endif

{

	int      width, height;		/* Image size			*/

	int      width3;		/* Width * 3			*/

	int      x, y;			/* X and Y image coordinates	*/

	unsigned char   *runBuffer;		/* Buffered up runs		*/

	unsigned char   *rbp;			/* Run buffer pointer		*/

	ImVfbPtr pPixel;		/* VFB pixel pointer		*/





	/*

	 *  Write out the uncompressed 24-bit RGB VFB, uninterleaved.

	 */

	pPixel = ImVfbQFirst( vfb );

	width  = ImVfbQWidth( vfb );

	width3 = width * 3;

	height = ImVfbQHeight( vfb );

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * width3 );



	for ( y = 0; y < height; y++ )

	{

		rbp = runBuffer;

		for ( x = 0; x < width; x++ )

		{

			*rbp++ = ImVfbQRed( vfb, pPixel );

			*rbp++ = ImVfbQGreen( vfb, pPixel );

			*rbp++ = ImVfbQBlue( vfb, pPixel );

			ImVfbSInc( vfb, pPixel );

		}



		Write( runBuffer, UCHAR, 1, width3 );

	}

	free( (char *)runBuffer );



	return ( width3 * height );

}



static long				/* Returns number of bytes	*/

#ifdef __STDC__

imHdfVfbWriteRGBLine( int ioType, int fd, FILE *fp, ImVfb *vfb )

#else

imHdfVfbWriteRGBLine( ioType, fd, fp, vfb )

	int      ioType;		/* I/O flags			*/

	int      fd;			/* Output file descriptor	*/

	FILE    *fp;			/* Output file pointer		*/

	ImVfb   *vfb;			/* VFB to write out		*/

#endif

{

	int      width, height;		/* Image size			*/

	int      x, y;			/* X and Y image coordinates	*/

	unsigned char   *runBuffer;		/* Buffered up runs		*/

	unsigned char   *rbp;			/* Run buffer pointer		*/

	ImVfbPtr pPixel;		/* VFB pixel pointer		*/

	ImVfbPtr pPixel2;		/* Start of scanline pixel ptr	*/





	/*

	 *  Write out the uncompressed 24-bit RGB VFB, scanline interleaved.

	 */

	pPixel = ImVfbQFirst( vfb );

	width  = ImVfbQWidth( vfb );

	height = ImVfbQHeight( vfb );

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * width );



	for ( y = 0; y < height; y++ )

	{

		pPixel2 = pPixel;

		rbp = runBuffer;

		for ( x = 0; x < width; x++ )

		{

			*rbp++ = ImVfbQRed( vfb, pPixel );

			ImVfbSInc( vfb, pPixel );

		}

		Write( runBuffer, UCHAR, 1, width );



		pPixel = pPixel2;

		rbp = runBuffer;

		for ( x = 0; x < width; x++ )

		{

			*rbp++ = ImVfbQGreen( vfb, pPixel );

			ImVfbSInc( vfb, pPixel );

		}

		Write( runBuffer, UCHAR, 1, width );



		pPixel = pPixel2;

		rbp = runBuffer;

		for ( x = 0; x < width; x++ )

		{

			*rbp++ = ImVfbQBlue( vfb, pPixel );

			ImVfbSInc( vfb, pPixel );

		}

		Write( runBuffer, UCHAR, 1, width );

	}

	free( (char *)runBuffer );



	return ( width * height * 3 );

}



static long				/* Returns number of bytes	*/

#ifdef __STDC__

imHdfVfbWriteRGBPlane( int ioType, int fd, FILE *fp, ImVfb *vfb )

#else

imHdfVfbWriteRGBPlane( ioType, fd, fp, vfb )

	int      ioType;		/* I/O flags			*/

	int      fd;			/* Output file descriptor	*/

	FILE    *fp;			/* Output file pointer		*/

	ImVfb   *vfb;			/* VFB to write out		*/

#endif

{

	int      width, height;		/* Image size			*/

	int      x, y;			/* X and Y image coordinates	*/

	unsigned char   *runBuffer;		/* Buffered up runs		*/

	unsigned char   *rbp;			/* Run buffer pointer		*/

	ImVfbPtr pPixel;		/* VFB pixel pointer		*/





	/*

	 *  Write out the uncompressed 24-bit RGB VFB, plane interleaved.

	 */

	width  = ImVfbQWidth( vfb );

	height = ImVfbQHeight( vfb );

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * width );



	pPixel = ImVfbQFirst( vfb );

	IMCOPYPLANE( ImVfbQRed, UCHAR, 1 );



	pPixel = ImVfbQFirst( vfb );

	IMCOPYPLANE( ImVfbQGreen, UCHAR, 1 );



	pPixel = ImVfbQFirst( vfb );

	IMCOPYPLANE( ImVfbQBlue, UCHAR, 1 );



	free( (char *)runBuffer );



	return ( width * height * 3 );

}



static long				/* Returns number of bytes	*/

#ifdef __STDC__

imHdfVfbWriteRLERGB( int ioType, int fd, FILE *fp, ImVfb *vfb )

#else

imHdfVfbWriteRLERGB( ioType, fd, fp, vfb )

	int      ioType;		/* I/O flags			*/

	int      fd;			/* Output file descriptor	*/

	FILE    *fp;			/* Output file pointer		*/

	ImVfb   *vfb;			/* VFB to write out		*/

#endif

{

	int      width, height;		/* Image size			*/

	int      width3;		/* 3 times width		*/

	int      x, y;			/* X and Y image coordinates	*/

	unsigned char   *runBuffer;		/* Buffered up runs		*/

	unsigned char   *rbp;			/* Run buffer pointer		*/

	unsigned char   *pCount;		/* Last run's count byte	*/



	int      index;			/* New pixel index		*/

	int      oldIndex;		/* Old pixel index		*/

	int      count;			/* Run length			*/

	ImVfbPtr pPixel;		/* VFB pixel pointer		*/

	int      onRGB;			/* Which RGB component are we on?*/



	long     nBytes = 0;		/* Total number of bytes written*/





	/*

	 *  Run-length encode the 24-bit RGB VFB, uninterleaved.

	 *	Worst case compression:

	 *		input:   ABB

	 *		output:  1A2BB

	 *	Worst case takes 5/3 * width * 3 bytes.  Run buffer is

	 *	allocated to be big enough for the worst case.

	 */

	pPixel = ImVfbQFirst( vfb );

	width  = ImVfbQWidth( vfb );

	height = ImVfbQHeight( vfb );

	width3 = width * 3;

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * width3 * 2 );



	for ( y = 0; y < height; y++ )

	{

		rbp      = runBuffer;

		oldIndex = ImVfbQRed( vfb, pPixel );

		onRGB    = GREEN;

		count    = 1;



		pCount   = rbp++;

		*rbp++   = oldIndex;



		for ( x = 1; x < width3; x++ )

		{

			switch ( onRGB )

			{

			case RED:	index = ImVfbQRed( vfb, pPixel );

					onRGB = GREEN;

					break;

			case GREEN:	index = ImVfbQGreen( vfb, pPixel );

					onRGB = BLUE;

					break;

			case BLUE:	index = ImVfbQBlue( vfb, pPixel );

					ImVfbSInc( vfb, pPixel );

					onRGB = RED;

					break;

			}

			IMADDTOBUFFER( );

		}

		if ( count < 0 )

			*pCount = ((unsigned char)(-count)) | 0x80;

		else

			*pCount  = (unsigned char)count;



		/*  Write out the run buffer.				*/

		Write( runBuffer, UCHAR, 1, (rbp - runBuffer) );

		nBytes += (rbp - runBuffer);

	}

	free( (char *)runBuffer );



	return ( nBytes );

}



static long				/* Returns number of bytes	*/

#ifdef __STDC__

imHdfVfbWriteRLERGBLine( int ioType, int fd, FILE *fp, ImVfb *vfb )

#else

imHdfVfbWriteRLERGBLine( ioType, fd, fp, vfb )

	int      ioType;		/* I/O flags			*/

	int      fd;			/* Output file descriptor	*/

	FILE    *fp;			/* Output file pointer		*/

	ImVfb   *vfb;			/* VFB to write out		*/

#endif

{

	int      width, height;		/* Image size			*/

	int      x, y;			/* X and Y image coordinates	*/

	unsigned char   *runBuffer;		/* Buffered up runs		*/

	unsigned char   *rbp;			/* Run buffer pointer		*/

	unsigned char   *pCount;		/* Last run's count byte	*/



	int      index;			/* New pixel index		*/

	int      oldIndex;		/* Old pixel index		*/

	int      count;			/* Run length			*/

	ImVfbPtr pPixel;		/* VFB pixel pointer		*/

	ImVfbPtr pPixel2;		/* Start of line pixel pointer	*/



	long     nBytes = 0;		/* Total number of bytes written*/





	/*

	 *  Run-length encode the 24-bit RGB VFB, line interleaved.

	 *	Worst case compression:

	 *		input:   ABB

	 *		output:  1A2BB

	 *	Worst case takes 5/3 * width bytes * 3.  Run buffer is

	 *	allocated to be big enough for the worst case.

	 */

	pPixel = ImVfbQFirst( vfb );

	width  = ImVfbQWidth( vfb );

	height = ImVfbQHeight( vfb );

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * width * 6 );



	for ( y = 0; y < height; y++ )

	{

		rbp = runBuffer;



		/* Red part of scan-line.				*/

		pPixel2  = pPixel;

		oldIndex = ImVfbQRed( vfb, pPixel );

		ImVfbSInc( vfb, pPixel );

		count    = 1;

		pCount   = rbp++;

		*rbp++   = oldIndex;

		for ( x = 1; x < width; x++ )

		{

			index = ImVfbQRed( vfb, pPixel );

			ImVfbSInc( vfb, pPixel );

			IMADDTOBUFFER( );

		}

		if ( count < 0 )

			*pCount = ((unsigned char)(-count)) | 0x80;

		else

			*pCount  = (unsigned char)count;



		/* Green part of scan-line.				*/

		pPixel   = pPixel2;

		oldIndex = ImVfbQGreen( vfb, pPixel );

		ImVfbSInc( vfb, pPixel );

		count    = 1;

		pCount   = rbp++;

		*rbp++   = oldIndex;

		for ( x = 1; x < width; x++ )

		{

			index = ImVfbQGreen( vfb, pPixel );

			ImVfbSInc( vfb, pPixel );

			IMADDTOBUFFER( );

		}

		if ( count < 0 )

			*pCount = ((unsigned char)(-count)) | 0x80;

		else

			*pCount  = (unsigned char)count;



		/* Blue part of scan-line.				*/

		pPixel   = pPixel2;

		oldIndex = ImVfbQBlue( vfb, pPixel );

		ImVfbSInc( vfb, pPixel );

		count    = 1;

		pCount   = rbp++;

		*rbp++   = oldIndex;

		for ( x = 1; x < width; x++ )

		{

			index = ImVfbQBlue( vfb, pPixel );

			ImVfbSInc( vfb, pPixel );

			IMADDTOBUFFER( );

		}

		if ( count < 0 )

			*pCount = ((unsigned char)(-count)) | 0x80;

		else

			*pCount  = (unsigned char)count;



		/*  Write out the run buffer.				*/

		Write( runBuffer, UCHAR, 1, (rbp - runBuffer) );

		nBytes += (rbp - runBuffer);

	}

	free( (char *)runBuffer );



	return ( nBytes );

}



static long				/* Returns number of bytes	*/

#ifdef __STDC__

imHdfVfbWriteRLERGBPlane( int ioType, int fd, FILE *fp, ImVfb *vfb )

#else

imHdfVfbWriteRLERGBPlane( ioType, fd, fp, vfb )

	int      ioType;		/* I/O flags			*/

	int      fd;			/* Output file descriptor	*/

	FILE    *fp;			/* Output file pointer		*/

	ImVfb   *vfb;			/* VFB to write out		*/

#endif

{

	int      width, height;		/* Image size			*/

	int      x, y;			/* X and Y image coordinates	*/

	unsigned char   *runBuffer;		/* Buffered up runs		*/

	unsigned char   *rbp;			/* Run buffer pointer		*/

	unsigned char   *pCount;		/* Last run's count byte	*/



	int      index;			/* New pixel index		*/

	int      oldIndex;		/* Old pixel index		*/

	int      count;			/* Run length			*/

	ImVfbPtr pPixel;		/* VFB pixel pointer		*/



	long     nBytes = 0;		/* Total number of bytes written*/





	/*

	 *  Run-length encode the 24-bit RGB VFB, uninterleaved.

	 *	Worst case compression:

	 *		input:   ABB

	 *		output:  1A2BB

	 *	Worst case takes 5/3 * width bytes.  Run buffer is

	 *	allocated to be big enough for the worst case.

	 */

	width  = ImVfbQWidth( vfb );

	height = ImVfbQHeight( vfb );

	ImMalloc( runBuffer, unsigned char *, sizeof( unsigned char ) * width * 2 );



	/* Red plane.							*/

	pPixel = ImVfbQFirst( vfb );

	IMENCODEPLANE( ImVfbQRed, UCHAR, 1 );



	/* Green plane.							*/

	pPixel = ImVfbQFirst( vfb );

	IMENCODEPLANE( ImVfbQGreen, UCHAR, 1 );



	/* Blue plane.							*/

	pPixel = ImVfbQFirst( vfb );

	IMENCODEPLANE( ImVfbQBlue, UCHAR, 1 );



	free( (char *)runBuffer );



	return ( nBytes );

}

