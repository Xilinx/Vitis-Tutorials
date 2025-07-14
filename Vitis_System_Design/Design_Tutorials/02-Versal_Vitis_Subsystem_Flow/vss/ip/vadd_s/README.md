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

# HLS Vector adder

Simple HLS design taking two cint16 input vectors from axi stream and adds them together using complex addition.

Files used in this lab:
| Component                        | Type   | Description
| ---------------------------------|--------|-------------------------------------------------
| [vadd_s.cpp](./src/vadd_s.cpp)| HLS kernel    | Adds two input vectors to an output vector
| [vadd_s.h](./src/vadd_s.h)| HLS header    | Describes the kernel interfaces


## Simulating and checking the HLS kernel
**Note** Please use the [VFS Python lab](../../python/README.md) to run functional verification.



<p class="sphinxhide" align="center"><sub>Copyright © 2021-2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022-2025 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
