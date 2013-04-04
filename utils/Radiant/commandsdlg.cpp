// CommandsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Radiant.h"
#include "CommandsDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommandsDlg dialog


CCommandsDlg::CCommandsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommandsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommandsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCommandsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommandsDlg)
	DDX_Control(pDX, IDC_LIST_COMMANDS, m_lstCommands);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommandsDlg, CDialog)
	//{{AFX_MSG_MAP(CCommandsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommandsDlg message handlers

BOOL CCommandsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
  m_lstCommands.SetTabStops(96);
  int nCount = g_nCommandCount;
  for (int n = 0; n < nCount; n++)
  {
    CString strLine;
    char c = g_Commands[n].m_nKey;
    CString strKeys = c;
    for (int k = 0; k < g_nKeyCount; k++)
    {
      if (g_Keys[k].m_nVKKey == g_Commands[n].m_nKey)
      {
        strKeys = g_Keys[k].m_strName;
        break;
      }
    }
    if (g_Commands[n].m_nModifiers & RAD_SHIFT)
      strKeys += "+Shift";
    if (g_Commands[n].m_nModifiers & RAD_ALT)
      strKeys += "+Alt";
    if (g_Commands[n].m_nModifiers & RAD_CONTROL)
      strKeys += "+Control";
    strLine.Format("%s \t%s", g_Commands[n].m_strCommand, strKeys);
    m_lstCommands.AddString(strLine);
  }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
