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

   Versal Custom Thin Platform Extensible System <./01-Versal_Custom_Thin_Platform_Extensible_System/README>
   AIE-ML Programming <./01-AIE-ML-programming-and-optimization/README>


.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description
	 
   * - :doc:`Versal Custom Thin Platform Extensible System <./01-Versal_Custom_Thin_Platform_Extensible_System/README>`
     - This is a Versal system example design based on a VEK280 thin custom platform (Minimal clocks and AXI exposed to PL) that includes HLS/RTL kernels and AI Engine kernel using a full Makefile build-flow.

   * - :doc:`LeNet Tutorial <./01-AIE-ML-programming-and-optimization/README>`
     - This tutorial helps user understand the differences between AI Engine and AI Engine-ML architecture, it is based on matrix multiplication which is a usual algorithm in Machine Learning applications.
