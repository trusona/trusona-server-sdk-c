#!/usr/bin/env bash

set -e

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

rm -rf check-*
