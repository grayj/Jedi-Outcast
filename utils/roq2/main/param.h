#include "gdefs.h"

#ifndef _Param_H_INCLUDED
#define _Param_H_INCLUDED

typedef struct {
	BOOL scaleDown;
	BOOL encodeVideo;
	BOOL useTimecodeForRange;
	BOOL addPath;
	BOOL screenShots;
	BOOL startPalette;
	BOOL endPalette;
	BOOL fixedPalette;
	BOOL keyColor;
	BOOL justDelta;
	BOOL makeVectors;
	BOOL justDeltaFlag;
	BOOL noAlphaAtAll;
	BOOL fullSearch;
	BOOL hasSound;
	BOOL isScaleable;
	int  *range;
	BOOL *padding, *padding2;
	char **file, **file2;
	char *soundfile;
	char *currentPath;
	char *outputFilename;
	char *tempFilename;
	char *startPal;
	char *endPal;
	char *currentFile;
	int *skipnum, *skipnum2;
	int *startnum, *startnum2;
	int	*endnum, *endnum2;
	int *numpadding, *numpadding2;
	int *numfiles;
	byte	keyR, keyG, keyB;
	int field;
	int realnum;
	int numInputFiles;
	int onFrame;
	int	firstframesize;
	int	normalframesize;
	int jpegDefault;
	int fadeDownStartFrame;
	int fadeDownDuration;
	int fadeUpStartFrame;
	int fadeUpDuration;
	int mpegCropWidth;
	int mpegCropHeight;
	int mpegCropXOrigin;
	int mpegCropYOrigin;
	int fps;
  } Param;

Param *ParamNewFromFile(const char *fileName);
void ParamFree(Param *self);
void ParamGetCurrentRange(Param *self, int * start, int *end,int * skip,BOOL incr);
void ParamGetNthInputFileName(Param *self, byte* fileName, int n);
const char*ParamGetNextImageFilename(Param *self);
const char*ParamRoqFilename(Param *self);
const char*ParamSoundFilename(Param *self);
const char*ParamRoqTempFilename(Param *self);
int ParamMoveFile(Param *self);
BOOL ParamTimecode(Param *self);
BOOL outputVectors(Param *self);
BOOL ParamHasSound(Param *self);
BOOL ParamDeltaFrames(Param *self);
BOOL ParamNoAlpha(Param *self);
BOOL ParamSearchType(Param *self);
BOOL ParamMoreFrames(Param *self);
int ParamNumberOfFrames(Param *self);
int ParamFirstframesize(Param *self);
int ParamNormalframesize(Param *self);
BOOL ParamIsScaleable(Param *self);
int ParamJpegQuality(Param *self);
int ParamMpegCropWidth(Param *self);
int ParamMpegCropHeight(Param *self);
int ParamMpegCropXOrigin(Param *self);
int ParamMpegCropYOrigin(Param *self);
double ParamFadeFromFrame(Param *self, int frameNum);
BOOL ParamScaleDown(Param *self);

int parseRange(char *rangeStr,int field, int skipnum[], int startnum[], int endnum[],int numfiles[],BOOL padding[],int numpadding[] );
int parseTimecodeRange(char *rangeStr,int field, int skipnum[], int startnum[], int endnum[],int numfiles[],BOOL padding[],int numpadding[] );

#endif //_Param_H_INCLUDED 
