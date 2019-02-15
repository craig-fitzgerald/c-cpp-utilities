// Util.h: interface for the CUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTIL_H__D2BBE03D_F4F1_11D3_829C_0050DA0C5DE1__INCLUDED_)
#define AFX_UTIL_H__D2BBE03D_F4F1_11D3_829C_0050DA0C5DE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUtil  
	{
public:
	CUtil();
	virtual ~CUtil();

	static CString& GetDir (CString strFilePath, CString& strPart, int iParts);
	static CString GetFileName (int iFile);
	static void SetFileName (CString strFileName, int iFile);
	static CString& ConvertRelativePathInfo (CString &strPath);
	};

#endif // !defined(AFX_UTIL_H__D2BBE03D_F4F1_11D3_829C_0050DA0C5DE1__INCLUDED_)
