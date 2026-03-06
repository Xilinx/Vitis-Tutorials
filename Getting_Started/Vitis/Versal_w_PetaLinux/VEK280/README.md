<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ Getting Started Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
    </td>
  </tr>
</table>


# Vitis Introduction and Getting Started Tutorial

***Version: Vitis 2025.2***

Welcome to Vitis Getting Started!

This tutorial showcases the important steps to build the HLS and AIE kernel dedicated for AIE-ML architecture along with a Host application and running the design on the hardware and hardware emulation.

This tutorial uses pre-built xilinx_vek280_base_202520_1.xpfm from Vitis installation path to compile HLS and AIE kernels.

Please go through the Makefile provided in this tutorial to better understand the Vitis tool flow. At the end of each section, a link to the next chapter is included.

**Note: Most of the documentation below is Generic and platform independent.**

1. [Understanding the Vitis Tool Flow](../../Vitis_ToolFlow.md)
2. [Getting Started with Designing using the Vitis Tool](../../Design_Overview.md)
3. [Running the Hardware Emulation](../../Running_the_Hardware_Emulation.md)
4. [Running the design on a VEK280 Evaluation Board](../../Hardware_Run.md)

## Prerequisites

1. Source Vitis 2025.2:

   ```
   source <path_to_vitis_install>/settings64.sh
   ```

2. Export the following variable:

   ```
   export COMMON_IMAGE_VERSAL=<path_to_common_image: xilinx-versal-common-v2025.2>
   ```

3. To compile the binaries:

   ```
   cd <path_to_Tutorial>/Getting_Started/Vitis/Versal_w_PetaLinux/VEK280/
   make all
   ```
   Please refer [Makefile](./Makefile) for more details on the commands.

Ensure you have the following installed on your machine:

* Vitis tools
* Versal AI Edge Series VEK280 Evaluation Kit
* Common Image

**NOTE: Hardware run flow for VEK280 is as same as VCK190 board flow. Please refer steps documented for [VCK190 board hardware run](../VCK190_dfx/README.md).**

<p class="sphinxhide" align="center"><sub>Copyright © 2026 Advanced Micro Devices, Inc.</sub></p>
<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
