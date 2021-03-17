#!/usr/bin/env bash

version=$1
semver='^[0-9]+\.[0-9]+\.[0-9]+(-)?(\w)*$'

if [[ $version =~ $semver ]]; then
	:
else
	echo "Error: expecting tag version in semver format e.g. 1.2.3 or 4.5.6-beta"
	exit -1
fi

checkinstall -y \
	--install=no \
	--maintainer="engineering@trusona.com" \
	--pkgversion=1.0 \
	--pkgrelease=1 \
	--pkglicense=MIT \
	--requires=libssl-dev,uuid-dev,libjansson-dev,libcurl4-openssl-dev,libjansson4,libuuid1,uuid-runtime,openssl \
	--nodoc \
	--delspec=no \
	--type=debian \
	--backup=no \
	--autodoinst=no \
	--strip=yes \
	--stripso=yes \
	--addso=yes \
	--reset-uids=yes \
	--deldoc=yes
