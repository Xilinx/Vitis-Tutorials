<p align="right" class="sphinxhide">
<a>English</a> | <a href="/docs-jp/README.md">日本語</a>
</p>

<table class="sphinxhide">
  <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ Application Acceleration Development Flow Tutorials</h1>
    <a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">See 2019.2 Vitis Application Acceleration Development Flow Tutorials</a>
    </td>
 </tr>
 </table>

# Getting Started

## Developing Accelerated Applications using the Vitis Core Development Kit

![Pathways](images/pathway.png)

### 1. Programming and Execution Model

In the Vitis™ unified software platform, an application is split into a host program and hardware accelerated kernels, with a communication channel between them for data transfer. The host application, written in C/C++ and using API calls like OpenCL™, runs on an x86 server, or Arm™ embedded processor; the hardware accelerated kernels run within the Xilinx FPGA on a target platform, such as an Alveo™ Data Center accelerator card.

[Read more...](../vitis-execution-model/README.md)

### 2. Setting up the Alveo Data Center Accelerator Cards and Vitis Core Development Kit

The Alveo Data Center accelerator cards provide compute acceleration performance and flexibility for Data Centers looking to increase throughput. You can install Alveo cards in deployment systems for running accelerated applications, or in development systems, you can develop, debug, and optimize applications running on Alveo cards.

[Read more...](../alveo-getting-started/README.md)

### 3. Building the Accelerated Application — Essential Concepts

The following labs will introduce you to the essential concepts for building and running an accelerated application using the Vitis core development kit.

In this tutorial, you will learn how to do the following:

- Build an application's host software and the hardware kernel.
- Run hardware and software emulation on an application.
- Learn how to generate application profiling reports to better understand an application's performance.  
- Execute an application on an accelerator card.

[Read more...](../Pathway3/README.md)

### 4. Your First Program

After learning the essential concepts, you will work through the steps of creating a simple Vitis core development kit application by building the x86 host program and kernel code (compute device), and use a Makefile to build the design. The Vitis core development kit application consists of a host x86 CPU and hardware kernel running on a Xilinx FPGA on an Xilinx Alveo Data Center accelerator card. The kernel code in this example performs a simple vector addition, `C[i]= A[i]+ B[i]`. This tutorial also demonstrates the host code structure and required API calls in detail.

In this tutorial, you will learn how to do the following:

- Create a simple hardware kernel code to run on the accelerator card.
- Create a simple host program code, including making the required API calls to run the kernel.
- Build the host program and hardware kernel.
- Profile and optimize the application design.

[Read more...](../my-first-program/README.md)

### 5. Optimizing Accelerated FPGA Applications — Based on Vitis Environment Methodology

The methodology for developing optimized accelerated applications is comprised of two major phases: architecting the application, and developing the hardware kernels. In the first phase, you make key decisions about the application architecture by determining which software functions should be accelerated onto FPGA kernels, how much parallelism can be achieved, and how to deliver it in code. In the second phase, you implement the kernels by structuring the source code, and applying the necessary compiler options and pragmas to create the kernel architecture needed to achieve the optimized performance target.

* [Convolution Example](../convolution-tutorial/README.md)

## Additional Resources

For more training and in-depth documentation on the topics discussed in this pathway, refer to the following resources.

* [Methodology for Accelerating Applications with the Vitis Unified Software Platform](https://www.xilinx.com/cgi-bin/docs/rdoc?t=vitis+doc;v=2020.1;d=methodologyacceleratingapplications.html)
* [Xilinx Customer Training: Vitis Unified Software Development Environment](https://xilinxprod-catalog.netexam.com/Search?searchText=vitis)

</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href=./README.md">Return to Main Page</a></b></p>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
