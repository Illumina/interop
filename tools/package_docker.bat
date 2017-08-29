rem Build InterOp in Docker



set image=ezralanglois/interop
set dist_dir=dist

if exist %dist_dir%  rd /s /q %dist_dir%
mkdir %dist_dir%

docker pull %image%
if %errorlevel% neq 0 exit /b %errorlevel%
docker run --rm -v %cd%:/src:ro -v %cd%/dist:/dist:rw %image% sh /io/tools/package.sh /io /dist teamcity OFF Release
if %errorlevel% neq 0 exit /b %errorlevel%

