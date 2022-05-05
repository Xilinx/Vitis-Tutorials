<table>
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Debug Walkthrough Tutorial - From Simulation to Hardware</h1>
   </td>
 </tr>
 <tr>
 <td align="center"><h1>Profiling Graph Inputs and Outputs with Profiling APIs</h1>
 </td>
 </tr>
</table>

## Overview

In this tutorial you will learn how to:

* Add profiling APIs in graph and PS code to calculate design performance.
* Generate VCD file to view design information.
* Cross check between calculated performance with profiling APIs and vitis_analyzer trace view.
* Use profiling features to inspect design.

## Profiling APIs
Start profiling API,
```bash
/// Start profiling and acquire resources needed for profiling. Should be called after graph::init().
/// @param io Plarform PLIO or GMIO object.
/// @param option io_profiling_option enum.
/// @param value Optional value for the specified option.
/// @return Return event::handle to be used by read_profiling and stop_profiling. Return event::invalid_handle for error conditions or unsupported use cases.
static handle start_profiling(IoAttr& io, io_profiling_option option, uint32 value = 0);
```
Read profiling API,
```bash
/// Read profiling.
/// @param h event::handle returned from start_profiling.
/// @return Profiling value.
static long long read_profiling(handle h);
```
Stop profiling API,
```bash
/// Stop profiling and release resources needed for profiling.
/// @param h event::handle returned from start_profiling.
static void stop_profiling(handle h);
```


[1. Profiling APIs with AIE Simulator](#1-Profiling-APIs-with-AIE-Simulator)

[2. Profiling APIs with HW Emulator](#2-Profiling-APIs-with-HW-Emulator)

[3. Profiling APIs on HW](#3-Profiling-APIs-on-HW)

[4. Cross Check I/O Performance values with VCD](#4-Cross-Check-I/O-Performance-values-with-VCD)


## 1. Profiling APIs with AIE Simulator

### Step 1.1 Download the Project
Clone the project source from git repository and unzip the zip file.

### Step 1.2 Prepare Profiling APIs applied graph code
Use this tutorial's graph.cpp.profile_api that contains AIE profiling APIs.
```bash
cd ${DOWNLOAD_PATH}/AI_Engine_Development/Feature_Tutorials/09-debug-walkthrough/aie
cp graph.cpp.profile_api graph.cpp
cd ..
cp Makefile.emu Makefile
make libadf.a
```

### Step 1.3 Run AIE Simulator
```bash
aiesimulator --pkg-dir=./Work --i=. --profile
```

### Step 1.4 Expected Result
<img src="images/pr_aie_perf.png" width="600">


```bash
vitis_analyzer ./aiesimulator_output/default.aierun_summary
```

## 2. Profiling APIs with HW Emulator

### Step 2.1 Prepare profiling APIs applied host code and Makefile
After step 1.1 completed.
Use this tutorial's host.cpp.profile_api that contains AIE profiling APIs and Makefile for HW Emulation.
```bash
cd ${DOWNLOAD_PATH}/AI_Engine_Development/Feature_Tutorials/09-debug-walkthrough/sw
cp host.cpp.profile_api host.cpp
cd ..
cp Makefile.emu Makefile
```

### Step 2.2 Build the Design
```bash
make
```

### Step 2.3 Run Hardware Emulator
```bash
./launch_hw_emu.sh
```

### Step 2.4 Run Application
After Petalinux boots up.
```bash
cd /run/media/mmcblk0p1
./host.exe a.xclbin
```
<img src="images/pr_hw_emu_perf.png" width="600">


## 3. Profiling APIs on HW
### Step 3.1 Prepare HW Makefile
After step 1.1 completed.
Use this tutorial's Makefile for Hardware.
```bash
cd ${DOWNLOAD_PATH}/AI_Engine_Development/Feature_Tutorials/09-debug-walkthrough
cp Makefile.profile_hw Makefile
make
```

### Step 3.2 Flash SD card
Flash generated sd_card.img to SD card.

### Step 3.3 Boots up VCK190 board
Plug in flashed completed SD card to vck190 board's sd slot.
Power up the vck190 board.

### Step 3.4 Run the Application
After vck190 board boots up and ready to accepts commands with Linux prompt, issue these commands from terminal.
```bash
cd /run/media/mmcblk0p1
./host.exe a.xclbin
```

### Step 3.5 Expected result
<img src="images/pr_hw_perf.png" width="600">

Note: Due to slower memory access, hardware and hardware emulation performance values are not optimized and less than AIE simulation.


## 4. Cross Check I/O Performance values with VCD
### Step 4.1 VCD generation
Command to generate AIE simulation VCD file.
```bash
aiesimulator --pkg-dir=./Work --i=. --dump-vcd=foo
```
Command to generate hardware emulation VCD file.
```bash
./launch_hw_emu.sh -add-env AIE_COMPILER_WORKDIR=${PROJECT_FULL_PATH}/Work -aie-sim-options ${PROJECT_FULL_PATH}/aiesim_options.txt
```
Where aiesim_options.txt content is
```bash
AIE_DUMP_VCD=foo
```
Follow step 2.4 to run application.

### Step 4.2 Launch vitis_analyzer to view trace
To launch vitis_analyser for AIE simulation run.
```bash
vitis_analyzer ./aiesimulator_output/default.aierun_summary
```
To launch vitis_analyser for hardware emulation run.
```bash
vitis_analyzer ./sim/behav_waveform/xsim/default.aierun_summary
```

For hardware event trace steps that are available at https://gitenterprise.xilinx.com/swm/Vitis-Tutorials/blob/2022.1_next/AI_Engine_Development/Feature_Tutorials/09-debug-walkthrough/Debug4_et.md

### Step 4.3 Locate profiled interface
After default.aierun_summary file is opened with Vitis_analyzer, select `Graph` view, locate the output file `data/ublf_out0.txt` that associated with `ulbfo0` output_plio object from graph.h
```bash
for(unsigned k=0;k<4; k++) {
    ulout[k]=output_plio::create("ulbfo"+std::to_string(k), plio_64_bits, "data/ulbf_out"+std::to_string(k)+".txt");
    connect<>(ulbf.out[k], ulout[k].in[0]);
}
```
This outpt_plio object is configured using profile API for output performance measurement. This can be found from host.cpp
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
<img src="images/pr_aie_perf_tile.png" width="900">
Select the tile links to the output file by moving cursor on top of the tile.

### Step 4.4 Locate tile
Switch to `Trace` view, Locate tile(21,0).

### Step 4.5 Use tool to measure execution time
To measure execution time with tool, move marker to beginning of kernel execution from an iteration. Add second marker and move the second marker to beginning of another iteration. This method is same to measure execution time for AIE Simulation, hardware emulaiton or on Hardware.

<img src="images/pr_aie_perf_vcd.png" width="900">

### Step 4.6 Performance Calculation
Above example indicates 9.400 microseconds (us) is used for 10 iterations execution time. Each iteration execution time is 0.94 us or 940 nanoseconds (ns) in average.

Per output file `ulbf_out0.txt`, 38400 lines for 100 iterations. Each iteration processes and outputs 384 lines. Each line has 2 cint16 samples.

Performance calculation:

1,000,000,000(AI engine frequency in HZ) / 940(clock cycles each iteration) x 384(lines each iteration) x 2(samples per line) = 817,021,276.59(samples/second). This number is close to profiling API reported, 818,527,715.90 samples/s.



## Limitations
Due to limited AIE performance counters, calling AIE profiling APIs may return errors. Host code is required to check profiling APIs' return code to ensure correctness of profiling APIs.

# Support

GitHub issues will be used for tracking requests and bugs. For questions go to [support.xilinx.com](https://support.xilinx.com/).

# License

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0


Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

<p align="center"><sup>XD005 | &copy; Copyright 2021 Xilinx, Inc.</sup></p>
