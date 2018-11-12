#!/usr/bin/env sh

BASEDIR=$(dirname "$0")

cd $BASEDIR/..
mkdir -p Build
cd Build

cmake ..
cmake --build . --target install
