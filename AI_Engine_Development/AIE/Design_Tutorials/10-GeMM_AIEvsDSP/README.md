<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">See Vitis™ AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# Versal GeMM Implementation Using Vitis Acceleration Library and DSP58 Tutorial

***Version: Vitis 2025.2***

## Table of Contents

- [Introduction](#introduction)
  - [Objectives](#objectives)
  - [Design Overview](#design-overview)
  - [Directory Structure](#directory-structure)
- [Before You Begin](#before-you-begin)
  - [Documentation: Explore AI Engine Architecture](#documentation-explore-ai-engine-architecture)
  - [Installing the Tools](#installing-the-tools)
  - [Platform](#platform)
  - [Setting Up the Environment](#setting-up-the-environment)
  - [Confirming Tool Installation](#confirming-tool-installation)
- [Design Implementations](#design-implementations)
- [AI Engine and DSP Implementation Comparison](#ai-engine-and-dsp-implementation-comparison)
- [References](#references)
- [Known Issues](#known-issues)
- [Support](#support)

## Introduction

Versal™ adaptive SoCs combine programmable logic (PL), processing system (PS), and AI Engines with leading-edge memory and interfacing technologies to deliver powerful heterogeneous acceleration for any application. The hardware and software are targeted for programming and optimization by data scientists and software and hardware developers. A host of tools, software, libraries, IP, middleware, and frameworks enable Versal adaptive SoCs to support all industry-standard design flows.

This tutorial performs two implementations of a system-level design: one with AI Engine, and the other with RTL using the DSP Engines. In each implementation, the tutorial takes you through the hardware emulation and hardware flow in the context of a complete Versal adaptive SoC system design.

There is a Makefile for each implementation. Use it to create the design for `cint16` datatype, for matrix dimensions 32x32x32 (MAT A, B, and C Dimensions - 32x32), 64x64x64, 128x128x128, 256x256x256, 512x512x512, and 1024x1024x1024 and lastly for different targets (hw_emu and hw).

The design documentation demonstrates hardware and software design details including the methodology for each implementation, explaining the functional partitioning. It provides the compilation, execution, and measurement steps as well as observations.

<details>
  <summary>Objectives</summary>

### Objectives

After completing the tutorial, you can:

- Develop a system-level GeMM design by identifying an algorithm and deploying it on AI Engines or PL and DSP Engines.
- Build a complete system design by going through the following steps in the Vitis software flow:
  - Create the AI Engine Adaptive Data Flow API (ADF) graph.
  - Compile the A72 host application and PL kernels.
  - Use the Vitis compiler (V++) to link the AI Engine and HLS kernels with the platform.
  - Package the design.
  - Run the design through the hardware emulation and hardware flow in a mixed SystemC/RTL cycle-accurate/QEMU-based simulator.
- Understand graph control APIs for AI Engine implementation and HLS APIs for controlling HLS/PL kernels.
- Understand the methodological differences between a design created using AI Engines and a design created using PL and DSP Engines.
- Understand metrics including utilization, performance/throughput, and power across various instances of FFT arrays of different dimensions.

</details>

<details>
  <summary>Design Overview</summary>

### Design Overview

#### AIE

In this design, a 24-AIE core overlay multiplies two square matrices (MatA and MatB). The design divides MatA into 3 x 8 blocks and MatB into 8 x 3 blocks. Eight input streams provide MatA input 1x8 block at a time. 24 input streams provide MatB for each 8x3 blocks. The design divides Output Matrix MatC into 3x3 blocks and outputs it as 1x3 block at a time using three output streams. The 24 core overlay keeps the core overlay same across all Matrix Dimensions, 32x32x32-64x64x64 onwards to 1024x1024x1024 and maintains high performance.

#### DSP

In this design, a systolic array of 1024 DSP58 Engines implements matrix multiplication. There are 32 DSP58 cascade chains, each chain has 32 DSP58s. The design decomposes Matrix-Matrix multiplication into Matrix-Vector multiplication. Each row of Matrix A multiplies one Matrix B column vector. Broadcasting Matrix B column vector to DSPs at the same position in each cascade chain achieves this, while the design reads all 1K elements of Matrix A and each element drives one Port A of DSP58. One cascade chain implements one column vector and one row vector multiplication. This operation completes in 32 clocks.

Thus 32x32 matrix is the basic matrix multiplication unit. The design breaks down larger matrices into submatrices of size 32x32, and multiplies each 32x32 submatrix of Matrix A with each submatrix of Matrix B. For larger matrix multiplication, partial sum needs to be stored, read back, added to the new value and stored back.

</details>

<details>
  <summary>Directory Structure</summary>

### Directory Structure

```txt
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

- [AI Engine Development Design Process](https://www.xilinx.com/support/documentation-navigation/design-process/ai-engine-development.html)
- [AM009 AI Engine Architecture Manual](https://docs.amd.com/r/en-US/am009-versal-ai-engine/Revision-History)
- [Versal adaptive SoC AI Engines for Dummies](https://forums.xilinx.com/t5/Design-and-Debug-Techniques-Blog/Versal-ACAP-AI-Engines-for-Dummies/ba-p/1132493)

</details>

<details>
<summary>Installing the Tools</summary>

### Installing the Tools

- [AI Engine Documentation](https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html)

To build and run the GeMM tutorial (AI Engine and HLS implementations), perform the following steps:

- Obtain licenses for AI Engine tools.

</details>

<details>
<summary>Platform</summary>

### Platform

This tutorial targets the [VCK190 production board](https://www.xilinx.com/products/boards-and-kits/vck190.html). If you have already purchased this board, download the necessary files from the lounge and ensure you have the correct licenses installed. If you do not have a board and the required license, contact your AMD sales team.

</details>
<summary>Setting Up the Environment</summary>

### Setting Up the Environment

After installing the elements of the Vitis software platform, update the shell environment script. Set the environment variables to your system-specific paths.

To set up XRT, if you have not done this already, run the following command:

```bash
* source \<XRT-Location\>/setup.sh
```

In the design directory of each implementation, edit `sample_env_setup.sh` script with your file paths, then source the environment script:

```bash
source sample_env_setup.sh
```

The script sets up the environment variables and sources scripts explained in the following:

1. The `PLATFORM_REPO_PATHS` environment variable is based on where you downloaded the platform.
2. The `XILINX_TOOLS_LOCATION` path to the AMD tools is used to source the `settings64.sh` script.
3. The `XLNX_VERSAL` path to the `xilinx-versal-common-v2025.2` directory is used in the following step.
4. The platform is set up by running the `xilinx-versal-common-v2025.2/environment-setup-cortexa72-cortexa53-amd-linux` script as provided in the platform download This script sets up the `SDKTARGETSYSROOT` and `CXX` variables. If the script is not present, you *must* run the `xilinx-versal-common-v2025.2/sdk.sh` script.
5. `DSPLIB_VITIS` is the path to the downloaded Vitis Libraries. This is only required for the AI Engine implementation.
6. In the script, you can optionally set up an `XRT_ROOT` environment variable, pointing to XRT - RPMs, which the Vitis compiler packaging step can package. If you do not set it up, the packaging automatically excludes this environment variable.
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

The Makefile and source files for the AI Engine and HLS implementations are in the `AIE` and `DSP` directories respectively. For the documentation of the flow to build the design and details of the hardware and software design, follow each of these links:

- [AI Engine design implementation](AIE/README.md)
- [DSP design implementation with DSP Engines](DSP/README.md)

## AI Engine and DSP Implementation Comparison

The following table shows a comparison between a 1024 x 1024 x1024 GeMM design implemented using the AI Engines and DSP with DSP Engines, respectively. It lists the throughput, resource utilization, power consumption, and performance in throughput/Watt for `cint16` implementations.

| Design Target | TOPS<br/> | Average Latency (in μs) | AIE Vector Cores | AIE Vector Load | Active Mem Banks /<br/> Mem R/W Rate | Active AIE Tiles | FF (Regs) /<br/> CLB LUTs | Block RAMs | DSPs | Dynamic Power<br/>(in mW) | TOPS per Watt<br/>(in TOPS/Watt) |
|:-------------:|:---------:|:-----------------------:|:----------------:|:---------------:|:------------------------------------:|:----------------:|:-------------------------:|:-----:|:----:|:-------------------------:|:--------------------------------:|
| AIE           | 1.551     | 3.331                   | 24               | 82.96%          | 252 /<br/>13.980%                    | 46               | 26471 /<br/> 13546        | 66    | 0    | 4876                      | 0.320                            |
| DSP           | 1.433     | 1497.971                | NA               | NA              | NA                                   | NA               | 71086 /<br/> 17324        | 64    | 1024 | 8682                      | 0.165                            |

It is important to understand that those 46 AI Engines tiles are not all required for the GeMM compute: 24 AI Engines/vector cores compute the result, and 22 AI Engines store the matrices in memory and enable connectivity around the array. The average load on these additional 22 AI Engine tiles is 84.63%.

Measurement:

1. The AI Engine design resource utilization uses Xilinx Power Estimator (XPE) and AMD Vivado Design Suite (report utilization under implementation for FFs and CLB LUTs). For the HLS design, resource utilization uses Vivado tools.
2. AI Engine power consumption uses XPE. HLS power consumption uses Vivado tools (report power under implementation).
3. Throughput measurement uses viewing runtime profiling generated trace texts in `vitis_analyzer`.

For detailed instructions on taking measurements of the parameters, refer to the individual implementation section.

## References

- [Vitis Unified Software Development Platform Documentation](https://docs.amd.com/v/u/en-US/ug1416-vitis-documentation): Contains sections on how to develop AI Engine graphs, how to use the AI Engine compiler and AI Engine simulation, and performance analysis.
- [Vitis DSP Libraries](https://github.com/Xilinx/Vitis_Libraries/tree/master/dsp)
- [Vitis DSP Libraries Comprehensive Documentation](https://docs.amd.com/r/en-US/Vitis_Libraries/dsp/index.html)
- [Xilinx Runtime (XRT) Architecture](https://xilinx.github.io/XRT/master/html/index.html)
- [Vitis Unified Software Development Platform 2025.2 Documentation](https://docs.amd.com/v/u/en-US/ug1416-vitis-documentation)

Following are links to the XRT information used by this tutorial:

- [XRT Documentation](https://xilinx.github.io/XRT/master/html/index.html): Explains general XRT API calls used in the PS Host Application.

- [XRT Github Repo](https://github.com/Xilinx/XRT): Contains the XRT source code.

- [XRT AIE API](https://github.com/Xilinx/XRT/blob/master/src/runtime_src/core/include/experimental/xrt_aie.h): Documents the AI Engine XRT API calls

- [XRT Release Notes](https://www.xilinx.com/support/documentation/sw_manuals/xilinx2025_2/ug1451-xrt-release-notes.pdf)

Following are links to Vitis platform related information referenced in this tutorial:

- [Vitis Application Acceleration Development Flow Documentation](https://docs.amd.com/r/en-US/ug1702-vitis-accelerated-reference)

- [Vitis Application Acceleration Development Flow Tutorials](https://github.com/Xilinx/Vitis-Tutorials)

- [Vitis HLS](https://docs.amd.com/r/en-US/ug1399-vitis-hls)

## Known Issues

The timestamps represented in the hardware execution generated trace has a known issue that scales them compared to the actual values. Subsequent tool versions fix this issue. All calculations/observations in this tutorial use the hw_emu runs.

## Support

GitHub issues are used for tracking requests and bugs. For questions, go to [support.xilinx.com](http://support.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
