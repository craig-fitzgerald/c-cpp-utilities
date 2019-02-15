// cdmenu.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "cdmenu.h"
#include "Data.h"
#include "Dib.h"
#include "cdmenuDlg.h"
#include "MyCommandLineInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCdmenuApp

BEGIN_MESSAGE_MAP(CCdmenuApp, CWinApp)
	//{{AFX_MSG_MAP(CCdmenuApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCdmenuApp construction

CCdmenuApp::CCdmenuApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCdmenuApp object

CCdmenuApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCdmenuApp initialization

BOOL CCdmenuApp::InitInstance()
	{
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	char szPath[_MAX_PATH];

	GetModuleFileName(NULL, szPath, _MAX_PATH);
	CUtil::SetFileName (szPath, 0);

	CMyCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	CUtil::SetFileName (cmdInfo.m_strCfgFile, 1);

	CCdmenuDlg dlg;
	m_pMainWnd = &dlg;

	int nResponse = dlg.DoModal(); // IDOK or IDCANCEL
	return FALSE;
	}
