#!/bin/bash
#
# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Authors: Daniele Bagni, Peter Schillinger
# Date:    14 Feb 20223

# ******************************************************************************************************
## global variables
# ******************************************************************************************************
BOARD=vck190
RELEASE=2022.2
#BASE_NUM=202310_1
 BASE_NUM=202220_1

TUTORIAL=02-AIE_DSP_with_Makefile_and_GUI

# working directory: change with your own
DATA_DIR=/media/danieleb/DATA/$RELEASE
export WRK_DIR=$DATA_DIR/Vitis-Tutorials-$RELEASE/Developer_Contributed/$TUTORIAL

# FOR VITIS DSP LIBRARIES
export DSPLIB_VITIS=$DATA_DIR/Vitis_Libraries-main
export DSPLIB_ROOT=$DSPLIB_VITIS/dsp

# ******************************************************************************************************
# VITIS ENVIRONMENT SETTINGS
# ******************************************************************************************************


# FOR VITIS AIE TUTORIALS
export XILINX_VITIS=$DATA_DIR/tools/Xilinx/Vitis/$RELEASE

# set the Vitis/Vivado release
source ${XILINX_VITIS}/settings64.sh

# set petalinux release
source /tools/Xilinx/PetaLinux/$RELEASE/settings.sh

# for Vitis AI 3.0: change with your own or leave commented if you do not have it
#export VITIS_AI_PATH=/media/danieleb/DATA/VAI3.0

# XRT
export XILINX_XRT=/opt/xilinx/xrt #DB
source ${XILINX_XRT}/setup.sh

# ******************************************************************************************************
# for Model Composer
# ******************************************************************************************************
#$XILINX_VITIS/../../Model_Composer/2022.2/bin/model_composer -matlab /usr/local/MATLAB/R2021a -vitis $XILINX_VITIS

# ******************************************************************************************************
# COMMON VCK190  package
# ************************************************************************************************
export COMMON_IMAGE_VERSAL=/opt/xilinx/common/xilinx-versal-common-v$RELEASE

unset LD_LIBRARY_PATH
source ${COMMON_IMAGE_VERSAL}/environment-setup-cortexa72-cortexa53-xilinx-linux

# for VCK190P common sw package
export VERSAL_VITIS_PLATFORM=xilinx_${BOARD}\_base_${BASE_NUM}
export PLATFORM_REPO_PATHS=${XILINX_VITIS}/base_platforms
export VITIS_PLATFORM_DIR=${PLATFORM_REPO_PATHS}/${VERSAL_VITIS_PLATFORM}
export VITIS_PLATFORM_XPFM=${VITIS_PLATFORM_DIR}/${VERSAL_VITIS_PLATFORM}.xpfm

# SysRoot, RootFS and Image
export VITIS_SYSROOTS=${COMMON_IMAGE_VERSAL}/sysroots/cortexa72-cortexa53-xilinx-linux
export SDKTARGETSYSROOT=${VITIS_SYSROOTS}
export SYSROOT_DIR=${VITIS_SYSROOTS}
export K_IMAGE=${COMMON_IMAGE_VERSAL}/Image
export ROOTFS=${COMMON_IMAGE_VERSAL}/rootfs.ext4
export IMAGE=${K_IMAGE}

# ******************************************************************************************************
# for compatibility with official Vitis Tutorials
# ******************************************************************************************************
export XLNX_VERSAL=${COMMON_IMAGE_VERSAL}
export PLATFORM=${VITIS_PLATFORM_XPFM}

# ******************************************************************************************************
#some basic checking
# ******************************************************************************************************
error=0

: '
if [ -d "$TRD_HOME" ]; then
  echo "TRD_HOME=$TRD_HOME exists"
else
  echo "TRD_HOME=$TRD_HOME does not exist"
  error=$((error+1))
fi

if [ -d "$VITIS_AI_PATH" ]; then
  echo "VITIS_AI_PATH=$VITIS_AI_PATH exists"
else
  echo "VITIS_AI_PATH=$VITIS_AI_PATH does not exist"
  error=$((error+1))
fi

if [ -d "$MPSOCDPU_PRE_POST_PL_ACC" ]; then
echo "MPSOCDPU_PRE_POST_PL_ACC=$MPSOCDPU_PRE_POST_PL_ACC exists"
else
  echo "MPSOCDPU_PRE_POST_PL_ACC=$MPSOCDPU_PRE_POST_PL_ACC does not exist"
  error=$((error+1))
fi
'

if [ -d "$VITIS_SYSROOTS" ]; then
echo "VITIS_SYSROOTS=$VITIS_SYSROOTS exists"
else
  echo "VITIS_SYSROOTS=$VITIS_SYSROOTS does not exist"
  error=$((error+1))
fi

if [ -d "$VITIS_PLATFORM_DIR" ]; then
echo "VITIS_PLATFORM_DIR=$VITIS_PLATFORM_DIR exists"
else
  echo "VITIS_PLATFORM_DIR=$VITIS_PLATFORM_DIR does not exist"
  error=$((error+1))
fi
if [ $error > 0 ];
then
    echo "$error variables not set correctly";
else
    echo "All Variables checked OK";
fi;



rm -rf 0
