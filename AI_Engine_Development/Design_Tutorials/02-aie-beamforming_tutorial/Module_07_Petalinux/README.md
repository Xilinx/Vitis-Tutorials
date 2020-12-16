# Building the Design 
To create, configure, and built the Petalinux project with a single command. 
```
make petalinux
```
The individual commands are explained later on in this README.md. 

# Introduction
In Module 05, we created a standalone software platform and compiled a Baremetal PS Host Application. In this Module, we will build a PetaLinux software platform, and, in Module 08, we will compile the Linux PS Host Application. Petalinux is an embedded Linux Software Development Kit (SDK) targeting FPGA-based system on-a-chip (SoC) designs. Petalinux Tools offer everything necessary to customize, build, and deploy embedded Linux solutions on Xilinx Processing Systems. Petalinux tool offers a full Linux distribution building system which includes the Linux OS as well as a complete configuration, build, and deploy environment for Xilinx silicon. 

## Differences Between Baremetal and Petalinux
In the Baremetal application, we had to access our PL kernels registers by computing the physical memory addresses. These memory addresses can change between builds and software releases. With the the Petalinux software platform, we can general UIO drivers for each PL kernel instance. Instead of accessing specific physical addresses like we did in Baremetal, we can use these UIO drivers which access the physical addresses for us.

Creating a Petalinux application allows us to take advantage of a variety of other features such as security? TODO. 

# Building the Petalinux Software Platform
Next, we go into the steps to creating the Petalinux software platform. The `make petalinux` step does all these steps with one command. 

Creation of the Petalinux software platform can be broken down into three phases: create, config, and build.

## Create: Creating the Petalinux Project with a BSP
First we create the Petalinux Project from a Board Support Package (BSP) with the `petalinux-create` command. A BSP is a collection of software drivers and operating system on which your Linux applications are built on. It is the support code for a given hardware platform or board that helps in basic initialization at power up and helps software applications to be run on top of it. Since we're using a VCK190-ES1 board, we will need the xilinx-vck190-es1-v2020.2-final.bsp. 

TODO: Contents of this BSP? 
```
mkdir -p build;
cd build;
petalinux-create -t project -s <PATH-TO-BSP>/xilinx-vck190-es1-v2020.2-final.bsp -n vck190_petalinux
```
These commands creates a new Petalinux project directory structure under `build/vck190_linux`. 

## Config: Updating the Petalinux Project with an XSA
Next, we have update the Petalinux project with the XSA we created in Module04. The XSA contains the configured PS and the PL Kernels we want to control from our Linux Applications. This step enables you to make the PetaLinux tools software platform ready for building a Linux system, customized to your hardware platform. We import the hardware description with `petalinux-config` command by giving the path of the directory containing the .xsa file as follows:
```
petalinux-config -p build/vck190_petalinux --get-hw-description ../Module04_AI_Engine_and_PL_Integration/build/rev1/hw –silentconfig
```
The --silentconfig option allows you to reuse a prior configuration used during the petalinux_create command.

## Config: Edit the rootfs_config file
Next, we edit the `build/vck190_linux/project-spec/configs/rootfs_config` file. This is the configuration file used for root file system. By default, most config options are sets to "is not set". We need to update the following config options to "=y":  

Enable the libsysfs, libmetal, and OpenAMP libraries. The libmetal library provides common user APIs used to access devices, handle device interrupts, and request memory across different operating environments. OpenAMP builds on top of libmetal to provide a framework for remote processor management and inter-processor communication.
```
CONFIG_libmetal=y
CONFIG_open-amp=y
CONFIG_libsysfs=y
```

Enable the ai-engine-driver. 
```
CONFIG_ai-engine-driver=y
```

Enable debug tweaks. 
```
CONFIG_YOCTO_ENABLE_DEBUG_TWEAKS=y
```

Configure for PetaLinux Auto Login:
```
CONFIG_auto-login=y
```

TODO: Need to find documentation for these config settings. and why each thing needs to be enabled. 

## Config: Updating the DTSI file
Next, we need to to create a DTSI file. We have provided the DTSI file for you. We have to copy the uio-system-user.dtsi to the Petalinux Project. 

```
cp uio-system-user.dtsi build/vck190_petalinux/project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi
```
This is a user-modifiable PetaLinux device tree configuration file. Open the uio-system-user.dtsi. You’ll see that this file creates a “generic-uio” driver for each PL kernel instance (dlbf_data_00, dlbf_data_01, dblf_coeff_00, ... etc). With this device tree configuration, we are able to communicate to the memory addresses of the PL kernels through these UIO drivers without actually knowing the physical addresses.  

## Config: Creating bsp.cfg file 
Next, we create the vck190_linux/project-spec/meta-user/recipes-kernel/linux/linux-xlnx/bsp.cfg file. This is a U-Boot fragment config file with the following config options:

```
CONFIG_I2C_XILINX=y
CONFIG_GPIO_XILINX=y
CONFIG_FPGA=y
CONFIG_FPGA_MGR_VERSAL_FPGA=y
CONFIG_XILINX_INTC=y
```
TODO: explain these configuration settings we're enabling. 

## Build: Building the Petalinux System Image
We build the Petalinux System Image with the following command:
```
petalinux-build -p build/vck190_petalinx
```
This step generates a device tree DTB file, Platform Loader and Manager Firmware (PLM), Processing System Management Firmware (PSM), and Arm Trusted Firmware (ATF), U-Boot, the Linux kernel, a root file system image and the boot script for a Versal ACAP. Finally, it generates the necessary boot images. The compilation progress shows on the console when executing this command.  

When the build finishes, the generated images are stored in the build/vck190_petalinux/images/linux directory.

## Build: Building the SDK (Target Sysroot Generation)
The OpenEmbedded build system uses BitBake to generate the Software Development Kit (SDK) installer script standard SDKs. PetaLinux builds Yocto SDK and installs it. The installed SDK can be used as sysroot for the application development. We build the SDK with the following command:

```
petalinux-build -p build/vck190_petalinux –sdk
```

This command builds SDK and copies it at build/vck190_petalinx/images/linux/sdk.sh.

## Build: Installing the SDK (Target Sysroot Generation)
The generated SDK has to be installed/extracted to a directory. The following command extracts the SDK to the default installation directory build/vck190_petalinux/images/linux/sdk/.

```
cd build/vck190_petalinux
petalinux-package –sysroot
```

## Build: Generating the Boot Image 
Lastly, we generate the boot image (BOOT.BIN). A boot image usually contains a PDI file (imported from hardware design), PLM, PSM firmware, Arm® trusted firmware, U-Boot, and DTB. 
```
cd build/vck190_petalinux
petalinux-package --boot --u-boot
```
This generates BOOT.BIN, BOOT_bh.bin, and qemu_boot.img in build/vck190_petalinux/images/linux directory. The default DTB load address is 0x1000. For more information, see Bootgen User Guide (UG1283).


# Refereces
UG1144 PetaLinux Tools Documentation
UG1283 Bootgen User Guide 
https://www.xilinx.com/products/design-tools/embedded-software/petalinux-sdk.html#documentation
Ug1186 Libmetal and OpenAMP https://www.xilinx.com/support/documentation/sw_manuals/xilinx2020_2/ug1186-zynq-openamp-gsg.pdf
