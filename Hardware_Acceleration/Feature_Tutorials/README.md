<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Feature Tutorials

The Hardware Acceleration **Feature Tutorials** illustrate specific features or flows of Vitis, some features may not be required by all designs but are still useful for some use cases.

The <a href="../README.md">README</a> of Hardware Acceleration contains important information including tool version, environment settings, and a table describing the platform, kernels, and supported features or flows of each tutorial. It is strongly recommended that you review the details before starting to use the acceleration tutorials.

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="./01-rtl_kernel_workflow/README.md">Getting Started with RTL Kernels</a></td>
 <td>This tutorial demonstrates how to use the Vitis core development kit to program an RTL kernel into an FPGA and build a Hardware Emulation using a common development flow.</td>
 </tr>
 <tr>
 <td align="center"><a href="./02-mixing-c-rtl-kernels/README.md">Mixing C and RTL</a></td>
 <td>This tutorial demonstrates how to work with an application containing RTL and C kernels, along with various design analysis features.</td>
 </tr>
<tr>
 <td align="center"><a href="./03-dataflow_debug_and_optimization/README.md">Dataflow Debug and Optimization</a></td>
 <td>This tutorial demonstrates how to debug and optimize the dataflow optimization in Vitis HLS.</td>
 </tr>
  <td align="center"><a href="./04-mult-ddr-banks/README.md">Using Multiple DDR Banks</a></td>
 <td>This tutorial demonstrates how using multiple DDRs can improve data transfer between kernels and global memory.</td>
 </tr>
  <tr>
 <td align="center"><a href="./05-using-multiple-cu/README.md">Using Multiple Compute Units</a></td>
 <td>This tutorial demonstrates the flexible kernel linking process to increase the number of kernel instances on an FPGA, which improves the parallelism in a combined host-kernel system.</td>
 </tr>
  <tr>
 <td align="center"><a href="./06-controlling-vivado-implementation/">Controlling Vivado Implementation</a></td>
 <td>This tutorial demonstrates how you can control the Vivado&reg; tools flow when implementing your project.</td>
 </tr>
  <tr>
 <td align="center"><a href="./07-using-hbm/">Optimizing for HBM</a></td>
 <td>This tutorial demonstrates how you can take best advantage of HBM on platforms that support it.</td>
 </tr>
 <tr>
<td align="center"><a href="./08-using-hostmem/">Host Memory Access</a></td>
<td>This tutorial demonstrates how kernels can directly access buffers host memory directly. This capability requires a compatible platform.</td>
</tr>
 <tr>
<td align="center"><a href="./09-using-ethernet-on-alveo/">Using GT Kernels and Ethernet IPs on Alveo</a></td>
<td>This tutorial demonstrates how to use networking GT kernels with generated Ethernet IPs and implement them on Alveo card with Vitis flow.</td>
</tr> 
 <tr>
<td align="center"><a href="./10-p2p-card-to-card/">Enabling FPGA to FPGA P2P Transfer using Native XRT C++ API</a></td>
<td>This tutorial demonstrates how to enable p2p transfer from one FPGA device to another using XRT API host code.</td>
</tr>
 </table>



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
