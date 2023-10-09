#!/bin/bash
#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#




# =========================================================
# Do not change anything below this line unless necessary
# =========================================================

export TOOLS_VERSION=`basename $XILINX_VITIS`

# Python setup (libraries for external traffic generators)
export PYTHONPATH=${PWD}/Utils:${PYTHONPATH}
export PATH=${PWD}/Utils:$PATH



#Print env variables
echo "Tools Version      : "$TOOLS_VERSION
echo "XILINX_VITIS       : "$XILINX_VITIS
