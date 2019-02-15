// crypt.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include <limits.h>
#include <conio.h>

#include "crypt.h"
#include "cryptDlg.h"
#include "cryptError.h"
#include "gnudir.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// --------------------------- CONSTANTS ----------------------------

#define STR_FLAG_UNCRYPT		"d"
#define STR_FULL_FLAG_UNCRYPT	"decrypt"
#define STR_FLAG_PASSWORD		"p"
#define STR_FULL_FLAG_PASSWORD	"password"
#define STR_FLAG_HELP			"h"
#define STR_FLAG_HELP2			"?"
#define STR_FULL_FLAG_HELP		"help"
#define STR_FLAG_REGISTER		"register"
#define STR_PROG_REG_NAME		"DesCrypt"
#define STR_PROG_TITLE			"InfoTech DES Crypt"
#define STR_LINK_NAME			"ITI Encryption Tool"
#define STR_PROFILE_PATH		"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"
#define STR_ERR_PASSWD_MISSING	"Password is missing, exiting.\n"
#define STR_MSG_INVALID_FILENAME	"'%s' is not a valid file name"
#define STR_MSG_NO_FILES		"No files to crypt, goodbye ..."

#define ERR_PASSWD_MISSING		1


// ------------------------- HELPER FUNCTIONS -----------------------

// Creates a shell link. Taken from "Fusing Your Applications to the 
// System Through the Windows95 Shell" by Jeff Richter (in MSDN).

#define LD_USEDESC     0x00000001
#define LD_USEARGS     0x00000002
#define LD_USEICON     0x00000004
#define LD_USEWORKDIR  0x00000008
#define LD_USEHOTKEY   0x00000010
#define LD_USESHOWCMD  0x00000020

typedef struct 
{
	// Mandatory members
	LPTSTR pszPathname;		// Pathname of original object
	DWORD fdwFlags;			// LD_* flags ORed together for optional members
	// Optional members
	LPTSTR pszDesc;			// Description of link file (its filename)
	LPTSTR pszArgs;			// command-line arguments
	LPTSTR pszIconPath;		// Pathname of file containing the icon
	int  nIconIndex;		// Index of icon in pszIconPath
	LPTSTR pszWorkingDir;	// Working directory when process starts
	int  nShowCmd;			// How to show the initial window
	WORD  wHotkey;			// Hot key for the link
} LINKDATA, *PLINKDATA;


// --- CreateShellLink() --------------------------------------------
// Creates a link.
//  
HRESULT WINAPI CreateShellLink(LPTSTR pszLinkFilePathname, PLINKDATA pld)
{ 
	HRESULT hres;  
	IShellLink* psl;
	IPersistFile* ppf;
	BOOL bDeallocatePathName = FALSE;
	TCHAR szPathName[MAX_PATH];
	PTCHAR pszPathName = NULL;

	// If the link file path doesn't yet have the .lnk suffix then add it ---
	if (strcmp(pszLinkFilePathname + strlen(pszLinkFilePathname) - 4, ".lnk"))
	{
		sprintf(szPathName, "%s.lnk", pszLinkFilePathname);
		pszPathName = szPathName;
	}
	else
	{
		pszPathName = pszLinkFilePathname;
	}

	hres = CoInitialize(NULL);

	// Create a shell link object
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (PVOID *) &psl);
	if (SUCCEEDED(hres))
	{
		// Initialize the shell link object
		psl->SetPath(pld->pszPathname);
		if (pld->fdwFlags & LD_USEARGS)
			psl->SetArguments(pld->pszArgs);
		if (pld->fdwFlags & LD_USEDESC)
			psl->SetDescription(pld->pszDesc);
		if (pld->fdwFlags & LD_USEICON)
			psl->SetIconLocation(pld->pszIconPath, pld->nIconIndex);
		if (pld->fdwFlags & LD_USEWORKDIR)
			psl->SetWorkingDirectory(pld->pszWorkingDir);
		if (pld->fdwFlags & LD_USESHOWCMD)
			psl->SetShowCmd(pld->nShowCmd);
		if (pld->fdwFlags & LD_USEHOTKEY)
			psl->SetHotkey(pld->wHotkey);

		// Save the shell link object on the disk
		hres = psl->QueryInterface(IID_IPersistFile, (PVOID *) &ppf);

		if (SUCCEEDED(hres))
		{
#ifndef UNICODE
			// Convert the ANSI path to a Unicode path
			WCHAR szPath[_MAX_PATH] = { 0 };
			MultiByteToWideChar(CP_ACP, 0, pszPathName, strlen(pszPathName), szPath, sizeof(szPath));
			hres = ppf->Save(szPath, TRUE);
#else
			hres = ppf->Save(pszPathName, TRUE);
#endif
			ppf->Release();
		}
		psl->Release();
	}
	CoUninitialize();
	return(hres);
}




// ------------------------- CCryptCmdLineInfo ----------------------



// --- ParseParam() -------------------------------------------------
void CCryptCmdLineInfo::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	static BOOL bPassword = FALSE;	// next token should be password

	if (bFlag)
	{
		if (bPassword && m_strPassword.IsEmpty())
		{
			// Error: password expected ---
			exit(ERR_PASSWD_MISSING);
		}
		if (!_stricmp(lpszParam, STR_FLAG_UNCRYPT) || !_stricmp(lpszParam, STR_FULL_FLAG_UNCRYPT))
			m_bEncrypt = FALSE;
		else if (!_stricmp(lpszParam, STR_FLAG_HELP) || !_stricmp(lpszParam, STR_FLAG_HELP2) || !_stricmp(lpszParam, STR_FULL_FLAG_HELP))
			m_bHelpWanted = TRUE;
		else if (!_stricmp(lpszParam, STR_FLAG_PASSWORD) || !_stricmp(lpszParam, STR_FULL_FLAG_PASSWORD))
			bPassword = TRUE;
		else if (!_stricmp(lpszParam, STR_FLAG_REGISTER))
			m_bRegister = TRUE;
	}
	else
	{
		if (bPassword)
		{
			// Password expected ---
			m_strPassword = lpszParam;
			bPassword = FALSE;
		}
		else
			// this is the name of the file to open ---
			m_fileList.AddTail(lpszParam);
	}
}



/////////////////////////////////////////////////////////////////////////////
// CCryptApp

BEGIN_MESSAGE_MAP(CCryptApp, CWinApp)
	//{{AFX_MSG_MAP(CCryptApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCryptApp construction

CCryptApp::CCryptApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCryptApp object

CCryptApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCryptApp initialization

BOOL CCryptApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Get path of current.exe
	TCHAR szExePath[MAX_PATH];
	GetModuleFileName(NULL, szExePath, MAX_PATH);	// get directory of exe file

	// Register extension .des ---
	RegisterExtension(szExePath);
	CreateSendToShortcut(szExePath);
	
	TCryptFileList fileList;
	CCryptCmdLineInfo cmdInfo(fileList);
	ParseCommandLine (cmdInfo);

	// If all is wanted is to register the application, then quit now ---
	if (cmdInfo.WantRegister())
		return FALSE;

	if (cmdInfo.IsHelpWanted())
	{
		DisplayHelp();
		return FALSE;
	}

	CCryptError cryptError;
	if (!cryptError.Init(!cmdInfo.GetPassword().IsEmpty()))
		return FALSE;

	// Convert file names to long file names ---
	CString strShortFileName;
	CString strLongFileName;
	LPTSTR lpstrShortFileName, lpstrLongFileName;
	POSITION pos = fileList.GetHeadPosition();
	while (pos)
	{
		strShortFileName = fileList.GetNext(pos);

		lpstrShortFileName = strShortFileName.GetBuffer(strShortFileName.GetLength()+1);
		lpstrLongFileName = strLongFileName.GetBuffer(_MAX_PATH);

		BOOL bStatus = DirGetLongFileName(lpstrLongFileName, lpstrShortFileName);

		strShortFileName.ReleaseBuffer();
		strLongFileName.ReleaseBuffer();

		if (!bStatus)
		{
			CString strError;
			strError.Format(STR_MSG_INVALID_FILENAME, (LPCTSTR) strShortFileName);
			cryptError.Error(strError);
		}
		else
		{
			m_fileList.AddTail(strLongFileName);
		}
	}

	if (m_fileList.IsEmpty())
	{
		cryptError.Error(STR_MSG_NO_FILES);
		return FALSE;
	}

	CCryptDlg dlg(m_fileList, cmdInfo.IsFlagEncrypt(), cryptError);
	if (cmdInfo.GetPassword().IsEmpty())
	{
		// no password: display dialog box ---
		m_pMainWnd = &dlg;

		int nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with OK
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with Cancel
		}
	}
	else
	{
		// Password has been given: don't display dialog box ---
		dlg.SetPassword(cmdInfo.GetPassword());
		dlg.Crypt();
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


// --- RegisterExtension() ------------------------------------------
// Registers the application so that .des is associated with crypt
//
void CCryptApp::RegisterExtension(LPCTSTR szExePath)
{
	HKEY hKey = 0, hSubKey = 0;

	// Register extension ---
	RegCreateKeyEx(HKEY_CLASSES_ROOT, ".des", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	RegSetValueEx(hKey, NULL, 0, REG_SZ, (BYTE*) STR_PROG_REG_NAME, sizeof(STR_PROG_REG_NAME));
//	RegSetValueEx (hKey, "Content Type", 0, REG_SZ, (PUCHAR)REG_DOCCT, strlen (REG_DOCCT)+1);
	RegCloseKey(hKey);

	// Register program ---
	RegCreateKeyEx (HKEY_CLASSES_ROOT, STR_PROG_REG_NAME, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	RegSetValueEx(hKey, NULL, 0, REG_SZ, (BYTE*) STR_PROG_TITLE, sizeof(STR_PROG_TITLE));

	// Register program icon ---
	CString strIconPath;
	strIconPath.Format("%s,0", szExePath);

	RegCreateKeyEx(hKey, "DefaultIcon", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hSubKey, NULL);
	RegSetValueEx(hSubKey, NULL, 0, REG_SZ, (BYTE*) (LPCTSTR) strIconPath, strIconPath.GetLength()+1);
	RegCloseKey(hSubKey);

	// Register program shell command ---
	CString strShellCmd;
	strShellCmd.Format("\"%s\" /d \"%%1\"", szExePath);
	HKEY hOpenKey = 0, hCmdKey = 0;
	RegCreateKeyEx(hKey, "shell", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hSubKey, NULL);
	RegCreateKeyEx(hSubKey, "open", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hOpenKey, NULL);
	RegCreateKeyEx(hOpenKey, "command", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hCmdKey, NULL);
	RegSetValueEx(hCmdKey, NULL, 0, REG_SZ, (BYTE*) (LPCTSTR) strShellCmd, strShellCmd.GetLength()+1);
	
	RegCloseKey(hCmdKey);
	RegCloseKey(hOpenKey);
	RegCloseKey(hSubKey);
	RegCloseKey(hKey);
}


// --- RegisterExtension() ------------------------------------------
void CCryptApp::CreateSendToShortcut(LPTSTR szExePath)
{
	HKEY hSendToKey = 0;
	DWORD dwBufLen = MAX_PATH;
	TCHAR szSendToPath[MAX_PATH];
	long status = 0;

	if (   (RegOpenKeyEx(HKEY_CURRENT_USER, STR_PROFILE_PATH, 0, KEY_READ, &hSendToKey) != ERROR_SUCCESS)
		|| (RegQueryValueEx(hSendToKey, "SendTo", NULL, NULL, (LPBYTE) szSendToPath, &dwBufLen) != ERROR_SUCCESS))
		return;

	strcat(szSendToPath, "\\");
	strcat(szSendToPath, STR_LINK_NAME);
	strcat(szSendToPath, ".lnk");		// for debugging

	LINKDATA linkData;
	linkData.fdwFlags = LD_USEARGS | LD_USEDESC | LD_USEICON | LD_USEDESC | LD_USESHOWCMD;
	linkData.pszPathname = szExePath;
	linkData.pszArgs = "\"%1\"";
	linkData.pszDesc = "Shortcut to Infotech DES Crypt";
	linkData.pszIconPath = szExePath;
	linkData.nIconIndex = 0;
	linkData.nShowCmd = SW_SHOWNORMAL;

	CreateShellLink(szSendToPath, &linkData);
}



// --- DisplayHelp() ------------------------------------------------
// 
void CCryptApp::DisplayHelp()
{
/***
	if (GetMainWnd())
		CWinApp::WinHelp(0, HELP_FINDER);
	else
	{
		// No window displayed, have to use ::WinHelp() ---
		::WinHelp(GetDesktopWindow(), m_pszHelpFilePath, HELP_FINDER, 0);
	}
***/
	CAboutDlg dlg;
	dlg.DoModal();
}


// --- WinHelp() ----------------------------------------------------
// 
void CCryptApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: Add your specialized code here and/or call the base class
	DisplayHelp();
}
