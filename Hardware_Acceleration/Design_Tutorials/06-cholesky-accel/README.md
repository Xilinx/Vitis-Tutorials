<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

### Vitis Hardware Acceleration on Cholesky Algorithm

***Version: Vitis 2023.1***

This tutorial is divided in three sections.

* [**Section 1**](./01-Workflows/README.md) (~10 mins):
  * An overview of Vitis and the host/kernel paradigm
    * See how Vitis takes care of the heavy lifting to let you focus on the application code
* [**Section 2**](./02-System_Setup/README.md) (a couple of minutes if an AMD Alveo™ U50 card is already installed):
  * Set up the Vitis development tools
  * Detect and check the card installed on the server
* [**Section 3**](./03-Algorithm_Acceleration/README.md) (a few hours):
  * Understand the Cholesky algorithm and run it on the central processin unit (CPU) first
  * Re-organize the code to create both a host and a kernel program
  * Review the APIs that bind the host and the kernel
  * Apply incremental optimizations to the kernel across several modules to improve throughput. Finally, program the Alveo card with the fully optimized accelerator to verify the performance

  * [**Introduction**](./03-Algorithm_Acceleration/docs/cpu_src/README.md)
    * Meet the Cholesky algorithm
    * Run a CPU version
  * [**Vitis Module 1**](./03-Algorithm_Acceleration/docs/module1_baseline/README.md)
    * Get a performance baseline as a reference point with a first kernel design
    * Run Vitis in GUI mode or via `make`
    * Run Vitis Analyzer to visualize the application timeline
    * Run Vitis HLS to study kernel code performance and resource metrics
  * [**Vitis Module 2**](./03-Algorithm_Acceleration/docs/module2_pipeline/README.md) (short module to focus on the impact of `PIPELINE` and `INTERFACE`)
    * Understanding instruction parallelism with the HLS `PIPELINE` pragma
    * Applying the `INTERFACE` pragma to manage physical ports adapters
  * [**Vitis Module 3**](./03-Algorithm_Acceleration/docs/module3_datatype/README.md)
    * Modify design to use the more hardware efficient C++ `float` data types (compared to `double`)
    * Run Vitis, Vitis Analyzer, and Vitis HLS
  * [**Vitis Module 4**](./03-Algorithm_Acceleration/docs/module4_dataflow/README.md)
    * Apply the `DATAFLOW` task parallelism optimization pragma
    * Run Vitis, Vitis Analyzer, and Vitis HLS (including viewing specific dataflow waveforms)
    * Run on the U50 card

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
