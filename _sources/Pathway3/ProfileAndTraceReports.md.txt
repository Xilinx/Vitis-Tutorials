<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ Application Acceleration Development Flow Tutorials</h1>
   <a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">See 2019.2 Vitis Application Acceleration Development Flow Tutorials</a>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# 3. Generating Profile and Trace Reports

## Introduction

To better understand the design, you can generate a Profile Summary and Application Timeline reports.  

* The Profile Summary provides annotated details regarding the overall application performance.  
* The Application Timeline collects and displays host and device events on a common timeline to help you understand and visualize the overall health and performance of your systems.

The Profile Summary and Timeline Trace reports are available for all build targets (`sw_emu`, `hw_emu`, and `hw`). However, the granularity of the reports can be different for different build targets. For example, with the `sw_emu` build, the Profile Summary report will not include any data transfer details under kernel execution efficiency and data transfer efficiency.

This lab uses Hardware Emulation to illustrate the steps of profiling. The steps here can be easily migrated to other flows by targeting `sw_emu` or `hw` (`-t sw_emu` or `-t hw`).

>**IMPORTANT:** Turning on device profiling for the system build (`-t hw`) is intrusive and can negatively affect overall application performance. This feature should only be used for performance debugging and should be disabled in the production build.

## Before You Begin

Before running this lab, it is required to run the [Building an Application](./BuildingAnApplication.md) lab and [Running Software and Hardware Emulation](./Emulation.md) lab.

## Generating Emulation Data

The Profile Summary and Application Timeline reports are not enabled by default because the generated emulation data requires more time to run and added disk space. Therefore, you must enable the profiling data collection before running emulation. This is done by setting options in an `xrt.ini` text file, located in the same directory as the host program, in this case the `./reference-files/run` directory. For more information on the `xrt.ini` file, see the [Vitis Environment Reference Materials](https://www.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=yxl1556143111967.html) in the Application Acceleration Development flow of the Vitis Unified Software Platform Documentation (UG1416).

### Create the xrt.ini File

First, create the `xrt.ini` file, and add the required options. The `xrt.ini` file configures Xilinx Runtime (XRT) to support a variety of features for debug, emulation, and reporting, that can be enabled at runtime. These features can be enabled when needed and disabled to improve performance and reduce application resource consumption.

1. In the `./reference-files/run` directory, create a file called `xrt.ini`.

2. Enter following lines in the file.

   ```
   [Debug]
   profile=true
   timeline_trace=true
   data_transfer_trace=fine
   ```

   >**Command Options and Descriptions**
   >
   >* `[Debug]`: Debug specific commands to follow
   >* `profile=true`: Enable profile monitoring
   >* `timeline_trace=true`: Enable timeline trace information gathering
   >* `data_transfer_trace=<value>`: Enables device-level AXI data transfer information.
    >   * `fine`: Show all AXI-level burst data transfers.
    >   * `coarse`: Show the compute unit (CU) transfer activity from the beginning of first transfer to end of last transfer (before compute unit transfer ends).
    >   * `off`: Turn off reading and reporting of device-level trace during runtime.

3. Save and close the file.

### Specify --profile_kernel to Build and Run Hardware Emulation

With profiling and the timeline trace enabled in the `xrt.ini` file, performance data is gathered during emulation and saved to CSV files. However, it is also necessary to build the device binary (.xclbin) using the `--profile_kernel` option during the hardware linking stage to allow performance information to be captured from the Xilinx device. For more information on `--profile_kernel`, refer to the [Vitis Environment Reference Materials](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/yxl1556143111967.html) in the Application Acceleration Development flow of the Vitis Unified Software Platform Documentation (UG1416). 

1. Edit the `design.cfg` file to add the `--profile_kernel` option as shown below:

   ```
   platform=xilinx_u200_xdma_201830_2
   debug=1
   save-temps=1
   profile_kernel=data:all:all:all

   [connectivity]
   nk=mmult:1:mmult_1
   ```

1. Rebuild the kernel using the following commands.

     ```bash
     v++ -t hw_emu --config design.cfg -c -k mmult -I'../src' -o mmult.hw_emu.xilinx_u200_xdma_201830_2.xo ../src/mmult.cpp
     v++ -t hw_emu --config design.cfg -l -o mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin mmult.hw_emu.xilinx_u200_xdma_201830_2.xo
     ```

   >**TIP:** The `--profile_kernel` option can also be specified on the `v++` command line.

   This generates a new `.xclbin` file which supports profiling the kernels. For details on building a design, refer to the [Building an Application](./BuildingAnApplication.md) lab.

2. After the build is complete, run emulation using the following commands.  

   ```bash
   emconfigutil --platform xilinx_u200_xdma_201830_2
   export XCL_EMULATION_MODE=hw_emu
   ./host mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin
   ```

### View the Run Summary

After running emulation, the following files are generated in the execution directory.

* `run_summary` (Vitis analyzer Run Summary)
* `profile_summary.csv` (Profile Report)
* `timeline_trace.csv` (Timeline Trace)

The Vitis analyzer can be used to load the `run_summary` file, which also loads the Profile Summary and Timeline Trace. For more information on the Vitis analyzer, see to the [Using the Vitis Analyzer](https://www.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=jfn1567005096685.html) in the Application Acceleration Development flow of the Vitis Unified Software Platform Documentation (UG1416).

>**IMPORTANT:** The `xrt.ini` must have `profile=true` and `timeline_trace=true` to generate the run_summary from the hardware emulation run, as described earlier in this Lab.

1. Run the following command to open the `run_summary`:

   ```bash
   vitis_analyzer mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin.run_summary
   ```

2. Select the Profile Summary report, as shown in the following figure. You can see information related to kernel execution time, host to global memory data transfer and kernel to global memory data transfer.

    ![ProfileReport](./images/vitis_analyzer-profile_summary.png)

   3. Select the Timeline Trace view. You will see information about the events including kernel running, data read and write from host, OpenCL API calls, etc.
   
   ![Timeline](./images/vitis_analyzer-timeline_trace.png)

      For more information about interpreting the Profile Summary and Timeline Trace reports, refer to the [Profiling, Optimizing, and Debugging the Application](https://www.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=wzc1553475252001.html) in the Application Acceleration Development flow of the Vitis Unified Software Platform Documentation (UG1416).

## Putting it All Together

Below is a summary of all the steps necessary to generate and view Profile Summary and Timeline Trace reports.  

1. Create the `xrt.ini` file.

   ```bash
   [Debug]
   profile=true
   timeline_trace=true
   data_transfer_trace=fine
   ```

2. Add the `--profile_kernel` option to the `design.cfg` file.

   ```
   platform=xilinx_u200_xdma_201830_2
   debug=1
   save-temps=1
   profile_kernel=data:all:all:all

   [connectivity]
   nk=mmult:1:mmult_1
   ```

3. Build the platform with `--profile_kernel` option.

   ```bash
   v++ -t hw_emu --config design.cfg -c -k mmult -I'../src' -o mmult.hw_emu.xilinx_u200_xdma_201830_2.xo ../src/mmult.cpp
   v++ -t hw_emu --config design.cfg -l  -o mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin mmult.hw_emu.xilinx_u200_xdma_201830_2.xo
   ```

4. Run the application.

   ```bash
   emconfigutil --platform xilinx_u200_xdma_201830_2
   export XCL_EMULATION_MODE=hw_emu
   ./host mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin
   ```

5. View the Run Summary, and the associated reports.

   ```bash
   vitis_analyzer mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin.run_summary
   ```

# Next Steps

After you perform emulation to check correctness and performance, run application in a real device. The [Execute in Hardware](./HardwareExec.md) lab details how to run application in hardware.
</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="../vitis-getting-started/README.md">Return to Getting Started Pathway</a> — <a href="/docs/Pathway3/README.md">Return to Start of Tutorial</a></b></p>

<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
