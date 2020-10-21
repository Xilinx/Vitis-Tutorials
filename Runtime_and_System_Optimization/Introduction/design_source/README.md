# UG1352: Get Moving with Alveo

This repository contains the source files for the exercises in *UG1352: Get Moving
With Alveo* and for the "Runtime and System Optimization" Vitis introductory tutorial.

This repository makes use of Git submodules to pull in other repositories such as
the Vitis Vision hardware accelerated library. To properly clone this repository
be sure to include the ```--recurse-submodules``` command line switch.

```bash
git clone --recurse-submodules https://github.com/Xilinx/Vitis-In-Depth-Tutorial.git
```

This repository includes both hardware and software sources.

This repository has been validated against the Vitis Unified Development Environment
release 2020.1 and has been hardware-validated against the following platforms:

| Alveo Card |             Platform             |   XRT   |
| :--------- | :------------------------------: | :-----: |
| U200       |    xilinx_u200_xdma_201830_2     | 2.6.655 |
| U250       |    xilinx_u250_xdma_201830_2     | 2.6.655 |
| U50        | xilinx_u50_gen3x16_xdma_201920_3 | 2.6.655 |

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
PLATFORM ?= xilinx_$(DEVICE)_xdma_201830_2
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
PLATFORM=xilinx_u50_gen3x16_xdma_201920_3 make
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
