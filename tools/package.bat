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

set SOURCE_DIR=%CD%
set BUILD_DIR=%SOURCE_DIR%\build
set DIST_DIR=%SOURCE_DIR%\dist
set BUILD_PARAM=
set BUILD_TYPE=Release
set BUILD_PATH=%1%
pushd %BUILD_PATH%
set BUILD_PATH=%CD%
popd
if NOT "%1" == "" (
set BUILD_PARAM=-DGTEST_ROOT=%BUILD_PATH% -DJUNIT_ROOT=%BUILD_PATH% -DGMOCK_ROOT=%BUILD_PATH% -DNUNIT_ROOT=%BUILD_PATH%/NUnit-2.6.4
)
set BUILD_PARAM=%BUILD_PARAM% -DPACKAGE_OUTPUT_FILE_PREFIX=%DIST_DIR%



echo ##teamcity[blockOpened name='Configure %BUILD_TYPE% Visual Studio 2015 Win64']
if exist %BUILD_DIR%  rd /s /q %BUILD_DIR%
if exist %DIST_DIR%  rd /s /q %DIST_DIR%
mkdir %BUILD_DIR%
cd %BUILD_DIR%
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

echo ##teamcity[blockOpened name='Python2 Wheel Release Visual Studio 2015 Win64']
cmake --build . --target package_wheel --config Release -- /M
if %errorlevel% neq 0 exit /b %errorlevel%
echo ##teamcity[blockClosed name='Python2 Wheel Release Visual Studio 2015 Win64']

echo "##teamcity[blockOpened name='NuPack Visual Studio 2015 Win64']"
echo %BUILD_PATH%\nuget pack %BUILD_DIR%\src\ext\csharp\package.nuspec
%BUILD_PATH%\nuget pack %BUILD_DIR%\src\ext\csharp\package.nuspec -OutputDirectory %DIST_DIR%
echo "##teamcity[blockClosed name='NuPack Visual Studio 2015 Win64']"

echo ##teamcity[blockOpened name='Python3 Wheel Release Visual Studio 2015 Win64']
del /f /q CMakeCache.txt
set PYTHON_PATH_DIR=C:\Miniconda3
set PATH=%PYTHON_PATH_DIR%;%PYTHON_PATH_DIR%\Scripts;%PATH%
cmake %SOURCE_DIR% -G"Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=%BUILD_TYPE%  %BUILD_PARAM%
if %errorlevel% neq 0 exit /b %errorlevel%
cmake --build . --target package_wheel --config Release -- /M
if %errorlevel% neq 0 exit /b %errorlevel%
echo ##teamcity[blockClosed name='Python3 Wheel Release Visual Studio 2015 Win64']

cd %SOURCE_DIR%
rd /s /q %BUILD_DIR%
if exist %BUILD_DIR% rd /s /q %BUILD_DIR%
