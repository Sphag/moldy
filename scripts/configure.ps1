param(
    [string]$BuildDir = "build",
    [string]$Configuration = "Debug"
)

$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$cmakeFile = Join-Path $repoRoot "CMakeLists.txt"

if ([System.IO.Path]::IsPathRooted($BuildDir)) {
    $buildDirPath = $BuildDir
}
else {
    $buildDirPath = Join-Path $repoRoot $BuildDir
}

$cacheFile = Join-Path $buildDirPath "CMakeCache.txt"

function Get-IsMultiConfigGenerator {
    param([string]$CachePath)

    if (-not (Test-Path -LiteralPath $CachePath)) {
        return $null
    }

    $cacheText = Get-Content -LiteralPath $CachePath -Raw
    return ($cacheText -match "(?m)^CMAKE_CONFIGURATION_TYPES:STRING=")
}

if (-not (Test-Path -LiteralPath $cmakeFile)) {
    Write-Host "Project scaffold not created yet: CMakeLists.txt is missing."
    Write-Host "Create the C++/CMake scaffold before configuring."
    exit 1
}

$isMultiConfig = Get-IsMultiConfigGenerator -CachePath $cacheFile

if ($null -eq $isMultiConfig) {
    cmake -S $repoRoot -B $buildDirPath
    $initialExitCode = $LASTEXITCODE

    if ($initialExitCode -ne 0) {
        exit $initialExitCode
    }

    $isMultiConfig = Get-IsMultiConfigGenerator -CachePath $cacheFile

    if ($true -eq $isMultiConfig) {
        exit 0
    }
}

if ($true -eq $isMultiConfig) {
    cmake -S $repoRoot -B $buildDirPath
}
else {
    cmake -S $repoRoot -B $buildDirPath "-DCMAKE_BUILD_TYPE=$Configuration"
}

exit $LASTEXITCODE
