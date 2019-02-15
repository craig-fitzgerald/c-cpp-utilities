// ItemsEnum.h: Definition of the ItemsEnum class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSENUM_H__1E6F1C8B_D19B_11D3_B4D6_005004D39EC7__INCLUDED_)
#define AFX_ITEMSENUM_H__1E6F1C8B_D19B_11D3_B4D6_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// ItemsEnum

class EbsItemsEnum : 
	public IDispatchImpl<IEbsItemsEnum, &IID_IEbsItemsEnum, &LIBID_EBSCOMLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<EbsItemsEnum,&CLSID_EbsItemsEnum>,
	IEnumVARIANT
{
	PPROP		m_pProp;
	LONG		m_lItemIndex;			// Current item index
	LONG		m_lSectionIndex;		// Owner section

public:
	EbsItemsEnum() : m_pProp(NULL), m_lItemIndex(ItemsEnumBegin), m_lSectionIndex(0) {}

	void Init(PPROP pProp, LONG lSectionIndex);

BEGIN_COM_MAP(EbsItemsEnum)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IEbsItemsEnum)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IEnumVARIANT)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(ItemsEnum) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_EbsItemsEnum)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	STDMETHOD(Next)(ULONG celt, VARIANT* rgelt, ULONG* peltFetched);
	STDMETHOD(Skip)(ULONG celt);
	STDMETHOD(Reset)(void);
	STDMETHOD(Clone)(IEnumVARIANT** ppEnum);

// IEbsItemsEnum
public:
};

typedef CComObject<EbsItemsEnum>		CComItemsEnum;


#endif // !defined(AFX_ITEMSENUM_H__1E6F1C8B_D19B_11D3_B4D6_005004D39EC7__INCLUDED_)
