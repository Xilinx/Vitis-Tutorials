# Still under construction
## VAI 1.2 flow is under construction. Need to keep master unmodified for VAI1.1 publish. So create this tmp repository

# Vitis AI Custom Platform Development<br /><br />
1. Introduction to Vitis Acceleration Platform<br />
2. Create the Vivado Hardware Component<br />
3. Configure Platform Interface Properties and Generate XSA<br />
4. Create the PetaLinux Software Component<br />
5. Create the Vitis Platform<br />
6. Prepare for the DPU Kernel<br />
7. Create and Build a Vitis Application<br />
8. Prepare the Network Deployment File<br />
9. Run Application on Board<br />

## Introduction to Vitis Acceleration Platform<br /><br />
The Vivado Design Suite is used to generate XSA containing a few additional IP blocks and metadata to support kernel connectivity. The following figure shows the acceleration kernel application development flow:<br />
![vitis_acceleration_flow.PNG](/pic_for_readme/vitis_acceleration_flow.PNG)<br />
For Vitis AI platform, DPU is integrated as RTL kernel. To create a Vitis AI platform on MPSoC and run ConvNet on that, you need to create a Vivado HW platform, a PetaLinux SW platform, a Vitis platform which contains both the HW/SW platform you created. Then create a Vitis application based on this Vitis platform, import DPU kernel & ARM deployment code and build the Vitis application to be a HW-SW cowork design. Vitis would generate a SD card folder as output which would contain all the files needed to boot up from a target board. In the meanwhile to cross-compile the application and run it on board you may need Vitis AI library and DNNDK, you should install them both on the host and target board.<br />

## Create the Vivado Hardware Component and Generate XSA<br /><br />
1. Source <Vitis_Install_Directory>/settings64.sh, and call Vivado out by typing "vivado" in the console.<br />
2. Create a Vivado project named zcu104_custom_platform.<br />
   a) Select ***File->Project->New***.<br />
   b) Click ***Next***.<br />
   c) In Project Name dialog set Project name to ```zcu104_custom_platform```.<br />
   d) Click ***Next***.<br />
   e) Leaving all the setting to default until you goto the Default Part dialog.<br />
   f) Select ***Boards*** tab and then select ***Zynq UltraScale+ ZCU104 Evaluation Board***<br />
   g) Click ***Next***, and your project summary should like below:<br />
   ![vivado_project_summary.png](/pic_for_readme/vivado_project_summary.png)<br />
   h) Then click ***Finish***<br />
3. Create a block design named system. <br />
   a) Select Create Block Design.<br />
   b) Change the design name to ```system```.<br />
   c) Click ***OK***.<br />
4. Add MPSoC IP and run block automation to configure it.<br />
   a) Right click Diagram view and select ***Add IP***.<br />
   b) Search for ```zynq``` and then double-click the ***Zynq UltraScale+ MPSoC*** from the IP search results.<br />
   c) Click the ***Run Block Automation*** link to apply the board presets.<br />
      In the Run Block Automation dialog, ensure the following is check marked:<br />
      - All Automation<br />
      - Zynq_ultra_ps_e_0<br />
      - Apply Board Presets<br />

   d) Click ***OK***. You should get MPSoC block configured like below:<br />
![block_automation_result.png](/pic_for_readme/block_automation_result.png)<br />

***Note: At this stage, the Vivado block automation has added a Zynq UltraScale+ MPSoC block and applied all board presets for the ZCU104. Add the IP blocks and metadata to create a base hardware design that supports acceleration kernels.***<br /><br />

5. Re-Customizing the Processor IP Block<br />
   a) Double-click the Zynq UltraScale+ MPSoC block in the IP integrator diagram.<br />
   b) Select ***Page Navigator > PS-PL Configuration***.<br />
   c) Expand ***PS-PL Configuration > PS-PL Interfaces*** by clicking the ***>*** symbol.<br />
   d) Expand Master Interface.<br />
   e) Uncheck the AXI HPM0 FPD and AXI HPM1 FPD interfaces.<br />
   f) Click OK.<br />
   g) Confirm that the IP block interfaces were removed from the Zynq UltraScale+ MPSoC symbol in your block design.<br />
   ![hp_removed.png](/pic_for_readme/hp_removed.png)<br />
  
***Note: This is a little different from traditional Vivado design flow. When trying to make AXI interfaces available in Vitis design you should disable these interfaces at Vivado IPI platform and enable them at platform interface properties. We will show you how to do that later***<br><br />

6. Add clock block:<br />
   a) Right click Diagram view and select ***Add IP***.<br />
   b) Search for and add a Clocking Wizard from the IP Search dialog.<br />
   c) Double-click the clk_wiz_0 IP block to open the Re-Customize IP dialog box.<br />
   d) Click the Output Clocks tab.<br />
   e) Enable clk_out1 through clk_out3 in the Output Clock column, rename them as ```clk_100m```, ```clk_200m```, ```clk_400m``` and set the Requested Output Freq as follows:<br />
      - clk_100m to ```100``` MHz.<br />
      - clk_200m to ```200``` MHz.<br />
      - clk_400m to ```400``` MHz.<br />

   f) At the bottom of the dialog box set the ***Reset Type*** to ***Active Low***.<br />
   g) Click ***OK*** to close the dialog.<br />
  The settings should like below:<br />
  ![clock_settings.png](/pic_for_readme/clock_settings.png)<br />
***Note: So now we have set up the clock system for our design. This clock wizard use the pl_clk as input clock and geneatate clocks needed for the whole logic design. In this simple design I would like to use 100MHz clock as the axi_lite control bus clock, 200MHz clock as DPU AXI interface clock and 400MHz as DPU core clock. You can just modifiy these clocks as you like and remember we should "tell" Vitis what clock we can use. Let's do that later.(And after creating this example I learn that the Vitis AI DPU can only have 2 clock domains and the axi_lite control bus and DPU AXI interface share same clock. So the 100MHz clock can't be used as axi_lite control bus for DPU now.)***<br><br />

7. Add the Processor System Reset blocks:<br />
   a) Right click Diagram view and select ***Add IP***.<br />
   b) Search for and add a Processor System Reset from the IP Search dialog<br />
   c) Add 2 more Processor System Reset blocks, using the previous step; or select the proc_sys_reset_0 block and Copy (Ctrl-C) and Paste (Ctrl-V) it four times in the block diagram<br />
   d) Rename them as ```proc_sys_reset_100m```, ```proc_sys_reset_200m```, ```proc_sys_reset_400m```<br />
  
8. Connect Clocks and Resets: <br />
   a) Click ***Run Connection Automation***, which will open a dialog that will help connect the proc_sys_reset blocks to the clocking wizard clock outputs.<br />
   b) Enable All Automation on the left side of the Run Connection Automation dialog box.<br />
   c) Select clk_in1 on clk_wiz_0, and set the Clock Source to ***/zynq_ultra_ps_e_0/pl_clk0***.<br />
   d) For each proc_sys_reset instance, select the slowest_sync_clk, and set the Clock Source as follows:<br />
      - proc_sys_reset_100m with /clk_wiz_0/clk_100m<br />
      - proc_sys_reset_200m with /clk_wiz_0/clk_200m<br />
      - proc_sys_reset_400m with /clk_wiz_0/clk_400m<br />

   e) On each proc_sys_reset instance, select the ***ext_reset_in***, set ***Board Part Interface*** to ***Custom*** and set the ***Select Manual Source*** to ***/zynq_ultra_ps_e_0/pl_resetn0***.<br />
   f) Make sure all checkboxes are enabled, and click ***OK*** to close the dialog and create the connections.<br />
   g) Connect all the ***dcm_locked*** signals on each proc_sys_reset instance to the locked signal on ***clk_wiz_0***.<br />
   Then the connection should like below:<br />
   ![clk_rst_connection.png](/pic_for_readme/clk_rst_connection.png)<br /><br />
***Now we have added clock and reset IPs and configure and connect them. Some would be used in creating the hardware platform and some would be called in Vitis high level design***<br /><br />

9. Add Kernel Interrupt Support<br />
You can provide kernel interrupt support by adding an AXI interrupt controller to the base hardware design.<br />
   a) In the block diagram, double-click the Zynq UltraScale+ MPSoC block.<br />
   b) Select ***PS-PL Configuration > PS-PL interfaces > Master interface***.<br />
   c) Select the ***AXI HPM0 LPD*** check box, keep the ***AXI HPM0 LPD Data width*** settings as default ***32***.<br />
   d) Click ***OK*** to finsih the configuration.<br />
   e) Connect ***maxihpm0_lpd_aclk*** to ***/clk_wiz_0/clk_100m***.<br />
   f) Right click Diagram view and select ***Add IP***, search and add ***AXI Interrupt Controller*** IP.<br />
   g) Double click the AXI Interrupt Controller block, set ***Interrupts type*** to ```Level Interrupt```, set ***Level type*** to ```Active High```, set ***Interrupt Output Connection*** to ```Bus```. Click ***OK*** to save the change.<br />
   ![intc_settings.png](/pic_for_readme/intc_settings.png)<br /><br />
   h) Click the AXI Interrupt Controller block and go to ***Block Properties -> Properties***, configure or make sure the parameters are set as following:
***C_ASYNC_INTR***: ```0xFFFFFFFF```. <br />
   ![async_intr.png](/pic_for_readme/async_intr.png)<br /><br />
   ***When interrupts generated from kernels are clocked by different clock domains, this option is useful to capture the interrupt signals properly. For the platform that has only one clock domain, this step can be skipped.***

   i) Click ***Run Connection Automation***<br />   
   j) Leave the default values for Master interface and Bridge IP.<br />
      - Master interface default is /zynq_ultra_ps_e_0/M_AXI_HPM0_LPD.<br />
      - Bridge IP default is New AXI interconnect.<br />
   k) Expand output interface Interrupt of axi_intc_0 to show the port irq, connect this irq portto zynq_ultra_ps_e_0.pl_ps_irq0<br />
   l) Setup ***PFM_IRQ** property by typing following command in Vivado console:<br />
   ```set_property PFM.IRQ {intr {id 0 range 32}} [get_bd_cells /axi_intc_0]```<br />
***The IPI design connection would like below:***<br />
![ipi_fully_connection.png](/pic_for_readme/ipi_fully_connection.png)<br /><br />
***Note: Now we have finished the IPI design input, let's set some platform parameters and generate the DSA***<br /><br /><br />

## Configuring Platform Interface Properties<br /><br />
1. Click ***Window->Platform interfaces***, and then click ***Enable platform interfaces*** link to open the ***Platform Interfaces*** Window.<br />
2. Select ***Platform-system->zynq_ultra_ps_e_0->S_AXI_HP0_FPD***, in ***Platform interface Properties*** tab enable the ***Enabled*** option like below:<br />
![enable_s_axi_hp0_fpd.png](/pic_for_readme/enable_s_axi_hp0_fpd.png)<br /><br />
3. Select ***Options*** tab, set ***memport*** to ```S_AXI_HP``` and set ***sptag*** to ```HP0``` like below:
![set_s_axi_hp0_fpd_options.png](/pic_for_readme/set_s_axi_hp0_fpd_options.png)<br /><br />
4. Do the same operations for ***S_AXI_HP1_FPD, S_AXI_HP2_FPD, S_AXI_HP3_FPD, S_AXI_HPC0_FPD, S_AXI_HPC1_FPD*** and set ***sptag*** to ```HP1```, ```HP2```, ```HP3```, ```HPC0```, ```HPC1```. And be noticed that for HPC0/HPC1 ports the ***memport*** is set to ```S_AXI_HPC``` in default, but actually we would use these ports without data coherency function enabled to get a high performance. So please modify it into ```S_AXI_HP``` manually.<br />
![set_s_axi_hpc0_fpd_options.png](/pic_for_readme/set_s_axi_hpc0_fpd_options.png)<br /><br />
5. Enable the M01_AXI ~ M08_AXI ports of ps8_0_axi_periph IP(The axi_interconnect between M_AXI_HPM0_LPD and axi_intc_0), and set these ports with the same ***sptag*** name to ```HPM0_LPD``` and ***memport*** type to ```M_AXI_GP```<br />
6. Enable the ***M_AXI_HPM0_FPD*** and ***M_AXI_HPM1_FPD*** ports, set ***sptag*** name to ```HPM0_FPD```, ```HPM1_FPD``` and ***memport*** to ```M_AXI_GP```.<br />
***Now we enable AXI master/slave interfaces that can be used for Vitis tools on the platform***<br /><br />
7. Enable ***clk_200m***, ***clk_400m***, ***clk_100m*** of clk_wiz_0, set ***id*** of ***clk_200m*** to ```0```, set ***id*** of ***clk_400m*** to ```1```, set ***id*** of ***clk_100m*** to ```2```, enable ***is default*** for ***clk_200m***.<br />

8. Create a ```xsa_gen``` folder inside your Vivado project.<br />
9. Create a file named ```xsa.tcl``` inside the ***xsa_gen*** folder.<br />
10. Copy the following commands into the xsa.tcl file and save the file.<br />
```
# Set the platform design intent properties
set_property platform.design_intent.embedded true [current_project]
set_property platform.design_intent.server_managed false [current_project]
set_property platform.design_intent.external_host false [current_project]
set_property platform.design_intent.datacenter false [current_project]

get_property platform.design_intent.embedded [current_project]
get_property platform.design_intent.server_managed [current_project]
get_property platform.design_intent.external_host [current_project]
get_property platform.design_intent.datacenter [current_project]

# Set the platform default output type property
set_property platform.default_output_type "sd_card" [current_project]

get_property platform.default_output_type [current_project]
```
11. In your Vivado project, use the ***Tcl console*** to ***navigate to the xsa_gen folder***, and run ```source ./xsa.tcl``` command.
![run_xsa_tcl.png](/pic_for_readme/run_xsa_tcl.png)<br /><br />
12. Right-click and select ***Validate Design*** on ***IP integrator diagram***<br />
13. Create the HDL wrapper:<br />
    a. Right-click ***system.bd*** in the Block Design, Sources view and select Create HDL Wrapper.<br />
    b. Select Let Vivado manage wrapper and ***auto-update***.<br />
    c. Click ***OK***.<br />

14. Right-click ***system.bd*** in the Block Design, Sources view and select ***Generate Output Products***.<br />
15. Type the tcl command in tcl console like:<br />
```write_hw_platform -unified -force -file <your_vivado_project_dir>/xsa_gen/zcu104_custom_platform.xsa```<br />
If you use ***export Hardware*** function in Vivado GUI it would add ***-fixed*** option which would generate a XSA for traditional embedded platform which can't add DPU acceleration kernel here.
16. Check the ***<your_vivado_project_dir>/xsa_gen*** folder, you should find the ***zcu104_custom_platform.xsa*** generated there.<br />

***Now we finish the Hardware platform creation flow, then we should go to the Software platform creation***<br /><br />

## Create the PetaLinux Software Component<br /><br />

A Vitis platform requires software components. For Linux, the PetaLinux tools are invoked outside of the Vitis tools by the developer to create the necessary Linux image,Executable and Linkable Format (ELF) files, and sysroot with XRT support. Yocto or third-party Linux development tools can also be used as long as they produce the same Linux output products as PetaLinux. <br />
1. source <petaLinux_tool_install_dir>/settings.sh<br />
2. Create a PetaLinux project named ***zcu104_custom_plnx*** and configure the hw with the XSA file we created before:<br />
```petalinux-create --type project --template zynqMP --name zcu104_custom_plnx```<br />
```cd zcu104_custom_plnx```<br />
```petalinux-config --get-hw-description=<you_vivado_design_dir>/xsa_gen/```<br />
3. A petalinux-config menu would be launched, select ***DTG Settings->MACHINE_NAME***, modify it to ```zcu104-revc```.<br />
***Note: If you are using a Xilinx development board it is recomended to modify the machine name so that the board configurations would be involved in the DTS auto-generation. Otherwise you would need to configure the associated settings(e.g. the PHY information DTS node) by yourself manually.***<br />
4. Add user packages by appending the CONFIG_x lines below to the <your_petalinux_project_dir>/project-spec/meta-user/conf/user-rootfsconfig file.<br />
Packages for base XRT support:<br />
```
CONFIG_xrt
CONFIG_xrt-dev
CONFIG_zocl
CONFIG_dnf
CONFIG_e2fsprogs-resize2fs
CONFIG_parted
CONFIG_mesa-megadriver
CONFIG_opencl-clhpp-dev
CONFIG_opencl-headers-dev
CONFIG_packagegroup-petalinux-opencv
CONFIG_packagegroup-petalinux-opencv-dev
```
Packages for DPU support:<br />
```
CONFIG_packagegroup-petalinux-vitisai
```
Packages for building Vitis AI applications:<br /> 
```
CONFIG_json-c-dev
CONFIG_protobuf-dev
CONFIG_protobuf-c
CONFIG_libeigen-dev
```
Packages for native compiling on target board:<br /> 
```
CONFIG_packagegroup-petalinux-self-hosted
CONFIG_cmake 
```
Packages mentioned at DPU integration lab for Vivado flow:<br /> 
```
CONFIG_packagegroup-petalinux-x11
CONFIG_packagegroup-petalinux-v4lutils
CONFIG_packagegroup-petalinux-matchbox
```
5. Run ```petalinux-config -c rootfs``` and select ***user packages***, select name of rootfs all the libraries listed above, save and exit.
![petalinux_rootfs.png](/pic_for_readme/petalinux_rootfs.png)<br /><br />

6. Enable OpenSSH and disable dropbear<br /> 
Dropbear is the default SSH tool in Vitis Base Embedded Platform. If OpenSSH is used to replace Dropbear, it could achieve 4x times faster data transmission speed (tested on 1Gbps Ethernet environment). Since Vitis-AI applications may use remote display feature to show machine learning results, using OpenSSH can improve the display experience.<br /> 
    a) Run ```petalinux-config -c rootfs```.<br /> 
    b) Go to ***Image Features***.<br /> 
    c) Disable ***ssh-server-dropbear*** and enable ***ssh-server-openssh***.<br /> 
    ![ssh_settings.png](/pic_for_readme/ssh_settings.png)<br /><br />
    d) Go to ***Filesystem Packages-> misc->packagegroup-core-ssh-dropbear*** and disable ***packagegroup-core-ssh-dropbear***.<br />
    e) Go to ***Filesystem Packages  -> console  -> network -> openssh*** and enable ***openssh***, ***openssh-sftp-server***, ***openssh-sshd***, ***openssh-scp***.<br />
7. In rootfs config go to ***Image Features*** and enable ***package-management*** and ***debug_tweaks*** option, store the change and exit.<br />
8. CPU IDLE would cause CPU IDLE when JTAG is connected. So it is recommended to disable the selection.<br /> 
    a) Type ```petalinux-config -c kernel```<br /> 
    b) Ensure the following are ***TURNED OFF*** by entering 'n' in the [ ] menu selection for:<br />
       - ***CPU Power Mangement > CPU Idle > CPU idle PM support***<br />
       - ***CPU Power Management > CPU Frequency scaling > CPU Frequency scaling***<br /><br />

9. Update the Device tree to include the zocl driver by appending the text below to the ***project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi*** file. 
```
&amba {
	zyxclmm_drm {
		compatible = "xlnx,zocl";
		status = "okay";
		interrupt-parent = <&axi_intc_0>;
		interrupts = <0  4>, <1  4>, <2  4>, <3  4>,
			     <4  4>, <5  4>, <6  4>, <7  4>,
			     <8  4>, <9  4>, <10 4>, <11 4>,
			     <12 4>, <13 4>, <14 4>, <15 4>,
			     <16 4>, <17 4>, <18 4>, <19 4>,
			     <20 4>, <21 4>, <22 4>, <23 4>,
			     <24 4>, <25 4>, <26 4>, <27 4>,
			     <28 4>, <29 4>, <30 4>, <31 4>;
	};
};

&axi_intc_0 {
      xlnx,kind-of-intr = <0x0>;
      xlnx,num-intr-inputs = <0x20>;
      interrupt-parent = <&gic>;
      interrupts = <0 89 4>;
};

&sdhci1 {
      no-1-8-v;
      disable-wp;
};

```

10. Modify the u-boot settings:<br />
Because we didn't use SD card to store the rootfs files. So that u-boot need to load a large image. We need to modify the u-boot so that it can load larger image.
Open ***project-spec/meta-user/recipes-bsp/u-boot/files/platform-top.h*** and add:<br />
```
#define CONFIG_SYS_BOOTM_LEN 0x80000000
#undef CONFIG_SYS_BOOTMAPSZ
```
11. From within the PetaLinux project (petalinux), type ```petalinux-build``` to build the project.<br />
12. Create a sysroot self-installer for the target Linux system:<br />
```
cd images/linux
petalinux-build --sdk
```
***Note: We would store all the necessary files for Vitis platform creation flow. Here we name it ```zcu102_dpu_pkg ```. Then we create a pfm folder inside.***<br />
13. Type ```./sdk.sh``` to install PetaLinux SDK, provide a full pathname to the output directory ***<full_pathname_to_zcu104_dpu_pkg>/pfm***(here in this example I use ***/home/wuxian/wu_project/vitis2020.1/vitis_custom_platform_flow/zcu104_dpu_pkg/pfm***) and confirm.<br />
14. We would install Vitis AI library and DNNDK into this rootfs in the future.<br />
15. After the PetaLinux build succeeds, the generated Linux software components are in the ***<your_petalinux_dir>/images/linux directory***. For our example, the ***images/linux*** directory contains the generated image and ELF files listed below. Copy these files to the ***<full_pathname_to_zcu104_dpu_pkg>/pfm/boot*** directory in preparation for running the Vitis platform creation flow:<br />
```
    - image.ub
    - fsbl.elf (Rename the petalinux generated zynq_fsbl.elf to fsbl.elf)
    - pmufw.elf
    - bl31.elf
    - u-boot.elf
    - boot.scr
```
16. Add a BIF file (linux.bif) to the ***<full_pathname_to_zcu104_dpu_pkg>/pfm/boot*** directory with the contents shown below. The file names should match the contents of the boot directory. The Vitis tool expands these pathnames relative to the sw directory of the platform at v++ link time or when generating an SD card. However, if the bootgen command is used directly to create a BOOT.BIN file from a BIF file, full pathnames in the BIF are necessary. Bootgen does not expand the names between the <> symbols.<br />
```
/* linux */
 the_ROM_image:
 {
 	[fsbl_config] a53_x64
 	[bootloader] <fsbl.elf>
 	[pmufw_image] <pmufw.elf>
 	[destination_device=pl] <bitstream>
 	[destination_cpu=a53-0, exception_level=el-3, trustzone] <bl31.elf>
 	[destination_cpu=a53-0, exception_level=el-2] <u-boot.elf>
 }
```
***Note: Now we prepare the HW platform and SW platform, next we would create a Vitis Platform.***

## Create the Vitis Platform<br /><br />

1. Source Vitis and XRT settings<br />
```
source <Vitis_Install_Directory>/settings64.sh
source /opt/xilinx/xrt/setup.sh
```
2. Go to the ***zcu104_dpu_pkg*** folder you created: ```cd <full_pathname_to_zcu104_dpu_pkg>```.<br />
3. Launch Vitis by typing ```vits``` in the console.<br />
4. Select ***zcu104_dpu_pkg*** folder as workspace directory.<br />
![vitis_launch.png](/pic_for_readme/vitis_launch.png)<br /><br />
5. In the Vitis IDE, select ***File > New > Platform Project*** to create a platform project.<br />
6. In the Create New Platform Project dialog box, do the following:<br />
   a) Enter the project name. For this example, type ```zcu104_vai_custom```.<br />
   b) Leave the checkbox for the default location selected.<br />
   c) Click ***Next***.<br />
7. In the Platform Project dialog box, do the following:<br />
   a) Select ***Create from hardware specification (XSA)***.<br />
   b) Click ***Next***.<br />
8. In the Platform Project Specification dialog box, do the following:<br />
   a) Browse to the XSA file generated by the Vivado. In this case, it is located in ```vitis_custom_platform_flow/zcu104_custom_platform/xsa_gen/zcu104_custom_platform.xsa```.<br />
   b) Set the operating system to ***linux***.<br />
   c) Set the processor to ***psu_cortexa53***.<br />
   d) Leave the checkmark selected to generate boot components.<br />
   e) Click ***Finish***.<br />
9. In the Platform Settings view, observe the following:<br />
   - The name of the Platform Settings view matches the platform project name of ***zcu104_vai_custom***.<br />
   - A psu_cortexa53 device icon is shown, containing a Linux on psu_cortexa53 domain.<br />
   - A psu_cortexa53 device icon is shown, containing a zynqmp_fsbl BSP.<br />
   - A psu_pmu_0 device icon is shown, containing a zynqmp_pmufw BSP.<br />
10. Click the linux on psu_cortexa53 domain, browse to the locations and select the directory or file needed to complete the dialog box for the following:
```
Linux Build Output:
    Browse to zcu104_dpu_pkg/pfm/boot and click OK.
    
Bif file:
    Browse to zcu104_dpu_pkg/pfm/boot/linux.bif file and click OK.

Image:
    Browse to zcu104_dpu_pkg/pfm/boot and click OK.
```
![vitis_linux_config.png](/pic_for_readme/vitis_linux_config.png)<br /><br />
11. Click ***zcu104_vai_custom*** project in the Vitis Explorer view, click the ***Build*** button to generate the platform.
![build_vitis_platform.png](/pic_for_readme/build_vitis_platform.png)<br /><br />
***Note: The generated platform is placed in the export directory. BSP and source files are also provided for re-building the FSBL and PMU if desired and are associated with the platform. The platform is ready to be used for application development.***<br />

## Prepare for the DPU Kernel<br /><br />

1. Download Vitis AI by calling command ```git clone https://github.com/Xilinx/Vitis-AI.git```.<br />
2. Navigate to the repository:```cd Vitis-AI```, set the tag to proper tag(here we use ***v1.1***) by typing: ```git checkout v1.2```.<br />
3. If you don't want to destroy the TRD reference design. Copy ***DPU-TRD*** folder into another directory. For example I would copy that into my ***zcu104_dpu_pkg*** folder: ```cp -r DPU-TRD ~/wu_project/vitis2019.2/vitis_custom_platform_flow/zcu104_dpu_pkg/```<br />
4. Source Vitis tools setting sh file: ```source <vitis install path>/Vitis/2019.2/settings64.sh```.<br />
5. Source XRT sh file:```source opt/xilinx/xrt/setup.sh```.<br />
6. Export SDX_PLATFORM with the directory of the custom platform xpfm file which you created before. Here in my project it would be: ```export SDX_PLATFORM=/home/wuxian/wu_project/vitis2020.1/vitis_custom_platform_flow/zcu104_dpu_pkg/zcu104_vai_custom/export/zcu104_vai_custom/zcu104_vai_custom.xpfm```. Remember now this custom platform name is ***zcu104_vai_custom***.<br />
7. Navigate to the copy of the ***DPU-TRD*** folder, then go to the ***./prj/Vitis*** folder.<br />
There are 2 files can be used to modify the DPU settings: The ***config_file/prj_config*** file is for DPU connection in Vitis project and the dpu_conf.vh is for other DPU configurations. Here we would modify the prj_config so that 2 DPU cores are enabled. And then we modify dpu_conf.vh as [DPU-TRD readme](https://github.com/Xilinx/Vitis-AI/blob/v1.2/DPU-TRD/README.md) suggested.<br />
8. Modify the ***config_file/prj_config*** like below:<br />
```

[clock]

id=0:DPUCZDX8G_1.aclk
id=1:DPUCZDX8G_1.ap_clk_2
id=0:DPUCZDX8G_2.aclk
id=1:DPUCZDX8G_2.ap_clk_2

[connectivity]

sp=DPUCZDX8G_1.M_AXI_GP0:HPC0
sp=DPUCZDX8G_1.M_AXI_HP0:HP0
sp=DPUCZDX8G_1.M_AXI_HP2:HP1
sp=DPUCZDX8G_2.M_AXI_GP0:HPC1
sp=DPUCZDX8G_2.M_AXI_HP0:HP2
sp=DPUCZDX8G_2.M_AXI_HP2:HP3

[advanced]
misc=:solution_name=link
#param=compiler.addOutputTypes=sd_card

#param=compiler.skipTimingCheckAndFrequencyScaling=1

[vivado]
prop=run.impl_1.strategy=Performance_Explore
#param=place.runPartPlacer=0

```
9. Modify dpu_conf.vh from:<br />
```
`define URAM_DISABLE 
```
to<br />
```
`define URAM_ENABLE 
```

10. Generate the XO file by typing: ```make binary_container_1/dpu.xo DEVICE=zcu104_vai_custom```.<br />
11. Verify if the XO file is generated here: ***<zcu104_dpu_pkg directory>/DPU-TRD/prj/Vitis/binary_container_1/dpu.xo***.<br />

## Create and Build a Vitis application
1. Open Vitis workspace you were using before.<br />
2. Select ***File -> New -> Application Project***.<br />
3. Click ***next***<br />
4. Select ***zcu104_vai_custom*** as platform, click ***next***.<br />
5. Name the project ```hello_dpu```, click ***next***.<br />
5. Set Domain to ***linux on psu_cortexa53***, set ***Sys_root path*** to ```<full_pathname_to_zcu104_dpu_pkg>/pfm/sysroots/aarch64-xilinx-linux```(as you created by running ***sdk.sh***), keep the ***Kernel Image*** setting in default and click ***next***.<br />
6. Select ***System Optimization Examples -> Empty application*** and click ***finish*** to generate the application.<br />
7. Right click on the ***src*** folder under your ***hello_dpu*** application  in the Expplorer window, and select "Import Sources"
![import_sources.png](/pic_for_readme/import_sources.png)<br /><br />
8. Choose from directory ***<zcu104_dpu_pkg directory>/DPU-TRD/prj/Vitis/binary_container_1/*** as the target location, and import the ***dpu.xo*** file that we just created.<br />
9. Import sources again, and add the cpp, header and prj_config files from ***ref_files/src*** folder provided by this Git repository.<br />
10. In the Explorer window double click the hello_dpu.prj file to open it, change the ***Active Build configuration*** from ***Emulation-SW*** to ***Hardware***.<br />
11. Under Hardware Functions, click the lightning bolt logo to ***Add Hardware Function***.<br />
![add_hardware_function.png](/pic_for_readme/add_hardware_function.png)<br /><br />
12. Select the "DPUCZDX8G" included as part of the dpu.xo file that we included earlier.<br />
13. Click on binary_container_1 to change the name to dpu.<br />
14. Click on ***DPUCZDX8G*** and change the ***Compute Units*** from ```1``` to ```2``` because we have 2 dpu cores involved.<br />
15. Right click on "dpu", select ***Edit V++ Options***, add ```--config ../src/prj_config -s``` as ***V++ Options***, then click ***OK***.<br />
16. Go back to the ***Explorer*** window, right click on the ***hello_dpu*** project folder select ***C/C++ Building Settings**.<br />
17. In ***Propery for hello_dpu*** dialog box, select ***C/C++ Build->Settings->Tool Settings->GCC Host Linker->Libraries***
, click the green "+" to add the following libraries:
```
opencv_core
opencv_imgcodecs
opencv_highgui
opencv_imgproc
opencv_videoio
n2cube
hineon
```
18. In the same page, Check the ***Library search path*** to makesure the ```${SYSROOT}/usr/lib/``` is added, click ***Apply***<br />
![vitis_lib_settings.png](/pic_for_readme/vitis_lib_settings.png)<br /><br />
19. Then go to ***C/C++ Build->Settings->Tool Settings->GCC Host Compiler->Includes***, remove the HLS include directory and add ```${SYSROOT}/usr/include/``` like below, then click ***Apply and Close*** to save the changes.<br />
![vitis_include_settings.png](/pic_for_readme/vitis_include_settings.png)<br /><br />
***These steps are used to make sure your application can call libs in rootfs directly on Vitis appilcation build***
20. The Vitis AI library and DNNDK are not included in PetaLinux SDK rootfs, now let's install them into the rootfs directory:<br />
***Note:*** We should follow the section ***Setting Up the Host For Edge*** of [Vitis AI library readme file](https://github.com/Xilinx/Vitis-AI/blob/v1.2/Vitis-AI-Library/README.md) to install the Vitis AI library and section ***Setup cross-compiler for Vitis AI DNNDK and make samples*** of [DNNDK readme file](https://github.com/Xilinx/Vitis-AI/blob/v1.2/mpsoc/README.md) to install the DNNDK. If you feel difficult to following the official guide there you can refer to the following ones. ***Please just skip these steps if you already install the libs referring to the readme files***:<br />
    a) Download the [vitis_ai_2020.1-r1.2.0.tar.gz](https://www.xilinx.com/bin/public/openDownload?filename=vitis_ai_2020.1-r1.2.0.tar.gz) to a particular directory(here we take ***~/Downloads*** as example) and install it to the roofs folder:<br />
    ```
    cd ~/Downloads # Or some place else you download the vitis_ai_2019.2-r1.1.0.tar.gz file
    tar -xzvf vitis_ai_2020.1-r1.2.0.tar.gz -C <full_pathname_to_zcu104_dpu_pkg>/pfm/sysroots/aarch64-xilinx-linux
    ```
    b) Download DNNDK runtime package [vitis-ai_v1.2_dnndk.tar.gz ](https://www.xilinx.com/bin/public/openDownload?filename=vitis-ai_v1.2_dnndk.tar.gz) to ***~/Downloads*** and install it into rootfs
    ```
    cd ~/Downloads # Or some place else you download the file
    tar -xzvf vitis-ai_v1.2_dnndk.tar.gz
    cd vitis-ai_v1.2_dnndk
    ./install.sh <full_pathname_to_zcu104_dpu_pkg>/pfm/sysroots/aarch64-xilinx-linux
    ```
***Now we install both the VAI lib and DNNDK packages into the rootfs set as Vitis sysroot, then we can build application on Vitis.***<br />

21. Right click the ***hello_dpu*** project folder and select ***Build Project***<br />

## Prepare the Network Deployment File<br />

1. Find HWH file from your Vitis application folder***hello_dpu/Hardware/dpu.build/link/vivado/vpl/prj/prj.srcs/sources_1/bd/system/hw_handoff/system.hwh***<br />
Or go to your Vitis application folder use command ```find -name *.hwh``` to search for the file.<br />
2. Copy the ***ref_files/Tool-Example*** folder provided by this Github repository to your Vitis AI download directory.<br />
3. Copy this HWH file into ***<Vitis-AI-download_directory>/Tool-Example*** folder.<br />
4. Go to ***<Vitis-AI-download_directory>*** folder and launch the docker.<br />
5. Use following command to activate TensorFlow tool conda environment:<br />
```
conda activate vitis-ai-tensorflow
```
6. Go to ***/workspace/Tool-Example*** folder and run ```dlet -f ./system.hwh```.<br />
You should get the running log like below:
```
(vitis-ai-tensorflow) wuxian@wuxian-Ubuntu1804:/workspace/Tool-Example$ dlet -f ./system.hwh 
[DLet]Generate DPU DCF file dpu-06-18-2020-12-00.dcf successfully.
```
The DCF file name should be associated with the time and date you generating this file.<br />
7. Open the ***arch.json*** file and make sure the ***"dcf"*** parameter is set with the name you got on the previous step:<br />
```"dcf"      : "./dpu-06-18-2020-12-00.dcf",```<br />
8. Run command```sh download_model.sh``` to download the Xilinx Model Zoo files for resnet-50.<br />
9. Run command```sh custom_platform_compile.sh```, go to ***tf_resnetv1_50_imagenet_224_224_6.97G/vai_c_output_ZCU104/dpu_resnet50_0.elf*** to get the ***dpu_resnet50_0.elf*** file.<br />
10. Copy that file to the ***src*** folder of Vitis application ***hello_dpu***<br />
11. Right click on the ***hello_dpu*** project folder in Vitis select ***C/C++ Building Settings**.<br />
12. In ***Propery for Hello_DPU*** dialog box, select ***C/C++ Build->Settings->Tool Settings->GCC Host Linker->Miscellaneous->Other objects***, add a new object: ```"${workspace_loc:/${ProjName}/src/dpu_resnet50_0.elf}"```, click ***Apply and Close***.<br />
13. Right click the ***hello_dpu*** project folder and select ***Build Project***<br />
***Now you should get an updated hello_dpu with a size of about 20MB(the ConvNet model is involved).***<br />

## Run Application on Board<br />
1. Copy all the files from ***sd_card folder*** inside your Vitis application like ***<hello_dpu_application_directory>/Hardware/sd_card/*** to SD card, set ZCU104 to SD boot mode and boot up the board, connect the board with serial port.<br />
2. Connect SSH:<br />
   a) Run ```ifconfig``` to get the IP address, here we take ```172.16.75.189``` as example.<br />
   b) Using SSH terminal to connect ZCU104 with SSH: ```ssh -x root@172.16.75.189```, or use MobaXterm in Windows.<br />
3. Mount SD card to mnt folder by running command: ```mount /dev/mmcblk0p1 /mnt```.<br />
4. Go to the /mnt folder and create a new folder named "package":
```
cd /mnt
mkdir package
```
5. Since this is a custom design the Vitis AI library, DNNDK and test images are not installed. We need to install them on board.<br />
I would suggest you to refer to section "Setting Up the Target" of [Vitis AI library readme file](https://github.com/Xilinx/Vitis-AI/blob/v1.2/Vitis-AI-Library/README.md) to install the Vitis AI library and refer to section "Setup Evaluation Board and run Vitis AI DNNDK samples" of [DNNDK example readme file](https://github.com/Xilinx/Vitis-AI/blob/v1.2/mpsoc/README.md) to install DNNDK and test images. If you feel difficult to do that please follow the steps below:<br />
   a) Download the Vitis AI Runtime 1.1 package [Vitis AI Runtime 1.2.0](https://www.xilinx.com/bin/public/openDownload?filename=vitis-ai-runtime-1.2.0.tar.gz)<br />
   b) Copy the vitis-ai-runtime-1.2.0.tar.gz from host to board with the following command running on host:<br />
   ```
   cd <path_to_vitis-ai-runtime-1.2.0.tar.gz>
   scp vitis-ai-runtime-1.2.0.tar.gz 172.16.75.189:/mnt/package
   ```
   c) Untar the packet and install them one by one on target board:<br />
   ```
   cd /mnt/package
   tar -zxvf vitis-ai-runtime-1.2.0.tar.gz
   cd ./vitis-ai-runtime-1.2.0/aarch64/centos/
   rpm -ivh --force --ignoresize libunilog-1.2.0-r10.aarch64.rpm
   rpm -ivh --force --ignoresize libxir-1.2.0-r12.aarch64.rpm
   rpm -ivh --force --ignoresize libtarget-factory-1.2.0-r10.aarch64.rpm
   rpm -ivh --force --ignoresize libvart-1.2.0-r16.aarch64.rpm
   rpm -ivh --force --ignoresize libvitis_ai_library-1.2.0-r15.aarch64.rpm   
   ```


   d) Download the package [vitis-ai_v1.2_dnndk.tar.gz](https://www.xilinx.com/bin/public/openDownload?filename=vitis-ai_v1.2_dnndk.tar.gz) and package [vitis-ai_v1.2_dnndk_sample_img.tar.gz](https://www.xilinx.com/bin/public/openDownload?filename=vitis-ai_v1.2_dnndk_sample_img.tar.gz), copy them to board:<br />
   ```
   scp vitis-ai_v1.2_dnndk.tar.gz root@172.16.75.189:/mnt/package
   scp vitis-ai_v1.2_dnndk_sample_img.tar.gz root@172.16.75.189:/mnt/package
   ```
   e) Install DNNDK package like below:<br />
   ```
   cd /mnt/package
   cp vitis-ai_v1.2_dnndk.tar.gz ~/
   cd ~/
   tar -zxvf vitis-ai_v1.2_dnndk.tar.gz
   cd vitis-ai_v1.2_dnndk/
   ./install.sh
   ```
   g) Go back to ***/mnt/package*** folder and untar the dnndk example file:<br />
   ```
   cd /mnt/package
   tar -zxvf vitis-ai_v1.2_dnndk_sample_img.tar.gz
   ```
6. Go to the vitis_ai_dnndk_samples and run the hello_dpu application:<br />
```
cd /mnt/package/vitis_ai_dnndk_samples
mkdir test
cd test
cp /mnt/hello_dpu ./
./hello_dpu
```
***We store the hello_dpu to /mnt/package/vitis_ai_dnndk_samples/test folder to suit the relative path in my code, you can do that according to your code context. The hello_dpu is generated in Vitis application build and was copied to sd card from previous operation.***<br />
7. You should see the result like below:<br />
![test_result.PNG](/pic_for_readme/test_result.PNG)<br /><br />

## Reference<br />

https://www.xilinx.com/html_docs/xilinx2019_2/vitis_doc/index.html<br />
https://github.com/Xilinx/Vitis-AI<br />
https://github.com/Xilinx/Vitis_Embedded_Platform_Source<br />
https://github.com/Xilinx/Vitis-AI-Tutorials/tree/Vitis-AI-Custom-Platform<br />
https://github.com/Xilinx/Edge-AI-Platform-Tutorials/tree/3.1/docs/DPU-Integration<br /><br />
***Note: If you would like to try with one click creating VAI platform flow it is recommended to try with the official platform source code for*** [zcu102_dpu](https://github.com/Xilinx/Vitis_Embedded_Platform_Source/tree/master/Xilinx_Official_Platforms/zcu102_dpu) ***and*** [zcu104_dpu](https://github.com/Xilinx/Vitis_Embedded_Platform_Source/tree/master/Xilinx_Official_Platforms/zcu104_dpu)***.*** <br /><br /><br />

## More Information about Install and Set Vitis and XRT Environment<br />
https://www.xilinx.com/html_docs/xilinx2019_2/vitis_doc/Chunk2027126153.html#zks1565446519267<br />
https://www.xilinx.com/html_docs/xilinx2019_2/vitis_doc/pjr1542153622642.html<br />
https://www.xilinx.com/html_docs/xilinx2019_2/vitis_doc/rbk1547656041291.html<br />




   
   





