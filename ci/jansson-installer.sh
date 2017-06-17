#!/bin/sh

##
## grab latest Jansson
##
wget http://www.digip.org/jansson/releases/jansson-2.10.tar.gz
tar xzf jansson-2.10.tar.gz
cd jansson-2.10 && ./configure && make && make check && make install
