<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ In-Depth Tutorials</h1>
    </td>
 </tr>
</table>


# Updating device tree overlays manually

This method uses `device-tree-xlnx` from GitHub and tcl scripts to build dts from the supplied fixed xsa.


To update the device tree from linux/dtg, run:
```
make all
```

Or calling Makefiles from top folder:
```
make dtg
```

## Description of device tree generation related folders

| Folder                           | Type   | Description
| ---------------------------------|--------|-------------------------------------------------
| [src](./src)          | Sources | Tcl scripts and custom dtsi.
| [device-tree-xlnx](./device-tree-xlnx/)           | Generated | Clone of device-tree-xlnx repo from GitHub
| [dts](./dts)          | Generated | DTS outputs

***Note:*** *To use the generated dts, manually override the dts variable in v++ package step.*

## Next step
  - [8. Compiling PS host applications](../../ps_apps/README.md)



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2025 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
