<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ - Acceleration Tutorial for Alveo U50</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

### Getting Started Tutorial

This tutorial is divided in 3 sections.

* [**Section 1**](./Getting_Started/01-Workflows) (~10 mins):
  + An overview of Vitis and the host/kernel paradigm
    * See how Vitis takes care of the heavy lifting to let you focus on the application code
* [**Section 2**](./Getting_Started/02-System_Setup) (a couple of minutes if Alveo U50 card is already installed):
  + Setup the Vitis development tools
  + Detect and check the card installed on the server
* [**Section 3**](./Getting_Started/03-Hardware_Accelerators) (a few hours):
  + Understand the Cholesky algorithm and run it on the CPU first
  + Re-organize the code to create both a host and a kernel program
  + Review the APIs that bind the host and the kernel
  + Apply incremental optimizations to the kernel across several modules to improve throughput. Finally program the Alveo card with the fully optimized accelerator to verify the performance

  + [**Introduction**](./Getting_Started/03-Hardware_Accelerators/docs/cpu_src)
    * Meet the Cholesky algorithm!
    * Run a CPU version
  + [**Vitis Module 1**](./Getting_Started/03-Hardware_Accelerators/docs/module1_baseline)
    * Get a performance baseline as a reference point with a first kernel design
    * Run Vitis in GUI mode or via <code>make</code>
    * Run Vitis Analyzer to visualize the application timeline
    * Run Vitis HLS to study kernel code performance and resource metrics
  + [**Vitis Module 2**](./Getting_Started/03-Hardware_Accelerators/docs/module2_pipeline) (short module to focus on the impact of <code>PIPELINE</code> and <code>INTERFACE</code>)
    * Understanding instruction parallelism with the HLS <code>PIPELINE</code> pragma
    * Applying the <code>INTERFACE</code> pragma to manage physical ports adapters
  + [**Vitis Module 3**](./Getting_Started/03-Hardware_Accelerators/docs/module3_datatype)
    * Modify design to use the more hardware efficient C++ <code>float</code> data types (compared to <code>double</code>)
    * Run Vitis, Vitis Analyzer and Vitis HLS
  + [**Vitis Module 4**](./Getting_Started/03-Hardware_Accelerators/docs/module4_dataflow)
    * Apply the <code>DATAFLOW</code> task parallelism optimization pragma
    * Run Vitis, Vitis Analyzer and Vitis HLS (including viewing specific dataflow waveforms)
    * Run on the U50 card

### Design Tutorials - Optimizing Accelerated FPGA Applications Based on Vitis Environment Methodology

The methodology for developing optimized accelerated applications is comprised of two major phases: architecting the application, and developing the hardware kernels. In the first phase, you make key decisions about the application architecture by determining which software functions should be accelerated onto FPGA kernels, how much parallelism can be achieved, and how to deliver it in code. In the second phase, you implement the kernels by structuring the source code, and applying the necessary compiler options and pragmas to create the kernel architecture needed to achieve the optimized performance target.

The following examples illustrate the use of this methodology in real-world applications:

* [Convolution Example](./Design_Tutorials/01-convolution-tutorial/README.md)
* [Bloom Filter Example](./Design_Tutorials/02-bloom/README.md)

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
