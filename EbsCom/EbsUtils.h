// EbsUtils.h




#ifndef EBS_UTILS_H
#define EBS_UTILS_H

#include "Ebs.h"



typedef enum {ItemsEnumBegin = -1, ItemsEnumEnd = -2}	ItemsEnumPos;

	
INT GetItem(PPROP pProp, INT iItemNum, BOOL bSection);


// Returns the index of the next section after index iSectionIndex.
INT GetNextSection(PPROP pProp, INT iSectionIndex);

// Returns the index of the next item after index iItemIndex.
INT GetNextItem(PPROP pProp, INT iItemIndex);

// Returns the index of the next proposal after index iProposalIndex
INT GetNextProposal(PPROP pProp, INT iProposalIndex);



// Do not write anything after this line ---
#endif // EBS_UTILS_H
