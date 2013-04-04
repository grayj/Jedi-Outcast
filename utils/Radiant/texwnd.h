#if !defined(AFX_TEXWND_H__44B4BA05_781B_11D1_B53C_00AA00A410FC__INCLUDED_)
#define AFX_TEXWND_H__44B4BA05_781B_11D1_B53C_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TexWnd.h : header file
//
#include "texedit.h"

/////////////////////////////////////////////////////////////////////////////
// CTexWnd window

class CTexWnd : public CWnd
{
  DECLARE_DYNCREATE(CTexWnd);
// Construction
public:
	CTexWnd();
  void UpdateFilter(const char* pFilter);
  void UpdatePrefs();
  void FocusEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTexWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTexWnd();

protected:
  CTexEdit m_wndFilter;
  CButton  m_wndShaders;
  bool m_bNeedRange;
	// Generated message map functions
protected:
	//{{AFX_MSG(CTexWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTexturesFlush();
	//}}AFX_MSG
	afx_msg void OnShaderClick();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXWND_H__44B4BA05_781B_11D1_B53C_00AA00A410FC__INCLUDED_)
