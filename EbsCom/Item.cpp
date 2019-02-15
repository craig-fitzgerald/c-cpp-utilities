// Item.cpp : Implementation of CEbsComApp and DLL registration.

#include "stdafx.h"
#include "EbsCom.h"
#include "gnutype.h"
#include "ebs.h"
#include "ComUtil.h"
#include "GnuMem.h"
#include "Item.h"
#include "EbsDat.h"
#include "GnuMisc.h"
#include "GnuMath.h"
#include "EbsCheck.h"


/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP EbsItem::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEbsItem,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


// --- Init() -------------------------------------------------------
// 
void EbsItem::Init(PPROP pProp, long lIndex, long lSection)
{
	m_pProp = pProp;
	m_lIndex = lIndex;
    m_lSection = lSection;
}



// --- get_Extension() ----------------------------------------------
// 
STDMETHODIMP EbsItem::get_Extension(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (IsFlag (m_pProp->GetItem (m_lIndex)->m_wFlags, fFIXEDPRICE)
		 && !IsFlag (m_pProp->GetItem (m_lIndex)->m_wFlags, fISBID))
		*pVal = 0.0;
	else
		*pVal = m_pProp->GetItem (m_lIndex)->m_fExt;

	return S_OK;
}



// --- get_LineNumber() ---------------------------------------------
// 
STDMETHODIMP EbsItem::get_LineNumber(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->GetItem (m_lIndex)->m_pszLineNum);

	return S_OK;
}


// --- get_LongDescription() ----------------------------------------
// 
STDMETHODIMP EbsItem::get_LongDescription(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->GetItem (m_lIndex)->m_pszLongDesc);

	return S_OK;
}


// --- get_Quantity() -----------------------------------------------
// 
STDMETHODIMP EbsItem::get_Quantity(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_pProp->GetItem (m_lIndex)->m_fQuan;

	return S_OK;
}


// --- get_ShortDescription() ---------------------------------------
// 
STDMETHODIMP EbsItem::get_ShortDescription(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->GetItem (m_lIndex)->m_pszShortDesc);

	return S_OK;
}


// --- get_Unit() ---------------------------------------------------
// 
STDMETHODIMP EbsItem::get_Unit(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->GetItem (m_lIndex)->m_pszUnit);

	return S_OK;
}



// --- get_UnitPrice() ----------------------------------------------
// 
STDMETHODIMP EbsItem::get_UnitPrice(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (IsFlag (m_pProp->GetItem (m_lIndex)->m_wFlags, fFIXEDPRICE)
		 && !IsFlag (m_pProp->GetItem (m_lIndex)->m_wFlags, fISBID))
		*pVal = 0.0;
	else
		*pVal = m_pProp->GetItem (m_lIndex)->m_fUnitPrice;

	return S_OK;
}



// --- put_UnitPrice() ----------------------------------------------
// 
STDMETHODIMP EbsItem::put_UnitPrice(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if (!IsFlag (m_pProp->GetItem (m_lIndex)->m_wFlags, fFIXEDPRICE))
		m_pProp->GetItem (m_lIndex)->m_fUnitPrice = newVal;
	TotalAll (m_pProp, m_lIndex);
	CheckBid (m_pProp, FALSE);

	return S_OK;
}


// --- IsBid() ------------------------------------------------------
// 
STDMETHODIMP EbsItem::IsBid(VARIANT_BOOL *pRes)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pRes = ValidExtension(m_pProp->GetItem (m_lIndex)) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}


// --- get_ItemNumber() ---------------------------------------------
// 
STDMETHODIMP EbsItem::get_ItemNumber(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->GetItem (m_lIndex)->m_pszItemNum);

	return S_OK;
}


// --- get_AltCode() ------------------------------------------------
// 
STDMETHODIMP EbsItem::get_AltCode(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->GetItem (m_lIndex)->m_pszAlt);

	return S_OK;
}


// --- get_IsFixedPrice() -------------------------------------------
// 
STDMETHODIMP EbsItem::get_IsFixedPrice(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = IFixedPrice(m_pProp->GetItem (m_lIndex)) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}


// --- get_IsLumpSum() ----------------------------------------------
// 
STDMETHODIMP EbsItem::get_IsLumpSum(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = ILumpSum(m_pProp->GetItem (m_lIndex)) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}


// --- get_IsUsed() -------------------------------------------------
// 
STDMETHODIMP EbsItem::get_IsUsed(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = IIsBid(m_pProp->GetItem (m_lIndex)) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}


// --- put_IsUsed() -------------------------------------------------
// 
STDMETHODIMP EbsItem::put_IsUsed(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	/*
	 *	If item is fixed price and is part of an item
	 *	or section option, allow the user to select/deselect
	 */
	if (   IFixedPrice (m_pProp->GetItem (m_lIndex)) 
		&& (!IsFlag (m_pProp->m_wDisplayFlags, FIXEDPRICENOSEL) || IIsOpt(m_pProp->GetItem (m_lIndex))))
	{
		SetFlag (&(m_pProp->GetItem (m_lIndex)->m_wFlags), fISBID, !!newVal);
		TotalAll (m_pProp, m_lIndex);
		CheckBid (m_pProp, FALSE);
	}

	return S_OK;
}


STDMETHODIMP EbsItem::get_IsAlternate(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = ( IIsAlt(m_pProp->GetItem (m_lIndex)) ) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP EbsItem::get_IsOption(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = ( IIsOpt(m_pProp->GetItem (m_lIndex)) ) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP EbsItem::InvalidateItem()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // If this is a Fixed Price and NOT non-selectable, clear ISBID flag
    if (IFixedPrice (m_pProp->GetItem (m_lIndex)))
    {
        if (!IsFlag(m_pProp->m_wDisplayFlags, FIXEDPRICENOSEL)) 
        SetFlag (&(m_pProp->GetItem (m_lIndex)->m_wFlags), fISBID, 0);
    }

    // If not Fixed Price, invalidate unit price
    else 
    {
        MthInvalidate (&(m_pProp->GetItem (m_lIndex)->m_fUnitPrice));
        TotalAll (m_pProp, m_lIndex);
    	CheckBid (m_pProp, FALSE);
    }
    
	return S_OK;
}


STDMETHODIMP EbsItem::get_IsFixedPriceChangeable(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = ( !IsFlag (m_pProp->m_wDisplayFlags, FIXEDPRICENOSEL) ) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP EbsItem::EnableFixedPriceItem(VARIANT_BOOL Enable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // do nothing if not Fixed Price item
    if ( !IFixedPrice (m_pProp->GetItem (m_lIndex)) )
        return S_OK; 

    // do nothing if Fixed price is not selectable
    if (IsFlag (m_pProp->m_wDisplayFlags, FIXEDPRICENOSEL))
        return S_OK; 

    // set/clear flag
    SetFlag (&(m_pProp->GetItem (m_lIndex)->m_wFlags), fISBID, !!Enable);
	
    TotalAll (m_pProp, m_lIndex);
	CheckBid (m_pProp, FALSE);

	return S_OK;
}
