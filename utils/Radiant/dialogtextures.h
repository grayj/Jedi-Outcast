#if !defined(AFX_DIALOGTEXTURES_H__F3F3F984_E47E_11D1_B61B_00AA00A410FC__INCLUDED_)
#define AFX_DIALOGTEXTURES_H__F3F3F984_E47E_11D1_B61B_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DialogTextures.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogTextures dialog

class CDialogTextures : public CDialog
{
// Construction
public:
	CDialogTextures(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogTextures)
	enum { IDD = IDD_DIALOG_TEXTURELIST };
	CListBox	m_wndList;
	//}}AFX_DATA
  int m_nSelection;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTextures)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogTextures)
	virtual void OnOK();
	afx_msg void OnDblclkListTextures();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTEXTURES_H__F3F3F984_E47E_11D1_B61B_00AA00A410FC__INCLUDED_)
