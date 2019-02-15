// MyCommandLineInfo.cpp: implementation of the CMyCommandLineInfo class.
//

#include "stdafx.h"
#include "MyCommandLineInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyCommandLineInfo::CMyCommandLineInfo()
	{
	/*--- default name for cfg file ---*/
	TCHAR strName[_MAX_PATH];
	GetModuleFileName(NULL, strName, _MAX_PATH);
	PCHAR pExt = strrchr(strName, '.');
	if (pExt == NULL)
		pExt = strName + strlen(strName);	// no '.', go to end of name
	strcpy(pExt, ".cfg");
	m_strCfgFile = strName;
	}

CMyCommandLineInfo::~CMyCommandLineInfo()
	{
	}

void CMyCommandLineInfo::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
	{
	if (bFlag && !strnicmp (lpszParam, "cfg=", 4))
		m_strCfgFile = (CString)(strchr (lpszParam, '=')+1);
	else if (!bFlag)
		m_strCfgFile = lpszParam;
	else
		CCommandLineInfo::ParseParam(lpszParam, bFlag, bLast);
	}
