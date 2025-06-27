<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Compiling AI Engine Graphs for Independent Partitions & Partition Reloading

***Version: Vitis 2025.1***

## Introduction

This tutorial demonstrates the workflow for compiling AI Engine graphs within AI Engine partitions. It also covers the process of reloading these partitions. 
The AI Engine graphs, which are distributed across different partitions, are independently verified using the AI Engine simulator and later integrated onto the device using the V++ linker and packager. 
This workflow is particularly well-suited for collaborative development, where multiple teams work concurrently on different parts of a system project, or when integrating user-developed designs with vendor-provided IPs (e.g., from AMD).

>**IMPORTANT**: Before beginning the tutorial, make sure you have installed AMD Vitis™ 2025.1 software. 
The Vitis release includes all the embedded base platforms, including the VCK190 base platform that is used in this tutorial. 
In addition, ensure that you have downloaded the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html).

The 'common image' package contains a pre-built Linux kernel and root file system that you can use with the AMD Versal™ board for embedded design development using Vitis tools.

Before starting this tutorial, run the following steps:

1. Open the directory where the Versal Common Image package is unzipped.
2. In a bash shell, run the ``/Common Images Dir/xilinx-versal-common-v2025.1/environment-setup-cortexa72-cortexa53-amd-linux`` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you must run the ``/Common Images Dir/xilinx-versal-common-v2025.1/sdk.sh`` file.
3. Set up your ROOTFS and IMAGE to point to the `rootfs.ext4` and image files located in the ``/Common Images Dir/xilinx-versal-common-v2025.1`` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to ``$XILINX_VITIS/base_platforms``.

This tutorial targets the VCK190 production board for the 2025.1 version.

## Overview

The AI Engine supports column-based partitions. This is enabled by creating independent graphs that can be compiled and simulated separately. 
Each graph is mapped to a specific column or a contiguous set of columns on the AI Engine array. During the v++ link stage, you can integrate multiple partitions 
together, provided there is no overlap in the resource usage by the graphs in those partitions.

Following is a conceptual illustration of the AI Engine partitions (independent graphs) flow:

>Note: The following image is not the exact design of the tutorial 

![AIE_partitions](./images/AIE_partitions.PNG)

For an overview of the concepts and configuration details related to the AI Engine partition flow, refer to [UG1076: Compiling-AI-Engine-Graph-for-Independent-Partitions](https://docs.amd.com/r/en-US/ug1076-ai-engine-environment/Compiling-AI-Engine-Graph-for-Independent-Partitions).

For code examples and guidance on the partition reload process, see [UG1076: Programming-the-PS-Host-Application](https://docs.amd.com/r/en-US/ug1076-ai-engine-environment/Programming-the-PS-Host-Application).

This tutorial includes reference designs tailored for specific use cases:

* **Compiling AI Engine Graphs for Independent Partitions**: Please refer to [Comple Independent Graphs](./independent_graphs.md). The corresponding reference design is located in [independent_graphs](./independent_graphs).

* **Reload partitions with the same graphs**: Please refer to [Partition Reload](./partition_reload.md). The corresponding reference design is located in [partition_reload_same_graph](./partition_reload_same_graph).

* **Reload partitions with different graphs**: Please refer to [Partition Reload](./partition_reload.md). The corresponding reference design is located in [partition_reload_diff_graph](./partition_reload_diff_graph).

* **Reload the entire AI Engine array as a signle partition**: Please refer to [Partition Reload](./partition_reload.md). The corresponding reference design is located in [AIE_reload_whole_array](./AIE_reload_whole_array).

## Summary

By completing this tutorial, you will have learned how to:

* Configure the compiler for building AI Engine partitions.

* Use the V++ linker and packager to integrate multiple AI Engine partitions.

* Implement host code to manage and control different AI Engine partitions in hardware.

* Apply partition reloading techniques using recommended host code practices.

### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums](http://forums.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2022–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>

