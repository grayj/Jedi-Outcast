/**
 **	$Header: /sdsc/dev/vis/image/imtools/v3.0/libim/src/include/RCS/iminternal.h,v 1.15 1995/06/30 22:13:26 bduggan Exp $
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

/**
 **  FILE
 **	iminternal.h		-  Internal header definitions for libim
 **
 **  PROJECT
 **	libim	-  SDSC image manipulation library
 **
 **  DESCRIPTION
 **	iminternal.h contains macros common to most file read and
 **	write functions.  It should not be used outside of the scope
 **	of libim read and write routines.
 **
 **  PUBLIC CONTENTS
 **			d =defined constant
 **			f =function
 **			m =defined macro
 **			t =typedef/struct/union
 **			v =variable
 **			? =other
 **
 **
 **	__IMINTERNALH__		d  file inclusion flag
 **	
 **	ImReturnBinError	m  return right error based on Bin call
 **	ImMalloc		m  malloc memory & return -1 on error
 **	ImMallocRetOnError	m  malloc memory & return specified value on error
 **	ImCalloc		m  calloc memory & return -1 on error
 **	ImCallocRetOnError	m  calloc memory & return specified value on error
 **	ImBinReadStruct		m  binary struct read from fd or fp
 **	ImBinRead		m  binary read from fd or fp
 **	ImBinWrite		m  binary write from fd or fp
 **	ImSeek			m  file seek from fd or fp
 **
 **	ImErrorFatal		m  issue a fatal error
 **	ImErrorWarning		m  issue a warning error
 **	ImErrorInfo		m  issue an info error
 **	ImInfo			m  issue general information message
 **
 **	NULL			d  an empty pointer
 **     xxxxx			d  abbreviations for items in Read/Write maps
 **
 **	ImHist			?  stuff for imvfbhist.c
 **
 **  PRIVATE CONTENTS
 **	none
 **
 **  HISTORY
 **	$Log: iminternal.h,v $
 **	Revision 1.15  1995/06/30  22:13:26  bduggan
 **	removed strings.h
 **
 **	Revision 1.14  1995/06/29  00:32:03  bduggan
 **	updated copyright
 **
 **	Revision 1.13  1995/06/16  09:04:16  bduggan
 **	Added prototypes for
 **	ImGetTransparency,ImVfbProcessMapRequests,imLzwReadByte,
 **	imLzwCompGif,PackBits,UnPackBits,UnpackBits,UnpackBits3
 **	Added ImErrorFatalExit
 **	Added IM_NOTHING
 **	Added some include files
 **
 **	Revision 1.12  1995/05/18  00:08:28  bduggan
 **	Changed 'return ( ret ) ' into 'return ret' so that
 **	the parameter IM_NOTHING can be passed to macros.
 **
 **	Revision 1.11  1995/01/18  21:52:23  bduggan
 **	Added ImCallocRetOnError macros
 **	added #include <stdlib.h> and prototypes for strlen and strcpy
 **
 **	Revision 1.10  94/10/03  16:05:10  nadeau
 **	Added definitions of TRUE, FALSE, L_SET et al, and F_SET et al for
 **	internal routine use.
 **	Updated copyright message.
 **	
 **	Revision 1.9  92/12/03  02:19:42  nadeau
 **	Updated histogram information.
 **	
 **	Revision 1.8  92/11/06  17:02:24  groening
 **	added #defines for format reading
 **	
 **	Revision 1.7  92/10/12  16:12:52  vle
 **	Added ImInfo declarations.
 **	
 **	Revision 1.6  92/09/23  15:27:50  groening
 **	Added histogram declarations.
 **	
 **	Revision 1.5  91/10/03  12:57:05  nadeau
 **	Added error code to ImErrorHandler() call.
 **	
 **	Revision 1.4  90/07/23  13:52:38  nadeau
 **	Added curly braces to ImMalloc macro.  Added ImTell macro.
 **	
 **	Revision 1.3  90/07/02  13:24:00  nadeau
 **	Added new error handling macros and globals and updated existing macros
 **	to use them.
 **	
 **	Revision 1.2  90/06/26  09:10:39  todd
 **	Useful macros for writing image file read and write routines.
 **	These should be taken out of imras.c someday.
 ** 
 **	Revision 1.1  90/05/15  14:00:06  todd
 **	Initial revision
 ** 
 **
 **/

#ifndef __IMINTERNALH__
#define __IMINTERNALH__

#ifndef __SDSCCOPYRIGHTH__
#include "sdsccopyright.h"
#endif

#include <errno.h>
extern int sys_nerr;
extern char *sys_errlist[];

#include <stdio.h>
#include <stdlib.h>

#include "unistd.h"
#include <fcntl.h>
#include <ctype.h>

#ifndef __SDSCH__
#include "sdsc.h"
#endif /* __SDSCH__ */

#ifndef __IMH__
#include "im.h"
#endif /* __IMH__ */

#ifndef NULL
#define NULL	(0)
#endif /* NULL */

#ifndef TRUE
#define TRUE	(1)
#endif

#ifndef FALSE
#define FALSE	(0)
#endif

#include <string.h>



/*
 *  CONSTANTS
 *	L_*	-  file descriptor seek types
 *	F_*	-  file pointer seek types
 *
 *  DESCRIPTION
 *	Not all OS variants define values for the lseek() and fseek()
 *	seek types, though the actual type values are highly standardized
 *	by existing heavy use as raw numbers.  If this OS hasn't defined
 *	constants for these, we do.
 */
#ifndef L_SET
#define L_SET   0               /* Absolute offset                      */
#define L_INCR  1               /* Relative to current offset           */
#define L_XTND  2               /* Relative to end of file              */
#endif

#ifndef F_SET
#define F_SET   0               /* Absolute offset                      */
#define F_INCR  1               /* Relative to current offset           */
#define F_XTND  2               /* Relative to end of file              */
#endif





/*
 *  CONSTANTS
 *	XXX	-  abbreviations for Read/Write map fields
 *
 *  DESCRIPTION
 *	These constants are simply shorthands for flags used in initialization
 *	of the file format read/write maps.  They are only provided so that
 *	such initializations don't look too cluttered.
 */
#define C       IMCLTYES                /* CLT included                 */
#define A       IMALPHAYES              /* Alpha plane included         */

#define RLE     IMCOMPRLE               /* RLE compression              */
#define LZW     IMCOMPLZW               /* LZW compression              */
#define PB      IMCOMPPACKBITS          /* Packbits compression         */
#define DCT     IMCOMPDCT               /* DCT compression              */
#define ASC	IMCOMPASCII		/* ASCII compression            */

#define LI      IMINTERLINE             /* Line interleaved             */
#define PI      IMINTERPLANE            /* Plane interleaved            */

#define IN      IMTYPEINDEX             /* Index type of image          */
#define RGB     IMTYPERGB               /* RGB type of image            */

#define T	IMGROUPTILES		/* Group into tiles		*/

#define Q	IMQUALITYYES		/* Optional quality control     */



/*
 *  CONSTANTS
 *	IMFILEIO...	-  Image file I/O type
 *
 *  DESCRIPTIONS
 *	These constants are bitwise OR-ed together to create the ioType
 *	argument to the internal per-format read and write routines.
 *	They should never be used by the applications programmer.
 */

#define IMFILEIOFD	0x1		/* Use file descriptor		*/
#define IMFILEIOFP	0x2		/* Use file pointer		*/

#define IMFILEIOFILE	0x10		/* I/O on file			*/
#define IMFILEIOPIPE	0x20		/* I/O on pipe			*/





/*
 *  MACROS
 *	ImReturnBinError	-  set correct error based on Bin call, and return -1
 *	ImReturnValBinError	-  Same as above, but return specified value
 *	ImCalloc		-  calloc memory & return -1 on error
 *	ImCallocRetOnError	-  calloc memory & return specified value on error
 *	ImMalloc		-  malloc memory & return -1 on error
 *	ImMallocRetOnError	-  malloc memory & return specified value on error
 *	ImBinReadStruct		-  binary struct read from fd or fp
 *	ImBinRead		-  binary read from fd or fp
 *	ImBinWrite		-  binary write from fd or fp
 *	ImSeek			-  file seek from fd or fp
 *
 *  DESCRIPTION
 *	These actions occur so often in the imfile read/write code that they 
 *	have been turned into macros in order to reduce clutter and make the
 *	algorithms more apparent.
 */

#define ImReturnBinError()							\
	ImReturnValBinError( -1 )

#define ImReturnValBinError( ret )					\
{									\
	if ( BinErrNo == BINEMALLOC )					\
	{								\
		ImErrorFatal( BinQError( ), ret, IMEMALLOC );		\
	}								\
	else								\
	{								\
		ImErrorFatal( BinQError( ), ret, IMESYS );		\
	}								\
}


#define ImMallocRetOnError(variable,type,nbytes,retvalue)		\
{									\
	if ( ((variable) = (type)malloc( (nbytes) )) == NULL )		\
	{								\
		ImErrNo = IMEMALLOC;					\
		ImErrorFatal( ImQError( ), retvalue, IMEMALLOC );	\
	}								\
}



#define ImMalloc(variable,type,nbytes)					\
	ImMallocRetOnError(variable,type,nbytes,-1)


#define ImCallocRetOnError(variable,type,nbytes,size,errvalue)		\
{																	\
	if ( ((variable) = (type)calloc( (nbytes),(size) )) == NULL )	\
	{																\
		ImErrNo = IMEMALLOC;										\
		ImErrorFatal( ImQError( ), errvalue, IMEMALLOC );			\
	}																\
}


#define ImCalloc(variable,type,nbytes,size)				\
	ImCallocRetOnError(variable,type,nbytes,size,-1)

#define ImBinReadStruct(ioType,fd,fp,buf,fields)		\
	(((ioType)&IMFILEIOFD) ?							\
		BinReadStruct( (fd), (buf), (fields) )			\
	:													\
		BinFReadStruct( (fp), (buf), (fields) )			\
	)

#define ImBinRead(ioType,fd,fp,buf,type,nbytes,len)			\
	(((ioType)&IMFILEIOFD) ?								\
		BinRead( (fd), (buf), (type), (nbytes), (len) )		\
		:													\
		BinFRead( (fp), (buf), (type), (nbytes), (len) )	\
	)

#define ImBinWriteStruct(ioType,fd,fp,buf,fields)		\
	(((ioType)&IMFILEIOFD) ?							\
		BinWriteStruct( (fd), (buf), (fields) )			\
		:												\
		BinFWriteStruct( (fp), (buf), (fields) )		\
	)

#define ImBinWrite(ioType,fd,fp,buf,type,nbytes,len)		\
	(((ioType)&IMFILEIOFD) ?								\
		BinWrite( (fd), (buf), (type), (nbytes), (len) )	\
		:													\
		BinFWrite( (fp), (buf), (type), (nbytes), (len) )	\
	)

#define ImSeek(ioType,fd,fp,offset,type)			\
	(((ioType)&IMFILEIOFD) ?						\
		lseek( (fd), (offset), (type) )				\
		:											\
		fseek( (fp), (offset), (type) )				\
	)

#define ImTell(ioType,fd,fp)					\
	(((ioType)&IMFILEIOFD) ?					\
		lseek( (fd), 0, 1 )						\
		:										\
		ftell( (fp) )							\
	)


/*
 *  MACROS and CONSTANTS
 *	ImErrorFatal		-  issue a fatal error and return a value
 *	ImErrorFatalExit	-  issue a fatal error and exit the process
 *	ImErrorWarning		-  issue a warning error and possibly return a value
 *	ImErrorInfo		-  issue an info error
 *	ImInfo			-  issue an information message
 *	IMINFOMSGWIDTH...	-  Image Info Message Widths
 *	IMINFOMSGLENGTH		-  Image Info Message Length
 *	IM_NOTHING		-  Nothing
 *
 *  DESCRIPTION
 *	These macros cover up some of the busy-work of issueing error
 *	and info messages and codes while processing file formats.
 *
 *	IM_NOTHING can be used with the error macros, if your
 *	function does not return a value.
 */

#define IM_NOTHING	/* nothing */

extern char *ImErrorProgramName;		/* Program name		    */
extern char *ImErrorFileName;			/* File name		    */
extern FILE *ImErrorStream;			/* Error stream		    */
extern FILE *ImInfoStream;			/* Info stream		    */
#ifdef __STDC__
extern int (*ImErrorHandler)(int, int, char* );	/* Error handler	    */
extern int (*ImInfoHandler)(char*,char*,char*);	/* Info handler	    	    */
#else
extern int (*ImErrorHandler)( );		/* Error handler	    */
extern int (*ImInfoHandler)( );			/* Info handler	    	    */
#endif

#define ImErrorFatal(msg,ret,er)					\
{									\
	if ( ImErrorHandler )						\
	{								\
		char errorMessage[500];					\
		sprintf( errorMessage, "%s: %s: %s\n", ImErrorProgramName,\
			ImErrorFileName, (msg) );			\
		(*ImErrorHandler)( IMERRORFATAL, er, errorMessage );	\
		ImErrNo = (er);						\
		return ret ;						\
	}								\
	else if ( ImErrorStream )					\
	{								\
		fprintf( ImErrorStream, "%s: %s: %s\n", ImErrorProgramName,\
			ImErrorFileName, (msg) );			\
		ImErrNo = (er);						\
		return ret ;						\
	}								\
	else								\
	{								\
		ImErrNo = (er);						\
		return ret ;						\
	}								\
}

#define ImErrorWarning(msg,ret,er)					\
{									\
	if ( ImErrorHandler )						\
	{								\
		char errorMessage[500];					\
		sprintf( errorMessage, "%s: %s: %s\n", ImErrorProgramName,\
			ImErrorFileName, (msg) );			\
		if ( (*ImErrorHandler)( IMERRORWARNING, er, errorMessage ) == -1 )\
		{							\
			ImErrNo = (er);					\
			return ret ;					\
		}							\
	}								\
	else if ( ImErrorStream )					\
		fprintf( ImErrorStream, "%s: %s: %s\n", ImErrorProgramName,\
			ImErrorFileName, (msg) );			\
	else								\
	{								\
		ImErrNo = (er);						\
		return ret ;						\
	}								\
}


#define ImErrorInfo(msg,ret,er)						\
{									\
	if ( ImErrorHandler )						\
	{								\
		char errorMessage[500];					\
		sprintf( errorMessage, "%s: %s: %s\n", ImErrorProgramName,\
			ImErrorFileName, (msg) );			\
		if ( (*ImErrorHandler)( IMERRORINFO, er, errorMessage ) == -1 )\
		{							\
			ImErrNo = (er);					\
			return ret ;					\
		}							\
	}								\
	else if ( ImErrorStream )					\
		fprintf( ImErrorStream, "%s: %s: %s\n", ImErrorProgramName,\
			ImErrorFileName, (msg) );			\
}


#define ImErrorFatalExit(msg,status,er)					\
{									\
	if ( ImErrorHandler )						\
	{								\
		char errorMessage[500];					\
		sprintf( errorMessage, "%s: %s: %s\n", ImErrorProgramName,\
			ImErrorFileName, (msg) );			\
		(*ImErrorHandler)( IMERRORFATAL, er, errorMessage );	\
		exit (status) ;						\
	}								\
	else if ( ImErrorStream )					\
	{								\
		fprintf( ImErrorStream, "%s: %s: %s\n", ImErrorProgramName,\
			ImErrorFileName, (msg) );			\
		exit (status) ;						\
	}								\
	else								\
	{								\
		exit (status) ;						\
	}								\
}

#define IMINFOMSGWIDTH1 20


#define ImInfo(label,msg)						\
{									\
	char infoMessage[500];						\
	char infoLabel[IMINFOMSGWIDTH1+2];				\
	sprintf( infoLabel, "%s:", label );				\
	sprintf( infoMessage, "%-*s %s\n", IMINFOMSGWIDTH1, infoLabel, msg );\
	if ( ImInfoHandler )						\
		(*ImInfoHandler)( ImErrorProgramName, ImErrorFileName,	\
			infoMessage );					\
	else if ( ImInfoStream )					\
		fprintf( ImInfoStream, "%s: %s: %s", ImErrorProgramName,\
			ImErrorFileName, (infoMessage) );		\
}

/*
 * Following are some functions that are used in a variety of places
 * within the library.
 */
#ifdef __STDC__
extern int ImGetTransparency(TagTable* tagTable, TagTable* flagsTable, ImVfb* vfb);
extern int ImVfbProcessMapRequests( TagTable* flagsTable, TagTable* tagTable);
extern int imLzwReadByte(int ioType,int fd,FILE *fp,int flag,int input_code_size);
extern int imLzwCompGif(int ioType,int fd,FILE *fp,int code_size, unsigned char *rasterdata, int size);
extern void PackBits( unsigned char *cIn, unsigned char *cOut, unsigned int *width );
extern void PackBits3( unsigned char *cIn, unsigned char *cOut, unsigned int * width );
extern void UnpackBits( unsigned char *srcPtr, unsigned char *dstPtr, unsigned int *cnt );
extern void UnpackBits3( unsigned char *srcPtr, unsigned char *dstPtr, unsigned int *cnt );

#else
extern int ImGetTransparency( ); 	/* declared in imvfb.c     */
extern int ImVfbProcessMapRequests( ); 	/* declared in imvfbchan.c */
extern int imLzwReadByte();		/* declared in imgiflzw.c  */
extern int imLzwCompGif();		/* declared in imgiflzw.c  */
extern void PackBits();			/*             macpack.c   */
extern void UnPackBits();		/*             macpack.c   */
extern void UnpackBits( );		/*             macpack.c   */
extern void UnpackBits3( );		/*             macpack.c   */
#endif

#endif /* __IMINTERNALH__ */
