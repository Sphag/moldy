param(
    [string]$BuildDir = "build",
    [string[]]$Configuration = @("Debug", "Release")
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

if ($Configuration.Count -eq 0) {
    Write-Host "At least one build configuration is required."
    exit 1
}

Write-Host "Running project checks for $($Configuration -join ', ')..."

Invoke-CheckStep -Name "toolchain policy" -Action {
    & (Join-Path $scriptDir "test-toolchain-policy.ps1")
}

Invoke-CheckStep -Name "Agent Skills" -Action {
    & (Join-Path $scriptDir "test-agent-skills.ps1")
}

Invoke-CheckStep -Name "tool availability" -Action {
    & (Join-Path $scriptDir "install-tools.ps1") -Check
}

Invoke-CheckStep -Name "configure" -Action {
    & (Join-Path $scriptDir "configure.ps1") -BuildDir $BuildDir -Configuration $Configuration[0]
}

Invoke-CheckStep -Name "format" -Action {
    & (Join-Path $scriptDir "format.ps1") -BuildDir $BuildDir
}

Invoke-CheckStep -Name "lint" -Action {
    & (Join-Path $scriptDir "lint.ps1") -BuildDir $BuildDir
}

foreach ($currentConfiguration in $Configuration) {
    Invoke-CheckStep -Name "build ($currentConfiguration)" -Action {
        & (Join-Path $scriptDir "build.ps1") -BuildDir $BuildDir -Configuration $currentConfiguration
    }

    Invoke-CheckStep -Name "test ($currentConfiguration)" -Action {
        & (Join-Path $scriptDir "test.ps1") -BuildDir $BuildDir -Configuration $currentConfiguration
    }
}

Write-Host ""
Write-Host "Summary: all checks passed."
exit 0
