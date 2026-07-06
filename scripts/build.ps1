param(
    [string]$BuildDir = "build",
    [string]$Configuration = "Debug"
)

$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$buildDir = if ([System.IO.Path]::IsPathRooted($BuildDir)) { $BuildDir } else { Join-Path $repoRoot $BuildDir }

Write-Host "Configuring build directory for $Configuration."
& (Join-Path $PSScriptRoot "configure.ps1") -BuildDir $buildDir -Configuration $Configuration
$configureExitCode = $LASTEXITCODE

if ($configureExitCode -ne 0) {
    exit $configureExitCode
}

cmake --build $buildDir --config $Configuration
exit $LASTEXITCODE
