<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Packet Switching Tutorial

***Version: Vitis 2024.1***

This tutorial targets AIE-ML device, but shares test cases and description files with [AIE 04-packet-switching](../../../AIE/Feature_Tutorials/04-packet-switching).

>**IMPORTANT**: Before beginning the tutorial make sure you have installed the AMD Vitis™ software platform 2024.1. The Vitis release includes all the embedded base platforms including the VEK280 ES1 base platform that is used in this tutorial. In addition, ensure that you have downloaded the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html).

The ‘common image’ package contains a prebuilt Linux kernel and root file system that can be used with the AMD Versal™ board for embedded design development using the Vitis tools.

Before starting this tutorial, run the following steps:

1. Go to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the ``/**Common Images Dir**/xilinx-versal-common-v2024.1/environment-setup-cortexa72-cortexa53-xilinx-linux`` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you must run the ``/**Common Images Dir**/xilinx-versal-common-v2024.1/sdk.sh``.
3. Set up your ROOTFS and IMAGE to point to the `rootfs.ext4` and Image files located in the ``/**Common Images Dir**/xilinx-versal-common-v2024.1`` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to ``$XILINX_VITIS/base_platforms``.

This tutorial targets VEK280 ES1 board for 2024.1 version.

## Objectives

After completing this tutorial, you will be able to:

* Construct a packet switching graph
* Understand the packet format for packet switching
* Write an AI Engine kernel that handles packet stream explicitly
* Design PL kernels for packet switching
* Write a PS application for hardware and hardware emulation flows

## Steps

**Step 1**: Copy test case from [Buffer AIE](../../../AIE/Feature_Tutorials/04-packet-switching/buffer_aie). 

Follow instructions in [Buffer-Based AI Engine Kernels](../../../AIE/Feature_Tutorials/04-packet-switching/buffer_based_aie_kernel.md). But, **run all make commands with `ARCH=aie-ml`**. For example:

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

**Step 2**: Copy test case from [Buffer AIE mix types](../../../AIE/Feature_Tutorials/04-packet-switching/buffer_aie_mix_int32_float_cint16). 

Follow instructions in [Buffer-Based AI Engine Kernels with Mixed Data Types](../../../AIE/Feature_Tutorials/04-packet-switching/buffer_based_mix_data_type.md). But, **run all make commands with `ARCH=aie-ml`**. See run commands in step 1.

**Step 3**: Copy test case from [Pktstream AIE](../../../AIE/Feature_Tutorials/04-packet-switching/pktstream_aie). 

Follow instructions in [Packet Stream-Based AI Engine Kernels](../../../AIE/Feature_Tutorials/04-packet-switching/pktstream_based_aie_kernel.md). But, **run all make commands with `ARCH=aie-ml`**. See run commands in step 1.

### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums](http://forums.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
