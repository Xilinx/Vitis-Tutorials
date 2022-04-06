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

## Step 2: Create the Software Components


<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>2021.1 Vitis™ Platform Creation Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

Due to the boot flow differences between KV260 and base platforms for Xilinx evaluation boards like ZCU104, platform developer needs to prepare one additional DTBO file besides the normal software components for Vitis platforms because application developers needs to add this DTBO file to the application deployment package. The other software components generation flow is similar to the flow described in the [ZCU104 tutorial](../../Introduction/02-Edge-AI-ZCU104/step2.md). Since KV260 provides its BSP, we can use the PetaLinux BSP to generate Linux kernel, root file system and boot components, or do more customization based on the BSP.

# Step 2: Create the Software Components with PetaLinux

KV260 provides an off-the-shelf boot image and has its enhanced boot sequence. Since the image can boot successfully out of the box, Vitis platform developers can skip some software component preparation steps for Linux booting.


### Generating Normal Software Components from the KV260 Starter Kit BSP

1. Check [Kria K26 SOM wiki](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/1641152513/Kria+K26+SOM). Download the **Starter Kit SOM BSP** that matches your tool version under **PetaLinux Board Support Package** section.

    For example, https://www.xilinx.com/member/forms/download/xef.html?filename=xilinx-k26-starterkit-v2021.1-final.bsp

    Save it to your working directory.

2. Setup PetaLinux environment

    ```bash
    source <petaLinux_tool_install_dir>/settings.sh
    ```

3. Update PetaLinux eSDK to enable the recipes needed by the Starter Kit SOM BSP.

    ```bash
    petalinux-upgrade -u 'http://petalinux.xilinx.com/sswreleases/rel-v2021/sdkupdate/2021.1_update1/' -p 'aarch64'
    ```

4. Create PetaLinux with the Starter Kit SOM BSP and the XSA export from step 1.

    ```bash
    petalinux-create --type project -s xilinx-k26-starterkit-v2021.1-final.bsp
    cd xilinx-k26-starterkit-v2021.1
    petalinux-config --get-hw-description=<vivado_design_dir> --silent
    ```

5. Add XRT to rootfs

    KV260 PetaLinux BSP doesn't enable XRT because it installs XRT with overlay. To create sysroot for application developer cross compiling, we enable XRT in the rootfs.

    - Run `petalinux-config -c rootfs` to launch rootfs configuration window.
    - Go to **Filesystem packages -> libs -> xrt**
    - Enable `xrt`
    - Press Exit to exit configuration. Press Save to save the configuration.

6. Build PetaLinux and generate SDK

    ```bash
    petalinux-build
    petalinux-build --sdk
    ```

    The PetaLinux image files and sysroot sdk.sh will be generated in <PetaLinux Project>/images/linux directory. We will use them in the next step.


## Generate Device Tree Overlay

Since KV260 loads PL after Linux boots up, the PL IP information in the platform needs to be loaded dynamicly as device tree overlay. So one additional requirement for KV260 acceleration platform software is to generate the device tree overlay for the platform PL IPs. This device tree overlay serves two purposes:

1. It needs to have ZOCL node so that XRT driver can be loaded properly
2. It can include any configurations of the PL IP in the platform logic designed in step 1.

The device tree information for PL in your application is loaded after Linux boot together with the PL bitstream. A device tree overlay (DTBO) can be loaded and unloaded in Linux. For more information about DTBO, please refer to https://lkml.org/lkml/2012/11/5/615. 

Xilinx provides Device Tree Generator (DTG) to generate device tree from XSA file exported from Vivado. DTG is configurable for which information to be generated. According to our requirement, we will use these two options:

- `CONFIG.dt_zocl`: Generate ZOCL device tree node for XRT
- `CONFIG.dt_overlay`: Add overlay properties in dtsi for DTBO generation

Run the following steps to generate DTBO from XSA

1. Install DTG

    ```bash
    git clone https://github.com/Xilinx/device-tree-xlnx
    cd device-tree-xlnx
    git checkout xlnx_rel_v2021.1
    ```

2. Generate DTS from XSA with XSCT.

   - Launch XSCT

    ```bash
    xsct
    ```

   - Use XSCT HSI commands to read XSA and generate DTS.

    ```bash
    hsi open_hw_design <design_name.xsa>
    hsi set_repo_path <path to device-tree-xlnx repository>
    hsi create_sw_design device-tree -os device_tree -proc psu_cortexa53_0
    hsi set_property CONFIG.dt_overlay true [hsi::get_os]
    hsi set_property CONFIG.dt_zocl true [hsi::get_os]
    hsi generate_target -dir <desired_dts_filename>
    hsi close_hw_design [hsi current_hw_design]
    ```

    You can save the hsi commands in a gen_dt.tcl file and use `xsct gen_dt.tcl` to execute it. If you type these commands in XSCT console directly, you can close XSCT console after the generation completes.

3. Compile the dtsi to dtbo.

    Run the following command in the shell with PetaLinux environment.

    ```bash
    dtc -@ -O dtb -o pl.dtbo pl.dtsi
    ```

    > Note: `dtc` is device tree compiler. For more info about dtc, please check its [man page](http://manpages.ubuntu.com/manpages/trusty/man1/dtc.1.html) and [source code](https://git.kernel.org/pub/scm/utils/dtc/dtc.git/tree/README?h=main).



### Next Step

> Note:  Now HW platform and SW platform are all generated. Next we would [package the Vitis Platform](step3.md).

<p class="sphinxhide" align="center"><sup>Copyright&copy; 2021 Xilinx</sup></p>
