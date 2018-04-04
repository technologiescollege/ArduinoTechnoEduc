@echo off
break ON
cls
rem fichiers BAT et programme créés par Sébastien CANET
cd /d %~dp0
set path=%CD%
ren %CD%\..\..\BlocklyArduinoPlugin\tool\Blockly-at-rduino-gh-pages "Blockly@rduino"