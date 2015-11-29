echo off
cls
g++ *.cpp -IC:\\freeglut\\include -LC:\\freeglut\\lib -w -Wl,-subsystem,windows -lOpenGL32 -lglu32 -lfreeGLUT -lgdi32 -o Zsistemasolar.exe
pause
Zsistemasolar
