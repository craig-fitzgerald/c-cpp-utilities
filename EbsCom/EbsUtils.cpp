// EbsUtils.cpp


#include "stdafx.h"
#include "GnuType.h"
#include "Ebs.h"
#include "GnuMisc.h"
#include "EbsUtils.h"



// --- GetItem() ----------------------------------------------------
/*
 *  Returns the index to the n'th item (1 based).
 *  bSection determines whether n'th section or n'th item is wanted.
 */
INT GetItem(PPROP pProp, INT iItemNum, BOOL bSection)
	{
	INT i,j;

	for (j=i=0; i<pProp->ItemCount (); i++)
		{
		BOOL bIsSection = IsFlag(pProp->GetItem (i)->m_wFlags, fSECTION);
		j += ((bIsSection && bSection) || (!bIsSection && !bSection)) ? 1 : 0;
		if (iItemNum == j)
			return i;
		}
	return ItemsEnumEnd;
	}



// --- GetNextSection() ---------------------------------------------
/*	 
 *  returns the index of the next section after index iSectionIndex
 */
INT GetNextSection(PPROP pProp, INT iSectionIndex)
{
	for (INT index = iSectionIndex; index < pProp->ItemCount (); index++)
	{
		if (IsFlag(pProp->GetItem (index)->m_wFlags, fSECTION))
			return index;
	}
	return ItemsEnumEnd;
}


// --- GetNextItem() ------------------------------------------------
/*	 
 *  returns the index of the next section after index iItemIndex
 */
INT GetNextItem(PPROP pProp, INT iItemIndex)
{
	if (   (iItemIndex > 0)
		 && (iItemIndex < pProp->ItemCount ())
		 && !IsFlag(pProp->GetItem (iItemIndex)->m_wFlags, fSECTION))
		return iItemIndex;
	else
		return ItemsEnumEnd;
}


// --- GetNextProposal() ---------------------------------------------
/*	 
 *  returns the index of the next proposal after index iProposalIndex
 */
INT GetNextProposal(PPROP pProp, INT iProposalIndex)
{
	return ItemsEnumEnd;
}

