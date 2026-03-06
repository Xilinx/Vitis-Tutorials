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

# Migrating Fractional Delay Farrow Filter from AIE-ML to AIE-ML v2 Architecture

***Version: Vitis 2025.2***

## Introduction

The *Fractional Delay Farrow Filter* design is already implemented for both AIE and AIE-ML architectures. This tutorial shows you how to migrate the GMIO-based design from the AIE-ML architecture to the AIE-ML v2 architecture.

A fractional delay filter is a common digital signal processing (DSP) algorithm used in many applications such as digital receivers in modems and required for timing synchronization.

Before porting to the AIE-ML v2 architecture, understand the Farrow Filter and its implementation details with the AIE architecture.

Study **[Fractional Delay Farrow Filter Targeting AIE Architecture](../../../AIE/Design_Tutorials/15-farrow_filter/README.md)** to learn the following:

1. What is a Farrow Filter?
2. Requirements and AIE System Partitioning
3. AI Engine Implementation and Optimization

You can also learn how to migrate from AIE to AIE-ML architecture in this tutorial **[Migrating Fractional Delay Farrow Filter from AIE to AIE-ML Architecture](../../../AIE-ML/Design_Tutorials/06-farrow_filter/README.md)**. We are porting the GMIO-based farrow filter design from the AIE-ML tutorial to the AIE-ML v2 architecture.

The GMIO-based farrow filter design from AIE-ML makes use of the GMIO ports. A GMIO port attribute is used to make external memory-mapped connections to or from the global memory. These connections are made between AI Engine kernels and the logical global memory ports of a hardware platform design. To learn more about how to implement the GMIO interface, review the kernel written in the graph and test bench (`farrow_graph.h` and `farrow_app_adf.cpp`) located under the `<path-to-tutorial>/designs/aie_ml/` directory.

Now that you have reviewed the background on the farrow filter design in AIE and AIE-ML, you are ready to port the design to the AIE-ML v2 architecture. The design requirements are identical here as you are simply migrating the design to AIE-ML v2 architecture:

|Requirements| |
|---|---|
| Sampling rate | 1 GSPS |
| I/O data type | `cint16` |
| Coefficients data type | `int16` |
| Delay input data type | `int16` |

**IMPORTANT**: Before beginning the tutorial, make sure that you have read and followed the *Vitis Software Platform Release Notes* (v2025.2) for setting up the software and installing the VEK385 base platform.

Before starting this tutorial, run the following steps:

1. Set up your `YOCTO_ARTIFACTS` environment variable to the AMD EDF from our embedded platforms download page [EDF Packages](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-design-tools.html).
2. Set up your `PLATFORM_REPO_PATHS` environment variable based upon where you downloaded the platform.

The following is a list of changes that are required to port over the design from AIE-ML on VEK280 to AIE-ML v2 on VEK385.

* Change `platform` in [Top Makefile](Makefile) from `xilinx-vek280_base` to `vek385_base`.
* For Versal AI Edge Series Gen 2, AMD tools by default use [Segmented Configuration](https://docs.amd.com/r/en-US/ug1273-versal-acap-design/Segmented-Configuration) and [AMD Embedded Development Framework (EDF)](https://docs.amd.com/r/en-US/ug1304-versal-acap-ssdg/Embedded-Development-Framework-for-Versal-Prime-Series-Gen-2-and-Versal-AI-Edge-Series-Gen-2-Devices).
* Segmented Configuration is a solution that enables users to boot the processors in a Versal device and access DDR memory before the programmable logic (PL) is configured. This is enabled by a primary boot firmware Image, an example of a pre-built disk image `VEK385 EDF boot firmware Image (OSPI) Image` can be found in [Versal AI Edge Series Gen 2 VEK385 HeadStart Board Early Access Secure Site](https://account.amd.com/en/member/vek385-board-ea.html#tabs-bdeb221ec4-item-2b4db1da1e-tab). The secondary image includes Linux built using Yocto, also found in the same lounge `EDF Linux® BSP Common SD-Card Image for Cortex A78 devices`. To generate the device tree overlay `pl.dtbo`, we use sdtgen and lopper. After completing these steps, the necessary files to boot and run the design on hardware transfers over scp. `fpgautil` is used to load the `pl-aie.pdi` as well as the device tree. The design can then be exercised by executing the host program and pointing to the `dut.xclbin`.

## Table of Contents

- [Migrating the Design from AIE-ML to AIE-ML v2 architecture](#migrating-the-design-from-aie-ml-to-aie-ml-v2-Architecture)
- [Analyze the Reports](#analyze-the-reports)
- [Building and Running the Design on the Board](#building-and-running-the-design-on-the-board)

### Objectives

* Migrate the farrow filter from AIE-ML to AIE-ML v2 architecture
* Analyze the design to illustrate enhancements in AIE-ML v2
* Implement the design using the Vitis tool
* Run the design on the board

### Migrating the Design from AIE-ML to AIE-ML v2 Architecture

#### Change the Project Path

Switch the device from AIE-ML to AIE-ML v2, then compile the design to ensure it compiles without errors.  \
Enter the following command to navigate to the project path of the final AIE-ML design:

```
cd <path-to-tutorial>/designs/farrow_final_aie-ml
```

Make sure to set the `PLATFORM_REPO_PATHS` environment variable.

#### Source the Vitis Tool

Enter the following command to source the Vitis tool:

```
source /<TOOL_INSTALL_PATH>/Vitis/2025.2/settings.sh
```

#### Update the Makefile to switch the device from AIE-ML to AIE-ML v2

Open the Makefile and modify the platform as shown below:

```
PLATFORM_USE  := vek385_base_reva
```

Save the file.

Note: ensure you set the platform corresponding to the appropriate board. You may be using a different revision than the tutorial.

#### Compile and Simulate the Design

Enter the following command to navigate to the aie-ml directory to compile and simulate the design:

```
cd aie-ml
```

Enter the following command to compile (x86compile) and simulate (x86sim) to verify the functional correctness of the design:

```
$ make x86compile
$ make x86sim
$ make check_sim_output
```

Following the last command, the console should output as below:

```
Max error LSB = 1
```

In this GMIO-based design, the data is written to memory. Thus, first read the output from global memory and then write it to a file.
You can perform AI Engine emulation using the SystemC simulator by entering the following sequence of commands:

```
$ make compile
$ make sim
$ make check_sim_output
```

Following the last command, the console should output as below:

```
Max error LSB = 1
```

Since the AIE-ML v2 architecture supports the API functions and data-types used in the farrow filter design from the AIE-ML tutorial, no code changes were necessary to compile and simulate the ported design.

The following figure shows the supported parameters type (coeff x data) for AIE and AIE-ML architecture. **coeff** is *int16* and **data** is *cint16*.

![AIE API Parameters](./images/AIE_API_Parameters.png)

Let's review the simulation results to determine if we need to make changes to the code to satisfy the design requirements listed in the Introduction.

### Analyze the Reports

Enter the following command to launch Vitis Analyzer and review the reports:

```
$ vitis_analyzer aiesimulator_output/default.aierun_summary
```

![Farrow_AIE-MLv2_Graph](./images/Farrow_AIE-MLv2_Graph.png)
Graph view shows the five kernels (four for filters and one for final computation). Select the I/O tabs as shown in the above diagram. Observe the Throughput column in the I/O tab.

The output GMIO port throughput shows the value 7831.7397. This throughput value needs to be divided by four because the data type used is `cint16`, which is four bytes in size. You will get the throughput value as 1957.53 MSPS.

A more accurate throughput measurement can be made by measuring the steady state achieved in the final graph iteration. In vitis_analyzer, select the trace view and set markers to measure the throughput of this final iteration as shown below.

Because each graph iteration processes 1024 samples, throughput is 1024/0.5582 = **1834.47 MSPS**.

Note: In the graph, select the output port which will show the net name, in this case it is net9.
![Farrow_AIE-MLv2_Trace](./images/Farrow_AIE-MLv2_Trace.png)

The design was able to meet the desired 1 GSPS by nearly 2x. This is attributed to the increase in compute capacity as there are more multipliers in the AIE-ML v2 architecture compared to previous architectures. No additional code changes are necessary to meet the design requirements.

After reviewing the report, close Vitis Analyzer.

Run the script which will read the II from the compiler log for each tiles.

```
$ make get_II
```

The console should output as below:

```
*** [LOOP_II] *** Tile 19_1 minII = 3 achieves II = 3
*** [LOOP_II] *** Tile 19_1 minII = 3 achieves II = 3
*** [LOOP_II] *** Tile 19_1 minII = 3 achieves II = 3
*** [LOOP_II] *** Tile 19_3 minII = 16 achieves II = 16
*** [LOOP_II] *** Tile 20_0 minII = 16 achieves II = 16
*** [LOOP_II] *** Tile 20_1 minII = 16 achieves II = 16
*** [LOOP_II] *** Tile 20_2 minII = 16 achieves II = 16
```

The implementation of `farrow_kernel1.cpp` spans across tiles 19_1, 19_3, 20_0, 20_1, and 20_2. Based on the results above, these tiles achieved an II of 16 for each of their respective for loops. Without any code optimizations, we see that upgrading the AIE architecute results in nearly 1/2 the II from AIE-ML.

### Building and Running the Design on the Board

#### Review of Tool Flow

The diagram below illustrates the EDF flows. The flow we use in this tutorial is illustrated on the right called "Hardware Development in Vitis tool flow," encompassing the development stages for AI kernels, PL kernels, and PS code. Once the development of AIE kernels and PL kernels is completed, the subsequent step involves linking `libadf.a` and all `.xo` kernels with the designated platform, the output of the linker, which includes `.xsa` and `host.exe`, is packaged together to generate `.xclbin`, `.dtbo`, and `.pdi` required for programming the board.
  
![Tool_Flow](./images/Tool_Flow_EDF.png)

#### Setup and Initialization

IMPORTANT: Before beginning the tutorial, ensure you have:

* Installed AMD Vitis™ 2025.2 software and set `PLATFORM_REPO_PATHS` to the value `<Vitis_tools>/base_platforms`.
* Created directory `<path-to-design>/yocto_artifacts` and set environment variable YOCTO_ARTIFACTS to that path.
* From [Embedded Development Framework (EDF) downloads page](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-design-tools.html) package 25.11:
  * Downloaded EDF Application & Machine SDK, run the script and set path output to `<path-to-design>/yocto_artifacts/amd-cortexa78-mali-common_meta-edf-app-sdk/sdk`.
  * Downloaded SD/WIC Linux Image VEK385 and move them to `<path-to-design>/yocto_artifacts/amd-cortexa78-mali-common_edf-linux-disk-image`.
  * Downloaded EDF QEMU File Set for Versal™ AI Edge Gen 2 VEK385 evaluation board, unzip and move `amd-cortexa78-mali-common_vek385_qemu_prebuilt` into `<path-to-design>/yocto_artifacts/`.

##### Host Code with XRT APIs

It is recommended to use the XRT APIs for the host code. Review the code to view the XRT APIs and then build the project and run it on board.

Enter the following command to change project path:

```
$ cd ../ps_apps/hw_emu
```

Review the `host.cpp` file.

The XRT profiling API is also used to measure the throughput of the design.

###### Hardware Emulation

Enter the following command to build the design for hardware emulation:

```
$ cd <path-to-tutorial>/designs/farrow_final_aie-ml/
$ make clean all TARGET=hw_emu
```

This will take about 20 minutes to run.

```
...
PASSED:  auto xclbin_uuid = my_device.load_xclbin(dut.xclbin)
PASSED:  auto my_graph  = xrt::graph(my_device, xclbin_uuid, "aie_dut")
PASSED:  my_graph.reset()
GMIO::malloc completed
PASSED:  xrt::aie::profiling handle(my_device);

INFO:    Started profiling timers...

PASSED:  my_graph.run( ITERATION=4 )
Throughput of the graph: 7831.74 MB/s
Throughput of the graph: 1957.93 MSPS
--- PASSED ---
GMIO transactions finished
INFO: Embedded host run completed.
...
```

Note: You can safely ignore the warnings.

To exit the QEMU, press Ctrl A + X.
After the hardware emulation run is complete, you can analyze the reports in Vitis Analyzer.

###### Hardware Run

Enter the following command to build the design for hardware validation:

```
$ cd <path-to-tutorial>/designs/farrow_final_aie-ml/
$ make clean all TARGET=hw
```

The build process generates all the design specific files needed to run the design on hardware in the ```package``` folder.

1. Write the EDF boot firmware (OSPI) to the primary boot device following instructions [here](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/3258155011/AMD+EDF+Getting+started+-+Discovery+and+Evaluation+AMD+Versal+device+portfolio#Writing-the-EDF-boot-firmware-to-the-primary-boot-device-%2F-media-using-System-Controller-(SC)). OSPI image can be found in `<path-to-design>/yocto_artifacts/amd-cortexa78-mali-common_vek385_qemu_prebuilt/qemu-ospi-versal-2ve-2vm-vek385-sdt-seg.bin`.
2. Write `<path-to-design>/yocto_artifacts/amd-cortexa78-mali-common_edf-linux-disk-image/edf-linux-disk-image-amd-cortexa78-mali-common.rootfs.wic` to sd_card using your favorite SD imaging tool (Balena Etcher and Win32DiskImager seems to work well).
3. Put the sd_card in to the board, boot it and log in. (default username is amd-edf and you will be promted to set a password)
4. On your terminal application, determine the IPv6 address eth0 on the board by typing `ip addr show eth0`.
5. cd `<path-to-design>/package; scp -6 * amd-edf@<ipv6_address>:~/`
6. Run the design: `sudo ./embedded_exec.sh`

Note: You may need to change the permissions of the files in the home directory with **"chmod +x *"**

The following is displayed on the terminal:

```
...
PASSED:  auto xclbin_uuid = my_device.load_xclbin(dut.xclbin)
PASSED:  auto my_graph  = xrt::graph(my_device, xclbin_uuid, "aie_dut")
PASSED:  my_graph.reset()
GMIO::malloc completed
PASSED:  xrt::aie::profiling handle(my_device);

INFO:    Started profiling timers...

PASSED:  my_graph.run( ITERATION=4 )
Throughput of the graph: 8572.62 MB/s
Throughput of the graph: 2143.16 MSPS
--- PASSED ---
GMIO transactions finished
INFO: Embedded host run completed.
...
```

Note: You can safely ignore the warnings.

## Comparison of AIE & AIE-ML vs AIE-ML v2 Farrow Filter Design Implementation

The following table compares the implementation of a farrow filter in AIE, AIE-ML, AIE-ML v2 architectures.
This indicates that approximately twice the number of tiles is required for kernel computation in AIE-ML v2 compared to the AIE and just 2 tiles more than AIE-ML architecture to achieve nearly 2x the performance.

| Design                  | Tiles for AIE Kernels | Tiles for Buffers | Total Tiles |  Throughput         | Relative MSPS per tile |
|-------------------------|-----------------------|-------------------|-------------|---------------------|------------------------|
| farrow - AIE (PLIO)     |       2               | 5                 | 5           | 1138 MSPS (HW_EMU)  | 227.6                  |
| farrow - AIE-ML (GMIO)  |       5               | 8                 | 8           | 1061 MSPS (HW_EMU)  | 132.6                  |
| farrow - AIE-MLv2 (GMIO)|       5               | 10                | 10          | 1958 MSPS (HW_EMU)  | 195.8                  |

```* Total Tiles: Represents the total count of tiles, including those that have both kernels and buffers within the same tile.```

## Conclusion

This tutorial has demonstrated the following:

* How to migrate the design from AIE-ML Engine to AIE-ML v2 architecture.
* How to analyze the design implementation and throughput with Vitis Analyzer and Hardware Emulation
* Running the design on the board.

<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>