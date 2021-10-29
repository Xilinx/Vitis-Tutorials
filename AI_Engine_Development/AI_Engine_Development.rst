#######################################
AI Engine Development
#######################################


.. sidebar:: More Information

   See Vitis™ Development Environment on `xilinx.com <https://www.xilinx.com/products/design-tools/vitis.html>`_



The methodology for developing optimized accelerated applications is comprised of two major phases: architecting the application, and developing the kernels. In the first phase, you make key decisions about the application architecture by determining which software functions should be accelerated onto ACAP kernels, how much parallelism can be achieved, and how to deliver it in code. In the second phase, you implement the kernels by structuring the source code, and applying the necessary build optionss to create the kernel architecture needed to achieve the optimized performance target. The following examples illustrate the use of this methodology in real-world applications.



******************************************
Design Tutorials
******************************************


.. toctree::
   :maxdepth: 3
   :caption: Design Tutorials
   :hidden:

   LeNet Tutorial <./Design_Tutorials/01-aie_lenet_tutorial/README>
   Super Sampling Rate FIR Filters <./Design_Tutorials/02-super_sampling_rate_fir/README>
   AIE Emulation on Custom Platforms <./Design_Tutorials/04-custom-platform-emulation/README>




.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description
	 
   * - :doc:`LeNet Tutorial <Design_Tutorials/01-aie_lenet_tutorial/README>`
     - This tutorial uses the LeNet algorithm to implement a system-level design to perform image classification using the AI Engine and PL logic, including block RAM (BRAM). The design demonstrates functional partitioning between the AI Engine and PL. It also highlights memory partitioning and hierarchy among DDR memory, PL (BRAM) and AI Engine memory.

   * - :doc:`Super Sampling Rate FIR Filters <./Design_Tutorials/02-super_sampling_rate_fir/README>`
     - The purpose of this tutorial is to provide a methodology to enable you to make appropriate choices depending on the filter characteristics, and to provide examples on how to implement Super Sampling Rate (SSR) FIR Filters on a Versal® ACAP AI Engine processor array.

   * - :doc:`AIE Emulation on Custom Platforms <./Design_Tutorials/04-custom-platform-emulation/README>`
     - This tutorial demonstrates the creation and emulation of an AIE design including the Adaptive DataFlow (ADF) graph, RTL kernels, and a custom VCK190 platform.



******************************************
Feature Tutorials
******************************************


.. toctree::
   :maxdepth: 3
   :caption: Feature Tutorials
   :hidden:

   A to Z Bare-metal Flow <./Feature_Tutorials/01-aie_a_to_z/README>
   Using GMIO with AIE <./Feature_Tutorials/02-using-gmio/README>
   Runtime Parameter Reconfiguration <./Feature_Tutorials/03-rtp-reconfiguration/README>
   Packet Switching <./Feature_Tutorials/04-packet-switching/README>
   Versal Integration for Hardware Emulation and Hardware <./Feature_Tutorials/05-AI-engine-versal-integration/README>
   Versal System Design Clocking <./Feature_Tutorials/06-versal-system-design-clocking-tutorial/README>
   Using Floating-Point in the AI Engine <./Feature_Tutorials/07-AI-Engine-Floating-Point/README>
   DSP Library Tutorial <./Feature_Tutorials/08-dsp-library/README>
   Debug Walkthrough <./Feature_Tutorials/09-debug-walkthrough/README>
   AI Engine DSP Library and Model Composer Tutorial <./Feature_Tutorials/10-aie-dsp-lib-model-composer/README>
   Versal Emulation Waveform Analysis <./Feature_Tutorials/11-ai-engine-emulation-waveform-analysis/README>
   AXIS External Traffic Generator <./Feature_Tutorials/12-axis-traffic-generator/README>
   AI Engine Performance and Deadlock Analysis Tutorial <./Feature_Tutorials/13-aie-performance-analysis/README>
   Implementing an IIR Filter on the AI Engine <./Feature_Tutorials/14-implementing-iir-filter/README>

 <td align="center"><a href="./Feature_Tutorials/implementing-iir-filter/">Implementing an IIR Filter on the AI Engine</a></td>
 <td>
This multi-part tutorial describes how to implement an [infinite impulse response (IIR) filter](https://en.wikipedia.org/wiki/Infinite_impulse_response) on the AI Engine.</td>
 </tr>

.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description
	 
   * - :doc:`A to Z Bare-metal Flow <./Feature_Tutorials/01-aie_a_to_z/README>`
     - This tutorial introduces a complete end to end flow for a bare-metal host application using AI Engines and PL kernels.

   * - :doc:`Using GMIO with AIE <./Feature_Tutorials/02-using-gmio/README>`
     - Introduces the usage of global memory I/O (GMIO) for sharing data between the AI Engines and external DDR.

   * - :doc:`Runtime Parameter Reconfiguration <./Feature_Tutorials/03-rtp-reconfiguration/README>`
     - Learn how to dynamically update AI Engine runtime parameters.

   * - :doc:`Packet Switching <./Feature_Tutorials/04-packet-switching/README>`
     - This tutorial illustrates how to use data packet switching with AI Engine designs to optimize efficiency.

   * - :doc:`AI Engine Versal Integration for Hardware Emulation and Hardware <./Feature_Tutorials/05-AI-engine-versal-integration/README>`
     - This tutorial demonstrates creating a system design running on the AI Engine, PS, and PL and validating the design running on these heterogeneous domains by running Hardware Emulation.

   * - :doc:`Versal System Design Clocking <./Feature_Tutorials/06-versal-system-design-clocking-tutorial/README>`
     - This tutorial demonstrates clocking concepts for the Vitis compiler by defining clocking for ADF graph PL kernels and PLIO kernels, using the clocking automation functionality.

   * - :doc:`Using Floating-Point in the AI Engine <./Feature_Tutorials/07-AI-Engine-Floating-Point/README>`
     - These examples demonstrate floating-point vector computations in the AI Engine.

   * - :doc:`DSP Library Tutorial <./Feature_Tutorials/08-dsp-library/README>`
     - This tutorial demonstrates how to use kernels provided by the DSP library for a filtering application, how to analyze the design results, and how to use filter parameters to optimize the design's performance using simulation.

   * - :doc:`Debug Walkthrough <./Feature_Tutorials/09-debug-walkthrough/README>`
     - This tutorial demonstrates how to debug a multi-processor application using the Versal ACAP AI Engines, using a beamformer example design. The tutorial illustrates functional debug and performance level debug techniques.

   * - :doc:`AI Engine DSP Library and Model Composer Tutorial <./Feature_Tutorials/10-aie-dsp-lib-model-composer/README>`
     - This tutorial shows how to design AI Engine applications using Model Composer. This set of blocksets for Simulink is used to demonstrate how easy it is to develop applications for Xilinx devices, integrating RTL/HLS blocks for the Programmable Logic, as well as AI Engine blocks for the AI Engine array.

   * - :doc:`Versal Emulation Waveform Analysis <./Feature_Tutorials/11-ai-engine-emulation-waveform-analysis/README>`
     - This tutorial demonstrates how you can use the Vivado logic simulator (XSIM) waveform GUI, and the Vitis analyzer to debug and analyze your design for a Versal ACAP.

   * - :doc:`AXIS External Traffic Generator <./Feature_Tutorials/12-axis-traffic-generator/README>`
     - This tutorial shows how to use AXI Traffic Generators to provide input and capture output from an AI Engine kernel in hardware emulation.

   * - :doc:`AI Engine Performance and Deadlock Analysis Tutorial <./Feature_Tutorials/13-aie-performance-analysis/README>`
     - This tutorial introduces you to performance analysis and optimization methods, and shows you how synchronization works in graph execution. It also demonstrates the analysis of a hang issue using an example.

   * - :doc:`Implementing an IIR Filter on the AI Engine <./Feature_Tutorials/14-implementing-iir-filter/README>`
     - This multi-part tutorial describes how to implement an `infinite impulse response (IIR) filter <https://en.wikipedia.org/wiki/Infinite_impulse_response>`_ on the AI Engine.