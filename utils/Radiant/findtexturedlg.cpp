// FindTextureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Radiant.h"
#include "FindTextureDlg.h"
#include "oddbits.h"

#pragma warning(disable : 4786)     // shut the fuck up about debug symbol name length
#include <list>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void PrintReplacementCount(int iReplacements, bool bSelectOnlyNoReplace)
{
	if (iReplacements)
	{
		if (bSelectOnlyNoReplace)
		{	
			Sys_Printf(va("( Selected %d brushes with matching textures )\n",iReplacements));
		}
		else
		{
			Sys_Printf(va("( Replaced %d search-matching texture usages )\n",iReplacements));
		}
	}
	else
	{
		Sys_Printf("( No search-matching textures usages found )\n");
	}
}


// Ideally I'd have put this above the original version it overrides, and added it to the H file etc, but the
//	problem is that that'd mean making all other modules that included that header file STL_aware, and I just 
//	can't be bothered, besides this is the only place in the code that calls the other one in this fashion.
//
typedef pair<CString, CString> FindReplaceStringPair_t;
typedef list<FindReplaceStringPair_t> FindReplaceList_t;
//
// return is simply a count of how many replacements took place, used for friendly stat printing...
//
int FindReplaceTextures(FindReplaceList_t &FindReplaceList, bool bSelected, bool bForce, bool bReScale, bool bSelectOnlyNoReplace)
{
	CWaitCursor wait;

	int iReplacedTextures = 0;

	int iLoopCount= FindReplaceList.size();
	int iLoopIter = 0;

	for (FindReplaceList_t::iterator it = FindReplaceList.begin(); it != FindReplaceList.end(); ++it, iLoopIter++)
	{	
		CString strFind		= (*it).first;
		CString strReplace	= (*it).second;

		Sys_Printf(va("Loop %d/%d, replacing \"%s\" with \"%s\"...",iLoopIter+1,iLoopCount, (LPCSTR)strFind, (LPCSTR)strReplace));

		int iThisReplacedTextures = FindReplaceTextures(strFind, strReplace, bSelected, bForce, bReScale, bSelectOnlyNoReplace,
														true,					// bInhibitCameraUpdate
														iLoopIter?true:false	// bCalledDuringLoopAndNotFirstTime
														);

		Sys_Printf(va("%d found\n",iThisReplacedTextures));

		iReplacedTextures += iThisReplacedTextures;
	}

	if (iReplacedTextures)	// :-)
	{
		Sys_UpdateWindows (W_CAMERA);	// since we inhibited the one in the function we called
	}

	return iReplacedTextures;
}




/////////////////////////////////////////////////////////////////////////////
// CFindTextureDlg dialog

CFindTextureDlg g_TexFindDlg;
CFindTextureDlg& g_dlgFind = g_TexFindDlg;
static bool g_bFindActive = true;
static bool gbIsOpen = false;	// needed, or the .isopen() member doesn't actually work

void CFindTextureDlg::updateTextures(const char *p)
{
  if (isOpen())
  {
    if (g_bFindActive)
    {
      setFindStr(p);
    }
    else
    {
      setReplaceStr(p);
    }
  }
}


CFindTextureDlg::CFindTextureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindTextureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindTextureDlg)
	m_bSelectedOnly = FALSE;
	m_strFind = _T("");
	m_strReplace = _T("");
	m_bForce = FALSE;
	m_bLive = TRUE;
	m_bReScale = TRUE;
	m_bSelectOnlyNoReplace = FALSE;
	//}}AFX_DATA_INIT
}


void CFindTextureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindTextureDlg)
	DDX_Check(pDX, IDC_CHECK_SELECTED, m_bSelectedOnly);
	DDX_Text(pDX, IDC_EDIT_FIND, m_strFind);
	DDX_Text(pDX, IDC_EDIT_REPLACE, m_strReplace);
	DDX_Check(pDX, IDC_CHECK_FORCE, m_bForce);
	DDX_Check(pDX, IDC_CHECK_LIVE, m_bLive);
	DDX_Check(pDX, IDC_CHECK_RESCALE, m_bReScale);
	DDX_Check(pDX, IDC_CHECK_SELECTONLY, m_bSelectOnlyNoReplace);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindTextureDlg, CDialog)
	//{{AFX_MSG_MAP(CFindTextureDlg)
	ON_BN_CLICKED(ID_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_BUTTON_BATCHFINDREPLACE, OnButtonBatchfindreplace)
	ON_EN_SETFOCUS(IDC_EDIT_FIND, OnSetfocusEditFind)
	ON_EN_SETFOCUS(IDC_EDIT_REPLACE, OnSetfocusEditReplace)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_SELECTONLY, OnCheckSelectonly)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
				   
LPCSTR scGetUserName(void);

void CFindTextureDlg::OnBtnApply() 
{
	SaveToRegistry();

	if (m_bSelectOnlyNoReplace && !stricmp(scGetUserName(),"ebiessman"))
	{
		if (!GetYesNo("Are you *really* sure you want to do this, Eric?\n\nI mean, *really* sure?"))
			return;
	}
	CWaitCursor wait;
	int iReplacements = FindReplaceTextures(m_strFind, m_strReplace, m_bSelectedOnly, m_bForce, m_bReScale, m_bSelectOnlyNoReplace);
	PrintReplacementCount(iReplacements, m_bSelectOnlyNoReplace);
}

void CFindTextureDlg::OnOK() 
{
	SaveToRegistry();

	if (m_bSelectOnlyNoReplace && !stricmp(scGetUserName(),"ebiessman"))
	{
		if (!GetYesNo("Are you *really* sure you want to do this, Eric?\n\nI mean, *really* sure?"))
			return;
	}
	
	CWaitCursor wait;
	int iReplacements = FindReplaceTextures(m_strFind, m_strReplace, m_bSelectedOnly, m_bForce, m_bReScale, m_bSelectOnlyNoReplace);
	PrintReplacementCount(iReplacements, m_bSelectOnlyNoReplace);
	
	CDialog::OnOK();

	gbIsOpen = false;
}

void CFindTextureDlg::show()
{
  if (g_dlgFind.GetSafeHwnd() == NULL || IsWindow(g_dlgFind.GetSafeHwnd()) == FALSE)
  {
    g_dlgFind.Create(IDD_DIALOG_FINDREPLACE);
    g_dlgFind.ShowWindow(SW_SHOW);
  }
  else
  {
    g_dlgFind.ShowWindow(SW_SHOW);
  }
  CRect rct;
  LONG lSize = sizeof(rct);
  if (LoadRegistryInfo("Radiant::TextureFindWindow", &rct, &lSize))
    g_dlgFind.SetWindowPos(NULL, rct.left, rct.top, 0,0, SWP_NOSIZE | SWP_SHOWWINDOW);  

  g_dlgFind.LoadFromRegistry();

  g_dlgFind.HandleItemGreying();

  gbIsOpen = true;
}


bool CFindTextureDlg::isOpen()
{		
  //return (g_dlgFind.GetSafeHwnd() == NULL || IsWindow(g_dlgFind.GetSafeHwnd()) == FALSE) ? false : true;
	return gbIsOpen;
}

void CFindTextureDlg::setFindStr(const char * p)
{
  g_dlgFind.UpdateData(TRUE);
  if (g_dlgFind.m_bLive)
  {
    g_dlgFind.m_strFind = p;
    g_dlgFind.UpdateData(FALSE);
  }
}

void CFindTextureDlg::setReplaceStr(const char * p)
{
  g_dlgFind.UpdateData(TRUE);
  if (g_dlgFind.m_bLive)
  {
    g_dlgFind.m_strReplace = p;
    g_dlgFind.UpdateData(FALSE);
  }
}


void CFindTextureDlg::OnCancel() 
{
	SaveToRegistry();
	CDialog::OnCancel();
	gbIsOpen = false;
}

BOOL CFindTextureDlg::DestroyWindow() 
{
	gbIsOpen = false;
	return CDialog::DestroyWindow();
}

void CFindTextureDlg::OnButtonBatchfindreplace() 
{
	UpdateData(DIALOG_TO_DATA);	// setup member vars properly

	char	sFileToLoad[MAX_PATH]={0};

	LONG	lSize = sizeof(sFileToLoad);
	if (!LoadRegistryInfo("Radiant::TextureFindReplaceBatchTextFile", &sFileToLoad, &lSize))
		strcpy(sFileToLoad,va("%s\\%s",Filename_PathOnly(currentmap),"texturenames.txt"));

	char *psTextFilename = InputLoadFileName(	Filename_WithoutPath(sFileToLoad), 
												"Name of text files for batch find/replace", 
												Filename_PathOnly(sFileToLoad), 
												"Text Files|*.txt;||");

	if (psTextFilename)
	{
		// save for next time...
		//
		strcpy(sFileToLoad,psTextFilename);
		SaveRegistryInfo("Radiant::TextureFindReplaceBatchTextFile", &sFileToLoad, sizeof(sFileToLoad));

		
		// now read this file in and build up a find/replace list to feed to the normal code...
		//
		FindReplaceList_t FindReplaceList;

		FILE *fhTextFile = fopen(sFileToLoad, "rt");	

		if (fhTextFile)
		{
			char sLine[16384];	// should be enough
			char *psLine;
			CString strLine;

			CString strFind;
			CString strReplace;						
		
			while ((psLine = fgets( sLine, sizeof(sLine), fhTextFile ))!=NULL)
			{	
				strLine = strlwr(sLine);
				strLine.Replace("\n","");
				strLine.TrimRight();
				strLine.TrimLeft();

				if (!strLine.IsEmpty())
				{
					int iLoc = strLine.FindOneOf(" \t");

					if (iLoc >=0)
					{
						strFind		= strLine.Left(iLoc);
						strReplace	= strLine.Mid(iLoc);
	
						strFind.TrimRight();
						strReplace.TrimLeft();
	
						FindReplaceList.insert(FindReplaceList.end(), FindReplaceStringPair_t(strFind, strReplace));
					}
					else
					{
						Sys_Printf(va("Syntax error in line \"%s\"! (need find and replace strings)\n",sFileToLoad));
					}
				}
			}

			// ( finished reading from the file )
			
			if (ferror(fhTextFile))
			{
				Sys_Printf(va("Error while reading file \"%s\"!\n",sFileToLoad));
			}
			else
			{
				int iReplacements = FindReplaceTextures(FindReplaceList, m_bSelectedOnly, m_bForce, m_bReScale, m_bSelectOnlyNoReplace);
				PrintReplacementCount(iReplacements, m_bSelectOnlyNoReplace);
			}

			fclose(fhTextFile);
		}
		else
		{
			// shouldn't happen...
			//			
			Sys_Printf(va("Error opening file \"%s\"!\n",sFileToLoad));
		}		
	}
}


void CFindTextureDlg::LoadFromRegistry() 
{		
	LONG 
	lSize = sizeof(this->m_bForce);
	if (!LoadRegistryInfo("Radiant::TextureFindWindow_Force", &m_bForce, &lSize))
		m_bForce = false;
	
	lSize = sizeof(m_bLive);
	if (!LoadRegistryInfo("Radiant::TextureFindWindow_Live", &m_bLive, &lSize))
		m_bLive = true;
	
	lSize = sizeof(m_bReScale);
	if (!LoadRegistryInfo("Radiant::TextureFindWindow_ReScale", &m_bReScale, &lSize))
		m_bReScale = true;

	lSize = sizeof(m_bSelectOnlyNoReplace);
	if (!LoadRegistryInfo("Radiant::TextureFindWindow_SelectOnlyNoReplace", &m_bSelectOnlyNoReplace, &lSize))
		m_bSelectOnlyNoReplace = false;
	
	UpdateData(DATA_TO_DIALOG);	
}

void CFindTextureDlg::SaveToRegistry(void)
{
	UpdateData(DIALOG_TO_DATA);

	CRect rct;
	GetWindowRect(rct);
	SaveRegistryInfo("Radiant::TextureFindWindow", &rct, sizeof(rct));

	SaveRegistryInfo("Radiant::TextureFindWindow_Force",	&m_bForce,		sizeof(m_bForce));
	SaveRegistryInfo("Radiant::TextureFindWindow_Live",		&m_bLive,		sizeof(m_bLive));
	SaveRegistryInfo("Radiant::TextureFindWindow_ReScale",	&m_bReScale,	sizeof(m_bReScale));	
	SaveRegistryInfo("Radiant::TextureFindWindow_SelectOnlyNoReplace",	&m_bSelectOnlyNoReplace,	sizeof(m_bSelectOnlyNoReplace));	
}


void CFindTextureDlg::OnSetfocusEditFind() 
{
  g_bFindActive = true;
}

void CFindTextureDlg::OnSetfocusEditReplace() 
{
  g_bFindActive = false;
}

void CFindTextureDlg::OnCheckSelectonly() 
{
	HandleItemGreying();	
}


void CFindTextureDlg::HandleItemGreying()
{
	UpdateData(DIALOG_TO_DATA);

	bool bOnOff = !m_bSelectOnlyNoReplace;

	GetDlgItem(IDC_CHECK_RESCALE)->EnableWindow(bOnOff);
	GetDlgItem(IDC_CHECK_SELECTED)->EnableWindow(bOnOff);
	GetDlgItem(IDC_CHECK_FORCE)->EnableWindow(bOnOff);
	GetDlgItem(IDC_CHECK_LIVE)->EnableWindow(bOnOff);
}
