#if !defined(AFX_ROTATEDLG_H__D4B79152_7A7E_11D1_B541_00AA00A410FC__INCLUDED_)
#define AFX_ROTATEDLG_H__D4B79152_7A7E_11D1_B541_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RotateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRotateDlg dialog

class CRotateDlg : public CDialog
{
// Construction
public:
	CRotateDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRotateDlg)
	enum { IDD = IDD_ROTATE };
	CSpinButtonCtrl	m_wndSpin3;
	CSpinButtonCtrl	m_wndSpin2;
	CSpinButtonCtrl	m_wndSpin1;
	CString	m_strX;
	CString	m_strY;
	CString	m_strZ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRotateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ApplyNoPaint();

	// Generated message map functions
	//{{AFX_MSG(CRotateDlg)
	virtual void OnOK();
	afx_msg void OnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROTATEDLG_H__D4B79152_7A7E_11D1_B541_00AA00A410FC__INCLUDED_)
