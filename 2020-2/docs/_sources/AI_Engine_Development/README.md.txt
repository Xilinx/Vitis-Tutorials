<table class="sphinxhide">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis™ - AI Engine Development Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# AI Engine Development

### Design Tutorials

The methodology for developing optimized accelerated applications is comprised of two major phases: architecting the application, and developing the kernels. In the first phase, you make key decisions about the application architecture by determining which software functions should be accelerated onto ACAP kernels, how much parallelism can be achieved, and how to deliver it in code. In the second phase, you implement the kernels by structuring the source code, and applying the necessary build options s to create the kernel architecture needed to achieve the optimized performance target. The following examples illustrate the use of this methodology in real-world applications.



 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/01-aie_lenet_tutorial/README.html">LeNet Tutorial</a></td>
 <td>This tutorial uses the LeNet algorithm to implement a system-level design to perform image classification using the AI Engine and PL logic, including block RAM (BRAM). The design demonstrates functional partitioning between the AI Engine and PL. It also highlights memory partitioning and hierarchy among DDR memory, PL (BRAM) and AI Engine memory.</td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/02-super_sampling_rate_fir/README.html">Super Sampling Rate FIR Filters</a></td>
 <td>The purpose of this tutorial is to provide a methodology to enable you to make appropriate choices depending on the filter characteristics, and to provide examples on how to implement Super Sampling Rate (SSR) FIR Filters on a Versal ACAP AI Engine processor array.</td>
 </tr>
 </table>




### Feature Tutorials

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/01-aie_a_to_z/README.html">A to Z Bare-metal Flow</a></td>
 <td>This tutorial introduces a complete end to end flow for a bare-metal host application using AI Engines and PL kernels.</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/02-using-gmio/README.html">Using GMIO with AIE</a></td>
 <td>This tutorial introduces the usage of global memory I/O (GMIO) for sharing data between the AI Engines and external DDR</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/03-rtp-reconfiguration/README.html">Runtime Parameter Reconfiguration</a></td>
 <td>Learn how to dynamically update AI Engine runtime parameters</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/04-packet-switching/README.html">Packet Switching</a></td>
 <td>This tutorial illustrates how to use data packet switching with AI Engine designs to optimize efficiency.</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/05-AI-engine-versal-integration/README.html">AI Engine Versal Integration for Hardware Emulation and Hardware</a></td>
 <td>This tutorial demonstrates creating a system design running on the AI Engine, PS, and PL and validating the design running on these heterogeneous domains by running Hardware Emulation.</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/06-versal-system-design-clocking-tutorial/README.html">Versal System Design Clocking</a></td>
 <td>In this tutorial you will learn clocking concepts for the Vitis compiler via how to define clocking for ADF Graph PL kernels, PLIO kernels by using clocking automation functionality..</td>
 </tr>
 </table>

<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>