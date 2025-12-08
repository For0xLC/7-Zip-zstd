@echo off
setlocal enabledelayedexpansion

set i=0
:loop
if "%~1"=="" goto list
set /a i+=1
set "param[!i!]=%~1"
shift
goto loop

:list
echo Parameters:
echo -----------
for /l %%n in (1,1,!i!) do (
    echo !param[%%n]!
)
echo -----------
echo Total: !i!
pause