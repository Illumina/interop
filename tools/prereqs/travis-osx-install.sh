#!/usr/bin/env bash

sw_vers

brew update
brew list
which cmake || brew unlink cmake
brew install cmake
brew link cmake
brew install zlib
brew install swig
brew install doxygen
brew install mono
brew install nuget
brew install coreutils || brew install gstat
which pyenv || brew unlink pyenv
brew install pyenv
brew link pyenv

pyenv install 2.7.11
pyenv global 2.7.11
export PATH=$(pyenv root)/shims:${PATH}

pip install numpy
pip install wheel
pip install delocate
