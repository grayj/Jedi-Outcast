/**
 **	$Header: /roq/libim/imfile.c 1     11/02/99 4:38p Zaphod $
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

#define HEADER	"	$Header: /roq/libim/imfile.c 1     11/02/99 4:38p Zaphod $"

/**
 **  FILE
 **	imfile.c	-  file I/O for the image manipulation library
 **
 **  PROJECT
 **	libim		-  SDSC image manipulation library
 **
 **  DESCRIPTION
 **	imfile.c contains the top level read and write routines of the
 **	image library and the dispatch table for each format supported.
 **
 **
 **  PUBLIC CONTENTS
 **			d =defined constant
 **			f =function
 **			m =defined macro
 **			t =typedef/struct/union
 **			v =variable
 **			? =other
 **
 **	ImFileQFormat	f  query the format of a file
 **	ImFileQFFormat	f  query the format of a file
 **
 **	ImFileQNFormat	f  query # of formats
 **
 **	ImFileRead	f  read an image file from a file descriptor
 **	ImFileFRead	f  read an image file from a file pointer
 **	ImFileWrite	f  write an image file to a file descriptor
 **	ImFileFWrite	f  write an image file to a file pointer
 **	ImFileFindFormat	f  find the format info given a format name
 **
 **	imFileQCompression	f  query the compression scheme of a file
 **	ImFileFindCompressionScheme	f find the scheme info given a name
 **
 **
 **  PRIVATE CONTENTS
 **	IOTYPE		d  figure the IoType for an unbuffered stream
 **	FIOTYPE		d  figure the IoType for a buffered stream
 **
 **	imFileRead	f  do the real work of read
 **	imFileSelectMap	f  select a good write map to use
 **	imFileReadCompressedFile 	f Read a compressed file
 **	imFileWriteCompressedFile 	f Write a compressed file
 **
 **  HISTORY
 **	$Log: /roq/libim/imfile.c $
 * 
 * 1     11/02/99 4:38p Zaphod
 **	Revision 1.44  1995/08/15  08:37:32  bduggan
 **	Modified ImFileWrite to return the name of the format.
 **
 **	Revision 1.44  1995/08/15  08:37:32  bduggan
 **	Modified ImFileWrite to return the name of the format.
 **
 **	Revision 1.43  1995/06/30  21:59:39  bduggan
 **	removed mktemp prototype
 **	put 'int' between 'static' and the variable name
 **
 **	Revision 1.42  1995/06/29  00:28:04  bduggan
 **	updated copyright year
 **
 **	Revision 1.41  1995/06/29  00:19:29  bduggan
 **	changed error message
 **
 **	Revision 1.40  1995/06/16  08:40:19  bduggan
 **	Improved compression support
 **
 **	Revision 1.39  1995/05/19  13:01:17  bduggan
 **	Added imcompress interface
 **
 **	Revision 1.38  1995/05/06  01:43:56  bduggan
 **	Added .Z compression support!
 **
 **	Revision 1.38  1995/05/06  01:43:56  bduggan
 **	Added .Z compression support!
 **
 **	Revision 1.37  1995/04/03  21:23:57  bduggan
 **	took out #ifdef NEWMAGIC
 **
 **	Revision 1.36  1995/02/16  21:38:44  bduggan
 **	Added support checking for 'image compression quality' tag
 **	Moved ImFileFormatEquivs and ImFileFormatOptions from here
 **	to imtools.c
 **
 **	Revision 1.35  1995/01/10  23:22:33  bduggan
 **	Added grouping option for tiles vs. scanlines
 **
 **	Revision 1.34  94/10/03  11:29:33  nadeau
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
 **	Revision 1.33  93/10/22  23:20:17  nadeau
 **	Added restoration of binary I/O library state around all
 **	file I/O calls.
 **	
 **	Revision 1.32  92/11/24  11:48:58  groening
 **	Corrected problem that caused the wrong write-map to be
 **	selected for non-RGB images when the flagsTable requested
 **	interleaving.
 **	
 **	Revision 1.31  92/11/04  11:45:03  groening
 **	since ImFileFormats got changed around
 **	I changed all the types dealing with
 **	ImFileFormats, also added multiple magic
 **	number capabilities. cheerio!
 **	
 **	Revision 1.30  92/10/12  15:55:23  vle
 **	Added code to handle ImInfo stuff.
 **	
 **	Revision 1.29  92/09/03  17:00:55  nadeau
 **	Fixed a minor typo.
 **	
 **	Revision 1.28  92/09/03  16:47:18  vle
 **	Updated calls to ImVfbGrayRamp() to match new parameters.
 **	
 **	Revision 1.27  92/04/09  09:33:11  groening
 **	In order for the compiler to like the code had to add extern statements.
 **	
 **	Revision 1.26  91/10/03  08:59:13  nadeau
 **	Changed 'interlace' to 'interleave'.  Made format arguments
 **	only show up under -fullhelp.
 **	
 **	Revision 1.25  91/03/12  10:59:49  nadeau
 **	Fixed bug in fopen of temp files for read and write.
 **	
 **	Revision 1.24  91/03/08  14:27:01  nadeau
 **	Totally rewrite.  Moved the formats table and declarations into
 **	a separate file (imfmt.c).  Enhanced ImFileRead to automatically
 **	copy pipe data into a temp file for formats that cannot handle
 **	pipes directly.  Enhanced ImFileWrite to do likewise.  Lots of
 **	changes to ImFileWrite to make it automatically do VFB depth
 **	conversions prior to calling format write code.  It also does
 **	lots of standard error checking.
 **	
 **	Revision 1.23  91/01/30  17:14:36  nadeau
 **	First attempt at restructuring to support more error checking
 **	in ImFile before it gets to the format routines.
 **	
 **	Revision 1.22  91/01/22  09:48:23  nadeau
 **	NULL-ed out all info query calls.  Added more equiv names for
 **	various formats.
 **	
 **	Revision 1.21  90/11/14  18:35:03  mjb
 **	Added cgm support
 **	
 **	Revision 1.20  90/08/30  10:00:27  moreland
 **	Added MacPaint support
 **	
 **	Revision 1.19  90/08/02  13:32:51  mcleodj
 **	Added PIC support
 **	
 **	Revision 1.18  90/07/31  12:42:39  mjb
 **	Changed PICT routine names to match those in impict.c
 **	
 **	Revision 1.17  90/07/30  16:38:47  moreland
 **	Enabled Apple PICT format.
 **	Sorry, no single magic number...
 **	
 **	Revision 1.16  90/07/30  16:31:20  nadeau
 **	Remove 'x' as one of the equivalent names for XWD.  Added support for
 **	the 'x' format used by Stardent for their AVS system.
 **	
 **	Revision 1.15  90/07/25  13:32:02  todd
 **	Add imtiff back in to the list of known formats
 **	
 **	Revision 1.14  90/07/24  15:34:23  ferrerj
 **	Temporarily removed TIFF support.
 **	
 **	Revision 1.13  90/07/24  13:51:23  nadeau
 **	Added XWD support.
 **	
 **	Revision 1.12  90/07/23  13:48:34  nadeau
 **	Added HDF support and prepared for PIC support.
 **	
 **	Revision 1.11  90/07/05  11:41:36  todd
 **	add .tif and .TIF to equivalent name table.  Changed tif magic number
 **	to 4d4d (MBF).  4f4f is LBF.
 **	
 **	Revision 1.10  90/07/02  13:18:15  nadeau
 **	Changed ImTag* to Tag* (new names), added new error handling code, added
 **	flags table code, entered SYNU format, unifdefed TIFF format, and
 **	rearranged the names of all read/write calls from ImReadXXX to
 **	ImXXXRead, etc.
 **	
 **	Revision 1.9  90/06/25  14:32:19  nadeau
 **	Changed ImTag* to Tag* (new names).
 **	
 **	Revision 1.8  90/06/25  14:27:11  ferrerj
 **	Removed #ifdef's from RGB format initialization.
 **	
 **	Revision 1.7  90/06/04  10:42:08  nadeau
 **	Just added some curly braces.
 **	
 **	Revision 1.6  90/05/15  16:29:13  todd
 **	Add imiff.c and imrla.c to tables
 **	
 **	Revision 1.5  90/05/15  13:59:30  nadeau
 **	Removed old format table code between #ifdefs and added more comments.
 **	
 **	Revision 1.4  90/05/11  09:53:29  nadeau
 **	Changed format arg to read/write calls to a character string.  Added
 **	a master file format table with lots of info on the format, including
 **	magic number and equivalent names.  Added format query calls to intuit
 **	a file's format.  Added calls to add the format names to the options
 **	and equivs lists for the arg package.  All additions are #ifdef'ed in.
 **	
 **	Revision 1.3  90/04/09  10:58:56  todd
 **	Added Sun-TAAC IFF format  ImReadIff() and ImWriteIff()
 **	
 **	Revision 1.2  90/03/28  11:15:17  nadeau
 **	Dropped the format set/query functions.  Added format and flags
 **	arguments to the read and write calls.  Added two new variations
 **	of the read and write calls:  fd and fp read.
 **	
 **	Revision 1.1  90/03/06  17:32:14  nadeau
 **	Initial revision
 **	
 **/

//#include <unistd.h>
//#include <sys/wait.h>
#include "im.h"
#include "iminternal.h"
#include "impref.h"

/**
 **  CODE CREDITS
 **	Custom development, Dave Nadeau, San Diego Supercomputer Center, 1990.
 **	Custom development, Brian Duggan, San Diego Supercomputer Center, 1995.
 **/


/*	NOTE
 *
 * 	Note that here and throughout the library, we try to refer to a file
 *	compression 'scheme' and an image 'format'; not a compression
 *	'format', (and not an image 'scheme') in order to keep things clear.
 */


/*
 * DEFINES
 *	IMREADMAGIC
 *	IMREADFMAGIC
 *	IMMAXMAGIC
 *
 * DESCRIPTION
 *	These macros are used by input routines in 
 * imfile.c and imcompress.c to query the magic
 * number of a file.
 */

#define IMMAXMAGIC	100		/* Max size magic number	*/

#define IMREADMAGIC(fd,buf,size,at)					\
{									\
	long location;			/* Saved file location		*/\
	location = lseek( (fd), 0, L_INCR );				\
	lseek( (fd), (at), L_SET );					\
	BinRead( (fd), (buf), UCHAR, 1, (size) );			\
	lseek( (fd), location, L_SET );					\
}
#define IMREADFMAGIC(fp,buf,size,at)					\
{									\
	long location;			/* Saved file location		*/\
	location = fseek( (fp), 0, F_INCR );				\
	fseek( (fp), (at), F_SET );					\
	BinFRead( (fp), (buf), UCHAR, 1, (size) );			\
	fseek( (fp), location, F_SET );					\
}



/*
 *  FUNCTION DECLARATIONS
 */
#ifdef __STDC__
/* extern char *mktemp( char * ); */

static char          *imFileQFormat( int, int, FILE *, char * );
static void           imFileErrorTags( TagTable * );
static void           imFileInfoTags( TagTable * );
static void           imFileImageTags( TagTable *, int *, int *, int *, int *,
			int *, int *, int *, int *, int *, int * );
static int            imFileRead( int, int, FILE *, char *, TagTable *,
			TagTable * );
static ImFileFormatWriteMap *imFileSelectMap( ImVfb *, ImFileFormat **, int,
			int, int, int, int, int, int, int );
static int            imFileWrite( int, int, FILE *, char *, TagTable *,
			TagTable * );
static int            imFileWriteSingle( int, int, FILE *, ImFileFormat **,
			TagTable *, TagTable * );
static int            imFileWriteMultiple( int, int, FILE *, ImFileFormat **,
			TagTable *, TagTable * );
static int            imFileWriteIt( ImFileFormatWriteMap *, int, int, FILE *,
			ImFileFormat **, TagTable *, TagTable * );
static int 	 	imFileReadCompressedFile(int , int , FILE* , TagTable* , TagTable* , char* );
static int 		imFileWriteCompressedFile(int, int, FILE*, TagTable*, TagTable*, 
			 ImFileFormatWriteMap *, char* );
static char *		imFileQCompression(int ioType, int fd, FILE* fp, TagTable* flagsTable);
#else
extern char *mktemp( );

static char          *imFileQFormat( );
static void           imFileErrorTags( );
static void           imFileInfoTags( );
static void           imFileImageTags( );
static int            imFileRead( );
static ImFileFormatWriteMap *imFileSelectMap( );
static int            imFileWrite( );
static int            imFileWriteSingle( );
static int            imFileWriteMultiple( );
static int 	 	imFileReadCompressedFile();
static int 		imFileWriteCompressedFile();
static char *		imFileQCompression();
#endif





/*
 *  FUNCTION
 *	imMakeTempFile()
 *
 *  DESCRIPTION
 *	Because some files use byte offsets that point hither and yon
 *	throughout the file, we can't always handle an input or output
 *	stream "on the fly".
 *
 *	For reads we copy the input stream to a file, then read it.
 *	For writes we write to a file, then copy it to the output stream.
 *
 *	imMakeTempFile() generates a char* which should be used as the
 *	a temporary file.
 */

#ifdef __STDC__
static char* imMakeTempFile(void);
#else
static char* imMakeTempFile();
#endif






/*
 *  MACRO
 *	IOTYPE	-  figure the IoType for an unbuffered stream
 *	FIOTYPE	-  figure the IoType for a buffered stream
 *
 *  DESCRIPTION
 *	These macros determine whether the stream corresponds to a tty,
 *	pipe, or file.  Tty's and pipes must be handled specially (without
 *	seeking) by the lower level read and write code.
 *
 *	The following cases must be recognized (using input as an example):
 *
 *	Command line		Meaning			Classification
 *	tool file		read from file		file
 *	tool < file		redirect from file	file
 *	tool < /dev/tty		read from tty		pipe
 *	tool			read from tty		pipe
 *	cat file | tool		read from pipe		pipe
 *
 *	lseek() or fseek() on a pipe will fail.  Seeking on a tty is supposed
 *	to fail, but doesn't on some versions of UNIX.
 *
 *	isatty() reports whether an fd is a tty device.
 */

#define IOTYPE(fd)							\
	((((lseek( (fd), 0, L_INCR )==-1) && (errno==ESPIPE))||isatty( (fd) ))?\
		(IMFILEIOFD | IMFILEIOPIPE)				\
	:								\
		(IMFILEIOFD | IMFILEIOFILE)				\
	)

#define FIOTYPE(fp)							\
	(((fseek( (fp), 0, F_INCR )==-1)||isatty( fileno(fp) ))?	\
		(IMFILEIOFP | IMFILEIOPIPE)				\
	:								\
		(IMFILEIOFP | IMFILEIOFILE)				\
	)





/*
 *  FUNCTION
 *	ImFileFindFormat	-  find the format info given a format name
 *
 *  DESCRIPTION
 *	The format's name is converted to lower case, then looked up in the
 *	format table.  A pointer to the found format table entry is returned.
 */

ImFileFormat **			/* Returns ptr to file format info*/
#ifdef __STDC__
ImFileFindFormat( char *format )
#else
ImFileFindFormat( format )
	char *format;			/* Format name to look up	*/
#endif
{
	ImFileFormat **pFormat;		/* Pointer into format table	*/
	char         **pName;		/* Name list pointer		*/
	char           lcFormat[IMMAXNAME];/* Lower case name		*/
	char          *s;		/* String pointer		*/

	if (format==NULL || *format==NULL)
	{
		ImErrNo = IMEFORMATUNKNOWN;
		return NULL;
	}

	/* Convert to lower case.					*/
	for ( s = lcFormat; *format; s++, format++ )
	{
		if ( isupper( *format ) )
			*s = tolower( *format );
		else
			*s = *format;
	}
	*s = '\0';

	/* Scan the format table.					*/
	for ( pFormat = ImGetFileFormats(); *pFormat; pFormat++ )
	{
		for ( pName = (*pFormat)->format_names; *pName; pName++ )
			if ( strcmp( *pName, lcFormat ) == 0 )
				break;
		if ( *pName )
			break;
	}

	if ( ( pFormat== NULL ) ||  (!(*pName)) )
	{
		ImErrNo = IMEFORMATUNKNOWN;
		return ( NULL );
	}
	return ( pFormat );
}





/*
 *  FUNCTION
 *	ImFileQFormat	-  query the format of a file
 *	ImFileQFFormat	-  query the format of a file
 *	imFileQFormat	-  do the actual format search
 *
 *  DESCRIPTION
 *	ImFileQFormat( ) and ImFileQFFormat( ) call the underlying
 *	imFileQFormat( ) to do the actual work, telling it what type of
 *	file is open and how it was opened (descriptor or pointer).
 *
 *	If input isn't a pipe, tty, or write-only file, we check the magic
 *	number against the format list.  If that doesn't work, we check the
 *	file name extension against the format list.  The name of the format,
 *	or NULL is returned.
 */

char *					/* Returns format name		*/
#ifdef __STDC__
ImFileQFormat( int fd, char *fileName )
#else
ImFileQFormat( fd, fileName )
	int   fd;			/* File's descriptor		*/
	char *fileName;			/* File's name			*/
#endif
{
	return ( imFileQFormat( /*IOTYPE(fd)*/ 0, fd, NULL, fileName ) );
}


char *					/* Returns format name		*/
#ifdef __STDC__
ImFileQFFormat( FILE *fp, char *fileName )
#else
ImFileQFFormat( fp, fileName )
	FILE *fp;			/* File's pointer		*/
	char *fileName;			/* File's name			*/
#endif
{
	return ( imFileQFormat( /*FIOTYPE(fp)*/ 0, -1, fp, fileName ) );
}


static char *				/* Returns format name		*/
#ifdef __STDC__
imFileQFormat( int ioType, int fd, FILE *fp, char *fileName )
#else
imFileQFormat( ioType, fd, fp, fileName )
	int   ioType;			/* File descriptor or pointer	*/
	int   fd;			/* File's descriptor		*/
	FILE *fp;			/* File's pointer		*/
	char *fileName;			/* File's name			*/
#endif
{
	ImFileFormat **pFmt;		/* Format list pointer		*/
	int            length;		/* Length of magic #		*/
	unsigned char  magic1[IMMAXMAGIC];/* Test magic #		*/
	unsigned char  magic2[IMMAXMAGIC];/* Another test magic #	*/
	unsigned char *magic;		/* Current magic #		*/
	unsigned char *fmtMagic;	/* Current format's magic #	*/
	int            i;		/* Counter			*/
	int            doMagic = TRUE;	/* Do we check magic numbers?	*/
	char          *extension;	/* File name extension		*/
	int            ioOperation;	/* Read or write?		*/
	ImFileMagic   *pMagic;		/* pointer to magic file info	*/
	char	       tmpExtension[128];/* Holds the extension        */
	char	      *tmp;		/* points to a char*		*/

	/*
	 *  Should we check the magic number?
	 *	1.  If file is a pipe or a tty, NO.
	 *	2.  If file is open for writing only, NO.
	 *	3.  Otherwise, YES.
	 */
/*	if ( ioType & IMFILEIOPIPE )
		doMagic = FALSE;	// Input is a pipe or a tty	
	else
	{
		if ( ioType & IMFILEIOFD )
			ioOperation = fcntl( fd, F_GETFL, &ioOperation );
		else
			ioOperation = fcntl( fileno(fp), F_GETFL, &ioOperation);
		if ( ioOperation == -1 )
		{
			ImErrNo = IMESYS;	// Bad fp/fd?		
			return ( NULL );
		}
		if ( ioOperation == O_WRONLY )
			doMagic = FALSE;
	}
*/

	/*
	 *  Get the magic number from the file and check it against each of
	 *  the file format list.
	 */
	if ( doMagic )
	{
		/*
		 *  Read in the magic number at location 0 (where most are).
		 */
		if ( ioType & IMFILEIOFD )
		{
			IMREADMAGIC( fd, magic1, IMMAXMAGIC, 0 );
		}
		else
		{
			IMREADFMAGIC( fp, magic1, IMMAXMAGIC, 0 );
		}


		/*
		 *  Check it against the format list.  If a format doesn't
		 *  have a magic number, skip it.  If it has a magic number,
		 *  but not at byte 0 in the file, then read in and test
		 *  with that magic.  Otherwise test with the magic numbers
		 *  read in above.
		 */
		magic = magic1;
		for ( pFmt = ImGetFileFormats(); *pFmt; pFmt++ )
		{
			pMagic = (*pFmt)->format_magicMap;
			if ( pMagic == NULL )
			{
				/* This format has no magic numbers.	*/
				continue;
			}
			for ( ; pMagic->format_magicNumber != NULL; pMagic++ )
			{
				if ( pMagic->format_magicLength == 0 )
					continue;	/* No magic #	*/
				if ( pMagic->format_magicLocation != 0 )
				{
					if ( ioType & IMFILEIOFD )
					{
						IMREADMAGIC( fd, magic2,
							pMagic->format_magicLength,
							pMagic->format_magicLocation );
					}
					else
					{
						IMREADFMAGIC( fp, magic2,
							pMagic->format_magicLength,
							pMagic->format_magicLocation );
					}
					magic = magic2;
				}
				length   = pMagic->format_magicLength;
				fmtMagic = pMagic->format_magicNumber;

				/* See if it matches the magic number	*/
				for ( i = 0; i < length; i++ )
				       if ( fmtMagic[i] != magic[i] )
						break;
				if ( i == length )
					return ( (*pFmt)->format_names[0] );
				magic = magic1;
			}
		}
		/* No match.  Fall through to the file name check.	*/
	}


	/*
	 *  Get the file name extension, then check it against the format list.
	 */
	extension = &fileName[strlen(fileName)-1];
	while ( extension != fileName && *extension != '.' )
		extension--;
	if ( *extension != '.' )
	{
		ImErrNo = IMEFORMATUNKNOWN;
		return ( NULL );
	}
	strcpy (tmpExtension, extension+1);

	pFmt = ImFileFindFormat( tmpExtension );
	if ( pFmt != NULL )
		return ( (*pFmt)->format_names[0] );

	/*
	 * There may be a compression suffix on the file name.
	 * Search back to one more '.'.
	 */
	 extension--;
	 while ( extension != fileName && *extension != '.' )
		extension--;
	 if ( *extension != '.' )
	 {
		ImErrNo = IMEFORMATUNKNOWN;
		return ( NULL );
	 }
	 strcpy(tmpExtension, extension+1);

	 /* Take the compression suffix off */
	 tmp = strchr(tmpExtension,'.');
	 *tmp = '\0';

 	 pFmt = ImFileFindFormat( tmpExtension );

	 if (pFmt==NULL)
	 {
		ImErrNo = IMEFORMATUNKNOWN;
		return ( NULL );
	 }

	 return ( (*pFmt)->format_names[0] );
}





/*
 *  FUNCTION
 *	ImFileQNFormat	-  query # of formats
 *
 *  DESCRIPTION
 *	On the first call, the number of formats in the format table is
 *	counted and saved away.  On all subsequent calls, this saved
 *	count is returned.
 */

int					/* Retursn # of formats		*/
#ifdef __STDC__
ImFileQNFormat( void )
#else
ImFileQNFormat( )
#endif
{
	ImFileFormat **pFmt;		/* File format list pointer	*/
	static int count = -1;		/* Saved format count		*/

	if ( count != -1 )
		return ( count );
	for ( count = 0, pFmt = ImGetFileFormats(); *pFmt; count++, pFmt++ )
		;
	return ( count );
}



/*
 *  FUNCTION
 *	imFileErrorTags	-  process error tags
 *	imFileImageTags	-  process image tags
 *	imFileInfoTags	-  process info tags
 *
 *  DESCRIPTION
 *	imFileErrorTags looks for the generic error handling tags in the
 *	flagsTable and sets globals accordingly.
 *
 *	imFileImageTags looks for the image write handling tags in the
 *	flagsTable and sets arguments accordingly.
 *
 *	imFileInfoTags looks for the generic info handling tags in the
 *	flagsTable and sets globals accordingly.
 */

static void				/* Returns nothing		*/
#ifdef __STDC__
imFileErrorTags( TagTable *flagsTable )
#else
imFileErrorTags( flagsTable )
	TagTable   *flagsTable;		/* Flags			*/
#endif
{
	TagEntry   *tmpEntry;		/* Tmp table entry holder	*/

	ImErrorProgramName  = "program";
	ImErrorFileName     = "stream";
	ImErrorHandler      = NULL;
	ImErrorStream       = NULL;

	if ( flagsTable == TAGTABLENULL )
		return;

	tmpEntry = TagTableQDirect( flagsTable, "program name", 0 );
	if ( tmpEntry != TAGENTRYNULL )
		TagEntryQValue( tmpEntry, &ImErrorProgramName );
	tmpEntry = TagTableQDirect( flagsTable, "file name", 0 );
	if ( tmpEntry != TAGENTRYNULL )
		TagEntryQValue( tmpEntry, &ImErrorFileName );
	tmpEntry = TagTableQDirect( flagsTable, "error handler", 0 );
	if ( tmpEntry != TAGENTRYNULL )
		TagEntryQValue( tmpEntry, &ImErrorHandler );
	tmpEntry = TagTableQDirect( flagsTable, "error stream", 0 );
	if ( tmpEntry != TAGENTRYNULL )
		TagEntryQValue( tmpEntry, &ImErrorStream );
}

static void				/* Returns nothing		*/
#ifdef __STDC__
imFileImageTags( TagTable *flagsTable, int *monoThreshold, int *typeRequest,
	int *channelRequest, int *depthRequest, int *interRequest,
	int *groupRequest, int * qualityRequest, int *compRequest, 
	int *cltRequest, int *alphaRequest )
#else
imFileImageTags( flagsTable, monoThreshold, typeRequest, channelRequest,
	depthRequest, interRequest, groupRequest, qualityRequest, compRequest, 
	cltRequest, alphaRequest )
	TagTable   *flagsTable;		/* User flags			*/
	int	   *monoThreshold;	/* Mono conversion threshold	*/
	int	   *typeRequest;	/* Image type request		*/
	int	   *channelRequest;	/* # of channels requested	*/
	int	   *depthRequest;	/* Depth request		*/
	int	   *interRequest;	/* Interleave request		*/
	int	   *groupRequest;	/* Group request		*/
	int	   *qualityRequest;	/* Quality request		*/
	int	   *compRequest;	/* Compression request		*/
	int	   *cltRequest;		/* CLT request			*/
	int	   *alphaRequest;	/* Alpha request		*/
#endif
{
	TagEntry   *tmpEntry;		/* Table entry holder		*/

	*monoThreshold  = IMDEFMONOTHRESH;
	*typeRequest    = -1;
	*channelRequest = -1;
	*depthRequest   = -1;
	*interRequest   = -1;
	*compRequest	= -1;
	*cltRequest	= -1;
	*alphaRequest	= -1;
	*groupRequest   = -1;
	*qualityRequest = -1;

	if ( flagsTable == TAGTABLENULL )
		return;

	tmpEntry = TagTableQDirect( flagsTable, "image mono threshold", 0 );
	if ( tmpEntry != TAGENTRYNULL )
		TagEntryQValue( tmpEntry, monoThreshold );

	tmpEntry = TagTableQDirect( flagsTable, "image interleave request", 0 );
	if ( tmpEntry != TAGENTRYNULL )
		TagEntryQValue( tmpEntry, interRequest );

	tmpEntry = TagTableQDirect( flagsTable, "image type request", 0 );
	if ( tmpEntry != TAGENTRYNULL )
		TagEntryQValue( tmpEntry, typeRequest );

	tmpEntry = TagTableQDirect( flagsTable, "image channel number request", 0 );
	if ( tmpEntry != TAGENTRYNULL )
		TagEntryQValue( tmpEntry, channelRequest );

	tmpEntry = TagTableQDirect( flagsTable, "image channel depth request", 0 );
	if ( tmpEntry != TAGENTRYNULL )
		TagEntryQValue( tmpEntry, depthRequest );

	tmpEntry = TagTableQDirect( flagsTable, "image compression request", 0);
	if ( tmpEntry != TAGENTRYNULL )
		TagEntryQValue( tmpEntry, compRequest );

	tmpEntry = TagTableQDirect( flagsTable, "image clt request", 0 );
	if ( tmpEntry != TAGENTRYNULL )
		TagEntryQValue( tmpEntry, cltRequest );

	tmpEntry = TagTableQDirect( flagsTable, "image alpha request", 0 );
	if ( tmpEntry != TAGENTRYNULL )
		TagEntryQValue( tmpEntry, alphaRequest );

	tmpEntry = TagTableQDirect( flagsTable, "image group request", 0 );
	if ( tmpEntry != TAGENTRYNULL )
		TagEntryQValue( tmpEntry, groupRequest );

	tmpEntry = TagTableQDirect( flagsTable, "image compression quality request", 0 );
	if ( tmpEntry != TAGENTRYNULL )
	{
		/* We don't care about the value; we just care that the
		   request is there. */
		*qualityRequest = IMQUALITYYES;
	}
}

static void				/* Returns nothing		*/
#ifdef __STDC__
imFileInfoTags( TagTable *flagsTable )
#else
imFileInfoTags( flagsTable )
	TagTable   *flagsTable;		/* Flags			*/
#endif
{
	TagEntry   *tmpEntry;		/* Tmp table entry holder	*/

	ImInfoHandler = NULL;
	ImInfoStream  = NULL;

	if ( flagsTable == TAGTABLENULL )
		return;

	tmpEntry = TagTableQDirect( flagsTable, "info handler", 0 );
	if ( tmpEntry != TAGENTRYNULL )
		TagEntryQValue( tmpEntry, &ImInfoHandler );
	tmpEntry = TagTableQDirect( flagsTable, "info stream", 0 );
	if ( tmpEntry != TAGENTRYNULL )
		TagEntryQValue( tmpEntry, &ImInfoStream );
}





/*
 *  FUNCTION
 *	ImFileRead	-  read an image file from a file descriptor
 *	ImFileFRead	-  read an image file from a file pointer
 *	imFileRead	-  do the real work of read
 *
 *  DESCRIPTION
 *	ImFileRead and ImFileFRead just call imFileRead to do their work.
 *
 *	Incomming arguments are checked and the flagsTable scanned for
 *	relevant flags.  The format table is searched for the format's
 *	read function, and that function called.
 */

int					/* Returns number of entries added*/
#ifdef __STDC__
ImFileRead( int fd, char *format, TagTable *flagsTable, TagTable *tagTable )
#else
ImFileRead( fd, format, flagsTable, tagTable )
	int         fd;			/* File descriptor to read from	*/
	char       *format;		/* File format to use		*/
	TagTable   *flagsTable;		/* Flags			*/
	TagTable   *tagTable;		/* Tag table to append to	*/
#endif
{
	return ( imFileRead( /*IOTYPE(fd)*/ 0, fd, NULL, format,
		flagsTable, tagTable ) );
}

int					/* Returns number of entries added*/
#ifdef __STDC__
ImFileFRead( FILE *fp, char *format, TagTable *flagsTable, TagTable *tagTable )
#else
ImFileFRead( fp, format, flagsTable, tagTable )
	FILE       *fp;			/* File pointer to read from	*/
	char       *format;		/* File format to use		*/
	TagTable   *flagsTable;		/* Flags			*/
	TagTable   *tagTable;		/* Tag table to append to	*/
#endif
{
	return ( imFileRead( FIOTYPE(fp), -1, fp, format,
		flagsTable, tagTable ) );
}

static int				/* Returns status		*/
#ifdef __STDC__
imFileRead( int ioType, int fd, FILE *fp, char *format, TagTable *flagsTable,
	TagTable *tagTable )
#else
imFileRead( ioType, fd, fp, format, flagsTable, tagTable )
	int         ioType;		/* I/O flags			*/
	int         fd;			/* Input file descriptor	*/
	FILE	   *fp;			/* Input file pointer		*/
	char       *format;		/* File format to use		*/
	TagTable   *flagsTable;		/* Flags			*/
	TagTable   *tagTable;		/* Tag table to append to	*/
#endif
{
	ImFileFormat **pFormat;		/* Pointer into format table	*/
	FILE       *tmpFp;		/* Temporary fp			*/
	int         tmpFd;		/* Temporary fd			*/
	char        tmpName[1024];	/* Temporary file name		*/
	unsigned char buffer[1024];	/* Temporary write buffer	*/
	char        message[1024];	/* Holds iminfo message 	*/
	int         status;		/* Return status		*/
	int	    n;			/* # of bytes read		*/
	int	    binByteOrder;	/* Current byte order		*/
	int	    binFloatFormat;	/* Current floating point format*/
	TagEntry   *tmpEntry;		/* Tmp table entry holder	*/
	char       *fileName;		/* name of file			*/
	char	   *tmp;		/* temp string			*/
	ImCompressScheme* pCompress;	/* compression scheme           */


	/*
	 *  Get and save the current state of the Binary I/O package.
	 */
	binByteOrder   = BinQByteOrder( );
	binFloatFormat = BinQFloatFormat( );


	/*
	 *  Get stuff from the flags table.
	 */
	imFileErrorTags( flagsTable );
	imFileInfoTags( flagsTable );

	/* Check tagTable */
	if ( tagTable == TAGTABLENULL )
	{
                ImErrNo = IMENULLTAGTABLE;
                ImErrorFatal( ImQError( ), -1, ImErrNo );
	}

	/*
	 * Check for compression.
	 */
	if ((tmp = imFileQCompression(ioType, fd, fp, flagsTable))!=NULL)
	{
		pCompress = ImFileFindCompressionScheme(tmp);
		sprintf(message,"'%s' (%s)",pCompress->compress_suffixes[0],pCompress->compress_name);
		ImInfo("File Compression",message);
		if (imFileReadCompressedFile(ioType, fd, fp, flagsTable, tagTable, tmp)==-1)
			return -1;
	}
	else
	{
		/*
		 * Read an uncompressed file
		 */

		/*
		 * If no format was specifed, get the format from the
		 * file name, or flags in the flags table. 
		 */
		 if (format==NULL || *format=='\0')
		 {
			tmpEntry = TagTableQDirect( flagsTable, "file name", 0 );
			if ( tmpEntry != TAGENTRYNULL )
			{
				TagEntryQValue( tmpEntry, &fileName);
			}
			else
				fileName = NULL;
			if (ioType & IMFILEIOFD)
				format = ImFileQFormat(fd, fileName);
			else
				format = ImFileQFFormat(fp, fileName);
		 }

	 	/*
		 *  Look up the format 
		 */
		if ( (pFormat = ImFileFindFormat( format )) == NULL )
		{
			ImErrorFatal( ImQError( ), -1, ImErrNo );
		}

		if ( (*pFormat)->format_read == NULL )
		{
			ImErrNo = IMENOREAD;
			ImErrorFatal( ImQError( ), -1, ImErrNo );
		}

		sprintf(message,"'%s' (%s)",(*pFormat)->format_names[0],(*pFormat)->format_help);
		ImInfo("Image Format",message);

		/*
		 *  If the fd/fp is a file, or it's a pipe and the handler can read
		 *  directly to a pipe, read it.
		 */
		if ( ioType & IMFILEIOFILE || (*pFormat)->format_readPipe == IMPIPE )
		{
			status = (*(*pFormat)->format_read)( ioType, fd, fp,
				flagsTable, tagTable );
		}
		else
		{
			/*
			 *  Create a temporary file, copy input to the temp file, then do
			 *  all reading from it.  When complete, delete the file, then
			 *  return.  If errors occur, save them until the unlink has been done.
			 */
			strcpy( tmpName, imMakeTempFile() );
			if ( ioType & IMFILEIOFD )
			{
				if ( (tmpFd = open( tmpName, O_RDWR|O_CREAT, 0666 )) == -1 )
				{
					ImErrNo = IMESYS;
					ImErrorFatal( ImQError( ), -1, ImErrNo );
				}
				while ( (n = read( fd, buffer, sizeof( buffer ) )) > 0 )
					write( tmpFd, buffer, n );
				lseek( tmpFd, 0, 0 );
				status = (*(*pFormat)->format_read)( IMFILEIOFD | IMFILEIOFILE,
					tmpFd, NULL, flagsTable, tagTable );
				close( tmpFd );
				unlink( tmpName );
			}
			else
			{
				if ( (tmpFp = fopen( tmpName, "w+b" )) == NULL )
				{
					ImErrNo = IMESYS;
					ImErrorFatal( ImQError( ), -1, ImErrNo );
				}
				while ( (n = fread( buffer, 1, sizeof( buffer ), fp )) != 0 )
					fwrite( buffer, 1, n, tmpFp );
				fseek( tmpFp, 0, 0 );
				status = (*(*pFormat)->format_read)( IMFILEIOFP | IMFILEIOFILE, -1,
					tmpFp, flagsTable, tagTable );
				fclose( tmpFp );
				unlink( tmpName );
			}
		}
	}
	/*
	 *  Restore the Binary I/O library state.
	 */
	BinByteOrder( binByteOrder );
	BinFloatFormat( binFloatFormat );

	/*
	 * Process input requests
	 */
	if (  flagsTable && TagTableQDirect(flagsTable, "channel map request", 0) != TAGENTRYNULL )
	{
		if (ImVfbProcessMapRequests( flagsTable, tagTable )<=0)
			ImErrorFatal( ImQError( ), -1, ImErrNo );
	}

	return ( status );
}





/*
 *  FUNCTION
 *	imFileSelectMap	-  select a good write map to use
 *
 *  DESCRIPTION
 *	The format's write map is scanned to find the best match for output
 *	of the given VFB based upon the user's requests.
 */

static ImFileFormatWriteMap *		/* Returns write map entry	*/
#ifdef __STDC__
imFileSelectMap( ImVfb *pVfb, ImFileFormat **pFormat, int typeRequest,
	int channelRequest, int depthRequest, int interRequest,
	int groupRequest, int qualityRequest, int otherRequest, int otherRequestMask )
#else
imFileSelectMap( pVfb, pFormat, typeRequest, channelRequest, depthRequest,
	interRequest, groupRequest, qualityRequest, otherRequest, otherRequestMask )
	ImVfb	   *pVfb;		/* Image to handle		*/
	ImFileFormat **pFormat;		/* Format conversion info	*/
	int	    typeRequest;	/* Type request			*/
	int	    channelRequest;	/* Channel request		*/
	int	    depthRequest;	/* Depth request		*/
	int	    interRequest;	/* Interleave request		*/
	int	    groupRequest;	/* Group request                */
	int	    qualityRequest;	/* Quality request              */
	int	    otherRequest;	/* CLT/Alpha/Comp requests	*/
	int	    otherRequestMask;	/* Which of those are in otherRequest*/
#endif
{
	ImFileFormatWriteMap *pMap;	/* Write map entry		*/
	int	    fields;		/* VFB field mask		*/
	imFilePrefTable *pPrefTable;	/* Preference table		*/
	imFilePref *pPrefSubTable;	/* Preference subtable		*/
	imFilePref *pPref;		/* Preference table entry ptr	*/
	int	    bestIndex;		/* Best preference index	*/
	ImFileFormatWriteMap *pBest;	/* Best write map entry		*/
	int         i;			/* Counters			*/


	/*
	 *  To gauge which of possibly many output image types (and their
	 *  CLT, alpha, interleave, grouping, and compression attributes) is 
	 *  "best" for the incomming VFB we use a series of "preference tables".
	 *  There is one master table for each VFB type (mono, index8,
	 *  index16, and rgb).  Within each table are four sub-tables, for
	 *  each combination of (have/don't have CLT) and (have/don't have
	 *  alpha planes).
	 *
	 *  Within each sub-table are 16 entries for every combination of
	 *  (mono, index8, index16, rgb), (have/don't have CLT), and
	 *  (have/don't have alpha).  These are ordered from most preferable
	 *  to least preferable for this VFB.
	 *
	 *  In evaluating a given format's selection of write map entries,
	 *  we look up each one in the preference table and get the table
	 *  index of it's entry.  The entry with the lowest index, of those
	 *  available to the format, is the one we select.
	 *
	 *  Among several write map entries with the same preference value
	 *  (same incomming VFB and attributes but different channel numbers
	 *  and depths), the earliest one in the write map is chosen.
	 */
	fields = ImVfbQFields( pVfb );
	if ( fields & IMVFBMONO )
		pPrefTable = &imPrefMono;
	else if ( fields & IMVFBINDEX8 )
		pPrefTable = &imPrefIndex8;
	else if ( fields & IMVFBINDEX16 )
		pPrefTable = &imPrefIndex16;
	else if ( fields & IMVFBRGB )
		pPrefTable = &imPrefRgb;
	else
	{
		/* What kind of image is stored here?			*/
		return ( NULL );
	}
	if ( fields & IMVFBALPHA )
	{
		if ( ImVfbQClt( pVfb ) != IMCLTNULL )
			pPrefSubTable = pPrefTable->pref_cltAlpha;
		else
			pPrefSubTable = pPrefTable->pref_noCltAlpha;
	}
	else
	{
		if ( ImVfbQClt( pVfb ) != IMCLTNULL )
			pPrefSubTable = pPrefTable->pref_cltNoAlpha;
		else
			pPrefSubTable = pPrefTable->pref_noCltNoAlpha;
	}

	bestIndex = 1000;
	pBest     = NULL;
	for ( pMap = (*pFormat)->format_writeMap; pMap->map_inField != -1; pMap++ )
	{
		/* Reject if not the type, # channel, channel depth requested.*/
		if ( typeRequest != -1 && pMap->map_outType != typeRequest )
		{
			continue;
		}
		if ( channelRequest != -1 && pMap->map_outNChannels != channelRequest )
		{
			continue;
		}
		if ( depthRequest != -1 && pMap->map_outChannelDepth != depthRequest )
		{
			continue;
		}

		/* Reject if not the CLT/Alpha/Comp requested.		*/
		if ( (pMap->map_outAttributes & otherRequestMask) !=
			otherRequest )
		{
			continue;
		}

		/* Reject if not the interleave requested.		*/
		if ( (interRequest != -1) &&
		     ((pMap->map_outAttributes & IMINTERMASK) != interRequest) )
		{
			continue;
		}

		/* Reject if not the group request                      */
		if ( (groupRequest != -1) &&
		     ((pMap->map_outAttributes & IMGROUPMASK) != groupRequest))
		{
			continue;
		}

		/* Reject if not the quality request                    */
		if ( (qualityRequest != -1) &&
		    ((pMap->map_outAttributes & IMQUALITYMASK) != qualityRequest))
		{
			continue;
		}

		/* Find it's preference index.				*/
		for ( pPref = pPrefSubTable, i = 0; i < 16; i++, pPref++ )
		{
			if ( pMap->map_inField != pPref->pref_field ||
				pMap->map_inAttributes !=pPref->pref_attributes)
				continue;

			/* Match.  Better than last match?		*/
			if ( i < bestIndex )
			{
				bestIndex = i;
				pBest = pMap;
			}
			break;
		}
	}

	if ( pBest == NULL )
	{
		/* Nothing got past user requirements!			*/
		return ( NULL );
	}
	return ( pBest );
}





/*
 *  FUNCTION
 *	ImFileWrite	-  write an image file to a file descriptor
 *	ImFileFWrite	-  write an image file to a file pointer
 *	imFileWrite	-  do the real work of write
 *
 *  DESCRIPTION
 *	ImFileWRite and ImFileFWRite just call imFileWRite to do their work.
 *
 *	Incoming arguments are checked and the flagsTable scanned for
 *	relevant flags.  The format table is searched for the format's
 *	write map.  The write map is scanned to find the best match, or
 *	the match requested by incomming flags.  If the match requires
 *	depth conversion of the VFB(s), it's done.  The match's write
 *	routine is then called.
 *
 *	If NULL is passed as the image file format, then the format is
 *	discerned from this routine.
 *
 *	If the format parameter points to NULL, then the format is discerned
 *	here, AND the format which is ultimately chosen is returned in the format
 *	parameter.
 */

int					/* Returns number of entries used*/
#ifdef __STDC__
ImFileWrite( int fd, char *format, TagTable *flagsTable, TagTable *tagTable )
#else
ImFileWrite( fd, format, flagsTable, tagTable )
	int         fd;			/* Output file descriptor	*/
	char       *format;		/* File format to use		*/
	TagTable   *flagsTable;		/* Flags			*/
	TagTable   *tagTable;		/* Tag table to read from	*/
#endif
{
	return ( imFileWrite( IOTYPE(fd), fd, NULL, format,
		flagsTable, tagTable ) );
}

int					/* Returns number of entries used*/
#ifdef __STDC__
ImFileFWrite( FILE *fp, char *format, TagTable *flagsTable, TagTable *tagTable )
#else
ImFileFWrite( fp, format, flagsTable, tagTable )
	FILE	   *fp;			/* Output file pointer		*/
	char       *format;		/* File format to use		*/
	TagTable   *flagsTable;		/* Flags			*/
	TagTable   *tagTable;		/* Tag table to read from	*/
#endif
{
	return ( imFileWrite( FIOTYPE(fp), -1, fp, format,
		flagsTable, tagTable ) );
}


static int				/* Returns number of entries used*/
#ifdef __STDC__
imFileWrite( int ioType, int fd, FILE *fp, char *format, TagTable *flagsTable,
	TagTable *tagTable )
#else
imFileWrite( ioType, fd, fp, format, flagsTable, tagTable )
	int         ioType;		/* I/O flags			*/
	int         fd;			/* Output file descriptor	*/
	FILE	   *fp;			/* Output file pointer		*/
	char       *format;		/* File format to use		*/
	TagTable   *flagsTable;		/* Flags			*/
	TagTable   *tagTable;		/* Tag table to read from	*/
#endif
{
	int         nVfb;		/* # of vfb's in tag table	*/
	ImFileFormat **pFormat;		/* Pointer into format table	*/
	int	    status;		/* Return status		*/
	int	    binByteOrder;	/* Current byte order		*/
	int	    binFloatFormat;	/* Current floating point format*/
	char*       fileName;		/* name of file (in flagsTable) */
	TagEntry*   tmpEntry;		/* entry in tag table		*/


	/*
	 *  Get and save the current state of the Binary I/O package.
	 */
	binByteOrder   = BinQByteOrder( );
	binFloatFormat = BinQFloatFormat( );

	/* Check integrity of tagTable */
	if ( tagTable == TAGTABLENULL )
	{
		ImErrNo = IMENULLTAGTABLE;
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	}
	nVfb = TagTableQNEntry( tagTable, "image vfb" );
	if ( nVfb < 1 )
	{
		ImErrNo = IMENOVFB;
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	}

	/*
	 *  Get stuff from the flags table.
	 */
	imFileErrorTags( flagsTable );
	imFileInfoTags( flagsTable );

	/*
	 * If no format was specifed, get the format from the
	 * file name, or flags in the flags table. 
	 */
	 if (format==NULL || *format=='\0')
	 {
		tmpEntry = TagTableQDirect( flagsTable, "file name", 0 );
		if ( tmpEntry != TAGENTRYNULL )
		{
			TagEntryQValue( tmpEntry, &fileName);
		}
		else
			fileName = NULL;
		/*
		 * Return the format name in 'format' if
		 * NULL was not passed
		 */
		if (format==NULL)
		{
			if (ioType & IMFILEIOFD)
				format = ImFileQFormat(fd, fileName);
			else
				format = ImFileQFFormat(fp, fileName);
		}
		else
		{
			if (ioType & IMFILEIOFD)
				strcpy(format,ImFileQFormat(fd, fileName));
			else
				strcpy(format, ImFileQFFormat(fp, fileName));
		}
	 }


	/*
	 *  Handle any channel mapping requests
	 */
	if (  flagsTable && TagTableQDirect(flagsTable, "channel map request", 0) 
		!= TAGENTRYNULL )
	{
		if (ImVfbProcessMapRequests( flagsTable, tagTable )<=0)
			ImErrorFatal( ImQError( ), -1, ImErrNo );
	}

	/*
	 *  Find the format 
	 */
	if ( (pFormat = ImFileFindFormat( format )) == NULL )
	{
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	}
	if ( (*pFormat)->format_writeMap == NULL )
	{
		ImErrNo = IMENOWRITE;
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	}

	/*
	 *  Handle the single- and multiple-VFB cases separately.
	 */
	if ( nVfb == 1 )
		status = imFileWriteSingle( ioType, fd, fp,
			pFormat, flagsTable, tagTable );
	else
		status = imFileWriteMultiple( ioType, fd, fp,
			pFormat, flagsTable, tagTable );

	/*
	 *  Restore the Binary I/O library state.
	 */
	BinByteOrder( binByteOrder );
	BinFloatFormat( binFloatFormat );

	return ( status );
}


static int				/* Returns number of entries used*/
#ifdef __STDC__
imFileWriteSingle( int ioType, int fd, FILE *fp, ImFileFormat **pFormat,
	TagTable *flagsTable, TagTable *tagTable )
#else
imFileWriteSingle( ioType, fd, fp, pFormat, flagsTable, tagTable )
	int         ioType;		/* I/O flags			*/
	int         fd;			/* Output file descriptor	*/
	FILE	   *fp;			/* Output file pointer		*/
	ImFileFormat **pFormat;		/* File format to use		*/
	TagTable   *flagsTable;		/* Flags			*/
	TagTable   *tagTable;		/* Tag table to read from	*/
#endif
{
	TagEntry   *tmpEntry;		/* Tmp table entry holder	*/
	TagEntry   *newEntry;		/* New table entry holder	*/
	TagTable   *newTable;		/* New Tag Table		*/

	ImFileFormatWriteMap *pMap;	/* Write map entry		*/

	ImVfb      *origVfb;		/* Original VFB			*/
	int	    origFields;		/* Original VFB field mask	*/
	ImVfb      *newVfb;		/* New VFB			*/
	ImVfb      *statusVfb;		/* Return status as a VFB	*/
	int	    newFields;		/* New VFB field mask		*/
	ImClt	   *newClt;		/* New CLT			*/

	int         status;		/* Return status		*/
	int	    nEntry;		/* Tag entry #			*/

	int	    monoThreshold;	/* Mono conversion threshold	*/
	int	    typeRequest;	/* Type request			*/
	int	    channelRequest;	/* Channel request		*/
	int	    depthRequest;	/* Depth request		*/
	int	    interRequest;	/* Interleave request		*/
	int	    groupRequest;	/* Group request                */
	int	    qualityRequest;	/* Quality request              */
	int	    compRequest;	/* Compression request		*/
	int	    cltRequest;		/* CLT request			*/
	int	    alphaRequest;	/* Alpha request		*/
	int	    otherRequest;	/* CLT/Alpha/Comp requests	*/
	int	    otherRequestMask;	/* Which of those are in otherRequest*/


	/*
	 *  Get stuff from the flags table.
	 */
	imFileImageTags( flagsTable, &monoThreshold, &typeRequest,
		&channelRequest, &depthRequest, &interRequest,
		&groupRequest, &qualityRequest, &compRequest, &cltRequest, 
		&alphaRequest );
	otherRequest     = 0;
	otherRequestMask = 0;
	if ( compRequest != -1 )
	{
		otherRequest     |= (compRequest & IMCOMPMASK);
		otherRequestMask |= IMCOMPMASK;
	}
	if ( cltRequest != -1 )
	{
		otherRequest     |= (cltRequest & IMCLTMASK);
		otherRequestMask |= IMCLTMASK;
	}
	if ( alphaRequest != -1 )
	{
		otherRequest     |= (alphaRequest & IMALPHAMASK);
		otherRequestMask |= IMALPHAMASK;
	}
	if ( groupRequest != -1 )
	{
		otherRequest     |= (groupRequest & IMGROUPMASK);
		otherRequestMask |= IMGROUPMASK;
	}
	if ( qualityRequest != -1 )
	{
		otherRequest     |= (qualityRequest & IMQUALITYMASK);
		otherRequestMask |= IMQUALITYMASK;
	}

	/*
	 *  Get the single VFB in the tag table and determine its type.
	 */
	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &origVfb );
	origFields = ImVfbQFields( origVfb );


	/*
	 *  Get the write map entry that matches this VFB best.
	 */
	pMap = imFileSelectMap( origVfb, pFormat, typeRequest, channelRequest,
		depthRequest, interRequest, groupRequest, qualityRequest, otherRequest, 
		otherRequestMask );
	if ( pMap == NULL )
	{
		/* Can't do it.					*/
		ImErrNo = IMENOTPOSSIBLE;
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	}


	/*
	 *  Avoid making a new VFB and new tag table if the current
	 *  one matches in depth and doesn't require an alpha plane.
	 */
	if ( (origFields & pMap->map_inField) &&
	     !( (pMap->map_inAttributes & IMALPHAYES) &&
	        !(origFields & IMVFBALPHA) ) )
	{
		/*
		 *  Generate a CLT if we need to.
		 */
		if ( (ImVfbQClt( origVfb ) == IMCLTNULL) &&
			(pMap->map_inAttributes & IMCLTYES) )
		{
			/*
			 *  Make a CLT, attach it, write it all out,
			 *  then remove the CLT and throw it away.
			 *
			 *  Note that we always generate a grayscale CLT:
			 *	if Mono, no CLT means grayscale.
			 *	if Index8, no CLT must default to grayscale.
			 *	if Index16, no CLT must default to grayscale.
			 *	if RGB, any CLT is meaningless.  So grayscale.
			 */
			if ( origFields & IMVFBMONO )
				newClt = ImCltGrayRamp( IMCLTNULL, 0, 1, 0,
					 255, IMCLTNEW);
			else if ( origFields & IMVFBINDEX16 )
				newClt = ImCltGrayRamp( IMCLTNULL, 0, 65535,
					 0, 65535, IMCLTNEW);
			else
				newClt = ImCltGrayRamp( IMCLTNULL, 0, 255, 0,
					 255, IMCLTNEW);
			if ( newClt == IMCLTNULL )
			{
				ImErrorFatal( ImQError( ), -1, ImErrNo );
			}
			ImVfbSClt( origVfb, newClt );
			status = imFileWriteIt( pMap,
				ioType, fd, fp, pFormat,
				flagsTable, tagTable );
			ImVfbSClt( origVfb, IMCLTNULL );
			ImCltFree( newClt );
			return ( status );
		}

		/*
		 *  The VFB matches just fine.  Write it out.
		 */
		return ( imFileWriteIt( pMap, ioType, fd, fp, pFormat,
			flagsTable, tagTable ) );
	}


	/*
	 *  Allocate a new VFB.
	 */
	newFields = pMap->map_inField;
	if ( pMap->map_inAttributes & IMALPHAYES )
		newFields |= IMVFBALPHA;

	newVfb = ImVfbAlloc( ImVfbQWidth( origVfb ), ImVfbQHeight( origVfb ),
		newFields );
	if ( newVfb == IMVFBNULL )
	{
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	}

	if ( newFields & IMVFBALPHA )
	{
		if ( origFields & IMVFBALPHA )
		{
			/*
			 *  Copy alpha from original VFB.
			 */
			if ( ImVfbCopy( origVfb, 0, 0, ImVfbQWidth( origVfb ),
				ImVfbQHeight( origVfb ), IMVFBALPHA,
				newVfb, 0, 0 ) == IMVFBNULL )
			{
				ImVfbFree( newVfb );
				ImErrorFatal( ImQError( ), -1, ImErrNo );
			}
		}
		else
		{
			/*
			 *  Generate an empty alpha plane.
			 */
			if ( ImVfbClear( IMVFBALPHA, ~0, newVfb ) == IMVFBNULL )
			{
				ImVfbFree( newVfb );
				ImErrorFatal( ImQError( ), -1, ImErrNo );
			}
		}
	}


	/*
	 *  Copy the original image to the new VFB, changing
	 *  depth if necessary.
	 */
	if ( newFields & IMVFBMONO )
		statusVfb = ImVfbToMono( origVfb, monoThreshold, newVfb );
	else if ( newFields & IMVFBINDEX8 )
		statusVfb = ImVfbToIndex8( origVfb, newVfb );
	else if ( newFields & IMVFBINDEX16 )
		statusVfb = ImVfbToIndex16( origVfb, newVfb );
	else if ( newFields & IMVFBRGB )
		statusVfb = ImVfbToRgb( origVfb, newVfb );
	else
	{
		/* Unknown VFB selection?  How is that possible?	*/
		ImVfbFree( newVfb );
		ImErrNo = IMENOTPOSSIBLE;
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	}
	if ( statusVfb == IMVFBNULL )
	{
		ImVfbFree( newVfb );
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	}


	/*
	 *  Add a CLT if necessary.
	 */
	newClt = ImVfbQClt( newVfb );
	if ( (newClt == IMCLTNULL) && (pMap->map_inAttributes & IMCLTYES) )
	{
		/*
		 *  Note that we always generate a grayscale CLT:
		 *	if Mono, no CLT means grayscale.
		 *	if Index8, no CLT must default to grayscale.
		 *	if Index16, no CLT must default to grayscale.
		 *	if RGB, any CLT is meaningless.  So grayscale.
		 *  Mono gets a 2 entry CLT.  All others 255 entries.
		 */
		if ( newFields & IMVFBMONO )
			newClt = ImCltGrayRamp( IMCLTNULL, 0, 1, 0, 255,
				 IMCLTNEW );
		else if ( newFields & IMVFBINDEX16 )
			newClt = ImCltGrayRamp( IMCLTNULL, 0, 65535, 0, 65535,
				 IMCLTNEW );
		else
			newClt = ImCltGrayRamp( IMCLTNULL, 0, 255,0,255,
				 IMCLTNEW );
		if ( newClt == IMCLTNULL )
		{
			ImVfbFree( newVfb );
			ImErrorFatal( ImQError( ), -1, ImErrNo );
		}
		ImVfbSClt( newVfb, newClt );
	}


	/*
	 *  Copy the tag table, replacing the VFB and CLT, if any.
	 *
	 *  Note:  we can't just temporarly swap the new VFB into the
	 *  original tag table.  Doing so causes the VFB's tag entry to be
	 *  freed and a new one allocated.  If the caller had a pointer to
	 *  that entry, it would become garbage.  So, we are not allowed
	 *  to change, in any way, the incomming tag table.
	 */
	newTable = TagTableAlloc( );
	if ( newTable == TAGTABLENULL )
	{
		ImCltFree( newClt );
		ImVfbFree( newVfb );
		ImErrNo = IMEMALLOC;
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	}
	TagTableCopy( tagTable, newTable, 0, TagTableQNEntry( tagTable, NULL )-1 );

	/* Replace the VFB.						*/
	tmpEntry = TagTableQDirect( newTable, "image vfb", 0 );
	nEntry   = TagEntryQNthEntry( tmpEntry );
	TagTableReplace( newTable, TagEntryQNthEntry( tmpEntry ),
		TagEntryAlloc( "image vfb", POINTER, &newVfb ) );

	/* Replace the CLT (assume there is at most one!).		*/
	tmpEntry = TagTableQDirect( newTable, "image clt", 0 );
	if ( newClt == IMCLTNULL )
	{
		/* New format doesn't want CLT.  Remove it, if any.	*/
		if ( tmpEntry != TAGENTRYNULL )
			TagTableDelete( newTable, TagEntryQNthEntry( tmpEntry));
	}
	else
	{
		/* New format requires CLT.  Replace or insert.		*/
		newEntry = TagEntryAlloc( "image clt", POINTER, &newClt);
		if ( tmpEntry != TAGENTRYNULL )
			TagTableReplace( newTable, TagEntryQNthEntry( tmpEntry),
				newEntry );
		else
			TagTableInsert( newTable, nEntry - 1, newEntry );
	}


	/*
	 *  Write it out, then clean up.
	 */
	status = imFileWriteIt( pMap, ioType, fd, fp, pFormat,
		flagsTable, newTable );

	if ( newClt != IMCLTNULL )
		ImCltFree( newClt );
	ImVfbFree( newVfb );
	TagTableFree( newTable );

	return ( status );
}

static int				/* Returns number of entries used*/
#ifdef __STDC__
imFileWriteMultiple( int ioType, int fd, FILE *fp, ImFileFormat **pFormat,
	TagTable *flagsTable, TagTable *tagTable )
#else
imFileWriteMultiple( ioType, fd, fp, pFormat, flagsTable, tagTable )
	int         ioType;		/* I/O flags			*/
	int         fd;			/* Output file descriptor	*/
	FILE	   *fp;			/* Output file pointer		*/
	ImFileFormat **pFormat;		/* File format to use		*/
	TagTable   *flagsTable;		/* Flags			*/
	TagTable   *tagTable;		/* Tag table to read from	*/
#endif
{
	TagEntry   *tmpEntry;		/* Tmp table entry holder	*/
	TagTable   *newTable;		/* New Tag Table		*/

	ImFileFormatWriteMap *pMap;	/* Write map entry		*/

	ImVfb      *origVfb;		/* Original VFB			*/
	int	    origFields;		/* Original VFB field mask	*/
	ImVfb      *newVfb;		/* New VFB			*/
	int	    newFields;		/* New VFB field mask		*/
	ImClt	   *newClt;		/* New CLT			*/
	ImVfb      *statusVfb;		/* Return status as a VFB	*/

	int         status;		/* Return status		*/
	int	    nEntry;		/* Tag entry #			*/
	int	    i;			/* Entry #			*/
	char	   *tag;		/* Tag name			*/
	int	    error;		/* Error occured?		*/

	int	    monoThreshold;	/* Mono conversion threshold	*/
	int	    typeRequest;	/* Type request			*/
	int	    channelRequest;	/* Channel request		*/
	int	    depthRequest;	/* Depth request		*/
	int	    interRequest;	/* Interleave request		*/
	int	    groupRequest;	/* Group request		*/
	int	    qualityRequest;	/* Quality request		*/
	int	    compRequest;	/* Compression request		*/
	int	    cltRequest;		/* CLT request			*/
	int	    alphaRequest;	/* Alpha request		*/
	int	    otherRequest;	/* CLT/Alpha/Comp requests	*/
	int	    otherRequestMask;	/* Which of those are in otherRequest*/


	/*
	 *  Get stuff from the flags table.
	 */
	imFileImageTags( flagsTable, &monoThreshold, &typeRequest,
		&channelRequest, &depthRequest, &interRequest,
		&groupRequest, & qualityRequest, &compRequest, 
		&cltRequest, &alphaRequest );
	otherRequest     = 0;
	otherRequestMask = 0;
	if ( compRequest != -1 )
	{
		otherRequest     |= (compRequest & IMCOMPMASK);
		otherRequestMask |= IMCOMPMASK;
	}
	if ( cltRequest != -1 )
	{
		otherRequest     |= (cltRequest & IMCLTMASK);
		otherRequestMask |= IMCLTMASK;
	}
	if ( alphaRequest != -1 )
	{
		otherRequest     |= (alphaRequest & IMALPHAMASK);
		otherRequestMask |= IMALPHAMASK;
	}
	if ( groupRequest != -1 )
	{
		otherRequest     |= (groupRequest & IMGROUPMASK);
		otherRequestMask |= IMGROUPMASK;
	}
	if ( qualityRequest != -1 )
	{
		otherRequest     |= (qualityRequest & IMQUALITYMASK);
		otherRequestMask |= IMQUALITYMASK;
	}


	/*
	 *  Allocate a new tag table.
	 */
	newTable = TagTableAlloc( );


	/*
	 *  Walk through the original tag table, watching for VFB's and CLT's.	
	 *  Everything else is copied straight across.
	 */
	nEntry = TagTableQNEntry( tagTable, NULL );
	error  = FALSE;
	for ( i = 0; i < nEntry; i++ )
	{
		tmpEntry = TagTableQLinear( tagTable, i );
		tag = TagEntryQTag( tmpEntry );

		if ( strcmp( tag, "image clt" ) == 0 )
			continue;	/* Remove these for now		*/

		if ( strcmp( tag, "image vfb" ) != 0 )
		{
			/* Some other tag.  Copy to new table.		*/
			TagTableCopy( tagTable, newTable, i, i );
			continue;
		}

		/*
		 *  At this point we have a VFB from the original tag table.
		 */
		TagEntryQValue( tmpEntry, &origVfb );
		origFields = ImVfbQFields( origVfb );


		/*
		 *  Get the write map entry that matches this VFB best.
		 */
		pMap = imFileSelectMap( origVfb, pFormat, typeRequest,
			channelRequest, depthRequest, interRequest,
			groupRequest, qualityRequest, otherRequest, otherRequestMask );
		if ( pMap == NULL )
		{
			/* Can't do it.					*/
			ImErrNo = IMENOTPOSSIBLE;
			error   = TRUE;
			break;
		}


		/*
		 *  Allocate a new VFB.
		 */
		newFields = pMap->map_inField;
		if ( pMap->map_inAttributes & IMALPHAYES )
			newFields |= IMVFBALPHA;

		newVfb = ImVfbAlloc( ImVfbQWidth( origVfb ),
			ImVfbQHeight( origVfb ), newFields );
		if ( newVfb == IMVFBNULL )
		{
			error = TRUE;
			break;
		}

		if ( newFields & IMVFBALPHA )
		{
			if ( origFields & IMVFBALPHA )
			{
				/*
				 *  Copy alpha from original VFB.
				 */
				if ( ImVfbCopy( origVfb, 0, 0,
					ImVfbQWidth( origVfb ),
					ImVfbQHeight( origVfb ), IMVFBALPHA,
					newVfb, 0, 0 ) == IMVFBNULL )
				{
					ImVfbFree( newVfb );
					ImErrorFatal( ImQError( ), -1, ImErrNo);
				}
			}
			else
			{
				/*
				 *  Generate an empty alpha plane.
				 */
				if ( ImVfbClear( IMVFBALPHA, ~0, newVfb ) == IMVFBNULL )
				{
					ImVfbFree( newVfb );
					ImErrorFatal( ImQError( ), -1, ImErrNo);
				}
			}
		}


		/*
		 *  Copy the original image to the new VFB, changing
		 *  depth if necessary.
		 */
		if ( newFields & IMVFBMONO )
			statusVfb = ImVfbToMono( origVfb, monoThreshold,newVfb);
		else if ( newFields & IMVFBINDEX8 )
			statusVfb = ImVfbToIndex8( origVfb, newVfb );
		else if ( newFields & IMVFBINDEX16 )
			statusVfb = ImVfbToIndex16( origVfb, newVfb );
		else if ( newFields & IMVFBRGB )
			statusVfb = ImVfbToRgb( origVfb, newVfb );
		else
		{
			/* Unknown VFB selection?  How is that possible?*/
			ImErrNo = IMENOTPOSSIBLE;
			error = TRUE;
			break;
		}
		if ( statusVfb == IMVFBNULL )
		{
			error = TRUE;
			break;
		}


		/*
		 *  Add a CLT if necessary.
		 */
		newClt = ImVfbQClt( newVfb );
		if ( (newClt == IMCLTNULL) && (pMap->map_inAttributes & IMCLTYES) )
		{
			/*
			 *  Note that we always generate a grayscale CLT:
			 *	if Mono, no CLT means grayscale.
			 *	if Index8, no CLT must default to grayscale.
			 *	if Index16, no CLT must default to grayscale.
			 *	if RGB, any CLT is meaningless.  So grayscale.
			 *  Mono gets a 2 entry CLT.  All others 255 entries.
			 */
			if ( newFields & IMVFBMONO )
				newClt = ImCltGrayRamp( IMCLTNULL, 0, 1, 0, 255,
					 IMCLTNEW );
			else if ( newFields & IMVFBINDEX16 )
				newClt = ImCltGrayRamp( IMCLTNULL, 0, 65535, 0,
					 65535, IMCLTNEW);
			else
				newClt = ImCltGrayRamp( IMCLTNULL, 0, 255,0,
					 255, IMCLTNEW);
			if ( newClt == IMCLTNULL )
			{
				error = TRUE;
				break;
			}
			ImVfbSClt( newVfb, newClt );
		}


		/*
		 *  Add the new CLT & VFB to the tag table and continue.
		 */
		if ( newClt != IMCLTNULL )
			TagTableAppend( newTable,
				TagEntryAlloc( "image clt", POINTER, &newClt ));
		TagTableAppend( newTable,
			TagEntryAlloc( "image vfb", POINTER, &newVfb ) );
	}


	/*
	 *  Write out the new tag table.
	 */
	if ( !error )
		status = imFileWriteIt( pMap, ioType, fd, fp, pFormat,
			flagsTable, newTable );


	/*
	 *  Scan the new table for all the CLT's and VFB's we created
	 *  and blow them away.
	 */
	nEntry = TagTableQNEntry( newTable, "image clt" );
	for ( i = 0; i < nEntry; i++ )
	{
		tmpEntry = TagTableQDirect( newTable, "image clt", i );
		TagEntryQValue( tmpEntry, &newClt );
		ImCltFree( newClt );
	}
	nEntry = TagTableQNEntry( newTable, "image vfb" );
	for ( i = 0; i < nEntry; i++ )
	{
		tmpEntry = TagTableQDirect( newTable, "image vfb", i );
		TagEntryQValue( tmpEntry, &newVfb );
		ImVfbFree( newVfb );
	}
	TagTableFree( newTable );

	if ( error )
	{
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	}
	return ( status );
}


static int				/* Returns number of entries used*/
#ifdef __STDC__
imFileWriteIt( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,
	ImFileFormat **pFormat, TagTable *flagsTable, TagTable *tagTable )
#else
imFileWriteIt( pMap, ioType, fd, fp, pFormat, flagsTable, tagTable )
	ImFileFormatWriteMap *pMap;	/* Write map entry to use	*/
	int         ioType;		/* I/O flags			*/
	int         fd;			/* Output file descriptor	*/
	FILE	   *fp;			/* Output file pointer		*/
	ImFileFormat **pFormat;		/* Format to use		*/
	TagTable   *flagsTable;		/* Flags			*/
	TagTable   *tagTable;		/* Tag table to read from	*/
#endif
{
	FILE       *tmpFp;		/* Temporary fp			*/
	int         tmpFd;		/* Temporary fd			*/
	char        tmpName[1024];	/* Temporary file name		*/
	unsigned char buffer[1024];	/* Temporary write buffer	*/
	int         status;		/* Return status		*/
	int	    n;			/* Number of bytes read		*/
	char        message[1024];	/* Holds a message		*/
	ImCompressScheme* pCompress;	/* compression scheme           */
	char	   *tmp;		/* temp string			*/

	if ((tmp=imFileQCompression(ioType, fd, fp, flagsTable ))!=NULL)
	{
		pCompress = ImFileFindCompressionScheme(tmp);
		sprintf(message,"'%s' (%s)",pCompress->compress_suffixes[0],pCompress->compress_name);
		ImInfo("File Compression",message);
		sprintf(message,"'%s' (%s)",(*pFormat)->format_names[0],(*pFormat)->format_help);
		ImInfo("Image Format",message);
		status = imFileWriteCompressedFile(ioType, fd, fp, flagsTable, tagTable, pMap, tmp);
	}
	else
	{  
		/* Write an uncompressed file */
		sprintf(message,"'%s' (%s)",(*pFormat)->format_names[0],(*pFormat)->format_help);
		ImInfo("Image Format",message);

		/*
		 *  If the fd/fp is a file, or it's a pipe and the handler can write
		 *  directly to a pipe, write it.
		 */
		if ( ioType & IMFILEIOFILE || (*pFormat)->format_writePipe == IMPIPE )
			return ( (*pMap->map_write)( pMap, ioType, fd, fp,
				flagsTable, tagTable ) );

		/*
		 *  Create a temporary file.  Write to it, then copy it out to the
		 *  stream and delete it.  If errors occur, save them until the
		 *  unlink has been done.
		 */
		strcpy( tmpName, imMakeTempFile() );
		if ( ioType & IMFILEIOFD )
		{
			if ( (tmpFd = open( tmpName, O_RDWR|O_CREAT, 0666 )) == -1 )
			{
				ImErrNo = IMESYS;
				ImErrorFatal( ImQError( ), -1, ImErrNo );
			}
			status = (*pMap->map_write)( pMap, IMFILEIOFD | IMFILEIOFILE,
					tmpFd, NULL, flagsTable, tagTable );
			lseek( tmpFd, 0, 0 );
			while ( (n = read( tmpFd, buffer, sizeof( buffer ) )) > 0 )
				write( fd, buffer, n );
			close( tmpFd );
			unlink( tmpName );
			return ( status );
		}
		if ( (tmpFp = fopen( tmpName, "w+b" )) == NULL )
		{
			ImErrNo = IMESYS;
			ImErrorFatal( ImQError( ), -1, ImErrNo );
		}
		status = (*pMap->map_write)( pMap, IMFILEIOFP | IMFILEIOFILE, -1, tmpFp,
			flagsTable, tagTable );
		fseek( tmpFp, 0, 0 );
		while ( (n = fread( buffer, 1, sizeof( buffer ), tmpFp )) != 0 )
			fwrite( buffer, 1, n, fp );
		fclose( tmpFp );
		unlink( tmpName );
	}
	return ( status );
}


/*
 *  FUNCTIONS
 *	ImFileQCompression
 *	ImFileQFCompression
 *	imFileQCompression
 *
 *  DESCRIPTION
 *	Query the compression scheme of a file using the
 *	file descriptor, file pointer, or ioType.
 *
 */
char *
#ifdef __STDC__
ImFileQCompression( int fd, TagTable* flagsTable)
#else
ImFileQCompression( fd, flagsTable)
int fd;
TagTable* flagsTable;
#endif
{
	return imFileQCompression(IOTYPE(fd), fd, NULL, flagsTable);
}

char *
#ifdef __STDC__
ImFileQFCompression( FILE* fp, TagTable* flagsTable)
#else
ImFileQFCompression( fp, flagsTable)
FILE* fp;
TagTable* flagsTable;
#endif
{
	return imFileQCompression(FIOTYPE(fp), -1, fp, flagsTable);
}

/*
 *  FUNCTION
 *	imFileQCompression
 *
 *  DESCRIPTION
 *	Determine whether or not a file is compressed.  If it is,
 *  return the name of the compression scheme.
 */
static char *	/* returns name of compression scheme or NULL */
#ifdef __STDC__
imFileQCompression(int ioType, int fd, FILE* fp, TagTable* flagsTable)
#else
imFileQCompression(ioType, fd, fp, flagsTable)
int ioType;
int fd;
FILE* fp;
TagTable* flagsTable;
#endif
{
	char* fileName;		   /* file name                             */
	TagEntry* tmpEntry;  	   /* holds a tag table entry               */
	int doMagic;		   /* should we check magic#?               */
	int ioOperation;	   /* Read or write?	                    */
	char *strTmp;		   /* holds a string                        */
	int i;		     	   /* index                                 */
	int length;	           /* length of a magic #                   */
	ImCompressScheme**ppScheme;/* points into the table of schemes      */
	ImCompressScheme*pScheme;  /* points into the table of schemes      */
	unsigned char 	  magic1[IMMAXMAGIC]; /* Test magic number          */
	unsigned char     magic2[IMMAXMAGIC]; /* another magic #            */
	unsigned char    *magic;	      /* current magic #            */
	unsigned char    *fmtMagic;           /* current format's magic #   */
	ImFileMagic      *pMagic;	      /* pointer to magic file info */


	/*
	 * We are compressed if...
	 *	the file compression option is in the flags table
	 *  OR  we have one of the magic numbers in the compression table
	 *  OR	the filename in the flags table ends in a suffix in the compression table
	 */

	if (TagTableQNEntry( flagsTable, "file compression" )>0)
	{
		tmpEntry = TagTableQDirect( flagsTable, "file compression", 0 );
		TagEntryQValue( tmpEntry, &strTmp);
		if (ImFileFindCompressionScheme(strTmp)!=NULL)
			return strTmp;
		return NULL;
	}
	
	/*
	 *  Should we check the magic number?
	 *	1.  If file is a pipe or a tty, NO.
	 *	2.  If file is open for writing only, NO.
	 *	3.  Otherwise, YES.
	 */
	doMagic = TRUE;
#if 0
	if ( ioType & IMFILEIOPIPE )
		doMagic = FALSE;	/* Input is a pipe or a tty	*/
	else
	{
		if ( ioType & IMFILEIOFD )
			ioOperation = fcntl( fd, F_GETFL, &ioOperation );
		else
			ioOperation = fcntl( fileno(fp), F_GETFL, &ioOperation);
		if ( ioOperation == -1 )
		{
			ImErrNo = IMESYS;	/* Bad fp/fd?		*/
			return ( 0 );
		}
		if ( ioOperation == O_WRONLY )
			doMagic = FALSE;
	}
#endif
	/*
	 * Get the magic number from the file and check it against each of
	 * the ones in the compression scheme list.
	 */
	if (doMagic)
	{
		/*
		 * Read in the magic number at location 0 (where most are)
		 */
		 if (ioType & IMFILEIOFD)
		 {
			IMREADMAGIC( fd, magic1, IMMAXMAGIC, 0);
		 }
		 else
		 {
			IMREADFMAGIC( fp, magic1, IMMAXMAGIC, 0 );
		 }

		 /*
		  * Check it against the scheme list.  If a scheme
		  * doesn't have a magic number, skip it.  If it has
		  * a magic number, but not at byte 0, then read in
		  * and test with that magic.  Otherwise test with the
		  * the magic number ead in above.
		  */

		  magic = magic1;
		  for (ppScheme = ImGetCompressSchemes(); *ppScheme; ppScheme++)
		  {
			pMagic = (*ppScheme)->compress_magic_numbers;
			if (pMagic == NULL )
			{
				/* No magic numbers */
				continue;
			}
			for ( ; pMagic->format_magicNumber != NULL; pMagic++)
			{
				if (pMagic->format_magicLength==0)
					continue;  /* no magic number */
				if (pMagic->format_magicLocation != 0 )
				{
					if ( ioType & IMFILEIOFD )
					{
						IMREADMAGIC( fd, magic2,
							pMagic->format_magicLength,
							pMagic->format_magicLocation );
					}
					else
					{
						IMREADFMAGIC( fp, magic2,
							pMagic->format_magicLength,
							pMagic->format_magicLocation );
					}
					magic = magic2;
				}
				length = pMagic->format_magicLength;
				fmtMagic = pMagic->format_magicNumber;

				/* See if it matches the magic number */
				for (i=0; i < length; i++)
					if (fmtMagic[i] != magic[i] )
						break;
				if (i==length)
					return ( (*ppScheme)->compress_suffixes[0] );
				magic = magic1;
			}

		  }

	}

	/*
	 * No magic number match.  Check the file name.
	 */

	tmpEntry = TagTableQDirect( flagsTable, "file name", 0 );
	if ( tmpEntry != TAGENTRYNULL )
	{
		TagEntryQValue( tmpEntry, &fileName);
		/*
		 * Set strTmp to be the one more than the last period
		 * in the filename.
		 */
		strTmp = fileName + strlen(fileName) - 1; 
		while (*strTmp!='.' && strTmp!=fileName)
			strTmp--;
		if (*strTmp=='.')
		{
			strTmp++;
			if ( (pScheme=ImFileFindCompressionScheme(strTmp))!=NULL) 
				return pScheme->compress_suffixes[0];
		}
	}

	return NULL;
}




/*
 *  FUNCTION
 *  	imFileWriteCompressedFile
 *
 *  DESCRIPTION
 *	Write a compressed file from the tagTable
 */
static int	/* returns status */
#ifdef __STDC__
imFileWriteCompressedFile(int ioType, int fd, FILE* fp, TagTable* flagsTable, TagTable* tagTable, 
		ImFileFormatWriteMap *pMap, char* scheme)
#else
imFileWriteCompressedFile(ioType, fd, fp, flagsTable, tagTable, pMap, scheme)
int ioType;
int fd;
FILE* fp;
TagTable* flagsTable;
TagTable* tagTable;
ImFileFormatWriteMap *pMap;
char* scheme;			/* Compression scheme to use */
#endif
{
	char tmpName[1024];		/* uncompressed file        */
	char newName[1024];		/* compressed file          */
	FILE* tmpFp;                    /* a file pointer           */
	int status;			/* status                   */
	int n;				/* bytes read               */
	unsigned char buffer[1024];	/* Temporary write buffer   */
	ImCompressScheme* pScheme;	/* points into our table    */

	/*
	 * 1. Dump all of the data into a temp file.
	 * 2. Compress the temp file using the write routine from our table.
	 *    (This will destroy the uncompressed file)
	 * 3. Dump the compressed file to our output stream.
	 * 4. Remove the compressed file.
	 */
	strcpy(tmpName,imMakeTempFile());
	tmpFp = fopen(tmpName,"wb");
	if (tmpFp==NULL)
	{
		ImErrNo = IMESYS;
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	}

	status = (*pMap->map_write)( pMap, IMFILEIOFP | IMFILEIOFILE, -1, tmpFp,
		flagsTable, tagTable );
	fclose(tmpFp);

	/*
	 * Look up the compression scheme
	 */
	 pScheme = ImFileFindCompressionScheme(scheme);
	 if (pScheme==NULL)
	 {
		ImErrNo = IMEFORMATUNKNOWN;
		return NULL;
	 }

	 if ( pScheme->compress_encode == NULL )
	 {
		ImErrNo = IMENOREAD;
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	 }

	/*
	 * Compress our temp file, and destroy the uncompressed one.
	 */
	 if ((*pScheme->compress_encode)(tmpName, newName, flagsTable)==-1)
	 {	
		return -1;
	 }

	tmpFp = fopen(newName,"rb");
	if (tmpFp==NULL)
	{
		ImErrNo = IMESYS;
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	}

	/*
	 * Send the new file to the outgoing stream
	 */
	if ( ioType & IMFILEIOFD )
	{
		while ( (n = fread( buffer, 1, sizeof( buffer ), tmpFp )) != 0 )
			write( fd, buffer, n );
	}
	else
	{
		while ( (n = fread( buffer, 1, sizeof( buffer ), tmpFp )) != 0 )
			fwrite( buffer, 1, n, fp );
	}
	fclose(tmpFp);
	unlink(newName);
	
	return status;
}

/*
 *  FUNCTION
 *  	imFileReadCompressedFile
 *
 *  DESCRIPTION
 *	Read a compressed file into the tagTable
 */
static int
#ifdef __STDC__
imFileReadCompressedFile(int ioType, int fd, FILE* fp, TagTable* flagsTable, TagTable* tagTable, char* scheme)
#else
imFileReadCompressedFile(ioType, fd, fp, flagsTable, tagTable, scheme)
int ioType;
int fd;
FILE* fp;
TagTable* flagsTable;
TagTable* tagTable;
char* scheme;
#endif
{
	char tmpName[1024];		/* compressed file              */
	char newName[1024];		/* uncompressed file            */
	FILE* tmpFp;			/* holds an fp    		*/
	int tmpFd;			/* holds an fd    		*/
	unsigned char buffer[1024];	/* Temporary write buffer	*/
	int status = 1;			/* status			*/
	int n;				/* num of bytes read            */
	ImFileFormat **pFormat;		/* format that we're reading    */
	char* fileName;			/* Name of file (in flagsTable) */
	TagEntry* tmpEntry;		/* holds a tag table entry      */
	char* format;			/* format name                  */
	char message[1024];		/* holds a message		*/
	ImCompressScheme* pScheme;	/* compression scheme entry     */

	/*
	 *	We could do things here with lots of processes
	 *	and lots of pipes.  However, that would get
	 *	quite messy.  Instead we're just going to
	 *	use temp files.
	 *
	 *	Algorithm:
	 *
	 * 	1. Put all the data in a temp file
	 *	2. Uncompress the temp file
	 *	3. Read the uncompressed file
	 *	4. Destroy both files.
	 */
	strcpy( tmpName , imMakeTempFile() );

	/*
	 * Write all the data
	 */
	if ( ioType & IMFILEIOFD )
	{
		if ( (tmpFd = open( tmpName, O_RDWR|O_CREAT, 0666 )) == -1 )
		{
			ImErrNo = IMESYS;
			ImErrorFatal( ImQError( ), -1, ImErrNo );
		}
		while ( (n = read( fd, buffer, sizeof( buffer ) )) > 0 )
			write( tmpFd, buffer, n );
		close( tmpFd );
	}
	else
	{
		if ( (tmpFp = fopen( tmpName, "w+b" )) == NULL )
		{
			ImErrNo = IMESYS;
			ImErrorFatal( ImQError( ), -1, ImErrNo );
		}
		while ( (n = fread( buffer, 1, sizeof( buffer ), fp )) != 0 )
			fwrite( buffer, 1, n, tmpFp );
		/* Write an EOF */
		fclose( tmpFp );
	}

	/*
	 * Call the scheme's read routine.
	 * This routine will uncompress the file,
	 * delete the file we pass it,
	 * and tell us the name of the new file.
	 */
	 pScheme = ImFileFindCompressionScheme(scheme);
	 if (pScheme==NULL)
	 {
		ImErrNo = IMEFORMATUNKNOWN;
		return NULL;
	 }
	 if ( pScheme->compress_decode == NULL )
	 {
		ImErrNo = IMENOREAD;
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	 }
	 if ((*pScheme->compress_decode)(tmpName, newName, flagsTable)==-1)
	 {	
		return -1;
	 }

	/*
	 * Read the uncompressed file
	 */
	 tmpFp = fopen(newName,"rb");
	 if (tmpFp==NULL)
	 {
		ImErrNo = IMESYS;
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	 }

	/*
	 * What format is the image file?
	 */
	tmpEntry = TagTableQDirect( flagsTable, "file name", 0 );
	if ( tmpEntry != TAGENTRYNULL )
	{
		TagEntryQValue( tmpEntry, &fileName);
	}
	else
		fileName = NULL;
	if ((format = ImFileQFFormat(tmpFp, fileName))==NULL)
	{	
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	}

	/*
	 *  Look up the format
	 */
	if ( (pFormat = ImFileFindFormat( format )) == NULL )
	{
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	}

	if ( (*pFormat)->format_read == NULL )
	{
		ImErrNo = IMENOREAD;
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	}

	sprintf(message,"'%s' (%s)",(*pFormat)->format_names[0],(*pFormat)->format_help);
	ImInfo("Image Format",message);


	 status = (*(*pFormat)->format_read)( IMFILEIOFP | IMFILEIOFILE, -1,
		tmpFp, flagsTable, tagTable );
	/*
	 * Destroy the uncompressed file
	 */
	unlink(newName);

	return status;
}


/*
 *  FUNCTION
 *      ImFileFindCompressionScheme
 *
 *  DESCRIPTION
 *	Search through the compression table for this
 *  scheme.
 */
ImCompressScheme *
#ifdef __STDC__
ImFileFindCompressionScheme( char* scheme)
#else
ImFileFindCompressionScheme(scheme)
char* scheme;
#endif
{
 	 int 			found;	   /* Have we found it yet? */
	 ImCompressScheme**	pCompress; /* points into the table */
	 char**			strPtr;	   /* points to list of strs */

	 found = 0;
	 pCompress = ImGetCompressSchemes();
	 while (!found && *pCompress)
	 {
		/* 
		 * Check all the names for this scheme
		 */
		strPtr = (*pCompress)->compress_suffixes;
		while (*strPtr!=NULL && strcmp(*strPtr,scheme)!=0)
			strPtr++;
		if (*strPtr!=NULL)
			found = 1;
		if (!found)
			pCompress++;
	 }
	 if (!found)
	 {	
		ImErrNo = IMEFORMATUNKNOWN;
		return NULL;
	 }
	 return *pCompress;
}

/*
 *  FUNCTION
 *	imMakeTempFile
 *  DESCRIPTION
 *	make file name for a temp file.
 *	This name will have the full path of
 *	the temp file.  (i.e. in the correct
 *	temp directory)
 */
static char* 
#ifdef __STDC__
imMakeTempFile(void)
#else
imMakeTempFile()
#endif
{
	char* tmpDir;	 	/* temp directory */
	static char ret[1024]; 	/* string to return */

	/* Get tmpDir from environment. */
	tmpDir = getenv("TMPDIR");
	if (tmpDir==NULL)
		sprintf(ret,"/tmp/im.XXXXXX");
	else
		sprintf(ret,"%s/im.XXXXXX",tmpDir);
	mktemp(ret);

	return ret;
}




