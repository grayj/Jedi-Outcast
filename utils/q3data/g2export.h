// Filename:-	g2export.h
//

#ifndef G2EXPORT_H
#define G2EXPORT_H

#ifndef LPCSTR
#define LPCSTR const char *
#endif


#ifdef __cplusplus
extern "C"
{
#endif
char *ExportGhoul2FromMD3(LPCSTR psFullPathedFilename, int iNumLODs, int iNumSurfaces, int iNumTags
						  //,LPCSTR *ppsFullPathedNameGLA /*= NULL*/
						  );

#ifdef __cplusplus
}
#endif





#endif	// #ifndef G2EXPORT_H

//////////////// eof ///////////////

