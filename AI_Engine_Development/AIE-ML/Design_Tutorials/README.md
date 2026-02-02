<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See AMD Vitis™ Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">See AMD Vitis™ AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# Design Tutorials

The AI Engine Development Design Tutorials demonstrate the two major phases of AI Engine-ML application development: architecting the application and developing the kernels. Both phases are demonstrated in these tutorials.

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
<tr>
 <td align="center"><a href="../../../Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System/">Versal Custom Thin Platform Extensible System</a></td>
 <td>This is an AMD Versal™ system example design based on a VEK280 thin custom platform (Minimal clocks and AXI exposed to PL) that includes HLS/RTL kernels and AI Engine kernel using a full Makefile build-flow.</td>
 </tr>
 <tr>
 <td align="center"><a href="./01-AIE-ML-programming-and-optimization/">AIE-ML Programming</a></td>
 <td>This tutorial helps you understand the differences between AI Engine and AI Engine-ML architecture, based on the matrix multiplication which is a usual algorithm in Machine Learning applications. </td>
 </tr>
 <tr>
 <td align="center"><a href="./02-Prime-Factor-FFT/">Prime Factor FFT-1008 on AIE-ML</a></td>
 <td>This Versal system example implements a 1008-pt FFT using the Prime Factor algorithm. The design uses both AI Engine and PL kernels working cooperatively. Developers hand-code AI Engine elements using the AIE API, and they implement PL elements using Vitis HLS.. The new v++ Unified Command Line flow manages system integration in Vitis. This tutorial targets the AIE-ML architecture. </td>
 </tr>
 <tr>
 <td align="center"><a href="./03-AIE-ML-lenet_tutorial/">AIE-ML LeNet Tutorial</a></td>
 <td>This tutorial uses the LeNet algorithm to implement a system-level design to perform image classification using the AIE-ML architecture and PL logic, including block RAM (BRAM). The design demonstrates functional partitioning between the AIE-ML and PL. It also highlights memory partitioning and hierarchy among DDR memory, PL (BRAM), Memory tile and AI Engine memory. </td>
 </tr>
 <tr>
 <td align="center"><a href="./04-AIE-API-based-FFT-for-many-instances-applications/">AIE API based FFT for Many Instances Applications</a></td>
 <td>This tutorial walks you through the design and the implementation of an FFT for many parallel signals on a Real-Time system, using the AI Engine APIs. The design performance objective is minimizing power and utilization, maintaining a high throughput to at least match the Real-Time acquisition bandwidth. Moreover, the design leverages the AIE-ML Memory Tiles to minimize programmable logic utilization. The considered case study comprises 128 parallel signals, each with a 125 MSa/s sample rate and CINT16 datatype, with a total aggregated bandwidth of 64 GBytes/s. </td>
 </tr>
 <tr>
 <td align="center"><a href="./05-Softmax-Function/">Softmax Function on AIE-ML</a></td>
 <td>Softmax is an activation function often used in the output layer of a neural network designed for multi-class classification. This tutorial illustrates how to implement the softmax function for developing custom machine learning inference applications on AI Engines. </td>
 </tr> 
 <tr>
 <td align="center"><a href="./06-farrow_filter/">Migrating Farrow Filter from AIE to AIE-ML</a></td>
 <td>Many applications, including digital receivers in modems, commonly use a fractional delay filter, a digital signal processing (DSP) algorithm. This filter is essential for timing synchronization. The AIE architecture already includes the implementation of the Fractional Delay Farrow Filter design. This tutorial guides how to migrate the existing Fractional Delay Farrow Filter design from AIE to AIE-ML architecture. </td>
 </tr>
 <tr>
 <td align="center"><a href="./07-Channelizer-Using-Vitis-Libraries/">Polyphase Channelizer on AIE-ML using Vitis Libraries</a></td>
 <td>This tutorial demonstrates how to leverage Vitis Libraries IP blocks to build a high performance Polyphase Channelizer on AIE-ML running at 2GSPS. </td>
 </tr> 
 <tr>
 <td align="center"><a href="./08-MNIST-ConvNet/">MNIST ConvNet on AIE-ML</a></td>
 <td>This tutorial implements a Convolutional Neural Network classifier on AIE-ML for identifying hand-written digits from the MNIST database. The goal is to illustrate how to partition and vectorize a simple machine learning example to Versal AI Engines.</td>
 </tr>  
</table>


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
