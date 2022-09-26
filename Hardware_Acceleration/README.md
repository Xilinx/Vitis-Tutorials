<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Hardware Acceleration Tutorials

## Introduction to Vitis Hardware Acceleration

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="./Introduction/README.md">Introduction to Vitis Hardware Acceleration</a></td>
 <td>This tutorial provides you with an easy-to-follow, guided introduction to accelerating applications with Xilinx technology. We will begin from the first principles of acceleration: understanding the fundamental architectural approaches, identifying suitable code for acceleration, and interacting with the software APIs for managing memory and interacting with the target device in an optimal way.</td>
 </tr>
 </table>


## Design Tutorials

The methodology for developing optimized accelerated applications is comprised of two major phases: architecting the application, and developing the hardware kernels. In the first phase, you make key decisions about the application architecture by determining which software functions should be accelerated onto FPGA kernels, how much parallelism can be achieved, and how to deliver it in code. In the second phase, you implement the kernels by structuring the source code, and applying the necessary compiler options and pragmas to create the kernel architecture needed to achieve the optimized performance target. The following examples illustrate the use of this methodology in real-world applications.



<table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/02-bloom/README.md">Bloom Filter Example</a></td>
 <td>This tutorial shows how to achieve a 10x speed-up on a data analytics application using a combination of kernel and host code optimization techniques.</td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/01-convolution-tutorial/README.md">Convolution Example</a></td>
 <td>This tutorial walks through the process of analyzing and optimizing a 2D convolution used for real-time processing of a video stream.</td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/03-rtl_stream_kernel_integration/README.md">RTL Systems Integration Example</a></td>
 <td>This tutorial demonstrates how to integrate free-running RTL kernels, Vitis Library functions, and custom Vitis HLS kernels into a real system.</td>
 </tr>
<tr>
 <td align="center"><a href="./Design_Tutorials/04-traveling-salesperson/README.md">Traveling Salesperson Problem</a></td>
 <td>This tutorial demonstrates the full flow to implement a HLS kernel from algorithm model to hardware.</td>
</tr>
<tr>
 <td align="center"><a href="./Design_Tutorials/05-bottom_up_rtl_kernel/README.md">Bottom RTL Kernel Design Flow Example</a></td>
 <td>This tutorial demonstrates how to develope a complex RTL kernel from scratch via batch mode without GUI environment.</td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/06-cholesky-accel/">Choleskey Algorithm Acceleration </a></td>
 <td>This tutorial puts in practice the concepts of FPGA acceleration and illustrates how to gradually optimize a hardware accelerator implementing the Cholesky matrix decomposition algorithm.</td>
 </tr>
  <tr>
 <td align="center"><a href="./Design_Tutorials/07-host-code-opt/">XRT Host Code Optimization</a></td>
 <td>This tutorial demonstrates how to optimize your CPU host code to get the most out of interaction between your hardware accelerators and your runtime software.</td>
 </tr>
  <td align="center"><a href="./Design_Tutorials/08-alveo_aurora_kernel/">Aurora Kernel on Alveo</a></td>
 <td>This tutorial demonstrates how to integrate Aurora IP in user design on Alveo card with Vitis flow to realize high-speed inter-card communications.</td>
 </tr>
  </tr>
  <td align="center"><a href="./Design_Tutorials/09-sssp-application/">Single Source Shortest Path Application</a></td>
 <td>This tutorial demonstrates how to design an application of finding shortest path with Vitis Graph library on Alveo U50.</td>
 </tr>
</table>




## Feature Tutorials

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/01-rtl_kernel_workflow/README.md">Getting Started with RTL Kernels</a></td>
 <td>This tutorial demonstrates how to use the Vitis core development kit to program an RTL kernel into an FPGA and build a Hardware Emulation using a common development flow.</td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/02-mixing-c-rtl-kernels/README.md">Mixing C and RTL</a></td>
 <td>This tutorial demonstrates how to work with an application containing RTL and C kernels, along with various design analysis features.</td>
 </tr>
<tr>
 <td align="center"><a href="./Feature_Tutorials/03-dataflow_debug_and_optimization/README.md">Dataflow Debug and Optimization</a></td>
 <td>This tutorial demonstrates how to debug and optimize the dataflow optimization in Vitis HLS.</td>
 </tr>
  <td align="center"><a href="./Feature_Tutorials/04-mult-ddr-banks/README.md">Using Multiple DDR Banks</a></td>
 <td>This tutorial demonstrates how using multiple DDRs can improve data transfer between kernels and global memory.</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/05-using-multiple-cu/README.md">Using Multiple Compute Units</a></td>
 <td>This tutorial demonstrates the flexible kernel linking process to increase the number of kernel instances on an FPGA, which improves the parallelism in a combined host-kernel system.</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/06-controlling-vivado-implementation/">Controlling Vivado Implementation</a></td>
 <td>This tutorial demonstrates how you can control the Vivado&reg; tools flow when implementing your project.</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/07-using-hbm/">Optimizing for HBM</a></td>
 <td>This tutorial demonstrates how you can take best advantage of HBM on platforms that support it.</td>
 </tr>
 <tr>
<td align="center"><a href="./Feature_Tutorials/08-using-hostmem/">Host Memory Access</a></td>
<td>This tutorial demonstrates how kernels can directly access buffers host memory directly. This capability requires a compatible platform.</td>
</tr>
 <tr>
<td align="center"><a href="./Feature_Tutorials/09-using-ethernet-on-alveo/">Using GT Kernels and Ethernet IPs on Alveo</a></td>
<td>This tutorial demonstrates how to use networking GT kernels with generated Ethernet IPs and implement them on Alveo card with Vitis flow.</td>
</tr>
 <tr>
<td align="center"><a href="./Feature_Tutorials/10-p2p-card-to-card/">Enabling FPGA to FPGA P2P Transfer using Native XRT C++ API</a></td>
<td>This tutorial demonstrates how to enable p2p transfer from one FPGA device to another using XRT API host code.</td>
</tr>
 </table>

<p align="center"><sup>Copyright&copy; 2020-2022 Xilinx</sup></p>
