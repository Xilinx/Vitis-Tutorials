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


# System integration and packaging with Vitis

Vitis serves both hardware and software activities. To guide through the design flow, the detailed description use link to previous and next step.

#### Overview of Vitis activities
1. Hardware integration
    1. [Compile additional Vitis kernels](./ip/README.md).
    2. Integrating [VSS component](../vss/README.md) and Vitis kernels to a [custom extensible platform](../vivado/README.md) and export to a VMA.
2. [Prepate Software / Embedded platform](./Platform.md)
    1. Generate board support package / drivers.
    2. Apply device tree overlays.
3. Packaging to delivery container
    1. [Vitis Packaging](./Packaging.md) collect binaries, boot artifacts, OS and host applications and deploy on a SD card.


## 1. Hardware integration

### Adding VSS Component and Vitis Components to an extensible platform

This part of the tutorial demonstrates adding a Vitis Subsystem component and additional Vitis PL components to demonstrate integrating both VSS and Vitis kernels onto an extensible platform.
    - [Add RTL Subtractor and HLS mm2s and s2mm Components](./ip/README.md)
    - Declare connections between VSS Component, Vitis compnents and extensible platform.
      [Inspect the config file for the example](./src/system.cfg)
    - Build System Component and export Vitis Managed Archive (VMA)

The VMA archive is generated with v++ using:
```
v++ --link --target <hw|hw_emu> --export -save-temps --platform <platform_name> --config ./src/system.cfg <list_of_xo> <VSS archive> --output <VMA file>
```

Makefiles are provided to build everything from the tutorial top folder. It will automatically compile RTL and HLS components from the [`ip` folder](./ip/README.md) as required by Vitis during linking.<br>
**Note** The VSS component need to be compiled as described in [Using VSS to develop AIE+PL Subsystem](./vss/README.md).

From top folder, run:
```
make vitis_ip vma_export
```

After the VMA export, the design is synthesized and implemented in Vivado:
[Import VMA and finalize the design in Vivado](../vivado/Finalize_Vivado.md)


## 2. Prepare Software / Embedded platform
After building the fixed Vivado platform and prepared [Linux build tools (sysroot), Image and filesystem](../linux/README.md), the device tree overlay is applied and BSP/drivers is generated. See [creating Vitis Platform component](./Platform.md)

## 3. Package to delivery container
Last step in the flow is to package the design to a suitable container. This tutorial use [Vitis Package](./Packaging.md) to create a SD card folder.

## Navigation helper
 - [Return to top](../README.md)
 - [Vitis Subsystem](../vss/README.md)
 - [Vivado hw platform](../vivado/README.md)
 - [Prepare Linux](../linux/README.md)
 - [Host applications](../ps_apps/README.md)
 - [Vitis sw platform](./Platform.md)
 - [Vitis Package](./Packaging.md)





<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2025 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
