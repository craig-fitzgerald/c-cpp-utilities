// DBE.h: Definition of the DBE class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBE_H__0111E79E_D32B_11D3_B4D8_005004D39EC7__INCLUDED_)
#define AFX_DBE_H__0111E79E_D32B_11D3_B4D8_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "GnuType.h"
#include "Ebs.h"


/////////////////////////////////////////////////////////////////////////////
// EbsDBE

class EbsDBE : 
	public IDispatchImpl<IEbsDBE, &IID_IEbsDBE, &LIBID_EBSCOMLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<EbsDBE,&CLSID_EbsDBE>
{
	PPROP		m_pProp;
	CWinMeta*		m_pwm;
	CRowData*		m_pDBE;

public:
	EbsDBE() : m_pProp(NULL), m_pwm(NULL), m_pDBE(NULL) {}

	void	Init(PPROP pProp, CWinMeta* pwm, CRowData* pwmDBE)		{m_pProp = pProp; m_pwm = pwm; m_pDBE = pwmDBE;} 

private:
	void	SetFieldData(long lField);

BEGIN_COM_MAP(EbsDBE)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IEbsDBE)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(DBE) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_EbsDBE)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IEbsDBE
public:
	STDMETHOD(get_Amount)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_Amount)(/*[in]*/ double newVal);
	STDMETHOD(get_ID)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ID)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_IsBid)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_IsBid)(/*[in]*/ long newVal);
	STDMETHOD(get_UsedAs)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_UsedAs)(/*[in]*/ long newVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Name)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_DBEItems)(/*[out, retval]*/ LPDISPATCH *pVal);
};

typedef CComObject<EbsDBE>		CComDBE;


#endif // !defined(AFX_DBE_H__0111E79E_D32B_11D3_B4D8_005004D39EC7__INCLUDED_)
