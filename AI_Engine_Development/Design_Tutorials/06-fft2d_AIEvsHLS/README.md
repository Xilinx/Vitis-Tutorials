<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Versal® ACAP AI Engine Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Versal 2D-FFT Implementation Using Vitis Acceleration Library Tutorial (XD073)

***Version: Vitis 2022.1***

## Table of Contents

[Introduction](#introduction)

[Before You Begin](#Before-you-Begin)

[Design Implementations](#Design-Implementations)

[AI Engine and HLS Implementation Comparison](#AI-Engine-and-HLS-Implementation-Comparison)

[References](#References)

[Known Issues](#Known-Issues)

# Introduction

The Versal® ACAP is a a fully software programmable, heterogeneous compute platform that combines the following:

- Scalar Engines (a processor subsystem (PS) that includes Arm® processors)
- Adaptable Engines (programmable logic (PL) and memory)
- Intelligent Engines (including both including both AI and DSP Engines)

This tutorial performs two implementations of a system-level design: one with AI Engine, and the other with HLS using the DSP Engines. In each implementation, the tutorial takes you through the hardware emulation and hardware flow in the context of a complete Versal ACAP system design.

A Makefile is provided for each implementation. It can be used to create the design for `cint16` and `cfloat` datatypes, for various point sizes (32 x 64, 64 x 128, 128 x 256, 256 x 512, and 1024 x 2048), different numbers of fft_2d HLS kernel/AI Engine graph instances (1, 5, and 10), and lastly for different targets (hw_emu and hw).

The design documentation demonstrates hardware and software design details including the methodology for each implementation, elaborating on the functional partitioning. The compilation, execution, and measurement steps as well as observations are given.

<details>
  <summary>Objectives</summary> 
	
## Objectives

After completing the tutorial, you should be able to:

* Develop a system-level 2D-FFT design by identifying an algorithm and deploying it on AI Engines or PL and DSP Engines using Vitis™ HLS. 
* Build a complete system design by going through the following steps in the Vitis flow:
  * Create the AI Engine Adaptive Data Flow API (ADF) graph.
  * Compile the A72 host application and compiling PL kernels.
  * Use the Vitis compiler (V++) to link the AI Engine and HLS kernels with the platform.
  * Package the design.
  * Run the design through the hardware emulation and hardware flow in a mixed SystemC/RTL cycle-accurate/QEMU-based simulator.
* Develop a consistent harness to have the data mover kernels maintain a similar interface with AI Engine or HLS/PL kernels (using AXI4-Stream).
* Understand graph control APIs for AI Engine implementation and HLS APIs for controlling HLS/PL kernels.
* Understand the methodological differences between a design created using AI Engines and a design created using PL and DSP Engines via HLS.
* Understand metrics including utilization, performance/throughput, and power across various instances of FFT arrays of different dimensions.

</details>

<details>
  <summary>Design Overview</summary> 
 
## Design Overview

This design aims to implement a 2D-FFT algorithm performed on (for example) a 1024 x 2048 matrix using 1024- and 2048-point 1D-FFT kernels. The pseudo-code to implement the algorithm is shown in the following example:

```
x = rand(a,b);         % Create matrix of dimensions m x n

x_2fft = fft2(x);      % Reference generation, 2 dimensional FFT of matrix 'x'

FFT2D, using 1D – FFT.
x_row = fft(x);        % Row wise 1D-FFT
x_row = x_row.’;       % Transpose
x_col = fft(x_row);    % Row wise 1D-FFT
x_col = x_col.’;       % Transpose

x_2fft - x_col         % Calculate the error difference
```

In calculating each 1D-FFT, one dimension is kept constant while the other is computed. The transpose function is applied after each 1D-FFT compute. The transpose function moves the entry along each element of the dimension to the corresponding element of the other dimension. A golden data set is generated as reference and the error difference is calculated.

A similar algorithm is deployed in the two implementations using either the AI Engines or HLS targeting the PL and DSP Engines. The design compiles through Vitis compiler, creates a PetaLinux-based platform using a script, and generates the PDI and host application. Instead of the transpose part, however, a PL-based data generator and checker (referred to as a data mover) is used to give an impulse (value = 1 or 1.5, depending on the cint16 or cfloat datatype) input to the row-wise 1D-FFT and check its output against the expected FFT output for the same (the first row containing all 1s and remaining 0s).

The transposed pattern of that, generated within the PL, is then streamed as input to the col-wise 1D-FFT. Its output is then checked against the expected output (all 1s). The data mover kernel returns the total error count in both stages to the host application, which is used to declare a pass or fail of the test case.

To help you compare the methodology of the AI Engine and HLS based implementations, the design build process can be modified to analyze how each implementation scales with different numbers of instances (x1, x5, and x10) of various matrix array dimensions (rows x cols = 32 x 64, 64 x 128, 128 x 256, 256 x 512, and 1024 x 2048). 

A similar set of harnesses is developed and maintained between the two implementations. This allows you to generate and check input/output vectors using the PL-based data mover kernels, and to move data to and from the AI and HLS kernels. In both cases, Xilinx Runtime (XRT) running on A72 controls data flow in compute and data mover kernels through graph control APIs. These graph control APIs control the AI Engine kernels and HLS APIs, which in turn control the HLS/PL kernels.

</details>

<details>
  <summary>Directory Structure</summary> 
	
## Directory Structure

```
fft2d_AIEvsHLS
|__sample_env_setup.sh......contains sample template for setting up the environment for running the designs
|__AIE......................contains AI Engine implementation
|    |Makefile....................with recipes for each step of the design compilation
|    |images......................contains images used for AI Engine Design documentation
|    |description.json............required for XOAH
|    |build.......................created while execution and contains subfolders from design build
|    |design......................contains source and include files
|    |       |aie_src....................contains all the aie source files
|    |       |pl_src.....................contains all the data mover source files
|    |       |host_app_src...............contains host application source files
|    |       |system_configs.............contains all system configuration files
|    |       |profiling_configs..........contains xrt.ini file
|    |       |hw_emu_files...............contains hw_emu launch script
|__HLS......................contains HLS implementation targeting PL and DSP Engines
|    |Makefile....................with recipes for each step of the design compilation
|    |images......................contains images used for DSP Design documentation
|    |description.json............required for XOAH
|    |build.......................created while execution and contains subfolders from design build
|    |design......................contains source and include files
|    |       |pl_src.....................contains all the fft_2d and data mover source files
|    |       |host_app_src...............contains host application source files
|    |       |system_configs.............contains all system configuration files
|    |       |profiling_configs..........contains xrt.ini file
|    |       |directives.................contains directives for various vitis compilation stages like hls.pre_tcl etc.
|    |       |hw_emu_files...............contains hw_emu launch script
```
</details>

# Before You Begin

<details>
	
<summary>Documentation: Explore AI Engine Architecture</summary> 

## Documentation: Explore AI Engine Architecture

* [AI Engine Development Design Process](https://www.xilinx.com/support/documentation-navigation/design-process/ai-engine-development.html)

* [AM009 AI Engine Architecture Manual](https://www.xilinx.com/support/documentation/architecture-manuals/am009-versal-ai-engine.pdf)

* [Versal ACAP AI Engines for Dummies](https://forums.xilinx.com/t5/Design-and-Debug-Techniques-Blog/Versal-ACAP-AI-Engines-for-Dummies/ba-p/1132493)

</details>

<details>
<summary>Installing the Tools</summary> 
	
## *Tools*: Installing the Tools

Tools Documentation:

* [Versal AI Engines Secure Site](https://www.xilinx.com/member/forms/registration/versal_ai_engines.html#documentation)

* [AI Engine Documentation](https://docs.xilinx.com/search/all?filters=Document_ID~%2522UG1076%2522_%2522UG1079%2522&content-lang=en-US)

To build and run the FIR filter tutorial (AI Engine and DSP implementations), you will need the following tools downloaded/installed:

* [DSP Library (DSPLib) Documentation](https://xilinx.github.io/Vitis_Libraries/dsp/2022.1/index.html)

* Download and set up the [VCK190 Vitis Platform for 2022.1](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html)
* Install the [Vitis Platform 2022.1](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Installation)
* Installing the [Vitis Embedded Platforms 2022.1](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Installing-Embedded-Platforms?tocId=dtznqDN8RSf91SuduQDZ2A)
* [Platforms Repository 2022.1](https://github.com/Xilinx/Vitis_Embedded_Platform_Source/tree/2022.1)

* [Get Versal Commom Image, comman target and sysroot lincenses and sources](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms/2022-1.html)

* Obtain licenses for AI Engine tools

* [DSP Library (DSPLib) Documentation](https://xilinx.github.io/Vitis_Libraries/dsp/2022.1/index.html)
* Download the [DSP Library](https://github.com/Xilinx/Vitis_Libraries/tree/master/dsp)

* Follow the instructions in [Installing Xilinx Runtime and Platforms](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Installing-Xilinx-Runtime-and-Platforms) (XRT)

</details>

<details>
<summary>Platform</summary> 

## Platform

Before beginning the tutorial, make sure you have read and followed the [Vitis Software Platform Release Notes (v2022.1)](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Vitis-Software-Platform-Release-Notes) for setting up software and installing the VCK190 base platform.

This tutorial targets the [VCK190 production board](https://www.xilinx.com/products/boards-and-kits/vck190.html). If you have already purchased this board, download the necessary files from the Xilinx Website and ensure you have the correct licenses installed. If you do not have a board and the required license, contact your Xilinx sales contact.

</details>

<details>
<summary>Setting Up the Environment</summary>
 
## Setting up the Environment

When the elements of the Vitis software platform are installed, create a shell environment script, `env_setup.sh`, using the template given as `sample_env_setup.sh`. Set the environment variables to your system specific paths.

Edit `env_setup.sh` script with your file paths, then source the environment script: 

```bash
source env_setup.sh
``` 

The script sets up the environment variables and sources scripts explained below:

1. `unset LD_LIBRARY_PATH ` used to unset the previously set-uped environment paths
2. The `PLATFORM_REPO_PATHS` environment variable is based on where you downloaded the "xilinx_vck190_base_202210_1" platform.
3. The `XILINX_TOOLS_LOCATION` path to the Xilinx tools is used to source the `settings64.sh` script.
4. The `XLNX_VERSAL` path to the `xilinx-versal-common-v2022.1` directory is used in the step below.
5. The platform is set up by running the `xilinx-versal-common-v2022.1/environment-setup-cortexa72-cortexa53-xilinx-linux` script as provided in the platform download This script sets up the `SDKTARGETSYSROOT` and `CXX` variables. If the script is not present, you _must_ run the `xilinx-versal-common-v2022.1/sdk.sh` script FIRST.
6. `DSPLIB_VITIS` is the path to the downloaded Vitis DSP Libraries. This is only required for the AI Engine implementation.
7. In the script, you can optionally set up an `XRT_ROOT` environment variable, pointing to XRT - RPMs, which can be packaged in the Vitis compiler packaging step. If it is not set up, this environment variable is automatically be excluded from packaging.
8. The script also sets up the `PLATFORM` variable pointing to the required `.xpfm` file of the target platform set by the variable `tgt_plat`.

</details>

<details>
<summary>Confirming Tool Installation</summary> 
	
## Confirming Tool Installation

To confirm that you have installed the correct tools, run the following command: 

```bash
which vitis
which aiecompiler
```

To confirm you have the VCK190 base platform, run the following command: 

```bash
platforminfo --list | grep -m 1 -A 9 vck190
```

The output of the above command should be as follows:

```bash
 "baseName": "xilinx_vck190_base_202210_1",
            "version": "1.0",
            "type": "sdsoc",
            "dataCenter": "false",
            "embedded": "true",
            "externalHost": "false",
            "serverManaged": "false",
            "platformState": "pre_synth",
            "usesPR": "false",
```

</details>

# Design Implementations

The Makefile and source files for the AI Engine and HLS implementations are in the `AIE` and `DSP` directories respectively. For the documentation of the flow to build the design and details of the hardware and software design, follow each of the links below:

* [AI Engine design implementation](AIE)
* [HLS design implementation with DSP Engines](HLS)

# AI Engine and HLS Implementation Comparison

The following table shows a comparison between a 1024 x 2048 point 10-instance FFT-2D design implemented using the AI Engines and HLS with DSP Engines respectively. It lists the throughput, resource utilization, power consumption, and performance in throughput/Watt for `cint16` implementations.

| Design Target | Aggregate Throughput<br/>(in MSPS) | Average Latency (in μs) | AIE Vector Cores | AIE Vector Load | Active Mem Banks /<br/> Mem R/W Rate | Active AIE Tiles | FF (Regs) /<br/> CLB LUTs | BRAMs | DSPs | Dynamic Power<br/>(in mW) | Performance per Watt<br/>(in MSPS/Watt) |
|:-------------:|:----------------------------------:|:-----------------------:|:----------------:|:---------------:|:------------------------------------:|:----------------:|:-------------------------:|:-----:|:----:|:-------------------------:|:---------------------------------------:|
| AIE           | 7528.710                           | 2873.628                | 20               | 78%             | 404 /<br/>20%                        | 46               | 11720 /<br/> 4157         | 0     | 0    | 5572                      | 1351.17                                |
| HLS           | 4979.797                           | 4040.900                | NA               | NA              | NA                                   | NA               | 98899 /<br/> 61591        | 250   | 180  | 5800                      | 858.59                                 |

These observations give a clear indication of where the AI Engines in Versal can offer improvements:

* An almost 51% improvement on the aggregate throughput.
* Reduced latency by ~29%.
* Performance increase of ~2x to 1351 MSPS/Watt.
* Moving to AI Engine implementation reduces the PL and DSP resources considerably; 180 DSPs, ~98K FFs, ~61K LUTs and 250 BRAMs are reduced to just 46 AI Engines, 11k FFs, and 4K LUTs.

It is important to understand that those 46 AI Engines are not all required for the 2D-FFT compute: 20 AI Engines/vector cores are required for computation, and 26 AI Engines are required for the memory to store the FFT twiddle factors and also to enable connectivity around the array. The average load on these additional 26 AI Engine tiles is only 25%. This means that if your application needs it, these AI Engines can be shared with other functions to run sequentially, or they can use user constraints to better map and route this function to a reduced number of AI Engine tiles (see [this page](https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment/Mapper/Router-Methodology) for details on the AI Engine mapper/router).

Additionally, further increasing the number of instances in the AI Engine design is easier compared to the HLS design, which will run into timing closure issues, especially for higher FFT point size designs.

**Measurement:**

1. AI Engine design resource utilization is measured using Xilinx Power Estimator (XPE) and Vivado (report utilization under implementation for FFs and CLB LUTs). For the HLS design, resource utilization is measured using Vivado.
2. AI Engine power consumption is measured using XPE. HLS power consumption is measured using Vivado (report power under implementation).
3. Throughput is measured using viewing runtime profiling generated trace texts in `vitis_analyzer`.

For detailed instructions on taking measurements of the parameters, refer to the individual implementation section.

# References

### [AI Engine Documentation](https://docs.xilinx.com/search/all?filters=Document_ID~%2522UG1076%2522_%2522UG1079%2522&content-lang=en-US)

Contains sections on how to develop AI Engine graphs, how to use the AI Engine compiler, and AI Engine simulation, and performance analysis.

### Vitis DSP Libraries

* [Vitis DSP Libraries Comprehensive Documentation](https://xilinx.github.io/Vitis_Libraries/dsp/2022.1/) 

### Xilinx Runtime (XRT) Architecture

Below are links to the XRT information used by this tutorial: 

* [XRT Documentation](https://xilinx.github.io/XRT/master/html/index.html): Explains general XRT API calls used in the PS Host Application. 

* [XRT Github Repo](https://github.com/Xilinx/XRT): Contains the XRT source code. 

* [XRT AIE API](https://github.com/Xilinx/XRT/blob/master/src/runtime_src/core/include/experimental/xrt_aie.h): Documents the AI Engine XRT API calls

* [XRT Release Notes](https://docs.xilinx.com/r/en-US/ug1451-xrt-release-notes)

### Vitis Unified Software Development Platform 2022.1 Documentation

Below are links to Vitis related information referenced in this tutorial:

* [Vitis Application Acceleration Development Flow Documentation](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration)

* [Vitis Application Acceleration Development Flow Tutorials](https://github.com/Xilinx/Vitis-Tutorials)

* [Vitis HLS](https://docs.xilinx.com/r/en-US/ug1399-vitis-hls)

# Known Issues

The timestamps represented in the hardware execution generated trace, have known a issue due to which they are scaled compared to the actual. This issue will be fixed in the subsequent tool versions. All calculations/observations in this tutorial are based on the hw_emu runs.

# Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [forums.xilinx.com](http://forums.xilinx.com/).


# License

© Copyright 2020-2022 Xilinx, Inc.

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.

You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )


Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center"> XD073 | &copy; Copyright 2021–2022 Xilinx, Inc.</p>
