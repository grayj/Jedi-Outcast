// Radiant.h : main header file for the RADIANT application
//

#if !defined(AFX_RADIANT_H__330BBF06_731C_11D1_B539_00AA00A410FC__INCLUDED_)
#define AFX_RADIANT_H__330BBF06_731C_11D1_B539_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRadiantApp:
// See Radiant.cpp for the implementation of this class
//

class CRadiantApp : public CWinApp
{
public:
	CRadiantApp(LPCTSTR lpszAppName = NULL);     // app name defaults to EXE name

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRadiantApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRadiantApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RADIANT_H__330BBF06_731C_11D1_B539_00AA00A410FC__INCLUDED_)
