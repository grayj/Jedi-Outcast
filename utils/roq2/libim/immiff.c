/**

 **	$Header: /roq/libim/immiff.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/immiff.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	immiff.c-  ImageMagick's Machine Independent File Format file i/o

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	immiff.c contains routines to read and write ImageMagick's MIFF

 **   	files for the image manipulation library.  Raster data read in 

 **	is stored in a VFB.  Raster data written out is taken from a tag table.

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

 **	imMiffRead		f  read a MIFF file

 **	imMiffWriteRGB		f  write an RGB MIFF file

 **	imMiffWriteIndex	f  write an Indexed MIFF file

 **

 **  HISTORY

 **	$Log: /roq/libim/immiff.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 * Revision 1.6  1995/06/30  22:09:07  bduggan

 * removed strings.h

 *

 * Revision 1.5  1995/06/29  00:28:04  bduggan

 * updated copyright year

 *

 * Revision 1.4  1995/06/29  00:20:26  bduggan

 * changed comment

 *

 * Revision 1.3  1995/06/15  20:10:59  bduggan

 * took out some useless vars

 *

 * Revision 1.2  1995/05/17  23:45:35  bduggan

 * Used library 'imSameRGB' calls instead of local miff calls

 *

 ** Revision 1.1  1995/04/03  21:29:34  bduggan

 ** Initial revision

 **

 **	

 **/



#include <string.h>

#include "iminternal.h"







/**

 **  FORMAT

 **	MIFF	- Magick Image File Format

 **

 **  AKA

 **	(none)

 **

 **  FORMAT REFERENCES

 **  	Anonymous ftp at ftp.x.org, 

 **	file: contrib/applications/ImageMagick/ImageMagick-3.6.tar.gz	

 **

 **  CODE CREDITS

 **     Brian Duggan, Custom Development, San Diego Supercomputer Center, 1995.

 **

 **  DESCRIPTION

 **	MIFF files may contain multiple images.  To store multiple images,

 ** 	one can simply create several MIFF's, then concatenate them together.

 **	A single MIFF file contains a header, followed by a clt (optional), 

 **	followed by pixel data.

 **

 **  Header

 **  ------

 **	The header is text based.  The idea is that if you `more` a 

 **	.miff, you'll be able to see what type of file it is.

 **	

 **	A miff header consists of labels of the form, "field=value",

 **	and comments contained in curly braces ({}).  The header ends

 **	with a colon (:) followed by a newline character.

 **

 **	By convention, a formfeed character and a newline character

 **	appear before the colon.  (Hence, using 'more' will pause

 **	before the binary data.)

 **

 **	The header fields are as follows:

 **		id=ImageMagick

 **			This indicates that we have a MIFF file.

 **			This field is mandatory.

 **	

 **		class=<class>

 **		        <class> can be one of:

 **				DirectClass  (RGB Image)

 **				PseudoClass  (Indexed Image)

 **			If this field is omitted, the image is RGB.

 **

 **		colors=<size of clt>

 **			If this is not present, there is not clt.

 **			(i.e. the image is RGB or grayscale)

 **			If this is present and the image is RGB, then

 **			this field is supposed to contain the number

 **			of colors in the image.

 **

 **		columns=<image width>

 **		rows=<image height>

 **			These fields are required.

 **

 **		compression=<compression type>

 **			<compression type> can be one of

 **				RunlengthEncoded

 **				QEncoded

 **			The RLE algorithm for RunlengthEncoded images is explained below.

 **			The algorithm for QEncoded images is similar to JPEG encoding.  It

 **			is not supported here.

 **			If this field is omitted, there is no compression.

 **

 **		matte=<value>

 **			if <value> is True, we have an alpha channel.

 **			Otherwise, we don't.  Color indexed images

 **			can't have alpha channels.

 **

 **		packets=<value>  

 **			This is used for QEncoding.  We ignore it.

 **

 **		montage=<width>x<height>{+-}<x offset>{+-}<y offset>

 **			This is miff's handy way of storying a storyboard

 **			of pictures as one file, while seperating

 **			them within the file.  It is not supported here.

 **

 **		scene=<value>

 **		signature=<value>

 **			These fields are used when a miff is part of

 **		        a sequence of pictures, and hence may have

 **			a CLT stored in a different frame.  These

 **			options are not supported here.

 **

 **	Sample MIFF header:

 **	{

 **		This is a color indexed image.

 **	}

 **	id=ImageMagick

 **	class=PseudoClass colors=2

 **	compression=RunlengthEncoded

 **	columns=768 rows=640

 **	<FF>

 **	:

 **	Notice that fields are seperate by spaces and/or

 **	carriage returns, and may occur in any order.

 **

 **	Pixel Storage

 **	-------------

 **	If there is no compression, the pixels are simply stored

 **	as RGBRGB or RGBARGBA or IIIII (I = index).  If there is

 **	RLE encoding then the following scheme is used:

 **			

 **	RLE encoding scheme:

 **

 **	For RGB images, the data are encoded in 4 byte packages.

 **	(or 5 bytes if there is an alpha channel.)  The first three

 **	(or four) bytes contain the data for a pixel.  The fourth

 **	(fifth) byte contains one less than the number of pixels in 

 **	the run.  This value is in the range 0-255.

 **	

 **	For instance  4A 0D 00 05  indicates a run of six pixels

 **	with red component 4A, green component 0D, and blue component

 **	00.

 **

 **	The same scheme is used for encoding color-indexed images.

 **	The number of bytes in a package is either two or three

 **	depending on whether this is a 16-bit or an 8-bit color

 **	indexed image.

 **

 **	Storage of the color lookup table:

 **	

 **	The CLT is stored directly after the header (if there

 **	is a CLT).  It is simply a series of R, G, and B values.

 **	The number of triplets is indicated by the "colors" field

 **	in the header.

 **/



/*

 *  MIFF - ImageMagick's MIFF file format

 *      For information on these structures, how to use them, etc. please

 *      see imfmt.c.

 */



#ifdef __STDC__

static int imMiffRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imMiffWriteRGB( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable);

static int imMiffWriteRawRGB( ImVfb* vfb, int fd, FILE* fp, int numChans, int ioType);

static int imMiffWriteRLERGB( ImVfb* vfb, int fd, FILE* fp, int ioType);

static int imMiffWriteRLERGBA( ImVfb* vfb, int fd, FILE* fp, int ioType);

static int imMiffWriteRawIndex8(ImVfb* vfb, int fd, FILE* fp, int ioType);

static int imMiffWriteRawIndex16(ImVfb* vfb, int fd, FILE* fp, int ioType);

static int imMiffWriteRLEIndex8(ImVfb* vfb, int fd, FILE* fp, int ioType);

static int imMiffWriteRLEIndex16(ImVfb* vfb, int fd, FILE* fp, int ioType);

static int imMiffWriteIndex( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable);

static char* imMiffGetHeaderField( char* header, char* field);

static ImClt * imMiffReadClt( int ioType, int fd, FILE* fp, int cltSize );

static ImVfb*  imMiffReadRLE(int ioType, int fd, FILE* fp, int width,int height,int numChans, int chanDepth);

static ImVfb*  imMiffReadRaw(int ioType, int fd, FILE* fp, int width,int height,int numChans, int chanDepth);

static char* strToLower( char* str);



#else



static int imMiffRead( );

static int imMiffWriteRGB( );

static char* imMiffGetHeaderField( );

static ImClt * imMiffReadClt( );

static ImVfb*  imMiffReadRLE( );

static ImVfb*  imMiffReadRaw( );

static char* strToLower( );

static int imMiffWriteIndex();

static int imMiffWriteRawRGB( );

static int imMiffWriteRLERGB( );

static int imMiffWriteRLERGBA( );

static int imMiffWriteRLEIndex8();

static int imMiffWriteRLEIndex16();

static int imMiffWriteRawIndex8();

static int imMiffWriteRawIndex16();

#endif



static char *imMiffNames[ ]  = { "miff", NULL };

static ImFileFormatReadMap imMiffReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { IN,1,8,       0,      IMVFBINDEX8,    0 },

        { IN,1,8,       RLE,    IMVFBINDEX8,    0 },

        { IN,1,8,       C,      IMVFBINDEX8,    0 },

        { IN,1,8,       RLE|C,  IMVFBINDEX8,    0 },

        { IN,1,16,      C,      IMVFBINDEX16,   0 },

        { IN,1,16,      RLE | C,IMVFBINDEX16,   0 },

        { RGB,3,8,      0,      IMVFBRGB,       0 },

        { RGB,3,8,      RLE,    IMVFBRGB,       0 },

	{ RGB,4,8,	A,	IMVFBRGB,	A },

	{ RGB,4,8,	RLE|A,	IMVFBRGB,	A },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imMiffWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */

	{ IMVFBRGB,	0,	RGB,3,8,        RLE,    imMiffWriteRGB},

	{ IMVFBRGB,	A,	RGB,4,8,        A|RLE,  imMiffWriteRGB},

	{ IMVFBRGB,	0,	RGB,3,8,        0,      imMiffWriteRGB},

	{ IMVFBRGB,	A,	RGB,4,8,        A,      imMiffWriteRGB},

	{ IMVFBINDEX8,	0,	IN,1,8,         RLE,    imMiffWriteIndex},

	{ IMVFBINDEX8,	C,	IN,1,8,         C|RLE,  imMiffWriteIndex},

	{ IMVFBINDEX8,	0,	IN,1,8,         0,      imMiffWriteIndex},

	{ IMVFBINDEX8,	C,	IN,1,8,         C,      imMiffWriteIndex},

	{ IMVFBINDEX16,	C,	IN,1,16,        C|RLE,  imMiffWriteIndex},

	{ IMVFBINDEX16,	C,	IN,1,16,        C,      imMiffWriteIndex},

        { -1,           0,      -1,             0,      NULL },

};



static ImFileMagic imFileMiffMagic []=

{

	{ 0, 0, NULL },

};



ImFileFormat ImFileMiffFormat =

{

	imMiffNames, "ImageMagick's MIFF file format",

	"John Cristy",

	"8 or 16-bit color index with optional CLT and 24-bit RGB color images with optional alpha channels,\n\
uncompressed (verbatim) and RLE-compressed.",
	"8 or 16-bit color index with optional CLT and 24-bit RGB color images with optional alpha channels,\n\
uncompressed (verbatim) and RLE-compressed.",

	imFileMiffMagic,

	IMMULTI, IMPIPE,  

	IMMULTI, IMPIPE,

	imMiffRead, imMiffReadMap, imMiffWriteMap

};







/*

 *  FUNCTION

 *	imMiffRead	-  Read a MIFF file

 *

 *  DESCRIPTION

 *	The file header is read and the size of the image determined.

 *	Space is allocated for the VFB and the run codes read in to

 *	a run buffer.  The run buffer is then expanded into straight

 *	RGB values in the VFB and the completed VFB added to the tag list.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__

imMiffRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imMiffRead( ioType, fd, fp, flagsTable, tagTable )

	int         ioType;		/* I/O flags			*/

	int         fd;			/* Input file descriptor	*/

	FILE	   *fp;			/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable   *tagTable;		/* Tag table to add to		*/

#endif

{

	char *header;		/* Contains the (text) header          */

	char c;         	/* Contains a single char              */

	int skippingComment;	/* flag that says we're skipping stuff */

	int index;    		/* index into character array          */

	char* fieldValue;       /* value of a field in the header      */

	int numChans; 		/* 1,3 or 4 (Indexed, rgb or rgba)     */

	int chanDepth;          /* 8 or 16  (16 for index16)           */

	int compression;        /* IMCOMPNONE or IMCOMPRLE             */

	int width, height;      /* Dimensions                          */

	int cltSize;            /* 0 means no CLT                      */

	char message[300];      /* Holds message                       */

	ImVfb *vfb;             /* new vfb                             */

	ImClt* clt;             /* new clt                             */

	int moreImages;         /* flag saying to continue reading     */

	int numImages;          /* number of images read               */

	int retVal;             /* return value from BinRead call      */



	/*

	 * Read in the header.

	 */

	ImMalloc (header, char* , 1024*sizeof(char));

	numImages = 0;

	moreImages = 1;



	while (moreImages)

	{

		index = 0;

		c = '\0';

		skippingComment = 0;

		/*

		 * Read the next character if ...

		 *    we haven't read a :, and we're not inside a comment block

		 *    OR

		 *    we are inside a comment block 

		 *

		 * This loop reads in anything that is not contained between two

		 * curly braces.  (It also skips any left curly braces.)

		 */

		while ( (c!=':' && !skippingComment) || skippingComment)

		{

			if( (retVal = ImBinRead( ioType,fd,fp,&c,CHAR,sizeof(char),1))<0)

				ImReturnBinError( );

			if (retVal==0)

				break;

			if (skippingComment==1 && c=='}')

				skippingComment = 0;

			if (c=='{')

				skippingComment = 1;

			if (skippingComment==0 && c!='}')

			{       /* Add this character to our array */

				header[index++] = c;

				if (index >= 1024)

				{

					/* This'll only happen if there

					 * are an extraordinary number of

					 * whitespace characters in the

					 * header, or if there are a lot

					 * of illegal fields.  We shan't

					 * cater to such miff files.

					 */

					ImErrorFatal(ImQError(), -1, IMEMALLOC);

				}

			}



		}

		header[index] = '\0';

		if (retVal==0)

			break;



		/*

		 * We just read in a ':'.  Read in a newline character,

		 * then we'll be done with the header.

		 */

		if( ImBinRead( ioType,fd,fp,&c,CHAR,sizeof(char),1)<=0)

			ImReturnBinError( );



		numImages++;  /* If we made it this far, hopefully there's an image. */

		if (numImages > 1)

		{

			sprintf(message,"%d",numImages);

			ImInfo ("Image", message);

		}



		/*

		 * Get the image dimensions

		 */

		fieldValue = imMiffGetHeaderField(header,"rows");

		if (fieldValue==NULL)

		{

			ImErrorFatal (ImQError( ), -1, IMENOIMAGE);

		}

		height = atoi(fieldValue);



		fieldValue = imMiffGetHeaderField(header,"columns");

		if (fieldValue==NULL)

		{

			ImErrorFatal (ImQError( ), -1, IMENOIMAGE);

		}

		width = atoi(fieldValue);



		sprintf(message,"%d x %d",width, height);

		ImInfo("Resolution",message);



		fieldValue = imMiffGetHeaderField(header,"class");

		if (fieldValue==NULL || strcmp(fieldValue,"directclass")==0)

		{

			ImInfo ("Type","24-bit RGB");



			/* Is there an alpha channel ? */

			fieldValue = imMiffGetHeaderField(header,"matte");

			if (fieldValue && strcmp(fieldValue,"true")==0)

			{

				numChans = 4;

				ImInfo ("Alpha Channel","8-bit");

			}

			else

			{

				numChans = 3;

				ImInfo ("Alpha Channel","none");

			}

			chanDepth = 8;



			/* Can't a have a clt with an RGB image */

			cltSize = 0;

			ImInfo("Color Table", "none");

		}

		else

		{

			/* Color indexed */

			numChans = 1;



			/* Are we 8 or 16 bit? */

			fieldValue = imMiffGetHeaderField(header,"colors");

			if (fieldValue==NULL)

				cltSize = 0;

			else

				cltSize = atoi(fieldValue);



			if (cltSize > (1<<8))

				chanDepth = 16;

			else

				chanDepth = 8;



			sprintf(message,"%d-bit color indexed",chanDepth);

			ImInfo ("Type",message);



			/* Do we have a clt? */

			if (cltSize==0)

			{

				ImInfo("Color Table","none");

			}

			else

			{

				sprintf(message,"%d entries",cltSize);

				ImInfo("Color Table", message);

			}

		}



		/* 

		 * Is there compression?

		 */

		fieldValue = imMiffGetHeaderField(header,"compression");

		if (fieldValue==NULL)

		{

			compression = IMCOMPNONE;

			ImInfo ("Compression","none");

		}

		else

		if (strcmp(fieldValue,"runlengthencoded")==0)

		{

			compression = IMCOMPRLE;

			ImInfo ("Compression","Run Length Encoded");

		}

		else

		{

			ImErrorFatal ("Unsupported compression scheme", -1, IMENOIMAGE);

		}





		/*

		 * Read the clt if there is one

		 */

		if (cltSize!=0)

		{

			clt = imMiffReadClt( ioType, fd, fp, cltSize );

			if (clt==IMCLTNULL) /* error */

				return 0;

		}

		else

			clt = IMCLTNULL;



		/* Read the vfb! */

		if (compression==IMCOMPRLE)

		{

			vfb = imMiffReadRLE(ioType, fd, fp, width, height, numChans, chanDepth);

		}

		else

		{

			vfb = imMiffReadRaw(ioType, fd, fp, width, height, numChans, chanDepth);

		}

		if (vfb==IMVFBNULL)

			return 0;



		if (clt!=IMCLTNULL)

		{

			ImVfbSClt (vfb, clt);

			TagTableAppend( tagTable, TagEntryAlloc( "image clt", POINTER, &clt));

		}

		else

		{

			ImVfbSClt (vfb, IMCLTNULL);

		}



		TagTableAppend( tagTable, TagEntryAlloc( "image vfb", POINTER, &vfb));

		/*

		 * Are there any more images ?

		 */

		 moreImages = 1;  /* Yes.  The only way to exit the loop is by */

				  /* failing to read the header.               */

	}  /* End of while-more images */



	return numImages;

}





/*

 * FUNCTION

 * 	imMiffGetHeaderField

 *

 * DESCRIPTION

 *	Parse the string, looking for something of

 * the form field=value.  Return value (in lower case).

 * If field is not found, return NULL.

 */



static char*  /* returns value for this field in lower case */

#ifdef __STDC__

imMiffGetHeaderField( char* header, char* field)

#else

imMiffGetHeaderField( header, field)

char* header;

char* field;

#endif

{

	char* ptr;   /* points into the string */

	static char ret[100];   /* string to return       */



	if (header==NULL) return NULL;

	if (field==NULL)  return NULL;



	ptr = strstr(header,field);



	if (ptr==NULL)

		return NULL;

	ptr += strlen(field);



	if ((*ptr)!='=') return NULL;  /* need an '=' */

	sscanf (ptr+1,"%s",ret); /* Read the next word */



	return strToLower(ret);

}



/*

 * FUNCTION

 *   imMiffReadClt

 *

 * DECSRIPTION

 *   Read in the clt from a MIFF image file

 */



static ImClt * /* Returns a brand new clt */

#ifdef __STDC__

imMiffReadClt( int ioType, int fd, FILE* fp,int cltSize )

#else

imMiffReadClt( ioType, fd, fp, cltSize )

int ioType;

int fd;

FILE* fp;

int cltSize;

#endif

{

	ImClt* clt;          /* Our new clt           */

	ImCltPtr cltPtr;     /* points at our new clt */

	unsigned char* rbuf; /* holds the clt data    */

	unsigned char* rbufptr; /* points into rbuf   */

	int i;               /* loop index            */



	clt = ImCltAlloc (cltSize);



	/* Read in the values */

	cltPtr = ImCltQFirst(clt);

	ImMallocRetOnError (rbuf, unsigned char*, 3 * cltSize, IMCLTNULL);

	rbufptr = rbuf;

	if ( ImBinRead (ioType, fd, fp, rbuf, UCHAR, 1, 3 * cltSize) == -1)

	{

		ImReturnValBinError( IMCLTNULL );

	}



	/*

	 * Now we've read in the data.  Put it in the clt.

	 */

	for (i=0; i<cltSize; i++)

	{

		ImCltSRed   ( cltPtr, *rbufptr++);

		ImCltSGreen ( cltPtr, *rbufptr++);

		ImCltSBlue  ( cltPtr, *rbufptr++);

		cltPtr = ImCltQNext( clt, cltPtr );

	}

	free (rbuf);

	return clt;

}







/*

 * FUNCTION

 *     imMiffReadRLE

 *

 * DESCRIPTION

 *     Read an RLE encoded MIFF vfb

 */



static ImVfb*   /* Returns a new vfb */

#ifdef __STDC__

imMiffReadRLE(int ioType, int fd, FILE* fp, int width,int height,int numChans, int chanDepth)

#else

imMiffReadRLE(ioType, fd, fp, width, height, numChans, chanDepth)

int ioType;

int fd;

FILE *fp;

int width;

int height;

int numChans;

int chanDepth;

#endif

{

	ImVfb* vfb;            /* new vfb                 */

	ImVfbPtr vfbPtr;       /* points into the new vfb */

	unsigned char rbuf[7]; /* Holds some data         */

	int i;                 /* loop index              */

	unsigned int uintval;   /* holds an index16 value  */



	switch (numChans)

	 {

		case 1 : /* Color indexed */

		    if (chanDepth==8)

		    {  /* index8  */

			if ((vfb=ImVfbAlloc( width, height, IMVFBINDEX8 ))==IMVFBNULL)

			{

				ImErrorFatal (ImQError( ), IMVFBNULL, ImErrNo);

			}



			/* Read packets of size 2 */



			vfbPtr  = ImVfbQPtr( vfb, 0, 0);

			while ( ImBinRead (ioType, fd, fp, rbuf, UCHAR, 1, 2) > 0)

			{

				for (i=0; i < rbuf[1] + 1; i++)

				{

					ImVfbSIndex8( vfb, vfbPtr, rbuf[0]);

					vfbPtr = ImVfbQNext( vfb, vfbPtr);

				}

			}

		    }

		    else

		    {  /* index16 */

			if ((vfb=ImVfbAlloc( width, height, IMVFBINDEX16 ))==IMVFBNULL)

			{

				ImErrorFatal (ImQError( ), IMVFBNULL, ImErrNo);

			}

			vfbPtr  = ImVfbQPtr( vfb, 0, 0);



			/* Read packets of size 3 */



			while ( ImBinRead (ioType, fd, fp, rbuf, UCHAR, 1, 3) > 0)

			{

				uintval = rbuf[1];

				uintval |= rbuf[0] << 8;

				for (i=0; i < rbuf[2] + 1; i++)

				{  

					ImVfbSIndex16( vfb, vfbPtr, uintval);

					vfbPtr = ImVfbQNext( vfb, vfbPtr);

				}

			}

		    }  /* End of index16 */

		    break;



		case 3 : /* RGB */

			if ((vfb=ImVfbAlloc( width, height, IMVFBRGB ))==IMVFBNULL)

			{

				ImErrorFatal (ImQError( ), IMVFBNULL, ImErrNo);

			}

			vfbPtr  = ImVfbQPtr( vfb, 0, 0);



			/* Read packets of size 4 */



			while ( ImBinRead (ioType, fd, fp, rbuf, UCHAR, 1, 4) > 0)

			{  

				for (i=0;i<rbuf[3]+1;i++)

				{

					ImVfbSRed  ( vfb, vfbPtr, rbuf[0]);

					ImVfbSGreen( vfb, vfbPtr, rbuf[1]);

					ImVfbSBlue ( vfb, vfbPtr, rbuf[2]);

					vfbPtr = ImVfbQNext( vfb, vfbPtr);

				}

			}

		    break;



		case 4 : /* RGBA */

			if ((vfb=ImVfbAlloc( width, height, IMVFBRGB|IMVFBALPHA ))==IMVFBNULL)

			{

				ImErrorFatal (ImQError( ), IMVFBNULL, ImErrNo);

			}

			vfbPtr  = ImVfbQPtr( vfb, 0, 0);



			/* Read packets of size 5 */



			while ( ImBinRead (ioType, fd, fp, rbuf, UCHAR, 1, 5) > 0)

			{  

				for (i=0;i<rbuf[4]+1;i++)

				{

					ImVfbSRed  ( vfb, vfbPtr, rbuf[0]);

					ImVfbSGreen( vfb, vfbPtr, rbuf[1]);

					ImVfbSBlue ( vfb, vfbPtr, rbuf[2]);

					ImVfbSAlpha( vfb, vfbPtr, rbuf[3]);

					vfbPtr = ImVfbQNext( vfb, vfbPtr);

				}

			}

		    break;

	 }  /* End of switch */



	return vfb;

}







/*

 * FUNCTION

 *     imMiffReadRaw

 *

 * DESCRIPTION

 *     Read an unencoded MIFF vfb

 */



static ImVfb*  /* returns a new vfb */

#ifdef __STDC__

imMiffReadRaw(int ioType, int fd, FILE* fp, int width,int height,int numChans, int chanDepth)

#else

imMiffReadRaw(ioType, fd, fp, width, height, numChans, chanDepth)

int ioType;

int fd;

FILE *fp;

int width;

int height;

int numChans;

int chanDepth;

#endif

{

	ImVfb* vfb;             /* new vfb                 */

	ImVfbPtr vfbPtr;        /* points into the new vfb */

	unsigned char* rbuf;    /* Holds the data          */

	unsigned char* rbufptr; /* Points at the data      */

	int i;                  /* index                   */

	unsigned int uintval;   /* holds an index16 value  */





	ImCallocRetOnError(rbuf, unsigned char*, numChans * width * height * (chanDepth / 8), sizeof(char), IMVFBNULL);



	if (ImBinRead(ioType, fd, fp, rbuf, UCHAR, 1, 

		numChans * width * height * (chanDepth / 8))==-1)

	{

		ImReturnValBinError( IMVFBNULL );

	}

			

	/* 

	 * rbuf now contains all the data we need. 

	 * Store it in the vfb in the correct manner

	 * depending on what type of vfb we have.

	 */



	/* Initialize rbufptr, and vfbPtr */

	rbufptr = rbuf;

	switch (numChans)

	 {

		case 1 : /* Color indexed */

		    if (chanDepth==8)

		    {  /* index8  */

			if ((vfb=ImVfbAlloc( width, height, IMVFBINDEX8 ))==IMVFBNULL)

			{

				ImErrorFatal (ImQError( ), IMVFBNULL, ImErrNo);

			}

			vfbPtr  = ImVfbQPtr( vfb, 0, 0);

			for (i=0; i < width * height; i++)

			{

				ImVfbSIndex8( vfb, vfbPtr, *rbufptr++);

				vfbPtr = ImVfbQNext( vfb, vfbPtr);

			}

		    }

		    else

		    {  /* index16 */

			if ((vfb=ImVfbAlloc( width, height, IMVFBINDEX16 ))==IMVFBNULL)

			{

				ImErrorFatal (ImQError( ), IMVFBNULL, ImErrNo);

			}

			vfbPtr  = ImVfbQPtr( vfb, 0, 0);

			for (i=0; i < width * height; i++)

			{  

				uintval = *rbufptr++ << 8;

				uintval |= (*rbufptr++);

				ImVfbSIndex16( vfb, vfbPtr, uintval);

				vfbPtr = ImVfbQNext( vfb, vfbPtr);

			}

		    }

		    break;



		case 3 : /* RGB */

			if ((vfb=ImVfbAlloc( width, height, IMVFBRGB ))==IMVFBNULL)

			{

				ImErrorFatal (ImQError( ), IMVFBNULL, ImErrNo);

			}

			vfbPtr  = ImVfbQPtr( vfb, 0, 0);

		        for (i=0; i < width * height; i++)

			{  

				ImVfbSRed  ( vfb, vfbPtr, *rbufptr++);

				ImVfbSGreen( vfb, vfbPtr, *rbufptr++);

				ImVfbSBlue ( vfb, vfbPtr, *rbufptr++);

				vfbPtr = ImVfbQNext( vfb, vfbPtr);

			}

		    break;



		case 4 : /* RGBA */

			if ((vfb=ImVfbAlloc( width, height, IMVFBRGB|IMVFBALPHA ))==IMVFBNULL)

			{

				ImErrorFatal (ImQError( ), IMVFBNULL, ImErrNo);

			}

			vfbPtr  = ImVfbQPtr( vfb, 0, 0);

		        for (i=0; i < width * height; i++)

			{  

				ImVfbSRed  ( vfb, vfbPtr, *rbufptr++);

				ImVfbSGreen( vfb, vfbPtr, *rbufptr++);

				ImVfbSBlue ( vfb, vfbPtr, *rbufptr++);

				ImVfbSAlpha( vfb, vfbPtr, *rbufptr++);

				vfbPtr = ImVfbQNext( vfb, vfbPtr);

			}

		    break;

	 }

 free (rbuf);



 return vfb;

}





/*

 * FUNCTION

 *    imMiffWriteRGB

 *

 * DESCRIPTION

 *     Write an RGB miff file with...

 *	  - RLE or no compression

 *  	  - possibly an alpha channel

 */

static int   /* returns number of tags used */

#ifdef __STDC__

imMiffWriteRGB( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable)

#else

imMiffWriteRGB( pMap, ioType, fd, fp, flagsTable, tagTable)

ImFileFormatWriteMap * pMap;

int ioType;

int fd;

FILE* fp;

TagTable* flagsTable;

TagTable* tagTable;

#endif

{

 ImVfb* vfb; 		/* image vfb           */

 ImVfbPtr vfbptr;       /* points into a vfb   */

 char header[400];	/* header (text based) */

 int x, y;              /* dimensions of vfb   */

 char message[100];     /* another buffer      */

 char* filename;        /* filename            */

 int numChans;          /* 3 or 4 channels.    */

 int numImages;         /* number of images    */

 int curImage;          /* loop index          */



 BinByteOrder( BINMBF );

 numImages = TagTableQNEntry (tagTable, "image vfb");



 for (curImage=0;curImage<numImages;curImage++)

 {

	 /* Get the vfb */

	 TagEntryQValue( TagTableQDirect( tagTable, "image vfb", curImage ), &vfb );

	 x = ImVfbQWidth(vfb);

	 y = ImVfbQHeight(vfb);



	 if (numImages > 1)

	 {

		sprintf(message,"%d of %d",curImage+1, numImages);

		ImInfo("Image",message);

	 }

	 sprintf(message,"%d x %d", x, y);

	 ImInfo ("Resolution", message);

	 ImInfo ("Type", "24-bit RGB");

	 ImInfo ("Color Table","none");

	 if (pMap->map_outAttributes & IMCOMPRLE)

	 {

		ImInfo ("Compression Type", "Run Length Encoded");

	 }

	 else

	 {

		ImInfo ("Compression Type", "none");

	 }

	 if (pMap->map_outAttributes & IMALPHAYES)

	 {

		numChans = 4;

		ImInfo ("Alpha Channel", "8-bit");

	 }

	 else

	 {

		 numChans = 3;

		 ImInfo ("Alpha Channel", "none");

	 }

	 

	 if (!flagsTable || TagTableQNEntry (flagsTable, "file name")==0)

	 {

		filename = NULL;

	 }

	 else

	 {

		TagEntryQValue( TagTableQDirect ( flagsTable, "file name", 0 ), &filename);

	 }



	/*

	 * Write the header

	 */

	 sprintf(header, "{\n    %s\n    %s\n}\nid=ImageMagick\nclass=DirectClass\nrows=%d\ncolumns=%d\n%s%s\f\n:\n",

		filename ? filename : "(untitled)",

		"This file was created by the SDSC Image Tools.",

		y, /* rows    */

		x,  /* columns */

		numChans==4 ? "matte=True\n" : "matte=False\n",

		(pMap->map_outAttributes & IMCOMPRLE) ? "compression=RunlengthEncoded\n" : ""

		);



	 if ( ImBinWrite( ioType, fd, fp, header, CHAR, 1, strlen(header))==-1)

	 {

		ImReturnBinError( );

	 }



	 /*

	  * Write the pixels

	  */

	  if (!(pMap->map_outAttributes & IMCOMPRLE))

	  {

		if (imMiffWriteRawRGB(vfb, fd, fp, numChans, ioType)==-1)

			return -1;

	  }

	  else  /* RLE compress */

	  {

	 	if (pMap->map_outAttributes & IMALPHAYES)

		{

			if (imMiffWriteRLERGBA(vfb, fd, fp, ioType)==-1)

				return -1;

		}

		else

		{

			if (imMiffWriteRLERGB(vfb, fd, fp, ioType)==-1)

				return -1;

		}

	  }

 }



 return numImages;

}



/*

 * FUNCTION

 *     imMiffWriteRawRGB

 *

 * DESCRIPTION

 *     Write the pixels of the vfb without compressing them.

 */

static int /* returns status */

#ifdef __STDC__

imMiffWriteRawRGB( ImVfb* vfb, int fd, FILE* fp, int numChans, int ioType)

#else

imMiffWriteRawRGB( vfb, fd, fp, numChans, ioType)

ImVfb* vfb; 

int fd; 

FILE* fp; 

int numChans; 

int ioType; 

#endif

{

 unsigned char* runBuffer;    /* holds pixels        */

 ImVfbPtr vfbptr;             /* points into a vfb   */

 int runIndex, i,j,x,y;       /* indexes, dimensions */



 x = ImVfbQWidth(vfb);

 y = ImVfbQHeight(vfb);



 ImMalloc( runBuffer, unsigned char *, sizeof (unsigned char) * x * numChans);

 vfbptr = ImVfbQPtr( vfb, 0, 0);

 for (i=0; i<y; i++)  /* i goes through the rows */

 {

	runIndex = 0;

	for (j=0;j<x;j++)  /* Fill a buffer for this row. */

	{

		runBuffer[runIndex++] = ImVfbQRed(vfb, vfbptr);

		runBuffer[runIndex++] = ImVfbQGreen(vfb, vfbptr);

		runBuffer[runIndex++] = ImVfbQBlue(vfb, vfbptr);

		if (numChans == 4)

			runBuffer[runIndex++] = ImVfbQAlpha( vfb, vfbptr);

		vfbptr = ImVfbQNext(vfb,vfbptr);	

	}	

	if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, x * numChans)==-1)

	{

		ImReturnBinError( );

	}

 }

 free(runBuffer);

 return 1;

}





/*

 * MACROS

 *    imMiffDumpRun3

 *    imMiffDumpRun4

 *    imMiffDumpRun1

 *    imMiffDumpRun2

 *

 * DESCRIPTION

 *    Dump RGB, RGBA, Index, and Index16 runs respectively.

 *    (These all use the file-wide variable 'dumpBuf')

 */



static unsigned char dumpBuf[7];





#define imMiffDumpRun3(r,g,b,len)	                            \
	dumpBuf[0] = r; dumpBuf[1] = g; dumpBuf[2] = b;             \
	dumpBuf[3] = len;					    \
	if ( ImBinWrite( ioType, fd, fp, dumpBuf, UCHAR, 1, 4)==-1) \
	{							    \
		ImReturnBinError( );				    \
	}		



#define imMiffDumpRun4(r,g,b,a,len)	                            \
	dumpBuf[0] = r; dumpBuf[1] = g; dumpBuf[2] = b;             \
	dumpBuf[3] = a; dumpBuf[4] = len;			    \
	if ( ImBinWrite( ioType, fd, fp, dumpBuf, UCHAR, 1, 5)==-1) \
	{							    \
		ImReturnBinError( );				    \
	}		



#define imMiffDumpRun1(v,len)		                            \
	dumpBuf[0] = v; 				            \
	dumpBuf[1] = len;					    \
	if ( ImBinWrite( ioType, fd, fp, dumpBuf, UCHAR, 1, 2)==-1) \
	{							    \
		ImReturnBinError( );				    \
	}		



#define imMiffDumpRun2(v,len)		                            \
	dumpBuf[0] = ((v & 0xff00) >> 8); 		            \
	dumpBuf[1] = (v & 0x00ff); 			            \
	dumpBuf[2] = len;					    \
	if ( ImBinWrite( ioType, fd, fp, dumpBuf, UCHAR, 1, 3)==-1) \
	{							    \
		ImReturnBinError( );				    \
	}		





/*

 * FUNCTION

 *     imMiffWriteRLERGB

 *

 * DESCRIPTION

 *     Write the pixels of the vfb using RLE compression.

 */

static int /* returns status */

#ifdef __STDC__

imMiffWriteRLERGB( ImVfb* vfb, int fd, FILE* fp, int ioType)

#else

imMiffWriteRLERGB( vfb, fd, fp, ioType)

ImVfb* vfb;

int fd;

FILE* fp;

int ioType;

#endif

{

 int runLength, i,x,y;        /* index, dimensions   */

 ImVfbPtr thisPixel;          /* this pixel          */

 ImVfbPtr lastPixel;          /* last pixel          */

 unsigned char dumpBuf[10];   /* Buffer for output   */



 /* 

  * The unsigned ints above are used to store red,green, and blue

  * values for pixels.  i.e. three of the bytes are used for these

  * values.

  */



 x = ImVfbQWidth(vfb);

 y = ImVfbQHeight(vfb);



 runLength = 0;



 lastPixel = ImVfbQPtr( vfb, 0, 0);

 thisPixel = ImVfbQPtr( vfb, 0, 0);

 for (i=1; i<x*y; i++)  /* Skip the first pixel */

 {

	thisPixel = ImVfbQNext(vfb,thisPixel);

	if (ImVfbSameRGB(vfb, lastPixel,thisPixel))

	{

		runLength++;

		if (runLength > 0xff)

		{

			runLength--;

			/* Dump run */

			imMiffDumpRun3(ImVfbQRed(vfb,lastPixel),

				       ImVfbQGreen(vfb,lastPixel),

				       ImVfbQBlue(vfb,lastPixel),

				       runLength);

			runLength = 0;

		}

	}

	else /* new run.  Dump the last one. */

	{

		imMiffDumpRun3(ImVfbQRed(vfb,lastPixel),

			       ImVfbQGreen(vfb,lastPixel),

			       ImVfbQBlue(vfb,lastPixel),

			       runLength);

		runLength = 0;

	}

	lastPixel = thisPixel;

 }



 /* Dump the last run */

imMiffDumpRun3(ImVfbQRed(vfb,lastPixel),

	       ImVfbQGreen(vfb,lastPixel),

	       ImVfbQBlue(vfb,lastPixel),

	       runLength);

 return 1;

}





/*

 * FUNCTION

 *     imMiffWriteRLERGBA

 *

 * DESCRIPTION

 *     Write the pixels of the vfb (with alpha) using RLE compression.

 */

static int /* returns status */

#ifdef __STDC__

imMiffWriteRLERGBA( ImVfb* vfb, int fd, FILE* fp, int ioType)

#else

imMiffWriteRLERGBA( vfb, fd, fp, ioType)

ImVfb* vfb;

int fd;

FILE* fp;

int ioType;

#endif

{

 ImVfbPtr vfbptr;             /* points into a vfb   */

 int runLength, i,x,y;        /* indexes, dimensions */

 ImVfbPtr thisPixel;          /* this pixel          */

 ImVfbPtr lastPixel;          /* last pixel          */

 unsigned char dumpBuf[10];   /* Buffer for output   */



 /* 

  * The unsigned ints above are used to store red,green, and blue

  * values for pixels.  i.e. three of the bytes are used for these

  * values.

  */



 x = ImVfbQWidth(vfb);

 y = ImVfbQHeight(vfb);



 runLength = 0;



 lastPixel = ImVfbQPtr( vfb, 0, 0);

 thisPixel = ImVfbQPtr( vfb, 0, 0);

 for (i=1; i<x*y; i++)  /* Skip the first pixel */

 {

	thisPixel = ImVfbQNext(vfb,thisPixel);

	if (ImVfbSameRGBA(vfb, lastPixel,thisPixel))

	{

		runLength++;

		if (runLength > 0xff)

		{

			runLength--;

			/* Dump run */

			imMiffDumpRun4(ImVfbQRed(vfb,lastPixel),

				       ImVfbQGreen(vfb,lastPixel),

				       ImVfbQBlue(vfb,lastPixel),

				       ImVfbQAlpha(vfb,lastPixel),

				       runLength);

			runLength = 0;

		}

	}

	else /* new run.  Dump the last one. */

	{

		imMiffDumpRun4(ImVfbQRed(vfb,lastPixel),

			       ImVfbQGreen(vfb,lastPixel),

			       ImVfbQBlue(vfb,lastPixel),

			       ImVfbQAlpha(vfb,lastPixel),

			       runLength);

		runLength = 0;

	}

	lastPixel = thisPixel;

 }



 /* Dump the last run */

imMiffDumpRun4(ImVfbQRed(vfb,lastPixel),

	       ImVfbQGreen(vfb,lastPixel),

	       ImVfbQBlue(vfb,lastPixel),

	       ImVfbQAlpha(vfb,lastPixel),

	       runLength);

 return 1;

}



/*

 * FUNCTION

 *    imMiffWriteIndex

 *

 * DESCRIPTION

 *     Write an indexed miff file with...

 *	 - no compression

 * 	 - possibly a clt

 */

static int   /* returns number of tags used */

#ifdef __STDC__

imMiffWriteIndex( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable)

#else

imMiffWriteIndex( pMap, ioType, fd, fp, flagsTable, tagTable)

ImFileFormatWriteMap * pMap;

int ioType;

int fd;

FILE* fp;

TagTable* flagsTable;

TagTable* tagTable;

#endif

{

 ImVfb* vfb; 		/* image vfb           */

 char header[400];	/* header (text based) */

 int i;			/* loop indexes        */

 int runIndex;          /* yet another index   */

 unsigned char* runBuffer;/* buffer              */

 char message[100];     /* another buffer      */

 char* filename;        /* filename            */

 ImClt* clt;            /* image clt           */

 ImCltPtr cltptr;       /* points into the clt */

 char colorstring[30];  /* string for the header */

 int numColors;         /* num of colors in clt*/

 int chanDepth;         /* 8 or 16             */

 int numImages;         /* number of images    */

 int curImage;          /* loop index          */

 int numZeros;          /* how much to pad the clt */



 BinByteOrder( BINMBF );

 numImages = TagTableQNEntry (tagTable, "image vfb");



 for (curImage=0;curImage<numImages;curImage++)

 {

	 /* Get the vfb */

	 TagEntryQValue( TagTableQDirect( tagTable, "image vfb", curImage ), &vfb );



	 /* 

	  * Write the clt, if there is one.

	  */

	 if (pMap->map_outAttributes & IMCLTYES)

	 {

		clt = ImVfbQClt(vfb);

		numColors = ImCltQNColors(clt);



		if (pMap->map_outChannelDepth==16)

		{

			chanDepth = 16;

			ImInfo ("Type", "16-bit Color Indexed");

			/* Need to pad the color table if we don't have enough colors */

			if (numColors <= 256) 

				numZeros = 257-numColors;

			else

				numZeros = 0;

		}

		else

		{

			chanDepth = 8;

			ImInfo ("Type", "8-bit Color Indexed");

			numZeros = 0;

		}



		sprintf(message,"%d entries",numColors+numZeros);

		ImInfo ("Color Table",message);

		sprintf(colorstring,"colors=%d\n",numColors+numZeros);

	 }

	 else  /* no clt */

	 {

		 strcpy(colorstring, "" );

		 chanDepth = pMap -> map_outChannelDepth;

		 sprintf(message,"%d-bit Greyscale",chanDepth);

		 ImInfo ("Color Table","none");

		 ImInfo ("Type",message);

	 }



	 sprintf(message,"%d x %d", ImVfbQWidth(vfb), ImVfbQHeight(vfb));

	 ImInfo ("Resolution", message);

	 ImInfo ("Alpha Channel", "none");

	 

	 if (!flagsTable || TagTableQNEntry (flagsTable, "file name")==0)

	 {

		filename = NULL;

	 }

	 else

	 {

		TagEntryQValue( TagTableQDirect ( flagsTable, "file name", 0 ), &filename);

	 }



	/*

	 * Write the header

	 */

	 sprintf(header, "{\n    %s\n    %s\n}\nid=ImageMagick\nclass=PseudoClass\nrows=%d\ncolumns=%d\n%s%s\f\n:\n",

		filename ? filename : "(untitled)",

		"This file was created by the SDSC Image Tools.",

		ImVfbQHeight(vfb), /* rows    */

		ImVfbQWidth(vfb),  /* columns */

	 	(pMap->map_outAttributes & IMCOMPRLE) ? "compression=RunlengthEncoded\n" : "",

		colorstring

		);



	 if ( ImBinWrite( ioType, fd, fp, header, CHAR, 1, strlen(header))==-1)

	 {

		ImReturnBinError( );

	 }



	 

	 if (pMap->map_outAttributes & IMCLTYES)

	 {

		 /*

		  * Write the clt

		  */

		 cltptr = ImCltQFirst( clt );

		 ImCalloc ( runBuffer, unsigned char *, (numColors + numZeros) * 3, 1);

		 runIndex = 0;

		 for (i=0;i<numColors;i++)

		 {

			runBuffer[runIndex++] = ImCltQRed  (cltptr);

			runBuffer[runIndex++] = ImCltQGreen(cltptr);

			runBuffer[runIndex++] = ImCltQBlue (cltptr);

			cltptr = ImCltQNext( clt, cltptr);

		 }

		 if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, (numColors+numZeros) * 3)==-1)

		 {

			ImReturnBinError( );

		 }

		 free(runBuffer);

	 }



	 /*

	  * Write the pixels

	  */

	 if (chanDepth==8)

	 {

	 	if (pMap->map_outAttributes & IMCOMPRLE)

		{

			if (imMiffWriteRLEIndex8(vfb, fd, fp, ioType)<0)

				return -1;

	 		ImInfo ("Compression Type", "Run Length Encoded");

		}

		else

		{

	 		ImInfo ("Compression Type", "none");

			if (imMiffWriteRawIndex8(vfb, fd, fp, ioType)<0)

				return -1;

		}

	 }

	 else  /* 16-bit */

	 {

	 	if (pMap->map_outAttributes & IMCOMPRLE)

		{

			if (imMiffWriteRLEIndex16(vfb, fd, fp, ioType)<0)

				return -1;

	 		ImInfo ("Compression Type", "Run Length Encoded");

		}

		else

		{

			if (imMiffWriteRawIndex16(vfb, fd, fp, ioType)<0)

				return -1;

	 		ImInfo ("Compression Type", "none");

		}



	 }

 }



 return numImages;

}





/*

 * FUNCTION

 *    imMiffWriteRawIndex8

 *

 * DESCRIPTION

 *    Write the pixels of an indexed vfb with no compression

 */

static int /* returns status */

#ifdef __STDC__

imMiffWriteRawIndex8(ImVfb* vfb, int fd, FILE* fp, int ioType)

#else

imMiffWriteRawIndex8(vfb, fd, fp, ioType)

ImVfb* vfb;

int fd;

FILE* fp;

int ioType;

#endif

{

 int runIndex;          /* index               */

 unsigned char* runBuffer;/* buffer              */

 ImVfbPtr vfbptr;       /* points into a vfb   */

 int x, y;              /* dimensions of vfb   */

 int i,j;		/* loop indexes        */



 x = ImVfbQWidth(vfb);

 y = ImVfbQHeight(vfb);



 vfbptr = ImVfbQPtr( vfb, 0, 0);

 ImMalloc( runBuffer, unsigned char *, sizeof (unsigned char) * x  );



 for (i=0; i<y; i++)  /* i goes through the rows */

 {

	runIndex = 0;

	for (j=0;j<x;j++)  /* Fill a buffer for this row. */

	{

		runBuffer[runIndex++] = ImVfbQIndex(vfb, vfbptr);

		vfbptr = ImVfbQNext(vfb,vfbptr);	

	}	

	if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, x )==-1)

	{

		ImReturnBinError( );

	}

 }

 free(runBuffer);

return 1;

}







/*

 * FUNCTION

 *    imMiffWriteRawIndex16

 *

 * DESCRIPTION

 *    Write the pixels of an indexed vfb with no compression

 */

static int /* returns status */

#ifdef __STDC__

imMiffWriteRawIndex16(ImVfb* vfb, int fd, FILE* fp, int ioType)

#else

imMiffWriteRawIndex16(vfb, fd, fp, ioType)

ImVfb* vfb;

int fd;

FILE* fp;

int ioType;

#endif

{

 int runIndex;          /* index               */

 unsigned char* runBuffer;/* buffer              */

 ImVfbPtr vfbptr;       /* points into a vfb   */

 int x, y;              /* dimensions of vfb   */

 int i,j;		/* loop indexes        */



 x = ImVfbQWidth(vfb);

 y = ImVfbQHeight(vfb);



 vfbptr = ImVfbQPtr( vfb, 0, 0);

 ImMalloc( runBuffer, unsigned char *, sizeof (unsigned char) * x * 2 );



 for (i=0; i<y; i++)  /* i goes through the rows */

 {

	runIndex = 0;

	for (j=0;j<x;j++)  /* Fill a buffer for this row. */

	{

		runBuffer[runIndex++] = ((ImVfbQIndex16(vfb, vfbptr)) & 0xff00) >> 8;

		runBuffer[runIndex++] = (ImVfbQIndex16(vfb, vfbptr)) & 0x00ff;

		vfbptr = ImVfbQNext(vfb,vfbptr);	

	}	

	if ( ImBinWrite( ioType, fd, fp, runBuffer, UCHAR, 1, x*2 )==-1)

	{

		ImReturnBinError( );

	}

 }



 free(runBuffer);

return 1;

}







/*

 * FUNCTION

 *     imMiffWriteRLEIndex8

 *

 * DESCRIPTION

 *     Write the pixels of the vfb using RLE compression.

 */

static int /* returns status */

#ifdef __STDC__

imMiffWriteRLEIndex8( ImVfb* vfb, int fd, FILE* fp, int ioType)

#else

imMiffWriteRLEIndex8( vfb, fd, fp, ioType)

ImVfb* vfb;

int fd;

FILE* fp;

int ioType;

#endif

{

 int runLength, i,x,y;        /* indexes, dimensions */

 ImVfbPtr thisPixel;          /* this pixel          */

 ImVfbPtr lastPixel;          /* last pixel          */

 unsigned char dumpBuf[10];   /* Buffer for output   */



 x = ImVfbQWidth(vfb);

 y = ImVfbQHeight(vfb);



 runLength = 0;



 lastPixel = ImVfbQPtr( vfb, 0, 0);

 thisPixel = ImVfbQPtr( vfb, 0, 0);

 for (i=1; i<x*y; i++)  /* Skip the first pixel */

 {

	thisPixel = ImVfbQNext(vfb,thisPixel);

	if (ImVfbSameIndex8(vfb, lastPixel,thisPixel))

	{

		runLength++;

		if (runLength > 0xff)

		{

			runLength--;

			/* Dump run */

			imMiffDumpRun1(ImVfbQIndex(vfb,lastPixel),

				       runLength);

			runLength = 0;

		}

	}

	else /* new run.  Dump the last one. */

	{

		imMiffDumpRun1(ImVfbQIndex(vfb,lastPixel),

			       runLength);

		runLength = 0;

	}

	lastPixel = thisPixel;

 }



 /* Dump the last run */

imMiffDumpRun1(ImVfbQIndex8(vfb,lastPixel), runLength);



 return 1;

}







/*

 * FUNCTION

 *     imMiffWriteRLEIndex16

 *

 * DESCRIPTION

 *     Write the pixels of the vfb using RLE compression.

 */

static int /* returns status */

#ifdef __STDC__

imMiffWriteRLEIndex16( ImVfb* vfb, int fd, FILE* fp, int ioType)

#else

imMiffWriteRLEIndex16( vfb, fd, fp, ioType)

ImVfb* vfb;

int fd;

FILE* fp;

int ioType;

#endif

{

 int runLength, i,x,y;        /* indexes, dimensions */

 ImVfbPtr thisPixel;          /* this pixel          */

 ImVfbPtr lastPixel;          /* last pixel          */

 unsigned char dumpBuf[10];   /* Buffer for output   */



 x = ImVfbQWidth(vfb);

 y = ImVfbQHeight(vfb);



 runLength = 0;



 lastPixel = ImVfbQPtr( vfb, 0, 0);

 thisPixel = ImVfbQPtr( vfb, 0, 0);

 for (i=1; i<x*y; i++)  /* Skip the first pixel */

 {

	thisPixel = ImVfbQNext(vfb,thisPixel);

	if (ImVfbSameIndex16(vfb, lastPixel,thisPixel))

	{

		runLength++;

		if (runLength > 0xff)

		{

			runLength--;

			/* Dump run */

			imMiffDumpRun2(ImVfbQIndex16(vfb,lastPixel),

				       runLength);

			runLength = 0;

		}

	}

	else /* new run.  Dump the last one. */

	{

		imMiffDumpRun2(ImVfbQIndex16(vfb,lastPixel),

			       runLength);

		runLength = 0;

	}

	lastPixel = thisPixel;

 }



 /* Dump the last run */

imMiffDumpRun2(ImVfbQIndex16(vfb,lastPixel), runLength);



 return 1;

}







/*

 * FUNCTION

 *    strToLower

 * DESCRIPTION

 *    convert a string into lower case

 */



static char* /* returns the string */

#ifdef __STDC__

strToLower(char* str)

#else

strToLower(str)

char* str;

#endif

{

 char* ptr;

 if (str==NULL)

	return NULL;

 ptr = str;

 while (*ptr!='\0')

 {

	if (( *ptr >= 'A') && (*ptr <= 'Z'))

		*ptr -= ('A' - 'a');

	ptr++;

 }



 return str;

}



