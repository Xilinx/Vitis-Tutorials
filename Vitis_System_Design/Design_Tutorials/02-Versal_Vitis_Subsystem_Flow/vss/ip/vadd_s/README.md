
<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ In-Depth Tutorials</h1>
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
