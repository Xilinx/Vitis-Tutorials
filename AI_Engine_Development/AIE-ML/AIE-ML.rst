##############################################################################
AI Engine for Machine Learning Development
##############################################################################


.. sidebar:: More Information

   See AMD Vitis™ Development Environment on `xilinx.com <https://www.xilinx.com/products/design-tools/vitis.html>`_

The tutorials under the AI Engine for Machine Learning (AIE-ML) Development help you learn how to target, develop, and deploy advanced algorithms using a Versal AIE-ML array in conjunction with PL IP/kernels and software applications running on the embedded processors.


* :doc:`Feature Tutorials <./Feature_Tutorials/Feature_Tutorials>` highlight specific features and flows that help develop AI Engine-ML applications.
* :doc:`Design Tutorials <./Design_Tutorials/Design_Tutorials>` showcase the two major phases of AI Engine-ML application development: architecting the application and developing the kernels. Both phases are demonstrated in these tutorials.


.. important:: 

   Before beginning a tutorial, ensure you have installed the Vitis 2024.1 software. The Vitis release includes all the embedded base platforms, including the VEK280 base platform that is used in these tutorials. In addition, ensure you have downloaded the Common Images for Embedded Vitis Platforms from `Downloads <https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html>`_.

The `common image` package contains a prebuilt Linux kernel and root file system that can be used with the AMD Versal™ board for embedded design development using the Vitis software platform.

Before starting a tutorial, run the following steps:

1. Go to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the ``/Common Images Dir/xilinx-versal-common-v2024.1/environment-setup-cortexa72-cortexa53-xilinx-linux`` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you must run ``/Common Images Dir/xilinx-versal-common-v2024.1/sdk.sh``.
3. Set up your ROOTFS and IMAGE to point to the ``rootfs.ext4`` and Image files located in the ``/Common Images Dir/xilinx-versal-common-v2024.1`` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to ``$XILINX_VITIS/base_platforms``.


******************************************
Feature Tutorials
******************************************


These tutorials target the **VEK280** board. The table below lists the tutorials available, and the features and flows showcased in all of these tutorials. The various columns correspond to specific features/flows supported in these tutorials and will help you identify tutorials that showcase specific flows and features that you are interested in.


.. toctree::
   :maxdepth: 3
   :caption: Feature Tutorials
   :hidden:

   Feature Tutorials <./Feature_Tutorials/Feature_Tutorials.rst>

.. csv-table:: Feature Tutorials
   :header: "Tutorial","Platform","OS","IDE Flow","Libraries Used","HLS Kernel","x86 simulator","aie simulator","SW Emu","HW Emu","HW","Event Trace in HW","Profile in HW"
   :widths: 22, 8, 8, 6, 7, 8, 7, 7, 5, 6, 4, 6, 6

   ":doc:`A to Z Bare-metal Flow <./Feature_Tutorials/01-aie_a_to_z/README>`","Custom","Baremetal","Vivado, Vitis IDE","","MM2S / S2MM","","Yes","","Yes","Yes","",""
   ":doc:`Using GMIO with AIE <./Feature_Tutorials/02-using-gmio/README>`","Base","Linux","","","","","Yes","","Yes","Yes","","Yes"
   ":doc:`Runtime Parameter Reconfiguration <./Feature_Tutorials/03-rtp-reconfiguration/README>`","Base","Linux","","","MM2S / S2MM","","Yes","","Yes","Yes","",""
   ":doc:`Packet Switching <./Feature_Tutorials/04-packet-switching/README>`","Base","Linux","","","MM2S / S2MM","","Yes","","Yes","Yes","",""
   ":doc:`AIE Versal Integration <./Feature_Tutorials/05-AI-engine-versal-integration/README>`","Base","Linux","CLI, Vitis Unified IDE","","MM2S / S2MM","Yes","Yes","Yes","Yes","Yes","",""
   ":doc:`AIE-ML Performance Analysis <./Feature_Tutorials/13-aie-ml-performance-analysis/README>`","Base","Linux","","","","","Yes","","Yes","Yes","","Yes"
   ":doc:`AIE Compiler Features <./Feature_Tutorials/20-aiecompiler-features/README>`","Base","Linux","","","MM2S / S2MM","Yes","Yes","","Yes","Yes","Yes","Yes"


******************************************
Design Tutorials
******************************************

These tutorials target the **VEK280** board. The table below lists the tutorials available, and the features and flows showcased in all of these tutorials. The various columns correspond to specific features/flows supported in these tutorials and will help you identify tutorials that showcase specific flows and features that you are interested in.

.. toctree::
   :maxdepth: 3
   :caption: Design Tutorials
   :hidden:

   Design Tutorials <./Design_Tutorials/Design_Tutorials.rst>



.. csv-table:: Design Tutorials
   :header: "Tutorial","Platform","OS","IDE Flow","Libraries Used","HLS Kernel","x86 simulator","aie simulator","SW Emu","HW Emu","HW","Event Trace in HW","Profile in HW"
   :widths: 22, 8, 8, 6, 7, 8, 7, 7, 5, 6, 4, 6, 6

   ":doc:`Versal Custom Thin Platform Extensible System <./Design_Tutorials/01-Versal_Custom_Thin_Platform_Extensible_System/README>`","Custom","Linux","","","MM2S / S2MM / VADD","","","","Yes","Yes","",""
   ":doc:`AIE-ML Programming <./Design_Tutorials/01-AIE-ML-programming-and-optimization/README>`","Base","Linux","","","","Yes","Yes","","","","",""

