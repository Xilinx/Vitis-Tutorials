<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>AI Engine GMIO Tutorial</h1>
   </td>
 </tr>
 <tr>
 </td>
 </tr>
</table>

## Introduction
A GMIO port attribute is used to make external memory-mapped connections to or from the global memory. These connections are made between AI Engine kernels or programmable logic kernels and the logical global memory ports of a hardware platform design. This tutorial is designed to demonstrate how to work with the GMIO interface in AI Engine simulator and hardware flows. It includes the AI Engine GMIO, as well as PL GMIO.

__Note:__ In this tutorial, the variable "PLATFORM_REPO_PATHS" is used to find the platform installation path. Set this variable appropriately.

This tutorial targets the VCK190 ES board (see https://www.xilinx.com/products/boards-and-kits/vck190.html). This board is currently available via early access. If you have already purchased this board, download the necessary files from the lounge and ensure you have the correct licenses installed. If you do not have a board and ES license please contact your Xilinx sales contact.

## Objectives
After completing this tutorial, you will be able to:
* Understand the programming model and software programmability of the AI Engine GMIO.
* Construct PL kernels inside graph and work with PL GMIO.
* Profile GMIO design in AI Engine simulator and improve performance.
* Profile GMIO performance by event trace API or Linux system timer.
* Measure the NOC bandwidth and make trade offs between GMIO and PLIO.

## Steps
__Note:__ This tutorial assumes that the user has basic understanding of Adaptive Data Flow (ADF) API and Xilinx® Runtime (XRT) API usage. For more information about ADF API and XRT usage, refer to AI Engine Runtime Parameter Reconfiguration Tutorial and Versal ACAP AI Engine Programming Environment User Guide (UG1076).

__IMPORTANT:__ GMIO is not supported in hardware emulation flow.

**Step 1 - AI Engine GMIO**: Introduces the programming model of AI Engine GMIO, including blocking and non-blocking GMIO transactions. See details in [AIE GMIO Programming Model](./single_aie_gmio.md).

**Step 2 - PL GMIO**: Introduces the programming model of PL GMIO, including the coding of PL kernels to make PL-to-DDR memory connections. See details in [PL GMIO Programming Model](./single_pl_gmio.md).

**Step 3 - AI Engine GMIO Performance Profile**: Introduces ways to do performance profile of AI Engine GMIO. Learn about the maximum supported GMIO ports and NOC bandwidth. See details in [AIE GMIO Performance Profile](./perf_profile_aie_gmio.md).

__Note:__ In this tutorial, a Makefile is provided. If make commands exist, you just need to run them. Detailed commands are also shown to better illustrate the concepts. You can run these commands manually.

__Hint:__ In this tutorial, the designs are self-contained in each step. You can choose to start at any step depending on your experience and requirements. Be aware that the concepts and options introduced in the previous step might not be repeated later. It is highly recommended to start from the beginning and progress to completion.


<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>

Licensed under the Apache License, Version 2.0 (the "License");

you may not use this file except in compliance with the License.

You may obtain a copy of the License at



    http://www.apache.org/licenses/LICENSE-2.0



Unless required by applicable law or agreed to in writing, software

distributed under the License is distributed on an "AS IS" BASIS,

WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and

limitations under the License.


<p align="center"><sup>XD007</sup></p>


