// 
// RC4.C
// 
// To encrypt a small buffer, simply call Crypt
// To encrypt a stream call InitSBox and then call CryptStream as many
// times as needed.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "Rc4.h"

RC4Data* Init (PSZ pszKey)
   {
   PSZ  psz = pszKey;
   BYTE j, tmp, k[256];
   int  i;
   
   RC4Data* pData = malloc (sizeof (RC4Data));

   for (pData->bI=pData->bJ=i=0; i<256; i++)
      {
      pData->bSBox[i] =i;
      if (!*psz) psz = pszKey;
      k[i] = *psz++;
      }
   for (j=i=0; i<256; i++)
      {
      j   = (j + pData->bSBox[i] + k[i]);
      tmp = pData->bSBox[i], pData->bSBox[i] = pData->bSBox[j], pData->bSBox[j] = tmp;
      }
   return pData;
   }


RC4Data* Term (RC4Data* pData)
   {
   if (pData)
      free (pData);
   return NULL;
   }


PSZ CryptStream (RC4Data* pData, PSZ pszOut, PSZ pszIn, int iSrcLen)
   {
   int  i;
   BYTE tmp, t;

   for (i=0; i< iSrcLen; i++)
      {
      pData->bI += 1;
      pData->bJ += pData->bSBox[pData->bI];
      tmp = pData->bSBox[pData->bI], pData->bSBox[pData->bI] = pData->bSBox[pData->bJ], pData->bSBox[pData->bJ] = tmp;
      t   = pData->bSBox[pData->bI] + pData->bSBox[pData->bJ];
      *pszOut++ = *pszIn++ ^ pData->bSBox[t];
      }
   return pszOut;
   }


PSZ Crypt (PSZ pszOut, PSZ pszIn, int iSrcLen, PSZ pszKey)
   {
   RC4Data* pData = Init (pszKey);
   return CryptStream (pData, pszOut, pszIn, iSrcLen);
   pData = Term (pData);
   }


// output buffer is twice as long as input, output buffer is null terminated
PSZ AsciiArmor (PSZ pszOut, PSZ pszIn, int iLen)
   {
   int  i;
	char szTmp[4];

   *pszOut = '\0';

   for (i=0; i<iLen; i++)
      {
		sprintf (szTmp, "%2.2x", (UINT)(UCHAR)pszIn[i]);
		strcat (pszOut, szTmp);
      }
   return pszOut;
   }


static BYTE _HexVal (char c)
   {
   if (c >= '0' && c <= '9')
      return c - '0';
   c = tolower (c);
   if (c >= 'a' && c <= 'f')
      return c - 'a' + 10;
   return 0;
   }

// output is half the size of the input, output buffer is not null terminated
PSZ UnAsciiArmor (PSZ pszOut, PSZ pszIn)
   {
   int i;
   int iLen = strlen (pszIn);
   for (i=0; i<iLen/2; i++)
      pszOut[i] = _HexVal (pszIn[i*2]) * 16 + _HexVal (pszIn[i*2+1]);
   return pszOut;
   }


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

Test (PSZ pszKey, PSZ pszString)
   {
   char szOut[256];
   char szAsciiOut[256];
   int iStrLen = strlen (pszString);

   // ENCRYPT
   Crypt (szOut, pszString, iStrLen, pszKey); // szOut is binary & same length as source
   AsciiArmor (szAsciiOut, szOut, iStrLen);        // szAsciiOut is ascii armored version of encryption data
   printf ("String:[%s]  encrypted with Key:[%s]  Generates:[%s]\n", pszString, pszKey, szAsciiOut);

   // DECRYPT
   UnAsciiArmor (pszString, szAsciiOut);      // make encrypted binary data the new input
   *szOut = '\0';                             // clear the output buffer
   Crypt (szOut, pszString, iStrLen, pszKey); // crypt again
   szOut[iStrLen] = '\0';                     // null terminte the output (remember the api thinks the output is binary)
   printf ("String:[%s]  encrypted with Key:[%s]  Generates:[%s]\n", szAsciiOut, pszKey, szOut);

   }

int main (int argc, char *argv[])
   {
   if (argc != 3)
      return printf ("Usage: RC4 key  string\n");
   Test (argv[1], argv[2]);
   return printf ("Done.\n");
   }


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
