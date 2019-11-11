<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ Application Acceleration Development Flow Tutorials</h1>
   <a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">See SDAccel™ Development Environment 2019.1 Tutorials</a>
   </td>
 </tr>
 <tr>
 <td align="center"><h1>Optimizing Accelerated FPGA Applications: Bloom Filter Example
 </td>
 </tr>
</table>

# Understanding the Makefile

The source files required for this lab are located in the `design` folder. The `design` folder contains three sub-directories: `cpu_src` with the original application code, `src` with the accelerated application code, and `makefile`.

Under the `src` folder, there are five sub-folders, which contain the source files for each step. The following directories correspond to each step of the tutorial.

| Tutorial Step                                             | Associated Directory  |
|-------------------------------------------------------------|------------------------ |
| Creating a Vitis Core Development Kit Application from C Application          |     [src/baseline_fpga](./design/src/baseline_fpga)        |
| Optimizing Memory Transfers                                  |     [src/localbuf](./design/src/localbuf)        |
| Optimizing with Dataflow                                    |     [src/dataflow](./design/src/dataflow)        |
| Using Out-of-Order Queues and Multiple Kernels              |     [src/multicu](./design/src/multicu)         |
| Optimizing with Multiple DDR Banks                 |     [src/multiddr](./design/src/multiddr)      |

Makefile targets can be used to run software emulation, hardware emulation, as well as hardware on the Alveo Data Center accelerator card.

## Makefile Targets for Building Designs

To build the design, use the following command.

```
make build TARGET=<sw_emu/hw_emu/hw> STEP=<baseline/localbuf/dataflow/multicu/multiddr>
```

Along with the `src` directory that can be used to update the code based on the tutorial, the `reference-files` directory is also provided. You can directly use files in the `reference-files` directory by setting `NUM_DOCS=100` for the makefile targets.
`NUM_DOCS` represents the number of documents you are computing score for by running this application.

For example, to build the hardware emulation xclbin for the baseline lab, use the following command.

```
make build TARGET=hw_emu STEP=baseline NUM_DOCS=100
```

## Makefile Targets for Running Designs

To run the design in emulation or hardware, use the following command.

```
make run TARGET=<sw_emu/hw_emu/hw> STEP=<baseline/localbuf/dataflow/multicu/multiddr> NUM_DOCS=100
```

The `NUM_DOCS` option defines the number of documents running for the host application execution. For software and hardware emulation, it is recommended to set `NUM_DOCS` to **100** to reduce the emulation time.

For example, to run hardware emulation for baseline step, use the following command.

```
make run TARGET=hw_emu STEP=baseline NUM_DOCS=100
```

## Makefile Targets for Generating Profile and Timeline Trace Reports

To analyze the performance of the design, you need to look at the reports. For details about the reports, refer to the [Generate Profile and Trace Reports](../Pathway3/ProfileAndTraceReports.md) lab in the Essential Concepts for Building and Running the Accelerated Application tutorial.

## Makefile Targets for Viewing Profile and Timeline Trace

Use the following command to view the Profile Summary and Timeline Trace reports.

   ```
   make view_report TARGET=hw_emu STEP=baseline
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
