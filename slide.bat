@echo off
:loop
set /p var=< a.txt
if "%var%"=="left" (start wheelUp.exe
echo "left starts"
Wscript sleep2000.vbs
echo "stops"
) else if "%var%"=="right" (start wheelDown.exe
echo "right starts"
Wscript sleep2000.vbs
echo "stops"
)
goto loop