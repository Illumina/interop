#!/usr/bin/env bash
########################################################################################################################
# This script packages the InterOp library using Docker
#
# Requires two command line arguments:
# 1. source file directory
# 2. artifact directory
#
# Run the Image
#
# $ docker run --rm -v `pwd`:/io ezralangois/interop sh /io/tools/package_linux.sh /io /io/dist
#
# Build the Image
#
# $ docker build --rm -t ezralangois/interop -f ./tools/DockerFile tools
#
#
########################################################################################################################
set -ex

CMAKE_URL="http://www.cmake.org/files/v3.4/cmake-3.4.3-Linux-x86_64.tar.gz"
SWIG_URL="http://prdownloads.sourceforge.net/swig/swig-3.0.12.tar.gz"
PROG_HOME=/opt
CMAKE_HOME=${PROG_HOME}/cmake34
SWIG_HOME=${PROG_HOME}/swig3

if [ ! -z $1 ] ; then
    SOURCE_PATH=$1
fi
if [ ! -z $2 ] ; then
    ARTIFACT_PATH=$2
else
    ARTIFACT_PATH=$SOURCE_PATH/dist
fi

if [ ! -e ${CMAKE_HOME}/bin/cmake ]; then
    if [ ! -e ${CMAKE_HOME} ]; then
        mkdir ${CMAKE_HOME}
    fi
    wget --no-check-certificate --quiet -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C ${CMAKE_HOME}
fi

if [ ! -e ${SWIG_HOME}/bin/swig ]; then
    if [ ! -e ${SWIG_HOME} ]; then
        mkdir ${SWIG_HOME}
    fi
    if [ ! -e ${SWIG_HOME}/src ]; then
        mkdir ${SWIG_HOME}/src
    fi
    wget --no-check-certificate --quiet -O - ${SWIG_URL} | tar --strip-components=1 -xz -C ${SWIG_HOME}/src
    cd ${SWIG_HOME}/src

    # TODO test if on centos or ubuntu
    yum install pcre-devel -y


    ./configure --prefix=${SWIG_HOME} --with-python=/opt/python/cp27-cp27m/bin/python --with-python3=/opt/python/cp36-cp36m/bin/python
    make swig > /dev/null
    make install
    rm -fr ${SWIG_HOME}/src
    cd -
fi


for PYBUILD in `ls -1 /opt/python`; do
    if [[ "$PYBUILD" == cp26* ]]; then
        continue
    fi
    if [[ "$PYBUILD" == cp33* ]]; then
        continue
    fi
    "/opt/python/${PYBUILD}/bin/pip" install numpy
done

export PATH=${CMAKE_HOME}/bin:${SWIG_HOME}/bin:$PATH
which cmake
which gcc
which g++
which swig


cmake --version
gcc --version
swig -version

if [ ! -z $SOURCE_PATH ] ; then
    mkdir build
    for PYBUILD in `ls -1 /opt/python`; do
        PYTHON_BIN=/opt/python/${PYBUILD}/bin
        if [[ "$PYBUILD" == cp26* ]]; then
            continue
        fi
        if [[ "$PYBUILD" == cp33* ]]; then
            continue
        fi
        rm -fr build/src/ext/python
        cmake $SOURCE_PATH -Bbuild -DPYTHON_EXECUTABLE=${PYTHON_BIN}/python -DPACKAGE_OUTPUT_FILE_PREFIX=${ARTIFACT_PATH} -DENABLE_PORTABLE=ON
        cmake --build build --target check -- -j4
        cmake --build build --target package_wheel -- -j4
        auditwheel show ${ARTIFACT_PATH}/interop*${PYBUILD}*linux_x86_64.whl
        auditwheel repair ${ARTIFACT_PATH}/interop*${PYBUILD}*linux_x86_64.whl -w ${ARTIFACT_PATH}
        rm -f ${ARTIFACT_PATH}/interop*${PYBUILD}*linux_x86_64.whl
    done
    cmake --build build --target package -- -j4
fi

