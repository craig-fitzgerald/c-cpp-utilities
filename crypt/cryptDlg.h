// cryptDlg.h : header file
//

#if !defined(AFX_CRYPTDLG_H__A208E58C_823D_11D3_B488_005004D39EC7__INCLUDED_)
#define AFX_CRYPTDLG_H__A208E58C_823D_11D3_B488_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "zlib.h"
#include "CryptError.h"


/////////////////////////////////////////////////////////////////////////////
// CCryptDlg dialog

class CCryptDlg : public CDialog
{
// Types
	typedef enum {CryptChOk, CryptChErrPass, CryptChErr} TCryptChStatus;	// return status for CheckHeader()

// Attributes
	TCryptFileList&		m_fileList;		// list of file to encrypt
	BOOL				m_bEncrypt;		// encrypt or decrypt ?
	CCryptError&		m_error;		// message error handler


// Construction
public:
	CCryptDlg(TCryptFileList& fileList, BOOL bEncrypt, CCryptError& cryptError, CWnd* pParent = NULL);	// standard constructor

	void SetPassword(const CString& strPassword)	{m_strPassword = strPassword;}
	void Crypt()		{OnCrypt();}

// Dialog Data
	//{{AFX_DATA(CCryptDlg)
	enum { IDD = IDD_CRYPT_DIALOG };
	CString	m_strPassword;
	CString	m_strPassword2;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCryptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCryptDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCrypt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL	CreateTempFile(CFile& tempFile);
	BOOL	AddHeader(CFile& destFile, DWORD dwZipLen);
	TCryptChStatus CheckHeader(CFile& srcFile, DWORD& dwZipLen);
	void	DisplayFileExceptMsg(const PTCHAR lpszMsg, CString strFileName, CFileException& fileExcept);
	void	TryRemoveFile(CFile& file);
	BOOL	RenameFile(CFile& tempFile, CString strSrcName, CString& strDestName);
	BOOL	RenameFileToDes(CString strTempName, CString strSrcName, CString& strDestName);
	BOOL	RenameFileToUndes(CString strTempName, CString strSrcName, CString& strDestName);
	BOOL	GZipInit(z_stream& zipStream, PUCHAR bufOut, ULONG sizeBufOut, const char* strFileName);
	BOOL	GZipEnd(z_stream& zipStream, const char* strFileName);
	BOOL	GZipData(z_stream& zipStream, BOOL bFinish, BOOL& bEOZ, const char* strFileName);
	BOOL	GUnzipData(z_stream& zipStream, BOOL bFinish, BOOL& bEOZ, const char* strFileName);
};

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_CRYPTDLG_H__A208E58C_823D_11D3_B488_005004D39EC7__INCLUDED_)
