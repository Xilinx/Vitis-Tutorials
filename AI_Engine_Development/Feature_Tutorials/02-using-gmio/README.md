<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# GMIO Tutorial

***Version: Vitis 2022.1***

## Introduction

A GMIO port attribute is used to make external memory-mapped connections to or from the global memory. These connections are made between AI Engine kernels or programmable logic kernels and the logical global memory ports of a hardware platform design. This tutorial is designed to demonstrate how to work with the GMIO interface in AI Engine simulator and hardware flows.

**IMPORTANT**: Before beginning the tutorial make sure you have read and followed the *Vitis Software Platform Release Notes* (v2022.1) for setting up software and installing the VCK190 base platform.

Before starting this tutorial, run the following steps:

1. Set up your platform by running the `xilinx-versal-common-v2022.1/environment-setup-cortexa72-cortexa53-xilinx-linux` script as provided in the platform download. This script sets up the `SYSROOT` and `CXX` variables. If the script is not present, you **must** run the `xilinx-versal-common-v2022.1/sdk.sh`.
2. Set up your ROOTFS to point to the xilinx-versal-common-v2022.1/rootfs.ext4
3.	Set up your IMAGE to point to xilinx-versal-common-v2022.1/Image.
4. Set up your `PLATFORM_REPO_PATHS` environment variable based upon where you downloaded the platform.

This tutorial targets VCK190 production board, PLATFORM variable in the Makefile(s) is: 

	PLATFORM = ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_202210_1/xilinx_vck190_base_202210_1.xpfm

## Objectives

After completing this tutorial, you will be able to:

* Understand the programming model and software programmability of the AI Engine GMIO.
* Profile GMIO design in AI Engine simulator and improve performance.
* Profile GMIO performance by event trace API or Linux system timer.
* Measure the NOC bandwidth and make trade offs between GMIO and PLIO.

## Steps
> **Note**: This tutorial assumes that the user has basic understanding of Adaptive Data Flow (ADF) API and Xilinx® Runtime (XRT) API usage. For more information about ADF API and XRT usage, refer to AI Engine Runtime Parameter Reconfiguration Tutorial and Versal ACAP AI Engine Programming Environment User Guide ([UG1076]).

**Step 1 - AI Engine GMIO**: Introduces the programming model of AI Engine GMIO, including blocking and non-blocking GMIO transactions. See details in [AIE GMIO Programming Model](./single_aie_gmio.md).

**Step 2 - AI Engine GMIO Performance Profile**: Introduces ways to do performance profile of AI Engine GMIO. Learn about the maximum supported GMIO ports and NOC bandwidth. See details in [AIE GMIO Performance Profile](./perf_profile_aie_gmio.md).

> **Note**: In this tutorial, a Makefile is provided. If make commands exist, you just need to run them. Detailed commands are also shown to better illustrate the concepts. You can run these commands manually.

> **Note**: In this tutorial, the designs are self-contained in each step. You can choose to start at any step depending on your experience and requirements. Be aware that the concepts and options introduced in the previous step might not be repeated later. It is highly recommended to start from the beginning and progress to completion.

<br></br>

Licensed under the Apache License, Version 2.0 (the "License");

you may not use this file except in compliance with the License.

You may obtain a copy of the License at



    http://www.apache.org/licenses/LICENSE-2.0



Unless required by applicable law or agreed to in writing, software

distributed under the License is distributed on an "AS IS" BASIS,

WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and

limitations under the License.



<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020–2021 Xilinx</sup><br><sup>XD007</sup></br></p>
