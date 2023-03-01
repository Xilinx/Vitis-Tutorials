#!/bin/bash
#
# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#


## global variables
export BOARD=vck190
export RELEASE=2022.2
export TUTORIAL=03-HLS_Code_Optimization

## working directory
export WRK_DIR=/group/xirguup/danieleb/ide$RELEASE/Vitis-Tutorials-$RELEASE\_19jan2023/Developer_Contributed # MY GUUP
#export WRK_DIR=/media/danieleb/DATA/${RELEASE}/Vitis-Tutorials-${RELEASE}/Developer_Contributed # MY DESKTOP

## dsp library root directory
export DSPLIB_ROOT=/group/xirguup/danieleb/ide$RELEASE/Vitis_Libraries/dsp # MY GUUP
ts $RELEASE\_released -petalinux petalinux-v$RELEASE\_daily_latest #-matlab R2021a
#export DSPLIB_ROOT=${WRK_DIR}/../../Vitis_Libraries-main/dsp               # MY DESKTOP

## petalinux
#source ${WRK_DIR}/${TUTORIAL}/files/scripts/env_setup_petalinux.sh
source ${WRK_DIR}/${TUTORIAL}/files/scripts/guup_env_setup_versal.sh



# -------------------------
# The following Env Var are provided by Regression Framework. No need to setup manually
# - XILINX_VITIS
# - XILINX_VIVADO
# - XILINX_HLS
# - PLATFORM_REPO_PATHS
# - XILINX_XRT=/proj/xbuilds/XRT/2022.2/202220.2.14.354/packages/x86_64/xrt-2.14.354_ubuntu_18.04/opt/xilinx/xrt
# - PLATFORM_REPO_PATHS=/proj/rdi/xbuilds/released/2022.2/2022.2_1015_1/internal_platforms
# -------------------------

: '
## only in case of using MY DESKTOP
export XILINX_TOOLS_LOCATION=/media/danieleb/DATA/${RELEASE}/tools/Xilinx/Vitis/${RELEASE}
export XILINX_VITIS=${XILINX_TOOLS_LOCATION}
export XILINX_HLS=${XILINX_VITIS}/../../Vitis_HLS/${RELEASE}
source ${XILINX_VITIS}/settings64.sh
'

## valid for both GUUP and my DESKTOP
export VITIS_PLATFORM_DIR=${XILINX_VITIS}/base_platforms
export VITIS_PLATFORM=xilinx_${BOARD}\_base_202220_1
export VITIS_PLATFORM_PATH=${VITIS_PLATFORM_DIR}/${VITIS_PLATFORM}/${VITIS_PLATFORM}.xpfm
export PLATFORM_REPO_PATHS=${VITIS_PLATFORM_PATH}
