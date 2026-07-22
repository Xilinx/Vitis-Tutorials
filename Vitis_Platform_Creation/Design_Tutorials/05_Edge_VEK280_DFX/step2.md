<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ Platform Creation Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# Step 2: Create the Software Components and the Vitis DFX Platform

The software components of an AMD Vitis™ extensible platform are identical regardless of whether the platform supports DFX. Refer to the [VCK190 Custom Platform Tutorial](../03_Edge_VCK190/README.md) for details about preparing software components from the common image or building them from scratch.

The unique requirement of a DFX platform comes from the device tree.

## Generating the Device Tree

The static XSA is used to generate the base device tree with `sdtgen` and `lopper`. In the Vitis Unified ID flow, device tree generation (`*.dts`, `*.dtsi`) is automatic and includes IP information from the static region. If any IP in the dynamic region needs driver support, you must add it to the device tree manually. The AI Engine-ML (AIE-ML) array is the canonical example that needs driver support; its node is added to `system-user.dtsi`.

> **Note:** Only one DFX region is supported at the moment; therefore the ZOCL driver and device tree only support one DFX Decoupler IP address.

> **Note:** If you have multiple RMs and they expose different IPs, you can enable those IPs with a device tree overlay. Device tree overlays are out of scope for this tutorial.

### Add AIE-ML Information to `system-user.dtsi`

The VEK280-specific AIE-ML node looks like this (already present in [`ref_files/step2_sw/system-user.dtsi`](./ref_files/step2_sw/system-user.dtsi)):

```dts
&amba_pl {
    ai_engine@20000000000 {
        #address-cells = <0x02>;
        #size-cells    = <0x02>;
        clock-names    = "aclk0";
        clocks         = <0x13>;
        compatible     = "xlnx,ai-engine-2.0\0xlnx,ai-engine-v2.0";
        power-domains  = <&versal_firmware 0x18800000>;
        ranges;
        reg            = <0x200 0x00 0x01 0x00>;
        xlnx,aie-gen   = [02];           /* AIE-ML generation */
        xlnx,core-rows = [03 08];
        xlnx,mem-rows  = [01 02];
        xlnx,shim-rows = [00 01];

        aie_aperture@20000000000 {
            #address-cells     = <0x02>;
            #size-cells        = <0x02>;
            interrupt-names    = "interrupt1\0interrupt2\0interrupt3";
            interrupt-parent   = <0x05>;
            interrupts         = <0x00 0x94 0x04 0x00 0x95 0x04 0x00 0x96 0x04>;
            power-domains      = <&versal_firmware 0x18800000>;
            reg                = <0x200 0x00 0x01 0x00>;
            xlnx,columns       = <0x00 0x26>;
            xlnx,node-id       = <0x18800000>;
        };
    };

    aie_core_ref_clk_0 {
        #clock-cells     = <0x00>;
        clock-frequency  = <0x4a817c80>;   /* 1.25 GHz */
        compatible       = "fixed-clock";
        phandle          = <0x13>;
    };
};
```

> **Note:** `xlnx,aie-gen = [02]` selects the AIE-ML generation (vs. classic AIE on VCK190, which uses `xlnx,aie-gen = [01]`). The `core-rows`, `mem-rows`, `shim-rows`, and `columns` properties reflect the VEK280 AIE-ML array geometry.

If you change the AIE-ML clock frequency in your hardware design, update `clock-frequency` of `aie_core_ref_clk_0` to match.

## Creating the Vitis Platform

The Vitis platform creation workflow for DFX platforms is almost identical to the flat-platform flow, with the following exceptions:

- DFX platforms can only be created with the **Python CLI flow**. The Vitis IDE does not support DFX platform creation.
- Both the **static XSA** and the **RP XSA** are required. The static XSA is used to generate `BOOT.BIN`; the RP XSA is used by `v++` when linking acceleration kernels.
- Device tree generation is automatic via `sdtgen` and `lopper` (part of the Vitis 2026.1 tool chain).
- The platform is created in two phases: [`generate_platform.py`](./ref_files/step2_sw/generate_platform.py) lays down the platform component metadata, and [`build_platform.py`](./ref_files/step2_sw/build_platform.py) builds it after `bootgen` produces the static boot image.

### Prepare for Platform Packaging

You should prepare the following components before creating the platform.

| Component                                     | Conventional Path or Filename                              | Description                                                                                                                                                  |
| --------------------------------------------- | ---------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| Boot components                               | `boot/bl31.elf`, `boot/u-boot.elf`, `boot/system.dtb`      | All components referenced in `linux.bif` must be in this folder.                                                                                             |
| Boot components in FAT32 partition of SD card | `sd_dir/boot.scr`                                          | U-Boot configuration file to store in the FAT32 partition of the SD card.                                                                                    |
| Linux software components                     | `sw_comp/Image`, `sw_comp/rootfs.ext4`, `sw_comp/sysroots` | Linux components for application creation and Linux boot. They can be packaged into the platform or stay standalone and be linked at application build time. |

This tutorial uses the Linux software components from the AMD-released common image. The `step2_sw/Makefile` stages everything inside `step2_sw/build/`.

1. Download and extract the common image for AMD Versal™ devices.

   - Visit the [Vitis Embedded Platforms](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html) download page.
   - Download the Versal common image (e.g. `xilinx-versal-common-v2026.1.tar.gz`).
   - Extract it: `tar -xzf xilinx-versal-common-v2026.1.tar.gz`.
   - Export the resulting directory as `COMMON_IMAGE_VERSAL` (or `EDGE_COMMON_SW`) before invoking `make`.

2. Prepare the boot components.

   - `bl31.elf`, `u-boot.elf` come from the common image.
   - `system.dtb` is generated by `sdtgen` from the static XSA. The Makefile copies it from `<platform>/hw/sdt/`.
   - The Makefile creates `step2_sw/build/boot/` and stages all three components automatically.

3. Prepare the `sd_dir` directory. Contents are packaged into the FAT32 partition of the SD card image by the `v++ --package` step.

   - `boot.scr` is copied from the common image. U-Boot reads it from the FAT32 partition during boot.

4. Prepare Linux software components for application creation. The Makefile copies `Image` and `rootfs.ext4` into `step2_sw/build/sw_comp/`. If you also need the sysroot for cross-compilation, run `sdk.sh -d <path>` from the common image directory.

The final directory layout looks like:

```
step2_sw/build/
├── boot/
│   ├── bl31.elf
│   ├── system.dtb
│   └── u-boot.elf
├── boot_bin/
│   └── BOOT.BIN
├── sd_dir/
│   └── boot.scr
└── sw_comp/
    ├── Image
    └── rootfs.ext4
```

### Generating the Static Boot Image

A DFX platform can boot the static region at power-on. The static boot image bundles the static-region PDI, Arm® Trusted Firmware (`bl31.elf`), `u-boot.elf`, and the device tree for U-Boot.

When creating a DFX platform, the static `BOOT.BIN` is mandatory. The Makefile generates it between `generate_platform.py` and `build_platform.py` by running `bootgen` against [`bootgen.bif`](./ref_files/step2_sw/bootgen.bif):

```bash
bootgen -arch versal -image bootgen.bif -o BOOT.BIN -w
```

The BIF file:

```text
the_ROM_image:
{
    image {
        { type=bootimage, file=./vek280_custom_dfx/hw/sdt/vek280_custom_dfx_static.pdi }
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

The Python CLI flow drives the entire create-and-build sequence. The relevant snippet from `step2_sw/Makefile`:

```bash
# Create a platform component
vitis -s generate_platform.py \
    --platform_name   $(PLATFORM_NAME) \
    --static_xsa_path $(STATIC_XSA) \
    --emu_xsa_path    ${HW_EMU_XSA} \
    --platform_out    ${PLATFORM_OUT_PATH} \
    --boot_dir_path   ${BOOT_DIR} \
    --sd_dir_path     ${SD_DIR} \
    --rp_xsa_path     ${RP_XSA} \
    --user_dtsi       $(USER_DTSI)

# Build the platform with the static BOOT.BIN
vitis -s build_platform.py \
    --platform_name $(PLATFORM_NAME) \
    --platform_out  $(ROOT_DIR) \
    --boot_image    $(BOOT_IMAGE)
```

See [generate_platform.py](./ref_files/step2_sw/generate_platform.py) and [build_platform.py](./ref_files/step2_sw/build_platform.py) for the full Python CLI usage.

### Fast Track

The Makefile drives the whole flow. From `ref_files/step2_sw/`:

```bash
source <Vitis_Install_Directory>/settings64.sh
export COMMON_IMAGE_VERSAL=<path/to/xilinx-versal-common-v2026.1/>
make all
```

The generated platform appears in `step2_sw/vek280_custom_dfx/export/vek280_custom_dfx/vek280_custom_dfx.xpfm`.

To clean:

```bash
make clean
```

### Next Step

Continue with [Step 3: Test the Platform](./step3.md).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/legal/copyright.html">Terms and Conditions</a></sup></p>
