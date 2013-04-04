/**

 **	$Header: /roq/libim/imicon.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imicon.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imicon.c	-  Sun icon/cursor format I/O

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imicon.c contains routines to read and write Sun Icon files for

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

 **	imIconRead		f  read a Sun Icon file

 **	imIconWrite		f  write a Sun Icon file

 **

 **  HISTORY

 **	$Log: /roq/libim/imicon.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.10  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.9  1995/06/15  20:27:45  bduggan

 **	removed unused var.s

 **

 **	Revision 1.8  1995/04/03  21:26:45  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.7  1995/01/10  23:31:08  bduggan

 **	uncapitlized i's in local functions

 **	made read/write routines static

 **

 **	Revision 1.6  94/10/03  11:30:15  nadeau

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

 **	Revision 1.5  92/11/23  18:42:34  nadeau

 **	Removed use of IMINFOMSG.

 **	

 **	Revision 1.4  92/11/04  12:01:29  groening

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.3  92/09/29  17:58:16  vle

 **	Added ImInfo messages.

 **	

 **	Revision 1.2  92/09/01  20:15:51  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.1  91/10/03  09:05:46  nadeau

 **	Initial revision

 **	

 **/



#include <ctype.h>

#include "iminternal.h"





/**

 **  FORMAT

 **	icon	-  Sun Icon/Cursor/Image

 **

 **  AKA

 **  	icon, cursor, pr

 **

 **  FORMAT REFERENCES

 **	- iconedit(1) from the Sun OpenWindows man page set.

 **	- Pixrect Reference Manual, Sun Microsystems.

 **

 **  CODE CREDITS

 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1992.

 **

 **  DESCRIPTION

 **	Sun icon files are used to describe icons, cursors, widgets, and

 **	full black-and-white images on Sun workstations.  Originally used

 **	for the Sunview window system, the Sun icon format is now used by

 **	OpenWindows and XView.

 **

 **	Sun icon files are simple C code including a comment header, and

 **	a string of comma-separated hex values.

 **

 **	The comment header has the format:

 **

 **

 ** Format_version=1, Width=XX, Height=YY, Depth=1, Valid_bits_per_item=16

 **

 **

 **	The Format_version is always 1.

 **

 **	The Width and Height fields give the width (XX) and height (YY) of

 **	the icon in pixels.  The width **MUST** be a multiple of 16!  When

 **	writing images appropriate black padding is added to the right side.

 **	( Note:  The PBM suite chose to pad by putting equal padding on the

 **	left and right side.  We choose to pad by adding only on the right.

 **	This seems more intuitive and easier to compensate for during use. )

 **

 **	The Depth is always 1.

 **

 **	The Valid_bits_per_item is always 16.

 **

 **	The comment header may be preceded by blank lines and other comments,

 **	and may be followed by additional comments and white space.  Most of

 **	the standard icons found in /usr/include/images on Sun systems

 **	have headers of the form:

 **

 **

 ** Format_version=1, Width=64, Height=64, Depth=1, Valid_bits_per_item=16

 ** Description: Tombstone with letters R.I.P

 ** Background: White

 **

 **	Following the file header is a list of hex values for the icon.

 **	Any number of values may occur per line, with any amount of white

 **	space, and any number of lines.  Each hex value is of the form:

 **	"0x0000" (without the quotes).  Hex values are separated by a comma.

 **

 **	The number of hex values may be computed from the width and height:

 **

 **		nvalues = Width ** Height / Valid_bits_per_item

 **

 **	0 bits are black.  1 bits are white.  Bits are ordered with

 **	the left-most pixel bit the left-most (highest) in each 16-bit value.

 **/



/*

 *  ICON - Sun Icon and Cursor bitmap

 *      For information on these structures, how to use them, etc. please

 *      see imfmt.c.

 */

#ifdef __STDC__

static int imIconRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imIconWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

#else

static int imIconRead( );

static int imIconWrite();

#endif



static char *imIconNames[ ]  = { "icon", "cursor", "pr", NULL };

static ImFileFormatReadMap imIconReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IN,1,1,       0,      IMVFBMONO,      0 },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imIconWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBMONO,    0,      IN,1,1,         0,      imIconWrite },

        { -1,           0,      -1,             0,      NULL },

};



static ImFileMagic imFileIconMagic []=

{

	{ 0, 0, NULL},

};



ImFileFormat ImFileIconFormat =

{

	imIconNames, "Sun Icon and Cursor file",

	"Sun Microsystems, Inc.",

	"1-bit ASCII bitmap files.",

	"1-bit ASCII bitmap files.",

	imFileIconMagic,

	IMNOMULTI, IMPIPE,

	IMNOMULTI, IMPIPE,

	imIconRead, imIconReadMap, imIconWriteMap

};







/*

 *  FUNCTION

 *	imIconRead	-  read an Sun Icon file

 *

 *  DESCRIPTION

 *	The file is read and it's mono image written to a new mono VFB.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__

imIconRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imIconRead( ioType, fd, fp, flagsTable, tagTable )

	int         ioType;		/* I/O flags			*/

	int         fd;			/* Input file descriptor	*/

	FILE	   *fp;			/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable   *tagTable;		/* Tag table to add to		*/

#endif

{

	int            xdim;		/* # columns			*/

	int            ydim;		/* # rows			*/

	ImVfb         *vfb;		/* Read in image		*/

	ImVfbPtr       pPixel;		/* Pixel pointer		*/

	ImVfbPtr       pLast;		/* Last pixel pointer		*/

	int	       pixel;		/* 16-bits of pixels		*/

	int	       i, j, k;		/* Counters			*/



	int            c;		/* Character holder		*/

	char	       token[1024];	/* Input token			*/

	char	      *t;		/* Token pointer		*/

	int	       value;		/* Token value			*/

	int	       found;		/* Header information found?	*/

	char	       message[100];	/* ImInfo message		*/





	if ( (ioType & IMFILEIOFD) && (fp = fdopen( fd, "rb" )) == NULL )

	{

		ImErrNo = IMESYS;

		return ( -1 );

	}





	for ( xdim = ydim = -1, found = FALSE; !found; )

	{

		/* Skip to the start of a C comment.			*/

		while ( (c = getc( fp )) != '/' && c != EOF )

			;

		if ( c == EOF )

			ImErrorFatal( "Premature EOF in icon file header", -1, IMESYNTAX );

		if ( (c = getc( fp )) != '*' )

			continue;

		c = getc( fp );



		for ( ; ; )

		{

			/* Skip whitespace.				*/

			for ( ; isspace( c ) ||  c == ','; c = getc( fp ) )

				;

			if ( c == EOF )

				ImErrorFatal( "Premature EOF in icon file header", -1, IMESYNTAX );



			if ( c == '*' )

			{

				if ( (c = getc( fp )) == '/' )

					break;	/* Done with comment	*/

				continue;

			}



			/* Get the next token.				*/

			t = token;

			for ( *t++ = c; (c = getc( fp )) != EOF &&

				c != ',' && c != '=' && c != '*' && !isspace(c);

				*t++ = c )

				;

			if ( c == EOF )

				ImErrorFatal( "Premature EOF in icon file header", -1, IMESYNTAX );

			*t = '\0';



			if ( c == '*' )

			{

				if ( (c = getc( fp )) == '/' )

					break;	/* Done with comment	*/

				continue;

			}



			if ( c != '=' )

			{

				/* Not a token we want.  Skip to whitespace*/

				for ( ; !isspace( c ); c = getc( fp ) )

					;

				continue;

			}



			/* Get the value following the '='.		*/

			if ( fscanf( fp, "%d", &value ) != 1 )

			{

				/* No value.  Not a token we want.	*/

				for ( c = getc( fp ); !isspace( c ); c = getc( fp ) )

					;

				continue;

			}

			c = getc( fp );



			/* Check which token it is.			*/

			if ( strcmp( token, "Format_version" ) == 0 )

			{

				if ( value != 1 )

					ImErrorFatal( "Only format version 1 Sun icon files are supported", -1, IMESYNTAX );

				found = TRUE;



				ImInfo( "Version", "1" );

				continue;

			}

			if ( strcmp( token, "Width" ) == 0 )

			{

				if ( value <= 0 )

					ImErrorFatal( "Sun icon width must be positive", -1, IMESYNTAX );

				if ( (value & 0xF) != 0 )

					ImErrorFatal( "Sun icon width must be multiple of 16 pixels", -1, IMESYNTAX );

				xdim = value;

				found = TRUE;

				continue;

			}

			if ( strcmp( token, "Height" ) == 0 )

			{

				if ( value <= 0 )

					ImErrorFatal( "Sun icon height must be positive", -1, IMESYNTAX );

				ydim = value;

				found = TRUE;

				continue;

			}

			if ( strcmp( token, "Depth" ) == 0 )

			{

				if ( value != 1 )

					ImErrorFatal( "Only Depth=1 Sun icon files are supported", -1, IMESYNTAX );

				found = TRUE;

				continue;

			}

			if ( strcmp( token, "Valid_bits_per_item" ) == 0 )

			{

				if ( value != 16 )

					ImErrorFatal( "Only Valid_bits_per_item=16 Sun icon files are supported", -1, IMESYNTAX );

				found = TRUE;

				continue;

			}



			/* Unknown token.  Skip to whitespace.		*/

			for ( ; !isspace( c ); c = getc( fp ) )

				;

		}

	}



	if ( xdim == -1 )

		ImErrorFatal( "Missing width for Sun icon", -1, IMESYNTAX );

	if ( ydim == -1 )

		ImErrorFatal( "Missing height for Sun icon", -1, IMESYNTAX );



	sprintf( message, "%d x %d", xdim, ydim );

	ImInfo( "Resolution", message );



	ImInfo( "Type", "1-bit Monochrome" );



	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (vfb = ImVfbAlloc( xdim, ydim, IMVFBMONO )) == IMVFBNULL )

		ImErrorFatal( ImQError( ), -1, ImErrNo );





	/*

	 *  Scan the file, reading in each hex character value holding

	 *  16 bits.  Unpack the bits into mono values (0 = black,

	 *  1 = white).

	 */

	pPixel = ImVfbQFirst( vfb );

	for ( i = 0; i < ydim; i++ )

	{

		/*

		 *  Handle a scanline, modulo 16.

		 */

		for ( j = 0; j < xdim; j += 16 )

		{

			/*

			 *  Skip to the next hex number, read it in, and

			 *  break it down into mono single-bit pixels.

			 */

			for ( c = getc( fp ); c != 'x' && c != EOF ; )

				c = getc( fp );

			if ( c == EOF )

				break;



			if ( fscanf( fp, "%x", &pixel ) != 1 )

				break;

			for ( k = 15; k >= 0; k-- )

			{

				ImVfbSMono( vfb, pPixel, ((pixel>>k)&1) );

				ImVfbSInc( vfb, pPixel );

			}

		}

		if ( c == EOF )

			break;

	}



	for ( pLast=ImVfbQLast( vfb ); pPixel <= pLast; ImVfbSInc( vfb, pPixel))

		ImVfbSMono( vfb, pPixel, 0 );





	/*

	 *  Add the VFB to the tagTable.

	 */

	TagTableAppend( tagTable, TagEntryAlloc( "image vfb", POINTER, &vfb ) );



	return ( 1 );

}











/*

 *  FUNCTION

 *	imIconWrite	-  write a Sun Icon file

 *

 *  DESCRIPTION

 *	The Sun Icon header and image content are written out.

 */



static int				/* Returns # of entries used	*/

#ifdef __STDC__

imIconWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imIconWrite( pMap, ioType, fd, fp, flagsTable, tagTable )

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

	ImVfb          *srcVfb;		/* VFB to convert		*/

	ImVfbPtr       pPixel;		/* pixel pointer		*/

	char           buf[1024];	/* Output buffer		*/

	int            pixel;		/* pixel value			*/

	int	       j, k;		/* Counters			*/

	int	       nchars;		/* Number of characters output	*/

	char		 message[100];	/* ImInfo message		*/





	/*

	 *  Write out the header.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &srcVfb );

	xdim = ImVfbQWidth( srcVfb );

	ydim = ImVfbQHeight( srcVfb );



	xdimExtra = xdim & 0xF;			/* Bits in last 2 bytes	*/

	xdim2     = xdim - xdimExtra;		/* Bits before last 2 bytes */

	j = ((xdim + 15) / 16) * 16;		/* Bits rounded up to next 16*/



	sprintf( buf, "/* Format_version=1, Width=%d, Height=%d, Depth=1, Valid_bits_per_item=16\n */\n", j, ydim );

	if ( ioType & IMFILEIOFD )

		write( fd, buf, strlen( buf ) );

	else

		fprintf( fp, buf );



	/*

	 * Output -verbose message

	 */

	ImInfo( "Version", "1" );



	sprintf( message, "%d x %d", j, ydim );

	ImInfo( "Resolution", message );



	ImInfo( "Type", "1-bit Monochrome" );



	/*

	 *  Write out the image.

	 */

	pPixel = ImVfbQFirst( srcVfb );

	for ( nchars = 0; ydim--; )

	{

		for ( j = 0; j < xdim2; j += 16 )

		{

			pixel = 0;

			for ( k = 15; k >= 0; k-- )

			{

				if ( ImVfbQMono( srcVfb, pPixel ) > 0 )

					pixel |= (1<<k);

				ImVfbSInc( srcVfb, pPixel );

			}

			sprintf( buf, "0x%04x,", pixel );

			if ( (nchars += 7) > 72 )

			{

				strcat( buf, "\n" );

				nchars = 0;

			}

			if ( ioType & IMFILEIOFD )

				write( fd, buf, strlen( buf ) );

			else

				fprintf( fp, buf );

		}

		if ( xdimExtra == 0 )

			continue;



		pixel = 0;

		for ( k = xdimExtra - 1; k >= 0; k-- )

		{

			if ( ImVfbQMono( srcVfb, pPixel ) > 0 )

				pixel |= (1<<k);

			ImVfbSInc( srcVfb, pPixel );

		}

		sprintf( buf, "0x%04x,", pixel );

		if ( (nchars += 7) > 72 )

		{

			strcat( buf, "\n" );

			nchars = 0;

		}

		if ( ioType & IMFILEIOFD )

			write( fd, buf, strlen( buf ) );

		else

			fprintf( fp, buf );

	}

	if ( ioType & IMFILEIOFD )

		write( fd, "\n", 2 );

	else

		fprintf( fp, "\n" );



	return ( 1 );			/* Used 1 tagTable entry:  VFB	*/

}

