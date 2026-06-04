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

# Digital Down-conversion Chain: Converting from Intrinsics to API

***Version: Vitis 2026.1***

## Introduction

AMD Versal™ adaptive SoCs combine programmable logic (PL), processing system (PS), and AI Engines with leading-edge memory and interfacing technologies to deliver powerful heterogeneous acceleration for any application. Data scientists and software and hardware developers can program and optimize the hardware and software. A host of tools, software, libraries, IP, middleware, and frameworks enable Versal adaptive SoCs to support all industry-standard design flows.

This tutorial demonstrates the steps to upgrade a 32-branch digital down-conversion chain so that it is compliant with the latest tools and coding practice. The tutorial includes examples for the following changes with side-by-side view of the original and upgraded code.

- Converting coding style from kernel functions to kernel C++ classes
- Relocating global variables to kernel class data members
- Handling state variables to enable x86sim
- Migrating Windows (deprecated) to buffers for non-stream based kernel I/O
- Replacing kernel intrinsics with equivalent AI Engine APIs
- Updating older pragmas
- Supporting x86 compilation and simulation

You can find the design description in the [Digital Down-conversion Chain Implementation on AI Engine (XAPP1351)](https://docs.amd.com/r/en-US/xapp1351-ddc-ai-engine). The codebase associated with the original design can be found in the [Reference Design Files](https://www.xilinx.com/cgi-bin/docs/ctdoc?cid=d3643c02-32f7-407c-b680-59c0b234e703;d=xapp1352-beamforming-ai-engine.zip).

## Upgrading Tools, Device Speed Grade, and Makefile

>**Note:** Simply loading the latest version of the tools and compiling the design is not possible because the baseline Makefile has deprecated compiler options.

![figure1](images/Makefile_differences.png)

You must make the following important changes to the Makefile:

- Upgrade part speed grade xcvc1902-vsva2197-1LP-e-S-es1 (previously specified by ``--device``) to xcvc1902-vsva2197-2MP-e-S (specified by ``--platform``). The following table shows this change (referenced from Versal AI Core Series Data Sheet: DC and AC Switching Characteristics [(DS957)](https://docs.amd.com/go/en-US/ds957-versal-ai-core)), which increases the AI Engine clock frequency from 1 GHz to 1.25 GHz.

  ![figure2](images/ds957_table74.png)

  Recompiling and simulating the design with this change causes the throughput to increase by around 17-25%.

- Upgrade to use v++ unified compiler command.

- Add support for x86 compilation and simulation.

## Upgrading the Code

### Converting Kernel Functions to Kernel Classes

The new kernel C++ class constructor incorporates functionality from the `init()` function. The new class `run()` member function incorporates the main kernel function wrapper.

![figure3](images/functions_to_classes_1.png)

Create a header file for the class. You must write the `static void registerKernelClass()` method in the header file. Inside the `registerKernelClass()` method, call the `REGISTER_FUNCTION` macro. This macro registers the class run method to execute on the AI Engine core to perform the kernel functionality.

![figure4](images/functions_to_classes_2.png)

When creating the kernel in the upper graph or subgraph, use `kernel::create_object` instead of `kernel::create`. Remove ``initialization_function`` as it is now part of class constructor.

![figure5](images/functions_to_classes_3.png)

### Migrating from Windows to Buffers

The 2023.2 release of the AMD Vitis™ software platform deprecated Windows I/O connections between kernels. The AI Engine Kernel and Graph Programming Guide [(UG1079)](https://docs.amd.com/r/en-US/ug1079-ai-engine-kernel-coding/Comparison-between-Buffer-Ports-and-Windows) describes how to change the source code of a design to upgrade it to buffer I/Os. To upgrade I/O connections from Windows to buffers, repeat the following steps for every kernel.

1. Make the changes shown in the following figure in the ``kernel.cc`` file:

   ![figure6](images/windows_to_buffers_1.png)

2. If the design uses classes, upgrade the associated header file.

   ![figure7](images/windows_to_buffers_2.png)

3. In the graph file, modify the connection type and specify dimension. Note division by 4 to convert from bytes to samples.

   ![figure8](images/windows_to_buffers_3.png)

### Replacing Intrinsics with APIs

The following example shows a side-by-side comparison of intrinsic-based code compared to API-based code. Both are functionally equivalent and produce the same final hardware usage and throughput.

![figure9](images/intrinsics_to_API.png)

### Relocating Global Variables to Kernel Class Data Members

![figure10](images/handling_global_variables.png)

### Handling State Variables to Enable x86sim

Move the state variables instantiation from ``kernel::run`` to class member or use ``thread_local``, as shown in the following figure. For more information, refer to the Memory Model section of the AI Engine Tools and Flows User Guide [(UG1076)](https://docs.amd.com/r/en-US/ug1076-ai-engine-environment/Memory-Model).

![figure11](images/handling_state_variables_x86sim.png)

### Updating Older Pragmas

Update `chess_alignof` to `alignas`. The previous figure highlights this change.

### Supporting x86 Compilation and Simulation

The following actions enable x86 compilation and functionally correct simulation:

- Modifying the Makefile to include target=x86sim capability.

  ![figure12](images/support_x86sim.png)

- Relocating global variables to kernel class data members, as highlighted in a [previous step](#relocating-global-variables-to-kernel-class-data-members).

- Moving state variables instantiation from ``kernel::run`` to class member or use ``thread_local``, as highlighted in a [previous step](#handling-state-variables-to-enable-x86sim).

## Building and Running the Design

You can build the 32-branch digital down-conversion design using the command line.

### Setup and Initialization

IMPORTANT: Before beginning the tutorial, install the AMD Vitis™ 2026.1 software platform. Also, download the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html). 

Set the environment variable ```COMMON_IMAGE_VERSAL``` to the full path where you have downloaded the Common Images. Then set the environment variable ```PLATFORM_REPO_PATHS``` to the value ```$XILINX_VITIS/base_platforms```. The remaining environment variables are configured in the top level Makefile.

### x86 Functional Simulation

You can build and simulate the DDC design targeting x86sim to functionally verify the C code as follows:

```shell
[shell]% cd <path-to-09-ddc_chain-dir>
[shell]% make x86all
```

The number of simulation samples mismatch compared to expected outputs is displayed.

### Hardware Simulation

You can build and simulate the DDC design by targeting hardware using the Makefile as follows:

```shell
[shell]% cd <path-to-09-ddc_chain-dir>
[shell]% make all
```

The simulation displays the number of samples mismatch compared to expected outputs. The simulation also displays achieved throughput for all branches against minimum requirement.

## Summary

In this tutorial, we highlight steps that an AI Engine designer can take to upgrade their design to use APIs instead of intrinsics. The upgraded AIE API version achieves the same throughput performance as the original code base, while being easier to read and maintain.

The following table summarizes key parameters for the older design (ran on newer version of the tools) and compares it to upgraded design.

|Parameters|Original|Upgraded|
|---|---|---|
|Support x86sim | No | Yes |
| Intrinsics vs API | Intrinsics | Mostly APIs |
| Windows vs Buffers | Windows | Buffers |
| Functionally correct | Yes | Yes |
| Throughput (MSPS)| ~247/224 | ~247/224 |

## Support 

GitHub issues are used to track requests and bugs. For questions, go to [support.amd.com](https://adaptivesupport.amd.com/s/?language=en_US).

## License

Components: xilinx-images

images in the documentation

Components: xilinx-files

The MIT License (MIT)

Copyright (c) 2023 Advanced Micro Devices, Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

<p class="sphinxhide" align="center"><sub>Copyright © 2023 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>

<p class="sphinxhide" align="center"><sub>Copyright © 2021 Xilinx Inc.</sub></p>

Components: generated-xilinx-files

Redistribution and use in binary form only, without modification,
is permitted provided that the following conditions are met:

1. Redistributions must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

1. The name of Xilinx, Inc. may not be used to endorse or promote products
redistributed with this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY XILINX, INC. "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL XILINX, INC. BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
