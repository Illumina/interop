#!/usr/bin/env bash
########################################################################################################################
# This script setups dependencies for the InterOp library using Docker
#
#
#
# Build the Image
#
# $ docker build --rm -t ezralanglois/interop_centos7 -f ./tools/docker/DockerFile_Centos7 tools
# $ docker images
# $ docker tag <image-id> ezralanglois/interop:last_good
# $ docker tag ezralanglois/interop:last_good ezralanglois/interop:latest
# $ docker push ezralanglois/interop:latest
#
# Clean containers and images
#
# $ docker rm $(docker ps -a -f status=exited -q)
# $ docker rmi $(docker images -f dangling=true -q)
#
########################################################################################################################
#set -ex
set -e

CMAKE_URL="http://www.cmake.org/files/v3.8/cmake-3.8.2-Linux-x86_64.tar.gz"
SWIG_URL="http://prdownloads.sourceforge.net/swig/swig-3.0.12.tar.gz"
NUGET_URL="https://dist.nuget.org/win-x86-commandline/latest/nuget.exe"
GOOGLETEST_URL="https://github.com/google/googletest/archive/release-1.8.0.tar.gz"
JUNIT_URL="http://search.maven.org/remotecontent?filepath=junit/junit/4.12/junit-4.12.jar"
NUNIT_URL="https://github.com/nunit/nunitv2/releases/download/2.6.4/NUnit-2.6.4.zip"
JAVA_URL="http://download.oracle.com/otn-pub/java/jdk/8u131-b11/d54c1d3a095b4ff2b6607d096fa80163/jdk-8u131-linux-x64.rpm"
VALGRIND_URL="ftp://sourceware.org/pub/valgrind/valgrind-3.13.0.tar.bz2"
PROG_HOME=/opt
SWIG_HOME=${PROG_HOME}/swig3
JUNIT_HOME=${PROG_HOME}/junit
NUNIT_HOME=${PROG_HOME}/nunit

#yum -y update
yum install -y wget which
yum install -y gcc gcc-c++ libtool git make rpm-build


#yum install -y python-devel
#curl "https://bootstrap.pypa.io/get-pip.py" -o "get-pip.py"
#python get-pip.py
#pip install auditwheel==1.5.0
#pip install numpy

if hash cmake  2> /dev/null; then
    echo "Found CMake"
else
    wget --no-check-certificate --quiet -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C /usr
fi

#if hash mono  2> /dev/null; then
#    echo "Found mono"
#else
#    yum install yum-utils -y
#    rpm --import "http://keyserver.ubuntu.com/pks/lookup?op=get&search=0x3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF"
#    yum-config-manager --add-repo http://download.mono-project.com/repo/centos/
#    yum install mono-complete  -y
#fi

if hash swig  2> /dev/null; then
    echo "Found Swig"
else
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


    ./configure --prefix=/usr --with-python=/opt/python/cp27-cp27m/bin/python --with-python3=/opt/python/cp36-cp36m/bin/python
    make swig -j4 > /dev/null
    make install
    rm -fr ${SWIG_HOME}
    cd -
fi

if [ -e /usr/include/gtest/gtest.h ]; then
    echo "Found GTest and GMock"
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

if hash java  2> /dev/null; then
    echo "Found Java"
else
    wget --quiet --no-cookies --no-check-certificate --header "Cookie: gpw_e24=http%3A%2F%2Fwww.oracle.com%2F; oraclelicense=accept-securebackup-cookie" "${JAVA_URL}" -O ${JAVA_URL##*/}
    rpm -Uvh ${JAVA_URL##*/}
    rm -f  ${JAVA_URL##*/}
fi

#if [ ! -e ${NUNIT_HOME}/NUnit-2.6.4 ]; then
#    mkdir ${NUNIT_HOME}
#    wget --no-check-certificate --quiet ${NUNIT_URL} -O  ${NUNIT_HOME}/${NUNIT_URL##*/}
#    unzip ${NUNIT_HOME}/${NUNIT_URL##*/} -d ${NUNIT_HOME}
#    rm -f ${NUNIT_HOME}/${JUNIT_URL##*/}
#else
#    echo "Found NUnit at ${NUNIT_HOME}/NUnit-2.6.4"
#fi
#NUNIT_HOME=${NUNIT_HOME}/NUnit-2.6.4

if [ ! -e ${JUNIT_HOME}/${JUNIT_URL##*/} ]; then
    mkdir ${JUNIT_HOME}
    wget --no-check-certificate --quiet ${JUNIT_URL} -O  ${JUNIT_HOME}/${JUNIT_URL##*/}
else
    echo "Found JUnit at ${JUNIT_HOME}/${JUNIT_URL##*/}"
fi

mkdir tmp_build
wget --no-check-certificate --quiet -O - ${VALGRIND_URL} | tar --strip-components=1 -xj -C ./tmp_build
cd tmp_build
./configure --prefix=/usr
make -j 4
make install
cd -
rm -fr tmp_build


export JAVA_HOME=/usr/java/jdk1.8.0_131

which gcc
which g++
which swig
which java
which cmake
#which mono


gcc --version
swig -version
java -version
cmake --version
#mono --version

yum clean all

