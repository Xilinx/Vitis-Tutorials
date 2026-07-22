<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Versal™ AI Edge Gen2 Design Flow with Vitis™ Unified IDE</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

# Versal AI Edge Gen2 Design Flow with Vitis Unified IDE

***Version: Vitis 2026.1 and Vivado 2026.1***

In this module, you will create an acceleration application for the VEK385 Evaluation board, starting from an extensible XSA instead of an extensible platform.

First, you'll generate the extensible XSA using the Versal Gen2 CED. This CED includes two parts:

   - Base part: PS and PS-to-NoC-DDR connectivity

   - Extensible part: PL and AIE regions

   ![Application development ](images/ced_structure.svg)

The base part will serve as the foundation to generate the EDF WIC image. For more information about AMD EDF, please refer to the official [AMD EDF Documentation](https://edf.docs.amd.com/en/latest/) (the 2026.1 home for EDF documentation, superseding the legacy Confluence wiki).  The extensible part will later be used to link with your custom kernels, ensuring your development flow is fully aligned with the AMD EDF methodology.

   >Note: This CED design enables segmented configuration by default. The PS-NoC-to-LPDDR is used to initialize the LPDDR memory and provide access to it during system bring-up. For more details about segmented configuration, please refer to [UG1273](https://docs.amd.com/r/en-US/ug1273-versal-acap-design/Segmented-Configuration).

 Then, you'll develop the AIE and HLS kernels and link them with the extensible XSA to produce a fixed XSA. Finally, you'll develop the acceleration application based on this fixed XSA.

The diagram below illustrates this XSA-based flow.

![Application development ](images/xsa_flow_new.svg)

The following sections will introduce the detailed steps. Each section describes one major step in the platform creation process.

- [Step 1: Create the Extensible hardware](./step1.md)</br>
             Create Vivado Design to generate extensible XSA
- [Step 2: Kernel Integration](./step2.md)</br>
            Develop and Integrate Kernels to generate final hardware design
- [Step 3: Application](./step3.md)</br>
             Software application development

### Setup and Initialization

IMPORTANT: Before beginning the tutorial, ensure you have:
* Installed AMD Vitis™ 2026.1 software and set `PLATFORM_REPO_PATHS` to the value `<Vitis_tools>/base_platforms`.
* Created directory `<path-to-design>/yocto_artifacts` and set environment variable YOCTO_ARTIFACTS to that path.
      
      For Bash shell:
      export YOCTO_ARTIFACTS=<path-to-design>/yocto_artifacts
      For CSH Shell:
      setenv YOCTO_ARTIFACTS "<path-to-design>/yocto_artifacts"
* Download the AMD Embedded Development Framework (EDF) **v26.06** release (Yocto manifest `amd-edf-rel-v26.06`, EDF branch `rel-v2026.1`) matching Vitis 2026.1, from the [AMD account downloads portal](https://account.amd.com/en/forms/downloads/xef.html). See the [Downloads and Release Notes page](https://edf.docs.amd.com/en/latest/downloads-and-release-notes.html) for the full artifact list and MD5 checksums:
  * Download `amd-edf-glibc-x86_64-meta-edf-app-sdk-cortexa72-cortexa53-amd-cortexa78-mali-common-toolchain-26.06+release-*.sh`, run the script and set path output to `<path-to-design>/yocto_artifacts/amd-cortexa78-mali-common_meta-edf-app-sdk/sdk.sh`.
  * Download the VEK385 OSPI image `edf-ospi-versal-2ve-2vm-vek385-multidomain-*.bin` and move into `<path-to-design>/yocto_artifacts/`.
  * Download `edf-platform-disk-image-amd-cortexa78-mali-common.rootfs-*.wic.ufs.xz` (amd-cortexa78-mali-common_edf-platform-disk-image, SD wic), unzip and move into `<path-to-design>/yocto_artifacts/`.
  * Download `amd-cortexa78-mali-common_vek385_qemu_prebuilt.tar.gz`, unzip and move `amd-cortexa78-mali-common_vek385_qemu_prebuilt` into `<path-to-design>/yocto_artifacts/`.
This tutorial is aligned with the AMD Embedded Development Framework (EDF). For more information about AMD EDF, please refer to the official [AMD EDF Documentation](https://edf.docs.amd.com/en/latest/) (the 2026.1 home for EDF documentation, superseding the legacy Confluence wiki).

<p class="sphinxhide" align="center"><sub>Copyright © 2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/legal/copyright.html">Terms and Conditions</a></sup></p>
