// srcpackDlg.h : header file
//

#if !defined(AFX_SRCPACKDLG_H__DB565D88_F8D1_11D3_829C_0050DA0C5DE1__INCLUDED_)
#define AFX_SRCPACKDLG_H__DB565D88_F8D1_11D3_829C_0050DA0C5DE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSrcpackDlg dialog

class CSrcpackDlg : public CDialog
{
// Construction
public:
	CSrcpackDlg(CWnd* pParent = NULL);	// standard constructor
	BOOL Encrypt (CString strInFile, CString strPWD);

// Dialog Data
	//{{AFX_DATA(CSrcpackDlg)
	enum { IDD = IDD_SRCPACK_DIALOG };
	CString	m_strZipFile;
	CString	m_strPassword;
	CString	m_strPassword2;
	CString	m_strMessage;
	CString	m_strInstallDir;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSrcpackDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSrcpackDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelectFile();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRCPACKDLG_H__DB565D88_F8D1_11D3_829C_0050DA0C5DE1__INCLUDED_)
