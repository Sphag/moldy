Set-StrictMode -Version Latest

$script:WorkflowExit = @{ Success = 0; Degraded = 1; InvalidInput = 2; MissingPrerequisite = 3 }

function Resolve-WorkflowRepositoryRoot {
    param([string]$RepositoryRoot)

    $candidate = if ([string]::IsNullOrWhiteSpace($RepositoryRoot)) {
        Split-Path -Parent $PSScriptRoot
    }
    else {
        $RepositoryRoot
    }

    try {
        $root = (Resolve-Path -LiteralPath $candidate -ErrorAction Stop).Path
    }
    catch {
        throw "Repository root '$candidate' does not exist."
    }

    if (-not (Test-Path -LiteralPath (Join-Path $root '.git'))) {
        throw "Repository root '$root' does not contain .git."
    }
    return $root.TrimEnd([System.IO.Path]::DirectorySeparatorChar, [System.IO.Path]::AltDirectorySeparatorChar)
}

function Test-WorkflowPathInsideRoot {
    param([string]$RepositoryRoot, [string]$Path)

    $root = $RepositoryRoot.TrimEnd([System.IO.Path]::DirectorySeparatorChar, [System.IO.Path]::AltDirectorySeparatorChar)
    $fullPath = [System.IO.Path]::GetFullPath($Path)
    return $fullPath.Equals($root, [System.StringComparison]::OrdinalIgnoreCase) -or
        $fullPath.StartsWith($root + [System.IO.Path]::DirectorySeparatorChar, [System.StringComparison]::OrdinalIgnoreCase)
}

function Resolve-WorkflowRepositoryPath {
    param(
        [string]$RepositoryRoot,
        [string]$Path,
        [switch]$AllowMissing
    )

    if ([string]::IsNullOrWhiteSpace($Path) -or [System.IO.Path]::IsPathRooted($Path)) {
        throw "Path '$Path' must be a non-empty repository-relative path."
    }
    $fullPath = [System.IO.Path]::GetFullPath((Join-Path $RepositoryRoot $Path))
    if (-not (Test-WorkflowPathInsideRoot -RepositoryRoot $RepositoryRoot -Path $fullPath)) {
        throw "Path '$Path' is outside the repository root."
    }
    if (-not $AllowMissing -and -not (Test-Path -LiteralPath $fullPath -PathType Leaf)) {
        throw "Path '$Path' is not a file."
    }
    return $fullPath
}

function Get-WorkflowRelativePath {
    param([string]$RepositoryRoot, [string]$Path)

    $uriRoot = [System.Uri]($RepositoryRoot.TrimEnd([System.IO.Path]::DirectorySeparatorChar, [System.IO.Path]::AltDirectorySeparatorChar) + [System.IO.Path]::DirectorySeparatorChar)
    $relative = $uriRoot.MakeRelativeUri([System.Uri]$Path).ToString().Replace('/', '\')
    return [System.Uri]::UnescapeDataString($relative)
}

function Test-WorkflowGeneratedPath {
    param([string]$RepositoryRoot, [string]$Path)

    $relative = Get-WorkflowRelativePath -RepositoryRoot $RepositoryRoot -Path $Path
    $segments = $relative -split '[\\/]'
    return @($segments | Where-Object { $_ -eq '.git' -or $_ -eq 'out' -or $_ -eq 'CMakeFiles' -or $_ -like 'build*' }).Count -gt 0
}

function Test-WorkflowIgnoredPath {
    param([string]$RepositoryRoot, [string]$RelativePath)

    & git -C $RepositoryRoot check-ignore -q -- $RelativePath
    if ($LASTEXITCODE -eq 0) { return $true }
    if ($LASTEXITCODE -eq 1) { return $false }
    throw "Git could not evaluate ignore rules for '$RelativePath'."
}

function Test-WorkflowTrackedPath {
    param([string]$RepositoryRoot, [string]$RelativePath)

    & git -C $RepositoryRoot ls-files --error-unmatch -- $RelativePath *> $null
    if ($LASTEXITCODE -eq 0) { return $true }
    if ($LASTEXITCODE -eq 1) { return $false }
    throw "Git could not evaluate tracked state for '$RelativePath'."
}

function Get-WorkflowTextFile {
    param([string]$Path)

    $bytes = [System.IO.File]::ReadAllBytes($Path)
    if ($bytes.Length -ge 2 -and (($bytes[0] -eq 0xFF -and $bytes[1] -eq 0xFE) -or ($bytes[0] -eq 0xFE -and $bytes[1] -eq 0xFF))) {
        throw "Path '$Path' uses an unsupported UTF-16 encoding."
    }
    if ($bytes.Length -ge 4 -and $bytes[0] -eq 0x00 -and $bytes[1] -eq 0x00 -and $bytes[2] -eq 0xFE -and $bytes[3] -eq 0xFF) {
        throw "Path '$Path' uses an unsupported UTF-32 encoding."
    }
    $encoding = [System.Text.UTF8Encoding]::new($false, $true)
    try { $text = $encoding.GetString($bytes) }
    catch { throw "Path '$Path' does not contain valid UTF-8 text." }
    return @{ Bytes = $bytes; Text = $text }
}

function Protect-WorkflowText {
    param([AllowEmptyString()][string]$Text)

    $redacted = $Text
    $redacted = [regex]::Replace($redacted, '(?im)\b(authorization)\s*[:=]\s*(?:bearer\s+)?[^\s"]+', '$1=<redacted>')
    $redacted = [regex]::Replace($redacted, '(?im)\b(api[_-]?key|access[_-]?token|token|password|secret)\s*[:=]\s*[^\s"]+', '$1=<redacted>')
    $redacted = [regex]::Replace($redacted, '(?i)\bBearer\s+[A-Za-z0-9._~+/-]+=*', 'Bearer <redacted>')
    return $redacted
}

function Get-WorkflowSha256 {
    param([byte[]]$Bytes)
    $hash = [System.Security.Cryptography.SHA256]::Create()
    try { return ([System.BitConverter]::ToString($hash.ComputeHash($Bytes))).Replace('-', '').ToLowerInvariant() }
    finally { $hash.Dispose() }
}

function Write-WorkflowReport {
    param(
        [ValidateSet('Text', 'Json')][string]$Format,
        [string]$Title,
        [hashtable]$Report
    )

    if ($Format -eq 'Json') {
        [Console]::OutputEncoding = [System.Text.UTF8Encoding]::new($false)
        [ordered]@{ schemaVersion = 1; title = $Title; status = $Report.status; exitCode = $Report.exitCode; data = $Report.data } |
            ConvertTo-Json -Depth 8 -Compress
        return
    }

    Write-Host "${Title}: $($Report.status) (exit $($Report.exitCode))"
    foreach ($key in $Report.data.Keys) {
        $value = $Report.data[$key]
        if ($value -is [System.Collections.IEnumerable] -and $value -isnot [string]) {
            Write-Host "${key}: $($value.Count) item(s)"
        }
        else { Write-Host "${key}: $value" }
    }
}

function Get-WorkflowCommandHealth {
    param([string]$Name)

    $command = Get-Command $Name -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($null -eq $command) { return [ordered]@{ name = $Name; available = $false; healthy = $false; path = $null } }
    & $command.Source --version *> $null
    $healthy = $LASTEXITCODE -eq 0
    return [ordered]@{ name = $Name; available = $true; healthy = $healthy; path = $command.Source }
}

function Get-WorkflowMsvcHealth {
    $command = Get-Command cl -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($null -eq $command) { return [ordered]@{ name = 'msvc'; available = $false; healthy = $false; path = $null } }
    return [ordered]@{ name = 'msvc'; available = $true; healthy = $true; path = $command.Source }
}

function Get-WorkflowChangedCheckPlan {
    param([string]$RepositoryRoot, [string]$BaseRef)

    if ([string]::IsNullOrWhiteSpace($BaseRef)) {
        return [ordered]@{ selection = 'full'; reason = 'No base reference was supplied.'; paths = @() }
    }
    & git -C $RepositoryRoot rev-parse --verify --quiet "${BaseRef}^{commit}" *> $null
    if ($LASTEXITCODE -ne 0) { throw "Base reference '$BaseRef' does not resolve to a commit." }

    $changedPaths = @()
    foreach ($arguments in @(@('diff', '--name-only', "${BaseRef}...HEAD"), @('diff', '--cached', '--name-only'), @('diff', '--name-only'))) {
        $result = & git -C $RepositoryRoot @arguments
        if ($LASTEXITCODE -ne 0) { throw 'Git could not determine changed paths.' }
        $changedPaths += @($result | Where-Object { -not [string]::IsNullOrWhiteSpace($_) })
    }
    $untracked = @(& git -C $RepositoryRoot ls-files --others --exclude-standard)
    if ($LASTEXITCODE -ne 0) { throw 'Git could not determine untracked paths.' }
    $changedPaths = @($changedPaths | Sort-Object -Unique)
    if ($untracked.Count -gt 0) {
        return [ordered]@{ selection = 'full'; reason = 'Untracked paths make focused coverage unprovable.'; paths = $changedPaths; untracked = $untracked }
    }
    if ($changedPaths.Count -eq 0) {
        return [ordered]@{ selection = 'full'; reason = 'No changed paths were found.'; paths = @(); untracked = @() }
    }
    $isDocumentationOnly = @($changedPaths | Where-Object { $_ -notmatch '^(docs[\\/]|README\.md$)' }).Count -eq 0
    if ($isDocumentationOnly) {
        return [ordered]@{ selection = 'documentation'; reason = 'All changed paths are documentation.'; paths = $changedPaths; untracked = @() }
    }
    $isAgentSkillsOnly = @($changedPaths | Where-Object { $_ -notmatch '^\.agents[\\/]skills[\\/]' }).Count -eq 0
    if ($isAgentSkillsOnly) {
        return [ordered]@{ selection = 'agent-skills'; reason = 'All changed paths are foundational Agent Skills.'; paths = $changedPaths; untracked = @() }
    }
    return [ordered]@{ selection = 'full'; reason = 'Changed paths include source, build, script, or unknown scope.'; paths = $changedPaths; untracked = @() }
}
