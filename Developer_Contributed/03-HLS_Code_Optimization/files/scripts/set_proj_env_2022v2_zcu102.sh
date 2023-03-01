#!/bin/bash

# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Authors: Peter Schillinger, Daniele Bagni  Xilinx Inc
# Date:    27 Jan 2023

RELEASE=2022.2
export DATA_DIR=/media/danieleb/DATA/$RELEASE

# ******************************************************************************************************
# COMMON ZCU102  package
# ************************************************************************************************
# ZCU102 common sw package
export COMMON_IMAGE_ZYNQMP=/opt/xilinx/common/xilinx-zynqmp-common-v$RELEASE

unset LD_LIBRARY_PATH
source ${COMMON_IMAGE_ZYNQMP}/environment-setup-cortexa72-cortexa53-xilinx-linux

# ******************************************************************************************************
# VITIS ENVIRONMENT SETTINGS
# ******************************************************************************************************

# FOR VITIS AIE TUTORIALS
export XILINX_VITIS=$DATA_DIR/tools/Xilinx/Vitis/$RELEASE

# set the Vitis/Vivado release
source ${XILINX_VITIS}/settings64.sh

# set petalinux release
source /tools/Xilinx/PetaLinux/2022.2/settings.sh

# working directory: change with your own
export WRK_DIR=$DATA_DIR/Vitis-Tutorials-$RELEASE/Developer_Contributed

# for Vitis AI 2.5: change with your own or leave commented if you do not have it
export VITIS_AI_PATH=/media/danieleb/DATA/VAI2.5

# for ZCU102 common sw package
export VITIS_SYSROOTS=${COMMON_IMAGE_ZYNQMP}/sysroots/cortexa72-cortexa53-xilinx-linux
export SYSROOT=${VITIS_SYSROOTS}

# XRT
export XILINX_XRT=/opt/xilinx/xrt #DB
source ${XILINX_XRT}/setup.sh

# for common zcu102 platform
export ZYNQMP_VITIS_PLATFORM=xilinx_zcu102_base_202220_1
export VITIS_PLATFORM_DIR=${XILINX_VITIS}/base_platforms/${ZYNQMP_VITIS_PLATFORM}
export VITIS_PLATFORM_PATH=${VITIS_PLATFORM_DIR}/${ZYNQMP_VITIS_PLATFORM}.xpfm
export PLATFORM_REPO_PATHS=${XILINX_VITIS}/base_platforms
#xilinx_zcu102_base_202210_1
export PLATFORM=${PLATFORM_REPO_PATHS}/${ZYNQMP_VITIS_PLATFORM}/${ZYNQMP_VITIS_PLATFORM}.xpfm

# for Model Composer
#$XILINX_VITIS/../../Model_Composer/2022.2/bin/model_composer -matlab /usr/local/MATLAB/R2021a -vitis $XILINX_VITIS

# FOR VITIS DSP LIBRARIES
export DSPLIB_ROOT=$DATA_DIR/Vitis_Libraries/dsp

# SysRoot and Image
export K_IMAGE=${EDGE_COMMON_SW}/Image
export ROOTFS=${EDGE_COMMON_SW}/rootfs.ext4
export IMAGE=${K_IMAGE}


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
'

if [ -d "$VITIS_AI_PATH" ]; then
  echo "VITIS_AI_PATH=$VITIS_AI_PATH exists"
else
  echo "VITIS_AI_PATH=$VITIS_AI_PATH does not exist"
  error=$((error+1))
fi

: '
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

if [ -f "$VITIS_PLATFORM_PATH" ]; then
echo "VITIS_PLATFORM_PATH=$VITIS_PLATFORM_PATH exists"
else
  echo "VITIS_PLATFORM_PATH=$VITIS_PLATFORM_PATH does not exist"
  error=$((error+1))
fi

if [ $error > 0 ];
then
    echo "$error variables not set correctly";
else
    echo "All Variables checked OK";
fi;



rm -rf 0
