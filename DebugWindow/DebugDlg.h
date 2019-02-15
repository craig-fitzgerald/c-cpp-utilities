// DebugDlg.h : header file
//

#if !defined(AFX_DEBUGDLG_H__2DCBFBD1_CF23_40BA_8A7F_366F39A3D6FE__INCLUDED_)
#define AFX_DEBUGDLG_H__2DCBFBD1_CF23_40BA_8A7F_366F39A3D6FE__INCLUDED_
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDebugDlg dialog

class CDebugDlg : public CDialog
	{
public:

	CDebugDlg(CWnd* pParent = NULL);	// standard constructor

	int AddMessage (LPCTSTR pszMessage);
	

	//{{AFX_DATA(CDebugDlg)
	enum { IDD = IDD_DEBUG_DIALOG };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDebugDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	HICON m_hIcon;

	//{{AFX_MSG(CDebugDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEBUGDLG_H__2DCBFBD1_CF23_40BA_8A7F_366F39A3D6FE__INCLUDED_)
