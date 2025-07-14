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


# HLS, RTL and AIE component folder for VSS

The Makefile in this folder automatically assumes subfolders at this level are components to be added to the VSS component.
Each folder have their own Makefiles and are self contained, but may have parameters overriden by calling Makefile.
This makes it easy to control modularity and portability so that any RTL or HLS component can be moved between this folder and the [vitis/ip folder](../../vitis/ip) depending on which level the component is to be added.
The xo_hw folder is excluded from the search, as this is where RTL and HLS kernel output .xo will be generated.
The AIE build is treated separately and will result in a libadf.a file located in aie folder.

## Adding or removing HLS and RTL components
A component can be removed from the VSS region by deleting the folder and subfolders, and ensure the corresponding .xo file is removed from [xo_hw](./xo_hw) folder

To add a new component, it's easiest to copy one of the below examples and add the corresponding sources files and modify the Makefile and config files accordingly.
**Note** Make sure to update the VSS link configuration according to the changes: [../src/vss_conn.cfg](../src/vss_conn.cfg)

Components used for the VSS region:
| Component                        | Type   | Description
| ---------------------------------|--------|-------------------------------------------------
| [counter](./counter/README.md)   | RTL    | Freerunning counter to generate input stimuli for datamovers
| [axis1to2](./axis1to2/README.md)| HLS    | Duplicating one axi stream to two ports to copy the inputs to each AIE FIR filter.
| [vadd_s](./vadd_s/README.md)  | HLS    | Vector adder taking complex int16 stream inputs. Used to demo Python VFS
| [aie](./aie/README.md)        | AIE    | Top graph consisting of a subgraph with datamover kernels and a subgraph with two FIR filter kernels

[Continue with VSS Component](../README.md)



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2025 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
