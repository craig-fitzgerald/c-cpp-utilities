// Section.h: Definition of the Section class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SECTION_H__942E2BFB_D003_11D3_B4D5_005004D39EC7__INCLUDED_)
#define AFX_SECTION_H__942E2BFB_D003_11D3_B4D5_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "ebs.h"


/////////////////////////////////////////////////////////////////////////////
// EbsSection

class EbsSection : 
	public IDispatchImpl<IEbsSection, &IID_IEbsSection, &LIBID_EBSCOMLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<EbsSection,&CLSID_EbsSection>
{
	PPROP		m_pProp;
	LONG		m_lIndex;		// Index of section

public:
	EbsSection() : m_pProp(NULL), m_lIndex(0) {}

	void	Init(PPROP pProp, LONG SectionIndex);

BEGIN_COM_MAP(EbsSection)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IEbsSection)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(EbsSection) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_EbsSection)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IEbsSection
public:
	STDMETHOD(get_IsOption)(VARIANT_BOOL *pVal);
	STDMETHOD(get_IsAlternate)(VARIANT_BOOL *pVal);
	STDMETHOD(get_Items)(/*[out, retval]*/ LPDISPATCH *pVal);
	STDMETHOD(get_Total)(/*[out, retval]*/ double *pVal);
	STDMETHOD(get_Description)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_AltCode)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Number)(/*[out, retval]*/ BSTR *pVal);
};

typedef CComObject<EbsSection>		CComSection;


#endif // !defined(AFX_SECTION_H__942E2BFB_D003_11D3_B4D5_005004D39EC7__INCLUDED_)
