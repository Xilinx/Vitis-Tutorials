<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Introduction

The previous tutorial is built based on using the A53 core. To migrate it to use the R5 core, follow the instructions on this page. The flow is the same but you need to make a few modifications.

## Base Platform Changes

The AMD Vivado™ tools design needs to be modified to have the AI Engine domain mapped into the RPU address space. The AMD Versal™ adaptive SoC real-time processing unit (RPU) is a 32-bit processor, but addressing the AI Engine requires at least 44 bits. This issue can be resolved using the address remap capability of the NoC NMU.

To do this, run the following commands in the Tcl console:

```
set_property CONFIG.REMAPS {{M00_AXI {{0x4000_0000 0x200_0000_0000 1G}}}} [get_bd_intf_pins /ps_noc/S01_AXI]
set_property range 1G [get_bd_addr_segs {versal_cips_0/DATA_RPU0/SEG_ps_noc_C0_DDR_LOW0}]
assign_bd_address -offset 0x40000000 -range 0x40000000 -target_address_space [get_bd_addr_spaces versal_cips_0/DATA_RPU0] [get_bd_addr_segs ai_engine_0/S00_AXI/AIE_ARRAY_0] -force
assign_bd_address
validate_bd_design
```

After the block design passes validation, regenerate the output and export the platform XSA file.

## Rebuild Simple Application System Project

1. Because the XSA file has been changed, you need to rebuild the base platform first. To rebuild the base platform, right-click the **base_pfm_vck190** project, and select **update hardware specifications**. Choose the XSA file that you just exported in the previous step.

2. After the XSA file has been updated, click the hammer icon to rebuild the base platform project.

3. After the platform project building completes, rebuild the simple application system project. It is recommended to clean up the project first.

## Update Fixed Platform

1. Because the simple application system project was rebuilt, the exported fixed platform XSA was changed. Therefore, you need to follow the same process to update the **AIE_A-to-Z_pfm_vck190** platform project. Right-click it, and select **update hardware specifications**.

2. After the XSA file has been updated, click the hammer icon to rebuild the fixed platform project.

## PS Application Project Changes

The bare-metal application uses AI Engine driver APIs that are included in the BSP of the platform. To point the tool to those libraries, you need to make a few changes to the compiler/linker settings.

1. Right-click the A72 PS application (`A-to-Z_app`), and click **c/c++ Build Settings**.

      a. In the directories section under **ARM R5 g++ compiler**, add the directory for the AI Engine application: `${XILINX_VITIS_AIETOOLS}/../include`.

      ![missing image](images/r5_app_cfg1.png)

      b. In the libraries section under **ARM R5 g++ compiler**, add the following to the linker flags:  `-ladf_api and -L"${XILINX_VITIS_AIETOOLS}/lib/armr532.o"`.

      ![missing image](images/r5_app_cfg2.png)

2. Rebuild the application project, and run the system.

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
