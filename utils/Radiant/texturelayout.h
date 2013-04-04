#if !defined(AFX_TEXTURELAYOUT_H__5EA4C722_173F_11D2_B024_00AA00A410FC__INCLUDED_)
#define AFX_TEXTURELAYOUT_H__5EA4C722_173F_11D2_B024_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TextureLayout.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextureLayout dialog

class CTextureLayout : public CDialog
{
// Construction
public:
	CTextureLayout(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTextureLayout)
	enum { IDD = IDD_DIALOG_TEXTURELAYOUT };
	float	m_fX;
	float	m_fY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextureLayout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextureLayout)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTURELAYOUT_H__5EA4C722_173F_11D2_B024_00AA00A410FC__INCLUDED_)
