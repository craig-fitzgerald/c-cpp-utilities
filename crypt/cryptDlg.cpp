// cryptDlg.cpp : implementation file
//

#include "stdafx.h"

#include <io.h>

#include "crypt.h"
#include "cryptDlg.h"
#include "CryptUtils.h"

#include "Gnudes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// ------------------------ CONSTANTS -------------------------------
#define STR_PROG_NAME				"Crypt"
#define STR_TITLE_ENCRYPT			"Encrypt"
#define STR_TITLE_DECRYPT			"Decrypt"
#define STR_LABEL_ENCRYPT			"&Encrypt"
#define STR_LABEL_DECRYPT			"&Decrypt"
#define MAX_CRYPT_BUFFER_SIZE		4096
#define STR_CHECK_DECRYPT			"And miles to go before I sleep"
#define MAX_MESSAGE_LENGTH			255
#define STR_TEMP_FILE_BASE			"cryptxyz"
#define STR_CRYPT_FILE_SUFFIX		".des"
#define STR_UNCRYPT_FILE_SUFFIX		""

#define STR_MSG_ERR_GEN_CRYPT_TMP	"Unable to generate the temporary file '%s' to crypt data"
#define STR_MSG_ERR_GEN_UNCRYPT_TMP	"Unable to generate the temporary file '%s' to decrypt data"

#define STR_MSG_ERR_OPEN_CRYPT_TMP		"Unable to open the temporary file '%s' to crypt data"
#define STR_MSG_ERR_OPEN_UNCRYPT_TMP	"Unable to open the temporary file '%s' to decrypt data"

#define STR_MSG_ERR_DEL_FILE		"Unable to delete file '%s'"
#define STR_MSG_ERR_REN_TMP_FILE	"Error while renaming the temporary file to '%s'"

#define STR_MSG_ERR_OPEN_FILE		"Error while opening the file '%s'"
#define STR_MSG_ERR_READ_FILE		"Error while reading file '%s'"
#define STR_MSG_ERR_WRITE_FILE		"Error while writing in file '%s'"
#define STR_MSG_ERR_CRYPT_FILE		"Error while crypting"
#define STR_MSG_ERR_UNCRYPT_FILE	"Error while decrypting"
#define STR_MSG_ERR_ACCESS_FILE		"Error accessing file '%s'"
#define STR_MSG_ERR_NOT_CRYPT		"This file has not been encrypted by " STR_PROG_NAME
#define STR_MSG_ERR_PASSWORD		"Incorrect password or file not encrypted with " STR_PROG_NAME

#define STR_MSG_FILE_ENCRYPTED		"File '%s' has been encrypted into file '%s'"
#define STR_MSG_FILE_UNCRYPTED		"File '%s' has been decrypted into file '%s'"

#define STR_MSG_ERR_COMPRESS		"Error while compressing file '%s'"
#define STR_MSG_ERR_DECOMPRESS		"Error while decompressing file '%s'"
#define STR_MSG_ERR_PASSWD_DIFF		"Sorry, the two passwords are not the same"
#define STR_MSG_ERR_PASSWD_EMPTY	"Sorry, the password must have at least one character"

#define STR_MSG_ABOUT_TXT			\
			"\r\nITICrypt encrypts and decrypts files using the DES algorithm.\r\n\r\n" \
			"The first time crypt is run, it creates an entry in the Windows" \
			" Explorer context menu. Then, by right-clicking on a file in Windows" \
			" Explorer, you can choose the entry ‘Send To’ then ‘ITI Encryption" \
			" Tool’ in order to encrypt the file. It creates a new file with the" \
			" same name to which" \
			" is added the extension ‘.des’. This encrypted file can be decrypted" \
			" by double-clicking on it.\r\n\r\n\r\n" \
			"Usage:\r\n" \
			"ITICrypt [/p password] [/d] files\r\n\r\n" \
			"By default, ITICrypt encrypts. To decrypt, specify the /d option.\r\n\r\n" \
			"When a password is provided on the command line, no message box is" \
			" displayed and ITICrypt creates a log file with messages about its action." \
			" This log file is named ITICryptlog.txt, and is in the directory ITICrypt" \
			" has been run from.\r\n\r\n"

#define DEST_DES_BUF_SIZE(nSrcSize)	(nSrcSize + ((nSrcSize % 8) ? (8 - nSrcSize % 8) : 0))
#define PREV_MULT_OF_8(nSrcSize)	(nSrcSize - (nSrcSize % 8))


// ------------------------ PRIVATE TYPES ---------------------------

// This header is used to check that the file is correctly decrypted, and with
// the right length
struct TCheckHeader
{
	TCHAR	m_szCheckStr[sizeof(STR_CHECK_DECRYPT)];
	DWORD	m_dwZipLen;

	TCheckHeader() : m_dwZipLen(0)	{strcpy(m_szCheckStr, STR_CHECK_DECRYPT);}
};

#define CHECK_HEADER_SIZE		DEST_DES_BUF_SIZE(sizeof(TCheckHeader))



// ------------------------ ABOUT BOX -------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// --- OnInitDialog() -----------------------------------------------
//
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_ABOUT_DESCRIPTION, STR_MSG_ABOUT_TXT);

	return TRUE;
}



// ------------------- CCryptDlg dialog -----------------------------
//
CCryptDlg::CCryptDlg(TCryptFileList& fileList, BOOL bEncrypt, CCryptError& cryptError, CWnd* pParent /*=NULL*/)
	: CDialog(CCryptDlg::IDD, pParent),
	  m_fileList(fileList),
	  m_bEncrypt(bEncrypt),
	  m_error(cryptError)
{
	//{{AFX_DATA_INIT(CCryptDlg)
	m_strPassword = _T("");
	m_strPassword2 = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCryptDlg)
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_PASSWORD2, m_strPassword2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCryptDlg, CDialog)
	//{{AFX_MSG_MAP(CCryptDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CRYPT, OnCrypt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCryptDlg message handlers

// --- OnInitDialog() -----------------------------------------------
//
BOOL CCryptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Set the appropriate label on Encrypt/Uncrypt button.
	SetDlgItemText(IDC_BTN_CRYPT, m_bEncrypt ? STR_LABEL_ENCRYPT : STR_LABEL_DECRYPT);

	// Set title of dialog. Add file to crypt if there's only one ---
	CString strTitle;
	strTitle = m_bEncrypt ? STR_TITLE_ENCRYPT : STR_TITLE_DECRYPT;
	if (m_fileList.GetCount() == 1)
		strTitle += " - " + m_fileList.GetHead();
	SetWindowText(strTitle);

/*** That was for debugging ---
	// Display list of messages to crypt in dialog box
	CString strMessage("Files to crypt: ");
	bool first = true;
	POSITION pos = m_fileList.GetHeadPosition();
	while (pos)
	{
		if (!first)
			strMessage += ", ";
		strMessage += m_fileList.GetNext(pos);
	}
	strMessage += ".";
	SetDlgItemText(IDC_LABEL_MESSAGE, strMessage);
***/

	GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();

	return FALSE;  // return TRUE  unless you set the focus to a control
}


// --- OnSysCommand() -----------------------------------------------
//
void CCryptDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


// --- OnDestroy() --------------------------------------------------
//
void CCryptDlg::OnDestroy()
{
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.


// --- OnPaint() ----------------------------------------------------
//
void CCryptDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


// --- OnQueryDragIcon() --------------------------------------------
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCryptDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



// --- DisplayFileExceptMsg() ---------------------------------------
// Displays a message box with the reason why the operation on the file failed.
// Message to display must include a '%s' format for the file name.
//
void CCryptDlg::DisplayFileExceptMsg(const PTCHAR lpszMsg, CString strFileName, CFileException& fileExcept)
{
	char lpszMesExcept[MAX_MESSAGE_LENGTH];
	fileExcept.GetErrorMessage(lpszMesExcept, MAX_MESSAGE_LENGTH);

	CString strMes;
	CString strFormat;
	strFormat.Format("%s (%s)", lpszMsg, lpszMesExcept);
	strMes.Format(strFormat, strFileName);
	m_error.Error(strMes);
}


// --- TryRemoveFile() ----------------------------------------------
// Closes the file and removes it from disk
//
void CCryptDlg::TryRemoveFile(CFile& file)
{
	CString strFileName = file.GetFilePath();
	file.Close();
	try
	{
		CFile::Remove(strFileName);
	}
	catch(CFileException* pExcept)
	{
		pExcept->Delete();
		CString strMsg;
		strMsg.Format(STR_MSG_ERR_DEL_FILE, strFileName);
		m_error.Error(strMsg);
	}
}




// --- RenameFile() -------------------------------------------------
// Rename the temporary file to the dest file. The nature of the 
// renaming depends on the operation:
// - crypting: .des is appended
// - uncrypting: if there was a .des suffix, it is removed, otherwise
//               a new file name is generated.
//
BOOL CCryptDlg::RenameFile(CFile& tempFile, CString strSrcName, CString& strDestName)
{
	CString strTempName = tempFile.GetFilePath();
	tempFile.Close();

	if (m_bEncrypt)
		return RenameFileToDes(strTempName, strSrcName, strDestName);
	else
		return RenameFileToUndes(strTempName, strSrcName, strDestName);
}


// --- RenameFileToDes() --------------------------------------------
// Renames file 'foo' to 'foo.des' or something like 'foo1.des' if
// foo.des already exists
//
BOOL CCryptDlg::RenameFileToDes(CString strTempName, CString strSrcName, CString& strDestName)
{
	if (!CreateNewFileName(strSrcName, strDestName, STR_CRYPT_FILE_SUFFIX))
		return FALSE;

	// Rename file ---
	try
	{
		CFile::Rename(strTempName, strDestName);
	}
	catch(CFileException* pFileExcept)
	{
		// error while renaming file ---
		DisplayFileExceptMsg(STR_MSG_ERR_REN_TMP_FILE, strDestName, *pFileExcept);
		pFileExcept->Delete();
		return FALSE;
	}

	return TRUE;
}


// --- RenameFileToUndes() ------------------------------------------
BOOL CCryptDlg::RenameFileToUndes(CString strTempName, CString strSrcName, CString& strDestName)
{
	// remove .des extension if it's here ---
	if (!strSrcName.Right(4).CompareNoCase(STR_CRYPT_FILE_SUFFIX))
	{
		// Remove .des extension ---
		strDestName = strSrcName.Left(strSrcName.GetLength() - 4);
	}
	else
		strDestName = strSrcName;

	CFileStatus fileStatus;
	BOOL bStatus = CFile::GetStatus(strDestName, fileStatus);
	if (bStatus || FileMightExist(GetLastError()))
	{
		strSrcName = strDestName;
		// The file already exists, find another name ---
		if (!CreateNewFileName(strSrcName, strDestName, STR_UNCRYPT_FILE_SUFFIX))
			return FALSE;
	}

	// Rename file ---
	try
	{
		CFile::Rename(strTempName, strDestName);
	}
	catch(CFileException* pFileExcept)
	{
		// error while renaming file ---
		DisplayFileExceptMsg(STR_MSG_ERR_REN_TMP_FILE, strDestName, *pFileExcept);
		pFileExcept->Delete();
		return FALSE;
	}

	return TRUE;
}


// --- CreateNewFile() ----------------------------------------------
// Creates a new file and returns its name.
// Returns true if it succeeded, false otherwise.
//
BOOL CCryptDlg::CreateTempFile(CFile& tempFile)
{
	// _mktemp can create only 27 filenames for a given base and process or thread.
	// Add a suffix to base to go around this limitation.
	static nSuffix = 0;
	nSuffix++;

	// Create a new name for the temporary file ---

	// Try to get path of temporary files directory ---
	PTCHAR lpszTempDir = getenv("TEMP");
	if (lpszTempDir == NULL)
	{
		lpszTempDir = getenv("TMP");
		if (lpszTempDir == NULL)
			lpszTempDir = "";
	}

	CString strTemplateName;
	strTemplateName.Format("%s\\%s%dXXXXXX", lpszTempDir, STR_TEMP_FILE_BASE, nSuffix);
	LPTSTR lpszBuf = strTemplateName.GetBuffer(strTemplateName.GetLength()+1);
	PTCHAR lpszNewName = _mktemp(lpszBuf);
	if (lpszNewName == NULL)
	{
		// Error generating new file name ---
		CString strMessage;
		PTCHAR lpszMsgFormat = m_bEncrypt ? STR_MSG_ERR_GEN_CRYPT_TMP : STR_MSG_ERR_GEN_UNCRYPT_TMP;
		strMessage.Format(lpszMsgFormat, lpszNewName);
		m_error.Error(strMessage);
		return FALSE;
	}
	strTemplateName.ReleaseBuffer();	// don't use lpszNewName after this instruction

	// Open file ---

	CFileException fileExcept;
	if (!tempFile.Open(strTemplateName, CFile::modeCreate | CFile::modeWrite, &fileExcept))
	{
		// Error opening file ---
		PTCHAR lpszMsgFormat = m_bEncrypt ? STR_MSG_ERR_OPEN_CRYPT_TMP : STR_MSG_ERR_OPEN_UNCRYPT_TMP;
		DisplayFileExceptMsg(lpszMsgFormat, strTemplateName, fileExcept);
		return FALSE;
	}

	return TRUE;
}


// --- AddHeader() --------------------------------------------------
// Adds a header at beginning of file to:
// - check that password for uncrypting is correct
// - store length of source file
// - store length of compressed data before being crypted
//
BOOL CCryptDlg::AddHeader(CFile& destFile, DWORD dwZipLen)
{
	TCheckHeader checkHeader;

	try
	{
		checkHeader.m_dwZipLen = dwZipLen;

		// Crypt header ---
		char destBuf[CHECK_HEADER_SIZE];
		if (!DesBuff((PVOID) destBuf, (PVOID) &checkHeader, sizeof(checkHeader), 
					(PVOID) (LPCTSTR) m_strPassword, m_strPassword.GetLength(), 
					m_bEncrypt))
		{
			m_error.Error(m_bEncrypt ? STR_MSG_ERR_CRYPT_FILE : STR_MSG_ERR_UNCRYPT_FILE);
			return FALSE;
		}

		destFile.Write((PVOID) destBuf, CHECK_HEADER_SIZE);
	}
	catch (CFileException* pError)
	{
		DisplayFileExceptMsg(STR_MSG_ERR_ACCESS_FILE, pError->m_strFileName, *pError);
		pError->Delete();
		return FALSE;
	}
	return TRUE;
}


// --- CheckHeader() ------------------------------------------------
// Checks whether the header at the beginning of the file is ok
//
CCryptDlg::TCryptChStatus CCryptDlg::CheckHeader(CFile& srcFile, DWORD& dwZipLen)
{
	char headerSrcBuf[CHECK_HEADER_SIZE], headerDestBuf[CHECK_HEADER_SIZE];
	memset(headerSrcBuf, 0, sizeof(headerSrcBuf));
	TCheckHeader* pCheckHeader;
	long nBytesRead = 0;
	try
	{
		nBytesRead = srcFile.Read(&headerSrcBuf, CHECK_HEADER_SIZE);
	}
	catch (CFileException* pFileErr)
	{
		// error while reading file ---
		DisplayFileExceptMsg(STR_MSG_ERR_READ_FILE, pFileErr->m_strFileName, *pFileErr);
		pFileErr->Delete();
		return CryptChErr;
	}

	if (nBytesRead < CHECK_HEADER_SIZE)
	{
		// The file has not been encrypted with Crypt (or is severely damaged ...) ---
		m_error.Error(STR_MSG_ERR_NOT_CRYPT);
		return CryptChErr;
	}

	if (!DesBuff((PVOID) headerDestBuf, (PVOID) headerSrcBuf, sizeof(headerSrcBuf), 
				(PVOID) (LPCTSTR) m_strPassword, m_strPassword.GetLength(), 
				m_bEncrypt))
	{
		m_error.Error(m_bEncrypt ? STR_MSG_ERR_CRYPT_FILE : STR_MSG_ERR_UNCRYPT_FILE);
		return CryptChErr;
	}

	pCheckHeader = (TCheckHeader*) headerDestBuf;
	if (strcmp(pCheckHeader->m_szCheckStr, STR_CHECK_DECRYPT))
	{
		// Uncrypted file is different from original file ---
		m_error.Error(STR_MSG_ERR_PASSWORD);
		return CryptChErrPass;
	}

	dwZipLen = pCheckHeader->m_dwZipLen;
	return CryptChOk;
}


// --- OnCrypt() ----------------------------------------------------
// Encrypts files if m_bEncrypt = TRUE, decrypts them otherwise
//
// Main flow of data:
//
//                       compress
//  Input  read         +--------> zipBufOut+
//  file  -----> bufIn -+-------------------+-> pCryptBufIn
//
//
//                crypt/decrypt
//  pCryptBufIn ----------------> cryptBufOut
//
//                uncompress
//               +----------> zipBufOut+            write   Output
//  cryptBufOut -+---------------------+-> pBufOut ------->  file
//
//
// pCryptBufIn points to bufIn if decrypting, zipBufOut if crypting.
// pBufOut points to cryptBufOut if crypting, zipBufOut if decrypting. 
//
// When crypting/decrypting, DesBuff pads buffer with unrelevant bytes until 8-byte
// boundery. As we don't want that in our stream of zipped data, some extra code is
// devoted to ensuring the data sent to DesBuff ends on an 8-byte boundery. If there
// was more data in the input buffer, this data is moved at beginning of buffer after
// crypting :
// 
//                 moved to beginning
//                ___________________
//                |                 |
//                |                 | 
//                V for DesBuff     |            nOffset
//                 --------------  ---     -->    ---
//
//                 1 2 3 4 5 6 7 8 9 0            9 0
// pCryptBufIn -> |_|_|_|_|_|_|_|_|_|_|          |_|_|_|_|_|_|_|_|_|_|
//
// Each cell is a byte.
// nOffset is the length of the remaining data. The next writing in the buffer has to be
// performed after this offset.
//
// DesBuff will add bytes at the end anyway if the total length of data to crypt is not 
// a multiple of 8. So the length of the compressed data is stored in the header of the
// output file, and when decrypted, only this length of compressed data is uncompressed.
//
// The header of the file contains a constant string, to check uncrypting is ok.
//
void CCryptDlg::OnCrypt()
{
	CWaitCursor waitCursor;

	if (m_hWnd != NULL)
		// dialog box is displayed: retrieve data from it
		UpdateData(TRUE);

	if (m_strPassword != m_strPassword2)
	{
		m_error.Error(STR_MSG_ERR_PASSWD_DIFF);
		return;
	}
	
	if (m_strPassword.IsEmpty())
	{
		m_error.Error(STR_MSG_ERR_PASSWD_EMPTY);
		return;
	}
	
	// Encrypt each file ---
	UCHAR	bufIn[MAX_CRYPT_BUFFER_SIZE];			// buffer for reading from files
	UCHAR	zipBufOut[MAX_CRYPT_BUFFER_SIZE];		// output buffer for compressor
	PUCHAR	pCryptBufIn = NULL;						// input buffer for DesBuff
	UCHAR	cryptBufOut[MAX_CRYPT_BUFFER_SIZE+9];	// DesBuff need dest buffer larger than src (+8+1)
	PUCHAR	pBufOut = NULL;							// points to buffer for writting to files
	BOOL	bEOF = TRUE;							// end of source file ?
	BOOL	bEOZ = FALSE;							// compressing/decompressing is finished ?

	POSITION pos = m_fileList.GetHeadPosition();
	while (pos)
	{
		CString strFileName = m_fileList.GetNext(pos);
		CFileException fileExcept;
		CFile srcFile;
		if (!srcFile.Open(strFileName, CFile::modeRead | CFile::typeBinary, &fileExcept))
		{
			// error while opening file ---
			DisplayFileExceptMsg(STR_MSG_ERR_OPEN_FILE, strFileName, fileExcept);
			EndDialog(IDCANCEL);
			return;
		}

		CFile tempFile;
		if (!CreateTempFile(tempFile))
		{
			// Serious error: quit encrypting --- 
			EndDialog(IDCANCEL);
			return;
		}

		DWORD dwZipLen = 0;
		TCryptChStatus status = CryptChOk;
		if (   (m_bEncrypt && !AddHeader(tempFile, 0))			 // encrypts: add header
			|| (!m_bEncrypt && ((status = CheckHeader(srcFile, dwZipLen)) != CryptChOk))) // uncrypts: check header
		{
			// error accessing file ---
			TryRemoveFile(tempFile);

			// close dialog box if error is not an invalid password ---
			if (status != CryptChErrPass)
				EndDialog(IDCANCEL);
			return;
		}

		// Encrypt/uncrypt by blocks of MAX_CRYPT_BUFFER_SIZE size ---
		z_stream zipStream;	// used by compressor/decompressor
		if (!GZipInit(zipStream, zipBufOut, sizeof(zipBufOut), strFileName))
		{
			TryRemoveFile(tempFile);
			EndDialog(IDCANCEL);
			return;
		}

		ULONG nBytesRead = 0;	// number of bytes read from src file
		ULONG nOffset = 0;		// number of unprocessed bytes at head of src buffer that are left from previous pass in loop
		do 
		{
			nBytesRead = 0;
			if (zipStream.avail_in == 0)
			{
				// If decrypting, there might be unprocessed bytes at beginning of buffer ---
				ULONG nInOffset = (m_bEncrypt ? 0 : nOffset);
				try
				{
					nBytesRead = srcFile.Read(bufIn + nInOffset, MAX_CRYPT_BUFFER_SIZE - nInOffset);
				}
				catch (CFileException* pFileErr)
				{
					// error while reading file ---
					DisplayFileExceptMsg(STR_MSG_ERR_READ_FILE, strFileName, *pFileErr);
					GZipEnd(zipStream, strFileName);
					pFileErr->Delete();
					TryRemoveFile(tempFile);
					EndDialog(IDCANCEL);
					return;
				}
				bEOF = (nBytesRead < MAX_CRYPT_BUFFER_SIZE) ? TRUE : FALSE;
				zipStream.avail_in = nBytesRead;
				zipStream.next_in = bufIn;
			}

			ULONG nBufLen = nBytesRead;
			if (m_bEncrypt && ((zipStream.avail_in > 0) || !bEOZ))
			{
				// Encrypting: compress data before crypting ---
				if (!GZipData(zipStream, /*bFinish=*/ bEOF, bEOZ, strFileName))
				{
					GZipEnd(zipStream, strFileName);
					TryRemoveFile(tempFile);
					EndDialog(IDCANCEL);
					return;
				}
				pCryptBufIn = zipBufOut;
				nBufLen = sizeof(zipBufOut) - zipStream.avail_out;
			}
			else
			{
				// Buffer to crypt is buffer read from file ---
				pCryptBufIn = bufIn;
			}

			// Crypt only a multiple of 8 bytes, or all the buffer if it's the last pass in loop ---
			ULONG nDestLen = nBufLen;
			if (!bEOZ && !bEOF)
				nDestLen = PREV_MULT_OF_8(nDestLen);
			if (nDestLen > 0)
			{
				if (!DesBuff((PVOID) cryptBufOut, (PVOID) pCryptBufIn, nDestLen, 
							(PVOID) (LPCTSTR) m_strPassword, m_strPassword.GetLength(), 
							m_bEncrypt))
				{
					GZipEnd(zipStream, strFileName);
					m_error.Error(m_bEncrypt ? STR_MSG_ERR_CRYPT_FILE : STR_MSG_ERR_UNCRYPT_FILE);
					TryRemoveFile(tempFile);
					EndDialog(IDCANCEL);
					return;
				}
				nOffset = nBufLen - nDestLen;
				if (nOffset != 0)
				{
					// There are unprocessed bytes at end of buffer, move them to head ---
					memcpy(pCryptBufIn, pCryptBufIn + nDestLen, nOffset);
				}
			}

			if (m_bEncrypt)
				nDestLen = DEST_DES_BUF_SIZE(nDestLen);

			if (!m_bEncrypt && ((nDestLen > 0) || !bEOZ))
			{
				// Decrypting: uncompress data before writing it to file ---
				if (nDestLen > 0)
				{
					// In last pass, don't uncompress trailing unrelevant bytes added by crypt ---
					zipStream.next_in = cryptBufOut;
					zipStream.avail_in = min(nDestLen, dwZipLen - zipStream.total_in);
				}
				if (!GUnzipData(zipStream, /*bFinish=*/ bEOF, bEOZ, strFileName))
				{
					GZipEnd(zipStream, strFileName);
					TryRemoveFile(tempFile);
					EndDialog(IDCANCEL);
					return;
				}

				pBufOut = zipBufOut;
				nDestLen = sizeof(zipBufOut) - zipStream.avail_out;
			}
			else
			{
				// Buffer to crypt is buffer read from file ---
				pBufOut = cryptBufOut;
			}

			if (nDestLen > 0)
			{
				try
				{
					tempFile.Write(pBufOut, nDestLen);
				}
				catch (CFileException* pFileErr)
				{
					// error while writing file ---
					GZipEnd(zipStream, strFileName);
					DisplayFileExceptMsg(STR_MSG_ERR_WRITE_FILE, tempFile.GetFilePath(), *pFileErr);
					pFileErr->Delete();
					TryRemoveFile(tempFile);
					EndDialog(IDCANCEL);
					return;
				}
				zipStream.next_out -= nDestLen;
				zipStream.avail_out += nDestLen;
			}
		} while (!bEOF || !bEOZ);

		if (!GZipEnd(zipStream, strFileName))
		{
			TryRemoveFile(tempFile);
			EndDialog(IDCANCEL);
			return;
		}

		// If Encrypting, update header of encrypted file to add length of zipped data ---
		if (m_bEncrypt)
		{
			try
			{
				tempFile.SeekToBegin();
			}
			catch (CFileException* pFileErr)
			{
				// error while writing file ---
				DisplayFileExceptMsg(STR_MSG_ERR_ACCESS_FILE, tempFile.GetFilePath(), *pFileErr);
				pFileErr->Delete();
				TryRemoveFile(tempFile);
				EndDialog(IDCANCEL);
				return;
			}
			if (!AddHeader(tempFile, zipStream.total_out))
			{
				TryRemoveFile(tempFile);
				EndDialog(IDCANCEL);
				return;
			}
		}

		// Rename dest file to srcFile.des ---
		CString strNewFileName;
		BOOL bStatus = RenameFile(tempFile, strFileName, strNewFileName);

		// Tell user about it ---
		if (bStatus)
		{
			CString strMes;
			PTCHAR lpszMsgFormat = m_bEncrypt ? STR_MSG_FILE_ENCRYPTED : STR_MSG_FILE_UNCRYPTED;
			strMes.Format(lpszMsgFormat, strFileName, strNewFileName);
			m_error.Error(strMes);
		}
	} // next file ---

	EndDialog(IDOK);
}



// --- GZipInit() ---------------------------------------------------
//
BOOL CCryptDlg::GZipInit(z_stream& zipStream, PUCHAR bufOut, ULONG sizeBufOut, const char* strFileName)
{
    zipStream.zalloc = (alloc_func)0;
    zipStream.zfree = (free_func)0;
    zipStream.opaque = (voidpf)0;
	zipStream.next_in = (Bytef*) NULL;
	zipStream.avail_in = 0;
	zipStream.next_out = (Bytef*) bufOut;
	zipStream.avail_out = sizeBufOut;
	int nStatus = Z_OK;
	
	if (m_bEncrypt)
		nStatus = deflateInit(&zipStream, Z_DEFAULT_COMPRESSION);
	else
		nStatus = inflateInit(&zipStream);

	if (nStatus != Z_OK)
	{
		CString strMsg;
		strMsg.Format((m_bEncrypt ? STR_MSG_ERR_COMPRESS : STR_MSG_ERR_DECOMPRESS), strFileName);
		m_error.Error(strMsg);
		return FALSE;
	}
	return TRUE;
}


// --- GZipEnd() ----------------------------------------------------
//
BOOL CCryptDlg::GZipEnd(z_stream& zipStream, const char* strFileName)
{
	int nStatus = Z_OK;
	if (m_bEncrypt)
		nStatus = deflateEnd(&zipStream);
	else
		nStatus = inflateEnd(&zipStream);

	if (nStatus != Z_OK)
	{
		CString strMsg;
		strMsg.Format((m_bEncrypt ? STR_MSG_ERR_COMPRESS : STR_MSG_ERR_DECOMPRESS), strFileName);
		m_error.Error(strMsg);
		return FALSE;
	}
	return TRUE;
}


// --- GZipData() ---------------------------------------------------
// Compresses a buffer of data
//
BOOL CCryptDlg::GZipData(z_stream& zipStream, BOOL bFinish, BOOL& bEOZ, const char* strFileName)
{
	int nStatus = deflate(&zipStream, (bFinish ? Z_FINISH : Z_NO_FLUSH));
	if ((nStatus != Z_OK) && (nStatus != Z_STREAM_END))
	{
		CString strMsg;
		strMsg.Format((m_bEncrypt ? STR_MSG_ERR_COMPRESS : STR_MSG_ERR_DECOMPRESS), strFileName);
		m_error.Error(strMsg);
		return FALSE;
	}
	bEOZ = (nStatus == Z_STREAM_END) ? TRUE : FALSE;
	return TRUE;
}


// --- GUnzipData() -------------------------------------------------
// Uncompresses a buffer of data.
//
BOOL CCryptDlg::GUnzipData(z_stream& zipStream, BOOL bFinish, BOOL& bEOZ, const char* strFileName)
{
	int nStatus = inflate(&zipStream, Z_NO_FLUSH /*(bFinish ? Z_FINISH : Z_NO_FLUSH)*/);
	if ((nStatus != Z_OK) && (nStatus != Z_STREAM_END))
	{
		CString strMsg;
		strMsg.Format((m_bEncrypt ? STR_MSG_ERR_COMPRESS : STR_MSG_ERR_DECOMPRESS), strFileName);
		m_error.Error(strMsg);
		return FALSE;
	}
	bEOZ = (nStatus == Z_STREAM_END) ? TRUE : FALSE;
	return TRUE;
}



