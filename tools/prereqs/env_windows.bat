echo off
rem --------------------------------------------------------------------------------------------------------------------
rem Software required to build InterOp
rem Assumes you have the windows package manager installed: https://chocolatey.org/
rem --------------------------------------------------------------------------------------------------------------------

set python_version=2.7
set numpy_version=
if NOT '%1' == '' (
 set python_version=%1
)
if NOT '%2' == '' (
set conda_version=%2
)
if NOT '%3' == '' (
set numpy_version="=%3"
)
set MINICONDA_HOME=C:\tools\miniconda3
set CMAKE_HOME=C:\Program Files\CMake\bin
set NUNIT_HOME=C:\Program Files (x86)\NUnit 2.6.4\bin
set JAVA_BIN=C:\Program Files\Java\jdk1.8.0_144\bin
set MINGW_HOME=C:\mingw\mingw64\bin
set DOTNET_HOME=c:\dotnet
set PATH=%PATH%;%CMAKE_HOME%;%NUNIT_HOME%;%JAVA_BIN%;%MINGW_HOME%;%DOTNET_HOME%

rem --------------------------------------------------------------------------------------------------------------------
rem Install MinGW
rem --------------------------------------------------------------------------------------------------------------------
set MINGW_ZIP=x86_64-6.2.0-release-posix-seh-rt_v5-rev0.7z
set MINGW_URL="https://sourceforge.net/projects/mingw-w64/files/Toolchains%%20targetting%%20Win64/Personal%%20Builds/mingw-builds/6.2.0/threads-posix/seh/%MINGW_ZIP%/download"

set DOTNET_URL="https://download.microsoft.com/download/1/1/5/115B762D-2B41-4AF3-9A63-92D9680B9409/dotnet-sdk-2.1.4-win-x64.zip"
set DOTNET_ZIP=dotnet-sdk-2.1.4-win-x64.zip

where /q 7z.exe
if %errorlevel% neq 0  choco install 7zip.install --yes --limit-output

if exist %MINGW_HOME%\gcc.exe goto SKIP_GCC_UPDATE
powershell -Command "[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12;(New-Object System.Net.WebClient).DownloadFile(\"%MINGW_URL%\", \"%MINGW_ZIP%\")"
powershell -Command "& 7z.exe x -oc:\mingw %MINGW_ZIP%" -aoa
:SKIP_GCC_UPDATE

if exist %DOTNET_HOME%\dotnet.exe goto SKIP_DOTNET_UPDATE
powershell -Command "(New-Object System.Net.WebClient).DownloadFile(\"%DOTNET_URL%\", \"%DOTNET_ZIP%\")"
powershell -Command "& 7z.exe x -oc:\dotnet %DOTNET_ZIP%" -aoa
:SKIP_DOTNET_UPDATE

rem --------------------------------------------------------------------------------------------------------------------
rem Install Programs if not found
rem --------------------------------------------------------------------------------------------------------------------

where /q cmake
if %errorlevel% neq 0 choco install cmake --yes --limit-output --installargs 'ADD_CMAKE_TO_PATH=""System""'

choco upgrade cmake --yes --limit-output --installargs 'ADD_CMAKE_TO_PATH=""System""'  --no-progress

choco uninstall miniconda --yes --limit-output
choco uninstall miniconda3 --yes --limit-output
choco install miniconda3 --yes --limit-output  --no-progress
call %MINICONDA_HOME%\Scripts\\activate.bat
call conda config --set always_yes yes --set changeps1 no
call conda info

where /q javac
if %errorlevel% neq 0 choco install jdk8 --yes --limit-output --version 8.0.144 --force  --no-progress
where /q nuget
if %errorlevel% neq 0 choco install nuget.commandline --yes --limit-output

choco install swig --yes --limit-output --version 3.0.12 --allow-downgrade --force  --no-progress
if %errorlevel% neq 0 exit /b %errorlevel%

where /q git
if %errorlevel% neq 0 choco install git.install --yes --limit-output  --no-progress

where /q nunit-console.exe
if %errorlevel% neq 0 choco install nunit --version 2.6.4 --yes --limit-output  --no-progress

rem --------------------------------------------------------------------------------------------------------------------
rem Test if required programs are available
rem --------------------------------------------------------------------------------------------------------------------

where /q javac
if errorlevel 1 (
     dir C:\Program Files\Java\jdk1.8.0_144\bin
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
javac -version
nuget help
swig -version
git --version
dotnet --version


echo "Create environment: %python_version% - %conda_version%"
if '%python_version%' == '' goto SKIP_CONDA_UPDATE
echo "Configure conda"
rem call conda config --add channels conda-forge
rem call conda config --set channel_priority strict
rem call conda config --set allow_conda_downgrades true
echo "Update Anaconda"
call conda update -n base conda
call conda remove --name py%python_version% --all -y
rem call conda install conda=4.6.14 -y
echo "Create environment"
call conda create --no-default-packages -n py%python_version% python=%python_version% -y || echo "Environment exists"
echo "Activate py%python_version%"
call activate py%python_version%
if %errorlevel% neq 0 exit /b %errorlevel%
call conda install numpy wheel pandas -y
if %errorlevel% neq 0 exit /b %errorlevel%
:SKIP_CONDA_UPDATE

echo "Installing requirements complete"


