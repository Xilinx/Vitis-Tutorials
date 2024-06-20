##############################################################################
AIE-ML Development: Design Tutorials
##############################################################################


.. sidebar:: More Information

   See AMD Vitis™ Development Environment on `xilinx.com <https://www.xilinx.com/products/design-tools/vitis.html>`_



The AI Engine Development Design Tutorials showcase the two major phases of AI Engine-ML application development: architecting the application and developing the kernels. Both phases are demonstrated in these tutorials.


.. toctree::
   :maxdepth: 3
   :caption: Design Tutorials
   :hidden:

   Versal Custom Thin Platform Extensible System <./01-Versal_Custom_Thin_Platform_Extensible_System/README.md>
   AIE-ML Programming <./01-AIE-ML-programming-and-optimization/README.md>
   Prime Factor FFT-1008 on AIE-ML <./02-Prime-Factor-FFT/README.md>
   AIE-ML LeNet Tutorial <./03-AIE-ML-lenet_tutorial/README.md>








.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description
	 
   * - :doc:`Versal Custom Thin Platform Extensible System <./01-Versal_Custom_Thin_Platform_Extensible_System/README>`
     - This is a Versal system example design based on a VEK280 thin custom platform (Minimal clocks and AXI exposed to PL) that includes HLS/RTL kernels and AI Engine kernel using a full Makefile build-flow.

   * - :doc:`AIE-ML Programming <./01-AIE-ML-programming-and-optimization/README>`
     - This tutorial helps user understand the differences between AI Engine and AI Engine-ML architecture, it is based on matrix multiplication which is a usual algorithm in Machine Learning applications.

   * - :doc:`Prime Factor FFT-1008 on AIE-ML <./02-Prime-Factor-FFT/README>`
     - This Versal system example implements a 1008-pt FFT using the Prime Factor algorithm. The design uses both AI Engine and PL kernels working cooperatively. AI Engine elements are hand-coded using AIE API, PL elements are implemented using Vitis HLS. System integration in Vitis is managed using the new v++ Unified Command Line flow. This tutorial targets the AIE-ML architecture.

   * - :doc:`AIE-ML LeNet Tutorial <./03-AIE-ML-lenet_tutorial/README>`
     - This tutorial uses the LeNet algorithm to implement a system-level design to perform image classification using the AIE-ML architecture and PL logic, including block RAM (BRAM). The design demonstrates functional partitioning between the AIE-ML and PL. It also highlights memory partitioning and hierarchy among DDR memory, PL (BRAM), Memory tile and AI Engine memory.















