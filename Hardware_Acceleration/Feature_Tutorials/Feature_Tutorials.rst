##############################################################################
Hardware Acceleration: Feature Tutorials
##############################################################################


.. sidebar:: More Information

   See Vitis™ Development Environment on `xilinx.com <https://www.xilinx.com/products/design-tools/vitis.html>`_



The Hardware Acceleration Feature Tutorials illustrate specific features or flows of Vitis, some features may not be required by all designs but are still useful for some use cases.

The :doc:`landing page <../../Hardware_Acceleration>` of Hardware Acceleration contains important information including tool version, environment settings, and a table describing the platform, kernels, and supported features or flows of each tutorial. It is strongly recommended that you review the details before starting to use the acceleration tutorials.




.. toctree::
   :maxdepth: 3
   :caption: Feature Tutorials
   :hidden:

   Getting Started with RTL Kernels <./01-rtl_kernel_workflow/README>
   Mixing C and RTL <./02-mixing-c-rtl-kernels/README>
   Dataflow Debug and Optimization <./03-dataflow_debug_and_optimization/README>
   Using Multiple DDR Banks <./04-mult-ddr-banks/README>
   Using Multiple Compute Units <./05-using-multiple-cu/README>
   Controlling Vivado Implementation <./06-controlling-vivado-implementation/README>
   Optimizing for HBM <./07-using-hbm/README>
   Host Memory Access <./08-using-hostmem/README>
   Using GT Kernels and Ethernet IPs on Alveo <./09-using-ethernet-on-alveo/README>
   Enabling FPGA to FPGA P2P Transfer <./10-p2p-card-to-card/README>



.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description
	 
   * - :doc:`Getting Started with RTL Kernels <./01-rtl_kernel_workflow/README>`
     - This tutorial demonstrates how to use the Vitis core development kit to program an RTL kernel into an FPGA and build a Hardware Emulation using a common development flow.

   * - :doc:`Mixing C and RTL <./02-mixing-c-rtl-kernels/README>`
     - This tutorial demonstrates how to work with an application containing RTL and C kernels, along with various design analysis features.

   * - :doc:`Dataflow Debug and Optimization <./03-dataflow_debug_and_optimization/README>`
     - This tutorial demonstrates how to debug and optimize the dataflow optimization in Vitis HLS.

   * - :doc:`Using Multiple DDR Banks <./04-mult-ddr-banks/README>`
     - This tutorial demonstrates how using multiple DDRs can improve data transfer between kernels and global memory.

   * - :doc:`Using Multiple Compute Units <./05-using-multiple-cu/README>`
     - This tutorial demonstrates the flexible kernel linking process to increase the number of kernel instances on an FPGA, which improves the parallelism in a combined host-kernel system.

   * - :doc:`Controlling Vivado Implementation <./06-controlling-vivado-implementation/README>`
     - This tutorial demonstrates how you can control the Vivado&reg; tools flow when implementing your project.

   * - :doc:`Optimizing for HBM <./07-using-hbm/README>`
     - This tutorial demonstrates how you can take best advantage of HBM on platforms that support it.

   * - :doc:`Host Memory Access <./08-using-hostmem/README>`
     - This tutorial demonstrates how kernels can directly access buffers host memory directly. This capability requires a compatible platform.

   * - :doc:`Using GT Kernels and Ethernet IPs on Alveo <./09-using-ethernet-on-alveo/README>`
     - This tutorial demonstrates how to use networking GT kernels with generated Ethernet IPs and implement them on Alveo card with Vitis flow.

   * - :doc:`Enabling FPGA to FPGA P2P Transfer using Native XRT C++ API <./10-p2p-card-to-card/README>`
     - This tutorial demonstrates how to enable p2p transfer from one FPGA device to another using XRT API host code.

