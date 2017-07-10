@echo off
echo Detecting OS processor type
echo Launching Driver Installer
sleep 1

if "%PROCESSOR_ARCHITECTURE%"=="AMD64" goto 64BIT

START "" "%~dp0\Install 32bit Drivers.exe"
GOTO END

:64BIT
START "" "%~dp0\Install 64bit Drivers.exe"


:END
sleep 1