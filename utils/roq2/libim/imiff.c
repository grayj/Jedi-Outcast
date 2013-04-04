/**

 **	$Header: /roq/libim/imiff.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER        "    $Header: /roq/libim/imiff.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imiff.c		-  Sun TAAC file format I/O

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imiff.c contains routines to read and write Sun TAAC IFF files for

 **	the image manipulation library.  Raster data read in is stored

 **	in a VFB and optional CLT in a tag list.  Raster data written

 **	out is taken from a tag list.

 **

 **  PUBLIC CONTENTS

 ** 			d =defined constant

 **                     f =function

 **                     m =defined macro

 **                     t =typedef/struct/union

 **                     v =variable

 **                     ? =other

 **

 **	none

 **

 **  PRIVATE CONTENTS

 **     imIffRead               f  read a Sun-TAAC image file

 **     imIffWrite              f  write a Sun-TAAC image file

 **

 **     imIffHeaderInfo	        t IFF file header information

 **	imGetFDStr		f Read a stream of bytes

 **

 **  HISTORY

 **	$Log: /roq/libim/imiff.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.24  1995/06/30  22:16:18  bduggan

 **	changed index to strchr

 **

 **	Revision 1.23  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.22  1995/06/15  20:30:31  bduggan

 **	changed "dont" to "don't".  Added cast for strncmp.  Removed prototype for strtok.

 **

 **	Revision 1.21  1995/04/03  21:27:18  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.20  1995/02/16  21:40:27  bduggan

 **	Made write function static

 **

 **	Revision 1.19  1995/01/16  22:09:45  bduggan

 **	Added prototype for index()

 **

 **	Revision 1.18  1995/01/10  23:31:31  bduggan

 **	uncapitlized i's in local functions

 **	put in IMMULTI, IMPIPE instead of TRUE/FALSE

 **

 **	Revision 1.18  1995/01/10  23:31:31  bduggan

 **	uncapitlized i's in local functions

 **	put in IMMULTI, IMPIPE instead of TRUE/FALSE

 **

 **	Revision 1.17  94/10/03  11:30:16  nadeau

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

 **	Revision 1.16  92/12/03  01:49:45  nadeau

 **	Corrected info messages.

 **	

 **	Revision 1.15  92/11/23  18:42:36  nadeau

 **	Removed use of IMINFOMSG.

 **	

 **	Revision 1.14  92/11/04  11:58:35  groening

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.13  92/10/19  14:14:10  groening

 **	added ImInfo statements

 **	

 **	Revision 1.12  92/09/02  15:58:04  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.11  92/09/02  15:48:31  todd

 **	modify to handle sun VX/MVX vff files.  minor changes.

 **	

 **	Revision 1.10  92/04/09  09:31:00  groening

 **	To make the compiler happy added extern statements.  

 **	

 **	Revision 1.9  91/10/04  10:50:21  nadeau

 **	Fixed shift problem for VAX.

 **	

 **	Revision 1.8  91/10/03  09:06:07  nadeau

 **	Fixed #includes.

 **	

 **	Revision 1.7  91/02/12  11:33:09  nadeau

 **	Removed the tag table checking now handled by

 **	ImFileRead and ImFileWrite.

 **	

 **	Revision 1.6  91/01/29  11:09:03  todd

 **	minor changes

 **	

 **	Revision 1.5  90/09/18  08:18:08  todd

 **	Added 8 bit image with colormap functionality.

 **	cleaned up header reading code.

 **	

 **	Revision 1.4  90/07/02  13:20:03  nadeau

 **	Updated to the new error handling mechanism.

 **	

 **	Revision 1.3  90/06/25  14:46:36  nadeau

 **	Changed ImTag* to Tag* (new names).

 **

 **	Revision 1.2  90/06/25  13:22:24  todd

 **	Fixed a few little bugs.  Version 2.4 of voxvu write ^L out to the

 **	image file as the ascii string "^L" where version 2.3 writes it out

 **	as the printf string "\f".  I think this is a bug in 2.4.  I haven't

 **	done anything about it yet.

 **

 **	Revision 1.1  90/04/05  17:05:53  todd

 **	Initial revision

 ** 

 **/



#include "iminternal.h"





/**

 **

 **  FORMAT

 **  	IFF - Sun TAAC Image File Format

 **

 **  AKA

 ** 	vff, suniff, taac

 **

 **  FORMAT REFERENCES

 **	Sun-Taac User Manual, Volume Rendering Package, Sun Microsystems

 **

 **  CODE CREDITS

 **     Custom development, Todd Elvins, San Diego Supercomputer Center, 1990.

 **

 **  DESCRIPTION

 **	A typical IFF ascii header looks like this:

 **

 **		ncaa

 **		rank=2;

 **		size=512 512;

 **		bands=3;

 **		bits=8 8 8;

 **		format=base;

 **		^L

 **		(followed by lots of raw pixel data)

 **

 **	The header can also contain a colormap such as this one:

 **		bands=1;

 **		bits=8;

 **		colormapsize=256;

 **		colormap= ...

 **

 **

 **	Storage formats "Block Pseudo" and "Two Bit Movie Dump" are specific

 **	to the Sun-TAAC system, and may well become obsolete within 6 months.

 **	For these reasons they are not supported at this time.

 **

 **

 **

 **/



#ifdef __STDC__

static int imIffRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imIffWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

#else

static int imIffRead( ), imIffWrite( );

#endif

static char *imIffNames[ ]  = { "iff", "vff", "suniff", "taac", NULL };

static unsigned char imIffMagicNumber[ ] = { 'n', 'c', 'a', 'a' };

static ImFileMagic imFileIffMagic []=

{

	{ 0, 4, imIffMagicNumber},

	{ 0, 0, NULL },

};



static ImFileFormatReadMap imIffReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IN,1,8,       0,      IMVFBINDEX8,    0 },

        { IN,1,8,       C,      IMVFBINDEX8,    C },

        { RGB,3,8,      0,      IMVFBRGB,       0 },

        { RGB,3,8,      A,      IMVFBRGB,       A },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imIffWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBINDEX8,  0,      IN,1,8,         0,      imIffWrite },

        { IMVFBINDEX8,  C,      IN,1,8,         C,      imIffWrite },

        { IMVFBRGB,     0,      RGB,3,8,        0,      imIffWrite },

        { IMVFBRGB,     A,      RGB,3,8,        A,      imIffWrite },

        { -1,           0,      -1,             0,      NULL },

};





ImFileFormat ImFileIffFormat =

{

	imIffNames, "Sun TAAC Image File Format",

	"Sun Microsystems, Inc.",

	"8-bit, 24-bit RGB, and 32-bit RGB+alpha image.",

	"8-bit, 24-bit RGB, and 32-bit RGB+alpha image.",

	imFileIffMagic,

	IMNOMULTI, IMNOPIPE,

	IMNOMULTI, IMNOPIPE,

	imIffRead, imIffReadMap, imIffWriteMap

};









/*

 *  TYPEDEF & STRUCTURE

 * 	IMIFFMAGIC	The magic number at the beginning of an IFF file.

 * 	IMIFFMAGICCNT	The number of bytes of magic number in an IFF file.

 * 	IMIFF24BITHEADSTRINGS	Required number of keywords

 * 	IMIFFHEADSTRINGS	Required plus optional keywords

 * 	IMIFFSTRLENGTH	The number of bytes in a long string.

 * 	IMIFFLONGBUFF	In case we have to read in an ascii colormap

 * 	IMRANK		The index of the rank entry in the imIffHeader array.

 * 	IMSIZE		The index of the size entry in the imIffHeader array.

 * 	IMBANDS		The index of the bands entry in the imIffHeader array.

 * 	IMBITS		The index of the bits entry in the imIffHeader array.

 * 	IMFORMAT		The index of the format entry in the imIffHeader array.

 * 	IMCOLORMAPSIZE	The index of the colormapsize entry in imIffHeader array

 * 	IMCOLORMAP	The index of the colormap entry in the imIffHeader array

 *

 *  DESCRIPTION

 *	These are keywords to be expected in an IFF file image header.

 */



#ifdef __STDC__

static int imGetFDStr( int ioType, int fd, FILE *fp, char *s );

#else

static int imGetFDStr( );

#endif



#define 	IMIFFMAGIC	"ncaa"

#define 	IMIFFMAGICCNT	4

#define 	IMIFFSTARTPIXELS	1

#define 	IMIFF24BITHEADSTRINGS	5

#define 	IMIFFHEADSTRINGS	7

#define 	IMIFFSTRLENGTH	256

#define 	IMIFFLONGBUF	8192

#define 	IMRANK		0

#define 	IMSIZE		1

#define 	IMBANDS		2

#define 	IMBITS		3

#define 	IMFORMAT	4

#define 	IMCOLORMAPSIZE	5

#define 	IMCOLORMAP	6





/*

 *  TYPEDEF & STRUCTURE

 *	imIffHeader	-  IFF file header information

 *

 *  DESCRIPTION

 *	A IFF file's header contains the image's dimensionality,

 *	width, height, depth, bits/pixel, and storage.

 */



typedef struct imIffHeaderInfo

{

	char *name;

	int set;

} imIffHeaderInfo ;





static imIffHeaderInfo imIffHeader[] = 

{

 	"rank", 	FALSE,

 	"size", 	FALSE,

 	"bands", 	FALSE,

 	"bits",		FALSE,

 	"format",	FALSE,

 	"colormapsize",	FALSE, 

 	"colormap",	FALSE 

} ;







/*

 *  FUNCTION

 *	imGetFDStr	- Read a stream of bytes

 *

 *  DESCRIPTION

 *	Read from the file descriptor fd until EOL.  Put the string

 *	into s.  Return -1 on failure.

 */

static int

#ifdef __STDC__

imGetFDStr( int ioType, int fd, FILE *fp, char *s )

#else

imGetFDStr( ioType, fd, fp, s )

	int ioType;

	int fd;

	FILE *fp;

	char *s;

#endif

{

	char *buf;			/* The buffer to fill up 	 */

	int cnt;			/* Count the number of byte read */



	buf = s;

	cnt = 0;



	do

	{

		if ( ImBinRead( ioType, fd, fp, buf, CHAR, 1, 1 ) == -1 )

		{

			ImReturnBinError( );

		}



		if (++cnt >= IMIFFLONGBUF )

		{

			ImErrNo = IMESYNTAX;

			ImErrorFatal( ImQError( ), -1, ImErrNo );

		}



		if (*buf == 0x0c )		/* ^L */

		{

			/* Skip the following carriage return */

			if ( ImBinRead( ioType, fd, fp, buf, CHAR, 1, 1 )== -1)

			{

				ImReturnBinError( );

			}

			return( IMIFFSTARTPIXELS );

		}

	}

	while ( *buf++ != '\n' );



	*--buf = '\0';

	return( 0 );

}











/*

 *  FUNCTION

 *	imIffRead	-  read a Sun-TAAC image file

 *

 *  DESCRIPTION

 *	The file header is read and the size of the image determined.

 *	Space is allocated for the VFB.  The image data is stored in

 *	either RGB or RGBA.  The RGB[A] data is read into the VFB one

 *	scanline at a time and the completed VFB is added to the tag list.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__

imIffRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imIffRead( ioType, fd, fp, flagsTable, tagTable )

        int         ioType;             /* I/O flags                    */

        int         fd;                 /* Input file descriptor        */

        FILE       *fp;                 /* Input file pointer           */

	TagTable   *flagsTable;		/* Flags			*/

        TagTable   *tagTable;           /* Tag list to add to           */

#endif

{

	ImVfb       *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	ImClt       *clt=NULL;		/* Read in image		*/

	ImCltPtr     cptr;		/* Pixel pointer		*/

	unsigned char       *runBuffer;	/* Run buffer			*/

	unsigned char       *rbp;	/* Run buffer pointer		*/

 	char	    *eSign;		/* Pointer into an input  string*/

 	char	    *nextstr;		/* Pointer into an input  string*/

	unsigned char	     magic[IMIFFSTRLENGTH];	/* Store the magic number*/

	char	     buf[IMIFFLONGBUF];	/* Keyword buffer	*/

	char	     key[IMIFFSTRLENGTH];	/* Keyword buffer	*/

 	char	     format[IMIFFSTRLENGTH];	/* Storage == "Base"	*/

	char         msg[IMIFFSTRLENGTH];	/* Tmp message holder	*/

	int	     rank;		/* Dimensionality		*/

	int	     sizex, sizey;	/* Image width and height	*/	

	int	     bands;		/* Either 3 or 4 RGB or RGBA	*/

	int 	     bitsr,bitsg,bitsb,bitsa;	/* Image depth		*/

 	int	     status;		/* Returned function status	*/

	int	     cltsize;		/* How many clt entries		*/

 	unsigned int	     rgbFlag;		/* Is there an alpha channel	*/

 	unsigned int	     rgbvals;		/* An rgb clt entry		*/

	int i,j,cnt=0;		/* Loop counters		*/

	char		 message[100];	/* ImInfo message		*/



	/*

	 *  IFF files are usually generated on Sun-TAACs, which have an MBF

	 *  byte order.  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINMBF );



	/*

	 *  Read in the magic number and check it.

	 */

	if ( ImBinRead( ioType, fd, fp, magic, UCHAR, 1, IMIFFMAGICCNT+1)== -1 )

		ImReturnBinError( );



	if ( strncmp( IMIFFMAGIC, (char*)magic, IMIFFMAGICCNT ) != 0)

	{

		ImErrNo = IMEMAGIC;

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	

	/*

	 *  Read in the file header.

	 */

	while((status = imGetFDStr( ioType, fd, fp, buf )) != IMIFFSTARTPIXELS )

	{

		if ( status == -1 )

			return( -1 );		/* ImErrNo already set */



		if ( *buf == '\n' || *buf == '\0' )

			continue;



		/*

		 * Get the keyword out of the string into key var

		 */

		if ( (eSign = (char *)strchr( buf, '=' )) == NULL )

		{

			ImErrNo = IMESYNTAX;

			ImErrorFatal( ImQError( ), -1, ImErrNo );

		}



		*eSign++ = '\0';	/* Skip the equal sign */

		sscanf( buf, "%s", key );



		/*

		 * Look in the table of known keywords for a match

		 */

		for( j=0; j<IMIFFHEADSTRINGS; j++ )

			if ( strcmp( imIffHeader[j].name, key ) == 0 )  

			{

				imIffHeader[j].set = TRUE;

				break;

			}



		/*

		 * Now that we know what the keyword is, we can read in the

		 * associated parameters and do some more error checking.

	 	 */



		switch (j) 

		{

		case IMRANK:

			status = sscanf(eSign,"%d;\n",&rank);

			if ( status != 1 || rank != 2 )

			{

				ImErrorInfo( "IFF header:  rank", -1,IMESYNTAX);

				ImErrNo = IMESYNTAX;

				ImErrorFatal( ImQError( ), -1, ImErrNo );

			}

			break;



		case IMSIZE:

			status = sscanf(eSign,"%d %d;\n",&sizex, &sizey);

			if ( status != 2 )

			{

				ImErrorInfo( "IFF header:  sizes", -1, IMESYNTAX );

				ImErrNo = IMESYNTAX;

				ImErrorFatal( ImQError( ), -1, ImErrNo );

			}

			sprintf (message, "%d x %d",sizex,sizey);

			ImInfo ("Resolution",message);

			break;



		case IMBANDS:

			status = sscanf(eSign,"%d;\n",&bands);



			switch ( bands )

			{

			case 4:

				rgbFlag = IMVFBRGB | IMVFBALPHA;

				ImInfo ("Type","24-bit RGB");

				ImInfo ("Alpha Channel","8 bit");

				break;

			case 3:

				rgbFlag = IMVFBRGB;

				ImInfo ("Type","24-bit RGB");

				ImInfo ("Alpha Channel","none");

				break;

			case 1:

				rgbFlag = IMVFBINDEX8;

				ImInfo ("Type","8-bit Color Indexed");

				ImInfo ("Alpha Channel","none");

				break;

			default:

				ImErrNo = IMEDEPTH;

				ImErrorFatal( "Illegal bands in header", 

								-1, ImErrNo );

			}

			break;



		case IMBITS:

			status = sscanf(eSign,"%d %d %d %d;\n",

						&bitsr,&bitsg,&bitsb,&bitsa);



			if ( status < 1 || bitsr != 8 )

			{

				ImErrNo = IMEDEPTH;

				ImErrorFatal( "Illegal bits in header", -1, 

								ImErrNo );

			}

			break;



		case IMFORMAT:

			status = sscanf(eSign,"%s\n",format);

			if (( strncmp(format,"base",4) != 0 ) || status != 1)  

			{

				ImErrNo = IMEFORMAT;

				ImErrorFatal( "Illegal format", -1, ImErrNo );

			}

			break;



		case IMCOLORMAPSIZE:

			status = sscanf(eSign,"%d;\n",&cltsize);

			if ( cltsize < 1 )

			{

				ImErrNo = IMEFORMAT;

				ImErrorFatal( "Illegal CLT size", -1, ImErrNo );

			}

			clt = ImCltAlloc( cltsize );

			if ( clt == IMCLTNULL )

			{

				ImErrorFatal( ImQError( ), -1, ImErrNo );

			}

			sprintf (message, "%d Entries",cltsize);

			ImInfo ("Color Table",message);

			break;



		case IMCOLORMAP:

			cptr = ImCltQFirst( clt );

			

			nextstr = (char *) strtok(eSign, " " ) ;

			if (nextstr == NULL)

			{

				ImErrNo = IMESYNTAX;

				ImErrorFatal( "Can't read colormap",-1,ImErrNo );

			}

			sscanf( nextstr,"%x",&rgbvals );



			ImCltSBlue(  cptr, rgbvals>>16 & 0xff );

			ImCltSGreen( cptr, rgbvals>> 8 & 0xff );

			ImCltSRed(   cptr, rgbvals     & 0xff );

			ImCltSInc( clt, cptr );



			for( i=1; i<cltsize; i++ )

			{

				if ((nextstr = (char *)strtok( NULL, " " )) == NULL)

				{

					ImErrNo = IMESYNTAX;

					ImErrorFatal( "Cant read colormap", 

								-1, ImErrNo );

				}

				sscanf( nextstr,"%x",&rgbvals );

				ImCltSBlue(  cptr, rgbvals>>16 & 0xff );

				ImCltSGreen( cptr, rgbvals>> 8 & 0xff );

				ImCltSRed(   cptr, rgbvals     & 0xff );

				ImCltSInc( clt, cptr );

			}

			break;



		default:

			sprintf( msg, "Unknown keyword '%s' in file header\n",

									 key );

                        ImErrorWarning( msg, -1, IMESYNTAX );



		} /* end switch 	*/				



	} /* end foreach keyword loop	*/





	/*

	 * Do sanity checking.  Some of the keywords are mandatory.

	 */

	for( i=0; i<IMIFF24BITHEADSTRINGS;i++ )

		if (imIffHeader[i].set != TRUE)

		{

			ImErrorFatal( "Missing keyword in IFF header", -1, 

								IMESYNTAX );

		}





	/*

	 *  Allocate a VFB of the required size.

	 */

	if ( (vfb = ImVfbAlloc( sizex, sizey, rgbFlag )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	/*

	 *  Allocate a scanline buffer;

	 */

	ImMalloc( runBuffer, unsigned char *, bands * sizex );



 	pptr = ImVfbQFirst( vfb );



	/*

	 * Foreach scanline, readin a scanline worth of pixels and put them

	 * into the vfb.  There may or may not be an alpha channel present.

	 * format=base indicates that the pixel data is stored AGBRAGBR...

	 */

	for( i=0; i<sizey; i++ )  

	{

		rbp   = runBuffer;



	 	if ((ImBinRead( ioType, fd, fp, rbp, UCHAR, 1, bands * sizex)) == -1) 

		{

			free( (char *)runBuffer );

			ImReturnBinError( );

		}	



		if (bands == 4)

			for( j=0; j<sizex; j++ ) 

			{

				ImVfbSAlpha( vfb, pptr, *rbp++ );

				ImVfbSBlue(  vfb, pptr, *rbp++ );

				ImVfbSGreen( vfb, pptr, *rbp++ );

				ImVfbSRed(   vfb, pptr, *rbp++ );

				ImVfbSInc(   vfb, pptr );

			}

		else if ( bands == 3 )

			for( j=0; j<sizex; j++ ) 

			{

				ImVfbSBlue(  vfb, pptr, *rbp++ );

				ImVfbSGreen( vfb, pptr, *rbp++ );

				ImVfbSRed(   vfb, pptr, *rbp++ );

				ImVfbSInc(   vfb, pptr );

			}

		else	/* bands == 1 */

			for( j=0; j<sizex; j++ ) 

			{

				ImVfbSIndex8(  vfb, pptr, *rbp++ );

				ImVfbSInc(   vfb, pptr );

			}

	}

	free( (char *)runBuffer );





	/*

	 * If there is a CLT, associate it with the vfb and put it in the

	 * tag table.  Put the VFB in teh tag table.

	 */

	if ( clt != IMCLTNULL )

	{

		ImVfbSClt( vfb, clt );

        	TagTableAppend( tagTable,

			TagEntryAlloc( "image clt", POINTER, &clt));

		TagTableAppend( tagTable,

			TagEntryAlloc( "image vfb", POINTER, &vfb ) );

		return ( 2 );

	}

	TagTableAppend( tagTable,

		TagEntryAlloc( "image vfb", POINTER, &vfb ) );

	return ( 1 );

}











/*

 *  FUNCTION

 *	imIffWrite	-  write an Sun TAAC image file

 *

 *  DESCRIPTION

 *	The VFB is queried, and the IFF file header set up and written out.

 *	The VFB data is then written out in AGBRAGBR... unencoded pixel format.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imIffWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imIffWrite( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

        int          ioType;            /* I/O flags                    */

        int          fd;                /* Input file descriptor        */

        FILE        *fp;                /* Input file pointer           */

	TagTable    *flagsTable;	/* Flags			*/

        TagTable    *tagTable;          /* Tag list to read from        */

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	ImClt        *clt;		/* Pixel pointer		*/

	ImCltPtr     pclt;		/* Pixel pointer		*/

	unsigned char       *runBuffer;		/* Run buffer			*/

	unsigned char *rbp;		/* Run buffer pointer		*/

	char	     buf[512];		/* Keyword buffer		*/

	int	     sizex, sizey;	/* Image width and height	*/	

	int	     bands;		/* Either 3 or 4 RGB or RGBA	*/

	int 	     bits;		/* Image depth			*/

	int 	     fields;		/* Vfb fields			*/

	int 	     cltsize;		/* Number of clt entries	*/

	unsigned int	     rgbval;		/* One rgb color table entry	*/

	int i;			/* Loop counter 		*/

	int x, y;		/* Pixel counters		*/

	char		 message[100];	/* ImInfo message		*/







	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	fields = ImVfbQFields( vfb );



	bits = 8;



	if ( fields & IMVFBINDEX8 )

		bands = 1;

	else

	{

		/* RGB */

		bands = 3;

		if ( pMap->map_outAttributes & IMALPHAYES )

			bands++;

	}



	/*

	 *  IFF files are usually generated on Sun-TAACs, which have an MBF

	 *  byte order.  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINMBF );



	/*

	 *  Set up the header and write it out.

	 */

	sizex  = ImVfbQWidth( vfb );

	sizey = ImVfbQHeight( vfb );



	/*

	 * Dump out the ascii header

	 */

	sprintf( buf, "ncaa\n\0" );		/* Magic		*/

	ImBinWrite( ioType, fd, fp, buf, UCHAR, 1, strlen(buf) );



	sprintf( buf, "rank=%d;\n\0", 2 );	/* Two dimensional	*/

	ImBinWrite( ioType, fd, fp, buf, UCHAR, 1, strlen(buf) );



	sprintf( buf, "bands=%d;\n\0", bands );	/* Depth 		*/

	ImBinWrite( ioType, fd, fp, buf, UCHAR, 1, strlen(buf) );



	sprintf( buf, "size=%d %d;\n\0", sizex, sizey );/* Width height */ 

	ImBinWrite( ioType, fd, fp, buf, UCHAR, 1, strlen(buf) );

        sprintf (message, "%d x %d",sizex,sizey);

        ImInfo ("Resolution",message);



	sprintf( buf, "format=%s;\n\0", "base" ); /* Storage format	*/ 

	ImBinWrite( ioType, fd, fp, buf, UCHAR, 1, strlen(buf) );



	if ( bands == 1 )

	{

		sprintf( buf, "bits=%d;\n\0", bits );  

                ImInfo ("Type","8-bit Color Indexed");

	}

	else if ( bands == 3 )

	{

		sprintf( buf, "bits=%d %d %d;\n\0", bits, bits, bits );  

                ImInfo ("Type","24-bit RGB");

	}

	else if ( bands == 4 )

	{

		sprintf( buf, "bits=%d %d %d %d;\n\0", bits, bits, bits, bits );

                ImInfo ("Type","32-bit RGB and Alpha");

	}

	ImBinWrite( ioType, fd, fp, buf, UCHAR, 1, strlen(buf) );



	if ( pMap->map_outAttributes & IMCLTYES )

	{

        	clt = ImVfbQClt( vfb );

		cltsize = ImCltQNColors( clt );

		pclt = ImCltQFirst( clt );



                sprintf (message, "%d Entries",cltsize);

                ImInfo ("Color Table",message);

 

		sprintf( buf, "colormapsize=%d;\n\0", cltsize);

		ImBinWrite( ioType, fd, fp, buf, UCHAR, 1, strlen(buf) );



		sprintf( buf, "colormap=\0");

		ImBinWrite( ioType, fd, fp, buf, UCHAR, 1, strlen(buf) );



		for( i=0; i<cltsize; i++ )

		{

			rgbval = ImCltQRed( pclt );

			rgbval = (((unsigned int)ImCltQGreen( pclt )) << 8 ) | rgbval;

			rgbval = (((unsigned int)ImCltQBlue( pclt ))  << 16) | rgbval;

			ImCltSInc( clt, pclt ); 

			sprintf( buf, "%06x ", rgbval );

			ImBinWrite( ioType, fd, fp, buf, UCHAR, 1, strlen(buf));

		}

		sprintf( buf, ";\n\0");

		ImBinWrite( ioType, fd, fp, buf, UCHAR, 1, strlen(buf) );

	}

		

	sprintf( buf, "\f\n\0");		/* ^L signals data	*/

	ImBinWrite( ioType, fd, fp, buf, UCHAR, 1, strlen(buf) );



	/*

	 *  Allocate space for a run buffer large enough for 1 raw scanline

	 */

	ImMalloc( runBuffer, unsigned char *, bands * sizex );



	pptr  = ImVfbQFirst( vfb );



	/*

	 * There is some repetitive code here in order to keep from putting

 	 * the conditional inside the loop.  It was running really slow for

	 * some reason.  

	 */

	if ( 1 == bands )

		for ( y = 0; y < sizey; y++ )

		{

			rbp = runBuffer;

			for ( x = 0; x < sizex; x++ )

			{

				*rbp++ = ImVfbQIndex8( vfb, pptr );

				ImVfbSInc( vfb, pptr );

			}

			/*

		 	 *  Write out the run buffer.

		 	 */

			if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, 

							sizex * bands ) == -1)

			{

				free( (char *)runBuffer );

				ImReturnBinError( );

			}

		}

	else if ( 3 == bands )

		for ( y = 0; y < sizey; y++ )

		{

			rbp = runBuffer;

			for ( x = 0; x < sizex; x++ )

			{

				*rbp++ = ImVfbQBlue( vfb, pptr );

				*rbp++ = ImVfbQGreen( vfb, pptr );

				*rbp++ = ImVfbQRed( vfb, pptr );

				ImVfbSInc( vfb, pptr );

			}

			/*

		 	 *  Write out the run buffer.

		 	 */

			if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, 

							sizex * bands ) == -1)

			{

				free( (char *)runBuffer );

				ImReturnBinError( );

			}

		}

	else       /* bands == 4 */

		for ( y = 0; y < sizey; y++ )

		{

			rbp = runBuffer;

			for ( x = 0; x < sizex; x++ )

			{

				*rbp++ = ImVfbQAlpha( vfb, pptr );

				*rbp++ = ImVfbQBlue( vfb, pptr );

				*rbp++ = ImVfbQGreen( vfb, pptr );

				*rbp++ = ImVfbQRed( vfb, pptr );

				ImVfbSInc( vfb, pptr );

			}

			/*

		 	 *  Write out the run buffer.

		 	 */

			if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, 

							sizex * bands ) == -1)

			{

				free( (char *)runBuffer );

				ImReturnBinError( );

			}

		}



	free( (char *)runBuffer );

	return ( 1 );

}

