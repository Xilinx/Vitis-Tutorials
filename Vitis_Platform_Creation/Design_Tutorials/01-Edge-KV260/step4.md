<!--
# Copyright 2021 Xilinx Inc.
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

<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>2021.1 Vitis™ Platform Creation Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

# Step 4: Test the Platform

- [Step 4: Test the Platform](#step-4-test-the-platform)
  - [Test 1: Read Platform Info](#test-1-read-platform-info)
    - [Test 2: Run Vector Addition Application](#test-2-run-vector-addition-application)
    - [Congratulations](#congratulations)

## Test 1: Read Platform Info

With Vitis environment setup, **platforminfo** tool can report XPFM platform information.

<details>

<summary><strong>Click for detailed logs</strong></summary>  

```bash
# in kv260_custom_pkg directory
platforminfo ./kv260_custom/export/kv260_custom/kv260_custom.xpfm
==========================
Basic Platform Information
==========================
Platform:           kv260_custom_platform
File:               <your path to>/kv260_custom_platform.xpfm
Description:        
A custom platform KV260 platform
    

=====================================
Hardware Platform (Shell) Information
=====================================
Vendor:                           xilinx
Board:                            KV260_Custom_Platform
Name:                             KV260_Custom_Platform
Version:                          0.0
Generated Version:                2021.1
Hardware:                         1
Software Emulation:               1
Hardware Emulation:               1
Hardware Emulation Platform:      0
FPGA Family:                      zynquplus
FPGA Device:                      xck26
Board Vendor:                     xilinx.com
Board Name:                       xilinx.com:kv260:1.1
Board Part:                       XCK26-SFVC784-2LV-C

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
Default System Configuration:  kv260_custom_platform
System Configurations:
  System Config Name:                      kv260_custom_platform
  System Config Description:               kv260_custom_platform
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
    Boot Image BIF:            kv260_custom_platform/boot/linux.bif
    Boot Image Data:           kv260_custom_platform/xrt/image
    Boot Image Boot Mode:      sd
    Boot Image RootFileSystem: 
    Boot Image Mount Path:     /mnt
    Boot Image Read Me:        kv260_custom_platform/boot/generic.readme
    Boot Image QEMU Args:      kv260_custom_platform/qemu/pmu_args.txt:kv260_custom_platform/qemu/qemu_args.txt
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
   - Select **kv260_custom** as platform, click **next**.
   - Name the project **vadd**, click **next**.
   - Set Domain to **linux on psu_cortexa53**, set **Sys_root path** to ```<full_pathname_to_kv260_custom_pkg>/pfm/sysroots/aarch64-xilinx-linux```(as you created by running **sdk.sh**). Set the **Root FS** to rootfs.ext4 and **Kernel Image** to Image. These files are in `kv260_custom_plnx/images` directory, which are generated in Step 2. click **next**.
   - Select **System Optimization Examples -> Vector Addition** and click **finish** to generate the application.
   - In the Explorer window double click the **vadd.prj** file to open it, change the **Active Build configuration** from **Emulation-SW** to **Hardware**.
   - Select **vadd_system** in Explorer window and Click **Build** icon in toolbar.

   **Note**: If you cannot see the **kv260_custom** platform we created, we can add it to platform list of New Project Wizard by selecting the add button and point to **kv260_custom_pkg/kv260_custom** directory.

   **Note**: If you would like to test this application in emulation mode, please change  **Active Build configuration** from **Emulation-SW** to **Emulation-HW** on Step 8.

2. Test Vector Addition Application in Emulation Mode (Optional)

   - Use **Vitis menu -> Xilinx -> Start/Stop Emulator** to launch QEMU. Project is vadd, configuration is Emulation-HW. Click Start. Wait for Linux to boot. Log in with root/root.
   - Right click **vadd** project (not the vadd_system system project), select **Run as -> Launch on Emulator**

   The result will show on Console tab.

   ```bash
   Loading: './binary_container_1.xclbin'
   TEST PASSED
   ```

3. Prepare the files to transfer to the board

   Kria SOM uses `xmutil` to load applications dynamically. The load process includes downloading PL bit stream and loading device tree overlay. `xmutil` requires the application files to be stored in `/lib/firmware/xilinx` directory.

   The files related to this application need to have same file names but different extensions. Since this is a vector addition application, we will call it vadd in this example. The final directory on the board would look like this.

   ```bash
   # On target board
   ls /lib/firmware/xilinx/vadd
   vadd.dtbo
   vadd.bit.bin
   shell.json
   ```

   The bitstream needs to be in bin format so that fpgamanager can load it. Convert bit filt to bit.bin file with the following commands.

   ```bash
   cd vadd_system/Package
   echo 'all:{system.bit}'>bootgen.bif
   bootgen -w -arch zynqmp -process_bitstream bin -image bootgen.bif
   mv system.bit.bin vadd.bit.bin
   ```

   The source of DTBO is generated in Step 2. We need to do some small updates to generate the dtbo and rename it so that the file name is aligned with application name.

   - Open `step2_petalinux/dt_output/pl.dtsi`
   - Update the line of `firmware-name =`, type in `vadd.bit.bin` as its value so that xmutil can load the bitstream. `firmware-name = "vadd.bit.bin";`
   - Compile `pl.dtsi` to dtbo with command `dtc -@ -O dtb -o step4_validate/vadd.dtbo step2_petalinux/dt_output/pl.dtsi`

   Prepare shell.json. You can copy it from other applications or create one with the following contents.

    ```json
    {
      "shell_type" : "XRT_FLAT",
      "num_slots": "1"
    }
    ```

4. Transferring the files to the board

    Make sure the Ethernet cable of SOM Starter Kit is connected. Use SCP or SFTP to upload the files from host to target board.

    ```bash
    # Running on host machine
    scp vadd.dtbo vadd.bit.bin shell.json vadd binary_container_1.xclbin petalinux@<SOM Starter Kit IP>:/home/petalinux
    ```
5. Load the hardware

    ```bash
    # Running on target board
    mkdir /lib/firmware/xilinx/vadd
    cd /home/petalinux
    mv vadd.dtbo vadd.bit.bin shell.json /lib/firmware/xilinx/vadd
    sudo xmutil listapps
    sudo xmutil unloadapp
    sudo xmutil loadapp vadd
    ```


6. Running Vector Addition Application on the Board

   - Run vadd application

   ```bash
   ./vadd binary_container_1.xclbin
   ```

   - It should show program prints.

   ```
    xilinx-k26-starterkit-2021_1:~$ ./vadd binary_container_1.xclbin
    INFO: Reading binary_container_1.xclbin
    Loading: 'binary_container_1.xclbin'
    TEST PASSED
   ```

### Congratulations

We have completed creating a custom platform from scratch and verifying it with a simple vadd application.

Please feel free to check more tutorials in this repository.

<p class="sphinxhide" align="center"><sup>Copyright&copy; 2021 Xilinx</sup></p>
