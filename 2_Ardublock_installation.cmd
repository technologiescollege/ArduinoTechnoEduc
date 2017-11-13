@echo off
break ON
rem fichiers BAT et fork créés par Sébastien CANET
SET currentpath=%~dp1
cd %currentpath%portable\ardublock
cls
xcopy ArduBlock*.* /S/C ..\..\tools
pause
xcopy "..\sketchbook\6.complements\DuinoEdu_bibliotheques" /S/C ..\sketchbook\libraries
pause
xcopy "4.DuinoEDU - Ardublock" /S/C/I "..\sketchbook\4.DuinoEDU - Ardublock"
pause
rd /s/q "..\sketchbook\4.TechnoZone51 - Organigram"
pause
rd /s/q "..\sketchbook\6.complements\DuinoEdu_bibliotheques"
pause
cd ..
rd /s/q ardublock
pause