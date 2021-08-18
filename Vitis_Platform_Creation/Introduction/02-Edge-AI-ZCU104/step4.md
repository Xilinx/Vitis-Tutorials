<!--
# Copyright 2020 Xilinx Inc.
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

- [Step 4: Test the Platform](#step-4-test-the-platform)
  - [Test 1: Read Platform Info](#test-1-read-platform-info)
  - [Test 2: Run Vector Addition Application](#test-2-run-vector-addition-application)
  - [Test 3: Run a Vitis-AI Demo](#test-3-run-a-vitis-ai-demo)
  - [Fast Track](#fast-track)
  - [Congratulations](#congratulations)

## Step 4: Test the Platform

### Test 1: Read Platform Info

With Vitis environment setup, **platforminfo** tool can report XPFM platform information.

<details>

<summary><strong>Click for detailed logs</strong></summary>  

```bash
# in zcu104_custom_pkg directory
platforminfo ./zcu104_custom/export/zcu104_custom/zcu104_custom.xpfm
==========================
Basic Platform Information
==========================
Platform:           zcu104_custom_platform
File:               <your path to>/zcu104_custom_platform.xpfm
Description:        
A custom platform ZCU104 platform

=====================================
Hardware Platform (Shell) Information
=====================================
Vendor:                           xilinx
Board:                            ZCU104_Custom_Platform
Name:                             ZCU104_Custom_Platform
Version:                          0.0
Generated Version:                2021.1
Hardware:                         1
Software Emulation:               1
Hardware Emulation:               1
Hardware Emulation Platform:      0
FPGA Family:                      zynquplus
FPGA Device:                      xczu7ev
Board Vendor:                     xilinx.com
Board Name:                       xilinx.com:zcu104:1.1
Board Part:                       xczu7ev-ffvc1156-2-e

=================
Clock Information
=================
  Default Clock Index: 2
  Clock Index:         1
    Frequency:         100.000000
  Clock Index:         2
    Frequency:         200.000000
  Clock Index:         3
    Frequency:         400.000000

==================
Memory Information
==================
  Bus SP Tag: HP0
  Bus SP Tag: HP1
  Bus SP Tag: HP2
  Bus SP Tag: HP3
  Bus SP Tag: HPC0
  Bus SP Tag: HPC1

=============================
Software Platform Information
=============================
Number of Runtimes:            1
Default System Configuration:  zcu104_custom_platform
System Configurations:
  System Config Name:                      zcu104_custom_platform
  System Config Description:               zcu104_custom_platform
  System Config Default Processor Group:   xrt
  System Config Default Boot Image:        standard
  System Config Is QEMU Supported:         1
  System Config Processor Groups:
    Processor Group Name:      xrt
    Processor Group CPU Type:  cortex-a53
    Processor Group OS Name:   linux
  System Config Boot Images:
    Boot Image Name:           standard
    Boot Image Type:
    Boot Image BIF:            zcu104_custom_platform/boot/linux.bif
    Boot Image Data:           zcu104_custom_platform/xrt/image
    Boot Image Boot Mode:      sd
    Boot Image RootFileSystem:
    Boot Image Mount Path:     /mnt
    Boot Image Read Me:        zcu104_custom_platform/boot/generic.readme
    Boot Image QEMU Args:      zcu104_custom_platform/qemu/pmu_args.txt:zcu104_custom_platform/qemu/qemu_args.txt
    Boot Image QEMU Boot:
    Boot Image QEMU Dev Tree:
Supported Runtimes:
  Runtime: OpenCL
```

</details>

We can verify clock information and memory information are set as expected.



### Test 2: Run Vector Addition Application

Vector addition is the simplest acceleration PL kernel. Vitis can create this application automatically. Running this test can check the AXI control bus, memory interface and interrupt setting in platform are working properly.

1. Creating Vector Addition Application

   - Open Vitis workspace you were using before.
   - Select **File -> New -> Application Project**.
   - Click **next**
   - Select **zcu104_custom** as platform, click **next**.
   - Name the project **vadd**, click **next**.
   - Set Domain to **linux on psu_cortexa53**, set **Sys_root path** to ```<full_pathname_to_zcu104_custom_pkg>/pfm/sysroots/aarch64-xilinx-linux```(as you created by running **sdk.sh**). Set the **Root FS** to rootfs.ext4 and **Kernel Image** to Image. These files are located in `zcu104_custom_plnx/images` directory, which are generated in Step 2. click **next**.
   - Select **System Optimization Examples -> Vector Addition** and click **finish** to generate the application.
   - In the Explorer window double click the **vadd.prj** file to open it, change the **Active Build configuration** from **Emulation-SW** to **Hardware**.
   - Select **vadd_system** in Explorer window and Click **Build** icon in toolbar.

   **Note**: If you cannot see the **zcu104_custom** platform we created, we can add it to platform list of New Project Wizard by selecting the add button and point to **zcu104_custom_pkg/zcu104_custom** directory.

   **Note**: If you'd like to test this application in emulation mode, plese change  **Active Build configuration** from **Emulation-SW** to **Emulation-HW** on Step 8.

2. Running Vector Addition Application on the Board

   - Copy **zcu104_custom_pkg/vadd_system/Hardware/package/sd_card.img** to local if Vitis is running on a remote server.
   - Write **sd_card.img** into SD Card with SD Card image writer applications like Etcher on Windows or dd on Linux.
   - Boot ZCU104 board with the SD card in SD boot mode.
   - Go to auto mounted FAT32 partition

   ```bash
   cd /mnt/sd-mmcblk0p1
   ```

   - Run vadd application

   ```bash
   ./vadd binary_container_1.xclbin
   ```

   - It should show program prints and XRT debug info.

   ```
   TEST PASSED
   ```

3. Test Vector Addition Application in Emulation Mode (Optional)

   - Use **Vitis menu -> Xilinx -> Start/Stop Emulator** to launch QEMU. Project is vadd, configuration is Emulation-HW. Click Start. Wait for Linux to boot. Log in with root/root.
   - Right click **vadd** project (not the vadd_system system project), select **Run as -> Launch on Emulator**

   The result will show on Console tab.

   ```
   Loading: './binary_container_1.xclbin'
   TEST PASSED
   ```

### Test 3: Run a Vitis-AI Demo

Vitis-AI test demo will be added soon.


### Fast Track

Scripts are provided to create the test applications on the custom platform we created. To use these scripts, please run the following steps.

1. Run build

   ```bash
   # cd to the step directory, e.g.
   cd step4_validate
   make all
   ```

   The default verification uses hardware emulation. If you'd like to verify vadd application on hardware board, please run the following command to generate the SD card image.

   ```bash
   cd step4_validate
   make vadd_hw
   ```

2. To clean the generated files, please run

   ```bash
   make clean
   ```

### Congratulations

We have completed creating a custom platform from scratch and verifying it with a simple vadd application and a relatively complex Vitis-AI use cases.

Please feel free to check more tutorials in this repository.

<p align="center"><sup>Copyright&copy; 2021 Xilinx</sup></p>
