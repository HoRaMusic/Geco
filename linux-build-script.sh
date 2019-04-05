#!/bin/sh
DATE=`date +%Y-%m-%d`

# TODO:
QTDIR=~/Qt/5.8/gcc_64/
echo $QTDIR
mkdir build-linux
cd build-linux
rm -r full
mkdir full
cd full
$QTDIR/bin/qmake ../../geco.pro -config release
make
$QTDIR/bin/linuxdeployqt Geco
cd ..
rm -r light
mkdir light
cd light
$QTDIR/bin/qmake ../../geco_light.pro -config release
make
$QTDIR/bin/linuxdeployqt GecoLight

# output
cd ../..
cd GecoBuilds
mkdir linux
cd linux
cp ../../build-osx/full/Geco ./Geco-$DATE
cp ../../build-osx/light/GecoLight ./GecoLight-$DATA
