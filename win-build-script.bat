REM This is a stub


REM DATE=`date +%Y-%m-%d`
REM TODO:
REM QTDIR=~/Qt/5.8/clang_64/
REM echo $QTDIR

mkdir build-win
cd build-win
rd full
mkdir full
cd full
qmake.exe ../../geco.pro -config release
mingw32-make.exe
windeployqt Geco.exe

cd ..
rd light
mkdir light
cd light
qmake.exe ../../geco_light.pro -config release
mingw32-make.exe
macdeployqt.exe GecoLight.exe

REM output:

REM cd ../..
REM cd GecoBuilds
REM mkdir win
REM cd win
REM copy ../../build-osx/full/Geco ./Geco-$DATE.dmg
REM copy ../../build-osx/light/GecoLight.dmg ./GecoLight-$DATE.dmg
