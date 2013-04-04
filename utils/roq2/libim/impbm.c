/**

 **	$Header: /roq/libim/impbm.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/impbm.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	impbm.c	-  I/O for Jef Poskanzer's PBM file formats

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	impbm.c contains routines to read and write the various PBM suite

 **	image files for the image manipulation library.  Raster data read

 **	in is stored in a VFB.  Raster data written out is taken from a

 **	tag table.

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

 **	imPbmRead	f  read a PBM file

 **

 **	imPbmWrite1	f  write a 1-bit ASCII PBM file

 **	imPbmWrite8	f  write an 8-bit ASCII PGM file

 **	imPbmWriteRGB	f  write an RGB ASCII PPM file

 **	imRpbmWrite1	f  write an 1-bit raw PBM file

 **	imRpbmWrite8	f  write an 8-bit raw PGM file

 **	imRpbmWriteRGB	f  write an RGB raw PPM file

 **

 **	readAsciiInt	f  read in an ASCII string integer

 **

 **  HISTORY

 **	$Log: /roq/libim/impbm.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.15  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.14  1995/06/15  20:34:11  bduggan

 **	added casts for strlen

 **

 **	Revision 1.13  1995/05/17  23:46:16  bduggan

 **	Merged everything into one format, with an ascii compression option

 **

 **	Revision 1.12  1995/04/03  21:32:07  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.11  1995/01/10  23:35:34  bduggan

 **	uncapitlized i's in local functions

 **	made read/write routines static

 **

 **	Revision 1.10  1994/10/03  11:30:19  nadeau

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

 **	Revision 1.10  1994/10/03  11:30:19  nadeau

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

 **	Revision 1.9  92/12/03  01:50:12  nadeau

 **	Corrected info messages.

 **	

 **	Revision 1.8  92/11/24  11:51:57  groening

 **	Removed use of IMINFOMSG and added file format table

 **	declarations and initializations.

 **	

 **	Revision 1.7  92/10/19  14:17:09  groening

 **	fixed bug on read mono

 **	added ImINfo statements

 **	

 **	Revision 1.6  92/08/31  17:29:10  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.5  92/04/09  09:32:36  groening

 **	To make the compiler happy added extern statements.

 **	

 **	Revision 1.4  92/04/08  10:48:30  nadeau

 **	Fixed SGI ANSI cc complaints about passing unsigned char arrays

 **	as args to functions expecting char arrays.  Big deal.

 **	

 **	Revision 1.3  91/10/03  09:14:47  nadeau

 **	Fixed #includes.  Fixed monochrome write bug that

 **	inverted meaning of black and white.

 **	

 **	Revision 1.2  91/03/08  14:29:15  nadeau

 **	Nearly a total rewrite.  Ran it all through 'indent', the UNIX

 **	C beautifier, just to make it readable.  Rearranged.  Restructured.

 **	Rewrote.  Added comments.  Removed meaningless ones.  Added code

 **	to work with the latest enhancements to ImFileRead and ImFileWrite.

 **	Basically a total rewrite.

 **	

 **	Revision 1.1  91/01/18  15:40:34  doeringd

 **	Initial revision

 **/



#include <ctype.h>

#include "iminternal.h"



/**

 **  FORMAT

 **	PBM	-  Jef Poskanzer's PBM file formats

 **

 **  AKA

 **	rpbm,pgm,rpgm,ppm,rppm

 **

 **  FORMAT REFERENCES

 **	Graphics File Formats, David C. Kay, John R. Levine

 **

 **  CODE CREDITS

 **     Custom development, Donald Doering, San Diego Supercomputer Center, 1991.

 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1991.

 **

 **  DESCRIPTION

 **	The PBM suite really defines six (6) file formats:

 **

 **		PBM	ASCII Monochrome bitmap

 **		PGM	ASCII Grayscale pixel map

 **		PPM	ASCII Color pixel map

 **

 **		RPBM	Raw Monochrome bitmap

 **		RPGM	Raw Grayscale pixel map

 **		RPPM	Raw Color pixel map

 **

 **	The ASCII formats store the image as ASCII, human-readable files,

 **	while the raw formats store them as raw binary files.  Since the

 **	binary files are still only byte-based (8-bit grayscale, 8-bit per

 **	channel RGB), they are still portable (but a lot smaller and faster).

 **

 **	All six formats start with a file header of the form:

 **

 **		unsigned char magic1;

 **		unsigned char magic2;

 **		unsigned int  width, height;

 **

 **	magic1 is always a 'P'.

 **

 **	magic2 selects which of the six formats:

 **

 **		'1'	PBM

 **		'2'	PGM

 **		'3'	PPM

 **		'4'	RPBM

 **		'5'	RPGM

 **		'6'	RPPM

 **

 **	width and height are the width and height of the image in pixels.

 **

 **	For formats other than PBM and RPBM, the header also contains:

 **

 **		unsigned int  maxIntensity;

 **

 **	maxIntensity is the highest value allowed for one channel of the

 **	image.  For instance, for 8-bit grayscale, maxIntensity is 255.

 **	For 24-bit RGB images, maxIntensity is also 255 (8-bit channels).

 **

 **	When reading images, incomming pixel values are scaled from the

 **	range 0-maxIntensity, up or down to the VFB 8-bit range 0-255.

 **

 **	For monochrome PBM and RPBM files, 0 is white and 1 is black.  This

 **	is the opposite of the image library's conventions.  However, PGM,

 **	RPGM, PPM, and RPPM files all follow the convention that 0 is black,

 **	and 1 is brighter than black (255 is full intensity).

 **

 **	NOTE:  the header is always ASCII, including maxIntensity.

 **

 **  ASCII FORMATS

 **	For the three ASCII formats, the rest of the file is filled with

 **	ASCII integer values separated by white space.  Carriage returns,

 **	form feeds, spaces, and tabs are ignored.  Comments starting with

 **	a '#' and extending to the end of the line are ignored.

 **

 **	When we write ASCII formats, to make the format more readable,

 **	carriage returns are inserted after every 70 characters or so.

 **

 **  BINARY FORMATS

 **	For the three "raw" binary formats, the rest of the file is filled

 **	with 8-bit values.

 **/



/*

 *  PBM         - ASCII bitmap  (monochrome)

 *  PGM         - ASCII gray    (grayscale)

 *  PNM         - any ASCII format

 *  PPM         - ASCII pixel   (RGB color)

 *  RPBM        - Raw bitmap    (monochrome)

 *  RPGM        - Raw gray      (grayscale)

 *  RPNM        - any Raw format

 *  RPPM        - Raw pixel     (RGB color)

 *

 *  All 8 formats share the same read call.  Some share the same write calls.

 */

#ifdef __STDC__

static int imPbmRead(int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

static int imPbmWrite1( ImFileFormatWriteMap * pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imPbmWrite8( ImFileFormatWriteMap * pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imPbmWriteRGB( ImFileFormatWriteMap * pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imRpbmWrite1( ImFileFormatWriteMap * pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imRpbmWrite8( ImFileFormatWriteMap * pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imRpbmWriteRGB( ImFileFormatWriteMap * pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

#else

static int imPbmRead( );

static int imPbmWrite1( ), imPbmWrite8( ), imPbmWriteRGB( );

static int imRpbmWrite1( ), imRpbmWrite8( ), imRpbmWriteRGB( );

#endif





/*

 *  Magic constants

 */

#define IMPBM_MAGIC1	'P'

#define IMPBM_MAGIC2	'1'

#define IMPGM_MAGIC2	'2'

#define IMPPM_MAGIC2	'3'

#define IMRPBM_MAGIC2	'4'

#define IMRPGM_MAGIC2	'5'

#define IMRPPM_MAGIC2	'6'





static char *imPbmNames[ ] = { "pbm", "rpbm", "pgm", "rpgm", "pnm", "rpnm", "ppm", "rppm", NULL };



static unsigned char imPbmMagicNumber[ ] = { IMPBM_MAGIC1, IMPBM_MAGIC2 };

static unsigned char imRpbmMagicNumber[ ] = { IMPBM_MAGIC1, IMRPBM_MAGIC2 };

static unsigned char imPgmMagicNumber[ ] = { IMPBM_MAGIC1, IMPGM_MAGIC2 };

static unsigned char imRpgmMagicNumber[ ] = { IMPBM_MAGIC1, IMRPGM_MAGIC2 };

static unsigned char imPpmMagicNumber[ ] = { IMPBM_MAGIC1, IMPPM_MAGIC2 };

static unsigned char imRppmMagicNumber[ ] = { IMPBM_MAGIC1, IMRPPM_MAGIC2 };





static ImFileFormatReadMap imPbmReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IN,1,1,       0,      IMVFBMONO,      0 },

        { IN,1,8,       0,      IMVFBINDEX8,    0 },

        { RGB,3,8,      0,      IMVFBRGB,       0 },

        { IN,1,1,       ASC,    IMVFBMONO,      0 },

        { IN,1,8,       ASC,    IMVFBINDEX8,    0 },

        { RGB,3,8,      ASC,    IMVFBRGB,       0 },

        { -1,           0,      -1,             0 },

};





static ImFileFormatWriteMap imPbmWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBMONO,    0,      IN,1,1,         ASC,    imPbmWrite1 },

        { IMVFBMONO,    0,      IN,1,1,         0,      imRpbmWrite1 },

        { IMVFBINDEX8,  0,      IN,1,8,         ASC,    imPbmWrite8 },

        { IMVFBINDEX8,  0,      IN,1,8,         0,      imRpbmWrite8 },

        { IMVFBRGB,     0,      RGB,3,8,        ASC,    imPbmWriteRGB },

        { IMVFBRGB,     0,      RGB,3,8,        0,      imRpbmWriteRGB },

        { -1,           0,      -1,             0,      NULL },

};







static ImFileMagic imFilePbmMagic []=

{

	{0, 2, imPbmMagicNumber },

	{0, 2, imRpbmMagicNumber },

	{0, 2, imPgmMagicNumber },

	{0, 2, imRpgmMagicNumber },

	{0, 2, imPpmMagicNumber },

	{0, 2, imRppmMagicNumber },

	{ 0, 0, NULL },

};



ImFileFormat ImFilePbmFormat =

{

	imPbmNames, "PBM Portable Bit Map file",

	"Jef Poskanzer",

	"1-bit monochrome, 8-bit grayscale, 24-bit rgb, ascii encoded or binary",

	"1-bit monochrome PBM images.",

	imFilePbmMagic,

	IMNOMULTI, IMPIPE,

	IMNOMULTI, IMPIPE,

	imPbmRead, imPbmReadMap, imPbmWriteMap

};







#ifdef __STDC__

static int skipEOL( int ioTyne, int fd, FILE *fp );

static int readAsciiInt( int ioType, int fd, FILE *fp, int *value );

#else

static int skipEOL( );

static int readAsciiInt();

#endif





#define IMWrite(ioType,fd,fp,buffer,size)				\
	((ioType&IMFILEIOFD) ?						\
		write( fd, buffer, size )				\
	:								\
		fwrite( buffer, 1, size, fp ))




/*

 *  FUNCTION

 *	imPbmRead	-  read a PBM file

 *

 *  DESCRIPTION

 *	The header is read in, followed by the image pixels in either

 *	ASCII or raw format.

 */



static int			       /* Returns status		*/

#ifdef __STDC__

imPbmRead(int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imPbmRead(ioType, fd, fp, flagsTable, tagTable)

	int     ioType;		       /* I/O type			*/

	int     fd;		       /* Input file descriptor		*/

	FILE   *fp;		       /* Input file buffer pointer	*/

	TagTable *flagsTable;	       /* Input parameter/flags		*/

	TagTable *tagTable;	       /* Tag table to add to		*/

#endif

{

	ImVfbPtr pPixel;	/* VFB pixel pointer		*/

	unsigned char *pBuffer;	/* Buffer pointer		*/

	int bitShift;		/* Shift amount for expanding Mono*/

	int	width, height;		/* Image size			*/

	int     fields;			/* VFB fields			*/

	int     maxIntensity;		/* Maximum channel intensity	*/

	float	colorScale;		/* Intensity scaling		*/

	unsigned char	magic[2];		/* Magic number			*/

	unsigned char   byte;			/* Byte holder			*/

	int	hue;			/* Incomming channel value	*/

	ImVfb   *vfb;			/* New VFB			*/

	unsigned char	*buffer;		/* Incomming byte buffer	*/

	int	x, y;			/* VFB location			*/

	char		message[100];	/* ImInfo message		*/





	/*

	 * Get the magic number

	 */

	BinByteOrder( BINMBF );

	ImInfo ("Byte Order","Most Significant Byte First");

	if ( ImBinRead( ioType, fd, fp, magic, UCHAR, 1, 2 ) == -1 )

		ImReturnBinError( );

	if ( magic[0] != IMPBM_MAGIC1 )

	{

		ImErrNo = IMEMAGIC;

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	switch ( magic[1] )

	{

	case IMPBM_MAGIC2:

	case IMRPBM_MAGIC2:

		fields = IMVFBMONO;

		break;



	case IMPGM_MAGIC2:

	case IMRPGM_MAGIC2:

		fields = IMVFBINDEX8;

		break;



	case IMPPM_MAGIC2:

	case IMRPPM_MAGIC2:

		fields = IMVFBRGB;

		break;



	default:

		ImErrNo = IMEMAGIC;

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}





	/*

	 *  Read image size.

	 */

	if ( readAsciiInt( ioType, fd, fp, &width ) == -1)

		return ( -1 );		/* Error already handled	*/

	if ( readAsciiInt( ioType, fd, fp, &height ) == -1)

		return ( -1 );		/* Error already handled	*/





	sprintf (message, "%d x %d",width, height);

	ImInfo ("Resolution",message);		

	switch ( magic[1] )

	{

	case IMPBM_MAGIC2:

		ImInfo ("Type","1-bit Monochrome"); 

		ImInfo ("Compression", "Ascii");

		break;

	case IMRPBM_MAGIC2:

		ImInfo ("Type","1-bit Monochrome"); 

		ImInfo ("Compression", "none");

		break;



	case IMPGM_MAGIC2:

		ImInfo ("Type","8-bit Grayscale"); 

		ImInfo ("Compression", "Ascii");

		break;

	case IMRPGM_MAGIC2:

		ImInfo ("Type","8-bit Grayscale"); 

		ImInfo ("Compression", "none");

		break;



	case IMPPM_MAGIC2:

		ImInfo ("Type","24-bit RGB"); 

		ImInfo ("Compression", "Ascii");

		break;

	case IMRPPM_MAGIC2:

		ImInfo ("Type","24-bit RGB"); 

		ImInfo ("Compression", "none");

		break;

	}



	/*

	 *  Except for PBM and RPBM formats, read in the maximum channel

	 *  value 'maxIntensity'.

	 *

	 *  Compute how to scale color values from their incomming value

	 *  range of 0-maxIntensity, to our range of 0-255 (8-bit indexes

	 *  and RGB components).

	 */

	maxIntensity = 0;

	if ( magic[1] != IMPBM_MAGIC2 && magic[1] != IMRPBM_MAGIC2 )

	{

		if ( readAsciiInt( ioType, fd, fp, &maxIntensity ) == -1 )

			return ( -1 );	/* Error already handled	*/

		colorScale = 255.0 / (float) maxIntensity;

	}

	skipEOL( ioType, fd, fp );





	/*

	 *  Allocate a VFB of the required size.

	 */

	if ((vfb = ImVfbAlloc( width, height, fields )) == IMVFBNULL)

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}





	/*

	 *  Read in the image, depending upon how it is stored.

	 */

	switch ( magic[1] )

	{

	case IMPPM_MAGIC2:	/* ASCII Color				*/

		pPixel = ImVfbQFirst( vfb );

		for (y = 0; y < height; y++)

		{

			for ( x = 0; x < width; x++)

			{

				if ( readAsciiInt( ioType, fd, fp, &hue ) == -1)

					return (-1);

				ImVfbSRed( vfb, pPixel, colorScale * hue );



				if ( readAsciiInt( ioType, fd, fp, &hue ) == -1)

					return (-1);

				ImVfbSGreen( vfb, pPixel, colorScale * hue );



				if ( readAsciiInt( ioType, fd, fp, &hue ) == -1)

					return (-1);

				ImVfbSBlue( vfb, pPixel, colorScale * hue );



				ImVfbSInc( vfb, pPixel );

			}

		}

		break;



	case IMRPPM_MAGIC2:	/* Raw Color				*/

		/* Skip carriage return.				*/

		ImMalloc( buffer, unsigned char *, width * 3 );

		pPixel = ImVfbQFirst( vfb );

		for (y = 0; y < height; y++)

		{

			if ( ImBinRead( ioType, fd, fp, buffer, UCHAR, 1, width * 3 ) == -1 )

			{

				free( (char *)buffer );

				ImReturnBinError( );

			}

			pBuffer = buffer;

			for ( x = 0; x < width; x++ )

			{

				ImVfbSRed(   vfb, pPixel, colorScale * *pBuffer++ );

				ImVfbSGreen( vfb, pPixel, colorScale * *pBuffer++ );

				ImVfbSBlue(  vfb, pPixel, colorScale * *pBuffer++ );

				ImVfbSInc( vfb, pPixel );

			}

		}

		free( (char *) buffer );

		break;



	case IMPGM_MAGIC2:	/* ASCII grayscale			*/

		pPixel = ImVfbQFirst( vfb );

		for (y = 0; y < height; y++)

		{

			for (x = 0; x < width; x++ )

			{

				if ( readAsciiInt( ioType, fd, fp, &hue ) == -1)

					return (-1);

				ImVfbSIndex8( vfb, pPixel, colorScale * hue );

				ImVfbSInc( vfb, pPixel );

			}

		}

		break;



	case IMRPGM_MAGIC2:	/* Raw grayscale			*/

		/* Skip carriage return.				*/

		ImMalloc( buffer, unsigned char *, width );

		pPixel = ImVfbQFirst( vfb );

		for (y = 0; y < height; y++)

		{

			if ( ImBinRead( ioType, fd, fp, buffer, UCHAR, 1, width ) == -1 )

			{

				free( (char *)buffer );

				ImReturnBinError( );

			}

			pBuffer = buffer;

			for ( x = 0; x < width; x++ )

			{

				ImVfbSIndex8( vfb, pPixel, colorScale * *pBuffer++ );

				ImVfbSInc( vfb, pPixel );

			}

		}

		free( (char *) buffer );

		break;



	case IMPBM_MAGIC2:	/* ASCII bitmap (mono)			*/

		pPixel = ImVfbQFirst( vfb );

		for (y = 0; y < height; y++)

		{

			for (x = 0; x < width; x++ )

			{

				if ( readAsciiInt( ioType, fd, fp, &hue ) == -1)

					return (-1);

				/*

				 *  hue = 0 (white) maps to VFB 1 (white).

				 *  hue = 1 (black) maps to VFB 0 (black).

				 */

				ImVfbSMono( vfb, pPixel, (~hue) & 0x1 );

				ImVfbSInc( vfb, pPixel );

			}

		}

		break;



	case IMRPBM_MAGIC2:	/* Raw bitmap (mono)			*/

		/* Skip carriage return.				*/

		ImMalloc( buffer, unsigned char *, (width + 7) / 8 );

		pPixel = ImVfbQFirst( vfb );

		for (y = 0; y < height; y++)

		{

			bitShift = -1;

			if ( ImBinRead( ioType, fd, fp, buffer, UCHAR, 1, (width + 7) / 8 ) == -1 )

			{

				free( (char *)buffer );

				ImReturnBinError( );

			}

			pBuffer = buffer;

			for ( x = 0; x < width; x++ )

			{

				if (bitShift == -1)

				{

					byte = ~(*pBuffer++);

					bitShift = 7;

				}

				/*

				 *  hue = 0 (white) maps to VFB 1 (white).

				 *  hue = 1 (black) maps to VFB 0 (black).

				 */

				ImVfbSMono( vfb, pPixel,

					 !((byte >> bitShift--)& 0x1) );

				ImVfbSInc( vfb, pPixel );

			}

		}

		free( (char *) buffer );

		break;

	}





	/*

	 *  Add the VFB to the tagTable.

	 */

	TagTableAppend( tagTable, TagEntryAlloc( "image vfb", POINTER, &vfb ) );



	return ( 1 );

}











/*

 *  FUNCTION

 *	skipEOL		-  skip to end of line

 *	readAsciiInt	-  read in an ASCII string integer

 *

 *  DESCRIPTION

 *	White space and comments are skipped up to the first digit of a number.

 *	The number is read in and returned.  If a read problem occurs, a -1

 *	is returned as the function value.  Otherwise 0 is returned.

 */



static int				/* Returns status		*/

#ifdef __STDC__

skipEOL( int ioType, int fd, FILE *fp )

#else

skipEOL( ioType, fd, fp )

	int     ioType;			/* I/O type			*/

	int     fd;			/* File descriptor to use	*/

	FILE   *fp;			/* File pointer to use		*/

#endif

{

	int   ich;		/* Input character (as integer)	*/

	unsigned char ch;			/* Input character		*/



	ch = '\0';

	if ( ioType & IMFILEIOFD )

	{

		while ( ch != '\n' )

		{

			if ( read( fd, &ch, 1 ) <= 0 )

			{

				ImErrNo = IMESYNTAX;

				ImErrorFatal( ImQError( ), -1, ImErrNo );

			}

		}

		return ( 0 );

	}



	while ( (ich = getc( fp )) != '\n' && ich != EOF )

		;

	if ( ich == EOF )

	{

		ImErrNo = IMESYNTAX;

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	return ( 0 );

}



static int				/* Returns status		*/

#ifdef __STDC__

readAsciiInt( int ioType, int fd, FILE *fp, int *value )

#else

readAsciiInt( ioType, fd, fp, value )

	int     ioType;			/* I/O type			*/

	int     fd;			/* File descriptor to use	*/

	FILE   *fp;			/* File pointer to use		*/

	int  *value;		/* Returned integer value	*/

#endif

{

	int   ich;		/* Input character (as integer)	*/

	unsigned char ch;			/* Input character		*/



	if ( ioType & IMFILEIOFD )

	{

		/* Skip white space and comments that start with a '#'	*/

		do

		{

			if ( read( fd, &ch, 1 ) <= 0 )

			{

				ImErrNo = IMESYNTAX;

				ImErrorFatal( ImQError( ), -1, ImErrNo );

			}

			if ( ch == '#' )

			{

				if ( skipEOL( ioType, fd, fp ) == -1 )

					return ( -1 );

				ch = '\n';

			}

		} while ( isspace( ch ) );



		/* Read in and assemble an integer.			*/

		*value = 0;

		while ( isdigit( ch ) )

		{

			*value = *value * 10 + (ch - '0');

			if ( read( fd, &ch, 1 ) <= 0 )

			{

				ImErrNo = IMESYNTAX;

				ImErrorFatal( ImQError( ), -1, ImErrNo );

			}

		}

		return ( 0 );

	}



	/*

	 *  Skip white space and comments starting with '#'.  Read in

	 *  a single integer.

	 */

	while ( (ich = getc( fp )) == '#' || isspace( ich ) )

	{

		if ( ich == '#' )

			if ( skipEOL( ioType, fd, fp ) == -1 )

				return ( -1 );

	}

	if ( ich == EOF )

	{

		ImErrNo = IMESYNTAX;

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	ungetc( ich, fp );

	if ( fscanf( fp, "%d", value ) != 1 )

	{

		ImErrNo = IMESYNTAX;

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	return ( 0 );

}











/*

 *  FUNCTION

 *	imPbmWrite1	-  write a 1-bit ASCII PBM file

 *	imPbmWrite8	-  write an 8-bit ASCII PGM file

 *	imPbmWriteRGB	-  write an RGB ASCII PPM file

 *

 *  DESCRIPTION

 *	1-bit:  The image is written out in ASCII as a series of 1-bit values

 *	each in the form:  "i ".  To make the PBM file more readable,

 *	carriage returns are added every 70 characters or so.

 *

 *	8-bit:  The image is written out in ASCII as a series of 8-bit index

 *	values, each in the form:  "iii ".  To make the PGM file more readable,

 *	carriage returns are added every 70 characters or so.

 *

 *	RGB:  The image is written out in ASCII as a series of RGB values, each

 *	in the form:  "rrr ggg bbb  ".  To make the PPM file more readable,

 *	carriage returns are added every 70 characters or so (after the nearest

 *	complete tripplet).

 */



static int

#ifdef __STDC__

imPbmWrite1(ImFileFormatWriteMap * pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imPbmWrite1( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int     ioType;			/* I/O type flag		*/

	int     fd;			/* Unbuffered file descriptor	*/

	FILE   *fp;			/* Buffered file pointer	*/

	TagTable *flagsTable;		/* Max color value flag		*/

	TagTable *tagTable;		/* Tag list to read from	*/

#endif

{

	int       x, y;	/* Pixel location counters	*/

	ImVfbPtr  pPixel;	/* Current pixel		*/

	ImVfb    	  *vfb;		/* Image to output		*/

	int		   width;	/* Image width			*/

	int		   height;	/* Image height			*/

	unsigned char		   buffer[200];	/* Line buffer			*/

	unsigned char		  *pBuffer;	/* Current location in buffer	*/

	unsigned char		  *pBufferStop;	/* Where to stop & flush in buffer*/

	char		message[100];	/* ImInfo message		*/





	/*

	 *  Get the VFB and its attributes.  Write the PBM header.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	width  = ImVfbQWidth( vfb );

	height = ImVfbQHeight( vfb );

	sprintf( (char *)buffer, "%c%c\n%d %d\n", IMPBM_MAGIC1, IMPBM_MAGIC2,

		width, height );

	IMWrite( ioType, fd, fp, buffer, strlen( (char*)buffer ) );



	ImInfo ("Byte Order","Most Significant Byte First");

	ImInfo ("Type","1-bit Monochrome");

	sprintf (message, "%d x %d",width, height);

	ImInfo ("Resolution",message);

	ImInfo ("Compression", "Ascii");



	pPixel = ImVfbQFirst( vfb );

	pBufferStop = buffer + 70;	/* Flush soon after 70 chars	*/

	pBuffer = buffer;



	for ( y = 0; y < height; y++ )

	{

		for ( x = 0; x < width; x++ )

		{

			if ( pBuffer >= pBufferStop )

			{

				*pBuffer = '\n';

				IMWrite( ioType, fd, fp, buffer,pBuffer-buffer+1);

				pBuffer  = buffer;

			}

			sprintf( (char *)pBuffer, "%1d ",

				(~ImVfbQMono( vfb, pPixel )) & 0x1 );

			pBuffer += 2;

			ImVfbSInc( vfb, pPixel );

		}

		if ( pBuffer != buffer )

		{

			*pBuffer = '\n';

			IMWrite( ioType, fd, fp, buffer,pBuffer-buffer+1);

			pBuffer  = buffer;

		}

	}

	return ( 1 );

}



static int

#ifdef __STDC__

imPbmWrite8(ImFileFormatWriteMap * pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imPbmWrite8( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int     ioType;			/* I/O type flag		*/

	int     fd;			/* Unbuffered file descriptor	*/

	FILE   *fp;			/* Buffered file pointer	*/

	TagTable *flagsTable;		/* Max color value flag		*/

	TagTable *tagTable;		/* Tag list to read from	*/

#endif

{

	int       x, y;	/* Pixel location counters	*/

	ImVfbPtr  pPixel;	/* Current pixel		*/

	ImVfb    	  *vfb;		/* Image to output		*/

	int		   width;	/* Image width			*/

	int		   height;	/* Image height			*/

	unsigned char		   buffer[200];	/* Line buffer			*/

	unsigned char		  *pBuffer;	/* Current location in buffer	*/

	unsigned char		  *pBufferStop;	/* Where to stop & flush in buffer*/

	char		message[100];	/* ImInfo message		*/





	/*

	 *  Get the VFB and its attributes.  Write the PGM header.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	width  = ImVfbQWidth( vfb );

	height = ImVfbQHeight( vfb );

	sprintf( (char *)buffer, "%c%c\n%d %d\n255\n", IMPBM_MAGIC1, IMPGM_MAGIC2,

		width, height );

	IMWrite( ioType, fd, fp, buffer, strlen((char*) buffer ) );



	ImInfo ("Byte Order","Most Significant Byte First");

	ImInfo ("Type","8-bit Grayscale");

	sprintf (message, "%d x %d",width, height);

	ImInfo ("Resolution",message);

	ImInfo ("Compression", "Ascii");



	pPixel = ImVfbQFirst( vfb );

	pBufferStop = buffer + 68;	/* Flush soon after 68 chars	*/

	pBuffer = buffer;



	for ( y = 0; y < height; y++ )

	{

		for ( x = 0; x < width; x++ )

		{

			if ( pBuffer >= pBufferStop )

			{

				*pBuffer = '\n';

				IMWrite( ioType, fd, fp, buffer,pBuffer-buffer+1);

				pBuffer  = buffer;

			}

			sprintf( (char *)pBuffer, "%03d ",

				(ImVfbQIndex8( vfb, pPixel ) & 0xFF) );

			pBuffer += 4;

			ImVfbSInc( vfb, pPixel );

		}

		if ( pBuffer != buffer )

		{

			*pBuffer = '\n';

			IMWrite( ioType, fd, fp, buffer,pBuffer-buffer+1);

			pBuffer  = buffer;

		}

	}

	return ( 1 );

}



static int

#ifdef __STDC__

imPbmWriteRGB(ImFileFormatWriteMap * pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imPbmWriteRGB( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int     ioType;			/* I/O type flag		*/

	int     fd;			/* Unbuffered file descriptor	*/

	FILE   *fp;			/* Buffered file pointer	*/

	TagTable *flagsTable;		/* Max color value flag		*/

	TagTable *tagTable;		/* Tag list to read from	*/

#endif

{

	int       x, y;	/* Pixel location counters	*/

	ImVfbPtr  pPixel;	/* Current pixel		*/

	ImVfb    	  *vfb;		/* Image to output		*/

	int		   width;	/* Image width			*/

	int		   height;	/* Image height			*/

	unsigned char		   buffer[200];	/* Line buffer			*/

	unsigned char		  *pBuffer;	/* Current location in buffer	*/

	unsigned char		  *pBufferStop;	/* Where to stop & flush in buffer*/

	char		message[100];	/* ImInfo message		*/





	/*

	 *  Get the VFB and its attributes.  Write the PPM header.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	width  = ImVfbQWidth( vfb );

	height = ImVfbQHeight( vfb );

	sprintf( (char *)buffer, "%c%c\n%d %d\n255\n", IMPBM_MAGIC1, IMPPM_MAGIC2,

		width, height );

	IMWrite( ioType, fd, fp, buffer, strlen((char*) buffer ) );



	ImInfo ("Byte Order","Most Significant Byte First");

	ImInfo ("Type","24-bit RGB");

	sprintf (message, "%d x %d",width, height);

	ImInfo ("Resolution",message);

	ImInfo ("Compression", "Ascii");



	pPixel = ImVfbQFirst( vfb );

	pBufferStop = buffer + 65;	/* Flush soon after 65 chars	*/

	pBuffer = buffer;



	for ( y = 0; y < height; y++ )

	{

		for ( x = 0; x < width; x++ )

		{

			if ( pBuffer >= pBufferStop )

			{

				*pBuffer = '\n';

				IMWrite( ioType, fd, fp, buffer,pBuffer-buffer+1);

				pBuffer  = buffer;

			}

			sprintf( (char *)pBuffer, "%03d %03d %03d  ",

				(ImVfbQRed( vfb, pPixel ) & 0xFF),

				(ImVfbQGreen( vfb, pPixel ) & 0xFF),

				(ImVfbQBlue( vfb, pPixel ) & 0xFF) );

			pBuffer += 13;

			ImVfbSInc( vfb, pPixel );

		}

		if ( pBuffer != buffer )

		{

			*pBuffer = '\n';

			IMWrite( ioType, fd, fp, buffer,pBuffer-buffer+1);

			pBuffer  = buffer;

		}

	}

	return ( 1 );

}











/*

 *  FUNCTION

 *	imRpbmWrite1	-  write an 1-bit raw PBM file

 *	imRpbmWrite8	-  write an 8-bit raw PGM file

 *	imRpbmWriteRGB	-  write an RGB raw PPM file

 *

 *  DESCRIPTION

 *	1-bit:  The image is written out in binary as a series of 8-bit values,

 *	each one constructed from 8 1-bit mono values.

 *

 *	8-bit:  The image is written out in binary as a series of 8-bit index

 *	values.

 *

 *	RGB:  The image is written out in binary as a series of RGB values.

 */



static int

#ifdef __STDC__

imRpbmWrite1(ImFileFormatWriteMap * pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRpbmWrite1( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int     ioType;			/* I/O type flag		*/

	int     fd;			/* Unbuffered file descriptor	*/

	FILE   *fp;			/* Buffered file pointer	*/

	TagTable *flagsTable;		/* Max color value flag		*/

	TagTable *tagTable;		/* Tag list to read from	*/

#endif

{

	int       x, y;	/* Pixel location counters	*/

	ImVfbPtr  pPixel;	/* Current pixel		*/

	ImVfb    	  *vfb;		/* Image to output		*/

	int		   width;	/* Image width			*/

	int		   height;	/* Image height			*/

	unsigned char		   buf[200];	/* Temp buffer			*/

	unsigned char		  *buffer;	/* Line buffer			*/

	unsigned char		  *pBuffer;	/* Current location in buffer	*/

	int		   shift;	/* How far to shift		*/

	int		   byte;	/* Byte being built up		*/

	char		message[100];	/* ImInfo message		*/





	/*

	 *  Get the VFB and its attributes.  Write the PBM header.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	width  = ImVfbQWidth( vfb );

	height = ImVfbQHeight( vfb );

	sprintf( (char *)buf, "%c%c\n%d %d\n", IMPBM_MAGIC1, IMRPBM_MAGIC2,

		width, height );

	IMWrite( ioType, fd, fp, buf, strlen((char*) buf ) );



	ImInfo ("Byte Order","Most Significant Byte First");

	ImInfo ("Type","1-bit Monochrome");

	sprintf (message, "%d x %d",width, height);

	ImInfo ("Resolution",message);

	ImInfo ("Compression", "none");



	ImMalloc( buffer, unsigned char *, (width + 7) / 8 );

	pPixel  = ImVfbQFirst( vfb );

	for ( y = 0; y < height; y++ )

	{

		byte    = 0;

		shift   = 7;

		pBuffer = buffer;

		for ( x = 0; x < width; x++ )

		{

			byte |= (ImVfbQMono( vfb, pPixel ) & 0x1) << shift--;

			ImVfbSInc( vfb, pPixel );

			if ( shift == -1 )

			{

				*pBuffer++ = byte;

				shift = 7;

				byte  = 0;

			}

		}

		if ( shift != 7 )

			*pBuffer++ = byte;

		IMWrite( ioType, fd, fp, buffer, pBuffer - buffer );

	}



	free( (char *)buffer );

	return ( 1 );

}



static int

#ifdef __STDC__

imRpbmWrite8(ImFileFormatWriteMap * pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRpbmWrite8( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int     ioType;			/* I/O type flag		*/

	int     fd;			/* Unbuffered file descriptor	*/

	FILE   *fp;			/* Buffered file pointer	*/

	TagTable *flagsTable;		/* Max color value flag		*/

	TagTable *tagTable;		/* Tag list to read from	*/

#endif

{

	int       x, y;	/* Pixel location counters	*/

	ImVfbPtr  pPixel;	/* Current pixel		*/

	ImVfb    	  *vfb;		/* Image to output		*/

	int		   width;	/* Image width			*/

	int		   height;	/* Image height			*/

	unsigned char		   buf[200];	/* Temp buffer			*/

	unsigned char		  *buffer;	/* Line buffer			*/

	unsigned char		  *pBuffer;	/* Current location in buffer	*/

	char		message[100];	/* ImInfo message		*/





	/*

	 *  Get the VFB and its attributes.  Write the PBM header.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	width  = ImVfbQWidth( vfb );

	height = ImVfbQHeight( vfb );

	sprintf( (char *)buf, "%c%c\n%d %d\n255\n", IMPBM_MAGIC1, IMRPGM_MAGIC2,

		width, height );

	IMWrite( ioType, fd, fp, buf, strlen((char*) buf ) );



	ImInfo ("Byte Order","Most Significant Byte First");

	ImInfo ("Type","8-bit Grayscale");

	sprintf (message, "%d x %d",width, height);

	ImInfo ("Resolution",message);

	ImInfo ("Compression", "none");



	ImMalloc( buffer, unsigned char *, width );

	pPixel = ImVfbQFirst( vfb );

	for ( y = 0; y < height; y++ )

	{

		pBuffer = buffer;

		for ( x = 0; x < width; x++ )

		{

			*pBuffer++ = ImVfbQIndex8( vfb, pPixel );

			ImVfbSInc( vfb, pPixel );

		}

		IMWrite( ioType, fd, fp, buffer, pBuffer - buffer );

	}



	free( (char *)buffer );

	return ( 1 );

}



static int

#ifdef __STDC__

imRpbmWriteRGB(ImFileFormatWriteMap * pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRpbmWriteRGB( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int     ioType;			/* I/O type flag		*/

	int     fd;			/* Unbuffered file descriptor	*/

	FILE   *fp;			/* Buffered file pointer	*/

	TagTable *flagsTable;		/* Max color value flag		*/

	TagTable *tagTable;		/* Tag list to read from	*/

#endif

{

	int       x, y;	/* Pixel location counters	*/

	ImVfbPtr  pPixel;	/* Current pixel		*/

	ImVfb    	  *vfb;		/* Image to output		*/

	int		   width;	/* Image width			*/

	int		   height;	/* Image height			*/

	unsigned char		   buf[200];	/* Temp buffer			*/

	unsigned char		  *buffer;	/* Line buffer			*/

	unsigned char		  *pBuffer;	/* Current location in buffer	*/

	char		message[100];	/* ImInfo message		*/





	/*

	 *  Get the VFB and its attributes.  Write the PPM header.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	width  = ImVfbQWidth( vfb );

	height = ImVfbQHeight( vfb );

	sprintf( (char *)buf, "%c%c\n%d %d\n255\n", IMPBM_MAGIC1, IMRPPM_MAGIC2,

		width, height );

	IMWrite( ioType, fd, fp, buf, strlen((char*) buf ) );



	ImInfo ("Byte Order","Most Significant Byte First");

	ImInfo ("Type","24-bit RGB");

	sprintf (message, "%d x %d",width, height);

	ImInfo ("Resolution",message);

	ImInfo ("Compression", "none");



	ImMalloc( buffer, unsigned char *, width * 3 );

	pPixel = ImVfbQFirst( vfb );

	for ( y = 0; y < height; y++ )

	{

		pBuffer = buffer;

		for ( x = 0; x < width; x++ )

		{

			*pBuffer++ = ImVfbQRed( vfb, pPixel );

			*pBuffer++ = ImVfbQGreen( vfb, pPixel );

			*pBuffer++ = ImVfbQBlue( vfb, pPixel );

			ImVfbSInc( vfb, pPixel );

		}

		IMWrite( ioType, fd, fp, buffer, pBuffer - buffer );

	}



	free( (char *)buffer );

	return ( 1 );

}

