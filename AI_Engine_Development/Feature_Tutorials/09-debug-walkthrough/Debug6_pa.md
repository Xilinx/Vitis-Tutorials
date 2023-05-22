<table class="sphinxhide" width="100%">
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Debug Walkthrough Tutorial - From Simulation to Hardware</h1>
   </td>
 </tr>
 <tr>
 <td align="center"><h2>Profiling Graph Inputs and Outputs with Profiling APIs</h1>
 </td>
 </tr>
</table>

## Overview

In this tutorial, you will learn how to:

* Add profiling APIs in the graph and PS code to calculate design performance.
* Generate a VCD file to view the design information.
* Cross check between the calculated performance with the profiling APIs and vitis_analyzer trace view.
* Use the profiling features to the inspect design.

## Profiling APIs

Start the profiling API:

```bash
/// Start profiling and acquire resources needed for profiling. Should be called after graph::init().
/// @param io Plarform PLIO or GMIO object.
/// @param option io_profiling_option enum.
/// @param value Optional value for the specified option.
/// @return Return event::handle to be used by read_profiling and stop_profiling. Return event::invalid_handle for error conditions or unsupported use cases.
static handle start_profiling(IoAttr& io, io_profiling_option option, uint32 value = 0);
```

Read the profiling API:

```bash
/// Read profiling.
/// @param h event::handle returned from start_profiling.
/// @return Profiling value.
static long long read_profiling(handle h);
```

Stop the profiling API:

```bash
/// Stop profiling and release resources needed for profiling.
/// @param h event::handle returned from start_profiling.
static void stop_profiling(handle h);
```

[1. Profiling APIs with the AI Engine Simulator](#1-profiling-apis-with-the-ai-engine-simulator)

[2. Profiling APIs with the HW Emulator](#2-profiling-apis-with-the-hw-emulator)

[3. Profiling APIs on the Hardware](#3-profiling-apis-on-the-hardware)

[4. Cross Check the I/O Performance Values with VCD](#4-cross-check-the-io-performance-values-with-vcd)

## 1: Profiling APIs with the AI Engine Simulator

### Step 1.1: Download the Project

Clone the project source from the git repository, and unzip the zip file.

### Step 1.2: Prepare the Profiling APIs Applied Graph Code

Use this tutorial's `graph.cpp.profile_api` that contains AI Engine profiling APIs.

```bash
cd ${DOWNLOAD_PATH}/AI_Engine_Development/Feature_Tutorials/09-debug-walkthrough/aie
cp graph.cpp.profile_api graph.cpp
cd ..
cp Makefile.emu Makefile
make libadf.a
```

### Step 1.3: Run the AI Engine Simulator

```bash
aiesimulator --pkg-dir=./Work --i=. --profile
```

### Step 1.4: Expected Result

![alt text](images/pr_aie_perf.png)

```bash
vitis_analyzer ./aiesimulator_output/default.aierun_summary
```

## 2. Profiling APIs with the HW Emulator

### Step 2.1: Prepare the Profiling APIs Applied Host Code and Makefile

After Step 1.1 completed, use this tutorial's `host.cpp.profile_api` that contains the AI Engine profiling APIs and Makefile for HW Emulation.

```bash
cd ${DOWNLOAD_PATH}/AI_Engine_Development/Feature_Tutorials/09-debug-walkthrough/sw
cp host.cpp.profile_api host.cpp
cd ..
cp Makefile.emu Makefile
```

### Step 2.2: Build the Design

```bash
make
```

### Step 2.3: Run the Hardware Emulator

```bash
./launch_hw_emu.sh
```

### Step 2.4: Run the Application

After Petalinux boots up.

```bash
cd /run/media/mmcblk0p1
./host.exe a.xclbin
```

![alt text](images/pr_hw_emu_perf.png)

## 3: Profiling APIs on the Hardware

### Step 3.1: Prepare the Hardware Makefile

After Step 1.1 completed, use this tutorial's Makefile for Hardware.

```bash
cd ${DOWNLOAD_PATH}/AI_Engine_Development/Feature_Tutorials/09-debug-walkthrough
cp Makefile.profile_hw Makefile
make
```

### Step 3.2: Flash the SD ard

Flash the generated `sd_card.img` to the SD card.

### Step 3.3: Boots Up the VCK190 Board

Plug in the flashed completed SD card to the VCK190 board's SD slot. Power up the VCK190 board.

### Step 3.4: Run the Application

After the VCK190 board boots up and is ready to accept commands with Linux prompt, issue these commands from the terminal.

```bash
cd /run/media/mmcblk0p1
./host.exe a.xclbin
```

### Step 3.5: Expected Result

![alt text](images/pr_hw_perf.png)

>**NOTE:** Because of slower memory access, hardware and hardware emulation performance values are not optimized and less than AI Engine simulation.

## 4: Cross Check the I/O Performance Values with VCD

### Step 4.1: VCD Generation

Use the following command to generate the AI Engine simulation VCD file.

```bash
aiesimulator --pkg-dir=./Work --i=. --dump-vcd=foo
```

Use the following command to generate the hardware emulation VCD file.

```bash
./launch_hw_emu.sh -add-env AIE_COMPILER_WORKDIR=${PROJECT_FULL_PATH}/Work -aie-sim-options ${PROJECT_FULL_PATH}/aiesim_options.txt
```

Where `aiesim_options.txt` content is:

```bash
AIE_DUMP_VCD=foo
```

Follow Step 2.4 to run the application.

### Step 4.2: Launch Vitis Analyzer to View Trace

To launch Vitis Analyzer for AI Engine simulation run:

```bash
vitis_analyzer ./aiesimulator_output/default.aierun_summary
```

To launch Vitis Analyzer for hardware emulation run:

```bash
vitis_analyzer ./sim/behav_waveform/xsim/default.aierun_summary
```

The hardware event trace steps are available at [AI Engine Debug with Event Trace](./Debug4_et.md)

### Step 4.3: Locate the Profiled Interface

After the `default.aierun_summary` file is opened with Vitis Analyzer, select the **Graph** view, and  locate the output file `data/ublf_out0.txt` that is associated with the `ulbfo0` output_plio object from `graph.h`.

```bash
for(unsigned k=0;k<4; k++) {
    ulout[k]=output_plio::create("ulbfo"+std::to_string(k), plio_64_bits, "data/ulbf_out"+std::to_string(k)+".txt");
    connect<>(ulbf.out[k], ulout[k].in[0]);
}
```

This outpt_plio object is configured using the profile API for output performance measurement. This can be found in `host.cpp`.

```bash
    event::handle handle1 = event::start_profiling(dut.ulout[0],  event::io_stream_start_to_bytes_transferred_cycles, OUT_LEN*sizeof(cint16)*2);
...
    if (handle1 != event::invalid_handle)
    {   
        cycle_count1 = event::read_profiling(handle1);
    }
...
    if (cycle_count1)
    {
        double throughput1 = (double)OUT_LEN*2/(cycle_count1 * 1e-9); //samples per second
        printf(" Output: Throughput %f samples\n", throughput1);
    } else {
        printf("cycle_count1 is ZERO!\n");
    }
```

![alt text](images/pr_aie_perf_tile.png)
Select the tile links to the output file by moving the cursor on top of the tile.

### Step 4.4: Locate Tile

Switch to the `Trace` view, and locate tile(21,0).

### Step 4.5: Use the Tool to Measure the Execution Time

To measure the execution time with the tool, move the marker to the beginning of kernel execution from an iteration. Add the second marker, and move the second marker to the beginning of another iteration. This method is the same to measure the execution time for AI Engine Simulation, hardware emulation, or on hardware.

![alt text](images/pr_aie_perf_vcd.png)

### Step 4.6: Performance Calculation

The above example indicates 9.400 microseconds (us) is used for 10 iterations execution time. Each iteration execution time is 0.94 us or 940 nanoseconds (ns) in average.

Per the output file `ulbf_out0.txt`, 38400 lines for 100 iterations. Each iteration processes and outputs 384 lines. Each line has two cint16 samples.

Performance calculation:

1,000,000,000(AI engine frequency in HZ) / 940(clock cycles each iteration) x 384(lines each iteration) x 2(samples per line) = 817,021,276.59(samples/second). This number is close to the profiling API reported, 818,527,715.90 samples/s.

## Limitations

Because of the limited AIE performance counters, calling the AI Engine profiling APIs can return errors. Host code is required to check profiling APIs' return code to ensure correctness of profiling APIs.

# Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [support.xilinx.com](https://support.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
