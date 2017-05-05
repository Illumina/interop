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
MONO_URL="https://download.mono-project.com/sources/mono/mono-4.8.1.0.tar.bz2"
NUGET_URL="https://dist.nuget.org/win-x86-commandline/latest/nuget.exe"
GOOGLETEST_URL="https://github.com/google/googletest/archive/release-1.8.0.tar.gz"
JUNIT_URL="http://search.maven.org/remotecontent?filepath=junit/junit/4.12/junit-4.12.jar"
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

CMAKE_EXTRA_FLAGS="-DDISABLE_PACKAGE_SUBDIR=ON -DPACKAGE_OUTPUT_FILE_PREFIX=${ARTIFACT_PATH} -DENABLE_PORTABLE=ON"

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

export PATH=${CMAKE_HOME}/bin:${SWIG_HOME}/bin:$PATH

if hash mono  2> /dev/null; then
    which mono
    mono --version
else
    # For some reason GLIBC_HAS_CPU_COUNT does not work properly on Centos5 for 4.8.1.0
    #
    # The following function needs to be added to
    # 1. mono_src/mono/profiler/mono-profiler-log.c (4.8.1.0) or mono/profiler/proflog.c (Line: 48)
    # 2. mono/utils/mono-proclib.c (Line: 19)
    # #ifndef CPU_COUNT
    # #define CPU_COUNT(set) _CPU_COUNT((unsigned int *)(set), sizeof(*(set))/sizeof(unsigned int))
    # static int _CPU_COUNT(unsigned int *set, size_t len) {
    #     int cnt;
    #     cnt = 0;
    #     while (len--)
    #         cnt += __builtin_popcount(*set++);
    #     return cnt;
    # }
    #endif
    mkdir /mono_clean
    wget --no-check-certificate --quiet -O - ${MONO_URL} | tar --strip-components=1 -xj -C /mono_clean
    patch -p0 < /mono_patch.txt


    #cd /io/mono_src
    cd /mono_clean
    ./configure --prefix=/usr
    make && make install
    cd -
    rm -fr /mono_clean
    which dmcs
    which mono
    mono --version

    wget --no-check-certificate --quiet ${NUGET_URL} -O /usr/lib/nuget.exe
    echo "mono /usr/lib/nuget.exe $@" > /usr/bin/nuget
    chmod +x /usr/bin/nuget

    nuget help
fi

if ldconfig -p | grep libgmock -q; then
    ldconfig -p | grep libgmock
    ldconfig -p | grep libgtest
else
    mkdir /gtest
    wget --no-check-certificate --quiet -O - ${GOOGLETEST_URL} | tar --strip-components=1 -xz -C /gtest
    mkdir /gtest/build
    cmake -H/gtest -B/gtest/build
    cmake --build /gtest/build -- -j4
    cp -r /gtest/googletest/include/gtest /usr/include/
    cp -r /gtest/googlemock/include/gmock /usr/include/
    cp /gtest/build/googlemock/gtest/lib*.a /usr/lib/
    cp /gtest/build/googlemock/lib*.a /usr/lib/

    rm -fr /gtest
fi

if [ ! -e /usr/lib/junit-4.12.jar ]; then
    wget --no-check-certificate --quiet ${JUNIT_URL} -O /usr/lib/junit-4.12.jar
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
        touch $SOURCE_PATH/src/ext/python/CMakeLists.txt
        cmake $SOURCE_PATH -Bbuild -DPYTHON_EXECUTABLE=${PYTHON_BIN}/python ${CMAKE_EXTRA_FLAGS}
        cmake --build build --target check -- -j4
        cmake --build build --target package_wheel -- -j4
        auditwheel show ${ARTIFACT_PATH}/interop*${PYBUILD}*linux_x86_64.whl
        auditwheel repair ${ARTIFACT_PATH}/interop*${PYBUILD}*linux_x86_64.whl -w ${ARTIFACT_PATH}
        rm -f ${ARTIFACT_PATH}/interop*${PYBUILD}*linux_x86_64.whl
    done
    cmake --build build --target package -- -j4
fi

