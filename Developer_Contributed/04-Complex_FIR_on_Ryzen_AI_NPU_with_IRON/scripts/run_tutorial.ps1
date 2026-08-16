# Copyright (c) 2026 Midhat Nashar
# SPDX-License-Identifier: MIT
#
# Run this script from a Developer PowerShell for Visual Studio after
# dot-sourcing the MLIR-AIE checkout's iron_env.ps1 activation helper.

$ErrorActionPreference = "Stop"
$env:PYTHONDONTWRITEBYTECODE = "1"

$here = Split-Path -Parent $MyInvocation.MyCommand.Path
$root = Split-Path -Parent $here
$preflightFile = Join-Path $root "src\check_environment.py"
$testFile = Join-Path $root "src\test_fir_complex_m19.py"
$python = Get-Command python -ErrorAction Stop

function Invoke-PythonChecked {
    param(
        [Parameter(Mandatory = $true)]
        [string[]]$Arguments,

        [Parameter(Mandatory = $true)]
        [string]$FailureMessage
    )

    & $python.Source @Arguments
    $exitCode = $LASTEXITCODE
    if ($exitCode -ne 0) {
        throw "$FailureMessage (python exit code $exitCode)"
    }
}

Write-Host "Tutorial root: $root"
Write-Host "Python:        $($python.Source)"
Write-Host "Preflight:     $preflightFile"
Write-Host "Test file:     $testFile"

Push-Location $root
try {
    Invoke-PythonChecked `
        -Arguments @($preflightFile) `
        -FailureMessage "IRON/XRT dependency preflight failed"

    Invoke-PythonChecked `
        -Arguments @($testFile) `
        -FailureMessage "Complex FIR tutorial failed"
}
finally {
    Pop-Location
}
