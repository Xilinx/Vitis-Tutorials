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

# Launch emulation
./package/launch_sw_emu.sh

