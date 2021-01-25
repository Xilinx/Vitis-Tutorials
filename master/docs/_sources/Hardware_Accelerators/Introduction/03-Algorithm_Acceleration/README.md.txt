<table class="sphinxhide">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ - Algorithm Acceleration</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

> **_In this section..._**
_1 Introductory reference module in which we run the CPU version of the algorithm in <code>./cpu_src</code>_
_4 Alveo U50 modules, located under the <code>./docs</code> directory_
_Instructions in local readme files for each module_

* [**Introduction — CPU Run**](./docs/cpu_src/README.md): The C++ implementation of the algorithm
  + Run a C++ non-accelerated version of Cholesky algorithm
* [**Module 1**](./docs/module1_baseline/README.md): Setting up the design and establish a performance **baseline**
  + Understand the host OpenCL APIs that help connect to the kernel implemented onto the Xilinx device
  + Verify results through emulation both at the software level (sw_emu) and the hardware level (hw_emu)
  + Evaluate the performance by visualizing the timeline trace with Vitis Analyzer
  + Launch Vitis HLS to review the kernel optimizations
* [**Module 2**](./docs/module2_pipeline/README.md): This version of the code explicitely applies the <code>**PIPELINE**</code> and <code>**INTERFACE**</code> directive
  + Learn about these pragmas and their impact on designs
* [**Module 3**](./docs/module3_datatype/README.md): Change <code>double</code> data types to <code>float</code>
  + Run hardware emulation and then Vitis Analyzer and Vitis HLS
  + Measure the impact on physical resources required to implement the design and performance
* [**Module 4**](./docs/module4_dataflow/README.md): Back to using <code>double</code> , the task parallelism pragma is applied to improve results
  + Re-arrange code to enable the task parallelism optimization <code>**DATAFLOW**</code> pragma
  + Evaluate the performance improvement with Vitis Analyzer
  + Use Vitis HLS to confirm the new micro-architecture created by dataflow
  + Generate the binary (xclbin) to program the card and measure the actual performance

<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>
