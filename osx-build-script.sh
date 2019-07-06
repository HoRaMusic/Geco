#!/bin/sh
DATE=`date +%Y-%m-%d`

# TODO:
QTDIR=~/Qt/5.8/clang_64/
echo $QTDIR
mkdir build-osx
cd build-osx
rm -r full
mkdir full
cd full
$QTDIR/bin/qmake ../../geco.pro -config release
make
$QTDIR/bin/macdeployqt Geco.app -dmg
cd ..
rm -r light
mkdir light
cd light
$QTDIR/bin/qmake ../../geco_light.pro -config release
make
$QTDIR/bin/macdeployqt GecoLight.app -dmg

# output
cd ../..
cd GecoBuilds
mkdir osx
cd osx
cp ../../build-osx/full/Geco.dmg ./Geco-$DATE.dmg
cp ../../build-osx/light/GecoLight.dmg ./GecoLight-$DATE.dmg
