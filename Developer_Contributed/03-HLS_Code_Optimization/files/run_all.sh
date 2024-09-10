#!/bin/bash

# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


# where you are
echo " "
echo "STARTING DIR IS: " $PWD
echo " "

# clean file text formatting
rm -f ${PWD}/scripts/*~
rm -f ${PWD}/examples/*~
rm -f ${PWD}/*~
rm -f ${PWD}/*.log
rm -f ${PWD}/scripts/#*

: '
dos2unix ${PWD}/run_all.sh
source ${PWD}/scripts/dos2unix_all.sh


export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu

## ZCU102
source ${PWD}/scripts/zcu102_run_all.sh  # XILINX-AMD GUUP SERVER

## VCK190
source ${PWD}/scripts/vck190_run_all.sh  # XILINX-AMD GUUP SERVER
'
