#!/bin/bash

# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# Author: Daniele Bagni
# Date:    14 Feb 20223

######################################################################################
# WARNING: THIS SCRIPT IS VALID ONLY FOR VCK190 on AMD/XILINX GUUP (LINUX) SERVERS
######################################################################################

# =======================================================
# Setup 2022.2 Vitis / Petalinux env. in GUUP Servers
# =======================================================
##ts 2022.2_daily_latest -petalinux petalinux-v2022.2_daily_latest

# ==========================================================
# Validating Tool Installation
# ==========================================================
echo ""
echo "Aiecompiler:"
which aiecompiler
echo ""
echo "Vivado:"
which vivado
echo ""
echo "Vitis:"
which vitis
echo ""
echo "Vitis HLS:"
which vitis_hls
echo ""
echo ""
echo "PETALINUX"
echo "$PETALINUX"
echo ""

# =======================================================
# global variables for this tutorial
# =======================================================
RELEASE=2022.2
BOARD=vck190
#BASE_NUM=202310_1
 BASE_NUM=202220_1

TUTORIAL=02-AIE_DSP_with_Makefile_and_GUI
 
#export WRK_DIR=/group/xirguup/danieleb/ide2023.1/Vitis-Tutorials-2023.1/Developer_Contributed #only for GUUP
export WRK_DIR=/media/danieleb/DATA/${RELEASE}/Vitis-Tutorials-${RELEASE}/Developer_Contributed/${TUTORIAL}


# =======================================================
# Set Platform, Vitis and Petalinux repo
# =======================================================
##export PLATFORM_REPO_PATHS = <YOUR-${RELEASE}-PLATFORM-DIRECTORY>
#export PLATFORM_REPO_PATHS=/proj/rdi/xbuilds/2023.1_daily_latest/internal_platforms #only fr GUUP
export PLATFORM_REPO_PATHS=/media/danieleb/DATA/${RELEASE}/tools/Xilinx/Vitis/${RELEASE}/base_platforms

##export XILINX_VITIS = <YOUR-2023.1-VITIS-DIRECTORY>/2023.1
#export XILINX_VITIS=/proj/xbuilds/SWIP/2023.1_0211_1903/installs/lin64/Vitis/2023.1 #only fr GUUP
export XILINX_VITIS=/media/danieleb/DATA/${RELEASE}/tools/Xilinx/Vitis/${RELEASE}

##export PETALINUX = <YOUR-2023.1-PETALINUX-DIRECTORY>
#export PETALINUX=/proj/petalinux/2023.1/petalinux-v2023.1_02111228/tool/petalinux-v2023.1-final #only for GUUP
export PETALINUX=/tools/Xilinx/PetaLinux/${RELEASE}

# ==================================================================================
# Set Versal Common Image repo (OR eventually install it into /tmp)
# This local foldr must not be a "NFS" hard disk drive, only "ext4" is supported
# ==================================================================================
# export TMP DIRECTORY=<YOUR-EXT4-TMP-(NO-NFS)-DIRECTORY>
export TMPDIR=/tmp/danieleb
mkdir -p ${TMPDIR}/common

##export COMMON_IMAGE_VERSAL=<YOUR-XILINX-VERSAL-COMMON-V2023.1-DIRECTORY>
export COMMON_IMAGE_VERSAL=${TMPDIR}/common/xilinx-versal-common-v${RELEASE}

# clean versal common image local folder content
rm -rf ${COMMON_IMAGE_VERSAL}
mkdir -p ${COMMON_IMAGE_VERSAL}

# set the folder of Versal Common Image source files
#VERSAL_COMMON_2023v1=${PETALINUX}/../../bsp/internal/xilinx-versal-common-v2023.1/ #only for GUUP
VERSAL_COMMON_2023v1=/opt/xilinx/common/xilinx-versal-common-v${RELEASE}
cd ${VERSAL_COMMON_2023v1}
# install Versal SDK on $HOME/common
./sdk.sh -y -p -d ${COMMON_IMAGE_VERSAL}
#copy the other files
cp bl31.elf boot.scr Image README.txt rootfs.ext4 rootfs.manifest u-boot.elf ${COMMON_IMAGE_VERSAL}/
#back to the wrk dir
cd ${WRK_DIR}

# ====================================================
# Source Versal Image, Vitis and Aietools
# ====================================================
unset LD_LIBRARY_PATH
source $COMMON_IMAGE_VERSAL/environment-setup-cortexa72-cortexa53-xilinx-linux
# Run the below command to setup environment and CXX
source $XILINX_VITIS/settings64.sh

# ========================================================
# Set DSP Library for Vitis
# ========================================================
##export DSPLIB_ROOT=<Path to DSP Libs - Directory>
#export DSPLIB_ROOT=/group/xirguup/danieleb/ide${RELEASE}/Vitis_Libraries/dsp #only for GUUP
export DSPLIB_VITIS=/group/xirguup/danieleb/ide${RELEASE}/Vitis_Libraries
export DSPLIB_ROOT=${DSPLIB_VITIS}/dsp

#==========================================================
# Board/Platform Selection...
# =========================================================
## tgt_plat=xilinx_vck190_base_202220_1
## export PLATFORM=$PLATFORM_REPO_PATHS/$tgt_plat/$tgt_plat\.xpfm

export VERSAL_VITIS_PLATFORM=xilinx_${BOARD}\_base_${BASE_NUM}
export VITIS_PLATFORM_DIR=${PLATFORM_REPO_PATHS}/${VERSAL_VITIS_PLATFORM}
export VITIS_PLATFORM_XPFM=${VITIS_PLATFORM_DIR}/${VERSAL_VITIS_PLATFORM}.xpfm

# ========================================================
# Set SysRoot, RootFS and Image
# ========================================================
export VITIS_SYSROOTS=${COMMON_IMAGE_VERSAL}/sysroots/cortexa72-cortexa53-xilinx-linux
export SDKTARGETSYSROOT=${VITIS_SYSROOTS}
export KERNEL_IMAGE=${COMMON_IMAGE_VERSAL}/Image
export ROOTFS=${COMMON_IMAGE_VERSAL}/rootfs.ext4

# ========================================================
# for compatibility with official Vitis Tutorials
# ========================================================
export XLNX_VERSAL=${COMMON_IMAGE_VERSAL}
export PLATFORM=$VITIS_PLATFORM_XPFM

# ========================================================
# check env variables
# ========================================================
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


