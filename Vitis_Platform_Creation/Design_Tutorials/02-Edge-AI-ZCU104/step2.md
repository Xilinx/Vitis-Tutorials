<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Platform Creation Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

# Step 2: Create the Vitis Software Platform

In this step, you will create an AMD Vitis™ platform running Linux operation system. The Vitis platform requires several software components which need to be prepared in advance. Since AMD provides common software images for quick evaluation, you will use the common image for a quick start. DTB files are not prepared in the common image package as different platforms have different device peripherals. Use the `createdts` command to generate the device tree file for your platform. If you need to customize the system image like kernel or rootfs, refer to the [PetaLinux customization page](../../Feature_Tutorials/02_petalinux_customization/README.md) for customization. Following are the software components that are required for the platform:

| Component                                     | Conventional Path or Filename | Description                                                      | Provenance                         |
| --------------------------------------------- | ----------------------------- | ---------------------------------------------------------------- | ---------------------------------- |
| Boot components in BOOT.BIN                   | boot/fsbl.elf                 | First stage boot loader                                          | Generated with platform creation   |
| Boot components in BOOT.BIN                   | boot/pmufw.elf                | Chip internal power and security related management              | Generated with platform creation   |
| Boot components in BOOT.BIN                   | boot/bl31.elf                 | ARM trusted firmware/ secure monitor                             | Extracted from common image        |
| Boot components in BOOT.BIN                   | boot/u-boot.elf               | Second stage boot loader                                         | Extracted from common image        |
| Boot components in BOOT.BIN                   | boot/system.dtb               | Device tree information file                                     | Generated from "createdts" command |
| Boot components in FAT32 partition of SD card | sd_dir/boot.scr               | U-boot configuration file to store in FAT32 partition of SD card | Extracted from common image        |
| Linux Software Components                     | sw_comp/Image                 | Linux kernel  Image                                              | Extracted from common image        |
| Linux Software Components                     | sw_comp/rootfs.ext4           | Linux file system                                                | Extracted from common image        |
| Linux SDK                                     | sysroot                       | Cross compile and header files                                   | Extracted from common image        |

As most of the components are extracted from the common image package, you should prepare the common image first.

## Prepare the Common Images

1. Download the common image from [website download page.](https://www.xilinx.com/support/download.html).

2. Navigate to the **WorkSpace** folder that you created in step1 and place the image package in **WorkSpace** folder using the following commands:

   ```bash
   cd WorkSpace
   tree -L 1     # to see the directory hierarchy
   .
   ├── xilinx-zynqmp-common-v2023.1.tar.gz
   ├── zcu104_appliation_vitis
   ├── zcu104_custom_platform
   └── zcu104_software_platform
   ```

3. Extract the common image.

   In this example, you will create a folder named ``zcu104_software_platform`` to store the work content for step2.

   ```bash
   mkdir zcu104_software_platform
   cd zcu104_software_platform
   tar xvf ../xilinx-zynqmp-common-v2023.1.tar.gz -C .
   ```

You can see the ``xilinx-zynqmp-common-v2023.1`` folder which contains some components located in the ``zcu104_software_platform`` folder as shown below:

```
   tree -L 2
   ├── xilinx-zynqmp-common-v2023.1
   │   ├── bl31.elf
   │   ├── boot.scr
   │   ├── Image
   │   ├── README.txt
   │   ├── rootfs.ext4
   │   ├── rootfs.manifest
   │   ├── rootfs.tar.gz
   │   ├── sdk.sh
   │   └── u-boot.elf
```

From the above directory structure, you can see that the boot file, kernel image, rootfs, and SDK tool are ready. DTB and first stage boot related files are not available. In the next steps, you will create a Vitis platform and enable the **Generate boot components** option to generate the FSBL and PMU firmware.

## Create FSBL and PMU Firmware

You can create a platform using either the Vitis IDE or the XSCT console. However, the Vitis IDE only supports one XSA file as input. If your design has two XSA (hw and hw-emu) files, use XSCT to create the platform.

<details>
<summary><strong>Click here to see how to use the Vitis IDE to create a Vitis platform</strong></summary>  

 1. Launch Vitis IDE.
    
    1. Prepare Vitis environment
  
         ```bash
         source <Vitis_tool_install_dir>/settings64.sh
         ```

    2. Launch Vitis by typing `vitis &` in the console.
    3. Select `zcu104_software_platform` folder as workspace directory.

 2. Create a new platform project.

    1. Select **File > New > Platform Project** to create a platform project.
    2. Enter the project name. For this example, type `zcu104_custom_fsbl`. Click **Next**.
    3. In the Platform page,
         - Click **Browse** button, select the XSA file generated by the Vivado. In this case, it is `zcu104_custom_platform_hw.xsa`.
         - Set the operating system to **standalone**.
         - Set the processor to **psu_cortexa53_0**.
         - Set the architecture to **64-bit**.
         - Check the option **Generate boot components**, because the auto generated FSBL and PMU firmware are our goals.
    4. Click **Finish**.

 3. Click **zcu104_custom_fsbl** project in the Vitis Explorer view, then click the **Build** button to generate the platform.

  After building we can get FSBL and PMU firmware.

</details>

<details>
<summary><strong>Click here to see how to use XSCT to create a Vitis platform</strong></summary>  

Create a Tcl file with XSCT commands. For example, here is the parts of [xsct_create_pfm.tcl](./ref_files/step2_pfm/xsct_create_pfm.tcl).

```Tcl
setws .
#set OUTPUT platform_repo
platform create -name $platform_name \
    -desc "A custom platform ZCU104 platform" \
    -hw ${xsa_path}/${xsa_name}_hw.xsa \
    -hw_emu ${xsa_path}/${xsa_name}_hwemu.xsa \
    -fsbl-target psu_cortexa53_0 \
#    -out ./${OUTPUT} 
 
# standalone domain
domain create -name standalone -proc psu_cortexa53_0 -os standalone -arch {64-bit} 
# Linux domain
#domain create -name xrt -proc psu_cortexa53 -os linux -arch {64-bit} -runtime {ocl} -sd-dir {./sd_dir}  -bootmode {sd}
# domain active xrt
#domain config -boot {./boot}
#domain config -generate-bif
#domain config -qemu-data ./boot

platform write
platform generate
```

>**Note:** Replace the file name and directory name in the script with your project file location.

The `platform create` command needs the following input values:

- `-name`: Platform name.
- `-hw`: Hardware XSA file location.
- `-hw_emu`: Hardware emulation XSA file location.
- `-out`: platform output path if you use `setws` to specify the workspace there is no need for this option.
- `-sd-dir`: the directory that contains the files to be included in the FAT32 partition of the SD card image.
- `-fsbl-target`: specify the CPU core and generate corresponding FSBL and PMU image.

>**Note:** If you do not need to support hardware emulation, you can omit the option `-hw_emu` and its value for the command `platform create`.

The `domain` command sets up a standalone domain or the Linux domain with SD boot mode. It uses files in the `./sd_dir` folder to form the FAT32 partition of the SD card image and files in the `./boot` directory to generate `boot.bin`.

You can pass the values to the script directly by replacing the variable with the actual value, or define them in the header of the tcl script, or pass the value to XSCT when calling this script.

Here is an example of calling XSCT if you hardcode all contents in `xsct_create_pfm.tcl`.

```bash
xsct xsct_create_pfm.tcl
```

To support better generalization, the example [Makefile](./ref_files/step2_pfm/Makefile) and [xsct_create_pfm.tcl](./ref_files/step2_pfm/xsct_create_pfm.tcl) in the `ref_files` directory uses variables to represent the file names and directory location. Refer to them if you would like to get more programmability in your scripts.

</details>

In this step, you created a platform to get FSBL and PMU firmware. You will add all the components to the platform in the last step. The next file that you need to prepare is the DTB file.

## Create the Device Tree File

The device tree describes the hardware components of the system. The `createdts` command can generate the device tree file according to the hardware configurations from the XSA file. If there are any settings unavailable in the XSA, for example, any driver nodes that do not have corresponding hardware, or if you have their own design hardware, you must add customization settings in the `system-user.dtsi` file.

Besides U-Boot, the common image does not have default environment variables. So you must update the `bootargs` manually. A pre-prepared [system-user.dtsi](./ref_files/step2_pfm/system-user.dtsi) file which adds pre-defined `bootargs` is located in the `step2_pfm` directory. Copy the `system-user.dtsi` file to the `zcu104_software_platform` directory and follow these steps to generate the DTB file.

>Note: Device tree knowledge is a common know-how. Please refer to [AMD Device tree WIKI page](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/862421121/Device+Trees) or [Device Tree WIKI page](https://en.wikipedia.org/wiki/Devicetree#Linux) for more information if you are not familiar with it.

1. First, go to `zcu104_software_platform` directory and launch `XSCT` tool.

   ```bash
   cd zcu104_software_platform
   xsct 
   ```

2. Execute the `createdts ` command in the XSCT console as shown below:

   ```bash
   createdts -hw ../zcu104_hardware_platform/zcu104_custom_platform_hw.xsa -zocl  -platform-name mydevice \
    -git-branch xlnx_rel_v2023.1 -board  zcu104-revc  -dtsi system-user.dtsi -compile
   ```

   The `createdts` command needs the following input values:

   -  `-hw`: Hardware XSA file with path.
   -  `-platform-name`: Platform name.
   -  `-git-branch`: device tree branch.
   -  `-board`: board name of the device. You can check the board name at <DTG Repo>/device_tree/data/kernel_dtsi.
   -  `-zocl`: enable the zocl driver support.
   -  `-dtsi`: Add user's device tree file support.
   -  `-compile`: specify the option to compile the device tree.

   The following information would show on the XSCT console. You can ignore the warning. This means that you have succeeded in getting the `system.dtb` file which is located in the `<mydevice/psu_cortexa53_0/device_tree_domain/bsp>` folder.

   ```
   pl.dtsi:9.21-32.4: Warning (unit_address_vs_reg): /amba_pl@0: node has a unit name, but no reg property                                                      
   system-top.dts:26.9-29.4: Warning (unit_address_vs_reg): /memory: node has a reg or ranges property, but no unit name
   zynqmp.dtsi:790.43-794.6: Warning (pci_device_reg): /axi/pcie@fd0e0000/legacy-interrupt-controller: missing PCI reg property
   pl.dtsi:27.26-31.5: Warning (simple_bus_reg): /amba_pl@0/misc_clk_0: missing or empty reg/ranges property
   ```

   >**Note:** `createdts` is a command that is executed on the XSCT console to generate device files. This command needs several inputs to generate the device tree files. Use the `help` command to find out about the input options. XSCT is a console tool for Vitis. You can start it by typing `xsct` on a Linux terminal, or you can select menu **Xilinx > XSCT Console** to start the XSCT tool after you launch the Vitis software platform.

   Execute the following command to exit XSCT console.

   ```bash
   exit
   ```

After this step, all the components for platform creation are ready. Next you will attach all the components to your platform and build it.

## Create a Vitis Platform

1. Create four directories, `pfm`, `boot`, `sd_dir`, and `sw_comp`, to store the components and copy files to these directories.

   ```bash
   cd WorkSpace/zcu104_software_platform
   mkdir pfm 
   mkdir pfm/boot
   mkdir pfm/sd_dir
   mkdir pfm/sw_comp 
   cp zcu104_platform_fsbl/zynqmp_fsbl/fsbl_a53.elf pfm/boot/fsbl.elf        #rename it to fsbl.elf in case of V++ can not find it by name 
   cp zcu104_platform_fsbl/zynqmp_pmufw/pmufw.elf pfm/boot/
   cp xilinx-zynqmp-common-v2023.1/bl31.elf pfm/boot/
   cp xilinx-zynqmp-common-v2023.1/u-boot.elf pfm/boot/
   cp mydevice/psu_cortexa53_0/device_tree_domain/bsp/system.dtb  pfm/boot/
   cp xilinx-zynqmp-common-v2023.1/boot.scr pfm/sd_dir/
   cp mydevice/psu_cortexa53_0/device_tree_domain/bsp/system.dtb  pfm/sd_dir/
   cp xilinx-zynqmp-common-v2023.1/rootfs.ext4 pfm/sw_comp
   cp xilinx-zynqmp-common-v2023.1/Image pfm/sw_comp
   ```

   >**Note:** `fsbl_a53.elf`, `pmufw.elf`, `bl31.elf`, `u-boot.elf`, and `system.dtb` in boot DIR are the source of `BOOT.BIN` image. `Boot.src` and `system.dtb` in `sd_dir` are used for U-Boot initialization and Linux boot up and will be packaged to the FAT32 partition by the V++ package tool. Image and `rootfs.ext4` are Linux kernel and root file system and they will also be packaged to `SD.IMG` by the V++ tool.

2. Install the sysroot 

   - Go to common image extracted directory `<WorkSpace/zcu104_software_platform/xilinx-zynqmp-common-v2023.1/>`.
   - Type ``./sdk.sh -d <Install Target Dir>`` to install the PetaLinux SDK. Use the `-d` option to provide a full pathname to the output directory  **.** (This is an example. < . > means current Dir ) and confirm.

   The environment variable **LD_LIBRARY_PATH** must not be set when running this command.

3. Create the Vitis platform.

   For this example, you will use the Vitis IDE to create the Vitis Platform. If you want to use XSCT to create platform, see [Using XSCT to create Vitis Platform](#create-fsbl-and-pmu-firmware).

   1. Open the Vitis workspace you were using before in step2.
   2. Select **File > New > Platform Project** to create a platform project.
   3. Enter the project name. For this example, type `zcu104_custom`. Click **Next**.
   4. On the Platform page,
      - Click **Browse** button, select the XSA file generated by the Vivado. In this case, it is `zcu104_custom_platform_hw.xsa`.
      - Set the operating system to **linux**.</br>
      - Set the processor to **psu_cortexa53**.</br>
      - Architecture: **64-bit**</br>
      - **uncheck** option **Generate boot components**, because we have got FSBL and PMU already.</br>
   5. Click **Finish**.
  
   >**Note:** To run hardware emulation, select `zcu104_custom_platform_hwemu.xsa` when you use the Vitis IDE to create the platform.

4. Set up the software settings in the Platform Settings view by clicking the **linux on psu_cortexa53** domain, browse to the locations and select the directory or file needed to complete the dialog box for the following:

   - **Bif file**: Click the drop-down icon and select **Generate BIF**.

     >**Note:** The filenames in `<>` are placeholders in the bif file. Vitis will replace the placeholders with the relative path to platform during platform packaging. V++ packager, which runs when building the final application#, would expand it further to the full path during image packaging. Filename placeholders point to the files in boot components directory. The filenames in boot directory need to match with placeholders in BIF file. `<bitstream>` is a reserved keyword. V++ packager will replace it with the final system bit file.

   - **Boot Components Directory**: Browse to **zcu104_software_platform/pfm/boot** and click **OK**.

   - **FAT32 Partition Directory**: Browse to **zcu104_software_platform/pfm/sd_dir** and click **OK**.

   - **QEMU Data**: Browse to **zcu104_software_platform/pfm/boot** and click **OK**.

   ![vitis_linux_config.PNG](./images/vitis_linux_config.PNG)

   >**Note:** Setting QEMU DATA to boot components directory would help the emulator find the boot components during Linux boot when doing emulation.

   >**Note:** If there are additional QEMU settings, update `qemu_args.txt` accordingly.

5. Click **zcu104_custom** project in the Vitis Explorer view, click the **Build** button to build the platform.

   ![missing image](./images/build_vitis_platform.png)

   >**Note:** The generated platform is placed in the export directory. BSP and source files are also provided for rebuilding the FSBL and PMU, if required, and are associated with the platform. The platform is ready to be used for application development.

   ![missing image](./images/vitis_platform_output.png)

   If you create a Vitis application in the same workspace as this platform, you can find this platform available in the platform selection page in the Platform Creation wizard. If you want to reuse this platform in another workspace, add its path to the `PLATFORM_REPO_PATHS` environment variable before launching the Vitis GUI, or use the "Add" button on the platform selection page of the Vitis GUI to add its path.

### Fast Track

Scripts are provided to create the Vitis platform. To use these scripts, run the following steps.

1. Run build.

   >**Note:** Download the common image from the [website download page](https://www.xilinx.com/support/download.html) and give its path to the following command.


   ```bash
   # cd to the step directory, e.g.
   cd step2_pfm
   make all COMMON_IMAGE_ZYNQMP=<path/to/common_image/>
   ```

2. To clean the generated files, run the following command:

   ```bash
   make clean
   ```

## Next Steps

In the next step, you will [build some applications on this platform and test them.](./step3.md).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
