#if !defined(AFX_SCALEDIALOG_H__8A9B33B2_9922_11D1_B568_00AA00A410FC__INCLUDED_)
#define AFX_SCALEDIALOG_H__8A9B33B2_9922_11D1_B568_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ScaleDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScaleDialog dialog

class CScaleDialog : public CDialog
{
// Construction
public:
	CScaleDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScaleDialog)
	enum { IDD = IDD_DIALOG_SCALE };
	float	m_fZ;
	float	m_fX;
	float	m_fY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScaleDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScaleDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCALEDIALOG_H__8A9B33B2_9922_11D1_B568_00AA00A410FC__INCLUDED_)
