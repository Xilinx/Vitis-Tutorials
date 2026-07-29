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


# Creating Vitis software platform component

This tutorial now use SDTgen/Lopper to extract device tree and lop processor domains. See [sdtgen](../linux/sdtgen/README.md)
The zocl and other overlays are now applied by [Vitis package](Package.md)

Vitis platform component to modify `system.dtb` for zocl overlay through calling SDTgen and lopper tools.
Secondary, it adds a baremetal domain, including BSP drivers and header files that translate AIE and PL addresses to aliases usable by baremetal host application.

***Note:*** The tutorial has baremetal preparade but not yet implemented. The mechanism for generating BSP will be added when the baremetal option is completed.

The Vitis platform component is generated with Vitis CLI using:
```
vitis -s vitis_platform.py
```


## Description of generated Vitis platform component folders

| Folder                           | Type   | Description
| ---------------------------------|--------|-------------------------------------------------
| build_pfm                                   | Generated | Work folder for Vitis.
| build_pfm/pfm                               | Generated | Platform component.
| build_pfm/pfm/export/pfm/hw/sdt/system.dtb  | Generated | Updated devicetree blob.
| build_pfm/pfm/export/pfm/hw/                | Generated | Exported platform component hardware related files.
| build_pfm/pfm/export/pfm/sw/                | Generated | Exported platform component software related files.
| build_pfm/pfm/export/pfm/sw/boot/           | Generated | Boot artifacts copied from linux image folder. Not used here.
| build_pfm/pfm/export/pfm/sw/linux_xrt/      | Generated | `boot.scr` copied from linux image folder. Not used here.
| build_pfm/pfm/export/pfm/sw/standalone/     | Generated | BSP and drivers for baremetal domain. Used by baremetal host app.



## Navigation helper
  - [Return to top](../README.md)



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2026 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/legal/copyright.html">Terms and Conditions</a></sup></p>
