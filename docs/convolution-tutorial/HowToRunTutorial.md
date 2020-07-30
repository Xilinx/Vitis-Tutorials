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

# Understanding the Makefile

The source files required for this lab are all under the `design` folder. The `design` folder contains three sub-directories: `cpu_src` with the original application code, `src` with the accelerated application code, and `makefile`.

Under the `src` folder, there are six sub-folders, which contain the source files for each step. The following directories correspond to each step of the tutorial.

| Tutorial Step                                             | Associated Directory  |
|-------------------------------------------------------------|------------------------ |
| Creating a Vitis core development kit application from C application          |     `src/baseline`        |
| Optimizing memory transfers                                  |     `src/localbuf`        |
| Optimizing by using fixed point data types:                 |     `src/fixedpoint`      |
| Optimizing with dataflow                                    |     `src/dataflow`        |
| Using Out-of-order queues and multiple kernels              |     `src/multicu`         |
| Using Using QDMA with Multiple Compute Units              |     `src/qdma`         |

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

Use the following command to view the Profile Summary report and Timeline Trace.

   ```
   make view_run_summary TARGET=hw_emu STEP=multicu
   ```

   The makefile uses the following command to open the Profile Summary report and Timeline Trace report.

   ```
   vitis_analyzer run_summary
   ```

## Makefile Targets for Clean Up

To clean the previously generated build, use the following command.

```
make clean TARGET=hw_emu STEP=baseline
```

This cleans the hardware emulation-generated files for the baseline version design.
</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="/docs/vitis-getting-started/README.md">Return to Getting Started Pathway</a> — <a href="/docs/convolution-tutorial/README.md">Return to Start of Tutorial</a></b></p>

<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
