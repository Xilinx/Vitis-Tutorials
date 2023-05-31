<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Section 3: Algorithm Acceleration

> **_In this section..._**
_1 Introductory reference module in which we run the CPU version of the algorithm in ./cpu_src</code>_
_4 Alveo U50 modules, located under the ./docs</code> directory_
_Instructions in local readme files for each module_

* [**Introduction — CPU Run**](./docs/cpu_src/README.md): The C++ implementation of the algorithm
  * Run a C++ non-accelerated version of Cholesky algorithm
* [**Module 1**](./docs/module1_baseline/README.md): Setting up the design and establish a performance **baseline**
  * Understand the host OpenCL APIs that help connect to the kernel implemented onto an AMD device
  * Verify results through emulation both at the software level (sw_emu) and the hardware level (hw_emu)
  * Evaluate the performance by visualizing the timeline trace with Vitis Analyzer
  * Launch Vitis HLS to review the kernel optimizations
* [**Module 2**](./docs/module2_pipeline/README.md): This version of the code explicitely applies the `PIPELINE` and `INTERFACE` directive
  * Learn about these pragmas and their impact on designs
* [**Module 3**](./docs/module3_datatype/README.md): Change `double` data types to `float`
  * Run hardware emulation and then Vitis Analyzer and Vitis HLS
  * Measure the impact on physical resources required to implement the design and performance
* [**Module 4**](./docs/module4_dataflow/README.md): Back to using `double`, the task parallelism pragma is applied to improve results
  * Re-arrange code to enable the task parallelism optimization `DATAFLOW` pragma
  * Evaluate the performance improvement with Vitis Analyzer
  * Use Vitis HLS to confirm the new micro-architecture created by dataflow
  * Generate the binary (xclbin) to program the card and measure the actual performance

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
