<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

## Introduction

The tutorials under the AI Engine Development help you learn how to target, develop, and deploy advanced algorithms using a Versal AI Engine array in conjunction with PL IP/kernels and software applications running on the embedded processors. In order to successfully deploy AI Engine applications in hardware, you need to be aware of the Vitis and AI Engine tools and flows.

- The AI Engine Development <a href="./Feature_Tutorials/">**Feature Tutorials**</a> highlight specific features and flows that help develop AI Engine applications.

- The AI Engine Development <a href="./Design_Tutorials/">**Design Tutorials**</a> showcase the two major phases of AI Engine application development: architecting the application and developing the kernels. Both these phases are demonstrated in these tutorials.

## Getting Started

### AI Engine Documentation
To easily find the right documentation corresponding to the development stage you are at, we recommend you use the <a href="https://docs.xilinx.com/p/ai-engine-development">AI Engine Design Process Hub </a>.
The major documentation for AI Engine includes:
- <a href="https://docs.xilinx.com/r/en-US/am009-versal-ai-engine">AM009 - Versal ACAP AI Engine Architecture Manual</a>
- <a href="https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment">UG1076 - AI Engine Tools and Flows</a>
- <a href="https://docs.xilinx.com/r/en-US/ug1079-ai-engine-kernel-coding">UG1079 - AI Engine Kernel and Graph Programming Guide</a>

### AI Engine Training
If you are new with AI Engine, the following training courses can help you understand the architechure and design flow.
- <a href="https://xilinxprod-catalog.netexam.com/Search?searchText=Designing+with+Versal+AI+Engine+1">Designing with Versal AI Engine 1: Architecture and Design Flow	</a>
- <a href="https://xilinxprod-catalog.netexam.com/Search?searchText=Designing+with+Versal+AI+Engine+2">Designing with Versal AI Engine 2: Graph Programming with AI Engine Kernels </a>
- <a href="https://xilinxprod-catalog.netexam.com/Search?searchText=Designing+with+Versal+AI+Engine+3">Designing with Versal AI Engine 3: Kernel Programming and Optimization </a>

### Environment Settings

**IMPORTANT**: Before beginning the tutorial make sure you have read and followed the *[Vitis Software Platform Release Notes](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Vitis-Software-Platform-Release-Notes)* (v2023.1) for setting up software and installing the VCK190 base platform.

Run the following steps to setup environment ( **NOT** apply to tutorials that do not use the VCK190 base platform ):

1. Set up your platform by running the `xilinx-versal-common-v2023.1/environment-setup-cortexa72-cortexa53-xilinx-linux` script as provided in the platform download. This script sets up the `SYSROOT` and `CXX` variables. If the script is not present, you **must** run the `xilinx-versal-common-v2023.1/sdk.sh`.
2. Set up your `ROOTFS` to point to the `xilinx-versal-common-v2023.1/rootfs.ext4`.
3. Set up your `IMAGE` to point to `xilinx-versal-common-v2023.1/Image`.
4. Set up your `PLATFORM_REPO_PATHS` environment variable based upon where you downloaded the platform.

### Getting Started with AI Engine Development Using the AI Engine Tutorials
![missing image](AI_Engine_dev_flow.jpg)

If you are new with the AI Engine Architecture and tools, we recommend that you start with the  <a href="./Feature_Tutorials/01-aie_a_to_z/">A to Z Bare-metal Flow</a> which will guide you through the entire flow from platform creation in vivado to AI Engine application creation, system integration and testing on Hardware using the Vitis IDE.

#### AI Engine Application Development
To get started with AI Engine application development, we recommend that you look at the following tutorials:
- <a href="./Feature_Tutorials/08-dsp-library/">DSP Library Tutorial</a> which will guide you to create an AI Engine using application using the AMD provided DSP library
- <a href="./Feature_Tutorials/10-aie-dsp-lib-model-composer/">AIE DSPLib and Model Composer</a> which will also guide you to create an AI Engine using application using the AMD provided DSP library but using the ModelComposer tool which enables easy designs through a MATLAB Simulink environment.
- <a href="./Feature_Tutorials/02-using-gmio/">Using GMIO with AIE</a> which goes through using the GMIOs for connectivity between the AI Engine and the DDR (through the NoC)
- <a href="./Feature_Tutorials/14-implementing-iir-filter/">Implementing an IIR Filter on the AIE</a> will guide you into custom kernel coding using a IIR application

The following tutorials are describing some features of the AI Engine which can be useful for your application:
- <a href="./Feature_Tutorials/03-rtp-reconfiguration/">Runtime Parameter Reconfiguration</a>
- <a href="./Feature_Tutorials/04-packet-switching/">Packet Switching</a>
- <a href="./Feature_Tutorials/07-AI-Engine-Floating-Point/">Using Floating-Point in the AIE</a>

#### AI Engine Application Debug and Optimization
Once you have written your first AI Engine application you might want to verify the correct functionality of your graphs and kernels using x86 simulation and AI Engine simulation. In this regards, the following tutorials will be useful for you:
- <a href="./Feature_Tutorials/09-debug-walkthrough/">Debug Walkthrough Tutorial</a> will guide to analyze the performance of your AI Engine application and help you with debugging functional issues.

- <a href="./Feature_Tutorials/13-aie-performance-analysis/">AIE Performance and Deadlock Analysis</a> which introduces you to performance analysis and optimization methods, and shows you how synchronization works in graph execution.
- <a href="./Feature_Tutorials/16-external-traffic-generator-aie/">Python and C++ External Traffic Generators for AI Engine Simulation and Emulation Flows</a>: This tutorial demonstrates how to create external traffic generators as Python scripts or C++ applications to exercise the AI Engine kernels in the x86 simulator, AI Engine simulator, and in hardware emulation.

#### System Integration
When you AI Engine Application meets your expectation in terms of functionality and performances it will be the right time to integrate it with the rest of the Versal System. At this stage, the following tutorials will be useful for you:
- <a href="./Feature_Tutorials/05-AI-engine-versal-integration/">AIE Versal Integration</a>. This tutorial demonstrates creating a system design running on the AI Engine, PS, and Programmable Logic (PL).
- <a href="./Feature_Tutorials/06-versal-system-design-clocking-tutorial/">Versal System Design Clocking</a>. In this tutorial, you will learn clocking concepts for the Vitis compiler and how to define clocking for an ADF Graph, as well as PL kernels using clocking automation functionality.
- <a href="./Feature_Tutorials/11-ai-engine-emulation-waveform-analysis/">Versal Emulation Waveform Analysis</a> which demonstrates how you can use the AMD Vivado™ Design Suite logic simulator (XSIM) waveform GUI, and the Vitis analyzer to debug and analyze your design.


## Available Tutorials
### Feature Tutorials
These tutorials target the **VCK190** board. The table below lists the tutorials available, and the features and flows showcased in all of these tutorials. The various columns correspond to specific features/flows supported in these tutorials and will help you identify tutorials that showcase specific flows and features that you are interested in.

 <table style="width:100%">
 <tr>
 <td width="7%" align="center"><b>Tutorial</b>
 <td width="7%" align="center"><b>Platform</b>
 <td width="7%" align="center"><b>OS</b>
 <td width="7%" align="center"><b>IDE Flow</b>
 <td width="7%" align="center"><b>Libraries Used</b>
 <td width="7%" align="center"><b>HLS Kernel</b>
 <td width="7%" align="center"><b>x86 simulator</b>
 <td width="7%" align="center"><b>aie simulator</b>
 <td width="7%" align="center"><b>SW Emu</b>
 <td width="7%" align="center"><b>HW Emu</b>
 <td width="7%" align="center"><b>HW</b>
 <td width="7%" align="center"><b>Event Trace in HW</b>
 <td width="7%" align="center"><b>Profile in HW</b>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/18-aie_a_to_z_custom_linux_platform/">AI Engine A-to-Z Flow for Linux</a></td>
 <td>Base / Custom</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/01-aie_a_to_z/">A to Z Bare-metal Flow</a></td>
 <td>Custom</td>
 <td>Baremetal</td>
 <td>Vivado &<br>Vitis IDE</td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/02-using-gmio/">Using GMIO with AIE</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/03-rtp-reconfiguration/">Runtime Parameter Reconfiguration</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/04-packet-switching/">Packet Switching</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/05-AI-engine-versal-integration/">AIE Versal Integration</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td>CLI / Vitis Unified IDE</td>
 <td></td>
 <td>MM2S / S2MM</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/06-versal-system-design-clocking-tutorial/">Versal System Design Clocking</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/07-AI-Engine-Floating-Point/">Using Floating-Point in the AIE</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/08-dsp-library/">DSP Library Tutorial</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td>DSPLib</td>
 <td>MM2S / S2MM Variant</td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/09-debug-walkthrough/">Debug Walkthrough Tutorial</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td>Vitis IDE</td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/10-aie-dsp-lib-model-composer/">AIE DSPLib and Model Composer</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td>Simulink</td>
 <td>DSPLib</td>
 <td>MM2S / S2MM</td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/11-ai-engine-emulation-waveform-analysis/">Versal Emulation Waveform Analysis</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>Traffic Generators</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/12-axis-traffic-generator/">AXIS External Traffic Generator</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td>DSPLib</td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/13-aie-performance-analysis/">AIE Performance and Deadlock Analysis</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/14-implementing-iir-filter/">Implementing an IIR Filter on the AIE</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td>Vitis IDE</td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td></td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/15-post-link-recompile/">Post-Link Recompile of an AIE Application </a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> MM2S / S2MM </td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
  <td align="center"><a href="./Feature_Tutorials/16-external-traffic-generator-aie/">Python and C++ External Traffic Generators for AI Engine Simulation and Emulation Flows</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> MM2S / S2MM / PolarClip </td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/17-RTL-IP-with-AIE-Engines/">Using RTL IP with AI Engines</a></td>
 <td>Custom</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/19-aie_external_io_sv/">Using Verilog Traffic Generators in AIE Simulation</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td>Vivado</td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
 </table>

### Design Tutorials
These tutorials target the **VCK190** board. The table below lists the tutorials available, and the features and flows showcased in all of these tutorials. The various columns correspond to specific features/flows supported in these tutorials and will help you identify tutorials that showcase specific flows and features that you are interested in.

 <table style="width:100%">
 <tr>
 <td width="7%" align="center"><b>Tutorial</b>
 <td width="7%" align="center"><b>Platform</b>
 <td width="7%" align="center"><b>OS</b>
 <td width="7%" align="center"><b>IDE Flow</b>
 <td width="7%" align="center"><b>Libraries Used</b>
 <td width="7%" align="center"><b>HLS Kernel</b>
 <td width="7%" align="center"><b>x86 simulator</b>
 <td width="7%" align="center"><b>aie simulator</b>
 <td width="7%" align="center"><b>SW Emu</b>
 <td width="7%" align="center"><b>HW Emu</b>
 <td width="7%" align="center"><b>HW</b>
 <td width="7%" align="center"><b>Event Trace in HW</b>
 <td width="7%" align="center"><b>Profile in HW</b>
 </tr>
 <tr>
 <td align="center"><a href="../Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System/">Versal Custom Thin Platform Extensible System</a></td>
 <td>Custom</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM / VADD</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/01-aie_lenet_tutorial/">LeNet Tutorial</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>Yes</td>
 <td></td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/02-super_sampling_rate_fir/">Super Sampling Rate FIR Filters</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/03-beamforming/">Beamforming Design</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/04-Polyphase-Channelizer/">Polyphase Channelizer</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/05-Prime-Factor-FFT/">Prime Factor FFT</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/06-fft2d_AIEvsHLS/">2D-FFT</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td>DSPLib</td>
 <td>PL Data Generator and Checker</td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/07-firFilter_AIEvsHLS/">FIR Filter</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td>DSPLib</td>
 <td>PL Data Generator and Checker</td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/08-n-body-simulator/">N-Body Simulator</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>PL Datamover</td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/09-ddc_chain/">Digital Down-conversion Chain</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr> 
 <tr>
 <td align="center"><a href="./Design_Tutorials/10-GeMM_AIEvsDSP/">Versal GeMM Implementation</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td>DSPLib</td>
 <td>Datamover</td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/11-Bilinear_Interpolation/">Bilinear Interpolation</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td></td>
 </tr>
 </table>



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
