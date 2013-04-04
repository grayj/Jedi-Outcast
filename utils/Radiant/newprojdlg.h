#if !defined(AFX_NEWPROJDLG_H__1E2527A2_8447_11D1_B548_00AA00A410FC__INCLUDED_)
#define AFX_NEWPROJDLG_H__1E2527A2_8447_11D1_B548_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// NewProjDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewProjDlg dialog

class CNewProjDlg : public CDialog
{
// Construction
public:
	CNewProjDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewProjDlg)
	enum { IDD = IDD_DLG_NEWPROJECT };
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewProjDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewProjDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPROJDLG_H__1E2527A2_8447_11D1_B548_00AA00A410FC__INCLUDED_)
