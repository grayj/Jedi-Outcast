/**

 **	$Header: /roq/libim/imschemes.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"	$Header: /roq/libim/imschemes.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imschemes.c	- compression scheme structure declaration

 **

 **  PROJECT

 **	libim		-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imschemes.c contains the structure that holds routines/ info for

 **	reading/ writing encoded files.

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **	ImGetCompressSchemes f	get the list of compression/encoding schemes

 **

 **  PRIVATE CONTENTS

 ** 	

 **	imCompressSchemes v	list of compression/encoding schemes

 **

 **

 **  HISTORY

 **	$Log: /roq/libim/imschemes.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 * Revision 1.3  1995/06/29  00:28:04  bduggan

 * updated copyright year

 *

 * Revision 1.2  1995/06/16  08:45:42  bduggan

 * Took out declarations of schemes.  Moved 'em into

 * immiscschemes.c

 *

 * Revision 1.1  1995/05/24  17:05:59  bduggan

 * Initial revision

 *

 **/



//#include <unistd.h>

//#include <sys/wait.h>

#include "im.h"

#include "iminternal.h"





/**

 **  CODE CREDITS

 **	Custom development, Brian Duggan, San Diego Supercomputer Center, 1995.

 **/



/*

 * VARIABLE

 *	imCompressSchemes

 *

 * DESCRIPTION

 *	A list of compression schemes that are supported.

 *

 * ADDING A NEW SCHEME

 *

 * 	Note that here and throughout the library, we try to refer to a file

 *	compression 'scheme' and an image 'format', and not a compression

 *	'format', in order to keep things clear.

 *

 *	Schemes may be added to this list quite easily.

 * Here's how to do it:

 *

 * 	1. Add the declaration 'extern ImCompressScheme myscheme'

 *	2. Add &myscheme to the list 'imCompressSchemes'.

 *	3. Fill up the structure 'myscheme' with the following

 *	   elements:

 *		- Suffixes of the compressed files

 *		- The name of the scheme

 *		- Magic number(s) for the scheme

 *		- a subroutine that will encode a file

 *		- a subroutine that will decode a file

 *	

 *	Here's an explanation of these structure elements for

 *	Lempel-Ziv Encoding (.Z files).

 *

 *	1. Suffix list.

 *	   First we declare imCompressZNames as...

 *		extern char* imCompressZNames[] = { "Z", NULL };

 *	   Then imCompressZNames is the first element of the ImCompressScheme 

 *	   structure.

 *

 *	2. Scheme name.

 *	   "Lempel-Ziv Encoding" is the second element of the ImCompressScheme

 *	   structure.

 *

 *	3. Magic numbers

 *	   We make the following declarations:	

 *		 static unsigned char imZMagic1[] = { 0x1f, 0x9d };

 *		 static unsigned char imZMagic2[] = { 0x9d, 0x1f };

 *		 static ImFileMagic imZMagic[] =

 *		 {

 *			{ 0, 2, imZMagic1},

 *			{ 0, 2, imZMagic2},

 *			{ 0, 0, NULL}

 *		 };

 *	   This signifies that out files may have one of the following two

 *	   magic numbers:  1) a magic number of length 2 at location 0 (1f9d)

 *			or 2) a magic number of length 2 at location 0 (9d1f)

 *

 *	4. Decoding subroutine

 *	   The subroutine 'imZDecode' has the responsibility of decoding the

 *	   the file passed to it, and destroying that file.  It must also name

 *	   the new file that it creates, and return that name.

 *

 *	5. Encoding subroutine.

 *	   The subroutine 'imZEncode' has the responsibility of encoding the

 *	   file passed to it, and destroying that file.  It must also name the 

 *	   new file that it creates, and return that name.

 *

 * 	IMPORTANT:

 *		The decoding and encoding subroutines will receive a file 

 *	such as /usr/tmp/tmpfile.  When creating a new file, the new file name 

 *	should be as close to the old file name as possible.  More 

 *	specifically, the file should be in the temp directory.  Thus, creating

 *	a file such as /usr/tmp/tmpfile.z is fine, but creating a file like 

 *	'tmpfile.z' (in the working directory) is not.  

 *

 *		The reason we don't want to create a file in the working 

 *	directory is that the file that is created by these routines will later

 *	be destroyed.

 *

 *		Many of these routines assume that it's okay to destroy a file 

 *	that has a name similar to the incoming file.  e.g. if the incoming 

 *	file is '/usr/tmp/im.zzyxxy', then it's okay to destroy anything named 

 *	'/usr/tmp/im.zzyxxy.Z'.

 *	

 *

 *	The routines in this file are all simple calls to external programs.  

 *	If you wish to do something more elaborate, then you may want to make a

 *	seperate file for your structure, and function declarations, and simply

 *	declare the structure as an 'extern' in this file.

 *	

 */



#ifndef WIN32

extern ImCompressScheme imCompressuu;

extern ImCompressScheme imCompressGZ;

extern ImCompressScheme imCompressz;

extern ImCompressScheme imCompressZ;



static ImCompressScheme *imCompressSchemes[] =

{

	&imCompressuu,

	&imCompressGZ,

	&imCompressz,

	&imCompressZ,

	NULL

};



#else

static ImCompressScheme *imCompressSchemes[] =

{

	NULL

};



#endif

/*

 *  FUNCTION

 *	ImGetCompressSchemes

 *

 *  DESCRIPTION

 *	Get the list of schemes.

 */

ImCompressScheme**

#ifdef __STDC__

ImGetCompressSchemes( void )

#else

ImGetCompressSchemes( )

#endif

{

	return imCompressSchemes;

}





