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


source_dir="../"
build_type="Debug"
build_param=""

if [ ! -z $1 ] ; then
    build_type=$1
fi
if [ ! -z $2 ] ; then
    build_param="-DGTEST_ROOT=$2 -DGMOCK_ROOT=$2 -DNUNIT_ROOT=$2/NUnit-2.6.4"
fi

echo "##teamcity[blockOpened name='Configure $build_type']"
mkdir build_${build_type}
cd build_${build_type}
echo "cmake $source_dir -DCMAKE_BUILD_TYPE=$build_type $build_param"
cmake $source_dir -DCMAKE_BUILD_TYPE=$build_type $build_param -DCMAKE_INSTALL_PREFIX=../usr
echo "##teamcity[blockClosed name='Configure $build_type']"

echo "##teamcity[blockOpened name='Build $build_type']"
cmake --build . -- -j8
echo "##teamcity[blockClosed name='Build $build_type']"

echo "##teamcity[blockOpened name='Test $build_type']"
cmake --build . --target check -- -j8
echo "##teamcity[blockClosed name='Test $build_type']"

echo "##teamcity[blockOpened name='Install $build_type']"
cmake --build . --target install -- -j4
echo "##teamcity[blockClosed name='Install $build_type']"

cd ..
