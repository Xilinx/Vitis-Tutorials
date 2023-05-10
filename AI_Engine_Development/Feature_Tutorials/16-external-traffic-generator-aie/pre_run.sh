
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11


#!/bin/bash

#Setup environment variables

export XILINX_TOOLS_LOCATION=<Path to Vitis Build - Directory>/Vitis/2022.2
export PLATFORM_REPO_PATHS=<YOUR-PLATFORMS-DIRECTORY>
export XILINX_VERSAL_SW=<Path to xilinx-versal-common-v2022.2 - Directory>
export XILINX_XRT=/<user-path>/opt/xilinx/xrt


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
