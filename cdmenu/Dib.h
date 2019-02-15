// Dib.h: interface for the CDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIB_H__7911A603_EE15_11D3_91D9_85B8253EA626__INCLUDED_)
#define AFX_DIB_H__7911A603_EE15_11D3_91D9_85B8253EA626__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDib : public CBitmap  
	{
public:
	BITMAP	m_bm;			// stored for speed
	CPalette m_pal;		// palette
	HDRAWDIB m_hdd;		// if using DrawDib

	CDib();
	virtual ~CDib();

	BOOL DeleteObject ();
	BOOL Load (CString strBitmapFile);
	BOOL Attach (HGDIOBJ hbm);

	int  RealizePalette (CDC &dc, BOOL bForeground);

	BOOL FillBmp (CDC &dc, CRect &cRect, CPoint ptBmpOffset, BOOL bForeground);
	BOOL FillBmp (CDC &dc, CRect &cRect, BOOL bForeground);
	BOOL DrawBmp (CDC &dc, CRect &cRect, CPoint &ptOffset, BOOL bForeground);

	BOOL CreatePalette ();
	BOOL AdjustPalette(COLORREF* clrReplacements, int iReplacementColors);
	BOOL CreatePaletteWithAdjust(CPalette& pal, COLORREF* clrReplacements, int iReplacementColors);
	CPalette* GetPalette ();

	int GetColorTable (RGBQUAD* colorTab, int iColors);
	CSize GetBitmapSize();

	};

#endif // !defined(AFX_DIB_H__7911A603_EE15_11D3_91D9_85B8253EA626__INCLUDED_)
