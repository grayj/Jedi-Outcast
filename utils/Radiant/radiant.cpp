// Radiant.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Radiant.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "RadiantDoc.h"
#include "RadiantView.h"
#include "PrefsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRadiantApp

BEGIN_MESSAGE_MAP(CRadiantApp, CWinApp)
	//{{AFX_MSG_MAP(CRadiantApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRadiantApp construction

CRadiantApp::CRadiantApp(LPCTSTR lpszAppName/*=NULL*/)     // app name defaults to EXE name
:CWinApp(lpszAppName)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRadiantApp object

#ifdef QUAKE3
CRadiantApp theApp("Q3Trek_Radiant");
#else
CRadiantApp theApp;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRadiantApp initialization

HINSTANCE g_hOpenGL32 = NULL;
HINSTANCE g_hOpenGL = NULL;
bool g_bBuildList = false;

BOOL CRadiantApp::InitInstance()
{
  //g_hOpenGL32 = ::LoadLibrary("opengl32.dll");
	// AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
  //AfxEnableMemoryTracking(FALSE);

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey("QERadiant");

	LoadStdProfileSettings(_AFX_MRU_MAX_COUNT);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

//	CMultiDocTemplate* pDocTemplate;
//	pDocTemplate = new CMultiDocTemplate(
//		IDR_RADIANTYPE,
//		RUNTIME_CLASS(CRadiantDoc),
//		RUNTIME_CLASS(CMainFrame), // custom MDI child frame
//		RUNTIME_CLASS(CRadiantView));
//	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window

  g_PrefsDlg.LoadPrefs();

  CString strOpenGL = (g_PrefsDlg.m_bSGIOpenGL) ? "opengl.dll" : "opengl32.dll";
  CString strGLU = (g_PrefsDlg.m_bSGIOpenGL) ? "glu.dll" : "glu32.dll";
  
  if (!QGL_Init(strOpenGL, strGLU))
  {
    g_PrefsDlg.m_bSGIOpenGL ^= 1;
    strOpenGL = (g_PrefsDlg.m_bSGIOpenGL) ? "opengl.dll" : "opengl32.dll";
    strGLU = (g_PrefsDlg.m_bSGIOpenGL) ? "glu.dll" : "glu32.dll";
    if (!QGL_Init(strOpenGL, strGLU))
    {
      AfxMessageBox("Failed to load OpenGL libraries. \"OPENGL32.DLL\" and \"OPENGL.DLL\" were tried");
      return FALSE;
    }
    g_PrefsDlg.SavePrefs();
  }


  CString strTemp = m_lpCmdLine;
  strTemp.MakeLower();
  if (strTemp.Find("builddefs") >= 0)
    g_bBuildList = true;



	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

  if (pMainFrame->m_hAccelTable)
	{
    ::DestroyAcceleratorTable(pMainFrame->m_hAccelTable);
		pMainFrame->m_hAccelTable=NULL;
	}
  
  pMainFrame->LoadAccelTable(MAKEINTRESOURCE(IDR_MINIACCEL));


	m_pMainWnd = pMainFrame;

  // Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.m_strFileName.CompareNoCase("builddefs")==-1)
	{
		// Dispatch commands specified on the command line..... if command line was not "builddefs"
//		if (!ProcessShellCommand(cmdInfo))
//			return FALSE;
	}

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CRadiantApp commands

int CRadiantApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
  //::FreeLibrary(g_hOpenGL32);
	QGL_Shutdown();
	return CWinApp::ExitInstance();
}

BOOL CRadiantApp::OnIdle(LONG lCount) 
{
	if (g_pParentWnd)
    g_pParentWnd->RoutineProcessing();
	return CWinApp::OnIdle(lCount);
}
