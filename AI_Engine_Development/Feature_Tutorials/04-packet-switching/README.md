<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Packet Switching Tutorial

***Version: Vitis 2023.1***

AI Engine kernels can share a single processor and execute in an interleaved manner. In addition, they can also share multiple stream connections on a single physical channel. The explicit packet switching feature allows fine-grain control over how packets are generated, distributed, and consumed in a graph computation. Explicit packet switching is typically recommended in cases where many low bandwidth streams from a common programmable logic (PL) source can be distributed to different AI Engine destinations. Similarly many low bandwidth streams from different AI Engine sources to a common PL destination can also take advantage of this feature. Because a single physical channel is shared between multiple streams, you minimize the number of AI Engine - PL interface streams used.

This tutorial walks you through the steps to create buffer interface AI Engine kernels that share the same AI Engine - PL interface streams (step 1), to create designs with float and `cint16` data types (step 2), and to create packet stream interface AI Engine kernels that share AI Engine - PL interface streams (step 3). The AI Engine kernels are different in these steps. In particular, packet stream interfaces and associated built-in functions are introduced in step 3. The graph construction is covered in detail in step 1 and step 3 (only with differences). All these designs share the same PL kernels that are introduced in step 1. The PS codes for these steps are similar; this is covered in detail in step 1 and step 2 (with differences).

There is a limitation in the current version of the AI Engine tools that only integer format values are supported in data files for the AI Engine simulator. Details on the data format and how to convert data from other types to integer format are covered in steps 1 and 2.

>**IMPORTANT**: Before beginning the tutorial make sure you have installed the AMD Vitis™ software platform 2023.1. The Vitis release includes all the embedded base platforms including the VCK190 base platform that is used in this tutorial. In addition, ensure that you have downloaded the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html).

The ‘common image’ package contains a prebuilt Linux kernel and root file system that can be used with the AMD Versal™ board for embedded design development using the Vitis tools.

Before starting this tutorial, run the following steps:

1. Go to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the ``/**Common Images Dir**/xilinx-versal-common-v2023.1/environment-setup-cortexa72-cortexa53-xilinx-linux`` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you must run the ``/**Common Images Dir**/xilinx-versal-common-v2023.1/sdk.sh``.
3. Set up your ROOTFS and IMAGE to point to the `rootfs.ext4` and Image files located in the ``/**Common Images Dir**/xilinx-versal-common-v2023.1`` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to ``$XILINX_VITIS/base_platforms``.

This tutorial targets VCK190 production board for 2023.1 version.

## Objectives

After completing this tutorial, you will be able to:

* Construct a packet switching graph
* Understand the packet format for packet switching
* Write an AI Engine kernel that handles packet stream explicitly
* Design PL kernels for packet switching
* Write a PS application for hardware and hardware emulation flows

## Steps

**Step 1**: Construct an explicit packet switching graph with buffer interface AI Engine kernels. PL kernels and PS code for the system design are also introduced. See details in [Buffer-Based AI Engine Kernels](./buffer_based_aie_kernel.md).

**Step 2**: Special consideration on float and `cint*` data types. See details in [Buffer-Based AI Engine Kernels with Mixed Data Types](./buffer_based_mix_data_type.md).

**Step 3**: Introduce the packet stream interface and built-in functions for AI Engine kernels, which allows you to fine control how packets are decoded and constructed. See details in [Packet Stream-Based AI Engine Kernels](./pktstream_based_aie_kernel.md).

>**Note:** In this tutorial, a Makefile and instructions are provided.

>**Hint:** In this tutorial, the designs are self-contained in each step, but the steps refer to previous steps. Therefore, it is highly recommended to start from the beginning and progress to completion.

### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums](http://forums.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
