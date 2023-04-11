<!-- 
# Copyright 2023 Xilinx Inc.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
-->

# Step 1: Create a hardware platform for customer's VCK190

In this step, we will create a hardware design using Versal Extensible Embedded Part Support Example Platform for customer's vck190 board (as mentioned we will regard vck190 as a customer's board). As a part support design for Versal it has already added the CIPS, NOC, NOC DDR, AI engine and other platform required IPs. And  board level configurations are reserved, e.g. PS side peripherals, clock and DDR related parameters for user to configure due to different boards have different peripherals. After we config the PS side CIPS, DDR related parameters and other platform related properties, XSA file will be exported. 

### Create Vivado project 

1. Create workspace and Launch Vivado if you haven't

   Run the following command to create a workspace and launch Vivado in the console

   ```bash
   mkdir WorkSpace #create workspace
   cd WorkSpace
   source <Vivado_Install_Directory>/settings64.sh
   ```

2. Download the Versal Customer Extensible Embedded Platform Example
   
   - Click menu **Tools -> Vivado Store..**
   - Click **OK** to agree to download open source examples from web
   - Select **Example Designs ->Platform -> Versal Extensible Embedded Platform part_support** and click the download button on the tool bar
   - Click **Close** after installation complete.

   ![STEP1](images/step1/vivado_download_example.png)

3. Create the Versal Extensible Embedded Part Support Platform Example project.

   - Click **File -> Project -> Open Example..**
   - Click **Next**
   - Select **Versal Extensible Embedded Platform (Part based)** in Select Project Template window. Then click **Next**.
   - In Project Name dialog set Project name to **custom_hardware_platform** and confirm your project location is in **Workspace** directory. Keep **Create project subdirectory** option checked. Then click **Next**.
   - In default part dialog select the silicon model. Please select **XCVC1902-vsva2197-2MP-e-S** as the chip on VCK190 board is `XCVC1902-vsva2197-2MP-e-S`  which you can find on board user manual. 

   ![STEP1](./images/step1/vivado_project_silicon.PNG)

   - Click **Next**. You would get following table.

   ![STEP1](./images/step1/vivado_ced_config.png)

   - Configure Clocks Settings. You can enable more clocks, update output frequency and define default clock in this view. In this example, we can keep the default settings.
   - Configure Interrupt Settings. You can choose how many interrupt should this platform support. 63 interrupts mode will use two AXI_INTC in cascade mode. In this example, we can keep the default setting.
   - Enable AIE or not. In this example, we can keep the default setting.
   - Click **Next**.
   - Review the new project summary and click **Finish**.
   - After a while, you will see the design example has been generated.

   The generated design is like the following:

   ![Vivado Design Block Diagram](images/step1/vivado_design_diagram.PNG)

   At this stage, the Vivado block automation has added a Control, Interface & Processing System (shorten with CIPS in the future) block, AXI NOC block, AI Engine, and all supporting logic blocks to the diagram. However board level related configurations are not set as different board has different resources. Then we will config the CIPS PS part and DDR related parameters according to the board.

### Config the block

1. Config the Versal_cips IP

   This step is mainly to config the peripherals according to our board.
      
   - Double-click `CIPS_0` in the Block Diagram window. Click **Next**, then click **PS PMC** to config the PS-PMC parts one by one
  
     - Go to **Boot Mode**, select the supported boot interface according to your board. In this case we use the default selection **SD1/eMMC1** boot as VCK190 support **SD1/eMMC1** boot. Then choose slot type as **SD3.0** by clicking drop-down button. Enable **Detect Location** and **Bus Power Location**. configured setting is like the following:

     ![STEP0](./images/step1/vivado_project_cipsbootmode.PNG)

     - Go to **Peripherals**, select the peripherals according to your board. In this case we select several interfaces like the following. Please enable the **MDIO** support when you select **GEM0**.
      
     ![STEP0](./images/step1/vivado_project_cipsperpherial.PNG)
     
     >Note: When you configure the peripherals, there are some red warnings about pin conflicts. In this case just ignore it we will resolve it in IO configuration tab.  

     - Go to **IO**, select the corresponding IO pins according to your board. In this case we configure the IO pins like the following:
      
     ![STEP0](./images/step1/vivado_project_cipisio.PNG)

   - Click **Finish** to exit PS PMC config. 
   - Click **Finish** to exit Versal_cips config. 

   **Note**:
      - Select the peripherals, boot mode and IO pin-group according to your own board. 
      - Regarding the clock, we use the default configuration. If you have other configurations for your board please go to clocking part to config it. 

2. Config the noc_ddr4 IP

   This step is to config the DDR related parameters.

   - Double click the noc_ddr4 IP, Click **DDR Basic** tab, configure the following settings for this tab.

   ![STEP0](./images/step1/vivado_project_noc0ddrbasic.PNG)

   >Note: Please set the input system clock period according to your board. In this case we change the input system clock period to 5000 as the input frequency is 200M on VCK190 board.

   - Click **DDR Memory** tab, configure the following settings for this tab.

   ![STEP0](./images/step1/vivado_project_noc0ddrpara.PNG)

   >Note: Please set the DDR memory parameters according to the DDR on your board

   - As well, double click the external port `sys_clk0_0`,change the value of Frequency to **200**M HZ as we provide 200M HZ for DDR4 on vck190 board.

   Now we have configured the PS side peripherals and DDR related parameters. But the external DDR port connection is not set. Therefore the next we will add DDR constraint file to set the DDR PIN placement.

3. Config the noc_lpddr4 IP
   This step is to config the LPDDR4 related parameters.

   - Double click the noc_lpddr4 IP, click the **General** tab, configure the following settings for this tab.
  
   ![STEP0](./images/step1/vivado_project_noc0lpddr4general.PNG)

   - Click the **DDR Basic** tab, configure the following settings for this tab.

   ![STEP0](./images/step1/vivado_project_noc0lpddr4basic.PNG)

   - Click the **DDR Memory** tab, enable the **Flipped Pinout** option like the foolowing:

   ![STEP0](./images/step1/vivado_project_noc0lpddr4memory.PNG)

   - Click **ok** to confirm and exit the configuration.
   - Click **Run Connection Automation**, slect all the interfaces, then click **ok**.

   ![STEP0](./images/step1/vivado_project_noc0lpddr4automation.PNG)

   - double click the external port `sys_clk0_1`,change the value of Frequency to **200.321**M HZ.

   >Note: Please set the LPDDR memory parameters according to the LPDDR4 on your board

### Add constraint file

   As customer board has no ready-made board file which contains the pin placement data. Pin location file in XDC file format which contains the DDR, clocks and resets related pin placement is required for the hardware design. In this case we have two external ports related to DDR on our design which should be declared about the PIN connections. Therefore a [ddr.xdc](./ref_files/step1_vivado/ddr.xdc) file for DDR pin placement is prepared in advance to illustrate how to prepare the XDC file. And please follow the below steps to add the XDC format constraint file. 

   - Copy the DDR.xdc file to the **WorkSpace** directory
   - Click the **source** under the **BLOCK DESIGN** dialog, select the **conttraints** by clicking your right button on your mouse, selct **Add source**,
   - In Add source set dialog, click **Next**,
   - Click **Add files**, then browse to **WorkSpace** directory and select **ddr.xdc file**
   - Keep the option of **copy the constraint files into  project** checked.

   ![STEP0](./images/step1/vivado_project_addxdcfile.PNG)

   - Click **Finish**

Now we have complete the hardware design. In the coming steps we will review the platform related property settings.

### Review the Versal Extensible Platform Example Part_base Platform Setup

1. (Optional) Export block diagram tcl to cross check the tcl commands or recreate design in the future.

   - Click **File -> Export -> Export Block Diagram**
   - Check the Tcl file location and click **OK**
   - Open the exported tcl file

2. Go to **Platform Setup** tab. 

   - If it's not open, click menu Window -> Platform Setup to open it.

   > Note: If you can't find Platform Setup tab, please make sure your design is a Vitis platform project. Open **Settings** in **Project Manager**, go to **Project Settings -> General** tab, make sure **Project is an extensible Vitis platform** is enabled.

3. Review the AXI port settings

   - In **axi_noc_ddr4**, S01_AXI to S27_AXI are enabled. **SP Tag** is set to **DDR**

   ![STEP1](images/step1/vivado_platform_setup.PNG)

   **Note**: Vitis emulation automation scripts require that AXI slave interfaces on Versal platforms to have SP Tag as either **DDR** or **LPDDR**. 

   - In **icn_ctrl_0** and **icn_ctrl_1** , M01_AXI to M15_AXI are enabled. In **icn_ctrl**, M03_AXI and M04_AXI are enabled. Memport is set to M_AXI_GP. SP Tag is empty. These ports provide the AXI master interfaces to control PL kernels. In the block diagram, icn_ctrl_0 and icn_ctrl_1 connects to an AXI Verification IP because the AXI SmartConnect IP requires a load. The AXI Verification IP is used here as a dummy.

   ![STEP1](images/step1/vivado_platform_setup_axi_master.PNG)

   **Note**: SP Tag for AXI Master doesn't take effect.

4. Review the Clock settings

   - In Clock tab, clk_out is the default clock. V++ linker will use this clock to connect the kernel if link configuration doesn't specify any clocks.
   - The Proc Sys Reset property is set to the synchronous reset signal associated with each clock.

   ![STEP1](./images/step1/vivado_platform_setup_clock.PNG)


5. Review the Interrupt Tab

   - In Interrupt tab, **intr** is enabled.

   ![STEP1](./images/step1/vivado_platform_setup_irq.PNG)


### Review the Simulation Model

The Versal Extensible Platform Example has setup the simulation model of each IP properly. We will review the settings in this session. If you created the block design by yourself, please make sure these settings are applied before running emulation on your platform.

Some blocks in the block design has multiple types of simulation models. Vitis emulation requires these blocks to use SystemC TLM (Transaction-level Modeling) model when available. TLM is the default simulation model for CIPS, NOC and AI Engine. We can review them to make sure they are correct before exporting the hardware.

1. Review CIPS simulation model settings

   - In Vivado GUI, select the CIPS instance
   - Check the ***Block Properties*** window
   - In ***Properties*** tab, it shows **ALLOWED_SIM_MODELS** is `tlm,rtl`, **SELECTED_SIM_MODEL** is `tlm`. It means this block supports two simulation models. We selected to use `tlm` model.

   ![STEP1](./images/step1/vivado_cips_tlm.PNG)

2. Review the simulation model property for NOC and AI Engine in the block design.



### Export Hardware XSA

1. Validate the block design by clicking ***Validate Design*** button

   - There should be only one interrupt controller related Critical Warning in the above design, because intr is floating and v++ will be responsible to connect it during acceleration application compiling.

   ```
   [BD 41-759] The input pins (listed below) are either not connected or do not have a source port, and they don't have a tie-off specified. These pins are tied-off to all 0's to avoid error in Implementation flow.
   Please check your design and connect them as needed: 
   /axi_intc_0/intr
   ```     
2. Update the module wrapper file

   As we have changed the block design, the top module wrapper file should be updated.

   - In **source** tab, right click **ext_platform_part_wrapper** file
   - Slect **Remove file from project** and slect **also delete the project local file form disk** and click **ok**.
   
   ![STEP1](./images/step1/update_wrapper.PNG)

   - right click **ext_platform_part.bd** in Design Sources group
   - Select **Create HDL Wrapper...**
   - Select **Let Vivado manage wrapper and auto-update**.
   - Click **OK** to generate wrapper for block design.

   
3. Select ***Generate Block Design*** from Flow Navigator, Select **Synthesis Options** to **Global**. It will skip IP synthesis during generation. Then click **Generate**.

4. Export hardware platform with the following scripts

   - Click **File -> Export -> Export Platform**. Alternative ways are: **Flow Navigator** window: **IP Integrator -> Export Platform**, or the **Export Platform** button on the bottom of **Platform Setup** tab.
   - Click **Next** on Export Hardware Platform page
   - Select **Hardware**. If there are any IP that doesn't support simulation, we need to generate Hardware XSA and Hardware Emulation XSA separately. Click **Next**
   - Select **Pre-synthesis**, because we're not making an DFX platform. Click **Next**
   - Keep it as it is in **Platform properties** setup dialog, click **Next**
   - Input XSA file name to **custom_hardware_platform_hw**, click Next.
   - Review the summary. Click **Finish**.
   - **custom_hardware_platform_hw.xsa** file will be generated in `custom_hardware_platform` directory.

   Rerun the Export Platform wizard again and export the XSA for hardware emulation.

   - Click **File -> Export -> Export Platform**. Alternative ways are: **Flow Navigator** window: **IP Integrator -> Export Platform**, or the **Export Platform** button on the bottom of **Platform Setup** tab.
   - Click **Next** on Export Hardware Platform page
   - Select **Hardware emulation**. If there are any IP that doesn't support simulation, we need to generate Hardware XSA and Hardware Emulation XSA separately. Click **Next**
   - Select **Pre-synthesis**, because we're not making an DFX platform. Click **Next**
   - Keep it as it is in **Platform properties** setup dialog, click **Next**
   - Input XSA file name to **custom_hardware_platform_hwemu**, click Next.
   - Review the summary. Click **Finish**.
   - **custom_hardware_platform_hwemu.xsa** file will be generated in `custom_hardware_platform` directory.
  
   You can also execute the following scripts to export the platform.

   ```tcl
   # Setting platform properties
   set_property pfm_name {xilinx:vck190_es:VCK190_Custom_Platform:0.0} [get_files -norecurse *.bd]
   set_property platform.default_output_type "sd_card" [current_project]
   set_property platform.design_intent.embedded "true" [current_project]
   set_property platform.design_intent.server_managed "false" [current_project]
   set_property platform.design_intent.external_host "false" [current_project]
   set_property platform.design_intent.datacenter "false" [current_project]
   # Export Expandable XSA with PDI
   write_hw_platform -hw -force -file ./custom_hardware_platform_hw.xsa
   write_hw_platform -hw_emu -force -file ./custom_hardware_platform_hwemu.xsa
   ```

### Validation the output of step1

   Please refer to [Platform Validation Example](../..//Feature_Tutorials/03_platform_validation/)

### Fast Track

Scripts are provided to re-create projects and generate outputs in each step. To use these scripts, please run the following steps.

1. Run build

   ```bash
   # cd to the step directory, e.g.
   cd step1_vivado
   make all
   ```
   
2. To clean the generated files, please run

   ```bash
   make clean
   ```
This script creates the same design, generate block diagram and export XSA.

   

A top level all in one build script is also provided. To build everything (step 1 to step 3) with one command, please go to ***ref_files*** directory and run

```bash
cd ref_files
make all COMMON_IMAGE_VERSAL=<path/to/common_image/>  #Specify the path of the common image
```

This command is to generate platform with pre-built software components and do sw emulation by running vadd applicationand aie_adder to test this platform

```
make sd_card COMMON_IMAGE_VERSAL=<path/to/common_image/>  #Specify the path of the common image
```
This command is to generate platform with pre-built software components and do hw test on board by running vadd and aie_adder application to test this platform

To clean all the generated files, please run

```bash
make clean
```

### Next Step

Now we have a hardware design. Next we'll go to [step 2 create Vitis Platform with common image](./step2.md).






<p align="center"><sup>Copyright&copy; 2023 Xilinx</sup></p>
