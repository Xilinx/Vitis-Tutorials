# UG1352: Get Moving with Alveo

**NOTE:** Please note that the Makefile under the `Introduction` directory take care of all the steps mentioned in this file including the `git clone`. 

This repository contains the source files for the exercises in *UG1352: Get Moving
With Alveo* and for the "Runtime and System Optimization" Vitis introductory tutorial.

This repository makes use of Vitis Vision hardware accelerated library. Clone the Vitis_Libraries under hw_src folder.

```bash
git clone https://github.com/Xilinx/Vitis_Libraries.git
```

This repository includes both hardware and software sources.

This repository has been validated against the Vitis Unified Development Environment
release 2023.2 and has been hardware-validated against the following platforms:

| Alveo Card |             Platform                          |   XRT    |
| :--------- | :-------------------------------------------: | :-----:  |
| U200       |    xilinx_u200_gen3x16_xdma_2_202110_1        | 2.14.354 |
| U250       |    xilinx_u250_gen3x16_xdma_4_1_202210_1      | 2.14.354 |
| U50        |    xilinx_u50_gen3x16_xdma_5_202210_1         | 2.14.354 |

## Building the Hardware Design

All hardware sources for this design are under the `hw_src` directory and can
be built by running `make`. Before doing so, edit config.mk or override these
values on the command line to point to your particular system:

```Makefile
# Options for TARGET: sw_emu, hw_emu and hw
TARGET ?= hw
# Options for DEVICE: u200, u250. Default platform is XDMA, defined by PLATFORM
DEVICE ?= u250
# If other some specific platforms needs to be used, provide platform path directly
PLATFORM ?= xilinx_$(DEVICE)_gen3x16_xdma_2_202110_1
# If your platform is not in the standard install area edit this line
PLATFORM_REPO_PATH ?= /opt/xilinx/platforms/
```

Then build the platform:

```bash
cd hw_src
make
```

Alternately, if you want to specify a particular platform on the command line, run:

```bash
cd hw_src
PLATFORM=xilinx_u250_gen3x16_xdma_4_1_202210_1 make
```

Substitute your platform name as needed.

## Building the Software Design

All software for these references is built using CMake. For some examples we rely
on external libraries, in particular OpenCV and OpenMP. If these libraries are not
present on your system the dependent examples will not be built.

From the root directory of this archive, to build the software:

```bash
mkdir build
cd build

cmake ..

make -j
```

**NOTE:** This should be done *after* building hardware so that the .xclbin file exists

To run any of the resulting examples, execute them directly as per *UG1352: Get Moving
With Alveo* or the accompanying tutorial.  Note that some examples, specifically #7 and #8,
require additional command line arguments. Running these examples with no command line
arguments will print a help message with further instructions.

```bash
./00_load_kernels
```
