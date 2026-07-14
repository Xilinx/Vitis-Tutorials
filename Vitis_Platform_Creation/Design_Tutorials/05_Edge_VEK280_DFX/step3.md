<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ Platform Creation Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# Step 3: Test the Platform

You will validate the generated `vek280_custom_dfx.xpfm` platform with two applications:

1. **Vector addition (`vadd`)** — a pure PL kernel pulled from `$XILINX_VITIS/samples/vadd/`. This exercises the PL acceleration path through the DFX boundary.
2. **AIE-ML matrix multiply (`aieml_system`)** — built from the **locally shipped** sources in [`ref_files/step3_validate/aie_src/`](./ref_files/step3_validate/aie_src/). This exercises the AIE-ML array, the PL ↔ AIE-ML stream connectivity, and the dynamic-region reconfiguration path.

> **Why local AIE-ML sources?** The Vitis 2026.1 install layout for AIE-ML samples is not stable across daily builds. Shipping a known-good AIE-ML graph inside the tutorial makes the validation deterministic.

## Test 1: Read Platform Info

With the Vitis environment set up, `platforminfo` reports the XPFM platform information. Use it to verify the hardware configuration (clocks, memory) and software configuration (domains) match expectations.

```bash
cd ref_files/step2_sw/vek280_custom_dfx/export/vek280_custom_dfx
platforminfo vek280_custom_dfx.xpfm
```

You should see (abridged):

- **Platform**: `vek280_custom_dfx`
- **FPGA Device**: `xcve2802`
- **Board Part**: `xcve2802-vsvh1760-2MP-e-S`
- **Reconfigurable Partition**: id 0, with an extensible RP XSA and an HW-emulation XSA registered
- **System configuration**: default `vek280_custom_dfx`, processor groups `aiengine` (AIE runtime) and `xrt` (cortex-a72 Linux)

## Test 2: PL Acceleration — `vadd`

The `vadd` flow copies `$XILINX_VITIS/samples/vadd/` into `vadd_work/`, overrides the sample Makefile with `makefile_vadd`, then runs the standard `v++ → host build → package` sequence against `vek280_custom_dfx.xpfm`.

### Hardware Emulation

```bash
cd ref_files/step3_validate
source <Vitis_Install_Directory>/settings64.sh
export COMMON_IMAGE_VERSAL=<path/to/xilinx-versal-common-v2026.1/>
make vadd_emu
```

The emulation reaches `TEST PASSED` on the QEMU serial console once the kernel runs.

> **Vitis 2026.1 note:** `makefile_vadd` passes the boot components explicitly to `v++ --package` (`--package.bl31`, `--package.uboot`, `--package.generate_sdcard`, `--package.sd_file <Image>` / `<boot.scr>`). This selects the classic (non-EDF) QEMU launcher and avoids the lopper "`82-12113 DTSI file type is not found in XSA`" regression that affects implicit BOOT.BIN generation in 2026.1.

### Hardware Build (for on-board test)

```bash
make vadd_hw
```

Produces `vadd_work/build_dir.hw./vadd.xclbin` (the DFX-RM xclbin), `vadd_work/simple_vadd` (host ELF), and the assembled SD-card image under `vadd_work/package.hw/sd_card/` (via `--package.generate_sdcard`).

Flash the SD-card image and boot the VEK280 in SD mode. On the UART console:

```bash
cd /run/media/sd-mmcblk0p1
./simple_vadd vadd.xclbin
```

Expected tail of the UART log:

```
TEST PASSED
```

## Test 3: AIE-ML Matrix Multiply

The AIE-ML matrix-multiply graph lives entirely inside the tutorial — sources under [`aie_src/`](./ref_files/step3_validate/aie_src/), kernel/host build driven by [`makefile_aieml`](./ref_files/step3_validate/makefile_aieml), and `system.cfg` describing the PL ↔ AIE-ML stream connectivity. The top-level Makefile (`step3_validate/Makefile`) wraps it all:

```makefile
cpAieCode:
    mkdir -p aie_work
    cp -rf $(ROOT_DIR)/aie_src/* aie_work/
    cp makefile_aieml aie_work/Makefile
```

### What gets built

| File                                                  | Role                                                                                                                |
| ----------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------- |
| `aie_src/src/graph.cpp`, `MultGraph.h`, `kernels.h`   | AIE-ML graph definition and kernel declarations.                                                                    |
| `aie_src/src/matmult.cpp`                             | AIE-ML matmul kernel implementation.                                                                                |
| `aie_src/src/mm2s_8_128.cpp`, `s2mm_16_128.cpp`, `s2mm_32_128.cpp` | PL HLS data-movers (memory-mapped to stream, and back).                                                  |
| `aie_src/src/host.cpp`                                | XRT host application that orchestrates AIE-ML graph execution and PL data movers.                                   |
| `aie_src/src/system_settings*.h`, `tiling_parameters.h` | Static configuration for matrix sizes, sub-matrix tiling, PLIO widths, etc.                                       |
| `aie_src/data/input{A,B}_128.txt`, `outputC_ref_128_{16,32}b.txt` | Reference inputs and expected outputs used by the host for self-check.                                  |
| `aie_src/system.cfg`                                  | `v++ --link` connectivity directives (`nk=...`, `stream_connect=...`) and default clock frequency.                  |

> **Clock choice (Vitis 2026.1):** `system.cfg` ships with `defaultFreqHz=156250000` (156.25 MHz). The HLS `srem` divider inside `mm2s_8_128.cpp` fails timing closure on the VEK280 DFX-RP at 312.5 MHz with the 2026.1 default impl strategy. If you raise the clock, expect to also pass a non-default Vivado impl strategy via `--config` to close timing.

### Hardware Emulation

```bash
cd ref_files/step3_validate
source <Vitis_Install_Directory>/settings64.sh
export COMMON_IMAGE_VERSAL=<path/to/xilinx-versal-common-v2026.1/>
make aie_adder_emu
```

Build time is approximately 30–40 minutes (mostly `v++` link and `aiecompiler`). On success, the QEMU run prints `TEST PASSED` from `host.cpp` after the AIE-ML graph completes and the reference comparison succeeds.

> **Vitis 2026.1 note:** `makefile_aieml` uses the same non-EDF `v++ --package` flag set (`--package.bl31`, `--package.uboot`, `--package.generate_sdcard`, `--package.sd_file <Image>`/`<boot.scr>`, `--package.defer_aie_run`). The resulting `launch_hw_emu.sh` drives `launch_emulator` with `-qemu-args-file`, `-pmc-args-file`, `-qemu-dtb`, `-pmc-dtb`, `-sd-card-image` — exactly the framework used by the passing 2026.1 regression suite. No `-enable-edf`.

### Hardware Build (for on-board test)

```bash
make aie_adder_hw
```

Build time is approximately 30–35 minutes. The package output is at `aie_work/package./` (note the trailing dot — Versal `v++ --package` for AIE-ML+PL emits the assembled SD-card content under `package.<target>/sd_card/` plus a flashable `package.<target>/sd_card.img` (≈3 GB)).

Contents of `aie_work/package./sd_card/`:

| File | Purpose |
| --- | --- |
| `BOOT.BIN` | Static-region boot image (PMC + APU + bl31 + u-boot + system.dtb) |
| `Image`, `boot.scr` | Linux kernel + U-Boot script |
| `aieml_system` | AIE-ML host ELF (cross-compiled for `cortexa72-cortexa53-amd-linux`) |
| `krnl_aieml.xclbin` | DFX RM xclbin containing the AIE-ML graph + 3 PL data-movers |
| `run_aieml.sh` | Launcher script |
| `data/` | Reference inputs and expected outputs |

Either `dd` `package./sd_card.img` onto an SD card, or copy the contents of `package./sd_card/` onto a FAT32 partition.

On VEK280:

```bash
cd /run/media/sd-mmcblk0p1
./aieml_system krnl_aieml.xclbin
```

Expected tail of the UART log:

```
TEST PASSED
```

## Test 4: DFX Reconfiguration on Hardware

The whole point of a DFX platform is runtime reconfiguration: loading a different `xclbin` into the dynamic region without rebooting. After running the AIE-ML graph above, copy `simple_vadd` and `vadd.xclbin` to the FAT32 partition of the same SD card (do not reflash), reboot, and run:

```bash
cd /run/media/sd-mmcblk0p1
./simple_vadd vadd.xclbin      # PL-only RM
./aieml_system krnl_aieml.xclbin   # AIE-ML + PL RM (reconfigures)
```

Both should pass back-to-back. Each `xclbin` load triggers a full dynamic-region reconfiguration via XRT and the DFX Decoupler; the static region (CIPS, NoC, interrupt controller, U-Boot, Linux) keeps running.

## What's Next

If you need to iterate on the hardware, software, or application layers, follow the [Iteration Guidelines](./Iteration_guideline.md).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
