<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">Refer to Vitis™ Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">Refer to Vitis™ AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# Versal 2D-FFT Implementation Using Vitis Acceleration Library Tutorial (XD073)

***Version: Vitis 2025.2***

## Table of Contents

[Introduction](#introduction)

[Before You Begin](#before-you-begin)

[Design Implementations](#design-implementations)

[AI Engine and HLS Implementation Comparison](#ai-engine-and-hls-implementation-comparison)

[References](#references)

[Known Issues](#known-issues)

## Introduction

AMD Versal™ adaptive SoCs combine programmable logic (PL), processing system (PS), and AI Engines with leading-edge memory and interfacing technologies to deliver powerful heterogeneous acceleration for any application. The hardware and software are targeted for programming and optimization by data scientists and software and hardware developers. A host of tools, software, libraries, IP, middleware, and frameworks enable Versal adaptive SoCs to support all industry-standard design flows.

This tutorial performs two implementations of a system-level design: one with an AI Engine and another with HLS using the DSP Engines. In each implementation, the tutorial takes you through the hardware emulation and hardware flow in the context of a complete Versal adaptive SoC system design.

A Makefile is available for each implementation. You can use it to create the design for the following:

* `cint16` and `acfloat` datatypes for various point sizes (32 x 64, 64 x 128, 128 x 256, 256 x 512, and 1024 x 2048)
* Different numbers of fft_2d HLS kernel/AI Engine graph instances (1, 5, and 10)
* Different targets (hw_emu and hw)

The design documentation demonstrates hardware and software design details, including the methodology for each implementation, elaborating on the functional partitioning. The documentation provides compilation, execution, measurement steps, and observations.

<details>
  <summary>Objectives</summary>

### Objectives

After completing the tutorial, you can do the following:

* Develop a system-level 2D-FFT design by identifying and deploying an algorithm on AI Engines or PL and DSP Engines using AMD Vitis&trade; HLS.
* Build a complete system design using the following steps in the Vitis flow:
  * Create the AI Engine Adaptive Data Flow API (ADF) graph.
  * Compile the A72 host application and PL kernels.
  * Use the Vitis compiler (V++) to link the AI Engine and HLS kernels with the platform.
  * Package the design.
  * Run the design through the hardware emulation and flow in a mixed SystemC/RTL cycle-accurate/QEMU-based simulator.
* Develop a consistent harness to have the data mover kernels maintain a similar interface with AI Engine or HLS/PL kernels (using AXI4-Stream).
* Understand graph control APIs for AI Engine implementation, and HLS APIs for controlling HLS/PL kernels.
* Understand the methodological differences between a design created using AI Engines and one created using PL and DSP Engines using HLS.
* Understand metrics, including utilization, performance/throughput, and power across various instances of FFT arrays of different dimensions.

</details>

<details>
  <summary>Design Overview</summary>

### Design Overview

This design implements a 2D-FFT algorithm performed on (for example) a 1024 x 2048 matrix using 1024- and 2048-point 1D-FFT kernels. The following example shows the pseudo code to implement the algorithm:

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

In calculating each 1D-FFT, one dimension remains constant while the other is computed. The transpose function is applied after each 1D-FFT compute. The transpose function moves the entry along each element of the dimension to the corresponding element of the other dimension. A golden data set is generated as a reference, and the error difference is calculated.

A similar algorithm is deployed in the two implementations using either the AI Engines or HLS targeting the PL and DSP Engines. The design compiles through the Vitis compiler, creates a PetaLinux-based platform using a script, and generates the PDI and host application. Instead of the transpose part, however, a PL-based data generator and checker (referred to as a data mover) is used to give an impulse (value = 1 or 1.5, depending on the cint16 or cfloat datatype) input to the row-wise 1D-FFT. It also checks its output against the expected FFT output for the same (the first row containing all 1s and remaining 0s).

The transposed pattern generated within the PL is streamed as input to the col-wise 1D-FFT. Its output is checked against the expected output (all 1s). The data mover kernel returns the total error count in both stages to the host application. The application declares a pass or fail of the test case.

To help you compare the methodology of the AI Engine and HLS-based implementations, you can modify the design-build process to analyze how each implementation scales with different numbers of instances (x1, x5, and x10) of various matrix array dimensions (rows x cols = 32 x 64, 64 x 128, 128 x 256, 256 x 512, and 1024 x 2048).

A similar set of harnesses is developed and maintained between the two implementations. This lets you to generate and check input/output vectors using the PL-based data mover kernels, and to move data to and from the AI and HLS kernels. In both cases, Xilinx Runtime (XRT) running on A72 controls data flow in compute, and data mover kernels through graph control APIs. These graph control APIs control the AI Engine kernels and HLS APIs, which control the HLS/PL kernels.

</details>

<details>
  <summary>Directory Structure</summary>

### Directory Structure

```
fft2d_AIEvsHLS
|__AIE......................contains AI Engine implementation
|    |Makefile....................with recipes for each step of the design compilation
|    |images......................contains images used for AI Engine Design documentation
|    |description.json............required for internal regression
|    |multi_params.json...........required for internal regression
|    |sample_env_setup.sh.........required to setup Vitis environment variables and Libraries
|    |build.......................created and contains subfolders from design build
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
|    |description.json............required for internal regression
|    |multi_params.json...........required for internal regression
|    |sample_env_setup.sh.........required to setup Vitis environment variables and Libraries
|    |build.......................created and contains subfolders from design build
|    |design......................contains source and include files
|    |       |pl_src.....................contains all the fft_2d and data mover source files
|    |       |host_app_src...............contains host application source files
|    |       |system_configs.............contains all system configuration files
|    |       |profiling_configs..........contains xrt.ini file
|    |       |directives.................contains directives for various vitis compilation stages like hls.pre_tcl etc.
|    |       |hw_emu_files...............contains hw_emu launch script
```

</details>

## Before You Begin

<details>

<summary>Documentation: Explore AI Engine Architecture</summary>

### Documentation: Explore AI Engine Architecture

* [AI Engine Development Design Process](https://docs.amd.com/p/ai-engine-development)

* [AM011 AI Engine Architecture Manual](https://docs.amd.com/r/en-US/am011-versal-acap-trm)

* [Versal adaptive SoC AI Engines for Dummies](https://adaptivesupport.amd.com/s/article/1132493?language=en_US)

</details>

<details>
<summary>Installing the Tools</summary>

### Installing the Tools

To build and run the 2D-FFT tutorial (AI Engine and HLS implementations), perform the following steps:

* Install the [Vitis Software Platform](https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html)

* Obtain licenses for AI Engine tools.

* [DSP Library(DSPLIB) Documentation](https://docs.amd.com/r/en-US/Vitis_Libraries/dsp/index.html)

* Download the [DSP Library](https://github.com/Xilinx/Vitis_Libraries/tree/master/dsp)

</details>

<details>
<summary>Platform</summary>

### Platform

Before beginning the tutorial, read and follow the [Vitis Software Platform Release Notes](https://docs.amd.com/r/en-US/ug1393-vitis-application-acceleration/Vitis-Software-Platform-Release-Notes) for setting up software.

This tutorial targets the [VCK190 production board](https://www.amd.com/en/products/adaptive-socs-and-fpgas/evaluation-boards/vck190.html). If you have already purchased this board, download the necessary files by following installation steps and make sure you have the correct licenses installed. If you do not have a board and the required license, contact your AMD sales contact.

</details>

<details>
<summary>Setting Up the Environment</summary>

### Setting Up the Environment

After installing the Vitis software platform elements, update the shell environment script. Set the environment variables to your system-specific paths.

To set up XRT, if you have not done this already, run the following command:

```
* source \<XRT-Location\>/setup.sh
```

In the design directory of each implementation, edit `env_setup.sh` script with your file paths and source the environment script:  

```bash
source env_setup.sh
```

The script sets up the environment variables and sources scripts, explained as follows:

1. The `PLATFORM_REPO_PATHS` environment variable is based on where you downloaded the platform.
2. The `XILINX_TOOLS_LOCATION` path to the Xilinx tools is for sourcing the `settings64.sh` script.
3. The `XLNX_VERSAL` path to the `xilinx-versal-common-v2025.2` directory is used in the step following step.
4. Running the `xilinx-versal-common-v2025.2/environment-setup-cortexa72-cortexa53-amd-linux` script as provided in the platform download sets up the platform. This script sets up the `SDKTARGETSYSROOT` and `CXX` variables. If the script is not present, you *must* run the `xilinx-versal-common-v2025.2/sdk.sh` script.
5. `DSPLIB_ROOT` is the path to the downloaded Vitis DSP Libraries. This is only required for the AI Engine implementation.
6. In the script, you can optionally set up an `XRT_ROOT` environment variable, pointing to XRT - RPMs, which can you can package in the Vitis compiler packaging step. If not set up, this environment variable is automatically excluded from packaging.
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

The output of the preceding command is as follows:

```bash
 "baseName": "xilinx_vck190_base_202520_1",
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

The Makefile and source files for the AI Engine and HLS implementations are in the `AIE` and `DSP` directories, respectively. For documentation of the flow to build the design and details of the hardware and software design, refer to the following links:

* [AI Engine design implementation](AIE)
* [HLS design implementation with DSP Engines](HLS)

## AI Engine and HLS Implementation Comparison

The following table compares a 1024 x 2048 point 10-instance FFT-2D design implemented using the AI Engines and HLS with DSP Engines respectively. It lists the throughput, resource utilization, power consumption, and performance in throughput/Watt for `cint16` implementations.

| Design Target | Aggregate Throughput<br/>(in MSPS) | Average Latency (in μs) | AIE Vector Cores | AIE Vector Load | Active Mem Banks /<br/> Mem R/W Rate | Active AIE Tiles | FF (Regs) /<br/> CLB LUTs | BRAMs | DSPs | Dynamic Power<br/>(in W) | Performance per Watt<br/>(in MSPS/Watt) |
|:-------------:|:----------------------------------:|:-----------------------:|:----------------:|:---------------:|:------------------------------------:|:----------------:|:-------------------------:|:-----:|:----:|:-------------------------:|:---------------------------------------:|
| AIE           | 6216.973                           | 3537.296                | 20              | 79.00%           | 420 /<br/>44%                        | 60               | 11360 /<br/> 3647         | 0     | 0    | 5.547                      | 1121.185                                 |
| HLS           | 6277.483                           | 4211.296                | NA              | NA               | NA                                   | NA               | 88447 /<br/> 56429        | 250   | 180  | 6.819                      | 920.587051                               |

These observations give a clear indication of where the AI Engines in Versal can offer improvements:

* Reduced latency by ~19.054%.
* Moving to AI Engine implementation reduces the PL and DSP resources considerably; 180 DSPs, ~88K FFs, ~56K LUTs, and 250 BRAMs reduce to 72 AI Engines, 11k FFs, and 3K LUTs.

It is important to understand that those 72 AI Engines are not all required for the 2D-FFT compute. Computation requires 20 AI Engines/vector cores. Memory requires 52 AI Engines for Storing the FFT twiddle factors and enabling connectivity around the array. The average load on these additional 52 AI Engine tiles is only 79%. This means that if your application needs it, you can share these AI Engines with other functions to run sequentially. Alternatively, they can use user constraints to better map and route this function to a reduced number of AI Engine tiles. Refer to [this page](https://docs.amd.com/r/en-US/ug1076-ai-engine-environment/Mapper/Router-Methodology) for details on the AI Engine mapper/router.

Also, increasing the number of instances in the AI Engine design is easier than the HLS design, which runs into timing closure issues. This is especially true for higher FFT point size designs.

**Measurement:**

1. You can use Xilinx Power Estimator (XPE) and AMD Vivado&trade; (report utilization under implementation for FFs and CLB LUTs) to measure AI Engine design resource utilization. Vivado measures resource utilization in the HLS design.
2. XPE measures AI Engine power consumption. Vivado (report power under implementation) measures HLS power consumption.
3. Throughput is measured using viewing runtime profiling generated trace texts in `vitis_analyzer`.

For detailed instructions on taking measurements of the parameters, refer to the individual implementation section.

## References

#### [AI Engine Documentation](https://docs.amd.com/search/all?filters=Document_ID~%2522UG1076%2522_%2522UG1079%2522&content-lang=en-US)

Contains sections on how to develop AI Engine graphs, how to use the AI Engine compiler, AI Engine simulation, and performance analysis.

#### Vitis DSP Libraries

* [Vitis DSP Libraries Comprehensive Documentation](https://docs.amd.com/r/en-US/Vitis_Libraries/dsp/index.html)

#### Xilinx Runtime (XRT) Architecture

Following are links to the XRT information used by this tutorial:

* [XRT Documentation](https://xilinx.github.io/XRT/master/html/index.html): Explains general XRT API calls used in the PS Host Application.

* [XRT Github Repo](https://github.com/Xilinx/XRT): Contains the XRT source code.

* [XRT AIE API](https://github.com/Xilinx/XRT/blob/master/src/runtime_src/core/include/experimental/xrt_aie.h): Documents the AI Engine XRT API calls

* [XRT Release Notes](https://docs.amd.com/r/en-US/ug1742-vitis-release-notes/Vitis-Release-Notes)

#### Vitis Unified Software Development Platform Documentation

* https://docs.amd.com/v/u/en-US/ug1416-vitis-documentation

Following are links to Vitis-related information referenced in this tutorial:

* [Vitis Application Acceleration Development Flow Documentation](https://docs.amd.com/v/u/en-US/ug1393-vitis-application-acceleration)

* [Vitis Application Acceleration Development Flow Tutorials](https://github.com/Xilinx/Vitis-Tutorials)

* [Vitis HLS](https://docs.amd.com/r/en-US/ug1399-vitis-hls)

## Known Issues

The timestamps represented in the hardware execution generated trace have a known issue due to how they are scaled compared to the actual. This issue will be fixed in subsequent tool versions. All calculations/observations in this tutorial use the hw_emu runs.

## Support

Requests and bugs are tracked using GitHub issues. For questions, go to [Adaptive SoC & FPGA Support](https://adaptivesupport.amd.com/s/?language=en_US).


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
