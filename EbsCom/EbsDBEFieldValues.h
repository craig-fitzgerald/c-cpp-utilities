// EbsDBEFieldValues.h: Definition of the EbsDBEFieldValues class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EBSDBEFIELDVALUES_H__2D386F71_D3F3_11D3_B4D9_005004D39EC7__INCLUDED_)
#define AFX_EBSDBEFIELDVALUES_H__2D386F71_D3F3_11D3_B4D9_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// EbsDBEFieldValues

class EbsDBEFieldValues : 
	public IDispatchImpl<IEbsDBEFieldValues, &IID_IEbsDBEFieldValues, &LIBID_EBSCOMLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<EbsDBEFieldValues,&CLSID_EbsDBEFieldValues>
{
		LONG	m_lCount;

public:
	EbsDBEFieldValues() : m_lCount(0) {}

BEGIN_COM_MAP(EbsDBEFieldValues)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IEbsDBEFieldValues)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(EbsDBEFieldValues) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_EbsDBEFieldValues)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IEbsDBEFieldValues
public:
	STDMETHOD(get__NewEnum)(/*[out, retval]*/ IUnknown* *pVal);
	STDMETHOD(Item)(/*[in]*/ long Index, /*[out, retval]*/ VARIANT* retval);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
};

#endif // !defined(AFX_EBSDBEFIELDVALUES_H__2D386F71_D3F3_11D3_B4D9_005004D39EC7__INCLUDED_)
