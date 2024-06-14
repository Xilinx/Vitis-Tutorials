<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Performance Validation in Analysis View of the Vitis Unified IDE

***Version: Vitis 2024.1***

## Introduction

This tutorial targets AIE-ML device, but shares test cases and description files with [AIE 22-Peformance-Validation-in-Analysis-View](../../../AIE/Feature_Tutorials/22-Performance-Validation-in-Analysis-View).

**IMPORTANT**: Before beginning the tutorial ensure you have installed Vitis&trade; 2024.1 software. The software includes all the embedded base platforms including the VEK280 base platform that is used in this tutorial. In addition, ensure you have downloaded the Common Images for Embedded Vitis Platforms from this link.

https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms/2024.1.html

The 'common image' package contains a prebuilt Linux kernel and root file system that can be used with the Versal board for embedded design development using Vitis.
Before starting this tutorial run the following steps:

1. Navigate to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the ```/Common Images Dir/xilinx-versal-common-v2024.1/environment-setup-cortexa72-cortexa53-xilinx-linux``` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you must run the ```/Common Images Dir/xilinx-versal-common-v2024.1/sdk.sh```.
3. Set up your ROOTFS, and IMAGE to point to the ```rootfs.ext4``` and Image files located in the ```/Common Images Dir/xilinx-versal-common-v2024.1``` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to ```$XILINX_VITIS/lin64/Vitis/2024.1/base_platforms/```.

This tutorial targets VEK280 production board for 2024.1 version.

## Objectives

After completing this tutorial, you should be able to:

* Understand the simple Filter-Gain Application.
* See Average Throughput of the design reported after AI Engine Simulation. 
* Explore Analysis View of the Vitis Unified IDE for viewing the Throughput and Latency Reports.
* Use the XRT profiling APIs in the Hardware Flow for profiling Throughput.

## Tutorial Sections

This tutorial contains 3 namely *Design Overview* , *Throughput and Latency Measurement* and *Usage of XRT based event APIs*

Follow instructions in [Peformance-Validation-in-Analysis-View](../../../AIE/Feature_Tutorials/22-Performance-Validation-in-Analysis-View). But, run all make commands with ARCH=aie-ml.

### How to operate the Tutorial

Compile the default design (AI Engine, 4 antennas, 16 iterations, single rate) and simulate:
```bash
make clean data aie aiesim
```

Compile for AI Engine-ML, 4 antennas, 32 iterations and Multi-rate:
```bash
make clean ARCH=aie-ml NIterations=32 MULTIRATE=true clean aie aiesim
```
 After AI Engine Simulation open the summary with vitis_analyzer:

 ```bash
 vitis_analyzer aie/aiesimulator_output/default.aierun_summary
 ```

Build the design to generate the SD Card image for the VEK280: `make ARCH=aie-ml build_hw`