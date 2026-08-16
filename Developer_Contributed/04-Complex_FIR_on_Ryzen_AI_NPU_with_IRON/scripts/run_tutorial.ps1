# Purpose: One-command runner for the Developer Contributed tutorial
#          "Bit-Exact Complex FIR on the AMD Ryzen AI NPU using IRON / MLIR-AIE".
# Target operating system: Windows 11 Pro 25H2, PowerShell 5.1 or 7+.
# Prerequisite: MLIR-AIE ironenv must be activated in the current shell,
#               or IRONENV_ROOT must point at the ironenv root
#               (see doc/SETUP.md).
# Test harness: The M19 test is a self-contained main()-driven script that
#               prints "PASS!" on success. Any per-check reference failure or
#               silicon comparison mismatch raises a Python exception, which
#               propagates as a non-zero exit status. It is not a pytest suite.
# Exit code: 0 on test pass, non-zero on any failure.

$ErrorActionPreference = "Stop"

$here = Split-Path -Parent $MyInvocation.MyCommand.Path
$root = Split-Path -Parent $here

Write-Host "Tutorial root: $root"
Write-Host "Test file:     $root\src\test_fir_complex_m19.py"

Push-Location $root
try {
    # Sanity check: aie and pyxrt must be importable.
    python -c "import aie, pyxrt; print('aie:', aie.__file__); print('pyxrt:', pyxrt.__file__)"

    # Run the silicon test directly (main()-driven, not pytest).
    python .\src\test_fir_complex_m19.py
} finally {
    Pop-Location
}
