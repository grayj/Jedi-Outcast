#include "stdafx.h"
#include "qe3.h"
#include "mru.h"
#include "PrefsDlg.h"
#include "oddbits.h"

extern CEdit* g_pEdit;

int	screen_width;
int	screen_height;
qboolean	have_quit;

int	update_bits;

HANDLE	bsp_process;

//===========================================

void Sys_MarkMapModified (void)
{
	char			title[1024];

	if (modified != 1)
	{
		modified = true;	// mark the map as changed
		sprintf (title, "%s *", currentmap);

		QE_ConvertDOSToUnixName( title, title );
		Sys_SetTitle (title);
	}
}


void Sys_SetTitle (char *text)
{
	SetWindowText (g_qeglobals.d_hwndMain, text);
}

HCURSOR	waitcursor;

void Sys_BeginWait (void)
{
	waitcursor = SetCursor (LoadCursor (NULL, IDC_WAIT));
}

void Sys_EndWait (void)
{
	if (waitcursor)
	{
		SetCursor (waitcursor);
		waitcursor = NULL;
	}
}


void Sys_GetCursorPos (int *x, int *y)
{
	POINT lpPoint;

	GetCursorPos (&lpPoint);
	*x = lpPoint.x;
	*y = lpPoint.y;
}

void Sys_SetCursorPos (int x, int y)
{
	SetCursorPos (x, y);
}


void Sys_Beep (void)
{
	MessageBeep (MB_ICONASTERISK);
}

char	*TranslateString (char *buf)
{
	static	char	buf2[32768];
	int		i, l;
	char	*out;

	l = strlen(buf);
	out = buf2;
	for (i=0 ; i<l ; i++)
	{
		if (buf[i] == '\n')
		{
			*out++ = '\r';
			*out++ = '\n';
		}
		else
			*out++ = buf[i];
	}
	*out++ = 0;

	return buf2;
}


void Sys_ClearPrintf (void)
{
	char	text[4];
	text[0] = 0;

  SendMessage (g_qeglobals.d_hwndEdit, WM_SETTEXT, 0,	(LPARAM)text);
}


//#define SCROLLBACK_MAX_LINES	600
//#define SCROLLBACK_DEL_CHARS	500
#define SCROLLBACK_MAX_LINES	300

void Sys_Printf (char *text, ...)
{
	va_list argptr;
	char	buf[32768];
	char	*out;
	LRESULT	result;				// PGM
//	DWORD	oldPosS, oldPosE;	// PGM

	va_start (argptr,text);
	vsprintf (buf, text,argptr);
	va_end (argptr);

	out = TranslateString (buf);

#ifdef LATER
	Sys_Status(out);
#else
//PGM
	result = SendMessage (g_qeglobals.d_hwndEdit, EM_GETLINECOUNT, 0, 0);
	if(result > SCROLLBACK_MAX_LINES)
	{
		// this doesn't work if the average number of chars per line is >86 (which exceeds 32k), so for now...
/*
		char	replaceText[5];
		
		replaceText[0] = '\0';

		SendMessage (g_qeglobals.d_hwndEdit, WM_SETREDRAW, (WPARAM)0, (LPARAM)0);
		SendMessage (g_qeglobals.d_hwndEdit, EM_GETSEL, (WPARAM)&oldPosS, (LPARAM)&oldPosE);
		SendMessage (g_qeglobals.d_hwndEdit, EM_SETSEL, 0, SCROLLBACK_DEL_CHARS);
		SendMessage (g_qeglobals.d_hwndEdit, EM_REPLACESEL, (WPARAM)0, (LPARAM)replaceText);
		SendMessage (g_qeglobals.d_hwndEdit, EM_SETSEL, oldPosS, oldPosE);
		SendMessage (g_qeglobals.d_hwndEdit, WM_SETREDRAW, (WPARAM)1, (LPARAM)0);
*/
		Sys_ClearPrintf();
		SendMessage (g_qeglobals.d_hwndEdit, EM_REPLACESEL, 0, (LPARAM) "(Output buffer full: clearing it)\r\n");
	}
//PGM

	SendMessage (g_qeglobals.d_hwndEdit, EM_REPLACESEL, 0, (LPARAM)out);
#endif

}


double Sys_DoubleTime (void)
{
	return clock()/ 1000.0;
}

void PrintPixels (HDC hDC)
{
	int		i;
	PIXELFORMATDESCRIPTOR p[64];

	printf ("### flags color layer\n");
	for (i=1 ; i<64 ; i++)
	{
		if (!DescribePixelFormat ( hDC, i, sizeof(p[0]), &p[i]))
			break;
		printf ("%3i %5i %5i %5i\n", i,
			p[i].dwFlags,
			p[i].cColorBits,
			p[i].bReserved);
	}
	printf ("%i modes\n", i-1);
}


/*
** ChoosePFD
**
** Helper function that replaces ChoosePixelFormat.
*/
#define MAX_PFDS 256

static int GLW_ChoosePFD( HDC hDC, PIXELFORMATDESCRIPTOR *pPFD )
{
	PIXELFORMATDESCRIPTOR pfds[MAX_PFDS+1];
	int maxPFD = 0;
	int i;
	int bestMatch = 0;

	OutputDebugString( va("...GLW_ChoosePFD( %d, %d, %d )\n", ( int ) pPFD->cColorBits, ( int ) pPFD->cDepthBits, ( int ) pPFD->cStencilBits) );

	// count number of PFDs
	//
	maxPFD = DescribePixelFormat( hDC, 1, sizeof( PIXELFORMATDESCRIPTOR ), &pfds[0] );

	if ( maxPFD > MAX_PFDS )
	{
		OutputDebugString( va( "...numPFDs > MAX_PFDS (%d > %d)\n", maxPFD, MAX_PFDS) );
		maxPFD = MAX_PFDS;
	}

	OutputDebugString( va("...%d PFDs found\n", maxPFD - 1) );

	FILE *handle = fopen("c:\\chcRadiant_GL_report.txt","wt");

	fprintf(handle,"Total PFDs: %d\n\n",maxPFD);

	// grab information
	for ( i = 1; i <= maxPFD; i++ )
	{
		DescribePixelFormat( hDC, i, sizeof( PIXELFORMATDESCRIPTOR ), &pfds[i] );

		fprintf(handle,"PFD %d/%d\n",i,maxPFD);
		fprintf(handle,"=========\n");		

#define FLAGDUMP(flag) if ( (pfds[i].dwFlags & flag ) != 0 ) fprintf(handle,"(flag: %s)\n",#flag);

		FLAGDUMP( PFD_DOUBLEBUFFER            );
		FLAGDUMP( PFD_STEREO                  );
		FLAGDUMP( PFD_DRAW_TO_WINDOW          );
		FLAGDUMP( PFD_DRAW_TO_BITMAP          );
		FLAGDUMP( PFD_SUPPORT_GDI             );
		FLAGDUMP( PFD_SUPPORT_OPENGL          );
		FLAGDUMP( PFD_GENERIC_FORMAT          );
		FLAGDUMP( PFD_NEED_PALETTE            );
		FLAGDUMP( PFD_NEED_SYSTEM_PALETTE     );
		FLAGDUMP( PFD_SWAP_EXCHANGE           );
		FLAGDUMP( PFD_SWAP_COPY               );
		FLAGDUMP( PFD_SWAP_LAYER_BUFFERS      );
		FLAGDUMP( PFD_GENERIC_ACCELERATED     );
		FLAGDUMP( PFD_SUPPORT_DIRECTDRAW      );

		if ( pfds[i].iPixelType == PFD_TYPE_RGBA )
		{
//			fprintf(handle,"RGBA mode\n");
		}
		else
		{
			fprintf(handle,"NOT RGBA mode!!!!!!!!!!!!\n");
		}

		fprintf(handle, "Colour bits: %d\n",pfds[i].cColorBits);
		fprintf(handle, "Depth  bits: %d\n",pfds[i].cDepthBits);

		fprintf(handle,"\n");
	}
	

	// look for a best match
	for ( i = 1; i <= maxPFD; i++ )
	{
		fprintf(handle,"(bestMatch: %d)\n",bestMatch );

		//
		// make sure this has hardware acceleration
		//
		if ( ( pfds[i].dwFlags & PFD_GENERIC_FORMAT ) != 0 ) 
		{
//			if ( !r_allowSoftwareGL->integer )
			{
//				if ( r_verbose->integer )
				{
					fprintf(handle,//OutputDebugString(
						 va ("...PFD %d rejected, software acceleration\n", i ));
				}
				continue;
			}
		}

		// verify pixel type
		if ( pfds[i].iPixelType != PFD_TYPE_RGBA )
		{
//			if ( r_verbose->integer )
			{
				fprintf(handle,//OutputDebugString(
					va("...PFD %d rejected, not RGBA\n", i) );
			}
			continue;
		}

		// verify proper flags
		if ( ( ( pfds[i].dwFlags & pPFD->dwFlags ) & pPFD->dwFlags ) != pPFD->dwFlags ) 
		{
//			if ( r_verbose->integer )
			{
				fprintf(handle,//OutputDebugString(
					va("...PFD %d rejected, improper flags (0x%x instead of 0x%x)\n", i, pfds[i].dwFlags, pPFD->dwFlags) );
			}
			continue;
		}

		// verify enough bits
		if ( pfds[i].cDepthBits < 15 )
		{
			fprintf(handle,va("...PFD %d rejected, depth bits only %d (<15)\n", i, pfds[i].cDepthBits) );
			continue;
		}
/*		if ( ( pfds[i].cStencilBits < 4 ) && ( pPFD->cStencilBits > 0 ) )
		{
			continue;
		}
*/
		//
		// selection criteria (in order of priority):
		// 
		//  PFD_STEREO
		//  colorBits
		//  depthBits
		//  stencilBits
		//
		if ( bestMatch )
		{
/*
			// check stereo
			if ( ( pfds[i].dwFlags & PFD_STEREO ) && ( !( pfds[bestMatch].dwFlags & PFD_STEREO ) ) && ( pPFD->dwFlags & PFD_STEREO ) )
			{
				bestMatch = i;
				continue;
			}
			
			if ( !( pfds[i].dwFlags & PFD_STEREO ) && ( pfds[bestMatch].dwFlags & PFD_STEREO ) && ( pPFD->dwFlags & PFD_STEREO ) )
			{
				bestMatch = i;
				continue;
			}
*/
			// check color
			if ( pfds[bestMatch].cColorBits != pPFD->cColorBits )
			{
				// prefer perfect match
				if ( pfds[i].cColorBits == pPFD->cColorBits )
				{
					bestMatch = i;
					continue;
				}
				// otherwise if this PFD has more bits than our best, use it
				else if ( pfds[i].cColorBits > pfds[bestMatch].cColorBits )
				{
					bestMatch = i;
					continue;
				}
			}

			// check depth
			if ( pfds[bestMatch].cDepthBits != pPFD->cDepthBits )
			{
				// prefer perfect match
				if ( pfds[i].cDepthBits == pPFD->cDepthBits )
				{
					bestMatch = i;
					continue;
				}
				// otherwise if this PFD has more bits than our best, use it
				else if ( pfds[i].cDepthBits > pfds[bestMatch].cDepthBits )
				{
					bestMatch = i;
					continue;
				}
			}
/*
			// check stencil
			if ( pfds[bestMatch].cStencilBits != pPFD->cStencilBits )
			{
				// prefer perfect match
				if ( pfds[i].cStencilBits == pPFD->cStencilBits )
				{
					bestMatch = i;
					continue;
				}
				// otherwise if this PFD has more bits than our best, use it
				else if ( ( pfds[i].cStencilBits > pfds[bestMatch].cStencilBits ) && 
					 ( pPFD->cStencilBits > 0 ) )
				{
					bestMatch = i;
					continue;
				}
			}
*/
		}
		else
		{
			bestMatch = i;
		}
	}

	fprintf(handle,"Bestmode: %d\n",bestMatch);
	
	if ( !bestMatch )
	{
		fprintf(handle,"No decent mode found!\n");
		fclose(handle);
		return 0;
	}

	if ( ( pfds[bestMatch].dwFlags & PFD_GENERIC_FORMAT ) != 0 )
	{
//		if ( !r_allowSoftwareGL->integer )
//		{
//			ri.Printf( PRINT_ALL, "...no hardware acceleration found\n" );
//			return 0;
//		}
//		else
		{
			fprintf(handle,//OutputDebugString(
				"...using software emulation\n" );
		}
	}
	else if ( pfds[bestMatch].dwFlags & PFD_GENERIC_ACCELERATED )
	{
		fprintf(handle,//OutputDebugString(
			"...MCD acceleration found\n" );
	}
	else
	{
		fprintf(handle,//OutputDebugString(
			"...hardware acceleration found\n" );
	}

	*pPFD = pfds[bestMatch];

	fclose(handle);

	return bestMatch;
}


//==========================================================================

void QEW_StopGL( HWND hWnd, HGLRC hGLRC, HDC hDC )
{
  qwglMakeCurrent( NULL, NULL );
  qwglDeleteContext( hGLRC );
	ReleaseDC( hWnd, hDC );
}
		
int QEW_SetupPixelFormat(HDC hDC, qboolean zbuffer )
{
    static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// size of this pfd
		1,								              // version number
		PFD_DRAW_TO_WINDOW |			      // support window
		PFD_SUPPORT_OPENGL |			      // support OpenGL
		PFD_DOUBLEBUFFER,				        // double buffered
		PFD_TYPE_RGBA,					        // RGBA type
		24,								              // 24-bit color depth
		0, 0, 0, 0, 0, 0,	              // color bits ignored
		0,								              // no alpha buffer
		0,								              // shift bit ignored
		0,								              // no accumulation buffer
		0, 0, 0, 0,						          // accum bits ignored
		32,							                // depth bits
		0,								              // no stencil buffer
		0,								              // no auxiliary buffer
		PFD_MAIN_PLANE,					        // main layer
		0,								              // reserved
		0, 0, 0							            // layer masks ignored
    };                              //
    int pixelformat = 0;            

	zbuffer = true;
	if ( !zbuffer )
		pfd.cDepthBits = 0;

  if (g_PrefsDlg.m_bSGIOpenGL)
  {
    if ( (pixelformat = qwglChoosePixelFormat(hDC, &pfd)) == 0 )
    {
	    printf("%d",GetLastError());
      Error ("ChoosePixelFormat failed");
    }
  
    if (!qwglSetPixelFormat(hDC, pixelformat, &pfd))
      Error ("SetPixelFormat failed");
  }
  else
  {
	  // try the pixel format chooser from the game first, then fall back to the normal one
	  //	if the game-chooser couldn't find one (which only happens on shit cards with no 3d support)
	  //
    if ( (pixelformat = GLW_ChoosePFD/*ChoosePixelFormat*/(hDC, &pfd)) == 0 
		&&
		 (pixelformat = ChoosePixelFormat(hDC, &pfd)) == 0 
		)
    {
	    printf("%d",GetLastError());
      Error ("ChoosePixelFormat failed");
    }

    if (!SetPixelFormat(hDC, pixelformat, &pfd))
      Error ("SetPixelFormat failed");
  }

	return pixelformat;
}

/*
=================
Error

For abnormal program terminations
=================
*/
void Error (char *error, ...)
{
	va_list argptr;
	char	text[1024];
	char	text2[1024];
	int		err;

	err = GetLastError ();
  int i = qglGetError();

	va_start (argptr,error);
	vsprintf (text, error,argptr);
	va_end (argptr);

	sprintf (text2, "%s\nGetLastError() = %i - %i\nAn unrecoverable error has occured. Would you like to edit Preferences before exiting QERadiant?", text, err, i);
  
  if (MessageBox(g_qeglobals.d_hwndMain, text2, "Error", MB_YESNO) == IDYES)
  {
    g_PrefsDlg.LoadPrefs();
    g_PrefsDlg.DoModal();
  }

	exit (1);
}


void Warning (char *error, ...)
{
	va_list argptr;
	char	text[1024];
	int		err;

	err = GetLastError ();
  int i = qglGetError();

	va_start (argptr,error);
	vsprintf (text, error,argptr);
	va_end (argptr);

  Sys_Printf(text);
}


/*
======================================================================

FILE DIALOGS

======================================================================
*/
 
qboolean ConfirmModified (void)
{
	if (!modified)
		return true;

	if (MessageBox (g_qeglobals.d_hwndMain, "This will lose changes to the map"
		, "warning", MB_OKCANCEL) == IDCANCEL)
		return false;
	return true;
}

static OPENFILENAME ofn;       /* common dialog box structure   */ 
static char szDirName[MAX_PATH];    /* directory string              */ 
static char szFile[260];       /* filename string               */ 
static char szFileTitle[260];  /* file title string             */ 
static char szFilter[260] =     /* filter string                 */ 
	"Map file (*.map, *.reg)\0*.map;*.reg\0\0";
static char szProjectFilter[260] =     /* filter string                 */ 
	"QERadiant project (*.qe4, *.prj)\0*.qe4;*.prj\0\0";
static char chReplace;         /* string separator for szFilter */ 
static int i, cbString;        /* integer count variables       */ 
static HANDLE hf;              /* file handle                   */ 

void OpenDialog (void)
{
	/* 
	 * Obtain the system directory name and 
	 * store it in szDirName. 
	 */ 
 
	strcpy (szDirName, ValueForKey (g_qeglobals.d_project_entity, "mapspath") );
  if (strlen(szDirName) == 0)
  {
	  strcpy (szDirName, ValueForKey (g_qeglobals.d_project_entity, "basepath") );
	  strcat (szDirName, "\\maps");
  }

	/* Place the terminating null character in the szFile. */ 
 
	szFile[0] = '\0'; 
 
	/* Set the members of the OPENFILENAME structure. */ 
 
	ofn.lStructSize = sizeof(OPENFILENAME); 
	ofn.hwndOwner = g_qeglobals.d_hwndCamera;
	ofn.lpstrFilter = szFilter; 
	ofn.nFilterIndex = 1; 
	ofn.lpstrFile = szFile; 
	ofn.nMaxFile = sizeof(szFile); 
	ofn.lpstrFileTitle = szFileTitle; 
	ofn.nMaxFileTitle = sizeof(szFileTitle); 
	ofn.lpstrInitialDir = szDirName; 
	ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | 
		OFN_FILEMUSTEXIST; 

	/* Display the Open dialog box. */ 
 
	if (!GetOpenFileName(&ofn))
		return;	// canceled
 
	// Add the file in MRU.
  //FIXME
	AddNewItem( g_qeglobals.d_lpMruMenu, ofn.lpstrFile);

	// Refresh the File menu.
  //FIXME
	PlaceMenuMRUItem(g_qeglobals.d_lpMruMenu,GetSubMenu(GetMenu(g_qeglobals.d_hwndMain),0), 
			ID_FILE_EXIT);

	/* Open the file. */ 
 
	Map_LoadFile (ofn.lpstrFile);	
}

void ProjectDialog (void)
{
	/* 
	 * Obtain the system directory name and 
	 * store it in szDirName. 
	 */ 
 
	strcpy (szDirName, ValueForKey(g_qeglobals.d_project_entity, "basepath") );
	strcat (szDirName, "\\scripts");

	/* Place the terminating null character in the szFile. */ 
 
	szFile[0] = '\0'; 
 
	/* Set the members of the OPENFILENAME structure. */ 
 
	ofn.lStructSize = sizeof(OPENFILENAME); 
	ofn.hwndOwner = g_qeglobals.d_hwndCamera;
	ofn.lpstrFilter = szProjectFilter; 
	ofn.nFilterIndex = 1; 
	ofn.lpstrFile = szFile; 
	ofn.nMaxFile = sizeof(szFile); 
	ofn.lpstrFileTitle = szFileTitle; 
	ofn.nMaxFileTitle = sizeof(szFileTitle); 
	ofn.lpstrInitialDir = szDirName; 
	ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | 
		OFN_FILEMUSTEXIST; 

	/* Display the Open dialog box. */ 
 
	if (!GetOpenFileName(&ofn))
		return;	// canceled
 
	// Refresh the File menu.
	PlaceMenuMRUItem(g_qeglobals.d_lpMruMenu,GetSubMenu(GetMenu(g_qeglobals.d_hwndMain),0), 
			ID_FILE_EXIT);

	/* Open the file. */ 
	if (!QE_LoadProject(ofn.lpstrFile))
		Error ("Couldn't load project file");
}


extern void AddSlash(CString& strPath);
void SaveAsDialog (bool bRegion)
{ 
	strcpy (szDirName, ValueForKey (g_qeglobals.d_project_entity, "basepath") );
  CString strPath = szDirName;
  AddSlash(strPath);
  strPath += "maps";

	/* Place the terminating null character in the szFile. */ 
 
	szFile[0] = '\0'; 
 
	/* Set the members of the OPENFILENAME structure. */ 
 
	ofn.lStructSize = sizeof(OPENFILENAME); 
	ofn.hwndOwner = g_qeglobals.d_hwndCamera;
	ofn.lpstrFilter = szFilter; 
	ofn.nFilterIndex = 1; 
	ofn.lpstrFile = szFile; 
	ofn.nMaxFile = sizeof(szFile); 
	ofn.lpstrFileTitle = szFileTitle; 
	ofn.nMaxFileTitle = sizeof(szFileTitle); 
	ofn.lpstrInitialDir = strPath; 
	ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | 
		OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT; 

	/* Display the Open dialog box. */ 
 
	if (!GetSaveFileName(&ofn))
		return;	// canceled

  if (bRegion)
	  DefaultExtension (ofn.lpstrFile, ".reg");
  else
	  DefaultExtension (ofn.lpstrFile, ".map");

  if (!bRegion)
  {
	  strcpy (currentmap, ofn.lpstrFile);
    AddNewItem(g_qeglobals.d_lpMruMenu, ofn.lpstrFile);
	  PlaceMenuMRUItem(g_qeglobals.d_lpMruMenu,GetSubMenu(GetMenu(g_qeglobals.d_hwndMain),0),	ID_FILE_EXIT);
  }
	Map_SaveFile (ofn.lpstrFile, bRegion);	// ignore region
}

/*
=======================================================

Menu modifications

=======================================================
*/

/*
==================
FillBSPMenu

==================
*/
char	*bsp_commands[256];

void FillBSPMenu (void)
{
	HMENU	hmenu;
	epair_t	*ep;
	int		i;
	static int count;

	hmenu = GetSubMenu (GetMenu(g_qeglobals.d_hwndMain), MENU_BSP);

	for (i=0 ; i<count ; i++)
		DeleteMenu (hmenu, CMD_BSPCOMMAND+i, MF_BYCOMMAND);
	count = 0;

	i = 0;
	for (ep = g_qeglobals.d_project_entity->epairs ; ep ; ep=ep->next)
	{
		if (ep->key[0] == 'b' && ep->key[1] == 's' && ep->key[2] == 'p')
		{
			bsp_commands[i] = ep->key;
			AppendMenu (hmenu, MF_ENABLED|MF_STRING,
			CMD_BSPCOMMAND+i, (LPCTSTR)ep->key);
			i++;
		}
	}
	count = i;
}

//==============================================
void AddSlash(CString& strPath)
{
  if (strPath.GetLength() > 0)
  {
    if (strPath.GetAt(strPath.GetLength()-1) != '\\')
      strPath += '\\';
  }
}


bool ExtractPath_and_Filename(const char* pPath, CString& strPath, CString& strFilename)
{
  CString strPathName = pPath;
  int nSlash = strPathName.ReverseFind('\\');
  if (nSlash >= 0)
  {
    strPath = strPathName.Left(nSlash+1);
    strFilename = strPathName.Right(strPathName.GetLength() - nSlash - 1);
  }
  else strFilename = pPath;
  return true;
}


/*
===============
CheckBspProcess

See if the BSP is done yet
===============
*/
extern void FindReplace(CString& strContents, const char* pTag, const char* pValue);
extern CTime g_tBegin;

void CheckBspProcess (void)
{
	char	outputpath[1024];
	char	temppath[512];
	DWORD	exitcode;
	char	*out;
	BOOL	ret;

	if (!bsp_process)
		return;

	ret = GetExitCodeProcess (bsp_process, &exitcode);
	if (!ret)
		Error ("GetExitCodeProcess failed");
	if (exitcode == STILL_ACTIVE)
		return;

	bsp_process = 0;

	GetTempPath(512, temppath);
	sprintf (outputpath, "%sjunk.txt", temppath);

	LoadFile (outputpath, (void **)&out);
	Sys_Printf ("%s", out);
	Sys_Printf ("\ncompleted.\n");
	free (out);

  CTime tEnd = CTime::GetCurrentTime();
  CTimeSpan tElapsed = tEnd - g_tBegin;
  CString strElapsed;
  strElapsed.Format("Run time was %i hours, %i minutes and %i seconds", tElapsed.GetHours(), tElapsed.GetMinutes(), tElapsed.GetSeconds());
	Sys_Printf(strElapsed.GetBuffer(0));


	Sys_Beep ();
	Pointfile_Check();
  if (g_PrefsDlg.m_bRunQuake == TRUE)
  {
    char cCurDir[1024];
    GetCurrentDirectory(1024, cCurDir);
    CString strExePath = g_PrefsDlg.m_strQuake2;
    CString strOrgPath;
    CString strOrgFile;
    ExtractPath_and_Filename(currentmap, strOrgPath, strOrgFile);
    if (g_PrefsDlg.m_bSetGame == TRUE) // run in place with set game.. don't copy map
    {
	    CString strBasePath = ValueForKey(g_qeglobals.d_project_entity, "basepath");
      strExePath += " +set game ";
      strExePath += strBasePath;
      WinExec(strExePath, SW_SHOW);
    }
    else
    {
      CString strCopyPath = strExePath;
      char* pBuffer = strCopyPath.GetBufferSetLength(_MAX_PATH + 1);
      pBuffer[strCopyPath.ReverseFind('\\') + 1] = '\0';
      strCopyPath.ReleaseBuffer();
      SetCurrentDirectory(strCopyPath);
      CString strOrgPath;
      CString strOrgFile;
      ExtractPath_and_Filename(currentmap, strOrgPath, strOrgFile);
      AddSlash(strCopyPath);
      FindReplace(strOrgFile, ".map", ".bsp");
      strCopyPath += "\\baseq2\\maps\\";
      strCopyPath += strOrgFile;
      AddSlash(strOrgPath);
      strOrgPath += strOrgFile;
      bool bRun = (strOrgPath.CompareNoCase(strCopyPath) == 0);
      if (!bRun)
        bRun = (CopyFile(strOrgPath, strCopyPath, FALSE) == TRUE);
      if (bRun)
      {
        FindReplace(strOrgFile, ".bsp", "");
        strExePath += " +map ";
        strExePath += strOrgFile;
        WinExec(strExePath, SW_SHOW);
      }
    }
    SetCurrentDirectory(cCurDir);
  }
}

extern int	cambuttonstate;


