// Items.cpp : Implementation of CEbsComApp and DLL registration.

#include "stdafx.h"
#include "EbsCom.h"
#include "GnuType.h"
#include "GnuMisc.h"
#include "ebs.h"
#include "ComUtil.h"
#include "Items.h"
#include "Item.h"
#include "EbsUtils.h"
#include "ItemsEnum.h"


/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP EbsItems::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEbsItems,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


// --- Init() -------------------------------------------------------
// 
void EbsItems::Init(PPROP pProp, LONG lSectionIndex)
{
	m_pProp         = pProp;
	m_lItemsCount   = 0;
	m_lSectionIndex = lSectionIndex;

	// Count number of items in section ---
	if (m_pProp)
	{
		for (LONG index = lSectionIndex+1 ; 
			  (index < m_pProp->ItemCount ()) && !ISection(m_pProp->GetItem (index)); 
			  index++)
				m_lItemsCount++;
	}
}


// --- get_Count() --------------------------------------------------
// 
STDMETHODIMP EbsItems::get_Count(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_lItemsCount;

	return S_OK;
}


// --- Item() -------------------------------------------------------
// 
STDMETHODIMP EbsItems::Item(long Index, VARIANT *retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (retval == NULL)
		return E_POINTER;

	VariantInit(retval);
	retval->vt = VT_UNKNOWN;
	retval->m_punkVal = NULL;

	// use 1-based index, VB like ---
	if ((Index < 1) || (Index > m_lItemsCount))
		return E_INVALIDARG;

	CComItem* pComItem = new CComItem;
	if (!pComItem)
		return E_FAIL;

	pComItem->Init(m_pProp, Index + m_lSectionIndex, m_lSectionIndex);

	LPDISPATCH pDisp;
	pComItem->QueryInterface(IID_IDispatch, (void**)&pDisp);
	retval->vt = VT_DISPATCH;
	retval->m_pdispVal = pDisp;

	return S_OK;
}


// --- get__NewEnum() -----------------------------------------------
// 
STDMETHODIMP EbsItems::get__NewEnum(IUnknown **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pVal == NULL)
		return E_POINTER;
	*pVal = NULL;
	
	CComItemsEnum* pEnum = new CComItemsEnum;
	pEnum->Init(m_pProp, m_lSectionIndex);
	HRESULT hRes = pEnum->QueryInterface(IID_IEnumVARIANT, (void**)pVal);

	if (FAILED(hRes))
	{
		delete pEnum;
		return hRes;
	}

	return S_OK;
}
