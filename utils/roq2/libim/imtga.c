/**

 **     $Header: /roq/libim/imtga.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER  "    $Header: /roq/libim/imtga.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imtga.c		- Targa  

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	imtga.c contains routines to read and write Targa image files for

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

 **

 **	imTgaRead		f  read a Targa file

 **     imTgaWriteRGB16   	f  write an uncomp RGB file with 16 bits/pixel

 **     imTgaWriteRGB24   	f  write an uncomp RGB file with 24 bits/pixel

 **     imTgaWriteRGB32   	f  write an uncomp RGB file with 32 bits/pixel

 **     imTgaWriteRGBRLE16   	f  write an RLE comp RGB file with 16 bits/pixel

 **     imTgaWriteRGBRLE24   	f  write an RLE comp RGB file with 24 bits/pixel

 **     imTgaWriteRGBRLE32   	f  write an RLE comp RGB file with 32 bits/pixel

 **     imTgaWriteCLT8   	f  write an uncomp CLT file with 8 bits/pixel

 **     ImTgaWritCLT16		f  write an uncomp CLT file with 16 bits/pixel

 **     imTgaWriteCLTRLE8   	f  write an RLE comp CLT file with 8 bits/pixel

 **     imTgaWriteCLTRLE16   	f  write an RLE comp CLT file with 16 bits/pixel

 **     imTgaWriteGREY8   	f  write an uncomp grscl file with 8 bits/pixel

 **     ImTgaWritGREY16		f  write an uncomp grscl file with 16 bits/pixel

 **     imTgaWriteGREYRLE8   	f  write an RLE comp grsclfile with 8 bits/pixel

 **     imTgaWriteGREYRLE16   	f  write an RLE comp grscfile with 16 bits/pixel

 **

 **	imTga2ByteExpand_PIX( ); f  expands 2 bytes of color into the vfb

 **	imTga2ByteExpand_CLT( ); f  expands 2 bytes of color into the clt

 **	imReadTga_1and3();	 f  reads in a Targa type 1 or type 3 file

 **	imReadTga_2();	 	 f  reads in a Targa type 2 file

 **	imReadTga_9and11();	 f  reads in a Targa type 9 or type 11 file

 **	imReadTga_10();	 	 f  reads in a Targa type 10 file 

 **	imTgaWriteHeader();	 f  writes out a Targa header file

 **	imTgaWriteRLE8();	 f  writes out a RLE encoded 8bit/pixel file

 **	imTgaWrite8image();	 f  writes out an unencoded 8bit/pixel file

 **	imTgaWriteRLE16();	 f  writes out a RLE encoded 16bit/pixel file

 **	imTgaWrite16image();	 f  writes out an unencoded 16bit/pixel file

 **

 **

 **  HISTORY

 **     $Log: /roq/libim/imtga.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **     Revision 1.17  1995/06/29  00:28:04  bduggan

 **     updated copyright year

 **

 **     Revision 1.16  1995/06/15  21:15:56  bduggan

 **     removed unreachable code.

 **     changed spelling of significant

 **

 **     Revision 1.15  1995/05/17  23:47:42  bduggan

 **     Fixed crummy rle encoding algorithm

 **

 **     Revision 1.14  1995/04/03  21:37:40  bduggan

 **     took out #ifdef NEWMAGIC

 **

 **     Revision 1.13  1995/01/10  23:44:43  bduggan

 **     put in IMMULTI, IMPIPE instead of TRUE/FALSE

 **     made read/write routines static

 **

 **     Revision 1.12  94/10/03  11:30:59  nadeau

 **     Updated to ANSI C and C++ compatibility.

 **     Removed all use of register keyword.

 **     Minimized use of custom SDSC types (e.g., uchar vs. unsigned char)

 **     Changed all float arguments to double.

 **     Added forward declarations.

 **     Added misc. casts to passify SGI and DEC compilers.

 **     Changed all macros and defined constants to have names

 **     starting with IM.

 **     Rearranged magic number structures for format handlers.

 **     Made format handler routines static (i.e., local to file).

 **     Updated comments, adding format descriptions and references.

 **     Updated indenting on some code.

 **     Updated copyright message.

 **     

 **     Revision 1.11  92/12/03  01:52:48  nadeau

 **     Corrected info messages.

 **     

 **     Revision 1.10  92/11/04  12:08:08  groening

 **     put ImFIleFormat info and magic number info

 **     from imfmt.c into this file.

 **     

 **     Revision 1.9  92/10/19  14:18:54  groening

 **     forg

 **     forgot to remove debug statements

 **     

 **     Revision 1.8  92/10/19  14:08:59  groening

 **     fixed bug in rle

 **     added ImInfo stuff again

 **     file was mangled in disk crash

 **     

 **     Revision 1.7  92/09/25  12:05:58  groening

 **     added print information for iminfo

 **     

 **     Revision 1.6  92/09/05  11:33:32  groening

 **     fixed bug in read index 16

 **     

 **     Revision 1.5  92/09/03  18:12:36  vle

 **     Fixed vfb initialization bug in imTgaWrite16image().

 **     

 **     Revision 1.4  92/09/02  17:38:08  groening

 **     fixed insignificant stupid bug in index 16 conversion

 **     

 **     Revision 1.3  92/08/31  17:36:06  vle

 **     Updated copyright notice.

 **     

 **     Revision 1.2  92/08/24  14:28:49  groening

 **     minor stuff

 **     

 **     Revision 1.1  92/06/02  09:33:24  groening

 **     Initial revision

 **     

 **	

 **/



#include "iminternal.h"



/**

 **  FORMAT

 **	TGA	-  Truevision Targa image file

 **

 **  AKA

 **	vda,ivb

 **

 **  FORMAT REFERENCES

 **	Supercharged Bitmapped Graphics, Steve Rimmer

 **	Graphics File Formats, David C. Kay, John R. Levine

 **

 **  CODE CREDITS

 **     Custom development, Chris Groening, San Diego Supercomputer Center, 1992.

 **

 **  DESCRIPTION

 **	There are nine different ways that a targa file can be stored:

 **	   0 - No image data included.

 **	   1 - Uncompressed, color-mapped images.

 **	   2 - Umcompressed, RGB images.

 **	   3 - Uncompressed, black and white images.

 **	   9 - Runlength encoded color mapped images.

 **	  10 - Runlength encoded RGB images.

 **	  11 - Compressed black and white images.

 **	  32 - Compressed color-mapped data, using Huffman, Delta

 **						 and RLE encoding.

 **	  33 - Compressed color-mapped data, using Huffman, Delta

 **		and RLE encoding, 4-pass quadtree-type process.

 **

 *************************************************************************

 **    AS OF NOW ONLY TYPES 0,1,2,3,9,10,11 ARE SUPPORTED

 **

 **    2 AND 4 WAY TYPE INTERLEAVING ARE NOT SUPPORTED

 ** 

 **    COLOR MAP TABLES ARE ONLY WRITTEN RED, GREEN, BLUE WITH 1 BYTE EACH

 **

 **    NOTE: Word form truevision has it that types 32 and 33 are obsolete, and

 **    were never that widespread.  The same with two and four way interleaving.

 *************************************************************************

 **

 **	The 'header' lies in the first 18 bytes of the targa file.

 **	It is described as follows:

 **

 ** 	OFFSET	LENGTH			DESCRIPTION

 **    -----------------------------------------------------------------

 **    |       |       |                                               |

 **    |   0   |   1   | Number of characters in Identification field. |

 **    |       |       |                                               |

 **    |       |       | This field is a one byte unsigned integer,    |

 **    |       |       | specifying the length of the Image            |

 **    |       |       | Identification Filed. It ranges from 0-255.   |

 **    |       |       | A 0 means that no Image Ident. field is there.|

 **    |       |       |                                               |

 **    -----------------------------------------------------------------

 **    |       |       |                                               |

 **    |   1   |   1   | Color Map type                                |

 **    |       |       |                                               |

 **    |       |       | 0 - no color map included.                    |

 **    |       |       | 1 - color map included.                       |

 **    |       |       |                                               |

 **    -----------------------------------------------------------------

 **    |       |       |                                               |

 **    |   2   |   1   | Image Type Code                               |

 **    |       |       |                                               |

 **    |       |       | What kind of Data type is in the file.        |

 **    |       |       | ('magic number'? (0,1,2,3,9,10,11,32,33))     |

 **    |       |       |                                               |

 **    -----------------------------------------------------------------

 **    |       |       |                                               |

 **    |   3   |   5   | Color map Specification                       |

 **    |       |       |                                               |

 **    |   3   |   2   | Color Map Origin                              |

 **    |       |       | Integer (lo -hi) index of first colormap entry|

 **    |       |       |                                               |

 **    |   5   |   2   | Color Map Length                              |

 **    |       |       | Integer (lo-hi) count of colormap entries     |

 **    |       |       |                                               |

 **    |   7   |   1   | Color Map entry Size                          |

 **    |       |       | Number of bits in color map entry.            |

 **    |       |       | 16 for Targa 16, 24 for targa 24, 32 for 32   |

 **    |       |       |                                               |

 **    -----------------------------------------------------------------

 **    |       |       |                                               |

 **    |   8   |  10   | Image specification                           |

 **    |       |       |                                               |

 **    |   8   |   2   | X origin of Image.                            |

 **    |       |       | Integer (lo-hi) X coordinate of lower left    |

 **    |       |       | corner of image.                              |

 **    |       |       |                                               |

 **    |   10  |   2   | Y origin of Image.                            |

 **    |       |       | Integer (lo-hi) Y coordinate of lower left    |

 **    |       |       | corner of image.                              |

 **    |       |       |                                               |

 **    |   12  |   2   | Width of image.                               |

 **    |       |       | Integer (lo-hi) width of image in pixels.     |

 **    |       |       |                                               |

 **    |   14  |   2   | Length of image.                              |

 **    |       |       | Integer (lo-hi) length of image in pixels.    |

 **    |       |       |                                               |

 **    |   16  |   l   | Image Pixel size                              |

 **    |       |       | Number of bits in a pixel, 16 for Targa 16 etc|

 **    |       |       |                                               |

 **    |   17  |   1   | Image Descriptor Byte                         |

 **    |       |       | BITS 3-0: number of attribute bits associated |

 **    |       |       | with each pixel. For targa 16 this should be  |

 **    |       |       | 0 or 1.  For the Targa 24 it should be 0 and  |

 **    |       |       | for the 32 it should be 8                     |

 **    |       |       | BIT 4 & BIT 5   : Pixel data storage order    |

 **    |       |       |                Screen dest of first pixel     |

 **    |       |       |   0      0        Bottom left                 |

 **    |       |       |   0      1        Top left                    |

 **    |       |       |   1      0        Bottom right                |

 **    |       |       |   1      1        Top right                   |

 **    |       |       | BITS 7-6: must be zero			       |

 **    |       |       |                                               |

 **    -----------------------------------------------------------------

 **    |       |       |                                               |

 **    |  18   |varies | Image identification field.                   |

 **    |       |       | Contains a free-form identification field of  |

 **    |       |       | the length specified in byte 1 of the image   |

 **    |       |       | record.  It's usually omitted (byte 1 =0), but|

 **    |       |       | can be up to 255 characters. If more info is  |

 **    |       |       | needed, it can be stored after the image data |

 **    |       |       |                                               |

 **    -----------------------------------------------------------------

 **    |       |       |                                               |

 **    |varies |varies | Color Map Data                                |

 **    |       |       | The offset is determined by the size of the   |

 **    |       |       | Image Identifaction Field.                    |

 **    |       |       | The length is determined by the Color Map     |

 **    |       |       | Specification, which describes the size of    |

 **    |       |       | each entry and the number of entries.         |

 **    |       |       | Each color map entry is 2, 3, or 4 bytes.     |

 **    |       |       | Unused bits are assumed to specify attribute  |

 **    |       |       | bits.                                         |

 **    |       |       |                                               |

 **    |       |       | 4 bytes - 1 red, 1 green, 1 blue and 1 att.   |

 **    |       |       |                                               |

 **    |       |       | 3 bytes - 1 red, 1 green, 1 blue.             |

 **    |       |       |                                               |

 **    |       |       | 2 bytes - ARRRRRGG GGGBBBBB each letter= 1 bit|

 **    |       |       |   There is lo-hi storage order so bytes will  |

 **    |       |       |   be reversed                                 |

 **    |       |       |                                               |

 **    -----------------------------------------------------------------

 **    |       |       |                                               |

 **    |varies |varies |  Image Data Field                             |

 **    |       |       |                                               |

 **    |       |       |  This field specifies (width) x (height)      |

 **    |       |       |  color map indices.  The indices are stored in|

 **    |       |       |  packets.  There are many kinds of packets.   |

 **    |       |       |  they consist of a 1 byte header, which       |

 **    |       |       |  idenitfies the type of packet and specifies  |

 **    |       |       |  a count, folowed by a variable-length body.  |

 **    |       |       |                                               |

 **    |       |       |  Run-length packet:                           |

 **    |       |       |   First bit ID =1 then 7 bit repetition count |

 **    |       |       |   minus one.  (largest possible size is 128)  |

 **    |       |       |                                               |

 **    |       |       |  Raw packet:                                  |

 **    |       |       |   First bit ID =0 then 7 bit repetition count |

 **    |       |       |   minus one.  (largest possible size is 128)  |

 **    |       |       |                                               |

 **    |       |       |  Both of these can cross scanlines            |

 **    |       |       |                                               |

 **    -----------------------------------------------------------------

 **/



/*

 *  TGA - Targa File Format

 *      For information on these structures, how to use them, etc. please

 *      see imfmt.c.

 *      TGA

 */

#ifdef __STDC__

static int imTgaRead( int ioType, int fd, FILE* fp, TagTable *flagsTable, TagTable *tagTable );

static int imTgaWriteRGB16( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imTgaWriteRGBRLE16( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE* fp, TagTable *flagsTable, TagTable *tagTable );

static int imTgaWriteRGB24( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int imTgaWriteRGBRLE24( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

static int imTgaWriteRGB32( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

static int imTgaWriteRGBRLE32( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

static int imTgaWriteCLT8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

static int imTgaWriteCLTRLE8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

static int imTgaWriteCLT16( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

static int imTgaWriteCLTRLE16( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

static int imTgaWriteGREY8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

static int imTgaWriteGREYRLE8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

static int imTgaWriteGREY16( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

static int imTgaWriteGREYRLE16( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable);

#else

static int imTgaRead();

static int imTgaWriteRGB16();

static int imTgaWriteRGBRLE16();

static int imTgaWriteRGB24(); 

static int imTgaWriteRGBRLE24();

static int imTgaWriteRGB32( );

static int imTgaWriteRGBRLE32(); 

static int imTgaWriteCLT8(); 

static int imTgaWriteCLTRLE8(); 

static int imTgaWriteCLT16( );

static int imTgaWriteCLTRLE16( );

static int imTgaWriteGREY8( );

static int imTgaWriteGREYRLE8();

static int imTgaWriteGREY16( );

static int imTgaWriteGREYRLE16( );

#endif

static char *imTgaNames[ ] = { "tga", "vda", "ivb",  NULL};

static ImFileFormatReadMap imTgaReadMap[ ] =

{

        /* in                   out                                     */

        /* type, ch, dep, attr.  VFB type       attr.                   */

        { IN,1,8,       C|RLE,  IMVFBINDEX8,    C},

        { IN,1,16,      C|RLE,  IMVFBINDEX16,   C},

        { RGB,3,8,      C|RLE,  IMVFBRGB,       C},

        { RGB,3,8,      A|C|RLE,IMVFBRGB,       A|C},

        { RGB,3,5,      A|C|RLE,IMVFBRGB,       A|C},

        { RGB,3,5,      A|RLE,  IMVFBRGB,       A},

        { RGB,3,8,      RLE,    IMVFBRGB,       0},

        { RGB,3,8,      A|RLE,  IMVFBRGB,       A},

        { IN,1,8,       RLE,    IMVFBGREY,      0},

        { IN,1,16,      RLE,    IMVFBGREY,      0},

        { IN,1,8,       C,      IMVFBINDEX8,    C},

        { IN,1,16,      C,      IMVFBINDEX16,   C},

        { RGB,3,8,      C,      IMVFBRGB,       C},

        { RGB,3,8,      A|C,    IMVFBRGB,       A|C},

        { RGB,3,5,      A|C,    IMVFBRGB,       A|C},

        { RGB,3,5,      A,      IMVFBRGB,       A},

        { RGB,3,8,      0,      IMVFBRGB,       0},

        { RGB,3,8,      A,      IMVFBRGB,       A},

        { IN,1,8,       0,      IMVFBGREY,      0},

        { IN,1,16,      0,      IMVFBGREY,      0},

        { -1,           0,      -1,             0},

};



static ImFileFormatWriteMap imTgaWriteMap[ ] =

{

        /* in                   out                                     */

        /* VFB type,    attr.,  type,ch,dep,    attr.,  func            */



        { IMVFBINDEX8,  C,      IN,1,8,         C|RLE,  imTgaWriteCLTRLE8  },

        { IMVFBINDEX8,  C,      IN,1,8,         C,      imTgaWriteCLT8  },

        { IMVFBINDEX8,  0,      IN,1,8,         RLE,    imTgaWriteGREYRLE8  },

        { IMVFBINDEX8,  0,      IN,1,8,         0,      imTgaWriteGREY8 },

        { IMVFBINDEX16, 0,      IN,1,16,        RLE,    imTgaWriteGREYRLE16  },

        { IMVFBINDEX16, 0,      IN,1,16,        0,      imTgaWriteGREY16 },

        { IMVFBINDEX16, C,      IN,1,16,        C|RLE,  imTgaWriteCLTRLE16  },

        { IMVFBINDEX16, C,      IN,1,16,        C,      imTgaWriteCLT16  },

        { IMVFBRGB,     0,      RGB,3,8,        RLE,    imTgaWriteRGBRLE24  },

        { IMVFBRGB,     A,      RGB,3,8,        A|RLE,  imTgaWriteRGBRLE32  },

        { IMVFBRGB,     0,      RGB,3,8,        0,      imTgaWriteRGB24  },

        { IMVFBRGB,     A,      RGB,3,8,        A,      imTgaWriteRGB32  },

        { IMVFBRGB,     A,      RGB,3,5,        A,      imTgaWriteRGB16  },

        { IMVFBRGB,     A,      RGB,3,5,        A|RLE,  imTgaWriteRGBRLE16  },

        { -1,           0,      -1,             0},

};



static ImFileMagic imFileTgaMagic []=

{

	{ 0, 0, NULL},

};



ImFileFormat ImFileTgaFormat =

{

	imTgaNames, "Truevision Targa image file",

	"Truevision",

	"8- and 16-bit monochrome, 8- and 16-bit color index, 16-, 24-, and\n\
32-bit RGB+alpha, uncompressed (standard) and RLE compressed image files.",
	"8- and 16-bit monochrome, 8- and 16-bit color index, 16-, 24-, and\n\
32-bit RGB+alpha, uncompressed (standard) and RLE compressed image files.",
	imFileTgaMagic,

	IMNOMULTI, IMNOPIPE,

	IMNOMULTI, IMNOPIPE,

	imTgaRead, imTgaReadMap, imTgaWriteMap

};



#ifdef __STDC__

static void imTga2ByteExpand_PIX (int byte2, int byte1, ImVfbPtr pPixel, ImVfb *vfb);

static void imTga2ByteExpand_CLT (int byte2, int byte1, ImCltPtr pColor);

static int  imReadTga_1and3 (int ioType,int fd,FILE *fp,ImVfb *vfb, int q,int h,int w);

static int  imReadTga_2 (int ioType,int fd,FILE *fp,ImVfb *vfb, int q,int h,int w);

static int  imReadTga_9and11 (int ioType,int fd,FILE* fp,ImVfb *vfb, int q,int h,int w);

static int  imReadTga_10 (int ioType,int fd,FILE* fp,ImVfb *vfb, int q,int h,int w);

static int  imTgaWriteHeader( int ioType, int fd, FILE *fp,TagTable *flagsTable, TagTable *tagTable,int stype,int spix,int cmsize );

static int  imTgaWriteRLE8(  int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int  imTgaWrite8image( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int  imTgaWriteRLE16( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

static int  imTgaWrite16image( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable );

#else

static void imTga2ByteExpand_PIX( );

static void imTga2ByteExpand_CLT( );

static int  imReadTga_1and3();

static int  imReadTga_2();

static int  imReadTga_9and11();

static int  imReadTga_10();

static int  imTgaWriteHeader();

static int  imTgaWriteRLE8();

static int  imTgaWrite8image();

static int  imTgaWriteRLE16();

static int  imTgaWrite16image();

#endif



#define	IMTGA_NODATA		0

#define	IMTGA_UNCOMPCOLORMAP	1

#define	IMTGA_UNCOMPRGB		2

#define	IMTGA_UNCOMPBW		3

#define	IMTGA_RLECOLORMAP	9

#define	IMTGA_RLERGB		10

#define	IMTGA_RLEBW		11

#define	IMTGA_HUFF		32

#define	IMTGA_HUFF4TYPE		33





/*  TYPEDEF & STRUCTURE

 *  imTgaHeaderInfo	- Targa file header information

 *  imTgaHeaderFields	- Targa file header fields for binary package

 */



typedef struct imTgaHeaderInfo

{

	int    tga_numc_ident;  /* number of chars. in ident. field 	*/

        unsigned short tga_cmtype;	/* whether or not a colormap exists 	*/

        int    tga_stype;	/* Storage type 			*/

	int    tga_cmorigin;	/* Color map origin 			*/

	int    tga_cmlen;	/* Color map length 			*/

	int    tga_cmsize;	/* Color map entry size 		*/

	int    tga_imxorg;	/* X origin of image 			*/

	int    tga_imyorg;	/* Y origin of image 			*/

	int    tga_imwid;	/* Width of image 			*/

	int    tga_imhgt;	/* Height of image 			*/

	int    tga_spix;	/* Image pixel size 			*/

	int    tga_imdesc;	/* Image descriptor byte		*/

} imTgaHeaderInfo;



static BinField imTgaHeaderFields[]=

{

	{ UINT, 1, 1   },	/* tga_numc_ident		*/

	{ USHORT, 1, 1 },       /* tga_cmtype 			*/

	{ UINT, 1, 1   },	/* tga_stype 			*/

	{ UINT, 2, 1   },	/* tga_cmorigin			*/

	{ UINT, 2, 1   },	/* tga_cmlen			*/

	{ UINT, 1, 1   },	/* tga_cmsize			*/

	{ UINT, 2, 1   },	/* tga_imxorg			*/

	{ UINT, 2, 1   },	/* tga_imyorg			*/

	{ UINT, 2, 1   },	/* tga_imwid			*/

	{ UINT, 2, 1   },	/* tga_imlen			*/

	{ UINT, 1, 1   },	/* tga_spix			*/

	{ UINT, 1, 1   },       /* tga_imdesc			*/

	{ 0, 0, 0}

};





/*  FUNCTION

 *

 *  imTga2ByteExpand_PIX

 *

 *  DESCRIPTION

 *  Takes a long word (2 bytes) which is configured as per Targa

 *  specs as ARRRRRGG GGGBBBBB.  It then breaks it up into the

 *  corresponding components.  And adds them into the vfb.

 */

static void

#ifdef __STDC__

imTga2ByteExpand_PIX (int byte2, int byte1, ImVfbPtr pPixel, ImVfb* vfb)

#else

imTga2ByteExpand_PIX (byte2, byte1, pPixel, vfb)

	int	byte2,byte1;		/* 2bytes to be expanded 	*/

	ImVfbPtr pPixel;	

	ImVfb	*vfb;

#endif

{

	int	expatr;			/* The attribute bit		*/

	int     expred;			/* The five red bits		*/

	int     expgreen;		/* The five green bits		*/

	int     expblue;		/* The five blue bits		*/ 

	int	exptemp;

	expatr =   (byte1 &0x80) >> 7;

	expred =   (byte1 &0x7c) >> 2;

	expgreen = (byte1 &0x03) << 3;

	exptemp =  (byte2 &0xe0) >> 5;

	expgreen = (expgreen |exptemp);

	expblue =  (byte2 &0x1f) ;



	ImVfbSRed  (vfb, pPixel, expred*8);

	ImVfbSGreen (vfb, pPixel, expgreen*8);

	ImVfbSBlue (vfb, pPixel, expblue*8);

	ImVfbSAlpha (vfb, pPixel, expatr);

}





/*  FUNCTION

 *

 *  imTga2ByteExpand_CLT

 *

 *  DESCRIPTION

 *  Takes a long word (2 bytes) which is configured as per Targa

 *  specs as ARRRRRGG GGGBBBBB.  It then breaks it up into the

 *  corresponding components.  And adds them into the clt.

 */

static void

#ifdef __STDC__

imTga2ByteExpand_CLT (int byte2, int byte1, ImCltPtr pColor)

#else

imTga2ByteExpand_CLT (byte2, byte1, pColor)

	int	byte2,byte1;		/* 2bytes to be expanded 	*/

	ImCltPtr pColor;	

#endif

{

	int	expatr;			/* The attribute bit		*/

	int     expred;			/* The five red bits		*/

	int     expgreen;		/* The five green bits		*/

	int     expblue;		/* The five blue bits		*/ 

	int	exptemp;



	expatr =   (byte1 &0x80) >> 7;

	expred =   (byte1 &0x7c) >> 2;

	expgreen = (byte1 &0x03) << 3;

	exptemp =  (byte2 &0xe0) >> 5;

	expgreen = (expgreen |exptemp);

	expblue =  (byte2 &0x1f) ;



	ImCltSBlue (pColor, expblue);

	ImCltSGreen (pColor, expgreen);

	ImCltSRed  (pColor, expred);

}





/*

 *  FUNCTION

 *	ImXRead	-  read a Stardent AVS X file

 *

 *  DESCRIPTION

 *	The image size is read in, followed by the pixel values.

 *	A VFB is added to the tag table.

 */



static int				/* returns number of tags read in */

#ifdef __STDC__

imTgaRead( int ioType, int fd, FILE* fp, TagTable *flagsTable, TagTable *tagTable )

#else

imTgaRead( ioType, fd, fp, flagsTable, tagTable )

	int         ioType;		/* I/O flags			*/

	int         fd;			/* Input file descriptor	*/

	FILE	   *fp;			/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable   *tagTable;		/* Tag table to add to		*/

#endif

{

	unsigned char *pBuffer;		/* buffer pointer		*/

	ImVfbPtr pPixel;		/* VFB pixel pointer		*/

	ImVfb        *vfb;		/* Read in image		*/

        ImClt	     *clt;		/* New clt			*/	

	ImCltPtr     pColor;		/* CLT entry pointer		*/

	imTgaHeaderInfo  header;	/* Tga header info pointer 	*/

	unsigned char *buffer;		/* Run buffer			*/

	char	     *TgaName;		/* Name of targa image in file  */

	int          i,x,y;		/* Counter			*/

	int	     colortable;	/* color map   flag		*/

	int	     n,m,h,w,z,q;	/* convenient short names	*/

	int	     tga_attbits;	/* number of attribute bits per pixel */

	int          tga_orgbita;	/* screen origin bit 4	 	*/

	int          tga_orgbitb;	/* screen origin bit 5	 	*/

	int 	     tga_rsrvd;		/* data storage interleave bits */

	char	     message[500];	/* Tmp error message text	*/



	/* the binary byte order is lo-hi */

	BinByteOrder (BINLBF);



	/*

	 *  Read in the header..

	 */

	if (ImBinReadStruct (ioType, fd, fp, &header, imTgaHeaderFields)==-1)

	{

		ImReturnBinError();

	}

	w = header.tga_imwid;

	h = header.tga_imhgt;

	z = header.tga_stype;

	q = header.tga_spix;

	tga_attbits = header.tga_imdesc & 0xf; 

	tga_orgbita = (header.tga_imdesc & 0x10) >> 4; 

	tga_orgbitb = (header.tga_imdesc & 0x20) >> 5; 

	tga_rsrvd = (header.tga_imdesc & 0xc0) >> 6; 



	/* makes sure the type is one we deal with, check color map

	   flag if there should be one */

	colortable=0;

	switch (z)

	{

		case IMTGA_NODATA:

			ImErrorFatal (ImQError(), -1, IMENOIMAGE);

			/* break; (not reached) */

		case IMTGA_UNCOMPCOLORMAP:

		case IMTGA_RLECOLORMAP:

			colortable=1;

			break;

		case IMTGA_UNCOMPRGB:

		case IMTGA_UNCOMPBW:

		case IMTGA_RLERGB:

		case IMTGA_RLEBW:

			break;

		case IMTGA_HUFF:

		case IMTGA_HUFF4TYPE:

		default:	

			ImErrorFatal (ImQError(), -1, IMEFORMAT);

			/* break; (not reached) */

	}



	/* Make sure that there is a color map if the type requires one */

	if ((colortable) & (!header.tga_cmtype))

		 ImErrorFatal (ImQError(), -1, IMENOCLT);



	/* If there is a color table when one doesn't need to be there

		save it anyway	*/

	if (header.tga_cmtype) colortable=1;



	/* Allocate a VFB of the required size and type */

	switch (z)

	{

		case IMTGA_UNCOMPCOLORMAP:		/*color mapped image */

		case IMTGA_UNCOMPBW:		/*monochrome image */

		case IMTGA_RLECOLORMAP:		/*RLE color mapped image */

		case IMTGA_RLEBW:	/* compressed monochrome image*/

		  switch (q)

		  {

		    case 8:     /*Targa 16 */

		      if ( (vfb = ImVfbAlloc (w, h, IMVFBINDEX8) ) ==IMVFBNULL)

		      {

			ImErrorFatal (ImQError(), -1, IMEMALLOC);

		      }

		      break;

		    case 16:	/*Targa 24 (and hopefully 32*/

		      if ( (vfb = ImVfbAlloc (w, h, IMVFBINDEX16) ) ==IMVFBNULL)

		      {

			ImErrorFatal (ImQError(), -1, IMEMALLOC);

		      }

		      break;

		    default:    /* unsupported type 1 */

		      sprintf (message, "Weird number of bits per pixel stored'%i'",q);

		      ImErrorFatal (message, -1, IMESYNTAX);

		      /* break; (not reached) */

		  }

		  break;



		case IMTGA_UNCOMPRGB:		/*RGB image */

		case IMTGA_RLERGB:	/*RLE RGB image */

		  switch (q)

		  {

		    case 24:    /* No attribute byte */

		      if ( (vfb =ImVfbAlloc (w,h,IMVFBRGB) ) == IMVFBNULL)

		      {

			ImErrorFatal (ImQError(), -1, IMEMALLOC);

		      }

		      break;

		    case 16:	/* ARRRRRGG GGGBBBBB */

		    case 32:    /* Attribute byte as well */

		      if ((vfb=ImVfbAlloc (w,h,IMVFBRGB|IMVFBALPHA))==IMVFBNULL)

		      {

			ImErrorFatal (ImQError(), -1, IMEMALLOC);

		      }

		      break;

		    default:    /*kind've a weird amount of bits*/

		      sprintf (message, "Weird number of bits per pixel stored'%i'",q);

		      ImErrorFatal (message, -1, IMESYNTAX);

		      /* break; (not reached) */

		  }

		  break;



		default:

		      ImErrorFatal (ImQError(), -1, IMEFORMAT);

		      /* break; (not reached) */

	}	



	/* if needed allocate a name field and read into it */

	if (header.tga_numc_ident)

	{

	   ImMalloc (TgaName, char*, header.tga_numc_ident+1);

	   ImBinRead (ioType, fd, fp, TgaName, UCHAR,1, header.tga_numc_ident);



	   /* information to printout if verbose flag has been set */

           sprintf( message, "%s", TgaName);

           ImInfo( "Image Name", message );

	}



	   /* information to printout if verbose flag has been set */

           sprintf( message, "Least Significant Byte First");

           ImInfo( "Byte Order", message );

           sprintf( message, "%d x %d",w,h);

           ImInfo( "Resolution", message );



	   if ( (z==IMTGA_UNCOMPCOLORMAP) || (z==IMTGA_RLECOLORMAP) )

		   sprintf (message, "%d-bit Color Indexed", header.tga_spix);



	   if ( (z==IMTGA_UNCOMPRGB) || (z==IMTGA_RLERGB) )

		   sprintf (message, "%d-bit RGB", header.tga_spix);



	   if ( (z==IMTGA_UNCOMPBW) || (z==IMTGA_RLEBW) )

		   sprintf (message, "%d-bit Grayscale", header.tga_spix);

           ImInfo( "Type", message );



	/* if needed allocate a color lookup table and read into it*/

	if (colortable)

	{

	   n=header.tga_cmlen;

	   m=header.tga_cmsize;

	   if ((clt = ImCltAlloc (n)) == IMCLTNULL)

		ImErrorFatal (ImQError(), -1, IMEMALLOC);



	   /* information to printout if verbose flag has been set */

	   sprintf (message, "%d Entries",m);

           ImInfo( "Color Table", message );



	/* there appears to be no interleaving for targa color tables */

	/* there is however a variable number of bytes per entry */

	   switch (m)

	   {

		case 16:		/* ARRRRRGG GGGBBBBB */

		   ImMalloc ( buffer, unsigned char *, sizeof(unsigned char)*n*2);

		   pColor = ImCltQFirst (clt);



		   if (ImBinRead (ioType,fd,fp,buffer, UCHAR, 1, n*2)== -1)

		   {

			free ( (char *)buffer);

			ImReturnBinError();

		   }

		   n *=2;

		   for (i =0; i<n; i+=2)

		   {

		      imTga2ByteExpand_CLT (buffer[i],buffer[i+1],pColor);

		      ImCltSInc (clt, pColor);

		   }

		   free ( (char *)buffer);

		   break;



		case 24:		/* BBBBBBBB GGGGGGGG RRRRRRRR */

		   ImMalloc ( buffer, unsigned char *, sizeof(unsigned char)*n*3);

		   pColor = ImCltQFirst (clt);



		   if (ImBinRead (ioType,fd,fp,buffer, UCHAR, 1, n*3)== -1)

		   {

			free ( (char *)buffer);

			ImReturnBinError();

		   }

		   n *=3;

		   for (i =0; i<n; i+=3)

		   {

			ImCltSBlue (pColor, buffer[i]);

			ImCltSGreen(pColor, buffer[i+1]);

			ImCltSRed (pColor, buffer[i+2]);

			ImCltSInc (clt, pColor);

		   }

		   free ( (char *)buffer);

		   break;



		case 32:		/* BBBBBBBB GGGGGGGG RRRRRRRR AAAAAAAA*/

		   ImMalloc ( buffer, unsigned char *, sizeof(unsigned char)*n*4);

		   pColor = ImCltQFirst (clt);



		   if (ImBinRead (ioType,fd,fp,buffer, UCHAR, 1, n*4)== -1)

		   {

			free ( (char *)buffer);

			ImReturnBinError();

		   }



		   n *=4;

		   for (i =0; i<n; i+=4)

		   {

			ImCltSBlue (pColor, buffer[i]);

			ImCltSGreen(pColor, buffer[i+1]);

			ImCltSRed (pColor, buffer[i+2]);

		        /* we don't like any attribute bits mucking around our color tables */

			ImCltSInc (clt, pColor);

		   }

		   free ( (char *)buffer);

		   break;	



		default:		/* kooky entry type */

		   sprintf (message, "Weird number of bits per pixel in color table stored'%i'",q);

		   ImErrorFatal (message, -1, IMESYNTAX);

		   /* break; (not reached) */

	     }

	  } 





	   /* information to printout if verbose flag has been set */

	   if ((z>8) && (z<12))

	   	sprintf (message, "Run Length Encoded");

	   else

	   	sprintf (message, "none");

           ImInfo( "Compression Type", message );



	   if (((q==16) || (q==32) ) && ( (z==IMTGA_UNCOMPRGB) || (z==IMTGA_RLERGB)))

	   	sprintf (message, "%d-bit", q);

	   else

	   	sprintf (message, "none");

           ImInfo( "Alpha Channel", message );



	/* Now we read the image pixel data in from our data file */



	switch (z)

	{

	   case IMTGA_UNCOMPCOLORMAP:    /* CLT unencoded */

	   case IMTGA_UNCOMPBW:	  /* GreyScale     */

	        imReadTga_1and3 (ioType,fd,fp,vfb, q,h,w);

		break;

			

	   case IMTGA_UNCOMPRGB:    /* RGB  unencoded */

	        imReadTga_2 (ioType,fd,fp,vfb, q,h,w);

	        break;



	   case IMTGA_RLECOLORMAP:	  /* CLT RLE encoded */

	   case IMTGA_RLEBW:	  /* B&W RLE encoded */

		imReadTga_9and11 (ioType,fd,fp,vfb,q,h,w);

		break;



	   case IMTGA_RLERGB:	  /* RGB RLE encoded */

		imReadTga_10 (ioType,fd,fp,vfb,q,h,w);

		break;



	   default:

		ImErrorFatal (ImQError(), -1, IMEFORMAT);

		/* break; (not reached) */

		

	}	



	/* Attach (set) the vfb's color lookup table if one */

	if (colortable)

		ImVfbSClt(vfb, clt);



	if ( (!tga_orgbita) & (!tga_orgbitb))

	{

		/* If the screen origin is in the lower left hand corner flip */

	   	 sprintf (message, "Lower Left-Hand Corner");

		 ImVfbFlip (vfb, IMVFBYFLIP, vfb);

	}

	else if ( (tga_orgbita) & (!tga_orgbitb)) 

	{

		/* If the screen origin is in the lower right hand corner flip*/

	   	sprintf (message, "Lower Right-Hand Corner");

		ImVfbFlip (vfb, IMVFBXYFLIP, vfb);

	}

	else if ( (tga_orgbita) & (tga_orgbitb)) 

	{

		/* If the screen origin is in the top right hand corner flip */

	   	sprintf (message, "Upper Right-Hand Corner");

		ImVfbFlip (vfb, IMVFBXFLIP, vfb);

	}

	else

		sprintf (message, "Upper Left-Hand Corner");



        ImInfo( "Screen Origin", message );



	/* Attach name field to vfb is there is one */

	if (header.tga_numc_ident)

	{

		TagTableAppend (tagTable,

			TagEntryAlloc("image name",POINTER,&TgaName));

	}



	/* Attach vfb to tag table */

	TagTableAppend (tagTable, TagEntryAlloc ("image vfb", POINTER, &vfb));

	return(1);

}



/*

 *  FUNCTION

 *    imReadTga_1and3 - read either a type 1 or type 3 that has 1 or 2 bytes

 *					per pixel

 *

 *    type 1 - color lookup table that is not encoded.

 *    type 3 - greyscale image that is not encoded.

 */

static int	/* Returns 1 on successful read */

#ifdef __STDC__

imReadTga_1and3 (int ioType,int fd,FILE *fp,ImVfb *vfb, int q,int h,int w)

#else

imReadTga_1and3 (ioType,fd,fp,vfb, q,h,w)

	int   ioType;		/* I/O flags			*/

	int   fd;		/* Input file descriptor	*/

	FILE  *fp;		/* Input file pointer		*/

	ImVfb *vfb;		/* Read in Image		*/

	int   q;		/* How many bits per pixel	*/

	int   h;		/* Height of image		*/

	int   w;		/* width of image		*/

#endif

{

	sdsc_uint16	     *bufferl;		/* Run buffer (type long)	*/

	sdsc_uint16 *pBufferl;	/* bufferl pointer		*/

	ImVfbPtr pPixel;	/*VFB pixel pointer		*/

	int   x,y;		/* Counters			*/

	unsigned char *buffer;	/* Run buffer			*/

	unsigned char *pBuffer;	/* buffer pointer		*/

		/* interleaving best be noninterleaved as per specs*/

	

		if (q==8)   /* one byte per index */

		{

		   ImMalloc (buffer, unsigned char *, w * sizeof(unsigned char) );

		   pPixel = ImVfbQFirst (vfb);

		   for (y =0; y<h; y++)

		   {

		      if (ImBinRead (ioType,fd,fp,buffer, UCHAR, 1,w) == -1)

		      {

			free ( (char *)buffer);

			ImReturnBinError();

		      }

		      pBuffer = buffer;

		      for (x=0; x<w; x++)

		      {

		   	 ImVfbSIndex8(vfb, pPixel, *pBuffer++ );

			 ImVfbSInc(vfb, pPixel);

		      }

		   }

		   free ( (char *) buffer);

		}



		else	    /*two bytes per index */

		{

		   ImMalloc (bufferl, sdsc_uint16 *, w* sizeof(sdsc_uint16) );

		   pPixel = ImVfbQFirst (vfb);

		   for (y =0; y<h; y++)

		   {

		      if (ImBinRead (ioType,fd,fp,bufferl,UINT16,2,w) == -1)

		      {

			free ( (sdsc_uint16 *)bufferl);

			ImReturnBinError();

		      }

		      pBufferl = bufferl;

		      for (x=0; x<w; x++)

		      {

		   	 ImVfbSIndex16(vfb, pPixel, *pBufferl++ );

			 ImVfbSInc(vfb, pPixel);

		      }

		   }

		    free ( (sdsc_uint16 *) bufferl);

		} 	

	return(1);

}



/*

 *  FUNCTION

 *    imReadTga_2 - read a type 2 that has  2, 3 or 4 bytes per pixel

 *

 *    type 2 - unencoded RGB image.

 */

static int	/* Returns 1 on successful read */

#ifdef __STDC__

imReadTga_2 (int ioType,int fd,FILE *fp,ImVfb *vfb, int q,int h,int w)

#else

imReadTga_2 (ioType,fd,fp,vfb, q,h,w)

	int   ioType;		/* I/O flags			*/

	int   fd;		/* Input file descriptor	*/

	FILE  *fp;		/* Input file pointer		*/

	ImVfb *vfb;		/* Read in Image		*/

	int   q;		/* How many bits per pixel	*/

	int   h;		/* Height of image		*/

	int   w;		/* width of image		*/

#endif

{

	ImVfbPtr pPixel;	/*VFB pixel pointer	*/

	int   x,y;		/* Counters			*/

	unsigned char *buffer;		/* Run buffer		*/

	unsigned char *pBuffer;	/* buffer pointer	*/

		  switch (q)

		  {

		   case 16:     /*two bits per pixel*/

		   ImMalloc (buffer, unsigned char *, w * 2);

		   pPixel = ImVfbQFirst (vfb);

		   for (y =0; y<h; y++)

		   {

		      if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,2*w) == -1)

		      {

			free ( (char *)buffer);

			ImReturnBinError();

		      }

		      pBuffer = buffer;

		      for (x=0; x<w; x++)

		      {

			imTga2ByteExpand_PIX (*pBuffer,*(pBuffer+1),pPixel,vfb);

			pBuffer+=2;

			ImVfbSInc(vfb,pPixel);	

		      }

		    }

		   free ( (char *) buffer);

		   break;



		   case 24:	/* three bits per pixel */	

		   ImMalloc (buffer, unsigned char *, w * 3);

		   pPixel = ImVfbQFirst (vfb);

		   for (y =0; y<h; y++)

		   {

		      if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,3*w) == -1)

		      {

			free ( (char *)buffer);

			ImReturnBinError();

		      }

		      pBuffer = buffer;

		      for (x=0; x<w; x++)

		      {

		   	 ImVfbSBlue(vfb, pPixel,  *pBuffer++ );

		   	 ImVfbSGreen(vfb, pPixel,  *pBuffer++ );

		   	 ImVfbSRed(vfb, pPixel,  *pBuffer++ );

			 ImVfbSInc(vfb, pPixel);

		      }

		    }

		    free ( (char *) buffer);

		    break;



		    case 32:	/* four bits per pixel */	

		    ImMalloc (buffer, unsigned char *, w * 4);

		    pPixel = ImVfbQFirst (vfb);

		    for (y =0; y<h; y++)

		    {

		      if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,4*w) == -1)

		      {

			free ( (char *)buffer);

			ImReturnBinError();

		      }

		      pBuffer = buffer;

		      for (x=0; x<w; x++)

		      {

		   	 ImVfbSBlue(vfb, pPixel,  *pBuffer++ );

		   	 ImVfbSGreen(vfb, pPixel,  *pBuffer++ );

		   	 ImVfbSRed(vfb, pPixel,  *pBuffer++ );

			 ImVfbSAlpha(vfb, pPixel, *pBuffer++ );

			 ImVfbSInc(vfb, pPixel);

		       }

		    }

		    free ( (char *) buffer);

		    break;

                  }

	return(1);

}



/*

 *  FUNCTION

 *	imReadTga_9and11	-  read a Targa CLT or B&W file

 *

 *  DESCRIPTION

 *     Reads in a type 9 or type 11 image with either 8 or 16 bits per pixel.

 *

 *  type 9 - run-length-encoded color-lookup-table

 *  type 11 - run-length-encoded greyscale image

 */

static int	/* Returns 1 on successful read */

#ifdef __STDC__

imReadTga_9and11 (int ioType,int fd,FILE* fp,ImVfb *vfb, int q,int h,int w)

#else

imReadTga_9and11 (ioType,fd,fp,vfb, q,h,w)

	int   ioType;		/* I/O flags			*/

	int   fd;		/* Input file descriptor	*/

	FILE  *fp;		/* Input file pointer		*/

	ImVfb *vfb;		/* Read in Image		*/

	int   q;		/* How many bits per pixel	*/

	int   h;		/* Height of image		*/

	int   w;		/* width of image		*/

#endif

{

	ImVfbPtr pPixel;	/*VFB pixel pointer	*/

	int   x,y;		/* Counters			*/

	unsigned char *buffer;		/* Run buffer (type uchar)	*/

	sdsc_uint16 *bufferl;		/* Run buffer (type unsigned char)	*/

	unsigned char *pBuffer;	/* buffer pointer	*/

	sdsc_uint16 *pBufferl;	/* buffer pointer	*/

	int   total;		/* Counter for number of bytes encode */

	int   count;		/* Number of bytes to decode	*/

	unsigned char   rle;		/* whether something is rle or not	*/

	int   field;		/* red,green, or blue color	*/

	unsigned char   leadByte;	/* Read in the lead byte	*/



	total = h*w;	/* a running count of bytes encode */

	field = 0;		/* Start with the red pixel	*/

	pPixel = ImVfbQFirst(vfb);



	switch (q)

	{

	  case 8:		/* one byte per pixel	*/

	   ImMalloc (buffer, unsigned char *, 129);

	   while (total>0) {

		if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,1) == -1)

		{

		   free ( (char *)buffer);

		   ImReturnBinError();

		}



		pBuffer = buffer;



		   leadByte = (*pBuffer++);

		   rle = (leadByte&0x80);	/* rle = first bit   */

		   count = (leadByte&0x7F)+1;  /*count = last 7 bytes +1 */



		   if (rle)	/* run length encoded packet */

		   {

		        if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,1) == -1)

		        {

		           free ( (char *)buffer);

		           ImReturnBinError();

		        }

		        pBuffer = buffer;

			for (y=0; y<count; y++)

			{

			   ImVfbSIndex8(vfb,pPixel,*pBuffer);

			   ImVfbSInc(vfb,pPixel);

			}

		   }

		   

		   else		/* non rleencoded packet	*/

		   {

		        if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,count) == -1)

		        {

		           free ( (char *)buffer);

		           ImReturnBinError();

		        }

		        pBuffer = buffer;

			for (y=0; y<count; y++)

			{

			   ImVfbSIndex8(vfb,pPixel,*pBuffer++);

			   ImVfbSInc(vfb,pPixel);

			}

		   }

		

		   

	    	total-=count;

	    } 

	    free ( (char *)buffer);

	    break;	/*break q=8*/





	  case 16:		/* two bytes per pixel	*/

	   ImMalloc (bufferl, sdsc_uint16 *, 129* sizeof( sdsc_uint16));

	   while (total>0 ) {

		if (ImBinRead (ioType,fd,fp,bufferl,UINT16,2,1) == -1)

		{

		   free ( (sdsc_uint16 *)bufferl);

		   ImReturnBinError();

		}



		pBufferl = bufferl;



		   leadByte = (*pBufferl++);

		   rle = (leadByte&0x80);	/* rle = first bit   */

		   count = (leadByte&0x7F)+1;  /*count = last 7 bytes +1 */



		   if (rle)	/* run length encoded packet */

		   {

		        if (ImBinRead (ioType,fd,fp,bufferl,UINT16,2,1) == -1)

		        {

		           free ( (sdsc_uint16 *)bufferl);

		           ImReturnBinError();

		        }

		        pBufferl = bufferl;

			for (y=0; y<count; y++)

			{

			   ImVfbSIndex16(vfb,pPixel,*pBufferl);

			   ImVfbSInc (vfb, pPixel);

			}

		   }

		   

		   else		/* non rleencoded packet	*/

		   {

		        if (ImBinRead (ioType,fd,fp,bufferl,UINT16,2,count) == -1)

		        {

		           free ( (sdsc_uint16 *)bufferl);

		           ImReturnBinError();

		        }

		        pBufferl = bufferl;

			for (y=0; y<count; y++)

			{

			   ImVfbSIndex16(vfb,pPixel,*pBufferl++);

			   ImVfbSInc (vfb, pPixel);

			}

		   }

		

		   

	    	total-=count;

	    } 

            free ( (sdsc_uint16 *)bufferl);

	    break;	/*break q=16*/

	}

	return(1);

}















/*

 *  FUNCTION

 *	imReadTga_10	-  write a Targa RGB file

 *

 *  DESCRIPTION

 *      type 10 - RGB RLE encoded image with 2, 3 or 4 bytes per pixel.

 */

static int	/* Returns 1 on successful read */

#ifdef __STDC__

imReadTga_10 (int ioType,int fd,FILE* fp,ImVfb *vfb, int q,int h,int w)

#else

imReadTga_10 (ioType,fd,fp,vfb, q,h,w)

	int   ioType;		/* I/O flags			*/

	int   fd;		/* Input file descriptor	*/

	FILE  *fp;		/* Input file pointer		*/

	ImVfb *vfb;		/* Read in Image		*/

	int   q;		/* How many bits per pixel	*/

	int   h;		/* Height of image		*/

	int   w;		/* width of image		*/

#endif

{

	ImVfbPtr pPixel;	/*VFB pixel pointer	*/

	int   x,y;		/* Counters			*/

	unsigned char *buffer;		/* Run buffer (type uchar)	*/

	unsigned char *pBuffer;	/* buffer pointer	*/

	int   total;		/* Counter for number of bytes encode */

	int   count;		/* Number of bytes to decode	*/

	unsigned char   rle;		/* whether something is rle or not	*/

	int   field;		/* red,green, or blue color	*/

	unsigned char   leadByte;	/* Read in the lead byte	*/

	int	expatr;			/* The attribute bit		*/

	int     expred;			/* The five red bits		*/

	int     expgreen;		/* The five green bits		*/

	int     expblue;		/* The five blue bits		*/ 

	int	exptemp;



	BinByteOrder (BINLBF);

	field =0;		/* Start with the red pixel	*/

	pPixel = ImVfbQFirst(vfb);



	switch (q)

	{

	  case 16:		/*three bits per pixel	*/

	ImMalloc (buffer, unsigned char *, 260);

	   total=h*w;		/* a running count of bytes decode */

	   while (total>0)  {

		if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,1) == -1)

		{

		   free ( (char *)buffer);

		   ImReturnBinError();

		}



		pBuffer = buffer;



		   leadByte = (*pBuffer++);

		   rle = (leadByte&0x80);	/* rle = first bit   */

		   count = (leadByte&0x7F)+1;  /*count = last 7 bytes +1 */



		   if (rle)	/* run length encoded packet */

		   {

		        if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,2) == -1)

		        {

		           free ( (char *)buffer);

		           ImReturnBinError();

		        }

		        pBuffer = buffer;

			for (y=0; y<count; y++)

			{

			   imTga2ByteExpand_PIX(*pBuffer,*(pBuffer+1),pPixel,vfb);

			   ImVfbSInc(vfb,pPixel);

			}

		   }

		   

		   else		/* non rleencoded packet	*/

		   {

		        if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,count*2) == -1)

		        {

		           free ( (char *)buffer);

		           ImReturnBinError();

		        }

		        pBuffer = buffer;

			for (y=0; y<count; y++)

			{

			   imTga2ByteExpand_PIX(*pBuffer,*(pBuffer+1),pPixel,vfb);

			   pBuffer+=2;

			   ImVfbSInc(vfb,pPixel);

			}

		   }

		

		   

	    	total-=count;

	    } 

	    free ( (char *)buffer);

	    break;		/* end case q=24	*/



	  case 24:		/*three bits per pixel	*/

	   ImMalloc (buffer, unsigned char *, 390);

	   total=h*w*1;		/* a running count of bytes decode */

	   while (total >0 ) {

		if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,1) == -1)

		{

		   free ( (char *)buffer);

		   ImReturnBinError();

		}



		pBuffer = buffer;



		   leadByte = (*pBuffer++);

		   rle = (leadByte&0x80);	/* rle = first bit   */

		   count = (leadByte&0x7F)+1;  /*count = last 7 bytes +1 */



		   if (rle)	/* run length encoded packet */

		   {

		        if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,3) == -1)

		        {

		           free ( (char *)buffer);

		           ImReturnBinError();

		        }

		        pBuffer = buffer;

			for (y=0; y<count; y++)

			{

		 	   ImVfbSBlue (vfb,pPixel,*(pBuffer));

			   ImVfbSGreen(vfb,pPixel,*(pBuffer+1));

			   ImVfbSRed  (vfb,pPixel,*(pBuffer+2));

			   ImVfbSInc  (vfb,pPixel);

			}

		   }

		   

		   else		/* non rleencoded packet	*/

		   {

		        if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,count*3) == -1)

		        {

		           free ( (char *)buffer);

		           ImReturnBinError();

		        }

		        pBuffer = buffer;

			for (y=0; y<count; y++)

			{

		 	   ImVfbSBlue (vfb,pPixel,*pBuffer++);

			   ImVfbSGreen(vfb,pPixel,*pBuffer++);

			   ImVfbSRed (vfb,pPixel,*pBuffer++);

			   ImVfbSInc (vfb,pPixel);

			}

		   }

		

		   

	    	total-=count;

	    } 



	    free ( (char *)buffer);

	    break;		/* end case q=24	*/



	  case 32:		/* four bits per pixel	*/

	   ImMalloc (buffer, unsigned char *, 520);

	   total=h*w*1;		/* a running count of bytes decode */

	   while (total > 0)  {

		if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,1) == -1)

		{

		   free ( (char *)buffer);

		   ImReturnBinError();

		}



		pBuffer = buffer;



		   leadByte = (*pBuffer++);

		   rle = (leadByte&0x80);	/* rle = first bit   */

		   count = (leadByte&0x7F)+1;  /*count = last 7 bytes +1 */



		   if (rle)	/* run length encoded packet */

		   {

		        if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,4) == -1)

		        {

		           free ( (char *)buffer);

		           ImReturnBinError();

		        }

		        pBuffer = buffer;

			for (y=0; y<count; y++)

			{

		 	   ImVfbSBlue (vfb,pPixel,*(pBuffer));

			   ImVfbSGreen(vfb,pPixel,*(pBuffer+1));

			   ImVfbSRed  (vfb,pPixel,*(pBuffer+2));

			   ImVfbSAlpha(vfb,pPixel,*(pBuffer+3));

			   ImVfbSInc (vfb,pPixel);

			}

		   }

		   

		   else		/* non rleencoded packet	*/

		   {

		        if (ImBinRead (ioType,fd,fp,buffer,UCHAR,1,count*4) == -1)

		        {

		           free ( (char *)buffer);

		           ImReturnBinError();

		        }

		        pBuffer = buffer;

			for (y=0; y<count; y++)

			{

		 	   ImVfbSBlue (vfb,pPixel,*pBuffer++);

			   ImVfbSGreen(vfb,pPixel,*pBuffer++);

			   ImVfbSRed  (vfb,pPixel,*pBuffer++);

		 	   ImVfbSAlpha(vfb,pPixel,*pBuffer++);

			   ImVfbSInc  (vfb,pPixel);

			}

		   }

		

		   

	    	total-=count;

	    } 

	    free ( (char *)buffer);

	    break;		/* end case q=32	*/

	}   /* end switch q */

	return(1);

}











/* 

 * MACROS

 *   imTgaDumpVarRun

 *

 *   imTgaDumpConstantRun1

 *   imTgaDumpConstantRun3

 *   imTgaDumpConstantRun4

 *

 * DESCRIPTION

 *   These are used for RLE encoding.

 *

 *   imTgaDumpVarRun will write out a variable run.  That is,

 *   a stream of pixel values with no two consecutive values

 *   that are the same.  (i.e. A B C D E)

 *

 *   imTgaDumpConstantRun3 will write out a stream of 

 *   pixels with the same value.  (i.e. A A A A A)

 *

 *   The integers at the end of these functions' names indicate

 *   the number of bytes per pixel.

 */



static unsigned char imTgaRLEBuf[10];    /* Buffer for these macros */



#define imTgaDumpVarRun(bytesPerPixel, buffer, len)	\
	imTgaRLEBuf[0] = (unsigned char) len;    	\
	imTgaRLEBuf[0] &= 0x7f;                         \
	if ( ImBinWrite( ioType, fd, fp, imTgaRLEBuf, UCHAR, 1, 1)==-1) \
	{                                               \
		ImReturnBinError( );                    \
	}						\
	if ( ImBinWrite( ioType, fd, fp, buffer, UCHAR, 1, bytesPerPixel * (len+1))==-1) \
	{                                               \
		ImReturnBinError( );                    \
	}

   

#define imTgaDumpConstantRun1(index,len)		\
	imTgaRLEBuf[0] = (unsigned char) len;		\
	imTgaRLEBuf[0] |= 0x80;				\
	imTgaRLEBuf[1] = index;                         \
	if ( ImBinWrite( ioType, fd, fp, imTgaRLEBuf, UCHAR, 1, 2)==-1) \
	{                                                           \
		ImReturnBinError( );                                \
	}



#define imTgaDumpConstantRun2(index16,len)		\
	imTgaRLEBuf[0] = (unsigned char) len;		\
	imTgaRLEBuf[0] |= 0x80;				\
	imTgaRLEBuf[1] = (index16 & 0xff00) >> 8;       \
	imTgaRLEBuf[2] = (index16 & 0x00ff);            \
	if ( ImBinWrite( ioType, fd, fp, imTgaRLEBuf, UCHAR, 1, 3)==-1) \
	{                                                           \
		ImReturnBinError( );                                \
	}



#define imTgaDumpConstantRun3(b,g,r,len)		\
	imTgaRLEBuf[0] = (unsigned char) len;		\
	imTgaRLEBuf[0] |= 0x80;				\
	imTgaRLEBuf[1] = b;                             \
	imTgaRLEBuf[2] = g;                             \
	imTgaRLEBuf[3] = r;                             \
	if ( ImBinWrite( ioType, fd, fp, imTgaRLEBuf, UCHAR, 1, 4)==-1) \
	{                                                           \
		ImReturnBinError( );                                \
	}


#define imTgaDumpConstantRun4(b,g,r,a,len)		\
	imTgaRLEBuf[0] = (unsigned char) len;		\
	imTgaRLEBuf[0] |= 0x80;				\
	imTgaRLEBuf[1] = b;                             \
	imTgaRLEBuf[2] = g;                             \
	imTgaRLEBuf[3] = r;                             \
	imTgaRLEBuf[4] = a;                             \
	if ( ImBinWrite( ioType, fd, fp, imTgaRLEBuf, UCHAR, 1, 5)==-1) \
	{                                                           \
		ImReturnBinError( );                                \
	}


/*

 *  FUNCTION

 *	imTgaWriteRGB24	-  write a Targa RGB file

 *

 *  DESCRIPTION

 *	That VFB is queried, and the Targa file header written out.

 *	The VFB data is then copied to the file.

 */

static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWriteRGB24( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imTgaWriteRGB24( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;	/* Pixel pointer		*/

	imTgaHeaderInfo  header;	/* Tga file header		*/

	unsigned char      *buffer;		/* line buffer			*/

	unsigned char      *pBuffer;		/* current location in  buffer  */

	int          n,x,y,i,j;		/* Counter			*/

	TagEntry    *tagEntry;		/* Tmp table entry		*/

	char	    *tmpString;		/* Tmp string holder		*/

	int         stype;              /* Storage type                 */

        int         spix;               /* Number of bits per image pixel */

        int         cmsize;             /* Number of bits per color map pixel */

	

	/* Set up and write out the header */

	BinByteOrder (BINLBF);



	TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);



	stype =IMTGA_UNCOMPRGB;

	spix =24;

	cmsize =24;

	/* write the header out */

	imTgaWriteHeader ( ioType, fd, fp,flagsTable, tagTable,stype,spix,cmsize);

	x = ImVfbQWidth(vfb);

	y = ImVfbQHeight(vfb);



	/* allocate buffer space  for (width*length*3) scan lines */

	ImMalloc (buffer, unsigned char *,3*x);

	pptr = ImVfbQFirst (vfb);



	for (j=0; j<y; j++)

	{



		pBuffer = buffer;

		for (i=0; i<x; i++)

		{

			*pBuffer++ = ImVfbQBlue (vfb, pptr);

			*pBuffer++ = ImVfbQGreen (vfb, pptr);

			*pBuffer++ = ImVfbQRed (vfb, pptr);

			ImVfbSInc (vfb, pptr);

		}





		if (ImBinWrite (ioType, fd, fp, buffer, UCHAR,1,pBuffer - buffer) ==-1)

		{

			free ( (char *)buffer);

			ImReturnBinError( );

		}



	}



	free ( (char *)buffer);

	return (1);

}



/*

 *  FUNCTION

 *	imTgaWriteRGBRLE24	-  write a Targa RGB file that is RLE

 *

 *  DESCRIPTION

 *	That VFB is queried, and the Targa file header written out.

 *	The VFB data is then copied to the file.

 */

static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWriteRGBRLE24( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imTgaWriteRGBRLE24( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     vfbptr;		/* Pixel pointer		*/

	imTgaHeaderInfo  header;	/* Tga file header		*/

	TagEntry    *tagEntry;		/* Tmp table entry		*/

	int         stype;              /* Storage type                 */

        int         spix;               /* Number of bits per image pixel */

        int         cmsize;             /* Number of bits per color map pixel */

	unsigned char varRun[128*3];    /* Holds a variable run          */

 	int varRunLength;               /* Length of this variable run.  */

	int constantRunLength;          /* Length of this constant run.  */

	ImVfbPtr lastPixel, thisPixel;  /* point to pixels.              */

	int i;                          /* loop index                    */



	/*

	 * In this function, a variable run refers to a series of different

	 * pixel values.  The 'length' of such a run is one less than the

	 * number of pixels.  For instance, if our pixel values were

	 *  "A B C D" then this would be a "variable run of length 3".

	 *

	 * Similarly, "A A A" would be a "constant run of length 3".

	 */



	BinByteOrder (BINLBF);

	TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);



	/* Set up and write out the header */

	stype  = IMTGA_RLERGB;

	spix   = 24;

        cmsize = 24;

        imTgaWriteHeader ( ioType, fd, fp,flagsTable, tagTable,stype,

				spix,cmsize);



	/* Initialize stuff. */

	varRunLength      = 0;

	constantRunLength = 0;

	thisPixel = ImVfbQFirst( vfb );

	lastPixel = thisPixel;



	for (i = 1;i < ImVfbQWidth(vfb) * ImVfbQHeight( vfb ) ; i++)

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

				imTgaDumpVarRun(3,varRun, varRunLength - 1); 

				varRunLength = 0;

			}

			constantRunLength++;

			if (constantRunLength==128)

			{   /* Dump a constant run. */

				imTgaDumpConstantRun3(ImVfbQBlue(vfb,lastPixel), 

					ImVfbQGreen(vfb, lastPixel),

					ImVfbQRed(vfb, lastPixel),

					constantRunLength - 1);

				constantRunLength = 0;

			}

		 }

		 else  /* This is different from the last character. */

		       /* Do the exact opposite of the if-branch.    */

		 {

			varRun[varRunLength*3]   = ImVfbQBlue(vfb, lastPixel);

			varRun[varRunLength*3+1] = ImVfbQGreen(vfb, lastPixel);

			varRun[varRunLength*3+2] = ImVfbQRed(vfb, lastPixel);



			if (constantRunLength > 0)

			{

				imTgaDumpConstantRun3(ImVfbQBlue(vfb, lastPixel),

					ImVfbQGreen(vfb, lastPixel),

					ImVfbQRed(vfb, lastPixel),

					constantRunLength);

				constantRunLength = 0;

			}

			else

			{   /* Only increment if this is not the end of a constant run. */

				varRunLength++;

			}

			if (varRunLength==128)

			{   /* Dump variable run. */

				imTgaDumpVarRun(3,varRun, varRunLength - 1); 

				varRunLength = 0;

			}

		 }

		 lastPixel = thisPixel;

	}



	/* Dump whatever is left. */

	if (constantRunLength==0 && varRunLength==0)

	{  /* single pixel is left. */

		imTgaDumpConstantRun3(ImVfbQBlue(vfb, thisPixel),

			ImVfbQGreen(vfb, thisPixel),

			ImVfbQRed(vfb, thisPixel),

			0 );

	}

	if (constantRunLength > 0)

	{

		imTgaDumpConstantRun3(ImVfbQBlue(vfb, thisPixel),

			ImVfbQGreen(vfb, thisPixel),

			ImVfbQRed(vfb, thisPixel),

			constantRunLength );

	}

	if (varRunLength > 0)

	{

		varRun[varRunLength*3]   = ImVfbQBlue (vfb, lastPixel);

		varRun[varRunLength*3+1] = ImVfbQGreen(vfb, lastPixel);

		varRun[varRunLength*3+2] = ImVfbQRed  (vfb, lastPixel);

		imTgaDumpVarRun(3,varRun, varRunLength );

	}



	return (1);

}



/*

 *  FUNCTION

 *	imTgaWriteRGB32	-  write a Targa RGB file that has an alpha channel

 *

 *  DESCRIPTION

 *	That VFB is queried, and the Targa file header written out.

 *	The VFB data is then copied to the file.

 */

static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWriteRGB32( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imTgaWriteRGB32( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;	/* Pixel pointer		*/

	imTgaHeaderInfo  header;	/* Tga file header		*/

	unsigned char      *buffer;		/* line buffer			*/

	unsigned char      *pBuffer;		/* current location in  buffer  */

	int          n,x,y,i,j;		/* Counter			*/

	TagEntry    *tagEntry;		/* Tmp table entry		*/

	char	    *tmpString;		/* Tmp string holder		*/

	int	    stype;		/* Storage type			*/

	int	    spix;		/* Number of bits per image pixel */

	int	    cmsize;		/* Number of bits per color map pixel */

	



	/* Set up and write out the header */

	BinByteOrder (BINLBF);



	TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);



	stype = IMTGA_UNCOMPRGB;

	spix =32;

	cmsize =24;

/* write the header out */

	imTgaWriteHeader ( ioType, fd, fp,flagsTable, tagTable,stype,spix,cmsize);



	x = ImVfbQWidth(vfb);

	y = ImVfbQHeight(vfb);



	/* allocate buffer space  for (width*length83) scan lines */

	ImMalloc (buffer, unsigned char *,4*x);

	pptr = ImVfbQFirst (vfb);



	for (j=0; j<y; j++)

	{



		pBuffer = buffer;

		for (i=0; i<x; i++)

		{

			*pBuffer++ = ImVfbQBlue (vfb, pptr);

			*pBuffer++ = ImVfbQGreen (vfb, pptr);

			*pBuffer++ = ImVfbQRed (vfb, pptr);

			*pBuffer++ = ImVfbQAlpha (vfb, pptr);

			ImVfbSInc (vfb, pptr);

		}



		if (ImBinWrite (ioType, fd, fp, buffer, UCHAR,1,pBuffer - buffer) ==-1)

		{

			free ( (char *)buffer);

			ImReturnBinError( );

		}



	}



	free ( (char *)buffer);

	return (1);

}



/*

 *  FUNCTION

 *	imTgaWriteRGBRLE32	-  write a Targa RGBA file that is RLE

 *

 *  DESCRIPTION

 *	That VFB is queried, and the Targa file header written out.

 *	The VFB data is then copied to the file.

 */

static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWriteRGBRLE32( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imTgaWriteRGBRLE32( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;	/* Pixel pointer		*/

	imTgaHeaderInfo  header;	/* Tga file header		*/

	TagEntry    *tagEntry;		/* Tmp table entry	*/		

	int         stype;              /* Stoage type                 */

        int         spix;               /* Numer of bits per image pixel */

        int         cmsize;             /* Numberof bits per color map pixel*/

	unsigned char varRun[128*4];    /* Holds a variable run          */

 	int varRunLength;               /* Length of this variable run.  */

	int constantRunLength;          /* Length of this constant run.  */

	ImVfbPtr lastPixel, thisPixel;  /* point to pixels.              */

	int i;                          /* loop index                    */



	/* Set up and write out the header */

	BinByteOrder (BINLBF);



	TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);





	stype  = IMTGA_RLERGB;

	spix   = 32;

        cmsize = 24;



	/* Write out the header. */

        imTgaWriteHeader ( ioType, fd, fp,flagsTable, tagTable,stype,

			spix,cmsize);



	/*

	 * In this function, a variable run refers to a series of different

	 * pixel values.  The 'length' of such a run is one less than the

	 * number of pixels.  For instance, if our pixel values were

	 *  "A B C D" then this would be a "variable run of length 3".

	 *

	 * Similarly, "A A A" would be a "constant run of length 3".

	 */



	/* Initialize stuff. */

	varRunLength      = 0;

	constantRunLength = 0;

	thisPixel = ImVfbQFirst( vfb );

	lastPixel = thisPixel;



	for (i = 1;i < ImVfbQWidth(vfb) * ImVfbQHeight( vfb ) ; i++)

	{

		ImVfbSInc(vfb,thisPixel);

		

		/* 

		 * If this pixel is the same as the last one then:

		 *   if varRunLength > 0, dump varRun.

		 *   otherwise, add this pixel to constantRun.

		 */

		 if (ImVfbSameRGBA(vfb,lastPixel,thisPixel))  

		 {

			if (varRunLength > 0)

			{

				imTgaDumpVarRun(4,varRun, varRunLength - 1); 

				varRunLength = 0;

			}

			constantRunLength++;

			if (constantRunLength==128)

			{   /* Dump a constant run. */

				imTgaDumpConstantRun4(

					ImVfbQBlue(vfb,lastPixel), 

					ImVfbQGreen(vfb, lastPixel),

					ImVfbQRed(vfb, lastPixel),

					ImVfbQAlpha(vfb,lastPixel),

					constantRunLength - 1);

				constantRunLength = 0;

			}

		 }

		 else  /* This is different from the last character. */

		       /* Do the exact opposite of the if-branch.    */

		 {

			varRun[varRunLength*4] = ImVfbQBlue(vfb, lastPixel);

			varRun[varRunLength*4+1] = ImVfbQGreen(vfb, lastPixel);

			varRun[varRunLength*4+2] = ImVfbQRed(vfb, lastPixel);

			varRun[varRunLength*4+3] = ImVfbQAlpha(vfb, lastPixel);



			if (constantRunLength > 0)

			{

				imTgaDumpConstantRun4(

					ImVfbQBlue(vfb, lastPixel),

					ImVfbQGreen(vfb, lastPixel),

					ImVfbQRed(vfb, lastPixel),

					ImVfbQAlpha(vfb, lastPixel),

					constantRunLength);

				constantRunLength = 0;

			}

			else

			{   /* Only increment if this is not the end of a constant run. */

				varRunLength++;

			}

			if (varRunLength==128)

			{   /* Dump variable run. */

				imTgaDumpVarRun(4,varRun, varRunLength - 1); 

				varRunLength = 0;

			}

		 }

		 lastPixel = thisPixel;

	}



	/* Dump whatever is left. */

	if (constantRunLength==0 && varRunLength==0)

	{  /* single pixel is left. */

		imTgaDumpConstantRun4(

			ImVfbQBlue(vfb, thisPixel),

			ImVfbQGreen(vfb, thisPixel),

			ImVfbQRed(vfb, thisPixel),

			ImVfbQAlpha(vfb,thisPixel),

			0 );

	}

	if (constantRunLength > 0)

	{

		imTgaDumpConstantRun4(

			ImVfbQBlue(vfb, thisPixel),

			ImVfbQGreen(vfb, thisPixel),

			ImVfbQRed(vfb, thisPixel),

			ImVfbQAlpha(vfb,thisPixel),

			constantRunLength );

	}

	if (varRunLength > 0)

	{

		varRun[varRunLength*4] = ImVfbQBlue (vfb, lastPixel);

		varRun[varRunLength*4+1] = ImVfbQGreen(vfb, lastPixel);

		varRun[varRunLength*4+2] = ImVfbQRed  (vfb, lastPixel);

		varRun[varRunLength*4+3] = ImVfbQAlpha (vfb, lastPixel);

		imTgaDumpVarRun(4,varRun, varRunLength );

	}



	return (1);

}







/*

 *  FUNCTION

 *	imTgaWriteCLT8 -  write a Targa CLT file that has 8 bits pre channel

 *

 *  DESCRIPTION

 *	That VFB is queried, and the Targa file header written out.

 *	The VFB data is then copied to the file.

 */

static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWriteCLT8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imTgaWriteCLT8( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	TagEntry    *tagEntry;		/* Tmp table entry		*/

	int         stype;              /* Storage type                 */

        int         spix;               /* Number of bits per image pixel */

        int         cmsize;             /* Number of bits per color map pixel */

	/* Set up and write out the header */

	stype  = IMTGA_UNCOMPCOLORMAP;

	spix   = 8;

	cmsize = 24;

	/*write header out */

	imTgaWriteHeader (ioType, fd, fp, flagsTable, tagTable,stype,spix,cmsize);



	imTgaWrite8image (ioType, fd, fp, flagsTable, tagTable);

	return(1);

}





/*

 *  FUNCTION

 *	imTgaWrite8image -  write a Targa file that has 8 bits pre channel

 *

 *  DESCRIPTION

 *	That VFB is queried, The VFB data is then copied to the file.

 */

static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWrite8image( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imTgaWrite8image( ioType, fd, fp, flagsTable, tagTable )

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;	/* Pixel pointer		*/

	imTgaHeaderInfo  header;	/* Tga file header		*/

	unsigned char      *buffer;		/* line buffer			*/

	unsigned char      *pBuffer;		/* current location in  buffer  */

	int          x,y,i,j;		/* Counter			*/

	TagEntry    *tagEntry;		/* Tmp table entry		*/



	BinByteOrder (BINLBF);



	TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);

	x =ImVfbQWidth(vfb);

	y = ImVfbQHeight(vfb);



	/* allocate buffer space  for (width*length83) scan lines */

	ImMalloc (buffer, unsigned char *, 1*x);

	pptr = ImVfbQFirst (vfb);



	for (j=0; j<y; j++)

	{



		pBuffer = buffer;

		for (i=0; i<x; i++)

		{

			*pBuffer++ = ImVfbQIndex8 (vfb, pptr);

			ImVfbSInc (vfb, pptr);

		}



		if (ImBinWrite (ioType, fd, fp, buffer, UCHAR,1,pBuffer - buffer) ==-1)

		{

			free ( (char *)buffer);

			ImReturnBinError( );

		}



	}



	free ( (char *)buffer);

	return (1);

}



/*

 *  FUNCTION

 *	imTgaWriteCLTRLE8 -  write a Targa CLT file that has 8 bits pre channel

 *					and is RLE.

 *

 *  DESCRIPTION

 *	That VFB is queried, and the Targa file header written out.

 *	The VFB data is then copied to the file.

 */

static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWriteCLTRLE8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imTgaWriteCLTRLE8( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	int         stype;              /* Storage type                 */

        int         spix;               /* Number of bits per image pixel */

        int         cmsize;             /* Number of bits per color map pixel */



	stype  = IMTGA_RLECOLORMAP;

	spix   = 8;

        cmsize = 24;

	BinByteOrder (BINLBF);



	/* write the header out */

        imTgaWriteHeader ( ioType, fd, fp,flagsTable, tagTable,stype,spix,cmsize);



        imTgaWriteRLE8 ( ioType, fd, fp,flagsTable, tagTable);



	return(1);

}



/*

 *  FUNCTION

 *	imTgaWriteRLE8 -  write a Targa CLT file that has 8 bits per channel

 *					and is RLE.

 *

 *  DESCRIPTION

 *	That VFB is queried, The VFB data is then copied to the file.

 */

static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWriteRLE8(  int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imTgaWriteRLE8(  ioType, fd, fp, flagsTable, tagTable )

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     vfbPtr;		/* Pixel pointer		*/

	imTgaHeaderInfo  header;	/* Tga file header		*/

	TagEntry    *tagEntry;		/* Tmp table entry		*/

	int         stype;              /* Storage type                 */

        int         spix;               /* Number of bits per image pixel */

        int         cmsize;             /* Number of bits per color map pixel */

	unsigned char varRun[128*1];    /* Holds a variable run          */

 	int varRunLength;               /* Length of this variable run.  */

	int constantRunLength;          /* Length of this constant run.  */

	ImVfbPtr lastPixel, thisPixel;  /* point to pixels.              */

	int i;                          /* loop index                    */



	/*

	 * In this function, a variable run refers to a series of different

	 * pixel values.  The 'length' of such a run is one less than the

	 * number of pixels.  For instance, if our pixel values were

	 *  "A B C D" then this would be a "variable run of length 3".

	 *

	 * Similarly, "A A A" would be a "constant run of length 3".

	 */



	TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);



	/* Initialize stuff. */

	varRunLength      = 0;

	constantRunLength = 0;

	thisPixel = ImVfbQFirst( vfb );

	lastPixel = thisPixel;



	for (i = 1;i < ImVfbQWidth(vfb) * ImVfbQHeight( vfb ) ; i++)

	{

		ImVfbSInc(vfb,thisPixel);

		

		/* 

		 * If this pixel is the same as the last one then:

		 *   if varRunLength > 0, dump varRun.

		 *   otherwise, add this pixel to constantRun.

		 */

		 if (ImVfbSameIndex8(vfb,lastPixel,thisPixel))  

		 {

			if (varRunLength > 0)

			{

				imTgaDumpVarRun(1,varRun, varRunLength - 1); 

				varRunLength = 0;

			}

			constantRunLength++;

			if (constantRunLength==128)

			{   /* Dump a constant run. */

				imTgaDumpConstantRun1(ImVfbQIndex(vfb,lastPixel), 

					constantRunLength - 1);

				constantRunLength = 0;

			}

		 }

		 else  /* This is different from the last character. */

		       /* Do the exact opposite of the if-branch.    */

		 {

			varRun[varRunLength] = ImVfbQIndex(vfb, lastPixel);



			if (constantRunLength > 0)

			{

				imTgaDumpConstantRun1(ImVfbQIndex(vfb, lastPixel),

					constantRunLength);

				constantRunLength = 0;

			}

			else

			{   /* Only increment if this is not the end of a constant run. */

				varRunLength++;

			}

			if (varRunLength==128)

			{   /* Dump variable run. */

				imTgaDumpVarRun(1,varRun, varRunLength - 1); 

				varRunLength = 0;

			}

		 }

		 lastPixel = thisPixel;

	}



	/* Dump whatever is left. */

	if (constantRunLength==0 && varRunLength==0)

	{  /* single pixel is left. */

		imTgaDumpConstantRun1(ImVfbQIndex(vfb, thisPixel), 0 );

	}

	if (constantRunLength > 0)

	{

		imTgaDumpConstantRun1(ImVfbQIndex(vfb, thisPixel),

			constantRunLength );

	}

	if (varRunLength > 0)

	{

		varRun[varRunLength] = ImVfbQIndex (vfb, lastPixel);

		imTgaDumpVarRun(1,varRun, varRunLength );

	}

	return (1);

}



/*

 *  FUNCTION

 *	imTgaWriteCLT16 -  write a Targa CLT file that has 16 bits pre channel

 *

 *  DESCRIPTION

 *	That VFB is queried, and the Targa file header written out.

 *	The VFB data is then copied to the file.

 */

static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWriteCLT16( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imTgaWriteCLT16( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	int         stype;              /* Storage type                 */

        int         spix;               /* Number of bits per image pixel */

        int         cmsize;             /* Number of bits per color map pixel */

	

	/* Set up and write out the header */

	stype  = IMTGA_UNCOMPCOLORMAP;

	spix   = 16;

	cmsize = 24;



	/* Write header out */



	imTgaWriteHeader (ioType, fd, fp, flagsTable, tagTable,stype,spix,cmsize);



	imTgaWrite16image (ioType, fd, fp, flagsTable, tagTable);

	return(1);

}







/*

 *  FUNCTION

 *	imTgaWrite16image -  write a Targa CLT file that has 16 bits per channel

 *

 *  DESCRIPTION

 *	That VFB is queried, The VFB data is then copied to the file.

 */

static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWrite16image( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imTgaWrite16image( ioType, fd, fp, flagsTable, tagTable )

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	imTgaHeaderInfo  header;	/* Tga file header		*/

	unsigned char  *bufferl;	/* line buffer			*/

	unsigned char  *pBufferl;	/* current location in  buffer  */

	int          x,y,i,j;		/* Counter			*/

	TagEntry    *tagEntry;		/* Tmp table entry		*/





	BinByteOrder (BINLBF);



	TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);



	x = ImVfbQWidth(vfb);

	y = ImVfbQHeight(vfb);



	/* allocate buffer space  for (width*length) scan lines */



	ImMalloc (bufferl, unsigned char *, 2 * x * sizeof(unsigned char));

	pptr = ImVfbQFirst (vfb);



	for (j=0; j<y; j++)

	{



		pBufferl = bufferl;

		for (i=0; i<x; i++)

		{

			*pBufferl++ = (ImVfbQIndex16 (vfb, pptr) & 0xff) ;

			*pBufferl++ = (ImVfbQIndex16 (vfb, pptr) & 0xff00) >> 8;

			ImVfbSInc (vfb, pptr);

		}



		if (ImBinWrite (ioType, fd, fp, bufferl, UCHAR, 1, x*2 ) ==-1)

		{

			free ( bufferl);

			ImReturnBinError( );

		}



	}



	free (bufferl);

	return (1);



}



/*

 *  FUNCTION

 *	imTgaWriteCLTRLE16 -  write a Targa CLT file that has 16 bits per channel

 *					and is RLE.

 *

 *  DESCRIPTION

 *	That VFB is queried, and the Targa file header written out.

 *	The VFB data is then copied to the file.

 */

static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWriteCLTRLE16( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imTgaWriteCLTRLE16( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	int         stype;              /* Storage type                 */

        int         spix;               /* Number of bits per image pixel */

        int         cmsize;             /* Number of bits per color map pixel */

	

	/* Set up and write out the header */



	BinByteOrder (BINLBF);

	stype  = IMTGA_RLECOLORMAP;

	spix   = 16;

        cmsize = 24;



	/* write the header out */

        imTgaWriteHeader ( ioType, fd, fp,flagsTable, tagTable,stype,spix,cmsize);



	imTgaWriteRLE16(ioType,fd,fp,flagsTable,tagTable);

	return(1);

}



/*

 *  FUNCTION

 *	imTgaWriteRLE16 -  write a Targa CLT file that has 16 bits pre channel

 *					and is RLE.

 *

 *  DESCRIPTION

 *	That VFB is queried, 	The VFB data is then copied to the file.

 */

static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWriteRLE16( int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imTgaWriteRLE16( ioType, fd, fp, flagsTable, tagTable )

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     vfbPtr;		/* Pixel pointer		*/

	TagEntry    *tagEntry;		/* Tmp table entry		*/

	int         stype;              /* Storage type                 */

        int         spix;               /* Number of bits per image pixel */

        int         cmsize;             /* Number of bits per color map pixel */

	unsigned char varRun[128*2];    /* Holds a variable run          */

 	int varRunLength;               /* Length of this variable run.  */

	int constantRunLength;          /* Length of this constant run.  */

	ImVfbPtr lastPixel, thisPixel;  /* point to pixels.              */

	int i;                          /* loop index                    */



	/*

	 * In this function, a variable run refers to a series of different

	 * pixel values.  The 'length' of such a run is one less than the

	 * number of pixels.  For instance, if our pixel values were

	 *  "A B C D" then this would be a "variable run of length 3".

	 *

	 * Similarly, "A A A" would be a "constant run of length 3".

	 */



	TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);



	/* Initialize stuff. */

	varRunLength      = 0;

	constantRunLength = 0;

	thisPixel = ImVfbQFirst( vfb );

	lastPixel = thisPixel;



	for (i = 1;i < ImVfbQWidth(vfb) * ImVfbQHeight( vfb ) ; i++)

	{

		ImVfbSInc(vfb,thisPixel);

		

		/* 

		 * If this pixel is the same as the last one then:

		 *   if varRunLength > 0, dump varRun.

		 *   otherwise, add this pixel to constantRun.

		 */

		 if (ImVfbSameIndex16(vfb,lastPixel,thisPixel))  

		 {

			if (varRunLength > 0)

			{

				imTgaDumpVarRun(2,varRun, varRunLength - 1); 

				varRunLength = 0;

			}

			constantRunLength++;

			if (constantRunLength==128)

			{   /* Dump a constant run. */

				imTgaDumpConstantRun2(ImVfbQIndex16(vfb,lastPixel), 

					constantRunLength - 1);

				constantRunLength = 0;

			}

		 }

		 else  /* This is different from the last character. */

		       /* Do the exact opposite of the if-branch.    */

		 {

			varRun[varRunLength*2]   = (ImVfbQIndex16(vfb, lastPixel) & 0xff00) >> 8;

			varRun[varRunLength*2+1] = ImVfbQIndex16(vfb, lastPixel) & 0x00ff;



			if (constantRunLength > 0)

			{

				imTgaDumpConstantRun2(ImVfbQIndex16(vfb, lastPixel),

					constantRunLength);

				constantRunLength = 0;

			}

			else

			{   /* Only increment if this is not the end of a constant run. */

				varRunLength++;

			}

			if (varRunLength==128)

			{   /* Dump variable run. */

				imTgaDumpVarRun(2,varRun, varRunLength - 1); 

				varRunLength = 0;

			}

		 }

		 lastPixel = thisPixel;

	}



	/* Dump whatever is left. */

	if (constantRunLength==0 && varRunLength==0)

	{  	/* single pixel is left. */

		imTgaDumpConstantRun2(ImVfbQIndex16(vfb, thisPixel), 0 );

	}

	if (constantRunLength > 0)

	{

		imTgaDumpConstantRun2(ImVfbQIndex16(vfb, thisPixel),

			constantRunLength );

	}

	if (varRunLength > 0)

	{

		varRun[varRunLength*2]   = (ImVfbQIndex16(vfb, lastPixel) & 0xff00) >> 8;

		varRun[varRunLength*2+1] = ImVfbQIndex16(vfb, lastPixel) & 0x00ff;

		imTgaDumpVarRun(2,varRun, varRunLength );

	}



	return (1);

}





/*

 *  FUNCTION

 *	imTgaWriteRGB16	-  write a Targa RGB with 2 bytes per pixel file

 *

 *  DESCRIPTION

 *	That VFB is queried, and the Targa file header written out.

 *	The VFB data is then copied to the file.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWriteRGB16( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable )

#else

imTgaWriteRGB16( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;	/* Pixel pointer		*/

	imTgaHeaderInfo  header;	/* Tga file header		*/

	unsigned char      *buffer;		/* line buffer			*/

	unsigned char      *pBuffer;		/* current location in  buffer  */

	int          n,x,y,i,j;		/* Counter			*/

	TagEntry    *tagEntry;		/* Tmp table entry		*/

	char	    *tmpString;		/* Tmp string holder		*/

	int         stype;              /* Storage type                 */

        int         spix;               /* Number of bits per image pixel */

        int         cmsize;             /* Number of bits per color map pixel */

	unsigned char	    expBlue,expGreen,expRed;	/* Pixels before shrunk	*/

	unsigned char	    byte1,byte2;	/* Pixels after shrunk to 2 bytes */

	



	/* Set up and write out the header */

	BinByteOrder (BINLBF);



	TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);



	stype  = IMTGA_UNCOMPRGB;

	spix   = 16;

	cmsize = 24;

	/* write the header out */

	imTgaWriteHeader ( ioType, fd, fp,flagsTable, tagTable,stype,spix,cmsize);

	x = ImVfbQWidth(vfb);

	y = ImVfbQHeight(vfb);



	/* allocate buffer space  for (width*length*3) scan lines */

	ImMalloc (buffer, unsigned char *,2*x);

	pptr = ImVfbQFirst (vfb);



	for (j=0; j<y; j++)

	{



		pBuffer = buffer;

		for (i=0; i<x; i++)

		{

			byte2 = ((ImVfbQAlpha (vfb, pptr))) << 7;

			expRed= ((ImVfbQRed (vfb, pptr))>>3) <<2 ;

			expGreen= ((ImVfbQGreen (vfb, pptr))>>2) >>4 ;

			byte2 = (byte2 | expRed |expGreen);

			byte1 = ((ImVfbQGreen (vfb, pptr))>>2) << 5;

			expBlue= (ImVfbQBlue (vfb, pptr))>>3 ;

			byte1 = (byte1 |expBlue);

			*pBuffer++ =byte1;

			*pBuffer++ =byte2;

			ImVfbSInc (vfb, pptr);

		}





		if (ImBinWrite (ioType, fd, fp, buffer, UCHAR,1,pBuffer - buffer) ==-1)

		{

			free ( (char *)buffer);

			ImReturnBinError( );

		}



	}



	free ( (char *)buffer);

	return (1);

}



/*

 *  FUNCTION

 *	imTgaWriteRGBRLE16	-  write a Targa RGB file that is RLE 2 bytes

 *							per pixel.

 *

 *  DESCRIPTION

 *	That VFB is queried, and the Targa file header written out.

 *	The VFB data is then copied to the file.

 */

static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWriteRGBRLE16( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE* fp, TagTable *flagsTable, TagTable *tagTable )

#else

imTgaWriteRGBRLE16( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;	/* Pixel pointer		*/

	imTgaHeaderInfo  header;	/* Tga file header		*/

	unsigned char      *bufferRle;		/* line buffer for rle packets	*/

	unsigned char      *bufferUn;		/* line buffer for unencoded 	*/

	unsigned char      *bufferCount;	/* Count buffer			*/

	unsigned char      *pBuffer;		/* current location in  buffer  */

	int          n,x,y,i,j;		/* Counter			*/

	TagEntry    *tagEntry;		/* Tmp table entry		*/

	char	    *tmpString;		/* Tmp string holder		*/

	int         stype;              /* Storage type                 */

        int         spix;               /* Number of bits per image pixel */

        int         cmsize;             /* Number of bits per color map pixel */

	int 	    total;		/* counter for number of bytes encoded*/

	int	    currentColor;	/* What color from vfb are we on */

	int	    field;		/* The field from the color we are on */

	int	    fieldNext;		/* The field from the color we are on */

	int	    fieldRed;		/* The field from the color we are on */

	int	    fieldGreen;		/* The field from the color we are on */

	int	    fieldBlue;		/* The field from the color we are on */

	int	    fieldAlpha;		/* The field from the color we are on */

	int	    fieldRedNext;	/* The field from the color we are on */

	int	    fieldGreenNext;	/* The field from the color we are on */

	int	    fieldBlueNext;	/* The field from the color we are on */

	int	    fieldAlphaNext;	/* The field from the color we are on */

	int	    count;		/* Numbe to store in packet	*/

	unsigned char	    byte1,byte2,byte3,byte4;/* Bytes to write from rle packet */

	int	    atend;		/* whether at end of vfb or not  */

	int	    stopSpot;		/* when should stop reading out of vfb*/

	unsigned char	    expBlue,expGreen,expRed;	/* Pixels before shrunk	*/

	

	



	/* Set up and write out the header */

	BinByteOrder (BINLBF);



	TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);





	stype  = IMTGA_RLERGB;

	spix   = 16;

        cmsize = 24;

	/* write the header out */

        imTgaWriteHeader ( ioType, fd, fp,flagsTable, tagTable,stype,spix,cmsize);



	y = ImVfbQWidth(vfb);

        x = ImVfbQHeight(vfb);

	stopSpot=(x*y*1)-3;



	/* allocate buffer space  for one RLE packet  */

	/* unencoded it can reach up to 129 bytes    */

	ImMalloc (bufferUn, unsigned char *,260);

	ImMalloc (bufferRle, unsigned char *,4);

	ImMalloc (bufferCount, unsigned char *,1);

	pptr = ImVfbQFirst (vfb);



	/* get the first pixel color to start with */

	total =0;	/* a running count of bytes encoded */

	currentColor = 0; /* Color to start with	    */

	atend =0;	/*not at the end of the vfb yet */



	fieldRed = ImVfbQRed(vfb,pptr);

	fieldGreen = ImVfbQGreen(vfb,pptr);

	fieldBlue = ImVfbQBlue(vfb,pptr);

	ImVfbSInc(vfb,pptr);

	fieldRedNext = ImVfbQRed(vfb,pptr);

	fieldGreenNext = ImVfbQGreen(vfb,pptr);

	fieldBlueNext = ImVfbQBlue(vfb,pptr);

	if ((fieldRed==fieldGreen) && (fieldGreen==fieldBlue)) field=fieldRed;

		else field = -1;

	if ((fieldRedNext==fieldGreenNext) && (fieldGreenNext==fieldBlueNext))

		 fieldNext=fieldRedNext;

		else fieldNext = -2;



	do {		/* encode width by height scanlines */

	   count=0;	/* Count-1 is what's stored*/



	/* see if you should rle something	*/

	   while ( (count<128) && (field == fieldNext) && (!atend) ) 

	   {

		if ( ( count+total) > stopSpot) atend=1;

		count++;

		fieldRed = fieldRedNext;

		fieldGreen = fieldGreenNext;

		fieldBlue = fieldBlueNext;

		fieldAlpha = fieldAlphaNext;

		ImVfbSInc(vfb,pptr);

		fieldRedNext = ImVfbQRed(vfb,pptr);

		fieldGreenNext = ImVfbQGreen(vfb,pptr);

		fieldBlueNext = ImVfbQBlue(vfb,pptr);

		fieldAlphaNext = ImVfbQAlpha(vfb,pptr);

		if ((fieldRed==fieldGreen) && (fieldGreen==fieldBlue)

		   && (fieldBlue==fieldAlpha)) field=fieldRed;

			else field = -1;

		if ((fieldRedNext==fieldGreenNext) && (fieldGreenNext==fieldBlueNext) && (fieldBlueNext==fieldAlphaNext))

			 fieldNext=fieldRedNext;

			else fieldNext = -2;

	   }

	   if (count>0)		/* Actually have something to encode */

	   {

		count--;

		pBuffer = bufferRle;

		byte1= count | 0x80;	/* Store 1 then count in one byte */

		*pBuffer++ = byte1;

			byte4 = fieldAlpha << 7;

			expRed= (fieldRed>>3) <<2 ;

			expGreen= (fieldGreen>>2) >>4 ;

			byte4 = (byte4 | expRed |expGreen);

			byte3 = (fieldGreen>>2) << 5;

			expBlue= fieldBlue >>3 ;

			byte3 = (byte3 |expBlue);

		*pBuffer++ = byte3;

		*pBuffer++ = byte4;

		if (ImBinWrite (ioType,fd,fp,bufferRle,UCHAR,1,3)==-1)

		{

		   free ( (char *) bufferRle);

		   ImReturnBinError( );

		}

	   }

	        else

	   	{

		   count= -1;

		   pBuffer = bufferUn;

		   while ( (count<127) && (field !=fieldNext) && (!atend) )

		   {

			byte4 = fieldAlpha << 7;

			expRed= (fieldRed>>3) <<2 ;

			expGreen= (fieldGreen>>2) >>4 ;

			byte4 = (byte4 | expRed |expGreen);

			byte3 = (fieldGreen>>2) << 5;

			expBlue= fieldBlue >>3 ;

			byte3 = (byte3 |expBlue);

		     *pBuffer++ = byte3;

		     *pBuffer++ = byte4;

		     if ( ( count+total) > stopSpot) atend=1;

		     count++;

		     fieldRed = fieldRedNext;

		     fieldGreen = fieldGreenNext;

		     fieldBlue = fieldBlueNext;

		     fieldAlpha = fieldAlphaNext;

		     ImVfbSInc(vfb,pptr);

		     fieldRedNext = ImVfbQRed(vfb,pptr);

		     fieldGreenNext = ImVfbQGreen(vfb,pptr);

		     fieldBlueNext = ImVfbQBlue(vfb,pptr);

		     fieldAlphaNext = ImVfbQAlpha(vfb,pptr);

		     if ((fieldRed==fieldGreen) && (fieldGreen==fieldBlue)

			&& (fieldBlue==fieldAlpha)) field=fieldRed;

		     	else field = -1;

		     if ((fieldRedNext==fieldGreenNext) && (fieldGreenNext==fieldBlueNext) && (fieldBlueNext==fieldAlphaNext))

		     	 fieldNext=fieldRedNext;

		     	else fieldNext = -2;

		   }



			

		   byte1= count  & 0x7F; /* Store 0 then count in one byte*/

		   pBuffer = bufferCount;

		   *pBuffer = byte1;

		   if (ImBinWrite (ioType,fd,fp,bufferCount,UCHAR,1,1)==-1)

		   {

		      free ( (char *) bufferCount);

		      ImReturnBinError( );

		   }

		   if (ImBinWrite (ioType,fd,fp,bufferUn,UCHAR,1,2*(count+1))==-1)

		   {

		      free ( (char *) bufferUn);

		      ImReturnBinError( );

		   }



		  

		 }





	   total+=count+1;



	} while (!atend);	/* encode width by height scanlines */



	free ( (char *)bufferRle);

	free ( (char *)bufferUn);

	free ( (char *)bufferCount);



	return (1);



}







/*

 *  FUNCTION

 *	imTgaWriteGREY8	-  write a Targa file that is greyscale with 8bits

 *

 *  DESCRIPTION

 *	That VFB is queried, and the Targa file header written out.

 *	The VFB data is then copied to the file.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWriteGREY8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imTgaWriteGREY8( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	int         stype;              /* Storage type                 */

        int         spix;               /* Number of bits per image pixel */

        int         cmsize;             /* Number of bits per color map pixel */

	

	BinByteOrder (BINLBF);



	/* Set up and write out the header */

	stype  = IMTGA_UNCOMPBW;

	spix   = 8;

	cmsize = 0;

	/* write the header out */

	imTgaWriteHeader ( ioType, fd, fp,flagsTable, tagTable,stype,spix,cmsize);

	imTgaWrite8image ( ioType, fd, fp,flagsTable, tagTable);

	return (1);

}

/*

 *  FUNCTION

 *	imTgaWriteGREYRLE8	-  write a Targa file that is greyscale

 *				  run-length-encoded with 2 bytes per pixel.

 *

 *  DESCRIPTION

 *	That VFB is queried, and the Targa file header written out.

 *	The VFB data is then copied to the file.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWriteGREYRLE8( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imTgaWriteGREYRLE8( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	int         stype;              /* Storage type                 */

        int         spix;               /* Number of bits per image pixel */

        int         cmsize;             /* Number of bits per color map pixel */

	



	BinByteOrder (BINLBF);



	stype  = IMTGA_RLEBW;

	spix   = 8;

	cmsize = 0;

	/* write the header out */

	imTgaWriteHeader ( ioType, fd, fp,flagsTable, tagTable,stype,spix,cmsize);



	imTgaWriteRLE8 (ioType, fd, fp, flagsTable,tagTable);

	return (1);

}

/*

 *  FUNCTION

 *	imTgaWriteGREY16  -  write a Targa file that is greyscale with 16bits.

 *

 *  DESCRIPTION

 *	That VFB is queried, and the Targa file header written out.

 *	The VFB data is then copied to the file.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWriteGREY16( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imTgaWriteGREY16( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	int         stype;              /* Storage type                 */

        int         spix;               /* Number of bits per image pixel */

        int         cmsize;             /* Number of bits per color map pixel */

	

	BinByteOrder (BINLBF);



	/* Set up and write out the header */

	stype  = IMTGA_UNCOMPBW;

	spix   = 16;

	cmsize = 0;



	/* write the header out */

	imTgaWriteHeader ( ioType, fd, fp,flagsTable, tagTable,stype,spix,cmsize);

	imTgaWrite16image (ioType, fd, fp, flagsTable,tagTable);

	return (1);

}







/*

 *  FUNCTION

 *	imTgaWriteGREYRLE16	-  write a Targa file run-length-encoded

 *				   with 16 bits per pixel.

 *

 *  DESCRIPTION

 *	That VFB is queried, and the Targa file header written out.

 *	The VFB data is then copied to the file.

 */



static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWriteGREYRLE16( ImFileFormatWriteMap *pMap, int ioType, int fd, FILE *fp, TagTable *flagsTable, TagTable *tagTable)

#else

imTgaWriteGREYRLE16( pMap, ioType, fd, fp, flagsTable, tagTable )

	ImFileFormatWriteMap *pMap;	/* Write map entry to adhear to	*/

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable   *flagsTable;		/* Flags			*/

	TagTable    *tagTable;		/* Tag table to read from	*/

#endif

{

	int         stype;              /* Storage type                 */

        int         spix;               /* Number of bits per image pixel */

        int         cmsize;             /* Number of bits per color map pixel */

	



	BinByteOrder (BINLBF);



	/* Set up and write out the header */

	stype  = IMTGA_RLEBW;

	spix   = 16;

	cmsize = 0;



	/* write the header out */

	imTgaWriteHeader ( ioType, fd, fp,flagsTable, tagTable,stype,spix,cmsize);

	imTgaWriteRLE16 (ioType, fd, fp, flagsTable,tagTable);

	return (1);

}





/*

 *  FUNCTION

 *	imTgaWriteHeader	-  write a Targa file header

 *

 *  DESCRIPTION

 *	Based on info passed to it a standard Targa file header written out.

 *      If there is a color lookup table then it too is written out.

 *	If there is a name field it too is written out.

 */

static int				/* Returns # of tags used	*/

#ifdef __STDC__

imTgaWriteHeader( int ioType, int fd, FILE *fp,TagTable *flagsTable, TagTable *tagTable,int stype,int spix,int cmsize )

#else

imTgaWriteHeader( ioType, fd, fp,flagsTable, tagTable,stype,spix,cmsize )

	int          ioType;		/* I/O flags			*/

	int          fd;		/* Input file descriptor	*/

	FILE	    *fp;		/* Input file pointer		*/

	TagTable    *flagsTable;

	TagTable    *tagTable;

	int	    stype;		/* what data storage type it is */	

	int	    spix;		/* The number of bits per image pixel*/

	int	    cmsize;		/* The number of bits per clt pixel*/

#endif

{

	ImVfb        *vfb;		/* Read in image		*/

	ImVfbPtr     pptr;		/* Pixel pointer		*/

	ImClt		*clt;		/* CLT pointer			*/

	ImCltPtr	pColor;

	imTgaHeaderInfo  header;	/* Tga file header		*/

	unsigned char      *buffer;	/* line buffer			*/

	unsigned char      *pBuffer;	/* current location in  buffer  */

	int          n,x,y,i,j;		/* Counter			*/

	TagEntry    *tagEntry;		/* Tmp table entry		*/

	char	    *tmpString;		/* Tmp string holder		*/

	int	   tga_attbits;		/* number of attribute bits per pix. */

	int        tga_rsrvd;		/* Reserved bit			*/

	int	   tga_orgbita;		/* Screen origin bit 4		*/

	int	   tga_orgbitb;		/* Screen origin bit 5		*/

	char	   message[200];	/* Used for ImInfo string	*/

	



	/* Set up and write out the header */

	BinByteOrder (BINLBF);



	TagEntryQValue (TagTableQDirect (tagTable, "image vfb", 0), &vfb);



	clt = ImVfbQClt(vfb);



	/* Get the name field if there is one  */

	tagEntry = TagTableQDirect (tagTable, "image name",0); 

	if (tagEntry == TAGENTRYNULL) header.tga_numc_ident = 0;

	else

	{

		TagEntryQValue(tagEntry, &tmpString);

		header.tga_numc_ident=strlen(tmpString);

                if (header.tga_numc_ident>255) header.tga_numc_ident=255;

        	ImInfo( "Image Name", tmpString );

	}



	if (ImVfbQClt(vfb)==IMCLTNULL) 

		header.tga_cmtype = 0;

	else if (cmsize == 0) 

		header.tga_cmtype = 0;

	else 

		header.tga_cmtype = 1;   



	header.tga_stype    = stype;

	header.tga_cmorigin = 0;

	if (header.tga_cmtype) 

		header.tga_cmlen = ImCltQNColors(clt);

	   else header.tga_cmlen = 0;

	header.tga_cmsize = cmsize;

	header.tga_imxorg = 0;

	header.tga_imyorg = 0;

	header.tga_imwid  = ImVfbQWidth(vfb);

	header.tga_imhgt  = ImVfbQHeight( vfb);

	header.tga_spix   = spix;

	tga_attbits = 0;

	tga_orgbita = 0 << 4;

	tga_orgbitb = 1 << 5;

	tga_rsrvd   = 0 << 6;

	header.tga_imdesc=tga_attbits|tga_orgbita|tga_orgbitb|tga_rsrvd;



	/* information to printout if verbose flag has been set */

	ImInfo ("Byte Order", "Least Significant Byte First");



	sprintf (message, "%d x %d", ImVfbQWidth(vfb), ImVfbQHeight(vfb));

	ImInfo ("Resolution", message);



	if ( (stype==IMTGA_UNCOMPCOLORMAP) || (stype==IMTGA_RLECOLORMAP) )

		sprintf (message, "%d-bit Color Indexed", spix);



	if ( (stype==IMTGA_UNCOMPRGB) || (stype==IMTGA_RLERGB) )

		sprintf (message, "%d-bit RGB", spix);



	if ( (stype==IMTGA_UNCOMPBW) || (stype==IMTGA_RLEBW) )

		sprintf (message, "%d-bit Greyscale", spix);

        ImInfo( "Type", message );



	if (cmsize>0)

	{

		sprintf (message, "%d Entries",cmsize);

        	ImInfo( "Color Table", message );

	}



	/* information to printout if verbose flag has been set */

	if ((stype>8) && (stype<12))

	   	sprintf (message, "Run Length Encoded (RLE)");

	else

	   	sprintf (message, "none");

        ImInfo( "Compression Type", message );



	if (((spix==16) || (spix==32) ) && ( (stype==IMTGA_UNCOMPRGB) || (stype==IMTGA_RLERGB)))

	   	sprintf (message, "%d-bit", spix);

	else

	   	sprintf (message, "none");

        ImInfo( "Alpha Channel", message );



	/* Now we read the image pixel data in from our data file */

	/* write the header out */

	if (ImBinWriteStruct (ioType, fd, fp, &header, imTgaHeaderFields) ==-1)

		ImReturnBinError( );



	if (header.tga_numc_ident)  /* if there is a name to write */



	  ImBinWrite (ioType, fd, fp, tmpString, UCHAR,1,header.tga_numc_ident);





	/* write out a clt if there is one */

	if (header.tga_cmlen)

	{

		ImMalloc (buffer, unsigned char *, 3*header.tga_cmlen)

       		pColor = ImCltQFirst(clt);

        	pBuffer = buffer;



        	for (j=0; j<header.tga_cmlen; j++)

        	{

               	   *pBuffer++ = ImCltQBlue  ( pColor);

               	   *pBuffer++ = ImCltQGreen( pColor);

               	   *pBuffer++ = ImCltQRed ( pColor);

               	   ImCltSInc(clt, pColor);

        	}

        	if (ImBinWrite (ioType, fd, fp, buffer, UCHAR,1,pBuffer - buffer) ==-1)

        	{

               	 free ( (char *)buffer);

               	 ImReturnBinError( );

        	}

        	free ( (char *)buffer);

	}

	return(1);

}







