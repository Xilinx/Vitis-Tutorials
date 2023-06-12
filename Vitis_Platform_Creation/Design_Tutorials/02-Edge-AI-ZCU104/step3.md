<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Platform Creation Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

# Step 3: Test the Platform

- [Step 3: Test the Platform](#step-3-test-the-platform)
    - [Test 1: Read Platform Info](#test-1-read-platform-info)
    - [Test 2: Run Vector Addition Application](#test-2-run-vector-addition-application)
    - [Fast Track for Vector Addition](#fast-track-for-vector-addition)
    - [Congratulations](#congratulations)

## Test 1: Read Platform Info

Using the AMD Vitis integrated development environment (IDE) setup in step2, the **platforminfo** tool can report XPFM platform information.

<details>
<summary><strong>Click for detailed logs</strong></summary>  

```bash
# Run in zcu104_software_platform directory
platforminfo ./zcu104_custom/export/zcu104_custom/zcu104_custom.xpfm

==========================
Basic Platform Information
==========================
Platform:           zcu104_custom
File:               /Vitis-Tutorials/Vitis_Platform_Creation/Design_Tutorials/02-Edge-AI-ZCU104/ref_files/step2_pfm/zcu104_custom/export/zcu104_custom/zcu104_custom.xpfm
Description:        
A custom platform ZCU104 platform
    

=====================================
Hardware Platform (Shell) Information
=====================================
Vendor:                           xilinx
Board:                            ZCU104_Custom_Platform
Name:                             ZCU104_Custom_Platform
Version:                          0.0
Generated Version:                2023.1
Hardware:                         1
Software Emulation:               1
Hardware Emulation:               0
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
Default System Configuration:  zcu104_custom
System Configurations:
  System Config Name:                      zcu104_custom
  System Config Description:               zcu104_custom
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
    Boot Image BIF:            zcu104_custom/boot/linux.bif
    Boot Image Data:           zcu104_custom/xrt/image
    Boot Image Boot Mode:      sd
    Boot Image RootFileSystem: 
    Boot Image Mount Path:     /mnt
    Boot Image Read Me:        zcu104_custom/boot/generic.readme
    Boot Image QEMU Args:      zcu104_custom/qemu/pmu_args.txt:zcu104_custom/qemu/qemu_args.txt
    Boot Image QEMU Boot:      
    Boot Image QEMU Dev Tree:  
    Supported Runtimes:
  Runtime: OpenCL
```

</details>

You can verify clock information and memory information are set as expected.

## Test 2: Run Vector Addition Application

Vector addition is the simplest acceleration PL kernel. The Vitis IDE can create this application automatically. Running this test can check the AXI control bus, memory interface, and interrupt setting in platform are working properly.

1. Create a application directory

   ```bash
   cd WorkSpace
   mkdir zcu104_application_vitis
   ```

2. Creating Vector Addition Application

   1. Switch Vitis workspace by selecting **File -> Switch Workspace -> other -> Brose** to the directory you created. Here is **zcu104_application_vitis**.
   2. Select **File -> New -> Application Project**.
   3. Click **Next**.
   4. Click **add** button and choose the folder of `zcu104_custom` which is  located under `zcu104_software_platform` folder.
   5. Select **zcu104_custom** as platform, click **Next**.
   6. Name the project **vadd**, click **Next**.
   7. Set Domain to **linux on psu_cortexa53**, set **Sys_root path** to ```zcu104_software_platform/xilinx-zynqmp-common-v2023.1/sysroots/cortexa72-cortexa53-xilinx-linux```(as you created by running **sdk.sh** in Step2). Set the **Root FS** to rootfs.ext4 and **Kernel Image** to Image. These files are located in `zcu104_software_platform/pfm/sw_comp` directory, which are prepared in Step 2. click **Next**.
   8. Select **System Optimization Examples -> Vector Addition** and click **finish** to generate the application.
   9. In the Explorer window, double-click the **vadd.prj** file to open it, change the **Active Build configuration** from **Emulation-SW** to **Hardware**.
   10. Select **vadd_system** in Explorer window and click the build icon in toolbar.

   >**Note**: If you want to test this application in emulation mode, change  **Active Build configuration** from **Emulation-SW** to **Emulation-HW** on Step 8.

3. Running Vector Addition Application on the Board

   1. Copy ``zcu104_application_vitis/vadd_system/Hardware/package/sd_card.img`` to local if the Vitis IDE is running on a remote server.

   2. Write ``sd_card.img`` onto the SD Card with SD Card image writer applications like Etcher on Windows or dd on Linux.

   3. Boot ZCU104 board with the SD card in SD boot mode.

      <details>
      <summary><strong>Steps to run the application:</strong></summary>

      First, login with user `petalinux` and set up a new password (it is then also the sudo password):

      1. Log into the system
      
         ```bash
         petalinux login:petalinux
         You are required to change your password immediately (administrator enforced).
         New password:
         Retype new password:
         petalinux:~$ sudo su
         We trust you have received the usual lecture from the local System
         Administrator. It usually boils down to these three things:
               #1) Respect the privacy of others.
               #2) Think before you type.
               #3) With great power comes great responsibility.
         Password:
         petalinux:/home/petalinux#
         ```

      2. Go to auto mounted FAT32 partition

         ```bash
         petalinux:/home/petalinux# cd /run/media/mmcblk0p1/
         ```

      3. Run vadd application

         ```bash
         ./simple_vadd krnl_vadd.xclbin
         ```

      It should show program prints and XRT debug information .

      ```
      TEST PASSED
      ```

      </details>

4. Test Vector Addition Application in Emulation Mode (Optional)

   1. Select **Vitis menu -> Xilinx -> Start/Stop Emulator** to launch QEMU. Project is vadd, configuration is Emulation-HW. Click **Start** and wait for Linux to boot. 
   2. Log in with root/root.
   3. Right-click **vadd** project (not the vadd_system system project), select **Run as -> Launch on Emulator**.

   The result will show on Console tab.

   ```
   Loading: './binary_container_1.xclbin'
   TEST PASSED
   ```

### Fast Track for Vector Addition

Scripts are provided to create the test applications on the custom platform we created. To use these scripts, please run the following steps.

1. Run build

   ```bash
   # cd to the step directory, e.g.
   cd step3_validate
   make all
   ```

   The default verification uses hardware emulation. To verify vadd application on hardware board, run the following command to generate the SD card image.

   ```bash
   cd step3_validate
   make sd_card
   ```

2. To clean the generated files, run the following commands:

   ```bash
   make clean
   ```

The command line flow has slight differences comparing to Vitis IDE flow.

The vector addition application is called `vadd` and `binary_container_1.xclbin` in the Vitis IDE flow. The generated files in command line flow are called `simple_vadd` and `krnl_vadd.xclbin`.


## Congratulations

You have completed creating a custom platform from scratch and verifying it with a simple vadd application.

Feel free to check more tutorials in this repository.

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
