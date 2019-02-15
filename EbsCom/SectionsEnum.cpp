// SectionsEnum.cpp : Implementation of CEbsComApp and DLL registration.

#include "stdafx.h"
#include "EbsCom.h"
#include "GnuType.h"
#include "EbsUtils.h"
#include "SectionsEnum.h"
#include "Section.h"


/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP EbsSectionsEnum::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEbsSectionsEnum,
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
STDMETHODIMP EbsSectionsEnum::Next(ULONG celt, VARIANT* rgvar, ULONG* peltFetched)
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

	if (m_lSectionIndex == ItemsEnumEnd)
		return ResultFromScode( S_FALSE );

	// Retrieve the next celt elements.    
	for (l = 0 ; (m_lSectionIndex < m_pProp->ItemCount ()) && (celt != 0) ; l++)    
	{
		m_lSectionIndex = GetNextSection(m_pProp, m_lSectionIndex+1);
		if (m_lSectionIndex == ItemsEnumEnd)
			return ResultFromScode( S_FALSE );

		celt-- ;

		// Create object ---
		CComSection* pComSection = new CComSection;
		if (!pComSection)
		{
			return E_FAIL;
		}
		pComSection->Init(m_pProp, m_lSectionIndex);

		// Assign interface ---
		LPDISPATCH pDisp;
		pComSection->QueryInterface(IID_IDispatch, (void**)&pDisp);
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
STDMETHODIMP EbsSectionsEnum::Skip(ULONG celt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	while ((m_lSectionIndex != ItemsEnumEnd) && celt--)
		m_lSectionIndex = GetNextSection(m_pProp, m_lSectionIndex+1);  
	
	return (celt == 0 ? NOERROR : ResultFromScode( S_FALSE )) ;
}


// --- Reset() ------------------------------------------------------
// 
STDMETHODIMP EbsSectionsEnum::Reset(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_lSectionIndex = -1;
    return NOERROR ;
}


// --- Clone() ------------------------------------------------------
// 
STDMETHODIMP EbsSectionsEnum::Clone(IEnumVARIANT** ppEnum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (ppEnum == NULL)
		return E_POINTER;
	*ppEnum = NULL;
	
	CComSectionsEnum* pEnum = new CComSectionsEnum;
	pEnum->Init(m_pProp);
	HRESULT hRes = pEnum->QueryInterface(IID_IEnumVARIANT, (void**)ppEnum);

	if (FAILED(hRes))
	{
		delete pEnum;
		return hRes;
	}

	return S_OK;
}


