// DBEItemsEnum.h: Definition of the DBEItemsEnum class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBEITEMSENUM_H__0111E7A4_D32B_11D3_B4D8_005004D39EC7__INCLUDED_)
#define AFX_DBEITEMSENUM_H__0111E7A4_D32B_11D3_B4D8_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "GnuType.h"
#include "Ebs.h"

/////////////////////////////////////////////////////////////////////////////
// EbsDBEItemsEnum

class EbsDBEItemsEnum : 
	public IDispatchImpl<IEbsDBEItemsEnum, &IID_IEbsDBEItemsEnum, &LIBID_EBSCOMLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<EbsDBEItemsEnum,&CLSID_EbsDBEItemsEnum>,
	public IEnumVARIANT
{
	CDBEItem**		m_pItems;
	LONG			m_lCount;
	LONG			m_lIndex;

public:
	EbsDBEItemsEnum() : m_pItems(NULL), m_lCount(0), m_lIndex(0) {}

	void	Init(CDBEItem** pItems, LONG lCount)		{m_pItems = pItems; m_lCount = lCount;}

BEGIN_COM_MAP(EbsDBEItemsEnum)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IEbsDBEItemsEnum)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IEnumVARIANT)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(EbsDBEItemsEnum) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_EbsDBEItemsEnum)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	STDMETHOD(Next)(ULONG celt, VARIANT* rgelt, ULONG* peltFetched);
	STDMETHOD(Skip)(ULONG celt);
	STDMETHOD(Reset)(void);
	STDMETHOD(Clone)(IEnumVARIANT** ppEnum);

// IDBEItemsEnum
public:
};

typedef CComObject<EbsDBEItemsEnum>		CComDBEItemsEnum;


#endif // !defined(AFX_DBEITEMSENUM_H__0111E7A4_D32B_11D3_B4D8_005004D39EC7__INCLUDED_)
