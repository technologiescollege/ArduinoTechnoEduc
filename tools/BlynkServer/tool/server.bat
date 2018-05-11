break ON
cls
rem fichiers BAT et programme créés par Sébastien CANET, v0.36.1
cd /d %~dp0
set path=%CD%
%path%\..\..\..\java\bin\java.exe -jar server.jar -dataFolder %path%