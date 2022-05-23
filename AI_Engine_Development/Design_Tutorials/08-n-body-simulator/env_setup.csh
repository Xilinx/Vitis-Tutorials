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

#setenv XILINX_XRT=/<user-path>/opt/xilinx/xrt
setenv XILNX_TOOLS_LOCATION /proj/xbuilds/2022.1_daily_latest/installs/lin64/Vitis/HEAD
setenv PLATFORM_REPO_PATHS /proj/xbuilds/2022.1_daily_latest/internal_platforms
setenv ROOTFS $PLATFORM_REPO_PATHS/sw/versal/xilinx-versal-common-v2022.1/rootfs.ext4
setenv IMAGE $PLATFORM_REPO_PATHS/sw/versal/xilinx-versal-common-v2022.1/Image
setenv EDGE_COMMON_SW $PLATFORM_REPO_PATHS/sw/versal/xilinx-versal-common-v2022.1
setenv XLNX_VERSAL /proj/xbuilds/2022.1_daily_latest/internal_platforms/sw/versal/xilinx-versal
setenv SDKTARGETSYSROOT $XLNX_VERSAL/sysroots/cortexa72-cortexa53-xilinx-linux
setenv CXX "aarch64-linux-gnu-g++  -mcpu=cortex-a72.cortex-a53 -march=armv8-a+crc -fstack-protector-strong  -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=$SDKTARGETSYSROOT"
# =========================================================
# PLATFORM SELECTION
# PLATFORM=xilinx_vck190_base_<ver> (default)
# PLATFORM=xilinx_vck190_es1_base_<ver>
# =========================================================
setenv PLATFORM xilinx_vck190_base_202210_1


#Source setup scripts
source $XILNX_TOOLS_LOCATION/settings64.csh
#source /<user-path>/Vitis/2021.2/settings64.sh
#source /<user-path>/petalinux-v2021.2-final/settings.sh
#source $XILINX_XRT/setup.sh

#Print env variables
echo "XILINX_VITIS       : "$XILINX_VITIS
echo "XILINX_VIVADO      : "$XILINX_VIVADO
echo "XILINX_HLS         : "$XILINX_HLS
#echo "PETALINUX          : "$PETALINUX
#echo "XILINX_XRT         : "$XILINX_XRT
echo "PLATFORM_REPO_PATHS: "$PLATFORM_REPO_PATHS
echo "SDKTARGETSYSROOT   : "$SDKTARGETSYSROOT
echo "CXX                : "$CXX
echo "EDGE_COMMON_SW     : "$EDGE_COMMON_SW

