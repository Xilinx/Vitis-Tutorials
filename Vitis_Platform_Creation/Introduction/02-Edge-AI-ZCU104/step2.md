## Step 2: Create the Software Components with PetaLinux

A Vitis platform requires software components. Xilinx provides common software images for quick evaluation. Here since we'd like to do more customization, we'll use the PetaLinux tools to create the Linux image and sysroot with XRT support. Yocto or third-party Linux development tools can also be used as long as they produce the same Linux output products as PetaLinux.

### PetaLinux Project Settings

1. Setup PetaLinux environment: `source <petaLinux_tool_install_dir>/settings.sh`

2. Create a PetaLinux project named ***zcu104_custom_plnx*** and configure the hw with the XSA file we created before:

   ```
   petalinux-create --type project --template zynqMP --name zcu104_custom_plnx
   cd zcu104_custom_plnx
   petalinux-config --get-hw-description=<you_vivado_design_dir>
   ```

3. A petalinux-config menu would be launched, select ***DTG Settings->MACHINE_NAME***, modify it to ```zcu104-revc```.<br />
   ***Note: If you are using a Xilinx development board it is recommended to modify the machine name so that the board configurations would be involved in the DTS auto-generation. If you're using a custom board, you would need to configure the associated settings(e.g. the PHY information DTS node) by yourself manually.***<br />



### Customize Root File System, Kernel, Device Tree and U-boot

1. Add user packages by appending the CONFIG_x lines below to the ***<your_petalinux_project_dir>/project-spec/meta-user/conf/user-rootfsconfig*** file.

   ***Note: This step is not a must but it makes it easier to find and select all required packages in next step.***

   Packages for base XRT support:

    ```
   CONFIG_packagegroup-petalinux-xrt
    ```

   Packages for easy system management

    ```
   CONFIG_dnf
   CONFIG_e2fsprogs-resize2fs
   CONFIG_parted
    ```

    Packages for Vitis-AI dependencies support:

    ```
   CONFIG_packagegroup-petalinux-vitisai
    ```

   Packages for natively building Vitis AI applications on target board:

    ```
   CONFIG_packagegroup-petalinux-self-hosted
   CONFIG_cmake
   CONFIG_packagegroup-petalinux-vitisai-dev
   CONFIG_xrt-dev
   CONFIG_opencl-clhpp-dev
   CONFIG_opencl-headers-dev
   CONFIG_packagegroup-petalinux-opencv
   CONFIG_packagegroup-petalinux-opencv-dev
    ```

    Packages for running Vitis-AI demo applications with GUI

    ```
    CONFIG_mesa-megadriver
    CONFIG_packagegroup-petalinux-x11
    CONFIG_packagegroup-petalinux-v4lutils
    CONFIG_packagegroup-petalinux-matchbox
    ```

2. Run ```petalinux-config -c rootfs``` and select ***user packages***, select name of rootfs all the libraries listed above, save and exit.
   ![petalinux_rootfs.png](./images/petalinux_rootfs.png)

3. Enable OpenSSH and disable dropbear
   Dropbear is the default SSH tool in Vitis Base Embedded Platform. If OpenSSH is used to replace Dropbear, the system could achieve 4x times faster data transmission speed (tested on 1Gbps Ethernet environment). Since Vitis-AI applications may use remote display feature to show machine learning results, using OpenSSH can improve the display experience.
   a) Run ```petalinux-config -c rootfs```
   b) Go to ***Image Features***.
   c) Disable ***ssh-server-dropbear*** and enable ***ssh-server-openssh***.
   ![ssh_settings.png](./images/ssh_settings.png)


d) Go to ***Filesystem Packages-> misc->packagegroup-core-ssh-dropbear*** and disable ***packagegroup-core-ssh-dropbear***.

e) Go to ***Filesystem Packages  -> console  -> network -> openssh*** and enable ***openssh***, ***openssh-sftp-server***, ***openssh-sshd***, ***openssh-scp***.

4. In rootfs config go to ***Image Features*** and enable ***package-management*** and ***debug_tweaks*** option, store the change and exit.

5. Disable CPU IDLE in kernel config.

   CPU IDLE would cause CPU IDLE when JTAG is connected. So it is recommended to disable the selection during project development phase. It can be enabled for production to save power.
   a) Type ```petalinux-config -c kernel```
   b) Ensure the following are ***TURNED OFF*** by entering 'n' in the [ ] menu selection for:

   - ***CPU Power Mangement > CPU Idle > CPU idle PM support***
     - ***CPU Power Management > CPU Frequency scaling > CPU Frequency scaling***

6. Update the Device tree.

   Append the following contents to the ***project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi*** file.

   - ***zyxclmm_drm*** node is required by zocl driver, which is a part of XRT.
   - ***axi_intc_0*** node defines 32 interrupt inputs. This can not be inferred by the hardware settings in Vivado.
   - ***sdhci1*** node decreases SD Card speed for better card compatibility on ZCU104 board. This only relates to ZCU104. It's not a part of Vitis acceleration platform requirements.

   ***Note***: an example file is provided in ***ref_files/step2_petalinux/system-user.dtsi***.

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




7. Add EXT4 rootfs support

   Since Vitis-AI software stack is not included in PetaLinux yet, they need to be installed after PetaLinux generates rootfs. PetaLinux uses initramfs format for rootfs by default, it can't retain the rootfs changes in run time. To make the root file system retain changes, we'll use EXT4 format for rootfs in second partition while keep the first partition FAT32 to store boot.bin file.

   Run `petalinux-config`, go to ***Image Packaging Configuration***, select ***Root File System Type*** as ***EXT4***, and append `ext4` to ***Root File System Formats***.

   ![](./images/petalinux_root_filesystem_type.png)

   ![](./images/petalinux_add_rootfs_types.png)

   Update ***bootargs*** to allow Linux to boot from EXT4 partition. There are various ways to update bootargs. Please take either way below.
   
   - Update in `petalinux-config`: Change ***Device Tree -> SUBSYSTEM_BOOTARGS_AUTO*** to NO and update ***SUBSYSTEM_DTB_OVERLAY*** to `earlycon console=ttyPS0,115200 clk_ignore_unused root=/dev/mmcblk0p2 rw rootwait cma=512M`
   
   - Update in  ***system-user.dtsi***: add `chosen` node in root
   ```
   /include/ "system-conf.dtsi"
   / {
	   chosen {
	   	bootargs = "earlycon console=ttyPS0,115200 clk_ignore_unused root=/dev/mmcblk0p2 rw rootwait cma=512M";
	   };
   };
   ```

   Please note in the bootargs, we also set these options:
   - ***clk_ignore_unused***: it tells Linux kernel don't turn off clocks if this clock is not used. It's useful clocks that only drives PL kernels because PL kernels are not represented in device tree.
   - ***cma=512M***: CMA is used to exchange data between PS and PL kernel. The size for CMA is determined by PL kernel requirements. Vitis-AI/DPU needs at least 512MB CMA.

### Build Image and Prepare for Platform Packaging

We would store all the necessary files for Vitis platform creation flow. Here we name it ```zcu104_custom_pkg ```. Then we create a pfm folder inside.

1. From any directory within the PetaLinux project, build the PetaLinux project.

   ```
   petalinux-build
   ```



2. Copy the generated Linux software boot components from ***<your_petalinux_dir>/images/linux directory*** to the ***<full_pathname_to_zcu104_custom_pkg>/pfm/boot*** directory to prepare for running the Vitis platform packaging flow:

   - zynqmp_fsbl.elf: ***rename as fsbl.elf*** as a workaround of a Vitis known issue.
   - pmufw.elf
   - bl31.elf
   - u-boot.elf

Note: These files are the sources of creating BOOT.BIN.

3. Add a BIF file (linux.bif) to the ***<full_pathname_to_zcu104_custom_pkg>/pfm/boot*** directory with the contents shown below. The file names should match the contents of the boot directory. The Vitis tool expands these pathnames relative to the sw directory of the platform at v++ link time or when generating an SD card. However, if the bootgen command is used directly to create a BOOT.BIN file from a BIF file, full pathnames in the BIF are necessary. Bootgen does not expand the names between the <> symbols.<br />

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

4. Prepare image directory. Contents in this directory will be packaged to FAT32 partition by v++ package tool.

   a) Copy the generated Linux software components from ***<your_petalinux_dir>/images/linux directory*** to the ***<full_pathname_to_zcu104_custom_pkg>/pfm/image*** directory.

   - boot.scr: script for u-boot initialization
   - system.dtb: device tree file for Linux to boot

   b) Copy ***init.sh*** and ***platform_description.txt*** from ***ref_files/step3_pfm*** to ***<full_pathname_to_zcu104_custom_pkg>/pfm/image*** directory.

   - init.sh will set environment variable XILINX_XRT for XRT and copy platform_desc.txt to /etc/xocl.txt
   - platform_desc.txt has the platform name. XRT will check platform name before loading xclbin file.

5. Create a sysroot self-installer for the target Linux system

   ```
   petalinux-build --sdk
   ```

6. Install sysroot: type ```./images/linux/sdk.sh``` to install PetaLinux SDK, provide a full pathname to the output directory ***zcu104_custom_pkg/pfm*** (This is an example ) and confirm.<br />

  We would install Vitis AI library and DNNDK into this rootfs in the future.

***Note: Now HW platform and SW platform are all generated. Next we would [package the Vitis Platform](./step3.md).***

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>