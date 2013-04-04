/**
 **	$Header: /sdsc/dev/vis/misc/libsdsc/v3.0/libsdsc/src/include/RCS/sdsc.h,v 1.5 1995/06/29 00:12:47 bduggan Exp $
 **	Copyright (c) 1989-1995  San Diego Supercomputer Center (SDSC),
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
 **	sdsc.h	-  SDSC projects standard include file
 **
 **  PROJECT
 **	All SDSC software
 **
 **  PUBLIC CONTENTS
 **			d =defined constant
 **			f =function
 **			m =defined macro
 **			t =typedef/struct/union
 **			v =variable
 **			? =other
 **
 **	__SDSCH__	d  File inclusion flag
 **
 **	sdsc_int8	t  8-bit integer
 **	sdsc_uint8	t  8-bit unsigned integer
 **	sdsc_int16	t  16-bit integer
 **	sdsc_uint16	t  16-bit unsigned integer
 **	sdsc_int32	t  32-bit integer
 **	sdsc_uint32	t  32-bit unsigned integer
 **	sdsc_int64	t  64-bit integer
 **	sdsc_uint64	t  64-bit unsigned integer
 **	sdsc_boolean	t  boolean
 **
 **  PRIVATE CONTENTS
 **	none
 **
 **  HISTORY
 **	$Log: sdsc.h,v $
 **	Revision 1.5  1995/06/29  00:12:47  bduggan
 **	removed snet.h
 **
 **	Revision 1.4  94/10/03  16:40:38  nadeau
 **	Changed all SDSC internal typedefs to start with 'sdsc_'.
 **	Moved the copyright notice to 'sdsccopyright.h'.
 **	Dropped 'private' and 'public' fake storage classes.
 **	Dropped 'TRUE' and 'FALSE', 'YES' and 'NO' defines.
 **	Updated to ANSI C and C++ compatibility.
 **	Updated comments.
 **	Updated copyright message.
 **	
 **	Revision 1.3  92/09/02  15:15:03  vle
 **	Updated copyright notice.
 **	
 **	Revision 1.2  91/01/09  16:54:21  nadeau
 **	Updated copyright.
 **	
 **	Revision 1.1  90/06/22  12:16:21  nadeau
 **	Initial revision
 **	
 */

#ifndef __SDSCH__
#define __SDSCH__

#include "sdscconfig.h"
#include "bin.h"
#include "arg.h"
#include "tag.h"





/*
 *  Standard types:
 */

#if CHAR_SIZE >= 8
#define __8	char
#else
#if SHORT_SIZE >= 8
#define __8	short
#else
#if INT_SIZE >= 8
#define __8	int
#else
#if LONG_SIZE >= 8
#define __8	long
#endif
#endif
#endif
#endif

#if CHAR_SIZE >= 16
#define __16	char
#else
#if SHORT_SIZE >= 16
#define __16	short
#else
#if INT_SIZE >= 16
#define __16	int
#else
#if LONG_SIZE >= 16
#define __16	long
#endif
#endif
#endif
#endif

#if CHAR_SIZE >= 32
#define __32	char
#else
#if SHORT_SIZE >= 32
#define __32	short
#else
#if INT_SIZE >= 32
#define __32	int
#else
#if LONG_SIZE >= 32
#define __32	long
#endif
#endif
#endif
#endif

#if CHAR_SIZE >= 64
#define __64	char
#else
#if SHORT_SIZE >= 64
#define __64	short
#else
#if INT_SIZE >= 64
#define __64	int
#else
#if LONG_SIZE >= 64
#define __64	long
#endif
#endif
#endif
#endif

#ifdef __8
typedef __8		sdsc_int8;
typedef unsigned __8	sdsc_uint8;
#undef __8
#endif /* __8 */
#ifdef __16
typedef __16		sdsc_int16;
typedef unsigned __16	sdsc_uint16;
#undef __16
#endif /* __16 */
#ifdef __32
typedef __32		sdsc_int32;
typedef unsigned __32	sdsc_uint32;
#undef __32
#endif /* __32 */
#ifdef __64
typedef __64		sdsc_int64;
typedef unsigned __64	sdsc_uint64;
#undef __64
#endif /* __64 */

typedef unsigned int	sdsc_boolean;

#endif /* __SDSCH__ */
