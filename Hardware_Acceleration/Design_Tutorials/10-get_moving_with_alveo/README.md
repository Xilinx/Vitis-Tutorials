<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Hardware Acceleration Introduction Tutorial</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Get Moving with Alveo

## Introduction

***Version: Vitis 2023.1***

AMD FPGAs and AMD Versal™ adaptive SoC devices are uniquely suitable for low-latency acceleration of high-performance algorithms and workloads. With the demise of traditional Moore's Law scaling, design-specific architectures (DSAs) are becoming the tool of choice for developers needing the optimal balance of capability, power, latency, and flexibility. But, approaching FPGA and Versal adaptive SoC development from a purely software background can seem daunting.

With this set of documentation and tutorials, our goal is to provide you with an easy-to-follow, guided introduction to accelerating applications with AMD technology. We will begin from the first principles of acceleration: understanding the fundamental architectural approaches, identifying suitable code for acceleration, and interacting with the software APIs for managing memory and interacting with the target
device in an optimal way.

This set of documents is intended for use by software developers; it is not a low-level hardware developer's guide. The topics of RTL coding, low-level FPGA architecture, high-level synthesis optimization, and so on are covered elsewhere in other AMD documents. Our goal here is to get you up and running with Vitis quickly, with the confidence to approach your own acceleration goals and grow your familiarity and skill with AMD over time.

## Provided Design Files

In this directory tree you will find a collection of documents and a directory named `design_source`.
The `design_source` directory contains all of the design elements — hardware and software.The example applications correspond to specific sections in the guide.
Every effort has been made to keep the code samples as concise and "to the point" as possible.

## Build the Software and Hardware

The attached `Makefile` can be used to build the design:

Command       |                    Run Result                            |
--------------|----------------------------------------------------------|
`make run TARGET=sw_emu` | Build and run Software Emulation             |
`make run TARGET=hw_emu` | Build and run Hardware Emulation             |
`make run TARGET=hw`     | Build and run the application on Hardware    |

The above Makefile flow would need `PLATFORM` and `PLATFORM_REPO_PATH`.

```
export PLATFORM=xilinx_u250_gen3x16_xdma_4_1_202210_1 # Example to set the PLATFORM before launching the run 
export PLATFORM_REPO_PATH=/opt/xilinx/platforms/      # Example to set the PLATFORM_REPO_PATH before launching the run
```

This tutorial uses OpenCV for the application, set the OpenCV `env` variable as follows:

```
export OPENCV_INCLUDE=<path>/opencv-4.4.0/include
export OPENCV_LIB=<path>/opencv-4.4.0/lib
export LD_LIBRARY_PATH=<path>/opencv-4.4.0/lib:$LD_LIBRARY_PATH
```

Once the build is completed, a folder `build` under directory `design_source` will contain all the executables and `alveo_example.xclbin` to run the tutorial.

Note: The `hw_emu` is not tested for this release.

## Table of Contents

This tutorial is divided into several discrete example designs. Note that each design builds on the last one, so if this is your first time here, we recommend proceeding through the tutorial in order.

* [**Acceleration Basics**](./acceleration_basics.md) (~10 mins):
  * An overview of acceleration systems, AMD Alveo™, and Xilinx Runtime (XRT)
    * See how Vitis takes care of the heavy lifting to let you focus on the application code.
* [**Runtime SW Design**](./runtime_sw_design.md) (~10 mins):
  * An introduction to memory allocation, and how XRT interacts with your application.
* [**Guided SW Examples**](./guided_sw_examples.md)
  * Initial setup for the following examples:

  * [**Example 0: Loading an Alveo Image**](./00-loading-an-alveo-image.md)
    * Learn how to program an FPGA or adaptive SoC image into the device (in this case, an Alveo card) using XRT.
  * [**Example 1: Simple Memory Allocation**](./01-simple-memory-allocation.md)
    * What happens when you do not put too much thought into it?
  * [**Example 2: Aligned Memory Allocation**](./02-aligned-memory-allocation.md)
    * See the effects of allocating page-aligned memory versus using standard `malloc()`.
  * [**Example 3: XRT Memory Allocation**](./03-xrt-memory-allocation.md)
    * Compare using XRT-allocated memory to standard C++ allocators.
  * [**Example 4: Parallelizing the Data Path**](./04-parallelizing-the-data-path.md)
    * You will see the effect of keeping the same host software but swapping to better hardware.
  * [**Example 5: Optimizing Compute and Transfer**](./05-optimizing-compute-and-transfer.md)
    * In this example, we finally beat the CPU! Yay!
  * [**Example 6: Meet the Other Shoe**](./06-meet-the-other-shoe.md)
    * Alas, our victory was short lived! But why?
  * [**Example 7: Image Resizing with Vitis Vision**](./07-image-resizing-with-vitis-vision.md)
    * Let's make a less trivial application. Slightly less.
  * [**Example 8: Building Processing Pipelines with Vitis Vision**](./08-vitis-vision-pipeline.md)
    * And now, the true ultimate power of Vitis: pipelines!
    * Or: How I learned to love FPGAs

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
