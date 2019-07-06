REM DATE=`date +%Y-%m-%d`

For /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%c-%%a-%%b)

set QTDIR=C:\Qt\5.6\mingw49_32\bin-script
set TOOLS=C:\Qt\Tools\mingw492_32\bin

echo %QTDIR%

rd build-win
mkdir build-win
cd build-win
%QTDIR%/qmake.exe ../geco.pro -config release
%TOOLS%/mingw32-make.exe

REM output:

cd ..
cd GecoBuilds

mkdir win
cd win

mkdir Geco%mydate%

copy ..\..\build-win\release\Geco.exe Geco%mydate%\Geco.exe

%QTDIR%/windeployqt.exe Geco%mydate%\Geco.exe

cd ../..
