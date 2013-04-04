/**

 **	$Header: /sdsc/dev/vis/misc/libsdsc/v3.0/libsdsc/src/include/RCS/sdsccopyright.h,v 1.3 1995/06/29 00:11:50 bduggan Exp $

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

 **	sdsccopyright.h	-  SDSC copyright notice

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

 **	__SDSCCOPYRIGHTH__	d  File inclusion flag

 **

 **	copyright	v  Copyright message

 **

 **  PRIVATE CONTENTS

 **	none

 **

 **  HISTORY

 **	$Log: sdsccopyright.h,v $

 **	Revision 1.3  1995/06/29  00:11:50  bduggan

 **	changed 94 to 95

 **	q

 **

 **	Revision 1.2  1995/06/01  17:46:10  bduggan

 **	changed 94 to 95

 **

 **	Revision 1.1  94/10/03  16:35:50  nadeau

 **	Initial revision

 **	

 **	

 **/



#ifndef __SDSCCOPYRIGHTH__

#define __SDSCCOPYRIGHTH__





/*

 *  Standard Copyright Notice:

 *	This copyright notice appears as clear text in all SDSC binaries.

 *

 *	HEADER is #defined by the standard C file comment header to be the RCS

 *	header string for the file.  Placement of this string in the copyright

 *	message is sufficient to associate the copyright with the file.

 *

 *	Because lint doesn't like variables that are declared but not

 *	used in the file, we surround the lot with #ifndef lint.

 */



#ifndef lint

static char *copyright[ ] =

{

"------------------------------------------------------------------------",

#ifdef HEADER

HEADER,

#endif /* HEADER */

"    Copyright (c) 1989-1995  San Diego Supercomputer Center (SDSC),",

"    a division of General Atomics, San Diego, CA, USA",

"========================================================================"

};

#endif /* lint */





#endif

