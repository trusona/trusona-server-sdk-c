#!/usr/bin/env bash

set -e

##
## grab latest Jansson
##
wget -q http://www.digip.org/jansson/releases/jansson-2.10.tar.gz -O jansson-2.10.tar.gz
tar xzf jansson-2.10.tar.gz
cd jansson-2.10
./configure
make
make check
make install
