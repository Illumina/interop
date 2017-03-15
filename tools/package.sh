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
root_dir=${PWD}
dist_dir="${PWD}/dist"
build_dir="${PWD}/build"

build_type="Linux"

if [ ! -z $1 ] ; then
    pushd $1 > /dev/null
    build_path=`pwd`
    popd > /dev/null
    build_param="-DGTEST_ROOT=$build_path -DGMOCK_ROOT=$build_path -DNUNIT_ROOT=$build_path/NUnit-2.6.4"
fi

if [ ! -z $2 ] ; then
    build_param="$build_param -DENABLE_BACKWARDS_COMPATIBILITY=$2"
fi

if [ ! -z $3 ] ; then
    build_param="$build_param -DPACKAGE_SUFFIX=$3"
fi

build_param="$build_param -DCMAKE_BUILD_TYPE=Release -DPACKAGE_OUTPUT_FILE_PREFIX=$dist_dir"

if [ -e /opt/rh/devtoolset-2/root/usr/bin/g++ ] ; then
    export CXX=/opt/rh/devtoolset-2/root/usr/bin/g++
    export CC=/opt/rh/devtoolset-2/root/usr/bin/gcc
    echo "Found GCC4.8 dev"
fi

echo "##teamcity[blockOpened name='Configure $build_type']"
rm -fr $dist_dir
rm -fr $build_dir
mkdir $build_dir
cd $build_dir
echo "cmake $source_dir $build_param "
cmake $source_dir $build_param
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

echo "##teamcity[blockOpened name='Python Wheel Creation $build_type']"
cmake --build . --target package_wheel -- -j 8
echo "##teamcity[blockClosed name='Python Wheel Creation $build_type']"

cd $dist_dir
echo "##teamcity[blockOpened name='NuPack $build_type']"
nuget pack ${build_dir}/src/ext/csharp/package.nuspec
echo "##teamcity[blockClosed name='NuPack $build_type']"

cd $root_dir
rm -fr $build_dir
