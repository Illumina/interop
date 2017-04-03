#!/usr/bin/env bash
########################################################################################################################
# Test Build Script for Linux Compilers
#
# This script takes two parameters:
#   1. Build Configuration: Debug or Release
#   2. Path to third party binaries: E.g. GTest, NUnit
#
# Example running script (from the source directory)
#
#   sh tools/build_test.sh Debug /var/external_libs
#
# Note, you must already have CMake and GCC installed and on your path.
#
########################################################################################################################

# Ensure the script stops on first error
set -e


source_dir="${PWD}"
dist_dir="${PWD}/dist"
build_dir="${PWD}/build"
build_type="Debug"
build_param=""

if [ ! -z $1 ] ; then
    build_type=$1
fi
if [ ! -z $2 ] ; then
    pushd $2 > /dev/null
    build_path=`pwd`
    popd > /dev/null
    build_param="-DGTEST_ROOT=$build_path -DGMOCK_ROOT=$build_path -DNUNIT_ROOT=$build_path/NUnit-2.6.4"
fi

build_param="$build_param -DCMAKE_INSTALL_PREFIX=$dist_dir"

if [ -e /opt/rh/devtoolset-2/root/usr/bin/g++ ] ; then
    build_type="Release"
    export CXX=/opt/rh/devtoolset-2/root/usr/bin/g++
    export CC=/opt/rh/devtoolset-2/root/usr/bin/gcc
    echo "Found GCC4.8 dev"
fi

if [ -e $HOME/miniconda2 ]; then
    export PATH=$HOME/miniconda2/bin:$PATH
fi

echo "##teamcity[blockOpened name='Configure $build_type']"
rm -fr $build_dir
rm -fr $dist_dir
mkdir $build_dir
cd $build_dir
echo "cmake $source_dir $build_param  -DCMAKE_BUILD_TYPE=$build_type"
cmake $source_dir $build_param  -DCMAKE_BUILD_TYPE=$build_type
echo "##teamcity[blockClosed name='Configure $build_type']"

echo "##teamcity[blockOpened name='Build $build_type']"
cmake --build . -- -j8
echo "##teamcity[blockClosed name='Build $build_type']"

echo "##teamcity[blockOpened name='Test $build_type']"
cmake --build . --target check -- -j8
echo "##teamcity[blockClosed name='Test $build_type']"

echo "##teamcity[blockOpened name='Install $build_type']"
rm -f CMakeCache.txt
cmake $source_dir -DCMAKE_BUILD_TYPE=Release $build_param
cmake --build . --target install -- -j4
echo "##teamcity[blockClosed name='Install $build_type']"

echo "##teamcity[blockOpened name='Test Python3']"
export PATH=$HOME/miniconda3/bin:$PATH
which python
rm -fr CMakeCache.txt
cmake $source_dir $build_param
cmake --build . --target check_python -- -j 8
echo "##teamcity[blockClosed name='Test Python3']"

cd ..
rm -fr $build_dir
