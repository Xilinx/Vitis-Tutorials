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

# AMD Versal Custom DFX Platform Creation Tutorial

***Version: 2026.1***

In this module, you will create a custom Dynamic Function eXchange (DFX) platform to run acceleration applications for VCK190 evaluation board. Platforms that support DFX features allows you to reload acceleration kernels during runtime.

You can define one reconfigurable partition (RP) region and create multiple reconfigurable modules (RM). The RP defines the boundary between reconfigurable region and static region. The RM defines the content to be reconfigured each time. The RP can include Programmable Logic and AI Engine resources. You can reconfigure the RM (``rm1.xclbin``, ``rm2.xclbin``, etc.) during runtime with XRT. The AI Engine needs to be fully reconfigured or not at all. Multi-partition AI Engine and PL DFX is not supported in this version.

![DFX Diagram](./images/dfx_rp_rm.svg)

This tutorial assumes that you understand the basics of the AMD Versal™ Custom Platform creation steps, for example, creating AMD Vivado™ designs from Vivado Customizable Example Design template, setting up software components with common images, creating AMD Vitis™ platforms, and creating basic acceleration applications for platform validation. This tutorial will focus on the differences between the DFX platforms and the normal flat platforms.

In a general Vitis acceleration platform design, you start from building a base bootable design, then add platform required components and properties to it. You must validate each step before going to the next step to prevent error propagating. This workflow is described in the following diagram.

![Vitis DFX Platform Creation Steps Diagram](images/platform_creation_steps.svg)

In this tutorial, you will use the Vivado Versal DFX customizable example design to create the hardware design.

To prepare the software components, use the common image released by AMD and the `createdts` command to generate the device tree file.

After the software and hardware components are ready, you can package the platform.

In each step, test the generated files to make sure they work as expected. A frequent test methodology can help to narrow down the root causes if any error occurs. Finally, run several test applications on this platform to test this customized platform.

Navigate through this tutorial using the following table of contents links.

- [Step 1: Hardware Settings for Vitis Platform](./step1.md)
- [Step 2: Create the Software Components and Generate Platform](./step2.md)
- [Step 3: Test the Platform](./step3.md)
- [Iteration Guidelines](./Iteration_guideline.md)
- [Frequently Asked Questions](faq.md)

## Requirements and Assumptions

This tutorial design uses VCK190 production board for demonstration. The methodology works for all boards.

This design requires a Linux host machine with Internet access. The Linux OS needs to support the Vitis software and PetaLinux.

This tutorial assumes that you are familiar with Vitis platform creation flow. If not, refer to VCK190 Platform Creation Tutorial and *Vitis Acceleration Flow User Guide* [(UG1701)](https://docs.amd.com/r/en-US/ug1701-vitis-accelerated-embedded/Getting-Started-with-Vitis-Unified-Software-Platform) for more information.

## What's Next

Start with [Step 1](./step1.md).

## Reference

- *Vitis Acceleration Flow User Guide* [(UG1701)](https://docs.amd.com/r/en-US/ug1701-vitis-accelerated-embedded/Developing-Vitis-Kernels-and-Applications)

   - [Creating Embedded Platforms in Vitis](https://docs.amd.com/r/en-US/ug1701-vitis-accelerated-embedded/Creating-and-Using-Vitis-Platforms)

- [Vitis Embedded Platform Source Github Repository](https://github.com/Xilinx/Vitis_Embedded_Platform_Source)

## Changelog

### 2026.1

- Migrated from 2025.2 to 2026.1.
- Updated `VERSION=2026.1` in `step2_sw/Makefile` and `step3_validate/Makefile`.
- Reworked `makefile_aie` and `makefile_vadd` `v++ --package` invocations for both the `hw_emu` and `hw` targets to use explicit boot components (`--package.bl31`, `--package.uboot`, `--package.generate_sdcard`, `--package.sd_file <Image>`/`<boot.scr>`) instead of the legacy `--package.image_format`/`--package.boot_mode`/`--package.kernel_image` set. The legacy flags trigger a 2026.1 `82-12113 "DTSI file type is not found in XSA"` warning on CED-based custom DFX platforms; for `hw`, this went on to fail SD-card image creation outright (`82-10337`).
- Fixed `cpAieCode` in `step3_validate/Makefile`: it copied the AIE sample sources with `cp -a` (preserving the Vitis install's original timestamps) into `aie_work/` without first removing a prior build. Since a stale `hw_emu`-built `krnl_adder.xclbin` could be newer than the freshly-copied (but old-timestamped) sources, `make sd_card` would silently skip rebuilding it (`Nothing to be done for 'sd_card'`) if run after `make all` in the same tree. Now does `rm -rf aie_work` first and copies with `cp -rf`, matching `cpVaddCode` and the VEK280 DFX tutorial.
- Fixed `step3.md`'s `platforminfo` walkthrough: the documented `cd` path (`step2_sw/build/pfm/vck190_custom_dfx/export/...`) no longer matches where `step2_sw/Makefile` actually exports the platform (`step2_sw/vck190_custom_dfx/export/...`). Updated the path and refreshed the sample `platforminfo` output to match the current tool's output format.

### 2025.2

- Update system-user.dtsi as per CED design.
- Update Makefile as per new Python CLI support and python scripts.

### 2025.1

- Update system-user.dtsi as per CED design.
- Update Makefile as per new VADD applications.
- Update platform settings to change selection of default clock to 156.25Mhz

### 2023.1

- Hardware emulation XSA file is no longer mandatory required to create a platform

### 2022.2

- Versal DFX CED is utilized to create the hardware design

### 2022.1

- New Tutorial

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
