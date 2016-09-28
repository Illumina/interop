#!/usr/bin/env bash


version=`git describe --tags --dirty=-src`
cp CMakeLists.txt .CMakeLists.txt.bak
sed "s/set(ARCHIVE_VERSION .*)/set(ARCHIVE_VERSION \"${version}\")/g" .CMakeLists.txt.bak > CMakeLists.txt
rm -f .CMakeLists.txt.bak
git add CMakeLists.txt
git commit --amend -C HEAD --no-verify

