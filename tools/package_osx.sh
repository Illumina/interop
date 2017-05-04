#!/usr/bin/env bash
########################################################################################################################
# This script packages the InterOp library for Mac OSX using CPython
#
#
########################################################################################################################
set -ex
GET_PIP_URL=https://bootstrap.pypa.io/get-pip.py

SOURCE_PATH=$1
PYTHON_VER=$2
PYTHON_VER_SUFFIX=`echo $PYTHON_VER | awk -F "." '{printf "%d%d%d", $1, $2, $3}'`

if [ ! -z $3 ] ; then
    ARTIFACT_PATH=$2
else
    ARTIFACT_PATH=$SOURCE_PATH/dist
fi

if [[ "$4" == "travis" ]]; then
    TRAVIS_BEG='travis_fold:start:script.1\\r'
    TRAVIS_END='travis_fold:end:script.1\\r'
    CMAKE_EXTRA_FLAGS="-DDISABLE_PACKAGE_SUBDIR=ON"
else
    # Already installed by a previous travis step
    TRAVIS_BEG=""
    TRAVIS_END=""
    CMAKE_EXTRA_FLAGS=""
    sh `dirname $0`/travis-osx-install.sh
fi

echo "Install Python ${PYTHON_VER}..." && echo -en "${TRAVIS_BEG}"
brew install pyenv && true
pyenv install ${PYTHON_VER} && true
pyenv local ${PYTHON_VER}
which python
echo -en ${TRAVIS_END}
python --version


echo "Install Deps with Python ${PYTHON_VER}..." && echo -en "${TRAVIS_BEG}"
pip install numpy
pip install wheel
pip install delocate
echo -en ${TRAVIS_END}



########################################################################################################################
echo "Configure with Python ${PYTHON_VER}..." && echo -en "${TRAVIS_BEG}"
cmake $SOURCE_PATH -Bbuild_${PYTHON_VER_SUFFIX} -DENABLE_PYTHON_DYNAMIC_LOAD=ON -DPYTHON_EXECUTABLE=`which python` -DPACKAGE_OUTPUT_FILE_PREFIX=${ARTIFACT_PATH} -DENABLE_PORTABLE=ON -DPACKAGE_SUFFIX=py${PYTHON_VER_SUFFIX} ${CMAKE_EXTRA_FLAGS}
echo -en "${TRAVIS_END}"

echo "Test with Python ${PYTHON_VER}..." && echo -en "${TRAVIS_BEG}"
cmake --build build_${PYTHON_VER_SUFFIX} --target check -- -j4 VERBOSE=1
echo -en "${TRAVIS_END}"

echo "Package with Python ${PYTHON_VER}..." && echo -en "${TRAVIS_BEG}"
cmake --build build_${PYTHON_VER_SUFFIX} --target package -- -j4
echo -en "${TRAVIS_END}"

echo "Package Wheel with Python ${PYTHON_VER}..." && echo -en "${TRAVIS_BEG}"
cmake --build build_${PYTHON_VER_SUFFIX} --target package_wheel -- -j4
delocate-listdeps ${ARTIFACT_PATH}/*.whl
delocate-wheel $ARTIFACT_PATH/*.whl
delocate-addplat --rm-orig -x 10_9 -x 10_10 $ARTIFACT_PATH/*.whl
ls $ARTIFACT_PATH/*.whl
echo -en "${TRAVIS_END}"
