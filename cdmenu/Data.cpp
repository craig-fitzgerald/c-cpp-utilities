// Data.cpp: implementation of the CData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cdmenu.h"
#include "Data.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CData::CData()
	{
	}

CData::~CData()
	{
	}


static void CleanStr (CString &strBuff)
	{
	int iPos;

	/*--- Cut off any possible comment ---*/
	if ((iPos = strBuff.Find (';')) > -1)
		strBuff = strBuff.Left (iPos);
	strBuff.TrimRight ();
	strBuff.TrimLeft ();
	}

static BOOL IsSection (CString strBuff)
	{
	if (strBuff.IsEmpty () || strBuff[0] != '[')
		return FALSE;
	return (strBuff[strBuff.GetLength()-1] == ']');
	}




static CString* CSVSplit (CString s, CString *ps)
	{
	int i, iStr;

	for (i=0; i < 10; i++)
		ps[i] = "";

	BOOL bLastCharWasQuote = FALSE, bInQuotes=FALSE;
	for (i=iStr=0; i < s.GetLength(); i++)
		{
		if (!bInQuotes && s[i] == ',')
			{
			iStr++;
			bLastCharWasQuote = FALSE;
			continue;
			}
		if (s[i] != '"')
			{
			ps[iStr] += s[i];
			bLastCharWasQuote = FALSE;
			continue;
			}
		if (bInQuotes)
			{
			bInQuotes = FALSE;
			bLastCharWasQuote = TRUE;
			continue;
			}
		if (bLastCharWasQuote)
			ps[iStr] += s[i];
		bInQuotes = TRUE;
		bLastCharWasQuote = TRUE;
		}
	for (i=0; i < 10; i++)
		ps[i].TrimLeft(), ps[i].TrimRight();
	return ps;
	}

CExpr* pExpr = NULL;

double AToBIG (LPCTSTR psz)
	{
	if (!pExpr)
		pExpr = (CExpr*)new CExpr;
	return pExpr->Eval (psz);
	}


static int HexVal(CString str)
	{
	int i = (str[0] > '9' ? str[0]-'A'+10 : str[0]-'0');
	int j = (str[1] > '9' ? str[1]-'A'+10 : str[1]-'0');
	return (i<<4) + j;
	}



typedef struct 
	{
	PSZ pszColor;
	int iIdx;
	} CLRMAP;

static CLRMAP ColorMap[] =
	{	{"3ddkshadow"				 	,	COLOR_3DDKSHADOW				  },
		{"3dface"						,	COLOR_3DFACE					  },	
		{"3dhilight"					,	COLOR_3DHILIGHT				  },	
		{"3dlight"					 	,	COLOR_3DLIGHT					  },
		{"3dshadow"					 	,	COLOR_3DSHADOW					  },
		{"activeborder"				,	COLOR_ACTIVEBORDER			  },	
		{"activecaption"			 	,	COLOR_ACTIVECAPTION			  },
		{"appworkspace"				,	COLOR_APPWORKSPACE			  },	
		{"background"				 	,	COLOR_BACKGROUND				  },
		{"btntext"					 	,	COLOR_BTNTEXT					  },
		{"captiontext"				 	,	COLOR_CAPTIONTEXT				  },
		{"graytext"					 	,	COLOR_GRAYTEXT					  },
		{"highlight"					,	COLOR_HIGHLIGHT				  },	
		{"highlighttext"			 	,	COLOR_HIGHLIGHTTEXT			  },
		{"inactiveborder"			 	,	COLOR_INACTIVEBORDER			  },
		{"inactivecaption"			,	COLOR_INACTIVECAPTION		  },	
		{"inactivecaptiontext"	 	,	COLOR_INACTIVECAPTIONTEXT	  },
		{"infobk"						,	COLOR_INFOBK					  },	
		{"infotext"					 	,	COLOR_INFOTEXT					  },
		{"menu"						 	,	COLOR_MENU						  },
		{"menutext"					 	,	COLOR_MENUTEXT					  },
		{"scrollbar"					,	COLOR_SCROLLBAR				  },	
		{"window"						,	COLOR_WINDOW					  },	
		{"windowframe"				 	,	COLOR_WINDOWFRAME				  },
		{"windowtext"				 	,	COLOR_WINDOWTEXT				  },
		{NULL							 	,	0}
	};



static COLORREF CvtColor (CString strBuff, COLORREF clrRef)
	{
	strBuff.MakeLower ();

	clrRef = (clrRef == -1 ? GetSysColor (COLOR_BTNFACE) : clrRef);

	/*--- blank or malformed ---*/
	if (strBuff.IsEmpty ()|| (strBuff[0]=='#' && strBuff.GetLength() < 7))
		return clrRef; // blank means system color

	/*--- WEB style ---*/
	if (strBuff[0] == '#')
		return RGB (HexVal(strBuff), HexVal(strBuff.Mid(2)), HexVal(strBuff.Mid(4)));

	/*--- a couple of specials ---*/
	if (strBuff == "lighter")
		return RGB (128 + GetRValue (clrRef)/2, 128 + GetGValue (clrRef)/2, 128 + GetBValue (clrRef)/2);
	if (strBuff == "darker")
		return RGB (GetRValue (clrRef)/2, GetGValue (clrRef)/2, GetBValue (clrRef)/2);
	if (strBuff == "same")
		return clrRef;

	/*--- a system color ? ---*/
	for (int i=0; ColorMap[i].pszColor; i++)
		if (strBuff == ColorMap[i].pszColor)
			return GetSysColor (ColorMap[i].iIdx);

	CString ps[10];
	CSVSplit (strBuff, ps);

	/*--- specified as a base 10 long ---*/
	if (ps[1].IsEmpty ())
		return (COLORREF)(int)AToBIG (ps[0]);

	/*--- normal r,g,b spec ---*/
	UINT uR = (UINT) AToBIG (ps[0]);
	UINT uG = (UINT) AToBIG (ps[1]);
	UINT uB = (UINT) AToBIG (ps[2]);
	return RGB(uR,uG,uB);
	}




static CPoint CvtPoint (CString strBuff)
	{
	CPoint  point;
	CString ps[10];
	CSVSplit (strBuff, ps);
	point.x = (INT) AToBIG (ps[0]);
	point.y = (INT) AToBIG (ps[1]);
	if (!ps[0].IsEmpty() && !strBuff.Find(',')) point.y = point.x;
	return  point;
	}

static CRect CvtSizeRect (CString strBuff)
	{
	static CRect rectLast (0,0,0,0);
	CRect rect;
	CString ps[20];
	CSVSplit (strBuff, ps);
	rect.left	= (INT) AToBIG ((PSZ)(LPCTSTR)ps[0]);
	rect.top		= (INT) AToBIG ((PSZ)(LPCTSTR)ps[1]);
	rect.right	= (INT) AToBIG ((PSZ)(LPCTSTR)ps[2]);
	rect.bottom	= (INT) AToBIG ((PSZ)(LPCTSTR)ps[3]);

	if (!strBuff)
		return CRect (0,0,0,0);
	
	/*--- are numbers relative ? ---*/
	CString strZ = "+-";
	if (!ps[0].IsEmpty() && strZ.Find(ps[0][0])!=-1) rect.left	 = rectLast.left	+ (ps[0][0]=='+' ? rect.left   :	-rect.left  );
	if (!ps[1].IsEmpty() && strZ.Find(ps[1][0])!=-1) rect.top	 = rectLast.top	+ (ps[1][0]=='+' ? rect.top	 :	-rect.top	);
	if (!ps[2].IsEmpty() && strZ.Find(ps[2][0])!=-1) rect.right	 = rectLast.right	+ (ps[2][0]=='+' ? rect.right  :	-rect.right );
	if (!ps[3].IsEmpty() && strZ.Find(ps[3][0])!=-1) rect.bottom = rectLast.bottom+ (ps[3][0]=='+' ? rect.bottom :	-rect.bottom);

	/*--- left blank ---*/
	if (ps[0].IsEmpty()) rect.left	= rectLast.left;
	if (ps[1].IsEmpty()) rect.top	 	= rectLast.top;
	if (ps[2].IsEmpty()) rect.right	= rectLast.right;
	if (ps[3].IsEmpty()) rect.bottom = rectLast.bottom;

	rectLast = rect;
	rect.right += rect.left; // actually sizes
	rect.bottom += rect.top;
	return rect;
	}


static CRect CvtPosition (CString strBuff)
	{
	CString ps[10];
	strBuff.MakeLower ();
	CSVSplit (strBuff, ps);

	if (strBuff.IsEmpty ())
		return CRect (0,0,0,0);

	INT iL = (UINT) AToBIG ((PSZ)(LPCTSTR)ps[0]);
	INT iT = (UINT) AToBIG ((PSZ)(LPCTSTR)ps[1]);
	INT iR = (UINT) AToBIG ((PSZ)(LPCTSTR)ps[2]); // actually XSize
	INT iB = (UINT) AToBIG ((PSZ)(LPCTSTR)ps[3]); // actually YSize

	/*--- are numbers relative ? ---*/
	CString strZ = "+-";
	iL = (!ps[0].IsEmpty() && strZ.Find(ps[0][0])!=-1 ? iL + BPOS_RELATIVE : iL);
	iT = (!ps[1].IsEmpty() && strZ.Find(ps[1][0])!=-1 ? iT + BPOS_RELATIVE : iT);
	iR = (!ps[2].IsEmpty() && strZ.Find(ps[2][0])!=-1 ? iR + BPOS_RELATIVE : iR);
	iB = (!ps[3].IsEmpty() && strZ.Find(ps[3][0])!=-1 ? iB + BPOS_RELATIVE : iB);

	for (int i=0; i<8; i++)
		{
		if (ps[i] == "left"		) iL = -BPOS_EDGE;
		if (ps[i] == "top"		) iT = -BPOS_EDGE;
		if (ps[i] == "right"		) iL =  BPOS_EDGE;
		if (ps[i] == "bottom"	) iT =  BPOS_EDGE;
		if (ps[i] == "centerx" 	) iL =  BPOS_CTR;
		if (ps[i] == "centery"	) iT =  BPOS_CTR;
		if (ps[i] == "moveleft"	) iL = -BPOS_GAP;
		if (ps[i] == "moveup"	) iT = -BPOS_GAP;
		if (ps[i] == "moveright") iL =  BPOS_GAP;
		if (ps[i] == "movedown"	) iT =  BPOS_GAP;
		}
	return CRect(iL,iT,iR,iB);
	}

static BOOL CvtBool (CString strBuff)
	{
	strBuff.MakeLower ();

	return (strBuff == "yes"  || strBuff == "y" || 
	        strBuff == "true" || strBuff == "t" || 
	        strBuff == "1");
	}

static CString &FilterString (CString &str)
	{
	str.Replace ("\\n", "\n");
	return str;
	}


static void CvtBitmapInfo (CString strBuff, CString *pstrBitmap, CSize *pClientSize)
	{
	CString ps[10];
	CSVSplit (strBuff, ps);

	if (!ps[0].IsEmpty()) *pstrBitmap 	  = CUtil::ConvertRelativePathInfo (ps[0]);
	if (!ps[1].IsEmpty()) pClientSize->cx = (INT) AToBIG ((PSZ)(LPCTSTR)ps[1]);
	if (!ps[2].IsEmpty()) pClientSize->cy = (INT) AToBIG ((PSZ)(LPCTSTR)ps[2]);
	}

void CButtonData::CvtBitmapInfo (CString strBuff, int iType)
	{
	static CPoint ptLast(0,0);
	
	CString ps[10];
	CSVSplit (strBuff, ps);

	CString *pStr;
	CPoint  *pPt;	
	switch (iType)
		{
		case 0: pStr = &m_strBmpNormal; 	pPt = &m_ptOffsetNormal; 	break;
		case 1: pStr = &m_strBmpHilight;	pPt = &m_ptOffsetHilight; 	break;
		case 2: pStr = &m_strBmpPush; 	pPt = &m_ptOffsetPush; 		break;
		}
	if (strBuff.IsEmpty())
		{
		pStr->Empty();
		return;
		}

	if (!ps[0].IsEmpty()) *pStr  = CUtil::ConvertRelativePathInfo (ps[0]);
	if (!ps[1].IsEmpty()) pPt->x = (INT) AToBIG ((PSZ)(LPCTSTR)ps[1]);
	if (!ps[2].IsEmpty()) pPt->y = (INT) AToBIG ((PSZ)(LPCTSTR)ps[2]);

	/*--- are numbers relative ? ---*/
	CString strZ = "+-";
	if (!ps[1].IsEmpty() && strZ.Find(ps[1][0])!=-1) pPt->x += ptLast.x;
	if (!ps[2].IsEmpty() && strZ.Find(ps[2][0])!=-1) pPt->y += ptLast.y;

	ptLast = *pPt;
	}


void CStringDat::CvtFontInfo (CString strBuff)
	{
	CString ps[10];
	CSVSplit (strBuff, ps);

	if (!ps[0].IsEmpty()) m_strFontName = ps[0];
	if (!ps[1].IsEmpty()) m_iFontSize   = (INT) AToBIG ((PSZ)(LPCTSTR)ps[1]);
	if (!ps[2].IsEmpty()) m_iFontWeight = (INT) AToBIG ((PSZ)(LPCTSTR)ps[2]);
	}

void CStringDat::CvtJust (CString strBuff)
	{
	strBuff.MakeLower ();

	if (strBuff == "center") m_iJustify = 0;
	if (strBuff == "left")   m_iJustify = 1;
	if (strBuff == "right")  m_iJustify = 2;
	}


void CPageData::CvtSoundInfo (CString strBuff)
	{
	CString ps[10];

	CSVSplit (strBuff, ps);
	m_strSoundStart			=	CUtil::ConvertRelativePathInfo (ps[0]);
	m_strSoundEnd	 			=	CUtil::ConvertRelativePathInfo (ps[1]);
	m_DefaultBtn.m_strSound =	CUtil::ConvertRelativePathInfo (ps[2]);
	}

static int CvtFontShadowMode (CString strBuff)
	{
	strBuff.MakeLower ();
	if (strBuff == "off" || strBuff == "no")
		return -1;
	return atoi (strBuff);
	}


static BOOL NextString (CStdioFile *pFile, CString &strName, CString &strVal)
	{
	CString strBuff;
	int	  iIdx;

	strName.Empty();
	strVal.Empty();
	while (pFile->ReadString (strBuff))
		{
		CleanStr (strBuff);
		if ((iIdx = strBuff.Find ('=')) > -1)
			{
			strName = strBuff.Left (iIdx);
			strName.TrimRight ();
			strName.MakeLower ();
			strVal  = strBuff.Mid (iIdx+1);
			strVal.TrimLeft ();
			return TRUE;
			}
		else if (!strBuff.IsEmpty())
			{
			strName = strBuff;
			strName.MakeLower ();
			return TRUE;
			}
		}
	return FALSE;
	}


CPageData* CData::NewPage (CString strBuff)
	{
	CPageData *pPage = new CPageData;
	CPageData *pDefaultPage;
	if (m_pages.Lookup ("[default]", pDefaultPage))
		*pPage = *pDefaultPage;

	pPage->m_strPage = strBuff;
	m_pages[strBuff] = pPage;

	return pPage;
	}

CButtonData* CPageData::NewButton (BOOL bLinkType)
	{
	CButtonData *pButton = new CButtonData;

	pButton->m_bLinkType = bLinkType;

	int i = m_buttons.GetSize();
	m_buttons.SetAtGrow (i, pButton);
	*pButton = m_DefaultBtn;
	return pButton;
	}


BOOL CData::AddFileType (CString strName, CString strBuff)
	{
	if (strName != "filetype")
		return FALSE;
	CString ps[10];
	CSVSplit (strBuff, ps);

	if (ps[0].IsEmpty() || ps[1].IsEmpty() || ps[2].IsEmpty())
		return FALSE;

	CFileType* pType = new CFileType;
	pType->m_strFileType	= ps[0];	 // yeah, so its redundant
	pType->m_strName		= ps[1];
	pType->m_strCmd		= CUtil::ConvertRelativePathInfo (ps[2]);

	m_types[pType->m_strFileType] = pType;
	return TRUE;
	}


CFileType* CData::HaveInstallForType (CString strBuff)
	{
	PSZ p1;

	strBuff.MakeLower ();
	char szBuff[256];
	strcpy (szBuff, (LPCTSTR)strBuff);
	if (!(p1 = strrchr (strBuff, '.')))
		return NULL;
	strcpy (szBuff, (p1+1));
	return m_types[szBuff];
	}
	
void CPageData::AddStaticMessage (CString strMsg)
	{
	CStringDat* psd = (CStringDat*)new CStringDat;
	*psd = m_sdMsg;
	psd->m_str = FilterString (strMsg);

	m_StaticMsgList.AddTail (psd);
	}

void CPageData::MessageArea (CString strMsg)
	{
	m_sdMsg.m_rect	= CvtSizeRect (strMsg);

	CString ps[10];
	CSVSplit (strMsg, ps);
	if (!ps[4].IsEmpty())
		AddStaticMessage (ps[4]);
	}


void CPageData::AddPageField (CString strName, CString strVal)
	{
	if (strName == "pageimage" 				) ::CvtBitmapInfo (strVal, &m_strBitmap, &m_clientSize);
	if (strName == "pagetitle"		 			) m_strTitle  			= strVal;
	if (strName == "pagetitlebar"		 	 	) m_bHasCaption 		= CvtBool (strVal);
	if (strName == "pagesounds"  				) CvtSoundInfo (strVal);

 	if (strName == "pagepopupmessage" 			) m_sdMsg.m_str 		= FilterString (strVal);
 	if (strName == "pagemessage" 				) AddStaticMessage (strVal);
	if (strName == "pagemessagefont" 		) m_sdMsg.CvtFontInfo (strVal);
 	if (strName == "pagemessagecolor" 	   ) m_sdMsg.m_clrFont 	= CvtColor (strVal, GetSysColor (COLOR_WINDOWTEXT));
 	if (strName == "pagemessagearea" 		) MessageArea (strVal); // m_sdMsg.m_rect= CvtSizeRect (strVal);
 	if (strName == "pagemessagejustify"	   ) m_sdMsg.CvtJust 	 (strVal);

	if (strName == "defaultimage" 			) m_DefaultBtn.CvtBitmapInfo (strVal, 0);
	if (strName == "defaulthilightimage"   ) m_DefaultBtn.CvtBitmapInfo (strVal, 1);
	if (strName == "defaultpushimage" 	   ) m_DefaultBtn.CvtBitmapInfo (strVal, 2);

	if (strName == "defaultfont" 				) m_DefaultBtn.m_sdBtn.CvtFontInfo   (strVal);
	if (strName == "defaultfontcolor" 		) m_DefaultBtn.m_sdBtn.m_clrFont = CvtColor (strVal, GetSysColor (COLOR_WINDOWTEXT));
	if (strName == "defaultfontshadowmode" ) m_DefaultBtn.m_sdBtn.m_iFontShadow = CvtFontShadowMode (strVal);

	if (strName == "defaultedge" 				) m_DefaultBtn.m_ptEdge  	  = CvtPoint (strVal);
 	if (strName == "defaultgap" 				) m_DefaultBtn.m_ptGap   	  = CvtPoint (strVal);
	if (strName == "defaultcolor" 			) m_DefaultBtn.m_clrNormal   = CvtColor (strVal, -1);
	if (strName == "defaulthilightcolor"	) m_DefaultBtn.m_clrHilight  = CvtColor (strVal, m_DefaultBtn.m_clrNormal);
	if (strName == "defaultpushcolor" 	   ) m_DefaultBtn.m_clrPush     = CvtColor (strVal, m_DefaultBtn.m_clrNormal);

	if (strName == "default3deffect"			) m_DefaultBtn.m_b3DEffect 		= CvtBool (strVal);

 	if (strName == "defaultmessage" 			) m_DefaultBtn.m_sdMsg.m_str 		= FilterString (strVal);
 	if (strName == "defaultmessagefont"   	) m_DefaultBtn.m_sdMsg.CvtFontInfo (strVal);
	if (strName == "defaultmessagecolor" 	) m_DefaultBtn.m_sdMsg.m_clrFont = CvtColor (strVal, GetSysColor (COLOR_WINDOWTEXT));
 	if (strName == "defaultmessagearea" 	) m_DefaultBtn.m_sdMsg.m_rect		= CvtSizeRect (strVal);
	if (strName == "defaultmessagejustify"	) m_DefaultBtn.m_sdMsg.CvtJust 	  (strVal);
	}

void CButtonData::AddButtonField (CString strName, CString strVal)
	{
	if (strName == "pos" 		  		) m_rect		  	 				= CvtPosition (strVal);
	if (strName == "cmd" 		  		) m_strCmd	  	 				= strVal;
	if (strName == "sound"  	  		) m_strSound	 				= CUtil::ConvertRelativePathInfo (strVal);
	if (strName == "edge"				) m_ptEdge 						= CvtPoint (strVal);
	if (strName == "gap"					) m_ptGap  						= CvtPoint (strVal);
	if (strName == "image" 				) CvtBitmapInfo (strVal, 0);
	if (strName == "hilightimage" 	) CvtBitmapInfo (strVal, 1);
	if (strName == "pushimage" 		) CvtBitmapInfo (strVal, 2);

	if (strName == "color"				) m_clrNormal   				= CvtColor (strVal, -1);
	if (strName == "hilightcolor"		) m_clrHilight  				= CvtColor (strVal, m_clrNormal);
	if (strName == "pushcolor"		   ) m_clrPush						= CvtColor (strVal, m_clrNormal);

	if (strName == "text" 				) m_sdBtn.m_str 				= FilterString (strVal);
	if (strName == "button" 			) m_sdBtn.m_str 				= FilterString (strVal);
	if (strName == "font"   			) m_sdBtn.CvtFontInfo (strVal);
	if (strName == "fontcolor"  		) m_sdBtn.m_clrFont 			= CvtColor (strVal, GetSysColor (COLOR_BTNTEXT));
	if (strName == "fontshadowmode" 	) m_sdBtn.m_iFontShadow		= CvtFontShadowMode (strVal);

	if (strName == "3deffect"			) m_b3DEffect					= CvtBool (strVal);

	if (strName == "message"  	  		) m_sdMsg.m_str       		= FilterString (strVal);
	if (strName == "msg"  	  			) m_sdMsg.m_str       		= FilterString (strVal);
	if (strName == "messagefont"    	) m_sdMsg.CvtFontInfo	(strVal);
	if (strName == "messagecolor"		) m_sdMsg.m_clrFont			= CvtColor (strVal, GetSysColor (COLOR_WINDOWTEXT));
 	if (strName == "messagearea" 		) m_sdMsg.m_rect 				= CvtSizeRect (strVal);
 	if (strName == "messagejustify"	) m_sdMsg.CvtJust 		(strVal);
	}


BOOL CData::LoadCfgData (CStdioFile *pFile)
	{
	CPageData*   pPage   = NULL;
	CButtonData* pButton = NULL;
	CString		 strName, strVal;

	while (NextString (pFile, strName, strVal))
		{
		if (AddFileType (strName, strVal))
			continue;

		if (IsSection (strName))
			{
			pPage   = NewPage (strName);
			pButton = NULL;
			continue;
			}
		if (!pPage)
			continue;

		pPage->AddPageField (strName, strVal);

		if (strName == "button:" || strName == "button")
			pButton = pPage->NewButton ();

		if (strName == "link:" || strName == "link")
			pButton = pPage->NewButton (TRUE);


		if (!pButton)
			continue;

		pButton->AddButtonField (strName, strVal);
		}
	return TRUE;
	}


BOOL CData::LoadCfg (CString &strFile)
	{
	CStdioFile cFile;
	if (!cFile.Open (strFile, CFile::modeRead | CFile::shareDenyNone))
		{
		AfxMessageBox ((CString)"Unable to open file: " + strFile);
		return FALSE;
		}
	strFile = cFile.GetFilePath(); // flesh out full path

	CUtil::SetFileName (cFile.GetFilePath(), 1);
	LoadCfgData (&cFile);
	cFile.Close ();

	/*--- do a consistancy check ---*/
	POSITION pos = m_pages.GetStartPosition ();
	while (pos)
		{
		CPageData *pPage;
		CString   strKey;
		m_pages.GetNextAssoc (pos, strKey, pPage);
		if (pPage->m_strBitmap.IsEmpty()  && pPage->m_strPage != "[default]")
			{
			if (pPage->m_clientSize.cx == 0 && pPage->m_clientSize.cy == 0)
				{
				AfxMessageBox (pPage->m_strPage + " has no background Image or size");
				return FALSE;
				}
			}
//		if (!pPage->m_buttons.GetSize() && pPage->m_strPage != "[default]")
//			{
//			AfxMessageBox (pPage->m_strPage + " has no buttons");
//			return FALSE;
//			}
		for (int i=0; i< pPage->m_buttons.GetSize (); i++)
			pPage->m_buttons[i]->FillInButtonDefaults ();
		}
	return TRUE;
	}


CPageData* CData::FindPage(CString strPage)
	{
	CPageData *pPage;
	strPage.MakeLower ();
	if (!m_pages.Lookup (strPage, pPage))
		return NULL;
	return pPage;
	}

CPageData::CPageData ()
	{
	m_bHasCaption = FALSE;

	m_strPage.Empty();
	m_strTitle.Empty();
	m_strBitmap.Empty();
	m_strSoundStart.Empty();
	m_strSoundEnd.Empty();
	m_clientSize = CSize (0, 0);
	}

CPageData::~CPageData ()
	{
	m_StaticMsgList.RemoveAll ();
	}


CPageData &CPageData::operator= (CPageData &pgSrc)
	{
	m_bHasCaption 	= pgSrc.m_bHasCaption  ;

	m_strPage 		= pgSrc.m_strPage 	  ;
	m_strTitle 		= pgSrc.m_strTitle	  ;
	m_strBitmap 	= pgSrc.m_strBitmap	  ;
	m_DefaultBtn   = pgSrc.m_DefaultBtn	  ;
	m_strSoundStart= pgSrc.m_strSoundStart;
	m_strSoundEnd	= pgSrc.m_strSoundEnd  ;	 
	m_sdMsg 			= pgSrc.m_sdMsg;
	m_clientSize   = pgSrc.m_clientSize;

	return *this;
	}

BOOL CButtonData::FillInButtonDefaults ()
	{
	if (m_rect == CRect(0,0,0,0))
		m_rect = CvtPosition ("movedown");
	if (m_clrNormal == -1)
		m_clrHilight = GetSysColor (COLOR_BTNFACE);
	if (m_clrHilight == -1)
		m_clrHilight = m_clrNormal;
	if (m_clrPush == -1)
		m_clrPush =	m_clrHilight;
	}

CButtonData::CButtonData()
	{
	m_iID					= 0;
	m_rect         	= CRect (0,0,0,0);
	m_strCmd.Empty();

	m_clrNormal	 		= GetSysColor (COLOR_BTNFACE);
	m_clrHilight 		= -1;
	m_clrPush	 		= -1;
	m_b3DEffect    	= TRUE;

	m_strBmpNormal.Empty();
	m_ptOffsetNormal	= CPoint (0,0);

	m_strBmpHilight.Empty();
	m_ptOffsetHilight	= CPoint (0,0);

	m_strBmpPush.Empty();
	m_ptOffsetPush		= CPoint (0,0);

	m_strSound.Empty();
	m_ptEdge 	 		= CPoint (15,15);
	m_ptGap 		 		= CPoint (5, 5);
	}


CButtonData &CButtonData::operator= (CButtonData &bdSrc)
	{
	m_iID			  		= bdSrc.m_iID;			 
	m_rect		  		= bdSrc.m_rect;			 
	m_strCmd		  		= bdSrc.m_strCmd;		 

	m_sdBtn 				= bdSrc.m_sdBtn;

	m_clrNormal	  		= bdSrc.m_clrNormal;
	m_clrHilight  		= bdSrc.m_clrHilight;
	m_clrPush	  		= bdSrc.m_clrPush;
	m_b3DEffect    	= bdSrc.m_b3DEffect;

	m_sdMsg 				= bdSrc.m_sdMsg;

	m_strBmpNormal	  	= bdSrc.m_strBmpNormal;
	m_ptOffsetNormal 	= bdSrc.m_ptOffsetNormal;

	m_strBmpHilight	= bdSrc.m_strBmpHilight;		
	m_ptOffsetHilight	= bdSrc.m_ptOffsetHilight;

	m_strBmpPush		= bdSrc.m_strBmpPush;		
	m_ptOffsetPush		= bdSrc.m_ptOffsetPush;		

	m_strSound    		= bdSrc.m_strSound;

	m_ptEdge		  		= bdSrc.m_ptEdge;			 
	m_ptGap		  		= bdSrc.m_ptGap;			 

	return *this;
	}


CStringDat::CStringDat()
	{
	m_str.Empty()				;
	m_strFontName	 = "Arial";
	m_iFontSize  	 = 15;
	m_iFontWeight	 = 6 ;
	m_iFontShadow	 = -1;
	m_clrFont	 	 = GetSysColor (COLOR_BTNTEXT);
//	m_clrFontShadow = GetSysColor (COLOR_3DSHADOW);
	m_iJustify	 	 =	0; // center

	m_rect			 = CRect (0,0,0,0);
	}

CStringDat &CStringDat::operator= (CStringDat &sdSrc)
	{
	m_str				 =	sdSrc.m_str				; 
	m_strFontName	 =	sdSrc.m_strFontName	; 
	m_iFontSize		 =	sdSrc.m_iFontSize		; 
	m_iFontWeight	 =	sdSrc.m_iFontWeight	; 
	m_iFontShadow	 =	sdSrc.m_iFontShadow	; 
	m_clrFont		 =	sdSrc.m_clrFont		; 
//	m_clrFontShadow =	sdSrc.m_clrFontShadow; 
	m_iJustify	 	 =	sdSrc.m_iJustify     ;
	m_rect			 = sdSrc.m_rect			;

	return *this;
	}