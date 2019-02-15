// DBEItemsEnum.cpp : Implementation of CEbsComApp and DLL registration.

#include "stdafx.h"
#include "EbsCom.h"
#include "DBEItemsEnum.h"
#include "DBEItem.h"


/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP EbsDBEItemsEnum::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEbsDBEItemsEnum,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


// --- Next() -------------------------------------------------------
// 
STDMETHODIMP EbsDBEItemsEnum::Next(ULONG celt, VARIANT* rgvar, ULONG* peltFetched)
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

	ULONG l;
	for (l=0; l < celt; l++)        
	VariantInit( &rgvar[l] ) ;

	if (m_lIndex >= m_lCount)
		return ResultFromScode( S_FALSE );

	// Retrieve the next celt elements.    
	for (l = 0 ; (m_lIndex < m_lCount) && (celt != 0) ; l++, m_lIndex++)    
	{
		celt-- ;

		// Create object ---
		CComDBEItem* pComDBEItem = new CComDBEItem;
		if (!pComDBEItem)
		{
			// TODO: delete array of com items ---
			delete pComDBEItem;
			return E_FAIL;
		}
		pComDBEItem->Init(m_pItems[m_lIndex]);

		// Assign interface ---
		LPDISPATCH pDisp;
		pComDBEItem->QueryInterface(IID_IDispatch, (void**)&pDisp);
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
STDMETHODIMP EbsDBEItemsEnum::Skip(ULONG celt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	while ((m_lIndex < m_lCount) && celt--)
		m_lIndex++;  
	
	return (celt == 0 ? NOERROR : ResultFromScode( S_FALSE )) ;
}


// --- Reset() ------------------------------------------------------
// 
STDMETHODIMP EbsDBEItemsEnum::Reset(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_lIndex = 0;
	return NOERROR ;
}


// --- Clone() ------------------------------------------------------
// 
STDMETHODIMP EbsDBEItemsEnum::Clone(IEnumVARIANT** ppEnum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (ppEnum == NULL)
		return E_POINTER;
	*ppEnum = NULL;
	
	CComDBEItemsEnum* pEnum = new CComDBEItemsEnum;
	pEnum->Init(m_pItems, m_lCount);
	HRESULT hRes = pEnum->QueryInterface(IID_IEnumVARIANT, (void**)ppEnum);

	if (FAILED(hRes))
	{
		delete pEnum;
		return hRes;
	}

	return S_OK;
}

