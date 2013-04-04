# Microsoft Developer Studio Project File - Name="libim" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libim - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libim.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libim.mak" CFG="libim - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libim - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libim - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/roq/libim", NCEAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libim - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir "."
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\libsdsc" /I "..\jpeg" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "USE_JPEG_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libim - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir "."
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /I "..\libsdsc" /I "..\jpeg" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "USE_JPEG_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libim - Win32 Release"
# Name "libim - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=im.c
# End Source File
# Begin Source File

SOURCE=imbmp.c
# End Source File
# Begin Source File

SOURCE=imclt.c
# End Source File
# Begin Source File

SOURCE=imcur.c
# End Source File
# Begin Source File

SOURCE=imeps.c
# End Source File
# Begin Source File

SOURCE=imerrno.c
# End Source File
# Begin Source File

SOURCE=imfile.c
# End Source File
# Begin Source File

SOURCE=imfmt.c
# End Source File
# Begin Source File

SOURCE=imgif.c
# End Source File
# Begin Source File

SOURCE=imgiflzw.c
# End Source File
# Begin Source File

SOURCE=imhdfread.c
# End Source File
# Begin Source File

SOURCE=imhdfwrite.c
# End Source File
# Begin Source File

SOURCE=imico.c
# End Source File
# Begin Source File

SOURCE=imicon.c
# End Source File
# Begin Source File

SOURCE=imiff.c
# End Source File
# Begin Source File

SOURCE=imipw.c
# End Source File
# Begin Source File

SOURCE=imjpeg.c
# End Source File
# Begin Source File

SOURCE=imlzw.c
# End Source File
# Begin Source File

SOURCE=immiff.c
# End Source File
# Begin Source File

SOURCE=immpnt.c
# End Source File
# Begin Source File

SOURCE=impbm.c
# End Source File
# Begin Source File

SOURCE=impcx.c
# End Source File
# Begin Source File

SOURCE=impic.c
# End Source File
# Begin Source File

SOURCE=impict.c
# End Source File
# Begin Source File

SOURCE=impix.c
# End Source File
# Begin Source File

SOURCE=imras.c
# End Source File
# Begin Source File

SOURCE=imrgb.c
# End Source File
# Begin Source File

SOURCE=imrla.c
# End Source File
# Begin Source File

SOURCE=imrle.c
# End Source File
# Begin Source File

SOURCE=imschemes.c
# End Source File
# Begin Source File

SOURCE=imsoftimage.c
# End Source File
# Begin Source File

SOURCE=imsynu.c
# End Source File
# Begin Source File

SOURCE=imtga.c
# End Source File
# Begin Source File

SOURCE=imtiff.c
# End Source File
# Begin Source File

SOURCE=imvfb.c
# End Source File
# Begin Source File

SOURCE=imvfbadjust.c
# End Source File
# Begin Source File

SOURCE=imvfbchan.c
# End Source File
# Begin Source File

SOURCE=imvfbcomp.c
# End Source File
# Begin Source File

SOURCE=imvfbflip.c
# End Source File
# Begin Source File

SOURCE=imvfbgamma.c
# End Source File
# Begin Source File

SOURCE=imvfbhist.c
# End Source File
# Begin Source File

SOURCE=imvfblight.c
# End Source File
# Begin Source File

SOURCE=imvfbresize.c
# End Source File
# Begin Source File

SOURCE=imvfbrotate.c
# End Source File
# Begin Source File

SOURCE=imvfbto.c
# End Source File
# Begin Source File

SOURCE=imviff.c
# End Source File
# Begin Source File

SOURCE=imx.c
# End Source File
# Begin Source File

SOURCE=imxbm.c
# End Source File
# Begin Source File

SOURCE=imxpm.c
# End Source File
# Begin Source File

SOURCE=imxwd.c
# End Source File
# Begin Source File

SOURCE=macpack.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\libsdsc\arg.h
# End Source File
# Begin Source File

SOURCE=..\libsdsc\bin.h
# End Source File
# Begin Source File

SOURCE=.\im.h
# End Source File
# Begin Source File

SOURCE=.\imhdfinternal.h
# End Source File
# Begin Source File

SOURCE=.\iminternal.h
# End Source File
# Begin Source File

SOURCE=.\impref.h
# End Source File
# Begin Source File

SOURCE=.\imxpm.h
# End Source File
# Begin Source File

SOURCE=..\jpeg\jconfig.h
# End Source File
# Begin Source File

SOURCE=..\jpeg\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=..\jpeg\jpeglib.h
# End Source File
# Begin Source File

SOURCE=..\libsdsc\sdsc.h
# End Source File
# Begin Source File

SOURCE=..\libsdsc\sdscconfig.h
# End Source File
# Begin Source File

SOURCE=..\libsdsc\sdsccopyright.h
# End Source File
# Begin Source File

SOURCE=..\libsdsc\tag.h
# End Source File
# Begin Source File

SOURCE=.\unistd.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
