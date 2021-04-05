set build_number=%1%
call tools\teamcity\pull_request\msvc.bat %build_number% "Visual Studio 15 2017" DotNet 3.7 package_wheel
