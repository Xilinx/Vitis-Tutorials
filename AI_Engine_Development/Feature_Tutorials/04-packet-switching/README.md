<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Packet Switching Tutorial

***Version: Vitis 2022.1***

AI Engine kernels can share a single processor and execute in an interleaved manner. In addition, they can also share multiple stream connections on a single physical channel. The explicit packet switching feature allows fine-grain control over how packets are generated, distributed, and consumed in a graph computation. Explicit packet switching is typically recommended in cases where many low bandwidth streams from a common programmable logic (PL) source can be distributed to different AI Engine destinations. Similarly many low bandwidth streams from different AI Engine sources to a common PL destination can also take advantage of this feature. Because a single physical channel is shared between multiple streams, you minimize the number of AI Engine - PL interface streams used.

This tutorial walks you through the steps to create window interface AI Engine kernels that share the same AI Engine - PL interface streams (step 1), to create designs with float and `cint16` data types (step 2), and to create packet stream interface AI Engine kernels that share AI Engine - PL interface streams (step 3). The AI Engine kernels are different in these steps. In particular, packet stream interfaces and associated built-in functions are introduced in step 3. The graph construction is covered in detail in step 1 and step 3 (only with differences). All these designs share the same PL kernels that are introduced in step 1. The PS codes for these steps are similar; this is covered in detail in step 1 and step 2 (with differences). 

There is a limitation in the current version of the AI Engine tools that only integer format values are supported in data files for the AI Engine simulator. Details on the data format and how to convert data from other types to integer format are covered in steps 1 and 2.

**IMPORTANT**: Before beginning the tutorial make sure you have read and followed the *Vitis Software Platform Release Notes* (v2022.1) for setting up software and installing the VCK190 base platform. 

Before starting this tutorial run the following steps.

1. Set up your platform by running the `xilinx-versal-common-v2022.1/environment-setup-cortexa72-cortexa53-xilinx-linux` script as provided in the platform download. This script sets up the `SYSROOT` and `CXX` variables. If the script is not present, you _must_ run `xilinx-versal-common-v2022.1/sdk.sh`.
2. Set up your ROOTFS to point to the `xilinx-versal-common-v2022.1/rootfs.ext4`. 
3. Set up your IMAGE to point to `xilinx-versal-common-v2022.1/Image`.
4. Set up your `PLATFORM_REPO_PATHS` environment variable based upon where you downloaded the platform.

This tutorial targets the VCK190 production board. The `PLATFORM` variable in the Makefile(s) is as follows: 

	PLATFORM = ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_202210_1/xilinx_vck190_base_202210_1.xpfm


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

__Note:__ In this tutorial, a Makefile and instructions are provided. For detailed information about AI Engine tools commands, see the [AI Engine Documentation]([AI Engine Documentation](https://docs.xilinx.com/search/all?filters=Document_ID~%2522UG1076%2522_%2522UG1079%2522&content-lang=en-US).

__Hint:__ In this tutorial, the designs are self-contained in each step, but the steps refer to previous steps. Therefore, it is highly recommended to start from the beginning and progress to completion.


# Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

# License

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.

You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )


Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center"><sup>XD029 | &copy; Copyright 2020-2022 Xilinx, Inc.</sup></p>

