// DBEItem.h: Definition of the DBEItem class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBEITEM_H__0111E7A7_D32B_11D3_B4D8_005004D39EC7__INCLUDED_)
#define AFX_DBEITEM_H__0111E7A7_D32B_11D3_B4D8_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "GnuType.h"
#include "Ebs.h"


/////////////////////////////////////////////////////////////////////////////
// EbsDBEItem

class EbsDBEItem : 
	public IDispatchImpl<IEbsDBEItem, &IID_IEbsDBEItem, &LIBID_EBSCOMLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<EbsDBEItem,&CLSID_EbsDBEItem>
{
	CDBEItem*		m_pItem;

public:
	EbsDBEItem() : m_pItem(NULL) {}

	void	Init(CDBEItem* pItem)		{m_pItem = pItem;}

BEGIN_COM_MAP(EbsDBEItem)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IEbsDBEItem)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(EbsDBEItem) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_EbsDBEItem)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IEbsDBEItem
public:
	STDMETHOD(get_Extension)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_Extension)(/*[in]*/ double newVal);
	STDMETHOD(get_Note)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(putref_Note)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_UnitPrice)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_UnitPrice)(/*[in]*/ double newVal);
	STDMETHOD(get_Quantity)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_Quantity)(/*[in]*/ double newVal);
	STDMETHOD(get_Index)(/*[out, retval]*/ long *pVal);
};

typedef CComObject<EbsDBEItem>		CComDBEItem;


#endif // !defined(AFX_DBEITEM_H__0111E7A7_D32B_11D3_B4D8_005004D39EC7__INCLUDED_)
