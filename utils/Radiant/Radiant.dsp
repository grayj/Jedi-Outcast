# Microsoft Developer Studio Project File - Name="Radiant" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Radiant - Win32 Q3Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Radiant.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Radiant.mak" CFG="Radiant - Win32 Q3Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Radiant - Win32 Q3Release" (based on "Win32 (x86) Application")
!MESSAGE "Radiant - Win32 Q3Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Utils/Radiant", GKOAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Radiant - Win32 Q3Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Radiant1"
# PROP BASE Intermediate_Dir "Radiant1"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Q3Release"
# PROP Intermediate_Dir "Q3Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GR /GX /Zd /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GR /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "QUAKE3" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib glu32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 winmm.lib ..\libs\pak.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib" /out:"Q3Release/chcRadiant.exe"
# SUBTRACT LINK32 /incremental:yes /debug

!ELSEIF  "$(CFG)" == "Radiant - Win32 Q3Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Radiant2"
# PROP BASE Intermediate_Dir "Radiant2"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Q3Debug"
# PROP Intermediate_Dir "Q3Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GR /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MTd /W3 /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "QUAKE3" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib glu32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 winmm.lib ..\libs\pakd.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib" /out:"Q3Debug/chcRadiant.exe" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "Radiant - Win32 Q3Release"
# Name "Radiant - Win32 Q3Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "QE4 Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Bmp.cpp
# End Source File
# Begin Source File

SOURCE=.\brush.cpp
# End Source File
# Begin Source File

SOURCE=.\cmdlib.cpp
# End Source File
# Begin Source File

SOURCE=.\csg.cpp
# End Source File
# Begin Source File

SOURCE=.\drag.cpp
# End Source File
# Begin Source File

SOURCE=.\eclass.cpp
# End Source File
# Begin Source File

SOURCE=.\entity.cpp
# End Source File
# Begin Source File

SOURCE=.\lbmlib.cpp
# End Source File
# Begin Source File

SOURCE=.\map.cpp
# End Source File
# Begin Source File

SOURCE=.\mathlib.cpp
# End Source File
# Begin Source File

SOURCE=.\mru.cpp
# End Source File
# Begin Source File

SOURCE=.\parse.cpp
# End Source File
# Begin Source File

SOURCE=.\points.cpp
# End Source File
# Begin Source File

SOURCE=.\qe3.cpp
# End Source File
# Begin Source File

SOURCE=.\select.cpp
# End Source File
# Begin Source File

SOURCE=.\vertsel.cpp
# End Source File
# Begin Source File

SOURCE=.\win_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\win_ent.cpp
# End Source File
# Begin Source File

SOURCE=.\win_main.cpp
# End Source File
# Begin Source File

SOURCE=.\win_qe3.cpp
# End Source File
# Begin Source File

SOURCE=.\z.cpp
# End Source File
# End Group
# Begin Group "From others"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fnmatch.cpp
# End Source File
# Begin Source File

SOURCE=.\inc.cpp
# End Source File
# End Group
# Begin Group "Quake 3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Quake3\cbrush.cpp
# End Source File
# Begin Source File

SOURCE=.\Quake3\pmesh.cpp
# End Source File
# End Group
# Begin Group "JPEG Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\jpeg-6\jcomapi.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdapimin.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdapistd.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdatasrc.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdcoefct.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdcolor.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jddctmgr.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdhuff.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdinput.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdmainct.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdmarker.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdmaster.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdpostct.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdsample.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdtrans.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jerror.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jidctflt.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jmemmgr.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jmemnobs.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jutils.c"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "JPEG Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\jpeg-6\jconfig.h"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdct.h"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdhuff.h"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jerror.h"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jinclude.h"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jmemsys.h"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jmorecfg.h"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jpegint.h"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jpeglib.h"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jversion.h"
# End Source File
# End Group
# Begin Source File

SOURCE=.\autocaulk.cpp
# End Source File
# Begin Source File

SOURCE=.\BrushScript.cpp
# End Source File
# Begin Source File

SOURCE=.\BSInput.cpp
# End Source File
# Begin Source File

SOURCE=.\CamWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CapDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\cbrushstub.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CommandsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogTextures.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogThick.cpp
# End Source File
# Begin Source File

SOURCE=.\EditWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\EntityListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EntKeyFindReplace.cpp
# End Source File
# Begin Source File

SOURCE=.\FindTextureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GetString.cpp
# End Source File
# Begin Source File

SOURCE=.\groupnames.cpp
# End Source File
# Begin Source File

SOURCE=.\JPGFile.cpp
# End Source File
# Begin Source File

SOURCE=.\JPGFile.h
# End Source File
# Begin Source File

SOURCE=.\LstToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MapInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\NewProjDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\oddbits.cpp
# End Source File
# Begin Source File

SOURCE=.\PatchDensityDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PlugIn.cpp
# End Source File
# Begin Source File

SOURCE=.\PlugInManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\qgl_win.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\radbsp.cpp
# End Source File
# Begin Source File

SOURCE=.\RADEditWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Radiant.cpp
# End Source File
# Begin Source File

SOURCE=.\RotateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScaleDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShaderInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\sourcesafe.cpp
# End Source File
# Begin Source File

SOURCE=.\SourceSafeSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SurfaceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TexEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureBar.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureLayout.cpp
# End Source File
# Begin Source File

SOURCE=.\TexWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\XYWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\zclip.cpp
# End Source File
# Begin Source File

SOURCE=.\ZWnd.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\3DFXCamWnd.h
# End Source File
# Begin Source File

SOURCE=.\autocaulk.h
# End Source File
# Begin Source File

SOURCE=.\BMP.H
# End Source File
# Begin Source File

SOURCE=.\BRUSH.H
# End Source File
# Begin Source File

SOURCE=.\BSInput.h
# End Source File
# Begin Source File

SOURCE=.\CAMERA.H
# End Source File
# Begin Source File

SOURCE=.\CamWnd.h
# End Source File
# Begin Source File

SOURCE=.\CapDialog.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\CMDLIB.H
# End Source File
# Begin Source File

SOURCE=.\CommandsDlg.h
# End Source File
# Begin Source File

SOURCE=.\DialogInfo.h
# End Source File
# Begin Source File

SOURCE=.\DialogTextures.h
# End Source File
# Begin Source File

SOURCE=.\DialogThick.h
# End Source File
# Begin Source File

SOURCE=.\EditWnd.h
# End Source File
# Begin Source File

SOURCE=.\ENTITY.H
# End Source File
# Begin Source File

SOURCE=.\EntityListDlg.h
# End Source File
# Begin Source File

SOURCE=.\ENTITYW.H
# End Source File
# Begin Source File

SOURCE=.\EntKeyFindReplace.h
# End Source File
# Begin Source File

SOURCE=.\FindTextureDlg.h
# End Source File
# Begin Source File

SOURCE=.\FNMATCH.H
# End Source File
# Begin Source File

SOURCE=.\GetString.h
# End Source File
# Begin Source File

SOURCE=.\groupnames.h
# End Source File
# Begin Source File

SOURCE=.\INC.H
# End Source File
# Begin Source File

SOURCE=.\LBMLIB.H
# End Source File
# Begin Source File

SOURCE=.\LstToolBar.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MAP.H
# End Source File
# Begin Source File

SOURCE=.\MapInfo.h
# End Source File
# Begin Source File

SOURCE=.\MATHLIB.H
# End Source File
# Begin Source File

SOURCE=.\MRU.H
# End Source File
# Begin Source File

SOURCE=.\NewProjDlg.h
# End Source File
# Begin Source File

SOURCE=.\oddbits.h
# End Source File
# Begin Source File

SOURCE=..\Libs\pakstuff.h
# End Source File
# Begin Source File

SOURCE=.\PARSE.H
# End Source File
# Begin Source File

SOURCE=.\PatchDensityDlg.h
# End Source File
# Begin Source File

SOURCE=.\PlugIn.h
# End Source File
# Begin Source File

SOURCE=.\PlugInManager.h
# End Source File
# Begin Source File

SOURCE=.\PrefsDlg.h
# End Source File
# Begin Source File

SOURCE=.\qe3.h
# End Source File
# Begin Source File

SOURCE=.\QEDEFS.H
# End Source File
# Begin Source File

SOURCE=.\qerplugin.h
# End Source File
# Begin Source File

SOURCE=.\QERTYPES.H
# End Source File
# Begin Source File

SOURCE=.\QFILES.H
# End Source File
# Begin Source File

SOURCE=.\QGL.H
# End Source File
# Begin Source File

SOURCE=.\RADEditWnd.h
# End Source File
# Begin Source File

SOURCE=.\Radiant.h
# End Source File
# Begin Source File

SOURCE=.\RadiantDoc.h
# End Source File
# Begin Source File

SOURCE=.\RadiantView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RotateDlg.h
# End Source File
# Begin Source File

SOURCE=.\ScaleDialog.h
# End Source File
# Begin Source File

SOURCE=.\ScriptDlg.h
# End Source File
# Begin Source File

SOURCE=.\SELECT.H
# End Source File
# Begin Source File

SOURCE=.\ShaderInfo.h
# End Source File
# Begin Source File

SOURCE=.\sourcesafe.h
# End Source File
# Begin Source File

SOURCE=.\SourceSafeSettings.h
# End Source File
# Begin Source File

SOURCE=.\ssauterr.h
# End Source File
# Begin Source File

SOURCE=.\ssauto.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SurfaceDlg.h
# End Source File
# Begin Source File

SOURCE=.\TexEdit.h
# End Source File
# Begin Source File

SOURCE=.\TextureBar.h
# End Source File
# Begin Source File

SOURCE=.\TextureLayout.h
# End Source File
# Begin Source File

SOURCE=.\Textures.h
# End Source File
# Begin Source File

SOURCE=.\TexWnd.h
# End Source File
# Begin Source File

SOURCE=.\XY.H
# End Source File
# Begin Source File

SOURCE=.\XYWnd.h
# End Source File
# Begin Source File

SOURCE=.\Z.H
# End Source File
# Begin Source File

SOURCE=.\zclip.h
# End Source File
# Begin Source File

SOURCE=.\ZWnd.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bevel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\endcap.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ibevel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\iendcap.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo_sm3dfx.bmp
# End Source File
# Begin Source File

SOURCE=.\res\q.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Radiant.ico
# End Source File
# Begin Source File

SOURCE=.\Radiant.rc
# End Source File
# Begin Source File

SOURCE=.\res\Radiant.rc2
# End Source File
# Begin Source File

SOURCE=.\res\RadiantDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\viewdefa.bmp
# End Source File
# Begin Source File

SOURCE=.\res\viewoppo.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
