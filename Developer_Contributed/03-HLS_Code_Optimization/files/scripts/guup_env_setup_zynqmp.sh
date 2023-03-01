#!/bin/bash
#
# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

# -------------------------
# Setup common environments
# - Vitis Library Path
# -------------------------
#source $WRK_DIR/$TUTORIAL/files/scripts/env_setup.sh

# ----------------------------
# Environment Setup for ZynqMP
# ----------------------------
#export COMMON_IMAGE_ZYNQMP=/opt/xilinx/common/xilinx-zynqmp-common-v${RELEASE}     # MY DESKTOP
export COMMON_IMAGE_ZYNQMP=/home/danieleb/common/xilinx-zynqmp-common-v${RELEASE}        # MY GUUP
echo Setting Up COMMON_IMAGE_ZYNQMP=${COMMON_IMAGE_ZYNQMP}

# Run Environment setup to get SDKTARGETSYSROOT and CXX
unset LD_LIBRARY_PATH
source ${COMMON_IMAGE_ZYNQMP}/environment-setup-cortexa72-cortexa53-xilinx-linux

# For using createdts in Sprite
export _JAVA_OPTIONS="-Duser.home=/tmp"
