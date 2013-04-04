#if !defined(AFX_ENTITYLISTDLG_H__C241B9A3_819F_11D1_B548_00AA00A410FC__INCLUDED_)
#define AFX_ENTITYLISTDLG_H__C241B9A3_819F_11D1_B548_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EntityListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEntityListDlg dialog

class CEntityListDlg : public CDialog
{
// Construction
public:
	CEntityListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEntityListDlg)
	enum { IDD = IDD_DLG_ENTITYLIST };
	CListCtrl	m_lstEntity;
	CTreeCtrl	m_treeEntity;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEntityListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEntityListDlg)
	afx_msg void OnSelect();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTreeEntity(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListInfo();
	afx_msg void OnDblclkTreeEntity(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYLISTDLG_H__C241B9A3_819F_11D1_B548_00AA00A410FC__INCLUDED_)
