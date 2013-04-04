/**

 **     $Header: /roq/libim/imgiflzw.c 1     11/02/99 4:38p Zaphod $

 **     Copyright (c) 1989-1995  San Diego Supercomputer Center (SDSC)

 **             a division of General Atomics, San Diego, California, USA

 **

 **     Users and possessors of this source code are hereby granted a

 **     nonexclusive, royalty-free copyright and design patent license to

 **     use this code in individual software.  License is not granted for

 **     commercial resale, in whole or in part, without prior written

 **     permission from SDSC.  This source is provided "AS IS" without express

 **     or implied warranty of any kind.

 **

 **     For further information contact:

 **             E-Mail:         info@sds.sdsc.edu

 **

 **             Surface Mail:   Information Center

 **                             San Diego Supercomputer Center

 **                             P.O. Box 85608

 **                             San Diego, CA  92138-5608

 **                             (619) 534-5000

 **/

 

#define HEADER  "    $Header: /roq/libim/imgiflzw.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **     imgiflzw.c         -  Do Lempel-Ziv & Welch uncompress/compress

 **

 **  PROJECT

 **     libim   	   -  SDSC image manipulation library

 **

 **  DESCRIPTION

 **     imlzw.c contains rountines for decoding and encoding bytes 

 **     using Lempel-Ziv & Welch uncompress/compress algorithm.

 **

 **  PUBLIC CONTENTS

 **                     d =defined constant

 **                     f =function

 **                     m =defined macro

 **                     t =typedef/struct/union

 **                     v =variable

 **                     ? =other

 **

 **	imLzwCompGif           f

 **

 **  PRIVATE CONTENTS

 **

 **	 imLzwGetCode		f

 **	 imLzwError		f

 **	 imLzwGifNextPixel	f

 **	 imLzwPackBits		f

 **	 

 **	 struct str_table_node  t                         

 **      struct str_table_node  t

 **      strTableNode		t

 **      strTableNodePtr	t

 **      strTable		t

 **

 **

 **      IMMAX_LWZ_BITS         d   maximum number of bits allowed by LWZ

 **      IMMAXIMUMCODE          d   maximum code size

 **      IMBLOCKSIZE            d   maximum block byte count plus one

 **      IMNULLPREFIX           d   null prefix   

 **

 **	 ImGifEasyFail          m   non fatal error ocurred. report the error.

 **

 **  CODE CREDITS

 **     David Koblas, 1990.

 **     Custom development, Soraya Gonzales, Intevep S.A., Venzuela, 1991.

 **     Extensions, Vinh Le, San Diego Supercomputer Center, 1992.

 **     Extensions, Dave Nadeau, San Diego Supercomputer Center, 1993.

 **

 **

 **  HISTORY

 **     $Log: /roq/libim/imgiflzw.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 **	Revision 1.11  1995/06/29  00:28:04  bduggan

 **	updated copyright year

 **

 **	Revision 1.10  1995/06/16  08:43:52  bduggan

 **	cleaned up code formatting

 **

 **	Revision 1.9  1995/01/10  23:25:55  bduggan

 **	Updated copyright notice

 **

 **	Revision 1.8  1994/10/03  11:29:39  nadeau

 **	Updated to ANSI C and C++ compatibility.

 **	Removed all use of register keyword.

 **	Minimized use of custom SDSC types (e.g., uchar vs. unsigned char)

 **	Changed all float arguments to double.

 **	Added forward declarations.

 **	Added misc. casts to passify SGI and DEC compilers.

 **	Changed all macros and defined constants to have names

 **	starting with IM.

 **	Updated comments.

 **	Updated indenting on some code.

 **	Updated copyright message.

 **

 **	Revision 1.7  92/08/31  17:24:09  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.6  91/10/03  09:03:23  nadeau

 **	Fixed #includes.

 **	

 **	Revision 1.5  91/03/21  08:51:38  nadeau

 **	ifdef-ed out inclusion of strings.h.  Nothing seems

 **	to need it and it confuses the compiler on SysV vs.

 **	BSD machines (string.h vs. strings.h).

 **	

 **	Revision 1.4  91/03/13  17:11:53  soraya

 **	*** empty log message ***

 **	

 **	Revision 1.3  91/03/11  09:19:07  soraya

 **	Optimization and comment cleanup.

 **	

 ** Revision 1.2  91/01/31  08:34:42  soraya

 **	Adding  comments

 **	 

 **

 **

 **/



#if 0

The lines between the dashes are taken directly from the file jrdgif.c:

-----------------------------------------------------------------------------

/*

 * jrdgif.c

 *

 * Copyright (C) 1991, 1992, Thomas G. Lane.

 * This file is part of the Independent JPEG Group's software.

 * For conditions of distribution and use, see the accompanying README file.

 *

 * This file contains routines to read input images in GIF format.

 *

 * These routines may need modification for non-Unix environments or

 * specialized applications.  As they stand, they assume input from

 * an ordinary stdio stream.  They further assume that reading begins

 * at the start of the file; input_init may need work if the

 * user interface has already read some data (e.g., to determine that

 * the file is indeed GIF format).

 *

 * These routines are invoked via the methods get_input_row

 * and input_init/term.

 */



/*

 * This code is loosely based on giftoppm from the PBMPLUS distribution

 * of Feb. 1991.  That file contains the following copyright notice:

 * +-------------------------------------------------------------------+

 * | Copyright 1990, David Koblas.                                     |

 * |   Permission to use, copy, modify, and distribute this software   |

 * |   and its documentation for any purpose and without fee is hereby |

 * |   granted, provided that the above copyright notice appear in all |

 * |   copies and that both that copyright notice and this permission  |

 * |   notice appear in supporting documentation.  This software is    |

 * |   provided "as is" without express or implied warranty.           |

 * +-------------------------------------------------------------------+

 *

 * We are also required to state that

 *    "The Graphics Interchange Format(c) is the Copyright property of

 *    CompuServe Incorporated. GIF(sm) is a Service Mark property of

 *    CompuServe Incorporated."

 */

-----------------------------------------------------------------------------



#endif



/*

 *	imLzwGetCode

 *	imLzwReadByte

 *	

 *	Gif Image decompression - from LZW to Gif

 *				  Written by David Koblas.

 *				  Copyright 1989

 *				  Modified by Soraya Gonzalez

 *				  San Diego Supercomputer Center

 *				  to include it in the image library

 *	==================================================================

 *	imLzwError             

 *      imLzwGifNextPixel      

 *      imLzwPackBits          

 *      imLzwCompGif          

 *

 *

 *	 GIF Image compression - LZW algorithm implemented with Trie type

 *                         	 structure.

 *                         	 Written by Bailey Brown, Jr.

 *		                 last change May 24, 1990

 *	 Modified by Soraya Gonzalez.

 *		     San Diego Supercomputer Center.

 *		     December 1990

 *		     to include it in the image library

 *

 *

 */





#ifdef USE_STDLIB_H

#include <stdlib.h>

#else



#ifdef old

#ifdef USE_STRING_H

#include <string.h>

#else

#include <strings.h>

#endif

#endif





#endif /* USE_STDLIB_H */



#include "iminternal.h"









/*

 *  TYPEDEF & STRUCTURE

 *	struct str_table_node	                       

 *	struct str_table_node

 *	strTableNode

 *	strTableNodePtr

 *	strTable

 *  DESCRIPTION

 *

 */



typedef struct str_table_entry {

        int code;

        int prefix;

        int suffix;

}  strTableEntry;



typedef struct str_table_node {

        strTableEntry entry;

        struct str_table_node *left;

        struct str_table_node *right;

        struct str_table_node *children;

} strTableNode, *strTableNodePtr, **strTable;



/*

 *  CONSTANTS

 *

 *	 IMMAX_LWZ_BITS		- maximum number of bits allowed by LWZ

 *	 IMMAXIMUMCODE		- maximum code size

 *	 IMBLOCKSIZE		- maximum block byte count plus one

 *	 IMNULLPREFIX 		- null prefix

 */



#define IMMAX_LWZ_BITS          12

#define IMMAXIMUMCODE           4095    /* 2**maximum_code_size     */

#define IMBLOCKSIZE             256     /* max block byte count + 1 */

#define IMNULLPREFIX            -1





/*

 *  MACROS

 *	ImGifEasyFail		- non fatal error occurred. report the error.

 */



#define ImGifEasyFail(str,status)  			\
	{						\
		ImErrorWarning(str,status,IMEUNKNOWN);	\
		return(status);				\
	}



/*

 *  FILE-WIDE VARIABLE 

 */



static int	RasterIndex;			/* index over the rasterdata array  */



/*

 * FUNCTION

 *	imLzwGetCode	- return the next code from the input stream.

 *

 * DESCRIPTION

 *	return the next code from the input stream. The code read

 *	has code_size size. flag is used to initialize

 *	the internal variables.

 */	



static int

#ifdef __STDC__

imLzwGetCode(int ioType, int fd, FILE *fp, int code_size, int flag)

#else

imLzwGetCode(ioType, fd, fp, code_size, flag)

int		ioType;		/* I/O flags		   	   */

int		fd;		/* Input file descriptor     	   */

FILE		*fp;		/* Input file pointer 		   */

int		code_size;	/* current code size used  	   */

int		flag;		/* used for to initializations     */

#endif

{

    static unsigned char	buf[280];

    static int			curbit,lastbit,done,last_byte;

    int				i, j, ret;

    unsigned char		count;



    if (flag) 

    {

	curbit  = 0;

	lastbit = 0;

	done    = FALSE;

	return 0;

    }



    if ( (curbit+code_size) >= lastbit) 

    {

	if (done) 

	    if (curbit>=lastbit)

		ImGifEasyFail("Ran off the end of my bits",-1);

	buf[0] = buf[last_byte-2];

	buf[1] = buf[last_byte-1];

	if ( ImBinRead( ioType,fd,fp,&count, UCHAR, 1, 1 ) == -1 )

	    ImReturnBinError( );

	if (count == 0)

	    done = TRUE;

	else if ( ImBinRead( ioType,fd,fp,&buf[2],UCHAR,1,count ) == -1 ) 

	    ImReturnBinError( );

	last_byte = 2 + count;

	curbit    = (curbit - lastbit) + 16;

	lastbit   = (2+count)*8 ;

    }

    ret = 0;

    for( i = curbit, j = 0; j < code_size; i++, j++ ) 

	ret |= ((buf[ i / 8 ] & (1 << (i % 8))) != 0) << j;



    curbit += code_size;



    return ret;

}  



/*

 * FUNCTION

 *    ImLzwReadByte	- return the next decoded pixel

 *

 * DESCRIPTION

 *    Return the next decoded pixel value. 	

 */



int

#ifdef __STDC__

imLzwReadByte(int ioType,int fd,FILE *fp,int flag,int input_code_size)

#else

imLzwReadByte(ioType,fd,fp,flag,input_code_size)

int		ioType;		/* I/O flags		 */

int		fd;		/* Input file descriptor */

FILE		*fp;		/* Input file pointer	 */

int		flag;		/* to make intializations*/

int		input_code_size;/* code size		 */

#endif

{

    static int		fresh=FALSE;

    int			code,incode;

    unsigned char	count,junk;

    static int		code_size,set_code_size;

    static int		max_code,max_code_size;

    static int		firstcode,oldcode;

    static int		clear_code,end_code;

    static int		table[2][(1<< IMMAX_LWZ_BITS)];

    static int		stack[(1<<(IMMAX_LWZ_BITS))*2],*sp;

    int	i;



    if (flag) 

    {

	set_code_size = input_code_size;

	code_size     = set_code_size+1;

	clear_code    = 1 << set_code_size ;

	end_code      = clear_code + 1;

	max_code_size = 2*clear_code;

	max_code      = clear_code+2;



	imLzwGetCode(ioType,fd,fp,NULL,TRUE);

		

	fresh=TRUE;



	for (i=0;i<clear_code;i++) 

	{

	    table[0][i] = 0;

	    table[1][i] = i;

	}

	for (;i<(1<<IMMAX_LWZ_BITS);i++)

	    table[0][i] = table[1][0] = 0;



	sp = stack;



	return 0;

    } 

    else 

    if (fresh) 

    {

	fresh = FALSE;

	do 

	{

	    firstcode=oldcode=

		imLzwGetCode(ioType,fd,fp, code_size, FALSE);

	} while (firstcode == clear_code);

	return firstcode;

    }



    if (sp > stack) 

	return *--sp;



    while ((code=imLzwGetCode(ioType,fd,fp,code_size,FALSE))>=0) 

    {

	if (code == clear_code) 

	{

	    for (i=0;i<clear_code;i++) 

	    {

		table[0][i] = 0;

		table[1][i] = i;

	    }

	    for (;i<(1<<IMMAX_LWZ_BITS);i++)

		table[0][i] = table[1][i] = 0;

	    code_size     = set_code_size+1;

	    max_code_size = 2*clear_code;

	    max_code      = clear_code+2;

	    sp = stack; 

	    firstcode=oldcode=

		imLzwGetCode(ioType,fd,fp,code_size,FALSE);

	    return firstcode;

	} 

	else 

	if (code == end_code) 

	{

	    while ( ImBinRead(ioType,fd,fp,&count,UCHAR,1,1) != -1  && (count!=0))

		while (count-->0 && ImBinRead( ioType,fd,fp,&junk,UCHAR,1,1) != -1);

	    if (count!=0)

		ImGifEasyFail("Missing EOD in data stream (common occurance)",-1);

	    return -1;

	}



	incode = code;



	if (code >= max_code) 

	{

	    *sp++ = firstcode;

	    code = oldcode;

	}



	while (code >= clear_code) 

	{

	    *sp++ = table[1][code];

	    if (code == table[0][code])

		ImGifEasyFail("Circular table entry BIG ERROR",-1);

	    code = table[0][code];

	}



	*sp++ = firstcode = table[1][code];



	if ((code=max_code)<(1<<IMMAX_LWZ_BITS)) 

	{

	    table[0][code] = oldcode;

	    table[1][code] = firstcode;

	    max_code++;

	    if ((max_code >= max_code_size) && 

		(max_code_size < (1<<IMMAX_LWZ_BITS))) 

		{

			max_code_size *= 2;

			code_size++;

	    	}

	}



	oldcode = incode;



	if (sp > stack) 

	    return *--sp;

    }

    return code;

}



/*

 *  FUNCTION

 *    ImLzwError	- prints the error ocurred in the standar error file

 *			  and return with status equal to -1

 */





static int

#ifdef __STDC__

imLzwError(char *s)

#else

imLzwError(s)

char *s;

#endif

{

    ImErrorFatal(s, -1, IMEUNKNOWN);

}





/*

 *  FUNCTION

 *    ImLzwGifNextPixel		- Return the next pixel from the raster data

 *

 *  DESCRIPTION

 *    The pixel values are stored in the rasterdata buffer. This

 *    function returns the next pixel value.

 */



static int

#ifdef __STDC__

imLzwGifNextPixel(unsigned char *rasterdata,int size)

#else

imLzwGifNextPixel( rasterdata,size)

unsigned char *rasterdata;

int size;

#endif

{



	if ( RasterIndex == size )

		return (EOF);

	return( rasterdata[RasterIndex++] );

}



/*

 *   FUNCTION

 *	imLzwPackBits		- packs the bits of the codes generated 

 *				  by imLzwCompGif.

 *   DESCRIPTION

 *      imLzwPackBits() packs the bits of the codes generated by ImLzwCompGif

 *	into a 1..256 byte output block. The first byte of the block is the 

 *	number 0..255 of data bytes in the block. To flush or initialize

 *	the block, pass a negative argument.

 */



static int

#ifdef __STDC__

imLzwPackBits(int compress_size, int prefix, int ioType, int fd, FILE *fp)

#else

imLzwPackBits(compress_size, prefix, ioType, fd, fp)

int compress_size;

int prefix;

int     ioType;

int     fd;

FILE    *fp;

#endif

{

    static int cur_bit = 8;

    static unsigned char block[IMBLOCKSIZE] = { 0 };

    int i, left_over_bits;



    /* if we are about to excede the bounds of block or if the flush

       code (code_bis < 0) we output the block */

    if((cur_bit + compress_size > (IMBLOCKSIZE-1)*8) || (prefix < 0)) 

    {

        /* handle case of data overlapping blocks */

        if ((left_over_bits = (((cur_bit>>3) +

                ((cur_bit & 7) != 0))<<3) - cur_bit) != 0) 

	{

            for (i=0; i < left_over_bits; i++) 

	    {

                if (prefix & (1<<i)) 

			block[cur_bit>>3] |= (char)(1<<(cur_bit & 7));

                /* note n>>3 == n/8 and n & 7 == n % 8 */

                cur_bit++;

            }

        }

        compress_size -= left_over_bits;

        prefix = prefix>>left_over_bits;

        block[0] =  (unsigned char)((cur_bit>>3) - 1);

        if (block[0]) 

	   if ( ImBinWrite( ioType, fd, fp, block, UCHAR, 1, block[0]+1) == -1)

                        ImReturnBinError();

        for(i=0; i < IMBLOCKSIZE; i++) 

		block[i] = 0;

        cur_bit = 8;

    }

    if (prefix >= 0) 

    {

        for (i=0; i < compress_size; i++) 

	{

            if (prefix & (1<<i)) 

		block[cur_bit>>3] |= (unsigned char)(1<<(cur_bit & 7));

            /* note n>>3 == n/8 and n & 7 == n % 8 */

            cur_bit++;

        }

    }

    return (1);

}





/*

 *   FUNCTION

 *	ImLzwCompGif	- compresses and pack the pixels values that are

 *			  stored in rasterdata buffer using the

 *			  LZW algorithm. 

 *   DESCRIPTION

 *	ImLzwCompGif () recieves pointers to a buffer and an output

 *	stream, and the code size as parameters and outputs successive

 *	blocks of LZW compressed gif data. The calling routine should

 *	have aready written the GIF file header out to the output file.

 *	It assumes that there will be no more than 8 bits/pixel and that

 *	each data item comes from successive bytes returned by 

 *	ImLzwGifNextPixel().

 */



int

#ifdef __STDC__

imLzwCompGif(int ioType,int fd,FILE *fp,int code_size, unsigned char *rasterdata, int size)

#else

imLzwCompGif(ioType,fd,fp,code_size, rasterdata, size)

int               ioType;       /* I/O flags                    	*/

int               fd;           /* Input file descriptor        	*/

FILE             *fp;           /* Input file pointer           	*/

int 		  code_size;    /* i.e. for 8 bits/pixel code_size = 9; */

unsigned char	 *rasterdata;	/* rasterdata to be compress		*/

int		  size;		/* size of the raster data		*/

#endif

{

    strTable heap; 		/* our very own memory manager */

    int heap_index;

    int clear_code, end_code, cur_code;

    int i, found, num_colors, prefix, compress_size;

    int cur_char, end_of_data, bits_per_pix;

    strTableNodePtr cur_node;

    strTable root;	  	/* root of string table for LZW compression */

                                /* is an array of 2**bits_per_pix pointers

				   to atomic nodes */

    heap_index = 0;

    heap = (strTable)malloc(sizeof(strTableNodePtr)*IMMAXIMUMCODE);

    if (heap == NULL) 

	imLzwError("can't allocate heap");

    for (i=0; i < IMMAXIMUMCODE; i++) 

    {

	heap[i] = (strTableNodePtr)malloc(sizeof(strTableNode));

        if (heap[i] == NULL) 

		imLzwError("can't allocate heap");

    }

    bits_per_pix  = code_size - 1;

    compress_size = code_size;

    num_colors    = 1<<(bits_per_pix);

    clear_code    = num_colors;

    end_code      = clear_code + 1;

    cur_code      = end_code + 1;

    RasterIndex   = 0;

    prefix        = IMNULLPREFIX;

    root = (strTable)malloc(sizeof(strTableNodePtr)*num_colors);

    if (!root) 

	imLzwError("memory allocation failure (root)");

    for(i=0; i<num_colors; i++) 

    {

	root[i] = heap[heap_index++];

        root[i]->entry.code   = i;

        root[i]->entry.prefix = IMNULLPREFIX;

        root[i]->entry.suffix = i;

	root[i]->left         = NULL;

	root[i]->right        = NULL;

        root[i]->children     = NULL;

    }



    /* initialize  output block */

    imLzwPackBits(compress_size, -1, ioType,fd,fp);

    imLzwPackBits(compress_size, clear_code, ioType,fd,fp);

    end_of_data = 0;

    if ((cur_char = imLzwGifNextPixel(rasterdata,size)) == EOF) 

	imLzwError("premature end of data");

    while (!end_of_data) 

    {

        prefix = cur_char;

        cur_node = root[prefix];

        found = 1;

        if ((cur_char = imLzwGifNextPixel(rasterdata,size)) == EOF) 

	{

        	end_of_data = 1; 

		break;

        }

        while(cur_node->children && found) 

	{

        	cur_node = cur_node->children;

        	while(cur_node->entry.suffix != cur_char) 

		{

			if (cur_char < cur_node->entry.suffix) 

			{

				if (cur_node->left) 

					cur_node = cur_node->left;

				else 

				{

					cur_node->left = heap[heap_index++];

					cur_node = cur_node->left;

					found = 0; 

					break;

				}

			}

			else 

			{

				if (cur_node->right) 

					cur_node = cur_node->right;

				else 

				{

					cur_node->right = heap[heap_index++];

					cur_node = cur_node->right;

					found = 0; 

					break;

				}

			}

		}

            if (found) 

	    {

            	prefix = cur_node->entry.code;

            	if((cur_char = imLzwGifNextPixel(rasterdata,size)) == EOF) 

		{

			end_of_data = 1; 

			break;

                }

            }

        }

        if (end_of_data) 

		break;

        if (found) 

	{

            cur_node->children = heap[heap_index++];

            cur_node = cur_node->children;

        }

        cur_node->children     = NULL;

        cur_node->left         = NULL;

        cur_node->right        = NULL;

        cur_node->entry.code   = cur_code;

        cur_node->entry.prefix = prefix;

        cur_node->entry.suffix = cur_char;

        imLzwPackBits(compress_size, prefix, ioType,fd,fp);

        if (cur_code > ((1<<(compress_size))-1))

            compress_size++;

        if (cur_code < IMMAXIMUMCODE) 

	{

            cur_code++;

        }

        else 

	{

        	heap_index = num_colors;  /* reinitialize string table */

        	for (i=0; i < num_colors; i++ ) 

			root[i]->children = NULL;

        	imLzwPackBits(compress_size, clear_code, ioType,fd,fp);

        	compress_size = bits_per_pix + 1;

        	cur_code = end_code + 1;

        }

    }

    imLzwPackBits(compress_size, prefix, ioType,fd,fp);

    imLzwPackBits(compress_size, end_code, ioType,fd,fp);

    imLzwPackBits(compress_size, -1, ioType,fd,fp);

    for (i=0; i < IMMAXIMUMCODE; i++) 

	free(heap[i]);

    free(heap);

    free(root);

    return (1);

}



