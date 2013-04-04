/**

 **	$Header: /roq/libim/imrla.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imrla.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imrla.c		-  Wavefront RLA/RLB file I/O

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imrla.c contains routines to read and write Wavefront RLA/RLB files for

 **	the image manipulation library.

 **

 **	NOTE: rla files have their origin in the lower left corner.  VFBs

 **	have their origin in the upper left corner so we have to read and

 **	write our VFB scanlines in reverse order.

 **

 **  PUBLIC CONTENTS

 **                     d =defined constant

 **                     f =function

 **                     m =defined macro

 **                     t =typedef/struct/union

 **                     v =variable

 **                     ? =other

 **

 **  PRIVATE CONTENTS

 **

 **     imRlaRead               f  read a Wavefront RLA/RLB file

 **     imRlaWrite              f  write a Wavefront RLA/RLB file

 **

 **     NULL                    d  an empty pointer

 **

 **     imRlaHeaderInfo         t  RLA header

 **     imRlaHeaderFields       t  RLA header description for Bin pkg

 **	imRlaPrHeader		f  Debug routine for printing header

 **	imRlaPrWindow		f  Debug routine for printing header

 **	imRlaDecode		f  decode one scanline of pixel values

 **	imRlaEncode		f  encode one scanline of pixel values

 **

 **  HISTORY

 **	$Log: /roq/libim/imrla.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.23  1995/06/30  22:09:41  bduggan

 **	fixed very weird bug with c++ on decalpha.

 **	(prob'ly a compiler bug.)

 **

 **	Revision 1.22  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.21  1995/06/15  21:12:27  bduggan

 **	changed bzero to memset.  added include

 **

 **	Revision 1.20  1995/04/03  21:36:19  bduggan

 **	took out #ifdef NEWMAGIC

 **

 **	Revision 1.19  1995/01/10  23:42:38  bduggan

 **	put in IMMULTI, IMPIPE instead of TRUE/FALSE

 **	made read/write routines static

 **

 **	Revision 1.18  94/10/03  11:30:53  nadeau

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

 **	Revision 1.17  93/09/22  11:41:33  nadeau

 **	Corrected resolution calculation for info message.

 **	

 **	Revision 1.16  92/12/03  01:52:08  nadeau

 **	Corrected info messages.

 **	

 **	Revision 1.15  92/11/04  12:07:00  groening

 **	put ImFIleFormat info and magic number info

 **	from imfmt.c into this file.

 **	

 **	Revision 1.14  92/10/19  14:11:06  groening

 **	added ImInfo statements

 **	

 **	Revision 1.13  92/08/31  17:34:37  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.12  91/10/04  08:40:11  nadeau

 **	Fixed bad strcpy.

 **	

 **	Revision 1.11  91/10/03  09:18:08  nadeau

 **	Removed bogus PVFILE header field and merged with

 **	GAMMA field.  Rearranged RGB read and write loops.

 **	Expanded window structure into header structure to

 **	avoid struct padding quirks of some C compilers.

 **	

 **	Revision 1.10  91/03/18  15:45:37  todd

 **	added a call to bzero to fix a bug on the cray

 **	

 **	Revision 1.9  91/02/12  11:37:21  nadeau

 **	Removed the tag table checking and VFB conversion now

 **	handled by ImFileRead and ImFileWrite.  Removed pipe

 **	handling.

 **	

 **	Revision 1.8  91/01/29  11:09:44  todd

 **	make alpha channel optional

 **	

 **	Revision 1.7  90/11/30  14:39:48  mercurio

 **	Unchanged wrt 1.6

 **	

 **	Revision 1.6  90/09/07  16:02:38  mercurio

 **	Read function now clears VFB to all zeros before filling (important--

 **	since only the active area is filled explicitly).

 **	

 **	Revision 1.5  90/07/25  14:23:24  mercurio

 **	Fixed read routine to read entire window, not just active window

 **	

 **	Revision 1.4  90/07/02  13:21:56  nadeau

 **	Updated to the new error handling mechanism.

 **	

 **	Revision 1.3  90/06/25  14:47:39  nadeau

 **	Changed ImTag* to Tag* (new names).

 ** 

 **	Revision 1.2  90/06/25  13:19:49  todd

 **	Lots of changes and functionality added.  Works with pipes.

 **	Both RLA and RLB formats accepted.  Fixed bugs.  Tested.

 ** 

 **	Revision 1.1  90/04/11  08:37:46  todd

 **	Initial revision

 ** 

 **	

 **/



/**

 **  FORMAT 

 **	RLA - Wavefront run length encoded format

 **

 **  AKA

 **	.rlb, .rle 

 **

 **  FORMAT REFERENCES

 **	Run Length Encoded File Format, ".rla", Techinal Memo 860605,

 **		Wavefront Technologies

 **

 **  CODE CREDITS

 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1990.

 **     Custom development, Todd Elvins, San Diego Supercomputer Center, 1990.

 **

 **  DESCRIPTION

 **

 ** 	All data in these files are stored Most Significant Byte first.

 ** 	The files are sequential scan-line write, and random scan-line read.  A 

 ** 	scanline offset table is used to locate the beginning of each encoded 

 ** 	scanline within the file.

 **

 ** 	The file is organized into three sections:

 **		- header		740 bytes

 **		- offset table		4*(y resolution;( # of scanlines) bytes

 **		- encoded channel data	variable length

 **

 **    ======

 **    Header

 **    ======

 **

 **		short   window_left;		 Boundaries of complete image	

 **		short   window_right;

 **		short   window_bottom;

 **		short   window_top;

 **		short   active_window_left;	 Boundaries of non-zero image	

 **		short   active_window_right;

 **		short   active_window_bottom;

 **		short   active_window_top;

 **

 ** 	The active_window values specify the dimensions for what is encoded.  

 ** 	The window values specify the dimensions for the complete image.

 **	

 **		short	frame;			 Frame number			

 **

 ** 	The frame number in a sequence of frames.

 **

 **		short	storage_type;		 Byte or word data		

 ** 

 ** 	Specifies whether it is byte or word data.

 **

 **		short	num_chan;		 Number of image channels	

 **

 ** 	Typically 3 for R,G,B.

 ** 

 **		short	num_matte;		 Number of matte channels	

 **

 ** 	0 = no alpha channel.  1 = alpha channel.

 ** 	Could be more if the transparency information includes spectral 

 **	information.

 ** 

 **		short	num_aux;		 Number of aux channels	

 **		short	aux_mask;		 Aux channel type mask	

 **

 **	num_aux is the number of auxillary channels.  Auxillary channel 

 **	information could include distance from the eye, direction of normal,

 **	etc.  There are typically 0 auxillary channels.

 **	aux_mask is a bitmask that describes what is in the auxiallry channels.  

 **	The interpretation of this bitmask is governed by the fmt__aux_chan

 **	table in the system configuration file.

 **

 **		char	gamma[IMRLANGAMMA];	 Image storage gamma		

 **

 ** 	Floating point number: gamma that was applied to the file for storage.

 **

 **		char	red_pri[IMRLANPRI];	 Image red primary chromaticity 

 ** 		char	green_pri[IMRLANPRI];	 Image grn primary chromaticity 

 **		char	blue_pri[IMRLANPRI];	 Image blu primary chromaticity 

 **		char	white_pt[IMRLANPRI];	 White point 			

 **

 ** 	Chromaticities of the red, green, and blue primaries, and the white 

 **   	point that were used to store the image, if it was an rgb image.  

 **	Each field contains the x,y chromaticity as two floating point 

 **	numbers seperated by a space.

 **

 **		int	job_num;		 Job Number			

 **		char	name[IMRLANNAME];	 Original file name 		

 **		char	desc[IMRLANNAME];	 File description number	

 **		char	program[IMRLANPROG];	 Creating program name	

 **		char	machine[IMRLANSTR];	 Creating machine name	

 **		char	user[IMRLANSTR];	 Creating user		

 **		char	date[IMRLANDATE];	 Creation date  		

 **

 ** 	Various informative pieces of data.

 ** 

 **		char	aspect[IMRLANASPECT];	 Image aspect type		

 **		char	aspect_ratio[IMRLANAR];	 Image aspect ratio		

 **

 ** 	Aspect ratio for the image.

 **

 **		char	chan[IMRLANSTR];	 Image color space type	

 **

 ** 	Must be 'rgb'

 **

 **		short	field;			 Rendered on fields flag	

 **		short	filter_type;		 Post filtered interleaved flag

 **		int	magic_number;		 unused			

 **		int	lut_size;		 unused			

 **		int	user_space_size;	 unused			

 **		int	wf_space_size;		 unused			

 **		short	lut_type;		 unused			

 **		short	mix_type;		 unused			

 **		short	encode_type;		 unused			

 **		short	padding;		 unused			

 **		char	space[IMRLANSPACE];	 Unused expansion space	

 **

 ** 	Unused fields.

 **

 **     ============

 **     Offset Table

 **     ============

 ** 

 ** 	The length of each encoded scanline varies, depending on how well the 

 ** 	encoding algorithm compresses the scanline.  The offset table contains

 ** 	the starting positions of each scanline.

 ** 

 **    ====================

 **    Encoded Channel Data

 **     ====================

 ** 

 **	All of the data for one particular scanline appears sequentially in 

 **	a file. If an image has an r,g,b, and alpha channel, then there will 

 **	be a record for the red channel, followed by a record for the green 

 **	channel, followed by a record for the blue channel, followed by a 

 ** 	record for the alpha channel.

 **   	The offset (from the offset table) points to the beginning of the record

 **    	for the red channel.  A record is of the form:

 ** 

 ** 		- length 			2 bytes

 ** 		- encoded string data		length bytes

 **

 ** 	The encoding scheme is fairly simple. It consists of a series of 'run's.

 ** 	There are two types of runs: verbatim and compressed.  A run looks 

 ** 	like this:

 **

 **		- count				2 byte signed integer

 **		- data				MAX(1,-count) bytes of data

 ** 

 ** 	If the count is positive then this is a compressed run.  In this case, 

 ** 	the data is simply a value, which is to be repeated count+1 times.

 ** 

 ** 	If the count is negative, then the next -count bytes of data are to be

 ** 	copied verbatim into the image.

 ** 

 ** 	Data is not compressed if the repeat count is two or less.

 **/



#include <time.h>

//#include <pwd.h>

#include "iminternal.h"





/*

 *  RLA/RLB - Wavefront image file

 *      For information on these structures, how to use them, etc. please

 *      see imfmt.c.

 */

#ifdef __STDC__

static int imRlaRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imRlaWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

#else

static int imRlaRead( ), imRlaWrite( );

#endif

static char *imRlaNames[ ]  = { "rla", "rlb", NULL };

static ImFileFormatReadMap imRlaReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { RGB,3,8,      RLE,    IMVFBRGB,       0 },

        { RGB,3,8,      RLE|A,  IMVFBRGB,       A },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imRlaWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBRGB,     0,      RGB,3,8,        RLE|A,  imRlaWrite },

        { IMVFBRGB,     A,      RGB,3,8,        RLE|A,  imRlaWrite },

        { -1,           0,      -1,             0,      NULL },

};



static ImFileMagic imFileRlaMagic []=

{

	{ 0, 0, NULL },

};



ImFileFormat ImFileRlaFormat =

{

	imRlaNames, "Wavefront raster image file",

	"Wavefront",

	"24-bit RGB and 32-bit RGB+alpha RLE-compressed image files in both\n\
RLB and the older RLA format.",
	"24-bit RGB and 32-bit RGB+alpha RLE-compressed image files in RLB\n\
format.",

	imFileRlaMagic,

	IMNOMULTI, IMNOPIPE,

	IMNOMULTI, IMNOPIPE,

	imRlaRead, imRlaReadMap, imRlaWriteMap

};





/*

 *  TYPEDEF & STRUCTURE

 *	imRlaHeaderInfo		-  RLA file header information

 *	imRlaHeaderFields 	-  Description for Binio package

 *

 *  DESCRIPTION

 *	A RLA file's header contains the image's width, height, and depth,

 *	and many other fields.

 */



#define IMRLANGAMMA	(16)

#define IMRLANPRI	(24)

#define IMRLANNAME	(128)

#define IMRLANPROG	(64)

#define IMRLANSTR	(32)

#define IMRLANDATE	(20)

#define IMRLANASPECT	(24)

#define IMRLANAR	(8)

#define IMRLANSPACE	(100)

#define IMRLAHEADERSIZE	(740)



typedef struct imRlaHeaderInfo

{

	short   window_left;		/* Boundaries of complete image	*/

	short   window_right;

	short   window_bottom;

	short   window_top;

	short   active_window_left;	/* Boundaries of non-zero image	*/

	short   active_window_right;

	short   active_window_bottom;

	short   active_window_top;

	short	frame;			/* Frame number			*/

	short	storage_type;		/* Byte or word data		*/

	short	num_chan;		/* Number of image channels	*/

	short	num_matte;		/* Number of matte channels	*/

	short	num_aux;		/* Number of aux channels	*/

	short	aux_mask;		/* Aux channel type mask	*/

	char	gamma[IMRLANGAMMA];	/* Image storage gamma		*/

	char	red_pri[IMRLANPRI];	/* Image red primary chromaticity */

	char	green_pri[IMRLANPRI];	/* Image grn primary chromaticity */

	char	blue_pri[IMRLANPRI];	/* Image blu primary chromaticity */

	char	white_pt[IMRLANPRI];	/* White point 			*/

	int	job_num;		/* Job Number			*/

	char	name[IMRLANNAME];	/* Original file name 		*/

	char	desc[IMRLANNAME];	/* File description number	*/

	char	program[IMRLANPROG];	/* Creating program name	*/

	char	machine[IMRLANSTR];	/* Creating machine name	*/

	char	user[IMRLANSTR];	/* Creating user		*/

	char	date[IMRLANDATE];	/* Creation date  		*/

	char	aspect[IMRLANASPECT];	/* Image aspect type		*/

	char	aspect_ratio[IMRLANAR];	/* Image aspect ratio		*/

	char	chan[IMRLANSTR];	/* Image color space type	*/

	short	field;			/* Rendered on fields flag	*/

	short	filter_type;		/* Post filtered interleaved flag*/

	int	magic_number;		/* unused			*/

	int	lut_size;		/* unused			*/

	int	user_space_size;	/* unused			*/

	int	wf_space_size;		/* unused			*/

	short	lut_type;		/* unused			*/

	short	mix_type;		/* unused			*/

	short	encode_type;		/* unused			*/

	short	padding;		/* unused			*/

	char	space[IMRLANSPACE];	/* Unused expansion space	*/

} imRlaHeaderInfo;





BinField imRlaHeaderFields[ ] =

{

	{ SHORT, 2, 1 },		/* Window boundaries		*/

	{ SHORT, 2, 1 },

	{ SHORT, 2, 1 },

	{ SHORT, 2, 1 },

	{ SHORT, 2, 1 },		/* Active window boundaries	*/

	{ SHORT, 2, 1 },

	{ SHORT, 2, 1 },

	{ SHORT, 2, 1 },

	{ SHORT, 2, 1 },		/* Frame			*/

	{ SHORT, 2, 1 },		/* Storage type 		*/

	{ SHORT, 2, 1 },		/* Number of image channels	*/

	{ SHORT, 2, 1 },		/* Number of matte channels	*/

	{ SHORT, 2, 1 },		/* Number of aux channels	*/

	{ SHORT, 2, 1 },		/* Aux channel type mask	*/

	{ CHAR, 1, IMRLANGAMMA },	/* Image storage gamma		*/

	{ CHAR, 1, IMRLANPRI },		/* Image red primary chromaticity */

	{ CHAR, 1, IMRLANPRI },		/* Image grn primary chromaticity */

	{ CHAR, 1, IMRLANPRI },		/* Image blu primary chromaticity */

	{ CHAR, 1, IMRLANPRI },		/* Image whi primary chromaticity */

	{ INT, 4, 1 },			/* Job number			*/

	{ CHAR, 1, IMRLANNAME },	/* Original file name 		*/

	{ CHAR, 1, IMRLANNAME },	/* File description 		*/

	{ CHAR, 1, IMRLANPROG },	/* Creating program name	*/

	{ CHAR, 1, IMRLANSTR },		/* Creating machine name	*/

	{ CHAR, 1, IMRLANSTR },		/* Creating user name	 	*/

	{ CHAR, 1, IMRLANDATE },	/* Creation date 	 	*/

	{ CHAR, 1, IMRLANASPECT },	/* Image aspect type  	 	*/

	{ CHAR, 1, IMRLANAR },		/* Image aspect ratio  	 	*/

	{ CHAR, 1, IMRLANSTR },		/* Image color space type 	*/

	{ SHORT, 2, 1 },		/* Rendered on fields flag	*/

	{ SHORT, 2, 1 },		/* Post filtered interleaved flag*/

	{ INT, 4, 1 },			/* unused                 	*/

	{ INT, 4, 1 },			/* unused                 	*/

	{ INT, 4, 1 },			/* unused                 	*/

	{ INT, 4, 1 },			/* unused                 	*/

	{ SHORT, 2, 1 },		/* unused                 	*/

	{ SHORT, 2, 1 },		/* unused                 	*/

	{ SHORT, 2, 1 },		/* unused                 	*/

	{ SHORT, 2, 1 },		/* unused                 	*/

	{ CHAR, 1, IMRLANSPACE },	/* Unused expansion space 	*/

	{ 0, 0, 0 }

};











/*

 *  MACRO

 *	imRlaFrBuf -  Free six chunks of memory

 *

 *  DESCRIPTION

 *	Call free six times to free the six chunks of memory pointed to by

 *	a-f. 

 */



#define imRlaFrBuf(a,b,c,d,e,f) free((a));free((b));free((c));free((d));free((e));free((f))











/*

 *  FUNCTION

 *	imPrRlaHeader	-  print out the header of a rla file

 *

 *  DESCRIPTION

 *	The contents of the ImRlaHeader structure is printed on the 

 *	standard error.

 */



static void

#ifdef __STDC__

imRlaPrHeader(imRlaHeaderInfo *p)

#else

imRlaPrHeader(p)

	imRlaHeaderInfo *p;

#endif

{

        char         message[200];      /* contains information for ImInfo */



	ImInfo ("Image Name", p->name);



	ImInfo ("Byte Order", "Most Significant Byte First");



	sprintf (message, "%d x %d",

		( (p->active_window_right) - (p->active_window_left) + 1),

		( (p->active_window_top) - (p->active_window_bottom) + 1));



	ImInfo ("Resolution", message);



	if (p->storage_type==0)

		sprintf ( message , "24 bit RGB");

	else sprintf ( message , "3 word RGB");

	ImInfo ("Type",message);



	if (p->num_matte>0)

	{

		if (p->storage_type==0)

			sprintf ( message , "8 bit");

		else sprintf ( message , "1 word");

	}

	

	else sprintf (message, "None");



	ImInfo ("Alpha Channel", message);



#ifdef DEBUG

    	fprintf(stderr,"Channels: Image %d  Matte %d  Aux %d   Aux Mask %02x\n",

		p->num_chan, p->num_matte, p->num_aux, p->aux_mask);

	

    	fprintf(stderr,"Job Number %ld  Name \"%s\"\n",p->job_num,p->name);

    	fprintf(stderr,"Desc \"%s\"\n",p->desc);

    	fprintf(stderr,"Program \"%s\" Machine \"%s\"\n",p->program,p->machine);

    

        fprintf(stderr,"User %s  Date %s  Aspect %s  Aspect ratio %s\n",

		p->user,p->date,p->aspect,p->aspect_ratio);

        fprintf(stderr,"Chan %s  Field %d  Filter_type %d  Magic %d\n",

		p->chan, p->field, p->filter_type, p->magic_number );

#endif



    	sprintf (message, "%d", p->frame);

	ImInfo ("Frame Number", message);



	ImInfo ( "Gamma", p->gamma);



	ImInfo ( "Red Primary", p->red_pri);



	ImInfo ( "Green Primary", p->green_pri);



	ImInfo ( "Blue Primary", p->blue_pri);



	ImInfo ( "White Point", p->white_pt);



}











/*

 *  FUNCTION

 *	imRlaDecode	-  decode one channel of one scanline

 *

 *  DESCRIPTION

 *	One channel of one scanline (cIn) in Wavefront RLA format is decoded

 *	and returned in cOut.  len is the number of bytes in cIn.

 */

static void

#ifdef __STDC__

imRlaDecode( unsigned char *cIn, unsigned char *cOut, int len )

#else

imRlaDecode( cIn, cOut, len )

	unsigned char *cIn;

	unsigned char *cOut;

	int len;

#endif

{

	int cnt;



	/*

	 * Loop through the bytes to be decoded

	 */

	while( len > 0 ) 

	{

		cnt = *cIn++;

		len--;

		if ( cnt < 128 ) 

		{

			/*

			 * Repeat pixel value cnt+1 times 

			 */

			while ( cnt-- >= 0 )

				*cOut++ = *cIn;

			cIn++;

			len--;

		}

		else

			/*

			 * Just copy the unencoded bytes directly

			 */

			for ( cnt=256-cnt; cnt-- > 0; len-- )

				*cOut++ = *cIn++;

	}

}











/*

 *  FUNCTION

 *	imRlaRead	-  read an Wavefront RLA file

 *

 *  DESCRIPTION

 *	The file header is read and the size of the image determined.

 *	Space is allocated for the VFB and the run codes read in to

 *	a run buffer.  The run buffer is then expanded into straight

 *	RGB values in the VFB and the completed VFB added to the tag table.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__

imRlaRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRlaRead( ioType, fd, fp, flagsTable, tagTable )

        int         ioType;             /* I/O flags                    */

        int         fd;                 /* Input file descriptor        */

        FILE       *fp;                 /* Input file pointer           */

	TagTable   *flagsTable;		/* Flags			*/

        TagTable   *tagTable;           /* Tag list to add to           */

#endif

{

	ImVfb       *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	ImVfbPtr     leftpptr;		/* Points to start of scanlines */

	imRlaHeaderInfo  header;	/* RLA file header		*/

	unsigned char       *redBuf;		/* Run buffer			*/

	unsigned char       *grnBuf;		/* Run buffer			*/

	unsigned char       *bluBuf;		/* Run buffer			*/

	unsigned char       *alpBuf;		/* Run buffer			*/

	unsigned char       *buf;		/* Run buffer			*/

	short	     len;		/* Number of bytes unencoded	*/

	int	    *offsetTable;	/* Array of scanline offsets	*/

	int	     status;		/* Status returned from seek	*/

	int x,y,dep;		/* Convenient short names	*/

	int i,j;		/* Loop counters		*/

	int width;		/* true width of image		*/

	int height;		/* true height of image		*/

	int bPad;		/* bottom padding amount	*/

	int lPad;		/* left padding amount		*/

	unsigned char* freak;	/* See the paragraph below	*/



	/* The variable 'freak' is to have a totally gratuitous

	 * cast from an int into an unsigned char*.  The purpose

	 * of this is to stop the decalpha c++ compiler from making

	 * some sort of weird crazy screwed up optimization that 

	 * results in an endless loop.  

	 *

	 * I don't understand why this works.  But take out the

	 * cast, and this routine won't work on decalpha's with c++.

	 * 

	 * You don't believe me?  Try it!

	 *

	 * I haven't had enough free time to investigate this compiler

	 * bug, or pinpoint it very well.  

	 */



	/*

	 *  RLA files are usually generated on Irises, which have an MBF

	 *  byte order.  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINMBF );





	/*

	 *  Read in the header.

	 */

	if (ImBinReadStruct(ioType, fd, fp, &header, imRlaHeaderFields ) == -1)

	{

		ImReturnBinError( );

	}



	imRlaPrHeader( &header );



	width  = 1 + header.window_right - header.window_left;

	height = 1 + header.window_top - header.window_bottom;



	x      = 1 + header.active_window_right - header.active_window_left;

	y      = 1 + header.active_window_top - header.active_window_bottom;



	bPad   = (int) (header.active_window_bottom - header.window_bottom);

	lPad   = (int) (header.active_window_left - header.window_left); 

	/*

	 * Here's the moronic hack of all hacks to defeat

	 * the evil decalpha c++ compiler:

	 */

	freak = (unsigned char*)&lPad;

	

	dep    = header.num_chan + header.num_matte;





	/*

	 *  Check the important header parameters for legal bounds.

	 *	header.chan		must be 'rgb'

	 *	x and y			must have positive active window size

	 *	image depth		must be 3 (RGB) or 4 (RGBA)

	 *	header.num_aux		must be 0

	 *

	 *  We'd like to check these as well, however while RLB files have

	 *  them, RLA files don't, so we just skip checking their values.

	 *	header.field		must be 0

	 *	header.magic_number	must be 0

	 *	header.filter_type	must be 0

	 */

	if ((strcmp( header.chan, "rgb" ) != 0) || x < 1 || y < 1 || 

		(dep != 4 && dep != 3) || header.num_aux != 0 )

	{

		ImErrNo = IMESYNTAX;

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}





	/*

	 *  Allocate space for the offset table and read it in

	 */

	ImMalloc( offsetTable, int *, sizeof(int) * y );

	if ( ImBinRead( ioType, fd, fp, offsetTable, INT, 4, y ) == -1 )

	{

		free( (char *)offsetTable );

		ImReturnBinError( );

	}



	

	/*

	 *  Allocate a VFB of the required size and clear it.

	 */

	if ( header.num_matte == 1 )

		vfb = ImVfbAlloc( width, height, IMVFBRGB | IMVFBALPHA );

	else

		vfb = ImVfbAlloc( width, height, IMVFBRGB );

	if ( vfb == IMVFBNULL )

	{

		free( (char *)offsetTable );

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}

	ImVfbClear( ImVfbQFields( vfb ), 0, vfb );





	/*

	 *  Allocate run buffers large enough for one scanline per channel.

	 */ 

	ImCalloc( redBuf, unsigned char *, x, 1 );

	ImCalloc( grnBuf, unsigned char *, x, 1 );

	ImCalloc( bluBuf, unsigned char *, x, 1 );

	ImCalloc( alpBuf, unsigned char *, x, 1 );

	ImCalloc( buf,    unsigned char *, x*2, 1 );





	/*

	 *  Loop through the scan lines.  Read in a scanline and decode it.

	 *  Once all of the channels are decoded, copy them into the vfb,

	 *  going from bottom to top.

	 */

	pptr = leftpptr = ImVfbQPtr( vfb, 0, height-bPad-1 );

	for( i=0; i<y; i++ )

	{

		ImSeek( ioType, fd, fp, offsetTable[i], 0 );



		for ( j = 0; j < dep; j++ )

		{

			if (ImBinRead(ioType, fd, fp, &len, SHORT, 2, 1 ) == -1)

			{

				imRlaFrBuf( redBuf, grnBuf, bluBuf, alpBuf,

					buf, offsetTable );

				ImReturnBinError( );

			}

			if ( len == 0 )

			{

				imRlaFrBuf( redBuf, grnBuf, bluBuf, alpBuf,

					buf, offsetTable );

				ImErrNo = IMESYNTAX;

				ImErrorFatal( "Bad scanline encoding", -1,

					ImErrNo );

			}

			if (ImBinRead(ioType, fd, fp, buf, UCHAR, 1, len )== -1)

			{

				imRlaFrBuf( redBuf, grnBuf, bluBuf, alpBuf,

					buf, offsetTable );

				ImReturnBinError( );

			}

			switch ( j )

			{

			case 0:	imRlaDecode( buf, redBuf, len ); break;

			case 1:	imRlaDecode( buf, grnBuf, len ); break;

			case 2:	imRlaDecode( buf, bluBuf, len ); break;

			case 3:	imRlaDecode( buf, alpBuf, len ); break;

			}

		}



		/*

		 *  Skip over left padding area

		 */

		for ( j = 0; j < lPad; j++ )

		{

			ImVfbSInc( vfb, pptr );

		}



		/*

		 * Copy the buffers into the vfb 

		 */

		if ( header.num_matte == 1 )

		{

			for ( j = 0; j < x; j++ )

			{

				ImVfbSRed(   vfb, pptr, redBuf[j] );

				ImVfbSGreen( vfb, pptr, grnBuf[j] );

				ImVfbSBlue(  vfb, pptr, bluBuf[j] );

				ImVfbSAlpha( vfb, pptr, alpBuf[j] );

				ImVfbSInc(   vfb, pptr );

			}

		}

		else

		{

			for ( j = 0; j < x; j++ )

			{

				ImVfbSRed(   vfb, pptr, redBuf[j] );

				ImVfbSGreen( vfb, pptr, grnBuf[j] );

				ImVfbSBlue(  vfb, pptr, bluBuf[j] );

				ImVfbSInc(   vfb, pptr );

			}

		}



		/*

		 * Position leftpptr at the beginning of the scanline above

		 */

		ImVfbSUp( vfb, leftpptr );

		pptr = leftpptr;

	}





	/*

	 *  Add the VFB to the tagTable.

	 */

	TagTableAppend( tagTable, TagEntryAlloc( "image vfb", POINTER, &vfb ) );



	/*

	 * Free the pixel storage arrays

	 */

	imRlaFrBuf(redBuf,grnBuf,bluBuf,alpBuf,buf,offsetTable);



	return ( 1 );

}











/*

 *  FUNCTION

 *	imRlaEncode	-  encode one channel of one scanline

 *

 *  DESCRIPTION

 *	One unencoded channel of one scanline (cIn) is encoded into

 * 	Wavefront RLA format and returned in cOut.  

 */



static int				/* Returns # of encoded bytes	*/

#ifdef __STDC__

imRlaEncode( unsigned char *cOut, unsigned char *cIn, int width )

#else

imRlaEncode( cOut, cIn, width )

	unsigned char *cOut;		/* Returned encoded bytes	*/

	unsigned char *cIn;		/* Incomming unencoded data	*/

	int    width;		/* # of unencoded bytes		*/

#endif

{

	int    cnt;		/* Run length counter		*/

	int    len = 0;	/* Encoded byte counter		*/



	/*

	 * Loop thru the unencoded bytes

	 */

	while ( width > 0 )

	{

		if (( width > 1 ) && ( cIn[0] == cIn[1] ))

		{

			/* Encode case */

			for( cnt=2; cnt < width; cnt++ )

			{

				if ( cIn[cnt] != cIn[cnt-1] )

					break;

				if ( cnt >= 127 )

					break;

			}



			/* Write out count */

			*cOut++ = cnt-1;

			len++;



			/* Write out value */

			*cOut++ = *cIn;

			len++;

			width -= cnt;

			cIn += cnt;

		}

		else	/* Don't encode */

		{

			for( cnt=1; cnt <  width; cnt++ )

			{

				if ((width-cnt > 1) && (cIn[cnt]==cIn[cnt+1]))

					break;

				if ( cnt >= 127 )

					break;

			}



			/* Write out count */

			*cOut++ = 256 - cnt;

			len++;

		

			/* Copy string of pixels */

			for ( ; cnt-- > 0; len++, width-- )

				*cOut++ = *cIn++;

		}

	}

	return( len );

}











/*

 *  FUNCTION

 *	imRlaWrite	-  write an Wavefront RLA file

 *

 *  DESCRIPTION

 *	The VFB is queried, and the RLA file header set up and written out.

 *	The VFB data is then read out and converted to run-codes, and those

 *	codes written out.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imRlaWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imRlaWrite( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

        int          ioType;            /* I/O flags                    */

        int          fd;                /* Input file descriptor        */

        FILE        *fp;                /* Input file pointer           */

	TagTable    *flagsTable;	/* Flags			*/

        TagTable    *tagTable;          /* Tag list to read from        */

#endif

{

	int x,y,dep;		/* Convenient short names	*/

	ImVfb        *vfb;		/* Read in image		*/

	imRlaHeaderInfo header;		/* RLA file header		*/

	time_t	     seconds;		/* Return value from time()	*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	ImVfbPtr     leftpptr;		/* Points to start of scanlines */

	unsigned char       *redBuf;		/* Run buffer			*/

	unsigned char       *grnBuf;		/* Run buffer			*/

	unsigned char       *bluBuf;		/* Run buffer			*/

	unsigned char       *alpBuf;		/* Run buffer			*/

	unsigned char       *buf;		/* Run buffer			*/

	int	    *offsetTable;	/* Array of scanline offsets	*/

        int          writeCount;        /* Keep count of file offset    */

	short	     len;		/* Number of bytes unencoded	*/

	int i,j;		/* Loop counters		*/



	char	    *name;		/* Login name			*/

	char	     tmp[100];		/* Temp buffer			*/

	struct passwd *pass;		/* Password file entry		*/





	/*

	 *  RLA files are usually generated on Irises, which have an MBF

	 *  byte order.  There is no floating point in the file so we

	 *  won't bother setting the float format for BIN.

	 */

	BinByteOrder( BINMBF );





	/*

	 *  Set up the header and write it out.

	 */

	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	x = ImVfbQWidth( vfb );

	y = ImVfbQHeight( vfb );



	header.window_right 	   = x-1; /* Right side			*/

	header.window_top 	   = y-1; /* Top side			*/

	header.window_left 	   = 0; /* Left side			*/

	header.window_bottom 	   = 0; /* Bottom side			*/



	header.active_window_right = x-1; /* Right side			*/

	header.active_window_top   = y-1; /* Top side			*/

	header.active_window_left  = 0; /* Left side			*/

	header.active_window_bottom= 0; /* Bottom side			*/



	header.frame               = 1;	/* First frame	    (anything)	*/

	header.storage_type        = 0;	/* Image data	    (must be 0)	*/

	header.num_chan            = 3;	/* RGB		    (must be 3)	*/

	header.num_matte           = 1;	/* Alpha included   (0 or 1)	*/

	header.num_aux             = 0;	/* no aux channels  (must be 0)	*/

	header.aux_mask            = 0;	/* no aux channels  (must be 0)	*/

	header.job_num             = 0;	/* job 0	    (anything)	*/

	header.field               = 0;	/* not on fields    (0 or 1)	*/

	header.filter_type         = 0;	/* non-interleaved   (0 or 1)	*/

	header.magic_number        = 0;	/* no magic	    (must be 0)	*/

	header.lut_size            = 0;	/* no CLT	    (must be 0)	*/

	header.user_space_size     = 0;	/* no user space    (must be 0)	*/

	header.wf_space_size       = 0;	/* no wf space      (must be 0)	*/

	header.lut_type            = 0;	/* no CLT	    (must be 0)	*/

	header.mix_type            = 0;	/* no mix	    (must be 0)	*/

	header.encode_type         = 0;	/* no encoding	    (must be 0)	*/

	header.padding             = 0;	/* no padding	    (must be 0)	*/



	strcpy( header.gamma,      "2.200" );

	strcpy( header.red_pri,    "00.6700 00.3300" );

	strcpy( header.green_pri,  "00.2100 00.7100" );

	strcpy( header.blue_pri,   "00.1400 00.0800" );

	strcpy( header.white_pt,   "00.3100 00.3160" );



	strncpy( header.name,     ImErrorFileName, IMRLANNAME );

	strcpy(  header.desc,     "image file" );

	strncpy( header.program,  ImErrorProgramName, IMRLANPROG );

	strcpy(  header.chan,     "rgb" );



	/*

	if ( (name = getlogin( )) == NULL )

	{

		pass = getpwuid( getuid( ) );

		name = pass->pw_name;

	}

	*/



	name = "Creator";



     	strncpy( header.user, name, IMRLANSTR );



        strcpy( header.aspect, "user defined");

	sprintf( tmp, "%3.2f", (float)x/y );

	strncpy( header.aspect_ratio, tmp, IMRLANAR );



        seconds = time( (time_t *)NULL );

        strncpy( header.date, ctime( &seconds ), 19 );



#ifndef WIN32

	gethostname( header.machine, IMRLANSTR );

#else

	strcpy(header.machine, "jaws");

#endif



	/*

	 *  Write out the RLA image file header

	 */

	if (( writeCount = ImBinWriteStruct( ioType, fd, fp, &header, imRlaHeaderFields )) == -1 )

	{

		ImReturnBinError( );

	}

	if ( writeCount != IMRLAHEADERSIZE ) 

	{

		ImErrNo = IMESYS;

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	imRlaPrHeader( &header );



	/*

	 *  Allocate an offset table and initialize it to zeroes.  We'll

	 *  write it out as zeroes now, fill it while we encode the data,

	 *  then seek back and write the real table out when we're done.

	 */

	ImMalloc( offsetTable, int *, sizeof(int) * y );

	memset( offsetTable, 0x00, sizeof(int) * y );

	if ( ImBinWrite( ioType, fd, fp, offsetTable, INT, 4, y ) == -1 )

	{

		free( (char *)offsetTable );

		ImReturnBinError( );

	}

	writeCount += 4 * y;





	/*

	 * Allocate run buffers large enough for one scanline per color

	 */ 

	ImCalloc( redBuf, unsigned char *, x, 1 );

	ImCalloc( grnBuf, unsigned char *, x, 1 );

	ImCalloc( bluBuf, unsigned char *, x, 1 );

	ImCalloc( alpBuf, unsigned char *, x, 1 );

	ImCalloc( buf,    unsigned char *, x*2, 1 );





	/*

	 *  Loop through the scan lines, from the bottom up.  Fill the

	 *  buffers with channel values, encode them, and then write them out.

	 */

	pptr = leftpptr = ImVfbQPtr( vfb, 0, y-1 );

	for( i=0; i<y; i++ )

	{

		offsetTable[i] = writeCount;



		/*

		 * Copy the vfb pixel values into scanline buffers.

		 * If an alpha channel is present, use it.  If not, set to

	 	 * 0xFF.

		 */

		if ((ImVfbQFields(vfb) & IMVFBALPHA) == IMVFBALPHA) 

		{

			for ( j = 0; j < x; j++ )

			{

				redBuf[j] = ImVfbQRed(   vfb, pptr );

				grnBuf[j] = ImVfbQGreen( vfb, pptr );

				bluBuf[j] = ImVfbQBlue(  vfb, pptr );

				alpBuf[j] = ImVfbQAlpha( vfb, pptr );

				ImVfbSInc(   vfb, pptr );

			}

		}

		else

		{

			for ( j = 0; j < x; j++ )

			{

				redBuf[j] = ImVfbQRed(   vfb, pptr );

				grnBuf[j] = ImVfbQGreen( vfb, pptr );

				bluBuf[j] = ImVfbQBlue(  vfb, pptr );

				alpBuf[j] = 0xFF;

				ImVfbSInc(   vfb, pptr );

			}

		}





		for ( j = 0; j < 4;  j++ )

		{

			switch( j )

			{

			case 0:	len = imRlaEncode( buf, redBuf, x ); break;

			case 1:	len = imRlaEncode( buf, grnBuf, x ); break;

			case 2:	len = imRlaEncode( buf, bluBuf, x ); break;

			case 3:	len = imRlaEncode( buf, alpBuf, x ); break;

			}

			writeCount += (2 + len);

#ifdef DEBUG

{int k;

fprintf(stderr,"    unenc %2d,%2d:  ",i,j);

switch(j){

case 0: for(k=0;k<x;k++)fprintf(stderr,"%3d ",redBuf[k]); break;

case 1: for(k=0;k<x;k++)fprintf(stderr,"%3d ",grnBuf[k]); break;

case 2: for(k=0;k<x;k++)fprintf(stderr,"%3d ",bluBuf[k]); break;

case 3: for(k=0;k<x;k++)fprintf(stderr,"%3d ",alpBuf[k]); break;

}fprintf(stderr,"\n      enc (%3d):  ",len);

for(k=0;k<len;k++)if(buf[k]&0x80)fprintf(stderr,"-%2d ",256-buf[k]);else fprintf(stderr,"%3d ",buf[k]);

fprintf(stderr,"\n");

}

#endif



			if (ImBinWrite(ioType, fd, fp, &len, SHORT, 2, 1 )== -1)

			{

				imRlaFrBuf( redBuf, grnBuf, bluBuf, alpBuf, buf,

					offsetTable );

				ImReturnBinError( );

			}

			if (ImBinWrite(ioType, fd, fp, buf, UCHAR, 1, len)== -1)

			{

				imRlaFrBuf( redBuf, grnBuf, bluBuf, alpBuf, buf,

					offsetTable );

				ImReturnBinError( );

			}

		}

		



		/*

		 *  Position leftpptr at the beginning of the scanline above

		 */

		ImVfbSUp( vfb, leftpptr );

		pptr = leftpptr;

	}





	/*

	 * Seek backwards and write out the correct offsetTable.

	 */

	ImSeek( ioType, fd, fp, IMRLAHEADERSIZE, 0 );

#ifdef DEBUG

if(ioType&IMFILEIOFD)

 fprintf(stderr,"seeked to %d bytes.  tell reports 0x%08x (%d)\n",IMRLAHEADERSIZE,tell(fd),tell(fd));

else

 fprintf(stderr,"seeked to %d bytes.  tell reports 0x%08x (%d)\n",IMRLAHEADERSIZE,ftell(fp),ftell(fp));

for(i=0;i<y;i++)fprintf(stderr,"offsetTable[i] = 0x%08x (%d)\n",offsetTable[i],offsetTable[i]);

#endif

	if ( ImBinWrite( ioType, fd, fp, offsetTable, INT, 4, y ) == -1 )

	{

		imRlaFrBuf(redBuf,grnBuf,bluBuf,alpBuf,buf,offsetTable);

		ImReturnBinError( );

	}



	imRlaFrBuf(redBuf,grnBuf,bluBuf,alpBuf,buf,offsetTable);



	return ( 1 );

}

