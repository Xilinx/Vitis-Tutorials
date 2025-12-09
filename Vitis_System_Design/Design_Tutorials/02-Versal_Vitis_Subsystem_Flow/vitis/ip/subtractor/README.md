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

The subtractor has 5 AXI Stream inputs, each with 64-bit data, and a AXI4-Lite bus interface.

It has 4 subtractors, where the first AXI Stream input is the reference common to all subtractions, and the remaining 4 are individual per subtraction.

The subtraction use DSP element and is designed to run up to 625 MHz and performs subtraction using the real part from one of the two cint16 samples from the AXI stream.

To reduce timing closure issues, the AXI4-Lite parts are clocked separately using<br>
***f<sub>axi</sub> = f<sub>dsp</sub> / 2***

A simple hold circuit is used before the results are collected by the AXI4-Lite registers.

## Files used for this IP

| File                                                   | Type              | Description                                                                                                      |
|--------------------------------------------------------|-------------------|------------------------------------------------------------------------------------------------------------------|
| [subtractor.v](./src/subtractor.v)                     | RTL kernel        | Implements four subtractors with results stored in an AXI4-Lite status register.                                 |
| [subtractor_S00_AXI.v](./src/subtractor_S00_AXI.v)     | RTL kernel        | AXI4-Lite slave using a modified code template.                                                                  |
| [my_diff.v](./src/my_diff.v)                           | RTL kernel        | Code to infer a subtractor using DSP58/DSP48 primitives.                                                         |
| [pack_kernel.tcl](./pack_kernel.tcl)                   | Project Tcl script| Sets up a simple Vivado IP project to compile and package the design as a `.xo` file.                            |
| [subtractor.xdc](./subtractor.xdc)                     | Constraints file  | Placeholder file for constraints. Currently, no constraints are used.                                            |


### Simulating and checking the RTL IP

The project script will run synthesis and use `report_methodology` as means to check for trivial user errors. It's not required to run synthesis to generate the `xo` for Vitis.

For real designs, the synthesis step should be replaced with traditional RTL verification.

> **Note** It's encouraged to setup either traditional RTL verification or run hardware emulation to verify this block.

[Return to Vitis IP](../README.md)

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2025 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
