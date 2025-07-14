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
1.  Hardware integration
    1. [Compile additional Vitis kernels](./ip/README.md).
    2. Integrating [VSS component](../vss/README.md) and Vitis kernels to a [custom extensible platform](../vivado/README.md) and export to a VMA.
2. Software / Embedded
    1. Generate board support package / drivers.
    2. Apply device tree overlays.
3. Packaging to delivery container
    1. Collect binaries, boot artifacts, OS and host applications and deploy on a SD card.


## Hardware integration

This step assumes the following pre-requisite steps is completed:
Create VSS component   | cint16


### Adding VSS Component and Vitis Components to an extensible platform

This part of the tutorial demonstrates adding a Vitis Subsystem component and additional Vitis PL components to demonstrate integrating both VSS and Vitis kernels onto an extensible platform.


## Vitis design description
The design reuses blocks from previous tutorials to make it easier to compare the mechanics for adding and integrating them to a small system.


The example will go through the steps of creating a small AMD Versal™ VCK190 System Example Design consisting of:
 - [3. Importing and integrating DSP Subsystem to Vitis Unified IDE](./README_INTEGRATE.md)
    - [Add RTL Subtractor and HLS mm2s and s2mm Components](./ip/README.md)
    - Declare connections between VSS Component, Vitis compnents and extensible platform. 
      [Inspect the config file for the example](../vss/src/vss_conn.cfg)
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


## Navigation helper
 - [Next step - Import VMA and finalize the design in Vivado](../vivado/Finalize_Vivado.md)
 - [Previous step - Develop Custom Vivado Extensible Platform](../vivado/README.md)
 - [Return to top](../README.md)





<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2025 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
