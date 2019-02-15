#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <os2.h>
#include <string.h>
#include <GnuArg.h>
#include <GnuMisc.h>
#include <GnuStr.h>
#include <GnuFile.h>
#include "arg.h"

#define ROWSIZE  16
#define LINESIZE 256

BOOL bNOOFFSETS;     // -n
BOOL bDECOFFSETS;    // -d
BOOL bNOVALUES;      // -x
BOOL bNOHIBYTES;     // -z
BOOL bNOBYTES;       // -t
BOOL bWORDGROUP;     // -w
BOOL bSKIP;          // -k
BOOL bUNDUMP;        // -u   

unsigned char aRow[17],  aOldRow[17];


void PrintAddress (FILE *fp, ULONG ulCurrentLine, BOOL bSmall)
   {
   USHORT  uHigh, uLow;

   if (bNOOFFSETS) /* -n on cmd line */
      return;
   if (bDECOFFSETS) /* -d on cmd line */
      {
      if (bSmall)
         fprintf (fp, "%5.5ld: ", ulCurrentLine << 4);
      else
         fprintf (fp, "%9.9ld: ", ulCurrentLine << 4);
      return;
      }
   uHigh = (USHORT) (ulCurrentLine >> 12);
   uLow  = (USHORT) (ulCurrentLine << 4);
   if (bSmall)
      fprintf (fp, " %4.4X: ", uLow);
   else
      fprintf (fp, "%4.4X-%4.4X: ", uHigh, uLow);
   }


void PrintHexNumbers (FILE *fp, USHORT uBytesRead, PSZ psz)
   {
   USHORT   i;

   if (bNOVALUES)
      return;
   
   for (i = 0; i < uBytesRead; i++)
      {
      fprintf (fp, "%.2X", (unsigned) psz[i]);
      if (bWORDGROUP)
         {
         if (i % 2)
            putc ((int)' ', fp);
         }
      else
         {
         putc ((int)' ', fp);
         }
      if (!((i+1) % 4))
         putc ((int)' ', fp);
      }
   for (i = uBytesRead; i < ROWSIZE; i++)
      {
      fprintf (fp, "   ");
      if (!((i+1) % 4))
         putc ((int)' ', fp);
      }
   }



void PrintText (FILE *fp, USHORT uBytesRead, PSZ psz)
   {
   USHORT   i;

   if (bNOBYTES)
      return;
   for (i = 0; i < uBytesRead; i++)
      if (psz[i] == 128+27)
         putc ('.', fp);
      else if (psz[i] < 32)
         putc ('.', fp);
      else if (bNOHIBYTES && psz[i] > 126)
         putc ('.', fp);
      else
         putc ((unsigned) psz[i], fp);
   }


void PrintSplit (FILE *fp, BOOL bSmall)
   {
   if (!bNOOFFSETS)
      {
      fprintf (fp, "-------");
      if (!bSmall)
         fprintf (fp, "----");
      }
   if (!bNOVALUES)
      {
      if (bWORDGROUP)
         fprintf (fp, "0-1--2-3---4-5--6-7---8-9--a-b---c-d--e-f-");
      else
         fprintf (fp, "0--1--2--3---4--5--6--7---8--9--a--b---c--d--e--f-");
      }
   if (!bNOBYTES)
      fprintf (fp, "------------------");
   putc ('\n', fp);
   }




USHORT Usage (void)
   {
   printf ("DUMP   hex file dump utility       v1.0  2/07/91            Info Tech Inc. cf\n\n");
   printf ("USAGE: DUMP {options} InFile [OutFile]\n\n");
   printf ("WHERE: InFile is any valid filename to dump.\n");
   printf ("       OutFile is file to dump to. Default is InFile.BIN\n");
   printf ("       {options} may be 0 or more of:\n");
   printf ("        -d   ... Print offsets on decimal (default = hex)\n");
   printf ("        -n   ... Don't print offsets at all\n");
   printf ("        -z   ... Don't print high bytes in ascii representation\n");
   printf ("        -t   ... Don't print ascii representation at right\n");
   printf ("        -w   ... Group hex values as words (default = byte grouping)\n");
   printf ("        -x   ... Don't print hex values at all\n");
   printf ("        -k   ... Skip Duplicated lines\n");
   printf ("        -h # ... Print header every # lines (0 = turn off, default = 16)\n");
   printf ("        -s # ... Start dump at #\n");
   printf ("        -e # ... End dump at #\n");
   printf ("        -l # ... Dump # bytes from start (overrides /e)\n\n");
   printf ("        -u   ... Rebuild file (built with no options!)\n");
   printf ("       numbers may be represented in 3 ways:\n");
   printf ("       123456 ... decimal (no leading 0)\n");
   printf ("       012345 ... octal   (leading 0)\n");
   printf ("       0x1234 ... hex     (leading 0x)\n\n");
   printf ("EXAMPLE:  DUMP -s 0x0100 -l 255 -w dump.exe\n");
   return 0;
   }

static USHORT hexcharval (USHORT c)
   {
   c = toupper (c);
   if (!strchr ("0123456789ABCDEF", c))
      return 0;
   return (c - '0' - 7 * (c > '9'));
   }


static USHORT hexval (PSZ psz)
   {
   if (!psz || !*psz || !psz[1])
      return 0;
   return 16 * hexcharval (psz[0]) + hexcharval (psz[1]);
   }


USHORT ParseDumpLine (unsigned char aRow[], PSZ pszLine, BOOL bShort)
   {
   USHORT i, j, k, uOffset;

   uOffset = (bShort ? 7 : 11);

   for (i=0; i<4; i++)
      for (j=0; j<4; j++)
         {
         k = uOffset + j*3 + i*13;
         if (pszLine[k] == ' ' || !pszLine[k])
            return j+i*4;
         aRow[j+i*4] = (char) hexval (pszLine + k);
         }
   return 16;
   }


ULONG GetAddress (ULONG ulAddress, PSZ pszLine, BOOL bShort)
   {
   char szTmp [64];

   strcpy (szTmp, "0x0000");
   strncpy (szTmp+2, pszLine+!!bShort, 4);
   ulAddress = strtol (szTmp, NULL, 0);

   if (bShort)
      return ulAddress;

   ulAddress <<= 16;
   strncpy (szTmp+2, pszLine + 5, 4);
   ulAddress += strtol (szTmp, NULL, 0);
   return ulAddress;
   }


void UnDumpFile (PSZ pszOutFile, PSZ pszInFile)
   {
   FILE *fpIn;
   FILE *fpOut;
   char szLine[LINESIZE];
   BOOL bShort;
   USHORT uLineLen = 16;
   ULONG  ulAddress;

   if ((fpIn = fopen (pszInFile, "rt")) == NULL)
      Error ("Unable to open input file %s\n", pszInFile);

   if ((fpOut = fopen (pszOutFile, "wb")) == NULL)
      Error ("Unable to open output file %s\n", pszOutFile);

   printf ("Building File to %s\n", pszOutFile);

   /*--- skip first line ---*/
   FilReadLine (fpIn, szLine, "-;", LINESIZE);

   while (uLineLen == 16)
      {
      FilReadLine (fpIn, szLine, "-;", LINESIZE);

      if (StrBlankLine (szLine))
         continue;

      if (*szLine == '*')
         Error ("Cannot handle SkipLines, create file with -k option");

      bShort = (*szLine == ' ');

      ulAddress = GetAddress (ulAddress, szLine, bShort);
      uLineLen = ParseDumpLine (aRow, szLine, bShort);

      if (fseek (fpOut, ulAddress, SEEK_SET))
         Error ("Cannot seek address %p", ulAddress);

      if (fwrite (aRow, 1, uLineLen, fpOut) != uLineLen)
         Error ("Cannot write data at %p", ulAddress);
      }
   fclose (fpIn);
   fclose (fpOut);
   }


void DumpFile (PSZ pszOutFile, PSZ pszInFile)
   {
   FILE *fpIn;
   FILE *fpOut;
   ULONG    ulCurrentLine,  ulStartLine,   ulSplit,
            ulFileSize,     ulEnd,         ulSkipping,
            ulStart;
   USHORT   uBytesRead,     uFileHandle;   
   BOOL     bSmall;

   ulCurrentLine = ulSkipping = 0L;

   if ((fpIn = fopen (pszInFile, "rb")) == NULL)
      Error ("Unable to open input file %s\n", pszInFile);

   if ((fpOut = fopen (pszOutFile, "wt")) == NULL)
      Error ("Unable to open output file %s\n", pszOutFile);

   printf ("Dumping File to %s\n", pszOutFile);

   uFileHandle = fileno (fpIn);
   ulFileSize  = filelength (uFileHandle);
   bSmall      = (ulFileSize < 0x0000FFFF);

   ulStart = (ArgIs ("s") ? strtol (ArgGet ("s", 0), NULL, 0) : 0UL);
   ulSplit = (ArgIs ("h") ? strtol (ArgGet ("h", 0), NULL, 0) : 16UL);
   ulEnd   = (ArgIs ("e") ? strtol (ArgGet ("e", 0), NULL, 0) : 0xFFFFFFFF);
   ulEnd   = (ArgIs ("l") ? ulStart + strtol (ArgGet ("l", 0), NULL, 0) : ulEnd);
   ulEnd   = min (ulEnd, ulFileSize);

   if (bDECOFFSETS)
      fprintf (fpOut, "File:%s  Size:%ld  Start:%ld  End:%ld\n", pszInFile, ulFileSize, ulStart, ulEnd);
   else
      fprintf (fpOut, "File:%s  Size:0x%lX  Start:0x%lX  End:0x%lX\n", pszInFile, ulFileSize, ulStart, ulEnd);

   if (ArgIs ("t") && ArgIs ("n") && ArgIs ("x") || (ulStart > ulEnd))
      Error ("Nothing to Do!\n");

   ulStart      -= ulStart % ((LONG) ROWSIZE);
   ulStartLine   = ulStart / ((LONG) ROWSIZE);

   /*--- skip to start pos in file ---*/
   for (; ulCurrentLine < ulStartLine; ulCurrentLine++)
      if (ROWSIZE != fread (aRow, 1, ROWSIZE, fpIn))
         Error ("What?");
   do
      {
      uBytesRead = fread (aRow, 1, ROWSIZE, fpIn);
      if (!memcmp (aRow, aOldRow, sizeof aRow) &&
         uBytesRead    == ROWSIZE  &&
         ulCurrentLine != ulStart  &&
         bSKIP)
         {
         if (!ulSkipping)
            fprintf (fpOut, "*");
         ulSkipping++;
         }
      else
         {
         if (ulSkipping)
            {
            fprintf (fpOut, " (%ld [0x%.4lX] Identical Lines Skipped) *\n", ulSkipping, ulSkipping);
            ulSkipping = 0L;
            }
         if (ulSplit && !(ulCurrentLine % ulSplit))
            PrintSplit (fpOut, bSmall);
         PrintAddress    (fpOut, ulCurrentLine, bSmall);
         PrintHexNumbers (fpOut, uBytesRead, aRow);
         PrintText       (fpOut, uBytesRead, aRow);
         putc ('\n', fpOut);
         }
      memcpy (aOldRow, aRow, sizeof aRow);
      }
   while (!feof (fpIn) && ++ulCurrentLine * ROWSIZE < ulEnd);
   fclose (fpIn);
   }


USHORT _cdecl main (int argc, char *argv[])
   {
   char     szInFile [256], szOutFile [256];

   putchar ('\n');
   if (ArgBuildBlk ("? ^d- ^t- ^w- ^n- ^x- ^z- h% s% e% l% ^u- ^k-"))
      return printf ("%s", ArgGetErr ());

   if (ArgFillBlk (argv))
      return printf ("%s", ArgGetErr ());

   if (argc < 2 || ArgIs ("?"))
      return Usage();

   if (!ArgIs (NULL))
      return printf ("File Name required on input\n");

   bSKIP       = ArgIs ("k");
   bNOOFFSETS  = ArgIs ("n");
   bDECOFFSETS = ArgIs ("d");
   bNOVALUES   = ArgIs ("x");
   bNOHIBYTES  = ArgIs ("z");
   bNOBYTES    = ArgIs ("t");
   bWORDGROUP  = ArgIs ("w");
   bUNDUMP     = ArgIs ("u");

   MakeFileName (szInFile,  ArgGet (NULL, 0), NULL, NULL);
   MakeFileName (szOutFile, ArgGet (NULL, 1), szInFile, ".BIN");

   if (bUNDUMP)
      UnDumpFile (szOutFile, szInFile);
   else
      DumpFile (szOutFile, szInFile);

   return 0;
   }

