#!/usr/bin/env bash
########################################################################################################################
# Test Build Script for Coverity
#
# This script takes three parameters:
#   1. Build Configuration: Debug or Release
#   2. Path to third party binaries: E.g. GTest, NUnit
#   3. Location of Coverity tools
#
# Example running script (from the source directory)
#
#   sh tools/build_test.sh Debug /var/external_libs /coverity/bin
#
# Note, you must already have CMake and GCC installed and on your path. And Coverity properly configured for C++.
#
########################################################################################################################

# Ensure the script stops on first error
set -e


source_dir="${PWD}"
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
if [ ! -z $3 ] ; then
    cov_path=$3
fi

echo "##teamcity[blockOpened name='Configure $build_type']"
rm -fr $build_dir
mkdir $build_dir
cd $build_dir
echo "cmake $source_dir -DCMAKE_BUILD_TYPE=$build_type $build_param"
cmake $source_dir -DCMAKE_BUILD_TYPE=$build_type $build_param
echo "##teamcity[blockClosed name='Configure $build_type']"

echo "##teamcity[blockOpened name='Coverity Build $build_type']"
$cov_path/cov-build --dir cov make
echo "##teamcity[blockClosed name='Coverity Build $build_type']"

echo "##teamcity[blockOpened name='Coverity Analyze $build_type']"
$cov_path/cov-analyze --dir cov --strip-path $(dirname `pwd`) --security --concurrency --enable-constraint-fpp --enable-fnptr --enable-virtual
echo "##teamcity[blockClosed name='Coverity Analyze $build_type']"

echo "##teamcity[blockOpened name='Coverity Upload $build_type']"
$cov_path/cov-commit-defects --dir cov --host ussd-prd-cove01 --stream InterOp_Master --auth-key-file $HOME/key
echo "##teamcity[blockClosed name='Coverity Upload $build_type']"

cd ..
rm -fr $build_dir

