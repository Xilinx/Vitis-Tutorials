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

# My First Program

## Introduction

A Vitis core development kit application consists of a software program running on a host CPU and interacting with one or more accelerators running on a Xilinx FPGA. In this tutorial, you will learn how to write code for the host and for the accelerator, build the application using the Vitis compiler, and then run and profile the application. The accelerator used in this tutorial is a vector-add function.

The Makefile used in this tutorial has a set of commands to compile, link, and run the host and kernel. The Makefile also provides a capability to enable profiling features to visualize the results.

## Before You Begin

The labs in this tutorial use:

* BASH Linux shell commands.
* 2020.1 Vitis core development kit release and the *xilinx_u200_xdma_201830_2* platform. If necessary, it can be easily ported to other versions and platforms.

>**IMPORTANT:**  
>
> * Before running any of the examples, make sure you have installed the Vitis core development kit as described in [Installation](https://www.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=vhc1571429852245.html) in the Application Acceleration Development flow of the Vitis Unified Software Platform Documentation (UG1416).
>* If you run applications on Xilinx® Alveo™ Data Center accelerator cards, ensure the card and software drivers have been correctly installed by following the instructions on the [Alveo Portfolio page](https://www.xilinx.com/products/boards-and-kits/alveo.html).

### Accessing the Tutorial Reference Files

1. To access the reference files, type the following into a terminal: `git clone https://github.com/Xilinx/Vitis-Tutorials`.
2. Navigate to the `my-first-program` directory, and then access the `reference-files` directory.

## Next Steps

Complete the labs in the following order:

* [Coding My First C++ Kernel](./my-first-program/cpp_kernel.md)
* [Coding My First Host Program](./my-first-program/host_program.md)
* [Compiling and Linking the Application](./my-first-program/building_application.md)
* [Profiling the Application](./my-first-program/profile_debug.md)





<!--
1. [Coding My First C++ Kernel](./my-first-program/cpp_kernel.md): Create the hardware kernel to run on the accelerator card.
2. [Coding My First Host Program](./my-first-program/host_program.md): Create the host program code, including making the required API calls to run the kernel.
3. [Compiling and Linking the Application](./my-first-program/building_application.md): Build the host program and hardware kernel.
4. [Profiling the Application](./my-first-program/profile_debug.md): Profile and optimize the application design.

-->
</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="./README.md">Return to Main Page</a> — <a href="/docs/vitis-getting-started/README.md">Return to Getting Started Pathway</a></b></p>
</br>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
