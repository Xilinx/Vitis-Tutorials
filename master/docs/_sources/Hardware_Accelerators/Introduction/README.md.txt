<table class="sphinxhide">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ - Acceleration Tutorial for Alveo U50</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

### Introduction to Vitis Hardware Accelerators Tutorial

This tutorial is divided in 3 sections.

* [**Section 1**](./01-Workflows/README.md) (~10 mins):
  + An overview of Vitis and the host/kernel paradigm
    * See how Vitis takes care of the heavy lifting to let you focus on the application code
* [**Section 2**](./02-System_Setup/README.md) (a couple of minutes if Alveo U50 card is already installed):
  + Setup the Vitis development tools
  + Detect and check the card installed on the server
* [**Section 3**](./03-Algorithm_Acceleration/README.md) (a few hours):
  + Understand the Cholesky algorithm and run it on the CPU first
  + Re-organize the code to create both a host and a kernel program
  + Review the APIs that bind the host and the kernel
  + Apply incremental optimizations to the kernel across several modules to improve throughput. Finally program the Alveo card with the fully optimized accelerator to verify the performance

  + [**Introduction**](./03-Algorithm_Acceleration/docs/cpu_src/README.md)
    * Meet the Cholesky algorithm!
    * Run a CPU version
  + [**Vitis Module 1**](./03-Algorithm_Acceleration/docs/module1_baseline/README.md)
    * Get a performance baseline as a reference point with a first kernel design
    * Run Vitis in GUI mode or via <code>make</code>
    * Run Vitis Analyzer to visualize the application timeline
    * Run Vitis HLS to study kernel code performance and resource metrics
  + [**Vitis Module 2**](./03-Algorithm_Acceleration/docs/module2_pipeline/README.md) (short module to focus on the impact of <code>PIPELINE</code> and <code>INTERFACE</code>)
    * Understanding instruction parallelism with the HLS <code>PIPELINE</code> pragma
    * Applying the <code>INTERFACE</code> pragma to manage physical ports adapters
  + [**Vitis Module 3**](./03-Algorithm_Acceleration/docs/module3_datatype/README.md)
    * Modify design to use the more hardware efficient C++ <code>float</code> data types (compared to <code>double</code>)
    * Run Vitis, Vitis Analyzer and Vitis HLS
  + [**Vitis Module 4**](./03-Algorithm_Acceleration/docs/module4_dataflow/README.md)
    * Apply the <code>DATAFLOW</code> task parallelism optimization pragma
    * Run Vitis, Vitis Analyzer and Vitis HLS (including viewing specific dataflow waveforms)
    * Run on the U50 card

<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>
