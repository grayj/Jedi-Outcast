#include "ParamImSeq.h"
#include "MpegDecoder.h"
#include "Param.h"
#include <stdio.h>
#include <string.h>
#include "im.h"
#include "Zim.h"

extern int globalRoqWidth;
extern int globalRoqHeight;
extern int globalRoqHorizOffset;
extern int globalRoqVertOffset;

TagTable *ParamImSeqNextImageFromParam(ParamImSeq *self);

void ParamImSeqFree(ParamImSeq *self)
{
  free(self);
}

ParamImSeq *ParamImSeqNew(Param *paramobj)
{
  ParamImSeq *self;

  self = calloc(1, sizeof(ParamImSeq));
  
  /*mpeg setup*/
  if (ParamTimecode(paramobj))
  {
  	self->skipFields = YES;
  }
  else
  {
  	self->skipFields = NO;
  }
  globalRoqWidth = ParamMpegCropWidth(paramobj);
  globalRoqHeight = ParamMpegCropHeight(paramobj);
  globalRoqHorizOffset = ParamMpegCropXOrigin(paramobj);
  globalRoqVertOffset = ParamMpegCropYOrigin(paramobj);

  self->mpegFirstFrame = 0;
  self->mpegLastFrame = -1;
  
  self->param = paramobj;
  self->mpeg = NULL;
  self->currentImageFilename[0] = '\0';
  self->hasAlpha = NO;
  self->frameNum = -1;//on first increment it will be 0
  self->start = 0;
  return self;
}

const char * ParamImSeqCurrentImageFilename(ParamImSeq *self)
{
	return self->currentImageFilename;
}

BOOL ParamImSeqMoreFrames(ParamImSeq *self)
{
  BOOL ret;

  ret = NO;
  
  if (self->mpeg != NULL)
	{
	  ret = MpegDecoderMoreFrames(self->mpeg);
	}

  if (! ret)
	{
	  ret = ParamMoreFrames(self->param);
	}
  
  return ret;
}

double ParamImSeqCurrentFade(ParamImSeq *self)
{
	return ParamFadeFromFrame(self->param, self->frameNum);
}

TagTable *ParamImSeqNextImage(ParamImSeq *self)
{
  ImVfb *vfb;
  TagTable *frameTagTable;
  time_t now;
  float rate;
  
  self->frameNum++;
  if (self->frameNum ==1) //The first frame (#0) could include a large amount of time seeking
  {
  	self->start = time(NULL);
  }
  frameTagTable = ParamImSeqNextImageFromParam(self);

  if (ParamImSeqCurrentFade(self) != 1.0)
	{
	  TagEntryQValue(TagTableQDirect(frameTagTable, "image vfb", 0), &vfb);
	  ImVfbFade(vfb, ParamImSeqCurrentFade(self), IMVFBRGB, vfb);
	}
  self ->hasAlpha = ZimHasAlpha(frameTagTable);

  if (ParamScaleDown(self->param))
	{
	  TagTable *old;
	  old = frameTagTable;
	  frameTagTable = ZimExtractField(frameTagTable);
	  ZimFree(old);
	}
  now = time(NULL);
  rate = 60.0f*(float) self->frameNum/((float) now - self->start);
  printf("\nParamImSeq: on %s frame %d out of %d, rate = %0.2f frames/min, ETA = %0.2f min\n\n", self ->currentImageFilename, self->frameNum, ParamNumberOfFrames(self->param), rate, ((float) ParamNumberOfFrames(self->param) - self->frameNum)/rate);
  return frameTagTable;
}

TagTable *ParamImSeqNextImageFromParam(ParamImSeq *self)
{
  const char *str;
  char *ext;
  TagTable  *nextImage = NULL, *unusedImage;
  int mpegSkipNum;
  
  if (self->mpeg)
	{
	  if (MpegDecoderMoreFrames(self->mpeg))
		{
		  nextImage = MpegDecoderNextImage(self->mpeg);
		  if (self->skipFields)
			{
			  sprintf(self->currentImageFilename, "%s%.06db",MpegDecoderCurrentFilename(self->mpeg),MpegDecoderBitstreamFrameNum(self->mpeg));
			}
		  else
			{
			  sprintf(self->currentImageFilename, "%s%.06d",MpegDecoderCurrentFilename(self->mpeg),MpegDecoderBitstreamFrameNum(self->mpeg));
			}
		  if (self->mpegSkipNum > 1)
			{  // skip the required number of images so we're primed for the next pass
			  mpegSkipNum = self->mpegSkipNum -1;
			  while (mpegSkipNum--)
				{
				  unusedImage = MpegDecoderNextImage(self->mpeg);
				  ZimFree(unusedImage);
				}
			}
		  
		  return nextImage;
		}
	  else
		{
		  MpegDecoderFree(self->mpeg);
		  self->mpeg = NULL;
		}
	}
  if (ParamMoreFrames(self->param) == YES)
	{
	  str =  ParamGetNextImageFilename(self->param);
	}
  else
	{
	  return NULL;
	}
  if ((str == NULL) || (str[0] == '\0'))
	{
	  return NULL;
	}
  else
	{
	  strcpy(self->currentImageFilename, str);
	}
  if( (ext = strstr(self->currentImageFilename , ".mpg")) || (ext = strstr(self->currentImageFilename, ".m2v")))
	{
	  char *margv[] = 
		{
		  "mpegstream", "-b", NULL, "-r", "-o6", "-v0", NULL, NULL
		};
	  int margc = 6;
	  char *fn;	  
	  char *frameNum;

	  self->mpegFirstFrame = 0;
	  self->mpegLastFrame = 999999;
	  self->mpegSkipNum = 1;
	  if (ext[4] != '\0')
		{
		  ParamGetCurrentRange(self->param, &self->mpegFirstFrame, &self->mpegLastFrame, &self->mpegSkipNum ,YES);
		  if (self->mpegLastFrame ==999999)  //we didn't have a range
		  {
		  	self->mpegLastFrame = atoi(&ext[4]);  //use the number by itself, only one frame
		  }
		}

	  fn = strdup(self->currentImageFilename);
	  frameNum = strrchr(fn, '.')+4;
	  *frameNum = '\0';//remove frame number so mpeg object can open the file
	  margv[2] = fn;
	  
	  if (! self->skipFields)  //specify frame flag
	  {
	  	margv[margc++] = "-f";
	  }
	  
	  self->mpeg = MpegDecoderNew(margc, margv, self->mpegFirstFrame, self->mpegLastFrame);
	  return ParamImSeqNextImage(self);/*in case it's an empty mpg...*/
	}
  else
	{
	  char *secondFilename,firstFilename[MAXPATHLEN+1];
	  TagTable *image1tt ;
	  ImVfb *img, *image1;
	  int fields;
	  
	  if (!(secondFilename= strchr(self->currentImageFilename,'\n')))
		{  //one filename, no compositing
			nextImage = ZimImageTagTableFromFile(self->currentImageFilename, "");
		}
	  else
		{
		  strncpy(firstFilename,self->currentImageFilename,secondFilename-self->currentImageFilename);
		  firstFilename[secondFilename-self->currentImageFilename]='\0';
		  secondFilename++;//point past the \n
		  image1tt = ZimImageTagTableFromFile(firstFilename, "");
		  nextImage = ZimImageTagTableFromFile(secondFilename, "");//result will be in here
		  TagEntryQValue(TagTableQDirect(image1tt, "image vfb", 0), &image1);
		  TagEntryQValue(TagTableQDirect(nextImage, "image vfb", 0), &img);		  
		  
		//image is the composite of those two
		  fields = IMRED | IMGREEN | IMBLUE;
		  if (ImVfbQFields(img) && IMVFBALPHA)
			{
			  fields |= IMALPHA;
			}
		  
		  ImVfbComp(image1, 0, 0, ImVfbQWidth(img), ImVfbQHeight(img), fields, IMCOMPOVER, img, 0, 0);
		  //free image1tt, it's not needed any more
		  ZimFree(image1tt);
		}
	}
  
  return nextImage;
}
BOOL ParamImSeqCurrentImageHasAlpha(ParamImSeq *self)
{
	return self->hasAlpha;
}
BOOL ParamImSeqCurrentImageHadAlpha(ParamImSeq *self)
{
	return ParamImSeqCurrentImageHasAlpha(self);
}
