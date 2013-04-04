// Filename:-	JPGFile.h
//


#ifndef JPGFILE_H
#define JPGFILE_H

#ifdef __cplusplus
extern "C"
{
#endif



#ifndef LPCSTR 
typedef const char * LPCSTR;
#endif

// bool return not allowed by C compiler (JPG includes this, and is .c compile)
//
int LoadJPG( const char *filename, unsigned char **pic, int *width, int *height );

// special hacky shit for JPG C code to call back into CPP with stack throws... (aaarrggh)...
//
void JPG_ErrorThrow(LPCSTR message);
void JPG_MessageOut(LPCSTR message);
#define ERROR_STRING_NO_RETURN(message) JPG_ErrorThrow(message)
#define MESSAGE_STRING(message)			JPG_MessageOut(message)



#ifdef __cplusplus
};
#endif


#endif	// #ifndef JPGFILE_H

////////////////////// eof ////////////////////


