#!/bin/bash
#
# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Authors: Peter Schillinger, Daniele Bagni  Xilinx Inc
# Date:    5 October 2022



# COMMON ZCU102  "SDK" settings
# ************************************************************************************************
# ZCU102 common sw package
export EDGE_COMMON_SW=/opt/xilinx/common/xilinx-zynqmp-common-v2022.1

unset LD_LIBRARY_PATH
source ${EDGE_COMMON_SW}/environment-setup-cortexa72-cortexa53-xilinx-linux
# ******************************************************************************************************



# ******************************************************************************************************
# VITIS ENVIRONMENT SETTINGS
# ******************************************************************************************************

# FOR VITIS AIE TUTORIALS
export XILINX_TOOLS_LOCATION=/tools/Xilinx/Vitis/2022.1


# set the Vitis/Vivado 2021.2 release
source ${XILINX_TOOLS_LOCATION}/settings64.sh

# set petalinux 2021.2 release
source /petalinux_2022.1/settings.sh


# working directory: change with your own
export WRK_DIR=/media/danieleb/DATA/2022.1/tutorials

# for Vitis AI 2.5: change with your own or leave commented if you do not have it
#export VITIS_AI_PATH=/media/danieleb/DATA/VAI2.5

# for the HLS kernels and host applications
#export MPSOCDPU_PRE_POST_PL_ACC=${WRK_DIR}/MPSOCDPU-PRE-POST-PL-ACC/files

# for the necessary included files and libraries of host_apps
#export TRD_HOME=/media/danieleb/DATA/MPSOCDPU-PRE-POST-PL-ACC/files/dpu_trd


# for VCK190P common sw package
export VITIS_SYSROOTS=${EDGE_COMMON_SW}/sysroots/cortexa72-cortexa53-xilinx-linux
export SYSROOT=${VITIS_SYSROOTS}


export XILINX_XRT=/opt/xilinx/xrt #DB
#cd ${XILINX_XRT}
source ${XILINX_XRT}/setup.sh
#cd ${WRK_DIR}

# for common zcu102 platform
export VITIS_PLATFORM=xilinx_zcu102_base_202210_1
#export VITIS_PLATFORM_DIR=${XILNX_TOOLS_LOCATION}/base_platforms/${VITIS_PLATFORM}
#export VITIS_PLATFORM_PATH=${VITIS_PLATFORM_DIR}
#/${VITIS_PLATFORM}.xpfm
export VITIS_PLATFORM_PATH=${XILINX_TOOLS_LOCATION}/base_platforms
#/${VITIS_PLATFORM}
export PLATFORM_REPO_PATHS=${VITIS_PLATFORM_PATH}
#/${VITIS_PLATFORM}.xpfm
#export SDX_PLATFORM=${VITIS_PLATFORM}


# for Model Composer
export XILINX_LOC=${XILINX_TOOLS_LOCATION}
#$XILINX_LOC/../../Model_Composer/2022.1/bin/model_composer -matlab /usr/local/MATLAB/R2021a -vitis $XILINX_LOC

# FOR VITIS DSP LIBRARIES
export DSPLIB_ROOT=/media/danieleb/DATA/2022.1/Vitis_Libraries/dsp
plat=${VITIS_PLATFORM}
#xilinx_vck190_base_202210_1
export PLATFORM=${PLATFORM_REPO_PATHS}/${plat}/${plat}.xpfm


#export PLATFORM_REPO_PATHS=/opt/xilinx/platforms/xilinx_vck190_base_202120_1
#export SYSROOT=${VITIS_SYSROOTS} #already done
export K_IMAGE=${EDGE_COMMON_SW}/Image
export ROOTFS=${EDGE_COMMON_SW}/rootfs.ext4
export IMAGE=${K_IMAGE}

#some basic checking
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

if [ -d "$VITIS_PLATFORM_PATH" ]; then
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
