REM DATE=`date +%Y-%m-%d`

For /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%c-%%a-%%b)

set QTDIR=C:\Qt\Qt5.8.0\5.8\mingw53_32\bin
set TOOLS=C:\Qt\Qt5.8.0\Tools\mingw530_32\bin

echo %QTDIR%

mkdir build-win
cd build-win
rd full
mkdir full
cd full
%QTDIR%/qmake.exe ../../geco.pro -config release
%TOOLS%/mingw32-make.exe
REM %QTDIR%/windeployqt release/Geco.exe

cd ..
rd light
mkdir light
cd light
%QTDIR%/qmake.exe ../../geco_light.pro -config release
%TOOLS%/mingw32-make.exe
REM %QTDIR%/windeployqt.exe release/GecoLight.exe

REM output:

cd ../..
cd GecoBuilds

mkdir win
cd win

mkdir Geco%mydate%
mkdir GecoLight%mydate%

copy ..\..\build-win\full\release\Geco.exe Geco%mydate%\Geco.exe
copy ..\..\build-win\light\release\GecoLight.exe GecoLight%mydate%\GecoLight.exe

%QTDIR%/windeployqt.exe Geco%mydate%\Geco.exe
%QTDIR%/windeployqt.exe GecoLight%mydate%\GecoLight.exe


cd ../..
