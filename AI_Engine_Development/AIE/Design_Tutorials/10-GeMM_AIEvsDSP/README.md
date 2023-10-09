<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Versal® ACAP AI Engine Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

## Versal GeMM Implementation Using Vitis Acceleration Library and DSP58 Tutorial

***Version: Vitis 2023.1***

## Table of Contents

[Introduction](#introduction)

[Before You Begin](#Before-you-Begin)

[Design Implementations](#Design-Implementations)

[AI Engine and HLS Implementation Comparison](#AI-Engine-and-HLS-Implementation-Comparison)

[References](#References)

[Known Issues](#Known-Issues)

## Introduction

The Versal™ adaptive SoC is a a fully software programmable, heterogeneous compute platform that combines the following:

- Scalar Engines (a processor subsystem (PS) that includes Arm® processors)
- Adaptable Engines (programmable logic (PL) and memory)
- Intelligent Engines (including both including both AI and DSP Engines)

This tutorial performs two implementations of a system-level design: one with AI Engine, and the other with RTL using the DSP Engines. In each implementation, the tutorial takes you through the hardware emulation and hardware flow in the context of a complete Versal adaptive SoC system design.

A Makefile is provided for each implementation. It can be used to create the design for `cint16` datatype, for matrix dimensions 32x32x32 (MAT A, B, and C Dimensions - 32x32), 64x64x64, 128x128x128, 256x256x256, 512x512x512, and 1024x1024x1024 and lastly for different targets (hw_emu and hw).

The design documentation demonstrates hardware and software design details including the methodology for each implementation, explaining the functional partitioning. The compilation, execution, and measurement steps as well as observations are given.

<details>
  <summary>Objectives</summary> 
	
### Objectives

After completing the tutorial, you should be able to:

* Develop a system-level GeMM design by identifying an algorithm and deploying it on AI Engines or PL and DSP Engines. 
* Build a complete system design by going through the following steps in the Vitis flow:
  * Create the AI Engine Adaptive Data Flow API (ADF) graph.
  * Compile the A72 host application and compiling PL kernels.
  * Use the Vitis compiler (V++) to link the AI Engine and HLS kernels with the platform.
  * Package the design.
  * Run the design through the hardware emulation and hardware flow in a mixed SystemC/RTL cycle-accurate/QEMU-based simulator.
* Understand graph control APIs for AI Engine implementation and HLS APIs for controlling HLS/PL kernels.
* Understand the methodological differences between a design created using AI Engines and a design created using PL and DSP Engines.
* Understand metrics including utilization, performance/throughput, and power across various instances of FFT arrays of different dimensions.

</details>

<details>
  <summary>Design Overview</summary> 
 
### Design Overview

#### AIE
In this design, the multiplication of 2 square matrices (MatA and MatB) is done using a 32-AIE core overlay. MatA is
divided into 8 x 4 blocks and MatB into 4 x 8 blocks. MatA input is provided 1x4 block at a time, using 4 input streams,
and MatB is provided using 32 input streams for each 4x8 blocks. Output Matrix MatC is divided into 8x8 blocks and is given out
as 1x8block at a time using 8 output streams. 32 core overlay is chosen to keep the core overlay same across all Matrx
Dimensions, 32x32x32-64x64x64 onwards to 1024x1024x1024 and keep the performance high.

#### DSP
In this design, Matrix multiplication is implemented using Systolic array of 1024 DSP58 Engines. There are 32 DSP58 cascade chains, 
each chain has 32 DSP58s. Matrix-Matrix multiplication is decomposed into Matrix-Vector multiplication. One Matrix B column vector
is multiplied by each row of Matrix A. This is achieved by broadcasting Matrix B column vector to DSPs at the same position in each cascade chain, while all 1K elements of Matrix A are read and each element drives one Port A of DSP58. One cascade chain implements
one column vector and one row vector multiplication. This operation completes in 32 clocks.

Thus 32x32 matrix is the basic matrix multiplication unit. Larger matrices are broken down into submatrices of size 32x32, and each 
32x32 submatrix of Matrix A is multiplied with each submatrix of Matrix B. For larger matrix multiplication, partial sum needs to be 
stored, read back, added to the new value and stored back.

</details>

<details>
  <summary>Directory Structure</summary> 
	
### Directory Structure

```
GeMM_AIEvsDSP
|__AIE......................contains AI Engine implementation
|    |Makefile....................with recipes for each step of the design compilation
|    |images......................contains images used for AI Engine Design documentation
|    |description.json............required for internal regression 
|    |multi_params.json...........required for internal regression 
|    |build.......................created and contains subfolders from design build
|    |design......................contains source and include files
|    |      |aie_src....................contains all the aie source files and aiesimulator input files
|    |      |      |aiesim_data.................contains all the files for the aiesimulator input
|    |      |pl_src.....................contains all the data mover source files
|    |      |host_app_src...............contains host application source files
|    |      |system_configs.............contains all system configuration files
|    |      |profiling_configs..........contains xrt.ini file
|    |      |exec_files.................contains hw_emu launch script
|    |      |vivado_metrics_scripts.....contains script for reporting utilisation and power from vivado
|__DSP......................contains DSP implementation targeting DSP Engines
|    |Makefile....................with recipes for each step of the design compilation
|    |images......................contains images used for DSP Design documentation
|    |description.json............required for XOAH
|    |multi_params.json...........required for XOAH
|    |build.......................created and contains subfolders from design build
|    |design......................contains source and include files
|    |      |pl_src.....................contains all GeMM and data mover source files
|    |      |host_app_src...............contains host application source files
|    |      |system_configs.............contains all system configuration files
|    |      |profiling_configs..........contains xrt.ini file
|    |      |exec_files.................contains hw_emu launch script
|    |      |vivado_metrics_scripts.....contains script for reporting utilisation and power from vivado
```
</details>

## Before You Begin

<details>
	
<summary>Documentation: Explore AI Engine Architecture</summary> 

### Documentation: Explore AI Engine Architecture

* [AI Engine Development Design Process](https://www.xilinx.com/support/documentation-navigation/design-process/ai-engine-development.html)

* [AM009 AI Engine Architecture Manual](https://docs.xilinx.com/r/en-US/am009-versal-ai-engine/Revision-History)

* [Versal ACAP AI Engines for Dummies](https://forums.xilinx.com/t5/Design-and-Debug-Techniques-Blog/Versal-ACAP-AI-Engines-for-Dummies/ba-p/1132493)

</details>

<details>
<summary>Installing the Tools</summary> 
	
### Installing the Tools

* [AI Engine Tools Lounge](https://www.xilinx.com/member/versal_ai_tools_ea.html)

* [AI Engine Documentation](https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html)

To build and run the GeMM tutorial (AI Engine and HLS implementations), perform the following steps:

* Install the [Vitis Software Platform](https://www.xilinx.com/products/design-tools/vitis/vitis-platform.html).

* Obtain licenses for AI Engine tools.

* Follow the instructions in [Installing Xilinx Runtime and Platforms](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Installing-Xilinx-Runtime-and-Platforms) (XRT).

* Download and set up the [VCK190 Vitis Platform]( https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html).

</details>

<details>
<summary>Platform</summary> 

### Platform

Before beginning the tutorial, make sure you have read and followed the [Vitis Software Platform Release Notes (v2023.1)](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Vitis-Software-Platform-Release-Notes) for setting up software and installing the VCK190 base platform.

This tutorial targets the [VCK190 production board](https://www.xilinx.com/products/boards-and-kits/vck190.html). If you have already purchased this board, download the necessary files from the lounge and ensure you have the correct licenses installed. If you do not have a board and the required license, contact your AMD sales team.

</details>

<details>
<summary>Setting Up the Environment</summary>
 
### Setting up the Environment

When the elements of the Vitis software platform are installed, update the shell environment script. Set the environment variables to your system-specific paths.

To set up XRT, if you have not done this already, run the following command:

```
* source \<XRT-Location\>/setup.sh
```

In the design directory of each implementation, edit `sample_env_setup.sh` script with your file paths, then source the environment script: 

```bash
source sample_env_setup.sh
``` 

The script sets up the environment variables and sources scripts explained below:

1. The `PLATFORM_REPO_PATHS` environment variable is based on where you downloaded the platform.
2. The `XILINX_TOOLS_LOCATION` path to the AMD tools is used to source the `settings64.sh` script.
3. The `XLNX_VERSAL` path to the `xilinx-versal-common-v2023.1` directory is used in the step below.
4. The platform is set up by running the `xilinx-versal-common-v2023.1/environment-setup-cortexa72-cortexa53-xilinx-linux` script as provided in the platform download This script sets up the `SDKTARGETSYSROOT` and `CXX` variables. If the script is not present, you _must_ run the `xilinx-versal-common-v2023.1/sdk.sh` script.
5. `DSPLIB_VITIS` is the path to the downloaded Vitis Libraries. This is only required for the AI Engine implementation.
6. In the script, you can optionally set up an `XRT_ROOT` environment variable, pointing to XRT - RPMs, which can be packaged in the Vitis compiler packaging step. If it is not set up, this environment variable is automatically excluded from packaging.
7. The script also sets up the `PLATFORM` variable pointing to the required `.xpfm` file of the target platform set by the variable `tgt_plat`.

</details>

<details>
<summary>Confirming Tool Installation</summary> 
	
### Confirming Tool Installation

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
 "baseName": "xilinx_vck190_base_202310_1",
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

## Design Implementations

The Makefile and source files for the AI Engine and HLS implementations are in the `AIE` and `DSP` directories respectively. For the documentation of the flow to build the design and details of the hardware and software design, follow each of the links below:

* [AI Engine design implementation](AIE)
* [DSP design implementation with DSP Engines](DSP)

## AI Engine and DSP Implementation Comparison

The following table shows a comparison between a 1024 x 1024 x1024 GeMM design implemented using the AI Engines and DSP with DSP Engines respectively. It lists the throughput, resource utilization, power consumption, and performance in throughput/Watt for `cint16` implementations.

| Design Target | TOPS<br/> | Average Latency (in μs) | AIE Vector Cores | AIE Vector Load | Active Mem Banks /<br/> Mem R/W Rate | Active AIE Tiles | FF (Regs) /<br/> CLB LUTs | BRAMs | DSPs | Dynamic Power<br/>(in mW) | TOPS per Watt<br/>(in TOPS/Watt) |
|:-------------:|:----------------------------------:|:-----------------------:|:----------------:|:---------------:|:------------------------------------:|:----------------:|:-------------------------:|:-----:|:----:|:-------------------------:|:---------------------------------------:|
| AIE           | 2.406     | 11.251                  | 32               | 95%             | 608 /<br/>6%                         | 144              | 15940 /<br/> 3680        |0       | 0    | 10132                      |  0.237                         |    
| DSP           | 1.499     |   1398.106              | NA               | NA              | NA                                   | NA               | 80480 /<br/> 25231       | 64     | 1024  | 4765.4                   | 0.186                            |


It is important to understand that those 144 AI Engines tiles are not all required for the GeMM compute: 32 AI Engines/vector cores are required for computation, and 112 AI Engines are required for the memory to store the Matrices and also to enable connectivity around the array. The average load on these additional 112 AI Engine tiles is 95%.

Measurement:

1. AI Engine design resource utilization is measured using Xilinx Power Estimator (XPE) and Vivado (report utilization under implementation for FFs and CLB LUTs). For the HLS design, resource utilization is measured using Vivado.
2. AI Engine power consumption is measured using XPE. HLS power consumption is measured using Vivado (report power under implementation).
3. Throughput is measured using viewing runtime profiling generated trace texts in `vitis_analyzer`.

For detailed instructions on taking measurements of the parameters, refer to the individual implementation section.

## References

#### [Vitis Unified Software Development Platform Documentation](https://docs.xilinx.com/v/u/en-US/ug1416-vitis-documentation)

Contains sections on how to develop AI Engine graphs, how to use the AI Engine compiler and AI Engine simulation, and performance analysis.

#### [Vitis DSP Libraries](https://github.com/Xilinx/Vitis_Libraries/tree/master/dsp)

* [Vitis DSP Libraries Comprehensive Documentation](https://docs.xilinx.com/r/en-US/Vitis_Libraries/dsp/index.html) 


#### [Xilinx Runtime (XRT) Architecture](https://xilinx.github.io/XRT/master/html/index.html)

Below are links to the XRT information used by this tutorial: 

* [XRT Documentation](https://xilinx.github.io/XRT/master/html/index.html): Explains general XRT API calls used in the PS Host Application. 

* [XRT Github Repo](https://github.com/Xilinx/XRT): Contains the XRT source code. 

* [XRT AIE API](https://github.com/Xilinx/XRT/blob/master/src/runtime_src/core/include/experimental/xrt_aie.h): Documents the AI Engine XRT API calls

* [XRT Release Notes](https://www.xilinx.com/support/documentation/sw_manuals/xilinx2023_1/ug1451-xrt-release-notes.pdf)


#### [Vitis Unified Software Development Platform 2023.1 Documentation](https://docs.xilinx.com/v/u/en-US/ug1416-vitis-documentation)


Below are links to Vitis related information referenced in this tutorial:

* [Vitis Application Acceleration Development Flow Documentation](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration)

* [Vitis Application Acceleration Development Flow Tutorials](https://github.com/Xilinx/Vitis-Tutorials)

* [Vitis HLS](https://docs.xilinx.com/r/en-US/ug1399-vitis-hls)

## Known Issues

The timestamps represented in the hardware execution generated trace, has a known issue due to which they are scaled compared to the actual. This issue will be fixed in the subsequent tool versions. All calculations/observations in this tutorial are based on the hw_emu runs.

## Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [support.xilinx.com](http://support.xilinx.com/).



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
