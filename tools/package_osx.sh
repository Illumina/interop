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
PYTHON_DIR=$HOME/python${PYTHON_VER}
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

if [ ! -e $PYTHON_DIR ] ; then
    export CFLAGS="-I/usr/local/opt/openssl/include -I/usr/local/include"
    export LDFLAGS="-L/usr/local/opt/openssl/lib -L/usr/local/lib"
    #brew install openssl xz --force
    brew list openssl
    brew install xz berkeley-db readline readline sqlite
    ls /usr/local/opt

    echo "Download with Python ${PYTHON_VER}..." && echo -en "${TRAVIS_BEG}"
    mkdir $PYTHON_DIR
    mkdir $PYTHON_DIR/src
    curl -sLO $GET_PIP_URL
    wget --quiet -O - https://www.python.org/ftp/python/${PYTHON_VER}/Python-${PYTHON_VER}.tgz | tar --strip-components=1 -xz -C $PYTHON_DIR/src
    pushd $PYTHON_DIR/src
    echo -en "${TRAVIS_END}"

    echo "Configure with Python ${PYTHON_VER}..." && echo -en "${TRAVIS_BEG}"
    CFLAGS="-Wformat" ./configure --prefix=${PYTHON_DIR} --disable-shared
    echo -en "${TRAVIS_END}"
    echo "Build with Python ${PYTHON_VER}..." && echo -en "${TRAVIS_BEG}"
    make -j4
    echo -en "${TRAVIS_END}"

    echo "Install with Python ${PYTHON_VER}..." && echo -en "${TRAVIS_BEG}"
    make install
    popd
    if [ ! -e ${PYTHON_DIR}/bin/python ]; then
        ln -s ${PYTHON_DIR}/bin/python3 ${PYTHON_DIR}/bin/python
    fi
    rm -fr $PYTHON_DIR/src
    $PYTHON_DIR/bin/python get-pip.py
    if [ -e ${PYTHON_DIR}/bin/pip3 ] && [ ! -e ${PYTHON_DIR}/bin/pip ]; then
        ln -s pip3 ${PYTHON_DIR}/bin/pip
    fi
    echo -en "${TRAVIS_END}"

    echo "Install Deps with Python ${PYTHON_VER}..." && echo -en "${TRAVIS_BEG}"
    $PYTHON_DIR/bin/pip install numpy
    $PYTHON_DIR/bin/pip install wheel
    $PYTHON_DIR/bin/pip install delocate
    rm -fr get-pip.py
    rm -f ${PYTHON_DIR}/lib/libpython*.a
    echo -en ${TRAVIS_END}
fi


########################################################################################################################
echo "Configure with Python ${PYTHON_VER}..." && echo -en "${TRAVIS_BEG}"
cmake $SOURCE_PATH -Bbuild_${PYTHON_VER_SUFFIX} -DENABLE_PYTHON_DYNAMIC_LOAD=ON -DPYTHON_EXECUTABLE=$PYTHON_DIR/bin/python -DPACKAGE_OUTPUT_FILE_PREFIX=${ARTIFACT_PATH} -DENABLE_PORTABLE=ON -DPACKAGE_SUFFIX=py${PYTHON_VER_SUFFIX} ${CMAKE_EXTRA_FLAGS}
echo -en "${TRAVIS_END}"

echo "Test with Python ${PYTHON_VER}..." && echo -en "${TRAVIS_BEG}"
cmake --build build_${PYTHON_VER_SUFFIX} --target check -- -j4 VERBOSE=1
echo -en "${TRAVIS_END}"

echo "Package with Python ${PYTHON_VER}..." && echo -en "${TRAVIS_BEG}"
cmake --build build_${PYTHON_VER_SUFFIX} --target package -- -j4
echo -en "${TRAVIS_END}"

echo "Package Wheel with Python ${PYTHON_VER}..." && echo -en "${TRAVIS_BEG}"
cmake --build build_${PYTHON_VER_SUFFIX} --target package_wheel -- -j4
$PYTHON_DIR/bin/delocate-listdeps ${ARTIFACT_PATH}/*.whl
$PYTHON_DIR/bin/delocate-wheel $ARTIFACT_PATH/*.whl
$PYTHON_DIR/bin/delocate-addplat --rm-orig -x 10_9 -x 10_10 $ARTIFACT_PATH/*.whl
ls $ARTIFACT_PATH/*.whl
echo -en "${TRAVIS_END}"
