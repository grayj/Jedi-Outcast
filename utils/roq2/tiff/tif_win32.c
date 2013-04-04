/* $Header: /roq/tiff/tif_win32.c 1     11/02/99 4:40p Zaphod $ */



/*

 * Copyright (c) 1988-1996 Sam Leffler

 * Copyright (c) 1991-1996 Silicon Graphics, Inc.

 *

 * Permission to use, copy, modify, distribute, and sell this software and 

 * its documentation for any purpose is hereby granted without fee, provided

 * that (i) the above copyright notices and this permission notice appear in

 * all copies of the software and related documentation, and (ii) the names of

 * Sam Leffler and Silicon Graphics may not be used in any advertising or

 * publicity relating to the software without the specific, prior written

 * permission of Sam Leffler and Silicon Graphics.

 * 

 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 

 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 

 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  

 * 

 * IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR

 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,

 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,

 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 

 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 

 * OF THIS SOFTWARE.

 */



/*

 * TIFF Library Win32-specific Routines.  Adapted from tif_unix.c 4/5/95 by

 * Scott Wagner (wagner@itek.com), Itek Graphix, Rochester, NY USA

 */

#include <windows.h>

#include "tiffiop.h"



static tsize_t

_tiffReadProc(thandle_t fd, tdata_t buf, tsize_t size)

{

	DWORD dwSizeRead;

	if (!ReadFile(fd, buf, size, &dwSizeRead, NULL))

		return(0);

	return ((tsize_t) dwSizeRead);

}



static tsize_t

_tiffWriteProc(thandle_t fd, tdata_t buf, tsize_t size)

{

	DWORD dwSizeWritten;

	if (!WriteFile(fd, buf, size, &dwSizeWritten, NULL))

		return(0);

	return ((tsize_t) dwSizeWritten);

}



static toff_t

_tiffSeekProc(thandle_t fd, toff_t off, int whence)

{

	DWORD dwMoveMethod;

	switch(whence)

	{

	case 0:

		dwMoveMethod = FILE_BEGIN;

		break;

	case 1:

		dwMoveMethod = FILE_CURRENT;

		break;

	case 2:

		dwMoveMethod = FILE_END;

		break;

	default:

		dwMoveMethod = FILE_BEGIN;

		break;

	}

	return ((toff_t)SetFilePointer(fd, off, NULL, dwMoveMethod));

}



static int

_tiffCloseProc(thandle_t fd)

{

	return (CloseHandle(fd) ? 0 : -1);

}



static toff_t

_tiffSizeProc(thandle_t fd)

{

	return ((toff_t)GetFileSize(fd, NULL));

}



/*

 * Because Windows uses both a handle and a pointer for file mapping, and only

 * the pointer is returned, the handle must be saved for later use (by the

 * unmap function).  To do this, the tiff structure has an extra member,

 * pv_map_handle, which is contiguous with (4 bytes or one 32-bit word above)

 * the tif_base parameter which is passed as *pbase to the map function.

 * pv_map_handle is then accessed indirectly (and perhaps somewhat unsafely)

 * as an offset from the *pbase parameter by _tiffMapProc.  The handle thus

 * created and saved is destroyed by _tiffUnmapProc, which does not need size

 * in Win32 but receives the map handle value in the size parameter instead.

 */



#pragma argsused

static int

_tiffDummyMapProc(thandle_t fd, tdata_t* pbase, toff_t* psize)

{

	return(0);

}



static int

_tiffMapProc(thandle_t fd, tdata_t* pbase, toff_t* psize)

{

	toff_t size;

	HANDLE *phMapFile;

	if ((size = _tiffSizeProc(fd)) == (toff_t)-1)

		return(0);

	phMapFile = (HANDLE *)(((BYTE *)pbase) + 4);

	if ((*phMapFile = CreateFileMapping(fd, NULL, PAGE_READONLY, 0, size, NULL))

			== NULL)

		return(0);

	if ((*pbase = MapViewOfFile(*phMapFile, FILE_MAP_READ, 0, 0, 0)) ==

			NULL)

	{

		CloseHandle(*phMapFile);

		*phMapFile = NULL;

		return(0);

	}

	*psize = size;

	return(1);

}



#pragma argsused

static void

_tiffDummyUnmapProc(thandle_t fd, tdata_t base, toff_t size)

{

	return;

}



static void

_tiffUnmapProc(thandle_t fd, tdata_t base, toff_t map_handle)

{

	UnmapViewOfFile(base);

	CloseHandle((HANDLE)map_handle);

	return;

}



/*

 * Open a TIFF file descriptor for read/writing.

 * Note that TIFFFdOpen and TIFFOpen recognise the character 'u' in the mode

 * string, which forces the file to be opened unmapped.

 */

TIFF*

TIFFFdOpen(int ifd, const char* name, const char* mode)

{

	TIFF* tif;

	BOOL fSuppressMap = (mode[1] == 'u' || mode[2] == 'u');



	tif = TIFFClientOpen(name, mode,

		 (thandle_t)ifd,

	    _tiffReadProc, _tiffWriteProc,

	    _tiffSeekProc, _tiffCloseProc, _tiffSizeProc,

		 fSuppressMap ? _tiffDummyMapProc : _tiffMapProc,

		 fSuppressMap ? _tiffDummyUnmapProc : _tiffUnmapProc);

	if (tif)

		tif->tif_fd = ifd;

	return (tif);

}



/*

 * Open a TIFF file for read/writing.

 */

TIFF*

TIFFOpen(const char* name, const char* mode)

{

	static const char module[] = "TIFFOpen";

	thandle_t fd;

	int m;

	DWORD dwMode;



	m = _TIFFgetMode(mode, module);



	switch(m)

	{

	case O_RDONLY:

		dwMode = OPEN_EXISTING;

		break;

	case O_RDWR:

		dwMode = OPEN_ALWAYS;

		break;

	case O_RDWR|O_CREAT:

		dwMode = CREATE_NEW;

		break;

	case O_RDWR|O_TRUNC:

		dwMode = CREATE_ALWAYS;

		break;

	case O_RDWR|O_CREAT|O_TRUNC:

		dwMode = CREATE_ALWAYS;

		break;

	default:

		return ((TIFF*)0);

	}

	fd = (thandle_t)CreateFile(name, (m == O_RDONLY) ? GENERIC_READ :

			(GENERIC_READ | GENERIC_WRITE), FILE_SHARE_READ, NULL, dwMode,

			(m == O_RDONLY) ? FILE_ATTRIBUTE_READONLY : FILE_ATTRIBUTE_NORMAL, NULL);

	if (fd == INVALID_HANDLE_VALUE) {

		TIFFError(module, "%s: Cannot open", name);

		return ((TIFF *)0);

	}

	return (TIFFFdOpen((int)fd, name, mode));

}



tdata_t

_TIFFmalloc(tsize_t s)

{

	return ((tdata_t)GlobalAlloc(GMEM_FIXED, s));

}



void

_TIFFfree(tdata_t p)

{

	GlobalFree(p);

	return;

}



tdata_t

_TIFFrealloc(tdata_t p, tsize_t s)

{

	void* pvTmp;

	if ((pvTmp = GlobalReAlloc(p, s, 0)) == NULL) {

		if ((pvTmp = GlobalAlloc(GMEM_FIXED, s)) != NULL) {

			CopyMemory(pvTmp, p, s);

			GlobalFree(p);

		}

	}

	return ((tdata_t)pvTmp);

}



void

_TIFFmemset(void* p, int v, tsize_t c)

{

	FillMemory(p, c, (BYTE)v);

}



void

_TIFFmemcpy(void* d, const tdata_t s, tsize_t c)

{

	CopyMemory(d, s, c);

}



int

_TIFFmemcmp(const tdata_t p1, const tdata_t p2, tsize_t c)

{

	register const BYTE *pb1 = p1;

	register const BYTE *pb2 = p2;

	register DWORD dwTmp = c;

	register int iTmp;

	for (iTmp = 0; dwTmp-- && !iTmp; iTmp = (int)*pb1++ - (int)*pb2++)

		;

	return (iTmp);

}



static void

Win32WarningHandler(const char* module, const char* fmt, va_list ap)

{

	LPTSTR szTitle;

	LPTSTR szTmp;

	LPCTSTR szTitleText = "%s Warning";

	LPCTSTR szDefaultModule = "TIFFLIB";

	szTmp = (module == NULL) ? (LPTSTR)szDefaultModule : (LPTSTR)module;

	if ((szTitle = (LPTSTR)LocalAlloc(LMEM_FIXED, (lstrlen(szTmp) +

			lstrlen(szTitleText) + lstrlen(fmt) + 128)*sizeof(TCHAR))) == NULL)

		return;

	wsprintf(szTitle, szTitleText, szTmp);

	szTmp = szTitle + (lstrlen(szTitle)+2)*sizeof(TCHAR);

	wvsprintf(szTmp, fmt, ap);

	MessageBox(GetFocus(), szTmp, szTitle, MB_OK | MB_ICONINFORMATION);

	LocalFree(szTitle);

	return;

}

TIFFErrorHandler _TIFFwarningHandler = Win32WarningHandler;



static void

Win32ErrorHandler(const char* module, const char* fmt, va_list ap)

{

	LPTSTR szTitle;

	LPTSTR szTmp;

	LPCTSTR szTitleText = "%s Error";

	LPCTSTR szDefaultModule = "TIFFLIB";

	szTmp = (module == NULL) ? (LPTSTR)szDefaultModule : (LPTSTR)module;

	if ((szTitle = (LPTSTR)LocalAlloc(LMEM_FIXED, (lstrlen(szTmp) +

			lstrlen(szTitleText) + lstrlen(fmt) + 128)*sizeof(TCHAR))) == NULL)

		return;

	wsprintf(szTitle, szTitleText, szTmp);

	szTmp = szTitle + (lstrlen(szTitle)+2)*sizeof(TCHAR);

	wvsprintf(szTmp, fmt, ap);

	MessageBox(GetFocus(), szTmp, szTitle, MB_OK | MB_ICONEXCLAMATION);

	LocalFree(szTitle);

	return;

}

TIFFErrorHandler _TIFFerrorHandler = Win32ErrorHandler;

