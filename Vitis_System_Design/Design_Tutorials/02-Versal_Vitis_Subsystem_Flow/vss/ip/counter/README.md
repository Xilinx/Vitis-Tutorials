<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ System Design Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# RTL Subtractor

This counter example demonstrate how to use Vivado to create a packaged RTL kernel in `xo` format which can be integrated to a system with Vitis.<br>
It also serve the purpose of creating input data stimuli to measure the propagation delay in clock cycles for
dataflow signal processing paths in AI Engine.

The counter generates 5 copies of AXI Stream data sending two samples of `cint16` each clock cycle.
This is designed to match the native port width of 64-bit AIE PLIO and be clocked up to 625 MHz.<br>

When used in the tutorial, the first AXI Stream is connected to a [subtractor](../../../vitis/ip/subtractor/README.md) as a reference signal, while the remaining 4 can be used to pass through various designs.<br>


As the paths are measured independently of each other, the counter disregard any `tready` backpressure from receiving blocks.<br>
This helps to analyze and identify the occurence of backpressure by identifying gaps in the counter sequence for respective data paths.

### Files used for this IP
| File                        | Type   | Description
| ---------------------------------|--------|-------------------------------------------------
| [counter.sv](./src/counter.sv)| RTL kernel    | Freerunning counter that disregard df backpressure.
| [pack_kernel.tcl](./pack_kernel.tcl)| Project tcl    | Setup a simple Vivado IP project to manage compilation to packaged `.xo` file.
| [counter.xdc](./counter.xdc)| Constraints file    | Placeholder file for constraints. Currently no constraints used.


### Simulating and checking the RTL IP
The project script will run synthesis and use `report_methodology` as means to check for trivial user errors. It's not required to run synthesis to generate the `xo` for Vitis.<br>
For real designs, the synthesis step should be replaced with traditional RTL verification.<br>

**Note:** For verification please use the [VSS Simulation](../../cosim/README.md) or [VFS Matlab lab](../../matlab/README.md) to simulate the behavior, or add appropriate block level verification.<br>

[Return to VSS IP](../README.md)

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2025 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
