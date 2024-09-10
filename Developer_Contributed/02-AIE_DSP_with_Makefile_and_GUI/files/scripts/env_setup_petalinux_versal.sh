#!/bin/bash
#!/bin/bash
# -------------------------
# Use PetaLinux toolset
# Setup common image path COMMON_IMAGE_VERSAL
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

export COMMON_IMAGE_VERSAL=/proj/xbb/xrt-embedded/${XLNX_RELEASE}/sw/latest/sysroot/versal/xilinx-versal-common-v${XLNX_RELEASE}
echo Setting Up COMMON_IMAGE_VERSAL=$COMMON_IMAGE_VERSAL


# -------------------------
# The following Env Var are provided by Regression Framework. No need to setup manually
# - XILINX_VITIS
# - XILINX_VIVADO
# - XILINX_HLS
# - PLATFORM_REPO_PATHS
# -------------------------
