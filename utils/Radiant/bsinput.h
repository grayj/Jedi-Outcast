#if !defined(AFX_BSINPUT_H__81DF2A32_A552_11D1_B58E_00AA00A410FC__INCLUDED_)
#define AFX_BSINPUT_H__81DF2A32_A552_11D1_B58E_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BSInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBSInput dialog

class CBSInput : public CDialog
{
// Construction
public:
	CBSInput(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBSInput)
	enum { IDD = IDD_DIALOG_INPUT };
	float	m_fField1;
	float	m_fField2;
	float	m_fField3;
	float	m_fField4;
	float	m_fField5;
	CString	m_strField1;
	CString	m_strField2;
	CString	m_strField3;
	CString	m_strField4;
	CString	m_strField5;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBSInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBSInput)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BSINPUT_H__81DF2A32_A552_11D1_B58E_00AA00A410FC__INCLUDED_)
