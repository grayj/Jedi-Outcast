/**
 **	$Header: /roq/libim/imvfbhist.c 1     11/02/99 4:38p Zaphod $
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

#define HEADER	"    $Header: /roq/libim/imvfbhist.c 1     11/02/99 4:38p Zaphod $"



/**
 **  FILE
 **	imhist.c	-  VFB histogram and statistics functions
 **
 **  PROJECT
 **	libim		-  SDSC image manipulation library
 **
 **  DESCRIPTION
 **	imhist compute image statistics and histograms.
 **
 **  PUBLIC CONTENTS
 **			d =defined constant
 **			f =function
 **			m =defined macro
 **			t =typedef/struct/union
 **			v =variable
 **			? =other
 **
 **	ImVfbStat		f  compute simple image statistics
 **	ImVfbHist		f  compute a histogram of pixel values
 **	ImVfbHistTableFree	f  free histogram results
 **
 **  PRIVATE CONTENTS
 **	imVfbHistTableSort	f  sort histogram results
 **	imVfbHistTableAlloc	f  allocate a histogram data table
 **	imVfbHistFieldOrder	f  determine the best ordering for field hashing
 **	imVfbHistAddValue	f  add value to histogram table
 **	imVfbHistHashFree	f  free a hash table
 **
 **	imXXXHist		f  histogram computation functions
 **
 **	imHistBucket		t  bucket in collision list
 **	imHistBucketHeader	t  collision list header
 **
 **	imHistRGBA4WayFuncs	v  function table for 4-way RGBA hashing
 **	imHistRGBA3WayFuncs	v  function table for 3-way RGBA hashing
 **	imHistRGBA2WayFuncs	v  function table for 2-way RGBA hashing
 **
 **	imHistHSIA4WayFuncs	v  function table for 4-way HSIA hashing
 **	imHistHSIA3WayFuncs	v  function table for 3-way HSIA hashing
 **	imHistHSIA2WayFuncs	v  function table for 2-way HSIA hashing
 **
 **	IM_HIST1FIELD	m  build a function for single-field histograms
 **	IM_HIST1FIELDHSI	m  same deal but for HSI fields
 **
 **	IM_HIST2FIELD	m  build a function for double-field histograms
 **	IM_HIST2FIELD1HSI	m  same deal but for 1 HSI field
 **	IM_HIST2FIELD2HSI	m  same deal but for 2 HSI fields
 **
 **	IM_HIST3FIELD	m  build a function for triple-field histograms
 **	IM_HIST3FIELD1HSI	m  same deal but for 1 HSI field
 **	IM_HIST3FIELD2HSI	m  same deal but for 2 HSI fields
 **	IM_HIST3FIELD3HSI	m  same deal but for 2 HSI fields
 **
 **	IM_HIST4FIELD	m  build a function for quadruple-field histograms
 **	IM_HIST4FIELD3HSI	m  same deal but for 3 HSI fields
 **	IM_HIST4FIELD3HSI2	m  same deal but for 3 HSI fields, different alpha loc
 **
 **  HISTORY
 **     $Log: /roq/libim/imvfbhist.c $
* 
* 1     11/02/99 4:38p Zaphod
 **     Revision 1.9  1995/06/29  00:28:04  bduggan
 **     updated copyright year
 **
 **     Revision 1.8  1995/06/16  08:54:12  bduggan
 **     changed bzero to memset
 **
 **     Revision 1.7  1994/10/03  11:29:51  nadeau
 **     Updated to ANSI C and C++ compatibility.
 **     Removed all use of register keyword.
 **     Minimized use of custom SDSC types (e.g., uchar vs. unsigned char)
 **     Changed all float arguments to double.
 **     Added forward declarations.
 **     Added misc. casts to passify SGI and DEC compilers.
 **     Changed all macros and defined constants to have names
 **     starting with IM.
 **     Updated comments.
 **     Updated indenting on some code.
 **     Updated copyright message.
 **
 **     Revision 1.7  1994/10/03  11:29:51  nadeau
 **     Updated to ANSI C and C++ compatibility.
 **     Removed all use of register keyword.
 **     Minimized use of custom SDSC types (e.g., uchar vs. unsigned char)
 **     Changed all float arguments to double.
 **     Added forward declarations.
 **     Added misc. casts to passify SGI and DEC compilers.
 **     Changed all macros and defined constants to have names
 **     starting with IM.
 **     Updated comments.
 **     Updated indenting on some code.
 **     Updated copyright message.
 **
 **     Revision 1.6  92/12/03  02:39:26  nadeau
 **     Removed bogus semi-colons where they should be.
 **     
 **     Revision 1.5  92/12/03  01:55:03  nadeau
 **     Total rewrite.
 **     
 **     Revision 1.4  92/08/31  17:38:40  vle
 **     Updated copyright notice.
 **     
 **     Revision 1.3  92/08/26  12:46:23  groening
 **     minor error corrections
 **     
 **     Revision 1.2  92/08/07  14:38:30  groening
 **     Imitial revision
 **     
 **/

/**
 **  CODE CREDITS
 **     Custom development, Chris Groening, San Diego Supercomputer Center, 1992.
 **     Custom development, Dave Nadeau, San Diego Supercomputer Center, 1992.
 **/

#include "iminternal.h"




/*
 *  STRUCT & TYPEDEF
 *	imHistBucket	-  bucket in collision list
 *
 *  DESCRIPTION
 *	Hash table collisions result in searching a collision list of
 *	buckets containing 3rd and 4th histogram field values (the 1st and
 *	2nd histogram field values are used as hash table indexes).  Each
 *	bucket holds an occurrence count and the two field values for which
 *	the bucket exists.
 */
#define IM_STARTSIZE       5
#define IM_SIZEINC         20
typedef struct imHistBucket
{
	sdsc_uint16 imhist_value1;	/* First value in bucket.		*/
	sdsc_uint16 imhist_value2;	/* Second value in bucket.		*/
	sdsc_uint32 imhist_nOccur;	/* Number of times combo has occurred	*/
} imHistBucket;





/*
 *  STRUCT & TYPEDEF
 *	imHistBucketHeader	-  collision list header
 *
 *  DESCRIPTION
 *	Collision lists are pointed to by a collision list header that is
 *	pointed at by each hash table entry.  The header gives the size
 *	of the collision list, the next slot in the list that's available,
 *	and a pointer to the list itself.
 */
typedef struct imHistBucketHeader
{
        int     imhist_allocSize;	/* Allocated size of imhist_list*/
        int     imhist_nextSlot;	/* Next free slot in imhist_list*/
        imHistBucket *imhist_list;	/* Collision list of buckets	*/
} imHistBucketHeader;


#ifdef __STDC__
static struct ImHistTable *	imVfbHistTableSort( ImHistTable *histTable, int left, int right );
static void 			imVfbHistFieldOrder( int fieldMask, int hashFields[3], ImVfb *vfb );
static ImHistTable *		imVfbHistTableAlloc( int numEntry, int fieldMask, int numChannels );

extern ImHistTable *    imRHist(ImVfb* vfb );
extern ImHistTable *    imBHist(ImVfb* vfb );
extern ImHistTable *    imGHist(ImVfb* vfb );
extern ImHistTable *    imAHist(ImVfb* vfb );

extern ImHistTable *    imHHist( ImVfb* vfb);
extern ImHistTable *    imSHist( ImVfb* vfb);
extern ImHistTable *    imIHist( ImVfb* vfb);

extern ImHistTable *    imMHist( ImVfb* vfb);
extern ImHistTable *    imMAHist( ImVfb* vfb);
extern ImHistTable *    im8Hist( ImVfb* vfb);
extern ImHistTable *    im8AHist( ImVfb* vfb);
extern ImHistTable *    im16Hist( ImVfb* vfb);
extern ImHistTable *    im16AHist( ImVfb* vfb);

extern ImHistTable *	imRGHist( ImVfb* vfb);
extern ImHistTable *	imRBHist( ImVfb* vfb);
extern ImHistTable *	imRAHist( ImVfb* vfb);
extern ImHistTable *	imGBHist( ImVfb* vfb);
extern ImHistTable *	imGAHist( ImVfb* vfb);
extern ImHistTable *	imBAHist( ImVfb* vfb);

extern ImHistTable *	imHSHist( ImVfb* vfb);
extern ImHistTable *	imHIHist( ImVfb* vfb);
extern ImHistTable *	imHAHist( ImVfb* vfb);
extern ImHistTable *	imSIHist( ImVfb* vfb);
extern ImHistTable *	imSAHist( ImVfb* vfb);
extern ImHistTable *	imIAHist( ImVfb* vfb);
#else
extern struct ImHistTable *	imVfbHistTableSort( );
extern void 			imVfbHistFieldOrder( );
extern ImHistTable *		imVfbHistTableAlloc( );

extern ImHistTable *    imRHist( );
extern ImHistTable *    imBHist( );
extern ImHistTable *    imGHist( );
extern ImHistTable *    imAHist( );

extern ImHistTable *    imHHist( );
extern ImHistTable *    imSHist( );
extern ImHistTable *    imIHist( );

extern ImHistTable *    imMHist( );
extern ImHistTable *    imMAHist( );
extern ImHistTable *    im8Hist( );
extern ImHistTable *    im8AHist( );
extern ImHistTable *    im16Hist( );
extern ImHistTable *    im16AHist( );

extern ImHistTable *	imRGHist( );
extern ImHistTable *	imRBHist( );
extern ImHistTable *	imRAHist( );
extern ImHistTable *	imGBHist( );
extern ImHistTable *	imGAHist( );
extern ImHistTable *	imBAHist( );

extern ImHistTable *	imHSHist( );
extern ImHistTable *	imHIHist( );
extern ImHistTable *	imHAHist( );
extern ImHistTable *	imSIHist( );
extern ImHistTable *	imSAHist( );
extern ImHistTable *	imIAHist( );

#endif

#ifdef __STDC__

ImHistTable *imRGBHist(ImVfb * vfb);
ImHistTable *imRGAHist(ImVfb *vfb);
ImHistTable *imRBGHist(ImVfb *vfb);
ImHistTable *imRBAHist(ImVfb *vfb);
ImHistTable *imRAGHist(ImVfb *vfb);
ImHistTable *imRABHist(ImVfb *vfb);
ImHistTable *imBGRHist(ImVfb *vfb);
ImHistTable *imBGAHist(ImVfb *vfb);
ImHistTable *imGARHist(ImVfb *vfb);
ImHistTable *imGABHist(ImVfb *vfb);
ImHistTable *imBARHist(ImVfb *vfb);
ImHistTable *imBAGHist(ImVfb *vfb);
ImHistTable *imHSIHist(ImVfb *vfb);
ImHistTable *imHSAHist(ImVfb *vfb);
ImHistTable *imHISHist(ImVfb *vfb);
ImHistTable *imHIAHist(ImVfb *vfb);
ImHistTable *imHASHist(ImVfb *vfb);
ImHistTable *imHAIHist(ImVfb *vfb);
ImHistTable *imSIHHist(ImVfb *vfb);
ImHistTable *imSIAHist(ImVfb *vfb);
ImHistTable *imSAHHist(ImVfb *vfb);
ImHistTable *imSAIHist(ImVfb *vfb);
ImHistTable *imIAHHist(ImVfb *vfb);
ImHistTable *imIASHist(ImVfb *vfb);
ImHistTable *imRGBAHist(ImVfb *vfb);
ImHistTable *imRBGAHist(ImVfb *vfb);
ImHistTable *imRAGBHist(ImVfb *vfb);
ImHistTable *imRGBAHist(ImVfb *vfb);
ImHistTable *imGBRAHist(ImVfb *vfb);
ImHistTable *imGARBHist(ImVfb *vfb);
ImHistTable *imBARGHist(ImVfb *vfb);
ImHistTable *imHSIAHist(ImVfb *vfb);
ImHistTable *imHISAHist(ImVfb *vfb);
ImHistTable *imHAISHist(ImVfb *vfb);
ImHistTable *imSIHAHist(ImVfb *vfb);
ImHistTable *imSAHIHist(ImVfb *vfb);
ImHistTable *imIAHSHist(ImVfb *vfb);
ImHistTable *imHAISHist(ImVfb *vfb);

#else

ImHistTable *imRGBHist();
ImHistTable *imRGAHist();
ImHistTable *imRBGHist();
ImHistTable *imRBAHist();
ImHistTable *imRAGHist();
ImHistTable *imRABHist();
ImHistTable *imBGRHist();
ImHistTable *imBGAHist();
ImHistTable *imGARHist();
ImHistTable *imGABHist();
ImHistTable *imBARHist();
ImHistTable *imBAGHist();
ImHistTable *imHSIHist();
ImHistTable *imHSAHist();
ImHistTable *imHISHist();
ImHistTable *imHIAHist();
ImHistTable *imHASHist();
ImHistTable *imHAIHist();
ImHistTable *imSIHHist();
ImHistTable *imSIAHist();
ImHistTable *imSAHHist();
ImHistTable *imSAIHist();
ImHistTable *imIAHHist();
ImHistTable *imIASHist();
ImHistTable *imRGBAHist();
ImHistTable *imRBGAHist();
ImHistTable *imRAGBHist();
ImHistTable *imRGBAHist();
ImHistTable *imGBRAHist();
ImHistTable *imGARBHist();
ImHistTable *imBARGHist();
ImHistTable *imHSIAHist();
ImHistTable *imHISAHist();
ImHistTable *imHAISHist();
ImHistTable *imSIHAHist();
ImHistTable *imSAHIHist();
ImHistTable *imIAHSHist();
ImHistTable *imHAISHist();

#endif



/*
 *  GLOBALS
 *	imHistRGBA4WayFuncs	-  function table for 4-way RGBA hashing
 *	imHistRGBA3WayFuncs	-  function table for 3-way RGBA hashing
 *
 *	imHistHSIA4WayFuncs	-  function table for 4-way HSIA hashing
 *	imHistHSIA3WayFuncs	-  function table for 3-way HSIA hashing
 *
 *  DESCRIPTION
 *	A zillion individual histogram computation functions are vectored
 *	to based upon the choice of histogram fields.  Vectoring is by
 *	way of the tables below, indexed into by the field numbers
 *	involved.
 */

#define IMVFBH_RED	0
#define IMVFBH_GREEN	1
#define IMVFBH_BLUE	2
#define IMVFBH_ALPHA	3
#define IMVFBH_HUE	0
#define IMVFBH_SATURATION 1
#define IMVFBH_INTENSITY 2

#ifdef __STDC__
static ImHistTable *(*imHistRGBA4WayFuncs[4][4])(ImVfb* ) =
#else
static ImHistTable *(*imHistRGBA4WayFuncs[4][4])( ) =
#endif
{
	/*		RED	      GREEN 	  BLUE 	      ALPHA	*/
	/* RED */	{ NULL,       imRGBAHist, imRBGAHist, imRAGBHist },
	/* GREEN */	{ imRGBAHist, NULL,       imGBRAHist, imGARBHist },
	/* BLUE */	{ imRBGAHist, imGBRAHist, NULL,       imBARGHist },
	/* ALPHA */	{ imRAGBHist, imGARBHist, imBARGHist, NULL }
};

#ifdef __STDC__
static ImHistTable *(*imHistRGBA3WayFuncs[4][4][4])( ImVfb* ) =
#else
static ImHistTable *(*imHistRGBA3WayFuncs[4][4][4])( ) =
#endif
{
	/*		RED     GREEN 	BLUE	ALPHA	*/
	/* RED */	{{NULL,NULL,NULL,NULL},
				{NULL,NULL,imRGBHist,imRGAHist},
					{NULL,imRBGHist,NULL,imRBAHist},
						{NULL,imRAGHist,imRABHist,NULL}},
	/* GREEN */	{{NULL,NULL,imRGBHist,imRGAHist},
				{NULL,NULL,NULL,NULL},
					{imBGRHist,NULL,NULL,imBGAHist},
						{imGARHist,NULL,imGABHist,NULL}},
	/* BLUE */	{{NULL,imRBGHist,NULL,imRBAHist},
				{imBGRHist,NULL,NULL,imBGAHist},
					{NULL,NULL,NULL,NULL},
						{imBARHist,imBAGHist,NULL,NULL}},
	/* ALPHA */	{{NULL,imRAGHist,imRABHist,NULL},
				{imGARHist,NULL,imGABHist,NULL},
					{imBARHist,imBAGHist,NULL,NULL},
						{NULL,NULL,NULL,NULL}}
};

#ifdef __STDC__
static ImHistTable *(*imHistHSIA4WayFuncs[4][4])( ImVfb* ) =
#else
static ImHistTable *(*imHistHSIA4WayFuncs[4][4])( ) =
#endif
{
	/*		HUE	      SAT 	  INTENT      ALPHA	*/
	/* HUE */	{ NULL,       imHSIAHist, imHISAHist, imHAISHist },
	/* SAT */	{ imHSIAHist, NULL,       imSIHAHist, imSAHIHist },
	/* INTENT */	{ imHISAHist, imSIHAHist, NULL,       imIAHSHist },
	/* ALPHA */	{ imHAISHist, imSAHIHist, imIAHSHist, NULL }
};

#ifdef __STDC__
static ImHistTable *(*imHistHSIA3WayFuncs[4][4][4])( ImVfb* ) =
#else
static ImHistTable *(*imHistHSIA3WayFuncs[4][4][4])( ) =
#endif
{
	/*		HUE     SAT 	INTEN	ALPHA	*/
	/* HUE */	{{NULL,NULL,NULL,NULL},
				{NULL,NULL,imHSIHist,imHSAHist},
					{NULL,imHISHist,NULL,imHIAHist},
						{NULL,imHASHist,imHAIHist,NULL}},
	/* SAT */	{{NULL,NULL,imHSIHist,imHSAHist},
				{NULL,NULL,NULL,NULL},
					{imSIHHist,NULL,NULL,imSIAHist},
						{imSAHHist,NULL,imSAIHist,NULL}},
	/* INTEN */	{{NULL,imHISHist,NULL,imHIAHist},
				{imSIHHist,NULL,NULL,imSIAHist},
					{NULL,NULL,NULL,NULL},
						{imIAHHist,imIASHist,NULL,NULL}},
	/* ALPHA */	{{NULL,imHASHist,imHAIHist,NULL},
				{imSAHHist,NULL,imSAIHist,NULL},
					{imIAHHist,imIASHist,NULL,NULL},
						{NULL,NULL,NULL,NULL}}
};


/*
 *  FUNCTION
 *	ImVfbStat	-  compute simple image statistics
 *
 *  DESCRIPTION
 *	Scan the image and compute the minimum, maximum, and number of
 *	unique values for the field.  All this is accomplished by scanning
 *	the image and using the pixel's field value as an index into a
 *	hash table of value occurrence counts (all initially zero).
 *
 *	Note that the hash table size varies depending upon the type of
 *	field being scanned.  For 8-bit integer fields, the table size
 *	is 256.  For 16-bit integers its 65536.  For saturation,
 *	and intensity floating point fields, we map from the range 0.0 to 1.0
 *	to 0 to 1024.  For the hue floating point field, we map from the
 *	range 0.0 to 360.0 to 0 to 1024.
 */


#define IMVFBH_HASH( vfb, query, hash )					\
{									\
	int i, j;		/* Counters				*/\
	int h, w;		/* Height and width of image		*/\
	ImVfbPtr pPixel;	/* Pixel pointer			*/\
									\
	pPixel = ImVfbQFirst( vfb );					\
	h      = ImVfbQHeight( vfb );					\
	w      = ImVfbQWidth( vfb );					\
	for ( i = 0; i < h; i++ )					\
	{								\
		for ( j = 0; j < w; j++ )				\
		{							\
			hash[ query( vfb, pPixel ) ]++;			\
			ImVfbSInc( vfb, pPixel );			\
		}							\
	}								\
}
#define IMVFBH_HASHHSI( vfb, which, factor, hash )			\
{									\
	int i, j;		/* Counters				*/\
	int h, w;		/* Height and width of image		*/\
	ImVfbPtr pPixel;	/* Pixel pointer			*/\
	int	 rgb[3];	/* RGB triplet				*/\
	float	 hsi[3];	/* HSI equivalent			*/\
									\
	pPixel = ImVfbQFirst( vfb );					\
	h      = ImVfbQHeight( vfb );					\
	w      = ImVfbQWidth( vfb );					\
	for ( i = 0; i < h; i++ )					\
	{								\
		for ( j = 0; j < w; j++ )				\
		{							\
			rgb[0] = ImVfbQRed(   vfb, pPixel );		\
			rgb[1] = ImVfbQGreen( vfb, pPixel );		\
			rgb[2] = ImVfbQBlue(  vfb, pPixel );		\
			ImRgbToHsi( rgb, hsi );				\
			hash[ (int)(hsi[which] * (factor)) ]++;		\
			ImVfbSInc( vfb, pPixel );			\
		}							\
	}								\
}



float *			/* Returns pointer to results		*/
#ifdef __STDC__
ImVfbStat( ImVfb *vfb, int field, float data[IMMAXNUMSTATS] )
#else
ImVfbStat( vfb, field, data )
	ImVfb	*vfb;		/* incoming vfb				*/
	int	field;		/* What field to perform operation on	*/
	float   data[IMMAXNUMSTATS];	/* array to return results in	*/
#endif
{
	sdsc_int32   *hash;		/* Hash table				*/
	int      hashSize;	/* Hash table size			*/
	int	 i;		/* Counter				*/


	/*
	 *  Create the hash table and zero it out.
	 */
	hashSize = 256;
	if ( field & IMVFBINDEX16 )
		hashSize = 65536;
	if ( field & (IMHUE|IMSATURATION|IMINTENSITY) )
		hashSize = 1024;
	if ( (hash = (sdsc_int32 *) malloc( sizeof( sdsc_int32 ) * hashSize )) == NULL )
	{
		ImErrNo = IMEMALLOC;
		return ( NULL );
	}
	for ( i = 0; i < hashSize; i++ )
		hash[i] = 0;


	/*
	 *  Hash the field values into the table, counting occurrences.
	 */
	if ( field & IMMONO )		IMVFBH_HASH( vfb, ImVfbQMono, hash );
	if ( field & IMINDEX8 )		IMVFBH_HASH( vfb, ImVfbQIndex8, hash );
	if ( field & IMINDEX16 )	IMVFBH_HASH( vfb, ImVfbQIndex16, hash );
	if ( field & IMRED )		IMVFBH_HASH( vfb, ImVfbQRed, hash );
	if ( field & IMGREEN )		IMVFBH_HASH( vfb, ImVfbQGreen, hash );
	if ( field & IMBLUE )		IMVFBH_HASH( vfb, ImVfbQBlue, hash );
	if ( field & IMALPHA )		IMVFBH_HASH( vfb, ImVfbQAlpha, hash );

	if ( field & IMHUE )		IMVFBH_HASHHSI( vfb, 0, 1023.0/360.0, hash );
	if ( field & IMSATURATION )	IMVFBH_HASHHSI( vfb, 1, 1023.0, hash );
	if ( field & IMINTENSITY )	IMVFBH_HASHHSI( vfb, 2, 1023.0, hash );
	

	/*
	 *  Scan the hash table.  The first non-zero value near the top
	 *  of the hash table is the lowest value occurring in the image.
	 *  Similarly, the first non-zero value near the bottom of the
	 *  hash table is the highest value occurring in the image.
	 *  Finally, we count the number of non-zero entries to get a
	 *  count of the number of unique values for the field.
	 */
	data[IMMINIMUM] = -1.0;
	for ( i = 0; i < hashSize; i++ )
	{
		if ( hash[i] != 0 )
		{
			/* First non-zero entry at top of table.	*/
			data[IMMINIMUM] = (float)i;
			break;
		}
	}
	data[IMMAXIMUM] = -1.0;
	for ( i = hashSize-1; i >= 0; i-- )
	{
		if ( hash[i] != 0 ) 
		{
			/* First non-zero entry at bottom of table.	*/
			data[IMMAXIMUM] = (float)i;
			break;
		}
	}	
	data[IMUNIQUEVAL] = 0.0;
	for ( i = 0; i < hashSize; i++ )
		if ( hash[i] != 0 )
			data[IMUNIQUEVAL] += 1.0;
		
		if ( field & IMHUE )
		{
			data[IMMINIMUM] *= (360.0/1023.0);
			data[IMMAXIMUM] *= (360.0/1023.0);
		}
		if ( field & (IMSATURATION|IMINTENSITY) )
	{
		data[IMMINIMUM] /= 1023.0;
		data[IMMAXIMUM] /= 1023.0;
	}

	free( (char *)hash );

	return ( data );
}





/*
 *  FUNCTION
 *	ImVfbHist	-  compute a histogram of pixel values
 *
 *  DESCRIPTION
 *	Based upon image attributes and the field selection for creating
 *	the histogram, we vector to any of umpteen individualized histogram
 *	computation routines.  The finished results are then sorted and
 *	returned.
 *
 *	Our argument checking insures that color spaces are not mixed and
 *	that unsupported fields are excluded.  This leaves the following
 *	histogram combinations:
 *
 *		R, G, B, H, S, I, A, I8, and I16 individually
 *		I8A and I16A
 *		any 2 of RGBA
 *		any 2 of HSIA
 *		any 3 of RGBA
 *		any 3 of HSIA
 *		all 4 of RGBA
 *		all 4 of HSIA
 */

ImHistTable *		/* Returns histogram data		*/
#ifdef __STDC__
ImVfbHist( ImVfb *vfb, int fieldMask, int sort )
#else
ImVfbHist( vfb, fieldMask, sort )
	ImVfb	*vfb;		/* incoming vfb to do histogram on 	*/
	int	fieldMask;	/* what fields to do histogram on 	*/
	int	sort;		/* whether or not to sort histogram results */
#endif
{
	ImHistTable *histTable;	/* return data structure		*/
	int     hashFields[3];	/* Optimal hash ordering of fields	*/
	int     nColorSpaces;	/* Number of color spaces asked for	*/
	int	nRGBA;		/* Number of RGBA fields asked for	*/
	int	nHSIA;		/* Number of HSIA fields asked for	*/


	/*
	 *  Check that the user hasn't requested any illegal field mixes.
	 */
	nColorSpaces = 0;
	if ( fieldMask & (IMRED|IMGREEN|IMBLUE) )
		nColorSpaces++;
	if ( fieldMask & (IMHUE|IMSATURATION|IMINTENSITY) )
		nColorSpaces++;
	if ( fieldMask & (IMMONO|IMINDEX8|IMINDEX16) )
		nColorSpaces++;
	if ( nColorSpaces > 1 )
	{
		/* Can't mix color spaces.				*/
		ImErrNo = IMEFIELD;
		return ( NULL );
	}
	if ( fieldMask & (IMZ|IMWPROT|IMIDATA|IMFDATA) )
	{
		/* Don't support these pixel fields.			*/
		ImErrNo = IMEFIELD;
		return ( NULL );
	}


	/*
	 *  Build the histogram.  The number of fields to use in computing
	 *  the histogram depends upon the fieldMask and varies from 1 to 4
	 *  in several combinations.  To keep things speedy and memory
	 *  efficient, each combination of fields is treated separately.
	 */
	nRGBA = 0;
	nHSIA = 0;
	if ( fieldMask & IMRED )	nRGBA++;
	if ( fieldMask & IMGREEN )	nRGBA++;
	if ( fieldMask & IMBLUE )	nRGBA++;
	if ( fieldMask & IMHUE )	nHSIA++;
	if ( fieldMask & IMSATURATION )	nHSIA++;
	if ( fieldMask & IMINTENSITY )	nHSIA++;
	if ( fieldMask & IMALPHA )	nRGBA++, nHSIA++;

	/*
	 *  When more than 2 fields are hashed, the choice of which two
	 *  fields to hash on is important.  For instance, if we hash on
	 *  red and green, but the image has loads of different shades of
	 *  blue, then the hash table will be only sparsely used, but we'll
	 *  have a huge collision list.  Since hashing is more efficient, we
	 *  want to choose which two fields to hash on intelligently.  This
	 *  is done by selecting the two fields with the most unique values
	 *  using imVfbHistFieldOrder().
	 *
	 *  When hashing on 2 or fewer fields, we don't worry about this.
	 */
	if ( nRGBA == 4 )
	{
		/*
		 *  Red, green, blue, and alpha, all at once.  That's 4
		 *  fields.  We'll create a hash table for hashing the first
		 *  two fields, then use a collision list afterwards for
		 *  the remaining 2 fields.
		 */
		imVfbHistFieldOrder( fieldMask, hashFields, vfb );
		histTable = (*imHistRGBA4WayFuncs[hashFields[0]][hashFields[1]])( vfb );
	}
	else if ( nRGBA == 3 )
	{
		/*
		 *  Any 3 of Red, green, blue, and alpha.  We'll create a
		 *  hash table for hashing the first two fields, then use
		 *  a collision list afterwards for the remaining field.
		 */
		imVfbHistFieldOrder( fieldMask, hashFields, vfb );
		histTable = (*imHistRGBA3WayFuncs[hashFields[0]][hashFields[1]][hashFields[2]])( vfb );
	}
	else if ( nRGBA == 2 )
	{
		/*
		 *  Any 2 of red, green, blue, and alpha.  We'll just hash
		 *  with both of them.
		 */
		if ( fieldMask & IMRED && fieldMask & IMGREEN )
			histTable = imRGHist( vfb );
		if ( fieldMask & IMRED && fieldMask & IMBLUE )
			histTable = imRBHist( vfb );
		if ( fieldMask & IMRED && fieldMask & IMALPHA )
			histTable = imRAHist( vfb );
		if ( fieldMask & IMGREEN && fieldMask & IMBLUE )
			histTable = imGBHist( vfb );
		if ( fieldMask & IMGREEN && fieldMask & IMALPHA )
			histTable = imGAHist( vfb );
		if ( fieldMask & IMBLUE && fieldMask & IMALPHA )
			histTable = imBAHist( vfb );
	}
	else if ( nHSIA == 4 )
	{
		/*
		 *  Hue, saturation, intensity, and alpha all at once.  That's
		 *  4 fields.  We'll create a hash table for hashing the first
		 *  two fields, then use a collision list afterwards for
		 *  the remaining 2 fields.
		 */
		imVfbHistFieldOrder( fieldMask, hashFields, vfb );
		histTable = (*imHistHSIA4WayFuncs[hashFields[0]][hashFields[1]])( vfb );
	}
	else if ( nHSIA == 3 )
	{
		/*
		 *  Any 3 of Hue, saturation, intensity, and alpha.
		 *  We'll create a hash table for hashing the first
		 *  two fields, then use a collision list afterwards for
		 *  the remaining field.
		 */
		imVfbHistFieldOrder( fieldMask, hashFields, vfb );
		histTable = (*imHistHSIA3WayFuncs[hashFields[0]][hashFields[1]][hashFields[2]])( vfb );
	}
	else if ( nHSIA == 2 )
	{
		/*
		 *  Any 2 of hue, saturation, intensity, and alpha.  We'll
		 *  just hash with both of them.
		 */
		if ( fieldMask & IMHUE && fieldMask & IMSATURATION )
			histTable = imHSHist( vfb );
		if ( fieldMask & IMHUE && fieldMask & IMINTENSITY )
			histTable = imHIHist( vfb );
		if ( fieldMask & IMHUE && fieldMask & IMALPHA )
			histTable = imHAHist( vfb );
		if ( fieldMask & IMSATURATION && fieldMask & IMINTENSITY )
			histTable = imSIHist( vfb );
		if ( fieldMask & IMSATURATION && fieldMask & IMALPHA )
			histTable = imSAHist( vfb );
		if ( fieldMask & IMINTENSITY && fieldMask & IMALPHA )
			histTable = imIAHist( vfb );
	}
	else if ( (fieldMask & (IMMONO|IMALPHA)) == (IMMONO|IMALPHA) )
		histTable = imMAHist( vfb );
	else if ( (fieldMask & (IMINDEX8|IMALPHA)) == (IMINDEX8|IMALPHA) )
		histTable = im8AHist( vfb );
	else if ( (fieldMask & (IMINDEX16|IMALPHA)) == (IMINDEX16|IMALPHA) )
		histTable = im16AHist( vfb );
	else if ( fieldMask & IMMONO )
		histTable = imMHist( vfb );
	else if ( fieldMask & IMINDEX8 )
		histTable = im8Hist( vfb );
	else if ( fieldMask & IMINDEX16 )
		histTable = im16Hist( vfb );
	else if ( fieldMask & IMRED )
		histTable = imRHist( vfb );
	else if ( fieldMask & IMGREEN )
		histTable = imGHist( vfb );
	else if ( fieldMask & IMBLUE )
		histTable = imBHist( vfb );
	else if ( fieldMask & IMHUE )
		histTable = imHHist( vfb );
	else if ( fieldMask & IMSATURATION )
		histTable = imSHist( vfb );
	else if ( fieldMask & IMINTENSITY )
		histTable = imIHist( vfb );
	else if ( fieldMask & IMALPHA )
		histTable = imAHist( vfb );
	else
	{
		ImErrNo = IMEFIELD;
		return ( NULL );
	}
	if ( histTable == NULL )
	{
		/* ImErrNo already set.					*/
		return ( NULL );
	}


	/*
	 *  Sort the results.
	 */
	if ( !sort )
		return ( histTable );
	histTable = imVfbHistTableSort( histTable, 0, histTable->imhist_nEntries-1 );
	if ( histTable == NULL )
	{
		ImErrNo = IMEMALLOC;
		return ( NULL );
	}
	return ( histTable );
}





/*
 *  FUNCTION
 *	imVfbHistTableSort	-  sort histogram results
 *
 *  DESCRIPTION
 *	Recursively sort a portion of a histogram results structure.
 *	Sort based upon the count value, from high to low.
 */


#define IM_SWAP( field, i, j, results )					\
{									\
	int tmp;		/* Temp value holder			*/\
	tmp = (results)->field[i];					\
	(results)->field[i] = (results)->field[j];			\
	(results)->field[j] = tmp;					\
}

#define IM_SWAPF( field, i, j, results )				\
{									\
	float tmp;		/* Temp value holder			*/\
	tmp = (results)->field[i];					\
	(results)->field[i] = (results)->field[j];			\
	(results)->field[j] = tmp;					\
}

static ImHistTable *		/* Returns sorted results		*/
#ifdef __STDC__
imVfbHistTableSort( ImHistTable *histTable, int left, int right )
#else
imVfbHistTableSort( histTable, left, right )
	ImHistTable *histTable;	/* Data to sort				*/
	int	left;		/* left indicie to sort			*/
	int	right;		/* right indicie to sort		*/
#endif
{
	sdsc_uint32  v;		/* Sort value				*/
	int	 i, j;		/* counters				*/


	if ( right <= left )
		/* End recursion and just return.			*/
		return ( histTable );

	v = histTable->imhist_nOccur[right];	/* start with right side*/
	i = left - 1;
	j = right;

	for ( ; ; )
	{
		/* Collapse the sort area to unsorted entries.		*/
		while ( histTable->imhist_nOccur[++i] > v )
			;
		while ( histTable->imhist_nOccur[--j] < v )
			;
		if ( i >= j )
			/* They're already sorted.			*/
			break;

		/* Swap the two values.					*/
		IM_SWAP( imhist_nOccur, i, j, histTable );

		/* And swap the individual field values as well.	*/
		if ( histTable->imhist_mono != NULL )
			IM_SWAP( imhist_mono, i, j, histTable );
		if ( histTable->imhist_index8 != NULL )
			IM_SWAP( imhist_index8, i, j, histTable );
		if ( histTable->imhist_index16 != NULL )
			IM_SWAP( imhist_index16, i, j, histTable );
		if ( histTable->imhist_red != NULL )
			IM_SWAP( imhist_red, i, j, histTable );
		if ( histTable->imhist_green != NULL )
			IM_SWAP( imhist_green, i, j, histTable );
		if ( histTable->imhist_blue != NULL )
			IM_SWAP( imhist_blue, i, j, histTable );
		if ( histTable->imhist_alpha != NULL )
			IM_SWAP( imhist_alpha, i, j, histTable );
		if ( histTable->imhist_hue != NULL )
			IM_SWAPF( imhist_hue, i, j, histTable );
		if ( histTable->imhist_saturation != NULL )
			IM_SWAPF( imhist_saturation, i, j, histTable );
		if ( histTable->imhist_intensity != NULL )
			IM_SWAPF( imhist_intensity, i, j, histTable );
	}


	/* Swap the current and right edge values.			*/
	IM_SWAP( imhist_nOccur, right, i, histTable );

	/* And swap the individual field values as well.		*/
	if ( histTable->imhist_mono != NULL )
		IM_SWAP( imhist_mono, right, i, histTable );
	if ( histTable->imhist_index8 != NULL )
		IM_SWAP( imhist_index8, right, i, histTable );
	if ( histTable->imhist_index16 != NULL )
		IM_SWAP( imhist_index16, right, i, histTable );
	if ( histTable->imhist_red != NULL )
		IM_SWAP( imhist_red, right, i, histTable );
	if ( histTable->imhist_green != NULL )
		IM_SWAP( imhist_green, right, i, histTable );
	if ( histTable->imhist_blue != NULL )
		IM_SWAP( imhist_blue, right, i, histTable );
	if ( histTable->imhist_alpha != NULL )
		IM_SWAP( imhist_alpha, right, i, histTable );
	if ( histTable->imhist_hue != NULL )
		IM_SWAPF( imhist_hue, right, i, histTable );
	if ( histTable->imhist_saturation != NULL )
		IM_SWAPF( imhist_saturation, right, i, histTable );
	if ( histTable->imhist_intensity != NULL )
		IM_SWAPF( imhist_intensity, right, i, histTable );


	/* Recurse on the two sublists.					*/
	imVfbHistTableSort( histTable, left, i-1 );
	imVfbHistTableSort( histTable, i+1, right );

	return ( histTable );
}

#undef IM_SWAP
#undef IM_SWAPF





/*
 *  FUNCTION
 *	ImVfbHistTableFree	-  free histogram results
 *
 *  DESCRUPTION
 *	Each of the field data lists is tossed.
 */
void				/* Returns nothing		*/
#ifdef __STDC__
ImVfbHistTableFree( ImHistTable *histTable )
#else
ImVfbHistTableFree( histTable )
	ImHistTable *histTable;		/* Data to toss			*/
#endif
{
	if ( histTable->imhist_nOccur != NULL )
		free( (char *)histTable->imhist_nOccur );
	if ( histTable->imhist_red != NULL )
		free( (char *)histTable->imhist_red );
	if ( histTable->imhist_green != NULL )
		free( (char *)histTable->imhist_green );
	if ( histTable->imhist_blue != NULL )
		free( (char *)histTable->imhist_blue );
	if ( histTable->imhist_alpha != NULL )
		free( (char *)histTable->imhist_alpha );
	if ( histTable->imhist_mono != NULL )
		free( (char *)histTable->imhist_mono );
	if ( histTable->imhist_index8 != NULL )
		free( (char *)histTable->imhist_index8 );
	if ( histTable->imhist_index16 != NULL )
		free( (char *)histTable->imhist_index16 );
	if ( histTable->imhist_hue != NULL )
		free( (char *)histTable->imhist_hue );
	if ( histTable->imhist_saturation != NULL )
		free( (char *)histTable->imhist_saturation );
	if ( histTable->imhist_intensity != NULL )
		free( (char *)histTable->imhist_intensity );

	free( (char *)histTable );
}





/*
 *  FUNCTION
 *	imVfbHistTableAlloc	-  allocate a histogram data table
 *
 *  DESCRIPTION
 *	Space is allocated for the histogram data table based upon the
 *	fields we're histogramming and the number of table entries.
 */

#define IM_HISTALLOC( mask, field, name, type, num, table )		\
{									\
	if ( ((mask) & (field)) && ((table)->name = (type *)malloc(	\
		sizeof( type ) * (num)) ) == NULL )			\
	{								\
		ImVfbHistTableFree( table );				\
		ImErrNo = IMEMALLOC;					\
		return ( NULL );					\
	}								\
}



static ImHistTable *		/* Returns histogram table		*/
#ifdef __STDC__
imVfbHistTableAlloc( int numEntry, int fieldMask, int numChannels )
#else
imVfbHistTableAlloc( numEntry, fieldMask, numChannels )
        int     numEntry;	/* number of entries in data strusture	*/
        int     fieldMask;	/* what fields to do histogram on	*/
        int     numChannels;	/* how many channels to do histogram on	*/
#endif
{
        ImHistTable   *histTable;	/* return data structure        */

	/* Allocate the table itself.					*/
	if ((histTable = (ImHistTable *)malloc( sizeof( ImHistTable )) )
		== NULL )
	{
		ImErrNo = IMEMALLOC;
		return( NULL );
	}
	memset( (void *) histTable, 0x00, sizeof( ImHistTable ) ); 


	/* Allocate the occurrence count list.				*/
	if ( (histTable->imhist_nOccur = (unsigned int *)malloc( sizeof( sdsc_uint32 ) *
		numEntry)) == NULL )
	{
		ImErrNo = IMEMALLOC;
		return ( NULL );
	}

	/* Allocate the individual field data lists.			*/
	IM_HISTALLOC( fieldMask, IMMONO, imhist_mono, unsigned char,
		numEntry, histTable );
	IM_HISTALLOC( fieldMask, IMINDEX8, imhist_index8, unsigned char,
		numEntry, histTable );
	IM_HISTALLOC( fieldMask, IMINDEX16, imhist_index16, sdsc_uint16,
		numEntry, histTable );
	IM_HISTALLOC( fieldMask, IMALPHA, imhist_alpha, unsigned char,
		numEntry, histTable );
	IM_HISTALLOC( fieldMask, IMRED, imhist_red, unsigned char,
		numEntry, histTable );
	IM_HISTALLOC( fieldMask, IMGREEN, imhist_green, unsigned char,
		numEntry, histTable );
	IM_HISTALLOC( fieldMask, IMBLUE, imhist_blue, unsigned char,
		numEntry, histTable );
	IM_HISTALLOC( fieldMask, IMHUE, imhist_hue, float,
		numEntry, histTable );
	IM_HISTALLOC( fieldMask, IMSATURATION, imhist_saturation, float,
		numEntry, histTable );
	IM_HISTALLOC( fieldMask, IMINTENSITY, imhist_intensity, float,
		numEntry, histTable );


	/* set up other header fields					*/
	histTable->imhist_nEntries  = numEntry;
	histTable->imhist_fieldMask = fieldMask;
	histTable->imhist_nFields   = numChannels; 

	return ( histTable );
}

#undef IM_HISTALLOC





/*
 *  FUNCTION
 *	imVfbHistFieldOrder	-  determine the best ordering for field hasing
 *
 *  DESCRIPTION
 *	To construct the histogram, pixel field data will be hashed into
 *	a hash table.  For multi-channel histograms, such as RGB histograms,
 *	two fields are combined to construct a single hash index (such as
 *	index = (red << 8) | green).  The selection of which two fields to
 *	combine is important.
 *
 *	If, for instance, we have an image with lots of shades of blue, but
 *	use red and green for the hash index, then the hash table will be
 *	sparsly filled and we'll have huge collision chains.  So, we'd like
 *	to pick the two hash fields as being the ones representing the most
 *	number of colors in the image.
 *
 *	To do this we compute image statistics on the number of unique values
 *	for each of the fields and pick the top two.
 */

static void				/* Returns nothing		*/
#ifdef __STDC__
imVfbHistFieldOrder( int fieldMask, int hashFields[3], ImVfb *vfb )
#else
imVfbHistFieldOrder( fieldMask, hashFields, vfb )
	int	fieldMask;		/* what fields to do histogram on */
	int     hashFields[3];		/* Returned field order		*/
	ImVfb	*vfb;			/* incoming vfb to do histogram on*/
#endif
{
	int	i;			/* Counter			*/
	float   stats[IMMAXNUMSTATS];	/* Image statistics		*/
	int	hashFieldUniques[4];	/* Number of unique values for fields*/
	int     biggest, biggestUniques;/* Biggest number of unique values*/
	int     bigger, biggerUniques;	/* Bigger number of unique values*/
	int     smaller, smallerUniques;/* Smaller number of unique values*/


	/*
	 *  Zero out our count of unique values for each of the fields.
	 */
	hashFieldUniques[IMVFBH_RED]        = 0;
	hashFieldUniques[IMVFBH_GREEN]      = 0;
	hashFieldUniques[IMVFBH_BLUE]       = 0;
	hashFieldUniques[IMVFBH_ALPHA]      = 0;
	hashFieldUniques[IMVFBH_HUE]        = 0;
	hashFieldUniques[IMVFBH_SATURATION] = 0;
	hashFieldUniques[IMVFBH_INTENSITY]  = 0;


	/*
	 *  Compute the number of unique values for each of the fields.
	 */
	if ( fieldMask & IMRED )
	{
		ImVfbStat( vfb, IMRED, stats );
		hashFieldUniques[IMVFBH_RED] = (int)stats[IMUNIQUEVAL];
	}
	if ( fieldMask & IMGREEN )
	{
		ImVfbStat( vfb, IMGREEN, stats );
		hashFieldUniques[IMVFBH_GREEN] = (int)stats[IMUNIQUEVAL];
	}
	if ( fieldMask & IMBLUE )
	{
		ImVfbStat( vfb, IMBLUE, stats );
		hashFieldUniques[IMVFBH_BLUE] = (int)stats[IMUNIQUEVAL];
	}
	if ( fieldMask & IMALPHA )
	{
		ImVfbStat( vfb, IMALPHA, stats );
		hashFieldUniques[IMVFBH_ALPHA] = (int)stats[IMUNIQUEVAL];
	}
	if ( fieldMask & IMHUE )
	{
		ImVfbStat( vfb, IMHUE, stats );
		hashFieldUniques[IMVFBH_HUE] = (int)stats[IMUNIQUEVAL];
	}
	if ( fieldMask & IMSATURATION )
	{
		ImVfbStat( vfb, IMSATURATION, stats );
		hashFieldUniques[IMVFBH_SATURATION] = (int)stats[IMUNIQUEVAL];
	}
	if ( fieldMask & IMINTENSITY )
	{
		ImVfbStat( vfb, IMINTENSITY, stats );
		hashFieldUniques[IMVFBH_INTENSITY] = (int)stats[IMUNIQUEVAL];
	}


	/*
	 *  Determine which fields have the largest number of unique values
	 *  and select them as the best choices for the hash field order.
	 *
	 *  NOTE:  RGB and HSI are mutually exclusive, but both can have
	 *  alpha fields.
	 */
	biggestUniques = biggerUniques = smallerUniques = -1;
	biggest        = bigger        = smaller        = -1;
	for ( i = 0; i < 4; i++ )
	{
		if ( hashFieldUniques[i] > biggestUniques )
		{
			biggestUniques = hashFieldUniques[i];
			biggest = i;
		}
	}
	for ( i = 0; i < 4; i++ )
	{
		if ( hashFieldUniques[i] <= biggestUniques &&
			i != biggest && hashFieldUniques[i] > biggerUniques )
		{
			biggerUniques = hashFieldUniques[i];
			bigger = i;
		}
	}
	for ( i = 0; i < 4; i++ )
	{
		if ( hashFieldUniques[i] <= biggerUniques &&
			i != biggest && i != bigger &&
			hashFieldUniques[i] > smallerUniques )
		{
			smallerUniques = hashFieldUniques[i];
			smaller = i;
		}
	}

	hashFields[0] = biggest;
	hashFields[1] = bigger;
	hashFields[2] = smaller;
}





/*
 *  FUNCTION
 *	imVfbHistAddValue	-  add value to histogram table
 *
 *  DESCRUPTION
 *	When histogramming more than 2 fields at a time, we build a hash
 *	table where each entry points to a collision list.  Two of the
 *	field values are used to index into the hash table.  The remaining
 *	one or two fields are found in the collision list.
 *
 *	imVfbHistAddValue( ) adds a new pair of values to the hash table
 *	and collision list.  If the hashed-to entry doesn't have a
 *	collision list yet, one is allocated and initialized.  The pair of
 *	values are then looked up in the collision list.  If they're found,
 *	the entry's occurrence count is incremented.  Otherwise the pair of
 *	values are put into a new collision list entry, possibly requiring
 *	a realloc to extend the collision list size to accomidate them.
 */

static int imVfbHistNColors;	/* Number of unique colors		*/

static int			/* Returns status			*/
#ifdef __STDC__
imVfbHistAddValue( imHistBucketHeader **hash, int index, int value1, int value2 )
#else
imVfbHistAddValue( hash, index, value1, value2 )
	imHistBucketHeader **hash;	/* Hash table to add to		*/
	int index;		/* Hash table index			*/
	int value1;		/* First value to add			*/
	int value2;		/* Second value to add			*/
#endif
{
	imHistBucketHeader *pHash;/* Pointer to hash table entry	*/
	int k;			/* Counter				*/


	/*
	 *  If there's no collision list there yet, allocate one.
	 */
	pHash = hash[ index ];
	if ( pHash == NULL )
	{
		/* Allocate a header.					*/
		if ( (hash[ index ] = pHash = (imHistBucketHeader *)malloc(
			sizeof( imHistBucketHeader ))) == NULL )
		{
			ImErrNo = IMEMALLOC;
			return ( -1 );
		}
		pHash->imhist_allocSize = IM_STARTSIZE;
		pHash->imhist_nextSlot  = 0;

		/* Allocate an initial collision list.			*/
		if ( (pHash->imhist_list = (imHistBucket *)malloc(
			sizeof( imHistBucket ) * IM_STARTSIZE )) == NULL )
		{
			ImErrNo = IMEMALLOC;
			return ( -1 );
		}
	}

	/*
	 *  Walk the collision list looking for this value.
	 */
	for ( k = 0; k < pHash->imhist_nextSlot; k++ )
	{
		if ( pHash->imhist_list[k].imhist_value1 == value1 &&
			pHash->imhist_list[k].imhist_value2 == value2 )
		{
			++pHash->imhist_list[k].imhist_nOccur;
			return ( 0 );
		}
	}

	/*
	 *  Not found in the collision list.  If there's no room left in
	 *  the list, increase the size.
	 */
	if ( pHash->imhist_nextSlot == pHash->imhist_allocSize )
	{
		pHash->imhist_allocSize += IM_SIZEINC;
		if ( (pHash->imhist_list = (imHistBucket *)realloc(
			(char *)pHash->imhist_list,
			sizeof( imHistBucket ) * pHash->imhist_allocSize )) == NULL )
		{
			ImErrNo = IMEMALLOC;
			return ( -1 );
		}
	}

	/*
	 *  Add a new collision list entry.
	 */
	pHash->imhist_list[k].imhist_value1 = value1;
	pHash->imhist_list[k].imhist_value2 = value2;
	pHash->imhist_list[k].imhist_nOccur = 1;
	pHash->imhist_nextSlot++;
	imVfbHistNColors++;
	return( 0 );
}





/*
 *  FUNCTION
 *	imVfbHistHashFree	-  free a hash table
 *
 *  DESCRIPTION
 *	The hash table and all its collision lists are deallocated.
 */

static void			/* Returns nothing			*/
#ifdef __STDC__
imVfbHistHashFree( imHistBucketHeader **hash, int hashSize )
#else
imVfbHistHashFree( hash, hashSize )
	imHistBucketHeader **hash;	/* Hash table to toss		*/
	int hashSize;		/* Size of hash table			*/
#endif
{
	int j;			/* Counter				*/

	for ( j = 0; j < hashSize; j++ )
	{
		if ( hash[ j ] == NULL )
			continue;
		if ( hash[j]->imhist_list )
			free( (char *)hash[j]->imhist_list );
		free( (char *)hash[j] );
	}
	free( (char *)hash );
}





/*
 *  SINGLE FIELD HISTOGRAMS
 */

/*
 *  MACRO
 *	IM_HIST1FIELD	-  build a function for single-field histograms
 *	IM_HIST1FIELDHSI	-  same deal but for HSI fields
 *
 *  DESCRIPTION
 *	Each of the histogram functions that compute for a single pixel
 *	field look nearly identical.  They could all be combined into a
 *	single function, but we'd have to embedd lots of loop-invariant
 *	if-statements to select which field to work on.  Instead, we
 *	split them into several nearly identical routines.
 *
 *	These single-field histogram routines are all automatically
 *	generated by the IM_HIST1FIELD macro.  Parameters to the macro
 *	select the function name to create, the field to compute on,
 *	the VFB query call to use to get that field value, the histogram
 *	table field to contain the value list, and the size of the hash
 *	table to work with.
 *
 *	IM_HIST1FIELD hashes the field value into a hash table then counts
 *	the number of unique field values.  A histogram data table is
 *	then allocated and those entries copied into it.
 *
 *	IM_HIST1FIELDHSI is nearly identical to IM_HIST1FIELD, but does the
 *	extra work needed for dealing with HSI virtual fields.
 */
#ifdef __STDC__
#define IM_VFBHEADER(fn)	fn( ImVfb * vfb)							
#else
#define IM_VFBHEADER(fn)	fn(vfb)	ImVfb *vfb;
#endif


#define IM_HIST1FIELD( funcName, field, query, tableField, hashSize )	\
ImHistTable *		/* Returns histogram table		*/\
IM_VFBHEADER(funcName)								\
{									\
        int     i, j;		/* Counters				*/\
        int     hash[hashSize];	/* Hash table counting unique pixels	*/\
        int     nEntry;		/* Number of unique pixels		*/\
        ImHistTable *histTable;/* Histogram table			*/\
									\
	/*  Hash the color index into an occurrence table.		*/\
	memset( (void *)hash, 0x00, sizeof( int ) * (hashSize) );	\
	IMVFBH_HASH( vfb, query, hash );				\
									\
	/*  Count the number of unique values.				*/\
	for ( nEntry = i = 0; i < hashSize; i++ )			\
		if ( hash[i] != 0 )					\
			++nEntry;					\
									\
	/*  Allocate the histogram table.				*/\
        if ( (histTable = imVfbHistTableAlloc( nEntry, field, 1 )) == NULL )\
	{								\
		/* ImErrNo already set.					*/\
		return( NULL );						\
	}								\
									\
	/*  Set the histogram table.					*/\
        for ( i = j = 0; i < (hashSize); i++ )				\
        {								\
                if ( hash[i] != 0 )					\
                {							\
			histTable->tableField[j] = i;			\
			histTable->imhist_nOccur[j++]    = hash[i];		\
                }							\
        }								\
	return ( histTable );						\
}

#define IM_HIST1FIELDHSI( funcName, field, which, factor, tableField, hashSize )\
ImHistTable *		/* Returns histogram table		*/\
IM_VFBHEADER(funcName)								\
{									\
        int     i, j;		/* Counters				*/\
        int     hash[hashSize];	/* Hash table counting unique pixels	*/\
        int     nEntry;		/* Number of unique pixels		*/\
        ImHistTable *histTable;/* Histogram table			*/\
									\
	/*  Hash the color index into an occurrence table.		*/\
	memset( (void *)hash, 0x00, sizeof( int ) * (hashSize) );	\
	IMVFBH_HASHHSI( vfb, which, factor, hash );			\
									\
	/*  Count the number of unique values.				*/\
	for ( nEntry = i = 0; i < hashSize; i++ )			\
		if ( hash[i] != 0 )					\
			++nEntry;					\
									\
	/*  Allocate the histogram table.				*/\
        if ( (histTable = imVfbHistTableAlloc( nEntry, field, 1 )) == NULL )\
	{								\
		/* ImErrNo already set.					*/\
		return( NULL );						\
	}								\
									\
	/*  Set the histogram table.					*/\
        for ( i = j = 0; i < (hashSize); i++ )				\
        {								\
                if ( hash[i] != 0 )					\
                {							\
			histTable->tableField[j] = (float)i/(factor);	\
			histTable->imhist_nOccur[j++]    = hash[i];		\
                }							\
        }								\
	return ( histTable );						\
}


IM_HIST1FIELD( imMHist,  IMMONO,    ImVfbQMono,    imhist_mono,    2 )
IM_HIST1FIELD( im8Hist,  IMINDEX8,  ImVfbQIndex8,  imhist_index8,  256 )
IM_HIST1FIELD( im16Hist, IMINDEX16, ImVfbQIndex16, imhist_index16, 65536 )

IM_HIST1FIELD( imRHist,  IMRED,     ImVfbQRed,     imhist_red,     256 )
IM_HIST1FIELD( imGHist,  IMGREEN,   ImVfbQGreen,   imhist_green,   256 )
IM_HIST1FIELD( imBHist,  IMBLUE,    ImVfbQBlue,    imhist_blue,    256 )
IM_HIST1FIELD( imAHist,  IMALPHA,   ImVfbQAlpha,   imhist_alpha,   256 )

IM_HIST1FIELDHSI( imHHist, IMHUE,        0, (1023.0/360.0),
	imhist_hue,        1024 )
IM_HIST1FIELDHSI( imSHist, IMSATURATION, 1, (1023.0),
	imhist_saturation, 1024 )
IM_HIST1FIELDHSI( imIHist, IMINTENSITY,  2, (1023.0),
	imhist_intensity,  1024 )





/*
 *  DOUBLE FIELD HISTOGRAMS
 */

/*
 *  MACRO
 *	IM_HIST2FIELD	-  build a function for double-field histograms
 *	IM_HIST2FIELD1HSI	-  same deal but for 1 HSI field
 *	IM_HIST2FIELD2HSI	-  same deal but for 2 HSI fields
 *
 *  DESCRIPTION
 *	As with IM_HIST1FIELD, all of the double-field histogram functions
 *	are automatically generated.
 *
 *	IM_HIST2FIELD hashes the field values into a hash table then counts
 *	the number of unique field values.  A histogram data table is
 *	then allocated and those entries copied into it.
 *
 *	IM_HIST1FIELD and IM_HIST2FIELD differ in that the former uses one
 *	pixel field in the hash function, and the later uses two.
 *
 *	IM_HIST2FIELD1HSI and IM_HIST2FIELD2HSI are nearly identical to IM_HIST2FIELD,
 *	but include the extra work needed for dealing with HSI virtual fields.
 */


#define IM_HIST2FIELD( funcName, fields, query1, query2, tableField1, tableField2, hashSize1, hashSize2, shift )	\
ImHistTable *		/* Returns histogram table		*/\
IM_VFBHEADER(funcName)								\
{									\
        int     i, j, k;	/* Counters				*/\
	int     h, w;		/* Height and width of image		*/\
	ImVfbPtr pPixel;	/* Pixel pointer			*/\
        int     hash[(hashSize1)*(hashSize2)];/* Table counting unique pixels*/\
        int     nEntry;		/* Number of unique pixels		*/\
        ImHistTable *histTable;/* Histogram table			*/\
	int     hashSize;	/* Hash table size			*/\
	int     index;		/* Hash table index			*/\
									\
	hashSize = (hashSize1) * (hashSize2);				\
									\
	/*  Hash the color index into an occurrence table.		*/\
	memset( (void *)hash, 0x00, sizeof( int ) * hashSize );		\
	pPixel = ImVfbQFirst( vfb );					\
	h      = ImVfbQHeight( vfb );					\
	w      = ImVfbQWidth( vfb );					\
	for ( i = 0; i < h; i++ )					\
	{								\
		for ( k = 0; k < w; k++ )				\
		{							\
			index = (query1( vfb, pPixel ) << (shift)) |	\
				query2( vfb, pPixel );			\
			hash[ index ]++;				\
			ImVfbSInc( vfb, pPixel );			\
		}							\
	}								\
								\
	/*  Count the number of unique values.				*/\
	for ( nEntry = i = 0; i < hashSize; i++ )			\
		if ( hash[i] != 0 )					\
			++nEntry;					\
									\
	/*  Allocate the histogram table.				*/\
        if ( (histTable = imVfbHistTableAlloc( nEntry, fields, 2 )) == NULL )\
	{								\
		/* ImErrNo already set.					*/\
		return( NULL );						\
	}								\
									\
	/*  Set the histogram table.					*/\
        for ( i = j = 0; i < (hashSize1); i++ )				\
        {								\
		for ( k = 0; k < (hashSize2); k++ )			\
		{							\
			index = (i << (shift)) | k;			\
			if ( hash[index] != 0 )				\
			{						\
				histTable->tableField1[j] = i;		\
				histTable->tableField2[j] = k;		\
				histTable->imhist_nOccur[j++]     = hash[index];\
			}						\
		}							\
        }								\
	return ( histTable );						\
}


#define IM_HIST2FIELD1HSI( funcName, fields, which1, factor1, query2, tableField1, tableField2, hashSize1, hashSize2, shift )	\
ImHistTable *		/* Returns histogram table		*/\
IM_VFBHEADER(funcName)								\
{									\
        int     i, j, k;	/* Counters				*/\
	int     h, w;		/* Height and width of image		*/\
	ImVfbPtr pPixel;	/* Pixel pointer			*/\
        int     hash[(hashSize1)*(hashSize2)];/* Table counting unique pixels*/\
        int     nEntry;		/* Number of unique pixels		*/\
        ImHistTable *histTable;/* Histogram table			*/\
	int     hashSize;	/* Hash table size			*/\
	int     index;		/* Hash table index			*/\
	int	rgb[3];		/* RGB triplet				*/\
	float	hsi[3];		/* HSI equivalent			*/\
									\
	hashSize = (hashSize1) * (hashSize2);				\
									\
	/*  Hash the color index into an occurrence table.		*/\
	memset( (void *)hash, 0x00,  sizeof( int ) * hashSize );	\
	pPixel = ImVfbQFirst( vfb );					\
	h      = ImVfbQHeight( vfb );					\
	w      = ImVfbQWidth( vfb );					\
	for ( i = 0; i < h; i++ )					\
	{								\
		for ( k = 0; k < w; k++ )				\
		{							\
			rgb[0] = ImVfbQRed(   vfb, pPixel );		\
			rgb[1] = ImVfbQGreen( vfb, pPixel );		\
			rgb[2] = ImVfbQBlue(  vfb, pPixel );		\
			ImRgbToHsi( rgb, hsi );				\
			index = ((int)(hsi[which1] * (factor1)) << (shift)) |\
				query2( vfb, pPixel );			\
			hash[ index ]++;				\
			ImVfbSInc( vfb, pPixel );			\
		}							\
	}								\
									\
	/*  Count the number of unique values.				*/\
	for ( nEntry = i = 0; i < hashSize; i++ )			\
		if ( hash[i] != 0 )					\
			++nEntry;					\
									\
	/*  Allocate the histogram table.				*/\
        if ( (histTable = imVfbHistTableAlloc( nEntry, fields, 2 )) == NULL )\
	{								\
		/* ImErrNo already set.					*/\
		return( NULL );						\
	}								\
									\
	/*  Set the histogram table.					*/\
        for ( i = j = 0; i < (hashSize1); i++ )				\
        {								\
		for ( k = 0; k < (hashSize2); k++ )			\
		{							\
			index = (i << (shift)) | k;			\
			if ( hash[index] != 0 )				\
			{						\
				histTable->tableField1[j] = (float)i/(factor1);\
				histTable->tableField2[j] = k;		\
				histTable->imhist_nOccur[j++]     = hash[index];\
			}						\
		}							\
        }								\
	return ( histTable );						\
}


#define IM_HIST2FIELD2HSI( funcName, fields, which1, factor1, which2, factor2, tableField1, tableField2, hashSize1, hashSize2, shift )	\
ImHistTable *		/* Returns histogram table		*/\
IM_VFBHEADER(funcName)								\
{									\
        int     i, j, k;	/* Counters				*/\
	int     h, w;		/* Height and width of image		*/\
	ImVfbPtr pPixel;	/* Pixel pointer			*/\
        int     hash[(hashSize1)*(hashSize2)];/* Table counting unique pixels*/\
        int     nEntry;		/* Number of unique pixels		*/\
        ImHistTable *histTable;/* Histogram table			*/\
	int     hashSize;	/* Hash table size			*/\
	int     index;		/* Hash table index			*/\
	int	rgb[3];		/* RGB triplet				*/\
	float	hsi[3];		/* HSI equivalent			*/\
									\
	hashSize = (hashSize1) * (hashSize2);				\
									\
	/*  Hash the color index into an occurrence table.		*/\
	memset( (void *)hash, 0x00, sizeof( int ) * hashSize );		\
	pPixel = ImVfbQFirst( vfb );					\
	h      = ImVfbQHeight( vfb );					\
	w      = ImVfbQWidth( vfb );					\
	for ( i = 0; i < h; i++ )					\
	{								\
		for ( k = 0; k < w; k++ )				\
		{							\
			rgb[0] = ImVfbQRed(   vfb, pPixel );		\
			rgb[1] = ImVfbQGreen( vfb, pPixel );		\
			rgb[2] = ImVfbQBlue(  vfb, pPixel );		\
			ImRgbToHsi( rgb, hsi );				\
			index = ((int)(hsi[which1] * (factor1)) << (shift)) |\
				(int)(hsi[which2] * (factor2));		\
			hash[ index ]++;				\
			ImVfbSInc( vfb, pPixel );			\
		}							\
	}								\
									\
	/*  Count the number of unique values.				*/\
	for ( nEntry = i = 0; i < hashSize; i++ )			\
		if ( hash[i] != 0 )					\
			++nEntry;					\
									\
	/*  Allocate the histogram table.				*/\
        if ( (histTable = imVfbHistTableAlloc( nEntry, fields, 2 )) == NULL )\
	{								\
		/* ImErrNo already set.					*/\
		return( NULL );						\
	}								\
									\
	/*  Set the histogram table.					*/\
        for ( i = j = 0; i < (hashSize1); i++ )				\
        {								\
		for ( k = 0; k < (hashSize2); k++ )			\
		{							\
			index = (i << (shift)) | k;			\
			if ( hash[index] != 0 )				\
			{						\
				histTable->tableField1[j] = (float)i/(factor1);\
				histTable->tableField2[j] = (float)k/(factor2);\
				histTable->imhist_nOccur[j++]     = hash[index];\
			}						\
		}							\
        }								\
	return ( histTable );						\
}

/*
 *  All possible legal combinations of 2 fields:
 *	( different orderings of the first 2 fields are equivalent )
 *
 *	R G
 *	R B
 *	R A
 *
 *	G R	(same as R G)
 *	G B
 *	G A
 *
 *	B R	(same as R B)
 *	B G	(same as G B)
 *	B A
 *
 *	M A
 *	8 A
 *	16 A
 *
 *	H S
 *	H I
 *	H A
 *
 *	S H	(same as H S)
 *	S I
 *	S A
 *
 *	I H	(same as H I)
 *	I S	(same as S I)
 *	I A
 */

IM_HIST2FIELD( imRGHist, IMRED|IMGREEN,
	ImVfbQRed,	ImVfbQGreen,
	imhist_red,	imhist_green,
	256, 256, 8 )
IM_HIST2FIELD( imRBHist, IMRED|IMBLUE,
	ImVfbQRed,	ImVfbQBlue,
	imhist_red,	imhist_blue,
	256, 256, 8 )
IM_HIST2FIELD( imRAHist, IMRED|IMALPHA,
	ImVfbQRed,	ImVfbQAlpha,
	imhist_red,	imhist_alpha,
	256, 256, 8 )

IM_HIST2FIELD( imGAHist, IMGREEN|IMALPHA,
	ImVfbQGreen,	ImVfbQAlpha,
	imhist_green,	imhist_alpha,
	256, 256, 8 )
IM_HIST2FIELD( imGBHist, IMGREEN|IMBLUE,
	ImVfbQGreen,	ImVfbQBlue,
	imhist_green,	imhist_blue,
	256, 256, 8 )

IM_HIST2FIELD( imBAHist, IMBLUE|IMALPHA,
	ImVfbQBlue,	ImVfbQAlpha,
	imhist_blue,	imhist_alpha,
	256, 256, 8 )


IM_HIST2FIELD( imMAHist, IMMONO|IMALPHA,
	ImVfbQMono,	ImVfbQAlpha,
	imhist_mono,	imhist_alpha,
	2, 256, 8 )
IM_HIST2FIELD( im8AHist, IMINDEX8|IMALPHA,
	ImVfbQIndex8,	ImVfbQAlpha,
	imhist_index8,	imhist_alpha,
	256, 256, 8 )
IM_HIST2FIELD( im16AHist, IMINDEX16|IMALPHA,
	ImVfbQIndex16,	ImVfbQAlpha,
	imhist_index16,	imhist_alpha,
	65536, 256, 8 )


IM_HIST2FIELD2HSI( imHSHist, IMHUE|IMSATURATION,
	0, (1023.0/360.0),	1, 1023.0,
	imhist_hue,		imhist_saturation,
	1024, 1024, 10 )
IM_HIST2FIELD2HSI( imHIHist, IMHUE|IMINTENSITY,
	0, (1023.0/360.0),	2, 1023.0,
	imhist_hue,		imhist_intensity,
	1024, 1024, 10 )
IM_HIST2FIELD1HSI( imHAHist, IMHUE|IMALPHA,
	0, (1023.0/360.0),	ImVfbQAlpha,
	imhist_hue,		imhist_alpha,
	1024, 256, 8 )

IM_HIST2FIELD2HSI( imSIHist, IMSATURATION|IMINTENSITY,
	1, 1023.0,		2, (1023.0),
	imhist_saturation,	imhist_intensity,
	1024, 1024, 10 )
IM_HIST2FIELD1HSI( imSAHist, IMSATURATION|IMALPHA,
	1, 1023.0,		ImVfbQAlpha,
	imhist_saturation,	imhist_alpha,
	1024, 256, 8 )

IM_HIST2FIELD1HSI( imIAHist, IMINTENSITY|IMALPHA,
	2, 1023.0,		ImVfbQAlpha,
	imhist_intensity,	imhist_alpha,
	1024, 256, 8 )





/*
 *  TRIPLE FIELD HISTOGRAMS
 */

/*
 *  MACRO
 *	IM_HIST3FIELD	-  build a function for triple-field histograms
 *	IM_HIST3FIELD1HSI	-  same deal but for 1 HSI field
 *	IM_HIST3FIELD2HSI	-  same deal but for 2 HSI fields
 *	IM_HIST3FIELD3HSI	-  same deal but for 2 HSI fields
 *
 *  DESCRIPTION
 *	As with IM_HIST1FIELD, all of the triple-field histogram functions
 *	are automatically generated.
 *
 *	IM_HIST3FIELD hashes two field values together, then follows a
 *	collision chain along until it finds the third field's value, or
 *	reaches the end.  In the first case it just increments the occurrence
 *	count.  In the second case it adds a new entry to the collision list
 *	and sets the occurrence count to 1.
 *
 *	After hasing is complete, IM_HIST3FIELD functions allocate a histogram
 *	data table and copy the data into it.
 */


#define IM_HIST3FIELD( funcName, fields, query1, query2, query3, tableField1, tableField2, tableField3, hashSize1, hashSize2, shift )	\
ImHistTable *		/* Returns histogram table		*/\
IM_VFBHEADER(funcName)								\
{									\
        int     i, j, k;	/* Counters				*/\
	int     h, w;		/* Height and width of image		*/\
	ImVfbPtr pPixel;	/* Pixel pointer			*/\
	imHistBucketHeader **hash;/* Hash table				*/\
	imHistBucketHeader *pHash;/* Hash table pointer			*/\
        int     nEntry;		/* Number of unique pixels		*/\
        ImHistTable *histTable;/* Histogram table			*/\
	int     hashSize;	/* Hash table size			*/\
	int     index;		/* Hash table index			*/\
									\
	/*  Allocate the hash table as a table of collision lists.	*/\
	hashSize = (hashSize1) * (hashSize2);				\
	if ( (hash = (imHistBucketHeader **)malloc(			\
		sizeof( imHistBucketHeader * ) * hashSize )) == NULL )	\
	{								\
		ImErrNo = IMEMALLOC;					\
		return ( NULL );					\
	}								\
	memset( (void *)hash, 0x00, sizeof( imHistBucketHeader * ) * hashSize );\
									\
	/*  Hash the color index into an occurrence table.		*/\
	pPixel = ImVfbQFirst( vfb );					\
	h      = ImVfbQHeight( vfb );					\
	w      = ImVfbQWidth( vfb );					\
	imVfbHistNColors = 0;						\
	for ( i = 0; i < h; i++ )					\
	{								\
		for ( k = 0; k < w; k++ )				\
		{							\
			index = (query1( vfb, pPixel ) << (shift)) |	\
				query2( vfb, pPixel );			\
			if ( imVfbHistAddValue( hash, index, query3( vfb, pPixel ), 0 ) == -1 )\
			{						\
				/* ImErrNo already set.			*/\
				imVfbHistHashFree( hash, hashSize );	\
				return ( NULL );			\
			}						\
			ImVfbSInc( vfb, pPixel );			\
		}							\
	}								\
									\
	/*  Allocate the histogram table.				*/\
        if ( (histTable = imVfbHistTableAlloc( imVfbHistNColors, fields, 3 )) == NULL )\
	{								\
		/* ImErrNo already set.					*/\
		return( NULL );						\
	}								\
									\
	/*  Set the histogram table.					*/\
        for ( nEntry = i = 0; i < (hashSize1); i++ )			\
        {								\
		for ( k = 0; k < (hashSize2); k++ )			\
		{							\
			index = (i << (shift)) | k;			\
			pHash = hash[ index ];				\
			if ( pHash == NULL )				\
				continue;				\
			for ( j = 0; j < pHash->imhist_nextSlot; j++ )	\
			{						\
				histTable->tableField1[nEntry] = i;	\
				histTable->tableField2[nEntry] = k;	\
				histTable->tableField3[nEntry] = pHash->imhist_list[j].imhist_value1;\
				histTable->imhist_nOccur[nEntry++]     = pHash->imhist_list[j].imhist_nOccur;\
			}						\
			/* Toss the collision list.			*/\
			free( (char *)pHash->imhist_list );		\
			free( (char *)pHash );				\
		}							\
        }								\
	free( (char *)hash );						\
	return ( histTable );						\
}


#define IM_HIST3FIELD1HSI( funcName, fields, which1, factor1, query2, which3, factor3, tableField1, tableField2, tableField3, hashSize1, hashSize2, shift )	\
ImHistTable *		/* Returns histogram table		*/\
IM_VFBHEADER(funcName)								\
{									\
        int     i, j, k;	/* Counters				*/\
	int     h, w;		/* Height and width of image		*/\
	ImVfbPtr pPixel;	/* Pixel pointer			*/\
	imHistBucketHeader **hash;/* Hash table				*/\
	imHistBucketHeader *pHash;/* Hash table pointer			*/\
        int     nEntry;		/* Number of unique pixels		*/\
        ImHistTable *histTable;/* Histogram table			*/\
	int     hashSize;	/* Hash table size			*/\
	int     index;		/* Hash table index			*/\
	int	rgb[3];		/* RGB triplet				*/\
	float	hsi[3];		/* HSI equivalent			*/\
									\
	/*  Allocate the hash table as a table of collision lists.	*/\
	hashSize = (hashSize1) * (hashSize2);				\
	if ( (hash = (imHistBucketHeader **)malloc(			\
		sizeof( imHistBucketHeader * ) * hashSize )) == NULL )	\
	{								\
		ImErrNo = IMEMALLOC;					\
		return ( NULL );					\
	}								\
	memset( (void *)hash, 0x00, sizeof( imHistBucketHeader * ) * hashSize );\
									\
	/*  Hash the color index into an occurrence table.		*/\
	pPixel = ImVfbQFirst( vfb );					\
	h      = ImVfbQHeight( vfb );					\
	w      = ImVfbQWidth( vfb );					\
	imVfbHistNColors = 0;						\
	for ( i = 0; i < h; i++ )					\
	{								\
		for ( k = 0; k < w; k++ )				\
		{							\
			rgb[0] = ImVfbQRed(   vfb, pPixel );		\
			rgb[1] = ImVfbQGreen( vfb, pPixel );		\
			rgb[2] = ImVfbQBlue(  vfb, pPixel );		\
			ImRgbToHsi( rgb, hsi );				\
			index = ((int)(hsi[which1] * (factor1)) << (shift)) |\
				query2( vfb, pPixel );			\
			if ( imVfbHistAddValue( hash, index, (int)(hsi[which3] * (factor3)), 0 ) == -1 )\
			{						\
				/* ImErrNo already set.			*/\
				imVfbHistHashFree( hash, hashSize );	\
				return ( NULL );			\
			}						\
			ImVfbSInc( vfb, pPixel );			\
		}							\
	}								\
									\
	/*  Allocate the histogram table.				*/\
        if ( (histTable = imVfbHistTableAlloc( imVfbHistNColors, fields, 3 )) == NULL )\
	{								\
		/* ImErrNo already set.					*/\
		return( NULL );						\
	}								\
									\
	/*  Set the histogram table.					*/\
        for ( nEntry = i = 0; i < (hashSize1); i++ )			\
        {								\
		for ( k = 0; k < (hashSize2); k++ )			\
		{							\
			index = (i << (shift)) | k;			\
			pHash = hash[ index ];				\
			if ( pHash == NULL )				\
				continue;				\
			for ( j = 0; j < pHash->imhist_nextSlot; j++ )	\
			{						\
				histTable->tableField1[nEntry] = (float)i/(factor1);\
				histTable->tableField2[nEntry] = k;\
				histTable->tableField3[nEntry] = (float)(pHash->imhist_list[j].imhist_value1)/(factor3);\
				histTable->imhist_nOccur[nEntry++]     = pHash->imhist_list[j].imhist_nOccur;\
			}						\
			/* Toss the collision list.			*/\
			free( (char *)pHash->imhist_list );		\
			free( (char *)pHash );				\
		}							\
        }								\
	free( (char *)hash );						\
	return ( histTable );						\
}


#define IM_HIST3FIELD2HSI( funcName, fields, which1, factor1, which2, factor2, query3, tableField1, tableField2, tableField3, hashSize1, hashSize2, shift )	\
ImHistTable *		/* Returns histogram table		*/\
IM_VFBHEADER(funcName)								\
{									\
        int     i, j, k;	/* Counters				*/\
	int     h, w;		/* Height and width of image		*/\
	ImVfbPtr pPixel;	/* Pixel pointer			*/\
	imHistBucketHeader **hash;/* Hash table				*/\
	imHistBucketHeader *pHash;/* Hash table pointer			*/\
        int     nEntry;		/* Number of unique pixels		*/\
        ImHistTable *histTable;/* Histogram table			*/\
	int     hashSize;	/* Hash table size			*/\
	int     index;		/* Hash table index			*/\
	int	rgb[3];		/* RGB triplet				*/\
	float	hsi[3];		/* HSI equivalent			*/\
									\
	/*  Allocate the hash table as a table of collision lists.	*/\
	hashSize = (hashSize1) * (hashSize2);				\
	if ( (hash = (imHistBucketHeader **)malloc(			\
		sizeof( imHistBucketHeader * ) * hashSize )) == NULL )	\
	{								\
		ImErrNo = IMEMALLOC;					\
		return ( NULL );					\
	}								\
	memset( (void *)hash, 0x00, sizeof( imHistBucketHeader * ) * hashSize );\
									\
	/*  Hash the color index into an occurrence table.		*/\
	pPixel = ImVfbQFirst( vfb );					\
	h      = ImVfbQHeight( vfb );					\
	w      = ImVfbQWidth( vfb );					\
	imVfbHistNColors = 0;						\
	for ( i = 0; i < h; i++ )					\
	{								\
		for ( k = 0; k < w; k++ )				\
		{							\
			rgb[0] = ImVfbQRed(   vfb, pPixel );		\
			rgb[1] = ImVfbQGreen( vfb, pPixel );		\
			rgb[2] = ImVfbQBlue(  vfb, pPixel );		\
			ImRgbToHsi( rgb, hsi );				\
			index = ((int)(hsi[which1] * (factor1)) << (shift)) |\
				(int)(hsi[which2] * (factor2));		\
			if ( imVfbHistAddValue( hash, index, query3( vfb, pPixel ), 0 ) == -1 )\
			{						\
				/* ImErrNo already set.			*/\
				imVfbHistHashFree( hash, hashSize );	\
				return ( NULL );			\
			}						\
			ImVfbSInc( vfb, pPixel );			\
		}							\
	}								\
									\
	/*  Allocate the histogram table.				*/\
        if ( (histTable = imVfbHistTableAlloc( imVfbHistNColors, fields, 3 )) == NULL )\
	{								\
		/* ImErrNo already set.					*/\
		return( NULL );						\
	}								\
									\
	/*  Set the histogram table.					*/\
        for ( nEntry = i = 0; i < (hashSize1); i++ )			\
        {								\
		for ( k = 0; k < (hashSize2); k++ )			\
		{							\
			index = (i << (shift)) | k;			\
			pHash = hash[ index ];				\
			if ( pHash == NULL )				\
				continue;				\
			for ( j = 0; j < pHash->imhist_nextSlot; j++ )	\
			{						\
				histTable->tableField1[nEntry] = (float)i/(factor1);\
				histTable->tableField2[nEntry] = (float)k/(factor2);\
				histTable->tableField3[nEntry] = pHash->imhist_list[j].imhist_value1;\
				histTable->imhist_nOccur[nEntry++]     = pHash->imhist_list[j].imhist_nOccur;\
			}						\
			/* Toss the collision list.			*/\
			free( (char *)pHash->imhist_list );		\
			free( (char *)pHash );				\
		}							\
        }								\
	free( (char *)hash );						\
	return ( histTable );						\
}


#define IM_HIST3FIELD3HSI( funcName, fields, which1, factor1, which2, factor2, which3, factor3, tableField1, tableField2, tableField3, hashSize1, hashSize2, shift )	\
ImHistTable *		/* Returns histogram table		*/\
IM_VFBHEADER(funcName)								\
{									\
        int     i, j, k;	/* Counters				*/\
	int     h, w;		/* Height and width of image		*/\
	ImVfbPtr pPixel;	/* Pixel pointer			*/\
	imHistBucketHeader **hash;/* Hash table				*/\
	imHistBucketHeader *pHash;/* Hash table pointer			*/\
        int     nEntry;		/* Number of unique pixels		*/\
        ImHistTable *histTable;/* Histogram table			*/\
	int     hashSize;	/* Hash table size			*/\
	int     index;		/* Hash table index			*/\
	int	rgb[3];		/* RGB triplet				*/\
	float	hsi[3];		/* HSI equivalent			*/\
									\
	/*  Allocate the hash table as a table of collision lists.	*/\
	hashSize = (hashSize1) * (hashSize2);				\
	if ( (hash = (imHistBucketHeader **)malloc(			\
		sizeof( imHistBucketHeader * ) * hashSize )) == NULL )	\
	{								\
		ImErrNo = IMEMALLOC;					\
		return ( NULL );					\
	}								\
	memset( (void *)hash, 0x00, sizeof( imHistBucketHeader * ) * hashSize );\
									\
	/*  Hash the color index into an occurrence table.		*/\
	pPixel = ImVfbQFirst( vfb );					\
	h      = ImVfbQHeight( vfb );					\
	w      = ImVfbQWidth( vfb );					\
	imVfbHistNColors = 0;						\
	for ( i = 0; i < h; i++ )					\
	{								\
		for ( k = 0; k < w; k++ )				\
		{							\
			rgb[0] = ImVfbQRed(   vfb, pPixel );		\
			rgb[1] = ImVfbQGreen( vfb, pPixel );		\
			rgb[2] = ImVfbQBlue(  vfb, pPixel );		\
			ImRgbToHsi( rgb, hsi );				\
			index = ((int)(hsi[which1] * (factor1)) << (shift)) |\
				(int)(hsi[which2] * (factor2));		\
			if ( imVfbHistAddValue( hash, index, (int)(hsi[which3] * (factor3)), 0 ) == -1 )\
			{						\
				/* ImErrNo already set.			*/\
				imVfbHistHashFree( hash, hashSize );	\
				return ( NULL );			\
			}						\
			ImVfbSInc( vfb, pPixel );			\
		}							\
	}								\
									\
	/*  Allocate the histogram table.				*/\
        if ( (histTable = imVfbHistTableAlloc( imVfbHistNColors, fields, 3 )) == NULL )\
	{								\
		/* ImErrNo already set.					*/\
		return( NULL );						\
	}								\
									\
	/*  Set the histogram table.					*/\
        for ( nEntry = i = 0; i < (hashSize1); i++ )			\
        {								\
		for ( k = 0; k < (hashSize2); k++ )			\
		{							\
			index = (i << (shift)) | k;			\
			pHash = hash[ index ];				\
			if ( pHash == NULL )				\
				continue;				\
			for ( j = 0; j < pHash->imhist_nextSlot; j++ )	\
			{						\
				histTable->tableField1[nEntry] = (float)i/(factor1);\
				histTable->tableField2[nEntry] = (float)k/(factor2);\
				histTable->tableField3[nEntry] = (float)pHash->imhist_list[j].imhist_value1/(factor3);\
				histTable->imhist_nOccur[nEntry++]     = pHash->imhist_list[j].imhist_nOccur;\
			}						\
			/* Toss the collision list.			*/\
			free( (char *)pHash->imhist_list );		\
			free( (char *)pHash );				\
		}							\
        }								\
	free( (char *)hash );						\
	return ( histTable );						\
}

/*
 *  All possible legal combinations of 3 fields:
 *	( different orderings of the first 2 fields are equivalent )
 *
 *	R G B
 *	R G A
 *	R B G
 *	R B A
 *	R A B
 *	R A G
 *
 *	B R G	(same as R B G)
 *	B R A	(same as R B A)
 *	B G R
 *	B G A
 *	B A R
 *	B A G
 *
 *	G B R	(same as B G R)
 *	G B A	(same as B G A)
 *	G R B	(same as R G B)
 *	G R A	(same as R G A)
 *	G A R
 *	G A B
 *
 *	A R B	(same as R A B)
 *	A R G	(same as R A G)
 *	A B R	(same as B A R)
 *	A B G	(same as B A G)
 *	A G R	(same as G A R)
 *	A G B	(same as G A B)
 *
 *	H S I
 *	H S A
 *	H I S
 *	H I A
 *	H A S
 *	H A I
 *
 *	S H I	(same as H S I)
 *	S H A	(same as H S A)
 *	S I H
 *	S I A
 *	S A H
 *	S A I
 *
 *	I H S	(same as H I S)
 *	I H A	(same as H I A)
 *	I S H	(same as S I H)
 *	I S A	(same as S I A)
 *	I A H
 *	I A S
 *
 *	A H S	(same as H A S)
 *	A H I	(same as H A I)
 *	A S H	(same as S A H)
 *	A S I	(same as S A I)
 *	A I H	(same as I A H)
 *	A I S	(same as I A S)
 */

IM_HIST3FIELD( imRGBHist, IMRED|IMGREEN|IMBLUE,
	ImVfbQRed,	ImVfbQGreen,	ImVfbQBlue,
	imhist_red,	imhist_green,	imhist_blue,
	256, 256, 8 )
IM_HIST3FIELD( imRGAHist, IMRED|IMGREEN|IMALPHA,
	ImVfbQRed,	ImVfbQGreen,	ImVfbQAlpha,
	imhist_red,	imhist_green,	imhist_alpha,
	256, 256, 8 )
IM_HIST3FIELD( imRBGHist, IMRED|IMBLUE|IMGREEN,
	ImVfbQRed,	ImVfbQBlue,	ImVfbQGreen,
	imhist_red,	imhist_blue,	imhist_green,
	256, 256, 8 )
IM_HIST3FIELD( imRBAHist, IMRED|IMBLUE|IMALPHA,
	ImVfbQRed,	ImVfbQBlue,	ImVfbQAlpha,
	imhist_red,	imhist_blue,	imhist_alpha,
	256, 256, 8 )
IM_HIST3FIELD( imRABHist, IMRED|IMALPHA|IMBLUE,
	ImVfbQRed,	ImVfbQAlpha,	ImVfbQBlue,
	imhist_red,	imhist_alpha,	imhist_blue,
	256, 256, 8 )
IM_HIST3FIELD( imRAGHist, IMRED|IMALPHA|IMGREEN,
	ImVfbQRed,	ImVfbQAlpha,	ImVfbQGreen,
	imhist_red,	imhist_alpha,	imhist_green,
	256, 256, 8 )

IM_HIST3FIELD( imBGRHist, IMBLUE|IMGREEN|IMRED,
	ImVfbQBlue,	ImVfbQGreen,	ImVfbQRed,
	imhist_blue,	imhist_green,	imhist_red,
	256, 256, 8 )
IM_HIST3FIELD( imBGAHist, IMBLUE|IMGREEN|IMALPHA,
	ImVfbQBlue,	ImVfbQGreen,	ImVfbQAlpha,
	imhist_blue,	imhist_green,	imhist_alpha,
	256, 256, 8 )
IM_HIST3FIELD( imBARHist, IMBLUE|IMALPHA|IMRED,
	ImVfbQBlue,	ImVfbQAlpha,	ImVfbQRed,
	imhist_blue,	imhist_alpha,	imhist_red,
	256, 256, 8 )
IM_HIST3FIELD( imBAGHist, IMBLUE|IMALPHA|IMGREEN,
	ImVfbQBlue,	ImVfbQAlpha,	ImVfbQGreen,
	imhist_blue,	imhist_alpha,	imhist_green,
	256, 256, 8 )

IM_HIST3FIELD( imGARHist, IMGREEN|IMALPHA|IMRED,
	ImVfbQGreen,	ImVfbQAlpha,	ImVfbQRed,
	imhist_green,	imhist_alpha,	imhist_red,
	256, 256, 8 )
IM_HIST3FIELD( imGABHist, IMGREEN|IMALPHA|IMBLUE,
	ImVfbQGreen,	ImVfbQAlpha,	ImVfbQBlue,
	imhist_green,	imhist_alpha,	imhist_blue,
	256, 256, 8 )

IM_HIST3FIELD3HSI( imHSIHist, IMHUE|IMSATURATION|IMINTENSITY,
	0, (1023.0/360.0),	1, 1023.0,		2, 1023.0,
	imhist_hue,		imhist_saturation,	imhist_intensity,
	1024, 1024, 10 )
IM_HIST3FIELD2HSI( imHSAHist, IMHUE|IMSATURATION|IMALPHA,
	0, (1023.0/360.0),	1, 1023.0,		ImVfbQAlpha,
	imhist_hue,		imhist_saturation,	imhist_alpha,
	1024, 1024, 10 )
IM_HIST3FIELD3HSI( imHISHist, IMHUE|IMINTENSITY|IMSATURATION,
	0, (1023.0/360.0),	2, 1023.0,		1, 1023.0,
	imhist_hue,		imhist_intensity,	imhist_saturation,
	1024, 1024, 10 )
IM_HIST3FIELD2HSI( imHIAHist, IMHUE|IMINTENSITY|IMALPHA,
	0, (1023.0/360.0),	2, 1023.0,		ImVfbQAlpha,
	imhist_hue,		imhist_intensity,	imhist_alpha,
	1024, 1024, 10 )
IM_HIST3FIELD1HSI( imHASHist, IMHUE|IMALPHA|IMSATURATION,
	0, (1023.0/360.0),	ImVfbQAlpha,		1, 1023.0,
	imhist_hue,		imhist_alpha,		imhist_saturation,
	1024, 256, 8 )
IM_HIST3FIELD1HSI( imHAIHist, IMHUE|IMALPHA|IMINTENSITY,
	0, (1023.0/360.0),	ImVfbQAlpha,		2, 1023.0,
	imhist_hue,		imhist_alpha,		imhist_intensity,
	1024, 256, 8 )

IM_HIST3FIELD3HSI( imSIHHist, IMSATURATION|IMINTENSITY|IMHUE,
	1, 1023.0,		2, 1023.0,		0, (1023.0/360.0),
	imhist_saturation,	imhist_intensity,	imhist_hue,
	1024, 1024, 10 )
IM_HIST3FIELD2HSI( imSIAHist, IMSATURATION|IMINTENSITY|IMALPHA,
	1, 1023.0,		2, 1023.0,		ImVfbQAlpha,
	imhist_saturation,	imhist_intensity,	imhist_alpha,
	1024, 1024, 10 )
IM_HIST3FIELD1HSI( imSAHHist, IMSATURATION|IMALPHA|IMHUE,
	1, 1023.0,		ImVfbQAlpha,		0, (1023.0/360.0),
	imhist_saturation,	imhist_alpha,		imhist_hue,
	1024, 256, 8 )
IM_HIST3FIELD1HSI( imSAIHist, IMSATURATION|IMALPHA|IMINTENSITY,
	1, 1023.0,		ImVfbQAlpha,		2, 1023.0,
	imhist_saturation,	imhist_alpha,		imhist_intensity,
	1024, 256, 8 )

IM_HIST3FIELD1HSI( imIAHHist, IMINTENSITY|IMALPHA|IMHUE,
	2, 1023.0,		ImVfbQAlpha,		0, (1023.0/360.0),
	imhist_intensity,	imhist_alpha,		imhist_hue,
	1024, 256, 8 )
IM_HIST3FIELD1HSI( imIASHist, IMINTENSITY|IMALPHA|IMSATURATION,
	2, 1023.0,		ImVfbQAlpha,		1, 1023.0,
	imhist_intensity,	imhist_alpha,		imhist_saturation,
	1024, 256, 8 )





/*
 *  QUADRUPLE FIELD HISTOGRAMS
 */

/*
 *  MACRO
 *	IM_HIST4FIELD	-  build a function for quadruple-field histograms
 *	IM_HIST4FIELD3HSI	-  same deal but for 3 HSI fields
 *	IM_HIST4FIELD3HSI2	-  same deal but for 3 HSI fields, different alpha loc
 *
 *  DESCRIPTION
 *	As with IM_HIST1FIELD, all of the quadruple-field histogram functions
 *	are automatically generated.
 *
 *	IM_HIST4FIELD hashes two field values together, then follows a
 *	collision chain along until it finds the third and fourth field values,
 *	or reaches the end.  In the first case it just increments the occurrence
 *	count.  In the second case it adds a new entry to the collision list
 *	and sets the occurrence count to 1.
 *
 *	After hasing is complete, IM_HIST4FIELD functions allocate a histogram
 *	data table and copy the data into it.
 */


#define IM_HIST4FIELD( funcName, fields, query1, query2, query3, query4, tableField1, tableField2, tableField3, tableField4, hashSize1, hashSize2, shift )	\
ImHistTable *		/* Returns histogram table		*/\
IM_VFBHEADER(funcName)								\
{									\
        int     i, j, k;	/* Counters				*/\
	int     h, w;		/* Height and width of image		*/\
	ImVfbPtr pPixel;	/* Pixel pointer			*/\
	imHistBucketHeader **hash;/* Hash table				*/\
	imHistBucketHeader *pHash;/* Hash table pointer			*/\
        int     nEntry;		/* Number of unique pixels		*/\
        ImHistTable *histTable;/* Histogram table			*/\
	int     hashSize;	/* Hash table size			*/\
	int     index;		/* Hash table index			*/\
									\
	/*  Allocate the hash table as a table of collision lists.	*/\
	hashSize = (hashSize1) * (hashSize2);				\
	if ( (hash = (imHistBucketHeader **)malloc(			\
		sizeof( imHistBucketHeader * ) * hashSize )) == NULL )	\
	{								\
		ImErrNo = IMEMALLOC;					\
		return ( NULL );					\
	}								\
	memset( (void *)hash, 0x00, sizeof( imHistBucketHeader * ) * hashSize );\
								\
	/*  Hash the color index into an occurrence table.		*/\
	pPixel = ImVfbQFirst( vfb );					\
	h      = ImVfbQHeight( vfb );					\
	w      = ImVfbQWidth( vfb );					\
	imVfbHistNColors = 0;						\
	for ( i = 0; i < h; i++ )					\
	{								\
		for ( k = 0; k < w; k++ )				\
		{							\
			index = (query1( vfb, pPixel ) << (shift)) |	\
				query2( vfb, pPixel );			\
			if ( imVfbHistAddValue( hash, index, query3( vfb, pPixel ), query4( vfb, pPixel ) ) == -1 )\
			{						\
				/* ImErrNo already set.			*/\
				imVfbHistHashFree( hash, hashSize );	\
				return ( NULL );			\
			}						\
			ImVfbSInc( vfb, pPixel );			\
		}							\
	}								\
									\
	/*  Allocate the histogram table.				*/\
        if ( (histTable = imVfbHistTableAlloc( imVfbHistNColors, fields, 3 )) == NULL )\
	{								\
		/* ImErrNo already set.					*/\
		return( NULL );						\
	}								\
									\
	/*  Set the histogram table.					*/\
        for ( nEntry = i = 0; i < (hashSize1); i++ )			\
        {								\
		for ( k = 0; k < (hashSize2); k++ )			\
		{							\
			index = (i << (shift)) | k;			\
			pHash = hash[ index ];				\
			if ( pHash == NULL )				\
				continue;				\
			for ( j = 0; j < pHash->imhist_nextSlot; j++ )	\
			{						\
				histTable->tableField1[nEntry] = i;	\
				histTable->tableField2[nEntry] = k;	\
				histTable->tableField3[nEntry] = pHash->imhist_list[j].imhist_value1;\
				histTable->tableField4[nEntry] = pHash->imhist_list[j].imhist_value2;\
				histTable->imhist_nOccur[nEntry++]     = pHash->imhist_list[j].imhist_nOccur;\
			}						\
			/* Toss the collision list.			*/\
			free( (char *)pHash->imhist_list );		\
			free( (char *)pHash );				\
		}							\
        }								\
	free( (char *)hash );						\
	return ( histTable );						\
}


#define IM_HIST4FIELD3HSI( funcName, fields, which1, factor1, query2, which3, factor3, which4, factor4, tableField1, tableField2, tableField3, tableField4, hashSize1, hashSize2, shift )	\
ImHistTable *		/* Returns histogram table		*/\
IM_VFBHEADER(funcName)								\
{									\
        int     i, j, k;	/* Counters				*/\
	int     h, w;		/* Height and width of image		*/\
	ImVfbPtr pPixel;	/* Pixel pointer			*/\
	imHistBucketHeader **hash;/* Hash table				*/\
	imHistBucketHeader *pHash;/* Hash table pointer			*/\
        int     nEntry;		/* Number of unique pixels		*/\
        ImHistTable *histTable;/* Histogram table			*/\
	int     hashSize;	/* Hash table size			*/\
	int     index;		/* Hash table index			*/\
	int	rgb[3];		/* RGB triplet				*/\
	float	hsi[3];		/* HSI equivalent			*/\
									\
	/*  Allocate the hash table as a table of collision lists.	*/\
	hashSize = (hashSize1) * (hashSize2);				\
	if ( (hash = (imHistBucketHeader **)malloc(			\
		sizeof( imHistBucketHeader * ) * hashSize )) == NULL )	\
	{								\
		ImErrNo = IMEMALLOC;					\
		return ( NULL );					\
	}								\
	memset( (void *)hash, 0x00, sizeof( imHistBucketHeader * ) * hashSize );\
									\
	/*  Hash the color index into an occurrence table.		*/\
	pPixel = ImVfbQFirst( vfb );					\
	h      = ImVfbQHeight( vfb );					\
	w      = ImVfbQWidth( vfb );					\
	imVfbHistNColors = 0;						\
	for ( i = 0; i < h; i++ )					\
	{								\
		for ( k = 0; k < w; k++ )				\
		{							\
			rgb[0] = ImVfbQRed(   vfb, pPixel );		\
			rgb[1] = ImVfbQGreen( vfb, pPixel );		\
			rgb[2] = ImVfbQBlue(  vfb, pPixel );		\
			ImRgbToHsi( rgb, hsi );				\
			index = ((int)(hsi[which1] * (factor1)) << (shift)) |\
				query2( vfb, pPixel );			\
			if ( imVfbHistAddValue( hash, index, (int)(hsi[which3] * (factor3)), (int)(hsi[which4] * (factor4)) ) == -1 )\
			{						\
				/* ImErrNo already set.			*/\
				imVfbHistHashFree( hash, hashSize );	\
				return ( NULL );			\
			}						\
			ImVfbSInc( vfb, pPixel );			\
		}							\
	}								\
									\
	/*  Allocate the histogram table.				*/\
        if ( (histTable = imVfbHistTableAlloc( imVfbHistNColors, fields, 3 )) == NULL )\
	{								\
		/* ImErrNo already set.					*/\
		return( NULL );						\
	}								\
									\
	/*  Set the histogram table.					*/\
        for ( nEntry = i = 0; i < (hashSize1); i++ )			\
        {								\
		for ( k = 0; k < (hashSize2); k++ )			\
		{							\
			index = (i << (shift)) | k;			\
			pHash = hash[ index ];				\
			if ( pHash == NULL )				\
				continue;				\
			for ( j = 0; j < pHash->imhist_nextSlot; j++ )	\
			{						\
				histTable->tableField1[nEntry] = (float)i/(factor1);	\
				histTable->tableField2[nEntry] = k;	\
				histTable->tableField3[nEntry] = (float)pHash->imhist_list[j].imhist_value1/(factor3);\
				histTable->tableField4[nEntry] = (float)pHash->imhist_list[j].imhist_value2/(factor4);\
				histTable->imhist_nOccur[nEntry++]     = pHash->imhist_list[j].imhist_nOccur;\
			}						\
			/* Toss the collision list.			*/\
			free( (char *)pHash->imhist_list );		\
			free( (char *)pHash );				\
		}							\
        }								\
	free( (char *)hash );						\
	return ( histTable );						\
}

#define IM_HIST4FIELD3HSI2( funcName, fields, which1, factor1, which2, factor2, which3, factor3, query4, tableField1, tableField2, tableField3, tableField4, hashSize1, hashSize2, shift )	\
ImHistTable *		/* Returns histogram table		*/\
IM_VFBHEADER(funcName)								\
{									\
        int     i, j, k;	/* Counters				*/\
	int     h, w;		/* Height and width of image		*/\
	ImVfbPtr pPixel;	/* Pixel pointer			*/\
	imHistBucketHeader **hash;/* Hash table				*/\
	imHistBucketHeader *pHash;/* Hash table pointer			*/\
        int     nEntry;		/* Number of unique pixels		*/\
        ImHistTable *histTable;/* Histogram table			*/\
	int     hashSize;	/* Hash table size			*/\
	int     index;		/* Hash table index			*/\
	int	rgb[3];		/* RGB triplet				*/\
	float	hsi[3];		/* HSI equivalent			*/\
									\
	/*  Allocate the hash table as a table of collision lists.	*/\
	hashSize = (hashSize1) * (hashSize2);				\
	if ( (hash = (imHistBucketHeader **)malloc(			\
		sizeof( imHistBucketHeader * ) * hashSize )) == NULL )	\
	{								\
		ImErrNo = IMEMALLOC;					\
		return ( NULL );					\
	}								\
	memset( (void *)hash, 0x00, sizeof( imHistBucketHeader * ) * hashSize );\
									\
	/*  Hash the color index into an occurrence table.		*/\
	pPixel = ImVfbQFirst( vfb );					\
	h      = ImVfbQHeight( vfb );					\
	w      = ImVfbQWidth( vfb );					\
	imVfbHistNColors = 0;						\
	for ( i = 0; i < h; i++ )					\
	{								\
		for ( k = 0; k < w; k++ )				\
		{							\
			rgb[0] = ImVfbQRed(   vfb, pPixel );		\
			rgb[1] = ImVfbQGreen( vfb, pPixel );		\
			rgb[2] = ImVfbQBlue(  vfb, pPixel );		\
			ImRgbToHsi( rgb, hsi );				\
			index = ((int)(hsi[which1] * (factor1)) << (shift)) |\
				(int)(hsi[which2] * (factor2));		\
			if ( imVfbHistAddValue( hash, index, (int)(hsi[which3] * (factor3)), query4( vfb, pPixel ) ) == -1 )\
			{						\
				/* ImErrNo already set.			*/\
				imVfbHistHashFree( hash, hashSize );	\
				return ( NULL );			\
			}						\
			ImVfbSInc( vfb, pPixel );			\
		}							\
	}								\
									\
	/*  Allocate the histogram table.				*/\
        if ( (histTable = imVfbHistTableAlloc( imVfbHistNColors, fields, 3 )) == NULL )\
	{								\
		/* ImErrNo already set.					*/\
		return( NULL );						\
	}								\
									\
	/*  Set the histogram table.					*/\
        for ( nEntry = i = 0; i < (hashSize1); i++ )			\
        {								\
		for ( k = 0; k < (hashSize2); k++ )			\
		{							\
			index = (i << (shift)) | k;			\
			pHash = hash[ index ];				\
			if ( pHash == NULL )				\
				continue;				\
			for ( j = 0; j < pHash->imhist_nextSlot; j++ )	\
			{						\
				histTable->tableField1[nEntry] = (float)i/(factor1);	\
				histTable->tableField2[nEntry] = (float)k/(factor2);	\
				histTable->tableField3[nEntry] = (float)pHash->imhist_list[j].imhist_value1/(factor3);\
				histTable->tableField4[nEntry] = pHash->imhist_list[j].imhist_value2;\
				histTable->imhist_nOccur[nEntry++]     = pHash->imhist_list[j].imhist_nOccur;\
			}						\
			/* Toss the collision list.			*/\
			free( (char *)pHash->imhist_list );		\
			free( (char *)pHash );				\
		}							\
        }								\
	free( (char *)hash );						\
	return ( histTable );						\
}


/*
 *  All possible legal combinations of 4 fields:
 *	( different orderings of the first 2 fields are equivalent )
 *	( different orderings of the last 2 fields are equivalent )
 *
 *	R G B A
 *	R G A B	(same as R G B A)
 *	R B G A
 *	R B A G	(same as R B G A)
 *	R A G B
 *	R A B G	(same as R A G B)
 *
 *	G R B A	(same as R G B A)
 *	G R A B	(same as R G B A)
 *	G B R A
 *	G B A R	(same as G B R A)
 *	G A R B
 *	G A B R	(same as G A R B)
 *
 *	B R G A	(same as R B G A)
 *	B R A G	(same as R B G A)
 *	B G R A	(same as G B R A)
 *	B G A R	(same as G B R A)
 *	B A R G
 *	B A G R	(same as B A R G)
 *
 *	A R G B	(same as R A B G)
 *	A R B G	(same as R A B G)
 *	A B R G	(same as B A R G)
 *	A B G R	(same as B A R G)
 *	A G R B	(same as G A R B)
 *	A G B R	(same as G A R B)
 *
 *	H S I A
 *	H S A I	(same as H S I A)
 *	H I S A
 *	H I A S	(same as H I S A)
 *	H A I S
 *	H A S I	(same as H A I S)
 *
 *	S H I A	(same as H S I A)
 *	S H A I	(same as H S I A)
 *	S I H A
 *	S I A H	(same as S I H A)
 *	S A H I
 *	S A I H	(same as S A H I)
 *
 *	I H S A	(same as H I S A)
 *	I H A S	(same as H I S A)
 *	I S H A	(same as S I H A)
 *	I S A H	(same as S I H A)
 *	I A H S
 *	I A S H	(same as I A H S)
 *
 *	A H S I	(same as H A I S)
 *	A H I S	(same as H A I S)
 *	A I H S	(same as I A H S)
 *	A I S H	(same as I A H S)
 *	A S H I	(same as S A H I)
 *	A S I H	(same as S A H I)
 */

IM_HIST4FIELD( imRGBAHist, IMRED|IMGREEN|IMBLUE|IMALPHA,
	ImVfbQRed,	ImVfbQGreen,	ImVfbQBlue,	ImVfbQAlpha,
	imhist_red,	imhist_green,	imhist_blue,	imhist_alpha,
	256, 256, 8 )
IM_HIST4FIELD( imRBGAHist, IMRED|IMBLUE|IMGREEN|IMALPHA,
	ImVfbQRed,	ImVfbQBlue,	ImVfbQGreen,	ImVfbQAlpha,
	imhist_red,	imhist_blue,	imhist_green,	imhist_alpha,
	256, 256, 8 )
IM_HIST4FIELD( imRAGBHist, IMRED|IMALPHA|IMGREEN|IMBLUE,
	ImVfbQRed,	ImVfbQAlpha,	ImVfbQGreen,	ImVfbQBlue,
	imhist_red,	imhist_alpha,	imhist_green,	imhist_blue,
	256, 256, 8 )

IM_HIST4FIELD( imGBRAHist, IMGREEN|IMBLUE|IMRED|IMALPHA,
	ImVfbQGreen,	ImVfbQBlue,	ImVfbQRed,	ImVfbQAlpha,
	imhist_green,	imhist_blue,	imhist_red,	imhist_alpha,
	256, 256, 8 )
IM_HIST4FIELD( imGARBHist, IMGREEN|IMALPHA|IMRED|IMBLUE,
	ImVfbQGreen,	ImVfbQAlpha,	ImVfbQRed,	ImVfbQBlue,
	imhist_green,	imhist_alpha,	imhist_red,	imhist_blue,
	256, 256, 8 )

IM_HIST4FIELD( imBARGHist, IMBLUE|IMALPHA|IMRED|IMGREEN,
	ImVfbQBlue,	ImVfbQAlpha,	ImVfbQRed,	ImVfbQGreen,
	imhist_blue,	imhist_alpha,	imhist_red,	imhist_green,
	256, 256, 8 )

IM_HIST4FIELD3HSI2( imHSIAHist, IMHUE|IMSATURATION|IMINTENSITY|IMALPHA,
	0, (1023.0/360.0),1, 1023.0,	2, 1023.0,	ImVfbQAlpha,
	imhist_hue,	imhist_saturation,imhist_intensity,imhist_alpha,
	1024, 1024, 10 )
IM_HIST4FIELD3HSI2( imHISAHist, IMHUE|IMINTENSITY|IMSATURATION|IMALPHA,
	0, (1023.0/360.0),2, 1023.0,	1, 1023.0,	ImVfbQAlpha,
	imhist_hue,	imhist_intensity,imhist_saturation,imhist_alpha,
	1024, 1024, 10 )
IM_HIST4FIELD3HSI( imHAISHist, IMHUE|IMALPHA|IMINTENSITY|IMSATURATION,
	0, (1023.0/360.0),ImVfbQAlpha,	2, 1023.0,	1, 1023.0,
	imhist_hue,	imhist_alpha,	imhist_intensity,imhist_saturation,
	1024, 256, 8 )
IM_HIST4FIELD3HSI2( imSIHAHist, IMSATURATION|IMINTENSITY|IMHUE|IMALPHA,
	1, 1023.0,	2, 1023.0,	0, (1023.0/360.0),ImVfbQAlpha,
	imhist_saturation,imhist_intensity,imhist_hue,	imhist_alpha,
	1024, 1024, 10 )
IM_HIST4FIELD3HSI( imSAHIHist, IMSATURATION|IMALPHA|IMHUE|IMINTENSITY,
	1, 1023.0,	ImVfbQAlpha,	0, (1023.0/360.0),2, 1023.0,
	imhist_saturation,imhist_alpha,	imhist_hue,	imhist_intensity,
	1024, 256, 8 )
IM_HIST4FIELD3HSI( imIAHSHist, IMINTENSITY|IMALPHA|IMHUE|IMSATURATION,
	2, 1023.0,	ImVfbQAlpha,	0, (1023.0/360.0),1, 1023.0,
	imhist_intensity,imhist_alpha,	imhist_hue,	imhist_saturation,
	1024, 256, 8 )
