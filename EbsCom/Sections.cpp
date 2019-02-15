// Sections.cpp : Implementation of CEbsComApp and DLL registration.

#include "stdafx.h"
#include "EbsCom.h"
#include "gnutype.h"
#include "gnumisc.h"
#include "Sections.h"
#include "Section.h"
#include "EbsUtils.h"
#include "SectionsEnum.h"


/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP EbsSections::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEbsSections,
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
void EbsSections::Init(PPROP pProp, LONG lSectionsCount)
{
	m_pProp = pProp;
	m_lSectionsCount = lSectionsCount;
}


// --- get_Count() --------------------------------------------------
// 
STDMETHODIMP EbsSections::get_Count(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_lSectionsCount;

	return S_OK;
}


// --- Item() -------------------------------------------------------
// 
STDMETHODIMP EbsSections::Item(long Index, VARIANT *retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (retval == NULL)
		return E_POINTER;

	VariantInit(retval);
	retval->vt = VT_UNKNOWN;
	retval->m_punkVal = NULL;

	// use 1-based index, VB like ---
	if ((Index < 1) || (Index > m_lSectionsCount))
		return E_INVALIDARG;

	INT nSectPos = GetItem(m_pProp, Index, /* bSection */ TRUE);
	if (nSectPos < 0)
		return S_OK;

	CComSection* pComSection = new CComSection;
	if (!pComSection)
		return E_FAIL;
	pComSection->Init(m_pProp, nSectPos);

	LPDISPATCH pDisp;
	pComSection->QueryInterface(IID_IDispatch, (void**)&pDisp);
	retval->vt = VT_DISPATCH;
	retval->m_pdispVal = pDisp;

	return S_OK;
}


// --- get__NewEnum() -----------------------------------------------
// 
STDMETHODIMP EbsSections::get__NewEnum(IUnknown **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pVal == NULL)
		return E_POINTER;
	*pVal = NULL;
	
	CComSectionsEnum* pEnum = new CComSectionsEnum;
	pEnum->Init(m_pProp);
	HRESULT hRes = pEnum->QueryInterface(IID_IEnumVARIANT, (void**)pVal);

	if (FAILED(hRes))
	{
		delete pEnum;
		return hRes;
	}

	return S_OK;
}


