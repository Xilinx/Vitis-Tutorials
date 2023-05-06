#!/bin/bash

# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# ZCU102
# set the environment (GUUP) for ZCU102
#source ${PWD}/scripts/desktop_zcu102_sample_env_setup.sh  # XILINX-AMD GUUP SERVER
source ${PWD}/scripts/guupServer_zcu102_sample_env_setup.sh

# run all HLS examples
cd ${PWD}/examples
make clean
make hls_kernels

# Vitis Acceleration designs
cd ${PWD}/examples

# vector add
make vadd_zclean
make vadd_zcu102

# matrix mult
#make mmult_zclean
make mmult_zcu102

# to finish
cd ..
