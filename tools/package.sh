#!/usr/bin/env bash
########################################################################################################################
# This script packages the InterOp library using Docker
#
# Requires two command line arguments:
# 1. source file directory
# 2. artifact directory
#
# Pull the Image
#
# $ docker pull ezralanglois/interop
#
# Run the Image
#
# $ docker run --rm -w /tmp --user `id -u`:`id -g` -v `pwd`:/src:ro -v `pwd`/dist:/dist:rw ezralanglois/interop sh /src/tools/package.sh /src /dist travis OFF Release
# $ docker run --rm -w /tmp --user `id -u`:`id -g` -v `pwd`:/src:ro -v `pwd`/dist:/dist:rw ezralanglois/interop sh /src/tools/package.sh /src /dist teamcity OFF Release
#
# Debug the Image Interactively
#
# $ docker run --rm -i -t -v `pwd`:/io ezralanglois/interop sh /io/tools/package_linux.sh /io /io/dist
#
#
########################################################################################################################
set -e
INTEROP_C89=OFF
BUILD_TYPE=Release

# When inside docker and not using root, but using the root home. So, change it to working directory
# Without this, nuget fails when trying to read Nuget.config
whoami 1>/dev/null 2>&1 || export HOME=$PWD

# Get value from environment for low memory vms
if [ -z $THREAD_COUNT ] ; then
    THREAD_COUNT=4
fi

if [ ! -z $1 ] ; then
    SOURCE_PATH=$1
fi
BUILD_PATH=build

if [ ! -z $2 ] ; then
    ARTIFACT_PATH=$2
elif [ ! -z $SOURCE_PATH ]; then
    ARTIFACT_PATH=$SOURCE_PATH/dist
fi

# ARTIFACT_PATH="$(echo $(cd $(dirname "$ARTIFACT_PATH") && pwd -P)/$(basename "$ARTIFACT_PATH"))"

if hash greadlink  2> /dev/null; then
    readlink="greadlink"
else
    readlink="readlink"
fi
ARTIFACT_PATH=`$readlink -f $ARTIFACT_PATH`

if [ ! -z $3 ] ; then
    BUILD_SERVER="$3"
    DISABLE_SUBDIR=OFF
    if [[ "$BUILD_SERVER" == "travis" ]]; then
        DISABLE_SUBDIR=ON
    fi
else
    DISABLE_SUBDIR=OFF
    OFF=
fi

if [ ! -z $4 ] ; then
    INTEROP_C89="$4"
fi

if [ ! -z $5 ] ; then
    BUILD_TYPE="$5"
fi

if [ ! -z $6 ] ; then
    PYTHON_VERSION="$6"
fi

if [ ! -z $7 ] ; then
    BUILD_NUMBER="$7"
fi

if [ ! -z "$8" ] ; then
    MORE_FLAGS="$8"
fi

echo "-------------------------------"
echo "package.sh Configuration"
echo "Source path: ${SOURCE_PATH}"
echo "Artifact path: ${ARTIFACT_PATH}"
echo "Build server: ${BUILD_SERVER}"
echo "C89 Support: ${INTEROP_C89}"
echo "Build Type: ${BUILD_TYPE}"
echo "Python Version: ${PYTHON_VERSION}"
echo "Build Number: ${ARTFACT_BUILD_NUMBER}"
echo "Additional Flags: ${MORE_FLAGS}"
echo "-------------------------------"

CMAKE_EXTRA_FLAGS="-DDISABLE_PACKAGE_SUBDIR=${DISABLE_SUBDIR} -DENABLE_PORTABLE=ON -DENABLE_BACKWARDS_COMPATIBILITY=$INTEROP_C89 -DCMAKE_BUILD_TYPE=$BUILD_TYPE $MORE_FLAGS"


if [ "$PYTHON_VERSION" == "None" ] ; then
    PYTHON_VERSION=
fi

if [ "$PYTHON_VERSION" == "Disable" ] ; then
    CMAKE_EXTRA_FLAGS="-DENABLE_SWIG=OFF $CMAKE_EXTRA_FLAGS"
fi

if [ ! -z $BUILD_NUMBER ] ; then
  CMAKE_EXTRA_FLAGS="-DBUILD_NUMBER=$BUILD_NUMBER  $CMAKE_EXTRA_FLAGS"
fi


if [ ! -z $ARTIFACT_PATH ] ; then
  CMAKE_EXTRA_FLAGS="-DPACKAGE_OUTPUT_FILE_PREFIX=${ARTIFACT_PATH} -DDOCS_OUTPUT_PATH=${ARTIFACT_PATH} $CMAKE_EXTRA_FLAGS"
fi

if hash cmake  2> /dev/null; then
    echo "Found CMake"
else
    CMAKE_URL="http://www.cmake.org/files/v3.8/cmake-3.8.2-Linux-x86_64.tar.gz"
    mkdir /tmp/cmake
    wget --no-check-certificate --quiet -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C /tmp/cmake
    export PATH=$PATH:/tmp/cmake/bin
fi

# Utility macros
source `dirname $0`/prereqs/utility.sh

if [ -z $SOURCE_PATH ] ; then
    exit 0
fi

if [ -e $BUILD_PATH ] ; then
    rm -fr $BUILD_PATH
fi
mkdir $BUILD_PATH

if [ -e $ARTIFACT_PATH ] ; then
    rm -fr $ARTIFACT_PATH/*
else
    mkdir $ARTIFACT_PATH
fi

# Build Python Wheels for a range of Python Versions
if [ -z $PYTHON_VERSION ] && [  -e /opt/python ] ; then
    echo "ManyLinux with: "
    for PYBUILD in `ls -1 /opt/python`; do
      echo "Python ${PYBUILD}"
    done
    for PYBUILD in `ls -1 /opt/python`; do
        PYTHON_BIN=/opt/python/${PYBUILD}/bin
        if [[ "$PYBUILD" == cp26* ]]; then
            continue
        fi
        if [[ "$PYBUILD" == cp33* ]]; then
            continue
        fi
        rm -fr ${BUILD_PATH}/src/ext/python/*
        run "Configure ${PYBUILD}" cmake $SOURCE_PATH -B${BUILD_PATH} -DPYTHON_EXECUTABLE=${PYTHON_BIN}/python ${CMAKE_EXTRA_FLAGS} -DSKIP_PACKAGE_ALL_WHEEL=ON -DPYTHON_WHEEL_PREFIX=${ARTIFACT_PATH}/tmp

        run "Test ${PYBUILD}" cmake --build $BUILD_PATH --target check -- -j${THREAD_COUNT}
        run "Build ${PYBUILD}" cmake --build $BUILD_PATH --target package_wheel -- -j${THREAD_COUNT}
        auditwheel show ${ARTIFACT_PATH}/tmp/interop*${PYBUILD}*linux_x86_64.whl
        auditwheel repair ${ARTIFACT_PATH}/tmp/interop*${PYBUILD}*linux_x86_64.whl -w ${ARTIFACT_PATH}
        rm -fr ${ARTIFACT_PATH}/tmp
    done
fi

if [ "$PYTHON_VERSION" != "" ] && [ "$PYTHON_VERSION" != "Disable" ] ; then
    if [ "$PYTHON_VERSION" == "ALL" ] ; then
        # python_versions="2.7.17 3.5.9 3.6.10 3.7.7 3.8.2"
        python_versions="2.7.17 3.5.9"
        pyenv install --list | grep " 2.7"
        pyenv install --list | grep " 3\.[45678]"
    else
        python_versions="$PYTHON_VERSION"
    fi
    if hash brew 2> /dev/null; then
      CFLAGS="-I$(brew --prefix openssl)/include -I$(xcrun --show-sdk-path)/usr/include"
      LDFLAGS="-L$(brew --prefix openssl)/lib"
    fi
    for py_ver in $python_versions; do
        echo "Building Python $py_ver - $CFLAGS"
        if hash pyenv 2> /dev/null; then
            export PATH=$(pyenv root)/shims:${PATH}
            if [[ "$OSTYPE" == "linux-gnu" ]]; then
                if hash patchelf 2> /dev/null; then
                    pyenv install 3.5 -s  || true
                    pyenv global 3.5 || true
                    pip3 install auditwheel==1.5
                fi
            fi
            pyenv install $py_ver -s
            pyenv global $py_ver
            if [[ "$OSTYPE" == "darwin"* ]]; then
                pip install setuptools==43.0.0
                pip install delocate
                # pip install delocate=0.7.3
            fi
            which python
            which pip
            python --version
            pip install numpy
            pip install wheel
            echo "Check setuptools"
            python -c "import setuptools"
            echo "Check numpy"
            python -c "import numpy"
            echo "Check wheel"
            python -c "import wheel"
            # pip install wheel=0.30.0
        fi
        run "Configure $py_ver" cmake $SOURCE_PATH -B${BUILD_PATH} ${CMAKE_EXTRA_FLAGS} -DENABLE_CSHARP=OFF -DENABLE_PYTHON_DYNAMIC_LOAD=ON -DPYTHON_EXECUTABLE=`which python` -DSKIP_PACKAGE_ALL_WHEEL=ON -DPYTHON_WHEEL_PREFIX=${ARTIFACT_PATH}/tmp
        run "Build $py_ver" cmake --build $BUILD_PATH -- -j${THREAD_COUNT}
        run "Test $py_ver" cmake --build $BUILD_PATH --target check_python -- -j${THREAD_COUNT}
        run "Build Wheel $py_ver" cmake --build $BUILD_PATH --target package_wheel -- -j${THREAD_COUNT}

        if hash delocate-wheel 2> /dev/null; then
            ls ${ARTIFACT_PATH}/tmp/*.whl
            delocate-listdeps ${ARTIFACT_PATH}/tmp/*.whl
            delocate-wheel ${ARTIFACT_PATH}/tmp/*.whl
            delocate-addplat -c --rm-orig -x 10_9 -x 10_10 $ARTIFACT_PATH/tmp/*.whl -w $ARTIFACT_PATH
        elif hash auditwheel 2> /dev/null; then
            auditwheel --version
            auditwheel show ${ARTIFACT_PATH}/tmp/*.whl
            auditwheel repair ${ARTIFACT_PATH}/tmp/*.whl -w ${ARTIFACT_PATH}
        fi
        rm -fr ${BUILD_PATH}/src/ext/python/*
        rm -fr ${ARTIFACT_PATH}/tmp
    done
fi

if [ ! -e $BUILD_PATH/CMakeCache.txt ] ; then
    run "Configure" cmake $SOURCE_PATH -B${BUILD_PATH} ${CMAKE_EXTRA_FLAGS}
    run "Build" cmake --build $BUILD_PATH -- -j${THREAD_COUNT}
    run "Test" cmake --build $BUILD_PATH --target check -- -j${THREAD_COUNT}
fi

run "Package" cmake --build $BUILD_PATH --target bundle


if [ "$PYTHON_VERSION" != "Disable" ] ; then
  # Workaround for OSX
  export PATH=/usr/local/share/dotnet:${PATH}
  if hash dotnet 2> /dev/null; then
      run "Configure DotNetStandard" cmake $SOURCE_PATH -B${BUILD_PATH} ${CMAKE_EXTRA_FLAGS} -DCSBUILD_TOOL=DotNetStandard -DENABLE_PYTHON=OFF
      run "Test DotNetStandard" cmake --build $BUILD_PATH --target check -- -j${THREAD_COUNT}
      run "Package DotNetStandard" cmake --build $BUILD_PATH --target nupack -- -j${THREAD_COUNT}
  fi
fi
rm -fr ${ARTIFACT_PATH}/tmp
echo "List Artifacts:"
ls -l $ARTIFACT_PATH/*
echo "----"


rm -fr $BUILD_PATH

setuser $SOURCE_PATH $ARTIFACT_PATH

