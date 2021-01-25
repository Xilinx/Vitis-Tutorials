<table class="sphinxhide">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis™ - Runtime and System Optimization Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Xilinx Runtime (XRT) and Vitis System Optimization Tutorials

### Design Tutorials

The methodology for developing optimized accelerated applications is comprised of two major phases: architecting the application, and developing the hardware kernels. In the first phase, you make key decisions about the application architecture by determining which software functions should be accelerated onto FPGA kernels, how much parallelism can be achieved, and how to deliver it in code. In the second phase, you implement the kernels by structuring the source code, and applying the necessary compiler options and pragmas to create the kernel architecture needed to achieve the optimized performance target. The following examples illustrate the use of this methodology in real-world applications.


 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/01-host-code-opt/README.html">XRT Host Code Optimization</a></td>
 <td>This tutorial demonstrates how to optimize your CPU host code to get the most out of interaction between your
 hardware accelerators and your runtime software.</td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/02-ivas-ml/README.html">Streaming Video Analytics with IVAS</a></td>
 <td>This tutorial demonstrates a reference platform using the Xilinx IVAS framework for streaming video
 analytics with Vitis and Vitis AI 1.2.</td>
 </tr>
 </table>



### Feature Tutorials

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/01-mult-ddr-banks/README.html">Using Multiple DDR Banks</a></td>
 <td>This tutorial demonstrates how using multiple DDRs can improve data transfer between kernels and global memory.</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/02-using-multiple-cu/README.html">Using Multiple Compute Units</a></td>
 <td>This tutorial demonstrates the flexible kernel linking process to increase the number of kernel instances on an FPGA,
 which improves the parallelism in a combined host-kernel system.</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/03-controlling-vivado-implementation/README.html">Controlling Vivado Implementation</a></td>
 <td>This tutorial demonstrates how you can control the Vivado&reg; tools flow when implementing your project.</td>
 </tr>
 </table>

<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>
