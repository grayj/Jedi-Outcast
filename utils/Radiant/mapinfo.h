#if !defined(AFX_MAPINFO_H__C241B9A2_819F_11D1_B548_00AA00A410FC__INCLUDED_)
#define AFX_MAPINFO_H__C241B9A2_819F_11D1_B548_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MapInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapInfo dialog

class CMapInfo : public CDialog
{
// Construction
public:
	CMapInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMapInfo)
	enum { IDD = IDD_DLG_MAPINFO };
	CListBox	m_lstEntity;
	int		m_nNet;
	int		m_nTotalBrushes;
	int		m_nTotalEntities;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMapInfo)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPINFO_H__C241B9A2_819F_11D1_B548_00AA00A410FC__INCLUDED_)
