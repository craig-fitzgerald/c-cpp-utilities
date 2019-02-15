// Items.h: Definition of the Items class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMS_H__1E6F1C88_D19B_11D3_B4D6_005004D39EC7__INCLUDED_)
#define AFX_ITEMS_H__1E6F1C88_D19B_11D3_B4D6_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// Items

class EbsItems : 
	public IDispatchImpl<IEbsItems, &IID_IEbsItems, &LIBID_EBSCOMLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<EbsItems,&CLSID_EbsItems>
{
	PPROP		m_pProp;
	LONG		m_lItemsCount;			// Number of items in section
	LONG		m_lSectionIndex;		// Index of owning section

public:
	EbsItems() : m_pProp(NULL), m_lItemsCount(0) {}

	void	Init(PPROP pProp, LONG lSectionIndex);

BEGIN_COM_MAP(EbsItems)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IEbsItems)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(EbsItems) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_EbsItems)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IEbsItems
public:
	STDMETHOD(get__NewEnum)(/*[out, retval]*/ IUnknown* *pVal);
	STDMETHOD(Item)(/*[in]*/ long Index, /*[out, retval]*/ VARIANT* retval);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
};

typedef CComObject<EbsItems>		CComItems;


#endif // !defined(AFX_ITEMS_H__1E6F1C88_D19B_11D3_B4D6_005004D39EC7__INCLUDED_)
