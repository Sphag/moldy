param(
    [string]$BuildDir = "build",
    [string]$Configuration = "Debug"
)

$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

function Invoke-CheckStep {
    param(
        [string]$Name,
        [scriptblock]$Action
    )

    Write-Host ""
    Write-Host "==> $Name"

    & $Action
    $exitCode = $LASTEXITCODE

    if ($exitCode -ne 0) {
        Write-Host ""
        Write-Host "Check failed at step '$Name' with exit code $exitCode."
        Write-Host "Summary: failed."
        exit $exitCode
    }
}

Write-Host "Running project checks for $Configuration..."

Invoke-CheckStep -Name "configure" -Action {
    & (Join-Path $scriptDir "configure.ps1") -BuildDir $BuildDir -Configuration $Configuration
}

Invoke-CheckStep -Name "format" -Action {
    & (Join-Path $scriptDir "format.ps1") -BuildDir $BuildDir
}

Invoke-CheckStep -Name "lint" -Action {
    & (Join-Path $scriptDir "lint.ps1") -BuildDir $BuildDir
}

Invoke-CheckStep -Name "build" -Action {
    & (Join-Path $scriptDir "build.ps1") -BuildDir $BuildDir -Configuration $Configuration
}

Invoke-CheckStep -Name "test" -Action {
    & (Join-Path $scriptDir "test.ps1") -BuildDir $BuildDir -Configuration $Configuration
}

Write-Host ""
Write-Host "Summary: all checks passed."
exit 0
