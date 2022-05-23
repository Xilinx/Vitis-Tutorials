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

#Setup environment variables 
export PLATFORM_REPO_PATHS=/proj/xbuilds/2022.1_daily_latest/internal_platforms
export XILNX_TOOLS_LOCATION=/proj/xbuilds/2022.1_daily_latest/installs/lin64/Vitis/HEAD
export PATH_TO_BSP=/proj/petalinux/2022.1/petalinux-v2022.1_daily_latest/bsp/release
export PETALINUX_TOOLS_LOCATION=/proj/petalinux/2022.1/petalinux-v2022.1_daily_latest/tool/petalinux-v2022.1-final
export XILINX_XRT=/proj/xbuilds/2022.1_daily_latest/xbb/xrt/packages
export XILINXD_LICENSE_FILE=2100@aiengine-eng

#Source setup scripts
source $XILNX_TOOLS_LOCATION/settings64.sh
source $PETALINUX_TOOLS_LOCATION/settings.sh 
source $XILINX_XRT/setenv.sh

#Print env variables
echo "XILINX_VITIS       : "$XILINX_VITIS
echo "XILINX_VIVADO      : "$XILINX_VIVADO
echo "XILINX_HSL         : "$XILINX_HLS
echo "PETALINUX          : "$PETALINUX
echo "PATH_TO_BSP        : "$PATH_TO_BSP
echo "XILINX_XRT         : "$XILINX_XRT
