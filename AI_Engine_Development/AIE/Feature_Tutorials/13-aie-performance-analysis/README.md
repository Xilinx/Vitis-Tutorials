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

# AI Engine Performance and Deadlock Analysis Tutorial

***Version: Vitis 2025.2***

## Introduction

This tutorial introduces you to performance analysis and optimization methods, and shows you how synchronization works in graph execution. It also demonstrates the analysis of a hang issue using an example.

## Before You Begin

**IMPORTANT**: Before beginning the tutorial, install the AMD Vitis™ 2025.2 software platform. This Vitis release includes all the embedded base platforms, including the VCK190 base platform that this tutorial uses. Also, download the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html).

The ‘common image’ package contains a prebuilt Linux kernel and root file system that you can use with the AMD Versal™ board for embedded design development using the Vitis software platform.

Before starting this tutorial, run the following steps:

1. Go to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the `/**Common Images Dir**/xilinx-versal-common-v2025.2/environment-setup-cortexa72-cortexa53-amd-linux` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you **must** run the `/**Common Images Dir**/xilinx-versal-common-v2025.2/sdk.sh`.
3. Set up your ROOTFS, and IMAGE to point to the `rootfs.ext4` and image files located in the `/**Common Images Dir**/xilinx-versal-common-v2025.2` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to `$XILINX_VITIS/base_platforms`.

This tutorial targets VCK190 production board for 2025.2 version.

### Objectives

After completing this tutorial, you can:

- Read lock signals in Vitis Analyzer.
- Profile design performance.
- Identify a performance bottleneck.
- Analyze a deadlock (hang issue) in the design.
- Analyze AI Engine status using Vitis Analyzer.

## Steps

**Step 1**: Learn about the execution model of an AI Engine graph, including how memory with lock and FIFO works. Learn performance measurement methods. Identify performance bottlenecks and practise optimization. Go to [AI Engine Graph Execution and Measurement](./aie_execution_measurement.md).

**Step 2**: Learn about how a deadlock can occur with an example design. Learn how to analyze a deadlock in the AI Engine simulator, hardware emulation, and hardware flows. Go to [AI Engine Deadlock Analysis](./aie_hang_analysis.md).

**Step 3**: Learn about AI Engine status analysis in the hardware and hardware emulation flows using Vitis Analyzer. Go to [AI Engine Status Analysis](./aie_status_analysis.md).

**Tip:** In this tutorial, the designs are self contained in each step. You can choose to start at any step depending on your experience and requirements. Be aware that the concepts and options introduced in the previous step might not be repeated later. AMD recommends you to start from the beginning and progress through to completion.

### Support

GitHub issues are used for tracking requests and bugs. For questions, go to [support.amd.com](https://adaptivesupport.amd.com/s/?language=en_US).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
