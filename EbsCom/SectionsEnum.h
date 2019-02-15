// SectionsEnum.h: Definition of the SectionsEnum class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SECTIONSENUM_H__942E2BFE_D003_11D3_B4D5_005004D39EC7__INCLUDED_)
#define AFX_SECTIONSENUM_H__942E2BFE_D003_11D3_B4D5_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "ebs.h"


/////////////////////////////////////////////////////////////////////////////
// EbsSectionsEnum

class EbsSectionsEnum : 
	public IDispatchImpl<IEbsSectionsEnum, &IID_IEbsSectionsEnum, &LIBID_EBSCOMLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<EbsSectionsEnum,&CLSID_EbsSectionsEnum>,
	public IEnumVARIANT
{
	PPROP		m_pProp;
	LONG		m_lSectionIndex;

public:
	EbsSectionsEnum() : m_pProp(NULL), m_lSectionIndex(ItemsEnumBegin) {}

	void Init(PPROP pProp)	{m_pProp = pProp;}

BEGIN_COM_MAP(EbsSectionsEnum)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IEbsSectionsEnum)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IEnumVARIANT)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(EbsSectionsEnum) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_EbsSectionsEnum)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	STDMETHOD(Next)(ULONG celt, VARIANT* rgelt, ULONG* peltFetched);
	STDMETHOD(Skip)(ULONG celt);
	STDMETHOD(Reset)(void);
	STDMETHOD(Clone)(IEnumVARIANT** ppEnum);

// IEbsSectionsEnum
public:
};

typedef CComObject<EbsSectionsEnum>		CComSectionsEnum;


#endif // !defined(AFX_SECTIONSENUM_H__942E2BFE_D003_11D3_B4D5_005004D39EC7__INCLUDED_)
