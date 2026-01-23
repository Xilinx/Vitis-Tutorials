<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ System Design Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
    </td>
  </tr>
</table>


# Package the design and generate a SD card for Linux

The packaging step prepares and assembles boot artifacts, linux image and file system, and host applications and deploy them to a SD card image or folder.
For convenience with Windows OS, the formatting is set to FAT32 so the SD card can be accessed natively by Windows.

To package the design, call Makefile from top folder:
```
make package
```

The command will change directory to `build_hw` to keep all logs and outputs in a separate folder.

**Note** The Linux image, file system and boot artifacts is picked primarily from the generated outputs from Petalinux or Yocto. This is to allow for user to setup reusable linux artifacts to speed up build time.


Example of the command:
```
cd build_hw && \
v++ --package --debug --save-temps \
        --target hw \
        --platform .././build_pfm/pfm/export/pfm/pfm.xpfm \
        --package.defer_aie_run \
        --package.out_dir ../../package_linux_hw \
        --package.boot_mode=sd \
        --package.no_image \
        --package.image_format=fat32 \
        --package.sd_file=../../linux/vck190-versal/images/linux/Image \
        --package.rootfs=../../linux/vck190-versal/images/linux/rootfs.cpio.gz.u-boot  \
        --package.dtb=.././build_pfm/pfm/export/pfm/sw/boot/system.dtb  \
        --package.bl31_elf=../../linux/vck190-versal/images/linux/bl31.elf  \
        --package.uboot=../../linux/vck190-versal/images/linux/u-boot.elf  \
        ../../vss/./ip/aie/datamove_app_libadf.a \
        --package.sd_file=../../ps_apps/linux/exe/aie_dly_test.exe \
        --package.sd_file=../../linux/vck190-versal/images/linux/boot.scr \
        -o my.xclbin \
        2>&1 | tee vpp_pack_sd.log
echo vck190_thin > ../package_linux_hw/sd_card/platform_desc.txt
```

The package command outputs is directed to `package_linux_hw` in the top folder with the FAT32 formatted SD cards files in `package_linux_hw/sd_card`.

**Note** The [Makefile](./Makefile) identifies host `*.exe` files from `../ps_apps/linux/exe` folder and automatically adds them to the `v++ --package` arguments.


## Next step
  - [Run on hardware](../README.md#run-on-hardware)



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2025 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
