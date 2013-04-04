/**

 **	$Header: /sdsc/dev/vis/image/imtools/v3.0/libim/src/include/RCS/impref.h,v 1.4 1995/06/29 00:32:03 bduggan Exp $

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

 **	impref.h	-  Automatic format conversion preferences

 **

 **  PROJECT

 **	libim		-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	impref.h contains initializations for several tables that indicate

 **	what format conversions are prefered over what other conversions.

 **	These tables are used by ImFileWrite() to decide which of several

 **	possible file format variants to choose given a VFB.

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **	__IMPREFH__	d  file inclusion flag

 **

 **	imFilePref	t  format preference table entry

 **	imFilePrefTable	t  format preference table

 **

 **	imPrefMono	v  monochrome preference table

 **	imPrefIndex8	v  8-bit index preference table

 **	imPrefIndex16	v  16-bit index preference table

 **	imPrefRgb	v  RGB preference table

 **

 **  PRIVATE CONTENTS

 **	none

 **

 **  HISTORY

 **	$Log: impref.h,v $

 **	Revision 1.4  1995/06/29  00:32:03  bduggan

 **	updated copyright

 **

 **	Revision 1.3  1994/10/03  16:04:05  nadeau

 **	Updated to ANSI C and C++ compatibility.

 **	Updated copyright message.

 **

 **	Revision 1.2  92/09/02  13:19:39  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.1  91/10/03  12:57:51  nadeau

 **	Initial revision

 **	

 **/





#ifndef __IMPREFH__

#define __IMPREFH__











/*

 *  TYPEDEF & STRUCT

 *	imFilePref	-  format preference table entry

 *	imFilePrefTable	-  format preference table

 *

 *  GLOBALS

 *	imPrefMono	-  monochrome preference table

 *	imPrefIndex8	-  8-bit index preference table

 *	imPrefIndex16	-  16-bit index preference table

 *	imPrefRgb	-  RGB preference table

 *

 *  DESCRIPTION

 *	These tables are used by ImFileWrite to decide which of several

 *	output variants supported by a file format is the best one to

 *	choose given the attributes of an incomming VFB.

 *

 *	Each table contains four sub-tables, one for each combination of

 *	having a CLT and having alpha planes.  Within each sub-table,

 *	output format attributes (depth, CLT status, and alpha status) are

 *	ordered from most preferable to least preferable.

 *

 *	Preferences have determined by a human (ie., me) based upon the

 *	following general guidelines:

 *

 *	1.  Would rather promote to a "higher" image type than lose CLT.

 *	2.  Would rather promote to a "higher" image type than lose Alpha.

 *	3.  Would rather generate a bogus CLT than promote.

 *	4.  If original is index8, promoting to index16 is better than RGB.

 *	5.  All things being equal, promotion is in the path:

 *		mono -> index8 -> rgb -> index16

 *	    index16 is generally worse than rgb because of increased storage.

 *	6.  Only demote to "lower" image type when all else fails.

 *	7.  All things being equal, demotion is in the path:

 *		rgb -> index16 -> index8 -> mono

 *	8.  In a choice between loosing CLT or alpha, will lose alpha.

 *	9.  Would rather lose CLT than demote.

 *	10. Would rather lose alpha than demote.

 *

 *	When looking at a file format's supported depth variations (1-bit,

 *	8-bit, 16-bit, RGB, whatever), depths that fall between the cracks

 *	of those represented in VFB's are treated as the next higher VFB

 *	type in the order:

 *		mono -> index8 -> index16 -> rgb

 *

 *  EXAMPLE

 *	A given format "Xyz" supports the following:

 *		1-bit mono,  no CLT, no alpha

 *		8-bit index, no CLT, no alpha

 *		8-bit index, CLT,    no alpha

 *		24-bit RGB,  no CLT, no alpha

 *		24-bit RGB,  no CLT, alpha

 *

 *	We are given an 8-bit VFB, with CLT and no alpha planes.  To a

 *	human, a glance is enough to say "Yup, use the 3rd variant for Xyz".

 *	To ImFileWrite the following is done:

 *

 *		1.  Determine incomming VFB type.

 *		2.  Get 'imIndex8' preferences table.

 *		3.  Find 'Xyz' in format table.

 *		4.  Get 'Xyz's write map.

 *		5.  For each variant in the write map, look it up in the

 *		    'imIndex8' preference table.  Record the index of the

 *		    table entry that matches.

 *		6.  Use the entry with the lowest index (ie, most prefered).

 */



typedef struct imFilePref

{

	int	pref_field;		/* VFB field mask		*/

	int	pref_attributes;	/* Attribute mask		*/

} imFilePref;



typedef struct imFilePrefTable

{

	imFilePref pref_noCltNoAlpha[16];/* No CLT, no Alpha plane	*/

	imFilePref pref_cltNoAlpha[16];	/* Has CLT, no Alpha plane	*/

	imFilePref pref_noCltAlpha[16];	/* No CLT, has Alpha plane	*/

	imFilePref pref_cltAlpha[16];	/* Has CLT, has Alpha plane	*/

} imFilePrefTable;





static imFilePrefTable imPrefMono =

{

	/* Mono VFB, no CLT, no Alpha planes				*/

	{

		{ IMVFBMONO,	IMCLTNO		| IMALPHANO, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHANO, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHANO, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHANO, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHAYES, },

	},



	/* Mono VFB, CLT, no Alpha planes				*/

	{

		{ IMVFBMONO,	IMCLTYES	| IMALPHANO, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHANO, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHANO, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHANO, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHAYES, },

	},



	/* Mono VFB, no CLT, Alpha planes				*/

	{

		{ IMVFBMONO,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHANO, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHANO, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHANO, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHANO, },

	},



	/* Mono VFB, CLT, Alpha planes					*/

	{

		{ IMVFBMONO,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHANO, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHANO, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHANO, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHANO, },

	},

};





static imFilePrefTable imPrefIndex8 =

{

	/* Index8 VFB, no CLT, no Alpha planes				*/

	{

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHANO, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHANO, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHANO, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHANO, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHAYES, },

	},



	/* Index8 VFB, CLT, no Alpha planes				*/

	{

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHANO, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHANO, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHANO, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHANO, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHAYES, },

	},



	/* Index8 VFB, no CLT, Alpha planes				*/

	{

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHANO, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHANO, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHANO, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHANO, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHANO, },

	},



	/* Index8 VFB, CLT, Alpha planes				*/

	{

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHANO, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHANO, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHANO, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHANO, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHANO, },

	},

};





static imFilePrefTable imPrefIndex16 =

{

	/* Index16 VFB, no CLT, no Alpha planes				*/

	{

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHANO, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHANO, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHANO, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHANO, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHAYES, },

	},



	/* Index16 VFB, CLT, no Alpha planes				*/

	{

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHANO, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHANO, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHANO, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHANO, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHAYES, },

	},



	/* Index16 VFB, no CLT, Alpha planes				*/

	{

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHANO, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHANO, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHANO, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHANO, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHANO, },

	},



	/* Index16 VFB, CLT, Alpha planes				*/

	{

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHANO, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHANO, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHANO, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHANO, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHANO, },

	},

};





static imFilePrefTable imPrefRgb =

{

	/* Rgb VFB, no CLT, no Alpha planes				*/

	{

		{ IMVFBRGB,	IMCLTNO		| IMALPHANO, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHANO, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHANO, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHANO, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHAYES, },

	},



	/* Rgb VFB, CLT, no Alpha planes				*/

	{

		{ IMVFBRGB,	IMCLTYES	| IMALPHANO, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHANO, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHANO, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHANO, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHAYES, },

	},



	/* Rgb VFB, no CLT, Alpha planes				*/

	{

		{ IMVFBRGB,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHANO, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHANO, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHANO, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHANO, },

	},



	/* Rgb VFB, CLT, Alpha planes					*/

	{

		{ IMVFBRGB,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTYES	| IMALPHANO, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBRGB,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX16,	IMCLTNO		| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTYES	| IMALPHANO, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBINDEX8,	IMCLTNO		| IMALPHANO, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTYES	| IMALPHANO, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHAYES, },

		{ IMVFBMONO,	IMCLTNO		| IMALPHANO, },

	},

};





#endif /* __IMVFBREFH__ */

