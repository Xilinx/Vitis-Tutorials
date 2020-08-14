﻿
<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ Application Acceleration Development Flow Tutorials</h1>
   <a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">See 2019.2 Vitis Application Acceleration Development Flow Tutorials</a>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# Optimizing Accelerated FPGA Applications: Bloom Filter Example

## Introduction

The methodology for accelerating applications on an FPGA is comprised of multiple phases:

   - **Architecting the application**: Make key decisions about the architecture of the application and decide some important factors, such as what software functions should be mapped to FPGA kernels, how much parallelism is needed, and how it should be delivered.
   - **Developing the accelerator to meet your desired performance goals**: Implement the kernel by modifying the source code and applying pragmas to create a kernel architecture that can achieve the desired performance goals.
   - **Optimize the host code**: Review the application's access patterns, data movements, CPU and FPGA idle time, and update the host code to meet your performance goals.

You begin this tutorial with a baseline application, and you profile the application to examine the potential for hardware acceleration. The tutorial application involves searching through an incoming stream of documents to find the documents that closely match a user’s interest based on a search profile.

In general, a Bloom filter application has use cases in data analytics, such as browsing through unstructured email and text file data to identify the documents that are closely associated with a specific user and send notifications accordingly.

## Before You Begin

>**TIP**: This tutorial takes approximately two hours to complete.

The labs in this tutorial use:

* BASH Linux shell commands.
* 2020.1 Vitis core development kit release and the *xilinx_u200_xdma_201830_2* platform. If necessary, it can be easily ported to other versions and platforms.

This tutorial guides you to run the designed accelerator on the FPGA; therefore, the expectation is that you have an Xilinx® Alveo™ U200 Data Center accelerator card set up to run this tutorial. Because it can take several (six or seven) hours to generate the multiple `xclbin` files needed to run the accelerator, pregenerated `xclbin` files are provided for the U200 card. To use these pregenerated files, when building the hardware kernel or running the accelerator on hardware, you need to add the `SOLUTION=1` argument. 

>**IMPORTANT:**  
>
> * Before running any of the examples, make sure you have installed the Vitis core development kit as described in [Installation](https://www.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=vhc1571429852245.html) in the Application Acceleration Development flow of the Vitis Unified Software Platform Documentation (UG1416).
>* If you run applications on Alveo cards, ensure the card and software drivers have been correctly installed by following the instructions on the [Alveo Portfolio page](https://www.xilinx.com/products/boards-and-kits/alveo.html).

### Accessing the Tutorial Reference Files

1. To access the tutorial content, enter the following in a terminal: `git clone http://github.com/Xilinx/Vitis-Tutorials`.
2. Navigate to the `bloom` directory.
    * `Makefile` under the `makefile` directory explains the commands used in this lab. Use the `PLATFORM` variable if targeting different platforms.
    * `cpu_src` contains all the original source code before modification.
    * `reference_file` contains the modified kernel and host-related files for achieving higher performance.
    * `xclbin_save` contains the saved `xclbin` files that can be used directly for running on hardware by setting `SOLUTION=1` for the `make run` commands.

### Tutorial Overview

* [Overview of the Original Application](1_overview.md)
* [Experience Acceleration Performance](2_experience-acceleration.md)
* [Architecting the Application](3_architect-the-application.md)
* [Implementing the Kernel](4_implement-kernel.md)
* [Analyze Data Movement Between Host and Kernel](5_data-movement.md)
* [Using Multiple DDR Banks](6_using-multiple-ddr)

<!--
1. [Overview of the Original Application](1_overview.md): Provides a brief overview of the Bloom filter application with some examples of how this application is used in real-world scenarios.
2. [Experience Acceleration Performance](2_experience-acceleration.md): Profile the Bloom filter application and evaluate which sections are best suited for FPGA acceleration. You will also experience the acceleration potential by running the application first as a software-only version and then as an optimized FPGA-accelerated version.
3. [Architecting the Application](3_architect-the-application.md): In this lab, the original C++ based application computes scores for the documents using a Bloom filter. This lab also discusses setting realistic performance goals for an accelerated application. At the end of this lab, you will have a specification of the kernel based on the [Methodology for Accelerating Applications with the Vitis Software Platform](https://www.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=methodologyacceleratingapplications.html#wgb1568690490380).
4. [Implementing the Kernel](4_implement-kernel.md): Implement the kernel based on the specification from the previous lab, and run the compute part of the algorithm on the FPGA.
5. [Analyze Data Movement Between Host and Kernel](5_data-movement.md): Analyze the performance results of the kernel you generated. Working with a predefined FPGA accelerator, you learn how to optimize data movements between the host and FPGA, how to efficiently invoke the FPGA kernel, and how to overlap computation on the CPU and FPGA to maximize application performance.
6. [Using Multiple DDR Banks](6_using-multiple-ddr): Configure multiple DDR banks to improve the kernel performance.
-->

<hr/>
<p align= center class="sphinxhide"><b><a href="/README.md">Return to Main Page</a> — <a href="/docs/vitis-getting-started/">Return to Getting Started Pathway</a></b></p>
</br><p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>

