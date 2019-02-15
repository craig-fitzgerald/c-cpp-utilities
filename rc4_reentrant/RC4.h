//
// Rc4.h
//

typedef struct
   {
   BYTE bI;
   BYTE bJ;
   BYTE bSBox[256];
   } RC4Data;

// Simple API 
////////////////////////////////////////////////

// Crypt (combines init(), CryptStream(), Term())
//
// parameters:
//    pszOut  - the output buffer.  It is assumed to be already adequately allocated.
//              the length of the output is the same as the length of the input
//              The output data is binary and not null terminated.  Convert the
//              buffer using AsciiArmor() if you want it to be a human readable 
//              terminated string.
//    pszIn   - Ihe input data buffer.  It is assumed to be binary data
//    iSrcLen - The length of the input data buffer.
//    pszKey  - A null terminated key string of any length
//
//    RC4 is symmetric cypher, so encrypting and decrypting are the same.
//
//
PSZ Crypt (PSZ pszOut, PSZ pszIn, int iSrcLen, PSZ pszKey);


// Super Advanced API
////////////////////////////////////////////////

// Notes:
//    RC4 is stream cypher (inlike DES which is a block cypher), so encrypting 
//    depends on the previously encrypted data as well as the data and the key.
//    So you will need to call Term and then Init again if you do not want 
//    encryption dependencies (this is similar to a running CRC or MD5 check),
//    or simply use Ctypt() which inits and terms automatically
//
RC4Data* Init (PSZ pszKey);
PSZ CryptStream (RC4Data* pData, PSZ pszOut, PSZ pszIn, int iSrcLen);
RC4Data* Term (RC4Data* pData);


// util functions
////////////////////////////////////////////////

// output buffer is twice as long as input, output buffer is null terminated
PSZ AsciiArmor (PSZ pszOut, PSZ pszIn, int iLen);

// output is half the size of the input, output buffer is not null terminated
PSZ UnAsciiArmor (PSZ pszOut, PSZ pszIn);
