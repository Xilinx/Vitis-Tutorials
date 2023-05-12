<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>2022.2 Vitis™ - Runtime and System Optimization - Guided Software Examples</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

## Provided Design Files

In the installation package for this tutorial series, you will find a `design_source` directory. This directory contains all of the source files necessary to build and run the examples (with the exception of the build tools such as the Vitis, Xilinx Runtime (XRT), and the target development platform, which you must install yourself).

Under the design_source directory, there are two primary directories: `hw_src` and `sw_src`. As the names imply, these contain the hardware and software source files for our application. The hardware sources synthesize to algorithms that run on the FPGA through the Vitis **V++** compiler, and the software sources are compiled using standard GCC to run on the host processor.

We are focusing on software more than hardware in this tutorial, so we have split the source files for easy organization. In a real project any directory structures, etc., are arbitrary; you can follow the best practices of your team or organization.

Because some of the examples rely on external libraries, we are using the **CMake** build system for the software examples as it simplifies environment configuration. But on the hardware side, we are using standard **make**. This is so you can easily see the command line arguments passed to V++.

## Hardware Design Setup

This guided introduction will introduce acceleration concepts targeting the AMD Alveo™ cards. While not explicitly covered in this release, the concepts, techniques, and relative performance apply equally well to embedded processors also. We will start with writing host code: programming the FPGA, allocating memory, and moving memory around. Our accelerators will be very simple for these early examples. In fact, we will likely see that the algorithms run faster on the CPU, at least at first, since our acceleration hardware is so trivial.

Building hardware designs can also be quite time-consuming — it turns out that synthesizing, placing, and routing custom logic across billions of transistors with sub-nanosecond timing is just a teeny bit more complex than compiling to machine code. Bear with us, because the results are worth it.  To avoid having to needlessly rebuild the FPGA hardware, we provide a single FPGA design with many kernel instances, which we will mix-and-match as needed for the example designs. This guide will touch a bit on kernel optimization, but beyond basic concepts, we will mostly leave that for other tutorials in this repository.

This onboarding example comes with a hardware image targeting the Alveo U200 accelerator card with shell version 201830_2. You will need to build the hardware design before you can run. To do that, change into the directory:

```
Hardware_Acceleration/Introduction/design_src/hw_src
```

If you have a different version of the target platform installed on your board, you can skip this step. But if you are targeting a platform other than the Alveo U200, open the Makefile, and change the first line to point to your platform's .xpfm file.

**Note:** Line breaks might have been added to the example below for formatting, depending on your resolution. Do not add line breaks to the path in your Makefile.

```make
PLATFORM := /opt/xilinx/platforms/xilinx_u200_gen3x16_xdma_1_202110_1/xilinx_u200_gen3x16_xdma_1_202110_1.xpfm
```

After you edit the Makefile, ensure that your Vitis and XRT environments are set up properly. If you have not done so already, run the commands:

```bash
source /opt/Xilinx/Vitis/2022.2/settings.sh
source /opt/xilinx/xrt/setup.sh
```

If your installation path for either XRT or Vitis are different than the default locations, update the command line accordingly. Then, run the command:

`make`

The build process will take some time, but in the end you will have a file in this directory called `alveo_examples.xclbin`. This file contains all of the kernels we will use during this exercise. After you
have this file compiled for your combination of board and shell, you are ready to proceed to the next section.

Read [**Example 0:** Loading an Alveo Image](./00-loading-an-alveo-image.md)

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
