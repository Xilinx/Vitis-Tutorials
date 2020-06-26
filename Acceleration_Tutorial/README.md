<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ - Acceleration Tutorial for Alveo U50</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

This tutorial is divided in 3 sections.
* [**Section 1**](./Section_1-Workflows) (~10 mins):
  + A walkthrough of the algorithm chosen for the tutorial: the Cholesky matrix decomposition 
  + [**Cholesky on the CPU:**](./docs/cpu_src)
    * Evaluate the CPU version of the algorithm changing the matrix size
* [**Section 2**](./Section_2-System_Setup) (a couple of minutes if Alveo U50 card is already installed):
  + Setup the Vitis development tools
  + Detect and check the card installed on the server 
* [**Section 3**](./Section_3-Algorithm_Acceleration) (a few hours):
  + Re-organize the code to create both a host and a kernel program
  + Review the APIs needed to bind the host and the kernel
  + Apply incremental optimizations to the kernel across several modules to improve throughput. Finally program the Alveo card with the fully optimized accelerator to verify the performance
  + [**Module 1:**](./docs/module1_baseline)
    * Get a performance baseline as a reference point
  + [**Module 2**](./docs/module2_pipeline)
    * Boost throughput with the HLS PIPELINE pragma
  + [**Module 3**](./docs/module3_dependency_removal)
    * Remove dependencies on data accesses to improve performance with the DEPENDENCE pragma
  + [**Module 4**](./docs/module4_flatten_loop)
    * Flatten critical loop to improve design
  + [**Module 5**](./docs/module5_dataflow)
    * Apply the DATAFLOW task parallelism optimization pragma
