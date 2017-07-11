# --------------------------------------------------------------------------------------------------------------------
# Regression test for MS Visual Studio Compiler
#
# The script takes 4 arguments:
#     1. Build Configuration: Debug or Release
#     2. Path to third party binaries: E.g. GTest, NUnit
#     3. Path to regression test data
#     4. Path to baseline data
#
# Example Running script (from source directory)
#     .\tools\build_test.bat -config Release -lib_path c:\external -data_path c:\RegressionData -baseline_path c:\BaselineData
#
# Note, you must already have CMake and Visual Studio installed and on your path.
#
# --------------------------------------------------------------------------------------------------------------------


param(
    [string]$config="Debug",
    [string]$lib_path="",
    [Parameter()][ValidateNotNullOrEmpty()][string]$data_path=$(throw "Regression data path must be specified"),
    [Parameter()][ValidateNotNullOrEmpty()][string]$baseline_path=$(throw "Regression baseline path must be specified"),
    [string]$source_path=".",
    [string]$generator="`"Visual Studio 14 2015 Win64`"",
    [switch]$rebaseline = $false
)

$build_param=""
if($lib_path)
{
$lib_path=(resolve-path $lib_path).path
}

# --------------------------------------------------------------------------------------------------------------------
Write-Host "##teamcity[blockOpened name='Configure $config $generator']"
if(Test-Path -Path build){
    Remove-Item build -Force -Recurse
}

Write-Host "cmake $source_path -Bbuild -G $generator -DCMAKE_BUILD_TYPE=$config $build_param"
cmake $source_path -Bbuild -G $generator -DCMAKE_BUILD_TYPE=$config $build_param.Split(" ")
$test_code=$lastexitcode
Write-Host "##teamcity[blockClosed name='Configure $config $generator']"
if ($test_code -ne 0)
{
    Write-Host "##teamcity[buildStatus status='FAILURE' text='Configure Failed!']"
    if(Test-Path -Path build){
        Remove-Item build -Force -Recurse
    }
    exit $test_code
}

# --------------------------------------------------------------------------------------------------------------------

Write-Host "##teamcity[blockOpened name='Build $config $generator']"
& cmake --build build --config $config --target interop_gtests
$test_code=$lastexitcode
Write-Host "##teamcity[blockClosed name='Build $config $generator']"
if ($test_code -ne 0)
{
    Write-Host "##teamcity[buildStatus status='FAILURE' text='Build Failed!']"
    if(Test-Path -Path build){
        Remove-Item build -Force -Recurse
    }
    exit 1
}

$baseline_path = $baseline_path + "_master"

if(-Not (Test-Path -Path $data_path) ) {
    Write-Host "##teamcity[buildStatus status='FAILURE' text='Cannot find data directory!']"
    if(Test-Path -Path build){
        Remove-Item build -Force -Recurse
    }
    exit 1
}
$datasets = Get-ChildItem $data_path -Filter RunInfo.xml -Recurse | foreach {$_.DirectoryName}
if($rebaseline)
{
    $backup_baseline=$baseline_path + "_last"
    if(Test-Path -Path $backup_baseline)
    {
        Write-Host "Remove backup baseline: $baseline_path"
        Remove-Item -Recurse -Force $backup_baseline
    }
    Move-Item $baseline_path $backup_baseline
    Write-Host "##teamcity[blockOpened name='Rebaseline $config $generator']"
    & build\src\tests\interop\$config\interop_gtests.exe $baseline_path $datasets --rebaseline
    $test_code=$lastexitcode
    Write-Host "##teamcity[blockClosed name='Test $config $generator']"
    if ($test_code -ne 0)
    {
        Write-Host "##teamcity[buildStatus status='FAILURE' text='Rebaseline failed!']"
        if(Test-Path -Path build){
            Remove-Item build -Force -Recurse
        }
        exit 1
    }
}

Write-Host "##teamcity[blockOpened name='Test $config $generator']"
& build\src\tests\interop\$config\interop_gtests.exe $baseline_path $datasets
$test_code=$lastexitcode
Write-Host "##teamcity[blockClosed name='Test $config $generator']"
if ($test_code -ne 0)
{
    Write-Host "##teamcity[buildStatus status='FAILURE' text='Not all regression tests passed!']"
    if(Test-Path -Path build){
        Remove-Item build -Force -Recurse
    }
    exit 1
}

if(Test-Path -Path build){
    Remove-Item build -Force -Recurse
}

