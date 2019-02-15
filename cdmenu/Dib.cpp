// Dib.cpp: implementation of the CDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cdmenu.h"
#include "Dib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int MAXPALCOLORS = 256;
#define PALVERSION 0x300	// magic number for LOGPALETTE


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDib::CDib()
	{
	memset(&m_bm, 0, sizeof(m_bm));
	m_pal.m_hObject = NULL;
	m_hdd = NULL;
	}

CDib::~CDib()
	{
	DeleteObject();
	}

BOOL CDib::DeleteObject()
	{
	m_pal.DeleteObject();
	memset(&m_bm, 0, sizeof(m_bm));
	return CBitmap::DeleteObject();
	}

BOOL CDib::Load (CString strBitmapFile)
	{
	return Attach(::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0,
				LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
	}

BOOL CDib::Attach(HGDIOBJ hbm)
	{
	if (CBitmap::Attach(hbm)) 
		{
		if (!GetBitmap(&m_bm))			// load BITMAP for speed
			return FALSE;
		if (m_pal.m_hObject)
			m_pal.DeleteObject();			// in case one is already there
		return CreatePalette();				// create palette
//return TRUE;
		}
	return FALSE;	
	}

CSize CDib::GetBitmapSize()
	{
	GetBitmap(&m_bm);
	return CSize(m_bm.bmWidth, m_bm.bmHeight);
	}

int CDib::RealizePalette (CDC &dc, BOOL bForeground)
	{
	CPalette* pPal 	= &m_pal; //GetPalette();		// use default
	CPalette* pOldPal = dc.SelectPalette(pPal, !bForeground);
	int iNewColors = dc.RealizePalette();
	if (pOldPal)
		dc.SelectPalette(pOldPal, TRUE);
	return iNewColors;
	}



BOOL CDib::DrawBmp (CDC &dc, CRect &cRect, CPoint &ptOffset, BOOL bForeground)
	{
	if (!m_hObject)
		return FALSE;

	CPalette* pPal 	= &m_pal; //GetPalette();		// use default
	CPalette* pOldPal = dc.SelectPalette(pPal, !bForeground);
	dc.RealizePalette();

	CDC srcDC;
	srcDC.CreateCompatibleDC(&dc);
   HBITMAP hOldBitmap = (HBITMAP)srcDC.SelectObject((HBITMAP)m_hObject /*(CBitmap*)this*/);

   dc.BitBlt(cRect.left, cRect.top, cRect.Width(), cRect.Height(), &srcDC,
   			 ptOffset.x, ptOffset.y, SRCCOPY);
   
   srcDC.SelectObject(hOldBitmap);

	if (pOldPal)
		dc.SelectPalette(pOldPal, TRUE);

	return TRUE;
	}


BOOL CDib::FillBmp (CDC &dc, CRect &cRect, BOOL bForeground)
	{
	return FillBmp (dc, cRect, CPoint(0,0), bForeground);

//	if (!m_hObject)
//		return FALSE;
//
//	CPalette* pPal 	= &m_pal; //GetPalette();		// use default
//	CPalette* pOldPal = dc.SelectPalette(pPal, !bForeground);
//	dc.RealizePalette();
//
//	CDC srcDC;
//	srcDC.CreateCompatibleDC(NULL);
//   CBitmap* pOldBitmap = srcDC.SelectObject((CBitmap*)this);
//
//	CSize siz = GetBitmapSize();
//	for (int y=0; y<cRect.bottom; y += siz.cy)
//		for (int x=0; x<cRect.right; x+= siz.cx)
//		   dc.BitBlt(x, y, siz.cx, siz.cy, &srcDC, 0, 0, SRCCOPY);
//
//   srcDC.SelectObject(pOldBitmap);
//
//	if (pOldPal)
//		dc.SelectPalette(pOldPal, TRUE);
//
//	return TRUE;
	}


BOOL CDib::FillBmp (CDC &dc, CRect &cRect, CPoint ptBmpOffset, BOOL bForeground)
	{
	if (!m_hObject)
		return FALSE;

	CPalette* pPal 	= &m_pal; //GetPalette();		// use default
	CPalette* pOldPal = dc.SelectPalette(pPal, !bForeground);
	dc.RealizePalette();

	CDC srcDC;
	srcDC.CreateCompatibleDC(NULL);
   CBitmap* pOldBitmap = srcDC.SelectObject((CBitmap*)this);


	CSize siz = GetBitmapSize();
	int iBmpStartX = (ptBmpOffset.x - cRect.left) % siz.cx + cRect.left;
	int iBmpStartY = (ptBmpOffset.y - cRect.top ) % siz.cy + cRect.top;

	for (int y=iBmpStartY; y<cRect.bottom; y += siz.cy)
		for (int x=iBmpStartX; x<cRect.right; x+= siz.cx)
			{
			int XPosDC  = x;
			int YPosDC  = y;
			int XPosBmp = 0;
			int YPosBmp = 0;
			if (x<0) XPosDC =	0, XPosBmp=-x;
			if (y<0) YPosDC =	0, YPosBmp=-y;

			int XSizDC  = min (siz.cx, cRect.right -  XPosDC);
			int YSizDC  = min (siz.cy, cRect.bottom - YPosDC);

		   dc.BitBlt(XPosDC,  YPosDC, 
		   			 XSizDC,  YSizDC, &srcDC, 
		   			 XPosBmp, YPosBmp, SRCCOPY);
			}
   srcDC.SelectObject(pOldBitmap);

	if (pOldPal)
		dc.SelectPalette(pOldPal, TRUE);

	return TRUE;
	}



static BOOL ColorExistsInPalette (LOGPALETTE*pLogPal, COLORREF clr, int nColors)
	{
	for (int i = 0; i < nColors; i++)
		if(pLogPal->palPalEntry[i].peRed   == GetRValue (clr) &&
			pLogPal->palPalEntry[i].peGreen == GetGValue (clr)	&&
			pLogPal->palPalEntry[i].peBlue  == GetBValue (clr))
			return TRUE;
	return FALSE;
	}


BOOL CDib::CreatePaletteWithAdjust(CPalette& pal, COLORREF* clrReplacements, int iReplacementColors)
	{
	if (pal.m_hObject)
		pal.DeleteObject();

	BOOL bRet = FALSE;
	RGBQUAD* colors = new RGBQUAD[MAXPALCOLORS];

	int iColors = GetColorTable(colors, MAXPALCOLORS);
	if (iColors > 0) 
		{
		// Allocate memory for logical palette 
		int len = sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * iColors;
		LOGPALETTE* pLogPal = (LOGPALETTE*)new char[len];
		if (!pLogPal)
			return NULL;

		// set version and number of palette entries
		pLogPal->palVersion = PALVERSION;
		pLogPal->palNumEntries = iColors;

		// copy color entries 
		for (int i = 0; i < iColors; i++) 
			{
			pLogPal->palPalEntry[i].peRed   = colors[i].rgbRed;
			pLogPal->palPalEntry[i].peGreen = colors[i].rgbGreen;
			pLogPal->palPalEntry[i].peBlue  = colors[i].rgbBlue;
			pLogPal->palPalEntry[i].peFlags = 0;
			}
		// add replacements
		for (i = 0; i < iReplacementColors; i++) 
			if (!ColorExistsInPalette (pLogPal, clrReplacements[i], iColors))
				{
				int iIdx = 245-i;
				pLogPal->palPalEntry[iIdx].peRed   = GetRValue (clrReplacements[i]);
				pLogPal->palPalEntry[iIdx].peGreen = GetGValue (clrReplacements[i]);
				pLogPal->palPalEntry[iIdx].peBlue  = GetBValue (clrReplacements[i]);
				pLogPal->palPalEntry[iIdx].peFlags = 0;
				}

		// create the palette and destroy LOGPAL
		bRet = pal.CreatePalette(pLogPal);
		delete [] (char*)pLogPal;
		} 
	else 
		{
		CWindowDC dcScreen(NULL);
		bRet = pal.CreateHalftonePalette(&dcScreen);
		}
	return bRet;
	}


BOOL CDib::CreatePalette()
	{
	return CreatePaletteWithAdjust(m_pal, NULL, 0);
	}


BOOL CDib::AdjustPalette(COLORREF* clrReplacements, int iReplacementColors)
	{
	return CreatePaletteWithAdjust(m_pal, clrReplacements, iReplacementColors);
	}


int CDib::GetColorTable(RGBQUAD* colorTab, int iColors)
	{
	CWindowDC dcScreen(NULL);
	CDC memdc;
	memdc.CreateCompatibleDC(&dcScreen);
	CBitmap* pOldBm = memdc.SelectObject(this);
	iColors = GetDIBColorTable(memdc, 0, iColors, colorTab);
	memdc.SelectObject(pOldBm);
	return iColors;
	}

CPalette* CDib::GetPalette ()
	{
	return &m_pal;
	}
