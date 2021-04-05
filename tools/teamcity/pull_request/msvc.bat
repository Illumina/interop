set GENERATOR="Visual Studio 14 2015"
set PY=
set CS_BUILD="DotNet"
set TARGET=bundle

set build_number=%1%
if NOT '%2' == '' (
set GENERATOR=%2%
)
if NOT "%3" == "" (
set CS_BUILD=%3%
)
if NOT "%4" == "" (
set PY=%4%
)
if NOT "%5" == "" (
set TARGET=%5%
)

if "%PY%" == "2.7" (
set EXTRA="conda=4.5.3"
)

echo "Generator: %GENERATOR%"
echo "CS_BUILD: %CS_BUILD%"
echo "PY: %PY%"
echo "EXTRA: %EXTRA%"


echo "##teamcity[blockOpened name='Install Deps']"
call tools\prereqs\env_windows.bat %PY% %EXTRA%
if %errorlevel% neq 0 exit /b %errorlevel%
echo "##teamcity[blockClosed name='Install Deps']"

echo "##teamcity[blockOpened name='Disable Float Compression %PY%']"
call tools\package.bat Release %GENERATOR% %TARGET% "-Ax64 -DBUILD_NUMBER=%build_number% -DENABLE_FLOAT_COMPRESSION=OFF -DCSBUILD_TOOL=%CS_BUILD%" %PY%
set errorcode=%errorlevel%
type %CD%\build\CMakeFiles\CMakeOutput.log
if %errorcode% neq 0 exit /b %errorcode%
echo "##teamcity[blockClosed name='Disable Float Compression %PY%']"

echo "##teamcity[blockOpened name='Disable Dynamic Array %PY%']"
call tools\package.bat Release %GENERATOR% %TARGET% "-Ax64 -DBUILD_NUMBER=%build_number% -DDISABLE_DYNAMIC_ARRAY=OFF -DCSBUILD_TOOL=%CS_BUILD%" %PY%
if %errorlevel% neq 0 exit /b %errorlevel%
echo "##teamcity[blockClosed name='Disable Dynamic Array %PY%']"

echo "##teamcity[blockOpened name='Python %PY%']"
call tools\package.bat Release %GENERATOR% %TARGET% "-Ax64 -DBUILD_NUMBER=%build_number% -DCSBUILD_TOOL=%CS_BUILD%" %PY%
if %errorlevel% neq 0 exit /b %errorlevel%
echo "##teamcity[blockClosed name='Python %PY%']"

echo "##teamcity[blockOpened name='RTA Settings %PY%']"
call tools\package.bat Release %GENERATOR% %TARGET% "-Ax64 -DENABLE_BACKWARDS_COMPATIBILITY=OFF -DENABLE_EXAMPLES=OFF -DENABLE_DOCS=OFF -DENABLE_SWIG=OFF -DENABLE_TEST=OFF -DENABLE_APPS=OFF -DENABLE_PYTHON=OFF -DDISABLE_DYNAMIC_ARRAY=ON -DENABLE_FLOAT_COMPRESSION=OFF -DENABLE_PORTABLE=ON" %PY%
if %errorlevel% neq 0 exit /b %errorlevel%
echo "##teamcity[blockClosed name='RTA Settings %PY%']"