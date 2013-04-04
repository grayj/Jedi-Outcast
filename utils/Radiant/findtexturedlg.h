#if !defined(AFX_FINDTEXTUREDLG_H__34B75D32_9F3A_11D1_B570_00AA00A410FC__INCLUDED_)
#define AFX_FINDTEXTUREDLG_H__34B75D32_9F3A_11D1_B570_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FindTextureDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFindTextureDlg dialog

class CFindTextureDlg : public CDialog
{
// Construction
public:
	void SaveToRegistry(void);
	void LoadFromRegistry(void);

	static void setReplaceStr(const char* p);
	static void setFindStr(const char* p);
	static bool isOpen();
  static void show();
  static void updateTextures(const char* p);
	CFindTextureDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFindTextureDlg)
	enum { IDD = IDD_DIALOG_FINDREPLACE };
	BOOL	m_bSelectedOnly;
	CString	m_strFind;
	CString	m_strReplace;
	BOOL	m_bForce;
	BOOL	m_bLive;
	BOOL	m_bReScale;
	BOOL	m_bSelectOnlyNoReplace;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindTextureDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFindTextureDlg)
	afx_msg void OnBtnApply();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonBatchfindreplace();
	afx_msg void OnSetfocusEditFind();
	afx_msg void OnSetfocusEditReplace();
	afx_msg void OnCheckSelectonly();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void HandleItemGreying();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDTEXTUREDLG_H__34B75D32_9F3A_11D1_B570_00AA00A410FC__INCLUDED_)
