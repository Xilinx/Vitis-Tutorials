##############################################################################
Vitis System Design Tutorials: Design Tutorials
##############################################################################


Design tutorials illustrate higher-level concepts or design flows, walk through specific examples or reference designs, and describe more complex and complete designs or applications.


.. toctree::
   :maxdepth: 3
   :caption: Design Tutorials
   :hidden:

   Versal Custom Thin Platform Extensible System <./01-Versal_Custom_Thin_Platform_Extensible_System/README.md>
   Versal Custom Platform Integration using Vitis Subsystem <./02-Versal_Vitis_Subsystem_Flow/README.md>
   Versal AI Edge Gen2 Design Flow with Vitis Unified IDE <./03-Versal_AI_Edge_Gen2_Design_Flow/README.md>
   Flexible AI Engine Development - From Vitis to Vivado with Advanced Control <./04-Versal_AIE_Advanced_Flow_VSS_VMA_NPM/README.md>
   VCK190 Segmented Configuration with Petalinux <./05-Versal_Segmented_configuration_Petalinux_Flow/README.md>



.. list-table::
   :widths: 20 80
   :header-rows: 1

   * - Tutorial
     - Description

   * - :doc:`Versal Custom Thin Platform Extensible System <./01-Versal_Custom_Thin_Platform_Extensible_System/README>`
     - Versal VCK190 System Example Design based on a thin custom platform (Minimal clocks and AXI exposed to PL) including HLS/RTL kernels and AI Engine kernel using a full Makefile build-flow.

   * - :doc:`Versal Custom Platform Integration using Vitis Subsystem <./02-Versal_Vitis_Subsystem_Flow/README>`
     - An end to end system design tutorial using Vitis Subsystem design and Vitis Export to Vivado Flow as described in UG1701. Check out the features demonstrated in this tutorial.

   * - :doc:`Versal AI Edge Gen2 Design Flow with Vitis Unified IDE <./03-Versal_AI_Edge_Gen2_Design_Flow/README>`
     - This tutorial guides you creating an acceleration application for the VEK385 Evaluation board, starting from an extensible XSA with pre-built YOCTO binaries supporting EDF flow and built together in Vitis Unified IDE.

   * - :doc:`Flexible AI Engine Development: From Vitis to Vivado with Advanced Control <./04-Versal_AIE_Advanced_Flow_VSS_VMA_NPM/README>`
     - This tutorial demonstrates a complete workflow for integrating AI Engine designs using the Vitis Subsystem (VSS) → Vitis Metadata Archive (VMA) → Vivado Non-Project Mode (NPM) flow. This workflow enables independent AIE development, seamless transition from Vitis to Vivado, and advanced control for experienced hardware designers.

   * - :doc:`VCK190 Segmented Configuration with Petalinux <./05-Versal_Segmented_configuration_Petalinux_Flow/README>`
     - This tutorial guides you through creating a complete acceleration application for the VCK190 board using segmented configuration desing and Petalinux common image. You will learn how to build a hardware platform, integrate AI Engine and PL kernels, and deploy a complete system.



.. Copyright © 2025 Advanced Micro Devices, Inc

.. `Terms and Conditions <https://www.amd.com/en/corporate/copyright>`_.
