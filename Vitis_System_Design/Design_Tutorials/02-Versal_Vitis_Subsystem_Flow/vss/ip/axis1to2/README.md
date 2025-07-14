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

# HLS Vector AXI Stream splitter
Simple HLS design that copy an input cint16 AXI Stream to two outputs.

Files used in this lab:
| Component                        | Type   | Description
| ---------------------------------|--------|-------------------------------------------------
| [axis1to2.cpp](./src/axis1to2.cpp)| HLS kernel    | Duplicates incoming AXI stream to two AXI stream outputs
| [axis1to2.h](./src/axis1to2.h)| HLS header    | Describes the kernel interfaces


## Simulating and checking the HLS kernel
**Note This component have no prebuilt testbench.** Please use the [VFS Python lab](../../python/README.md) or [VFS Matlab lab](../../matlab/README.md) as inspiration to create your own testbench!

[Return to VSS IP](../README.md)

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2025 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
