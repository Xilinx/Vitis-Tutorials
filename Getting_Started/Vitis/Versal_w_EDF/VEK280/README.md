<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Getting Started with AMD Versal™ AI Edge VEK280 with Vitis™ Unified IDE (EDF flow)</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

***Version: Vitis 2026.1 and Vivado 2026.1***

Welcome to the Vitis Getting Started tutorial for the VEK280 EDF flow.

This tutorial showcases the steps to build High-Level Synthesis (HLS) Programmable Logic (PL) kernels and an AI Engine-ML (AIE-ML) graph along with a host application, and run the design in hardware emulation (QEMU) and on the VEK280 board.

This tutorial uses the pre-built `vek280_base.xpfm` platform from the Vitis installation (`${PLATFORM_REPO_PATHS}/vek280_base/vek280_base.xpfm`) and the AIE-ML system design sample shipped with Vitis (`${XILINX_VITIS}/samples/aie_system_examples/aie-ml_sys_design`).

This tutorial is aligned with the AMD **Embedded Development Framework (EDF)**. For more information, see the official [AMD EDF Wiki page](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/3250585601/AMD+Embedded+Development+Framework+EDF#Introduction-to-the-AMD-Embedded-Development-Framework).

Please go through the [Makefile](./Makefile) and [makefile_aieml](./makefile_aieml) provided in this tutorial to better understand the Vitis EDF tool flow.

## Design overview

* AIE-ML graph implementing a tiled matrix multiplication (`ClassicMatMult`), with configurable matrix dimensions and sub-tile sizes.
* Three HLS PL kernels for data movement: `mm2s_8_128`, `s2mm_16_128`, `s2mm_32_128`.
* Host application that loads the xclbin, drives the kernels and graph, and verifies the result.

## Prerequisites

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

   * `amd-cortexa72-common_meta-edf-app-sdk` – install into `${YOCTO_ARTIFACTS}/amd-cortexa72-common_meta-edf-app-sdk/sdk`:

     ```bash
     ./amd-edf-glibc-x86_64-meta-edf-app-sdk-cortexa72-cortexa53-amd-cortexa72-common-toolchain.sh \
         -d ${YOCTO_ARTIFACTS}/amd-cortexa72-common_meta-edf-app-sdk/sdk -y
     ```

   * `amd-cortexa72-common_edf-platform-disk-image` – unzip and move into `${YOCTO_ARTIFACTS}/`.
   * `amd-cortexa72-common_vek280_qemu_prebuilt` – unzip and move into `${YOCTO_ARTIFACTS}/`.

4. Set up the EDF sysroot environment:

   ```bash
   source ${YOCTO_ARTIFACTS}/amd-cortexa72-common_meta-edf-app-sdk/sdk/environment-setup-cortexa72-cortexa53-amd-linux
   ```

## Building the design

### Hardware emulation (QEMU)

```bash
make all
```

This stages the AIE-ML sample into `aieml_work/`, compiles HLS PL kernels and the AIE-ML graph, builds the host application, packages with `--package.defer_aie_run`, assembles the QEMU combined image, copies binaries into the WIC rootfs using `wic cp`, and launches `launch_hw_emu.sh` with the EDF `combined.qemuboot.conf`. The QEMU session auto-logs in as `amd-edf`, mounts `/dev/sda2`, and executes `run_app_hw_emu.sh`.

### Hardware (SD-card boot)

```bash
make sd_card
```

This builds everything for `TARGET=hw` and prepares `aieml_work/hw_run/` with the `.pdi`, `.dtbo`, `.xclbin`, `aieml_system` host application, and `embedded_exec.sh` ready to copy to the VEK280 board.

## Running on the VEK280 board

Refer to [UG1612](https://docs.amd.com/r/en-US/ug1612-vek280) for VEK280 hardware details.

1. Program the EDF platform rootfs WIC (`edf-platform-disk-image-amd-cortexa72-common.rootfs.wic.xz`) to an SD card using `balenaEtcher` or similar.
2. Insert the SD card into the VEK280 and set boot mode to **SD** (refer to UG1612 for the dip-switch configuration).
3. Power on the board and open the UART console.
4. Log in as `amd-edf` (set a new password on first login):

   ```bash
   amd-edf login: amd-edf
   amd-edf:~$ sudo su
   ```

5. Copy the contents of `aieml_work/hw_run/` to the board (via `scp` or by placing them on the SD card).
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
make clean        # remove intermediate build artifacts
make ultraclean   # remove the entire aieml_work/ directory
```

<p class="sphinxhide" align="center"><sub>Copyright © 2025–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/legal/copyright.html">Terms and Conditions</a></sup></p>
