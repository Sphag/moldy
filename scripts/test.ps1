param(
    [string]$BuildDir = "build",
    [string]$Configuration = "Debug"
)

$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$cmakeFile = Join-Path $repoRoot "CMakeLists.txt"
$buildDir = if ([System.IO.Path]::IsPathRooted($BuildDir)) { $BuildDir } else { Join-Path $repoRoot $BuildDir }

if (-not (Test-Path -LiteralPath $cmakeFile)) {
    Write-Host "No tests configured because the project scaffold is absent."
    exit 1
}

Write-Host "Configuring build directory for $Configuration."
& (Join-Path $PSScriptRoot "configure.ps1") -BuildDir $buildDir -Configuration $Configuration
$configureExitCode = $LASTEXITCODE

if ($configureExitCode -ne 0) {
    exit $configureExitCode
}

$cacheText = Get-Content -LiteralPath (Join-Path $buildDir "CMakeCache.txt") -Raw
$testTargets = @("core_tests", "math_tests", "math_assertion_tests", "math_policy_tests")
if ($cacheText -match "(?m)^MOLDY_MATH_BUILD_CUSTOM_ASSERT_FIXTURE:BOOL=ON$") {
    $testTargets += "math_custom_assert_tests"
}

cmake --build $buildDir --config $Configuration --target $testTargets
$buildExitCode = $LASTEXITCODE

if ($buildExitCode -ne 0) {
    exit $buildExitCode
}

ctest --test-dir $buildDir --build-config $Configuration --output-on-failure
exit $LASTEXITCODE
