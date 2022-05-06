#  (c) Copyright 2022 Xilinx, Inc.
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
#!/bin/bash

#Setup environment variables

export XILINX_TOOLS_LOCATION=<Path to Vitis Build - Directory>/Vitis/2022.1
export PLATFORM_REPO_PATHS=<YOUR-PLATFORMS-DIRECTORY>
export XILINX_VERSAL_SW=<Path to xilinx-versal-common-v2022.1 - Directory>
export XILINX_XRT=/<user-path>/opt/xilinx/xrt
export PYTHON3_LOCATION=<user-path>


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
echo "XILINX_HLS         : "$XILINX_HLS
echo "XILINX_XRT         : "$XILINX_XRT
echo "XILINX_VERSAL_SW   : "$XILINX_VERSAL_SW


# Python setup
export PYTHONPATH=${XILINX_VIVADO}/data/emulation/hw_em/lib/python/:${XILINX_VIVADO}/data/emulation/ip_utils/xtlm_ipc/xtlm_ipc_v1_0/python/:${PYTHONPATH}
export PATH=$PYTHON3_LOCATION:$PATH

export LD_LIBRARY_PATH_SAVE=$LD_LIBRARY_PATH
unset LD_LIBRARY_PATH

export ROOTFS=$XILINX_VERSAL_SW/rootfs.ext4
export IMAGE=$XILINX_VERSAL_SW/Image
export SYSROOT=$XILINX_VERSAL_SW/sysroots/cortexa72-cortexa53-xilinx-linux

# Setup SDKTARGETSYSROOT and CXX...
#source $XLNX_VERSAL/environment-setup-cortexa72-cortexa53-xilinx-linux
export SDKTARGETSYSROOT=$XILINX_VERSAL_SW/sysroots/cortexa72-cortexa53-xilinx-linux
export CXX="aarch64-linux-gnu-g++  -mcpu=cortex-a72.cortex-a53 -march=armv8-a+crc -fstack-protector-strong  -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=$SDKTARGETSYSROOT"


# LIBRARY_PATH setting
export LIBRARY_PATH=$LD_LIBRARY_PATH_SAVE:$XILINX_VIVADO/lib/lnx64.o:$XILINX_VIVADO/data/simmodels/xsim/$TOOLS_VERSION/lnx64/6.2.0/ext/protobuf:/usr/lib/x86_64-linux-gnu
export LD_LIBRARY_PATH=$LIBRARY_PATH
