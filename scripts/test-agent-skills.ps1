param(
    [string]$SkillsDirectory = (Join-Path (Split-Path -Parent $PSScriptRoot) ".agents/skills")
)

$ErrorActionPreference = "Stop"

$expectedSkills = @(
    "spec",
    "plan-eng-review",
    "investigate",
    "review",
    "ship",
    "ci-triage",
    "dependency-review",
    "checkpoint",
    "health",
    "document-release"
)

$requiredSections = @(
    "## Trigger",
    "## Inputs",
    "## Outputs",
    "## Mutations and approvals",
    "## Prerequisites and failure behavior",
    "## Verification"
)

function Test-SkillContent {
    param(
        [string]$Content,
        [string]$DirectoryName,
        [string]$Label
    )

    $documentMatch = [regex]::Match($Content, "(?s)\A---\r?\n(?<frontmatter>.*?)\r?\n---\r?\n(?<body>.+)\z")
    if (-not $documentMatch.Success) {
        throw "${Label}: SKILL.md must contain YAML frontmatter followed by Markdown content."
    }

    $body = $documentMatch.Groups["body"].Value
    $fields = @{}
    foreach ($line in $documentMatch.Groups["frontmatter"].Value -split "\r?\n") {
        $fieldMatch = [regex]::Match($line, "^(?<key>[a-z-]+): (?<value>.+)$")
        if (-not $fieldMatch.Success) {
            throw "${Label}: frontmatter line '$line' must be a simple key-value pair."
        }

        $key = $fieldMatch.Groups["key"].Value
        if ($key -notin @("name", "description")) {
            throw "${Label}: frontmatter field '$key' is not supported by this dependency-free validator."
        }
        if ($fields.ContainsKey($key)) {
            throw "${Label}: frontmatter field '$key' must not be repeated."
        }
        $fields[$key] = $fieldMatch.Groups["value"].Value.Trim()
    }

    foreach ($field in @("name", "description")) {
        if (-not $fields.ContainsKey($field) -or [string]::IsNullOrWhiteSpace($fields[$field])) {
            throw "${Label}: required frontmatter field '$field' is missing."
        }
    }

    $name = $fields.name
    if ($name.Length -gt 64 -or $name -notmatch "^[a-z0-9]+(?:-[a-z0-9]+)*$") {
        throw "${Label}: name '$name' must use 1-64 lowercase letters, numbers, and single hyphens."
    }
    if ($name -ne $DirectoryName) {
        throw "${Label}: name '$name' must match its directory '$DirectoryName'."
    }
    if ($fields.description.Length -gt 1024) {
        throw "${Label}: description must not exceed 1024 characters."
    }

    foreach ($section in $requiredSections) {
        if ($body -notmatch [regex]::Escape($section)) {
            throw "${Label}: missing required section '$section'."
        }
    }
    if ($body -notmatch "(?im)^\s*(?:-\s+)?\*\*Failure:\*\*") {
        throw "${Label}: missing failure behavior reporting."
    }
    if ($body -notmatch "If approval is denied, stop\.") {
        throw "${Label}: missing denied-approval stop condition."
    }
}

function Invoke-SkillSmokeTest {
    param(
        [string]$Name,
        [string]$Content,
        [string]$ExpectedMessage = ""
    )

    $validationError = $null
    try {
        Test-SkillContent -Content $Content -DirectoryName "spec" -Label "fixture '$Name'"
    }
    catch {
        $validationError = $_.Exception.Message
    }

    if ($ExpectedMessage) {
        if ($null -eq $validationError -or $validationError -notlike "*$ExpectedMessage*") {
            throw "Smoke test '$Name' did not report '$ExpectedMessage'. Actual: $validationError"
        }
    }
    elseif ($null -ne $validationError) {
        throw "Smoke test '$Name' failed: $validationError"
    }

    Write-Host "PASS: $Name"
}

function New-ValidFixture {
    return @'
---
name: spec
description: Produces a decision-ready specification when a new contract needs explicit scope.
---
## Trigger
Use when a new contract needs a decision.

## Inputs
- Goal and constraints.

## Outputs
- Scope and acceptance criteria.

## Mutations and approvals
- Read-only by default.
- If approval is denied, stop.

## Prerequisites and failure behavior
**Failure:** Report missing context and the decision needed.

## Verification
- Trace criteria to the stated goal.
'@
}

try {
    if (-not (Test-Path -LiteralPath $SkillsDirectory -PathType Container)) {
        throw "Skills directory '$SkillsDirectory' is missing."
    }

    $skillDirectories = @(Get-ChildItem -LiteralPath $SkillsDirectory -Directory | Sort-Object Name)
    $actualSkills = @($skillDirectories.Name)
    $unexpectedSkills = @($actualSkills | Where-Object { $_ -notin $expectedSkills })
    $missingSkills = @($expectedSkills | Where-Object { $_ -notin $actualSkills })
    if ($unexpectedSkills.Count -ne 0 -or $missingSkills.Count -ne 0) {
        throw "Skill directory set differs from the approved foundational set. Missing: $($missingSkills -join ', '). Unexpected: $($unexpectedSkills -join ', ')."
    }

    foreach ($skillDirectory in $skillDirectories) {
        $skillPath = Join-Path $skillDirectory.FullName "SKILL.md"
        if (-not (Test-Path -LiteralPath $skillPath -PathType Leaf)) {
            throw "Skill '$($skillDirectory.Name)' is missing SKILL.md."
        }
        Test-SkillContent -Content (Get-Content -LiteralPath $skillPath -Raw) -DirectoryName $skillDirectory.Name -Label $skillPath
        Write-Host "PASS: $($skillDirectory.Name)"
    }

    $validFixture = New-ValidFixture
    Invoke-SkillSmokeTest -Name "valid skill" -Content $validFixture
    Invoke-SkillSmokeTest -Name "missing prerequisite section" -Content ($validFixture -replace "## Prerequisites and failure behavior", "## Environment") -ExpectedMessage "missing required section '## Prerequisites and failure behavior'"
    Invoke-SkillSmokeTest -Name "denied approval" -Content ($validFixture -replace "If approval is denied, stop\.", "Approval is required.") -ExpectedMessage "missing denied-approval stop condition"
    Invoke-SkillSmokeTest -Name "failure reporting" -Content ($validFixture -replace "\*\*Failure:\*\*", "**Status:**") -ExpectedMessage "missing failure behavior reporting"
}
catch {
    Write-Host "FAIL: $($_.Exception.Message)"
    exit 1
}

Write-Host "Agent Skills validation passed for $($expectedSkills.Count) foundational skills."
exit 0
