#!/bin/bash

# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# clean file text formatting
##source ./scripts/dos2unix_all.sh
#echo " "
#echo "CURRENT DIR IS: " $PWD
#echo " "

# set the environment (GUUP) for VCK190
#source ${PWD}/scripts/desktop_vck190_sample_env_setup.sh  # XILINX-AMD GUUP SERVER
source ${PWD}/scripts/guupServer_vck190_sample_env_setup.sh

cd ${PWD}/examples
#make mmult_vclean
make mmult_vck190

# to finish
cd ..
