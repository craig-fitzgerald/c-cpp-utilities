// cdmenuDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mmsystem.h"
#include <stdio.h>
#include <process.h>
#include "cdmenu.h"
#include "Data.h"
#include "dib.h"
#include "cdmenuDlg.h"
#include "MyButton.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCdmenuDlg dialog

CCdmenuDlg::CCdmenuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCdmenuDlg::IDD, pParent)
	{
	//{{AFX_DATA_INIT(CCdmenuDlg)
	//}}AFX_DATA_INIT
	m_hIcon 		  = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pCurrPage   = NULL;
	m_pCurrBitmap = 0;
	m_pPal        = NULL;
	m_iMouseOverID= -1;
	}

void CCdmenuDlg::DoDataExchange(CDataExchange* pDX)
	{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCdmenuDlg)
	//}}AFX_DATA_MAP
	}

BEGIN_MESSAGE_MAP(CCdmenuDlg, CDialog)
	//{{AFX_MSG_MAP(CCdmenuDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_PALETTECHANGED()
	ON_WM_ACTIVATE()
	ON_WM_QUERYNEWPALETTE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCdmenuDlg message handlers

BOOL CCdmenuDlg::OnInitDialog()
	{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	if (!m_data.LoadCfg (CUtil::GetFileName (1)))
		{
		EndDialog (0);
		return FALSE;
		}
	if (!DisplayPage("[Main]"))
		{
		EndDialog (0);
		return FALSE;
		}

	if (!m_pCurrPage->m_strSoundStart.IsEmpty())
		PlaySound (m_pCurrPage->m_strSoundStart, NULL, SND_FILENAME | SND_NODEFAULT | SND_NOWAIT);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
	}


void CCdmenuDlg::OnPaint() 
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

HCURSOR CCdmenuDlg::OnQueryDragIcon()
	{
	return (HCURSOR) m_hIcon;
	}


void CCdmenuDlg::OnDestroy() 
	{
	CDialog::OnDestroy();
	// cleanup GDI object map

	CString strKey;
	CGdiObject *pObject;
	POSITION pos = m_objects.GetStartPosition ();
	while (pos)
		{
		m_objects.GetNextAssoc (pos, strKey, pObject);
		pObject->DeleteObject ();
		delete pObject;
		}
	}


CRect CCdmenuDlg::ButtonPos (int i, CSize sizeScreen)
	{
	CButtonData *pData = m_pCurrPage->m_buttons.ElementAt(i);

	CRect rPrev (0,0,0,0);
	if (i) // only if were not the 1st button
		{
		CWnd *pCtl = GetDlgItem (1000+i-1);
		pCtl->GetWindowRect (rPrev);
		ScreenToClient (rPrev);
		}
	CRect rect    = pData->m_rect;
	CRect rectCpy = rect;

	rect.right  = (rect.right  ? rect.right  : (i ? rPrev.Width()  : 100));
	rect.bottom = (rect.bottom ? rect.bottom : (i ? rPrev.Height() : 25 ));

	if (rect.left == -BPOS_EDGE)	rect.left = pData->m_ptEdge.x;
	if (rect.left ==  BPOS_EDGE)	rect.left = sizeScreen.cx - rect.right - pData->m_ptEdge.x;
	if (rect.top  == -BPOS_EDGE)	rect.top  = pData->m_ptEdge.y;
	if (rect.top  ==  BPOS_EDGE)	rect.top  = sizeScreen.cy - rect.bottom - pData->m_ptEdge.y;
	if (rect.left ==  BPOS_CTR)	rect.left = (sizeScreen.cx - rect.right)/2;
	if (rect.top  ==  BPOS_CTR)	rect.top  = (sizeScreen.cy - rect.bottom)/2;
	if (rect.left == -BPOS_GAP)	rect.left = rPrev.left  - rect.right - pData->m_ptGap.x,
											rect.top  = (rect.top ? rect.top : rPrev.top);
	if (rect.left ==  BPOS_GAP)	rect.left = rPrev.right + pData->m_ptGap.x,
											rect.top  = (rect.top ? rect.top : rPrev.top);
	if (rect.top  == -BPOS_GAP)	rect.top  = rPrev.top   - rect.bottom - pData->m_ptGap.y,
											rect.left =	(rect.left ? rect.left : rPrev.left);
	if (rect.top  ==  BPOS_GAP)	rect.top  = rPrev.bottom + pData->m_ptGap.y,
											rect.left =	(rect.left ? rect.left : rPrev.left);

	if (rectCpy.left   > BPOS_RELATIVE-2000) rect.left   = rPrev.left  + (rectCpy.left   - BPOS_RELATIVE);
	if (rectCpy.top    > BPOS_RELATIVE-2000) rect.top    = rPrev.top   + (rectCpy.top    - BPOS_RELATIVE);
	if (rectCpy.right  > BPOS_RELATIVE-2000) rect.right  = rPrev.right + (rectCpy.right  - BPOS_RELATIVE);
	if (rectCpy.bottom > BPOS_RELATIVE-2000) rect.bottom = rPrev.bottom+ (rectCpy.bottom - BPOS_RELATIVE);

	if (!rect.left) rect.left = rPrev.left;
	if (!rect.top ) rect.top  = rPrev.top;

	rect.right  +=	rect.left;	//	cvt size into loc
	rect.bottom	+=	rect.top;	//	cvt size into loc
	return rect;
	}	

//	iType one of: TYP_FONT, TYP_PEN, TYP_BRUSH,  TYP_BITMAP
//
CObject* CCdmenuDlg::GetGDIObject (int iType, CString strName, int iSize, int iWeight, COLORREF clr)
	{
	CString strKey;
	CGdiObject *pObject;

	strKey.Format ("%d:%s:%d:%d:%d", iType, strName, iSize, iWeight, clr);

	if (!m_objects.Lookup (strKey, pObject))
		{
		switch (iType)
			{
			case TYP_FONT  :
				pObject = (CGdiObject*) new CFont;
				((CFont *)pObject)->CreateFont (iSize, 0, 0, 0, 100*iWeight, 0, 0, 0, 0, 0, 0, PROOF_QUALITY, 0, strName);
				break;
			case TYP_PEN   :
				pObject = (CGdiObject*) new CPen;
				((CPen *)pObject)->CreatePen (PS_SOLID, iSize, clr);
				break;
			case TYP_BRUSH :
				pObject = (CGdiObject*) new CBrush;
				((CBrush*)pObject)->CreateSolidBrush (clr);
				break;
			case TYP_BITMAP:
				{
				//pObject = (CGdiObject*) new CBitmap;
				//HBITMAP hBitmap = (HBITMAP)LoadImage (0, strName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
				//if (!hBitmap)
				//	AfxMessageBox ((CString)"Unable to load image: " + strName);
				//((CBitmap*)pObject)->Attach (hBitmap);

				if (strName.IsEmpty ())
					return NULL;

				pObject = (CGdiObject*) new CDib;
				if (!((CDib*)pObject)->Load (strName))
					{
					AfxMessageBox ((CString)"Unable to load image: " + strName);
					delete pObject;
					return NULL;
					}
				}
				break;
			default:
				pObject = NULL;
			}
		m_objects[strKey] = pObject;
		}
	return pObject;
	}


#define MAXOVERRIDECOLORS 16

static int AddColorIfNeeded (COLORREF* colors, COLORREF clr, int iColorCount)
	{
	if (iColorCount+1 >= MAXOVERRIDECOLORS) 		// array full ?
		return iColorCount;

	if	(clr == GetSysColor (COLOR_BTNFACE)   ||	  	// a std color?
		 clr == GetSysColor (COLOR_3DSHADOW)  ||
		 clr == GetSysColor (COLOR_3DHILIGHT) ||
		 clr == GetSysColor (COLOR_BTNTEXT)   )
		 return iColorCount;

	for (int i=0; i<iColorCount; i++)				// already got it?
		if (colors[i] == clr)
			return iColorCount;

	colors[iColorCount++] = clr;						// add to list
		return iColorCount;
	}


int CCdmenuDlg::ExamineColorSet ()
	{
	if (!m_pCurrBitmap)
		return 0;

	COLORREF* colors = new COLORREF[MAXOVERRIDECOLORS];
	int i, iNewColors = 0;

	iNewColors = AddColorIfNeeded (colors, m_pCurrPage->m_sdMsg.m_clrFont, iNewColors);

	for (i=0; i< m_pCurrPage->m_buttons.GetSize () &&  iNewColors+1 < MAXOVERRIDECOLORS; i++)
		iNewColors = AddColorIfNeeded (colors, (m_pCurrPage->m_buttons.ElementAt(i))->m_clrNormal, iNewColors);
	for (i=0; i< m_pCurrPage->m_buttons.GetSize () &&  iNewColors+1 < MAXOVERRIDECOLORS; i++)
		iNewColors = AddColorIfNeeded (colors, (m_pCurrPage->m_buttons.ElementAt(i))->m_sdBtn.m_clrFont, iNewColors);
	for (i=0; i< m_pCurrPage->m_buttons.GetSize () &&  iNewColors+1 < MAXOVERRIDECOLORS; i++)
		iNewColors = AddColorIfNeeded (colors, (m_pCurrPage->m_buttons.ElementAt(i))->m_sdMsg.m_clrFont, iNewColors);
	for (i=0; i< m_pCurrPage->m_buttons.GetSize () &&  iNewColors+1 < MAXOVERRIDECOLORS; i++)
		iNewColors = AddColorIfNeeded (colors, (m_pCurrPage->m_buttons.ElementAt(i))->m_clrHilight, iNewColors);
	for (i=0; i< m_pCurrPage->m_buttons.GetSize () &&  iNewColors+1 < MAXOVERRIDECOLORS; i++)
		iNewColors = AddColorIfNeeded (colors, (m_pCurrPage->m_buttons.ElementAt(i))->m_clrPush, iNewColors);

	if (iNewColors)
		return m_pCurrBitmap->AdjustPalette (colors, iNewColors);
	return iNewColors;
	}



BOOL CCdmenuDlg::DisplayPage(CString strPage)
	{
	CPageData *pNewPage;
	if (!(pNewPage = m_data.FindPage (strPage)))
		{
		AfxMessageBox ((CString)"Unable to Find Page: " + strPage);
		return FALSE;
		}

	/*--- remove the old buttons ---*/
	CWnd *pCtl;
	if (m_pCurrPage)
		for (int i=0; i< m_pCurrPage->m_buttons.GetSize (); i++)
			if (pCtl = GetDlgItem (1000+i))
				delete pCtl;
	Invalidate (TRUE);


	/*--- replace background image? ---*/
	if (!m_pCurrPage || m_pCurrPage->m_strBitmap != pNewPage->m_strBitmap)
		{
		m_pCurrBitmap = (CDib*)GetGDIObject (TYP_BITMAP, pNewPage->m_strBitmap, 0, 0, 0);
		if (!m_pCurrBitmap && pNewPage->m_clientSize == CSize (0, 0))
			{
			AfxMessageBox ((CString)"No page image or page size specified");
			return FALSE;
			}

		CRect rect;
		if (pNewPage->m_clientSize == CSize (0, 0))
			pNewPage->m_clientSize = CSize (m_pCurrBitmap->m_bm.bmWidth, m_pCurrBitmap->m_bm.bmHeight);
		rect = CRect(0, 0, pNewPage->m_clientSize.cx, pNewPage->m_clientSize.cy);

		DWORD dwStyle = GetWindowLong (this->m_hWnd, GWL_STYLE);
		if (pNewPage->m_bHasCaption)
			{
			dwStyle |= WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
			SetWindowLong (m_hWnd, GWL_STYLE, dwStyle);

			rect.right  += GetSystemMetrics (SM_CXFIXEDFRAME) * 2;
			rect.bottom += GetSystemMetrics (SM_CYFIXEDFRAME) * 2;
			rect.bottom += GetSystemMetrics (SM_CYCAPTION);
			}
		else
			{
			dwStyle &= ~(WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU);
			}

		SetWindowLong (m_hWnd, GWL_STYLE, dwStyle);
		rect += CPoint ((GetSystemMetrics (SM_CXSCREEN) - rect.Width())/2,
					  		 (GetSystemMetrics (SM_CYSCREEN) - rect.Height())/2);

		MoveWindow (rect);
		}
	else
		{
		m_pCurrBitmap = m_pCurrBitmap;
		if (pNewPage->m_clientSize == CSize (0, 0))
			pNewPage->m_clientSize = CSize (m_pCurrBitmap->m_bm.bmWidth, m_pCurrBitmap->m_bm.bmHeight);
		}
	m_pCurrPage = pNewPage;

	SetWindowText (m_pCurrPage->m_strTitle);

	ExamineColorSet ();
	if (m_pCurrBitmap)
		{
		m_pPal = m_pCurrBitmap->GetPalette ();
		DoRealizePalette(TRUE);
		}

	DWORD dwStyles = BS_PUSHBUTTON | WS_CHILD | WS_TABSTOP | BS_OWNERDRAW | WS_VISIBLE;

	/*--- create new buttons ---*/
	for (int i=0; i< m_pCurrPage->m_buttons.GetSize (); i++)
		{
		CButtonData *pData = m_pCurrPage->m_buttons.ElementAt(i);
		pData->m_iID = 1000+i;
		CMyButton *pButton = new CMyButton (pData, this);
		CRect rect = ButtonPos(i, pNewPage->m_clientSize);
		pButton->Create (pData->m_sdBtn.m_str, dwStyles, rect, this, pData->m_iID);

		pButton->m_pExternalPal = (m_pCurrBitmap ? m_pCurrBitmap->GetPalette () : NULL);

//pButton->ModifyStyleEx (0, WS_EX_TRANSPARENT);
		}
	m_iMouseOverID = -1;
	return TRUE;
	}


static BOOL IsSection (CString strBuff)
	{
	if (strBuff.IsEmpty () || strBuff[0] != '[')
		return FALSE;
	return (strBuff[strBuff.GetLength()-1] == ']');
	}

///*
// *	iLevel 0	- return drive only
// *			 1	- return parent dir
// *			 2	- dir of file in FilePath
// */
//static PSZ GetDir (PSZ pszFilePath, PSZ pszDir, int iLevel)
//	{
//	PSZ  p;
//	char szDrive[_MAX_DRIVE];
//	char szDir[_MAX_DIR];
//	char szName[_MAX_FNAME];
//	char szExt[_MAX_EXT];
//
//	_splitpath (pszFilePath, szDrive, szDir, szName, szExt);
//	strcpy (pszDir, szDrive);
//	if (!iLevel)
//		return pszDir; // drive only
//
//	if (*szDir && strchr ("\\/", szDir[strlen(szDir)-1]))
//		szDir[strlen(szDir)-1] = '\0';
//	strcat (pszDir, szDir);
//	if (iLevel == 2)
//		return pszDir; // full dir
//
//	if ((p = strrchr (pszDir, '\\')) || (p = strrchr (pszDir, '/')))
//		*p = '\0';
//	return pszDir; // parent dir
//	}


void CCdmenuDlg::SplitCmd (CString strCmdLine, CString &strFile, CString &strParams, CString &strExt)
	{
 	strFile.Empty();
 	strParams.Empty();
 	strExt.Empty();

	CString strCfg = CUtil::GetFileName (1);
	CString strDir;
	/*--- look for replacements ---*/
	if (strCmdLine.Find ("%here%") > -1)
		strCmdLine.Replace ("%here%", 	CUtil::GetDir (strCfg, strDir, 2));
	if (strCmdLine.Find ("%cfgdir%") > -1)
		strCmdLine.Replace ("%cfgdir%", 	CUtil::GetDir (strCfg, strDir, 2));
	if (strCmdLine.Find ("%parent%") > -1)
		strCmdLine.Replace ("%parent%", 	CUtil::GetDir (strCfg, strDir, 1));
	if (strCmdLine.Find ("%up%") > -1)
		strCmdLine.Replace ("%up%", 		CUtil::GetDir (strCfg, strDir, 1));
	if (strCmdLine.Find ("%root%") > -1)
		strCmdLine.Replace ("%root%", 	CUtil::GetDir (strCfg, strDir, 0));
	if (strCmdLine.Find ("%exedir%") > -1)
		strCmdLine.Replace ("%exedir%", CUtil::GetDir (CUtil::GetFileName (0), strDir, 2));

	strCmdLine.TrimLeft ();
	strCmdLine.TrimRight ();

	strFile = "";
	strParams = "";
	int iInQuotes, i;

	for (iInQuotes=i=0; i<strCmdLine.GetLength(); i++)
		{
		if (strCmdLine[i] == '"')
			iInQuotes = 1 - iInQuotes;
		else if (!iInQuotes && (strCmdLine[i] == ' ' || strCmdLine[i] == '\t'))
			break;
		else
			strFile += strCmdLine[i];
		}
	if (i+1 < strCmdLine.GetLength())
		strParams = strCmdLine.Mid (i+1);
	}


void CCdmenuDlg::HandleButton (CButtonData* pData)
	{
	CString strCmd = pData->m_strCmd;

	if (strCmd.IsEmpty ()) // the Exit command
		{
		if (!m_pCurrPage->m_strSoundEnd.IsEmpty())
			PlaySound (m_pCurrPage->m_strSoundEnd, NULL, SND_FILENAME | SND_NODEFAULT | SND_NOWAIT);
		EndDialog (0);
		}
	else if (strCmd[0] == '[' && strCmd[strCmd.GetLength()-1] == ']') // a new page
		{
		if (!pData->m_strSound.IsEmpty())
			PlaySound (pData->m_strSound, NULL, SND_FILENAME | SND_NODEFAULT | SND_NOWAIT);
		DisplayPage (strCmd);
		}
	else // exe / dir / url
		{
		char szDummy[256];
		CFileType* pType;
		CString strFile, strParams, strExt;

		SplitCmd (strCmd, strFile, strParams, strExt);

		if (!pData->m_strSound.IsEmpty())
			PlaySound (pData->m_strSound, NULL, SND_FILENAME | SND_NODEFAULT | SND_NOWAIT);

	int iResult;
	if ((strCmd[0] == '\\' && strCmd.GetLength() == 1) ||	  // FindExecutable chokes on
		!strnicmp ((LPCTSTR)strCmd, "http://", 7) 		||	  // URL's and the \ cmds
		!strnicmp ((LPCTSTR)strCmd, "mailto:", 7))			  //
		iResult = 100;
	else
		iResult = (int)FindExecutable(strFile, "", szDummy);

		/*--- see if we can really launch this cmd ---*/
		switch (iResult)
			{
			case 0: 							
				AfxMessageBox ("The system is out of memory or resources. "); 
				break;
			case ERROR_FILE_NOT_FOUND: 
				AfxMessageBox ((CString)"The file "+ strFile +" was not found. "); 
				break;
			case ERROR_PATH_NOT_FOUND: 
				AfxMessageBox (CString("The path ")+ strFile +" was not found. "); 
				break;
			case ERROR_BAD_FORMAT: 		
				AfxMessageBox ((CString)"The file "+ strFile +" is invalid (non-Win32 .exe or error in .exe)."); 
				break;
			case 31:
				if (pType = m_data.HaveInstallForType (strFile))
					{
					CString strMsg;
					strMsg.Format ("The file you are attempting to run: %s cannot be executed until you install %s.  "
					               "Would you like to install this program now?", strFile, pType->m_strName);
					if (AfxMessageBox (strMsg, MB_YESNO) == IDYES)
						ShellExecute (this->m_hWnd, "open", pType->m_strCmd, "", "", SW_SHOWDEFAULT);
					}
				else
					AfxMessageBox ((CString)"There is no association for the file " + strFile);
				break;
			default:							
				{
				PSZ p;

				//AfxMessageBox ("CmdLine:"+strCmd+"\n____Exe:"+strFile+"\n__Params:"+strParams);
				int iRet = (int)ShellExecute (this->m_hWnd, "open", strFile, strParams, "", SW_SHOWDEFAULT);
				if (iRet >= 32)
					return; // All OK
				switch (iRet)
					{
					case 0:  							p = "The operating system is out of memory or resources. "; break;
					case ERROR_FILE_NOT_FOUND:  	p = "The file %s was not found.  "; break;
					case ERROR_PATH_NOT_FOUND:  	p = "The path %s was not found.  "; break;
					case ERROR_BAD_FORMAT: 		 	p = "The .exe file %s is invalid (non-Win32.exe or error in .exe).  "; break;
					case SE_ERR_ACCESSDENIED: 		p = "The operating system denied access to the file %s.   "; break;
					case SE_ERR_ASSOCINCOMPLETE:  p = "The file name association is incomplete or invalid: %s.  "; break;
					case SE_ERR_DDEBUSY:  			p = "The DDE transaction could not be completed: %s. "; break;
					case SE_ERR_DDEFAIL:  			p = "The DDE transaction failed: %s.  "; break;
					case SE_ERR_DDETIMEOUT:  		p = "The DDE transaction could not be completed because the request timed out: %s.  "; break;
					case SE_ERR_DLLNOTFOUND:  		p = "The dynamic-link library %s was not found.   "; break;
					case SE_ERR_NOASSOC:  			p = "There is no application associated with the file %s.  "; break;
					case SE_ERR_OOM:  				p = "There was not enough memory to open %s.  "; break;
					case SE_ERR_SHARE:  				p = "A sharing violation occurred opening %s.  "; break;
					}
				CString strErr;
				strErr.Format (p, strFile);
				AfxMessageBox (strErr);
				}
			}
		}
	}



BOOL CCdmenuDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
	{
	INT iID  = LOWORD (wParam);
	INT iMsg = HIWORD (wParam);

	if (iMsg != BN_CLICKED || iID < 1000 || iID > 1000 + m_pCurrPage->m_buttons.GetSize ())
		return CDialog::OnCommand(wParam, lParam);

	CButtonData *pData = m_pCurrPage->m_buttons.ElementAt(iID - 1000);

	HandleButton (pData);
	return TRUE;
	}



void CCdmenuDlg::OnMouseMove(UINT nFlags, CPoint point) 
	{
	CDialog::OnMouseMove(nFlags, point);
	HandleMouseMovement (nFlags, point, 0);
	}



BOOL CCdmenuDlg::PreCreateWindow(CREATESTRUCT& cs) 
	{

	// TODO: Add your specialized code here and/or call the base class
	return CDialog::PreCreateWindow(cs);
	}


void CCdmenuDlg::DisplayStringDat (CDC* pDC, CStringDat* psd)
	{
	if (!pDC || !psd)
		return;

	CFont *pFont = (CFont*) GetGDIObject (TYP_FONT, psd->m_strFontName, psd->m_iFontSize, psd->m_iFontWeight, 0);
   pDC->SetTextColor(psd->m_clrFont);
   pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject (pFont);

	DWORD dwFlags = DT_WORDBREAK|DT_VCENTER| (psd->m_iJustify==0 ? DT_CENTER : (psd->m_iJustify==1 ? DT_LEFT : DT_RIGHT));
	pDC->DrawText(psd->m_str, psd->m_str.GetLength(), psd->m_rect, dwFlags);
	pDC->SelectObject (pOldFont);
	}

BOOL CCdmenuDlg::PaintBitmapBackground(CDC* pDC, CRect cRect, CPoint ptOffset) 
	{
	if (!m_pCurrBitmap)
		return FALSE;
	m_pCurrBitmap->FillBmp (*pDC, cRect, ptOffset, FALSE);
	return TRUE;
	}


BOOL CCdmenuDlg::OnEraseBkgnd(CDC* pDC) 
	{
	CRect cRect;
	GetClientRect (cRect);

	if (m_pCurrBitmap)
		m_pCurrBitmap->FillBmp (*pDC, cRect, FALSE);
	else
		CDialog::OnEraseBkgnd(pDC);

	if (!m_pCurrPage)
		return TRUE;

	/*--- optimize with GetUpdateRect! ---*/
	POSITION pos = m_pCurrPage->m_StaticMsgList.GetHeadPosition();
	while (pos)
		{
		CStringDat* psd = m_pCurrPage->m_StaticMsgList.GetNext (pos);
		DisplayStringDat (pDC, psd);
		}
	return TRUE;
	}



void CCdmenuDlg::SetMessage (CStringDat* psd)
	{
//	define an == operator someday ...
//	if (m_sdCurr == *psd)
//		return;

	m_sdCurr.m_str.Empty();
	InvalidateRect (m_sdCurr.m_rect); // clear out the old message
	UpdateWindow ();


	if (!psd || psd->m_str.IsEmpty() || psd->m_rect == CRect (0,0,0,0))
		return;

//	if (psd->m_rect != m_sdCurr.m_rect)
//		{
//		InvalidateRect (psd->m_rect);
//		UpdateWindow ();
//		}

	m_sdCurr = *psd;

	CDC* pDC = GetDC ();
	DisplayStringDat (pDC, psd);
	}

void CCdmenuDlg::HandleMouseMovement (UINT nFlags, CPoint point, int iID)
	{
	CMyButton* pCtl;

	if (iID == m_iMouseOverID)
		return; // no change

	/*--- notify button that mouse has left the building ---*/
	if (m_iMouseOverID >= 1000 && (pCtl = (CMyButton*)GetDlgItem (m_iMouseOverID)))
		pCtl->HandleMouseMovement (nFlags, point, FALSE);
	else if (!m_iMouseOverID)
		{
		/*--- for now, main wnd doesn't need to know it's losing the mouseover ---*/
		}

	/*--- notify button that mouse has entered the building ---*/
	if (iID >= 1000 && (pCtl = (CMyButton*)GetDlgItem (iID)))
		pCtl->HandleMouseMovement (nFlags, point, TRUE);
	else if (!iID)
		SetMessage (&m_pCurrPage->m_sdMsg);

	m_iMouseOverID = iID;
	}


int CCdmenuDlg::DoRealizePalette(BOOL bForeground)
	{
	int iNewColors;

	if (!m_pCurrBitmap)
		return 0;

	CClientDC dc = this;
	if (iNewColors = m_pCurrBitmap->RealizePalette (dc, bForeground))
		Invalidate(TRUE);
	return iNewColors;
	}

void CCdmenuDlg::OnPaletteChanged(CWnd* pFocusWnd) 
	{
	//CDialog::OnPaletteChanged(pFocusWnd);
	if (pFocusWnd!=this) 
		DoRealizePalette(FALSE);
	}


CPalette* CCdmenuDlg::GetPalette ()
	{
	return m_pPal;
	}

void CCdmenuDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
	{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	if (nState != WA_INACTIVE)
		DoRealizePalette(TRUE);
	// TODO: Add your message handler code here
	}

BOOL CCdmenuDlg::OnQueryNewPalette() 
	{
	// TODO: Add your message handler code here and/or call default
	
	DoRealizePalette(TRUE);
	return TRUE;
	}
