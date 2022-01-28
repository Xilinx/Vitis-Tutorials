# Copyright 2021 Xilinx Inc.
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

#Environment Setup Script 
#Replace <user-path> with your appropriate tool locations 

export XILINX_XRT=/<user-path>/opt/xilinx/xrt
export PLATFORM_REPO_PATHS=<user-path>
export ROOTFS=$PLATFORM_REPO_PATHS/sw/versal/xilinx-versal-common-v2021.2/rootfs.ext4
export IMAGE=$PLATFORM_REPO_PATHS/sw/versal/xilinx-versal-common-v2021.2/Image
export EDGE_COMMON_SW=$PLATFORM_REPO_PATHS/sw/versal/xilinx-versal-common-v2021.2

# =========================================================
# PLATFORM SELECTION
# PLATFORM=xilinx_vck190_base_<ver> (default)
# PLATFORM=xilinx_vck190_es1_base_<ver>
# =========================================================
export PLATFORM=xilinx_vck190_base_202120_1


#Source setup scripts
source /<user-path>/Vitis/2021.2/settings64.sh
source /<user-path>/petalinux-v2021.2-final/settings.sh
source $PLATFORM_REPO_PATHS/sw/versal/xilinx-versal-common-v2021.2/environment-setup-cortexa72-cortexa53-xilinx-linux
source $XILINX_XRT/setup.sh

#Print env variables
echo "XILINX_VITIS       : "$XILINX_VITIS
echo "XILINX_VIVADO      : "$XILINX_VIVADO
echo "XILINX_HLS         : "$XILINX_HLS
echo "PETALINUX          : "$PETALINUX
echo "XILINX_XRT         : "$XILINX_XRT
echo "PLATFORM_REPO_PATHS: "$PLATFORM_REPO_PATHS
echo "SDKTARGETSYSROOT   : "$SDKTARGETSYSROOT
echo "CXX                : "$CXX
echo "EDGE_COMMON_SW     : "$EDGE_COMMON_SW
