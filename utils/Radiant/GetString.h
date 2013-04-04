#if !defined(AFX_GETSTRING_H__062547B2_2CFC_4DA8_A379_65F83606D96A__INCLUDED_)
#define AFX_GETSTRING_H__062547B2_2CFC_4DA8_A379_65F83606D96A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetString.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGetString dialog

class CGetString : public CDialog
{
// Construction
public:
	CGetString(LPCSTR pPrompt, CString *pFeedback, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGetString)
	enum { IDD = IDD_DIALOG_GETSTRING };
	CString	m_strEditBox;
	//}}AFX_DATA
	CString *m_pFeedback;
	LPCSTR	m_pPrompt;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetString)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGetString)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETSTRING_H__062547B2_2CFC_4DA8_A379_65F83606D96A__INCLUDED_)
