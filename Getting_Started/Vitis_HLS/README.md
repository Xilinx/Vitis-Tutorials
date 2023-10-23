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

***Version: Vitis 2023.2***

## Introduction

Vitis High-Level Synthesis (HLS) lets you compile C/C++ code into RTL code for implementation in the programmable logic (PL) region of AMD devices. It is the tool that performs high-level synthesis of the C/C++ code and exports it as either a Vivado IP (.zip) for use in the Vivado Design Suite and the Embedded Software development flow, or as a Vitis kernel (.xo) for use in the Vitis acceleration flow and heterogeneous system designs. Although the Vitis kernel and Vivadp IP flows are similar, there are some fundamental differences as explained in [Introduction to Vitis HLS Components](https://docs.xilinx.com/access/sources/dita/topic?Doc_Version=2023.2%20English&url=ug1399-vitis-hls&resourceid=tgc1661815832870). This tutorial demonstrates the Vitis unified IDE flow for the bottom-up development of HLS designs.


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

This tutorials presents [Using the Vitis unified IDE](./unified_ide_project.md) for creating an HLS component:

<!--

1. [Creating the Vitis HLS Project](./unified_ide_project.md) - Create the project to deine the HLS component.

2. [Running High-Level Synthesis and Analyzing Results](./unified-synth_and_analysis.md) - Simulate and synthesize the design, and analyze the results.

3. [Using Optimization Techniques](./unified-optimization_techniques.md) - Try different optimization techniques to achieve the initiation interval (II)=1.

4. [Reviewing the Dataflow Optimization](./dataflow_design.md) - Add the Dataflow optimization to achieve even better results.

-->

</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="./README.md">Return to Main Page</a> — <a href="./../README.md">Return to Getting Started Pathway</a></b></p>


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>

