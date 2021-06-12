﻿<table width=100%>
 <tr width=50%>
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h2>2020.2 Vitis™ - The Traveler Salesman Problem - Tutorial</h2>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# The Travelling Salesman Problem

## Introduction

In this tutorial we look into the Travelling Salesman Problem (TSP) which is a classic “NP-hard problem.” That is, in computational complexity theory, NP-hardness defines a class of problems that are informally "at least as hard as the hardest problems in NP.” For more information refer to this [Wikipedia Travelling Salesman Problem article](https://en.wikipedia.org/wiki/Travelling_salesman_problem).

To ensure the shortest route, we must test each possible combination of cities. So, the time complexity of the algorithm scales with the factorial of the number of cities, ~0(n!). For instance, with 13 cities, there are *13! = 6.2 billion routes* to compare. 

Then each route requires 12 memory lookups to calculate the distance, we need 12 * 6.2 = *75 billion memory lookups* to identify the shortest route.

## Run a simple CPU Implementation
A simple CPU implementation in the CPU_POC directory.
Notice how much time it takes for N=13 cities when running on the CPU. 

Build and run in a terminal with:
```bash
cd CPU_POC
g++ -O3 main_gold.cpp && ./a.out
```
The execution could take over a minute for 13 cities depending on your CPU, and we will see in this tutorial how an FPGA can solve the problem in less than a second...
### Setup Vitis
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

Complete this lab in the following order:
 
* [Load the Vitis HLS project](./project.md)
* [Understand the design structure](./design.md)
* [Run the C simulation](./csim.md)
* [Run the C synthesis](./csynth.md)
* [Run the RTL/C cosimulation](./cosim.md)
* [Export the design and evaluate performance in Vivado](./export.md)
* [Improved performance with 4 parallel distance lookups](./code_opt.md)
