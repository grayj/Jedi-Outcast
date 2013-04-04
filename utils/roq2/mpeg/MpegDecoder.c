#include "MpegDecoder.h"

#include "config.h"

#include "global.h"

#ifndef WIN32

#include <libc.h>

#endif //WIN32

#include <time.h>



void MpegDecoderFree(MpegDecoder *self)

{

  free(self);  

}



MpegDecoder *MpegDecoderNew(int argc, char ** argv, int ff, int lf)

{

	int outType;

	time_t start, now;

	MpegDecoder *self;

	float framesPerMin;

	

	self = (MpegDecoder *)  calloc(sizeof(MpegDecoder), 1);

  mpeg_main(argc, argv);

  self->moreFrames = 1;

  self->bitstreamFrameNum = 0;

  self->sequenceFrameNum = 0;

  self->firstFrame = ff;

  self->lastFrame = lf;

  if (self->firstFrame > 0 )

  {

  	//set output routine to a null routine to speed things up

	outType = Output_Type;

	Output_Type = T_NoWrite;

	start = time(NULL);

	while (MpegDecoderBitstreamFrameNum(self) < (self->firstFrame -1) )  //skip to frame before first frame

	{

		MpegDecoderNextImage(self);

		now = time(NULL);

		framesPerMin = 60*((float) MpegDecoderBitstreamFrameNum(self) )/(((float) now - start));

		printf("seeking to frame %d, on %d, decode rate = %0.2f frames/min, ETA = %0.2f minutes\n", self->firstFrame , MpegDecoderBitstreamFrameNum(self) , framesPerMin, ((float) self->firstFrame - MpegDecoderBitstreamFrameNum(self) )/framesPerMin);

	}

	//set version back to what it was

	Output_Type = outType;

  }

  return self;

}

int MpegDecoderMoreFrames(MpegDecoder *self)

{

  return self->moreFrames;

}



TagTable *MpegDecoderNextImage(MpegDecoder *self)

{

  globalCurrentRoqImage = NULL;//don't free it, we don't own it, whoever asked for it does!

	

	do {

		if ((self->bitstreamFrameNum-1) > self->lastFrame)

		{

			globalCurrentRoqImage = NULL;

			self->moreFrames = 0;

			//printf("last frame encountered, MPEG frame numbers %d, %d\n", self->bitstreamFrameNum, self->sequenceFrameNum);

			return globalCurrentRoqImage;

		}

		if (!Headers())

		{

			if (self->sequenceFrameNum != 0)  //end of sequence

			{

				Output_Last_Frame_of_Sequence(self->bitstreamFrameNum);//get that last frame

				Deinitialize_Sequence();

				self->sequenceFrameNum = 0;

	#ifdef VERIFY

				Clear_Verify_Headers();

	#endif /* VERIFY */

  				//printf("end of sequence, MPEG frame numbers %d, %d\n", self->bitstreamFrameNum, self->sequenceFrameNum);

				return globalCurrentRoqImage;

			}

			else  //end of stream

			{

				globalCurrentRoqImage = NULL;

				self->moreFrames = 0;

  				//printf("end of stream, MPEG frame numbers %d, %d\n", self->bitstreamFrameNum, self->sequenceFrameNum);

				return globalCurrentRoqImage;

			}

		}

	

		if (self->sequenceFrameNum == 0)

		{

			Initialize_Sequence();

		}

	

	

		Decode_Picture(self->bitstreamFrameNum, self->sequenceFrameNum);

		//printf("MPEG frame numbers %d, %d\n", self->bitstreamFrameNum, self->sequenceFrameNum);

		if (!Second_Field)

		{

			self->bitstreamFrameNum++;

			self->sequenceFrameNum++;

			if ((self->bitstreamFrameNum-1) > self->lastFrame)

			{

				self->moreFrames = 0;

			}

		}

	} while (self->bitstreamFrameNum <=1);

  return globalCurrentRoqImage;

}



int MpegDecoderBitstreamFrameNum(MpegDecoder *self)

{

	return self->bitstreamFrameNum-2;

}



const char * MpegDecoderCurrentFilename(MpegDecoder *self)

{

	return Main_Bitstream_Filename;

}



