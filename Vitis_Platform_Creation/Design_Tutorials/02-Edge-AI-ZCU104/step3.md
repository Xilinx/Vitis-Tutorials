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
    - [Test 3: Run a Vitis-AI Demo](#test-3-run-a-vitis-ai-demo)
      - [Create the design](#create-the-design)
      - [Run Application on Board](#run-application-on-board)
      - [Known Issues](#known-issues)
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

## Test 3: Run a Vitis AI Demo

>**Note**: Vitis AI for Vitis 2023.1 is not released. This will be updated to Vitis 2023.1 soon after Vitis AI for Vitis 2023.1 released.

<details>
<summary><strong>Vitis AI Demo</strong></summary>

This test will run a Vitis AI test application in the DPU-TRD to verify DPU function on your custom platform. The most instructions below follows [Vitis-AI DPU-TRD document](https://github.com/Xilinx/Vitis-AI/tree/master/dsa/DPU-TRD/prj/Vitis#6-gui-flow).

### Create the Design

1. Add the Vitis AI repository into the Vitis IDE

   1. Launch Vitis IDE if you have not. You can reuse the workspace of vadd application.
   2. Select **Window -> Preferences**.
   3. Go to the Library Repository tab.
   4. Add Vitis AI:
      - Click **Add** button
      - Input ID: vitis-ai
      - Name: Vitis AI
      - Location: Assign a target download directory or keep empty. Vitis will use default path `~/.Xilinx` if this field is empty.
      - Git URL: `https://github.com/Xilinx/Vitis-AI.git`
      - Branch: Verify the branch with your platform. Use `master` for the Vitis AI version that matches Vitis 2021.1. You can use `master` for the latest patched version. Note that the master branch will move forward. At some point `master` branch will point to a new release that may not be compatible with Vitis 2021.2.

      ![missing image](./images/vitis_repo_add_vai.png)

2. Download the Vitis AI library.

   1. Select **Xilinx -> Libraries**.
   2. Find the Vitis AI entry that you just added. Click the Download button on it.
   3. Wait until tVitis AI repository downloads.
   4. Click **OK** to close this window.

   The Vitis IDE will check the upstream status of each repository. If there are updates, it will allow you to download the updates if the source URL is a remote Git repository.

3. Download Vitis AI specific sysroot.

   Since Vitis AI has a different release cycle with PetaLinux, Vitis AI related PetaLinux recipes are released later than PetaLinux release. At the time that this tutorial releases, Vitis AI related recipes are not released yet. You cannot build PetaLinux `sysroot/sdk` with Vitis AI dependencies. Use the pre-built Vitis AI SDK instead.

   1. Download the Vitis AI cross-compile environment setup script: `wget https://raw.githubusercontent.com/Xilinx/Vitis-AI/1.4/setup/mpsoc/VART/host_cross_compiler_setup.sh`.
   2. Update the script for installation area. The default install path is `install_path=~/petalinux_sdk_2021.1`. Since you are using PetaLinux 2021.2, it is recommended that you change `install_path=~/petalinux_sdk_2021.2`.
   3. Run the script to set up the cross compile environment: `./host_cross_compiler_setup.sh`.

   Once Vitis AI recipes are released, this tutorial will update the steps for building Vitis AI dependencies to the `sysroot` using PetaLinux.

4. Create a Vitis AI design on the `zcu104_custom` platform.

   1. Go to menu **File** -> **New** -> **Application Project**
   2. Click **Next** in Welcome page
   3. Select platform **zcu104_custom**. Click **Next**.
   4. Name the project **dpu_trd**, click **next**.
   5. Set Domain to **linux on psu_cortexa53**, set **Sys_root path** to `sysroot` installation path in previous step, for example, `~/petalinux_sdk_2021.2/sysroots/cortexa72-cortexa53-xilinx-linux/`.
   6. Set the **Root FS** to `rootfs.ext4` and **Kernel Image** to Image. These files are located in `zcu104_software_platform/sw_comp` directory, which are generated in Step 2. click **next**.
   7. Select **dsa -> DPU Kernel (RTL Kernel)** and click **Finish** to generate the application.

   ![missing image](images/vitis_add_dpu.png)

5. Update Build Target.

   1. Double-click the system project file `dpu_trd_system.sprj`.
   2. Change Active Build Configuration to **Hardware**

6. Review and update DPU settings for ZCU104. The default created design has the DPU settings for ZCU102.

   1. Open **dpu_conf.vh** from **dpu_trd_kernels/src/prj/Vitis** directory
   2. Update line 37 from `URAM_DISABLE` to `URAM_ENABLE`
   3. Press Ctrl+S to save the changes.

   >**Note**: ZCU104 has ZU7EV device on board. It has less block RAM than ZU9EG on ZCU102 but it has UltraRAM. Turning on UltraRAM can fulfill the on chip memory requirement of the DPU.

7. Update system_hw_link for proper kernel instantiation.

   Since the ZCU104 has less LUT resources than the ZCU102, it is more difficult  to meet the timing closure target if you include the softmax IP in PL like ZCU102. The implementation could take a long time. The Vitis AI DPU-TRD design removes the softmax IP in hardware for ZCU104. When the host application detects no softmax IP in hardware, it calculates softmax with software. The result is identical but the calculation time is different. Since your target is to verify the platform, it is recommended that you remove the softmax kernel in your test application.

   1. Double-click `dpu_trd_system_hw_link.prj`.
   2. In the Hardware Functions window, remove the `sfm_xrt_top` instance by right-clicking it and select **Remove**.

      After removing the `sfx_xrt_top` instance, the remaining instances in Hardware Functions window is DPUCZDX8G with Compute Units = 2.

8. Review system_hw_link v++ for proper kernel instantiation.

   The DPU kernel requires two phase-aligned clocks, 1x clock and 2x clock. The configuration is stored in the example design. It sets up clock and AXI interface connections between the DPU kernel to the platform.

   To review the setup in the project, follow these steps:

   1. Go to **Assistant View**.
   2. Double-click **dpu_trd_system [System]**.
   3. Expand the left tree panel and find **dpu_trd_system -> dpu_trd_system_hw_link -> Hardware -> dpu**.

      ![missing image](./images/hw_link_dpu.png)

   4. Click `...` button on the line of V++ Configuration Settings, it shows the configuration like this:

      ```
      [clock]
      freqHz=300000000:DPUCZDX8G_1.aclk
      freqHz=600000000:DPUCZDX8G_1.ap_clk_2
      freqHz=300000000:DPUCZDX8G_2.aclk
      freqHz=600000000:DPUCZDX8G_2.ap_clk_2

      [connectivity]
      sp=DPUCZDX8G_1.M_AXI_GP0:HPC0
      sp=DPUCZDX8G_1.M_AXI_HP0:HP0
      sp=DPUCZDX8G_1.M_AXI_HP2:HP1
      sp=DPUCZDX8G_2.M_AXI_GP0:HPC0
      sp=DPUCZDX8G_2.M_AXI_HP0:HP2
      sp=DPUCZDX8G_2.M_AXI_HP2:HP3
      ```

   >**Note:** The contents are written to `dpu-link.cfg` during build time and passed to the v++ Linker command line.

   >**Note:** To customize the v++ link configuration, you can add contents in the V++ configuration settings, or create your own configuration file and add `--config <your_config_file.cfg>` to the **V++ Command Line Options** field. If you need to use relative path for the configuration file, the base location is `dpu_trd_system_hw_link/Hardware` directory.

9. Update package options to add dependency models into SD Card

   1. Double-click ``dpu_trd_system.sprj``.
   2. Click ... button on Package options.
   3. Input `--package.sd_dir=../../dpu_trd/src/app`.
   4. Click **OK**.

   All content in the `--package.sd_dir` assigned directory will be added to the FAT32 partition of the `sd_card.img`. Samples and models are packaged for verification.

   The `dpu_trd` in the path name is the application project name in this example. If your project name is different, update the project name accordingly.

10. Build the hardware design.

      1. Select the `dpu_trd_system` system project.
      2. Click the hammer button to build the system project.
      3. The generated SD card image is located at **dpu_trd_system/Hardware/package/sd_card.img**.

>**Note**: Refer to the [Vitis AI document](https://github.com/Xilinx/Vitis-AI/tree/master/dsa/DPU-TRD/prj/Vitis#6-gui-flow) for details about the Vitis AI project creation flow.

### Run Application on Board

1. Write image to SD card.

   1. Copy the `sd_card.img` to a local workstation or laptop with SD card readers.
   2. Write the image to SD card with [Balena Etcher](https://www.balena.io/etcher/) or similar tools.

2. Boot the board.

   1. Insert the SD card to ZCU104.
   2. Set boot mode to SD boot.
   3. Connect USB UART cable.
   4. Power on the board. It should boot Linux properly in a minute.

3. Resize ext4 partition

   1. Connect UART console if it is not connected.
   2. On the ZCU104 board UART console, run `df .` to check current available disk size.

      ```bash
      root@petalinux:~# df .
      Filesystem           1K-blocks      Used Available Use% Mounted on
      /dev/root               564048    398340    122364  77% /
      ```

   3. Run `resize-part /dev/mmcblk0p2` to resize the ext4 partition. Input **Yes** and **100%** for confirming the resize to utilize full of the rest of SD card.

      ```bash
      root@petalinux:~# resize-part /dev/mmcblk0p2
      /dev/mmcblk0p2
      Warning: Partition /dev/mmcblk0p2 is being used. Are you sure you want to continue?
      parted: invalid token: 100%
      Yes/No? yes
      End?  [2147MB]? 100%
      Information: You may need to update /etc/fstab.

      resize2fs 1.45.3 (14-Jul-2019)
      Filesystem at /dev/mmcblk0p2 is mounted on /media/sd-mmcblk0p2; o[   72.751329] EXT4-fs (mmcblk0p2): resizing filesystem from 154804 to 1695488 blocks
      n-line resizing required
      old_desc_blocks = 1, new_desc_blocks = 1
      [   75.325525] EXT4-fs (mmcblk0p2): resized filesystem to 1695488
      The filesystem on /dev/mmcblk0p2 is now 1695488 (4k) blocks long.
      ```

   4. Check available size again to verify that the ext4 partition size is enlarged.

      ```bash
      root@petalinux:~# df . -h
      Filesystem                Size      Used Available Use% Mounted on
      /dev/root                 6.1G    390.8M      5.4G   7% /
      ```

   >**Note**: The available size would be different according to your SD card size.

   >**Note**: **resize-part** is a script that you added in [Step 2](./step2.md). It calls Linux utilities **parted** and **resize2fs** to extend the ext4 partition to take the rest of the SD card.

4. Copy dependency files to home folder.

   ```bash
   # Libraries
   root@petalinux:~# cp -r /mnt/sd-mmcblk0p1/app/samples/ ~
   # Model
   root@petalinux:~# cp /mnt/sd-mmcblk0p1/app/model/resnet50.xmodel ~
   # Host app
   root@petalinux:~# cp /mnt/sd-mmcblk0p1/dpu_trd ~
   # Image to test
   root@petalinux:~# cp /mnt/sd-mmcblk0p1/app/img/bellpeppe-994958.JPEG ~
   ```

5. Run the application.

   ```bash
   root@petalinux:~# env LD_LIBRARY_PATH=samples/lib XLNX_VART_FIRMWARE=/mnt/sd-mmcblk0p1/dpu.xclbin ./dpu_trd bellpeppe-994958.JPEG
   ```

   It would show bell pepper has the highest possibility.

   ```
   score[945]  =  0.992235     text: bell pepper,
   score[941]  =  0.00315807   text: acorn squash,
   score[943]  =  0.00191546   text: cucumber, cuke,
   score[939]  =  0.000904801  text: zucchini, courgette,
   score[949]  =  0.00054879   text: strawberry,
   ```
  
<details>
<summary><strong>Detailed Log</strong></summary>  

   ```bash
   [  196.247066] [drm] Pid 948 opened device
   [  196.250926] [drm] Pid 948 closed device
   [  196.254833] [drm] Pid 948 opened device
   [  196.258679] [drm] Pid 948 closed device
   [  196.269515] [drm] Pid 948 opened device
   [  196.273384] [drm] Pid 948 closed device
   [  196.277243] [drm] Pid 948 opened device
   [  196.281076] [drm] Pid 948 closed device
   [  196.285073] [drm] Pid 948 opened device
   [  196.288984] [drm] Pid 948 closed device
   [  196.293230] [drm] Pid 948 opened device
   [  196.297096] [drm] Pid 948 closed device
   [  196.300963] [drm] Pid 948 opened device
   [  196.307660] [drm] zocl_xclbin_read_axlf The XCLBIN already loaded
   [  196.307672] [drm] zocl_xclbin_read_axlf 1cdede23-0755-458e-8dac-7ef1b3845fa4 ret: 0
   [  196.317747] [drm] bitstream 1cdede23-0755-458e-8dac-7ef1b3845fa4 locked, ref=1
   [  196.325431] [drm] Reconfiguration not supported
   [  196.337206] [drm] bitstream 1cdede23-0755-458e-8dac-7ef1b3845fa4 unlocked, ref=0
   [  196.337361] [drm] Pid 948 opened device
   [  196.348581] [drm] Pid 948 closed device
   [  196.352580] [drm] Pid 948 opened device
   [  196.356638] [drm] bitstream 1cdede23-0755-458e-8dac-7ef1b3845fa4 locked, ref=1
   [  196.356659] [drm] Pid 948 opened device
   [  196.367712] [drm] Pid 948 closed device
   [  196.371560] [drm] Pid 948 opened device
   [  196.375507] [drm] bitstream 1cdede23-0755-458e-8dac-7ef1b3845fa4 locked, ref=2
   [  196.375539] [drm] Pid 948 opened device
   [  196.386590] [drm] Pid 948 closed device
   [  196.390439] [drm] Pid 948 opened device
   [  196.394331] [drm] bitstream 1cdede23-0755-458e-8dac-7ef1b3845fa4 locked, ref=3
   [  196.394822] [drm] Pid 948 opened device
   [  196.405867] [drm] Pid 948 closed device
   [  196.409717] [drm] Pid 948 opened device
   score[945]  =  0.992235     text: bell pepper,
   score[941]  =  0.00315807   text: acorn squash,
   score[943]  =  0.00191546   text:[  196.413579] [drm] bitstream 1cdede23-0755-458e-8dac-7ef1b3845fa4 locked, ref=4
   cucumber, cuke,
   score[939]  =  0.000904801  text: zucchini, co[  197.997865] [drm] bitstream 1cdede23-0755-458e-8dac-7ef1b3845fa4 unlocked, ref=3
   urgette,
   score[949]  =  0.00054879   text: strawberry,
   [  198.010569] [drm] Pid 948 closed device
   [  198.032534] [drm] bitstream 1cdede23-0755-458e-8dac-7ef1b3845fa4 unlocked, ref=2
   [  198.032546] [drm] Pid 948 closed device
   [  198.229797] [drm] bitstream 1cdede23-0755-458e-8dac-7ef1b3845fa4 unlocked, ref=0
   [  198.229803] [drm] Pid 948 closed device
   [  198.241056] [drm] bitstream 1cdede23-0755-458e-8dac-7ef1b3845fa4 unlocked, ref=0
   [  198.241059] [drm] Pid 948 closed device
   [  198.252434] [drm] Pid 948 closed device
   ```

The XRT prints can be eliminated by running `echo 6 > /proc/sys/kernel/printk` before launching the application.

</details>



## Known Issues

The default setting of PMIC (irps5401) on ZCU104 cannot afford to have the DPU running on heavy loading. It may crash. Refer to [DPU TRD Known issues](https://github.com/Xilinx/Vitis-AI/blob/master/dsa/DPU-TRD/prj/Vitis/README.md#553-known-issues) for more information. (Ref: [issue 101](https://github.com/Xilinx/Vitis-Tutorials/issues/101)).

</details>

## Congratulations

You have completed creating a custom platform from scratch and verifying it with a simple vadd application and a relatively complex Vitis AI use cases.

Feel free to check more tutorials in this repository.

## Next Steps

If user need to do iteration for your project, you could go through the [Iteration Guidelines](./Iteration_guideline.md) to do iterations.

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
