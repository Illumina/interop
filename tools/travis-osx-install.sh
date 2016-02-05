#!/usr/bin/env bash

sw_vers

brew update
brew list
brew unlink cmake
brew install cmake
brew install swig
brew install doxygen
brew install https://s3.amazonaws.com/travisbuilds.swig.org/mono.rb

# add swig
# add nunit

if [ ! -e $HOME/rapidxml-1.13 ]; then
    wget http://downloads.sourceforge.net/project/rapidxml/rapidxml/rapidxml%201.13/rapidxml-1.13.zip
    unzip rapidxml-1.13.zip
fi

