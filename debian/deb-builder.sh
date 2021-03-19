#!/usr/bin/env bash

set -e

version=$1
version=${version:=0.0.1-unspecified}

if ! [[ $version =~ ^[0-9]+\.[0-9]+\.[0-9]+(-)?[a-z]*$ ]]; then
  echo "Error: expecting tag version in semver format e.g. 1.2.3 or 4.5.6-beta"
  exit -1
fi

rm -f *.deb

checkinstall -y \
  --requires=libssl-dev,libpam0g-dev,uuid-dev,libjansson-dev,libcurl4-openssl-dev,libjansson4,libuuid1,uuid-runtime,libssl1.1 \
  --maintainer="engineering@trusona.com" \
  --pkgversion=$version \
  --pkglicense=MIT \
  --reset-uids=yes \
  --autodoinst=no \
  --pkgrelease=1 \
  --delspec=yes \
  --type=debian \
  --stripso=yes \
  --deldoc=yes \
  --install=no \
  --backup=no \
  --strip=yes \
  --addso=yes \
  --nodoc
