// Item.h: Definition of the Item class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__1E6F1C8E_D19B_11D3_B4D6_005004D39EC7__INCLUDED_)
#define AFX_ITEM_H__1E6F1C8E_D19B_11D3_B4D6_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols


/////////////////////////////////////////////////////////////////////////////
// Item

class EbsItem : 
	public IDispatchImpl<IEbsItem, &IID_IEbsItem, &LIBID_EBSCOMLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<EbsItem,&CLSID_EbsItem>
{
	PPROP		m_pProp;
	long		m_lIndex;		// index of item in item array
    long        m_lSection;     // index of section

public:
	EbsItem() : m_pProp(NULL), m_lIndex(-1), m_lSection(-1) {}

	void	Init(PPROP pProp, long lIndex, long lSection);

BEGIN_COM_MAP(EbsItem)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IEbsItem)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(EbsItem) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_EbsItem)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IEbsItem
public:
	STDMETHOD(EnableFixedPriceItem)(/*[in]*/ VARIANT_BOOL Enable);
	STDMETHOD(get_IsFixedPriceChangeable)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(InvalidateItem)();
	STDMETHOD(get_IsOption)(VARIANT_BOOL *pVal);
	STDMETHOD(get_IsAlternate)(VARIANT_BOOL *pVal);
	STDMETHOD(get_IsUsed)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_IsUsed)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_IsLumpSum)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_IsFixedPrice)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_AltCode)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ItemNumber)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(IsBid)(/*[out, retval]*/ VARIANT_BOOL* pRes);
	STDMETHOD(get_UnitPrice)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_UnitPrice)(/*[in]*/ double newVal);
	STDMETHOD(get_Unit)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ShortDescription)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Quantity)(/*[out, retval]*/ double *pVal);
	STDMETHOD(get_LongDescription)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_LineNumber)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Extension)(/*[out, retval]*/ double *pVal);
};

typedef CComObject<EbsItem>		CComItem;


#endif // !defined(AFX_ITEM_H__1E6F1C8E_D19B_11D3_B4D6_005004D39EC7__INCLUDED_)
