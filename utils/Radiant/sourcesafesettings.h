#if !defined(AFX_SOURCESAFESETTINGS_H__DB2B9121_EAE1_11D3_8A5B_00500424438B__INCLUDED_)
#define AFX_SOURCESAFESETTINGS_H__DB2B9121_EAE1_11D3_8A5B_00500424438B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SourceSafeSettings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSourceSafeSettings dialog

class CSourceSafeSettings : public CDialog
{
// Construction
public:
	CSourceSafeSettings(CString* pcsINI, CString* pcsProject, BOOL *pbEnabled, CString* pcsBehavEd, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CSourceSafeSettings)
	enum { IDD = IDD_SOURCESAFE };
	BOOL	m_bUseSS;
	CString	m_sINIPath;
	CString	m_sProjectPath;
	CString	m_sBehavEdPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSourceSafeSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CString*	m_pcstrINIPath;
	CString*	m_pcstrProjectPath;
	CString*	m_pcstrBehavEdPath;
	BOOL*		m_pboolUseSS;

	// Generated message map functions
	//{{AFX_MSG(CSourceSafeSettings)
	afx_msg void OnButtonDefaultall();
	virtual void OnOK();
	afx_msg void OnButtonDefaultall2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOURCESAFESETTINGS_H__DB2B9121_EAE1_11D3_8A5B_00500424438B__INCLUDED_)
