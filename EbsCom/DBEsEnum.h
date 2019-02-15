// DBEsEnum.h: Definition of the DBEsEnum class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBESENUM_H__0111E79B_D32B_11D3_B4D8_005004D39EC7__INCLUDED_)
#define AFX_DBESENUM_H__0111E79B_D32B_11D3_B4D8_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "GnuType.h"
#include "Ebs.h"


/////////////////////////////////////////////////////////////////////////////
// EbsDBEsEnum

class EbsDBEsEnum : 
	public IDispatchImpl<IEbsDBEsEnum, &IID_IEbsDBEsEnum, &LIBID_EBSCOMLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<EbsDBEsEnum,&CLSID_EbsDBEsEnum>,
	public IEnumVARIANT
{
	PPROP		m_pProp;
	CWinMeta*		m_pwm;
	LONG		m_lCount;
	LONG		m_lIndex;

public:
	EbsDBEsEnum() : m_pProp(NULL), m_pwm(NULL), m_lCount(0), m_lIndex(0) {}

	void	Init(PPROP pProp, CWinMeta* pwm, LONG lCount)	{m_pProp = pProp; m_pwm = pwm; m_lCount = lCount;}


BEGIN_COM_MAP(EbsDBEsEnum)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IEbsDBEsEnum)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IEnumVARIANT)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(EbsDBEsEnum) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_EbsDBEsEnum)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	STDMETHOD(Next)(ULONG celt, VARIANT* rgelt, ULONG* peltFetched);
	STDMETHOD(Skip)(ULONG celt);
	STDMETHOD(Reset)(void);
	STDMETHOD(Clone)(IEnumVARIANT** ppEnum);

// IEbsDBEsEnum
public:
};

typedef CComObject<EbsDBEsEnum>		CComDBEsEnum;


#endif // !defined(AFX_DBESENUM_H__0111E79B_D32B_11D3_B4D8_005004D39EC7__INCLUDED_)
