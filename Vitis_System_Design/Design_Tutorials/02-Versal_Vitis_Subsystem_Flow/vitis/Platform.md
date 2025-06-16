<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ In-Depth Tutorials</h1>
    </td>
 </tr>
</table>


# Creating Vitis software platform component

Primarily, this step use the automated mechanism of Vitis platform component to modify `system.dtb` for zocl overlay through calling SDTgen and lopper tools.
Secondary, it adds a baremetal domain, including BSP drivers and header files that translate AIE and PL addresses to aliases usable by baremetal host application.

***Note:*** *This also pulls hardware platform related files and Linux artifacts into the Vitis platform component work folder, but in this case we will just use the modified `system.dtb`.*

Alternatively the `system.dtb` can be updated manually using xsct and lopper, see [devicetree generation overlay](../linux/dtg).

The Vitis platform component is generated with Vitis CLI using:
```
vitis -s vitis_platform.py
```

Or calling Makefiles from top folder:
```
make vitis_platform
```

## Description of generated Vitis platform component folders

| Folder                           | Type   | Description
| ---------------------------------|--------|-------------------------------------------------
| [build_pfm/](./build_pfm/)          | Generated | Work folder for Vitis.
| [build_pfm/pfm](./build_pfm/pfm/)          | Generated | Platform component.
| [build_pfm/pfm/export/pfm/hw/sdt/system.dtb](./build_pfm/pfm/export/pfm/hw/sdt/system.dtb)          | Generated | Updated devicetree blob.
| [build_pfm/pfm/export/pfm/hw/](./build_pfm/pfm/export/pfm/hw/)          | Generated | Exported platform component hardware related files.
| [build_pfm/pfm/export/pfm/sw/](./build_pfm/pfm/export/pfm/sw/)          | Generated | Exported platform component software related files.
| [build_pfm/pfm/export/pfm/sw/boot/](./build_pfm/pfm/export/pfm/sw/boot/)          | Generated | Boot artifacts copied from linux image folder. Not used here.
| [build_pfm/pfm/export/pfm/sw/linux_xrt/](./build_pfm/pfm/export/pfm/sw/linux_xrt/)     | Generated | `boot.scr` copied from linux image folder. Not used here.
| [build_pfm/pfm/export/pfm/sw/standalone/](./build_pfm/pfm/export/pfm/sw/standalone/)       | Generated | BSP and drivers for baremetal domain. Used by baremetal host app.



## Navigation helper
  - [Next step - Compiling PS host applications](../ps_apps/README.md)
  - [Previous step - Configure and build custom Linux](../linux/README.md)
  - [Return to top](../linux/README.md)



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2025 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
