// DBEs.cpp : Implementation of CEbsComApp and DLL registration.

#include "stdafx.h"
#include "EbsCom.h"
#include "DBEs.h"
#include "DBE.h"
#include "DBEsEnum.h"


/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP EbsDBEs::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEbsDBEs,
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
void EbsDBEs::Init(PPROP pProp, CWinMeta* pwm)
{
	m_pProp = pProp;
	m_pwm = pwm;

	// Number of entries ? ---
	for (m_lCount = 0 ; pwm->GetRow (m_lCount)->m_ppdi ; m_lCount++)	;
}


// --- get_Count() --------------------------------------------------
// 
STDMETHODIMP EbsDBEs::get_Count(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_lCount;

	return S_OK;
}


// --- Item() -------------------------------------------------------
// 
STDMETHODIMP EbsDBEs::Item(long Index, VARIANT *retval)
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

	CComDBE* pwmDBE = new CComDBE;
	if (!pwmDBE)
		return E_FAIL;
	pwmDBE->Init(m_pProp, m_pwm, &m_pwm->GetRow (Index));

	LPDISPATCH pDisp;
	pwmDBE->QueryInterface(IID_IDispatch, (void**)&pDisp);
	retval->vt = VT_DISPATCH;
	retval->m_pdispVal = pDisp;

	return S_OK;
}


// --- get__NewEnum() -----------------------------------------------
// 
STDMETHODIMP EbsDBEs::get__NewEnum(IUnknown **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pVal == NULL)
		return E_POINTER;
	*pVal = NULL;
	
	CComDBEsEnum* pEnum = new CComDBEsEnum;
	pEnum->Init(m_pProp, m_pwm, m_lCount);
	HRESULT hRes = pEnum->QueryInterface(IID_IEnumVARIANT, (void**)pVal);

	if (FAILED(hRes))
	{
		delete pEnum;
		return hRes;
	}

	return S_OK;
}
