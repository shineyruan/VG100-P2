@echo off
ping -n 10 localhost> nul
for /l %%i in (1,1,300) do (start mouseDrag.exe
Wscript sleep.vbs)
pause