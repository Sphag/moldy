param(
    [string]$BuildDir = "build"
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

$clangTidy = Get-Command clang-tidy -ErrorAction SilentlyContinue
if ($null -eq $clangTidy) {
    Write-Host "clang-tidy was not found on PATH."
    Write-Host "Run scripts/install-tools.ps1 -Check for setup guidance."
    exit 1
}

$cppcheck = Get-Command cppcheck -ErrorAction SilentlyContinue
if ($null -eq $cppcheck) {
    Write-Host "cppcheck was not found on PATH."
    Write-Host "Run scripts/install-tools.ps1 -Check for setup guidance."
    exit 1
}

if ($sourceFiles.Count -eq 0) {
    Write-Host "No C++ files found."
    exit 0
}

$compileCommands = Join-Path $resolvedBuildDir "compile_commands.json"
$clangTidyFiles = @(
    $sourceFiles |
        Where-Object { $_.Extension.ToLowerInvariant() -in @(".cc", ".cpp", ".cxx", ".ixx", ".cppm") }
)

if (Test-Path -LiteralPath $compileCommands) {
    Write-Host "Running clang-tidy on $($clangTidyFiles.Count) translation unit(s)."
    foreach ($sourceFile in $clangTidyFiles) {
        & $clangTidy.Source $sourceFile.FullName -p $resolvedBuildDir
        if ($LASTEXITCODE -ne 0) {
            exit $LASTEXITCODE
        }
    }
}
else {
    $moduleInterfaceFiles = @($clangTidyFiles | Where-Object { $_.Extension.ToLowerInvariant() -eq ".cppm" })
    Write-Host "compile_commands.json was not found in '$resolvedBuildDir'."
    Write-Host "Running clang-tidy fallback on $($moduleInterfaceFiles.Count) module interface file(s) with explicit C++23 args."
    Write-Host "Use a CMake generator that exports compile_commands.json for full clang-tidy coverage."

    foreach ($sourceFile in $moduleInterfaceFiles) {
        & $clangTidy.Source $sourceFile.FullName -- -std=c++23
        if ($LASTEXITCODE -ne 0) {
            exit $LASTEXITCODE
        }
    }
}

Write-Host "Running cppcheck on $($sourceFiles.Count) C++ file(s)."
$cppcheckArgs = @(
    "--enable=warning,style,performance,portability",
    "--error-exitcode=1",
    "--inline-suppr",
    "--language=c++",
    "--std=c++23",
    "--suppressions-list=$repoRoot\.cppcheck-suppressions"
)

foreach ($sourceFile in $sourceFiles) {
    $cppcheckArgs += $sourceFile.FullName
}

& $cppcheck.Source @cppcheckArgs
exit $LASTEXITCODE
