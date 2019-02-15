/*
 *
 * genpwd.c
 * Wednesday, 8/23/1995.
 *
 */

#include <os2.h>
#include <io.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GnuFile.h>
#include <GnuMisc.h>
#include <GnuArg.h>

char szWORD [256];


void Usage (void)
   {
   printf ("GenPWD Password Generator v1.0                                %s\n\n", __DATE__);
   printf ("USAGE: GenPWD [options] NumPasswords\n\n");
   printf ("WHERE: NumPasswords .... Is the number of passwords to generate.\n");
   printf ("       [options1] are 0 or more of:\n");
   printf ("         /NumWords=# ..... Number of words in each password. [2] \n");
   printf ("         /MinSize=# ...... Minimum size of password. [7] \n");
   printf ("         /MaxSize=# ...... Maximum Size of password. [16]\n");
   printf ("         /Prefix=str ..... String to prepend to password.\n");
   printf ("         /Suffix=str ..... String to append to password.\n");
   printf ("         /Separator[=str]. Separator between words. [.] use no = for none.\n");
   printf ("         /Cut ............ Cut String to fit max size.\n");
   printf ("         /Upper .......... Uppercase the password.\n");
   printf ("         /Seed=# ......... Set random starting seed.\n");
   printf ("         /WordsFile=file . Use alternate words file. [words]\n");
   printf ("\n");
   printf ("   # in password are changed to a random digit.\n");
   printf ("   @ in password are changed to a random character.\n");
   printf ("\n");
   printf ("EXAMPLES:\n");
   printf ("  genpwd 10\n");
   printf ("  genpwd 500 /MaxSize=7 /MinSize=7 /NumWords=1 /Suf=# /Pre=#\n");
   printf ("  genpwd 100 /Max=7 /Min=7 /Num=1 /Suffix=##@\n");
   printf ("  genpwd 15 /Max=40 /Num=5\n");
   printf ("  genpwd 50 /Min=20 /Max=40 /Num=3 /Upper /Sep\n");
   printf ("  genpwd 15 /Max=40 /Num=5 /Sep=#\n");
   printf ("  genpwd 50 /Pre=@@@### /Cut /Max=6 /Num=1\n");
   exit (0);
   }


ULONG lrnd (void)
   {
   ULONG l;

   l  = (ULONG)rand () << 15;
   l |= (ULONG)rand ();
   return l;
   }


PSZ GetWord (FILE *fp, ULONG ulFileLen)
   {
   ULONG ulOffset;

   ulOffset = lrnd () % (ulFileLen - 32);
   fseek (fp, ulOffset, SEEK_SET);
   FilReadTo (fp, NULL, "\n", 0, TRUE);
   FilReadWord (fp, szWORD, " ", "\n", sizeof (szWORD), TRUE);
   return szWORD;
   }


int _cdecl main (int argc, char *argv[])
   {
   USHORT uMinSize, uMaxSize, uWords, uPWDs, idx;
   USHORT uPWDLen, uSepLen, uPWDCount, uWCount, uSeed;
   ULONG  ulFileLen;
   char   szPWD[512];
   PSZ    pszWordFile, pszWord, pszSeparator, pszSuf, pszPre;
   FILE   *fp;
   BOOL   bCut, bUpper;


   if (ArgBuildBlk ("? *^MinSize% *^MaxSize% *^WordsFile% *^NumWords% "
                    "*^Separator? *^Cut *^Upper *^Prefix% *^Suffix% *^Seed%"))
      Error ("%s", ArgGetErr ());

   if (ArgFillBlk (argv))
      Error ("%s", ArgGetErr ());

   if (ArgIs ("?") || !ArgIs (NULL))
      Usage ();

   pszSeparator= (ArgIs ("Separator") ? ArgGet ("Separator", 0) : ".");
   if (!stricmp (pszSeparator, "none"))
      pszSeparator = NULL;

   pszSuf= (ArgIs ("Suffix") ? ArgGet ("Suffix", 0) : "");
   pszPre= (ArgIs ("Prefix") ? ArgGet ("Prefix", 0) : "");
   pszWordFile = (ArgIs ("WordsFile") ? ArgGet ("WordsFile", 0) : "words");
   uMinSize = (ArgIs ("MinSize" ) ? atoi (ArgGet ("MinSize", 0)) : 4);
   uMaxSize = (ArgIs ("MaxSize" ) ? atoi (ArgGet ("MaxSize", 0)) : 16);
   uWords   = (ArgIs ("NumWords") ? atoi (ArgGet ("NumWords", 0)): 2);
   uSeed    = (ArgIs ("Seed")     ? atoi (ArgGet ("Seed",     0)): (USHORT)time(NULL));
   uPWDs    = atoi (ArgGet (NULL, 0));
   bCut     = ArgIs ("Cut");
   bUpper   = ArgIs ("Upper");
   uSepLen  = (pszSeparator ? strlen (pszSeparator) : 0);

   srand (uSeed);


   /*--- sanity checks ---*/
   if (!uWords)
      Error ("Need at least 1 word");
   if (uMinSize < (uWords-1)*uSepLen + uWords*3)
      uMinSize = (uWords-1)*uSepLen + uWords*3;
   if (uMaxSize < uMinSize)
      uMaxSize = uMinSize;

   if (!(fp = fopen (pszWordFile, "rt")))
      Error ("Cannot open words file: %s", pszWordFile);
   ulFileLen = filelength (fileno (fp));

   for (uPWDCount=0; uPWDCount< uPWDs; )
      {
      strcpy (szPWD, pszPre);
      for (uWCount=0; uWCount< uWords; uWCount++)
         {
         pszWord = GetWord (fp, ulFileLen);
         strcat (szPWD, pszWord);
         strcat (szPWD, (pszSeparator ? pszSeparator : ""));
         }
      uPWDLen = strlen (szPWD) - uSepLen;
      szPWD[uPWDLen] = '\0';             // clip trailing dot

      strcat (szPWD, pszSuf);
      uPWDLen = strlen (szPWD);

      if (bCut && uPWDLen > uMaxSize)
         uPWDLen = uMaxSize;
      szPWD[uPWDLen] = '\0';             // clip trailing dot

      /*--- resolve prefix/separator/suffix references ---*/
      for (idx=0; idx<uPWDLen; idx++)
         {
         if (szPWD[idx]=='#')
            szPWD[idx] = (char)('0' + (rand () / 256) % 10);
         if (szPWD[idx]=='@')
            szPWD[idx] = (char)('a' + (rand () / 256) % 26);
         }

      if (uPWDLen < uMinSize || uPWDLen > uMaxSize)
         continue;

      if (bUpper)
         for (idx=0; idx<uPWDLen; idx++)
            szPWD[idx] = (char)toupper (szPWD[idx]);
         
      printf ("%s\n", szPWD);
      uPWDCount++;
      }
   return 0;
   }

