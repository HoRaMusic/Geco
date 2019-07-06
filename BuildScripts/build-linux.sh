#!/bin/sh
DATE=`date +%Y-%m-%d`

# TODO:
QTDIR=~/Qt/5.8/gcc_64/
echo $QTDIR
rm -r build-linux
mkdir build-linux
cd build-linux
$QTDIR/bin/qmake ../geco.pro -config release
make
$QTDIR/bin/linuxdeployqt Geco
cd ..

# output
cd GecoBuilds
mkdir linux
cd linux
cp ../../build-osx/Geco ./Geco-$DATE
