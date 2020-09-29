# IVAS ZCU104 ML Acceleration Reference Release #

**Required Tool Version: 2020.1 with Vitis AI 1.2**

This repository contains a reference design for the use of the Xilinx IVAS framework
in a multi-channel machine learning system. In includes:

- **hw_src** with the hardware source files needed to build the Vitis project

- **hls_tests** with HLS scripts and test data for validation of individual kernels

- **sw_src** contains source code for both individual test applications and the gstreamer pipeline

- **platform** contains the platform source

- **vitis_ai_library** contains some content overrides for the Vitis AI library

- **vitis_patch** contains an SD card packaging patch for Vitis

***Note: Vitis Patch Required:***
This design has a large rootfs, and Vitis 2020.1 has an issue packaging SD card images with
ext4 partitions over 2GB. This patch changes the packaging flow to round up the initial rootfs
size to the first full multiple of 512MB over the ext4 partition size. To install it:

```bash
$ cp ./vitis_patch/mkfsImage.sh ${XILINX_VITIS}/scripts/vitis/util
```

## Build Steps

#### 1. Clone the full repository (including submodules)

```bash
$ git clone --recurse-submodules https://github.com/Xilinx/Vitis-In-Depth-Tutorial
```
***Note: Make sure the path where you are cloning this repo is not NFS mounted***

#### 2. Build the hardware platform

With Vitis and PetaLinux sourced, change into the *platform/dev/zcu104_vcu* directory and
run **make**. This will output the platform source to *platform/repo/xilinx_zcu104_vcu_202020_1*.
After building the platform, extract and install the sysroot.

```bash
$ cd platform/dev/zcu104_vcu

$ make

$ cd ../../repo/xilinx_zcu104_vcu_202010_1/sysroot

$ ./sdk.sh -d `pwd` -y
```

#### 3. Build the Vitis design

Source the **setup.sh** script from your XRT installation area. This platform is built with the
2020.1 release of XRT, version 2.6.655. Version 2.7.766 is also supported. You must use
one of these versions; building with different versions of XRT is not supported.

```bash
source /opt/xilinx/xrt/setup.sh
```

One time, and one time only, you must apply the patch in the hw_src directory against the
Vitis Vision library.

```bash
$ cd hw_src/Vitis_Libraries

$ patch -p1 < ../vision_lib_area_resize_ii_fix.patch
```

With the patch applied, build the hardware design:

```bash
$ cd hw_src

$ make
```

***NOTE: From this point we strongly recommend starting in a 'fresh' terminal***

#### 4. Source the cross-compilation toolchain

```bash
$ cd platform/repo/xilinx_zcu104_vcu_202010_1/sysroot/

$ source ./environment-setup-aarch64-xilinx-linux
```

#### 5. Build the target software

This is done using a combination of cmake and meson, but requires setup of the cross-compilation environment first. The build process in inludes multiple conceptual submodules using different build systems, so a script has been provided to help automate it and to make the various processes clear.

The script will do the the following things automatically:

- Add an XRT component to the cross-compilation rootfs for pkfconfig, if it doesn't already exist
- Deploy Vitis AI cross-compilation components into the rootfs
- Configure meson cross-compilation files for the target
- Build all source references

***Note: user to create ml_models folder and keep generated densebox and reid models in it.***

Please refer to Vitis AI 1.2 documentation for compiling models

```bash
- ml_models/dpu_densebox_640_360.elf
- ml_models/dpu_reid.elf
```

```bash
$ cd sw_src

$ ./go.sh
```

## Preparing the SD Card Image for Boot

#### 1. Burning the image on micro SDcard
Using an SD card flashing tool like dd, Win32DiskImager, or Balena Etcher, burn the image
```hw_src/sd_card_zcu104/sd_card.img``` to an SD card and boot the board.

This version uses a non-volatile root file system stored on the SD card. Some post-installation
setup is necessary. The following steps are performed on the target post-boot unless otherwise noted.

#### 2. Copy sd_card utilitis
- Copy ```sd_card_utils/*``` to /home/root on the target (we will use these scripts and JSON files in the next step).
  On the host, run:

```bash
$ cd sd_card_utils

$ scp -r * root@<zcu104 board ip>:~

$ scp -r .bashrc root@<zcu104 board ip>:~
```


#### 3. Copy sw stack
- Copy update.tar.gz from ```sw_src/install``` to the root (/) directory on the board using SSH or similar

```bash
$ cd <project_root>/sw_src/install

$ scp update.tar.gz root@<zcu104 board ip>:/
```

***Note: All remaining steps are on the target***

#### 4. resize rootfs
Resize the root file system partition to use remaining space on the sd card. You can verify the used disk space before and after with ```df -h```

```bash
$ cd ~/scripts

$ ./ext4_auto_resize.sh
```

#### 5. Install the software libraries and Vitis AI:

```bash
$ cd scripts

$ ./update.sh

$ ./install_vai.sh
```

***Note: For the Vitis AI install script to run the board must have a path to the internet
(script uses wget to download, so you may be able to set proxy settings). If this is not possible
please check the script source and execute the steps manually***

- Copy additional test data (videos, etc) to the same home/root directory (_Note that the videos
are not part of this repository_)

***At this point you must reboot the board for all configuration changes to take effect. Do not
turn off the power without running ```shutdown -h now```, ```reboot```, or similar***

## Running the application

```bash
$ cd /home/root/scripts

$ ./1ch_crop_reid.sh
```

- ***OPTIONAL:*** Beginning in Vitis 2020.1, a package feed is available for on-target installation
of many different tools. The package manager used is 'dnf' which has command line syntax very
similar to apt or yum. This requires network connectivity and potentially proxy configuration.
To set up this package feed, and to install e.g. the utility ```vim```:

```bash
$ cd /home/root/scripts

$ ./package_feed_setup.sh

$ dnf install vim
```

You may see warnings during DNF package feed setup, these can be ignored.
