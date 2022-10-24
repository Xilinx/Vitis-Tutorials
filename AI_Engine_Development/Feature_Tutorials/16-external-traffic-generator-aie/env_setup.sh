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


export PYTHON3_LOCATION=<user-path>


######################## DO NOT CHANGE BELOW LINES ##########################

# =========================================================
# Platform Selection...
# =========================================================

export TOOLS_VERSION=`basename $XILINX_VITIS`

#Print env variables
echo "Tools Version      : "$TOOLS_VERSION
echo "XILINX_VITIS       : "$XILINX_VITIS
echo "XILINX_VIVADO      : "$XILINX_VIVADO
echo "XILINX_HLS         : "$XILINX_HLS
echo "XILINX_XRT         : "$XILINX_XRT
echo "XILINX_VERSAL_SW   : "$XILINX_VERSAL_SW


# Python setup
export PYTHONPATH=${XILINX_VIVADO}/data/emulation/hw_em/lib/python/:${XILINX_VIVADO}/data/emulation/ip_utils/xtlm_ipc/xtlm_ipc_v1_0/python/:$XILINX_VIVADO/data/emulation/python/xtlm_ipc:${PYTHONPATH}
export PATH=$PYTHON3_LOCATION:$PATH

# LIBRARY_PATH setting
export LIBRARY_PATH=$LD_LIBRARY_PATH:$XILINX_VIVADO/lib/lnx64.o:$XILINX_VIVADO/data/simmodels/xsim/$TOOLS_VERSION/lnx64/6.2.0/ext/protobuf:/usr/lib/x86_64-linux-gnu
export LD_LIBRARY_PATH=$LIBRARY_PATH
