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

# AMD Versal AI Edge Custom DFX Platform Creation Tutorial (VEK280 / AI Engine-ML)

***Version: Vitis 2026.1***

In this module, you will create a custom Dynamic Function eXchange (DFX) platform for the AMD Versal™ AI Edge VEK280 evaluation board to run AI Engine-ML (AIE-ML) and PL acceleration applications. Platforms that support DFX features allow you to reload acceleration kernels during runtime.

You can define one Reconfigurable Partition (RP) region and create multiple Reconfigurable Modules (RM). The RP defines the boundary between the reconfigurable region and the static region. The RM defines the content to be reconfigured each time. The RP can include Programmable Logic and AI Engine-ML resources. You can reconfigure the RM (`rm1.xclbin`, `rm2.xclbin`, etc.) during runtime with XRT. The AI Engine-ML array needs to be fully reconfigured or not at all. Multi-partition AIE-ML and PL DFX is not supported in this release.

This tutorial assumes that you understand the basics of AMD Versal™ Custom Platform creation, for example, creating AMD Vivado™ designs from a Vivado Customizable Example Design (CED) template, setting up software components from the AMD-released common image, creating AMD Vitis™ platforms, and creating basic acceleration applications for platform validation. This tutorial focuses on the differences between a DFX platform targeting AIE-ML on VEK280 and a regular flat platform.

The general Vitis acceleration platform workflow is to first build a base bootable design, then add platform-required components and properties on top of it. You should validate each step before moving on to prevent errors from propagating.

In this tutorial, you will use the Vivado Versal DFX Customizable Example Design to create the hardware design targeting the `xcve2802` device on VEK280. To prepare the software components, you will use the common image released by AMD together with the new Python CLI flow (`generate_platform.py` / `build_platform.py`) and `sdtgen`/`lopper` for device tree generation. After the software and hardware components are ready, you will package the platform and validate it with both a PL `vadd` application (pulled from the Vitis install samples) and a locally provided AIE-ML matrix-multiply application (shipped under `ref_files/step3_validate/aie_src/`).

Navigate through this tutorial using the following table of contents:

- [Step 1: Create the Hardware Design](./step1.md)
- [Step 2: Create the Software Components and the Vitis DFX Platform](./step2.md)
- [Step 3: Test the Platform (vadd + AIE-ML matmul)](./step3.md)
- [Iteration Guidelines](./Iteration_guideline.md)

## Requirements and Assumptions

This tutorial targets the **AMD Versal AI Edge VEK280 evaluation board** (`xcve2802-vsvh1760-2MP-e-S`). The same methodology applies to other Versal AI Edge boards with AIE-ML.

This design requires a Linux host machine with internet access. The Linux distribution must be supported by Vitis 2026.1.

You must have the following toolchain and assets installed and accessible:

| Component | Notes |
| --- | --- |
| AMD Vitis™ 2026.1 | Provides `vitis`, `v++`, `aiecompiler`, `bootgen`, `sdtgen`, `lopper`, `emconfigutil`, `aarch64-linux-gnu-g++`. |
| Vivado 2026.1 | Required for step 1 (synth, P&R, write_hw_platform). Bundled with the Vitis install. |
| Common image for Versal (2026.1) | `Image`, `rootfs.ext4`, `boot.scr`, `bl31.elf`, `u-boot.elf`, `sdk.sh`. Export as `COMMON_IMAGE_VERSAL`. Download from the [AMD Embedded Platforms](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html) page. |

This tutorial assumes you are familiar with the standard Vitis flat-platform creation flow. If not, refer to the [VCK190 Platform Creation Tutorial](../03_Edge_VCK190/README.md) and the [Vitis Acceleration Flow User Guide (UG1701)](https://docs.amd.com/r/en-US/ug1701-vitis-accelerated-embedded/Getting-Started-with-Vitis-Unified-Software-Platform) before starting.

## What's Next

Start with [Step 1: Create the Hardware Design](./step1.md).

## Reference

- *Vitis Acceleration Flow User Guide* [(UG1701)](https://docs.amd.com/r/en-US/ug1701-vitis-accelerated-embedded/Developing-Vitis-Kernels-and-Applications)
  - [Creating Embedded Platforms in Vitis](https://docs.amd.com/r/en-US/ug1701-vitis-accelerated-embedded/Creating-and-Using-Vitis-Platforms)
- *Vivado Design Suite User Guide: Dynamic Function eXchange* [(UG909)](https://docs.amd.com/r/en-US/ug909-vivado-partial-reconfiguration)
- [Versal AI Edge VEK280 Evaluation Kit](https://www.amd.com/en/products/system-on-modules/kria/k26/vek280-evaluation-kit.html)
- [Vitis Embedded Platform Source GitHub Repository](https://github.com/Xilinx/Vitis_Embedded_Platform_Source)

## Changelog

### 2026.1

- Migrated from 2025.2 to 2026.1.
- Localized AIE-ML application sources under `ref_files/step3_validate/aie_src/`; step 3 no longer copies the AIE-ML graph from `$XILINX_VITIS/samples/`. The `vadd` application is still pulled from `$XILINX_VITIS/samples/vadd/`.
- Updated `VERSION=2026.1` in `step2_sw/Makefile` and `step3_validate/Makefile`.
- Reworked `makefile_aieml` and `makefile_vadd` `v++ --package` invocations to use explicit boot components (`--package.bl31`, `--package.uboot`, `--package.generate_sdcard`, `--package.sd_file <Image>`/`<boot.scr>`) instead of the legacy `--package.image_format`/`--package.boot_mode`/`--package.kernel_image` set. This selects the classic non-EDF QEMU launcher and works around the 2026.1 `82-12113 "DTSI file type is not found in XSA"` regression that affects CED-based custom DFX platforms.
- Lowered AIE-ML `defaultFreqHz` from 312.5 MHz to 156.25 MHz in `aie_src/system.cfg` to close timing on the `mm2s_8_128 srem` divider with the 2026.1 default impl strategy.
- Added top-level documentation (README, step1, step2, step3, Iteration_guideline) mirroring the layout of `04_Edge_VCK190_DFX`.

### 2025.2

- First release of this tutorial for the VEK280 DFX platform.

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/legal/copyright.html">Terms and Conditions</a></sup></p>
