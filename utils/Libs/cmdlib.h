//
// start of shared cmdlib stuff
// 

#ifndef __CMDLIB__
#define __CMDLIB__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>

#ifndef __BYTEBOOL__
#define __BYTEBOOL__

#ifndef __cplusplus
  typedef enum {false, true} boolean;
#else
  typedef unsigned char boolean;
#endif


typedef unsigned char byte;
//typedef unsigned char byte;
#endif

FILE	*SafeOpenWrite (const char *filename);
FILE	*SafeOpenRead (const char *filename);
void	SafeRead (FILE *f, void *buffer, int count);
void	SafeWrite (FILE *f, void *buffer, int count);
int		LoadFile (const char *filename, void **bufferptr);
int		LoadFileNoCrash (const char *filename, void **bufferptr);
void	SaveFile (const char *filename, void *buffer, int count);
void 	DefaultExtension (char *path, char *extension);
void 	DefaultPath (char *path, char *basepath);
void 	StripFilename (char *path);
void 	StripExtension (char *path);
void 	ExtractFilePath (const char *path, char *dest);
void	ExtractFileName (const char *path, char *dest);
void 	ExtractFileBase (const char *path, char *dest);
void	ExtractFileExtension (const char *path, char *dest);
short	BigShort (short l);
short	LittleShort (short l);
int		BigLong (int l);
int		LittleLong (int l);
float	BigFloat (float l);
float	LittleFloat (float l);
void *qmalloc (size_t size);
void* qblockmalloc(size_t nSize);



// error and printf functions
typedef void (PFN_ERR)(const char *pFormat, ...);
typedef void (PFN_PRINTF)(const char *pFormat, ...);
typedef void (PFN_ERR_NUM)(int nNum, const char *pFormat, ...);
typedef void (PFN_PRINTF_NUM)(int nNum, const char *pFormat, ...);

void Error(const char *pFormat, ...);
void Printf(const char *pFormat, ...);
void ErrorNum(int n, const char *pFormat, ...);
void PrintfNum(int n, const char *pFormat, ...);

void SetErrorHandler(PFN_ERR pe);
void SetPrintfHandler(PFN_PRINTF pe);
void SetErrorHandlerNum(PFN_ERR_NUM pe);
void SetPrintfHandlerNum(PFN_PRINTF_NUM pe);
void ConvertDOSToUnixName( char *dst, const char *src );
char* StrDup(char* pStr);
char* StrDup(const char* pStr);


#endif