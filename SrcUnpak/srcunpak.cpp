// srcunpak.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "srcunpak.h"
#include "srcunpakDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSrcunpakApp

BEGIN_MESSAGE_MAP(CSrcunpakApp, CWinApp)
	//{{AFX_MSG_MAP(CSrcunpakApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSrcunpakApp construction

CSrcunpakApp::CSrcunpakApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSrcunpakApp object

CSrcunpakApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSrcunpakApp initialization

BOOL CSrcunpakApp::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CSrcunpakDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
