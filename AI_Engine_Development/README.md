<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis™ - AI Engine Development Tutorials</h1>
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
 <td align="center"><a href="./Design_Tutorials/01-aie_lenet_tutorial/README.md">LeNet Tutorial</a></td>
 <td>This tutorial uses the LeNet algorithm to implement a system-level design to perform image classification using the AI Engine and PL logic, including block RAM (BRAM). The design demonstrates functional partitioning between the AI Engine and PL. It also highlights memory partitioning and hierarchy among DDR memory, PL (BRAM) and AI Engine memory.</td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/02-super_sampling_rate_fir/README.md">Super Sampling Rate FIR Filters</a></td>
 <td>The purpose of this tutorial is to provide a methodology to enable you to make appropriate choices depending on the filter characteristics, and to provide examples on how to implement Super Sampling Rate (SSR) FIR Filters on a Versal® ACAP AI Engine processor array.</td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/03-beamforming/README.md">Beamforming Design</a></td>
 <td>This tutorial demonstrates the creation of a beamforming system running on the AI Engine, PL, and PS, and the validation of the design running on this heterogeneous domain.</td>
 </tr>

 <tr>
 <td align="center"><a href="./Design_Tutorials/04-custom-platform-emulation/README.md">AIE Emulation on Custom Platforms</a></td>
 <td>This tutorial demonstrates the creation and emulation of an AIE design including the Adaptive DataFlow (ADF) graph, RTL kernels, and a custom VCK190 platform.</td>
 </tr>

 <tr>
 <td align="center"><a href="./Design_Tutorials/06-fft2d_AIEvsHLS/README.md">2D-FFT</a></td>
 <td>This tutorial performs two implementations of a system-level design (2D-FFT): one with AI Engine, and the other with HLS using the DSP Engines. </td>
 </tr>

  <tr>
 <td align="center"><a href="./Design_Tutorials/07-firFilter_AIEvsHLS/README.md">FIR Filter</a></td>
 <td>This tutorial demonstrates the implementations of a system-level design (FIR Filter) using AI Engines and HLS with DSP Engines in the Versal device plus PL including LUTs, flip-flops (FFs), and block RAMs.</td>
 </tr>

  <tr>
 <td align="center"><a href="./Design_Tutorials/08-n-body-simulator/README.md">N-Body Simulator</a></td>
 <td>It is a system-level design that uses the AI Engine, PL, and PS resources to showcase the following features:

   <li>A Python model of an N-Body Simulator run on x86 machine
   <li>A scalable AI Engine design that can utilize up to 400 AI Engine tiles
   <li>AI Engine packet switching
   <li>AI Engine single-precision floating point calculations
   <li>AI Engine 1:400 broadcast streams
   <li>Codeless PL HLS datamover kernels from the Vitis™ Utility Library
   <li>PL HLS packet switching kernels
   <li>PS Host Application that validates the data coming out of the AI Engine design
   <li>C++ model of an N-Body Simulator
   <li>Performance comparisons between Python x86, C++ Arm A72, and AI Engine N-Body Simulators
   <li>Effective throughput calculation (GFLOPS) vs. Theoretical peak throughput of AI Engine
 </tr>


 </table>



### Feature Tutorials

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/01-aie_a_to_z/">A to Z Bare-metal Flow</a></td>
 <td>This tutorial introduces a complete end to end flow for a bare-metal host application using AI Engines and PL kernels.</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/02-using-gmio/">Using GMIO with AIE</a></td>
 <td>This tutorial introduces the usage of global memory I/O (GMIO) for sharing data between the AI Engines and external DDR</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/03-rtp-reconfiguration/">Runtime Parameter Reconfiguration</a></td>
 <td>Learn how to dynamically update AI Engine runtime parameters</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/04-packet-switching/">Packet Switching</a></td>
 <td>This tutorial illustrates how to use data packet switching with AI Engine designs to optimize efficiency.</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/05-AI-engine-versal-integration/">AI Engine Versal Integration for Hardware Emulation and Hardware</a></td>
 <td>This tutorial demonstrates creating a system design running on the AI Engine, PS, and PL and validating the design running on these heterogeneous domains by running Hardware Emulation.</td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/06-versal-system-design-clocking-tutorial/">Versal System Design Clocking</a></td>
 <td>This tutorial demonstrates clocking concepts for the Vitis compiler by defining clocking for ADF graph PL kernels and PLIO kernels, using the clocking automation functionality.</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/07-AI-Engine-Floating-Point/">Using Floating-Point in the AI Engine</a></td>
 <td>These examples demonstrate floating-point vector computations in the AI Engine.</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/08-dsp-library/">DSP Library Tutorial</a></td>
 <td>This tutorial demonstrates how to use kernels provided by the DSP library for a filtering application, how to analyze the design results, and how to use filter parameters to optimize the design's performance using simulation.</td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/09-debug-walkthrough/">Debug Walkthrough Tutorial</a></td>
 <td>
This tutorial demonstrates how to debug a multi-processor application using the Versal ACAP AI Engines, using a beamformer example design. The tutorial illustrates functional debug and performance level debug techniques.</td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/10-aie-dsp-lib-model-composer/">AI Engine DSP Library and Model Composer Tutorial</a></td>
 <td>
This tutorial shows how to design AI Engine applications using Model Composer. This set of blocksets for Simulink is used to demonstrate how easy it is to develop applications for Xilinx devices, integrating RTL/HLS blocks for the Programmable Logic, as well as AI Engine blocks for the AI Engine array.</td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/11-ai-engine-emulation-waveform-analysis/">Versal Emulation Waveform Analysis</a></td>
 <td>
This tutorial demonstrates how you can use the Vivado logic simulator (XSIM) waveform GUI, and the Vitis analyzer to debug and analyze your design for a Versal ACAP.</td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/12-axis-traffic-generator/">AXIS External Traffic Generator</a></td>
 <td>
This tutorial shows how to use AXI Traffic Generators to provide input and capture output from an AI Engine kernel in hardware emulation.</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/13-aie-performance-analysis/">AI Engine Performance and Deadlock Analysis Tutorial</a></td>
 <td>
This tutorial introduces you to performance analysis and optimization methods, and shows you how synchronization works in graph execution. It also demonstrates the analysis of a hang issue using an example. </td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/14-implementing-iir-filter/">Implementing an IIR Filter on the AI Engine</a></td>
 <td>
This multi-part tutorial describes how to implement an [infinite impulse response (IIR) filter](https://en.wikipedia.org/wiki/Infinite_impulse_response) on the AI Engine.</td>
 </tr>
 </table>

<p align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>
