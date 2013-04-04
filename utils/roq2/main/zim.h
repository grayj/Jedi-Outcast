
#ifndef _ZIM_H_INCLUDED
#define _ZIM_H_INCLUDED

#include "gdefs.h"

void ZimFree(TagTable *image);
int ZimWidth(TagTable *image);
int ZimHeight(TagTable *image);
int ZimFields(TagTable *image);
void *ZimData(TagTable *image);
int ZimBpp(TagTable *image);
BOOL ZimIsPlanar(TagTable *image);
BOOL ZimHasAlpha(TagTable *image);
TagTable *ZimImageTagTableFromFile(const char *filename, const char *format);
TagTable *ZimImageTagTableNew(int width, int height, int fields);
int ZimWriteJFIF(TagTable *image,const char *filename, int quality);
void ZimGetDataPlanes(TagTable *image, byte **iPlanes);
TagTable *ZimExtractField(TagTable *image);
void ZimResize(TagTable *image, int newx, int newy);

#endif //_ZIM_H_INCLUDED
