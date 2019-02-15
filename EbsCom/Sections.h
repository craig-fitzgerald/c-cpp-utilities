// Sections.h: Definition of the Sections class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SECTIONS_H__942E2BF8_D003_11D3_B4D5_005004D39EC7__INCLUDED_)
#define AFX_SECTIONS_H__942E2BF8_D003_11D3_B4D5_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "ebs.h"


/////////////////////////////////////////////////////////////////////////////
// EbsSections

class EbsSections : 
	public IDispatchImpl<IEbsSections, &IID_IEbsSections, &LIBID_EBSCOMLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<EbsSections,&CLSID_EbsSections>
{
	PPROP		m_pProp;
	LONG		m_lSectionsCount;

public:
	EbsSections() : m_pProp(NULL), m_lSectionsCount(0) {}

	void	Init(PPROP pProp, LONG lSectionsCount);

BEGIN_COM_MAP(EbsSections)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IEbsSections)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(EbsSections) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_EbsSections)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IEbsSections
public:
	STDMETHOD(get__NewEnum)(/*[out, retval]*/ IUnknown* *pVal);
	STDMETHOD(Item)(/*[in]*/ long Index, /*[out, retval]*/ VARIANT* retval);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
};

typedef CComObject<EbsSections>		CComSections;


#endif // !defined(AFX_SECTIONS_H__942E2BF8_D003_11D3_B4D5_005004D39EC7__INCLUDED_)
