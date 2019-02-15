// CryptUtils.cpp


#include "stdafx.h"
#include "CryptUtils.h"
#include "gnudir.h"


// ------------------------- CONSTANTS ------------------------------
#define STR_MSG_ERR_REN_FILE		"Problem renaming file '%s' to '%s'"



// ------------------------- FUNCTIONS ------------------------------

// --- FileNotFound() -----------------------------------------------
// Returns TRUE if the error (returned by GetLastError) is that the
// file couldn't be found.
// Returns FALSE otherwise.
//
BOOL FileNotFound(DWORD err)
{
	return ((err == ERROR_FILE_NOT_FOUND) ? TRUE : FALSE);
}


// --- FileMightExist() ---------------------------------------------
// Returns TRUE if the error code indicates that the file might exist.
// Returns FALSE otherwise.
//
BOOL FileMightExist(DWORD err)
{
	return ((err == ERROR_ACCESS_DENIED) ? TRUE : FALSE);
}


// --- CreateNewFileName() ------------------------------------------
// Create a file name which doesn't exist yet, and with the extension.
// passed as a parameter.
// If bDisplay = TRUE, then display a message box if there's an error.
//
BOOL CreateNewFileName(CString strSrcName, CString& strNewName, CString strExt, BOOL bDisplay)
{
	// Find a name which doesn't already exist ---
	int nSuffix = 0;
	strNewName = strSrcName + strExt;

	BOOL bStatus = TRUE;
	CFileStatus fileStatus;
	bStatus = CFile::GetStatus(strNewName, fileStatus);
	if (bStatus || FileMightExist(GetLastError()))
	{
		// The file exists: find another name ---

		// Break the path name into directory and file names
		TCHAR strNewNameTmp[_MAX_PATH];
		TCHAR strFileName[_MAX_PATH];
		LPTSTR lpstrSrcName = strSrcName.GetBuffer(strSrcName.GetLength()+1);
		DirSplitPath(strNewNameTmp, lpstrSrcName, DIR_DRIVE | DIR_DIR);
		DirSplitPath(strFileName, lpstrSrcName, DIR_NAME | DIR_EXT);
		strSrcName.ReleaseBuffer();

		// Get part of file name before the first dot (but not in the directory 
		// name !) so that foobar.txt is renamed foobar1.txt.
		TCHAR strFileExt[_MAX_PATH];
		LPTSTR lpstrExt = strchr(strFileName, '.');
		if (lpstrExt == NULL)
		{
			// No extension
			strcat(strNewNameTmp, strFileName);
			strFileExt[0] = '\0';
		}
		else
		{
			int len = strlen(strNewNameTmp);
			strncat(strNewNameTmp, strFileName, lpstrExt - strFileName);
			strNewNameTmp[len + lpstrExt - strFileName] = '\0';
			strcpy(strFileExt, lpstrExt);
		}
		
		do
		{
			nSuffix++;
			strNewName.Format("%s%d%s%s", strNewNameTmp, nSuffix, strFileExt, strExt);
			bStatus = CFile::GetStatus(strNewName, fileStatus);
		} while (bStatus || FileMightExist(GetLastError()));
	}
	if ((bStatus == FALSE) && (!FileNotFound(GetLastError())))
	{
		// Maybe the file already exist but there's a problem accessing it:
		// It's better not to do anything, lest we can delete an existing file.
		if (bDisplay)
		{
			CString strMsg;
			strMsg.Format(STR_MSG_ERR_REN_FILE, strSrcName, strNewName);
			AfxMessageBox(strMsg);
		}
		return FALSE;
	}
	return TRUE;
}


