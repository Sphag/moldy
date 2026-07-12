$ErrorActionPreference = "Stop"

. (Join-Path $PSScriptRoot "toolchain-policy.ps1")

$manifestPath = Join-Path (Split-Path -Parent $PSScriptRoot) "config/toolchain.psd1"
$baseManifest = Get-Content -LiteralPath $manifestPath -Raw
$testDirectory = Join-Path ([System.IO.Path]::GetTempPath()) ("moldy-toolchain-policy-" + [guid]::NewGuid().ToString("N"))
$script:passed = 0
$script:failed = 0

function Invoke-PolicyTest {
    param(
        [string]$Name,
        [string]$Manifest,
        [string]$ExpectedMessage
    )

    $testPath = Join-Path $testDirectory (($Name -replace "[^a-zA-Z0-9]", "-") + ".psd1")
    Set-Content -LiteralPath $testPath -Value $Manifest -Encoding UTF8

    $validationError = $null
    try {
        $null = Import-ToolchainPolicy -Path $testPath
    }
    catch {
        $validationError = $_
    }

    if ($ExpectedMessage) {
        if ($null -eq $validationError) {
            Write-Host "FAIL: $Name - invalid manifest was accepted."
            $script:failed++
            return
        }
        if ($validationError.Exception.Message -notlike "*$ExpectedMessage*") {
            Write-Host "FAIL: $Name - $($validationError.Exception.Message)"
            $script:failed++
            return
        }
    }
    elseif ($null -ne $validationError) {
        Write-Host "FAIL: $Name - $($validationError.Exception.Message)"
        $script:failed++
        return
    }

    Write-Host "PASS: $Name"
    $script:passed++
}

New-Item -ItemType Directory -Path $testDirectory | Out-Null

try {
    Invoke-PolicyTest -Name "valid manifest" -Manifest $baseManifest
    Invoke-PolicyTest -Name "missing tool" -Manifest ($baseManifest -replace "(?ms)        msvc = @\{.*?        \}\r?\n", "") -ExpectedMessage "'msvc': the tool is missing"
    Invoke-PolicyTest -Name "unknown constraint" -Manifest ([regex]::Replace($baseManifest, "Constraint = 'Exact'", "Constraint = 'Compatible'", 1)) -ExpectedMessage "Constraint must be Exact or Range"
    Invoke-PolicyTest -Name "malformed version" -Manifest ([regex]::Replace($baseManifest, "Version = '19.1.7'", "Version = 'not-a-version'", 1)) -ExpectedMessage "is not parseable"
    Invoke-PolicyTest -Name "exact range field mixing" -Manifest ([regex]::Replace($baseManifest, "Version = '19.1.7'", "Version = '19.1.7'`n            MinVersion = '19.0.0'", 1)) -ExpectedMessage "MinVersion and MaxVersion are not allowed"
    Invoke-PolicyTest -Name "range exact field mixing" -Manifest ([regex]::Replace($baseManifest, "MinVersion = '4.3.4'", "Version = '4.3.4'`n            MinVersion = '4.3.4'", 1)) -ExpectedMessage "Version is not allowed"
    Invoke-PolicyTest -Name "missing bound" -Manifest ([regex]::Replace($baseManifest, "            MaxVersion = '4.4.0'\r?\n", "", 1)) -ExpectedMessage "MinVersion and MaxVersion are required"
    Invoke-PolicyTest -Name "reversed range" -Manifest ([regex]::Replace($baseManifest, "MinVersion = '4.3.4'", "MinVersion = '4.5.0'", 1)) -ExpectedMessage "MinVersion must be less than or equal to MaxVersion"
}
finally {
    Remove-Item -LiteralPath $testDirectory -Recurse -Force -ErrorAction SilentlyContinue
}

Write-Host "Toolchain policy tests: $script:passed passed, $script:failed failed."
if ($script:failed -ne 0) {
    exit 1
}

exit 0
