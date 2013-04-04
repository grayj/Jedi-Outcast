// Filename:-	oddbits.h
//

#ifndef ODDBITS_H
#define ODDBITS_H

char *va(char *format, ...);
bool FileExists (LPCSTR psFilename);

void ErrorBox(const char *sString);
void InfoBox(const char *sString);
void WarningBox(const char *sString);
//
// (Afx message boxes appear to be MB_TASKMODAL anyway, so no need to specify)
//
#define GetYesNo(psQuery) (!!(AfxMessageBox(psQuery,MB_YESNO|MB_ICONWARNING)==IDYES))


char *scGetTempPath(void);
char *InputLoadFileName(char *psInitialLoadName, char *psCaption, const char *psInitialDir, char *psFilter);
long filesize(FILE *handle);
//int  LoadFile (char *psPathedFilename, void **bufferptr);
void Filename_RemoveQUAKEBASE(CString &string);
void Filename_RemoveFilename(CString &string);
void Filename_BaseOnly(CString &string);

//#define StartWait() HCURSOR hcurSave = SetCursor(::LoadCursor(NULL, IDC_WAIT))
//#define EndWait()   SetCursor(hcurSave)

char *Filename_WithoutPath(LPCSTR psFilename);
char *Filename_WithoutExt(LPCSTR psFilename);
char *Filename_PathOnly(LPCSTR psFilename);
char *Filename_ExtOnly(LPCSTR psFilename);


#endif	// #ifndef ODDBITS_H

/////////////////// eof ////////////////////


