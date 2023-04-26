

#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

# =======================================================
# Set Platform ,Vitis and Versal Image repo
# =======================================================
export PLATFORM_REPO_PATHS= <YOUR-2023.1-PLATFORM-DIRECTORY>
export XILINX_VITIS = <YOUR-2023.1-VITIS-DIRECTORY>/2023.1
export COMMON_IMAGE_VERSAL=<YOUR-XILINX-VERSAL-COMMON-V2023.1-DIRECTORY>

# ====================================================
# Source Versal Image ,Vitis and Aietools
# ====================================================
# Run the below command to setup environment and CXX
source $COMMON_IMAGE_VERSAL/environment-setup-cortexa72-cortexa53-xilinx-linux
source $XILINX_VITIS/settings64.sh

# ========================================================
# Set DSP Library for Vitis
# ========================================================
export DSPLIB_VITIS=<Path to Vitis Libs - Directory>

# =========================================================
# Platform Selection...
# =========================================================
tgt_plat=xilinx_vck190_base_202310_1
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
echo ""
echo "DSPLIBS"
echo "$DSPLIB_VITIS"
echo ""
