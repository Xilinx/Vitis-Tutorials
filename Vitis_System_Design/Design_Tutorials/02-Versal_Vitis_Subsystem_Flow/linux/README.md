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


# Creating a Linux image with custom machine

This section setup and builds a Linux environment using the fixed XSA from:
[4. Import VMA and finalize the design in Vivado](../vivado/Finalize_Vivado.md)

## Principles for building Linux
The makefile is prepared to step by step download and configure the linux environment.

 - Generate devicetree overlays with SDTGen and extract a processor domain with Lopper using the fixed XSA.
 - Create a Linux project for Versal using either Yocto/EDF or Petalinux, downloading the tools to temporary drive.
 - Create a custom machine based on the device hardware description from SDTGen/Lopper.
 - Copy custom user overlay for device tree to the project specification.
 - Enable/Disable which drivers and libraries to use.
 - If static IP address is used, copy the ethernet settings to the project specification, otherwise use DHCP assigned IP addresses.
 - Build the Linux project.
 - **Petalinux only:** Create boot artifacts using Petalinux package.
 - Download and configure SDK for host applications, then install it to `sysroot` folder.

***Note:*** The tutorial is currently setup to use EDF as default option.
The legacy Petalinux build option has not yet been aligned with the SDTGen/Lopper stand alone approach.
It uses Vitis Platform component to do that, but with EDF and custom machine it fits better to use SDTGen/Lopper from command line.


### Choosing optional packages for Petalinux
By updating the configuration, a user can pick and choose which drivers and packages to install to the linux file system.
See the [petalinux/Makefile](./petalinux/Makefile) rule `update_config` which ones are added.



### Description of file structure

| Folder                           | Type   | Description
| ---------------------------------|--------|-------------------------------------------------
| [sdtgen](./sdtgen/)              | Source | System Devicetree and Lopper tools.
| [petalinux](./petalinux/)        | Source | Recipe and build scripts for Petalinux flow.
| [src](./src/)                    | Source | Custom user overlays organized per board. (bif, bsp, and core recipes)
| [yocto](./yocto/)                | Source | Recipe and build scripts for Yocto/EDF flow.
| my-<board_name>-versal                 | Generated | Image deploy folder containing rootfs and boot files.
| my-<board_name>-versal-meta            | Generated | Optional custom layers used by Yocto.
| sysroot                          | Generated | Crosscompiler tools.

## Navigation helper
 - [Running SDTgen](./sdtgen/README.md)
 - [Custom user overlays](./src/README.md)
 - [Yocto/EDF Flow](./yocto/README.md)
 - [Previous step - Import VMA and finalize the design in Vivado](../vivado/Finalize_Vivado.md)
 - [Return to top](../README.md)


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2026 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/legal/copyright.html">Terms and Conditions</a></sup></p>
