#if !defined(AFX_TEXEDIT_H__913F6812_99CD_11D1_B568_00AA00A410FC__INCLUDED_)
#define AFX_TEXEDIT_H__913F6812_99CD_11D1_B568_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TexEdit.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CTexEdit window

class CTexWnd;

class CTexEdit : public CEdit
{
// Construction
public:
	CTexEdit();

// Attributes
public:

protected:
  CTexWnd* m_pTexWnd;
  CFont m_Font;
// Operations
public:
  void SetTexWnd(CTexWnd* pTex) {m_pTexWnd = pTex;};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTexEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTexEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTexEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnChange();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXEDIT_H__913F6812_99CD_11D1_B568_00AA00A410FC__INCLUDED_)
