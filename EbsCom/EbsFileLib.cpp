// EbsFileLib.cpp : Implementation of CEbsFileLib
#include "stdafx.h"
#include <stdlib.h>
#include "EbsCom.h"
#include "EbsFileLib.h"
#include "gnutype.h"
#include "ComUtil.h"
#include "GnuMem.h"
#include "GnuMisc.h"
#include "gnudir.h"
#include "EbsDat.h"
#include "EbsRead.h"
#include "ReadEbl.h"
#include "ApplyAmd.h"
#include "Amdview.h"
#include "EbsWrite.h"
#include "EbsProposal.h"
#include "EbsErrors.h"
#include "EbsCheck.h"


/////////////////////////////////////////////////////////////////////////////
// CEbsFileLib

CEbsFileLib::~CEbsFileLib()
{
	FreePROP(m_pProp, TRUE);
}

STDMETHODIMP CEbsFileLib::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEbsFileLib
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


// --- OpenFile()
STDMETHODIMP CEbsFileLib::OpenFile(BSTR LibName, BSTR FileName, EbsIOError* pError)
{
	
    BOOLEAN bLib = false;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

   	if (!FileName || !LibName || SysStringLen(FileName)==0)
	{
		return E_INVALIDARG;
	}

	m_pProp = FreePROP(m_pProp, TRUE);

	CNewPtr<char> strLibName = _com_util::ConvertBSTRToString(LibName);
	CNewPtr<char> strFileName = _com_util::ConvertBSTRToString(FileName);
	if ( strlen(strLibName) )
	{
		int iRet;
        if (iRet = ExtractFiles (strLibName,strFileName))
		{
            switch (iRet)
            {
                 case 0: *pError = EBSIOERR_NO_ERROR; break;
                 case 1: *pError = EBSIOERR_CANNOT_OPEN; break;
                 case 2: *pError = EBSIOERR_CANNOT_READ; break;
                 case 3: *pError = EBS_NOT_FOUND_IN_EBL_FILE; break;
            }
            return S_OK;
		}
        bLib = true;
	}
    
    // Open EBS file
    CHAR szPath[256];
    CHAR szPathFileName[256];
    
    if (bLib)
    {
        DirSplitPath (szPath, strLibName, DIR_DRIVE | DIR_DIR);
        strcpy(szPathFileName,szPath);
        strcat(szPathFileName, strFileName);
	}
    else
    {
        strcpy(szPathFileName, strFileName);
    }
    *pError = (EbsIOError) ReadProposal (&m_pProp, (PSZ)(LPCTSTR)szPathFileName, FALSE);
    
    if (*pError == EBSIOERR_NO_ERROR && m_pProp)
        TotalAll (m_pProp, 0);     // Bid totals
    
    return S_OK;
}

STDMETHODIMP CEbsFileLib::OpenAmendment(BSTR LibName, BSTR FileName, EbsIOError *pError)
{
    BOOLEAN bLib = false;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (!FileName || !LibName || SysStringLen(FileName)==0)
	{
		return E_INVALIDARG;
	}

    CNewPtr<char> strLibName = _com_util::ConvertBSTRToString(LibName);
	CNewPtr<char> strFileName = _com_util::ConvertBSTRToString(FileName);
	if ( strlen(strLibName) )
	{
		int iRet;
        if (iRet = ExtractFiles (strLibName,strFileName))
		{
            switch (iRet)
            {
                 case 0: *pError = EBSIOERR_NO_ERROR; break;
                 case 1: *pError = EBSIOERR_CANNOT_OPEN; break;
                 case 2: *pError = EBSIOERR_CANNOT_READ; break;
                 case 3: *pError = EBS_NOT_FOUND_IN_EBL_FILE; break;
            }
            return S_OK;
		}
        bLib = true;
	}
    
    // Open amend file
    CHAR szPath[256];
    CHAR szPathFileName[256];
    
    if (bLib)
    {
        DirSplitPath (szPath, strLibName, DIR_DRIVE | DIR_DIR);
        strcpy(szPathFileName,szPath);
        strcat(szPathFileName, strFileName);
	}
    else
    {
        strcpy(szPathFileName, strFileName);
    }
    
    
    *pError = (EbsIOError) ReadProposal (&m_amend_pprop, (PSZ)(LPCTSTR)szPathFileName, TRUE);
    
    
    return S_OK;

}


STDMETHODIMP CEbsFileLib::OpenLib(BSTR LibName,/* [in,out] */ VARIANT* FileNames, EbsIOError *pError)
{
   	PFDESC pcd = NULL;
	PLDESC pld;	
	long index[1];
	SAFEARRAY FAR* psa;
	BSTR bsFileName;
	SAFEARRAYBOUND rgsabound[1];

	AFX_MANAGE_STATE(AfxGetStaticModuleState())


   	if (!LibName || SysStringLen(LibName)==0)
	{
		return E_INVALIDARG;
	}


    CNewPtr<char> strFileName = _com_util::ConvertBSTRToString(LibName);
    if (!(pld = ::OpenLib (strFileName)))
    {
        int nLibError=0;
        GetLibErr(&nLibError);
        switch (nLibError)
        {
             case 6: *pError = EBSIOERR_CANNOT_OPEN; break;
             default: *pError = EBSIOERR_CANNOT_READ; break;
        };
		return S_OK;
    }

	// create a SAFEARRAY of BSTRs which
	// will store the filenames
	rgsabound[0].lLbound = 0;
	rgsabound[0].cElements = pld->m_iCount;
	psa = SafeArrayCreate(VT_BSTR, 1, rgsabound);
	if(psa == NULL)
	{
		return E_OUTOFMEMORY;
	}

	// go through all the filenames and store
	// them in the SAFEARRAY
	for (int i=0; i< pld->m_iCount; i++)
	{
		if (!(pcd = GetNextFile (pld, pcd, TRUE)))
			break;
		wchar_t *wcFileName  = (wchar_t *)malloc( (strlen(pcd->szName)+1)*sizeof( wchar_t ));        
		int r = mbstowcs(wcFileName,pcd->szName,strlen(pcd->szName)+1);
		bsFileName = SysAllocString(wcFileName);
		free(wcFileName);			
		index[0] = i;
		HRESULT ret = SafeArrayPutElement(psa,index,bsFileName);
		if (!SUCCEEDED(ret))
		{
			fclose(pld->m_fp);
			FreePLD(pld);
			return ret;
		}
	}
	fclose (pld->m_fp);
	FreePLD (pld);
	
	// now wrap the SAFEARRAY in a Variant
	VariantInit(FileNames);
	FileNames->vt  = VT_ARRAY | VT_BSTR;
	FileNames->m_parray = psa;
		
	return S_OK;
}

STDMETHODIMP CEbsFileLib::get_Proposal(LPDISPATCH *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (!m_pProp)
		return ERROR_INVALID_DATA;

	CComProposal* pProposal = new CComProposal;
	pProposal->Init(m_pProp, m_amend_pprop);
	return pProposal->QueryInterface(IID_IDispatch, (void**) pVal);
    
}

STDMETHODIMP CEbsFileLib::SaveProposal(BSTR FileName, EbsIOError *pError)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CNewPtr<char> strFileName = _com_util::ConvertBSTRToString(FileName);
	
	if (!FileName || SysStringLen(FileName)==0)
	{
		return E_INVALIDARG;
	}
    
    if (m_pProp->m_pbfo && m_pProp->m_pszAgency)
    {
         if (m_pProp->m_pbfo->m_pszAgency)
            strcpy(m_pProp->m_pbfo->m_pszAgency,m_pProp->m_pszAgency);
         else
            m_pProp->m_pbfo->m_pszAgency = strdup(m_pProp->m_pszAgency);
    }

	*pError = (EbsIOError) WriteProposal (m_pProp, (PSZ)(LPCTSTR)strFileName);

	return S_OK;
}

