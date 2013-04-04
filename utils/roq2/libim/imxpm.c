/**

 **	$Header: /roq/libim/imxpm.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imxpm.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imxpm.c		-  X11 PixMap file I/O

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imxpm.c contains routines to read and write Sun Icon files for

 **	the image manipulation library.  Raster data read in is stored

 **	in a VFB and optional CLT in a tag list.  Raster data written

 **	out is taken from a tag list.

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **	none

 **

 **  PRIVATE CONTENTS

 **

 **	imXpmRead		f  read a X11 Bitmap file

 **	imXpmWrite		f  write a X11 Bitmap file

 **

 **

 **  HISTORY

 **	$Log: /roq/libim/imxpm.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 * Revision 1.5  1995/06/30  22:12:26  bduggan

 * Fixed a macro bug (macro was expanded into 's)

 *

 * Revision 1.4  1995/06/29  00:28:04  bduggan

 * updated copyright year

 *

 * Revision 1.3  1995/06/29  00:21:14  bduggan

 * changed comment

 *

 * Revision 1.2  1995/06/15  20:12:07  bduggan

 * Removed embedded comments, took out a useless var.

 *

 * Revision 1.1  1995/05/17  23:49:56  bduggan

 * Initial revision

 *

 **/



#include <ctype.h>

#include "iminternal.h"

#include "imxpm.h"





/**

 **  FORMAT

 **	xpm	-  X11 Pixel Map

 **

 **  AKA

 **	pm

 **

 **  FORMAT REFERENCES

 **	XPM Manual by Arnaud Le Hors

 **	(Available in postscript format from ftp.x.org in /R5contrib/xpm-3.4a.tar.gz)

 **

 **

 **  CODE CREDITS

 **	Custom Development, Brian Dugggan, San Diego SuperComputer Center, 1995

 **

 **  DESCRIPTION

 **	X11 pixmaps are simple C code to declare and initialize an array of strings.

 **	The elements of the array are indexes into a color lookup table, which is

 **	described in the beginning of the file.

 **

 **	For instance, here's a typical xpm file:

 **	(Replace all the \'s below with /'s.  I put them in

 **	 backwards so that this example wouldn't mess up compilation

 **	 of this file (imxpm.c). )

 **

 **	static char* my_pixmap[] = {

 **	\* width height ncolors charsperpixel [xhot yhot] [XPMEXT] *\

 **	"10 10 3 2 0 0 XPMEXT",

 **	\* colors *\

 **	"    c red    m white  s name_1",

 **	"xx  c green  m black  s name_2",

 **	"yy  c blue   m white  s name_3",

 **	\* pixels  *\

 **	"              xxyy",

 **	"yy  yy  yy  yy  yy",

 **	"  yy  yy  yy  yy  ",

 **	"xx  xx  xx  xx  xx",

 **	"  xx  xx  xx  xx  ",

 **	"              xxyy",

 **	"yy  yy  yy  yy  yy",

 **	"  yy  yy  yy  yy  ",

 **	"xx  xx  xx  xx  xx",

 **	"              xxyy",

 **	"yy  yy  yy  yy  yy",

 **	\* extension data *\

 **	"XPMEXT ext1 data1",

 **	"XPMENDEXT"

 **	};

 **

 **	The stuff at the end of the file (XPMEXT..) is an extension for the

 **	the picture.  We don't touch those here. 

 **

 **	The color lookup table works as follows:

 **	The first 2 characters (since there are 2 character per pixel in this particular

 **	image) reference the number of the clt.  Then "c red" indicates that if color

 **	is possible this color should be read.  "m white" means if only mono is possible, this

 **	value should be white.  "g4 val" means use val for 4-bit grayscale. "g val" means

 **	Use val for >4 bit grayscale.  "s name" means 'name' is the name of this entry.  (We

 **	don't use the s flag.)

 **

 **	xhot and yhot are the hot spot location.  

 **	We read and write hotspots.

 **

 **/



/*

 * TYPEDEF

 *    charTable

 *

 * DESCRIPTION

 *	This is the hash array that we're going to use to store the

 * various character combination.  That is, 2 characters determines

 * an index value.  So the array element corresponding to the 2 characters

 * will contain this index value.

 */

typedef int *charTable;



/*

 *  XPM - MIT X11 Window System PixMap

 *      For information on these structures, how to use them, etc. please

 *      see imfmt.c.

 */

#ifdef __STDC__

static int imXpmRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imXpmWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 

	TagTable *tagTable );

static int imXpmReadClt(FILE* fp,int numColors, TagTable* tagTable, charTable cltTable, int charsPerPixel, ImClt* clt);

static int imXpmGetRgb(char* buffer, unsigned char *red, unsigned char *green, unsigned char *blue, int *transparent);

static char* strHasFlag(char* buffer, char c);

static int imXpmLookUpRgb(char *colorName,unsigned char*red,unsigned char *green,unsigned char *blue, int* transparent);

#else

static int imXpmRead( );

static int imXpmWrite( );

static int imXpmReadClt();

static int imXpmGetRgb();

static char* strHasFlag();

static int imXpmLookUpRgb();

#endif



static char *imXpmNames[ ]  = { "xpm", "pm", NULL };

static ImFileFormatReadMap imXpmReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IN,1,8,       0,      IMVFBINDEX8,    0 },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imXpmWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

        { IMVFBINDEX8,  0,      IN,1,8,         0,      imXpmWrite },

        { -1,           0,      -1,             0,      NULL },

};



static ImFileMagic imFileXpmMagic []=

{

	{ 0, 0, NULL},

};



ImFileFormat ImFileXpmFormat =

{

	imXpmNames, "X11 pixmap file",

	"X Consortium / MIT",

	"ASCII pixmap color indexed files.",

	"ASCII pixmap color indexed files.",

	imFileXpmMagic,

	IMNOMULTI, IMPIPE,

	IMNOMULTI, IMPIPE,

	imXpmRead, imXpmReadMap, imXpmWriteMap

};











/*

 *  FUNCTION

 *	imXpmRead	-  read an X11 pixmap file

 *

 *  DESCRIPTION

 *	The file is read and it's mono image written to a new mono VFB.

 *

 *	Xpm files have characters which are associated with index values.

 *	Thus, we need to associate an integer (the index value) with

 *	several different one or two character strings.  This is done using

 *	a very primitive hash table;  An array of integers with 128x128 entries.

 *	i.e. one for each two letter comibination.  When we read in the colors,

 *	we set the entry in the array corresponding to the two letter code, to its

 *	corresponding index value.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__

imXpmRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imXpmRead( ioType, fd, fp, flagsTable, tagTable )

	int         ioType;		/* I/O flags			*/

	int         fd;			/* Input file descriptor	*/

	FILE	   *fp;			/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable   *tagTable;		/* Tag table to add to		*/

#endif

{

	char           message[1024];	/* Error message holder		*/

	char	       buffer[1024];    /* really big char buffer       */

	char c,c1,c2;				/* characters 			*/

	int xSize, ySize;		/* dimensions			*/

	int charsPerPixel;		/* chars per pixel		*/

	int numColors;			/* number of colors		*/

	ImClt* clt;			/* clt				*/

	charTable	cltTable;	/* table w/ clt values          */

	int i,j;			/* indices			*/

	ImVfb* vfb;			/* vfb				*/

	ImVfbPtr vfbPtr;		/* vfb pointer 			*/

	int index;			/* hash index			*/

	ImHotSpotPtr	hotspotptr;	/* points to hot spot		*/

	int xHot, yHot;			/* hot spot values		*/



	/*

	 * Please notice that in this routine, I have decided NOT

	 * to rewrite a C-language parser.  Instead, I'm only going

	 * to be able to read nice xpm files.  So xpm's with quotation 

	 * marks in the comment blocks, hex values instead of strings,

	 * extreme amounts of whitespace, or other devious sorts of things 

	 * will not be handled very well here.  Sorry.

	 */





	if ( ioType & IMFILEIOFD )

	{

		/*

		 *  Given file descriptor.  More convenient to deal with

		 *  a file pointer for this format.

		 */

		if ( (fp = fdopen( fd, "rb" )) == NULL )

		{

			ImErrNo = IMESYS;

			return ( -1 );

		}

	}



	/*

	 * Find the first quote in the file.  Then read

	 * in width, height, number of colors, chars per pixel.

	 */

	c = '0';

 	while (c!='"' && !feof(fp))

		c = fgetc(fp);

	if (c!='"')

		ImErrorFatal("Unexpected end of file", -1, IMESYS);

	fscanf(fp, "%d %d %d %d", &xSize, &ySize, &numColors, &charsPerPixel);



	sprintf(message,"%d x %d",xSize,ySize);

	ImInfo("Resolution",message);

	ImInfo("Type","8-bit Color Indexed");

	sprintf(message,"%d entries",numColors);

	ImInfo("Color Table",message);

	ImInfo("Compression Type","none");

	ImInfo("Alpha Channel","none");



	/* Read the rest of this string into a buffer */

	i = 0;

 	while (c!=',' && !feof(fp))

	{

		c = fgetc(fp);

		buffer[i++] = c;

	}

	if (c!=',')

		ImErrorFatal("Unexpected end of file", -1, IMESYS);

	buffer[i] = '\0';

	if (sscanf(buffer,"%d %d",&xHot,&yHot)==2)

	{	/* we have a hot spot */

		sprintf(message,"%d, %d",xHot, yHot);

		ImInfo("Hot Spot",message);

		ImMalloc(hotspotptr, ImHotSpotPtr, sizeof(ImHotSpot) * 1);

		ImHotSpotSX( hotspotptr, xHot);

		ImHotSpotSY( hotspotptr, yHot);

		TagTableAppend( tagTable, TagEntryAlloc( "image hot spot",

			POINTER, &hotspotptr) );

	}



	if (numColors > 256)

	{

		ImErrorFatal( "Too many colors", -1, IMEUNSUPPORTED );

	}

	if (charsPerPixel > 2)

	{

		ImErrorFatal( "Too many characters per pixel.  (Max is 2). ", -1, IMEUNSUPPORTED);

	}



	/* 

	 * Allocate our hash table  

	 */

	ImMalloc(cltTable, charTable, (sizeof(int) * 128 * 128) );

	clt = ImCltAlloc( numColors );



	/*

	 * Read the clt

	 */

	imXpmReadClt(fp,numColors, tagTable, cltTable, charsPerPixel, clt);



	/*

	 * Read the pixels

	 */

	vfb = ImVfbAlloc(xSize, ySize, IMVFBINDEX8);

	ImVfbSClt(vfb,clt);

	vfbPtr = ImVfbQFirst(vfb);

	for (i=0;i<ySize; i++)

	{

		/*

		 * Go to the beginning of the next row

		 */

		c = '\0';

		while (c!='"' && !feof(fp))

			c = fgetc(fp);

		if (c!='"')

			ImErrorFatal("Unexpected end of file", -1, IMESYS);

		/* Read in this row */

		for (j=0;j<xSize;j++)

		{

			if (charsPerPixel==1)

			{

				c1 = fgetc(fp);

				index = (int)c1;

				ImVfbSIndex8(vfb,vfbPtr,cltTable[index]);

			}

			else

			{  	/* two chars per pixel */

				c1 = fgetc(fp);

				c2 = fgetc(fp);

				index = ((int)c1) * 128 + ((int)c2);

				ImVfbSIndex8(vfb,vfbPtr,cltTable[index]);

			}

			ImVfbSInc(vfb, vfbPtr);

		}

		fgetc(fp);   /* Get the " from the end */

	}



	TagTableAppend( tagTable,

		TagEntryAlloc( "image vfb", POINTER, &vfb ) );

	return ( 1 );

}





/*

 *  FUNCTION

 * 	imXpmReadClt

 *

 *  DESCRIPTION

 *	Read a clt.  This is made tricky by the fact that NAMES

 *  of colors may be used.

 *

 *	Also, if there's a transparent color, set that in the

 *  the data table.

 *

 */

static int /* returns status */

#ifdef __STDC__

imXpmReadClt(FILE* fp,int numColors, TagTable* tagTable, charTable cltTable, int charsPerPixel, ImClt* clt)

#else

imXpmReadClt(fp,numColors, tagTable, cltTable, charsPerPixel, clt)

FILE* fp;

int numColors;

TagTable* tagTable;

charTable cltTable;

int charsPerPixel;

ImClt* clt;

#endif

{

	int i,j;		/* index        */

	char c = '2';		/* holds a char */

	char buffer[1024];  	/* holds a line */

	int index;		/* hash index   */

	ImCltPtr cltPtr;	/* clt pointer  */

	unsigned char red, green, blue; /* rgb values */

	int transparent;	/* Is this value transparent? */

	char* tmp;		/* holds a char*  */

	char message[100];	/* holds a message*/

	

	if (numColors==0)

		return 0;



	cltPtr = ImCltQFirst(clt);

	for (i=0;i<numColors;i++)

	{

		/*

		 * Go to the beginning of the next line

		 * (Lines start with a " )

		 */

		c = '\0';

		while (c!='"' && !feof(fp))

			c = fgetc(fp);

		if (c!='"')

			ImErrorFatal("Unexpected end of file", -1, IMESYS);



		/*

		 * Read a line, parse it, add it to the table

		 */

		 j = 0;

		 c = '\0';   

		 while (c!='"' && !feof(fp))

		 {

			c = fgetc(fp);

			buffer[j] = c;

			j++;

		 }

		if (c!='"')

			ImErrorFatal("Unexpected end of file", -1, IMESYS);

		 buffer[j] = '\0';

		 if (charsPerPixel==1)

			index = buffer[0];

		 else

		 	index = ((int)buffer[0])*128 + (int)buffer[1];

		 cltTable[index] = i;



		/*

		 * Now we want to set entry i in the clt to be this color.

		 */

		 imXpmGetRgb(buffer+charsPerPixel, &red, &green, &blue, &transparent);

		 ImCltSRed(cltPtr, red);

		 ImCltSBlue(cltPtr, blue);

		 ImCltSGreen(cltPtr, green);

		 ImCltSInc(clt,cltPtr);

		 if (transparent==1)

		 {	/* Add to tag table */

			sprintf(message,"Pixels with index %d. (RGB %d, %d, %d)",i,

				red,green,blue);

			ImInfo("Transparency",message);

			ImMalloc(tmp, char *, 15);

			sprintf(tmp,"index=%d",i);

			TagTableAppend( tagTable,

				TagEntryAlloc( "transparency value", POINTER, &tmp));

		 }

	}

	return 1;

}







/*

 *  FUNCTION

 *	imXpmGetRgb

 *

 *  DESCRIPTION

 *	Figure out r,g,b values based on a text line.

 *

 */

static int /* returns status */

#ifdef __STDC__

imXpmGetRgb(char* buffer, unsigned char *red, unsigned char *green, unsigned char *blue, int* transparent)

#else

imXpmGetRgb(buffer, red, green, blue, transparent)

char* buffer;

unsigned char* red;

unsigned char* green;

unsigned char* blue;

int* transparent;

#endif

{

	char* colorName;	/* name of the color */

	int redInt, blueInt, greenInt; /* color values */



	*transparent = 0;

	/*

	 * The string should look something like this:

	 *

	 *  "<tab>c red"

	 *  This means that the color is red.

	 *  The 'c' indicates that a color is next.  A 'c' may

	 *  also be followed by an rgb value, as follows:

	 *

	 *  "<tab>c #AA00AB"

	 *  This indicates 0xaa red, 0x00 blue, 0xab green.

	 *

	 *  The 'c' may be replaced by a ..

	 *	'g' to indicate >4 bit grayscale

	 *	'g4' to indicate 4-bit grayscale

	 *	'm' to indicate mono

	 *	's' to indicate a name (which we don't care about)

	 *

	 *  More than one of the above flags may be given.

	 *  We'll check the string in the order given above

	 *  ('c', 'g', 'g4', 'm'), and use the first thing we find.

	 *

	 *  There is no support for the g4 flag here.  (I have yet

	 *  to see an image with ONLY the g4 flag, and no 'c' flag.)

	 *  If somebody wants to add g4 support, then the strHasFlag

	 *  routine must be duplicated, and modified to check for a

	 *  flag of length 2.

	 */

	if (colorName=strHasFlag(buffer,'c'))

	{

		/* We have a color */

		if (colorName[0] == '#')

		{

			sscanf(colorName,"#%2x%2x%2x",&redInt, &greenInt, &blueInt);

			*red = (unsigned char)redInt;

			*green = (unsigned char)greenInt;

			*blue  = (unsigned char)blueInt;

			*transparent = 0;

			return 1;

		}

		else

			return imXpmLookUpRgb(colorName,red,green,blue,transparent);

	}



	if (colorName=strHasFlag(buffer,'g'))

	{

		/* We have something like "sgi grey 2" */

		if (colorName[0] == '#')

		{

			sscanf(colorName,"#%2x%2x%2x",&redInt, &greenInt, &blueInt);

			*red = (unsigned char)redInt;

			*green = (unsigned char)greenInt;

			*blue  = (unsigned char)blueInt;

			*transparent = 0;

			return 1;

		}

		else

			return imXpmLookUpRgb(colorName,red,green,blue,transparent);



	}



	if (colorName=strHasFlag(buffer,'m'))

	{

		/* we hopefully have black or white */

		if (colorName[0] == '#')

		{

			sscanf(colorName,"#%2x%2x%2x",&redInt, &greenInt, &blueInt);

			*red = (unsigned char)redInt;

			*green = (unsigned char)greenInt;

			*blue  = (unsigned char)blueInt;

			*transparent = 0;

			return 1;

		}

		else

			return imXpmLookUpRgb(colorName,red,green,blue,transparent);



	}



	ImErrorFatal("Couldn't parse line!", -1, IMESYNTAX);

}





#define IS_WHITESPACE(s)	((s)==' ' || (s)=='\t')

#define IS_SPECIAL_CHAR(xxx)	((xxx)=='m' || (xxx)=='s' || (xxx)=='g' || (xxx)=='c')



/*

 *  FUNCTION

 *	strHasFlag

 *

 *  DESCRIPTION

 *	Check to see if a string has a given character,

 *  surrounded by whitespace.  If it does, return the 

 *  entry that follows it.

 * 

 *  The string must be null terminated for this to 

 *  work.

 */

static char*

#ifdef __STDC__

strHasFlag(char* str,char c)

#else

strHasFlag(str , c)

char* str;

char c;

#endif

{

	char* ptr;	/* points into the string */

	int found = 0;  /* means we found it      */

	static char *ret = NULL; /* return this          */



	if (ret==NULL)

	{

		ImMallocRetOnError(ret, char *, 100,NULL);

	}

	ptr = str;

	while (*ptr!='\0' && !found)

	{

		/* Advance to the next occurence of c */

		while (*ptr!='\0' && *ptr!=c)

		{

			ptr++;

		}

		if (*ptr==c)

		{

			/* is it surrounded by whitespace? */

			if (ptr==str && IS_WHITESPACE(*(ptr+1)) )

				found = 1;

			if (IS_WHITESPACE(*(ptr-1)) && IS_WHITESPACE(*(ptr+1)))

				found = 1;

		}

		if (!found && *ptr!='\0')

			ptr++;

	}

	if (!found)

		return NULL;  /* boo hoo */



	/*

	 * Copy the next word into a new string.  Return that.

	 * Well, we don't really want to copy just the next word.

	 * There are colors named "dark slate grey" and "sgi gray 0".  

	 * What a pain.

	 * So, we'll copy the next words that occur, as long as

	 * the words are not one of { "m", "s", "g4", "g", "c" }.

	 */

	 strcpy(ret, ptr+2);	

	 strcat(ret,"      ");  /* for checking past the end */

	 ptr = ret;

	 while (*ptr!='\0')

	 {

		ptr++;

		if (*ptr=='"')

			*ptr = '\0';



		/*

		 * Check for m,s,g,c 

		 */

		if (IS_WHITESPACE(*ptr) && IS_WHITESPACE(*(ptr+2)) 

			&& IS_SPECIAL_CHAR(*(ptr+1))) 

			*ptr = '\0';



		/*

		 * Check for g4

		 */

		if (IS_WHITESPACE(*ptr) && IS_WHITESPACE(*(ptr+3)) &&

		   *(ptr+1)=='g' && *(ptr+2)=='4')

			*ptr = '\0';

	 }



	 /*

	  * Take any whitespace off of the end of the word

	  */

	  ptr--;

	  while (IS_WHITESPACE(*ptr))

	  {

		*ptr = '\0';

		ptr--;

	  }

	return ret;

}





/*

 *  FUNCTION

 *	imXpmLookUpRgb

 *

 *  DESCRIPTION

 *	Figure out the r,g,b values based on the name of the color.

 *

 */

static int /* returns status */

#ifdef __STDC__

imXpmLookUpRgb(char *colorName,unsigned char*red,unsigned char *green,unsigned char *blue, int* transparent)

#else

imXpmLookUpRgb(colorName,red,green,blue,transparent)

char* colorName;

unsigned char* red;

unsigned char* green;

unsigned char* blue;

int* transparent;

#endif

{

	static struct imXpmRecordStruct *rgbList=NULL; /* List that we read in */

	static int first_time = 1;	/* First time in this routine? */

	imXpmColor	index;	/* One color entry */

	char message[500];	/* message buffer  */

	char filename[1024];	/* Name of file with color names */

	char* env_var;		/* environment variable          */

	FILE* fp;		/* file pointer    */

	int nColors=0;		/* How many colors we read in */

	char str[100];		/* string we read in */

	char tmpStr[100];	/* Holds the name for a sec */

	int tmpRed,tmpGreen,tmpBlue; /* holds these as ints */



	/*

	 * Well, here's how we do this.

	 *

	 * On the first pass through this routine, we'll load into memory

	 * a table with all of the color names and rgb values.

	 *

	 * On subsequent passes we'll look stuff up.

	 *

	 * We'll store the rgb value information in a tagTable.

	 *

	 * Where do we get the information?

	 * 1. Check for the environment variable 'IM_XPM_COLORTABLE'

	 *    If it is set, use that filename instead of /usr/lib/X11/rgb.txt.

	 * 2. Look in /usr/lib/X11 for rgb.txt.

	 *

	 * There are some very annoying things about rgb colornames.

	 * Most significantly, the rgb.txt file is often different

	 * from the names of the colors in the rgb.dir and rgb.pag

	 * files.  If you don't believe me, type 'showrgb'.  This'll

	 * show you the names of the colors in the database.  Then look

	 * through rgb.txt.  Capital letters and spaces are thrown around

	 * like mashed potatoes in a grade school cafeteria.

	 *

	 * The database contained in imxpm.h came from an execution of

	 * the showrgb command.  Hopefully by looking for rgb.txt we'll

	 * be okay...  If we really wanted to be thorough, we'd use the

	 * dbm library routines to read the rgb.dir file.  But, these

	 * routines seem to be on their way to obsolesence.

	 *

	 */



	 /*

	  * Is this the first pass?

	  * If so, load in the color list.

	  */

	 if (first_time==1)

	 {

		first_time = 0;



		/* Get environment variable. */

		env_var = getenv("IM_XPM_COLORTABLE");

		if (env_var)

			strcpy(filename,env_var);

		else

			strcpy(filename,"/usr/lib/X11/rgb.txt");



		/* Load list */

		fp = fopen(filename,"rb");

		if (fp)

		{

			/*

			 * We don't know the size of the file ahead of time,

			 * and we want to read into an array.

			 *

			 * Rather than use a complicated memory scheme, we'll

			 * just read the file twice.  Once to see how big it is,

			 * the second time to read the stuff in.

			 */



			 nColors = 0;

			 while (!feof(fp))

			 {

				fgets(str, 100, fp);

				nColors++;

			 }

			ImMalloc(rgbList, struct imXpmRecordStruct *,(nColors+1)* sizeof(struct imXpmRecordStruct));



			/* Okay, now reopen the file, and read the stuff in */

			fclose(fp);

			index = rgbList;

			fp = fopen(filename,"rb");

			while (!feof(fp))

			{

				fgets(str,100,fp);

				if (str[0]!='\0')

				{

					sscanf(str,"%d %d %d %[^\n]",

						&tmpRed,

						&tmpGreen,

						&tmpBlue,

						tmpStr);

					index->red = tmpRed;

					index->green = tmpGreen;

					index->blue  = tmpBlue;

					ImMalloc(index->name,char *, sizeof(char) * (strlen(tmpStr)+1));

					strcpy(index->name,tmpStr);

					index++;

				}

			}

			/* Set last entry to NULL */

			index->name = NULL;

		} /* End of if-fp */

	 }	/* End of if-first time */



	 /*

	  * First check external list for color name

	  */



	  if (rgbList)

	  {

		 index = rgbList;

		 while (index->name != NULL && strcmp(index->name,colorName)!=0)

		 {

			index++;

		 }

		 if (index->name!=NULL)

		 {	/* Found in file */

			*red = index->red;

			*green = index->green;

			*blue  = index->blue;

			 if (strcmp(colorName,"None")==0)

				*transparent = 1;

			 else

				*transparent = 0;

			 return 1;

		}

	 }



	  /*

	   * Check internal list for color name

	   */



	 index = imXpmColorList;

	 while (index->name != NULL && strcmp(index->name,colorName)!=0)

	 {

		index++;

	 }

	 if (index->name==NULL)

	 {	/* Couldn't find it */

		sprintf(message,"Unknown color: '%s'",colorName);

		ImErrorFatal( message, -1, IMEUNSUPPORTED);

	 }

	*red   = index->red;

	*green = index->green;

	*blue  = index->blue;

	if (strcmp(colorName,"None")==0)

		*transparent = 1;

	else

		*transparent = 0;

	return 1;

}









/*

 *  FUNCTION

 *	imXpmWrite	-  write an X11 pixmap file

 *

 *  DESCRIPTION

 *	The X11 pixmap header and image content are written out.

 */



static int				/* Returns # of entries used	*/

#ifdef __STDC__

imXpmWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imXpmWrite( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int            ioType;		/* Type of I/O to perform	*/

	int            fd;		/* Output file descriptor	*/

	FILE          *fp;		/* Output file pointer		*/

	TagTable      *flagsTable;	/* Flags			*/

	TagTable      *tagTable;	/* Table of info to write	*/

#endif

{

	char		 message[300];	/* various messages		*/

	int xSize, ySize;		/* dimensions			*/

	ImVfb* vfb;			/* a very furry buffalo		*/

	ImVfbPtr vfbPtr;		/* points to a vfb		*/

	ImClt* clt;			/* a clt			*/

	ImCltPtr cltPtr;		/* points to a clt		*/

	char var_name[100];		/* Name of the variable (in xpm header) */

	int nColors;			/* number of colors in the clt  */

	char** cltCodes;		/* list of codes for indexes    */

	int i,j;			/* indexes			*/

	int transparency_color=-1;	/* transparent color 		*/

	char c,d;			/* characters we loop with      */

	char colorName[100];		/* name of a color		*/

	ImHotSpotPtr hotspotptr;	/* points to a hotspot		*/

	int xHot, yHot;			/* hot spot values		*/

	

	ImInfo("Type","8-bit Color Indexed");

        TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );

	clt = ImVfbQClt( vfb );



	if (ioType & IMFILEIOFD)

	{	/* I prefer an fp, thank you very much */

		if ( (fp = fdopen( fd, "rb" )) == NULL )

		{

			ImErrNo = IMESYS;

			return ( -1 );

		}

	}



	nColors = ImCltQNColors(clt);

	xSize = ImVfbQWidth(vfb);

	ySize = ImVfbQHeight(vfb);



	sprintf(message,"%d x %d",xSize,ySize);

	ImInfo("Resolution",message);

	sprintf(message,"%d entries",nColors);

	ImInfo("Color Table",message);

	ImInfo("Compression Type","none");

	ImInfo("Alpha Channel","none");

	fprintf(fp,"/* XPM */\nstatic char*%s[] = {\n",var_name);

	if (TagTableQNEntry(tagTable, "image hot spot")==0)

	{

		/* no hot spot */

		fprintf(fp,"/* width height ncolors chars_per_pixel */\n");

		fprintf(fp,"\"%d %d %d 2\",\n",xSize,ySize,nColors);

	}

	else

	{

		/* there is a hot spot */

		fprintf(fp,"/* width height ncolors chars_per_pixel hotspotx hotspoty*/\n");

		TagEntryQValue( TagTableQDirect( tagTable, "image hot spot", 0),

			&hotspotptr);

		xHot = ImHotSpotQX( hotspotptr );

		yHot = ImHotSpotQY( hotspotptr );

		sprintf(message,"%d, %d",xHot, yHot);

		ImInfo("Hot Spot",message);

		fprintf(fp,"\"%d %d %d 2 %d %d\",\n",xSize,ySize,nColors,xHot, yHot);



	}







	fprintf(fp,"/* colors */\n");



	/*

	 * Print out the clt

	 */



	/* 

	 * For each entry in the clt we want to make a new two character

	 * code.  Then we want to print this code, followed by 'c <colorname>'.

	 * 

	 * We also need to store this code at the array index corresponding to

	 * it's color entry in the clt for later reference.

	 */

	 ImMalloc(cltCodes, char**, nColors * sizeof(char *) ); /* list of strings of length 2 */

	 for (i=0; i< nColors; i++)

	 {

		ImMalloc(cltCodes[i], char*, 3*sizeof(char));

	 }



	 /*

	  * Fill up the array of codes.  If we have a transparent color,

	  * make that "  ", since then the text file will look cool.

	  */

	  transparency_color = ImGetTransparency(tagTable, flagsTable, vfb);

	  if (transparency_color!=-1)

	  {

		sprintf(message,"Pixels with index %d.",transparency_color);

		ImInfo("Transparency",message);

	  }

	  c = 'a';

	  d = 'a';

	  cltPtr = ImCltQFirst(clt);

	  for (i=0;i<nColors;i++)

	  {

		if (i==transparency_color)

		{

			cltCodes[i][0] = ' ';

			cltCodes[i][1] = ' ';

			cltCodes[i][2] = '\0';

		}

		else

		{

			cltCodes[i][0] = c;

			cltCodes[i][1] = d;

			cltCodes[i][2] = '\0';

			c++;

			if (c=='z')

			{

				d++;

				c = 'a';

			}

		}

		

		if (i==transparency_color)

		{

			sprintf(message,"\"%s c None\",\n",

				cltCodes[i]);

		}

		else

		{

			sprintf(colorName,"#%02X%02X%02X",

				ImCltQRed(cltPtr),

				ImCltQGreen(cltPtr),

				ImCltQBlue(cltPtr));

			sprintf(message,"\"%s c %s\",\n",

				cltCodes[i], colorName);

		}

		fprintf(fp,"%s",message);

		if (i<nColors)

			ImCltSInc(clt, cltPtr);

	  }



	/*

	 * We're done with the clt.  Now print out the pixels.

	 */

	vfbPtr = ImVfbQFirst(vfb);



	 fprintf(fp,"/* pixels */\n");

	 for (i=0; i<ySize; i++)

	 {   /* Line number i */

		fprintf(fp,"\"");

		for (j=0;j<xSize;j++)

		{

			fprintf(fp,"%s",cltCodes[ImVfbQIndex(vfb,vfbPtr)]);

			ImVfbSInc(vfb,vfbPtr);

		}

		fprintf(fp,"\"");

		if (i<ySize-1)

			fprintf(fp,",\n");

	}

	fprintf(fp,"\n};\n");



	return 1;

}







