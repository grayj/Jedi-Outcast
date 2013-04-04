#if !defined(AFX_COMMANDSDLG_H__C80F6E42_8531_11D1_B548_00AA00A410FC__INCLUDED_)
#define AFX_COMMANDSDLG_H__C80F6E42_8531_11D1_B548_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CommandsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommandsDlg dialog

class CCommandsDlg : public CDialog
{
// Construction
public:
	CCommandsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCommandsDlg)
	enum { IDD = IDD_DLG_COMMANDLIST };
	CListBox	m_lstCommands;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommandsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommandsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMANDSDLG_H__C80F6E42_8531_11D1_B548_00AA00A410FC__INCLUDED_)
