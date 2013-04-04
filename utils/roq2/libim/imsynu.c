/**

 **	$Header: /roq/libim/imsynu.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imsynu.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imsynu.c	-  Synu image file I/O

 **

 **  PROJECT

 **	libim		-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imsynu.c contains routines to read and write Synu image files

 **	for the image manipulation library.

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

 **     imSynuRead              f  read a Synu image file

 **     imSynuWrite             f  write a Synu image file

 **	imSynuRead		f  read a Synu image file

 **	imSynuWrite		f  write a Synu image file

 **

 **	IMSYNUMAGIC		d  file magic "number"

 **	IMSYNURGB		d  mode string identifying RGB format

 **	IMSYNUGRAY		d  mode string identifying gray-scale format

 **

 **	imGetFDStr		f  read a string in from fd or fp

 **

 **  HISTORY

 **	$Log: /roq/libim/imsynu.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.16  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.15  1995/06/15  21:14:33  bduggan

 **	Took out an embedded comment

 **

 **	Revision 1.14  1995/04/03  21:37:14  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.13  1995/01/10  23:43:54  bduggan

 **	put in IMMULTI, IMPIPE instead of TRUE/FALSE

 **	made read/write routines static

 **

 **	Revision 1.12  94/10/03  11:30:58  nadeau

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

 **	Revision 1.11  92/12/03  01:52:31  nadeau

 **	Corrected info messages.

 **	

 **	Revision 1.10  92/11/04  12:07:48  groening

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.9  92/10/19  14:09:40  groening

 **	added ImINfo statements

 **	

 **	Revision 1.8  92/08/31  17:35:37  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.7  92/04/09  09:31:57  groening

 **	To make the compiler happy added extern statements.

 **	

 **	Revision 1.6  91/10/03  09:19:34  nadeau

 **	Fixed #includes.

 **	

 **	Revision 1.5  91/03/14  14:41:14  nadeau

 **	Changed 1L to 4L in line skip of image object.

 **	Added return(0) to end of write... was causing

 **	weirdness on Alliant FX/2800.

 **	

 **	Revision 1.4  91/03/08  14:33:43  nadeau

 **	Cleaned up some error checking and changed the name of the

 **	defined name globals giving buffer sizes.

 **	

 **	Revision 1.3  91/02/12  11:39:56  nadeau

 **	Almost completely rewrote.  Removed the tag table checking

 **	now handled by ImFileRead and ImFileWrite.  Removed

 **	goto's.  Removed bogus static variables.  Moved loop-

 **	invariant code outside of loops.  Removed illegal read

 **	call that read directly into VFB (thus assuming internal

 **	data structure it shouldn't have).  Buffered up writes by

 **	scanline rather than issue a write system call PER PIXEL.

 **	And generally optimized the code into a usable state.

 **	

 **	Revision 1.2  91/01/09  14:02:00  mercurio

 **	Minor spelling errors in comments fixed.

 **	

 **	Revision 1.1  90/09/20  09:46:38  mercurio

 **	Initial revision

 ** 

 **/





#include "iminternal.h"







/*

 **  FORMAT

 **	SYNU  -  Synthetic Universe

 **

 **  AKA

 **

 **  FORMAT REFERENCES

 **	Synu Reference Manual, San Diego Supercomputer Center

 **

 **  CODE CREDITS

 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1991.

 ** 

 **  DESCRIPTION

 **	Synu images are stored in Synu object files, which may contain

 **	any of a number of objects of any type.  We're interested in

 **	only the "image" (IMSYNUMAGIC) objects--all others are skipped

 **	past.

 **

 **	Each object is preceeded by a header string, in ASCII, terminated

 **	by a newline character.  The first space-delimited string in the

 **	header is the object type.  The remaining strings are number-letter

 **	pairs (eg.: 786432b) describing the object contents.  The letter

 **	may be either 'l', indicating that number of lines (newline-delimited

 **	strings), or 'b', indicating that number of bytes.  'L' and 'B'

 **	are also acceptable, and mean the same as their lower-case counter-

 **	parts.  For example:

 **

 **		image 4l 786432b

 **

 **	indicates an "image" object, consisting of 4 lines followed by

 **	786432 bytes.  Reading four lines then 786432 bytes will skip

 **	past the entire object.

 **

 **	The image object type will always consist of 4 header lines

 **	followed by some number of bytes.  The header lines are of the

 **	form:

 **

 **		<x>

 **		<y>

 **		<bytes>

 **		<mode>

 **

 **	where <x> and <y> are the width and height of the image, <bytes>

 **	is the number of bytes per pixel, and <mode> is a string describing

 **	how the pixels are stored.  Currently, <mode> will either be

 **	"rgb" for a three-byte-per-pixel, red-green-blue format, or

 **	"gray" for a one-byte-per-pixel, gray-scale format.  Possible

 **	future extensions include "rgba", for RGB plus an alpha channel,

 **	or allowing the pixel components to be specified in another

 **	order, such as "bgr".

 **

 **	Following the image header lines are <x> * <y> * <bytes> bytes,

 **	representing the pixel values in scanline order.  The origin

 **	of the image is in the lower left corner.

 **/



/*

 *  SYNU - SDSC Synthetic Universe image renderer file

 */



#ifdef __STDC__

static int imGetFDStr( int ioType, int fd, FILE *fp, char *buf, int size);

static int imSynuRead(int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

static int imSynuWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

#else

static int imGetFDStr();

static int imSynuRead( );

static int imSynuWrite( );

#endif

static char *imSynuNames[ ]  = { "synu", NULL };

static ImFileFormatReadMap imSynuReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IN,1,8,       0,      IMVFBINDEX8,    0 },

        { RGB,3,8,      0,      IMVFBRGB,       0 },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imSynuWriteMap[ ] =

{

        /*

         *  For Synu, all image types vector thru to the same imSynuWrite()

         *  routine.  This is necessary because a tagTable may be passed in

         *  that contains multiple VFB's, each with different depth, CLT

         *  and alpha attributes.  The write map primarily serves as a

         *  filter to make sure all such incomming VFB's are in one of the

         *  supported formats.

         */



        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBINDEX8,  0,      IN,1,8,         0,      imSynuWrite },

        { IMVFBRGB,     0,      RGB,3,8,        0,      imSynuWrite },

        { -1,           0,      -1,             0,      NULL },

};



static ImFileMagic imFileSynuMagic []=

{

	{ 0, 0, NULL},

};



ImFileFormat ImFileSynuFormat =

{

	imSynuNames, "SDSC Synu image file",

	"SDSC",

	"8-bit grayscale and 24-bit RGB uncompressed image files.",

	"8-bit grayscale and 24-bit RGB uncompressed image files.",

	imFileSynuMagic,

	IMMULTI, IMPIPE,

	IMMULTI, IMPIPE,

	imSynuRead, imSynuReadMap, imSynuWriteMap

};











/*

 *  CONSTANTS

 *	IMSYNUMAGIC	-  file magic "number" (see below)

 *	IMSYNURGB	-  mode string indicating RGB data

 *	IMSYNUGRAY	-  mode string indicating gray-scale data

 *

 *  DESCRIPTION

 *	IMSYNUMAGIC is the string identifying an image in the

 *	Synu object header.  It serves as a magic number, also.

 *

 *	IMSYNURGB and IMSYNUGRAY are possible values for the mode

 *	field of an image header line.

 */

#define IMSYNUMAGIC	"image"

#define IMSYNURGB	"rgb"

#define IMSYNUGRAY	"gray"











/*

 *  FUNCTION

 *	imSynuRead	-  read a Synu image file

 *

 *  DESCRIPTION

 *	Each object in the input stream is read.  "image" objects

 *	are reading 24- or 8-bit VFBs and added to the tagTable.

 *	All other objects are ignored.

 */

#define IMSYNUSTRSIZE		(256)		/* size of header string buffer */

#define IMSYNUBUFSIZE		(2048)		/* buffer size			*/



static int				/* Returns status		*/

#ifdef __STDC__

imSynuRead(int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imSynuRead(ioType, fd, fp, flagsTable, tagTable)

	int              ioType;	/* I/O flags			*/

	int	         fd;		/* Input file descriptor	*/

	FILE	        *fp;		/* Input file pointer		*/

	TagTable        *flagsTable;	/* Flags			*/

	TagTable        *tagTable;	/* Tag table to add to		*/

#endif

{



	ImVfb		 *vfb;		/* Read in image		*/

	ImVfbPtr	p;	/* pixel pointer		*/

	int		i,y;		/* Counters			*/

	int		status;		/* Return status holder		*/

	char		str[IMSYNUSTRSIZE];	/* string used for header input */

	char		junk[IMSYNUBUFSIZE];	/* garbage buffer		*/

	char *		pHdr;		/* pointer into header string	*/

	int		size;		/* size of object part		*/

	char		type;		/* type of object part		*/

	int		nRead;		/* # of bytes scanned from string */

	int		width;		/* width of image		*/

	int		height;		/* height of image		*/

	int		nBytes;		/* number of bytes per pixel	*/

	char 		mode[IMSYNUSTRSIZE];	/* storage mode for image	*/

	unsigned char	       *buffer;		/* Allocated buffer		*/

	unsigned char	       *bp;		/* Buffer pointer		*/

	int		n;		/* # of bytes			*/

	char		message[256];	/* print buffer for use with ImInfo */	



	/*

	 * SYNU files don't really have a byte order, since everything

	 * is either ASCII or raw bytes.  We set an order anyway, just to

	 * be sure.

	 */

	BinByteOrder( BINMBF );



	/*

	 * Loop until we get an EOF

	 */

	for ( ; ; )

	{

		/*

		 *  Read in the magic number.

		 */

		status = imGetFDStr(ioType,fd,fp,str,IMSYNUSTRSIZE);

		if ( status == -1)	/* EOF reached, not an error	*/

			break;

		if ( status == -2)	/* a read error			*/

		{

			ImReturnBinError();

		}



		if ( strncmp(IMSYNUMAGIC,str,strlen(IMSYNUMAGIC)) != 0)

		{		/* uninteresting object, skip it 	*/

			pHdr = &str[strlen(IMSYNUMAGIC)];

			while(sscanf(pHdr,"%d%c%n",&size,&type,&nRead) >= 2)

			{

				pHdr += nRead;



				switch(type)

				{

				case 'b':

				case 'B':	/* skip bytes	*/

					for ( ; size > 0; size-=IMSYNUBUFSIZE)

						if ( ImBinRead(ioType,fd,fp,

							junk,CHAR,1,IMSYNUBUFSIZE)<0)

						{

							ImReturnBinError();

						}

					break;



				case 'l':

				case 'L':	/* skip lines	*/

					for ( ; size > 0; size-- )

						if ( imGetFDStr(ioType,fd,fp,

							junk,IMSYNUBUFSIZE) < 0)

						{

							ImReturnBinError();

						}

					break;

				}

			}

			continue;

		}



		/* An image, read it in					*/



		/*

		 * Read width, height, nBytes, and mode (4 separate lines).

		 */

		if ( imGetFDStr(ioType,fd,fp,str,IMSYNUSTRSIZE) < 0)

		{

			ImReturnBinError( );

		}

		sscanf(str,"%d",&width);



		if ( imGetFDStr(ioType,fd,fp,str,IMSYNUSTRSIZE) < 0)

		{

			ImReturnBinError( );

		}

		sscanf(str,"%d",&height);



		if ( imGetFDStr(ioType,fd,fp,str,IMSYNUSTRSIZE) < 0)

		{

			ImReturnBinError( );

		}

		sscanf(str,"%d",&nBytes);



		if ( imGetFDStr(ioType,fd,fp,str,IMSYNUSTRSIZE) < 0)

		{

			ImReturnBinError( );

		}

		sscanf(str,"%s",mode);



		/* These lines sre to printout if a verbose flag

			has been set in imfile or imconv */

		ImInfo ("Byte Order", "Most Significant Byte First");	

		sprintf (message, "%d x %d",width, height);

		ImInfo ("Resolution",message);



		/*

		 * Allocate the correct VFB

		 */

		if ( strcmp(mode,IMSYNURGB) == 0)

		{

			ImInfo ("Type","24-bit RGB");

			vfb = ImVfbAlloc(width,height,IMVFBRGB);

		}

		else

		{

			ImInfo ("Type","8-bit Grayscale");

			vfb = ImVfbAlloc(width,height,IMVFBINDEX8);

		}

		if ( vfb == IMVFBNULL)

		{

			ImErrorFatal(ImQError(), -1, ImErrNo);

		}





		/*

		 *  Allocate a temp buffer big enough for 1 scanline.

		 */

		n = nBytes * width;

		ImMalloc( buffer, unsigned char *, n );





		/* 

		 * Read pixels in directly, one scanline at a time.

		 */

		p = ImVfbQPtr( vfb, 0, height - 1 );

		for ( y = height-1; y >= 0; y-- )

		{

			if ( ImBinRead( ioType, fd, fp, buffer, UCHAR, 1, n ) != n )

			{

				free( (char *)buffer );

				ImReturnBinError();

			}



			if ( ImVfbQFields( vfb ) & IMVFBINDEX8 )

			{

				for ( i = 0, bp = buffer; i < width; i++ )

				{

					ImVfbSIndex8( vfb, p, *bp++ );

					ImVfbSInc( vfb, p );

				}

			}

			else

			{

				for ( i = 0, bp = buffer; i < width; i++ )

				{

					ImVfbSRed(   vfb, p, *bp );

					ImVfbSGreen( vfb, p, *(bp+1) );

					ImVfbSBlue(  vfb, p, *(bp+2) );

					ImVfbSInc(   vfb, p );

					bp += nBytes;

				}

			}

			ImVfbSUp( vfb, p );

			ImVfbSUp( vfb, p );

		}

			

		/*

		 * Append the new VFB to the tag table

		 */

		TagTableAppend( tagTable, 

			TagEntryAlloc( "image vfb", POINTER, &vfb ) );



		free( (char *)buffer );

	}



	return ( 0 );

}











/*

 *  FUNCTION

 *	imSynuWrite	-  write a Synu image file

 *

 *  DESCRIPTION

 *	All of the VFBs are retrieved from the Tag Table and written

 *	to the output in Synu image format.  Gray-scale VFBs are 

 *	written as gray-scale Synu images, all others are written

 *	as RGB Synu images.  Monochrome VFBs are not supported.

 */



static int				/* Returns status		*/

#ifdef __STDC__

imSynuWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imSynuWrite( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int	     fd;		/* Output file descriptor	*/

	FILE	    *fp;		/* Output file pointer		*/

	TagTable    *flagsTable;	/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	ImVfb *		vfb;		/* Written out image		*/

	ImVfbPtr	p;	/* pixel pointer		*/

	int		fields;		/* VFB flag fields		*/

	int		i,x,y;		/* Counters			*/

	int		nVfbs;		/* number of VFBs		*/

	int		width;		/* width of image		*/

	int		height;		/* height of image		*/

	char		str[IMSYNUSTRSIZE];	/* string used for header input */

	unsigned char	       *buffer;		/* Tmp scanline buffer		*/

	unsigned char          *bp;		/* Buffer pointer		*/

	char		message[512];	/* print buffer for ImInfo	*/





	/*

	 *  Process each of the VFBs

	 */

	nVfbs = TagTableQNEntry(tagTable,"image vfb");

	for ( i=0; i < nVfbs; i++)

	{

		TagEntryQValue( TagTableQDirect( tagTable, "image vfb", i ), &vfb );

		fields = ImVfbQFields(vfb);

		width  = ImVfbQWidth(vfb);

		height = ImVfbQHeight(vfb);



		/* These lines sre to printout if a verbose flag

			has been set in imfile or imconv */

		ImInfo ("Byte Order", "Most Significant Byte First");	

		sprintf (message, "%d x %d",width, height);

		ImInfo ("Resolution",message);



		if ( (fields & IMVFBINDEX8) != 0)

		{

			sprintf (message,"8-bit Grayscale");

			ImInfo ("Type", message);

		}	

		else 

		{

			sprintf (message,"24-bit RGB");

			ImInfo ("Type", message);

		}

		/*

		 *  Process the VFB.

		 */

		if ( (fields & IMVFBINDEX8) != 0)

		{

			/*

			 * Write the object header

			 */

			sprintf(str,"%s 4L %db\n",IMSYNUMAGIC,width*height);

			if ( ImBinWrite(ioType,fd,fp,str,CHAR,1,strlen(str))

				== -1 )

			{

				ImReturnBinError();

			}



			/*

			 * Write the image header

			 */

			sprintf(str,"%d\n%d\n1\n%s\n",width,height,IMSYNUGRAY);

			if ( ImBinWrite(ioType,fd,fp,str,CHAR,1,strlen(str))

				== -1 )

			{

				ImReturnBinError();

			}



			/*

			 *  Allocate a buffer for one scanline.

			 */

			ImMalloc( buffer, unsigned char *, width );



			/*

			 * Write the pixel values, inverting the scanlines

			 * (since a VFB's origin is in the upper left)

			 */

			for ( y = height-1; y >= 0; y-- )

			{

				p = ImVfbQPtr( vfb, 0, y );



				for ( bp = buffer, x = 0; x < width; x++, bp++ )

				{

					*bp = ImVfbQIndex8( vfb, p );

					ImVfbSInc( vfb, p );

				}

				if ( ImBinWrite( ioType, fd, fp, buffer,

					UCHAR, 1, width ) == -1 )

				{

					free( (char *)buffer );

					ImReturnBinError( );

				}

			}



			free( (char *)buffer );

			continue;

		}



		/*

		 * Write the object header

		 */

		sprintf(str,"%s 4L %db\n",IMSYNUMAGIC, width*height*3);

		if ( ImBinWrite(ioType,fd,fp,str,CHAR,1,strlen(str)) == -1 )

		{

			ImReturnBinError();

		}



		/*

		 * Write the image header

		 */

		sprintf(str,"%d\n%d\n%d\n%s\n",width,height,3, IMSYNURGB);

		if ( ImBinWrite(ioType,fd,fp,str,CHAR,1,strlen(str)) == -1 )

		{

			ImReturnBinError();

		}



		/*

		 *  Allocate a buffer for one scanline.

		 */

		ImMalloc( buffer, unsigned char *, width * 3 );



		/*

		 * Write the pixel values, inverting the scanlines

		 * (since a VFB's origin is in the upper left)

		 */

		for ( y = height-1; y >= 0; y-- )

		{

			p = ImVfbQPtr( vfb, 0, y );

			for ( bp = buffer, x = 0; x < width; x++ )

			{

				*bp++ = ImVfbQRed(   vfb, p );

				*bp++ = ImVfbQGreen( vfb, p );

				*bp++ = ImVfbQBlue(  vfb, p );

				ImVfbSInc( vfb, p );

			}

			if ( ImBinWrite( ioType, fd, fp, buffer, UCHAR, 1, width * 3 ) == -1 )

			{

				free( (char *)buffer );

				ImReturnBinError();

			}

		}

		free( (char *)buffer );

	}

	return ( 0 );

}











/*

 *  FUNCTION

 *	imGetFDStr	- Read a stream of bytes, looking for a newline

 *

 *  DESCRIPTION

 *	Read from the file descriptor fd until EOL.  Put the string

 *	into s.  Return -2 on failure, -1 on EOF, 0 on success.

 *

 *  STOLEN FROM

 *	imiff.c  Thanks, Todd!

 */



static int				/* Returns status		*/

#ifdef __STDC__

imGetFDStr( int ioType, int fd, FILE *fp, char *buf, int size)

#else

imGetFDStr( ioType, fd, fp, buf, size)

	int          ioType;		/* I/O flags			*/

	int	     fd;		/* Output file descriptor	*/

	FILE	    *fp;		/* Output file pointer		*/

	char	    *buf;		/* Buffer to use		*/

	int	     size;		/* Size of that buffer		*/

#endif

{

	size--;	/* predecrement, to prevent read on size == 0		*/

	do

	{

		switch ( ImBinRead( ioType, fd, fp, buf, CHAR, 1, 1 ) )

		{

		case -1:	return ( -2 );	/* read error		*/

		case 0:		return ( -1 );	/* EOF			*/

		default:	break;

		}

	}

	while ( size-- && *buf++ != '\n' );



	if ( size <= 0)

		return(-2);			/* No newline?		*/



	*--buf = '\0';

	return( 0 );

}

