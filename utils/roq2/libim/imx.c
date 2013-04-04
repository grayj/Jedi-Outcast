/**
 **	$Header: /roq/libim/imx.c 1     11/02/99 4:38p Zaphod $
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

#define HEADER        "    $Header: /roq/libim/imx.c 1     11/02/99 4:38p Zaphod $"

/**
 **  FILE
 **	imx.c		-  Stardent AVS X image file I/O
 **
 **  PROJECT
 **	libim	-  SDSC image manipulation library
 **
 **  DESCRIPTION
 **	imx.c contains routines to read and write Stardent AVS X files for
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
 **	none
 **
 **  PRIVATE CONTENTS
 **	imXRead		f  read a Stardent AVS X file
 **	imXWrite	f  write a Stardent AVS X X file
 **
 **
 **  HISTORY
 **	$Log: /roq/libim/imx.c $
* 
* 1     11/02/99 4:38p Zaphod
 **	Revision 1.15  1995/06/29  00:28:04  bduggan
 **	updated copyright year
 **
 **	Revision 1.14  1995/04/03  21:40:55  bduggan
 **	took out #ifdef NEWMAGIC
 **
 **	Revision 1.13  1995/01/10  23:50:33  bduggan
 **	made read/write routines static
 **
 **	Revision 1.12  94/10/03  11:31:12  nadeau
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
 **	Revision 1.11  92/12/03  01:56:31  nadeau
 **	Corrected info messages.
 **	
 **	Revision 1.10  92/11/24  11:52:29  groening
 **	Removed bogus code dealing with mythical, but not actual,
 **	DEC AVS version of the X format.
 **	
 **	Revision 1.9  92/11/18  12:29:21  groening
 **	added error statements to correspond
 **	with addition of new imdecx.c file
 **	
 **	Revision 1.8  92/11/04  12:10:22  groening
 **	put ImFIleFormat info and magic number info
 **	from imfmt.c into this file.
 **	
 **	Revision 1.7  92/10/19  14:05:46  groening
 **	added ImInfo features
 **	
 **	Revision 1.6  92/09/25  12:07:04  groening
 **	added print information for ImInfo
 **	
 **	Revision 1.5  92/09/02  11:12:38  vle
 **	Fixed copyright notice.
 **	
 **	Revision 1.4  92/08/31  17:43:50  vle
 **	Updated copyright notice.
 **	
 **	Revision 1.3  91/10/03  09:24:45  nadeau
 **	Fixed problem with alpha channel handling.
 **	
 **	Revision 1.2  91/02/12  10:47:13  nadeau
 **	Removed the tag table error checking now handled by
 **	ImFileWrite.  Removed VFB conversion also now handled by
 **	ImFileWrite.
 **	
 **	Revision 1.1  91/01/30  18:17:56  nadeau
 **	Initial revision
 **	
 **/


#include "iminternal.h"


#ifndef L_SET
#define L_SET   0               /* Absolute offset                      */
#define L_CUR   1
#define L_END   2
#endif




/**
 **  FORMAT
 **	X	-  Stardent AVS X image file
 **
 **  AKA
 **	avs, mbfx, mbfavs
 **
 **  FORMAT REFERENCES
 **	AVS File Formats, Appendix E, Stardent Application Visualization
 **		System, User's Guide, Stardent
 **
 **  CODE CREDITS
 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1991.
 ** 
 **  DESCRIPTION
 **	Stardent AVS X image files start with a two 32-bit words giving the
 **	image width and height, respectively.  Following these size words
 **	are a stream of (width*height) 32-bit words giving the RGB value
 **	for consecutive pixels.  Scanlines are not padded.
 **
 **	Each 32-bit pixel value is structured as four 8-bit quantities:
 **
 **		high-order	matte (alpha)
 **				red
 **				green
 **		low-order	blue
 **/


/*
 *  X - Stardent AVS image file
 *      For information on these structures, how to use them, etc. please
 *      see imfmt.c.
 */

#ifdef __STDC__
static int imXRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );
static int imXWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, 
	TagTable *tagTable );
#else
static int imXRead( );
static int imXWrite( );
#endif

static char *imXNames[ ]  = { "x", "avs", "mbfx", "mbfavs", NULL };
static ImFileFormatReadMap imXReadMap[ ] =
{
        /* in                   out                                     */
        /* type,ch,dep, attr.   VFB type        attr.                   */
        { RGB,3,8,      A,      IMVFBRGB,       A },
        { -1,           0,      -1,             0 },
};
static ImFileFormatWriteMap imXWriteMap[ ] =
{
        /* in                   out                                     */
        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */
        { IMVFBRGB,     0,      RGB,3,8,        A,      imXWrite },
        { IMVFBRGB,     A,      RGB,3,8,        A,      imXWrite },
        { -1,           0,      -1,             0,      NULL },
};

static ImFileMagic imFileXMagic []=
{
	{ 0, 0, NULL},
};

ImFileFormat ImFileXFormat =
{
	imXNames, "AVS X image file",
	"AVS Inc.",
	"24-bit RGB + 8-bit Alpha plane uncompressed image files.",
	"24-bit RGB + 8-bit Alpha plane uncompressed image files.",
	imFileXMagic,
	IMNOMULTI, IMPIPE,
	IMNOMULTI, IMPIPE,
	imXRead, imXReadMap, imXWriteMap
};


/*
 *  FUNCTION
 *	imXRead	-  read a Stardent AVS X file
 *
 *  DESCRIPTION
 *	The image size is read in, followed by the pixel values.
 *	A VFB is added to the tag table.
 */

static int				/* Returns # tags read in	*/
#ifdef __STDC__
imXRead( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )
#else
imXRead( ioType, fd, fp, flagsTable, tagTable )
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
 *	imXWrite	-  write a Stardent AVS X file
 *
 *  DESCRIPTION
 *	That VFB is queried, and the X file header written out.
 *	The VFB data is then copied to the file.
 */

static int				/* Returns # of tags used	*/
#ifdef __STDC__
imXWrite( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )
#else
imXWrite( pMap, ioType, fd, fp, flagsTable, tagTable )
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
