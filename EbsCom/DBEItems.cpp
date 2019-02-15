// DBEItems.cpp : Implementation of CEbsComApp and DLL registration.

#include "stdafx.h"
#include "EbsCom.h"
#include "DBEItems.h"
#include "DBEItem.h"
#include "DBEItemsEnum.h"


/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP EbsDBEItems::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEbsDBEItems,
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
void EbsDBEItems::Init(CDBEItem** pItems)
{
	m_pItems = pItems; 

	for (m_lCount=0; m_pItems && m_pItems[m_lCount]; m_lCount++) ;
}


// --- get_Count() --------------------------------------------------
// 
STDMETHODIMP EbsDBEItems::get_Count(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_lCount;

	return S_OK;
}


// --- Item() -------------------------------------------------------
// 
STDMETHODIMP EbsDBEItems::Item(long Index, VARIANT *retval)
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

	CComDBEItem* pComDBEItem = new CComDBEItem;
	if (!pComDBEItem)
		return E_FAIL;
	pComDBEItem->Init(m_pItems[Index]);

	LPDISPATCH pDisp;
	pComDBEItem->QueryInterface(IID_IDispatch, (void**)&pDisp);
	retval->vt = VT_DISPATCH;
	retval->m_pdispVal = pDisp;

	return S_OK;
}


// --- get__NewEnum() -----------------------------------------------
// 
STDMETHODIMP EbsDBEItems::get__NewEnum(IUnknown **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pVal == NULL)
		return E_POINTER;
	*pVal = NULL;
	
	CComDBEItemsEnum* pEnum = new CComDBEItemsEnum;
	pEnum->Init(m_pItems, m_lCount);
	HRESULT hRes = pEnum->QueryInterface(IID_IEnumVARIANT, (void**)pVal);

	if (FAILED(hRes))
	{
		delete pEnum;
		return hRes;
	}

	return S_OK;
}
