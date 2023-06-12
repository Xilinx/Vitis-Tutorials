<table class="sphinxhide" width="100%">
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Application Acceleration Tutorials</h1>

   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

<!-- 
#/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#*/
-->

# Vitis HLS Analysis and Optimization

***Version: Vitis 2023.1***

## Introduction

Vitis High-Level Synthesis (HLS) is a key part of the Vitis application acceleration development flow. The tool is responsible for compiling C/C++ and OpenCL code into a kernel for acceleration in the programmable logic (PL) region of AMD devices. Thus, it is the tool that compiles the hardware kernels for the Vitis tools by performing high-level synthesis.

> **TIP:** Vitis HLS can also be used to generate Vivado IP from C/C++ code, but that flow is not the subject of this tutorial. Although similar, there are some significant differences between producing Vitis XO kernels and Vivado RTL IP. However, you can use this tutorial as a general introduction to the Vitis HLS tool. 

This tutorial contains two flows that you can work through: 
* The Vitis HLS tool GUI flow to build, analyze, and optimize a hardware kernel. For more information, refer to [Vitis HLS Flow Overview](https://docs.xilinx.com/r/en-US/ug1399-vitis-hls/Vitis-HLS-Flow-Overview).
* The new Vitis unified IDE flow which includes the bottom-up development of HLS designs as described in the *Vitis Unified IDE and Common Command-Line Reference Guide* ([UG1553](https://docs.xilinx.com/r/en-US/ug1553-vitis-ide)).


### Before You Begin

The labs in this tutorial use:

* BASH Linux shell commands.
* 2023.1 Vitis core development kit release.

>**IMPORTANT:**  
>
> * Before running any of the examples, make sure you have installed the Vitis core development kit as described in [Installation](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Installation-Requirements) in the Application Acceleration Development flow of the Vitis Unified Software Platform Documentation (UG1416).
>* If you run applications on the AMD Alveo™ Data Center accelerator cards, ensure the card and software drivers have been correctly installed by following the instructions To complete installation, follow the instructions on the [Alveo Product Documentation tab](https://www.xilinx.com/products/boards-and-kits/alveo.html).

### Setup the environment to run Vitis

To configure the environment to run Vitis, run the following scripts which set up the environment to run in a specific command shell.

```bash
source <Vitis_install_path>/Vitis/2023.1/settings64.sh
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

For more information see [AMD AR 73698](https://www.xilinx.com/support/answers/73698.html).

### Accessing the Tutorial Reference Files

1. To access the reference files, type the following into a terminal: `git clone https://github.com/Xilinx/Vitis-Tutorials`.
2. Navigate to the `Getting_Started/Vitis_HLS` directory, and then access the `reference-files` directory.

## Next Steps

This tutorials presents two different tool flows for users of high-level synthesis. The first is the new Vitis unified IDE for creating an HLS component; the second is the existing Vitis HLS tool. You can choose your path by selecting one of the following:

1.  [Using the new Vitis unified IDE](./unified_ide_project.md)
2.  [Using the classic Vitis HLS GUI](./new_project.md)

<!--

1. [Creating the Vitis HLS Project](./new_project.md) - Create the project to specify the source code and testbench.

2. [Running High-Level Synthesis and Analyzing Results](./synth_and_analysis.md) - Simulate and synthesize the design, and analyze the results.

3. [Using Optimization Techniques](./optimization_techniques.md) - Try different optimization techniques to achieve the initiation interval (II)=1.

4. [Reviewing the Dataflow Optimization](./dataflow_design.md) - Add the Dataflow optimization to achieve even better results.

-->

</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="./README.md">Return to Main Page</a> — <a href="./../README.md">Return to Getting Started Pathway</a></b></p>


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>

