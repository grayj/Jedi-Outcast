/**
 **	$Header: /roq/libim/imipw.c 1     11/02/99 4:38p Zaphod $
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

#define HEADER        "    $Header: /roq/libim/imipw.c 1     11/02/99 4:38p Zaphod $ "

/**
 **  FILE
 **	imipw.c		-  UCSB IPW image file
 **
 **  PROJECT
 **	libim	-  SDSC image manipulation library
 **
 **  DESCRIPTION
 **	imipw.c contains routines to read and write UCSB IPW files for
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
 **	ImIPWRead	f  read an IPW file
 **	ImIPWWrite	f  write an IPW file
 **
 **  PRIVATE CONTENTS
 **	none
 **
 **  HISTORY
 **	$Log: /roq/libim/imipw.c $
* 
* 1     11/02/99 4:38p Zaphod
 * Revision 1.4  1995/06/29  00:28:04  bduggan
 * updated copyright year
 *
 * Revision 1.3  1995/04/03  21:27:51  bduggan
 * took out #ifdef NEWMAGIC
 *
 * Revision 1.2  1995/01/10  23:32:25  bduggan
 * put in IMMULTI, IMPIPE instead of TRUE/FALSE
 *
 * Revision 1.1  1994/10/03  11:32:30  nadeau
 * Initial revision
 *
 * Revision 1.1  1994/10/03  11:32:30  nadeau
 * Initial revision
 *
 **	
 **/

#include "iminternal.h"


#ifndef L_SET
#define L_SET   0               /* Absolute offset                      */
#define L_CUR   1
#define L_END   2
#endif


/* header string for IPW files:						*/

/* #define HEADER	"!<header>" */




/*
 *  FORMAT
 *	IPW	-  UCSB Image Processing Workbench image file
 *
 *  DESCRIPTION
 *	UCSB IPW image files start with a two 32-bit words giving the
 *	image width and height, respectively.  Following these size words
 *	are a stream of (widht*height) 32-bit words giving the RGB value
 *	for consecutive pixels.  Scanlines are not padded.
 *
 *	Each 32-bit pixel value is structured as four 8-bit quantities:
 *
 *		high-order	matte (alpha)
 *				red
 *				green
 *		low-order	blue
 */


/*
 *  IPW - UCSB IPW image file
 *      For information on these structures, how to use them, etc. please
 *      see imfmt.c.
 */
#ifdef __STDC__
extern int ImIPWRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );
extern int ImIPWWrite(ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );
#else
extern int ImIPWRead( );
extern int ImIPWWrite( );
#endif
static char *imIPWNames[ ]  = { "ipw", "ucsb", "mbfx", "mbfavs", NULL };
static ImFileFormatReadMap imIPWReadMap[ ] =
{
        /* in                   out                                     */
        /* type,ch,dep, attr.   VFB type        attr.                   */
        { RGB,3,8,      A,      IMVFBRGB,       A },
        { -1,           0,      -1,             0 },
};
static ImFileFormatWriteMap imIPWWriteMap[ ] =
{
        /* in                   out                                     */
        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */
        { IMVFBRGB,     0,      RGB,3,8,        A,      ImIPWWrite },
        { IMVFBRGB,     A,      RGB,3,8,        A,      ImIPWWrite },
        { -1,           0,      -1,             0,      NULL },
};

static ImFileMagic imFileIPWMagic[ ] =
{
	{ 0,0,NULL },
};



ImFileFormat ImFileXFormat =
{
	imIPWNames, "UCSB IPW image file",
	"UCSB",
	"8-bit grayscale uncompressed image files.",
	"24-bit RGB uncompressed image files.",
	imFileIPWMagic,
	IMNOMULTI, IMPIPE,	/* Read  */
	IMNOMULTI, IMPIPE,	/* Write */
	ImIPWRead, imIPWReadMap, imIPWWriteMap
};


/*
 *  FUNCTION
 *	ImIPWRead	-  read a UCSB IPW file
 *
 *  DESCRIPTION
 *	The file header is read in, followed by the pixel values.
 *	One VFB per band is added to the tag table.
 */

int				/* Returns # tags read in	*/
#ifdef __STDC__
ImIPWRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )
#else
ImIPWRead( ioType, fd, fp, flagsTable, tagTable )
	int         ioType;		/* I/O flags			*/
	int         fd;			/* Input file descriptor	*/
	FILE	   *fp;			/* Input file pointer		*/
	TagTable   *flagsTable;		/* Flags			*/
	TagTable   *tagTable;		/* Tag table to add to		*/
#endif
{
	ImVfb        *vfb;		/* Read in image		*/
	ImVfbPtr     pPixel;		/* Pixel pointer		*/
	sdsc_uint32      *buffer;		/* Run buffer			*/
	sdsc_uint32      *pBuffer;		/* Run buffer pointer		*/
	int          width, height;	/* Image dimensions		*/
	int          i;			/* Counter			*/
	char	     message[200];	/* contains information for ImInfo */

	/*
	 *  Read in the image size.
	 */
	BinByteOrder( BINMBF );
	if ( ImBinRead( ioType, fd, fp, &width, INT, 4, 1 ) == -1 )
	{
		ImReturnBinError( );
	}
	if ( ImBinRead( ioType, fd, fp, &height, INT, 4, 1 ) == -1 )
	{
		ImReturnBinError( );
	}

	sprintf (message, "Most Significant Byte First");
	ImInfo ("Byte Order",message);
	sprintf (message, "%d x %d", width, height);
	ImInfo ("Resolution",message);
	ImInfo ("Type","24-bit RGB");
	ImInfo ("Alpha Channel","8-bit");

	/*
	 *  Allocate an RGB and Alpha plane VFB of the required size.
	 */
	if ( (vfb = ImVfbAlloc( width, height, IMVFBRGB|IMVFBALPHA )) == IMVFBNULL )
	{
		ImErrorFatal( ImQError( ), -1, ImErrNo );
	}


	/*
	 *  Read in and copy to the VFB, one scanline at a time.
	 */
	pPixel = ImVfbQFirst( vfb );
	ImMalloc( buffer, sdsc_uint32 *, sizeof( sdsc_uint32 ) * width );
	while ( height-- )
	{
		pBuffer = buffer;
		if ( ImBinRead( ioType, fd, fp, buffer, UINT32, 4, width )== -1)
		{
			free( (char *)buffer );
			ImVfbFree( vfb );
			ImReturnBinError( );
		}

		for ( i = 0; i < width; i++, pBuffer++ )
		{
			ImVfbSAlpha( vfb, pPixel, ((*pBuffer)>>24) & 0xFF );
			ImVfbSRed(   vfb, pPixel, ((*pBuffer)>>16) & 0xFF );
			ImVfbSGreen( vfb, pPixel, ((*pBuffer)>>8)  & 0xFF );
			ImVfbSBlue(  vfb, pPixel, (*pBuffer)       & 0xFF );
			ImVfbSInc(   vfb, pPixel );
		}
	}
	free( (char *)buffer );

	TagTableAppend( tagTable, TagEntryAlloc( "image vfb", POINTER, &vfb ) );

	return ( 1 );
}





/*
 *  FUNCTION
 *	ImIPWWrite	-  write a UCSB IPW file
 *
 *  DESCRIPTION
 *	That VFB is queried, and the X file header written out.
 *	The VFB data is then copied to the file.
 */

int				/* Returns # of tags used	*/
#ifdef _STDC__
ImIPWWrite(ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )
#else
ImIPWWrite( pMap, ioType, fd, fp, flagsTable, tagTable )
	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/
	int          ioType;		/* I/O flags			*/
	int          fd;		/* Input file descriptor	*/
	FILE	    *fp;		/* Input file pointer		*/
	TagTable   *flagsTable;		/* Flags			*/
	TagTable    *tagTable;		/* Tag table to read from	*/
#endif
{
	ImVfb        *vfb;		/* Read in image		*/
	ImVfbPtr     pPixel;		/* Pixel pointer		*/
	sdsc_uint32      *buffer;		/* Run buffer			*/
	sdsc_uint32      *pBuffer;		/* Run buffer pointer		*/
	int          width, height;	/* Image dimensions		*/
	int          i;			/* Counter			*/
	char	     message[200];	/* contains information for ImInfo */


	/*
	 *  Write out the image size.
	 */
	TagEntryQValue( TagTableQDirect( tagTable, "image vfb", 0 ), &vfb );
	width  = ImVfbQWidth( vfb );
	height = ImVfbQHeight( vfb );
	BinByteOrder( BINMBF );
	if ( ImBinWrite( ioType, fd, fp, &width, INT, 4, 1 ) == -1 )
	{
		ImReturnBinError( );
	}
	if ( ImBinWrite( ioType, fd, fp, &height, INT, 4, 1 ) == -1 )
	{
		ImReturnBinError( );
	}


	sprintf (message, "Most Significant Byte First");
	ImInfo ("Byte Order",message);
	sprintf (message, "%d x %d", width, height);
	ImInfo ("Resolution",message);
	ImInfo ("Type","24-bit RGB" );
	ImInfo ("Alpha Channel","8-bit");

	/*
	 *  Copy the image to the file.  If there's an alpha channel, add it.
	 */
	pPixel = ImVfbQFirst( vfb );
	ImMalloc( buffer, sdsc_uint32 *, sizeof( sdsc_uint32 ) * width );
	if ( ImVfbQFields( vfb ) & IMVFBALPHA )
	{
		while ( height-- )
		{
			pBuffer = buffer;
			for ( i = 0; i < width; i++, pBuffer++ )
			{
				*pBuffer =
					(((ImVfbQAlpha(vfb,pPixel))&0xFF)<<24) |
					(((ImVfbQRed(vfb,pPixel))&0xFF)<<16)|
					(((ImVfbQGreen(vfb,pPixel))&0xFF)<<8)|
					((ImVfbQBlue(vfb,pPixel))&0xFF);
				ImVfbSInc( vfb, pPixel );
			}

			if ( ImBinWrite( ioType, fd, fp, buffer, UINT32, 4, width ) == -1 )
			{
				free( (char *)buffer );
				ImReturnBinError( );
			}
		}
		free( (char *)buffer );
		return ( 1 );
	}

	while ( height-- )
	{
		pBuffer = buffer;
		for ( i = 0; i < width; i++, pBuffer++ )
		{
			*pBuffer =
				(((ImVfbQRed(vfb,pPixel))&0xFF)<<16)  |
				(((ImVfbQGreen(vfb,pPixel))&0xFF)<<8) |
				((ImVfbQBlue(vfb,pPixel))&0xFF);
			ImVfbSInc( vfb, pPixel );
		}

		if ( ImBinWrite( ioType, fd, fp, buffer, UINT32, 4, width)== -1)
		{
			free( (char *)buffer );
			ImReturnBinError( );
		}
	}
	free( (char *)buffer );
	return ( 1 );
}
