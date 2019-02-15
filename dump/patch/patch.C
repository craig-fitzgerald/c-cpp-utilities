/*
 *
 * dump2.c
 * Tuesday, 9/9/1997.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GnuType.h>
#include <GnuFile.h>
#include <GnuMisc.h>
#include <GnuArg.h>


ULONG ulSTART, ulEND;
PSZ   pszFILE;

void Usage (void)
   {
   }

void ApplyPatch (void)
   {
   }


ULONG WriteLine (FILE *fpIn, FILE *fpOut, ULONG ulAddr)
   {
   UINT i, uRead;
   CHAR sz[32];

   if (ulEND && ulAddr >= ulEND)
      return 0;

   uRead = fread (sz, 1, 16, fpIn);
   if (uRead < 16)
      ulEND = ulAddr + uRead;

   /*--- write address ---*/
   fprintf (fpOut, "%4.4x %4.4x: ", HIWORD (ulAddr), LOWORD (ulAddr));

   /*--- write data ---*/
   for (i=0; i<16; ulAddr++, i++)
      {
      if (ulAddr < ulSTART || ulAddr >= ulEND)
         fprintf (fpOut, "   ");
      else
         fprintf (fpOut, "%2.2x ", (USHORT)(UCHAR)sz[i]);
      }
   fprintf (fpOut, "\n");
   return uRead;
   }


void MakePatch (void)
   {
   CHAR  szOutFile [258];
   PSZ   psz;
   FILE  *fpIn, *fpOut;
   ULONG ulAddr, ulLen;

   strcpy (szOutFile, pszFILE);
   if (psz = strrchr (szOutFile, '.'))
      *psz = '\0';
   strcat (szOutFile, ".pat");;

   if (!(fpIn  = fopen (pszFILE, "rb")))
      Error ("Cannot open input file: %s", pszFILE);
   if (!(fpOut = fopen (szOutFile, "wt")))
      Error ("Cannot open output file: %s", szOutFile);

   ulAddr = ulSTART & 0xFFFFFFF0;

   if (ulAddr)
      if (fseek (fpIn, ulAddr, SEEK_SET))
         Error ("Could not seek to location: %lx", ulAddr);

   fprintf (fpOut, "FILE: %s\n", pszFILE);

   /*--- write partial line ---*/
   while (ulLen = WriteLine (fpIn, fpOut, ulAddr))
      ulAddr += ulLen;
   }


int main (int argc, char *argv[])
   {
   BOOL  bPatch;
   ULONG ulLen;
   PSZ   psz;

   ArgBuildBlk ("? *^Start% *^End% *^Length%");

   if (ArgFillBlk (argv))
      Error ("%s", ArgGetErr ());

   if (ArgIs ("?") || !ArgIs (NULL))
      Usage ();

   pszFILE = ArgGet (NULL, 0);
   ulSTART = (ArgIs ("Start")  ? atol (ArgGet ("Start" , 0)) : 0L);
   ulLen   = (ArgIs ("Length") ? atol (ArgGet ("Length", 0)) : 0L);
   ulEND   = (ArgIs ("End")    ? atol (ArgGet ("End"   , 0)) : 0xFFFFFFFF);
   bPatch  = ((psz = strrchr (pszFILE, '.')) && !strnicmp (psz, ".pat", 3));

   if (ulEND < ulSTART)
      Error ("Start[%lu] > End[%lu]", ulSTART, ulEND);
   if (ulLen)
      ulEND = ulSTART + ulLen;

   if (bPatch)
      ApplyPatch ();
   else
      MakePatch ();
   return 0;
   }

