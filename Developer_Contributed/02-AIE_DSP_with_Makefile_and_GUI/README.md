<!--

Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT

-->

<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ In-Depth Tutorials</h1>
    </td>
 </tr>
</table>

# DSP Design on AI Engine Technology with GUI and Makefile Flows

***Version: Vitis 2023.2***

- Board: VCK190
- Last update: 13 Oct 2023

- STATUS:
  - build on ``xirengwts08``: OK
  - 2023.2-related hyperlinks and archives yet to be updated
  - test on VCK190: OK

## Table of Contents

[1.0 Introduction](#1-introduction)

[2.0 Project Setup](#2-project-setup)

[3.0 Decimation FIR Filter DSP Design](#3-decimation-fir-filter-dsp-design)

[4.0 Using the Thin Platform](#4-using-the-thin-platform)

[References](#references)

[Support](#support)

[License](#license)

## 1 Introduction

Developing a DSP algorithm with the AI Engine technology is only one part of the design flow and not necessarily the most complicated one. At the end of the day, the DSP kernels implemented into the AI Engine array have to be connected with the Programmable Logic (PL) datamover kernels and with the ARM A72 CPU host Programmable Subsystem (PS): the PL kernels require AMD Vitis™ HLS as design flow, the PS application requires C/C++ programming with either XRT or OpenCL APIs to manage the kernels at runtime execution on the target board within the PetaLinux OS.

Building a system from scratch like this is not trivial and it is better to have a good starting point that could be adapted with few changes, a sort of "system design template" or "toolbox" to use some improper terminology.

This tutorial explains how to design a DSP algorithm and implement it into the AI Engine domain of the AMD Versal&trade; device, using the production [VCK190](https://www.xilinx.com/products/boards-and-kits/vck190.html) target board and the [Vitis 2023.2 Unified software platform](https://www.xilinx.com/products/design-tools/vitis/vitis-platform.html) design tools.

The DSP algorithm is a decimation FIR filter taken from the [Vitis DSP Libraries (for AI Engine)](https://github.com/Xilinx/Vitis_Libraries/tree/master/dsp).

You will build a system running on the VCK190 board having:

- A DSP kernel implemented on AI Engine domain (the decimation FIR filter)
- Two PL datamover kernels, designed with [HLS](https://www.xilinx.com/support/documentation-navigation/design-hubs/dh0090-vitis-hls-hub.html)
- Software application for the ARM host processor using the [XRT](https://www.xilinx.com/products/design-tools/vitis/xrt.html) APIs
- An SD card with PetaLinux OS for booting

As you use this document, it is assumed that you have named this tutorial repository ``02-AIE_DSP_with_Makefile_and_GUI`` and placed it in a certain working directory ``${WRK_DIR}``. For example, in this case:
`export WRK_DIR=/media/danieleb/DATA/2023.2/Vitis-Tutorials-2023.2/Developer_Contributed`.

The organization of the folders tree of this tutorial is illustrated here (with emphasis on the folder related to AI Engine; otherwise, the tree would explode):

```
/media/danieleb/DATA/2023.2
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

    ├── Vitis-Tutorials-2023.2
    │   ├── AI_Engine_Development
    │   │   ├── Design_Tutorials
    │   │   ├── Feature_Tutorials
    │   │   └── Introduction
    │   ├── Developer_Contributed                   # the ${WRK_DIR}
    │   │   └── 01-Versal_Custom_Thin_Platform_Extensible_System
    │   │   └── 03-HLS_Code_Optimization    
    │   │   └── 02-AIE_DSP_with_Makefile_and_GUI    # this tutorial
    │   │   |   └── files                           # current directory

```


## 2 Project Setup

### 2.1 Warnings

1. Everything shown in this project used an Ubuntu 18.04 Desktop with related Vitis 2023.2 release (with or without updates).

2. It is recommended to write the SD card that boots the VCK190 board with the Windows OS utility called [Win32 Disk Imager 1.0](https://sourceforge.net/projects/win32diskimager/).

3. Some figures are screenshots related to the earlier release of Vitis and there might be a few differences, although minimal, with the current one.

4. It is recommended that you set correctly the environment before running any script, as described in details in the next subsection 2.3.6. The ``*_sample_env_setup.sh`` [scripts](files/scripts) contain a template to be adapted for your needs, based on the (AMD internal network) Ubuntu server or on a standalone Linux desktop settings adopted in developing this tutorial.  

5. The [run_all.sh](files/run_all.sh) must always be launched **only after** all the variables of previous script have been set at least once, and **only from** the [files](files) folder, which has to be your current directory.

6. Read the entire README.md document carefully before launching any script or Makefile.

### 2.2 Dos-to-Unix Conversion

In case you get some unexpected errors during the execution of the scripts, once pre-process all the``*.sh``, ``*.cpp``, ``*.h`` files with the [dos2unix](http://archive.ubuntu.com/ubuntu/pool/universe/d/dos2unix/dos2unix_6.0.4.orig.tar.gz) utility.
In that case, run the following commands from your Ubuntu host PC:

```bash
#sudo apt-get install dos2unix
cd <WRK_DIR> #your working directory
cd 02-AIE_DSP_with_Makefile_and_GUI/files
source scripts/dos2unix_all.sh
```

### 2.3 Environment Setup

#### 2.3.1 Archives Download

You need the following archives from either AMD/Xilinx Download or GitHub websites, in particular:

- From [Vitis (SW Developer) Downloads](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vitis.html) area, take the **2023.2 Vitis Installer** (it makes the Vitis install process easy) and the **2023.2 Vitis Update**.

- From [Vitis Embedded Platforms](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html) area, take the **Common Images for Embedded Vitis Platforms 2023.2**.

- From [PetaLinux](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-design-tools.html) area, take the **2023.2 PetaLinux Tools Installer**.

- Go to the bottom of [PetaLinux 2023.2 - Product Update Release Notes and Known Issues](https://support.xilinx.com/s/article/000034483?language=en_US) and take **2023.2_PetaLinux_Package_List.xlsx** file, which contains all the packages needed by PetaLinux into your Ubuntu OS computer (install all of them before installing PetaLinux).

- Go to the [Xilinx GitHub page](www.github,com/Xilinx) and zip both the [Vitis Libraries](https://github.com/Xilinx/Vitis_Libraries) and [Vitis Tutorials](https://github.com/Xilinx/Vitis-Tutorials).

- Go to the [AMD Alveo™ Packages](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/alveo.html) area, select release 2023.2 and Ubuntu 18.04 OS, and then  take the **Xilinx Runtime (XRT)** archive.

At the end, you should have the following files:

```text
(permissions)   (#bytes)  (archive name)
-rwxrwxrwx                 Xilinx_Unified_2023.2_*_Lin64.bin
-rwxrwxrwx                 Xilinx_Vivado_Vitis_Update_2023.*.tar.gz
-rwxrwxrwx                 xilinx-versal-common-v2023.2_*.tar.gz
-rwxrwxrwx                 Vitis_Libraries-main.zip
-rwxrwxrwx                 Vitis-Tutorials-2023.2.zip
-rwxrwxrwx                 xrt_202310.*-amd64-xrt.deb
```

First, install the basic version of Vitis 2023.2 via the installer ``Xilinx_Unified_2023.2_*_Lin64.bin``. Then add the update ``Xilinx_Vivado_Vitis_Update_2023.2*.tar.gz``. Everything is placed in the folder ``/tools/Xilinx/``.

#### 2.3.2 Sudo or not Sudo?

You **might** need ``sudo`` privilege to install tools on these folders, primarily ``/tools`` and ``/opt``, unless you change the ownership and group of folders accordingly.

If you created the ``/tools`` directory as a super-user (or ``root``, or with ``sudo``), whatever you want to write or install there can only be done by the ``root`` super-user.
This is a basic Linux OS behavior. However,  you can change the group and owner so that you can do what you like as a normal user, the way you do it in your ``$HOME`` directory.

So, if you run the following commands (and you need ``sudo``):

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

Then you can install the above tools and archives without ``sudo`` privilege, just as a normal user.

As a last cross-check, if you installed the tools as a normal user, you should see the hidden folder ``.Xilinx`` inside your ``$HOME`` directory. If you installed them as a super-user, you should see
the hidden folder ``.Xilinx`` inside your ``/root`` directory.

In case of installation done with the ``sudo`` privilege, there seems to be only a small issue with one line in the script ``installLibs.sh``:

```shell
su - $user -c "touch $logFile; chmod 777 $logFile"
```

The ``touch`` and ``chmod`` commands can not be executed because the logfile is located below the ``/root`` directory and the shell was switched to a normal user without root privileges.
This means that the script assumes that the installation was done without root privileges. That line in the script could be replaced by the following commands:

```shell
sudo touch $logFile
sudo chmod 777 $logFile
```

In conclusion, either the installation had root privileges (``sudo``) and the ``installLibs.sh`` needs to be changed, or the installation was done as a normal user.

#### 2.3.3 Install Versal Common

You have to install this archive ``xilinx-versal-common-v2023.2_*.tar.gz`` in the folder  ``/opt/xilinx/common/`` (which has to be into an ``ext4`` file system, not on an ``NTFS`` one), as reported here:

```text
$ ls -l /opt/xilinx/common/
drwxr-xr-x 3 root  root  4096 gen 29 15:01 xilinx-versal-common-v2023.2
```

Then, you have to execute the following commands as a normal user, according to what is discussed in the previous subsection:

```shell
cd /opt/xilinx/common/xilinx-versal-common-v2023.2/
chmod 777 ./sdk.sh # if needed
./sdk.sh -p -y -d .
```

Check if you need to decompress the file ``rootfs.ext4.gz`` directly.
You can then remove the ``sdk.sh`` file to save storage space.

#### 2.3.4 Install XRT

The easiest way to download XRT was from the [Alveo Packages ](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/alveo.html) site. As already discussed, select the Alveo U200 card and then the Ubuntu 18.04 OS to get the ``xrt_202310.*-amd64-xrt.deb`` file.

Then install the archive via the following command:

```shell
sudo apt install --reinstall ./xrt_<version>.deb
```

To set ``xrt`` into your terminal, you need the following command:

```shell
source /opt/xilinx/xrt/settings.sh
```

which is also in the ``*_sample_env_setup.sh`` scripts.

#### 2.3.5 Install Petalinux

> **WARNING: You cannot install PetaLinux into an NFS driver; otherwise, the install process ends with a non-predictable error message**.

Before installing ``petalinux``, check in the Excel foil ``2023.2_PetaLinux_Package_List.xlsx`` what all packages ``petalinux`` requires. Once done, install the missing ones.

```
#create the destination folder   
sudo mkdir /tools/Xilinx/PetaLinux/2023.2
#change permissions

$ chmod 777 ~/Downloads/petalinux-v2023.2-10141622-installer.run  
$ ~/Downloads/petalinux-v2023.2-10141622-installer.run -d /tools/Xilinx/PetaLinux/2023.2

...

INFO: Installing PetaLinux...
INFO: Checking PetaLinux installer integrity...
INFO: Installing PetaLinux to "/tools/Xilinx/PetaLinux/2023.2/."
INFO: Installing buildtools in /tools/Xilinx/PetaLinux/2023.2/./components/yocto/buildtools
INFO: Installing buildtools-extended in /petalinux_2023.2/./components/yocto/buildtools_extended
INFO: PetaLinux has been installed to /tools/Xilinx/PetaLinux/2023.2/.
```

To set ``petalinux`` into your terminal, you need the following command:

```shell
source /tools/Xilinx/PetaLinux/2023.2/settings.sh
```

which is also in the ``*_sample_env_setup.sh`` scripts.

#### 2.3.6 Set Environmental Variables

The ``*_sample_env_setup.sh`` [scripts](files/scripts) contain a template to be adapted by the user to set the tools environment for designing an embedded system on the VCK190 target board.

Once done, call the script with the following command:

```shell
cd <WRK_DIR>  #your working directory
cd 02-AIE_DSP_with_Makefile_and_GUI/files
# set your environment
source files/scripts/<YOUR_OWN>_sample_env_setup.sh
```

## 3 Decimation FIR Filter DSP Design

In this section, you will build the decimation FIR filter DSP system using the Vitis GUI. The PS application will apply XRT APIs. The OS is PetaLinux.


### 3.1 GUI-based Flow

Now launch the commands:

```shell
cd <WRK_DIR> #your working directory
cd 02-AIE_DSP_with_Makefile_and_GUI/files
# set your environment
#source files/scripts/<YOUR_OWN>_sample_env_setup.sh
# call the Vitis GUI
cd gui-flow/fir-dsplib_prj
vitis --workspace ./xrt_wrk
```

Follow the following steps:

1. Select ``File -> New -> Application Project -> Next`` from the GUI and then select the ``xilinx_vck190_base_202310_1`` platform for your project.

1. Click ``Next`` and give a name to your application project and your system project. For example, ``dsplib_fir_AIE`` and ``aie_L2dsplib_system``, respectively.

1. Select ``aie_engine`` as the target processor and click ``Next`` twice.

1. Select an ``Empty application`` template and click ``Finish``. Following the above steps, you have created the folder for the AI Engine subsystem.  

1. Select ``File -> New -> HW Kernel Project -> Next`` from the GUI, select again, and then select the ``xilinx_vck190_base_202310_1`` platform for your project.

1. Give a name to your PL HW kernel project. For example, ``dsplib_fir_PL``. Ensure that you are still using the  ``aie_L2dsplib_system``. Click ``Finish``. With steps 5 and 6,  you have created the folder for the PL subsystem.

1. Select ``File -> New -> Application Project -> Next`` from the GUI and then select the ``xilinx_vck190_base_202310_1`` platform for your project.

1. Click ``Next`` and give a name to your application project  and ensure that you are still using the  ``aie_L2dsplib_system``.

1. Select ``cortexA72`` as the target processor and click ``Next``. Then select the ``xrt domain``.

1. Set the path names for the  ``Sysroot, Root FS, Kernel Image`` of the embedded OS. Then click ``Next``. Here are the environment settings for your reference:

    ```text
    - /opt/xilinx/common/xilinx-versal-common-v2023.2/sysroots/cortexa72-cortexa53-xilinx-linux  #Sysroot path
    - /opt/xilinx/common/xilinx-versal-common-v2023.2/rootfs.ext4  #Root FS
    - /opt/xilinx/common/xilinx-versal-common-v2023.2/Image        #Kernel Image
    ```

1. Select an ``Empty application`` template and click ``Finish``. With steps 7 to 11, you have created the folder for the PS subsystem.  Now it is time to import all the source files into the Vitis project (still empty at the moment).

1. From Vitis GUI, right-click on the ``dsplib_fir_AIE [aiengine]`` folder, select ``Import Sources`` and take all the source files from ``files/src/ip/aie/src`` and put them into ``dsplib_fir_AIE/src``. Copy the entire subfolder ``files/src/ip/aie/src/graphs`` into ``dsplib_fir_AIE/src/``. Repeat the step for all the data files from ``files/src/ip/aie/data`` to ``dsplib_fir_AIE/data``.

1. From Vitis GUI, right-click on the ``dsplib_fir_PL`` folder, select ``Import Sources`` and take all the source files from the two folders ``files/src/ip/mm2s_aie/src`` and ``files/src/ip/s2mm_aie/src`` and put them into ``dsplib_fir_PL/src``.

1. From Vitis GUI, right-click on the ``dsplib_fir_PS [xrt]`` folder, select ``Import Sources`` and take all the source files from ``files/src/ps_apps/aie_test/src`` and put them into ``dsplib_fir_PS/src``.

1. From Vitis GUI, right-click on the ``aie_L2dsplib_system_hw_link [pl]``, select ``Import Sources`` and take the ``files/src/vitis/src/system.cfg`` file and put it into ``aie_L2dsplib_system_hw_link [pl]`` itself.  Now that all the source files have been imported, you should see something like in Figure 3.1-1. Now set up all the settings to finally build the entire project.

1. Select the ``dsplib_fir_AIE.prj`` file and select the ``Top-Level File`` file named ``fir_graph.cpp``.

1. From Vitis GUI, right-click on the ``dsplib_fir_AIE [aiengine]`` domain and select ``C/C++ Build Settings``. Make sure to select ``Configuration [All Configurations]``. Add the following directories to the default ones in ``Input Spec``, then click ``Apply and Close``. You should see something like in Figure 3.2-2.

  ```text
  ${env_var:XILINX_VITIS}/aietools/include
  ${env_var:DSPLIB_ROOT}/L1/src/aie
  ${env_var:DSPLIB_ROOT}/L1/include/aie
  ${env_var:DSPLIB_ROOT}/L2/include/aie
  ${env_var:XILINX_HLS}/include
  ${workspace_loc:/${ProjName}/src}
  ${workspace_loc:/${ProjName}/src/graphs}
  ${workspace_loc:/${ProjName}/data}
  ```

1. Click on ``aie_L2dsplib_system_hw_link.prj`` file, right-click on ``binary_container_1``, select ``Edit V++ Options settings``, and add ``--config=../system.cfg`` as illustrated in Figure 3.2-3.

1. The ``system.cfg`` file for ``aie_L2dsplib_system_hw_link [pl]`` domain is set as in the following (see Figure 3.2-4):

  ```
  [connectivity]
  stream_connect=mm2s_1.s:ai_engine_0.DataIn1
  stream_connect=ai_engine_0.DataOut1:s2mm_1.s
```

1. Click on the ``dsplib_fir_PL.prj`` file. Push the ``Add the HW functions`` button and select the ``mm2s`` and ``s2mm`` functions, as illustrated in Figure 3.2.5.

1. From Vitis GUI, right-click on the ``dsplib_fir_PS [xrt]`` domain and select ``C/C++ Build Settings``.  Make sure to select ``Configuration [All Configurations]``.

  - As illustrated in Figure 3.2-6,  for the GCC host compiler, the other dialect flag has to be set to

  ```
  -std=c++14
  ```

  Then click ``Apply and Close``.

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

  - As illustrated in Figure 3.2.8, add the following directories to the default ones in ``Include``, then click ``Apply and Close``.

  ```text
  ${env_var:XILINX_VITIS}/aietools/include
  ${env_var:XILINX_VITIS}/include
  ${env_var:XILINX_HLS}/include
  ${env_var:DSPLIB_ROOT}/L1/src/aie
  ${env_var:DSPLIB_ROOT}/L1/include/aie
  ${env_var:DSPLIB_ROOT}/L2/include/aie
  ${workspace_loc:/dsplib_fir_AIE/src}
  ${workspace_loc:/dsplib_fir_AIE/src/graphs}
  ${SYSROOT}/usr/include/xrt
  ```


![figure1B](files/images/fir_sys.png)

*(Figure 3.2-1) Setting OS paths to compile the host software subsystem.*

![figure2B](files/images/fir_aie.png)

*(Figure 3.2-2) Vitis GUI-based flow: C/C++ Build Settings for the AIE subsystem.*

![figure3B](files/images/fir_sys_pl.png)

*(Figure 3.2-3) Vitis GUI-based flow: assigning the system.cfg file to the binary container.*  

![figure4B](files/images/fir_connect.png)

*(Figure 3.2-4) Vitis GUI-based flow: system.cfg configuration file in details.*  

![figure5B](files/images/fir_hls.png)

*(Figure 3.2-5) Vitis GUI-based flow: project settings for the kernels of PL subsystem.*

![figure6B](files/images/fir_dialect.png)

*(Figure 3.2-6) Vitis GUI-based flow: settings the C++ dialect to compile the host software subsystem.*

![figure7B](files/images/fir_PS_libs.png)

*(Figure 3.2-7) Setting the libraries and related paths to compile the host software subsystem.*

![figure8B](files/images/fir_ps_inc.png)

*(Figure 3.2-8) Setting the include paths to compile the host software subsystem.*

### 3.2 Makefile-based Flow

Alternatively, to the GUI-based flow, use Vitis from command line with the proper makefile by typing the following:

```shell
cd <WRK_DIR> #your working directory
cd 02-AIE_DSP_with_Makefile_and_GUI/files
# set your environment
#source files/scripts/<YOUR_OWN>_sample_env_setup.sh
# launch makefile-based flow
#source ./run_all.sh
cd make-flow
make all
```

The makefiles here adopted were automatically generated by Vitis (Eclipse) GUI in the previous section and then manually modified to use the environmental variables and avoid absolute path names.  

### 3.3 Test on VCK190 Target Board

Whatever flow you have selected based on either GUI or Makefile, once you have written the generated SD card, you can boot the VCK190 board.

At the prompt on UART terminal digit, insert the ``petalinux`` username and choose a password to login, for example, ``root``.
You should see the following:

```text
versal-rootfs-common-20231 login: petalinux
Password:
versal-rootfs-common-20231:~$ sudo su
Password:
versal-rootfs-common-20231:/home/petalinux# mount /dev/mmcblk0p1 /mnt                                           
versal-rootfs-common-20231:/home/petalinux# cd /mnt

versal-rootfs-common-20231:/mnt# ls -l
total 33549
-rwxrwx---    1 root     disk       4002464 Jan  1  2015 BOOT.BIN
-rwxrwx---    1 root     disk      22268416 Jan  1  2015 Image
drwxrwx---    2 root     disk           512 Apr 14  2023 System Volume Information
-rwxrwx---    1 root     disk       6840118 Jan  1  2015 binary_container_1.xclbin
-rwxrwx---    1 root     disk          3015 Jan  1  2015 boot.scr
-rwxrwx---    1 root     disk       1238840 Jan  1  2015 dsp_fir.exe
versal-rootfs-common-20231:/mnt#
versal-rootfs-common-20231:/mnt#
```

You can now launch the following commands:

```shell
sudo su
mount /dev/mmcblk0p1 /mnt
cd /mnt
./dsplib_fir_PS binary_container_1.xclbin
```

You should see the following:

```
versal-rootfs-common-20231:/mnt# ./dsp_fir.exe ./binary_container_1.xclbin
. . .
INFO:    iterations    = 0
INFO:    words SizeIn  = 8192
INFO:    words SizeOut = 4096
INFO:    bytes SizeIn  = 32768
INFO:    bytes SizeOut = 16384
. . .
PASSED:  my_graph.reset()
PASSED:  my_graph.run(0)
INFO:    Waiting for kernels to end...
PASSED:  in_0_run.wait()
PASSED:  out_0_run.wait()
PASSED:  out_0_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE)
INFO:    Veryfing output data vs. golden ones

PASSED:  ./dsp_fir.exe

versal-rootfs-common-20231:/mnt#
```

## 4 Using the Thin Platform

### 4.1 Get the Thin Platform

The [Versal Custom Thin Platform Extensible System](https://github.com/Xilinx/Vitis-Tutorials/tree/2022.1/Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System) tutorial has the following aspects to be considered:

- It is always well-maintained and promptly updated along the various Vitis releases.
- By default, it generates a ``sd_card.img`` file with ``petalinux`` file system on it and the SD card has only one partition (FAT32).
- It applies a hierarchy of ``Makefiles`` that should enable an "easier" customization of the system to be designed, without touching or modifying the directories structure and the Makefile themselves too.

Few notes to be mentioned besides what is already reported in its related [README.md](https://github.com/Xilinx/Vitis-Tutorials/blob/2022.1/Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System/README.md) file:

1. Do not use the ``sd_card.img`` file generated in the ``package_output_hw``. On the contrary,  format the SD card with a Windows OS utility like [SD Card Formatter (5.0.2 developed by Tuxera Inc)](https://www.sdcard.org/cms/wp-content/uploads/2020/11/SD_Card-Formatter_Press_Release.pdf) in FAT32 format to a size of more than 2GB (at least). Then you can directly copy the files from the ``package_output_hw/sd_card`` folder. The reason is that with the ``sd_card.img`` file, the VCK190 sometimes does not boot for this particular design.

2. At the prompt on UART terminal digit, insert the ``petalinux`` username and  choose a password (for example, ``root``).


### 4.2 Integrate the FIR Filter in the Thin Platform

As already stated in the previous subsection, the Thin Platform has several advantages, being based on makefile flow, including ease of use, and fast compilation time. In this separate section, you will design the same project of section [3 Decimation FIR Filter DSP Design](#31-decimation-fir-filter-dsp-design),
that is, a decimation-by-2 FIR filter taken from the Vitis DSP Library for AI Engine.
This time, you will implement such a design into the Thin Platform makefile based template as an alternative to using the two flows described in subsection 3.1 (GUI-based flow) and 3.2 (Makefile-based flow).

The [prepare_thin-plat_files.sh](files/scripts/prepare_thin-plat_files.sh) script called from within [run_all.sh](files/run_all.sh) script copies the Thin Platform folder and renames it into ``thin-plat``. It then removes folders (``ip/counter``, ``ip/mm2s_vadd_s``, ``ip/s2mm_vadd_s``, ``ip/subtractor``) that are not required for this FIR filter design. It then changes other files, as explained in the next section.

```shell
cd <WRK_DIR> #your working directory
cd 02-AIE_DSP_with_Makefile_and_GUI/files #current dir
# set your environment
#source files/scripts/<YOUR_OWN>_sample_env_setup.sh
# source run_all.#
source ./scripts/prepare_thin-plat-files.sh
```

#### 4.2.1 What To Change in the Thin Platform

The original Thin Platform has the following subfolders:

```
01-Versal_Custom_Thin_Platform_Extensible_System/
├── documentation
├── linux
├── ps_apps
│   └── linux
│       ├── aie_dly_test
│       ├── vadd_mm_cpp
│       ├── vadd_mm_ocl
│       └── vadd_s
├── vitis
│   ├── ip
│   │   ├── aie
│   │   ├── counter
│   │   ├── mm2s_vadd_s
│   │   ├── s2mm_vadd_s
│   │   ├── subtractor
│   │   ├── vadd_mm
│   │   ├── vadd_s
│   ├── src
│   └── xpfm_export
└── vivado
```

while this project requires the following folders:

```   
thin-plat/
├── linux
├── ps_apps
│   └── linux
│       └── aie_test
├── vitis
│   ├── ip
│   │   ├── aie
│   │   ├── mm2s_aie
│   │   ├── s2mm_aie
│   ├── src
│   └── xpfm_export
└── vivado
```

therefore you should have to do manually the following changes:

1. Add your AI Engine graph and kernels code into the ``vitis/ip/aie`` folder and adapt the related ``vitis/ip/aie/Makefile`` to your case.
1. Add your HLS kernels code into the ``vitis/ip`` folder, one HLS kernel in each subfolder, for example, ``vitis/ip/mm2s_aie`` and ``vitis/ip/s2mm_aie``. Then check or adapt the related ``vitis/ip/mm2s_aie/Makefile`` and ``vitis/ip/s2mm_aie/Makefile``.
1. Add your PS code into the ``ps_apps/linux`` folder. For example, ``ps_apps/linux/aie_test``. Then check or adapt the related ``ps_apps/linux/aie_test/Makefile``.
1. Adapt the ``vitis/src/system.cfg`` configuration file for the Vitis Linker according to your connectivity (in this specific case, it is the same as the original Thin Platform repository, so no changes at all).
1. Do not modify the ``vitis/ip/Makefiles`` because it orchestrates all the Makefiles of the various subfolders. The remaining ones are managed "automatically" by the other Makefiles of the Thin Platform project.

To provide ease, all above steps are already done by the script [prepare_thin-plat_files.sh](files/scripts/prepare_thin-plat_files.sh) and the Makefiles are already changed and available in this repository.

Run the following commands:

```shell
cd <WRK_DIR> #your working directory
cd 02-AIE_DSP_with_Makefile_and_GUI/files #cur dir
# set your environment
#source files/scripts/<YOUR_OWN>_sample_env_setup.sh
#prepare the Thin Platform folder
source ./scripts/prepare_thin-plat_files.sh
```

Now you can build the SD card with the following commands:

```shell
cd <WRK_DIR> #your working directory
cd 02-AIE_DSP_with_Makefile_and_GUI/files
# set your environment
#source files/scripts/<YOUR_OWN>_sample_env_setup.sh
cd thin-plat
make all TARGET=hw
```

As already mentioned in the previous subsection, do not use  the ``sd_card.img`` file generated in the ``package_output_hw`` directly. On the contrary, just format the SD card with a Windows OS utility like [SD Card Formatter (5.0.2 developed by Tuxera Inc)](https://www.sdcard.org/cms/wp-content/uploads/2020/11/SD_Card-Formatter_Press_Release.pdf) in FAT32 to a size of more than 2GB (at least) as illustrated in Figure 4.2.1-1. Then you can directly copy the files from the ``package_output_hw/sd_card`` folder, as illustrated in Figure 4.2.1-2.

![figure1C](files/images/fat32_format_sdcard.png)

*(Figure 4.2.1-1) FAT32 formatting to prepare the sd card for VCK190.*

![figure2C](files/images/copy_files_sdcard.png)

*(Figure 4.2.1-2) Copy the files on sd card to boot the VCK190.*

### 4.3 Run on VCK190 Target Board

At the prompt on UART terminal digit, insert the ``petalinux`` username and choose a password to login. For example, ``root``. You should see the following:

```text
versal-rootfs-common-20231 login: petalinux
You are required to change your password immediately (administrator enforced).
New password:
Retype new password:

versal-rootfs-common-20231:~$ sudo su

We trust you have received the usual lecture from the local System
Administrator. It usually boils down to these three things:
    #1) Respect the privacy of others.
    #2) Think before you type.
    #3) With great power comes great responsibility.
Password:
```

You can now launch the following commands:

```shell
sudo su
mount /dev/mmcblk0p1 /mnt  
cd /mnt
./xrt_host.exe ./*.xclbin
```

You should see something similar to this:

```text
versal-rootfs-common-20231:/mnt# ./xrt_host.exe ./a.xclbin

INFO:    iterations    = 0
INFO:    words SizeIn  = 8192
INFO:    words SizeOut = 4096
INFO:    bytes SizeIn  = 32768
INFO:    bytes SizeOut = 16384
PASSED:  auto my_device = xrt::device(0)
PASSED:  auto xclbin_uuid = my_device.load_xclbin(./a.xclbin)
PASSED:  auto in_0 = xrt::kernel(my_device, xclbin_uuid, "mm2s:{mm2s_1}")
. . .

PASSED:  my_graph.reset()
PASSED:  my_graph.run(0)
INFO:    Waiting for kernels to end...
PASSED:  in_0_run.wait()
PASSED:  out_0_run.wait()
PASSED:  out_0_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE)
INFO:    Veryfing output data vs. golden ones

PASSED:  ./xrt_host.exe

versal-rootfs-common-20231:/mnt#
```



## References

### [AI Engine Documentation](https://docs.xilinx.com/search/all?filters=Document_ID~%2522UG1076%2522_%2522UG1079%2522&content-lang=en-US)

Contains sections on how to develop AI Engine graphs, how to use the AI Engine compiler and AI Engine simulation, and performance analysis.

### Vitis DSP Libraries

* [Vitis DSP Libraries Comprehensive Documentation](https://xilinx.github.io/Vitis_Libraries/dsp/2022.1/)

### Xilinx Runtime (XRT) Architecture

Below are the links to the XRT information used by this tutorial:

* [XRT Documentation](https://xilinx.github.io/XRT/master/html/index.html): Explains general XRT API calls used in the PS Host Application.

* [XRT Github Repo](https://github.com/Xilinx/XRT): Contains the XRT source code.

* [XRT AI Engine API](https://github.com/Xilinx/XRT/blob/master/src/runtime_src/core/include/experimental/xrt_aie.h): Documents the AI Engine XRT API calls.

* [XRT Release Notes](https://docs.xilinx.com/r/en-US/ug1451-xrt-release-notes)

### Vitis Unified Software Development Platform 2022.1 Documentation

Below are links to Vitis related information referenced in this tutorial:

* [Vitis Application Acceleration Development Flow Documentation](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration)

* [Vitis Application Acceleration Development Flow Tutorials](https://github.com/Xilinx/Vitis-Tutorials)

* [Vitis HLS](https://docs.xilinx.com/r/en-US/ug1399-vitis-hls)

### Vitis Model Composer

* [Vitis Model Composer examples](https://github.com/Xilinx/Vitis_Model_Composer)

* [Vitis Model Composer Tutorial](https://docs.xilinx.com/r/en-US/ug1498-model-composer-sys-gen-tutorial/Locating-the-Tutorial-Design-Files)

#### Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [support.xilinx.com](https://support.xilinx.com/).


### License

The MIT License (MIT)

Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.

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

<p class="sphinxhide" align="center">&copy; Copyright 2023 Advanced Micro Devices, Inc.</p>
