#!/bin/bash

# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# =======================================================
# global variables for this tutorial
# =======================================================
#RELEASE=2022.2
RELEASE=2023.1
BOARD=vck190
#BASE_NUM=202220_1
BASE_NUM=202310_1


# =======================================================
# Available from Regression Framework
# =======================================================
#export PLATFORM_REPO_PATHS
#export XILINX_VITIS
#export XILINX_VIVADO
#export XILINX_HLS

# =======================================================
# Available from env_setup.h
# =======================================================
#export DSPLIB_VITIS
#export DSPLIB_ROOT=${DSPLIB_VITIS}/dsp

# =======================================================
# Available from env_setup_petalinux.h
# =======================================================
#export PETALINUX
#export BSP_DIR

# =======================================================
# Available from env_setup_petalinux_versal.h
# =======================================================
#export COMMON_IMAGE_VERSAL


#==========================================================
# Platform Selection...
# =========================================================
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
#export XLNX_VERSAL=${COMMON_IMAGE_VERSAL}
#export PLATFORM=${VITIS_PLATFORM_XPFM}

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
#echo "XILINX_XRT          "  $XILINX_XRT
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
#echo "VERSAL_COMMON_2023v1 "  ${VERSAL_COMMOM_2023v1}
echo "COMMON_IMAGE_VERSAL  "  ${COMMON_IMAGE_VERSAL}
echo "KERNEL_IMAGE         "  ${KERNEL_IMAGE}
echo "ROOTFS               "  ${ROOTFS}
echo "VITIS_PLATFORM_XPFM  "  ${VITIS_PLATFORM_XPFM}
echo "PLATFORM_REPO_PATHS  "  ${PLATFORM_REPO_PATHS}
echo "PLATFORM             "  ${PLATFORM}
echo "BASE_PLATFORM        "  ${BASE_PLATFORM}
echo "DEVICE               "  ${DEVICE}

#echo "PATH " ${PATH}
