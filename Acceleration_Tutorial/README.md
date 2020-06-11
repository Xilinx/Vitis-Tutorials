<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ — Acceleration Tutorial —</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

This tutorial is divided in 3 sections.
* **Section 1**:
  + A walkthrough of the algorithm chosen for the tutoria: the Cholesky decomposition 
  + Compile and run the non-accelerated version of the algorithm on the CPU
  + Evaluate the performance
* **Section 2**:
  + Setup the Vitis development tools
  + Detect and check the card installed on the server 
* **Section 3**:
  + Re-organize the code to extract the algorithm in its own unit (the kernel)
  + Review the APIs needed to enable communication between the host and the kernel
  + Apply optimizations to the kernel to improve its throughput
  + Program the Alveo card with the optimized accelerator
