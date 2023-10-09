<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Runtime Parameter Reconfiguration

***Version: Vitis 2023.1***

## Introduction

This tutorial is designed to demonstrate how the runtime parameters (RTP) can be changed during execution to modify the behavior of AI Engine kernels. Both scalar and array parameters are supported.

>**IMPORTANT**: Before beginning the tutorial make sure you have installed the AMD Vitis™ 2023.1 software. The Vitis release includes all the embedded base platforms including the VCK190 base platform that is used in this tutorial. In addition, ensure that you have downloaded the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html).

The ‘common image’ package contains a prebuilt Linux kernel and root file system that can be used with the AMD Versal™ board for embedded design development using Vitis tools.

Before starting this tutorial, run the following steps:

1. Go to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the ``/Common Images Dir/xilinx-versal-common-v2023.1/environment-setup-cortexa72-cortexa53-xilinx-linux`` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you must run the ``/Common Images Dir/xilinx-versal-common-v2023.1/sdk.sh``.
3. Set up your ROOTFS and IMAGE to point to the `rootfs.ext4` and Image files located in the ``/Common Images Dir/xilinx-versal-common-v2023.1`` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to ``$XILINX_VITIS/base_platforms``.

This tutorial targets the VCK190 production board for the 2023.1 version.

## Objectives

After completing this tutorial, you will be able to:

* Specify a scalar or array parameter as part of a kernel function signature.
* Connect a parameterized kernel into a graph, exposing the parameter for runtime updates.
* Simulate a graph containing runtime parameters with AI Engine simulator (aiesimulator).
* Build a system with AI Engine kernels and PL kernels, plus PS code to control their execution.
* Use XRT API to control graph execution and RTP operations.
* Use XRT API to control PL kernel execution.
* Verify the system by HW co-simulation and running in hardware.

## Steps

**Step 1**: Integrate a kernel with a synchronous scalar runtime parameter into a graph. See details in [Synchronous Update of Scalar RTP](./step1_sync_scalar.md).

**Step 2**: Mark the runtime parameter for asynchronous updates and observe the effect this has on a simulation. See details in [Asynchronous Update of Scalar RTP](./step2_async_scalar.md).

**Step 3**: Design a filter, change the array of filter coefficients at runtime, and observe a change in the filter behavior. See details in [Asynchronous Update of Array RTP](./step3_async_array.md).

**Step 4**: Control graph execution using the XRT API. See details in [Asynchronous Update of Array RTP for AI Engine Kernel](./step4_async_aie_array.md).

**Step 5**: Update the AI Engine kernel with an asynchronous RTP ``inout`` port and learn how to use the XRT API to control graph execution and RTP reads. See details in [Asynchronous Array RTP Update and Read for AI Engine Kernel](./step5_async_array_update_read.md).

>**Note:** In this tutorial, a Makefile is provided. If `make` commands exist, you can run them. Detailed commands are also shown to better illustrate the concepts. You can run these commands manually.

>**Hint:** In this tutorial, the designs are self-contained in each step. You can choose to start at any step depending on your experience and requirements. Be aware that the concepts and options introduced in the previous step might not be repeated later. It is highly recommended to start from the beginning and progress to completion.

### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums](http://forums.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
