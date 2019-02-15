; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCryptDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "crypt.h"
LastPage=0

ClassCount=3
Class1=CCryptApp
Class2=CAboutDlg
Class3=CCryptDlg

ResourceCount=2
Resource1=IDD_ABOUTBOX
Resource2=IDD_CRYPT_DIALOG

[CLS:CCryptApp]
Type=0
BaseClass=CWinApp
HeaderFile=crypt.h
ImplementationFile=crypt.cpp
Filter=N
VirtualFilter=AC
LastObject=CCryptApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=cryptDlg.cpp
ImplementationFile=cryptDlg.cpp
LastObject=CAboutDlg

[CLS:CCryptDlg]
Type=0
BaseClass=CDialog
HeaderFile=cryptDlg.h
ImplementationFile=cryptDlg.cpp
Filter=D
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CRYPT_DIALOG]
Type=1
Class=CCryptDlg
ControlCount=7
Control1=IDC_BTN_CRYPT,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=ID_HELP,button,1342242816
Control4=IDC_LABEL_PASSWORD,static,1342308353
Control5=IDC_EDIT_PASSWORD,edit,1350631584
Control6=IDC_LABEL_PASSWORD2,static,1342308353
Control7=IDC_EDIT_PASSWORD2,edit,1350631584

