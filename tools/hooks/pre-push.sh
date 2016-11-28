#!/usr/bin/env bash


version=`git describe --tags --dirty=-src | cut - -f1`

grep -q $version CMakeLists.txt
ret=$?
if [ "$?" -eq "1" ] ; then
    echo "Incorrect archive version, update to $version"
    exit 1
fi

