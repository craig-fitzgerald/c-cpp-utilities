// EbsProposal.h : Declaration of the CEbsProposal

#ifndef __EBSPROPOSAL_H_
#define __EBSPROPOSAL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "Gnutype.h"
#include "ebs.h"



/////////////////////////////////////////////////////////////////////////////
// CEbsProposal
class ATL_NO_VTABLE CEbsProposal : 
//	public IDispatchImpl<IEbsProposal, &IID_IEbsProposal, &LIBID_EBSCOMLib>,	// +
//	public CComObjectRoot,																		// +
//	public CComCoClass<CEbsProposal,&CLSID_EbsProposal>								// +

	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEbsProposal, &CLSID_EbsProposal>,
	public ISupportErrorInfo,
    public IDispatchImpl<IEbsProposal, &IID_IEbsProposal, &LIBID_EBSCOMLib>
{
   PPROP		m_pProp;
   PPROP		m_amend_pprop;         // amendment
   LONG		    m_lSectionsCount;
   
public:
    CEbsProposal(): m_pProp(0), m_amend_pprop(0), m_lSectionsCount(0) { }
    ~CEbsProposal() { };

	Init(PPROP pProp=NULL, PPROP amndPprop=NULL);
    


DECLARE_REGISTRY_RESOURCEID(IDR_EBSPROPOSAL)

// DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CEbsProposal)
	COM_INTERFACE_ENTRY(IEbsProposal)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IEbsProposal
public:
   	STDMETHOD(CheckProposal)(/*[in,out]*/ VARIANT *Errors,  INT *pNumErrors);
    STDMETHOD(GetAmendmentChanges)(/* [in,out] */ VARIANT* Changes, EbsIOError *pError);
	STDMETHOD(ApplyAmendment)(EbsIOError *pError);
	STDMETHOD(get_BidderEmail)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_BidderEmail)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_BidderPager)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_BidderPager)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_BidderFax)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_BidderFax)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_BidderPhone)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_BidderPhone)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_BidderZip)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_BidderZip)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_BidderState)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_BidderState)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_BidderCity)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_BidderCity)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_BidderAddr2)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_BidderAddr2)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_BidderAddr1)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_BidderAddr1)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_BidderName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_BidderName)(/*[in]*/ BSTR newVal);
    STDMETHOD(get_BidderID)(/*[out, retval]*/ BSTR *pVal);
    STDMETHOD(put_BidderID)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_HighwayNumber)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ContractType)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_LettingID)(/*[out, retval]*/ BSTR *pVal);
//	STDMETHOD(get_MBEs)(/*[out, retval]*/ LPDISPATCH *pVal);	TODO upgrade <GL>: remove comment
//	STDMETHOD(get_WBEs)(/*[out, retval]*/ LPDISPATCH *pVal);	TODO upgrade <GL>: remove comment
//	STDMETHOD(get_DBEs)(/*[out, retval]*/ LPDISPATCH *pVal);	TODO upgrade <GL>: remove comment
    STDMETHOD(get_BidTotal)(/*[out, retval]*/ double *pVal);
	STDMETHOD(get_Description)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Check)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Informational)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_ValidBid)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_JointBid)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_AmendmentsCount)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_ProjectID)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_DateRevised)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(get_DateGenerated)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(get_County)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ContractID)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_CallOrder)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_LettingDate)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(get_Agency)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Sections)(/*[out, retval]*/ LPDISPATCH *pVal);
};

typedef CComObject<CEbsProposal>		CComProposal;

#endif //__EBSPROPOSAL_H_
