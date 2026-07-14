<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Getting Started with AMD Versal™ Adaptive SoCs using AMD Vitis™ Unified IDE and the Embedded Development Framework (EDF)</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

***Version: Vitis 2026.1 and Vivado 2026.1***

Welcome to the Vitis Getting Started tutorials for the **AMD Embedded Development Framework (EDF)**.

These tutorials showcase the steps to build AI Engine (AIE) and High-Level Synthesis (HLS) kernels, compile a host application, and run the design in **hardware emulation (QEMU)** and **on the board**, using pre-built EDF Yocto Software Development Kit (SDK), rootfs WIC (Yocto disk image), and QEMU prebuilts instead of the legacy PetaLinux common-image flow.

The tutorials use pre-built base platforms from the Vitis installation path to compile AIE and HLS kernels. They are aligned with the AMD EDF methodology described on the official [AMD EDF Wiki page](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/3250585601/AMD+Embedded+Development+Framework+EDF#Introduction-to-the-AMD-Embedded-Development-Framework).

Please read the `Makefile` provided in each sub-tutorial to better understand the Vitis EDF tool flow.

| Tutorial | Board | AI Engine Architecture | Status |
|---|---|---|---|
| [VCK190](./VCK190/README.md) | AMD Versal™ AI Core VCK190 | AI Engine | EDF flow (Vitis 2026.1) |
| [VEK280](./VEK280/README.md) | AMD Versal™ AI Edge VEK280 | AI Engine-ML (AIE-ML) | EDF flow (Vitis 2026.1) |
| [VEK385](./VEK385/README.md) | AMD Versal™ AI Edge Gen2 VEK385 | AI Engine 2-PS (AIE2-PS) | EDF flow (Vitis 2026.1) |
| [VRK160](./VRK160/README.md) | AMD Versal™ RF Series VRK160 | AI Engine (AIE) | EDF flow (Vitis 2026.1) |

> **Note on VCK190 DFX:** Dynamic Function eXchange (DFX) is **not supported in the EDF flow for Vitis 2026.1**. For the DFX flow, use the legacy [`Versal_w_PetaLinux/VCK190_dfx`](../Versal_w_PetaLinux/VCK190_dfx/README.md) tutorial.

## Setup and Initialization

IMPORTANT: Before beginning any sub-tutorial, ensure you have:

* Installed AMD Vitis™ 2026.1 software and set `PLATFORM_REPO_PATHS` to `<Vitis_tools>/base_platforms`.
* Created a directory `<path-to-design>/yocto_artifacts` and exported `YOCTO_ARTIFACTS` to point at it:

  ```bash
  export YOCTO_ARTIFACTS=<path-to-design>/yocto_artifacts
  ```

Download the EDF Yocto artifacts for your board from the [Embedded Development Framework (EDF) downloads page](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-design-tools.html) (package matching Vitis 2026.1):

### Versal Gen1 (Cortex-A72) – VCK190, VEK280, and VRK160

* **EDF Application SDK (Cortex-A72)**: `amd-cortexa72-common_meta-edf-app-sdk`. Run the installer and direct the output to `${YOCTO_ARTIFACTS}/amd-cortexa72-common_meta-edf-app-sdk/`:

  ```bash
  ./amd-edf-glibc-x86_64-meta-edf-app-sdk-cortexa72-cortexa53-amd-cortexa72-common-toolchain.sh \
      -d ${YOCTO_ARTIFACTS}/amd-cortexa72-common_meta-edf-app-sdk/sdk -y
  ```

* **EDF Platform Disk Image (Cortex-A72)**: `amd-cortexa72-common_edf-platform-disk-image`. Unzip and move into `${YOCTO_ARTIFACTS}/`.
* **QEMU Prebuilt (per board)**:
  * VCK190: `amd-cortexa72-common_vck190_qemu_prebuilt`
  * VEK280: `amd-cortexa72-common_vek280_qemu_prebuilt`
  * VRK160: `amd-cortexa72-common_vrk160_qemu_prebuilt`

  Unzip and move each into `${YOCTO_ARTIFACTS}/`.

### Versal Gen2 (Cortex-A78) – VEK385

* **EDF Application SDK (Cortex-A78)**: `amd-cortexa78-mali-common_meta-edf-app-sdk`. Run the installer and direct the output to `${YOCTO_ARTIFACTS}/amd-cortexa78-mali-common_meta-edf-app-sdk/sdk.sh`.
* **VEK385 Octal SPI (OSPI) Image**: Download and move into `${YOCTO_ARTIFACTS}/`.
* **EDF Platform Disk Image (Cortex-A78)**: `amd-cortexa78-mali-common_edf-platform-disk-image` (WIC). Unzip and move into `${YOCTO_ARTIFACTS}/`.
* **QEMU Prebuilt (VEK385)**: `amd-cortexa78-mali-common_vek385_qemu_prebuilt`. Unzip and move into `${YOCTO_ARTIFACTS}/`.

After downloading, your `yocto_artifacts/` directory should contain a subdirectory per artifact. Each sub-tutorial `README.md` documents the exact `make` commands to invoke for hardware emulation (`make all`) and hardware run (`make sd_card`).

<p class="sphinxhide" align="center"><sub>Copyright © 2025–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
