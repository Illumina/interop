set build_number=%1%
call tools\teamcity\pull_request\msvc.bat %build_number% "Visual Studio 14 2015" DotNet 2.7
