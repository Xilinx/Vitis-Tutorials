<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

***Version: Vitis 2023.1***

## Introduction

The tutorials under the Hardware Acceleration category help you learn how to use the Vitis core development kit to build, analyze, and optimize an accelerated algorithm developed in C++, OpenCL, and even low-level hardware description languages (HDLs) like Verilog and VHDL. You may also learn how to optimize the CPU side of your application for efficient memory allocation, how to sequence system-level events, and more. 

- The Hardware Acceleration <a href="./Feature_Tutorials/">**Feature Tutorials**</a> illustrate specific features or flows of Vitis, some features may not be required by all designs but are still useful for some use cases.

- The Hardware Acceleration <a href="./Design_Tutorials/">**Design Tutorials**</a> illustrate higher-level concepts or design flows, walk through specific examples or reference designs, and more complex and complete designs or applications.

These tutorials target different boards including Alveo™ Data Center acceleration cards or MPSoC Evaluation Boards like ZCU104. The table below lists the tutorials available, and the features and flows showcased in all of these tutorials. The various columns correspond to specific features/flows supported in these tutorials and will help you identify tutorials that showcase specific flows and features that you are interested in.

## Feature Tutorials

 <table style="width:100%">
 <tr>
 <td width="14%" align="center"><b>Tutorial</b>
 <td width="14%" align="center"><b>Board / Platform</b>
 <td width="14%" align="center"><b>Kernel</b>
 <td width="14%" align="center"><b>XRT APIs</b>
 <td width="14%" align="center"><b>Libraries Used</b>
 <td width="14%" align="center"><b>Highlighted Features</b>
 <td width="14%" align="center"><b>GUI Flow</b>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/01-rtl_kernel_workflow/">Getting Started with RTL Kernels</a></td>
 <td>U200</td>
 <td>C/C++</td>
 <td>Native</td>
 <td> </td>
 <td>RTL kernel</td>
 <td>Vivado<br>Vitis IDE</td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/02-mixing-c-rtl-kernels/">Mixing C and RTL</a></td>
 <td>U200</td>
 <td>RTL + C/C++</td>
 <td>OpenCL</td>
 <td> </td>
 <td>Mixed C++ and RTL kernel</td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/03-dataflow_debug_and_optimization/">Dataflow Debug and Optimization</a></td>
 <td>HLS Part</td>
 <td>C/C++</td>
 <td>HLS</td>
 <td> </td>
 <td>HLS design and analysis</td>
 <td>Vitis HLS<br>XSim</td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/04-mult-ddr-banks/">Using Multiple DDR Banks</a></td>
 <td>U200</td>
 <td>C/C++</td>
 <td>Native</td>
 <td> </td>
 <td>DDR bank assignment</td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/05-using-multiple-cu/">Using Multiple Compute Units</a></td>
 <td>U200</td>
 <td>C/C++</td>
 <td>OpenCL</td>
 <td> </td>
 <td>Multiple Compute Units</td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/06-controlling-vivado-implementation/">Controlling Vivado Implementation</a></td>
 <td>U200</td>
 <td>C/C++</td>
 <td>OpenCL</td>
 <td> </td>
 <td>Use Vivado in the Vitis flow</td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/07-using-hbm/">Optimizing for HBM</a></td>
 <td>U200 / U50</td>
 <td>C/C++</td>
 <td>OpenCL</td>
 <td> </td>
 <td>HBM and RAMA IP</td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/08-using-hostmem/">Host Memory Access</a></td>
 <td>U250</td>
 <td>C/C++</td>
 <td>OpenCL</td>
 <td> </td>
 <td>Use Host Memory</td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/09-using-ethernet-on-alveo/">Using GT Kernels and Ethernet IPs on Alveo</a></td>
 <td>U200</td>
 <td>RTL</td>
 <td>Native</td>
 <td> </td>
 <td>Ethernet Subsystem</td>
 <td> </td>
 </tr>
<tr>
 <td align="center"><a href="./Feature_Tutorials/10-p2p-card-to-card/">Enabling FPGA to FPGA P2P Transfer</a></td>
 <td>U50 / U200</td>
 <td>C/C++</td>
 <td>Native</td>
 <td> </td>
 <td>PCIe P2P</td>
 <td> </td>
 </tr>
 </table>

## Design Tutorials

 <table style="width:100%">
 <tr>
 <td width="14%" align="center"><b>Tutorial</b>
 <td width="14%" align="center"><b>Board / Platform</b>
 <td width="14%" align="center"><b>Kernel</b>
 <td width="14%" align="center"><b>XRT APIs</b>
 <td width="14%" align="center"><b>Libraries Used</b>
 <td width="14%" align="center"><b>Highlighted Features</b>
 <td width="14%" align="center"><b>GUI Flow</b>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/01-convolution-tutorial/">Convolution Example</a></td>
 <td>U200</td>
 <td>C/C++</td>
 <td>OpenCL</td>
 <td>Vision</td>
 <td>Performance analysis and optimization</td>
 <td>Vitis HLS</td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials//02-bloom/">Bloom Filter Example</a></td>
 <td>U200</td>
 <td>C/C++</td>
 <td>OpenCL</td>
 <td> </td>
 <td>Performance analysis and optimization</td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/03-rtl_stream_kernel_integration/">RTL Systems Integration Example</a></td>
 <td>U50 / 55C / U200 / U250 / U280</td>
 <td>RTL + C/C++</td>
 <td>Native</td>
 <td>Vision</td>
 <td>Mixed kernel</td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/04-traveling-salesperson/">Traveling Salesperson Problem</a></td>
 <td>U200</td>
 <td>C/C++</td>
 <td>HLS</td>
 <td> </td>
 <td>HLS design and analysis</td>
 <td>Vitis HLS</td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/05-bottom_up_rtl_kernel/">Bottom RTL Kernel Design Flow Example</a></td>
 <td>U50 / 55C / U200 / U250 / U280</td>
 <td>RTL + C/C++</td>
 <td>Native</td>
 <td> </td>
 <td>RTL kernel</td>
 <td></td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/06-cholesky-accel/">Choleskey Algorithm Acceleration</a></td>
 <td>U200</td>
 <td>C/C++</td>
 <td>OpenCL</td>
 <td> </td>
 <td>Performance analysis and optimization</td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/07-host-code-opt/">XRT Host Code Optimization</a></td>
 <td>U200</td>
 <td>C/C++</td>
 <td>OpenCL</td>
 <td> </td>
 <td>Host code optimization</td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/08-alveo_aurora_kernel/">Aurora Kernel on Alveo</a></td>
 <td>U50 / 55C / U200 / U250 / U280</td>
 <td>RTL + C/C++</td>
 <td>Native</td>
 <td> </td>
 <td>GT kernel</td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/09-sssp-application/">Single Source Shortest Path Application</a></td>
 <td>U50</td>
 <td>C/C++</td>
 <td>OpenCL</td>
 <td>Graph</td>
 <td>Vitis Library</td>
 <td>Vitis IDE</td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/10-get_moving_with_alveo/">Get Moving with Alveo</a></td>
 <td>U200</td>
 <td>C/C++</td>
 <td>OpenCL</td>
 <td>Vision</td>
 <td>System Optimization</td>
 <td> </td>
 </tr>
 </table>


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
