#include "sdsc.h"
#include "im.h"
#include "Zim.h"
#include <stdlib.h>
#include <string.h>

void ZimFree(TagTable *image)
{
  ImVfb *vfb;
  if (image)
	{
	  TagEntryQValue(TagTableQDirect(image, "image vfb", 0), &vfb);
	  ImVfbFree(vfb);
	  TagTableFree(image);
	}
}

int ZimWidth(TagTable *image)
{
  ImVfb *vfb;
  TagEntryQValue(TagTableQDirect(image,"image vfb", 0), &vfb);
  return ImVfbQWidth(vfb);
}

int ZimHeight(TagTable *image)
{
  ImVfb *vfb;
  TagEntryQValue(TagTableQDirect(image,"image vfb", 0), &vfb);
  return ImVfbQHeight(vfb);
}

int ZimFields(TagTable *image)
{
  ImVfb *vfb;
  TagEntryQValue(TagTableQDirect(image,"image vfb", 0), &vfb);
  return  ImVfbQFields(vfb);
}

void *ZimData(TagTable *image)
{
  ImVfb *vfb;
  TagEntryQValue(TagTableQDirect(image,"image vfb", 0), &vfb);
  return (void *) vfb->vfb_pfirst;
}

int ZimBpp(TagTable *image)
{
  ImVfb *vfb;
  TagEntryQValue(TagTableQDirect(image,"image vfb", 0), &vfb);
  return  vfb->vfb_nbytes;
}

BOOL ZimIsPlanar(TagTable *image)
{
  ImVfb *vfb;
  TagEntryQValue(TagTableQDirect(image,"image vfb", 0), &vfb);
  return  (vfb->vfb_goff - vfb->vfb_roff) > 1 ? 1:0 ;
}

BOOL ZimHasAlpha(TagTable *image)
{
	return 0;
//  ImVfb *vfb;
//  TagEntryQValue(TagTableQDirect(image,"image vfb", 0), &vfb);
//  return  ImVfbQFields(vfb) & IMVFBALPHA ? 1:0;
}

void ZimResize(TagTable *image, int newx, int newy)
{
  ImVfb *vfb;
  ImVfb *newt;

  TagEntry   *tmpEntry;		/* Tmp table entry holder	*/
  int	     nEntry;		/* Tag entry #			*/

  TagEntryQValue(TagTableQDirect(image,"image vfb", 0), &vfb);
  newt = ImVfbResize( vfb, IMVFBBILINEAR, NULL, newx, newy );

  /* Replace the VFB.						*/
  tmpEntry = TagTableQDirect( image, "image vfb", 0 );
  nEntry   = TagEntryQNthEntry( tmpEntry );
  TagTableReplace( image, TagEntryQNthEntry( tmpEntry ), TagEntryAlloc( "image vfb", POINTER, &newt ) );

  ImVfbFree(vfb);
  return;
}

TagTable *ZimImageTagTableFromFile(const char *filename, const char *informat)
{
  FILE* fp;		/* File pointer   */
  TagTable* flagsTable;	/* Information about how to read the image */
  TagTable* dataTable;	/* The image data */
  char *test;
  FILE *err;
  int ret;
  char format[1025];
  
  if (informat && *informat)
  {
  	strcpy(format, informat);
  }
  else
  {
  	format[0]='\0';
  }
  err = stderr;
  /* Open the file */
  if (!(fp = fopen(filename,"rb")))
  {
  	fprintf(stderr,"Could not open %s\n", filename);
	exit(4);
  }

	if ( (dataTable = TagTableAlloc( )) == TAGTABLENULL )
	{
		TagPError( "roqvq" );
		exit( 1 );
	}

	if ( (flagsTable = TagTableAlloc( )) == TAGTABLENULL )
	{
		TagPError( "roqvq" );
		exit( 1 );
	}
  
  /* Add the file name to the flags table */
  TagTableAppend( flagsTable,
				 TagEntryAlloc( "file name", POINTER, &filename));
  TagTableAppend( flagsTable,
				 TagEntryAlloc( "error stream", POINTER, &err));
	  TagEntryQValue(TagTableQDirect(flagsTable, "file name", 0), &test);
  printf(test);
  printf("\n");
  /* Read the file, getting the name of the format */
  ret = ImFileFRead(fp, format, flagsTable, dataTable);
  if (ret == -1)
  {
  	fprintf(stderr, "Problem reading %s, return code = %d, ImErrNo = %d\n", filename, ret, ImErrNo);
	ImPError("roqvq: ZimImageTagTableFromFile");
  }
  fclose(fp);
 
  return  dataTable;
}

TagTable *ZimImageTagTableNew(int width, int height, int fields)
{
  ImVfb *vfb;
  TagTable *image;
  image = TagTableAlloc();
  if (image == TAGTABLENULL)
  {
  	perror("Couldn't allocate an image");
  }				

  vfb = ImVfbAlloc(width, height, fields);
  if (vfb == IMVFBNULL)
  {
  	perror("Couldn't allocate an image");
  }					 
  ImVfbClear(IMVFBALL, 0, vfb);
  TagTableAppend(image, TagEntryAlloc("image vfb", POINTER, &vfb));
    return  image;
}

int ZimWriteJFIF(TagTable *image,const char *filename, int quality)
{
  TagTable *flagsTable;
  FILE *fp;
  int ret, alphaRequest;
  FILE *err;
  
  err = stderr;
  
  flagsTable = TagTableAlloc();
  
  TagTableAppend( flagsTable,
	TagEntryAlloc( "file name", POINTER,  &filename));

  TagTableAppend( flagsTable,
	TagEntryAlloc( "image compression quality request", INT, &quality));
  
  alphaRequest = IMALPHANODUMP;
  TagTableAppend( flagsTable,
	TagEntryAlloc( "image alpha request", INT, &alphaRequest));
  
  TagTableAppend( flagsTable,
				 TagEntryAlloc( "error stream", POINTER, &err));

  printf("ZimWriteJFIF: %dx%d, hasAlpha = %d, bpp = %d, planar = %d\n", ZimWidth(image), ZimHeight(image), ZimHasAlpha(image), ZimBpp(image), ZimIsPlanar(image));
  
  /* Write the data out as a JPEG image file */
  fp = fopen(filename,"wb");
  ret = ImFileFWrite( fp, "jpeg", flagsTable, image);
  if (ret != 1)
  {
  	fprintf(stderr, "Problem saving %s, return code = %d, ImErrNo = %d\n", filename, ret, ImErrNo);
	ImPError("ZimWriteJFIF");
  }
  fclose(fp);
  TagTableFree(flagsTable);
  
  return ret;
}

void ZimGetDataPlanes(TagTable *image, byte **iPlanes)
{
  ImVfb *vfb;
  TagEntryQValue(TagTableQDirect(image,"image vfb", 0), &vfb);
  iPlanes[0] = vfb->vfb_pfirst + vfb->vfb_roff;
  iPlanes[1] = vfb->vfb_pfirst + vfb->vfb_goff;
  iPlanes[2] = vfb->vfb_pfirst + vfb->vfb_boff;
  iPlanes[3] = vfb->vfb_pfirst + vfb->vfb_aoff;
  return;
}

TagTable *ZimExtractField(TagTable *image)
{
  TagTable *newImage;
  ImVfb *vfb, *newVfb;
  int line, pixel;
  ImVfbPtr im, nim;
  char colorVal;
  
  newImage = ZimImageTagTableNew(ZimWidth(image), ZimHeight(image)/2, ZimFields(image));
  TagEntryQValue(TagTableQDirect(image,"image vfb", 0), &vfb);
  TagEntryQValue(TagTableQDirect(newImage,"image vfb", 0), &newVfb);

  for (line = 0; line < ZimHeight(image); line+=2)
	{
	  for (pixel = 0; pixel < ZimWidth(image); pixel++)
		{
		  im = ImVfbQPtr(vfb, pixel, line);
		  nim = ImVfbQPtr(newVfb, pixel, line/2);
		  colorVal = ImVfbQRed(vfb, im);
		  ImVfbSRed(newVfb, nim, colorVal);
		  colorVal = ImVfbQGreen(vfb, im);
		  ImVfbSGreen(newVfb, nim, colorVal);
		  colorVal = ImVfbQBlue(vfb, im);
		  ImVfbSBlue(newVfb, nim, colorVal);
		}
	}
  
  return newImage;
}
