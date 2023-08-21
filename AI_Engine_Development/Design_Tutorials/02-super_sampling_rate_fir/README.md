<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Super Sampling Rate FIR Filters: Implementation on the AI Engine

***Version: Vitis 2023.1***


## Introduction

AMD Versal&trade; adaptive SoC AI Core Series are heterogeneous devices containing many domains with compute capabilities. With respect to Digital Signal Processing (DSP) and particularly Finite Impulse Response (FIR) filters, the two domains of interest are:

- The Programmable Logic (PL), which is the "classical" domain of AMD devices.
- The AI Engine Processor Array, which is a new domain within Versal adaptive SoC AMD devices

FIR filter architecture is a rich and fruitful electrical engineering domain, especially when the input sampling rate becomes higher than the clock rate of the device (Super Sampling Rate or SSR). For the PL, there exists a number of solutions that are already available using turnkey IP solution (FIR Compiler). The AI Engine array is a completely new processor and processor array architecture with enormous compute capabilities, so an efficient filtering architecture has to be found using all the capabilities of the AI Engine array, but also all the communications that are possible with the PL.

The purpose of this tutorial is to provide a methodology to enable you to make appropriate choices depending on the filter characteristics, and to provide examples on how to implement Super Sampling Rate (SSR) FIR Filters on a Versal adaptive SoC AI Engine processor array.

## Before You Begin

Before beginning this tutorial, you should be familiar with Versal adaptive SoC architecture and more specifically on the AI Engine array processor and interconnect architecture.

**IMPORTANT**: Before beginning the tutorial, make sure that you have installed the Vitis 2023.1 software.  The AMD Vitis&trade; release includes all the embedded base platforms, including the VCK190 base platform that is used in this tutorial. In addition, ensure that you have downloaded the Common Images for Embedded Vitis Platforms from this link [https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms/2023-1.html](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms/2023-1.html).
The ‘common image’ package contains a prebuilt Linux kernel and root file system that can be used with the Versal board for embedded design development using Vitis.
Before starting this tutorial, run the following steps:

1. Go to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the `/Common Images Dir/xilinx-versal-common-v2023.1/environment-setup-cortexa72-cortexa53-xilinx-linux` script. This script sets up the `SDKTARGETSYSROOT` and `CXX` variables. If the script is not present, run the `/Common Images Dir/xilinx-versal-common-v2023.1/sdk.sh`.
3. Set up your `ROOTFS` and `IMAGE` to point to the `rootfs.ext4` and `Image` files located in the `/Common Images Dir/xilinx-versal-common-v2023.1` directory.
4. Set up your `PLATFORM_REPO_PATHS` environment variable to `$XILINX_VITIS/lin64/Vitis/2023.1/base_platforms/xilinx_vck190_base_dfx_202310_1/xilinx_vck190_base_dfx_202310_1.xpfm`.
This tutorial targets VCK190 production board for 2023.1 version.

Data generation for this tutorial requires Python:
* [Python 3](https://www.python.org/downloads/)
    * Packages: math, shutils, functools, matplotlib, numpy, random, subprocess

### Accessing the Tutorial Reference Files

1. To access the reference files, type the following into a terminal: `git clone https://github.com/Xilinx/Vitis-Tutorials.git`.
2. Navigate to the `Vitis-Tutorials/AI_Engine_Development/Design_Tutorials/02-super_sampling_rate_fir/` directory, and type `source addon_setup.sh` to update the path for Python libraries and executable.

You can now start the tutorial.

## SSR FIR Tutorial

This tutorial is decomposed into multiple steps:

1. [Summary of AI Engine Architecture](#AIE_Architecture)
2. [What is a FIR Filter?](#FIR_Filter)
3. ["Utils" directory](#UtilsDirectory)
4. [Single Kernel FIR](SingleKernel/README.md)
5. [Multi-Kernel FIR](MultiKernel/README.md)
6. Polyphase FIR (SSR)
   1. [Single Stream](SingleStreamSSR/README.md)
   2. [Double Stream](DualStreamSSR/README.md)


<a name="AIE_Architecture"></a>

## Summary of AI Engine Architecture

You should have already read the [AI Engine Detailed Architecture](https://www.xilinx.com/support/documentation/architecture-manuals/am009-versal-ai-engine.pdf), so the purpose of this chapter is simply to highlight the features of the AI Engine that are useful for this tutorial.

Versal adaptive SoCs combine Scalar Engines, Adaptable Engines, and Intelligent Engines with leading-edge memory and interfacing technologies to deliver powerful heterogeneous acceleration for any application.

![missing image](./Images/Versal.jpg)

Intelligent Engines are SIMD VLIW AI Engines for adaptive inference and advanced signal processing compute.

DSP Engines are for fixed point, floating point, and complex MAC operations.

The SIMD VLIW AI Engines come as an array of interconnected processors using the AXI-Stream interconnect blocks as shown in the following figure:

![missing image](./Images/AIEngineArray.jpg)

All arrays (processors, memory modules, AXI interconnects) are driven by a single clock. The slowest speed grade device can run @1 GHz. The highest speedgrade allows 1.3 GHz clock rates. The AI Engine allows for numerous connection possibilities with the surrounding environment as shown in the following figure.

![missing image](./Images/AIEngine.jpg)

### Memory interface

![missing image](./Images/AIE_MemIF.jpg)

Each AI Engine is surrounded by 4x 32 kB memories, each one being divided in four pairs of banks. The bandwidth is  high:

- 2 reads / cycle on 32 bytes (256 bits) each
  - Each bank has a single port, the accesses must be done on different banks to achieve 2x 256 bits/cycle.
- 1 write / cycle on 32 bytes (256 bits)
  - On another bank to achieve the highest bandwidth.
- Be aware that you need also to feed the memories using DMAs or other AI Engines.


### Streaming interface

![missing image](./Images/AIE_Streams.jpg)

The streaming interface is based on two incoming streams and two outgoing streams, each one on 32 bits per clock cycle. These four streams are handled by a stream FIFO that allows the processor to use different bitwidths to access these streams:

- 2 streams in, 2 streams out:
  - Each one 4 bytes/cycle or 16 bytes/ 4 cycles
- Parallel access to streams per VLIW:
  - 2 reads (4/16 bytes), 1 write (4/16 bytes)
  - OR 1 read (4/16 bytes), 2 writes (4/16 bytes)
- Using 1 stream:
  - 4 bytes/cycle read and 4 bytes/cycle write
- Using the 2 streams and the 16-byte access option:
  - Reads and/or writes can be dispatched over time
  - On an average 8 bytes/cycle read and 8 bytes/cycle write

Accessing the data to/from the streams using the 128-bit interface does not increase the bandwidth, but limits the number of accesses that must be scheduled within the microcode of the VLIW processor.

### Cascade Streams

![missing image](./Images/AIE_Cascade.jpg)

The cascade stream allows an AI Engine processor to transfer the value of some of its accumulator register (384 bits) to its neighbor (on the left or right depending on the row):

- It is capable of 8x 48-bit word transfer v8acc48 or v4cacc48 in a single cycle.
- 48 bits is the number of bits of the result of a 16 bits x 16 bits multiplication.
- If the transfer concerns a 768-bit register, it takes 2 clock cycles.


<a name="FIR_Filter"></a>

## What is a FIR Filter?

The purpose of this tutorial is not to train you to be an expert in Digital Signal Processing. Howver, to grasp the basics of FIR filtering, it is necessary to understand the computations that are required, and the data that is consumed and produced by the compute block.

A digital signal is an analog signal (audio, radio frequencies) that has been received by a converter (Analog to Digital Converter: ADC), which performs two operations:

- **Slicing**: The impinging signal is sliced into very small time slots on which its amplitude is approximated by a constant value.
- **Quantizing**: Digital systems understand only bits. The constant value at the output of the slicer is transformed into an integer value whose maximum represents the maximum amplitude that the system can receive.

As a result, the digital signal at the output of the ADC is simply a series of *N*-bits values (called samples) that can be processed to extract some useful information. The most basic operation is to multiply some samples by some specific coefficients and accumulate these values to create a "summary" of this part of the signal.

A filtering operation performs this using a sliding window on the signal as shown in the following figure:

![missing image](./Images/FIR_Filter.jpg)

Input data samples are in general called **x** (blue squares), the coefficients **c** (green squares) and the output samples **y**:

![missing image](./Images/FIR_Equation.jpg)

DSP experts may say that this equation represents a _correlation_ and not a _convolution,_ which is the mathematical expression of the filtering operation. The easy answer may be to say that it is simply a question of coefficients ordering (and perhaps conjugation for complex coefficients).

That is why you always see the two lines at the beginning of the various _graph.h_ files:

```C++
std::vector<cint16> taps = std::vector<cint16>({
  ...
});

std::vector<cint16> taps_aie(taps.rbegin(),taps.rend());
```

The first line is the taps vector definition in the correct order for a DSP expert, and the second line defines the vector that is used in the AI Engine implementation as the same vector but in the reverse order.


<a name="UtilsDirectory"></a>

## "Utils" Directory

For this tutorial, a number of utilities have been created that you can reuse for your own purposes.

First, to allow these utilities to be called from anywhere during this tutorial, add this directory in your **_PATH_** but also indicate to `python` that this directory contains some libraries and should be checked during imports.

Navigate to the `Utils` directory, and type `source InitPythonPath` to have this directory in your path for Python libraries and executable search path.

### GenerateStreams

This utility will use a library **_GenerationLib.py_** to generate input data suitable for the cases you want to test. It is called by typing `GenerateStreamsGUI`. This displays a GUI in which you can select the appropriate parameters to generate the correct input data files:

![missing image](./Images/GenerateStreams.jpg)

You have access to a number of parameters:

- _Data Type_: By default, `cint16`, as this is what you use throughout this tutorial.
- _PLIO Width_: By default, `64`, as this is the width which is used in this tutorial.
- _Number of Phases_: For Super Sampling Rate Filters.
- _Number of Streams_: For the SSR filters using the 2 streams of the AI Engines.
- _Number of Samples per Stream per Phase_: Each stream contains a number of samples defined there.
- _Number of Frames_: Simulations are launched for a limited number of Frames.
- _Base of the Filename_: `PhaseIn` by default, which generates the following names:
  - Single Stream, Single Phase: `PhaseIn_0.txt`
  - Single Stream, Polyphase: `PhaseIn_0.txt`, `PhaseIn_1.txt`, ...
  - Dual streams, Polyphase:  `PhaseIn_0_0.txt`, `PhaseIn_0_0.txt`, `PhaseIn_1_0.txt, `PhaseIn_1_0.txt``, ...

Another possibility is to type `GenerateStreams` with the same parameters. If you type `GenerateStreams` without parameters, a usage text is displayed:

```BASH
>>GenerateStreams

Stream Content Generation

================================================================================================

Usage:
GenerateStreams DataType PLIO_Width NPhases NStreams NSamples NFrames SequenceType Basename
   Datatype: cint16, int16, int32
   PLIO_Width: 32, 64 or 128
   NPhases: any integer >= 1
   NStreams: 1 or 2
   NSamples: integer, multiple of 8
   NFrames: Any integer >= 1
   SequenceType: Dirac, Linear, SinCos, Random
   Basename: file name that will prepend phase and stream index

================================================================================================
```



## ProcessAIEOutput

This utility takes all generated outputs and displays the reconstructed signal. For Single Stream/Single Phase, it displays a signal using the timestamps written in the file.

If your output signals are stored in files named `output_0.txt`, navigate to the output directory and type `ProcessAIEOutput output_*` to process the output of the AI Engines.

Two other files are generated:

- `Atot.txt`, which is the output phase by phase.
- `out.txt`, which is the textfile of the reconstructed signal.


### StreamThroughput

This utility computes the throughput concerning all AI Engine output files given in an argument.

### GetDeclare.sh

This utility has been created to view the template arguments that were used for kernel declaration in the Double Stream SSR case. It can be easily modified to be adapted to different cases.




## License

___

The MIT License (MIT)

Copyright (c) 2023 Advanced Micro Devices, Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020–2023 Advanced Micro Devices, Inc </sup><br><sup>XD020</sup></br></p>
