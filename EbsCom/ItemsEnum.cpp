// ItemsEnum.cpp : Implementation of CEbsComApp and DLL registration.

#include "stdafx.h"
#include "EbsCom.h"
#include "GnuType.h"
#include "Ebs.h"
#include "EbsUtils.h"
#include "ItemsEnum.h"
#include "Item.h"


/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP EbsItemsEnum::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEbsItemsEnum,
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
void EbsItemsEnum::Init(PPROP pProp, LONG lSectionIndex)	
{
	m_pProp = pProp; 
	m_lSectionIndex = lSectionIndex;
	m_lItemIndex = lSectionIndex;
}


// --- Next() -------------------------------------------------------
// 
STDMETHODIMP EbsItemsEnum::Next(ULONG celt, VARIANT* rgvar, ULONG* peltFetched)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// peltFetched can legally == 0
	//                                               
	if (peltFetched != NULL)
		*peltFetched = 0;    
	else if (celt > 1)    
	{   
		return ResultFromScode( E_INVALIDARG ) ;       
	}

	ULONG   l ;
	for (l=0; l < celt; l++)        
	VariantInit( &rgvar[l] ) ;

	if (m_lItemIndex == ItemsEnumEnd)
		return ResultFromScode( E_UNEXPECTED );

	// Retrieve the next celt elements.    
	for (l = 0 ; (m_lItemIndex < m_pProp->ItemCount ()) && (celt != 0) ; l++)    
	{
		m_lItemIndex = GetNextItem(m_pProp, m_lItemIndex+1);
		if (m_lItemIndex == ItemsEnumEnd)
			return ResultFromScode( S_FALSE );

		celt-- ;

		// Create object ---
		CComItem* pComItem = new CComItem;
		if (!pComItem)
		{
			return E_FAIL;
		}
		pComItem->Init(m_pProp, m_lItemIndex, m_lSectionIndex);

		// Assign interface ---
		LPDISPATCH pDisp;
		pComItem->QueryInterface(IID_IDispatch, (void**)&pDisp);
		rgvar[l].vt = VT_DISPATCH ;
		rgvar[l].m_pdispVal = pDisp;
		if (peltFetched != NULL)                
			(*peltFetched)++ ;
	}

	HRESULT hr = NOERROR ;
	if (celt != 0)       
	  hr = ResultFromScode( S_FALSE ) ;    
	return hr ;
}


// --- Skip() -------------------------------------------------------
// 
STDMETHODIMP EbsItemsEnum::Skip(ULONG celt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	while ((m_lItemIndex != ItemsEnumEnd) && celt--)
		m_lItemIndex = GetNextItem(m_pProp, m_lItemIndex+1);
	
	return (celt == 0 ? NOERROR : ResultFromScode( S_FALSE )) ;
}


// --- Reset() ------------------------------------------------------
// 
STDMETHODIMP EbsItemsEnum::Reset(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_lItemIndex = -1;
    return NOERROR ;
}


// --- Clone() ------------------------------------------------------
// 
STDMETHODIMP EbsItemsEnum::Clone(IEnumVARIANT** ppEnum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (ppEnum == NULL)
		return E_POINTER;
	*ppEnum = NULL;
	
	CComItemsEnum* pEnum = new CComItemsEnum;
	pEnum->Init(m_pProp, m_lSectionIndex);
	HRESULT hRes = pEnum->QueryInterface(IID_IEnumVARIANT, (void**)ppEnum);

	if (FAILED(hRes))
	{
		delete pEnum;
		return hRes;
	}

	return S_OK;
}


