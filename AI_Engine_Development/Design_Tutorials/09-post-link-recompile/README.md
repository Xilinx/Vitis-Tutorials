<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>Post-Link Recompile of an AI Engine Application</h1>
   </td>
 </tr>

</table>

# Introduction

AI Engine application development can start early in the system development stage. Little by little, the AI Engine development team and the Hardware development team converge to an interface between the Adaptable Engine and the AI Engine array. At some point, this interface is fixed and should not be changed. This is to avoid a complete Vivado® tool run, which can take a long time if timing closure requires specific attention.

Running the AI Engine tools provides a fixed platform that acts as a constraint on the AI Engine array – Adaptable Engine interface specification. This tutorial explains the complete flow, which comprises the following steps:

1. Using the AI Engine tools with an extensible platform.
2. Using the fixed platform generated in phase 1 and modifying the AI Engine application.

This tutorial details all the steps to perform hardware emulation (**hw_emu**), but it supports also the hardware implementation flow by changing the compilation target.


# Before You Begin

This tutorial requires a number of tools and files to be installed:

* Download and install the Vitis™ software platform from [here](https://www.xilinx.com/support/download.html). See the installation instructions [here](https://www.xilinx.com/html_docs/xilinx2021_2/vitis_doc/acceleration_installation.html).
* Download and install the Vitis Embedded Base Platform [VCK190](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html).
* Download and install the [common image](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html) for embedded Vitis platforms for Versal™ ACAP.

The AI Engine development documentation is also available [here](https://www.xilinx.com/html_docs/xilinx2021_2/vitis_doc).


## Accessing the Tutorial Reference Files

1. To access the reference files, type the following into a terminal: `git clone https://github.com/Xilinx/Vitis-Tutorials`.
2. Navigate to the `PostLinkRecompile` directory.

The sub-directory `Files` contains all the source files necessary for this tutorial.

A `setup.sh` script is provided to initialize the Vitis tool and set the platform repository. Modify it accordingly for your install.

# AI Engine Application Post-Link Recompile

There are two labs in this tutorial:

- A [Vitis IDE](VitisIDE_Flow.md)-based lab.
- A [Vitis Makefile](VitisMakefile_Flow.md)-based lab.

Each lab is divided in two phases:

1. Creating an AI Engine application using the VCK190 platform with all necessary PL kernels added, and linking the complete system.
2. Creating a new AI Engine application with the previously created fixed platform, adding a PS application, and running hardware emulation.


# Objectives

This tutorial shows you how to modify an AI Engine application after the platform has been frozen. It demonstrates a Vitis IDE flow and a Makefile flow. 

Completing this tutorial should give you confidence to develop AI Engine designs based on a hardware/software platform, knowing that you can modify the AI Engine part and the processing system control application. The only limitation is that the hardware connection between the AI Engine array and the programmable logic (PL) must be kept fixed.


---

&copy; Copyright 2021 Xilinx, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

```
    http://www.apache.org/licenses/LICENSE-2.0
```

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


<p align="center"><sup>Copyright&copy; 2021 Xilinx</sup><br><sup>XD039</sup></br></p>
