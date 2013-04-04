// BSInput.cpp : implementation file
//

#include "stdafx.h"
#include "Radiant.h"
#include "BSInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBSInput dialog


CBSInput::CBSInput(CWnd* pParent /*=NULL*/)
	: CDialog(CBSInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBSInput)
	m_fField1 = 0.0f;
	m_fField2 = 0.0f;
	m_fField3 = 0.0f;
	m_fField4 = 0.0f;
	m_fField5 = 0.0f;
	m_strField1 = _T("");
	m_strField2 = _T("");
	m_strField3 = _T("");
	m_strField4 = _T("");
	m_strField5 = _T("");
	//}}AFX_DATA_INIT
}


void CBSInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBSInput)
	DDX_Text(pDX, IDC_EDIT_FIELD1, m_fField1);
	DDX_Text(pDX, IDC_EDIT_FIELD2, m_fField2);
	DDX_Text(pDX, IDC_EDIT_FIELD3, m_fField3);
	DDX_Text(pDX, IDC_EDIT_FIELD4, m_fField4);
	DDX_Text(pDX, IDC_EDIT_FIELD5, m_fField5);
	DDX_Text(pDX, IDC_STATIC_FIELD1, m_strField1);
	DDX_Text(pDX, IDC_STATIC_FIELD2, m_strField2);
	DDX_Text(pDX, IDC_STATIC_FIELD3, m_strField3);
	DDX_Text(pDX, IDC_STATIC_FIELD4, m_strField4);
	DDX_Text(pDX, IDC_STATIC_FIELD5, m_strField5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBSInput, CDialog)
	//{{AFX_MSG_MAP(CBSInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBSInput message handlers

BOOL CBSInput::OnInitDialog() 
{
	CDialog::OnInitDialog();

  if (m_strField1.GetLength() == 0)
  {
    GetDlgItem(IDC_EDIT_FIELD1)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_STATIC_FIELD1)->ShowWindow(SW_HIDE);
  }
  if (m_strField2.GetLength() == 0)
  {
    GetDlgItem(IDC_EDIT_FIELD2)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_STATIC_FIELD2)->ShowWindow(SW_HIDE);
  }
  if (m_strField3.GetLength() == 0)
  {
    GetDlgItem(IDC_EDIT_FIELD3)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_STATIC_FIELD3)->ShowWindow(SW_HIDE);
  }
  if (m_strField4.GetLength() == 0)
  {
    GetDlgItem(IDC_EDIT_FIELD4)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_STATIC_FIELD4)->ShowWindow(SW_HIDE);
  }
  if (m_strField5.GetLength() == 0)
  {
    GetDlgItem(IDC_EDIT_FIELD5)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_STATIC_FIELD5)->ShowWindow(SW_HIDE);
  }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
