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


# Compile and build host applications executables

The host applications are organized per domain. Currently only Linux user space host application is available.
A similar structure can be setup for bare metal applications, with the `exe` folder being replaced by an `elf` folder.

To compile and build linux executables, call Makefile from top folder:

```
make ps_apps
```

## Description of generated Vitis platform component folders

Each application is selfcontained per folder to make it easy to transfer and reused in other designs.


| Folder                                       | Type       | Description                                                                                                  |
|----------------------------------------------|------------|--------------------------------------------------------------------------------------------------------------|
| [linux/Makefile](./linux/Makefile)           | Source     | Generalized recipe to find and compile all application folders.                                              |
| [linux/exe](./linux/exe)                     | Generated  | Destination of all application executables. This folder is searched by the packager to automatically add them to the SD card. |
| [linux/aie_dly_test/](./linux/aie_dly_test/) | Workspace  | Test application for a small AI Engine + PL system.                                                          |
| [linux/aie_dly_test/src](./linux/aie_dly_test/src) | Source     | Application source code folder.                                                                              |
| [linux/aie_dly_test/Makefile](./linux/aie_dly_test/Makefile) | Source     | Application makefile.                                                                                        |


## Next step

  - [9. Package the design to SD card](../vitis/Package.md)



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2025 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
