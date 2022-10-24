#!/bin/bash

# © Copyright 2022 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Make sure the SHELL is in /bin/bash if not do
# echo $SHELL
# export SHELL=/bin/bash
# echo $SHELL

unset LD_LIBRARY_PATH
export PLATFORM_REPO_PATHS=<PATH-TO-PLATFORMS-DIRECTORY>
export XILINX_VITIS=<PATH-TO-VITIS-BUILD-DIRECTORY>/Vitis/2022.2
export XILINX_VERSAL=<PATH TO xilinx-versal-common-v2022.2 - DIRECTORY>
export PATH_TO_BSP=<PATH TO xilinx-petalinux-DIRECTORY>/2022.2_next/petalinux_regression_latest/bsp/release
export BSP=xilinx-vck190-v2022.2-final.bsp

# Setup SDKTARGETSYSROOT and CXX...
source $XILINX_VERSAL/environment-setup-cortexa72-cortexa53-xilinx-linux

# Source XRT, VITIS, and Aietools...
source $XILINX_VITIS/settings64.sh

#To source petalinux: 
source <PATH TO petalinux-v2022.2-final>/settings.sh

# =========================================================
# Platform Selection...
# =========================================================
tgt_plat=xilinx_vck190_base_202220_1
export PLATFORM=$PLATFORM_REPO_PATHS/$tgt_plat/$tgt_plat\.xpfm

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
