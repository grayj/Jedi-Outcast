/**

 **	$Header: /roq/libim/imerrno.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imerrno.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imerrno.c	-  Global Variables regarding errors

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imerrno.c contains several global variables related to errors

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **	ImErrorProgramName	v  program name

 **	ImErrorFileName		v  file name

 **	ImErrorHandler		v  error handler function pointer

 **	ImErrorStream		v  error message output stream

 **

 **	ImErrNo		v  error number

 **	ImNNrr		v  number of error messages

 **	ImErrList	v  error messages

 **

 **	NOTE:	Don't add any functions to this file!  This file was 

 **	created because sun requires that global variables be

 **	isolated in seperate files when making shared objects!

 **

 **  PRIVATE CONTENTS

 **	none

 **

 **  HISTORY

 **	$Log: /roq/libim/imerrno.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 * Revision 1.2  1995/06/29  00:28:04  bduggan

 * updated copyright year

 *

 * Revision 1.1  1995/06/16  08:34:53  bduggan

 * Initial revision

 *

 **	

 **/



#include "iminternal.h"





/**

 **  CODE CREDITS

 **	Custom development, Dave Nadeau, San Diego Supercomputer Center, 1990.

 **	Custom development, Brian Duggan, San Diego Supercomputer Center, 1990.

 **/











/*

 *  GLOBAL VARIABLE

 *	ImErrorProgramName	-  program name

 *	ImErrorFileName		-  file name

 *	ImErrorHandler		-  error handler function pointer

 *	ImErrorStream		-  error message output stream

 *	ImInfoHandler		-  info handler function pointer

 *	ImInfoStream		-  info message output stream

 *

 *  FUNCTION

 *	These globals are used by the ImError* macros in the reporting of

 *	errors.  All are set by quering a flagsTable using ImErrorFlags().

 */



char *ImErrorProgramName = "program";		/* Program name		    */

char *ImErrorFileName = "stream";		/* File name		    */

FILE *ImErrorStream = NULL;			/* Error stream		    */

FILE *ImInfoStream = NULL;			/* Info stream		    */

#ifdef __STDC__

int (*ImErrorHandler)(int, int, char* ) = NULL;	/* Error handler	    */

int (*ImInfoHandler)(char*,char*,char* )= NULL;	/* Info handler	 	    */

#else

int (*ImErrorHandler)( ) = NULL;		/* Error handler	    */

int (*ImInfoHandler)( ) = NULL;			/* Info handler	 	    */

#endif











/*

 *  GLOBAL VARIABLE

 *	ImErrNo		-  error number

 *	ImNNrr		-  number of error messages

 *	ImErrList	-  error messages

 *

 *  DESCRIPTION

 *	On an error, the image manipulation routines return -1 and set

 *	ImErrNo to an error code.  The programmer may call ImPError

 *	to print the associated error message to stderr, or may do the

 *	message lookup in ImErrList themselves.

 */

int ImErrNo = -1;		/* VFB package error number		*/

char *ImErrList[] =		/* Error message list			*/

{

	/*  0 IMESYS */		"System call error;  see errno",

	/*  1 IMEMALLOC */	"Cannot allocate host memory",



	/*  2 IMENOCONTENTS */	"VFB contents mask == 0?",

	/*  3 IMENOFPDATA */	"No floating point data in VFB",

	/*  4 IMEBADINOUT */	"Illegal value for 'inout'",

	/*  5 IMEBADFBTYPE */	"Bad frame buffer type",

	/*  6 IMEFORMAT */	"Bad image file format selection",

	/*  7 IMENOFILE */	"File cannot be opened",

	/*  8 IMENOTINFO */	"Not enough information in VFB",

	/*  9 IMEBADFLIP */	"Illegal flip direction for VFB",

	/* 10 IMEBADALGORITHM */"Bad resolution change algorithm",



	/* 11 IMENOREAD */	"Read operation not possible on this image format",

	/* 12 IMENOWRITE */	"Write operation not possible on this image format",

	/* 13 IMENOVFB */	"No VFB provided to a function that needs one",

	/* 14 IMEMANYFB */	"Too many VFB's provided to a function that needs only one",

	/* 15 IMENOTRGB */	"RGB VFB image required",

	/* 16 IMEMAGIC */	"Bad magic number on image file",

	/* 17 IMEIMAGETYPE */	"Unknown image type",

	/* 18 IMECLTTYPE */	"Unknown CLT type",

	/* 19 IMEDEPTH */	"Unknown image depth",

	/* 20 IMECLTLENGTH */	"Bad CLT length",

	/* 21 IMENOCLT */	"No CLT provided to a function than needs one",

	/* 22 IMEMANYCLT */	"Too many CLT's provided to a function that needs only one",

	/* 23 IMEUNKNOWN */	"Unknown",

	/* 24 IMENULLTAGTABLE */"Null tag table given",

	/* 25 IMESYNTAX */	"Syntax error",

	/* 26 IMENOIMAGE */	"No image present in input file",

	/* 27 IMEUNSUPPORTED */	"VFB type unsupported for this operation",

	/* 28 IMEDIFFSIZE */	"VFB's have different X and Y dimensions",

	/* 29 IMEVERSION    */	"Bad version number in header",

	/* 30 IMEPLANES     */  "Unknown image plane config",

	/* 31 IMEOUTOFRANGE */  "Header value out of legal range",

	/* 32 IMEORIENTATION*/  "Unsupported image orientation",

	/* 33 IMEWIDTH      */  "Zero or negative image width",

	/* 34 IMEHEIGHT     */  "Zero or negative image height",

	/* 35 IMECONFLICT   */  "Conflicting info in im header",

	/* 36 IMENOTINDEX8 */	"Index 8 VFB image required",

	/* 37 IMENOTINDEX16 */	"Index 16 VFB image required",

	/* 38 IMENOTMONO */	"Mono VFB image required",

	/* 39 IMEFIELD */	"Bad field mask",

	/* 40 IMENOTPOSSIBLE */	"Image conversion/output not possible as specified",

	/* 41 IMEDECODING */	"Error encountered while decoding image",

	/* 42 IMEENCODING */	"Error encountered while encoding image",

	/* 43 IMEOPERATION */	"Unknown operation",

	/* 44 IMEGRADUATION */	"Unknown graduation",

	/* 45 IMEHISTMAX */	"Maximum number of unqiue colors exceeded given parameter",

	/* 46 IMEBADRANGE */	"Inappropriate use of value range",

	/* 47 IMEKEY */		"Invalid keying parameter",

	/* 48 IMEADJUST */	"Invalid adjust parameter",

	/* 49 IMENOALPHA */	"No alpha field present when needed",

	/* 50 IMEPIXELREP */	"Source and destination are not exact multiples of each other for pixel replication",

	/* 51 IMEKEYFIELD */	"Bad keyField on pixel adjustment",

	/* 52 IMEADJUSTFIELD */	"Bad adjustField on pixel adjustment",

	/* 53 IMEIMPSHEAR */	"Impossible shear attempted (<-90 or >90)",

	/* 54 IMEBADCHANNEL */	"Invalid channel mask request",

	/* 55 IMEBADCHANNELS */	"Inconsistent channel mapping requests",

	/* 56 IMEVALOUTOFRANGE*/ "Value out of range",

	/* 56 IMEUNKNOWNFORMAT*/ "Could not discern format for file",

};





int ImNErr = 58;		/* Number of error messages		*/











