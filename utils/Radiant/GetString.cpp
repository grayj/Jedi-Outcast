// GetString.cpp : implementation file
//

#include "stdafx.h"
#include "radiant.h"
#include "GetString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetString dialog


CGetString::CGetString(LPCSTR pPrompt, CString *pFeedback, CWnd* pParent /*=NULL*/)
	: CDialog(CGetString::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetString)
	m_strEditBox = _T("");
	//}}AFX_DATA_INIT

	m_pFeedback = pFeedback;
	m_pPrompt	= pPrompt;
}


void CGetString::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetString)
	DDX_Text(pDX, IDC_EDIT1, m_strEditBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGetString, CDialog)
	//{{AFX_MSG_MAP(CGetString)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetString message handlers

void CGetString::OnOK() 
{
	UpdateData(DIALOG_TO_DATA);

	*m_pFeedback = m_strEditBox;
	
	CDialog::OnOK();
}

BOOL CGetString::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here


	GetDlgItem(IDC_PROMPT)->SetWindowText(m_pPrompt);
/*	CWnd* theWnd = GetDlgItem(IDC_TREKPROPBUTTON_TOGGLEVIEW);
	if (m_button.m_DrawType == gldt_STRETCHFIT)
	{
		theWnd->SetWindowText("Stretched");		
	}
*/
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
