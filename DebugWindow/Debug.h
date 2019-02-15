// Debug.h : main header file for the DEBUG application
//

#if !defined(AFX_DEBUG_H__7E0071E0_0B39_414F_9636_24BC475CE409__INCLUDED_)
#define AFX_DEBUG_H__7E0071E0_0B39_414F_9636_24BC475CE409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDebugApp:
// See Debug.cpp for the implementation of this class
//

class CDebugApp : public CWinApp
{
public:
	CDebugApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDebugApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDebugApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEBUG_H__7E0071E0_0B39_414F_9636_24BC475CE409__INCLUDED_)
