// EbsDBEFieldValue.h: Definition of the EbsDBEFieldValue class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EBSDBEFIELDVALUE_H__2D386F77_D3F3_11D3_B4D9_005004D39EC7__INCLUDED_)
#define AFX_EBSDBEFIELDVALUE_H__2D386F77_D3F3_11D3_B4D9_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// EbsDBEFieldValue

class EbsDBEFieldValue : 
	public IDispatchImpl<IEbsDBEFieldValue, &IID_IEbsDBEFieldValue, &LIBID_EBSCOMLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<EbsDBEFieldValue,&CLSID_EbsDBEFieldValue>
{
public:
	EbsDBEFieldValue() {}
BEGIN_COM_MAP(EbsDBEFieldValue)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IEbsDBEFieldValue)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(EbsDBEFieldValue) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_EbsDBEFieldValue)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IEbsDBEFieldValue
public:
};

#endif // !defined(AFX_EBSDBEFIELDVALUE_H__2D386F77_D3F3_11D3_B4D9_005004D39EC7__INCLUDED_)
