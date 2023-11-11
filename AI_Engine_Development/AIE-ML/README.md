<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1> AIE-ML Development </h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

## Introduction

The tutorials under the AI Engine for Machine Learning (AIE-ML) Development help you learn how to target, develop, and deploy advanced algorithms using a Versal AIE-ML array in conjunction with PL IP/kernels and software applications running on the embedded processors. 

- The AIE-ML Development <a href="./Feature_Tutorials/">**Feature Tutorials**</a> highlight specific features and flows that help develop AI Engine-ML applications. 

- The AIE-ML Development <a href="./Design_Tutorials/">**Design Tutorials**</a> showcase the two major phases of AI Engine-ML application development: architecting the application and developing the kernels. Both these phases are demonstrated in these tutorials.

>**IMPORTANT**: Before beginning the tutorial make sure you have installed the AMD Vitis™ 2023.2 software. The Vitis release includes all the embedded base platforms including the VEK280 ES1 base platform that is used in these tutorials. In addition, ensure you have downloaded the Common Images for Embedded Vitis Platforms from [Downloads](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html).

The ‘common image’ package contains a prebuilt Linux kernel and root file system that can be used with the AMD Versal™ board for embedded design development using the Vitis software platform.

Before starting a tutorial, run the following steps:

1. Go to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the ``/Common Images Dir/xilinx-versal-common-v2023.2/environment-setup-cortexa72-cortexa53-xilinx-linux`` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you must run the ``/Common Images Dir/xilinx-versal-common-v2023.2/sdk.sh``.
3. Set up your ROOTFS and IMAGE to point to the ``rootfs.ext4`` and Image files located in the ``/Common Images Dir/xilinx-versal-common-v2023.2`` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to ``$XILINX_VITIS/base_platforms``.

Note: These tutorials target VEK280 ES1 board or build custom board using ES devices, which are subject to a special license, please obtain a license for using Beta Devices in AMD tools, and make sure it's enabled by executing `enable_beta_device` or add it to the tools initial tcl files. 

### Feature Tutorials
These tutorials target the **VEK280 ES1** board. The table below lists the tutorials available, and the features and flows showcased in all of these tutorials. The various columns correspond to specific features/flows supported in these tutorials and will help you identify tutorials that showcase specific flows and features that you are interested in.

 <table style="width:100%">
 <tr>
 <td width="7%" align="center"><b>Tutorial</b>
 <td width="7%" align="center"><b>Platform</b>
 <td width="7%" align="center"><b>OS</b>
 <td width="7%" align="center"><b>IDE Flow</b>
 <td width="7%" align="center"><b>Libraries Used</b>
 <td width="7%" align="center"><b>HLS Kernel</b>
 <td width="7%" align="center"><b>x86 simulator</b>
 <td width="7%" align="center"><b>aie simulator</b>
 <td width="7%" align="center"><b>SW Emu</b>
 <td width="7%" align="center"><b>HW Emu</b>
 <td width="7%" align="center"><b>HW</b>
 <td width="7%" align="center"><b>Event Trace in HW</b>
 <td width="7%" align="center"><b>Profile in HW</b>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/01-aie_a_to_z/">A to Z Bare-metal Flow</a></td>
 <td>Custom</td>
 <td>Baremetal</td>
 <td>Vivado &<br>Vitis IDE</td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/02-using-gmio/">Using GMIO with AIE</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/03-rtp-reconfiguration/">Runtime Parameter Reconfiguration</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/04-packet-switching/">Packet Switching</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/05-AI-engine-versal-integration/">AIE Versal Integration</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td>CLI / Vitis Unified IDE</td>
 <td></td>
 <td>MM2S / S2MM</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/20-aiecompiler-features/">AIE Compiler Features</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 </tr>
 </table>

### Design Tutorials
These tutorials target the **VEK280 ES1** board. The table below lists the tutorials available, and the features and flows showcased in all of these tutorials. The various columns correspond to specific features/flows supported in these tutorials and will help you identify tutorials that showcase specific flows and features that you are interested in.

 <table style="width:100%">
 <tr>
 <td width="7%" align="center"><b>Tutorial</b>
 <td width="7%" align="center"><b>Platform</b>
 <td width="7%" align="center"><b>OS</b>
 <td width="7%" align="center"><b>IDE Flow</b>
 <td width="7%" align="center"><b>Libraries Used</b>
 <td width="7%" align="center"><b>HLS Kernel</b>
 <td width="7%" align="center"><b>x86 simulator</b>
 <td width="7%" align="center"><b>aie simulator</b>
 <td width="7%" align="center"><b>SW Emu</b>
 <td width="7%" align="center"><b>HW Emu</b>
 <td width="7%" align="center"><b>HW</b>
 <td width="7%" align="center"><b>Event Trace in HW</b>
 <td width="7%" align="center"><b>Profile in HW</b>
 </tr>
 <tr>
 <td align="center"><a href="../../Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System/">Versal Custom Thin Platform Extensible System</a></td>
 <td>Custom</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM / VADD</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/01-AIE-ML-programming-and-optimization/">AIE-ML Programming</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
 </table>



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
