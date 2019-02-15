// CryptError.h: interface for the CCryptError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYPTERROR_H__DB7E9A76_9E8F_11D3_B4AC_005004D39EC7__INCLUDED_)
#define AFX_CRYPTERROR_H__DB7E9A76_9E8F_11D3_B4AC_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCryptError  
{
	CString		m_strLogFilename;		// name of log file
	CStdioFile	m_logFile;

public:
	CCryptError();
	virtual ~CCryptError();

	BOOL	Init(BOOL bLog);
	void	Error(const CString& strMess);
};

#endif // !defined(AFX_CRYPTERROR_H__DB7E9A76_9E8F_11D3_B4AC_005004D39EC7__INCLUDED_)
