# Microsoft Developer Studio Project File - Name="jpeg6" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=jpeg6 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "jpeg6.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "jpeg6.mak" CFG="jpeg6 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "jpeg6 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "jpeg6 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Utils/Libs/jpeg6", DINAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "jpeg6 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I ".." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\jpeg6.lib"

!ELSEIF  "$(CFG)" == "jpeg6 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\jpeg6d.lib"

!ENDIF 

# Begin Target

# Name "jpeg6 - Win32 Release"
# Name "jpeg6 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\jcomapi.cpp
# End Source File
# Begin Source File

SOURCE=.\jdapimin.cpp
# End Source File
# Begin Source File

SOURCE=.\jdapistd.cpp
# End Source File
# Begin Source File

SOURCE=.\jdatasrc.cpp
# End Source File
# Begin Source File

SOURCE=.\jdcoefct.cpp
# End Source File
# Begin Source File

SOURCE=.\jdcolor.cpp
# End Source File
# Begin Source File

SOURCE=.\jddctmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\jdhuff.cpp
# End Source File
# Begin Source File

SOURCE=.\jdinput.cpp
# End Source File
# Begin Source File

SOURCE=.\jdmainct.cpp
# End Source File
# Begin Source File

SOURCE=.\jdmarker.cpp
# End Source File
# Begin Source File

SOURCE=.\jdmaster.cpp
# End Source File
# Begin Source File

SOURCE=.\jdpostct.cpp
# End Source File
# Begin Source File

SOURCE=.\jdsample.cpp
# End Source File
# Begin Source File

SOURCE=.\jdtrans.cpp
# End Source File
# Begin Source File

SOURCE=.\jerror.cpp
# End Source File
# Begin Source File

SOURCE=.\jfdctflt.cpp
# End Source File
# Begin Source File

SOURCE=.\jidctflt.cpp
# End Source File
# Begin Source File

SOURCE=.\jmemmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\jmemnobs.cpp
# End Source File
# Begin Source File

SOURCE=.\jpgload.cpp
# End Source File
# Begin Source File

SOURCE=.\jutils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\jchuff.h
# End Source File
# Begin Source File

SOURCE=.\jconfig.h
# End Source File
# Begin Source File

SOURCE=.\jdct.h
# End Source File
# Begin Source File

SOURCE=.\jdhuff.h
# End Source File
# Begin Source File

SOURCE=.\jerror.h
# End Source File
# Begin Source File

SOURCE=.\jinclude.h
# End Source File
# Begin Source File

SOURCE=.\jmemsys.h
# End Source File
# Begin Source File

SOURCE=.\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\jpegint.h
# End Source File
# Begin Source File

SOURCE=..\jpeglib.h
# End Source File
# Begin Source File

SOURCE=.\jversion.h
# End Source File
# End Group
# End Target
# End Project
