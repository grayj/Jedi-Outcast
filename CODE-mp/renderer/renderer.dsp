# Microsoft Developer Studio Project File - Name="renderer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=renderer - Win32 Debug TA
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "renderer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "renderer.mak" CFG="renderer - Win32 Debug TA"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "renderer - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "renderer - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "renderer - Win32 Release TA" (based on "Win32 (x86) Static Library")
!MESSAGE "renderer - Win32 Debug TA" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MissionPack/code/renderer", EJBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "renderer - Win32 Release"

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
# ADD CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug"

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
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "renderer - Win32 Release TA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "renderer___Win32_Release_TA"
# PROP BASE Intermediate_Dir "renderer___Win32_Release_TA"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_TA"
# PROP Intermediate_Dir "Release_TA"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "__USEA3D" /D "__A3D_GEOM" /YX /FD /c
# ADD CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "renderer - Win32 Debug TA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "renderer___Win32_Debug_TA"
# PROP BASE Intermediate_Dir "renderer___Win32_Debug_TA"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_TA"
# PROP Intermediate_Dir "Debug_TA"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "__USEA3D" /D "__A3D_GEOM" /FR /YX /FD /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "renderer - Win32 Release"
# Name "renderer - Win32 Debug"
# Name "renderer - Win32 Release TA"
# Name "renderer - Win32 Debug TA"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ref_trin.def
# End Source File
# Begin Source File

SOURCE=.\tr_animation.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_backend.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_bsp.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_cmds.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_curve.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_flares.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_font.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_image.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_init.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_light.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_main.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_marks.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_mesh.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_model.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_noise.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_scene.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_shade.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_shade_calc.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_shader.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_shadows.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_sky.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_surface.cpp
# End Source File
# Begin Source File

SOURCE=.\tr_world.cpp
# End Source File
# Begin Source File

SOURCE=..\win32\win_gamma.cpp
# End Source File
# Begin Source File

SOURCE=..\win32\win_glimp.cpp
# End Source File
# Begin Source File

SOURCE=..\win32\win_qgl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\qcommon\cm_public.h
# End Source File
# Begin Source File

SOURCE=..\win32\glw_win.h
# End Source File
# Begin Source File

SOURCE=..\game\q_shared.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\qcommon.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\qfiles.h
# End Source File
# Begin Source File

SOURCE=.\qgl.h
# End Source File
# Begin Source File

SOURCE=..\game\surfaceflags.h
# End Source File
# Begin Source File

SOURCE=.\tr_local.h
# End Source File
# Begin Source File

SOURCE=.\tr_public.h
# End Source File
# Begin Source File

SOURCE=..\cgame\tr_types.h
# End Source File
# Begin Source File

SOURCE=..\win32\win_local.h
# End Source File
# End Group
# Begin Group "jpeg"

# PROP Default_Filter ""
# Begin Group "Source Files No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\jpeg-6\jcapimin.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jccoefct.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jccolor.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jcdctmgr.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jchuff.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jcinit.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jcmainct.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jcmarker.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jcmaster.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jcomapi.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jcparam.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jcphuff.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jcprepct.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jcsample.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jctrans.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jdapimin.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jdapistd.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jdatadst.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jdatasrc.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jdcoefct.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jdcolor.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jddctmgr.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jdhuff.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jdinput.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jdmainct.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jdmarker.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jdmaster.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jdpostct.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jdsample.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jdtrans.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jerror.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jfdctflt.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jidctflt.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jmemmgr.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jmemnobs.cpp"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jutils.cpp"
# End Source File
# End Group
# Begin Group "Header Files No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\jpeg-6\jchuff.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jconfig.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jdct.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jdhuff.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jerror.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jinclude.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jmemsys.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jmorecfg.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jpegint.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jpeglib.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6\jversion.h"
# End Source File
# Begin Source File

SOURCE=..\ft2\sfdriver.h
# End Source File
# Begin Source File

SOURCE=..\ft2\sfobjs.h
# End Source File
# Begin Source File

SOURCE=..\ft2\ttcmap.h
# End Source File
# Begin Source File

SOURCE=..\ft2\ttload.h
# End Source File
# Begin Source File

SOURCE=..\ft2\ttpost.h
# End Source File
# Begin Source File

SOURCE=..\ft2\ttsbit.h
# End Source File
# End Group
# End Group
# Begin Group "FreeType2"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Include files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\ft2\ahangles.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ahglobal.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ahglyph.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ahhint.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ahloader.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ahmodule.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ahoptim.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ahtypes.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\autohint.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\freetype.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftbbox.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftcalc.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftconfig.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftdebug.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftdriver.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\fterrors.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftextend.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftglyph.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftgrays.h
# End Source File
# Begin Source File

SOURCE=..\ft2\ftimage.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftlist.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftmemory.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftmm.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftmodule.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftnames.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftobjs.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftoption.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftoutln.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftraster.h
# End Source File
# Begin Source File

SOURCE=..\ft2\ftrend1.h
# End Source File
# Begin Source File

SOURCE=..\ft2\ftrender.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftsmooth.h
# End Source File
# Begin Source File

SOURCE=..\ft2\ftstream.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ftsystem.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\fttypes.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\psnames.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\sfnt.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\t1errors.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\t1tables.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\t1types.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\t2errors.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\t2types.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ttdriver.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\tterrors.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ttgload.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ttinterp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ttnameid.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ttobjs.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\ttpload.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\tttables.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\tttags.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ft2\tttypes.h
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Source File

SOURCE=..\ft2\ahangles.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ahglobal.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ahglyph.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ahhint.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ahmodule.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ahoptim.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ftcalc.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ftdebug.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ftextend.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ftglyph.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ftgrays.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ftinit.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ftlist.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ftmm.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ftnames.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ftobjs.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ftoutln.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ftraster.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ftrend1.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ftsmooth.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ftstream.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ftsystem.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\sfdriver.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\sfobjs.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ttcmap.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ttdriver.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ttgload.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ttinterp.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ttload.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ttobjs.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ttpload.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ttpost.cpp
# End Source File
# Begin Source File

SOURCE=..\ft2\ttsbit.cpp
# End Source File
# End Group
# End Target
# End Project
