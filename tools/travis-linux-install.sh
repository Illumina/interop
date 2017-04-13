#!/bin/bash

lsb_release -a

# http://stackoverflow.com/questions/33196136/travis-ci-update-cmake-using-the-packages-cache
CMAKE_URL="http://www.cmake.org/files/v3.4/cmake-3.4.3-Linux-x86_64.tar.gz"
if [ ! -e ${HOME}/cmake/bin/cmake ]; then
    if [ ! -e ${HOME}/cmake ]; then
        mkdir $HOME/cmake
    fi
    travis_retry wget --quiet -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C $HOME/cmake
    export PATH=${HOME}/cmake/bin:${PATH}
else
    echo "Using cached cmake"
fi


python --version
python -c "import sys; opts={1114111: 'ucs4', 65535: 'ucs2'}; print opts.get(sys.maxunicode)"


if [ ! -e $HOME/NUnit-2.6.4/bin/nunit.framework.dll ]; then
    travis_retry wget https://github.com/nunit/nunitv2/releases/download/2.6.4/NUnit-2.6.4.zip
    unzip NUnit-2.6.4.zip -d $HOME
else
    echo "Using cached NUnit"
fi




