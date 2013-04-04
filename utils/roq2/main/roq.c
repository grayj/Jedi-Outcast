
#include "roq.h"
#include "codec.h"
#include "ParamImSeq.h"
#include "Param.h"
#include "Zim.h"
#include <sys/stat.h>

static 	Param 				*param;
static 	FILE				*RoQFile;
static	TagTable   	        *image;
static 	int					numQuadCels;
static 	int					quietMode;
static 	int					lastFrame;
static 	char 				roqOutfile[MAXPATHLEN];
static 	char    			currentFile[MAXPATHLEN];
static 	int					numberOfFrames;
static 	int					previousSize;
static 	byte				*codes;
static 	BOOL				dataStuff;
static 	ParamImSeq			*imgSeq;

typedef struct {
	int			format;
	int			rate;
	int			width;
	int			channels;
	int			samples;
	int			dataofs;		// chunk starts this many bytes from file start
} wavinfo_t;

int  RoqWrite16Word( word * aWord ,FILE * stream )
{
  byte	a, b;
	
  a = *aWord & 0xff;
  b = *aWord >> 8;

  fputc( a, stream );
  fputc( b, stream );

  return 1;
}

int  RoqWrite32Word(  unsigned int * aWord ,FILE * stream )
{
  byte	a, b, c, d;
	
  a = *aWord & 0xff;
  b = (*aWord >> 8) & 0xff;
  c = (*aWord >> 16) & 0xff;
  d = (*aWord >> 24) & 0xff;

  fputc( a, stream );
  fputc( b, stream );
  fputc( c, stream );
  fputc( d, stream );
  return 1;
}

/*
===============================================================================

WAV loading

===============================================================================
*/

byte		*data_p;
byte 		*iff_end;
byte 		*last_chunk;
byte 		*iff_data;
int 		iff_chunk_len;
wavinfo_t	info;
byte		*snddata;

short GetLittleShort(void)
{
	short val = 0;
	val = *data_p;
	val = val + (*(data_p+1)<<8);
	data_p += 2;
	return val;
}

int GetLittleLong(void)
{
	int val = 0;
	val = *data_p;
	val = val + (*(data_p+1)<<8);
	val = val + (*(data_p+2)<<16);
	val = val + (*(data_p+3)<<24);
	data_p += 4;
	return val;
}

void FindNextChunk(char *name)
{
	while (1)
	{
		data_p=last_chunk;

		if (data_p >= iff_end)
		{	// didn't find the chunk
			data_p = NULL;
			return;
		}
		
		data_p += 4;
		iff_chunk_len = GetLittleLong();
		if (iff_chunk_len < 0)
		{
			data_p = NULL;
			return;
		}
		data_p -= 8;
		last_chunk = data_p + 8 + ( (iff_chunk_len + 1) & ~1 );
		if (!strncmp((char *)data_p, name, 4))
			return;
	}
}

void FindChunk(char *name)
{
	last_chunk = iff_data;
	FindNextChunk (name);
}


void DumpChunks(void)
{
	char	str[5];
	
	str[4] = 0;
	data_p=iff_data;
	do
	{
		memcpy (str, data_p, 4);
		data_p += 4;
		iff_chunk_len = GetLittleLong();
		printf ("0x%x : %s (%d)\n", (int)(data_p - 4), str, iff_chunk_len);
		data_p += (iff_chunk_len + 1) & ~1;
	} while (data_p < iff_end);
}

/*
============
GetWavinfo
============
*/
wavinfo_t GetWavinfo (const char *name, byte *wav, int wavlength)
{
	wavinfo_t	info;
	int		samples;

	memset (&info, 0, sizeof(info));

	if (!wav)
		return info;
		
	iff_data = wav;
	iff_end = wav + wavlength;

// find "RIFF" chunk
	FindChunk("RIFF");
	if (!(data_p && !strncmp((char *)data_p+8, "WAVE", 4)))
	{
		printf("Missing RIFF/WAVE chunks\n");
		return info;
	}

// get "fmt " chunk
	iff_data = data_p + 12;
// DumpChunks ();

	FindChunk("fmt ");
	if (!data_p)
	{
		printf("Missing fmt chunk\n");
		return info;
	}
	data_p += 8;
	info.format = GetLittleShort();
	info.channels = GetLittleShort();
	info.rate = GetLittleLong();
	data_p += 4+2;
	info.width = GetLittleShort() / 8;

	if (info.format != 1)
	{
		printf("Microsoft PCM format only\n");
		return info;
	}


// find data chunk
	FindChunk("data");
	if (!data_p)
	{
		printf("Missing data chunk\n");
		return info;
	}

	data_p += 4;
	samples = GetLittleLong () / info.width;

	if (info.samples)
	{
		if (samples < info.samples)
			printf ("Sound %s has a bad loop length", name);
	}
	else
		info.samples = samples;

	info.dataofs = data_p - wav;


	return info;
}

ParamImSeq * RoqParamImSeq()
{
	return imgSeq;
}

int  RoqInit(  )
{
  image = NULL;
  quietMode = NO;
  previousSize = 0;
  lastFrame = NO;
  codes = malloc( 4*1024 );
  dataStuff=NO;
  return 1;
}

int  RoqFree(  )
{
  free( codes );

  if (image)
	{
	  ZimFree(image);
	}
	
  return 1;
}

int  RoqEncodeQuietly( BOOL which )
{
  quietMode = which;
  return 1;
}

BOOL  RoqIsQuiet(  )
{
  return quietMode;
}

BOOL  RoqIsLastFrame(  )
{
  return lastFrame;
}

BOOL  RoqScaleable(  )
{
  return ParamIsScaleable(param);
}

BOOL  RoqParamNoAlpha(  )
{
  return  ParamNoAlpha(param);
}

double  RoqCurrentFade()
{
  return  ParamImSeqCurrentFade(imgSeq);
}

BOOL  RoqMakingVideo(  )
{
  return ParamTimecode(param);
}

BOOL  RoqSearchType(  )
{
  return	ParamSearchType(param);
}

BOOL  RoqHasSound(  )
{
  return	ParamHasSound(param);
}

int  RoqPreviousFrameSize(  )
{
  return	previousSize;
}

int  RoqFirstFrameSize(  )
{
  return ParamFirstframesize(param);
}

int  RoqNormalFrameSize(  )
{
  return	ParamNormalframesize(param);
}

char *  RoqCurrentFilename(  )
{
  return currentFile;
}

int FS_ReadFile( const char *filename, byte **data)
{
	FILE*	file;
	int size;

	file = fopen(filename,"rb");
	if (!file) return 0;

	fseek (file, 0, SEEK_END);
	size = ftell (file);
	fseek (file, 0, SEEK_SET);

	*data = malloc(size);
	fread(*data, size, 1, file);

	fclose(file);

	return size;
}

#define ZA_SOUND_MONO		0x1020
#define ZA_SOUND_STEREO		0x1021

int EncodeSound(int onFrame) {
	static	int soundOffset = 0;
	static	short lastFlagsR = 0;
	static	short lastFlagsL = 0;
	static	short	sqrTable[256];

	int		size, i, z;
	word	direct;
	short	left, right, *audio;

	if (onFrame == 1)
	{
		for (z=0;z<128;z++) {
			sqrTable[z] = (short)(z*z);
			sqrTable[z+128] = (short)(-sqrTable[z]);
		}
	}

	if ((soundOffset*info.channels) == info.samples)
		return 0;

	if (info.channels == 2) {
		direct = ZA_SOUND_STEREO;
	} else {
		direct = ZA_SOUND_MONO;
	}

	RoqWrite16Word(&direct, RoQFile);

	size = ((info.rate/30)+1) & (~1);

	if (onFrame == 1) {
		size = size*8;
	}

	if ((soundOffset+size)*info.channels > info.samples) {
		size = (info.samples - (soundOffset*info.channels))/info.channels;
	}

	i = size*info.channels;

	RoqWrite32Word(&i, RoQFile);

	printf("encodeSound: %d bytes to sound\n", i);

	if (info.channels == 2) {
		direct = (lastFlagsL & 0xff00) | ((lastFlagsR & 0xff00)>>8);
	} else {
		direct = lastFlagsL;
	}

	RoqWrite16Word(&direct, RoQFile);

	audio = (short *)(&snddata[info.dataofs]);

	for(i=0;i<size;i++) {
		if (info.channels == 2) {
			left  = (short)sqrt(abs(audio[soundOffset*2+0] - lastFlagsL));
			if (left  > 127) left = 127; if (audio[soundOffset*2+0] - lastFlagsL < 0) left += 128;
			fputc(left,  RoQFile);
			lastFlagsL = lastFlagsL + sqrTable[left ];
			right = (short)sqrt(abs(audio[soundOffset*2+1] - lastFlagsR));
			if (right > 127) right= 127; if (audio[soundOffset*2+1] - lastFlagsR < 0) right+= 128;
			fputc(right, RoQFile);
			lastFlagsR = lastFlagsR + sqrTable[right];
		} else {
			left  = (short)sqrt(abs(audio[soundOffset] - lastFlagsL));
			if (left  > 127) left = 127; if (audio[soundOffset] - lastFlagsL < 0) left += 128;
			fputc(left,  RoQFile);
			lastFlagsL = lastFlagsL + sqrTable[left];
		}
		soundOffset++;
		if ((soundOffset*info.channels) == info.samples) {
			break;
		}
	}

	if (info.channels == 2) {
		lastFlagsR &= 0xff00;
		lastFlagsL &= 0xff00;
	}

	return 1;
}

int  RoqEncodeStream(  Param * paramInputFile )
{
	int			onFrame;
	TagTable	*img;
	int			size;

  onFrame = 1;
  
  CodecInit();
  param = paramInputFile;
  numberOfFrames = ParamNumberOfFrames(param);
  printf("Encoding a total of %d frames (not including mpeg frames)\n", numberOfFrames);
  
  if (ParamNoAlpha(param)==YES) printf("encodeStream: eluding alpha\n");

  snddata = NULL;

  if (ParamHasSound(param)==YES) {
	// load it in
	size = FS_ReadFile( ParamSoundFilename(param), &snddata );
	if ( !snddata ) {
		printf("encodeStream: sound file not found");
		return 0;
	}
	info = GetWavinfo( ParamSoundFilename(param), snddata, size );
  }

  imgSeq = ParamImSeqNew(param);

	img = NULL;
  while( ParamImSeqMoreFrames(imgSeq) ) {
	if (ParamHasSound(param)==YES) {
		EncodeSound(onFrame);
	}
	if (img != NULL)
	{
		ZimFree(img);
	}
	img = ParamImSeqNextImage(imgSeq);
	strcpy(currentFile, ParamImSeqCurrentImageFilename(imgSeq));
	RoqLoadAndDisplayImage(img);
//	if (onFrame==1 && (ParamImSeqCurrentImageHadAlpha(imgSeq)==NO || ParamNoAlpha(param)==YES) && !RoqMakingVideo() && !RoqScaleable()) {
//	  RoqWriteLossless();
//	} else {
	  CodecSparseEncode();
//	}
	onFrame++;
  }

  if (numberOfFrames != 1) {
//	if (ParamImSeqCurrentImageHadAlpha(imgSeq) &&  ParamNoAlpha(param)==NO) {
	  lastFrame = YES;
	  CodecSparseEncode();
//	} else {
//		lastFrame = YES;
//	  RoqWriteLossless();		
//	}
  }

  if (snddata) {
	  free(snddata);
  }

  return 1;
}

long int  RoqSizeFile( FILE * ftosize )
{
  long int	fat, fend;
  fat = ftell(ftosize);
  fseek( ftosize, 0, SEEK_END );
  fend = ftell(ftosize);
  fseek( ftosize, fat, SEEK_SET);
  return (fend);
}

int  RoqWriteLossless(  )
{
  word direct;
  unsigned int j;
  char tempFile[MAXPATHLEN];
  FILE *ftemp;
  byte *buffer;
  int res, mess;

  if (!dataStuff) {
    RoqInitRoQPatterns();
	dataStuff=YES;
  }
  direct = RoQ_QUAD_JPEG;
  RoqWrite16Word(&direct,RoQFile);
  sprintf(tempFile, "%s.jpg",ParamRoqTempFilename(param));

  ZimWriteJFIF(image, tempFile, ParamJpegQuality(param));

  ftemp = fopen(tempFile, "rb");
  if (!ftemp) { fprintf(stderr, "Could not open temp file\n"); exit(1); }
  j = RoqSizeFile(ftemp);
  printf("writeLossless: writing %d bytes to RoQ_QUAD_JPEG\n", j);
  RoqWrite32Word(&j, RoQFile);
  direct = 0;		// flags
	RoqWrite16Word( &direct, RoQFile);
  
  buffer = malloc( 16384 );
  do {
	res = fread( buffer, 1, 16384, ftemp);
	mess = fwrite( buffer, 1, res, RoQFile );
	if (res != mess) { fprintf(stderr, "Could not write to output stream\n"); exit(1); }
  } while ( res == 16384 );
  free( buffer );
  fclose(ftemp);
  if (!lastFrame)
  {
		CodecSetPreviousImage(tempFile, image);
	}
  unlink( tempFile );
  fflush( RoQFile );

  return 1;
}	

int  RoqInitRoQFile( const char * RoQFilename, const int iFPS )
{
  word i;
  static finit = 0;

  if (!finit) {
	finit++;
	printf("initRoQFile: %s\n", RoQFilename);
	RoQFile = fopen( RoQFilename, "wb" );
#ifdef S_ISUID
	chmod(RoQFilename, S_IREAD|S_IWRITE|S_ISUID|S_ISGID|0070|0007 );
#else
	chmod(RoQFilename, S_IREAD|S_IWRITE|0070|0007 );
#endif
	if (!RoQFile) {
	  fprintf(stderr,"Unable to open output file %s.\n", RoQFilename);
	  exit(1);
	}

	i = RoQ_ID;
	RoqWrite16Word(&i, RoQFile);

	i = 0xffff;
	RoqWrite16Word(&i, RoQFile);
	RoqWrite16Word(&i, RoQFile);

	i = iFPS;						// framerate
	RoqWrite16Word(&i, RoQFile);
	fflush( RoQFile );
  }
  strcpy( roqOutfile, RoQFilename );

  return 1;
}

int  RoqInitRoQPatterns(  )
{
  int j;
  word direct;
  
  int pixelsWide;
  int pixelsHigh;
    
  pixelsWide = ZimWidth(image);
  pixelsHigh = ZimHeight(image);
  

  direct = RoQ_QUAD_INFO;
  RoqWrite16Word( &direct, RoQFile);

  j = 8;
  RoqWrite32Word(&j, RoQFile);
  printf("initRoQPatterns: outputting %d bytes to RoQ_INFO\n", j);
  direct = ParamImSeqCurrentImageHadAlpha(imgSeq);
  if (RoqParamNoAlpha() == YES) direct = 0;

  RoqWrite16Word( &direct, RoQFile);

  direct = pixelsWide;
  RoqWrite16Word( &direct, RoQFile);
  direct = pixelsHigh;
  RoqWrite16Word( &direct, RoQFile);
  direct = 8;
  RoqWrite16Word( &direct, RoQFile);
  direct = 4;
  RoqWrite16Word( &direct, RoQFile);

  fflush( RoQFile );

  return 1;
}

int  RoqCloseRoQFile(  )
{
  fflush( RoQFile );
  printf("closeRoQFile: closing RoQ file\n");
  fclose( RoQFile );

  return 1;
}

int  RoqWriteHangFrame(  )
{
  int j;
  word direct;
  printf("*******************************************************************\n");
  direct = RoQ_QUAD_HANG;
  RoqWrite16Word( &direct, RoQFile);
  j = 0;
  RoqWrite32Word(&j, RoQFile);
  direct = 0;
  RoqWrite16Word( &direct, RoQFile);
  return 1;
}

int  RoqWriteCodeBookToStream(  byte * codebook , int csize , word cflags )
{
  int j;
  word direct;

  if (!csize) {
	printf("writeCodeBook: NO VQ DATA!!!!\n");
	return 1;
  }
	
  direct = RoQ_QUAD_CODEBOOK;

  RoqWrite16Word( &direct, RoQFile);

  j = csize;

  RoqWrite32Word(&j, RoQFile);
  printf("writeCodeBook: outputting %d bytes to RoQ_QUAD_CODEBOOK\n", j);

  direct = cflags;
  RoqWrite16Word( &direct, RoQFile);

  fwrite( codebook, j, 1, RoQFile);

  fflush( RoQFile );

  return 1;
}

int  RoqWriteCodeBook(  byte * codebook )
{
  memcpy( codes, codebook, 4096 );
  return 1;
}

int  RoqWriteFrame( quadcel * pquad )
{
  word action, direct;
  int	onCCC, onAction, i, j, code;
  byte *cccList;
  BOOL *use2, *use4;
  int dx,dy,dxMean,dyMean,index2[256],index4[256], dimension;

  cccList = malloc( numQuadCels * 8);					// maximum length 
	use2 = malloc(256*sizeof(BOOL));
  use4 = malloc(256*sizeof(BOOL));

  for(i=0;i<256;i++) {
	use2[i] = NO;
	use4[i] = NO;
  }

  action = 0;
  j = onAction = 0;
  onCCC = 2;											// onAction going to go at zero

	dxMean = CodecMotMeanX();
  dyMean = CodecMotMeanY();

  if (ParamImSeqCurrentImageHadAlpha(imgSeq)) dimension = 10; else dimension = 6;

  for (i=0; i<numQuadCels; i++) {
	if ( pquad[i].size && pquad[i].size < 16 ) {
	  switch( pquad[i].status ) {
	  case	SLD:
		use4[pquad[i].patten[0]] = YES;
		use2[codes[dimension*256+(pquad[i].patten[0]*4)+0]] = YES;
		use2[codes[dimension*256+(pquad[i].patten[0]*4)+1]] = YES;
		use2[codes[dimension*256+(pquad[i].patten[0]*4)+2]] = YES;
		use2[codes[dimension*256+(pquad[i].patten[0]*4)+3]] = YES;
		break;
	  case	PAT:
		use4[pquad[i].patten[0]] = YES;
		use2[codes[dimension*256+(pquad[i].patten[0]*4)+0]] = YES;
		use2[codes[dimension*256+(pquad[i].patten[0]*4)+1]] = YES;
		use2[codes[dimension*256+(pquad[i].patten[0]*4)+2]] = YES;
		use2[codes[dimension*256+(pquad[i].patten[0]*4)+3]] = YES;
		break;
	  case	CCC:
		use2[pquad[i].patten[1]] = YES;
		use2[pquad[i].patten[2]] = YES;
		use2[pquad[i].patten[3]] = YES;
		use2[pquad[i].patten[4]] = YES;
	  }
	}
  }

  if (!dataStuff) {
	dataStuff=YES;
	RoqInitRoQPatterns();
	if (ParamImSeqCurrentImageHadAlpha(imgSeq)) i = 3584; else i = 2560;
	RoqWriteCodeBookToStream( codes, i, 0);
	for(i=0;i<256;i++) {
	  index2[i] = i;
	  index4[i] = i;
	}
  } else {
	j = 0;
	for(i=0;i<256;i++) {
	  if (use2[i]) {
		index2[i] = j;
		for(dx=0;dx<dimension;dx++) cccList[j*dimension+dx] = codes[i*dimension+dx];
		j++;
	  }
	}
	code = j*dimension;
	direct = j;
	printf("writeFrame: really used %d 2x2 cels\n", j);
	j = 0;
	for(i=0;i<256;i++) {
	  if (use4[i]) {
		index4[i] = j;
		for(dx=0;dx<4;dx++) cccList[j*4+code+dx] = index2[codes[i*4+(dimension*256)+dx]];
		j++;
	  }
	}
	code += j*4;
	direct = (direct<<8) + j;
	printf("writeFrame: really used %d 4x4 cels\n", j);
	if (ParamImSeqCurrentImageHadAlpha(imgSeq)) i = 3584; else i = 2560;
	if ( code == i || j == 256) {
	  RoqWriteCodeBookToStream( codes, i, 0);
	} else {
	  RoqWriteCodeBookToStream( cccList, code, direct);
	}
  }

  action = 0;
  j = onAction = 0;

  for (i=0; i<numQuadCels; i++) {
	if ( pquad[i].size && pquad[i].size < 16 ) {
	  code = -1;
	  switch( pquad[i].status ) {
	  case	DEP:
		code = 3;
		break;
	  case	SLD:
		code = 2;
		cccList[onCCC++] = index4[pquad[i].patten[0]];
		break;
	  case	MOT:
		code = 0;
		break;
	  case	FCC:
		code = 1;
		dx = ((pquad[i].domain >> 8  )) - 128 - dxMean + 8;
		dy = ((pquad[i].domain & 0xff)) - 128 - dyMean + 8;
		if (dx>15 || dx<0 || dy>15 || dy<0 ) {
		  printf("writeFrame: FCC error %d,%d mean %d,%d at %d,%d,%d rmse %f\n", dx,dy, dxMean, dyMean,pquad[i].xat,pquad[i].yat,pquad[i].size, pquad[i].snr[FCC] );
		  exit(1);
		}
		cccList[onCCC++] = (dx<<4)+dy;
		break;
	  case	PAT:
		code = 2;
		cccList[onCCC++] = index4[pquad[i].patten[0]];
		break;
	  case	CCC:
		code = 3;
		cccList[onCCC++] = index2[pquad[i].patten[1]];
		cccList[onCCC++] = index2[pquad[i].patten[2]];
		cccList[onCCC++] = index2[pquad[i].patten[3]];
		cccList[onCCC++] = index2[pquad[i].patten[4]];
		break;
	  case	DEAD:
		fprintf(stderr,"dead cels in picture\n");
		break;
	  }
	  if (code == -1) {
		fprintf(stderr, "writeFrame: an error occurred writing the frame\n");
		exit(2);
	  }

	  action = (action<<2)|code;
	  j++;
	  if (j == 8) {
		j = 0;
		cccList[onAction+0] = (action & 0xff);
		cccList[onAction+1] = ((action >> 8) & 0xff);
		onAction = onCCC;
		onCCC += 2;
	  }
	}
  }

  if (j) {
	action <<= ((8-j)*2);
	cccList[onAction+0] = (action & 0xff);
	cccList[onAction+1] = ((action >> 8) & 0xff);
  }

  direct = RoQ_QUAD_VQ;
	
  RoqWrite16Word(&direct, RoQFile);

  j = onCCC;
  RoqWrite32Word(&j, RoQFile);

  direct  = dyMean;
  direct &= 0xff;
  direct += (dxMean<<8);		// flags

  RoqWrite16Word(&direct, RoQFile);

  printf("writeFrame: outputting %d bytes to RoQ_QUAD_VQ\n", j);

  previousSize = j;
	
  fwrite( cccList, onCCC, 1, RoQFile );

  fflush( RoQFile );

  free( cccList );
  free( use2 );
  free( use4 );

  return 1;
}

int  RoqWritePuzzleFrame( quadcel * pquad )
{
  return 1;
}

//
// load a frame, create a window (if neccesary) and display the frame
//
int  RoqLoadAndDisplayImage(TagTable *newimage )
{
  // unsigned char *imageData;
  // static BOOL cleared = NO;
  
  int pixelsWide;
  int pixelsHigh;
  
  pixelsWide = ZimWidth(newimage);
  pixelsHigh = ZimHeight(newimage);

    
  if (pixelsWide == 800 && pixelsHigh == 600) {
	printf("resizing %dx%d - ", pixelsWide,  pixelsHigh);
	ZimResize(newimage, 512, 512);
  } else if (pixelsWide == 640 && pixelsHigh == 480) {
	printf("resizing %dx%d - ", pixelsWide,  pixelsHigh);
	ZimResize(newimage, 512, 256);
  }

  image = newimage;

  
  pixelsWide = ZimWidth(image);
  pixelsHigh = ZimHeight(image);
  
  numQuadCels  = ((pixelsWide & 0xfff0)*( pixelsHigh & 0xfff0))/(MINSIZE*MINSIZE);
  numQuadCels += numQuadCels/4 + numQuadCels/16;

  if (!quietMode) printf("loadAndDisplayImage: %dx%d\n", pixelsWide,  pixelsHigh);

  return 1;
}

TagTable *  RoqCurrentImage(  )
{
  return	image;
}


int  RoqNumberOfFrames (  )
{
  return numberOfFrames;
}

