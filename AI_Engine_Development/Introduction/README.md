<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

***Version: Vitis 2022.1***

## Introduction

The tutorials under the AI Engine Development category help you learn how to target, develop, and deploy advanced algorithms using a Versal AI Engine array in conjunction with PL IP/kernels and software applications running on the embedded processors. In order to successfully deploy AI Engine applications in hardware, you need to be aware of the Vitis and AI Engine tools and flows. 

- The AI Engine <a href="../Feature_Tutorials/">**Feature Tutorials**</a> highlight specific features and flows that help develop AI Engine applications.

- The AI Engine <a href="../Design_Tutorials/">**Design Tutorials**</a> showcase the two major phases of AI Engine application development: architecting the application and developing the kernels. Both these phases are demonstrated in these tutorials.

These tutorials target the **VCK190** board. The table below lists the tutorials available, and the features and flows showcased in all of these tutorials. The various columns correspond to specific features/flows supported in these tutorials and will help you identify tutorials that showcase specific flows and features that you are interested in.

## Environment Settings

**IMPORTANT**: Before beginning the tutorial make sure you have read and followed the *Vitis Software Platform Release Notes* (v2022.1) for setting up software and installing the VCK190 base platform.

Run the following steps to setup environment ( **NOT** apply to tutorials that do not use the VCK190 base platform ):

1. Set up your platform by running the `xilinx-versal-common-v2022.1/environment-setup-cortexa72-cortexa53-xilinx-linux` script as provided in the platform download. This script sets up the `SYSROOT` and `CXX` variables. If the script is not present, you **must** run the xilinx-versal-common-v2022.1/sdk.sh.
2. Set up your `ROOTFS` to point to the `xilinx-versal-common-v2022.1/rootfs.ext4`
3. Set up your `IMAGE` to point to `xilinx-versal-common-v2022.1/Image`.
4. Set up your `PLATFORM_REPO_PATHS` environment variable based upon where you downloaded the platform.

### Feature Tutorials

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
 <td align="center"><a href="../Feature_Tutorials/02-using-gmio/">Using GMIO with AIE</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 </tr>
  <tr>
 <td align="center"><a href="../Feature_Tutorials/03-rtp-reconfiguration/">Runtime Parameter Reconfiguration</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 <td>x</td>
 <td> </td>
 <td> </td>
 </tr>
  <tr>
 <td align="center"><a href="../Feature_Tutorials/04-packet-switching/">Packet Switching</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 <td>x</td>
 <td> </td>
 <td> </td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/05-AI-engine-versal-integration/">AIE Versal Integration</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td>x</td>
 <td>x</td>
 <td>x</td>
 <td>x</td>
 <td>x</td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="../Feature_Tutorials/06-versal-system-design-clocking-tutorial/">Versal System Design Clocking</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 <td>x</td>
 <td> </td>
 <td> </td>
 </tr>
  <tr>
 <td align="center"><a href="../Feature_Tutorials/07-AI-Engine-Floating-Point/">Using Floating-Point in the AIE</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
  <tr>
 <td align="center"><a href="../Feature_Tutorials/08-dsp-library/">DSP Library Tutorial</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td>DSPLib</td>
 <td>MM2S / S2MM Variant</td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="../Feature_Tutorials/09-debug-walkthrough/">Debug Walkthrough Tutorial</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td>x</td>
 <td> </td>
 <td> </td>
 <td>x</td>
 <td>x</td>
 <td>x</td>
 <td>x</td>
 <td>x</td>
 <td>x</td>
 <td>x</td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/10-aie-dsp-lib-model-composer/">AIE DSPLib and Model Composer</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td>x</td>
 <td>DSPLib</td>
 <td>MM2S / S2MM</td>
 <td>x</td>
 <td>x</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="../Feature_Tutorials/11-ai-engine-emulation-waveform-analysis/">Versal Emulation Waveform Analysis</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>Traffic Generators</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="../Feature_Tutorials/12-axis-traffic-generator/">AXIS External Traffic Generator</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td>DSPLib</td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
  <tr>
 <td align="center"><a href="../Feature_Tutorials/13-aie-performance-analysis/">AIE Performance and Deadlock Analysis</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 </tr>
  <tr>
 <td align="center"><a href="../Feature_Tutorials/14-implementing-iir-filter/">Implementing an IIR Filter on the AIE</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td>x</td>
 <td> </td>
 <td> </td>
 <td>x</td>
 <td>x</td>
 <td>x</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
  <tr>
 <td align="center"><a href="../Feature_Tutorials/01-aie_a_to_z/">A to Z Bare-metal Flow</a></td>
 <td>Custom</td>
 <td>Baremetal</td>
 <td>x</td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 <td>x</td>
 <td> </td>
 <td> </td>
 </tr>
  <tr>
 <td align="center"><a href="../Feature_Tutorials/15-post-link-recompile/">Post-Link Recompile of an AIE Application </a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 <td>x</td>
 <td> </td>
 <td> </td>
 </tr>
  <td align="center"><a href="../Feature_Tutorials/16-external-traffic-generator-aie/">Python and C++ External Traffic Generators for AI Engine Simulation and Emulation Flows</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S/S2MM/PolarClip</td>
 <td>x</td>
 <td>x</td>
 <td>x</td>
 <td>x</td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
 </table>

### Design Tutorials

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
 <td align="center"><a href="../Design_Tutorials/01-aie_lenet_tutorial/">LeNet Tutorial</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>x</td>
 <td></td>
 <td>x</td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 </tr>
 <tr>
 <td align="center"><a href="../Design_Tutorials/02-super_sampling_rate_fir/">Super Sampling Rate FIR Filters</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="../Design_Tutorials/03-beamforming/">Beamforming Design</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 </tr> <tr>
 <td align="center"><a href="../Design_Tutorials/04-custom-platform-emulation/">AIE Emulation on Custom Platforms</a></td>
 <td>Custom</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr><tr>
 <td align="center"><a href="../Design_Tutorials/06-fft2d_AIEvsHLS/">2D-FFT</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td>DSPLib</td>
 <td>PL Data Generator and Checker</td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 </tr><tr>
 <td align="center"><a href="../Design_Tutorials/07-firFilter_AIEvsHLS/">FIR Filter</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td>DSPLib</td>
 <td>PL Data Generator and Checker</td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 </tr> <tr>
 <td align="center"><a href="../Design_Tutorials/08-n-body-simulator/">N-Body Simulator</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>PL Datamover</td>
 <td> </td>
 <td>x</td>
 <td> </td>
 <td>x</td>
 <td>x</td>
 <td> </td>
 <td> </td>
 </tr>
 </table>


 
 
 <p align="center"><sup>Copyright&copy; 2022 Xilinx</sup></p>