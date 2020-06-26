<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ - Algorithm Acceleration</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

This section is divided in 5 modules (each module is located under the <code>./docs</code> directory and comes with its own readme with step-by-step instructions).
* [**Module 1**](../docs/module1_baseline): Setting up the design and establish a performance **baseline**
  + Understand the OpenCL APIs that help connect host program to the kernel
  + Verify results through emulation both at the software level (sw_emu) and the hardware level (hw_emu)
  + Evaluate the performance by visualizing the timeline trace with Vitis Analyzer
  + Launch Vitis HLS to review the kernel optimizations
* [**Module 2**](../docs/module2_pipeline): This version of the code explicitely applies the **PIPELINE** directive.
  + Learn about the pragma and its impact on designs
* [**Module 3**](../docs/module3_dependency_removal): Analyze and solve false dependencies to improve the kernel
  + Understand the changes applied in the code to circumvent the throughput bottleneck.
  + Apply the **DEPENDENCE** pragma
  + Rerun the kernel and observe the results through Vitis Analyzer
* [**Module 4**](../docs/module4_flatten_loop): Merge and flatten critical loop to implement more parallelism
  + Reuse the results from module 3 and flatten a critical loop to gain more parallelism.
  + Rerun and observe the impact on resources, latency and throughput
* [**Module 5**](../docs/module5_dataflow): Add the task parallelism pragma to further improve the kernel
  + Reuse the results from module 4 to re-arrange the code to enable the task parallelism optimization pragma called **DATAFLOW**.
  + Evaluate the performance improvement with Vitis Analyzer
  + Create the binary (xclbin) to program the card and measure the actual performance
