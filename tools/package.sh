#!/usr/bin/env bash
########################################################################################################################
# Package InterOp for Deployment
#
# This script takes three parameters:
#   1. Path to third party binaries: E.g. GTest, NUnit
#   2. Package suffix
#   3. Enable C++98
#
# Example running script (from the source directory)
#
#   sh tools/build_test.sh /var/external_libs OFF centos7
#
# Note, you must already have CMake, GCC and nuget installed and on your path.
#
########################################################################################################################

# Ensure the script stops on first error
set -e


source_dir="../"
build_param=""

build_type="Linux"

if [ ! -z $1 ] ; then
    build_param="-DJUNIT_ROOT=$1 -DGTEST_ROOT=$1 -DGMOCK_ROOT=$1 -DNUNIT_ROOT=$1/NUnit-2.6.4"
fi

if [ ! -z $2 ] ; then
    build_param="$build_param -DENABLE_BACKWARDS_COMPATIBILITY=$2"
fi

if [ ! -z $3 ] ; then
    build_param="$build_param -DPACKAGE_SUFFIX=$3"
fi

if [ -e /opt/rh/devtoolset-2/root/usr/bin/g++ ] ; then
    export CXX=/opt/rh/devtoolset-2/root/usr/bin/g++
    export CC=/opt/rh/devtoolset-2/root/usr/bin/gcc
fi

echo "##teamcity[blockOpened name='Configure $build_type']"
mkdir build_${build_type}
cd build_${build_type}
echo "cmake $source_dir -DCMAKE_BUILD_TYPE=Release $build_param"
cmake $source_dir -DCMAKE_BUILD_TYPE=Release $build_param
echo "##teamcity[blockClosed name='Configure $build_type']"

echo "##teamcity[blockOpened name='Test $build_type']"
cmake --build . --target check -- -j 8
echo "##teamcity[blockClosed name='Test $build_type']"

echo "##teamcity[blockOpened name='Build $build_type']"
cmake --build . -- -j 8
echo "##teamcity[blockClosed name='Build $build_type']"

echo "##teamcity[blockOpened name='Package $build_type']"
cmake --build . --target package -- -j 8
echo "##teamcity[blockClosed name='Package $build_type']"

echo "##teamcity[blockOpened name='NuSpec Creation $build_type']"
cmake --build . --target nuspec -- -j 8
echo "##teamcity[blockClosed name='NuSpec Creation $build_type']"

echo "##teamcity[blockOpened name='NuPack $build_type']"
nuget pack src/ext/csharp/package.nuspec
echo "##teamcity[blockClosed name='NuPack $build_type']"

cd ..
