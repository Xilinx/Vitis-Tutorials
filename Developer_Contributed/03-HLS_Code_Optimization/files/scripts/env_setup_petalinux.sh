#!/bin/bash
# -------------------------
# Setup common environments
# - Vitis Library Path
# -------------------------
#source env_setup.sh



# -------------------------------
# Environment Setup for PetaLinux
# -------------------------------

# A tutorial either uses the pre-built SDK toolchain or the PetaLinux to build things from scratch.

export BSP_DIR=/proj/petalinux/2024.1/petalinux-v2024.1_daily_latest/bsp/release
export PETALINUX=/proj/petalinux/2024.1/petalinux-v2024.1_daily_latest/tool/petalinux-v2024.1-final


source $PETALINUX/settings.sh


# -------------------------
# The following Env Var are provided by Regression Framework. No need to setup manually
# - XILINX_VITIS
# - XILINX_VIVADO
# - XILINX_HLS
# - PLATFORM_REPO_PATHS
# -------------------------
