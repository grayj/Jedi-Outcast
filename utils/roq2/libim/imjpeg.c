/**
 **	$Header: /roq/libim/imjpeg.c 1     11/02/99 4:38p Zaphod $
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

#define HEADER        "    $Header: /roq/libim/imjpeg.c 1     11/02/99 4:38p Zaphod $"

/**
 **  FILE
 **	imjpeg.c  - JPEG (Joint Photographic Experts Group) file routines
 **
 **  PROJECT
 **	libim	-  SDSC image manipulation library
 **
 **  DESCRIPTION
 **	imjpeg.c contains routines to read and write JPEG files for
 **	the image manipulation library.  Raster data read in is stored
 **	in a VFB and optional CLT in a tag list.  Raster data written
 **	out is taken from a tag list.
 **
 **  PUBLIC CONTENTS
 ** 			d =defined constant
 **                     f =function
 **                     m =defined macro
 **                     t =typedef/struct/union
 **                     v =variable
 **                     ? =other
 **
 **	none
 **
 **  PRIVATE CONTENTS
 **     imJpegRead               f  read a JPEG image file
 **     imJpegWrite              f  write a JPEG image file
 **
 **  HISTORY
 **	$Log: /roq/libim/imjpeg.c $
 * 
 * 1     11/02/99 4:38p Zaphod
 * Revision 1.4  1995/06/29  00:28:04  bduggan
 * updated copyright year
 *
 * Revision 1.3  1995/06/15  20:09:19  bduggan
 * Added iminfo message "no alpha channel"
 *
 * Revision 1.2  1995/04/03  21:28:34  bduggan
 * took out #ifdef NEWMAGIC
 *
 * Revision 1.1  1995/02/16  21:37:45  bduggan
 * Initial revision
 *
 ** 
 **/


#ifdef USE_JPEG_LIB
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "jpeglib.h"
#include "iminternal.h" 

#else
#include "iminternal.h"

#endif

#define IMJPEG_TRACE_LEVEL  1  /* level of verbosity for jpeg library */
                               /* 0 = no messages.  A higher number   */
			       /* indicates more messages.            */

/**
 **
 **  FORMAT
 **	JPEG - Joint Photographic Experts Group image file format
 **
 **  AKA
 **	jpg
 **
 **  FORMAT REFERENCES
 **	Wallace, Gregory K.  "The JPEG Still Picture Compression Standard",
 **	   Communications of the ACM, April 1991 (vol. 34 no. 4), pp. 30-44
 **	"The Data Compression Book" by Mark Nelson, published by M&T Books (Redwood
 **	   City, CA), 1991, ISBN 1-55851-216-0.
 **	"JPEG Still Image Data Compression Standard", by Pennebaker and Mitchell
 **	    published by Van Nostrand Reinhold, 1993, ISBN 0-442-01272-1.
 **
 **  CODE CREDITS
 **     Custom development, Brian Duggan, San Diego Supercomputer Center, 1995.
 **
 **  DESCRIPTION
 **     The routines in this file use the library developed by the Independent
 **     JPEG group.  This "official" site of this library is ftp.uu.net (192.48.96.9)
 **     in the directory graphics/jpeg.  
 **
 **	Please refer to the documentation in that library for more information
 ** 	about the JPEG format.
 **/


#ifdef USE_JPEG_LIB

#ifdef __STDC__
static int imJpegRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );
static int imJpegWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );
#else
static int imJpegRead( ), imJpegWrite( );
#endif

#endif  /* USE_JPEG_LIB */

static char *imJpegNames[ ]  = { "jpeg", "jpg", "jfif", NULL };


static unsigned char imJpegMagicNumber[ ] = { 0xFF, 0xD8 };
static ImFileMagic imFileJpegMagic []=
{
	{ 0, 2, imJpegMagicNumber},
	{ 0, 0, NULL },
};

static ImFileFormatReadMap imJpegReadMap[ ] =
{
        /* in                   out                                     */
        /* type,ch,dep, attr.   VFB type        attr.                   */
        { IN,1,8,       T,      IMVFBINDEX8,    0 },
        { RGB,3,8,      T,      IMVFBRGB,       0 },
        { -1,           0,      -1,             0 },
};
static ImFileFormatWriteMap imJpegWriteMap[ ] =
{
        /* in                   out                                     */
        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */
#ifdef USE_JPEG_LIB
        { IMVFBINDEX8,  0,      IN,1,8,         DCT|Q|T,  imJpegWrite },
        { IMVFBRGB,     0,      RGB,3,8,        DCT|Q|T,  imJpegWrite },
#endif
        { -1,           0,      -1,             0,      NULL },
};


ImFileFormat ImFileJpegFormat =
{
	imJpegNames, "JPEG Image File Format",
	"Joint Photographic Experts Group ",
#ifdef USE_JPEG_LIB
	"8-bit, 24-bit RGB image.",
	"8-bit, 24-bit RGB image.",
#else
	"none",
	"none",
#endif
	imFileJpegMagic,
	IMNOMULTI, IMPIPE,
	IMNOMULTI, IMPIPE,
#ifdef USE_JPEG_LIB
	imJpegRead, imJpegReadMap, imJpegWriteMap
#else
	NULL, NULL, NULL
#endif
};




#ifdef USE_JPEG_LIB

/* We use C's setjmp/longjmp facility to return control to our routine,
 * rather than simply allowing the JPEG library to exit().  This means that the
 * we must first execute a setjmp() call to establish the return point.  
 * We want the replacement error_exit to do a longjmp().  But we need to 
 * make the setjmp buffer accessible to the error_exit routine.  To do this, 
 * we make a private extension of the standard JPEG error handler object.  
 * (If we were using C++, we'd say we were making a subclass of the regular 
 * error handler.)
 */

/* Error handler struct: */

struct im_error_mgr {
  struct jpeg_error_mgr pub;    /* "public" fields */

  jmp_buf setjmp_buffer;        /* for return to caller */
};

typedef struct im_error_mgr * im_error_ptr;

/*
 * Routine that will replace the standard error_exit method:
 */

METHODDEF void
im_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a im_error_mgr struct, so coerce pointer */
  im_error_ptr myerr = (im_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}


/*
 * FUNCTION
 *    imJpegError
 *
 * DESCRIPTION
 *     Print an error using the image tools error
 * handler stuff
 */

METHODDEF void
#ifdef __STDC__
imEmitMessage (j_common_ptr cinfo, int msg_level)
#else
imEmitMessage (cinfo, msg_level)
j_common_ptr cinfo;
int msg_level;
#endif
{
	char buffer[JMSG_LENGTH_MAX];
	struct jpeg_error_mgr* err = cinfo->err;

	/* Create the message */
	(*cinfo->err->format_message) (cinfo, buffer);

	if (msg_level < 0)
	{  /* warning */
		ImErrorWarning(buffer, IM_NOTHING, IMEUNKNOWN );
		err->num_warnings++;
	}
	else
	{
		/* info message */
		if (IMJPEG_TRACE_LEVEL >= msg_level)
			ImInfo("JPEG information",buffer);
	}
}

/*
 * FUNCTION
 *    imOutputMessage
 *
 * DESCRIPTION
 *    Print a single message line using image
 * library's error handler stuff
 */

METHODDEF void
#ifdef __STDC__
imOutputMessage (j_common_ptr cinfo)
#else
imOutputMessage (cinfo)
j_common_ptr cinfo;
#endif
{
	char buffer[JMSG_LENGTH_MAX];

	/* Create the message */
	(*cinfo->err->format_message) (cinfo, buffer);

	/* This could be a warning or a fatal error.  We don't know
	 * which it is, so just call it a warning.
	 */
        ImErrorWarning(buffer, IM_NOTHING, IMEUNKNOWN );
}




/*
 *  FUNCTION
 *	imJpegRead
 *
 *  DESCRIPTION
 *	Read in a jpeg file, using the 
 *  Independent JPEG Group's library.
 */

static int				/* Returns # tags read in	*/
#ifdef __STDC__
imJpegRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )
#else
imJpegRead( ioType, fd, fp, flagsTable, tagTable )
        int         ioType;             /* I/O flags                    */
        int         fd;                 /* Input file descriptor        */
        FILE       *fp;                 /* Input file pointer           */
	TagTable   *flagsTable;		/* Flags			*/
        TagTable   *tagTable;           /* Tag list to add to           */
#endif
{
	struct jpeg_decompress_struct jpeg; /* holds data about our file */
	JSAMPARRAY buffer;			/* Buffer for reading scanlines  */
	int scanline_size;		/* Length of each scanline       */
	ImVfb* vfb;                     /* new vfb we're creating        */
	ImVfbPtr vfbptr;                /* pointer into this vfb         */
	int width, height;              /* Dimensions of the vfb         */
	char message[1000];             /* Buffer for messages           */
	int numChans; 			/* number of channels in the image */
	int i;                          /* loop index                    */
	struct im_error_mgr jerr;       /* Error struct                  */
	J_COLOR_SPACE jpg_color_space;  /* Holds the type of the image   */

	if (!(ioType & IMFILEIOFP))
	{
		fp = fdopen( fd, "rb");
		rewind (fp);
	}

	/*
	 * Set up the error handler
	 */
	jpeg.err = jpeg_std_error(&jerr.pub);  
	jerr.pub.error_exit     = im_error_exit;	/* Override standard exit routine   */
	jerr.pub.output_message = imOutputMessage;	/* Override standard output routine */
	jerr.pub.emit_message   = imEmitMessage;	/* Override standard emitting routine */

	/* Establish the setjmp return context for im_error_exit to use */
	if (setjmp(jerr.setjmp_buffer))
	{
		/* The JPEG code has signalled an error. */
		jpeg_destroy_decompress(&jpeg);
		return 0;
	}

	/* Initialize jpeg decompression object */
	jpeg_create_decompress(&jpeg);

	/* Specify the data source (fp) */
	jpeg_stdio_src(&jpeg, fp);

	/* Read JPEG header */
	jpeg_read_header(&jpeg, TRUE);

	/* Output compression type */
	ImInfo ("Compression Type","Discrete Cosine Transform");

	ImInfo ("Alpha Channel","None");

	/* Start jpeg decompression */
	jpeg_start_decompress(&jpeg);

	/* Discern the scanline size */
	scanline_size = jpeg.output_width * jpeg.output_components;

	/* Discern the image dimensions */
	height = jpeg.image_height;
	width  = jpeg.image_width;
	sprintf(message,"%d x %d",width,height);
	ImInfo("Resolution",message);

	/* DIscern the type of the image */
	jpg_color_space = jpeg.out_color_space;

	ImMalloc( buffer,    unsigned char**, sizeof(unsigned char*)* 1 );
	ImMalloc( buffer[0], unsigned char* , sizeof(unsigned char) * scanline_size );

	/* Figure out what we have, then read the stuff in! */

	numChans = jpeg.num_components;
	switch (numChans)
	{
		case 1:
			if (jpg_color_space!=JCS_GRAYSCALE)
			{
				ImErrorFatal( "Unknown color space",-1, IMEUNSUPPORTED);
			}
			ImInfo ("Type", "8-bit Greyscale");
			/* Read a greyscale / index8 image */
			vfb    = ImVfbAlloc( width, height, IMVFBGREY);
			vfbptr = ImVfbQPtr(vfb, 0, 0);

			/* Read the scanlines */
			while (jpeg.output_scanline < jpeg.output_height)
			{
				jpeg_read_scanlines(&jpeg, buffer, 1);

				/* Add scanline to the vfb */
				for (i=0; i<width; i++)
				{
					ImVfbSGrey(vfb,vfbptr, buffer[0][i]);
					ImVfbSInc (vfb,vfbptr);
				}
			}
			break;
		case 3:
			if (jpg_color_space!=JCS_RGB)
			{
				ImErrorFatal( "No support for a non-rgb color space",
				-1, IMEUNSUPPORTED);
			}
			ImInfo ("Type", "24-bit RGB");

			/* Read an rgb image */
			vfb    = ImVfbAlloc( width, height, IMVFBRGB);
			vfbptr = ImVfbQPtr(vfb, 0, 0);

			/* Read the scanlines */
			while (jpeg.output_scanline < jpeg.output_height)
			{
				jpeg_read_scanlines(&jpeg, buffer, 1);

				/* Add scanline to the vfb */
				for (i=0; i<width * 3 ; i++)
				{
					ImVfbSRed(vfb,vfbptr,   buffer[0][i++]);
					ImVfbSGreen(vfb,vfbptr, buffer[0][i++]);
					ImVfbSBlue(vfb,vfbptr,  buffer[0][i]);
					ImVfbSInc (vfb,vfbptr);
				}
			}
			break;
		default :
			ImErrorFatal ("Unknown color space", -1, IMEUNSUPPORTED);
	}
	jpeg_finish_decompress(&jpeg);
	jpeg_destroy_decompress(&jpeg);

	/* Add this vfb to the tagtable */
	TagTableAppend(tagTable, TagEntryAlloc("image vfb", POINTER, &vfb));


	return 1;
}




/*
 *  FUNCTION
 *	imJpegWrite
 *
 *  DESCRIPTION
 *	Write a jpeg file, using the IJG's library
 */

int				/* Returns # of tags used	*/
#ifdef __STDC__
imJpegWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )
#else
imJpegWrite( pMap, ioType, fd, fp, flagsTable, tagTable )
	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/
        int          ioType;            /* I/O flags                    */
        int          fd;                /* Input file descriptor        */
        FILE        *fp;                /* Input file pointer           */
	TagTable    *flagsTable;	/* Flags			*/
        TagTable    *tagTable;          /* Tag list to read from        */
#endif
{
	struct jpeg_compress_struct jpeg;	/* JPEG object                 */
	struct im_error_mgr jerr;   		/* Error hander                */
	int height, width;                      /* Size of VFB                 */
	TagEntry* tagEntry;			/* Entry in tag table          */
	ImVfb* vfb;                             /* our vfb                     */
	ImVfbPtr vfbptr;                        /* points into a vfb           */
	int x,y;				/* loop variables              */
	JSAMPARRAY buffer;			/* buffer for scanlines        */
	int bufIndex;                           /* loop index                  */
	int numChans;  				/* number of channels          */
	char message[300];                      /* For info messages           */
	int quality;                            /* Quality parameter for jpeg  */


	/* Specify data destination (file) */

	if (!(ioType & IMFILEIOFP))
	{ 
		fp = fdopen( fd, "rb");
		rewind (fp);
	}


	/*
	 * Set up the error handler
	 */
	jpeg.err = jpeg_std_error(&jerr.pub);  
	jerr.pub.error_exit     = im_error_exit;	/* Override standard exit routine   */
	jerr.pub.output_message = imOutputMessage;	/* Override standard output routine */
	jerr.pub.emit_message   = imEmitMessage;	/* Override standard emitting routine */

	/* Establish the setjmp return context for im_error_exit to use */
	if (setjmp(jerr.setjmp_buffer))
	{
		/* The JPEG code has signalled an error. */
		jpeg_destroy_compress(&jpeg);
		return 0;
	}


	/* Allocate and initialize JPEG object */
	jpeg_create_compress(&jpeg);          

	/* Specify the data destination (fp) */
	jpeg_stdio_dest(&jpeg, fp);

	/* Get the vfb from the tag table */
	tagEntry = TagTableQDirect( tagTable, "image vfb", 0 );
	TagEntryQValue( tagEntry, &vfb );

	/* Discern image resolution */
	height = ImVfbQHeight(vfb);
	width  = ImVfbQWidth (vfb);
	sprintf(message,"%d x %d",width,height);
	ImInfo("Resolution",message);

	ImInfo ("Alpha Channel","None");

	/* Set parameters in JPEG object */
	jpeg.image_width  = width;
	jpeg.image_height = height;

	/* What type of image is this? */

	numChans  = pMap->map_outNChannels;
	if (numChans == 3)
	{
		sprintf(message,"%d-bit RGB",numChans * 8);
		ImInfo("Type",message);
		jpeg.in_color_space = JCS_RGB;
	}
	else 
	{
		ImInfo("Type","8-bit grayscale");
		jpeg.in_color_space = JCS_GRAYSCALE;
	}
	
	ImInfo ("Compression Type","Discrete Cosine Transform");
	
	jpeg.input_components = numChans;

	/* Set default compression parameters */
	jpeg_set_defaults(&jpeg);   

	/* Set the quality, if a request was given. Otherwise JPEG creates a default one. */
	if (TagTableQNEntry(flagsTable, "image compression quality request") > 0)
	{
		TagEntryQValue( TagTableQDirect( flagsTable, 
			"image compression quality request", 0), &quality);
		/* Set it.  TRUE limits to baseline-JPEG values.  i.e.
		   it ensures that lots of jpeg-readers can understand
		   the file we create. */
		if (quality<0 || quality>100)
		{
			sprintf(message,"Ignoring invalid quality request: %d.  Must be between 1 and 100.",
				quality);
			ImErrorWarning (message, -1, IMEOUTOFRANGE);
		}
		sprintf(message,"%d",quality);
		ImInfo("Compression Quality",message);
		jpeg_set_quality(&jpeg, quality, TRUE);
	}

	/* Start up compression */
	jpeg_start_compress(&jpeg, TRUE);

	/* Allocate some memory for our scanline buffer */
	ImMalloc( buffer   , unsigned char**, sizeof(unsigned char*)* 1 );
	ImMalloc( buffer[0], unsigned char* , sizeof(unsigned char) * width * numChans );

	/* Start at the top */
	vfbptr = ImVfbQPtr(vfb, 0, 0);
	

	/* Write scanlines to jpeg! */
	switch (numChans)
	{
		case 1: /* Write grayscale */
			for (y=0;y<height; y++)
			{
				/* Create a buffer to hold this scanline */
				bufIndex = 0;
				for (x=0;x<width;x++)
				{
					buffer[0][bufIndex++] = ImVfbQGrey (vfb, vfbptr);
					ImVfbSInc(vfb,vfbptr);
				}
				/* Write this scanline to the jpeg thing */
				jpeg_write_scanlines(&jpeg, buffer, 1);
			}
			break;

		case 3: /* Write rgb */
			for (y=0;y<height; y++)
			{
				/* Create a buffer to hold this scanline */
				bufIndex = 0;
				for (x=0;x<width;x++)
				{
					buffer[0][bufIndex++] = ImVfbQRed  (vfb, vfbptr);
					buffer[0][bufIndex++] = ImVfbQGreen(vfb, vfbptr);
					buffer[0][bufIndex++] = ImVfbQBlue (vfb, vfbptr);
					ImVfbSInc(vfb,vfbptr);
				}
				/* Write this scanline to the jpeg thing */
				jpeg_write_scanlines(&jpeg, buffer, 1);
			}
			break;

	} /* end of switch */ 
	
	jpeg_finish_compress(&jpeg);
	jpeg_destroy_compress(&jpeg);   /* free memory */

	return 1;
}

#endif /* USE_JPEG_LIB */

