# Setup: Ryzen AI NPU and IRON on native Windows

This tutorial was validated on one AMD Phoenix XDNA1 system. Treat the versions below as the tested baseline, not as a promise of compatibility with every Ryzen-branded laptop.

## Tested baseline

| Component | Tested value |
|---|---|
| System | ASUS TUF Gaming A15 FA507XI |
| Processor | AMD Ryzen 9 7940HS |
| NPU | Phoenix XDNA1 / AIE2, reported by XRT as `NPU Phoenix` |
| Operating system | Windows 11 Pro, build 26200.9168 |
| Python | 3.13.15 |
| XRT runtime | 2.21.0 |
| XRT SDK package | 2.21.75 |
| NPU driver | 32.0.20102.3930 |
| NPU firmware | 1.5.5.391 |
| MLIR-AIE Python package | 1.4.1 |
| MLIR-AIE checkout used for the recorded run | `3ca0193cea9e2c39ec670a65f93e1dd43c969f22` |
| Reproducible reader checkout | tag `v1.4.1`, revision `601fc859532f2539bebb33ac89139584c76ae8a2` |
| LLVM-AIE (Peano) | `21.0.0.2026080301+c9c5ecb7` |
| NumPy | 2.5.2 |
| `ml_dtypes` | 0.6.0 |

Newer compatible drivers or firmware may work, but they were not part of this recorded run. Systems without an NPU visible to XRT cannot run the silicon stage.

## Prerequisites

Follow the [MLIR-AIE v1.4.1 native Windows guide](https://xilinx.github.io/mlir-aie/1.4.1/buildHostWinNative/) to install:

- Visual Studio 2022 with the C++ desktop workload;
- 64-bit Python 3.13;
- a compatible AMD XDNA NPU driver;
- the XRT SDK, normally under `C:\Xilinx\XRT`; and
- a checkout-local IRON environment created by `utils\iron_setup.py`.

The official guide uses either an x64 Native Tools command prompt or a Developer PowerShell for Visual Studio. A plain PowerShell session that lacks the Visual Studio compiler environment is not sufficient.

## Create the pinned IRON environment

Open Developer PowerShell for Visual Studio. Create a recursive checkout at the
release tag and let that checkout's setup script install the release-paired
`mlir_aie` wheel and its pinned requirements:

```powershell
cd C:\dev
git clone --recurse-submodules https://github.com/Xilinx/mlir-aie.git mlir-aie-v1.4.1
cd .\mlir-aie-v1.4.1
git checkout v1.4.1
git submodule update --init --recursive
python .\utils\iron_setup.py
. .\iron_env.ps1
```

The leading dot on the activation command is required. In each later
Developer PowerShell session, reactivate the existing environment with:

```powershell
cd C:\dev\mlir-aie-v1.4.1
. .\iron_env.ps1
```

Confirm that the release tag resolves to the reviewed source revision:

```powershell
git rev-parse HEAD
```

Expected for the recorded baseline:

```text
601fc859532f2539bebb33ac89139584c76ae8a2
```

Do not substitute an untagged checkout: `iron_setup.py` uses a rolling
development wheel when no release tag names `HEAD`. At `v1.4.1`, it selects
`mlir_aie==1.4.1`, installs the repository's runtime requirements, and installs
the Peano version pinned by `utils\peano-requirements.txt`.

The recorded silicon run used the later untagged source revision shown in the
tested-baseline table with the same `mlir-aie` 1.4.1 and pinned Peano packages.
That exact local environment is recorded for provenance, but the instructions
above deliberately use the immutable v1.4.1 release path instead of asking
readers to resolve a mutable rolling-wheel channel.

## Verify hardware and Python dependencies

Confirm that XRT sees the NPU:

```powershell
& "C:\Windows\System32\AMD\xrt-smi.exe" examine
```

The device list must contain a compatible NPU. Driver and firmware values do not need to equal the tested baseline exactly unless reproducing that baseline.

Confirm the required distributions and imports:

```powershell
python -m pip show mlir-aie llvm-aie numpy ml-dtypes
python -c "import numpy, ml_dtypes, aie, pyxrt; print(numpy.__version__); print(ml_dtypes.__version__); print(aie.__file__); print(pyxrt.__file__)"
```

Do not separately upgrade `numpy`, `ml_dtypes`, `mlir_aie`, or `llvm_aie`
after setup. The tutorial runner rejects a wrong MLIR-AIE or Peano package and
checks the NumPy and `ml_dtypes` constraints recorded by the v1.4.1 checkout.

## Toolchain readiness gate

Before running this tutorial, execute the SAXPY example recommended by the official Windows guide:

```powershell
cd C:\dev\mlir-aie-v1.4.1\programming_examples\getting_started\01_SAXPY
python saxpy.py
```

Proceed only after SAXPY completes with `PASS!`. This verifies the compiler, XRT runtime, driver, and NPU path independently of the complex FIR example.

## Run the tutorial

From the tutorial directory:

```powershell
.\scripts\run_tutorial.ps1
```

The script disables Python bytecode generation, checks imports, prints the active package information, and propagates non-zero Python exit codes through Windows PowerShell 5.1 and PowerShell 7.
