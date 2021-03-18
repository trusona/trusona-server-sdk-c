#!/usr/bin/env bash

set -e

##
## grab latest Jansson - https://github.com/akheron/jansson
##
version=2.13.1
wget -q https://github.com/akheron/jansson/archive/v$version.tar.gz -O jansson-$version.tar.gz
tar xzf jansson-$version.tar.gz
cd jansson-$version
autoreconf --install
./configure
make
make check
make install
cd ..

##
## Grab and install check - https://github.com/libcheck/check
##
version=0.15.2
wget -q https://github.com/libcheck/check/archive/$version.tar.gz -O check-$version.tar.gz
tar xzf check-$version.tar.gz
cd check-$version
autoreconf --install
./configure
make
make install

cd ..

rm -rf check-* jansson-*
