param(
    [Parameter(Mandatory = $true)][string]$OutputDirectory,
    [string[]]$LogPath = @(),
    [ValidateSet('Text', 'Json')][string]$Format = 'Text',
    [string]$RepositoryRoot
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'workflow-common.ps1')
$maximumLogBytes = 64KB

try {
    $root = Resolve-WorkflowRepositoryRoot -RepositoryRoot $RepositoryRoot
    if ($null -eq (Get-Command git -ErrorAction SilentlyContinue)) { throw 'Git is required.' }
    $outputPath = Resolve-WorkflowRepositoryPath -RepositoryRoot $root -Path $OutputDirectory -AllowMissing
    $outputRelative = Get-WorkflowRelativePath -RepositoryRoot $root -Path $outputPath
    if ($outputPath -eq $root -or (Test-WorkflowGeneratedPath -RepositoryRoot $root -Path $outputPath)) { throw "Output directory '$OutputDirectory' is unsafe." }
    if (-not (Test-WorkflowIgnoredPath -RepositoryRoot $root -RelativePath $outputRelative)) { throw "Output directory '$OutputDirectory' must be ignored by Git." }
    if (Test-WorkflowTrackedPath -RepositoryRoot $root -RelativePath $outputRelative) { throw "Output directory '$OutputDirectory' is tracked." }
    $trackedChildren = @(& git -C $root ls-files -- $outputRelative)
    if ($trackedChildren.Count -gt 0) { throw "Output directory '$OutputDirectory' contains tracked files." }
    if (Test-Path -LiteralPath $outputPath -PathType Container) {
        if ((Get-ChildItem -LiteralPath $outputPath -Force | Measure-Object).Count -gt 0) { throw "Output directory '$OutputDirectory' must be empty." }
    }
    elseif (Test-Path -LiteralPath $outputPath) { throw "Output path '$OutputDirectory' is not a directory." }
    else { New-Item -ItemType Directory -Path $outputPath -Force | Out-Null }
}
catch {
    $missing = $_.Exception.Message -eq 'Git is required.'
    $report = @{ status = if ($missing) { 'missing-prerequisite' } else { 'invalid-input' }; exitCode = if ($missing) { $script:WorkflowExit.MissingPrerequisite } else { $script:WorkflowExit.InvalidInput }; data = [ordered]@{ message = $_.Exception.Message } }
    Write-WorkflowReport -Format $Format -Title 'collect-diagnostics' -Report $report
    exit $report.exitCode
}

$failures = @()
$logs = @()
foreach ($inputLogPath in $LogPath) {
    try {
        $fullLogPath = Resolve-WorkflowRepositoryPath -RepositoryRoot $root -Path $inputLogPath
        $relativeLogPath = Get-WorkflowRelativePath -RepositoryRoot $root -Path $fullLogPath
        if ($relativeLogPath -match '(^|[\\/])\.git([\\/]|$)') { throw "Log path '$inputLogPath' is unsafe." }
        $textFile = Get-WorkflowTextFile -Path $fullLogPath
        $redacted = Protect-WorkflowText -Text $textFile.Text
        $bytes = [System.Text.Encoding]::UTF8.GetBytes($redacted)
        $truncated = $bytes.Length -gt $maximumLogBytes
        if ($truncated) { $redacted = [System.Text.Encoding]::UTF8.GetString($bytes, 0, $maximumLogBytes) }
        $fileName = ('log-{0:D2}.txt' -f ($logs.Count + 1))
        [System.IO.File]::WriteAllText((Join-Path $outputPath $fileName), $redacted, [System.Text.UTF8Encoding]::new($false))
        $logs += [ordered]@{ source = $relativeLogPath; artifact = $fileName; byteLength = $textFile.Bytes.Length; truncated = $truncated }
    }
    catch { $failures += "Log '$inputLogPath': $($_.Exception.Message)" }
}

$policyValid = $true
$policyError = $null
try {
    . (Join-Path $PSScriptRoot 'toolchain-policy.ps1')
    $null = Import-ToolchainPolicy -Path (Join-Path $root 'config/toolchain.psd1')
}
catch { $policyValid = $false; $policyError = $_.Exception.Message; $failures += "Toolchain policy: $policyError" }

$gitState = [ordered]@{}
try {
    $gitState.branch = (& git -C $root branch --show-current | Out-String).Trim()
    $gitState.head = (& git -C $root rev-parse HEAD | Out-String).Trim()
    $gitState.status = Protect-WorkflowText -Text ((& git -C $root status --short | Out-String).Trim())
}
catch { $failures += "Git state: $($_.Exception.Message)" }

$commands = @('git', 'cmake', 'ninja', 'clang-format', 'clang-tidy', 'cppcheck') | ForEach-Object { Get-WorkflowCommandHealth -Name $_ }
$commands += Get-WorkflowMsvcHealth
$manifest = [ordered]@{
    schemaVersion = 1
    repositoryRoot = $root
    environment = [ordered]@{ osDescription = [System.Environment]::OSVersion.VersionString; processArchitecture = [System.Runtime.InteropServices.RuntimeInformation]::ProcessArchitecture.ToString(); powerShellVersion = $PSVersionTable.PSVersion.ToString() }
    git = $gitState
    toolchainPolicy = [ordered]@{ valid = $policyValid; error = $policyError }
    commands = $commands
    logs = $logs
    failures = $failures
}
[System.IO.File]::WriteAllText((Join-Path $outputPath 'manifest.json'), ($manifest | ConvertTo-Json -Depth 8), [System.Text.UTF8Encoding]::new($false))
$exitCode = if ($failures.Count -gt 0 -or -not $policyValid) { $script:WorkflowExit.Degraded } else { $script:WorkflowExit.Success }
$report = @{ status = if ($exitCode -eq 0) { 'success' } else { 'degraded' }; exitCode = $exitCode; data = [ordered]@{ outputDirectory = $outputRelative; manifest = (Join-Path $outputRelative 'manifest.json'); logs = $logs; failures = $failures } }
Write-WorkflowReport -Format $Format -Title 'collect-diagnostics' -Report $report
exit $exitCode
