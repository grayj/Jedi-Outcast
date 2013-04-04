#include "sdsc.h"

#include "MpegDecoder.h"

//#include "sys/param.h"

#include "Param.h"

#include <time.h>



#ifndef _ParamImSeq_H_INCLUDED

#define _ParamImSeq_H_INCLUDED

#ifndef MAXPATHLEN

#include <stdlib.h>

#define MAXPATHLEN _MAX_PATH

#endif 



typedef struct 

{

  Param *param;

  MpegDecoder *mpeg;

  BOOL skipFields;

  char currentImageFilename[MAXPATHLEN + 1];

  int mpegFirstFrame, mpegLastFrame, mpegSkipNum;

  BOOL hasAlpha;

  int frameNum;

  time_t start;

} ParamImSeq;



ParamImSeq *ParamImSeqNew(Param *paramobj);

void ParamImSeqFree(ParamImSeq *self);

const char * ParamImSeqCurrentImageFilename(ParamImSeq *self);

BOOL ParamImSeqMoreFrames(ParamImSeq *self);

TagTable *ParamImSeqNextImage(ParamImSeq *self);



BOOL ParamImSeqCurrentImageHasAlpha(ParamImSeq *self);

BOOL ParamImSeqCurrentImageHadAlpha(ParamImSeq *self);

double ParamImSeqCurrentFade(ParamImSeq *self);

#endif //_ParamImSeq_H_INCLUDED

