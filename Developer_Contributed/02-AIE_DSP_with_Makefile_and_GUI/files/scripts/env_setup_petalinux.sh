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

export BSP_DIR=/proj/petalinux/${XLNX_RELEASE}/petalinux-v${XLNX_RELEASE}_daily_latest/bsp/release
export PETALINUX=/proj/petalinux/${XLNX_RELEASE}/petalinux-v${XLNX_RELEASE}_daily_latest/tool/petalinux-v${XLNX_RELEASE}-final


source $PETALINUX/settings.sh


# -------------------------
# The following Env Var are provided by Regression Framework. No need to setup manually
# - XILINX_VITIS
# - XILINX_VIVADO
# - XILINX_HLS
# - PLATFORM_REPO_PATHS
# -------------------------
