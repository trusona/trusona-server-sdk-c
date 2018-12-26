#!/usr/bin/env bash

set -e

wget -q https://github.com/libcheck/check/archive/0.12.0.tar.gz -O check-0.12.0.tar.gz
tar xzf check-0.12.0.tar.gz
cd check-0.12.0

autoreconf --install
./configure
make
make check
