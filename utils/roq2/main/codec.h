#define MAXERRORMAX 200
#define IPSIZE int
#define MOTION_MIN 10

#define FULLFRAME	0
#define JUSTMOTION	1

#define VQDATA		double

#include "gdefs.h"
#include "roq.h"
#include "quadDefs.h"

#ifndef _codec_H_INCLUDE
#define _codec_H_INCLUDE

int stdvq( long dimension, long codebooksize, byte *image, int pixelsWide, int *coords, int trainsize, VQDATA **codebook, int gotcodes, float *list );
long min_sup(long first, long last, VQDATA tempnorm, VQDATA *norm);
long max_inf(long first, long last, VQDATA tempnorm, VQDATA *norm);


int  CodecInit( );
int  CodecPrepareCodeBook(  );
int  CodecSparseEncode( );
int  CodecIRGBtab(  );
int  CodecInitImages(  );
int  CodecQuadX(  int  startX , int  startY , int  quadSize );
int  CodecInitQStatus(  );
float  CodecSnr( byte * old ,byte * new ,int size );
int  CodecFvqData(  byte * bitmap , int size , int realx , int realy , quadcel * pquad ,BOOL clamp );
int  CodecGetData(  unsigned char * iData , int qSize , int startX , int startY , TagTable * bitmap );
int  CodecComputeMotionBlock(  byte * old ,byte * new ,int size );
int  CodecVqData8(  byte * cel , quadcel * pquad );
int  CodecVqData4(  byte * cel , quadcel * pquad );
int  CodecVqData2(  byte * cel , quadcel * pquad );
int  CodecMotMeanY(  );
int  CodecMotMeanX(  );
int  CodecSetPreviousImage( const char* filename ,TagTable * timage );
int  CodecBestCodeword( unsigned char * tempvector ,int dimension ,VQDATA ** codebook ,VQDATA * norm ,int * index );
int  CodecSetStatus( int status ,quadcel *  cel );
int  CodecComputeCurrentQuadOutputSize( quadcel *  pquad );
int  CodecSetSize( int  size ,quadcel *  cel );

#endif //_codec_H_INCLUDE