<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Application Acceleration Development Flow Tutorials</h1>

   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# Vitis™ Getting Started Tutorial

***Version: Vitis 2022.1***

![img](./images/vitis_101.png)

Welcome to the Vitis Getting Started tutorial. The Vitis tool provides a unified flow for developing FPGA accelerated application targeted to either Data Center accelerator cards or Embedded Processor platforms. If you are looking to learn more about the Vitis application acceleration development flow in order to get started with FPGA acceleration, you have come to the right place.

This tutorial is divided into two separate flows: the [Data Center flow](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Data-Center-Application-Acceleration-Development-Flow), and the [Embedded Processor flow](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Embedded-Processor-Application-Acceleration-Development-Flow). These two flows are similar in that host applications and accelerated kernels written for one flow can be used in the other flow, and the build processes are similar. However, while similar the flows are also different in that the build and runtime environments of Data Center accelerator cards and Embedded Processor platforms have different requirements that must be met.

This tutorial provides instructions for building and running on both the Alveo U200 Data Center accelerator card, and the Zynq Ultrascale MPSoC ZCU102 platform. These instructions can be easily adapted to other Xilinx cards. 

The two flows in this tutorial are both organized into 5 parts and are designed to walk you through all the key aspects of the Vitis flow.

* **Part 1** covers all the essential concepts of the Vitis FPGA acceleration flow in under 10 minutes

* **Part 2** guides you through the process of installing the Vitis tools, platforms and runtime library.

* **Part 3** explains the source code of vector-add example used in the rest of the tutorial

* **Part 4** describes the Data Center flow and the Embedded Platform flow. Each flow includes the commands required to compile, link and run the example on your acceleration card

* **Part 5** gives an overview of Vitis Analyzer and shows how to open and analyze reports

## Part 1 : Essential Concepts

The Vitis unified software platform provides a framework for developing and delivering FPGA accelerated applications using standard programming languages like C and C++. The Vitis flow offers all of the features of a standard software development environment, including:

* Compiler or cross-compiler for host applications running on x86 or Arm® processors
* Cross-compilers for building the FPGA binary
* Debugging environment to help identify and resolve issues in the code
* Performance profilers to identify bottlenecks and help you optimize the application

### Understanding the Vitis Programming and Execution Model

A Vitis accelerated application consists of two distinct components: a software program running on a standard processor such as an X86 processor, or ARM embedded processor, and a Xilinx device binary (`xclbin`) containing hardware accelerated functions, or kernels.

* The software program, or host application, is written in C/C++ and runs on a conventional CPU. The software program uses the [XRT native API](https://xilinx.github.io/XRT/master/html/index.html) implemented by the Xilinx Runtime library (XRT), or OpenCL 1.2 C/C++ API to interact with the acceleration kernel in the Xilinx device. A description of the host application and required API calls can be found in the Vitis documentation under [Host Programming](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Host-Programming).

* The hardware accelerated kernels can be written in C/C++ or RTL (Verilog or VHDL) and run within the programmable logic part of the Xilinx device. Refer to [C/C++ Kernels](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/C/C-Kernels), or [RTL Kernels](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/RTL-Kernels) in the Vitis documentation for coding requirements. The kernels are integrated with a Vitis hardware platform using standard AXI interfaces.

![img](./images/part1_execution_model.png)

Vitis accelerated applications can execute on either Data Center or Embedded Processor acceleration platforms:

* On Data Center accelerator cards, the software program runs on an x86 server and the kernels run in the FPGA on a PCIe-attached acceleration card.

* On Embedded Processor platforms, the software program runs on an ARM processor of a Xilinx MPSoC device and the kernels run within the same device. 

Because the software and hardware components of a Vitis application use standardized interfaces (XRT APIs and AXI protocols) to interact with each other, the user's source code remains mostly agnostic of platform-specific details and can be easily ported across different acceleration platforms.

There are multiple ways by which the software program can interact with the hardware kernels. The simplest method can be decomposed into the following steps:

1. The host application writes the data needed by a kernel into the global memory of the FPGA device.
2. The host program sets up the input parameters of the kernel.
3. The host program triggers the execution of the kernel.
4. The kernel performs the required computation, accessing global memory to read or write data, as necessary. Kernels can also use streaming connections to communicate with other kernels, passing data from one kernel to the next.
5. The kernel notifies the host that it has completed its task.
6. The host program transfers data from global memory back into host memory, or can give ownership of the data to another kernel.

### Understanding the Vitis Build Process

The Vitis build process follows a standard compilation and linking process for both the host program and the kernel code:

* The host program is built using the GNU C++ compiler (g++) for Data Center applications or the GNU C++ Arm cross-compiler for Embedded Processor devices.

* The FPGA binary is built using the Vitis compiler (v++). First the kernels are compiled into a Xilinx object (.xo) file. Then, the .xo files are linked with the hardware platform to generate the Xilinx device binary (.xclbin) file. As described in [Vitis Compiler Command](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Vitis-Compiler-Command), the Vitis compiler and linker accepts a wide range of options to tailor and optimize the results.

![img](./images/part1_build_flow.png)

### Understanding Vitis Build Targets

The Vitis compiler provides three different build targets: two emulation targets used for debug and validation purposes, and the default hardware target used to generate the actual FPGA binary:

* Software Emulation - The kernel code is compiled to run on the host processor. This allows iterative algorithm refinement through fast build-and-run loops. This target is useful for identifying syntax errors, performing source-level debugging of the kernel code running together with application, and verifying the behavior of the system.

* Hardware Emulation - The kernel code is compiled into a hardware model (RTL), which is run in a dedicated simulator. This build-and-run loop takes longer but provides a detailed, cycle-accurate view of kernel activity. This target is useful for testing the functionality of the logic that will go in the FPGA and getting initial performance estimates.

* Hardware - The kernel code is compiled into a hardware description language (RTL), and then synthesized and implemented for a target Xilinx device, resulting in a binary (`xclbin`) file that will run on the actual FPGA.

>**TIP**: As described in [Running Emulation](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Running-Emulation), there are significant differences in the build and runtime environments between Data Center and Embedded Processor platforms. These two flows will be discussed in detail in the following sections.

## Next Steps

  **Click here for [Installation and Setup Instructions](./Part2.md)**

*************************************
License statement

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020–2022 Xilinx</sup></p>
