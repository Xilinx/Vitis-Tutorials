<!--

Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT


Author: Daniele Bagni, Xilinx Inc
-->

<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ In-Depth Tutorials</h1>
    </td>
 </tr>
</table>

# DSP Design on AI Engine Technology with GUI and Makefile Flows


***Version: Vitis 2022.1***

***Board: VCK190 production***

***Last update: 12 Jan. 2023***



# Table of Contents

[1.0 Introduction](#1-introduction)

[2.0 Tools Setup](#2-tools-setup)

[3.0 GUI based Flow](#3-gui-based-flow)

[4.0 Makefile based Flow](#4-makefile-based-flow)

[5.0 Conclusion](#5-conclusion)

[References](#references)

[Support](#support)

[Acknowledgment](#acknowledgment)

[License](#license)


# 1 Introduction

Developing a DSP algorithm with the AI Engine technology is only one part of the design flow and not necessarily the most complicated one. At the end of the day the DSP kernels implemented into the AI Engine array have to be connected with the Programmable Logic (shortly "PL") datamover kernels and with the ARM A72 CPU host Programmable Subsystem (shortly "PS"): the PL kernels require Vitis HLS as design flow, the PS application requires C/C++ programming with either XRT or OpenCL APIs to manage the kernels at runtime execution on the target board within the petalinux OS.

Building from scratch a system like this is not trivial and it would be better to have a good starting point that could be adapted with few changes, a sort of "system design template" or "toolbox" to use some improper terminology.

This tutorial explains how to design a DSP algorithm and implement it into the AI Engine domain of the Versal device, using the production [VCK190](https://www.xilinx.com/products/boards-and-kits/vck190.html) target board and the [Vitis 2022.1 Unified software platform](https://www.xilinx.com/products/design-tools/vitis/vitis-platform.html) design tools.

The DSP algorithm is a decimation FIR filter taken from the [Vitis DSP Libraries (for AI Engine)](https://github.com/Xilinx/Vitis_Libraries/tree/master/dsp).

You will build a system running on the VCK190 board having:
- a DSP kernel implemented on AI Engine domain (the decimation FIR filter);
- two PL datamover kernels (designed with [HLS](https://www.xilinx.com/support/documentation-navigation/design-hubs/dh0090-vitis-hls-hub.html);
- software application for the ARM host processor using the [XRT](https://www.xilinx.com/products/design-tools/vitis/xrt.html) APIs;
- an SD-card with petalinux OS for booting.


In the following of this document, I assume you have named this tutorial repository ``02-AIE_DSP_with_Makefile_and_GUI`` and placed it into a certain working directory ``${WRK_DIR}`` (for example, in my case I used:
``export WRK_DIR=/media/danieleb/DATA/2022.1/Vitis-Tutorials-2022.1tutorials``.


Here is illustrated how the organization of the folders tree of this tutorial (with emphasis on the folder related to AI Engine, otherwise the tree would explode):
```
/media/danieleb/DATA/2022.1
    |
    ├── Vitis_Libraries
    │   ├── dsp
    │   │   ├── docs
    │   │   ├── ext
    │   │   ├── L1
    │   │   └── L2
    │   │       ├── benchmarks
    │   │       ├── examples
    │   │       ├── include
    │   │       ├── meta
    │   │       └── tests/aie

    ├── Vitis-Tutorials-2022.1
    │   ├── AI_Engine_Development
    │   │   ├── Design_Tutorials
    │   │   ├── Feature_Tutorials
    │   │   └── Introduction
    │   ├── Developer_Contributed
    │   │   └── 01-Versal_Custom_Thin_Platform_Extensible_System
    │   │   └── 02-AIE_DSP_with_Makefile_and_GUI # the ${WRK_DIR}

```


# 2 Tools Setup

## 2.1 Warnings

1. Everything shown in this project was done on an Ubuntu 18.04 Desktop with related Vitis 2022.1 release (with or without update 1).

2. It is recommended to write the SD-card that boots the VCK190 board with the Windows OS utility called Win32 Disk Imager 1.0.


## 2.2 Dos-to-Unix Conversion

In case you might get some strange errors during the execution of the scripts, you have to pre-process -just once- all the``*.sh``, ``*.cpp``, ``*.h`` files with the [dos2unix](http://archive.ubuntu.com/ubuntu/pool/universe/d/dos2unix/dos2unix_6.0.4.orig.tar.gz) utility.
In that case run the following commands from your Ubuntu host PC:
```bash
#sudo apt-get install dos2unix
cd <WRK_DIR> #your working directory
for file in $(find . -name "*.sh" );   do dos2unix ${file}; done
for file in $(find . -name "*.tcl");   do dos2unix ${file}; done
for file in $(find . -name "*.h"  );   do dos2unix ${file}; done
for file in $(find . -name "config");  do dos2unix ${file}; done
for file in $(find . -name "*.c*"  );  do dos2unix ${file}; done
```

## 2.3 Environment Setup

### 2.3.1 Downloads

You need to download the following archives from Xilinx website:

```text
(permissions)    (#bytes)  (archive name)
-rwxrwxrwx 1 danieleb danieleb 79257804751 lug 20 19:22 Xilinx_Unified_2022.1_0420_0327.tar.gz
-rwxrwxrwx 1 danieleb danieleb 56658085481 lug 20 19:04 Xilinx_Vivado_Vitis_Update_2022.1.1_0603_1803.tar.gz
-rwxrwxrwx 1 danieleb danieleb  2615466683 lug 21 10:59 petalinux-v2022.1-04191534-installer.run
-rwxrwxrwx 1 danieleb danieleb       28450 lug 21 17:07 2022.1_PetaLinux_Package_List.xlsx
-rwxrwxrwx 1 danieleb danieleb  1060959796 lug 14 16:33 xilinx-versal-common-v2022.1_04191534.tar.gz
-rwxrwxrwx 1 danieleb danieleb   335442935 lug 25 17:49 Vitis_Libraries-master.zip
-rwxrwxrwx 1 danieleb danieleb   433992874 lug 28 09:18 Vitis-Tutorials-2022.1.zip
-rwxrwxrwx 1 danieleb danieleb    15481576 lug 27 11:33 xrt_202210.2.13.466_18.04-amd64-xrt.deb
```

First install the basic version of Vitis 2022.1 ``Xilinx_Unified_2022.1_0420_0327.tar.gz``, then its update
``Xilinx_Vivado_Vitis_Update_2022.1.1_0603_1803.tar.gz``. Everything must be placed in the folder ``/tools/Xilinx/``.

### 2.3.2 Sudo yes or not

You **might** need ``sudo`` privilege to install the tools on these folders, primarily ``/tools`` and ``/opt``, unless you do not change accordingly the ownership and group of those folders.

In fact if you created the ``/tools`` directory as super-user (or ``root``, or with ``sudo``), whatever you wish to write/install there can be done only by the ``root`` super-user.
This is basic Linux OS behavior. But nobody prevents you to change the group and owner so that you -as normal user – can do what you like there too, as if you were in your ``$HOME`` directory.

So, if you run the following commands (and you must need ``sudo``):
```shell
sudo su
mkdir /tools
mkdir /opt
# -R stays for recursively on each subfolder
chown  -R you_user_name  /tools
chgrp  -R you_user_name  /tools
chown  -R you_user_name  /opt
chgrp  -R you_user_name  /opt
exit
```
then you can install all the above tools and archives without ``sudo`` privilege, just as a normal user.

As a last crosscheck, if you installed the tools as normal user, you will see the hidden folder ``.Xilinx`` inside your ``$HOME`` directory, otherwise if you installed them as super-user, you should see
the hidden folder ``.Xilinx`` inside your ``/root`` directory.


In case of install done with the ``sudo`` privilege, there seems to be only a small issue with on line in the script ``installLibs.sh``:
```shell
su - $user -c "touch $logFile; chmod 777 $logFile"
```

The ``touch`` and ``chmod`` commands can not be executed because the logfile is located below the ``/root`` directory and the shell was switched to a normal user without root privileges.
This means that the script assumes that the installation was done without root privileges. That line in the script could be replaced by following commands:
```shell
sudo touch $logFile
sudo chmod 777 $logFile
```

In conclusion: either the installation is done with root privileges (``sudo``) then the ``installLibs.sh`` needs to be changed or the installation is done as normal user.



### 2.3.3 Installing Common and Platform for VCK190P

I have installed this archive ``xilinx-versal-common-v2022.1_04191534.tar.gz`` in the folder  ``/opt/xilinx/common/`` as reported here:

```text
$ ls -l /opt/xilinx/common/
drwxr-xr-x 3 root  root  4096 gen 29 15:01 xilinx-versal-common-v2022.1
```

Then, you have to execute the following commands to build the sdk folders, as a normal user, according to what discussed in the [1.3.2 Sudo yes or not](#132-sudo-yes-or-not) sub-section:
```shell
cd /opt/xilinx/common/xilinx-versal-common-v2022.1/
./sdk.sh -p -y -d .
```

Then you have to decompress the file ``rootfs.ext4.gz`` directly.


### 2.3.4 Install XRT

The easiest way to download XRT was from the [ALVEO](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/alveo.html) site,
I selected the ALVEO U200 card and then the Ubuntu 18 OS to arrive at this [link](https://www.xilinx.com/bin/public/openDownload?filename=xrt_202210.2.13.466_18.04-amd64-xrt.deb).

Then install the archive via the the following command:
```shell
sudo apt install --reinstall ./xrt_<version>.deb
```

To set ``xrt`` into your terminal you need the following command:

```shell
source /opt/xilinx/xrt/settings.sh
```

which was also put in the script  ``set_proj_env_2022v1.sh``.




### 2.3.5 Petalinux

**WARNING: You cannot install Petalinux into a NFS driver, otherwise the install process will end with a not-predictable error message**

Before installing ``petalinux`` you  should check in the Excel foil ``2022.1_PetaLinux_Package_List.xlsx`` what are all the packages that ``petalinux``requires and eventually install the missing ones.

```
#create the destination folder   
sudo mkdir /petalinux_2022.1
#change permissions



$ chmod 777 ~/Downloads/petalinux-v2022.1-04191534-installer.run  
$ ~/Downloads/petalinux-v2022.1-04191534-installer.run  -d /petalinux_2022.1

INFO: Checking installation environment requirements...
WARNING: This is not a supported OS
INFO: Checking free disk space
INFO: Checking installed tools
INFO: Checking installed development libraries
INFO: Checking network and other services
WARNING: No tftp server found - please refer to "UG1144  PetaLinux Tools Documentation Reference Guide" for its impact and solution
INFO: Checking installer checksum...
INFO: Extracting PetaLinux installer...

LICENSE AGREEMENTS

PetaLinux SDK contains software from a number of sources.  Please review
the following licenses and indicate your acceptance of each to continue.

You do not have to accept the licenses, however if you do not then you may
not use PetaLinux SDK.

Use PgUp/PgDn to navigate the license viewer, and press 'q' to close

Press Enter to display the license agreements
Do you accept Xilinx End User License Agreement? [y/N] > y
Do you accept Third Party End User License Agreement? [y/N] > y
INFO: Installing PetaLinux...
INFO: Checking PetaLinux installer integrity...
INFO: Installing PetaLinux SDK to "/petalinux_2022.1/."
INFO: Installing buildtools in /petalinux_2022.1/./components/yocto/buildtools
INFO: Installing buildtools-extended in /petalinux_2022.1/./components/yocto/buildtools_extended
INFO: PetaLinux SDK has been installed to /petalinux_2022.1/.
```

To set ``petalinux`` into your terminal you need the following command:

```shell
source /petalinux_2022.1/settings.sh
```

which was also placed in the script  ``set_proj_env_2022v1.sh``.


### 2.3.6 Environmental Variables


 Here is my recipe for setting the Linux environmental variables, without any claim it is "the best" solution, but at least it works for me.

Beside Vivado and Vitis environment for 2022.1, the variables below have to be set according to where you have placed them.
The script ``set_proj_env_2022v1.sh`` contain a template to be adapted by the user, to set the tools environment
for designing an embedded system on VCK190 target board.


Once done that, you have to call such script with the command:

```shell
cd ${WRK_DIR}  # this repo folder
source files/scripts/set_proj_env_2022v1
```


# 3 GUI based Flow

## 3.1 Decimation FIR Filter Design with GUI based Flow and Host XRT APIs

In this section you will build the decimation FIR filter DSP system using the Vitis GUI. The PS application will apply XRT APIs. The OS will be petalinux.

Now launch the commands:

```shell
cd ${WRK_DIR}/files
source ./scripts/set_proj_env_2022v1.sh
cd gui-flow/fir-dsplib_prj
vitis --workspace ./xrt_wrk
```

Follow the following steps:

1. Select ``File -> New -> Application Project -> Next`` from the GUI and then select the ``xilinx_vck190_base_202210_1`` platform for your project.

2. Click ``Next`` and give a name to your application project and your system project, for example respectively ``dsplib_fir_AIE`` and ``aie_L2dsplib_system``.

3. Select ``aie_engine`` as target processor and click ``Next`` two times.

4. Select an ``Empty application`` template and click ``Finish``. So far (with steps 1,2,3, 4) you have created the folder for the AI Engine subsystem.  

5. Select ``File -> New -> HW Kernel Project -> Next`` from the GUI and then select again and then select the ``xilinx_vck190_base_202210_1`` platform for your project.

6. Give a name to your PL HW kernel project, for example ``dsplib_fir_PL`` and make sure you are still using the  ``aie_L2dsplib_system``. Click ``Finish``. So far (with steps 5, 6)  you have created the folder for the PL subsystem.

7. Select ``File -> New -> Application Project -> Next`` from the GUI and then select the ``xilinx_vck190_base_202210_1`` platform for your project.

8. Click ``Next`` and give a name to your application project  and make sure you are still using the  ``aie_L2dsplib_system``.

9. Select ``cortexA72`` as target processor and click ``Next``. Then select the ``xrt domain``.

10. You have also to set the path names for the  ``Sysroot, Root FS, Kernel Image`` of the embedded OS. Then click ``Next``. Here are my environment settings for your reference:
    ```text
    - /opt/xilinx/common/xilinx-versal-common-v2021.2/sysroots/cortexa72-cortexa53-xilinx-linux  #Sysroot path
    - /opt/xilinx/common/xilinx-versal-common-v2021.2/rootfs.ext4  #Root FS
    - /opt/xilinx/common/xilinx-versal-common-v2021.2/Image        #Kernel Image
    ```
11. Select an ``Empty application`` template and click ``Finish``. So far (with steps 7, 8, 9, 10, 11) you have created the folder for the PS subsystem.  Now it is time to import all the source files into the Vitis project (still empty at the moment).

12. From Vitis GUI, mouse right click on the ``dsplib_fir_AIE [aiengine]`` folder, select ``Import Sources`` and take all the source files from ``gui-flow/fir-dsplib_prj/aie_src`` and put them into ``dsplib_fir_AIE/src``. Do the same action also for all the data files from ``gui-flow/fir-dsplib_prj/aie_data`` to ``dsplib_fir_AIE/data``.

13. From Vitis GUI, mouse right click on the ``dsplib_fir_PL`` folder, select ``Import Sources`` and take all the source files from ``gui-flow/fir-dsplib_prj/pl_src`` and put them into ``dsplib_fir_PL/src``.

14. From Vitis GUI, mouse right click on the ``dsplib_fir_PS [xrt]`` folder, select ``Import Sources`` and take all the source files from ``gui-flow/fir-dsplib_prj/ps_src`` and put them into ``dsplib_fir_PS/src``.

15. From Vitis GUI, mouse right click on the ``aie_L2dsplib_system_hw_link [pl]``, select ``Import Sources`` and take all the source files from ``gui-flow/fir-dsplib_prj/cfg_src`` and put them into ``aie_L2dsplib_system_hw_link [pl]`` itself.  Now that all the source files have been imported, you should see something like in Figure 3.2-1. It is time to set all the settings to finally build the entire project.

16. Select the ``dsplib_fir_AIE.prj`` file and select the ``Top-Level File`` file named ``fir_graph.cpp``.

17. From Vitis GUI, mouse right click on the ``dsplib_fir_AIE [aiengine]`` domain and select ``C/C++ Build Settings``. Make sure to have selected ``Configuration [All Configurations]``. Add the following further includes to the default ones in ``Input Spec``, then click ``Apply and Close``. You should see something like in Figure 3.2-2.
  ```text
  ${env_var:XILINX_VITIS}/aietools/include
  ${env_var:DSPLIB_ROOT}/L1/src/aie
  ${env_var:DSPLIB_ROOT}/L1/include/aie
  ${env_var:DSPLIB_ROOT}/L2/include/aie
  ${env_var:XILINX_HLS}/include
  ${workspace_loc:/${ProjName}/src}
  ```

18. You have to click on ``aie_L2dsplib_system_hw_link.prj`` file, right click on ``binary_container_1`` then select ``Edit V++ Options settings`` and add ``--config=../system.cfg`` as illustrated into Figure 3.2-3.

19. The ``system.cfg`` file for ``aie_L2dsplib_system_hw_link [pl]`` domain is set as in the following (see Figure 3.2-4):
    ```text
  [connectivity]
  stream_connect=mm2s_1.s:ai_engine_0.DataIn1
  stream_connect=ai_engine_0.DataOut1:s2mm_1.s
    ```

20. Click on the ``dsplib_fir_PL.prj`` file push the ``Add the HW functions`` button and select the ``mm2s`` and ``s2mm`` functions, as illustrated in figure 3.2.5.

21. From Vitis GUI, mouse right click on the ``dsplib_fir_PS [xrt]`` domain and select ``C/C++ Build Settings``.  Make sure to have selected ``Configuration [All Configurations]``.

  - As illustrated in Figure 3.2-6,  for the GCC host compiler the other dialect flag has to be set to
  ```
  -std=c++14
  ```
  then click ``Apply and Close``.

  - As illustrated in  Figure 3.2-7, add the following library search paths (``-L`` flag):
  ```text
  ${env_var:XILINX_VITIS)/aietools/lib/aarch64.o
    ${SYSROOT}/usr/lib
  ```
  - and libraries (``-l`` flag):
  ```text
  xilinxopencl
  xaiengine
  adf_api_xrt
  xrt_core
  xrt_coreutil
  ```
  - As illustrated in Figure 3.2.8, add the following further includes to the default ones in ``Include``, then click ``Apply and Close``.
  ```text
  ${env_var:XILINX_VITIS}/aietools/include
  ${env_var:XILINX_VITIS}/include
  ${env_var:XILINX_HLS}/include
  ${env_var:DSPLIB_ROOT}/L1/src/aie
  ${env_var:DSPLIB_ROOT}/L1/include/aie
  ${env_var:DSPLIB_ROOT}/L2/include/aie
  ${workspace_loc:/dsplib_fir_AIE/src}
  ${workspace_loc:/${ProjName}/src}
  ${SYSROOT}/usr/include/xrt
  ```


![figure1B](files/images/fir_sys.png)

*(Figure 3.2-1) Setting OS paths to compile the host software subsystem*

![figure2B](files/images/fir_aie.png)

*(Figure 3.2-2) Vitis GUI-based flow: C/C++ Build Settings for the AIE subsystem*

![figure3B](files/images/fir_sys_pl.png)

*(Figure 3.2-3) Vitis GUI-based flow: assigning the system.cfg file to the binary container*  

![figure4B](files/images/fir_connect.png)

*(Figure 3.2-4) Vitis GUI-based flow: system.cfg configuration file in details*  

![figure5B](files/images/fir_hls.png)

*(Figure 3.2-5) Vitis GUI-based flow: project settings for the kernels of PL subsystem*

![figure6B](files/images/fir_dialect.png)

*(Figure 3.2-6) Vitis GUI-based flow: settings the C++ dialect to compile the host software subsystem*

![figure7B](files/images/fir_PS_libs.png)

*(Figure 3.2-7) Setting the libraries and related paths to compile the host software subsystem*

![figure8B](files/images/fir_ps_inc.png)

*(Figure 3.2-8) Setting the include paths to compile the host software subsystem*

Once you have written the sd-card you can boot the VCk190 board and at the propmpt you can launch the following commands:
```
mount /dev/mmcblk0p1 /mnt
cd /mnt
./dsplib_fir_PS binary_container_1.xclbin
```

The runtime execution on VCK190 works correctly:

```text
Initializing ADF API...
INFO:    iterations    = 0
INFO:    words SizeIn  = 8192
INFO:    words SizeOut = 4096
INFO:    bytes SizeIn  = 32768
INFO:    bytes SizeOut = 16384
PASSED:  auto my_device = xrt::device(0)
XAIEFAL: INFO: Resource group Avail is created.
XAIEFAL: INFO: Resource group Static is created.
XAIEFAL: INFO: Resource group Generic is created.
PASSED:  auto xclbin_uuid = my_device.load_xclbin(binary_container_1.xclbin)
PASSED:  auto in_0 = xrt::kernel(my_device, xclbin_uuid, "mm2s:{mm2s_1}")
PASSED:  auto in_0_bo = xrt::bo(my_device, bSizeIn, XCL_BO_FLAGS_NONE, in_0.group_id(0) (=1))
PASSED:  auto in_0_bo_mapped = = in_0_bo.map<TYPE_DATA*>()
PASSED:  memcpy(in_0_bo_mapped, cint16input, bSizeIn)
PASSED:  in_0_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE)
PASSED:  auto in_0_run = in_0(in_0_bo, nullptr, 32768)
PASSED:  auto out_0 = xrt::kernel(my_device, xclbin_uuid, "s2mm:{s2mm_1}")
PASSED:  auto out_0_bo = xrt::bo(my_device, bSizeOut, XCL_BO_FLAGS_NONE, out.group_id(0) (=1))
PASSED:  auto out_0_bo_mapped = out_0_bo.map<TYPE_DATA*>()
PASSED:  auto out_0_run = out(out_0_bo, nullptr, 16384)
PASSED:  auto my_graph  = xrt::graph(my_device, xclbin_uuid, "filter")
PASSED:  my_graph.reset()
PASSED:  my_graph.run(0)

INFO:    Waiting for kernels to end...

PASSED:  in_0_run.wait()
PASSED:  out_0_run.wait()
PASSED:  out_0_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE)

INFO:    Veryfing output data vs. golden ones

PASSED:  ./dsplib_fir_PS
```




# 4 Makefile based Flow


## 4.1 Thin Platform from Vitis Tutorial

The [Versal Custom Thin Platform Extensible System](https://github.com/Xilinx/Vitis-Tutorials/tree/2022.1/Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System) tutorial has the following aspects to be considered:
- it is always well maintained and promptly updated along the various Vitis releases;
- by default it generates a ``sd_card.img`` file with ``petalinux`` file system on it and the SD-card has only one partition (FAT32);
- it applies a hierarchy of ``Makefiles`` that should enable an "easier" customization of the system to be designed, without touching or modifying the directories structure and the Makefile themselves too.

Few notes to be mentioned besides what already reported in its related [README.md](https://github.com/Xilinx/Vitis-Tutorials/blob/2022.1/Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System/README.md) file:

1. Do not use the ``sd_card.img`` file generated in the ``package_output_hw``. On the contrary, just format the sd card with a Windows OS utility like [SD Card Formatter (5.0.2 developed by Tuxera Inc)](https://www.sdcard.org/cms/wp-content/uploads/2020/11/SD_Card-Formatter_Press_Release.pdf) in FAT32 format to a size of more than 2GB (at least). Then you can copy directly on it the files from the ``package_output_hw/sd_card`` folder. The reason for this action is that with the ``sd_card.img`` file the VCK190 sometimes does not boot for this particular design.

2. At the prompt on UART terminal digit the ``petalinux`` username and the choose a password (for example ``root``).

Here is the log of the runtime execution of the various applications:

```text
vck190-versal:/home/petalinux# cd /run/media/mmcblk0p1/
vck190-versal:/run/media/mmcblk0p1# ls -l
total 636736
-rwxrwx--- 1 root disk   5167472 Jul 28 11:15 BOOT.BIN
-rwxrwx--- 1 root disk  22000128 Jul 28 11:15 Image
drwxrwx--- 2 root disk     32768 Jul 28  2022 System Volume Information
-rwxrwx--- 1 root disk   9258616 Jul 28 11:15 a.xclbin
drwxrwx--- 5 root disk     32768 Jul 28 11:32 a72_profiling
-rwxrwx--- 1 root disk    424448 Jul 28 11:15 aie_dly_test.exe
-rwxrwx--- 1 root disk      2777 Jul 28 11:15 boot.scr
drwxrwx--- 2 root disk     32768 Jul 28 11:33 data
-rwxrwx--- 1 root disk       709 Jul 28 11:15 init.sh
-rwxrwx--- 1 root disk        12 Jul 28 11:16 platform_desc.txt
-rwxrwx--- 1 root disk 614870276 Jul 28 11:15 rootfs.cpio.gz.u-boot
-rwxrwx--- 1 root disk     31296 Jul 28 11:15 trfo_test.exe
vck190-versal:/run/media/mmcblk0p1#
vck190-versal:/run/media/mmcblk0p1# ./trfo_test.exe a.xclbin
INFO:    Amount of samples     = 1800000
INFO:    Angle  Byte Size      = 3600000
INFO:    Layer  Byte Size      = 3600000
INFO:    XYZ    Byte Size      = 3600000
INFO:    DUT    radialDistance = 5000
CHRONO:  Average Calibration   = 0.069413
INFO:    Loading the XCLBIN...
XAIEFAL: INFO: Resource group Avail is created.
XAIEFAL: INFO: Resource group Static is created.
XAIEFAL: INFO: Resource group Generic is created.
CHRONO:  1263ms -> Load XCLBIN
INFO:    Opening the Input mm2s Kernels...
INFO:    Populating Input Buffers...
CHRONO:  11575ms -> Populating Input Buffers
INFO:    Sync Input Buffers...
CHRONO:  19us -> Sync Input Buffers
INFO:    Starting the Input mm2s Kernels...
INFO:    Opening the Output s2mm Kernels...
INFO:    Starting the Output s2mm Kernels...
INFO:    Opening the DUT Kernel...
INFO:    Starting the DUT Kernel + Waiting for All kernels to Finish...
CHRONO:  2014us -> Run All Kernels + All Kernels Finished
INFO:    Sync Output Buffers...
CHRONO:  11us -> Sync Output Buffers
INFO:    Verifying the results...
PASSED:  ./trfo_test.exe with 0 errors

vck190-versal:/run/media/mmcblk0p1#
vck190-versal:/run/media/mmcblk0p1# ./aie_dly_test.exe a.xclbin
Initializing ADF API...
PASSED:  auto my_device = xrt::device(0)
XAIEFAL: INFO: Resource group Avail is created.
XAIEFAL: INFO: Resource group Static is created.
XAIEFAL: INFO: Resource group Generic is created.
PASSED:  auto xclbin_uuid = my_device.load_xclbin(a.xclbin)
PASSED:  auto my_rtl_ip = xrt::ip(my_device, xclbin_uuid, "subtractor:{subtractor_1}")
PASSED:  auto my_graph  = xrt::graph(my_device, xclbin_uuid, "mygraph_top")
PASSED:  my_graph.reset()
PASSED:  my_graph.run()
Poll subtractor register
  Value Reg0:  222
  Value Reg1:  124
  Value Reg2:  14a
  Value Reg3:  38
. . .

Poll subtractor register
  Value Reg0:  224
  Value Reg1:  124
  Value Reg2:  124
  Value Reg3:  36
PASSED:  my_graph.end()

PASSED:  ./aie_dly_test.exe
```


## 4.2 Decimation FIR filter from Vitis DSP Library

As already stated in subsection [4.1 Thin Platform from Vitis Tutorial](#41-thin-platform-from-vitis-tutorial), the Thin Platform has a lot of advantages, being based on makefile flow, including ease of use and fast compilation time, so in this separate section you will design the  same project of section [3.1 Decimation FIR Filter Design with GUI based Flow and Host XRT APIs](#31-decimation-fir-filter-design-with-gui-based-flow-and-host-xrt-apis),
that is, a decimation-by-2 FIR filter taken from the Vitis DSP Library for AI Engine.

At this point, it should be clear to you that you will implement the design into the Thin Platform makefile based template as an alternative of using the GUI flow.

The project is available in the folder [vck190_fir](files/vck190_fir). To compile it execute the following commands:
```shell
cd ${WRK_DIR}/AIE-DSP-DESIGN-WITH-2022.1/files
source ./scripts/set_proj_env_2022v1.sh
cd vck190_fir
make all TARGET=hw
```

### 4.1.1 What To Change in the Thin Platform

If you look at the original [Thin Platform repository](https://github.com/Xilinx/Vitis-Tutorials/tree/2022.1/Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System)
file system you can see the following folders:

```
Vitis-Tutorials-2022.1/Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System$
.
├── documentation
│   └── readme_files
|
├── ip               # put your IP (kernels) here
│   ├── aie          # AI Engine graph and kernels
│   │   ├── data
│   │   └── src
│   │       └── graph
│   │       └── kernels
│   ├── counter      # RTL kernel  
│   │   └── src
│   ├── mm2s_vadd_s  # HLS kernel
│   │   └── src
│   ├── s2mm_vadd_s  # HLS kernel
│   │   └── src
│   ├── subtractor   # RTL kernel
│   │   └── src
│   ├── vadd_mm      # HLS kernel
│   │   └── src
│   └── vadd_s       # HLS kernel
│       └── src
|
├── linux
│   ├── petalinux
│   ├── src
│   └── yocto
|
├── platform
│   ├── hw
│   │   └── src
│   └── sw
│       └── src
|
├── ps_apps           # put your ARM CPU host code here
│   ├── aie_dly_test  # PS code for AI Engine with XRT APIs
│   ├── vadd_mm_cpp   # PS code for HLS kernel with XRT APIs
│   ├── vadd_mm_ocl   # PS code for HLS kernel with OpenCL APIs
│   └── vadd_s        # PS code for HLS kernel with XRT APIs
|
└── vitis
    └── src
        └── system.cfg
```

 to be compared with the folders in this tutorial:

```   
${WRK_DIR}/files/vck190_fir$
.
├── documentation
│   └── readme_files
|
├── ip                # put your IP (kernels) here
│   ├── aie           # AI Engine graph and kernels
│   │   ├── data
│   │   └── src
│   │       └── graph
│   |
│   ├── mm2s_aie      # HLS kernel
│   │   └── src
│   ├── s2mm_aie      # HLS kernel
│      └── src
|
├── linux
│   ├── petalinux
│   │   └── src
│   ├── src
│   └── yocto
│       
├── platform
│   ├── hw
│   │   └── src
│   └── sw
│       └── src
|
├── ps_apps         # put your ARM CPU host code here
│   ├── aie_test    # PS code for AI Engine with XRT APIs
│       └── src
|
└── vitis
   └── src
       └── system.cfg
```

In few words you have to:

1. add your AI Engine graph and kernels code into the ``ip/aie`` folder and adapt the related ``ip/aie/Makefile`` to your case;
2. add your HLS kernels code into the ``ip`` folder, one HLS kernel in each subfolder, for example: ``ip/mm2s_aie`` and ``ip/s2mm_aie``; then check/adapt the related ``ip/mm2s_aie/Makefile`` and ``ip/s2mm_aie/Makefile``;  
3. add your PS code into the ``ps_apps`` folder, for example: ``ps_apps/aie_test``, then check/adapt the related ``ps_apps/aie_test/Makefile``;
4. adapt the ``vitis/src/system.cfg`` configuration file for the Vitis Linker according to your connectivity (in this specific case it is the same of the original Thin Platform repository, so no changes at all);
5. leave untouched the ``ip/Makefiles``, since it orchestrates all the Makefiles of the various subfolders; all the rest will be managed "automatically" by the other Makefiles of the Thin Platform project;
6. finally launch the following commands to build the sd card:
  ```shell
  cd ${WRK_DIR}/files
  source ./scripts/set_proj_env_2022v1.sh
  cd vck190_fir
  make all TARGET=hw
  ```
You can note that some folders (for example ``ip/counter``, ``ip/mm2s_vadd_s``, etc) of the original Thin Platform repository were removed simply because not needed for the purpose of this FIR decimation filter design.

As already mentioned in [4.1 Thin Platform from Vitis Tutorial](#41-thin-platform-from-vitis-tutorial) subsection, do not use directly the ``sd_card.img`` file generated in the ``package_output_hw``. On the contrary, just format the sd card with a Windows OS utility like [SD Card Formatter (5.0.2 developed by Tuxera Inc)](https://www.sdcard.org/cms/wp-content/uploads/2020/11/SD_Card-Formatter_Press_Release.pdf) in FAT32 to a size of more than 2GB (at least) as illustrated in Figure 4.1.1-1. Then you can copy directly on it the files from the ``package_output_hw/sd_card`` folder, as illustrated in Figure 4.1.1-2.

At the prompt on UART terminal digit the ``petalinux`` username and the choose a password -for example ``root``- to login, as reported in Figure 4.1.1-3, then you can launch the application. Here are the commands:

```shell
sudo su
cd /run/media/mmcblk0p1/
source ./init.sh
./aie_test.exe a.clbin
```

The runtime execution of the FIR decimation filter application is illustrated in Figure 4.1.1-4.

![figure1C](files/images/fat32_format_sdcard.png)

*(Figure 4.1.1-1) FAT32 formatting to prepare the sd card for VCK190*

![figure2C](files/images/copy_files_sdcard.png)

*(Figure 4.1.1-2) Copy the files on sd card to boot the VCK190*

![figure3C](files/images/thinplatf_prompt.png)

*(Figure 4.1.1-3) Set the username and password at the VCK190 runtime prompt*

![figure4C](files/images/thinplatf_runtime.png)

*(Figure 4.1.1-4) Log of the FIR decimation filter runtime execution on the VCK190*



# 5 Conclusion

## 5.1 Rationale  

This project tries to create a **design template** with either **makefile-based** or **GUI-based** flow, which might be particularly useful for who needs to design **a DSP system in the Versal device with a mix of PL and AI Engine kernels** and requires more expertise to do that.

The Versal AI Engine technology is indeed very sophisticated and the beginner can become easily overwhelmed by the great variety of tutorials, libraries and documentation, each one written with a different style and highlighting different features. Furthermore this technology is always evolving  with giant steps, especially in the design tools, so if you are not working on it continuously you might feel easily lost when you pass from one release (which you used) to another more recent one (which you never used so far).

A robust and well consolidated design template can be extremely helpful and avoid wasting your time with trials and errors.

## 5.2 Summary   

Based on these premises, I walked through some tutorials designed with the makefile-based flow, at least the ones that generate an sd-card for the runtime execution on the target VCK190 board.

I modified the FIR filter design from single rate to decimation, in order to use the decimation FIR filter IP available from the Vitis DSP (AI Engine) Library, the project is placed in the ``make-flow`` folder.

Since there is not yet any official tutorial dealing with an embedded Linux design with GUI-based flow, I ported such decimation FIR filter from makefile- to GUI-based flow, in the ``gui-flow`` folder. Designing the AI Engine kernel subsystem with the GUI - and also the software application with either host APIs (either OpenCL or XRT) on the ARM CPU - is certainly easier and more intuitive than not with the makefile-based flow; this is my personal opinion and experience, hence I consider the ``gui-xrt-dsplibfirdecim_archive.zip`` archive a good design template for the GUI-based flow.

Finally I took the so-called [Versal Custom Thin Platform Extensible System](https://github.com/Xilinx/Vitis-Tutorials/tree/2022.1/Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System) and I customized it with the  same design, see the ``vck190_fir`` folder.

The methodology adopted into such [Thin Platform](https://github.com/Xilinx/Vitis-Tutorials/tree/2022.1/Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System) makes it really easy and flexible to build the entire embedded system composed of PL and AI Engine kernels, petalinux and PS software with XRT APIs. Furthermore the entire [Thin Platform](https://github.com/Xilinx/Vitis-Tutorials/tree/2022.1/Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System)  applies a system of Makefiles hierarchy which does all the work automatically thus requiring minimal changes from the user: **the makefiles only re-compile what changed**.

In particular I explained what you need to do to adapt the [Thin Platform](https://github.com/Xilinx/Vitis-Tutorials/tree/2022.1/Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System) folders structure to your design needs.

## 5.3 Final Remarks

I think that for my future designs on Versal AI Engine I will use a mix of  
GUI- and makefile- based flow, in particular:

- the GUI mainly to develop the AI Engine kernels and the PS software application;

- the makefile of the [Thin Platform](https://github.com/Xilinx/Vitis-Tutorials/tree/2022.1/Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System) to build the entire system.


# References

### [AI Engine Documentation](https://docs.xilinx.com/search/all?filters=Document_ID~%2522UG1076%2522_%2522UG1079%2522&content-lang=en-US)

Contains sections on how to develop AI Engine graphs, how to use the AI Engine compiler, and AI Engine simulation, and performance analysis.

### Vitis DSP Libraries

* [Vitis DSP Libraries Comprehensive Documentation](https://xilinx.github.io/Vitis_Libraries/dsp/2022.1/)

### Xilinx Runtime (XRT) Architecture

Below are links to the XRT information used by this tutorial:

* [XRT Documentation](https://xilinx.github.io/XRT/master/html/index.html): Explains general XRT API calls used in the PS Host Application.

* [XRT Github Repo](https://github.com/Xilinx/XRT): Contains the XRT source code.

* [XRT AI Engine API](https://github.com/Xilinx/XRT/blob/master/src/runtime_src/core/include/experimental/xrt_aie.h): Documents the AI Engine XRT API calls

* [XRT Release Notes](https://docs.xilinx.com/r/en-US/ug1451-xrt-release-notes)

### Vitis Unified Software Development Platform 2022.1 Documentation

Below are links to Vitis related information referenced in this tutorial:

* [Vitis Application Acceleration Development Flow Documentation](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration)

* [Vitis Application Acceleration Development Flow Tutorials](https://github.com/Xilinx/Vitis-Tutorials)

* [Vitis HLS](https://docs.xilinx.com/r/en-US/ug1399-vitis-hls)

### Vitis Model Composer

* [Vitis Model Composer examples](https://github.com/Xilinx/Vitis_Model_Composer)

* [Vitis Model Composer Tutorial](https://docs.xilinx.com/r/en-US/ug1498-model-composer-sys-gen-tutorial/Locating-the-Tutorial-Design-Files)

# Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [forums.xilinx.com](http://forums.xilinx.com/).



# Acknowledgment

A big **thank you** to my colleagues Florent W, Derek H, Bruno V, Ally Z and Ricky S for the great support they gave me during this work.



# License

The MIT License (MIT)

Copyright (c) 2022 Advanced Micro Devices, Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


<p align="center"><sup>XD106 | © Copyright 2022 Xilinx, Inc.</sup></p>
