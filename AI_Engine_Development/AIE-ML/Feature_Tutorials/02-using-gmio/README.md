<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">See Vitis™ AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# GMIO Tutorial

***Version: Vitis 2025.2***

## Introduction

This tutorial targets AIE-ML devices, but shares test cases and description files with [AIE 02-using-gmio](../../../AIE/Feature_Tutorials/02-using-gmio).

>**IMPORTANT**: Before beginning the tutorial, install the AMD Vitis™ unified software platform 2025.2. This Vitis release includes all the embedded base platforms including the VCK190 base platform that this tutorial uses. Also, download the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/amd/en/downloadNav/embedded-platforms.html).

The ‘common image’ package contains a prebuilt Linux kernel and root file system that you can use with AMD Versal™ boards for embedded design development using the Vitis software platform.

Before starting this tutorial, run the following steps:

1. Go to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the ``/Common Images Dir/xilinx-versal-common-v2025.2/environment-setup-cortexa72-cortexa53-amd-linux`` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you must run the ``/Common Images Dir/xilinx-versal-common-v2025.2/sdk.sh``.
3. Set up your ROOTFS and IMAGE to point to the ``rootfs.ext4`` and Image files located in the ``/Common Images Dir/xilinx-versal-common-v2025.2`` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to ``$XILINX_VITIS/base_platforms``.

This tutorial targets VEK280 board for 2025.2 version.

## Objectives

After completing this tutorial, you can:

* Understand the programming model and software programmability of the AI Engine GMIO
* Profile GMIO design in AI Engine simulator and improve performance
* Profile GMIO performance by event trace API or Linux system timer
* Measure the NOC bandwidth and make trade offs between GMIO and PLIO

## Steps

**Step 1 - AI Engine GMIO**: Copy test case from [Single AIE test cases](../../../AIE/Feature_Tutorials/02-using-gmio/single_aie_gmio).

Follow instructions in [AIE GMIO Programming Model](../../../AIE/Feature_Tutorials/02-using-gmio/README.md#single-aie-kernel-gmio). But, **run all make commands with `ARCH=aie-ml`**. For example:

To build AIE graph, run:

```bash
make aie ARCH=aie-ml
```

To run `aiesimulator`, run:

```bash
make aiesim ARCH=aie-ml
```

To run HW emulation, run:

```bash
make run_hw_emu ARCH=aie-ml
```

After Linux has booted, run the following commands at the Linux prompt. To exit QEMU press `Ctrl+A` and then press `x`. (this is only for HW cosim):

```bash
./host.exe a.xclbin
```

To build system design targeting HW flow, run:

```bash
make package TARGET=hw ARCH=aie-ml
```

To run in hardware, run the following commands at the Linux prompt after booting Linux. Login with username:`petalinux`, password:`petalinux`, then run `sudo su` with password:`petalinux` before running following commands:

```bash
cd /run/media/mm*1
./host.exe a.xclbin
```

**Step 2 - AI Engine GMIO Performance Profile**: Copy test case from [Performance profile test cases](../../../AIE/Feature_Tutorials/02-using-gmio/perf_profile_aie_gmio).

Follow instructions in [AIE GMIO Performance Profile](../../../AIE/Feature_Tutorials/02-using-gmio/perf_profile_aie_gmio.md). But, **run all make commands with `ARCH=aie-ml`**. See run commands in step 1.

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
