<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

## Feature Tutorials
The AI Engine Development Feature Tutorials highlight specific features and flows that help develop AI Engine applications.

The <a href="../README.md">**README**</a> of AI Engine Development contains important information including tool version, environment settings, and a table describing the platform, operating system, and supported features or flows of each tutorial. It is strongly recommended that you review details before starting to use the AIE tutorials.

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="./18-aie_a_to_z_custom_linux_platform/">AI Engine A-to-Z Flow for Linux</a></td>
 <td>This tutorial intoduces a platform-based approach to develop the adaptable subsystem that contains PL kernels and AI Engine graph, demonstrates how you can quickly validate the design by means of hardware emulation or hardware using the base platform, and quickly switch to the custom platform with minimal changes.</td>
 </tr>
 <tr>
 <td align="center"><a href="./01-aie_a_to_z/">A to Z Bare-metal Flow</a></td>
 <td>This tutorial walks through the steps to create a custom Baremetal platform, and also integrate Baremetal host application along with  an AI Engines graph and PL kernels.</td>
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
 <td>This tutorial illustrates how to use data packet switching with AI Engine designs to optimize efficiency.</td>
 </tr>
  <tr>
 <td align="center"><a href="./05-AI-engine-versal-integration/">AI Engine Versal Integration for Hardware Emulation and Hardware</a></td>
 <td>This tutorial demonstrates creating a system design running on the AI Engine, PS, and PL and validating the design running on these heterogeneous domains by running Hardware Emulation.</td>
 </tr>
 <tr>
 <td align="center"><a href="./06-versal-system-design-clocking-tutorial/">Versal System Design Clocking</a></td>
 <td>This tutorial demonstrates clocking concepts for the Vitis compiler by defining clocking for ADF graph PL kernels and PLIO kernels, using the clocking automation functionality.</td>
 </tr>
  <tr>
 <td align="center"><a href="./07-AI-Engine-Floating-Point/">Using Floating-Point in the AI Engine</a></td>
 <td>These examples demonstrate floating-point vector computations in the AI Engine.</td>
 </tr>
  <tr>
 <td align="center"><a href="./08-dsp-library/">DSP Library Tutorial</a></td>
 <td>This tutorial demonstrates how to use kernels provided by the DSP library for a filtering application, how to analyze the design results, and how to use filter parameters to optimize the design's performance using simulation.</td>
 </tr>
 <tr>
 <td align="center"><a href="./09-debug-walkthrough/">Debug Walkthrough Tutorial</a></td>
 <td>
This tutorial demonstrates how to debug a multi-processor application using the Versal ACAP AI Engines, using a beamformer example design. The tutorial illustrates functional debug and performance level debug techniques.</td>
 </tr>
 <tr>
 <td align="center"><a href="./10-aie-dsp-lib-model-composer/">AI Engine DSP Library and Model Composer Tutorial</a></td>
 <td>
This tutorial shows how to design AI Engine applications using Model Composer. This set of blocksets for Simulink is used to demonstrate how easy it is to develop applications for Xilinx devices, integrating RTL/HLS blocks for the Programmable Logic, as well as AI Engine blocks for the AI Engine array.</td>
 </tr>
 <tr>
 <td align="center"><a href="./11-ai-engine-emulation-waveform-analysis/">Versal Emulation Waveform Analysis</a></td>
 <td>
This tutorial demonstrates how you can use the Vivado logic simulator (XSIM) waveform GUI, and the Vitis analyzer to debug and analyze your design for a Versal ACAP.</td>
 </tr>
 <tr>
 <td align="center"><a href="./12-axis-traffic-generator/">AXIS External Traffic Generator</a></td>
 <td>
This tutorial shows how to use AXI Traffic Generators to provide input and capture output from an AI Engine kernel in hardware emulation.</td>
 </tr>
  <tr>
 <td align="center"><a href="./13-aie-performance-analysis/">AI Engine Performance and Deadlock Analysis Tutorial</a></td>
 <td>
This tutorial introduces you to performance analysis and optimization methods, and shows you how synchronization works in graph execution. It also demonstrates the analysis of a hang issue using an example. </td>
 </tr>
  <tr>
 <td align="center"><a href="./14-implementing-iir-filter/">Implementing an IIR Filter on the AI Engine</a></td>
 <td>
This multi-part tutorial describes how to implement an [infinite impulse response (IIR) filter](https://en.wikipedia.org/wiki/Infinite_impulse_response) on the AI Engine.</td>
 </tr>
 <tr>
 <td align="center"><a href="./15-post-link-recompile/">Post-Link Recompile of an AI Engine Application </a></td>
 <td>This tutorial shows you how to modify an AI Engine application after the platform has been frozen. It avoids a complete Vivado® tool run, which can take a long time if timing closure requires specific attention. The only limitation is that the hardware connection between the AI Engine array and the programmable logic (PL) must be kept fixed. The tutorial demonstrates a Vitis IDE flow and a Makefile flow.</td>
 </tr>
  <tr>
 <td align="center"><a href="./16-external-traffic-generator-aie/">Python and C++ External Traffic Generators for AI Engine Simulation and Emulation Flows</a></td>
 <td>This tutorial develops a case in which the dataflow goes back and forth multiple times between the programmable logic (PL) and the AI Engine array. Some PL blocks are only source or destination kernels, whereas others are processing kernels within the dataflow. This tutorial demonstrates how to create external traffic generators as Python scripts or C++ applications to exercise the AI Engine kernels in the x86 simulator, AI Engine simulator, and in hardware emulation.</td>
 </tr>
 <tr>
 <td align="center"><a href="./17-RTL-IP-with-AIE-Engines/">Using RTL IP with AI Engines</a></td>
 <td>This tutorial demonstrates how to reuse any AXI-based IP you have created as an RTL IP, control your platform, and convert your RTL IP to an RTL kernel allowing for a more streamlined process for creating the design you need.</td>
 </tr>
 <tr>
 <td align="center"><a href="./19-aie_external_io_sv/">Using Verilog Traffic Generators in AIE Simulation</a></td>
 <td>This tutorial demonstrates how to use Verilog or System Verilog modules to drive traffic in and out of an ADF graph running in the AIE Simulator.</td>
 </tr>
 </table>



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
