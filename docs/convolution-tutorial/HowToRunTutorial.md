<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ Application Acceleration Development Flow Tutorials</h1>
   <a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">See other versions</a>
   </td>
 </tr>
 <tr>
 <td align="center"><h1>Methodology for Optimizing Accelerated FPGA Applications
 </td>
 </tr>
</table>

# Understanding the Makefile

The source files required for this lab are all under the `design` folder. The `design` folder contains three sub-directories: `cpu_src` with the original application code, `src` with the accelerated application code, and `makefile`.

Under the `src` folder, there are six sub-folders, which contain the source files for each step. The following directories correspond to each step of the tutorial.

| Tutorial Step                                             | Associated Directory  |
|-------------------------------------------------------------|------------------------ |
| Creating a Vitis core development kit application from C application          |     [src/baseline](./design/src/baseline)        |
| Optimizing memory transfers                                  |     [src/localbuf](./design/src/localbuf)        |
| Optimizing by using fixed point data types:                 |     [src/fixedpoint](./design/src/fixedpoint)      |
| Optimizing with dataflow                                    |     [src/dataflow](./design/src/dataflow)        |
| Using Out-of-order queues and multiple kernels              |     [src/multicu](./design/src/multicu)         |

Makefile targets can be used to run software emulation, hardware emulation, as well as hardware on the Alveo Data Center accelerator card.

## Makefile Targets for Building Designs

To build the design, use the following command.

```
make build TARGET=<sw_emu/hw_emu/hw> STEP=<baseline/localbuf/fixedpoint/dataflow/multicu/> SOLUTION=1
```

Along with the `src` directory, that can be used to update the code based on tutorials, the `reference-files` directory is also provided. You can directly use files in the `reference-files` directory by setting `SOLUTION=1` for the makefile targets.

For example, to build the hardware emulation xclbin for the baseline lab, use the following command.

```
make build TARGET=hw_emu STEP=baseline SOLUTION=1
```

## Makefile Targets for Running Designs

To run the design in emulation or hardware, use the following command.

```
make run TARGET=<sw_emu/hw_emu/hw> STEP=<baseline/localbuf/fixedpoint/dataflow/multicu> SOLUTION=1 [NUM_FRAMES=1]
```

The NUM_FRAMES option defines the input video frame number used for the host application execution. For software and hardware emulation, it is recommended to set NUM_FRAMES to **1** to reduce emulation time. For hardware run, do not use this option; a full video file will be used.

For example, to run hardware emulation for baseline step, use the following command.

```
make run TARGET=hw_emu STEP=baseline SOLUTION=1 NUM_FRAMES=1
```

## Makefile Targets for Generating Profile and Timeline Trace Reports

To analyze the performance of the design, you need to look at the reports. For details about the reports, refer to the [Generate Profile and Trace Reports](../Pathway3/ProfileAndTraceReports.md) lab in the Essential Concepts for Building and Running the Accelerated Application tutorial.


## Makefile Targets for Viewing Profile and Timeline Trace

1. Use the following command to view the Profile Summary report.

   ```
   make view_prof_report TARGET=hw_emu STEP=multicu
   ```

   The makefile uses the following command to open the Profile Summary report.

   ```
   vitis_analyzer -open ./sdaccel_profile_summary_hw_emu.csv
   ```

2. Use the following command to view the Timeline Trace report.

   ```
   make view_timeline_trace TARGET=hw STEP=multicu
   ```

   The makefile uses the following command to launch the Vitis IDE and display the Timeline Trace report.

   ```
   vitis_analyzer -open ./sdaccel_timeline_trace_hw.csv
   ```

## Makefile Targets for Clean Up

To clean the previously generated build, use the following command.

```
make clean TARGET=hw_emu STEP=baseline
```

This cleans the hardware emulation-generated files for the baseline version design.

</br>
<hr/>
<p align="center"><b><a href="/docs/vitis-getting-started/">Return to Getting Started Pathway</a> — <a href="./README.md">Return to Start of Tutorial</a></b></p>

<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>
