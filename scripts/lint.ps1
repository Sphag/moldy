param(
    [string]$BuildDir = "build"
)

$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$buildDirPath = if ([System.IO.Path]::IsPathRooted($BuildDir)) { $BuildDir } else { Join-Path $repoRoot $BuildDir }
$resolvedBuildDir = [System.IO.Path]::GetFullPath($buildDirPath).TrimEnd([System.IO.Path]::DirectorySeparatorChar, [System.IO.Path]::AltDirectorySeparatorChar)
$sourcePatterns = @("*.c", "*.cc", "*.cpp", "*.cxx", "*.h", "*.hh", "*.hpp", "*.hxx")
$sourceFiles = @(
    Get-ChildItem -Path $repoRoot -Recurse -File -Include $sourcePatterns -ErrorAction SilentlyContinue |
        Where-Object {
            $filePath = [System.IO.Path]::GetFullPath($_.FullName)
            $isInConfiguredBuildDir = $filePath.StartsWith($resolvedBuildDir + [System.IO.Path]::DirectorySeparatorChar, [System.StringComparison]::OrdinalIgnoreCase)
            $isCMakeGenerated = $filePath -match "[\\/]CMakeFiles[\\/]"
            -not ($isInConfiguredBuildDir -or $isCMakeGenerated)
        }
)

Write-Host "Linter/static analysis is not selected yet."
Write-Host "Placeholder pass: no lint or static-analysis command was run."

if ($sourceFiles.Count -eq 0) {
    Write-Host "No C++ files found."
    exit 0
}

Write-Host "C++ files found: $($sourceFiles.Count). Lint/static-analysis tool selection is deferred."
exit 0
