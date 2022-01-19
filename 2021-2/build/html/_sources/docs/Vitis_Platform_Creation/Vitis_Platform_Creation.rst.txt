##############################################################################
Vitis Platform Creation Tutorials
##############################################################################







.. sidebar:: More Information

    See Vitis Development Environment on `xilinx.com <https://www.xilinx.com/products/design-tools/vitis.html>`_.


******************************************
Introduction
******************************************

Xilinx provides base platforms for Xilinx Evaluation Boards to allow for quick out-of-the-box experimentation.

For additional customization or more in-depth development on Xilinx boards, or to develop on custom board, you might want to create a fully customized hardware platform.

These tutorials introduce the basics of platform creation and provide step-by-step tutorials for creating custom Vitis and Vitis AI™ ready platforms.


.. toctree::
   :maxdepth: 3
   :caption: Introduction
   :hidden:

   Module 1: Platform Creation Overview <Introduction/01-Overview/README>
   Module 2: Vitis Custom Embedded Platform Creation Tutorial on MPSoC <Introduction/02-Edge-AI-ZCU104/README>
   Module 3: Versal Custom Platform Creation Tutorial <Introduction/03_Edge_VCK190/README>


.. list-table:: 
   :widths: 100
   :header-rows: 1
   
   * - Tutorial
	 
   * - :doc:`Module 1-Platform Creation Overview <Introduction/01-Overview/README>`

   * - :doc:`Module 2-Vitis Custom Embedded Platform Creation Tutorial on MPSoC <Introduction/02-Edge-AI-ZCU104/README>`

   * - :doc:`Module 3-Versal Custom Platform Creation Tutorial <Introduction/03_Edge_VCK190/README>`


******************************************
Design Tutorials
******************************************


.. toctree::
   :maxdepth: 3
   :caption: Design Tutorials
   :hidden:

   Custom Embedded Platform Creation Example on KV260 <./Design_Tutorials/01-Edge-KV260/README.md>


.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description
	 
   * - :doc:`Custom Embedded Platform Creation Example on KV260 <./Design_Tutorials/01-Edge-KV260/README>`
     - This tutorial extends the :doc:`ZCU104 custom embedded platform creation example <./Introduction/02-Edge-AI-ZCU104/README>` to the KV260 Vision AI Starter Kit.



******************************************
Feature Tutorials
******************************************


.. toctree::
   :maxdepth: 3
   :caption: Feature Tutorials
   :hidden:

   Incorporation Stream Interfaces <./Feature_Tutorials/01_platform_creation_streaming_ip/README.md>




.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description
	 
   * - :doc:`Incorporation Stream Interfaces <./Feature_Tutorials/01_platform_creation_streaming_ip/README>`
     - This tutorial demonstrates how you can stream data between Vitis kernels and AXI stream connections exposed in the platform.
