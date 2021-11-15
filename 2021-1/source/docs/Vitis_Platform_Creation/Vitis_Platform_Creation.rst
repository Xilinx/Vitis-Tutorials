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

   Module 1: Platform Creation Overview <Introduction/01-Overview/README>
   Module 2: Vitis Custom Embedded Platform Creation Tutorial on MPSoC <Introduction/02-Edge-AI-ZCU104/README>
   Module 3: Versal Custom Platform Creation Tutorial <Introduction/03_Edge_VCK190/README>




******************************************
Design Tutorials
******************************************


.. toctree::
   :maxdepth: 3
   :caption: Design Tutorials
   :hidden:

   Custom Embedded Platform Creation Example on KV260 <./Design_Tutorials/01-Edge-KV260/README>



.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description
	 
   * - :doc:`Custom Embedded Platform Creation Example on KV260 <./Design_Tutorials/01-Edge-KV260/README>`
     - `Kria KV260 Vision AI Starter Kit <https://www.xilinx.com/products/som/kria/kv260-vision-starter-kit.html>`_ is the development platform for Kria K26 SOM. The KV260 is built for advanced vision application development without requiring complex hardware design knowledge. It's based on the UltraScale+ MPSoC technology similar to ZCU104 evaluation board. In this example, the :doc:`ZCU104 custom embedded platform creation example <./Introduction/02-Edge-AI-ZCU104/README>` is extended to the KV260 Vision AI Starter Kit.




******************************************
Feature Tutorials
******************************************


.. toctree::
   :maxdepth: 3
   :caption: Feature Tutorials
   :hidden:

   Incorporation Stream Interfaces <./Feature_Tutorials/01_platform_creation_streaming_ip/README>




.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description
	 
   * - :doc:`Incorporation Stream Interfaces <./Feature_Tutorials/01_platform_creation_streaming_ip/README>`
     - This tutorial demonstrates how you can stream data between Vitis kernels and AXI stream connections exposed in the platform.
