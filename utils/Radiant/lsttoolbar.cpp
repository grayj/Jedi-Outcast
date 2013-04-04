// LstToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "Radiant.h"
#include "LstToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLstToolBar

CLstToolBar::CLstToolBar()
{
}

CLstToolBar::~CLstToolBar()
{
}


BEGIN_MESSAGE_MAP(CLstToolBar, CToolBar)
	//{{AFX_MSG_MAP(CLstToolBar)
	ON_WM_PARENTNOTIFY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLstToolBar message handlers

void CLstToolBar::OnParentNotify(UINT message, LPARAM lParam) 
{
	CToolBar::OnParentNotify(message, lParam);
}
