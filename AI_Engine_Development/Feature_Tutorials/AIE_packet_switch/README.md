<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>AI Engine Packet Switching Tutorial</h1>
   </td>
 </tr>
 <tr>
 </td>
 </tr>
</table>

## Introduction
The packet switching feature allows multiple stream connections to share the same physical channel. Explicit Packet Switching is typically recommended in cases where many low bandwidth streams from a common PL source need to be distributed to different AI Engines. Similarly, many low bandwidth streams from different AI Engines going to a common PL destination can also take advantage of this feature. After completing this tutorial, you will be able to:

* Construct a packet switching graph
* Understand the packet format for packet switching
* Write an AI Engine kernel that handles packet stream explicitly
* Design PL kernels for packet switching
* Write a PS application for hardware and hardware emulation flows

This tutorial targets the VCK190 ES board (see https://www.xilinx.com/products/boards-and-kits/vck190.html). This board is currently available via early access. If you have already purchased this board, download the necessary files from the lounge and ensure you have the correct licenses installed. If you do not have a board and ES license please contact your Xilinx sales contact.

## General flow
**Step 1**: Construct an explicit packet switching graph with window interface AI Engine kernels. See details in [Window Based AI Engine Kernels](./window_based_aie_kernel.md). 

**Step 2**: Special consideration on float and `cint*` data types. See details in [Window Based AI Engine Kernels wiht Mix Data Type](./window_based_mix_data_type.md).

**Step 3**: Introduce the packet stream interface and built-in functions for AI Engine kernels. See details in [Packet Stream Based AI Engine Kernels](./pktstream_based_aie_kernel.md).

__Note:__ In this tutorial, a Makefile and instructions are provided. For detailed information about AI Engine tools commands, see the [AI Engine Documentation](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/yii1603912637443.html).

__Note:__ In this tutorial, the variable "PLATFORM_REPO_PATHS" is used to find the platform installation path. Set this variable appropriately or set paths to your installation for all Makefiles.

__Hint:__ In this tutorial, the designs are self-contained in each step, but the steps refer to previous steps. Therefore, it is highly recommended to start from the beginning and progress to completion.

© Copyright 2020 Xilinx, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

<p align="center"><sup>XD029</sup></p>
