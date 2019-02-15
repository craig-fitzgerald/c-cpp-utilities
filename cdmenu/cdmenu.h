// cdmenu.h : main header file for the CDMENU application
//

#if !defined(AFX_CDMENU_H__9A2EA1C4_EA10_11D3_829C_0050DA0C5DE1__INCLUDED_)
#define AFX_CDMENU_H__9A2EA1C4_EA10_11D3_829C_0050DA0C5DE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCdmenuApp:

class CCdmenuApp : public CWinApp
{
public:
	CCdmenuApp();

	CString m_strAppName;
	CString m_strCfgName;

	//{{AFX_VIRTUAL(CCdmenuApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCdmenuApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDMENU_H__9A2EA1C4_EA10_11D3_829C_0050DA0C5DE1__INCLUDED_)
