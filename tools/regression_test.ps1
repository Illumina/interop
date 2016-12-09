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
#     .\tools\build_test.bat -config Debug -lib_path c:\external -data_path c:\RegressionData -baseline_path c:\BaselineData
#
# Note, you must already have CMake and Visual Studio installed and on your path.
#
# --------------------------------------------------------------------------------------------------------------------


param(
    [string]$config="Debug",
    [string]$lib_path="",
    [Parameter()][ValidateNotNullOrEmpty()][string]$data_path=$(throw "Regression data path must be specified"),
    [Parameter()][ValidateNotNullOrEmpty()][string]$baseline_path=$(throw "Regression baseline path must be specified"),
    [string]$source_path="../",
    [string]$generator="`"Visual Studio 14 2015 Win64`"",
    [switch]$rebaseline = $false
)

$build_param=""
if($lib_path) { $build_param="-DGTEST_ROOT=$lib_path -DJUNIT_ROOT=$lib_path -DGMOCK_ROOT=$lib_path -DNUNIT_ROOT=$lib_path/NUnit-2.6.4"}

# --------------------------------------------------------------------------------------------------------------------
Write-Host "##teamcity[blockOpened name='Configure $config $generator']"
new-item build_vs2015_x64_$config -itemtype directory
set-location -path build_vs2015_x64_$config
Write-Host "cmake $source_path -G $generator -DCMAKE_BUILD_TYPE=$config $build_param -DFORCE_SHARED_CRT=OFF"
cmake $source_path -G $generator -DCMAKE_BUILD_TYPE=$config $build_param -DFORCE_SHARED_CRT=OFF
$test_code=$lastexitcode
Write-Host "##teamcity[blockClosed name='Configure $config $generator']"
if ($test_code -ne 0)
{
    cd ..
    exit $test_code
}

# --------------------------------------------------------------------------------------------------------------------

Write-Host "##teamcity[blockOpened name='Build $config $generator']"
& cmake --build . --config $config --target interop_gtests
$test_code=$lastexitcode
Write-Host "##teamcity[blockClosed name='Build $config $generator']"
if ($test_code -ne 0)
{
    cd ..
    exit $test_code
}

$baseline_path = $baseline_path + "_master"

$datasets = Get-ChildItem $data_path | foreach {$_.fullname}
if($rebaseline)
{
    $backup_baseline=$baseline_path + "_last"
    if(Test-Path -Path $backup_baseline)
    {
        Remove-Item -Recurse -Force $backup_baseline
    }
    Move-Item $baseline_path $backup_baseline
    Write-Host "##teamcity[blockOpened name='Rebaseline $config $generator']"
    & src\tests\interop\$config\interop_gtests.exe $baseline_path $datasets --rebaseline
    $test_code=$lastexitcode
    Write-Host "##teamcity[blockClosed name='Test $config $generator']"
    if ($test_code -ne 0)
    {
        cd ..
        Write-Host "##teamcity[buildStatus status='FAILURE' text='Rebaseline failed!']"
        exit 1
    }
}

Write-Host "##teamcity[blockOpened name='Test $config $generator']"
& src\tests\interop\$config\interop_gtests.exe $baseline_path $datasets
$test_code=$lastexitcode
Write-Host "##teamcity[blockClosed name='Test $config $generator']"
if ($test_code -ne 0)
{
    cd ..
    Write-Host "##teamcity[buildStatus status='FAILURE' text='Not all regression tests passed!']"
    exit 1
}

cd ..


