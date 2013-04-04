/**
 **	$Header: /roq/libim/im.c 1     11/02/99 4:38p Zaphod $
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

#define HEADER	"    $Header: /roq/libim/im.c 1     11/02/99 4:38p Zaphod $"

/**
 **  FILE
 **	im.c		-  Image Library Admin Functions
 **
 **  PROJECT
 **	libim	-  SDSC image manipulation library
 **
 **  DESCRIPTION
 **	im.c contains the error handling and other admin functions of
 **	the image library.
 **
 **  PUBLIC CONTENTS
 **			d =defined constant
 **			f =function
 **			m =defined macro
 **			t =typedef/struct/union
 **			v =variable
 **			? =other
 **
 **	ImPError	f  Print error message
 **	ImQError	f  Query error message
 **
 **  PRIVATE CONTENTS
 **	none
 **
 **  HISTORY
 **	$Log: /roq/libim/im.c $
* 
* 1     11/02/99 4:38p Zaphod
 **	Revision 1.22  1995/06/29  00:28:04  bduggan
 **	updated copyright year
 **
 **	Revision 1.21  1995/06/16  08:32:41  bduggan
 **	Removed global variables.  They are now in imerrno.c
 **
 **	Revision 1.20  1995/05/19  13:02:26  bduggan
 **	Added IMEFORMATUNKNOWN
 **
 **	Revision 1.19  1995/04/13  00:02:23  bduggan
 **	Added errors for channel maps
 **
 **	Revision 1.18  94/10/03  11:29:22  nadeau
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
 **	Revision 1.17  92/10/19  14:16:10  groening
 **	added more error statements
 **	
 **	Revision 1.16  92/09/01  12:27:32  nadeau
 **	Added IMEKEYFIELD and IMEADJUSTFIELD error codes and updated
 **	error code names and messages for IMEOPERATION, IMEGRADUATION,
 **	IMEKEY, IMEADJUST, and IMEBADRANGE.
 **	
 **	Revision 1.15  92/08/28  15:33:47  groening
 **	Added more error codes.
 **	
 **	Revision 1.14  91/10/03  08:47:52  nadeau
 **	Fixed #includes.
 **	
 **	Revision 1.13  91/09/07  16:21:45  todd
 **	Added IMEDECODING and IMEENCODING error codes.
 **	
 **	Revision 1.12  91/03/08  14:25:46  nadeau
 **	More error codes and messages.
 **	
 **	Revision 1.11  90/07/25  13:32:51  todd
 **	added several new IME* errors
 **	
 **	Revision 1.10  90/07/23  13:48:01  nadeau
 **	Added IMEDIFFSIZE error.
 **	
 **	Revision 1.9  90/07/09  07:12:23  mjb
 **	replaced #include <malloc.h> with declarations for malloc() and
 **	realloc() because of Alliant
 **	
 **	Revision 1.8  90/07/02  15:47:54  mercurio
 **	Error messages added
 **	
 **	Revision 1.7  90/06/28  20:15:33  mercurio
 **	Added IMENOIMAGE and IMEUNSUPPORTED error codes
 **	
 **	Revision 1.6  90/06/28  15:14:57  nadeau
 **	Added error handler globals, ImQError() and IMESYNTAX.
 **	
 **	Revision 1.5  90/06/25  14:34:51  nadeau
 **	Added IMENULLTAGTABLE error code.
 **	
 **	Revision 1.4  90/06/25  14:15:07  nadeau
 **	Removed error codes associated with tag table routines (they've been
 **	moved to libsdsc.a).
 **	
 **	Revision 1.3  90/05/11  14:27:01  nadeau
 **	Added IMENOTGRAY error code.
 **	
 **	Revision 1.2  90/05/11  09:53:03  nadeau
 **	Added error messages and rearranged them.
 **	
 **	Revision 1.1  90/03/06  17:31:16  nadeau
 **	Initial revision
 **	
 **/

#include "iminternal.h"


/**
 **  CODE CREDITS
 **	Custom development, Dave Nadeau, San Diego Supercomputer Center, 1990.
 **/






/*
 *  FUNCTION
 *	ImPError	-  Print error message
 *
 *  DESCRIPTION
 *	The error text associated with the current ImErrNo is printed
 *	to stderr, preceded by the given leader string.
 */

void				/* Returns nothing			*/
#ifdef __STDC__
ImPError( char *s )
#else
ImPError( s )
	char *s;		/* Leader string			*/
#endif
{
	if ( ImErrNo == IMESYS )
		perror( s );
	else if ( ImErrNo < 0 || ImErrNo >= ImNErr )
		fprintf( stderr, "Unknown error\n" );
	else if ( s && *s )
		fprintf( stderr, "%s: %s\n", s, ImErrList[ImErrNo] );
	else
		fprintf( stderr, "%s\n", ImErrList[ImErrNo] );
}





/*
 *  FUNCTION
 *	ImQError	-  Query error message
 *
 *  DESCRIPTION
 *	The error text associated with the current ImErrNo is returned.
 */

char *				/* Returns error text			*/
#ifdef __STDC__
ImQError( void )
#else
ImQError( )
#endif
{
	if ( ImErrNo == IMESYS )
	{
		if ( errno < 0 || errno >= sys_nerr )
			return ( "Unknown error" );
		return ( sys_errlist[errno] );
	}
	if ( ImErrNo < 0 || ImErrNo >= ImNErr )
		return ( "Unknown error" );
	return ( ImErrList[ImErrNo] );
}
