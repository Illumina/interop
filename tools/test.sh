#!/usr/bin/env bash
set -ex

INTEROP_C89=OFF
BUILD_TYPE=Release

if [ ! -z $1 ] ; then
    INTEROP_C89=$1
fi

if [ ! -z $2 ] ; then
    BUILD_TYPE=$2
fi

if [[ "$3" == "travis" ]]; then
    TRAVIS_BEG='travis_fold:start:script.1\\r'
    TRAVIS_END='travis_fold:end:script.1\\r'
else
    TRAVIS_BEG=""
    TRAVIS_END=""
fi

if [ -e build ]; then
    rm -fr build
fi

mkdir build

echo 'Configuring...' && echo -en ${TRAVIS_BEG}
cmake -H. -Bbuild -DNUNIT_ROOT=$HOME/NUnit-2.6.4 -DENABLE_BACKWARDS_COMPATIBILITY=$INTEROP_C89 -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=./usr
echo -en ${TRAVIS_END}

echo 'Building...' && echo -en ${TRAVIS_BEG}
cmake --build build -- -j4
echo -en ${TRAVIS_END}

echo 'Testing...' && echo -en ${TRAVIS_BEG}
cmake --build build --target check -- -j4
echo -en ${TRAVIS_END}

echo 'Installing...' && echo -en ${TRAVIS_BEG}
cmake --build build --target install -- -j4
echo -en ${TRAVIS_END}

echo 'Cleaning...' && echo -en ${TRAVIS_BEG}
cmake --build build --target clean -- -j4
echo -en ${TRAVIS_END}
