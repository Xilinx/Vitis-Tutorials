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

This section of the tutorial explains how to build HLS and AIE kernels, along with host applications, for various platforms. The example uses pre-built platforms included with Vitis installations, as well as pre-built Versal common kernel and rootfs binaries generated using the PetaLinux tool flow, to enable testing on both hardware and hardware emulation.

1. [VCK190](./VCK190)
2. [VCK190_dfx](./VCK190_dfx)
3. [VEK280](./VEK280)

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
   cd <path_to_Tutorial>/Getting_Started/Vitis/Versal_w_PetaLinux/<platform_to_test>
   make all
   ```
   Please refer Makefile of each tutorial for more details on the commands.

Ensure you have the following installed on your machine:

* Vitis tools
* Versal Common Image and sysroot


<p class="sphinxhide" align="center"><sub>Copyright © 2026 Advanced Micro Devices, Inc.</sub></p>
<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
