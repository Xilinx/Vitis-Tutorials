<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

## Design Tutorials

The AI Engine Development Design Tutorials showcase the two major phases of AI Engine application development: architecting the application and developing the kernels. Both these phases are demonstrated in these tutorials.

The <a href="../README.md">**README**</a> of AI Engine Development contains important information including tool version, environment settings, and a table describing the platform, operating system, and supported features or flows of each tutorial. It is strongly recommended that you review details before starting to use the AIE tutorials.

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
<tr>
 <td align="center"><a href="../../Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System/">Versal Custom Thin Platform Extensible System</a></td>
 <td>This is a Versal system example design based on a VCK190 thin custom platform (Minimal clocks and AXI exposed to PL) that includes HLS/RTL kernels and AI Engine kernel using a full Makefile build-flow.</td>
 </tr>
 <tr>
 <td align="center"><a href="./01-aie_lenet_tutorial/">LeNet Tutorial</a></td>
 <td>This tutorial uses the LeNet algorithm to implement a system-level design to perform image classification using the AI Engine and PL logic, including block RAM (BRAM). The design demonstrates functional partitioning between the AI Engine and PL. It also highlights memory partitioning and hierarchy among DDR memory, PL (BRAM) and AI Engine memory.</td>
 </tr>
 <tr>
 <td align="center"><a href="./02-super_sampling_rate_fir/">Super Sampling Rate FIR Filters</a></td>
 <td>The purpose of this tutorial is to provide a methodology to enable you to make appropriate choices depending on the filter characteristics, and to provide examples on how to implement Super Sampling Rate (SSR) FIR Filters on a Versal® ACAP AI Engine processor array.</td>
 </tr>
 <tr>
 <td align="center"><a href="./03-beamforming/">Beamforming Design</a></td>
 <td>This tutorial demonstrates the creation of a beamforming system running on the AI Engine, PL, and PS, and the validation of the design running on this heterogeneous domain.</td>
 </tr> 
 <td align="center"><a href="./04-Polyphase-Channelizer/">Polyphase Channelizer</a></td>
 <td>This tutorial demonstrates an implementation of a system-level design (ie. Polyphase Channelizer) using a combination of AI Engine and PL/HLS kernels.</td>
 </tr> 
 </tr> 
 <td align="center"><a href="./05-Prime-Factor-FFT/">Prime Factor FFT-1008</a></td>
 <td>This Versal system example implements a 1008-pt FFT using the Prime Factor Algorithm. The design uses both AI Engine and PL kernels working cooperatively. AI Engine elements are hand-coded using AIE API. PL elements are implemented using Vitis HLS. System integration in Vitis is managed using the new v++ Unified Command Line flow. </td>
 </tr> 
 <tr>
 <td align="center"><a href="./06-fft2d_AIEvsHLS/">2D-FFT</a></td>
 <td>This tutorial performs two implementations of a system-level design (2D-FFT): one with AI Engine, and the other with HLS using the DSP Engines. </td>
 </tr>
 <tr>
 <td align="center"><a href="./07-firFilter_AIEvsHLS/">FIR Filter</a></td>
 <td>This tutorial demonstrates the implementations of a system-level design (FIR Filter) using AI Engines and HLS with DSP Engines in the Versal device plus PL including LUTs, flip-flops (FFs), and block RAMs.</td>
 </tr> 
 <tr>
 <td align="center"><a href="./08-n-body-simulator/">N-Body Simulator</a></td>
 <td>It is a system-level design that uses the AI Engine, PL, and PS resources to showcase the following features:<li>A Python model of an N-Body Simulator run on x86 machine
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
 <tr>
 <td align="center"><a href="./09-ddc_chain/">Digital Down-conversion Chain</a></td>
 <td>This tutorial demonstrates the steps to upgrade a 32-branch digital down-conversion chain (XAPP1351) to the latest recommended tools and coding practice, including conversion of most AI Engine Intrinsics to APIs. The upgraded AIE API version achieves the same throughput performance as the original code base, while being easier to read and maintain.</td>
 </tr>
 <tr>
 <td align="center"><a href="./10-GeMM_AIEvsDSP/">Versal GeMM Implementation</a></td>
 <td>This tutorial performs two implementations of a system-level design: one with AI Engine, and the other with RTL using the DSP Engines. In each implementation, the tutorial takes you through the hardware emulation and hardware flow in the context of a complete Versal ACAP system design.</td>
 </tr>
 <tr>
 <td align="center"><a href="./11-Bilinear_Interpolation/">Bilinear Interpolation</a></td>
 <td>This tutorial demonstrates how the bilinear interpolation algorithm may be efficiently implemented using AI Engines. It also provides guidance for customizing the design to function with varying image resolutions, and to take advantage of multicore processing on the AI Engine array to achieve desired throughput.</td>
 </tr>
 </table>



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
