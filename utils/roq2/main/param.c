
//
// read a parameter file in (yes I bloddy well had to do this again)
//

#include "Param.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifndef MAXPATHLEN
#include <stdlib.h>
#define MAXPATHLEN _MAX_PATH
#endif 
#ifndef MAXINT
#include <limits.h>
#define MAXINT INT_MAX
#endif

void ParamFree(Param *self)
{
  free(self->soundfile);
  free(self->currentPath);
  free(self->outputFilename);
  free(self->tempFilename);
  free(self->startPal);
  free(self->endPal);
  free(self->currentFile);

  if (self->range)
    {
      free(self->range);
    }
  if (self->padding)
    {
      free(self->padding);
    }
  if (self->padding2)
    {
      free(self->padding2);
    }
  if (self->skipnum)
    {
      free(self->skipnum);
    }
  if (self->skipnum2)
    {
      free(self->skipnum2);
    }
  if (self->startnum)
    {
      free(self->startnum);
    }
  if (self->startnum2)
    {
      free(self->startnum2);
    }
  if (self->endnum)
    {
      free(self->endnum);
    }
  if (self->endnum2)
    {
      free(self->endnum2);
    }
  if (self->numpadding)
    {
      free(self->numpadding);
    }
  if (self->numpadding2)
    {
      free(self->numpadding2);
    }
  if (self->numfiles)
    {
      free(self->numfiles);
    }
  if (self->file)
    {
      free(self->file);
    }
  if (self->file2)
    {
      free(self->file2);
    }

  free(self);
}

Param *ParamNewFromFile(const char *fileName)
{
  FILE *script_fp;
  char arg0[MAXPATHLEN+1];
  char arg1[MAXPATHLEN+1];
  char arg2[MAXPATHLEN+1];
  char arg3[MAXPATHLEN+1];
  char buffer[MAXPATHLEN*4+1];
  int i, readarg;
  Param *self;


  // "c:\\clan\\player\\roq\\debug\\alt_07m.par"
#ifndef WIN32
  if ( ( script_fp = fopen( fileName, "r" ) ) == NULL ) {
	fprintf(stderr,"Error: can't open param file %s\n", fileName);
	exit(1);
  }
#else
  if ( ( script_fp = fopen( fileName, "r+b" ) ) == NULL ) {
	fprintf(stderr,"Error: can't open param file %s\n", fileName);
	exit(1);
  }
#endif
  //printf("initFromFile: %s\n", fileName);

  self = calloc(1, sizeof(Param));


  self->soundfile = calloc( MAXPATHLEN, sizeof(char) );
  self->currentPath = calloc( MAXPATHLEN, sizeof(char) );
  self->outputFilename = calloc( MAXPATHLEN, sizeof(char) );
  self->tempFilename = calloc( MAXPATHLEN, sizeof(char) );
  self->startPal = calloc( MAXPATHLEN, sizeof(char) );
  self->endPal = calloc( MAXPATHLEN, sizeof(char) );
  self->currentFile = calloc( MAXPATHLEN, sizeof(char) );

  self->range = NULL;
  self->padding = NULL;
  self->padding2 = NULL;
  self->skipnum = NULL;
  self->skipnum2 = NULL;
  self->startnum = NULL;
  self->startnum2 = NULL;
  self->endnum = NULL;
  self->endnum2 = NULL;
  self->numpadding = NULL;
  self->numpadding2 = NULL;
  self->numfiles = NULL;
  self->file = NULL;
  self->file2 = NULL;

  self->fullSearch = NO;
  self->scaleDown = NO;
  self->encodeVideo = NO;
  self->addPath = NO;
  self->screenShots = NO;
  self->startPalette = NO;
  self->endPalette = NO;
  self->fixedPalette = NO;
  self->keyColor = NO;
  self->justDelta = NO;
  self->useTimecodeForRange = NO;
  self->onFrame = 0;
  self->numInputFiles = 0;
  self->currentPath[0] = '\0';
  self->makeVectors = NO;
  self->justDeltaFlag = NO;
  self->noAlphaAtAll = NO;
  self->hasSound = NO;
  self->isScaleable = NO;
  self->firstframesize = 56*1024;
  self->normalframesize = 16384;
  self->jpegDefault = 85;
  self->fadeDownStartFrame = MAXINT;
  self->fadeDownDuration = 0;
  self->fadeUpStartFrame = 0;
  self->fadeUpDuration = 0;
  self->mpegCropWidth = 0;
  self->mpegCropHeight = 0;
  self->mpegCropXOrigin = 0;
  self->mpegCropYOrigin = 0; // just worked out that way for test mpegs
  self->fps = 30; 
  
  self->realnum = 0;
  do
	{
	  buffer[0] = '\0';
	  arg0[0] = '\0';
	  fgets( buffer, 256, script_fp );
	  if (buffer[strlen(buffer)-2] == '\r') //strip \r
		{
		  buffer[strlen(buffer)-2] = '\n';
		  buffer[strlen(buffer)-1] = '\0';
		}
	  sscanf(buffer, "%s %s %s %s", arg0, arg1, arg2, arg3);
		
	  readarg = 0;
	  // input dir
		if (strcasecmp(arg0, "input_dir") == 0) {
		  self->addPath = YES;
		  strcpy( self->currentPath, arg1 );
		  //			[textDirectory setStringValue: "hello world"];
		  //			fprintf(stderr,"  + input directory is %s\n", self->currentPath );
		  readarg++;
		}
	  // input dir
		if (strcasecmp(arg0, "scale_down") == 0) {
		  self->scaleDown = YES;
		  //			fprintf(stderr,"  + scaling down input\n" );
		  readarg++;
		}
	  // full search
		if (strcasecmp(arg0, "fullsearch") == 0) {
		  self->normalframesize += self->normalframesize/2;
		  self->fullSearch = YES;
		  readarg++;
		}
	  // scaleable
		if (strcasecmp(arg0, "scaleable") == 0) {
		  self->isScaleable = YES;
		  readarg++;
		}
	  // input dir
		if (strcasecmp(arg0, "no_alpha") == 0) {
		  self->noAlphaAtAll = YES;
		  //			fprintf(stderr,"  + scaling down input\n" );
		  readarg++;
		}
	  //timecode range
		if (strcasecmp(arg0, "timecode") == 0) {
		  self->useTimecodeForRange = YES;
		  self->firstframesize = 12*1024;
		  self->normalframesize = 4500;
		  if (self->mpegCropWidth == 0)
			{
			  self->mpegCropWidth = 640;
			}
		  if (self->mpegCropHeight == 0)
			{
			  self->mpegCropHeight = 160;
			}
		  if (self->mpegCropXOrigin == 0)
			{
			  self->mpegCropXOrigin = (704-640)/2;
			}
		  if (self->mpegCropYOrigin == 0)
			{
			  self->mpegCropYOrigin = (240-160)/2 + 1; // just worked out that way for test mpegs
			}
  		  //			fprintf(stderr,"  + Using timecode as range\n");
		}
	  // soundfile for making a .RnR
		if (strcasecmp(arg0, "sound") == 0) {
		  strcpy(self->soundfile,arg1);
		  self->hasSound = YES;
		  //			fprintf(stderr,"  + Using timecode as range\n");
		}
	  // soundfile for making a .RnR
		if (strcasecmp(arg0, "has_sound") == 0) {
		  self->hasSound = YES;
		}
	  // outfile	
		if (strcasecmp(arg0, "filename") == 0) {
		  strcpy( self->outputFilename, arg1 );
		  i = strlen(self->outputFilename);
		  if (self->outputFilename[i-3] == 'v' && self->outputFilename[i-2] == 'd' && self->outputFilename[i-1] == 'x') {
			//				fprintf(stderr,"correcting file extension to .RoQ\n");
			self->outputFilename[i-3] = 'R';
			self->outputFilename[i-2] = 'o';
			self->outputFilename[i-1] = 'Q';
		  }
		  //			fprintf(stderr,"  + output file is %s\n", self->outputFilename );
		  readarg++;
		}
	  // starting palette
		if (strcasecmp(arg0, "start_palette") == 0) {
		  sprintf(self->startPal, "/LocalLibrary/vdxPalettes/%s", arg1);
		  //			fprintf(stderr,"  + starting palette is %s\n", self->startPal );
		  self->startPalette = YES;
		  readarg++;
		}
	  // ending palette
		if (strcasecmp(arg0, "end_palette") == 0) {
		  sprintf(self->endPal, "/LocalLibrary/vdxPalettes/%s", arg1);
		  //			fprintf(stderr,"  + ending palette is %s\n", self->endPal );
		  self->endPalette = YES;
		  readarg++;
		}
	  // fixed palette
		if (strcasecmp(arg0, "fixed_palette") == 0) {
		  sprintf(self->startPal, "/LocalLibrary/vdxPalettes/%s", arg1);
		  //			fprintf(stderr,"  + fixed palette is %s\n", self->startPal );
		  self->fixedPalette = YES;
		  readarg++;
		}
	  // these are screen shots
		if (strcasecmp(arg0, "screenshot") == 0) {
		  //			fprintf(stderr,"  + shooting screen shots\n" );
		  self->screenShots = YES;
		  readarg++;
		}
	  //	key_color	r g b	
		if (strcasecmp(arg0, "key_color") == 0) {
		  self->keyR = atoi(arg1);
		  self->keyG = atoi(arg2);
		  self->keyB = atoi(arg3);
		  self->keyColor = YES;
		  //			fprintf(stderr,"  + key color is %03d %03d %03d\n", self->keyR, self->keyG, self->keyB );
		  readarg++;
		}
	  // only want deltas
		if (strcasecmp(arg0, "just_delta") == 0) {
		  //			fprintf(stderr,"  + outputting deltas in the night\n" );
		  //			self->justDelta = YES;
		  //			self->justDeltaFlag = YES;
		  readarg++;
		}
	  // makes codebook vector tables
		if (strcasecmp(arg0, "codebook") == 0) {
		  self->makeVectors = YES;
		  readarg++;
		}
	  // set first frame size
		if (strcasecmp(arg0, "firstframesize") == 0) {
		  self->firstframesize = atoi( arg1 );
		  readarg++;
		}
	  // set normal frame size
		if (strcasecmp(arg0, "normalframesize") == 0) {
		  self->normalframesize = atoi( arg1 );
		  readarg++;
		}
	  // set still frame quality
		if (strcasecmp(arg0, "stillframequality") == 0) {
		  self->jpegDefault = atoi( arg1 );
		  readarg++;
		}
	  // set fade down start frame
		if (strcasecmp(arg0, "fadeDownStartFrame") == 0) {
		  self->fadeDownStartFrame = atoi( arg1 );
		  readarg++;
		}
	  // set fade down duration 
		if (strcasecmp(arg0, "fadeDownDuration") == 0) {
		  self->fadeDownDuration = atoi( arg1 );
		  readarg++;
		}
	  // set fade up start frame
		if (strcasecmp(arg0, "fadeUpStartFrame") == 0) {
		  self->fadeUpStartFrame = atoi( arg1 );
		  readarg++;
		}
	  // set fade up duration 
		if (strcasecmp(arg0, "fadeUpDuration") == 0) {
		  self->fadeUpDuration = atoi( arg1 );
		  readarg++;
		}
	  // set mpeg crop width
		if (strcasecmp(arg0, "mpegCropWidth") == 0) {
		  self->mpegCropWidth = atoi( arg1 );
		  readarg++;
		}
	  // set mpeg crop height
		if (strcasecmp(arg0, "mpegCropHeight") == 0) {
		  self->mpegCropHeight = atoi( arg1 );
		  readarg++;
		}
	  // set mpeg x origin
		if (strcasecmp(arg0, "mpegCropXOrigin") == 0) {
		  self->mpegCropXOrigin = atoi( arg1 );
		  readarg++;
		}
	  // set mpeg y origin
		if (strcasecmp(arg0, "mpegCropYOrigin") == 0) {
		  self->mpegCropYOrigin = atoi( arg1 );
		  readarg++;
		}
	  // set frames per second
		if (strcasecmp(arg0, "fps") == 0) {
		  self->fps = atoi( arg1 );
		  readarg++;
		}
	  if (strcasecmp(arg0, "input") == 0) {
		int num_files = 0;
		long filepos = ftell( script_fp );

		do {
		  fgets( buffer, 256, script_fp );
		  if (buffer[strlen(buffer)-2] == '\r') //strip \r
			{
			  buffer[strlen(buffer)-2] = '\n';
			  buffer[strlen(buffer)-1] = '\0';
			}
		  sscanf(buffer, "%s %s %s %s", arg0, arg1, arg2, arg3);
		  num_files++;
		} while	( strcasecmp(arg0, "end_input") != 0 );

		fseek( script_fp, filepos, SEEK_SET );				
		//printf("roqParam: reading %d lines from param file\n", num_files);

		self->range = calloc( num_files, sizeof(int) );
		self->padding = calloc( num_files, sizeof(BOOL) );
		self->padding2 = calloc( num_files, sizeof(BOOL) );
		self->skipnum = calloc( num_files, sizeof(int) );
		self->skipnum2 = calloc( num_files, sizeof(int) );
		self->startnum = calloc( num_files, sizeof(int) );
		self->startnum2 = calloc( num_files, sizeof(int) );
		self->endnum = calloc( num_files, sizeof(int) );
		self->endnum2 = calloc( num_files, sizeof(int) );
		self->numpadding = calloc( num_files, sizeof(int) );
		self->numpadding2 = calloc( num_files ,sizeof(int) );
		self->numfiles = calloc( num_files, sizeof(int) );
		self->file  = calloc( num_files, sizeof(char *) );
		self->file2 = calloc( num_files, sizeof(char *) );

		for(i=0;i<num_files;i++) {
		  self->file[i] = NULL;
		  self->file2[i] = NULL;
		}

		self->field = 0;
		self->realnum = 0;
		do {	
		  buffer[0] = '\0';
		  arg0[0] = '\0';
		  arg1[0] = '\0';
		  arg2[0] = '\0';
		  arg3[0] = '\0';
		  fgets( buffer, 256, script_fp );
		  if (buffer[strlen(buffer)-2] == '\r') //strip \r
			{
			  buffer[strlen(buffer)-2] = '\n';
			  buffer[strlen(buffer)-1] = '\0';
			}
		  sscanf(buffer, "%s %s %s %s", arg0, arg1, arg2, arg3);
		  if ( arg0[0] && strcasecmp(arg0, "end_input") != 0 ) {
			if ( !self->file[self->field] ) self->file[self->field] = calloc( MAXPATHLEN, sizeof(char) );
			strcpy( self->file[self->field], arg0 );
			if ( !arg1[0] ) {
			  //						fprintf(stderr,"  + reading %s\n", self->file[self->field] );
			  self->range[self->field] = 0;
			  self->numfiles[self->field] = 1;
			  self->realnum++;
			}
			else {
			  if (arg1[0] == '[')
				{
				  self->range[self->field] = 1;
				  if (self->useTimecodeForRange)  {
					self->realnum += parseTimecodeRange(arg1, self->field, self->skipnum, self->startnum, self->endnum, self->numfiles, self->padding, self->numpadding);
					//								fprintf(stderr,"  + reading %s from %d to %d\n", self->file[self->field], self->startnum[self->field], self->endnum[self->field]);
				  }
				  else {
					self->realnum += parseRange(arg1, self->field, self->skipnum, self->startnum, self->endnum, self->numfiles, self->padding, self->numpadding);
					//								fprintf(stderr,"  + reading %s from %d to %d\n", self->file[self->field], self->startnum[self->field], self->endnum[self->field]);
				  }
				}
			  else if (( arg1[0] != '[' ) && (arg2[0] == '[') && (arg3[0] =='[')) {  //a double ranger...
																					   int files1,files2;
							
																					   if ( !self->file2[self->field] ) self->file2[self->field] = calloc( MAXPATHLEN, sizeof(char) );
																					   strcpy(self->file2[self->field],arg1);
																					   self->range[self->field] = 2;
																					   files1 = parseRange(arg2, self->field, self->skipnum, self->startnum, self->endnum, self->numfiles, self->padding, self->numpadding);
																					   //							fprintf(stderr,"  + reading %s from %d to %d\n", self->file[self->field], self->startnum[self->field], self->endnum[self->field]);
																					   files2 = parseRange(arg3, self->field, self->skipnum2, self->startnum2, self->endnum2, self->numfiles, self->padding2, self->numpadding2);
																					   //							fprintf(stderr,"  + reading %s from %d to %d\n", self->file2[self->field], self->startnum2[self->field], self->endnum2[self->field]);
																					   if (files1 != files2) {
																						 //								fprintf(stderr,"You had %d files for %s and %d for %s!",files1,arg0,files2,arg1);
																						 exit(1);
																					   }
																					   else	{
																						 self->realnum += files1;//not both, they are parallel
																					   }
																					 }
			  else	{
				fprintf(stderr,"Error: invalid range on open (%s %s %s)\n", arg1,arg2,arg3 );
				exit(1);
			  }
			} 					self->field++;
		  }
		} while (strcasecmp(arg0, "end_input") != 0);
	  }
	}while ( !feof( script_fp ) );
		

  self->numInputFiles = self->realnum;
  //fprintf(stderr,"  + reading a total of %d frames in %s\n", self->numInputFiles, self->currentPath );
  fclose(script_fp);

  if (self->mpegCropWidth == 0)
	{
	  self->mpegCropWidth = 512;
	}
  if (self->mpegCropHeight == 0)
	{
	  self->mpegCropHeight = 256;
	}
  if (self->mpegCropXOrigin == 0)
	{
	  self->mpegCropXOrigin = (704-512)/2;
	}
  if (self->mpegCropYOrigin == 0)
	{
	  self->mpegCropYOrigin = (480-256)/2 -2; // just worked out that way for test mpegs
	}
  
  return self;
}

void ParamGetCurrentRange(Param *self, int * start, int *end,int * skip,BOOL incr)
{
	int i, myfield;
	
	i = 0;
	myfield = 0;
	
	while (i <= self->onFrame) {
		i += self->numfiles[myfield++];
	}
	myfield--;
	i -= self->numfiles[myfield];
	
	if ( self->range[myfield] == 1 ) {
		*start = self->startnum[myfield];
		*end = self->endnum[myfield];
		*skip = self->skipnum[myfield];
		if (incr)
		{
			self->onFrame += self->numfiles[myfield] -1;
		}
	}
}

void ParamGetNthInputFileName(Param *self, byte* fileName, int n)
{
int i, myfield, index,hrs,mins,secs,frs;
char tempfile[33], left[256], right[256], *strp;
	if ( n > self->realnum ) n = self->realnum;
	
	i = 0;
	myfield = 0;
	
	while (i <= n) {
		i += self->numfiles[myfield++];
	}
	myfield--;
	i -= self->numfiles[myfield];
	
	if ( self->range[myfield] == 1 ) {
		
		strcpy( left, self->file[myfield] );
		strp = strstr( left, "*" );
		*strp++ = 0;
		sprintf(right, "%s", strp);
		
		if ( self->startnum[myfield] <= self->endnum[myfield] ) {
			index = self->startnum[myfield] + ((n-i)*self->skipnum[myfield]);
		} else {
			index = self->startnum[myfield] - ((n-i)*self->skipnum[myfield]);
		}
		
		if ( self->padding[myfield] == YES ) {
			if (self->useTimecodeForRange) {
				hrs = index/(30*60*60) ;
				mins = (index/(30*60)) %60;
				secs = (index/(30)) % 60;
				frs = index % 30;
				sprintf(fileName,"%s%.02d%.02d/%.02d%.02d%.02d%.02d%s",left,hrs,mins,hrs,mins,secs,frs,right);
			}
			else  {
				sprintf(tempfile, "%032d", index );
				sprintf(fileName, "%s%s%s", left, &tempfile[ 32-self->numpadding[myfield] ], right );
			}
		} else {
			if (self->useTimecodeForRange) {
				hrs = index/(30*60*60) ;
				mins = (index/(30*60)) %60;
				secs = (index/(30)) % 60;
				frs = index % 30;
				sprintf(fileName,"%s%.02d%.02d/%.02d%.02d%.02d%.02d%s",left,hrs,mins,hrs,mins,secs,frs,right);
			}
			else  {
				sprintf(fileName, "%s%d%s", left, index, right );
			}
		}
	} else if ( self->range[myfield] == 2 ) {
		
		strcpy( left, self->file[myfield] );
		strp = strstr( left, "*" );
		*strp++ = 0;
		sprintf(right, "%s", strp);
		
		if ( self->startnum[myfield] <= self->endnum[myfield] ) {
			index = self->startnum[myfield] + ((n-i)*self->skipnum[myfield]);
		} else {
			index = self->startnum[myfield] - ((n-i)*self->skipnum[myfield]);
		}
		
		if ( self->padding[myfield] == YES ) {
			sprintf(tempfile, "%032d", index );
			sprintf(fileName, "%s%s%s", left, &tempfile[ 32-self->numpadding[myfield] ], right );
		} else {
			sprintf(fileName, "%s%d%s", left, index, right );
		}

		strcpy( left, self->file2[myfield] );
		strp = strstr( left, "*" );
		*strp++ = 0;
		sprintf(right, "%s", strp);
		
		if ( self->startnum2[myfield] <= self->endnum2[myfield] ) {
			index = self->startnum2[myfield] + ((n-i)*self->skipnum2[myfield]);
		} else {
			index = self->startnum2[myfield] - ((n-i)*self->skipnum2[myfield]);
		}
		
		if ( self->padding2[myfield] == YES ) {
			sprintf(tempfile, "%032d", index );
			sprintf(fileName+strlen(fileName), "\n%s%s%s", left, &tempfile[ 32-self->numpadding2[myfield] ], right );
		} else {
			sprintf(fileName+strlen(fileName), "\n%s%d%s", left, index, right );
		}
	} else {
		strcpy( fileName, self->file[myfield] );
	}
}

const char*ParamGetNextImageFilename(Param *self)
{
  char tempBuffer[MAXPATHLEN*2+1];
  char *hey;
  
  ParamGetNthInputFileName(self, tempBuffer, self->onFrame++);
  if ( self->justDeltaFlag == YES ) {
	self->onFrame--;
	self->justDeltaFlag = NO;
  }
  
  if ( self->addPath == YES ) {
	sprintf(self->currentFile, "%s\\%s", self->currentPath, tempBuffer );
	if (hey = strchr(self->currentFile,'\n'))  //two files in there
	  {
		*hey++ = '\0';
		sprintf(tempBuffer,"%s\n%s\\%s",self->currentFile,self->currentPath,hey);
		strcpy(self->currentFile,tempBuffer);
	  }
  } else {
	sprintf(self->currentFile, "%s", tempBuffer );
  }
  
  return self->currentFile;
}

const char *ParamRoqFilename(Param *self)
{
	return self->outputFilename;
}

const char *ParamSoundFilename(Param *self)
{
	return self->soundfile;
}

const char *ParamRoqTempFilename(Param *self)
{
unsigned int i, j;
	j = 0;
	for(i=0; i<strlen( self->outputFilename ); i++ ){
		if ( self->outputFilename[i] == '/' ) j = i;
		if ( self->outputFilename[i] == '\\' ) j = i;
	}
#ifndef WIN32
	sprintf(self->tempFilename, "/tmp/%s_temp", &self->outputFilename[j+1] );
#else
	sprintf(self->tempFilename, "C:\\%s_temp", &self->outputFilename[j+1] );
#endif
	return self->tempFilename;
}


int ParamMoveFile(Param *self)
{
char temp[MAXPATHLEN*2+10];
int res, mess;
FILE *test, *outfp;
byte	*buffer;

	sprintf(temp, "%s.bak",self->outputFilename );
	if ((test=fopen(temp,"rb"))==NULL) {
		if ((test=fopen(self->outputFilename,"rb"))!=NULL) {
			fclose(test);
			fprintf(stderr,"file %s does not exist, making backup\n", temp);
			sprintf(temp, "%s.bak", self->outputFilename );
			res = rename( self->outputFilename, temp );
			if (res)
			{
				fprintf(stderr,"%s\rename returned non-zero status: %d\n",temp,res);
			}
		}
	} else {
		fclose(test);
	}
#define BUFFER_COPY_SIZE (1024*1024)
	buffer = malloc( BUFFER_COPY_SIZE );
	if (!buffer) {
		fprintf(stderr,"malloc failed moving %s\n",self->tempFilename);
		exit(1);
	}
	test  = fopen( self->tempFilename, "rb" );
	outfp = fopen( self->outputFilename, "wb" );
	if (!test||!outfp) {
		fprintf(stderr,"fopen could not open files\n");
		exit(1);
	}
		
	do {
		res = fread( buffer, 1, BUFFER_COPY_SIZE, test);
		mess = fwrite( buffer, 1, res, outfp );
		if (res != mess) { 
			fprintf(stderr, "Could not write to output stream %s\n",self->outputFilename); 
			exit(1); 
		}
	} while ( res == BUFFER_COPY_SIZE );

	fclose( test );
	fclose( outfp );

	unlink( self->tempFilename );
	free( buffer );

	return 1;
}

BOOL ParamTimecode(Param *self)
{
	return self->useTimecodeForRange;
}

BOOL outputVectors(Param *self)
{
	return self->makeVectors;
}

BOOL ParamHasSound(Param *self)
{
	return self->hasSound;
}

BOOL ParamDeltaFrames(Param *self)
{
	return self->justDelta;
}

BOOL ParamNoAlpha(Param *self)
{
	return self->noAlphaAtAll;
}

BOOL ParamSearchType(Param *self)
{
	return self->fullSearch;
}

BOOL ParamMoreFrames(Param *self)
{
	if (self->onFrame < self->numInputFiles) {
		return YES;
	} else {
		return NO;
	}
}

int ParamNumberOfFrames(Param *self)
{
	return self->numInputFiles;
}

int ParamFirstframesize(Param *self)
{
	return self->firstframesize;
}

int ParamNormalframesize(Param *self)
{
	return self->normalframesize;
}

BOOL ParamScaleDown(Param *self)
{
  return self->scaleDown;
}


BOOL ParamIsScaleable(Param *self)
{
	return self->isScaleable;
}

int ParamJpegQuality(Param *self)
{
	return	self->jpegDefault;
}

int ParamMpegCropWidth(Param *self)
{
	return	self->mpegCropWidth;
}
int ParamMpegCropHeight(Param *self)
{
	return	self->mpegCropHeight;
}
int ParamMpegCropXOrigin(Param *self)
{
	return	self->mpegCropXOrigin;
}
int ParamMpegCropYOrigin(Param *self)
{
	return	self->mpegCropYOrigin;
}

double ParamFadeFromFrame(Param *self, int frame)
{
 double currentFade = 1.0;
 int framesIntoFade = 0;
 
  if (frame >= self->fadeDownStartFrame)
	{
	  framesIntoFade = frame - self->fadeDownStartFrame;
	  if (framesIntoFade >= self->fadeDownDuration)
		{
		  currentFade = 0.0;
		}
	  else
		{
		  if (self->fadeDownDuration == 0)
			{
			  currentFade = 0.0;// a zero frame fade
			}
		  else
			{
			  currentFade = 1.0 - (double) framesIntoFade/ (double) self->fadeDownDuration;
			}
		}
	}
  else if ((frame < self->fadeUpStartFrame))
	{
	  currentFade = 0.0;
	}
  else if (frame >= self->fadeUpStartFrame)
	{
	  framesIntoFade = frame - self->fadeUpStartFrame;
	  if (framesIntoFade >= self->fadeUpDuration)
		{
		  currentFade = 1.0;
		}
	  else
		{
		  if (self->fadeUpDuration == 0)
			{
			  currentFade = 1.0;// a zero frame fade
			}
		  else
			{
			  currentFade = (double) framesIntoFade/ (double) self->fadeUpDuration;
			}
		}
	}
	return currentFade;
}

int parseRange(char *rangeStr,int field, int skipnum[], int startnum[], int endnum[],int numfiles[],BOOL padding[],int numpadding[] )
{
	char start[64], end[64], skip[64];
	char *stptr, *enptr, *skptr;
	int i,realnum;

	i = 1;
	realnum = 0;
	stptr = start;
	enptr = end;
	skptr = skip;
	do {
		*stptr++ = rangeStr[i++];
	} while ( rangeStr[i] >= '0' && rangeStr[i] <= '9' );
	*stptr = '\0';
	if ( rangeStr[i++] != '-' ) {
		fprintf(stderr,"Error: invalid range on middle \n");
		exit(1);
	}
	do {
		*enptr++ = rangeStr[i++];
	} while ( rangeStr[i] >= '0' && rangeStr[i] <= '9' );
	*enptr = '\0';
	if ( rangeStr[i] != ']' ) {
		if ( rangeStr[i++] != '+' ) {
			fprintf(stderr,"Error: invalid range on close\n");
			exit(1);
		}
		do {
			*skptr++ = rangeStr[i++];
		} while ( rangeStr[i] >= '0' && rangeStr[i] <= '9' );
		*skptr = '\0';
		skipnum[field] = atoi( skip );
	} else {
		skipnum[field] = 1;
	}
	startnum[field] = atoi( start );
	endnum[field] = atoi( end );
	numfiles[field] = (abs( startnum[field] - endnum[field] ) / skipnum[field]) + 1;
	realnum += numfiles[field];
	if ( start[0] == '0' && start[1] != '\0' ) {
		padding[field] = YES;
		numpadding[field] = strlen( start );
	} else {
		padding[field] = NO;
	}
	return realnum;
}

int parseTimecodeRange(char *rangeStr,int field, int skipnum[], int startnum[], int endnum[],int numfiles[],BOOL padding[],int numpadding[] )
{
	char start[64], end[64], skip[64];
	char *stptr, *enptr, *skptr;
	int i,realnum,hrs,mins,secs,frs;

	i = 1;//skip the '['
	realnum = 0;
	stptr = start;
	enptr = end;
	skptr = skip;
	do {
		*stptr++ = rangeStr[i++];
	} while ( rangeStr[i] >= '0' && rangeStr[i] <= '9' );
	*stptr = '\0';
	if ( rangeStr[i++] != '-' ) {
		fprintf(stderr,"Error: invalid range on middle \n");
		exit(1);
	}
	do {
		*enptr++ = rangeStr[i++];
	} while ( rangeStr[i] >= '0' && rangeStr[i] <= '9' );
	*enptr = '\0';
	if ( rangeStr[i] != ']' ) {
		if ( rangeStr[i++] != '+' ) {
			fprintf(stderr,"Error: invalid range on close\n");
			exit(1);
		}
		do {
			*skptr++ = rangeStr[i++];
		} while ( rangeStr[i] >= '0' && rangeStr[i] <= '9' );
		*skptr = '\0';
		skipnum[field] = atoi( skip );
	} else {
		skipnum[field] = 1;
	}
	sscanf(start,"%2d%2d%2d%2d",&hrs,&mins,&secs,&frs);
	startnum[field] = hrs*30*60*60 + mins *60*30 + secs*30 +frs;
	sscanf(end,"%2d%2d%2d%2d",&hrs,&mins,&secs,&frs);
	endnum[field] = hrs*30*60*60 + mins *60*30 + secs*30 +frs;
	numfiles[field] = (abs( startnum[field] - endnum[field] ) / skipnum[field]) + 1;
	realnum += numfiles[field];
	if ( start[0] == '0' && start[1] != '\0' ) {
		padding[field] = YES;
		numpadding[field] = strlen( start );
	} else {
		padding[field] = NO;
	}
	return realnum;
}



