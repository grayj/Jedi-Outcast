/**

 **	$Header: /roq/libim/imeps.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imeps.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imeps.c		-  Encapsulated PostScript file output

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imeps.c contains a write routine to write Encapsulated PostScript

 **	files for the image manipulation library.

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **	ImFileEpsFormat	v  master format description

 **

 **  PRIVATE CONTENTS

 **	imEpsNames	v  format's name and aliases

 **	imEpsWriteMap	v  write attributes

 **	imEpsMagicNumber v magic number

 **	imEpsMagic	v  list of magic numbers

 **

 **	imEpsHex	v  Integer->Hex character conversion table

 **	imEpsHeader	v  PostScript header section

 **	imEpsProlog	v  PostScript prolog section

 **	imEpsImageColor	v  Color printing PostScript procedure

 **	imEpsImageGray	v  Grayscale printing PostScript procedure

 **	imEpsImageMono	v  Monochrome printing PostScript procedure

 **	imEpsScript	v  PostScript page script section

 **	imEpsData	v  PostScript data section

 **	imEpsTrailer	v  PostScript trailer section

 **

 **	imEpsWrite1	f  write a 1-bit monochrome image to a PostScript file

 **	imEpsWriteGray	f  write a grayscale image to a PostScript file

 **	imEpsWrite8	f  write an 8-bit index image to a PostScript file

 **	imEpsWriteRGB	f  write an RGB image to a PostScript file

 **

 **  HISTORY

 **	$Log: /roq/libim/imeps.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.12  1995/06/30  21:59:24  bduggan

 **	replaced long with time_t

 **

 **	Revision 1.11  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.10  1995/06/15  20:18:01  bduggan

 **	Added some function prototypes.

 **	Removed {}'s from char[] declaration

 **

 **	Revision 1.9  1995/04/03  21:22:57  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.8  1995/01/10  23:21:11  bduggan

 **	Uncapitilized i's in local functions

 **

 **	Revision 1.7  94/10/03  11:30:04  nadeau

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

 **	Revision 1.6  92/12/03  01:47:38  nadeau

 **	Corrected info messages.

 **	

 **	Revision 1.5  92/11/04  11:59:57  groening

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.4  92/10/19  14:15:21  groening

 **	added ImINfo statements

 **	

 **	Revision 1.3  92/09/25  12:38:01  groening

 **	added lines for ImInfo to printout verbose information

 **	

 **	Revision 1.2  92/08/31  17:22:40  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.1  91/10/03  08:49:41  nadeau

 **	Initial revision

 **	

 **	Revision 1.7  91/09/17  20:17:10  nadeau

 **	Added color PostScript support.

 **	

 **	Revision 1.6  91/02/12  10:53:58  nadeau

 **	Removed the tag table checking and VFB conversion now

 **	handled by ImFileWrite.  Updated to use INDEX8 instead

 **	of GRAY VFB.  Changed 'Creator' comment to include the

 **	program name from the flags table, if given.

 **	

 **	Revision 1.5  90/11/27  16:40:44  mjb

 **	Removed spaces in between hex bytes (xerox says this will make it

 **	read much faster).   Made 'image' byte string size the same as vfb

 **	width (ditto above comment)

 **	

 **	Revision 1.4  90/07/02  13:21:33  nadeau

 **	Updated to the new error handling mechanism.

 **	

 **	Revision 1.3  90/06/25  14:36:16  nadeau

 **	Changed ImTag* to Tag* (new names).

 ** 

 **	Revision 1.2  90/05/16  07:47:03  todd

 **	Add #include "iminternal.h" to top of file

 ** 

 **	Revision 1.1  90/05/11  14:28:23  nadeau

 **	Initial revision

 ** 

 **/



#include <time.h>

#include "iminternal.h"



/**

 **  FORMAT

 **	eps	-  Adobe Encapsulated PostScript

 **

 **  AKA

 **

 **  FORMAT REFERENCES

 **	PostScript Language Reference Manual, second edition, Adobe Systems,

 **	Addison Wesley, 1990.

 **

 **	Bit-Mapped Graphics, Steve Rimmer, McGraw-Hill, 1990.

 **

 **	Adobe PhotoShop 2.0 EPS File Format, Thomas Knoll, 1990-1991.

 **

 **  CODE CREDITS

 **	Custom development, Dave Nadeau, San Diego Supercomputer Center, 1990.

 **

 **	Partially based upon ideas and methods provided by Loren "Buck"

 **	Buchanan, Naval Research Laboratory, Kestrel Associates Inc.

 **

 **  DESCRIPTION

 **	Adobe's EPS format is used by applications that create graphics or

 **	imagery for inclusion in another document by another application.

 **	Those graphics or imagery are described using Adobe's PostScript

 **	programming language, but augmented with a rigid set of special EPS

 **	file comments.  Those comments help the including application figure

 **	out what's in the EPS file, how big it is, what it's font requirements

 **	are, and so on.

 **

 **	Adobe PhotoShop further defines an additional EPS file comment that

 **	lets PhotoShop find and read in image data stored in an EPS file.

 **

 **  Header

 **	EPS files begin with a special comment:

 **

 **		%!PS-Adobe-3.0 EPSF-3.0

 **

 **	This line is treated as a magic number by the SDSC Image Tools.

 **

 **	Following the magic number line are a series of header comments

 **	describing the file's contents and purpose:

 **

 **		%%BoundingBox:  lx ly ux uy

 **		%%Creator:      program

 **		%%For:          person

 **		%%CreationDate: date

 **		%%Title:        filename

 **		%%Pages:        # of pages

 **		%%EndComments

 **

 **	The %%BoundingBox comment is required of all EPS files and tells the

 **	reader how much space to leave in the including document for the

 **	EPS file's contents.  Applications may just present the user with a

 **	square box of this size and let the user move it about to locate

 **	the included image in the document.  Such applications need not read

 **	or understand the rest of the EPS file.

 **

 **	The rest of the header comments are optional and are just for

 **	documentation purposes.  The header ends with a %%EndComments comment.

 **

 **  PhotoShop Support

 **	Following this header is PhotoShop's custom "%ImageData" comment:

 **

 **		%ImageData:	w h d ch pch block binhex "startline"

 **

 **	This comment is, essentially, a pointer down to the raw image data

 **	stored later in the file.  "w" and "h" are the width and height of

 **	the EPS image.  "d" is it's channel depth (1 or 8 bits).  "ch" is

 **	the number of image channels (1, 3, or 4).

 **

 **	"pch" is the number of pad channels.  These are extra channels added

 **	to the data that aren't to be imaged (PhotoShop uses this for a

 **	pre-computed grayscale image).  "binhex" is a flag indicating if the

 **	raw image data is binary (1) or hex (2).

 **

 **	The final argument to %ImageData is a copy of the entire PostScript

 **	line immediately preceeding the raw image data.  PhotoShop uses this

 **	in a pattern match search through the file to find the line before

 **	the raw data.  It then uses the rest of the arguments to figure out

 **	how to read that data.

 **

 **	The SDSC Image Tools write out this comment so that PhotoShop can

 **	read in our EPS images.

 **

 **  Preview

 **	In general, an EPS file need not contain raw image data (though that's

 **	what the SDSC Image Tools and PhotoShop output).  An EPS file may

 **	contain almost any PostScript algorithm and data to draw anything.

 **	This presents something of a problem to applications wishing to import

 **	an EPS file and include it in a document (such as a diagram in a Word

 **	document).  How can that application show something meaningful to the

 **	user so that the user can move and adjust the imported EPS data?  The

 **	application can't be expected to parse and execute the PostScript code

 **	of the EPS file.  So, now what?

 **

 **	The answer is something called a "Preview Image".  Programs that

 **	generate EPS files are kindly asked to rasterize the file's contents,

 **	possibly at low res, and always in grayscale, and place that rasterized

 **	version at the head of the file in a preview image comment.  Now,

 **	applications that include the EPS file can present that low res gray

 **	image to the user as a crude representation of what's really in the

 **	EPS file and what will really get printed when the thing is sent to a

 **	PostScript printer.

 **

 **	The Preview Image is a comment block starting and ending with:

 **

 **		%%BeginPreview:  h w d lines

 **		...

 **		%%EndPreview

 **

 **	The %%BeginPreview comment gives the preview image's height, width,

 **	depth (1, 2, 4, or 8) and number of lines of text preview data that

 **	follow.  Applications that wish to strip out the preview image (such

 **	as lpr filters) can toss anything between %%BeginPreview and

 **	%%EndPreview.

 **

 **	Why doesn't PhotoShop use this preview image?  First, it's not

 **	required to be the same resolution as the real image.  Second, it's

 **	always grayscale, even when the real image is color.

 **

 **	The SDSC Image Tools output a Preview Image for those applications

 **	that need it.  The image is always grayscale, and always the same

 **	resolution as the real image later on in the file.

 **

 **  Prolog

 **	EPS files next contain a prolog section intended to be used to define

 **	assorted functions and variables for later use in the file.  We use

 **	the prolog section to define a function "ImEpsImage" to read in and

 **	image the raw image data onto the page.  That function is defined

 **	differently for monochrome, grayscale, and RGB image data.  For RGB

 **	data, that function figures out if the printer to which the EPS data

 **	is being sent can handle color.  If not, the RGB data is converted to

 **	grayscale on the fly.

 **

 **	The Prolog is surrounded by:

 **

 **		%%BeingProlog

 **		...

 **		%%EndProlog

 **		

 **  Page

 **	The actual EPS drawing is surrounded by:

 **

 **		%%Page:  n m

 **		...

 **		%%PageTrailer

 **

 **	Within this comment structure are the PostScript commands to create

 **	the EPS image.  For us, that means the raster image.

 **

 **  Raster Image

 **	The EPS spec requests that raster image data be enclosed within

 **	the comments:

 **

 **		%%BeginData:  format

 **		...

 **		%%EndData

 **

 **	Applications wishing to skip this data (such as a PostScript pretty

 **	printer) use the "format" argument of the %%BeginData line to see

 **	how much to skip.  There are several format types.  We use the form

 **	that lists the number of hex data lines that follow.  We intentionally

 **	use hex, rather than binary, so that our EPS data files can be sent

 **	to printers over 7-bit ASCII connections.

 **

 **	Immediately following the %%BeginData comment is an invokation of our

 **	ImEpsImage function defined in the prolog.  That function reads in

 **	the rest of the data and renders the image on to the page.  Also note

 **	that it is this line (the one invoking ImEpsImage) that is referenced

 **	by the %ImageData comment for PhotoShop found earlier in the file.

 **	So, PhotoShop can scan down to this line, then read the same raw

 **	image data that follows as does ImEpsImage when printing.

 **

 **	Note that the ImEpsImage invokation is within the %%BeginData/%%EndData

 **	comments.  As such we need to add one extra line to our hex line count

 **	on the %%BeginData line.

 **

 **  Trailer

 **	The whole file is ended with a %%Trailer and %%EOF set of comments.

 **

 **/











/*

 *  FUNCTION DECLARATIONS

 */

#ifdef __STDC__

static int imEpsWrite1( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable );

static int imEpsWriteGray( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable );

static int imEpsWrite8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable );

static int imEpsWriteRGB( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable );

#else

static int imEpsWrite1( );

static int imEpsWriteGray( );

static int imEpsWrite8( );

static int imEpsWriteRGB( );

#endif











/*

 *  FORMAT INFORMATION

 *	imEpsNames		-  format's name and aliases

 *	imEpsWriteMap		-  write attributes

 *	imEpsMagicNumber	-  magic number

 *	imEpsMagic		-  list of magic numbers

 *	ImFileEpsFormat		-  master format description

 */

static char *imEpsNames[ ]  = { "eps", "epi", "epsf", "epsi", NULL };

static ImFileFormatWriteMap imEpsWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBMONO,    0,      IN,1,1,         0,      imEpsWrite1 },

        { IMVFBINDEX8,  0,      IN,1,8,         0,      imEpsWriteGray },

        { IMVFBINDEX8,  C,      IN,1,8,         C,      imEpsWrite8 },

        { IMVFBRGB,     0,      RGB,3,8,        0,      imEpsWriteRGB },

        { -1,           0,      -1,             0,      NULL },

};



static unsigned char imEpsMagicNumber[ ] = { '!', '%', 'P', 'S', '-',

	'A', 'd', 'o', 'b', 'e', '-', '3', '.', '0', ' ',

	'E', 'P', 'S', 'F', '-', '3', '.', '0' };

static ImFileMagic imEpsMagic[ ] =

{

	{ 0, 23, imEpsMagicNumber },

	{ 0, 0, NULL },

};



ImFileFormat ImFileEpsFormat=

{

	imEpsNames,					/* Names	*/

	"Encapsulated PostScript file",			/* Description	*/

	"Adobe",					/* Creator	*/

	"None.  Encapsulated PostScript files cannot be read!",/* Read support*/

	"1-bit monochrome, 8-bit grayscale, 8-bit color index, and 24-bit\n\
	RGB color images.",				/* Write support*/

	imEpsMagic,					/* Magic #'s	*/

	IMNOMULTI, IMNOPIPE,				/* Read?	*/

	IMNOMULTI, IMPIPE,				/* Write?	*/

	NULL, NULL, imEpsWriteMap			/* Maps		*/

};











static char imEpsHex[] = "0123456789abcdef";





/*

 *  GLOBAL

 *	imEpsHeader	-  PostScript header section

 *

 *  DESCRIPTION

 *	The PostScript file header is a set of special comments conforming

 *	to the Adobe Document Structuring Conventions (DSC) as specified

 *	in the Adobe PostScript Language Reference Manual (second edition).

 *

 *	The DSC comments give the document name, creator, and whatnot.

 *	Most importantly, the DSC also gives the bounding box for the

 *	document.

 *

 *	After the comment header is the %ImageData comment used by Adobe's

 *	PhotoShop to locate the raw image data for it to read in.

 *

 *	Note that this string will be sent through printf() to fill in the

 *	comment arguments.  So, we need to escape all % comment characters

 *	with a second % to insure it gets through to the output.

 */



static char *imEpsHeader = "\
%%!PS-Adobe-3.0 EPSF-3.0\n\
%%%%BoundingBox:   0 0 %d %d\n\
%%%%Creator:       %s\n\
%%%%For:           %s\n\
%%%%CreationDate:  %s\
%%%%Title:         %s\n\
%%%%Pages:         0\n\
%%%%EndComments\n\
%%ImageData:       %d %d %d %d %d %d %d \"ImEpsImage\"\n\
";











/*

 *  GLOBALS

 *	imEpsImageColor	-  Color printing PostScript procedure

 *	imEpsImageGray	-  Grayscale printing PostScript procedure

 *	imEpsImageMono	-  Monochrome printing PostScript procedure

 *

 *  DESCRIPTION

 *	Each of these three globals defines the same function (proc) for the

 *	PostScript file header.  In each case, the ImEpsImage function takes

 *	hex image data and renders it onto the page.  The difference is only

 *	in the type of data being fed ImEpsImage.

 *

 *	Some parts of this PostScript code were inspired by similar procedures

 *	designed by Loren "Buck" Buchanan of the Naval Research Lab at

 *	Kestrel Associates Inc.  His ideas are used here with his permission

 *	and our thanks.

 */



static char *imEpsImageColor = "\
%%BeginProlog\n\
%  PROC\n\
%	ImEpsImage\n\
%\n\
%  DESCRIPTION\n\
%	RGB hex image data is read from the current file.  If the PostScript\n\
%	device can handle color, the RGB image is imaged as is.  If not, the\n\
%	RGB pixels are converted to grayscale using the NTSC Y equation and\n\
%	imaged.\n\
\n\
/ImEpsImage\n\
{\n\
	/buffer     ImEpsImageW 3 mul string def\n\
	/graybuffer ImEpsImageW string       def\n\
\n\
	ImEpsImageW ImEpsImageH 8\n\
	[ 1 0\n\
	  0 -1\n\
	  0 ImEpsImageH ]\n\
\n\
	% Determine if the PostScript device can handle color by checking if\n\
	% the colorimage operator is defined.\n\
	systemdict /colorimage known\n\
	{\n\
		{\n\
			currentfile buffer readhexstring pop\n\
		} false 3\n\
		colorimage\n\
	}\n\
	{\n\
		% The PostScript device cannot do color.  Convert to grayscale.\n\
		{\n\
			% Get the RGB data\n\
			currentfile buffer readhexstring pop pop\n\
\n\
			% For each pixel...\n\
			0 1 ImEpsImageW 1 sub\n\
			{\n\
				% Compute gray value and store in graybuffer\n\
				graybuffer exch\n\
				dup 3 mul dup 1 add dup 1 add\n\
				     buffer exch get 0.114 mul\n\
				exch buffer exch get 0.587 mul add\n\
				exch buffer exch get 0.299 mul add\n\
				cvi\n\
				put\n\
			} for\n\
			graybuffer\n\
		}\n\
		image\n\
	} ifelse\n\
	showpage\n\
} bind def\n\
\n\
%%EndProlog\n\
";





static char *imEpsImageGray = "\
%%BeginProlog\n\
%  PROC\n\
%	ImEpsImage\n\
%\n\
%  DESCRIPTION\n\
%	Grayscale hex image data is read from the current file and imaged.\n\
\n\
/ImEpsImage\n\
{\n\
	/buffer ImEpsImageW string def\n\
\n\
	ImEpsImageW ImEpsImageH 8\n\
	[ 1 0\n\
	  0 -1\n\
	  0 ImEpsImageH ]\n\
	{\n\
		currentfile buffer readhexstring pop\n\
	}\n\
	image\n\
	showpage\n\
} bind def\n\
\n\
%%EndProlog\n\
";





static char *imEpsImageMono = "\
%%BeginProlog\n\
%  PROC\n\
%	ImEpsImage\n\
%\n\
%  DESCRIPTION\n\
%	Grayscale hex image data is read from the current file and imaged.\n\
\n\
/ImEpsImage\n\
{\n\
	/buffer ImEpsImageW 7 add 8 idiv string def\n\
\n\
	ImEpsImageW ImEpsImageH 1\n\
	[ 1 0\n\
	  0 -1\n\
	  0 ImEpsImageH ]\n\
	{\n\
		currentfile buffer readhexstring pop\n\
	}\n\
	image\n\
	showpage\n\
} bind def\n\
\n\
%%EndProlog\n\
";










/*

 *  GLOBAL

 *	imEpsData	-  PostScript data section

 *

 *  DESCRIPTION

 *	The data section of a PostScript script gives the image data itself.

 *	In our case it defines the size of the image and invokes the image

 *	rendering procedure.  Following it we dump the image data.

 */



static char *imEpsData = "\
%%%%Page:  1 1\n\
userdict begin\n\
/ImEpsImageW	%d def			%% Width in pixels\n\
/ImEpsImageH	%d def			%% Height in pixels\n\
%%%%BeginData:  %d Hex Lines\n\
ImEpsImage\n\
";











/*

 *  GLOBAL

 *	imEpsTrailer	-  PostScript trailer section

 *

 *  DESCRIPTION

 *	The trailer of the page, and file, is just a bunch of DSC keywords.

 */



static char *imEpsTrailer = "\
%%EndData\n\
end\n\
%%PageTrailer\n\
%%Trailer\n\
%%EOF\n\
";











/*

 *  FUNCTION

 *	imEpsWrite1	-  write a 1-bit monochrome image to a PostScript file

 *	imEpsWriteGray	-  write a grayscale image to a PostScript file

 *	imEpsWrite8	-  write an 8-bit index image to a PostScript file

 *	imEpsWriteRGB	-  write an RGB image to a PostScript file

 *

 *  DESCRIPTION

 *	The image is retrieved from the tag table and a PostScript header

 *	giving the image's dimensions output.  The appropriate PostScript

 *	procedure is dumped (grayscale or color pixel handling), followed

 *	by standard execution code and the pixels themselves.

 */



static int				/* Returns # of entries used	*/

#ifdef __STDC__

imEpsWrite1( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable )

#else

imEpsWrite1( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int      ioType;		/* Type of I/O to perform	*/

	int      fd;			/* Output file descriptor	*/

	FILE     *fp;			/* Output file pointer		*/

	TagTable *flagsTable;		/* Flags			*/

	TagTable *tagTable;		/* Table of info to write	*/

#endif

{

	int   ipix;			/* pixel counter		*/

	ImVfbPtr p;			/* VFB pixel pointer		*/

	int   pixel;			/* Gray pixel color		*/

	int   x, y;			/* Pixel location		*/

	int       xdim;			/* # columns			*/

	int       ydim;			/* # rows			*/

	ImVfb    *srcVfb;		/* VFB to convert		*/

	time_t    clock;		/* Clock time			*/

	char	  message[100];		/* information for ImInfo	*/





	/*  Turn fd into fp so we can use fprintf.			*/

	if ( (ioType & IMFILEIOFD) && (fp = fdopen( fd, "rb" )) == NULL )

	{

		ImErrNo = IMESYS;

		return ( -1 );

	}



	/* Dump the header, prolog, script, and data sections.		*/

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &srcVfb );

	xdim = ImVfbQWidth( srcVfb );

	ydim = ImVfbQHeight( srcVfb );

	clock = time( NULL );

#ifndef WIN32

	fprintf( fp, imEpsHeader,

		xdim, ydim,			/* BoundingBox:		*/

		ImErrorProgramName,		/* Creator:		*/

		getlogin( ),			/* For:			*/

		ctime( &clock ),		/* CreationDate:	*/

		ImErrorFileName,		/* Title:		*/

						/* ImageData:		*/

		xdim, ydim, 1,			/* Width, height, depth	*/

		1, 0,				/* # channels, pad chan	*/

		(xdim*1+7)/8,			/* block size		*/

		2 );				/* hex format		*/

#else

	fprintf( fp, imEpsHeader,

		xdim, ydim,			/* BoundingBox:		*/

		ImErrorProgramName,		/* Creator:		*/

		"Virtual Monty",			/* For:			*/

		ctime( &clock ),		/* CreationDate:	*/

		ImErrorFileName,		/* Title:		*/

						/* ImageData:		*/

		xdim, ydim, 1,			/* Width, height, depth	*/

		1, 0,				/* # channels, pad chan	*/

		(xdim*1+7)/8,			/* block size		*/

		2 );				/* hex format		*/

#endif

	sprintf (message, ImErrorFileName);

	ImInfo ("Image Name",message);

	sprintf (message, "%d x %d",xdim, ydim);

	ImInfo ("Resolution",message);

	ImInfo ("Type","1-bit Monochrome");



	/*

	 *  Dump the preview.  Nearly identical to image data except:

	 *	-  0 = white and 1 = black instead of the usual way.

	 *	-  bottom to top instead of top to bottom.

	 */

	fprintf( fp, "%%%%BeginPreview:  %d %d %d %d\n%%", xdim, ydim, 1,

		(((xdim + 7) / 8) * 2 * ydim + 71) / 72 );

	for ( ipix = 0, y = ydim-1; y >= 0; y-- )

	{

		p = ImVfbQPtr( srcVfb, 0, y );

		for ( pixel = 0, x = 1; x <= xdim; x++, ImVfbSInc( srcVfb, p ) )

		{

			pixel = (pixel << 1) | ((~ImVfbQMono( srcVfb, p ))&0x1);

			if ( (x & 0x7) == 0 )

			{

				putc( imEpsHex[ ((pixel >> 4) & 0xF) ], fp );

				putc( imEpsHex[ (pixel & 0xF) ], fp );

				pixel = 0;

				if( ++ipix >= 36 )

				{

					putc( '\n', fp );

					putc( '%', fp );

					ipix = 0;

				}

			}

		}

		if ( (xdim & 0x7) != 0 )

		{

			pixel <<= 8 - (xdim & 0x7);

			putc( imEpsHex[ ((pixel >> 4) & 0xF) ], fp );

			putc( imEpsHex[ (pixel & 0xF) ], fp );

			if( ++ipix >= 36 )

			{

				putc( '\n', fp );

				putc( '%', fp );

				ipix = 0;

			}

		}

	}

	fprintf( fp, "\n%%%%EndPreview\n" );





	/*

	 *  Dump the data routine.

	 *

	 *  Compute the number of hex lines of data written out and put

	 *  that in the args of the %%BeginData comment.  Note that we

	 *  add 1 extra line for the "ImPsImage" function invokation.

	 */

	fprintf( fp, "%s", imEpsImageMono );

	fprintf( fp, imEpsData,

		xdim,				/* ImEpsImageWidth	*/

		ydim,				/* ImEpsImageHeight	*/

		1 + (((xdim + 7) / 8) * 2 * ydim + 71) / 72 );	/* Lines*/





	/* Dump the pixels.						*/

	p = ImVfbQFirst( srcVfb );

	for ( ipix = 0, y = ydim - 1; y >= 0; y-- )

	{

		for ( pixel = 0, x = 1; x <= xdim; x++, ImVfbSInc( srcVfb, p ) )

		{

			pixel = (pixel << 1) | (ImVfbQMono( srcVfb, p ) & 0x1);

			if ( (x & 0x7) == 0 )

			{

				putc( imEpsHex[ ((pixel >> 4) & 0xF) ], fp );

				putc( imEpsHex[ (pixel & 0xF) ], fp );

				pixel = 0;

				if( ++ipix >= 36 )

				{

					/* Add a line break every 36*2=72 char*/

					putc( '\n', fp );

					ipix = 0;

				}

			}

		}

		if ( (xdim & 0x7) != 0 )

		{

			pixel <<= 8 - (xdim & 0x7);

			putc( imEpsHex[ ((pixel >> 4) & 0xF) ], fp );

			putc( imEpsHex[ (pixel & 0xF) ], fp );

			if( ++ipix >= 36 )

			{

				/* Add a line break every 36*2=72 characters*/

				putc( '\n', fp );

				ipix = 0;

			}

		}

	}



	/* Dump the trailer.						*/

	fprintf( fp, "\n%s", imEpsTrailer );

	fflush( fp );



	return ( 1 );		/* Used VFB from tag table		*/

}





static void				/* Returns nothing		*/

#ifdef __STDC__

imEpsWriteGrayPreview( FILE *fp, ImVfb *srcVfb )

#else

imEpsWriteGrayPreview( fp, srcVfb )

	FILE  *fp;			/* File to write to		*/

	ImVfb *srcVfb;			/* VFB to write out		*/

#endif

{

	int       ipix;			/* pixel counter		*/

	ImVfbPtr  p;			/* VFB pixel pointer		*/

	int       pixel;		/* Gray pixel color		*/

	ImVfb    *grayVfb;		/* Grayscale VFB		*/

	int       x, y;			/* Pixel location		*/

	int       xdim;			/* # columns			*/

	int       ydim;			/* # rows			*/



	xdim = ImVfbQWidth( srcVfb );

	ydim = ImVfbQHeight( srcVfb );



	if ( (ImVfbQFields( srcVfb ) & IMVFBINDEX8) == 0 ||

		ImVfbQClt( srcVfb ) != IMCLTNULL )

	{

		/* Not a grayscale VFB.  Make one first.		*/

		grayVfb = ImVfbToGray( srcVfb, IMVFBNEW );

	}

	else

		grayVfb = srcVfb;



	/* Write out a grayscale preview image.				*/

	fprintf( fp, "%%%%BeginPreview:  %d %d %d %d\n%%", xdim, ydim, 8,

		(xdim * 2 * ydim + 71) / 72);

	for ( ipix = 0, y = ydim-1; y >= 0; y-- )

	{

		p = ImVfbQPtr( grayVfb, 0, y );

		for ( x = 0; x < xdim; x++, ImVfbSInc( grayVfb, p ) )

		{

			pixel = ImVfbQGray( grayVfb, p );

			putc( imEpsHex[ (pixel>>4) & 0xf ], fp );

			putc( imEpsHex[ pixel & 0xf ], fp );



			if( ++ipix >= 36 )

			{

				/* Add a line break every 36*2=72 characters*/

				putc( '\n', fp );

				putc( '%', fp );

				ipix = 0;

			}

		}

	}

	fprintf( fp, "\n%%%%EndPreview\n" );



	if ( grayVfb != srcVfb )

		ImVfbFree( grayVfb );

}



static int				/* Returns # of entries used	*/

#ifdef __STDC__

imEpsWriteGray( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable )

#else

imEpsWriteGray( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int      ioType;		/* Type of I/O to perform	*/

	int      fd;			/* Output file descriptor	*/

	FILE     *fp;			/* Output file pointer		*/

	TagTable *flagsTable;		/* Flags			*/

	TagTable *tagTable;		/* Table of info to write	*/

#endif

{

	int       ipix;			/* pixel counter		*/

	ImVfbPtr  p;			/* VFB pixel pointer		*/

	int       pixel;		/* Gray pixel color		*/

	ImVfbPtr  last;			/* Last pixel in VFB		*/

	int       xdim;			/* # columns			*/

	int       ydim;			/* # rows			*/

	ImVfb    *srcVfb;		/* VFB to convert		*/

	time_t    clock;		/* Clock time			*/

	char	  message[100];		/* information for ImInfo	*/





	/*  Turn fd into fp so we can use fprintf.			*/

	if ( (ioType & IMFILEIOFD) && (fp = fdopen( fd, "rb" )) == NULL )

	{

		ImErrNo = IMESYS;

		return ( -1 );

	}





	/* Dump the header.						*/

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &srcVfb );

	xdim = ImVfbQWidth( srcVfb );

	ydim = ImVfbQHeight( srcVfb );

	clock = time( NULL );

#ifndef WIN32

	fprintf( fp, imEpsHeader,

		xdim, ydim,			/* BoundingBox:		*/

		ImErrorProgramName,		/* Creator:		*/

		getlogin( ),			/* For:			*/

		ctime( &clock ),		/* CreationDate:	*/

		ImErrorFileName,		/* Title:		*/

						/* ImageData:		*/

		xdim, ydim, 8,			/* Width, height, depth	*/

		1, 0,				/* # channels, pad chan	*/

		(xdim*1+7)/8,			/* block size		*/

		2 );				/* hex format		*/

#else

	fprintf( fp, imEpsHeader,

		xdim, ydim,			/* BoundingBox:		*/

		ImErrorProgramName,		/* Creator:		*/

		"Virtual Monty",			/* For:			*/

		ctime( &clock ),		/* CreationDate:	*/

		ImErrorFileName,		/* Title:		*/

						/* ImageData:		*/

		xdim, ydim, 8,			/* Width, height, depth	*/

		1, 0,				/* # channels, pad chan	*/

		(xdim*1+7)/8,			/* block size		*/

		2 );				/* hex format		*/

#endif

	sprintf (message, ImErrorFileName);

	ImInfo ("Image Name",message);

	sprintf (message, "%d x %d",xdim, ydim);

	ImInfo ("Resolution",message);

	ImInfo ("Type","8-bit Grayscale");





	/*

	 *  Dump the Preview data.  Identical to grayscale data except

	 *  bottom up.

	 */

	imEpsWriteGrayPreview( fp, srcVfb );





	/*

	 *  Dump the data routine.

	 *

	 *  Compute the number of hex lines of data written out and put

	 *  that in the args of the %%BeginData comment.  Note that we

	 *  add 1 extra line for the "ImPsImage" function invokation.

	 */

	fprintf( fp, "%s", imEpsImageGray );

	fprintf( fp, imEpsData,

		xdim,				/* ImEpsImageWidth	*/

		ydim,				/* ImEpsImageHeight	*/

		1 + (xdim * 2 * ydim + 71) / 72 );	/* # of lines	*/





	/* Dump the pixels.						*/

	p    = ImVfbQFirst( srcVfb );

	last = ImVfbQLast( srcVfb );

	for ( ipix = 0; p <= last; ImVfbSInc( srcVfb, p ) )

	{

		pixel = ImVfbQGray( srcVfb, p );

		putc( imEpsHex[ (pixel>>4) & 0xf ], fp );

		putc( imEpsHex[ pixel & 0xf ], fp );



		if( ++ipix >= 36 )

		{

			/* Add a line break every 36*2=72 characters	*/

			putc( '\n', fp );

			ipix = 0;

		}

	}



	/* Dump the trailer.						*/

	fprintf( fp, "\n%s", imEpsTrailer );

	fflush( fp );



	return ( 1 );		/* Used VFB from tag table		*/

}



static int				/* Returns # of entries used	*/

#ifdef __STDC__

imEpsWrite8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable )

#else

imEpsWrite8( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int      ioType;		/* Type of I/O to perform	*/

	int      fd;			/* Output file descriptor	*/

	FILE     *fp;			/* Output file pointer		*/

	TagTable *flagsTable;		/* Flags			*/

	TagTable *tagTable;		/* Table of info to write	*/

#endif

{

	int       ipix;			/* pixel counter		*/

	ImVfbPtr  p;			/* VFB pixel pointer		*/

	int       value;		/* CLT value			*/

	ImVfbPtr  last;			/* Last pixel in VFB		*/

	int       pixel;		/* Gray pixel color		*/

	ImClt    *clt;			/* VFB's color lookup table	*/

	ImCltPtr  c;			/* CLT entry pointer		*/

	int       xdim;			/* # columns			*/

	int       ydim;			/* # rows			*/

	ImVfb    *srcVfb;		/* VFB to convert		*/

	time_t    clock;		/* Clock time			*/

	char	  message[100];		/* information for ImInfo	*/





	/*  Turn fd into fp so we can use fprintf.			*/

	if ( (ioType & IMFILEIOFD) && (fp = fdopen( fd, "rb" )) == NULL )

	{

		ImErrNo = IMESYS;

		return ( -1 );

	}





	/* Dump the header.						*/

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &srcVfb );

	xdim = ImVfbQWidth( srcVfb );

	ydim = ImVfbQHeight( srcVfb );

	clock = time( NULL );

#ifndef WIN32

	fprintf( fp, imEpsHeader,

		xdim, ydim,			/* BoundingBox:		*/

		ImErrorProgramName,		/* Creator:		*/

		getlogin( ),			/* For:			*/

		ctime( &clock ),		/* CreationDate:	*/

		ImErrorFileName,		/* Title:		*/

						/* ImageData:		*/

		xdim, ydim, 8,			/* Width, height, depth	*/

		3, 0,				/* # channels, pad chan	*/

		1,				/* block size (interleaved)*/

		2 );				/* hex format		*/

#else

	fprintf( fp, imEpsHeader,

		xdim, ydim,			/* BoundingBox:		*/

		ImErrorProgramName,		/* Creator:		*/

		"Virtual Monty",			/* For:			*/

		ctime( &clock ),		/* CreationDate:	*/

		ImErrorFileName,		/* Title:		*/

						/* ImageData:		*/

		xdim, ydim, 8,			/* Width, height, depth	*/

		3, 0,				/* # channels, pad chan	*/

		1,				/* block size (interleaved)*/

		2 );				/* hex format		*/

#endif

	sprintf (message, ImErrorFileName);

	ImInfo ("Image Name",message);

	sprintf (message, "%d x %d",xdim, ydim);

	ImInfo ("Resolution",message);

	ImInfo ("Type","8-bit Color Indexed");

	ImInfo ("Color Table","256 Entries");





	/* Dump the preview image as 8-bit grayscale, bottom to top.	*/

	imEpsWriteGrayPreview( fp, srcVfb );





	/*

	 *  Dump the data routine.

	 *

	 *  Compute the number of hex lines of data written out and put

	 *  that in the args of the %%BeginData comment.  Note that we

	 *  add 1 extra line for the "ImPsImage" function invokation.

	 */

	fprintf( fp, "%s", imEpsImageColor );

	fprintf( fp, imEpsData,

		xdim,				/* ImEpsImageWidth	*/

		ydim,				/* ImEpsImageHeight	*/

		1 + (xdim * 6 * ydim + 71) / 72 );	/* # of lines	*/





	/*

	 *  Dump the pixels.  RGBRGBRGB...

	 */

	p    = ImVfbQFirst( srcVfb );

	last = ImVfbQLast( srcVfb );

	clt  = ImVfbQClt( srcVfb );

	for ( ipix = 0; p <= last; ImVfbSInc( srcVfb, p ) )

	{

		pixel = ImVfbQIndex8( srcVfb, p );

		c     = ImCltQPtr( clt, pixel );



		value = ImCltQRed( c );

		putc( imEpsHex[ (value>>4) & 0xf ], fp );

		putc( imEpsHex[ value & 0xf ], fp );



		value = ImCltQGreen( c );

		putc( imEpsHex[ (value>>4) & 0xf ], fp );

		putc( imEpsHex[ value & 0xf ], fp );



		value = ImCltQBlue( c );

		putc( imEpsHex[ (value>>4) & 0xf ], fp );

		putc( imEpsHex[ value & 0xf ], fp );



		if( ++ipix >= 12 )

		{

			/* Add a line break every 12*3*2=72 characters	*/

			putc( '\n', fp );

			ipix = 0;

		}

	}



	/* Dump the trailer.						*/

	fprintf( fp, "\n%s", imEpsTrailer );

	fflush( fp );



	return ( 2 );		/* Used VFB & CLT from tag table	*/

}



static int				/* Returns # of entries used	*/

#ifdef __STDC__

imEpsWriteRGB( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable )

#else

imEpsWriteRGB( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int      ioType;		/* Type of I/O to perform	*/

	int      fd;			/* Output file descriptor	*/

	FILE     *fp;			/* Output file pointer		*/

	TagTable *flagsTable;		/* Flags			*/

	TagTable *tagTable;		/* Table of info to write	*/

#endif

{

	int       ipix;			/* pixel counter		*/

	ImVfbPtr  p;			/* VFB pixel pointer		*/

	int       pixel;		/* RGB pixel color		*/

	ImVfbPtr  last;			/* Last pixel in VFB		*/

	int       xdim;			/* # columns			*/

	int       ydim;			/* # rows			*/

	ImVfb    *srcVfb;		/* VFB to convert		*/

	time_t    clock;		/* Clock time			*/

	char	  message[100];		/* information for ImInfo	*/





	/* Turn fd into fp so we can use fprintf.			*/

	if ( (ioType & IMFILEIOFD) && (fp = fdopen( fd, "rb" )) == NULL )

	{

		ImErrNo = IMESYS;

		return ( -1 );

	}





	/* Dump the header.						*/

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &srcVfb );

	xdim = ImVfbQWidth( srcVfb );

	ydim = ImVfbQHeight( srcVfb );

	clock = time( NULL );

#ifndef WIN32

	fprintf( fp, imEpsHeader,

		xdim, ydim,			/* BoundingBox:		*/

		ImErrorProgramName,		/* Creator:		*/

		getlogin( ),			/* For:			*/

		ctime( &clock ),		/* CreationDate:	*/

		ImErrorFileName,		/* Title:		*/

						/* ImageData:		*/

		xdim, ydim, 8,			/* Width, height, depth	*/

		3, 0,				/* # channels, pad chan	*/

		1,				/* block size (interleaved)*/

		2 );				/* hex format		*/

#else

	fprintf( fp, imEpsHeader,

		xdim, ydim,			/* BoundingBox:		*/

		ImErrorProgramName,		/* Creator:		*/

		"Virtual Monty",			/* For:			*/

		ctime( &clock ),		/* CreationDate:	*/

		ImErrorFileName,		/* Title:		*/

						/* ImageData:		*/

		xdim, ydim, 8,			/* Width, height, depth	*/

		3, 0,				/* # channels, pad chan	*/

		1,				/* block size (interleaved)*/

		2 );				/* hex format		*/

#endif



	sprintf (message, ImErrorFileName);

	ImInfo ("Image Name",message);

	sprintf (message, "%d x %d",xdim, ydim);

	ImInfo ("Resolution",message);

	ImInfo ("Type","24-bit RGB");





	/* Dump the preview image as 8-bit grayscale, bottom to top.	*/

	imEpsWriteGrayPreview( fp, srcVfb );





	/*

	 *  Dump the data routine.

	 *

	 *  Compute the number of hex lines of data written out and put

	 *  that in the args of the %%BeginData comment.  Note that we

	 *  add 1 extra line for the "ImPsImage" function invokation.

	 */

	fprintf( fp, "%s", imEpsImageColor );

	fprintf( fp, imEpsData,

		xdim,				/* ImEpsImageWidth	*/

		ydim,				/* ImEpsImageHeight	*/

		1 + (xdim * 6 * ydim + 71) / 72 );	/* # of lines	*/



	/*

	 *  Dump the pixels.  RGBRGBRGB...

	 */

	p    = ImVfbQFirst( srcVfb );

	last = ImVfbQLast( srcVfb );

	for ( ipix = 0; p <= last; ImVfbSInc( srcVfb, p ) )

	{

		pixel = ImVfbQRed( srcVfb, p );

		putc( imEpsHex[ (pixel>>4) & 0xf ], fp );

		putc( imEpsHex[ pixel & 0xf ], fp );



		pixel = ImVfbQGreen( srcVfb, p );

		putc( imEpsHex[ (pixel>>4) & 0xf ], fp );

		putc( imEpsHex[ pixel & 0xf ], fp );



		pixel = ImVfbQBlue( srcVfb, p );

		putc( imEpsHex[ (pixel>>4) & 0xf ], fp );

		putc( imEpsHex[ pixel & 0xf ], fp );



		if ( ++ipix >= 12 )

		{

			/* Add a line break every 12*3*2=72 characters	*/

			putc( '\n', fp );

			ipix = 0;

		}

	}



	/* Dump the trailer.						*/

	fprintf( fp, "\n%s", imEpsTrailer );

	fflush( fp );



	return ( 1 );		/* Used VFB from tag table		*/

}

