#######################################
Vitis Hardware Acceleration
#######################################

.. sidebar:: More Information

    See Vitis™ Development Environment on `xilinx.com <https://www.xilinx.com/products/design-tools/vitis.html>`_


The methodology for developing optimized accelerated applications is comprised of two major phases: architecting the application, and developing the hardware kernels. In the first phase, you make key decisions about the application architecture by determining which software functions should be accelerated onto FPGA kernels, how much parallelism can be achieved, and how to deliver it in code. In the second phase, you implement the kernels by structuring the source code, and applying the necessary compiler options and pragmas to create the kernel architecture needed to achieve the optimized performance target. The following examples illustrate the use of this methodology in real-world applications.



************************************************
Introduction
************************************************


.. toctree::
   :maxdepth: 1
   :caption: Introduction
   :hidden:

   Introduction to Vitis Hardware Acceleration <./Introduction/README>



.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description
	 
   * - :doc:`Introduction to Vitis Hardware Acceleration <./Introduction/README>`
     - This tutorial provides you with an easy-to-follow, guided introduction to accelerating applications with Xilinx technology. We will begin from the first principles of acceleration: understanding the fundamental architectural approaches, identifying suitable code for acceleration, and interacting with the software APIs for managing memory and interacting with the target device in an optimal way.





************************************************
Design Tutorials
************************************************


.. toctree::
   :maxdepth: 3
   :caption: Design Tutorials
   :hidden:

   Bloom Filter Example <./Design_Tutorials/02-bloom/README>
   Convolution Example <./Design_Tutorials/01-convolution-tutorial/README>
   RTL Systems Integration Example <./Design_Tutorials/03-rtl_stream_kernel_integration/README>
   Traveling Salesperson Problem <./Design_Tutorials/04-traveling-salesperson/README>
   Bottom RTL Kernel Design Flow Example <./Design_Tutorials/05-bottom_up_rtl_kernel/README>
   Choleskey Algorithm Acceleration <./Design_Tutorials/06-cholesky-accel/README>
   XRT Host Code Optimization <./Design_Tutorials/07-host-code-opt/README>
   Aurora Kernel on Alveo <./Design_Tutorials/08-alveo_aurora_kernel/README>



.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description
	 
   * - :doc:`Bloom Filter Example <./Design_Tutorials/02-bloom/README>`
     - This tutorial shows how to achieve a 10x speed-up on a data analytics application using a combination of kernel and host code optimization techniques.

   * - :doc:`Convolution Example <./Design_Tutorials/01-convolution-tutorial/README>`
     - This tutorial walks through the process of analyzing and optimizing a 2D convolution used for real-time processing of a video stream.

   * - :doc:`RTL Systems Integration Example <./Design_Tutorials/03-rtl_stream_kernel_integration/README>`
     - This tutorial demonstrates how to integrate free-running RTL kernels, Vitis Library functions, and custom Vitis HLS kernels into a real system.

   * - :doc:`Traveling Salesperson Problem <./Design_Tutorials/04-traveling-salesperson/README>`
     - This tutorial demonstrates the full flow to implement a HLS kernel from algorithm model to hardware.

   * - :doc:`Bottom RTL Kernel Design Flow Example <./Design_Tutorials/05-bottom_up_rtl_kernel/README>`
     - This tutorial demonstrates how to develope a complex RTL kernel from scratch via batch mode without GUI environment.

   * - :doc:`Cheleskey Algorithm Acceleration <./Design_Tutorials/06-cholesky-accel/README>`
     - This tutorial puts in practice the concepts of FPGA acceleration and illustrates how to gradually optimize a hardware accelerator implementing the Cholesky matrix decomposition algorithm.

   * - :doc:`XRT Host Code Optimization <./Design_Tutorials/07-host-code-opt/README>`
     - This tutorial demonstrates how to optimize your CPU host code to get the most out of interaction between your hardware accelerators and your runtime software.

   * - :doc:`Streaming Video Analytics with IVAS <./Design_Tutorials/08-ivas-ml/README>`
     - This tutorial demonstrates a reference platform using the Xilinx IVAS framework for streaming video analytics with Vitis and Vitis AI.



************************************************
Feature Tutorials
************************************************

.. toctree::
   :maxdepth: 3
   :caption: Feature Tutorials
   :hidden:

   Getting Started with RTL Kernels <./Feature_Tutorials/01-rtl_kernel_workflow/README>
   Mixing C and RTL <./Feature_Tutorials/02-mixing-c-rtl-kernels/README>
   Dataflow Debug and Optimization <./Feature_Tutorials/03-dataflow_debug_and_optimization/README>
   Using Multiple DDR Banks <./Feature_Tutorials/04-mult-ddr-banks/README>
   Using Multiple Compute Units <./Feature_Tutorials/05-using-multiple-cu/README>
   Controlling Vivado Implementation <./Feature_Tutorials/06-controlling-vivado-implementation/README>
   Optimizing for HBM <./Feature_Tutorials/07-using-hbm/README>
   Host Memory Access <./Feature_Tutorials/08-using-hostmem/README>
   Using GT Kernels and Ethernet IPs on Alveo <./Feature_Tutorials/09-using-ethernet-on-alveo/README>





.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description
	 
   * - :doc:`Getting Started with RTL Kernels <./Feature_Tutorials/01-rtl_kernel_workflow/README>`
     - This tutorial demonstrates how to use the Vitis core development kit to program an RTL kernel into an FPGA and build a Hardware Emulation using a common development flow.

   * - :doc:`Mixing C and RTL <./Feature_Tutorials/02-mixing-c-rtl-kernels/README>`
     - This tutorial demonstrates how to work with an application containing RTL and C kernels, along with various design analysis features.

   * - :doc:`Dataflow Debug and Optimization <./Feature_Tutorials/03-dataflow_debug_and_optimization/README>`
     - This tutorial demonstrates how to debug and optimize the dataflow optimization in Vitis HLS.

   * - :doc:`Using Multiple DDR Banks <./Feature_Tutorials/04-mult-ddr-banks/README>`
     - This tutorial demonstrates how using multiple DDRs can improve data transfer between kernels and global memory.

   * - :doc:`Using Multiple Compute Units <./Feature_Tutorials/05-using-multiple-cu/README>`
     - This tutorial demonstrates the flexible kernel linking process to increase the number of kernel instances on an FPGA, which improves the parallelism in a combined host-kernel system.

   * - :doc:`Controlling Vivado Implementation <./Feature_Tutorials/06-controlling-vivado-implementation/README>`
     - This tutorial demonstrates how you can control the Vivado&reg; tools flow when implementing your project.

   * - :doc:`Optimizing for HBM <./Feature_Tutorials/07-using-hbm/README>`
     - This tutorial demonstrates how you can take best advantage of HBM on platforms that support it.

   * - :doc:`Host Memory Access <./Feature_Tutorials/08-using-hostmem/README>`
     - This tutorial demonstrates how kernels can directly access buffers host memory directly. This capability requires a compatible platform.

   * - :doc:`Using GT Kernels and Ethernet IPs on Alveo <./Feature_Tutorials/09-using-ethernet-on-alveo/README>`
     - This tutorial demonstrates how to use networking GT kernels with generated Ethernet IPs and implement them on Alveo card with Vitis flow.

