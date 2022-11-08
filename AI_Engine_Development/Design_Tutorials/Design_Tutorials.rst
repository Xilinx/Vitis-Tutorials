##############################################################################
AI Engine Development: Design Tutorials
##############################################################################


.. sidebar:: More Information

   See Vitis™ Development Environment on `xilinx.com <https://www.xilinx.com/products/design-tools/vitis.html>`_



The AI Engine Development Design Tutorials showcase the two major phases of AI Engine application development: architecting the application and developing the kernels. Both these phases are demonstrated in these tutorials.

The :doc:`landing page <../../AI_Engine_Development>` of AI Engine Development contains important information including tool version, environment settings, and a table describing the platform, operating system, and supported features or flows of each tutorial. It is strongly recommended that you review details before starting to use the AIE tutorials.




.. toctree::
   :maxdepth: 3
   :caption: Design Tutorials
   :hidden:

   Versal Custom Thin Platform Extensible System <../Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System/README>
   LeNet Tutorial <./01-aie_lenet_tutorial/README>
   Super Sampling Rate FIR Filters <./02-super_sampling_rate_fir/README>
   Beamforming Design <./03-beamforming/README>
   2D-FFT <./06-fft2d_AIEvsHLS/README>
   FIR Filter <./07-firFilter_AIEvsHLS/README>
   N-Body Simulator <./08-n-body-simulator/README>
   Versal GeMM Implementation <./10-GeMM_AIEvsDSP/README>





.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description
	 
   * - :doc:`Versal Custom Thin Platform Extensible System <../Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System/README>`
     - This is a Versal system example design based on a VCK190 thin custom platform (Minimal clocks and AXI exposed to PL) that includes HLS/RTL kernels and AI Engine kernel using a full Makefile build-flow.

   * - :doc:`LeNet Tutorial <./01-aie_lenet_tutorial/README>`
     - This tutorial uses the LeNet algorithm to implement a system-level design to perform image classification using the AI Engine and PL logic, including block RAM (BRAM). The design demonstrates functional partitioning between the AI Engine and PL. It also highlights memory partitioning and hierarchy among DDR memory, PL (BRAM) and AI Engine memory.

   * - :doc:`Super Sampling Rate FIR Filters <./02-super_sampling_rate_fir/README>`
     - The purpose of this tutorial is to provide a methodology to enable you to make appropriate choices depending on the filter characteristics, and to provide examples on how to implement Super Sampling Rate (SSR) FIR Filters on a Versal® ACAP AI Engine processor array.

   * - :doc:`Beamforming Design <./03-beamforming/README>`
     - This tutorial demonstrates the creation of a beamforming system running on the AI Engine, PL, and PS, and the validation of the design running on this heterogeneous domain.

   * - :doc:`2D-FFT <./06-fft2d_AIEvsHLS/README>`
     - This tutorial performs two implementations of a system-level design (2D-FFT): one with AI Engine, and the other with HLS using the DSP Engines.

   * - :doc:`FIR Filter <./07-firFilter_AIEvsHLS/README>`
     - This tutorial demonstrates the implementations of a system-level design (FIR Filter) using AI Engines and HLS with DSP Engines in the Versal device plus PL including LUTs, flip-flops (FFs), and block RAMs.

   * - :doc:`N-Body Simulator <./08-n-body-simulator/README>`
     - It is a system-level design that uses the AI Engine, PL, and PS resources to showcase the following features:

       - A Python model of an N-Body Simulator run on x86 machine
       - A scalable AI Engine design that can utilize up to 400 AI Engine tiles
       - AI Engine packet switching
       - AI Engine single-precision floating point calculations
       - AI Engine 1:400 broadcast streams
       - Codeless PL HLS datamover kernels from the Vitis™ Utility Library
       - PL HLS packet switching kernels
       - PS Host Application that validates the data coming out of the AI Engine design
       - C++ model of an N-Body Simulator
       - Performance comparisons between Python x86, C++ Arm A72, and AI Engine N-Body Simulators
       - Effective throughput calculation (GFLOPS) vs. Theoretical peak throughput of AI Engine

   * - :doc:`Versal GeMM Implementation <./10-GeMM_AIEvsDSP/README>`
     - This tutorial performs two implementations of a system-level design: one with AI Engine, and the other with RTL using the DSP Engines. In each implementation, the tutorial takes you through the hardware emulation and hardware flow in the context of a complete Versal ACAP system design.

