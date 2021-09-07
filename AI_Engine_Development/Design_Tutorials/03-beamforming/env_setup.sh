# © Copyright 2020 Xilinx, Inc.
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
export XILINX_XRT=<XRT-LOCATION>
export PATH_TO_BSP=<PATH-TO-BSPs>

#Source setup scripts
source <XILNX-TOOLS-LOCATION>/Vitis/<TOOLS-BUILD>/settings64.sh 
source <PETALINUX-TOOLS-LOCATION>/settings.sh 
source $XILINX_XRT/setup.sh

#Print env variables
echo "XILINX_VITIS       : "$XILINX_VITIS
echo "XILINX_VIVADO      : "$XILINX_VIVADO
echo "XILINX_HSL         : "$XILINX_HLS
echo "PETALINUX          : "$PETALINUX
echo "PATH_TO_BSP        : "$PATH_TO_BSP
echo "XILINX_XRT         : "$XILINX_XRT
