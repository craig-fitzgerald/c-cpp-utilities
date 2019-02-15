// DBE.cpp : Implementation of CEbsComApp and DLL registration.

#include "stdafx.h"
#include "EbsCom.h"
#include "DBE.h"
#include "DBEItems.h"


/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP EbsDBE::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEbsDBE,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}



STDMETHODIMP EbsDBE::get_DBEItems(LPDISPATCH *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = NULL;
	CComDBEItems* pDBEItems = new CComDBEItems;
	pDBEItems->Init(m_pDBE->m_ppdi);
	return pDBEItems->QueryInterface(IID_IDispatch, (void**) pVal);
}



STDMETHODIMP EbsDBE::get_Name(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP EbsDBE::put_Name(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP EbsDBE::get_UsedAs(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP EbsDBE::put_UsedAs(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP EbsDBE::get_IsBid(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP EbsDBE::put_IsBid(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP EbsDBE::get_ID(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP EbsDBE::put_ID(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP EbsDBE::get_Amount(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP EbsDBE::put_Amount(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}


void EbsDBE::SetFieldData(long lField)
{
// This code isn't appropriate anyway ...
//#if 0
//	if ((lField < CTLID_FIRST) || (lField >= CTLID_FIRST + pwm->FieldCount ()))
//		return;
//
//	CCellData* pcd  = pwm->GetRow (m_iIndex)->m_pcd;  // pcd here is row of user data
//	int 	 iCol  = uID - CTLID_FIRST;
//	CFieldInfo*  pFldInfo = pwm->GetField (iCol);
//
//	CBidTreeView *pTree = (CBidTreeView *)((CBidView *)GetParent())->GetTreeView();
//
//	switch (pFldInfo->m_iDataType)
//		{
//		case FLDDT_BOOL  :
//			{
//			CComboBox *pCtl = (CComboBox *)m_ppCtl[iCol];
//
//			if ((pcd + iCol)->m_i == pCtl->GetCurSel ()) // no change?
//				break;
//
//			GetDocument ()->SetModifiedFlag ();
//			//SetFlag (&pProp->m_wFlags, DIRTY, 1);
//
//			(pcd + iCol)->m_i = pCtl->GetCurSel ();  // save to original data structures
//			pCtl->GetWindowText (sz, uSZBUFSIZE);
//			StrReplace (&(pcd + iCol)->m_psz, sz); 
//			break;
//			}
//
//		case FLDDT_LIST  :
//		case FLDDT_EDLIST:
//			{
//			CComboBox *pCtl = (CComboBox *)m_ppCtl[iCol];
//
//			int iSel = pCtl->GetCurSel ();
//
//			if (((pcd + iCol)->m_i == iSel) && iSel != CB_ERR) // no change?
//				break;
//
//			if (iSel == CB_ERR && IsFlag (pFldInfo->m_wFlags, FLDD_SMARTMATCH))
//				{
//				if (SelectionInList (pCtl))
//					iSel = pCtl->GetCurSel ();
//				}
//
//			GetDocument ()->SetModifiedFlag ();
//			//SetFlag (&pProp->m_wFlags, DIRTY, 1);
//
//			if (iSel == CB_ERR) // if no selection get from edit ctl
//				pCtl->GetWindowText (sz, uSZBUFSIZE);
//			else
//				pCtl->GetLBText (iSel, sz);
//			StrReplace (&(pcd + iCol)->m_psz, sz); // save string to original data structures
//
//			for (int i=0; i<pwm->FieldCount (); i++)
//				{
//				CFieldInfo* pFldInfoTmp = pwm->GetField (i);
//
//				if (pFldInfoTmp->m_iDataType != FLDDT_LIST && 
//					 pFldInfoTmp->m_iDataType != FLDDT_EDLIST)
//					continue;
//
//				pCtl = (CComboBox *)m_ppCtl[i];
//
//				if (i != iCol && stricmp (pFldInfo->m_pszReferent, pFldInfoTmp->m_pszReferent))  // only change linked combo's
//					continue;
//
//				/*--- set selection of linked controls ---*/
//				if (i != iCol) 
//					{
//					int iOldSel = pCtl->GetCurSel ();
//					if (iSel != iOldSel)
//						pCtl->SetCurSel (iSel);
//					}
//
//				/*--- save control data to datasource ---*/
//				(pcd + i)->m_i = (iSel == CB_ERR ? -1 : iSel);
//				if (iSel == CB_ERR)
//					pCtl->GetWindowText (sz, uSZBUFSIZE);
//				else
//					pCtl->GetLBText (iSel, sz);
//				StrReplace (&(pcd + i)->m_psz, sz); // save string to original data structures
//
//				/*--- repaint tree if changing DBE name ---*/
//				if (i == ((CBidDoc *)GetDocument ())->m_iDBENameCol)
//					pTree->RedrawWindow ();
//				}
//			break;
//			}
//
//		case FLDDT_NUMBER:
//		case FLDDT_MONEY :
//		case FLDDT_DATE  :
//		case FLDDT_TEXT  :
//			{
//			CEdit *pEdit = (CEdit *)m_ppCtl[iCol];
//
//			if (VerifyFieldData (pEdit, pFldInfo->m_iDataType, pFldInfo->m_iXSize pProp))
//				{
//				Beep (1000, 100);
//				pEdit->SetFocus (); // problem, stay right here
//				}
//			else
//				{
//				pEdit->GetWindowText (sz, uSZBUFSIZE);
//
//				if ((pcd + iCol)->m_psz && !strcmp (sz, (pcd + iCol)->m_psz)) // no change ?
//					break;
//
//				//SetFlag (&pProp->m_wFlags, DIRTY, 1);
//				GetDocument ()->SetModifiedFlag ();
//
//
//				StrReplace (&(pcd + iCol)->m_psz, sz); // save string to data structures
//				if (iCol == ((CBidDoc *)GetDocument ())->m_iDBENameCol) // repaint tree if changing DBE name
//					pTree->RedrawWindow ();
//				}
//			break;
//		}
//	}
//#endif
}

