// EditWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Radiant.h"
#include "EditWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditWnd
IMPLEMENT_DYNCREATE(CEditWnd, CWnd);

CEditWnd::CEditWnd()
{
}

CEditWnd::~CEditWnd()
{
}


BEGIN_MESSAGE_MAP(CEditWnd, CEdit)
	//{{AFX_MSG_MAP(CEditWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditWnd message handlers

BOOL CEditWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
  cs.style = WS_CHILD | WS_VSCROLL | ES_AUTOHSCROLL | ES_MULTILINE | WS_VISIBLE;
  cs.lpszClass = "EDIT";
	return CEdit::PreCreateWindow(cs);
}
