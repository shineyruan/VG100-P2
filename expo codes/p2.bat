@echo off
::start VG100_Project2_COM_Bluetooth_Mouse.exe
::ping -n 20 localhost> nul
:loop
set /p mode=<mode.txt
::echo "%mode%"
if "%mode%"=="slide" (
call slide.bat
)
if "%mode%"=="drag" (::echo "in drag mode"
call drag.bat
)
::echo "loop finishes"
goto loop


