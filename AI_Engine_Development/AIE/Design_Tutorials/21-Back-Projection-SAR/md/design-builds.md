
<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on amd.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on amd.com</a>
    </td>
 </tr>
</table>

# Back-Projection for Synthetic Aperture Radar on AI Engines
## Design Builds

### Setup and Initialization

IMPORTANT: Before beginning the tutorial ensure you have installed Vitis™ 2025.1 software. Ensure you have downloaded the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html).

Set the environment variable ```COMMON_IMAGE_VERSAL``` to the full path where you have downloaded the Common Images. Then set the environment variable ```PLATFORM_REPO_PATHS``` to the value ```$XILINX_VITIS/base_platforms```. Additional information on this process may be found [here](../../../AIE#environment-settings).

The remaining environment variables are configured in the top level Makefile ```<path-to-design>/21-Back-Projection-SAR/Makefile``` file.

```
RELEASE=2025.1

TOP_DIR                   ?= $(shell readlink -f .)
PLATFORM_NAME              = xilinx_vck190_base_202510_1
PLATFORM_PATH              = ${PLATFORM_REPO_PATHS}

export PLATFORM            = ${PLATFORM_PATH}/${PLATFORM_NAME}/${PLATFORM_NAME}.xpfm
export SYSROOT             = ${COMMON_IMAGE_VERSAL}/sysroots/cortexa72-cortexa53-amd-linux
export KERNEL_IMAGE        = ${COMMON_IMAGE_VERSAL}/Image
export ROOTFS              = ${COMMON_IMAGE_VERSAL}/rootfs.ext4
export PREBUILT_LINUX_PATH = ${COMMON_IMAGE_VERSAL}
```

### Single Engine Design Build

The single engine SAR design can be built for the VCK190 board using the Makefile as follows:
```
[shell]% cd <path-to-design>/21-Back-Projection-SAR
[shell]% make dev1
```

The build process with generate the SD card image in the ```<path-to-design>/21-Back-Projection-SAR/package1/sd_card``` folder.

### Multiple Engine Design Build

The multiple engine SAR design can be built for the VCK190 board using the Makefile as follows:
```
[shell]% cd <path-to-design>/21-Back-Projection-SAR
[shell]% make dev8
```

The build process with generate the SD card image in the ```<path-to-design>/21-Back-Projection-SAR/package8/sd_card``` folder.

<p class="sphinxhide" align="center"><sub>Copyright © 2025 Advanced Micro Devices, Inc</sub></p>
<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
