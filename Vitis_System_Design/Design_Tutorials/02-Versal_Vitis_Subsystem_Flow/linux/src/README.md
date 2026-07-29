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


# Board specific settings and device tree overrides

This folder contains board related settings and device tree overrides. They are collected from 
[System Device Tree - Board DTSI](https://github.com/Xilinx/system-device-tree-xlnx/tree/xlnx_rel_v2026.1/device_tree/data/kernel_dtsi/2026.1)
 and placed here to allow user modifications and custom adaptions for demonstration purpose.




Each supported board have a package_zocl folder which is arranged with files needed to create the `zocl.dtsi` zip file used either for creating prebuilt EDF artifacts or used with `v++ package` to setup correct BOOT.bin and devicetree overlays.




### Description of files in package_zocl folder
The files listed represents the vck190 board example. See [./vck190/package_zocl](./vck190/package_zocl) to inspect each file.

| Folder                           | Type   | Description
| ---------------------------------|--------|-------------------------------------------------
| domain.yaml                      | Source | Description of processor domains and memory reservations.
| zocl-template.dtsi               | Source | Template for zyxclmm_drm node declarations.
| versal-vck190-reva.dtsi          | Source | Board DTSI. Only one DTSI file may exist here, any extra dtsi needs to be in the include folder.
| include/versal-vmk180-reva.dtsi  | Source | Common DTSI settings included by `versal-vck190-reva.dtsi`
| include/gpio/gpio.h              | Source | Include for GPIO masks.

## Navigation helper
 - [Return to Creating a Linux image with custom machine](../README.md)
 - [Return to top](../README.md)


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2026 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>

