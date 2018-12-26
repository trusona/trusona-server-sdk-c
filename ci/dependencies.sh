#!/usr/bin/env bash

set -e

##
## grab latest Jansson - https://github.com/akheron/jansson
##
wget -q https://github.com/akheron/jansson/archive/v2.12.tar.gz -O jansson-2.12.tar.gz
tar xzf jansson-2.12.tar.gz
cd jansson-2.12
autoreconf --install
./configure
make
make check
make install
cd ..

##
## Grab and install check - https://github.com/libcheck/check
##
wget -q https://github.com/libcheck/check/archive/0.12.0.tar.gz -O check-0.12.0.tar.gz
tar xzf check-0.12.0.tar.gz
cd check-0.12.0
autoreconf --install
./configure
make
make install

cd ..
