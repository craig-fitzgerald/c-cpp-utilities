// CryptUtils.h
// Helper functions for crypt


#ifndef CRYPTUTILS_H
#define CRYPTUTILS_H



extern BOOL FileNotFound(DWORD err);
extern BOOL FileMightExist(DWORD err);

	
// Create a file name which doesn't exist yet, and with the extension
// passed as a parameter.
//
extern BOOL CreateNewFileName(CString strSrcName, CString& strNewName, CString strExt, BOOL bDisplay = TRUE);



#endif // CRYPTUTILS_H
