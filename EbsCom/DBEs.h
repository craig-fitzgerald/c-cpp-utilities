// DBEs.h: Definition of the DBEs class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBES_H__0111E798_D32B_11D3_B4D8_005004D39EC7__INCLUDED_)
#define AFX_DBES_H__0111E798_D32B_11D3_B4D8_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "GnuType.h"
#include "ebs.h"


/////////////////////////////////////////////////////////////////////////////
// EbsDBEs

class EbsDBEs : 
	public IDispatchImpl<IEbsDBEs, &IID_IEbsDBEs, &LIBID_EBSCOMLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<EbsDBEs,&CLSID_EbsDBEs>
{
	PPROP		m_pProp;
	CWinMeta*		m_pwm;
	LONG		m_lCount;

public:
	EbsDBEs() : m_pProp(NULL), m_pwm(NULL), m_lCount(0) {}

	void	Init(PPROP pProp, CWinMeta* pwm);


BEGIN_COM_MAP(EbsDBEs)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IEbsDBEs)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(EbsDBEs) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_EbsDBEs)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IEbsDBEs
public:
	STDMETHOD(get__NewEnum)(/*[out, retval]*/ IUnknown* *pVal);
	STDMETHOD(Item)(/*[in]*/ long Index, /*[out, retval]*/ VARIANT* retval);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
};

typedef CComObject<EbsDBEs>		CComDBEs;


#endif // !defined(AFX_DBES_H__0111E798_D32B_11D3_B4D8_005004D39EC7__INCLUDED_)
