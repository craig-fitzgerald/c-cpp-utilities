// EbsDBEFieldValuesEnum.h: Definition of the EbsDBEFieldValuesEnum class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EBSDBEFIELDVALUESENUM_H__2D386F74_D3F3_11D3_B4D9_005004D39EC7__INCLUDED_)
#define AFX_EBSDBEFIELDVALUESENUM_H__2D386F74_D3F3_11D3_B4D9_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// EbsDBEFieldValuesEnum

class EbsDBEFieldValuesEnum : 
	public IDispatchImpl<IEbsDBEFieldValuesEnum, &IID_IEbsDBEFieldValuesEnum, &LIBID_EBSCOMLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<EbsDBEFieldValuesEnum,&CLSID_EbsDBEFieldValuesEnum>,
	IEnumVARIANT
{
public:
	EbsDBEFieldValuesEnum() {}
BEGIN_COM_MAP(EbsDBEFieldValuesEnum)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IEbsDBEFieldValuesEnum)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IEnumVARIANT)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(EbsDBEFieldValuesEnum) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_EbsDBEFieldValuesEnum)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	STDMETHOD(Next)(ULONG celt, VARIANT* rgelt, ULONG* peltFetched);
	STDMETHOD(Skip)(ULONG celt);
	STDMETHOD(Reset)(void);
	STDMETHOD(Clone)(IEnumVARIANT** ppEnum);

// IEbsDBEFieldValuesEnum
public:
};

#endif // !defined(AFX_EBSDBEFIELDVALUESENUM_H__2D386F74_D3F3_11D3_B4D9_005004D39EC7__INCLUDED_)
