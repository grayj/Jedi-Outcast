

#include "sdsc.h"

#include "im.h"



#ifndef _MpegDecoder_H_INCLUDED

#define _MpegDecoder_H_INCLUDED



typedef struct MpegDecoder MpegDecoder;



struct MpegDecoder {

  int sequenceFrameNum;

  int bitstreamFrameNum;

  int moreFrames;

  int firstFrame;

  int lastFrame;

};



MpegDecoder *MpegDecoderNew(int argc, char ** argv, int firstFrame, int lastFrame);

void MpegDecoderFree(MpegDecoder *self);



TagTable *MpegDecoderNextImage(MpegDecoder *self);

int MpegDecoderMoreFrames(MpegDecoder *self);

int MpegDecoderBitstreamFrameNum(MpegDecoder *self);

const char * MpegDecoderCurrentFilename(MpegDecoder *self);



#endif //_MpegDecoder_H_INCLUDED