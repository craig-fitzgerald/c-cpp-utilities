// srcunpakDlg.h : header file
//

#if !defined(AFX_SRCUNPAKDLG_H__DB565D99_F8D1_11D3_829C_0050DA0C5DE1__INCLUDED_)
#define AFX_SRCUNPAKDLG_H__DB565D99_F8D1_11D3_829C_0050DA0C5DE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSrcunpakDlg dialog

class CSrcunpakDlg : public CDialog
{
// Construction
public:
	CString m_strInFile;
	CString m_strTest;
//	CString m_strInstallDir;
	CString m_strZipFile;
	BYTE m_bFingerprintBuff[256];

	CSrcunpakDlg(CWnd* pParent = NULL);	// standard constructor

	BOOL MakeInputFileName ();
	BOOL ReadFileHeader ();
	BOOL CheckPassword(CString strPassword);
	BOOL MakeDir (CString strPath);
	BOOL CheckDestDir ();
	BOOL DecryptFile (CString strPWD);
	BOOL UnpackFile ();



// Dialog Data
	//{{AFX_DATA(CSrcunpakDlg)
	enum { IDD = IDD_SRCUNPAK_DIALOG };
	CString	m_strInstallDir;
	CString	m_strMessage;
	CString	m_strPassword;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSrcunpakDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSrcunpakDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRCUNPAKDLG_H__DB565D99_F8D1_11D3_829C_0050DA0C5DE1__INCLUDED_)
