
#include "codec.h"
//#include "libc.h"
#include "Zim.h"
#include "math.h"
#include "ParamImSeq.h"

static TagTable *image;
static TagTable *newImage;
static TagTable *previousImage[2];		// the ones in video ram and offscreen ram
static int					numQuadCels;
static int					whichFrame;
static BOOL 				detail;
static int 				onQuad;
static int					initRGBtab;
static byte				luty[256];
static byte				*luti;
static quadcel 			*qStatus;
static int					dxMean;
static int					dyMean; 
static int					codebooksize;
static int					index2[256];
static int					overAmount;
static VQDATA				**codebook2;
static VQDATA				norm2[256];
static int					index4[256];
static VQDATA				**codebook4;
static VQDATA				norm4[256];
static int					pixelsWide;
static int					pixelsHigh;
static int					codebookmade;
static BOOL				used2[256];
static BOOL				used4[256];
static int					dimension2;
static int					dimension4;
static int                 currentTotalBits;
static int                 sizeQuickAdd[DEAD+1];


int  CodecInit( )
{
  int i;

  printf("init: initing.....\n");
  codebooksize = 256;
  codebook2 = (VQDATA **) calloc(256,sizeof(VQDATA *));
  for(i=0; i < 256; i++) {
	codebook2[i] = (VQDATA *) calloc(16,sizeof(VQDATA));
  }

  codebook4 = (VQDATA **) calloc(256,sizeof(VQDATA *));
  for(i=0; i < 256; i++) {
	codebook4[i] = (VQDATA *) calloc(64,sizeof(VQDATA));
  }
  previousImage[0] = 0;
  image = NULL;
  whichFrame = 0;
  qStatus = 0;
  luti = 0;
  overAmount = 0;
  codebookmade = 0;

  sizeQuickAdd[DEP] = 2;
  sizeQuickAdd[SLD] = 10;
  sizeQuickAdd[PAT] = 10;
  sizeQuickAdd[CCC] = 34;
  sizeQuickAdd[MOT] = 2;
  sizeQuickAdd[FCC] = 10;
  sizeQuickAdd[DEAD] = 0;
  currentTotalBits = 0;

  if ( RoqIsQuiet() == NO)
    {
	  unlink("/LocalLibrary/RoQFiles/previousFrame.tiff");
	  unlink("/LocalLibrary/RoQFiles/previousPreviousFrame.tiff");
	}
  return 1;
}

int  CodecFree(  )
{
  printf("free: resetting\n");
  if (qStatus) free(qStatus);
  if (luti) free(luti);
  if (previousImage[0]) ZimFree(previousImage[0]);
  if (previousImage[1]) ZimFree(previousImage[1]);
  qStatus = 0;
  initRGBtab = 0;
  previousImage[0] = 0;
  whichFrame = 0;
  luti = 0;
  return 1;
}

/* Because Shellsort is a variation on Insertion Sort, it has the same 
 * inconsistency that I noted in the InsertionSort class.  Notice where I 
 * subtract a move to compensate for calling a swap for visual purposes.
 */

int  CodecSort( float * list ,int * intIndex ,int numElements )
{
#define STRIDE_FACTOR 3 	// good value for stride factor is not well-understood
  // 3 is a fairly good choice (Sedgewick)
    int c,d, stride;
  BOOL found;

  stride = 1;
  while (stride <= numElements)
	stride = stride*STRIDE_FACTOR +1;
    
  while (stride>(STRIDE_FACTOR-1)) { // loop to sort for each value of stride
									   stride = stride / STRIDE_FACTOR;

									   for (c = stride; c < numElements; c++){
										 found = NO;
										 d = c-stride;
										 while ((d >= 0) && !found) { // move to left until correct place
																		if (list[d]<list[d+stride]) {
																		  float ftemp;
																		  int itemp;
																		  ftemp = list[d]; list[d] = list[d+stride]; list[d+stride] = ftemp;
																		  itemp = intIndex[d]; intIndex[d] = intIndex[d+stride]; intIndex[d+stride] = itemp;
																		  d -= stride;		// jump by stride factor
																		} else
																		  found = YES;
																	  }
									   }
									 }
  return 1;
}

int  CodecSegment( int * alist , float * flist ,int numElements ,float rmse )
{
  int x, y, yy, numc, onf, *ilist, index, temp, best, a0, a1, a2, a3;
  byte	find[16], *lineout;
  float	fy, fcr, fcb;
  FILE *fpcb;
  char cbFile[MAXPATHLEN], tempcb[MAXPATHLEN];
  BOOL doopen, dosave;
  float y0,y1,y2,y3,cr,cb;

  doopen = NO;
  dosave = YES;
  a0 = a1 = a2 = a3 = 0;

  sprintf( tempcb, "%s.cb", RoqCurrentFilename());

  onf = 0;
  for(x=0;x<strlen(tempcb);x++) {
	if (tempcb[x] == '\n') for(y=x;y<strlen(tempcb);y++) if (tempcb[y] == '/') x = y+1;
	cbFile[onf++] = tempcb[x];
  }
  cbFile[onf] = 0;

  lineout = malloc( 4*1024 );
  ilist = calloc( (numQuadCels+1)*4, sizeof(int) );

  if ((fpcb = fopen( cbFile, "rb")) == NULL) {
	doopen = YES;
  } else {
	if ( dimension2 == 16 ) x = 3584; else x = 2560;
	if ( (fread( lineout, 1, x, fpcb)) != x ) doopen = YES;
	fclose( fpcb );
  }
  if (RoqCurrentFade() != 1.0)  //can't use pre-generated codebooks for images that are faded 
  {
	  printf("Ignoring saved codebook because of fade = %f \n",RoqCurrentFade());
  	doopen = YES;
	dosave = NO;
  }

  if ( doopen ) {
	printf("segment: making %s\n", cbFile);
	numc = numElements;
	if (numElements > numc) numc = numElements;
	onf = 0;

	for(x=0;x<numc;x++) {
	  ilist[onf++] = (qStatus[alist[x]].yat<<16)+(qStatus[alist[x]].xat);
	  ilist[onf++] = (qStatus[alist[x]].yat<<16)+(qStatus[alist[x]].xat+4);
	  ilist[onf++] = ((qStatus[alist[x]].yat+4)<<16)+(qStatus[alist[x]].xat);
	  ilist[onf++] = ((qStatus[alist[x]].yat+4)<<16)+(qStatus[alist[x]].xat+4);
	}

	if (RoqIsQuiet() == NO) {
	  printf("segment: making a new codebook, number of cels to VQ = %d (out of %d)\n", onf, numElements*4);
	}

	for(x=0;x<256;x++) {
	  for(y=0;y<dimension2;y++) codebook2[x][y] = 0;
	  for(y=0;y<dimension4;y++) codebook4[x][y] = 0;
	}
	y = ZimBpp(image) * ZimWidth(image);
	stdvq( dimension4, 256, ZimData(image), y, ilist, onf, codebook4, NO, flist );
	stdvq( dimension2, 256, ZimData(image), y, ilist, onf, codebook2, NO, flist );

	CodecPrepareCodeBook();

	index = 0;
	for(onf=0;onf<256;onf++) {
	  numc = 0; fcr = fcb = 0;
	  for(x=0;x<4;x++) {
		fy  = RMULT*codebook2[onf][numc+0];
		fy += GMULT*codebook2[onf][numc+1];
		fy += BMULT*codebook2[onf][numc+2];
		fy  = glimit( fy+0.5 );
		fcr += RIEMULT*codebook2[onf][numc+0]; 
		fcr += GIEMULT*codebook2[onf][numc+1];
		fcr += BIEMULT*codebook2[onf][numc+2];
		fcb += RQEMULT*codebook2[onf][numc+0]; 
		fcb += GQEMULT*codebook2[onf][numc+1];
		fcb += BQEMULT*codebook2[onf][numc+2];
		lineout[index++] = (byte)fy;
		if ( dimension2 == 16 ) {
		  lineout[index++] = (byte)codebook2[onf][numc+3];
		  numc += 4;
		} else {
		  numc += 3;
		}
	  }
	  fcr = glimit((fcr/4)+128.5);
	  fcb = glimit((fcb/4)+128.5);
	  lineout[index++] = (byte)fcr;
	  lineout[index++] = (byte)fcb;
	}

	for(onf=0;onf<256;onf++) {
	  for(y=0;y<4;y+=2) {
		for(x=0;x<4;x+=2) {
		  numc = 0;
		  for(yy=y;yy<(y+2);yy++) {
			temp = (yy*dimension2)+x*(dimension2/4);
			find[numc++] = (byte)(codebook4[onf][temp+0] + 0.50);
			find[numc++] = (byte)(codebook4[onf][temp+1] + 0.50);
			find[numc++] = (byte)(codebook4[onf][temp+2] + 0.50);
			find[numc++] = (byte)(codebook4[onf][temp+3] + 0.50);
			find[numc++] = (byte)(codebook4[onf][temp+4] + 0.50);
			find[numc++] = (byte)(codebook4[onf][temp+5] + 0.50);
			if ( dimension2 == 16 ) {
			  find[numc++] = (byte)(codebook4[onf][temp+6] + 0.50);
			  find[numc++] = (byte)(codebook4[onf][temp+7] + 0.50);
			}
		  }
		  lineout[index++] = CodecBestCodeword(find, dimension2, codebook2, norm2, index2);
		}
	  }
	}

	printf("made up %d entries\n", index);
	if (dosave)
	{
		fpcb = fopen( cbFile, "wb" );
		fwrite( lineout, index, 1, fpcb );
		fclose( fpcb );
		printf("finished write\n");
	}
  }

  for(y=0;y<256;y++) {
	if ( dimension2 == 16 ) x = y*10; else x = y*6;
	y0 = (float)lineout[x++];
	if ( dimension2 == 16 ) a0 = (int)lineout[x++];
	y1 = (float)lineout[x++];
	if ( dimension2 == 16 ) a1 = (int)lineout[x++];
	y2 = (float)lineout[x++];
	if ( dimension2 == 16 ) a2 = (int)lineout[x++];
	y3 = (float)lineout[x++];
	if ( dimension2 == 16 ) a3 = (int)lineout[x++];
	cb = (float)lineout[x++]; cb -= 128;
	cr = (float)lineout[x  ]; cr -= 128;
	x = 0;
	codebook2[y][x++] = glimit( y0 + 1.40200*cr );
	codebook2[y][x++] = glimit( y0 - 0.34414*cb - 0.71414*cr );
	codebook2[y][x++] = glimit( y0 + 1.77200*cb );
	if ( dimension2 == 16 ) codebook2[y][x++] = a0;
	codebook2[y][x++] = glimit( y1 + 1.40200*cr );
	codebook2[y][x++] = glimit( y1 - 0.34414*cb - 0.71414*cr );
	codebook2[y][x++] = glimit( y1 + 1.77200*cb );
	if ( dimension2 == 16 ) codebook2[y][x++] = a1;
	codebook2[y][x++] = glimit( y2 + 1.40200*cr );
	codebook2[y][x++] = glimit( y2 - 0.34414*cb - 0.71414*cr );
	codebook2[y][x++] = glimit( y2 + 1.77200*cb );
	if ( dimension2 == 16 ) codebook2[y][x++] = a2;
	codebook2[y][x++] = glimit( y3 + 1.40200*cr );
	codebook2[y][x++] = glimit( y3 - 0.34414*cb - 0.71414*cr );
	codebook2[y][x++] = glimit( y3 + 1.77200*cb );
	if ( dimension2 == 16 ) codebook2[y][x++] = a3;
  }

  if ( dimension2 == 16 ) index = 10*256; else index = 6*256;

  for(onf=0;onf<256;onf++) {
	for(y=0;y<4;y+=2) {
	  for(x=0;x<4;x+=2) {
		best = lineout[index++];
		numc = 0;
		for(yy=y;yy<(y+2);yy++) {
		  temp = (yy*dimension2)+x*(dimension2/4);
		  codebook4[onf][temp+0] = codebook2[best][numc++];	//r
			codebook4[onf][temp+1] = codebook2[best][numc++];	//g
			  codebook4[onf][temp+2] = codebook2[best][numc++];	//b
				codebook4[onf][temp+3] = codebook2[best][numc++];	//r a
				  codebook4[onf][temp+4] = codebook2[best][numc++];	//g r
					codebook4[onf][temp+5] = codebook2[best][numc++];	//b g 
					  if ( dimension2 == 16 ) {
						codebook4[onf][temp+6] = codebook2[best][numc++];	//b
						  codebook4[onf][temp+7] = codebook2[best][numc++];	//a
					  }
		}
	  }
	}
  }

  RoqWriteCodeBook(lineout);
  CodecPrepareCodeBook();

  free(ilist);
  free(lineout);

  return 1;
}

int  CodecPrepareCodeBook(  )
{
  VQDATA  tempfloat;			/* temporary variable */
  int     i, j;
  int     bestnorm;				/* used as an index for sorting norm */

  /* initialize index order */
  for (i = 0; i < 256; i++) {
    index2[i] = i;
    index4[i] = i;
  }

  /* find the norm of each codeword and store in an array */
  for (i = 0; i < 256; i++) {
    norm2[i] = 0.0;
    for (j = 0; j < dimension2; j++) {
      norm2[i] += codebook2[i][j]*codebook2[i][j];
    }
    norm2[i] = sqrt(norm2[i]);

    norm4[i] = 0.0;
    for (j = 0; j < dimension4; j++) {
      norm4[i] += codebook4[i][j]*codebook4[i][j];
    }
    norm4[i] = sqrt(norm4[i]);
  }
  
  /* reorder the codebook by ascending norm */
  for (i = 0; i < 256; i++) {
    bestnorm = i;
    /* find the lowest norm */
    for ( j = i; j < 256; j++) {
      if (norm2[j] < norm2[bestnorm]) bestnorm = j;
    }
    /* record index change */
    j = index2[i];
    index2[i] = bestnorm;
    index2[bestnorm] = j;
    
    /* reorder the norm arrray */
    tempfloat = norm2[i];
    norm2[i] = norm2[bestnorm];
    norm2[bestnorm] = tempfloat;
  }

  /* reorder the codebook by ascending norm */
  for (i = 0; i < 256; i++) {
    bestnorm = i;
    /* find the lowest norm */
    for ( j = i; j < 256; j++) {
      if (norm4[j] < norm4[bestnorm]) bestnorm = j;
    }
    /* record index change */
    j = index4[i];
    index4[i] = bestnorm;
    index4[bestnorm] = j;
    
    /* reorder the norm arrray */
    tempfloat = norm4[i];
    norm4[i] = norm4[bestnorm];
    norm4[bestnorm] = tempfloat;
  }

  return 1;
}

int  CodecBestCodeword( unsigned char * tempvector ,int dimension ,VQDATA ** codebook ,VQDATA * norm ,int * index )
{
  VQDATA  tempnorm;				/* temporary variable used to sort codebook */
  VQDATA  tempfloat;			/* temporary variable */
  VQDATA  temp;					/* temporary variable */
  VQDATA  bestdistortion;		/* distortion between vector and best codeword */
  int    i,j;					/* counters and indices */
  int    bestcodeword;			/* index of closest codeword */
  int    imin,imax;				/* indices */
  int	  itemp;

  /* compute the norm of the input vector */
  //    tempnorm = 0.0;
  itemp = 0;
  for (j = 0; j < dimension; j+=4) {
	itemp += tempvector[j+0]*tempvector[j+0];
	itemp += tempvector[j+1]*tempvector[j+1];
	itemp += tempvector[j+2]*tempvector[j+2];
	itemp += tempvector[j+3]*tempvector[j+3];
  }
  tempnorm = (VQDATA)itemp;
  tempnorm = sqrt(tempnorm);
    
  /* find the codeword with a norm closest to the input vector */
  bestcodeword = min_sup( 0, (codebooksize-1), tempnorm, norm);
    
  /* compute the distance between the codeword and the input vector that
	 has the closest norm */
  tempfloat = 0.0;
  for (j = 0; j < dimension; j+=4) {
	temp = ( (VQDATA) tempvector[j+0]) - codebook[index[bestcodeword]][j+0];
	tempfloat += temp*temp;
	temp = ( (VQDATA) tempvector[j+1]) - codebook[index[bestcodeword]][j+1];
	tempfloat += temp*temp;
	temp = ( (VQDATA) tempvector[j+2]) - codebook[index[bestcodeword]][j+2];
	tempfloat += temp*temp;
	temp = ( (VQDATA) tempvector[j+3]) - codebook[index[bestcodeword]][j+3];
	tempfloat += temp*temp;
  }    
  tempfloat = sqrt(tempfloat);
    
  /* identify the subset of the codewords to search such that 
	 norm(input)-distance <= norm(codeword) <= norm(input)+distance */
  imin = min_sup(0, codebooksize-1, tempnorm-tempfloat, norm);
  imax = max_inf(imin, codebooksize-1, tempnorm+tempfloat, norm);
    
  /* find the bestcodeword using partial distortion method */
  bestdistortion = HUGE;		/* keep convention that ties go to lower index */
  /* find the best codeword */
  for (i = imin; i <= imax; i++) { 
	tempfloat = 0.0;
	for (j = 0; j < dimension; j++) {
	  temp = ( (VQDATA) tempvector[j]) - codebook[index[i]][j];
	  tempfloat += temp*temp;
	  if (tempfloat > bestdistortion) j = dimension; /* abort loop */
	}
      
	if (tempfloat < bestdistortion) {
	  bestdistortion = tempfloat;
	  bestcodeword = i;
	}
      
	/* if bestidistortion is 0 then the best codeword has been found */
	if (bestdistortion == 0.0) i = codebooksize;
  } /* the best codeword has been found */
  return index[bestcodeword];
}    

int  CodecSetPreviousImage( const char* filename ,TagTable * timage)
{
  if (previousImage[0])
	{
	  ZimFree(previousImage[0]);
	}
  if (previousImage[1])
	{
	  ZimFree(previousImage[1]);
	}
  printf("setPreviousImage:%s\n", filename);
  
  image = ZimImageTagTableFromFile(filename, "jpeg");
  
  whichFrame=1;
  
  pixelsHigh = ZimHeight(image);
  pixelsWide = ZimWidth(image);

  printf("setPreviousImage: %dx%d\n", pixelsWide, pixelsHigh );

  
  previousImage[0] = ZimImageTagTableNew(pixelsWide, pixelsHigh, IMVFBRGB | IMVFBALPHA);
  previousImage[1] = ZimImageTagTableNew(pixelsWide, pixelsHigh, IMVFBRGB | IMVFBALPHA);
  
//  memcpy( ZimData(previousImage[0]), ZimData(image), pixelsWide*pixelsHigh*4);
//  memcpy( ZimData(previousImage[1]), ZimData(image), pixelsWide*pixelsHigh*4);
						
  ZimFree(image);
  printf("setPreviousImage:done\n");
  return 1;
}

int  CodecMakePreviousImage( quadcel * pquad )
{
  int i, dy, dx, pluck, size, ind, renamedone, xx, yy, pWide;
  int x, y;
  byte *rgbmap, *idataA, *fccdictionary;
  BOOL diff;
  
  if (RoqIsQuiet() == NO) {
	renamedone = rename("/LocalLibrary/RoQFiles/previousFrame.tiff",
						"/LocalLibrary/RoQFiles/previousPreviousFrame.tiff" );
  }
  for(i=0;i<256;i++) { used2[i] = used4[i] = NO; }

  pWide = pixelsWide & 0xfff0;
  
  if (!previousImage[0]) {
	previousImage[0] = ZimImageTagTableNew(pWide, pixelsHigh & 0xfff0, IMVFBRGB | IMVFBALPHA);
	previousImage[1] = ZimImageTagTableNew(pWide, pixelsHigh & 0xfff0, IMVFBRGB | IMVFBALPHA);
  }
  
  rgbmap = ZimData(previousImage[(whichFrame&1)]);
  
  if ((whichFrame&1) == 1) {
	fccdictionary = ZimData(previousImage[0]); 
  } else {
	fccdictionary = ZimData(previousImage[1]);
  }
  
  idataA = malloc( 16*16*4 );

  for(i=0;i<numQuadCels;i++) {
	diff = NO;
	size = pquad[i].size;
	if (size) {
	  switch( pquad[i].status ) {
	  case	DEP:
		break;
	  case	SLD:
		ind = pquad[i].patten[0];
		used4[ind] = YES;
		for( dy=0; dy<size; dy++ ) {
		  pluck = (((dy+pquad[i].yat)*pWide)+pquad[i].xat)*4;
		  for( dx=0; dx<size; dx++ ) {
			xx = ((dy>>1)*dimension2)+(dx>>1)*(dimension2/4);
			if (rgbmap[pluck+0] != codebook4[ind][xx+0]) diff = YES;
			if (rgbmap[pluck+1] != codebook4[ind][xx+1]) diff = YES;
			if (rgbmap[pluck+2] != codebook4[ind][xx+2]) diff = YES;
			if (dimension4 == 64 && rgbmap[pluck+3] != codebook4[ind][xx+3]) diff = YES;
					
			rgbmap[pluck+0] = codebook4[ind][xx+0];
			rgbmap[pluck+1] = codebook4[ind][xx+1];
			rgbmap[pluck+2] = codebook4[ind][xx+2];
			if (dimension4 == 64) 
			  rgbmap[pluck+3] = codebook4[ind][xx+3];
			else
			  rgbmap[pluck+3] = 255;
			pluck += 4;
		  }
		}
		if (diff == NO && whichFrame) printf("drawImage: SLD just changed the same thing\n");
		break;
	  case	PAT:
		ind = pquad[i].patten[0];
		used4[ind] = YES;
		for( dy=0; dy<size; dy++ ) {
		  pluck = (((dy+pquad[i].yat)*pWide)+pquad[i].xat)*4;
		  for( dx=0; dx<size; dx++ ) {
			xx = (dy*size*(dimension2/4))+dx*(dimension2/4);
			if (rgbmap[pluck+0] != codebook4[ind][xx+0]) diff = YES;
			if (rgbmap[pluck+1] != codebook4[ind][xx+1]) diff = YES;
			if (rgbmap[pluck+2] != codebook4[ind][xx+2]) diff = YES;
			if (dimension4 == 64 && rgbmap[pluck+3] != codebook4[ind][xx+3]) diff = YES;
					
			rgbmap[pluck+0] = codebook4[ind][xx+0];
			rgbmap[pluck+1] = codebook4[ind][xx+1];
			rgbmap[pluck+2] = codebook4[ind][xx+2];
			if (dimension4 == 64) 
			  rgbmap[pluck+3] = codebook4[ind][xx+3];
			else
			  rgbmap[pluck+3] = 255;
			pluck += 4;
		  }
		}
		if (diff == NO && whichFrame) printf("drawImage: PAT just changed the same thing\n");
		break;
	  case	CCC:
		dx = 1;
		for(yy=0;yy<4;yy+=2) {
		  for(xx=0;xx<4;xx+=2) {
			ind = pquad[i].patten[dx++];
			used2[ind] = YES;
			dy = 0;
			for(y=yy;y<(yy+2);y++) {
			  for(x=xx;x<(xx+2);x++) {
				pluck = (((y+pquad[i].yat)*pWide)+(pquad[i].xat+x))*4;
				if (rgbmap[pluck+0] != codebook2[ind][dy+0]) diff = YES;
				if (rgbmap[pluck+1] != codebook2[ind][dy+1]) diff = YES;
				if (rgbmap[pluck+2] != codebook2[ind][dy+2]) diff = YES;
				if (dimension4 == 64 && rgbmap[pluck+3] != codebook2[ind][dy+3]) diff = YES;
					
				rgbmap[pluck+0] = codebook2[ind][dy+0];
				rgbmap[pluck+1] = codebook2[ind][dy+1];
				rgbmap[pluck+2] = codebook2[ind][dy+2];
				if (dimension4 == 64) {
				  rgbmap[pluck+3] = codebook2[ind][dy+3];
				  dy += 4;
				} else {
				  rgbmap[pluck+3] = 255;
				  dy += 3;
				}
			  }
			}
		  }
		}
		if (diff == NO && whichFrame) {
		  printf("drawImage: CCC just changed the same thing\n");
		  printf("sparseEncode: something is wrong here\n");
		  printf("xat:    %d\n", pquad[i].xat);
		  printf("yat:    %d\n", pquad[i].yat);
		  printf("size    %d\n", pquad[i].size);
		  printf("type:   %d\n", pquad[i].status);
		  printf("motsnr: %0f\n", pquad[i].snr[FCC]);
		  printf("cccsnr: %0f\n", pquad[i].snr[CCC]);
		  printf("rmse:   %0f\n", pquad[i].rsnr);
		  printf("pat0:   %0d\n", pquad[i].patten[1]);
		  printf("pat1:   %0d\n", pquad[i].patten[2]);
		  printf("pat2:   %0d\n", pquad[i].patten[3]);
		  printf("pat3:   %0d\n", pquad[i].patten[4]);
		  exit(1);
		}
		break;
	  case	FCC:
		dx = pquad[i].xat - ((pquad[i].domain >> 8  ) - 128);
		dy = pquad[i].yat - ((pquad[i].domain & 0xff) - 128);
		if (ZimWidth(image) == (ZimHeight(image)*4)) dx = pquad[i].xat - ((pquad[i].domain >> 8  ) - 128)*2; // interlaced style video recognition hack
		if (RoqScaleable()) {
		  dx = pquad[i].xat - ((pquad[i].domain >> 8  ) - 128)*2;
		  dy = pquad[i].yat - ((pquad[i].domain & 0xff) - 128)*2;
		}
		//				if (pquad[i].yat == 0) printf("dx = %d, dy = %d, xat = %d\n", dx, dy, pquad[i].xat);
				
		ind = (dy*pWide+dx)*4;
		for( dy=0; dy<size; dy++ ) {
		  pluck = (((dy+pquad[i].yat)*pWide)+pquad[i].xat)*4;
		  for( dx=0; dx<size; dx++ ) {
			if (rgbmap[pluck+0] != fccdictionary[ind+0]) diff = YES;
			if (rgbmap[pluck+1] != fccdictionary[ind+1]) diff = YES;
			if (rgbmap[pluck+2] != fccdictionary[ind+2]) diff = YES;
					
			rgbmap[pluck+0] = fccdictionary[ind+0];
			rgbmap[pluck+1] = fccdictionary[ind+1];
			rgbmap[pluck+2] = fccdictionary[ind+2];
			rgbmap[pluck+3] = fccdictionary[ind+3];
			pluck += 4; ind += 4;
		  }
		  ind += (pWide - size)*4;
		}
		if (diff == NO && whichFrame) printf("drawImage: FCC just changed the same thing\n");
		break;
	  case	MOT:
		break;
	  default:
		fprintf(stderr, "bad code!!\n");
		break;
	  }
	}
  }
 /* if (RoqIsQuiet() == NO) {
  	char filename[1025];
	sprintf(filename, "/tmp/%06d.jpg", whichFrame);
  	//ZimWriteJFIF(previousImage[whichFrame&1], filename, 95);
	//fprintf(stdout,"Saved image to %s\n", filename);
  	}*/
  /*	fd = open("/LocalLibrary/RoQFiles/previousFrame.tiff", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	if (fd<0) {
	  NXRunAlertPanel(0, "Cannot save file: %s", 0, 0, 0, strerror(errno));
	  exit(1);
	}
	stream = NXOpenFile(fd, NX_WRITEONLY);
	[previousImage[whichFrame&1] writeTIFF: stream];
	NXClose(stream);
	close(fd);
	if (!dictionaryWindow) {
	  cRect.origin.x = 8.0 * 48.0;
	  cRect.origin.y = ([previousImage[whichFrame&1] pixelsHigh]*2+160);
	  cRect.size.width = [previousImage[whichFrame&1] pixelsWide];
	  cRect.size.height = [previousImage[whichFrame&1] pixelsHigh];
	  dictionaryWindow = [[Window alloc] initContent:&cRect style:NX_TITLEDSTYLE
						backing:NX_RETAINED buttonMask:0 defer:NO];
	  cRect.origin.x = cRect.origin.y = 0.0;
	  [[dictionaryWindow contentView] setClipping:NO];
	  [dictionaryWindow makeKeyAndOrderFront: [dictionaryWindow contentView]];
	  [dictionaryWindow display];
	  [dictionaryWindow setTitle: "previous compressed frame"];
	}
	[[dictionaryWindow contentView] getBounds: &cRect];
	[[dictionaryWindow contentView] lockFocus];
	[previousImage[whichFrame&1] drawIn: &cRect];
	[[dictionaryWindow contentView] unlockFocus];
  }
  */
  if (whichFrame == 0) {
	memcpy( ZimData(previousImage[1]), ZimData(previousImage[0]), pWide*(pixelsHigh & 0xfff0)*4);
  }

  x = 0; y = 0;
  for(i=0;i<256;i++) { x=x+(used4[i]==YES); y=y+(used2[i]==YES); }

  if (RoqIsQuiet() == NO) printf("drawImage: used %d 4x4 and %d 2x2 VQ cels\n", x,y);

  free( idataA );
  return 1;
}

int  CodecInitImages(  )
{
  int x,y, index0, index1, temp;
  float ftemp;
  byte *lutimage;

  numQuadCels  = ((pixelsWide & 0xfff0)*(pixelsHigh & 0xfff0))/(MINSIZE*MINSIZE);
  numQuadCels += numQuadCels/4 + numQuadCels/16;

  if (qStatus) free(qStatus);
  qStatus = (quadcel *) calloc(numQuadCels, sizeof (quadcel));
  CodecInitQStatus();
  //
	if (previousImage[0]) {
	  pixelsWide = ZimWidth(previousImage[0]);
	  pixelsHigh = ZimHeight(previousImage[0]);
	  temp = ((whichFrame+1)&1);
	  if (!luti) luti = malloc(pixelsWide*pixelsHigh);
	  lutimage = ZimData(previousImage[temp]);
	  if (RoqIsQuiet() == NO) {
		printf("initImage: remaking lut image using buffer %d\n", temp);
	  }
	  index0 = index1 = 0;
	  for(y=0;y<pixelsHigh; y++) {
		for(x=0;x<pixelsWide; x++) {
		  ftemp = RMULT*lutimage[index0+0] + GMULT*lutimage[index0+1] + BMULT*lutimage[index0+2];
		  temp = ftemp;
		  luti[index1] = temp;
			
		  index0 += ZimBpp(previousImage[0]);
		  index1++;
		}
	  }
	}
  return 1;
}


int  CodecQuadX(  int  startX , int  startY , int  quadSize )
{
  int startSize;
  int bigx, bigy, lowx, lowy;

  lowx = lowy = 0;
  bigx = pixelsWide & 0xfff0;
  bigy = pixelsHigh & 0xfff0;
	
  if ( (startX >= lowx) && (startX+quadSize) <= (bigx) && (startY+quadSize) <= (bigy) && (startY >= lowy) && quadSize <= MAXSIZE) {
	qStatus[onQuad].size 	= quadSize;
	qStatus[onQuad].xat		= startX;
	qStatus[onQuad].yat		= startY;
	onQuad++;
  }

  if (quadSize != MINSIZE) {
	startSize = quadSize>>1;
	CodecQuadX( startX, startY, startSize);
	CodecQuadX( startX+startSize, startY, startSize);
	CodecQuadX( startX, startY+startSize, startSize);
	CodecQuadX( startX+startSize, startY+startSize, startSize);
  }

  return 1;
}

int  CodecInitQStatus(  )
{
  int i,x,y;

  for (i=0; i<numQuadCels; i++)
	qStatus[i].size = 0;
  
  onQuad = 0;
  for(y=0;y<pixelsHigh; y+=16) {
	for(x=0;x<pixelsWide; x+=16) {
	  CodecQuadX(x, y, 16);
	}
  }
  currentTotalBits = CodecComputeCurrentQuadOutputSize(qStatus);
  
  return 1;
}


int  CodecVqData8(  byte * cel , quadcel * pquad )
{
  byte	tempImage[8*8*4];
  int		x, y, i, best, temp;

  i = 0;
  for(y=0;y<4;y++) {
	for(x=0;x<4;x++) {
	  temp = y*64+x*8;
	  tempImage[i++] = (cel[temp+0]+cel[temp+4]+cel[temp+32]+cel[temp+36])/4;
	  tempImage[i++] = (cel[temp+1]+cel[temp+5]+cel[temp+33]+cel[temp+37])/4;
	  tempImage[i++] = (cel[temp+2]+cel[temp+6]+cel[temp+34]+cel[temp+38])/4;
	  if (dimension4 == 64)
		tempImage[i++] = (cel[temp+3]+cel[temp+7]+cel[temp+35]+cel[temp+39])/4;
	}
  }

  pquad->patten[0] = best = CodecBestCodeword(tempImage, dimension4, codebook4, norm4, index4);

  for(y=0;y<8;y++) {
	for(x=0;x<8;x++) {
	  temp = y*32+x*4;
	  i = ((y/2)*4*(dimension2/4))+(x/2)*(dimension2/4);
	  tempImage[temp+0] = codebook4[best][i+0];
	  tempImage[temp+1] = codebook4[best][i+1];
	  tempImage[temp+2] = codebook4[best][i+2];
	  if (dimension4 == 64)
		tempImage[temp+3] = codebook4[best][i+3];
	  else
		tempImage[temp+3] = 255;
	}
  }

  pquad->snr[SLD] = CodecSnr(cel, tempImage, 8);
  return 1;
}

int  CodecVqData4(  byte * cel , quadcel * pquad )
{
  byte	tempImage[64];
  int		i, best, bpp;

  //	if ([theRoQ makingVideo] && previousImage[0]) return 1;
  if (dimension4 == 64) bpp = 4; else bpp = 3;
  for(i=0;i<16;i++) {
	tempImage[i*bpp+0] = cel[i*4+0];
	tempImage[i*bpp+1] = cel[i*4+1];
	tempImage[i*bpp+2] = cel[i*4+2];
	if (dimension4 == 64) tempImage[i*bpp+3] = cel[i*4+3];
  }

  pquad->patten[0] = best = CodecBestCodeword(tempImage, dimension4, codebook4, norm4, index4);

  for(i=0;i<16;i++) {
	tempImage[i*4+0] = codebook4[best][i*bpp+0];
	tempImage[i*4+1] = codebook4[best][i*bpp+1];
	tempImage[i*4+2] = codebook4[best][i*bpp+2];
	if (dimension4 == 64)
	  tempImage[i*4+3] = codebook4[best][i*bpp+3];
	else
	  tempImage[i*4+3] = 255;
  }

  pquad->snr[PAT] = CodecSnr(cel, tempImage, 4);
  return 1;
}

int  CodecVqData2(  byte * cel , quadcel * pquad )
{
  byte	tempImage[16], tempOut[64];
  int		i, j, best,x,y,xx,yy,bpp;

  if (dimension4 == 64) bpp = 4; else bpp = 3;
  j = 1;
  for(yy=0;yy<4;yy+=2) {
	for(xx=0;xx<4;xx+=2) {
	  i = 0;
	  for(y=yy;y<(yy+2);y++) {
		for(x=xx;x<(xx+2);x++) {
		  tempImage[i++] = cel[y*16+x*4+0];
		  tempImage[i++] = cel[y*16+x*4+1];
		  tempImage[i++] = cel[y*16+x*4+2];
		  if (dimension4 == 64) tempImage[i++] = cel[y*16+x*4+3];
		}
	  }
	  pquad->patten[j++] = best = CodecBestCodeword(tempImage, dimension2, codebook2, norm2, index2);
	  i = 0;
	  for(y=yy;y<(yy+2);y++) {
		for(x=xx;x<(xx+2);x++) {
		  tempOut[y*16+x*4+0] = codebook2[best][i++];
		  tempOut[y*16+x*4+1] = codebook2[best][i++];
		  tempOut[y*16+x*4+2] = codebook2[best][i++];
		  if (dimension4 == 64)
			tempOut[y*16+x*4+3]  = codebook2[best][i++];
		  else
			tempOut[y*16+x*4+3] = 255;
		}
	  }
	}
  }

  pquad->snr[CCC] = CodecSnr(cel, tempOut, 4);
  return 1;
}

int  CodecIRGBtab (  )
{
  initRGBtab++;
  return 1;
}

float  CodecSnr( byte * old ,byte * new ,int size)
{
  
  int i, j;
  float fsnr;
  register int ind;

  ind = 0;
	
  for(i=0; i<size; i++) {
	for(j=0; j<size; j++) {
	  if (old[3]||new[3]) ind += RGBADIST( old, new );
	  old += 4; new += 4;
	}
  }

  fsnr = (float)ind;
  fsnr /= (size*size);
  fsnr = sqrt( fsnr );
	
  return (fsnr);
}
	 
int  CodecComputeMotionBlock(  byte * old ,byte * new ,int size )
{
  int i,j,snr;

  if (dimension4==64) return 0;	// do not use this for alpha pieces
	snr = 0;

  for(i=0; i<size; i++) {
	for(j=0; j<size; j++) {
	  snr += RGBADIST( old, new );
	  old += 4; new += 4;
	}
  }
  snr /= (size*size);
  return ( snr <= MOTION_MIN );
}

int  CodecFvqData(  byte * bitmap , int size , int realx , int realy , quadcel * pquad ,BOOL clamp )
{
  int x, y, xLen, yLen, mblur0, ripl, bpp, fabort, temp1;
  int lowX, lowY, onx, ony, sX, sY, depthx, depthy, breakHigh;
  float lowestSNR, fmblur0;
  byte *scale1;
  byte *bitma2;
  int searchY, searchX, xxMean, yyMean;
  
  if (size == 4)
	{
	  if ( !(pquad->snr[FCC] == 9999))
		{
		  return 1;//we already did this cell and dxMean and dyMean don't change for cells of size 4
	}
}

	if ( !previousImage[0] || dimension4 == 64) {
		return 1;
	}

	for(x=0; x<(size*size); x++) {
		fmblur0 = RMULT*bitmap[x*4+0] + GMULT*bitmap[x*4+1] + BMULT*bitmap[x*4+2];
		luty[x] = fmblur0;
	}
	if (!luti) {
		pquad->domain = 0x8080;
		pquad->snr[FCC] = 9999;
		return 1;
	}

	ony = realy - (realy & 0xfff0);
	onx = realx - (realx & 0xfff0);
	
	xLen = ZimWidth(previousImage[0]);
	yLen = ZimHeight(previousImage[0]);
	ripl = xLen-size;
	
	breakHigh = 99999999;

	fabort = 0;
	lowX = lowY = -1;
	depthx = depthy = 1;
	searchY = 32;
	searchX = 32;
	if (xLen == (yLen*4)) depthx = 2;
	if (RoqScaleable()) depthx = depthy = 2;
	
	if (clamp) { searchX = searchY = 8; }
	searchX = searchX*depthx;
	searchY = searchY*depthy;
	xxMean = dxMean*depthx;
	yyMean = dyMean*depthy;
	
	if (((realx-xxMean)+searchX)<0 ||(((realx-xxMean)-searchX)+depthx+size)>xLen || ((realy-yyMean)+searchY)<0 || (((realy-yyMean)-searchY)+depthy+size)>yLen) {
		pquad->snr[FCC] = 9999;
		return 1;
	}

	for( sX=(((realx-xxMean)-searchX)+depthx); sX<=((realx-xxMean)+searchX) && !fabort; sX+=depthx ) {
	for( sY=(((realy-yyMean)-searchY)+depthy); sY<=((realy-yyMean)+searchY); sY+=depthy ) {
	temp1 = xLen*sY+sX;
	if ( sX >= 0 && (sX+size) <= xLen && sY >= 0 && (sY+size) <= yLen ) {
		mblur0 = 0;
		bitma2 = luty;
		scale1 = luti + temp1;
		for( y=0; y<size; y++) {
		for( x=0; x<size; x+=4) {
			mblur0 += RGBADIST( bitma2, scale1);
			bitma2 += 4; scale1 += 4;
		}
		if (mblur0 > breakHigh) {
			break;
		}
		scale1 += ripl;
		}
		if (breakHigh > mblur0) {
			breakHigh = mblur0;
       		lowX = sX;
        	lowY = sY;
			if (breakHigh<size) fabort++;
		}
	}
	}
	}
	
	if (lowX != -1 && lowY != -1) {
		bpp  = ZimBpp(previousImage[0]);
		ripl = (xLen-size)*bpp;
		mblur0 = 0;
		bitma2 = bitmap;
		scale1 = ((byte *) ZimData(previousImage[((whichFrame+1)&1)])) + (xLen*lowY+lowX)*bpp; 
		for( y=0; y<size; y++) {
		for( x=0; x<size; x++) {
			mblur0 += RGBADIST( bitma2, scale1 );
			scale1 += 4; bitma2 += 4;
		}
		scale1 += ripl;
		}


		lowestSNR = mblur0;
		lowestSNR /= (size*size);
		lowestSNR = sqrt( lowestSNR );
	
		sX = (realx-lowX+128);
		sY = (realy-lowY+128);
		if (depthx==2) {
			sX = ((realx-lowX)/2+128);
		}
		if (depthy==2) {
			sY = ((realy-lowY)/2+128);
		}
		
		pquad->domain = (sX<<8)+sY;
		pquad->snr[FCC] = lowestSNR;
	}

	return 1;
}


int  CodecConvertPlanertoPacked(  )
{
  byte *iPlane[5], *newdata, *olddata;
  int x,y,index, sample;

  newImage = ZimImageTagTableNew(pixelsWide, pixelsHigh,IMVFBRGB | IMVFBALPHA );

  newdata = ZimData(newImage);
  index = 0;

  if (ZimIsPlanar(image)) {
	ZimGetDataPlanes(image, iPlane);
	for(y=0;y<pixelsHigh;y++) {
	  for(x=0;x<pixelsWide;x++) {
		newdata[index++] = iPlane[0][y*pixelsWide+x];
		newdata[index++] = iPlane[1][y*pixelsWide+x];
		newdata[index++] = iPlane[2][y*pixelsWide+x];
		if (ZimHasAlpha(image)) {
		  newdata[index++] = iPlane[3][y*pixelsWide+x];
		} else {
		  newdata[index++] = 255;
		}
	  }
	}
  } else {
	
	sample = 0;
	olddata = ZimData(image);
	for(y=0;y<pixelsHigh;y++) {
	  for(x=0;x<pixelsWide;x++) {
		newdata[index++] = olddata[sample++];
		newdata[index++] = olddata[sample++];
		newdata[index++] = olddata[sample++];
		if (ZimHasAlpha(image)) {
		  newdata[index++] = olddata[sample++];
		} else {
		  newdata[index++] = 255;
		}
	  }
	}  
  }
  
  //ZimFree(image); //bad idea-roq owns it!
  image = newImage;
  
  return 1;
}

int  CodecGetData(  unsigned char * iData , int qSize , int startX , int startY , TagTable * bitmap )
{
  int x,y,yoff,bpp,yend,xend;
  byte *iPlane[5];
  int r,g,b,a;
	
  yend = qSize+startY;
  xend = qSize+startX;

  if (startY > ZimHeight(bitmap)) return 1;

  if (yend > ZimHeight(bitmap)) yend =  ZimHeight(bitmap);
  if (xend > ZimWidth(bitmap)) xend = ZimWidth(bitmap);

  bpp = ZimBpp(bitmap);

	if (ZimIsPlanar(bitmap)) {
		ZimGetDataPlanes(bitmap, iPlane);
		for(y=startY;y<yend;y++) {
			yoff = y*ZimWidth(bitmap);
			for(x=startX;x<xend;x++) {
				r = iPlane[0][yoff+x];
				g = iPlane[1][yoff+x];
				b = iPlane[2][yoff+x];
				*iData++ = r; *iData++ = g; *iData++ = b;
				if (ZimHasAlpha(bitmap)) {
					*iData++ = iPlane[3][yoff+x];
				} else {
					*iData++ = 255;
				}
			}
		}
	} else if (ZimHasAlpha(bitmap)) {
	iPlane[0] = ZimData(bitmap);
	for(y=startY;y<yend;y++) {
	  yoff = y*ZimWidth(bitmap)*bpp;
	  for(x=startX;x<xend;x++) {
		r = iPlane[0][yoff+(x*bpp)+0];
		g = iPlane[0][yoff+(x*bpp)+1];
		b = iPlane[0][yoff+(x*bpp)+2];
		a = iPlane[0][yoff+(x*bpp)+3];
		*iData++ = r; *iData++ = g; *iData++ = b; *iData++ = a;
	  }
	}
	} else {
		iPlane[0] = ZimData(bitmap);
		for(y=startY;y<yend;y++) {
		  yoff = y*ZimWidth(bitmap)*bpp;
		  for(x=startX;x<xend;x++) {
			r = iPlane[0][yoff+(x*bpp)+0];
			g = iPlane[0][yoff+(x*bpp)+1];
			b = iPlane[0][yoff+(x*bpp)+2];
			*iData++ = r; *iData++ = g; *iData++ = b;
			*iData++ = 255;
		  }
		}
	}

  return 1;
}

int  CodecLowestQuad(  quadcel* qtemp , int* status , float* snr , int bweigh )
{
  float wtemp;
  int quickadd[DEAD], i;
  int tstatus=0;
  float tsnr=0.0;
  
  quickadd[CCC] = 2;
  quickadd[SLD] = 1;
  quickadd[MOT] = 1;
  quickadd[FCC] = 1;
  
  quickadd[PAT] = 1;
  if ((!previousImage[0] || RoqIsLastFrame() || detail) && !RoqMakingVideo() ) {
	quickadd[CCC] = 1;
	quickadd[PAT] = 2;
  }
  
  if (RoqMakingVideo()) {
	quickadd[CCC] = 2;
	if (detail) quickadd[CCC] = 1;
  }
  
  wtemp = 99999;
  
  for(i=(DEAD-1);i>0;i--) {
	if ( qtemp->snr[i]*quickadd[i] < wtemp ) {
	  tstatus = i;
	  tsnr 	= qtemp->snr[i];
	  wtemp 	= qtemp->snr[i]*quickadd[i];
	}
  }
  
  if ( qtemp->mark == YES ) tstatus = MOT;
  CodecSetStatus(tstatus,qtemp);
  *snr = tsnr;
  
  return 1;
}

int  CodecGetCurrentQuadOutputSize( quadcel * pquad )
{
  return (currentTotalBits >>3) +2;
}

int  CodecComputeCurrentQuadOutputSize( quadcel * pquad )
{
  int	totalbits, i;
  
  totalbits = 0;
  
  for( i=0; i<numQuadCels; i++ ) {
	if (pquad[i].size && pquad[i].size < 16 ) totalbits += sizeQuickAdd[pquad[i].status];
  }
  
  return (totalbits);
}

int  CodecSetStatus( int  status  ,quadcel *  cel )
{
  
  if (cel->status == status)
	{	  
	  return 1;
	}
  else if ((!(cel->size && cel->size < 16)) || (sizeQuickAdd[cel->status] == sizeQuickAdd[status]))
	{	 
	  cel->status = status;
	  return 1;
	}
  if (cel->size && cel->size < 16)
	{
	  currentTotalBits +=  sizeQuickAdd[status] - sizeQuickAdd[cel->status] ;
	}
  
  cel->status = status;

  return 1;
}

int  CodecSetSize( int  size ,quadcel *  cel )
{
  BOOL oldquick, newquick;

  oldquick = (cel->size && cel->size < 16);
  newquick = size && size <16;

  if (oldquick && !newquick)
	{
	  currentTotalBits -= sizeQuickAdd[cel->status];
	}
  if (!oldquick && newquick)
	{
	  currentTotalBits += sizeQuickAdd[cel->status];
	}
  
  cel->size = size;
  return 1;
}

float  CodecGetCurrentRMSE( quadcel * pquad )
{
int	i, j;
double totalbits;

	totalbits = 0;
	j = 0;
	for( i=0; i<numQuadCels; i++ ) {
		if (pquad[i].size && pquad[i].status && pquad[i].status != DEAD) {
			if (pquad[i].size ==  8) { totalbits += pquad[i].rsnr*4;  j += 4; }
			if (pquad[i].size ==  4) { totalbits += pquad[i].rsnr*1;  j += 1; }
		}
	}
	totalbits /= j;
	return ((float)totalbits);
}

int  CodecAddQuad( quadcel * pquad ,int lownum )
{
  int i, nx, nsize;
  float newsnr, cmul;
  byte *idataA, *idataB;

  if (lownum != -1) {
		
	if (pquad[lownum].size == 8) {
	  nx = 1; nsize = 4; cmul = 1;
	} else {
	  nx = 5; nsize = 8; cmul = 4;
	}	
	newsnr = 0;
	idataA = malloc(8*8*4);
	idataB = malloc(8*8*4);
	for( i=lownum+1; i<lownum+(nx*4)+1; i+=nx ) {
	  CodecSetSize(nsize, &pquad[i]);
		  
	  CodecGetData(idataA, pquad[i].size, pquad[i].xat, pquad[i].yat, image);
	  CodecVqData4(idataA, &pquad[i]);
	  CodecVqData2(idataA, &pquad[i]);
	  if (previousImage[0]) {
		CodecFvqData(idataA, pquad[i].size, pquad[i].xat, pquad[i].yat, &pquad[i], YES);
		CodecGetData(idataB, pquad[i].size, pquad[i].xat, pquad[i].yat, previousImage[whichFrame&1]);
		pquad[i].snr[MOT] = CodecSnr(idataA, idataB, pquad[i].size);
		if (CodecComputeMotionBlock(idataA, idataB, pquad[i].size) && !RoqIsLastFrame() && !detail) pquad[i].mark = YES;
	  }
	  CodecLowestQuad(&pquad[i], &pquad[i].status, &pquad[i].rsnr, YES);
	  newsnr += pquad[i].rsnr;
	}
	free(idataA); free(idataB);
	newsnr /= 4;
	CodecLowestQuad(&pquad[lownum], &pquad[lownum].status, &pquad[lownum].rsnr, 0);
		
	if (   pquad[lownum+nx*0+1].status == MOT && pquad[lownum+nx*1+1].status == MOT
		&& pquad[lownum+nx*2+1].status == MOT && pquad[lownum+nx*3+1].status == MOT  
		&& nsize == 4) { newsnr = 9999;	CodecSetStatus(MOT, &pquad[lownum]); }
				
	if ( pquad[lownum].rsnr > newsnr ) {
	  CodecSetStatus(DEP,&pquad[lownum]);
	  pquad[lownum].rsnr   = 0;
	} else {
	  CodecSetStatus(0,&pquad[lownum+nx*0+1]);
	  CodecSetStatus(0,&pquad[lownum+nx*1+1]);
	  CodecSetStatus(0,&pquad[lownum+nx*2+1]);
	  CodecSetStatus(0,&pquad[lownum+nx*3+1]);

	  CodecSetSize(0,&pquad[lownum+nx*0+1]);
	  CodecSetSize(0,&pquad[lownum+nx*1+1]);
	  CodecSetSize(0,&pquad[lownum+nx*2+1]);
	  CodecSetSize(0,&pquad[lownum+nx*3+1]);
	}
  } else {
	lownum = -1;
  }
  return lownum;
}

int  CodecMotMeanX(  )
{
	return dxMean;
}

int  CodecMotMeanY(  )
{
	return dyMean;
}

int  CodecSparseEncode(  )
{
  int i, j, osize, fsize, num[DEAD+1], *ilist, onf, ong, wtype, temp;
  float *flist, sRMSE, numredo;
  byte *idataA, *idataB;

	osize = 8;

	image = RoqCurrentImage();
	newImage = 0;

	pixelsHigh = ZimHeight(image);
	pixelsWide = ZimWidth(image);

	dimension2 = 12; dimension4 = 48;
	if (ParamImSeqCurrentImageHadAlpha(RoqParamImSeq()) && (RoqParamNoAlpha() == NO)) { dimension2 = 16; dimension4 = 64; }
	if (ZimIsPlanar(image) || ZimBpp(image)!=4) CodecConvertPlanertoPacked();

	idataA = malloc( 16*16*4 );
	idataB = malloc( 16*16*4 );

	if (!previousImage[0]) printf("sparseEncode: sparsely encoding a %d,%d image\n", pixelsWide, pixelsHigh);
	CodecInitImages();

	flist = calloc( (numQuadCels+1), sizeof(float) );
	ilist = calloc( (numQuadCels+1), sizeof(int  ) );


	fsize = 56*1024;
	if (previousImage[0]) fsize = RoqNormalFrameSize(); else fsize = RoqFirstFrameSize();
//	if (RoqHasSound() && fsize > 6000 && previousImage[0]) fsize = 6000;
	
	dxMean = dyMean = 0;
	if (previousImage[0]) wtype = 1; else wtype = 0;

	for( i=0; i<numQuadCels; i++ ) {
		for(j=0;j<DEAD;j++) qStatus[i].snr[j] = 9999;
		qStatus[i].mark = NO;
		if ( qStatus[i].size == osize ) {
			if (previousImage[0]) {
				CodecGetData(idataA, qStatus[i].size, qStatus[i].xat, qStatus[i].yat, image);
				CodecGetData(idataB, qStatus[i].size, qStatus[i].xat, qStatus[i].yat, previousImage[whichFrame&1]);
				qStatus[i].snr[MOT] = CodecSnr(idataA, idataB, qStatus[i].size);
				if (CodecComputeMotionBlock(idataA, idataB, qStatus[i].size) && !RoqIsLastFrame()) qStatus[i].mark = YES;
				if (!qStatus[i].mark) 
					CodecFvqData(idataA, qStatus[i].size, qStatus[i].xat, qStatus[i].yat, &qStatus[i], NO);
			}
			CodecLowestQuad(&qStatus[i], &qStatus[i].status, &qStatus[i].rsnr, wtype);
		} else {
			if ( qStatus[i].size < osize ) {
			  CodecSetStatus(0,&qStatus[i]);
			  CodecSetSize(0,&qStatus[i]);
			  
			} else {
			  CodecSetStatus(DEP,&qStatus[i]);
			  qStatus[i].rsnr   = 0;
			}
		}
	}
  //
  // the quad is complete, so status can now be used for quad decomposition
  // the first thing to do is to set it up for all the 4x4 cels to get output
  // and then recurse from there to see what's what
  //
	sRMSE = CodecGetCurrentRMSE(qStatus);

		  if (RoqIsQuiet() == NO) {
			printf("sparseEncode: rmse of quad0 is %f, size is %d (meant to be %d)\n", sRMSE, CodecGetCurrentQuadOutputSize( qStatus), fsize );
		  }
		
		  onf = 0;
		  for(i=0;i<numQuadCels;i++) {
			if ( qStatus[i].size && qStatus[i].status != DEP) {
			  flist[onf] = qStatus[i].rsnr;
			  ilist[onf] = i;
			  onf++;
			}
		  }

		  CodecSort(flist,ilist, onf);
		  CodecSegment(ilist, flist, onf, CodecGetCurrentRMSE( qStatus));

		  temp = dxMean = dyMean = 0;
		  for( i=0; i<numQuadCels; i++ ) {
			if (qStatus[i].size && qStatus[i].status == FCC) {
			  dxMean += (qStatus[i].domain >> 8  ) - 128;
			  dyMean += (qStatus[i].domain & 0xff) - 128;
			  temp++;
			}
		  }
		  if (temp) { dxMean /= temp; dyMean /= temp; }	
		  printf("sparseEncode: dx/dy mean is %d,%d\n", dxMean, dyMean);

		  numredo = 0;
		  detail = NO;
		  if (codebookmade && whichFrame>4) fsize -= 256;
		  temp = 0;
		  for( i=0; i<numQuadCels; i++ ) {
			if ( qStatus[i].size == osize && qStatus[i].mark == NO && qStatus[i].snr[MOT] > 0 ) {
			  CodecGetData(idataA, qStatus[i].size, qStatus[i].xat, qStatus[i].yat, image);
			  if (osize == 8) CodecVqData8(idataA, &qStatus[i]);
			  if (previousImage[0]) {
				int dx,dy;
				dx = (qStatus[i].domain >> 8  ) - 128 - dxMean + 8;
				dy = (qStatus[i].domain & 0xff) - 128 - dyMean + 8;
 				if (dx<0||dx>15||dy<0||dy>15) {
				  qStatus[i].snr[FCC] = 9999;
				  temp++;
				  CodecFvqData(idataA, qStatus[i].size, qStatus[i].xat, qStatus[i].yat, &qStatus[i], YES);
				  dx = (qStatus[i].domain >> 8  ) - 128 - dxMean + 8;
				  dy = (qStatus[i].domain & 0xff) - 128 - dyMean + 8;
				  if ((dx<0||dx>15||dy<0||dy>15)&&qStatus[i].snr[FCC]!=9999&&qStatus[i].status==FCC) {
					printf("sparseEncode: something is wrong here, dx/dy is %d,%d after being clamped\n", dx, dy);
					printf("xat:    %d\n", qStatus[i].xat);
					printf("yat:    %d\n", qStatus[i].yat);
					printf("size    %d\n", qStatus[i].size);
					printf("type:   %d\n", qStatus[i].status);
					printf("mot:    %04x\n", qStatus[i].domain);
					printf("motsnr: %0f\n", qStatus[i].snr[FCC]);
					printf("rmse:   %0f\n", qStatus[i].rsnr);
					exit(1);
				  }
				}
			  }
			  CodecLowestQuad(&qStatus[i], &qStatus[i].status, &qStatus[i].rsnr, wtype);
			  if (qStatus[i].status==FCC && qStatus[i].snr[FCC]>qStatus[i].snr[SLD]) {
				printf("sparseEncode: something is wrong here\n");
				printf("xat:    %d\n", qStatus[i].xat);
				printf("yat:    %d\n", qStatus[i].yat);
				printf("size    %d\n", qStatus[i].size);
				printf("type:   %d\n", qStatus[i].status);
				printf("mot:    %04x\n", qStatus[i].domain);
				printf("motsnr: %0f\n", qStatus[i].snr[FCC]);
				printf("sldsnr: %0f\n", qStatus[i].snr[SLD]);
				printf("rmse:   %0f\n", qStatus[i].rsnr);
				exit(1);
			  }
			}
		  }

		  if (RoqIsQuiet() == NO) {
			printf("sparseEncode: rmse of quad0 is %f, size is %d (meant to be %d)\n", CodecGetCurrentRMSE(qStatus), CodecGetCurrentQuadOutputSize(qStatus), fsize );
			printf("sparseEncode: %d outside fcc limits\n", temp);
		  }
		
		  onf = 0;
		  for(i=0;i<numQuadCels;i++) {
			if ( qStatus[i].size && qStatus[i].status != DEP) {
			  flist[onf] = qStatus[i].rsnr;
			  ilist[onf] = i;
			  onf++;
			}
		  }
	
		  CodecSort(flist, ilist, onf);

		  ong = 0; detail = NO;
		  while ( CodecGetCurrentQuadOutputSize( qStatus) < fsize && ong < onf && flist[ong] > 0 && qStatus[ilist[ong]].mark == NO) {
			osize = CodecAddQuad(qStatus, ilist[ong++]);
		  }

		  if ( CodecGetCurrentQuadOutputSize(qStatus) < fsize) {
			ong = 0;
			while ( CodecGetCurrentQuadOutputSize(qStatus) < fsize && ong < onf && flist[ong] > 0) {
			  i = ilist[ong++];
			  if (qStatus[i].mark) {
				detail = NO; 
				qStatus[i].mark = NO;
				CodecGetData(idataA, qStatus[i].size, qStatus[i].xat, qStatus[i].yat, image);
				if (qStatus[i].size == 8) CodecVqData8(idataA, &qStatus[i]);
				if (qStatus[i].size == 4) CodecVqData4(idataA, &qStatus[i]);
				if (qStatus[i].size == 4) CodecVqData2(idataA, &qStatus[i]);
				if (previousImage[0]) {
				  CodecFvqData(idataA, qStatus[i].size, qStatus[i].xat, qStatus[i].yat, &qStatus[i], YES);
				}
				CodecLowestQuad(&qStatus[i], &qStatus[i].status, &qStatus[i].rsnr, wtype);
			  }
			}
			ong = 0;
			while ( CodecGetCurrentQuadOutputSize(qStatus) < fsize && ong < onf && flist[ong] > 0) {
			  i = ilist[ong++];
			  detail = YES;
			  osize = CodecAddQuad(qStatus, i);
			}
		  }

		  printf("sparseEncode: rmse of frame %d is %f, size is %d\n", whichFrame, CodecGetCurrentRMSE(qStatus), CodecGetCurrentQuadOutputSize(qStatus) );

	
		  if (RoqIsQuiet() == NO) {
			for(i=0;i<DEAD;i++) num[i] = 0;
			j = 0;
			for( i=0; i<numQuadCels; i++ ) {
			  if (qStatus[i].size == 8 && qStatus[i].status) {
				if (qStatus[i].status < DEAD) num[qStatus[i].status]++; j++;
			  }
			}
			printf("sparseEncode: for 08x08 CCC = %d, FCC = %d, MOT = %d, SLD = %d, PAT = %d\n", num[CCC], num[FCC], num[MOT], num[SLD], num[PAT]);

			for(i=0;i<DEAD;i++) num[i] = 0;
			for( i=0; i<numQuadCels; i++ ) {
			  if (qStatus[i].size == 4 && qStatus[i].status) {
				if (qStatus[i].status < DEAD) num[qStatus[i].status]++; j++;
			  }
			}
			printf("sparseEncode: for 04x04 CCC = %d, FCC = %d, MOT = %d, SLD = %d, PAT = %d\n", num[CCC], num[FCC], num[MOT], num[SLD], num[PAT]);

			printf("sparseEncode: average RMSE = %f, numActiveQuadCels = %d, estSize = %d \n", CodecGetCurrentRMSE(qStatus), j,CodecGetCurrentQuadOutputSize(qStatus));
		  }

		  RoqWriteFrame(qStatus);
		  CodecMakePreviousImage(qStatus);

		  free(idataA);
		  free(idataB);
		  free(flist);
		  free(ilist);
		  if (newImage) ZimFree(newImage);

		  whichFrame++;

		  return 1;
}
