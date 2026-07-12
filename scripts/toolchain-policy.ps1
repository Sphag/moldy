function Import-ToolchainPolicy {
    [CmdletBinding()]
    param(
        [Parameter(Mandatory = $true)]
        [string]$Path
    )

    $manifestPath = [System.IO.Path]::GetFullPath($Path)

    try {
        $policy = Import-PowerShellDataFile -LiteralPath $manifestPath
    }
    catch {
        throw "Toolchain policy manifest '$manifestPath' could not be loaded: $($_.Exception.Message)"
    }

    if (-not $policy.ContainsKey("SchemaVersion") -or $policy.SchemaVersion -ne 1) {
        throw "Toolchain policy manifest '$manifestPath' violates schema rule: SchemaVersion must be 1."
    }

    if (-not $policy.ContainsKey("Tools") -or $policy.Tools -isnot [System.Collections.IDictionary]) {
        throw "Toolchain policy manifest '$manifestPath' violates schema rule: Tools must be a table."
    }

    $requiredTools = @("clang-format", "clang-tidy", "cppcheck", "cmake", "ninja", "msvc")
    foreach ($toolName in $requiredTools) {
        if (-not $policy.Tools.Contains($toolName)) {
            throw "Toolchain policy manifest '$manifestPath' violates required tool rule for '$toolName': the tool is missing."
        }
    }

    foreach ($toolName in $policy.Tools.Keys) {
        $tool = $policy.Tools[$toolName]
        if ($tool -isnot [System.Collections.IDictionary]) {
            throw "Toolchain policy manifest '$manifestPath' violates entry rule for tool '$toolName': the entry must be a table."
        }

        $allowedFields = @("Constraint", "Version", "MinVersion", "MaxVersion")
        foreach ($field in $tool.Keys) {
            if ($field -notin $allowedFields) {
                throw "Toolchain policy manifest '$manifestPath' violates field rule for tool '$toolName': field '$field' is not allowed."
            }
        }

        if (-not $tool.Contains("Constraint") -or $tool.Constraint -notin @("Exact", "Range")) {
            throw "Toolchain policy manifest '$manifestPath' violates constraint rule for tool '$toolName': Constraint must be Exact or Range."
        }

        if ($tool.Constraint -eq "Exact") {
            if (-not $tool.Contains("Version")) {
                throw "Toolchain policy manifest '$manifestPath' violates exact constraint rule for tool '$toolName': Version is required."
            }
            if ($tool.Contains("MinVersion") -or $tool.Contains("MaxVersion")) {
                throw "Toolchain policy manifest '$manifestPath' violates exact constraint rule for tool '$toolName': MinVersion and MaxVersion are not allowed."
            }

            $parsedVersion = $null
            if (-not [version]::TryParse([string]$tool.Version, [ref]$parsedVersion)) {
                throw "Toolchain policy manifest '$manifestPath' violates version rule for tool '$toolName': Version '$($tool.Version)' is not parseable."
            }
        }
        else {
            if ($tool.Contains("Version")) {
                throw "Toolchain policy manifest '$manifestPath' violates range constraint rule for tool '$toolName': Version is not allowed."
            }
            if (-not $tool.Contains("MinVersion") -or -not $tool.Contains("MaxVersion")) {
                throw "Toolchain policy manifest '$manifestPath' violates range constraint rule for tool '$toolName': MinVersion and MaxVersion are required."
            }

            $minimum = $null
            $maximum = $null
            if (-not [version]::TryParse([string]$tool.MinVersion, [ref]$minimum)) {
                throw "Toolchain policy manifest '$manifestPath' violates version rule for tool '$toolName': MinVersion '$($tool.MinVersion)' is not parseable."
            }
            if (-not [version]::TryParse([string]$tool.MaxVersion, [ref]$maximum)) {
                throw "Toolchain policy manifest '$manifestPath' violates version rule for tool '$toolName': MaxVersion '$($tool.MaxVersion)' is not parseable."
            }
            if ($minimum -gt $maximum) {
                throw "Toolchain policy manifest '$manifestPath' violates range order rule for tool '$toolName': MinVersion must be less than or equal to MaxVersion."
            }
        }
    }

    return $policy
}
