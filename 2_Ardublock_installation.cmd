@echo off
break ON
rem fichiers BAT et fork créés par Sébastien CANET
SET currentpath=%~dp1
cd %currentpath%portable\ardublock
cls
xcopy ArduBlockTool*.* /S/C ..\..\tools
pause
xcopy Matrice*.* /S/C ..\..\tools
pause
xcopy "..\sketchbook\7.complements\DuinoEdu_bibliotheques" /S/C ..\sketchbook\libraries
pause
xcopy "Ardublock - DuinoEDU" /S/C/I "..\sketchbook\5.Ardublock - DuinoEDU"
pause
rd /s/q "..\sketchbook\5.Organigram TZ51"
pause
rd /s/q "..\sketchbook\7.complements\DuinoEdu_bibliotheques"
pause
cd ..
rd /s/q ardublock
pause