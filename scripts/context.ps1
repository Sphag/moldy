param(
    [Parameter(Mandatory = $true)][string[]]$Path,
    [ValidateSet('Text', 'Json')][string]$Format = 'Text',
    [string]$RepositoryRoot
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'workflow-common.ps1')
$maximumFiles = 100
$maximumContentBytes = 64KB

try {
    $root = Resolve-WorkflowRepositoryRoot -RepositoryRoot $RepositoryRoot
    if ($Path.Count -eq 0) { throw 'At least one scope path is required.' }
    $files = @()
    foreach ($inputPath in $Path) {
        $fullPath = Resolve-WorkflowRepositoryPath -RepositoryRoot $root -Path $inputPath
        $relativePath = Get-WorkflowRelativePath -RepositoryRoot $root -Path $fullPath
        if (Test-WorkflowGeneratedPath -RepositoryRoot $root -Path $fullPath) { throw "Path '$inputPath' is in a generated or .git tree." }
        if (Test-WorkflowIgnoredPath -RepositoryRoot $root -RelativePath $relativePath) { throw "Path '$inputPath' is ignored." }
        if (-not (Test-WorkflowTrackedPath -RepositoryRoot $root -RelativePath $relativePath)) { throw "Path '$inputPath' is not tracked." }
        $files += [ordered]@{ fullPath = $fullPath; relativePath = $relativePath }
    }
    $files = @($files | Sort-Object relativePath -Unique)
    if ($files.Count -gt $maximumFiles) { throw "Scope contains more than $maximumFiles files." }

    $usedBytes = 0
    $truncated = $false
    $items = @()
    foreach ($file in $files) {
        $fileText = Get-WorkflowTextFile -Path $file.fullPath
        $redacted = Protect-WorkflowText -Text $fileText.Text
        $contentBytes = [System.Text.Encoding]::UTF8.GetBytes($redacted)
        $remaining = $maximumContentBytes - $usedBytes
        if ($remaining -le 0) { $truncated = $true; break }
        if ($contentBytes.Length -gt $remaining) {
            $redacted = [System.Text.Encoding]::UTF8.GetString($contentBytes, 0, $remaining)
            $truncated = $true
        }
        $usedBytes += [System.Text.Encoding]::UTF8.GetByteCount($redacted)
        $items += [ordered]@{ path = $file.relativePath; byteLength = $fileText.Bytes.Length; sha256 = Get-WorkflowSha256 -Bytes $fileText.Bytes; content = $redacted }
        if ($truncated) { break }
    }
    $exitCode = if ($truncated) { $script:WorkflowExit.Degraded } else { $script:WorkflowExit.Success }
    $report = @{ status = if ($truncated) { 'degraded' } else { 'success' }; exitCode = $exitCode; data = [ordered]@{ repositoryRoot = $root; limits = [ordered]@{ maximumFiles = $maximumFiles; maximumContentBytes = $maximumContentBytes }; contentBytes = $usedBytes; truncated = $truncated; files = $items } }
}
catch {
    $report = @{ status = 'invalid-input'; exitCode = $script:WorkflowExit.InvalidInput; data = [ordered]@{ message = $_.Exception.Message } }
}
Write-WorkflowReport -Format $Format -Title 'context' -Report $report
exit $report.exitCode
