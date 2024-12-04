
@echo off
if "%PROCESSOR_ARCHITECTURE%" == "AMD64" (
  "%~dp0\Drivers\DPinst64.exe"
) ELSE IF "%PROCESSOR_ARCHITEW6432%" == "AMD64" (
  "%~dp0\Drivers\DPinst64.exe"
) ELSE (
  "%~dp0\Drivers\DPinst.exe %ARGS%"
)
