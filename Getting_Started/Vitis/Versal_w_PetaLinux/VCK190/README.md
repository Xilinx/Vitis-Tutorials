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

This tutorial discusses the important concepts of the Vitis tool flow, building the components, building the design and running the design on the hardware and hardware emulation.

This tutorial uses pre-built xilinx_vck190_base_202520_1.xpfm from Vitis installation path to compile HLS and AIE kernels.

Please read the tutorial in the same order as listed here to better understand the Vitis tool flow. At the end of each section, a link to the next chapter is included.

***Note: Most of the documentation below is Generic and platform independent. 

1. [Understanding the Vitis Tool Flow](../../Vitis_ToolFlow.md)
2. [Getting Started with Designing using the Vitis Tool](../../Design_Overview.md)
3. [Running the Hardware Emulation](../../Running_the_Hardware_Emulation.md)
4. [Running the design on a VCK190 Evaluation Board](../../Hardware_Run.md)

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
   cd <path_to_Tutorial>/Getting_Started/Vitis/Versal_w_PetaLinux/VCK190/
   make all
   ```
   Please refer [Makefile](./Makefile) for more details on the commands.

Ensure you have the following installed on your machine:

* Vitis tools
* Versal AI Core Series VCK190 Evaluation Kit
* Common Image


<p class="sphinxhide" align="center"><sub>Copyright © 2026 Advanced Micro Devices, Inc.</sub></p>
<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
