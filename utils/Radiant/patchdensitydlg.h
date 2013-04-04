#if !defined(AFX_PATCHDENSITYDLG_H__509162A1_1023_11D2_AFFB_00AA00A410FC__INCLUDED_)
#define AFX_PATCHDENSITYDLG_H__509162A1_1023_11D2_AFFB_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PatchDensityDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPatchDensityDlg dialog

class CPatchDensityDlg : public CDialog
{
// Construction
public:
	CPatchDensityDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPatchDensityDlg)
	enum { IDD = IDD_DIALOG_NEWPATCH };
	CComboBox	m_wndWidth;
	CComboBox	m_wndHeight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatchDensityDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPatchDensityDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATCHDENSITYDLG_H__509162A1_1023_11D2_AFFB_00AA00A410FC__INCLUDED_)
