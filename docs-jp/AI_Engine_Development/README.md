﻿<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis™ - AI Engine Development Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>


### Design Tutorials

The methodology for developing optimized accelerated applications is comprised of two major phases: architecting the application, and developing the kernels. In the first phase, you make key decisions about the application architecture by determining which software functions should be accelerated onto ACAP kernels, how much parallelism can be achieved, and how to deliver it in code. In the second phase, you implement the kernels by structuring the source code, and applying the necessary build options s to create the kernel architecture needed to achieve the optimized performance target. The following examples illustrate the use of this methodology in real-world applications.



 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="../../AI_Engine_Development/Design_Tutorials/01-aie_lenet_tutorial/README.md">LeNet Tutorial</a></td>
 <td>This tutorial uses the LeNet algorithm to implement a system-level design to perform image classification using the AI Engine and PL logic, including block RAM (BRAM). The design demonstrates functional partitioning between the AI Engine and PL. It also highlights memory partitioning and hierarchy among DDR memory, PL (BRAM) and AI Engine memory.</td>
 </tr>
 <tr>
 <td align="center"><a href="../../AI_Engine_Development/Design_Tutorials/02-super_sampling_rate_fir/README.md">Super Sampling Rate FIR Filters</a></td>
 <td>The purpose of this tutorial is to provide a methodology to enable you to make appropriate choices depending on the filter characteristics, and to provide examples on how to implement Super Sampling Rate (SSR) FIR Filters on a Versal ACAP AI Engine processor array.</td>
 </tr>
  <tr>
 <td align="center"><a href="../../AI_Engine_Development/Design_Tutorials/03-beamforming/README.md">Beamforming Design</a></td>
 <td>This tutorial demonstrates the creation of a beamforming system running on the AI Engine, PL, and PS, and the validation of the design running on this heterogeneous domain.</td>
 </tr>
  <tr>
 <td align="center"><a href="../../AI_Engine_Development/Design_Tutorials/04-custom-platform-emulation/README.md">AIE Emulation on Custom Platforms</a></td>
 <td>This tutorial demonstrates the creation and emulation of an AIE design including the Adaptive DataFlow (ADF) graph, RTL kernels, and a custom VCK190 platform.</td>
 </tr>
 </table>



### Feature Tutorials

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="../../AI_Engine_Development/Feature_Tutorials/01-aie_a_to_z/README.md">A to Z Bare-metal Flow</a></td>
 <td>This tutorial introduces a complete end to end flow for a bare-metal host application using AI Engines and PL kernels.</td>
 </tr>
  <tr>
 <td align="center"><a href="../../AI_Engine_Development/Feature_Tutorials/02-using-gmio/README.md">Using GMIO with AIE</a></td>
 <td>This tutorial introduces the usage of global memory I/O (GMIO) for sharing data between the AI Engines and external DDR</td>
 </tr>
  <tr>
 <td align="center"><a href="../../AI_Engine_Development/Feature_Tutorials/03-rtp-reconfiguration/README.md">Runtime Parameter Reconfiguration</a></td>
 <td>Learn how to dynamically update AI Engine runtime parameters</td>
 </tr>
  <tr>
 <td align="center"><a href="../../AI_Engine_Development/Feature_Tutorials/04-packet-switching/README.md">Packet Switching</a></td>
 <td>This tutorial illustrates how to use data packet switching with AI Engine designs to optimize efficiency.</td>
 </tr>
  <tr>
 <td align="center"><a href="../../AI_Engine_Development/Feature_Tutorials/05-AI-engine-versal-integration/README.md">AI Engine Versal Integration for Hardware Emulation and Hardware</a></td>
 <td>This tutorial demonstrates creating a system design running on the AI Engine, PS, and PL and validating the design running on these heterogeneous domains by running Hardware Emulation.</td>
 </tr>
 <tr>
 <td align="center"><a href="../../AI_Engine_Development/Feature_Tutorials/06-versal-system-design-clocking-tutorial/README.md">Versal System Design Clocking</a></td>
 <td>This tutorial demonstrates clocking concepts for the Vitis compiler by defining clocking for ADF graph PL kernels and PLIO kernels, using the clocking automation functionality.</td>
 </tr>
  <tr>
 <td align="center"><a href="../../AI_Engine_Development/Feature_Tutorials/07-AI-Engine-Floating-Point/README.md">Using Floating-Point in the AI Engine</a></td>
 <td>These examples demonstrate floating-point vector computations in the AI Engine.</td>
 </tr>
  <tr>
 <td align="center"><a href="../../AI_Engine_Development/Feature_Tutorials/08-dsp-library/README.md">DSP Library Tutorial</a></td>
 <td>This tutorial demonstrates how to use kernels provided by the DSP library for a filtering application, how to analyze the design results, and how to use filter parameters to optimize the design's performance using simulation.</td>
 </tr>
 </table>

<p align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>
