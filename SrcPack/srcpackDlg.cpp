// srcpackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "srcpack.h"
#include "srcpackDlg.h"

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
// CSrcpackDlg dialog

CSrcpackDlg::CSrcpackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSrcpackDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSrcpackDlg)
	m_strZipFile = _T("");
	m_strPassword = _T("");
	m_strPassword2 = _T("");
	m_strMessage = _T("This program will install the source code for TrnsPort XXXXXXXXXX ");
	m_strInstallDir = _T("c:\\TrnsPort\\Source");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSrcpackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSrcpackDlg)
	DDX_Text(pDX, IDC_ZIPFILE, m_strZipFile);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_PASSWORD2, m_strPassword2);
	DDX_Text(pDX, IDC_MESSAGE, m_strMessage);
	DDX_Text(pDX, IDC_INSTALLDIR, m_strInstallDir);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSrcpackDlg, CDialog)
	//{{AFX_MSG_MAP(CSrcpackDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SELECTFILE, OnSelectFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSrcpackDlg message handlers

BOOL CSrcpackDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSrcpackDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSrcpackDlg::OnPaint() 
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

HCURSOR CSrcpackDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSrcpackDlg::OnSelectFile() 
	{
	CString sTmp = "Zip Files (*.ZIP)|*.zip|AllFiles (*.*)|*.*||";

	UpdateData (TRUE);

	CFileDialog  cDlg (TRUE, ".zip", m_strZipFile, OFN_HIDEREADONLY | OFN_ENABLESIZING, sTmp);
	cDlg.m_ofn.lpstrTitle = "Open Source code zip file";
	if (cDlg.DoModal() == IDCANCEL)
		return;

	m_strZipFile = cDlg.m_ofn.lpstrFile;
	UpdateData (FALSE);
	}


void CSrcpackDlg::OnOK() 
	{
	UpdateData (TRUE);

	if (m_strZipFile.IsEmpty() || m_strPassword.IsEmpty() || m_strPassword2.IsEmpty())
		{
		AfxMessageBox ("You must fill in all 3 fields - Bozo!");
		return;
		}
	m_strPassword.MakeLower();
	m_strPassword2.MakeLower();

	if (m_strPassword != m_strPassword2)
		{
		AfxMessageBox ("Password fields don't match - Loser!");
		return;
		}
	if (!Encrypt (m_strZipFile, m_strPassword))
		return;
	CDialog::OnOK();
	}

#define BUFFSIZE 4096
#define OUTFILE  "SRCUNPAK.BRP"
#define MAGICKEY "BRP!"


static CString& GetOutDir (CString strInFile, CString& strOutFile, CString strFixedName)
	{
	char szDrive[_MAX_DRIVE];
	char szDir[_MAX_DIR];
	char szName[_MAX_FNAME];
	char szExt[_MAX_EXT];

	strOutFile.Empty();

	if (strInFile.Find ('\\') || strInFile.Find ('/'))
		{
		_splitpath ((LPCTSTR)strInFile, szDrive, szDir, szName, szExt);
		strOutFile = szDrive;

		if (*szDir && strchr ("\\/", szDir[strlen(szDir)-1]))
			szDir[strlen(szDir)-1] = '\0';
		strOutFile += (CString)szDir + "\\";
		}
	strOutFile += strFixedName;
	return strOutFile;
	}


BOOL CSrcpackDlg::Encrypt (CString strInFile, CString strPWD)
	{
	/*--- open input file ---*/
	CFile cInFile;
	if (!cInFile.Open (strInFile, CFile::modeRead | CFile::shareDenyNone))
		{
		AfxMessageBox ((CString)"Unable to open file: " + strInFile);
		return FALSE;
		}

	/*--- open output file ---*/
	CString strOutFile;
	GetOutDir (strInFile, strOutFile, OUTFILE);

	CFile cOutFile;
	if (!cOutFile.Open (strOutFile, CFile::modeWrite | CFile::modeCreate))
		{
		AfxMessageBox ((CString)"Unable to open file: " + strOutFile);
		cInFile.Close();
		return FALSE;
		}

	char szInBuff[BUFFSIZE];
	char szOutBuff[BUFFSIZE];

	/*--- write magic number to file ---*/
	cOutFile.Write ("BRP!", 4); // fixed length

	/*--- write default dir to file ---*/
	memset (szOutBuff, 0, BUFFSIZE);
	strcpy (szOutBuff, (PSZ)(LPCTSTR)m_strInstallDir);
	cOutFile.Write (szOutBuff, 100);	// fixed length

	/*--- write message to file ---*/
	memset (szOutBuff, 0, BUFFSIZE);
	strcpy (szOutBuff, (PSZ)(LPCTSTR)m_strMessage);
	cOutFile.Write (szOutBuff, 256);	// fixed length

	/*--- write fingerprint string to output file ---*/
	CString strTest;
	strTest.LoadString (IDS_TESTSTRING);
	RC4Crypt (szOutBuff, (PSZ)(LPCTSTR)strTest, strTest.GetLength(), (PSZ)(LPCTSTR)strPWD);
	cOutFile.Write (szOutBuff, strTest.GetLength());

	/*-- encryptomania ---*/
	RC4InitSBox ((PSZ)(LPCTSTR)strPWD);
	while (int iRead = cInFile.Read (szInBuff, BUFFSIZE))
		{
		RC4CryptStream (szOutBuff, szInBuff, iRead);
		cOutFile.Write (szOutBuff, iRead);
		}
	cInFile.Close();
	cOutFile.Close();

	AfxMessageBox ((CString)"Zip successfully encrypted to: " + OUTFILE + 
	               ".\nMake sure to include SRCUNPAK.EXE with this file in your distribution");

	return TRUE;
	}


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

