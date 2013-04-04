# Microsoft Developer Studio Project File - Name="WinDed" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=WinDed - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WinDed.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WinDed.mak" CFG="WinDed - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WinDed - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "WinDed - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WinDed - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "DEDICATED" /D "BOTLIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib Winmm.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "WinDed - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "DEDICATED" /D "_JK2" /D "BOTLIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib Winmm.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "WinDed - Win32 Release"
# Name "WinDed - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
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

SOURCE=.\qcommon\disablewarnings.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\files.cpp
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

SOURCE=.\qcommon\q_math.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\q_shared.cpp
# End Source File
# Begin Source File

SOURCE=.\qcommon\qcommon.h
# End Source File
# Begin Source File

SOURCE=.\qcommon\qfiles.h
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
# Begin Group "Null"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\null\null_client.cpp
# End Source File
# Begin Source File

SOURCE=.\null\null_glimp.cpp
# End Source File
# Begin Source File

SOURCE=.\null\null_input.cpp
# End Source File
# Begin Source File

SOURCE=.\null\null_renderer.cpp
# End Source File
# Begin Source File

SOURCE=.\null\null_snddma.cpp
# End Source File
# Begin Source File

SOURCE=.\null\win_main.cpp
# End Source File
# End Group
# Begin Group "Renderer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\renderer\matcomp.c
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_backend.cpp
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

SOURCE=.\renderer\tr_main.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_mesh.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_model.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer\tr_shader.cpp
# End Source File
# End Group
# Begin Group "Win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\win32\win_net.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\win_shared.cpp
# End Source File
# End Group
# Begin Group "botlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\botlib\aasfile.h
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_bsp.h
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_bspq3.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_cluster.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_cluster.h
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_debug.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_debug.h
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_def.h
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_entity.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_entity.h
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_file.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_file.h
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_funcs.h
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_main.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_main.h
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_move.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_move.h
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_optimize.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_optimize.h
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_reach.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_reach.h
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_route.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_route.h
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_routealt.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_routealt.h
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_sample.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_aas_sample.h
# End Source File
# Begin Source File

SOURCE=.\botlib\be_ai_char.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_ai_chat.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_ai_gen.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_ai_goal.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_ai_move.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_ai_weap.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_ai_weight.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_ai_weight.h
# End Source File
# Begin Source File

SOURCE=.\botlib\be_ea.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\be_interface.h
# End Source File
# Begin Source File

SOURCE=.\botlib\l_crc.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\l_crc.h
# End Source File
# Begin Source File

SOURCE=.\botlib\l_libvar.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\l_libvar.h
# End Source File
# Begin Source File

SOURCE=.\botlib\l_log.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\l_log.h
# End Source File
# Begin Source File

SOURCE=.\botlib\l_memory.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\l_memory.h
# End Source File
# Begin Source File

SOURCE=.\botlib\l_precomp.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\l_precomp.h
# End Source File
# Begin Source File

SOURCE=.\botlib\l_script.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\l_script.h
# End Source File
# Begin Source File

SOURCE=.\botlib\l_struct.cpp
# End Source File
# Begin Source File

SOURCE=.\botlib\l_struct.h
# End Source File
# Begin Source File

SOURCE=.\botlib\l_utils.h
# End Source File
# End Group
# End Target
# End Project
