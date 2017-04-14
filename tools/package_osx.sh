#!/usr/bin/env bash
########################################################################################################################
# This script packages the InterOp library for Mac OSX using Miniconda
#
#
########################################################################################################################
set -ex

CONDA2=$HOME/miniconda2
CONDA3=$HOME/miniconda3

if [ ! -z $1 ] ; then
    SOURCE_PATH=$1
fi
if [ ! -z $2 ] ; then
    ARTIFACT_PATH=$2
else
    ARTIFACT_PATH=$SOURCE_PATH/dist
fi

if [[ "$3" == "travis" ]]; then
    TRAVIS_BEG='travis_fold:start:script.1\\r'
    TRAVIS_END='travis_fold:end:script.1\\r'
else
    TRAVIS_BEG=""
    TRAVIS_END=""
    brew update
    brew list
    brew unlink cmake
    brew install cmake
    brew install swig
    brew install doxygen
    brew install mono
fi


echo 'Installing Miniconda-2...' && echo -en ${TRAVIS_BEG}

if [ ! -e $CONDA2 ] ; then
    wget https://repo.continuum.io/miniconda/Miniconda2-latest-MacOSX-x86_64.sh -O miniconda.sh;
    bash miniconda.sh -b -p $CONDA2
    hash -r
    $CONDA2/bin/conda config --set always_yes yes --set changeps1 no
    $CONDA2/bin/conda update -q conda
    $CONDA2/bin/conda info -a
    $CONDA2/bin/conda install -y numpy
fi
echo -en ${TRAVIS_END}


echo 'Installing Miniconda-3...' && echo -en ${TRAVIS_BEG}

if [ ! -e $CONDA3 ] ; then
    wget https://repo.continuum.io/miniconda/Miniconda3-latest-MacOSX-x86_64.sh -O miniconda.sh;
    bash miniconda.sh -b -p $CONDA3
    hash -r
    $CONDA3/bin/conda config --set always_yes yes --set changeps1 no
    $CONDA3/bin/conda update -q conda
    $CONDA3/bin/conda info -a
    $CONDA3/bin/conda install -y numpy
fi
echo -en ${TRAVIS_END}

echo 'Configure with Py27...' && echo -en ${TRAVIS_BEG}
cmake $SOURCE_PATH -Bbuild27 -DPYTHON_EXECUTABLE=$CONDA2/bin/python -DPACKAGE_OUTPUT_FILE_PREFIX=${ARTIFACT_PATH} -DENABLE_PORTABLE=ON -DPACKAGE_SUFFIX=py27
echo -en ${TRAVIS_END}

echo 'Test with Py27...' && echo -en ${TRAVIS_BEG}
cmake --build build27 --target check -- -j4
echo -en ${TRAVIS_END}

echo 'Package with Py27...' && echo -en ${TRAVIS_BEG}
cmake --build build27 --target package -- -j4
echo -en ${TRAVIS_END}

echo 'Package Wheel with Py27...' && echo -en ${TRAVIS_BEG}
cmake --build build27 --target package_wheel -- -j4
echo -en ${TRAVIS_END}

echo 'Configure with Py36...' && echo -en ${TRAVIS_BEG}
cmake $SOURCE_PATH -Bbuild36 -DPYTHON_EXECUTABLE=$CONDA3/bin/python -DPACKAGE_OUTPUT_FILE_PREFIX=${ARTIFACT_PATH} -DENABLE_PORTABLE=ON -DPACKAGE_SUFFIX=py36
echo -en ${TRAVIS_END}

echo 'Test with Py36...' && echo -en ${TRAVIS_BEG}
cmake --build build27 --target check -- -j4
echo -en ${TRAVIS_END}

echo 'Package with Py36...' && echo -en ${TRAVIS_BEG}
cmake --build build36 --target package -- -j4
echo -en ${TRAVIS_END}

echo 'Package Wheel with Py36...' && echo -en ${TRAVIS_BEG}
cmake --build build36 --target package_wheel -- -j4
echo -en ${TRAVIS_END}

