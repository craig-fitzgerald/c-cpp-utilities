// EbsDBEFieldValues.cpp : Implementation of CEbsComApp and DLL registration.

#include "stdafx.h"
#include "EbsCom.h"
#include "EbsDBEFieldValues.h"
#include "EbsDBEFieldValuesEnum.h"


/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP EbsDBEFieldValues::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEbsDBEFieldValues,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


// --- get_Count() --------------------------------------------------
// 
STDMETHODIMP EbsDBEFieldValues::get_Count(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = 0;

	return S_OK;
}


// --- Item() -------------------------------------------------------
// 
STDMETHODIMP EbsDBEFieldValues::Item(long Index, VARIANT *retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (retval == NULL)
		return E_POINTER;

	VariantInit(retval);
	retval->vt = VT_UNKNOWN;
	retval->m_punkVal = NULL;

	// use 1-based index, VB like ---
	if ((Index < 1) || (Index > m_lCount))
		return E_INVALIDARG;
#if 0
	CComDBEItem* pComDBEItem = new CComDBEItem;
	if (!pComDBEItem)
		return E_FAIL;
	pComDBEItem->Init(m_pItems[Index]);

	LPDISPATCH pDisp;
	pComDBEItem->QueryInterface(IID_IDispatch, (void**)&pDisp);
	retval->vt = VT_DISPATCH;
	retval->m_pdispVal = pDisp;
#endif
	return S_OK;
}


// --- get__NewEnum() -----------------------------------------------
// 
STDMETHODIMP EbsDBEFieldValues::get__NewEnum(IUnknown **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
#if 0
	CComDBEItemsEnum* pEnum = new CComDBEItemsEnum;
	pEnum->Init(m_pItems, m_lCount);
	HRESULT hRes = pEnum->QueryInterface(IID_IEnumVARIANT, (void**)pVal);

	if (FAILED(hRes))
	{
		delete pEnum;
		return hRes;
	}
#endif
	return S_OK;
}
