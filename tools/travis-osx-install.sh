#!/usr/bin/env bash

sw_vers

brew update > /dev/null
#brew upgrade
brew list
brew unlink cmake
brew install cmake
brew install swig
brew install doxygen
brew install mono

brew list python
find /usr/local/lib -name "lib*python*.*"

