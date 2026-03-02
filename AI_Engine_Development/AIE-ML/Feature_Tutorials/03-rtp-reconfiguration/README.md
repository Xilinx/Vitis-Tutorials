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

# Runtime Parameter Reconfiguration

***Version: Vitis 2025.2***

## Introduction

This tutorial targets AIE-ML device, but shares test cases and description files with [AIE 03-rtp-reconfiguration](../../../AIE/Feature_Tutorials/03-rtp-reconfiguration).

>**IMPORTANT**: Before beginning the tutorial make sure you have installed the AMD Vitis™ 2025.2 software. The Vitis release includes all the embedded base platforms including the VEK280 base platform that this tutorial uses. Also, make sure that you have downloaded the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html).

The Common Image package includes a prebuilt Linux kernel and root file system for AMD Versal boards, supporting embedded development with Vitis tools.

Before starting this tutorial, run the following steps:

1. Go to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the ``/Common Images Dir/xilinx-versal-common-v2025.2/environment-setup-cortexa72-cortexa53-amd-linux`` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you must run the ``/Common Images Dir/xilinx-versal-common-v2025.2/sdk.sh``.
3. Set up your ROOTFS and IMAGE to point to the `rootfs.ext4` and Image files located in the ``/Common Images Dir/xilinx-versal-common-v2025.2`` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to ``$XILINX_VITIS/base_platforms``.

This tutorial targets the VEK280 board for the 2025.2 version.

## Objectives

After completing this tutorial, you can do the following:

* Specify a scalar or array parameter as part of a kernel function signature.
* Connect a parameterized kernel into a graph, exposing the parameter for runtime updates.
* Simulate a graph containing runtime parameters with AI Engine simulator (aiesimulator).
* Build a system with AI Engine kernels and PL kernels, plus PS code to control their execution.
* Use XRT API to control graph execution and RTP operations.
* Verify the system by HW co-simulation and running in hardware.

**Known issue in 2025.2**: There can exist hardware execution issue in asynchronous array RTP update & read. When it happens, wait for an update in the tool or a patch. Otherwise, continue to use 2024.1 instead.

## Steps

**Note:** Copy test cases and follow instructions in [AIE 03-rtp-reconfiguration](../../../AIE/Feature_Tutorials/03-rtp-reconfiguration). Run all make commands with `ARCH=aie-ml`. For example:

To build AIE graph, run:

```
make aie ARCH=aie-ml
```

To run `aiesimulator`, run:

```
make aiesim ARCH=aie-ml
```

To run HW emulation, run:

```
make run_hw_emu ARCH=aie-ml
```

After Linux has booted, run the following commands at the Linux prompt. To exit QEMU press `Ctrl+A` and then press `x`. (this is only for HW cosim):

```bash
./host.exe a.xclbin
```

To build system design targeting HW flow, run:

```
make package TARGET=hw ARCH=aie-ml
```

To run in hardware, after booting Linux, run the following commands at the Linux prompt. (login with user name:`petalinux`, password:`petalinux`. run `sudo su` with password:`petalinux` before running following commands):

```bash
cd /run/media/mm*1
./host.exe a.xclbin
```

### Support

GitHub issues are used for tracking requests and bugs. For questions go to [forums](http://forums.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
