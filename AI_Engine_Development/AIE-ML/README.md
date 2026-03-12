<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">See Vitis™ AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# AI Engine for Machine Learning (AIE-ML) Development

## Introduction

The tutorials under the AI Engine for Machine Learning (AIE-ML) Development help you learn how to target, develop, and deploy advanced algorithms using an AMD Versal™ AIE-ML array with PL IP/kernels and software applications running on embedded processors.

- The AIE-ML Development [**Feature Tutorials**](./Feature_Tutorials/) highlight specific features and flows that help develop AI Engine-ML applications.

- The AIE-ML Development [**Design Tutorials**](./Design_Tutorials/) showcase the two major phases of AI Engine-ML application development: architecting the application and developing the kernels. These tutorials demonstrate both phases.

>**IMPORTANT**: Before beginning the tutorial, install the AMD Vitis™ unified software platform 2025.2. This Vitis release includes all the embedded base platforms including the VCK190 base platform that this tutorial uses. Also, download the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/amd/en/downloadNav/embedded-platforms.html).

The `common image` package contains a prebuilt Linux kernel and root file system that you can use with Versal boards for embedded design development using the Vitis software platform.

Before starting a tutorial, run the following steps:

1. Go to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the `/Common Images Dir/xilinx-versal-common-v2025.2/environment-setup-cortexa72-cortexa53-amd-linux` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you must run `/Common Images Dir/xilinx-versal-common-v2025.2/sdk.sh`.
3. Set up your ROOTFS and IMAGE to point to the `rootfs.ext4` and Image files located in the `/Common Images Dir/xilinx-versal-common-v2025.2` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to `$XILINX_VITIS/base_platforms`.

### Feature Tutorials

These tutorials target the **VEK280** board. The following table lists the tutorials available, and the features and flows showcased in these tutorials. The various columns correspond to specific features/flows supported in these tutorials and help you identify tutorials that showcase specific flows and features that you are interested in.

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
 <td width="7%" align="center"><b>HW Emu</b>
 <td width="7%" align="center"><b>HW</b>
 <td width="7%" align="center"><b>Event Trace in HW</b>
 <td width="7%" align="center"><b>Profile in HW</b>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/01-aie_a_to_z/">A to Z Bare-metal Flow</a></td>
 <td>Custom</td>
 <td>Baremetal</td>
 <td>Yes</td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>Yes</td>
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
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/05-AI-engine-versal-integration/">AIE Versal Integration</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td>Yes</td>
 <td></td>
 <td>MM2S / S2MM</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/13-aie-ml-performance-analysis/">AIE-ML Performance Analysis</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
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
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/06-gemm/">Matrix Compute with Vitis Libraries</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td>DSPLib</td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/07-tiling-parameters/">Tiling Parameter Programming</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
 </table>

### Design Tutorials

These tutorials target the **VEK280** board. The following table lists the tutorials available, and the features and flows showcased in these tutorials. The various columns correspond to specific features/flows supported in these tutorials. Use them to identify tutorials that showcase specific flows and features that you are interested in.

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
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/02-Prime-Factor-FFT/">Prime Factor FFT-1008 on AIE-ML</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/03-AIE-ML-lenet_tutorial/">AIE-ML LeNet Tutorial</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td>MM2S / S2MM</td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/04-AIE-API-based-FFT-for-many-instances-applications/">AIE API based FFT for Many Instances Applications</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td> </td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/05-Softmax-Function/">Softmax Function on AIE-ML</a></td>
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
 </tr>  
 <tr>
 <td align="center"><a href="./Design_Tutorials/06-farrow_filter/">Migrating Farrow Filter from AIE to AIE-ML</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td> </td>
 <td> </td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td> </td>
 </tr> 
 <tr> 
 <td align="center"><a href="./Design_Tutorials/07-Channelizer-Using-Vitis-Libraries/">Polyphase Channelizer on AIE-ML using Vitis Libraries</a></td>
 <td>Base</td>
 <td>Linux</td>
 <td> </td>
 <td>DSPLib</td>
 <td>DataMover</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td>Yes</td>
 <td> </td>
 <td>Yes</td>
 </tr>  
 <tr>
 <td align="center"><a href="./Design_Tutorials/08-MNIST-ConvNet/">MNIST ConvNet on AIE-ML</a></td>
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
 </tr>  
 </table>

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
