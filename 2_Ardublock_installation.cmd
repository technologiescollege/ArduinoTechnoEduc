@echo off
break ON
rem fichiers BAT et fork créés par Sébastien CANET
SET currentpath=%~dp1
cd %currentpath%portable\ardublock
cls
xcopy ArduBlock*.* /S/C ..\..\tools
xcopy Matrice\* /S/C ..\..\tools\Matrice\
xcopy Molegraph\* /S/C ..\..\tools\Molegraph\
xcopy bibliotheques_DuinoEdu\* /S/C ..\sketchbook\libraries\