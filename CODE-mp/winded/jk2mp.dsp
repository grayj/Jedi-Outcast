# Microsoft Developer Studio Project File - Name="jk2mp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=jk2mp - Win32 Debug JK2
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "jk2mp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "jk2mp.mak" CFG="jk2mp - Win32 Debug JK2"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "jk2mp - Win32 Release JK2" (based on "Win32 (x86) Application")
!MESSAGE "jk2mp - Win32 Debug JK2" (based on "Win32 (x86) Application")
!MESSAGE "jk2mp - Win32 Final JK2" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""$/General/code", EAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "jk2mp - Win32 Release JK2"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release\jk2"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release\jk2"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /W4 /GX /O2 /Ob2 /D "WIN32" /D "NDebug" /D "_WINDOWS" /D "__USEA3D" /D "__A3D_GEOM" /YX /FD /c
# ADD CPP /nologo /G6 /W4 /GX /Zi /O2 /Ob0 /D "_WIN32" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_JK2" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDebug" /mktyplib203 /win32
# ADD MTL /nologo /D "NDebug" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDebug"
# ADD RSC /l 0x409 /d "NDebug"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 advapi32.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib ole32.lib /nologo /stack:0x800000 /subsystem:windows /map /debug /machine:I386
# SUBTRACT BASE LINK32 /incremental:yes /nodefaultlib
# ADD LINK32 advapi32.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib ole32.lib /nologo /stack:0x800000 /subsystem:windows /map:"Release/jk2mp.map" /debug /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "jk2mp - Win32 Debug JK2"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug\jk2"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug\jk2"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_Debug" /D "_WINDOWS" /D "__USEA3D" /D "__A3D_GEOM" /Fr /YX /FD /c
# ADD CPP /nologo /G6 /W3 /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_JK2" /Fr /FD /GZ /c
# ADD BASE MTL /nologo /D "_Debug" /mktyplib203 /win32
# ADD MTL /nologo /D "_Debug" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /fo"win32\winquake.res" /d "_Debug"
# ADD RSC /l 0x409 /fo"win32\winquake.res" /d "_Debug"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"Debug/jk2mp.bsc"
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 advapi32.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib ole32.lib /nologo /stack:0x800000 /subsystem:windows /profile /map /debug /machine:I386
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 advapi32.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib ole32.lib /nologo /stack:0x800000 /subsystem:windows /map:"Debug/jk2mp.map" /debug /machine:I386
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "jk2mp - Win32 Final JK2"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Final"
# PROP BASE Intermediate_Dir ".\Final\jk2"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Final"
# PROP Intermediate_Dir ".\Final\jk2"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /W4 /GX /Zi /O2 /Ob0 /I "./jk2/game" /I "." /D "_WIN32" /D "NDebug" /D "WIN32" /D "_WINDOWS" /D "_JK2" /YX /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /W4 /GX /O2 /Ob0 /D "_WIN32" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "_JK2" /D "FINAL_BUILD" /YX /FD /c
# ADD BASE MTL /nologo /D "NDebug" /mktyplib203 /win32
# ADD MTL /nologo /D "NDebug" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDebug"
# ADD RSC /l 0x409 /d "NDebug"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 advapi32.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib ole32.lib /nologo /stack:0x800000 /subsystem:windows /map:"Release/jk2mp.map" /debug /machine:I386 /out:".\Release/jk2mp.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 advapi32.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib ole32.lib /nologo /stack:0x800000 /subsystem:windows /map:"Final/jk2mp.map" /machine:I386
# SUBTRACT LINK32 /pdb:none /debug

!ENDIF 

# Begin Target

# Name "jk2mp - Win32 Release JK2"
# Name "jk2mp - Win32 Debug JK2"
# Name "jk2mp - Win32 Final JK2"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\game\anims.h
# End Source File
# Begin Source File

SOURCE=.\game\bg_weapons.h
# End Source File
# Begin Source File

SOURCE=.\game\botlib.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\disablewarnings.h
# End Source File
# Begin Source File

SOURCE=.\win32\resource.h
# End Source File
# Begin Source File

SOURCE=.\ui\ui_public.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\win32\qe3.ico
# End Source File
# Begin Source File

SOURCE=.\win32\winquake.rc

!IF  "$(CFG)" == "jk2mp - Win32 Release JK2"

!ELSEIF  "$(CFG)" == "jk2mp - Win32 Debug JK2"

# ADD BASE RSC /l 0x409 /i "win32"
# ADD RSC /l 0x409 /i "win32"

!ELSEIF  "$(CFG)" == "jk2mp - Win32 Final JK2"

!ENDIF 

# End Source File
# End Group
# Begin Group "Client"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\client\cl_cgame.cpp

!IF  "$(CFG)" == "jk2mp - Win32 Release JK2"

# ADD CPP /Ob0

!ELSEIF  "$(CFG)" == "jk2mp - Win32 Debug JK2"

!ELSEIF  "$(CFG)" == "jk2mp - Win32 Final JK2"

# ADD BASE CPP /Ob0
# ADD CPP /Ob0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\client\cl_cin.cpp
# End Source File
# Begin Source File

SOURCE=.\client\cl_console.cpp
# End Source File
# Begin Source File

SOURCE=.\client\cl_input.cpp
# End Source File
# Begin Source File

SOURCE=.\client\cl_keys.cpp
# End Source File
# Begin Source File

SOURCE=.\client\cl_main.cpp
# End Source File
# Begin Source File

SOURCE=.\client\cl_net_chan.cpp
# End Source File
# Begin Source File

SOURCE=.\client\cl_parse.cpp
# End Source File
# Begin Source File

SOURCE=.\client\cl_scrn.cpp
# End Source File
# Begin Source File

SOURCE=.\client\cl_ui.cpp
# End Source File
# Begin Source File

SOURCE=.\client\client.h
# End Source File
# Begin Source File

SOURCE=.\client\FXExport.cpp
# End Source File
# Begin Source File

SOURCE=.\client\FXExport.h
# End Source File
# Begin Source File

SOURCE=.\client\FxPrimitives.cpp
# End Source File
# Begin Source File

SOURCE=.\client\FxPrimitives.h
# End Source File
# Begin Source File

SOURCE=.\client\FxScheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\client\FxScheduler.h
# End Source File
# Begin Source File

SOURCE=.\client\FxSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\client\FxSystem.h
# End Source File
# Begin Source File

SOURCE=.\client\FxTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\client\FxUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\client\FxUtil.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\bg_public.h
# End Source File
# Begin Source File

SOURCE=.\cgame\cg_public.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\chash.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\cm_load.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\cm_local.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\cm_patch.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\cm_patch.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\cm_polylib.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\cm_polylib.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\cm_public.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\cm_shader.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\cm_test.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\cm_trace.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\cmd.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\CNetProfile.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\common.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\cvar.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\files.cpp
# End Source File
# Begin Source File

SOURCE=.\game\g_public.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\game_version.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\GenericParser2.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\GenericParser2.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\hstring.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\hstring.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\huffman.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\INetProfile.h
# End Source File
# Begin Source File

SOURCE=.\ui\keycodes.h
# End Source File
# Begin Source File

SOURCE=.\client\keys.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\md4.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\MiniHeap.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\msg.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\net_chan.cpp
# End Source File
# Begin Source File

SOURCE=.\game\q_math.c

!IF  "$(CFG)" == "jk2mp - Win32 Release JK2"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jk2mp - Win32 Debug JK2"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jk2mp - Win32 Final JK2"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\qcommon\q_math.cpp
# End Source File
# Begin Source File

SOURCE=.\game\q_shared.c

!IF  "$(CFG)" == "jk2mp - Win32 Release JK2"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jk2mp - Win32 Debug JK2"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jk2mp - Win32 Final JK2"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\qcommon\q_shared.cpp
# End Source File
# Begin Source File

SOURCE=.\game\q_shared.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\qcommon.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\qfiles.h
# End Source File
# Begin Source File

SOURCE=.\renderer\qgl.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\RoffSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\RoffSystem.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\sstring.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\strip.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\strip.h
# End Source File
# Begin Source File

SOURCE=.\game\surfaceflags.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\tags.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\unzip.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\unzip.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\vm.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\vm_interpreted.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\vm_local.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\vm_x86.cpp
# End Source File
# End Group
# Begin Group "Win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\win32\win_input.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\win_local.h
# End Source File
# Begin Source File

SOURCE=.\win32\win_main.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\win_net.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\win_shared.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\win_snd.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\win_syscon.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\win_wndproc.cpp
# End Source File
# End Group
# Begin Group "Server"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\server\server.h
# End Source File
# Begin Source File

SOURCE=.\server\sv_bot.cpp
# End Source File
# Begin Source File

SOURCE=.\server\sv_ccmds.cpp
# End Source File
# Begin Source File

SOURCE=.\server\sv_client.cpp
# End Source File
# Begin Source File

SOURCE=.\server\sv_game.cpp
# End Source File
# Begin Source File

SOURCE=.\server\sv_init.cpp
# End Source File
# Begin Source File

SOURCE=.\server\sv_main.cpp
# End Source File
# Begin Source File

SOURCE=.\server\sv_net_chan.cpp
# End Source File
# Begin Source File

SOURCE=.\server\sv_snapshot.cpp
# End Source File
# Begin Source File

SOURCE=.\server\sv_world.cpp
# End Source File
# End Group
# Begin Group "Sound"

# PROP Default_Filter ""
# Begin Group "MP3"

# PROP Default_Filter ""
# Begin Group "MP3 Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mp3code\config.h
# End Source File
# Begin Source File

SOURCE=.\mp3code\copyright.h
# End Source File
# Begin Source File

SOURCE=.\mp3code\htable.h
# End Source File
# Begin Source File

SOURCE=.\mp3code\jdw.h
# End Source File
# Begin Source File

SOURCE=.\mp3code\l3.h
# End Source File
# Begin Source File

SOURCE=.\mp3code\mhead.h
# End Source File
# Begin Source File

SOURCE=.\mp3code\mp3struct.h
# End Source File
# Begin Source File

SOURCE=.\mp3code\port.h
# End Source File
# Begin Source File

SOURCE=.\mp3code\small_header.h
# End Source File
# Begin Source File

SOURCE=.\mp3code\tableawd.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\mp3code\cdct.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\csbt.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\csbtb.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\csbtl3.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\cup.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\cupini.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\cupl1.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\cupl3.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\cwin.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\cwinb.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\cwinm.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\hwin.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\l3dq.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\l3init.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\mdct.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\mhead.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\msis.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\towave.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\uph.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\upsf.c
# End Source File
# Begin Source File

SOURCE=.\mp3code\wavep.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\client\snd_dma.cpp
# End Source File
# Begin Source File

SOURCE=.\client\snd_local.h
# End Source File
# Begin Source File

SOURCE=.\client\snd_mem.cpp
# End Source File
# Begin Source File

SOURCE=.\client\snd_mix.cpp
# End Source File
# Begin Source File

SOURCE=.\client\snd_mp3.cpp
# End Source File
# Begin Source File

SOURCE=.\client\snd_mp3.h
# End Source File
# Begin Source File

SOURCE=.\client\snd_public.h
# End Source File
# End Group
# Begin Group "Renderer"

# PROP Default_Filter ""
# Begin Group "jpeg files"

# PROP Default_Filter ""
# Begin Group "JPG Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\jpeg-6\jcapimin.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jccoefct.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jccolor.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jcdctmgr.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jchuff.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jcinit.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jcmainct.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jcmarker.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jcmaster.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jcomapi.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jcparam.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jcphuff.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jcprepct.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jcsample.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jctrans.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdapimin.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdapistd.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdatadst.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdatasrc.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdcoefct.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdcolor.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jddctmgr.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdhuff.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdinput.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdmainct.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdmarker.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdmaster.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdpostct.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdsample.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jdtrans.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jerror.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jfdctflt.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jidctflt.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jmemmgr.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jmemnobs.cpp"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6\jutils.cpp"
# End Source File
# End Group
# Begin Group "JPG Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\jpeg-6\jchuff.h"
# End Source File
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
# Begin Source File

SOURCE=.\ft2\sfdriver.h
# End Source File
# Begin Source File

SOURCE=.\ft2\sfobjs.h
# End Source File
# Begin Source File

SOURCE=.\ft2\ttcmap.h
# End Source File
# Begin Source File

SOURCE=.\ft2\ttload.h
# End Source File
# Begin Source File

SOURCE=.\ft2\ttpost.h
# End Source File
# Begin Source File

SOURCE=.\ft2\ttsbit.h
# End Source File
# End Group
# End Group
# Begin Group "png"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\png\png.cpp
# End Source File
# Begin Source File

SOURCE=.\png\png.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\renderer\glext.h
# End Source File
# Begin Source File

SOURCE=.\win32\glw_win.h
# End Source File
# Begin Source File

SOURCE=.\renderer\matcomp.c
# End Source File
# Begin Source File

SOURCE=.\renderer\matcomp.h
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_animation.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_backend.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_bsp.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_cmds.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_curve.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_flares.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_font.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_font.h
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_ghoul2.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_image.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_init.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_light.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_local.h
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_main.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_marks.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_mesh.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_model.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_noise.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_public.h
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_quicksprite.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_quicksprite.h
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_scene.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_shade.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_shade_calc.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_shader.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_shadows.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_sky.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_surface.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_surfacesprites.cpp
# End Source File
# Begin Source File

SOURCE=.\cgame\tr_types.h
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_world.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_WorldEffects.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_WorldEffects.h
# End Source File
# Begin Source File

SOURCE=.\win32\win_gamma.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\win_glimp.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\win_qgl.cpp
# End Source File
# End Group
# Begin Group "Ghoul2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ghoul2\G2.h
# End Source File
# Begin Source File

SOURCE=.\ghoul2\G2_API.cpp
# End Source File
# Begin Source File

SOURCE=.\ghoul2\G2_bolts.cpp
# End Source File
# Begin Source File

SOURCE=.\ghoul2\G2_bones.cpp
# End Source File
# Begin Source File

SOURCE=.\ghoul2\G2_gore.h
# End Source File
# Begin Source File

SOURCE=.\ghoul2\G2_local.h
# End Source File
# Begin Source File

SOURCE=.\ghoul2\G2_misc.cpp
# End Source File
# Begin Source File

SOURCE=.\ghoul2\G2_surfaces.cpp
# End Source File
# Begin Source File

SOURCE=.\ghoul2\ghoul2_shared.h
# End Source File
# Begin Source File

SOURCE=.\renderer\mdx_format.h
# End Source File
# End Group
# Begin Group "zlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zlib\adler32.c
# End Source File
# Begin Source File

SOURCE=.\zlib\crc32.cpp
# End Source File
# Begin Source File

SOURCE=.\zlib\deflate.c
# End Source File
# Begin Source File

SOURCE=.\zlib\deflate.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infblock.c
# End Source File
# Begin Source File

SOURCE=.\zlib\infblock.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infcodes.c
# End Source File
# Begin Source File

SOURCE=.\zlib\infcodes.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inffast.c
# End Source File
# Begin Source File

SOURCE=.\zlib\inffast.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inffixed.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inflate.c
# End Source File
# Begin Source File

SOURCE=.\zlib\inftrees.c
# End Source File
# Begin Source File

SOURCE=.\zlib\inftrees.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infutil.c
# End Source File
# Begin Source File

SOURCE=.\zlib\infutil.h
# End Source File
# Begin Source File

SOURCE=.\zlib\trees.c
# End Source File
# Begin Source File

SOURCE=.\zlib\trees.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zlib.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zutil.c
# End Source File
# Begin Source File

SOURCE=.\zlib\zutil.h
# End Source File
# End Group
# Begin Group "strings"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\strings\con_text.h
# End Source File
# Begin Source File

SOURCE=.\strings\str_server.h
# End Source File
# End Group
# Begin Group "encryption"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\encryption\buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\encryption\buffer.h
# End Source File
# Begin Source File

SOURCE=.\encryption\cpp_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\encryption\cpp_interface.h
# End Source File
# Begin Source File

SOURCE=.\encryption\encryption.h
# End Source File
# Begin Source File

SOURCE=.\encryption\sockets.cpp
# End Source File
# Begin Source File

SOURCE=.\encryption\sockets.h
# End Source File
# End Group
# End Target
# End Project
