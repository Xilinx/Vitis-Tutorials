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

# Packet Switching Tutorial

***Version: Vitis 2025.2***

AI Engine kernels can share a single processor and execute in an interleaved manner. Also, they can share multiple stream connections on a single physical channel. The explicit packet switching feature allows fine-grain control over how the system generates, distributes, and consumes packets in a graph computation. Explicit packet switching is typically recommended when many low bandwidth streams from a common programmable logic (PL) source distribute to different AI Engine destinations. Similarly, many low bandwidth streams from different AI Engine sources to a common PL destination can also take advantage of this feature. Because multiple streams share a single physical channel, you minimize the number of AI Engine - PL interface streams used.

This tutorial covers three main steps.

* Step 1 creates buffer interface AI Engine kernels that share the same AI Engine - PL interface streams.
* Step 2 creates designs with float and `cint16` data types.
* Step 3 creates packet stream interface AI Engine kernels that share AI Engine - PL interface streams.

The AI Engine kernels differ in each step. Step 3 introduces packet stream interfaces and associated built-in functions. Steps 1 and 3 describe the graph construction but with differences. All these designs share the same PL kernels introduced in step 1. The PS codes for these steps are similar. Step 1 and step 2 detail them (with differences).

The current version of the AI Engine tools supports only integer format values in data files for the AI Engine simulator. Steps 1 and 2 detail the data format and how to convert data from other types to integer format.

>**IMPORTANT**: Before beginning the tutorial, install the AMD Vitis™ unified software platform 2025.2. This Vitis release includes all the embedded base platforms including the VCK190 base platform that this tutorial uses. Also, download the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/amd/en/downloadNav/embedded-platforms.html).

The 'common image' package contains a prebuilt Linux kernel and root file system that you can use with the AMD Versal™ board for embedded design development using Vitis tools.

Before starting this tutorial, run the following steps:

1. Go to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the ``/**Common Images Dir**/xilinx-versal-common-v2025.2/environment-setup-cortexa72-cortexa53-amd-linux`` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you must run the ``/**Common Images Dir**/xilinx-versal-common-v2025.2/sdk.sh``.
3. Set up your ROOTFS and IMAGE to point to the `rootfs.ext4` and Image files located in the ``/**Common Images Dir**/xilinx-versal-common-v2025.2`` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to ``$XILINX_VITIS/base_platforms``.

This tutorial targets VCK190 production board for 2025.2 version.

## Objectives

After completing this tutorial, you can:

* Construct a packet switching graph
* Understand the packet format for packet switching
* Write an AI Engine kernel that handles packet stream explicitly
* Design PL kernels for packet switching
* Write a PS application for hardware and hardware emulation flows

## Steps

**Step 1**: Construct an explicit packet switching graph with buffer interface AI Engine kernels. This step also introduces PL kernels and PS code for the system design. Refer to [Buffer-Based AI Engine Kernels](./buffer_based_aie_kernel.md) for more details.

**Step 2**: Special consideration on float and `cint*` data types. Refer to [Buffer-Based AI Engine Kernels with Mixed Data Types](./buffer_based_mix_data_type.md) for more details.

**Step 3**: Introduce the packet stream interface and built-in functions for AI Engine kernels. This lets you fine control how the system decodes and constructs packets. Refer to [Packet Stream-Based AI Engine Kernels](./pktstream_based_aie_kernel.md) for more details.

>**Note:** This tutorial provides a Makefile and instructions.

>**Hint:** The designs in this tutorial are self-contained in each step, but the steps refer to previous steps. Therefore, it is highly recommended to start from the beginning and progress to completion.

### Support

GitHub issues are used for tracking requests and bugs. For questions go to [forums](https://adaptivesupport.amd.com/s/?language=en_US).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
