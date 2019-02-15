/*
 *
 * QUERYTSV.c
 * Tuesday, 6/22/1993.
 *
 */

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>     
#include <GnuType.h>
#include <GnuFile.h>
#include <GnuStr.h>
#include <GnuArg.h>
#include <GnuMath.h>
#include <GnuCfg.h>
#include <GnuMisc.h>


#define SZLEN       4096
#define MAXCOL      256
#define ERRLEN      128
#define MAXITEMKEYS 10000
#define MAXUNITS    200


#define EQ  1
#define NE  2
#define LT  3
#define GT  4
#define LE  5
#define GE  6

#define AND 1
#define OR  2
#define NOT 3
#define XOR 4

#define OMF 0
#define OML 1
#define ONF 2
#define ONL 3


typedef PSZ  *PPSZ;
typedef int (*PTFN)(PPSZ ppsz, int iCol, int iData);


typedef struct
   {
   BOOL   bCol;     // TRUE: data from file column,  FALSE: data from cmd line
   USHORT uCol;     // column index, if bCol=TRUE;
   PSZ    psz;      // data if bCol = FALSE;
   BOOL   bPrev;    // is col from the previous line?
   USHORT uXlate;   // should col be translated 0=no?
   } ELEMENT;

typedef struct
   {
   ELEMENT  l;      // data on left
   ELEMENT  r;      // data on right
   USHORT uTest;    // operation on data (GT LT EQ NE LE GE)
   USHORT uOP ;     // op with prev element (AND OR XOR)
   BOOL   bInvert;  // true means to invert the result of test
   BOOL   bNumeric; // true if compare should be done numerically
   } CONDITION;


/*--- current data line ---*/
char sz    [SZLEN] = "\0";
PSZ  pcols [MAXCOL];
USHORT uCOLCOUNT= 0;


/*--- previous data line ---*/
char szprev [SZLEN] = "\0"; 
PSZ  pprevcols [MAXCOL];     
USHORT uPREVCOLCOUNT= 0;     


ULONG ulLINE    = 0;
ULONG ulMATCHES = 0;

BOOL   bCASE, bREGEX, bBRACKET, bCLIP, bKEEPPREVLINE, bFILEPOS;
BOOL   bLISTLABELS, bCOLLABELS, bQUIET, bABORTABLE;
BOOL   bUNDERSCORE, bMATH0;


// OLD
//, bCOLMODE
// char cSEPARATOR    = '\t';
// char cOUTSEPARATOR = '\t';
//

#define COL '\xFE'
#define CSV '\xFF'

char cINFORMAT  = '\t';
char cOUTFORMAT = '\t';



USHORT uDECIMALS = 5;

FILE  *fpIn, *fpOut[4];

PSZ   pszInFile;


CONDITION CONDS[20];
USHORT uCONDS = 0;

ELEMENT PRINTCOLS [MAXCOL];
USHORT  uPRINTCOLS = 0;

ELEMENT COLLABELS [MAXCOL];
USHORT  uCOLLABELS = 0;


PSZ    *XLATE     = NULL;  // xlate array
USHORT uMAXXLATES = 8000;  // max size of array
USHORT uXLATES    = 0;     // actual size of array
ULONG  ulXLATIONS = 0;     // count of xlates performed

ULONG  ulMAXMATCHES;


/*************************************************************************/
/*                                                                       */
/*  Help/Info functions                                                  */
/*                                                                       */
/*************************************************************************/

void Label (void)
   {
   if (!bQUIET)
      fprintf (stderr, " QUERYTSV   Text file query utility  v1.1       C.F.       %s %s\n\n", __DATE__, __TIME__);
   }


void Help1 (void)
   {
   printf (" USAGE: QUERYTSV tsvFile [options] [operands] [conditions] [fields]\n\n");
   printf (" [overview]:\n\n");
   printf (" This program lets you do simple queries on a Tab Separated Value (TSV) file.\n");
   printf (" You supply conditions on the command line, and combine those conditions\n");
   printf (" using operands, and if the result evaluates to TRUE, text is produced.\n");
   printf (" The conditions are evaluated once for each line in the input file, and the\n");
   printf (" data for the current line is used in the conditions and in the output text.\n");
   printf (" The actual text produced is specified in the fields section.  See individual\n");
   printf (" help sections for more information.\n\n");
   printf (" Command line parameters may also be specified by using the QUERYTSV env var.\n");
   exit (0);
   }


void Help3 (void)
   {
   printf (" USAGE: QUERYTSV tsvFile [options] [operands] [conditions] [fields]\n\n");
   printf (" [operands]:\n\n");
   printf ("    & - AND result of next condition with result so far.\n");
   printf ("    ! - OR  result of next condition with result so far.\n");
   printf ("    ~ - XOR result of next condition with result so far.\n\n");
   printf (" Operands specify the relationship between conditions.\n");
   printf (" Operands do not need to be specified between each condition.  The last\n");
   printf (" operand specified is used until it is changed.  The default operand \n");
   printf (" is &.  Also note that precidence is LEFT TO RIGHT, and that\n");
   printf (" there is no way to override this precedence.  If you cannot construct\n");
   printf (" your query with this limitation, you can pipe the output of a partial\n");
   printf (" quert into another instance of this prog...\n");
   exit (0);
   }


void Help4 (void)
   {
   printf (" USAGE: QUERYTSV tsvFile [options] [operands] [conditions] [fields]\n\n");
   printf (" [conditions]:\n");
   printf ("\n");
   printf (" conditions are of the form [value][test][value]\n");
   printf ("\n");
   printf (" tests can be one of:\n");
   printf ("    .EQ.  - Equal                       .NEQ.  -  Numeric comparisons\n");
   printf ("    .NE.  - Not equal                   .NNE.  -   with the same meaning\n");
   printf ("    .LT.  - Less than                   .NLT.  -\n");
   printf ("    .GT.  - Greater than                .NGT.  -\n");
   printf ("    .LE.  - Less than or equal          .NLE.  -\n");
   printf ("    .GE.  - Greater than or equal       .NGE.  -\n\n");
   printf (" Use the tests on the left for strings, and the tests on the right for numeric. \n\n");
   printf (" values can be one of\n");
   printf ("    #n     - The data in this field (#5 for example, 1st col is 1).\n");
   printf ("    #Label - Data field specified by name (you need the -l option).\n");
   printf ("    ##n    - The data in this field from the previous line.\n");
   printf ("    ##Label- Data field specified by name in thew previous line.\n");
   printf ("    #~n    - Translate this col through xlate file, no match=pass thru.\n");
   printf ("    #!n    - Translate this col through xlate file, no match=no data.\n");
   printf ("    #0     - This special column means means the line number.\n");
   printf ("    Text   - Plain ole text to compare with.\n");
   printf ("    regexp - Text with '*' and/or '?' (you need the -r option).\n");
   printf ("    @expr  - Math expression. #n, #Label ok. (ex @log(#1)*6).\n\n");
   printf (" Values have a number of restrictions:  You will need to use a data column\n");
   printf (" (ie #5 or #Label) for at least one of the 2 values or the condition will\n");
   printf (" be invariant.  If you use the #Label format, you must use the -l option\n");
   printf (" and the text 'Label' must actually be in the first line, or you will get\n");
   printf (" an error.  If you use the regexp format, you must use the -r option (which\n");
   printf (" is slower).  Furthermore, regexp values must only be used for the right value\n");
   printf (" (? and * are treated as normal chars for the left value), and finally,\n");
   printf (" regexp conditions only make sense when used with the .EQ. and .NE. tests.\n");
   printf (" Values of the format @expr are slow.  This format allows you to create a\n");
   printf (" math expression using the data.  Expressions must have a @ as the first char,\n");
   printf (" and may not contain imbedded blanks.  All standard operands and functions are\n");
   printf (" valid, plus lots of others which I won't admit.  See examples.\n");
   exit (0);
   }


void Help5 (void)
   {
   printf (" USAGE: QUERYTSV tsvFile [options] [operands] [conditions] [fields]\n\n");
   printf (" [fields]:\n\n");
   printf (" fields specify what to print.  fields are of the form:\n\n");
   printf ("    #n     - The data in this field (#5 for example, 1st col is 1).\n");
   printf ("    #Label - Data field specified by name (you need the -l option).\n");
   printf ("    ##n    - The data in this field from the previous line.\n");
   printf ("    ##Label- Data field specified by name in thew previous line.\n");
   printf ("    #~n    - Translate this col through xlate file, no match=pass thru.\n");
   printf ("    #!n    - Translate this col through xlate file, no match=no data.\n");
   printf ("    Text   - Plain ole text to print.\n");
   printf ("    *      - Print all fields for the current line.\n");
   printf ("    $      - Print all in a 1 field per line, labeled format.\n"); 
   printf ("    #0     - This special column means print the line number.\n");
   printf ("    @expr  - Math expression. #n, #Label ok. (ex @#Cost*1.05+#Extra).\n");
   printf ("    #n-#m  - Print this range of columns.\n");
   printf (" Look at the conditions help section for more info, because fields\n");
   printf (" are actually the same datatype as a values.\n");
   exit (0);
   }



void Help6 (void)
   {
   printf (" USAGE: QUERYTSV tsvFile [options] [operands] [conditions] [fields]\n\n");
   printf (" examples:\n");
   printf ("\n");
   printf (" QUERYTSV data.tsv #1.EQ.FRED #2.NE.SAM *\n");
   printf ("\n");
   printf ("   the file data.tsv is read 1 line at a time, if the first col is 'FRED' and\n");
   printf ("   the 2nd col is not 'SAM' then the entire line is written to stdout.\n");
   printf ("\n");
   printf (" QUERYTSV data.csv -d=, -lcr #Desc.EQ.*MONEY* #Cost.NGT.500 #0 #1-#8\n");
   printf ("\n");
   printf ("   data.csv has commas (=d-,) separating fields, and the first line contains\n");
   printf ("   labels (-l). compares are case sensitive (-c), and wildcards are allowed \n");
   printf ("   (-r).  If the Desc col has 'MONEY' as part of the string and the Cost col\n");
   printf ("   > 500, then the line number and the data columns 1 thru 8 are printed.\n");
   printf ("\n");
   printf (" QUERYTSV Minval: -ib #5 MaxVal: #6 Tot: #9  \n");
   printf ("\n");
   printf ("   reads from stdin (-i) and writes to stdout.  Output cols are enclosed in\n");
   printf ("   brackets (-b).  There are no conditions, so every line is processed.  the\n");
   printf ("   text 'Minval:' is printed, then the data in column 5, ...\n");
   printf ("\n");
   printf (" QUERYTSV data.csv -d=, #1.EQ.IT1 | #1.EQ.IT2 & #2.NLE.#3 * -onl otherdat.tsv\n");
   printf ("\n");
   printf ("   the entire line is written to stdout if col 1 is 'IT1' or 'IT2' and the \n");
   printf ("   numeric value in col 2 is less than or equal to the val in col 3.  The lines\n");
   printf ("   that don't match are sent to the file otherdat.tsv.\n");
   printf ("\n");
   printf (" QUERYTSV data.tsv -onl=stdout | #5.EQ.#6 #5.EQ.#7 #6.EQ.#7 #1-#3 @#4*10 #5-#10\n");
   printf ("\n");
   printf ("   Output for both matching fields and non matching lines is sent to stdout.\n");
   printf ("   if col 5 = col 6 or col 5 = col 7 or col 6 = col7, then cols 1 thru 3, col\n");
   printf ("   4's value multiplied by 10, and columns 5 thru 10 are printed.  If the\n");
   printf ("   condition is false, the line is printed unmodified, also to stdout.\n");
   printf ("\n");
   printf (" QUERYTSV data.tsv @#5*3.NEQ.@sin(#6) @211^#2/#3\n");
   printf ("\n");
   printf ("   If the value of col 5 multiplied by 3 is numerically equal to the sine of \n");
   printf ("   the value of col 6, then print out the result of 211 to the power of the\n");
   printf ("   value of col 2 divided by the value of column 3.  Right?\n");
   printf ("\n");
   printf (" QUERYTSV a.TSV -z@bob=1000 -e0 @@bob=@bob-2 *\n");
   printf ("\n");
   printf ("   -z sets the expression var @bob to 1000.  For each row, @bob is decremented\n");
   printf ("   by 2 and printed, along with the row's data.  Expr vars are vars whose names\n");
   printf ("   start with a @.  They may be assigned and used within a value expression.\n");
   printf ("   The -z option is a param used to eval expressions only once.\n");
   exit (0);
   }


void Help7 (void)
   {
   printf (" USAGE: QUERYTSV tsvFile [options] [operands] [conditions] [fields]\n\n");
   printf (" Pseudo BNF for [fields] and [values]:\n\n");
   printf (" *field*   = *value* [ '-' *value*]\n");
   printf (" *value*   = { DATACOLUMN | text | REGEXP | EXPR}\n");
   printf (" DATACOL   = { ['#' ['#'] [ '~' | '!']] { ColumnNumber | ColumnName}}\n");
   printf (" REGEXP    = { text | '*' | '?' } ...\n");
   printf (" EXPR      = { '@' {ID [OPERAND ID] ... | ASSIGNMENT} }\n");
   printf (" ASSIGNMENT= { VARNAME = EXPR }\n");
   printf (" OPERAND   = { '+' | '-' | '*' | '/' | '^' }\n");
   printf (" ID        = { Name | DATACOL | VARNAME | FUNCTION }\n");
   printf (" VARNAME   = { '@' Name\n");
   printf (" FUNCTION  = { {'sin' | 'log'} '(' EXPR ')' }  + lots of other functions\n");
   exit (0);
   }



void Help2 (BOOL bUsage)
   {
   if (bUsage)
      printf (" USAGE: QUERYTSV tsvFile [options] [operands] [conditions] [fields]\n\n");
   printf (" [options]:\n\n");
   printf ("    -c        - Case sensitive comparisons.\n");
   printf ("    -r        - Regular expr matching (* and ? only).\n");
   printf ("    -k        - Clip leading/trailing whitespace before compare.\n");
   printf ("    -b        - Bracket output columns. Useful for seeing whitespace.\n");
   printf ("    -l        - 1st line in file is Column Labels, so read em.\n");
   printf ("    -l=file   - label names are contained in this file, 1 per line.\n");
   printf ("    -a        - List labels only. (Implies -l).\n");
   printf ("    -n        - Nologo, quiet mode, no stats or header line.\n");
   printf ("    -e=#      - Number of decimals to print for math expr results.\n");
   printf ("    -0        - Math errors return 0 rather than <invalid>.\n");
   printf ("    -d=#      - Set delimeter char to # (use ascii val or char) or 'd=col'.\n");
   printf ("    -outd=#   - Set delimeter char for output (use 0 for none)'.\n");
   printf ("    -q=#      - Quit after reading # lines.\n");
   printf ("    -m=#      - Quit after # matches.\n");
   printf ("    -i        - Use stdin for input file. default is 1st param.\n");
   printf ("    -s=#      - Write Stats to stderr after every # lines read.\n");
   printf ("    -omf=file - Output matching record fields to this file (default=stdout).\n");
   printf ("    -oml=file - Output matching record lines unmodified (default=none).\n");
   printf ("    -onf=file - Output non-matching record fields to this file (default=none).\n");
   printf ("    -onl=file - Output non-matching record lines unmodified (default=none).\n");
   printf ("    -f=file   - Retrieve rest of command line from this file.\n");
   printf ("    -j=#      - Start at this byte offset.\n");
   printf ("    -_        - Treat _ on cmd line as spaces.\n");
   printf ("    -x=file   - Load This Translation file (file w 2 cols).\n");
   printf ("    -xsize=#  - Maximum lines in Xlate File.\n");
   printf ("    -abort    - Abort on keypress.\n");
   printf ("    -fp       - Show file pos in stat line.\n");
   printf ("    -PClass=# - Set priority class.\n");
   printf ("    -PDelta=# - Set priority delta.\n");
   printf ("    -z=define - Initialize expr variables.\n\n");
   printf (" Options modify the default behavior of the program. Options with no values\n");
   printf (" can be combined. (I.E. -crnk is legal).\n");
   exit (0);
   }


void Usage (void)
   {
   printf ("\n");
   printf (" USAGE: QUERYTSV tsvFile [options] [operands] [conditions] [fields]\n\n");
   printf (" type  QUERYTSV -h1   for overview.\n");
   printf (" type  QUERYTSV -h2   for help with options. (below)\n");
   printf (" type  QUERYTSV -h3   for help with operands.\n");
   printf (" type  QUERYTSV -h4   for help with conditions.\n");
   printf (" type  QUERYTSV -h5   for help with fields.\n");
   printf (" type  QUERYTSV -h6   for examples.\n");
   printf (" type  QUERYTSV -h7   for pseudo-quasi-lazy BNF for fields & values.\n\n");
   Help2 (FALSE);
   exit (0);
   }


/*************************************************************************/
/*                                                                       */
/*  Generic functions                                                    */
/*                                                                       */
/*************************************************************************/


//USHORT SetPriority (USHORT uClass, SHORT iDelta)
//   {
//   USHORT  uScope;
//   USHORT  uID;
//   USHORT uRet;
//
//   uScope = 0;
//   uID    = 0;
//
//   if (uClass > 4)
//      uClass = 0;
//
//   if (iDelta < -31 || iDelta > 31)
//      iDelta = 0;
//
//   if (uRet = DosSetPrty (uScope, uClass, iDelta, uID))
//      {
//      fprintf (stderr, "Set priority error = %d\n", uRet);
//      exit (1);
//      return 1;
//      }
//   if (!bQUIET)
//      fprintf (stderr, "Priority Set to %d:%d\n", uClass, iDelta);
//
//   return 0;
//   }


/*******************************************************************/
/*                                                                 */
/* Line Input functions                                            */
/*                                                                 */
/*******************************************************************/


void StorePrevLine (void)
   {
   USHORT i;

   memcpy (szprev, sz, SZLEN);
   uPREVCOLCOUNT= uCOLCOUNT;

   for (i=0; i <= uPREVCOLCOUNT; i++)
      if (pcols[i])                                // 1st line in file has
         pprevcols[i] = szprev + (pcols[i] - sz);  // no previous line
      else                                         // so blank out the cols
         pprevcols[i] = "";                        // in that case
   }



/*
 * unlike the TSV line format,
 * This fn does not set the uCOLCOUNT
 *
 */
int ReadCOLLine (FILE *fp, PPSZ pcols)
   {
   USHORT uColSize, uCol = 1;
   int    c;
   PSZ    p;

   pcols[1] = p = sz;
   uColSize = COLLABELS [1].uCol;

   while ((c = getc (fp)) != '\n' && c != EOF)
      {
      *p++ = (char)c;
      if (--uColSize)
         continue;
      uCol++;
      *p++ = '\0';
      pcols[uCol] = p;
      uColSize = COLLABELS[uCol].uCol;
      }
   *p = '\0';

   /*--- check for EOF condition ---*/
   if ((c == EOF) && (uCol == 1) && !*pcols[1])
      return 0;

   /*--- cleanup any frayed ends ---*/
   if (uCol < uCOLCOUNT)
      {
      fprintf (stderr, "Short line: %ld  %d columns\n", ulLINE, uCol);
      for (; uCol <= uCOLCOUNT; uCol++)
         pcols[uCol] = p;
      }
   return 1;
   }



int ReadCSVLine (FILE *fp, PPSZ pcols)
   {
   PSZ    psz;
   USHORT uMaxLen, uLen, uCol, uDelim = ',';

   if (feof (fp))
      return 0;

   psz     = sz;
   uMaxLen = SZLEN;
   for (uCol=1; uDelim == ','; uCol++)
      {
      pcols[uCol] = psz;
      uDelim = FilReadCSVField (fp, psz, uMaxLen, 3);
      uLen = strlen (psz);
      psz     += uLen + !!(uLen < uMaxLen);
      uMaxLen -= uLen + !!(uLen < uMaxLen);
      }
   uCOLCOUNT = uCol -1;
   return 1;
   }



int ReadDelimLine (FILE *fp, PPSZ pcols)
   {
   PSZ   pLeft, pRight;
   int   iCol = 1;
   BOOL  bLast = FALSE;

   if (FilReadLine (fp, sz, "", SZLEN) == -1)
      return 0;

   for (pLeft = sz; !bLast; pLeft = pRight+1)
      {
      if (bLast = !(pRight = strchr (pLeft, cINFORMAT)))
         pRight = strchr (pLeft, '\0');

      *pRight = '\0';
      pcols[iCol] = pLeft;
      iCol++;
      }
   uCOLCOUNT = iCol -1;
   return 1;
   }


int ReadNextLine (FILE *fp, PPSZ pcols)
   {
   if (bKEEPPREVLINE)
      StorePrevLine ();

   if (cINFORMAT == COL)            // COL Format
      return ReadCOLLine (fp, pcols);
   if (cINFORMAT == CSV)            // CSV Format
      return ReadCSVLine (fp, pcols);
   return ReadDelimLine (fp, pcols);   // Delim Format
   }






/*******************************************************************/
/*                                                                 */
/* Xlate functions                                                 */
/*                                                                 */
/*******************************************************************/


int _cdecl pfnCmp (const PSZ *pp1, const PSZ *pp2)
   {
   if (bCASE)
      return strcmp (*pp1, *pp2);
   else
      return stricmp (*pp1, *pp2);
   }


void LoadXlateFile (PSZ pszFile)
   {
   FILE   *fp;
   PSZ    p;
   USHORT uStrLen;

   if (!(fp = fopen (pszFile, "rt")))
      Error ("Unable to open Xlate file %s", pszFile);

   if (!(XLATE = malloc (uMAXXLATES * sizeof (PSZ))))
      Error ("Unable to malloc %u bytes for xlate index", uMAXXLATES * sizeof (PSZ));

   if (!bQUIET)
      fprintf (stderr, "Loading xlate file %s ...     ", pszFile);

   for (uXLATES=0; uXLATES<uMAXXLATES && (FilReadLine (fp, sz, "", SZLEN) != -1); uXLATES++)
      {
      /*--- malloc an extra byte for double term, if only 1 col---*/
      uStrLen = strlen(sz);
      if (!(XLATE [uXLATES] = malloc (uStrLen + 2)))
         Error ("Unable to malloc %u for xlate entry", uStrLen + 2);
      strcpy (XLATE [uXLATES], sz);

      if (p = strchr (XLATE [uXLATES], cINFORMAT))
         *p = '\0';
      else
         XLATE [uXLATES][uStrLen+1] = '\0';  /*--- double null --*/

      if (!(uXLATES % 100))
         fprintf (stderr, "\b\b\b\b\b%5.5d", uXLATES);
      }
   if (!bQUIET)
      fprintf (stderr, "\b\b\b\b\b%5.5d Done.\n", uXLATES);

   if (!bQUIET)
      fprintf (stderr, "Sorting xlates ... ");

   qsort (XLATE, uXLATES, sizeof (PSZ), pfnCmp);

   if (!bQUIET)
      fprintf (stderr, "Done.\n", uXLATES);

   fclose (fp);
   }


PSZ XlateString (PSZ psz, BOOL bNoMatchOK)
   {
   PSZ p, *ppMatch;

   if (ppMatch = bsearch (&psz, XLATE, uXLATES, sizeof (PSZ), pfnCmp))
      {
      ulXLATIONS++;
      p = *ppMatch + strlen (*ppMatch) + 1;
      if (*p)
         return p;
      return "";
      }
   if (bNoMatchOK)
      return psz;

   return "";
   }

/*******************************************************************/
/*                                                                 */
/*                                                                 */
/*                                                                 */
/*******************************************************************/

char szEl [64];

PSZ XCOL (PSZ psz, USHORT uXlate)
   {
   PSZ p;

   p = (uXlate ? XlateString (psz, uXlate < 2) : psz);

   if (bCLIP && *p)
      StrStrip (StrClip (p, " \t\n"), " \t\n");
   return p;
   }


PSZ PCOL (USHORT i, BOOL bPrevLine, USHORT uXlate)
   {
   if (!i)
      {
      sprintf (szEl, "%lu", ulLINE - !!(bPrevLine));
      return XCOL (szEl, uXlate);
      }
   if (i > (bPrevLine ? uPREVCOLCOUNT : uCOLCOUNT))
      return "";

   if (bPrevLine)
      return XCOL (pprevcols[i], uXlate);
   else
      return XCOL (pcols[i], uXlate);
   }




/*******************************************************************/
/*                                                                 */
/*  Column label fn's                                              */
/*                                                                 */
/*******************************************************************/

USHORT MaxLabelSize (void)
   {
   USHORT i, j;

   for (j=i=0; i <= uCOLLABELS; i++)
      j = max (j, strlen (COLLABELS [i].psz));
   return j;
   }



USHORT ReadLabelsFile (PSZ pszFile)
   {
   FILE *fp;
   PSZ   p, p2;
   char  szBaseName [128];

   p = (p = strrchr (pszInFile, '\\') ? p : pszInFile);
   strcpy (szBaseName, p);
   if (p = strchr (szBaseName, '.'))
      *p = '\0';

   if (!(fp = CfgFindSection (pszFile, szBaseName)))
      Error ("Could not fine label file %s, or could not find "
             "section [%s] in label file.", pszFile, szBaseName);

   if (!bQUIET)
      fprintf (stderr, "Loading labels from label file %s ... ", pszFile);

   COLLABELS [0].psz  = "Index";
   COLLABELS [0].uCol = 6;
   COLLABELS [0].bCol = TRUE;

   for (uCOLLABELS=1; FilReadLine (fp, sz, ";", SZLEN) != -1; uCOLLABELS++)
      {
      if (CfgEndOfSection (sz))
         break;

      p = StrStrip (StrClip (sz, " \t\n"), " \t\n");

      for (p2=p; *p2 && *p2 != ' ' && *p2 != '\t'; p2++)
         ;
      *p2++ = '\0';

      COLLABELS [uCOLLABELS].psz  = strdup (p);
      COLLABELS [uCOLLABELS].uCol = ((cINFORMAT == COL) ? atoi (p2) : 0); // col len
      COLLABELS [uCOLLABELS].bCol = TRUE;
      }
   if (uCOLLABELS)
      uCOLLABELS--;
   fprintf (stderr, "Done.\n");
   fclose (fp);
   uCOLCOUNT = uCOLLABELS;  // only important if (cINFORMAT == COL)

   COLLABELS [0].uXlate  = MaxLabelSize ();

   return uCOLLABELS;
   }


USHORT ReadLabelsFromInFile (FILE *fp, PPSZ pcols)
   {
   USHORT i;

   ReadNextLine (fp, pcols);
   for (i=1; i<=uCOLCOUNT; i++)
      {
      COLLABELS [i].psz  = strdup (PCOL(i, FALSE, FALSE));
      COLLABELS [i].bCol = TRUE;
      }
   return uCOLLABELS = uCOLCOUNT;
   }



USHORT ReadCOLSPECFromInFile (FILE *fp, PPSZ pcols)
   {
   USHORT i = 0;
   char   c;
   PSZ    p;

   if (FilReadLine (fp, sz, "", SZLEN) == -1)
      return 0;

   p = sz;
   while (*p)
      {
      if (!(c = *p++))
         break;

      COLLABELS [++i].uCol = 1;
      COLLABELS [i].psz    = NULL;

      while ((char)*p == c)
         {
         COLLABELS [i].uCol++;
         p++;
         }
      }
   uCOLCOUNT = i;
   COLLABELS [i+1].uCol = 0xFFFF;
   }



/*******************************************************************/
/*                                                                 */
/*                                                                 */
/*                                                                 */
/*******************************************************************/



/*
 * returns len of colid string
 *
 */

USHORT ParseColName (PSZ psz, ELEMENT *pe)
   {
   USHORT i, j;
   PSZ    p = psz;
   
   if (*psz++ != '#')
      return 0;

   pe->bCol = TRUE;
   pe->psz  = NULL;

   if (pe->bPrev = (*psz == '#'))
      psz++;                                 // prev line
   if (pe->uXlate = !!(*psz == '~'))
      psz++;                                 // pass through xlate
   else if (pe->uXlate = 2*!!(*psz == '!'))
      psz++;                                 // exclusive translate

   if (isdigit (*psz))
      {
      pe->uCol = atoi (psz);

      for ( ; isdigit (*psz); psz++)
         ;

      return psz - p;
      }

   if (!bCOLLABELS)
      Error ("Columns must be specified by number unless Labels are defined (-l)\n");

   for (j=0; isalpha (psz[j]); j++)
      ;
   for (i=1; i<=uCOLLABELS; i++)
      if (!strnicmp (psz, COLLABELS[i].psz, j))
         {
         pe->uCol = i;
         return psz - p + j;
         }
   Error ("Column Identifier not found: %s", psz);
   return TRUE;
   }




/*******************************************************************/
/*                                                                 */
/* Display functions                                               */
/*                                                                 */
/*******************************************************************/

char sz2 [256];

PSZ ExprValStr (PSZ pszExpr, PSZ pszResult)
   {
   USHORT  i=0;
   BIG     bgRes;
   ELEMENT el;

   while (*pszExpr)
      {
      if (*pszExpr != '#')
         {
         sz2[i++] = *pszExpr++;
         continue;
         }
      pszExpr += ParseColName (pszExpr, &el);
      strcpy (sz2+i, PCOL(el.uCol, el.bPrev, el.uXlate));
      i += strlen (sz2+i); // never trust sprintfs return val
      }

   sz2[i] = '\0';
   bgRes = AToBIG (sz2);

   if (!MthValid (bgRes))
      {
      if (bMATH0)
         sprintf (pszResult, "%.*Lf", uDECIMALS, (BIG) 0.0);
      else
         strcpy (pszResult, "<invalid>");
      }
   else
      sprintf (pszResult, "%.*Lf", uDECIMALS, bgRes);
   return pszResult;
   }





PSZ ElementVal (ELEMENT *pe)
   {
   if (!pe->bCol)
      {
      if (*(pe->psz) == '@')
         return ExprValStr (pe->psz + 1, szEl);
      return pe->psz;
      }
   return PCOL (pe->uCol, pe->bPrev, pe->uXlate);
   }



//void PrintLine (FILE *fp, PPSZ pcols, BOOL bEOL)
//   {
//   BOOL bFirstCol = TRUE;
//   USHORT j;
//   char   cSpacer;
//
//   cSpacer = (bBRACKET ? (char)' ' : cOUTFORMAT);
//
//   for (j=1; j<=uCOLCOUNT; j++)
//      {
//      if (bFirstCol)
//         bFirstCol = FALSE;
//      else if (cSpacer)
//         fputc (cSpacer, fp);
//
//      if (bBRACKET) fputc ('{', fp);
//      fputs (PCOL (j, FALSE, FALSE), fp);
//      if (bBRACKET) fputc ('}', fp);
//      }
//   if (bEOL)
//      fputc ('\n', fp);
//   }

void PrintLine (FILE *fp, PPSZ pcols, BOOL bEOL)
   {
   USHORT uCol;
   PSZ    psz;
   char   szBuff [SZLEN];

   for (uCol=1; uCol<=uCOLCOUNT; uCol++)
      {
      /*--- derive the field ---*/
      psz = PCOL (uCol, FALSE, FALSE);
      if (cOUTFORMAT == CSV)
         psz = StrMakeCSVField (szBuff, psz);

      /*--- write the field ---*/
      if (bBRACKET) 
         fputc ('{', fp);
      fputs (psz, fp);
      if (bBRACKET) 
         fputc ('}', fp);

      /*--- no delimeter at end ---*/
      if (uCol == uCOLCOUNT)
         break;

      /*--- write delim ---*/
      if (cOUTFORMAT == CSV)
         fputc (',', fp);
      else if (cOUTFORMAT != COL)
         fputc (cOUTFORMAT, fp);
      }
   if (bEOL)
      fputc ('\n', fp);
   }


void PrintLongLine (FILE *fp, PPSZ pcols)
   {
   USHORT j;

   for (j=1; j<=uCOLCOUNT; j++)
      if (bCOLLABELS)
         fprintf (fp, "Line %3.3lu Col %3.3d Label %*s: %s\n", ulLINE, j,
               COLLABELS[0].uXlate, COLLABELS[j].psz,
               PCOL (j, FALSE, FALSE));
      else
         fprintf (fp, "Line %3.3lu Col %3.3d: %s\n", ulLINE, j,
               PCOL (j, FALSE, FALSE));
   }



//void PrintFields (FILE *fp, PPSZ pcols)
//   {
//   BOOL bFirstCol = TRUE;
//   USHORT i;
//   char   cSpacer;
//
//   cSpacer = (bBRACKET ? (char)' ' : cOUTFORMAT);
//
//   for (i=0; i<uPRINTCOLS; i++)
//      {
//      if (bFirstCol)
//         bFirstCol = FALSE;
//      else if (cSpacer)
//         fputc (cSpacer, fp);
//
//      /*--- print all cols? ---*/
//      if (!PRINTCOLS[i].bCol && *(PRINTCOLS[i].psz) == '*')
//         {
//         PrintLine (fp, pcols, FALSE);
//         continue;
//         }
//
//      /*--- print all cols? ---*/
//      if (!PRINTCOLS[i].bCol && *(PRINTCOLS[i].psz) == '$')
//         {
//         PrintLongLine (fp, pcols);
//         continue;
//         }
//
//      if (bBRACKET) fputc ('{', fp);
//      fputs (ElementVal (PRINTCOLS + i), fp);
//      if (bBRACKET) fputc ('}', fp);
//      }
//   if (!bFirstCol)
//      fputc ('\n', fp);
//   }


void PrintFields (FILE *fp, PPSZ pcols)
   {
   USHORT uCol;
   PSZ    psz;
   char   szBuff [SZLEN];

   for (uCol=0; uCol<uPRINTCOLS; uCol++)
      {
      if (!PRINTCOLS[uCol].bCol && *(PRINTCOLS[uCol].psz) == '*') //print all cols?
         PrintLine (fp, pcols, FALSE);
      else if (!PRINTCOLS[uCol].bCol && *(PRINTCOLS[uCol].psz) == '$') //print all cols?
         PrintLongLine (fp, pcols);
      else
         {
         /*--- derive the field ---*/
         psz = ElementVal (PRINTCOLS + uCol);
         if (cOUTFORMAT == CSV)
            psz = StrMakeCSVField (szBuff, psz);

         /*--- write the field ---*/
         if (bBRACKET) 
            fputc ('{', fp);
         fputs (psz, fp);
         if (bBRACKET) 
            fputc ('}', fp);

         /*--- no delimeter at end ---*/
         if (uCol+1 == uPRINTCOLS)
            {
            uCol++; //we want a newline
            break;
            }

         /*--- write delim ---*/
         if (cOUTFORMAT == CSV)
            fputc (',', fp);
         else if (cOUTFORMAT != COL)
            fputc (cOUTFORMAT, fp);
         }
      }
   if (uCol)
      fputc ('\n', fp);
   }




/*******************************************************************/
/*                                                                 */
/* Test Condition Eval functions                                   */
/*                                                                 */
/*******************************************************************/


int RawCompare (PSZ pszL, PSZ pszR, BOOL bNumeric)
   {
   if (bNumeric)
      {
      BIG bgL, bgR;

      bgL = atof (pszL);
      bgR = atof (pszR);
      if (bgL == bgR) return 0;
      if (bgL > bgR)  return 1;
      return -1;
      }
   if (bREGEX && (strchr (pszR, '*') || strchr (pszR, '?')))
      return !StrMatches (pszL, pszR, bCASE);
   if (bCASE)
      return strcmp (pszL, pszR);
   return stricmp (pszL, pszR);
   }


BOOL TestLine (PPSZ pcols)
   {
   BOOL   bResult      = TRUE;
   BOOL   bFirstTest   = TRUE;
   BOOL   bTestResult;
   USHORT i;
   int    iRet;
   PSZ    pszL, pszR;

   for (i=0; i<uCONDS; i++)
      {
      pszL = ElementVal (&(CONDS[i].l));
      pszR = ElementVal (&(CONDS[i].r));

      iRet = RawCompare (pszL, pszR, CONDS[i].bNumeric);

      switch (CONDS[i].uTest)
         {
         case EQ: bTestResult = !iRet;     break;
         case NE: bTestResult = !!iRet;    break;
         case LT: bTestResult = iRet < 0;  break;
         case GT: bTestResult = iRet > 0;  break;
         case LE: bTestResult = iRet <= 0; break;
         case GE: bTestResult = iRet >= 0; break;
         }    

      if (CONDS[i].bInvert)
         bTestResult = !bTestResult;

      if (bFirstTest)
         {
         bResult    = bTestResult;
         bFirstTest = FALSE;
         continue;
         }

      switch (CONDS[i].uOP)
         {
         case AND: bResult = bResult && bTestResult;      break;
         case  OR: bResult = bResult || bTestResult;      break;
         case XOR: bResult = (bResult && !bTestResult) ||
                             (!bResult && bTestResult);   break;
         }
      }
   return bResult;
   }




/*******************************************************************/
/*                                                                 */
/* Test Condition Parse functions                                  */
/*                                                                 */
/*******************************************************************/




USHORT TestID (PSZ p, BOOL *pbNumeric)
   {
   *pbNumeric  = FALSE;
   if (!strnicmp (p, ".EQ.",  4)) return EQ;
   if (!strnicmp (p, ".NE.",  4)) return NE;
   if (!strnicmp (p, ".LT.",  4)) return LT;
   if (!strnicmp (p, ".GT.",  4)) return GT;
   if (!strnicmp (p, ".LE.",  4)) return LE;
   if (!strnicmp (p, ".GE.",  4)) return GE;

   *pbNumeric  = TRUE;
   if (!strnicmp (p, ".NEQ.", 5)) return EQ;
   if (!strnicmp (p, ".NNE.", 5)) return NE;
   if (!strnicmp (p, ".NLT.", 5)) return LT;
   if (!strnicmp (p, ".NGT.", 5)) return GT;
   if (!strnicmp (p, ".NLE.", 5)) return LE;
   if (!strnicmp (p, ".NGE.", 5)) return GE;

   return 0;
   }



BOOL IsCondition (PSZ psz, BOOL *pbRes)
   {
   BOOL bDummy;

   if (!(psz = strchr (psz, '.')))
      return FALSE;
   return !!TestID (psz, &bDummy);
   }



USHORT IsOperator (PSZ psz)
   {
   USHORT uLen;

   if ((uLen = strlen (psz)) != 1)
      return FALSE;

   switch (*psz)
      {
      case '&': return AND;
      case '!': return OR;
      case '~': return XOR;
      }
   return FALSE;
   }


void ReadElement (PSZ psz, ELEMENT *pe)
   {
   if (ParseColName (psz, pe))
      return;

   pe->psz = strdup (psz);
   pe->bCol= FALSE;
   if (bCLIP)
      StrStrip (StrClip (pe->psz, " \t\n"), " \t\n");
   return;
   }


void AddCondition (PSZ psz, USHORT uOP, BOOL bInvert)
   {
   strcpy (sz, psz);
   psz = strchr (sz, '.');

   CONDS[uCONDS].uTest = TestID (psz, &(CONDS[uCONDS].bNumeric));
   *psz = 0;
   ReadElement (sz, &(CONDS[uCONDS].l));
   psz = strchr (psz+1, '.') + 1;
   ReadElement (psz, &(CONDS[uCONDS].r));
   CONDS[uCONDS].uOP = uOP;
   CONDS[uCONDS].bInvert = bInvert;
   uCONDS++;
   }


void AddPrintCol (PSZ psz)
   {
   char szTmp [256];
   PSZ  p;
   int iStart, iEnd, iInc, i;
   USHORT uDiff;

   if (*psz=='@' || !strchr (psz, '-'))
      {
      ReadElement (psz, &(PRINTCOLS[uPRINTCOLS]));
      uPRINTCOLS++;
      return;
      }

   /*--- a column range ---*/
   strcpy (szTmp, psz);
   p = strchr (szTmp, '-');
   *p = '\0';

   if (*szTmp != '#' || p[1] != '#')
      Error ("Field ranges require data fields: %s", psz);
   ReadElement (szTmp, &(PRINTCOLS[uPRINTCOLS]));
   iStart = PRINTCOLS[uPRINTCOLS].uCol;
   uPRINTCOLS++;
   ReadElement (p+1, &(PRINTCOLS[uPRINTCOLS]));
   iEnd = PRINTCOLS[uPRINTCOLS].uCol;

   iInc  = (iStart < iEnd ? 1 : -1);
   uDiff = (iStart < iEnd ? iEnd-iStart : iStart-iEnd);

   /*--- move end element to end ---*/
   PRINTCOLS[uPRINTCOLS + uDiff-1].bCol = PRINTCOLS[uPRINTCOLS].bCol;
   PRINTCOLS[uPRINTCOLS + uDiff-1].uCol = PRINTCOLS[uPRINTCOLS].uCol;
   PRINTCOLS[uPRINTCOLS + uDiff-1].psz  = PRINTCOLS[uPRINTCOLS].psz;

   for (i=iStart+iInc; i!=iEnd; i+=iInc)
      {
      sprintf (szTmp, "#%d", i);
      ReadElement (szTmp, &(PRINTCOLS[uPRINTCOLS++]));
      }
   uPRINTCOLS++;
   }


void ParseTests (PPSZ pcols, USHORT uFirstArg, USHORT uArgs)
   {
   BOOL   bInvertNext  = FALSE;
   BOOL   bDummy;
   USHORT uOP          = AND;
   USHORT i, uRet;
   PSZ    p, psz;

   for (i=uFirstArg; i<uArgs; i++)
      {
      psz = ArgGet (NULL, i);

      if (bUNDERSCORE)
         for (p = psz; *p; p++)
            if (*p == '_')
               *p = ' ';

      if (uRet = IsOperator (psz))
         {
         switch (uRet)
            {
            case AND: uOP = AND;                  break;
            case  OR: uOP = OR ;                  break;
            case XOR: uOP = XOR;                  break;
            case NOT: bInvertNext = !bInvertNext; break;
            }
         }
      else if (IsCondition (psz, &bDummy))
         {
         AddCondition (psz, uOP, bInvertNext);
         bInvertNext = FALSE;
         }
      else
         AddPrintCol (psz);
      }
   }


void ListLabels (void)
   {
   USHORT i;

   for (i=1; i<=uCOLLABELS; i++)
      printf ("Col#: %3d, Label:%s\n", i, COLLABELS[i].psz);
   exit (0);
   }

/*******************************************************************/
/*                                                                 */
/*******************************************************************/

void OpenOutFiles (void)
   {
   char   szTmp[4];
   PSZ    p[4];
   USHORT i, j;

   szTmp[0] = 'o';
   szTmp[3] = '\0';

   for (i=0; i<4; i++)
      {
      fpOut[i] = NULL;
      p[i]     = NULL;

      szTmp[1] = (char)(i>1 ? 'n' : 'm');
      szTmp[2] = (char)(i%2 ? 'l' : 'f');

      if (!ArgIs (szTmp))
         continue;

      p[i]  = ArgGet (szTmp, 0);

      if (!stricmp (p[i], "stdout"))
         fpOut[i] = stdout;
      else if (!stricmp (p[i], "stderr"))
         fpOut[i] = stderr;
      else if (!stricmp (p[i], "none"))
         continue;
      else
         for (j = 0; j<i; j++)
            if (p[j] && !stricmp (p[j], p[i]))
               fpOut[i] = fpOut[j];

      if (fpOut[i])
         continue;

      if (!(fpOut[i] = fopen (p[i], "wt")))
         Error ("Unable to open output file %s", p[i]);
      }

   /*--- default value for matching output field lines ---*/
   if (!fpOut[OMF] && !ArgIs ("omf"))
      fpOut[OMF] = stdout;

//   if (!fpOut[0] && !fpOut[1] && !fpOut[2] && !fpOut[3])
//      Error ("No output selected", "");
   }


char ReadSeparator (PSZ p)
   {
   if (!p || !*p)            return '\t';
   if (!stricmp (p, "col"))  return COL;
   if (!stricmp (p, "csv"))  return CSV;
   if (isdigit (*p))         return (char) atoi (p);
   if (!stricmp (p, "\\t"))  return '\t';
   return *p;
   }



/*
 * This main fn is real big, but most of it is just
 * for parsing the potentially huge number of command line options
 * The real work is done in the section named MAIN LOOP
 */
int _cdecl main (int argc, char *argv[])
   {
   FILE   *fpCmd;
   PSZ    p;
   USHORT uFirstArg, uArgs, i, c;
   BIG    bgTmp;
   ULONG  ulQuitAfter, ulStatLines, ulStartPos;
   ULONG  ulDelta, ulHr, ulMin, ulSec;
   time_t tstart, tstop;

   ArgBuildBlk ("^c- ^r- ^k- ^b- ^l? ^a- ^n- ^i- ^j% ^fp ^0- "
                "? *^help ^h- ^h1 ^h2 ^h3 ^h4 ^h5 ^h6 ^h7 ^m% ^x% "
                "^e% ^d% ^omf% ^oml% ^onf% ^onl% ^f% ^z% ^q% ^s%"
                "*^PClass% *^PDelta% *^Abort ^xsize% _- ^outd%");

   if (ArgFillBlk (argv))
      return fprintf (stderr, "QUERYTSV ERROR: %s", ArgGetErr ());

   for (i=ArgIs ("f"); i; i--)
      {
      if (!(fpCmd = fopen (ArgGet ("f", i-1), "rt")))
         Error ("Unable to open command file %s", ArgGet ("f", i-1));
      while (FilReadLine (fpCmd, sz, "", SZLEN) != -1)
         ArgFillBlk2 (sz);
      fclose (fpCmd);
      }

   for (i=0; i< ArgIs ("z"); i++)
      bgTmp = AToBIG (ArgGet ("z", i));

   if (ArgFillBlk2 (getenv ("QUERYTSV")))
      return fprintf (stderr, "QUERYTSV ENV ERROR: %s", ArgGetErr ());

   bCASE       = ArgIs ("c");
   bREGEX      = ArgIs ("r");
   bBRACKET    = ArgIs ("b");
   bCLIP       = ArgIs ("k");
   bCOLLABELS  = ArgIs ("l");
   bLISTLABELS = ArgIs ("a");
   bQUIET      = ArgIs ("n");
   bABORTABLE  = ArgIs ("Abort");
   bFILEPOS    = ArgIs ("fp");
   bMATH0      = ArgIs ("0");
   bUNDERSCORE = ArgIs ("_");

   Label ();

   if (ArgIs ("?") || ArgIs ("h") || ArgIs ("help"))
      Usage ();

   if (ArgIs ("h1")) Help1 ();
   if (ArgIs ("h2")) Help2 (TRUE);
   if (ArgIs ("h3")) Help3 ();
   if (ArgIs ("h4")) Help4 ();
   if (ArgIs ("h5")) Help5 ();
   if (ArgIs ("h6")) Help6 ();
   if (ArgIs ("h7")) Help7 ();

   if (!(uArgs = ArgIs (NULL)) && argc == 1)
      Usage ();


//   if (ArgIs ("PClass") || ArgIs ("PDelta"))
//      {
//      int iClass, iLevel;
//
//      iClass = (ArgIs ("PClass") ? atoi (ArgGet ("PClass", 0)) : 0);
//      iLevel = (ArgIs ("PDelta") ? atoi (ArgGet ("PDelta", 0)) : 0);
//
//      SetPriority (iClass, iLevel);
//      }


   if (ArgIs ("i"))
      {
      uFirstArg = 0;
      fpIn = stdin;
      }
   else
      {
      uFirstArg = 1;
      pszInFile  = ArgGet (NULL, 0);
   
      if (!(fpIn = fopen (pszInFile, "rt")))
         return Error ("Unable to open input file: %s", pszInFile);
      }

   if (ArgIs ("d"))
      cINFORMAT = cOUTFORMAT = ReadSeparator (ArgGet("d", 0));

   if (ArgIs ("outd"))
      cOUTFORMAT = ReadSeparator (ArgGet("outd", 0));

   if (ArgIs ("e"))
      uDECIMALS = atoi (ArgGet("e", 0));

   if (ArgIs ("xsize"))
      uMAXXLATES = atoi (ArgGet("xsize", 0));

   if (ArgIs ("x"))
      LoadXlateFile (ArgGet ("x", 0));

   ulQuitAfter = 0xFFFFFFFFL;
   if (ArgIs ("q") && !(ulQuitAfter = (ULONG) atol (ArgGet ("q", 0))))
      Error ("Quitting after reading 0 lines is tantamount to becoming a republican.");

   ulStatLines = 0xFFFFFFFFL;
   if (ArgIs ("s") && !(ulStatLines = (ULONG) atol (ArgGet ("s", 0))))
      Error ("Stats after every 0 lines? An interesting topological experience!.");

   ulMAXMATCHES = 0xFFFFFFFFL;
   if (ArgIs ("m") && !(ulMAXMATCHES = (ULONG) atol (ArgGet ("m", 0))))
      Error ("Quit after 0 matches?");

   if (bCOLLABELS || bLISTLABELS)
      {
      if (p = ArgGet ("l", 0))  // labels in a file
         ReadLabelsFile (p);
      else
         {
         if (cINFORMAT == COL)
            ReadCOLSPECFromInFile (fpIn, pcols);
         ReadLabelsFromInFile (fpIn, pcols);
         }
      }
   else if (cINFORMAT == COL)
      {
      ReadCOLSPECFromInFile (fpIn, pcols);
      }

   if (bLISTLABELS)
      ListLabels ();

   OpenOutFiles ();

   if (ArgIs ("j"))
      {
      ulStartPos = atol (ArgGet ("j", 0));
      fseek (fpIn, ulStartPos, SEEK_SET);
      while ((c=getc(fpIn)) != '\n' && c != EOF)
         ;
      }
   ParseTests (pcols, uFirstArg, uArgs);


   /*--- only keep previous line around if we actually use it ---*/
   bKEEPPREVLINE = FALSE;
   for (i=0; i< uPRINTCOLS; i++)
      if (PRINTCOLS[i].bCol && PRINTCOLS[i].bPrev)
         bKEEPPREVLINE = TRUE;

   for (i=0; i< uCONDS; i++)
      if ((CONDS[i].l.bCol && CONDS[i].l.bPrev) ||
          (CONDS[i].r.bCol && CONDS[i].r.bPrev))
         bKEEPPREVLINE = TRUE;


   if (!uPRINTCOLS && !fpOut[OML] && !fpOut[ONL] && bQUIET)
      Error ("No output or stats selected");

   if (bABORTABLE)
      for (; kbhit (); getchar ())
         ;

   time(&tstart);

   /*
    * ---=== MAIN LOOP ===---
    */
   while (ulLINE < ulQuitAfter && ulMATCHES < ulMAXMATCHES)
      {
      if (!ReadNextLine (fpIn, pcols))
         break;
      ulLINE++;
      if (TestLine (pcols))
         {
         if (fpOut[OMF])  PrintFields (fpOut[OMF], pcols);
         if (fpOut[OML])  PrintLine   (fpOut[OML], pcols, TRUE);
         ulMATCHES++;
         }
      else
         {
         if (fpOut[ONF])  PrintFields (fpOut[ONF], pcols);
         if (fpOut[ONL])  PrintLine   (fpOut[ONL], pcols, TRUE);
         }

      if (!(ulLINE % 1000))
         {
         for (i=0; i<4; i++)
            if (fpOut[i])
               fflush (fpOut[i]);
         fflush (stdout);
         fflush (stderr);
         }

      if (!(ulLINE % ulStatLines))
         {
         if (bFILEPOS)
            fprintf (stderr, "%ld Lines Read, %ld Matches, %ld NonMatches. (FilePos: %ld)\n", 
                     ulLINE, ulMATCHES, ulLINE-ulMATCHES, ftell (fpIn));
         else
            fprintf (stderr, "%ld Lines Read, %ld Matches, %ld NonMatches.  So far...\n", 
                     ulLINE, ulMATCHES, ulLINE-ulMATCHES);
         }

      if (bABORTABLE && kbhit ())
         break;
      }
   time(&tstop);
   ulDelta = (ULONG) difftime (tstop, tstart);
   ulHr    = ulDelta/3600;
   ulMin   = (ulDelta - ulHr*3600)/60;
   ulSec   = ulDelta - (ulHr*3600 + ulMin*60);

   for (i=0; i<4; i++)
      if (fpOut[i])
         fclose (fpOut[i]);

   if (bFILEPOS)
      sprintf (sz, "(FilePos: %ld)", ftell (fpIn));
   else
      *sz = '\0';

   fclose (fpIn);

   if (bQUIET)
      return 0;
   if (uXLATES)
      fprintf (stderr, "%ld Lines Read, %ld Matches, %ld NonMatches, %ld Xlates %s\n", 
              ulLINE, ulMATCHES, ulLINE-ulMATCHES, ulXLATIONS, sz);
   else
      fprintf (stderr, "%ld Lines Read, %ld Matches, %ld NonMatches %s\n", 
              ulLINE, ulMATCHES, ulLINE-ulMATCHES, sz);

   fprintf (stderr, "Exec Time: %ldh:%ldm:%lds\n", ulHr, ulMin, ulSec);
   return 0;
   }


