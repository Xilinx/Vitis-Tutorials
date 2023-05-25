<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Optimizing Accelerated FPGA Applications: Bloom Filter Example

***Version: Vitis 2023.1***

## Introduction

The methodology for accelerating applications on an field programmable array (FPGA) is comprised of multiple phases:

- **Architecting the Application**: Make key decisions about the architecture of the application and decide some important factors, such as what software functions should be mapped to the FPGA kernels, how much parallelism is needed, and how it should be delivered.
- **Developing the Accelerator to Meet Your Desired Performance Goals**: Implement the kernel by modifying the source code and applying pragmas to create a kernel architecture that can achieve the desired performance goals.
- **Optimize the Host Code**: Review the application's access patterns, data movements, CPU and FPGA idle time, and update the host code to meet your performance goals.

You begin this tutorial with a baseline application, and you profile the application to examine the potential for hardware acceleration. The tutorial application involves searching through an incoming stream of documents to find the documents that closely match a user’s interest based on a search profile.

In general, a Bloom filter application has use cases in data analytics, such as browsing through unstructured email and text file data to identify the documents that are closely associated with a specific user and send notifications accordingly.

## Before You Begin

>**TIP**: This tutorial takes approximately two hours to complete.

The labs in this tutorial use:

- BASH Linux shell commands.
- 2023.1 Vitis core development kit release and the *xilinx_u200_gen3x16_xdma_2_202110_1* platform. If necessary, it can be easily ported to other versions and platforms.

This tutorial guides you to run the designed accelerator on the FPGA; therefore, the expectation is that you have an AMD Alveo™ U200 Data Center accelerator card set up to run this tutorial. Because it can take several (six or seven) hours to generate the multiple `xclbin` files needed to run the accelerator, pregenerated `xclbin` files are provided for the U200 card. To use these pregenerated files, when building the hardware kernel or running the accelerator on hardware, you need to add the `SOLUTION=1` argument.

>**IMPORTANT:**  
>
> - Before running any of the examples, make sure you have installed the Vitis core development kit as described in [Installation](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Installation-Requirements) in the Application Acceleration Development flow of the Vitis Unified Software Platform Documentation (UG1416).
>- If you run applications on Alveo cards, ensure the card and software drivers have been correctly installed by following the instructions on the [Alveo Portfolio page](https://www.xilinx.com/products/boards-and-kits/alveo.html).

### Accessing the Tutorial Reference Files

1. To access the tutorial content, enter the following in a terminal: `git clone http://github.com/Xilinx/Vitis-Tutorials`.
2. Navigate to the `Hardware_Acceleration/Design_Tutorials/02-bloom` directory.
    - `cpu_src` contains all the original source code before modification.
    - `images` contains the figures in this tutorial.
    - `Makefile` in the `makefile` directory explains the commands used in this lab. Use the `PLATFORM` variable if targeting different platforms.
    - `reference_file` contains the modified kernel and host-related files for achieving higher performance.
3. Copy and extract large files in as follows:

   ```
   wget https://www.xilinx.com/bin/public/openDownload?filename=xclbin_save.tar.gz -O xclbin_save.tar.gz
   tar -xvzf  xclbin_save.tar.gz
   ```

   >**TIP:** The `xclbin_save` contains the saved `xclbin` files that can be used directly for running on hardware by setting `SOLUTION=1` for the `make run` commands.

### Tutorial Outline

- [Overview of the Original Application](1_overview.md)
- [Experience Acceleration Performance](2_experience-acceleration.md)
- [Architecting the Application](3_architect-the-application.md)
- [Implementing the Kernel](4_implement-kernel.md)
- [Analyze Data Movement Between Host and Kernel](5_data-movement.md)
- [Using Multiple DDR Banks](6_using-multiple-ddr)

<!--
1. [Overview of the Original Application](1_overview.md): Provides a brief overview of the Bloom filter application with some examples of how this application is used in real-world scenarios.
2. [Experience Acceleration Performance](2_experience-acceleration.md): Profile the Bloom filter application and evaluate which sections are best suited for FPGA acceleration. You will also experience the acceleration potential by running the application first as a software-only version and then as an optimized FPGA-accelerated version.
3. [Architecting the Application](3_architect-the-application.md): In this lab, the original C++ based application computes scores for the documents using a Bloom filter. This lab also discusses setting realistic performance goals for an accelerated application. At the end of this lab, you will have a specification of the kernel based on the [Methodology for Accelerating Applications with the Vitis Software Platform](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Methodology-for-Accelerating-Data-Center-Applications-with-the-Vitis-Software-Platform).
4. [Implementing the Kernel](4_implement-kernel.md): Implement the kernel based on the specification from the previous lab, and run the compute part of the algorithm on the FPGA.
5. [Analyze Data Movement Between Host and Kernel](5_data-movement.md): Analyze the performance results of the kernel you generated. Working with a predefined FPGA accelerator, you learn how to optimize data movements between the host and FPGA, how to efficiently invoke the FPGA kernel, and how to overlap computation on the CPU and FPGA to maximize application performance.
6. [Using Multiple DDR Banks](6_using-multiple-ddr): Configure multiple DDR banks to improve the kernel performance.
-->

<hr/>

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
