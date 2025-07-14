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


# Preparing a custom Linux environment

This section setup and builds a Linux environment using the fixed XSA from:
[4. Import VMA and finalize the design in Vivado](../vivado/Finalize_Vivado.md)

## Principles for building Linux with Petalinux
The makefile is prepared to step by step download and configure the linux environment.

 - Create a petalinux project for versal.
 - Update the project with the fixed XSA as hardware description.
 - Copy custom user overlay for device tree to the project specification.
 - Enable/Disable which drivers and libraries to use.
 - If static IP address is used, copy the ethernet settings to the project specification, otherwise use DHCP assigned IP addresses.
 - Build the petalinux project.
 - Create boot artifacts using petalinux package.
 - Download and configure sysroot.
 - Dump system device tree blob to human readable dts format.

By updating the configuration, a user can pick and choose which drivers and packages to install to the linux file system.
See the [petalinux/Makefile](./petalinux/Makefile) rule `update_config` which ones are added.

### Description of file structure

| Folder                           | Type   | Description
| ---------------------------------|--------|-------------------------------------------------
| [dtg](./dtg/)                    | Source | System Devicetree and Lopper tools for testing/debugging.
| [petalinux](./petalinux/)        | Source | Recipe and build scripts for Petalinux flow.
| [src](./src/)                    | Source | Custom user overlays. (bif, bsp, and core recipes)
| [yocto](./yocto/)                | Source | Recipe and build scripts for Yocto flow.
| sysroot                          | Generated | Crosscompiler tools.
| vck190-versal                    | Generated | Image, rootfs, boot files, and devicetree overlays.

## Navigation helper
 - [Next step - pdate system device tree with Vitis platform component](../vitis/Platform.md)
 - [Previous step - Import VMA and finalize the design in Vivado](../vivado/Finalize_Vivado.md)
 - [Return to top](../README.md)


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2025 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
