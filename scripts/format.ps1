param(
    [string]$BuildDir = "build",
    [switch]$Fix
)

$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$buildDirPath = if ([System.IO.Path]::IsPathRooted($BuildDir)) { $BuildDir } else { Join-Path $repoRoot $BuildDir }
$resolvedBuildDir = [System.IO.Path]::GetFullPath($buildDirPath).TrimEnd([System.IO.Path]::DirectorySeparatorChar, [System.IO.Path]::AltDirectorySeparatorChar)
$sourceExtensions = @(".c", ".cc", ".cpp", ".cxx", ".h", ".hh", ".hpp", ".hxx", ".ixx", ".cppm")

function Test-IsGeneratedPath {
    param([string]$Path)

    $fullPath = [System.IO.Path]::GetFullPath($Path)
    $directory = [System.IO.Path]::GetDirectoryName($fullPath)
    $directorySegments = $directory.Split([System.IO.Path]::DirectorySeparatorChar, [System.IO.Path]::AltDirectorySeparatorChar)
    $isInConfiguredBuildDir = $fullPath.StartsWith($resolvedBuildDir + [System.IO.Path]::DirectorySeparatorChar, [System.StringComparison]::OrdinalIgnoreCase)
    $isInBuildLikeDir = $directorySegments | Where-Object { $_ -eq "out" -or $_ -like "build*" -or $_ -eq "CMakeFiles" -or $_ -eq ".git" }

    return $isInConfiguredBuildDir -or ($null -ne $isInBuildLikeDir)
}

$sourceFiles = @(
    Get-ChildItem -Path $repoRoot -Recurse -File -ErrorAction SilentlyContinue |
        Where-Object {
            ($sourceExtensions -contains $_.Extension.ToLowerInvariant()) -and -not (Test-IsGeneratedPath -Path $_.FullName)
        }
)

$clangFormat = Get-Command clang-format -ErrorAction SilentlyContinue
if ($null -eq $clangFormat) {
    Write-Host "clang-format was not found on PATH."
    Write-Host "Run scripts/install-tools.ps1 -Check for setup guidance."
    exit 1
}

if ($sourceFiles.Count -eq 0) {
    Write-Host "No C++ files found."
    exit 0
}

if ($Fix) {
    Write-Host "Formatting $($sourceFiles.Count) C++ file(s) with clang-format."
    foreach ($sourceFile in $sourceFiles) {
        & $clangFormat.Source --style=file -i $sourceFile.FullName
        if ($LASTEXITCODE -ne 0) {
            exit $LASTEXITCODE
        }
    }

    exit 0
}

Write-Host "Checking $($sourceFiles.Count) C++ file(s) with clang-format."
foreach ($sourceFile in $sourceFiles) {
    & $clangFormat.Source --style=file --dry-run --Werror $sourceFile.FullName
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Format check failed. Run scripts/format.ps1 -Fix to update formatting."
        exit $LASTEXITCODE
    }
}

exit 0
