<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis™ Application Acceleration Tutorials</h1>
   <a href="https://github.com/Xilinx/Vitis-Tutorials/tree/2020.1">See 2020.1 Tutorials</a>
  </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

<!-- 
# Copyright 2021 Xilinx Inc.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
-->

# Vitis HLS Analysis and Optimization

## Introduction

Vitis High-Level Synthesis (HLS) is a key part of the Vitis application acceleration development flow. The tool is responsible for compiling C/C++ and OpenCL code into a kernel for acceleration in the programmable logic (PL) region of Xilinx devices. Thus, it is the tool that compiles the hardware kernels for the Vitis tools by performing high-level synthesis.

> **TIP:** Vitis HLS can also be used to generate Vivado IP from C/C++ code, but that flow is not the subject of this tutorial. Although similar, there are some significant differences between producing Vits XO kernels and Vivado RTL IP. However, you can use this tutorial as a general introduction to the Vitis HLS tool. 

In this tutorial, you will work through the Vitis HLS tool GUI to build, analyze, and optimize a hardware kernel. You are working through the Vitis kernel flow in the Vitis tool. For more information, refer to [Enabling the Vitis Kernel Flow](https://www.xilinx.com/cgi-bin/docs/rdoc?v=2020.2;t=vitis+doc;d=vitis_hls_process.html;a=uiy1584905571731) in the Vitis HLS Flow of the Vitis Unified Software Platform Documentation (UG1416).

### Before You Begin

The labs in this tutorial use:

* BASH Linux shell commands.
* 2020.2 Vitis core development kit release and the *xilinx_u200_xdma_201830_2* platform. If necessary, it can be easily ported to other versions and platforms.

>**IMPORTANT:**  
>
> * Before running any of the examples, make sure you have installed the Vitis core development kit as described in [Installation](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/acceleration_installation.html#vhc1571429852245) in the Application Acceleration Development flow of the Vitis Unified Software Platform Documentation (UG1416).
>* If you run applications on the Xilinx® Alveo™ Data Center accelerator cards, ensure the card and software drivers have been correctly installed by following the instructions To complete installation, follow the instructions on the [Alveo Product Documentation tab](https://www.xilinx.com/products/boards-and-kits/alveo.html).

### Setup the environment to run Vitis

To configure the environment to run Vitis, run the following scripts which set up the environment to run in a specific command shell.

```bash
source <Vitis_install_path>/Vitis/2020.2/settings64.sh
source /opt/xilinx/xrt/setup.sh
```

*NOTE: .csh scripts are also provided but this tutorial assumes a bash shell is used.*

To specify the location of any Data-Center or Embedded platforms you have installed, set the following environment variable:

```bash
export PLATFORM_REPO_PATHS=<path to platforms>
```

*NOTE: On some Ubuntu distributions, you must also export LIBRARY_PATH to properly set up Vitis.*

```bash
export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu
```

For more information see [Xilinx AR 73698](https://www.xilinx.com/support/answers/73698.html).

### Accessing the Tutorial Reference Files

1. To access the reference files, type the following into a terminal: `git clone https://github.com/Xilinx/Vitis-Tutorials`.
2. Navigate to the `Getting_Started/Vitis_HLS` directory, and then access the `reference-files` directory.

## Next Steps

Complete the labs in the following order:
 
* [Creating a Vitis HLS Project](./new_project.md)
* [Running High-Level Synthesis and Analyzing Results](./synth_and_analysis.md)
* [Using Optimization Techniques](./optimization_techniques.md)
* [Reviewing the DATAFLOW Optimization](./dataflow_design.md)

<!--

1. [Creating the Vitis HLS Project](./new_project.md) - Create the project to specify the source code and testbench.

2. [Running High-Level Synthesis and Analyzing Results](./synth_and_analysis.md) - Simulate and synthesize the design, and analyze the results.

3. [Using Optimization Techniques](./optimization_techniques.md) - Try different optimization techniques to achieve the initiation interval (II)=1.

4. [Reviewing the Dataflow Optimization](./dataflow_design.md) - Add the Dataflow optimization to achieve even better results.

-->

</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="./README.md">Return to Main Page</a> — <a href="./../README.md">Return to Getting Started Pathway</a></b></p>

<p align="center" class="sphinxhide"><sup>Copyright&copy; 2021 Xilinx</sup></p>
