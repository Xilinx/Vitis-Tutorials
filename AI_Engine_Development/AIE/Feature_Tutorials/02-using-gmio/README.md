<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">See Vitis AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# GMIO Tutorial

***Version: Vitis 2025.2***

## Introduction

A global memory input/output (GMIO) port attribute connects external memory-mapped interfaces to or from the global memory. You make these connections between AI Engine kernels or programmable logic (PL) kernels and the logical global memory ports of a hardware platform design. This tutorial guides you through using the GMIO interface in the AI Engine simulator and hardware flows.

>**IMPORTANT**: Before starting, make sure you installed the AMD Vitis™ 2025.2 software. This Vitis release includes all embedded base platforms, including the VCK190 base platform used in this tutorial. Also download the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html).

The common image package contains a prebuilt Linux kernel and root file system. You use it with the AMD Versal™ board for embedded design development in the Vitis software platform.

Before starting this tutorial, run the following steps:

1. Go to the directory where you unzipped the Versal Common Image package.
2. In a Bash shell, run ``/Common Images Dir/xilinx-versal-common-v2025.2/environment-setup-cortexa72-cortexa53-amd-linux``. This script sets the `SDKTARGETSYSROOT` and `CXX` variables. If the script is missing, run ``/Common Images Dir/xilinx-versal-common-v2025.2/sdk.sh``.
3. Set `ROOTFS` and `IMAGE` to point to ``rootfs.ext4`` ``Image`` files located in the ``/Common Images Dir/xilinx-versal-common-v2025.2`` directory.
4. Set your `PLATFORM_REPO_PATHS` environment variable to ``$XILINX_VITIS/base_platforms``.

This tutorial targets VCK190 production board for 2025.2.

## Objectives

After completing this tutorial, you can:

* Understand the programming model and software programmability of the AI Engine GMIO.
* Profile GMIO design in AI Engine simulator and improve performance.
* Profile GMIO performance by event trace API or Linux system timer.
* Measure the NOC bandwidth and make trade offs between GMIO and PLIO.

## Steps

>**Note**: This tutorial assumes that you have a basic understanding of the Adaptive Data Flow (ADF) API and Xilinx® Runtime (XRT) API usage. For more information about ADF API and XRT usage, refer to AI Engine Runtime Parameter Reconfiguration Tutorial and the Versal Adaptive SoC AI Engine Programming Environment User Guide ([UG1076](https://docs.amd.com/access/sources/dita/map?isLatest=true&ft:locale=en-US&url=ug1076-ai-engine-environment)).

**Step 1 - AI Engine GMIO**: Introduces the programming model of AI Engine GMIO, including blocking and non-blocking GMIO transactions. For details, refer to [AIE GMIO Programming Model](./single_aie_gmio.md).

**Step 2 - AI Engine GMIO Performance Profile**: Introduces ways to create a performance profile of the AI Engine GMIO. Learn about the maximum supported GMIO ports and network on chip (NoC) bandwidth. For details, refer to [AIE GMIO Performance Profile](./perf_profile_aie_gmio.md).

> **Note**: This tutorial provides a Makefile. If ``make`` commands exist, you can run them. Detailed commands appear for clarity, and you can run them manually.

Each step’s design is self‑contained. You can start at any step based on your experience and needs.
However, concepts and options from earlier steps might not appear later.
For best results, start at the beginning and work through to completion.

> **Note**: In this tutorial, the designs are self-contained in each step. You can start at any step based on your experience and requirements. However, concepts and options from earlier steps might not appear later. For best results, start at the beginning and work through to completion.

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
