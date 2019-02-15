// srcunpakDlg.cpp : implementation file
//

#include "stdafx.h"
#include "srcunpak.h"
#include "srcunpakDlg.h"

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>

#include <GnuDir.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSrcunpakDlg dialog

CSrcunpakDlg::CSrcunpakDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSrcunpakDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSrcunpakDlg)
	m_strInstallDir = _T("");
	m_strMessage = _T("");
	m_strPassword = _T("");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSrcunpakDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSrcunpakDlg)
	DDX_Text(pDX, IDC_INSTALLDIR, m_strInstallDir);
	DDX_Text(pDX, IDC_MESSAGE, m_strMessage);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSrcunpakDlg, CDialog)
	//{{AFX_MSG_MAP(CSrcunpakDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSrcunpakDlg message handlers

BOOL CSrcunpakDlg::MakeInputFileName ()
	{
	TCHAR strName[_MAX_PATH];
	GetModuleFileName(NULL, strName, _MAX_PATH);
	PCHAR pExt = strrchr(strName, '.');
	if (pExt == NULL)
		pExt = strName + strlen(strName);	// no '.', go to end of name
	strcpy(pExt, ".brp");
	m_strInFile = strName;
	return TRUE;
	}

static BOOL MyMessage (CString str)
	{
	AfxMessageBox (str);
	return FALSE;
	}

#define BUFFSIZE 4096
#define MAGICKEY "BRP!"


BOOL CSrcunpakDlg::ReadFileHeader ()
	{
	CFile cInFile;

	char szInBuff[BUFFSIZE];

	if (!cInFile.Open (m_strInFile, CFile::modeRead | CFile::shareDenyNone))
		return MyMessage ((CString)"Unable to open file: " + m_strInFile);
	if (cInFile.Read (szInBuff, 4) < 4)
		return MyMessage ((CString)"Unable to read file: " + m_strInFile);

	/*--- check magic number ---*/
	if (strncmp (szInBuff, MAGICKEY, 4))
		return MyMessage ((CString)"Incorrect File Type: " + m_strInFile);

	/*--- read default install dir ---*/
	if (cInFile.Read (szInBuff, 100) < 100)
		return MyMessage ((CString)"Unable to read file: " + m_strInFile);
	m_strInstallDir = szInBuff;

	/*--- read message ---*/
	if (cInFile.Read (szInBuff, 256) < 256)
		return MyMessage ((CString)"Unable to read file: " + m_strInFile);
	m_strMessage = szInBuff;

	/*--- read fingerprint ---*/
	m_strTest.LoadString (IDS_TESTSTRING);
	if (cInFile.Read (m_bFingerprintBuff, m_strTest.GetLength()) < (UINT)m_strTest.GetLength())
		return MyMessage ((CString)"Unable to read file: " + m_strInFile);

	cInFile.Close();
	UpdateData (FALSE);
	return TRUE;
	}


BOOL CSrcunpakDlg::CheckPassword(CString strPassword)
	{
	char szBuff[BUFFSIZE];

	RC4Crypt (szBuff, (PSZ)m_bFingerprintBuff, m_strTest.GetLength(), (PSZ)(LPCTSTR)strPassword);
	szBuff[m_strTest.GetLength()] = '\0';
	if (strcmp (szBuff, m_strTest))
		return MyMessage ("Incorrect Password");
	return TRUE;
	}


BOOL CSrcunpakDlg::OnInitDialog()
	{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
		{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
			{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
			}
		}
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	MakeInputFileName ();
	ReadFileHeader ();
	return TRUE;
	}

void CSrcunpakDlg::OnSysCommand(UINT nID, LPARAM lParam)
	{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
		{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
		}
	else
		{
		CDialog::OnSysCommand(nID, lParam);
		}
	}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSrcunpakDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CSrcunpakDlg::OnQueryDragIcon()
	{
	return (HCURSOR) m_hIcon;
	}


BOOL CSrcunpakDlg::MakeDir (CString strPath)
	{
	CString strTmp = strPath;

	/*--- trim trailing \\ if it exists ---*/
	strTmp.Replace ("/","\\");
	strTmp.TrimRight ("\\");
	return DirMakePath ((PSZ)(LPCTSTR)strTmp);
	}


BOOL CSrcunpakDlg::CheckDestDir ()
	{
	if (m_strInstallDir.IsEmpty())
		return MyMessage ((CString)"You must supply a destination directory");

	/*--- trim trailing \\ if it exists ---*/
	m_strInstallDir.Replace ("/","\\");
	m_strInstallDir.TrimRight ("\\");
	if (!MakeDir (m_strInstallDir))
		return MyMessage ((CString)"Couldn't create dir: " + m_strInstallDir);
	return TRUE;
	}


BOOL CSrcunpakDlg::DecryptFile (CString strPWD)
	{
	GetDlgItem(IDC_MESSAGE)->SetWindowText ((CString)"Decrypting data...");
	UpdateWindow ();


	m_strZipFile = m_strInstallDir + "\\source.zip";

	CFile cInFile;
	if (!cInFile.Open (m_strInFile, CFile::modeRead | CFile::shareDenyNone))
		MyMessage ((CString)"Unable to open input file: " + m_strInFile);

	CFile cOutFile;
	if (!cOutFile.Open (m_strZipFile, CFile::modeWrite | CFile::modeCreate))
		MyMessage ((CString)"Unable to open output file: " + m_strZipFile);

	char szInBuff[BUFFSIZE];
	char szOutBuff[BUFFSIZE];

	cInFile.Read (szInBuff, 256+100+4+m_strTest.GetLength()); // skip header junk

	/*-- encryptomania ---*/
	RC4InitSBox ((PSZ)(LPCTSTR)strPWD);
	while (int iRead = cInFile.Read (szInBuff, BUFFSIZE))
		{
		RC4CryptStream (szOutBuff, szInBuff, iRead);
		cOutFile.Write (szOutBuff, iRead);
		}
	cInFile.Close();
	cOutFile.Close();
	return TRUE;
	}




static BOOL HasPathInfo (PSZ pszFileName, PSZ pszPath, PSZ pszName)
	{
	PSZ psz;

	CString str = pszFileName;
	str.Replace ("/", "\\");
	strcpy (pszPath, (LPCTSTR)str);

	if (psz = strrchr (pszPath, '\\'))
		{
		*psz = '\0';
		strcpy (pszName, psz+1);
		}
	else
		{
		strcpy (pszName, pszPath);
		*pszPath = '\0';
		}
	return !!psz;
	}



BOOL CSrcunpakDlg::UnpackFile ()
	{
	CZipArchive zip;

	_chdir (m_strInstallDir);

	if (zip.OpenArchive (m_strZipFile, 0) < 0)
		return MyMessage ((CString)"Unable to open zip file for reading: " + m_strZipFile);

	char szFileName[_MAX_PATH];
	char szName[_MAX_PATH];
	char szPath[_MAX_PATH];
	int iRead;

	char szBuff[BUFFSIZE];

 	zip.GotoFirstFile ();
	while (zip.GetCurrentFileInfo (szFileName, _MAX_PATH, NULL, 0) >= 0)
		{
		if (HasPathInfo (szFileName, szPath, szName))
			MakeDir (szPath);
		if (*szName)
			{
			if (zip.OpenCurrentFile () < 0)
				{
				zip.CloseArchive();
				return MyMessage ((CString)"Error reading zip file: " + m_strZipFile);
				}
			CFile cOutFile;
			if (!cOutFile.Open (szFileName, CFile::modeWrite | CFile::modeCreate))
				MyMessage ((CString)"Unable to open output file: " + szFileName);
			else
				{
				GetDlgItem(IDC_MESSAGE)->SetWindowText ((CString)"Writing: " + szName);
				UpdateWindow ();

				while (iRead = zip.Read (szBuff, BUFFSIZE))
					cOutFile.Write (szBuff, iRead);
				cOutFile.Close();
				}
			}
 		if (zip.GotoNextFile () < 0)
			break;
		}
	zip.CloseArchive ();
		
	MyMessage ((CString)"Source code install is complete!");
	return TRUE;
	}


void CSrcunpakDlg::OnOK() 
	{
	UpdateData (TRUE);

	m_strPassword.MakeLower();
	if (!CheckPassword(m_strPassword))
		return;
	if (!CheckDestDir ())
		return;
	if (!DecryptFile (m_strPassword))
		return;
	if (!UnpackFile ())
		return;
	
	CDialog::OnOK();
	}
