<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>AI Engine Packet Switching Tutorial</h1>
   </td>
 </tr>
 <tr>
 </td>
 </tr>
</table>

## Introduction
AI Engine kernels can share a single processor and execute in a interleaved manner. In addition they can also share multiple stream connections on a single physical channel. The Explicit Packet Switching feature allows fine-grain control over how packets are generated, distributed, and consumed in a graph computation. Explicit packet switching is typically recommended in cases where many low bandwidth streams from a common PL source can be distributed to different AI Engine destinations. Similarly many low bandwidth streams from different AI Engine sources to a common PL destination can also take advantage of this feature. Because a single physical channel is shared between multiple streams, you minimize the number of AI Engine - PL interface streams used.

This tutorial walks you through the steps to create window interface AI Engine kernels that share the same AI Engine - PL interface streams (step 1), to create designs with float and `cint16` data types (step 2), and to create packet stream interface AI Engine kernels that share AI Engine - PL interface streams (step 3). The AI Engine kernels are different in these steps. In particular, packet stream interfaces and associated built-in functions are introduced in step 3. The graph construction is covered in detail in step 1 and step 3 (only with differences). All these designs share the same PL kernels, which are introduced in step 1. The PS codes for those steps are similar, which is coverred in detail in step 1 and step 2 (only with differences). 

There is a limitation in the current version of the AI Engine tools that only integer format values are supported in data files for the AI Engine simulator. Details on the data format and how to convert data from other types to integer format are covered in steps 1 and 2.

__Note:__ In this tutorial, the variable "PLATFORM_REPO_PATHS" is used to find the platform installation path. Set this variable appropriately or set paths to your installation for all Makefiles.

This tutorial targets the VCK190 ES board (see https://www.xilinx.com/products/boards-and-kits/vck190.html). This board is currently available via early access. If you have already purchased this board, download the necessary files from the lounge and ensure you have the correct licenses installed. If you do not have a board and ES license please contact your Xilinx sales contact.

## Objectives
After completing this tutorial, you will be able to:

* Construct a packet switching graph
* Understand the packet format for packet switching
* Write an AI Engine kernel that handles packet stream explicitly
* Design PL kernels for packet switching
* Write a PS application for hardware and hardware emulation flows

## Steps
**Step 1**: Construct an explicit packet switching graph with window interface AI Engine kernels. PL kernels and PS code for the system design are also introduced. See details in [Window-Based AI Engine Kernels](./window_based_aie_kernel.md). 

**Step 2**: Special consideration on float and `cint*` data types. See details in [Window-Based AI Engine Kernels with Mixed Data Types](./window_based_mix_data_type.md).

**Step 3**: Introduce the packet stream interface and built-in functions for AI Engine kernels, which allows you to fine control how packets are decoded and constructed. See details in [Packet Stream-Based AI Engine Kernels](./pktstream_based_aie_kernel.md).

__Note:__ In this tutorial, a Makefile and instructions are provided. For detailed information about AI Engine tools commands, see the [AI Engine Documentation](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/yii1603912637443.html).

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
