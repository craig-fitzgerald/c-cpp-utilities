// EbsProposal.cpp : Implementation of CEbsProposal
#include "stdafx.h"
#include "EbsCom.h"
#include "EbsProposal.h"
#include "EbsRead.h"
#include "EbsDat.h"
#include "GnuMisc.h"
#include "GnuStr.h"
#include "ComUtil.h"
#include "Sections.h"
#include "Items.h"
//#include "DBEs.h"
#include "EbsCrc.h"
#include "EbsCheck.h"
#include "ebs.h"
#include "Editfld0.h"
#include "EbsWrite.h"
#include "EbsErrors.h"
#include "ReadEbl.h"
#include "ApplyAmd.h"
#include "Amdview.h"
#include "EbsWrite.h"
#include "EbsErrors.h"







// --- GetAppIniFileName() ------------------------------------------
// 
CString GetAppIniFileName()
{
	return "";
}



/////////////////////////////////////////////////////////////////////////////
// CEbsProposal


STDMETHODIMP CEbsProposal::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEbsProposal
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


// --- Init() -------------------------------------------------------
// 
CEbsProposal::Init(PPROP pProp, PPROP amndProp)
{
	if (pProp)
		m_pProp = pProp;

    if (amndProp)
        m_amend_pprop = amndProp;

	if (m_pProp)
	{
		for (LONG index=0; index<m_pProp->ItemCount (); index++)
		{
			if (ISection(m_pProp->GetItem (index)))
			{
				m_lSectionsCount++;
			}

		}
	
	}
}





// --- ReadEbsFile() ------------------------------------------------
// 
//STDMETHODIMP CEbsProposal::ReadEbsFile(BSTR FileName, EbsIOError* pError)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState())

//	m_pProp = FreePROP(m_pProp, TRUE);

//	CNewPtr<char> strFileName = _com_util::ConvertBSTRToString(FileName);
//	*pError = (EbsIOError) ReadProposal (&m_pProp, (PSZ)(LPCTSTR)strFileName, FALSE);

//	if (*pError)
//		return S_OK;


	// Update DBE flag after bidder info has been set ---
    // CString strError;
    // UpdateDBEFlags(m_pProp, &strError);
    // if (!strError.IsEmpty() && ((CBidApp *)AfxGetApp())->m_bWarnRefNotFound)	TODO: m_bWarnRefNotFound attrib of EbsBid
	// if (!strError.IsEmpty())
	// {
    //		TODO: return strError in addition to pError
	// }

//	TotalAll (m_pProp, 0);     // Bid totals
    
	//	CalcDocInfo ();            // Display Metric stuff		TODO: useful in EbsCom ?
    //	SetModifiedFlag (FALSE);   // I actually need this		TODO: might be useful in EbsCom, too

//	Init();

//	return S_OK;
//}


// --- get_Sections() -----------------------------------------------
// 
STDMETHODIMP CEbsProposal::get_Sections(LPDISPATCH *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (!m_pProp)
		return E_FAIL;

	CComSections* pSections = new CComSections;
	pSections->Init(m_pProp, m_lSectionsCount);
	return pSections->QueryInterface(IID_IDispatch, (void**) pVal);
}



// --- get_Agency() -------------------------------------------------
// 
STDMETHODIMP CEbsProposal::get_Agency(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pszAgency);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_LettingDate(VARIANT *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	COleVariant varDate;
	if (m_pProp->m_dLettingDate)
		varDate = COleDateTime((time_t) m_pProp->m_dLettingDate);

	*pVal = varDate.Detach();

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_CallOrder(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pszCallOrder);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_ContractID(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pszContractID);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_County(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	PSZ pszCounty = (!m_pProp->m_iCountyCount ? "" : 
							(m_pProp->m_iCountyCount==1 ? m_pProp->m_pnty[0].m_pszName : "Multiple"));
	*pVal = _com_util::ConvertStringToBSTR(pszCounty);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_DateGenerated(VARIANT *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	COleVariant varDate;
	if (m_pProp->m_dDateGenerated)
		varDate = COleDateTime((time_t) m_pProp->m_dDateGenerated);

	*pVal = varDate.Detach();

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_DateRevised(VARIANT *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	COleVariant varDate;
	if (m_pProp->m_dDateRevised)
		varDate = COleDateTime((time_t) m_pProp->m_dDateRevised);

	*pVal = varDate.Detach();

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_BidderID(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pbfo->m_pszID);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_ProjectID(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	PSZ pszProjectID = (strchr (m_pProp->m_pszProjects, '\n') ? "Multiple" : m_pProp->m_pszProjects);
	*pVal = _com_util::ConvertStringToBSTR(pszProjectID);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_AmendmentsCount(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_pProp->m_iAmendCount;

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_JointBid(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = IsFlag (m_pProp->m_wFlags, JOINTBID) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_ValidBid(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	/*--- Perform check without report ---*/
	CheckBid (m_pProp, FALSE);

	*pVal = IsFlag (m_pProp->m_wFlags, BIDERR) ? VARIANT_FALSE : VARIANT_TRUE;

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_Informational(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = IsFlag (m_pProp->m_wFlags, INFO) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_Check(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	/*--- Re-generate CheckSum ---*/
	m_pProp->m_ulCRC = GenCRC (m_pProp);

	char szCheck[9];
	sprintf (szCheck, "%8.8lX", m_pProp->m_ulCRC);
	*pVal = _com_util::ConvertStringToBSTR(szCheck);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_Description(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pszDesc);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_BidTotal(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_pProp->m_fBidTotal;

	return S_OK;
}





STDMETHODIMP CEbsProposal::get_LettingID(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pszLettingID);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_ContractType(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pszContType);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_HighwayNumber(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pszHighwayNum);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_BidderName(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pbfo->m_pszName);
    
	return S_OK;
}

STDMETHODIMP CEbsProposal::put_BidderName(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_pProp->m_pbfo->m_pszName = _com_util::ConvertBSTRToString(newVal);

	return S_OK;
}


STDMETHODIMP CEbsProposal::put_BidderID(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_pProp->m_pbfo->m_pszID = _com_util::ConvertBSTRToString(newVal);

	return S_OK;
}


STDMETHODIMP CEbsProposal::get_BidderAddr1(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pbfo->m_pszAddr1);

	return S_OK;
}

STDMETHODIMP CEbsProposal::put_BidderAddr1(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_pProp->m_pbfo->m_pszAddr1 = _com_util::ConvertBSTRToString(newVal);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_BidderAddr2(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pbfo->m_pszAddr2);

	return S_OK;
}

STDMETHODIMP CEbsProposal::put_BidderAddr2(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_pProp->m_pbfo->m_pszAddr2 = _com_util::ConvertBSTRToString(newVal);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_BidderCity(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pbfo->m_pszCity);

	return S_OK;
}

STDMETHODIMP CEbsProposal::put_BidderCity(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_pProp->m_pbfo->m_pszCity = _com_util::ConvertBSTRToString(newVal);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_BidderState(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pbfo->m_pszState);

	return S_OK;
}

STDMETHODIMP CEbsProposal::put_BidderState(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_pProp->m_pbfo->m_pszState = _com_util::ConvertBSTRToString(newVal);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_BidderZip(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pbfo->m_pszZip);

	return S_OK;
}

STDMETHODIMP CEbsProposal::put_BidderZip(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_pProp->m_pbfo->m_pszZip = _com_util::ConvertBSTRToString(newVal);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_BidderPhone(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pbfo->m_pszPhone);

	return S_OK;
}

STDMETHODIMP CEbsProposal::put_BidderPhone(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_pProp->m_pbfo->m_pszPhone = _com_util::ConvertBSTRToString(newVal);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_BidderFax(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pbfo->m_pszFax);

	return S_OK;
}

STDMETHODIMP CEbsProposal::put_BidderFax(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_pProp->m_pbfo->m_pszFax = _com_util::ConvertBSTRToString(newVal);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_BidderPager(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pbfo->m_pszPager);

	return S_OK;
}

STDMETHODIMP CEbsProposal::put_BidderPager(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_pProp->m_pbfo->m_pszPager = _com_util::ConvertBSTRToString(newVal);

	return S_OK;
}

STDMETHODIMP CEbsProposal::get_BidderEmail(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _com_util::ConvertStringToBSTR(m_pProp->m_pbfo->m_pszEmail);

	return S_OK;
}

STDMETHODIMP CEbsProposal::put_BidderEmail(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_pProp->m_pbfo->m_pszEmail = _com_util::ConvertBSTRToString(newVal);

	return S_OK;
}


STDMETHODIMP CEbsProposal::ApplyAmendment(EbsIOError *pError)
{
    int nError;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    if (!m_pProp)
    {
        *pError = AMENDMENT_OUT_OF_ORDER;
        return S_OK;
    }

    nError = ::ApplyAmendment (m_pProp,m_amend_pprop);
    switch (nError)
    {
        /*  From Applyamd.cpp:
        **  1 - amendment already applied
        **  2 - amendment out of order
        **  3 - ppropAmend not an amendment
        **  4 - amendment for different proposal
        */
        case 0: *pError = EBSIOERR_NO_ERROR; break;
        case 1: *pError = AMENDMENT_ALREADY_APPLIED; break;
        case 2: *pError = AMENDMENT_OUT_OF_ORDER; break;
        case 3: *pError = FILE_NOT_AMENDMENT; break;
        case 4: *pError = AMENDMENT_FOR_DIFF_PROPOSAL; break;
    };


	return S_OK;
}


STDMETHODIMP CEbsProposal::GetAmendmentChanges(VARIANT* Changes, EbsIOError *pError)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    
    // store changes in SAFEARRAY
    long index[1];
	BSTR bsAmdChange;
	SAFEARRAYBOUND rgsabound[1];
    int nCount=0;
    PSZ    *apszList;
    SAFEARRAY FAR* psaAmendChanges; // amendment changes 
    
    nCount = GatherAmendmentChanges (m_pProp,m_amend_pprop);

    apszList = (PSZ *) szY;
    
    // create a SAFEARRAY of BSTRs which
	// will store the amendment changes
	rgsabound[0].lLbound = 0;
	rgsabound[0].cElements = nCount;
	psaAmendChanges = SafeArrayCreate(VT_BSTR, 1, rgsabound);
	if(psaAmendChanges == NULL)
	{
		return E_OUTOFMEMORY;
	}

	// go through all the filenames and store
	// them in the SAFEARRAY
    for (int i=0; i< nCount; i++)
    {
       	
        wchar_t *wcChangeLine;

	    /*--- * means a centered string ---*/
	    if (apszList[i][0] == '*' && apszList[i][1] == '*')
        {
            wcChangeLine  = (wchar_t *)malloc( (strlen(apszList[i])-1)*sizeof( wchar_t ));        
		    int r = mbstowcs(wcChangeLine,apszList[i]+2,strlen(apszList[i])-1);
        }
		    

	    /*--- # means a line ---*/
	    else if (apszList[i][0] == '#' && apszList[i][1] == '#')
        {
            int nLen = strlen("------------------------------------------------------------------------")+1;
            wcChangeLine  = (wchar_t *)malloc( nLen*sizeof( wchar_t ));        
		    
            int r = mbstowcs(wcChangeLine,"------------------------------------------------------------------------",nLen);

        }

	    /*--- @ means a left string ---*/
	    else if (apszList[i][0] == '@' && apszList[i][1] == '@')
        {
		    wcChangeLine  = (wchar_t *)malloc( (strlen(apszList[i])-1)*sizeof( wchar_t ));        
		    int r = mbstowcs(wcChangeLine,apszList[i]+2,strlen(apszList[i])-1);
        }

	    /*--- section/item data lines ---*/
        else
        {
            char szLine[400];
            CItem*   pItem;
	        pItem = (CItem*)(apszList[i]);
            memset(szLine,0,255);
	        if (IsFlag (pItem->m_wFlags, fSECTION))
		        sprintf(szLine,"   Section: %s  %s", pItem->m_pszLineNum, pItem->m_pszLongDesc);
	        else
    		    sprintf(szLine,"        Item: %s  %s  %s", pItem->m_pszLineNum, pItem->m_pszItemNum, pItem->m_pszLongDesc);
    	    wcChangeLine  = (wchar_t *)malloc( (strlen(szLine)+1) * sizeof( wchar_t ));        
		    int r = mbstowcs(wcChangeLine,szLine,strlen(szLine)+1);
        }
		
        bsAmdChange = SysAllocString(wcChangeLine);	

        free(wcChangeLine);			
		index[0] = i;
		HRESULT ret = SafeArrayPutElement(psaAmendChanges,index,bsAmdChange);
		if (!SUCCEEDED(ret))
		{
			return ret;
		}
	}
	
    m_amend_pprop = FreePROP(m_amend_pprop, TRUE);  

    
	// now wrap the SAFEARRAY in a Variant
	VariantInit(Changes);
	Changes->vt  = VT_ARRAY | VT_BSTR;
	Changes->m_parray = psaAmendChanges;	
 
	return S_OK;
}

STDMETHODIMP CEbsProposal::CheckProposal(VARIANT *Errors, INT *pNumErrors)
{
    long index[1];
	SAFEARRAY FAR* psa;
	BSTR bsError;
	SAFEARRAYBOUND rgsabound[1];
    int nErrorCount = 0;
    PERRNODE *pBidErrors;
    char *pOldIdPtr=0;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    *pNumErrors = 0;

    if (!m_pProp || !m_pProp->m_pbfo)
        return ERROR_INVALID_DATA;

   
    if (!m_pProp->m_pbfo->m_pszID || strlen(m_pProp->m_pbfo->m_pszID)==0) 
    {
        // Do this so CheckBid will work.
        // Keep old ptr to set back after CheckBid
        pOldIdPtr = m_pProp->m_pbfo->m_pszID; 
        m_pProp->m_pbfo->m_pszID = (char *)malloc(10);
        strcpy(m_pProp->m_pbfo->m_pszID,"TEMP"); 
    }
    
    if (!CheckBid (m_pProp, true))
    {
        //BidErrGetInfo (pNumErrors,0);
        pBidErrors = BidErrGetErrorList (&nErrorCount);
        
        *pNumErrors = nErrorCount;

       	// create a SAFEARRAY of BSTRs which
	    // will store the errors/warnings
	    rgsabound[0].lLbound = 0;
	    rgsabound[0].cElements = nErrorCount;
	    psa = SafeArrayCreate(VT_BSTR, 1, rgsabound);
	    if(psa == NULL)
	    {
            if (pOldIdPtr)
            {
                free(m_pProp->m_pbfo->m_pszID);
                m_pProp->m_pbfo->m_pszID = pOldIdPtr;
            }
                
		    return E_OUTOFMEMORY;
	    }

	    // go through all the Errors and store
	    // them in the SAFEARRAY
	    for (int i=0; i< nErrorCount; i++)
        { 
		    wchar_t *wcFileName  = (wchar_t *)malloc( (strlen(pBidErrors[i]->m_pszErr)+1)*sizeof( wchar_t ));        
		    int r = mbstowcs(wcFileName,pBidErrors[i]->m_pszErr,strlen(pBidErrors[i]->m_pszErr)+1);
		    bsError = SysAllocString(wcFileName);
		    free(wcFileName);			
		    index[0] = i;
		    HRESULT ret = SafeArrayPutElement(psa,index,bsError);
		    if (!SUCCEEDED(ret))
		    {
                if (pOldIdPtr)
                {   
                    free(m_pProp->m_pbfo->m_pszID);
                    m_pProp->m_pbfo->m_pszID = pOldIdPtr;
                }
                return ret;
		    }
	    }

        BidErrFreeErrors();

        
	    // now wrap the SAFEARRAY in a Variant
	    VariantInit(Errors);
	    Errors->vt  = VT_ARRAY | VT_BSTR;
	    Errors->m_parray = psa;

        

    }

    if (pOldIdPtr)
    {
        free(m_pProp->m_pbfo->m_pszID);
        m_pProp->m_pbfo->m_pszID = pOldIdPtr;
    }

	return S_OK;
}



/*** TODO upgrade <GL>: remove comment
// --- get_DBEs() ---------------------------------------------------
// 
STDMETHODIMP CEbsProposal::get_DBEs(LPDISPATCH *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (!m_pProp)
		return E_FAIL;

	*pVal = NULL;
	if (!m_pProp->m_pwmDBE)
		return S_OK;

	CComDBEs* pDBEs = new CComDBEs;
	pDBEs->Init(m_pProp, m_pProp->m_pwmDBE);
	return pDBEs->QueryInterface(IID_IDispatch, (void**) pVal);
}

// --- get_WBEs() ---------------------------------------------------
// 
STDMETHODIMP CEbsProposal::get_WBEs(LPDISPATCH *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (!m_pProp)
		return E_FAIL;

	*pVal = NULL;
	if (!m_pProp->m_pwmWBE)
		return S_OK;

	CComDBEs* pDBEs = new CComDBEs;
	pDBEs->Init(m_pProp, m_pProp->m_pwmWBE);
	return pDBEs->QueryInterface(IID_IDispatch, (void**) pVal);
}


// --- get_MBEs() ---------------------------------------------------
// 
STDMETHODIMP CEbsProposal::get_MBEs(LPDISPATCH *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (!m_pProp)
		return E_FAIL;

	*pVal = NULL;
	if (!m_pProp->m_pwmMBE)
		return S_OK;

	CComDBEs* pDBEs = new CComDBEs;
	pDBEs->Init(m_pProp, m_pProp->m_pwmMBE);
	return pDBEs->QueryInterface(IID_IDispatch, (void**) pVal);
}
***/




