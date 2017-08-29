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
THREAD_COUNT=4

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
if [ -e /opt/python ] ; then
    for PYBUILD in `ls -1 /opt/python`; do
        PYTHON_BIN=/opt/python/${PYBUILD}/bin
        if [[ "$PYBUILD" == cp26* ]]; then
            continue
        fi
        if [[ "$PYBUILD" == cp33* ]]; then
            continue
        fi
        touch $SOURCE_PATH/src/ext/python/CMakeLists.txt
        run "Configure ${PYBUILD}" cmake $SOURCE_PATH -B${BUILD_PATH} -DPYTHON_EXECUTABLE=${PYTHON_BIN}/python ${CMAKE_EXTRA_FLAGS} -DSKIP_PACKAGE_ALL_WHEEL=ON -DPYTHON_WHEEL_PREFIX=${ARTIFACT_PATH}/tmp

        run "Test ${PYBUILD}" cmake --build $BUILD_PATH --target check -- -j${THREAD_COUNT}
        run "Build ${PYBUILD}" cmake --build $BUILD_PATH --target package_wheel -- -j${THREAD_COUNT}
        auditwheel show ${ARTIFACT_PATH}/tmp/interop*${PYBUILD}*linux_x86_64.whl
        auditwheel repair ${ARTIFACT_PATH}/tmp/interop*${PYBUILD}*linux_x86_64.whl -w ${ARTIFACT_PATH}
        rm -fr ${ARTIFACT_PATH}/tmp
    done
fi

if [ ! -z $PYTHON_VERSION ] ; then
    if [ "$PYTHON_VERSION" == "ALL" ] ; then
        python_versions="2.7.11 3.4.4 3.5.1 3.6.0"
    else
        python_versions="$PYTHON_VERSION"
    fi
    for py_ver in $python_versions; do
        echo "Building Python $py_ver"
        if hash pyenv 2> /dev/null; then
            export PATH=$(pyenv root)/shims:${PATH}
            pyenv install $py_ver || true
            pyenv global $py_ver || true
            which python
            python --version
            pip install numpy
            pip install wheel
            if [[ "$OSTYPE" == "linux-gnu" ]]; then
                pip install auditwheel==1.5
            elif [[ "$OSTYPE" == "darwin"* ]]; then
                pip install delocate
            fi
        fi
        run "Configure $py_ver" cmake $SOURCE_PATH -B${BUILD_PATH} ${CMAKE_EXTRA_FLAGS} -DENABLE_PYTHON_DYNAMIC_LOAD=ON -DPYTHON_EXECUTABLE=`which python` -DPYTHON_WHEEL_PREFIX=${ARTIFACT_PATH}/tmp
        run "Build $py_ver" cmake --build $BUILD_PATH -- -j${THREAD_COUNT}
        run "Test $py_ver" cmake --build $BUILD_PATH --target check_python -- -j${THREAD_COUNT}
        run "Build Wheel $py_ver" cmake --build $BUILD_PATH --target package_wheel -- -j${THREAD_COUNT}

        if hash delocate-wheel 2> /dev/null; then
            delocate-listdeps ${ARTIFACT_PATH}/tmp/*.whl
            delocate-wheel $ARTIFACT_PATH/tmp/*.whl
            delocate-addplat -c --rm-orig -x 10_9 -x 10_10 $ARTIFACT_PATH/tmp/*.whl -w $ARTIFACT_PATH
        elif hash auditwheel 2> /dev/null; then
            auditwheel show ${ARTIFACT_PATH}/tmp/*.whl
            auditwheel repair ${ARTIFACT_PATH}/tmp/*.whl -w ${ARTIFACT_PATH}
        fi
        touch $SOURCE_PATH/src/ext/python/CMakeLists.txt
        rm -fr ${ARTIFACT_PATH}/tmp
    done
fi

if [ ! -e $BUILD_PATH/CMakeCache.txt ] ; then
    run "Configure" cmake $SOURCE_PATH -B${BUILD_PATH} ${CMAKE_EXTRA_FLAGS}
    run "Build" cmake --build $BUILD_PATH -- -j${THREAD_COUNT}
    run "Test" cmake --build $BUILD_PATH --target check -- -j${THREAD_COUNT}
fi

run "Package" cmake --build $BUILD_PATH --target bundle -- -j${THREAD_COUNT}

if hash dotnet 2> /dev/null; then
    run "Configure DotNetCore" cmake $SOURCE_PATH -B${BUILD_PATH} ${CMAKE_EXTRA_FLAGS} -DCSBUILD_TOOL=DotNetCore && cmake --build $BUILD_PATH --target nupack -- -j${THREAD_COUNT} || true
fi
rm -fr ${ARTIFACT_PATH}/tmp
echo "List Artifacts:"
ls -l $ARTIFACT_PATH/*
echo "----"


rm -fr $BUILD_PATH

setuser $SOURCE_PATH $ARTIFACT_PATH