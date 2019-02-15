// DBEItems.h: Definition of the DBEItems class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBEITEMS_H__0111E7A1_D32B_11D3_B4D8_005004D39EC7__INCLUDED_)
#define AFX_DBEITEMS_H__0111E7A1_D32B_11D3_B4D8_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "GnuType.h"
#include "Ebs.h"

/////////////////////////////////////////////////////////////////////////////
// DBEItems

class EbsDBEItems : 
	public IDispatchImpl<IEbsDBEItems, &IID_IEbsDBEItems, &LIBID_EBSCOMLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<EbsDBEItems,&CLSID_EbsDBEItems>
{
	CDBEItem**		m_pItems;
	LONG			m_lCount;

public:
	EbsDBEItems() : m_pItems(NULL), m_lCount(0) {}

	void	Init(CDBEItem** pItems);

BEGIN_COM_MAP(EbsDBEItems)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IEbsDBEItems)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(EbsDBEItems) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_EbsDBEItems)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IEbsDBEItems
public:
	STDMETHOD(get__NewEnum)(/*[out, retval]*/ IUnknown* *pVal);
	STDMETHOD(Item)(/*[in]*/ long Index, /*[out, retval]*/ VARIANT* retval);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
};

typedef CComObject<EbsDBEItems>		CComDBEItems;


#endif // !defined(AFX_DBEITEMS_H__0111E7A1_D32B_11D3_B4D8_005004D39EC7__INCLUDED_)
