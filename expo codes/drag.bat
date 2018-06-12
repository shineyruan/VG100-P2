@echo off
:loop
set /p mode=<mode.txt
if "%mode%"=="drag" (echo "start drag"
start mouseDrag.exe
Wscript sleep.vbs
goto loop
) else (echo "drag finishes")
