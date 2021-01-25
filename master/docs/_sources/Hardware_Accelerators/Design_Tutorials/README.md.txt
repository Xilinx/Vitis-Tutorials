<table class="sphinxhide">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ - Hardware Accelerators Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>


### Introduction to Vitis Hardware Accelerators

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="../Introduction/README.html">Introduction to Vitis Hardware Accelerators</a></td>
 <td>This tutorial puts in practice the concepts of FPGA acceleration and illustrates how to gradually optimize a hardware accelerator implementing the Cholesky matrix decomposition algorithm.</td>
 </tr>
 </table>


### Design Tutorials

The methodology for developing optimized accelerated applications is comprised of two major phases: architecting the application, and developing the hardware kernels. In the first phase, you make key decisions about the application architecture by determining which software functions should be accelerated onto FPGA kernels, how much parallelism can be achieved, and how to deliver it in code. In the second phase, you implement the kernels by structuring the source code, and applying the necessary compiler options and pragmas to create the kernel architecture needed to achieve the optimized performance target. The following examples illustrate the use of this methodology in real-world applications.



 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="./02-bloom/README.html">Bloom Filter Example</a></td>
 <td>This tutorial shows how to achieve a 10x speed-up on a data analytics application using a combination of kernel and host code optimization techniques.</td>
 </tr>
 <tr>
 <td align="center"><a href="./01-convolution-tutorial/README.html">Convolution Example</a></td>
 <td>This tutorial walks through the process of analyzing and optimizing a 2D convolution used for real-time processing of a video stream.</td>
 </tr>
 <tr>
 <td align="center"><a href="./03-rtl_stream_kernel_integration/README.html">RTL Systems Integration Example</a></td>
 <td>This tutorial demonstrates how to integrate free-running RTL kernels, Vitis Library functions, and custom Vitis HLS kernels into a real system.</td>
 </tr>
 </table>



### Feature Tutorials

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="../Feature_Tutorials/01-rtl_kernel_workflow/README.html">Getting Started with RTL Kernels</a></td>
 <td>This tutorial demonstrates how to use the Vitis core development kit to program an RTL kernel into an FPGA and build a Hardware Emulation using a common development flow.</td>
 </tr>
 <tr>
 <td align="center"><a href="../Feature_Tutorials/02-mixing-c-rtl-kernels/README.html">Mixing C and RTL</a></td>
 <td>This tutorial demonstrates how to work with an application containing RTL and C kernels, along with various design analysis features.</td>
 </tr>
 </table>

<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>
