<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Runtime Parameter Reconfiguration

***Version: Vitis 2022.1***

## Introduction

This tutorial is designed to demonstrate how the runtime parameters (RTP) can be changed during execution to modify the behavior of AI Engine kernels. Both scalar and array parameters are supported.

**IMPORTANT**: Before beginning the tutorial make sure you have read and followed the *Vitis Software Platform Release Notes* (v2022.1) for setting up software and installing the VCK190 base platform. 

Before starting this tutorial run the steps below:

1. Set up your platform by running the `xilinx-versal-common-v2022.1/environment-setup-cortexa72-cortexa53-xilinx-linux` script as provided in the platform download. This script sets up the `SYSROOT` and `CXX` variables. If the script is not present, you _must_ run `xilinx-versal-common-v2022.1/sdk.sh`.
2. Set up your ROOTFS to point to `xilinx-versal-common-v2022.1/rootfs.ext4`. 
3. Set up your IMAGE to point to `xilinx-versal-common-v2022.1/Image`.
4. Set up your `PLATFORM_REPO_PATHS` environment variable based upon where you downloaded the platform.

This tutorial targets the VCK190 production board. The `PLATFORM` variable in the Makefile(s) is as follows: 

```
   PLATFORM = ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_202210_1/xilinx_vck190_base_202210_1.xpfm
```


## Objectives

After completing this tutorial, you will be able to:

* Specify a scalar or array parameter as part of a kernel function signature.
* Connect a parameterized kernel into a graph, exposing the parameter for runtime updates.
* Simulate a graph containing runtime parameters with AI Engine simulator (aiesimulator).
* Build an system with AI Engine kernels and PL kernels, plus PS code to control their execution.
* Use Adaptive Data Flow (ADF) API or XRT API (C or C++) to control graph execution and RTP operations.
* Use OpenCL API or C version XRT API or C++ version XRT API to control PL kernel execution.
* Verify the system by HW co-simulation and running in hardware.

## Steps

**Step 1**: Integrate a kernel with a scalar runtime parameter into a graph. Demonstrate how to use OpenCL API to control PL kernels execution. See details in [Synchronous Update of Scalar RTP](./step1_sync_scalar.md).

**Step 2**: Mark the runtime parameter for asynchronous updates and observe the effect this has on a simulation. See details in [Asynchronous Update of Scalar RTP](./step2_async_scalar.md).

**Step 3**: Design a filter and change the array of filter coefficients at runtime, observing a change in the filter behavior. Use OpenCL API to control PL kernels exectuion. See details in [Asynchronous Update of Array RTP](./step3_async_array.md).

**Step 4**:  Demonstrate how to control graph execution by ADF API and XRT API (C version). And how to use the XRT API (C Version) to control the PL kernels. See details in [Asynchronous Update of Array RTP for AI Engine Kernel](./step4_async_aie_array.md).

**Step 5**: Update the AI Engine kernel with an asynchronous RTP `inout` port. Demonstrate how to use the Adaptive Data Flow (ADF) API and the XRT API (C++ Version) to control graph execution and RTP reads. Use the C++ version of the XRT API to control the PL kernels. See details in [Asynchronous Array RTP Update and Read for AI Engine Kernel](./step5_async_array_update_read.md).

__Note:__ In this tutorial, a Makefile is provided. If make commands exist, you just need to run them. Detailed commands are also shown to better illustrate the concepts. You can run these commands manually.

__Hint:__ In this tutorial, the designs are self-contained in each step. You can choose to start at any step depending on your experience and requirements. Be aware that the concepts and options introduced in the previous step might not be repeated later. It is highly recommended to start from the beginning and progress to completion.

==========

# Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

# License

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.

You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )


Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center"><sup>XD001 | &copy; Copyright 2020-2022 Xilinx, Inc.</sup></p>

==========

