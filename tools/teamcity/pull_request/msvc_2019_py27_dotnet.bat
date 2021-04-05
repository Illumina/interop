set build_number=%1%
call tools\teamcity\pull_request\msvc.bat %build_number% "Visual Studio 16 2019" DotNet 3.8
