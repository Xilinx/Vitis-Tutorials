</table>
<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# AI Engine Performance and Deadlock Analysis Tutorial

***Version: Vitis 2022.1***

## Introduction

This tutorial introduces you to performance analysis and optimization methods, and shows you how synchronization works in graph execution. It also demonstrates the analysis of a hang issue using an example.

## Before You Begin

**IMPORTANT**: First, set up the Vitis™ unified software platform and install the VCK190 base platform. Follow the instructions in the *Vitis Software Platform Release Notes* (v2022.1).

Before you begin this tutorial, run the following steps:

1. Set up your platform by running the `xilinx-versal-common-v2022.1/environment-setup-cortexa72-cortexa53-xilinx-linux` script as provided in the platform download. This script sets up the `SYSROOT` and `CXX` variables. If the script is not present, you _must_ run `xilinx-versal-common-v2022.1/sdk.sh`.
2. Set up your ROOTFS to point to `xilinx-versal-common-v2022.1/rootfs.ext4`. 
3. Set up your IMAGE to point to `xilinx-versal-common-v2022.1/Image`.
4. Set up your `PLATFORM_REPO_PATHS` environment variable based on where you downloaded the platform.

This tutorial targets the VCK190 production board. The `PLATFORM` variable in the Makefile(s) is as follows:

	PLATFORM = ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_202210_1/xilinx_vck190_base_202210_1.xpfm


### Objectives

After completing this tutorial, you will understand how to:

- Read lock signals in Vitis Analyzer.
- Profile design performance.
- Identify a performance bottleneck.
- Analyze a deadlock (hang issue) in the design.
- Analyze AI Engine status using Vitis Analyzer.

## Steps

**Step 1**: Learn about the execution model of an AI Engine graph, including how memory with lock and FIFO works. Learn performance measurement methods. Identify performance bottlenecks and practise optimization. Go to [AI Engine Graph Execution and Measurement](./aie_execution_measurement.md).

**Step 2**: Learn about how a deadlock might occur with an example design. Learn how to analyze a deadlock in the AI Engine simulator, hardware emulation, and hardware flows. Go to [AI Engine Deadlock Analysis](./aie_hang_analysis.md).

**Step 3**: Learn about AI Engine status analysis in the hardware and hardware emulation flows using Vitis Analyzer. Go to [AI Engine Status Analysis](./aie_status_analysis.md).


**Tip:** In this tutorial, the designs are self contained in each step. You can choose to start at any step depending on your experience and requirements. Be aware that the concepts and options introduced in the previous step might not be repeated later. It is recommended to start from the beginning and progress through to completion.

# Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

# License

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.

You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )


Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center"><sup>XD051 | &copy; Copyright 2020-2022 Xilinx, Inc.</sup></p>