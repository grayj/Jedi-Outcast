/*

.roq video file maker

*/

#include "Param.h"
#include "ParamImSeq.h"
#include "Zim.h"
#include "roq.h"
//#include <libc.h>

int main( int argc, char *argv[] ) {
  Param *inputParamFile;
  int temp;
  int type = 0;  // 0 = roq, 1 = decode images to dir, 2 = save images from mpeg for codebooking
  int encodeQuietly = 0;
  char *paramname = NULL;
  char *decodeDir = NULL;
  ParamImSeq *imSeq;
  TagTable *image;
  char filename[1024 + 1];
  
  printf("%s\nroq video file maker\n(c) 1995 Trilobyte, Inc.\nBuid Date: "__DATE__"\n", argv[0]);
  
  
  if ( argc < 2 ) {
	printf("use: roq file.param -q(quiet) -d dir (decode param's images to dir)\n");
	exit(1);
  }
  
  for (temp=1;temp<argc; temp++)
	{
	  if (strncasecmp(argv[temp], "-q", 2) == 0)
		{
		  encodeQuietly = 1;
		}
	  else if (strncasecmp(argv[temp], "-d", 2) == 0)
		{
		  type = 1;
		  temp++;
		  if (temp>=argc)
			{
			  fprintf(stderr, "Error: must specify a directory to decode frames into\n");
			  exit(1);
			}
		  
		  decodeDir = argv[temp];
		}
  	  else if (strncasecmp(argv[temp], "-m", 2) == 0)
		{
		  type = 2;
		}
	  else
		{
		  if (paramname)
			{
			  fprintf(stderr, "Error: can only roq one param at a time\n");
			  exit(2);
			}
		  paramname = argv[temp];
		}
	}	  
  if (!paramname)
	{
	  fprintf(stderr, "Error: no param\n");
	  exit(3);
	}
  
  inputParamFile = ParamNewFromFile(paramname);

  if (type == 0)
	{
	  
	  RoqInit();
	  RoqEncodeQuietly(encodeQuietly); 
	  RoqInitRoQFile(ParamRoqTempFilename(inputParamFile), inputParamFile->fps);
	  RoqEncodeStream(inputParamFile);
	  RoqCloseRoQFile();
	  RoqFree();
	  ParamMoveFile(inputParamFile);
	  ParamFree(inputParamFile);
	}
  else if (type == 1)
	{
	  int frameCount = 0;
	  
	  imSeq = ParamImSeqNew(inputParamFile);
	  while (ParamImSeqMoreFrames(imSeq))
		{
		  image = ParamImSeqNextImage(imSeq);
		  sprintf(filename, "%s/%06d.jpg", decodeDir, frameCount);
		  printf("Saving file to %s\n", filename);
		  ZimWriteJFIF(image, filename, 95);
		  frameCount++;
		}
	}
  else if (type == 2)
	{
	  fprintf(stderr, "Not yet implemented\n");
	}
  return 0;
}
