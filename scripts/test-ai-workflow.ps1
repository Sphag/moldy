$ErrorActionPreference = 'Stop'

$repoRoot = Split-Path -Parent $PSScriptRoot
. (Join-Path $PSScriptRoot 'workflow-common.ps1')
$script:passed = 0
$script:failed = 0
$scratchRoot = Join-Path $repoRoot 'temp/workflow-script-tests'

function Invoke-WorkflowTest {
    param([string]$Name, [scriptblock]$Action)
    try { & $Action; Write-Host "PASS: $Name"; $script:passed++ }
    catch { Write-Host "FAIL: $Name - $($_.Exception.Message)"; $script:failed++ }
}

function Invoke-WorkflowScriptJson {
    param([string]$ScriptName, [string[]]$ScriptArguments)
    $parameters = @{}
    for ($index = 0; $index -lt $ScriptArguments.Count; $index += 2) {
        $parameters[$ScriptArguments[$index].TrimStart('-')] = $ScriptArguments[$index + 1]
    }
    $output = & (Join-Path $PSScriptRoot $ScriptName) @parameters 2>&1 | Out-String
    return [ordered]@{ exitCode = $LASTEXITCODE; report = ($output | ConvertFrom-Json) }
}

New-Item -ItemType Directory -Path $scratchRoot -Force | Out-Null
try {
    Invoke-WorkflowTest -Name 'doctor emits local readiness evidence' {
        $result = Invoke-WorkflowScriptJson -ScriptName 'doctor.ps1' -ScriptArguments @('-Format', 'Json', '-RepositoryRoot', $repoRoot)
        if ($result.report.data.repositoryRoot -ne $repoRoot -or -not $result.report.data.policyValid) { throw 'Doctor did not report repository and policy evidence.' }
    }
    Invoke-WorkflowTest -Name 'normal context JSON output' {
        $result = Invoke-WorkflowScriptJson -ScriptName 'context.ps1' -ScriptArguments @('-Path', 'scripts/README.md', '-Format', 'Json', '-RepositoryRoot', $repoRoot)
        if ($result.exitCode -ne 0 -or $result.report.data.files.Count -ne 1 -or $result.report.data.files[0].path -ne 'scripts\README.md') { throw 'Context did not emit one tracked file.' }
    }
    Invoke-WorkflowTest -Name 'outside repository path is rejected' {
        $result = Invoke-WorkflowScriptJson -ScriptName 'context.ps1' -ScriptArguments @('-Path', '../outside.txt', '-Format', 'Json', '-RepositoryRoot', $repoRoot)
        if ($result.exitCode -ne $script:WorkflowExit.InvalidInput) { throw 'Outside path was accepted.' }
    }
    Invoke-WorkflowTest -Name 'generated path is rejected' {
        $result = Invoke-WorkflowScriptJson -ScriptName 'context.ps1' -ScriptArguments @('-Path', '.git/HEAD', '-Format', 'Json', '-RepositoryRoot', $repoRoot)
        if ($result.exitCode -ne $script:WorkflowExit.InvalidInput) { throw 'Generated path was accepted.' }
    }
    Invoke-WorkflowTest -Name 'ignored path is rejected' {
        [System.IO.File]::WriteAllText((Join-Path $scratchRoot 'ignored.txt'), 'ignored', [System.Text.UTF8Encoding]::new($false))
        $result = Invoke-WorkflowScriptJson -ScriptName 'context.ps1' -ScriptArguments @('-Path', 'temp/workflow-script-tests/ignored.txt', '-Format', 'Json', '-RepositoryRoot', $repoRoot)
        if ($result.exitCode -ne $script:WorkflowExit.InvalidInput) { throw 'Ignored path was accepted.' }
    }
    Invoke-WorkflowTest -Name 'unsupported encoding is rejected by the reader' {
        $utf16Path = Join-Path $scratchRoot 'utf16.txt'
        [System.IO.File]::WriteAllText($utf16Path, 'text', [System.Text.Encoding]::Unicode)
        try { $null = Get-WorkflowTextFile -Path $utf16Path; throw 'UTF-16 input was accepted.' }
        catch { if ($_.Exception.Message -notlike '*unsupported UTF-16*') { throw } }
    }
    Invoke-WorkflowTest -Name 'redaction and bounded content constants' {
        $redacted = Protect-WorkflowText -Text 'token=abc123 Authorization: Bearer secret-value'
        if ($redacted -match 'abc123|secret-value' -or $redacted -notmatch '<redacted>') { throw 'Secret text was not redacted.' }
        if (64KB -ne 65536 -or [System.Text.Encoding]::UTF8.GetByteCount(('x' * 65537)) -le 64KB) { throw 'Content limit is not bounded at 64 KiB.' }
    }
    Invoke-WorkflowTest -Name 'changed check selects full gate without a base reference' {
        if ((Get-WorkflowChangedCheckPlan -RepositoryRoot $repoRoot -BaseRef '').selection -ne 'full') { throw 'Missing base did not select the full gate.' }
    }
    Invoke-WorkflowTest -Name 'malformed base reference is rejected' {
        $result = Invoke-WorkflowScriptJson -ScriptName 'check-changed.ps1' -ScriptArguments @('-BaseRef', 'definitely-not-a-ref', '-Format', 'Json', '-RepositoryRoot', $repoRoot)
        if ($result.exitCode -ne $script:WorkflowExit.InvalidInput) { throw 'Malformed base reference was accepted.' }
    }
    Invoke-WorkflowTest -Name 'unsafe diagnostic output is rejected' {
        $result = Invoke-WorkflowScriptJson -ScriptName 'collect-diagnostics.ps1' -ScriptArguments @('-OutputDirectory', 'docs/diagnostics', '-Format', 'Json', '-RepositoryRoot', $repoRoot)
        if ($result.exitCode -ne $script:WorkflowExit.InvalidInput) { throw 'Non-ignored diagnostic output was accepted.' }
    }
    Invoke-WorkflowTest -Name 'partial diagnostics records collection failures' {
        $result = Invoke-WorkflowScriptJson -ScriptName 'collect-diagnostics.ps1' -ScriptArguments @('-OutputDirectory', 'temp/workflow-script-tests/diagnostics', '-LogPath', 'missing.log', '-Format', 'Json', '-RepositoryRoot', $repoRoot)
        $manifestPath = Join-Path $repoRoot 'temp/workflow-script-tests/diagnostics/manifest.json'
        if ($result.exitCode -ne $script:WorkflowExit.Degraded -or -not (Test-Path -LiteralPath $manifestPath)) { throw 'Partial diagnostics did not preserve a manifest.' }
        if ((Get-Content -LiteralPath $manifestPath -Raw | ConvertFrom-Json).failures.Count -eq 0) { throw 'Partial diagnostics did not record its failure.' }
    }
    Invoke-WorkflowTest -Name 'missing command prerequisite has a stable exit class' {
        $health = Get-WorkflowCommandHealth -Name 'moldy-command-that-does-not-exist'
        if ($health.available -or $script:WorkflowExit.MissingPrerequisite -ne 3) { throw 'Missing prerequisite classification changed.' }
    }
}
finally { Remove-Item -LiteralPath $scratchRoot -Recurse -Force -ErrorAction SilentlyContinue }

Write-Host "AI workflow script tests: $script:passed passed, $script:failed failed."
if ($script:failed -ne 0) { exit 1 }
exit 0
