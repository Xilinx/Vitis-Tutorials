#!/bin/bash

# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


# where you are
echo " "
echo "STARTING DIR IS: " $PWD
echo " "

# clean file text formatting
source ${PWD}/scripts/dos2unix_all.sh

## ZCU102
source ${PWD}/scripts/zcu102_run_all.sh  # XILINX-AMD GUUP SERVER

## VCK190
source ${PWD}/scripts/vck190_run_all.sh  # XILINX-AMD GUUP SERVER
