<table class="sphinxhide">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis™ - Runtime and System Optimization Tutorial</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

### Introduction to Vitis Runtime and System Optimization

## Introduction
Xilinx FPGAs and Versal ACAP devices are uniquely suitable for low-latency acceleration of high performance algorithms and workloads. With the demise of traditional Moore's Law scaling, design-specific architectures(DSAs) are becoming the tool of choice for developers needing the optimal balance of capability, power, latency, and flexibility. But, approaching FPGA and ACAP development from a purely software background can seem daunting.

With this set of documentation and tutorials, our goal is to provide you with an easy-to-follow, guided introduction to accelerating applications with Xilinx technology. We will begin from the first principles of acceleration: understanding the fundamental architectural approaches, identifying suitable code for acceleration, and interacting with the software APIs for managing memory and interacting with the target
device in an optimal way.

This set of documents is intended for use by software developers, it is not a low-level hardware developer's guide. The topics of RTL coding, low-level FPGA architecture, high-level synthesis optimization, and so on are covered elsewhere in other Xilinx documents. Our goal here is to get you up and running with Vitis quickly, with the confidence to approach your own acceleration goals and grow your familiarity and skill with Xilinx over time.

## Provided Design Files

In this directory tree you will find two a collection of documents and a directory named `design_source`.
The `design_source` directory contains all of the design elements - hardware and software - for the
tutorial you're currently reading. The example applications correspond to specific sections in the guide.
Every effort has been made to keep the code samples as concise and "to the point" as possible.

## Table of Contents

This tutorial is divided into several discrete example desings. Note that each design builds on the last,
so if this is youre first time here we recommend proceeding through the tutorial in order.

* [**Acceleration Basics**](./acceleration_basics.md) (~10 mins):
  + An overview of acceleration systems, Alveo, and XRT
    * See how Vitis takes care of the heavy lifting to let you focus on the application code
* [**Runtime SW Design**](./runtime_sw_design.md) (~10 mins):
  * An introduction to memory allocation, and how XRT interacts with your application
* [**Guided SW Examples**](./guided_sw_examples.md)
  + Initial setup for the following examples:

  + [**Example 0: Loading an Alveo Image**](./00-loading-an-alveo-image.md)
    + Learn how to program an FPGA or ACAP image into the device (in this case an Alveo card) using XRT
  + [**Example 1: Simple Memory Allocation**](./01-simple-memory-allocation.md)
    + What happens when you don't put too much thought into it?
  + [**Example 2: Aligned Memory Allocation**](./02-aligned-memory-allocation.md)
    + See the effects of allocating page-aligned memory vs. using standard `malloc()`
  + [**Example 3: XRT Memory Allocation**](./03-xrt-memory-allocation.md)
    + Compare using XRT-allocated memory to standard C++ allocators
  + [**Example 4: Parallelizing the Data Path**](./04-parallelizing-the-data-path.md)
    + You'll see the effect of keeping the same host software, but swapping to better hardware
  + [**Example 5: Optimizing Compute and Transfer**](./05-optimizing-compute-and-transfer.md)
    + In this example, we finally beat the CPU! Yay!
  + [**Example 6: Meet the Other Shoe**](./06-meet-the-other-shoe.md)
    + Alas, our victory was short lived! But why?
  + [**Example 7: Image Resizing with Vitis Vision**](./07-image-resizing-with-vitis-vision.md)
    + Let's make a less trivial application. Slightly less.
  + [**Example 8: Building Processing Pipelines with Vitis Vision**](./08-vitis-vision-pipeline.md)
    + And now, the true ultimate power of Vitis: pipelines!
    + Or: How I learned to love FPGAs


<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>
