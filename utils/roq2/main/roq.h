

#include <string.h>
#include <io.h>
#include <math.h>

#include "gdefs.h"
#include "Param.h"
#include "ParamImSeq.h"
#include "quadDefs.h"



int  RoqInit(  );
int  RoqFree(  );
int  RoqWriteLossless(  );
int  RoqCloseRoQFile(  );
int  RoqInitRoQFile( const char * roqFilename, const int iFPS );
int  RoqInitRoQPatterns(  );
int  RoqEncodeStream( Param * paramInputFile );
int  RoqEncodeQuietly( BOOL which );
BOOL  RoqIsQuiet(  );
BOOL  RoqIsLastFrame(  );
TagTable *  RoqCurrentImage(  );
int  RoqMarkQuadx(  int xat , int yat , int size , float cerror , int choice );
int  RoqWritePuzzleFrame( quadcel * pquad );
int  RoqWriteFrame( quadcel * pquad );
int  RoqWriteCodeBook(  byte * codebook );
int  RoqWriteCodeBookToStream(  byte * codes , int csize , word cflags );
int  RoqPreviousFrameSize(  );
BOOL  RoqMakingVideo(  );
BOOL  RoqParamNoAlpha(  );
BOOL  RoqSearchType(  );
BOOL  RoqHasSound(  );
char *  RoqCurrentFilename(  );
int  RoqNormalFrameSize(  );
int  RoqFirstFrameSize(  );
BOOL  RoqScaleable(  );
ParamImSeq * RoqParamImSeq();
int  RoqLoadAndDisplayImage(TagTable *newimage );
double  RoqCurrentFade();
