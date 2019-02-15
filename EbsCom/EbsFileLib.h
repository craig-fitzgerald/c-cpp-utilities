// EbsFileLib.h : Declaration of the CEbsFileLib

#ifndef __EBSFILELIB_H_
#define __EBSFILELIB_H_

#include "resource.h"       // main symbols
#include "ebs.h"
#include "Gnutype.h"

/////////////////////////////////////////////////////////////////////////////
// CEbsFileLib
class ATL_NO_VTABLE CEbsFileLib : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEbsFileLib, &CLSID_EbsFileLib>,
	public ISupportErrorInfo,
	public IDispatchImpl<IEbsFileLib, &IID_IEbsFileLib, &LIBID_EBSCOMLib>
{
	PPROP		m_pProp;        // proposal
    PPROP       m_amend_pprop;  // amendment
	
public:
	CEbsFileLib() : m_pProp(0), m_amend_pprop(0)	{ }
	~CEbsFileLib();

DECLARE_REGISTRY_RESOURCEID(IDR_EBSFILELIB)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CEbsFileLib)
	COM_INTERFACE_ENTRY(IEbsFileLib)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


// IEbsFileLib
public:
	STDMETHOD(OpenAmendment)(BSTR LibName, BSTR FileName, EbsIOError* pError);
	STDMETHOD(get_Proposal)(/*[out, retval]*/ LPDISPATCH *pVal);
	STDMETHOD(OpenLib)(BSTR LibName, VARIANT *FileNames, EbsIOError *pError);
	STDMETHOD(SaveProposal)(/*[in]*/BSTR FileName, /*[out,retval]*/EbsIOError *pError);
	STDMETHOD(OpenFile)(BSTR LibName,BSTR FileName,  EbsIOError* pErrors);
};

#endif //__EBSFILELIB_H_
