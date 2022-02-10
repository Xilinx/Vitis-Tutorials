#  (c) Copyright 2021 Xilinx, Inc.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License. 

#Setup environment variables 
export PLATFORM_REPO_PATHS=<YOUR-PLATFORMS-DIRECTORY>
export XIILNX_TOOLS_LOCATION=<Path to Vitis Build - Directory>/Vitis/2021.2
export XLNX_VERSAL=<Path to xilinx-versal-common-v2021.2 - Directory>

# Optionally Setup XRT_ROOT, pointing to XRT RPMs,
# if not set it will automatically be excluded from v++ packaging...
#export XRT_ROOT=<Path to XRT RPMs - Directory>


# Setup SDKTARGETSYSROOT and CXX...
#source $XLNX_VERSAL/environment-setup-cortexa72-cortexa53-xilinx-linux
export SDKTARGETSYSROOT=$XLNX_VERSAL/sysroots/cortexa72-cortexa53-xilinx-linux
export CXX="aarch64-linux-gnu-g++  -mcpu=cortex-a72.cortex-a53 -march=armv8-a+crc -fstack-protector-strong  -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=$SDKTARGETSYSROOT"


# Source XRT, VITIS, and Aietools...
source $XIILNX_TOOLS_LOCATION/settings64.sh

# =========================================================
# Platform Selection...
# =========================================================
tgt_plat=xilinx_vck190_base_202120_1
export PLATFORM=$PLATFORM_REPO_PATHS/$tgt_plat/$tgt_plat\.xpfm


#Print env variables
echo "XILINX_VITIS       : "$XILINX_VITIS
echo "XILINX_VIVADO      : "$XILINX_VIVADO
echo "XILINX_HLS         : "$XILINX_HLS
echo "XILINX_XRT         : "$XILINX_XRT
