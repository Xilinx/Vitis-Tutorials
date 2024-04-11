#!/bin/bash

#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

# =======================================================
# Set Platform ,Vitis and Versal Image repo
# =======================================================
export PLATFORM_REPO_PATHS= <YOUR-2023.2-PLATFORM-DIRECTORY>
export XILINX_VITIS = <YOUR-2023.2-VITIS-DIRECTORY>/2023.2
export COMMON_IMAGE_VERSAL=<YOUR-XILINX-VERSAL-COMMON-V2023.2-DIRECTORY>

# ====================================================
# Source Versal Image ,Vitis and Aietools
# ====================================================
# Run the below command to setup environment and CXX
source $COMMON_IMAGE_VERSAL/environment-setup-cortexa72-cortexa53-xilinx-linux
source $XILINX_VITIS/settings64.sh

# =========================================================
# Platform Selection...
# =========================================================
tgt_plat=xilinx_vek280_es1_base_202320_1
export PLATFORM=$PLATFORM_REPO_PATHS/$tgt_plat/$tgt_plat\.xpfm

# ==========================================================
# Validating Tool Installation
# ==========================================================
echo ""
echo "Aiecompiler:"
which aiecompiler
echo ""
echo "Vivado:"
which vivado
echo ""
echo "Vitis:"
which vitis
echo ""
echo "Vitis HLS:"
which vitis_hls
echo ""
