/**

 **	$Header: /roq/libim/imxbm.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imxbm.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imxbm.c		-  X11 bitmap file I/O

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imxbm.c contains routines to read and write Sun Icon files for

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

 **	none

 **

 **  PRIVATE CONTENTS

 **

 **	imXbmRead		f  read a X11 Bitmap file

 **	imXbmWrite		f  write a X11 Bitmap file

 **

 **

 **  HISTORY

 **	$Log: /roq/libim/imxbm.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.15  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.14  1995/05/16  22:12:25  bduggan

 **	made HotSpot static (since its address is used in the tagtable)

 **

 **	Revision 1.13  1995/04/03  21:41:29  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.12  1995/01/10  23:50:58  bduggan

 **	made read/write routines static

 **	put in IMMULTI, IMPIPE instead of TRUE/FALSE

 **

 **	Revision 1.11  94/10/03  11:31:13  nadeau

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

 **	Revision 1.10  92/11/23  18:43:20  nadeau

 **	Removed use of IMINFOMSG.

 **	

 **	Revision 1.9  92/11/04  12:10:54  groening

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.8  92/09/29  17:56:55  vle

 **	Added ImInfo messages.

 **	

 **	Revision 1.7  92/09/17  13:58:22  vle

 **	Moved a misplaced line of code.

 **	

 **	Revision 1.6  92/09/01  20:16:05  vle

 **	Updated copyright notice.  Added code to read/write hotspots, instead

 **	of just skipping them.

 **	

 **	Revision 1.5  91/10/04  08:52:26  nadeau

 **	Fixed strcpy typo.

 **	

 **	Revision 1.4  91/10/03  09:25:08  nadeau

 **	Minor comment updating.

 **	

 **	Revision 1.3  91/02/20  12:44:37  nadeau

 **	Added hotspot read handling.

 **	

 **	Revision 1.2  91/02/12  10:48:15  nadeau

 **	Removed the tag table checking and VFB conversion now

 **	handled by ImFileRead and ImFileWrite.  Changed grayscale

 **	to mono support.

 **	

 **	Revision 1.1  91/01/30  18:18:31  nadeau

 **	Initial revision

 **/



#include <ctype.h>

#include "iminternal.h"





/**

 **  FORMAT

 **	xbm	-  X11 Bitmap

 **

 **  AKA

 **	bm

 **

 **  FORMAT REFERENCES

 **	Graphics File Formats, David C. Kay, John R. Levine

 **

 **

 **  CODE CREDITS

 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1991.

 **

 **  DESCRIPTION

 **	X11 bitmaps are simple C code to declare and initialize a character

 **	array with bits for an X11 cursor, icon, or glyph of some other sort.

 **	Preceding the array declaration are a few #define-ed constants to

 **	give the image width and height, and optionally a cursor's hot spot

 **	location.

 **

 **		#define name_width  xxx

 **		#define name_height yyy

 **		#define name_x_hot  xhot

 **		#define name_y_hot  yhot

 **		static char name_bits[] = {

 **			... ASCII hex bytes ...

 **		};

 **

 **	xxx is the image width, in pixels.

 **

 **	yyy is the image height, in pixels.

 **

 **	xhot and yhot are the hot spot location.  These two lines are

 **	optional.  imxbm.c ignores the hot spot.

 **

 **	name is the name of the glyph.  imxbm.c write code generates the

 **	name based upon the outgoing file name given in the flags table.

 **/



/*

 *  XBM - MIT X11 Window System BitMap

 *      For information on these structures, how to use them, etc. please

 *      see imfmt.c.

 */

#ifdef __STDC__

static int imXbmRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imXbmWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 

	TagTable *tagTable );

#else

static int imXbmRead( );

static int imXbmWrite( );

#endif



static char *imXbmNames[ ]  = { "xbm", "bm", NULL };

static ImFileFormatReadMap imXbmReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IN,1,1,       0,      IMVFBMONO,      0 },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imXbmWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBMONO,    0,      IN,1,1,         0,      imXbmWrite },

        { -1,           0,      -1,             0,      NULL },

};



static ImFileMagic imFileXbmMagic []=

{

	{ 0, 0, NULL},

};



ImFileFormat ImFileXbmFormat =

{

	imXbmNames, "X11 bitmap file",

	"X Consortium / MIT",

	"1-bit ASCII bitmap files.",

	"1-bit ASCII bitmap files.",

	imFileXbmMagic,

	IMNOMULTI, IMPIPE,

	IMNOMULTI, IMPIPE,

	imXbmRead, imXbmReadMap, imXbmWriteMap

};









/*

 *  FUNCTION

 *	imXbmRead	-  read an X11 bitmap file

 *

 *  DESCRIPTION

 *	The file is read and it's mono image written to a new mono VFB.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__

imXbmRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imXbmRead( ioType, fd, fp, flagsTable, tagTable )

	int         ioType;		/* I/O flags			*/

	int         fd;			/* Input file descriptor	*/

	FILE	   *fp;			/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable   *tagTable;		/* Tag table to add to		*/

#endif

{

	int            xdim;		/* # columns			*/

	int            ydim;		/* # rows			*/

	int	       xdim2;		/* # columns (nearest 8bit bndry)*/

	int	       xdimExtra;	/* # columns mod 8		*/

	int            xhot = 0;	/* hotspot x coordinate		*/

	int            yhot = 0;	/* hotspot y coordinate		*/

	int	       hotspotflag = FALSE;	/* TRUE if there's a

						   hotspot		*/

	static ImHotSpot      hotspot;	/* Cursor hotspot		*/

	ImHotSpotPtr   hotspotptr;	/* Cursor hotspot pointer	*/

	ImVfb         *vfb;		/* Read in image		*/

	ImVfbPtr       pPixel;		/* Pixel pointer		*/

	int            c;		/* Character holder		*/

	int	       pixel;		/* 8-bits of pixels		*/

	int	       i, j, k;		/* Counters			*/

	char           message[1024];	/* Error message holder		*/





	/*

	 *  Read in the header.

	 */

	if ( ioType & IMFILEIOFD )

	{

		/*

		 *  Given file descriptor.  More convenient to deal with

		 *  a file pointer for this format.

		 */

		if ( (fp = fdopen( fd, "r" )) == NULL )

		{

			ImErrNo = IMESYS;

			return ( -1 );

		}

	}



	/* Read in width:  #define name_width xdim			*/

	if ( fscanf( fp, "%*s %*s %d", &xdim ) != 1 )

	{

		ImErrorFatal( "Syntax error in giving image width", -1, IMESYNTAX );

	}

	for ( c = fgetc( fp ); c != EOF && c != '\n'; c = fgetc( fp ) )

		;

	if ( c == EOF )

	{

		ImErrorFatal( "Syntax error in giving image width",

			-1, IMESYNTAX );

	}



	/* Read in height:  #define name_height ydim			*/

	if ( fscanf( fp, "%*s %*s %d", &ydim ) != 1 )

	{

		ImErrorFatal( "Syntax error in giving image height", -1, IMESYNTAX );

	}

	for ( c = fgetc( fp ); c != EOF && c != '\n'; c = fgetc( fp ) )

		;

	if ( c == EOF )

	{

		ImErrorFatal( "Syntax error in giving image height",

			-1, IMESYNTAX );

	}



	sprintf( message, "%d x %d", xdim, ydim );

	ImInfo( "Resolution", message );

	ImInfo( "Type", "1-bit Monochrome" );



	/* Read in x_hot (optional):  #define name_x_hot xhot		*/

	if ( (c = fgetc( fp )) == '#' )

	{

		if ( fscanf( fp, "%*s %*s %d", &xhot ) != 1 )

		{

			ImErrorFatal( "Syntax error in giving image hotspot x", -1,

				IMESYNTAX );

		}

		for ( c = fgetc( fp ); c != EOF && c != '\n'; c = fgetc( fp ) )

			;

		if ( c == EOF )

		{

			ImErrorFatal( "Syntax error in giving image hotspot x",

				-1, IMESYNTAX );

		}

		hotspotflag = TRUE;

	}

	

	/* Read in y_hot (optional):  #define name_y_hot xhot		*/

	if ( (c = fgetc( fp )) == '#' )

	{

		if ( fscanf( fp, "%*s %*s %d", &yhot ) != 1 )

		{

			ImErrorFatal( "Syntax error in giving image hotspot y", -1,

				IMESYNTAX );

		}

		for ( c = fgetc( fp ); c != EOF && c != '\n'; c = fgetc( fp ) )

			;

		if ( c == EOF )

		{

			ImErrorFatal( "Syntax error in giving image hotspot y",

				-1, IMESYNTAX );

		}

		hotspotflag = TRUE;

	}



	/*

	 * Store hotspot in TagTable, if there's at least one coordinate in the file

	 */

	if( hotspotflag == TRUE )

	{

		hotspotptr = &hotspot;

		ImHotSpotSX( hotspotptr, xhot );

		ImHotSpotSY( hotspotptr, yhot );

		TagTableAppend( tagTable, TagEntryAlloc( "image hot spot",

			POINTER, &hotspotptr ) );



		sprintf( message, "%d, %d", xhot, yhot );

		ImInfo( "Hot Spot", message );

	}

	else

	{

		ImInfo( "Hot Spot", "none" );

	}

	

	/*

	 *  Skip:

	 *	static char name_bits[] = {

	 */

	i = 1;

	while ( i-- )

	{

		for ( c = fgetc( fp ); c != EOF && c != '\n'; c = fgetc( fp ) )

			;

		if ( c == EOF )

		{

			if ( i == 0 )

				ImErrorFatal( "Syntax error in bits array declaration",

					-1, IMESYNTAX );

			ImErrorFatal( "Syntax error in giving hot spot location",

				-1, IMESYNTAX );

		}

	}





	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (vfb = ImVfbAlloc( xdim, ydim, IMVFBMONO )) == IMVFBNULL )

		ImErrorFatal( ImQError( ), -1, ImErrNo );





	/*

	 *  Scan the file, reading in each hex character value holding

	 *  8 bits.  Unpack the bits into gray-scale values (0 = black,

	 *  255 = white).

	 */

	xdimExtra = xdim & 0x7;

	xdim2     = xdim - xdimExtra;





	pPixel = ImVfbQFirst( vfb );

	for ( i = 0; i < ydim; i++ )

	{

		for ( j = 0; j < xdim2; j += 8 )

		{

			/* Skip up to the 'x' in hex number.		*/

			for ( c = fgetc( fp ); c != EOF && c != 'x'; )

				c = fgetc( fp );

			if ( c == EOF )

			{

				sprintf( message,

					"Unexpected EOF in bits array element %d",

					i * (xdim2/8 + (xdimExtra+7)/8) + j/8 );

				ImErrorFatal( message, -1, IMESYNTAX );

			}



			/* Read in the pixel value.			*/

			if ( fscanf( fp, "%x", &pixel ) != 1 )

			{

				sprintf( message,

					"Syntax error in bits array element %d",

					i * (xdim2/8 + (xdimExtra+7)/8) + j/8 );

				ImErrorFatal( message, -1, IMESYNTAX );

			}

			for( k = 0; k < 8; k++ )

			{

				ImVfbSMono( vfb, pPixel, (pixel&1) );

				ImVfbSInc( vfb, pPixel );

				pixel >>= 1;

			}

		}

		if ( xdimExtra == 0 )

			continue;



		/* Skip up to the 'x' in hex number.			*/

		for ( c = fgetc( fp ); c != EOF && c != 'x'; )

			c = fgetc( fp );

		if ( c == EOF )

		{

			sprintf( message,

				"Unexpected EOF in bits array element %d",

				i * (xdim2/8 + (xdimExtra+7)/8) + j/8 );

			ImErrorFatal( message, -1, IMESYNTAX );

		}

		if ( fscanf( fp, "%x", &pixel ) != 1 )

		{

			sprintf( message,

				"Syntax error in bits array element %d",

				i * (xdim2/8 + (xdimExtra+7)/8) + xdim2 );

			ImErrorFatal( message, -1, IMESYNTAX );

		}

		for( k = 0; k < xdimExtra; k++ )

		{

			ImVfbSMono( vfb, pPixel, (pixel&1) );

			ImVfbSInc( vfb, pPixel );

			pixel >>= 1;

		}

	}





	/*

	 *  Add the VFB to the tagTable.

	 */

	TagTableAppend( tagTable, TagEntryAlloc( "image vfb", POINTER, &vfb ) );



	return ( 1 );

}











/*

 *  FUNCTION

 *	imXbmWrite	-  write a X11 bitmap file

 *

 *  DESCRIPTION

 *	The X11 bitmap header and image content are written out.

 */



static int				/* Returns # of entries used	*/

#ifdef __STDC__

imXbmWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imXbmWrite( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int            ioType;		/* Type of I/O to perform	*/

	int            fd;		/* Output file descriptor	*/

	FILE          *fp;		/* Output file pointer		*/

	TagTable      *flagsTable;	/* Flags			*/

	TagTable      *tagTable;	/* Table of info to write	*/

#endif

{

	int   xdim;		/* # columns			*/

	int	       xdim2;		/* # columns (nearest 8bit bndry)*/

	int	       xdimExtra;	/* # columns mod 8		*/

	int   ydim;		/* # rows			*/

	ImHotSpotPtr   hotspotptr;	/* hotspot pointer		*/

	int	       hotspotflag = FALSE;	/* TRUE, if there's a

						   hotspot		*/

	int            xhot;		/* hotspot x coordinate		*/

	int            yhot;		/* hotspot y coordinate		*/

	ImVfb          *srcVfb;		/* VFB to convert		*/

	ImVfbPtr       pPixel;		/* gray pixel pointer		*/

	char           buf[1024];	/* Output buffer		*/

	int            pixel;		/* Gray pixel color		*/

	int	       i, j, k;		/* Counters			*/

	int	       n;		/* # of tag table entries	*/

	char	       name[1024];	/* Bitmap name			*/

	char	      *s;		/* String pointer		*/

	char		 message[100];	/* ImInfo message		*/





	/*

	 *  Use the output file's name, sans leading path and dot

	 *  extensions, to make up the root name of the #define's

	 *  and bits array declaration.

	 */

	s = &ImErrorFileName[strlen(ImErrorFileName)-1];

	while ( s != (ImErrorFileName-1) && *s != '/' )

		s--;

	++s;

	if ( !isalpha( *s ) )

	{

		/* Make it a legal variable name by adding an 'x'.*/

		name[0] = 'x';

		name[1] = '\0';

		strcat( name, s );

	}

	else

		strcpy( name, s );

	s = name;

	while ( isalpha( *s ) || isdigit( *s ) || *s == '_' )

		s++;

	*s = '\0';





	/*

	 *  Get hotspot info if available

	 */

	if( TagEntryQValue( TagTableQDirect( tagTable, "image hot spot", 0 ),

		&hotspotptr ) != -1 )

	{

		xhot = ImHotSpotQX( hotspotptr );

		yhot = ImHotSpotQY( hotspotptr );

		hotspotflag = TRUE;

	}





	/*

	 *  Write out the header.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &srcVfb );

	xdim = ImVfbQWidth( srcVfb );

	ydim = ImVfbQHeight( srcVfb );

	if ( ioType & IMFILEIOFD )

	{

		sprintf( buf, "#define %s_width %d\n", name, xdim );

		write( fd, buf, strlen( buf ) );

		sprintf( buf, "#define %s_height %d\n", name, ydim );

		write( fd, buf, strlen( buf ) );

		if( hotspotflag == TRUE )

		{

			sprintf( buf, "#define %s_x_hot %d\n", name, xhot );

			write( fd, buf, strlen( buf ) );

			sprintf( buf, "#define %s_y_hot %d\n", name, yhot );

			write( fd, buf, strlen( buf ) );

		}

		sprintf( buf, "static char %s_bits[] = {\n", name );

		write( fd, buf, strlen( buf ) );

	}

	else

	{

		fprintf( fp, "#define %s_width %d\n", name, xdim );

		fprintf( fp, "#define %s_height %d\n", name, ydim );

		if( hotspotflag == TRUE )

		{

			fprintf( fp, "#define %s_x_hot %d\n", name, xhot );

			fprintf( fp, "#define %s_y_hot %d\n", name, yhot );

		}

		fprintf( fp, "static char %s_bits[] = {\n", name );

	}

	xdimExtra = xdim & 0x7;

	xdim2 = xdim - xdimExtra;



	/*

	 * Output -verbose message

	 */

	sprintf( message, "%d x %d", xdim, ydim );

	ImInfo( "Resolution", message );

	ImInfo( "Type", "1-bit Monochrome" );



	if( hotspotflag == TRUE )

	{

		sprintf( message, "%d, %d", xhot, yhot );

		ImInfo( "Hot Spot", message );

	}

	else

	{

		ImInfo( "Hot Spot", "none" );

	}



	/*

	 *  Write out the image.

	 */

	pPixel = ImVfbQFirst( srcVfb );

	for ( i = 0; i < ydim; i++ )

	{

		for ( j = 0; j < xdim2; j += 8 )

		{

			pixel = 0;

			for ( k = 0; k < 8; k++ )

			{

				if ( ImVfbQMono( srcVfb, pPixel ) > 0 )

					pixel |= (1<<k);

				ImVfbSInc( srcVfb, pPixel );

			}

			if ( ioType & IMFILEIOFD )

			{

				sprintf( buf, "0x%02x, ", pixel );

				write( fd, buf, strlen( buf ) );

			}

			else

				fprintf( fp, "0x%02x, ", pixel );

		}

		if ( xdimExtra == 0 )

		{

			if ( ioType & IMFILEIOFD )

			{

				write( fd, "\n", 1 );

			}

			else

				fprintf( fp, "\n" );

			continue;

		}

		pixel = 0;

		for ( k = 0; k < xdimExtra; k++ )

		{

			if ( ImVfbQMono( srcVfb, pPixel ) > 0 )

				pixel |= (1<<k);

			ImVfbSInc( srcVfb, pPixel );

		}

		if ( ioType & IMFILEIOFD )

		{

			sprintf( buf, "0x%02x,\n", pixel );

			write( fd, buf, strlen( buf ) );

		}

		else

			fprintf( fp, "0x%02x,\n", pixel );

	}

	if ( ioType & IMFILEIOFD )

	{

		sprintf( buf, "};\n" );

		write( fd, buf, strlen( buf ) );

	}

	else

		fprintf( fp, "};\n" );



	return ( 1 );			/* Used 1 tagTable entry:  VFB	*/

}

