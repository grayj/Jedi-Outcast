#if !defined(AFX_RADEDITWND_H__DC829124_812D_11D1_B548_00AA00A410FC__INCLUDED_)
#define AFX_RADEDITWND_H__DC829124_812D_11D1_B548_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RADEditWnd.h : header file
//

#include "EditWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CRADEditWnd window

class CRADEditWnd : public CWnd
{
// Construction
public:
	CRADEditWnd();

// Attributes
public:
  CEdit* GetEditWnd() {return dynamic_cast<CEdit*>(&m_wndEdit); };
protected:
  CEditWnd m_wndEdit;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRADEditWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRADEditWnd();


	// Generated message map functions
protected:
	//{{AFX_MSG(CRADEditWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RADEDITWND_H__DC829124_812D_11D1_B548_00AA00A410FC__INCLUDED_)
