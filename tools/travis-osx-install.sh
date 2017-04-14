#!/usr/bin/env bash

sw_vers

PYTHON_VERSION_NUM=$1
if [ -z "$PYTHON_VERSION_NUM" ] ; then
    PYTHON_VERSION_NUM="2.7"
fi


brew update
brew list
brew unlink cmake
brew install cmake
brew install swig
brew install doxygen
brew install mono

CONDA2=$HOME/miniconda2
CONDA3=$HOME/miniconda3

if [ ! -e $CONDA2 ] ; then
    wget https://repo.continuum.io/miniconda/Miniconda2-latest-MacOSX-x86_64.sh -O miniconda.sh;
    bash miniconda.sh -b -p $CONDA2
    hash -r
    $CONDA2/bin/conda config --set always_yes yes --set changeps1 no
    $CONDA2/bin/conda update -q conda
    $CONDA2/bin/conda info -a
    $CONDA2/bin/conda install -y numpy
    export PATH="$CONDA2/bin:$PATH"
fi

if [ ! -e $CONDA3 ] ; then
    wget https://repo.continuum.io/miniconda/Miniconda3-latest-MacOSX-x86_64.sh -O miniconda.sh;
    bash miniconda.sh -b -p $CONDA3
    hash -r
    $CONDA3/bin/conda config --set always_yes yes --set changeps1 no
    $CONDA3/bin/conda update -q conda
    $CONDA3/bin/conda info -a
    $CONDA3/bin/conda install -y numpy
fi
