param(
    [ValidateSet('Text', 'Json')][string]$Format = 'Text',
    [string]$RepositoryRoot
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'workflow-common.ps1')

try {
    $root = Resolve-WorkflowRepositoryRoot -RepositoryRoot $RepositoryRoot
}
catch {
    $report = @{ status = 'invalid-input'; exitCode = $script:WorkflowExit.InvalidInput; data = [ordered]@{ message = $_.Exception.Message } }
    Write-WorkflowReport -Format $Format -Title 'doctor' -Report $report
    exit $report.exitCode
}

$policyValid = $true
$policyError = $null
try {
    . (Join-Path $PSScriptRoot 'toolchain-policy.ps1')
    $null = Import-ToolchainPolicy -Path (Join-Path $root 'config/toolchain.psd1')
}
catch { $policyValid = $false; $policyError = $_.Exception.Message }

$qualityToolExitCode = $script:WorkflowExit.Success
$qualityToolOutput = ''
try {
    $qualityToolOutput = & (Join-Path $PSScriptRoot 'install-tools.ps1') -Check 2>&1 | Out-String
    $qualityToolExitCode = $LASTEXITCODE
}
catch { $qualityToolExitCode = $script:WorkflowExit.MissingPrerequisite; $qualityToolOutput = $_.Exception.Message }

$gitHealth = Get-WorkflowCommandHealth -Name 'git'
$commands = @('cmake', 'ninja', 'clang-format', 'clang-tidy', 'cppcheck') | ForEach-Object { Get-WorkflowCommandHealth -Name $_ }
$commands += Get-WorkflowMsvcHealth
$missing = @($commands + $gitHealth | Where-Object { -not $_.available } | ForEach-Object name)
$unhealthy = @($commands + $gitHealth | Where-Object { $_.available -and -not $_.healthy } | ForEach-Object name)
$exitCode = if ($qualityToolExitCode -ne 0 -or $missing.Count -gt 0) { $script:WorkflowExit.MissingPrerequisite } elseif (-not $policyValid -or $unhealthy.Count -gt 0) { $script:WorkflowExit.Degraded } else { $script:WorkflowExit.Success }
$status = if ($exitCode -eq 0) { 'success' } elseif ($exitCode -eq 3) { 'missing-prerequisite' } else { 'degraded' }
$report = @{ status = $status; exitCode = $exitCode; data = [ordered]@{ repositoryRoot = $root; git = $gitHealth; policyValid = $policyValid; policyError = $policyError; qualityToolExitCode = $qualityToolExitCode; qualityToolOutput = (Protect-WorkflowText -Text $qualityToolOutput).Trim(); commands = $commands; missingCommands = $missing; unhealthyCommands = $unhealthy } }
Write-WorkflowReport -Format $Format -Title 'doctor' -Report $report
exit $exitCode
