/**

 **

 **	$Header: /roq/libim/impict.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/impict.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	impict.c	-  Apple Macintosh PICT file I/O

 **

 **  PROJECT

 **	libimage	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	impict.c contains routines to read/write Apple Macintosh PICT files for

 **	the image manipulation library.  Raster data read in is stored

 **	in a VFB.  Raster data written out is taken from a tag table.

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **	none

 **

 **  PRIVATE CONTENTS

 **	imPictRead		f  read a Macintosh PICT file

 **	imPictWrite		f  write a Macintosh PICT file

 **	NULL			d  an empty pointer

 **

 **	imPictHeaderInfo	t  PICT file header information

 **	imPictHeaderFields	v  imPictHeaderInfo description for Bin pkg

 **

 **	imReturnBinError	m  return right error based on Bin call

 **	imMalloc		m  malloc memory & return on error

 **	imBinRead		m  binary read from fd or fp

 **

 **  HISTORY

 **

 **	$Log: /roq/libim/impict.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.28  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.27  1995/06/15  20:51:58  bduggan

 **	Added #ifdef DEBUG's around messages going to stderr

 **

 **	Revision 1.26  1995/06/14  19:17:38  bduggan

 **	Added some casts, turned bzero into memset, took out some useless var's

 **

 **	Revision 1.25  1995/04/03  21:33:42  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.24  1995/02/16  05:32:17  moreland

 **	Fixed a pesky bug that turned out to be major - it has to do with

 **	the cmpCount field of a direct pixel map (32 bit).  First, the code

 **	did not calculate a channel offset for each bitplane.  Second, the

 **	programmer did not know that Apple chose to store 32-bit pixels

 **	with the alpha plane first (ie: ARGB, NOT RGBA).  Oh well, what can

 **	you do with an undocumented proprietary file format...  :^|

 **

 **	Revision 1.23  1995/01/10  23:39:19  bduggan

 **	put in IMMULTI, IMPIPE instead of TRUE/FALSE

 **	uncapitlized i's in local functions

 **	made read/write routines static

 **

 **	Revision 1.22  94/10/03  11:30:28  nadeau

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

 **	Revision 1.21  93/10/21  15:20:20  moreland

 **	Added support for 16 and 32 bit direct pixel image opcodes.

 **	Rearanged some code to support multiple ImFileWrite entry points.

 **	Fixed some gross mistakes in code that was "re-aranged" by the

 **	last person to diddle with the PICT module.

 **	PICT now supports 1, 2, 4, 8, 16, and 32 bit image reads and writes !

 **	

 **	Revision 1.20  93/03/09  10:49:36  nadeau

 **	Corrected non-ANSI-ism on an #endif comment.

 **	

 **	Revision 1.19  92/12/03  01:50:55  nadeau

 **	Corrected info messages.

 **	

 **	Revision 1.18  92/11/24  17:03:54  groening

 **	changed a SHORT to USHORT to maintian type compatibility.

 **	Made the variable clt global since it was used by different

 **	routines but it was not getting passed but declared in each routine.

 **	this fixed problems on crayy with losing the clt and problems

 **	on crayy when writing, which produced a truncation error.

 **	Happy, Happy.

 **	

 **	Revision 1.17  92/11/24  16:34:51  nadeau

 **	Corrected format information string.

 **	

 **	Revision 1.16  92/11/23  18:42:51  nadeau

 **	Removed use of IMINFOMSG.

 **	

 **	Revision 1.15  92/11/04  12:05:00  groening

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.14  92/10/12  15:59:38  vle

 **	Added ImInfo messages.

 **	

 **	Revision 1.13  92/08/31  17:31:43  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.12  92/07/01  13:34:56  groening

 **	added name of struct after terminating braces

 **	in struct to make the next compiler happy.

 **	

 **	Revision 1.11  91/09/26  14:11:35  moreland

 **	Added support for pen-mode (transfer modes)

 **	Fixed core-dumping bug on the Iris (overflowing buffer)

 **	

 **

 **	Revision 1.10  91/06/25  08:09:33  moreland

 **	Cleaned up some of the small inconsistancies reported as warnings

 **	by the "lint" utility.  Added some explicit casts, removed unused

 **	variables, and caught some typos in several 'unused' opcode handlers.

 **

 **	Revision 1.9  91/06/20  09:40:35  moreland

 **	Rewrote imPictRead to use table-driven parser and to

 **	eliminate the difficult-to-compile/giant "switch" statement.

 **	Added better error/status messages (including "English" descriptions

 **	of opcodes being skipped).

 **	

 **	Revision 1.8  91/04/09  11:33:41  nadeau

 **	Removed inclusion of sys/fcntl.h.

 **	

 **	Revision 1.7  91/02/19  07:08:47  moreland

 **	Added code for FrameSameRect opcode

 **	

 **	Revision 1.6  91/02/18  16:24:09  moreland

 **	fixed !

 **	

 **	Revision 1.5  91/02/13  13:38:37  nadeau

 **	Removed tag table checking now handled by ImFileRead and

 **	ImFileWrite.  Corrected a machine byte-order dependency

 **	in CLT writing.

 **	

 **	Revision 1.4  90/12/20  07:27:26  moreland

 **	*** empty log message ***

 **	

 **	Revision 1.3  90/12/13  13:14:41  rama

 **	Pictsize changed to unsigned int

 **

 **	Revision 1.2  90/12/13  07:08:11  moreland

 **	*** empty log message ***

 **

 **	Revision 1.1  90/11/09  09:06:40  moreland

 **	Initial revision

 **

 **	Revision 1.15  90/08/15  07:30:59  moreland

 **	Removed the outer-most for loop - disabled multiple bands per VFB

 **	This fixed "the problem" and imPictWrite now works !!!!  :^)

 **	Turns out rowBytes DOES have to be even.

 **

 **	Revision 1.14  90/08/02  08:55:56  moreland

 **	imPictWrite works !  (with my color smiley face test)

 **	Turns out rowBytes does NOT have to be even after all.

 **

 **	Revision 1.13  90/07/30  15:55:54  moreland

 **	Filled in Apple's reserved HeaderOp fields in imPictWrite.

 **	Color Lookup Table "val" field set to offset/index value.

 **

 **	Revision 1.12  90/06/21  15:42:12  moreland

 **	PackBits routine has been written and tested (a little),

 **	It is now time to start writing imPictWrite.

 **

 **	Revision 1.11  90/06/14  14:49:08  moreland

 **	Cleaned up "holes" for non-handled/skipped opcodes and put fprintf

 **	statements in to let a user know which opcodes are skipped.

 **

 **	Revision 1.10b 90/06/11  15:17:24  moreland

 **	Got "Exposure INIT" PICT ScreenDumps to parse.

 **

 **	Revision 1.10  90/06/11  15:17:24  moreland

 **	Got first PixMaps (pict1 & pict2) to display !

 **

 **	Revision 1.9  90/06/07  12:23:58  moreland

 **	Finished Unpackbits.

 **	Got PackBitsRect opcode working for version 1 PICT files.

 **

 **	Revision 1.8  90/06/05  15:32:32  moreland

 **	The UnpackBits routine MAY be working, but something is causing

 **	strange munging of bitmaps.  (maybe tstBit is broken ?).

 **

 **	Revision 1.7  90/06/04  14:49:28  moreland

 **	First attempt at PO_PackBitsRect opcode - spews raw bits into vfb

 **	At LEAST it skips over all of the PackBitsRect data correctly.

 **

 **	Revision 1.6  90/05/23  16:12:44  moreland

 **	gave up on xpict (no 24bit colors) and wrote pict2pix

 **	todd provided "pixtorle" and "getsun/getx" (rle to sunview/X)

 **	DISPLAYED FIRST PICT (pict8) IMAGE !!!  :^)

 **

 **	Revision 1.5  90/05/22  16:00:51  moreland

 **	finished opcodes to support test-file "pict8"

 **	added DEBUG fprintf statements and cpp DEBUG flag

 **	began constructing "xpict" tool to display PICT files under X

 **

 **	Revision 1.4  90/05/21  16:21:29  moreland

 **	wrote "draft" code for opcodes 0x0013 to 0x0030

 **	fixed stupid syntax errors so that the module compiles

 **

 **	Revision 1.3  90/05/18  16:56:33  moreland

 **	constructed main parser SWITCH block for all opcodes

 **	wrote "draft" code for first 19 or so opcodes

 **

 **	Revision 1.2  90/05/16  16:57:05  moreland

 **	typed in OPCODE define statements

 **

 **	Revision 1.1  90/05/16  13:10:33  moreland

 **	Initial revision

 **

 **	Revision 1.0  90/05/15  09:55:19  moreland

 **	First version creation

 **

 **/



/**

 **  FORMAT

 **	PICT - Apple Macintosh pictures

 **

 **  AKA

 **	pict2

 **

 **  FORMAT REFERENCES

 **	Inside Macintosh, Volumes I-V, Apple Computer, Inc.

 **

 **  CODE CREDITS

 **     Custom development, John Moreland, San Diego Supercomputer Center, 1990.

 **

 **  DESCRIPTION

 **

 **/





#include <stdio.h>

#include <errno.h>

#include <string.h>

#include <sdsc.h>

#include "iminternal.h"

#include "im.h"



/*

 *  PICT - Apple Macintosh Picture

 *      For information on these structures, how to use them, etc. please

 *      see imfmt.c.

 */



#ifdef __STDC__

static int imPictRead( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable );

static int imPictWrite( ImFileFormatWriteMap *ppMap, int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable);



static int imPictWritePB1(ImFileFormatWriteMap *ppMap, int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable );

static int imPictWritePB8( ImFileFormatWriteMap *ppMap, int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable);

static int imPictWritePB16( ImFileFormatWriteMap *ppMap, int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable);

static int imPictWritePBRGB( ImFileFormatWriteMap *ppMap, int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable);

#else

static int imPictRead( );

static int imPictWrite( );



static int imPictWritePB1( );

static int imPictWritePB8( );

static int imPictWritePB16( );

static int imPictWritePBRGB( );

#endif



static char *imPictNames[ ]  = { "pict", "pict2", NULL };

static ImFileFormatReadMap imPictReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IN,1,1,       PB,     IMVFBRGB,       0 },

        { IN,1,2,       PB | C, IMVFBRGB,       0 },

        { IN,1,4,       PB | C, IMVFBRGB,       0 },

        { IN,1,8,       PB | C, IMVFBRGB,       0 },

        { RGB,3,5,      PB,     IMVFBRGB,       0 },

	{ RGB,3,8,	PB,	IMVFBRGB,	0 },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imPictWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBMONO,    0,      IN,1,1,         PB,     imPictWritePB1 },

        { IMVFBINDEX8,  C,      IN,1,8,         PB | C, imPictWritePB8 },

        { IMVFBINDEX16, 0,      RGB,3,5,        PB,     imPictWritePB16 },

        { IMVFBRGB,     0,      RGB,3,8,        PB,     imPictWritePBRGB },

        { -1,           0,      -1,             0,      NULL },

};



static ImFileMagic imFilePictMagic []=

{

	{ 0, 0, NULL},

};



ImFileFormat ImFilePictFormat =

{

	imPictNames, "Apple Macintosh QuickDraw/PICT file",

	"Apple Computer, Inc.",

	"1-, 2-, 4-, 8-, 16-, and 32-bit PackBits-compressed color index\n\
and RGB image files in PICT and PICT2 formats.",
	"1-, 8-, 16-, and 32-bit PackBits-compressed color index and RGB\n\
image files in PICT2 format.",
	imFilePictMagic,

	IMNOMULTI, IMPIPE,

	IMNOMULTI, IMPIPE,

	imPictRead, imPictReadMap, imPictWriteMap

};





static ImClt * clt=NULL;       /* VFB ColorTable - made global because of way

					that code has already been written */





#ifndef NULL

#define NULL	(0)

#endif



#define IModd(a)  ( ( ( (a) % 2 ) == 0 )?FALSE:TRUE )

#define IMeven(a)  ( ( ( (a) % 2 ) == 0 )?TRUE:FALSE )



/*

 *

 *	DEFINES

 *

 */



#define IMFIXED LONG



#define IMBLACK 0

#define IMWHITE 255



/* Pen Transfer Modes */

#define IMTM_TRANSPARENT 36



#define IMSKIP_BUF_SIZ 4096	/* Generic "skip" buffer size		*/

#define IMSTR_LEN 256		/* Generic message-line-string size	*/

#define IMPICT_V1 0x1101		/* Version 1 PICT			*/

#define IMPICT_V2 0x0011		/* Version 2 PICT			*/





/* PICT COMMENTS */

/*      NAME              NUMBER   DESCRIPTION                    DATA(bytes) */

#define IMPC_PICLPAREN           0 /* Begin QD grouping             0           */

#define IMPC_PICRPAREN           1 /* End QD grouping               0           */

#define IMPC_PICAPPCOMMENT     100 /* Application-specific comment  var         */

#define IMPC_PICDWGBEG         130 /* Begin MacDraw picture         0           */

#define IMPC_PICDWGEND         131 /* End MacDraw picture           0           */

#define IMPC_PICGRPBEG         140 /* Begin grouped objects         0           */

#define IMPC_PICGRPEND         141 /* End grouped objects           0           */

#define IMPC_PICBITBEG         142 /* Begin series of bitmap bands  0           */

#define IMPC_PICBITEND         143 /* End series of bitmap bands    0           */

/* etc... */





/*

 *

 *	TYPEDEFS AND STRUCTURES

 *

 */





typedef struct Rect

{

	short int top, left, bottom, right;

} Rect;



static BinField RectFields[] =

{

	SHORT,2,1,

	SHORT,2,1,

	SHORT,2,1,

	SHORT,2,1,

	0,0,0

};





typedef struct Pict2Header

{

	unsigned short		headerOp;

	unsigned short		version;

	unsigned short		reserved1;

	unsigned long		hRes;

	unsigned long		vRes;

	short		srcRect_top;

	short		srcRect_left;

	short		srcRect_bottom;

	short		srcRect_right;

	unsigned long		reserved2;

} Pict2Header;



static BinField Pict2HeaderFields[ ] =

{

	USHORT, 2, 1,

	USHORT, 2, 1,

	USHORT, 2, 1,

	ULONG,  4, 1,

	ULONG,  4, 1,

	SHORT,  2, 4,

	ULONG,  4, 1,

	0, 0, 0,

};





typedef long Fixed;



typedef struct Point

{

	short int v, h;

} Point;



static BinField PointFields[] =

{

	SHORT,2,1,

	SHORT,2,1,

	0,0,0

};





typedef struct PMap

{

	struct Rect		bnd;

	short int		version;

	short int		packType;

	long int		packSize;

	Fixed			hRes;

	Fixed			vRes;

	short int		pixelType;

	short int		pixelSize;

	short int		cmpCount;

	short int		cmpSize;

	long int		planeBytes;

	long int		pmTable;

	long int		pmReserved;

} PMap;



static BinField PMapFields[] =

{

	SHORT,2,1, SHORT,2,1, SHORT,2,1, SHORT,2,1,

	SHORT,2,1,

	SHORT,2,1,

	LONG,4,1,

	IMFIXED,4,1,

	IMFIXED,4,1,

	SHORT,2,1,

	SHORT,2,1,

	SHORT,2,1,

	SHORT,2,1,

	LONG,4,1,

	LONG,4,1,

	LONG,4,1,

	0,0,0

};





typedef struct CTable

{

	long int	ctSeed;

	short int	ctFlags;

	short int	ctSize;

} CTable;



static BinField CTableFields[] =

{

	LONG,4,1,

	SHORT,2,1,

	SHORT,2,1,

	0,0,0

};





typedef struct CTEntry

{

	unsigned short val, red, green, blue;

} CTEntry;



static BinField CTEntryFields[] =

{

	SHORT,2,1,

	SHORT,2,1,

	SHORT,2,1,

	SHORT,2,1,

	0,0,0

};





typedef struct RGBColor

{

	short int red, green, blue;

} RGBColor;



static BinField RGBColorFields[] =

{

	SHORT,2,1,

	SHORT,2,1,

	SHORT,2,1,

	0,0,0

};





typedef struct Region

{

	short int		rgnSize;

	struct Rect		rgnBBox;

} Region;



static BinField RegionFields[] =

{

	SHORT,2,1,

	SHORT,2,1, SHORT,2,1, SHORT,2,1, SHORT,2,1,

	0,0,0

};





typedef unsigned char Pattern[8];





typedef struct BMap

{

	struct Rect		bnd;

	struct Rect		srcRect;

	struct Rect		dstRect;

	short int		mode;

} BMap;



static BinField BMapFields[] =

{

	SHORT,2,1, SHORT,2,1, SHORT,2,1, SHORT,2,1,

	SHORT,2,1, SHORT,2,1, SHORT,2,1, SHORT,2,1,

	SHORT,2,1, SHORT,2,1, SHORT,2,1, SHORT,2,1,

	SHORT,2,1,

	0,0,0

};





typedef struct GState

{

	/* EXTRACTED FIELDS FROM MACINTOSH "cGrafPort" STRUCTURE */

	short		chExtra;

	short		pnLocHFrac;

	struct Rect	portRect;

	struct Region	visRgn;

	struct Region	clipRgn;

	struct PMap	bkPixPat;

	struct RGBColor	rgbFgColor;	/* Used to "fill" objects */

	struct RGBColor	rgbBkColor;	/* Used to "paint" objects */

	struct Point	pnLoc;

	struct Point	pnSize;

	short		pnMode;

	struct PMap	pnPixPat;

	struct PMap	fillPixPat;

	short		pnVis;

	short		txFont;

	int		txFace;

	short		txMode;

	short		txSize;

	Fixed		spExtra;

	long		fgColor;

	long		bkColor;

	short		colrBit;

	short		patStretch;

	/* CUSTOM FIELDS */

	short		hiliteMode;	/* 0x001C - hilite mode flag - 0 */

	short		opColor;	/* 0x001F - RGB OpColor : arith. modes - 0 */

	struct RGBColor	hiliteColor;

	struct Rect	lastRect;	/* used for "Same" routines */

	Pattern		fgPat;

	Pattern		bkPat;

	Pattern		pnPat;

	Pattern		fillPat;

	struct Point	ovSize;

	struct Point	txRatNum;

	struct Point	txRatDen;

	char		version;

	struct Point	origin;

} GState;





/* Opcode handling functions */

static int PO_NOP();

static int PO_Clip();

static int PO_BkPat();

static int PO_TxFont();

static int PO_TxFace();

static int PO_TxMode();

static int PO_SpExtra();

static int PO_PnSize();

static int PO_PnMode();

static int PO_PnPat();

static int PO_FillPat();

static int PO_OvSize();

static int PO_Origin();

static int PO_TxSize();

static int PO_FgColor();

static int PO_BkColor();

static int PO_TxRatio();

static int PO_Version();

static int PO_BkPixPat();

static int PO_PnPixPat();

static int PO_FillPixPat();

static int PO_PnLocHFrac();

static int PO_ChExtra();

static int PO_reserved1();

static int PO_reserved2();

static int PO_reserved3();

static int PO_RGBFgCol();

static int PO_RGBBkCol();

static int PO_HiliteMode();

static int PO_HiliteColor();

static int PO_DefHilite();

static int PO_OpColor();

static int PO_Line();

static int PO_LineFrom();

static int PO_ShortLine();

static int PO_ShortLineFrom();

static int PO_reserved4();

static int PO_reserved5();

static int PO_reserved6();

static int PO_reserved7();

static int PO_LongText();

static int PO_DHText();

static int PO_DVText();

static int PO_DHDVText();

static int PO_reserved8();

static int PO_reserved9();

static int PO_reserved10();

static int PO_reserved11();

static int PO_FrameRect();

static int PO_PaintRect();

static int PO_EraseRect();

static int PO_InvertRect();

static int PO_FillRect();

static int PO_reserved12();

static int PO_reserved13();

static int PO_reserved14();

static int PO_FrameSameRect();

static int PO_PaintSameRect();

static int PO_EraseSameRect();

static int PO_InvertSameRect();

static int PO_FillSameRect();

static int PO_reserved15();

static int PO_reserved16();

static int PO_reserved17();

static int PO_FrameRRect();

static int PO_PaintRRect();

static int PO_EraseRRect();

static int PO_InvertRRect();

static int PO_FillRRect();

static int PO_reserved18();

static int PO_reserved19();

static int PO_reserved20();

static int PO_FrameSameRRect();

static int PO_PaintSameRRect();

static int PO_EraseSameRRect();

static int PO_InvertSameRRect();

static int PO_FillSameRRect();

static int PO_reserved21();

static int PO_reserved22();

static int PO_reserved23();

static int PO_FrameOval();

static int PO_PaintOval();

static int PO_EraseOval();

static int PO_InvertOval();

static int PO_FillOval();

static int PO_reserved24();

static int PO_reserved25();

static int PO_reserved26();

static int PO_FrameSameOval();

static int PO_PaintSameOval();

static int PO_EraseSameOval();

static int PO_InvertSameOval();

static int PO_FillSameOval();

static int PO_reserved27();

static int PO_reserved28();

static int PO_reserved29();

static int PO_FrameArc();

static int PO_PaintArc();

static int PO_EraseArc();

static int PO_InvertArc();

static int PO_FillArc();

static int PO_reserved30();

static int PO_reserved31();

static int PO_reserved32();

static int PO_FrameSameArc();

static int PO_PaintSameArc();

static int PO_EraseSameArc();

static int PO_InvertSameArc();

static int PO_FillSameArc();

static int PO_reserved33();

static int PO_reserved34();

static int PO_reserved35();

static int PO_FramePoly();

static int PO_PaintPoly();

static int PO_ErasePoly();

static int PO_InvertPoly();

static int PO_FillPoly();

static int PO_reserved36();

static int PO_reserved37();

static int PO_reserved38();

static int PO_FrameSamePoly();

static int PO_PaintSamePoly();

static int PO_EraseSamePoly();

static int PO_InvertSamePoly();

static int PO_FillSamePoly();

static int PO_reserved39();

static int PO_reserved40();

static int PO_reserved41();

static int PO_FrameRgn();

static int PO_PaintRgn();

static int PO_EraseRgn();

static int PO_InvertRgn();

static int PO_FillRgn();

static int PO_reserved42();

static int PO_reserved43();

static int PO_reserved44();

static int PO_FrameSameRgn();

static int PO_PaintSameRgn();

static int PO_EraseSameRgn();

static int PO_InvertSameRgn();

static int PO_FillSameRgn();

static int PO_reserved45();

static int PO_reserved46();

static int PO_reserved47();

static int PO_BitsRect();

static int PO_BitsRgn();

static int PO_reserved48();

static int PO_reserved49();

static int PO_reserved50();

static int PO_reserved51();

static int PO_reserved52();

static int PO_reserved53();

static int PO_PackBitsRect();

static int PO_PackBitsRgn();

static int PO_DirectBitsRect();

static int PO_DirectBitsRgn();

static int PO_reserved56();

static int PO_reserved57();

static int PO_reserved58();

static int PO_reserved59();

static int PO_ShortComment();

static int PO_LongComment();

static int PO_reserved60();

/* etc... */

static int PO_reserved73();

static int PO_reserved74();

/* etc... */

static int PO_reserved105();

static int PO_reserved106();

/* etc... */

static int PO_reserved152();

static int PO_opEndPic();

static int PO_reserved153();

/* etc... */

static int PO_reserved408();

static int PO_reserved409();

/* etc... */

static int PO_reserved2968();

static int PO_HeaderOp();

static int PO_reserved2969();

/* etc... */

static int PO_reserved9999();





/*

 *

 *

 *	PARSER TYPEDEFS AND STRUCTURES

 * 

 */





struct POCHandler

{

	int opcode;

	char * name;

	int (*handler)();

};





typedef struct POCHandler POCHandler;





/* PICT OPCODE VALUE/FUNCTION/DESCRIPTION LOOKUP TABLE */

POCHandler POCHandlerList[] =

{

    /* VALUE   DESCRIPTION                  HANDLER                DATA(bytes) */

    { 0x0000, "Null Operation",             PO_NOP },              /* 0 */

    { 0x0001, "Clip Region",                PO_Clip },             /* rgn size */

    { 0x0002, "Background Pattern",         PO_BkPat },            /* 8 */

    { 0x0003, "Text Font",                  PO_TxFont },           /* 2 */

    { 0x0004, "Text Face",                  PO_TxFace },           /* 1 */

    { 0x0005, "Text Mode",                  PO_TxMode },           /* 2 */

    { 0x0006, "Space Extra",                PO_SpExtra },          /* 4 */

    { 0x0007, "Pen Size",                   PO_PnSize },           /* 4 */

    { 0x0008, "Pen Mode",                   PO_PnMode },           /* 2 */

    { 0x0009, "Pen Pattern",                PO_PnPat },            /* 8 */

    { 0x000A, "Fill Pattern",               PO_FillPat },          /* 8 */

    { 0x000B, "Oval Size",                  PO_OvSize },           /* 4 */

    { 0x000C, "Set Origin - dh, dv",        PO_Origin },           /* 4 */

    { 0x000D, "Text Size",                  PO_TxSize },           /* 2 */

    { 0x000E, "Foreground Color",           PO_FgColor },          /* 4 */

    { 0x000F, "Background Color",           PO_BkColor },          /* 4 */

    { 0x0010, "Text Ratio",                 PO_TxRatio },          /* 4 */

    { 0x0011, "Version",                    PO_Version },          /* 1 */

    { 0x0012, "Background Pixel Pattern",   PO_BkPixPat },         /* var */

    { 0x0013, "Pen Pixel Pattern",          PO_PnPixPat },         /* var */

    { 0x0014, "Fill Pixel Pattern",         PO_FillPixPat },       /* var */

    { 0x0015, "Fractional Pen Position",    PO_PnLocHFrac },       /* 2 */

    { 0x0016, "Extra for each character",   PO_ChExtra },          /* 2 */

    { 0x0017, "Reserved1 for Apple use",    PO_reserved1 },        /* 0 */

    { 0x0018, "Reserved2 for Apple use",    PO_reserved2 },        /* 0 */

    { 0x0019, "Reserved3 for Apple use",    PO_reserved3 },        /* 0 */

    { 0x001A, "RGB Forground Color",        PO_RGBFgCol },         /* var */

    { 0x001B, "RGB Background Color",       PO_RGBBkCol },         /* var */

    { 0x001C, "Hilite Mode Flag",           PO_HiliteMode },       /* 0 */

    { 0x001D, "RGB Hilite Color",           PO_HiliteColor },      /* var */

    { 0x001E, "Use Default Hilite Color",   PO_DefHilite },        /* 0 */

    { 0x001F, "RGB OpColor / Arith Modes",  PO_OpColor },          /* 0 */

    { 0x0020, "Line (penLoc, newPt)",       PO_Line },             /* 8 */

    { 0x0021, "Line From (newPt)",          PO_LineFrom },         /* 4 */

    { 0x0022, "Short Line (pnLoc)",         PO_ShortLine },        /* 6 */

    { 0x0023, "Short Line From (dh, dv)",   PO_ShortLineFrom },    /* 2 */

    { 0x0024, "Reserved4 for Apple use",    PO_reserved4 },        /* 2+data */

    { 0x0025, "Reserved5 for Apple use",    PO_reserved5 },        /* 2+data */

    { 0x0026, "Reserved6 for Apple use",    PO_reserved6 },        /* 2+data */

    { 0x0027, "Reserved7 for Apple use",    PO_reserved7 },        /* 2+data */

    { 0x0028, "Long Text (loc, cnt, txt)",  PO_LongText },         /* 5+text */

    { 0x0029, "HOffset Text (dh,cnt,txt)",  PO_DHText },           /* 2+text */

    { 0x002A, "VOffset Text (dv,cnt,txt)",  PO_DVText },           /* 2+text */

    { 0x002B, "DHDV Text (dh,dv,cnt,txt)",  PO_DHDVText },         /* 3+text */

    { 0x002C, "Reserved8 for Apple use",    PO_reserved8 },        /* 2+data */

    { 0x002D, "Reserved9 for Apple use",    PO_reserved9 },        /* 2+data */

    { 0x002E, "Reserved10 for Apple use",   PO_reserved10 },       /* 2+data */

    { 0x002F, "Reserved11 for Apple use",   PO_reserved11 },       /* 2+data */

    { 0x0030, "Frame Rect (rect)",          PO_FrameRect },        /* 8 */

    { 0x0031, "Paint Rect (rect)",          PO_PaintRect },        /* 8 */

    { 0x0032, "Erase Rect (rect)",          PO_EraseRect },        /* 8 */

    { 0x0033, "Invert Rect (rect)",         PO_InvertRect },       /* 8 */

    { 0x0034, "Fill Rect (rect)",           PO_FillRect },         /* 8 */

    { 0x0035, "Reserved12 for Apple use",   PO_reserved12 },       /* 8 */

    { 0x0036, "Reserved13 for Apple use",   PO_reserved13 },       /* 8 */

    { 0x0037, "Reserved14 for Apple use",   PO_reserved14 },       /* 8 */

    { 0x0038, "Frame Same Rect",            PO_FrameSameRect },    /* 0 */

    { 0x0039, "Paint Same Rect",            PO_PaintSameRect },    /* 0 */

    { 0x003A, "Erase Same Rect",            PO_EraseSameRect },    /* 0 */

    { 0x003B, "Invert Same Rect",           PO_InvertSameRect },   /* 0 */

    { 0x003C, "Fill Same Rect",             PO_FillSameRect },     /* 0 */

    { 0x003D, "Reserved15 for Apple use",   PO_reserved15 },       /* 0 */

    { 0x003E, "Reserved16 for Apple use",   PO_reserved16 },       /* 0 */

    { 0x003F, "Reserved17 for Apple use",   PO_reserved17 },       /* 0 */

    { 0x0040, "Frame Round Rect (rect)",    PO_FrameRRect },       /* 8 */

    { 0x0041, "Paint Round Rect (rect)",    PO_PaintRRect },       /* 8 */

    { 0x0042, "Erase Round Rect (rect)",    PO_EraseRRect },       /* 8 */

    { 0x0043, "Invert Round Rect (rect)",   PO_InvertRRect },      /* 8 */

    { 0x0044, "Fill Round Rect (rect)",     PO_FillRRect },        /* 8 */

    { 0x0045, "Reserved18 for Apple use",   PO_reserved18 },       /* 8 */

    { 0x0046, "Reserved19 for Apple use",   PO_reserved19 },       /* 8 */

    { 0x0047, "Reserved20 for Apple use",   PO_reserved20 },       /* 8 */

    { 0x0048, "Frame Same Round Rect",      PO_FrameSameRRect },   /* 0 */

    { 0x0049, "Paint Same Round Rect",      PO_PaintSameRRect },   /* 0 */

    { 0x004A, "Erase Same Round Rect",      PO_EraseSameRRect },   /* 0 */

    { 0x004B, "Invert Same Round Rect",     PO_InvertSameRRect },  /* 0 */

    { 0x004C, "Fill Same Round Rect",       PO_FillSameRRect },    /* 0 */

    { 0x004D, "Reserved21 for Apple use",   PO_reserved21 },       /* 0 */

    { 0x004E, "Reserved22 for Apple use",   PO_reserved22 },       /* 0 */

    { 0x004F, "Reserved23 for Apple use",   PO_reserved23 },       /* 0 */

    { 0x0050, "Frame Oval (rect)",          PO_FrameOval },        /* 8 */

    { 0x0051, "Paint Oval (rect)",          PO_PaintOval },        /* 8 */

    { 0x0052, "Erase Oval (rect)",          PO_EraseOval },        /* 8 */

    { 0x0053, "Invert Oval (rect)",         PO_InvertOval },       /* 8 */

    { 0x0054, "Fill Oval (rect)",           PO_FillOval },         /* 8 */

    { 0x0055, "Reserved24 for Apple use",   PO_reserved24 },       /* 8 */

    { 0x0056, "Reserved25 for Apple use",   PO_reserved25 },       /* 8 */

    { 0x0057, "Reserved26 for Apple use",   PO_reserved26 },       /* 8 */

    { 0x0058, "Frame Same Oval (rect)",     PO_FrameSameOval },    /* 0 */

    { 0x0059, "Paint Same Oval (rect)",     PO_PaintSameOval },    /* 0 */

    { 0x005A, "Erase Same Oval (rect)",     PO_EraseSameOval },    /* 0 */

    { 0x005B, "Invert Same Oval (rect)",    PO_InvertSameOval },   /* 0 */

    { 0x005C, "Fill Same Oval (rect)",      PO_FillSameOval },     /* 0 */

    { 0x005D, "Reserved27 for Apple use",   PO_reserved27 },       /* 0 */

    { 0x005E, "Reserved28 for Apple use",   PO_reserved28 },       /* 0 */

    { 0x005F, "Reserved29 for Apple use",   PO_reserved29 },       /* 0 */

    { 0x0060, "Frame Arc (r,beg,angle)",    PO_FrameArc },         /* 12 */

    { 0x0061, "Paint Arc (r,beg,angle)",    PO_PaintArc },         /* 12 */

    { 0x0062, "Erase Arc (r,beg,angle)",    PO_EraseArc },         /* 12 */

    { 0x0063, "Invert Arc (r,beg,angle)",   PO_InvertArc },        /* 12 */

    { 0x0064, "Fill Arc (r,beg,angle)",     PO_FillArc },          /* 12 */

    { 0x0065, "Reserved30 for Apple use",   PO_reserved30 },       /* 12 */

    { 0x0066, "Reserved31 for Apple use",   PO_reserved31 },       /* 12 */

    { 0x0066, "Reserved32 for Apple use",   PO_reserved32 },       /* 12 */

    { 0x0068, "Frame Same Arc",             PO_FrameSameArc },     /* 4 */

    { 0x0069, "Paint Same Arc",             PO_PaintSameArc },     /* 4 */

    { 0x006A, "Erase Same Arc",             PO_EraseSameArc },     /* 4 */

    { 0x006B, "Invert Same Arc",            PO_InvertSameArc },    /* 4 */

    { 0x006C, "Fill Same Arc",              PO_FillSameArc },      /* 4 */

    { 0x006D, "Reserved33 for Apple use",   PO_reserved33 },       /* 4 */

    { 0x006E, "Reserved34 for Apple use",   PO_reserved34 },       /* 4 */

    { 0x006F, "Reserved35 for Apple use",   PO_reserved35 },       /* 4 */

    { 0x0070, "Frame Polygon",              PO_FramePoly },        /* poly size */

    { 0x0071, "Paint Polygon",              PO_PaintPoly },        /* poly size */

    { 0x0072, "Erase Polygon",              PO_ErasePoly },        /* poly size */

    { 0x0073, "Invert Polygon",             PO_InvertPoly },       /* poly size */

    { 0x0074, "Fill Polygon",               PO_FillPoly },         /* poly size */

    { 0x0075, "Reserved36 for Apple use",   PO_reserved36 },       /* poly Size */

    { 0x0076, "Reserved37 for Apple use",   PO_reserved37 },       /* poly Size */

    { 0x0077, "Reserved38 for Apple use",   PO_reserved38 },       /* poly Size */

    { 0x0078, "Frame Same Poly",            PO_FrameSamePoly },    /* ? */

    { 0x0079, "Paint Same Poly",            PO_PaintSamePoly },    /* ? */

    { 0x007A, "Erase Same Poly",            PO_EraseSamePoly },    /* ? */

    { 0x007B, "Invert Same Poly",           PO_InvertSamePoly },   /* ? */

    { 0x007C, "Fill Same Poly",             PO_FillSamePoly },     /* ? */

    { 0x007D, "Reserved39 for Apple use",   PO_reserved39 },       /* 0 */

    { 0x007E, "Reserved40 for Apple use",   PO_reserved40 },       /* 0 */

    { 0x007F, "Reserved41 for Apple use",   PO_reserved41 },       /* 0 */

    { 0x0080, "Frame Region",               PO_FrameRgn },         /* rgn size */

    { 0x0081, "Paint Region",               PO_PaintRgn },         /* rgn size */

    { 0x0082, "Erase Region",               PO_EraseRgn },         /* rgn size */

    { 0x0083, "Invert Region",              PO_InvertRgn },        /* rgn size */

    { 0x0084, "Fill Region",                PO_FillRgn },          /* rgn size */

    { 0x0085, "Reserved for Apple use",     PO_reserved42 },       /* rgn size */

    { 0x0086, "Reserved for Apple use",     PO_reserved43 },       /* rgn size */

    { 0x0087, "Reserved for Apple use",     PO_reserved44 },       /* rgn size */

    { 0x0088, "Frame Same Region",          PO_FrameSameRgn },     /* ? */

    { 0x0089, "Paint Same Region",          PO_PaintSameRgn },     /* ? */

    { 0x008A, "Erase Same Region",          PO_EraseSameRgn },     /* ? */

    { 0x008B, "Invert Same Region",         PO_InvertSameRgn },    /* ? */

    { 0x008C, "Fill Same Region",           PO_FillSameRgn },      /* ? */

    { 0x008D, "Reserved45 for Apple use",   PO_reserved45 },       /* 0 */

    { 0x008E, "Reserved46 for Apple use",   PO_reserved46 },       /* 0 */

    { 0x008F, "Reserved47 for Apple use",   PO_reserved47 },       /* 0 */

    { 0x0090, "Copybits, rect clipped",     PO_BitsRect },         /* var */

    { 0x0091, "Copybits, rgn clipped",      PO_BitsRgn },          /* var */

    { 0x0092, "Reserved48 for Apple use",   PO_reserved48 },       /* 2+data */

    { 0x0093, "Reserved49 for Apple use",   PO_reserved49 },       /* 2+data */

    { 0x0094, "Reserved50 for Apple use",   PO_reserved50 },       /* 2+data */

    { 0x0095, "Reserved51 for Apple use",   PO_reserved51 },       /* 2+data */

    { 0x0096, "Reserved52 for Apple use",   PO_reserved52 },       /* 2+data */

    { 0x0097, "Reserved53 for Apple use",   PO_reserved53 },       /* 2+data */

    { 0x0098, "Packed Bits (rect clipped)", PO_PackBitsRect },     /* var */

    { 0x0099, "Packed Bits (rgn clipped)",  PO_PackBitsRgn },      /* var */

    { 0x009A, "32-bit QuickDraw RGB map",   PO_DirectBitsRect },   /* 2+data */

    { 0x009B, "32-bit QuickDraw RGB map",   PO_DirectBitsRgn },    /* 2+data */

    { 0x009C, "Reserved56 for Apple use",   PO_reserved56 },       /* 2+data */

    { 0x009D, "Reserved57 for Apple use",   PO_reserved57 },       /* 2+data */

    { 0x009E, "Reserved58 for Apple use",   PO_reserved58 },       /* 2+data */

    { 0x009F, "Reserved59 for Apple use",   PO_reserved59 },       /* 2+data */

    { 0x00A0, "Short Comment",              PO_ShortComment },     /* 2 */

    { 0x00A1, "Long Comment",               PO_LongComment }       /* 4+data */



#define IMLAST_INDEXED_OPCODE 0x00A1



#ifdef CANT_BE_INDEXED

    { 0x00A2, "Reserved60 for Apple use",   PO_reserved60 },       /* 2+data */

    /* etc... */

    { 0x00AF, "Reserved for Apple use",     PO_reserved73 },       /* 2+data */

    { 0x00B0, "Reserved for Apple use",     PO_reserved74 },       /* 0 */

    /* etc... */

    { 0x00CF, "Reserved for Apple use",     PO_reserved105 },      /* 0 */

    { 0x00D0, "Reserved for Apple use",     PO_reserved106 },      /* 4+data */

    /* etc... */

    { 0x00FE, "Reserved for Apple use",     PO_reserved152 },      /* 4+data */

    { 0x00FF, "End of picture",             PO_opEndPic },         /* 0 */

    { 0x0100, "Reserved for Apple use",     PO_reserved153 },      /* 2 */

    /* etc... */

    { 0x01FF, "Reserved for Apple use",     PO_reserved408 },      /* 2 */

    { 0x0200, "Reserved for Apple use",     PO_reserved409 },      /* 4 */

    /* etc... */

    { 0x0BFF, "Reserved for Apple use",     PO_reserved2968 },     /* 4 */

    { 0x0C00, "Reserved for Apple use",     PO_HeaderOp },         /* 2 */

    { 0x0C01, "Reserved for Apple use",     PO_reserved2969 },     /* ? */

    /* etc... */

    { 0xFFFF, "Reserved for Apple use",     PO_reserved9999 },     /* ? */

#endif /* CANT_BE_INDEXED */



};





/*

 *

 *

 *	GLOBAL VARIABLES

 *

 *

 */



/*  g_ = global

 *  l_ = local

 *  e_ = external

 */



unsigned short		g_Opcode = 0;	/* Current (Last Read) PICT opcode	*/

struct GState	g_gs;		/* Current Graphics State		*/



int		g_ioType;	/* I/O flags				*/

int		g_fd;		/* Input file descriptor		*/

FILE *		g_fp;		/* Input file pointer			*/



ImVfb *		g_vfb;		/* The current image/vfb		*/

short		g_pictVers = 0;	/* PICT version				*/

struct Rect	g_pictFrame;	/* bounding rectangle of the image	*/



char		g_buf[ IMSKIP_BUF_SIZ ];	/* Used to skip data		*/





/*

 *

 *

 *		UTILILTY FUNCTIONS

 *

 */





/*                                                                          

 *  Returns TRUE if the two RGB colors are equal, or FALSE otherwise.       

 *                                                                          

 */

#ifdef __STDC__

int EqualRGB( struct RGBColor color1, struct RGBColor color2 )

#else

int EqualRGB( color1, color2 )

struct RGBColor	color1;

struct RGBColor	color2;

#endif

{

	if ( color1.red != color2.red )  return( FALSE );

	if ( color1.green != color2.green )  return( FALSE );

	if ( color1.blue != color2.blue )  return( FALSE );



	return( TRUE );

}





/*                                                                  

 *  This is the main routine that is called to SET a VFB pixel at the

 *  specified (x,y) coordinate to the given RGB color.  This routine  

 *  also CLIPS (does not draw) pixels which fall outside the VFB       

 *  drawing area.  This routine DOES NOT USE the pnSize "pen size"      

 *  to color pixels adjacent to the pen location.                        

 *                                                                        

 *  This routine uses the current pnMode (pen transfer mode) to determine  

 *  how to blend the source pixel with the destination pixel.               

 *                                                                          

 */

#ifdef __STDC__

static void SetPixel( short int x, short int y, struct RGBColor color )

#else

static void SetPixel( x, y, color )

	short int x, y;

	struct RGBColor color;

#endif

{

	ImVfbPtr pptr;



	/* Do nothing and return if the pixel is out of bounds */

	if ( (x < 0) || (y < 0) )  return;



	/* If the current pnMode is "transparent", */

	/* and "color" is the same as the current background color, */

	/* then there is no need to do any drawing.  Return */

	/* This has the effect of leaving transparent "holes" in an image. */

	if ( (g_gs.pnMode == IMTM_TRANSPARENT) &&

		EqualRGB( color, g_gs.rgbBkColor ) )

		return;



	pptr = ImVfbQPtr( g_vfb, x, y );

	ImVfbSRed( g_vfb, pptr, color.red );

	ImVfbSGreen( g_vfb, pptr, color.green );

	ImVfbSBlue( g_vfb, pptr, color.blue );

}





/*

 *                                                                     

 *  This is the main routine that is called to PAINT a VFB pixel at the 

 *  specified (x,y) coordinate to the given RGB color.  This routine  

 *  also CLIPS (does not draw) pixels which fall outside the VFB       

 *  drawing area.  Additionally, this routine uses the pnSize "pen size"

 *  to color pixels adjacent to the pen location based on the width and  

 *  height of the current pen size setting.                               

 *                                                                         

 */

#ifdef __STDC__

static void PaintPixel( short int x, short int y, struct RGBColor color )

#else

static void PaintPixel( x, y, color )

	short int x, y;

	struct RGBColor color;

#endif

{

	int i, j;



	if ( (g_gs.pnSize.h < 1) || (g_gs.pnSize.v < 1))  return;



	for ( i=x; i<(x+g_gs.pnSize.h); i++ )  {

		for ( j=y; j<(y+g_gs.pnSize.v); j++ )  {

			SetPixel( i, j, color );

		}

	}

}





/*

 *                                                              

 *  Bresenham's vector -> raster line conversion alogrithm,      

 *  written in special brute-force-and-ignorance style.           

 *                                                                 

 */

#ifdef __STDC__

static void rline( short int x1, short int y1, short int x2, short int y2 )

#else

static void rline( x1, y1, x2, y2 )

	short int x1, y1, x2, y2;

#endif

{

	int dx, dy, step1, step2, d, deltap, deltaq;



#ifdef DEBUG

	fprintf( stderr, "DEBUG: rline: (%d,%d) to (%d,%d)\n", x1, y1, x2, y2 );

#endif



	g_gs.pnLoc.h = x2;

	g_gs.pnLoc.v = y2;



	dx= abs( x2-x1 );

	dy= abs( y2-y1 );

	step1 = ( (x1<x2) ? 1 : (-1) );

	step2 = ( (y1<y2) ? 1 : (-1) );



	PaintPixel( x1, y1, g_gs.rgbFgColor );



	if( dy <= dx )

	{

		deltap = dy+dy;

		     d = deltap-dx;

		deltaq = d-dx;



		while ( x1 != x2 )

		{

			x1 += step1;



			if ( d < 0 )

				d += deltap;

			else

			{

				y1 += step2;

				d  += deltaq;

			}



			PaintPixel( x1, y1, g_gs.rgbFgColor );

		}

	} else {

		deltap = dx+dx;

		     d = deltap - dy;

		deltaq = d - dy;



		while( y1 != y2 )

		{

			y1 +=  step2;



			if ( d < 0 )

				d += deltap;

			else

			{

				x1 += step1;

				d += deltaq;

			}



			PaintPixel( x1, y1, g_gs.rgbFgColor );

		}

	}

}





/*

 *  The correct amount of data has been skipped for the opcode, but       

 *  let the user know that they may lose information and that we are able  

 *  to continue on to the next opcode.    

 *

 */

#ifdef __STDC__

static int OpcodeWarn( unsigned short oc )

#else

static int OpcodeWarn( oc )

	unsigned short oc;

#endif

{

	char msg[ IMSTR_LEN ];



	/* oc is always >= 0x0000 since it is unsigned. */

	if ( oc <= IMLAST_INDEXED_OPCODE )

		sprintf( msg, "Skipped opcode '%s' (0x%0x)", POCHandlerList[oc].name, oc );

	else

		sprintf( msg, "Skipped opcode 'NO DESCRIPTION AVAILABLE' 0x%x", oc );



	ImErrorWarning( msg, -1, ImErrNo = IMEUNSUPPORTED );



	return( -1 );

}





/*

 *                                                                      

 *  We have encountered an opcode which we do not know how to deal with. 

 *  The opcode/data is too complex to even be able to skip, so tell the   

 *  user we are going to have to stop where we are.                        

 *

 */

#ifdef __STDC__

static int OpcodeUns( unsigned short oc )

#else

static int OpcodeUns( oc )

	unsigned short oc;

#endif

{

	char msg[ IMSTR_LEN ];

	sprintf( msg, "Unsupported opcode 0x%x", oc );

	ImErrorFatal( msg, -1, ImErrNo = IMESYNTAX );

}





/*

 *                                                                     

 *  We have read an opcode that we could normaly handle, but we have  

 *  noticed that there is bad data following the opcode.  Tell the user 

 *  that we can not continue.                                          

 *                                                                    

 */

#ifdef __STDC__

static int OpcodeErr( unsigned short oc )

#else

static int OpcodeErr( oc )

	unsigned short oc;

#endif

{

	char msg[ IMSTR_LEN ];

	sprintf( msg, "Bad data in opcode 0x%x", oc );

	ImErrorFatal( msg, -1, ImErrNo = IMESYNTAX );

}





#ifdef __STDC__

static int tstBit( unsigned char uchr, int cnt )

#else

static int tstBit( uchr, cnt )

	unsigned char uchr;

	int cnt;

#endif

{

	if ( (cnt>7) || (cnt<0) )  return( (int) -1 );



	return( (int) ((1<<cnt) & uchr) );

}





#ifdef __STDC__

static int SkipBytes( long count )

#else

static int SkipBytes( count )

	long count;

#endif

{

	int result=0;

	int skip=0;



	while ( count > 0 )

	{

		if ( count <= IMSKIP_BUF_SIZ )

			skip = count;

		else

			skip = IMSKIP_BUF_SIZ;



		result = ImBinRead( g_ioType, g_fd, g_fp, g_buf, CHAR, 1, skip );

		if ( result <= -1 )  return( result );

		count -= skip;

	}



	return( result );

}



/*

 *

 *	Opcode handlers / functions

 *

 *

 */



/* 0x0000 - Null operation - 0 */

static int

PO_NOP( )

{

	int result = 0;



	return( result );

}





/* 0x0001 - define clipRegion - region size */

static int

PO_Clip( )

{

	int result = 10;

	struct Region rgn;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &rgn, RegionFields ) == -1 )

	{

		ImReturnBinError( );

	}

	if ( rgn.rgnSize != 10 )

	{

		result += (rgn.rgnSize - 10);

		if ( SkipBytes( (long) rgn.rgnSize - 10 ) == -1 )

		{

			ImReturnBinError( );

		}

	}

	else

	{

		g_gs.clipRgn.rgnBBox.top    = rgn.rgnBBox.top;

		g_gs.clipRgn.rgnBBox.left   = rgn.rgnBBox.left;

		g_gs.clipRgn.rgnBBox.bottom = rgn.rgnBBox.bottom;

		g_gs.clipRgn.rgnBBox.right  = rgn.rgnBBox.right;

	}



	return( result );



}





/* 0x0002 - background pattern - 8 */

static int

PO_BkPat( )

{

	int result = 8;

	unsigned char uc;



	uc = (unsigned char) result;

	if ( ImBinRead( g_ioType, g_fd, g_fp, g_gs.bkPat, UCHAR, 1, uc ) == -1 )

	{

		ImReturnBinError( );

	}



	return( result );

}





/* 0x0003 - text font (word) - 2 */

static int

PO_TxFont( )

{

	int result = 2;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &g_gs.txFont, SHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}



	return( result );



}





/* 0x0004 - text face (byte) - 1 */

static int

PO_TxFace( )

{

	int result = 1;

	if ( ImBinRead( g_ioType, g_fd, g_fp, &g_gs.txFace, UCHAR, 1, 1 ) == -1 )

	{

		ImReturnBinError( );

	}



	return( result );



}





/* 0x0005 - text mode (word) - 2 */

static int

PO_TxMode( )

{

	int result = 2;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &g_gs.txMode, SHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}



	return( result );



}





/* 0x0006 - space extra (fixed point) - 4 */

static int

PO_SpExtra( )

{

	int result = 4;

	if ( ImBinRead( g_ioType, g_fd, g_fp, &g_gs.spExtra, IMFIXED, 4, 1 ) == -1 )

	{

		ImReturnBinError( );

	}



	return( result );

}





/* 0x0007 - pen size (point) - 4 */

static int

PO_PnSize( )

{

	int result = 4;

	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.pnSize, PointFields)== -1)

	{

		ImReturnBinError( );

	}



	return( result );



}





/* 0x0008 - pen mode (word) - 2 */

static int

PO_PnMode( )

{

	int result = 2;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &g_gs.pnMode, SHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}



        return( result );

 

}





/* 0x0009 - pen pattern - 8 */

static int

PO_PnPat( )

{

	int result = 8;

	unsigned char uc;



	uc = (unsigned char) result;

	if ( ImBinRead( g_ioType, g_fd, g_fp, g_gs.pnPat, UCHAR, 1, uc ) == -1 )

	{

		ImReturnBinError( );

	}



        return( result );

 

}





/* 0x000A - fill pattern - 8 */

static int

PO_FillPat( )

{

	int result = 8;

	unsigned char uc;



	uc = (unsigned char) result;

	if ( ImBinRead( g_ioType, g_fd, g_fp, g_gs.fillPat, UCHAR, 1, uc )== -1)

	{

		ImReturnBinError( );

	}

 

        return( result );



}





/* 0x000B - oval size (point) - 4 */

static int

PO_OvSize( )

{

	int result = 4;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.ovSize, PointFields)== -1)

	{

		ImReturnBinError( );

	}

 

        return( result );



}





/* 0x000C - dh, dv (word) - 4 */

static int

PO_Origin( )

{

	int result = 4;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.origin, PointFields)== -1)

	{

		ImReturnBinError( );

	}

 

        return( result );



}





/* 0x000D - text size (word) - 2 */

static int

PO_TxSize( )

{

	int result = 2;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &g_gs.txSize, SHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

 

        return( result );



}





/* 0x000E - foreground color (long) - 4 */

static int

PO_FgColor( )

{

	int result = 4;

	if ( ImBinRead( g_ioType, g_fd, g_fp, &g_gs.fgColor, LONG, 4, 1 ) == -1 )

	{

		ImReturnBinError( );

	}



	return( result );

}





/* 0x000F - background color (long) - 4 */

static int

PO_BkColor( )

{

	int result = 4;

	if ( ImBinRead( g_ioType, g_fd, g_fp, &g_gs.bkColor, LONG, 4, 1 ) == -1 )

	{

		ImReturnBinError( );

	}



	return( result );



}





/* 0x0010 - numer, denom (points) - 4 */

static int

PO_TxRatio( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.txRatNum, PointFields)== -1)

	{

		ImReturnBinError( );

	}

	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.txRatDen, PointFields)== -1)

	{

		ImReturnBinError( );

	}



	return( result );



}





/* 0x0011 - version (byte) - 1 */

static int

PO_Version( )

{

	int result = 1;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &g_gs.version, UCHAR, 1, 1 ) == -1 )

	{

		ImReturnBinError( );

	}



	return( result );



}





/* 0x0012 - color background pattern - variable */

static int

PO_BkPixPat( )

{

	int result = 2;

	unsigned short ushort1;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &ushort1, SHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	if ( ushort1 == 1 )

	{

		result += -666;

		if ( SkipBytes( (long) -666 ) == -1 )

		{

			ImReturnBinError( );

		}

	}

	else

	{	/* ushort1 == 2 */

		result += 70;

		if ( SkipBytes( (long) 70 ) == -1 )

		{

			ImReturnBinError( );

		}

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0013 - color pen pattern - variable */

static int

PO_PnPixPat( )

{

	int result = 2;

	unsigned short ushort1=0;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &ushort1, SHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	if ( ushort1 == 1 )

	{

		result += -666;

		if ( SkipBytes( (long) -666 ) == -1 )

		{

			ImReturnBinError( );

		}

	}

	else

	{	/* ushort1 == 2 */

		result += 70;

		if ( SkipBytes( (long) 70 ) == -1 )

		{

			ImReturnBinError( );

		}

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0014 - color fill pattern - variable */

static int

PO_FillPixPat( )

{

	int result = 2;

	int i;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &i, INT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	if ( i == 1 )

	{

		result += -666;

		if ( SkipBytes( (long) -666 ) == -1 )

		{

			ImReturnBinError( );

		}

	}

	else

	{	/* i == 2 */

		result += 70;

		if ( SkipBytes( (long) 70 ) == -1 )

		{

			ImReturnBinError( );

		}

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0015 - fractional pen position - 2 */

static int

PO_PnLocHFrac( )

{

	int result = 2;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &g_gs.pnLocHFrac, SHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}



	return( result );



}





/* 0x0016 - extra for each character - 2 */

static int

PO_ChExtra( )

{

	int result = 2;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &g_gs.chExtra, SHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}



	return( result );



}





/* 0x0017 - reserved for Apple use - 0 */

static int

PO_reserved1( )

{

	int result = 0;



	return( result );



}





/* 0x0018 - reserved for Apple use - 0 */

static int

PO_reserved2( )

{

	int result = 0;



	return( result );



}





/* 0x0019 - reserved for Apple use - 0 */

static int

PO_reserved3( )

{

	int result = 0;



	return( result );



}





/* 0x001A - set RGB foreground color - variable */

static int

PO_RGBFgCol( )

{

	int result = 4;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.rgbFgColor, RGBColorFields)== -1)

	{

		ImReturnBinError( );

	}



	return( result );



}





/* 0x001B - set RGB background color - variable */

static int

PO_RGBBkCol( )

{

	int result = 4;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.rgbBkColor, RGBColorFields)== -1)

	{

		ImReturnBinError( );

	}



	return( result );



}





/* 0x001C - hilite mode flag - 0 */

static int

PO_HiliteMode( )

{

	int result = 0;



	g_gs.hiliteMode = ! g_gs.hiliteMode;



	return( result );



}





/* 0x001D - RGB hilite color - variable */

static int

PO_HiliteColor( )

{

	int result = 4;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.hiliteColor, RGBColorFields)== -1)

	{

		ImReturnBinError( );

	}



	return( result );



}





/* 0x001E - use default hilite color - 0 */

static int

PO_DefHilite( )

{

	int result = 0;



	g_gs.hiliteColor.red   = 0;

	g_gs.hiliteColor.green = 0;

	g_gs.hiliteColor.blue  = 0;



	return( result );



}





/* 0x001F - RGB OpColor : arith. modes - 0 */

static int

PO_OpColor( )

{

	int result = 0;



	g_gs.opColor = ! g_gs.opColor;



	return( result );



}





/* 0x0020 - pnLoc, newPt (points) - 8 */

static int

PO_Line( )

{

	int result = 8;

	struct Point point1;



	if (ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.pnLoc, PointFields )==-1)

	{

		ImReturnBinError( );

	}

	if (ImBinReadStruct( g_ioType, g_fd, g_fp, &point1, PointFields )==-1)

	{

		ImReturnBinError( );

	}

	rline( g_gs.pnLoc.h, g_gs.pnLoc.v, point1.h, point1.v );



	return( result );



}





/* 0x0021 - newPt (point) - 4 */

static int

PO_LineFrom( )

{

	int result = 4;

	struct Point point1;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &point1, PointFields ) == -1 )

	{

		ImReturnBinError( );

	}

	rline( g_gs.pnLoc.h, g_gs.pnLoc.v, point1.h, point1.v );



	return( result );



}





/* 0x0022 - pnLoc (point), dh, dv - 6 */

static int

PO_ShortLine( )

{

	int result = 6;

	char char1, char2;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.pnLoc, PointFields ) == -1 )

	{

		ImReturnBinError( );

	}

	if ( ImBinRead( g_ioType, g_fd, g_fp, &char1, CHAR, 1, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	if ( ImBinRead( g_ioType, g_fd, g_fp, &char2, CHAR, 1, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	rline( g_gs.pnLoc.h, g_gs.pnLoc.v,

			(g_gs.pnLoc.h + char1), (g_gs.pnLoc.v + char2) );



	return( result );



}





/* 0x0023 - dh (byte), dv (byte) - 2 */

static int

PO_ShortLineFrom( )

{

	int result = 2;

	char char1, char2;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &char1, CHAR, 1, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	if ( ImBinRead( g_ioType, g_fd, g_fp, &char2, CHAR, 1, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	rline( g_gs.pnLoc.h, g_gs.pnLoc.v,

			g_gs.pnLoc.h + char1, g_gs.pnLoc.v + char2 );



	return( result );



}





/* 0x0024 - reserved for Apple use - 2+data */

static int

PO_reserved4( )

{

	int result = 2;

	unsigned short ushort1=0;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &ushort1, USHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	if ( ushort1 > 0 )

	{

		result += ushort1;

		if ( SkipBytes( (long) ushort1 ) == -1 )

		{

			ImReturnBinError( );

		}

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0025 - reserved for Apple use - 2+data */

static int

PO_reserved5( )

{

	int result = 2;

	unsigned short ushort1=0;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &ushort1, USHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	if ( ushort1 > 0 )

	{

		result += ushort1;

		if ( SkipBytes( (long) ushort1 ) == -1 )

		{

			ImReturnBinError( );

		}

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0026 - reserved for Apple use - 2+data */

static int

PO_reserved6( )

{

	int result = 2;

	unsigned short ushort1=0;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &ushort1, USHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	if ( ushort1 > 0 )

	{

		result += ushort1;

		if ( SkipBytes( (long) ushort1 ) == -1 )

		{

			ImReturnBinError( );

		}

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0027 - reserved for Apple use - 2+data */

static int

PO_reserved7( )

{

	int result = 2;

	unsigned short ushort1=0;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &ushort1, USHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	if ( ushort1 > 0 )

	{

		result += ushort1;

		if ( SkipBytes( (long) ushort1 ) == -1 )

		{

			ImReturnBinError( );

		}

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0028 - txLoc (point), count, text - 5+text */

static int

PO_LongText( )

{

	int result = 5;

	struct Point point1;

	unsigned char uchar1;

	char msg[ IMSTR_LEN ];



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &point1, PointFields ) == -1 )

	{

		ImReturnBinError( );

	}

	if ( ImBinRead( g_ioType, g_fd, g_fp, &uchar1, UCHAR, 1, 1 )== -1)

	{

		ImReturnBinError( );

	}

	result += uchar1;

	if ( SkipBytes( (long) uchar1 ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	g_buf[uchar1] = '\0';



#ifdef DEBUG

	fprintf( stderr, "SKIPPED TEXT: '%s'\n", g_buf );

#endif



	return( result );



}





/* 0x0029 - dh, count, text - 2+text */

static int

PO_DHText( )

{

	int result = 2;

	unsigned char uchar1, uchar2;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &uchar1, UCHAR, 1, 1 )== -1)

	{

		ImReturnBinError( );

	}

	if ( ImBinRead( g_ioType, g_fd, g_fp, &uchar2, UCHAR, 1, 1 )== -1)

	{

		ImReturnBinError( );

	}

	result += uchar2;

	if ( SkipBytes( (long) uchar2 ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	g_buf[uchar2] = '\0';



#ifdef DEBUG

	fprintf( stderr, "SKIPPED TEXT: '%s'\n", g_buf );

#endif



	return( result );



}





/* 0x002A - dv, count, text - 2+text */

static int

PO_DVText( )

{

	int result = 2;

	unsigned char uchar1, uchar2;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &uchar1, UCHAR, 1, 1 )== -1)

	{

		ImReturnBinError( );

	}

	if ( ImBinRead( g_ioType, g_fd, g_fp, &uchar2, UCHAR, 1, 1 )== -1)

	{

		ImReturnBinError( );

	}

	result += uchar2;

	if ( SkipBytes( (long) uchar2 ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	g_buf[uchar2] = '\0';



#ifdef DEBUG

	fprintf( stderr, "SKIPPED TEXT: '%s'\n", g_buf );

#endif



	return( result );



}





/* 0x002B - dh, dv, count, text - 3+text */

static int

PO_DHDVText( )

{

	int result = 3;

	unsigned char uchar1, uchar2, uchar3;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &uchar1, UCHAR, 1, 1 )== -1)

	{

		ImReturnBinError( );

	}

	if ( ImBinRead( g_ioType, g_fd, g_fp, &uchar2, UCHAR, 1, 1 )== -1)

	{

		ImReturnBinError( );

	}

	if ( ImBinRead( g_ioType, g_fd, g_fp, &uchar3, UCHAR, 1, 1 )== -1)

	{

		ImReturnBinError( );

	}

	result += uchar3;

	if ( SkipBytes( (long) uchar3 ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	g_buf[uchar3] = '\0';



#ifdef DEBUG

	fprintf( stderr, "SKIPPED TEXT: '%s'\n", g_buf );

#endif



	return( result );



}





/* 0x002C - reserved for Apple use - 2+data */

static int

PO_reserved8( )

{

	int result = 2;

	short short1=0;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &short1, SHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}

	result += short1;

	if ( SkipBytes( (long) short1 ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x002D - reserved for Apple use - 2+data */

static int

PO_reserved9( )

{

	int result = 2;

	short short1=0;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &short1, SHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}

	result += short1;

	if ( SkipBytes( (long) short1 ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x002E - reserved for Apple use - 2+data */

static int

PO_reserved10( )

{

	int result = 2;

	short short1=0;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &short1, SHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}

	result += short1;

	if ( SkipBytes( (long) short1 ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x002F - reserved for Apple use - 2+data */

static int

PO_reserved11( )

{

	int result = 2;

	short short1=0;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &short1, SHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}

	result += short1;

	if ( SkipBytes( (long) short1 ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0030 - rect - 8 */

static int

PO_FrameRect( )

{

	int result = 8;

	int i=0;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

#ifdef DEBUG

fprintf( stderr, "DEBUG FrameRect: t, l, b, r = 0x%0x, 0x%0x, 0x%0x, 0x%0x\n", g_gs.lastRect.top, g_gs.lastRect.left, g_gs.lastRect.bottom, g_gs.lastRect.right );

#endif

	/* Draw the sides of the rectangle		*/

	for ( i=g_gs.lastRect.top; i<=(g_gs.lastRect.bottom-g_gs.pnSize.v); i++ )

	{

		PaintPixel( g_gs.lastRect.left, i, g_gs.rgbFgColor );

		PaintPixel( g_gs.lastRect.right-g_gs.pnSize.h, i, g_gs.rgbFgColor );

	}

	/* Draw the top & bottom of the rectangle	*/

	for ( i=g_gs.lastRect.left; i<=(g_gs.lastRect.right-g_gs.pnSize.h); i++ )

	{

		PaintPixel( i, g_gs.lastRect.top, g_gs.rgbFgColor );

		PaintPixel( i, g_gs.lastRect.bottom-g_gs.pnSize.v, g_gs.rgbFgColor );

	}



	return( result );



}





/* 0x0031 - rect - 8 */

static int

PO_PaintRect( )

{

	int result = 8;

	int i=0, j=0;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	for ( i=g_gs.lastRect.left+1; i<g_gs.lastRect.right; i++ )

	{

		for ( j=g_gs.lastRect.top+1; j<g_gs.lastRect.bottom; j++ )

		{

			PaintPixel( i, j, g_gs.rgbBkColor );

		}

	}



	return( result );



}





/* 0x0032 - rect - 8 */

static int

PO_EraseRect( )

{

	int result = 8;

	int i=0, j=0;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	for ( i=g_gs.lastRect.left+1; i<g_gs.lastRect.right; i++ )

	{

		for ( j=g_gs.lastRect.top+1; j<g_gs.lastRect.bottom; j++ )

		{

			PaintPixel( i, j, g_gs.rgbBkColor );

		}

	}



	return( result );



}





/* 0x0033 - rect - 8 */

static int

PO_InvertRect( )

{

	int result = 8;

	ImVfbPtr pptr;		/* Pixel pointer	*/

	int i=0, j=0;

	struct RGBColor color;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	for ( i=g_gs.lastRect.left+1; i<g_gs.lastRect.right; i++ )

	{

		for ( j=g_gs.lastRect.top+1; j<g_gs.lastRect.bottom; j++ )

		{

			pptr = ImVfbQPtr( g_vfb, i, j );

			color.red = (~0) ^ ImVfbQRed( g_vfb, pptr );

			color.green = (~0) ^ ImVfbQGreen( g_vfb, pptr );

			color.blue = (~0) ^ ImVfbQBlue( g_vfb, pptr );

			SetPixel( i, j, color );

		}

	}



	return( result );



}





/* 0x0034 - rect - 8 */

static int

PO_FillRect( )

{

	int result = 8;

	int i=0, j=0;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	for ( i=g_gs.lastRect.left+1; i<g_gs.lastRect.right; i++ )

	{

		for ( j=g_gs.lastRect.top+1; j<g_gs.lastRect.bottom; j++ )

		{

			PaintPixel( i, j, g_gs.rgbFgColor );

		}

	}



	return( result );



}





/* 0x0035 - reserved for Apple use - 8 */

static int

PO_reserved12( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0036 - reserved for Apple use - 8 */

static int

PO_reserved13( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0037 - reserved for Apple use - 8 */

static int

PO_reserved14( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0038 - rect - 0 */

static int

PO_FrameSameRect( )

{

	int result = 0;

	int i=0;



#ifdef DEBUG

fprintf( stderr, "DEBUG FrameSameRect: t, l, b, r = 0x%0x, 0x%0x, 0x%0x, 0x%0x\n", g_gs.lastRect.top, g_gs.lastRect.left, g_gs.lastRect.bottom, g_gs.lastRect.right );

#endif

	/* Draw the sides of the rectangle		*/

	for ( i=g_gs.lastRect.top; i<=(g_gs.lastRect.bottom-g_gs.pnSize.v); i++ )

	{

		PaintPixel( g_gs.lastRect.left, i, g_gs.rgbFgColor );

		PaintPixel( g_gs.lastRect.right-g_gs.pnSize.h, i, g_gs.rgbFgColor );

	}

	/* Draw the top & bottom of the rectangle	*/

	for ( i=g_gs.lastRect.left; i<=(g_gs.lastRect.right-g_gs.pnSize.h); i++ )

	{

		PaintPixel( i, g_gs.lastRect.top, g_gs.rgbFgColor );

		PaintPixel( i, g_gs.lastRect.bottom-g_gs.pnSize.v, g_gs.rgbFgColor );

	}



	return( result );

}





/* 0x0039 - rect - 0 */

static int

PO_PaintSameRect( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x003A - rect - 0 */

static int

PO_EraseSameRect( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x003B - rect - 0 */

static int

PO_InvertSameRect( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x003C - rect - 0 */

static int

PO_FillSameRect( )

{

	int result = 0;

	int i=0;



	/* Draw the sides of the rectangle		*/



	for ( i=g_gs.lastRect.top; i<=g_gs.lastRect.bottom; i++ )

	{

		PaintPixel( g_gs.lastRect.left, i, g_gs.rgbFgColor );

		PaintPixel( g_gs.lastRect.right, i, g_gs.rgbFgColor );

	}



	/* Draw the top & bottom of the rectangle	*/



	for ( i=g_gs.lastRect.left; i<=g_gs.lastRect.right; i++ )

	{

		PaintPixel( i, g_gs.lastRect.top, g_gs.rgbFgColor );

		PaintPixel( i, g_gs.lastRect.bottom, g_gs.rgbFgColor );

	}





	return( result );



}





/* 0x003D - reserved for Apple use - 0 */

static int

PO_reserved15( )

{

	int result = 0;



	return( result );



}





/* 0x003E - reserved for Apple use - 0 */

static int

PO_reserved16( )

{

	int result = 0;



	return( result );



}





/* 0x003F - reserved for Apple use - 0 */

static int

PO_reserved17( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0040 - rect - 8 */

static int

PO_FrameRRect( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0041 - rect - 8 */

static int

PO_PaintRRect( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0042 - rect - 8 */

static int

PO_EraseRRect( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0043 - rect - 8 */

static int

PO_InvertRRect( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0044 - rect - 8 */

static int

PO_FillRRect( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0045 - reserved for Apple use - 8 */

static int

PO_reserved18( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0046 - reserved for Apple use - 8 */

static int

PO_reserved19( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0047 - reserved for Apple use - 8 */

static int

PO_reserved20( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0048 - rect - 0 */

static int

PO_FrameSameRRect( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0049 - rect - 0 */

static int

PO_PaintSameRRect( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x004A - rect - 0 */

static int

PO_EraseSameRRect( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x004B - rect - 0 */

static int

PO_InvertSameRRect( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x004C - rect - 0 */

static int

PO_FillSameRRect( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x004D - reserved for Apple use - 0 */

static int

PO_reserved21( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x004E - reserved for Apple use - 0 */

static int

PO_reserved22( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x004F - reserved for Apple use - 0 */

static int

PO_reserved23( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0050 - rect - 8 */

static int

PO_FrameOval( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0051 - rect - 8 */

static int

PO_PaintOval( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0052 - rect - 8 */

static int

PO_EraseOval( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0053 - rect - 8 */

static int

PO_InvertOval( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0054 - rect - 8 */

static int

PO_FillOval( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0055 - reserved for Apple use - 8 */

static int

PO_reserved24( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0056 - reserved for Apple use - 8 */

static int

PO_reserved25( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0057 - reserved for Apple use - 8 */

static int

PO_reserved26( )

{

	int result = 8;



	if ( ImBinReadStruct( g_ioType, g_fd, g_fp, &g_gs.lastRect, RectFields)== -1)

	{

		ImReturnBinError();

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0058 - rect - 0 */

static int

PO_FrameSameOval( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0059 - rect - 0 */

static int

PO_PaintSameOval( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x005A - rect - 0 */

static int

PO_EraseSameOval( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x005B - rect - 0 */

static int

PO_InvertSameOval( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x005C - rect - 0 */

static int

PO_FillSameOval( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x005D - reserved for Apple use - 0 */

static int

PO_reserved27( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x005E - reserved for Apple use - 0 */

static int

PO_reserved28( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x005F - reserved for Apple use - 0 */

static int

PO_reserved29( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0060 - rect, startAngle, arcAngle - 12 */

static int

PO_FrameArc( )

{

	int result = 12;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0061 - rect, startAngle, arcAngle - 12 */

static int

PO_PaintArc( )

{

	int result = 12;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0062 - rect, startAngle, arcAngle - 12 */

static int

PO_EraseArc( )

{

	int result = 12;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0063 - rect, startAngle, arcAngle - 12 */

static int

PO_InvertArc( )

{

	int result = 12;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0064 - rect, startAngle, arcAngle - 12 */

static int

PO_FillArc( )

{

	int result = 12;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0065 - reserved for Apple use - 12 */

static int

PO_reserved30( )

{

	int result = 12;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0066 - reserved for Apple use - 12 */

static int

PO_reserved31( )

{

	int result = 12;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0067 - reserved for Apple use - 12 */

static int

PO_reserved32( )

{

	int result = 12;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0068 - rect - 4 */

static int

PO_FrameSameArc( )

{

	int result = 4;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0069 - rect - 4 */

static int

PO_PaintSameArc( )

{

	int result = 4;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x006A - rect - 4 */

static int

PO_EraseSameArc( )

{

	int result = 4;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x006B - rect - 4 */

static int

PO_InvertSameArc( )

{

	int result = 4;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x006C - rect - 4 */

static int

PO_FillSameArc( )

{

	int result = 4;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x006D - reserved for Apple use - 4 */

static int

PO_reserved33( )

{

	int result = 4;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x006E - reserved for Apple use - 4 */

static int

PO_reserved34( )

{

	int result = 4;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x006F - reserved for Apple use - 4 */

static int

PO_reserved35( )

{

	int result = 4;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0070 - polygon - poly size */

static int

PO_FramePoly( )

{

	int result = 2;

	int i=0;

	short short1=0, short2=0;

	struct Rect rect1;

	struct Point point1;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &short1, SHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}

	result += 8;

	if (ImBinReadStruct(g_ioType, g_fd, g_fp, &rect1, RectFields)==-1)

	{

		ImReturnBinError();

	}

	short2 = (short1 - 10) / 4;  /* how many points in the poly */

	result += (short2 * 4);       /* with 4 bytes per point */

	/* Read the first point as the new pen loc	*/

	if (ImBinReadStruct(g_ioType, g_fd, g_fp, &g_gs.pnLoc, PointFields)==-1)

	{

		ImReturnBinError( );

	}

#ifdef DEBUG

fprintf( stderr, "DEBUG FramePoly: polySize = %d\n", short1 );

fprintf( stderr, "DEBUG FramePoly: t, l, b, r = %d, %d, %d, %d\n", rect1.top, rect1.left, rect1.bottom, rect1.right );

fprintf( stderr, "DEBUG FramePoly: pt1 = %d, %d\n", g_gs.pnLoc.h, g_gs.pnLoc.v );

#endif

	/* Read and draw the rest of the points		*/

	for ( i=1; i<short2; i++ )

	{

		if (ImBinReadStruct(g_ioType, g_fd, g_fp, &point1, PointFields)==-1)

		{

			ImReturnBinError( );

		}

#ifdef DEBUG

fprintf( stderr, "DEBUG FramePoly: pt = %d, %d\n", point1.h, point1.v );

#endif

		rline( g_gs.pnLoc.h, g_gs.pnLoc.v, point1.h, point1.v );

	}



	return( result );



}





/* 0x0071 - polygon - poly size */

static int

PO_PaintPoly( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0072 - polygon - poly size */

static int

PO_ErasePoly( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0073 - polygon - poly size */

static int

PO_InvertPoly( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0074 - polygon - poly size */

static int

PO_FillPoly( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0075 - reserved for Apple use - poly size */

static int

PO_reserved36( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0076 - reserved for Apple use - poly size */

static int

PO_reserved37( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0077 - reserved for Apple use - poly size */

static int

PO_reserved38( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0078 - not yet Implemented - ? */

static int

PO_FrameSamePoly( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0079 - not yet Implemented - ? */

static int

PO_PaintSamePoly( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x007A - not yet Implemented - ? */

static int

PO_EraseSamePoly( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x007B - not yet Implemented - ? */

static int

PO_InvertSamePoly( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x007C - not yet implemented - ? */

static int

PO_FillSamePoly( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x007D - reserved for Apple use - 0 */

static int

PO_reserved39( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x007E - reserved for Apple use - 0 */

static int

PO_reserved40( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x007F - reserved for Apple use - 0 */

static int

PO_reserved41( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0080 - rgn - rgn size */

static int

PO_FrameRgn( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0081 - rgn - rgn size */

static int

PO_PaintRgn( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0082 - rgn - rgn size */

static int

PO_EraseRgn( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0083 - rgn - rgn size */

static int

PO_InvertRgn( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0084 - rgn - rgn size */

static int

PO_FillRgn( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0085 - reserved for Apple use - rgn size */

static int

PO_reserved42( )

{

	int result = 10;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0086 - reserved for Apple use - rgn size */

static int

PO_reserved43( )

{

	int result = 10;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0087 - reserved for Apple use - rgn size */

static int

PO_reserved44( )

{

	int result = 10;



	if ( SkipBytes( (long) result ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0088 - not yet implemented - ? */

static int

PO_FrameSameRgn( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0089 - not yet implemented - ? */

static int

PO_PaintSameRgn( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x008A - not yet implemented - ? */

static int

PO_EraseSameRgn( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x008B - not yet implemented - ? */

static int

PO_InvertSameRgn( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x008C - not yet implemented - ? */

static int

PO_FillSameRgn( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x008D - reserved for Apple use - 0 */

static int

PO_reserved45( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x008E - reserved for Apple use - 0 */

static int

PO_reserved46( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x008F - reserved for Apple use - 0 */

static int

PO_reserved47( )

{

	int result = 0;



	OpcodeWarn( g_Opcode );



	return( result );



}





/* 0x0092 - reserved for Apple use - 2+data */

static int

PO_reserved48( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0093 - reserved for Apple use - 2+data */

static int

PO_reserved49( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0094 - reserved for Apple use - 2+data */

static int

PO_reserved50( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0095 - reserved for Apple use - 2+data */

static int

PO_reserved51( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0096 - reserved for Apple use - 2+data */

static int

PO_reserved52( )

{

	int result = 0;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0097 - reserved for Apple use - 2+data */

static int

PO_reserved53( )

{

	int result = 2;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x0090, 0x0091, 0x0098, 0x0099 - CopyBits */

/* BitMaps - Version 1 PICTs */

static int

BitMap( )

{

	int result=0;		/* return value - number of bytes read  */

	unsigned short rowBytes;	/* PICT BitMap row byte count		*/

	short BCLen=0;		/* PICT BitMap byteCount length		*/

	struct BMap bMap;	/* PICT BitMap structure		*/

	unsigned short ushort1=0, ushort2=0;

	struct Rect rect1;

	short padRight;

	short sLines, lin;

	unsigned int byteCount=0;		/* PICT BitMap byteCount	*/

	unsigned char * runBuffer;

	unsigned char * pixBuffer;

	unsigned short byt;

	unsigned short bit;

	unsigned short bitC;

	int          x, y;	/* Convenient short names	*/

	char msg[ IMSTR_LEN ];

	struct RGBColor color;



	result = 2;

	/* read rowBytes */

	if ( ImBinRead( g_ioType, g_fd, g_fp, &rowBytes, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



	runBuffer = (unsigned char*) malloc( 4 * rowBytes );

	pixBuffer = (unsigned char*) malloc( 4 * rowBytes );

	if ( (runBuffer == NULL) || (pixBuffer == NULL) )

	{

		sprintf( msg, "Failed to allocate runBuffer" );

		ImErrorFatal( msg, -1, ImErrNo = IMESYNTAX );

	}



	/* if rowBytes > 250 then byteCount is a word */

	/* otherwise byteCount is a byte */

	if ( rowBytes > 250 ) BCLen = 2;

	else BCLen = 1;

	result += 26;

	if ( ImBinReadStruct(g_ioType, g_fd, g_fp, &bMap, BMapFields)== -1)

	{

		ImReturnBinError();

	}

	if ( g_Opcode == 0x0091 || g_Opcode == 0x0099 )

	{

		/*

		 *  If we have a region, read and ignore rgn data

		 */

		result += 10;

		if ( ImBinRead( g_ioType, g_fd, g_fp, &ushort1, USHORT, 2, 1 )== -1)

		{

			ImReturnBinError( );

		}

		if (ImBinReadStruct(g_ioType, g_fd, g_fp, &rect1, RectFields)==-1)

		{

			ImReturnBinError();

		}

		if ( ushort1 > 10 )

		{

			ushort2 = ushort1 - 10;

			result += ushort2;

			if ( SkipBytes( (long) ushort2 ) == -1 )

			{

				ImReturnBinError( );

			}

		}

	}

#ifdef DEBUG

fprintf( stderr, "DEBUG PackBitsRect ; rowBytes = %d\n", rowBytes );

fprintf( stderr, "DEBUG PackBitsRect ; bnd = {%d,%d,%d,%d}\n", bMap.bnd.top, bMap.bnd.left, bMap.bnd.bottom, bMap.bnd.right );

fprintf( stderr, "DEBUG PackBitsRect ; src = {%d,%d,%d,%d}\n", bMap.srcRect.top, bMap.srcRect.left, bMap.srcRect.bottom, bMap.srcRect.right );

fprintf( stderr, "DEBUG PackBitsRect ; dst = {%d,%d,%d,%d}\n", bMap.dstRect.top, bMap.dstRect.left, bMap.dstRect.bottom, bMap.dstRect.right );

#endif

	padRight = bMap.bnd.right - bMap.bnd.left;

	sLines = bMap.bnd.bottom - bMap.bnd.top;

	for ( lin=0; lin<sLines; lin++ )

	{

		if ( g_Opcode == 0x0098 )

		{

			result += 1;

			if ( ImBinRead(g_ioType, g_fd, g_fp, &byteCount, UINT, BCLen, 1)== -1)

			{

				ImReturnBinError( );

			}

#ifdef DEBUG

if ( lin==0 || lin==sLines-1 ) {

   fprintf( stderr, "DEBUG PackBitsRect ; sl=%d, bc=%d\n", lin+1, byteCount );

   if ( lin==0 )  fprintf( stderr, "...etc...\n" );

}

#endif

			result += byteCount;

			if ( ImBinRead( g_ioType, g_fd, g_fp, pixBuffer, UCHAR, 1, byteCount )== -1)

			{

				ImReturnBinError( );

			}

			UnpackBits( pixBuffer, runBuffer, &byteCount );

		}

		else

		{	/* g_Opcode == PO_BitsRect */

			byteCount = rowBytes;

			result += byteCount;

			if ( ImBinRead( g_ioType, g_fd, g_fp, runBuffer, UCHAR, 1, byteCount )== -1)

			{

				ImReturnBinError( );

			}

#ifdef DEBUG

fprintf( stderr, "DEBUG BitsRect ; sl=%d, bs=%d\n", lin+1, byteCount );

#endif

		}

		for ( byt=0; byt<byteCount; byt++ )

		{

			for ( bit=0; bit<8; bit++ )

			{

				if ( (byt*8 + bit) > padRight )

					break;

				x = bMap.dstRect.left + byt*8 + bit;

				y = bMap.dstRect.top + lin;

				if ( tstBit( runBuffer[byt], 7-bit ) )

					bitC = IMBLACK;

				else

					bitC = IMWHITE;

				color.red = color.green = color.blue = bitC;

				SetPixel( x, y, color );

			} /* bits in each byte in each scanline */

		} /* byte in each scanline */

	} /* each scanline */



	free( (char *)runBuffer );

	free( (char *)pixBuffer );

	return( result );

}





/* 0x0090, 0x0091, 0x0098, 0x0099 - CopyBits */

/* PixMaps - Version 2 PICTs */

static int

PixMap( )

{

	int result=0;		/* return value - number of bytes read  */

	int i=0;		/* generic loop counter			*/

	unsigned short rowBytes;	/* PICT PixMap row byte count		*/

	short BCLen=0;		/* PICT PixMap byteCount length		*/

	struct PMap pMap;	/* PICT PixMap structure		*/

	unsigned short ushort1=0;

	unsigned short ushort2=0;

	struct Rect rect1;

	short padRight;

	short sLines, lin;

	unsigned int byteCount=0;	/* PICT PixMap byteCount		*/

	struct CTable cTab;	/* PICT ColorLookupTable struct		*/

	unsigned short byt;

	int          x, y;	/* Convenient short names		*/

	struct CTEntry ctEnt;	/* PICT ColorLookupTable Entry struct	*/

	ImCltPtr ctPtr;		/* VFB ColorTable pointer		*/

	unsigned char * runBuffer;

	unsigned char * pixBuffer;

	char msg[ IMSTR_LEN ];

	struct RGBColor color;

	struct Rect pm_srcRect;

	struct Rect pm_dstRect;

	int old_mode;		/* Remember the old pen transfer mode	*/

	char message[100];	/* ImInfo message			*/



	result = 2;

	/* read rowBytes */

	if ( ImBinRead( g_ioType, g_fd, g_fp, &rowBytes, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



	runBuffer = (unsigned char*) malloc( 4 * rowBytes );

	pixBuffer = (unsigned char*) malloc( 4 * rowBytes );

	if ( (runBuffer == NULL) || (pixBuffer == NULL) )

	{

		sprintf( msg, "Failed to allocate runBuffer" );

		ImErrorFatal( msg, -1, ImErrNo = IMESYNTAX );

	}



	/* if rowBytes >= 0x8000 then we have a PixMap */

	if ( rowBytes >= 0x8000 )

	{

		rowBytes = 0x7FFF & rowBytes;

	}

	/* if rowBytes > 250 then byteCount is a word */

	/* otherwise byteCount is a byte */

	if ( rowBytes > 250 )

		BCLen = 2;

	else

		BCLen = 1;

	result += 46;

	if ( ImBinReadStruct(g_ioType, g_fd, g_fp, &pMap, PMapFields)== -1)

	{

		ImReturnBinError();

	}

	/* Do we have a REGION-Clipped PixMaps ? */

	if ( g_Opcode == 0x0091 || g_Opcode == 0x0099 )

	{

		/* If we have a region, read and ignore rgn data */

		result += 10;

		if ( ImBinRead( g_ioType, g_fd, g_fp, &ushort1, USHORT, 2, 1 )== -1)

		{

			ImReturnBinError( );

		}

		if (ImBinReadStruct(g_ioType, g_fd, g_fp, &rect1, RectFields)==-1)

		{

			ImReturnBinError();

		}

		if ( ushort1 > 10 )

		{

			ushort2 = ushort1 - 10;

			result += ushort2;

			if ( SkipBytes( (long) ushort2 ) == -1 )

			{

				ImReturnBinError( );

			}

		}

	}

#ifdef DEBUG

fprintf( stderr, "DEBUG PackBitsRect ; rowBytes = %d\n", rowBytes );

fprintf( stderr, "DEBUG PackBitsRect ; bnd = {%d,%d,%d,%d}\n", pMap.bnd.top, pMap.bnd.left, pMap.bnd.bottom, pMap.bnd.right );

#endif

	padRight = pMap.bnd.right - pMap.bnd.left;

	sLines = pMap.bnd.bottom - pMap.bnd.top;

	/* Read the colorTable header */

	result += 8;



	sprintf( message, "%d-bit Color Indexed", pMap.pixelSize );

	ImInfo( "Type", message );



	if (ImBinReadStruct(g_ioType, g_fd, g_fp, &cTab, CTableFields)==-1)

	{

		ImReturnBinError();

	}

	/* Allocate a new IM-CLT */

	if ( clt == NULL )

	{

		clt = ImCltAlloc( cTab.ctSize + 1 );

		ImVfbSClt( g_vfb, clt );

		sprintf( message, "%d Entries", (cTab.ctSize+1) );

		ImInfo( "Color Table", message );

	}

	else

	{

		ImInfo( "Color Table", "none" );

	}



	/* Read the colorTable data */

	for ( i=0; i<=cTab.ctSize; i++ )

	{

		result += 8;

		if (ImBinReadStruct(g_ioType,g_fd,g_fp,&ctEnt,CTEntryFields)==-1)

		{

			ImReturnBinError();

		}

#ifdef DEBUG

if ( i==0 || i==cTab.ctSize ) {

   fprintf( stderr, "DEBUG PackBitsRect ; ctEnt[%d] = (r=0x%0x,g=0x%0x,b=0x%0x)\n", i, ctEnt.red, ctEnt.green, ctEnt.blue );

   if ( i==0 )  fprintf( stderr, "...etc...\n" );

}

#endif

		ctPtr = ImCltQPtr( clt, i );

		ImCltSRed( ctPtr, ctEnt.red );

		ImCltSGreen( ctPtr, ctEnt.green );

		ImCltSBlue( ctPtr, ctEnt.blue );

	}



	/* Read the srcRect, dstRect, and transfer mode */

	result += 18;

	if (ImBinReadStruct(g_ioType,g_fd,g_fp,&pm_srcRect,RectFields)==-1)

	{

		ImReturnBinError();

	}

	if (ImBinReadStruct(g_ioType,g_fd,g_fp,&pm_dstRect,RectFields)==-1)

	{

		ImReturnBinError();

	}

	old_mode = g_gs.pnMode;   /* Remember global pen transfer mode */

	if ( ImBinRead( g_ioType, g_fd, g_fp, &g_gs.pnMode, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



	/* Read and process the PixData scanlines */

	for ( lin=0; lin<sLines; lin++ )

	{

		if ( g_Opcode == 0x0098 )

		{

			result += BCLen;

			if ( ImBinRead(g_ioType, g_fd, g_fp, &byteCount, UINT, BCLen, 1)== -1)

			{

				ImReturnBinError( );

			}

#ifdef DEBUG

if ( lin==0 || lin==sLines-1 ) {

   fprintf( stderr, "DEBUG PackBitsRect ; sl=%d, bc=%d\n", lin+1, byteCount );

   if ( lin==0 )  fprintf( stderr, "...etc...\n" );

}

#endif

			result += byteCount;

			if ( ImBinRead( g_ioType, g_fd, g_fp, pixBuffer, UCHAR, 1, byteCount )== -1)

			{

				ImReturnBinError( );

			}

			UnpackBits( pixBuffer, runBuffer, &byteCount );

		}

		else

		{	/* g_Opcode == PO_BitsRect */

			byteCount = rowBytes;

			result += byteCount;

			if ( ImBinRead( g_ioType, g_fd, g_fp, runBuffer, UCHAR, 1, byteCount )== -1)

			{

				ImReturnBinError( );

			}

#ifdef DEBUG

fprintf( stderr, "DEBUG BitsRect ; sl=%d, bc=%d\n", lin+1, byteCount );

#endif

		}

		for ( byt=0; byt<byteCount; byt++ )

		{

			x = pMap.bnd.left + byt;



			/* Don't draw pad-bits (beyond pixmap bounds) */

			if ( x >= pMap.bnd.right )  break;

#ifdef DEBUG

			y = g_pictFrame.bottom - (pMap.bnd.top + lin);

#else

			y = pMap.bnd.top + lin;

#endif

			ctPtr = ImCltQPtr( clt, runBuffer[byt] );

			color.red = ImCltQRed( ctPtr );

			color.green = ImCltQGreen( ctPtr );

			color.blue = ImCltQBlue( ctPtr );

			SetPixel( x, y, color );

		} /* byte in each scanline */

	} /* each scanline */



	g_gs.pnMode = old_mode;   /* Restore the global pen transfer mode */



	return( result );

}





/* 0x009A, 0x009B - DirectBits CopyBits */

/* 16 and 32 bit DirectBits PixMaps - Version 2 PICTs */

static int

DirectBits( )

{

	int result=0;		/* return value - number of bytes read  */

	int i=0;		/* generic loop counter			*/

	unsigned short rowBytes;	/* PICT PixMap row byte count		*/

	short BCLen=0;		/* PICT PixMap byteCount length		*/

	struct PMap pMap;	/* PICT PixMap structure		*/

	unsigned short ushort1=0;

	unsigned short ushort2=0;

	struct Rect rect1;

	short padRight;

	short sLines, lin;

	unsigned int byteCount=0;	/* PICT PixMap byteCount		*/

	unsigned short byt;

	int          x, y;	/* Convenient short names		*/

	struct CTEntry ctEnt;	/* PICT ColorLookupTable Entry struct	*/

	ImCltPtr ctPtr;		/* VFB ColorTable pointer		*/

	unsigned char * runBuffer;

	unsigned char * pixBuffer;

	char msg[ IMSTR_LEN ];

	struct RGBColor color;

	struct Rect pm_srcRect;

	struct Rect pm_dstRect;

	int old_mode;		/* Remember the old pen transfer mode	*/

	char message[100];	/* ImInfo message			*/

	int chanOff;		/* Component offset into pixel data */

	int alphaOff;		/* Alpha component offset into pixel data */



	result = 2;



	/* read and skip baseAddr */

	result += 4;

	if ( SkipBytes( (long) 4 ) == -1 )

	{

		ImReturnBinError( );

	}



	/* read rowBytes */

	if ( ImBinRead( g_ioType, g_fd, g_fp, &rowBytes, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



	runBuffer = (unsigned char*) malloc( 4 * rowBytes );

	pixBuffer = (unsigned char*) malloc( 4 * rowBytes );

	if ( (runBuffer == NULL) || (pixBuffer == NULL) )

	{

		sprintf( msg, "Failed to allocate runBuffer" );

		ImErrorFatal( msg, -1, ImErrNo = IMESYNTAX );

	}



	/* if rowBytes >= 0x8000 then we have a PixMap */

	if ( rowBytes >= 0x8000 )

	{

		rowBytes = 0x7FFF & rowBytes;

	}

	/* if rowBytes > 250 then byteCount is a word */

	/* otherwise byteCount is a byte */

	if ( rowBytes > 250 )

		BCLen = 2;

	else

		BCLen = 1;



	result += 46;

	if ( ImBinReadStruct(g_ioType, g_fd, g_fp, &pMap, PMapFields)== -1)

	{

		ImReturnBinError();

	}



	/* Do we have a REGION-Clipped PixMaps ? */

	if ( g_Opcode == 0x009B )

	{

		/* If we have a region, read and ignore rgn data */

		result += 10;

		if ( ImBinRead( g_ioType, g_fd, g_fp, &ushort1, USHORT, 2, 1 )== -1)

		{

			ImReturnBinError( );

		}

		if (ImBinReadStruct(g_ioType, g_fd, g_fp, &rect1, RectFields)==-1)

		{

			ImReturnBinError();

		}

		if ( ushort1 > 10 )

		{

			ushort2 = ushort1 - 10;

			result += ushort2;

			if ( SkipBytes( (long) ushort2 ) == -1 )

			{

				ImReturnBinError( );

			}

		}

	}

#ifdef DEBUG

fprintf( stderr, "DEBUG PackBitsRect ; rowBytes = %d\n", rowBytes );

fprintf( stderr, "DEBUG PackBitsRect ; bnd = {%d,%d,%d,%d}\n", pMap.bnd.top, pMap.bnd.left, pMap.bnd.bottom, pMap.bnd.right );

#endif

	padRight = pMap.bnd.right - pMap.bnd.left;

	sLines = pMap.bnd.bottom - pMap.bnd.top;



	/* Read the srcRect, dstRect, and transfer mode */

	result += 18;

	if (ImBinReadStruct(g_ioType,g_fd,g_fp,&pm_srcRect,RectFields)==-1)

	{

		ImReturnBinError();

	}

	if (ImBinReadStruct(g_ioType,g_fd,g_fp,&pm_dstRect,RectFields)==-1)

	{

		ImReturnBinError();

	}

	old_mode = g_gs.pnMode;   /* Remember global pen transfer mode */

	if ( ImBinRead( g_ioType, g_fd, g_fp, &g_gs.pnMode, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



	/* Read and process the PixData scanlines */

	for ( lin=0; lin<sLines; lin++ )

	{

		result += BCLen;

		if ( ImBinRead(g_ioType, g_fd, g_fp, &byteCount, UINT, BCLen, 1)== -1)

		{

			ImReturnBinError( );

		}

#ifdef DEBUG

if ( lin==0 || lin==sLines-1 ) {

   fprintf( stderr, "DEBUG PackBitsRect ; sl=%d, bc=%d\n", lin+1, byteCount );

   if ( lin==0 )  fprintf( stderr, "...etc...\n" );

}

#endif

		result += byteCount;

		if ( ImBinRead( g_ioType, g_fd, g_fp, pixBuffer, UCHAR, 1, byteCount )== -1)

		{

			ImReturnBinError( );

		}



#ifdef DEBUG

		printf( "DEBUG: byteCount (before unpacking) = %d\n", byteCount );

#endif



		if ( pMap.packType == 3 )

		{



			/* ITS A 16 BIT PIXMAP */



			UnpackBits3( pixBuffer, runBuffer, &byteCount );



			for ( byt=0; byt<byteCount; byt+=2 )

			{



				x = pMap.bnd.left + ( byt / 2 );



				/* 16 bit RLE by pixelSize chunks */

				ushort1 = runBuffer[byt];

				ushort1 = ushort1 << 8;

				ushort1 = ushort1 | runBuffer[byt+1];

				/* Construct BLUE */

				color.blue = ushort1 & 0x1f;

				color.blue = color.blue << 3;

				ushort1 = ushort1 >> 2;

				color.blue = color.blue | (ushort1 & 0x7);

				ushort1 = ushort1 >> 3;

				/* Construct GREEN */

				color.green = ushort1 & 0x1f;

				color.green = color.green << 3;

				ushort1 = ushort1 >> 2;

				color.green = color.green | (ushort1 & 0x7);

				ushort1 = ushort1 >> 3;

				/* Construct RED */

				color.red = ushort1 & 0x1f;

				color.red = color.red << 3;

				ushort1 = ushort1 >> 2;

				color.red = color.red | (ushort1 & 0x7);

				ushort1 = ushort1 >> 3;



				/* Don't draw pad-bits (beyond pixmap bounds) */

				if ( x >= pMap.bnd.right )  break;

				y = pMap.bnd.top + lin;

				SetPixel( x, y, color );

			}



		} else if ( pMap.packType == 4 )

		{



			/* ITS A 32 BIT PIXMAP */



			UnpackBits( pixBuffer, runBuffer, &byteCount );



			chanOff = byteCount/pMap.cmpCount;



			/* See if there is an alpha channel (cmpCount == 4 ) */

			/* Note: Apple stores alpha channels FIRST (ARGB) */

			/* For now, ignore alpha channel by stepping over it */

			if ( pMap.cmpCount == 4 )

				alphaOff = chanOff;

			else

				alphaOff = 0;



			y = pMap.bnd.top + lin;



			for ( byt=0; byt<chanOff; byt+=1 )

			{

				x = pMap.bnd.left + byt;

				/* 32 bit RLE by component chunks */

				color.red   = runBuffer[byt+alphaOff];

				color.green = runBuffer[byt+alphaOff+chanOff];

				color.blue  = runBuffer[byt+alphaOff+chanOff*2];



				/* Don't draw pad-bits (beyond pixmap bounds) */

				if ( x >= pMap.bnd.right )  break;

				SetPixel( x, y, color );

			}



		} else

		{

			sprintf( msg, "Bad packType %d", pMap.packType );

			ImErrorFatal( msg, -1, ImErrNo = IMESYNTAX );

		}



#ifdef DEBUG

		printf( "DEBUG: byteCount = %d, x = %d, y = %d\n", byteCount, x,  y );

#endif



	} /* each scanline */



	g_gs.pnMode = old_mode;   /* Restore the global pen transfer mode */



	return( result );

}





/* 0x0090, 0x0091, 0x0098, 0x0099 - CopyBits */

static int

CopyBits( )

{

	int result=0;		/* return value - number of bytes read  */



	if ( g_pictVers == IMPICT_V1 )

	{

		/* Bitmap - 1 bit QuickDraw */

		result = BitMap( );

	} else	/* g_pictVers == IMPICT_V2 */

	{

	   if ( g_Opcode == 0x009A || g_Opcode == 0x009B ) {

			/* DirectBits - 16 and 32 bit QuickDraw */

			result = DirectBits( );

		} else {

			/* Indexed Color - 2, 4, and 8 bit QuickDraw */

			result = PixMap( );

		}

	}



	return( result );



}







/* 0x0090 - copybits, rect clip - variable */

static int

PO_BitsRect( )

{



	return( CopyBits( ) );



}





/* 0x0091 - copybits, rgn clip - variable */

static int

PO_BitsRgn( )

{

	return( CopyBits( ) );



}





/* 0x0098 - packed, rect clip - variable */

static int

PO_PackBitsRect( )

{



	return( CopyBits( ) );



}





/* 0x0099 - packed, rgn clip - variable */

static int

PO_PackBitsRgn( )

{

	return( CopyBits( ) );

}





/* 0x009A - 32-bit QuickDraw - 2+data */

static int

PO_DirectBitsRect( )

{



	return( CopyBits( ) );



}





/* 0x009B - 32-bit QuickDraw - 2+data */

static int

PO_DirectBitsRgn( )

{



	return( CopyBits( ) );



}



/* 0x009C - reserved for Apple use - 2+data */

static int

PO_reserved56( )

{

	int result = 2;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x009D - reserved for Apple use - 2+data */

static int

PO_reserved57( )

{

	int result = 2;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x009E - reserved for Apple use - 2+data */

static int

PO_reserved58( )

{

	int result = 2;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x009F - reserved for Apple use - 2+data */

static int

PO_reserved59( )

{

	int result = 2;



	OpcodeUns( g_Opcode );



	return( result );



}





/* 0x00A0 - kind (word) - 2 */

static int

PO_ShortComment( )

{

	int result = 2;

	unsigned short ushort1 = 0;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &ushort1, USHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	/* This is too anoying to see the warning over and over */

	/* OpcodeWarn( g_Opcode ); */



	return( result );



}





/* 0x00A1 - kind, size (words), data - 4+data */

static int

PO_LongComment( )

{

	int result = 4;

	unsigned short ushort1 = 0;

	unsigned short ushort2 = 0;



	if ( ImBinRead( g_ioType, g_fd, g_fp, &ushort1, USHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	if ( ImBinRead( g_ioType, g_fd, g_fp, &ushort2, USHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	result += ushort2;

	if ( SkipBytes( (long) ushort2 ) == -1 )

	{

		ImReturnBinError( );

	}

	OpcodeWarn( g_Opcode );



	return( result );

}





/* 0x00FF - end of picture - 2 */

static int

PO_opEndPic( )

{

	int result = 2;



#ifdef DEBUG

fprintf( stderr, "DEBUG: OPCode just read is 'End Of Picture' (0x%0x)\n", g_Opcode );

#endif



	return( result );



}





/* 0x0C00 - Header Opcode - 2 */

static int

PO_HeaderOp( )

{

	int result = 2;



#ifdef DEBUG

fprintf( stderr, "DEBUG: OPCode just read is 'Header Opcode' (0x%0x)\n", g_Opcode );

#endif



	return( result );



}





/*

 *

 *			libim functions

 *

 *

 */



/*

 *  FUNCTION

 *	imPictRead	-  read a Macintosh PICT file

 *

 *  DESCRIPTION

 *	The file header is read and the size of the image determined.

 *	Space is allocated for the VFB and the run codes read in to

 *	a run buffer.  The run buffer is then expanded into straight

 *	RGB values in the VFB and the completed VFB added to the tag list.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__

imPictRead( int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable )

#else

imPictRead( ioType, fd, fp, flags, tagTable )

	int         ioType;		/* I/O flags			*/

	int         fd;			/* Input file descriptor	*/

	FILE        *fp;		/* Input file pointer		*/

	TagTable  * flags;		/* Format flags			*/

	TagTable  * tagTable;		/* Tag list to add to		*/

#endif

{

	int result=0;

	int          x, y, i, j;	/* Convenient short names	*/



	/* PICT STUFF */

	unsigned short pictSize=0;		/* size (in bytes) of image data */

	short OCLen=0;			/* PICT opcode length		*/



	/* bitmap-pixmap stuff */

	int NData=0;			/* Amount of data read for one opcode */



	/* GENERIC VARS */

	unsigned short ushort1;

	unsigned long ulong1;

	char msg[ IMSTR_LEN ];

	char		 message[100];	/* ImInfo message		*/





	/* SET GLOBAL COPIES OF ARGUMENTS */

	g_ioType = ioType;

	g_fd = fd;

	g_fp = fp;





	/*

	 *  Initialize the "Graphics State" structure elements to "zero".

	 */

	memset( (void *)&g_gs, 0x00, sizeof( struct GState ) );

	g_gs.rgbBkColor.red   = IMWHITE;

	g_gs.rgbBkColor.green = IMWHITE;

	g_gs.rgbBkColor.blue  = IMWHITE;

	g_gs.rgbFgColor.red   = IMBLACK;

	g_gs.rgbFgColor.green = IMBLACK;

	g_gs.rgbFgColor.blue  = IMBLACK;

	g_gs.pnSize.h = 1;

	g_gs.pnSize.v = 1;





#ifdef DEBUG

fprintf( stderr, "DEBUG: set our byte order to MBF\n" );

#endif

	/*

	 *  PICT files are usually generated on Macs, which have an MBF

	 *  byte order.  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINMBF );





#ifdef DEBUG

fprintf( stderr, "DEBUG: read and ignore the 512 byte header\n" );

#endif

	/*

	 *  Read in the 512 byte header.

	 *  Its application specific so after we read it we can throw it away.

	 */

	if ( SkipBytes( (long) 512 ) == -1 )

	{

		ImReturnBinError( );

	}





#ifdef DEBUG

fprintf( stderr, "DEBUG: read pictSize\n" );

#endif

	/*

	 *  Read in the 2 byte "pictSize" field.

	 *  Since PICT1 format PICTs can only be 32k the 2 bytes works fine,

	 *  but for PICT2 format PICTs this 2 bytes only represents the low

	 *  order word.   Apple says we can ignore this value.

	 */

	if ( ImBinRead( ioType, fd, fp, &pictSize, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}

#ifdef DEBUG

fprintf( stderr, "DEBUG: pictSize (LSW) = 0x%0x (%d)\n", pictSize, pictSize );

#endif





#ifdef DEBUG

fprintf( stderr, "DEBUG: read g_pictFrame\n" );

#endif

	/*

	 *  Read in the 8 byte "g_pictFrame" field.

	 *  The g_pictFrame describes the bounding rectangle of the image.

	 *  We can use this to tell how big our Virtual Frame Buffer should be.

	 */

	if ( ImBinReadStruct( ioType, fd, fp, &g_pictFrame, RectFields) == -1)

	{

		ImReturnBinError( );

	}

#ifdef DEBUG

fprintf( stderr, "DEBUG: g_pictFrame.top = 0x%0x (%d)\n", g_pictFrame.top, g_pictFrame.top );

fprintf( stderr, "DEBUG: g_pictFrame.left = 0x%0x (%d)\n", g_pictFrame.left, g_pictFrame.left );

fprintf( stderr, "DEBUG: g_pictFrame.bottom = 0x%0x (%d)\n", g_pictFrame.bottom, g_pictFrame.bottom );

fprintf( stderr, "DEBUG: g_pictFrame.right = 0x%0x (%d)\n", g_pictFrame.right, g_pictFrame.right );

#endif





#ifdef DEBUG

fprintf( stderr, "DEBUG: read version\n" );

#endif

	/*

	 *  Read in the 2 bytes of pict version numbers.

	 *  If it is a PICT2 then also skip over the extra 28 bytes of junk.

	 */

	if ( ImBinRead( ioType, fd, fp, &g_pictVers, SHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}

	if ( g_pictVers == IMPICT_V1 )

	{

		/* 1 BYTE OPCODES */

		OCLen = 1;

		strcpy( message, "1 (original PICT)" );

	}

	else if ( g_pictVers == IMPICT_V2 )

	{

		/* 2 BYTE OPCODES */

		OCLen = 2;



		/* PICT2 has some extra "reserved junk" we can skip over */

		if ( SkipBytes( (long) 28 ) == -1 )

		{

			ImReturnBinError( );

		}

		strcpy( message, "2 (32-bit QuickDraw PICT2)" );

	}

	else

	{

		sprintf( msg, "Unknown PICT version 0x%0x (%d)", g_pictVers, g_pictVers);

		ImErrorFatal( msg, -1, ImErrNo = IMESYNTAX );

	}

	ImInfo( "Version", message );

#ifdef DEBUG

if ( g_pictVers == IMPICT_V1 )

   fprintf( stderr, "DEBUG: This is a version 1 PICT file\n" );

if ( g_pictVers == IMPICT_V2 )

   fprintf( stderr, "DEBUG: This is a version 2 PICT file\n" );

#endif



	ImInfo( "Byte Order", "Most Significant Byte First" );



	/*

	 *  We are now ready to start reading opcodes and data,

	 *  so we might as well allocate a VFB of the required size...

	 */

#ifdef DEBUG

fprintf( stderr, "DEBUG: allocate a vfb\n" );

#endif

	x = g_pictFrame.right /* - g_pictFrame.left */ ;

	y = g_pictFrame.bottom /* - g_pictFrame.top */ ;



	sprintf( message, "%d x %d", x, y );

	ImInfo( "Resolution", message );



	if ( (g_vfb = ImVfbAlloc( x, y, IMVFBRGB )) == IMVFBNULL )

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}





	/*

	 *  Fill the VFB with the default background color.

	 */

#ifdef DEBUG

fprintf( stderr, "DEBUG: fill the vfb with background color\n" );

#endif

	for ( i=0; i<x; i++ )

	{

		for ( j=0; j<y; j++ )

		{

			SetPixel( i, j, g_gs.rgbBkColor );

		}

	}



	/*

	 * OK, lets start parsing the PICT opcodes !

	 */

#ifdef DEBUG

fprintf( stderr, "DEBUG: start reading opcodes\n" );

#endif

	g_Opcode = 0;

	if ( ImBinRead( ioType, fd, fp, &g_Opcode, USHORT, OCLen, 1 ) == -1 )

	{

		ImReturnBinError( );

	}



	while ( g_Opcode != 0x00FF )

	{





		/* g_Opcode is >= 0x0000 since it is unsigned. */

		if ( g_Opcode <= IMLAST_INDEXED_OPCODE )

		{

#ifdef DEBUG

fprintf( stderr, "DEBUG: OPCode just read is '%s' (0x%0x)\n", POCHandlerList[g_Opcode].name, g_Opcode );

#endif

			NData += (*POCHandlerList[g_Opcode].handler)( );

		}

		else

		{

#ifdef DEBUG

fprintf( stderr, "DEBUG: OPCode just read = 0x%0x\n", g_Opcode );

#endif

			/* 0x00A2 to 0x00AF - reserved for Apple use - 2+data */

			if ( (g_Opcode>=0x00A2) && (g_Opcode<=0x00AF) )

			{

				NData += 2;

				if ( ImBinRead( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

				{

					ImReturnBinError( );

				}

				if ( ushort1 > 0 )

				{

					NData += ushort1;

					if ( SkipBytes( (long) ushort1 ) == -1 )

					{

						ImReturnBinError( );

					}

				}

				OpcodeWarn( g_Opcode );

			/* 0x00B0 to 0x00CF - reserved for Apple use - 0 */

			}

			else if ( (g_Opcode>=0x00B0) && (g_Opcode<=0x00CF) )

			{

				NData += 0;

				OpcodeWarn( g_Opcode );

			/* 0x00D0 to 0x00FE - reserved for Apple use - 4+data */

			}

			else if ( (g_Opcode>=0x00D0) && (g_Opcode<=0x00FE) )

			{

				NData += 4;

				if ( ImBinRead( ioType, fd, fp, &ulong1, ULONG, 4, 1 )== -1)

				{

					ImReturnBinError( );

				}

				if ( ulong1 > 0 )

				{

					NData += (int) ulong1;

					if ( SkipBytes( (long) ulong1 ) == -1 )

					{

						ImReturnBinError( );

					}

				}

				OpcodeWarn( g_Opcode );

			/* 0x0100 to 0x01FF - reserved for Apple use - 2 */

			}

			else if ( (g_Opcode>=0x0100) && (g_Opcode<=0x01FF) )

			{

				NData += 2;

				if ( ImBinRead( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

				{

					ImReturnBinError( );

				}

				OpcodeWarn( g_Opcode );

			/* 0x0200 to 0x0BFF - reserved for Apple use - 2*nnXX */

			}

			else if ( (g_Opcode>=0x0200) && (g_Opcode<=0x7FFF) )

			{

				NData += (g_Opcode >> 8) * 2;

				if ( SkipBytes( (long) NData ) == -1 )

				{

					ImReturnBinError( );

				}

				OpcodeWarn( g_Opcode );

			/* 0x8100 to 0xFFFF - reserved for Apple use - 2*nnXX */

			}

			else if ( (g_Opcode>=0x8100) && (g_Opcode<=0xFFFF) )

			{

				NData += 4;

				if ( ImBinRead( ioType, fd, fp, &ulong1, ULONG, 4, 1 )== -1)

				{

					ImReturnBinError( );

				}

				if ( ulong1 > 0 )

				{

					NData += (int) ulong1;

					if ( SkipBytes( (long) ulong1 ) == -1 )

					{

						ImReturnBinError( );

					}

				}

				OpcodeWarn( g_Opcode );

			}

			else

			{

				NData = 0;

				OpcodeUns( g_Opcode );

			}

		}



		/*

		 *  If we are reading PICT2, the next opcode must be word

		 *  aligned relative to the last opcode.  So, if we are not

		 *  on an even boundary, read on extra "junk" byte.

		 *  NOTE:  If the amount of data read after an opcode is odd

		 *         then we know we have to read an extra byte.

		 */

		if ( g_pictVers == IMPICT_V2 )

		{

			if ( (NData % 2) == 1 )

			{

				NData += 1;

				if ( ImBinRead( ioType, fd, fp, &i, INT, 1, 1 ) == -1 )

				{

					ImReturnBinError( );

				}

			}

		}

		result += NData;

		NData = 0;



		/*

		 *  read the next opcode

		 */

		if ( ImBinRead( ioType, fd, fp, &g_Opcode, USHORT, OCLen, 1)== -1)

		{

			ImReturnBinError( );

		}



	}



	ImInfo( "Compression Type", "Apple Macintosh PackBits" );



	/*

	 *  Add the CLUT to the tagTable.

	 */

	TagTableAppend( tagTable, TagEntryAlloc( "image clt", POINTER, &clt ) );



	/*

	 *  Add the VFB to the tagTable.

	 */

	TagTableAppend( tagTable, TagEntryAlloc( "image vfb", POINTER, &g_vfb ) );



	return ( result );

}







/*

 *  FUNCTION

 *	imPictWrite	-  write a Macintosh PICT file

 *

 *  DESCRIPTION

 *	The tag list is checked to see that there is a VFB in it, and

 *	that there's only one VFB.  That VFB is then queried, and the

 *	PICT file header set up and written out.  The VFB data is then

 *	read out and converted to run-codes, and those codes written out.

 */





/*

 *                                             

 *  Write a PICT file containing a 1-bit BitMap 

 *                        

 *  Returns # of tags used 

 *

 */

static int

#ifdef __STDC__

imPictWritePB1( ImFileFormatWriteMap *ppMap, int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable )

#else

imPictWritePB1( ppMap, ioType, fd, fp, flags, tagTable )

	ImFileFormatWriteMap *ppMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE         *fp;		/* Input file pointer		*/

	TagTable   * flags;		/* Format flags			*/

	TagTable   * tagTable;		/* Tag list to read from	*/

#endif

{

	ImVfb	   * vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	unsigned char	   * runBuffer;		/* Run buffer			*/

	unsigned char	   * rbp;		/* Run buffer pointer		*/

	ImClt	   * clt;		/* A VFB ColorTable		*/

	ImCltPtr     ctPtr;		/* A VFB ColorTable pointer	*/



	/* PICT STUFF */

	unsigned int	     byteCount;		/* PICT bit-pixMap byteCount	*/

	unsigned short	     rowBytes;		/* PICT bit-pixMap rowBytes	*/

	short	     pictSize;		/* size (in bytes) of image data */

	short	     OCLen;		/* PICT opcode length		*/

	short	     BCLen;		/* PICT bit-pixMap byteCount length */

	struct PMap  pMap;		/* PICT PixMap structure	*/

	struct CTable cTab;		/* PICT ColorLookupTable struct */

	struct CTEntry ctEnt;		/* PICT ColorLookupTable Entry struct*/

	int	     nColors;

	int	     sLines;

	int	     clutBytes;

	int	     bytesPerSLine;

	int	     bytesPerBand;

	int	     sLinesPerBand;

	int	     sLine;

	int	     i = 0;

	int	     col;

	unsigned long	     OddCount;

	Pict2Header  header;		/* PICT 2 file header		*/

	int	     nBands;



	/* GENERIC VARS */

	struct Region rgn;

	unsigned short	     ushort1;

	char		 message[100];	/* ImInfo message		*/





	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	clt = ImVfbQClt( vfb );





	/*

	 *  PICT files are usually generated on Macs, which have an MBF

	 *  byte order.  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINMBF );



#ifdef DEBUG

fprintf( stderr, "DEBUG: write 512 byte header\n" );

#endif

	/*

	 *  Write in the 512 byte (blank) header.

	 */

	memset( (void *)g_buf, 0x00, 512 );

	if ( ImBinWrite( ioType, fd, fp, g_buf, UCHAR, 1, 512 )== -1)

	{

		ImReturnBinError( );

	}





#ifdef DEBUG

fprintf( stderr, "DEBUG: write pictSize\n" );

#endif

	/*

	 *  Write the 2 byte "pictSize" field.

	 *  Since PICT1 format PICTs can only be 32k the 2 bytes works fine,

	 *  but for PICT2 format PICTs this 2 bytes only represents the low

	 *  order word.  As IM V, p92 states, "Many applications already support

	 *  PICT resources larger than 32k. ... This was made possible by having

	 *  QuickDraw ignore the size word and simply read the picture until the

	 *  end-of-picture opcode is reached."

	 */

	pictSize = 0x0000;

	if ( ImBinWrite( ioType, fd, fp, &pictSize, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}





#ifdef DEBUG

fprintf( stderr, "DEBUG: write g_pictFrame\n" );

#endif

	/*

	 *  Write the 8 byte "g_pictFrame" field.

	 *  The g_pictFrame describes the bounding rectangle of the image.

	 *  We can get this by looking at the size of our Virtual Frame Buffer.

	 */

	g_pictFrame.top    = 0;

	g_pictFrame.left   = 0;

	g_pictFrame.bottom = ImVfbQHeight( vfb );

	g_pictFrame.right  = ImVfbQWidth( vfb );

	if ( ImBinWriteStruct( ioType, fd, fp, &g_pictFrame, RectFields)== -1)

	{

		ImReturnBinError( );

	}

#ifdef DEBUG

fprintf( stderr, "DEBUG: g_pictFrame.top = 0x%0x (%d)\n", g_pictFrame.top, g_pictFrame.top );

fprintf( stderr, "DEBUG: g_pictFrame.left = 0x%0x (%d)\n", g_pictFrame.left, g_pictFrame.left );

fprintf( stderr, "DEBUG: g_pictFrame.bottom = 0x%0x (%d)\n", g_pictFrame.bottom, g_pictFrame.bottom );

fprintf( stderr, "DEBUG: g_pictFrame.right = 0x%0x (%d)\n", g_pictFrame.right, g_pictFrame.right );

#endif





#ifdef DEBUG

fprintf( stderr, "DEBUG: write version\n" );

#endif

	/*

	 *  Write the 2 bytes of pict version numbers.

	 *  Note that we always write version 2 PICT files.

	 */

	ushort1 = 0x0011;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = 0x02FF;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



	ImInfo( "Version", "2" );

	ImInfo( "Byte Order", "Most Significant Byte First" );

	sprintf( message, "%d x %d", g_pictFrame.right, g_pictFrame.bottom );

	ImInfo( "Resolution", message );



	/* PICT2 has some extra "reserved junk" we have to fill		*/

	header.headerOp        = 0x0C00;

	header.version         = ~0;

	header.reserved1       = 0;

	header.hRes            = 0x00480000;

	header.vRes            = 0x00480000;

	header.srcRect_top     = 0;

	header.srcRect_left    = 0;

	header.srcRect_bottom  = g_pictFrame.bottom;

	header.srcRect_right   = g_pictFrame.right;

	header.reserved2       = 0;

	if ( ImBinWriteStruct( ioType, fd, fp, &header, Pict2HeaderFields)== -1)

	{

		ImReturnBinError( );

	}





	OCLen = 2;	/* 2 BYTE OPCODES */



	/*

	 *  Most PICT2 files usualy have several opcodes to get bitmaps started:

	 */

#ifdef DEBUG

fprintf( stderr, "DEBUG: write the traditional start-em-off opcodes\n" );

#endif



	/* ShortComment { picDwgBeg };					*/

	ushort1 = 0x00A0;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = IMPC_PICDWGBEG;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}





	/* DefHilite {};						*/

	ushort1 = 0x001E;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}



	/* ClipRgn { 10, { 0, 0, g_pictFrame.right, g_pictFrame.bottom }, $"" }; */

	ushort1 = 0x0001;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	rgn.rgnSize        = 10;

	rgn.rgnBBox.top    = 0;

	rgn.rgnBBox.left   = 0;

	rgn.rgnBBox.bottom = g_pictFrame.bottom;

	rgn.rgnBBox.right  = g_pictFrame.right;

	if ( ImBinWriteStruct( ioType, fd, fp, &rgn, RegionFields)== -1)

	{

		ImReturnBinError( );

	}





	/*

	 *  Allocate space for a run buffer large enough for 1 unencoded

	 *  scanline.  An encoded scanline should be smaller.

	 */

	ImMalloc( runBuffer, unsigned char *, 4 * g_pictFrame.right );





	/*

	 *  Construct all of the "magic" numbers we need to write the image.

	 */



	/* How many colors do we have to work with in the CLUT ?	*/

	nColors = ImCltQNColors( clt );



	/*

	 *  Figure out how many PixMap bands we want to break the image into.

	 *  Note that for each band we have to write out a new copy of the CLUT,

	 *  so we will try to figure on writting the least amount of data,

	 *  with the additional constraint that bands are no larger than 32k.

	 */

	sLines        = g_pictFrame.bottom;

	clutBytes     = nColors * 8;

	bytesPerSLine = g_pictFrame.right + 2;

	bytesPerBand  = 32000 - clutBytes;

	sLinesPerBand = bytesPerBand / bytesPerSLine;

	/* The last band may be small.					*/

	nBands        = (sLines / sLinesPerBand) + 1;



	/* Calculate the rowBytes and ByteCountSize for the pixMap	*/

	rowBytes = ( ( ( 8 * ( g_pictFrame.right ) + 31 ) >> 5 ) << 2 );

	/* if rowBytes > 250 then byteCount is a word, else it is a byte */

	if ( rowBytes > 250 )

		BCLen = 2;

	else

		BCLen = 1;

	/* rowBytes = 0x8000 | rowBytes;  High-bit lit means "PixMap" */





	/* ShortComment { picBitBeg };					*/

	ushort1 = 0x00A0;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = IMPC_PICBITBEG;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



	/* Data counter to keep track of opcode alignment		*/

	OddCount = 0;



	/* Write out the [Pack]PixMap g_Opcode itself			*/

	if ( rowBytes >= 8 )

		ushort1 = 0x0098;

	else

		ushort1 = 0x0090;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 ) == -1 )

	{

		ImReturnBinError( );

	}



	/* Write rowBytes						*/

	rowBytes |= 0x8000;    /* High-bit lit means "PixMap" */

	if ( ImBinWrite( ioType, fd, fp, &rowBytes, USHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	rowBytes &= 0x7FFF;   /* Undo the High-bit so we can use rowBytes "normally" */



	/* Write PixMap header						*/

	pMap.bnd.top    = 0;

	pMap.bnd.left   = 0;

	pMap.bnd.bottom = g_pictFrame.bottom;

	pMap.bnd.right  = g_pictFrame.right;

	pMap.version    = 0;

	pMap.packType   = 0;

	pMap.packSize   = 0x00000000;

	pMap.hRes       = 0x00480000;

	pMap.vRes       = 0x00480000;

	pMap.pixelType  = 0;

	pMap.pixelSize  = 8;

	pMap.cmpCount   = 1;

	pMap.cmpSize    = 8;

	pMap.planeBytes = 0x00000000;

	pMap.pmTable    = 0L;

	pMap.pmReserved = 0x00000000;

	if ( ImBinWriteStruct(ioType, fd, fp, &pMap, PMapFields)== -1)

	{

		ImReturnBinError();

	}



	/* Write out the CLUT (Color Lookup Table) for each PixMap (yuk!) */

	cTab.ctSeed  = 0L;

	cTab.ctFlags = 0x0000;

	cTab.ctSize  = nColors - 1;

	if ( ImBinWriteStruct( ioType, fd, fp, &cTab, CTableFields ) == -1 )

	{

		ImReturnBinError();

	}

	ctPtr = ImCltQFirst( clt );

	for ( i=0; i<nColors; i++ )

	{

		ctEnt.val   = i;

		/* Make the hi and low bytes of the clut values equal */

		ctEnt.red   = (ImCltQRed( ctPtr )   << 8) | ImCltQRed( ctPtr );

		ctEnt.green = (ImCltQGreen( ctPtr ) << 8) | ImCltQGreen( ctPtr);

		ctEnt.blue  = (ImCltQBlue( ctPtr )  << 8) | ImCltQBlue( ctPtr );

		ImCltSInc( clt, ctPtr );

		if ( ImBinWriteStruct( ioType, fd, fp, &ctEnt, CTEntryFields ) == -1 )

		{

			ImReturnBinError();

		}

	}



	ImInfo( "Type", "8-bit Color Indexed" );

	sprintf( message, "%d Entries", nColors );

	ImInfo( "Color Table", message );

	ImInfo( "Compression Type", "Apple MacIntosh Pack Bits" );



	/* Write the srcRect, dstRect, and mode				*/

	if ( ImBinWriteStruct( ioType, fd, fp, &g_pictFrame, RectFields)== -1)

	{

		ImReturnBinError( );

	}

	if ( ImBinWriteStruct( ioType, fd, fp, &g_pictFrame, RectFields)== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = 0;  /* srcCopy */

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



	/* Construct-Compress-Write the scanlines			*/

	for ( sLine=0; sLine<sLines ; sLine++ )

	{

		/* Build an unpacked scanline from the vfb		*/

		pptr = ImVfbQPtr( vfb, 0, sLine );

		for ( col=0; col<g_pictFrame.right; col++ )

		{

			runBuffer[col] = ImVfbQIndex8( vfb, pptr );

			ImVfbSInc( vfb, pptr );

		}



		/* byteCount = g_pictFrame.right;  DEBUG */

		/* byteCount = g_pictFrame.right; */

		byteCount = rowBytes;

		/* Pack the scanline ?					*/

		if ( rowBytes >= 8 )

		{

			PackBits( runBuffer, (unsigned char*) g_buf, &byteCount );

			rbp = (unsigned char*) g_buf;

		}

		else

			rbp = (unsigned char*) runBuffer;



		/* Write the packed scanline				*/

		if ( ImBinWrite( ioType, fd, fp, &byteCount, UINT,

			BCLen, 1 ) == -1 )

		{

			ImReturnBinError( );

		}

		if ( ImBinWrite( ioType, fd, fp, rbp, UCHAR,

			1, byteCount ) == -1 )

		{

			ImReturnBinError( );

		}

		OddCount += (BCLen + byteCount);

	}



	/*

	 *  If we have written an odd amount of data for the opcode,

	 *  then we have to pad an extra byte so that the next opcode

	 *  is word aligned

	 */

	if ( IModd( OddCount ) )

	{

		g_buf[0] = '\0';

		if ( ImBinWrite( ioType, fd, fp, g_buf, UCHAR, 1, 1)== -1)

		{

			ImReturnBinError( );

		}

	}



	/* ShortComment { picBitEnd };					*/

	ushort1 = 0x00A0;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = IMPC_PICBITEND;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}





	/* ShortComment { picDwgEnd };					*/

	ushort1 = 0x00A0;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = IMPC_PICDWGEND;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



#ifdef DEBUG

fprintf( stderr, "DEBUG: write the END opcode\n" );

#endif



	/* PO_opEndPic { };						*/

	ushort1 = 0x00FF;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}



	free( (char *)runBuffer );



	return ( 1 );

}





/*

 *                                                                

 *  Write a PICT file containing an 8-bit CLUT and PixMap          

 *                        

 *  Returns # of tags used 

 *

 */

static int

#ifdef __STDC__

imPictWritePB8( ImFileFormatWriteMap *ppMap, int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable )

#else

imPictWritePB8( ppMap, ioType, fd, fp, flags, tagTable )

	ImFileFormatWriteMap *ppMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE         *fp;		/* Input file pointer		*/

	TagTable   * flags;		/* Format flags			*/

	TagTable   * tagTable;		/* Tag list to read from	*/

#endif

{

	ImVfb	   * vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	unsigned char	   * runBuffer;		/* Run buffer			*/

	unsigned char	   * rbp;		/* Run buffer pointer		*/

	ImClt	   * clt;		/* A VFB ColorTable		*/

	ImCltPtr     ctPtr;		/* A VFB ColorTable pointer	*/



	/* PICT STUFF */

	unsigned int	     byteCount;		/* PICT bit-pixMap byteCount	*/

	unsigned short	     rowBytes;		/* PICT bit-pixMap rowBytes	*/

	short	     pictSize;		/* size (in bytes) of image data */

	short	     OCLen;		/* PICT opcode length		*/

	short	     BCLen;		/* PICT bit-pixMap byteCount length */

	struct PMap  pMap;		/* PICT PixMap structure	*/

	struct CTable cTab;		/* PICT ColorLookupTable struct */

	struct CTEntry ctEnt;		/* PICT ColorLookupTable Entry struct*/

	int	     nColors;

	int	     sLines;

	int	     clutBytes;

	int	     bytesPerSLine;

	int	     bytesPerBand;

	int	     sLinesPerBand;

	int	     sLine;

	int	     i = 0;

	int	     col;

	unsigned long	     OddCount;

	Pict2Header  header;		/* PICT 2 file header		*/

	int	     nBands;



	/* GENERIC VARS */

	struct Region rgn;

	unsigned short	     ushort1;

	char		 message[100];	/* ImInfo message		*/





	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	clt = ImVfbQClt( vfb );





	/*

	 *  PICT files are usually generated on Macs, which have an MBF

	 *  byte order.  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINMBF );



#ifdef DEBUG

fprintf( stderr, "DEBUG: write 512 byte header\n" );

#endif

	/*

	 *  Write in the 512 byte (blank) header.

	 */

	memset( (void *)g_buf, 0x00, 512 );

	if ( ImBinWrite( ioType, fd, fp, g_buf, UCHAR, 1, 512 )== -1)

	{

		ImReturnBinError( );

	}





#ifdef DEBUG

fprintf( stderr, "DEBUG: write pictSize\n" );

#endif

	/*

	 *  Write the 2 byte "pictSize" field.

	 *  Since PICT1 format PICTs can only be 32k the 2 bytes works fine,

	 *  but for PICT2 format PICTs this 2 bytes only represents the low

	 *  order word.  As IM V, p92 states, "Many applications already support

	 *  PICT resources larger than 32k. ... This was made possible by having

	 *  QuickDraw ignore the size word and simply read the picture until the

	 *  end-of-picture opcode is reached."

	 */

	pictSize = 0x0000;

	if ( ImBinWrite( ioType, fd, fp, &pictSize, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}





#ifdef DEBUG

fprintf( stderr, "DEBUG: write g_pictFrame\n" );

#endif

	/*

	 *  Write the 8 byte "g_pictFrame" field.

	 *  The g_pictFrame describes the bounding rectangle of the image.

	 *  We can get this by looking at the size of our Virtual Frame Buffer.

	 */

	g_pictFrame.top    = 0;

	g_pictFrame.left   = 0;

	g_pictFrame.bottom = ImVfbQHeight( vfb );

	g_pictFrame.right  = ImVfbQWidth( vfb );

	if ( ImBinWriteStruct( ioType, fd, fp, &g_pictFrame, RectFields)== -1)

	{

		ImReturnBinError( );

	}

#ifdef DEBUG

fprintf( stderr, "DEBUG: g_pictFrame.top = 0x%0x (%d)\n", g_pictFrame.top, g_pictFrame.top );

fprintf( stderr, "DEBUG: g_pictFrame.left = 0x%0x (%d)\n", g_pictFrame.left, g_pictFrame.left );

fprintf( stderr, "DEBUG: g_pictFrame.bottom = 0x%0x (%d)\n", g_pictFrame.bottom, g_pictFrame.bottom );

fprintf( stderr, "DEBUG: g_pictFrame.right = 0x%0x (%d)\n", g_pictFrame.right, g_pictFrame.right );

#endif





#ifdef DEBUG

fprintf( stderr, "DEBUG: write version\n" );

#endif

	/*

	 *  Write the 2 bytes of pict version numbers.

	 *  Note that we always write version 2 PICT files.

	 */

	ushort1 = 0x0011;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = 0x02FF;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



	ImInfo( "Version", "2" );

	ImInfo( "Byte Order", "Most Significant Byte First" );

	sprintf( message, "%d x %d", g_pictFrame.right, g_pictFrame.bottom );

	ImInfo( "Resolution", message );



	/* PICT2 has some extra "reserved junk" we have to fill		*/

	header.headerOp        = 0x0C00;

	header.version         = ~0;

	header.reserved1       = 0;

	header.hRes            = 0x00480000;

	header.vRes            = 0x00480000;

	header.srcRect_top     = 0;

	header.srcRect_left    = 0;

	header.srcRect_bottom  = g_pictFrame.bottom;

	header.srcRect_right   = g_pictFrame.right;

	header.reserved2       = 0;

	if ( ImBinWriteStruct( ioType, fd, fp, &header, Pict2HeaderFields)== -1)

	{

		ImReturnBinError( );

	}





	OCLen = 2;	/* 2 BYTE OPCODES */



	/*

	 *  Most PICT2 files usualy have several opcodes to get bitmaps started:

	 */

#ifdef DEBUG

fprintf( stderr, "DEBUG: write the traditional start-em-off opcodes\n" );

#endif



	/* ShortComment { picDwgBeg };					*/

	ushort1 = 0x00A0;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = IMPC_PICDWGBEG;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}





	/* DefHilite {};						*/

	ushort1 = 0x001E;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}



	/* ClipRgn { 10, { 0, 0, g_pictFrame.right, g_pictFrame.bottom }, $"" }; */

	ushort1 = 0x0001;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	rgn.rgnSize        = 10;

	rgn.rgnBBox.top    = 0;

	rgn.rgnBBox.left   = 0;

	rgn.rgnBBox.bottom = g_pictFrame.bottom;

	rgn.rgnBBox.right  = g_pictFrame.right;

	if ( ImBinWriteStruct( ioType, fd, fp, &rgn, RegionFields)== -1)

	{

		ImReturnBinError( );

	}





	/*

	 *  Allocate space for a run buffer large enough for 1 unencoded

	 *  scanline.  An encoded scanline should be smaller.

	 */

	ImMalloc( runBuffer, unsigned char *, 4 * g_pictFrame.right );





	/*

	 *  Construct all of the "magic" numbers we need to write the image.

	 */



	/* How many colors do we have to work with in the CLUT ?	*/

	nColors = ImCltQNColors( clt );



	/*

	 *  Figure out how many PixMap bands we want to break the image into.

	 *  Note that for each band we have to write out a new copy of the CLUT,

	 *  so we will try to figure on writting the least amount of data,

	 *  with the additional constraint that bands are no larger than 32k.

	 */

	sLines        = g_pictFrame.bottom;

	clutBytes     = nColors * 8;

	bytesPerSLine = g_pictFrame.right + 2;

	bytesPerBand  = 32000 - clutBytes;

	sLinesPerBand = bytesPerBand / bytesPerSLine;

	/* The last band may be small.					*/

	nBands        = (sLines / sLinesPerBand) + 1;



	/* Calculate the rowBytes and ByteCountSize for the pixMap	*/

	rowBytes = ( ( ( 8 * ( g_pictFrame.right ) + 31 ) >> 5 ) << 2 );

	/* if rowBytes > 250 then byteCount is a word, else it is a byte */

	if ( rowBytes > 250 )

		BCLen = 2;

	else

		BCLen = 1;

	/* rowBytes = 0x8000 | rowBytes;  High-bit lit means "PixMap" */





	/* ShortComment { picBitBeg };					*/

	ushort1 = 0x00A0;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = IMPC_PICBITBEG;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



	/* Data counter to keep track of opcode alignment		*/

	OddCount = 0;



	/* Write out the [Pack]PixMap g_Opcode itself			*/

	if ( rowBytes >= 8 )

		ushort1 = 0x0098;

	else

		ushort1 = 0x0090;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 ) == -1 )

	{

		ImReturnBinError( );

	}



	/* Write rowBytes						*/

	rowBytes |= 0x8000;    /* High-bit lit means "PixMap" */

	if ( ImBinWrite( ioType, fd, fp, &rowBytes, USHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	rowBytes &= 0x7FFF;   /* Undo the High-bit so we can use rowBytes "normally" */



	/* Write PixMap header						*/

	pMap.bnd.top    = 0;

	pMap.bnd.left   = 0;

	pMap.bnd.bottom = g_pictFrame.bottom;

	pMap.bnd.right  = g_pictFrame.right;

	pMap.version    = 0;

	pMap.packType   = 0;

	pMap.packSize   = 0x00000000;

	pMap.hRes       = 0x00480000;

	pMap.vRes       = 0x00480000;

	pMap.pixelType  = 0;

	pMap.pixelSize  = 8;

	pMap.cmpCount   = 1;

	pMap.cmpSize    = 8;

	pMap.planeBytes = 0x00000000;

	pMap.pmTable    = 0L;

	pMap.pmReserved = 0x00000000;

	if ( ImBinWriteStruct(ioType, fd, fp, &pMap, PMapFields)== -1)

	{

		ImReturnBinError();

	}



	/* Write out the CLUT (Color Lookup Table) for each PixMap (yuk!) */

	cTab.ctSeed  = 0L;

	cTab.ctFlags = 0x0000;

	cTab.ctSize  = nColors - 1;

	if ( ImBinWriteStruct( ioType, fd, fp, &cTab, CTableFields ) == -1 )

	{

		ImReturnBinError();

	}

	ctPtr = ImCltQFirst( clt );

	for ( i=0; i<nColors; i++ )

	{

		ctEnt.val   = i;

		/* Make the hi and low bytes of the clut values equal */

		ctEnt.red   = (ImCltQRed( ctPtr )   << 8) | ImCltQRed( ctPtr );

		ctEnt.green = (ImCltQGreen( ctPtr ) << 8) | ImCltQGreen( ctPtr);

		ctEnt.blue  = (ImCltQBlue( ctPtr )  << 8) | ImCltQBlue( ctPtr );

		ImCltSInc( clt, ctPtr );

		if ( ImBinWriteStruct( ioType, fd, fp, &ctEnt, CTEntryFields ) == -1 )

		{

			ImReturnBinError();

		}

	}



	ImInfo( "Type", "8-bit Color Indexed" );

	sprintf( message, "%d Entries", nColors );

	ImInfo( "Color Table", message );

	ImInfo( "Compression Type", "Apple MacIntosh Pack Bits" );



	/* Write the srcRect, dstRect, and mode				*/

	if ( ImBinWriteStruct( ioType, fd, fp, &g_pictFrame, RectFields)== -1)

	{

		ImReturnBinError( );

	}

	if ( ImBinWriteStruct( ioType, fd, fp, &g_pictFrame, RectFields)== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = 0;  /* srcCopy */

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



	/* Construct-Compress-Write the scanlines			*/

	for ( sLine=0; sLine<sLines ; sLine++ )

	{

		/* Build an unpacked scanline from the vfb		*/

		pptr = ImVfbQPtr( vfb, 0, sLine );

		for ( col=0; col<g_pictFrame.right; col++ )

		{

			runBuffer[col] = ImVfbQIndex8( vfb, pptr );

			ImVfbSInc( vfb, pptr );

		}



		/* byteCount = g_pictFrame.right;  DEBUG */

		/* byteCount = g_pictFrame.right; */

		byteCount = rowBytes;

		/* Pack the scanline ?					*/

		if ( rowBytes >= 8 )

		{

			PackBits( runBuffer, (unsigned char*) g_buf, &byteCount );

			rbp = (unsigned char*) g_buf;

		}

		else

			rbp = (unsigned char*) runBuffer;



		/* Write the packed scanline				*/

		if ( ImBinWrite( ioType, fd, fp, &byteCount, UINT,

			BCLen, 1 ) == -1 )

		{

			ImReturnBinError( );

		}

		if ( ImBinWrite( ioType, fd, fp, rbp, UCHAR,

			1, byteCount ) == -1 )

		{

			ImReturnBinError( );

		}

		OddCount += (BCLen + byteCount);

	}



	/*

	 *  If we have written an odd amount of data for the opcode,

	 *  then we have to pad an extra byte so that the next opcode

	 *  is word aligned

	 */

	if ( IModd( OddCount ) )

	{

		g_buf[0] = '\0';

		if ( ImBinWrite( ioType, fd, fp, g_buf, UCHAR, 1, 1)== -1)

		{

			ImReturnBinError( );

		}

	}



	/* ShortComment { picBitEnd };					*/

	ushort1 = 0x00A0;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = IMPC_PICBITEND;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}





	/* ShortComment { picDwgEnd };					*/

	ushort1 = 0x00A0;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = IMPC_PICDWGEND;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



#ifdef DEBUG

fprintf( stderr, "DEBUG: write the END opcode\n" );

#endif



	/* PO_opEndPic { };						*/

	ushort1 = 0x00FF;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}



	free( (char *)runBuffer );



	return ( 1 );

}





/*

 *                                             

 *  Write a PICT file containing a 16-bit PixMap

 *                       

 *  Returns # of tags used

 *

 */

static int

#ifdef __STDC__

imPictWritePB16( ImFileFormatWriteMap *ppMap, int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable )

#else

imPictWritePB16( ppMap, ioType, fd, fp, flags, tagTable )

	ImFileFormatWriteMap *ppMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE         *fp;		/* Input file pointer		*/

	TagTable   * flags;		/* Format flags			*/

	TagTable   * tagTable;		/* Tag list to read from	*/

#endif

{

	ImVfb	   * vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	unsigned char	   * runBuffer;		/* Run buffer			*/

	unsigned char	   * rbp;		/* Run buffer pointer		*/

	ImClt	   * clt;		/* A VFB ColorTable		*/

	ImCltPtr     ctPtr;		/* A VFB ColorTable pointer	*/



	/* PICT STUFF */

	unsigned int	     byteCount;		/* PICT bit-pixMap byteCount	*/

	unsigned short	     rowBytes;		/* PICT bit-pixMap rowBytes	*/

	short	     pictSize;		/* size (in bytes) of image data */

	short	     OCLen;		/* PICT opcode length		*/

	short	     BCLen;		/* PICT bit-pixMap byteCount length */

	struct PMap  pMap;		/* PICT PixMap structure	*/

	struct CTable cTab;		/* PICT ColorLookupTable struct */

	struct CTEntry ctEnt;		/* PICT ColorLookupTable Entry struct*/

	int	     nColors;

	int	     sLines;

	int	     clutBytes;

	int	     bytesPerSLine;

	int	     bytesPerBand;

	int	     sLinesPerBand;

	int	     sLine;

	int	     i = 0;

	int	     col;

	unsigned long	     OddCount;

	Pict2Header  header;		/* PICT 2 file header		*/

	int	     nBands;



	/* GENERIC VARS */

	struct Region rgn;

	unsigned short	     ushort1;

	char		 message[100];	/* ImInfo message		*/





	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	clt = ImVfbQClt( vfb );





	/*

	 *  PICT files are usually generated on Macs, which have an MBF

	 *  byte order.  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINMBF );



#ifdef DEBUG

fprintf( stderr, "DEBUG: write 512 byte header\n" );

#endif

	/*

	 *  Write in the 512 byte (blank) header.

	 */

	memset( (void *)g_buf, 0x00,512 );

	if ( ImBinWrite( ioType, fd, fp, g_buf, UCHAR, 1, 512 )== -1)

	{

		ImReturnBinError( );

	}





#ifdef DEBUG

fprintf( stderr, "DEBUG: write pictSize\n" );

#endif

	/*

	 *  Write the 2 byte "pictSize" field.

	 *  Since PICT1 format PICTs can only be 32k the 2 bytes works fine,

	 *  but for PICT2 format PICTs this 2 bytes only represents the low

	 *  order word.  As IM V, p92 states, "Many applications already support

	 *  PICT resources larger than 32k. ... This was made possible by having

	 *  QuickDraw ignore the size word and simply read the picture until the

	 *  end-of-picture opcode is reached."

	 */

	pictSize = 0x0000;

	if ( ImBinWrite( ioType, fd, fp, &pictSize, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}





#ifdef DEBUG

fprintf( stderr, "DEBUG: write g_pictFrame\n" );

#endif

	/*

	 *  Write the 8 byte "g_pictFrame" field.

	 *  The g_pictFrame describes the bounding rectangle of the image.

	 *  We can get this by looking at the size of our Virtual Frame Buffer.

	 */

	g_pictFrame.top    = 0;

	g_pictFrame.left   = 0;

	g_pictFrame.bottom = ImVfbQHeight( vfb );

	g_pictFrame.right  = ImVfbQWidth( vfb );

	if ( ImBinWriteStruct( ioType, fd, fp, &g_pictFrame, RectFields)== -1)

	{

		ImReturnBinError( );

	}

#ifdef DEBUG

fprintf( stderr, "DEBUG: g_pictFrame.top = 0x%0x (%d)\n", g_pictFrame.top, g_pictFrame.top );

fprintf( stderr, "DEBUG: g_pictFrame.left = 0x%0x (%d)\n", g_pictFrame.left, g_pictFrame.left );

fprintf( stderr, "DEBUG: g_pictFrame.bottom = 0x%0x (%d)\n", g_pictFrame.bottom, g_pictFrame.bottom );

fprintf( stderr, "DEBUG: g_pictFrame.right = 0x%0x (%d)\n", g_pictFrame.right, g_pictFrame.right );

#endif





#ifdef DEBUG

fprintf( stderr, "DEBUG: write version\n" );

#endif

	/*

	 *  Write the 2 bytes of pict version numbers.

	 *  Note that we always write version 2 PICT files.

	 */

	ushort1 = 0x0011;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = 0x02FF;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



	ImInfo( "Version", "2" );

	ImInfo( "Byte Order", "Most Significant Byte First" );

	sprintf( message, "%d x %d", g_pictFrame.right, g_pictFrame.bottom );

	ImInfo( "Resolution", message );



	/* PICT2 has some extra "reserved junk" we have to fill		*/

	header.headerOp        = 0x0C00;

	header.version         = ~0;

	header.reserved1       = 0;

	header.hRes            = 0x00480000;

	header.vRes            = 0x00480000;

	header.srcRect_top     = 0;

	header.srcRect_left    = 0;

	header.srcRect_bottom  = g_pictFrame.bottom;

	header.srcRect_right   = g_pictFrame.right;

	header.reserved2       = 0;

	if ( ImBinWriteStruct( ioType, fd, fp, &header, Pict2HeaderFields)== -1)

	{

		ImReturnBinError( );

	}





	OCLen = 2;	/* 2 BYTE OPCODES */



	/*

	 *  Most PICT2 files usualy have several opcodes to get bitmaps started:

	 */

#ifdef DEBUG

fprintf( stderr, "DEBUG: write the traditional start-em-off opcodes\n" );

#endif



	/* ShortComment { picDwgBeg };					*/

	ushort1 = 0x00A0;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = IMPC_PICDWGBEG;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}





	/* DefHilite {};						*/

	ushort1 = 0x001E;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}



	/* ClipRgn { 10, { 0, 0, g_pictFrame.right, g_pictFrame.bottom }, $"" }; */

	ushort1 = 0x0001;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	rgn.rgnSize        = 10;

	rgn.rgnBBox.top    = 0;

	rgn.rgnBBox.left   = 0;

	rgn.rgnBBox.bottom = g_pictFrame.bottom;

	rgn.rgnBBox.right  = g_pictFrame.right;

	if ( ImBinWriteStruct( ioType, fd, fp, &rgn, RegionFields)== -1)

	{

		ImReturnBinError( );

	}





	/*

	 *  Allocate space for a run buffer large enough for 1 unencoded

	 *  scanline.  An encoded scanline should be smaller.

	 */

	ImMalloc( runBuffer, unsigned char *, 4 * g_pictFrame.right );





	/*

	 *  Construct all of the "magic" numbers we need to write the image.

	 */



	/* How many colors do we have to work with in the CLUT ?	*/

	nColors = ImCltQNColors( clt );



	/*

	 *  Figure out how many PixMap bands we want to break the image into.

	 *  Note that for each band we have to write out a new copy of the CLUT,

	 *  so we will try to figure on writting the least amount of data,

	 *  with the additional constraint that bands are no larger than 32k.

	 */

	sLines        = g_pictFrame.bottom;

	clutBytes     = nColors * 8;

	bytesPerSLine = g_pictFrame.right + 2;

	bytesPerBand  = 32000 - clutBytes;

	sLinesPerBand = bytesPerBand / bytesPerSLine;

	/* The last band may be small.					*/

	nBands        = (sLines / sLinesPerBand) + 1;



	/* Calculate the rowBytes and ByteCountSize for the pixMap	*/

	rowBytes = ( ( ( 8 * ( g_pictFrame.right ) + 31 ) >> 5 ) << 2 );

	/* if rowBytes > 250 then byteCount is a word, else it is a byte */

	if ( rowBytes > 250 )

		BCLen = 2;

	else

		BCLen = 1;

	/* rowBytes = 0x8000 | rowBytes;  High-bit lit means "PixMap" */





	/* ShortComment { picBitBeg };					*/

	ushort1 = 0x00A0;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = IMPC_PICBITBEG;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



	/* Data counter to keep track of opcode alignment		*/

	OddCount = 0;



	/* Write out the [Pack]PixMap g_Opcode itself			*/

	if ( rowBytes >= 8 )

		ushort1 = 0x0098;

	else

		ushort1 = 0x0090;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 ) == -1 )

	{

		ImReturnBinError( );

	}



	/* Write rowBytes						*/

	rowBytes |= 0x8000;    /* High-bit lit means "PixMap" */

	if ( ImBinWrite( ioType, fd, fp, &rowBytes, USHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	rowBytes &= 0x7FFF;   /* Undo the High-bit so we can use rowBytes "normally" */



	/* Write PixMap header						*/

	pMap.bnd.top    = 0;

	pMap.bnd.left   = 0;

	pMap.bnd.bottom = g_pictFrame.bottom;

	pMap.bnd.right  = g_pictFrame.right;

	pMap.version    = 0;

	pMap.packType   = 0;

	pMap.packSize   = 0x00000000;

	pMap.hRes       = 0x00480000;

	pMap.vRes       = 0x00480000;

	pMap.pixelType  = 0;

	pMap.pixelSize  = 8;

	pMap.cmpCount   = 1;

	pMap.cmpSize    = 8;

	pMap.planeBytes = 0x00000000;

	pMap.pmTable    = 0L;

	pMap.pmReserved = 0x00000000;

	if ( ImBinWriteStruct(ioType, fd, fp, &pMap, PMapFields)== -1)

	{

		ImReturnBinError();

	}



	/* Write out the CLUT (Color Lookup Table) for each PixMap (yuk!) */

	cTab.ctSeed  = 0L;

	cTab.ctFlags = 0x0000;

	cTab.ctSize  = nColors - 1;

	if ( ImBinWriteStruct( ioType, fd, fp, &cTab, CTableFields ) == -1 )

	{

		ImReturnBinError();

	}

	ctPtr = ImCltQFirst( clt );

	for ( i=0; i<nColors; i++ )

	{

		ctEnt.val   = i;

		/* Make the hi and low bytes of the clut values equal */

		ctEnt.red   = (ImCltQRed( ctPtr )   << 8) | ImCltQRed( ctPtr );

		ctEnt.green = (ImCltQGreen( ctPtr ) << 8) | ImCltQGreen( ctPtr);

		ctEnt.blue  = (ImCltQBlue( ctPtr )  << 8) | ImCltQBlue( ctPtr );

		ImCltSInc( clt, ctPtr );

		if ( ImBinWriteStruct( ioType, fd, fp, &ctEnt, CTEntryFields ) == -1 )

		{

			ImReturnBinError();

		}

	}



	ImInfo( "Type", "8-bit Color Indexed" );

	sprintf( message, "%d Entries", nColors );

	ImInfo( "Color Table", message );

	ImInfo( "Compression Type", "Apple MacIntosh Pack Bits" );



	/* Write the srcRect, dstRect, and mode				*/

	if ( ImBinWriteStruct( ioType, fd, fp, &g_pictFrame, RectFields)== -1)

	{

		ImReturnBinError( );

	}

	if ( ImBinWriteStruct( ioType, fd, fp, &g_pictFrame, RectFields)== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = 0;  /* srcCopy */

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



	/* Construct-Compress-Write the scanlines			*/

	for ( sLine=0; sLine<sLines ; sLine++ )

	{

		/* Build an unpacked scanline from the vfb		*/

		pptr = ImVfbQPtr( vfb, 0, sLine );

		for ( col=0; col<g_pictFrame.right; col++ )

		{

			runBuffer[col] = ImVfbQIndex8( vfb, pptr );

			ImVfbSInc( vfb, pptr );

		}



		/* byteCount = g_pictFrame.right;  DEBUG */

		/* byteCount = g_pictFrame.right; */

		byteCount = rowBytes;

		/* Pack the scanline ?					*/

		if ( rowBytes >= 8 )

		{

			PackBits( runBuffer, (unsigned char*) g_buf, &byteCount );

			rbp = (unsigned char*) g_buf;

		}

		else

			rbp = (unsigned char*) runBuffer;



		/* Write the packed scanline				*/

		if ( ImBinWrite( ioType, fd, fp, &byteCount, UINT,

			BCLen, 1 ) == -1 )

		{

			ImReturnBinError( );

		}

		if ( ImBinWrite( ioType, fd, fp, rbp, UCHAR,

			1, byteCount ) == -1 )

		{

			ImReturnBinError( );

		}

		OddCount += (BCLen + byteCount);

	}



	/*

	 *  If we have written an odd amount of data for the opcode,

	 *  then we have to pad an extra byte so that the next opcode

	 *  is word aligned

	 */

	if ( IModd( OddCount ) )

	{

		g_buf[0] = '\0';

		if ( ImBinWrite( ioType, fd, fp, g_buf, UCHAR, 1, 1)== -1)

		{

			ImReturnBinError( );

		}

	}



	/* ShortComment { picBitEnd };					*/

	ushort1 = 0x00A0;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = IMPC_PICBITEND;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}





	/* ShortComment { picDwgEnd };					*/

	ushort1 = 0x00A0;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = IMPC_PICDWGEND;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



#ifdef DEBUG

fprintf( stderr, "DEBUG: write the END opcode\n" );

#endif



	/* PO_opEndPic { };						*/

	ushort1 = 0x00FF;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}



	free( (char *)runBuffer );



	return ( 1 );

}





/*

 *                                                

 *  Write a PICT file containing a 32-bit PixMap   

 *                                                  

 *  Returns # of tags used                           

 *

 */

static int

#ifdef __STDC__

imPictWritePBRGB( ImFileFormatWriteMap *ppMap, int ioType, int fd, FILE *fp, TagTable *flags, TagTable *tagTable )

#else

imPictWritePBRGB( ppMap, ioType, fd, fp, flags, tagTable )

	ImFileFormatWriteMap *ppMap;	/* Write map entry to adhear to */

	int          ioType;		/* I/O flags */

	int          fd;		/* Input file descriptor */

	FILE         *fp;		/* Input file pointer */

	TagTable   * flags;		/* Format flags */

	TagTable   * tagTable;		/* Tag list to read from */

#endif

{

	ImVfb	   * vfb;		/* Read in image */

	ImVfbPtr     pptr;		/* Pixel pointer */

	unsigned char	   * runBuffer;		/* Run buffer */

	unsigned char	   * rbp;		/* Run buffer pointer */



	/* PICT STUFF */

	unsigned int		byteCount;	/* PICT bit-pixMap byteCount */

	unsigned short		rowBytes;	/* PICT bit-pixMap rowBytes */

	short		pictSize;	/* size (bytes) of image data */

	short		OCLen;		/* PICT opcode length */

	short		BCLen;		/* PICT bit-pixMap byteCount length */

	struct PMap	pMap;		/* PICT PixMap structure */

	int		sLines;

	int		sLine;

	int		i = 0;

	int		col;

	unsigned long		OddCount = 0;	/* Keeps track of opcode alignment */

	Pict2Header	header;		/* PICT 2 file header */

	int		depth;



	/* GENERIC VARS */

	struct Region	rgn;

	unsigned short		ushort1;

	unsigned long		ulong1;

	char		message[100];	/* ImInfo message */





	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );





	/*

	 *  PICT files are usually generated on Macs, which have an MBF

	 *  byte order.  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINMBF );



#ifdef DEBUG

	fprintf( stderr, "DEBUG: write 512 byte header\n" );

#endif

	/*

	 *  Write in the 512 byte (blank) header.

	 */

	memset( (void *)g_buf, 0x00, 512 );

	if ( ImBinWrite( ioType, fd, fp, g_buf, UCHAR, 1, 512 )== -1)

	{

		ImReturnBinError( );

	}





#ifdef DEBUG

	fprintf( stderr, "DEBUG: write pictSize\n" );

#endif

	/*

	 *  Write the 2 byte "pictSize" field.

	 *  Since PICT1 format PICTs can only be 32k the 2 bytes works fine,

	 *  but for PICT2 format PICTs this 2 bytes only represents the low

	 *  order word.  As IM V, p92 states, "Many applications already support

	 *  PICT resources larger than 32k. ... This was made possible by having

	 *  QuickDraw ignore the size word and simply read the picture until the

	 *  end-of-picture opcode is reached."

	 */

	pictSize = 0x0000;

	if ( ImBinWrite( ioType, fd, fp, &pictSize, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}





#ifdef DEBUG

	fprintf( stderr, "DEBUG: write g_pictFrame\n" );

#endif

	/*

	 *  Write the 8 byte "g_pictFrame" field.

	 *  The g_pictFrame describes the bounding rectangle of the image.

	 *  We can get this by looking at the size of our Virtual Frame Buffer.

	 */

	g_pictFrame.top    = 0;

	g_pictFrame.left   = 0;

	g_pictFrame.bottom = ImVfbQHeight( vfb );

	g_pictFrame.right  = ImVfbQWidth( vfb );

	if ( ImBinWriteStruct( ioType, fd, fp, &g_pictFrame, RectFields)== -1)

	{

		ImReturnBinError( );

	}

#ifdef DEBUG

	fprintf( stderr, "DEBUG: g_pictFrame.top = 0x%0x (%d)\n", g_pictFrame.top, g_pictFrame.top );

	fprintf( stderr, "DEBUG: g_pictFrame.left = 0x%0x (%d)\n", g_pictFrame.left, g_pictFrame.left );

	fprintf( stderr, "DEBUG: g_pictFrame.bottom = 0x%0x (%d)\n", g_pictFrame.bottom, g_pictFrame.bottom );

	fprintf( stderr, "DEBUG: g_pictFrame.right = 0x%0x (%d)\n", g_pictFrame.right, g_pictFrame.right );

#endif





#ifdef DEBUG

	fprintf( stderr, "DEBUG: write version\n" );

#endif

	/*

	 *  Write the 2 bytes of pict version numbers.

	 *  Note that we always write version 2 PICT files.

	 */

	ushort1 = 0x0011;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = 0x02FF;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



	ImInfo( "Version", "2" );

	ImInfo( "Byte Order", "Most Significant Byte First" );

	sprintf( message, "%d x %d", g_pictFrame.right, g_pictFrame.bottom );

	ImInfo( "Resolution", message );



	/* PICT2 has some extra "reserved junk" we have to fill		*/

	header.headerOp        = 0x0C00;

	header.version         = 0xfffe;   /* = -2 Extended version 2 PICT */

	header.reserved1       = 0;

	header.hRes            = 0x00480000;

	header.vRes            = 0x00480000;

	header.srcRect_top     = 0;

	header.srcRect_left    = 0;

	header.srcRect_bottom  = g_pictFrame.bottom;

	header.srcRect_right   = g_pictFrame.right;

	header.reserved2       = 0;

	if ( ImBinWriteStruct( ioType, fd, fp, &header, Pict2HeaderFields)== -1)

	{

		ImReturnBinError( );

	}





	OCLen = 2;	/* 2 BYTE OPCODES */



	/*

	 *  Most PICT2 files usualy have several opcodes to get bitmaps started:

	 */

#ifdef DEBUG

	fprintf( stderr, "DEBUG: write the traditional start opcodes\n" );

#endif



	/* ShortComment { picDwgBeg };					*/

	ushort1 = 0x00A0;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = IMPC_PICDWGBEG;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}





	/* DefHilite {}; */

	ushort1 = 0x001E;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}



	/* ClipRgn { 10, { 0, 0, g_pictFrame.right, g_pictFrame.bottom } } */

	ushort1 = 0x0001;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	rgn.rgnSize        = 10;

	rgn.rgnBBox.top    = 0;

	rgn.rgnBBox.left   = 0;

	rgn.rgnBBox.bottom = g_pictFrame.bottom;

	rgn.rgnBBox.right  = g_pictFrame.right;

	if ( ImBinWriteStruct( ioType, fd, fp, &rgn, RegionFields)== -1)

	{

		ImReturnBinError( );

	}





	/*

	 *  Allocate space for a run buffer large enough for 1 unencoded

	 *  scanline.  An encoded scanline should be smaller.

	 */

	ImMalloc( runBuffer, unsigned char *, 4 * g_pictFrame.right );





	/*

	 *  Construct all of the "magic" numbers we need to write the image.

	 */



	sLines = g_pictFrame.bottom;



	/*

	 * Calculate the rowBytes and ByteCountSize for the pixMap.

	 * This formula multiplies the number of pixels across the PixMap by

	 * the pixel depth to get the number of bits, and then this is divided

	 * by eight to get the number of bytes. This division by eight looks

	 * very strange because the number of bytes per row must be even, so

	 * this formula takes advantage of integer division and multiplication

	 * to make the result come out even. This particular formula

	 * additionally makes sure that the number of bytes per row is a

	 * multiple of four. This helps optimize the performance of Color

	 * QuickDraw operations because it allows Color QuickDraw to refer

	 * to each row beginning on a long word boundary in memory.

	 */



	/* OLD WAY OF CALCULATING ROWBYTES (ONLY WORD ALIGNED) */

	/* rowBytes = 3 * g_pictFrame.right; */

	/* if ( IModd( rowBytes ) ) rowBytes++; */



	depth = 32;

	rowBytes = ((depth * (g_pictFrame.right - g_pictFrame.left) + 31) / 32) * 4;



	/* if rowBytes > 250 then byteCount is a word, else it is a byte */

	if ( rowBytes > 250 )

		BCLen = 2;

	else

		BCLen = 1;



	/* ShortComment { picBitBeg } */

	ushort1 = 0x00A0;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = IMPC_PICBITBEG;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}





	/* Write out the DirectBitsRect opcode itself */

	ushort1 = 0x009A;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 ) == -1 )

	{

		ImReturnBinError( );

	}



	/* Write out the baseAddr flag */

	ulong1 = 0x000000FF;    /* Means direct pixels */

	if ( ImBinWrite( ioType, fd, fp, &ulong1, ULONG, 4, 1 ) == -1 )

	{

		ImReturnBinError( );

	}





	/* Write rowBytes */

	rowBytes |= 0x8000;    /* High-bit lit means "PixMap" */

	if ( ImBinWrite( ioType, fd, fp, &rowBytes, USHORT, 2, 1 ) == -1 )

	{

		ImReturnBinError( );

	}

	rowBytes &= 0x7FFF;   /* Undo High-bit to use rowBytes "normally" */





	/* Write PixMap header */

	pMap.bnd.top    = 0;

	pMap.bnd.left   = 0;

	pMap.bnd.bottom = g_pictFrame.bottom;

	pMap.bnd.right  = g_pictFrame.right;

	pMap.version    = 0;

	pMap.packType   = 4;           /* RGB RLE encoding */

	pMap.packSize   = 0x00000000;

	pMap.hRes       = 0x00480000;  /* 72 dpi */

	pMap.vRes       = 0x00480000;  /* 72 dpi */

	pMap.pixelType  = 16;          /* For Direct Pixels */

	pMap.pixelSize  = 32;          /* RGB data */

	pMap.cmpCount   = 3;           /* Red, Green, Blue */

	pMap.cmpSize    = 8;

	pMap.planeBytes = 0x00000000;

	pMap.pmTable    = 0L;

	pMap.pmReserved = 0x00000000;

	if ( ImBinWriteStruct(ioType, fd, fp, &pMap, PMapFields)== -1)

	{

		ImReturnBinError();

	}



	/* Write the srcRect, dstRect, and mode */

	if ( ImBinWriteStruct( ioType, fd, fp, &g_pictFrame, RectFields)== -1)

	{

		ImReturnBinError( );

	}

	if ( ImBinWriteStruct( ioType, fd, fp, &g_pictFrame, RectFields)== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = 64;  /* ditherCopy */

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}





	/* Construct-Compress-Write the scanlines */

	for ( sLine=0; sLine<sLines ; sLine++ )

	{

		/* Build an unpacked scanline from the vfb */

		for ( col=0; col<g_pictFrame.right; col++ )

		{

			pptr = ImVfbQPtr( vfb, col, sLine );

			runBuffer[ col + 0 * g_pictFrame.right ] = ImVfbQRed( vfb, pptr );

			runBuffer[ col + 1 * g_pictFrame.right ] = ImVfbQGreen( vfb, pptr );

			runBuffer[ col + 2 * g_pictFrame.right ] = ImVfbQBlue( vfb, pptr );

		}



		byteCount = 3 * g_pictFrame.right;



		/* Pack the scanline ? */

		if ( rowBytes >= 8 )

		{

			PackBits( runBuffer, (unsigned char*)g_buf, &byteCount );

			rbp = (unsigned char*) g_buf;

		} else

		{

			rbp = (unsigned char*) runBuffer;

		}



		/* Write the packed scanline */

		if ( ImBinWrite( ioType, fd, fp, &byteCount, UINT,

			BCLen, 1 ) == -1 )

		{

			ImReturnBinError( );

		}

		if ( ImBinWrite( ioType, fd, fp, rbp, UCHAR,

			1, byteCount ) == -1 )

		{

			ImReturnBinError( );

		}

		OddCount += (BCLen + byteCount);

	}



	/*

	 *  If we have written an odd amount of data for the opcode,

	 *  then we have to pad an extra byte so that the next opcode

	 *  is word aligned

	 */

	if ( IModd( OddCount ) )

	{

		g_buf[0] = '\0';

		if ( ImBinWrite( ioType, fd, fp, g_buf, UCHAR, 1, 1)== -1)

		{

			ImReturnBinError( );

		}

	}



	/* ShortComment { picBitEnd };					*/

	ushort1 = 0x00A0;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = IMPC_PICBITEND;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}





	/* ShortComment { picDwgEnd };					*/

	ushort1 = 0x00A0;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}

	ushort1 = IMPC_PICDWGEND;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, 2, 1 )== -1)

	{

		ImReturnBinError( );

	}



#ifdef DEBUG

	fprintf( stderr, "DEBUG: write the END opcode\n" );

#endif



	/* PO_opEndPic { };						*/

	ushort1 = 0x00FF;

	if ( ImBinWrite( ioType, fd, fp, &ushort1, USHORT, OCLen, 1 )== -1)

	{

		ImReturnBinError( );

	}



	free( (char *)runBuffer );



	return ( 1 );

}



