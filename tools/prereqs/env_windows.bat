rem Software required to build InterOp

rem Assumes you have the windows package manager installed: https://chocolatey.org/
choco install -y vcbuildtools -ia "/Full"
choco info visualcpp-build-tools || choco install visualcpp-build-tools -y --params "'/IncludeOptional'"
rem choco install vcbuildtools -y
choco install miniconda -y
choco install jdk8 -y
choco install cmake -y --installargs 'ADD_CMAKE_TO_PATH=""User""'
choco install microsoft-build-tools -y

rem fails on AWS
rem choco install windows-sdk-8.1 -y

setx path "%PATH%;C:\ProgramData\Miniconda;C:\ProgramData\Miniconda\Scripts"

rem Available dependency
choco install swig -y
choco install nunit --version 2.6.4 -y
choco install nuget.commandline -y

rem Already supported by build server
rem choco install git.install -y

rem junit
rem gtest, gmock

rem Setup Path
conda install numpy -y
conda create --name python3 python=3 numpy -y


rem MinGW
REM Precompiled Deps
REM

