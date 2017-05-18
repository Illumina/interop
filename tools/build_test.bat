echo off
setlocal enabledelayedexpansion
rem --------------------------------------------------------------------------------------------------------------------
rem Test Build Script for Windows Compilers
rem
rem The script takes two arguments:
rem     1. Build Configuration: Debug or Release
rem     2. Path to third party binaries: E.g. GTest, NUnit
rem
rem Example Running script (from source directory)
rem     .\tools\build_test.bat Debug c:\external msvc
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
set BUILD_TYPE=Debug
set COMPILER=msvc
set INSTALL=nuspec
set MT=/M
if NOT "%1" == "" (
set BUILD_TYPE=%1
)
set BUILD_PATH=%2%
pushd %BUILD_PATH%
set BUILD_PATH=%CD%
popd
if NOT "%2" == "" (
set BUILD_PARAM=-DGTEST_ROOT=%BUILD_PATH% -DGMOCK_ROOT=%BUILD_PATH% -DNUNIT_ROOT=%BUILD_PATH%/NUnit-2.6.4
)

if NOT "%3" == "" (
set BUILD_PARAM=%BUILD_PARAM% -DBUILD_NUMBER=%3%
)
if NOT "%4" == "" (
set COMPILER=%4%
)

if not exist C:\Program Files\CMake\bin goto SKIP_CMAKE_PATH
set PATH=C:\Program Files\CMake\bin;%PATH%
:SKIP_CMAKE_PATH
for %%X in (cmake.exe) do (set FOUND=%%~$PATH:X)
if not defined FOUND exit /b 1

if exist %BUILD_DIR%  rd /s /q %BUILD_DIR%
if exist %DIST_DIR%  rd /s /q %DIST_DIR%
mkdir %BUILD_DIR%
cd %BUILD_DIR%


set BUILD_PARAM=%BUILD_PARAM% -DPACKAGE_OUTPUT_FILE_PREFIX=%DIST_DIR% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%DIST_DIR%

if "%COMPILER%" == "mingw" (
echo ##teamcity[blockOpened name='Configure %BUILD_TYPE% MinGW']
echo cmake %SOURCE_DIR% -G"MinGW Makefiles" %BUILD_PARAM% -DENABLE_PYTHON=OFF
cmake %SOURCE_DIR% -G"MinGW Makefiles" %BUILD_PARAM% -DENABLE_PYTHON=OFF
if !errorlevel! neq 0 exit /b !errorlevel!
echo ##teamcity[blockClosed name='Configure %BUILD_TYPE% MinGW']
set MT=-j8
set INSTALL=install
)

if "%COMPILER%" == "msvc" (
echo ##teamcity[blockOpened name='Configure %BUILD_TYPE% Visual Studio 2015 Win64']
echo cmake %SOURCE_DIR% -G"Visual Studio 14 2015 Win64"  %BUILD_PARAM%
cmake %SOURCE_DIR% -G"Visual Studio 14 2015 Win64" %BUILD_PARAM%
if !errorlevel! neq 0 exit /b !errorlevel!
echo ##teamcity[blockClosed name='Configure %BUILD_TYPE% Visual Studio 2015 Win64']
)

if "%5" == "nuspec" goto NUSPEC_ONLY

echo ##teamcity[blockOpened name='Build %BUILD_TYPE% %COMPILER%']
cmake --build . --config %BUILD_TYPE% -- %MT%
if %errorlevel% neq 0 exit /b %errorlevel%
echo ##teamcity[blockClosed name='Build %BUILD_TYPE% %COMPILER%']


echo ##teamcity[blockOpened name='Package %BUILD_TYPE% %COMPILER%']
cmake --build . --config Release --target package -- %MT%
if %errorlevel% neq 0 exit /b %errorlevel%
echo ##teamcity[blockClosed name='Package %BUILD_TYPE% %COMPILER%']

echo ##teamcity[blockOpened name='Test %BUILD_TYPE% %COMPILER%']
cmake --build . --config %BUILD_TYPE% --target check -- %MT%
if %errorlevel% neq 0 exit /b %errorlevel%
echo ##teamcity[blockClosed name='Test %BUILD_TYPE% %COMPILER%']

:NUSPEC_ONLY

echo ##teamcity[blockOpened name='Install %BUILD_TYPE% %COMPILER%']
cmake --build . --config Release --target %INSTALL% -- %MT%
if %errorlevel% neq 0 exit /b %errorlevel%
echo ##teamcity[blockClosed name='Install %BUILD_TYPE% %COMPILER%']


if "%COMPILER%" == "msvc" (
echo ##teamcity[blockOpened name='NuPack %BUILD_TYPE% %COMPILER%']
echo %BUILD_PATH%\nuget pack %BUILD_DIR%\src\ext\csharp\package.nuspec
%BUILD_PATH%\nuget pack %BUILD_DIR%\src\ext\csharp\package.nuspec -OutputDirectory %DIST_DIR%
echo ##teamcity[blockClosed name='NuPack %BUILD_TYPE% %COMPILER%']
)
if "%5" == "nuspec" goto SCRIPT_END

echo ##teamcity[blockOpened name='Test Python3 %BUILD_TYPE% %COMPILER%']
del /f /q CMakeCache.txt
set PYTHON_PATH_DIR=C:\Miniconda3
set PATH=%PYTHON_PATH_DIR%;%PYTHON_PATH_DIR%\Scripts;%PATH%
cmake %SOURCE_DIR% -G"Visual Studio 14 2015 Win64" %BUILD_PARAM%
if "%COMPILER%" == "msvc" (
cmake --build . --config %BUILD_TYPE% --target check_python -- %MT%
)
echo ##teamcity[blockClosed name='Test Python3 %BUILD_TYPE% %COMPILER%']

:SCRIPT_END

cd %SOURCE_DIR%
rd /s /q %BUILD_DIR%
if exist %BUILD_DIR% rd /s /q %BUILD_DIR%

