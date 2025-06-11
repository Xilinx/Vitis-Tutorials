<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>2024.1 Vitis™ Platform Creation Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

# Step 2: Create the Vitis Platform

In this step, you will create a Vitis platform running a Linux operation system. The Vitis platform requires several software components which need to be prepared in advance. Due to the boot flow differences between KV260 and base platforms for AMD evaluation boards like ZCU104, platform developer needs to prepare one additional DTBO file besides the normal software components for Vitis platforms because device tree file of programmable logic (PL) side depends on the hardware design and is loaded after Linux boots up. In addition, application developers need to add this DTBO file to the application deployment package as well. The other software components generation flow is similar to the flow described in the [ZCU104 tutorial](../02-Edge-AI-ZCU104/step2.md). However, in this case, the valuable software component is sysroot,which is used for host application cross-compilation. So you will go on utilizing the common image which provides the sysroot and expedite the process of platform creation.

### Prepare the Common Image

1. Download common image from [Xilinx website download page.](https://www.xilinx.com/support/download.html). Go to the **WorkSpace** folder you created in step 1, and place the image package in **WorkSpace** folder.

   ```bash
   cd WorkSpace
   tree -L 1     # to see the directory hierarchy
   ├── kv260_hardware_platform
   └── xilinx-zynqmp-common-v2025.1.tar.gz
   ```

2. Extract the common image.


   ```bash
   cd WorkSpace
   tar xvf ../xilinx-zynqmp-common-v2025.1.tar.gz -C .
   ```

   You can see **xilinx-zynqmp-common-v2025.1** folder which contains the components located in **WrokSpace** folder.

   ```bash
   tree -L 2
   ├── xilinx-zynqmp-common-v2025.1
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

From the above picture, you can see SDK tool and other components are ready.

### Generate Software Components from the KV260 Starter Kit BSP(Optional)

If you need to do system customization, take the following steps as reference. For more advanced tweaks, like kernel or uboot customization, refer to the [PetaLinux customization page](../../Feature_Tutorials/02_petalinux_customization/README.md) for reference.

<details>

<summary><strong>Click for Detailed Steps</strong></summary>  

1. Check the [AMD Kria™ K26 SOM wiki](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/1641152513/Kria+K26+SOM), and download the BSP package from the following link: <https://www.xilinx.com/member/forms/download/xef.html?filename=xilinx-k26-starterkit-v2024.1-final.bsp>

   Save it to **WorkSpace** directory.

2. Set up the PetaLinux environment.

    ```bash
    cd WorkSpace
    mkdir kv260_petalinux
    cd kv260_petalinux
    source <petaLinux_tool_install_dir>/settings.sh
    ```

3. Update the PetaLinux eSDK to enable the recipes needed by the Starter Kit SOM BSP.

    ```bash
    petalinux-upgrade -u 'http://petalinux.xilinx.com/sswreleases/rel-v2004/sdkupdate/' -p 'aarch64'
    ```

4. Create PetaLinux with the Starter Kit SOM BSP, and the XSA export from step 1.

    ```bash
    petalinux-create --type project -s xilinx-kv260-starterkit-v2024.1-final.bsp
    cd xilinx-kv260-starterkit-2024.1
    petalinux-config --get-hw-description=<vivado_design_dir> --silent  
    ```

5. Add XRT to rootfs.

    KV260 PetaLinux BSP does not enable Xilinx Runtime (XRT) because it installs XRT with an overlay. To create sysroot for application developer cross compiling, enable XRT in the rootfs. All the XRT required libraries are packaged into one group.

    - Run `petalinux-config -c rootfs` to launch rootfs configuration window.
    - Go to **Petalinux Package Groups -> packagegroup-petalinux-vitis-acceleration-essential**.
    - Enable `packagegroup-petalinux-vitis-acceleration-essential` and `packagegroup-petalinux-vitis-acceleration-dbg`.
    - Press **Exit** to exit configuration. Press Save to save the configuration.

6. Build PetaLinux and generate SDK.

    ```bash
    petalinux-build
    petalinux-build --sdk
    ```

    The PetaLinux image files and sysroot sdk.sh will be generated in `<PetaLinux Project>/images/linux` directory. You will use them in the next step.

</details>

### Create the Vitis Platform

1. Install sysroot.

   - Go to `<WorkSpace/xilinx-zynqmp-common-v2025.1>` directory.
   - Type `./sdk.sh -d <Install Target Dir>` to install PetaLinux SDK. Use the `-d` option to provide a full pathname to the output directory. For example: `./sdk.sh -d .`. **.** means the current directory. 
   >**NOTE:** The environment variable **LD_LIBRARY_PATH** must not be set when running this command.


   >Note: Sysroot is not mandatory components for the platform itself. It is the cross compile tool prepared for applications compilation.

2. Create a Vitis platform.

   For this example, you will use the Vitis Unifeid IDE to create the Vitis Platform. Got to `WorkSpace` directory and follow steps below to create the platform.

   1. Run Vitis by typing `vitis -w .` in the console. `-w` is to specify the workspace. `.` means the current workspace directory.
   2. In the Vitis Unified IDE, from menu select **File > New Component > Platform** to create a platform component.
   3. On the **Create Platform Component** setup dialog
      - Enter the component name and location. For this example, type `kv260_custom` and use default location. Click **Next**.
      - Click **Browse** button, select the XSA file generated by the Vivado. In this case, it is `kv260_hardware_platform.xsa`. 
      - Expand the **Advanced Options** and set the items as following:

         ![Created Vitis Platform](images/platform_generation_dts.jpg)
         - SDT Source Repo: This is used to replace the built-in SDT tool. For this tutorial, leave it empty.
         - Board DTSI: Specify the board machine name, which is used to retrieve the board-level DTSI file. For this tutorial, leave it empty. To check the board machine name, refer to [UG1144 Machine Name Checking](https://docs.amd.com/r/en-US/ug1144-petalinux-tools-reference-guide/Importing-a-Hardware-Configuration)
         - User DTSI: Allows you to specify a custom DTSI file. For this tutorial, leave it empty.
         - DT ZOCL: Enables Zocl node generation for the XRT driver. Ensure this option is enabled, then click **Next**.
      - Set the operating system to **Linux**.</br>
      - Set the processor to **psu_cortexa53**.</br>
      - Check the option **Generate boot artifacts**. Then click **Next**.</br> 
         >Note: Enabling this option will trigger the tool to automatically generate a PMU firmware domain and an FSBL (First Stage Boot Loader) domain into the platform
      - Enable and expand the **Generate Device Tree Blob (DTB)** option, and check the **DT Overlay** option. This option is used to generrate DTB and device tree blob overlay for SOM application. Then click **Next**.</br>
      - Review the summary and click **Finish**.
      >Note: After a few moments, the platform component will be prepared and available in the component view. Simultaneously, the platform configuration file, `vitis-comp.json`, will be automatically displayed in the main view. Users can access the `vitis-comp.json` file by expanding the Settings section under the platform component.

3. Set up the software settings in the Platform configuration view by clicking the **Linux On psu_cortexa53** domain, browse to the locations and select the directory or file needed to complete the dialog box for the following:

   - **Display Name**:  update it as `xrt`
   - **Bif file**: Click the button to generate bif file or click **Browse** to select existing bif file. 

     >**Note:** The filenames in `<>` are placeholders in the bif file. Vitis will replace the placeholders with the relative path to platform during platform packaging. V++ packager, which runs when building the final application#, would expand it further to the full path during image packaging. Filename placeholders point to the files in boot components directory. The filenames in boot directory need to match with placeholders in BIF file. `<bitstream>` is a reserved keyword. V++ packager will replace it with the final system bit file.

   - **Pre-Built Image Directory**: Browse to **xilinx-zynqmp-common-v2025.1** and click **OK**.

   - **DTB File**: It will be generated automatically and populated in this area. Then click **OK**.
      >Note: If the directory you specified for Pre-build image directory already contains DTB file, this DTB field will be automatically updated. 

   - **FAT32 Partition Directory**: This directory is used to add additional file to the fat32 partition. User can set it according to your requirement.

   - **QEMU Data**: This Directory is used to add additional file for emulation. User can set it accordingly.

   ![vitis_linux_config.PNG](./images/vitis_linux_config.png)

   >**Note:**: **Qemu Args File** and **Pmu Args File**  are populated by the tool automatically. If there are additional QEMU settings, please update it accordingly.

4. Select **kv260_custom** platform component in the flow navigator, then click the **Build** button to build the platform.

   ![missing image](./images/build_vitis_platform.png)

   >**Note:** The generated platform is placed in the export directory. BSP and source files are also provided for rebuilding the FSBL, if required, and are associated with the platform. The platform is ready to be used for application development.

   ![missing image](./images/vitis_platform_output.png)

   >Note: Once the compilation is complete, users can find the XPFM file by expanding the Output directory, which offers a structured view of the output. The actual file path of platform file is located in the `WorkSapce/kv260_custom/export/kv260_custom/` directory. Additionally, users can access the full path of the platform file by hovering the mouse pointer over the XPFM file.

   >Note: The Vitis Unified IDE will find the boot-related files mentioned in the software components in begin of this step from Pre-buit image directory and place them in the boot folder of the platform.

   If you create a Vitis application component in the same workspace as this platform component, you can find this platform available in the platform selection page in the application Creation wizard. If you want to reuse this platform in another workspace, add its path to the `PLATFORM_REPO_PATHS` environment variable before launching the Vitis GUI, or use the "Add" button on the platform selection page of the Vitis GUI to add its path.



### Next Step

Next, try to [build applications on this platform and test them.](./step3.md)

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
