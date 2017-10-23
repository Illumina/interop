rem Software required to build InterOp


choco install cmake --yes --limit-output --installargs 'ADD_CMAKE_TO_PATH=""User""'
choco install microsoft-build-tools --yes --limit-output
choco install jdk8 --yes --limit-output
choco install swig --yes --limit-output
choco install nunit --version 2.6.4 --yes --limit-output
choco install nuget.commandline --yes --limit-output

choco install vcbuildtools -ia "/Full" --yes --limit-output

choco install miniconda --yes --limit-output
setx path "%PATH%;C:\ProgramData\Miniconda;C:\ProgramData\Miniconda\Scripts"
setx path "%PATH%;c:\tools\miniconda;C:\tools\miniconda\scripts"
setx path "%PATH%;c:\ProgramData\chocolaty\lib\miniconda;C:\ProgramData\chocolaty\lib\miniconda\scripts"
where /q conda
if %errorlevel% neq 0 exit %errorlevel%

rem Assumes you have the windows package manager installed: https://chocolatey.org/
rem choco install -y vcbuildtools -ia "/Full" || echo "Already installed Visual Studio 2015"
rem choco info visualcpp-build-tools || choco install visualcpp-build-tools -y --params "'/IncludeOptional'"
rem choco install vcbuildtools -y
rem choco install miniconda -y
rem choco install jdk8 -y

rem fails on AWS
rem choco install windows-sdk-8.1 -y


rem Available dependency


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

