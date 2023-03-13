#!/bin/bash

# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

######################################################################################
# WARNING: THIS SCRIPT IS VALID ONLY FOR VCK190 on AMD/XILINX GUUP (LINUX) SERVERS
######################################################################################

# setup GUUP Vitis / Petalinux environment
ts 2023.1_daily_latest -petalinux petalinux-v2023.1_daily_latest

export CUR_DIR=$PWD

# global variables
export WRK_DIR=/group/xirguup/danieleb/ide2023.1/Vitis-Tutorials-2023.1/Developer_Contributed
export TUTORIAL=03-HLS_Code_Optimization
export BOARD=vck190
export NUM_BOARD=202310_1
# Vitis DSP Library
export DSPLIB_ROOT=/group/xirguup/danieleb/ide2022.2/Vitis_Libraries/dsp


# to emulate SPRITE on GUUP
cd ${WRK_DIR}/Vitis-Tutorials-Admin-2023.1_next
source env_setup_petalinux_versal.sh

# PETALINUX and VERSAL COMMON IMAGE
export COMMON_IMAGE_VERSAL=${HOME}/common/xilinx-versal-common-v2023.1

# install Versal SDK on $HOME/common
echo " "
echo "NOW INSTALLING VCK190 SDK in $HOME/common "
echo " "
export VERSAL_COMMON_2023v1=$PETALINUX/../../bsp/internal/xilinx-versal-common-v2023.1/
# clean versal folder content
rm -rf ${COMMON_IMAGE_VERSAL}
# install SDK
cd ${VERSAL_COMMON_2023v1}
bash ./sdk.sh -y -p -d ${COMMON_IMAGE_VERSAL}
cp bl31.elf boot.scr Image README.txt rootfs.ext4 rootfs.manifest u-boot.elf ${COMMON_IMAGE_VERSAL}/

cd ${WRK_DIR}/${TUTORIAL}/files


# SYSROOT, *PLATFORM*, ROOTFS, IMAGE
export VITIS_SYSROOTS=${COMMON_IMAGE_VERSAL}/sysroots/cortexa72-cortexa53-xilinx-linux
export SDKTARGETSYSROOT=${VITIS_SYSROOTS}

export BOARD=vck190
export VERSAL_VITIS_PLATFORM=xilinx_${BOARD}\_base_202310_1
export VITIS_PLATFORM_DIR=${PLATFORM_REPO_PATHS}/${VERSAL_VITIS_PLATFORM}
export VITIS_PLATFORM_XPFM=${VITIS_PLATFORM_DIR}/${VERSAL_VITIS_PLATFORM}.xpfm
export KERNEL_IMAGE=${COMMON_IMAGE_VERSAL}/Image
export ROOTFS=${COMMON_IMAGE_VERSAL}/rootfs.ext4


# check env variables
echo " "
echo "Vitis/Petalinux GUUP Environmental variables"
echo " "
echo "LD_LIBRARY_PATH     "  $LD_LIBRARY_PATH
echo "XILINX_VITIS        "  $XILINX_VITIS
echo "BSP_DIR             "  $BSP_DIR
#echo "PETALINUX_VER       "  $PETALINUX_VER
echo "PLATFORM_REPO_PATHS "  $PLATFORM_REPO_PATHS
echo "XILINX_XRT          "  $XILINX_XRT
echo "XILINX_VIVADO       "  $XILINX_VIVADO
#echo "XILINXD_LICENSE_FILE " $XILINXD_LICENSE_FILE
echo "XILINX_HLS          "  $XILINX_HLS
#echo "XSCT_TOOLCHAIN"        $XSCT_TOOLCHAIN
#echo "XDG_DATA_DIRS "        $XDG_DATA_DIRS
#echo "GUUP_ROOT "            $GUUP_ROOT
echo "SHELL               "   $SHELL
#echo "LM_LICENSE_FILE "      $LM_LICENSE_FILE
#echo "PYTHONPATH "           $PYTHONPATH
#echo "PETALINUX_MAJOR_VER "  $PETALINUX_MAJOR_VER
echo "PETALINUX            "  $PETALINUX
echo "SDKTARGETSYSROOT     "  ${VITIS_SYSROOTS}
echo "VERSAL_COMMON_2023v1 "  ${VERSAL_COMMOM_2023v1}
echo "COMMON_IMAGE_VERSAL  "  ${COMMON_IMAGE_VERSAL}
echo "SDKTARGETSYSROOT     "  ${VITIS_SYSROOTS}
echo "VITIS_PLATFORM_XPFM  "  ${VITIS_PLATFORM_XPFM}
echo "KERNEL_IMAGE         "  ${KERNEL_IMAGE}
echo "ROOTFS               "  ${ROOTFS}
echo "VITIS_PLATFORM_XPFM  "  ${VITIS_PLATFORM_XPFM}
echo "PLATFORM_REPO_PATHS  "  ${PLATFORM_REPO_PATHS}
#echo "PATH " ${PATH}
