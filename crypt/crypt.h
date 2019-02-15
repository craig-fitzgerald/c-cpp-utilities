// crypt.h : main header file for the CRYPT application
//

#if !defined(AFX_CRYPT_H__A208E58A_823D_11D3_B488_005004D39EC7__INCLUDED_)
#define AFX_CRYPT_H__A208E58A_823D_11D3_B488_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <afxtempl.h>


/////////////////////////////////////////////////////////////////////////////
// CCryptApp:
// See crypt.cpp for the implementation of this class
//

// List of files to crypt ---
typedef	CList<CString, CString> TCryptFileList;



class CCryptApp : public CWinApp
{
	TCryptFileList	m_fileList;

public:
	CCryptApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCryptApp)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCryptApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void RegisterExtension(LPCTSTR szExePath);
	void CreateSendToShortcut(LPTSTR szExePath);
	void DisplayHelp();
};



class CCryptCmdLineInfo : public CCommandLineInfo
{
	TCryptFileList&	m_fileList;
	BOOL			m_bEncrypt;
	CString			m_strPassword;
	BOOL			m_bHelpWanted;
	BOOL			m_bRegister;

	virtual void ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast);

public:
	CCryptCmdLineInfo(TCryptFileList& fileList)
		:	m_fileList(fileList),
			m_bEncrypt(TRUE),
			m_bHelpWanted(FALSE),
			m_bRegister(FALSE)
		{}

	BOOL	IsFlagEncrypt()		{return m_bEncrypt;}
	BOOL	IsHelpWanted()		{return m_bHelpWanted;}
	CString	GetPassword()		{return m_strPassword;}
	BOOL	WantRegister()		{return m_bRegister;}
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRYPT_H__A208E58A_823D_11D3_B488_005004D39EC7__INCLUDED_)
