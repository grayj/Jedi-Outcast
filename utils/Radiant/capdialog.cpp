// CapDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Radiant.h"
#include "CapDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCapDialog dialog


CCapDialog::CCapDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCapDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCapDialog)
	m_nCap = 0;
	//}}AFX_DATA_INIT
}


void CCapDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCapDialog)
	DDX_Radio(pDX, IDC_RADIO_CAP, m_nCap);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCapDialog, CDialog)
	//{{AFX_MSG_MAP(CCapDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCapDialog message handlers
