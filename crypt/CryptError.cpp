// CryptError.cpp: implementation of the CCryptError class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CryptError.h"
#include "CryptUtils.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


// ------------------------ CONSTANTS -------------------------------
#define STR_LOG_FILENAME	"ITICryptlog"
#define STR_LOG_EXTENSION	".txt"


// ------------------------- METHODS --------------------------------
// 
CCryptError::CCryptError()
{

}

CCryptError::~CCryptError()
{

}


// --- Init() -------------------------------------------------------
// Initializes error messages handling
// If bLog = TRUE, messages are saved in a log instead of being displayed.
// 
BOOL CCryptError::Init(BOOL bLog)
{
	if (bLog)
	{
		// create log file ---
		if (!CreateNewFileName(STR_LOG_FILENAME, m_strLogFilename, STR_LOG_EXTENSION, /*bDisplay=*/ FALSE))
			return FALSE;

		CFileException fileExcept;
		if (!m_logFile.Open(m_strLogFilename, CFile::modeCreate | CFile::modeWrite, &fileExcept))
		{
			return FALSE;
		}
	}
	return TRUE;
}


// --- Error() ------------------------------------------------------
// Either display message in a message box, or write it into log file,
// depending on log mode.
//
void CCryptError::Error(const CString& strMess)
{
	if (m_strLogFilename.IsEmpty())
	{
		// display mode ---
		AfxMessageBox(strMess);
	}
	else
	{
		// log mode: write message to log file ---
		try
		{
			m_logFile.WriteString(strMess + "\n");
		}
		catch (CFileException* pFileErr)
		{
			// error while writing file ---
			pFileErr->Delete();
		}
	}
}
