// RADEditWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Radiant.h"
#include "RADEditWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRADEditWnd

CRADEditWnd::CRADEditWnd()
{
}

CRADEditWnd::~CRADEditWnd()
{
}


BEGIN_MESSAGE_MAP(CRADEditWnd, CWnd)
	//{{AFX_MSG_MAP(CRADEditWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRADEditWnd message handlers

int CRADEditWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

  CRect rctClient;
  GetClientRect(rctClient);
  m_wndEdit.Create(WS_CHILD | WS_VSCROLL | ES_AUTOHSCROLL | ES_MULTILINE | WS_VISIBLE, rctClient, this, 101);
	return 0;
}

void CRADEditWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
  CRect rctClient;
  GetClientRect(rctClient);
  m_wndEdit.SetWindowPos(NULL, rctClient.left, rctClient.top, rctClient.Width(), rctClient.Height(), SWP_SHOWWINDOW);
}
