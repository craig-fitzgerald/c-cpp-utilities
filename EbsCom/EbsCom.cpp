// EbsCom.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To merge the proxy/stub code into the object DLL, add the file 
//      dlldatax.c to the project.  Make sure precompiled headers 
//      are turned off for this file, and add _MERGE_PROXYSTUB to the 
//      defines for the project.  
//
//      If you are not running WinNT4.0 or Win95 with DCOM, then you
//      need to remove the following define from dlldatax.c
//      #define _WIN32_WINNT 0x0400
//
//      Further, if you are running MIDL without /Oicf switch, you also 
//      need to remove the following define from dlldatax.c.
//      #define USE_STUBLESS_PROXY
//
//      Modify the custom build rule for EbsCom.m_idl by adding the following 
//      files to the Outputs.
//          EbsCom_p.c
//          dlldata.c
//      To build a separate proxy/stub DLL, 
//      run nmake -f EbsComps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "EbsCom.h"
#include "dlldatax.h"

#include "EbsCom_i.c"
#include "EbsUtils.h"
#include "Sections.h"
#include "Section.h"
#include "SectionsEnum.h"
#include "Items.h"
#include "ItemsEnum.h"
#include "Item.h"
//TODO upgrade <GL>: remove comment.#include "DBEs.h"
//TODO upgrade <GL>: remove comment.#include "DBEsEnum.h"
//TODO upgrade <GL>: remove comment.#include "DBE.h"
//TODO upgrade <GL>: remove comment.#include "DBEItems.h"
//TODO upgrade <GL>: remove comment.#include "DBEItemsEnum.h"
//TODO upgrade <GL>: remove comment.#include "DBEItem.h"
//TODO upgrade <GL>: remove comment.#include "EbsDBEFieldValues.h"
//TODO upgrade <GL>: remove comment.#include "EbsDBEFieldValuesEnum.h"
//TODO upgrade <GL>: remove comment.#include "EbsDBEFieldValue.h"
#include "EbsProposal.h"
#include "GnuMath.h"
#include "EbsDat.h"
#include "EbsFileLib.h"

#ifdef _MERGE_PROXYSTUB
extern "C" HINSTANCE hProxyDll;
#endif

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_EbsSections, EbsSections)
OBJECT_ENTRY(CLSID_EbsSection, EbsSection)
OBJECT_ENTRY(CLSID_EbsSectionsEnum, EbsSectionsEnum)
OBJECT_ENTRY(CLSID_EbsItems, EbsItems)
OBJECT_ENTRY(CLSID_EbsItemsEnum, EbsItemsEnum)
OBJECT_ENTRY(CLSID_EbsItem, EbsItem)
//TODO upgrade <GL>: remove comment.OBJECT_ENTRY(CLSID_EbsDBEs, EbsDBEs)
//TODO upgrade <GL>: remove comment.OBJECT_ENTRY(CLSID_EbsDBEsEnum, EbsDBEsEnum)
//TODO upgrade <GL>: remove comment.OBJECT_ENTRY(CLSID_EbsDBE, EbsDBE)
//TODO upgrade <GL>: remove comment.OBJECT_ENTRY(CLSID_EbsDBEItems, EbsDBEItems)
//TODO upgrade <GL>: remove comment.OBJECT_ENTRY(CLSID_EbsDBEItemsEnum, EbsDBEItemsEnum)
//TODO upgrade <GL>: remove comment.OBJECT_ENTRY(CLSID_EbsDBEItem, EbsDBEItem)
//TODO upgrade <GL>: remove comment.OBJECT_ENTRY(CLSID_EbsDBEFieldValues, EbsDBEFieldValues)
//TODO upgrade <GL>: remove comment.OBJECT_ENTRY(CLSID_EbsDBEFieldValuesEnum, EbsDBEFieldValuesEnum)
//TODO upgrade <GL>: remove comment.OBJECT_ENTRY(CLSID_EbsDBEFieldValue, EbsDBEFieldValue)
OBJECT_ENTRY(CLSID_EbsProposal, CEbsProposal)
OBJECT_ENTRY(CLSID_EbsFileLib, CEbsFileLib)
END_OBJECT_MAP()

class CEbsComApp : public CWinApp
{
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEbsComApp)
	public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CEbsComApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CEbsComApp, CWinApp)
	//{{AFX_MSG_MAP(CEbsComApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CEbsComApp theApp;

BOOL CEbsComApp::InitInstance()
{
#ifdef _MERGE_PROXYSTUB
    hProxyDll = m_hInstance;
#endif
    _Module.Init(ObjectMap, m_hInstance, &LIBID_EBSCOMLib);
    return CWinApp::InitInstance();
}

int CEbsComApp::ExitInstance()
{
	_Module.Term();
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
#ifdef _MERGE_PROXYSTUB
    if (PrxDllCanUnloadNow() != S_OK)
        return S_FALSE;
#endif
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
#ifdef _MERGE_PROXYSTUB
    if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
        return S_OK;
#endif
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
#ifdef _MERGE_PROXYSTUB
    HRESULT hRes = PrxDllRegisterServer();
    if (FAILED(hRes))
        return hRes;
#endif
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
#ifdef _MERGE_PROXYSTUB
    PrxDllUnregisterServer();
#endif
    return _Module.UnregisterServer(TRUE);
}

