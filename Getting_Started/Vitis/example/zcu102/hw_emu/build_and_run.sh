#!/bin/bash

# Exit when any command fails
set -e

if [[ -z "$ROOTFS" ]]; then
   echo "Error: make sure to set the ROOTFS environment variable"
   exit
fi
if [[ -z "$SYSROOT" ]]; then
   echo "Error: make sure to set the SYSROOT environment variable"
   exit
fi
if [[ -z "$PLATFORM_REPO_PATHS" ]]; then
   echo "Error: make sure to set the PLATFORM_REPO_PATHS environment variable"
   exit
fi

# Make sure everything is up to date
make all 

# Setup the environment for QEMU and launch emulation
unset LD_LIBRARY_PATH
source $XILINX_VITIS/data/emulation/qemu/unified_qemu_v4_2/environment-setup-aarch64-xilinx-linux
./package/launch_hw_emu.sh

