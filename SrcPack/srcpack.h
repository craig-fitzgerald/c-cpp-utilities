// srcpack.h : main header file for the SRCPACK application
//

#if !defined(AFX_SRCPACK_H__DB565D86_F8D1_11D3_829C_0050DA0C5DE1__INCLUDED_)
#define AFX_SRCPACK_H__DB565D86_F8D1_11D3_829C_0050DA0C5DE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSrcpackApp:
// See srcpack.cpp for the implementation of this class
//

class CSrcpackApp : public CWinApp
{
public:
	CSrcpackApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSrcpackApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSrcpackApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRCPACK_H__DB565D86_F8D1_11D3_829C_0050DA0C5DE1__INCLUDED_)
