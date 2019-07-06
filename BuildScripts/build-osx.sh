#!/bin/sh
DATE=`date +%Y-%m-%d`

# TODO:
QTDIR=~/Qt/5.8/clang_64/
echo $QTDIR
rm -r build-osx
mkdir build-osx
cd build-osx
$QTDIR/bin/qmake ../geco.pro -config release
make
$QTDIR/bin/macdeployqt Geco.app -dmg

# output
cd ..
cd GecoBuilds
mkdir osx
cd osx
cp ../../build-osx/Geco.dmg ./Geco-$DATE.dmg
