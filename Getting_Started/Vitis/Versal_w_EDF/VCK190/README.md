<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Getting Started with AMD Versal™ AI Core VCK190 with Vitis™ Unified IDE (EDF flow)</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

***Version: Vitis 2026.1 and Vivado 2026.1***

Welcome to the Vitis Getting Started tutorial for the VCK190 EDF flow.

This tutorial showcases the steps to build AI Engine (AIE) and High-Level Synthesis (HLS) kernels along with a host application, and run the design in hardware emulation (QEMU) and on the VCK190 board.

This tutorial uses the pre-built `vck190_base.xpfm` platform from the Vitis installation (`${PLATFORM_REPO_PATHS}/vck190_base/vck190_base.xpfm`).

This tutorial is aligned with the AMD **Embedded Development Framework (EDF)**. For more information about AMD EDF, please refer to the official [AMD EDF Wiki page](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/3250585601/AMD+Embedded+Development+Framework+EDF#Introduction-to-the-AMD-Embedded-Development-Framework).

Please go through the [Makefile](./Makefile) and [makefile_vck190](./makefile_vck190) provided in this tutorial to better understand the Vitis EDF tool flow.

## Design overview

* AIE graph (`AIE_Kernel/aie_srcs/graph.cpp`) implementing a matrix-multiply kernel.
* Two HLS Programmable Logic (PL) kernels (`HLS_Kernels/mm2s.cpp`, `HLS_Kernels/s2mm.cpp`) for data movement between DDR and the AIE graph.
* Host application (`Host_srcs/host.cpp`) using Xilinx Runtime (XRT) APIs to drive the kernels and graph.
* System connectivity (`vitis_dir/system.cfg`) wiring `mm2s_1`, `mm2s_2`, `ai_engine_0.DataIn{1,2}`, `ai_engine_0.DataOut1`, and `s2mm`.

## Prerequisites

Before building this tutorial:

1. Source Vitis 2026.1:

   ```bash
   source <path_to_vitis_install>/settings64.sh
   ```

2. Export `PLATFORM_REPO_PATHS` and `YOCTO_ARTIFACTS`:

   ```bash
   export PLATFORM_REPO_PATHS=<path_to_vitis_install>/base_platforms
   export YOCTO_ARTIFACTS=<path-to-design>/yocto_artifacts
   ```

3. Download the EDF Yocto artifacts for Versal Gen1 (Cortex-A72) from the [Embedded Development Framework (EDF) downloads page](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-design-tools.html) (package matching Vitis 2026.1):

   * `amd-cortexa72-common_meta-edf-app-sdk` – install the SDK into `${YOCTO_ARTIFACTS}/amd-cortexa72-common_meta-edf-app-sdk/sdk`:

     ```bash
     ./amd-edf-glibc-x86_64-meta-edf-app-sdk-cortexa72-cortexa53-amd-cortexa72-common-toolchain.sh \
         -d ${YOCTO_ARTIFACTS}/amd-cortexa72-common_meta-edf-app-sdk/sdk -y
     ```

   * `amd-cortexa72-common_edf-platform-disk-image` – unzip and move into `${YOCTO_ARTIFACTS}/`.
   * `amd-cortexa72-common_vck190_qemu_prebuilt` – unzip and move into `${YOCTO_ARTIFACTS}/`.

4. Set up the EDF sysroot environment:

   ```bash
   source ${YOCTO_ARTIFACTS}/amd-cortexa72-common_meta-edf-app-sdk/sdk/environment-setup-cortexa72-cortexa53-amd-linux
   ```

## Building the design

### Hardware emulation (QEMU)

```bash
make all
```

This compiles the AIE graph, HLS PL kernels, host application, then assembles the QEMU combined image, copies the binaries into the WIC rootfs using `wic cp`, and launches `launch_hw_emu.sh` with the EDF `combined.qemuboot.conf`. The QEMU session auto-logs in as `amd-edf`, mounts `/dev/sda2`, and executes `run_app_hw_emu.sh`.

### Hardware (SD-card boot)

```bash
make sd_card
```

This builds everything for `TARGET=hw` and prepares `vck190_work/hw_run/` containing the `.pdi`, `.dtbo`, `.xclbin`, `application`, and `embedded_exec.sh` ready to copy to the VCK190 board.

## Running on the VCK190 board

Refer to [UG1366](https://docs.amd.com/r/en-US/ug1366-vck190-eval-bd) for VCK190 hardware details.

1. Program the EDF platform rootfs WIC (`edf-platform-disk-image-amd-cortexa72-common.rootfs.wic.xz`) to an SD card using `balenaEtcher` or similar.
2. Insert the SD card into the VCK190 and set boot mode to **SD** (refer to UG1366 for the dip-switch configuration).
3. Power on the board and open the UART console.
4. Log in as `amd-edf` (you will be prompted to set a new password on first login — this also becomes the sudo password):

   ```bash
   amd-edf login: amd-edf
   New password:
   Retype new password:
   amd-edf:~$ sudo su
   ```

5. Copy the contents of `vck190_work/hw_run/` to the board (via `scp` or by placing them on the SD card).
6. Run the application:

   ```bash
   amd-edf:/home/amd-edf# ./embedded_exec.sh
   ```

   Expected output:

   ```text
   INFO: Load the pdi and dtbo using fpgautil
   ...
   TEST PASSED
   INFO: Embedded host run completed.
   ```

## Cleaning

```bash
make clean        # remove build artifacts
make ultraclean   # remove the entire vck190_work/ directory
```

<p class="sphinxhide" align="center"><sub>Copyright © 2025–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/legal/copyright.html">Terms and Conditions</a></sup></p>
