// Section.cpp : Implementation of CEbsComApp and DLL registration.

#include "stdafx.h"
#include "EbsCom.h"
#include "gnutype.h"
#include "Section.h"
#include "ComUtil.h"
#include "GnuMem.h"
#include "GnuMisc.h"
#include "Items.h"
#include "ebs.h"


/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP EbsSection::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEbsSection,
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
void EbsSection::Init(PPROP pProp, LONG SectionIndex)
{
	m_pProp = pProp;
	m_lIndex = SectionIndex;
}


// --- get_Number() -------------------------------------------------
// 
STDMETHODIMP EbsSection::get_Number(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->GetItem (m_lIndex)->m_pszLineNum);

	return S_OK;
}


// --- get_AltCode() ------------------------------------------------
// 
STDMETHODIMP EbsSection::get_AltCode(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->GetItem (m_lIndex)->m_pszAlt);

	return S_OK;
}


// --- get_Description() --------------------------------------------
// 
STDMETHODIMP EbsSection::get_Description(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->GetItem (m_lIndex)->m_pszLongDesc);

	return S_OK;
}


// --- get_Total() --------------------------------------------------
// 
STDMETHODIMP EbsSection::get_Total(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_pProp->GetItem (m_lIndex)->m_fExt;

	return S_OK;
}


// --- get_Items() --------------------------------------------------
// 
STDMETHODIMP EbsSection::get_Items(LPDISPATCH *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComItems* pItems = new CComItems;
	pItems->Init(m_pProp, m_lIndex);
	return pItems->QueryInterface(IID_IDispatch, (void**) pVal);
}



STDMETHODIMP EbsSection::get_IsAlternate(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = ( IIsAlt(m_pProp->GetItem (m_lIndex)) ) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP EbsSection::get_IsOption(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = ( IIsOpt(m_pProp->GetItem (m_lIndex)) ) ? VARIANT_TRUE : VARIANT_FALSE;
    
	return S_OK;
}
