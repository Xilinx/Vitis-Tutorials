<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">See Vitis™ AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# Feature Tutorials

The AI Engine Development Feature Tutorials highlight specific features and flows that help develop AI Engine applications.

The [**README**](../README.md) of AI Engine Development contains important information including tool version, environment settings. It also has a table that describes the platform, operating system, and supported features or flows of each tutorial. Review the details before starting to use the AIE tutorials.

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="./18-aie_a_to_z_custom_linux_platform/">AI Engine A-to-Z Flow for Linux</a></td>
 <td>This tutorial introduces a platform-based approach to develop an adaptable subsystem that contains PL kernels and AI Engine graph. It demonstrates how you can validate the design using hardware emulation or hardware using the base platform, and switch to the custom platform with minimal changes.</td>
 </tr>
 <tr>
 <td align="center"><a href="./01-aie_a_to_z/">A to Z Bare-metal Flow</a></td>
 <td>This tutorial walks through the steps to create a custom bare-metal platform. It also integrates a bare-metal host application along with an AI Engines graph and PL kernels.</td>
 </tr>
 <tr>
 <td align="center"><a href="./02-using-gmio/">Using GMIO with AIE</a></td>
 <td>This tutorial introduces the usage of global memory I/O (GMIO) for sharing data between the AI Engines and external DDR</td>
 </tr>
  <tr>
 <td align="center"><a href="./03-rtp-reconfiguration/">Runtime Parameter Reconfiguration</a></td>
 <td>Learn how to dynamically update AI Engine runtime parameters</td>
 </tr>
  <tr>
 <td align="center"><a href="./04-packet-switching/">Packet Switching</a></td>
 <td>This tutorial shows how to use data packet switching with AI Engine designs to optimize efficiency.</td>
 </tr>
  <tr>
 <td align="center"><a href="./05-AI-engine-versal-integration/">AI Engine Versal™ Adaptive SoC Integration for Hardware Emulation and Hardware</a></td>
 <td>This tutorial demonstrates creating a system design running on the AI Engine, PS, and PL and validating the design running on these heterogeneous domains by running Hardware Emulation.</td>
 </tr>
 <tr>
 <td align="center"><a href="./06-versal-system-design-clocking-tutorial/">Versal Adaptive SoC System Design Clocking</a></td>
 <td>This tutorial demonstrates clocking concepts for the Vitis compiler by defining clocking for Adaptive Data Flow (ADF) graph PL kernels and PLIO kernels. It uses the clocking automation functionality.</td>
 </tr>
  <tr>
 <td align="center"><a href="./07-AI-Engine-Floating-Point/">Using Floating-Point in the AI Engine</a></td>
 <td>These examples demonstrate floating-point vector computations in the AI Engine.</td>
 </tr>
  <tr>
 <td align="center"><a href="./08-dsp-library/">DSP Library Tutorial</a></td>
 <td>This tutorial demonstrates how to use kernels provided by the DSP library for a filtering application and how to analyze the design results. It also shows how to use filter parameters to optimize the design's performance using simulation.</td>
 </tr>
 <tr>
 <td align="center"><a href="./09-debug-walkthrough/">Debug Walk-through Tutorial</a></td>
 <td>
This tutorial demonstrates how to debug a multi-processor application using the Versal adaptive SoC AI Engines, using a beamformer example design. The tutorial shows functional debug and performance level debug techniques.</td>
 </tr>
 <tr>
 <td align="center"><a href="./10-aie-dsp-lib-model-composer/">AI Engine DSP Library and Model Composer Tutorial</a></td>
 <td>
This tutorial shows how to design AI Engine applications using the Model Composer. This set of block sets for Simulink demonstrates how easy it is to develop applications for AMD devices, integrating RTL/HLS blocks for the Programmable Logic and AI Engine blocks for the AI Engine array.</td>
 </tr>
 <tr>
 <td align="center"><a href="./11-ai-engine-emulation-waveform-analysis/">Versal Adaptive SoC Emulation Waveform Analysis</a></td>
 <td>
This tutorial demonstrates how to use the Vivado® logic simulator (XSIM) waveform GUI and the Vitis analyzer to debug and analyze your design for a Versal adaptive SoC.</td>
 </tr>
 <tr>
 <td align="center"><a href="./13-aie-performance-analysis/">AI Engine Performance and Deadlock Analysis Tutorial</a></td>
 <td>
This tutorial introduces you to performance analysis and optimization methods. It shows you how synchronization works in graph execution and demonstrates the analysis of a hang issue using an example. </td>
 </tr>
 <tr>
 <td align="center"><a href="./14-implementing-iir-filter/">Implementing an IIR Filter on the AI Engine</a></td>
 <td>
This multi-part tutorial describes how to implement an [infinite impulse response (IIR) filter](https://en.wikipedia.org/wiki/Infinite_impulse_response) on the AI Engine.</td>
 </tr>
 <tr>
 <td align="center"><a href="./15-post-link-recompile/">Post-Link Recompile of an AI Engine Application </a></td>
 <td>This tutorial shows you how to modify an AI Engine application after you freeze the platform. It avoids a complete Vivado® tool run, which can take a long time if timing closure requires specific attention. The only limitation is that the hardware connection between the AI Engine array and the programmable logic (PL) must remain fixed. The tutorial demonstrates a Vitis IDE flow and a Makefile flow.</td>
 </tr>
 <tr>
 <td align="center"><a href="./17-RTL-IP-with-AIE-Engines/">Using RTL IP with AI Engines</a></td>
 <td>This tutorial demonstrates how to reuse any AXI-based IP you have created as an RTL IP. It shows how to control your platform and convert your RTL IP to an RTL kernel, allowing for a more streamlined design process.</td>
 </tr>
 <tr>
 <td align="center"><a href="./20-aiecompiler-features/">AIE Compiler Features</a></td>
 <td>This tutorial shares a variety of features useful for AI Engine / AI Engine-ML (AIE-ML) programming. These features help create more visible and efficient code compared to early versions of the compiler.</td>
 </tr>
 <tr>
 <td align="center"><a href="./21-two_tone_filter/">Two Tone Filter on AIE Using DSP libraries and Vitis Model Composer </a></td>
 <td>This tutorial demonstrates how to implement the same MATLAB® model design using the Vitis DSP libraries targeting AI Engine. The MATLAB model design has a two-tone input signal. The Finite Impulse Response (FIR) filter suppresses one tone from the two-tone input signal. The output of the FIR filter connects to the FFT block, which acts as a monitor to display a spectrum plot. This tutorial has four parts: part 1 uses a 400 MSPS sampling rate, part 2 uses 2000 MSPS, part 3 implements the part 1 design using Vitis IDE, and part 4 implements the part 1 design using Vitis Model Composer tool.</td>
 </tr> 
 <tr>
 <td align="center"><a href="./23-AIE_independent_graphs/">Compiling AIE Graphs for Independent Partitions</a></td>
 <td>This tutorial demonstrates the flow for compiling AI Engine Graphs for AI Engine partitions. The AI Engine graphs exist in different partitions of the device, verified by the AIE simulator independently, but integrated and packaged by v++ linker and v++ packager together. The flow is suitable for multiple teams working simultaneously in different parts of a system project. It also supports integrating user-owned design with vendor (for example, AMD) provided IP cores.</td>
 </tr>
 <tr>
 <td align="center"><a href="./24-aie_pl_interface/">RTL / AI Engine interfacing Examples</a></td>
 <td>This tutorial shows ways of interfacing custom RTL logic to the AI Engine using the Vitis acceleration flow. </td>
 </tr>
 <tr>
 <td align="center"><a href="./25-AIE-kernel-optimization/">AIE Kernel Optimization</a></td>
 <td>This tutorial teaches how to diagnose and improve compute efficiency of algorithms implemented as AI Engine kernels by analyzing the generated microcode. It presents fundamentals of interpreting microcode and provides two example labs to encourage hands-on experience with optimizing AI Engine kernel performance. </td>
 </tr>
 <tr>
 <td align="center"><a href="../../AIE-ML/Feature_Tutorials/06-gemm/">Matrix Compute with Vitis Libraries</a></td>
 <td>In this tutorial, we explore how to use matrix multiplication/General Matrix Multiply (GEMM) from the DSP Vitis library. We examine various design requirements and configure the parameters accordingly. Finally, we migrate the design to the AIE-ML architecture and compare its performance with AIE architecture.</td>
 </tr>
 <tr>
 <td align="center"><a href="./26-gentle_intro_to_aie_kernel_programming/">A Gentle Introduction to AI Engine Kernel Programming</a></td>
 <td>In this tutorial, we guide you on how to get data into and out of a kernel using a simple contrived example.</td>
 </tr>
 <tr>
 <td align="center"><a href="./27-system-timeline/">System Timeline Tutorial</a></td>
 <td>This tutorial demonstrates how to use System Timeline, a new feature that traces all subsystems of the device (PL, PS and AI Engine array). It displays them in Vitis Analyzer on the same graph with a synchronized timeline. </td>
 </tr>   
 </table>



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
