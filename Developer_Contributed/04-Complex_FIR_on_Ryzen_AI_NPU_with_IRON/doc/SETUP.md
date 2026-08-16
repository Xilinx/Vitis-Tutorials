<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ Developer Contributed Tutorials</h1>
    </td>
  </tr>
</table>

# Setup — Ryzen AI NPU + IRON / MLIR-AIE toolchain

This tutorial targets the **AMD Ryzen 7040 / 8040-series laptop with the Phoenix NPU (XDNA1, AIE2 tile array)** running Windows-native. The reference configuration below is the exact hardware and toolchain used to validate the tutorial. Other Ryzen 7040 / 8040-class laptops with the Phoenix NPU should work with the same driver, firmware, XRT, and IRON versions, but this specific pair is the one confirmed silicon-passing.

## Verified reference platform

### Host

| Component | Verified value |
|---|---|
| Operating system | Windows 11 Pro, build 26200.9168 |
| System | ASUS TUF Gaming A15 FA507XI |
| Processor | [AMD Ryzen 9 7940HS](https://www.amd.com/en/products/processors/laptop/ryzen/7000-series/amd-ryzen-9-7940hs.html) with Radeon 780M Graphics |
| NPU | AMD Phoenix NPU ([XDNA1 / AIE2](https://docs.kernel.org/accel/amdxdna/amdnpu.html)) |
| Git | 2.48.1.windows.1 |
| Python | 3.13.15 |
| CMake | 4.3.2 |

### NPU runtime

| Component | Verified value |
|---|---|
| XRT | 2.21.0 ([SDK zip 2.21.75](https://github.com/Xilinx/XRT/releases/tag/2.21.75); [IRON Windows guide](https://xilinx.github.io/mlir-aie/1.4.1/buildHostWinNative/)) |
| XRT SDK root | `C:\Xilinx\XRT` |
| NPU driver | 32.0.20102.3930 |
| NPU firmware | 1.5.5.391 |
| XRT device string | `NPU Phoenix` |

### Compiler and Python environment

| Component | Verified value |
|---|---|
| MLIR-AIE | v1.4.1 + 13 commits (pin `3ca0193`) |
| LLVM-AIE (Peano) | `21.0.0.2026080301+c9c5ecb7` |
| MLIR-AIE repository | `https://github.com/Xilinx/mlir-aie.git` |
| MLIR-AIE commit | `3ca0193cea9e2c39ec670a65f93e1dd43c969f22` |
| Upstream release base | [v1.4.1](https://github.com/Xilinx/mlir-aie/releases/tag/v1.4.1) |
| Python venv name | `ironenv` (created by the MLIR-AIE Windows guide) |

## Installation summary

Follow the [MLIR-AIE Windows-native build guide](https://xilinx.github.io/mlir-aie/1.4.1/buildHostWinNative/) up to and including the point where `python -c "import aie"` succeeds inside the `ironenv` virtual environment. That guide installs XRT 2.21.0, builds MLIR-AIE at the pinned commit, and produces the working `ironenv`.

Then verify the NPU driver and firmware match the table above:

```powershell
& "C:\Windows\System32\AMD\xrt-smi.exe" examine
```

Expected output must include `NPU Phoenix` under devices, driver `32.0.20102.3930`, and firmware `1.5.5.391`. If your driver or firmware are newer, the tutorial should still pass, but this is the exact combination that has been silicon-validated.

Finally, from inside the `ironenv`:

```powershell
python -c "import aie; print(aie.__file__)"
python -c "import pyxrt; print(pyxrt.__file__)"
```

Both must succeed. This is the toolchain readiness gate before running the tutorial.

## Test dependency

The tutorial's Python test uses `numpy` and `ml_dtypes` (for the `bfloat16` dtype). Both wheels are already installed inside the standard IRON `ironenv` virtual environment used for MLIR-AIE examples, so no additional `pip install` step is needed. Verify with `python -c "import numpy, ml_dtypes; print(numpy.__version__, ml_dtypes.__version__)"` inside `ironenv`; if either import fails, install with `pip install numpy ml_dtypes`. The test is a self-contained `main()`-driven script and does not require `pytest`.

<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2026 Midhat Nashar. Licensed under the MIT License.</sub></p>
