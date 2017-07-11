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
# $ docker run --rm -v `pwd`:/io:Z ezralanglois/interop sh /io/tools/package.sh /io /io/dist travis OFF Release
# $ docker run --rm -v `pwd`:/io:Z ezralanglois/interop sh /io/tools/package.sh /io /io/dist teamcity OFF Release
#
# Debug the Image Interactively
#
# $ docker run --it -v `pwd`:/io ezralanglois/interop sh /io/tools/package_linux.sh /io /io/dist
#
#
########################################################################################################################
set -e
INTEROP_C89=OFF
BUILD_TYPE=Release

if [ ! -z $1 ] ; then
    SOURCE_PATH=$1
fi
BUILD_PATH=build

if [ ! -z $2 ] ; then
    ARTIFACT_PATH=$2
elif [ ! -z $SOURCE_PATH ]; then
    ARTIFACT_PATH=$SOURCE_PATH/dist
fi

if [ ! -z $3 ] ; then
    BUILD_SERVER=$3
    DISABLE_SUBDIR=OFF
    if [[ "$server" == "travis" ]]; then
        DISABLE_SUBDIR=ON
    fi
else
    DISABLE_SUBDIR=OFF
    OFF=
fi

if [ ! -z $4 ] ; then
    INTEROP_C89=$4
fi

if [ ! -z $5 ] ; then
    BUILD_TYPE=$5
fi

if [ ! -z $6 ] ; then
    PYTHON_VERSION=$6
fi

if [ ! -z $7 ] ; then
    BUILD_NUMBER=$7
fi

CMAKE_EXTRA_FLAGS="-DDISABLE_PACKAGE_SUBDIR=${DISABLE_SUBDIR} -DENABLE_PORTABLE=ON -DENABLE_BACKWARDS_COMPATIBILITY=$INTEROP_C89 -DCMAKE_BUILD_TYPE=$BUILD_TYPE"

if [ ! -z $BUILD_NUMBER ] ; then
 CMAKE_EXTRA_FLAGS="-DBUILD_NUMBER=$BUILD_NUMBER  $CMAKE_EXTRA_FLAGS"
fi


if [ ! -z $ARTIFACT_PATH ] ; then
    CMAKE_EXTRA_FLAGS="-DPACKAGE_OUTPUT_FILE_PREFIX=${ARTIFACT_PATH} $CMAKE_EXTRA_FLAGS"
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
    rm -fr $ARTIFACT_PATH
fi
mkdir $ARTIFACT_PATH


# Build Python Wheels for a range of Python Versions
for PYBUILD in `ls -1 /opt/python`; do
    PYTHON_BIN=/opt/python/${PYBUILD}/bin
    if [[ "$PYBUILD" == cp26* ]]; then
        continue
    fi
    if [[ "$PYBUILD" == cp33* ]]; then
        continue
    fi
    touch $SOURCE_PATH/src/ext/python/CMakeLists.txt
    run "Configure ${PYBUILD}" cmake $SOURCE_PATH -B${BUILD_PATH} -DPYTHON_EXECUTABLE=${PYTHON_BIN}/python ${CMAKE_EXTRA_FLAGS} -DSKIP_PACKAGE_ALL_WHEEL=ON

    run "Test ${PYBUILD}" cmake --build $BUILD_PATH --target check -- -j4
    run "Build ${PYBUILD}" cmake --build $BUILD_PATH --target package_wheel -- -j4
    auditwheel show ${ARTIFACT_PATH}/interop*${PYBUILD}*linux_x86_64.whl
    auditwheel repair ${ARTIFACT_PATH}/interop*${PYBUILD}*linux_x86_64.whl -w ${ARTIFACT_PATH}
    rm -f ${ARTIFACT_PATH}/interop*${PYBUILD}*linux_x86_64.whl
done

if [ ! -z $PYTHON_VERSION ] ; then
    if hash pyenv 2> /dev/null; then
        pyenv install $PYTHON_VERSION || true
        pyenv global $PYTHON_VERSION || true
        python --version
        pip install numpy
        pip install wheel
        if [[ "$OSTYPE" == "linux-gnu" ]]; then
            pip install auditwheel==1.5
        elif [[ "$OSTYPE" == "darwin"* ]]; then
            pip install delocate
        fi
    fi
    run "Configure" cmake $SOURCE_PATH -B${BUILD_PATH} ${CMAKE_EXTRA_FLAGS} -DENABLE_PYTHON_DYNAMIC_LOAD=ON -DPYTHON_EXECUTABLE=`which python`
    run "Build" cmake --build $BUILD_PATH -- -j4
    run "Test" cmake --build $BUILD_PATH --target check -- -j4
    run "Build Wheel $PYTHON_VERSION" cmake --build $BUILD_PATH --target package_wheel -- -j4

    if hash delocate-wheel 2> /dev/null; then
        delocate-listdeps ${ARTIFACT_PATH}/*.whl
        delocate-wheel $ARTIFACT_PATH/*.whl
        delocate-addplat --rm-orig -x 10_9 -x 10_10 $ARTIFACT_PATH/*.whl
    elif hash auditwheel 2> /dev/null; then
        auditwheel show ${ARTIFACT_PATH}/*.whl
        auditwheel repair ${ARTIFACT_PATH}/*.whl -w ${ARTIFACT_PATH}
        rm -f ${ARTIFACT_PATH}/interop*linux_x86_64.whl
    fi
fi

if [ ! -e $BUILD_PATH/CMakeCache.txt ] ; then
    run "Configure" cmake $SOURCE_PATH -B${BUILD_PATH} ${CMAKE_EXTRA_FLAGS}
    run "Build" cmake --build $BUILD_PATH -- -j4
    run "Test" cmake --build $BUILD_PATH --target check -- -j4

fi

run "Package" cmake --build $BUILD_PATH --target bundle -- -j4

if hash dotnet 2> /dev/null; then
    run "Configure DotNetCore" cmake $SOURCE_PATH -B${BUILD_PATH} ${CMAKE_EXTRA_FLAGS} -DCSBUILD_TOOL=DotNetCore && cmake --$BUILD_PATH --target nupack -- -j 4 || true
fi

rm -fr $BUILD_PATH

setuser $SOURCE_PATH $ARTIFACT_PATH