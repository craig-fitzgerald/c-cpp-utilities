#if !defined(AFX_MYBUTTON_H__C1CF7103_EAC9_11D3_829C_0050DA0C5DE1__INCLUDED_)
#define AFX_MYBUTTON_H__C1CF7103_EAC9_11D3_829C_0050DA0C5DE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyButton window

class CMyButton : public CButton
{
// Construction
public:
	CMyButton(CButtonData *pData, CCdmenuDlg *pDlg);
	virtual ~CMyButton();

	CButtonData* m_pData;

//	COLORREF 	 m_clrNormal;
//	COLORREF 	 m_clrHilight;
//	COLORREF 	 m_clrPush;
	CDib*			 m_pBmpNormal;
	CDib*			 m_pBmpHilight;
	CDib*			 m_pBmpPush;
	CPalette*	 m_pPal;

	CPalette*	 m_pExternalPal;

	CFont*		 m_pFont;

	int			 m_bHilight;
	int			 m_bPushed;

public:
	void DrawLabel (CDC *pDC, CString str, CRect rect);
	void DrawIt (CDC* pDC, CRect& rect, int iState);

	void DrawButtonShape (CDC* pDC, CRect& rect, int iState);
	void DrawButtonText (CDC* pDC, CRect& rect, int iState);

	void HandleMouseMovement (UINT nFlags, CPoint point, BOOL bHilight);
	COLORREF GetColor (int iState, int iEdge);
	CPen* MakePen (int iState, int iEdge);
	CBrush* MakeBrush (int iState);

	//{{AFX_VIRTUAL(CMyButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMyButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBUTTON_H__C1CF7103_EAC9_11D3_829C_0050DA0C5DE1__INCLUDED_)
