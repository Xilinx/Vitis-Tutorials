##############################################################################
Vitis Platform Creation Tutorials
##############################################################################




******************************************
Design Tutorials
******************************************


.. toctree::
   :maxdepth: 3
   :caption: Design Tutorials
   :hidden:

   Design Tutorials <./docs/Design_Tutorials/Design_Tutorials>






.. list-table:: 
   :widths: 21 8 8 6 12 45
   :header-rows: 1
   
   * - Tutorial
     - Device Family
     - Board
     - Platform Type
     - IDE Flow
     - Design Target
	 
   * - :doc:`Versal Platform Creation Quick Start <./docs/Design_Tutorials/Vitis_Platform/README>`
     - Versal AI Core
     - VCK190
     - Flat
     - 

       - Vivado
       - Vitis Unified IDE
  
     - **Highlights:** simplest Vitis Platform creation and usage flow.

       - **Hardware design:** Using Vivado Customizable Example Design template to quick start.  
       - **Software design:** Using ``createdts`` and Common Image to quick start. 
       - **Verification:** Vector Addition.
       Note: This design flow is applicable to most AMD demo boards. 

   * - :doc:`Create a Vitis Platform for Custom Versal Boards <./docs/Design_Tutorials/03_Edge_VCK190/README>`
     - Versal AI Core
     - VCK190
     - Flat
     - 

       - Vivado
       - Vitis IDE
  
     - **Highlights:** Platform design flow for custom boards.

       - **Hardware design:** Using Vivado Customizable Example Design (device part based) to create the hardware and do further customizations.  
       - **Software design:** Using ``createdts`` and Common Image to quick start. 
       - **Verification:** Vector Addition.
       Note: This tutorial uses VCK190 board as a custom board. The design does not use any of its presets 

   * - :doc:`Versal DFX Platform Creation Tutorial <./docs/Design_Tutorials/04_Edge_VCK190_DFX/README>`
     - Versal AI Core
     - VCK190
     - DFX
     - 

       - Vivado
       - Vitis IDE
  
     - **Highlights:** Design flow for Vitis DFX (Dynamic Function eXchange) Platform.

       - **Hardware design:** Using Vivado Customizable Example Design template to quick start.  
       - **Software design:** Using ``createdts`` and Common Image to quick start. 
       - **Verification:** Vector Addition.

   * - :doc:`Create Vitis Platforms for Zynq UltraScale+ MPSoC <./docs/Design_Tutorials/02-Edge-AI-ZCU104/README>`
     - ZYNQ UltraScale+ MPSoC
     - ZCU104
     - Flat
     - 

       - Vivado
       - Vitis IDE
  
     - **Highlights:** Creating a Vitis platform for Zynq UltraScale+ MPSoC from scratch.

       - **Hardware design:** Creating the hardware design from scratching without any help of Vivado example design templates.  
       - **Software design:** Using ``createdts`` and Common Image to quick start. 
       - **Verification:** Vector Addition and Vitis-AI

   * - :doc:`Custom Kria SOM Platform Creation Example <./docs/Design_Tutorials/01-Edge-KV260/README>`
     - ZYNQ UltraScale+ MPSoC
     - KV260
     - Flat
     - 

       - Vivado
       - Vitis IDE
  
     - **Highlights:** Kria SOM Platform creation and usage flow

       - **Hardware design:** Creating from scratch
       - **Software design:** Using Common Image and showing device tree overlay (DTBO) creation flow 
       - **Verification:** Vector Addition and Kria SOM application loading procedure.







******************************************
Feature Tutorials
******************************************


.. toctree::
   :maxdepth: 3
   :caption: Feature Tutorials
   :hidden:

   Feature Tutorials <./docs/Feature_Tutorials/Feature_Tutorials>






.. list-table:: 
   :widths: 21 8 8 6 12 45
   :header-rows: 1
   
   * - Tutorial
     - Device Family
     - Board
     - Platform Type
     - IDE Flow
     - Design Target
	 
   * - :doc:`Incorporating Stream Interfaces <./docs/Feature_Tutorials/01_platform_creation_streaming_ip/README>`
     - Generic but using Versal AI Core as example
     - VCK190
     - Flat
     - 

       - Vivado
       - Vitis IDE
  
     - **Highlights:**

       - Adding custom IP into the platform hardware
       - Using AXI Stream IP in platform and kernel

   * - :doc:`PetaLinux Building and System Customization <./docs/Feature_Tutorials/02_petalinux_customization/README>`
     - ZYNQ UltraScale+ MPSoC and Versal AI Core
     - ZCU104 and VCK190
     - Flat
     - 

       - Vivado
       - Vitis IDE
  
     - **Highlights:** Customize the software components with PetaLinux

   * - :doc:`Hardware Design Fast Iteration with Vitis Export to Vivado <./docs/Feature_Tutorials/03_Vitis_Export_To_Vivado/README>`
     - Versal AI Core
     - VCK190
     - Block Design Container
     - 

       - Vivado
       - Vitis IDE
  
     - **Highlights:**

       - Skipping creating the platform before v++ linking
       - Using Vivado to do design implementation and timing closure
       - Fast iteration for hardware design

























.. sidebar:: More Information

    See Vitis Development Environment on `xilinx.com <https://www.xilinx.com/products/design-tools/vitis.html>`_.

