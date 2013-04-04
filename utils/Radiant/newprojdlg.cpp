// NewProjDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Radiant.h"
#include "NewProjDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewProjDlg dialog


CNewProjDlg::CNewProjDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewProjDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewProjDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CNewProjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewProjDlg)
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewProjDlg, CDialog)
	//{{AFX_MSG_MAP(CNewProjDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewProjDlg message handlers
