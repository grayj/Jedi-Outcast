/**
 **	$Header: /roq/libsdsc/bin.c 1     11/02/99 4:39p Zaphod $
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

#define HEADER	"    $Header: /roq/libsdsc/bin.c 1     11/02/99 4:39p Zaphod $"

/**
 **  FILE
 **	bin.c		-  Binary I/O Functions
 **
 **  PROJECT
 **	libsdsc		-  SDSC standard function library
 **
 **  DESCRIPTION
 **	This file contains source for the portable binary I/O function set.
 **	These functions allow binary files to be read and written regardless
 **	of the machine type and its word size and structure field padding.
 **
 **  ASSUMPTIONS!!!
 **	It is assumed that a variable of type 'char' is exactly 8-bits long,
 **	and no longer!
 **
 **	The size of variables, and the resolution (amount of storage actually
 **	used) are both assumed to be multiplies of 1 byte (8-bits).
 **
 **	It is assumed that the host represents integers in 2's compliment form.
 **
 **	See the discussion of the 'binFloatFormatInfo' structure for further
 **	assumptions regarding floating point.
 **
 **  PUBLIC CONTENTS
 **			d =defined constant
 **			f =function
 **			m =defined macro
 **			t =typedef/struct/union
 **			v =variable
 **			? =other
 **
 **	BinErrNo	v  error number
 **	BinNErr		v  number of error messages
 **	BinErrList	v  error messages
 **
 **	BinQMachine	f  Query machine information
 **	BinPError	f  Print error message
 **	BinQError	f  Query error message
 **	BinErrorHandler	f  nominate an error condition handler func
 **
 **	BinByteOrder	f  Select the byte order of the file
 **	BinQByteOrder	f  Query the byte order of the file
 **	BinFloatFormat	f  Select the floating point format of the file
 **	BinQFloatFormat	f  Query the floating point format of the file
 **
 **	BinRead		f  Read binary file using unbuffered I/O
 **	BinFRead	f  Read binary file using buffered I/O
 **	BinSRead	f  Read binary string
 **	BinWrite	f  Write binary file using unbuffered I/O
 **	BinFWrite	f  Write binary file using buffered I/O
 **	BinSWrite	f  Write binary string
 **
 **	BinReadStruct	f  Read binary file into a C struct using unbuf I/O
 **	BinFReadStruct	f  Read binary file into a C struct using buf I/O
 **	BinSReadStruct	f  Read binary string into a C struct
 **	BinWriteStruct	f  Write binary file from a C struct using unbuf I/O
 **	BinFWriteStruct	f  Write binary file from a C struct using buf I/O
 **	BinSWriteStruct	f  Write binary string from a C struct
 **
 **  PRIVATE CONTENTS
 **	binFileBO	v  current file byte order
 **	binReadMBFDisp	v  array of MBF handling functions
 **	binReadLBFDisp	v  array of LBF handling functions
 **	binReadDisp	v  pointer to the current read function array
 **	binWriteMBFDisp	v  array of MBF handling functions
 **	binWriteLBFDisp	v  array of LBF handling functions
 **	binWriteDisp	v  pointer to the current write function array
 **
 **	binTypeName	v  names of the types
 **	binTypeSize	v  size of each type, as stored by host
 **	binTypeRes	v  resolution of each type, as used by host
 **	binTypeSigned	v  is the type signed?
 **	binTypePad	v  how are types padded?
 **	binTypePadMask	v  how are type's padding addresses masked?
 **
 **	binPadStructxxx 	t  Dummy structures to figure padding
 **	binFloatFormatInfo	t  Floating point format description
 **	binFloatFormatCooked	t  cooked floating point format description
 **	binHostFloatFormat	v  Host floating point format description
 **	binFileFloatFormat	v  File floating point format description
 **
 **	binErrFunc	v  error condition handler function
 **	binOp		v  Current operation (for error handling)
 **
 **	binInit		v  Tables initialized yet?
 **	binInitialize	f  Initialize the tables
 **	binDefaultHandler f  Default error condition handler
 **	binNoHandler	f  No error condition handler
 **
 **	binByteRead	f  read bytes into a buffer
 **	binByteWrite	f  write bytes from a buffer
 **
 **	binReadCharMBF	f  Read into characters from an MBF input
 **	binReadCharLBF	f  Read into characters from an LBF input
 **	binWriteCharMBF	f  Write from characters to MBF output
 **	binWriteCharLBF	f  Write from characters to LBF output
 **
 **	binReadIntMBF	f  Read MBF input into integers
 **	binReadIntLBF	f  Read LBF input into integers
 **	binWriteIntMBF	f  Write from integers to MBF output
 **	binWriteIntLBF	f  Write from integers to LBF output
 **
 **	binReadFloat	f  Read float from input
 **	binWriteFloat	f  Write from float to output
 **
 **  HISTORY
 **	$Log: /roq/libsdsc/bin.c $
 * 
 * 1     11/02/99 4:39p Zaphod
 **	Revision 1.18  1995/06/30  21:52:27  bduggan
 **	added a cast
 **	took  out curses.h
 **
 **	Revision 1.17  1995/06/29  07:19:16  bduggan
 **	changed an unsigned char to an unsigned int (oops.)
 **
 **	Revision 1.17  1995/06/29  07:17:54  bduggan
 **	changed an unsigned char to an inunsigned int
 **
 **	Revision 1.16  1995/06/29  00:17:39  bduggan
 **	updated copyright
 **
 **	Revision 1.15  1995/06/29  00:13:51  bduggan
 **	removed use of 'this', added prototypes
 **
 **	Revision 1.14  1995/04/21  17:56:41  bduggan
 **	Changed bzero to memset, replaced prototypes for
 **	standard functions with include files.
 **
 **	Revision 1.13  1995/04/12  22:27:52  bduggan
 **	Made non-ansi compatible.
 **
 **	Revision 1.12  1994/10/03  16:18:52  nadeau
 **	Built table initializations in to code instead of using external
 **	include files generated by the Makefile.  No longer seems necessary
 **	to generate on-the-fly since the value set is relatively static.
 **	Fixed a bug in reading & writing structures that failed to advance
 **	through memory correctly on non-32-bit architectures.
 **	Updated to ANSI C and C++ compatibility.
 **	Removed all use of register keyword.
 **	Minimized use of custom SDSC types (e.g., uchar vs. unsigned char)
 **	Added forward declarations.
 **	Added misc. casts to passify SGI and DEC compilers.
 **	Updated comments.
 **	Updated indenting on some code.
 **	Updated copyright message.
 **
 **	Revision 1.11  92/09/02  13:28:57  vle
 **	Updated copyright notice.
 **	
 **	Revision 1.10  92/03/04  09:19:54  nadeau
 **	Fixed bugs in BinSWriteStruct and BinSReadStruct.  Both were
 **	forgetting to increment the output/input 'data' buffer from
 **	struct field to struct field.  The resulting read and write
 **	operations failed to read or write from the correct locations.
 **	
 **	Revision 1.9  91/12/23  11:43:01  nadeau
 **	Corrected comments.  Optimized code throughout.  Added generic
 **	byte read and write functions to handle I/O to the various
 **	ioType's (fd, fp, and string).  Added support for string read
 **	and write (BinSRead, BinSWrite, etc).  Simplified and reduced
 **	bulky code.
 **	
 **	Revision 1.8  91/10/03  13:06:38  nadeau
 **	Changed binread/binwrite to accept counts of 0 and
 **	just return silently.
 **	
 **	Revision 1.7  91/09/17  19:18:43  nadeau
 **	Misc changes to comments.  Removed some ifdefs for MBF/LBF
 **	handling in favor of a defined BINHOSTBO constant.  Removed
 **	bzero alternate code.  Assume bzero is available.
 **	
 **	Revision 1.6  91/03/11  16:34:23  nadeau
 **	Fixed bug in float write case:  was calling write() when
 **	should have called fwrite().
 **	
 **	Revision 1.5  91/01/09  16:36:52  nadeau
 **	Added BinQError().
 **	
 **	Revision 1.4  90/06/25  10:38:11  ferrerj
 **	Added IBM floating point format.
 **	
 **	Revision 1.3  90/05/16  12:20:45  nadeau
 **	Added code to handle file pointers as well as file descriptors.
 **	Previous code just got the fileno() of the fp and used the file
 **	descriptor code.  This caused problems with data already in the
 **	input/output buffers.
 **	
 **	Revision 1.2  90/04/23  14:11:05  nadeau
 **	Changed BinErrno to BinErrNo.
 **	
 **	Revision 1.1  89/12/14  13:56:57  nadeau
 **	Initial revision
 **	
 **/
/*LINTLIBRARY*/

#include "sdsccopyright.h"
#include <stdlib.h>
#include <stdio.h>
#include "sdsc.h"
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
//#include <unistd.h>

#ifndef NULL
#define NULL	(0)
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef TRUE
#define TRUE	1
#endif

#define BINMAXBUF	(40*sizeof( int ))/* Max size before dynamic alloc*/

#define FPIO		1
#define FDIO		2
#define DATAIO		3

#ifdef MBF
#define BINHOSTBO	BINMBF
#else
#define BINHOSTBO	BINLBF
#endif

#define BINERROR(errno,ret)	{BinErrNo = (errno);return(ret);}



/*
 *  FUNCTION DECLARATIONS
 */

#ifdef __STDC__
static void	binInitialize( void );
static int	binNoHandler( int, int, int, void *, int, int );
static int	binDefaultHandler( int, int, int, void *, int, int );
static int	binByteRead(     int, int, FILE *, unsigned char *, unsigned char *, int );
static int	binByteWrite(    int, int, FILE *, unsigned char *, unsigned char *, int );
static int	binReadCharMBF(  int, int, FILE *, unsigned char *, unsigned char *, int, int, int );
static int	binReadCharLBF(  int, int, FILE *, unsigned char *, unsigned char *, int, int, int );
static int	binWriteCharMBF( int, int, FILE *, unsigned char *, unsigned char *, int, int, int );
static int	binWriteCharLBF( int, int, FILE *, unsigned char *, unsigned char *, int, int, int );
static int	binReadIntMBF(   int, int, FILE *, unsigned char *, unsigned char *, int, int, int );
static int	binReadIntLBF(   int, int, FILE *, unsigned char *, unsigned char *, int, int, int );
static int	binWriteIntMBF(  int, int, FILE *, unsigned char *, unsigned char *, int, int, int );
static int	binWriteIntLBF(  int, int, FILE *, unsigned char *, unsigned char *, int, int, int );
static int	binReadFloat(    int, int, FILE *, unsigned char *, unsigned char *, int, int, int );
static int	binWriteFloat(   int, int, FILE *, unsigned char *, unsigned char *, int, int, int );
#else

static void	binInitialize( );
static int	binNoHandler( );
static int	binDefaultHandler( );
static int	binByteRead( );
static int	binByteWrite( );
static int	binReadCharMBF( );
static int	binReadCharLBF( );
static int	binWriteCharMBF( );
static int	binWriteCharLBF( );
static int	binReadIntMBF( );
static int	binReadIntLBF( );
static int	binWriteIntMBF( );
static int	binWriteIntLBF( );
static int	binReadFloat( );
static int	binWriteFloat( );
#endif





/*
 *  GLOBAL VARIABLE
 *	BinErrNo	-  error number
 *	BinNErr		-  number of error messages
 *	BinErrList	-  error messages
 *	binErrFunc	-  error condition handler function
 *
 *  DESCRIPTION
 *	On an error, the binary I/O package routines return -1 and set
 *	BinErrNo to an error code.  The programmer may call BinPError
 *	to print the associated error message to stderr, or may do the
 *	message lookup in BinErrList themselves.
 */
int BinErrNo = -1;		/* Bin package error number		*/
char *BinErrList[] =		/* Error message list			*/
{
	/* BINESYS */		"System error:  see errno",
	/* BINETYPE */		"Unknown type selection",
	/* BINENBYTES */	"nBytes cannot be <= 0",
	/* BINECOUNT */		"count cannot be <= 0",
	/* BINEBYTEORDER */	"Unknown file byte order selection",
	/* BINEMALLOC */	"Cannot malloc more host memory",
	/* BINEFLOATFORMAT */	"Unknown floating point format selection",
	/* BINEFLOATUNSUPPORTED*/"Unsupported size for file floating point selection",
};
int BinNErr  = 8;		/* Number of error messages		*/
#ifdef __STDC__
static int (*binErrFunc)( int, int, int, void *, int, int ) = NULL;
				/* Error condition callback func	*/
#else
static int (*binErrFunc)( ) = NULL;
				/* Error condition callback func	*/
#endif






/*
 *  GLOBAL VARIABLE
 *	binTypeName	-  names of the types
 *
 *  DESCRIPTION
 *	To aid in debugging, binTypeName gives the character string name for
 *	a type.
 */
static char *binTypeName[BINNTYPE] =
{
	"pointer",
	"char",
	"unsigned char",
	"short",
	"long",
	"int",
	"int8",
	"int16",
	"int32",
	"unsigned short",
	"unsigned long",
	"unsigned int",
	"unsigned int8",
	"unsigned int16",
	"unsigned int32",
	"boolean",
	"float",
	"double",
	"int64",
	"unsigned int64"
};





/*
 *  GLOBAL VARIABLE
 *	binTypeSize	-  size of each type, as stored by host
 *	binTypeRes	-  resolution of each type, as used by host
 *
 *  DESCRIPTION
 *	binTypeSize holds the size, in bytes, of each type supported
 *	by this package.  The size is defined as the number of bytes
 *	a single item of this type takes up in memory on the host.
 *
 *	binTypeRes holds the resolution, in bytes, of each type.  The
 *	resolution is defined as the number of bytes ** ACTUALLY USED **
 *	by the host in manipulating an item of this type.
 *
 *	On most systems the size of a type and the resolution of a type
 *	are the same.  A 'short' on a SPARCstation takes up 2 bytes of
 *	memory and can hold 2 bytes worth of data (16 bits).  However,
 *	on some systems, most notibly the Cray, the size and resolution of
 *	some types differ.  A 'short' on a CRAY-XMP takes up 8 bytes of
 *	memory but can only hold 3 bytes of data (24 bits).
 *
 *	In processing items we need to know the size of an item so that
 *	we can properly advance through an array of data treated as
 *	unsigned char's.
 *
 *	We need to know the resolution of an item so that we know how
 *	many bytes to put into and take out of a file before padding.
 *
 *	binTypeSize is intialized at compile time.  binTypeRes is
 *	initialized at runtime upon its first usage.
 */
static int binTypeSize[BINNTYPE] =
{
#ifdef __STDC__
	sizeof( void * ),
#else
	sizeof( unsigned char * ),
#endif
	sizeof( char ),
	sizeof( unsigned char ),
	sizeof( short ),
	sizeof( long ),
	sizeof( int ),
	sizeof( sdsc_int8 ),
	sizeof( sdsc_int16 ),
	sizeof( sdsc_int32 ),
	sizeof( unsigned short ),
	sizeof( unsigned long ),
	sizeof( unsigned int ),
	sizeof( sdsc_uint8 ),
	sizeof( sdsc_uint16 ),
	sizeof( sdsc_uint32 ),
	sizeof( sdsc_boolean ),
	sizeof( float ),
	sizeof( double ),
#if LONG_SIZE >= 64
	sizeof( sdsc_int64 ),
	sizeof( sdsc_uint64 )
#else
	0,
	0
#endif
};
static int binTypeRes[BINNTYPE];




/*
 *  GLOBAL VARIABLE
 *	binTypeSigned	-  is the type signed?
 *
 *  DESCRIPTION
 *	binTypeSigned indicates whether a type is signed or not.  Signed
 *	types require sign-extension when they are read into with fewer
 *	bytes than their size, and cause sign-extension when they are
 *	written out into greater bytes than their size.
 *
 *	This array is initialized at compile time.
 */
static int binTypeSigned[BINNTYPE] =
{
	FALSE,
	TRUE,
	FALSE,
	TRUE,
	TRUE,
	TRUE,
	TRUE,
	TRUE,
	TRUE,
	FALSE,
	FALSE,
	FALSE,
	FALSE,
	FALSE,
	FALSE,
	FALSE,
	TRUE,
	TRUE,
	TRUE,
	FALSE
};





/*
 *  TYPEDEF & STRUCTURE
 *	binPadStructxxxx	-  Dummy structure to figure padding
 *
 *  DESCRIPTION
 *	Most compilers pad structures to move integer and float types
 *	to the next word or halfword boundary.  BinReadStruct and
 *	BinWriteStruct must be aware of this padding in order to
 *	properly advance through the input or output buffer to the
 *	next field of a struct to read or write.
 *
 *	These pad structures are only used in the initialization of the
 *	binTypePad[].
 */

#define PADSTRUCT(name,type)					\
typedef struct name						\
{								\
	char bin_c;						\
	type bin_i;						\
} name;


#ifdef __STDC__
PADSTRUCT( binPadStructpointer,	void * )
#else
PADSTRUCT( binPadStructpointer,	unsigned char * )
#endif
PADSTRUCT( binPadStructchar,	char )
PADSTRUCT( binPadStructuchar,	unsigned char )
PADSTRUCT( binPadStructshort,	short )
PADSTRUCT( binPadStructlong,	long )
PADSTRUCT( binPadStructint,	int )
PADSTRUCT( binPadStructint8,	sdsc_int8 )
PADSTRUCT( binPadStructint16,	sdsc_int16 )
PADSTRUCT( binPadStructint32,	sdsc_int32 )
PADSTRUCT( binPadStructushort,	unsigned short )
PADSTRUCT( binPadStructulong,	unsigned long )
PADSTRUCT( binPadStructuint,	unsigned int )
PADSTRUCT( binPadStructuint8,	sdsc_uint8 )
PADSTRUCT( binPadStructuint16,	sdsc_uint16 )
PADSTRUCT( binPadStructuint32,	sdsc_uint32 )
PADSTRUCT( binPadStructboolean,	sdsc_boolean )
PADSTRUCT( binPadStructfloat,	float )
PADSTRUCT( binPadStructdouble,	double )
#if LONG_SIZE >= 64
PADSTRUCT( binPadStructint64,	sdsc_int64 )
PADSTRUCT( binPadStructuint64,	sdsc_uint64 )
#endif
 
#undef PADSTRUCT
 
 


/*
 *  GLOBAL VARIABLE
 *	binTypePad	-  how are types padded?
 *	binTypePadMask	-  how are type's padding addresses masked?
 *
 *  DESCRIPTION
 *	Each entry in this array gives the padding boundary to advance to
 *	for a particular field type when padding structures.  For instance,
 *	on most 32-bit machines, structure fields of type 'short' are padded
 *	to bring them to the next 16-bit boundary, while fields of type
 *	'long' are padded to the next 32-bit boundary.  A structure like
 *	the following:
 *		struct example
 *		{
 *			char c1;
 *			short s;
 *			char c2;
 *			long l;
 *		};
 *	actually occupies space like the following:
 *		struct example
 *		{
 *			char c1;
 *			char pad1;
 *			short s;
 *			char c2;
 *			char pad2[3];
 *			long l;
 *		};
 *
 *	BinReadStruct and BinWriteStruct must know about such padding in
 *	order to properly advance from field to field within the structure.
 *
 *	The above dummy padding structure declarations create one structure
 *	for each supported type.  The structure declares a single character
 *	field, followed by a subject field.  By calculating the difference
 *	in the addresses between these two fields, we get the byte boundary
 *	the type must be padded to.  A value of 1 means no padding.
 *
 *	The binTypePadMask array is an array of address masks, one per type.
 *	The mask is calculated to mask off the relevant bits in a char ptr
 *	that should be 0 for the type's alignment needs.  For instance,
 *	on a byte-addressed machine (like a Sun) that aligns int's to 4-byte
 *	boundaries, the mask would be 0x3.  The mask is used in code like:
 *
 *			if ( ((int)addr & mask) != 0 )
 *				addr = (addr & ~mask) + pad;
 *
 *	The pad and mask tables are both initialized at runtime.
 */
static int binTypePad[BINNTYPE];
static unsigned long binTypePadMask[BINNTYPE];





/*
 *  GLOBAL VARIABLE
 *	binFileBO		-  current file byte order
 *	binReadMBFDisp		-  array of MBF handling functions
 *	binReadLBFDisp		-  array of LBF handling functions
 *	binReadDisp		-  pointer to the current read function array
 *
 *	binWriteMBFDisp		-  array of MBF handling functions
 *	binWriteLBFDisp		-  array of LBF handling functions
 *	binWriteDisp		-  pointer to the current write function array
 *
 *  DESCRIPTION
 *	The output file will be read or written assuming the byte order
 *	is that selected by binFileBO.  The programmer may call BinByteOrder
 *	to change that byte order.
 *
 *	The read and write dispatch tables point to functions to read or
 *	write files in the current byte order.
 *
 *		** MBF and LBF here refer to the byte order **
 *		** of data in the file, not on the host!    **
 *
 *	Each of the read functions read in 'nBytes' bytes from the file, then
 *	save the least-significant bytes into the user's buffer.  When file
 *	data is in MBF form (the default), the least-significant bytes are
 *	the last ones read in.  When file data is in LBF form, the least-
 *	significant bytes are the first ones read in.
 *
 *	Each of the write functions write the 'nBytes' least-significant bytes
 *	of items in the user's buffer to the file.  When file data is to be in
 *	MBF form, the least-significant bytes are the last ones written out.
 *	When file data is to be in LBF form, the least-significant bytes are
 *	the first ones written out.
 *
 *	What's the significance of reading and writing only the 'nBytes'
 *	least-significant bytes?  Consider writing out a 32-bit integer that
 *	is stored in a 64-bit word.  For numbers that take 32-bits or less
 *	to represent, it is the lower 32-bits of the 64-bit word that hold
 *	the value.  For numbers that take more than 32-bits, the number must be
 *	truncated in order to write it out in 32-bits.  Truncation traditionally
 *	happens from the top of the word.  So, writing the least-significant
 *	'nBytes' of each incomming data item handles both cases.  The
 *	situation is similar for reading.
 */

static int binFileBO = BINMBF;	/* File data byte order			*/

#ifdef __STDC__
static int (*binReadMBFDisp[])(int , int , FILE*, unsigned char *, unsigned char *, int ,int , int) =
#else
static int (*binReadMBFDisp[])() =
#endif
{
	binReadIntMBF,
	binReadCharMBF,
	binReadCharMBF,
	binReadIntMBF,
	binReadIntMBF,
	binReadIntMBF,
	binReadIntMBF,
	binReadIntMBF,
	binReadIntMBF,
	binReadIntMBF,
	binReadIntMBF,
	binReadIntMBF,
	binReadIntMBF,
	binReadIntMBF,
	binReadIntMBF,
	binReadIntMBF,
	binReadFloat,
	binReadFloat,
#if LONG_SIZE >= 64
	binReadIntMBF,
	binReadIntMBF,
#else
	NULL,
	NULL,
#endif
};
 
#ifdef __STDC__
static int (*binReadLBFDisp[])(int , int , FILE*, unsigned char *, unsigned char *, int ,int , int) =
#else
static int (*binReadLBFDisp[])() =
#endif
{
	binReadIntLBF,
	binReadCharLBF,
	binReadCharLBF,
	binReadIntLBF,
	binReadIntLBF,
	binReadIntLBF,
	binReadIntLBF,
	binReadIntLBF,
	binReadIntLBF,
	binReadIntLBF,
	binReadIntLBF,
	binReadIntLBF,
	binReadIntLBF,
	binReadIntLBF,
	binReadIntLBF,
	binReadIntLBF,
	binReadFloat,
	binReadFloat,
#if LONG_SIZE >= 64
	binReadIntLBF,
	binReadIntLBF,
#else
	NULL,
	NULL,
#endif
};
 
#ifdef __STDC__
static int (*binWriteMBFDisp[])(int , int , FILE*, unsigned char *, unsigned char *, int ,int , int) =
#else
static int (*binWriteMBFDisp[])() =
#endif
{
	binWriteCharMBF,
	binWriteCharMBF,
	binWriteCharMBF,
	binWriteIntMBF,
	binWriteIntMBF,
	binWriteIntMBF,
	binWriteIntMBF,
	binWriteIntMBF,
	binWriteIntMBF,
	binWriteIntMBF,
	binWriteIntMBF,
	binWriteIntMBF,
	binWriteIntMBF,
	binWriteIntMBF,
	binWriteIntMBF,
	binWriteIntMBF,
	binWriteFloat,
	binWriteFloat,
#if LONG_SIZE >= 64
	binWriteIntMBF,
	binWriteIntMBF,
#else
	NULL,
	NULL,
#endif
};
 
#ifdef __STDC__
static int (*binWriteLBFDisp[])(int , int , FILE*, unsigned char *, unsigned char *, int ,int , int) =
#else
static int (*binWriteLBFDisp[])() =
#endif
{
	binWriteCharLBF,
	binWriteCharLBF,
	binWriteCharLBF,
	binWriteIntLBF,
	binWriteIntLBF,
	binWriteIntLBF,
	binWriteIntLBF,
	binWriteIntLBF,
	binWriteIntLBF,
	binWriteIntLBF,
	binWriteIntLBF,
	binWriteIntLBF,
	binWriteIntLBF,
	binWriteIntLBF,
	binWriteIntLBF,
	binWriteIntLBF,
	binWriteFloat,
	binWriteFloat,
#if LONG_SIZE >= 64
	binWriteIntLBF,
	binWriteIntLBF,
#else
	NULL,
	NULL,
#endif
};

#ifdef __STDC__
static int (**binReadDisp)(int , int , FILE*, unsigned char *, unsigned char *, int ,int , int)  
	= binReadMBFDisp;
static int (**binWriteDisp)(int , int , FILE*, unsigned char *, unsigned char *, int ,int , int) 
	= binWriteMBFDisp;
#else
static int (**binReadDisp)()  = binReadMBFDisp;
static int (**binWriteDisp)() = binWriteMBFDisp;
#endif





/*
 *  Host's floating point type.
 */
#if FLOAT_TYPE == IEEE_FLOAT
#define BINHOSTFLOAT	BINIEEE
#endif

#if FLOAT_TYPE == VAX_FLOAT
#define BINHOSTFLOAT	BINVAX
#endif

#if FLOAT_TYPE == VAXG_FLOAT
#define BINHOSTFLOAT	BINVAXG
#endif

#if FLOAT_TYPE == CRAYMP_FLOAT
#define BINHOSTFLOAT	BINCRAYMP
#endif


#ifndef BINHOSTFLOAT
/*
 *  Crash and Burn:
 *	If the host's floating type is unknown, generate a syntax error
 *	to bring the programmer here...
 */
<<<<--+=+ Syntax Error +=+-->>>>	/* Read message below!!!	*/

/*
 *  Programmer!  This syntax error was generated because the floating point
 *  type used by this host is currently unknown by this package.  This
 *  prevents the functioning of code to convert from the host's floating
 *  point format to that of another machine.
 *
 *  Please read the comments describing the 'binFloatFormatInfo' struct and the
 *  initialization of the 'binFloatFormats' array below to learn about
 *  floating point formats and how to set them up for this host.  Please do
 *  not simply default them to something you think is right.  Check and make
 *  sure!  Bugs arising from faulty defaulting can be very hard to find.
 *
 *  When you have a format defined for this host, please forward it to us for
 *  inclusion in a future release of this software.  Thanks!
 */
#endif





/*
 *  STRUCTURE & TYPEDEF
 *	binFloatFormatInfo	-  floating point format description
 *
 *  DESCRIPTION
 *	The binFloatFormatInfo struct describes the attributes of a floating
 *	point format.  The values provided make it possible to generically
 *	convert a float from one format to another.
 *
 *	Floating point numbers have three pieces:
 *		sign		positive or negative flag
 *		exponent	the power to raise the mantissa to
 *		mantissa	the fractional part of the float
 *
 *	The sign is a 1-bit value that indicates if the floating point value
 *	is positive or negative.  Negative floats set the sign bit to a 1.
 *
 *	The exponent is the power to which the float is raised.  The exponent
 *	is "biased" to make it a positive value.  For instance, in IEEE
 *	single-precision, the exponent is 8-bits wide and its bias is 127.
 *	An unbiased 8-bit value can represent a signed value between -127 and
 *	+128, or an unsigned value from 0 to 255.  Biasing the exponent makes
 *	it always positive (unsigned) in representation, yet still allows
 *	negative exponents.  So, with an IEEE bias of 127, the exponent
 *	-10 would be represented as:  -10 + 127 = 117.
 *
 *	The mantissa is the fractional part of the float.  When normallized,
 *	the upper bit of the mantissa is always a 1.  Some formats keep this
 *	1 bit in the float, while others remove it (calling it an implied
 *	bit).
 *
 *	The binFloatFormatInfo struct uses the following fields to describe a
 *	floating point format:
 *
 *		bin_name
 *			The character string name of the format.
 *
 *		bin_number
 *			The #define number associated with this format.
 *
 *		bin_subname
 *			The name of this size of the float.
 *
 *		bin_expBits
 *			The number of bits in the exponent.
 *
 *		bin_mantBits
 *			The number of bits in the mantissa.
 *
 *		bin_expBias
 *			The exponent bias.
 *
 *		bin_mantImplied
 *			Is the implied bit kept in, or removed?
 *
 *		bin_signOverflow
 *			Whether the sign bit is set or not on an overflow.
 *
 *		bin_expOverflow
 *			The value of the exponent on an overflow.
 *
 *		bin_flags
 *			Flag special "exception" conditions in the format:
 *				BINVAXBO	Bizzare VAX float byte order
 *
 *  GENERAL ASSUMPTIONS
 *	The order of fields in a float is:
 *		On MBF systems:
 *			low		  high
 *			sign exponent mantissa
 *		On LBF systems:
 *			low		  high
 *			mantissa exponent sign
 *		Except when BINVAXBO is set in bin_flags, in which case bytes
 *		are swapped every-other to take into account of the VAX's
 *		strange byte ordering of floating point values.
 *
 *	The sign is 1 bit.
 *
 *	There are no unused bits in the float.
 *
 *	(bin_expBits + bin_mantBits + 1) % 8 == 0.
 *
 *	The float is normalized.
 *		
 *  SIGN ASSUMPTIONS
 *	If the sign is a 1, the float is negative.
 *
 *	The sign bit is 0 when the float is 0.
 *
 *  EXPONENT ASSUMPTIONS
 *	The exponent is representable as a signed 32-bit integer on the host.
 *
 *	The exponent is zero when the float is zero.
 *
 *  MANTISSA ASSUMPTIONS
 *	The mantissa is zero when the float is zero.
 *
 *	The mantissa is zero on an overflow.
 *
 *  NON-ASSUMPTIONS (ie, "It is **NOT** assumed that...")
 *	The mantissa is storeable in an integer on the host.
 *
 *	The # of bits in the exponent or mantissa are modulo 8 or that
 *	the exponent starts on a byte boundary.
 *
 *	The redundant high bit of the mantissa on a normalized float is dropped.
 */

typedef struct binFloatFormatInfo
{
	char *bin_name;		/* Name of format			*/
				/*   NULL flags end of list		*/
	int   bin_number;	/* Format number			*/
	char *bin_subname;	/* Name of this size of the format	*/
				/*   NULL flags unusable entry		*/
	int   bin_expBits;	/* Number of bits in exponent		*/
	int   bin_mantBits;	/* Number of bits in mantissa		*/
	int   bin_expBias;	/* Exponent bias			*/
	int   bin_mantImplied;	/* Is implied bit saved in format?	*/
	int   bin_signOverflow;	/*Is sign bit set on overflow?		*/
	unsigned int bin_expOverflow;/* Exponent value on overflow	*/
	int   bin_flags;	/* Excpetion flags			*/
} binFloatFormatInfo;





/*
 *  CONSTANTS
 *	BINVAXBO	-  Bizzare VAX floating point byte order exception
 */

#define BINVAXBO	0x1
		/*	0x2-0x800000	Reserved			*/





/*
 *  GLOBAL VARIABLE
 *	binFloatFormats	-  List of supported floating point formats
 *
 *  DESCRIPTION
 *	The binFloatFormats array of binFloatFormatInfo structs lists the
 *	attributes of each of the floating point formats supported.
 *
 *  TO ADD A NEW FORMAT
 *	1.  Create one or more entries in the array initilaization below.
 *
 *	2.  Make up a name for the format and create a #define BINname in
 *	    bin.raw.h.
 *
 *	3.  Add an #if FLOAT_TYPE for the format to define the BINHOSTFLOAT
 *	    constant on hosts that support that float (the #if's are just
 *	    before the description of the BinFloatFormat struct).
 *
 *	NOTE:  The list is terminated by a NULL pointer in bin_name.
 *	Additionally, a NULL pointer for bin_subname flags that entry as
 *	unusable.
 *
 *	NOTE NOTE:  Different precisions of the same format must be grouped
 *	together!
 */

#ifndef YES
#define YES	1
#endif

#ifndef NO
#define	NO	0
#endif

binFloatFormatInfo binFloatFormats[ ] =
{

/*				      # bits         overflow   	*/
/* name     number     subname  exp mant   bias imp? sgn?  exp     flags*/
{ "IEEE",   BINIEEE,   "single",  8,  23,   127,  NO, YES, 0x0FF,  0 },
{ "IEEE",   BINIEEE,   "double", 11,  52,  1023,  NO, YES, 0x7FF,  0 },
{ "CRAYMP", BINCRAYMP, "single", 15,  48, 16384, YES,  NO, 0x6000, 0 },
{ "VAX",    BINVAX,    "f",       8,  23,   129,  NO, YES, 0,      BINVAXBO },
{ "VAX",    BINVAX,    "d",       8,  55,   129,  NO, YES, 0,      BINVAXBO },
{ "VAX",    BINVAX,    "h",      15, 112, 16385,  NO, YES, 0,      BINVAXBO },
{ "VAXG",   BINVAXG,   "f",       8,  23,   129,  NO, YES, 0,      BINVAXBO },
{ "VAXG",   BINVAXG,   "g",      11,  52,  1025,  NO, YES, 0,      BINVAXBO },
{ "VAXG",   BINVAXG,   "h",      15, 112, 16385,  NO, YES, 0,      BINVAXBO },
#ifdef notdef
{ "IBM",    BINIBM,    "f",       7, 24,     63,  NO, YES, 0,      0 },
#endif
{ NULL,     -1,        NULL,      0,   0,     0,  NO,  NO, 0,      0 },

};





/*
 *  STRUCTURE & TYPEDEF
 *	binFloatFormatCooked	-  cooked floating point format description
 *
 *  DESCRIPTION
 *	The binFloatFormatCooked struct describes the attributes of a floating
 *	point format (as does the BinFloatFormat struct), but also includes
 *	some pre-calculated byte counts, shifts, and masks that help speed
 *	up the conversion algorithm.
 *
 *	The binFloatFormatCooked struct uses these fields to describe the
 *	floating point format:
 *
 *		bin_format
 *			A copy of the data from the binFloatFormatInfo struct.
 *
 *		bin_nbytes
 *			The number of bytes it takes to store this float.
 *
 *		bin_expStartByte
 *		bin_expEndByte
 *			The byte numbers of the first and last bytes
 *			containing exponent bits.
 *
 *		bin_expStartMask
 *		bin_expEndMask
 *			An 8-bit mask to extract only the relevant exponent
 *			bits from the first and last bytes of the exponent.
 *
 *		bin_expShift
 *			The number of bits in the starting byte NOT used
 *			for the exponent (ie, how far to shift the starting
 *			byte left to bring the 1st exponent bit into the
 *			1st bit of the byte).
 *
 *		bin_expMask
 *			A 32-bit mask to apply to an unsigned integer that
 *			will leave only the exponent bits.
 */

typedef struct binFloatFormatCooked
{
	binFloatFormatInfo bin_format;	/* Raw format			*/
	int    bin_nbytes;		/* Size, in bytes, of this entry*/

	int    bin_expStartByte;	/* Byte containing exponent start*/
	int    bin_expEndByte;		/* Byte containing exponent end	*/

	unsigned char bin_expStartMask;	/* Mask for starting exponent byte*/
	unsigned char bin_expEndMask;	/* Mask for ending exponent byte*/
	int    bin_expShift;		/* Shift for starting exponent byte*/
	unsigned int bin_expMask;	/* Mask to just get relevant bits*/
} binFloatFormatCooked;





/*
 *  GLOBAL VARIABLE
 *	binFloatFormatsCooked	-  cooked floating point format descriptions
 *
 *  DESCRIPTION
 *	The binFloatFormatsCooked array gives the "cooked" (after pre-calc)
 *	version of the data in the binFloatFormats array.  It is the
 *	cooked version of the data that is actually used by the floating
 *	point conversion code.
 *
 *	This array is initialized at startup by binInitialize().
 */
binFloatFormatCooked binFloatFormatsCooked[sizeof(binFloatFormats)/sizeof(binFloatFormatInfo)];





/*
 *  GLOBAL VARIABLE
 *	binHostFloatFormat	-  Host floating point format description
 *	binFileFloatFormat	-  File floating point format description
 *
 *  DESCRIPTION
 *	binHostFloatFormat points to the set of floating point format
 *	descriptions appropriate for the host.
 *
 *	binFileFloatFormat points to the set of floating point format
 *	descriptions selected for the file.
 *
 *	Both of these globals are initialized at startup by binInitialize().
 */
binFloatFormatCooked *binFileFloatFormat = NULL;
binFloatFormatCooked *binHostFloatFormat = NULL;





/*
 *  GLOBAL VARIABLE
 *	binOp			-  Current operation
 *
 *  DESCRIPTION
 *	Each of the top level read or write functions set binOp to the
 *	operation type being done so that, should an error occur, the
 *	user's error handler can be told what operation was in progress.
 */
static int binOp;		/* Current operation (for error handling)*/



#ifdef __STDC__
static int	binConvertFloat( int, binFloatFormatCooked *, int, int, unsigned char *, binFloatFormatCooked *, int, int, unsigned char *, int );
#else
static int	binConvertFloat( );
#endif





/*
 *  FUNCTION
 *	binInit		-  tables initialized yet?
 *	binInitialize	-  Initialize the tables
 *
 *  DESCRIPTION
 *	This routine initializes the tables:
 *		binTypeRes	-  type resolution
 *		binTypePad	-  type structure field padding
 *		binTypePadMask	-  type padding address masks
 *		binFloatFormatsCooked -  cooked float information
 *
 *	determines the floating point type of the host:
 *		binHostFloatFormat	-  host format selection
 *
 *	and the default floating point type for the file:
 *		binFileFloatFormat	-  file format selection
 *
 *	The resolution table indicates the number of bytes in a type that
 *	are actually available for storing data, as opposed to the number
 *	of bytes the type occupies in memory.  On most machines these
 *	two values are the same.  A notible exception is the CRAY's 'short'
 *	which occupies 8 bytes in memory, but can only hold a 3 byte value.
 *
 *	The padding table indicates how the various types are padded when
 *	they occur as fields in a structure.  To initialize the table we
 *	compute the difference between a char structure field's address
 *	and the address of a field of the desired type.  If padding has
 *	taken place, the difference will be greater than 1 byte.
 *
 *	Many machines require that all integer or floating point accesses
 *	be made at boundaries that are a multiple of the item size.  For
 *	instance, on a SPARCstation all 32-bit integers must be on 32-bit
 *	boundaries, and all 16-bit integers on 16-bit boundaries.  Accesses
 *	that are not word aligned cause a bus error.
 *
 *	Compilers for machines that do not require word alignment may still
 *	do structure padding, such as the VAX ULTRIX compiler.
 *
 *	Still other compilers do no structure padding, such as the VAX
 *	VMS compiler.
 *
 *	The Cooked formats table includes the basic floating point format
 *	information from binFloatFormats[], plus various masks and byte
 *	counts to make floating point conversions easier and quicker.
 */

static int binInit = FALSE;

static void			/* Returns nothing			*/
#ifdef __STDC__
binInitialize( void )
#else
binInitialize( )
#endif
{
	int *br = binTypeRes;	/* Pointer to Res table			*/
	int *bp = binTypePad;	/* Pointer to Pad table			*/
	unsigned long charbit;	/* Address bit for byte addressing	*/
	unsigned long shiftbit;	/* How far to shift to get to charbit	*/
	int i;			/* Counter				*/
	binFloatFormatInfo *fp;	/* Float format table pointer		*/
	binFloatFormatCooked *fcp;/* Cooked format table pointer	*/
	int nbits;		/* Number of extra bits in byte		*/
	binPadStructpointer	structpointer;
	binPadStructchar	structchar;
	binPadStructuchar	structuchar;
	binPadStructshort	structshort;
	binPadStructlong	structlong;
	binPadStructint		structint;
	binPadStructint8	structint8;
	binPadStructint16	structint16;
	binPadStructint32	structint32;
	binPadStructushort	structushort;
	binPadStructulong	structulong;
	binPadStructuint	structuint;
	binPadStructuint8	structuint8;
	binPadStructuint16	structuint16;
	binPadStructuint32	structuint32;
	binPadStructboolean	structboolean;
	binPadStructfloat	structfloat;
	binPadStructdouble	structdouble;
#if LONG_SIZE >= 64
	binPadStructint64	structint64;
	binPadStructuint64	structuint64;
#endif

	/*
	 *  Initialize the resolution table.
	 */

#define DEF(type)			\
{					\
	type var = ((type)1)<<(8*sizeof(type)-2);\
	var <<= 1;			\
	if ( var != ((type)0) )		\
		*br++ = sizeof(type);	\
	else				\
	{				\
		int ii = 1;		\
		for ( var = ((type)1); var != ((type)0); var <<= 1 )\
			ii++;		\
		*br++ = ii>>3;		\
	}				\
}
	*br++ = 0;	/* skip pointer */
	DEF(char)
	DEF(unsigned char)
	DEF(short)
	DEF(long)
	DEF(int)
	DEF(sdsc_int8)
	DEF(sdsc_int16)
	DEF(sdsc_int32)
	DEF(unsigned short)
	DEF(unsigned long)
	DEF(unsigned int)
	DEF(sdsc_uint8)
	DEF(sdsc_uint16)
	DEF(sdsc_uint32)
	DEF(sdsc_boolean)
	*br++ = 0;	/* skip float */
	*br++ = 0;	/* skip double */
#if LONG_SIZE >= 64
	DEF(sdsc_int64)
	DEF(sdsc_uint64)
#else
	*br++ = 0;
	*br++ = 0;
#endif
 
#undef DEF


	/*
	 *  Initialize the structure padding table.
	 */
	*bp++ = (char *)&structpointer.bin_i-&structpointer.bin_c;
	*bp++ = (char *)&structchar.bin_i-&structchar.bin_c;
	*bp++ = (char *)&structuchar.bin_i-&structuchar.bin_c;
	*bp++ = (char *)&structshort.bin_i-&structshort.bin_c;
	*bp++ = (char *)&structlong.bin_i-&structlong.bin_c;
	*bp++ = (char *)&structint.bin_i-&structint.bin_c;
	*bp++ = (char *)&structint8.bin_i-&structint8.bin_c;
	*bp++ = (char *)&structint16.bin_i-&structint16.bin_c;
	*bp++ = (char *)&structint32.bin_i-&structint32.bin_c;
	*bp++ = (char *)&structushort.bin_i-&structushort.bin_c;
	*bp++ = (char *)&structulong.bin_i-&structulong.bin_c;
	*bp++ = (char *)&structuint.bin_i-&structuint.bin_c;
	*bp++ = (char *)&structuint8.bin_i-&structuint8.bin_c;
	*bp++ = (char *)&structuint16.bin_i-&structuint16.bin_c;
	*bp++ = (char *)&structuint32.bin_i-&structuint32.bin_c;
	*bp++ = (char *)&structboolean.bin_i-&structboolean.bin_c;
	*bp++ = (char *)&structfloat.bin_i-&structfloat.bin_c;
	*bp++ = (char *)&structdouble.bin_i-&structdouble.bin_c;
#if LONG_SIZE >= 64
	*bp++ = (char *)&structint64.bin_i-&structint64.bin_c;
	*bp++ = (char *)&structuint64.bin_i-&structuint64.bin_c;
#else
	*bp++ = -1;
	*bp++ = -1;
#endif


	/*
	 *  Initialize the structure padding mask table.
	 */
	charbit = (unsigned long)((char *)&i + 1) - (unsigned long)(char *)&i;
	for ( shiftbit = ((unsigned long)(-1)); charbit != 0; shiftbit++ )
		charbit >>= 1;
	for ( i = 0; i < BINMAXTYPE; i++ )
		binTypePadMask[i] = ((unsigned long)(binTypePad[i]-1))<<shiftbit;


	/*
	 *  Initialize the cooked floating point format table.
	 */
	for ( fp = binFloatFormats, fcp = binFloatFormatsCooked;
		fp->bin_name != NULL; fp++, fcp++ )
	{
		fcp->bin_format = *fp;
		fcp->bin_nbytes = (fp->bin_expBits + fp->bin_mantBits + 1)/8;

		if ( (fp->bin_expBits + fp->bin_mantBits + 1) % 8 != 0 )
		{
			/* Illegal entry.				*/
			fcp->bin_format.bin_subname = NULL;
			continue;
		}

		fcp->bin_expStartByte = fp->bin_mantBits / 8;
		fcp->bin_expShift     = nbits = fp->bin_mantBits % 8;
		fcp->bin_expStartMask = (unsigned char) (((unsigned long)0xFF) << nbits);

		/* Assume last byte is end byte and last bit is sign.	*/
		fcp->bin_expEndByte = fcp->bin_nbytes - 1;
		fcp->bin_expEndMask = (unsigned char)((unsigned long)0x7F);
		fcp->bin_expMask = (unsigned int) (~( (~((unsigned long)0)) << fp->bin_expBits));
	}


	/*
	 *  Initialize the host and file floating point format selections.
	 */
	for ( fcp = binFloatFormatsCooked; fcp->bin_format.bin_name != NULL; fcp++ )
	{
		if ( fcp->bin_format.bin_number == BINHOSTFLOAT )
		{
			binHostFloatFormat = fcp;
			break;
		}
	}
	if ( binFileFloatFormat == NULL )
	{
		for ( fcp = binFloatFormatsCooked; fcp->bin_format.bin_name != NULL; fcp++ )
		{
			if ( fcp->bin_format.bin_number == BINIEEE )
			{
				binFileFloatFormat = fcp;
				break;
			}
		}
	}

	binErrFunc = binDefaultHandler;

	binInit = TRUE;
}





/*
 *  FUNCTION
 *	BinQMachine	-  query machine characteristics
 *
 *  DESCRIPTION
 *	The tables are initialized, if not done already, and a static
 *	BinMachineInfo struct set up.  A pointer to the info is returned.
 */

static BinMachineInfo binMachine;	/* Machine info struct		*/


BinMachineInfo *			/* Returns ptr to info struct	*/
#ifdef __STDC__
BinQMachine( void )
#else
BinQMachine( )
#endif
{
	if ( !binInit )
		binInitialize( );

	memset( (void *)&binMachine, 0x00, sizeof (binMachine) );

#ifdef VENDOR_NAME
	binMachine.bin_vendorName = VENDOR_NAME;
#else
	binMachine.bin_vendorName = "Unknown";
#endif

#ifdef MACHINE_NAME
	binMachine.bin_machineName = MACHINE_NAME;
#else
	binMachine.bin_machineName = "Unknown";
#endif

#ifdef CPU_NAME
	binMachine.bin_cpuName = CPU_NAME;
#else
	binMachine.bin_cpuName = "Unknown";
#endif

#ifdef OS_NAME
	binMachine.bin_osName = OS_NAME;
#else
	binMachine.bin_osName = "Unknown";
#endif

	binMachine.bin_byteOrder = BINHOSTBO;
#ifdef MBF
	binMachine.bin_byteOrderName = "MBF";
#else
	binMachine.bin_byteOrderName = "LBF";
#endif

	binMachine.bin_floatFormat= BINHOSTFLOAT;
	binMachine.bin_floatFormatName = binHostFloatFormat->bin_format.bin_name;

	binMachine.bin_typeSize   = binTypeSize;
	binMachine.bin_typeSigned = binTypeSigned;
	binMachine.bin_typeRes    = binTypeRes;
	binMachine.bin_typePad    = binTypePad;
	binMachine.bin_typeName   = binTypeName;
	binMachine.bin_typePadMask= binTypePadMask;

	return ( &binMachine );
}





/*
 *  FUNCTION
 *	BinPError	-  Print error message
 *
 *  DESCRIPTION
 *	The error text associated with the current BinErrNo is printed
 *	to stderr, preceded by the given leader string.
 */

void				/* Returns nothing			*/
#ifdef __STDC__
BinPError( char *s )
#else
BinPError( s )
	char *s;		/* Leader string			*/
#endif
{
	if ( BinErrNo == BINESYS )
		perror( s );
	else if ( BinErrNo < 0 || BinErrNo >= BinNErr )
		(void)fprintf( stderr, "Unknown error\n" );
	else if ( s && *s )
		(void)fprintf( stderr, "%s: %s\n", s, BinErrList[BinErrNo] );
	else
		(void)fprintf( stderr, "%s\n", BinErrList[BinErrNo] );
}





/*
 *  FUNCTION
 *	BinQError	-  Query error message
 *
 *  DESCRIPTION
 *	The error text associated with the current BinErrNo is returned.
 */

extern int errno;			/* System call error code	*/
extern int sys_nerr;			/* # of system call error codes	*/
extern char *sys_errlist[];		/* Error code message strings	*/

char *					/* Returns error text		*/
#ifdef __STDC__
BinQError( void )
#else
BinQError( )
#endif
{
	if ( BinErrNo == BINESYS )
	{
		if ( errno < 0 || errno >= sys_nerr )
			return ( "Unknown error" );
		return ( sys_errlist[errno] );
	}
	if ( BinErrNo < 0 || BinErrNo >= BinNErr )
		return ( "Unknown error" );
	return ( BinErrList[BinErrNo] );
}





/*
 *  FUNCTION
 *	BinErrorHandler		-  nominate an error condition handler func
 *	binDefaultHandler	-  Default error condition handler
 *	binNoHandler		-  No error condition handler
 *
 *  DESCRIPTION
 *	On errors in integer and floating point format conversion we call the
 *	nominated error condition callback function.  By default it is set to
 *	our own stderr printing function.  The user may override this by
 *	nominating his/her own function.
 */

void					/* Returns nothing		*/
#ifdef __STDC__
BinErrorHandler( int (*func)(int , int , int , void *, int , int ) )
#else
BinErrorHandler( func )
	int (*func)();			/* Function to nominate		*/
#endif
{
	if ( func == BINDEFFUNC )
		binErrFunc = binDefaultHandler;
	else if ( func == BINNOFUNC )
		binErrFunc = binNoHandler;
	else
		binErrFunc = func;
	return;
}

static int
#ifdef __STDC__
binNoHandler( int fd, int op, int reason, void *data, int frombits, int tobits )
#else
binNoHandler( fd, op, reason, data, frombits, tobits )
	int fd;				/* File descriptor		*/
	int op;				/* Current operation		*/
	int reason;			/* Error reason code		*/
	unsigned char *data;		/* Data holding bad value	*/
	int frombits;			/* Max # bits in type comming from*/
	int tobits;			/* Max # bits in type going to	*/
#endif
{
	return ( 0 );
}

static int				/* Returns nothing		*/
#ifdef __STDC__
binDefaultHandler( int fd, int op, int reason, void *data, int frombits, int tobits )
#else
binDefaultHandler( fd, op, reason, data, frombits, tobits )
	int fd;				/* File descriptor		*/
	int op;				/* Current operation		*/
	int reason;			/* Error reason code		*/
	unsigned char *data;		/* Data holding bad value	*/
	int frombits;			/* Max # bits in type comming from*/
	int tobits;			/* Max # bits in type going to	*/
#endif
{
	char opname[100];		/* Operation name		*/
	int  nBytes;			/* Number of bytes		*/
	unsigned char *pData;		/* Data byte pointer		*/

	switch ( op )
	{
	case BINOPREAD:
		(void)sprintf( opname, "read of file %d", fd );
		break;
	case BINOPWRITE:
		(void)sprintf( opname, "write of file %d", fd );
		break;
	case BINOPREADSTRUCT:
		(void)sprintf( opname, "structure read of file %d", fd );
		break;
	case BINOPWRITESTRUCT:
		(void)sprintf( opname, "structure write of file %d", fd );
		break;
	case BINOPCONVERTFLOAT:
		(void)sprintf( opname, "floating point conversion" );
		break;
	default:
		(void)sprintf( opname, "unknown operation" );
		break;
	}

	nBytes = frombits / 8;
	if ( frombits % 8 != 0 )
		nBytes++;

	pData = (unsigned char *)data;
	switch ( reason )
	{
	case BINEINTEGERTRUNC:	/* Integer truncation occurred		*/
		(void)fprintf( stderr, "Bin:  integer truncation during %s\n",
			opname );
		(void)fprintf( stderr, "  %d bit integer truncated on storage into %d bit integer\n",
			frombits, tobits );
		(void)fprintf( stderr, "  Pre-truncation integer = 0x" );
		do
			(void)fprintf( stderr, "%02x", (int)*(pData++) );
		while ( --nBytes );
		(void)fprintf( stderr, "\n" );
		return ( 0 );

	case BINEFLOATOVERFLOW:	/* Floating point overflow occurred	*/
		(void)fprintf( stderr, "Bin:  floating point overflow during %s\n",
			opname );
		(void)fprintf( stderr, "  %d bit exponent overflows when reduced to %d bits\n",
			frombits, tobits );
		(void)fprintf( stderr, "  Pre-overflow float = 0x" );
		do
			(void)fprintf( stderr, "%02x", (int)*(pData++) );
		while ( --nBytes );
		(void)fprintf( stderr, "\n" );
		return ( 0 );

	case BINEFLOATUNDERFLOW:/* Floating point underflow occurred	*/
		(void)fprintf( stderr, "Bin:  floating point underflow during %s\n",
			opname );
		(void)fprintf( stderr, "  %d bit exponent underflows when reduced to %d bits\n",
			frombits, tobits );
		(void)fprintf( stderr, "  Pre-underflow float = 0x" );
		do
			(void)fprintf( stderr, "%02x", (int)*(pData++) );
		while ( --nBytes );
		(void)fprintf( stderr, "\n" );
		return ( 0 );

	default:		/* Unknown!?				*/
		(void)fprintf( stderr, "Bin:  unknown error has occurred during %s\n",
			opname );
		return ( 0 );
	}
}





/*
 *  FUNCTION
 *	BinByteOrder	-  Select the byte order of the file
 *	BinQByteOrder	-  Query the byte order of the file
 *
 *  DESCRIPTION
 *	The byte order of the data in the file must be known in order to
 *	handle conversion of data to the byte order of the host.  The
 *	default is MBF (Most-Significant Byte First).
 *
 *	BinByteOrder sets the byte order and returns -1 or 0.
 *
 *	BinQByteOrder queries the byte order.
 */

int				/* Returns status			*/
#ifdef __STDC__
BinByteOrder( int which )
#else
BinByteOrder( which )
	int which;		/* Which byte order to use		*/
#endif
{
	if ( !binInit )
		binInitialize( );

	switch ( which )
	{
	case BINMBF:
		binFileBO    = BINMBF;
		binReadDisp  = binReadMBFDisp;
		binWriteDisp = binWriteMBFDisp;
		return ( 0 );

	case BINLBF:
		binFileBO    = BINLBF;
		binReadDisp  = binReadLBFDisp;
		binWriteDisp = binWriteLBFDisp;
		return ( 0 );

	default:
		BINERROR( BINEBYTEORDER, -1 );
	}
}

int				/* Returns current byte order		*/
#ifdef __STDC__
BinQByteOrder( void )
#else
BinQByteOrder( )
#endif
{
	if ( !binInit )
		binInitialize( );

	return ( binFileBO );
}





/*
 *  FUNCTION
 *	BinFloatFormat	-  Select the floating point format of the file
 *	BinQFloatFormat	-  Query the floating point format of the file
 *
 *  DESCRIPTION
 *	The floating format for data in the file must be known in order to
 *	handle conversion of data to the float format of the host.  The
 *	default is IEEE.
 *
 *	BinFloatFormat sets the floating point format and returns -1 or 0.
 *
 *	BinQFloatFormat queries the floating point format.
 */

int				/* Returns status			*/
#ifdef __STDC__
BinFloatFormat( int which )
#else
BinFloatFormat( which )
	int which;		/* Which floating point format to use	*/
#endif
{
	binFloatFormatCooked *fcp;/* Cooked table pointer		*/

	if ( !binInit )
		binInitialize( );

	for ( fcp = binFloatFormatsCooked; fcp->bin_format.bin_name != NULL; fcp++ )
		if ( fcp->bin_format.bin_number == which )
			break;
	if ( fcp->bin_format.bin_name == NULL )
		BINERROR( BINEFLOATFORMAT, -1 );	/* Unknown	*/
	binFileFloatFormat = fcp;
	return ( 0 );
}

int				/* Returns current float format		*/
#ifdef __STDC__
BinQFloatFormat( void )
#else
BinQFloatFormat( )
#endif
{
	if ( !binInit )
		binInitialize( );

	return ( binFileFloatFormat->bin_format.bin_number );
}





#define BINCHECKRTYPE(type)						\
	if ( (type) <= 0 || (type) > BINMAXTYPE || binReadDisp[(type)] == NULL)\
		BINERROR( BINETYPE, -1 );


#define BINCHECKWTYPE(type)						\
	if ( (type) <= 0 || (type) > BINMAXTYPE || binWriteDisp[(type)] ==NULL)\
		BINERROR( BINETYPE, -1 );


#define BINCHECKNBYTES(nBytes)						\
	if ( (nBytes) <= 0 )						\
		BINERROR( BINENBYTES, -1 );


#define BINCHECKCOUNT(count)						\
	if ( (count) == 0 )						\
		return ( 0 );						\
	if ( (count) < 0 )						\
		BINERROR( BINECOUNT, -1 );





/*
 *  FUNCTION
 *	BinRead		-  Read binary file using unbuffered I/O
 *	BinFRead	-  Read binary file using buffered I/O
 *	BinSRead	-  Read binary string
 *	BinWrite	-  Write binary file using unbuffered I/O
 *	BinFWrite	-  Write binary file using buffered I/O
 *	BinSWrite	-  Write binary string
 *
 *  DESCRIPTION
 *	The incomming arguments are checked for legality.  If any are
 *	bad, a -1 is returned and the global error variable 'BinErrNo'
 *	set to an appropriate error code.
 *
 *	Based on the user's 'type', an appropriate read or write function is
 *	called through the read or write dispatch tables.  The dispatch tables
 *	point to LBF or MBF functions, as selected by an earlier user
 *	call to BinByteOrder.
 */

int					/* Returns # of bytes read	*/
#ifdef __STDC__
BinRead( int fd, void *buf, int type, int nBytes, int count )
#else
BinRead( fd, buf, type, nBytes, count )
	int fd;				/* File descriptor		*/
	unsigned char *buf;		/* Data buffer			*/
	int type;			/* Buffer variable's type	*/
	int nBytes;			/* # of bytes when in file	*/
	int count;			/* # of items to read		*/
#endif
{
#ifdef __STDC__
	int (*dispatch)( int, int, FILE *, unsigned char *, unsigned char *,
		int, int, int );	/* Dispatch table entry		*/
#else
	int (*dispatch)(  );	/* Dispatch table entry		*/
#endif
	BINCHECKRTYPE(  type );
	BINCHECKNBYTES( nBytes );
	BINCHECKCOUNT(  count );

	if ( !binInit )
		binInitialize( );

	binOp = BINOPREAD;
	dispatch = binReadDisp[type];
	return ( (*dispatch)( FDIO, fd, (FILE *)NULL, (unsigned char *)NULL,
		(unsigned char*) buf, type, nBytes, count ) );
}

int					/* Returns # of bytes read	*/
#ifdef __STDC__
BinFRead( FILE *fp, void *buf, int type, int nBytes, int count )
#else
BinFRead( fp, buf, type, nBytes, count )
	FILE *fp;			/* File pointer			*/
	unsigned char *buf;		/* Data buffer			*/
	int type;			/* Buffer variable's type	*/
	int nBytes;			/* # of bytes when in file	*/
	int count;			/* # of items to read		*/
#endif
{
#ifdef __STDC__
	int (*dispatch)( int, int, FILE *, unsigned char *, unsigned char *,
		int, int, int );	/* Dispatch table entry		*/
#else
	int (*dispatch)(  );	/* Dispatch table entry		*/
#endif
	BINCHECKRTYPE(  type );
	BINCHECKNBYTES( nBytes );
	BINCHECKCOUNT(  count );

	if ( !binInit )
		binInitialize( );

	binOp = BINOPREAD;
	dispatch = binReadDisp[type];
	return ( (*dispatch)( FPIO, -1, fp,
		(unsigned char *)NULL, (unsigned char*) buf, type, nBytes, count ) );
}

int					/* Returns # of bytes read	*/
#ifdef __STDC__
BinSRead( unsigned char *data, void *buf, int type, int nBytes, int count )
#else
BinSRead( data, buf, type, nBytes, count )
	unsigned char *data;		/* Input data			*/
	unsigned char *buf;		/* Data buffer			*/
	int type;			/* Buffer variable's type	*/
	int nBytes;			/* # of bytes when in file	*/
	int count;			/* # of items to read		*/
#endif
{
#ifdef __STDC__
	int (*dispatch)( int, int, FILE *, unsigned char *, unsigned char *,
		int, int, int );	/* Dispatch table entry		*/
#else
	int (*dispatch)( );	/* Dispatch table entry		*/
#endif
	BINCHECKRTYPE(  type );
	BINCHECKNBYTES( nBytes );
	BINCHECKCOUNT(  count );

	if ( !binInit )
		binInitialize( );

	binOp = BINOPREAD;
	dispatch = binReadDisp[type];
	return ( (*dispatch)( DATAIO, -1, (FILE *)NULL,
		data, (unsigned char*) buf, type, nBytes, count ) );
}

int					/* Returns # of bytes written	*/
#ifdef __STDC__
BinWrite( int fd, void *buf, int type, int nBytes, int count )
#else
BinWrite( fd, buf, type, nBytes, count )
	int fd;				/* File descriptor		*/
	unsigned char *buf;		/* Data buffer			*/
	int type;			/* Buffer variable's type	*/
	int nBytes;			/* # of bytes when in file	*/
	int count;			/* # of items to write		*/
#endif
{
#ifdef __STDC__
	int (*dispatch)( int, int, FILE *, unsigned char *, unsigned char *,
		int, int, int );	/* Dispatch table entry		*/
#else
	int (*dispatch)(  );	/* Dispatch table entry		*/
#endif
	BINCHECKWTYPE(  type );
	BINCHECKNBYTES( nBytes );
	BINCHECKCOUNT(  count );

	if ( !binInit )
		binInitialize( );

	binOp = BINOPWRITE;
	dispatch = binWriteDisp[type];
	return ( (*dispatch)( FDIO, fd, (FILE *)NULL,
		(unsigned char *)NULL, (unsigned char*) buf, type, nBytes, count ) );
}

int					/* Returns # of bytes written	*/
#ifdef __STDC__
BinFWrite( FILE *fp, void *buf, int type, int nBytes, int count )
#else
BinFWrite( fp, buf, type, nBytes, count )
	FILE *fp;			/* File pointer			*/
	unsigned char *buf;		/* Data buffer			*/
	int type;			/* Buffer variable's type	*/
	int nBytes;			/* # of bytes when in file	*/
	int count;			/* # of items to write		*/
#endif
{
#ifdef __STDC__
	int (*dispatch)( int, int, FILE *, unsigned char *, unsigned char *,
		int, int, int );	/* Dispatch table entry		*/
#else
	int (*dispatch)(  );	/* Dispatch table entry		*/
#endif
	BINCHECKWTYPE(  type );
	BINCHECKNBYTES( nBytes );
	BINCHECKCOUNT(  count );

	if ( !binInit )
		binInitialize( );

	binOp = BINOPWRITE;
	dispatch = binWriteDisp[type];
	return ( (*dispatch)( FPIO, -1, fp,
		(unsigned char *)NULL, (unsigned char*) buf, type, nBytes, count ) );
}

int					/* Returns # of bytes written	*/
#ifdef __STDC__
BinSWrite( unsigned char *data, void *buf, int type, int nBytes, int count )
#else
BinSWrite( data, buf, type, nBytes, count )
	unsigned char *data;		/* Output data			*/
	unsigned char *buf;		/* Data buffer			*/
	int type;			/* Buffer variable's type	*/
	int nBytes;			/* # of bytes when in file	*/
	int count;			/* # of items to write		*/
#endif
{
#ifdef __STDC__
	int (*dispatch)( int, int, FILE *, unsigned char *, unsigned char *,
		int, int, int );	/* Dispatch table entry		*/
#else
	int (*dispatch)(  );	/* Dispatch table entry		*/
#endif
	BINCHECKWTYPE(  type );
	BINCHECKNBYTES( nBytes );
	BINCHECKCOUNT(  count );

	if ( !binInit )
		binInitialize( );

	binOp = BINOPWRITE;
	dispatch = binWriteDisp[type];
	return ( (*dispatch)( DATAIO, -1, (FILE *)NULL,
		data, (unsigned char*) buf, type, nBytes, count ) );
}





/*
 *  FUNCTION
 *	BinReadStruct	-  Read binary file into a C struct using unbuf I/O
 *	BinFReadStruct	-  Read binary file into a C struct using buf I/O
 *	BinSReadStruct	-  Read binary string into a C struct
 *	BinWriteStruct	-  Write binary file from a C struct using unbuf I/O
 *	BinFWriteStruct	-  Write binary file from a C struct using buf I/O
 *	BinSWriteStruct	-  Write binary string from a C struct
 *
 *  DESCRIPTION
 *	The incomming arguments are checked for legality.  If any are
 *	bad, a -1 is returned and the global error variable 'BinErrNo'
 *	set to an appropriate error code.
 *
 *	The structure description array is walked and the appropriate
 *	single type read or write function called through the read or write
 *	dispatch tables.  The dispatch tables point to LBF or MBF functions,
 *	as selected by an earlier user call to BinByteOrder( ).
 *
 *	Structure field padding is done per the host system's characteristics.
 *	For instance, on most 32-bit workstations, short's are aligned to
 *	16-bit boundaries, and int's and long's to 32-bit boundaries.
 */

int					/* Returns # of bytes read	*/
#ifdef __STDC__
BinReadStruct( int fd, void *buf, BinField *fields )
#else
BinReadStruct( fd, buf, fields )
	int fd;				/* File descriptor		*/
	unsigned char *buf;		/* Data buffer			*/
	BinField *fields;		/* Structure field description	*/
#endif
{
	BinField *bfp;			/* Field pointer		*/
	int nBytes = 0;			/* Number of bytes read		*/
	int n;				/* Read status & # bytes read	*/
	unsigned long ptrAsInt;		/* Pointer cast as int for math	*/
#ifdef __STDC__
	int (*dispatch)( int, int, FILE *, unsigned char *, unsigned char *,
		int, int, int );	/* Dispatch table entry		*/
#else
	int (*dispatch)( );	/* Dispatch table entry		*/
#endif

	/*
	 *  Check all values in the fields array before doing anything.
	 */
	for ( bfp = fields; bfp->bin_count; bfp++ )
	{
		BINCHECKRTYPE(  bfp->bin_type );
		BINCHECKNBYTES( bfp->bin_nbytes )
	}

	if ( !binInit )
		binInitialize( );


	/*
	 *  Now execute the reads.
	 *
	 *  This is potentially confusing code, so here's what's happening:
	 *
	 *	-  If the buf pointer is not currently at the next address
	 *	   needed based upon the host's struct padding characteristics,
	 *	   then we need to move it there.
	 *
	 *	-  To do that check we need to do some arithmetic on the
	 *	   buf pointer as an integer.  This requires that we cast it
	 *	   to an integer type (unsigned long).  We assume that an
	 *	   unsigned long is big enough to hold a pointer (!!!).
	 *
	 *	-  As a quick check, we mask off the lower bits of the address
	 *	   based upon the binTypePadMask[] value for the type.  This
	 *	   tells us fast if the buf pointer needs aligning or not.
	 *
	 *	-  Say we need to align.  To do so we need to get rid of those
	 *	   lower bits, then add the right amount to leap to the next
	 *	   alignment point for the address.  This is type dependent.
	 *	   The distance to leap is the pad amount for the type and is
	 *	   contained in binTypePad[].  The amount to mask off is again
	 *	   in binTypePadMask[].  Because we're doing arithmetic, we
	 *	   need to cast the buf pointer as an integer (unsigned long)
	 *	   to do the math.
	 *
	 *  That's it.
	 */
	binOp = BINOPREADSTRUCT;
	for ( bfp = fields; bfp->bin_count; bfp++ )
	{
		ptrAsInt = (unsigned long)buf;
		ptrAsInt &= binTypePadMask[bfp->bin_type];
		if ( ptrAsInt != (unsigned long)0 )
		{
			/* Then we need to align the pointer.		*/
			ptrAsInt  = (unsigned long)buf;
			ptrAsInt &= ~binTypePadMask[bfp->bin_type];
			buf = (unsigned char *)ptrAsInt + binTypePad[bfp->bin_type];
		}
		dispatch = binReadDisp[bfp->bin_type];
		if ( (n = (*dispatch)( FDIO, fd, (FILE *)NULL,
			(unsigned char *)NULL, (unsigned char*) buf, bfp->bin_type,
			bfp->bin_nbytes, bfp->bin_count )) == -1 )
			return ( -1 );		/* Error occured	*/
		nBytes   += n;
		buf = (unsigned char *)buf + binTypeSize[bfp->bin_type] * bfp->bin_count;
	}
	return ( nBytes );
}

int					/* Returns # of bytes read	*/
#ifdef __STDC__
BinFReadStruct( FILE *fp, void *buf, BinField *fields )
#else
BinFReadStruct( fp, buf, fields )
	FILE *fp;			/* File pointer			*/
	unsigned char *buf;		/* Data buffer			*/
	BinField *fields;		/* Structure field description	*/
#endif
{
	BinField *bfp;			/* Field pointer		*/
	int nBytes = 0;			/* Number of bytes read		*/
	int n;				/* Read status & # bytes read	*/
	unsigned long ptrAsInt;		/* Pointer cast as int for math	*/
#ifdef __STDC__
	int (*dispatch)( int, int, FILE *, unsigned char *, unsigned char *,
		int, int, int );	/* Dispatch table entry		*/
#else
	int (*dispatch)( );	/* Dispatch table entry		*/
#endif

	/*
	 *  Check all values in the fields array before doing anything.
	 */
	for ( bfp = fields; bfp->bin_count; bfp++ )
	{
		BINCHECKRTYPE(  bfp->bin_type );
		BINCHECKNBYTES( bfp->bin_nbytes )
	}

	if ( !binInit )
		binInitialize( );


	/*
	 *  Now execute the reads.
	 *
	 *  This is potentially confusing code, so here's what's happening:
	 *
	 *	-  If the buf pointer is not currently at the next address
	 *	   needed based upon the host's struct padding characteristics,
	 *	   then we need to move it there.
	 *
	 *	-  To do that check we need to do some arithmetic on the
	 *	   buf pointer as an integer.  This requires that we cast it
	 *	   to an integer type (unsigned long).  We assume that an
	 *	   unsigned long is big enough to hold a pointer (!!!).
	 *
	 *	-  As a quick check, we mask off the lower bits of the address
	 *	   based upon the binTypePadMask[] value for the type.  This
	 *	   tells us fast if the buf pointer needs aligning or not.
	 *
	 *	-  Say we need to align.  To do so we need to get rid of those
	 *	   lower bits, then add the right amount to leap to the next
	 *	   alignment point for the address.  This is type dependent.
	 *	   The distance to leap is the pad amount for the type and is
	 *	   contained in binTypePad[].  The amount to mask off is again
	 *	   in binTypePadMask[].  Because we're doing arithmetic, we
	 *	   need to cast the buf pointer as an integer (unsigned long)
	 *	   to do the math.
	 *
	 *  That's it.
	 */
	binOp = BINOPREADSTRUCT;
	for ( bfp = fields; bfp->bin_count; bfp++ )
	{
		ptrAsInt = (unsigned long)buf;
		ptrAsInt &= binTypePadMask[bfp->bin_type];
		if ( ptrAsInt != (unsigned long)0 )
		{
			/* Then we need to align the pointer.		*/
			ptrAsInt  = (unsigned long)buf;
			ptrAsInt &= ~binTypePadMask[bfp->bin_type];
			buf = (unsigned char *)ptrAsInt + binTypePad[bfp->bin_type];
		}
		dispatch = binReadDisp[bfp->bin_type];
		if ( (n = (*dispatch)( FPIO, -1, fp,
			(unsigned char *)NULL, (unsigned char*) buf, bfp->bin_type,
			bfp->bin_nbytes, bfp->bin_count )) == -1 )
			return ( -1 );		/* Error occured	*/
		nBytes   += n;
		buf = (unsigned char *)buf + binTypeSize[bfp->bin_type] * bfp->bin_count;
	}
	return ( nBytes );
}

int					/* Returns # of bytes read	*/
#ifdef __STDC__
BinSReadStruct( unsigned char *data, void *buf, BinField *fields )
#else
BinSReadStruct( data, buf, fields )
	unsigned char *data;		/* Input data			*/
	unsigned char *buf;		/* Data buffer			*/
	BinField *fields;		/* Structure field description	*/
#endif
{
	BinField *bfp;			/* Field pointer		*/
	int nBytes = 0;			/* Number of bytes read		*/
	int n;				/* Read status & # bytes read	*/
	unsigned long ptrAsInt;		/* Pointer cast as int for math	*/
#ifdef __STDC__
	int (*dispatch)( int, int, FILE *, unsigned char *, unsigned char *,
		int, int, int );	/* Dispatch table entry		*/
#else
	int (*dispatch)( );	/* Dispatch table entry		*/
#endif

	/*
	 *  Check all values in the fields array before doing anything.
	 */
	for ( bfp = fields; bfp->bin_count; bfp++ )
	{
		BINCHECKRTYPE(  bfp->bin_type );
		BINCHECKNBYTES( bfp->bin_nbytes )
	}

	if ( !binInit )
		binInitialize( );


	/*
	 *  Now execute the reads.
	 *
	 *  This is potentially confusing code, so here's what's happening:
	 *
	 *	-  If the buf pointer is not currently at the next address
	 *	   needed based upon the host's struct padding characteristics,
	 *	   then we need to move it there.
	 *
	 *	-  To do that check we need to do some arithmetic on the
	 *	   buf pointer as an integer.  This requires that we cast it
	 *	   to an integer type (unsigned long).  We assume that an
	 *	   unsigned long is big enough to hold a pointer (!!!).
	 *
	 *	-  As a quick check, we mask off the lower bits of the address
	 *	   based upon the binTypePadMask[] value for the type.  This
	 *	   tells us fast if the buf pointer needs aligning or not.
	 *
	 *	-  Say we need to align.  To do so we need to get rid of those
	 *	   lower bits, then add the right amount to leap to the next
	 *	   alignment point for the address.  This is type dependent.
	 *	   The distance to leap is the pad amount for the type and is
	 *	   contained in binTypePad[].  The amount to mask off is again
	 *	   in binTypePadMask[].  Because we're doing arithmetic, we
	 *	   need to cast the buf pointer as an integer (unsigned long)
	 *	   to do the math.
	 *
	 *  That's it.
	 */
	binOp = BINOPREADSTRUCT;
	for ( bfp = fields; bfp->bin_count; bfp++ )
	{
		ptrAsInt = (unsigned long)buf;
		ptrAsInt &= binTypePadMask[bfp->bin_type];
		if ( ptrAsInt != (unsigned long)0 )
		{
			/* Then we need to align the pointer.		*/
			ptrAsInt  = (unsigned long)buf;
			ptrAsInt &= ~binTypePadMask[bfp->bin_type];
			buf = (unsigned char *)ptrAsInt + binTypePad[bfp->bin_type];
		}
		dispatch = binReadDisp[bfp->bin_type];
		if ( (n = (*dispatch)( DATAIO, -1, (FILE *)NULL,
			data, (unsigned char*) buf, bfp->bin_type,
			bfp->bin_nbytes, bfp->bin_count )) == -1 )
			return ( -1 );		/* Error occured	*/
		nBytes   += n;
		data     += bfp->bin_nbytes * bfp->bin_count;
		buf = (unsigned char *)buf + binTypeSize[bfp->bin_type] * bfp->bin_count;
	}
	return ( nBytes );
}

int					/* Returns # of bytes written	*/
#ifdef __STDC__
BinWriteStruct( int fd, void *buf, BinField *fields )
#else
BinWriteStruct( fd, buf, fields )
	int fd;				/* File descriptor		*/
	unsigned char *buf;		/* Data buffer			*/
	BinField *fields;		/* Structure field description	*/
#endif
{
	BinField *bfp;			/* Field pointer		*/
	int nBytes = 0;			/* Number of bytes written	*/
	int n;				/* Read status & # bytes written*/
	unsigned long ptrAsInt;		/* Pointer cast as int for math	*/
#ifdef __STDC__
	int (*dispatch)( int, int, FILE *, unsigned char *, unsigned char *,
		int, int, int );	/* Dispatch table entry		*/
#else
	int (*dispatch)( );	/* Dispatch table entry		*/
#endif

	/*
	 *  Check all values in the fields array before doing anything.
	 */
	for ( bfp = fields; bfp->bin_count; bfp++ )
	{
		BINCHECKWTYPE(  bfp->bin_type );
		BINCHECKNBYTES( bfp->bin_nbytes );
	}

	if ( !binInit )
		binInitialize( );


	/*
	 *  Now execute the writes.
	 *
	 *  This is potentially confusing code, so here's what's happening:
	 *
	 *	-  If the buf pointer is not currently at the next address
	 *	   needed based upon the host's struct padding characteristics,
	 *	   then we need to move it there.
	 *
	 *	-  To do that check we need to do some arithmetic on the
	 *	   buf pointer as an integer.  This requires that we cast it
	 *	   to an integer type (unsigned long).  We assume that an
	 *	   unsigned long is big enough to hold a pointer (!!!).
	 *
	 *	-  As a quick check, we mask off the lower bits of the address
	 *	   based upon the binTypePadMask[] value for the type.  This
	 *	   tells us fast if the buf pointer needs aligning or not.
	 *
	 *	-  Say we need to align.  To do so we need to get rid of those
	 *	   lower bits, then add the right amount to leap to the next
	 *	   alignment point for the address.  This is type dependent.
	 *	   The distance to leap is the pad amount for the type and is
	 *	   contained in binTypePad[].  The amount to mask off is again
	 *	   in binTypePadMask[].  Because we're doing arithmetic, we
	 *	   need to cast the buf pointer as an integer (unsigned long)
	 *	   to do the math.
	 *
	 *  That's it.
	 */
	binOp = BINOPWRITESTRUCT;
	for ( bfp = fields; bfp->bin_count; bfp++ )
	{
		ptrAsInt = (unsigned long)buf;
		ptrAsInt &= binTypePadMask[bfp->bin_type];
		if ( ptrAsInt != (unsigned long)0 )
		{
			/* Then we need to align the pointer.		*/
			ptrAsInt  = (unsigned long)buf;
			ptrAsInt &= ~binTypePadMask[bfp->bin_type];
			buf = (unsigned char *) ptrAsInt + binTypePad[bfp->bin_type];
		}
		dispatch = binWriteDisp[bfp->bin_type];
		if ( (n = (*dispatch)( FDIO, fd, (FILE *)NULL,
			(unsigned char *)NULL, (unsigned char*) buf, bfp->bin_type,
			bfp->bin_nbytes, bfp->bin_count )) == -1 )
			return ( -1 );		/* Error occured	*/
		nBytes   += n;

		buf = (unsigned char *)buf + binTypeSize[bfp->bin_type] * bfp->bin_count;
	}
	return ( nBytes );
}

int					/* Returns # of bytes written	*/
#ifdef __STDC__
BinFWriteStruct( FILE *fp, void *buf, BinField *fields )
#else
BinFWriteStruct( fp, buf, fields )
	FILE *fp;			/* File pointer			*/
	unsigned char *buf;		/* Data buffer			*/
	BinField *fields;		/* Structure field description	*/
#endif
{
	BinField *bfp;			/* Field pointer		*/
	int nBytes = 0;			/* Number of bytes written	*/
	int n;				/* Read status & # bytes written*/
	unsigned long ptrAsInt;		/* Pointer cast as int for math	*/
#ifdef __STDC__
	int (*dispatch)( int, int, FILE *, unsigned char *, unsigned char *,
		int, int, int );	/* Dispatch table entry		*/
#else
	int (*dispatch)( );	/* Dispatch table entry		*/
#endif

	/*
	 *  Check all values in the fields array before doing anything.
	 */
	for ( bfp = fields; bfp->bin_count; bfp++ )
	{
		BINCHECKWTYPE(  bfp->bin_type );
		BINCHECKNBYTES( bfp->bin_nbytes );
	}

	if ( !binInit )
		binInitialize( );


	/*
	 *  Now execute the writes.
	 *
	 *  This is potentially confusing code, so here's what's happening:
	 *
	 *	-  If the buf pointer is not currently at the next address
	 *	   needed based upon the host's struct padding characteristics,
	 *	   then we need to move it there.
	 *
	 *	-  To do that check we need to do some arithmetic on the
	 *	   buf pointer as an integer.  This requires that we cast it
	 *	   to an integer type (unsigned long).  We assume that an
	 *	   unsigned long is big enough to hold a pointer (!!!).
	 *
	 *	-  As a quick check, we mask off the lower bits of the address
	 *	   based upon the binTypePadMask[] value for the type.  This
	 *	   tells us fast if the buf pointer needs aligning or not.
	 *
	 *	-  Say we need to align.  To do so we need to get rid of those
	 *	   lower bits, then add the right amount to leap to the next
	 *	   alignment point for the address.  This is type dependent.
	 *	   The distance to leap is the pad amount for the type and is
	 *	   contained in binTypePad[].  The amount to mask off is again
	 *	   in binTypePadMask[].  Because we're doing arithmetic, we
	 *	   need to cast the buf pointer as an integer (unsigned long)
	 *	   to do the math.
	 *
	 *  That's it.
	 */
	binOp = BINOPWRITESTRUCT;
	for ( bfp = fields; bfp->bin_count; bfp++ )
	{
		ptrAsInt = (unsigned long)buf;
		ptrAsInt &= binTypePadMask[bfp->bin_type];
		if ( ptrAsInt != (unsigned long)0 )
		{
			/* Then we need to align the pointer.		*/
			ptrAsInt  = (unsigned long)buf;
			ptrAsInt &= ~binTypePadMask[bfp->bin_type];
			buf = (unsigned char *)ptrAsInt + binTypePad[bfp->bin_type];
		}
		dispatch = binWriteDisp[bfp->bin_type];
		if ( (n = (*dispatch)( FPIO, -1, fp,
			(unsigned char *)NULL, (unsigned char*) buf, bfp->bin_type,
			bfp->bin_nbytes, bfp->bin_count )) == -1 )
			return ( -1 );		/* Error occured	*/
		nBytes   += n;
		buf = (unsigned char *)buf + binTypeSize[bfp->bin_type] * bfp->bin_count;
	}
	return ( nBytes );
}

int					/* Returns # of bytes written	*/
#ifdef __STDC__
BinSWriteStruct( unsigned char *data, void *buf, BinField *fields )
#else
BinSWriteStruct( data, buf, fields )
	unsigned char *data;		/* Output data			*/
	unsigned char *buf;		/* Data buffer			*/
	BinField *fields;		/* Structure field description	*/
#endif
{
	BinField *bfp;			/* Field pointer		*/
	int nBytes = 0;			/* Number of bytes written	*/
	int n;				/* Read status & # bytes written*/
	unsigned long ptrAsInt;		/* Pointer cast as int for math	*/
#ifdef __STDC__
	int (*dispatch)( int, int, FILE *, unsigned char *, unsigned char *,
		int, int, int );	/* Dispatch table entry		*/
#else
	int (*dispatch)( );	/* Dispatch table entry		*/
#endif

	/*
	 *  Check all values in the fields array before doing anything.
	 */
	for ( bfp = fields; bfp->bin_count; bfp++ )
	{
		BINCHECKWTYPE(  bfp->bin_type );
		BINCHECKNBYTES( bfp->bin_nbytes );
	}

	if ( !binInit )
		binInitialize( );


	/*
	 *  Now execute the writes.
	 *
	 *  This is potentially confusing code, so here's what's happening:
	 *
	 *	-  If the buf pointer is not currently at the next address
	 *	   needed based upon the host's struct padding characteristics,
	 *	   then we need to move it there.
	 *
	 *	-  To do that check we need to do some arithmetic on the
	 *	   buf pointer as an integer.  This requires that we cast it
	 *	   to an integer type (unsigned long).  We assume that an
	 *	   unsigned long is big enough to hold a pointer (!!!).
	 *
	 *	-  As a quick check, we mask off the lower bits of the address
	 *	   based upon the binTypePadMask[] value for the type.  This
	 *	   tells us fast if the buf pointer needs aligning or not.
	 *
	 *	-  Say we need to align.  To do so we need to get rid of those
	 *	   lower bits, then add the right amount to leap to the next
	 *	   alignment point for the address.  This is type dependent.
	 *	   The distance to leap is the pad amount for the type and is
	 *	   contained in binTypePad[].  The amount to mask off is again
	 *	   in binTypePadMask[].  Because we're doing arithmetic, we
	 *	   need to cast the buf pointer as an integer (unsigned long)
	 *	   to do the math.
	 *
	 *  That's it.
	 */
	binOp = BINOPWRITESTRUCT;
	for ( bfp = fields; bfp->bin_count; bfp++ )
	{
		ptrAsInt = (unsigned long)buf;
		ptrAsInt &= binTypePadMask[bfp->bin_type];
		if ( ptrAsInt != (unsigned long)0 )
		{
			/* Then we need to align the pointer.		*/
			ptrAsInt  = (unsigned long)buf;
			ptrAsInt &= ~binTypePadMask[bfp->bin_type];
			buf = (unsigned char *)ptrAsInt + binTypePad[bfp->bin_type];
		}
		dispatch = binWriteDisp[bfp->bin_type];
		if ( (n = (*dispatch)( DATAIO, -1, (FILE *)NULL,
			data, (unsigned char*) buf, bfp->bin_type,
			bfp->bin_nbytes, bfp->bin_count )) == -1 )
			return ( -1 );		/* Error occured	*/
		nBytes   += n;
		data     += bfp->bin_nbytes * bfp->bin_count;
		buf = (unsigned char *)buf + binTypeSize[bfp->bin_type] * bfp->bin_count;
	}
	return ( nBytes );
}





/*
 *  FUNCTION
 *	binByteRead	-  read bytes into a buffer
 *	binByteWrite	-  write bytes from a buffer
 *
 *  DESCRIPTION
 *	binByteRead:	N bytes of data are read in from a file descriptor,
 *	file pointer, or data buffer.
 *
 *	binByteWrite:	N bytes of data are written to a file descriptor,
 *	file pointer, or data buffer.
 *
 *	When reading from or writing to file descriptors or file pointers,
 *	we loop on the read or write in order to block, even on non-blocking
 *	I/O connections, such as pipes and sockets.
 */

static int				/* Returns # of bytes read	*/
#ifdef __STDC__
binByteRead( int ioType, int fd, FILE *fp, unsigned char *data, unsigned char *buffer, int n )
#else
binByteRead( ioType, fd, fp, data, buffer, n )
	int   ioType;			/* Where to get data		*/
	int   fd;			/* File descriptor		*/
	FILE *fp;			/* File pointer			*/
	unsigned char *data;		/* Input data			*/
	unsigned char *buffer;		/* Output data			*/
	int   n;			/* # of bytes to read		*/
#endif
{
	int cnt;			/* Counter			*/
	int nBytes;			/* # of bytes read in at once	*/
	unsigned char *pBuffer;		/* Buffer pointer		*/

	switch ( ioType )
	{
	case FDIO:	/* File descriptor.				*/
		for ( cnt = 0, pBuffer = buffer; cnt != n; )
		{
			switch ( (nBytes = read( fd, pBuffer, n-cnt )) )
			{
			case 0:	/* EOF reached.				*/
				return ( cnt );
			case -1:/* Error.				*/
				BINERROR( BINESYS, -1 );
			default:
				pBuffer += nBytes;
				cnt     += nBytes;
				break;
			}
		}
		return ( cnt );

	case FPIO:	/* File pointer.				*/
		for ( cnt = 0, pBuffer = buffer; cnt != n; )
		{
			switch ( (nBytes = fread( (char *)pBuffer, 1, n-cnt, fp )) )
			{
			case 0:	/* EOF or error reached.		*/
				return ( cnt );
			default:
				pBuffer += nBytes;
				cnt     += nBytes;
				break;
			}
		}
		return ( cnt );

	case DATAIO:	/* Data buffer.					*/
		memcpy( (void*)buffer, (void*)data, n);
		return ( n );
	}
	return( 0 );
	/*NOTREACHED*/
}

static int				/* Returns # of bytes written	*/
#ifdef __STDC__
binByteWrite( int ioType, int fd, FILE *fp, unsigned char *data,
	unsigned char *buffer, int n )
#else
binByteWrite( ioType, fd, fp, data, buffer, n )
	int   ioType;			/* Where to send data		*/
	int   fd;			/* File descriptor		*/
	FILE *fp;			/* File pointer			*/
	unsigned char *data;		/* Output data			*/
	unsigned char *buffer;		/* Input data			*/
	int   n;			/* # of bytes to write		*/
#endif
{
	int cnt;			/* Counter			*/
	int nBytes;			/* # of bytes read in at once	*/
	unsigned char *pBuffer;		/* Buffer pointer		*/

	switch ( ioType )
	{
	case FDIO:	/* File descriptor.				*/
		for ( cnt = 0, pBuffer = buffer; cnt != n; )
		{
			switch ( (nBytes = write( fd, (char *)pBuffer, n-cnt )) )
			{
			case 0:	/* EOF reached.				*/
				return ( cnt );
			case -1:/* Error.				*/
				BINERROR( BINESYS, -1 );
			default:
				pBuffer += nBytes;
				cnt     += nBytes;
				break;
			}
		}
		return ( cnt );

	case FPIO:	/* File pointer.				*/
		for ( cnt = 0, pBuffer = buffer; cnt != n; )
		{
			switch ( (nBytes = fwrite( (char *)pBuffer, 1, n-cnt, fp )) )
			{
			case 0:	/* EOF or error reached.		*/
				return ( cnt );
			default:
				pBuffer += nBytes;
				cnt     += nBytes;
				break;
			}
		}
		return ( cnt );

	case DATAIO:	/* Data buffer.					*/
		memcpy( (void*) data, (void*) buffer, n);
		return ( n );
	}
	return( 0 );
	/*NOTREACHED*/
}





/*
 *  FUNCTION
 *	binReadCharMBF	-  Read characters from an MBF input
 *	binReadCharLBF	-  Read characters from an LBF input
 *	binWriteCharMBF	-  Write characters to MBF output
 *	binWriteCharLBF	-  Write characters to LBF output
 *
 *  DESCRIPTION
 *	If the host and input/output data share the same size, then a
 *	simple byte-for-byte read or write is done.
 *
 *	Otherwise, for reads we read the required number of bytes from the
 *	input into a holding buffer, then pull out the least-significant
 *	byte of each 'nBytes' item and copy it into the next output buffer slot.
 *
 *	If the incomming data has more bits of precision (and uses them) than
 *	the host-side character, then truncation occurs and the user's
 *	integer truncation callback is called.  (if the input's type is signed,
 *	bytes that aren't 0x00 or 0xFF (sign-extension) indicate truncation.
 *	if the input's type is unsigned, only non-0x00 bytes indicate
 *	truncation).
 *
 *	For each item to be written, the byte is extracted from the input
 *	buffer and added to a holding buffer.  More-significant bytes in the
 *	holding buffer are zero padded or sign-extended.
 *
 *	For all four routines, a 'byteHold' buffer allocated on the stack
 *	is prefered.  However, if the buffer isn't big enough, a dynamically
 *	allocated holding buffer is used.
 */

static int				/* Returns # of bytes read	*/
#ifdef __STDC__
binReadCharMBF( int ioType, int fd, FILE *fp, unsigned char *data,
	unsigned char *buf, int type, int nBytes, int count )
#else
binReadCharMBF( ioType, fd, fp, data, buf, type, nBytes, count )
	int            ioType;		/* Use buffered or unbuffered I/O*/
	int            fd;		/* File descriptor		*/
	FILE          *fp;		/* File pointer			*/
	unsigned char *data;		/* Input data			*/
	unsigned char *buf;		/* Data buffer			*/
	int            type;		/* Always char			*/
	int            nBytes;		/* # of bytes when in file	*/
	int            count;		/* # of items to read		*/
#endif
{
	unsigned char  byteHold[BINMAXBUF];	/* Input holding buffer	*/
	unsigned char  *pHold  = byteHold;/* Holding buffer pointer	*/
	unsigned char  *pAlloc = NULL;	/* Dynamically allocated hold buf*/
	int		i, j;		/* Counters			*/
	unsigned char	thisOne, that;	/* Byte values to check for	*/


	if ( nBytes == 1 )		/* sizeof( char ) assumed to be 1*/
		return ( binByteRead( ioType, fd, fp, data, buf, nBytes*count));

	if ( (nBytes * count > BINMAXBUF) &&
		( (pAlloc = pHold = (unsigned char *)malloc( (unsigned int)(nBytes*count) )) == NULL) )
		BINERROR( BINEMALLOC, -1 );
	if ( binByteRead( ioType, fd, fp, data, pHold, nBytes * count ) == -1 )
	{
		if ( pAlloc )
			free( (char *)pAlloc );
		return ( -1 );
	}


	/*
	 *  Copy selected bytes from the input temp buffer to the return
	 *  buffer.  Check the bytes skipped to see if any of them contained
	 *  information that we're truncating.  If so, call the truncation
	 *  callback.
	 */
	thisOne = that = 0x00;
	if ( binTypeSigned[type] )
		that = 0xFF;
	for ( i = count; i; --i )
	{
		for ( j = nBytes - 1; j; --j, ++pHold )
		{
			if ( *pHold != thisOne && *pHold != that )
			{
				(*binErrFunc)( fd, binOp, BINEINTEGERTRUNC,
					(pHold-(nBytes-j-1)), nBytes*8, 8 );
				pHold += j;
				break;
			}
		}
		*buf++ = *pHold++;	/* Use the last byte read in	*/
	}
	if ( pAlloc )
		free( (char *)pAlloc );
	return ( nBytes * count );
}

static int				/* Returns # of bytes read	*/
#ifdef __STDC__
binReadCharLBF( int ioType, int fd, FILE *fp, unsigned char *data,
	unsigned char *buf, int type, int nBytes, int count )
#else
binReadCharLBF( ioType, fd, fp, data, buf, type, nBytes, count )
	int            ioType;		/* Use buffered or unbuffered I/O*/
	int            fd;		/* File descriptor		*/
	FILE          *fp;		/* File pointer			*/
	unsigned char *data;		/* Input data			*/
	unsigned char *buf;		/* Data buffer			*/
	int            type;		/* Always char			*/
	int            nBytes;		/* # of bytes when in file	*/
	int            count;		/* # of items to read		*/
#endif
{
	unsigned char  byteHold[BINMAXBUF];	/* Input holding buffer	*/
	unsigned char  *pHold  = byteHold;/* Holding buffer pointer	*/
	unsigned char  *pAlloc = NULL;	/* Dynamically allocated hold buf*/
	int		i, j;		/* Counters			*/
	unsigned char	thisOne, that;	/* Byte values to check for	*/


	if ( nBytes == 1 )		/* sizeof( char ) assumed to be 1*/
		return ( binByteRead( ioType, fd, fp, data, buf, nBytes*count));

	if ( (nBytes * count > BINMAXBUF) &&
		( (pAlloc = pHold = (unsigned char *)malloc( (unsigned int)(nBytes*count) )) == NULL) )
		BINERROR( BINEMALLOC, -1 );
	if ( binByteRead( ioType, fd, fp, data, pHold, nBytes * count ) == -1 )
	{
		if ( pAlloc )
			free( (char *)pAlloc );
		return ( -1 );
	}


	/*
	 *  Copy selected bytes from the input temp buffer to the return
	 *  buffer.  Check the bytes skipped to see if any of them contained
	 *  information that we're truncating.  If so, call the truncation
	 *  callback.
	 */
	thisOne = that = 0x00;
	if ( binTypeSigned[type] )
		that = 0xFF;
	for ( i = count; i; --i )
	{
		*buf++ = *pHold++;	/* Use the first byte read in	*/
		for ( j = nBytes - 1; j; --j, ++pHold )
		{
			if ( *pHold != thisOne && *pHold != that )
			{
				(*binErrFunc)( fd, binOp, BINEINTEGERTRUNC,
					(pHold-(nBytes-j)), nBytes*8, 8 );
				pHold += j;
				break;
			}
		}
	}
	if ( pAlloc )
		free( (char *)pAlloc );
	return ( nBytes * count );
}

static int				/* Returns # of bytes write	*/
#ifdef __STDC__
binWriteCharMBF( int ioType, int fd, FILE *fp, unsigned char *data,
	unsigned char *buf, int type, int nBytes, int count )
#else
binWriteCharMBF( ioType, fd, fp, data, buf, type, nBytes, count )
	int             ioType;		/* Use buffered or unbuffered I/O*/
	int             fd;		/* File descriptor		*/
	FILE           *fp;		/* File pointer			*/
	unsigned char  *data;		/* Output data			*/
	unsigned char  *buf;		/* Data buffer			*/
	int             type;		/* Always char			*/
	int             nBytes;		/* # of bytes when in file	*/
	int             count;		/* # of items to write		*/
#endif
{
	int             i, j;		/* Counters			*/
	unsigned char   byteHold[BINMAXBUF];	/* Temp holding buffer	*/
	unsigned char  *pHold  = byteHold;/* Pointer to holding buffer	*/
	unsigned char  *pAlloc = NULL;	/* Dynamically allocated hold buf*/


	if ( nBytes == 1 )		/* sizeof( char ) assumed to be 1*/
		return ( binByteWrite( ioType, fd, fp, data, buf, count ) );

	if ( (nBytes * count > BINMAXBUF) &&
		( (pAlloc = pHold = (unsigned char *)malloc( (unsigned int)(nBytes*count) )) == NULL) )
		BINERROR( BINEMALLOC, -1 );


	/*
	 *  Transfer the input data to the holding buffer in the right
	 *  units.  Zero pad or sign-extend as necessary.
	 */
	memset( (void *)pHold, 0x00, nBytes * count);
	if ( binTypeSigned[type] )
		for ( i = 0; i < count; i++, *pHold++ = *buf++ )
			if ( (*buf & 0x70) != 0 )	/* sign bit set	*/
				for ( j = 1; j < nBytes; j++ )
					*pHold++ = 0xFF;/* Sign extend	*/
			else
				pHold += nBytes - 1;
	else
		for ( i = 0; i < count; i++, *pHold++ = *buf++ )
			pHold += nBytes - 1;


	/*
	 *  Write the hold buffer to the output.
	 */
	if ( pAlloc )
	{
		i = binByteWrite( ioType, fd, fp, data, pAlloc, nBytes * count );
		free( (char *)pAlloc );
	}
	else
		i = binByteWrite( ioType, fd, fp, data, byteHold, nBytes * count );
	return ( i );
}

static int				/* Returns # of bytes write	*/
#ifdef __STDC__
binWriteCharLBF( int ioType, int fd, FILE *fp, unsigned char *data,
	unsigned char *buf, int type, int nBytes, int count )
#else
binWriteCharLBF( ioType, fd, fp, data, buf, type, nBytes, count )
	int             ioType;		/* Use buffered or unbuffered I/O*/
	int             fd;		/* File descriptor		*/
	FILE           *fp;		/* File pointer			*/
	unsigned char  *data;		/* Output data			*/
	unsigned char  *buf;		/* Data buffer			*/
	int             type;		/* Always char			*/
	int             nBytes;		/* # of bytes when in file	*/
	int             count;		/* # of items to write		*/
#endif
{
	int             i, j;		/* Counters			*/
	unsigned char   byteHold[BINMAXBUF];	/* Temp holding buffer	*/
	unsigned char  *pHold  = byteHold;/* Pointer to holding buffer	*/
	unsigned char  *pAlloc = NULL;	/* Dynamically allocated hold buf*/


	if ( nBytes == 1 )		/* sizeof( char ) assumed to be 1*/
		return ( binByteWrite( ioType, fd, fp, data, buf, count ) );

	if ( (nBytes * count > BINMAXBUF) &&
		( (pAlloc = pHold = (unsigned char *)malloc( (unsigned int)(nBytes*count) )) == NULL) )
		BINERROR( BINEMALLOC, -1 );


	/*
	 *  Transfer the input data to the holding buffer in the right
	 *  units.  Sign-extend as necessary.
	 */
	memset( (void *)pHold, 0x00, nBytes * count);
	if ( binTypeSigned[type] )
		for ( i = 0; i < count; i++ )
		{
			*pHold++ = *buf;
			if ( (*buf++ & 0x70) != 0 )	/* sign bit set	*/
				for ( j = 1; j < nBytes; j++ )
					*pHold++ = 0xFF;/* Sign extend	*/
			else
				pHold += nBytes - 1;
		}
	else
		for ( i = 0; i < count; i++, pHold += nBytes )
			*pHold = *buf++;


	/*
	 *  Write the hold buffer to the output.
	 */
	if ( pAlloc )
	{
		i = binByteWrite( ioType, fd, fp, data, pAlloc, nBytes * count );
		free( (char *)pAlloc );
	}
	else
		i = binByteWrite( ioType, fd, fp, data, byteHold, nBytes * count );
	return ( i );
}





/*
 *  FUNCTION
 *	binReadIntMBF	-  Read MBF input into integers
 *	binReadIntLBF	-  Read LBF input into integers
 *	binWriteIntMBF	-  Write from integers to MBF output
 *	binWriteIntLBF	-  Write from integers to LBF output
 *
 *  DESCRIPTION
 *	These functions read or write integers from or to MBF or LBF files.
 *	The basic shell of all four routines is identical.
 *
 *	If the host's size of the item to be read or written is the same
 *	as the size in the file, and the byte order is the same, then
 *	all four routines default to using the read() or write() system
 *	call directly on the incomming buffer of data.
 *
 *	Otherwise the packing of data differs between the host and the file
 *	and data must be copied to/from the byte array sent to the read()
 *	and write() system calls.
 *
 *	The incomming buffer of data must be truncated or sign-extended as
 *	it is copied into the outgoing byte array.  Code differs slightly
 *	between LBF and MBF hosts.
 *
 *	All four of these functions depend upon the binTypeSize,
 *	binTypeRes, and binTypeSigned arrays for information on the type
 *	to be read or written:
 *
 *		binTypeSize	size of item, as stored by the host
 *		binTypeRes	resolution of item (# of significant bytes)
 *		binTypeSigned	signed type?
 *
 *	On most machines, binTypeSize and binTypeRes are identical.
 *	The CRAY 'short' is an example of when they are not.  A CRAY
 *	'short' is stored in 8 bytes, but only the lower 3 bytes can
 *	be used.  In this case, binTypeSize[SHORT] = 8, but
 *	binTypeRes[SHORT] = 3.
 *
 *	We could use a great many more #ifdef's to remove lines like:
 *		buf += size - res;
 *	which is a waste of time when size and res are the same for a
 *	machine.  However, adding #ifdef's makes getting the code
 *	working on a new machine more difficult, and creates a maintenance
 *	headache.  The computation cost of such extra statements is
 *	low compared to the cost of the other byte-swapping loops so
 *	the execution time savings doesn't seem worth the increase in
 *	maintenance effort.
 */

static int				/* Returns # of bytes read	*/
#ifdef __STDC__
binReadIntMBF( int ioType, int fd, FILE *fp, unsigned char *data,
	unsigned char *buf, int type, int nBytes, int count )
#else
binReadIntMBF( ioType, fd, fp, data, buf, type, nBytes, count )
	int             ioType;		/* Use buffered or unbuffered I/O*/
	int             fd;		/* File descriptor		*/
	FILE           *fp;		/* File pointer			*/
	unsigned char  *data;		/* Input data			*/
	unsigned char  *buf;		/* Data buffer			*/
	int             type;		/* Type to read			*/
	int             nBytes;		/* # of bytes when in file	*/
	int             count;		/* # of items to read		*/
#endif
{
	int             i, j;		/* Counters			*/
	unsigned char   byteHold[BINMAXBUF];	/* Byte buffer		*/
	unsigned char  *pHold = byteHold;/* Byte holder			*/
	int             res;		/* Resolution of type		*/
	int             size;		/* Size of type			*/
	int             issigned;	/* Is type signed?		*/
	unsigned char  *pAlloc = NULL;	/* Dynamically allocated buffer	*/
	unsigned char   thisOne, that;	/* Byte comparison holders	*/


	size     = binTypeSize[type];	/* Storage size of item		*/
	res      = binTypeRes[type];	/* Significant bytes of item	*/
	issigned = binTypeSigned[type];	/* Whether signed or not	*/


	if ( size == nBytes && res == size && binFileBO == BINHOSTBO )
		return ( binByteRead( ioType, fd, fp, data, buf, nBytes * count ));

	if ( (nBytes * count > BINMAXBUF) &&
		((pAlloc = pHold = (unsigned char *)malloc( (unsigned int)(nBytes*count) )) == NULL))
		BINERROR( BINEMALLOC, -1 );
	if ( binByteRead( ioType, fd, fp, data, pHold, nBytes * count ) == -1 )
	{
		if ( pAlloc )
			free( (char *)pAlloc );
		return ( -1 );
	}


	/*
	 *  Copy data from the holding buffer into the return buffer.
	 */
	if ( nBytes < res )
	{
		/*
		 *  Holding buffer items require fewer bytes than the
		 *  destination.  We've got to sign-extend signed incomming
		 *  data as we copy it to the destination.
		 */
		memset( (void *)buf, 0x00, size * count);
		for ( i = 0; i < count; i++ )
		{
#ifdef MBF				/* MBF host			*/
			buf += size - res;
			if ( issigned && (*pHold & 0x80) )
				for ( j = res - nBytes; j; j-- )
					*buf++ = 0xFF;	/* Sign extend	*/
			else
				buf += res - nBytes;
			for ( j = 0; j < nBytes; j++ )
				*buf++ = *pHold++;

#else					/* LBF host			*/
			for ( j = nBytes - 1; j >= 0; j-- )
				buf[j] = *pHold++;
			if ( issigned && (*pHold & 0x80) )
			{
				buf += nBytes;
				for ( j = res - nBytes; j; j-- )
					*buf++ = 0xFF;	/* Sign extend	*/
				buf += size - res;
			}
			else
				buf += size;
#endif
		}
		if ( pAlloc )
			free( (char *)pAlloc );
		return ( nBytes * count );
	}


	/*
	 *  Holding buffer items require the same number, or more bytes
	 *  than the destination.  We've got to truncate incomming data
	 *  as we copy it to the destination.
	 */
	thisOne = that = 0x00;
	if ( binTypeSigned[type] )
		that = 0xFF;

	for ( i = 0; i < count; i++ )
	{
		for ( j = 0; j < nBytes - res; j++, pHold++ )
		{
			if ( *pHold != thisOne && *pHold != that )
			{
				(*binErrFunc)( fd, binOp, BINEINTEGERTRUNC,
					(pHold-j), nBytes*8, res*8 );
				pHold += nBytes-res-j;
				break;
			}
		}
#ifdef MBF				/* MBF host			*/
		buf += size - res;
		for ( j = res; j; j-- )
			*buf++ = *pHold++;
#else					/* LBF host			*/
		for ( j = res - 1; j >= 0; j-- )
			buf[j] = *pHold++;
		buf += size;
#endif
	}
	if ( pAlloc )
		free( (char *)pAlloc );
	return ( nBytes * count );
}

static int				/* Returns # of bytes read	*/
#ifdef __STDC__
binReadIntLBF( int ioType, int fd, FILE *fp, unsigned char *data,
	unsigned char *buf, int type, int nBytes, int count )
#else
binReadIntLBF( ioType, fd, fp, data, buf, type, nBytes, count )
	int             ioType;		/* Use buffered or unbuffered I/O*/
	int             fd;		/* File descriptor		*/
	FILE           *fp;		/* File pointer			*/
	unsigned char  *data;		/* Input data			*/
	unsigned char  *buf;		/* Data buffer			*/
	int             type;		/* Type to read			*/
	int             nBytes;		/* # of bytes when in file	*/
	int             count;		/* # of items to read		*/
#endif
{
	int             i, j;		/* Counters			*/
	unsigned char   byteHold[BINMAXBUF];	/* Byte buffer		*/
	unsigned char  *pHold = byteHold;/* Byte holder			*/
	int             res;		/* Resolution of type		*/
	int             size;		/* Size of type			*/
	int             issigned;	/* Is type signed?		*/
	unsigned char  *pAlloc = NULL;	/* Dynamically allocated buffer	*/
	unsigned char   thisOne, that;	/* Byte comparison holders	*/


	size     = binTypeSize[type];	/* Storage size of item		*/
	res      = binTypeRes[type];	/* Significant bytes of item	*/
	issigned = binTypeSigned[type];	/* Whether signed or not	*/


	if ( size == nBytes && res == size && binFileBO == BINHOSTBO )
		return ( binByteRead( ioType, fd, fp, data, buf, nBytes * count ));

	if ( (nBytes * count > BINMAXBUF) &&
		((pAlloc = pHold = (unsigned char *)malloc( (unsigned int)(nBytes*count) )) == NULL))
		BINERROR( BINEMALLOC, -1 );
	if ( binByteRead( ioType, fd, fp, data, pHold, nBytes * count ) == -1 )
	{
		if ( pAlloc )
			free( (char *)pAlloc );
		return ( -1 );
	}


	/*
	 *  Copy data from the holding buffer into the return buffer.
	 */
	if ( nBytes < res )
	{
		/*
		 *  Holding buffer items require fewer bytes than the
		 *  destination.  We've got to sign-extend signed incomming
		 *  data as we copy it to the destination.
		 */
		memset( (void *)buf, 0x00, size * count);
		for ( i = 0; i < count; i++ )
		{
#ifdef MBF				/* MBF host			*/
			buf += size - res;
			if ( issigned && (*pHold & 0x80) )
				for ( j = res - nBytes; j; j-- )
					*buf++ = 0xFF;	/* Sign extend	*/
			else
				buf += res - nBytes;
			for ( j = nBytes - 1; j >= 0; j-- )
				buf[j] = *pHold++;
			buf += nBytes;
#else					/* LBF host			*/
			for ( j = nBytes; j; j-- )
				*buf++ = *pHold++;
			if ( issigned && (*(pHold-1) & 0x80) )
				for ( j = res - nBytes; j; j-- )
					*buf++ = 0xFF;	/* Sign extend	*/
			else
				buf += res - nBytes;
			buf += size - res;
#endif
		}
		if ( pAlloc )
			free( (char *)pAlloc );
		return ( nBytes * count );
	}

	/*
	 *  Holding buffer items require the same number, or more bytes
	 *  than the destination.  We've got to truncate incomming data
	 *  as we copy it to the destination.
	 */
	thisOne = that = 0x00;
	if ( binTypeSigned[type] )
		that = 0xFF;

	for ( i = 0; i < count; i++ )
	{
#ifdef MBF				/* MBF host			*/
		buf += size - res;
		for ( j = res - 1; j >= 0; j-- )
			buf[j] = *pHold++;
		buf += res;
#else					/* LBF host			*/
		for ( j = res; j; j-- )
			*buf++ = *pHold++;
		buf += size - res;
#endif
		for ( j = 0; j < nBytes - res; j++, pHold++ )
		{
			if ( *pHold != thisOne && *pHold != that )
			{
				(*binErrFunc)( fd, binOp, BINEINTEGERTRUNC,
					(pHold-j-res), nBytes*8, res*8 );
				pHold += nBytes-res-j;
				break;
			}
		}
	}
	if ( pAlloc )
		free( (char *)pAlloc );
	return ( nBytes * count );
}

static int				/* Returns # of bytes written	*/
#ifdef __STDC__
binWriteIntMBF( int ioType, int fd, FILE *fp, unsigned char *data,
	unsigned char *buf, int type, int nBytes, int count )
#else
binWriteIntMBF( ioType, fd, fp, data, buf, type, nBytes, count )
	int             ioType;		/* Use buffered or unbuffered I/O*/
	int             fd;		/* File descriptor		*/
	FILE           *fp;		/* File pointer			*/
	unsigned char  *data;		/* Output data			*/
	unsigned char  *buf;		/* Data buffer			*/
	int             type;		/* Type to write		*/
	int             nBytes;		/* # of bytes when in file	*/
	int             count;		/* # of items to write		*/
#endif
{
	int             i, j;		/* Counters			*/
	unsigned char   byteHold[BINMAXBUF];	/* Byte buffer		*/
	unsigned char  *pHold = byteHold;/* Data buffer pointer		*/
	int             res;		/* Resolution of type		*/
	int             size;		/* Size of type			*/
	int             issigned;	/* Is type signed?		*/
	int             n;		/* # of bytes written		*/
	unsigned char  *pAlloc = NULL;	/* Dynamically allocated buffer	*/
	unsigned char	thisOne, that;	/* Byte comparison holders	*/

	size     = binTypeSize[type];	/* Storage size of item		*/
	res      = binTypeRes[type];	/* Significant bytes of item	*/
	issigned = binTypeSigned[type];	/* Whether signed or not	*/
	n        = nBytes * count;


	if ( size == nBytes && res == size && binFileBO == BINHOSTBO )
		return ( binByteWrite( ioType, fd, fp, data, buf, n ));

	if ( (nBytes * count > BINMAXBUF) &&
		((pAlloc = pHold = (unsigned char *)malloc( (unsigned int)n )) == NULL))
		BINERROR( BINEMALLOC, -1 );

	if ( nBytes <= res )
	{
		/*
		 *  Writing same or fewer bytes than stored by host.
		 *  Truncate high-order bytes.
		 */
		thisOne = that = 0x00;
		if ( binTypeSigned[type] )
			that = 0xFF;

		for ( i = 0; i < count; i++ )
		{
#ifdef MBF				/* MBF host			*/
			for ( j = 0; j < size - nBytes; j++, buf++ )
			{
				if ( *buf != thisOne && *buf != that )
				{
					(*binErrFunc)( fd, binOp, BINEINTEGERTRUNC,
						(buf-j), res*8, nBytes*8 );
					buf += size - nBytes - j;
					break;
				}
			}
			for ( j = nBytes; j; j-- )
				*pHold++ = *buf++;
#else					/* LBF host			*/
			for ( j = nBytes - 1; j >= 0; j-- )
				pHold[j] = *buf++;
			pHold += nBytes;

			for ( j = 0; j < size - nBytes; j++, buf++ )
			{
				if ( *buf != thisOne && *buf != that )
				{
					(*binErrFunc)( fd, binOp, BINEINTEGERTRUNC,
						(buf-j-nBytes), res*8, nBytes*8 );
					buf += size - nBytes - j;
					break;
				}
			}
#endif
		}
		if ( pAlloc )
		{
			i = binByteWrite( ioType, fd, fp, data, pAlloc, n );
			free( (char *)pAlloc );
		}
		else
			i = binByteWrite( ioType, fd, fp, data, byteHold, n );
		return ( i );
	}

	/*
	 *  Writing more bytes than stored by host.  Need to sign extend
	 *  or pad with 0's.
	 */
	memset( (void *)pHold, 0x00, n);
	for ( i = 0; i < count; i++ )
	{
#ifdef MBF				/* MBF host			*/
		if ( issigned && (*buf & 0x80) )
			for ( j = nBytes - res; j; j-- )
				*pHold++ = 0xFF;	/* Sign extend	*/
		else
			pHold += nBytes - res;
		buf += size - res;
		for ( j = res; j; j-- )
			*pHold++ = *buf++;
#else					/* LBF host			*/
		if ( issigned && (buf[res-1] & 0x80) )
			for ( j = nBytes - res; j; j-- )
				*pHold++ = 0xFF;	/* Sign extend	*/
		else
			pHold += nBytes - res;
		for ( j = res - 1; j >= 0; j-- )
			pHold[j] = *buf++;
		buf   += size - res;
		pHold += res;
#endif
	}

	if ( pAlloc )
	{
		i = binByteWrite( ioType, fd, fp, data, pAlloc, n );
		free( (char *)pAlloc );
	}
	else
		i = binByteWrite( ioType, fd, fp, data, byteHold, n );
	return ( i );
}

static int				/* Returns # of bytes written	*/
#ifdef __STDC__
binWriteIntLBF( int ioType, int fd, FILE *fp, unsigned char *data,
	unsigned char *buf, int type, int nBytes, int count )
#else
binWriteIntLBF( ioType, fd, fp, data, buf, type, nBytes, count )
	int             ioType;		/* Use buffered or unbuffered I/O*/
	int             fd;		/* File descriptor		*/
	FILE           *fp;		/* File pointer			*/
	unsigned char  *data;		/* Output data			*/
	unsigned char  *buf;		/* Data buffer			*/
	int             type;		/* Type to write		*/
	int             nBytes;		/* # of bytes when in file	*/
	int             count;		/* # of items to write		*/
#endif
{
	int             i, j;		/* Counters			*/
	unsigned char   byteHold[BINMAXBUF];	/* Byte buffer		*/
	unsigned char  *pHold = byteHold;/* Data buffer pointer		*/
	int             res;		/* Resolution of type		*/
	int             size;		/* Size of type			*/
	int             issigned;	/* Is type signed?		*/
	int             n;		/* # of bytes written		*/
	unsigned char  *pAlloc = NULL;	/* Dynamically allocated buffer	*/
	unsigned char	thisOne, that;	/* Byte comparison holders	*/


	size     = binTypeSize[type];	/* Storage size of item		*/
	res      = binTypeRes[type];	/* Significant bytes of item	*/
	issigned = binTypeSigned[type];	/* Whether signed or not	*/
	n        = nBytes * count;


	if ( size == nBytes && res == size && binFileBO == BINHOSTBO )
		return ( binByteWrite( ioType, fd, fp, data, buf, n ));

	if ( (nBytes * count > BINMAXBUF) &&
		((pAlloc = pHold = (unsigned char *)malloc( (unsigned int)n )) == NULL))
		BINERROR( BINEMALLOC, -1 );


	if ( nBytes <= res )
	{
		/*
		 *  Writing same or fewer bytes than stored by host.
		 *  Truncate high-order bytes.
		 */
		thisOne = that = 0x00;
		if ( binTypeSigned[type] )
			that = 0xFF;

		for ( i = 0; i < count; i++ )
		{
#ifdef MBF				/* MBF host			*/
			for ( j = 0; j < size - nBytes; j++, buf++ )
			{
				if ( *buf != thisOne && *buf != that )
				{
					(*binErrFunc)( fd, binOp, BINEINTEGERTRUNC,
						(buf-j), res*8, nBytes*8 );
					buf += size - nBytes - j;
					break;
				}
			}
			for ( j = nBytes - 1; j >= 0; j-- )
				pHold[j] = *buf++;
			pHold += nBytes;
#else					/* LBF host			*/
			for ( j = nBytes; j; j-- )
				*pHold++ = *buf++;
			for ( j = 0; j < size - nBytes; j++, buf++ )
			{
				if ( *buf != thisOne && *buf != that )
				{
					(*binErrFunc)( fd, binOp, BINEINTEGERTRUNC,
						(buf-j-nBytes), res*8, nBytes*8 );
					buf += size - nBytes - j;
					break;
				}
			}
#endif
		}
		if ( pAlloc )
		{
			i = binByteWrite( ioType, fd, fp, data, pAlloc, n );
			free( (char *)pAlloc );
		}
		else
			i = binByteWrite( ioType, fd, fp, data, byteHold, n );
		return ( i );
	}

	/*
	 *  Writing more bytes than stored by host.  Need to sign extend
	 *  or pad with 0's.
	 */
	memset( (void *)pHold, 0x00, n);
	for ( i = 0; i < count; i++ )
	{
#ifdef MBF				/* MBF host			*/
		buf += size - res;
		for ( j = res - 1; j >= 0; j-- )
			pHold[j] = *buf++;
		pHold += res;
		if ( issigned && (*buf & 0x80) )
			for ( j = nBytes - res; j; j-- )
				*pHold++ = 0xFF;	/* Sign extend	*/
		else
			pHold += nBytes - res;
#else					/* LBF host			*/
		for ( j = res; j; j-- )
			*pHold++ = *buf++;
		if ( issigned && (*(buf-1) & 0x80) )
			for ( j = nBytes - res; j; j-- )
				*pHold++ = 0xFF;	/* Sign extend	*/
		else
			pHold += nBytes - res;
		buf += size - res;
#endif
	}

	if ( pAlloc )
	{
		i = binByteWrite( ioType, fd, fp, data, pAlloc, n );
		free( (char *)pAlloc );
	}
	else
		i = binByteWrite( ioType, fd, fp, data, byteHold, n );
	return ( i );
}





/*
 *  FUNCTION
 *	binReadFloat	-  Read float from a input
 *	binWriteFloat	-  Write from float to output
 *
 *  DESCRIPTION
 *	If the host and file floating point formats, sizes of those formats,
 *	and byte orders are the same, then a simple read or writesystem call
 *	is used.
 *
 *	Otherwise, for reads, file data is read in to a holding array and
 *	passed off to binConvertFloat(), one at a time.
 *
 *	For writes, host data is passed to binConvertFloat() to conver into
 *	a holding array, and then written to the file.
 *
 *	NOTE:  This code could be sped up by including binConvertFloat()
 *	code in-line and having different code for different file byte orders.
 *	However, in the interests of reducing the maintenance hassle for the
 *	rather complex code in binConvertFloat(), this has not been done.
 */

static int				/* Returns # of bytes read	*/
#ifdef __STDC__
binReadFloat( int ioType, int fd, FILE *fp, unsigned char *data,
	unsigned char *buf, int type, int nBytes, int count )
#else
binReadFloat( ioType, fd, fp, data, buf, type, nBytes, count )
	int             ioType;		/* Use buffered or unbuffered I/O*/
	int             fd;		/* File descriptor		*/
	FILE           *fp;		/* File pointer			*/
	unsigned char  *data;		/* Input data			*/
	unsigned char  *buf;		/* Data buffer			*/
	int             type;		/* Type to read			*/
	int             nBytes;		/* # of bytes when in file	*/
	int             count;		/* # of items to read		*/
#endif
{
	int             i;		/* Counter			*/
	binFloatFormatCooked *host;	/* Host's float format info	*/
	binFloatFormatCooked *file;	/* File's float format info	*/
	unsigned char   byteHold[BINMAXBUF];	/* Byte buffer		*/
	unsigned char  *pHold  = byteHold;/* Data buffer pointer	*/
	unsigned char  *pAlloc = NULL;	/* Dynamically allocated buffer	*/
	int             size;		/* Size of host type		*/
	int		n;		/* # of bytes read		*/


	size = binTypeSize[type];
	host = binHostFloatFormat;
	do
	{
		if ( host->bin_nbytes == size && host->bin_format.bin_subname )
			break;
	} while ( (++host)->bin_format.bin_number == BINHOSTFLOAT );
	if ( host->bin_format.bin_number != BINHOSTFLOAT )
		BINERROR( BINEFLOATUNSUPPORTED, -1 );
	file = binFileFloatFormat;
	i    = file->bin_format.bin_number;
	do
	{
		if ( file->bin_nbytes == nBytes && file->bin_format.bin_subname)
			break;
	} while ( (++file)->bin_format.bin_number == i );
	if ( file->bin_format.bin_number != i )
		BINERROR( BINEFLOATUNSUPPORTED, -1 );
	n = nBytes * count;


	if ( host == file && size == nBytes && binFileBO == BINHOSTBO )
		return ( binByteRead( ioType, fd, fp, data, buf, n ) );

	if ( (n > BINMAXBUF) &&
		((pAlloc = pHold = (unsigned char *)malloc( (unsigned int)n )) == NULL))
		BINERROR( BINEMALLOC, -1 );
	if ( binByteRead( ioType, fd, fp, data, pHold, n ) == -1 )
	{
		if ( pAlloc )
			free( (char *)pAlloc );
		return ( -1 );
	}

	/* Convert from file format to host format.			*/
	if ( binConvertFloat( fd, file, binFileBO, nBytes, pHold,
		host, BINHOSTBO, size, buf, count ) == -1 )
	{
		if ( pAlloc )
			free( (char *)pAlloc );
		return ( -1 );
	}

	if ( pAlloc )
		free( (char *)pAlloc );

	return ( n );
}

static int				/* Returns # of bytes written	*/
#ifdef __STDC__
binWriteFloat( int ioType, int fd, FILE *fp, unsigned char *data,
	unsigned char *buf, int type, int nBytes, int count )
#else
binWriteFloat( ioType, fd, fp, data, buf, type, nBytes, count )
	int             ioType;		/* Use buffered or unbuffered I/O*/
	int             fd;		/* File descriptor		*/
	FILE           *fp;		/* File pointer			*/
	unsigned char  *data;		/* Output data			*/
	unsigned char  *buf;		/* Data buffer			*/
	int             type;		/* Type to write		*/
	int		nBytes;		/* # of bytes when in file	*/
	int             count;		/* # of items to write		*/
#endif
{
	unsigned char   byteHold[BINMAXBUF];	/* Byte buffer		*/
	unsigned char  *pHold  = byteHold;/* Data buffer pointer	*/
	unsigned char  *pAlloc = NULL;	/* Dynamically allocated buffer	*/
	binFloatFormatCooked *host;	/* Host's float format info	*/
	binFloatFormatCooked *file;	/* File's float format info	*/
	int             size;		/* Size of type			*/
	int             n;		/* # of bytes read		*/
	int		i;		/* Tmp				*/


	size = binTypeSize[type];
	host = binHostFloatFormat;
	do
	{
		if ( host->bin_nbytes == size && host->bin_format.bin_subname )
			break;
	} while ( (++host)->bin_format.bin_number == BINHOSTFLOAT );
	if ( host->bin_format.bin_number != BINHOSTFLOAT )
		BINERROR( BINEFLOATUNSUPPORTED, -1 );
	file = binFileFloatFormat;
	i    = file->bin_format.bin_number;
	do
	{
		if ( file->bin_nbytes == nBytes && file->bin_format.bin_subname)
			break;
	} while ( (++file)->bin_format.bin_number == i );
	if ( file->bin_format.bin_number != i )
		BINERROR( BINEFLOATUNSUPPORTED, -1 );
	n = nBytes * count;


	if ( host == file && size == nBytes && binFileBO == BINHOSTBO )
		return ( binByteWrite( ioType, fd, fp, data, buf, n ) );

	if ( (n > BINMAXBUF) &&
		((pAlloc = pHold = (unsigned char *)malloc( (unsigned int)n )) == NULL))
		BINERROR( BINEMALLOC, -1 );


	/* Convert from host format to file format.			*/
	if ( binConvertFloat( fd, host, BINHOSTBO, size, buf,
		file, binFileBO, nBytes, pHold, count ) == -1 )
	{
		if ( pAlloc )
			free( (char *)pAlloc );
		return ( -1 );
	}

	/* Write out the converted data.				*/
	n = binByteWrite( ioType, fd, fp, data, pHold, n );
	if ( pAlloc )
		free( (char *)pAlloc );
	return ( n );
}






/*
 *  FUNCTION
 *	binConvertFloat	-  convert a float from one format to another
 *
 *  DESCRIPTION
 *	This routine does the actual conversion of data in one floating
 *	point format into another.  It handles differences in sizes of
 *	the exponent and mantissa fields, underflow and overflow conditions,
 *	exponent biasing, and whether or not the redundant most-significant
 *	bit in the mantissa is saved in the format.
 *
 *	The procedure is thus...
 *
 *	1.  Extract out the sign-bit from the source.  It is assumed here
 *	    that the sign-bit is the most-significant bit in the most-
 *	    significant byte of the source data.
 *
 *	2.  Extract out the exponent bits from the source.  The exponent
 *	    is built up into an UNSIGNED 32-BIT INT by shifting and masking.
 *	    It is assumed that the exponent will fit into a 32-bit integer,
 *	    and that the host actually supports such a thing.  The exponent
 *	    is then converted to a SIGNED 32-BIT INT and unbiased.  It is
 *	    assumed that the unbiased exponent will fit into a 32-bit int.
 *
 *	3.  Conditions for extraction of the mantissa from the source are
 *	    figured out.  Primarily this involves checking if the source
 *	    and destination formats both treat the redundant most-significant
 *	    mantissa bit (the "implied bit") the same way.  If they don't,
 *	    then changes in the way the mantissa is copied in to the destination
 *	    must be figured out.  Along the way the signed exponent may be
 *	    altered by adding or subtracting one due to the implied bit's
 *	    existance or non-existance.
 *
 *	4.  The exponent is checked for underflow and overflow.  On underflow
 *	    the destination float is set to 0 and we return.  On overflow,
 *	    the sign and exponent fields are set to the values appropriate
 *	    for the destination format and the mantissa is not copied, thus
 *	    leaving it zero.  If all is OK, the exponent is biased by the
 *	    desination bias.
 *
 *	5.  The mantissa is then copied.  This is non-trivial because the
 *	    source and desination formats may have different numbers of
 *	    bits in the mantissa, and may start the mantissa at different
 *	    bit positions in the most-significant mantissa byte.  So, what
 *	    we have here is a non-aligned bit-copy operation.
 *
 *	    To make this easier (and faster?), the source data is copied,
 *	    one byte at a time, in to a temporary ALIGNED byte array.
 *	    During the copy process the implied bit is set or not-set if
 *	    needed.
 *
 *	    The temp byte array data is then copied in to the destination
 *	    byte array, with further shifting and masking taking place to
 *	    align it appropriately for the destination format.
 *
 *	6.  The exponent is then copied in to the destination and the sign
 *	    bit set if the source was negative.
 *
 *	NOTE:  This code is NOT FAST!  Sorry.  The object was to write a
 *	generic floating point format converter that didn't have to change
 *	for different formats and different source and destination byte
 *	orders.  This code could be sped up most easily by duplicating
 *	this routine four times, one for each combination of source and
 *	destination byte orders, then removing the unnecessary byte order
 *	if's.  This has not been done initially because of the increase in
 *	the maintenance hassle of the code.
 *
 *	Other speed-ups that could be done include:
 *		1.  Watching for the case where the source and desination
 *		    formats are the same, but the byte order differs and
 *		    then just do a byte swaped copy.
 *
 *		2.  If the mantissa will fit into an int (whatever the size
 *		    is on the host), copy it in to an int via shift's and
 *		    masks, then copy it back out into the desination.  This
 *		    would save all the byte copy hassle for single-precision
 *		    floats.
 */

#define MAXTMP		400		/* Max # of bytes in temps	*/
#define MAXMANT		100		/* Max # of bytes in temp mantissa*/

static int				/* Returns status		*/
#ifdef __STDC__
binConvertFloat( int fd, binFloatFormatCooked *srcfmt, int srcbo, int nsrcbytes,
	unsigned char *srcbytes, binFloatFormatCooked *dstfmt, int dstbo,
	int ndstbytes, unsigned char *dstbytes, int count )
#else
binConvertFloat( fd, srcfmt, srcbo, nsrcbytes, srcbytes,
		     dstfmt, dstbo, ndstbytes, dstbytes, count )
	int		fd;		/* File descriptor (for error handler)*/
	binFloatFormatCooked *srcfmt;	/* Source's float format info	*/
	int             srcbo;		/* Source's byte order		*/
	int             nsrcbytes;	/* # of bytes when in src	*/
	unsigned char  *srcbytes;	/* Source data			*/
	binFloatFormatCooked *dstfmt;	/* Desintation's float format info*/
	int             dstbo;		/* Destination's byte order	*/
	int		ndstbytes;	/* # of bytes when in dst	*/
	unsigned char  *dstbytes;	/* Destination data		*/
	int		count;		/* Number of conversions to do	*/
#endif
{
	int             i, j, k;	/* Counters and temps		*/
	int		n;		/* Count of things		*/

	int             negative;	/* Negative float?		*/
	unsigned int    exponent;	/* Exponent			*/
	int             signed_exponent;/* Unbiased signed exponent	*/
	unsigned char	implied_bit;	/* Implied bit of mantissa	*/

	unsigned char  *src;		/* Source bytes pointer		*/
	unsigned char  *dst;		/* Destination bytes pointer	*/
	unsigned char  *tmp;		/* Temporary uchar pointer	*/
	unsigned char	tmpmant[MAXMANT];/* Temporary mantissa array	*/
	unsigned char	tmpsrc[MAXTMP];/* Temporary src float bytes array*/
	unsigned char	tmpdst[MAXTMP];/* Temporary src float bytes array*/
	unsigned char  *allocsrc;	/* Allocated tmpsrc pointer	*/
	unsigned char  *allocdst;	/* Allocated tmpdst pointer	*/

	int		start_byte;	/* Starting byte of mantissa	*/
	unsigned char	mask1;		/* First mantissa byte mask	*/
	unsigned char	mask2;		/* Second mantissa byte mask	*/
	unsigned char	byte1_mask;	/* Special mask for 1st src byte*/
	int		shift;		/* Amount to shift mantissa bytes*/
	int		shift8;		/* 8 - shift			*/


	src      = srcbytes;
	dst      = dstbytes;
	allocsrc = NULL;
	allocdst = NULL;


	/*
	 *  Check the source format's flags to see if there is any special
	 *  pre-processing of the source bytes that needs to be done.
	 */
	if ( srcfmt->bin_format.bin_flags & BINVAXBO )
	{
		/*
		 *  Source float uses the VAX's LBF/MBF mix byte ordering.
		 *  Floating point values are ordered as a list of 16-bit
		 *  quantities, from most-significant to least-significant.
		 *  Within a 16-bit quantity, the first byte is the least-
		 *  significant and the second the most-significant.
		 *
		 *  For example, a single precision float ('f' format)
		 *  is a 4-byte quantity:
		 *
		 *	        7      0
		 *	byte 0  EMMMMMMM    high mantissa bits
		 *	byte 1  SEEEEEEE
		 *	byte 2  MMMMMMMM    low mantissa bits
		 *	byte 3  MMMMMMMM    middle mantissa bits
		 *
		 *  To handle VAX byte ordering of floats, we copy the source
		 *  bytes in to a temporary byte array, swapping every two
		 *  bytes, then set the source byte order to the opposite of
		 *  that selected.  The generic conversion algorithm below
		 *  can handle it from there.
		 */
		src = tmpsrc;
		n   = nsrcbytes * count;
		if ( ( n > MAXTMP ) &&
			((src = allocsrc = (unsigned char *)malloc( (unsigned int)n )) == NULL ) )
			BINERROR( BINEMALLOC, -1 );

		n /= 2;
		tmp = srcbytes;
		do
		{
			*src++  = tmp[1];
			*src++  = tmp[0];
			tmp    += 2;
		} while ( --n );
		src = tmpsrc;
		if ( allocsrc )
			src = allocsrc;
		srcbo = (srcbo == BINLBF) ? BINMBF : BINLBF;
	}


	/*
	 *  Check the destination format's flags to see if there is any special
	 *  post-processing of the destination bytes that needs to be setup.
	 */
	if ( dstfmt->bin_format.bin_flags & BINVAXBO )
	{
		/*
		 *  Destination float uses the VAX's LBF/MBF mix byte ordering.
		 *  See the comment above for details.
		 *
		 *  To handle VAX byte ordering of floats, we set the
		 *  destination pointer to point to a temp array and set the
		 *  destination byte order to the opposite of that selected.
		 *  When we've completed the conversion, we'll copy from
		 *  the temp array in to the real destination, doing byte
		 *  swapping along the way.
		 */
		dst = tmpdst;
		n   = ndstbytes * count;
		if ( ( n > MAXTMP ) &&
			( (dst = allocdst = (unsigned char *)malloc( (unsigned int)n )) == NULL ) )
			BINERROR( BINEMALLOC, -1 );
		dstbo = (dstbo == BINLBF) ? BINMBF : BINLBF;
	}


	/*
	 *  Zero out the dst float as a starting point.
	 */
	memset( (void *)dst, 0x00, ndstbytes * count);


	/*
	 *  Loop through the conversions to do.
	 */
	n = count;
	do
	{
		/*
		 *  Check the sign bit.
		 *	Assume sign bit is high bit in most-significant byte.
		 */
		if ( srcbo == BINMBF )
		{
			if ( (src[0] & 0x80) == 0 )
				negative = FALSE;
			else
				negative = TRUE;
		}
		else
		{
			if ( (src[nsrcbytes-1] & 0x80) == 0 )
				negative = FALSE;
			else
				negative = TRUE;
		}

		/*
		 *  Extract and unbias the exponent.
		 *	Assume won't overflow unsigned int.
		 */
		if ( srcbo == BINMBF )
		{
			i = nsrcbytes - 1 - srcfmt->bin_expStartByte;
			k = nsrcbytes - 1 - srcfmt->bin_expEndByte;
			exponent = src[i--] >> srcfmt->bin_expShift;
			for ( j = (8-srcfmt->bin_expShift); i > k; j += 8 )
				exponent |= ((unsigned int)src[i--]) << j;
			exponent |= ((unsigned int)src[i] & srcfmt->bin_expEndMask) <<j;
		}
		else
		{
			i = srcfmt->bin_expStartByte;
			k = srcfmt->bin_expEndByte;
			exponent = src[i++] >> srcfmt->bin_expShift;
			for ( j = (8-srcfmt->bin_expShift); i < k; j += 8 )
				exponent |= ((unsigned int)src[i++]) << j;
			exponent |= ((unsigned int)src[i] & srcfmt->bin_expEndMask) <<j;
		}
		if ( exponent == 0 )
		{
			/*
			 *  Original float's biased exponent was zero.
			 *  By definition, regardless of implied bit handling
			 *  or exponent biasing, this is a zero float.  Leave
			 *  the dst float all zeros.
			 */
			src += nsrcbytes;
			dst += ndstbytes;
			continue;
		}
		signed_exponent = exponent - srcfmt->bin_format.bin_expBias;

		/*
		 *  Set up for copying the mantissa into the temporary array.
		 */
		shift = srcfmt->bin_expShift;
		if ( srcfmt->bin_format.bin_mantImplied != dstfmt->bin_format.bin_mantImplied )
		{
			/*
			 *  Dest and src formats don't both handle the implied
			 *  bit the same.
			 *
			 *  If the src format has one, but the dst doesn't,
			 *  remove the bit during the copy and subtract one
			 *  from the exponent.
			 *
			 *  If the dst format has one, but the src format
			 *  doesn't, add the bit during the copy and add one
			 *  to the exponent.
			 */
			if ( srcfmt->bin_format.bin_mantImplied )
			{
				/* Source has implied bit.  Drop for dst.*/
				if ( shift == 0 )
				{
					shift = 7;
					mask1 = 0x7F;
					mask2 = 0x80;
					start_byte = srcfmt->bin_expStartByte-1;
				}
				else
				{
					shift--;
					mask1 = (~srcfmt->bin_expStartMask) >>1;
					mask2 = ~mask1;
					start_byte  = srcfmt->bin_expStartByte;
				}
				implied_bit = 0;
				signed_exponent--;
				if ( shift != 0 )
					byte1_mask = mask1;
				else
					byte1_mask = mask2;
			}
			else
			{
				/* Destination has implied bit.  Add from src.*/
				if ( shift == 7 )
				{
					shift = 0;
					start_byte = srcfmt->bin_expStartByte+1;
					byte1_mask = 0x7F;
				}
				else
				{
					shift++;
					mask2 = srcfmt->bin_expStartMask<<1;
					mask1 = ~mask2;
					start_byte = srcfmt->bin_expStartByte;
					byte1_mask = ~srcfmt->bin_expStartMask;
				}
				implied_bit = 0x80;
				signed_exponent++;
			}
		}
		else
		{
			/*
			 *  The dst and src formats both treat the implied
			 *  bit the same.
			 */
			mask2 = srcfmt->bin_expStartMask;
			mask1 = ~mask2;
			start_byte = srcfmt->bin_expStartByte;
			implied_bit = 0;
			if ( shift != 0 )
				byte1_mask = mask1;
			else
				byte1_mask = mask2;
		}

		/*
		 *  Bias the exponent to the dst format and check for
		 *  overflow and underflow.
		 *	Assume won't overflow int32.
		 */
		signed_exponent += dstfmt->bin_format.bin_expBias;
		if ( signed_exponent < 0 )
		{
			/*
			 *  Underflow.  Float in src had an exponent too
			 *  small to represent in the dst's format.  Leave
			 *  the dst float all zero's.
			 */
			src += nsrcbytes;
			dst += ndstbytes;

			/*
			 *  Report the error
			 */
			(*binErrFunc)( fd, binOp, BINEFLOATUNDERFLOW,
				&srcbytes[(count-n)*nsrcbytes],
				srcfmt->bin_format.bin_expBits,
				dstfmt->bin_format.bin_expBits );
			continue;
		}

		exponent = signed_exponent;
		if ( (exponent & ~dstfmt->bin_expMask) != 0 )
		{
			/*
			 *  Overflow.  Float in src had an exponent too
			 *  large to represent in the dst's format.  Set
			 *  the dst float to its overflow exponent and
			 *  sign.  Leave the mantissa zeroes.
			 */
			exponent = dstfmt->bin_format.bin_expOverflow;
			negative = dstfmt->bin_format.bin_signOverflow;;

			/*
			 *  Report the error
			 */
			(*binErrFunc)( fd, binOp, BINEFLOATOVERFLOW,
				&srcbytes[(count-n)*nsrcbytes],
				srcfmt->bin_format.bin_expBits,
				dstfmt->bin_format.bin_expBits );
		}
		else
		{
			/*
			 *  Zero out temporary mantissa byte array.  This
			 *  handles the filling in of zeros for mantissa
			 *  bytes in the dst format that are not provided by
			 *  the src format.
			 */
			memset( (void *)tmpmant, 0x00, MAXMANT);


			/*
			 *  Copy the src format bytes into the temporary array.
			 *  Bytes in the array are "left-justified" to bring
			 *  the most significant mantissa bit (perhaps the
			 *  implied bit) in to the most significant bit of
			 *  the first byte, and so on down the line.
			 *
			 *  Note:  the temporary array is in MBF format
			 *  regardless of the source format byte order.
			 */
			tmp = tmpmant;
			if ( shift != 0 )
			{
				shift8 = 8 - shift;
				if ( srcbo == BINMBF )
				{
					k = nsrcbytes - 1;
					j = k - start_byte;
					*tmp++ = ((src[j]&byte1_mask)<<shift8) |
						 ((src[j+1]&mask2)>>shift)     |
						 implied_bit;
					for ( i = j+1; i < k; i++ )
						*tmp++ = ((src[i] & mask1)<<shift8) |
							  ((src[i+1] & mask2) >> shift);
					*tmp = (src[i] & mask1) << shift8;
				}
				else
				{
					*tmp++ = ((src[start_byte] & byte1_mask)<<shift8) |
						 ((src[start_byte-1] & mask2)>>shift)  |
						 implied_bit;
					for ( i = start_byte - 1; i > 0; i-- )
						*tmp++ = ((src[i] & mask1)<<shift8) |
							  ((src[i-1] & mask2) >> shift);
					*tmp = (src[0] & mask1) << shift8;
				}
			}
			else
			{
				if ( srcbo == BINMBF )
				{
					*tmp++ = (src[nsrcbytes-start_byte] & byte1_mask) | implied_bit;
					for ( i = nsrcbytes-start_byte+1; i < nsrcbytes; i++ )
						*tmp++ = src[i];
				}
				else
				{
					*tmp++ = (src[start_byte-1] & byte1_mask) | implied_bit;
					for ( i = start_byte - 2; i >= 0; i-- )
						*tmp++ = src[i];
				}
			}
			tmp = tmpmant;

			/*
			 *  Copy the temporary array into the dst's mantissa.
			 */
			shift = dstfmt->bin_expShift;
			if ( shift != 0 )
			{
				mask2 = dstfmt->bin_expStartMask;
				mask1 = ~mask2;
				shift8= 8 - shift;
				if ( dstbo == BINMBF )
				{
					j = ndstbytes - 1;
					for ( i = j - dstfmt->bin_expStartByte; i < j; i++)
					{
						dst[i]   |= (*tmp >> shift8)&mask1;
						dst[i+1] |= (*tmp++<< shift)&mask2;
					}
					dst[i] |= (*tmp >> shift8) & mask1;
				}
				else
				{
					for ( i = dstfmt->bin_expStartByte; i > 0; i-- )
					{
						dst[i]   |= (*tmp >> shift8)&mask1;
						dst[i-1] |= (*tmp++<< shift)&mask2;
					}
					dst[0] |= (*tmp >> shift8) & mask1;
				}
			}
			else
			{
				if ( dstbo == BINMBF )
					for ( i = ndstbytes-dstfmt->bin_expStartByte; i < ndstbytes; i++ )
						dst[i] = *tmp++;
				else
					for ( i = dstfmt->bin_expStartByte-1; i >= 0; i-- )
						dst[i] = *tmp++;
			}
		}


		/*
		 *  Build the exponent and add in the sign.
		 */
		if ( dstbo == BINMBF )
		{
			i = ndstbytes - 1 - dstfmt->bin_expStartByte;
			j = ndstbytes - 1 - dstfmt->bin_expEndByte;
			dst[i--] |= (exponent << dstfmt->bin_expShift) &
				dstfmt->bin_expStartMask;
			exponent >>= (8 - dstfmt->bin_expShift);
			while ( i >= j )
			{
				dst[i--] |= exponent & 0xFF;
				exponent >>= 8;
			}
			dst[0] &= dstfmt->bin_expEndMask;
			if ( negative )
				dst[0] |= 0x80;
		}
		else
		{
			i = dstfmt->bin_expStartByte;
			j = dstfmt->bin_expEndByte;
			dst[i++] |= (exponent << dstfmt->bin_expShift) &
				dstfmt->bin_expStartMask;
			exponent >>= (8 - dstfmt->bin_expShift);
			while ( i <= j )
			{
				dst[i++] = exponent & 0xFF;
				exponent >>= 8;
			}
			dst[i-1] &= dstfmt->bin_expEndMask;
			if ( negative )
				dst[ndstbytes-1] |= 0x80;
		}

		src += nsrcbytes;
		dst += ndstbytes;
	} while ( --n );


	/*
	 *  Check the destination format's flags to see if there is any special
	 *  post-processing of the destination bytes that needs to be done.
	 */
	if ( dstfmt->bin_format.bin_flags & BINVAXBO )
	{
		/*
		 *  Destination float uses the VAX's LBF/MBF mix byte ordering.
		 *
		 *  To handle VAX byte ordering of floats, we copy the temporary
		 *  destination bytes (set up at the top of this function)
		 *  in to the destination byte array, swapping every two
		 *  bytes.
		 */
		if ( allocdst )
		{
			dst = allocdst;
			i   = count * ndstbytes / 2;
			do
			{
				*dstbytes++ = dst[1];
				*dstbytes++ = dst[0];
				dst += 2;
			} while ( --i );
			free( (char *)allocdst );
		}
		else
		{
			dst = tmpdst;
			i   = count * ndstbytes / 2;
			do
			{
				*dstbytes++ = dst[1];
				*dstbytes++ = dst[0];
				dst += 2;
			} while ( --i );
		}
	}

	if ( allocsrc )
		free( (char *)allocsrc );

	return ( 0 );
}
