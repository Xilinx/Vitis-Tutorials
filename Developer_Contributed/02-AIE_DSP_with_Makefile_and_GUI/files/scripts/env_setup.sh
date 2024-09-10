#!/bin/bash

# Report current shell
current_shell=$(echo $0)
echo "[env_setup.sh] Current shell: $current_shell"
if [ "$current_shell" = "-bash" ]; then
    # Print Bash version
    echo "[env_setup.sh] Bash version: $BASH_VERSION"
fi

# Vitis Libraries
#export DSPLIB_VITIS=/proj/xtools/xilinx_central_repo/Vitis_Libraries
# Point to the Vitis Libraries HEAD
# DB: commented by me
#export DSPLIB_VITIS=/proj/xtools/xilinx_central_repo/FaaSApps_next
#export DSPLIB_ROOT=${DSPLIB_VITIS}/dsp

# GHE Repo URL for CI
export DEVICE_TREE_REPO=https://gitenterprise.xilinx.com/Linux/device-tree-xlnx.git
export ATF_REPO=https://gitenterprise.xilinx.com/Linux/arm-trusted-firmware.git
export UBOOT_REPO=https://github.com/Xilinx/u-boot-xlnx
#REPO for Embd_Sw/F/01
export ATF_CHECKOUT=master
export UBOOT_CHECKOUT=master

# Add CMake to PATH
export PATH=${XILINX_VITIS}/tps/lnx64/cmake-3.3.2/bin:$PATH

# Add git to PATH
export PATH=${XILINX_VITIS}/tps/lnx64/git-2.41.0/bin:$PATH

# Update Perl Version
source perl.sh

