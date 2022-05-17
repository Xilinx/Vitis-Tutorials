#  (c) Copyright 2021-2022 Xilinx, Inc.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.


export XILINX_TOOLS_LOCATION=<Path to Vitis Build - Directory>/Vitis/2022.1
export PLATFORM_REPO_PATHS=<YOUR-PLATFORMS-DIRECTORY>
export DSPLIB_ROOT=<PATH-TO-DSP-LIBRARY>


######################## DO NOT CHANGE BELOW LINES ##########################

# =========================================================
# Platform Selection...
# =========================================================
export PFM_NAME=xilinx_vck190_base_202210_1


# Source XRT, VITIS, and Aietools...
source $XILINX_TOOLS_LOCATION/settings64.sh


export TOOLS_VERSION=`basename $XILINX_VITIS`

#Print env variables
echo "Tools Version      : "$TOOLS_VERSION
echo "XILINX_VITIS       : "$XILINX_VITIS
echo "XILINX_VIVADO      : "$XILINX_VIVADO
echo "DSPLIB_ROOT        : "$DSPLIB_ROOT
