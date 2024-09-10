#!/bin/bash
# -------------------------
# Use PetaLinux toolset
# Setup common image path COMMON_IMAGE_ZYNQMP
# -------------------------




# -------------------------
# Setup common environments
# - Vitis Library Path
# -------------------------
source env_setup.sh



# -------------------------------
# Environment Setup for PetaLinux
# -------------------------------

# A tutorial either uses the pre-built SDK toolchain or the PetaLinux to build things from scratch.

source env_setup_petalinux.sh

# -------------------------------
# Versal Common Image Path 
# -------------------------------

export COMMON_IMAGE_ZYNQMP=/proj/xbb/xrt-embedded/2024.1/sw/latest/sysroot/zynqmp/xilinx-zynqmp-common-v2024.1
echo Setting Up COMMON_IMAGE_ZYNQMP=$COMMON_IMAGE_ZYNQMP


# -------------------------
# The following Env Var are provided by Regression Framework. No need to setup manually
# - XILINX_VITIS
# - XILINX_VIVADO
# - XILINX_HLS
# - PLATFORM_REPO_PATHS
# -------------------------
