#!/usr/bin/env bash

sw_vers

brew update
brew list
brew unlink cmake
brew install cmake
brew install swig
brew install doxygen
brew install mono


wget https://repo.continuum.io/miniconda/Miniconda2-latest-MacOSX-x86_64.sh -O miniconda.sh;
bash miniconda.sh -b -p $HOME/miniconda
export PATH="$HOME/miniconda/bin:$PATH"
hash -r
conda config --set always_yes yes --set changeps1 no
conda update -q conda
conda install numpy
conda info -a

