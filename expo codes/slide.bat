@echo off
:loop
set /p mode=<mode.txt
if "%mode%"=="slide" (set /p var=< a.txt

if "%var%"=="left" (start wheelUp.exe

echo "left starts"

Wscript sleep100.vbs

echo "stops"

goto loop
)
if "%var%"=="right" (start wheelDown.exe

echo "right starts"

Wscript sleep100.vbs

echo "stops"

goto loop
)
)
 
echo "slide mode finishes"