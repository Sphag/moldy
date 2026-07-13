param(
    [string]$BaseRef,
    [ValidateSet('Text', 'Json')][string]$Format = 'Text',
    [string]$RepositoryRoot
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'workflow-common.ps1')

function Invoke-ChangedCheck {
    param([string]$Selection, [string]$Root, [string]$Base)

    if ($Selection -eq 'documentation') {
        $output = & git -C $Root diff --check "$Base...HEAD" 2>&1 | Out-String
        $firstExitCode = $LASTEXITCODE
        if ($firstExitCode -eq 0) { $output += (& git -C $Root diff --cached --check 2>&1 | Out-String); $firstExitCode = $LASTEXITCODE }
        if ($firstExitCode -eq 0) { $output += (& git -C $Root diff --check 2>&1 | Out-String); $firstExitCode = $LASTEXITCODE }
        return [ordered]@{ exitCode = $firstExitCode; output = $output }
    }
    if ($Selection -eq 'agent-skills') {
        $output = & (Join-Path $PSScriptRoot 'test-agent-skills.ps1') 2>&1 | Out-String
        return [ordered]@{ exitCode = $LASTEXITCODE; output = $output }
    }
    $output = & (Join-Path $PSScriptRoot 'check.ps1') 2>&1 | Out-String
    return [ordered]@{ exitCode = $LASTEXITCODE; output = $output }
}

try {
    $root = Resolve-WorkflowRepositoryRoot -RepositoryRoot $RepositoryRoot
    if ($null -eq (Get-Command git -ErrorAction SilentlyContinue)) { throw 'Git is required.' }
    $plan = Get-WorkflowChangedCheckPlan -RepositoryRoot $root -BaseRef $BaseRef
    $result = Invoke-ChangedCheck -Selection $plan.selection -Root $root -Base $BaseRef
    $exitCode = if ($result.exitCode -eq 0) { $script:WorkflowExit.Success } else { $script:WorkflowExit.Degraded }
    $safeOutput = Protect-WorkflowText -Text $result.output
    $report = @{ status = if ($exitCode -eq 0) { 'success' } else { 'degraded' }; exitCode = $exitCode; data = [ordered]@{ repositoryRoot = $root; baseRef = $BaseRef; selection = $plan.selection; reason = $plan.reason; paths = $plan.paths; untrackedPaths = $plan.untracked; checkExitCode = $result.exitCode; output = $safeOutput.Substring(0, [Math]::Min(16384, $safeOutput.Length)) } }
}
catch {
    $missing = $_.Exception.Message -eq 'Git is required.'
    $report = @{ status = if ($missing) { 'missing-prerequisite' } else { 'invalid-input' }; exitCode = if ($missing) { $script:WorkflowExit.MissingPrerequisite } else { $script:WorkflowExit.InvalidInput }; data = [ordered]@{ message = $_.Exception.Message } }
}
Write-WorkflowReport -Format $Format -Title 'check-changed' -Report $report
exit $report.exitCode
