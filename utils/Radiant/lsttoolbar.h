#if !defined(AFX_LSTTOOLBAR_H__279AAE23_78C5_11D1_B53C_00AA00A410FC__INCLUDED_)
#define AFX_LSTTOOLBAR_H__279AAE23_78C5_11D1_B53C_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LstToolBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLstToolBar window

class CLstToolBar : public CToolBar
{
// Construction
public:
	CLstToolBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLstToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLstToolBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLstToolBar)
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LSTTOOLBAR_H__279AAE23_78C5_11D1_B53C_00AA00A410FC__INCLUDED_)
