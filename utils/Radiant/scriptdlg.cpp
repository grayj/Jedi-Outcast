// ScriptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Radiant.h"
#include "ScriptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptDlg dialog


CScriptDlg::CScriptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScriptDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScriptDlg)
	m_strScript = _T("");
	//}}AFX_DATA_INIT
}


void CScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScriptDlg)
	DDX_Control(pDX, IDC_LIST_SCRIPTS, m_lstScripts);
	DDX_LBString(pDX, IDC_LIST_SCRIPTS, m_strScript);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScriptDlg, CDialog)
	//{{AFX_MSG_MAP(CScriptDlg)
	ON_BN_CLICKED(ID_RUN, OnRun)
	ON_LBN_DBLCLK(IDC_LIST_SCRIPTS, OnDblclkListScripts)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptDlg message handlers

void CScriptDlg::OnRun() 
{
  UpdateData(TRUE);
  EndDialog(IDOK);
  RunScriptByName(m_strScript.GetBuffer(0), true);
}

BOOL CScriptDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

  char* pBuff = new char[16384];
  CString strINI = g_strAppPath;
  strINI += "\\scripts.ini";
  int n = GetPrivateProfileSectionNames(pBuff, 16384, strINI);

  // CStringList list;
  m_lstScripts.ResetContent();
  char* pWorkBuff = pBuff;
  while (*pWorkBuff != NULL)
  {
    m_lstScripts.AddString(pWorkBuff);
    pWorkBuff += strlen(pWorkBuff) + 1;
  }
  delete []pBuff;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CScriptDlg::OnDblclkListScripts() 
{
  UpdateData(TRUE);
  EndDialog(IDOK);
  RunScriptByName(m_strScript.GetBuffer(0), true);
}
