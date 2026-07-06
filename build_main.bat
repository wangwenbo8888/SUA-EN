@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64
if errorlevel 1 exit /b 1

cd /d "D:\9706_draintank_EN\MainFrame\HIFUMainFrameWithOperatorNavigation"
if errorlevel 1 exit /b 1

echo === qmake ===
"C:\Qt\Qt5.12.11\5.12.11\msvc2017_64\bin\qmake.exe" "HIFUMainframe.pro" -spec win32-msvc CONFIG+=release CONFIG-=debug CONFIG-=debug_and_release
if errorlevel 1 exit /b 1

echo === nmake clean ===
nmake /f Makefile clean >nul 2>&1

echo === nmake ===
nmake /f Makefile
if errorlevel 1 exit /b 1

echo === SUCCESS ===
