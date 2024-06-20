##############################################################################
AIE-ML Development: Feature Tutorials
##############################################################################


.. sidebar:: More Information

   See AMD Vitis™ Development Environment on `xilinx.com <https://www.xilinx.com/products/design-tools/vitis.html>`_



The AIE-ML Development Feature Tutorials highlight specific features and flows that help develop AIE-ML applications.


.. toctree::
   :maxdepth: 3
   :caption: Feature Tutorials
   :hidden:

   A to Z Bare-metal Flow <./01-aie_a_to_z/README.md>
   Using GMIO with AIE-ML <./02-using-gmio/README.md>
   Runtime Parameter Reconfiguration <./03-rtp-reconfiguration/README.md>
   Packet Switching <./04-packet-switching/README.md>
   Versal Integration for Hardware Emulation and Hardware <./05-AI-engine-versal-integration/README.md>
   AI Engine-ML Performance Analysis Tutorial <./13-aie-ml-performance-analysis/README.md>
   AIE Compiler Features <./20-aiecompiler-features/README.md>
   Performance Validation in Analysis View <./22-Performance-Validation-in-Analysis-View/README.md>




.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description

   * - :doc:`A to Z Bare-metal Flow <./01-aie_a_to_z/README>`
     - This tutorial walks through the steps to create a custom Baremetal platform, and also integrate Baremetal host application along with an AI Engines graph and PL kernels.

   * - :doc:`Using GMIO with AIE <./02-using-gmio/README>`
     - This tutorial introduces the usage of global memory I/O (GMIO) for sharing data between the AI Engine-ML (AIE-ML) and external DDR.

   * - :doc:`Runtime Parameter Reconfiguration <./03-rtp-reconfiguration/README>`
     - Learn how to dynamically update AI Engine-ML (AIE-ML) runtime parameters.

   * - :doc:`Packet Switching <./04-packet-switching/README>`
     - This tutorial illustrates how to use data packet switching with AI Engine-ML (AIE-ML) designs to optimize efficiency.

   * - :doc:`AI Engine Versal Integration for Hardware Emulation and Hardware <./05-AI-engine-versal-integration/README>`
     - This tutorial demonstrates creating a system design running on the AI Engine-ML (AIE-ML), PS, and PL and validating the design running on these heterogeneous domains by running Hardware Emulation.

   * - :doc:`AI Engine-ML Performance Analysis Tutorial <./13-aie-ml-performance-analysis/README>`
     - This tutorial introduces you to performance analysis and optimization methods, and shows you how synchronization works in graph execution. It also demonstrates the analysis of a hang issue using an example.

   * - :doc:`AIE Compiler Features <./20-aiecompiler-features/README>`
     - This tutorial shares a variety of features that are useful for AI Engine / AI Engine-ML (AIE-ML) programming to create more visible and efficient code compared to early versions of the compiler.

   * - :doc:`Performance Validation in Analysis View <./22-Performance-Validation-in-Analysis-View/README>`
     - This tutorial demonstrates throughput and latency computation after AI Engine Simulation and their analysis in the Analysis View of the Vitis Unified IDE.















