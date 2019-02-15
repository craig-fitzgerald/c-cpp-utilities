// DBEItem.cpp : Implementation of CEbsComApp and DLL registration.

#include "stdafx.h"
#include "EbsCom.h"
#include "DBEItem.h"
#include "GnuMem.h"
#include "ComUtil.h"


/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP EbsDBEItem::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEbsDBEItem,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


// --- get_Index() --------------------------------------------------
// 
STDMETHODIMP EbsDBEItem::get_Index(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_pItem->m_i;

	return S_OK;
}


// --- get_Quantity() -----------------------------------------------
// 
STDMETHODIMP EbsDBEItem::get_Quantity(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_pItem->m_fQuan;

	return S_OK;
}


// --- put_Quantity() -----------------------------------------------
// 
STDMETHODIMP EbsDBEItem::put_Quantity(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_pItem->m_fQuan = newVal;
	// TODO: recalc

	return S_OK;
}


// --- get_UnitPrice() ----------------------------------------------
// 
STDMETHODIMP EbsDBEItem::get_UnitPrice(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_pItem->m_fUP;

	return S_OK;
}


// --- put_UnitPrice() ----------------------------------------------
// 
STDMETHODIMP EbsDBEItem::put_UnitPrice(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_pItem->m_fUP = newVal;
	// TODO: recalc

	return S_OK;
}


// --- get_Note() ---------------------------------------------------
// 
STDMETHODIMP EbsDBEItem::get_Note(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pItem->m_pszNote);

	return S_OK;
}


// --- putref_Note() ------------------------------------------------
// 
STDMETHODIMP EbsDBEItem::putref_Note(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	MemFreeData(m_pItem->m_pszNote);
	m_pItem->m_pszNote = _com_util::ConvertBSTRToString(newVal);

	return S_OK;
}

STDMETHODIMP EbsDBEItem::get_Extension(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_pItem->m_fExt;

	return S_OK;
}

STDMETHODIMP EbsDBEItem::put_Extension(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_pItem->m_fExt = newVal;
	// TODO: recalc

	return S_OK;
}
