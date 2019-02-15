// Util.cpp: implementation of the CUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cdmenu.h"
#include "Util.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUtil::CUtil()
	{
	}

CUtil::~CUtil()
	{
	}

//
//	 pszFullPath is fully qualified path to breeak up
//
//	 iParts = 0	- return drive only of pszFullPath
// 			 1	- return parent dir of pszFullPath
//				 2	- dir of file in pszFullPath
//
//	 pszPart is what is returned
//
CString& CUtil::GetDir (CString strFilePath, CString& strPart, int iParts)
	{
	PSZ  p;
	char szDrive[_MAX_DRIVE];
	char szDir[_MAX_DIR];
	char szName[_MAX_FNAME];
	char szExt[_MAX_EXT];

	_splitpath ((LPCTSTR)strFilePath, szDrive, szDir, szName, szExt);
	strPart = szDrive;
	if (!iParts)
		return strPart; // drive only

	if (*szDir && strchr ("\\/", szDir[strlen(szDir)-1]))
		szDir[strlen(szDir)-1] = '\0';
	strPart += szDir;
	if (iParts == 2)
		return strPart; // full dir

	PSZ pszBuff = (PSZ)(LPCTSTR)strPart;
	if ((p = strrchr (pszBuff, '\\')) || (p = strrchr (pszBuff, '/')))
		strPart = strPart.Left (p - pszBuff);
	return strPart; // parent dir
	}


//	iFile: 0	- exe filename
//			 1	- cfg filename
//
CString CUtil::GetFileName (int iFile)
	{
	CCdmenuApp* pApp = (CCdmenuApp*)AfxGetApp ();
	switch (iFile)
		{
		case 0: return pApp->m_strAppName;
		case 1: return pApp->m_strCfgName;
		}
	return CString("");
	}


//	iFile: 0	- exe filename
//			 1	- cfg filename
//
void CUtil::SetFileName (CString strFileName, int iFile)
	{
	CCdmenuApp* pApp = (CCdmenuApp*)AfxGetApp ();
	switch (iFile)
		{
		case 0: pApp->m_strAppName = strFileName;	  return; 
		case 1: pApp->m_strCfgName = strFileName;	  return; 
		}
	}





//	This fn makes relative path locations relative to the cfg
//	file rather than relative to the current directory
//
//	if config at e:\startup\cdmenu.cfg 
//
//	 a.bmp					  -> e:\startup\a.bmp
//	 images\a.bmp			  -> e:\startup\images\a.bmp
//	 \images\a.bmp			  -> <no change>
//	 c:\images\a.bmp		  -> <no change>
//	 \\joe\\c\images\a.bmp -> <no change>
//
//
CString& CUtil::ConvertRelativePathInfo (CString &strPath)
	{
	if (strPath.IsEmpty())
		return strPath;
	int iOrgLen = strPath.GetLength();
	if (strPath[0] == '\\' || strPath[0] == '/')
		return strPath; // a non local path
	if (iOrgLen > 1 && strPath[1] == ':')
		return strPath; // a fully spec'd path

	CString strDir;
	CFileStatus status;
	CString strNewPath = GetDir (CUtil::GetFileName(1), strDir, 2) + '\\' + strPath;

	//if(CFile::GetStatus(strNewPath, status))
   		strPath = strNewPath;

	return strPath;
	}


