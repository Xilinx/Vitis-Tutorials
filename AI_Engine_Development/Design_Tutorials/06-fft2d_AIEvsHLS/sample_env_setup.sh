#!/bin/bash

# © Copyright 2021–2022 Xilinx, Inc.
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

export PLATFORM_REPO_PATHS=<YOUR-PLATFORMS-DIRECTORY>
export XILNX_TOOLS_LOCATION=<Path to Vitis Build - Directory>/Vitis/2022.1
export XLNX_VERSAL=<Path to xilinx-versal-common-v2022.1 - Directory>

# Optionally Setup XRT_ROOT, pointing to XRT RPMs,
# if not set it will automatically be excluded from v++ packaging...
#export XRT_ROOT=<Path to XRT RPMs - Directory>

# Set the path to the VITIS LIBs, required only for AIE designs using Vitis DSP Libraries...
# DSPLIB_ROOT variable is set in Makefile using DSPLIB_VITIS variable set below:
export DSPLIB_VITIS=<Path to Vitis Libraries - Directory>

# Setup SDKTARGETSYSROOT and CXX...
unset LD_LIBRARY_PATH
source $XLNX_VERSAL/environment-setup-cortexa72-cortexa53-xilinx-linux

# Source XRT, VITIS, and Aietools...
source $XILNX_TOOLS_LOCATION/settings64.sh

# =========================================================
# Platform Selection...
# =========================================================
tgt_plat=xilinx_vck190_base_202210_1
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
