<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>


# Design Tutorials

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
</table>



<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
