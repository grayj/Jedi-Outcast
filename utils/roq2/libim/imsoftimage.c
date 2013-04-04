/**

 **	$Header: /roq/libim/imsoftimage.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imsoftimage.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imsoftimage.c -  Soft Image image file i/o

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imsoftimage.c contains routines to read and write SoftImages's image

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

 **	imSoftImageRead		f  read a MIFF file

 **

 **  HISTORY

 **	$Log: /roq/libim/imsoftimage.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 * Revision 1.4  1995/06/30  22:10:16  bduggan

 * added byte ordering stuff

 *

 * Revision 1.3  1995/06/29  00:28:04  bduggan

 * updated copyright year

 *

 * Revision 1.2  1995/06/15  20:03:15  bduggan

 * added line to return status

 *

 * Revision 1.1  1995/04/21  17:53:11  bduggan

 * Initial revision

 *

 **	

 **/



#include <string.h>

#include "iminternal.h"







/**

 **  FORMAT

 **	SI	- SoftImage image file format

 **

 **  AKA

 **	pic

 **

 **  FORMAT REFERENCES

 **	DKit User's Guide, SoftImage, Inc. 

 **

 **  CODE CREDITS

 **     Brian Duggan, Custom Development, San Diego Supercomputer Center, 1995.

 **

 **  DESCRIPTION

 **	SI's are single-image picture files.  

 **	They are divided into three sections:

 **		1. header

 **		2. channel information

 **		3. image data

 **

 **	======

 **	Header

 **	======

 **		Offset	Length	Name	Description

 **		-----------------------------------

 **		0	4	magic	Magic number

 **		4	4	version	version of format

 **		8	80	comment	user comment

 **		88	4	id	"PICT"

 **		92	2	width	image width (pixels)

 **		94	2	height	image height

 **		96	4	ratio	pixel width/height ratio

 **					= 1.0 for square pixels 

 **		100	2	fields	image fields type

 **					This indicates whether every scanline,

 **					odd scanlines, even scanlines, or no

 **					scanlines are stored.  We just handle

 **					every scanline.

 **		102	2	pad	nothing

 **

 **	===================

 **	Channel Information

 **	===================

 **

 **	This section of the file consists of a series of packets, describing

 ** 	the image data that follows.  Each packet describes a section of image

 **	data.  The organization of a packet is as follows:

 **

 **		Byte number	Description

 **		----------------------------

 **		1		Are there more packets after this one?

 **				0x00 means "no".

 **				0x01 means "no".

 **		2		How many bits per pixel per channel?

 **				This should always be 8.

 **		3		Is there compression?

 **				0x02 means there is rle compression.

 **				0x00 means there is no compression.

 **		4		Which channel is stored?

 **				0x80 means a red channel

 **				0x40 means a green channel

 **				0x20 means a blue channel

 **				0x10 means an alpha channel.

 **				This field may be a bit-or of the above channels.  Hence

 **				each packet may contain several channels.

 **

 **	==========

 **	Pixel data

 **	==========

 **	

 **	The organization of the pixels depends on the stuff in the "channel information"

 **	section.  For instance, if the channel info section has two packets,

 **	one indicating red, green, blue, no compression, and another indicating

 **	alpha and no compression, then the pixel data will be stored as...

 **		Scanline 1:   RGBRGBRGB..RGBAAAAAAA...AAA

 **		Scanline 2:   RGBRGBRGB..RGBAAAAAAA...AAA

 **	Notice that the channel information reflects the pixel data on a scanline by

 **	scanline basis.

 **

 **	Some portions of a scanline may be rle compressed.  Here's the rle compression

 **	scheme:

 **

 **		There are two types of runs, variable value runs and constant value runs.

 **		A variable value run looks like this:

 **			Encoded:    03 01 01 01 02 02 02 03 03 03 04 04 04 

 **			Unencoded:  pixel 1: 01 01 01

 **				    pixel 2: 02 02 02

 **				    pixel 3: 03 03 03

 **				    pixel 4: 04 04 04

 **		The '03' at the beginning signifies that there are 4 (3 + 1) pixels.

 **		Such a run must contain 128 or fewer pixels.

 **	

 **		A constant value run looks like this:

 **			Encoded:   83 01 01 01 

 **			Unencoded: pixel 1: 01 01 01

 **				   pixel 2: 01 01 01

 **				   pixel 3: 01 01 01

 **				   pixel 4: 01 01 01

 **		The high bit of the first byte is set to indicate a constant run.

 **		The number of pixels in the run is then one more than the value of

 **		the lower 7 bits.  

 **		You can have a constant run with more than 127 bytes by using the 

 **		following...

 **			Encoded:   80 aa aa 01 01 01

 **			Unencoded:  0xaaaa pixels with value 01 01 01.

 **		The 80 indicates that the next two bytes have the count.

 **

 **		All runs must be contained within a single scanline.

 **

 **	/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

 **	

 **	"Standard" SoftImage files are organized as follows:

 **		Packet 1 indicates red, green, & blue channels, rle compressed

 **		Packet 2 indicates alpha channel, rle compressed.

 **

 **	Although the official description of a softimage file is as described above,

 **	I have yet to see a non-standard one.  Hence, as of this revision, there is 

 **	no support herein for non-standard softimage files.  We only support

 **	RLE or raw softImages files, with 1 or 2 packets.

 **

 **/



/*

 *  SI - SoftImage's picture file format

 *      For information on these structures, how to use them, etc. please

 *      see imfmt.c.

 */



#ifdef __STDC__

static int imSoftimageRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imSoftReadRLERedGreenBlue( int ioType, int fd, FILE* fp, ImVfb* vfb, ImVfbPtr vfbPtr, unsigned int numPixels );

static int imSoftReadRLEAlpha( int ioType, int fd, FILE* fp, ImVfb* vfb, ImVfbPtr vfbPtr, unsigned int numPixels );

static int imSoftimageWriteRLE ( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable );

static int imSoftimageWriteRLEA ( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable );

static int imSoftimageWriteRaw ( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable );

static int imSoftimageWriteRawA ( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable );



static int imSoftimageWriteHeader( ImFileFormatWriteMap* pMap, int ioType, int fd, FILE* fp, ImVfb* vfb);

static int imSoftimageWriteRLERGBLine(ImVfb* vfb, ImVfbPtr vfbPtr, int numPixels, int ioType, FILE* fp, int fd);

static int imSoftimageWriteRLEALine(ImVfb* vfb, ImVfbPtr vfbPtr, int numPixels, int ioType, FILE* fp, int fd);

static int imSoftReadRawLineRedGreenBlue( int ioType, int fd, FILE* fp, ImVfb* vfb, ImVfbPtr vfbPtr );

static int imSoftReadRawLineAlpha ( int ioType, int fd, FILE* fp, ImVfb* vfb, ImVfbPtr vfbPtr );



#else



static int imSoftimageRead( );

static int imSoftReadRLERedGreenBlue( );

static int imSoftReadRLEAlpha( );



static int imSoftimageWriteRLE ( );

static int imSoftimageWriteRLEA ( );

static int imSoftimageWriteHeader( );

static int imSoftimageWriteRaw ( );

static int imSoftimageWriteRawA ( );

static int imSoftimageWriteRLERGBLine ( );

static int imSoftimageWriteRLEALine ( );

static int imSoftReadRawLineRedGreenBlue( );

static int imSoftReadRawLineAlpha ( );



#endif



/*

 * We omit "pic" from the list of suffixes because it

 * conflicts with pixar's suffix.

 */



static char *imSoftimageNames[ ]  = { "si", "softimage", NULL };

static ImFileFormatReadMap imSoftimageReadMap[ ] =

{

        /* in                   out                                     */

        /* type,ch,dep, attr.   VFB type        attr.                   */

        { RGB,3,8,      0,      IMVFBRGB,       0 },

        { RGB,3,8,      RLE,    IMVFBRGB,       0 },

	{ RGB,4,8,	A,	IMVFBRGB,	A },

	{ RGB,4,8,	RLE|A,	IMVFBRGB,	A },

        { -1,           0,      -1,             0 },

};

static ImFileFormatWriteMap imSoftimageWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */



        { IMVFBRGB,     0,      RGB,3,8,        RLE,    imSoftimageWriteRLE },

        { IMVFBRGB,     A,      RGB,4,8,        A|RLE,  imSoftimageWriteRLEA },

        { IMVFBRGB,     0,      RGB,3,8,        0,      imSoftimageWriteRaw },

        { IMVFBRGB,     A,      RGB,4,8,        A,      imSoftimageWriteRawA },



        { -1,           0,      -1,             0,      NULL },

};



static unsigned char imSoftimageMagicNumber[ ] = { 0x53, 0x80, 0xf6, 0x34 };



static ImFileMagic imFileSoftimageMagic []=

{

	{ 0, 4, imSoftimageMagicNumber },

	{ 0, 0, NULL },

};



ImFileFormat ImFileSoftimageFormat =

{

	imSoftimageNames, "SoftImage's image file format",

	"SoftImage, Inc.",

	"24-bit RGB color images with optional alpha channels,\n\
uncompressed (verbatim) and RLE-compressed.",
	"24-bit RGB color images with optional alpha channels,\n\
uncompressed (verbatim) and RLE-compressed.",

	imFileSoftimageMagic,

	IMNOMULTI, IMPIPE,  

	IMNOMULTI, IMPIPE,

	imSoftimageRead, imSoftimageReadMap, imSoftimageWriteMap

};







typedef struct imSoftHeaderInfo

{

	unsigned char si_magic[4];	/* SI magic #     */

	float si_version;		/* SI version #   */

	char 	si_comments[80];	/* comments       */

	char 	si_id[4];		/* "PICT"         */

	unsigned int	si_width;	/* width          */

	unsigned int	si_height;	/* height         */

	float 	si_ratio;		/* w/h ratio      */

	sdsc_uint16 	si_fields;	/* fields type    */

	unsigned char 	si_pad[2];	/* pad            */

} imSoftHeaderInfo;



static BinField imSoftHeaderFields[ ] =

{

	{ UCHAR,  1, 4 },		/* si_magic	*/

	{ FLOAT,  4, 1 },		/* si_version	*/

	{ CHAR,   1, 80},		/* si_comments	*/

	{ CHAR,   1, 4},		/* si_id	*/

	{ UINT,   2, 1},		/* si_width	*/

	{ UINT,   2, 1},		/* si_height	*/

	{ FLOAT,  4, 1},		/* si_ratio	*/

	{ UINT16, 2, 1 },		/* si_fields	*/

	{ UCHAR,  1, 2 },		/* si_pad	*/

	{ 0, 0, 0 }

};



/*

 * DEFINES

 *	These are possible values for the fields in the

 * 	imSoftPacketInfo structure.

 */

#define IMSI_ISLASTPACKET	0x00

#define IMSI_ISNOTLASTPACKET	0x01



#define IMSI_NO_COMPRESSION	0x00

#define IMSI_RLE_COMPRESSION	0x02



#define IMSI_RED		0x80

#define IMSI_GREEN		0x40

#define IMSI_BLUE		0x20

#define IMSI_ALPHA		0x10



typedef struct imSoftPacketInfo

{

	unsigned char si_last;		/* Is this the last channel ? */

	unsigned char si_size;		/* should be 8 (bits per chan)*/

	unsigned char si_compression;	/* RLE or none                */

	unsigned char si_channels;	/* describes channels         */

} imSoftPacketInfo;



static BinField imSoftPacketFields[ ] =

{

	{ UCHAR,  1, 1 },		/* si_last                      */

	{ UCHAR,  1, 1 },		/* si_size			*/

	{ UCHAR,  1, 1 },		/* si_compression		*/

	{ UCHAR,  1, 1 },		/* si_channels			*/

	{ 0, 0, 0 }

};



/*

 *  FUNCTION

 *	imSoftimageRead	-  Read a SoftImage file

 *

 *  DESCRIPTION

 *	The file header is read and the size of the image determined.

 */



static int				/* Returns # tags read in	*/

#ifdef __STDC__

imSoftimageRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imSoftimageRead( ioType, fd, fp, flagsTable, tagTable )

	int         ioType;		/* I/O flags			*/

	int         fd;			/* Input file descriptor	*/

	FILE	   *fp;			/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable   *tagTable;		/* Tag table to add to		*/

#endif

{

	imSoftHeaderInfo	header;		/* file header          */

	imSoftPacketInfo	packet[9];	/* Channel info packets */

	char 			message[300];   /* buffer 		*/

	int			numPackets;     /* number of channel packets */

	int			vfbType;	/* type of the vfb      */

	ImVfb*			vfb;		/* a vfb 		*/

	ImVfbPtr		vfbPtr;		/* points at a vfb      */

	int			compression; 	/* flag indicating rle  */

	int 			i, j;		/* loop indexes         */



	BinByteOrder( BINMBF );

	/* Read the header */

	if( ImBinReadStruct( ioType, fd, fp, &header, imSoftHeaderFields )== -1)

	{

		ImReturnBinError( );

	}

	ImInfo("Byte Order","Most Significant Byte First");

	sprintf(message,"%d x %d",header.si_width,header.si_height);

	ImInfo("Resolution",message);

	ImInfo("Type","24-bit RGB");

	vfbType = IMVFBRGB;

	

	/* Read in the channel packets */

	numPackets  = 0;

	compression = 0;

	packet[0].si_last = IMSI_ISLASTPACKET;  

	while (numPackets==0 || packet[numPackets-1].si_last==IMSI_ISNOTLASTPACKET)

	{

		/* Read another packet */

		if( ImBinReadStruct( ioType, fd, fp, &packet[numPackets], imSoftPacketFields )== -1)

		{

			ImReturnBinError( );

		}



		if (packet[numPackets].si_channels & IMSI_ALPHA)

			vfbType |= IMVFBALPHA;

		if (packet[numPackets].si_compression==IMSI_RLE_COMPRESSION)

			compression = 1;

		numPackets++;

	}

	

	if (vfbType & IMVFBALPHA)

	{

		ImInfo("Alpha Channel","8-bit");

	}

	else

	{

		ImInfo("Alpha Channel","none");

	}



	if (compression)

	{

		ImInfo("Compression","Run Length Encoded");

	}

	else

	{

		ImInfo("Compression","none");

	}



	if (strncmp(header.si_id,"PICT",4)!=0)

	{

		ImErrorFatal("Not a softimage picture file!", -1, IMENOIMAGE);

	}

	if (header.si_fields!=3)

	{

		/*

		 * if the value is 1, odd scanlines are present.

		 * if it's 2 even scanlines are present.

		 * if it's 0, no scanlines are present.

		 */

		ImErrorWarning("Scanlines may be missing",-1, IMEUNSUPPORTED);

	}

	if (header.si_comments[0])

	{

		ImInfo("Description",header.si_comments);

	}



	if ((vfb=ImVfbAlloc(header.si_width, header.si_height, vfbType))==IMVFBNULL)

	{

		ImErrorFatal( ImQError( ), -1, ImErrNo );

	}



	for (i=0;i<header.si_height;i++)

	{

		vfbPtr = ImVfbQPtr(vfb, 0, i);



		/* Read in one scanline */

		for (j=0;j<numPackets;j++)

		{

			if (packet[j].si_compression==IMSI_RLE_COMPRESSION)

			{

				switch (packet[j].si_channels)

				{

					case IMSI_ALPHA :

						imSoftReadRLEAlpha( ioType, fd, fp, vfb, vfbPtr, header.si_width );

						break;

					case IMSI_RED|IMSI_GREEN|IMSI_BLUE :

						imSoftReadRLERedGreenBlue( ioType, fd, fp, vfb, vfbPtr, header.si_width );

						break;

					/*

					 * Other combinations of Red,Green,Blue,Alpha may be added here

					 * for non-standard SoftImage files.

					 */

					default :

						ImErrorFatal( "Unknown or unsupported channel packet.", -1, IMEUNSUPPORTED);

				}

			}

			else

			/* No compression */

			{

				switch (packet[j].si_channels)

				{

					case IMSI_ALPHA :

						imSoftReadRawLineAlpha(ioType, fd, fp, vfb, vfbPtr );

						break;

					case IMSI_RED|IMSI_GREEN|IMSI_BLUE :

						imSoftReadRawLineRedGreenBlue(ioType, fd, fp, vfb, vfbPtr );

						break;

					/*

					 * Other combinations of Red,Green,Blue,Alpha may be added here

					 * for non-standard SoftImage files.

					 */

					default :

						ImErrorFatal( "Unknown or unsupported channel packet.", -1, IMEUNSUPPORTED);

				}

			}

		}	/* End of loop to read in one scanline. */

	}		/* End of loop through scanlines        */

	TagTableAppend( tagTable,

		TagEntryAlloc( "image vfb", POINTER, &vfb ) );



	return ( 1 );

}



/*

 * DEFINE

 *    imSoftGetBytes

 *

 * DESCRIPTION

 *    Get a few bytes.  This macro is just to

 * make the rest of the code easier to read.

 *    And the static variable below is just

 * for convenience.

 */



#define imSoftGetBytes(numBytes, buf)				\
if( ImBinRead( ioType, fd, fp, buf, UCHAR, 1, numBytes)==-1)	\
	{ 							\
		ImReturnBinError( );				\
	}



static unsigned char buf[256];





/*

 *  FUNCTION

 * 	imSoftReadRLEAlpha

 *

 *  DESCRIPTION

 *	Read alpha for pixels from a scanline, RLE encoded

 */

static int /* returns status */

#ifdef __STDC__

imSoftReadRLEAlpha( int ioType, int fd, FILE *fp, ImVfb *vfb, ImVfbPtr vfbPtr, unsigned int numPixels )

#else

imSoftReadRLEAlpha( ioType, fd, fp, vfb, vfbPtr, numPixels )

int ioType;

int fd;

FILE* fp;

ImVfb* vfb;

ImVfbPtr vfbPtr;

unsigned int numPixels;

#endif

{

	int i;		/* loop variable       */

	int runLength;  /* length of a run     */

	int pixelsDone=0;/* pixels read so far */



	while (pixelsDone < numPixels)

	{

		imSoftGetBytes( 1, buf);



		if (buf[0] & 0x80)

		{	/* Run of the same pixel */

			runLength = buf[0] & 0x7f;



			if (runLength==0)

			{

				imSoftGetBytes (2, buf);

				runLength = (buf[0]<<8)|(buf[1]);

			}

			else

				runLength++;

			

			imSoftGetBytes( 1, buf);



			/* Set all the pixels */

			for (i=0; i<runLength; i++)

			{

				ImVfbSAlpha(vfb, vfbPtr, buf[0]);

				ImVfbSInc  (vfb, vfbPtr);

			}

			pixelsDone += runLength;

		}

		else

		{	/* Run of different pixels */

			runLength = ( (int)(buf[0] & 0x7f) ) + 1;



			imSoftGetBytes( runLength , buf);

			for (i=0;i<runLength;i+=1)

			{

				ImVfbSAlpha(vfb, vfbPtr, buf[i]  );

				ImVfbSInc  (vfb, vfbPtr);

			}

			pixelsDone += runLength;

		}

	} /* End of while more pixels */

	return 1;

}





/*

 *  FUNCTION

 * 	imSoftReadRLERedGreenBlue

 *

 *  DESCRIPTION

 *	Read red, green, blue for pixels from a scanline, RLE encoded

 */

static int 	/* returns status */

#ifdef __STDC__

imSoftReadRLERedGreenBlue( int ioType, int fd, FILE* fp, ImVfb* vfb, ImVfbPtr vfbPtr, unsigned int numPixels )

#else

imSoftReadRLERedGreenBlue( ioType, fd, fp, vfb, vfbPtr, numPixels )

int ioType;

int fd;

FILE* fp;

ImVfb* vfb;

ImVfbPtr vfbPtr;

unsigned int numPixels;

#endif

{

	int i;		/* loop variable       */

	int runLength;  /* length of a run     */

	int pixelsDone=0;/* pixels read so far */



	while (pixelsDone < numPixels)

	{

		imSoftGetBytes( 1, buf);



		if (buf[0] & 0x80)

		{	/* Run of the same pixel */

			runLength = buf[0] & 0x7f;



			if (runLength==0)

			{

				imSoftGetBytes (2, buf);

				runLength = (buf[0]<<8)|(buf[1]);

			}

			else

				runLength++;

			

			imSoftGetBytes( 3, buf);



			/* Set all the pixels */

			for (i=0; i<runLength; i++)

			{

				ImVfbSRed  (vfb, vfbPtr, buf[0]);

				ImVfbSGreen(vfb, vfbPtr, buf[1]);

				ImVfbSBlue (vfb, vfbPtr, buf[2]);

				ImVfbSInc  (vfb, vfbPtr);

			}

			pixelsDone += runLength;

		}

		else

		{	/* Run of different pixels */

			runLength = ( (int)(buf[0] & 0x7f) ) + 1;



			imSoftGetBytes( runLength * 3, buf);

			for (i=0;i<runLength*3;i+=3)

			{

				ImVfbSRed  (vfb, vfbPtr, buf[i]  );

				ImVfbSGreen(vfb, vfbPtr, buf[i+1]);

				ImVfbSBlue (vfb, vfbPtr, buf[i+2]);

				ImVfbSInc  (vfb, vfbPtr);

			}

			pixelsDone += runLength;

		}

	} /* End of while more pixels */

	return 1;

}





/*

 *  FUNCTION

 * 	imSoftReadRawLineRedGreenBlue

 *

 *  DESCRIPTION

 *	Read red, green, blue for pixels from one scanline, no encoding

 */

static int 	/* returns status */

#ifdef __STDC__

imSoftReadRawLineRedGreenBlue( int ioType, int fd, FILE* fp, ImVfb* vfb, ImVfbPtr vfbPtr )

#else

imSoftReadRawLineRedGreenBlue( ioType, fd, fp, vfb, vfbPtr )

int ioType;

int fd;

FILE* fp;

ImVfb* vfb;

ImVfbPtr vfbPtr;

#endif

{

	static unsigned char *buffer=NULL; /* buffer */

	int numPixels;			   /* how much to read */

	int i;				   /* index */



	numPixels = ImVfbQWidth(vfb);

	if (buffer==NULL)  /* allocate the first time through */

	{

		ImMalloc(buffer, unsigned char *, numPixels * 3);

	}



	/* Read the data */

	imSoftGetBytes(numPixels * 3, buffer);



	for (i=0; i<numPixels; i++)

	{

		ImVfbSRed(vfb, vfbPtr, buffer[3*i]);

		ImVfbSGreen(vfb, vfbPtr, buffer[3*i+1]);

		ImVfbSBlue (vfb, vfbPtr, buffer[3*i+2]);

		ImVfbSInc (vfb, vfbPtr);

	}



	return 1;

}





/*

 *  FUNCTION

 * 	imSoftReadRawLineAlpha

 *

 *  DESCRIPTION

 *	Read alpha for pixels from one scanline, no encoding

 */

static int 	/* returns status */

#ifdef __STDC__

imSoftReadRawLineAlpha ( int ioType, int fd, FILE* fp, ImVfb* vfb, ImVfbPtr vfbPtr )

#else

imSoftReadRawLineAlpha ( ioType, fd, fp, vfb, vfbPtr )

int ioType;

int fd;

FILE* fp;

ImVfb* vfb;

ImVfbPtr vfbPtr;

#endif

{

	static unsigned char *buffer=NULL; /* buffer */

	int numPixels;			   /* how much to read */

	int i;				   /* index */



	numPixels = ImVfbQWidth(vfb);

	if (buffer==NULL)  /* allocate the first time through */

	{

		ImMalloc(buffer, unsigned char *, numPixels );

	}



	/* Read the data */

	imSoftGetBytes(numPixels , buffer);



	for (i=0; i<numPixels; i++)

	{

		ImVfbSAlpha(vfb, vfbPtr, buffer[i]);

		ImVfbSInc (vfb, vfbPtr);

	}



	return 1;

}







/* 

 * MACROS

 *   imSoftDumpVarRun

 *

 *   imSoftDumpConstantRun3

 *

 * DESCRIPTION

 *   These are used for RLE encoding.

 *

 *   imSoftDumpVarRun will write out a variable run.  That is,

 *   a stream of pixel values with no two consecutive values

 *   that are the same.  (i.e. A B C D E)

 *

 *   imSoftDumpConstantRun3 will write out a stream of 

 *   pixels with the same value.  (i.e. A A A A A)

 */



static unsigned char imSoftBuf[10];    /* Buffer for these macros */



#define imSoftDumpVarRun(bytesPerPixel, buffer, len)	\
	imSoftBuf[0] = (unsigned char) len;       	\
	if ( ImBinWrite( ioType, fd, fp, imSoftBuf, UCHAR, 1, 1)==-1) \
	{                                               \
		ImReturnBinError( );                    \
	}						\
	if ( ImBinWrite( ioType, fd, fp, buffer, UCHAR, 1, bytesPerPixel * (len+1))==-1) \
	{                                               \
		ImReturnBinError( );                    \
	}

   

/*

 * The parameter len, here, is actually one LESS than the number

 * of pixels in this run.  For instance, passing 0 signifies a

 * run of 1 pixel.  (Think of the "length" of a run as the number

 * of pixels after the first one.  One pixel is a run of length 0.

 * Two pixels is a run of length 1.) 

 */

#define imSoftDumpConstantRun3(r,g,b,len)			\
	if (len==0)						\
	{	/* Can't have a run of one pixel due to   */	\
		/* an ambiguity between this and when the */	\
		/* length is >= 128                       */	\
		imSoftBuf[0] = r;				\
		imSoftBuf[1] = g;				\
		imSoftBuf[2] = b;				\
		imSoftDumpVarRun(3, imSoftBuf, 0);		\
	}							\
	else							\
	if (len>=128)						\
	{							\
		imSoftBuf[0] = 0x80;				\
		imSoftBuf[1] = ((len+1) & 0xff00) >> 8;         \
		imSoftBuf[2] = (len+1) & 0xff;                  \
		imSoftBuf[3] = r;                               \
		imSoftBuf[4] = g;                               \
		imSoftBuf[5] = b;                               \
		if ( ImBinWrite( ioType, fd, fp, imSoftBuf, UCHAR, 1, 6)==-1) \
		{                                               \
			ImReturnBinError( );                    \
		}                                               \
	}                                                       \
	else                                                    \
	{                                                       \
		imSoftBuf[0] = (unsigned char) len;		\
		imSoftBuf[0] |= 0x80;				\
		imSoftBuf[1] = r;                               \
		imSoftBuf[2] = g;                               \
		imSoftBuf[3] = b;                               \
		if ( ImBinWrite( ioType, fd, fp, imSoftBuf, UCHAR, 1, 4)==-1) \
		{                                               \
			ImReturnBinError( );                    \
		}                                               \
	}



#define imSoftDumpConstantRun1(a,len)				\
	if (len==0)						\
	{	/* Can't have a run of one pixel due to   */	\
		/* an ambiguity between this and when the */	\
		/* length is >= 128                       */	\
		imSoftBuf[0] = a;				\
		imSoftDumpVarRun(1, imSoftBuf, 0);		\
	}							\
	else							\
	if (len>=128)						\
	{							\
		imSoftBuf[0] = 0x80;				\
		imSoftBuf[1] = ((len+1) & 0xff00) >> 8;         \
		imSoftBuf[2] = (len+1) & 0xff;                  \
		imSoftBuf[3] = a;                               \
		if ( ImBinWrite( ioType, fd, fp, imSoftBuf, UCHAR, 1, 4)==-1) \
		{                                               \
			ImReturnBinError( );                    \
		}                                               \
	}                                                       \
	else                                                    \
	{                                                       \
		imSoftBuf[0] = (unsigned char) len;		\
		imSoftBuf[0] |= 0x80;				\
		imSoftBuf[1] = a;                               \
		if ( ImBinWrite( ioType, fd, fp, imSoftBuf, UCHAR, 1, 2)==-1) \
		{                                               \
			ImReturnBinError( );                    \
		}                                               \
	}



/*

 *  FUNCTION

 *	imSoftimageWriteRLE

 *

 *  DESCRIPTION

 *	Create an SI using RLE.

 *

 */

static int    /* return # of tags used */

#ifdef __STDC__

imSoftimageWriteRLE ( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable )

#else

imSoftimageWriteRLE ( pMap, ioType, fd, fp, flagsTable, tagTable )

ImFileFormatWriteMap *pMap;

int ioType;

int fd;

FILE *fp;

TagTable *flagsTable;

TagTable *tagTable;

#endif

{

	ImVfb        *vfb;		/* Read in image		 */

	ImVfbPtr     vfbPtr;		/* Pixel pointer		 */

	imSoftHeaderInfo  header;	/* file header			 */

	int i;                          /* loop index                    */



	TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);

	BinByteOrder( BINMBF );

	ImInfo("Byte Order","Most Significant Byte First");



	/* Set up and write out the header */

        if (imSoftimageWriteHeader ( pMap, ioType, fd, fp, vfb )==-1)

		return -1;



	vfbPtr = ImVfbQFirst(vfb);



	for (i = 0;i < ImVfbQHeight( vfb ) ; i++)

	{

		if ( i>0 )

			vfbPtr = ImVfbQDown(vfb, vfbPtr);



		if (imSoftimageWriteRLERGBLine(vfb, vfbPtr, ImVfbQWidth(vfb), ioType, fp, fd)<0)

			return -1;

	}



	return (1);

}





/*

 *  FUNCTION

 *	imSoftimageWriteRLEA

 *

 *  DESCRIPTION

 *	Create an SI with alpha, using RLE.

 *

 */

static int    /* return # of tags used */

#ifdef __STDC__

imSoftimageWriteRLEA ( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable )

#else

imSoftimageWriteRLEA ( pMap, ioType, fd, fp, flagsTable, tagTable )

ImFileFormatWriteMap *pMap;

int ioType;

int fd;

FILE *fp;

TagTable *flagsTable;

TagTable *tagTable;

#endif

{

	ImVfb        *vfb;		/* Read in image		 */

	ImVfbPtr     vfbPtr;		/* Pixel pointer		 */

	imSoftHeaderInfo  header;	/* file header			 */

	int i;                          /* loop index                    */



	ImInfo("Byte Order","Most Significant Byte First");

	BinByteOrder( BINMBF );

	TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);



	/* Set up and write out the header */

        if (imSoftimageWriteHeader ( pMap, ioType, fd, fp, vfb )==-1)

		return -1;



	vfbPtr = ImVfbQFirst(vfb);



	for (i = 0;i < ImVfbQHeight( vfb ) ; i++)

	{

		if ( i>0 )

			vfbPtr = ImVfbQDown(vfb, vfbPtr);



		if (imSoftimageWriteRLERGBLine(vfb, vfbPtr, ImVfbQWidth(vfb), ioType, fp, fd)<0)

			return -1;



		if (imSoftimageWriteRLEALine(vfb, vfbPtr, ImVfbQWidth(vfb), ioType, fp, fd)<0)

			return -1;

	}



	return (1);

}



/*

 *  FUNCTION

 * 	imSoftimageWriteRLERGBLine

 *

 *  DESCRIPTION

 *	Write a certain number of pixels' rgb values, rle encoding 'em

 *

 */

static int /* Returns status */

#ifdef __STDC__

imSoftimageWriteRLERGBLine(ImVfb* vfb, ImVfbPtr vfbPtr, int numPixels, int ioType, FILE* fp, int fd)

#else

imSoftimageWriteRLERGBLine(vfb, vfbPtr, numPixels, ioType, fp, fd)

ImVfb* vfb;

ImVfbPtr vfbPtr;

int numPixels;

int ioType;

FILE* fp;

int fd;

#endif

{

	int i;	                        /* index */

	unsigned char varRun[128*3];    /* Holds a variable run          */

	ImVfbPtr lastPixel, thisPixel;  /* point to pixels.              */

 	int varRunLength;               /* Length of this variable run.  */

	int constantRunLength;          /* Length of this constant run.  */



	/* Initialize stuff. */

	varRunLength      = 0;

	constantRunLength = 0;

	thisPixel         = vfbPtr;

	lastPixel         = thisPixel;



	for (i=1;i<numPixels;i++)

	{

		ImVfbSInc(vfb,thisPixel);

		

		/* 

		 * If this pixel is the same as the last one then:

		 *   if varRunLength > 0, dump varRun.

		 *   otherwise, add this pixel to constantRun.

		 */

		 if (ImVfbSameRGB(vfb,lastPixel,thisPixel))  

		 {

			if (varRunLength > 0)

			{

				imSoftDumpVarRun(3,varRun, varRunLength - 1); 

				varRunLength = 0;

			}

			constantRunLength++;

			if (constantRunLength==65535)

			{   /* Dump a constant run. */

				imSoftDumpConstantRun3(ImVfbQRed(vfb,lastPixel), 

					ImVfbQGreen(vfb, lastPixel),

					ImVfbQBlue(vfb, lastPixel),

					constantRunLength - 1);

				constantRunLength = 0;

			}

		 }

		 else  /* This is different from the last character. */

		       /* Do the exact opposite of the if-branch.    */

		 {

			varRun[varRunLength*3]   = ImVfbQRed(vfb, lastPixel);

			varRun[varRunLength*3+1] = ImVfbQGreen(vfb, lastPixel);

			varRun[varRunLength*3+2] = ImVfbQBlue(vfb, lastPixel);



			if (constantRunLength > 0)

			{

				imSoftDumpConstantRun3(ImVfbQRed(vfb, lastPixel),

					ImVfbQGreen(vfb, lastPixel),

					ImVfbQBlue(vfb, lastPixel),

					constantRunLength);

				constantRunLength = 0;

			}

			else

			{   /* Only increment if this is not the end of a constant run. */

				varRunLength++;

			}

			if (varRunLength==128)

			{   /* Dump variable run. */

				imSoftDumpVarRun(3,varRun, varRunLength - 1); 

				varRunLength = 0;

			}

		 }

		 lastPixel = thisPixel;

	}



	/* Dump whatever is left. */

	if (constantRunLength==0 && varRunLength==0)

	{  /* single pixel is left. */

		imSoftDumpConstantRun3(ImVfbQRed(vfb, thisPixel),

			ImVfbQGreen(vfb, thisPixel),

			ImVfbQBlue(vfb, thisPixel), 0 );

	}

	if (constantRunLength > 0)

	{

		imSoftDumpConstantRun3(ImVfbQRed(vfb, thisPixel),

			ImVfbQGreen(vfb, thisPixel),

			ImVfbQBlue(vfb, thisPixel),

			constantRunLength );

	}

	if (varRunLength > 0)

	{

		varRun[varRunLength*3]   = ImVfbQRed  (vfb, lastPixel);

		varRun[varRunLength*3+1] = ImVfbQGreen(vfb, lastPixel);

		varRun[varRunLength*3+2] = ImVfbQBlue (vfb, lastPixel);

		imSoftDumpVarRun(3,varRun, varRunLength );

	}



 	return ( 1 );

}





/*

 *  FUNCTION

 * 	imSoftimageWriteRLEALine

 *

 *  DESCRIPTION

 *	Write a certain number of pixels' alpha values, rle encoding 'em

 *

 */

static int /* Returns status */

#ifdef __STDC__

imSoftimageWriteRLEALine(ImVfb* vfb, ImVfbPtr vfbPtr, int numPixels, int ioType, FILE* fp, int fd)

#else

imSoftimageWriteRLEALine(vfb, vfbPtr, numPixels, ioType, fp, fd)

ImVfb* vfb;

ImVfbPtr vfbPtr;

int numPixels;

int ioType;

FILE* fp;

int fd;

#endif

{

	int i;	                        /* index */

	unsigned char varRun[128];      /* Holds a variable run          */

	ImVfbPtr lastPixel, thisPixel;  /* point to pixels.              */

 	int varRunLength;               /* Length of this variable run.  */

	int constantRunLength;          /* Length of this constant run.  */



	/* Initialize stuff. */

	varRunLength      = 0;

	constantRunLength = 0;

	thisPixel         = vfbPtr;

	lastPixel         = thisPixel;



	for (i=1;i<numPixels;i++)

	{

		ImVfbSInc(vfb,thisPixel);

		

		/* 

		 * If this pixel is the same as the last one then:

		 *   if varRunLength > 0, dump varRun.

		 *   otherwise, add this pixel to constantRun.

		 */

		 if (ImVfbQAlpha(vfb,lastPixel)==ImVfbQAlpha(vfb,thisPixel))  

		 {

			if (varRunLength > 0)

			{

				imSoftDumpVarRun(1,varRun, varRunLength - 1); 

				varRunLength = 0;

			}

			constantRunLength++;

			if (constantRunLength==65535)

			{   /* Dump a constant run. */

				imSoftDumpConstantRun1(ImVfbQAlpha(vfb,lastPixel), 

					constantRunLength - 1);

				constantRunLength = 0;

			}

		 }

		 else  /* This is different from the last character. */

		       /* Do the exact opposite of the if-branch.    */

		 {

			varRun[varRunLength]   = ImVfbQAlpha(vfb, lastPixel);



			if (constantRunLength > 0)

			{

				imSoftDumpConstantRun1(ImVfbQAlpha(vfb, lastPixel),

					constantRunLength);

				constantRunLength = 0;

			}

			else

			{   /* Only increment if this is not the end of a constant run. */

				varRunLength++;

			}

			if (varRunLength==128)

			{   /* Dump variable run. */

				imSoftDumpVarRun(1,varRun, varRunLength - 1); 

				varRunLength = 0;

			}

		 }

		 lastPixel = thisPixel;

	}



	/* Dump whatever is left. */

	if (constantRunLength==0 && varRunLength==0)

	{  /* single pixel is left. */

		imSoftDumpConstantRun1(ImVfbQAlpha(vfb, thisPixel), 0 );

	}

	if (constantRunLength > 0)

	{

		imSoftDumpConstantRun1(ImVfbQAlpha(vfb, thisPixel), constantRunLength );

	}

	if (varRunLength > 0)

	{

		varRun[varRunLength]   = ImVfbQAlpha (vfb, lastPixel);

		imSoftDumpVarRun(1,varRun, varRunLength );

	}



 	return ( 1 );

}



/*

 *  FUNCTION

 *	imSoftimageWriteRaw

 *

 *  DESCRIPTION

 *	Create an SI using no compression.

 *

 */

static int    /* return # of tags used */

#ifdef __STDC__

imSoftimageWriteRaw ( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable )

#else

imSoftimageWriteRaw ( pMap, ioType, fd, fp, flagsTable, tagTable )

ImFileFormatWriteMap *pMap;

int ioType;

int fd;

FILE *fp;

TagTable *flagsTable;

TagTable *tagTable;

#endif

{

	ImVfb        *vfb;		/* Read in image		 */

	ImVfbPtr     vfbPtr;		/* Pixel pointer		 */

	imSoftHeaderInfo  header;	/* file header			 */

	int i,j;                        /* loop index                    */

	unsigned char *buffer;          /* buffer.                       */



	ImInfo("Byte Order","Most Significant Byte First");

	BinByteOrder( BINMBF );

	TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);

	ImMalloc (buffer, unsigned char* , ImVfbQWidth(vfb) * 3 );



	/* Set up and write out the header */

        if (imSoftimageWriteHeader ( pMap, ioType, fd, fp, vfb )==-1)

		return -1;



	vfbPtr = ImVfbQFirst(vfb);



	for (i = 0;i < ImVfbQHeight( vfb ) ; i++)

	{

		/* Write RGB values. */

		for (j=0;j < ImVfbQWidth(vfb); j++)

		{

			buffer[j*3]   = ImVfbQRed(vfb,vfbPtr);

			buffer[j*3+1] = ImVfbQGreen(vfb,vfbPtr);

			buffer[j*3+2] = ImVfbQBlue(vfb,vfbPtr);

			ImVfbSInc(vfb, vfbPtr);

		}



		if ( ImBinWrite( ioType, fd, fp, buffer, UCHAR, 1, 3 * ImVfbQWidth(vfb))==-1) 

		{                                               

			ImReturnBinError( );                   

		}                                             

	}



	return (1);

}





/*

 *  FUNCTION

 *	imSoftimageWriteRawA

 *

 *  DESCRIPTION

 *	Create an SI with alpha using no compression.

 *

 */

static int    /* return # of tags used */

#ifdef __STDC__

imSoftimageWriteRawA ( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp,

	TagTable *flagsTable, TagTable *tagTable )

#else

imSoftimageWriteRawA ( pMap, ioType, fd, fp, flagsTable, tagTable )

ImFileFormatWriteMap *pMap;

int ioType;

int fd;

FILE *fp;

TagTable *flagsTable;

TagTable *tagTable;

#endif

{

	ImVfb        *vfb;		/* Read in image		 */

	ImVfbPtr     vfbPtr;		/* Pixel pointer		 */

	imSoftHeaderInfo  header;	/* file header			 */

	int i,j;                        /* loop index                    */

	unsigned char *buffer;          /* buffer.                       */

	unsigned char *alphaBuffer;     /* buffer.                       */



	ImInfo("Byte Order","Most Significant Byte First");

	BinByteOrder( BINMBF );

	TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);

	ImMalloc (buffer, unsigned char* , ImVfbQWidth(vfb) * 3 );

	ImMalloc (alphaBuffer, unsigned char* , ImVfbQWidth(vfb) );



	/* Set up and write out the header */

        if (imSoftimageWriteHeader ( pMap, ioType, fd, fp, vfb )==-1)

		return -1;



	vfbPtr = ImVfbQFirst(vfb);



	for (i = 0;i < ImVfbQHeight( vfb ) ; i++)

	{

		/* Get RGB, A values. */

		for (j=0;j < ImVfbQWidth(vfb); j++)

		{

			buffer[j*3]   = ImVfbQRed(vfb,vfbPtr);

			buffer[j*3+1] = ImVfbQGreen(vfb,vfbPtr);

			buffer[j*3+2] = ImVfbQBlue(vfb,vfbPtr);

			alphaBuffer[j]= ImVfbQAlpha(vfb,vfbPtr);

			ImVfbSInc(vfb, vfbPtr);

		}



		/* Write RGB */

		if ( ImBinWrite( ioType, fd, fp, buffer, UCHAR, 1, 3 * ImVfbQWidth(vfb))==-1) 

		{                                               

			ImReturnBinError( );                   

		}                                             



		/* Write alpha */

		if ( ImBinWrite( ioType, fd, fp, alphaBuffer, UCHAR, 1,  ImVfbQWidth(vfb))==-1) 

		{                                               

			ImReturnBinError( );                   

		}

	}



	return (1);

}





/*

 *  FUNCTION

 *	imSoftimageWriteHeader

 *

 *  DESCRIPTION

 *	Write the header.

 */

static int	/* return status */

#ifdef __STDC__

imSoftimageWriteHeader( ImFileFormatWriteMap* pMap, int ioType, int fd, FILE* fp, ImVfb* vfb)

#else

imSoftimageWriteHeader( pMap, ioType, fd, fp, vfb)

ImFileFormatWriteMap* pMap;

int ioType;

int fd;

FILE* fp;

ImVfb* vfb;

#endif

{

	imSoftHeaderInfo header;        /* Header structure       */

	imSoftPacketInfo packet;        /* Channel info structure */

	int alpha;                      /* flag.  Is there alpha? */

	int compression;                /* flag.  Is there compression? */

	char message[140];              /* message */



	header.si_magic[0] = imSoftimageMagicNumber[0];

	header.si_magic[1] = imSoftimageMagicNumber[1];

	header.si_magic[2] = imSoftimageMagicNumber[2];

	header.si_magic[3] = imSoftimageMagicNumber[3];

	header.si_version = 2.6;

	strcpy(header.si_comments,

	"Created by the SDSC Image Library\0This space available for advertising.");

	strcpy(header.si_id, "PICT");

	header.si_width = ImVfbQWidth(vfb);

	header.si_height = ImVfbQHeight(vfb);

	header.si_ratio  = 1.0;    

	header.si_fields = 3;    /* Signifies that every scanline is in this picture */

	header.si_pad[0] = 0x00;

	header.si_pad[1] = 0x00;



	ImInfo("Type","24-bit RGB");

	sprintf(message,"%d x %d",header.si_width,header.si_height);

	ImInfo("Resolution",message);



        if ( ImBinWriteStruct( ioType, fd, fp, &header, imSoftHeaderFields )==-1)

	{

		ImReturnBinError( );

	}



	if (pMap->map_outAttributes & IMALPHAYES)

	{

		alpha = 1;

		ImInfo("Alpha Channel","8-bit");

	}

	else

	{

		ImInfo("Alpha Channel","none");

		alpha = 0;

	}

	

	if (pMap->map_outAttributes & IMCOMPRLE)

	{

		ImInfo("Compression","Run Length Encoded");

		compression = 1;

	}

	else

	{

		ImInfo("Compression","none");

		compression = 0;

	}

	

	packet.si_last = (alpha ? IMSI_ISNOTLASTPACKET : IMSI_ISLASTPACKET);

	packet.si_size = 0x08;

	packet.si_compression = (compression ? IMSI_RLE_COMPRESSION : IMSI_NO_COMPRESSION);

	packet.si_channels = IMSI_RED|IMSI_GREEN|IMSI_BLUE;



        if ( ImBinWriteStruct( ioType, fd, fp, &packet, imSoftPacketFields )==-1)

	{

		ImReturnBinError( );

	}



	if (alpha) /* Write another packet */

	{

		packet.si_last = IMSI_ISLASTPACKET;

		packet.si_size = 0x08;

		packet.si_compression = (compression ? IMSI_RLE_COMPRESSION : IMSI_NO_COMPRESSION);

		packet.si_channels = IMSI_ALPHA;

		if ( ImBinWriteStruct( ioType, fd, fp, &packet, imSoftPacketFields )==-1)

		{

			ImReturnBinError( );

		}

	}

	return 1;

}



