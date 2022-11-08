##############################################################################
Hardware Acceleration: Design Tutorials
##############################################################################


.. sidebar:: More Information

   See Vitis™ Development Environment on `xilinx.com <https://www.xilinx.com/products/design-tools/vitis.html>`_



The Hardware Acceleration Design Tutorials illustrate higher-level concepts or design flows, walk through specific examples or reference designs, and more complex and complete designs or applications.

The :doc:`landing page <../../Hardware_Acceleration>` of Hardware Acceleration contains important information including tool version, environment settings, and a table describing the platform, kernels, and supported features or flows of each tutorial. It is strongly recommended that you review the details before starting to use the acceleration tutorials.




.. toctree::
   :maxdepth: 3
   :caption: Design Tutorials
   :hidden:

   Convolution Example <./01-convolution-tutorial/README>
   Bloom Filter Example <./02-bloom/README>
   RTL Systems Integration Example <./03-rtl_stream_kernel_integration/README>
   Traveling Salesperson Problem <./04-traveling-salesperson/README>
   Bottom RTL Kernel Design Flow Example <./05-bottom_up_rtl_kernel/README>
   Choleskey Algorithm Acceleration <./06-cholesky-accel/README>
   XRT Host Code Optimization <./07-host-code-opt/README>
   Aurora Kernel on Alveo <./08-alveo_aurora_kernel/README>
   Single Source Shortest Path Application <./09-sssp-application/README>
   Get Moving with Alveo <./10-get_moving_with_alveo/README>





.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description

   * - :doc:`Convolution Example <./01-convolution-tutorial/README>`
     - This tutorial walks through the process of analyzing and optimizing a 2D convolution used for real-time processing of a video stream.

   * - :doc:`Bloom Filter Example <./02-bloom/README>`
     - This tutorial shows how to achieve a 10x speed-up on a data analytics application using a combination of kernel and host code optimization techniques.

   * - :doc:`RTL Systems Integration Example <./03-rtl_stream_kernel_integration/README>`
     - This tutorial demonstrates how to integrate free-running RTL kernels, Vitis Library functions, and custom Vitis HLS kernels into a real system.

   * - :doc:`Traveling Salesperson Problem <./04-traveling-salesperson/README>`
     - This tutorial demonstrates the full flow to implement a HLS kernel from algorithm model to hardware.

   * - :doc:`Bottom RTL Kernel Design Flow Example <./05-bottom_up_rtl_kernel/README>`
     - This tutorial demonstrates how to develope a complex RTL kernel from scratch via batch mode without GUI environment.

   * - :doc:`Cheleskey Algorithm Acceleration <./06-cholesky-accel/README>`
     - This tutorial puts in practice the concepts of FPGA acceleration and illustrates how to gradually optimize a hardware accelerator implementing the Cholesky matrix decomposition algorithm.

   * - :doc:`XRT Host Code Optimization <./07-host-code-opt/README>`
     - This tutorial demonstrates how to optimize your CPU host code to get the most out of interaction between your hardware accelerators and your runtime software.

   * - :doc:`Aurora Kernel on Alveo <./08-alveo_aurora_kernel/README>`
     - This tutorial demonstrates how to integrate Aurora IP in user design on Alveo card with Vitis flow to realize high-speed inter-card communications.

   * - :doc:`Single Source Shortest Path Application <./09-sssp-application/README>`
     - This tutorial demonstrates how to design an application of finding shortest path with Vitis Graph library on Alveo U50.

   * - :doc:`Get Moving with Alveo <./10-get_moving_with_alveo/README>`
     - This tutorial provides you with an easy-to-follow, guided introduction to accelerating applications with Xilinx technology, understanding the fundamental architectural approaches, identifying suitable code for acceleration, and interacting with the software APIs for managing memory and interacting with the target device in an optimal way.



