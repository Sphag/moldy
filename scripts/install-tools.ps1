param(
    [switch]$Check,
    [switch]$InstallWindows
)

$ErrorActionPreference = "Stop"

$tools = @(
    @{
        Name = "clang-format"
        Package = "LLVM.LLVM"
        KnownPaths = @(
            "C:\Program Files\LLVM\bin\clang-format.exe"
        )
    },
    @{
        Name = "clang-tidy"
        Package = "LLVM.LLVM"
        KnownPaths = @(
            "C:\Program Files\LLVM\bin\clang-tidy.exe"
        )
    },
    @{
        Name = "cppcheck"
        Package = "Cppcheck.Cppcheck"
        KnownPaths = @(
            "C:\Program Files\Cppcheck\cppcheck.exe"
        )
    }
)

function Get-ToolCandidates {
    param(
        [string]$Name,
        [string[]]$KnownPaths
    )

    $candidatePaths = [System.Collections.Generic.List[string]]::new()

    foreach ($knownPath in $KnownPaths) {
        if (Test-Path -LiteralPath $knownPath -PathType Leaf) {
            $candidatePaths.Add($knownPath)
        }
    }

    $commands = @(Get-Command $Name -All -ErrorAction SilentlyContinue)
    foreach ($command in $commands) {
        if ($null -ne $command.Source -and -not $candidatePaths.Contains($command.Source)) {
            $candidatePaths.Add($command.Source)
        }
    }

    return $candidatePaths.ToArray()
}

function Test-ToolCandidate {
    param(
        [string]$Name,
        [string]$Path
    )

    if ($Name -ne "cppcheck") {
        & $Path --version *> $null
        return ($LASTEXITCODE -eq 0)
    }

    $probeDir = Join-Path ([System.IO.Path]::GetTempPath()) "moldy-cppcheck-probe"
    $probeFile = Join-Path $probeDir "probe.cpp"
    New-Item -ItemType Directory -Path $probeDir -Force | Out-Null
    Set-Content -LiteralPath $probeFile -Value "int main() { return 0; }" -NoNewline

    try {
        & $Path --enable=warning --error-exitcode=1 --language=c++ --std=c++23 $probeFile *> $null
        return ($LASTEXITCODE -eq 0)
    }
    finally {
        Remove-Item -LiteralPath $probeFile -Force -ErrorAction SilentlyContinue
        Remove-Item -LiteralPath $probeDir -Force -ErrorAction SilentlyContinue
    }
}

function Add-ToolDirectoryToPath {
    param([string]$ToolPath)

    $toolDirectory = Split-Path -Parent $ToolPath
    $pathSegments = $env:PATH -split [System.IO.Path]::PathSeparator
    if ($pathSegments -notcontains $toolDirectory) {
        $env:PATH = $toolDirectory + [System.IO.Path]::PathSeparator + $env:PATH
    }

    if ($env:GITHUB_PATH) {
        Add-Content -LiteralPath $env:GITHUB_PATH -Value $toolDirectory
    }
}

function Resolve-HealthyTool {
    param([hashtable]$Tool)

    $candidates = @(Get-ToolCandidates -Name $Tool.Name -KnownPaths $Tool.KnownPaths)
    foreach ($candidate in $candidates) {
        if (Test-ToolCandidate -Name $Tool.Name -Path $candidate) {
            return $candidate
        }

        Write-Host "$($Tool.Name): ignoring unhealthy candidate $candidate"
    }

    return $null
}

function Show-ToolStatus {
    param([hashtable]$Tool)

    $healthyPath = Resolve-HealthyTool -Tool $Tool
    if ($null -eq $healthyPath) {
        Write-Host "$($Tool.Name): missing or unhealthy"
        return $false
    }

    Add-ToolDirectoryToPath -ToolPath $healthyPath
    Write-Host "$($Tool.Name): $healthyPath"
    return $true
}

if ($Check) {
    $allFound = $true
    foreach ($tool in $tools) {
        $found = Show-ToolStatus -Tool $tool
        $allFound = $allFound -and $found
    }

    if (-not $allFound) {
        Write-Host ""
        Write-Host "Install missing Windows tools with:"
        Write-Host "  scripts/install-tools.ps1 -InstallWindows"
        exit 1
    }

    exit 0
}

if ($InstallWindows) {
    if ($PSVersionTable.PSVersion.Major -ge 6 -and -not $IsWindows) {
        Write-Host "-InstallWindows is only supported on Windows."
        exit 1
    }

    $winget = Get-Command winget -ErrorAction SilentlyContinue
    if ($null -eq $winget) {
        Write-Host "winget was not found. Install LLVM tools and cppcheck manually, then rerun -Check."
        exit 1
    }

    foreach ($tool in $tools) {
        if ($null -eq (Resolve-HealthyTool -Tool $tool)) {
            Write-Host "Installing $($tool.Package) for $($tool.Name)."
            & $winget.Source install --id $tool.Package --exact --source winget --accept-source-agreements --accept-package-agreements --silent
            if ($LASTEXITCODE -ne 0) {
                exit $LASTEXITCODE
            }
        }
    }

    Write-Host ""
    & $PSCommandPath -Check
    exit $LASTEXITCODE
}

Write-Host "Usage:"
Write-Host "  scripts/install-tools.ps1 -Check"
Write-Host "  scripts/install-tools.ps1 -InstallWindows"
exit 0
