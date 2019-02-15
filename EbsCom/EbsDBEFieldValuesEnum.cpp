// EbsDBEFieldValuesEnum.cpp : Implementation of CEbsComApp and DLL registration.

#include "stdafx.h"
#include "EbsCom.h"
#include "EbsDBEFieldValuesEnum.h"

/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP EbsDBEFieldValuesEnum::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEbsDBEFieldValuesEnum,
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
STDMETHODIMP EbsDBEFieldValuesEnum::Next(ULONG celt, VARIANT* rgvar, ULONG* peltFetched)
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
#if 0
	if (m_lIndex >= m_lCount)
		return ResultFromScode( S_FALSE );

	// Retrieve the next celt elements.    
	for (l = 0 ; (m_lIndex < m_lCount) && (celt != 0) ; l++, m_lIndex++)    
	{
		celt-- ;

		// Create object ---
		CComDBE* pComDBE = new CComDBE;
		if (!pComDBE)
		{
			// TODO: delete array of com items ---
			delete pComDBE;
			return E_FAIL;
		}
		pComDBE->Init(&m_pwm->m_pRowData[m_lIndex]);

		// Assign interface ---
		LPDISPATCH pDisp;
		pComDBE->QueryInterface(IID_IDispatch, (void**)&pDisp);
		rgvar[l].vt = VT_DISPATCH ;
		rgvar[l].m_pdispVal = pDisp;
		if (peltFetched != NULL)                
			(*peltFetched)++ ;
	}
#endif
	HRESULT hr = NOERROR ;
	if (celt != 0)       
	  hr = ResultFromScode( S_FALSE ) ;    
	return hr ;
}


// --- Skip() -------------------------------------------------------
// 
STDMETHODIMP EbsDBEFieldValuesEnum::Skip(ULONG celt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

//	while ((m_lIndex < m_lCount) && celt--)
//		m_lIndex++;  
	
	return (celt == 0 ? NOERROR : ResultFromScode( S_FALSE )) ;
}


// --- Reset() ------------------------------------------------------
// 
STDMETHODIMP EbsDBEFieldValuesEnum::Reset(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

//	m_lIndex = 0;
	return NOERROR ;
}


// --- Clone() ------------------------------------------------------
// 
STDMETHODIMP EbsDBEFieldValuesEnum::Clone(IEnumVARIANT** ppEnum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (ppEnum == NULL)
		return E_POINTER;
	*ppEnum = NULL;
#if 0	
	CComDBEsEnum* pEnum = new CComDBEsEnum;
	pEnum->Init(m_pwm, m_lCount);
	HRESULT hRes = pEnum->QueryInterface(IID_IEnumVARIANT, (void**)ppEnum);

	if (FAILED(hRes))
	{
		delete pEnum;
		return hRes;
	}
#endif
	return S_OK;
}

