<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ - Algorithm Acceleration</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

> **_In this section..._**<br>
_5 modules, located under the <code>./docs</code> directory_<br>
_Instructions in local readme files for each module_

* [**Module 1**](../docs/module1_baseline): Setting up the design and establish a performance **baseline**
  + Understand the OpenCL APIs that help connect host program to the kernel
  + Verify results through emulation both at the software level (sw_emu) and the hardware level (hw_emu)
  + Evaluate the performance by visualizing the timeline trace with Vitis Analyzer
  + Launch Vitis HLS to review the kernel optimizations
* [**Module 2**](../docs/module2_pipeline): This version of the code explicitely applies the **PIPELINE** directive.
  + Learn about the pragma and its impact on designs
* [**Module 3**](../docs/module3_localram): Direct host transfers to the Programmable Logic RAM (PLRAM) for more efficient transfers
  + Modify the configuration file to use PLRAM
  + Rerun the hardware emulation and use Vitis Analyzer to review the results and verify the new connection
* [**Module 4**](../docs/module4_datatype): Change "double" to "float"
  + Run hardware emulation and then Vitis Analyzer and Vitis HLS
  + Measure the impact on physical resources required to implement the design and performance 
* [**Module 5**](../docs/module5_dataflow): The task parallelism pragma is applied to improve results
  + Re-arrange code to enable the task parallelism optimization pragma of Vitis HLS called **DATAFLOW**.
  + Evaluate the performance improvement with Vitis Analyzer
  + Use Vitis HLS to confirm the new micor-architecture created by dataflow
  + Generate the binary (xclbin) to program the card and measure the actual performance
