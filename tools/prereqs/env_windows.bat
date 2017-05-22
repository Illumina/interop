REM Software required to build InterOp

choco install vcbuildtools -y
choco install nuget.commandline -y
choco install miniconda -y
choco install jdk8 -y
choco install swig -y
choco install nunit --version 2.6.4 -y
choco install cmake -y
choco install git.install -y

REM junit
REM gtest, gmock

REM Setup Path
"c:\Program Files\MiniConda2\Scripts\conda.exe" install numpy -y
"c:\Program Files\MiniConda2\Scripts\conda.exe" create --name python3 python=3 numpy -y


REM MinGW
REM Precompiled Deps
REM

