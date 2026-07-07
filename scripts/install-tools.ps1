param(
    [switch]$Check,
    [switch]$InstallWindows
)

$ErrorActionPreference = "Stop"

$tools = @(
    @{
        Name = "clang-format"
        Package = "LLVM.LLVM"
    },
    @{
        Name = "clang-tidy"
        Package = "LLVM.LLVM"
    },
    @{
        Name = "cppcheck"
        Package = "Cppcheck.Cppcheck"
    }
)

function Show-ToolStatus {
    param([string]$Name)

    $command = Get-Command $Name -ErrorAction SilentlyContinue
    if ($null -eq $command) {
        Write-Host "${Name}: missing"
        return $false
    }

    Write-Host "${Name}: $($command.Source)"
    return $true
}

if ($Check) {
    $allFound = $true
    foreach ($tool in $tools) {
        $found = Show-ToolStatus -Name $tool.Name
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
        if ($null -eq (Get-Command $tool.Name -ErrorAction SilentlyContinue)) {
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
