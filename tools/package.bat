echo off
setlocal enabledelayedexpansion
rem --------------------------------------------------------------------------------------------------------------------
rem Package InterOp for Windows Systems
rem
rem The script takes two arguments:
rem     1. Path to third party binaries: E.g. GTest, NUnit
rem
rem Example Running script (from source directory)
rem     .\tools\build_test.bat c:\external
rem
rem Note, you must already have CMake, MinGW and Visual Studio installed and on your path.
rem
rem --------------------------------------------------------------------------------------------------------------------

rem --------------------------------------------------------------------------------------------------------------------
rem MinGW Build Test Script
rem --------------------------------------------------------------------------------------------------------------------

set SOURCE_DIR=..\
set BUILD_PARAM=
set BUILD_TYPE=Release
set BUILD_PATH=%1%
if NOT "%1" == "" (
set BUILD_PARAM=-DGTEST_ROOT=%BUILD_PATH% -DJUNIT_ROOT=%BUILD_PATH% -DGMOCK_ROOT=%BUILD_PATH% -DNUNIT_ROOT=%BUILD_PATH%/NUnit-2.6.4
)



echo ##teamcity[blockOpened name='Configure %BUILD_TYPE% Visual Studio 2015 Win64']
mkdir build_vs2015_x64_%BUILD_TYPE%
cd build_vs2015_x64_%BUILD_TYPE%
echo cmake %SOURCE_DIR% -G"Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=%BUILD_TYPE%  %BUILD_PARAM%
cmake %SOURCE_DIR% -G"Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=%BUILD_TYPE%  %BUILD_PARAM%
if %errorlevel% neq 0 exit /b %errorlevel%
echo ##teamcity[blockClosed name='Configure %BUILD_TYPE% Visual Studio 2015 Win64']

echo ##teamcity[blockOpened name='Build %BUILD_TYPE% Visual Studio 2015 Win64']
cmake --build . --config %BUILD_TYPE% -- /M
if %errorlevel% neq 0 exit /b %errorlevel%
echo ##teamcity[blockClosed name='Build %BUILD_TYPE% Visual Studio 2015 Win64']

echo ##teamcity[blockOpened name='Test %BUILD_TYPE% Visual Studio 2015 Win64']
cmake --build . --target check --config %BUILD_TYPE% -- /M
if %errorlevel% neq 0 exit /b %errorlevel%
echo ##teamcity[blockClosed name='Test %BUILD_TYPE% Visual Studio 2015 Win64']

echo ##teamcity[blockOpened name='Package Release Visual Studio 2015 Win64']
cmake --build . --target package --config Release -- /M
if %errorlevel% neq 0 exit /b %errorlevel%
echo ##teamcity[blockClosed name='Package Release Visual Studio 2015 Win64']

echo ##teamcity[blockOpened name='NuSpec Release Visual Studio 2015 Win64']
cmake --build . --target nuspec --config Release -- /M
if %errorlevel% neq 0 exit /b %errorlevel%
echo ##teamcity[blockClosed name='NuSpec Release Visual Studio 2015 Win64']

