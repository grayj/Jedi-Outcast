/**

 **	$Header: /roq/libim/imfmt.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"	$Header: /roq/libim/imfmt.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imfmt.c		-  master file format table for the image library

 **

 **  PROJECT

 **	libim		-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imfmt.c contains the master file format table for the image

 **	library.  The table lists all of the image file formats supported.

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **	ImGetFileFormats	f  Get the master list of formats.

 **

 **  PRIVATE CONTENTS

 **

 **	imFileFormats	v  list of file format information

 **

 **	None

 **

 **  HISTORY

 **	$Log: /roq/libim/imfmt.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.14  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.13  1995/06/16  08:41:13  bduggan

 **	Made ImFileFormats static, and added a function which returns the list.

 **	The reason for this is that when making a shared object on the sun,

 **	all global variables must be in a seperate file.  It's easier just

 **	to make this global variable static, and have a global function.

 **	(Sorry -- that's inaccurate.  Not ALL global variable must be in

 **	a seperate file.  All the global variables which are used by the tools

 **	need to be in a seperate file.)

 **

 **	Revision 1.12  1995/05/17  23:43:43  bduggan

 **	added miff, xpm

 **	Merged rpbm, rpnm, rppm, ppm, pnm, pbm

 **

 **	Revision 1.11  1995/02/16  21:40:00  bduggan

 **	Jpegs

 **

 **	Revision 1.10  1994/10/03  11:29:36  nadeau

 **	Updated to ANSI C and C++ compatibility.

 **	Removed all use of register keyword.

 **	Minimized use of custom SDSC types (e.g., uchar vs. unsigned char)

 **	Changed all float arguments to double.

 **	Added forward declarations.

 **	Added misc. casts to passify SGI and DEC compilers.

 **	Changed all macros and defined constants to have names

 **	starting with IM.

 **	Updated comments.

 **	Updated indenting on some code.

 **	Updated copyright message.

 **

 **	Revision 1.9  92/11/04  12:00:19  groening

 **	took the ImFileFormat info and put

 **	them in their corresponding image files.

 **	

 **	Revision 1.8  92/09/17  14:38:16  vle

 **	Added BMP, CUR, and ICO formats.  Re-ordered some declarations.

 **	Updated copyright notice.

 **	

 **	Revision 1.7  92/06/18  16:01:17  groening

 **	targa and viff formats added

 **	

 **	Revision 1.6  92/04/07  15:32:43  vle

 **	Re-ordered the format specifications for IRIS Rgb to be consistent

 **	with rest of imfmt.c.

 **	

 **	Revision 1.5  92/04/03  18:13:52  vle

 **	imconv can no longer output an xwd image without a colortable.

 **	Added lines to allow imconv to support 8-bit image read and 

 **	writes and changed read/write function names to be consistent

 **	with rest of code.

 **	

 **	Revision 1.4  91/10/03  09:01:29  nadeau

 **	Changed 'interlace' to 'interleave'.  Commented out CGM

 **	support.  Added EPS, ICON, and PCX support.  Changed

 **	PBM and RPBM entries to be multiple entries, one per

 **	format variant.  Updated support for XWD and RAS.  Changed

 **	comments for HDF, PS, PIC, and others (cosmetic changes).

 **	

 **	Revision 1.3  91/03/13  12:49:57  nadeau

 **	Fixed various can/can't handle pipe setting bugs.

 **	Reordered RGB alpha compression write preferences for TIFF.

 **	

 **	Revision 1.2  91/03/12  11:02:12  nadeau

 **	Changed pic support to allow pipe read and write.

 **	

 **	Revision 1.1  91/03/08  14:39:30  nadeau

 **	Initial revision

 **	

 **/



#include "iminternal.h"



/**

 **  TABLES AND INITIALIZATIONS

 **	imFileFormats	-  master file format table

 **

 **  CODE CREDITS

 **	Custom development, Dave Nadeau, San Diego Supercomputer Center, 1991.

 **

 **  DESCRIPTION

 **	The SDSC Image Tools' file format handling is driven by a master table

 **	listing the supported image file formats, their attributes, and the

 **	functions to call to read and write data in that format.  That master

 **	table is "imFileFormats" declared and initialized here in this file.

 **

 **	File formats are read and written by format "handlers":  C functions

 **	found in separate files in this package.  For instance, all of the

 **	handlers for the Sun Rasterfile format are found in "imras.c".

 **

 **	Each format handler file also declares and initializes a set of

 **	structures and arrays that serve to describe that format.  These

 **	structures give the name of the format, alternative names, details

 **	on what types of data might be found in the file, what variants the

 **	handlers can read and write, the magic numbers characteristic of that

 **	format, and so on.  All of this is contained within a struct of type

 **	"ImFileFormat".  The master imFileFormats array declared here, in this

 **	file, is just an array of pointers to these individual file format

 **	handler descriptions.  The format descriptions themselves are in the

 **	same source file as the handler routines for that format, such as

 **	"imras.c" for Sun Rasterfiles.

 **

 **  ADDING A NEW FILE FORMAT

 **	The SDSC Image Tools are highly configurable to support new file

 **	formats.  Adding a format is largely a matter of adding an additional

 **	initialization to the imFileFormats array.  Thereafter all of the

 **	SDSC Image Tools will automatically support that new format (after

 **	relinking) without any change to their code.

 **

 **	Here's a brief outline of the steps to go through to add a new file

 **	format.  We'll go into greater detail on each of these steps later in

 **	this comment block.

 **

 **		1.  Write the read and write routines.

 **

 **		2.  Declare and initialize the ImFileFormat struct and it's

 **		    associated sub-structures.

 **

 **		3.  Add an "extern" statement to this file to pull in your

 **		    new file format's ImFileFormat struct.  It's good practice

 **		    to also add a #define name and #ifdef's around that extern

 **		    statement (see below).

 **

 **		4.  Add an initialization to the imFileFormats array to point

 **		    to your file format's ImFileFormat struct.

 **

 **			PLEASE!  Add new entries in alphabetical order, sorted

 **			by the first file extension name of the format.  This

 **			will insure that printed file format lists, such as by

 **			the "imformats" tool, will look good.

 **

 **		5.  Update the Makefile, build the library and relink the tools.

 **

 **  WRITING THE READ ROUTINE

 **	Every format has one top-level read routine called by ImFileRead( )

 **	when the user requests that format.  Typically, read routines are

 **	named with the convention of starting with "im" followed by the format's

 **	name, followed by "Read".  So, for instance, the Sun Rasterfile read

 **	routine is named "imRasRead".

 **

 **	The read routine should be declared as "static" (private) to the

 **	handler source file.  There is no need for it to be called directly

 **	from outside the handler file.  It will, instead, be called via a

 **	function pointer that you place into your handler's "ImFileFormat"

 **	struct.

 **

 **	A handler read routine is called with the following arguments:

 **

 **		int ioType

 **			The type of file descriptor or pointer on which to

 **			do I/O.  The user's calls to either ImFileRead (with

 **			a file descriptor) or ImFileFRead (with a file pointer)

 **			both vector down into the same handler read function.

 **			Read functions should do if's on the ioType to see

 **			which kind of file I/O to do.

 **

 **		int fd

 **			The file descriptor if (ioType & IMFILEIOFD) is true.

 **

 **		FILE *fp

 **			The file pointer if (ioType & IMFILEIOFP) is true.

 **

 **		TagTable *flagsTable

 **			A table of flags that the user has supplied to steer

 **			the read operation.  Global flags that set up error

 **			message redirection, and so on, have already been

 **			extracted and handled.  In general, read handlers

 **			need not check the flagsTable.

 **

 **		TagTable *tagTable

 **			The table into which to put any data read from the

 **			file.

 **

 **	The read handler should direct all file I/O through the SDSC Binary

 **	I/O Library to insure maximum portability among hardware architectures

 **	with different byte orders, word sizes, floating point formats, and

 **	structure padding.  Please see the SDSC Binary I/O man pages for

 **	details.

 **

 **	Images read from the file should be placed into ImVfb's allocated via

 **	ImVfbAlloc( ).  Please see the ImVfbAlloc( ) man page for details and

 **	pointers to other relevant man pages.

 **

 **	Color tables read from the file should be placed into ImClt's allocated

 **	via ImCltAlloc( ).  Please see the ImCltAlloc( ) man page for details

 **	and pointers to other relevant man pages.

 **

 **	Data from the file should be appended to the tagTable using

 **	TagTableAppend( ) in the SDSC TagTable Library.  Please see the

 **	TagTableAppend( ) man page for details.

 **

 **	When everything else is done, the read handler should return a -1 on

 **	an error (and set ImErrNo to an appropriate error code), or a positive

 **	constant counting the number of things added to the tagTable.

 **

 **	Error messages should always be output using one of three macros:

 **

 **		ImErrorInfo( message, return_code, errno )

 **		ImErrorWarning( message, return_code, errno )

 **		ImErrorFatal( message, return_code, errno )

 **

 **	ImErrorInfo( ) messages are for information purposes only.  These

 **	are typically general info about what has been found in the file.

 **	Command-line tools often block such output unless the -verbose option

 **	has been given.

 **

 **	ImErrorWarning( ) messages are to warn of possible problems in reading

 **	the file.  Warnings are for recoverable problems.  Command-line tools

 **	typically print these to stderr.

 **

 **	ImErrorFatal( ) messages are to alert to a fatal reading problem and

 **	are not recoverable.  Command-line tools typically print these to

 **	stderr.

 **

 **	Each of these three macros covers up a set of if-statements that

 **	select printing the message to a user-supplied stream, calling a

 **	user-supplied callback, or skipping the message entirely.  If a

 **	callback is called, the callback may return a flag indicating that

 **	the read routine is to abort (imagine an X dialog box with a user

 **	choice for "continue" or "abort").  If the callback says to abort,

 **	the macro will automatically exit the read handler routine, returning

 **	the given "return_code" and setting ImErrNo to the given "errno".

 **	Please note:  ImErrorFatal( ) ALWAYS exits the read routine,

 **	regardless of the callback's return value.

 **

 **  WRITING THE WRITE ROUTINES

 **	Every format has one or more write routines.  When the user calls

 **	ImFileWrite( ), that function scans the list of supported output

 **	image types described in the format's write map (see below) and

 **	calls the write function appropriate for the given VFB.  So, typically

 **	there is one write routine for each VFB type that can be written to

 **	the output file.

 **

 **	Write handler's have the same call arguments as read handlers, with

 **	one extra argument at the front of the list:

 **

 **		ImFileFormatWriteMap *pMap

 **			While the write map is described in more detail below,

 **			the gist of it is that each file format has an array

 **			of structs called a "write map".  Each array entry

 **			describes a VFB type that can be written to that

 **			format.  Each array entry also has a pointer to a

 **			write function to do the writing of that VFB type.

 **			When that write function is called, the "pMap"

 **			argument is set to point to that array entry.

 **

 **			In general, write handlers can ignore the pMap

 **			argument.

 **

 **		int ioType

 **		int fd

 **		FILE *fp

 **		TagTable *flagsTable

 **		TagTable *tagTable

 **

 **	Most of the flags in the flagsTable have already been processed by

 **	ImFileWrite( ) and used to decide which write handler function to call.

 **	Most write handlers can ignore the flagsTable.

 **

 **	Data written to the output file should be pulled directly from the

 **	tagTable and should be written in the order in which it appears in

 **	the tagTable.

 **

 **	All file I/O should be done through the SDSC Binary I/O library in

 **	order to insure maximum portability.

 **

 **	Info, warning, and fatal error messages should be printed using the

 **	same set of macros as described above for read handlers.

 **

 **	When the write handler is complete, it should return a -1 if an error

 **	occurred (and set ImErrNo to an appropriate error code), or a positive

 **	integer indiciating the number of tags in the tagTable that were

 **	used.

 **

 **  MAPPING TABLE INITIALIZATIONS

 **	The mapping tables tell the generic portions of the library what the

 **	various formats are capable of handling.  This lets the higher-level

 **	code return errors to the user if, say, the user wants to write a

 **	JPEG-compressed image to a GIF file, which can't do JPEG compression.

 **	At the same time it lets that higher-level code convert images to

 **	different pixel depths if necessary *before* handing them to the

 **	write handler.  This means that if the user asks to write a 24-bit RGB

 **	image to a GIF file, which can't do 24-bit images, the higher-level

 **	code will automatically drop it down to 8-bit before calling the write

 **	handler.

 **

 **   Read Mapping

 **	The read mapping table is primarily informational.  Very little code

 **	uses it except for nice printouts.

 **

 **	The read map table is an array of structures, each one describing one

 **	type of data that might be found in the file format.  One entry might,

 **	for instance, say the format can support an 8-bit pseudo-color image

 **	with a color table.  The next entry might say the same format could

 **	instead have a 24-bit RGB image.  And the third entry might say the

 **	format could have a 24-bit RGB image with an alpha channel.  Each

 **	variant gets it's own struct in the array and it's own initialization.

 **

 **	Structure initializations are divided into columns (one column per

 **	struct field).

 **

 **	Columns 1-3 describe the image storage of the incomming image.  The

 **	first is a type indicator (color index or RGB).  The second is the

 **	number of channels of data (usually 1 or 3).  The third is the size,

 **	in bits, of each channel (usually 1 or 8).  Do not include the

 **	alpha channel (if any) in the depth.  That info goes in Column 4.

 **

 **		Example:	IN,1,8		-- 8-bit index

 **				RGB,3,8		-- 3*8-bit = 24-bit RGB

 **

 **	Column 4 is a mask of attributes for the incomming image.  The

 **	mask indicates if a CLT or alpha plane is included, how images

 **	are interleaved (RGB only), and what compression scheme is used.

 **

 **		Example:	0		-- nothing

 **				C		-- has CLT

 **				C|A		-- has CLT and alpha

 **				A|RLE|PI	-- plane interleaved RLE & alpha

 **

 **	Column 5 is the VFB type of the resulting image.  Do not include

 **	IMVFBALPHA if an alpha plane is present.  That info goes in Column 6.

 **

 **		Example:	IMVFBINDEX8	-- 8-bit index

 **				IMVFBRGB	-- RGB

 **

 **	Column 6 is a mask of attributes for the VFB.  The mask indicates if

 **	a CLT or alpha plane is included.  Interleave and compression codes

 **	are inappropriate for this column.

 **

 **		Example:	0		-- no clt or alpha

 **				C|A		-- clt and alpha

 **

 **	For example:  a mythical format "Xyz" supports 8-bit color index and

 **	24-bit true-color RGB images.  8-bit images always have a CLT.  RGB

 **	images never do.  RGB images may be uninterleaved, or plane interleaved.

 **	And finally, the image could be uncompressed, or run-length encoded

 **	(RLE).

 **

 **	static ImFileFormatReadMap imXyzReadMap[ ] =

 **	{

 **		{ IN,1,8,	RLE | C,	IMVFBINDEX8,	C },

 **		{ IN,1,8,	C,		IMVFBINDEX8,	C },

 **		{ RGB,3,8,	RLE | PI,	IMVFBRGB,	0 },

 **		{ RGB,3,8,	PI,		IMVFBRGB,	0 },

 **		{ RGB,3,8,	RLE,		IMVFBRGB,	0 },

 **		{ RGB,3,8,	0,		IMVFBRGB,	0 },

 **		{ -1,           0,		-1,		0 },

 **	};

 **

 **	Always end the list with a -1 depth entry.

 **

 **	8-bit index occurs twice in this example, once with "RLE" Or-ed into

 **	the incomming attribute mask, and once without.  If no compression

 **	flag is Or-ed in, it is assumed to mean uncompressed.

 **

 **	24-bit RGB occurs four times:  twice with "RLE", and twice without.

 **	For each of the RLE cases, one has "PI" for "Plane Interleave"

 **	Or-ed in, and one doesn't.  Likewise with the non-"RLE" cases.

 **

 **	The "C", "PI", "RLE", and so on flags are short-cut #define's found

 **	in iminternal.h.  They include:

 **

 **		C	-  CLT is included

 **		A	-  Alpha plane is included

 **

 **		LI	-  Line interleaved RGB

 **		PI	-  Plane interleaved RGB

 **

 **		RLE	-  Run-length encoded

 **		LZW	-  Lempel-Ziv & Welsh compressed

 **		PB	-  Macintosh PackBits

 **		DCT	-  Descrete Cosine Transform compression

 **

 **		IN	-  Index type image

 **		RGB	-  RGB type image

 **

 **	If C is not present, it is assumed there is no CLT.

 **

 **	If A is not present, it is assumed there is no alpha plane.

 **

 **	If neither LI or PI are present, it is assumed that the image

 **	is not interleaved.

 **

 **	If none of RLE, LZW, PB, or DCT are present, it is assumed that the

 **	image is uncompressed.

 **

 **   Write Mapping

 **	The write mapping table is critical.  It is used by the ImFileWrite( )

 **	code to convert the VFB to be written in to something the format can

 **	write.  Depth's are changed.  CLT's are added.  And so on.  The

 **	table is also used to make nice printouts.

 **

 **	As with the read map, the write map is really an array of structs

 **	initialized in the handler's source file.  That initialization takes

 **	the form of a series of rows and columns of data.  Each row describes

 **	one output variant supported.  The column meanings are described below:

 **

 **	Column 1 is the incomming VFB type.  Do not include IMVFBALPHA if an

 **	alpha plane is to be present.  That info goes in column 2.

 **

 **		Example:	IMVFBINDEX8	-- 8-bit index

 **				IMVFBRGB	-- RGB

 **				IMVFBMONO	-- monochrome index

 **

 **	Column 2 is a mask of required attributes for the incomming VFB.  The

 **	mask indicates if a CLT or alpha plane must be present.  Interleave

 **	and compression scheme codes are inappropriate for this column.

 **

 **		Example:	0		-- neither clt or alpha

 **				C|A		-- clt and alpha

 **

 **	Columns 3-5 are the image type and depth attributes of the outgoing

 **	(in the file) image.  This includes the type (index or RGB), number

 **	of channels (usually 1 or 3), and number of bits per channel (usually

 **	1 or 8).  Do not include an alpha channel in the depth.  That goes

 **	in column 6.

 **

 **		Example:	IN,1,8		-- 8-bit index

 **				RGB,3,8		-- 3*8-bit = 24-bit RGB

 **

 **	Column 6 is a mask of resulting attributes for the outgoing image.

 **	The mask indicates if a CLT or alpha plane is included, how images

 **	are interleaved (RGB only), and what compression scheme is used.

 **

 **		Example:	0		-- no clt, no alpha, ...

 **				C|A|RLE		-- clt + alpha + RLE compress

 **				A|LI		-- alpha + line interleaved

 **

 **	Column 7 is an integer function pointer to the function to do the

 **	writing of that variant of the format.  This is one of the write

 **	handlers for the format.

 **

 **	For example:  the same mythical "Xyz" format used for the read map

 **	example is used.

 **

 **	private ImFileFormatWriteMap imXyzWriteMap[ ] =

 **	{

 **		{ IMVFBINDEX8, 0,  IN,1,8,	RLE | C, ImXyzWrite8_RLE_C},

 **		{ IMVFBINDEX8, C,  IN,1,8,	RLE | C, ImXyzWrite8_RLE_C},

 **		{ IMVFBINDEX8, 0,  IN,1,8,	C,       ImXyzWrite8_C },

 **		{ IMVFBINDEX8, C,  IN,1,8,	C,       ImXyzWrite8_C },

 **

 **		{ IMVFBRGB,    0,  RGB,3,8,	RLE | PI,ImXyzWriteRGB_RLE_PI },

 **		{ IMVFBRGB,    0,  RGB,3,8,	RLE,     ImXyzWriteRGB_RLE },

 **		{ IMVFBRGB,    0,  RGB,3,8,	PI,      ImXyzWriteRGB_PI},

 **		{ IMVFBRGB,    0,  RGB,3,8,	0,       ImXyzWriteRGB },

 **		{ -1,	       0,  -1,		0,	 NULL },

 **	};

 **

 **	Always end the list with a -1 VFB type entry.

 **

 **	When multiple entries are made for the same VFB type and incomming

 **	attributes, list from "best" to "worst".  List compressed versions

 **	before uncompressed.  List plane and line interleaved RGB before

 **	uninterleaved.  List non-alpha versions before alpha, and so on.

 **

 **	In this example, 8-bit VFB's occur four times:  twice for RLE output

 **	encoding, and twice without.  Within each of those, one occurrence

 **	requires no CLT, and one requires one.  However, both of those cases

 **	go to the same routine (which presumably checks if the VFB has a CLT,

 **	and if not creates one).

 **

 **	24-bit RGB VFB's occur four times:  twice for RLE output encoding,

 **	and twice without.  Within each of those, one occurrence is

 **	uninterleaved, and one plane interleaved.

 **

 **	There may not be two identical lines in the table.  However, any number

 **	of lines may use the same input conditions and/or the same write

 **	function.

 **

 **	ImFileWrite() checks this table and the flagsTable giving the user's

 **	write request flags, and decides which write function to call.  If

 **	it can't find one that matches the request, and it can't convert the

 **	VFB into one that matches, then it returns an error to the user without

 **	every calling any format handler function.

 **

 **  MAGIC NUMBERS

 **	Magic numbers are a UNIX trick to help in identifying the type of data

 **	found in a file.  The typically just amount to 2 to 4 bytes at the

 **	start of the file that are a kind of signature for that type of data.

 **	A Sun Rasterfile, for instance, starts with 0x59, 0xA6, 0x6A, 0x95.

 **	An SGI RGB file starts with 0x01, 0xDA.  And so on.

 **

 **	Not all format types have magic numbers, but many do.  The SDSC Image

 **	Tools use magic numbers to help figure out what type of file format

 **	is being used by an incomming image file.  Based upon that we can

 **	figure out which format handler to call to read it in.

 **

 **	Magic numbers are described an array of ImFileMagic structs declared

 **	and initialized within the format handler's source file.  Each array

 **	entry describes one possible magic number for that format.  Formats

 **	with multiple possible magic numbers (such as the PBM+ format) will

 **	list multiple numbers.

 **

 **	Each array entry gives the file byte offset at which a magic number

 **	can be found (usually 0), the number of bytes in the magic number

 **	(usually 2 or 4), and a pointer to an unsigned char array of those

 **	bytes.  For example, here is the set of declarations for the magic

 **	number of the Microsoft Windows Cursor file format:

 **

 **		static unsigned char imCurMagicNumber[ ] =

 **			{ 0x00, 0x00, 0x02, 0x00 };

 **		static ImFileMagic imCurMagic[ ] =

 **		{

 **			{ 0, 4, imCurMagicNumber },

 **			{ 0, 0, NULL }

 **		};

 **

 **	The magic number initialization is always terminated with a

 **	{ 0, 0, NULL } entry.

 **

 **	If the format has no magic number, then the array should be initialized

 **	to just one { 0, 0, NULL }.

 **

 **  FORMAT NAMES

 **	Most image file formats have multiple common names by which they are

 **	known.  The Sun Rasterfile format, for instance, is known by at

 **	least the following names and common file name extensions:

 **

 **		ras, sun, sr, scr

 **

 **	The PBM+ files are known by:

 **

 **		pbm, pgm, ppm, pnm, rpbm, rpgm, rppm, rpnm

 **

 **	And so on.  Users of the SDSC Image Tools are able to use any of

 **	these file format names, and filename extensions, when refering to

 **	the format.  Writers of new file format handlers need to list those

 **	names so that the higher-level code can recognize them.  Such a list

 **	is simply an initialized array of char *'s.  The following, for

 **	instance, is the array initialization for the Sun Rasterfile handlers:

 **

 **		static char *imRasNames[ ] =

 **			{ "ras", "sun", "sr", "scr", NULL };

 **

 **	Note that the list must end with a NULL pointer.

 **

 **	The first entry in the array has special significance and is usually

 **	considered the primary name for the format.

 **

 **  THE IMFILEFORMAT INITIALIZATION

 **	Every file format has one, globally accessible, ImFileFormat struct

 **	and it's initialization.  It is a pointer to that struct that is

 **	placed into the imFileFormats array initialization found here in this

 **	file.  This one ImFileFormat struct for the format describes

 **	everything we need to know about the format.

 **

 **	The ImFileFormat struct has a number of fields as follows:

 **

 **		format_names

 **			The name of the format names array discussed earlier.

 **			This array lists the various names by which the

 **			file format is known.

 **

 **		format_help

 **			This is a character string giving the long form of

 **			the file format's name.  For instance, Adobe EPS

 **			file format says:  "Encapsulated PostScript file".

 **			This string is used in format listings and help lists.

 **

 **		format_creator

 **			This is a character string giving the name of the

 **			creator of the file format (not the programmer of

 **			this code!).  So, for Adobe EPS, the creator string

 **			says:  "Adobe".  This info is used in format listings.

 **

 **		format_readSupport

 **			This is a one or more line character string describing

 **			in human-readable terms what variants of the file

 **			format can be read by the handler.  This should closely

 **			mimic what is found in the handler's read map, but be

 **			more friendly to read.  This info is used in format

 **			listings.

 **

 **		format_writeSupport

 **			This is a one or more line character string describing

 **			in human-readable terms what variants of the file

 **			format can be written by the handler.  This should

 **			closely mimic what is found in the handler's write

 **			map, but be more friendly to read.  This info is used

 **			in format listings.

 **

 **		format_magicMap

 **			This is initialized to a pointer to the magic number

 **			array discussed earlier.  This array lists the

 **			various magic numbers for the file format.

 **

 **		format_readMultiVfb

 **			Can the read handler read multiple images from the

 **			same file?  This depends upon the abilities of the

 **			file format.  If so, initialize this to IMMULTI.  If

 **			not, initialize this to IMNOMULTI.

 **

 **		format_readPipe

 **			Can the read handler read from a pipe?  If the handler

 **			doesn't do any seeks, the answer is IMPIPE.  Otherwise

 **			the answer is IMNOPIPE.  For IMNOPIPE formats, the

 **			higher-level code will automatically stage things to

 **			an intermediate file and hand that file to the read

 **			handler instead of the original pipe.

 **

 **		format_writeMultiVfb

 **			Can the write handler write multiple images to the

 **			same file?  This depends upon the abilities of the

 **			file format.  If so, initialize this flag to IMMULTI.

 **			Otherwise initialize it to IMNOMULTI.

 **

 **		format_writePipe

 **			Can the write handler write to a pipe?  If the handler

 **			doesn't do any seeks, the answer is IMPIPE.  Otherwise

 **			the answer is IMNOPIPE and the higher-level code will

 **			automatically stage to a temp file rather than giving

 **			the pipe to the write handler.

 **

 **		format_read

 **			This is initialized to the handler's read function.

 **

 **		format_readMap

 **			This is initialized to a pointer to the read map

 **			array discussed earlier.

 **

 **		format_writeMap

 **			This is initialized to a pointer to the write map

 **			array discussed earlier.

 **

 **	For example, here's the ImFileFormat array structure declaration

 **	and initialization for the Compuserve GIF format:

 **

 **	ImFileFormat ImFileGifFormat =

 **	{

 **		imGifNames, "Graphics Image File",

 **		"Compuserve",

 **		"1- thru 8-bit color index Lempel-Ziv & Welsh-compressed files",

 **		"1 and 8-bit color index Lempel-Ziv & Welsh-compressed files",

 **		imGifMagic,

 **		IMNOMULTI, IMNOPIPE,

 **		IMNOMULTI, IMNOPIPE,

 **		imGifRead, imGifReadMap, imGifWriteMap,

 **	};

 **/















/*

 *  FORMATS

 *	These #define's enable inclusion of the initialization for that

 *	format into the imFileFormats table.

 *

 *	It is considered good style to define one of these for every file

 *	format added.  It is not, however, required.  These names are only

 *	known within the scope of this file.

 */



#define bmp

#define cur

#define eps

#define gif

#define hdf

#define ico

#define icon

#define iff

#define jpeg

#define miff

#define mpnt

#define pbm

#define pcx

#define pic

#define pict

#define pix

#define ras

#define rgb

#define rla

#define rle

#define softimage

#define synu

#define tga 

#define tiff

#define viff

#define x

#define xbm

#define xpm

#define xwd 











/*

 *  FUNCTION DECLARATIONS

 *	ImFile***Format	-  external file format description structures

 *

 *	It is considered good style to put an #ifdef around each extern

 *	statement so that sites that don't want all of these formats installed

 *	can block one or more of them.

 */



#ifdef bmp

	 extern ImFileFormat ImFileBmpFormat;

#endif /* bmp */



#ifdef cur

	 extern ImFileFormat ImFileCurFormat;

#endif /* cur */



#ifdef eps

	 extern ImFileFormat ImFileEpsFormat;

#endif /* eps */



#ifdef gif

	 extern ImFileFormat ImFileGifFormat;

#endif /* gif */



#ifdef hdf

	 extern ImFileFormat ImFileHdfFormat;

#endif /* hdf */



#ifdef ico

	 extern ImFileFormat ImFileIcoFormat;

#endif /* ico */



#ifdef icon

	 extern ImFileFormat ImFileIconFormat;

#endif /* icon */



#ifdef iff

	 extern ImFileFormat ImFileIffFormat;

#endif /* iff */



#ifdef jpeg

	 extern ImFileFormat ImFileJpegFormat;

#endif /* jpeg */



#ifdef miff

	 extern ImFileFormat ImFileMiffFormat;

#endif /* miff */



#ifdef mpnt

	 extern ImFileFormat ImFileMpntFormat;

#endif /* mpnt */



#ifdef pbm

	 extern ImFileFormat ImFilePbmFormat;

#endif /* pbm */



#ifdef pcx

	 extern ImFileFormat ImFilePcxFormat;

#endif /* pcx */



#ifdef pic

	 extern ImFileFormat ImFilePicFormat;

#endif /* pic */



#ifdef pict

	 extern ImFileFormat ImFilePictFormat;

#endif /* pict */



#ifdef pix

	 extern ImFileFormat ImFilePixFormat;

#endif /* pix */



#ifdef ps

	 extern ImFileFormat ImFilePsFormat;

#endif /* ps */



#ifdef ras

	 extern ImFileFormat ImFileRasFormat;

#endif /* ras */



#ifdef rgb

	 extern ImFileFormat ImFileRgbFormat;

#endif /* rgb */



#ifdef rla

	 extern ImFileFormat ImFileRlaFormat;

#endif /* rla */



#ifdef rle

	extern ImFileFormat ImFileRleFormat;

#endif /* rle */



#ifdef softimage

	 extern ImFileFormat ImFileSoftimageFormat;

#endif /* softimage */



#ifdef synu

	 extern ImFileFormat ImFileSynuFormat;

#endif /* synu */



#ifdef tga

	 extern ImFileFormat ImFileTgaFormat;

#endif /* tga */



#ifdef tiff

	 extern ImFileFormat ImFileTiffFormat;

#endif /* tiff */



#ifdef viff

	 extern ImFileFormat ImFileViffFormat;

#endif /* viff */



#ifdef x

	 extern ImFileFormat ImFileXFormat;

#endif /* x */



#ifdef xbm

	 extern ImFileFormat ImFileXbmFormat;

#endif /* xbm */



#ifdef xpm

	 extern ImFileFormat ImFileXpmFormat;

#endif /* xpm */



#ifdef xwd

	 extern ImFileFormat ImFileXwdFormat;

#endif /* xwd */











/*

 *  STATIC VARIABLE

 *	imFileFormats	-  ** Master file format table ! **

 *

 *  DESCRIPTION

 *	Add one pointer to this array initialization for each file format

 *	struct.  Please add them in alphabetical order, sorted by the first

 *	name in the format's name list.  This makes output listings look

 *	nicer.

 *

 *	It is considered good style to put an #ifdef around each initialization

 *	so that sites that don't wish all of these formats to be installed

 *	can simply and easily block one or more.

 */



static ImFileFormat  *imFileFormats[ ] =

{



#ifdef bmp

	 &ImFileBmpFormat,

#endif /* bmp */





#ifdef cur

	 &ImFileCurFormat,

#endif /* cur */





#ifdef eps

	 &ImFileEpsFormat,

#endif /* eps */





#ifdef gif

	 &ImFileGifFormat,

#endif /* gif */





#ifdef hdf

	 &ImFileHdfFormat,

#endif /* hdf */





#ifdef ico

	 &ImFileIcoFormat,

#endif /* ico */





#ifdef icon

	 &ImFileIconFormat,

#endif /* icon */





#ifdef iff

	 &ImFileIffFormat,

#endif /* iff */





#ifdef jpeg

	 &ImFileJpegFormat,

#endif /* jpeg */





#ifdef miff

	 &ImFileMiffFormat,

#endif /* miff */





#ifdef mpnt

	 &ImFileMpntFormat,

#endif /* mpnt */





#ifdef pbm

	 &ImFilePbmFormat,

#endif /* pbm */





#ifdef pcx

	 &ImFilePcxFormat,

#endif /* pcx */





#ifdef pgm

	 &ImFilePgmFormat,

#endif /* pgm */





#ifdef pic

	 &ImFilePicFormat,

#endif /* pic */





#ifdef pict

	 &ImFilePictFormat,

#endif /* pict */





#ifdef pix

	 &ImFilePixFormat,

#endif /* pix */





#ifdef ps

	 &ImFilePsFormat,

#endif /* ps */





#ifdef ras

	 &ImFileRasFormat,

#endif /* ras */





#ifdef rgb

	 &ImFileRgbFormat,

#endif /* rgb */





#ifdef rla

	 &ImFileRlaFormat,

#endif /* rla */





#ifdef rle

	 &ImFileRleFormat,

#endif /* rle */





#ifdef softimage

	 &ImFileSoftimageFormat,

#endif /* softimage */





#ifdef synu

	 &ImFileSynuFormat,

#endif /* synu */





#ifdef tga

	 &ImFileTgaFormat,

#endif /* tga */



#ifdef tiff

	 &ImFileTiffFormat,

#endif /* tiff */





#ifdef viff

	 &ImFileViffFormat,

#endif /* viff */





#ifdef x

	 &ImFileXFormat,

#endif /* x */





#ifdef xbm

	 &ImFileXbmFormat,

#endif /* xbm */





#ifdef xpm

	 &ImFileXpmFormat,

#endif /* xpm */





#ifdef xwd

	 &ImFileXwdFormat,

#endif /* xwd */





	  NULL			/* The table must end with a NULL	*/

};



/*

 *  FUNCTION

 *	ImGetFileFormats

 *

 *  DESCRIPTION

 *	Give the table to the user.

 */

ImFileFormat** 

#ifdef __STDC__

ImGetFileFormats( void )

#else

ImGetFileFormats()

#endif

{

	return imFileFormats;

}



