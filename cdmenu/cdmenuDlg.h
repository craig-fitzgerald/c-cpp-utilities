// cdmenuDlg.h : header file
//

#if !defined(AFX_CDMENUDLG_H__9A2EA1C6_EA10_11D3_829C_0050DA0C5DE1__INCLUDED_)
#define AFX_CDMENUDLG_H__9A2EA1C6_EA10_11D3_829C_0050DA0C5DE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCdmenuDlg dialog

#define TYP_FONT	  0
#define TYP_PEN	  1
#define TYP_BRUSH	  2
#define TYP_BITMAP  3

typedef CMap<CString,LPCTSTR,CGdiObject*,CGdiObject*&> CObjectMap;

class CCdmenuDlg : public CDialog
	{
protected:
	CData			m_data;
	HICON 		m_hIcon;
	CDib*			m_pCurrBitmap;
//	CStatic		m_splashCtl;
	CPageData*	m_pCurrPage;
	CObjectMap	m_objects;
//	CString		m_strCurrMsg;
//	CRect			m_rectCurrMsg;
	CPalette*	m_pPal;
	int		  	m_iMouseOverID;
	CStringDat  m_sdCurr;

public:
	CCdmenuDlg(CWnd* pParent = NULL);	// standard constructor

	BOOL DisplayPage (CString strPage);
	CRect ButtonPos (int i, CSize sizeScreen);
	void HandleButton (CButtonData* pData);
	CObject* GetGDIObject (int iType, CString strName, int iSize, int iWeight, COLORREF clr);
	int DoRealizePalette (BOOL bForeground);
	CPalette* GetPalette ();
	int ExamineColorSet ();
	void SplitCmd (CString strCmdLine, CString &strFile, CString &strParams, CString &strExt);


	void HandleMouseMovement (UINT nFlags, CPoint point, int iID);
	void SetMessage (CStringDat* psd);
	void DisplayStringDat (CDC* pDC, CStringDat* psd);
//	BOOL PaintBitmapBackground(CDC* pDC, CRect cRect);
	BOOL PaintBitmapBackground(CDC* pDC, CRect cRect, CPoint ptOffset);


	//{{AFX_DATA(CCdmenuDlg)
	enum { IDD = IDD_CDMENU_DIALOG};
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCdmenuDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCdmenuDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnQueryNewPalette();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_CDMENUDLG_H__9A2EA1C6_EA10_11D3_829C_0050DA0C5DE1__INCLUDED_)
