<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Custom Platform Creation

## Platforms

A platform is the starting point of your design and will be used to build Vitis software platform applications.

>**NOTE**: AMD recommends using the VCK190 base platform provided on xilinx.com as a starting point for designs. This page is intended for users who would like to understand how the base platform is generated. Other users can start directly from [AI Engine Development](./02-aie_application_creation.md).

In this first section of the tutorial, an example of how to create a new platform is shown. This starts with building the hardware system using the AI Engine in the AMD Vivado™ Design Suite.

This is in most ways a traditional AMD Vivado™ design. You are building the platform, that is, the part of the design that you do not want the Vitis tools to configure or modify. This can include completely unrelated logic, any hierarchy you want to have in the design, but there are some rules that you must follow:

- Your design must contain an IP integrator block diagram containing the CIPS, NOC, and other infrastructure IP.
- Your design must have at least one clock that you will expose to Vitis for use with any kernels that it adds.
This clock must have an associated `proc_sys_reset` block.

This tutorial targets the VCK190 board (see <https://www.xilinx.com/products/boards-and-kits/vck190.html>).

### Step 1: Build the AMD Versal™ Extensible Embedded Platform Example Design in Vivado

1. Launch Vivado IDE, and select ***Open Example Project*** from the Welcome window. You can also do it by clicking ***File*** from the menu, and select ***Project*** and then ***Open Example***.

2. Click ***Next*** to skip the first page of the wizard. In the template selection page, select the ***Versal Extensible Embedded Platform*** template. Click ***Next***.

3. Name this project as ***custom_pfm_vck190*** and click ***Next***.

4. In the board selection page, select ***VCK190 Evaluation Platform***. If you are going to target other platform, you can select others. Click ***Next***.

5. In the design preset page, keep the default settings. Note that the AI Engine is enabled:

      ![missing image](images/231_design_preset_config.jpg)

6. Click ***Finish*** to complete the example design creation phase, and this will open up the Vivado project with the template design you just created. You can open the block design to view the details of the platform design. By using the pre-built template, you can easily get a validated hardware design of the platform to move on to the next step. In your real design development procedure, you can use this as a baseline design and make further modifications on top of it.

      ![missing image](images/231_block_design_complete.jpg)

7. Click ***Generate Block Design*** from the Flow Navigator panel on the left, click ***Generate***, and wait for the process to complete.

>**NOTE**: A critical warning is displayed by Vivado when generating the Block Design. This is due to the fact that the Interrupt Controller IP has an unconnected input. This can be ignored as this input will be connected automatically by Vitis later in the flow.

8. Click ***File*** from the menu, and select ***Export*** > ***Export Platform***.

   a. On the second page, select ***Hardware and hardware emulation*** as the platform type.

      ![missing image](images/221_export_xsa_pg1.png)

   b. On the third page select ***Pre-synthesis***.

      ![missing image](images/export_xsa_pg2.png)

   c. On the fourth page, add the name of the platform.

      ![missing image](images/export_xsa_updated.PNG)

   d. On the fourth page, set the name of the XSA, and click ***Finish***.

      ![missing image](images/export_xsa_pg4.png)

9. Close the Vivado project after platform export process finishes.

### Step 2: Build the Platform in the Vitis Software Platform

1. Open the Vitis Unified IDE, and select a workspace.

2. On the Welcome Page, select ***Create Platform Component***, or select ***File → New Component →  Platform***.

3. Set the platform component name to ***base_pfm_vck190*** and click ***Next***.

      ![missing image](images/232_create_platform_pg1.jpg)

4. Select ***Hardware Design*** and use the XSA generated during the previous step and click ***Next***

     ![missing image](images/232_create_platform_pg2.jpg)

5. Set the Operating System to  ***aie_runtime*** and the Processor to ***ai_engine*** and click ***Next***. Then click ***Finish*** to create the platform component.

      ![missing image](images/232_create_platform_pg3.jpg)

6. Then build the platform by clicking on ***Build*** In the flow navigator with the base_pfm_vck190 component selected.

      ![missing image](images/232_build_platform.jpg)

      >**NOTE**: If you modify the XSA file later, go to vitis-comp.json located in the Settings folder of the platform component and click ***switch xsa***

7. The generated platform can be found in `base_pfm_vck190/export`.

In this step, you created the platform starting with building the hardware platform in the Vivado Design Suite. Then, you built the platform in the Vitis software platform, based on the exported XSA file.

In the next step, you will build an AI Engine application using this platform.

<p align="center"><b><a href="./README.md">Return to Start of Tutorial</a> — <a href="./02-aie_application_creation.md">Go to AI Engine Development</a></b></p>

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
