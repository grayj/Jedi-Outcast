// SourceSafeSettings.cpp : implementation file
//

#include "stdafx.h"
#include "radiant.h"
#include "SourceSafeSettings.h"
#include "SourceSafe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSourceSafeSettings dialog

CSourceSafeSettings::CSourceSafeSettings(CString* pcsINI, CString* pcsProject, BOOL *pbEnabled, CString* pcsBehavEd, CWnd* pParent /*=NULL*/)
	: CDialog(CSourceSafeSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSourceSafeSettings)
	m_bUseSS = FALSE;
	m_sINIPath = _T("");
	m_sProjectPath = _T("");
	m_sBehavEdPath = _T("");
	//}}AFX_DATA_INIT

	m_bUseSS		= *pbEnabled;
	m_sINIPath		= *pcsINI;
	m_sProjectPath	= *pcsProject;
	m_sBehavEdPath	= *pcsBehavEd;

	m_pcstrINIPath		= pcsINI;
	m_pcstrProjectPath	= pcsProject;
	m_pcstrBehavEdPath	= pcsBehavEd;
	m_pboolUseSS		= pbEnabled;
}


void CSourceSafeSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSourceSafeSettings)
	DDX_Check(pDX, IDC_CHECK1, m_bUseSS);
	DDX_Text(pDX, IDC_EDIT_SS_INIPATH, m_sINIPath);
	DDX_Text(pDX, IDC_EDIT_SS_PROJECTPATH, m_sProjectPath);
	DDX_Text(pDX, IDC_EDIT_SS_BEHAVEDPATH, m_sBehavEdPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSourceSafeSettings, CDialog)
	//{{AFX_MSG_MAP(CSourceSafeSettings)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULTALL, OnButtonDefaultall)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULTALL2, OnButtonDefaultall2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSourceSafeSettings message handlers

void CSourceSafeSettings::OnButtonDefaultall() 
{
	m_bUseSS		= true;
	m_sINIPath		= sEF1_SS_INI;
	m_sProjectPath	= sEF1_SS_PROJECT;
	m_sBehavEdPath	= sEF1_BEHAVED_PATH;

	UpdateData(DATA_TO_DIALOG);	
}

void CSourceSafeSettings::OnButtonDefaultall2() 
{
	m_bUseSS		= true;
	m_sINIPath		= sCHC_SS_INI;
	m_sProjectPath	= sCHC_SS_PROJECT;
	m_sBehavEdPath	= sCHC_BEHAVED_PATH;	

	UpdateData(DATA_TO_DIALOG);	
}


void CSourceSafeSettings::OnOK() 
{
	UpdateData(DIALOG_TO_DATA);

	*m_pcstrINIPath		= m_sINIPath;
	*m_pcstrProjectPath	= m_sProjectPath;
	*m_pcstrBehavEdPath	= m_sBehavEdPath;
	*m_pboolUseSS		= m_bUseSS;

	SS_Shutdown_OnceOnly();

	CDialog::OnOK();
}


