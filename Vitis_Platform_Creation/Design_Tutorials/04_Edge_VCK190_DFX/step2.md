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
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Versal Custom Platform Creation Tutorial</h1>
    </td>
 </tr>
</table>

## Step 2: Create the Software Components and the Vitis DFX Platform

The software components of a Vitis extensible platform are identical no matter whether the platform supports DFX or not. Please refer to VCK190 Custom Platform Tutorial for details about preparing software components from common image or build them from scratch.

The unique requirements from DFX platforms are from the device tree.

### Generating Device Tree

We use the static XSA to generate the base device tree with device tree generator (DTG). The generated device tree (.dts and .dtsi) only includes IP information in the static region. If any IP in the dynamic region needs driver support, we need to add them to the device tree manually. AI Engine is an example that needs driver support. We will add its info to system-user.dtsi. ZOCL driver needs to know the dfx_decoupler IP address so that when doing DFX reconfiguration, XRT can turn on the decoupler for isolation before configuring the dynamic region and turn the isolation off after configuration. DTG creates the default ZOCL nodes in the pl.dtsi file, we will append ZOCL configurations for DFX in the system-user.dtsi file.

> Note: Since currently only one DFX region is supported, ZOCL driver and device tree only supports one dfx_decoupler IP address.

> Note: If you have multiple RM and they have different IP, you can enable these IP with device tree overlay. Device tree overlay is out of the scope of this tutorial.

Generate the base device tree from the static XSA

```
createdts -hw <static XSA> \
    -zocl\
    -out . \
    -platform-name vck190_custom_dt \
    -git-branch xlnx_rel_v2022.1
```

The generated device tree files are located in build/vck190_custom_dt/psv_cortexa72_0/device_tree_domain/bsp path.

Create **system-user.dtsi** file in this directory and add the following contents to system-user.dtsi. 

```
&amba_pl {
	ai_engine@20000000000 {
		#address-cells = <0x02>;
		#size-cells = <0x02>;
		clock-names = "aclk0";
		clocks = <0x13>;
		compatible = "xlnx,ai-engine-2.0\0xlnx,ai-engine-v2.0";
		power-domains = <0x07 0x18224072>;
		ranges;
		reg = <0x200 0x00 0x01 0x00>;
		xlnx,aie-gen = [01];
		xlnx,core-rows = [01 08];
		xlnx,mem-rows = [00 00];
		xlnx,shim-rows = [00 01];

		aie_aperture@20000000000 {
			#address-cells = <0x02>;
			#size-cells = <0x02>;
			interrupt-names = "interrupt1\0interrupt2\0interrupt3";
			interrupt-parent = <0x05>;
			interrupts = <0x00 0x94 0x04 0x00 0x95 0x04 0x00 0x96 0x04>;
			power-domains = <0x07 0x18224072>;
			reg = <0x200 0x00 0x01 0x00>;
			xlnx,columns = <0x00 0x32>;
			xlnx,node-id = <0x18800000>;
		};
	};
	aie_core_ref_clk_0 {
		#clock-cells = <0x00>;
		clock-frequency = <0x3B9ACA00>;
		compatible = "fixed-clock";
		phandle = <0x13>;
	};
	zyxclmm_drm {
		compatible = "xlnx,zocl-versal";
		status = "okay";
		xlnx,pr-decoupler;
		xlnx,pr-isolation-addr = <0x0 0xA4000000>;
	};
};
```

Please check your DFX_Decoupler address in the block design and update it to `xlnx,pr-isolation-addr` in the device tree.

If you updated the AI Engine clock frequency, please update the `clock-frequency` of `aie_core_ref_clk_0`.

Include system-user.dtsi into system-top.dts

```
echo "#include \"system-user.dtsi\"" >> vck190_custom_dt/psv_cortexa72_0/device_tree_domain/bsp/system-top.dts
```

Generate the final DTB.

```bash
cd step2_sw/build/vck190_custom_dt/psv_cortexa72_0/device_tree_domain/bsp/
# preprocess the dts file because DTC command can not recognize the #include grammar
gcc -I my_dts -E -nostdinc -undef -D__DTS__ -x assembler-with-cpp -o system.dts system-top.dts  
# compile the dts into dtb
dtc -I dts -O dtb -o system.dtb system.dts 
```

You can find the system.dtb file in step2_sw/build/vck190_custom_dt/psv_cortexa72_0/device_tree_domain/bsp/ directory.



### Creating the Vitis Platform

The Vitis platform creation workflow for DFX platforms is almost identical to the flat platform with the following exceptions

1. The platform creation can only be down with XSCT. Vitis IDE doesn't support to create DFX platforms.
2. When creating the DFX platform, both static XSA and RP XSA are required. Static XSA will be used to create boot.bin. RP XSA will be used to link acceleration kernels.


#### Prepare for Platform Packaging

User should prepare the following components before creating the platform.

| Component                                     | Conventional Path or Filename                         | Description                                                   |
| --------------------------------------------- | ----------------------------------------------------- | ------------------------------------------------------------- |
| Boot components                   | boot/bl31.elf</br>boot/u-boot.elf</br>boot/system.dtb | All components referred in linux.bif should be in this folder |
| Boot components in FAT32 partition of SD Card | sd_dir/boot.scr                                        | U-boot configuration file to store in FAT32 partition of SD card                                    |
| Linux Software Components         | sw_comp/Image</br>sw_comp/rootfs.ext4</br>sw_comp/sysroots | Linux components for application creation and Linux booting. They can be packaged into platform or stay standalone and be linked during application creation process. |

In this tutorial, we will use the Linux software components provided by the Common Images.

We would store all the necessary files for Vitis platform creation flow. Here we call it **step2_sw/build/pfm** directory. 


1. Download and extract Common Images for Versal

   - Visit the [Vitis Embedded Platforms](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html) download page.
   - On Version 2022.1 page, download the Versal common image to step2_sw/build/common.
   - Extract the downloaded package with command `cd step2_sw/build/common; tar -xzf xilinx-versal-common-v2022.1_04191534.tar.gz` 

2. Prepare the boot components. 

   From the table above, we know that the boot components include the following items:

   - bl31.elf
   - u-boot.elf
   - system.dtb

   Follow these steps to copy them to boot directory.

   - Create directory for boot components `mkdir step2_sw/build/boot`
   - Copy boot components from common image directory
     ```bash
     cp step2_sw/build/common/bl31.elf step2_sw/build/boot
     cp step2_sw/build/common/u-boot.elf step2_sw/build/boot
     ```
   - Copy device tree
     ```bash
     cp step2_sw/build/vck190_custom_dt/psv_cortexa72_0/device_tree_domain/bsp/system.dtb step2_sw/build/boot
     ```


3. Prepare the **sd_dir** directory. Contents in this directory will be packaged to FAT32 partition of SD card image by v++ package tool.

   - Create sd_dir directory: `mkdir step2_sw/build/sd_dir`
   - Copy **boot.scr** from common image diretory **step2_sw/build/common** to the **step2_sw/build/sd_dir** directory. It's a script for u-boot initialization. U-boot will read it from fat32 partition during boot process.

4. Prepare Linux software components for application creation. Platform packaging can add them or exclude them. Due to the software component size, they are usually kept outside of platforms so that these components can be reused among multiple platforms with the same architecture. On the other side, keeping Linux software components in the platform can prevent mix and match different combinations when delivering the platform from one team to another. Here we keep the Linux software components standalone.

   ```bash
   # Create sw_comp directory
   mkdir step2_sw/build/sw_comp
   cd sw_comp
   # Copy Linux kernel image
   cp <COMMON_IMAGE_PATH>/Image .
   # Copy EXT4 Root Filesystem
   cp <COMMON_IMAGE_PATH>/rootfs.ext4 .
   # Install sysroot to this directory
   <COMMON_IMAGE_PATH>/sdk.sh -d ./ -y
   ```

Finally, your prepared directory structure looks like this.

```
tree -L 3 --charset ascii
.
|-- boot
|   |-- bl31.elf
|   |-- system.dtb
|   `-- u-boot.elf
|-- sd_dir
|   `-- boot.scr
|-- sw_comp
|   |-- environment-setup-cortexa72-cortexa53-xilinx-linux
|   |-- Image
|   |-- rootfs.ext4
|   |-- site-config-cortexa72-cortexa53-xilinx-linux
|   |-- sysroots
|   |   |-- cortexa72-cortexa53-xilinx-linux
|   |   `-- x86_64-petalinux-linux
|   `-- version-cortexa72-cortexa53-xilinx-linux
```

> Note: If you run the fast track script, the boot directory only has linux.bif file because the platform creation script xsct_create_pfm.tcl uses `<petalinux_project>/images/linux` directory as boot directory for components of the BIF file. To make the GUI flow easier, we copy these components to boot directory for preparation.


#### Generating Static Boot Image

A DFX platform boot the static region during power on. The static boot image can include static region PDI for hardware configuration, ARM Trusted Firmware bl31.elf, u-boot.elf and device tree for u-boot.

When creating a DFX platform, static boot.bin is required. We generate this boot image before creating the platform.

Static PDI is included in the static region XSA. We can use XSCT command `openhw $(XSA_NAME)_static.xsa` to extract the XSA and find the PDI.

Run bootgen command to create the boot.bin.
```
bootgen -arch versal -image bootgen.bif -o boot.bin;
```

The bootgen.bif should contain the following information.
```
the_ROM_image:
{
image {
	{ type=bootimage, file=<static>.pdi }
}
image {
	id = 0x1c000000, name=apu_subsystem 
	{ type=raw, load=0x00001000, file=build/boot/system.dtb }
	{ core=a72-0, exception_level=el-3, trustzone, file=build/boot/bl31.elf }
	{ core=a72-0, exception_level=el-2, file=build/boot/u-boot.elf }
}
}
```


### Platform Packaging

We will use XSCT command line tool to create the Vitis DFX platform. 

> Note: Vitis IDE support for creating Vitis DFX platforms will be added in the future.

Create a tcl file with XSCT commands. 

```Tcl
# Create a platform project
platform create -name vck190_dfx_custom \
    -desc "A custom VCK190 DFX platform" \
    -hw <Static>.xsa \
    -rp {id 0 hw <RP>.xsa hw_emu <HW_EMU>.xsa} \
    -out <Output_Directory> \
    -no-boot-bsp 

# AIE domain
domain create -name aiengine -os aie_runtime -proc ai_engine
domain config -qemu-data ./boot

# Add Linux domain
domain create -name xrt -proc psv_cortexa72 -os linux -sd-dir {./sd_dir} 
domain config -hw-boot-bin <PATH to Boot.bin>
domain config -boot {./boot}
domain config -generate-bif
domain config -qemu-data ./boot

platform write
platform generate
```

> Note: Please replace the file name and directory name in the script with your project file location.

> Note: If you don't need to support hardware emulation, you can omit the option `-hw_emu` and its value for the command `platform create`.

The `platform create` command needs the following input values:

- `-name`: Platform name
- `-hw`: Static Hardware XSA file location
- `-rp`: The reconfigureable partition info with ID, XSA and hardware emulation XSA info. ID is reserved for multi-partition DFX. For now only one partition is supported. Please use `id 0`.
- `-out`: platform output path. In this example, we set output directory to **step2_sw/build/pfm**.
- `-sd-dir`: the directory that contains the files to be included in the FAT32 partition of the SD card image.

> Note: Hardware and hardware emulation XSA are both required by v++ link in 2022.1. In future releases you will be able to provide only one XSA if you only need to run one target build.

The `domain` command will setup one AI Engine domain and one Linux domain. The Linux domain has SD boot mode. The DFX platform Linux domain requires user to provide the boot.bin to boot the static region. It will use files in `./sd_dir` to form the FAT32 partition of the SD card image. We have stored required files in these directories in [Prepare for Platform Packaging](#prepare-for-platform-packaging) step.

You can pass the values to the script directly by replacing the variable with the actual value, or define them in the header of the tcl script, or pass the value to XSCT when calling this script. 

Here's an example of calling XSCT if you hard code all contents in xsct_create_pfm.tcl.

```bash
xsct xsct_create_pfm.tcl
```

To support better generalization, the example [Makefile](./ref_files/step2_sw/Makefile) and [xsct_create_pfm.tcl](./ref_files/step2_sw/xsct_create_pfm.tcl) in ref_files directory use variables to represent the file names and directory location. Please refer to them if you would like to get more programmability in your scripts.




### Next Step

Next let's move to [step 3 to try to build some applications on this platform and test them.](./step3.md)



### Fast Track

Scripts are provided to generate the device tree and generating the platform. To use these scripts, please run the following steps.

1. Run build

   ```
   # cd to the step directory, e.g.
   cd step2_sw
   make
   ```

   The created platform is located in the `step2_sw/build/pfm/vck190_custom_dfx/export` directory.

2. To clean the generated files, please run

   ```bash
   make clean
   ```




<p align="center"><sup>Copyright&copy; 2022 Xilinx</sup></p>
