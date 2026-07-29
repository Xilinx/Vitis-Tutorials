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


# HLS and RTL component folder for Vitis

The Makefile in this folder automatically assumes subfolders at this level are components to be added to the Vitis platform.
Each folder have their own Makefiles and are self contained, but may have parameters overridden by calling Makefile.
This makes it easy to control modularity and portability so that any RTL or HLS component can be moved between this folder and the [vss/ip folder](../../vss/ip) depending on which level the component is to be added.
The xo_hw folder is excluded from the search, as this is where RTL and HLS kernel output .xo will be generated.
The AIE build is treated separately and will result in a libadf.a file located in aie folder.

## Adding or removing HLS and RTL components
A component can be removed from the VSS region by deleting the folder and subfolders, and ensure the corresponding .xo file is removed from `xo_hw` folder.

To add a new component, it's easiest to copy one of the below examples and add the corresponding sources files and modify the Makefile and config files accordingly.
**Note** Make sure to update the Vitis link configuration according to the changes: [../src/system.cfg](../src/system.cfg)

Components used for the Vitis region:
| Component                        | Type   | Description
| ---------------------------------|--------|-------------------------------------------------
| [subtractor](./subtractor/README.md)| RTL    | A subtractor acting as endpoint of the counter values passed through AIE datamover kernels. Only two channels are uses via AIE, the others are connected directly to the counter.
| mm2s                                | HLS    | Converts data from DDR memory buffer to the FIR DSP via the AXI Stream data duplicator. The PS application can be used to create the stimuli buffer.
| s2mm                                | HLS    | Converts the AXI Stream output from the vadd_s to DDR memory. Allows the PS application to analyze the output.


[Continue with Vitis integration](../README.md)



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2026 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/legal/copyright.html">Terms and Conditions</a></sup></p>
