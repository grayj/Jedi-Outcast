#if !defined(AFX_SCRIPTDLG_H__C241B9A4_819F_11D1_B548_00AA00A410FC__INCLUDED_)
#define AFX_SCRIPTDLG_H__C241B9A4_819F_11D1_B548_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ScriptDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScriptDlg dialog

class CScriptDlg : public CDialog
{
// Construction
public:
	CScriptDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScriptDlg)
	enum { IDD = IDD_DLG_SCRIPTS };
	CListBox	m_lstScripts;
	CString	m_strScript;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScriptDlg)
	afx_msg void OnRun();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListScripts();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTDLG_H__C241B9A4_819F_11D1_B548_00AA00A410FC__INCLUDED_)
