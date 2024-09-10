#!/bin/bash
# -------------------------
# Setup common environments
# - Vitis Library Path
# -------------------------
source env_setup.sh


#export COMMON_IMAGE_VERSAL=/proj/xbb/xrt-embedded/2023.1/sw/latest/sysroot/versal/xilinx-versal-common-v2023.1 #DB: this works only on USA servers
export COMMON_IMAGE_VERSAL=/home/danieleb/common/xilinx-versal-common-v${XLNX_RELEASE}
echo Setting Up COMMON_IMAGE_VERSAL=$COMMON_IMAGE_VERSAL #DB: this works also for EMEA servers

# Run Environment setup to get SDKTARGETSYSROOT and CXX
unset LD_LIBRARY_PATH
source ${COMMON_IMAGE_VERSAL}/environment-setup-cortexa72-cortexa53-xilinx-linux


# -------------------------
# The following Env Var are provided by Regression Framework. No need to setup manually
# - XILINX_VITIS
# - XILINX_VIVADO
# - XILINX_HLS
# - PLATFORM_REPO_PATHS
# -------------------------
