// MyCommandLineInfo.h: interface for the CMyCommandLineInfo class.
//

#if !defined(AFX_MYCOMMANDLINEINFO_H__D4843DE5_2318_11D3_829B_00105A21C878__INCLUDED_)
#define AFX_MYCOMMANDLINEINFO_H__D4843DE5_2318_11D3_829B_00105A21C878__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyCommandLineInfo : public CCommandLineInfo  
	{
	public:
	CMyCommandLineInfo();
	virtual ~CMyCommandLineInfo();
	void ParseParam ( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast );

	CString	m_strCfgFile;
	};

#endif // !defined(AFX_MYCOMMANDLINEINFO_H__D4843DE5_2318_11D3_829B_00105A21C878__INCLUDED_)
