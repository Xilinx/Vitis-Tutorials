<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ System Design Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
    </td>
  </tr>
</table>


# Versal Custom Thin Platform Extensible System

***Version: Vitis 2026.1***

This long-standing tutorial design has been used to explain and demonstrate Vivado and Vitis platform-based methodology.

A few releases ago, the [02-Vitis Subsystem Tutorial](../02-Versal_Vitis_Subsystem_Flow) was derived from this tutorial's building blocks and the experience gained from using them.
The newer tutorial uses the Vitis Export to Vivado flow by default. Its workflow was substantially revised to align software build environment to be consistent between Linux and baremetal systems, using the implemented XSA rather than the extensible XSA.

While investigating how to document this new workflow, I realized that the purpose of several useful features in this system design tutorial was not adequately described.
The README was expanded to cover major steps and folders and to provide references to relevant sections of the user guides.

For the 2026.1 tool release, the tutorial has been migrated to use [AMD® Embedded Development Framework (EDF)](https://edf.docs.amd.com/en/latest/index.html) by default.

Rather than reworking and reordering this tutorial as in the [02-Vitis Subsystem Tutorial](../02-Versal_Vitis_Subsystem_Flow), I added an option for selecting the Vitis flow that builds the fixed XSA.
This change also simplifies the addition of a hardware-emulation build option in a future update.

/ Derek Hagen

## Getting Started
Navigate to the [02-Vitis Subsystem Tutorial](../02-Versal_Vitis_Subsystem_Flow) and run
```
make all XILINX_VITIS=<Vitis_Install_Path> BOARD_NAME=<vck190|vek280|vck190seg|vek385> VITIS_FLOW=vitis
```

### Build Flow
The Vitis Integrated Flow builds the design in the following order:
```
  1.  version_check:   Checks whether the Vivado, Vitis, and (if used as build option) PetaLinux tools are setup and whether the versions are 2026.1
  2.  vss:             Builds Vitis Subsystem (AI Engine, HLS, and RTL) using a device part number
  3.  vivado_platform: Builds the thin platform XSA using the same device part number(only pre-synth)
  4.  vitis_flow_impl: Links all kernels and VSS in the thin platform and fully implement it
  5.  sdtgen:          Builds the System Device Tree using the fixed XSA
  6.  linux:           Builds the Linux and a sysroot (with EDF/Yocto)
  7.  ps_apps:         Builds all XRT-based PS applications using the sysroot
  8.  package:         Packages all necessary boot artifacts, overlays and PDIs
  9.  sdcard_image:    Creates a WIC image for the SD card, including updating BOOT.bin, XCLBIN, and user applications
```


## Revision History

Refer to [Previous version (2025.2) of the Versal Custom Thin Platform](https://github.com/Xilinx/Vitis-Tutorials/tree/2025.2/Vitis_System_Design/Design_Tutorials/01-Versal_Custom_Thin_Platform_Extensible_System)


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2026 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/legal/copyright.html">Terms and Conditions</a></sup></p>
