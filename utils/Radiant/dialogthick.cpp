// DialogThick.cpp : implementation file
//

#include "stdafx.h"
#include "Radiant.h"
#include "DialogThick.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogThick dialog


CDialogThick::CDialogThick(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogThick::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogThick)
	m_bSeams = TRUE;
	m_nAmount = 8;
	//}}AFX_DATA_INIT
}


void CDialogThick::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogThick)
	DDX_Check(pDX, IDC_CHECK_SEAMS, m_bSeams);
	DDX_Text(pDX, IDC_EDIT_AMOUNT, m_nAmount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogThick, CDialog)
	//{{AFX_MSG_MAP(CDialogThick)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogThick message handlers
