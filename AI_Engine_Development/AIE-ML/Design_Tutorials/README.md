<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AIE-ML Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See AMD Vitis™ Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

## Design Tutorials

The AI Engine Development Design Tutorials showcase the two major phases of AI Engine-ML application development: architecting the application and developing the kernels. Both phases are demonstrated in these tutorials.

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
<tr>
 <td align="center"><a href="../../../Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System/">Versal Custom Thin Platform Extensible System</a></td>
 <td>This is a Versal system example design based on a VEK280 thin custom platform (Minimal clocks and AXI exposed to PL) that includes HLS/RTL kernels and AI Engine kernel using a full Makefile build-flow.</td>
 </tr>
 <tr>
 <td align="center"><a href="./01-AIE-ML-programming-and-optimization/">AIE-ML Programming</a></td>
 <td>This tutorial helps user understand the differences between AI Engine and AI Engine-ML architecture, it is based on matrix multiplication which is a usual algorithm in Machine Learning applications. </td>
 </tr>
 <tr>
 <td align="center"><a href="./02-Prime-Factor-FFT/">Prime Factor FFT-1008 on AIE-ML</a></td>
 <td>This Versal system example implements a 1008-pt FFT using the Prime Factor algorithm. The design uses both AI Engine and PL kernels working cooperatively. AI Engine elements are hand-coded using AIE API, PL elements are implemented using Vitis HLS. System integration in Vitis is managed using the new v++ Unified Command Line flow. This tutorial targets the AIE-ML architecture. </td>
 </tr>
 <tr>
 <td align="center"><a href="./03-AIE-ML-lenet_tutorial/">AIE-ML LeNet Tutorial</a></td>
 <td>This tutorial uses the LeNet algorithm to implement a system-level design to perform image classification using the AIE-ML architecture and PL logic, including block RAM (BRAM). The design demonstrates functional partitioning between the AIE-ML and PL. It also highlights memory partitioning and hierarchy among DDR memory, PL (BRAM), Memory tile and AI Engine memory. </td>
 </tr>
 <tr>
 <td align="center"><a href="./04-AIE-API-based-FFT-for-many-instances-applications/">AIE API based FFT for Many Instances Applications</a></td>
 <td>This tutorial walks the user through the design and the implementation of an FFT for many parallel signals on a Real-Time system, using the AI Engine APIs. The design performance objective is minimizing power and utilization, maintaining a high throughput to at least match the Real-Time acquisition bandwidth. Moreover, the design leverages the AIE-ML Memory Tiles to minimize programmable logic utilization. The considered case study comprises 128 parallel signals, each with a 125MSa/s sample rate and CINT16 datatype, with a total aggregated bandwidth of 64GBytes/s.
 </td>
 </tr>
</table>


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2024 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
