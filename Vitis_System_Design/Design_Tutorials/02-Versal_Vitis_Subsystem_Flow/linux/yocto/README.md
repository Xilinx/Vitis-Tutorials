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


# Yocto/EDF with custom machine

This section downloads and builds a Linux environment using EDF Yocto manifest.
The `repo` tool is first setup locally, then used to download the dependencies from various github sites.

***Important:***
1. Yocto requires non-NFS drives to be used!
2. Yocto commands are sensitive to setup from other tools, run in a clean shell environment!


To further speed-up the compile process a local temporary drive is preferred.
The temporary location can be redirected by setting `TMP_DIR` as environment or variable to top Makefile

## Shell environments used by Yocto
The Yocto environment support various roles and activities, which have different demands in tool setup.
To avoid having to install these tools with root/sudo, they are downloaded and setup locally.

### Shell 1 / Yocto
The setup for this shell enables basic activities like downloading the environment and setting up a Yocto Linux project, adding custom layers to the project and running the EDF image recipe.
The Makefiles will configure/load `edf-init-build-env` which creates a project in missing and setup the environment for bitbake.

### Shell 2 / Yocto + buildtools
This shell extends the Yocto setting with buildtools needed by gen-machine-conf.


## Install local repo, clone EDF using the repo tool and add a dummy layer placeholder
From shell 2 run:
```
cd linux/yocto
make add_layers
```
This will install repo tool in `linux/yocto/bin/repo` and use it to download EDF (version rel-v2026.1).
To speed up deletion of files, both the edf sources, build folder and yocto work/sstate cache, etc, are placed on `/tmp/<user_name>/<board>/`.
| Item | Path |
|----|----|
|EDF sources          |`/<TMP_DIR>/<tool_version>/<board>/edf/sources`|
|EDF project folder   |`/<TMP_DIR>/<tool_version>/<board>/edf/build`|
|Yocto project folder |`/<TMP_DIR>/<tool_version>/<board>/yocto`|
|Deploy folder        |`/<TMP_DIR>/<tool_version>/<board>/yocto/deploy`|

The paths can be overridden by using variables in the [Makefile](./Makefile).

The mechanism to set the Yocto TMP folder is to add to `build/conf/local.conf`.

## Generate custom machine configuration
This use shell 2 to run gen-machine-conf using the system device tree to create a custom linux machine.


## Build EDF Linux disk image
With the updated custom machine configuration, this steps runs `bitbake` with the image `edf-linux-disk-image`
The Makefile is prepared for several image recipes to allow user to experiment with various image collections. For more details refer to [EDF common specifications](https://edf.docs.amd.com/en/latest/ref/common-specifications.html#edf-common-specifications).

## Install host application SDK
To build the host applications bitbake runs the recipe `meta-edf-app-sdk` to preparing SDK.
The SDK is then installed to `sysroot` and is compiled by the `make ps_apps` rule in top `Makefile`.
For details on the host applications, see [Compile and build host applications](../../ps_apps/README.md)

## Prerate BOOT.bin and xclbin
This part is covered in [Package files for EDF](../../vitis/Package.md#Package-files-for-EDF)


## Update SD card
This step copies the updated BOOT.bin to primary partition of the EDF wic image and adds the `xclbin` files and host applications to `/home/amd-edf` folder on the root filesystem on partition 3 of the wic image.
To speed up the process, the wic manipulation is done on the `TMP_DIR` drive and copied to `../<custom-machine-name>/images/linux` when the wic image is compressed with `xz`.

***Note:*** If transferring the wic to windows laptop, use the zipped `xz` to speed up the download.


### Description of file structure

| Folder                           | Type   | Description
| ---------------------------------|--------|-------------------------------------------------
| [src](./src/)                    | Source | Various configuration and recipe examples used as reference during tutorial development.
| bin                              | Generated | Used for local install of `repo` tool which is used to download dependencies.

## Navigation helper
 - [Build host applications](../../ps_apps/README.md)
 - [Create BOOT.bin with Vitis package](../../vitis/Package.md)
 - [Previous step - Setup devicetree and extract processor domains](../sdtgen/README.md)
 - [Return to Linux overview](../README.md)
 - [Return to top](../../README.md)


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2026 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>

