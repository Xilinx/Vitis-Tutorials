<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ - Acceleration Tutorial</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

This tutorial is divided in 3 sections.
* Section 1: Setting up the design to provide a baseline performance evaluation
  + Understand the OpenCL APIs that help connect host code to the kernel
  + Verify the results through both sw_emu and hw_emu
  + Evaluate the performance with Vitis Analyzer
* Section 2: Apply the PIPELINE to improve the kernel throughput
  + Using the module 1 results as a starting point to review the kernel performance in Vitis HLS
  + Run the sw_emu step and use the hw_emu results to quantify the improvement 
* Section 3: Add the task parallelism pragma to further improve the kernel
  + Reuse the results from module 2 to re-arrange the code to use the DATAFLOW optimization
  + Evaluate the performance with Vitis Analyzer
  + Create the binary (xclbin) to program the card and perform actual measurements
