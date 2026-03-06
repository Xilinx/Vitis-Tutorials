
<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ Getting Started Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
    </td>
  </tr>
</table>


# Vitis Introduction and Getting Started Tutorial

***Version: Vitis 2025.2***

Welcome to Vitis Getting Started!

In this tutorial we will showcase how to swap in and out two different AIE Graphs and HLS kernel dynamically using a single host application. For this use case we are using pre-built xilinx_vck190_base_dfx_202520_1.xpfm platform to compile AIE and HLS kernel.

**Note: This tutorial only supports HW flow.**

###  Following is the pictorial representation of DFX Design development Flow:
**Vitis 2025.2**

```mermaid
flowchart LR
    %% 平台
    PFM[VCK190 DFX XPFM <br/> DTB, BOOT.bin self contained]

    %% 左侧 DR1
    subgraph DR1[Dynamic Region 1]
      direction TB
      HLS1[HLS Kernels 1]
      AIE1[AIE Graph 1]
      XSA1[Fixed_XSA: DR1.xsa<br/>v++ -l]
      XCL1[XCLBIN1<br/>v++ -p defer_aie_run]
    end

    %% 右侧 DR2
    subgraph DR2[Dynamic Region 2]
      direction TB
      HLS2[HLS Kernels 2]
      AIE2[AIE Graph 2]
      XSA2[Fixed_XSA: DR2.xsa<br/>v++ -l]
      XCL2[XCLBIN2<br/>v++ -p defer_aie_run]
    end

    %% SD卡镜像输入
    ROOTFS[Rootfs]
    IMAGE[Linux Image]
    HOST[HOST Application]

    %% 输出
    SD[SD_card.img<br/>v++ -p package]

    %% —— 关键：用隐形锚点把 PFM 分到左右 ——
    PFM --> PFM_L
    PFM --> PFM_R
    PFM_L --> XSA1
    PFM_R --> XSA2
    %% 隐形样式
    style PFM_L fill:transparent,stroke:transparent
    style PFM_R fill:transparent,stroke:transparent

    %% DR1 连接（不与 PFM 交叉）
    HLS1 --> XSA1
    AIE1 --> XSA1
    XSA1 --> XCL1
    AIE1 --> XCL1

    %% DR2 连接
    HLS2 --> XSA2
    AIE2 --> XSA2
    XSA2 --> XCL2
    AIE2 --> XCL2

    %% 打包
    XCL1 --> SD
    XCL2 --> SD
    ROOTFS --> SD
    IMAGE --> SD
    HOST  --> SD
    PFM   --> SD

    %% 样式
    classDef platform fill:#e8f6f3,color:#2c3e50,stroke:#1abc9c
    classDef aie fill:#fef9e7,color:#2c3e50,stroke:#f39c12
    classDef hls fill:#f4ecf7,color:#2c3e50,stroke:#8e44ad
    classDef xsa fill:#fdebd0,color:#2c3e50,stroke:#e67e22
    classDef xclbin fill:#d5f5e3,color:#2c3e50,stroke:#27ae60
    classDef sd_components fill:#f4ecf7,color:#2c3e50,stroke:#8e44ad
    classDef sd_image fill:#e8daef,color:#2c3e50,stroke:#7d3c98

    class PFM platform
    class AIE1,AIE2 aie
    class HLS1,HLS2 hls
    class XSA1,XSA2 xsa
    class XCL1,XCL2 xclbin
    class ROOTFS,IMAGE,HOST sd_components
    class SD sd_image
```

>Note: In this diagram, PFM_L and PFM_R represent the same vck190 base DFX platform as XPFM — they are just shown separately to make the diagram lines clearer.

## Prerequisites

1. Source Vitis 2025.2:

   ```
   source <path_to_vitis_install>/settings64.sh
   ```

2. Export the following variable:

   ```
   export COMMON_IMAGE_VERSAL=<path_to_common_image: xilinx-versal-common-v2025.2>
   ```

3. To compile the binaries:

   ```
   cd <path_to_Tutorial>/Getting_Started/Vitis/Versal_w_PetaLinux/<platform_to_test>
   make sd_card
   ```
   # Hardware Run

To run the design on hardware, please ensure the design was run with the following command:

```
make all TARGET=hw
```

The above command will generate `pack_out_dir`, containing all the file required for the hardware run.

## Steps to run the design on Hardware

### 1. Connect the VCK190 board and set the SD boot mode

Refer to [UG1366](https://docs.amd.com/r/en-US/ug1366-vck190-eval-bd) to get more details on VCK190 board.

### 2.  Flash the SD Card

Use the balenaEtcher/similar tools to flash the SD card. Plug in the SD card to the VCK190 board to initiate the boot.

### 3.  Running the design and application on the VCK190

After the Linux boot is complete, use the below commands to run the `host.exe`, `dfx1.xclbin` as well as `dfx2.xclbin`
`versal-rootfs-common-20252:/mnt#`

```
sudo su
cd /run/media/mmcblk0p1
./run_aie.sh
```

- Check the results

```
TEST PASSED
```

With the above results on the hardware run, we have reached to the end of this tutorial.


<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
