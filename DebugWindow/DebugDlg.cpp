// DebugDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Debug.h"
#include "DebugDlg.h"

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

	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDebugDlg dialog

CDebugDlg::CDebugDlg(CWnd* pParent /*=NULL*/) : CDialog(CDebugDlg::IDD, pParent)
   {
	//{{AFX_DATA_INIT(CDebugDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
   }

void CDebugDlg::DoDataExchange(CDataExchange* pDX)
   {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDebugDlg)
	//}}AFX_DATA_MAP
   }

BEGIN_MESSAGE_MAP(CDebugDlg, CDialog)
	//{{AFX_MSG_MAP(CDebugDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_COPYDATA()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDebugDlg message handlers

BOOL CDebugDlg::OnInitDialog()
   {
	CDialog::OnInitDialog();

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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
   }

void CDebugDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDebugDlg::OnPaint() 
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

HCURSOR CDebugDlg::OnQueryDragIcon()
   {
	return (HCURSOR) m_hIcon;
   }

int CDebugDlg::AddMessage (LPCTSTR pszMessage)
   {
   CListBox* pList = (CListBox*)GetDlgItem (IDC_LIST);
   if (!pszMessage)
      return pList->AddString ("[Message was Null]");
   return pList->AddString (pszMessage);
   }

BOOL CDebugDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCDS) 
   {
   char* psz = (char*)pCDS->lpData;

   if (!pCDS->cbData || !pCDS->lpData)
      return TRUE;

   char szBuff[1024];
   strncpy (szBuff, (char*)pCDS->lpData, min (1023, pCDS->cbData));
   szBuff [min (1023, pCDS->cbData)] = '\0';

   AddMessage (szBuff);

//   if (psz && *psz && strlen (psz) > pCDS->cbData)
//      psz[pCDS->cbData] = '\0';
//
//   AddMessage ((LPCTSTR)pCDS->lpData);
   return TRUE;
   }

void CDebugDlg::OnSize(UINT nType, int cx, int cy) 
   {
	CDialog::OnSize(nType, cx, cy);

   CWnd* pList = GetDlgItem (IDC_LIST);
   CWnd* pBtn  = GetDlgItem (IDC_CLEAR);
   if (!pList || !pBtn)
      return;
   CRect cListRect;
   pList->GetWindowRect (cListRect);
   ScreenToClient (cListRect);
   int iDelta = cListRect.left;

   CRect cBtnRect;
   pBtn->GetWindowRect (cBtnRect);
   ScreenToClient (cBtnRect);
   pBtn->MoveWindow (iDelta, cy-iDelta-cBtnRect.Height(), cBtnRect.Width(), cBtnRect.Height());

   cListRect.right  = cx - iDelta;
   cListRect.bottom = cy-iDelta*2-cBtnRect.Height();
   pList->MoveWindow (cListRect);
   }

void CDebugDlg::OnClear() 
   {
   CListBox* pList = (CListBox*)GetDlgItem (IDC_LIST);
   pList->ResetContent ();
   }

