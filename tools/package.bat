rem echo off
rem --------------------------------------------------------------------------------------------------------------------
rem Package InterOp for Windows Systems
rem
rem The script takes two arguments:
rem     1. Path to third party binaries: E.g. GTest, NUnit
rem
rem Example Running script (from source directory)
rem     .\tools\package.bat Release mingw bundle "-DENABLE_APPS=OFF -DENABLE_EXAMPLES=OFF -DENABLE_CSHARP=OFF"
rem
rem All arguments are required for this script to work!
rem
rem Note, you must already have CMake, MinGW and Visual Studio installed and on your path.
rem
rem To select an older Visual Studio toolset use: cmake -T v120,host=x64
rem
rem --------------------------------------------------------------------------------------------------------------------

set MINICONDA_HOME=C:\ProgramData\Miniconda2
set MINGW_HOME=C:\Program Files\mingw-w64\x86_64-6.3.0-posix-seh-rt_v5-rev2\mingw64\bin
set CMAKE_HOME=C:\Program Files\CMake\bin
set MINGW_HOME=C:\mingw\mingw64\bin
set DOTNET_HOME=c:\dotnet
rem set PATH=%MINGW_HOME%;%MINICONDA_HOME%;%MINICONDA_HOME%\Scripts;%DOTNET_HOME%;%PATH%

set SOURCE_DIR=%CD%
set BUILD_DIR=%SOURCE_DIR%\build
set DIST_DIR=%SOURCE_DIR%\dist
set BUILD_PARAM=
set BUILD_TYPE=Release
set COMPILER=Visual Studio 14 2015 Win64
set MT=/M
set PACKAGE_TARGET=bundle
set PREFIX_BEG=##teamcity[blockOpened name='
set PREFIX_END=##teamcity[blockClosed name='
set SUFFIX=']
set python_version=

if NOT "%1" == "" (
set BUILD_TYPE=%1%
)
if NOT '%2' == '' (
set COMPILER=%2%
)
if NOT "%3" == "" (
set PACKAGE_TARGET=%3%
)
if NOT '%4' == '' (
 set ADDIONAL_CONFIG_OPTIONS=%4%
)
if NOT '%5' == '' (
 set python_version=%5%
)
if NOT '%6' == '' (
 set DIST_DIR=%6%
)
set ADDIONAL_CONFIG_OPTIONS=%ADDIONAL_CONFIG_OPTIONS:"=%

set BUILD_PARAM=%BUILD_PARAM% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DPACKAGE_OUTPUT_FILE_PREFIX=%CD%\dist %ADDIONAL_CONFIG_OPTIONS%

if '%python_version%' == '' goto SKIP_CONDA_UPDATE
where /q conda
if %errorlevel% neq 0 goto SKIP_CONDA_UPDATE
echo "Create environment: %python_version%"
conda create -n py%python_version% python=%python_version% numpy wheel -y || echo "Environment exists"
echo "Activate py%python_version%"
call activate py%python_version%
if %errorlevel% neq 0 exit /b %errorlevel%
pip install --upgrade --force-reinstall numpy
pip install pandas
:SKIP_CONDA_UPDATE


rem Clean build and dist directories
if exist %BUILD_DIR%  rd /s /q %BUILD_DIR%
if exist %DIST_DIR%  rd /s /q %DIST_DIR%
mkdir %DIST_DIR%

rem Ensure cmake is on the PATH
if not exist %CMAKE_HOME% goto SKIP_CMAKE_PATH
set PATH=%CMAKE_HOME%;%PATH%
:SKIP_CMAKE_PATH
for %%X in (cmake.exe) do (set FOUND=%%~$PATH:X)
if not defined FOUND exit /b 1

rem Check compiler type
if %COMPILER% == mingw goto CONFIGURE_MINGW
if %COMPILER% == "mingw" goto CONFIGURE_MINGW
if %COMPILER% == "MinGW Makefiles" goto CONFIGURE_MINGW

rem --------------------------------------------------------------------------------------------------------------------
rem Configure for MSVC
rem --------------------------------------------------------------------------------------------------------------------

rem https://github.com/actions/setup-python/issues/121
echo %PREFIX_BEG% Configure %SUFFIX%
cmake %SOURCE_DIR% -G%COMPILER% -B%BUILD_DIR% %BUILD_PARAM% -A x64
if "%errorlevel%" == "0" goto CONFIGURE_SUCCESS
set level=%errorlevel%
type %BUILD_DIR%\CMakeFiles\CMakeError.log

exit /b %level%

:CONFIGURE_SUCCESS
echo %PREFIX_END% Configure %SUFFIX%

goto CONFIGURE_DONE
:CONFIGURE_MINGW
rem --------------------------------------------------------------------------------------------------------------------
rem Configure for MinGW
rem --------------------------------------------------------------------------------------------------------------------

rem Ensure MinGW-w64 is on the PATH
if not exist %MINGW_HOME% goto SKIP_MINGW_PATH
set PATH=%MINGW_HOME%;%PATH%
:SKIP_MINGW_PATH

echo %PREFIX_BEG% Configure %SUFFIX%
cmake %SOURCE_DIR% -G"MinGW Makefiles" -B%BUILD_DIR% -DENABLE_PYTHON=OFF %BUILD_PARAM%
if %errorlevel% neq 0 exit /b %errorlevel%
echo %PREFIX_END% Configure %SUFFIX%

set MT=-j4

:CONFIGURE_DONE

rem --------------------------------------------------------------------------------------------------------------------
rem Build
rem --------------------------------------------------------------------------------------------------------------------

echo %PREFIX_BEG% Build %SUFFIX%
cmake --build %BUILD_DIR% --config %BUILD_TYPE%
rem -- %MT%
if %errorlevel% neq 0 exit /b %errorlevel%
echo %PREFIX_END% Build %SUFFIX%

rem --------------------------------------------------------------------------------------------------------------------
rem Test
rem --------------------------------------------------------------------------------------------------------------------

echo %PREFIX_BEG% Test %SUFFIX%
cmake --build %BUILD_DIR% --config %BUILD_TYPE% --target check -- %MT%
if %errorlevel% neq 0 exit /b %errorlevel%
echo %PREFIX_END% Test %SUFFIX%

rem --------------------------------------------------------------------------------------------------------------------
rem Package
rem --------------------------------------------------------------------------------------------------------------------

echo %PREFIX_BEG% %PACKAGE_TARGET% %SUFFIX%
cmake --build %BUILD_DIR% --config %BUILD_TYPE% --target %PACKAGE_TARGET% -- %MT%
if %errorlevel% neq 0 exit /b %errorlevel%
echo %PREFIX_END% %PACKAGE_TARGET% %SUFFIX%

rd /s /q %BUILD_DIR%
if exist %BUILD_DIR% rd /s /q %BUILD_DIR%

