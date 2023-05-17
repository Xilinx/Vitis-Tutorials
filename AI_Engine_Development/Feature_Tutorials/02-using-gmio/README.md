<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# GMIO Tutorial

***Version: Vitis 2023.1***

## Introduction

A GMIO port attribute is used to make external memory-mapped connections to or from the global memory. These connections are made between AI Engine kernels or programmable logic kernels and the logical global memory ports of a hardware platform design. This tutorial is designed to demonstrate how to work with the GMIO interface in AI Engine simulator and hardware flows.

>**IMPORTANT**: Before beginning the tutorial make sure you have installed the AMD Vitis™ 2023.1 software. The Vitis release includes all the embedded base platforms including the VCK190 base platform that is used in this tutorial. In addition, ensure you have downloaded the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html).

The ‘common image’ package contains a prebuilt Linux kernel and root file system that can be used with the AMD Versal™ board for embedded design development using the Vitis software platform.

Before starting this tutorial, run the following steps:

1. Go to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the ``/Common Images Dir/xilinx-versal-common-v2023.1/environment-setup-cortexa72-cortexa53-xilinx-linux`` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you must run the ``/Common Images Dir/xilinx-versal-common-v2023.1/sdk.sh``.
3. Set up your ROOTFS and IMAGE to point to the ``rootfs.ext4`` and Image files located in the ``/Common Images Dir/xilinx-versal-common-v2023.1`` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to ``$XILINX_VITIS/base_platforms``.

This tutorial targets VCK190 production board for 2023.1 version.

## Objectives

After completing this tutorial, you will be able to:

* Understand the programming model and software programmability of the AI Engine GMIO.
* Profile GMIO design in AI Engine simulator and improve performance.
* Profile GMIO performance by event trace API or Linux system timer.
* Measure the NOC bandwidth and make trade offs between GMIO and PLIO.

## Steps

>**Note**: This tutorial assumes that you have a basic understanding of the Adaptive Data Flow (ADF) API and Xilinx® Runtime (XRT) API usage. For more information about ADF API and XRT usage, refer to AI Engine Runtime Parameter Reconfiguration Tutorial and the Versal Adaptive SoC AI Engine Programming Environment User Guide ([UG1076](https://docs.xilinx.com/access/sources/dita/map?isLatest=true&ft:locale=en-US&url=ug1076-ai-engine-environment)).

**Step 1 - AI Engine GMIO**: Introduces the programming model of AI Engine GMIO, including blocking and non-blocking GMIO transactions. See details in [AIE GMIO Programming Model](./single_aie_gmio.md).

**Step 2 - AI Engine GMIO Performance Profile**: Introduces ways to create a performance profile of the AI Engine GMIO. Learn about the maximum supported GMIO ports and NOC bandwidth. See details in [AIE GMIO Performance Profile](./perf_profile_aie_gmio.md).

> **Note**: In this tutorial, a Makefile is provided. If ``make`` commands exist, you can run them. Detailed commands are also shown to better illustrate the concepts. You can run these commands manually.

> **Note**: In this tutorial, the designs are self-contained in each step. You can choose to start at any step depending on your experience and requirements. Be aware that the concepts and options introduced in the previous step might not be repeated later. It is highly recommended to start from the beginning and progress to completion.

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
