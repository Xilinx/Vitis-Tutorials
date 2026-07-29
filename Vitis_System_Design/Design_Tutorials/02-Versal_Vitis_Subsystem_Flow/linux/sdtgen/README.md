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

# Generate devicetree with SDTGen and extract a processor domain with Lopper
This step extracts the devicetree information from the implemented fixed hardware XSA and prepares processor domains.
It creates a custom DTS using default setup from a board DTS combined with custom user overlays targeting the fixed hw platform.

With lopper, the processor domain is configured using the lop file instructions and the custom DTS.

### Shell 1 / Vitis
From top `Makefile` run using:
```
make sdtgen
```


The sdtgen/lopper use Vitis 2026.2 command line tools. If invoked from top Makefile, the environment will automatically be setup for this shell.
In addition, Lopper needs to use a flag to add symbol info.

If running from command line instead of Makefiles, ensure to setup Vitis tools and add this environment variable for Lopper to add symbol info:
```
export LOPPER_DTC_FLAGS="-b 0 -@"
```


### Check the system.dtb
To check that the overlays reflect the custom hardware design blocks, convert the DTB to human readable format with:
```
dtc -I dtb -O dts -o check_dump.dts build/system.dtb 
```
The `check_dump.dts` should contain keywords like `ai_engine`, `subtractor`, and `zocl`. If missing, inspect the Vivado implementation results.
Missing devicetree nodes will cause the host application to fail loading or interacting with the AIE and PL kernels.


## Navigation helper
 - [Next step - Yocto/EDF build](../yocto/README.md)
 - [Previous step - Implement HW design in Vivado](../../vivado/Finalize_Vivado.md)
 - [Return to Linux overview](../README.md)
 - [Return to top](../../README.md)

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2026 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
