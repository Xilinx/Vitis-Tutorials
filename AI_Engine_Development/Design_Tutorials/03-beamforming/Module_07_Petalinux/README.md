
# Introduction

In Module 05, you created a standalone software platform and compiled a bare-metal PS host application. In this module, you will build a PetaLinux software platform. In Module 08, you will compile the Linux PS host application. PetaLinux is an embedded Linux software development kit (SDK) targeting FPGA-based system on-a-chip (SoC) designs. PetaLinux tools offer everything necessary to customize, build, and deploy embedded Linux solutions on Xilinx processing systems. The PetaLinux tool offers a full Linux distribution building system which includes the Linux OS as well as a complete configuration, build, and deploy environment for Xilinx silicon.

## Differences between Bare Metal and PetaLinux

In the bare-metal application, you had to access the registers of your PL kernels by computing the physical memory addresses. These memory addresses can change between builds and software releases. With the PetaLinux software platform, it is possible to bind general UIO drivers for each PL kernel instance. Instead of accessing specific physical addresses as you did in bare metal, you can use these UIO drivers which access the physical addresses for you.

Creating a PetaLinux application allows you to take advantage of a variety of other features such as security, multi-threading, and multi-processing.

# Building the Design

To create, configure, and build the PetaLinux project in one step, run the following command:

```
make petalinux
```

The rest of this module explains the individual commands in the Makefile that were used to create the Petalinux project.

# Building the PetaLinux Software Platform

Creation of the PetaLinux software platform can be broken down into the following phases: create PetaLinux, config PetaLinux, build PetaLinux, and build a Versal custom PetaLinux platform. The `make petalinux` step performs all these steps with one command.

## Create PetaLinux: Creating the PetaLinux Project with a BSP

First, create the PetaLinux project from a board support package (BSP) with the `petalinux-create` command. A BSP is a collection of software drivers and an operating system on which your Linux applications are built. It is the support code for a given hardware platform or board that helps in basic initialization at power-up and helps software applications to be run on top of it. Because you are using a VCK190-ES1 board, you will need the ``xilinx-vck190-es1-v2020.2-final.bsp`` package.

```
mkdir -p build;
cd build;
petalinux-create -t project -s <PATH-TO-BSP>/xilinx-vck190-es1-v2020.2-final.bsp -n vck190_linux
```

These commands create a new PetaLinux project directory structure under `build/vck190_linux`.

## Config PetaLinux: Updating the PetaLinux Project with an XSA

Next, update the PetaLinux project with the XSA created in Module 04. The XSA contains the configured PS and the PL kernels you want to control from your Linux applications. This step enables you to make the PetaLinux tools software platform ready for building a Linux system, customized to your hardware platform. Import the hardware description with the `petalinux-config` command by giving the path of the directory containing the XSA file as follows:

```
petalinux-config -p build/vck190_linux --get-hw-description ../Module04_AI_Engine_and_PL_Integration/build/rev1/hw –silentconfig
```

The ``--silentconfig`` option allows you to reuse a prior configuration used during the ``petalinux_create`` command.

## Config PetaLinux: Customizing the Root File System

Edit the `build/vck190_linux/project-spec/configs/config` file. In this configuration file, ensure that the root file system type is EXT4 by setting the following config options:

```
CONFIG_SUBSYSTEM_ROOTFS_EXT4=y
CONFIG_SUBSYSTEM_RFS_FORMATS="cpio cpio.gz cpio.gz.u-boot tar.gz jffs2 ext4"
```
To ensure the EXT4 root file system is generated, the bootargs in the `uio-system-user.dtsi` must be as follows:

```
 bootargs = "console=ttyAMA0 earlycon=pl011,mmio32,0xFF000000,115200n8 clk_ignore_unused root=/dev/mmcblk1p2 rw rootwait rootfs=ext4 uio_pdrv_genirq.of_id=generic-uio";
```

Lastly, the `config` file specifies the machine name as follows:

```
CONFIG_SUBSYSTEM_MACHINE_NAME="versal-vck190-reva-x-ebm-02-reva"
```

Next, add user packages by appending the CONFIG\_\* lines to the `build/vck190_linux/project-spec/configs/rootfs_config` file. By default, most config options are set to "is not set". Update the following config options to ``=y``:  

- **Packages for base XRT support:** This is required for the Vitis application acceleration development flow. It includes XRT and ZOCL.   

```
CONFIG_packagegroup-petalinux-xrt=y
```

- **Packages for easy system management (recommended):**

```
CONFIG_dnf
CONFIG_e2fsprogs-resize2fs
CONFIG_parted
CONFIG_imagefeature-package-management=y
```

- **Packages for libsysfs, libmetal, and OpenAMP libraries:** The libmetal library provides common user APIs used to access devices, handle device interrupts, and request memory across different operating environments. OpenAMP builds on top of libmetal to provide a framework for remote processor management and inter-processor communication.

```
CONFIG_libmetal=y
CONFIG_open-amp=y
CONFIG_libsysfs=y
```

- **The package for the AI Engine:**

```
CONFIG_ai-engine-driver=y
```

- **Enable debug tweaks:**

```
CONFIG_YOCTO_ENABLE_DEBUG_TWEAKS=y
```

- **Enable PetaLinux auto login:**

```
CONFIG_auto-login=y
```

## Config Petalinux: Updating the Device Tree

The DTSI file is provided for you. Copy the ``uio-system-user.dtsi`` file to the PetaLinux project.

```
cp uio-system-user.dtsi build/vck190_petalinux/project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi
```

This is a user-modifiable PetaLinux device tree configuration file. Open the ``uio-system-user.dtsi``. You will see that this file creates a ``generic-uio`` driver for each PL kernel instance (``dlbf_data_00``, ``dlbf_data_01``, ``dblf_coeff_00``, ... etc). With this device tree configuration, you can communicate to the memory addresses of the PL kernels through these UIO drivers without actually knowing the physical addresses.  

## Config Petalinux: Customizing Kernel Configuration

Create the ``vck190_linux/project-spec/meta-user/recipes-kernel/linux/linux-xlnx/bsp.cfg`` file. This is a Linux kernel config fragment file with the following config options:

```
CONFIG_I2C_XILINX=y
CONFIG_GPIO_XILINX=y
CONFIG_FPGA=y
CONFIG_FPGA_MGR_VERSAL_FPGA=y
CONFIG_XILINX_INTC=y
```
These configuration settings enable device drivers for general purpose GPIO IP, I2C IP, FPGA Manager, and Xilinx Interrupt Controller. These drivers are _not_ used in the Linux PS host application.

## Config Petalinux: Clean-Up

Run the following command to clean up the configuration files you edited:

```
petalinux-config -p build/vck190_linux --silentconfig
```

## Build PetaLinux: Building the PetaLinux Image

Build the PetaLinux system image with the following command:

```
petalinux-build -p build/vck190_linux
```

This step generates a device tree DTB file, platform loader and manager firmware (PLM), processing system management firmware (PSM), Arm™ trusted firmware (ATF), U-Boot, the Linux kernel, a root file system image, and the boot script for a Versal™ ACAP. Finally, it generates the necessary boot images. The compilation progress shows on the console when executing this command.  

When the build finishes, the generated U-Boot and Linux images are stored in the ``build/vck190_petalinux/images/linux`` directory.

## Build Petalinux: Building the SDK (Target Sysroot Generation)

The OpenEmbedded build system uses BitBake to generate the software development kit (SDK) installer script standard SDKs. PetaLinux builds Yocto SDK and installs it. The installed SDK can be used as a ``sysroot`` for the application development. Build the SDK with the following command:

```
petalinux-build -p build/vck190_petalinux –sdk
```

This command builds the SDK and copies it at ``build/vck190_petalinx/images/linux/sdk.sh``.

## Build PetaLinux: Installing the SDK (Target Sysroot Generation)

The generated SDK must be installed/extracted to a directory. The following command extracts the SDK to the default installation directory ``build/vck190_petalinux/images/linux/sdk/``.

```
cd build/vck190_petalinux
petalinux-package –sysroot
```

## Build PetaLinux: Generating the Boot Image

Lastly, generate the boot image (``BOOT.BIN``). A boot image usually contains a PDI file (imported from the hardware design), PLM, PSM firmware, Arm® trusted firmware, U-Boot, and DTB.

```
cd build/vck190_petalinux
petalinux-package --boot --u-boot
```
This generates ``BOOT.BIN``, ``BOOT_bh.bin``, and ``qemu_boot.img`` in the ``build/vck190_linux/images/linux`` directory. The default DTB load address is 0x1000. For more information, see the Bootgen User Guide ([UG1283](https://www.xilinx.com/search/support-keyword-search.html#q=ug1283)).

# Build the Versal Custom PetaLinux Platform

Now that we have our customized PetaLinux image, the next step is to create a Versal custom platform with that Linux image. Packaging a platform requires the following software components for EXT4 rootfs:

* ``build/vck190_linux/images/linux/linux.bif``: Boot image generation description file

* ``build/vck190_linux/images/linux/bl31.elf``: Component referred to by the ``linux.bif`` file in same folder

* ``build/vck190_linux/images/linux/u-boot.elf``: Component referred to by the ``linux.bif`` file in the same folder

* ``build/vck190_linux/images/linux/system.dtb``: Component referred to by the ``linux.bif`` file in the same folder

* ``build/image/boot.src`` - U-Boot configuration file

First, add the BIF file (``linux.bif``) to the ``build/vck190_linux/images/linux`` directory. We have provided one for you to copy. When you open the ``linux.bif`` file, you will notice that the file names should match the contents of the boot directory. They are the source for creating the ``BOOT.BIN``.

```
cp linux.bif build/vck190_linux/images/linux/linux.bif
```

Next, prepare the image directory. The contents of this directory will be packaged to FAT32 partition by the ``v++ --package`` tool. Copy the `boot.src` script for U-Boot initialization into the image directory.

```
mkdir build/image
cp build/vck190_linux/images/linux/boot.src build/image/.
```

Lastly, create the Versal custom PetaLinux platform (``.xpfm``) using the `xsct_create_pfm.tcl` script.

```
xsct xsct_create_pfm.tcl vck190_custom ../Module_04/build/rev1/hw/beamforming.rev1.hw.xsa
```

This script uses the XSA from Module 04 and the custom Petalinux image to generate a new Versal custom platform (.xpfm) in the `build/vck190_custom/` directory. The new XPFM platform is used in the ``v++ --package`` step in Module 09 to generate the SD card image.

# References

* [PetaLinux Tools Website](https://www.xilinx.com/products/design-tools/embedded-software/petalinux-sdk.html#tools)
* PetaLinux Tools Documentation ([UG1144](https://www.xilinx.com/search/support-keyword-search.html#q=ug1144))
* Bootgen User Guide ([UG1283](https://www.xilinx.com/search/support-keyword-search.html#q=ug1283))
* Libmetal and OpenAMP ([UG1186](https://www.xilinx.com/support/documentation/sw_manuals/xilinx2020_2/ug1186-zynq-openamp-gsg.pdf))
* [Versal Platform Creation Tutorial](https://gitenterprise.xilinx.com/swm/Versal_Platform_Creation/tree/master/Tutorial-VCK190_Custom)

© Copyright 2021 Xilinx, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0


Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
