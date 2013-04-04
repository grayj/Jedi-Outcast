del /q vm
mkdir vm
cd vm

set cc=lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1

%cc% ../ui_main.c
@if errorlevel 1 goto quit
%cc% ../../game/bg_misc.c
@if errorlevel 1 goto quit
%cc% ../../game/bg_weapons.c
@if errorlevel 1 goto quit
%cc% ../../game/bg_lib.c
@if errorlevel 1 goto quit
%cc% ../../game/q_math.c
@if errorlevel 1 goto quit
%cc% ../../game/q_shared.c
@if errorlevel 1 goto quit
%cc% ../ui_atoms.c
@if errorlevel 1 goto quit
%cc% ../ui_force.c
@if errorlevel 1 goto quit
%cc% ../ui_util.c
@if errorlevel 1 goto quit
%cc% ../ui_shared.c
@if errorlevel 1 goto quit
%cc% ../ui_gameinfo.c
@if errorlevel 1 goto quit

sysmaker ../ui_public.h ../ui_syscalls.c ../ui_syscalls.asm
@if errorlevel 1 goto quit

q3asm -f ../ui
@if errorlevel 1 goto quit

mkdir "..\..\base\vm"
copy *.map "..\..\base\vm"
copy *.qvm "..\..\base\vm"

:quit
cd ..
