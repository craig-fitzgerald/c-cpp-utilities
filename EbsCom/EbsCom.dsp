# Microsoft Developer Studio Project File - Name="EbsCom" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=EbsCom - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EbsCom.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EbsCom.mak" CFG="EbsCom - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EbsCom - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EbsCom - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EbsCom - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "$(ITIINC)" /I "..\Share" /I "..\GnuLib" /I "..\pgp\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_AS_LIB" /D "_WINDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 gnulibl.lib zlibl.lib comsupp.lib share.lib pgpsdkstatic.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /pdbtype:sept /libpath:"$(ITILIB)\Debug"
# SUBTRACT LINK32 /nodefaultlib
# Begin Custom Build
OutDir=.\Debug
TargetPath=.\Debug\EbsCom.dll
InputPath=.\Debug\EbsCom.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EbsCom - Win32 Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "EbsCom___Win32_Release"
# PROP BASE Intermediate_Dir "EbsCom___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Share" /I "..\GnuLib" /I "..\pgp\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_AS_LIB" /D "_WINDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MT /W3 /GX /I "$(ITIINC)" /I "..\Share" /I "..\GnuLib" /I "..\pgp\inc" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_AS_LIB" /D "_WINDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 gnulibl.lib zlibl.lib comsupp.lib share.lib pgpsdkstatic.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /pdbtype:sept /libpath:"..\Debug" /libpath:"..\pgp\lib" /libpath:"..\GNULIB\DEBUG"
# ADD LINK32 zlibl.lib comsupp.lib share.lib pgpsdkstatic.lib gnulibl.lib /nologo /subsystem:windows /dll /machine:I386 /pdbtype:sept /libpath:"$(ITILIB)\release"
# SUBTRACT LINK32 /debug /nodefaultlib
# Begin Custom Build
OutDir=.\Release
TargetPath=.\Release\EbsCom.dll
InputPath=.\Release\EbsCom.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "EbsCom - Win32 Debug"
# Name "EbsCom - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\dlldatax.c
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\EbsCom.cpp
# End Source File
# Begin Source File

SOURCE=.\EbsCom.def
# End Source File
# Begin Source File

SOURCE=.\EbsCom.idl
# ADD MTL /tlb ".\EbsCom.tlb" /h "EbsCom.h" /iid "EbsCom_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\EbsCom.rc
# End Source File
# Begin Source File

SOURCE=.\EbsFileLib.cpp
# End Source File
# Begin Source File

SOURCE=.\EbsProposal.cpp
# End Source File
# Begin Source File

SOURCE=.\EbsUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\Item.cpp
# End Source File
# Begin Source File

SOURCE=.\Items.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemsEnum.cpp
# End Source File
# Begin Source File

SOURCE=.\Section.cpp
# End Source File
# Begin Source File

SOURCE=.\Sections.cpp
# End Source File
# Begin Source File

SOURCE=.\SectionsEnum.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\dlldatax.h
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\EbsFileLib.h
# End Source File
# Begin Source File

SOURCE=.\EbsProposal.h
# End Source File
# Begin Source File

SOURCE=.\EbsUtils.h
# End Source File
# Begin Source File

SOURCE=.\Item.h
# End Source File
# Begin Source File

SOURCE=.\Items.h
# End Source File
# Begin Source File

SOURCE=.\ItemsEnum.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Section.h
# End Source File
# Begin Source File

SOURCE=.\Sections.h
# End Source File
# Begin Source File

SOURCE=.\SectionsEnum.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\EbsFileLib.rgs
# End Source File
# Begin Source File

SOURCE=.\EbsProposal.rgs
# End Source File
# Begin Source File

SOURCE=.\Item.rgs
# End Source File
# Begin Source File

SOURCE=.\Items.rgs
# End Source File
# Begin Source File

SOURCE=.\ItemsEnum.rgs
# End Source File
# Begin Source File

SOURCE=.\Section.rgs
# End Source File
# Begin Source File

SOURCE=.\Sections.rgs
# End Source File
# Begin Source File

SOURCE=.\SectionsEnum.rgs
# End Source File
# End Group
# End Target
# End Project
