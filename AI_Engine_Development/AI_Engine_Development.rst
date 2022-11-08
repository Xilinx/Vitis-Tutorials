#######################################
AI Engine Development
#######################################


.. sidebar:: More Information

   See Vitis™ Development Environment on `xilinx.com <https://www.xilinx.com/products/design-tools/vitis.html>`_

The tutorials under the AI Engine Development help you learn how to target, develop, and deploy advanced algorithms using a Versal AI Engine array in conjunction with PL IP/kernels and software applications running on the embedded processors. In order to successfully deploy AI Engine applications in hardware, you need to be aware of the Vitis and AI Engine tools and flows.


* :doc:`Feature Tutorials <./docs/Feature_Tutorials/Feature_Tutorials>` highlight specific features and flows that help develop AI Engine applications.
* :doc:`Design Tutorials <./docs/Design_Tutorials/Design_Tutorials>` showcase the two major phases of AI Engine application development: architecting the application and developing the kernels. Both these phases are demonstrated in these tutorials.

Tutorials target the **VCK190** board. The table below lists the tutorials available, and the features and flows showcased in all of these tutorials. The various columns correspond to specific features/flows supported in these tutorials and will help you identify tutorials that showcase specific flows and features that you are interested in.


******************************************
Environment Settings
******************************************

Before beginning a tutorial, read and follow the `Vitis Software Platform Release Notes <https://docs.xilinx.com/access/sources/dita/topic?Doc_Version=2022.2%20English&url=ug1393-vitis-application-acceleration&resourceid=wlk1553469789555.html>`_ (v2022.2) for setting up software and installing the VCK190 base platform.

Run the following steps to set up the environment (**NOT** apply to tutorials that do not use the VCK190 base platform):

1. Set up your platform by running the ``xilinx-versal-common-v2022.2/environment-setup-cortexa72-cortexa53-xilinx-linux`` script as provided in the platform download. This script sets up the ``SYSROOT`` and ``CXX`` variables. If the script is not present, you **must** run the ``xilinx-versal-common-v2022.2/sdk.sh``.
2. Set up your ``ROOTFS`` to point to the ``xilinx-versal-common-v2022.2/rootfs.ext4``.
3. Set up your ``IMAGE`` to point to ``xilinx-versal-common-v2022.2/Image``.
4. Set up your ``PLATFORM_REPO_PATHS`` environment variable based upon where you downloaded the platform.




******************************************
Feature Tutorials
******************************************


.. toctree::
   :maxdepth: 3
   :caption: Feature Tutorials
   :hidden:

   Feature Tutorials <./docs/Feature_Tutorials/Feature_Tutorials>

.. csv-table:: Feature Tutorials
   :header: "Tutorial","Platform","OS","IDE Flow","Libraries Used","HLS Kernel","x86 simulator","aie simulator","SW Emu","HW Emu","HW","Event Trace in HW","Profile in HW"
   :widths: 22, 8, 8, 6, 7, 8, 7, 7, 5, 6, 4, 6, 6

   ":doc:`AI Engine A-to-Z Flow for Linux <./docs/Feature_Tutorials/18-aie_a_to_z_custom_linux_platform/README>`","Base / Custom","Linux","","","MM2S / S2MM","x","x","x","x","x",""," "
   ":doc:`A to Z Bare-metal Flow <./docs/Feature_Tutorials/01-aie_a_to_z/README>`","Custom","Baremetal","x","","MM2S / S2MM","","x","","x","x",""," "
   ":doc:`Using GMIO with AIE <./docs/Feature_Tutorials/02-using-gmio/README>`","Base","Linux","","","","","x","","x","x","","x "
   ":doc:`Runtime Parameter Reconfiguration <./docs/Feature_Tutorials/03-rtp-reconfiguration/README>`","Base","Linux","","","MM2S / S2MM","","x","","x","x",""," "
   ":doc:`Packet Switching <./docs/Feature_Tutorials/04-packet-switching/README>`","Base","Linux","","","MM2S / S2MM","","x","","x","x",""," "
   ":doc:`AIE Versal Integration <./docs/Feature_Tutorials/05-AI-engine-versal-integration/README>`","Base","Linux","","","MM2S / S2MM","x","x","x","x","x",""," "
   ":doc:`Versal System Design Clocking <./docs/Feature_Tutorials/06-versal-system-design-clocking-tutorial/README>`","Base","Linux","","","MM2S / S2MM","","x","","x","x",""," "
   ":doc:`Using Floating-Point in the AIE <./docs/Feature_Tutorials/07-AI-Engine-Floating-Point/README>`","Base","Linux","","","","","x","","","",""," "
   ":doc:`DSP Library Tutorial  <./docs/Feature_Tutorials/08-dsp-library/README>`","Base","Linux","","DSPLib","MM2S / S2MM Variant","","x","","","",""," "
   ":doc:`Debug Walkthrough Tutorial <./docs/Feature_Tutorials/09-debug-walkthrough/README>`","Base","Linux","x","","","x","x","x","x","x","x","x "
   ":doc:`AIE DSPLib and Model Composer <./docs/Feature_Tutorials/10-aie-dsp-lib-model-composer/README>`","Base","Linux","x","DSPLib","MM2S / S2MM","x","x","","","",""," "
   ":doc:`Versal Emulation Waveform Analysis <./docs/Feature_Tutorials/11-ai-engine-emulation-waveform-analysis/README>`","Base","Linux","","","Traffic Generators","","","","x","",""," "
   ":doc:`AXIS External Traffic Generator <./docs/Feature_Tutorials/12-axis-traffic-generator/README>`","Base","Linux","","DSPLib","MM2S / S2MM","","x","","x","",""," "
   ":doc:`AIE Performance and Deadlock Analysis <./docs/Feature_Tutorials/13-aie-performance-analysis/README>`","Base","Linux","","","","","x","","x","x","","x "
   ":doc:`Implementing an IIR Filter on the AIE <./docs/Feature_Tutorials/14-implementing-iir-filter/README>`","Base","Linux","x","","","x","x","x","","",""," "
   ":doc:`Post-Link Recompile of an AIE Application <./docs/Feature_Tutorials/15-post-link-recompile/README>`","Base","Linux","","","MM2S / S2MM","","x","","x","x",""," "
   ":doc:`Python and C++ External Traffic Generators for AI Engine Simulation and Emulation Flows <./docs/Feature_Tutorials/16-external-traffic-generator-aie/README>`","Base","Linux","","","MM2S / S2MM / PolarClip","x","x","x","x","",""," "
   ":doc:`Using RTL IP with AI Engines <./docs/Feature_Tutorials/17-RTL-IP-with-AIE-Engines/README>`","Custom","Linux","","","MM2S / S2MM","","x","","x","",""," "






******************************************
Design Tutorials
******************************************



.. toctree::
   :maxdepth: 3
   :caption: Design Tutorials
   :hidden:

   Design Tutorials <./docs/Design_Tutorials/Design_Tutorials>



.. csv-table:: Feature Tutorials
   :header: "Tutorial","Platform","OS","IDE Flow","Libraries Used","HLS Kernel","x86 simulator","aie simulator","SW Emu","HW Emu","HW","Event Trace in HW","Profile in HW"
   :widths: 22, 8, 8, 6, 7, 8, 7, 7, 5, 6, 4, 6, 6

   ":doc:`Versal Custom Thin Platform Extensible System <./docs/Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System/README>`","Custom","Linux","","","MM2S / S2MM / VADD","","","","x","x",""," "
   ":doc:`LeNet Tutorial <./docs/Design_Tutorials/01-aie_lenet_tutorial/README>`","Base","Linux","","","MM2S / S2MM","","x","","x","x","","x "
   ":doc:`Super Sampling Rate FIR Filters <./docs/Design_Tutorials/02-super_sampling_rate_fir/README>`","Base","Linux","","","","","x","","","",""," "
   ":doc:`Beamforming Design <./docs/Design_Tutorials/03-beamforming/README>`","Base","Linux","","","","","x","","x","x","","x "
   ":doc:`2D-FFT <./docs/Design_Tutorials/06-fft2d_AIEvsHLS/README>`","Base","Linux","","DSPLib","PL Data Generator and Checker","","x","","x","x","","x "
   ":doc:`FIR Filter <./docs/Design_Tutorials/07-firFilter_AIEvsHLS/README>`","Base","Linux","","DSPLib","PL Data Generator and Checker","","x","","x","x","","x "
   ":doc:`N-Body Simulator <./docs/Design_Tutorials/08-n-body-simulator/README>`","Base","Linux","","","PL Datamover","","x","","x","x",""," "
   ":doc:`Versal GeMM Implementation <./docs/Design_Tutorials/10-GeMM_AIEvsDSP/README>`","Base","Linux","","DSPLib","Datamover","","x","","x","x","","x "



