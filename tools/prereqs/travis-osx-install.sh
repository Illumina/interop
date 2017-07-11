#!/usr/bin/env bash

sw_vers

brew update
brew list
brew unlink cmake
brew install cmake
brew install swig
brew install doxygen
brew install mono
brew install gstat
brew unlink pyenv
brew install pyenv

pyenv install 2.7.11
pyenv global 2.7.11

pip install numpy
pip install wheel
pip install delocate
