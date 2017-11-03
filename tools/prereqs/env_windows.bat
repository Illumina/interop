echo off
rem --------------------------------------------------------------------------------------------------------------------
rem Software required to build InterOp
rem Assumes you have the windows package manager installed: https://chocolatey.org/
rem --------------------------------------------------------------------------------------------------------------------

set MINICONDA_HOME=C:\ProgramData\Miniconda2
set CMAKE_HOME=C:\Program Files\CMake\bin
set NUNIT_HOME=C:\Program Files (x86)\NUnit 2.6.4\bin
set JAVA_HOME=C:\Program Files\Java\jdk1.8.0_144\bin
set PATH=%PATH%;%MINICONDA_HOME%;%MINICONDA_HOME%\Scripts;%CMAKE_HOME%;%NUNIT_HOME%;%JAVA_HOME%

rem --------------------------------------------------------------------------------------------------------------------
rem Install Programs if not found
rem --------------------------------------------------------------------------------------------------------------------

where /q cmake
if %errorlevel% neq 0 choco install cmake --yes --limit-output --installargs 'ADD_CMAKE_TO_PATH=""System""'
where /q conda
if %errorlevel% neq 0 choco install miniconda --yes --limit-output
where /q javac
if %errorlevel% neq 0 choco install jdk8 --yes --limit-output --version 8.0.144
where /q nuget
if %errorlevel% neq 0 choco install nuget.commandline --yes --limit-output
where /q swig
if %errorlevel% neq 0 choco install swig --yes --limit-output
where /q git
if %errorlevel% neq 0 choco install git.install --yes --limit-output
where /q nunit-console.exe
if %errorlevel% neq 0 choco install nunit --version 2.6.4 --yes --limit-output


rem --------------------------------------------------------------------------------------------------------------------
rem Install MinGW if Missing (TODO: So far we are dropping supoprt for MinGW)
rem --------------------------------------------------------------------------------------------------------------------

rem set MINGW_HOME=C:\Program Files\mingw-w64\x86_64-5.3.0-posix-seh-rt_v5-rev2\mingw64\bin
rem choco install mingw -params "/exception:seh /threads:posix"
rem dir C:\Program Files\mingw-w64


rem --------------------------------------------------------------------------------------------------------------------
rem Test if required programs are available
rem --------------------------------------------------------------------------------------------------------------------

where /q javac
if errorlevel 1 (
     echo Failed to find javac
     exit /b %errorlevel%
)

where /q conda
if errorlevel 1 (
    echo Failed to find conda
    exit /b %errorlevel%
)

where /q activate
if errorlevel 1 (
    echo Failed to find activate
    exit /b %errorlevel%
)

where /q swig
if errorlevel 1 (
    echo Failed to find swig
    exit /b %errorlevel%
)

where /q nuget
if errorlevel 1 (
    echo Failed to find nuget
    exit /b %errorlevel%
)

where /q git
if errorlevel 1 (
    echo Failed to find git
    exit /b %errorlevel%
)


rem --------------------------------------------------------------------------------------------------------------------
rem Version Info
rem --------------------------------------------------------------------------------------------------------------------

cmake --version
conda --version
javac -version
nuget help
swig -version
git --version

rem --------------------------------------------------------------------------------------------------------------------
rem Install Python Requirements
rem --------------------------------------------------------------------------------------------------------------------

conda install numpy -y -q
if %errorlevel% neq 0 exit %errorlevel%


activate python3 || conda create --name python3 python=3 numpy -y -q


