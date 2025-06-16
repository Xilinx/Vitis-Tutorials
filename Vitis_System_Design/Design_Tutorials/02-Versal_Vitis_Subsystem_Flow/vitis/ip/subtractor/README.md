
<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ In-Depth Tutorials</h1>
    </td>
 </tr>
</table>

# RTL Subtractor
The subtractor has 5 AXI Stream inputs, each with 64-bit data, and a AXI4-Lite bus interface.
It has 4 subtractors, where the first AXI Stream input is the reference common to all subtractions, and the remaining 4 are individual per subtraction.<br>

The subtraction use DSP element and is designed to run up to 625 MHz and performs subtraction using the real part from one of the two cint16 samples from the AXI stream.

To reduce timing closure issues, the AXI4-Lite parts are clocked separately using<br>
***f<sub>axi</sub> = f<sub>dsp</sub> / 2***

A simple hold circuit is used before the results are collected by the AXI4-Lite registers.

### Files used for this IP
| File                        | Type   | Description
| ---------------------------------|--------|-------------------------------------------------
| [subtractor.v](./src/subtractor.v)| RTL kernel    | Four subtractors with results in AXI4-Lite status register.
| [subtractor_S00_AXI.v](./src/subtractor_S00_AXI.v)| RTL kernel    | AXI4-Lite slave using modified code template.
| [my_diff.v](./src/my_diff.v)| RTL kernel    | Code to infer a subtractor using DSP58/48.
| [pack_kernel.tcl](./pack_kernel.tcl)| Project tcl    | Setup a simple Vivado IP project to manage compilation to packaged `.xo` file.
| [subtractor.xdc](./counter.xdc)| Constraints file    | Placeholder file for constraints. Currently no constraints used.


### Simulating and checking the RTL IP
The project script will run synthesis and use `report_methodology` as means to check for trivial user errors. It's not required to run synthesis to generate the `xo` for Vitis.<br>
For real designs, the synthesis step should be replaced with traditional RTL verification.<br>

**Note** It's encouraged to setup either traditional RTL verification or run hardware emulation to verify this block.

[Return to Vitis IP](../README.md)

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2025 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
