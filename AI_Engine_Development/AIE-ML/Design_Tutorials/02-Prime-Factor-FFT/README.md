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

# Prime Factor FFT-1008 on AIE-ML

***Version: Vitis 2025.2***

## Table of Contents

1. [Introduction](#introduction)
2. [MATLAB Models](#matlab-models)
3. [Design Overview](#design-overview)
4. [Design Resources](#design-resources)
5. [Build and Run Design](#build-and-run-design)
6. [References](#references)
7. [Support](#support)
8. [License](#license)

## Introduction

The Prime Factor Algorithm (PFA) [[1]] is a Fast Fourier Transform (FFT) algorithm [[2]] discovered by Good & Thomas before the more popular Cooley-Tukey algorithm with some interesting properties. The PFA is another "divide and conquer" approach for computing a Discrete Fourier Transform (DFT) of size $N = N_1 \cdot N_2$ as a two-dimensional DFT of size $N_1 \times N_2$ as long as $N_1$ and $N_2$ are relatively prime (ie. share no common divisors). The smaller transforms of size $N_1$ and $N_2$ may be computed by some other technique, for example using the Winograd FFT Algorithm, or the PFA technique may be applied recursively again to both $N_1$ and $N_2$. It turns out AI Engines compute DFT with small dimensions $N < 32$ very efficiently using direct vector/matrix multiplication. Consequently, the PFA approach using DFT on the individual prime factors provides an efficient approach to the FFT on AI Engines.

A second advantage of the PFA approach is that unlike the popular Cooley-Tukey FFT, no extra multiplications by "twiddle factors" need be performed between stages. This fact falls out of the DFT factorization when $N_1$ and $N_2$ share no common factors. This provides a computational advantage compared to the more traditional Cooley-Tukey formulation, but the PFA incurs a drawback in that a complicated re-indexing or permutation of its I/O samples is required. For AMD Versal™ devices with both AI Engines and Programmable Logic (PL), however, this drawback is solved easily by leveraging the PL to implement these permutations as part of a custom data flow tailored to the PFA signal flow graph.

An [earlier tutorial](../../../AIE/Design_Tutorials/05-Prime-Factor-FFT) implemented a PFA-1008 transform on AIE architecture in the VC1902 device. This tutorial maps the PFA-1008 transform to AIE-ML architecture in the VE2802 device. The short-length DFT-7, DFT-9, and DFT-16 transforms are mapped to AI Engines using vector-matrix DFTs to the AIE-ML architecture. The intermediate "memory transpose" operations mapped earlier to the programmable logic (PL) are instead mapped here to the Memory Tiles contained in the AIE-ML array. This simplifies data flow and keeps most of the graph inside the array. The input and output permutation blocks remain implemented in the PL as RTL obtained using AMD Vitis™ High Level Synthesis (HLS) from untimed C++ models. These cannot be mapped to Memory Tiles as they require a type of modulo addressing not supported by the Memory Tile buffer descriptors (BDs).

## MATLAB Models

This tutorial relies on the same MATLAB® models from the [original tutorial](../../../AIE/Design_Tutorials/05-Prime-Factor-FFT). These models have been replicated here in the `matlab` directory of the repo. These apply to both the signal processing functions and the I/O permutations and matrix transpose addressing operations. All remain identical.

## Design Overview

The following figure shows a block diagram of a 3D PFA-1008 hardware design implemented in Versal devices using AI Engines and PL. The design targets a 1 Gsps throughput (SSR=1). AI Engines implement the three DFT kernels, specifically DFT-7, DFT-9 and DFT-16, using a vector-matrix multiplication approach. The design implements the matrix transpose kernels in the AI Engine array using Memory Tiles, and maps the I/O permutation blocks to PL using Vitis HLS.

![figure1](images/pfa-1008-block-diagram.png)

Some details on each kernel design are provided in the following sections.

### INPUT PERMUTE Kernel

This PL kernel is implemented in HLS @ 312.5 MHz (SSR=4). Samples arriving on one 128-bit stream are written into a ping/pong buffer in 4X duplicate fashion. This is required since the design must read or write four samples per cycle. The input permutation $P_i$ is stored in a LUT (again with 4X duplication) so the samples may be read back in the required permuted order. The latency of the design is 1008/4 cycles due to the ping/pong nature of the design. A single output stream delivers consecutive 7-pt transforms to the AI Engine array.

The following figure shows the input permutation required by the PFA-1008 design. The permutation ordering may be considered as a 3D mapping with $R=7$ rows, $C=9$ columns, and a depth of $D=16$. The $R$ dimension is given horizontally, the $C$ dimension vertically, and the $D$ dimension is identified by the "Tile-N" labels in the figure. The required permutation may be computed as $P=mod(C \times D \times R + R \times D \times C + R \times C \times D,1008)$. Note that this pattern cannot be generated automatically using the DMA buffer descriptors of the AI Engine Memory Tiles since it contains the "modulo 1008" operation which is not supported by the hardware. For this reason, the I/O permutation kernels are mapped to the PL.

![figure2](images/permute_i_kernel.png)

### FFT-7 Kernel

This AI Engine kernel implements the DFT-7 using the 1x4x8 `aie::mmul()` API which takes two to three cycles per operation. The complete DFT-7 compute may be partitioned over two compute tiles, one to compute the green portion and a second to compute the blue portion identified in the following figure. The API computes a [1x4] x [4x8] matrix multiply, and the DFT-7 must be padded with an extra row and column of zeros. The entire transform must be computed over seven cycles (SSR=1). The actual API code computes eight transforms in less than 56 cycles.

The algorithm is vectorized using a pair of 32-lane registers in each AIE-ML core. A set of eight vector reads fully populates these registers with data from eight consecutive transforms. Once populated, a set of `aie::shuffle_up()` API operations are used to position the data in the proper lanes for computes performed by the `aie::mmul()` API routine.

The outputs from eight consecutive transforms are then packed together into seven 8-lane vectors using a third tile to perform this output combining. The `aie::shuffle_up()` and `aie::shuffle_down()` APIs are used to perform this packing.

![figure3](images/dft7-kernel.png)

### TRANSPOSE-0 Kernel

This AIE kernel implements the matrix transpose operation required to feed the proper 9-point input samples to the DFT-9 on the second dimension of the 3D cube. For AIE-ML technology, this matrix transpose may be implemented completely within the array using the Memory tile eliminating the need to exit the array to perform the operation in the PL.

Buffer descriptions control the sample ordering employed by the Memory tile on both input and output. A "write BD" controls the sample ordering on input to the Memory tile. The write BD is configured using an ADF graph programming model shown below. The 3D pattern required here has dimensions ${7,9,16}$. This is configured with a `buffer_dimension` of ${8,16,16}$ since alignment to the 32-bit boundaries of the Memory tile must be ensured. The write address pattern is linear in dimensions 7, 9, and 16, and so the `tile_traversal` is configured in this order with address wrapping occuring at the dimensions $0,1,2$.

```cpp
tiling_parameters write_bd = {
      .buffer_dimension = {8,16,16},
      .tiling_dimension = {1,1,1},
      .offset = {0,0,0},
      .tile_traversal = {{.dimension=0, .stride=1, .wrap=7},
                         {.dimension=1, .stride=1, .wrap=9},
                         {.dimension=2, .stride=1, .wrap=16}} };
```

The read BD is configured using a similar ADF graph programming model as shown below. In this case, the required 3D pattern has dimensions ${9,16,7}$ because data is sent first along the second $N_2=9$ dimension, then electing to process $N_3=16$ second and $N_1=7$ last. This is configured with a `tile_traversal` along dimensions $1,2,0$ with wrapping applied as before.

Finally, a `repetition_count` of four is specified because both the write BD and the read BD must be repeated four times each to match the number of transforms computed per kernel invocation by each DFT-7, DFT-9 and DFT-16 AIE compute kernel. The `num_buffers` is set to 2 because a ping/pong buffer arrangement is required here to support a full streaming data flow model.

![figure4](images/transpose0-kernel.png)

### FFT-9 Kernel

This AI Engine kernel implements the DFT-9 using the 1x4x8 `aie::mmul()` API which takes two to three cycles per operation. The complete DFT-9 compute may be partitioned over three compute tiles, one to compute the pink portion, a second to compute the orange portion, and a third to compute the green portion identified in the following figure. The API computes a [1x4] x [4x8] matrix multiply, and the DFT-9 must be padded with extra rows and columns of zeros. The entire transform must be computed over nine cycles (SSR=1). The actual API code computes eight transforms in less than 72 cycles.

The algorithm is vectorized using three 32-lane registers in each AIE-ML core. A set of ten vector reads fully populates these registers with data from eight consecutive transforms. Once populated, a set of `aie::shuffle_up()` API operations are used to position the data in the proper lanes for computes performed by the `aie::mmul()` API routine.

The outputs from eight consecutive transforms are then packed together into nine 8-lane vectors using a fourth tile to perform this output combining. The `aie::shuffle_up()` and `aie::shuffle_down()` APIs are used to perform this packing.

![figure5](images/dft9-kernel.png)

### TRANSPOSE-1 Kernel

This AIE kernel implements the matrix transpose operation required to feed the proper 16-point input samples to the DFT-16 on the third dimension of the 3D cube. Once again the matrix transpose may be implemented completely within the array using the Memory tile.

The `write_bd` buffer descriptor here is configured to match the `read_bd` buffer descriptor from the TRANSPOSE-0 kernel to write back the transformed results into the same memory locations from which they are read from. This configuration is shown below.

```cpp
 tiling_parameters write_bd = {
      .buffer_dimension = {8,16,16},
      .tiling_dimension = {1,1,1},
      .offset = {0,0,0},
      .tile_traversal = {{.dimension=1, .stride=1, .wrap=9},
                         {.dimension=2, .stride=1, .wrap=16},
                         {.dimension=0, .stride=1, .wrap=7}} };
```

The `read_bd` buffer descriptor here is configured to deliver the samples required by the DFT-16 kernel that follows. Those samples are read of the third dimension of the 3D data cube, taken from dimensions $16,9,6$ corresponding to configurations $2,1,0$.

```cpp
tiling_parameters read_bd = {
      .buffer_dimension = {8,16,16},
      .tiling_dimension = {1,1,1},
      .offset = {0,0,0},
      .tile_traversal = {{.dimension=2, .stride=1, .wrap=16},
                         {.dimension=1, .stride=1, .wrap=9},
                         {.dimension=0, .stride=1, .wrap=7}} };
```

Once again, `repetition_count` is configured to 4 and `num_buffers` is configured to 2 for the reasons explained above.

![figure6](images/transpose1-kernel.png)

### FFT-16 Kernel

This AI Engine kernel implements the DFT-16 using a method similar to the DFT-7 and DFT-9 kernels above. Unlike those kernels, however, the approach here is simplified since the transform length is a multiple of eight. Only two compute tiles are needed and because there is no complicated vectorization, there are no data movement APIs required and no output combining.

![figure7](images/dft16-kernel.png)

### OUTPUT PERMUTE Kernel

This PL kernel is implemented in HLS @ 312.5 MHz (SSR=4). Samples arriving over a single 128-bit stream are written are written into a ping/pong buffer in 4X duplicate fashion in a manner similar to the INPUT PERMUTE kernel. The output permutation $P_o$ is stored in a LUT (again with 4X duplication) so the samples may be read back in the required permuted order. The latency of the design is 1008/4 cycles due to the ping/pong nature of the design. The output stream delivers the data for consumption by the DMA sink buffer.

![figure8](images/permute_o_kernel.png)

## Design Resources

The following figure summarizes the final AI Engine graph and floorplan for the PFA-1008 design in the VE2802 device on the VEK280 board. The design uses nine AI Engine tiles for compute kernels, 14 local tiles for buffering, and two Memory Tiles for shared buffers.

![figure9](images/aie-graph-floorplan.png)

The following figure summarizes the PL resources required to implement the design. The PL requires 50 block RAMs and four URAMs to implement the I/O permutes.It also requires the DMA source and sink buffers to support the design data flow between DDR4 and the AI Engine array. Timing closure is achieved with sufficient margin. This design illustrates how Versal AIE-ML technology can be crafted with the PL to create a high performance tightly-coupled custom data path tailored directly to the algorithm of interest.

![figure9](images/pl-floorplan-resources.png)

## Build and Run Design

### Setup and Initialization

>**IMPORTANT:** Before beginning the tutorial, install the Vitis 2025.2 software platform. Also, download the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html).

Set the environment variable ```COMMON_IMAGE_VERSAL``` to the full path where you have downloaded the Common Images. Then set the environment variable ```PLATFORM_REPO_PATHS``` to the value ```$XILINX_VITIS/base_platforms```. You can find additional information on this process [here](../../../AIE#environment-settings).

The remaining environment variables are configured in the top level Makefile ```<path-to-design>/02-Prime-Factor-FFT/Makefile``` file.

```makefile
RELEASE=2025.2
BOARD=vek280
BASE_NUM=202520_1

# Platform Selection...
VERSAL_VITIS_PLATFORM      = xilinx_${BOARD}\_base_${BASE_NUM}
VITIS_PLATFORM_DIR         = ${PLATFORM_REPO_PATHS}/${VERSAL_VITIS_PLATFORM}
export VITIS_PLATFORM_XPFM = ${VITIS_PLATFORM_DIR}/${VERSAL_VITIS_PLATFORM}.xpfm

# Set SysRoot, RootFS and Image
export VITIS_SYSROOTS   = ${COMMON_IMAGE_VERSAL}/sysroots/cortexa72-cortexa53-amd-linux
export KERNEL_IMAGE     = ${COMMON_IMAGE_VERSAL}/Image
export ROOTFS           = ${COMMON_IMAGE_VERSAL}/rootfs.ext4
export XLNX_VERSAL      = ${COMMON_IMAGE_VERSAL}
export PLATFORM         = ${VITIS_PLATFORM_XPFM}
export SYSROOT          = ${VITIS_SYSROOT}
```

### Hardware Emulation

```bash
[shell]% cd <path-to-design>/02-Prime-Factor-FFT
[shell]% make all TARGET=hw_emu
```

This takes about 90 minutes to run the first time as the build performs C/RTL verification and AMD Vivado™ out-of-context place-and-route runs for all six HLS IP cores. This only occurs one time, but is not required for hardware emulation. To skip this step, run the following command:

```bash
[shell]% cd <path-to-design>/02-Prime-Factor-FFT
[shell]% make all TARGET=hw_emu DO_COSIM=false
```

The build process generates a directory ```02-Prime-Factor-FFT/package``` containing all the files required for hardware emulation. You can run this as shown below. You can apply an optional `-g` flag to the ```launch_hw_emu.sh``` command to launch the Vivado waveform GUI to observe the top-level AXI signal ports in the design.

```bash
[shell]% cd <path-to-design>/02-Prime-Factor-FFT/package
[shell]% ./launch_hw_emu.sh -run-app embedded_exec.sh
```

### Hardware

Build the design for the VEK280 board using the Makefile as follows:

```bash
[shell]% cd <path-to-design>/02-Prime-Factor-FFT
[shell]% make all TARGET=hw
```

The build process generates the SD card image in the ```<path-to-design>/02-Prime-Factor-FFT/package/sd_card``` directory.

## References

[1]: <https://en.wikipedia.org/wiki/Prime-factor_FFT_algorithm> "Prime Factor FFT Algorithm"

[[1]] Wikipedia, "[Prime Factor FFT Algorithm]([https://en.wikipedia.org/wiki/Prime-factor_FFT_algorithm])"

[2]: <https://eng.libretexts.org/Bookshelves/Electrical_Engineering/Signal_Processing_and_Modeling/Fast_Fourier_Transforms_(Burrus)> "Fast Fourier Transforms"

[[2]] C. Sidney Burrus, "[Fast Fourier Transforms]([https://eng.libretexts.org/Bookshelves/Electrical_Engineering/Signal_Processing_and_Modeling/Fast_Fourier_Transforms_(Burrus)])"

## Support

GitHub issues are used for tracking requests and bugs. For questions, go to [forums](https://adaptivesupport.amd.com/s/?language=en_US).

## License

Components: xilinx-images

images in the documentation

Components: xilinx-files

The MIT License (MIT)

Copyright (c) 2023 Advanced Micro Devices, Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>
<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
