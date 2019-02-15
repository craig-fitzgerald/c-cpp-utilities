// Data.h: interface for the CData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_H__742A0504_EA15_11D3_829C_0050DA0C5DE1__INCLUDED_)
#define AFX_DATA_H__742A0504_EA15_11D3_829C_0050DA0C5DE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BPOS_EDGE		 3000
#define BPOS_GAP		 4000
#define BPOS_CTR		 5000
#define BPOS_RELATIVE 10000

class CStringDat
	{
	public:

	CString	m_str;				//	the text
	CString  m_strFontName	;	//	Font name for text
	int		m_iFontSize  	;	//
	int		m_iFontWeight	;	//
	int		m_iFontShadow	;	//
	int		m_iJustify   	;	//
	COLORREF m_clrFont	 	;	//

	CRect	   m_rect			;  // location of the text


	CStringDat();
	CStringDat& operator= (CStringDat &sdSrc);

	void CvtFontInfo (CString strBuff);
	void CvtJust (CString strBuff);
	};

class CButtonData
	{
	public:

	int		m_iID;				//	Dialog ID
	CRect		m_rect;				//	Button Position
	CString	m_strCmd;			//	Button command

	CStringDat m_sdBtn;

	COLORREF m_clrNormal;		//	Normal color of buton
	COLORREF m_clrHilight;		// Button color when mouse is over it
	COLORREF m_clrPush;			// Button color when pushed
	CString  m_strBmpNormal;	//	bitmap for button
	CString  m_strBmpHilight;	//	
	CString  m_strBmpPush;		//	
	CPoint   m_ptOffsetNormal;	//	offset for button bitmap
	CPoint   m_ptOffsetHilight;//	
	CPoint   m_ptOffsetPush;	//	

	BOOL		m_b3DEffect;
	BOOL		m_bLinkType;      // only show button text, no button paint

	CStringDat m_sdMsg;			//
	CString  m_strSound; 	   //	

	CPoint   m_ptEdge;			//	border between buttons and splash screen edge
	CPoint	m_ptGap;				//	space between buttons

	CButtonData();
	CButtonData& operator= (CButtonData &bdSrc);

	void AddButtonField (CString strName, CString strVal);
	void CvtBitmapInfo (CString strBuff, int iType);
	FillInButtonDefaults ();
	};


typedef CArray<CButtonData*,CButtonData*> CButtonDataArray;

typedef CList<CStringDat*,CStringDat*> CStringDatList;


class CPageData
	{
	public:
	BOOL				m_bHasCaption;
	
	CString  		m_strPage;				// Page Identifier
	CString  		m_strTitle;				//	Window Title
	CString  		m_strBitmap;			//	Background Image
	CString  		m_strSoundStart; 		//	
	CString  		m_strSoundEnd;			//	

	CStringDat		m_sdMsg;					//
	CSize				m_clientSize;			//

	CStringDatList m_StaticMsgList;   	//


 	CButtonData m_DefaultBtn;	// Default Button attributes
	CButtonDataArray m_buttons;   // list of buttons on this page

	CPageData ();
	~CPageData ();
	CPageData &operator= (CPageData &bdSrc);
	CButtonData *NewButton (BOOL bLinkType = FALSE);
	void CopyPageAttributes (CPageData *pSrcPage);
	void SetButtonAttributes (int i);
	void AddPageField (CString strName, CString strVal);
	void CvtSoundInfo (CString strBuff);
	void AddStaticMessage (CString strMsg);
	void MessageArea (CString strMsg);
	};


typedef CMap<CString,LPCTSTR,CPageData*,CPageData*&> CPageDataMap;


class CFileType
	{
	public:
	CString m_strFileType;
	CString m_strName;
	CString m_strCmd;
	};

typedef CMap<CString,LPCTSTR,CFileType*,CFileType*&> CFileTypeMap;

class CData  
	{
	public:
	CPageDataMap m_pages;
	CFileTypeMap m_types;

	CData();
	virtual ~CData();
	CPageData* NewPage (CString strBuff);
	CPageData* FindPage (CString strPage);

	CFileType* HaveInstallForType (CString strBuff);
	BOOL AddFileType (CString strName, CString strBuff);


	BOOL LoadCfgData (CStdioFile *pFile);
	BOOL LoadCfg (CString &strFile);
	};

#endif // !defined(AFX_DATA_H__742A0504_EA15_11D3_829C_0050DA0C5DE1__INCLUDED_)
