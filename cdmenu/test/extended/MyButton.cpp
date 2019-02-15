// MyButton.cpp : implementation file
//

#include "stdafx.h"
#include "cdmenu.h"
#include "Data.h"
#include "Dib.h"
#include "CdmenuDlg.h"
#include "MyButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyButton

BEGIN_MESSAGE_MAP(CMyButton, CButton)
	//{{AFX_MSG_MAP(CMyButton)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CMyButton::CMyButton(CButtonData *pData, CCdmenuDlg *pDlg)
	{
	m_pData = pData;

	CStringDat sd = pData->m_sdBtn;

	m_pFont       		= (CFont*) pDlg->GetGDIObject (TYP_FONT, sd.m_strFontName, sd.m_iFontSize, sd.m_iFontWeight, 0);
//	m_pPenBorder  		= (CPen*)  pDlg->GetGDIObject (TYP_PEN,  "", 1, 1, RGB(0,0,0));
//	m_pPenShadow  		= (CPen*)  pDlg->GetGDIObject (TYP_PEN,  "", 1, 1, pData->m_clrShadow);
//	m_pPenHilight 		= (CPen*)  pDlg->GetGDIObject (TYP_PEN,  "", 1, 1, pData->m_clrHilight);
//	m_pBrush      		= (CBrush*)pDlg->GetGDIObject (TYP_BRUSH,"", 1, 1, pData->m_clrButton);

//	m_clrNormal		= pData->m_clrButton	;
//	m_clrHilight	= pData->m_clrHilight; // just a test for now
//	m_clrPush		= pData->m_clrShadow	; // just a test for now

	m_pPal            = pDlg->GetPalette();
	m_bHilight		 	= FALSE;

//	m_pPenFontShadow = NULL;
//	if (pData->m_sdBtn.m_iFontShadow > 0)
//		m_pPenFontShadow  = (CPen*)  pDlg->GetGDIObject (TYP_PEN,  "", 1, 1, sd.m_clrFontShadow);

	m_pBmpNormal = NULL;
	if (!pData->m_strBmpNormal.IsEmpty())
		m_pBmpNormal = (CDib*)pDlg->GetGDIObject (TYP_BITMAP, pData->m_strBmpNormal, 1, 1, 0);

	m_pBmpHilight = NULL;
	if (!pData->m_strBmpHilight.IsEmpty())
		m_pBmpHilight = (CDib*)pDlg->GetGDIObject (TYP_BITMAP, pData->m_strBmpHilight, 1, 1, 0);

	m_pBmpPush = NULL;
	if (!pData->m_strBmpPush.IsEmpty())
		m_pBmpPush = (CDib*)pDlg->GetGDIObject (TYP_BITMAP, pData->m_strBmpPush, 1, 1, 0);
	}

CMyButton::~CMyButton()
	{
	}

/////////////////////////////////////////////////////////////////////////////
// CMyButton message handlers

//	iState:  0=Normal, 1=Hilight, 2=Push
//	iEdge:  	1=Light Edge, 2=DarkEdge, 3 = border (always black)
COLORREF CMyButton::GetColor (int iState, int iEdge)
	{
	COLORREF clr = (iState==0? m_pData->m_clrNormal : (iState==1 ? m_pData->m_clrHilight : m_pData->m_clrPush));

	if (iEdge == 1) // lighten up
		return RGB (128 + GetRValue (clr)/2, 128 + GetGValue (clr)/2, 128 + GetBValue (clr)/2);
	if (iEdge == 2) // darken
		return RGB (GetRValue (clr)/2, GetGValue (clr)/2, GetBValue (clr)/2);
	if (iEdge == 3) // black
		return RGB (0,0,0);
	return clr;
	}

CPen* CMyButton::MakePen (int iState, int iEdge)
	{
	return (CPen*) new CPen (PS_SOLID, 1, GetColor (iState, iEdge));
	}

CBrush* CMyButton::MakeBrush (int iState)
	{
	return (CBrush*) new CBrush (GetColor (iState, 0));
	}



void CMyButton::DrawLabel (CDC *pDC, CString str, CRect rect)
	{
	CRect tmpRect;

//debug
//	str.Format ("P:%d O:%d", (int)m_bPushed, (int)m_bHilight);

	tmpRect = rect;
	pDC->DrawText(str, str.GetLength(), &tmpRect, DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_CALCRECT);
	if (tmpRect.Width() > rect.Width())
		{
		tmpRect = rect;
		pDC->DrawText(str, str.GetLength(), &tmpRect, DT_WORDBREAK|DT_CENTER|DT_VCENTER|DT_CALCRECT);
		if (tmpRect.Height() < rect.Height())
			rect.top += (rect.Height() - tmpRect.Height())/2;
		pDC->DrawText(str, str.GetLength(), &rect, DT_WORDBREAK|DT_CENTER|DT_VCENTER);
		}
	else
	   pDC->DrawText(str, str.GetLength(), &rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	}



void CMyButton::DrawIt (CDC* pDC, CRect& rect, int iState)
	{
//debug
//   CString strTexta;
//   GetWindowText(strTexta);
//
//   pDC->SetBkMode(TRANSPARENT);
//	CFont *pOldFonta = pDC->SelectObject (m_pFont);
//
//   pDC->SetTextColor(m_pData->m_sdBtn.m_clrFont);
//	DrawLabel (pDC, strTexta, rect);
//	pDC->SelectObject (pOldFonta);
//	return;
//debug




	CPalette* pOldPal = NULL;
	if (m_pPal)
		{
		CPalette* pOldPal = pDC->SelectPalette(m_pPal, TRUE);
		pDC->RealizePalette();
		}

	CPen*	  pLtEdge =	MakePen   (iState, 1);
	CPen*	  pDkEdge =	MakePen   (iState, 2);
	CPen*	  pBorder =	MakePen   (iState, 3);
	CBrush* pBrush	 =	MakeBrush (iState);
	CPen*   pOldPen   = (CPen*)  pDC->SelectObject (pBorder);
	CBrush* pOldBrush	= (CBrush*)pDC->SelectObject (pBrush );

	pDC->Rectangle (&rect);

	int i = !!(iState == 2); // i is 1 if button is pushed

	if (iState==2 && m_pBmpPush)
		m_pBmpPush->DrawBmp (*pDC, CRect (rect.left+i, rect.top+i, rect.right, rect.bottom), m_pData->m_ptOffsetPush, FALSE);
	else if (iState >= 1 && m_pBmpHilight)
		m_pBmpHilight->DrawBmp (*pDC, CRect (rect.left+i, rect.top+i, rect.right, rect.bottom), m_pData->m_ptOffsetHilight, FALSE);
	else if (m_pBmpNormal)
		m_pBmpNormal->DrawBmp (*pDC, CRect (rect.left+i, rect.top+i, rect.right, rect.bottom), m_pData->m_ptOffsetNormal, FALSE);


   if (iState==2 && m_pData->m_b3DEffect)
		{
		pDC->SelectObject (pDkEdge);
		pDC->MoveTo (rect.left+1,  rect.bottom-1);
		pDC->LineTo (rect.left+1,  rect.top+1);
		pDC->LineTo (rect.right-1, rect.top+1);

		pDC->SelectObject (pLtEdge);
		pDC->MoveTo (rect.left+1,  rect.bottom-1);
		pDC->LineTo (rect.right-1, rect.bottom-1);
		pDC->LineTo (rect.right-1, rect.top);

		pDC->SelectObject (pBorder);
		pDC->MoveTo (rect.left, rect.bottom-1);
		pDC->LineTo (rect.left, rect.top);
		pDC->LineTo (rect.right+1, rect.top);
		}
	else if (m_pData->m_b3DEffect)
		{
		pDC->SelectObject (pDkEdge);
		pDC->MoveTo (rect.left+1,  rect.bottom-2);
		pDC->LineTo (rect.right-2, rect.bottom-2);
		pDC->LineTo (rect.right-2, rect.top);

		pDC->SelectObject (pLtEdge);
		pDC->MoveTo (rect.left,    rect.bottom-2);
		pDC->LineTo (rect.left,    rect.top);
		pDC->LineTo (rect.right-1, rect.top);

		pDC->SelectObject (pBorder);
		pDC->MoveTo (rect.left, rect.bottom-1);
		pDC->LineTo (rect.right-1, rect.bottom-1);
		pDC->LineTo (rect.right-1, rect.top-1);
		}

   // Get the button's text.
   CString strText;
   GetWindowText(strText);

   pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject (m_pFont);

	int iOff = m_pData->m_sdBtn.m_iFontShadow;

	if (iOff > 0 && iState == 2)
		{
	   pDC->SetTextColor(GetColor (iState, 2));
		CRect rectText = rect + CPoint (iOff, iOff);
		DrawLabel (pDC, strText, rectText);
		}
	else if (iOff == -1 && iState == 2)
		{
		rect += CPoint (1, 1);
		}
   pDC->SetTextColor(m_pData->m_sdBtn.m_clrFont);
	DrawLabel (pDC, strText, rect);

	pDC->SelectObject (pOldPen);
	pDC->SelectObject (pOldBrush);
	pDC->SelectObject (pOldFont);

	if (pOldPal)
		pDC->SelectPalette(pOldPal, TRUE);

	delete pLtEdge;
	delete pDkEdge;
	delete pBorder;
	delete pBrush ;
	}

void CMyButton::DrawItem(LPDRAWITEMSTRUCT pDrawInfo) 
	{
	CDC   *pDC 	 = CDC::FromHandle (pDrawInfo->hDC);
	CRect rect 	 = pDrawInfo->rcItem;
	m_bPushed = !!(pDrawInfo->itemState & ODS_SELECTED);

	DrawIt (pDC, rect, (m_bPushed ? 2 : (m_bHilight ? 1 : 0)));
	}




void CMyButton::OnMouseMove(UINT nFlags, CPoint point) 
	{
	CButton::OnMouseMove(nFlags, point);

	CCdmenuDlg* pDlg = (CCdmenuDlg*)GetParent();
	if (!pDlg || !pDlg->m_hWnd)
		return;

	pDlg->HandleMouseMovement (nFlags, point, m_pData->m_iID);
	}



void CMyButton::HandleMouseMovement (UINT nFlags, CPoint point, BOOL bHilight)
	{
	CCdmenuDlg* pDlg = (CCdmenuDlg*)GetParent();
	if (!pDlg || !pDlg->m_hWnd)
		return;

	m_bHilight = bHilight;

	if (bHilight)
		pDlg->SetMessage (&m_pData->m_sdMsg);

//	if (m_pBmpHilight)
		{
		CDC* pDC = GetDC ();
		CRect rect;
		GetWindowRect (rect);
		ScreenToClient (rect);

		DrawIt (pDC, rect, (m_bPushed ? 2 : (m_bHilight ? 1 : 0)));
		}
	}


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

