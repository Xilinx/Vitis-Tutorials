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


<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>Versal Custom Platform Creation Tutorial</h1>
    </td>
 </tr>
</table>

## Step 2: Create the Software Components with PetaLinux

In this step, we'll create a PetaLinux project that includes Vitis Platform required components.

### Create the PetaLinux Project

1. Setup PetaLinux environment: `source <petaLinux_tool_install_dir>/settings.sh`

2. Create PetaLinux Project with XSA we created in previous step

   ```bash
   petalinux-create -t project --template versal --force -n petalinux
   cd petalinux
   petalinux-config --get-hw-description=<path to xsa directory> --silentconfig
   ```

   - The created PetaLinux project name is **petalinux**. Please feel free to change the PetaLinux project name with **petalinux-create -n** option.


   **Note**: PetaLinux 2021.2 requires GCC version >= 6.0. If your GCC version doesn't meet this requirement, please enable **Enable buildtools extended** from **petalinux-config → Yocto settings**, which uses the pre-compiled gcc binaries from the PetaLinux tool. For more information, please refer to UG1144.




### Customize Root File System, Kernel, Device Tree and U-boot

1. Prepare a user-rootfsconfig file to define the additional rootfs package. 
   
   Add user packages by appending the CONFIG_xxx lines below to the **<your_petalinux_project_dir>/project-spec/meta-user/conf/user-rootfsconfig** file.

   **Note: This step is not a must but it makes it easier to find and select all required packages in next step.**

   Packages for base XRT support (required):

   ```
   CONFIG_xrt
   ```

   - XRT package contains all the packages for Vitis acceleration runtime.
   - Note: `CONFIG_packagegroup-petalinux-xrt` is not needed from 2021.1
   
   Packages for on-board acceleration application compiling support (optional):

   ```
   CONFIG_xrt-dev
   ```
   
   - package names with `-dev` suffix means header files, dependency libraries and soft links required by compiling environment in Yocto.

   Packages for easy system management (Optional but recommended):

	```
   CONFIG_dnf
   CONFIG_e2fsprogs-resize2fs
   CONFIG_parted
	```
	- **dnf** is the package management tool
	- **parted** and **e2fsprogs-resize2fs** can expand the ext4 partition to use the rest of the SD card.  



2. Add rootfs packages. 
   
   Run ```petalinux-config -c rootfs``` and select **user packages**, select name of rootfs all the libraries listed above, save and exit. 
   
   If step 1 is skipped, please use search function with `/` key to find these packages and enable them.
   
3. In rootfs config, go to **Image Features** and enable **package-management** and **debug_tweaks** option, store the change and exit. (Recommended)

   - Exit from **user packages** to root configuration window by select **Exit** and press **Enter**.
   - Select **Image Features** and enter. 
   - Enable **package-management** and **debug_tweaks** by pressing space key. Sub items of package-management is not needed.
   - Exit
   - Exit
   - Save

   ![missing image](./images/step2/petalinux_package_management.png)

4. Use EXT4 as rootfs format for SD card boot (Recommended)

   PetaLinux uses **initrd** format for rootfs by default. This format extracts rootfs in DDR memory, which means it reduces the usable DDR memory for runtime and can't retain the rootfs changes after reboot. To enable the root file system to retain changes, we'll use EXT4 format for rootfs as the second partition on SD card while keep the first partition FAT32 to store other boot files.

   - Run `petalinux-config`
   - Go to **Image Packaging Configuration**, select **Root File System Type** as **EXT4**.

   ![missing image](./images/step2/petalinux_root_filesystem_type.png)


### Customize Device-tree   

1. Apply VCK190 device tree

   - Run `petalinux-config`
   - Go to **DTG Settings**
   - Enter **versal-vck190-reva-x-ebm-02-reva** for **MACHINE_NAME** option (Config option name is CONFIG_SUBSYSTEM_MACHINE_NAME)
   
   Note: This preset device setting adds ethernet PHY info to device tree for VCK190 board. The [device tree source code][1] will be applied to the PetaLinux project. If your VCK190 board version is different, please check the device tree source code directory whether it has a corresponding device tree version for your board and apply it in the PetaLinux project. You can clone the git repository, or use find file feature in github.

   ![Github Device Tree List](images/step2/github_find_available_device_tree.png)

[1]: https://github.com/Xilinx/u-boot-xlnx/blob/master/arch/arm/dts/versal-vck190-revA-x-ebm-02-revA.dts

2. (Optional) Update the system-user device tree.

   If you have any custom peripherals on board that needs special settings, please update it in system-user.dtsi.

**Note**: PetaLinux 2021.1 will detect XSA type and generate ZOCL node in device tree automatically and update interrupt input number according to your hardware settings in XSA if the XSA is an extensible XSA. This is a new feature from 2021.1.



### Build PetaLinux Image

1. From any directory within the PetaLinux project, build the PetaLinux project.

   ```
   petalinux-build
   ```

   The generated u-boot and Linux images will be located in **images/linux** directory.


2. Build sysroot

   ```
   petalinux-build --sdk
   ```

   Sysroot **sdk.sh** will be generated in **images/linux** directory. We will extract it in next step.

### Fast Track

Scripts are provided to re-create PetaLinux project and generate outputs. To use these scripts, please run the following steps.

1. Run build

   ```
   # cd to the step directory, e.g.
   cd step2_petalinux
   make
   ```

2. To clean the generated files, please run

   ```bash
   make clean
   ```

   



**Note: Now HW platform and SW platform are all generated. Next we would [package the Vitis Platform](./step3.md).**

<p align="center"><sup>Copyright&copy; 2021 Xilinx</sup></p>
